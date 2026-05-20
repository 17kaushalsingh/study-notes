# Chapter 12: Compound Types — References and Pointers

## 12.1 Introduction to Compound Data Types

Compound types are built from fundamental types:
- References (lvalue references, rvalue references)
- Pointers
- Arrays
- Structs, classes, unions
- Enumerations
- Functions

---

## 12.2 Value Categories (Lvalues and Rvalues)

### Lvalues
An **lvalue** is an expression that evaluates to an identifiable object with a persistent address:

```cpp
int x{ 5 };    // x is an lvalue
int& ref{ x }; // ref is an lvalue
```

Lvalues can be **modifiable** (non-const) or **non-modifiable** (const).

### Rvalues
An **rvalue** is a temporary value that doesn't persist beyond the expression:

```cpp
5              // literal rvalue
x + 1          // temporary result
return5()      // return by value
static_cast<int>(d)  // temporary result
```

### Key Rules
- Assignment requires a **modifiable lvalue** on the left
- Lvalues implicitly convert to rvalues when needed
- `++x` returns an **lvalue**; `x++` returns an **rvalue**

---

## 12.3 Lvalue References

A reference is an **alias** for an existing object:

```cpp
int x{ 5 };
int& ref{ x };   // ref is an alias for x

ref = 10;         // modifies x
std::cout << x;   // prints 10
```

### Rules
- Must be initialized at declaration
- Cannot be reseated (bound to a different object)
- The type must match (with some exceptions for const references)
- Has the same lifetime independence as the referenced object

---

## 12.4 Lvalue References to Const

```cpp
const int x{ 5 };
const int& ref{ x };    // OK: const ref to const value
const int& ref2{ 5 };   // OK: const ref can bind to rvalue!
const int& ref3{ x+1 }; // OK: const ref extends temporary lifetime
```

### The Power of Const References
- Can bind to **modifiable lvalues**, **non-modifiable lvalues**, AND **rvalues**
- When bound to a temporary (rvalue), the temporary's lifetime is **extended** to match the reference
- This is why `const&` is the go-to for function parameters

---

## 12.5-12.6 Pass by Reference

### Pass by Value (Copy)

```cpp
void print(std::string s) {    // expensive copy!
    std::cout << s;
}
```

### Pass by (Non-Const) Reference

```cpp
void addOne(int& y) {          // can modify original
    ++y;
}
int x{ 5 };
addOne(x);                     // x is now 6
```

### Pass by Const Reference (Best Practice)

```cpp
void print(const std::string& s) {  // no copy, can't modify
    std::cout << s;
}
print("hello");                // works with rvalues too!
```

### When to Use What

| Parameter type | When to use |
|---------------|-------------|
| Pass by value (`int x`) | Cheap-to-copy types (fundamental types, small objects) |
| Pass by const ref (`const T& x`) | Expensive-to-copy types (string, vector, classes) |
| Pass by non-const ref (`T& x`) | When function needs to modify the argument |

> **Best Practice**: Pass fundamental types by value, class types by `const&`.

---

## 12.7 Introduction to Pointers

A pointer holds the **memory address** of another object:

```cpp
int x{ 5 };
int* ptr{ &x };       // ptr holds address of x

std::cout << ptr;      // prints memory address (e.g., 0x7fff...)
std::cout << *ptr;     // dereferences: prints 5

*ptr = 10;             // modifies x through the pointer
```

### Key Operators
| Operator | Context | Meaning |
|----------|---------|---------|
| `&` | Declaration (`int& ref`) | Reference |
| `&` | Expression (`&x`) | Address-of |
| `*` | Declaration (`int* ptr`) | Pointer |
| `*` | Expression (`*ptr`) | Dereference |

### Pointer vs Reference
| Feature | Reference | Pointer |
|---------|-----------|---------|
| Can be null | No | Yes |
| Can be reseated | No | Yes |
| Must be initialized | Yes | No (but should be) |
| Syntax to access | Direct (`ref`) | Dereference (`*ptr`) |
| Can be dangling | Yes | Yes |

---

## 12.8 Null Pointers

```cpp
int* ptr{};           // null pointer (value initialization)
int* ptr{ nullptr };  // explicit null pointer

if (ptr)              // false if null
    std::cout << *ptr;

if (ptr == nullptr)   // explicit null check
    std::cout << "null!";
```

> **Warning**: Dereferencing a null pointer = **undefined behavior** (typically crashes).

> **Best Practice**:
> - Always initialize pointers (to an address or `nullptr`)
> - Check for null before dereferencing
> - Use `nullptr`, never `NULL` or `0`
> - Prefer references over pointers when you don't need null capability

---

## 12.9 Pointers and Const

```cpp
int value{ 5 };

// Pointer to const — can't modify value through pointer
const int* ptr1{ &value };   // *ptr1 = 10; ERROR
ptr1 = &other;               // OK — can change what it points to

// Const pointer — can't change what it points to
int* const ptr2{ &value };   // ptr2 = &other; ERROR
*ptr2 = 10;                  // OK — can modify value

// Const pointer to const — can't do either
const int* const ptr3{ &value };
```

### Memory Aid
> "const on the **left** of `*` → value is const. const on the **right** of `*` → pointer is const."

---

## 12.10-12.11 Pass by Address

```cpp
void print(const int* ptr) {  // pass by address
    if (ptr)
        std::cout << *ptr;
}

int x{ 5 };
print(&x);       // pass address of x
print(nullptr);  // can pass null — function handles it
```

### Pass by Address vs Reference

| Feature | By Reference | By Address |
|---------|-------------|-----------|
| Null allowed | No | Yes |
| Syntax at call site | `func(x)` | `func(&x)` |
| Syntax in function | `param` | `*param` |
| Safer | Yes | No (null/dangling risk) |

> **Best Practice**: Prefer pass by reference. Use pass by address only when null is a valid/meaningful input.

---

## 12.12 Return by Reference and Return by Address

### Safe Return by Reference

```cpp
// Safe: returning a reference parameter
const std::string& firstAlpha(const std::string& a, const std::string& b) {
    return (a < b) ? a : b;  // both outlive the function
}

// Safe: returning static local
const std::string& getName() {
    static const std::string name{ "Alex" };
    return name;
}
```

### DANGEROUS: Returning Local by Reference

```cpp
const std::string& getName() {
    std::string name{ "Alex" };  // local variable
    return name;  // DANGLING REFERENCE — name destroyed!
}
```

> **Rule**: Never return a reference or pointer to a local variable.

---

## 12.13 In and Out Parameters

| Parameter Direction | How to Pass |
|-------------------|-------------|
| **In** (read only) | By value (cheap) or `const&` (expensive) |
| **Out** (write only) | By non-const reference |
| **In-out** (read+write) | By non-const reference |

> **Best Practice**: Avoid out parameters when possible — prefer return values. They're harder to understand.

---

## 12.14 Type Deduction with Pointers, References, and Const

```cpp
const int x{ 5 };
auto y{ x };           // int (const AND reference dropped)
auto& ref{ x };        // const int& (reference kept, const kept)
auto* ptr{ &x };       // const int* (pointer kept, const kept)

const auto z{ x };     // const int (const reapplied)
```

> **Key**: `auto` drops top-level const and references. `auto&` and `auto*` preserve them.

---

## 12.15 std::optional

```cpp
#include <optional>

std::optional<int> doDiv(int x, int y) {
    if (y == 0)
        return std::nullopt;   // no value
    return x / y;
}

auto result{ doDiv(10, 2) };
if (result.has_value())         // or just: if (result)
    std::cout << result.value(); // or *result
```

Better than returning sentinel values or using out parameters.

---

## Chapter 12 — Quick Reference Card

| Concept | Rule |
|---------|------|
| Cheap types (int, etc.) | Pass by value |
| Expensive types (string, etc.) | Pass by `const&` |
| Need to modify argument | Pass by non-const `&` |
| Need nullable | Use pointer or `std::optional` |
| `const` left of `*` | Value is const |
| `const` right of `*` | Pointer is const |
| Return local by ref | **NEVER** — dangling reference |
| Prefer | References over pointers |
