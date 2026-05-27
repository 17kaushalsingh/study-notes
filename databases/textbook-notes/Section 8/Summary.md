# Section 8 Summary: Parallel and Distributed Databases

This section explores how database systems scale to handle petabytes of data and millions of users by spreading computation and storage across multiple machines, either within a single data center or across the globe.

## Key Themes

### 1. System Architectures (Chapter 20)
Modern hardware dictates database design.
- **Scalability**: Systems are designed for **Speedup** (doing a task faster) and **Scaleup** (doing a larger task in the same time).
- **Architectures**: While **Shared Memory** is fast for multi-core systems, **Shared Nothing** is the standard for massive scale, where each node is independent and communicates over a high-speed network.
- **Cloud Elasticity**: The move to the cloud has popularized **Containers** and **Serverless** architectures, allowing databases to expand or contract resources on demand.

### 2. Distributed Storage and Querying (Chapters 21 & 22)
Spreading data across nodes requires careful management.
- **Partitioning**: Data is split using **Hash** or **Range** partitioning. Range partitioning is better for range queries but prone to **Skew** (hot spots), which is mitigated using **Virtual Nodes**.
- **Parallel Execution**: Queries are broken into sub-tasks (Intraoperation parallelism) and coordinated using **Exchange Operators**. High-level frameworks like **MapReduce** and **Spark** abstract this complexity for Big Data tasks.
- **Replication**: Essential for availability. Replicas can be updated synchronously (consistent but slow) or asynchronously (fast but risks staleness).

### 3. Distributed Transactions and Consensus (Chapter 23)
Ensuring ACID properties across a network is fundamentally difficult.
- **Two-Phase Commit (2PC)**: The standard protocol for distributed atomicity. It ensures all nodes agree but is **blocking**; if the coordinator fails, participants are stuck.
- **CAP Theorem**: A fundamental law stating that in the presence of a network partition, a system must choose between **Consistency** and **Availability**.
- **Consensus Protocols**: Algorithms like **Paxos** and **Raft** provide a fault-tolerant way for nodes to agree on a sequence of actions, enabling highly available "replicated state machines" used for critical system metadata.

## Core Concepts to Remember
- **Network is the Bottleneck**: In distributed systems, minimizing data movement (shipping the query to the data, not the data to the query) is the primary optimization goal.
- **The Quorum Rule**: High availability is achieved through majorities. As long as more than half the nodes are healthy, the system can reach a consensus and continue operating.
- **Eventually Consistent (BASE)**: Many web-scale systems trade off strict ACID consistency for "Basic Availability" and "Eventual Consistency" to ensure the system never goes down.

## Transition to Next Section
Having covered the core architecture and mechanics of database systems, the final section (Advanced Topics) explores specialized areas like Blockchain databases, advanced indexing, and modern application development patterns.