# Chapter 17: Transactions

## 1. Transaction Concept

A **transaction** is a unit of program execution that accesses and possibly updates various data items. It is the fundamental concept that allows databases to handle concurrent access and system failures gracefully.

### The ACID Properties
To ensure integrity, transactions must satisfy the following four properties:
- **Atomicity**: "All or nothing." Either all operations of the transaction are reflected properly in the database, or none are. (If an error occurs, the system must *rollback* partial changes).
- **Consistency**: Execution of a transaction in isolation preserves the consistency of the database (e.g., in a bank transfer, money is neither created nor destroyed).
- **Isolation**: Even though multiple transactions execute concurrently, the system guarantees that each transaction is unaware of the others. It appears as if they are executing serially.
- **Durability**: Once a transaction completes successfully (commits), its changes persist, even if there is a subsequent system crash.

---

## 2. Transaction State

During its lifetime, a transaction passes through several states:
- **Active**: The initial state; stays here while executing.
- **Partially Committed**: After the final statement has been executed (but before results are safely written to disk).
- **Failed**: After the discovery that normal execution can no longer proceed.
- **Aborted**: After the transaction has been rolled back and the database restored to its prior state. (The system can then either *restart* or *kill* the transaction).
- **Committed**: After successful completion and recording of the commit to stable storage.

---

## 3. Concurrent Executions and Schedules

Allowing multiple transactions to run concurrently improves throughput and reduces waiting time.

- **Schedule**: The chronological sequence of instructions executed by concurrent transactions.
- **Serial Schedule**: A schedule where transactions are executed one after the other, with no interleaving of operations.

### Serializability
Since concurrent execution intertwines operations, we must ensure the result is correct. A schedule is **serializable** if its effect is equivalent to that of some serial schedule.

- **Conflict**: Two operations conflict if they belong to different transactions, access the same data item, and at least one of them is a `write` operation.
- **Conflict Serializability**: A schedule is conflict serializable if it can be transformed into a serial schedule by swapping non-conflicting operations.

### Testing for Serializability
We test for conflict serializability using a **Precedence Graph**:
- Create a node for each transaction.
- Draw an edge from $T_i$ to $T_j$ if $T_i$ executes a conflicting operation before $T_j$.
- **Rule**: If the precedence graph has a **cycle**, the schedule is *not* conflict serializable.

---

## 4. Recoverability

Even if a schedule is serializable, failures can cause problems if transactions depend on each other.

- **Recoverable Schedule**: If $T_j$ reads data written by $T_i$, then $T_i$ must commit *before* $T_j$ commits. (Otherwise, if $T_i$ aborts, $T_j$ would have read invalid data but already committed!).
- **Cascading Rollback**: A single transaction failure leads to a series of transaction rollbacks (because they read uncommitted data).
- **Cascadeless Schedule**: A schedule where transactions are only allowed to read data written by *committed* transactions. Every cascadeless schedule is also recoverable.

---

## 5. Transaction Isolation Levels

Strict serializability can cause performance bottlenecks. SQL allows weaker isolation levels:
1. **Serializable**: Guarantees serializable execution.
2. **Repeatable Read**: Only committed data is read, and a data item cannot be updated by another transaction between two reads by the same transaction.
3. **Read Committed**: Only committed data is read, but a value could change between two reads within the same transaction.
4. **Read Uncommitted**: The lowest level. Allows reading uncommitted data (dirty reads).

---

## Must-Know Things
- **Atomicity vs Durability**: Atomicity ensures incomplete transactions are completely undone; Durability ensures complete transactions are never undone. Both are handled by the Recovery System.
- **Isolation vs Consistency**: Isolation is handled by the Concurrency-Control System (via locking). Consistency is the responsibility of the application programmer and integrity constraints.
- **The Phantom Phenomenon**: Occurs when a transaction reads a set of rows satisfying a condition (e.g., `WHERE salary > 90000`), and a concurrent transaction inserts a new row that satisfies that condition.

---

## Quick Revision
- **ACID**: Atomicity, Consistency, Isolation, Durability.
- **States**: Active $	o$ Partially Committed $	o$ Committed (or Failed $	o$ Aborted).
- **Conflicts**: Read-Write, Write-Read, Write-Write.
- **Precedence Graph**: No cycles = Conflict Serializable.
- **Cascadelessness**: Read only committed data to avoid a domino effect of rollbacks.
