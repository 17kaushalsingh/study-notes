# Chapter 21: Parallel and Distributed Storage

## 1. Data Partitioning

In shared-nothing and distributed systems, data must be partitioned (fragmented) across multiple nodes.

### Partitioning Strategies
- **Round-Robin**: Scans the relation and assigns the $i$-th tuple to node $i \pmod n$. Excellent for sequential scans (even load) but terrible for point/range queries (requires querying all nodes).
- **Hash Partitioning**: Applies a hash function to the partitioning attribute. Excellent for point queries on the hash key (only one node queried). Bad for range queries.
- **Range Partitioning**: Assigns contiguous attribute-value ranges to each node using a partitioning vector. Excellent for both point and range queries on the partitioning attribute. Prone to **execution skew** (hot spots).

### Dealing with Skew
Skew (uneven load) destroys parallel performance.
- **Attribute-Value Skew**: Many tuples have the exact same value.
- **Partition Skew**: Poorly chosen boundaries (in range partitioning) lead to uneven sizes.
- **Solutions**:
  - **Balanced Partitioning Vectors**: Using histograms/sampling to define equal-weight ranges.
  - **Virtual Node Partitioning**: Creating many more virtual partitions (tablets) than physical nodes, then assigning multiple virtual nodes to each physical node (round-robin). Spreads out hot spots.
  - **Dynamic Repartitioning**: If a virtual node (tablet) gets too large, split it in two and migrate one to a less loaded physical node. Managed via a **Partition Table** tracked by a Master node.

## 2. Replication

Replication is critical for **High Availability** and fault tolerance.

- **Replica Location**: 
  - *Within a Datacenter*: Typically to a different rack to survive rack power/switch failures.
  - *Across Datacenters (Geo-Replication)*: To survive whole-datacenter disasters (fires, earthquakes) and provide low-latency reads to geographically distant users.

### Consistency vs Availability
Updating replicas introduces consistency challenges.
- **Synchronous Replication (2PC)**: Updates are applied to all replicas atomically. High consistency, but high latency and lower availability (if one node is down, writes block).
- **Asynchronous (Lazy) Replication**: Update is applied to a **Master Replica**, the transaction commits immediately, and updates are pushed to other replicas in the background. High performance and availability, but risks **lost updates** if the master dies before propagating, and readers might see stale data.
- **Quorum Consensus**: To read, lock $Q_r$ nodes. To write, lock $Q_w$ nodes. Must satisfy $Q_r + Q_w > S$ and $2 \cdot Q_w > S$ (where $S$ is total nodes). Provides tunable consistency.

## 3. Parallel Indexing

- **Local Index**: Built only on the tuples stored on a specific node.
- **Global Index**: An index covering data across all nodes.
  - *Global Primary Index*: Built on the partitioning attribute. It's just a collection of the local indices.
  - *Global Secondary Index*: Built on a non-partitioning attribute. To be efficient, the secondary index itself must be partitioned and distributed across the nodes.

## 4. Distributed File Systems (e.g., HDFS)

Designed for storing millions of very large files across thousands of commodity machines.
- **Architecture**: A central **NameNode** stores all metadata (directory tree, file-to-block mapping). **DataNodes** store the actual 64MB+ blocks.
- **Replication**: Every block is replicated (usually 3 times).
- **Immutable Files**: To avoid complex distributed locking, systems like HDFS often employ a **Write-Once-Read-Many (WORM)** model. Files can be appended to, but not updated in place.

## 5. Parallel Key-Value Stores (NoSQL)

Designed for massive scale (billions of small records) where traditional relational databases hit limits.
- **Data Models**: Pure Key-Value, Wide-Column Stores (Bigtable, HBase, Cassandra), Document Stores (MongoDB).
- **Lack of Declarative Queries**: Usually no SQL. Access is via simple `put()`, `get()`, `delete()` API calls.
- **Transactions**: Mostly restricted to atomic operations on a *single row/document*. Multi-row ACID transactions are often sacrificed for scalability, though systems like Google Spanner support them.
- **LSM Trees**: Because they run on top of append-only distributed file systems, many key-value stores use Log-Structured Merge (LSM) Trees instead of B+-Trees for indexing.

## Must-Know Things
- **Virtual Nodes (Tablets)**: The standard industry solution for elasticity and skew. Data is hashed/ranged into tablets; physical servers host many tablets.
- **Consistent Hashing**: Used by systems like Dynamo to avoid massive data movement when adding/removing nodes. Keys and nodes are hashed onto a ring.

## Quick Revision
- **Partitioning**: Round-Robin (Scan), Hash (Point), Range (Point & Range).
- **Skew**: Virtual node routing fixes it.
- **Replication**: Synchronous (Safe, slow) vs Asynchronous (Fast, allows stale reads).
- **Key-Value Stores**: Highly scalable, flexible schema, drop complex joins/transactions for raw speed and elasticity.