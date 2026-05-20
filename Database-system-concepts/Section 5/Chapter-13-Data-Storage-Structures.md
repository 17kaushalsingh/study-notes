# Chapter 13: Data Storage Structures

## 1. Database Storage Architecture

Databases map their logical structures (relations/tables) down to physical structures on disk (files and blocks).
- **Blocks (Pages)**: The unit of storage allocation and data transfer between disk and main memory (typically 4KB to 8KB). A block can contain several records.
- Records are generally not allowed to cross block boundaries (to prevent requiring two I/O reads for one record).

---

## 2. File Organization and Record Representation

### Fixed-Length Records
Records where all attributes are of fixed size.
- **Problem**: Deletion leaves gaps.
- **Solution**: Use a **Free List** (a linked list of pointers) to keep track of deleted spaces, so new records can reuse that space instead of just shifting all subsequent records.

### Variable-Length Records
Required for `varchar` strings, or types that can repeat (arrays).
- **Record Structure**: Usually starts with a fixed-length header containing offsets and lengths for each variable-sized attribute, followed by the actual variable-length data. It also includes a **null bitmap** to cheaply indicate which attributes are null.
- **Slotted-Page Structure**: The standard way to organize variable-length records within a block.
  - A block header at the start of the block points to the exact offset of each record.
  - Records are stored starting from the *end* of the block, growing backward.
  - The free space is between the header array and the data records.
  - This allows records to be moved around within the block (e.g., during defragmentation) without invalidating external pointers, since external pointers only point to the slot entry in the header.

### Large Objects (LOBs)
`BLOB` and `CLOB` data can be larger than a block. They are stored in separate files or structures (like a B+-tree of byte chunks), and the main record just holds a pointer to the LOB.

---

## 3. Organization of Records in Files

- **Heap File**: Records are placed anywhere there is space. Uses a **Free-Space Map** to quickly find blocks with enough room for insertion.
- **Sequential File**: Records are stored sequentially by a search key. Requires overflow blocks and periodic reorganization when new inserts mess up the physical sorting.
- **Multitable Clustering File**: Records of *different* relations that are frequently joined are stored together in the same block. (e.g., storing a `Department` record right next to all its `Instructor` records). Great for specific joins, terrible for scanning a single table.

---

## 4. Database Buffer Management

The **Buffer Manager** handles bringing disk blocks into main memory. Since memory is limited, it must decide which blocks to evict when space is needed.

### Key Concepts
- **Pinned Blocks**: A block is "pinned" in memory while a process is actively reading/writing it, preventing the buffer manager from evicting it.
- **Shared / Exclusive Locks**: Ensure that multiple processes don't interfere with each other when reading or updating a block.
- **Forced Output**: The recovery system sometimes forces a block to be written to disk immediately to ensure durability (WAL - Write-Ahead Logging).

### Replacement Strategies
- **LRU (Least Recently Used)**: Standard OS strategy. Evicts the block that hasn't been accessed in the longest time.
- **MRU (Most Recently Used)**: Better for database operations like nested-loop joins. If a block of a table is scanned once and won't be needed again until the next outer loop, it should be tossed immediately (Toss-Immediate) or evicted first.

---

## 5. Column-Oriented Storage

Traditional databases use **Row-Oriented Storage** (storing all attributes of a tuple together). **Column-Oriented Storage (Column Stores)** stores all values of a single attribute (column) consecutively.

### Advantages for Analytics (Data Warehousing)
1. **Reduced I/O**: If a query only needs 3 columns out of 100, it only reads those 3 files.
2. **Improved Compression**: Values in a single column are of the same type and often similar, allowing highly efficient compression (e.g., Run-Length Encoding).
3. **Vector Processing**: CPUs can process operations on contiguous arrays of identical data types much faster.

### Drawbacks
- **Tuple Reconstruction**: Very expensive if a query needs to output an entire row.
- **Deletions/Updates**: Modifying a single row requires updating multiple compressed column files, making it terrible for Transaction Processing (OLTP).

*Note: Formats like **ORC** and **Parquet** are hybrid columnar formats heavily used in Big Data systems (like Hadoop/Spark).*

---

## Must-Know Things
- **Slotted-Page Indirection**: Understand why pointers point to the header slot, not the direct memory address of the record. It's the key to intra-block defragmentation.
- **Buffer Manager > OS VM**: Databases often bypass OS memory management because the DB knows query access patterns (e.g., knowing when to use MRU instead of LRU) and needs strict control over disk writes for crash recovery.

---

## Quick Revision
- **Slotted-Page**: Block header with pointers $	o$ Free Space $\leftarrow$ Records growing from the end.
- **Buffer Replacements**: LRU (default) vs. MRU (good for sequential scans that won't be repeated soon).
- **Row Store**: Best for OLTP (Transaction Processing - inserts, updates, fetching whole rows).
- **Column Store**: Best for OLAP (Data Warehousing - aggregations on specific columns).
