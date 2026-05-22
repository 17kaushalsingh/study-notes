# Chapter 20: Database-System Architectures

## 1. Centralized Database Systems

- Run on a single computer system.
- Range from single-user (embedded databases) to high-performance multiuser server systems with multiple CPU cores and massive main memory.
- In modern multicore processors, parallelism with a small number of cores (and shared memory) is referred to as **coarse-grained parallelism**.

## 2. Server System Architectures

- **Transaction Servers** (Query Servers): Clients send requests (e.g., SQL queries) to perform an action. The server executes it and sends back results. This is the most widely used relational DB architecture.
- **Data Servers**: Supply raw data (files, pages, objects) to clients. Clients perform the processing and send updated data back.

## 3. Parallel Systems

Designed to improve throughput (number of transactions per second) and response time (time to complete a single task) by running tasks across many processors and disks.

### Measures of Performance
- **Speedup**: Running a given task in less time by increasing the degree of parallelism.
  - *Linear Speedup*: Speedup is $N$ when the system size is $N$ times larger.
  - *Sublinear Speedup*: Speedup is less than $N$.
- **Scaleup**: Handling larger tasks by increasing the degree of parallelism.
  - *Batch Scaleup*: Increasing database size and node count proportionally.
  - *Transaction Scaleup*: Increasing transaction rate and node count proportionally.

### Barriers to Parallelism
- **Sequential Computation**: Portions of the task cannot be parallelized (Amdahl's Law).
- **Interference**: Contention for shared resources (bus, locks, disks).
- **Skew**: Uneven distribution of work. The slowest parallel step dictates the overall speed.

### Interconnection Networks
- **Bus**: Shared communication line. Doesn't scale well.
- **Mesh / Hypercube**: Point-to-point connections.
- **Tree-like (Fat-Tree)**: Used in modern data centers with Top-of-Rack (Edge), Aggregation, and Core switches to provide high bandwidth and fault tolerance.

### Parallel Database Architectures
1. **Shared Memory**: All processors share a common memory and disks. Extremely fast communication but limited scalability (dozens of cores). Often uses **NUMA** (Non-Uniform Memory Access) where local memory access is faster than remote.
2. **Shared Disk**: Nodes have their own CPUs and memory but share a common pool of disks via a Storage Area Network (SAN). Good for high availability.
3. **Shared Nothing**: Each node has its own CPU, memory, and disks. Nodes communicate only via the network. Highly scalable (thousands of nodes). Used by modern massively parallel databases.
4. **Hierarchical**: A hybrid. A cluster of Shared Nothing nodes, where each individual node is a Shared Memory multi-core system.

## 4. Distributed Systems

The database is stored across geographically separated sites (Data Centers).
- **Motivation**: High availability (surviving natural disasters), reducing latency by putting data closer to users, and integrating legacy systems.
- **Challenges**: Network partitions, higher latency, lower bandwidth.
- **Homogeneous vs. Heterogeneous**: Homogeneous uses the same DBMS software globally. Heterogeneous (Federated) integrates different DBMS software under a global schema.

## 5. Cloud-Based Services

- **IaaS (Infrastructure-as-a-Service)**: Renting raw VMs or containers. Client installs the database.
- **PaaS (Platform-as-a-Service)**: Provider manages the database/storage platform. Client manages the application. Includes DBaaS (Database-as-a-Service) and Cloud Storage.
- **SaaS (Software-as-a-Service)**: Provider manages everything (e.g., Salesforce, Google Workspace).

**Virtual Machines vs Containers**: VMs emulate entire hardware and run full OSs (high overhead). Containers share the host OS kernel but have isolated filesystems and libraries (low overhead, highly elastic, great for microservices).