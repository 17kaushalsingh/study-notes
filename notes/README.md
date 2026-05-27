# Study Notes Directory

This folder consolidated all of your raw Markdown study notes. It is structured flatly on your disk to ensure easy management, quick backups, and high compatibility with standard markdown editors (such as Obsidian, VS Code, or standard git text editors).

---

## 📂 Directory Structures on Disk

The directory tree inside `/notes/` sits side-by-side without deep nesting:

- `databases/`: Database system lectures, indexing designs, and textbooks study notes.
- `dsa-and-cp/`: Algorithm logs, contests solutions, and problem sets.
- `languages/`: Languages details (modern C++ specifications and JVM garbage collection internals).
- `systes-and-backend/`:
  - `backend-from-first-principles/`: Complete lecture logs for the Backend First Principles course.
  - `low-level-design/`: Creational, structural, and behavioral design patterns.

---

## ✍️ Formatting Guidelines for Writing Notes

To ensure your notes are compiled into gorgeous, premium pages by the Astro Static Site Portal, follow these formatting guidelines:

### 1. File Metadata & Naming
- **Filename**: For active directories like `backend-from-first-principles/`, always use the `LEC*.md` pattern (e.g. `LEC5.md`, `LEC24.md`). This enables correct numerical sorting in the navigation sidebars.
- **Top Header**: Always begin your file with a single H1 header (`# Title`). This header is extracted dynamically as the document's official title:
  ```markdown
  # HTTP Protocol — Lecture Notes
  ```

### 2. Typography & Structural Sections
- Use standard markdown headers (`##`, `###`) to divide your concepts. 
- Astro's layout engine automatically scans these sections and compiles them into a **sticky clientside Index** on the right side of the screen for ultra-fast navigation!
- **Callout Blocks**: Wrap critical definitions or tips in markdown blockquotes (`>`) or GitHub-style alert flags. They will compile into beautiful glassmorphic colored cards:
  ```markdown
  > [!TIP]
  > Use persistent connections (HTTP/1.1 Keep-Alive) to avoid TCP handshakes!
  ```

### 3. Syntax-Highlighted Code Blocks
Write standard fenced code blocks with their respective language identifiers. Shiki syntax highlighting will automatically color keywords inside a responsive, premium dark slate viewport:
~~~markdown
```javascript
// Simple Graceful Shutdown hook
process.on('SIGTERM', () => {
  server.close(() => {
    console.log('HTTP server closed gracefully.');
  });
});
```
~~~

### 4. Tables
For specifications, cheatsheets, or indexes, use standard markdown table syntax. The site renders them in a glassmorphic borderless layout:
```markdown
| Method | Intent | Has Body | Idempotent |
|---|---|---|---|
| `GET` | Fetch Resource | No | Yes |
| `POST` | Create Resource | Yes | No |
```

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
