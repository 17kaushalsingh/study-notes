# Chapter 5: Advanced SQL

## 1. Accessing SQL from a Programming Language

Database systems provide APIs to allow programs to interact with the database.

### Core APIs
- **JDBC (Java Database Connectivity)**: The standard API for Java.
  - Uses `DriverManager` to connect.
  - `Statement` and `PreparedStatement` objects to execute queries.
  - `ResultSet` to handle results.
- **Python DB-API**: Uses drivers like `psycopg2` for PostgreSQL.
- **ODBC (Open Database Connectivity)**: A C-language standard for database access.
- **Embedded SQL**: SQL statements are identified at compile time by a preprocessor and replaced with function calls.

### Security: SQL Injection
SQL Injection occurs when malicious users input SQL code into form fields to manipulate queries.
- **Prevention**: Use **Prepared Statements** (parameterized queries) which treat user input as data, not executable code.

---

## 2. Functions and Procedures

Allow "business logic" to be stored in the database.

### SQL Functions
```sql
create function dept_count(dept_name varchar(20))
returns integer
begin
    declare d_count integer;
    select count(*) into d_count
    from instructor
    where instructor.dept_name = dept_name;
    return d_count;
end
```

### SQL Procedures
Procedures use `in` and `out` parameters and are invoked using the `call` statement.

### Procedural Constructs
- **Compound Statements**: `begin ... end`.
- **Conditional**: `if-then-else`.
- **Loops**: `while`, `repeat`, `for`.

---

## 3. Triggers

A trigger is a statement that the system executes automatically as a side effect of a modification to the database.

### Requirements
1. **Event**: When the trigger is checked (e.g., `after update of salary on instructor`).
2. **Condition**: A predicate that must be satisfied for the trigger to fire.
3. **Action**: The SQL statements to be executed.

---

## 4. Recursive Queries

Useful for finding **transitive closures** (e.g., finding all prerequisite courses, direct and indirect).

```sql
with recursive rec_prereq(course_id, prereq_id) as (
    select course_id, prereq_id from prereq
    union
    select rec_prereq.course_id, prereq.prereq_id
    from rec_prereq, prereq
    where rec_prereq.prereq_id = prereq.course_id
)
select * from rec_prereq;
```

---

## 5. Advanced Aggregation Features

### Ranking
- `rank()`: Assigns ranks (e.g., 1, 2, 2, 4).
- `dense_rank()`: No gaps in ranks (e.g., 1, 2, 2, 3).
- `row_number()`: Unique numbers for each row.
- `ntile(n)`: Divides tuples into $n$ buckets.

### Windowing
Allows calculations over a "window" of tuples relative to the current tuple.
`avg(salary) over (partition by dept_name order by salary rows 3 preceding)`

### OLAP Operations
- **Pivot**: Rotates a table-valued expression by turning unique values from one column into multiple columns.
- **Rollup**: Generates subtotal rows (aggregates from right to left).
- **Cube**: Generates all possible combinations of subtotals.

---

## Must-Know Things
- **SQL Injection Prevention**: Always use `PreparedStatement` instead of string concatenation.
- **Trigger Usage**: Triggers are powerful but can lead to complex debugging; use constraints where possible.
- **Recursion**: The `with recursive` clause is essential for graph-like data in relational tables.
- **Ranking vs Row Number**: Understand that `rank()` handles ties by skipping values, while `row_number()` gives a distinct count.

---

## Quick Revision
- **Connectivity**: JDBC, ODBC, Python DB-API.
- **Logic**: Functions (`return`) vs Procedures (`call`).
- **Automation**: Triggers (Event-Condition-Action).
- **Hierarchy**: `with recursive` for transitive closure.
- **Analytics**: `rank`, `dense_rank`, window functions, `rollup`, and `cube`.
