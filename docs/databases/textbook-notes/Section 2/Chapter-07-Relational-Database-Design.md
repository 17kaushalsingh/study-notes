# Chapter 7: Relational Database Design

## 1. Features of Good Relational Designs

A good design should avoid:
- **Redundancy**: Information should not be repeated unnecessarily, as it leads to inconsistent updates and wasted space.
- **Null Values**: Designs that lead to many nulls make it difficult to write queries and join tables.

### Decomposition
The process of breaking a relation into two or more smaller relations.
- **Lossless-Join Decomposition**: A decomposition where the natural join of the smaller relations yields exactly the same set of tuples as the original relation.
- **Lossy Decomposition**: A bad decomposition where information is lost (specifically, more tuples are generated than existed originally when joined back).

---

## 2. Functional Dependencies (FDs)

Functional dependencies are constraints on the set of legal relations. They allow us to express that the value of one set of attributes determines the value of another set.

- **Definition**: $\alpha 	o \beta$ holds if for every pair of tuples $t_1, t_2$ such that $t_1[\alpha] = t_2[\alpha]$, it is also the case that $t_1[\beta] = t_2[\beta]$.
- **Superkey**: If $K 	o R$, then $K$ is a superkey for relation $R$.

### Armstrong’s Axioms
Rules for inferring new functional dependencies:
1. **Reflexivity**: If $\beta \subseteq \alpha$, then $\alpha 	o \beta$.
2. **Augmentation**: If $\alpha 	o \beta$, then $\gamma\alpha 	o \gamma\beta$.
3. **Transitivity**: If $\alpha 	o \beta$ and $\beta 	o \gamma$, then $\alpha 	o \gamma$.

---

## 3. Normal Forms

Normalization is a formal process used to evaluate and improve the design of relational schemas.

### First Normal Form (1NF)
A domain is atomic if its elements are considered to be indivisible. A relation schema $R$ is in 1NF if all its attribute domains are atomic.

### Boyce-Codd Normal Form (BCNF)
A relation schema $R$ is in BCNF if for all functional dependencies $\alpha 	o \beta$ in $F^+$, at least one of the following holds:
1. $\alpha 	o \beta$ is a trivial FD (i.e., $\beta \subseteq \alpha$).
2. $\alpha$ is a superkey for $R$.

### Third Normal Form (3NF)
A relation schema $R$ is in 3NF if for all $\alpha 	o \beta$ in $F^+$, at least one of the following holds:
1. $\alpha 	o \beta$ is trivial.
2. $\alpha$ is a superkey for $R$.
3. Each attribute $A$ in $\beta - \alpha$ is contained in a candidate key for $R$ (**prime attribute**).

---

## 4. Multivalued Dependencies and 4NF

Functional dependencies are not sufficient to handle multivalued attributes like `phone_number`.

- **Multivalued Dependency (MVD)**: $\alpha 	woheadrightarrow \beta$ indicates that the set of values for $\beta$ depends only on $\alpha$ and is independent of other attributes.
- **Fourth Normal Form (4NF)**: A schema $R$ is in 4NF if for every $\alpha 	woheadrightarrow \beta$, either the dependency is trivial or $\alpha$ is a superkey for $R$.

---

## 5. Normalization Algorithms

- **BCNF Decomposition**: Repeatedly decomposes a relation into smaller relations until all are in BCNF. This is **lossless** but not always **dependency preserving**.
- **3NF Synthesis**: A process that produces a set of relations that are in 3NF and are both **lossless** and **dependency preserving**.

---

## Must-Know Things
- **Lossless vs. Dependency Preserving**: BCNF guarantees no redundancy but may lose the ability to check FDs easily. 3NF allows some redundancy but preserves all dependencies.
- **Prime Attribute**: An attribute that is part of *any* candidate key.
- **Canonical Cover**: A simplified, minimal set of functional dependencies that has the same closure as the original set.

---

## Quick Revision
- **1NF**: Atomic attributes only.
- **2NF**: No partial dependencies (historical).
- **3NF**: No transitive dependencies (except for prime attributes).
- **BCNF**: Every determinant is a candidate key.
- **4NF**: Handles multivalued dependencies.
- **Closure**: Use attribute closure to check if an attribute set is a superkey.
