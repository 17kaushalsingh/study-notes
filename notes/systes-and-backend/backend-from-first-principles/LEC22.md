# Concurrency & Parallelism: IO-Bound vs CPU-Bound

---

## 1. The Core Problem

A backend server that processes only one request at a time is useless in production. While waiting for a database response, the server should be doing something else — handling other requests, logging, making external API calls, etc.

**The waste without concurrency:**

A modern CPU executes ~3 billion instructions/second = 3 million instructions/millisecond. If a server waits 100ms for a database response (different region), it wastes the opportunity to execute **300 million instructions**. With synchronous single-task processing, it executes zero.

**Quantifying it in a typical API call:**

| Activity | Time |
|----------|------|
| CPU processing (validation, JSON parsing, business logic) | ~10ms |
| Waiting for IO (3–5 DB queries + external API calls at ~50ms each) | ~250ms |

**Result:** 95% of the time, the server is doing nothing — just waiting for IO responses. Concurrency exists to make productive use of that 95%.

---

## 2. IO-Bound vs CPU-Bound

### IO-Bound

Work where the bottleneck is **waiting for external responses**, not CPU computation.

Examples: database queries, external API calls, file system reads/writes, Redis/cache interactions, sending emails, logging to remote services.

> CPU is idle during IO — it has sent a request and is waiting. More CPU cores don't help here.

### CPU-Bound

Work where the bottleneck is **actual computation** using CPU cycles.

Examples: image/video processing (matrix multiplication), encryption/JWT verification, JSON serialization of large payloads, ML inference, string parsing.

> CPU is fully busy — more cores directly speed up the work.

**Key insight for backend:** Most SaaS backend applications are **IO-bound**, not CPU-bound. The bottleneck is almost always database queries, external API calls, or network operations — not computation.

---

## 3. Concurrency vs Parallelism

### Concurrency

**Doing multiple things at once** using structured program execution — start, pause, and resume multiple tasks to avoid wasting CPU during IO waits. Can be achieved with **a single CPU core**.

At any given moment, only one task is actually executing on the CPU, but multiple tasks are "in progress" (some waiting for IO, some running). From the outside, all look active simultaneously.

### Parallelism

**Truly simultaneous execution** of multiple tasks at the same physical moment. Requires **multiple CPU cores** — one instruction per core per clock cycle.

```
Concurrency (1 core):            Parallelism (2 cores):
  Task A ─────── IO wait ──────    Task A ─────────────────
  Task B ────────────── running    Task B ─────────────────
  (only one at a time, but        (both truly simultaneous)
   both "in progress")
```

**Summary:**
- Concurrency = *dealing* with multiple things at once (structure)
- Parallelism = *doing* multiple things at the exact same moment (hardware)

---

## 4. Visualizing Concurrency (Single Core)

```
Timeline (ms):   0    5    10   15   20   25   30   35   40   45   50   55   60

Request A:       [CPU]|..IO waiting (DB query).......................|[CPU]|→ response

Request B:            |[CPU processing]|..IO waiting (DB)........|[CPU]|→ response
```

- Request A uses CPU for 5ms → starts DB query → CPU goes to Request B
- Request B uses CPU → starts its own DB query → CPU goes elsewhere
- DB response for A arrives at ~40ms → waits for scheduling → gets CPU at 50ms → responds
- Both requests were "in progress" the entire time, but only one used CPU at any moment
- **No wasted CPU cycles**

---

## 5. Two Mechanisms for Concurrency

### 5.1 OS Threads (Kernel Threads)

An OS thread is an **independent unit of execution managed by the operating system**.

When created, the OS allocates:
- **Stack:** stores function call history, local variables (~8MB on Linux, though mostly virtual/lazy allocated)
- **Instruction pointer:** tracks which instruction the thread was executing (so it can resume after context switch)
- **Bookkeeping data structures** for scheduling

**Scheduler:** OS kernel's scheduler assigns each thread a **time slice** (a few milliseconds). When the slice expires, the scheduler **preempts** the thread — pauses it and switches to another. This is **preemptive scheduling** — threads don't voluntarily give up the CPU.

**When a thread blocks on IO:**
1. Thread calls a blocking IO operation (e.g., read on a DB socket)
2. OS marks that thread as `BLOCKED`
3. Scheduler immediately switches to another runnable thread
4. When IO completes (DB responds), OS marks thread as `RUNNABLE` again
5. Scheduler picks it up in its next cycle

**Thread overhead (why this matters at scale):**

| Overhead | Cost |
|---------|------|
| **Memory** | Each thread needs a stack (kilobytes to megabytes). 10,000 threads × 1MB = 10GB RAM just for stacks |
| **Creation** | System call to kernel → kernel allocates stack, sets up data structures, registers with scheduler → few μs to few ms |
| **Context switch** | Save CPU registers + bookkeeping of current thread, load saved state of next thread → 1–10μs per switch. With 100+ threads, this adds up to non-trivial latency |

**Threads + multiple cores = parallelism:** Thread 1 on Core 1, Thread 2 on Core 2 — executing simultaneously. This is true parallelism, ideal for CPU-bound work.

**Problem:** Threads-per-request model breaks at high concurrency (10k+ concurrent requests) due to memory exhaustion and excessive context switching overhead.

**Shared memory:** Threads within the same process share heap memory. Thread 1 can access an object allocated by Thread 2 via pointer. Fast communication but enables race conditions (see Section 7).

---

### 5.2 Event Loop

**One thread, no blocking.** The event loop model runs on a single thread and uses callbacks + OS IO notification APIs to handle thousands of concurrent IO operations efficiently.

**Key rule:** Never block the event loop. If any operation occupies the CPU for extended time (CPU-bound work), all other tasks are frozen.

**How it works:**

```
Event loop iteration:
  1. Check for completed IO operations (via epoll/kqueue/OS API)
  2. For each completed IO: run its registered callback
  3. Go back to step 1 (runs continuously = "loop")
```

When a task starts an IO operation:
1. It **registers a callback** — "when this IO completes, run this function"
2. It **hands control back** to the event loop
3. The event loop moves on to other tasks
4. When OS signals IO completion, the callback is queued and eventually executed

**OS support:**
- Linux: `epoll` — efficiently monitors thousands of file descriptors/sockets
- macOS: `kqueue`
- Windows: IOCP

These let the event loop check all pending IO operations in one call, without spinning or polling wastefully.

**Why event loop is more efficient than threading for IO-bound work:**
- No context switch overhead (single thread)
- No megabytes of stack memory per request
- Can handle thousands of concurrent IO operations cheaply
- Scales to very high concurrency

**The fundamental trade-off:** CPU-intensive code blocks the whole loop. A 100ms synchronous computation freezes all other tasks.

**Evolution of syntax:**

```javascript
// Old style (callback hell):
db.query("SELECT * FROM users WHERE id = ?", [userId], function(err, user) {
  db.query("SELECT * FROM orders WHERE user_id = ?", [user.id], function(err, orders) {
    // nesting keeps growing...
  });
});

// Modern style (async/await — syntactic sugar over callbacks):
async function fetchUser(userId) {
  const user = await db.getUser(userId);    // hands control to event loop here
  const orders = await db.getOrders(user.id); // and here
  return { user, orders };
}
```

`async/await` is syntactic sugar — the runtime transforms it into a **state machine** that transitions between states at each `await` point. Each `await` is a "give up CPU, register callback for when IO finishes."

**Why `await` only works inside `async` functions:** The function must be transformable into a state machine. `async` marks it for that transformation.

---

### 5.3 Virtual Threads / Goroutines (Go's Model)

Go takes a middle path — neither pure event loop nor raw OS threads.

**Key concept:** Go creates **goroutines** (virtual/green threads) managed by the **Go runtime scheduler**, not the OS scheduler. Goroutines are lightweight (~2–4KB initial stack, grows dynamically vs ~8MB for OS threads) so creating thousands is practical.

**Architecture:**

```
OS Threads (M1, M2, M3, M4 — one per CPU core, set by GOMAXPROCS)
     ↑
Go Runtime Scheduler (maps goroutines onto OS threads)
     ↑
Goroutines (G1, G2, G3... G10000 — one per HTTP request)
```

- The OS scheduler manages M threads (expensive but few)
- The Go scheduler manages G goroutines (cheap, many) and maps them to M threads
- When a goroutine blocks on IO, Go scheduler pauses it and runs another goroutine on the same M thread
- Context switching between goroutines is cheap (pointer swap) vs OS thread context switch (CPU register save/restore)

**In Go's HTTP server:** `go serve(conn)` — a new goroutine is spawned per request. This is safe because goroutines are cheap enough to create by the thousands.

```go
func handler(w http.ResponseWriter, r *http.Request) {
  user, err := db.QueryRow("SELECT * FROM users WHERE id = $1", userID)
  // goroutine pauses here, Go scheduler runs another goroutine
  // when DB responds, this goroutine resumes automatically
  json.NewEncoder(w).Encode(user)
}
```

**Why Go handles both IO-bound and CPU-bound well:**
- IO-bound: goroutine pauses at IO, Go scheduler runs other goroutines efficiently
- CPU-bound: goroutines can run in true parallel across M OS threads (one per core)
- No explicit `async/await` syntax needed — the runtime handles it transparently

---

## 6. Comparison: When to Use What

| | Threading (OS threads) | Event Loop | Virtual Threads / Goroutines |
|--|----------------------|-----------|------------------------------|
| **IO-bound workloads** | Works, but expensive context switching at scale | Excellent — minimal overhead | Excellent — lightweight switching |
| **CPU-bound workloads** | Excellent — true parallelism | Poor — blocks event loop | Excellent — maps to OS threads |
| **Memory per "task"** | High (MB per thread) | Low (no extra stack) | Low (~KB per goroutine) |
| **Context switch cost** | High (μs–ms, OS-level) | None (single thread) | Low (Go runtime level) |
| **Scalability** | Limited (1k–10k threads) | Very high (100k+ concurrent) | Very high (million goroutines feasible) |
| **Examples** | Java (classic), C, Python (threading) | Node.js, Python (asyncio), Rust (tokio) | Go, Java (virtual threads in Java 21+) |

---

## 7. Shared State Problems (Race Conditions)

The most common source of concurrency bugs. When multiple tasks share the same memory and modify it concurrently, results become unpredictable.

### Race Condition Example (Threading)

Incrementing a counter from two threads:

```
Thread A             Counter = 0          Thread B
─────────────────────────────────────────────────
Read counter → 0                          Read counter → 0
Increment: 0+1=1                          Increment: 0+1=1
Write 1 → counter                        Write 1 → counter
                     Counter = 1 (expected: 2)
```

Thread B reads the old value before Thread A writes back. Both increment from 0, both write 1 → the update from one thread is **lost**. This is the **lost update** problem.

### Race Condition in Async/Await (Single Thread)

Even single-threaded event loops have race conditions because `await` yields control:

```javascript
let balance = 100;

async function withdraw(amount) {
  if (balance >= amount) {          // Check passes: 100 >= 100 ✓
    await processWithdrawal(amount); // ← yields control here
    // ↑ Another withdraw(100) runs, balance becomes 0
    balance -= amount;              // Now: 0 - 100 = -100 !!
  }
}
```

Between the `if` check and the `balance -= amount` line, another task ran and changed `balance`. The invariant was violated.

### Solutions

**Mutex / Lock:**
```python
lock = threading.Lock()

def increment():
    with lock:                    # Only one thread enters this block at a time
        counter += 1              # Safe write
                                  # Lock released when block exits
```

Mutual exclusion: only one thread/task can hold the lock at a time. Others wait. Prevents concurrent writes to shared data.

**Channels (Go):**
Instead of multiple goroutines sharing and writing to the same variable, only one designated goroutine owns the variable. Others communicate by **sending messages through channels** — message passing instead of shared memory.

```go
// Instead of: shared counter++ from multiple goroutines
// Use: counter goroutine receives increment requests via channel
counterCh <- IncrementRequest{}
```

> "Don't communicate by sharing memory; share memory by communicating." — Go proverb

---

## 8. Summary

| Concept | Definition | When it matters |
|---------|-----------|-----------------|
| **IO-bound** | Program mostly waits for external responses (network, DB, file system) | Almost all SaaS backends |
| **CPU-bound** | Program mostly computes (image processing, encryption, ML) | Media pipelines, ML servers, crypto |
| **Concurrency** | Structuring a program to handle multiple tasks at once (single core possible) | IO-bound: avoid wasting CPU during waits |
| **Parallelism** | Truly simultaneous execution on multiple cores | CPU-bound: maximize throughput of computation |
| **OS Thread** | Kernel-managed execution unit; heavy (MB stack, μs–ms context switch) | True parallelism for CPU-bound work |
| **Event Loop** | Single-thread loop checking IO completion + running callbacks | IO-bound: maximum concurrency with minimum overhead |
| **Goroutine** | Go's lightweight virtual thread; Go scheduler maps many to few OS threads | Both IO-bound and CPU-bound |
| **Race Condition** | Two tasks read/modify shared state concurrently → unpredictable results | Any shared mutable state |
| **Mutex/Lock** | Ensures only one task modifies shared state at a time | Protecting shared variables |

---

## Quick Revision Checklist

- [ ] Problem: server idle 95% of the time waiting for IO → wasting 300M CPU instructions per 100ms wait
- [ ] IO-bound: waiting for DB/network/files — CPU is idle; concurrency is the solution
- [ ] CPU-bound: heavy computation — CPU is busy; parallelism (more cores) is the solution
- [ ] Most SaaS backends are IO-bound, not CPU-bound
- [ ] Concurrency = dealing with multiple tasks at once (can work with 1 core, via interleaving)
- [ ] Parallelism = doing multiple tasks truly simultaneously (requires multiple CPU cores)
- [ ] OS threads: kernel-managed, preemptive scheduling, ~8MB stack each, 1–10μs context switch
- [ ] Thread overhead: memory (MB per thread), creation (system call, μs–ms), context switching
- [ ] IO block + threading: thread marked BLOCKED, OS switches to another thread, resumed when IO completes
- [ ] Threads share heap memory within a process — fast but enables race conditions
- [ ] Event loop: single thread, callback-based, never blocks; OS APIs (epoll/kqueue) notify IO completion
- [ ] Event loop excels at IO-bound, fails at CPU-bound (blocks whole loop)
- [ ] `async/await` is syntactic sugar over callbacks; transforms function into state machine
- [ ] Each `await` = yield control to event loop; resume when IO completes
- [ ] Can only use `await` inside `async` functions (needed for state machine transformation)
- [ ] Goroutines (Go): lightweight virtual threads (~2–4KB stack); Go scheduler maps many G → few M OS threads
- [ ] Go spawns new goroutine per HTTP request (safe because goroutines are cheap)
- [ ] Goroutine context switch = pointer swap (cheap); OS thread context switch = register save/restore (expensive)
- [ ] GOMAXPROCS = number of OS threads Go creates (default = number of CPU cores)
- [ ] Race condition: two tasks read and write shared variable concurrently → lost updates, invalid state
- [ ] Race conditions exist even in single-threaded async/await (yield at await allows interleaving)
- [ ] Mutex/lock: only one task enters critical section at a time
- [ ] Go channels: message-passing instead of shared memory — avoids race conditions at design level