# Chapter 6: Database Design Using the E-R Model

## 1. Overview of the Design Process

Designing a database involves moving from a high-level requirement gathering phase to a physical implementation.

### Design Phases
1. **Requirement Analysis**: Characterize the data needs of the users.
2. **Conceptual Design**: Create a high-level description of the data (usually using the E-R model).
3. **Logical Design**: Map the conceptual schema to the implementation data model (e.g., the relational model).
4. **Physical Design**: Specify the physical storage features (file organization, indices).

---

## 2. The Entity-Relationship (E-R) Model

The E-R model employs three basic concepts: **entity sets**, **relationship sets**, and **attributes**.

- **Entity**: A "thing" or "object" in the real world that is distinguishable from other objects.
- **Entity Set**: A set of entities of the same type that share the same properties (e.g., all instructors).
- **Relationship**: An association among several entities.
- **Relationship Set**: A set of relationships of the same type.
- **Attributes**: Descriptive properties possessed by each member of an entity set.

---

## 3. Complex Attributes

Attributes can be characterized by their structure:
- **Simple vs. Composite**: Simple attributes are not divided into subparts. Composite attributes can be divided (e.g., `name` into `first_name`, `last_name`).
- **Single-valued vs. Multivalued**: Single-valued has one value for an entity (e.g., `ID`). Multivalued can have a set of values (e.g., `phone_number`).
- **Derived**: The value can be computed from other attributes (e.g., `age` from `date_of_birth`).

---

## 4. Mapping Cardinalities

Express the number of entities to which another entity can be associated via a relationship set.
- **One-to-one (1:1)**: An entity in A is associated with at most one entity in B, and vice versa.
- **One-to-many (1:N)**: An entity in A is associated with any number in B, but B is associated with at most one in A.
- **Many-to-one (N:1)**: Reverse of one-to-many.
- **Many-to-many (N:M)**: Entities in both sets can be associated with any number of entities in the other.

---

## 5. Weak Entity Sets

An entity set that does not have a primary key of its own is called a **weak entity set**.
- **Identifying Entity Set**: The entity set on which the weak entity set is existence-dependent.
- **Discriminator**: A set of attributes that allows the entities of a weak entity set to be distinguished (e.g., `sec_id` for sections of a course).
- **Identifying Relationship**: The relationship associating the weak entity set with the identifying entity set.

---

## 6. Extended E-R Features

### Specialization and Generalization
- **Specialization**: Top-down process of creating sub-groups within an entity set (e.g., `person` specialized into `student` and `employee`).
- **Generalization**: Bottom-up process of combining multiple entity sets into a higher-level entity set.
- **Attribute Inheritance**: A lower-level entity set inherits all attributes and relationship participation of its higher-level entity set.

### Aggregation
An abstraction through which relationships are treated as higher-level entities. This is useful when a relationship needs to participate in another relationship.

---

## 7. Reduction to Relational Schemas

E-R diagrams can be converted into a set of relation schemas:
- **Strong Entity Set**: Becomes a table with the same attributes.
- **Weak Entity Set**: Becomes a table including the primary key of the identifying entity set.
- **Relationship Set**: Becomes a table containing the primary keys of the participating entity sets.
- **Composite Attributes**: Represented by creating an attribute for each component.
- **Multivalued Attributes**: Require a separate table.

---

## Must-Know Things
- **Total vs. Partial Participation**: Total participation (double line) means every entity in the set must participate in at least one relationship.
- **Primary Key of Relationship**: 
  - For N:M, it is the union of primary keys of participating entity sets.
  - For many-to-one, it is the primary key of the "many" side.
- **Pitfalls**: Redundancy (repeating information) and Incompleteness (missing relationships).

---

## Quick Revision
- **E-R Model**: Entities (rectangles), Relationships (diamonds), Attributes (ovals).
- **Cardinality**: 1:1, 1:N, N:M.
- **Weak Entity**: Needs an owner and a discriminator.
- **Extended E-R**: Specialization/Generalization (ISA), Aggregation.
- **Mapping**: Entity $ightarrow$ Table; N:M Relationship $ightarrow$ Table; Multivalued Attr $ightarrow$ Table.
