# Chapter 26: Blockchain Databases

Blockchain technology provides a decentralized, tamper-resistant alternative to traditional centralized databases for managing digital ledgers and transaction logs.

## Detailed Notes

### 1. Blockchain Fundamentals
- **Definition**: A linked list of data blocks where each block contains a cryptographic hash of the previous block.
- **Genesis Block**: The first block in the chain (Block 0).
- **Tamper Resistance**: If any data in block $i$ is changed, its hash changes, breaking the link to block $i+1$ and all subsequent blocks.
- **Properties**:
    - **Decentralization**: No single entity controls the ledger.
    - **Immutability**: Once committed, records are extremely difficult to change.
    - **Transparency**: The entire history is visible to participants.

### 2. Cryptographic Tools
- **Hash Functions**: Must be **collision-resistant** (hard to find two inputs with same hash) and **irreversible** (hard to find input from hash). SHA-256 is the standard.
- **Digital Signatures**: Use public-key cryptography to ensure **irrefutability** (users cannot deny they signed a transaction).
- **Merkle Trees**: A tree of hashes used to efficiently verify that a specific transaction exists in a block without downloading the entire block.

### 3. Consensus Mechanisms
The protocol by which nodes agree on which block to add next.
- **Proof of Work (PoW)**: Nodes (miners) solve a computationally expensive puzzle (finding a "nonce"). High security but high energy consumption.
- **Proof of Stake (PoS)**: Nodes are chosen to validate blocks based on the amount of currency they "stake" (hold). More energy-efficient.
- **Byzantine Consensus**: Message-based protocols (like Practical Byzantine Fault Tolerance - PBFT) designed to handle nodes that are not just failing, but acting maliciously.

### 4. Public vs. Permissioned Blockchains
- **Public (Permissionless)**: Anyone can join, read, and write (e.g., Bitcoin, Ethereum). Uses PoW/PoS.
- **Permissioned**: Access is restricted to authorized participants (e.g., Hyperledger Fabric). Often used in enterprise settings. Uses Byzantine consensus.

### 5. Smart Contracts
Executable code stored on the blockchain.
- **Mechanism**: The contract runs automatically when specific conditions are met.
- **Deterministic**: Every node must reach the same result when executing the code.
- **Ethereum**: The first major platform to popularize Turing-complete smart contracts using the **Ethereum Virtual Machine (EVM)** and **Gas** (payment for computation).

### 6. Data Management in Blockchain
- **State**: Blockchains often maintain a "world state" (e.g., account balances).
- **Merkle-Patricia Trees**: A specialized data structure used in Ethereum to store state efficiently and allow for fast proofs.
- **Sharding**: Partitioning the blockchain state across multiple groups of nodes to improve throughput.

## Must-Know Things
- **Forking**: Occurs when two nodes add different blocks at the same height. Resolved by the **longest chain rule**.
- **Double-Spending**: The risk that a user spends the same digital token twice. Blockchains prevent this through consensus and verification.
- **Sybil Attack**: An attacker creating many fake identities to gain majority control. Mitigated by PoW/PoS.
- **Oracle**: A trusted external service that provides real-world data (e.g., weather, stock prices) to smart contracts.

## Quick Revision
- **Chain of Hashes**: Provides the core security and tamper-evidence.
- **Consensus**: PoW (mining) vs. PoS (staking) vs. PBFT (voting).
- **Smart Contracts**: Distributed programs that automate business logic.
- **Permissioned Chains**: Better suited for enterprise due to higher throughput and privacy.