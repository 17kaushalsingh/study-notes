# Chapter 4: Intermediate SQL

## 1. Join Expressions

Joins are used to combine records from two or more tables based on a related column.

### Types of Joins
- **Natural Join**: Matches tuples with the same values for all common attributes.
  - `select name, title from student natural join takes natural join course;`
- **Join with `using`**: Specify exactly which columns to equate.
  - `select name, title from student join takes using (ID);`
- **Join with `on`**: General join condition (like a `where` clause for the join).
  - `select * from student join takes on student.ID = takes.ID;`

### Outer Joins
Preserve tuples that would otherwise be lost by the join by using null values.
- **Left Outer Join**: Preserves all tuples in the left relation.
- **Right Outer Join**: Preserves all tuples in the right relation.
- **Full Outer Join**: Preserves all tuples in both relations.

---

## 2. Views

A view is a "virtual relation" that is not part of the logical model but is made visible to a user.

### View Definition
```sql
create view faculty as
select ID, name, dept_name
from instructor;
```

### Materialized Views
Some systems store the results of a view physically to speed up access. These must be updated when the underlying tables change (**view maintenance**).

---

## 3. Transactions

A transaction is a sequence of query and/or update statements that forms a single logical unit of work.
- **Commit**: Makes the updates permanent in the database.
- **Rollback**: Undoes all updates performed by the current transaction.
- **Atomicity**: Either all operations of the transaction are reflected in the database, or none are.

---

## 4. Integrity Constraints

Ensures that changes made to the database by authorized users do not result in a loss of data consistency.

- **Not Null**: `name varchar(20) not null`.
- **Unique**: `unique (A1, A2, ..., An)` ensures candidates for a superkey.
- **Check Clause**: `check (budget > 0)`.
- **Referential Integrity**: Ensures that a value appearing in one relation for a given set of attributes also appears for a certain set of attributes in another relation (`foreign key`).
  - **Cascading Actions**: `on delete cascade`, `on update set null`.

---

## 5. SQL Data Types and Schemas

### Specialized Types
- **Date/Time**: `date`, `time`, `timestamp`.
- **Large Objects**: `blob` (binary) and `clob` (character).
- **User-Defined Types**: `create type Dollars as numeric(12,2) final;`.
- **Domains**: `create domain PersonName varchar(20) not null;`.

### Indices
Indices help find tuples without scanning the entire relation.
`create index dept_index on instructor (dept_name);`

---

## 6. Authorization

We may assign a user several forms of authorizations on parts of the database.
- **Privileges**: `select`, `insert`, `update`, `delete`.
- **Grant**: `grant select on department to Amit;`.
- **Revoke**: `revoke select on department from Amit;`.
- **Roles**: Create a set of privileges and assign them to users.
  ```sql
  create role instructor;
  grant select on takes to instructor;
  grant instructor to Amit;
  ```

---

## Must-Know Things
- **Outer Join Padding**: Outer joins fill missing data with `null`.
- **View Updatability**: A view is generally updateable if it is defined on a single relation without aggregates or `distinct`.
- **ACID Properties**: Transactions must satisfy Atomicity, Consistency, Isolation, and Durability (covered in detail in Chapter 17, but introduced here via commit/rollback).
- **Referential Integrity vs. Foreign Key**: A foreign key is a specific type of referential integrity constraint.

---

## Quick Revision
- **Joins**: Natural, Inner (on/using), Outer (Left, Right, Full).
- **Views**: `create view`, virtual vs. materialized.
- **Transactions**: `commit` and `rollback`.
- **Constraints**: `not null`, `unique`, `check`, `foreign key`.
- **Security**: `grant`, `revoke`, and `roles`.
- **Indexing**: `create index` for performance.
