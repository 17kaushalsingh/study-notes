# Study Notes Directory

This folder consolidated all of my raw Markdown study notes.

---

## 📂 Directory Structures

The directory tree inside `/notes/` sits side-by-side without deep nesting:

- `databases/`: Database system lectures, indexing designs, and textbooks study notes.
  - `Databases@CMU/`: Lecture notes from the Databases course at Carnegie Mellon University.
  - `textbook-notes/`: Notes from "Database System Concepts" by Silberschatz.
- `dsa-and-cp/`: Algorithm logs, contests solutions, and problem sets.
  - `contests/`: Solutions and editorials for competitive programming contests (e.g. Codeforces, LeetCode).
  - `cp31/`: Notes and exercises from the CP31 course.
  - `cses-problems/`: Solutions to CSES problem set.
- `languages/`: Languages details (modern C++ specifications and JVM garbage collection internals).
  - `cpp/`: Modern C++ features, best practices, and design patterns.
  - `java/`: JVM internals, garbage collection algorithms, and Java language features.
- `systes-and-backend/`:
  - `backend-from-first-principles/`: Complete lecture logs for the Backend First Principles course.
  - `low-level-design/`: Creational, structural, and behavioral design patterns.

---

## 🔄 Commit & Auto-Publish Workflow

With each commit you make to your GitHub repository `17kaushalsingh/study-notes`, your notes are automatically compiled and updated on Vercel:

1. **Write or Edit Notes**: Edit any Markdown file in your editor of choice directly in this folder.
2. **Commit and Push**:
   ```bash
   git add .
   git commit -m "notes: document LEC24 on Distributed consensus"
   git push origin main
   ```
3. **Auto-Update**: Within seconds, Vercel pulls the commit, runs the Astro compiler, registers your new note in the sidebar navigation, parses headings for search-index matching, and publishes the new content!
