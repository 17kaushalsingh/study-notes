# Backend Security — Lecture Notes

> **Core mindset:** Attackers don't care about your framework or language. They ask one question: **"Where did the developer make an assumption?"** Every vulnerability in this chapter comes from a developer assuming user input would be clean, users are who they say they are, or requests come from their own frontend. Security is about being paranoid at every boundary.

---

## 1. The Attacker's Mental Model

Your backend speaks **multiple languages simultaneously**:

```
User (browser: HTML/JS/CSS)
        ↓
Backend (your code: Go/Python/Node)
        ↓                    ↓                    ↓
Database (SQL)        OS (shell)           HTML renderer
```

Every time user input crosses from one language context into another, a vulnerability can arise. The input that is *data* in one context may become *code* in another.

> **Root cause of all injection attacks:** Confusion between data and code. Treating data as code, or code as data.

**Three questions to ask at every boundary:**
1. Where is data crossing a boundary?
2. What assumptions am I making about this data?
3. What if those assumptions are wrong?

---

## 2. Injection Attacks

### 2.1 SQL Injection

**The vulnerable pattern — string concatenation:**
```sql
-- Template
SELECT * FROM users WHERE email = '<user_input>'

-- Legitimate input → alice@gmail.com
SELECT * FROM users WHERE email = 'alice@gmail.com'

-- Malicious input → ' OR '1'='1'--
SELECT * FROM users WHERE email = '' OR '1'='1'--'
```

**What happens:**
- The first `'` closes the opening quote, making email = empty string (false)
- `OR '1'='1'` is always true → entire WHERE becomes true
- `--` comments out everything after → the trailing `'` is ignored
- Result: `SELECT * FROM users` → **attacker gets all rows**

**More destructive variant:**
```sql
-- Malicious input → '; DROP TABLE users;--
SELECT * FROM users WHERE email = ''; DROP TABLE users;--'
```
The `DROP TABLE` executes as a second SQL statement, deleting the table.

**Further attack surface:**
- `UNION` statements to extract data from other tables (payment info, etc.)
- Database-specific functions to read server filesystem files
- In some configs, execute OS commands through the database

**Fix: Parameterized Queries (Prepared Statements)**

```
-- Two separate things sent to the database:
1. Query template:  SELECT * FROM users WHERE email = $1
2. User data:       "alice@gmail.com"  (or whatever was passed)

The database treats $1 slot content as PURE DATA, never as SQL syntax.
```

The malicious input `' OR '1'='1'--` becomes just a garbage string — it finds no matching email and causes no harm.

**Key points:**
- Every modern DB driver supports parameterized queries
- Every ORM uses them by default
- The only way to be vulnerable is to deliberately build raw SQL strings
- Validation layer should also catch non-email-shaped strings before they reach the DB

**NoSQL is not immune:** MongoDB query objects support operators (`$ne`, `$gt`, `$exists`). If user-controlled JSON is passed directly as a query object, operators can be injected. Always validate structure, not just values.

---

### 2.2 Command Injection

**The vulnerable pattern:** Constructing OS commands with string concatenation.

```bash
# Backend calls FFmpeg with user-provided filename
ffmpeg -height 120 -width 220 -o <user_input>

# Legitimate input → output.jpg
ffmpeg -height 120 -width 220 -o output.jpg

# Malicious input → output.jpg; rm -rf /
ffmpeg -height 120 -width 220 -o output.jpg; rm -rf /
```

When the shell encounters `;`, the `rm -rf /` becomes a new command and executes — deleting the entire root filesystem.

Attackers can also use `|` (pipe), `&` (background execution), and shell escape sequences for more creative exploits.

**Fix:** Use language-provided functions that accept command and arguments separately.

```
-- Bad: shell string (user input goes through shell interpreter)
exec("ffmpeg -o " + userInput)

-- Good: argument array (user input goes directly to process, bypasses shell)
exec(["ffmpeg", "-o", userInput])
```

With argument arrays, the OS passes the user string directly to the process — it's treated as data, never interpreted as shell syntax.

**Universal injection prevention rule:** Whenever building a string that will be interpreted by another system (SQL, OS, HTML, LDAP) **and** that string includes user input — stop, and find a parameterized alternative. It almost always exists.

---

## 3. Authentication Security

### 3.1 Use an Auth Provider (Production Recommendation)

Implementing production-grade auth yourself means handling:
- Stateful sessions (Redis, revocation, device tracking)
- OAuth flows (Google, GitHub) + account linking between email and social login
- Session-email linking (same email, different providers = same user)
- Token rotation, refresh strategies, timing attacks

Providers like Clerk, Auth0 handle all of this. When billing becomes painful (millions of users), you'll have revenue to support it. Start with a provider; migrate later if needed.

---

### 3.2 Password Storage

**Evolution of password storage:**

#### Method 1: Plain Text ❌
```
DB: { email: "alice@gmail.com", password: "123456" }
```
- Breach exposes all passwords directly
- Employees/DBAs can see all user passwords
- 70%+ of users reuse passwords → one breach = multiple account takeovers

#### Method 2: Hashing ⚠️ (not enough alone)

**Hashing function properties:**
- Takes any input of any length
- Always returns fixed-length output
- Same input → always same output
- One-way: mathematically impossible to reverse

```
hash("123456") → "$2b$12$xyz..." (bcrypt output)
```

On login: hash the provided password, compare with stored hash. Breach exposes hashes, not passwords.

**Problem — Rainbow Tables:** Attackers precompute a table of `common_password → hash` for all common hashing algorithms. If your hash matches a rainbow table entry, the password is cracked.

#### Method 3: Hashing + Salting ✅

**Salt:** A randomly generated string, unique per user, stored in the DB alongside the hash.

```
password = "123456"
salt = "sp3xR9kQ..." (cryptographically random, per-user)
hashed = hash(password + salt) → store in DB
```

**Why it defeats rainbow tables:** The rainbow table has `hash("123456")`. But your DB has `hash("123456" + "sp3xR9kQ...")`. These will never match. Each user's hash is unique even if their password is identical.

**Problem — Brute Force with GPUs:** Modern GPUs compute billions of SHA-256 hashes/second. With a breached DB (including salts), an attacker can brute-force offline: try every common password, hash it with the stolen salt, compare.

#### Method 4: Hashing + Salting + Slow Hash Functions ✅✅

**Do NOT use for passwords:** MD5, SHA-256, SHA-512 (general-purpose, very fast → billions/sec on GPU)

**Use for passwords (slow hash functions):**
- **bcrypt** (long-standing default)
- **Argon2id** (current industry standard)

These have a configurable **cost factor / work factor** that controls how slow they run:

| Scenario | Speed | Effect |
|----------|-------|--------|
| Genuine user login | 300–400ms per login | Imperceptible to user |
| GPU brute force | ~4–5 attempts/sec (vs billions without slow hash) | Would take decades/centuries instead of days |

---

### 3.3 Sessions (Stateful Authentication)

After successful login, the server:

**Step 1:** Generate a cryptographically secure random session ID (128–256 bits)
- Must use a CSPRNG (cryptographically secure pseudo-random number generator)
- 128 bits = more possible values than atoms in the universe → guessing is impossible

**Step 2:** Store session in DB/Redis with metadata:
- User ID
- Created timestamp
- Expiry time (e.g., 7 days)
- IP address (for "signed in from X location" feature)
- User agent (device/browser type)

**Step 3:** Send session ID to browser as a cookie. All subsequent requests automatically include it.

**Critical cookie flags:**

| Flag | Value | Effect |
|------|-------|--------|
| `HttpOnly` | `true` | JavaScript cannot read this cookie → XSS attacks can't steal it |
| `Secure` | `true` | Cookie only sent over HTTPS, never plain HTTP → prevents interception on public Wi-Fi |
| `SameSite` | `Strict` or `Lax` | Cookie not sent in cross-origin requests → prevents CSRF |

> Never store session ID or JWT in `localStorage` — XSS attacks can steal it. Use `HttpOnly` cookies.

---

### 3.4 JWT (Stateless Authentication)

**Structure:** Three base64-encoded parts separated by dots.

```
HEADER.PAYLOAD.SIGNATURE

Header:    { "alg": "HS256", "typ": "JWT" }
Payload:   { "sub": "user_id_123", "iat": 1710000000, "name": "Alice", "admin": false }
Signature: HMAC_SHA256(header + "." + payload, secret_key)
```

**How it works:**
- Server signs the payload with a secret key stored in env vars
- Client stores the JWT and sends it in `Authorization: Bearer <token>` header
- Server verifies signature on every request — any tampering invalidates the signature
- Payload is just base64 — **readable by anyone, but not modifiable without the secret**

**Important:** Never store sensitive data in JWT payload (it's not encrypted, just encoded).

**JWT limitations:**

| Problem | Impact |
|---------|--------|
| **Revocation is hard** | Can't immediately log out a compromised account from all devices |
| **Storage problem** | localStorage = vulnerable to XSS; cookies = need HttpOnly, ends up same as sessions anyway |

**Workarounds:**
- **Blacklist tokens:** Store revoked tokens in Redis; check on every request
- **Short access token + refresh token flow:**

```
Login → issue:
  access_token  (expires in 5–10 min)
  refresh_token (expires in 1–7 days)

Workflow:
  Request with access_token
    → 401 if expired
    → Send refresh_token to get new access_token + refresh_token
    → Cycle continues

If compromised: attacker has access for max 5–10 minutes,
then can't refresh without the refresh_token.
```

**Recommendation:** Unless you have specific horizontal scaling requirements, prefer stateful sessions over JWTs. The tradeoffs of stateless auth are rarely worth it for typical SaaS. If you use JWTs, use short expiry + refresh tokens + HttpOnly cookies.

---

### 3.5 Rate Limiting on Authentication Endpoints

Without rate limiting, attackers can brute-force credentials at thousands of attempts/second, or crash your server with volume.

**Layered rate limiting strategy:**

| Layer | Mechanism | Bypassed by |
|-------|-----------|-------------|
| **Per-IP** | 10 attempts/min per IP | VPNs, botnets, rotating IPs |
| **Per-account** | 5 failures per 15 min → lock account | Distributed password spray (1 attempt per account) |
| **Global** | 100 failed attempts/min system-wide → alert + CAPTCHA | Nothing — last resort |

Use all three layers. More restrictive limits for auth endpoints than general API endpoints.

---

## 4. Authorization Security

**Authentication** = Who is this user? (Identity)
**Authorization** = What is this user allowed to do? (Permissions)

### 4.1 The False Sense of Security

**The mistake:** Checking auth at the routing layer, then assuming the user has access to everything.

```
Routing layer: ✅ authenticated, ✅ has "read:books" permission
        ↓
Repository:   SELECT * FROM books WHERE id = 5   ← NO user check!
```

Book ID 5 may belong to a different user. The routing layer check doesn't prevent this.

**The fix:** Check authorization at the point of data access.

```sql
-- Wrong: fetches any book with ID 5
SELECT * FROM books WHERE id = 5

-- Right: fetches only if it belongs to the authenticated user
SELECT * FROM books WHERE id = 5 AND user_id = $currentUserId
```

Apply this to ALL operations: SELECT, UPDATE, DELETE, INSERT.

---

### 4.2 BOLA — Broken Object Level Authorization

**What:** User A can access User B's resources by guessing/enumerating resource IDs.

**Example:** Attacker iterates IDs in `/invoices/101`, `/invoices/102`, `/invoices/103`... downloads all invoices.

**Additional subtlety — 403 vs 404:**

```
❌ Pattern: fetch invoice → if not owner → return 403 Forbidden

Why bad: 403 confirms the invoice exists. Attacker can enumerate
which IDs exist, then plan social engineering attacks.

✅ Pattern: SELECT * FROM invoices WHERE id = 7 AND user_id = $currentUser

If invoice doesn't belong to user → zero rows → return 404 Not Found

Attacker cannot distinguish "doesn't exist" from "exists but not yours."
```

**Sequential IDs enable enumeration.** Use UUIDs as primary keys — unpredictable, impossible to iterate.

---

### 4.3 BFLA — Broken Function Level Authorization

**What:** Regular user accesses admin-only functions.

**The "security through obscurity" anti-pattern:**
```
"Only admins know the URL /admin/invoices — we don't share it"
```
Anyone monitoring network traffic can find this URL. No role check = anyone can call it.

**Fix:** Role-based middleware at the routing layer.

```
/admin/invoices route:
  1. requireAuth middleware        → is user logged in?
  2. requireRole("admin") middleware → is user an admin?
  3. Only then → handler runs
```

---

### 4.4 Authorization Framework

| Practice | Description |
|----------|-------------|
| **Centralize authorization** | All auth logic in one place — consistent, maintainable, not scattered across handlers |
| **Default deny** | If not explicitly allowed → deny. New endpoints protected by default, even if you forget |
| **Test authorization** | Automated tests: user A can't access user B's resources; member can't access admin functions; unauthenticated can't access protected resources |
| **Audit logs** | Log every access to sensitive functions (admin endpoints) and every authorization failure |

**Two categories of authorization attacks:**

| Category | What happens | Example vulnerability |
|----------|-------------|----------------------|
| **Horizontal** | User A → User B's data (same privilege level, different scope) | BOLA (Broken Object Level Auth) |
| **Vertical** | Regular user → Admin functions (escalating privilege) | BFLA (Broken Function Level Auth) |

---

## 5. XSS — Cross-Site Scripting

**What:** Attacker gets their JavaScript to execute in a genuine user's browser, in the context of your platform.

**Why it's dangerous — attacker's JavaScript can:**
- Read all page content including sensitive data
- Make API requests impersonating the logged-in user
- Steal session cookies (if not HttpOnly) or localStorage
- Redirect user to phishing pages
- Alter page content to trick users

### 5.1 Stored XSS

**Attack flow:**
1. Attacker submits a comment/post containing `<script>maliciousCode()</script>`
2. Server stores the HTML without sanitizing
3. Next time any user views that comment → script executes in their browser

**Prevention:** Sanitize user-provided markup before storing. Strip script tags, event handlers, and other executable HTML from user input on the server side.

### 5.2 Root Cause (Same as Injection)

User-defined content (data) is treated as code in the HTML/JS context. Same confusion between data and code as SQL injection, but happening in the browser.

### 5.3 Prevention

**Primary:** Sanitize all user-provided content server-side before storing or rendering. Never trust user input to be safe HTML.

**Secondary — Content Security Policy (CSP):**
- HTTP response header that tells browsers what to execute
- `Content-Security-Policy: script-src 'self'` → only run scripts from your domain
- `Content-Security-Policy: script-src 'none'` → block all inline scripts
- CSP is a **last line of defense**, not a prevention — fix the root cause first

---

## 6. CSRF — Cross-Site Request Forgery

**What:** Attacker tricks a user's browser into making a request to your site with their cookies attached.

**Example:**
1. User is logged into bank.com (browser has bank.com cookie)
2. User visits malicious evil.com
3. evil.com triggers a hidden form submission to bank.com
4. Browser automatically includes bank.com cookie → server thinks it's a legitimate request

**Why it's less relevant in modern apps:**
- `SameSite=Strict` or `SameSite=Lax` on cookies (modern browsers default to Lax) → cookie not sent in cross-origin requests → CSRF blocked
- CORS config blocks cross-origin requests without proper headers

**Verdict:** Not a major threat if using modern frameworks and proper cookie config. Don't obsess over it; ensure `SameSite` is not `None`.

---

## 7. Misconfiguration Vulnerabilities

### 7.1 Secrets in Source Code

```
❌ const apiKey = "sk-abc123..."  // committed to git

✓ const apiKey = process.env.OPENAI_API_KEY
```

If a secret is committed to git: **rotate it immediately**. Deleting the commit doesn't help — it remains in git history.

Store secrets in: environment variables, AWS Parameter Store, HashiCorp Vault, Azure Key Vault.

### 7.2 Debug Mode in Production

`LOG_LEVEL=debug` in production leaks:
- Stack traces with function/file names and code structure
- Explicit SQL queries and database configs
- Sensitive user data printed during debug

**Set `LOG_LEVEL=info` in production.** Debug logs contain internal implementation details that attackers can use to plan targeted attacks.

### 7.3 Missing Security Headers

Most web frameworks provide a security middleware (one-line setup) that configures all standard headers:

| Header | Protection |
|--------|-----------|
| `Content-Security-Policy` | Controls what scripts/resources browser will execute |
| `X-Frame-Options` | Prevents your site from being embedded in iframes (blocks clickjacking) |
| `X-Content-Type-Options` | Prevents MIME type sniffing |
| `Strict-Transport-Security` | Forces HTTPS |

Use your framework's security middleware — don't configure these manually.

---

## 8. Defense in Depth — Layered Security

No single defense is perfect. Layer them so an attacker must bypass all layers simultaneously:

```
Layer 1: Input validation
         → Validate everything at entry point; data leaving validation should
           be exactly the structure expected. No surprises downstream.

Layer 2: Parameterized operations
         → DB queries: parameterized queries/ORMs
         → OS commands: argument arrays, not shell strings

Layer 3: Authorization at point of access
         → Don't rely on routing-layer auth alone
         → Check user ownership in every DB query

Layer 4: Security headers and policies
         → CSP, SameSite cookies, X-Frame-Options
         → Limit blast radius if something gets through

Layer 5: Monitoring and logging
         → Log suspicious activity, failed auth attempts, admin access
         → Alert on anomalies; make attacks visible
```

---

## 9. Further Reading

| Resource | What it covers |
|----------|---------------|
| **PortSwigger Web Security Academy** | Free, comprehensive, hands-on labs for all major vulnerabilities (SQLi, XSS, CSRF, SSRF, auth attacks, JWT attacks, etc.) |
| **OWASP Top 10** | Current list of most critical web vulnerabilities with real-world instances and severity |
| **OWASP Cheat Sheet Series** | Best practices for specific topics: authentication, session management, input validation, etc. |
| **Lucia Auth docs** | Guidance for implementing secure authentication with industry best practices |

---

## Quick Revision Checklist

- [ ] Security mindset: think like an attacker — "where did the developer assume?"
- [ ] Root cause of all injection: data treated as code when crossing language boundaries
- [ ] SQL injection: string concatenation + user input = vulnerability → fix with parameterized queries
- [ ] Command injection: shell string + user input = vulnerability → fix with argument arrays
- [ ] NoSQL is not immune: MongoDB operators can be injected if user controls query structure
- [ ] Password storage: plain text ❌ → hashing ⚠️ → hashing + salt ✅ → slow hash (bcrypt/Argon2id) + salt ✅✅
- [ ] Slow hashes (bcrypt, Argon2id): cost factor makes brute force decades-long instead of days
- [ ] Session IDs: 128–256 bit, CSPRNG-generated, stored server-side (Redis/DB), sent as HttpOnly cookie
- [ ] Cookie flags: `HttpOnly=true`, `Secure=true`, `SameSite=Strict` or `Lax`
- [ ] JWT: payload is base64-encoded (readable!), not encrypted — don't store sensitive data
- [ ] JWT revocation problem: use short access token (5–10 min) + longer refresh token (1–7 days)
- [ ] Prefer stateful sessions over JWTs unless you have specific horizontal scaling needs
- [ ] Rate limiting: per-IP + per-account + global (all three layers for auth endpoints)
- [ ] BOLA: always include `AND user_id = $currentUser` in DB queries — don't just check at routing layer
- [ ] Return 404 (not 403) when user requests another user's resource — prevents existence confirmation
- [ ] BFLA: admin functions need role middleware, not just permission middleware — security through obscurity fails
- [ ] Default deny: new endpoints protected by default until explicitly granted
- [ ] UUID primary keys prevent enumeration attacks that sequential IDs enable
- [ ] XSS root cause: user HTML/JS content treated as code in browser context
- [ ] XSS fix: sanitize user markup server-side; CSP as last line of defense (not prevention)
- [ ] CSRF: largely mitigated by `SameSite` cookies (Lax/Strict) in modern browsers
- [ ] Never commit secrets to git; rotate immediately if accidentally committed
- [ ] `LOG_LEVEL=info` in production — debug logs expose code structure and sensitive data
- [ ] Use framework security middleware for headers (CSP, X-Frame-Options, HSTS) — one line
- [ ] Defense in depth: validation → parameterized ops → auth at access point → headers → monitoring
