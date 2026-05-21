# Lecture 10: Controllers, Services, Repositories, Middlewares, and Request Context

**Keywords:** Request Life Cycle, Handler, Controller, Service Layer, Repository Pattern, Middleware, Request Context, Data Binding, Orchestration, RBAC

## The Internal Request Life Cycle

A request doesn't just hit a database. It travels through a structured "pipeline" inside the server to ensure maintainability, security, and scalability.

1.  **Entry Point:** The OS forwards the HTTP request to the specific Port (e.g., 3000).
2.  **Routing:** The routing algorithm matches the URL pattern to a specific **Handler**.
3.  **Middleware Chain:** Common operations (Auth, Logging) are executed before reaching the business logic.
4.  **Handler/Controller:** Data is extracted, validated, and passed to the professional layer.
5.  **Service Layer:** The "Brain" of the operation executes the core business logic.
6.  **Repository Layer:** The "Librarian" interacts with the database.
7.  **Response:** The result flows back up the chain to the client.

---

## Architectural Layers (The Restaurant Analogy)

To understand why we separate these responsibilities, imagine a professional kitchen:

### 1. Handler / Controller (The Waiter)
The Handler is the first point of contact for the request.
-   **Responsibilities:**
    *   **Data Binding:** Deserializing JSON/Query params into native language formats (Structs, Classes).
    *   **Validation:** Ensuring the "order" makes sense (syntactic and semantic checks).
    *   **Status Codes:** Deciding if the result is a `200 OK` or a `400 Bad Request`.
-   **Rule:** Keep it "thin". It should only handle HTTP concerns, not business logic.

### 2. Service Layer (The Chef)
The Service Layer is the "Brain" where the actual work happens.
-   **Responsibilities:**
    *   **Business Logic:** Calculating totals, checking inventory, orchestrating multiple actions.
    *   **Orchestration:** Calling multiple Repository methods, sending emails, or triggering webhooks.
-   **Rule:** The Service Layer should be **HTTP-agnostic**. It shouldn't know it's being called by an API; it just takes data and returns a result.

### 3. Repository Layer (The Pantry Manager)
The Repository Layer has a single responsibility: talking to the data source.
-   **Responsibilities:**
    *   **Query Construction:** Writing the SQL or NoSQL queries.
    *   **Data Access:** Fetching, inserting, or deleting specific records.
-   **Rule:** One method should do one thing (e.g., `GetBookByID`, not a generic `GetBooks` with 10 optional filters that changes the return type).

---

## Middlewares: The Security Checkpoints

Middlewares are optional functions executed in the "middle" of the request life cycle. They help reduce code duplication for common tasks.

### Core Properties
-   **Inputs:** They receive the `Request` object, `Response` object, and a `Next()` function.
-   **The `Next()` Function:** This is the trigger that passes execution to the next middleware or handler in the chain.
-   **Termination:** A middleware can stop a request early (e.g., if Auth fails) by sending a response and *not* calling `Next()`.

### Common Examples & Ordering
Ordering is critical. A typical chain looks like this:
1.  **CORS:** Check origin immediately to terminate unauthorized domains.
2.  **Logging:** Log the start of the request for debugging.
3.  **Authentication:** Verify the token and identify the user.
4.  **Rate Limiting:** Ensure the client isn't spamming the server.
5.  **[Routing & Handlers happen here]**
6.  **Global Error Handling:** Always placed at the **end** to catch any unhandled errors from the layers above and return a structured JSON error.

---

## Request Context: The Visitor Badge

The **Request Context** is a shared, key-value storage scoped to a single specific request.

### Why use it?
It allows different parts of the system (Middlewares and Handlers) to share state without being tightly coupled.

### Practical Use Cases:
-   **Identity:** The Auth Middleware finds the `User_ID` and saves it in the Context. The Downstream Handler retrieves it to know who is making the post.
-   **Tracing:** A unique `Request_ID` (UUID) is generated at the start, stored in Context, and used in every log message to trace the request's journey.
-   **Cancellation:** Sending "Abort" signals if a client disconnects, preventing the server from wasting resources on a dead connection.

> **Key Takeaway:** If the Handler is the "What", the Service is the "How", and the Repository is the "Where", the **Context** is the "Who" and "When" that stays with the request from entry to exit.