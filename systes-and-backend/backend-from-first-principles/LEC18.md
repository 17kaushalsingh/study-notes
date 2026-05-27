# Logging, Monitoring & Observability — Lecture Notes

> **Key framing:** This is a spectrum, not a checkbox. No system is "100% observable." These are practices implemented to varying degrees — the goal is to progressively improve your ability to understand and debug your system in production.

---

## 1. Why This Matters

Modern backends run in distributed environments — multiple services, regions, databases, external integrations. Without structured observability:
- You only find out something is broken when users complain
- You know *that* something is wrong but not *what* or *where*
- Debugging production issues becomes guesswork

With proper logging, monitoring, and observability: you know what happened, where it happened, why it happened — and often before users notice.

---

## 2. Three Core Concepts

### 2.1 Logging

**What:** Recording important events throughout the application lifecycle with metadata.

**Think of it as:** A diary your backend keeps — what happened, when, and with what context.

**Examples of events to log:**
- User logs in
- Database query executes (or fails)
- External API call is made
- Business operation completes (to-do created, order placed)
- Error occurs (with stack trace, user ID, request ID)

---

### 2.2 Monitoring

**What:** Continuously tracking the health and performance of your system using real-time (≈10–15 second lag) data.

**Examples of what to monitor:**
- Server CPU and memory usage
- Requests per second
- Error rate (% of requests returning 4xx/5xx)
- Open database connection count
- Response time percentiles (p50, p95, p99)

Monitoring tells you **that** something is wrong. It doesn't tell you **why**.

---

### 2.3 Observability

**What:** The ability to determine the internal state of your system by examining its external outputs. Built on three pillars:

| Pillar | What it provides |
|--------|----------------|
| **Logs** | What happened — events with context |
| **Metrics** | How the system is performing — quantified numbers over time |
| **Traces** | How a request flowed through all components |

**Monitoring vs Observability:**

| | Monitoring | Observability |
|--|-----------|--------------|
| Tells you | *That* there's a problem | *What and where* the problem is |
| Data type | Aggregated metrics, alerts | Logs + metrics + distributed traces |
| Origin | Infrastructure-level | Code-level + infrastructure-level |

---

## 3. The Three Pillars in Detail

### 3.1 Logs

Already covered above. The key addition: logs should link to traces via a shared `traceId` / `requestId` so you can jump from a log entry to the full request trace.

### 3.2 Metrics

**Concrete numbers about your system**, either current or historical:

| Metric | Example |
|--------|---------|
| Request count | 1,200 requests in the last 5 minutes |
| Error rate | 4.2% of requests returned 5xx |
| Response time | p95 = 340ms |
| Business metrics | 47 to-dos created in the last hour |
| Infrastructure | Memory: 3 MB, GC pause: 12ms |

Metrics drive **alerts**: "if error rate > 80% for 5 minutes, send a Slack notification."

### 3.3 Traces

**A trace = the full journey of a single request through all system components.**

```
Incoming request
      ↓
Middleware (auth check)      ← span 1: 2ms
      ↓
Validation layer             ← span 2: 1ms
      ↓
Service method               ← span 3: 45ms
      ↓
Repository (DB query)        ← span 4: 38ms
      ↓
Response
```

Each step is called a **span**. Together they form a **trace**. A trace lets you see:
- Where time was spent
- Where an error first appeared
- Which downstream service caused a failure

---

## 4. Typical Debugging Workflow

```
Alert fires: "Error rate > 80%"
      ↓
Open monitoring dashboard (Grafana / New Relic)
      ↓
Look at metrics: confirm spike in 5xx errors
      ↓
Find related logs: filter by time window and error level
      ↓
Click on a specific log entry: see full context (user ID, route, error message)
      ↓
Jump to trace: see exactly which component failed and at what point
      ↓
Fix the issue
```

This workflow is only possible if all three pillars are implemented.

---

## 5. Log Levels

Each log statement should have an assigned level. Levels control what gets written to logs in each environment.

| Level | When to use | Environment |
|-------|-------------|-------------|
| **debug** | Detailed troubleshooting info — step-by-step flow, variable values | Dev only |
| **info** | General successful operations, business events ("to-do created") | Dev + Production |
| **warn** | Unexpected but non-critical — failed auth attempt, deprecated API usage | Dev + Production |
| **error** | Something went wrong — DB query failed, external API error, validation error | Dev + Production |
| **fatal** | Critical unrecoverable error — app shuts down / restarts | Dev + Production |

**In practice:**
- Set `LOG_LEVEL=debug` in development → see everything
- Set `LOG_LEVEL=info` in production → avoid noise; keep only meaningful events

---

## 6. Structured vs Unstructured Logging

### Unstructured (Development)

Human-readable, colored terminal output:
```
[INFO]  2024-01-15 10:23:41  Connected to database
[INFO]  2024-01-15 10:23:41  Server started on port 8080
[ERROR] 2024-01-15 10:24:12  Failed to create to-do: duplicate key
```

Easy for humans to read. Hard for machines to parse.

### Structured (Production)

JSON format — every field is explicit and parseable:
```json
{
  "level": "error",
  "timestamp": "2024-01-15T10:24:12Z",
  "message": "Failed to create to-do",
  "userId": "user_abc123",
  "requestId": "req_xyz456",
  "traceId": "trace_789",
  "error": "duplicate key violates unique constraint",
  "route": "POST /todos",
  "latencyMs": 45
}
```

**Why JSON in production:**
- Log management tools (ELK stack, Loki, New Relic) can parse and index every field
- You can filter logs by `userId`, `traceId`, `level`, `route`, etc.
- You can build dashboards and alerts from specific fields
- Impossible to do this reliably with plain text

> **Rule:** Human-readable in development → JSON in production.

---

## 7. Instrumentation and OpenTelemetry

**Instrumentation:** The practice of adding measurement code to your application — recording spans, adding attributes to traces, emitting metrics. This is the code-level work that makes observability possible.

**OpenTelemetry (OTel):** An open standard and ecosystem for instrumentation. Provides:
- SDKs for all major languages (Node.js, Go, Python, Java, Rust, etc.)
- Standardized APIs for emitting traces, metrics, and logs
- Vendor-neutral — works with Grafana, New Relic, Datadog, Jaeger, etc.

**How a trace is created in code (simplified):**

```go
// Middleware: create a transaction/trace when a request arrives
transaction := newrelic.StartTransaction("POST /todos")
transaction.AddAttribute("userId", userID)
transaction.AddAttribute("env", "production")
ctx = context.WithValue(ctx, "transaction", transaction)

// Service layer: continue the same trace
txn := ctx.Value("transaction")
defer txn.EndSegment()         // ends this span when function returns
txn.AddAttribute("title", todo.Title)

// Log the event
logger.Info("Creating to-do", "title", todo.Title, "userId", userID)

// On error: add to trace + log
txn.NoticeError(err)
logger.Error("Failed to create to-do", "error", err, "userId", userID)
```

The key: the same `requestId` / `traceId` flows through every log and span so you can correlate them.

---

## 8. What to Log (Practical Guide)

| Event type | Log level | What to include |
|-----------|-----------|----------------|
| Request received | `info` | route, method, userId, requestId, IP |
| Business operation start | `info` | operation name, key parameters |
| Business operation success | `debug` | result ID, duration |
| Business event (to-do created) | `info` | resource ID, user ID, key fields |
| Validation failure | `warn` | field names, validation errors |
| Auth failure | `warn` | userId (not password), route, IP |
| External API call | `info` | service name, endpoint, status |
| DB query failure | `error` | query type, error message (not raw SQL) |
| Unhandled exception | `error` | stack trace, userId, requestId |
| App startup | `info` | environment, port, config summary |
| App shutdown | `info` | reason, any cleanup status |

**Never log:** passwords, credit card numbers, full email addresses, API keys, raw JWT tokens (see error handling notes).

---

## 9. Tools

### Open Source Stack (Grafana Stack)

| Tool | Role |
|------|------|
| **Prometheus** | Metrics collection and storage |
| **Grafana** | Dashboard and visualization (connects to Prometheus, Loki, Jaeger) |
| **Loki** | Log aggregation (like Elasticsearch but for logs, pairs with Grafana) |
| **Jaeger** | Distributed trace collection and visualization |
| **OpenTelemetry Collector** | Receives traces/metrics from apps, forwards to backends |

**Pros:** Free, open source, highly configurable, used at most large companies.
**Cons:** Requires setup, configuration, and ongoing maintenance.

### Managed / Proprietary Solutions

| Tool | Notes |
|------|-------|
| **New Relic** | All-in-one: logs, metrics, traces, dashboards, alerts |
| **Datadog** | Similar to New Relic; strong APM (application performance monitoring) |
| **Elastic APM** | Add-on to ELK stack for traces and APM |

**Pros:** Simpler setup, integrated dashboards, managed infrastructure.
**Cons:** Cost at scale; vendor lock-in.

**Rule of thumb:** Start with a managed solution (New Relic, Datadog) if you don't have dedicated DevOps bandwidth. Migrate to open source when you have the team.

---

## 10. Shared Responsibility

Logging, monitoring, and observability require **both** developers and DevOps / infrastructure teams:

| Responsibility | Who |
|----------------|-----|
| Add log statements with proper levels and metadata | Developer |
| Instrument traces (create spans, add attributes) | Developer |
| Emit custom business metrics | Developer |
| Set up Prometheus / Grafana / Loki infrastructure | DevOps |
| Configure log aggregation and ingestion pipelines | DevOps |
| Set up alert rules and on-call routing | DevOps + Developer |
| Define which metrics and thresholds matter | Developer + Product |

---

## Quick Revision Checklist

- [ ] Logging = recording events; Monitoring = real-time health data; Observability = full internal state understanding
- [ ] Three pillars of observability: Logs (what happened), Metrics (how the system performs), Traces (how a request flowed)
- [ ] Monitoring tells you *that* something is wrong; observability tells you *why*
- [ ] Log levels: debug (dev only) → info → warn → error → fatal (app shuts down)
- [ ] `LOG_LEVEL=debug` in development, `LOG_LEVEL=info` in production
- [ ] Unstructured (human-readable) logs in dev; JSON (structured) logs in production
- [ ] JSON logs enable filtering, indexing, and dashboard building by tools
- [ ] A trace = full journey of one request through all components; each step = a span
- [ ] Instrumentation = adding measurement code; OpenTelemetry = vendor-neutral standard for this
- [ ] Pass traceId / requestId through all logs so you can correlate events from the same request
- [ ] Open source stack: Prometheus (metrics) + Loki (logs) + Grafana (dashboards) + Jaeger (traces)
- [ ] Managed options: New Relic, Datadog — simpler but cost more at scale
- [ ] Never log passwords, secrets, full emails, or credit card numbers
- [ ] Debugging workflow: alert → metrics → logs → trace → fix
- [ ] This is a spectrum — implement incrementally; something is always better than nothing