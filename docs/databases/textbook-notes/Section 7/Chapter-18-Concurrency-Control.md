# Chapter 18: Concurrency Control

## 1. Lock-Based Protocols

To ensure isolation, databases control concurrent access to data items using locks.
- **Shared Lock (S)**: A transaction can *read* but not write the item. Multiple transactions can hold S-locks on the same item.
- **Exclusive Lock (X)**: A transaction can both *read* and *write* the item. Only one transaction can hold an X-lock at a time.

A transaction must request the appropriate lock from the **concurrency-control manager** before accessing a data item.

### The Two-Phase Locking Protocol (2PL)
This is the most common protocol to ensure conflict serializability. A transaction must issue lock and unlock requests in two phases:
1. **Growing Phase**: The transaction may obtain locks, but may not release any lock.
2. **Shrinking Phase**: The transaction may release locks, but may not obtain any new locks.

- **Lock Point**: The point where the transaction has obtained its final lock (end of the growing phase).
- **Strict 2PL**: Avoids cascading rollbacks by requiring that all *exclusive* (X) locks be held until the transaction commits or aborts.
- **Rigorous 2PL**: Requires that *all* locks (S and X) be held until commit/abort.

### Deadlocks
Locking introduces the risk of **Deadlock**, where two or more transactions are waiting for each other to release locks (e.g., $T_1$ holds A, wants B; $T_2$ holds B, wants A).
- Handled via **Deadlock Prevention** (e.g., ordering lock requests, or using `wait-die` / `wound-wait` timestamp schemes) or **Deadlock Detection and Recovery** (building a *wait-for graph* and checking for cycles; if a cycle exists, aborting a "victim" transaction).

---

## 2. Multiple Granularity Locking

Instead of locking individual tuples, transactions can lock larger granules (e.g., blocks, files, or entire tables) to reduce lock overhead. This is represented as a tree of data granularities.

### Intention Locks
To determine if a node (like a table) can be locked, the system checks if any descendants (like tuples) are locked. To do this efficiently without scanning the whole tree, **Intention Locks** are used:
- **IS (Intention-Shared)**: Explicit locking is being done at a lower level with S-locks.
- **IX (Intention-Exclusive)**: Explicit locking is being done at a lower level with X-locks or S-locks.
- **SIX (Shared and Intention-Exclusive)**: The current node is locked in S-mode, but lower levels are being locked in X-mode.

Locks are acquired top-down (root to leaf) and released bottom-up (leaf to root).

---

## 3. Timestamp-Based Protocols

Instead of using locks to order transactions at runtime, timestamp protocols decide the serialization order *in advance*. Each transaction $T_i$ is assigned a unique timestamp $TS(T_i)$ when it begins.

Each data item $Q$ maintains two timestamps:
- **W-timestamp(Q)**: Largest timestamp of any transaction that successfully wrote $Q$.
- **R-timestamp(Q)**: Largest timestamp of any transaction that successfully read $Q$.

### The Protocol Rules
- If $T_i$ wants to **read(Q)**:
  - If $TS(T_i) < 	ext{W-timestamp}(Q)$, the value has been overwritten by a "newer" transaction. $T_i$ must abort.
  - Otherwise, read succeeds. Update R-timestamp.
- If $T_i$ wants to **write(Q)**:
  - If $TS(T_i) < 	ext{R-timestamp}(Q)$, a "newer" transaction already read the old value. $T_i$ must abort.
  - If $TS(T_i) < 	ext{W-timestamp}(Q)$, a "newer" transaction already wrote a value. $T_i$ aborts (unless using **Thomas' Write Rule**, which simply ignores this obsolete write without aborting).
  - Otherwise, write succeeds. Update W-timestamp.

---

## 4. Validation-Based Protocols (Optimistic Concurrency Control)

Assumes conflicts are rare. Transactions execute without locking, keeping updates in a private local workspace.
1. **Read Phase**: Read data and perform computations in local variables.
2. **Validation Phase**: Check if the local writes will violate serializability with other concurrent transactions.
3. **Write Phase**: If validation passes, copy local variables to the actual database. If it fails, abort.

---

## 5. Multiversion Schemes and Snapshot Isolation

Instead of overwriting data, **Multiversion** schemes create a new version of the data item for each write. Reads never wait; they simply read the appropriate older version of the data.

### Snapshot Isolation
Widely used in practice (e.g., PostgreSQL, Oracle).
- When a transaction begins, it gets a "snapshot" of the database (all committed data up to that point).
- It performs all reads and writes against this snapshot.
- When committing, it checks if any concurrent transaction has updated the same data. If so, it aborts (**First-Updater-Wins** or **First-Committer-Wins**).
- **Benefit**: Readers *never* block writers, and writers *never* block readers. Excellent for workloads mixing heavy analytics with short updates.
- **Drawback**: Can suffer from **Write Skew** anomalies, meaning it doesn't guarantee strict serializability without additional mechanisms (like Serializable Snapshot Isolation - SSI).

---

## Must-Know Things
- **Strict 2PL vs Basic 2PL**: Basic 2PL guarantees serializability but allows cascading rollbacks. Strict 2PL holds X-locks until commit to guarantee cascadelessness.
- **Wait-For Graph**: A directed graph where $T_i 	o T_j$ means $T_i$ is waiting for a lock held by $T_j$. A cycle indicates a deadlock.
- **Thomas' Write Rule**: An optimization for timestamps that ignores obsolete writes instead of aborting the transaction.

---

## Quick Revision
- **2PL**: Growing phase (acquire), Shrinking phase (release). Ensures serializability.
- **Multiple Granularity**: IS/IX locks at table level before S/X locks at row level.
- **Deadlock**: Cycle of waiting. Fixed by timeouts, prevention (wait-die), or detection (wait-for graph).
- **Snapshot Isolation**: Give each Txn a copy. Fast reads, no read/write blocking, but watch out for Write Skew.