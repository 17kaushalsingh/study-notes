# LearnCpp.com — Complete Chapter Notes

Comprehensive study notes from [learncpp.com](https://www.learncpp.com/), covering all chapters with code snippets, key concepts, best practices, and quick reference cards.

## Table of Contents

### Fundamentals
- [Chapter 0: Introduction & Getting Started](notes/chapter-00-introduction.md)
- [Chapter 1: C++ Basics](notes/chapter-01-cpp-basics.md)
- [Chapter 2: Functions and Files](notes/chapter-02-functions-and-files.md)
- [Chapter 3: Debugging](notes/chapter-03-debugging.md)
- [Chapter 4: Fundamental Data Types](notes/chapter-04-fundamental-data-types.md)
- [Chapter 5: Constants and Strings](notes/chapter-05-constants-and-strings.md)
- [Chapter 6: Operators](notes/chapter-06-operators.md)

### Program Structure
- [Chapter 7: Scope, Duration, and Linkage](notes/chapter-07-scope-duration-linkage.md)
- [Chapter 8: Control Flow](notes/chapter-08-control-flow.md)
- [Chapter 9: Error Detection and Handling](notes/chapter-09-error-detection.md)

### Type System
- [Chapter 10: Type Conversion, Aliases, and Deduction](notes/chapter-10-type-conversion.md)
- [Chapter 11: Function Overloading and Templates](notes/chapter-11-function-overloading-templates.md)

### Compound Types
- [Chapter 12: References and Pointers](notes/chapter-12-references-and-pointers.md)
- [Chapter 13: Enums and Structs](notes/chapter-13-enums-and-structs.md)

### Object-Oriented Programming
- [Chapter 14: Introduction to Classes](notes/chapter-14-introduction-to-classes.md)
- [Chapter 15: More on Classes](notes/chapter-15-more-on-classes.md)

### Arrays and Containers
- [Chapter 16: Dynamic Arrays (std::vector)](notes/chapter-16-dynamic-arrays-vector.md)
- [Chapter 17: Fixed-Size Arrays (std::array & C-style)](notes/chapter-17-fixed-arrays.md)
- [Chapter 18: Iterators and Algorithms](notes/chapter-18-iterators-algorithms.md)

### Memory Management
- [Chapter 19: Dynamic Allocation](notes/chapter-19-dynamic-allocation.md)
- [Chapter 22: Move Semantics and Smart Pointers](notes/chapter-22-move-semantics-smart-pointers.md)

### Advanced Functions
- [Chapter 20: Functions (Pointers, Recursion, Lambdas)](notes/chapter-20-functions.md)

### Operator Overloading
- [Chapter 21: Operator Overloading](notes/chapter-21-operator-overloading.md)

### Relationships and Inheritance
- [Chapter 23: Object Relationships](notes/chapter-23-object-relationships.md)
- [Chapter 24: Inheritance](notes/chapter-24-inheritance.md)
- [Chapter 25: Virtual Functions and Polymorphism](notes/chapter-25-virtual-functions.md)

### Advanced Topics
- [Chapter 26: Templates and Classes](notes/chapter-26-templates-and-classes.md)
- [Chapter 27: Exceptions](notes/chapter-27-exceptions.md)
- [Chapter 28: Input and Output (I/O)](notes/chapter-28-input-output.md)

---

## Quick Best Practices Summary

| Topic | Best Practice |
|-------|--------------|
| Integers | Use `int` (default) or `long long` (large) |
| Floats | Use `double` (not `float`) |
| Constants | `constexpr` (compile-time) or `const` (runtime) |
| Strings | `std::string_view` for read-only params; `std::string` for ownership |
| Containers | `std::vector` (dynamic) or `std::array` (fixed) |
| Passing objects | By value (cheap types) or `const&` (expensive types) |
| Memory | Smart pointers (`unique_ptr` / `shared_ptr`), never raw `new`/`delete` |
| Enums | Prefer `enum class` over `enum` |
| Classes | Private data, public interface, use `explicit` constructors |
| Inheritance | Public inheritance; virtual destructor if polymorphic |
| Overrides | Always use `override` keyword |
| Exceptions | Catch by `const&`; `noexcept` on move operations |

Source: [learncpp.com](https://www.learncpp.com/)
