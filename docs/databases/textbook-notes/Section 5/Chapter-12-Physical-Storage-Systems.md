# Chapter 12: Physical Storage Systems

## 1. Overview of Physical Storage Media

Storage media are classified by speed, cost, and reliability. They form a hierarchy:
- **Primary Storage (Volatile)**:
  - **Cache**: Fastest and most costly, managed by hardware.
  - **Main Memory**: Fast, but generally too small/expensive for entire large databases. Contents lost on power failure.
- **Secondary / Online Storage (Non-Volatile)**:
  - **Flash Memory (SSD)**: Faster than magnetic disk, lower power, but higher cost per byte.
  - **Magnetic Disk (HDD)**: The primary medium for long-term online storage. High capacity, low cost per byte, but slower mechanical access.
- **Tertiary / Offline Storage (Non-Volatile)**:
  - **Optical Storage**: DVDs, Blu-ray (often WORM - write-once, read-many).
  - **Tape Storage**: Sequential access. Very high capacity, very low cost, used for archival and backup.

---

## 2. Storage Interfaces

- **SATA / SAS**: Common interfaces for magnetic disks and older SSDs.
- **NVMe (Non-Volatile Memory Express)**: Developed specifically for SSDs over the PCIe interface, providing massively higher throughput.
- **Storage Area Networks (SAN)** and **Network Attached Storage (NAS)**: Architectures for connecting large numbers of disks to servers over high-speed networks.

---

## 3. Magnetic Disks

### Physical Characteristics
- **Platters**: Circular disks covered with magnetic material.
- **Tracks and Sectors**: Platters are divided into concentric tracks, which are subdivided into sectors (typically 512 bytes to 4KB).
- **Disk Arm**: Moves the read-write head across tracks.
- **Cylinder**: The set of $i$-th tracks across all platters.

### Performance Measures
- **Access Time**: The time to get data from disk to memory.
  - **Seek Time**: Time to move the arm to the correct track (2–20 ms).
  - **Rotational Latency**: Time waiting for the sector to rotate under the head (depends on RPM, usually 4-11 ms).
- **Data-Transfer Rate**: Rate of data retrieval once the head is positioned.
- **IOPS (I/O Operations Per Second)**: How many random block accesses can be done per second (typically 50–200 for HDD).
- **MTTF (Mean Time To Failure)**: A statistical measure of disk reliability.

---

## 4. Flash Memory (Solid-State Drives - SSD)

Built using NAND flash. Provides the same block-oriented interface as HDDs.

- **Reads**: Very fast (20-100 microseconds latency). High IOPS (10,000 to 350,000+).
- **Writes**: Complex. A page cannot be directly overwritten; the entire **erase block** (containing multiple pages) must be erased first. Erase operations are slow (2-5 ms) and degrade the memory over time.
- **Flash Translation Layer (FTL)**: Software layer inside the SSD that maps logical pages to physical pages. It handles **Wear Leveling** to distribute erase operations evenly, preventing premature failure of specific blocks.

---

## 5. RAID (Redundant Arrays of Independent Disks)

RAID improves performance through parallelism and reliability through redundancy.

### Key RAID Levels
- **RAID 0 (Striping)**: Data is striped across disks (block-level). Improves performance drastically but provides *zero* redundancy. If one disk fails, data is lost.
- **RAID 1 (Mirroring)**: Every block is written to two identical disks. Excellent reliability and read performance, but 100% storage overhead.
- **RAID 5 (Block-Interleaved Distributed Parity)**: Data and parity blocks are striped across $N+1$ disks. Can survive a single disk failure. Better storage efficiency than RAID 1, but random writes are expensive because parity must be read and recomputed.
- **RAID 6 (P + Q Redundancy)**: Stores two types of error-correcting codes. Can survive two simultaneous disk failures.

---

## 6. Disk-Block Access Optimization

Since disk I/O is the bottleneck, databases optimize access:
- **Buffering**: Storing frequently accessed blocks in main memory.
- **Read-Ahead**: Fetching consecutive blocks into memory before they are requested.
- **Disk-Arm Scheduling**: Algorithms like the *elevator algorithm* order disk requests to minimize seek time.
- **File Organization**: Allocating blocks contiguously (extents) to ensure sequential access.

---

## Must-Know Things
- **Sequential vs. Random I/O**: HDDs are terrible at random I/O (due to seek time) but decent at sequential I/O. SSDs excel at random reads.
- **RAID 1 vs. RAID 5**: Use RAID 1 for write-heavy systems (like log files). Use RAID 5 for read-heavy systems with moderate storage budgets.
- **SSD Wear Leveling**: You cannot infinitely rewrite an SSD block; the controller must move data around to ensure the drive ages evenly.

---

## Quick Revision
- **Hierarchy**: Cache $	o$ Main Memory $	o$ SSD $	o$ HDD $	o$ Tape.
- **Disk Latency**: Seek Time + Rotational Latency + Transfer Time.
- **SSD Writes**: Require block erasure; managed by FTL and Wear Leveling.
- **RAID 0**: Speed only.
- **RAID 1**: Mirroring (reliability).
- **RAID 5**: Distributed Parity (balance of cost, speed, reliability).
