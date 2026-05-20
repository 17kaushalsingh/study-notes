# Chapter 27: Exceptions

## 27.1 The Need for Exceptions

Traditional error handling (return codes, sentinel values) has problems:
- Callers can ignore error codes
- Error codes mixed with valid return values
- Error handling code clutters the logic

Exceptions provide a mechanism to **separate error handling from normal flow**.

---

## 27.2 Basic Exception Handling

### throw, try, catch

```cpp
#include <iostream>

double divide(int x, int y) {
    if (y == 0)
        throw std::runtime_error("Division by zero!");
    return static_cast<double>(x) / y;
}

int main() {
    try {
        std::cout << divide(10, 0);
    }
    catch (const std::runtime_error& e) {
        std::cerr << "Error: " << e.what() << '\n';
    }
}
```

### Key Rules
- `throw` immediately transfers control to the nearest matching `catch`
- Statements after `throw` are NOT executed
- No type conversion for exceptions (exact type matching)
- Multiple `catch` blocks can follow one `try`
- Catch by **const reference** (avoids copies and slicing)

### Catch-All Handler

```cpp
catch (...) {   // catches ANY exception type
    std::cerr << "Unknown exception!\n";
}
```

---

## 27.3 Exceptions, Functions, and Stack Unwinding

When an exception is thrown:
1. The runtime looks for a `catch` block in the current function
2. If not found, the function exits and the search continues in the caller
3. This process continues up the call stack (**stack unwinding**)
4. Local variables in each unwound function are properly destroyed
5. If no matching `catch` is found → `std::terminate()` is called

```
main() → a() → b() → c()  // c() throws
                    ← exception propagates back
         ← if b() doesn't catch
← if a() doesn't catch
← main() catches (or program terminates)
```

---

## 27.4 Uncaught Exceptions and Catch-All Handlers

If no matching `catch` exists:
- `std::terminate()` is called
- Stack may or may not be unwound (implementation-defined)

> **Best Practice**: Always have a catch-all (`catch(...)`) in `main()` as a safety net.

```cpp
int main() {
    try {
        runProgram();
    }
    catch (const std::exception& e) {
        std::cerr << "Exception: " << e.what() << '\n';
    }
    catch (...) {
        std::cerr << "Unknown exception!\n";
    }
}
```

---

## 27.5 Exceptions, Classes, and Inheritance

### std::exception Hierarchy

All standard exceptions derive from `std::exception`:

```
std::exception
├── std::bad_alloc          (new fails)
├── std::bad_cast           (dynamic_cast fails)
├── std::runtime_error
│   ├── std::overflow_error
│   ├── std::underflow_error
│   └── std::range_error
├── std::logic_error
│   ├── std::invalid_argument
│   ├── std::out_of_range
│   └── std::length_error
└── ... (28+ total)
```

### Catching Base Class Catches Derived

```cpp
catch (const std::exception& e) {
    // catches std::runtime_error, std::bad_alloc, etc.
    std::cerr << e.what() << '\n';
}
```

> **Rule**: List **derived** exception handlers BEFORE base class handlers (more specific first).

### Custom Exception Classes

```cpp
// Method 1: Derive from std::runtime_error (recommended)
class FileError : public std::runtime_error {
public:
    FileError(const std::string& msg)
        : std::runtime_error{msg} {}
};

throw FileError{"Could not open config.txt"};

// Method 2: Derive from std::exception (need to override what())
class MyException : public std::exception {
    std::string m_msg;
public:
    MyException(std::string_view msg) : m_msg{msg} {}
    const char* what() const noexcept override { return m_msg.c_str(); }
};
```

---

## 27.6 Rethrowing Exceptions

```cpp
catch (const std::exception& e) {
    std::cerr << "Logging: " << e.what() << '\n';
    throw;          // rethrow the SAME exception (preserves derived type)
    // throw e;     // WRONG: slices to std::exception!
}
```

> **Key**: Use `throw;` (no argument) to rethrow. Using `throw e;` causes slicing.

---

## 27.7 Function Try Blocks

Handle exceptions in constructor member initializer lists:

```cpp
class A {
    int m_x;
public:
    A(int x) try : m_x{x} {
        // constructor body
    }
    catch (const std::exception& e) {
        std::cerr << "Construction failed: " << e.what() << '\n';
        // exception is implicitly rethrown
    }
};
```

---

## 27.8 Exception Dangers and Downsides

### Cleanup Issues
- Dynamically allocated memory may leak if an exception is thrown before `delete`
- **Solution**: Use RAII (smart pointers, containers)

### Performance
- Exceptions add zero overhead when NOT thrown
- Throwing/catching has significant overhead (stack unwinding)
- Don't use exceptions for normal control flow

### When NOT to Use Exceptions
- For expected conditions (e.g., user enters bad input)
- In performance-critical inner loops
- In destructors (can cause `std::terminate`)

---

## 27.9 Exception Specifications and noexcept

```cpp
void safe() noexcept {      // guarantees: will not throw
    // if it DOES throw → std::terminate()!
}

void maybe() noexcept(false) {  // may throw (same as no specifier)
}
```

### When to Use noexcept

| Always noexcept | Consider noexcept |
|----------------|-------------------|
| Move constructors | Functions that don't throw |
| Move assignment | Swap functions |
| Destructors (implicit) | Simple getters |

### Benefits of noexcept
- Compiler can optimize (no unwinding support needed)
- `std::vector` will use move instead of copy for `noexcept` move operations
- Documents intent clearly

---

## 27.10 std::move_if_noexcept

```cpp
#include <utility>

// Uses move if move constructor is noexcept, otherwise copies
auto value{ std::move_if_noexcept(x) };
```

Used internally by `std::vector` during reallocation — if move might throw, it copies instead (safer but slower).

---

## Chapter 27 — Quick Reference Card

| Concept | Rule |
|---------|------|
| Catch exceptions | By `const&` (avoid slicing/copies) |
| Catch order | Most derived first, base last |
| Rethrow | Use `throw;` not `throw e;` |
| Custom exceptions | Derive from `std::runtime_error` |
| `noexcept` | Always on move operations and destructors |
| Cleanup during exceptions | Use RAII (smart pointers) |
| Exceptions in destructors | **Never throw** from destructors |
| Performance | Zero cost when not thrown; expensive when thrown |
| Safety net | `catch(...)` in main() |
