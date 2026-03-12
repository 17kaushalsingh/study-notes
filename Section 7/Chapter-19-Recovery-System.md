# Chapter 19: Recovery System

## 1. Failure Classification

Database systems must survive various types of failures:
- **Transaction Failure**: Logical errors (bad input, constraints violated) or System errors (deadlock).
- **System Crash**: Power failures, OS crashes, or hardware malfunctions that wipe out *volatile* storage (Main Memory/RAM). Non-volatile storage (Disk) remains intact. (The "fail-stop" assumption).
- **Disk Failure**: A head crash or sector failure destroys data on non-volatile storage. Recovered using RAID or archival backups.

---

## 2. Storage and Data Access

- **Volatile Storage**: Main memory (buffers). Lost on crash.
- **Non-Volatile Storage**: Magnetic disks, SSDs. Survives crashes.
- **Stable Storage**: An idealized form of storage that *never* loses data. Approximated by replicating data across multiple independent non-volatile disks (e.g., RAID 1 mirroring) or remote backups.

### Data Access
Transactions pull blocks from Disk into the **Disk Buffer** in memory, modify them there, and eventually write them back to disk. 
- A buffer block modified by a transaction is called a **dirty block**.
- Outputting a block to disk is called a **force-output**.

---

## 3. Recovery and Atomicity (Log-Based Recovery)

To ensure Atomicity and Durability, the database logs all modifications *before* applying them to the actual database on disk.

### Log Records
The Log is a sequence of records stored on stable storage. Key types:
- `<T_i start>`: Transaction $T_i$ has started.
- `<T_i, X_j, V_1, V_2>`: Update record. $T_i$ modified data item $X_j$ from old value $V_1$ to new value $V_2$.
- `<T_i commit>`: Transaction $T_i$ has successfully finished and is now durable.
- `<T_i abort>`: Transaction $T_i$ has been rolled back.

### The Write-Ahead Logging (WAL) Rule
Crucial for recovery: **Before a block of data in main memory can be output to the database on disk, all log records pertaining to that data must have been output to stable storage.**

### Redo and Undo
When the system recovers from a crash, it analyzes the log:
- **Redo**: If the log contains `<T_i start>` AND (`<T_i commit>` OR `<T_i abort>`), the system re-applies the new values ($V_2$) to ensure all committed changes are physically on disk (and to repeat history even for aborted transactions before undoing them).
- **Undo**: If the log contains `<T_i start>` but NO `<T_i commit>` or `<T_i abort>`, the transaction was incomplete at the time of the crash. The system works backwards through the log, applying the old values ($V_1$) to roll back the transaction.

---

## 4. Checkpoints

Scanning the entire log from the beginning of time after a crash is too slow. The system periodically creates a **Checkpoint**.

A basic checkpoint involves:
1. Outputting all log records currently in memory to stable storage.
2. Outputting all modified (dirty) buffer blocks to disk.
3. Writing a `<checkpoint L>` record to the log, where $L$ is a list of currently active transactions.

**During Recovery**: The system only needs to scan the log backwards until it finds the last `<checkpoint L>` record. It only needs to undo/redo transactions in $L$ and those that started *after* the checkpoint.

---

## 5. Buffer Management Policies

- **Force vs. No-Force**: 
  - *Force*: Requires all modified blocks of a transaction to be written to disk before it can commit (slow).
  - *No-Force*: Allows a transaction to commit even if its modified blocks are still only in the memory buffer (fast, relies on Redo logs for durability). Used by almost all databases.
- **Steal vs. No-Steal**:
  - *No-Steal*: A modified block cannot be written to disk until the transaction commits (limits buffer size).
  - *Steal*: The buffer manager can write an uncommitted transaction's dirty block to disk to make room in the buffer (relies on Undo logs to rollback if the transaction later aborts). Used by almost all databases.

---

## 6. ARIES Recovery Algorithm

ARIES (Algorithm for Recovery and Isolation Exploiting Semantics) is a state-of-the-art, industry-standard recovery algorithm.

### Key Features
- **Log Sequence Number (LSN)**: Every log record has a unique, monotonically increasing LSN. Every database page stores the `PageLSN` of the last operation that modified it.
- **Physiological Redo**: Redo is physical (to a specific page) but logical within the page (e.g., "insert record" rather than "shift bytes 50-100").
- **Compensation Log Records (CLRs)**: When an operation is undone during rollback, a CLR is written to the log. If the system crashes *during* recovery, it doesn't undo the undo; it reads the CLR and knows exactly where to resume.

### The Three Passes of ARIES
1. **Analysis Pass**: Scans forward from the last checkpoint. Identifies dirty pages and transactions that were active at the time of the crash.
2. **Redo Pass**: Starts from the oldest unwritten dirty page (`RedoLSN`). Scans forward, repeating history by applying all updates (even for aborted transactions) to bring the database exactly to the state it was in at the moment of the crash.
3. **Undo Pass**: Scans backwards. Rolls back all transactions that were incomplete at the time of the crash, writing CLRs along the way.

---

## Must-Know Things
- **Write-Ahead Logging (WAL)**: The single most important rule in database recovery. Logs must hit stable storage before the data blocks do.
- **Steal/No-Force**: The standard buffer management policy. "Steal" means uncommitted data can be written to disk (requires Undo). "No-Force" means committed data might only be in RAM (requires Redo).
- **Repeating History**: ARIES redoes *everything* first (including actions of transactions that eventually aborted), returning the RAM state to the exact moment of the crash, and only then performs Undo.

---

## Quick Revision
- **Log Record**: `<Transaction, DataItem, OldValue, NewValue>`.
- **Undo**: Restores OldValue (for uncommitted Txns).
- **Redo**: Restores NewValue (for committed Txns).
- **Checkpoint**: A marker to limit how far back the recovery system has to read the log.
- **ARIES**: 3 phases: Analysis $	o$ Redo (Repeat History) $	o$ Undo. Uses LSNs and CLRs.
