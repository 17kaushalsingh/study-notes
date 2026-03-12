# Chapter 11: Data Analytics

## 1. Overview of Analytics

Data analytics is the process of inspecting and modeling data to discover useful information and support decision-making.

- **Business Intelligence (BI)**: Tools and practices for collecting, integrating, and analyzing business data.
- **Decision Support Systems**: Interactive systems that help users identify and solve problems and make decisions.

---

## 2. Data Warehousing

A data warehouse is a repository of information gathered from multiple sources, stored under a unified schema, at a single site.

### Components
- **Source-driven Architecture**: Data sources transmit new information to the warehouse.
- **ETL Process (Extract, Transform, Load)**:
  - **Extract**: Get data from multiple sources.
  - **Transform**: Cleanse, reformat, and integrate data.
  - **Load**: Put data into the warehouse.

### Warehouse Schemas
- **Fact Table**: The central table containing the measures (e.g., `sales_amount`).
- **Dimension Tables**: Tables that describe the facts (e.g., `item`, `time`, `location`).
- **Star Schema**: A fact table surrounded by multiple dimension tables.
- **Snowflake Schema**: An extension of the star schema where dimension tables are normalized into further tables.

---

## 3. Online Analytical Processing (OLAP)

OLAP tools allow users to interactively analyze multidimensional data from different perspectives.

- **Data Cube**: A multidimensional representation of data (e.g., Sales by Item, Store, and Time).
- **OLAP Operations**:
  - **Rollup**: Moving from finer to coarser granularity (e.g., Day $	o$ Month).
  - **Drill-down**: Moving from coarser to finer granularity (e.g., Month $	o$ Day).
  - **Slicing**: Selecting a single dimension (e.g., "Sales for the year 2023").
  - **Dicing**: Selecting a sub-cube (e.g., "Sales for certain items in certain regions during certain months").
  - **Pivot**: Rotating the data axes to view it from different angles.

---

## 4. Data Mining

Data mining is the automated search for non-trivial patterns in large datasets.

### Core Tasks
- **Classification**: Predicting which category a data item belongs to.
  - **Decision Trees**: A tree-like model of decisions.
  - **Bayesian Classifiers**: Based on probability theory (Bayes' Theorem).
- **Regression**: Predicting a numeric value (e.g., predicting house prices based on size).
- **Association Rules**: Finding items that often occur together (e.g., "Customers who buy bread also buy butter").
- **Clustering**: Grouping similar items together without prior categories.

---

## Must-Know Things
- **Column-Oriented Storage**: Many data warehouses use column-oriented storage instead of row-oriented because it speeds up aggregations (like `sum` or `avg`).
- **Data Lake**: Unlike a warehouse, a data lake stores data in its raw format until it is needed.
- **Precision vs. Recall**: Key metrics for evaluating retrieval and mining effectiveness.

---

## Quick Revision
- **Data Warehouse**: Consolidated data for analysis.
- **ETL**: Extract $	o$ Transform $	o$ Load.
- **Star Schema**: Central facts, peripheral dimensions.
- **OLAP**: Slicing, Dicing, Rollup, Drill-down.
- **Data Mining**: Classification, Regression, Clustering, Association.
