# Database System Concepts - Note-Taking Instructions

This file contains the mandatory workflow and conventions for creating learning and revision notes for the "Database System Concepts" textbook.

## Development Workflow: Note Generation

For each section (e.g., `Section1-RelationalLanguages.pdf`):

1. **Folder Organization**:
   - Create a new directory named after the section number (e.g., `Section 1`).
   - Move the corresponding PDF into that folder.

2. **File Structure**:
   - Within each section folder, create separate Markdown (`.md`) files for each chapter contained in that section.
   - Create a single `Summary.md` file that summarizes the entire section.

## Content Guidelines for Chapter Notes

Each chapter markdown file must be structured as follows:

1. **Detailed Notes**:
   - Include both **theoretical concepts** and **practical applications** (e.g., SQL examples, design patterns).
   - Ensure notes are highly detailed yet concise, to-the-point, and non-repetitive.
   - Follow the textbook's methodology and use the **University Schema** for examples where applicable.

2. **Must-Know Things**:
   - A dedicated section for critical definitions, core theorems, and "exam-favorite" topics.

3. **Quick Revision**:
   - A final section summarizing the chapter's key points for rapid review.

## Project Structure (Target State)
```text
/Database System Concepts/
├── Section 1/
│   ├── Section1-RelationalLanguages.pdf
│   ├── Chapter-02-Relational-Model.md
│   ├── Chapter-03-SQL.md
│   └── Summary.md
├── Section 2/
│   ├── ...
```

## Guiding Principles
- **Conciseness**: Avoid fluff. Use bullet points and headers for readability.
- **Completeness**: Cover all major sub-topics mentioned in the PDF.
- **Accuracy**: Strictly adhere to the terminology and concepts in the source text.
