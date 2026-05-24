# Task Queues & Background Jobs — Lecture Notes

---

## 1. What is a Background Task?

**Definition:** Any piece of code or logic that runs **outside of the request-response lifecycle**.

```
Client ──── request ────► Server ──── response ────► Client
                         ↕
                   Background Task
                (separate process, no response needed)
```

**Key properties:**
- Not mission-critical to the immediate response
- Not synchronous — doesn't block the API call
- Can be deferred, retried, and executed independently

---

## 2. Why Background Tasks? (The Problem They Solve)

**Example: User sign-up with email verification**

**Synchronous approach (bad):**
```
Client → POST /signup → Server validates → calls Email API → waits → returns 200
```

Problems:
- If the email service is down → the signup API itself fails or lies to the user
- API response is held until the email provider responds — poor latency
- A slow or unresponsive external service directly degrades your API's responsiveness

**Async approach with background task (good):**
```
Client → POST /signup → Server validates → pushes task to queue → returns 200 immediately
                                                      ↓
                                            Worker picks up task
                                                      ↓
                                            Worker calls Email API
                                                      ↓
                                     (retry with backoff if it fails)
```

Benefits:
- API responds instantly — user sees "verification email sent" immediately
- Email service downtime doesn't affect signup success
- Worker retries automatically until the email is sent successfully

---

## 3. Common Use Cases for Background Tasks

| Task | Why offload? |
|------|-------------|
| **Sending emails** | Depends on external email providers (Resend, Mailgun, Brevo) that may be slow or down |
| **Processing images/videos** | CPU-intensive — resizing, encoding to multiple resolutions |
| **Generating reports** | Heavy DB queries + PDF generation; scheduled (daily/weekly/monthly) |
| **Push notifications** | External service call to Apple/Google push notification services |
| **Account deletion** | Large-scale DB operations across multiple tables/shards; can't fit in one request |
| **Cleanup/maintenance** | Deleting orphan sessions, expired tokens, stale data periodically |

---

## 4. How Task Queues Work

### Core Components

```
Producer (your app) ──► Queue / Broker ──► Consumer / Worker (separate process)
    │                        │                       │
  Creates task          Stores tasks          Picks up & executes tasks
  Serializes to JSON    until consumed        Deserializes → runs handler
  Pushes to queue       Manages ordering      Sends acknowledgement
```

**Producer:**
- Application code (Node.js/Python/Go/Rust — any language)
- Creates the task with all data the worker needs
- Serializes to JSON (or another format)
- **ENQueues** (pushes) the task into the broker

**Broker (Queue):**
- Temporary holding area for tasks
- Technologies: **RabbitMQ**, **Redis PubSub**, **Amazon SQS**, **BullMQ's Redis backend**
- Stores tasks until a worker is ready
- Manages ordering, retries, visibility timeouts

**Consumer / Worker:**
- Runs in a **separate process** from the main backend
- Constantly monitors the queue for new tasks
- **DEQueues** (picks up) a task when available
- Deserializes the JSON → native format → runs the registered handler
- Sends **acknowledgement** back to the queue on success or failure

### Libraries by Language

| Language | Popular Libraries |
|----------|-----------------|
| Python | Celery |
| Node.js | BullMQ |
| Go | asynq |

---

## 5. Reliability Mechanisms

### Acknowledgements

After a worker processes a task, it sends an **acknowledgement** (ack) back to the queue:
- **Success ack** → queue removes the task permanently
- **Failure ack** → queue schedules a retry
- **No ack (timeout)** → queue makes task available to another worker

### Visibility Timeout

The period during which a task is considered "in progress" by a worker. If the worker crashes or hangs and doesn't ack within the timeout, the queue re-releases the task for another worker to pick up.

```
Worker picks task → visibility timeout starts
                        ↓
Worker acks within timeout → task removed ✓
Worker doesn't ack in time → task requeued for another worker ✓
```

> Prevents tasks from being lost when a worker crashes mid-execution.

### Retry with Exponential Backoff

When a task fails, it's retried with increasing delays:

```
Attempt 1 fails → retry after 1 min
Attempt 2 fails → retry after 2 min
Attempt 3 fails → retry after 4 min
Attempt 4 fails → retry after 8 min
...up to max retries (e.g., 5)
```

Most external service downtime is seconds to milliseconds — the task usually succeeds within 1–2 retries.

---

## 6. Types of Background Tasks

### 6.1 One-Off Tasks

Triggered by a specific event; executed once.

**Examples:**
- Send verification email after signup
- Send welcome email after successful verification
- Send password reset email
- Send in-app or push notification when a user receives a message

---

### 6.2 Recurring Tasks (Cron Jobs)

Executed repeatedly at fixed intervals. Most task frameworks support scheduled/periodic tasks.

**Examples:**
- Send daily/weekly/monthly reports to users
- Clean up expired/orphan sessions from the database
- Delete soft-deleted records after a grace period
- Refresh external API data (exchange rates, weather)

---

### 6.3 Chain Tasks (Parent-Child Relationship)

Task B can only start after Task A completes successfully. Multiple independent tasks can run in parallel if they share the same parent.

**Example: Video upload pipeline on an LMS platform**

```
Video uploaded
       ↓
Task 1: Encode video to multiple resolutions
       ↓
  ┌────────────────────────┐
  │                        │
Task 2a: Generate          Task 2b: Generate audio
thumbnails                 transcription (subtitles)
  │
  ↓
Task 3: Process thumbnails to multiple resolutions
```

- Task 2a and 2b can run in parallel (neither depends on the other)
- Task 3 depends on Task 2a completing first
- Task 1 must complete before 2a or 2b can start

---

### 6.4 Batch Tasks

A single trigger spawns many tasks, or a single task processes a large volume of data.

**Example 1: Account deletion**
```
DELETE /account API call → returns 200 immediately
                      ↓
Worker: delete all user projects
        delete all user assets (images, files)
        delete user profile
        delete user record
        send confirmation email
```

Entire deletion happens in background; user is logged out and sees "account deleted" without waiting.

**Example 2: Scheduled report delivery**
```
Midnight cron job → triggers 10,000 report generation tasks
                    (one per user, all at once)
```

---

## 7. Design Considerations at Scale

| Consideration | Detail |
|--------------|--------|
| **Idempotency** | Design tasks so they can be safely retried from scratch without side effects. If a delete task fails midway, rolling back via DB transaction ensures the next retry starts clean |
| **Error handling** | Robust error handling is critical — everything runs in a separate process, so unhandled exceptions can cause silent failures |
| **Monitoring** | Track queue length, task success/failure rates, error types. Tools: Prometheus + Grafana |
| **Scalability** | Design consumers to scale horizontally — add more worker nodes as user base grows |
| **Ordering** | If task execution order matters, ensure your queue/library supports ordered delivery |
| **Rate limiting** | If tasks call external APIs, implement rate limiting on the worker side to avoid hitting provider rate limits |

---

## 8. Best Practices

**Keep tasks small and focused**
One task = one processing unit. Don't bundle unrelated operations. If one step fails, only that step retries — not the entire pipeline.

**Avoid long-running tasks**
If a task takes too long, it's a signal to break it into smaller chained or parallel tasks. Long tasks are harder to retry, harder to monitor, and hold up worker resources.

**Robust error handling and logging**
Log every failure with enough context to debug (task ID, user ID, error message, stack trace). Good logging = faster debugging when tasks fail in production.

**Monitor queue length and worker health**
Set up alerting: if queue length spikes unexpectedly, workers may have crashed or external services may be down. React before users notice.

---

## 9. Summary

| Concept | Definition |
|---------|-----------|
| **Background Task** | Code running outside the request-response lifecycle |
| **Producer** | Application code that creates and enqueues tasks |
| **Broker / Queue** | Temporary storage for tasks (RabbitMQ, Redis, SQS) |
| **Consumer / Worker** | Separate process that dequeues and executes tasks |
| **ENQueue** | Pushing a task into the queue |
| **DEQueue** | Pulling a task out of the queue |
| **Acknowledgement** | Signal from worker to queue confirming task success/failure |
| **Visibility Timeout** | Time window in which a task is "reserved" by a worker; re-released if no ack |
| **Exponential Backoff** | Retry delay that doubles after each failure |
| **Idempotency** | Task can safely be retried without causing unintended side effects |

---

## Quick Revision Checklist

- [ ] Background task = logic outside the request-response lifecycle; not blocking, not synchronous
- [ ] Main reasons to offload: external service dependency, heavy computation, long-running operations
- [ ] Components: Producer (creates task) → Broker/Queue (stores it) → Consumer/Worker (executes it)
- [ ] Worker runs in a separate process; monitors queue constantly
- [ ] Acknowledgement mechanism: success ack removes task; no ack within visibility timeout → task requeued
- [ ] Retry with exponential backoff: doubles wait time after each failure; configurable max retries
- [ ] One-off tasks: single trigger → single execution (emails, notifications)
- [ ] Recurring tasks: cron-like intervals (reports, cleanup jobs)
- [ ] Chain tasks: parent-child dependency (video encode → thumbnail generation → transcription)
- [ ] Batch tasks: one trigger → many tasks, or large-volume processing (account deletion, bulk reports)
- [ ] Design for idempotency: tasks must be safely retryable from scratch
- [ ] Monitor: queue length, success/failure rates, worker health (Prometheus + Grafana)
- [ ] Scale by adding more consumer instances horizontally
- [ ] Keep tasks small and focused; avoid long-running monolithic tasks
- [ ] Libraries: Celery (Python), BullMQ (Node.js), asynq (Go); brokers: RabbitMQ, Redis, Amazon SQS