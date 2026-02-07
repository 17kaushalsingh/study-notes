# LearnCpp.com — Complete Chapter Notes

Comprehensive study notes from [learncpp.com](https://www.learncpp.com/), covering all chapters with code snippets, key concepts, best practices, and quick reference cards.

## Table of Contents

### Fundamentals
- [Chapter 0: Introduction & Getting Started](chapter-00-introduction.md)
- [Chapter 1: C++ Basics](chapter-01-cpp-basics.md)
- [Chapter 2: Functions and Files](chapter-02-functions-and-files.md)
- [Chapter 3: Debugging](chapter-03-debugging.md)
- [Chapter 4: Fundamental Data Types](chapter-04-fundamental-data-types.md)
- [Chapter 5: Constants and Strings](chapter-05-constants-and-strings.md)
- [Chapter 6: Operators](chapter-06-operators.md)

### Program Structure
- [Chapter 7: Scope, Duration, and Linkage](chapter-07-scope-duration-linkage.md)
- [Chapter 8: Control Flow](chapter-08-control-flow.md)
- [Chapter 9: Error Detection and Handling](chapter-09-error-detection.md)

### Type System
- [Chapter 10: Type Conversion, Aliases, and Deduction](chapter-10-type-conversion.md)
- [Chapter 11: Function Overloading and Templates](chapter-11-function-overloading-templates.md)

### Compound Types
- [Chapter 12: References and Pointers](chapter-12-references-and-pointers.md)
- [Chapter 13: Enums and Structs](chapter-13-enums-and-structs.md)

### Object-Oriented Programming
- [Chapter 14: Introduction to Classes](chapter-14-introduction-to-classes.md)
- [Chapter 15: More on Classes](chapter-15-more-on-classes.md)

### Arrays and Containers
- [Chapter 16: Dynamic Arrays (std::vector)](chapter-16-dynamic-arrays-vector.md)
- [Chapter 17: Fixed-Size Arrays (std::array & C-style)](chapter-17-fixed-arrays.md)
- [Chapter 18: Iterators and Algorithms](chapter-18-iterators-algorithms.md)

### Memory Management
- [Chapter 19: Dynamic Allocation](chapter-19-dynamic-allocation.md)
- [Chapter 22: Move Semantics and Smart Pointers](chapter-22-move-semantics-smart-pointers.md)

### Advanced Functions
- [Chapter 20: Functions (Pointers, Recursion, Lambdas)](chapter-20-functions.md)

### Operator Overloading
- [Chapter 21: Operator Overloading](chapter-21-operator-overloading.md)

### Relationships and Inheritance
- [Chapter 23: Object Relationships](chapter-23-object-relationships.md)
- [Chapter 24: Inheritance](chapter-24-inheritance.md)
- [Chapter 25: Virtual Functions and Polymorphism](chapter-25-virtual-functions.md)

### Advanced Topics
- [Chapter 26: Templates and Classes](chapter-26-templates-and-classes.md)
- [Chapter 27: Exceptions](chapter-27-exceptions.md)
- [Chapter 28: Input and Output (I/O)](chapter-28-input-output.md)

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
