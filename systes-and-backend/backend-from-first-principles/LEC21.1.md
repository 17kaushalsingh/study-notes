# Backend Scaling & Performance Engineering — Part 1

---

## 1. Core Mindset

> **Never guess. Always measure.**

Most engineers jump straight to solutions (add caching, upgrade DB, add more servers) without identifying the actual bottleneck first. This results in spending a week solving a problem you don't have, while the real cause remains untouched. Identify the bottleneck before implementing any solution.

---

## 2. Performance Metrics

### 2.1 Latency

**Definition:** The total time from a user's action (e.g., button click) to the final result appearing on screen. Includes: network round trip + server processing + DB query + response transmission + browser rendering.

**Why averages are useless for latency:**

Consider 1,000 requests where 99% complete in 50ms but 1% take 5 seconds. The average looks healthy (~100ms) but 10,000 users/day are waiting 5 seconds. Averages mask outliers that represent your worst user experience.

**Use percentiles instead:**

| Metric | Meaning |
|--------|---------|
| **P50** | 50% of requests complete at or below this time. The median. |
| **P90** | 10% of requests experience this latency or worse. |
| **P95** | 5% of requests experience this latency or worse. |
| **P99** | 1% of requests experience this latency or worse. |

**Why focus on P99 and P95?**
- Requests with the worst latency usually contain your most complex business logic, complex DB queries, and external service calls
- These users are often your most valuable customers — payment flows, checkout, order processing — not casual browsing
- A user paying for something generates a more complex query than someone viewing a homepage

---

### 2.2 Throughput

**Definition:** How many requests your system can handle per unit time. Typically measured in **requests per second (RPS)** or requests per minute.

**Why it matters independently of latency:**
- A system might handle 10 RPS at 150ms latency — but at 1,000 RPS, latency spikes to 2 seconds
- Answers practical questions: Can we handle Black Friday traffic? Can we survive a viral surge from a podcast feature? How many concurrent users can we support?

**Relationship between throughput and latency:**
As throughput increases, latency increases slowly at first, then grows exponentially near maximum capacity. This is a critical, counter-intuitive relationship to internalize.

---

### 2.3 Utilization

**Definition:** The percentage of your system's capacity currently in active use. 0% = idle; 100% = maxed out.

**The key insight — utilization vs latency curve:**

```
Latency
  ^
  |                                  ╱
  |                              ╱
  |                          ╱
  |                      ╱
  |_________________╱___________________→ Utilization
                                      100%
```

Latency grows **exponentially** as utilization approaches 100%, not linearly. The highway analogy:
- 50% capacity: traffic flows freely, overtaking is easy
- 80% capacity: slowdowns appear, lane changes require more thought
- 90% capacity: highly unpredictable — either flows or jams based on one person braking
- 100% capacity: complete gridlock, nothing moves

**Practical rule:** Run production systems at **60–80% utilization max**. Reserve 20–40% headroom for:
- Traffic bursts (real traffic never arrives evenly — it comes in spikes)
- A single traffic spike can instantly push a 60%-loaded system to 100% and cause collapse

---

## 3. Finding Bottlenecks

**Bottleneck:** The specific component in your system causing the slowness.

### Why engineers skip this step

The temptation is to jump to standard solutions:
- System is slow? → Add caching
- DB seems slow? → Upgrade PostgreSQL version
- Server seems overloaded? → Add more servers (horizontal scale)

These sometimes work by luck. More often, you spend a week implementing a solution to a problem you don't actually have.

**Real example:**
An API seemed slow. Assumption: database queries are slow. Solution implemented: added Redis caching. Result: API still slow. Root cause when actually measured: a logging function was writing to a remote logging service **synchronously** — blocking the entire request for 500ms. The DB queries were 10ms. The cache was 5ms. The culprit was never suspected.

Other unintuitive bottlenecks found in practice:
- JSON serialization of large payloads
- A loop making external API calls (one per item)
- Response payload too large → network transmission is the bottleneck
- XML parsing step
- Connection pool exhaustion

### Tools for finding bottlenecks

#### Profiling

Attaches to a running application and samples what's executing at every moment during request serving. Output shows which functions consume the most CPU time.

**Represented as a flame graph:** wider bars = more time spent in that function; stacked bars = function call hierarchy. Lets you visually spot where time is spent.

**Limitation:** Profilers measure CPU-bound work well. Most backend performance problems are **I/O-bound** (DB queries, external API calls, file I/O, serialization) — not CPU-bound. CPU profilers are not well-suited to measuring I/O wait times.

#### Distributed Tracing

Follows a single request through every component of your system and records timing at each step:
- When did it enter the handler?
- When did the DB query start and finish?
- When did the external API call start and finish?

Output example:
```
POST /products/5
  → handler logic:        2ms
  → DB query:           800ms   ← bottleneck
  → response formatting:  3ms
```

Now you know to focus on the DB query, not on adding caching or upgrading servers.

**I/O-bound problems:** Distributed tracing is the right tool for typical backend bottlenecks (DB queries, external services, queue interactions).

---

## 4. Database Performance

Databases are the most common performance bottleneck in backend systems. They do heavy work: durable storage on disk, consistency guarantees, concurrent access management, complex query execution across millions of rows.

### 4.1 The N+1 Query Problem

**The pattern:**
```
# Fetch N posts
posts = db.select(posts)

# Then for each post, fetch the author separately
for post in posts:
    author = db.select(users, where=id == post.author_id)
```

For N posts, this makes **N+1 database queries**. At 1,000 posts with 5ms per query = 5,000ms = 5 seconds of loading time.

**Why it happens with ORMs:** ORM code looks like normal application code. A loop of `.findById()` calls doesn't look like a performance problem — it looks like regular iteration. But each call is a round-trip to the DB.

**Fix: Bulk fetch / JOIN**

```
# Two queries regardless of N
posts = db.select(posts)
author_ids = [p.author_id for p in posts]
authors = db.select(users, where=id IN (author_ids))
```

Or with raw SQL: use JOIN. With ORMs: use `select_related` (Django), `includes` (Rails), `with` (TypeScript ORMs like Prisma/Drizzle). Always prefer bulk fetching over iterative per-row fetching.

**Tip:** Enable ORM query logging in development to see the actual SQL being generated. What looks like two lines of code may be generating hundreds of queries.

---

### 4.2 Indexes

**The library analogy:** A library with a million books and no catalog requires walking every shelf to find books by a specific author (full table scan). A catalog organized by author name lets you jump directly to the right location (index scan).

**What an index is:** A B-tree data structure that maintains a sorted copy of column values with pointers to the actual rows. Allows the database to find rows matching a condition without scanning every row.

**Performance impact:**
- Without index on author_id: full table scan → ~4 seconds on 1M rows
- With index on author_id: index scan → ~40–100ms on 1M rows

**Indexes are not free — two costs:**

| Cost | Detail |
|------|--------|
| **Storage** | Index data structure takes disk space; grows with table size |
| **Write overhead** | Every INSERT, UPDATE, DELETE on the table must also update all associated indexes → slower write operations |

**Too many indexes = slow writes.** Don't index every column.

**When to add indexes:**
- Obvious ones upfront: foreign key columns frequently used in JOINs or WHERE clauses (e.g., `author_id` on posts table)
- Non-obvious ones: let production data guide you — use distributed tracing to find slow queries, then check the query plan

**Primary key is automatically indexed** in PostgreSQL — don't add a manual index on `id`.

**Using EXPLAIN ANALYZE:**
Run `EXPLAIN ANALYZE <your query>` to see the query execution plan:
- `Seq Scan` = full table scan → index missing
- `Index Scan` = using an index → good

Use this to confirm which columns need indexing, and to verify your index is actually being used after creation.

#### Composite Index

An index on multiple columns together. Example: `CREATE INDEX ON posts (user_id, created_at)`.

- Helps queries filtering by both `user_id` AND `created_at`
- Also helps queries filtering by `user_id` alone (leftmost prefix rule)
- Does NOT help queries filtering by `created_at` alone
- More efficient than two separate single-column indexes for combined queries

#### Covering Index

An index that includes all columns needed by a query. The DB can serve the result entirely from the index without touching the main table.

```sql
-- If you always query: SELECT id, name FROM departments WHERE ...
-- A covering index on (name) INCLUDE (id) serves this without a table lookup
```

Trade-off: covering indexes are larger. Only worth it for very high-frequency read queries.

---

### 4.3 Connection Overhead and Pooling

**What happens when your app connects to the database:**
1. TCP three-way handshake (SYN, SYN-ACK, ACK)
2. Authentication
3. Encryption negotiation (key exchange)
4. Session state setup
5. Database allocates memory for the connection

All of this happens for every new connection. If your app creates a new connection for every query and closes it immediately after, you pay this overhead on every single request.

**PostgreSQL connection limits:** Typically 400–500 max connections depending on server resources. Under traffic spikes, your backend can exhaust this limit and crash the database.

**Solution: Connection Pooling**

A pool maintains a set of pre-established, persistent connections to the DB. Your app borrows a connection from the pool, executes the query, returns it. No setup/teardown overhead per query.

#### Internal Pooling (Application-level)

Built into the DB driver. Each server instance maintains its own pool. Simple to set up.

**Problem at scale:** With 3 server instances each maintaining a pool of 150 connections → 450 total connections. If DB max is 300, a traffic spike causes connection exhaustion and DB crash.

#### External Pooling (e.g., PgBouncer for PostgreSQL)

A dedicated pooler process sits between all server instances and the DB. All instances share one centralized pool.

```
Server instance 1  ─┐
Server instance 2  ─┼→ PgBouncer (shared pool, 250–300 connections) → PostgreSQL
Server instance 3  ─┘
```

**Advantages:**
- Total connections to DB stays bounded regardless of how many server instances you spin up
- Prevents connection limit exhaustion during autoscaling events
- Lightweight open-source tool (PgBouncer, similar tools exist for other DBs)

**Production recommendation:** Use external pooling (PgBouncer) when running multiple server instances or deploying with Kubernetes autoscaling.

---

## 5. Caching (Performance Context)

> Once you've optimized queries, added appropriate indexes, and configured connection pooling — and the DB is still the bottleneck — the next step is caching.

**Core idea:** Store the result of expensive operations. On subsequent requests, serve from cache instead of re-executing the operation.

Example: A complex query taking 800ms → cached result returns in ~50ms. 16× latency improvement.

---

### 5.1 Cache Invalidation

> "There are only two hard problems in programming: naming things and cache invalidation."

Cache becomes problematic when the underlying data changes. Serving stale cached data = incorrect user experience.

**Two primary invalidation strategies:**

#### Time-Based Expiration (TTL)

Set an expiry time on cache entries. After TTL expires, next request goes to the DB and refreshes the cache.

- **Pro:** Simple, automatic, no code changes needed for updates
- **Con:** Choosing the right TTL is hard — too short = frequent DB hits, too long = stale data risk. Requires understanding access patterns per endpoint

#### Event-Based Invalidation

Explicitly delete or update cache entries whenever the underlying data changes.

```python
# In the update handler:
db.update(user, id=user_id, name=new_name)  # update DB
cache.delete(f"user:{user_id}")             # invalidate cache

# On next GET request:
# cache miss → fetch from DB → cache fresh result
```

- **Pro:** Never serves stale data; no TTL guessing required
- **Con:** Must remember to invalidate at every code path that modifies data. Missing one location → stale cache bug

---

### 5.2 Cache Location: Local vs Distributed

| | Local Cache | Distributed Cache (Redis, Memcached) |
|--|------------|--------------------------------------|
| **What** | In-process data structure (dictionary/map) in server memory | External caching service on separate process |
| **Latency** | ~2–3ms (no network) | ~50ms+ (network round-trip) |
| **Consistency** | ❌ Each server instance has its own cache — 10 servers = 10 different caches | ✅ All instances share one cache |
| **Use case** | Single-server deployments; "hot" data that rarely changes | Multi-server (horizontal scale) deployments |

**Tiered caching:** Use both. Local cache holds the "hottest" data (LRU/LFU). Distributed cache is the upstream source. Request flow:
```
Check local cache → miss → check distributed cache → miss → hit DB → populate both caches
```

---

### 5.3 Caching Patterns

#### Cache-Aside (Lazy Loading) — Most Common

```
Request arrives
  → Check cache: HIT → return cached result
  → Check cache: MISS → query DB → store result in cache → return result

On UPDATE:
  → Update DB
  → Delete/invalidate cache entry
```

- Read-optimized, simple to understand
- Stale data window exists between update and cache miss
- Most commonly used in practice

#### Write-Through

```
On WRITE:
  → Update DB AND cache simultaneously
  → Return success after both complete
```

- **Pro:** Cache is always fresh, no cache misses on reads
- **Con:** Every write operation has extra latency (writing to two stores before responding)

#### Write-Behind (Write-Back)

```
On WRITE:
  → Update cache immediately → return success
  → Asynchronously update DB in background
```

- **Pro:** Write latency is minimal (only writing to fast cache)
- **Con:** If DB update fails, cache and DB are inconsistent — complex to recover

---

### 5.4 Cache Hit Rate

**Definition:** Percentage of requests served from cache vs total requests.

A 90% cache hit rate means 90% of requests never reach the DB. A 20% hit rate means your caching strategy has a fundamental problem.

**Factors that affect cache hit rate:**

| Factor | Detail |
|--------|--------|
| **TTL** | Longer TTL → more hits, more staleness risk |
| **Cache size** | More memory → more entries → more potential hits |
| **Data access patterns** | Caching the right data requires understanding which endpoints are hit frequently and when |

High hit rate requires understanding your users' behavior — which endpoints they hit, at what times, with what data. Without this understanding, caching effort has diminishing returns.

---

## 6. Scaling: Vertical vs Horizontal

### 6.1 Vertical Scaling (Scale Up)

**What:** Replace your existing server with a more powerful one. Upgrade CPU, RAM, storage, network card.

**Advantages:**
- Zero code changes required — architecture stays the same
- Linear capacity increase: 2× CPU ≈ 2× request handling capacity
- No load balancer, no distributed state management
- Economically efficient: one 2× machine typically costs less than two 1× machines
- No operational overhead of managing multiple machines

**Disadvantages:**

| Limit | Detail |
|-------|--------|
| **Hard ceiling** | Cloud providers have a maximum instance size. Once you hit it, you can't scale further |
| **Single point of failure** | If the server crashes, entire service goes down. Mitigation (standby servers, failover) adds complexity |
| **No geographic distribution** | One server in US → Indian users experience higher latency regardless of server power |

---

### 6.2 Horizontal Scaling (Scale Out)

**What:** Add more instances of the same server. Multiple medium servers work together instead of one large server.

**Advantages:**

| Advantage | Detail |
|-----------|--------|
| **No hard ceiling** | No limit on how many instances you can add |
| **Linear throughput** | 1 server at 1,000 RPS → 5 servers at ~5,000 RPS (theoretical) |
| **Redundancy** | If one instance fails, others absorb its traffic — no downtime |
| **Geographic distribution** | Place instances in different regions — route users to nearest instance → lower latency |
| **On-demand scaling** | Add instances before known traffic spikes (campaigns, sales); remove after |

**Disadvantages — introduced complexity:**

| New Problem | What it requires |
|------------|-----------------|
| Traffic distribution | Load balancer (new infrastructure component) + choice of algorithm |
| State synchronization | How does server 2 know about user updates made on server 1? |
| Network partition | What happens when servers can't communicate? Risk of conflicting decisions |
| Health detection | How do you know when an instance is down? How do you stop routing to it? |
| Session/auth state | Sessions stored on server 1 are unavailable on server 2 (statelessness requirement) |

> Horizontal scaling doesn't eliminate problems — it transforms one set of problems (resource limits) into a different set (distributed systems complexity). Sometimes the new problems are more favorable; sometimes not. The choice depends on which tradeoffs you'd rather deal with.

---

## 7. Terminology Reference

| Term | Definition |
|------|-----------|
| **Latency** | Time from request initiation to response received |
| **P50/P90/P95/P99** | Percentile latency; P99 = 1% of users experience this latency or worse |
| **Throughput** | Requests processed per unit time (RPS) |
| **Utilization** | % of system capacity currently in use |
| **Bottleneck** | The specific component limiting overall system performance |
| **N+1 query** | Fetching N items then making N additional individual queries for related data |
| **Full table scan / Sequential scan** | DB reads every row to find matches — no index used |
| **Index scan** | DB uses index to jump directly to matching rows |
| **Composite index** | Index on multiple columns; column order matters |
| **Covering index** | Index includes all columns needed by a query; DB never touches main table |
| **Connection pooling** | Reusing pre-established DB connections instead of creating/closing per query |
| **PgBouncer** | External connection pooler for PostgreSQL |
| **Cache hit** | Request finds result in cache — no DB needed |
| **Cache miss** | Result not in cache — must query DB |
| **Cache hit rate** | % of requests served from cache |
| **TTL** | Time-to-live — how long before a cache entry expires |
| **Cache invalidation** | Process of removing/updating stale cache entries |
| **Cache-aside** | Cache populated on miss; invalidated on write |
| **Write-through** | Cache updated synchronously with every DB write |
| **Write-behind** | Cache updated first; DB updated asynchronously |
| **Vertical scaling** | Making one server more powerful (more CPU/RAM/storage) |
| **Horizontal scaling** | Adding more server instances that work together |
| **Flame graph** | Visual representation of profiler output; wider = more time spent |
| **Distributed tracing** | Tracking a request through all system components with per-component timing |

---

## Quick Revision Checklist

- [ ] Latency = time for one request end-to-end; varies per request — never use averages for latency analysis
- [ ] P50 = median; P99 = 1% of users experience this latency or worse; focus on P95/P99 in production
- [ ] P99 requests represent your most valuable customers (complex workflows like payments)
- [ ] Throughput = RPS; latency and throughput are both needed to answer "can we handle the traffic spike?"
- [ ] Utilization vs latency: relationship is exponential near 100%, not linear
- [ ] Run production systems at 60–80% utilization; keep headroom for traffic bursts
- [ ] Identify bottleneck before implementing solutions — don't guess, always measure
- [ ] Tools: profiling (CPU-bound work) + distributed tracing (I/O-bound work)
- [ ] Most backend performance problems are I/O-bound, not CPU-bound
- [ ] N+1 query: 1 query for N items + N queries for related data → fix with JOINs or ORM bulk fetch
- [ ] Enable ORM query logging in development to catch N+1 problems early
- [ ] Indexes: B-tree structure, dramatically faster reads (full table scan vs index scan: ~4s → ~100ms on 1M rows)
- [ ] Index costs: storage space + write overhead (every INSERT/UPDATE/DELETE updates all indexes)
- [ ] Don't index every column; add indexes for known high-frequency query columns first, then use EXPLAIN ANALYZE
- [ ] EXPLAIN ANALYZE: shows whether query uses full table scan or index scan; confirms index is being used
- [ ] Composite index: multi-column index; leftmost prefix rule determines which queries benefit
- [ ] Covering index: serves query entirely from index, no table lookup needed
- [ ] Primary key is auto-indexed in PostgreSQL — don't manually index `id`
- [ ] Connection pooling: reuses established connections; avoids TCP handshake cost per query
- [ ] PostgreSQL max connections ~400–500; easily exhausted during horizontal scale + traffic spikes
- [ ] Internal pooling (per-server): simple but each instance has its own pool → overflow risk
- [ ] External pooling (PgBouncer): single shared pool for all server instances → connection limit never exceeded
- [ ] Use external pooler in production with horizontal scaling / Kubernetes autoscaling
- [ ] Cache invalidation: time-based (TTL) or event-based (explicit delete on write)
- [ ] TTL: simple but requires tuning; event-based: precise but must invalidate at every write location
- [ ] Local cache: fast (no network) but inconsistent across server instances
- [ ] Distributed cache (Redis): consistent across all instances but adds ~50ms network latency
- [ ] Tiered caching: local cache for hottest data + Redis as upstream
- [ ] Cache-aside (most common): check cache → miss → query DB → populate cache → return; delete on write
- [ ] Write-through: write DB + cache together; always fresh but slower writes
- [ ] Write-behind: write cache first, async DB update; fast writes but risk of inconsistency
- [ ] Cache hit rate: target 80–90%+; low rate = wrong data being cached or wrong strategy
- [ ] Vertical scaling: simple, no code changes, but has hard ceiling, single point of failure, no geo distribution
- [ ] Horizontal scaling: no hard ceiling, redundancy, geo distribution, but requires load balancer + distributed state management
- [ ] Horizontal scaling transforms resource problems into distributed systems problems — both have tradeoffs