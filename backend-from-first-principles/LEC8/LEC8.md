# Lecture 8: Authentication and Authorization

**Keywords:** Authentication, Authorization, Sessions, JWT, Cookies, OAuth 2.0, OpenID Connect, RBAC, Security

## Core Definitions

- **Authentication:** The mechanism to assign an identity to a subject. It answers the question: **"Who are you?"**
- **Authorization:** The process of determining the capabilities and permissions of an identity. It answers the question: **"What can you do?"**

## The Evolution of Authentication

The journey to modern authentication is driven by the need for scalable trust and security.

1. **Pre-Industrial (Implicit Trust):** Based on human contextual trust. A village elder vouches for someone. Unscalable.
2. **Medieval Period (Cryptographic Era):** The introduction of **Wax Seals**. This functioned as a physical token (possession). It led to the first recorded bypass attacks (forgery).
3. **Industrial Revolution (Shared Secrets):** Telegraph operators used pre-agreed passphrases (static strings). This marked the shift to "something you know".
4. **Digital Phase (1961):** The Compatible Time-Sharing System (CTSS) at MIT introduced passwords for multi-user computers. Passwords were stored in plaintext, leading to a catastrophic vulnerability when the file was printed. This necessitated the invention of **hashing** for secure storage.
5. **The 1970s (Public Key Infrastructure):** The Diffie-Hellman key exchange introduced asymmetric cryptography (public/private keys). Kerberos later introduced ticket-based authentication.
6. **1990s (Multi-Factor Authentication - MFA):** To defend against brute-force and dictionary attacks, systems required a combination of:
   - **Something you know:** Passwords / PINs
   - **Something you have:** Smart Cards / OTPs
   - **Something you are:** Biometrics (Fingerprints, Retina scans)

## Core Components of Modern Authentication

### 1. Sessions (Stateful)

HTTP is inherently stateless—it treats every request as an isolated interaction. To support dynamic apps (e.g., e-commerce carts), the web needed a form of memory.

**Workflow:**
1. User logs in.
2. Server generates a unique **Session ID** and stores it alongside user data in a persistent store (initially file-based, later databases, now fast in-memory distributed stores like Redis).
3. Server sends the Session ID back to the client via a Cookie.
4. Client attaches the Cookie to subsequent requests. The server uses the ID to look up the session state.

### 2. JSON Web Tokens (JWTs) (Stateless)

As web apps grew into distributed microservices, maintaining and synchronizing session data across servers became costly and introduced latency.

**Workflow & Structure:**
- A stateless mechanism for transferring claims.
- **Self-contained:** Contains Base64 encoded metadata:
  - **Header:** Signing algorithm.
  - **Payload:** User data/claims (e.g., `sub` for user ID, `iat` for issued at, roles).
  - **Signature:** A cryptographic seal to verify authenticity and prevent tampering.

> **The Digital ID Card Analogy:** Think of a JWT as a digital ID card sealed in a tamper-proof envelope. The server verifies the seal (Signature) using its secret key without needing to check the central registry (Database/Redis) on every single request.

### 3. Cookies

A browser mechanism that allows servers to store a piece of information locally. The key feature is that the browser automatically attaches the cookie to subsequent requests to the originating server, streamlining authentication token delivery.

## Major Types of Authentication

### Stateful vs. Stateless Authentication

| Feature | Stateful Authentication (Sessions) | Stateless Authentication (JWTs) |
| :--- | :--- | :--- |
| **Storage Requirement** | Requires persistent store (Redis/DB). | None. Data is self-contained in the token. |
| **Scalability** | Harder. Requires synchronization across servers. | Highly scalable across distributed microservices. |
| **Revocation** | Easy. Server can instantly delete the session. | Difficult. Token remains valid until it expires manually. |

*Note: Modern systems often use a hybrid approach (e.g., using JWTs for stateless speed but maintaining a blacklist in Redis for revoked tokens).*

### API Key-Based Authentication

- **Purpose:** Designed for machine-to-machine (M2M) or programmatic interactions.
- **Workflow:** Instead of human UI flows (username/password), a developer generates a cryptographically random string (API Key) and the server securely attaches it to programmatic requests to access another platform's resources.

### OAuth 2.0 & OpenID Connect (OIDC)

- **The Delegation Problem:** How does a travel app scan your flight tickets on Gmail without you giving it your Google password? Giving out passwords means total, unrevokable access.
- **OAuth 2.0 (Authorization):** Uses token exchange. You authenticate with the Resource Server (Google), which issues an Access Token to the Client (Travel App) with strictly scoped permissions (e.g., read-only for emails).
- **OpenID Connect (Authentication):** Built on top of OAuth 2.0. It introduces an `ID Token` (a JWT) so clients can authenticate the user ("Sign in with Google") and obtain their basic identity profile.

## Authorization deep dive

### Role-Based Access Control (RBAC)

- Users are assigned roles (e.g., Admin, Moderator, User).
- **Workflow:** When an authenticated request arrives, the server deduces the role from the token/session. This role is passed down the middleware chain to verify against the permissions required for the target resource. If insufficient, the server returns a `403 Forbidden`.

## Security Best Practices

1. **Generic Error Messages:** During login, never return helpful errors like "User not found" or "Incorrect password". Attackers use this to map out valid accounts. Always return a generic **"Authentication failed"**.
2. **Preventing Timing Attacks:** If an invalid username fails instantly, but an invalid password takes 200ms (due to hashing overhead), attackers can measure the delay to confirm valid usernames. **Defense:** Use constant-time comparison functions or introduce simulated response delays.