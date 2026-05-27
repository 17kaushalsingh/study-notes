# Google Docs - Design Problem

A sample LLD problem demonstrating evolution from monolithic to SOLID-compliant design.

---

## Problem Statement
Design a document editor that can:
- Add text and images
- Render the document
- Save to different storage backends

---

## Design 1: Monolithic Approach (`design1.cpp`)

### Structure
```
DocumentEditor
├── addText()
├── addImage()
├── renderDocument()
├── printDocument()
└── saveToFile()
```

### Key Points
- Single class handles everything
- `isImagePath()` checks file extension (.png, .jpg)
- Stores elements as `vector<string>`
- Tight coupling between rendering and storage

### Issues
- **SRP Violation**: One class does rendering + storage + validation
- **OCP Violation**: Adding new element types requires modifying the class
- **Not extensible**: Can't add new storage types (database, cloud)

---

## Design 2: SOLID Principles Applied (`design2.cpp`)

### Class Hierarchy

```
DocumentElement (Abstract)
├── TextElement
├── ImageElement
└── TableElement

Persistence (Abstract)
├── FileStorage
└── DatabaseStorage

Document
└── addElement()
└── getDocument()
```

### Key Improvements

| Principle | Implementation |
|-----------|----------------|
| **SRP** | Separate classes for elements, document, and persistence |
| **OCP** | Add new element types without modifying existing code |
| **LSP** | All elements implement `render()` uniformly |
| **DIP** | Document depends on `DocumentElement*` abstraction |

### Adding New Features
- **New element type**: Create class extending `DocumentElement`, implement `render()`
- **New storage**: Create class extending `Persistence`, implement `save()`

---

## Quick Revision Checklist

1. **What pattern is used for elements?**
   - Polymorphism with abstract base class

2. **What pattern is used for storage?**
   - Strategy pattern (swap FileStorage/DatabaseStorage)

3. **How to add a new element (e.g., ChartElement)?**
   ```cpp
   class ChartElement : public DocumentElement {
       std::string render() override { return "[Chart: ...]"; }
   };
   ```

4. **How to add cloud storage?**
   ```cpp
   class CloudStorage : public Persistence {
       void save(std::string &data) override { /* upload to cloud */ }
   };
   ```

---

## Compile & Run
```bash
g++ -o design1 design1.cpp && ./design1
g++ -o design2 design2.cpp && ./design2
```
