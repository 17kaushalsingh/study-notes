# Chapter 0: Introduction / Getting Started

---

## 0.1 -- Introduction to These Tutorials

This lesson establishes the foundation and philosophy for learning C++.

### Target Audience

No prior programming experience is required. The tutorials are designed for hobbyists and professionals alike, beginning with a broad overview and deepening understanding in subsequent chapters.

### Goals of the Tutorials

1. **Holistic programming education** -- covers general programming topics (style, debugging, pitfalls, best practices), not just C++ syntax.
2. **Rich examples** -- numerous clear, concise examples prioritized over pure theory.
3. **Practical exercises** -- practice programs with solutions throughout for skill reinforcement.
4. **Engagement** -- programming should be enjoyable. Tired or unhappy programmers make mistakes.

### Best Practices for Learning

1. **Type code by hand** -- do not copy-paste. You learn error patterns and diagnostics by typing.
2. **Solve problems independently first** -- debug your own code before seeking help.
3. **Experiment actively** -- modify examples, test edge cases, explore variations.
4. **Attempt quizzes multiple times** -- struggle strengthens understanding.
5. **Write original programs** -- reinforce concepts with your own projects.

> **Key Takeaway:** If you get stuck: read comments on the lesson, scan ahead in the tutorials, search externally, consult AI cautiously, or ask on Stack Overflow.

---

## 0.2 -- Introduction to Programming Languages

### Computer Programs and Hardware

A **computer program** is a sequence of instructions that directs a computer to perform certain actions in a specified order. Programs run on hardware (CPU, memory, storage, interactive devices) and interact with the operating system.

### Three Categories of Programming Languages

#### Machine Language
- The only format CPUs natively understand.
- Appears as binary strings: `10110000 01100001`
- Specific to each CPU family/instruction set.
- Extremely difficult for humans to write and maintain.

#### Assembly Language
- Human-readable intermediary using mnemonics: `mov al, 0x61`
- Translated to machine code via an **assembler**.
- Still platform-specific and not portable.

#### High-Level Languages
- Examples: C, C++, Java, Python.
- Abstract away platform specifics and enable cross-platform development.
- Two execution methods:
  - **Compiling**: translates entire program to machine code before execution.
  - **Interpreting**: executes code directly, line by line, without prior compilation.

```cpp
// High-level C++ vs. low-level assembly
a = 97;           // C++ equivalent to a machine instruction
a = b * 2 + 5;    // Single line replacing 4-6 assembly instructions
```

> **Key Takeaway:** High-level languages provide portability -- write code once, run on multiple platforms. C++ sits as a "mid-level language" balancing performance with convenience.

> **Warning:** Compiler-specific extensions and platform-specific code break cross-platform compatibility. Design for portability early.

---

## 0.3 -- Introduction to C/C++

### Historical Context

- **C Language**: Developed in 1972 by Dennis Ritchie at Bell Labs as a systems programming language.
- **C++**: Created by Bjarne Stroustrup starting in 1979 as an extension to C, adding object-oriented programming.
- **Standardization**: C++ was formally standardized in 1998 by ISO. Major updates: C++11, C++14, C++17, C++20, C++23.

### Key Characteristics

- C++ is best thought of as a superset of C.
- Design philosophy: **"trust the programmer"** -- grants high freedom but demands responsibility.
- "Knowing what you shouldn't do in C/C++ is almost as important as knowing what you should do."

### Where C++ Excels

- Video games
- Real-time systems (e.g., transportation, manufacturing)
- High-performance financial applications
- Embedded software
- AI and neural networks

> **Key Takeaway:** C++ remains one of the top 2-3 most popular compiled languages. No prior C knowledge is required before learning C++. The language receives updates roughly every 3 years.

---

## 0.4 -- Introduction to C++ Development

### The Software Development Process (7 Steps)

1. **Define the problem** -- conceptualize what you want to build.
2. **Determine the solution approach** -- design before coding.
3. **Write the program** -- use a text editor to create source code.
4. **Compile** -- translate source code to machine code.
5. **Link** -- combine object files into an executable.
6. **Test** -- verify the program works correctly.
7. **Debug** -- find and fix errors.

### Step 2: Solution Design (Most Neglected Step)

Effective solutions share these traits:
- Straightforward and not overly complicated.
- Well-documented with clear assumptions.
- Built modularly for reusability.
- Graceful error handling with useful messages.

> **Remember:** Only 10-40% of developer time involves writing initial code. The remaining 60-90% goes to maintenance, debugging, updates, and improvements.

### Step 3: Writing the Program

```cpp
#include <iostream>

int main()
{
    std::cout << "Here is some text.";
    return 0;
}
```

### Essential Tools

- **Line numbering** -- helps locate compiler errors.
- **Syntax highlighting** -- color-codes program components.
- **Monospace font** -- distinguishes similar characters (0 vs O, 1 vs l vs I).

### File Naming Conventions

- Name the primary source file `main.cpp`.
- `.cpp` is the standard C++ extension (alternatives: `.cc`, `.cxx`).

### Key Terminology

- **Bug**: any programming error that prevents correct operation.
- **Debugging**: removing bugs from code.
- **Source code**: the C++ instructions saved in a text file.

> **Key Takeaway:** Spending extra time on design prevents significant problems later. Use proper tools (a code editor, not a word processor).

---

## 0.5 -- Introduction to the Compiler, Linker, and Libraries

### Step 4: Compiling Source Code

The C++ compiler performs two critical functions:
1. **Syntax validation** -- checks code against C++ language rules; reports errors with line numbers.
2. **Translation** -- converts C++ code into machine language instructions stored in **object files** (`.o` or `.obj`).

### Step 5: Linking

The **linker** combines object files and libraries to produce the final executable:
- Validates object files.
- Resolves cross-file dependencies (connecting definitions across multiple `.cpp` files).
- Links library files containing precompiled code.
- Generates the executable.

### The C++ Standard Library

A set of useful precompiled capabilities included with every C++ compiler. The **iostream** library is the most commonly used for console I/O. Most linkers include the standard library automatically.

### Steps 6 and 7: Testing and Debugging

- **Testing**: assessing software behavior against expected outcomes through varied inputs.
- **Debugging**: finding and fixing errors when behavior diverges from expectations.

### Integrated Development Environments (IDEs)

IDEs bundle the editor, compiler, linker, and debugger into a single unified application, streamlining the development workflow.

> **Key Takeaway:** Compilation and linking are distinct sequential processes. "Building" refers to the complete conversion from source code to executable.

---

## 0.9 -- Configuring Your Compiler: Build Configurations

### What Are Build Configurations?

A **build configuration** (or build target) is a collection of project settings determining how the IDE compiles and packages programs.

### Two Standard Configurations

| Feature | Debug | Release |
|---|---|---|
| Optimizations | Disabled | Enabled |
| Debug info | Included | Excluded |
| Executable size | Larger | Smaller |
| Speed | Slower | Faster |
| Use case | Development | Distribution |

**Example:** A Hello World program produced a 65KB debug executable vs. 12KB in release mode.

### Command-Line Flags (GCC/Clang)

```
Debug:    -O0 -ggdb
Release:  -O2 -DNDEBUG
Aggressive: -O3 (requires testing)
```

> **Best Practice:** Use the **debug** build configuration during development. Switch to **release** when distributing to others or testing performance.

> **Remember:** Apply configuration changes across all build profiles to prevent inconsistent settings.

---

## 0.10 -- Configuring Your Compiler: Compiler Extensions

### What Are Compiler Extensions?

Compiler extensions are **compiler-specific behaviors** that deviate from the C++ standard. They allow non-standard features, often enabled by default.

### The Problem

- Programs using extensions won't compile reliably across different compilers.
- Learners may mistake non-standard behavior for official C++ functionality.

### How to Disable Extensions

| Compiler | Setting |
|---|---|
| Visual Studio | Set Conformance mode to `Yes (/permissive-)` in project properties |
| Code::Blocks | Check `-pedantic-errors` in Settings > Compiler |
| GCC/Clang | Add `-pedantic-errors` flag |

> **Best Practice:** Disable compiler extensions to ensure your programs remain compliant with C++ standards and will work on any system. This must be configured for each new project.

---

## 0.11 -- Configuring Your Compiler: Warning and Error Levels

### Diagnostic Messages

The compiler generates two types of diagnostics:

1. **Compilation Errors** -- serious issues that halt compilation.
2. **Warnings** -- potential problems where compilation continues but the code may behave unexpectedly.

> **Warning:** Different compilers may categorize the same issue differently. One compiler's error might be another's warning.

### Recommended Warning Flags

| Compiler | Flags |
|---|---|
| Visual Studio | `/W4`, `/w44365` for signed/unsigned warnings |
| Code::Blocks | `-Wall -Weffc++ -Wextra -Wconversion -Wsign-conversion` |
| GCC/Clang | `-Wall -Weffc++ -Wextra -Wconversion -Wsign-conversion` |

### Treating Warnings as Errors

| Compiler | Flag |
|---|---|
| GCC/Clang | `-Werror` |
| Visual Studio | `/WX` or "Treat Warnings As Errors" in properties |

> **Best Practice:** Turn warning levels up to maximum, especially while learning. Do not let warnings pile up -- resolve them immediately. Enable "treat warnings as errors" to enforce this discipline.

> **Warning:** Avoid `/Wall` in Visual Studio -- it generates excessive standard library warnings that are not useful.

---

## 0.12 -- Configuring Your Compiler: Choosing a Language Standard

### C++ Language Standards

C++ has multiple versions based on publication years:

| Standard | Year | Development Name |
|---|---|---|
| C++11 | 2011 | C++0x |
| C++14 | 2014 | C++1y |
| C++17 | 2017 | C++1z |
| C++20 | 2020 | C++2a |
| C++23 | 2024 | C++2b |
| C++26 | (in progress) | C++2c |

### How to Set the Standard

**GCC/Clang command-line flags:**
```
-std=c++17
-std=c++20
-std=c++23
```

**Visual Studio:**
- Project > Properties > C/C++ > Language > C++ Language Standard
- Recommended: `ISO C++ Latest (/std:c++latest)`
- Must be set for each new project (no global setting).

**Code::Blocks:**
- Settings > Compiler > select checkbox for desired standard.
- Allows a global setting (more convenient than Visual Studio).

### Choosing a Standard

- **Professional environments**: use one or two versions behind the latest for stability.
- **Personal/learning projects**: use the latest finalized standard -- there is little downside.

> **Best Practice:** When changing the language standard (or any project setting), make the change to **all** build configurations.

> **Warning:** Even after a standard is finalized, compiler support may be incomplete, partial, or buggy. Check CPPReference compiler support tables if code fails to compile.

### Troubleshooting

If standards-compliant code won't compile:
1. Verify your compiler is using the intended language standard.
2. Check CPPReference for compiler support tables.
3. Consider upgrading your compiler.
4. Try an alternative compiler.
