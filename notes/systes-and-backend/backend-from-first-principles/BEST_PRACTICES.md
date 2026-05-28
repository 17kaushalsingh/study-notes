# Backend Best Practices

Distilled from the *Backend From First Principles* lecture series (LEC1–LEC22). Each practice is grounded in a specific lecture; the source is noted inline.

---

## Table of Contents

1. [HTTP & Networking](#1-http--networking)
2. [API Design & Routing](#2-api-design--routing)
3. [Authentication & Authorization](#3-authentication--authorization)
4. [Validation & Transformation](#4-validation--transformation)
5. [Architecture & Layer Separation](#5-architecture--layer-separation)
6. [Database Design & Querying](#6-database-design--querying)
7. [Caching](#7-caching)
8. [Background Jobs & Task Queues](#8-background-jobs--task-queues)
9. [Search](#9-search)
10. [Error Handling & Fault Tolerance](#10-error-handling--fault-tolerance)
11. [Configuration Management](#11-configuration-management)
12. [Logging, Monitoring & Observability](#12-logging-monitoring--observability)
13. [Graceful Shutdown](#13-graceful-shutdown)
14. [Security](#14-security)
15. [Scaling & Performance](#15-scaling--performance)
16. [Concurrency](#16-concurrency)

---

## 1. HTTP & Networking

**Source: LEC5**

- Always use HTTPS in production — HTTP + TLS; enables HTTPS at the infrastructure/proxy layer, not in your app code.
- Use `PATCH` for partial updates and `PUT` only when you intend full resource replacement. `PATCH` is almost always what you want in modern JSON APIs.
- `DELETE` and `GET` must never modify server state. `POST` is non-idempotent — each call may create a new resource.
- Return semantically correct status codes:
  - `201 Created` after successful POST/PUT that creates a resource
  - `204 No Content` for successful DELETE and CORS preflight responses
  - `400` for bad input, `401` for unauthenticated, `403` for forbidden, `404` for missing resource, `409` for duplicate/conflict, `429` for rate-limited, `500` for unexpected server crash
- Add security headers via middleware — `Strict-Transport-Security`, `Content-Security-Policy`, `X-Frame-Options`, `X-Content-Type-Options`. One line of framework middleware handles all of them.
- Set `Set-Cookie` with `HttpOnly` and `Secure` flags on session/auth cookies.
- Use `Access-Control-Max-Age` on CORS preflight responses to cache the preflight result for up to 24 hours and avoid redundant OPTIONS calls.
- Use HTTP compression (`Accept-Encoding: gzip`) — can reduce payload size from 26 MB to 3.8 MB at the cost of minor CPU overhead. Almost always worth it on slow/mobile networks.
- Use HTTP/2 or HTTP/3 (QUIC) in production for multiplexing and lower latency.

---

## 2. API Design & Routing

**Source: LEC6, LEC11**

- Resources in URLs must always be **plural lowercase nouns**: `/users`, `/organizations`, `/books`.
- Use hyphens for multi-word slugs, never spaces or underscores: `/harry-potter`.
- Use path parameters (`:id`) to identify a **specific resource**; use query parameters (`?key=val`) to **filter, sort, or paginate** a list. Never swap these.
- Design API routes before writing any code. Use Insomnia, Postman, or Swagger to prototype the interface independently of any framework.
- Version APIs in the URL: `/api/v1/`, `/api/v2/`. Never change existing routes in breaking ways — release a new version alongside the old one, notify consumers, deprecate, then remove.
- Always register a catch-all route (`/*`) **last** to return a meaningful 404 for unmatched paths rather than an empty or null response.
- For custom actions that don't map to CRUD, use `POST` with a named action appended to the resource URL: `POST /organizations/:id/archive`.
- Every list endpoint must support **pagination** (`page`, `limit`), **sorting** (`sortBy`, `sortOrder`), and **filtering**. The server sets sensible defaults — clients should never be required to send these.
- An empty list result from a list endpoint returns `200 OK` with `data: []` — never `404`.
- Response shape for list endpoints: `{ data: [...], total, page, totalPages }`.
- Use **camelCase** for all JSON field names. Never abbreviate (`description` not `desc`, `organizationId` not `orgId`).
- Exclude server-managed fields (`id`, `createdAt`, `updatedAt`) from request payloads.
- Sort results by default — without an explicit `ORDER BY`, databases return rows in undefined order.
- Set server-side defaults for all "obvious" parameters: `page = 1`, `limit = 10`, `sortBy = createdAt`, `sortOrder = desc`, `status = active` on creation.

---

## 3. Authentication & Authorization

**Source: LEC8, LEC20**

- Use an auth provider (Clerk, Auth0, Supabase Auth) in production. Implementing sessions, OAuth flows, account linking, token rotation, and timing-safe comparisons yourself is error-prone. Only build it yourself if you have a strong reason.
- **Never store passwords in plain text.** Store `hash(password + salt)` using a slow hash function — **bcrypt** or **Argon2id**. Never use MD5, SHA-256, or SHA-512 for passwords (they are too fast for brute force attacks).
- Generate session IDs using a **CSPRNG** (cryptographically secure PRNG) with 128–256 bits of entropy. Guessing a 128-bit value is computationally impossible.
- Store session metadata in Redis (not the DB): user ID, created timestamp, expiry time, IP address, user agent. Redis lookups are sub-millisecond vs 5–50ms for DB queries.
- Set `HttpOnly`, `Secure`, and `SameSite=Strict` (or `Lax`) flags on all auth cookies. Never store session IDs or JWTs in `localStorage` — XSS can steal it.
- JWTs: the payload is **base64-encoded, not encrypted** — anyone can decode it. Never put sensitive data in the payload.
- For JWT-based systems, prefer short-lived access tokens (5–10 minutes) + longer refresh tokens (1–7 days) to limit the damage window of a leaked token.
- Prefer stateful sessions (Redis) over JWTs for typical SaaS — token revocation is hard with JWTs. Use JWTs only when statelessness across distributed servers is a hard requirement.
- Implement **layered rate limiting** on authentication endpoints: per-IP (e.g., 10 attempts/min), per-account (e.g., 5 failures/15 min → lock), and global (100 failed attempts/min → alert + CAPTCHA).
- Use RBAC (Role-Based Access Control). Assign roles at registration; check permissions per-resource on every request.
- Return **the same generic error message** for all authentication failure paths ("Invalid email or password"). Never expose which field was wrong — enumeration attacks use specific messages to narrow their approach.
- Always read `userId` from the verified auth context (middleware), **never from the request body** — a malicious client can send any `userId` they want.
- For authorization, check ownership at the **point of data access** (in the SQL query), not just at the routing layer: `WHERE id = $id AND user_id = $currentUserId`.
- When a user requests another user's resource, return `404 Not Found` — not `403 Forbidden`. A `403` confirms the resource exists and enables enumeration.

---

## 4. Validation & Transformation

**Source: LEC9**

- Validate all incoming data (body, query params, path params, headers) **at the controller entry point** — before any service or DB calls run. Garbage in → garbage out.
- A failed validation returns `400 Bad Request` immediately with field-level error details. Nothing reaches the service layer.
- Run validation in this order: (1) type casting/transformation, (2) presence checks, (3) type validation, (4) syntactic validation, (5) semantic validation, (6) cross-field validation.
- **Transformation must happen before validation**: query params arrive as strings — cast `"2"` → `2` before validating it as a positive integer.
- Common transformations: lowercase email normalization (`"Test@GMAIL.COM"` → `"test@gmail.com"`), trim whitespace, prepend phone country codes, normalize dates to ISO 8601.
- Semantic validation catches logically impossible values that pass type and syntax checks: date of birth cannot be in the future; `endDate` must be after `startDate`; age must be between 1–120.
- Cross-field validation handles inter-field dependencies: `passwordConfirmation` must equal `password`; `partnerName` is required only when `married = true`.
- **Backend validation is mandatory, frontend validation is optional UX.** Anyone can call your API with `curl` or Postman and bypass the frontend form entirely. Never rely on frontend validation for security or data integrity.
- A validation error that returns all expected fields serves as implicit API documentation for consumers.

---

## 5. Architecture & Layer Separation

**Source: LEC10**

- Separate every backend into three distinct layers with no crossover: **Controller → Service → Repository**.
- **Controller** responsibilities only: deserialize/bind request data, validate and transform, call the service, return an HTTP response with the correct status code. No business logic here.
- **Service** responsibilities only: business logic, orchestration, external calls (email, notifications, webhooks). No HTTP concepts leak in — no `req`/`res` objects, no status codes.
- **Repository** responsibilities only: construct and execute database queries. One method = one data operation. Never use optional parameters to make one function do two different things.
- Middleware must satisfy two conditions before creating one: (1) the logic needs to run for multiple requests, and (2) it needs access to the `request` or `response` object.
- Typical middleware order: CORS → security headers → logging → authentication → rate limiting → routing → global error handler.
- The **global error handler goes last** — errors bubble up through all layers and are caught centrally.
- Use **request context** to pass auth data (userId, role) from authentication middleware to handlers. Never trust a userId passed in the request body.
- Generate a unique `requestId` (UUID) per request in an early middleware. Attach it to all logs, downstream service calls, and `X-Request-ID` response header for distributed tracing.

---

## 6. Database Design & Querying

**Source: LEC12, LEC21.1**

- Use **PostgreSQL** as the default for virtually every project. Only reconsider when optimizing for very specific bottlenecks at massive scale.
- Use `UUID` as the primary key type (generated by `gen_random_uuid()`). Sequential integer IDs enable enumeration attacks; UUIDs do not.
- Use `BIGSERIAL` only if you have an existing system requiring auto-increment integers — otherwise prefer UUID.
- Use `DECIMAL`/`NUMERIC` for financial data (prices, currency). Use `FLOAT` only for scientific measurements where small inaccuracies are acceptable.
- Always use `TEXT` instead of `VARCHAR(n)` in PostgreSQL — no performance difference, avoids arbitrary length limits that require migrations later.
- Always use `TIMESTAMPTZ` (with timezone) for `created_at` and `updated_at` fields, never `TIMESTAMP`.
- Always use `JSONB` instead of `JSON` — binary serialized, faster queries, supports indexing.
- Name all tables and fields with **lowercase snake_case**: `created_at`, `full_name`, `project_id`. Table names are **plural**: `users`, `tasks`, `projects`. Never use camelCase in PostgreSQL (forces double-quoting everywhere).
- Add `NOT NULL` to every field unless you have an explicit reason for the field to be nullable. Do not rely on PostgreSQL's default-allow-null behavior.
- Define enum types for fixed sets of allowed values at the **database level** (`CREATE TYPE ... AS ENUM`). Provides data integrity enforcement and serves as documentation.
- Always use `RETURNING *` after `INSERT` or `UPDATE` queries instead of running a second `SELECT`. One round-trip instead of two.
- **Always use parameterized queries.** Never concatenate user input into SQL strings — this is the only way to be vulnerable to SQL injection. Every modern DB driver and ORM supports this.
- Use **database migrations** (dbmate, go-migrate, etc.) for all schema changes. Maintain versioned `up` and `down` migration files. Never change production schemas manually.
- Solve the **N+1 query problem**: never query inside a loop. Use JOINs or bulk `IN (...)` fetches. Enable ORM query logging in development to catch N+1 patterns early.
- Run `EXPLAIN ANALYZE` on slow queries. `Seq Scan` means a missing index; `Index Scan` means the index is being used.
- Primary keys are automatically indexed — do not add a manual index on `id`. Manually index foreign keys, frequent WHERE-clause columns, and frequent ORDER BY columns.
- Create **composite indexes** when queries filter by multiple columns together. Remember the leftmost prefix rule — a composite index on `(user_id, created_at)` helps queries filtering by `user_id` alone, but not by `created_at` alone.
- Every index adds overhead to INSERT/UPDATE/DELETE operations. Evaluate frequency of reads vs writes before adding an index. Monitor and drop indexes that aren't paying for themselves.
- Use `ON DELETE CASCADE` for child records that belong exclusively to a parent (project's tasks). Use `ON DELETE RESTRICT` when parent deletion should be blocked until children are removed. Use `ON DELETE SET NULL` when the relationship is optional.
- Use database **triggers** to auto-update `updated_at` on every row change — reduces the risk of forgetting to include it in every UPDATE query.
- Use an **external connection pooler** (PgBouncer) in production with horizontal scaling — prevents connection limit exhaustion when multiple server instances each maintain their own pool.

---

## 7. Caching

**Source: LEC13, LEC21.1**

- Cache data that is: **read-heavy, write-infrequent, expensive to compute, and safe to be slightly stale**.
- Use Redis for software-level caching — in-memory, sub-millisecond reads/writes, key-value structure.
- The default caching pattern is **cache-aside (lazy loading)**: check cache first; on miss, fetch from DB, store in cache, return result. On write, delete the cache key.
- Use **write-through caching** only when data must always be fresh and you can accept slower write latency.
- Set a TTL (Time-to-Live) on every cache entry. Choose TTL based on how frequently the data changes and how much staleness is acceptable.
- For event-based invalidation, delete or update the cache key at **every code path** that modifies the underlying data. Missing one location causes stale data bugs.
- Prefer **distributed cache** (Redis, Memcached) over local in-process cache in multi-server deployments — local caches are inconsistent across instances.
- Use Redis for **session storage** — every request fetches the session, and Redis sub-millisecond lookups reduce DB load dramatically.
- Use Redis for **rate limiting** — store IP → counter with a TTL-based automatic reset. Redis atomic increments prevent race conditions.
- Cache expensive **external API responses** with a sensible TTL — reduces external call volume and protects against provider rate limits and downtime.
- Monitor your **cache hit rate**. Target 80–90%+. A low hit rate means the wrong data is being cached or the TTL is too short.
- Use **tiered caching** for the hottest data: local in-process cache → Redis → DB.
- CDN-level caching: cache static assets (JS/CSS/HTML bundles, images, fonts) and stable API responses at edge nodes. Configure CDN TTLs; purge on deploy.

---

## 8. Background Jobs & Task Queues

**Source: LEC14**

- Any operation that (a) depends on an external service, (b) is computationally heavy, or (c) is not required for the immediate HTTP response should be offloaded to a background worker.
- The architecture: **Producer** (your server pushes a serialized task to a broker) → **Broker/Queue** (stores tasks; RabbitMQ, Redis/BullMQ, Amazon SQS) → **Consumer/Worker** (separate process dequeues and executes).
- Return `200` (or `201`) to the client immediately after enqueuing — the worker handles the actual work asynchronously.
- Implement **acknowledgement**: workers send success ack (removes task) or failure ack (queues for retry). If no ack is received within the visibility timeout, the queue re-releases the task for another worker.
- Implement **exponential backoff** for retries: 1 min → 2 min → 4 min → 8 min → up to max retries. Most external service downtime resolves in seconds; the task succeeds within 1–2 retries.
- Design every task to be **idempotent** — safely retryable from scratch without unintended side effects. Use DB transactions so a partial failure doesn't corrupt data.
- Keep tasks **small and focused** — one task = one processing unit. Large tasks are harder to retry, monitor, and debug.
- Avoid long-running tasks. If a task takes too long, decompose it into smaller chained or parallel tasks.
- Log every failure with enough context: task ID, user ID, error message, stack trace. Good logging = faster debugging.
- Use **chain tasks** to express parent-child dependencies (encode video → generate thumbnails in parallel → process thumbnails to multiple resolutions).
- Monitor queue length and worker health. Alert when queue length spikes — it signals worker crashes or external service outages.
- Scale workers horizontally — add more consumer instances independently of HTTP server instances.

---

## 9. Search

**Source: LEC15**

- Never use `ILIKE '%term%'` for search on large datasets — it requires a full sequential table scan, ignores indexes, provides no relevance ranking, and has no typo tolerance. At millions of rows, latency exceeds 30 seconds.
- Use an **inverted index** for full-text search — maps terms → documents at storage time, enabling O(1) lookups at query time instead of linear scanning.
- For simple search needs on an existing PostgreSQL database, use PostgreSQL's built-in full-text search (`tsvector` / `tsquery`).
- For search-heavy features requiring relevance ranking, typo tolerance, and type-ahead at scale, use **Elasticsearch**.
- In Elasticsearch, use `text` field type for full-text search (analyzed, tokenized) and `keyword` for exact-match filtering (status, category).
- Use **field boosting** to control relevance — title matches should rank higher than description matches: `"fields": ["title^3", "description^2", "content^1"]`.
- Use **fuzzy matching** for typo tolerance — Levenshtein edit distance allows `"labtop"` to match `"laptop"`.
- If your company already uses the ELK stack for log management, use Elasticsearch for search too — it's already in your infrastructure.
- Treat Elasticsearch as a specialized tool, not a mastery requirement. Most common patterns (basic search, fuzzy, type-ahead, filters) are available as copy-paste JSON queries from the official docs.

---

## 10. Error Handling & Fault Tolerance

**Source: LEC16**

- **Errors are normal, not exceptional.** Design for failure from the start.
- Validate all required **environment variables at startup** and exit immediately with a clear error if any are missing. Never let a misconfiguration silently reach a live user. (Blue-green deployments keep the old version alive if the new one fails to start.)
- Implement a **global error handler middleware** as the last middleware in the chain. All errors from all layers bubble up to it. Map error types to HTTP responses in one central place.
- Create **typed custom error classes** (DatabaseError, ValidationError, NotFoundError, UniqueConstraintError, etc.) so the global handler can categorize errors precisely.
- Error type → HTTP code mapping:
  - Validation failure → `400`
  - Unique constraint violation → `400`
  - No rows returned (not found) → `404`
  - Foreign key violation (referenced entity missing) → `404`
  - Unknown/unclassified → `500` (generic message only)
- **Never send raw database errors to users.** DB errors expose table names, constraint names, and column names — attackers use this for targeted SQL injection. Map to user-friendly messages.
- The `500` default message must always be generic ("Something went wrong") — by the time you reach the default handler, you don't know what the error is.
- Use **exponential backoff** for recoverable errors (network timeouts, temporary DB connection failures, external API rate limits): retry with increasing delays.
- For non-recoverable errors: **contain and degrade gracefully** — switch to cached data, disable the broken feature, provide a fallback. Prevent one failure from cascading.
- Implement **health check endpoints**: HTTP liveness check, DB connectivity + query latency, external service test calls. Check all required env vars and caches at startup.
- Monitor **business metrics** as early warning signals — a sudden drop in successful transactions is often the first sign of a technical problem before error rates spike.
- In microservice architectures, implement timeouts at service boundaries and use message queues for async communication to prevent one failing service from cascading.
- Logic errors (wrong results without crashes) are the most dangerous — they can go undetected for months. Monitor business metrics alongside technical metrics.

---

## 11. Configuration Management

**Source: LEC17**

- **Never hardcode secrets** (DB passwords, API keys, JWT secrets) in source code. Read them from environment variables at runtime.
- If a secret is accidentally committed to git, **rotate it immediately**. Deleting the commit doesn't help — it remains in git history.
- Classify each config value before storing: sensitive vs non-sensitive, environment-specific vs shared, frequently vs rarely changing.
- Use environment variables (`.env` + dotenv library locally; injected by the deployment platform in production) for secrets and environment-specific settings.
- Use YAML/TOML config files (not JSON — JSON doesn't support comments) for non-sensitive application settings. Comments are critical for config files.
- For production at scale, use a **dedicated secrets management service**: HashiCorp Vault, AWS Parameter Store / Secrets Manager, Azure Key Vault, Google Secret Manager. These provide encryption at rest, access control, audit logs, and automatic rotation.
- Apply **principle of least privilege** to config access — frontend engineers get frontend keys; backend engineers get backend credentials; DevOps gets infra secrets. No one has access to everything.
- **Rotate secrets periodically** (API keys, JWT secrets, DB passwords). A leaked secret rotated last week causes far less damage than one unchanged for 3 years.
- **Validate all config at startup using a schema** (Zod for TypeScript, go-validator for Go). Fail fast before serving any users.
- Use **feature flags** to enable/disable features without deploying new code — invaluable for A/B testing, regional rollouts, and emergency feature disabling.
- Set `LOG_LEVEL=debug` in development and `LOG_LEVEL=info` in production. Debug logs contain internal implementation details attackers can use.
- Set appropriate DB connection pool sizes per environment: small in local/staging (cost), large in production (traffic).
- The same codebase should run in all environments — only configuration changes between environments, not code.

---

## 12. Logging, Monitoring & Observability

**Source: LEC18**

- Implement **structured JSON logs** in production — machine-parseable, filterable by field (userId, traceId, level, route). Use human-readable colored logs only in development.
- Use the correct log level for each event:
  - `debug` — detailed step-by-step info (development only)
  - `info` — successful operations, business events
  - `warn` — unexpected but non-critical (failed auth attempt, deprecated usage)
  - `error` — DB query failed, external API error, unhandled exception
  - `fatal` — unrecoverable error; app shuts down
- Set `LOG_LEVEL=debug` in development, `LOG_LEVEL=info` in production.
- **Never log** passwords, credit card numbers, full email addresses, API keys, or raw JWT tokens. Log `userId` and `requestId` for correlation instead.
- Propagate a `traceId` / `requestId` through all log entries and all downstream service calls so you can correlate every event from a single request.
- Implement **distributed tracing** — record per-component timing (span) for every request. This is the right tool for diagnosing I/O-bound performance problems (DB queries, external API calls).
- Monitor: HTTP error rates (4xx/5xx), DB query latency, requests per second, response time percentiles (P50/P95/P99), open connection counts, CPU/memory utilization.
- Set up **alerts**: "if error rate > 80% for 5 minutes, send Slack notification." Alerts fire from metrics; logs and traces provide the why.
- Monitor **performance degradation** — a system slowing down is often the warning before it breaks. Don't only alert on errors.
- Typical debugging workflow: alert fires → check metrics dashboard → find related logs → jump to trace → identify failing component → fix.
- Instrument code with **OpenTelemetry** for vendor-neutral trace/metric/log emission.
- Open source stack: Prometheus (metrics) + Loki (logs) + Grafana (dashboards) + Jaeger (traces). Managed options: New Relic, Datadog.
- Start with a managed observability solution if you lack dedicated DevOps bandwidth. Migrate to open source when you have the team.
- **Implement observability from day one** — not after you have scaling problems.

---

## 13. Graceful Shutdown

**Source: LEC19**

- Always implement graceful shutdown. An abrupt kill mid-deployment can corrupt DB writes, produce double charges, or leave transactions dangling.
- Handle both **SIGTERM** (sent by Kubernetes, PM2, systemd on deployment) and **SIGINT** (Ctrl+C in development) with the same graceful shutdown logic.
- Never catch **SIGKILL** — it's unblockable. If SIGTERM is not handled within the timeout, the OS escalates to SIGKILL. This is your last chance to clean up.
- Graceful shutdown in two steps: (1) **stop accepting new connections** and let in-flight requests complete, then (2) **release all acquired resources** in **reverse acquisition order**.
- Set a **graceful shutdown timeout** (commonly 30 seconds). Base it on your typical longest request duration. If a request hasn't finished within the timeout, force-close it.
- Resources to clean up: open DB transactions (commit or rollback), DB connection pool, Redis connections, background job workers, file handles, temporary files.
- Clean up in **reverse acquisition order** — stop the HTTP server first, then close the DB, then close Redis. Never close a dependency before its consumers have stopped.
- Most HTTP frameworks expose a `server.shutdown()` or `server.close()` method that handles connection draining internally. Use it; don't implement it manually.
- Graceful shutdown is what makes **zero-downtime (blue-green) deployments** work — old server drains in-flight requests while new server picks up new traffic.

---

## 14. Security

**Source: LEC20**

- **Root cause of all injection attacks**: user-supplied data treated as code when crossing a language boundary (SQL, OS shell, HTML/JS). At every boundary, ask: "is this input being treated as data, or could it become code?"
- Use **parameterized queries** (prepared statements) for all SQL containing user input. String concatenation + user input = SQL injection vulnerability. This is the only way to be vulnerable.
- Use **argument arrays** (not shell strings) when executing OS commands. `exec(["ffmpeg", "-o", userInput])` is safe; `exec("ffmpeg -o " + userInput)` is not.
- Sanitize all user-provided HTML/markup **server-side** before storing or rendering. Never trust user input to be safe HTML — this prevents stored XSS.
- Add **Content-Security-Policy** as a last line of defense against XSS — but fix the root cause (sanitization) first. CSP alone is not prevention.
- Set `SameSite=Lax` or `SameSite=Strict` on cookies to mitigate CSRF (cross-site request forgery) in modern browsers.
- Use **UUIDs as primary keys** — sequential integer IDs enable enumeration attacks (attacker iterates `/invoices/101`, `/102`, `/103`...).
- Implement **BOLA protection** (Broken Object Level Authorization): every DB query for user-owned resources must include `AND user_id = $currentUserId`. Don't rely on routing-layer auth alone.
- Implement **BFLA protection** (Broken Function Level Authorization): admin endpoints need role-checking middleware, not just auth middleware. Security through obscurity (hiding URLs) is not a control.
- Apply **default deny** for authorization — new endpoints are protected by default until explicitly granted. Missing a permission check is far safer than an overly permissive default.
- Never send raw database errors (constraint names, column names, table names) to API consumers. Map all DB errors to user-friendly messages in the global error handler.
- Set `LOG_LEVEL=info` in production — debug logs expose code structure, stack traces, query details, and sensitive user data.
- Enable your framework's **security header middleware** with one line: sets CSP, X-Frame-Options, X-Content-Type-Options, HSTS automatically.
- Follow **OWASP Top 10** and the **OWASP Authentication Cheat Sheet** for current vulnerability categories and countermeasures.
- Practice **PortSwigger Web Security Academy** labs for hands-on understanding of SQLi, XSS, CSRF, SSRF, JWT attacks, and auth vulnerabilities.
- Apply **defense in depth**: validation → parameterized operations → auth at access point → security headers → monitoring. Each layer assumes the others may fail.

---

## 15. Scaling & Performance

**Source: LEC21.1, LEC21.2**

- **Never guess — always measure.** Identify the actual bottleneck before implementing any solution. An unintuitive bottleneck (synchronous logging, JSON serialization, N+1 queries) is more common than the obvious one.
- Use **distributed tracing** to identify I/O-bound bottlenecks (DB queries, external API calls). Profiling (flame graphs) is better for CPU-bound bottlenecks.
- Use **latency percentiles** (P50, P95, P99), not averages. 1% of users experiencing 5-second latency (P99) is 10,000 users/day at 1M daily users — averages hide this entirely.
- P99 requests are typically your most valuable users — payment flows, complex operations. Prioritize them.
- Run production systems at **60–80% utilization max**. Latency grows exponentially near 100% utilization. Always maintain headroom for traffic bursts.
- **Progressive scaling approach** (simplest → most complex): DB indexing → query optimization → caching → vertical scaling → connection pooling → read replicas → horizontal scaling → sharding → microservices.
- **Don't scale prematurely.** Build for the scale you have with reasonable headroom. Netflix-scale solutions don't apply to early-stage SaaS.
- For **horizontal scaling**, externalize all state: sessions → Redis, file uploads → S3/object storage, database → shared PostgreSQL (not SQLite file).
- Use **PgBouncer** (external connection pooler) in production with multiple server instances — prevents DB connection limit exhaustion during autoscaling.
- Prefer **monolith** as the default architecture. A horizontally scaled monolith handles hundreds of thousands of users. Only migrate to microservices when you have 100+ developers, genuine independent scaling needs, or technology heterogeneity requirements.
- Microservices trade resource problems for distributed systems problems (network latency, distributed debugging, data consistency, operational complexity). Ensure the tradeoff is worth it.
- Use a **CDN** for static assets (JS/CSS/HTML bundles, images, fonts) and stable API responses. CDN edge nodes serve content from ~200km away instead of 20,000km — reduces latency from 100ms to 2–3ms.
- Use **edge computing** (Cloudflare Workers) for stateless, logic-light operations (auth checks, routing, localization) that benefit from being geographically close to the user. Not for heavy processing.
- Use **serverless** for event-triggered infrequent tasks, spiky traffic APIs, and scheduled jobs. Avoid it for latency-sensitive, long-running, WebSocket, or DB-heavy applications.
- For serverless + databases, use a **serverless-compatible DB** (Neon, PlanetScale) — traditional connection-per-request models exhaust DB connections from serverless environments.
- **Implement observability from day one** — even before you have scaling problems. Logs + metrics + traces enable measuring and diagnosing bottlenecks precisely.

---

## 16. Concurrency

**Source: LEC22**

- Most SaaS backend applications are **IO-bound** — the CPU spends 95% of its time waiting for DB responses, external API calls, or file I/O, not computing. Concurrency (not parallelism) is the solution.
- **Concurrency** = handling multiple tasks by interleaving them on one core. **Parallelism** = executing multiple tasks simultaneously on multiple cores. IO-bound work needs concurrency; CPU-bound work needs parallelism.
- Choose the right concurrency model for your language:
  - **Event loop** (Node.js, Python asyncio, Rust tokio): excellent for IO-bound, catastrophic for CPU-bound. Never block the event loop.
  - **OS threads** (Java classic, C): excellent for CPU-bound parallelism, expensive for high-concurrency IO-bound (10k+ threads exhaust memory).
  - **Goroutines** (Go): excellent for both — lightweight virtual threads (~2–4KB stack vs ~8MB for OS threads), Go runtime scheduler handles IO pausing transparently.
- In Node.js (event loop model): use `async/await` for all IO. Never run synchronous, CPU-intensive code on the main thread — it blocks all other requests.
- **Race conditions** occur when multiple tasks read and modify shared state concurrently. They exist in both multi-threaded and single-threaded async/await code (the `await` point is where interleaving happens).
- Protect shared mutable state with a **mutex/lock** — only one task can hold the lock at a time.
- In Go, prefer **channels** over shared memory for concurrent communication — "don't communicate by sharing memory; share memory by communicating."
- Offload CPU-bound work (image processing, video encoding, ML inference, heavy encryption) to background workers that run on separate processes, not the main request handler.
- Enable query logging in ORMs during development to verify that concurrent request handling is not generating N+1 queries or unexpected sequential operations.
