# Chapter 2: Introduction to the Relational Model

## 1. Structure of Relational Databases

A relational database consists of a collection of **tables**, each of which is assigned a unique name. In the relational model, the term **relation** is used to refer to a table, while **tuple** refers to a row and **attribute** refers to a column.
 
### Core Components
- **Relation (Table)**: A set of tuples.
- **Tuple (Row)**: A specific record in a table.
- **Attribute (Column)**: A named property of the relation.
- **Domain**: For each attribute, there is a set of permitted values, called the domain of that attribute.
- **Atomic Domain**: A domain is atomic if elements of the domain are considered to be indivisible units.
- **Null Value**: A special value that signifies that the value is unknown or does not exist.

---

## 2. Database Schema and Instance

- **Database Schema**: The logical design of the database (e.g., table structure, attribute names, types).
- **Database Instance**: A snapshot of the data in the database at a given instant in time.
- **Relation Schema**: Corresponds to the programming-language notion of a type definition.
- **Relation Instance**: Corresponds to the notion of a value of a variable.

---

## 3. Keys

Keys are used to uniquely identify tuples within a relation and to establish relationships between tables.

### Types of Keys
| Key Type | Description |
| :--- | :--- |
| **Superkey** | A set of one or more attributes that, taken collectively, allow us to identify uniquely a tuple in the relation. |
| **Candidate Key** | A minimal superkey; a superkey for which no proper subset is also a superkey. |
| **Primary Key** | A candidate key chosen by the database designer as the principal means of identifying tuples within a relation. |
| **Foreign Key** | An attribute in one relation (referencing relation) that matches the primary key of another relation (referenced relation). |

### Constraints
- **Referential Integrity Constraint**: Requires that the values appearing in specified attributes of any tuple in the referencing relation also appear in specified attributes of at least one tuple in the referenced relation.

---

## 4. Relational Query Languages

Query languages can be categorized into:
1. **Imperative**: The user instructs the system to perform a specific sequence of operations on the database to compute the result.
2. **Functional**: The computation is expressed as the evaluation of functions that may operate on data in the database.
3. **Declarative**: The user describes the desired information without giving a specific sequence of steps for obtaining that information (e.g., SQL).

---

## 5. Relational Algebra

Relational algebra is a formal query language that forms the basis for SQL. It consists of a set of operations that take one or two relations as input and produce a new relation as their result.

### Fundamental Operations
| Operation | Symbol | Description |
| :--- | :---: | :--- |
| **Select** | $\sigma$ | Selects tuples that satisfy a given predicate. |
| **Project** | $\Pi$ | Returns its argument relation, with certain attributes left out. |
| **Cartesian Product** | $	imes$ | Combines information from any two relations. |
| **Join** | $\Join$ | Combines a selection and a Cartesian product into a single operation. |
| **Union** | $\cup$ | Combines two relations with same schema into one. |
| **Set Difference** | $-$ | Finds tuples that are in one relation but not in another. |
| **Intersection** | $\cap$ | Finds tuples that are in both relations. |
| **Rename** | $
ho$ | Allows us to refer to a relation by more than one name. |
| **Assignment** | $\leftarrow$ | Assigns parts of a relational-algebra expression to temporary relation variables. |

---

## Must-Know Things
- **Schema vs. Instance**: Understand that schema is static (design) while instance is dynamic (data).
- **Primary Key Selection**: Primary keys must be non-null and unique.
- **Referential Integrity**: Crucial for maintaining consistency across related tables.
- **Relational Algebra Symbols**: $\sigma$ (Select), $\Pi$ (Project), $\Join$ (Join) are fundamental to understanding query optimization.

---

## Quick Revision
- **Relation = Table, Tuple = Row, Attribute = Column.**
- **Domains** must be atomic; **Nulls** represent missing data.
- **Keys**: Superkey $\supseteq$ Candidate Key $\supseteq$ Primary Key.
- **Foreign Keys** link tables and must satisfy referential integrity.
- **Relational Algebra** operations like Select, Project, and Join are the mathematical foundation of SQL.
