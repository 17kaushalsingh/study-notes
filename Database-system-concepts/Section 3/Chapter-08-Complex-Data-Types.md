# Chapter 8: Complex Data Types

## 1. Semi-structured Data

Relational models require strict schemas. However, many applications need flexibility for data whose schema changes often.

### JSON (JavaScript Object Notation)
- **Format**: Lightweight, text-based, and language-independent.
- **Structure**: Uses key-value pairs and arrays.
- **SQL Integration**: Modern databases allow storing JSON in a specialized data type and provide functions to query internal fields (e.g., `json_extract` or `->` operator).

### XML (Extensible Markup Language)
- **Format**: Uses tags to define structure (similar to HTML but for data).
- **Flexibility**: Allows hierarchical nesting and self-documenting data.
- **Storage**: Can be stored as a string or a native `XML` type.

---

## 2. Object-Based Data

Object-relational databases extend the relational model with object-oriented features.

- **User-Defined Types**: `create type Person as (name varchar(20), address varchar(20));`
- **Type Inheritance**: Subtypes can inherit attributes from supertypes.
- **Table Inheritance**: Subtables can inherit the schema of a supertable.
- **Object-Relational Mapping (ORM)**: Tools (like Hibernate for Java or Django for Python) that automatically map database tables to objects in a programming language.

---

## 3. Textual Data and Information Retrieval

Handling unstructured text requires specialized techniques.

- **Keyword Queries**: Finding documents containing specific words.
- **Relevance Ranking**:
  - **TF-IDF**: 
    - **TF (Term Frequency)**: How often a word appears in a document.
    - **IDF (Inverse Document Frequency)**: How rare a word is across all documents.
  - **PageRank**: Used by search engines to rank pages based on the number and quality of links pointing to them.

---

## 4. Spatial Data

Used for Geographic Information Systems (GIS) and CAD systems.

- **Geometric Data**: Points, lines, polygons, and circles in 2D or 3D space.
- **Geographic Data**: Data associated with locations on Earth (latitude and longitude).
- **Spatial Queries**:
  - **Region Queries**: Find objects within a specific area.
  - **Nearness Queries**: Find the closest object to a point.
  - **Spatial Joins**: Join two tables based on spatial proximity.

---

## Must-Know Things
- **JSON vs. Relational**: Use JSON when the schema is fluid or sparse; use relational for structured, high-integrity data.
- **ORM Overhead**: While ORMs simplify development, they can introduce performance overhead for complex queries.
- **Spatial Indexing**: Standard B-trees aren't efficient for spatial data; specialized indices like **R-trees** or **Quadtrees** are used.

---

## Quick Revision
- **JSON/XML**: For semi-structured data exchange.
- **ORM**: Bridges Object-Oriented code and Relational data.
- **TF-IDF**: Standard metric for text search relevance.
- **Spatial Data**: Deals with geometry and geography; requires specialized "region" or "nearness" queries.
