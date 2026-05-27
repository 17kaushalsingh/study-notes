# Chapter 16: Query Optimization

## 1. Overview

Query optimization is the process of selecting the most efficient query-evaluation plan from the many equivalent plans available for a given SQL query.
The optimizer must:
1. Generate logically equivalent relational-algebra expressions.
2. Annotate these expressions with specific algorithms (e.g., hash join vs. merge join) to create physical evaluation plans.
3. Estimate the cost of each plan based on catalog statistics and choose the cheapest one.

---

## 2. Transformation of Relational Expressions

Two relational-algebra expressions are **equivalent** if they generate the same set of tuples on any valid database instance.

### Key Equivalence Rules
- **Cascade of $\sigma$**: A conjunction of selections can be broken into individual selections. 
  $\sigma_{	heta_1 \wedge 	heta_2}(E) \equiv \sigma_{	heta_1}(\sigma_{	heta_2}(E))$
- **Commutativity of Selects**: $\sigma_{	heta_1}(\sigma_{	heta_2}(E)) \equiv \sigma_{	heta_2}(\sigma_{	heta_1}(E))$
- **Commutativity of Joins**: Natural joins and theta joins are commutative. $E_1 \bowtie E_2 \equiv E_2 \bowtie E_1$
- **Associativity of Joins**: $(E_1 \bowtie E_2) \bowtie E_3 \equiv E_1 \bowtie (E_2 \bowtie E_3)$
- **Pushing Selections**: "Perform selections as early as possible." Pushing a selection down the expression tree past a join drastically reduces the size of the intermediate relations.

---

## 3. Estimating Statistics of Expression Results

To evaluate the cost of a plan, the optimizer needs to estimate the size of intermediate results. It relies on metadata stored in the **System Catalog**.

### Catalog Information
- $n_r$: Number of tuples in relation $r$.
- $b_r$: Number of blocks containing tuples of $r$.
- $V(A, r)$: Number of distinct values for attribute $A$ in relation $r$.

### Histograms
To get better estimates than simply assuming a uniform distribution of values, databases use **histograms** (often **equi-depth histograms**) to capture the data distribution of an attribute, knowing exactly how many tuples fall into specific value ranges.

### Size Estimation Examples
- **Selection ($\sigma_{A=v}(r)$)**: Assuming uniform distribution, the estimated size is $n_r / V(A,r)$.
- **Join ($r \bowtie s$)**: If the join attribute is a primary key for $s$, the size of the join is at most $n_r$. If it is not a key for either, the size is estimated as $(n_r \cdot n_s) / V(A,s)$ or $(n_r \cdot n_s) / V(A,r)$ (usually taking the smaller of the two).

---

## 4. Choice of Evaluation Plans

### Cost-Based Join-Order Selection
The number of possible join orders grows exponentially with the number of relations (e.g., joining 10 relations yields billions of combinations).
- **Dynamic Programming**: Optimizers use dynamic programming to drastically reduce the search space. Once the best join order for a subset of relations is found, it is saved and reused, discarding all costlier ways to join that specific subset.
- **Left-Deep Join Trees**: Many optimizers (like System R) only consider left-deep join trees. This limits the search space to $O(n!)$ and is highly desirable because the inner operand is always a base relation, making pipelining highly efficient.

### Heuristics in Optimization
Because full cost-based optimization is expensive, optimizers apply heuristics first:
- Push selections down early to reduce data size.
- Push projections down early.
- Avoid Cartesian products unless absolutely necessary.

---

## 5. Materialized Views
A materialized view physically stores the result of a view query on disk.
- **View Maintenance**: The challenge is keeping the view up-to-date. **Incremental view maintenance** updates only the changed parts of the view (using the *differential* of the insert/delete operations) rather than recomputing the entire view from scratch.
- Query optimizers can automatically rewrite user queries to query a materialized view instead of the base tables if it calculates that doing so is cheaper.

---

## Must-Know Things
- **Optimization Trade-off**: The query optimizer has a "cost budget". It stops searching for the optimal plan when the time spent optimizing exceeds the estimated time saved by finding a slightly better plan.
- **Left-Deep Trees**: Understand why they are favored: they allow the result of a join to be directly piped into the outer loop of the next join, avoiding the need to write intermediate results to disk.

---

## Quick Revision
- **Equivalence Rules**: Commutativity and associativity allow joining tables in any order.
- **Catalog**: Stores $n_r$ (tuples), $b_r$ (blocks), $V(A, r)$ (distinct values).
- **Histograms**: Improve cardinality estimates for skewed data.
- **Dynamic Programming**: Used to find the cheapest join order efficiently without brute-forcing every combination.
- **Materialized Views**: Speed up reads but slow down writes (requires view maintenance).
