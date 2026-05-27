# Graceful Shutdown — Lecture Notes

---

## 1. What is Graceful Shutdown?

**The problem:** A server restart is triggered mid-deployment. At that exact moment, your backend may be processing active requests — including payment transactions, database writes, or file uploads. What happens to those?

**The answer:** Graceful shutdown — the practice of stopping a backend application in an orderly way rather than abruptly killing it.

> **Analogy:** When a restaurant closes, staff don't turn off the lights and push customers out. They stop seating new customers, let existing ones finish their meals, then clean up and close. Same principle applies to backend servers.

**Why it matters:**
- Prevents incomplete transactions (double charges, lost orders)
- Avoids data corruption from interrupted DB writes
- Prevents deadlocks from uncommitted transactions
- Ensures a good user experience during deployments

---

## 2. Process Lifecycle

Every application runs as a **process** inside an operating system. Like living things, processes have a lifecycle:

```
Born     → process starts
Lives    → process executes
Dies     → process terminates
```

When the OS needs to stop a process, it doesn't just "pull the plug." It follows an established communication protocol using **signals**.

---

## 3. Unix Signals

Signals are the OS mechanism for inter-process communication (IPC). Your application **registers signal handlers** — code that waits and listens for these signals and responds appropriately.

### 3.1 SIGTERM — The Polite Request

```
Signal: SIGTERM (Signal + Terminate)
Origin: Deployment systems, process managers (Kubernetes, PM2, systemd)
Meaning: "Please finish up and shut down cleanly"
```

- Sent programmatically by deployment tools, orchestration platforms
- Application **can** catch and handle it
- Gives the application time to complete in-flight work
- The standard way production deployments stop services

**Response:** finish existing requests → clean up resources → exit

---

### 3.2 SIGINT — The Developer Interrupt

```
Signal: SIGINT (Signal + Interrupt)
Origin: Developer pressing Ctrl+C in the terminal
Meaning: "User-initiated shutdown"
```

- Requires a human key press — primarily used in development
- Application **can** catch and handle it
- Should be handled **identically** to SIGTERM

> Whether it's a developer pressing Ctrl+C or Kubernetes sending SIGTERM, the intention is the same: shut down cleanly. Handle both signals with the same graceful shutdown logic.

---

### 3.3 SIGKILL — The Nuclear Option

```
Signal: SIGKILL (Signal + Kill)
Origin: OS, or manual kill -9
Meaning: "Stop immediately, no exceptions"
```

- Application **cannot** catch it
- Application **cannot** ignore it
- Process is terminated instantly — no cleanup, no finishing in-flight work
- Equivalent to pulling the power plug

> If you don't respond to SIGTERM within the timeout window, the OS escalates to SIGKILL. This is why handling SIGTERM gracefully is critical — it's your last chance to clean up.

---

## 4. Graceful Shutdown in Two Steps

### Step 1: Connection Draining

Stop accepting new work, finish existing work.

```
SIGTERM received
       ↓
Stop accepting new connections/requests
       ↓
Allow in-flight requests to complete
       ↓
Proceed to resource cleanup
```

**The restaurant analogy:**
1. Close the front door — stop seating new customers
2. Announce last call — let existing customers finish
3. Once everyone leaves — clean up and close

**By application type:**

| Application | Connection Draining Behavior |
|-------------|----------------------------|
| HTTP server | Stop accepting new HTTP requests; let active requests complete |
| Database | Finish active queries/transactions; stop new ones |
| WebSocket server | Notify connected clients of closure; close sockets |
| Background job worker | Finish current task; stop picking up new tasks |

### The Timeout Problem

You can't wait forever. Most systems set a **graceful shutdown timeout** (commonly 30 seconds):

```
SIGTERM
  ↓
Start draining connections
  ↓
Wait up to 30 seconds for in-flight work to finish
  ↓
If still running after timeout → force stop (SIGKILL behavior)
```

**Choosing the right timeout:**
- Too short → legitimate requests get interrupted
- Too long → deployments are slow, system is sluggish

Base it on your typical request duration. For standard CRUD APIs, 30 seconds is usually more than enough. For streaming, WebSocket, or long-running jobs, consider longer windows.

---

### Step 2: Resource Cleanup

Release everything the application acquired during its lifetime.

**Resources to clean up:**

| Resource | Why it matters |
|---------|---------------|
| **Database connections** | Must commit or rollback open transactions; close TCP connections in pool |
| **Network connections** | OS limits open connections per process; unreleased connections → memory/resource leak |
| **File handles** | OS limits open files per process; unreleased handles → memory leak |
| **Redis/cache connections** | Background job processing; must stop workers cleanly |
| **Temporary files** | Must be deleted; orphaned temp files accumulate over deployments |

**Critical rule:** Clean up resources in the **reverse order** of acquisition.

```
Startup order:
  1. Connect to Redis
  2. Connect to database
  3. Start HTTP server

Shutdown order:
  1. Stop HTTP server (stop accepting new requests)
  2. Close database connections
  3. Close Redis connections
```

**Why reverse order?** If a component depends on another (e.g., the HTTP server sends jobs to Redis), you must stop the consumer before the dependency. Reverse order prevents "cleanup a resource that something else still depends on."

---

## 5. The Full Graceful Shutdown Flow

```
SIGTERM or SIGINT received
          ↓
Register signal handler triggers shutdown function
          ↓
┌─────────────────────────────────────────────┐
│  Connection Draining                        │
│  • Stop HTTP server (no new connections)    │
│  • Wait for in-flight requests to finish    │
│  • Timeout: max 30 seconds                  │
└─────────────────────┬───────────────────────┘
                      ↓
┌─────────────────────────────────────────────┐
│  Resource Cleanup (reverse acquisition order│
│  • Close database connections               │
│    (commit/rollback open transactions)      │
│  • Stop background job workers              │
│  • Close Redis/cache connections            │
│  • Release file handles                     │
│  • Clean up temporary files                 │
└─────────────────────┬───────────────────────┘
                      ↓
              Log: "Server exited cleanly"
                      ↓
                   Process exits
```

---

## 6. Implementation (Language-Agnostic Pattern)

Most HTTP frameworks provide a `server.Shutdown()` or equivalent method that handles connection draining internally. The general pattern:

```
1. Register SIGTERM and SIGINT handlers
2. When signal received, call handler:
   a. Call server.Shutdown(ctx) — stops new requests, waits for existing
   b. Close database connections (after HTTP server finishes)
   c. Close Redis/cache connections
   d. Close any other acquired resources
   e. Log success message
3. Process exits cleanly
```

**Libraries that handle this:**
- Node.js: Express `server.close()`, Fastify `fastify.close()`
- Go: `http.Server.Shutdown(ctx)`
- Python: Signal module + `server.shutdown()`
- Most frameworks have this built in — copy the boilerplate from docs

---

## 7. Zero Downtime Deployments

Graceful shutdown is one piece of a zero-downtime deployment strategy.

**Blue-green deployment flow:**
```
1. New server (new code) starts up and passes health checks
2. Load balancer shifts traffic to new server
3. Old server receives SIGTERM → begins graceful shutdown
4. Old server drains connections → cleans up → exits
```

Graceful shutdown ensures step 3–4 don't corrupt in-flight requests on the old server during the transition.

---

## Quick Revision Checklist

- [ ] Graceful shutdown = stop accepting new work, finish existing work, clean up resources, then exit
- [ ] Every application runs as an OS process; shutdown is managed via Unix signals
- [ ] SIGTERM = polite request to stop (sent by Kubernetes, PM2, systemd) — **can be caught and handled**
- [ ] SIGINT = Ctrl+C from developer — **can be caught and handled**; treat identically to SIGTERM
- [ ] SIGKILL = instant termination — **cannot be caught or ignored**; no cleanup possible
- [ ] If SIGTERM is not handled within timeout → OS escalates to SIGKILL
- [ ] Connection draining: stop new connections first, let in-flight requests finish
- [ ] Graceful shutdown timeout: typically 30 seconds; tune based on your request duration
- [ ] Resource cleanup: database connections, network connections, file handles, Redis, temp files
- [ ] Clean up in **reverse acquisition order** — prevents cleaning a dependency before its consumers
- [ ] Database: commit or rollback open transactions before closing; never leave transactions dangling
- [ ] Most frameworks provide a `shutdown()` method — boilerplate is copy-paste; understanding *why* matters more than memorizing *how*
- [ ] Graceful shutdown + zero-downtime deployment = no requests lost during production deploys