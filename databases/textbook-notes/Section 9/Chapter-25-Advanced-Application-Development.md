# Chapter 25: Advanced Application Development

This chapter focuses on the practical aspects of building high-performance database applications, including tuning, benchmarking, and adherence to industry standards.

## Detailed Notes

### 1. Performance Tuning
The process of identifying and removing bottlenecks to improve system performance.
- **Tuning Levels**:
    - **Physical Schema**: Adding/removing indices, using materialized views, and horizontal partitioning.
    - **Logical Schema**: Normalization vs. Denormalization (to avoid joins).
    - **Queries**: Rewriting subqueries as joins (decorrelation), using batch updates, and optimizing bulk loads.
    - **Hardware**: Upgrading RAM (Five-minute rule), switching from HDD to SSD (Flash-as-buffer), or using RAID.
- **Queueing Theory**: System performance is often limited by resources becoming bottlenecks. Utilizations above 70% lead to exponential increases in wait times.
- **Tuning Wizards**: Automated tools provided by DBMS (e.g., Microsoft SQL Server Tuning Advisor) that analyze workloads and suggest schema changes.

### 2. Performance Benchmarks
Standardized tests to compare different database systems or configurations.
- **Throughput vs. Response Time**: Measured in Transactions Per Second (TPS).
- **TPC Benchmarks**:
    - **TPC-C**: Simulates a wholesale supplier (OLTP workload).
    - **TPC-H**: Ad hoc decision support (OLAP workload).
    - **TPC-DS**: Advanced decision support with complex data maintenance.
- **Harmonic Mean**: The correct way to average throughput across different types of transactions.

### 3. Testing and Migration
- **Regression Testing**: Ensuring that changes haven't introduced bugs in existing functionality.
- **Database Mutation**: Creating "mutant" versions of data to see if test suites catch subtle query errors.
- **Legacy Migration**:
    - **Big-bang Approach**: Moving everything at once (high risk).
    - **Chicken-little Approach**: Incremental migration (lower risk).
    - **Reverse Engineering**: Extracting E-R models from legacy code.

### 4. Standards
- **SQL Standards**: Evolution from SQL-86 to SQL:2016 (JSON support, temporal tables).
- **Connectivity**: **ODBC** (C-based) and **JDBC** (Java-based) provide vendor-neutral APIs.
- **Object Databases**: Standards like ODMG (now defunct) and JDO for mapping objects to data.
- **LDAP**: Lightweight Directory Access Protocol for storing and querying hierarchical user information (e.g., `cn=silberschatz, ou=CS, o=Yale, c=USA`).

## Must-Know Things
- **Materialized View Maintenance**: Choosing between **Immediate** (consistent but slow writes) and **Deferred** (faster writes but stale reads) maintenance.
- **Five-Minute Rule**: Data accessed more frequently than every five minutes should reside in memory rather than on disk (modern values may vary based on SSD/RAM prices).
- **OLTP vs. OLAP**: Online Transaction Processing (high concurrency, small writes) vs. Online Analytical Processing (complex read-only queries).
- **TPC-E**: A modern OLTP benchmark simulating a brokerage firm, replacing the older TPC-C.

## Quick Revision
- **Tuning**: Start with schema/queries before buying more hardware.
- **Benchmarks**: TPC provides industry-standard metrics for comparison.
- **Connectivity**: JDBC/ODBC abstract the underlying database protocol.
- **Directory Systems**: LDAP is the standard for hierarchical, read-heavy data like employee directories.