# Error Handling & Building Fault Tolerant Systems — Lecture Notes

> **Core mindset:** Errors are not exceptions — they are a normal, expected part of running a backend. The question is not *whether* errors will happen, but *how prepared you are when they do*.

---

## 1. Types of Errors

### 1.1 Logic Errors

The most dangerous type. The app doesn't crash — it runs correctly but **produces wrong results**.

**Why dangerous:**
- Silent by nature — no alerts, no crashes
- Can go undetected for weeks or months
- Can cause financial damage (e.g., discount applied twice → negative shipping cost)

**Common causes:**
- Misunderstood requirements (ambiguous meetings → wrong implementation)
- Incorrect algorithm (miscalculation in a discount or pricing workflow)
- Unhandled edge cases in business logic

**What to watch for:** Sudden unexpected business metric changes (e.g., revenue drop, order anomalies).

---

### 1.2 Database Errors

Can bring the entire system down. Most backend apps are wholly dependent on their database.

| Error Type | Description | Common Cause |
|-----------|-------------|--------------|
| **Connection errors** | App cannot talk to the DB | Network issues, DB server overloaded, connection pool exhausted |
| **Constraint violations** | Operation breaks DB rules | Inserting duplicate email (unique violation), inserting with non-existent foreign key |
| **Query errors** | Malformed SQL | Typo in table/column name, query too complex → timeout |
| **Deadlocks** | Circular wait between transactions | Multiple operations waiting on each other's locks |

**Connection pooling:** Backends maintain a pool of open TCP connections to the DB to avoid TCP handshake overhead on every request. Exhausting the pool = connection errors.

---

### 1.3 External Service Errors

Every third-party dependency is a point of failure you don't control.

**Examples of external dependencies:** Payment processors, email providers (Resend, Mailgun), cloud storage (S3), auth providers (Auth0, Clerk), AI APIs (OpenAI).

**Failure modes:**

| Failure | Description | Strategy |
|---------|-------------|---------|
| **Network issues** | Timeouts, DNS failures, routing problems | Retry with exponential backoff |
| **Rate limiting** | Provider rejects with `429 Too Many Requests` | Exponential backoff; audit your call frequency |
| **Authentication errors** | Bad credentials, expired tokens | Verify credentials; implement token refresh |
| **Service outage** | Provider goes down entirely | Fallback mechanisms (cache, secondary node, degraded mode) |

**Exponential backoff pattern:**
```
Request fails → wait 1 min → retry
Still fails   → wait 2 min → retry
Still fails   → wait 4 min → retry
Still fails   → wait 8 min → retry
...up to max retries
```

Most downtime resolves in seconds/minutes — the task usually succeeds within 1–2 retries.

---

### 1.4 Input Validation Errors

User-caused errors. These are the **easiest to handle** — you know the rules, enforce them at the entry point.

| Validation Type | Example |
|----------------|---------|
| **Format** | Email must match `user@domain.tld` pattern; date must be `YYYY-MM-DD` |
| **Range** | Price must be > 0; string length between 5–500 chars; array must have 1–100 items |
| **Required fields** | Name field must be present to create a book |

Return `400 Bad Request` with clear field-level messages. These errors should never reach service or repository layers.

---

### 1.5 Configuration Errors

Occur when moving between environments (dev → staging → production).

**Example:** `OPENAI_API_KEY` added to `.env` during development, forgotten in production environment variables.

**Two outcomes:**

| Setup | Outcome |
|-------|---------|
| Validate env vars at startup | App fails to start → previous deployment keeps running (preferred) |
| No startup validation | App starts, fails at runtime when the missing key is accessed → users get `500` |

> **Best practice:** Validate all required environment variables before the server starts. Fail fast with a clear message. Never let a misconfiguration reach a live user.

---

## 2. Prevention: Proactive Error Detection

> **The best error handling starts before errors happen.**

### 2.1 Health Checks

**Basic health endpoint:**
```
GET /health → 200 OK   (server is running)
             500        (something is wrong)
```

This only tells you the server is alive. More comprehensive checks needed:

**Database health checks:**
- Test connectivity to the DB
- Run a representative query and measure response time
- Alert if queries that used to take 500ms now take 5s

**External service health checks:**
- Payment processors → run a test transaction
- Email providers → send a test email to an internal address
- Auth services → generate and validate a test token

**Core functionality checks (at startup):**
- All required environment variables are present
- Essential caches are populated
- Internal data structures are consistent

---

### 2.2 Monitoring & Observability (Overview)

> Deep dive in a future video — high-level principles here.

**What to monitor:**
- HTTP error rates (4xx, 5xx)
- Database errors and query latency
- External service failure rates
- **Business metrics** (sudden drop in successful transactions is often the first sign of a technical problem)

**Key insight:** Don't only track error rates. **Monitor performance degradation** — a system slowing down is often the warning sign before it breaks.

**Logging best practices:**
- Use structured JSON logs (parseable by tools like Grafana/Loki)
- Include correlation IDs and request IDs for tracing
- Never log sensitive data (see Security section below)

---

## 3. Error Response Strategies

### 3.1 Recoverable Errors

Errors that can be retried without human intervention.

**Examples:** Network timeouts, temporary DB connection failures, external API rate limits.

**Strategy:** Retry with exponential backoff. Be careful not to add more load to an already stressed system.

### 3.2 Non-Recoverable Errors

Errors that require giving up on the current operation.

**Examples:** Persistent external service outage, corrupted data, exhausted all retries.

**Strategy: Containment + Graceful Degradation**
- Switch to cached data
- Disable non-essential features
- Provide alternative functionality / fallback
- Contain the scope of damage — prevent one failure from cascading

---

## 4. Global Error Handler — The Final Safety Net

The single most important error handling mechanism in a backend app.

### Architecture

```
Request
  ↓
Routing → Handler (validation, deserialization)
             ↓
          Service (business logic)
             ↓
          Repository (DB queries)
             ↓
       ← errors bubble up ←
             ↓
Global Error Handler Middleware
             ↓
         Response
```

Every error — regardless of where it's thrown (repository, service, handler) — bubbles up to the global error handler. The middleware reads the error type and returns the appropriate HTTP response.

### Error Type → HTTP Response Mapping

| Error | HTTP Code | Message |
|-------|-----------|---------|
| Validation failure (bad field format, missing required field) | `400 Bad Request` | Field-level error details |
| Unique constraint violation (email already exists) | `400 Bad Request` | "This book already exists" |
| No rows returned (resource not found) | `404 Not Found` | "Book with ID 123 does not exist" |
| Foreign key violation (author ID doesn't exist) | `404 Not Found` | "Author with this ID does not exist" |
| Unknown / unclassified error | `500 Internal Server Error` | "Something went wrong" (generic — no internal details) |

### Custom Error Classes

Create typed error classes (DatabaseError, ValidationError, NotFoundError, etc.) so the global handler can identify error type and respond correctly.

```
Repository throws: DatabaseUniqueConstraintError
Service passes it up unchanged
Handler passes it up unchanged
Global middleware: catches DatabaseUniqueConstraintError → return 400 + "already exists"
```

### Two Key Advantages

**1. Robustness:** All error types are handled in one place. You can't forget a condition — any unrecognized error falls back to the `500` default.

**2. Reduced redundancy:** Error handling logic isn't duplicated across every repository method. Define it once in the middleware, not in every SQL call.

---

## 5. Error Propagation & Boundaries

### Propagation

Errors should bubble up with enough context. Wrap low-level errors with business context as they travel up layers:

```
Repository: "UNIQUE constraint violated on column 'email'" (DB detail)
     ↓ wrap with context
Service:    EmailAlreadyExistsError("User with this email already registered")
     ↓
Handler:    passes up
     ↓
Global handler: return 400 + user-friendly message
```

### Service Boundaries

In microservice architectures, prevent errors in one service from cascading to others:
- Use separate processes for separate services
- Implement timeouts at service boundaries
- Use message queues (RabbitMQ, SQS) for async communication — a failing consumer doesn't block the producer

---

## 6. Security in Error Handling

### 6.1 Don't Leak Internal Details

**Never send raw database errors to users.** DB errors often contain table names, constraint names, column names — information an attacker can use for SQL injection or targeted attacks.

```
❌ Bad:  "ERROR: duplicate key violates unique constraint 'users_email_idx'"
✓ Good: "An account with this email already exists"

❌ Bad:  (any unhandled exception message in a 500 response)
✓ Good: "Something went wrong. Please try again later."
```

The `500` default handler message should always be generic — by the time you reach the default, you don't know what the error is, which means it probably contains internal detail.

### 6.2 Authentication Error Messages

Authentication endpoints are the most targeted by attackers. Specific error messages enable **enumeration attacks**:

```
❌ Bad:
  "User with this email does not exist" → attacker knows to try a different email
  "Incorrect password"                  → attacker knows the email is valid, focus on password brute-force

✓ Good:
  "Invalid email or password"           → same message regardless of which field is wrong
```

Reference: **OWASP Authentication Cheat Sheet** — follow established best practices for auth-related error handling.

### 6.3 Log Security

Logs are often stored in external services and can be leaked in breaches.

**Never log:**
- User passwords (even hashed)
- Credit card numbers
- API keys or secrets
- Full email addresses (use user ID for correlation instead)

**When logging auth errors:**
```
❌ Bad:  log { email: "user@example.com", error: "password mismatch" }
✓ Good: log { userId: "abc123", correlationId: "req-xyz", error: "auth_failed" }
```

---

## 7. Summary: Fault Tolerant Mindset

| Principle | Practice |
|-----------|---------|
| Expect errors | Design for failure, not against it |
| Fail fast | Validate config at startup; don't let misconfiguration reach users |
| Detect early | Health checks, monitoring, performance tracking |
| Handle centrally | Global error handler for all layers |
| Contain damage | Graceful degradation, service boundaries, fallbacks |
| Don't expose internals | Generic messages for unknown errors; typed messages for known ones |
| Secure auth errors | Same message regardless of which auth field is wrong |
| Protect logs | Never log PII, passwords, or secrets |

---

## Quick Revision Checklist

- [ ] Logic errors: silent, produce wrong results, can persist for months — monitor business metrics
- [ ] Database errors: connection (pool exhaustion), constraint violation (unique/FK), query (malformed SQL), deadlocks
- [ ] External service errors: network issues, rate limiting (→ exponential backoff), auth errors, outages (→ fallback)
- [ ] Input validation: format, range, required fields → `400 Bad Request` at the entry point
- [ ] Configuration errors: validate all required env vars at startup; fail fast before serving users
- [ ] Best error handling starts before errors happen — proactive detection
- [ ] Health checks: HTTP endpoint + DB query test + external service test + config/cache checks at startup
- [ ] Global error handler: catches all bubbled-up errors → maps error type → returns correct HTTP response
- [ ] Error type mapping: unique constraint → 400, no rows → 404, FK violation → 404, unknown → 500 (generic)
- [ ] Custom error classes enable typed matching in the global handler
- [ ] Never send raw DB/internal errors to users — strip details, send user-friendly messages
- [ ] Auth errors: always return the same message regardless of which field is wrong (prevent enumeration)
- [ ] Logs: never log passwords, emails, credit cards, API keys; log user ID + correlation ID instead
- [ ] Recoverable errors → retry with exponential backoff; non-recoverable → contain + degrade gracefully
- [ ] Service boundaries: timeouts + message queues prevent one failing service from cascading