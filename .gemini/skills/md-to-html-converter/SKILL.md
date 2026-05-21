---
name: md-to-html-converter
description: Converts Markdown (.md) files to polished HTML (.html) files with a consistent, modern dark-mode "Developer Documentation" CSS style. Use this when you need to generate high-quality web-ready notes or documentation from markdown sources.
---

# Markdown to HTML Converter

This skill automates the conversion of Markdown files into aesthetically pleasing HTML documents using a standardized CSS theme.

## Workflow

1.  **Read the Source**: Read the content of the specified `.md` file.
2.  **Generate HTML**: Convert the Markdown content into HTML.
    -   Use `<h1>` for the main title.
    -   Use `<section>` tags to group content under `<h2>` headers.
    -   Apply the standardized CSS from `assets/style.css`.
    -   Ensure tables, code blocks, and images are formatted according to the theme classes.
3.  **No Fluffy Content**: Do NOT add unnecessary footers, branding, or filler text (e.g., "Backend Engineering Academy - Lecture X Notes"). Keep the focus strictly on the technical content.
4.  **Write the Output**: Save the resulting HTML to a `.html` file with the same name as the source.

## Style Guidelines

The generated HTML must follow these structural patterns to ensure the CSS renders correctly:

-   **Container**: Wrap everything in a `<div class="container">`.
-   **Keywords**: For "Keywords" or "Tags", use `<div class="keywords">` with `<span class="keyword-tag">` for each item.
-   **Diagrams**: Wrap images in `<div class="diagram">`.
-   **Notes**: Use `<div class="note-box">` for important callouts.
-   **Tables**: Ensure standard `<table>` structure with `<thead>` and `<tbody>`.

## Resources

-   **CSS**: `assets/style.css` (The master stylesheet)
