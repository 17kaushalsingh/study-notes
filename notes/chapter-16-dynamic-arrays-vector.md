# Chapter 16: Dynamic Arrays — std::vector

## 16.1 Introduction to Containers and Arrays

**Containers** are data types that hold a collection of unnamed objects (elements).

**Arrays** are containers where elements are stored contiguously in memory, allowing direct access by index.

| Container | Size | When to use |
|-----------|------|-------------|
| `std::vector` | Dynamic (resizable) | Default choice |
| `std::array` | Fixed at compile time | When size is known and constexpr |
| C-style array | Fixed | Legacy code only |

---

## 16.2 Introduction to std::vector and List Constructors

```cpp
#include <vector>

// Empty vector
std::vector<int> empty{};

// List initialization
std::vector<int> primes{ 2, 3, 5, 7, 11 };

// CTAD (C++17) — compiler deduces type
std::vector data{ 1.0, 2.0, 3.0 };  // std::vector<double>

// Create vector of specific length (parentheses, NOT braces!)
std::vector<int> zeros(10);   // 10 elements, all 0
// std::vector<int> v{10};    // 1 element with value 10! (list constructor wins)
```

### Accessing Elements

```cpp
std::vector<int> v{ 10, 20, 30, 40 };
v[0];           // 10 — no bounds checking (undefined behavior if out of bounds!)
v.at(1);        // 20 — WITH bounds checking (throws exception)
v.front();      // 10 — first element
v.back();       // 40 — last element
```

> **Warning**: `operator[]` does NO bounds checking. For safety, use `.at()` or check the index first.

---

## 16.3 The Unsigned Length and Subscript Problem

`std::vector::size()` returns `std::size_t` (unsigned). Mixing with signed integers causes warnings:

```cpp
std::vector<int> v{ 1, 2, 3 };
for (int i{ 0 }; i < v.size(); ++i)  // WARNING: signed/unsigned comparison
```

### Solutions

```cpp
// Solution 1: Use std::size_t for the index
for (std::size_t i{ 0 }; i < v.size(); ++i)

// Solution 2: Use static_cast
for (int i{ 0 }; i < static_cast<int>(v.size()); ++i)

// Solution 3: Use std::ssize() (C++20) — returns signed size
for (auto i{ 0 }; i < std::ssize(v); ++i)

// Solution 4: Range-based for loop (best when you don't need the index)
for (auto& elem : v)
```

---

## 16.4 Passing std::vector

```cpp
// Pass by const reference (read-only) — preferred
void print(const std::vector<int>& v) {
    for (auto e : v) std::cout << e << ' ';
}

// Pass by reference (modifiable)
void doubleAll(std::vector<int>& v) {
    for (auto& e : v) e *= 2;
}
```

> **Best Practice**: Pass vectors by `const&` for read-only, by `&` when you need to modify.

---

## 16.5 Returning std::vector and Move Semantics

```cpp
std::vector<int> generate(int count) {
    std::vector<int> v(count);
    // fill v...
    return v;  // move semantics — efficient, no copy!
}
```

- Returning by value is fine — the compiler uses **move semantics** or **copy elision**
- The vector's internal data is "moved" (pointer swap), not copied

---

## 16.6-16.7 Arrays and Loops

```cpp
// Traditional for loop
for (std::size_t i{ 0 }; i < v.size(); ++i)
    std::cout << v[i] << ' ';

// Range-based for loop (preferred when index not needed)
for (const auto& elem : v)
    std::cout << elem << ' ';
```

---

## 16.8 Range-Based For Loops (for-each)

```cpp
for (element_declaration : container)
    statement;
```

### Best Practices for Element Declaration

| Declaration | Use when |
|-------------|----------|
| `auto elem` | Cheap-to-copy types, don't need to modify |
| `auto& elem` | Need to modify elements |
| `const auto& elem` | Expensive-to-copy types, read-only |

```cpp
std::vector<std::string> names{ "Alice", "Bob", "Charlie" };

// Const reference — no copies, no modification
for (const auto& name : names)
    std::cout << name << '\n';

// Reference — allows modification
for (auto& name : names)
    name = "Replaced";
```

> Range-based for does NOT give you the index. Use a traditional for loop if you need it.

---

## 16.9 Array Indexing and Length Using Enumerators

```cpp
enum StudentName {
    alice, bob, charlie, max_students
};

std::vector<int> scores(max_students);
scores[alice] = 95;
scores[bob] = 87;
```

Enumerators as indices make code more readable and maintainable.

---

## 16.10 std::vector Resizing and Capacity

### Length vs Capacity

| Property | Meaning | Query |
|----------|---------|-------|
| Length (size) | Elements currently in use | `v.size()` |
| Capacity | Memory allocated for elements | `v.capacity()` |

```cpp
std::vector<int> v{ 1, 2, 3 };
v.size();      // 3
v.capacity();  // >= 3 (implementation-defined)
```

### Resizing

```cpp
v.resize(5);      // length → 5 (new elements value-initialized to 0)
v.resize(2);      // length → 2 (elements removed from end)
```

### Reserve (Pre-allocate)

```cpp
v.reserve(100);   // allocate space for 100 elements (no size change)
```

> **Key**: Reallocation (when capacity increases) is expensive — it copies/moves ALL elements. Use `reserve()` if you know the final size.

### Shrink

```cpp
v.shrink_to_fit();  // request to reduce capacity to match size (non-binding)
```

---

## 16.11 std::vector and Stack Behavior

```cpp
std::vector<int> stack{};

stack.push_back(1);    // add to end: [1]
stack.push_back(2);    // add to end: [1, 2]
stack.push_back(3);    // add to end: [1, 2, 3]

stack.back();          // peek at top: 3
stack.pop_back();      // remove from end: [1, 2]

stack.emplace_back(4); // construct in-place (preferred over push_back for complex types)
```

---

## 16.12 std::vector<bool>

> **Warning**: `std::vector<bool>` is a special case — it's a packed bitfield, NOT a true vector of bools. It behaves differently from other vectors. Avoid it if possible; use `std::vector<char>` or `std::bitset` instead.

---

## Chapter 16 — Quick Reference Card

| Concept | Rule |
|---------|------|
| Default container | Use `std::vector` |
| Initialization | Use `{}` for values, `()` for count |
| Element access | `[]` = no bounds check; `.at()` = bounds check |
| Pass to function | By `const&` (read) or `&` (modify) |
| Return from function | By value (move semantics handles it) |
| Range-based for | `const auto&` for read-only |
| Pre-allocate | `reserve()` to avoid reallocation |
| Add elements | `push_back()` or `emplace_back()` |
| `vector<bool>` | **Avoid** — use `vector<char>` instead |
