# Study Notes & Engineering Portal

This repository is a dual-purpose developer ecosystem. It consolidates a robust corpus of engineering study notes, and integrates an **Astro Static Site Generator** to build and host them as a premium, highly responsive static site.

---

## 🛠️ The Architecture: How Astro is Connected to the Static Site

The portal at the root compiles your local Markdown documents into an ultra-fast, modern static site with a sleek dark tech aesthetic, persistent Light/Dark theme toggle, and a full-text clientside search engine.

```
                  [ Filesystem Content ]
                            │
                            ▼
              ┌───────────────────────────┐
              │     notes/ directory      │ (Raw Markdown notes)
              └─────────────┬─────────────┘
                            │
                            ▼
                  [ Astro Static Generator ]  (Compiled at Build-Time)
              ┌───────────────────────────┐
              │    src/pages/ & components│  (import.meta.glob parses notes)
              └─────────────┬─────────────┘
                            │
                            ▼
                  [ Static Assets Output ]
              ┌───────────────────────────┐
              │     dist/ directory       │  (Pure HTML/CSS/JS + search index)
              └─────────────┬─────────────┘
                            │
                            ▼
                  [ Vercel Edge Hosting ]    (Triggered automatically via Git)
```

1. **In-Place Globbing**: Astro is configured to scan your physical `/notes/` folder at compile-time using Vite's `import.meta.glob()`. This means **you do not need to move, duplicate, or rename** your content folders to host them.
2. **Static Prerendering**: During `npm run build`, Astro parses each `.md` file, extracts headings (creating a sticky index on the right pane), computes numerical order for Next/Previous pagination, and compiles everything into plain, static HTML and CSS under `/dist/`.
3. **Clientside Search Indexing**: The build automatically outputs `/search-index.json`, a highly optimized index containing full-text clips of your notes. The clientside Algolia-style modal (`Cmd + K`) queries this JSON file for high-speed substring matching and yellow marker highlights.
4. **Vercel Edge Integration**: When pushed to GitHub, Vercel's automated git hooks fetch the repo, run the Astro compiler, and deploy your static `dist/` pages to edge servers globally.

---

## ✍️ How to Add Notes for Automatic Publishing

Any note you create can be automatically published to the web. To ensure your notes are correctly parsed, sorted, and indexed, follow these structure guidelines:

### 1. Adding a Hosted Lecture (e.g., First Principles Backend)
Place your new Markdown file directly inside the active directory:
📂 `notes/systes-and-backend/backend-from-first-principles/`

To ensure automatic publishing, structure your note file as follows:
- **Filename Pattern**: Name the file starting with `LEC` followed by a number (e.g. `LEC24.md` or `LEC21.3.md`). Astro uses this pattern to numerically sort the sidebar list and build accurate Previous/Next navigation controls.
- **Header Structure**: Start the very first line of your Markdown file with a single `# H1 Heading` representing the lecture topic (e.g., `# Concurrency and Thread Pools`). Astro automatically extracts this heading as the page's HTML `<title>` and sidebar listing text.
- **Code Blocks**: Write standard Markdown code blocks. Astro's built-in Shiki syntax highlighter will automatically highlight your code keywords with high-contrast styles inside a permanent dark slate frame.

### 2. Adding a Sitemap Folder (Coming Soon)
If you add new folders or files to `notes/databases/`, `notes/dsa-and-cp/`, or `notes/languages/`:
- They will **automatically appear inside the sitemap explorer tree** on the website (`/sitemap`)!
- The clientside tree component recursively maps your physical directories at build-time, maintaining a complete, polished sitemap of your offline/local notes automatically.

---

## 🚀 Running the Site Locally

Verify and preview your notes portal locally before deploying:

1. **Install Dependencies** (Only required once):
   ```bash
   npm install
   ```
2. **Spin up Dev Server** (Launches Vite locally at `http://localhost:4321`):
   ```bash
   npm run dev
   ```
3. **Check Static Compilation**:
   ```bash
   npm run build
   ```
