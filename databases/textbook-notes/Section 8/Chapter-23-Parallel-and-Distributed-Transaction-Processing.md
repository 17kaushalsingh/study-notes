# Chapter 23: Parallel and Distributed Transaction Processing

## 1. Distributed Transactions Overview

A distributed database system consists of a collection of nodes.
- **Local Transactions**: Access data at only one node.
- **Global Transactions**: Access and update data across multiple nodes. The failure of one node or network link can cause erroneous computations.
- **Transaction Coordinator**: A subsystem responsible for starting the transaction, breaking it into sub-transactions, distributing them to nodes, and coordinating the final commit/abort.

## 2. Commit Protocols

To ensure atomicity across multiple nodes, all nodes must agree on the final outcome. A transaction must commit at *all* nodes or abort at *all* nodes.

### Two-Phase Commit (2PC)
The most widely used protocol, driven by a Coordinator.
- **Phase 1 (Prepare)**: Coordinator asks all participants to prepare to commit.
  - Participants check if they *can* commit. If yes, they write a `<ready T>` log record, force it to stable storage, and reply `ready T`.
  - If no, they reply `abort T`.
- **Phase 2 (Commit/Abort)**: 
  - If the coordinator receives `ready` from *all* participants, it decides to commit, logs `<commit T>`, and sends `commit T` to all nodes.
  - If even one replies `abort` (or times out), it logs `<abort T>` and sends `abort T` to all nodes.

**The Blocking Problem**: If a participant votes `ready`, it locks its resources. If the coordinator crashes before sending the Phase 2 message, the participant is *blocked*. It cannot commit, nor can it abort, until the coordinator recovers.

### Alternative: Persistent Messaging
To avoid the blocking problem of 2PC, systems can use persistent messaging.
- Transactions are broken into local transactions. A node executes its local part and commits, sending a **persistent message** (guaranteed exactly-once delivery) to the next node to trigger the next part.
- Loss of atomicity: If the next node's logic fails, the first node has already committed. Programmers must write application-specific **Compensating Transactions** to reverse the effects of the earlier commit.

## 3. Concurrency Control in Distributed Databases

- **Distributed Lock Manager**: Each node manages locks for its own data. Avoids the bottleneck of a single centralized lock manager.
- **Global Deadlocks**: $T_1$ waits for $T_2$ at Node A, and $T_2$ waits for $T_1$ at Node B. Local wait-for graphs show no cycles, but the global union does. Requires a centralized coordinator to periodically build a global wait-for graph.
- **Distributed Timestamps**: Timestamps must be globally unique. Created by concatenating the local timestamp (or clock) with the node ID. Clocks must be kept tightly synchronized (e.g., via NTP).

## 4. Replication and Failures

Replication provides high availability. If a node goes down, data can be read from a replica.

### Dealing with Failures
- **Read-One, Write-All**: Fast reads, but if a single node with a replica goes down, writes are blocked.
- **Quorum Consensus**: Read quorum $Q_r$, Write quorum $Q_w$. Can survive node failures as long as the quorum is met.

### CAP Theorem
A distributed data store can provide at most two of the following three guarantees:
1. **Consistency**: Every read receives the most recent write or an error.
2. **Availability**: Every request receives a (non-error) response, without guarantee that it contains the most recent write.
3. **Partition Tolerance**: The system continues to operate despite arbitrary network partitions dropping messages.

Since partitions (P) are unavoidable in large networks, systems must choose between Consistency (CP) and Availability (AP).
- **BASE Properties**: Used in AP systems (e.g., NoSQL). Basically Available, Soft state, Eventually consistent.
- **Resolving Conflicts**: If partitions allow divergent writes, vector clocks/version vectors or Merkle trees are used to detect inconsistencies, requiring reconciliation (e.g., merging shopping carts) later.

## 5. Distributed Consensus

To solve the 2PC blocking problem, or to elect new coordinators safely, systems use distributed consensus protocols.

### Problem
A set of nodes must agree on a single value (e.g., who is the leader, or whether to commit). They must all learn the same value, and the protocol must not block as long as a *majority* of nodes are alive.

### Paxos and Raft
- **Raft**: Designed to be easier to understand than Paxos. Uses a Leader-Follower model. 
  - Time is divided into **Terms**.
  - **Leader Election**: If followers don't hear a heartbeat, they trigger an election (using randomized timeouts to avoid split votes). The candidate with the majority of votes becomes the leader.
  - **Log Replication**: All writes go to the Leader, who appends them to its log and sends an `AppendEntries` RPC to followers. Once a majority of followers acknowledge, the entry is **committed**.
- **Replicated State Machines**: If all nodes start in the same state, are deterministic, and receive the exact same log of commands (guaranteed by Raft/Paxos), they will end up in the exact same state. This is how highly-available, fault-tolerant coordination services (like Apache ZooKeeper or Google Chubby) are built.

## Must-Know Things
- **Two-Phase Commit (2PC)**: Phase 1 is voting. Phase 2 is the decision. It is a *blocking* protocol.
- **CAP Theorem**: You cannot have perfect consistency and 100% uptime when the network breaks. You must choose one.
- **Raft / Paxos**: Consensus algorithms that rely on **majority (quorum) voting**. They guarantee progress as long as $(N/2) + 1$ nodes are alive.

## Quick Revision
- **2PC**: Coordinator asks, nodes prepare/vote, coordinator commands commit/abort.
- **Global Deadlock**: Requires union of local wait-for graphs to detect.
- **CAP Theorem**: Consistency, Availability, Partition Tolerance (pick 2, usually AP or CP).
- **Raft**: Leader election via timeouts, Log replication via majority acknowledgment.
