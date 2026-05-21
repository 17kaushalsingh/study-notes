# Lecture 9: Validations and Transformations

**Keywords:** Data Integrity, Security, Controller Layer, Type Validation, Syntactic Validation, Semantic Validation, Type Casting, Data Normalization, UX vs Security

## Architectural Context

In a standard backend architecture, requests flow through distinct layers:
1. **Controller Layer:** The entry point. Handles HTTP-specific logic, route matching, and returns status codes.
2. **Service Layer:** Executes business logic (orchestrating notifications, DB calls, webhooks, etc.).
3. **Repository Layer:** Deals with persistent storage (SQL, NoSQL, Redis).

**Where do Validations & Transformations fit?**
They occur at the **entry point of the Controller Layer**, immediately after a route is matched but *before* any significant business logic or repository methods are executed.

## Why Validate on the Backend?

1. **Preventing 500 Internal Server Errors:** Unvalidated data reaching the database can cause crashes (e.g., trying to insert a number into a `TEXT NOT NULL` PostgreSQL column).
2. **Predictable State:** Ensures the system doesn't enter an "unexpected state" under unusual circumstances.
3. **Better API Communication:** Allows the server to return clear **400 Bad Request** errors with descriptive messages (e.g., "Email is required") instead of generic failures.
4. **Self-Documentation:** Descriptive validation errors can act as a form of documentation for developers using the API.

## Types of Validation

### 1. Type Validation
Ensures the data matches the expected primitive or composite type.
- **Example:** Expecting a `string` for a name but receiving a `number` or `boolean`.
- **Deep Checks:** Enforcing that every element inside an `array` is a `string`.

### 2. Syntactic Validation
Checks if the *structure* of the data follows a specific pattern.
- **Email:** Must contain `@`, a domain name, and a TLD.
- **Phone Numbers:** Must follow international or local formats.
- **Dates:** Must follow specific ISO or custom formats (e.g., `YYYY-MM-DD`).

### 3. Semantic Validation
Checks if the data makes *logical sense* in the real world.
- **Future Dates:** A "Date of Birth" cannot be in the year 2099.
- **Range Constraints:** A human "Age" field should logically be between 0 and 120.

### 4. Complex (Dependent) Validation
Validations where one field depends on another.
- **Password Confirmation:** `password_confirmation` must exactly match `password`.
- **Conditional Fields:** If `married` is `true`, then the `partner_name` field becomes mandatory.

## Transformation (Type Casting & Normalization)

Transformation is the process of converting data into a desirable format before or after validation.

1. **Type Casting:** 
   - Query parameters (e.g., `?page=2&limit=20`) arrive at the server as **strings** by default.
   - The transformation pipeline casts these strings into **integers** so the service layer can perform math or limit DB queries correctly.
2. **Data Normalization:**
   - **Lowercasing:** Converting `UsEr@ExAmPlE.CoM` to `user@example.com` to ensure consistent lookups.
   - **Sanitization:** Stripping whitespace or adding prefixes (e.g., adding `+` to a phone number).
   - **Formatting:** Standardizing date objects.

## The Golden Rule: Frontend vs. Backend

| Validation Tier | Purpose | Mandatory? |
| :--- | :--- | :--- |
| **Frontend (Client)** | **User Experience (UX):** Provides immediate feedback and prevents unnecessary network calls. | No (Optional but recommended). |
| **Backend (Server)** | **Security & Integrity:** Protects the system from malicious data, bugs, and diverse clients (CLI, Postman, Mobile). | **YES (Mandatory).** |

> **Crucial Concept:** Never rely on frontend validation. A malicious user or a different client interface (like Insomnia) can easily bypass frontend checks and hit your API directly with "garbage" data. Backend validation is your last line of defense.