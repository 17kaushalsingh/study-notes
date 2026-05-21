# Project Instructions

## Local Skills Availability
Skills that are available locally for this project include:
- `lecture-notes-generator`
- `md-to-html-converter`

## Conventions & Style
- **Formatting:** Use GitHub-flavored Markdown for all notes.
- **HTML:** Generated HTML should follow the dark-mode "Developer Documentation" CSS style located in the `md-to-html-converter` skill assets.
- **Organization:** Each lecture has its own directory (e.g., `LEC8/`) containing the raw transcript (`.txt`), markdown notes (`.md`), and the final web-ready version (`.html`).
- **Index:** Always update the root `index.html` when a new lecture is added.

## Instructions
- If lecture transcript is provided, use lecture-notes-generator skill.
It generates ultra-detailed, high-quality Markdown lecture notes from YouTube transcripts (.txt). Use this to synthesize raw transcript data into professional documentation with First Principles analogies and visual integrations.
- If lecture link is provided or other relevant content is provided, or request to add other info in notes is made, add it on top of the notes genertaed by the lecture notes generator skill. 
- After generating the notes, use the md-to-html-converter skill to convert the Markdown notes into polished HTML files with a consistent, modern dark-mode "Developer Documentation" CSS style. This ensures that the generated notes are web-ready and visually appealing for readers.
- When generating HTML files, ensure that they are clean and free of any extraneous content or branding. The focus should be solely on the lecture notes and relevant information.
- After this flow is complete, add the generated HTML notes file to the main index.html file of the project, ensuring that it is easily accessible and well-integrated into the overall structure of the documentation.