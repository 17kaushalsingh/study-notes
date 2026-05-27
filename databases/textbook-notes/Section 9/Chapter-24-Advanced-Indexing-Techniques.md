# Chapter 24: Advanced Indexing Techniques

This chapter explores sophisticated indexing structures designed for specific workloads, such as write-heavy environments, multi-attribute queries, and spatial data.

## Detailed Notes

### 1. Bloom Filters
A probabilistic data structure used to check if an element is a member of a set.
- **Mechanism**: Uses a bitmap of $n$ bits and $k$ hash functions. Each element is hashed $k$ times, and the corresponding bits are set to 1.
- **Query**: To check if $v$ is in the set, hash $v$ $k$ times. If any bit is 0, $v$ is definitely **not** in the set. If all are 1, $v$ is **likely** in the set.
- **Trade-off**: No false negatives, but potential for **false positives**.
- **Use Case**: Used in LSM trees to avoid expensive disk lookups for keys that don't exist.

### 2. Log-Structured Merge (LSM) Trees
Designed to handle high-volume write workloads by transforming random I/O into sequential I/O.
- **Architecture**:
    - **$L_0$ (In-memory)**: A B+-tree or skip list.
    - **$L_1 \dots L_k$ (On-disk)**: Increasingly larger levels of sorted files (SSTables).
- **Operations**:
    - **Inserts**: Always written to $L_0$.
    - **Rolling Merge**: When $L_i$ fills up, its contents are merged (compacted) into $L_{i+1}$ using sequential I/O.
    - **Deletes**: Handled by inserting a **tombstone** (deletion entry) rather than physically deleting data immediately.
- **Stepped-Merge Index**: A variant where each level contains multiple trees, reducing write amplification at the cost of lookup performance.

### 3. Bitmap Indices
Specialized indices for attributes with a small number of distinct values (low cardinality).
- **Structure**: For each distinct value, a bit vector is maintained. The $i$-th bit is 1 if the $i$-th record has that value.
- **Advantages**:
    - **Space Efficiency**: Very compact for low-cardinality data.
    - **Bitwise Operations**: Complex queries (e.g., `gender='F' AND income_level='L2'`) can be resolved using fast bitwise `AND`, `OR`, and `NOT` operations.
- **Existence Bitmap**: A special bitmap to track which record slots actually contain valid records (handling deletions).

### 4. Indexing of Spatial Data
Used for geographic or geometric data.
- **Quadtrees**: Recursively divide 2D space into four quadrants until each leaf node contains a limited number of points.
    - **PR Quadtree**: Point Region quadtree, splits based on space rather than point distribution.
- **R-Trees**: A balanced tree structure where each node represents a **bounding box** (rectangle).
    - **Hierarchical**: Leaf nodes contain objects; internal nodes contain the smallest bounding box that encompasses all children.
    - **Search**: Can follow multiple paths because bounding boxes may overlap.
    - **Quadratic Split**: An algorithm to split a node by minimizing the total area of the resulting two bounding boxes.

### 5. Dynamic Hashing
- **Extendable Hashing**: Handles data growth by splitting buckets. Uses a prefix of the hash value to direct searches via a **bucket address table**.
- **Linear Hashing**: Allows the hash table to grow one bucket at a time without a global directory, using a temporary split pointer.

## Must-Know Things
- **Bloom Filter False Positives**: The probability depends on the size of the bitmap and the number of hash functions.
- **Write Amplification**: In LSM trees, the total number of bytes written to disk divided by the size of the data inserted.
- **R-Tree Search**: Unlike B+-trees, a point lookup in an R-tree might require traversing multiple branches due to overlapping bounding boxes.
- **Bitmap Cardinality**: Bitmaps become inefficient for high-cardinality attributes (e.g., Primary Keys) due to size and sparsity.

## Quick Revision
- **LSM Trees**: Best for heavy writes (sequential I/O). Uses Bloom filters to speed up reads.
- **Bitmaps**: Best for low-cardinality attributes. Uses bitwise logic for fast filtering.
- **Spatial Indexing**: Quadtrees (fixed grid splitting) vs. R-trees (dynamic bounding boxes).
- **Dynamic Hashing**: Extendable hashing uses a directory; Linear hashing grows incrementally.