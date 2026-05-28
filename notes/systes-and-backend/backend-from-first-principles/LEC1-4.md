# What is a Backend? — Lecture Notes (LEC 1–4)

---

## 1. What is a Backend?

A backend is a computer listening for requests (HTTP, WebSocket, gRPC, or similar) on an open port (80/443) that is accessible over the internet, so that clients or frontends can connect to it, send data, and receive data.

It is called a *server* because it **serves** content — static files (HTML, CSS, JS, images) or dynamic data (JSON) — and also **accepts** data from clients.

The single-word summary of what a backend does: **data** — fetch it, receive it, persist it, and act on it.

---

## 2. Request Flow: Browser to Server

A request from a browser travels through multiple hops before reaching the backend process:

```
Browser
  ↓
DNS Server        (resolves domain name → IP address)
  ↓
Network / Internet
  ↓
Cloud Firewall    (e.g., AWS Security Group — only allows traffic on specified ports)
  ↓
EC2 Instance / VM
  ↓
Reverse Proxy     (e.g., Nginx — listens on port 80/443, forwards to app port)
  ↓
Backend Process   (e.g., Node.js on localhost:3001)
```

**Key concepts in this flow:**

### DNS
- **A records** — map a domain/subdomain to an IP address.
- **CNAME records** — map a domain/subdomain to another domain name.
- A subdomain's A record points to the public IP of the server instance.

### Firewall (Cloud Security Group)
- Controls which ports are reachable from the internet.
- Port 22 — SSH (terminal access to the instance).
- Port 80 — HTTP traffic.
- Port 443 — HTTPS traffic.
- If a port is not explicitly allowed, the cloud provider blocks the request before it reaches the server.

### Reverse Proxy (Nginx)
- Sits in front of one or more application servers.
- Handles: SSL termination, HTTP → HTTPS redirects, routing traffic to the correct local port.
- Centralises configuration rather than requiring each app to manage its own TLS/routing.
- Typical config: listens on port 443 for a domain → proxies to `localhost:<app_port>`.

---

## 3. Frontend vs Backend: Why They Are Separate

**Frontend**: the server sends code (HTML/JS/CSS) to the browser, and the **browser executes it**. The browser is the runtime.

**Backend**: the server **executes the code itself** and sends back the result. Processing happens on the server.

### Why backend logic cannot live in the frontend

| Reason | Detail |
|--------|--------|
| **Security (sandboxing)** | Browsers run in isolated sandboxes — no access to the OS filesystem, environment variables, or system-level APIs. Backends frequently need these. |
| **CORS restrictions** | Browser JavaScript can only call APIs on the same origin unless the external API explicitly sets CORS headers. Backends have no such restriction. |
| **Database drivers** | Native DB drivers (pg, mongodb, etc.) require socket connections, binary data handling, and persistent connections — capabilities browsers do not support. |
| **Connection pooling** | Backends maintain a pool of persistent DB connections shared across thousands of requests. If each browser connected directly to the database, the DB would be overwhelmed with connections. |
| **Computing power** | Client devices range from high-end desktops to low-end phones. Heavy business logic on the client fails on low-spec devices. A centralised server can be scaled independently. |

---

## 4. Three-Layer Architecture Overview

Every backend, regardless of language or framework, is organised around the same three logical layers:

| Layer | Responsibility |
|-------|---------------|
| **Presentation Layer** | Routing, middleware, validation, controllers/handlers — everything that deals with accepting and returning user data. |
| **Business Logic Layer (BLL / Service Layer)** | Core domain rules, orchestration, decisions. Sits between presentation and data. |
| **Data Access Layer (Repository Layer)** | Database queries, inserts, updates, deletes. Used by the BLL; knows nothing about HTTP. |

---

## 5. Course Topic Map

The full curriculum covers these foundational backend topics (in order):

1. **HTTP Protocol** — statelessness, request/response structure, headers, methods, CORS, status codes, caching (ETags, max-age), HTTP/1.1 vs 2.0 vs 3.0, content negotiation, compression (gzip/deflate/br), TLS/HTTPS.
2. **Routing** — mapping URLs to handlers, path params, query params, static/dynamic/nested/wildcard/regex routes, API versioning, route grouping, middleware scoping.
3. **Serialization & Deserialization** — JSON vs binary formats (Protobuf), text vs binary tradeoffs (readability vs performance), common errors (null handling, date/timezone), schema validation, compression.
4. **Authentication & Authorization** — stateful (sessions) vs stateless (JWT), basic auth, bearer tokens, cookies, OAuth 2.0, OIDC, API keys, MFA, salting + hashing, RBAC/ABAC, CSRF/XSS/MITM prevention, audit logging, timing attack prevention, consistent error messages.
5. **Validation & Transformations** — type, syntactic, semantic, cross-field validation; transformation pipeline (casting → normalization → sanitization → validation); client-side vs server-side distinction; fail-fast strategy.
6. **Middleware** — request/response lifecycle, middleware chaining, ordering (logging → auth → validation → routing → error handling), common middlewares (security headers, CORS, CSRF, rate limiting, auth, logging, compression, body parsing).
7. **Request Context** — request-scoped state, sharing data across layers without coupling, carrying requestId/traceId/userId through the lifecycle, timeouts and cancellation signals.
8. **Controllers & Services** — MVC pattern, handler/controller/service responsibilities, centralized error handling, consistent response formats.
9. **REST API Design** — resource-centred URLs, HTTP method semantics, pagination/sorting/filtering, versioning strategies (URI, header, query string), OpenAPI spec, content negotiation.
10. **Databases** — relational vs non-relational, ACID, CAP theorem, schema design, indexing, query optimization, connection pooling, ORMs vs raw drivers, transactions, concurrency, migrations.
11. **Caching** — memory/browser/database caching, cache-aside/write-through/write-behind/read-through strategies, eviction (LRU, LFU, TTL, FIFO), cache invalidation (manual, TTL, event-based), L1 (in-process) vs L2 (distributed) caching, hit/miss ratio.
12. **Task Queues & Background Jobs** — producer/broker/consumer model, one-off/recurring/chain/batch tasks, error handling, retries, prioritization, rate limiting.
13. **Full Text Search (Elasticsearch)** — inverted index, TF-IDF, segments and shards, type-ahead, log analytics, index management, query types (basic, full-text, relevance scoring, fuzzy, aggregation), analyzers, field boosting.
14. **Error Handling & Fault Tolerance** — error types (syntax, runtime, logic), fail-safe vs fail-fast, graceful degradation, custom error types, global error handler, monitoring and alerting (Sentry, ELK).
15. **Configuration Management** — static vs dynamic vs sensitive configs, env files, YAML/JSON/TOML sources, environment variables vs CLI flags vs static files, feature flags, secrets management.
16. **Logging, Monitoring & Observability** — log levels (debug/info/warn/error/fatal), structured vs unstructured logging, three pillars (logs, metrics, traces), infrastructure/APM/uptime monitoring, Prometheus/Grafana, alert management.
17. **Graceful Shutdown** — Unix signals (SIGTERM, SIGINT, SIGKILL), connection draining, resource cleanup, zero-downtime deployments.
18. **Security** — SQL/NoSQL/command injection, XSS, CSRF, broken auth, insecure deserialization; principles: least privilege, defense in depth, fail secure, separation of duties; rate limiting, CSP, SameSite cookies, input sanitization.
19. **Scaling & Performance** — response time, resource utilization, bottleneck identification, N+1 query problem, database indexing, batch processing, memory leak prevention, payload compression, performance testing and profiling.
20. **Concurrency & Parallelism** — IO-bound vs CPU-bound work, concurrency models (event loop, OS threads, goroutines), race conditions, mutexes, channels.
21. **Object Storage & Large Files** — object storage (S3), chunked uploads, streaming, multipart file uploads.
22. **Real-time Systems** — WebSockets, Server-Sent Events, pub/sub architecture.
23. **Testing & Code Quality** — unit/integration/E2E/functional/regression/performance/load/security testing, TDD, CI/CD automation, linting, cyclomatic complexity, maintainability index.
24. **12-Factor App** — methodology for building portable, scalable, cloud-native applications.
25. **OpenAPI Standards** — Swagger → OpenAPI transition, spec structure (paths, parameters, schemas, security, responses), Swagger UI, API-first development.
26. **Webhooks** — push vs polling, webhook URL, event triggers, payload, signature verification, retry logic, testing.
27. **DevOps Fundamentals** — CI/CD (continuous integration, delivery, deployment), infrastructure as code, Docker (containerization), Kubernetes (orchestration), horizontal vs vertical scaling, deployment strategies (blue-green, rolling).

---

## Quick Revision Checklist

- [ ] A backend = a computer listening on a port, accessible over the internet, serving and accepting data
- [ ] Request path: Browser → DNS → Firewall → Reverse Proxy (Nginx) → Backend Process
- [ ] DNS A records map subdomains to IP addresses; CNAME records map to other domain names
- [ ] Cloud security groups act as firewalls — ports must be explicitly opened (22/SSH, 80/HTTP, 443/HTTPS)
- [ ] Reverse proxy centralises SSL, port 80→443 redirects, and routing across multiple app servers
- [ ] Frontend: server sends code, browser executes it. Backend: server executes code, sends result.
- [ ] Browsers are sandboxed — no OS filesystem, no system DB drivers, no connection pooling
- [ ] Three layers: Presentation (routing/middleware/controllers) → Business Logic (services) → Data Access (repositories)
- [ ] The core responsibility of a backend: data — fetch, receive, persist, act
