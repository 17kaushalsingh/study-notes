# Lecture 11: Complete REST API Design

**Keywords:** REST, API Design, Roy Fielding, Idempotency, HTTP Methods, URL Structure, Uniform Interface, Stateless

## The History & Purpose of REST

In the 1990s, the World Wide Web faced a massive scalability crisis due to exponential user growth. To solve this, **Roy Fielding** (co-founder of Apache) proposed an architectural style in his 2000 PhD dissertation designed to make the web scalable. He called it **REST (Representational State Transfer)**.

### The 6 Constraints of REST
For an architecture to be considered truly "RESTful", it must follow these constraints:
1.  **Client-Server:** Strict separation of concerns (UI vs. Data/Logic) so they can evolve independently.
2.  **Uniform Interface:** A standardized way for components to communicate (e.g., standard HTTP methods and standard payloads).
3.  **Layered System:** Components cannot "see" beyond their immediate layer. This allows for intermediate layers like Load Balancers or Proxies without breaking the system.
4.  **Cacheable:** Server responses must explicitly label themselves as cacheable or non-cacheable to improve network efficiency.
5.  **Stateless:** Every single request from the client must contain *all* the information the server needs to understand it. The server cannot rely on memory of previous requests. (This is crucial for horizontal scaling).
6.  **Code on Demand (Optional):** Servers can temporarily extend client functionality by sending executable code (like JavaScript).

## Decoding "Representational State Transfer"

-   **Representational:** Resources (data) on the server can be *represented* in various formats (JSON, XML, HTML) depending on who is asking for them.
-   **State:** The current condition or attributes of a resource at a given moment (e.g., a Shopping Cart's current items and total price).
-   **Transfer:** Moving these representations between the client and server using standard HTTP methods.

## Standard URL Structure

A well-designed REST API URL follows a clear, hierarchical pattern:

`https://api.example.com/v1/resources/:id/custom-action`

### URL Best Practices
1.  **Always use Plural Nouns:** Use `/books`, not `/book`. Even when fetching a single item (`/books/123`), you are fetching a specific ID from the *collection of books*.
2.  **Use Hyphens for Slugs:** Do not use spaces or underscores. If a book is named "Harry Potter", the slug should be `harry-potter` (`/books/harry-potter`).
3.  **Hierarchy:** Use forward slashes (`/`) to indicate relationships.

## HTTP Methods & Idempotency

**Idempotency** is a critical theoretical concept with practical implications. 
> An operation is **idempotent** if performing it *once* has the exact same side-effect on the server as performing it *1,000 times*.

| Method | Purpose | Idempotent? | Why? |
| :--- | :--- | :---: | :--- |
| **GET** | Retrieve data. | ✅ Yes | Fetching data doesn't change the server state. |
| **PUT** | Completely replace a resource. | ✅ Yes | Replacing `{name: "B"}` with `{name: "B"}` 100 times results in the same final state. |
| **PATCH** | Partially update a resource. | ✅ Yes | Updating `{status: "active"}` to `{status: "active"}` repeatedly yields the same state. |
| **DELETE** | Remove a resource. | ✅ Yes | The first call deletes it. Subsequent calls return a `404 Not Found`, but the *server state* (the item being gone) doesn't change again. |
| **POST** | Create a new resource. | ❌ **No** | Calling `POST /books` 100 times creates 100 *different* books with 100 different IDs. |

### The "Open-Ended" POST for Custom Actions
What if you need an API to "Send an Email" or "Clone a Project"? These aren't standard CRUD (Create, Read, Update, Delete) operations. 
-   When an action doesn't fit a standard method, use **POST**. 
-   Append the custom action to the end of the resource URL: `POST /projects/:id/clone` or `POST /organizations/:id/archive`.

## API Design Best Practices

1.  **Consistency is King:** If you use `description` in one payload, don't use `desc` in another. Don't force clients to guess your naming conventions.
2.  **Provide Sane Defaults:** 
    - If a client requests a list without query parameters, default to `?page=1&limit=20`. 
    - If a client creates an organization, default its status to `active` instead of requiring them to pass it.
3.  **Avoid Abbreviations:** Write clear, complete words (e.g., `description` instead of `desc`). The developer implementing your API shouldn't need a decoder ring.
4.  **Interactive Documentation:** Provide a Swagger/OpenAPI playground so clients can test endpoints directly.