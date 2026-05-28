# Backend Scaling & Performance Engineering — Part 2

---

## 1. Statelessness — The Key Enabler of Horizontal Scaling

**Definition:** No server instance holds any data exclusive to that instance. Any request routed to any instance must produce the same result.

**Why it matters:** If instance A stores session data in its own memory and the load balancer routes the user's next request to instance B, B has no access to that session → 401 error → terrible UX.

**The rule:** Any data that needs to persist or be shared across instances must live **outside** all instances in a centralized location accessible by every instance.

| State type | Wrong (per-instance) | Right (centralized) |
|-----------|---------------------|---------------------|
| Sessions / auth | In-memory array in instance A | Redis (shared) |
| File uploads | Instance A's local SSD | S3, Cloudflare R2, object storage |
| Database | SQLite file on instance | PostgreSQL / RDS (shared) |
| Cache | In-process map | Redis / Memcached |

> Once you externalize state, adding more server instances is trivial — each new instance is interchangeable with any other. You can remove any instance at any time without data loss.

---

## 2. Load Balancers

**What:** A component that sits between internet traffic and server instances, distributing requests across instances based on a routing algorithm.

```
Users → Internet → Load Balancer → Instance A
                                 → Instance B
                                 → Instance C
```

Without a load balancer, horizontal scaling is impossible — there's no mechanism to decide which instance handles which request.

### 2.1 Load Balancing Algorithms

#### Round Robin

Requests distributed in rotating order: A → B → C → A → B → C…

**Best for:** Homogeneous requests and equal-capacity servers.
**Problem:** Mindlessly distributes regardless of request weight. If a 2-second request lands on A while B and C get 50ms requests, A gets overloaded even if the next rotation sends more heavy requests to it.

#### Weighted Round Robin

Same as round robin, but servers with more capacity receive proportionally more requests.

Example: Server A (8GB RAM, 4 cores) → 2× requests vs Server B (4GB RAM, 2 cores) → 1× requests.

#### Least Connections

Routes each new request to the server with the fewest currently active connections.

**Why it's smarter:** An active HTTP connection stays open until the server finishes processing. An expensive 2-second request keeps a connection open; a 50ms request closes it quickly. So servers handling heavy requests accumulate fewer new connections automatically, and the algorithm routes lighter requests to them preferentially.

**Weighted Least Connections:** Same algorithm but larger-capacity servers can hold proportionally more connections before being deprioritized.

#### Least Response Time

Routes to the server currently responding fastest. Overloaded servers naturally respond slower → get fewer requests.

#### Resource-Based

Routes based on current CPU/RAM usage. Servers with lower resource utilization receive more requests.

---

### 2.2 Health Checks

**The problem:** If a server instance crashes, a round-robin load balancer keeps routing 1/N of requests to it → those users get 502/503 errors.

**Solution:** The load balancer continuously sends lightweight test requests (e.g., GET /health) to every instance, typically every ~1 second. It expects a `200` response.

```
Every 1 second:
  LB → GET /health → Instance A: 200 ✓ (keep routing)
  LB → GET /health → Instance B: 200 ✓ (keep routing)
  LB → GET /health → Instance C: no response / 5xx ✗ → blacklist
```

When an instance fails to return 200:
- Load balancer stops routing user requests to it
- Health check requests continue silently to that instance
- When it returns 200 again → removed from blacklist → resumes receiving traffic

Health checks add negligible load (a simple endpoint with no DB interaction) but eliminate the "dead instance keeps receiving traffic" problem.

---

## 3. Database Scaling

Scaling stateless application servers is straightforward once you externalize state. The hard part is scaling the database — it's inherently stateful.

### 3.1 Read Replicas

**Problem being solved:** 70–90% of typical SaaS requests are reads. One primary DB instance handles all reads and writes and eventually becomes the bottleneck.

**Architecture:**

```
Primary DB (handles all writes)
  ├→ Read Replica 1 (US East)     ← read-only
  ├→ Read Replica 2 (India)       ← read-only
  └→ Read Replica 3 (Singapore)   ← read-only
```

- All `INSERT`, `UPDATE`, `DELETE` → go to primary
- All `SELECT` → go to nearest read replica

**Benefits:**
- Primary DB load reduced from 100% of requests to ~30% (writes only)
- Replicas can be placed in different geographic regions → lower latency for users in those regions
- Replicas serve ~70% of traffic

**The critical problem — Replication Lag:**

Data from the primary takes some time to propagate to replicas (due to physical distance). This is called **replication lag**.

**Example failure scenario:**
1. User updates their name (write → primary DB)
2. Frontend immediately fires a GET request (read → nearest replica)
3. Replica hasn't received the update yet (replication lag ~200ms)
4. User sees their old name, despite just saving

**Mitigation strategies:**

| Strategy | How |
|----------|-----|
| Route reads to primary after a write | For a short window after a write operation, send read queries for that entity to the primary |
| Wait out replication lag | Block the GET request until replication completes (adds latency) |
| Client-side optimistic updates | Update the UI immediately without waiting for the GET — don't fire a re-fetch right after a save |

> Modern managed DB providers (AWS RDS, Neon, Google Cloud SQL) handle the replication mechanics. You configure it via toggles in their console — you don't implement replication yourself.

---

### 3.2 Sharding (Partitioning)

**Problem being solved:** Even with read replicas, a single primary DB handling a table with hundreds of billions of rows eventually becomes too slow for both queries and writes.

**What sharding is:** Physically dividing a table across multiple separate database instances (shards), where each shard holds a subset of the data.

```
Orders table (1B rows total)
  → Shard 1: Jan–Jun orders   (500M rows, DB instance 1)
  → Shard 2: Jul–Dec orders   (500M rows, DB instance 2)
```

**Benefits:**
- Query latency reduced (500M rows instead of 1B)
- Write throughput increases (two instances processing writes in parallel)
- Can scale to any number of shards as data grows

**The sharding key problem:** Choosing how to divide the data is tricky. A bad key creates **hot shards** (one shard receives most traffic while others sit idle). Common keys: time period, geographic region, user ID range, hash of user ID.

**Routing logic:** Your backend (or a routing layer) determines which shard holds the requested data before making the query:

```
Query for user_id = 12345
  → hash(12345) % number_of_shards = shard 2
  → route query to shard 2 DB instance
```

---

### 3.3 Distributed Databases (Modern Approach)

Managed distributed databases handle replication, sharding, distributed transactions, and backups automatically:

| Database | Type | Notable |
|---------|------|---------|
| **Neon** | Serverless PostgreSQL | Written in Rust |
| **PlanetScale** | MySQL-based (Vitess) | Branching, online schema changes |
| **CockroachDB** | Distributed SQL | Global transactions |
| **Yugabyte** | Distributed PostgreSQL | Strong consistency |

**Practical advice:** Unless you have deep database administration expertise, don't roll your own DB infrastructure. Use a managed provider. Configure read replicas and sharding through their console — the actual replication and distribution mechanics are handled for you.

---

## 4. CDN — Content Delivery Networks

### The Physics Problem

Light travels at ~200,000 km/s through fiber optic cables. The distance from Tokyo to US East (Virginia) is ~20,000 km → minimum round-trip latency: **~100ms**. This is a hard physics floor — no amount of code optimization can beat it.

100ms is just the network overhead before any server processing, DB queries, or external API calls. Combined with processing, Tokyo-to-US latency easily reaches 500–800ms per request.

### What CDNs Do

Cache content at **edge locations** (PoPs — Points of Presence) placed close to users, dramatically reducing the distance a request must travel.

```
User in Tokyo → CDN edge node in Tokyo (~200km, ~2–3ms)
                    vs
User in Tokyo → Origin server in Virginia (~20,000km, ~100ms)
```

**Two primary benefits:**
1. **Latency:** Tokyo user gets response in 2–3ms vs 100ms+ (network portion)
2. **Origin server load:** Traffic served by CDN edge nodes never reaches the primary server → origin handles far less load → less need for horizontal scaling

### What to Cache in CDN

| Content type | Why it's a good CDN candidate |
|-------------|-------------------------------|
| JavaScript bundles | Don't change between deploys |
| CSS bundles | Don't change between deploys |
| Images, videos, fonts | Static, large files (high bandwidth savings) |
| HTML (SPAs) | Single build artifact per deploy |
| API responses (product catalog, etc.) | Rarely changes, read-heavy |

**Cache invalidation in CDN:** When underlying data changes, "purge" the cache. Cloudflare supports tag-based purging — tag content by user ID, content type, etc., and purge all entries matching a tag when data changes.

### DDoS Protection via CDN

DDoS attacks flood your server with traffic from thousands of bots. With CDN (e.g., Cloudflare) in front:

- All traffic passes through Cloudflare first, not your server directly
- Cloudflare detects attack patterns and triggers rate limiting, CAPTCHAs, IP blocking
- Cloudflare's global CDN network absorbs even terabytes/petabytes of attack traffic without saturating your server
- Your server is shielded — it only receives traffic Cloudflare allows through

### Edge Computing

Traditional CDN: user requests a static file → CDN returns it. No processing.

**Edge computing:** CDN node executes code and returns a processed response — processing happens at the edge, close to the user.

**Why it's fast:** Even if the processing itself takes the same time, the reduction in round-trip latency makes the perceived response faster.

**Common edge computing use cases:**

| Use case | How it works |
|----------|-------------|
| Authentication | Check JWT/session at the edge → 401 immediately (2–3ms) instead of 401 after 100ms round trip to origin |
| Localization | Edge node knows the user's region → return Japanese version of the page without hitting origin |
| Request routing | Decide which origin server to route to based on geography, A/B test group, etc. |
| Validation | Reject malformed requests before they hit origin |

**Why edge computing can't replace primary servers — constraints:**
- Limited RAM (~1GB vs 8–32GB for primary servers)
- No filesystem access, limited TCP support
- Short execution time limits
- Cloudflare Workers uses V8 isolates (JavaScript only at high performance)

Edge nodes and primary servers are complementary: edge handles fast, stateless, logic-light operations; primary servers handle stateful, complex, resource-intensive operations.

---

## 5. Asynchronous Processing

**The key insight:** Not every operation needs to complete before sending a response. Operations where the user doesn't need immediate feedback can be offloaded to background workers.

### The Pattern

**Synchronous (bad for slow operations):**
```
User action → Server processing → Wait for external API → Wait for DB → Return response
                                                        ← 400ms total
```

**Asynchronous:**
```
User action → Server core processing (~100ms) → Return 200 immediately
                              ↓
              Push task to queue (Redis, RabbitMQ, Kafka)
                              ↓
              Worker picks up task → calls external API → updates DB (no user waiting)
```

### When to Use Async Processing

The question to ask: *Does the user need to see the result of this operation in the same request-response cycle?*

| Operation | Sync or Async? | Why |
|-----------|---------------|-----|
| Update username | Sync | User expects to see new name immediately |
| Send invitation email | **Async** | User only needs to know the invite was queued; they don't need the email to be delivered before you respond |
| Send notification | **Async** | Notifications don't need millisecond delivery |
| Process uploaded video (thumbnail, encoding) | **Async** | Users expect this to take time |
| Delete account (cascade delete across tables) | **Async** | Potentially seconds of DB work — user doesn't need to watch a spinner |
| Image resizing after upload | **Async** | Background processing is expected |

**Delete account example:**
- Sync approach: delete across 8 tables → 5ms × 8 = 40ms DB work + business logic = ~8 seconds total. User watches spinner for 8 seconds.
- Async approach: verify user, push `delete_user` task to queue → return 200 in ~100ms → user is logged out → worker handles the cleanup in background.

### Implementation

1. Choose a queue: Redis Queue (via BullMQ for Node.js), RabbitMQ, Kafka, Amazon SQS
2. Identify which tasks can be offloaded
3. Producer: server pushes task to queue after completing core processing, returns 200
4. Consumer/worker: separate process picks up task, executes it, handles errors and retries

Workers can be scaled independently from the main application servers — if email volume spikes, add more email workers without scaling the HTTP servers.

---

## 6. Microservices vs Monolith

### Monolith

All functionality (auth, orders, payments, notifications) lives in one codebase and is deployed as a single unit.

**Advantages:**
- Simple to develop, test, debug, and deploy
- All code in one repository
- Refactoring is easy — changes propagate naturally across modules
- Can still be horizontally scaled

**When to use:** Default choice for almost all teams. A monolith horizontally scaled is often sufficient for hundreds of thousands of users.

### Microservices

Application divided into independently deployable services, each handling one domain.

**Primary motivation: team scaling, not machine scaling.** Microservices make sense for 100+ developers where team coordination overhead, independent deployment cadences, and independent scaling needs outweigh the complexity cost.

**Advantages:**

| Advantage | Detail |
|-----------|--------|
| Independent deployment | Payments team deploys without waiting for Notifications team |
| Independent scaling | Scale only the payment service during Black Friday; leave notifications as-is |
| Technology flexibility | Markdown service in Node.js (rich npm ecosystem); image processing service in Go/Rust (CPU performance) |

**Disadvantages (significant):**

| Problem | Detail |
|---------|--------|
| Network latency | Function calls become network calls (gRPC/HTTP); latency and failures must be handled |
| Distributed debugging | A single request touches 4 services; you need distributed tracing to debug it |
| Data consistency | Each service may have its own DB; replication lag across databases |
| Operational complexity | Load balancers, service discovery, health checks, auth across services |

**Decision criteria for microservices:**

| Criterion | Required? |
|-----------|-----------|
| Large team (100+ developers) | Yes — organizational benefit is the primary driver |
| Independent scaling needs | Specific services genuinely need different capacity |
| Technology heterogeneity | Parts of the backend require different languages/runtimes |
| Clear domain boundaries | Services should map to well-defined business domains |

> If you can't answer "yes" to most of these, a monolith (possibly horizontally scaled) is almost always the better choice. Microservices complexity must be justified.

---

## 7. Serverless Computing

### Traditional Server Model

Provision a VM (e.g., AWS EC2, DigitalOcean Droplet), install OS, configure application, deploy. Server runs 24/7 — you pay for uptime regardless of actual usage.

**Problems with traditional servers:**

| Problem | Detail |
|---------|--------|
| Capacity planning | Must predict traffic before deployment — underprovision = crashes; overprovision = wasted spend |
| Autoscaling limitations | Reactive, not proactive — new instances take seconds/minutes to boot; can miss burst traffic |
| Always-on cost | Pay for idle server time even at 3am with zero requests |

### What Serverless Is

No pre-assigned machines. You provide code (functions) and triggers (events/routes). The provider spins up a machine on demand when a request arrives, runs your code, and terminates the instance after a short idle period.

```
Traditional: Server always running → 24/7 cost
Serverless:  No server → Request arrives → Provider spins up instance → Code runs → Response → Instance terminates
             Cost = only CPU/memory time your code actually executed
```

**Architecture:**
```
Request → API Gateway → routes to function → serverless provider spins instance → runs code → response
```

### Advantages

- **No capacity planning:** Provider scales automatically with demand — you don't provision, you just deploy code
- **Pay per execution:** Charged only for actual compute time (milliseconds of CPU/memory usage)
- **Auto-scaling inherent:** Each request can be its own instance in parallel — massive parallelism with no config

### Disadvantages

#### Cold Start Times

When no instance is "warm," the provider must:
1. Boot a VM or container
2. Load your language runtime
3. Execute your code

This takes time — from 100ms to several seconds depending on runtime and provider.

**Mitigation approaches:**
- Keep instances warm via automated pings (but this brings back always-on costs)
- Use runtimes with minimal cold start: Cloudflare Workers (V8 isolates + JS) claims ~0–1ms container startup; AWS Lambda Firecracker microVMs boot faster than traditional VMs
- Use interpreted languages (JS, Python) rather than compiled (Java, Rust) in serverless environments — no compilation overhead at cold start

> Cold start is inherent to serverless — traditional servers are always online and will always win on initial response time.

#### Other Limitations

| Limitation | Detail |
|------------|--------|
| Execution time limits | Lambda: max 15 minutes; Cloudflare Workers: much shorter |
| Statelessness required | Can't maintain TCP connections, in-memory state, or WebSocket sessions across invocations |
| Resource constraints | Limited RAM, no filesystem access, limited TCP support at edge |
| Vendor lock-in | Code written for Lambda doesn't easily port to Cloudflare Workers |

### When to Use Serverless

**Good fits:**
- Event-triggered, infrequent tasks: video/image processing, file uploads, batch jobs
- APIs with very spiky traffic (near-zero most of the time, occasional bursts)
- Scheduled tasks (cron-like functions)
- Webhooks and event processing pipelines

**Poor fits:**
- Latency-sensitive user-facing applications (cold starts hurt UX)
- Long-running requests (file streaming, ML inference, report generation)
- Applications requiring persistent connections (WebSockets, long polling)
- DB-heavy applications (serverless + traditional DB = connection exhaustion problem; need serverless DB like Neon/PlanetScale)

> Serverless is powerful for specific use cases, not a universal replacement for traditional servers. The industry has somewhat overhyped it.

---

## 8. Summary: Mental Models and Thumb Rules

### 8.1 Always Start with Measurement

Before reaching for any scaling solution:
1. **Measure first:** Use distributed tracing, metrics, and logs to find the actual bottleneck
2. **Find the specific problem:** "Database is slow" is not enough — which query, which table, which component?
3. **Then apply the solution:** Targeted solutions are far more effective than guessing

Tools: Prometheus + Grafana (open source), New Relic / Datadog (managed).

**Premature optimization** (implementing scaling solutions before identifying bottlenecks) wastes engineering time and often fails to fix the actual problem.

### 8.2 Prefer Simple Solutions

Complexity is a cost. Every component added:
- Can fail independently
- Must be monitored
- Must be understood by all engineers
- Increases operational burden

**Progression (simplest to most complex):**
```
Database indexing → Caching → Vertical scaling → Connection pooling →
Read replicas → Horizontal scaling → Sharding → Microservices
```

Don't jump to complex solutions when simpler ones suffice. A properly indexed, well-connected database is almost always a better first step than adding Redis.

### 8.3 Scale for Current Problems, Not Hypothetical Ones

Build for the scale you have, with reasonable headroom. Don't build for a million users on day one — most applications never reach that scale.

Netflix-scale solutions don't apply to your early-stage SaaS. Your application has unique characteristics; measure them to understand what actually needs scaling.

### 8.4 Implement Observability from Day One

This is the exception to "prefer simple solutions." Observability is always worth the upfront investment:

- Logs: structured JSON, all environments
- Metrics: request rates, error rates, latency percentiles, DB query times
- Traces: per-request visibility across all components

Without observability, you're guessing at bottlenecks. With it, you know exactly where the problem is and can fix it proactively before it affects users.

### 8.5 Performance is a Mindset

Performance engineering is learned through experience:
- You build systems, watch them struggle under load
- You measure, identify bottlenecks, fix them
- You develop intuition about what breaks at which scale

Your job is not to predict all failure modes — it's to build systems that fail gracefully and give you enough visibility to diagnose and fix problems quickly.

---

## Quick Revision Checklist

- [ ] Statelessness = no data exclusive to one instance; all shared data lives externalized (Redis, S3, shared DB)
- [ ] Sessions → Redis; file uploads → S3/object storage; database → centralized PostgreSQL (not SQLite)
- [ ] Load balancer sits between internet and server instances; distributes requests based on algorithm
- [ ] Round robin: rotating order; simple but mindless about request cost
- [ ] Weighted round robin: proportional to server capacity
- [ ] Least connections: routes to server with fewest active connections; smarter for mixed request weights
- [ ] Health checks: LB pings every instance ~every second; blacklists instances that stop returning 200; re-adds when they recover
- [ ] Read replicas: primary handles writes; replicas handle reads (70–90% of traffic); reduces primary load
- [ ] Replication lag: time for writes to propagate from primary to replicas; can cause stale reads immediately after a write
- [ ] Sharding: physically divide table across multiple DB instances by a sharding key; reduces rows per instance
- [ ] Managed DBs (Neon, PlanetScale, CockroachDB): handle replication, sharding, backups automatically — don't roll your own
- [ ] CDN physics: fiber optic speed = 200,000 km/s; Tokyo-to-Virginia = ~100ms minimum round trip — CDN brings content within ~200km
- [ ] CDN benefits: latency reduction (100ms → 2–3ms for cached content); origin load reduction
- [ ] CDN cache: static assets (JS/CSS/HTML bundles), images, videos, fonts, stable API responses
- [ ] DDoS protection: CDN as a shield layer; absorbs attack traffic before it reaches origin
- [ ] Edge computing: code execution at CDN edge nodes; use for auth checks, routing, localization — not for heavy processing
- [ ] Edge constraints: limited RAM (~1GB), no filesystem, short execution limits
- [ ] Async processing: offload operations where user doesn't need immediate result; return 200 first, process in background
- [ ] Good async candidates: sending emails/notifications, video processing, account deletion, image resizing
- [ ] Queue options: Redis (BullMQ for Node.js), RabbitMQ, Kafka, Amazon SQS
- [ ] Monolith = single deployable unit; simple to develop/test/deploy; default choice
- [ ] Microservices primary benefit = team scaling (100+ devs), independent deployment, independent scaling
- [ ] Microservices costs: network latency, distributed debugging, data consistency across DBs, operational complexity
- [ ] Don't adopt microservices unless you have large team, independent scaling needs, or technology heterogeneity requirements
- [ ] Traditional serverful: always-on, 24/7 cost, capacity planning required, autoscaling is reactive
- [ ] Serverless: pay per execution; no capacity planning; auto-scaling inherent; but cold starts, time limits, statelessness required
- [ ] Cold start: time to spin up a new instance; inherent to serverless; mitigated by warm instances or fast runtimes (V8 isolates, Firecracker)
- [ ] Serverless good fits: infrequent event-triggered tasks, spiky traffic, scheduled jobs
- [ ] Serverless poor fits: latency-sensitive, long-running, WebSocket, DB-heavy applications
- [ ] Observability from day one: logs + metrics + traces; don't wait for scale problems to instrument
- [ ] Premature optimization = implementing solutions before identifying the actual bottleneck; avoid it
- [ ] Complexity has costs: every added component can fail, must be monitored, must be understood
- [ ] Scale for problems you have, not hypothetical ones; Netflix-scale solutions don't apply on day one