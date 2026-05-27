# Production-Grade Configuration Management — Lecture Notes

---

## 1. What is Configuration Management?

**Definition:** The systematic approach to organizing, storing, accessing, and maintaining all the settings of a backend application.

> Think of it as the **DNA of your application** — it decides how your code behaves in different environments without changing the code itself.

**Common misconception:** Config management = just database passwords and API keys.

**Reality:** It covers a much broader scope:
- How the application starts up
- How it connects to external services
- What it logs, at what level, and where
- Which features are enabled or disabled
- For which users which features are active
- Performance tuning parameters
- Business rules and security settings

---

## 2. Types of Configuration

### 2.1 Application Settings

The most common type — controls core behavior of the running app.

| Setting | Example |
|---------|---------|
| `PORT` | Which port the server runs on (local: 8080, production: may differ) |
| `LOG_LEVEL` | `debug` in development, `info` in production |
| `REQUEST_TIMEOUT` | How long before a request is dropped (e.g., 60s → AI image gen taking 80s → 504) |
| `DB_POOL_SIZE` | Max open DB connections (10 locally, 50 in production) |

---

### 2.2 Database Config

All parameters needed to connect to the database:

```
host, port, username, password, database_name
→ combined into a connection URL
```

Plus: query timeout (how long before a DB query is aborted).

---

### 2.3 External Service Config

API keys and connection details for every third-party integration:
- Email providers (Resend, Mailchimp) → API key
- Payment processors (Stripe) → secret key
- Auth providers (Clerk, Auth0) → API key
- Cloud storage (S3) → access key + secret

---

### 2.4 Feature Flags

Dynamically enable or disable features **without deploying new code**.

**Use cases:**
- A/B testing a new checkout flow for a subset of users
- Rolling out a feature region-by-region (enable for US, disable for India)
- Disabling a broken feature instantly in production without a deployment

Feature flags add a config layer that sits above code — the same codebase can behave differently for different users or environments.

---

### 2.5 Other Config Types

| Type | Examples |
|------|---------|
| **Security config** | JWT secret, session secret, token expiry |
| **Infra / DevOps config** | Kubernetes settings, deployment parameters |
| **Performance tuning** | Max CPUs (Go `GOMAXPROCS`), thread pool sizes |
| **Business rules** | Max order quantity per user, discount limits |

---

## 3. Config Characteristics

Not all configs are equal. Classify them before storing:

| Characteristic | Examples |
|----------------|---------|
| **Sensitive / secret** | DB password, API keys, JWT secret → must be encrypted |
| **Non-sensitive** | Port number, log level, pool size → can be in plain files |
| **Environment-specific** | DB URL (localhost in dev, cloud in prod) |
| **Shared across environments** | Max string length rules, timeout values |
| **Frequently changing** | Feature flags |
| **Rarely changing** | Connection pool size, port |

---

## 4. Configuration Storage Options

### 4.1 Environment Variables

Most common. Loaded from `.env` file (locally) or injected by the deployment platform (production).

**Local:** `.env` file + a library like `dotenv` loads values into `process.env` (Node.js) or `os.Getenv` (Go).

**Production flow:**
```
Deployment triggers
      ↓
Fetch secrets from cloud provider (Vault, AWS Parameter Store, etc.)
      ↓
Inject as environment variables into the container / process
      ↓
App starts → reads from environment
```

---

### 4.2 Config Files (YAML / TOML / JSON)

Used heavily in open-source projects. YAML is preferred over JSON because it supports comments.

**Example (config.yaml):**
```yaml
server:
  port: 8080
  log_level: debug  # change to "info" in production

database:
  host: localhost
  pool_size: 10

features:
  new_checkout: false
```

TOML is a newer alternative — also supports comments, cleaner syntax for nested configs.

---

### 4.3 Dedicated Secrets Management Services

For production systems with distributed deployments:

| Service | Provider |
|---------|---------|
| **HashiCorp Vault** | Open source + cloud |
| **AWS Parameter Store / Secrets Manager** | AWS |
| **Azure Key Vault** | Azure |
| **Google Secret Manager** | GCP |

**What these provide:**
- Secrets encrypted at rest and in transit
- Access control (who can read which secret)
- Audit logs (who accessed what and when)
- Automatic secret rotation
- Integration with Kubernetes, CI/CD pipelines

---

### 4.4 Hybrid Strategy (Common in Production)

Load from multiple sources with defined priority:

```
Priority 1: Cloud secrets store (AWS Parameter Store / Vault)
Priority 2: config.yaml file
Priority 3: Environment variables
Priority 4: Application defaults
```

Higher priority sources override lower ones. Allows environment-specific overrides without touching code.

---

## 5. Environment-Specific Configuration

Each environment has different priorities — config should reflect that.

| Environment | Priority | Config characteristics |
|-------------|---------|----------------------|
| **Development** (localhost) | Developer productivity, debugging | `log_level: debug`, `pool_size: 10`, local DB URL |
| **Test / CI** | Automated validation, reproducibility | Isolated test DB, mock API keys |
| **Staging** | Mirror production behavior, minimize cost | Cloud DB (smaller), `pool_size: 2`, production-like URLs |
| **Production** | Reliability, security, performance | `log_level: info`, `pool_size: 50`, real secrets from vault |

**Key insight:** The same codebase runs in all environments. Only the config changes — no code changes needed to move between environments.

---

## 6. Security Best Practices

### Never Hardcode Secrets
```
❌ const dbPassword = "super_secret_password"  // committed to git
✓  const dbPassword = process.env.DB_PASSWORD  // loaded at runtime
```

Database URLs, API keys, JWT secrets — none of these should ever appear in source code.

### Use a Dedicated Secrets Management Service

When running at scale, a service like HashiCorp Vault or AWS Parameter Store handles:
- Encryption at rest
- Encryption in transit (TLS)
- No plaintext secrets in environment or code

### Principle of Least Privilege (Access Control)

| Role | Should have access to |
|------|----------------------|
| Frontend engineers | Frontend API URLs, frontend SDK keys |
| Backend engineers | DB credentials, Redis, Elasticsearch |
| DevOps / Infra team | Cloud instance configs, Kubernetes secrets |

**No one should have access to everything.** Segment access by role.

### Rotate Secrets Periodically

API keys, JWT secrets, database passwords — rotate them on a schedule. A leaked secret that was rotated last week causes far less damage than one that's been unchanged for 3 years.

---

## 7. The Most Important Practice: Validate Configs at Startup

> **If you take one thing from this topic: always validate your configuration before your server starts.**

**Why:**

| Scenario | With validation | Without validation |
|----------|----------------|-------------------|
| Missing `OPENAI_API_KEY` in production | App fails to start → previous deployment keeps running | App starts, fails at runtime when a user hits the AI feature → `500` error |
| Misconfigured DB URL | Clear error at startup: "invalid database URL" | Cryptic connection errors at request time |

**How:**

```
Before server.listen() / http.ListenAndServe():
  1. Load all configs from source
  2. Validate with a schema (Zod for TypeScript, go-validator for Go)
  3. Check required fields are present and correctly typed
  4. If any are missing → log a clear error → exit immediately
```

**Blue-green deployment alignment:** Most production deployments use blue-green — the old version only stops after the new version successfully starts. If your app fails to start due to config issues, the old version keeps running and users aren't impacted.

---

## 8. Configuration Chaos — What to Avoid

Without a systematic config management strategy:

| Problem | Impact |
|---------|--------|
| Hard-coded values scattered in codebase | Changing a value requires code change + deployment |
| Inconsistent behavior across environments | "Works on my machine" — hard to reproduce prod issues |
| Secrets in source code | Exposed in git history, logs, error messages |
| No validation | Silent failures when configs are missing or wrong |
| No access control | Any developer can see production DB passwords |

---

## Quick Revision Checklist

- [ ] Config management = all settings controlling how your app runs — not just secrets
- [ ] Config types: application settings, database, external services, feature flags, security, performance, business rules
- [ ] Classify configs: sensitive vs non-sensitive, environment-specific vs shared, frequently vs rarely changing
- [ ] Storage options: env vars (`.env` + library), config files (YAML preferred over JSON for comments), cloud secrets services
- [ ] Cloud secrets services: HashiCorp Vault, AWS Parameter Store, Azure Key Vault, Google Secret Manager
- [ ] Hybrid strategy: load from multiple sources with defined priority
- [ ] Environments: dev (debug/productivity), test (automation), staging (prod-mirror, lower cost), production (reliability/security)
- [ ] Same codebase, different configs → different behavior per environment, no code changes needed
- [ ] Never hardcode secrets — always read from environment at runtime
- [ ] Use cloud secrets management at scale — encryption at rest + in transit + access control
- [ ] Principle of least privilege — frontend, backend, DevOps each get only what they need
- [ ] Rotate secrets periodically (API keys, JWT secrets, DB passwords)
- [ ] **Validate all configs at startup** — fail fast before serving users; use Zod (TS), go-validator (Go), or equivalent
- [ ] Feature flags: enable/disable features without deployment; useful for A/B testing and regional rollouts
- [ ] `LOG_LEVEL=debug` in dev, `LOG_LEVEL=info` in production — adjust verbosity per environment
- [ ] DB pool size: small in local/staging (cost), large in production (traffic)