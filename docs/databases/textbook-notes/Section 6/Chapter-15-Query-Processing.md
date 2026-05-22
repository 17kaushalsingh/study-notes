# Chapter 15: Query Processing

## 1. Overview
Query processing refers to the activities involved in extracting data from a database. 
The basic steps are:
1. **Parsing and Translation**: Translating SQL into an internal relational-algebra representation.
2. **Optimization**: Finding the most efficient query-evaluation plan.
3. **Evaluation**: The query-execution engine runs the plan and returns the results.

A **query-evaluation plan** is a sequence of primitive operations annotated with instructions on how to evaluate them (e.g., "use index 1 for this selection").

---

## 2. Measures of Query Cost
Cost is primarily measured in terms of disk I/O, as it usually dominates CPU time in disk-resident databases.
- $t_S$: Average disk seek time (plus rotational latency).
- $t_T$: Average time to transfer a block of data.
- **Total Cost**: $b \cdot t_T + S \cdot t_S$ (where $b$ is the number of blocks transferred, and $S$ is the number of random seeks).

---

## 3. Selection Operation
- **A1 (Linear Search)**: Scans every block. Cost is $t_S + b_r \cdot t_T$.
- **A2 (Clustering Index, Equality on Key)**: Uses a primary index. Cost is $(h_i + 1) \cdot (t_T + t_S)$, where $h_i$ is the height of the index.
- **A3 (Clustering Index, Equality on Non-key)**: Fetches multiple records stored consecutively.
- **A4 (Secondary Index)**: If multiple records match on a secondary index, they may be scattered across many blocks, causing a high seek cost.

---

## 4. Sorting
Since relations might be too large to fit in main memory, databases use **External Sorting**, most commonly the **External Sort-Merge** algorithm.
1. **Create Runs**: Read $M$ blocks into memory, sort them, and write them out as a sorted "run".
2. **Merge**: Read the first block of each run, merge them in memory, and write out to the final sorted file. If there are too many runs to merge at once, this is done in multiple passes.

---

## 5. Join Operations
Joins are the most expensive operations in relational databases.

### Nested-Loop Join
- Two nested `for` loops. Very slow if relations don't fit in memory.
- **Block Nested-Loop Join**: Compares *blocks* instead of individual tuples, vastly reducing the number of disk accesses.

### Indexed Nested-Loop Join
- If an index exists on the inner relation's join attribute, the inner loop scan is replaced with a fast index lookup.

### Merge Join (Sort-Merge Join)
- Both relations must be sorted on the join attribute.
- A single pass through both relations merges the matching tuples. Very efficient ($O(b_r + b_s)$) if the relations are already sorted.

### Hash Join
- Uses a hash function to partition both relations $r$ and $s$ into $n$ smaller partitions ($r_i$ and $s_i$).
- Because the same hash function is used, matching tuples *must* be in corresponding partitions.
- The join is then performed by building an in-memory hash index on $s_i$ and probing it with $r_i$.

---

## 6. Evaluation of Expressions
- **Materialization**: Evaluates one operation at a time, writing the intermediate results to a temporary disk file. Simple, but involves heavy I/O overhead.
- **Pipelining**: Combines multiple operations. As one operation generates a tuple, it is immediately passed to the next operation without writing to disk.
  - **Demand-driven (Lazy)**: The top operator pulls data up from below using `open()`, `next()`, and `close()` iterators.
  - **Producer-driven (Eager)**: Bottom operators push data up into buffers.

---

## Must-Know Things
- **Hash Join vs. Merge Join**: Hash join is generally faster if relations are unsorted. Merge join is preferred if relations are already sorted or if the result needs to be sorted for a subsequent operation.
- **Pipeline Blocking Operations**: You cannot pipeline a `Sort` or a `Hash` (build phase) operation completely, because they require seeing all the data before they can output the first tuple.

---

## Quick Revision
- **Cost**: Dominated by block transfers and seeks.
- **Selection**: Linear scan vs. Index scan.
- **Sorting**: External Sort-Merge handles data larger than RAM.
- **Joins**: Nested-loop (worst), Block nested-loop, Indexed nested-loop, Merge join (needs sorted data), Hash join (needs partitioned data).
- **Execution**: Materialization (writes to disk) vs. Pipelining (keeps data in memory/cache).