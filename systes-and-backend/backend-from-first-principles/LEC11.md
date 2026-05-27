# REST API Design — Lecture Notes

---

## 1. Brief History & Why REST Exists

**1990 — Tim Berners-Lee** invented the WWW along with: URI, HTTP, HTML, the first web server, and the first browser. These foundations are still in use today.

**Problem:** The web grew exponentially — far beyond what its architecture was designed for. Scalability became a crisis.

**1993 — Roy Fielding** (co-founder of the Apache HTTP server project) proposed a set of architectural constraints to solve this. He later collaborated with Berners-Lee to write the HTTP/1.1 specification. In **2000**, Fielding named and described the web's architectural style in his PhD dissertation: **REST — Representational State Transfer**.

> Worth reading: Roy Fielding's original dissertation is publicly available and provides deep context for all the patterns we use today.

---

## 2. The Six REST Constraints

| Constraint | Meaning |
|-----------|---------|
| **Client-Server** | Separation of UI (client) and data/logic (server) — each evolves independently |
| **Uniform Interface** | Standardized communication; includes resource identification, self-descriptive messages, HATEOAS |
| **Layered System** | Architecture is hierarchical; each layer only sees the layer directly below it — enables load balancers, proxies, etc. |
| **Cacheable** | Responses must be labeled cacheable or non-cacheable; reduces server load and improves UX |
| **Stateless** | Every request contains all information needed to process it; server stores no client context between requests |
| **Code on Demand** *(optional)* | Servers can extend client functionality by sending executable code (e.g., JavaScript); rarely used |

---

## 3. What "REST" Actually Means

**Representational** — Resources (data/objects) have representations. The same resource can be represented differently depending on the client: JSON for API clients, HTML for browsers, XML for legacy systems.

**State** — The current condition/attributes of a resource. Example: a shopping cart's state = its items, quantities, total price.

**Transfer** — Movement of these representations between client and server via HTTP methods (GET, POST, PUT, PATCH, DELETE).

---

## 4. URL Anatomy for APIs

```
https://api.example.com/v1/books?page=2&limit=10#section
  │       │                │    │    └── query params
  │       │                │    └── resource (path)
  │       │                └── version
  │       └── subdomain (API standard: use "api.")
  └── scheme (always https in production)
```

**Rules for the path segment:**
- Resources are always **plural nouns** in lowercase: `/books`, `/users`, `/organizations`
- Use **hyphens** for multi-word slugs, never spaces or underscores: `/harry-potter`
- Forward slash `/` denotes **hierarchical relationship** between resources
- No trailing slashes

---

## 5. Idempotency

**Definition:** Calling the same operation N times has the same effect as calling it once — no additional side effects with repeated calls.

| Method | Idempotent? | Reason |
|--------|------------|--------|
| `GET` | ✓ Yes | Only reads; no state change |
| `PUT` | ✓ Yes | Full replacement — same payload always produces same end state |
| `PATCH` | ✓ Yes | Same partial update → same result |
| `DELETE` | ✓ Yes | Resource deleted on first call; subsequent calls find nothing to delete (no new side effects) |
| `POST` | ✗ No | Each call creates a new resource — different side effect each time |

> **Delete nuance:** The second DELETE call may return `404`, but no new state change occurred — the environment is the same as after the first call. That's why it's still idempotent.

---

## 6. HTTP Methods in REST

| Method | CRUD | When to use |
|--------|------|------------|
| `GET` | Read | Fetch a resource or list of resources |
| `POST` | Create | Create a new resource, OR custom actions |
| `PATCH` | Update | Partial update — send only changed fields |
| `PUT` | Replace | Full replacement — send complete resource representation |
| `DELETE` | Delete | Remove a resource |

**PATCH vs PUT in practice:** PATCH is almost always what you want in modern JSON-based APIs. PUT is a holdover from the MPA era and requires sending the full entity. Use PATCH for partial updates unless you specifically need full replacement.

**POST as custom action:** When an operation doesn't fit any CRUD category, use POST. The REST spec intentionally leaves POST open-ended for this purpose.

---

## 7. API Design Workflow

Before writing any code:

```
1. Study wireframes / Figma designs
       ↓
2. Identify resources (nouns from requirements)
   e.g., organizations, projects, tasks, users, tags
       ↓
3. Design database schema
       ↓
4. Define actions per resource (CRUD + custom actions)
       ↓
5. Design API interface (in Postman/Insomnia/Swagger)
       ↓
6. Write code
```

> Design the interface before programming it. Use a tool like Insomnia, Postman, or Swagger to prototype the API shape, independent of any programming language or framework.

---

## 8. CRUD Endpoint Patterns

For each resource (example: `organizations`):

| Operation | Method | Route | Response |
|-----------|--------|-------|----------|
| List all | `GET` | `/organizations` | `200` + paginated array |
| Get one | `GET` | `/organizations/:id` | `200` + entity, or `404` |
| Create | `POST` | `/organizations` | `201` + created entity |
| Update (partial) | `PATCH` | `/organizations/:id` | `200` + updated entity |
| Delete | `DELETE` | `/organizations/:id` | `204` (no body) |
| Custom action | `POST` | `/organizations/:id/archive` | `200` or `201` depending on action |

**Key observations:**
- List and Create share the same URL (`/organizations`) — differentiated by method.
- Get, Update, and Delete share the same URL (`/organizations/:id`) — differentiated by method.
- Custom actions extend the pattern: `/resource/:id/action-name`.

---

## 9. List API — Pagination, Sorting, Filtering

Every list endpoint should support all three features. Clients should never be required to send these — the server sets sensible defaults.

### Pagination

Query params: `page`, `limit`

**Response shape:**
```json
{
  "data": [...],
  "total": 50,
  "page": 2,
  "totalPages": 5
}
```

| Field | Purpose |
|-------|---------|
| `data` | The current page's records |
| `total` | Total count across all pages (for UI display) |
| `page` | Which page this response is for |
| `totalPages` | Total pages available (client uses this to know when to stop fetching) |

**Server defaults:** `page = 1`, `limit = 10` (or 20).

### Sorting

Query params: `sortBy` (field name), `sortOrder` (`asc` or `desc`)

**Server defaults:** `sortBy = createdAt`, `sortOrder = desc` (latest first — the natural expectation).

Always sort by default. Without an explicit sort, database returns rows in undefined order — different results on each call.

### Filtering

Query params: field names as keys, desired values as values.

```
GET /organizations?status=active
GET /organizations?status=active&name=acme
```

**Important:** If filters match nothing → return `200` with `data: []`, NOT `404`. A list endpoint never returns `404` — you're not requesting a specific entity.

---

## 10. Response Codes by Operation

| Scenario | Code |
|---------|------|
| Successful fetch (GET) | `200 OK` |
| Successful create (POST) | `201 Created` |
| Successful update (PATCH/PUT) | `200 OK` |
| Successful delete (DELETE) | `204 No Content` (empty body) |
| Custom action — no new resource | `200 OK` |
| Custom action — creates resource | `201 Created` |
| Resource not found (specific ID) | `404 Not Found` |
| Empty list result | `200 OK` + `data: []` — **never** `404` |
| Validation error | `400 Bad Request` |
| Unauthorized | `401 Unauthorized` |
| Forbidden | `403 Forbidden` |

> Don't blindly assume POST = `201`. Custom actions using POST return `200` when no resource is created.

---

## 11. Custom Actions

When an operation can't be expressed as CRUD:
- Use `POST`
- Append the action name after the resource ID

```
POST /organizations/:id/archive
POST /projects/:id/clone
POST /users/:id/send-verification-email
```

**Why not just PATCH the status field?** Because the action often triggers cascading operations on the server (archive org → delete all projects → notify users → etc.) that can't be captured by a simple field update. The named action communicates intent clearly.

---

## 12. JSON Payload Conventions

- **camelCase** for all JSON field names — both in request payloads and responses.
- **Never abbreviate** field names (`description` not `desc`, `organizationId` not `orgId`).
- **Exclude** server-managed fields from request payloads: `id`, `createdAt`, `updatedAt`.
- **Include** only what the client needs to provide; the server handles the rest.

---

## 13. Best Practices Summary

**1. Interactive documentation** — Integrate Swagger/OpenAPI from the start. It serves as both a testing playground and living documentation for consumers.

**2. Consistency** — Once you establish a pattern (plural resources, camelCase payloads, pagination shape), apply it uniformly across all resources and endpoints. Inconsistency forces consumers to do guesswork.

**3. Sane defaults** — Server should never require clients to send "obvious" params. Defaults for page, limit, sort field, sort order, and status values like "active" on creation should all be handled server-side.

**4. No abbreviations** — Field names in payloads and query params should be fully spelled out and self-explanatory. You have more context when building than consumers will have when integrating.

**5. Design before coding** — Dedicate a separate session to API interface design using Insomnia/Postman/Swagger before writing any implementation code, regardless of language or framework.

---

## Quick Revision Checklist

- [ ] REST = Representational State Transfer; formalized by Roy Fielding in 2000
- [ ] 6 constraints: client-server, uniform interface, layered, cacheable, stateless, code on demand
- [ ] URL structure: `https://api.domain.com/v{n}/resources/:id?query=params`
- [ ] Resources in URL = always **plural lowercase nouns**; hyphens for spaces
- [ ] Idempotent: GET, PUT, PATCH, DELETE — Non-idempotent: POST
- [ ] PATCH = partial update (preferred); PUT = full replacement
- [ ] POST = create OR custom actions (open-ended by spec)
- [ ] CRUD routes: list/create share URL; get/update/delete share URL with `:id`
- [ ] Custom actions: `POST /resource/:id/action-name`
- [ ] List API must support pagination (`page`, `limit`), sorting (`sortBy`, `sortOrder`), filtering
- [ ] Empty list → `200 + data: []`; specific resource not found → `404`
- [ ] Delete → `204 No Content` (empty body)
- [ ] JSON payloads: camelCase, no abbreviations, exclude server-managed fields
- [ ] Design the API interface (in Insomnia/Postman) before writing any code