# Chapter 3: Introduction to SQL

## 1. Overview of SQL
SQL (Structured Query Language) is the standard language for relational databases. It has several parts:
- **Data-Definition Language (DDL)**: Commands for defining relation schemas, deleting relations, and modifying schemas.
- **Data-Manipulation Language (DML)**: Ability to query information, insert, delete, and modify tuples.
- **Integrity**: Commands for specifying integrity constraints.
- **View Definition**: Commands for defining views.
- **Transaction Control**: Commands for specifying the beginning and end of transactions.
- **Embedded and Dynamic SQL**: Defining how SQL statements can be embedded within general-purpose programming languages.
- **Authorization**: Commands for specifying access rights to relations and views.

---

## 2. SQL Data Definition (DDL)

### Basic Types
- `char(n)`: Fixed-length character string.
- `varchar(n)`: Variable-length character string.
- `int`: Integer.
- `numeric(p, d)`: Fixed-point number with precision $p$ and scale $d$.
- `real`, `double precision`: Floating-point numbers.
- `float(n)`: Floating-point number with precision of at least $n$ digits.

### Basic Schema Definition
- **Create Table**:
  ```sql
  create table department (
      dept_name varchar(20),
      building varchar(15),
      budget numeric(12,2),
      primary key (dept_name)
  );
  ```
- **Integrity Constraints**: `primary key`, `foreign key`, `not null`.
- **Drop Table**: `drop table r;` (removes table and all tuples).
- **Alter Table**: `alter table r add A D;` or `alter table r drop A;`.

---

## 3. Basic Structure of SQL Queries

A typical SQL query has the form:
```sql
select A1, A2, ..., An
from r1, r2, ..., rm
where P;
```

### Key Clauses
- **SELECT**: Used to list the attributes desired in the result (corresponds to Project in relational algebra).
  - Use `distinct` to force removal of duplicates: `select distinct dept_name ...`
  - Use `*` to select all attributes.
- **FROM**: Lists the relations to be accessed.
- **WHERE**: A predicate involving attributes of the relations in the FROM clause (corresponds to Select in relational algebra).
  - Connectives: `and`, `or`, `not`.
  - Comparisons: `<`, `<=`, `>`, `>=`, `=`, `<>`.

---

## 4. Additional Basic Operations

### The Rename Operation (`as`)
Used to rename both relations and attributes: `select name as instructor_name ...`

### String Operations
- `like` operator for pattern matching:
  - `%`: Matches any substring.
  - `_`: Matches any character.
  - Example: `where name like '%dar%'` (names containing "dar").

### Ordering and Predicates
- **Order By**: Sorts the result: `order by name desc, salary asc`.
- **Between**: `where salary between 90000 and 100000`.
- **Row Constructor**: `where (instructor.ID, dept_name) = (teaches.ID, 'Biology')`.

---

## 5. Set Operations
SQL operations correspond to mathematical set operations:
- `union`: Combines results (removes duplicates). Use `union all` to keep duplicates.
- `intersect`: Returns common tuples.
- `except`: Returns tuples in the first result but not the second.

---

## 6. Aggregate Functions
Functions that take a collection of values as input and return a single value.
- `avg`, `min`, `max`, `sum`, `count`.
- **Group By**: Groups tuples by given attributes: `group by dept_name`.
- **Having**: Applies predicates to groups: `having avg(salary) > 42000`.

---

## 7. Modification of the Database

### Deletion
`delete from instructor where dept_name = 'Finance';`

### Insertion
```sql
insert into course (course_id, title, dept_name, credits)
values ('CS-437', 'Database Systems', 'Comp. Sci.', 4);
```

### Updates
```sql
update instructor
set salary = salary * 1.05
where salary < 70000;
```
- **Case Statement**: Used for conditional updates.

---

## Must-Know Things
- **Execution Order**: SQL is written as `select-from-where`, but conceptually evaluated as `from -> where -> select`.
- **Null Handling**: Arithmetic with null results in null. Comparisons with null result in `unknown`.
- **Count(*) vs Count(A)**: `count(*)` counts all rows; `count(A)` counts non-null values of attribute A.
- **Group By Constraint**: Any attribute in the `select` clause that is not an aggregate must appear in the `group by` clause.

---

## Quick Revision
- **DDL**: `create`, `drop`, `alter`.
- **DML**: `select`, `insert`, `update`, `delete`.
- **Aggregates**: `avg`, `sum`, `count`, `min`, `max`.
- **Groups**: Use `group by` to aggregate and `having` to filter those aggregates.
- **Sets**: `union`, `intersect`, `except`.
- **Strings**: Use `like` with `%` and `_`.
