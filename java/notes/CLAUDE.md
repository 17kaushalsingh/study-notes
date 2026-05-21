# Working Notes

## Objective

Produce academically rigorous Java notes in Markdown, chapter-wise, following the exact topic progression of `https://roadmap.sh/java`.

## Source of Structure

- Primary roadmap page: `https://roadmap.sh/java`
- Topic extraction cross-check: `https://roadmap.sh/pdfs/roadmaps/java.pdf`

## Implementation Decisions

- Use a chapter sequence that mirrors the roadmap node order rather than reorganizing into a custom syllabus.
- Keep one major conceptual cluster per file so topics remain traceable to roadmap sections.
- Add code and pseudocode for most sections, but prefer explanation when the roadmap node is primarily architectural or comparative.

## Verification Checklist

- Confirm every roadmap topic from the Java PDF is represented in either a chapter heading or subsection.
- Confirm root `README.md` and `chapters/README.md` exist and describe the structure.
- Confirm file names are sequential and navigable.
