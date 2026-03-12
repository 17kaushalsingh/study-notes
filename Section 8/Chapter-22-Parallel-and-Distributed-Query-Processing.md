# Chapter 22: Parallel and Distributed Query Processing

## 1. Overview of Parallelism

- **Interquery Parallelism**: Executing multiple distinct queries concurrently to increase transaction throughput.
- **Intraquery Parallelism**: Splitting a single, long-running query into parts and executing them in parallel across multiple nodes to reduce response time.
  - **Intraoperation Parallelism**: Executing a *single* operation (e.g., a sort or a join) in parallel across multiple nodes (data parallelism).
  - **Interoperation Parallelism**: Executing *different* operations of the same query concurrently (e.g., pipelining the output of a join directly into a projection).

## 2. Parallel Sort

If data is already range-partitioned on the sort attribute, each node simply sorts its local data. If not:
- **Range-Partitioning Sort**: Redistribute the relation across nodes using a range-partition vector. Each node sorts its partition locally. The final sorted relation is just the concatenation of the nodes' outputs.
- **Parallel External Sort-Merge**: Each node sorts its local data first. Then, the sorted runs are range-partitioned and sent to destination nodes where they are merged.

## 3. Parallel Join

### Partitioned Join
Used for equi-joins.
- Both relations $r$ and $s$ are partitioned across nodes using the *exact same* hash function or range vector on the join attribute.
- Because matching tuples are guaranteed to be routed to the same node, each node simply computes the join of its local partitions independently.
- Can be implemented locally as a Hash Join, Merge Join, or Nested-Loop Join.

### Fragment-and-Replicate Join
Used when the join condition is an inequality (e.g., $r.A < s.B$) where partitioning doesn't work.
- **Asymmetric (Broadcast Join)**: Leave relation $r$ partitioned as is. *Replicate* (broadcast) the entirety of relation $s$ to every node. Each node computes the join locally. Ideal when $s$ is very small.
- **Symmetric**: Both relations are partitioned and partially replicated across a grid of nodes.

### Handling Skew in Joins
- **Virtual Node Partitioning**: Helps avoid skew.
- **Work Stealing**: Dynamic handling of skew. If a node finishes its virtual nodes early, it "steals" unprocessed virtual nodes from a heavily loaded (skewed) node.

## 4. MapReduce Operations

A functional paradigm for processing massive datasets, abstracting away the complex routing and fault-tolerance mechanics.
- **Map Phase**: A `map()` function processes local data and outputs `(key, value)` pairs.
- **Shuffle/Exchange**: The framework automatically groups all outputs by `key` and routes them to specific reducer nodes.
- **Reduce Phase**: A `reduce()` function receives a key and all its associated values, usually performing an aggregation.
- **Fault Tolerance**: Mappers write intermediate results to local disk. If a reducer fails, it is restarted elsewhere. If a mapper fails, only its specific chunk of work is re-executed.

## 5. The Exchange Operator Model

The Volcano model popularized wrapping the network communication inside a standard relational operator called **Exchange**.
- The query execution engine at each node runs standard, unmodified, single-node operators (Sort, Hash Join, Aggregate).
- The Exchange operator sits between them in the query tree, acting as a producer (pushing data to the network based on a partition function) and a consumer (pulling and merging data from the network).

## 6. Parallel Stream Processing

Dealing with infinite, high-velocity streams of data (e.g., clickstreams, IoT sensors).
- **Routing**: Data is routed through a Directed Acyclic Graph (DAG) of operators, often using **Publish-Subscribe** systems (like Apache Kafka).
- **Windowing**: Data is chunked into time-based or count-based windows to perform aggregations.
- **Fault Tolerance**: Must guarantee exactly-once, at-least-once, or at-most-once semantics. Handled via periodic checkpointing of operator state and replaying messages from the pub/sub log.

## 7. Distributed Query Processing

- **Data Integration**: Querying across multiple disparate databases.
- **Semijoin Strategy**: Used to reduce network traffic when joining $r_1$ at Site A with $r_2$ at Site B. Instead of sending all of $r_1$, Site A sends only the join attributes $\Pi_{join\_attrs}(r_1)$ to Site B. Site B computes the semijoin to find which tuples in $r_2$ actually match, and sends only those matching tuples back to Site A.
- **Bloom Filters**: Often used to compress the semijoin data even further. Site A sends a bit-array (Bloom filter) instead of the actual join keys.

## Must-Know Things
- **Intraoperation vs Interoperation**: Intra is sorting $1/N$ of a file on $N$ nodes. Inter is Node 1 doing a scan and feeding Node 2 which does a Hash Join.
- **Broadcast Join**: Know when to use it. If you are joining a 10TB `Sales` table with a 10MB `Regions` table, do not shuffle the 10TB table. Broadcast the 10MB table to all nodes holding the `Sales` data.

## Quick Revision
- **Parallel Sort**: Range-partition then local sort.
- **Parallel Join**: Hash-partition both tables on the join key $	o$ local join.
- **Exchange Operator**: Hides network complexity from the DB engine.
- **MapReduce**: Map (filter/extract) $	o$ Shuffle (group by key) $	o$ Reduce (aggregate).
- **Semijoin**: Saves network bandwidth by only shipping tuples that are guaranteed to join.