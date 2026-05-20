# Chapter 14: Indexing

## 1. Basic Concepts

Indices are structures used to speed up the retrieval of records without having to scan the entire database table. They consist of **Index Entries** (or index records) containing a **search key** and a **pointer** to the record(s) on disk.

### Types of Ordered Indices
- **Clustering Index (Primary Index)**: The search key specifies the sequential order of the file. (The records themselves are physically sorted on this key).
- **Nonclustering Index (Secondary Index)**: The search key specifies an order different from the sequential order of the physical file.

### Dense vs. Sparse Indices
- **Dense Index**: An index entry appears for *every* search-key value in the file. Finding a record is faster, but it takes up more memory. Secondary indices *must* be dense.
- **Sparse Index**: An index entry appears for only *some* search-key values. Requires the physical file to be sorted on the search key (must be a clustering index). It takes up less space and has lower maintenance overhead, but finding a record requires finding the closest index entry and then scanning the physical file sequentially.

### Multilevel Indices
If an index is too large to fit in main memory, it is treated like a sequential file itself, and a *sparse outer index* is created on top of it, creating a tree-like structure.

---

## 2. B+-Tree Index Files

The B+-tree is the most widely used index structure because it remains balanced and efficient despite constant insertions and deletions, avoiding the need for periodic file reorganizations.

### Structure
- It is a **Balanced Tree**: Every path from the root to a leaf is exactly the same length.
- **Node Size**: Typically matches the disk block size (e.g., 4KB) to minimize I/O operations. Nodes are "fat" with a high fanout (large number of pointers).
- **Leaf Nodes**: Contain search-key values and pointers directly to the records. Leaf nodes are chained together via a pointer (`P_n`) to allow for efficient sequential/range queries.
- **Internal Nodes**: Contain search-key values and pointers to lower-level tree nodes. They act purely as a routing map to guide the search to the correct leaf node.

### Operations and Complexity
- **Lookup**: Traverses from the root to a leaf. Worst-case cost is proportional to $\log_{\lceil n/2 ceil}(N)$, where $n$ is the fanout (usually 100-200) and $N$ is the number of records. This typically results in a tree height of 3 or 4, meaning very few disk reads.
- **Insertion**: Find the correct leaf. If the leaf is full, a **node split** occurs, and the median key is pushed up to the parent. This can propagate up to the root.
- **Deletion**: Find the key and delete it. If the node becomes under-full (less than half full), it must either **borrow** an entry from a sibling or **merge (coalesce)** with a sibling.
- **Range Queries**: Highly efficient. The tree is traversed to find the starting value, and then the linked-list of leaf nodes is followed to retrieve all subsequent values in the range.

---

## 3. Hash Indices

Hashing uses a hash function to map search-key values to specific buckets (blocks) on disk.

- **Static Hashing**: The number of buckets is fixed.
  - **Bucket Overflow**: Handled using **Overflow Chaining** (a linked list of overflow blocks).
- **Lookup**: Extremely fast for exact matches ($O(1)$ block accesses ideally).
- **Limitation**: Cannot efficiently handle **Range Queries** (e.g., `salary > 50000`) because hash functions randomly scatter data, destroying sorting order.

---

## 4. Bitmap Indices

Bitmap indices are optimized for attributes that have a very small number of distinct values (low cardinality), such as `gender`, `status`, or `income_level`.

- **Structure**: For an attribute with $N$ possible values, $N$ bitmaps are created. Each bitmap is an array of bits where the $i$-th bit is 1 if the $i$-th record has that value, and 0 otherwise.
- **Advantage**: Extremely fast for evaluating complex `WHERE` clauses with multiple `AND` / `OR` conditions. The database can simply perform bitwise logical `AND` / `OR` operations directly on the bitmaps in CPU registers before fetching any actual records from disk.

---

## 5. Indexing Spatial Data

Traditional B+-trees don't work for multi-dimensional data (e.g., GPS coordinates, polygons).
- **R-Trees**: An extension of B-trees for spatial data. Instead of key ranges, nodes store **Bounding Boxes** (the smallest rectangle that encloses all objects in its child nodes). Used heavily in Geographic Information Systems (GIS) for "region" or "nearness" queries.

---

## Must-Know Things
- **B+-Tree vs B-Tree**: In a B+-tree, all record pointers are at the leaf level, and internal nodes only contain routing keys. In a standard B-Tree, internal nodes also contain record pointers. B+-trees are preferred because they allow higher fanout in internal nodes (making the tree shorter) and better sequential access via the leaf-level linked list.
- **Secondary Indices & Relocation**: If the primary data file is reorganized (e.g., a B+-tree leaf splits), physical record addresses change. To prevent having to update all secondary indices, secondary indices usually store the *Primary Key* of the record rather than a direct physical pointer.

---

## Quick Revision
- **Dense**: Entry for every key. **Sparse**: Entry for some keys (requires sorted data).
- **B+-Tree**: Balanced, $O(\log N)$ depth, leaf nodes chained. Great for exact matches and range queries.
- **Hash Index**: $O(1)$ exact match, terrible for range queries.
- **Bitmap Index**: Bitwise operations for fast multi-attribute filtering on low-cardinality data.
