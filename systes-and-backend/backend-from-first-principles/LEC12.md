# Mastering Databases with PostgreSQL — Lecture Notes

---

## 1. Why Databases?

**Persistence** = storing data so it survives after the program that created it has stopped, and remains accessible across sessions and locations.

Without persistence: every app restart loses all data (a to-do app with no memory of your tasks).

**Database (broad definition):** Any structured persistent system that provides CRUD operations — Create, Read, Update, Delete. This includes your phone's contacts, browser localStorage, or even a text file.

**Database (backend context):** Disk-based storage systems. Why disk?

| Storage | Speed | Capacity | Cost |
|---------|-------|----------|------|
| RAM (primary memory) | Very fast | 8–128 GB typical | Expensive |
| Disk / SSD (secondary) | Slower | 512 GB – 2 TB+ typical | Cheap |

Databases prioritize capacity over speed → disk-based. Caches (Redis, Memcached) prioritize speed → RAM-based.

---

## 2. Why Not Just Text Files?

Three fundamental problems with plain text as a database:

| Problem | Detail |
|---------|--------|
| **Parsing overhead** | Every read requires scanning and splitting the file in application code — slow and error-prone |
| **No schema enforcement** | Can't prevent wrong data types; any string can go anywhere |
| **No concurrency control** | Two simultaneous writes to the same value → unpredictable result (last-write-wins race condition) |

These limitations motivated the creation of **DBMS** (Database Management System) software.

---

## 3. DBMS Responsibilities

A DBMS software (e.g., PostgreSQL, MySQL, MongoDB) handles:
- **Data organization** — efficient storage and retrieval algorithms
- **Access** — CRUD operations via a query language
- **Integrity** — enforces data accuracy and consistency at the database level
- **Security** — role-based access control, user permissions

---

## 4. Relational vs Non-Relational

| | Relational (SQL) | Non-Relational (NoSQL) |
|--|-----------------|----------------------|
| **Structure** | Tables, rows, columns | Collections, documents |
| **Schema** | Predefined, strict | Flexible, schema-less |
| **Data Integrity** | Enforced at DB level | Must be enforced in application code |
| **Query language** | SQL | Varies (MongoDB query language, etc.) |
| **Examples** | PostgreSQL, MySQL, SQLite | MongoDB, DynamoDB |
| **Best for** | CRM, e-commerce, financial data — anything requiring consistency | CMS, prototyping, unstructured content |

**Non-relational flexibility can be a disadvantage:** without schema enforcement, data inconsistencies are easier to introduce and must be caught in application code, which is more error-prone.

---

## 5. Why PostgreSQL?

1. **Open source and free** — no licensing cost; can self-host
2. **SQL-standard compliant** — migrating to MySQL or another SQL DB requires minimal changes
3. **Highly extensible** — ~1,400 pages of documentation; extension system
4. **Reliable and scalable** — production-proven at scale
5. **Native JSON support** — `jsonb` type eliminates most reasons to choose MongoDB; PostgreSQL can handle dynamic/unstructured data without switching databases

> **Rule of thumb:** Use PostgreSQL as your default for virtually every project. Only reconsider when you're optimizing for very specific bottlenecks at massive scale.

---

## 6. PostgreSQL Data Types

### Numeric Types

| Type | Notes |
|------|-------|
| `SMALLINT`, `INTEGER`, `BIGINT` | Integers; use `BIGINT` for IDs in production |
| `SERIAL`, `BIGSERIAL` | Auto-incrementing integers; `BIGSERIAL` for primary keys |
| `DECIMAL(p, s)` / `NUMERIC(p, s)` | Exact precision — use for **prices**, financial data |
| `REAL`, `DOUBLE PRECISION`, `FLOAT` | Approximate — faster; use for scientific measurements |

> **Decimal vs Float:** Use `DECIMAL` when accuracy matters (price, currency). Use `FLOAT` when small inaccuracies are acceptable and speed matters (area calculations, measurements).

### String Types

| Type | Behavior | When to use |
|------|----------|------------|
| `CHAR(n)` | Fixed length; pads with spaces | Only if values are always exactly the same length (e.g., day codes: `MON`, `TUE`) |
| `VARCHAR(n)` | Variable up to n characters | Legacy; mostly a MySQL convention |
| `TEXT` | Unlimited length | **Always prefer this in PostgreSQL** |

> **PostgreSQL recommendation:** Use `TEXT` — no performance difference from `VARCHAR`, avoids arbitrary length limits that require migrations later, and makes schema easier to read.

### Other Important Types

| Type | Notes |
|------|-------|
| `BOOLEAN` | `true` / `false` |
| `DATE` | Date only |
| `TIME` | Time only (HH:MM:SS) |
| `TIMESTAMP` | Date + time |
| `TIMESTAMPTZ` | Date + time + timezone — use this for `created_at`, `updated_at` |
| `INTERVAL` | Duration (e.g., `10 days`, `1 week`) |
| `UUID` | Universally unique identifier — preferred for primary keys |
| `JSON` | Stored as plain text |
| `JSONB` | Stored as binary — **prefer this**; faster queries and indexing |
| `ARRAY` | Array of any data type (e.g., `INTEGER[]`, `TEXT[]`) |

> **JSON vs JSONB:** Always use `JSONB` — PostgreSQL serializes it to binary for faster processing and better query capabilities. The only reason to use `JSON` is if you need to preserve original formatting (rare).

### Naming Conventions

- All table names and field names: **lowercase snake_case** (`full_name`, `created_at`)
- Table names: **plural** (`users`, `projects`, `tasks`)
- PostgreSQL is case-insensitive by default — camelCase forces you to use double quotes everywhere, making code ugly and fragile

---

## 7. Database Migrations

### What are Migrations?

Versioned SQL files that track all changes made to a database schema over time.

```
db/
  migrations/
    001_create_users.sql
    002_add_projects.sql
    003_add_indexes.sql
```

A migration tool (e.g., **dbmate**, go-migrate) reads these files sequentially and applies them to the database.

### Up vs Down Migrations

| Direction | Purpose |
|-----------|---------|
| **Up** | Apply the change (CREATE TABLE, CREATE INDEX, etc.) |
| **Down** | Revert the change (DROP TABLE, DROP INDEX, etc.) |

Down migrations enable **rollbacks** — if something breaks in production, you can revert to the previous schema version.

### Why Not Just Write Queries Manually?

- No version history of schema changes
- Hard to replicate state across dev/staging/production environments
- No rollback capability
- No audit trail of who changed what

The migration tool maintains a `schema_migrations` table in your DB to track the current version.

### Seeding

Inserting test data into the database for development/testing. Typically a separate migration file labeled `seed_data`.

---

## 8. Schema Design — Project Management Platform

### Enum Types (Custom Types)

Define a fixed set of allowed values at the database level:

```sql
CREATE TYPE project_status AS ENUM ('active', 'completed', 'archived');
CREATE TYPE task_status AS ENUM ('pending', 'in_progress', 'completed', 'cancelled');
CREATE TYPE member_role AS ENUM ('owner', 'admin', 'member');
```

**Why enums over TEXT?**
1. **Data integrity** — DB rejects any value not in the enum; no application-level check needed
2. **Documentation** — future developers can see all allowed values directly in the schema without reading application code

---

## 9. Table Design Patterns

### Standard Fields for Every Table

```sql
id          UUID PRIMARY KEY DEFAULT gen_random_uuid(),
created_at  TIMESTAMPTZ NOT NULL DEFAULT NOW(),
updated_at  TIMESTAMPTZ NOT NULL DEFAULT NOW()
```

**`NOT NULL` rule:** Add `NOT NULL` to every field unless you have an explicit reason for that field to be nullable. Default behavior in PostgreSQL allows nulls — don't rely on it.

### Constraints

| Constraint | Behavior |
|-----------|---------|
| `PRIMARY KEY` | Implicitly: NOT NULL + UNIQUE. Used to uniquely identify a row |
| `NOT NULL` | Field cannot be null |
| `UNIQUE` | No two rows can have the same value for this field |
| `CHECK (condition)` | Custom condition must be true (e.g., `CHECK (priority BETWEEN 1 AND 5)`) |
| `REFERENCES table` | Foreign key — value must exist as a primary key in the referenced table |

---

## 10. Relationships

### One-to-One

Separate table for related data (e.g., `users` and `user_profiles`). The second table uses the primary key of the first table as its own primary key.

```sql
CREATE TABLE user_profiles (
  user_id UUID PRIMARY KEY REFERENCES users ON DELETE CASCADE,
  bio     TEXT,
  ...
);
```

**Why separate tables?** User profiles grow over time (social links, preferences, etc.) without touching the core `users` table. Isolates concerns and reduces migration risk.

### One-to-Many

A project has many tasks. The "many" side stores a foreign key pointing to the "one" side.

```sql
CREATE TABLE tasks (
  project_id UUID NOT NULL REFERENCES projects ON DELETE CASCADE,
  ...
);
```

### Many-to-Many

Users can belong to many projects; projects can have many users. Requires a **linking table** with a composite primary key.

```sql
CREATE TABLE project_members (
  project_id UUID REFERENCES projects ON DELETE CASCADE,
  user_id    UUID REFERENCES users   ON DELETE CASCADE,
  role       member_role NOT NULL DEFAULT 'member',
  PRIMARY KEY (project_id, user_id)  -- composite primary key
);
```

The composite primary key ensures the same user can't be added to the same project twice.

---

## 11. Referential Integrity Constraints

What happens to related rows when a parent row is deleted:

| Constraint | Behavior |
|-----------|---------|
| `ON DELETE RESTRICT` | Prevents deletion if related rows exist — must delete children first |
| `ON DELETE CASCADE` | Automatically deletes all related rows |
| `ON DELETE SET NULL` | Sets the foreign key to NULL (field must be nullable) |
| `ON DELETE SET DEFAULT` | Sets the foreign key to its default value |

**Example logic:**
- Deleting a **user** who owns projects → `RESTRICT` (can't delete until projects are removed)
- Deleting a **project** → `CASCADE` on tasks (tasks belong to the project, delete them too)
- Deleting a **user** assigned to a task → `SET NULL` on `assigned_to` (task survives, just unassigned)

---

## 12. SQL Query Patterns for APIs

### Get All Users (with Profile)

```sql
SELECT u.*, to_jsonb(up.*) AS profile
FROM users u
LEFT JOIN user_profiles up ON u.id = up.user_id
ORDER BY u.created_at DESC;
```

- **LEFT JOIN** — include users even if they have no profile entry
- **`to_jsonb()`** — converts a row into a JSON object; embed profile as a nested field
- **Always sort** — DB returns rows in undefined order without explicit ORDER BY

### Get Single User (Parameterized Query)

```sql
SELECT u.*, to_jsonb(up.*) AS profile
FROM users u
LEFT JOIN user_profiles up ON u.id = up.user_id
WHERE u.id = $userId;
```

### Dynamic Filters + Sorting + Pagination

```sql
SELECT u.*, to_jsonb(up.*) AS profile
FROM users u
LEFT JOIN user_profiles up ON u.id = up.user_id
WHERE u.full_name ILIKE $letter || '%'   -- filter by first letter
ORDER BY $sortBy $sortOrder              -- dynamic sort
LIMIT $limit OFFSET $offset;            -- pagination
```

- `ILIKE` = case-insensitive LIKE pattern matching
- `%` wildcard = match anything after the given letter
- `OFFSET = (page - 1) * limit` — DB pages start at 0, UI pages start at 1

### Insert (Create User)

```sql
INSERT INTO users (email, full_name, password_hash)
VALUES ($email, $name, $passwordHash)
RETURNING *;
```

`RETURNING *` returns the newly created row — use this instead of a second SELECT query.

### Update (PATCH)

```sql
UPDATE user_profiles
SET bio = $bio, phone = $phone
WHERE user_id = $userId
RETURNING *;
```

Only include fields in SET that the user actually passed — construct this dynamically in application code.

---

## 13. Parameterized Queries

**What:** Placeholders in SQL queries (`$1`, `$userId`, etc.) filled at runtime.

**Why:** Prevents **SQL injection** — a vulnerability where user input is treated as SQL code. With parameterized queries, all input is treated as a string value, never as executable SQL.

```
Dangerous (string concatenation):
  "SELECT * FROM users WHERE id = " + userId
  → userId = "1; DROP TABLE users;" ← SQL injection!

Safe (parameterized):
  "SELECT * FROM users WHERE id = $userId"
  → userId is always treated as a string, never parsed as SQL
```

> Always use parameterized queries when incorporating any user-supplied value into a SQL query.

---

## 14. Indexes

### The Problem

Without an index, finding a row by a field requires a **sequential scan** — checking every row one by one across disk locations. At millions of rows, this is very slow.

### What an Index Does

An index is a **lookup table** maintained by the database:

```
Index on tasks.id:
  task_id_1  →  disk location A
  task_id_2  →  disk location B
  task_id_3  →  disk location C
```

When you query by `id`, the DB checks the index (fast, sequential lookup) → finds the disk location → jumps directly to the data.

**Analogy:** A book's index tells you which page each chapter starts on — you jump directly instead of scanning page by page.

### When to Create an Index

Create an index when a field is used in:
1. **JOIN conditions** (e.g., `ON u.id = up.user_id`)
2. **WHERE clauses** (e.g., `WHERE status = 'pending'`)
3. **ORDER BY / sorting** (e.g., `ORDER BY created_at DESC`)

> **Primary keys are automatically indexed.** Only index foreign keys and other frequently queried fields manually.

### Index Direction

Specify ascending or descending to match your most common query:

```sql
CREATE INDEX idx_users_created_at ON users(created_at DESC);
```

If your API always returns results in descending order, a descending index avoids an extra sort step.

### Performance Tradeoff

Every index adds overhead to INSERT and UPDATE operations (the index must be maintained). Evaluate:
- How frequently is the query called?
- Does the performance gain outweigh the write overhead?
- Monitor and drop indexes that aren't earning their cost.

### Common Indexes for the Example Schema

| Index | Reason |
|-------|--------|
| `users(email)` | Frequent lookups by email (login, joins) |
| `users(created_at DESC)` | Default sort order for list API |
| `tasks(project_id)` | JOIN condition when fetching tasks of a project |
| `tasks(assigned_to)` | JOIN condition when fetching tasks of a user |
| `tasks(status)` | WHERE clause filter |
| `tasks(created_at DESC)` | Default sort order |
| `project_members(user_id)` | JOIN condition |

---

## 15. Triggers

Automate actions at the database level in response to events (INSERT, UPDATE, DELETE).

**Use case:** Automatically update `updated_at` whenever any row in any table is modified.

```sql
-- 1. Create the function
CREATE FUNCTION update_updated_at()
RETURNS TRIGGER AS $$
BEGIN
  NEW.updated_at = NOW();
  RETURN NEW;
END;
$$ LANGUAGE plpgsql;

-- 2. Attach trigger to each table
CREATE TRIGGER set_updated_at
BEFORE UPDATE ON users
FOR EACH ROW EXECUTE FUNCTION update_updated_at();
```

Without a trigger: you'd have to manually add `updated_at = NOW()` to every UPDATE query in your application code — easy to forget, error-prone.

---

## Quick Revision Checklist

- [ ] Persistence = data survives program restarts; disk-based DBs trade speed for capacity
- [ ] DBMS = software managing data organization, CRUD access, integrity, security
- [ ] Text files fail at: parsing efficiency, schema enforcement, concurrency
- [ ] Relational = strict schema, tables/rows, SQL, data integrity at DB level
- [ ] Non-relational = flexible schema, collections/documents, integrity in app code
- [ ] PostgreSQL preferred: open source, SQL-compliant, extensible, native JSONB support
- [ ] Data types: prefer `BIGSERIAL`/`UUID` for IDs, `DECIMAL` for prices, `TEXT` over `VARCHAR`, `TIMESTAMPTZ` for timestamps, `JSONB` over `JSON`
- [ ] Naming: lowercase snake_case for all field and table names; plural table names
- [ ] Migrations: versioned SQL files; up = apply; down = revert; seeding = test data
- [ ] Enums: enforce allowed values at DB level; serve as documentation
- [ ] Constraints: `PRIMARY KEY` (NOT NULL + UNIQUE), `NOT NULL`, `UNIQUE`, `CHECK`, foreign key `REFERENCES`
- [ ] Relationships: one-to-one (shared PK), one-to-many (FK on "many" side), many-to-many (linking table + composite PK)
- [ ] Referential integrity: `RESTRICT` (block), `CASCADE` (delete children), `SET NULL`, `SET DEFAULT`
- [ ] Always use `RETURNING *` after INSERT/UPDATE instead of a second SELECT
- [ ] Parameterized queries prevent SQL injection — always use them for user-supplied values
- [ ] Indexes = lookup tables; speed up JOIN, WHERE, ORDER BY; add overhead to writes
- [ ] Primary keys auto-indexed; manually index foreign keys and frequent query fields
- [ ] Triggers = automate DB-level actions on events (e.g., auto-update `updated_at`)