---
name: lecture-notes-generator
description: Generates ultra-detailed, high-quality Markdown lecture notes from YouTube transcripts (.txt). Use this to synthesize raw transcript data into professional documentation with First Principles analogies and visual integrations.
---

# Lecture Notes Generator

This skill transforms raw YouTube transcripts into structured, professional, and ultra-detailed Markdown notes.

## Workflow

1.  **Analyze Transcript**: Read the provided `.txt` transcript (e.g., `LEC5.txt`).
2.  **Contextual Scan**: Search the current directory for related assets:
    -   `.png` files (diagrams) to integrate into the notes.
    -   `.txt` files with supplementary topic data.
3.  **Synthesis & Expansion**:
    -   **First Principles**: Explain core concepts using intuitive analogies (e.g., "The Parcel Analogy" for headers).
    -   **External Depth**: Supplement transcript data with standard industry knowledge (e.g., expanding on TLS 1.3 handshake or QUIC mechanics).
    -   **Technical Tables**: Create descriptive tables for comparisons (Methods, HTTP Versions, Status Codes).
4.  **No Fluffy Content**: Focus strictly on technical value. Do NOT add branding, "Academy" footers, or filler text.
5.  **Write Output**: Create a `.md` file with the same name as the transcript (e.g., `LEC5.md`). Do NOT generate HTML.

## Content Standards

-   **Structure**: Logic should flow from core principles -> transport/lower layers -> detailed components -> performance/security -> advanced data handling.
-   **Visuals**: Place `![alt text](filename.png "Tooltip")` tags immediately following the relevant technical description.
-   **Semantics**: Clearly define method properties (Safe vs. Idempotent) and status code categories.
-   **Clarity**: Use bolding and bullet points to ensure the notes are descriptive but not overwhelming to skim.

## Examples

-   **Analogies**: "Think of an HTTP request as a physical parcel. The headers are the address written on top..."
-   **Tables**:
| Method | Safe? | Idempotent? | Description |
| :--- | :---: | :---: | :--- |
| GET | ✅ | ✅ | Fetch data... |
