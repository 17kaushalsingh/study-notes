# Caching — Lecture Notes

---

## 1. What is Caching?

**One-line definition:** A mechanism to decrease the amount of time and effort it takes to perform some work.

**Technical definition:** Keeping a **subset** of primary data in a location that is faster to access — chosen based on frequency of use, recency, and predicted future access.

**Two scenarios where caching is always relevant:**
1. **Heavy computation** — avoiding re-running expensive algorithms for every request
2. **Heavy data** — avoiding re-fetching or re-transmitting large amounts of data

> Caching is a subset, not a full copy. Full copies are too expensive and defeat the purpose — disk storage is cheap for a reason.

---

## 2. Real-World Examples

### Google Search
- Crawling, indexing, and ranking billions of pages is computationally expensive.
- Queries like "weather today" are searched millions of times daily.
- Without caching: every query triggers the full ranking algorithm → high latency + server overload.
- With caching: results are stored in a distributed in-memory cache. A **cache hit** returns instantly; a **cache miss** runs the algorithm, caches the result, then returns it.

### Netflix (CDN)
- Delivering terabytes of video to millions of global users from a single origin server would be impossible at low latency.
- Netflix uses a **CDN (Content Delivery Network)**: Edge servers spread geographically across the world cache popular content close to users.
- Content is cached at **Edge locations** (also called Points of Presence / PoPs). Users are routed to the nearest PoP → minimal buffering.

### Twitter / X (Trending Topics)
- Computing trending topics requires analyzing billions of tweets using ML algorithms — extremely expensive.
- If done per-request: servers would crash instantly.
- Instead: computed every few minutes, stored in an in-memory cache (like Redis). All users read from the cache.
- **Why it's safe to cache:** Trending topics don't change second-to-second; a few minutes of staleness is acceptable.

---

## 3. Three Levels of Caching (Backend-Relevant)

### 3.1 Network-Level Caching

#### CDN (Content Delivery Network)

```
User Request
     ↓
DNS resolves domain → routes to nearest PoP (Point of Presence)
     ↓
Edge Server checks cache:
  Cache Hit  → return content instantly
  Cache Miss → fetch from Origin Server → cache it → return to user
```

- **PoP** = cluster of Edge servers in a geographic region
- **TTL (Time to Live)** = how long content stays cached before the Edge server fetches a fresh copy from the Origin
- Used for: video files, images, static web assets (HTML/CSS/JS), API responses

#### DNS Caching

DNS resolving a domain name to an IP address is a multi-step recursive process:

```
User → OS local cache → Browser cache → Recursive Resolver (ISP / Google / Cloudflare)
     → Root Server → TLD Server (.com) → Authoritative Name Server → IP address
```

DNS caching is layered — each level tries its cache before going deeper:

| Layer | Cache Location |
|-------|---------------|
| Operating System | Local DNS cache (all OSes maintain this) |
| Browser | Chrome, Firefox maintain their own DNS cache |
| Recursive Resolver | ISP or public DNS (Google 8.8.8.8, Cloudflare 1.1.1.1) |
| Authoritative Server | Some also cache responses |

DNS is "recursive" because the resolver keeps querying deeper servers until it finds the answer. Caching at each layer avoids repeating this expensive traversal.

---

### 3.2 Hardware-Level Caching

```
CPU Core → L1 Cache → L2 Cache → L3 Cache (shared) → RAM → Disk
           (fastest)                                  (slower) (slowest)
```

- CPUs use predictive algorithms to pre-load data into L1/L2/L3 caches.
- **Why arrays are fast for sequential access:** CPU predicts the traversal pattern, prefetches the entire array block into L1/L2 cache.
- This is handled entirely by hardware — not something backend engineers configure directly.

---

### 3.3 Software-Level Caching (In-Memory Databases)

The primary focus for backend engineers. Technologies: **Redis**, **Memcached**, **AWS ElastiCache**.

#### Why In-Memory?

| Storage | Access Speed | Capacity | Volatile? |
|---------|-------------|----------|-----------|
| RAM (primary memory) | Very fast — direct electrical access, O(1) regardless of address | Limited (8–128 GB) | Yes — cleared on power loss |
| Disk (secondary) | Slower — mechanical (HDD) or flash (SSD) access | Huge (TBs) | No — persistent |

**RAM = Random Access Memory** → access time is constant regardless of which address is accessed. No mechanical seeking.

Redis and Memcached store data in RAM → very fast reads/writes. For persistence, they periodically flush to disk and reload on startup — but all active operations happen in RAM.

#### Structure: Key-Value Store

Unlike relational databases (tables, rows, strict schema), in-memory caches use a simple key-value model:

```
SET user:profile:123  →  { name: "Alice", role: "admin" }
GET user:profile:123  →  { name: "Alice", role: "admin" }
```

The value can be a string, number, JSON, list, set, hash — depending on the technology. No SQL, no schema, no joins.

---

## 4. Caching Strategies

### 4.1 Lazy Caching (Cache-Aside)

The most common pattern in backend development.

```
Request comes in
       ↓
Check cache:
  Cache Hit  → return from cache (fast)
  Cache Miss → fetch from DB/source → store in cache → return result
```

- Data is only cached **when first requested** — no proactive pre-warming.
- Simple to implement; cache naturally fills with frequently accessed data.
- Risk: first request after a cache miss is slower (goes to DB).

---

### 4.2 Write-Through Caching

Every write operation updates **both** the database and the cache simultaneously.

```
Client writes data
       ↓
Update DB + Update Cache (in the same execution flow)
       ↓
Next read → always a cache hit (cache is always fresh)
```

- **Advantage:** Cache is never stale — always in sync with the database.
- **Disadvantage:** Write operations are slower (two writes instead of one) and more complex.
- Best for: data that is read very frequently and must always be up-to-date.

---

## 5. Cache Eviction Policies

In-memory caches have limited capacity. When the cache is full, old data must be evicted to make room for new data.

| Policy | Mechanism |
|--------|-----------|
| **No Eviction** | New inserts fail when cache is full — rarely useful |
| **LRU (Least Recently Used)** | Evict the key that was accessed longest ago |
| **LFU (Least Frequently Used)** | Evict the key with the lowest access count |
| **TTL-Based** | Evict keys whose Time-to-Live has expired (or is expiring soonest) |

**LRU example:** Keys 1, 2, 3, 4 in cache. Keys 1/2/3 accessed today; key 4 accessed yesterday. New key 5 arrives → evict key 4 (least recently used).

**LFU example:** Key 1 accessed 5 times, key 2 ten times, key 3 six times, key 4 twenty-three times. New key 5 arrives → evict key 1 (least frequently used).

**TTL:** Each key has a configured expiry time. Expired keys are evicted automatically, or when eviction is needed, keys expiring soonest are evicted first.

---

## 6. Use Cases for Redis in Backend Systems

### 6.1 Database Query Caching

```
Expensive SQL query (many joins, aggregations, millions of rows)
       ↓
First request: run query → cache result in Redis with TTL (e.g., 1 hour)
Next requests: return from Redis (no DB hit)
On data change: manually invalidate or update the cache key
```

**When to apply:** Read-heavy operations where data changes infrequently and the query is expensive. Examples: dashboards, landing pages, product listings (Amazon), analytics reports.

---

### 6.2 Session Storage

```
User logs in → server creates session token → stores in Redis
Every subsequent request → middleware fetches session from Redis (fast)
```

**Why Redis over the DB?**
- Session data is fetched on **every** API request — even a 20ms DB lookup adds up at scale.
- Redis lookup is sub-millisecond vs 5–50ms for a DB query.
- Reduces database load dramatically.

---

### 6.3 External API Caching

```
Client → Your server → External API (weather, maps, payment, etc.)
```

External APIs have rate limits and per-call pricing. If 1,000 users hit your endpoint per minute, you'd make 1,000 external API calls.

**With caching:**
```
First request: call external API → cache result with TTL (e.g., 1 hour)
Next 999 requests: serve from cache → 1 external call per hour instead of 1,000
```

**When to apply:** Any external API call for data that doesn't change in real-time (weather, exchange rates, product data from third-party APIs).

---

### 6.4 Rate Limiting

Rate limiting tracks how many requests a client (identified by IP) has made in a time window.

```
Request arrives → middleware reads X-Forwarded-For header → extracts client IP
       ↓
Check Redis: { "10.0.0.1": counter }
  counter < limit → increment counter → allow request
  counter ≥ limit → reject with 429 Too Many Requests
```

**Why Redis and not the database?**
- Rate limit check happens on **every single request** — this would flood the DB with unnecessary queries.
- Redis increments are atomic and sub-millisecond.
- Keeps the database free for actual business operations.

**TTL on the counter key** = automatically resets the counter each time window (e.g., expire after 60 seconds for per-minute rate limiting).

---

## 7. Key Terminology

| Term | Definition |
|------|-----------|
| **Cache Hit** | Requested data found in cache — served instantly |
| **Cache Miss** | Requested data not in cache — must fetch from primary source |
| **TTL (Time to Live)** | How long a cache entry is valid before it expires |
| **Eviction** | Removing data from cache to make room for new data |
| **Edge Server / Edge Location** | CDN server geographically close to the user |
| **PoP (Point of Presence)** | Cluster of Edge servers in a geographic region |
| **Origin Server** | The authoritative server holding the actual data |
| **Recursive Resolver** | DNS component that queries multiple servers to resolve a domain |
| **In-Memory Database** | Database storing data in RAM (Redis, Memcached) — fast but limited capacity |
| **LRU** | Least Recently Used eviction policy |
| **LFU** | Least Frequently Used eviction policy |
| **Write-Through** | Cache updated synchronously with every DB write |
| **Lazy / Cache-Aside** | Cache populated only on cache miss |

---

## Quick Revision Checklist

- [ ] Caching = subset of data in faster storage to reduce time and effort of retrieval
- [ ] Two main triggers: heavy computation OR heavy data transfer
- [ ] Cache hit = found in cache; Cache miss = not found, must fetch from source
- [ ] Network level: CDN (edge servers, PoP, TTL) and DNS (layered caching across OS → browser → resolver)
- [ ] Hardware level: L1/L2/L3 cache managed by CPU predictive algorithms (not directly controlled by backend)
- [ ] Software level: Redis/Memcached — in-memory (RAM), key-value, NoSQL
- [ ] RAM = fast + limited + volatile; Disk = slow + large + persistent; Redis uses RAM for ops, disk for persistence
- [ ] Lazy caching: populate on miss; Write-through: update cache on every write
- [ ] Eviction policies: No eviction, LRU, LFU, TTL-based
- [ ] Redis use cases: DB query caching, session storage, external API caching, rate limiting
- [ ] Rate limiting stores IP → counter in Redis with TTL for auto-reset; rejects with 429 when limit exceeded
- [ ] Cache the data that is: read-heavy, write-infrequent, expensive to compute, and safe to be slightly stale