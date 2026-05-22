# Chapter 10: Big Data

## 1. Motivation and Characteristics

Big Data refers to datasets so large and complex that traditional relational database systems are inadequate for storing or processing them.

### The Three V's
- **Volume**: The sheer amount of data (terabytes to petabytes).
- **Velocity**: The speed at which data is generated and needs to be processed (e.g., social media feeds, sensor data).
- **Variety**: Data comes in many formats (structured, semi-structured like JSON, unstructured like text/video).

---

## 2. Big Data Storage Systems

Traditional centralized databases cannot scale to Big Data levels. Instead, we use distributed systems.

- **Distributed File Systems (HDFS)**: The Hadoop Distributed File System splits large files into blocks and replicates them across a cluster of machines for fault tolerance.
  - **NameNode**: Stores metadata (file structure).
  - **DataNodes**: Store the actual data blocks.
- **Sharding**: Partitioning a large database across multiple smaller, independent databases.
- **Key-Value Stores (NoSQL)**: Systems like MongoDB or Cassandra that sacrifice some relational features (like complex joins) for massive scalability and high availability.

---

## 3. The MapReduce Paradigm

MapReduce is a programming model for processing vast amounts of data in parallel across a cluster.

### Core Steps
1. **Map**: A function that takes a set of data and converts it into another set of data, where individual elements are broken down into **key-value pairs**.
2. **Shuffle/Sort**: The system redistributes the data so that all pairs with the same key are grouped together.
3. **Reduce**: A function that takes the output from a map task and combines those data tuples into a smaller set of tuples.

---

## 4. Modern Distributed Processing: Apache Spark

Spark is a faster successor to Hadoop MapReduce that keeps more data in memory.

- **Resilient Distributed Datasets (RDDs)**: The core data structure in Spark. They are collections of objects partitioned across the cluster that can be operated on in parallel.
- **Lazy Evaluation**: Transformations on RDDs are not executed until an **action** (like `count` or `save`) is called.

---

## 5. Streaming Data

Data that arrives in a continuous fashion rather than being stored at rest.

- **Windowing**: Since streams are infinite, queries are often performed on a "window" of time (e.g., "count clicks in the last 5 minutes").
  - **Tumbling Windows**: Fixed-size, non-overlapping intervals.
  - **Sliding Windows**: Overlapping intervals.

---

## 6. Graph Databases

Used for data where relationships are as important as the data itself (e.g., social networks, recommendation engines).

- **Nodes**: Represent entities.
- **Edges**: Represent relationships between nodes.
- **Querying**: Uses specialized languages like **Cypher** (Neo4j) to traverse complex paths efficiently.

---

## Must-Know Things
- **Fault Tolerance in HDFS**: Data is replicated across multiple nodes so that if one fails, the data is still accessible.
- **The Shuffle Step**: This is often the most expensive part of a MapReduce job because it involves significant network traffic.
- **NoSQL Trade-off**: NoSQL databases often provide **Eventual Consistency** instead of the immediate consistency found in relational databases.

---

## Quick Revision
- **3 V's**: Volume, Velocity, Variety.
- **HDFS**: Distributed storage with replication.
- **MapReduce**: Map (key-value) $ightarrow$ Shuffle $ightarrow$ Reduce (aggregate).
- **Spark**: Uses RDDs and in-memory processing for speed.
- **Streaming**: Processes data-in-motion using windows.
- **Graphs**: Optimized for modeling associations.
