# Section 2 Summary: Database Design

This section focuses on the engineering principles required to design a high-quality database schema, moving from abstract conceptualization to formal mathematical normalization.

## Key Themes

### 1. Conceptual Design with E-R (Chapter 6)
The **Entity-Relationship (E-R) model** serves as the primary tool for communication between database designers and stakeholders. It allows for the visual representation of data requirements using entities (objects), relationships (associations), and attributes (properties). Key extensions like **Specialization** and **Aggregation** provide the flexibility to model complex real-world hierarchies.

### 2. Formal Design and Normalization (Chapter 7)
Normalization provides the theoretical framework to evaluate the "goodness" of a relational schema.
- **Goals**: The primary goal is to minimize **redundancy** and prevent **update anomalies**.
- **The Role of Dependencies**: Functional Dependencies (FDs) and Multivalued Dependencies (MVDs) are the core constraints used to identify structural issues in a schema.
- **Trade-offs**: Designers must often choose between **BCNF** (maximum redundancy removal but potentially losing dependency preservation) and **3NF** (preserves dependencies but allows some controlled redundancy).

## Core Concepts to Remember
- **Atomic Data**: All fields must be indivisible (1NF).
- **The Golden Rule of Keys**: In a well-normalized relation (BCNF), "every non-key attribute must depend on the key, the whole key, and nothing but the key."
- **Integrity**: Database design is not just about structure, but about ensuring that constraints correctly model the business rules of the enterprise.

## Transition to Next Section
Now that we have established how to interact with a database (Section 1) and how to design its structure (Section 2), the next section (Application Design and Development) explores how to build complete software systems that utilize these databases.
