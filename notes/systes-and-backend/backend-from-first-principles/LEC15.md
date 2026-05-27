# Full Text Search & Elasticsearch — Lecture Notes

---

## 1. The Problem with Traditional Database Search

### The LIKE Query Approach

```sql
SELECT * FROM products
WHERE name ILIKE '%laptop%'
   OR description ILIKE '%laptop%';
```

This works fine at small scale. At millions of rows, it breaks.

### Why It Fails

**Performance:** The database performs a **full sequential scan** — it reads every row, examines every text field, and does character-by-character pattern matching. No index can help with a leading wildcard (`%laptop%`).

| Scale | Latency |
|-------|---------|
| 5,000 products | ~50ms |
| Millions of products | 30+ seconds |

**No relevance:** Results come back in arbitrary order. A product where "laptop" appears once in a buried description ranks the same as a MacBook Pro with "laptop" in the title, name, and specs.

**No typo tolerance:** `labtop` → zero results. The query is exact character matching only.

### The Librarian Analogy

Think of a relational database as a librarian with a fatal flaw: to find books about "machine learning," they go shelf by shelf, book by book, checking every title and every page. Thorough, but painfully slow. And once they find matches, they have no way to rank which result is more relevant — they return them in whatever order they were found.

---

## 2. The Solution: Inverted Index

### The Key Insight

Instead of searching through documents to find terms, **flip the problem**: while storing documents, extract all terms and build an index that maps terms → documents.

### How it Works

At storage time (not query time), every word in every document is extracted and indexed:

```
Term: "machine"
  → Introduction to Machine Learning  (pages 1, 15, 23)
  → The Machine Age                   (pages 5, 89)
  → Coffee Machine Manual             (page 1)

Term: "learning"
  → Introduction to Machine Learning  (pages 1, 16, 24)
  → Learning to Cook                  (pages 3, 45)
  → Deep Learning Fundamentals        (pages 2, 8, 31)
```

At query time for "machine learning":
1. Look up "machine" in the index → get document list instantly
2. Look up "learning" in the index → get document list instantly
3. Find intersection / union → score by relevance
4. Return ranked results

**No scanning. No waiting. Just lookups.**

> This technique — **inverted index** — is the core idea that powers Elasticsearch, Apache Lucene, and even PostgreSQL's built-in full text search.

---

## 3. Elasticsearch

### Background

Elasticsearch is built on top of **Apache Lucene** — the open-source library that implements the inverted index and relevance scoring. Lucene is the engine; Elasticsearch wraps it with a distributed, scalable, REST API-based interface.

Other tools (including PostgreSQL's `tsvector`/`tsquery` full text search) also use similar inverted index concepts.

### Data Model

In Elasticsearch, the equivalent of a database row is called a **document** (JSON format, similar to MongoDB). A collection of documents is called an **index**.

```json
{
  "id": "1",
  "review": "Great MacBook Pro, perfect for development",
  "sentiment": "positive"
}
```

Field types matter:
- `text` — analyzed; broken into terms, good for full text search
- `keyword` — not analyzed; exact match only (useful for filtering, e.g., `sentiment = "positive"`)

---

## 4. Relevance Scoring — BM25

Elasticsearch uses the **BM25 algorithm** to score how relevant a document is to a query. You don't need to understand the math — just understand the factors that influence the score:

| Factor | Description | Effect on Score |
|--------|-------------|----------------|
| **Term Frequency** | How often the search term appears in this document | More occurrences → higher score |
| **Document Frequency** | How common the term is across all documents | Rare terms → higher score (more distinctive) |
| **Document Length** | Length of the document | Short doc with many matches → higher score than long doc with few |
| **Field Boosting** | Which field the term appears in | Title > Description > Content (configurable) |

### Field Boosting Example

```
Query: "machine learning"

Book: "Introduction to Machine Learning"
  → Term in title → big relevance boost
  → Term appears many times → additional boost
  → Score: HIGH → ranked #1

Book: "The Machine Age"
  → Term in title → boost
  → Term appears less frequently
  → Score: MEDIUM → ranked #2

Book: "Some Random Book" (mentions "machine" once on last page)
  → Term not in title, mentioned once
  → Score: LOW → ranked #3 or filtered out
```

---

## 5. Capabilities Beyond Basic Search

### Typo Tolerance (Fuzzy Search)

Elasticsearch can detect and correct typos. Searching for `"treading"` when you meant `"trending"` still returns relevant results. The algorithm uses **edit distance** (Levenshtein distance) to find close matches.

This is how Google, Amazon, and similar platforms handle misspellings gracefully.

### Type-Ahead / Autocomplete

As a user types, Elasticsearch can return partial-match suggestions in real time — the "search-as-you-type" experience on Amazon, YouTube, etc.

### Multi-Field Search

A single query can search across multiple fields with different weights:

```json
{
  "query": {
    "multi_match": {
      "query": "machine learning",
      "fields": ["title^3", "description^2", "content^1"]
    }
  }
}
```

`^3` means "title matches count 3x more than content matches."

---

## 6. PostgreSQL vs Elasticsearch for Search

| | PostgreSQL ILIKE | PostgreSQL Full Text Search | Elasticsearch |
|--|-----------------|--------------------------|---------------|
| **Speed at scale** | Very slow (full scan) | Fast (inverted index) | Very fast (optimized distributed inverted index) |
| **Relevance ranking** | None | Basic | Advanced (BM25, field boosting, custom scoring) |
| **Typo tolerance** | No | Limited | Yes (fuzzy matching) |
| **Setup complexity** | None (already in DB) | Low | Higher (separate service) |
| **When to use** | Never for large datasets | Good for simple search on existing Postgres data | When search is a core feature, or you already use Elasticsearch (ELK stack) |

### Decision Rule

- **Already using PostgreSQL, simple search needs** → use PostgreSQL full text search (`tsvector`, `tsquery`)
- **Company already uses Elasticsearch (ELK stack for logging)** → use Elasticsearch for search too
- **Search is a core product feature requiring relevance + typo tolerance at scale** → Elasticsearch

---

## 7. The ELK Stack

Elasticsearch is commonly used as part of the **ELK Stack**:

| Component | Role |
|-----------|------|
| **E**lasticsearch | Fast search and storage |
| **L**ogstash (or Logstash) | Log ingestion and transformation pipeline |
| **K**ibana | Visualization and dashboards |

If your company uses the ELK stack for log management (very common), Elasticsearch is already in your infrastructure — use it for full text search too.

---

## 8. Demo Results: ILIKE vs Elasticsearch

Setup: 50,000 product reviews in both PostgreSQL (Neon) and Elasticsearch (Elastic Cloud), same US-west region.

| Query | PostgreSQL ILIKE | Elasticsearch |
|-------|-----------------|---------------|
| "laptop" | ~3–4 seconds | ~1 second |
| "only" (~8,000 matches) | ~7.5 seconds | ~500ms |

**Same number of results. Elasticsearch is 6–15x faster on this dataset.** The gap widens as dataset size grows.

---

## 9. Practical Advice

**Elasticsearch is a tool, not a mastery requirement.** Unlike databases (which you must deeply understand — indexes, query plans, transactions, etc.), Elasticsearch is something you can get productive with quickly:

- Know when to use it (search-heavy features, type-ahead, typo tolerance, log search)
- Refer to the official docs or Elasticsearch DSL examples for your specific query type
- Use field boosting and the multi-match query for most search use cases
- For complex scoring, read the BM25 docs — but don't block on theory

The Elasticsearch Query DSL is JSON-based, well-documented, and most common patterns (basic search, fuzzy, type-ahead, filters) are copy-paste ready from docs.

---

## Quick Revision Checklist

- [ ] `ILIKE '%term%'` = full table scan = slow at scale; no relevance, no typo tolerance
- [ ] **Inverted index** = map from terms → documents (flip the search direction); eliminates scanning
- [ ] Elasticsearch is built on Apache Lucene (which implements the inverted index)
- [ ] Elasticsearch document = JSON object; index = collection of documents
- [ ] Field types: `text` (analyzed, full text search) vs `keyword` (exact match, filtering)
- [ ] BM25 scoring factors: term frequency, document frequency, document length, field boosting
- [ ] Field boosting: title match > description match > content match (configurable)
- [ ] Typo tolerance: fuzzy matching via edit distance (Levenshtein)
- [ ] Type-ahead: partial match suggestions as user types
- [ ] PostgreSQL has built-in full text search (`tsvector`/`tsquery`) — good for simple needs
- [ ] Choose Elasticsearch when: search is core feature, or already using ELK stack for logs
- [ ] ELK Stack: Elasticsearch + Logstash + Kibana → common log management + visualization stack
- [ ] Demo: Elasticsearch is 6–15x faster than ILIKE on 50k rows; gap grows with scale
- [ ] Priority: master databases deeply; treat Elasticsearch as a specialized tool (docs-first approach is fine)