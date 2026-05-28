# Backend From First Principles

Detailed lecture notes for the **Backend From First Principles** series — built from first principles, covering everything from HTTP internals to production-grade scaling.`

📺 **Playlist:** [YouTube](https://www.youtube.com/playlist?list=PLui3EUkuMTPgZcV0QhQrOcwMPcBCcd_Q1)

📋 **Best Practices Reference:** [BEST_PRACTICES.md](./BEST_PRACTICES.md) — a distilled, actionable guide synthesized from all lectures.

---

## Lecture Index

| # | Topic | Key Concepts | Notes |
|---|-------|-------------|-------|
| 1–4 | *(TBD)* | — | [LEC1](./LEC1.md) · [LEC2](./LEC2.md) · [LEC3](./LEC3.md) · [LEC4](./LEC4.md) |
| 5 | **HTTP Protocol** | Statelessness, request/response anatomy, headers, CORS, status codes, caching, content negotiation, TLS | [LEC5](./LEC5.md) |
| 6 | **HTTP Routing** | Static vs dynamic routes, path params, query params, nested routes, versioning, catch-all | [LEC6](./LEC6.md) |
| 7 | **Serialization & Deserialization** | JSON, Protobuf, text vs binary formats, client-server data exchange | [LEC7](./LEC7.md) |
| 8 | **Authentication & Authorization** | Sessions, JWTs, cookies, OAuth 2.0, OIDC, API keys, RBAC, timing attacks | [LEC8](./LEC8.md) |
| 9 | **Validations & Transformations** | Type/syntactic/semantic/cross-field validation, transformation pipeline, backend vs frontend validation | [LEC9](./LEC9.md) |
| 10 | **Controllers, Services, Repositories, Middlewares** | Three-layer architecture, middleware chain, request context, auth propagation, distributed tracing IDs | [LEC10](./LEC10.md) |
| 11 | **REST API Design** | REST constraints, URL structure, idempotency, CRUD patterns, pagination/sorting/filtering, custom actions | [LEC11](./LEC11.md) |
| 12 | **Mastering Databases with PostgreSQL** | Data types, migrations, schema design, relationships, referential integrity, parameterized queries, indexes, triggers | [LEC12](./LEC12.md) |
| 13 | **Caching** | CDN, DNS caching, Redis, lazy caching, write-through, eviction policies, session/rate-limit/query caching | [LEC13](./LEC13.md) |
| 14 | **Task Queues & Background Jobs** | Producer/broker/consumer, acknowledgements, exponential backoff, one-off/recurring/chain/batch tasks | [LEC14](./LEC14.md) |
| 15 | **Full Text Search & Elasticsearch** | Inverted index, BM25 relevance, field boosting, fuzzy search, type-ahead, PostgreSQL FTS vs Elasticsearch | [LEC15](./LEC15.md) |
| 16 | **Error Handling & Fault Tolerance** | Error types, global error handler, custom error classes, health checks, graceful degradation, log security | [LEC16](./LEC16.md) |
| 17 | **Production-Grade Configuration Management** | Config types, env vars, secrets management, feature flags, startup validation, least privilege | [LEC17](./LEC17.md) |
| 18 | **Logging, Monitoring & Observability** | Three pillars (logs/metrics/traces), log levels, structured JSON logs, OpenTelemetry, Grafana stack | [LEC18](./LEC18.md) |
| 19 | **Graceful Shutdown** | SIGTERM/SIGINT/SIGKILL, connection draining, resource cleanup, reverse-order teardown, zero-downtime deploys | [LEC19](./LEC19.md) |
| 20 | **Backend Security** | SQL/command injection, password hashing (bcrypt/Argon2id), sessions, JWT security, BOLA, BFLA, XSS, CSRF, defense in depth | [LEC20](./LEC20.md) |
| 21.1 | **Scaling & Performance — Part 1** | Latency percentiles, throughput, utilization, N+1 queries, indexes, connection pooling, caching patterns, vertical vs horizontal scaling | [LEC21.1](./LEC21.1.md) |
| 21.2 | **Scaling & Performance — Part 2** | Statelessness, load balancers, read replicas, sharding, CDN, edge computing, async processing, microservices vs monolith, serverless | [LEC21.2](./LEC21.2.md) |
| 22 | **Concurrency & Parallelism** | IO-bound vs CPU-bound, event loop, OS threads, goroutines, race conditions, mutexes, Go channels | [LEC22](./LEC22.md) |
| 23 | *(TBD)* | — | [LEC23](./LEC23.md) |

---

## Topics at a Glance

### Foundation
- **LEC5 – HTTP**: The complete HTTP mental model — statelessness, request/response structure, all status codes, CORS preflight, caching headers, HTTP/1.1 → HTTP/3 evolution.
- **LEC6 – Routing**: How servers map `(method, path)` pairs to handlers. Path params identify resources; query params filter/paginate/sort them.
- **LEC7 – Serialization**: Why JSON exists, how data crosses language boundaries, text vs binary format tradeoffs.

### Auth & Data Integrity
- **LEC8 – Auth**: Every authentication pattern from sessions to OAuth 2.0 + OIDC. Why timing attacks work and how to prevent them.
- **LEC9 – Validation**: The four validation types, the transformation-first pipeline, and why backend validation is mandatory even when you have frontend validation.
- **LEC10 – Architecture**: The Controller → Service → Repository separation and the middleware chain. Request context for passing auth data without coupling.

### API Design & Databases
- **LEC11 – REST**: Roy Fielding's constraints, URL conventions, idempotency, the list API pattern, custom actions.
- **LEC12 – PostgreSQL**: Production-ready schema design — UUID keys, JSONB, TIMESTAMPTZ, parameterized queries, N+1 prevention, indexes, triggers, migrations.

### Infrastructure & Reliability
- **LEC13 – Caching**: CDN → DNS → hardware → Redis. Lazy caching, write-through, eviction, rate limiting with Redis, session storage.
- **LEC14 – Background Jobs**: Async task patterns — one-off, recurring (cron), chained, batch. Acknowledgements, visibility timeout, exponential backoff.
- **LEC15 – Search**: Why `ILIKE '%term%'` breaks at scale. Inverted index, BM25, Elasticsearch vs PostgreSQL FTS decision framework.
- **LEC16 – Error Handling**: The global error handler pattern, custom error types, health checks, graceful degradation, log security rules.
- **LEC17 – Config Management**: Environment-specific configuration, secrets management, startup validation, feature flags, principle of least privilege.
- **LEC18 – Observability**: The three pillars (logs, metrics, traces), structured JSON logging, OpenTelemetry, Grafana/Prometheus/Loki/Jaeger stack, debugging workflow.
- **LEC19 – Graceful Shutdown**: Unix signals, connection draining, reverse-order resource cleanup, zero-downtime deployments.

### Security & Performance
- **LEC20 – Security**: The full attack surface — SQL injection, command injection, password hashing evolution, session security, JWT pitfalls, BOLA, BFLA, XSS, CSRF, defense in depth.
- **LEC21.1 – Performance Part 1**: Latency percentiles over averages, finding bottlenecks (not guessing), N+1 queries, indexes deep dive, connection pooling, cache invalidation strategies.
- **LEC21.2 – Performance Part 2**: Stateless horizontal scaling, load balancing algorithms, read replicas + replication lag, sharding, CDN physics, edge computing, async processing, microservices vs monolith, serverless tradeoffs.
- **LEC22 – Concurrency**: Event loop vs OS threads vs goroutines. Why most backends are IO-bound. Race conditions in both multi-threaded and async/await code.

---
