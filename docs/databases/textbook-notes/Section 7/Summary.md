# Section 7 Summary: Transaction Management

This section explores how database systems provide the illusion of a flawless, single-user system even when thousands of users are reading and writing simultaneously, and hardware failures are a constant threat.

## Key Themes

### 1. The Transaction Concept (Chapter 17)
A transaction is a logical unit of work that must be treated as a single, indivisible entity.
- **The ACID Properties**: The foundation of transaction management. 
  - **Atomicity** (all-or-nothing) and **Durability** (survives crashes) protect against failures. 
  - **Isolation** (appears sequential) protects against concurrent interference. 
  - **Consistency** ensures business rules are maintained.
- **Schedules and Serializability**: A concurrent execution of operations (a schedule) is only deemed correct if its outcome is equivalent to executing the transactions one after the other (Conflict Serializability).

### 2. Concurrency Control (Chapter 18)
The database must proactively prevent or resolve conflicts between concurrent transactions.
- **Locking (Pessimistic)**: The **Two-Phase Locking (2PL)** protocol ensures serializability by forcing transactions to acquire all locks (Growing phase) before releasing any (Shrinking phase). Strict 2PL holds write locks until commit to prevent cascading rollbacks.
- **Deadlocks**: Locking creates the risk of deadlocks, requiring the database to detect cycles in a Wait-For Graph and abort victim transactions.
- **Snapshot Isolation (Optimistic)**: A multiversion scheme where each transaction reads a snapshot of the database. Writers don't block readers, and readers don't block writers, making it vastly superior for performance, though it introduces minor anomalies like Write Skew.

### 3. Recovery System (Chapter 19)
The database must be able to recover from crashes without losing committed data or keeping partial data from aborted transactions.
- **Write-Ahead Logging (WAL)**: The central rule of database recovery. The *intent* to modify data (the log record) must be safely written to stable storage *before* the actual data block is overwritten on disk.
- **Steal / No-Force**: Modern databases use memory buffers efficiently. They "steal" memory by writing uncommitted data to disk (handled by UNDO logs) and don't "force" committed data to disk immediately (handled by REDO logs).
- **ARIES Algorithm**: The industry-standard recovery algorithm that uses three passes: Analysis, Redo (Repeating History to restore RAM state), and Undo (rolling back uncommitted transactions using Compensation Log Records).

## Core Concepts to Remember
- **Concurrency vs. Consistency**: The strict mathematical guarantee of Serializability is often traded off for performance in the real world using weaker isolation levels like *Read Committed* or *Snapshot Isolation*.
- **The Log is the Database**: In modern systems, the transaction log *is* the definitive truth of the database. The physical data files are essentially just a cached materialization of the log.

## Transition to Next Section
Having covered how a single, centralized database engine stores data, optimizes queries, and manages concurrent transactions safely, the next section (Parallel and Distributed Databases) explores how to scale these concepts across multiple machines, data centers, and geographic regions.