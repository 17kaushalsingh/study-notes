# Chapter 11: Function Overloading and Function Templates

## 11.1 Introduction to Function Overloading

**Function overloading** allows multiple functions with the same name but different parameter types:

```cpp
int add(int x, int y) { return x + y; }
double add(double x, double y) { return x + y; }

add(1, 2);     // calls add(int, int)
add(1.5, 2.5); // calls add(double, double)
```

- Overloaded functions must differ in the **number** and/or **types** of parameters
- Return type alone is NOT sufficient to differentiate overloads
- Each overloaded function is a distinct function to the compiler

---

## 11.2 Function Overload Differentiation

### What Differentiates Overloads

| Differentiator | Example |
|---------------|---------|
| Number of parameters | `f(int)` vs `f(int, int)` |
| Parameter types | `f(int)` vs `f(double)` |
| Parameter type qualifiers | `f(int)` vs `f(const int&)` |

### What Does NOT Differentiate

| Not a differentiator | Reason |
|---------------------|--------|
| Return type only | `int f()` vs `double f()` — ambiguous at call site |
| Typedef aliases | `using Length = int;` — `f(int)` and `f(Length)` are the same |
| Top-level const on value params | `f(int)` and `f(const int)` are the same |

---

## 11.3 Function Overload Resolution

When you call an overloaded function, the compiler follows these steps:

### Resolution Steps
1. **Exact match**: No conversion needed
2. **Promotion match**: Numeric promotion (e.g., `char` → `int`)
3. **Numeric conversion match**: Other numeric conversions (e.g., `int` → `double`)
4. **User-defined conversion match**: Custom conversions
5. **Match via ellipsis**: C-style variadic
6. **No match found**: Compile error

### Ambiguous Matches

```cpp
void print(int x) { }
void print(double x) { }

print('a');   // OK: char promotes to int (step 2)
print(5L);    // AMBIGUOUS: long → int and long → double are both step 3
```

> Fix ambiguity by: using `static_cast`, adding a better overload, or using explicit template arguments.

---

## 11.4 Deleting Functions

Prevent specific function calls with `= delete`:

```cpp
void printInt(int x) { std::cout << x; }
void printInt(char) = delete;     // calling with char is compile error
void printInt(bool) = delete;     // calling with bool is compile error
```

Useful for preventing implicit conversions to specific overloads.

---

## 11.5 Default Arguments

```cpp
void print(int x, int y = 10, int z = 20) {
    std::cout << x << ' ' << y << ' ' << z << '\n';
}

print(1);          // 1 10 20
print(1, 2);       // 1 2 20
print(1, 2, 3);    // 1 2 3
```

### Rules
- Default arguments must be the **rightmost** parameters
- Cannot skip middle default arguments: `print(1, , 3)` is invalid
- If using forward declarations, defaults go in the **declaration** (not the definition)
- Default arguments are resolved at **compile time**

> **Best Practice**: Don't use default arguments when they make the function call ambiguous with other overloads.

---

## 11.6 Function Templates

Templates let you write one function that works with many types:

```cpp
template <typename T>
T max(T x, T y) {
    return (x < y) ? y : x;
}

max(3, 5);       // T = int → returns 5
max(3.5, 2.1);   // T = double → returns 3.5
```

### How It Works
1. You write a **template** with placeholder type `T`
2. When called, the compiler **instantiates** a concrete function with the actual type
3. Each unique type generates a separate function

### Template Syntax

```cpp
template <typename T>        // template parameter declaration
T functionName(T param) {    // T used as type
    // ...
}
```

- `typename` and `class` are interchangeable in template declarations — prefer `typename`
- Template parameters conventionally use single capital letters: `T`, `U`, `V`

---

## 11.7 Function Template Instantiation

### Implicit Instantiation (Argument Deduction)

```cpp
template <typename T>
T max(T x, T y) { return (x < y) ? y : x; }

max(1, 2);       // compiler deduces T = int
max<>(1, 2);     // empty <> also triggers deduction (prefers templates)
max<int>(1, 2);  // explicit specification
```

### Key Behaviors
- The compiler generates a separate function for each unique type
- Functions are only instantiated when actually called
- With normal syntax `max(1, 2)`, non-template overloads are preferred over templates

> **Best Practice**: Use normal function call syntax (no `<>`) — let the compiler deduce types.

---

## 11.8 Function Templates with Multiple Template Types

### Problem: Single T Can't Handle Mixed Types

```cpp
template <typename T>
T max(T x, T y) { return (x < y) ? y : x; }

max(2, 3.5);  // ERROR: can't deduce T (int vs double)
```

### Solution 1: Explicit Type

```cpp
max<double>(2, 3.5);  // forces T = double, int converts
```

### Solution 2: Multiple Template Parameters

```cpp
template <typename T, typename U>
auto max(T x, U y) {
    return (x < y) ? y : x;  // auto deduces return type
}

max(2, 3.5);  // T = int, U = double, returns double
```

### Solution 3: Abbreviated Function Templates (C++20)

```cpp
auto max(auto x, auto y) {
    return (x < y) ? y : x;
}
// Each auto parameter becomes a separate template type parameter
```

> This is shorthand — each `auto` parameter gets its own implicit template type.

---

## 11.9 Non-Type Template Parameters

Template parameters can be values, not just types:

```cpp
template <int N>
void printN() {
    std::cout << N << '\n';
}

printN<5>();   // prints 5
printN<100>(); // prints 100
```

- Non-type parameters must be **constexpr** (known at compile time)
- Common types: `int`, `std::size_t`, `bool`, pointers, enumerations
- Used extensively with `std::array`: `std::array<int, 5>`

```cpp
template <typename T, int size>
T average(const std::array<T, size>& arr) {
    T sum{ 0 };
    for (auto& val : arr)
        sum += val;
    return sum / size;
}
```

---

## 11.10 Using Function Templates in Multiple Files

Templates must be **visible in every file where they're used** — the full definition must be in the header file:

```cpp
// max.h
#ifndef MAX_H
#define MAX_H

template <typename T>
T max(T x, T y) {
    return (x < y) ? y : x;
}

#endif
```

> **Key Rule**: Template definitions go in **header files**, not .cpp files. The compiler needs to see the full definition to instantiate.

---

## Chapter 11 — Quick Reference Card

| Concept | Rule |
|---------|------|
| Overload differentiation | Number/types of params (NOT return type) |
| Ambiguous calls | Fix with `static_cast` or explicit templates |
| `= delete` | Prevents unwanted implicit conversions |
| Default arguments | Must be rightmost; put in declaration |
| Template syntax | `template <typename T>` (prefer over `class`) |
| Template deduction | Let compiler deduce: `max(1, 2)` not `max<int>(1, 2)` |
| Mixed types | Use multiple template params or C++20 `auto` params |
| Template location | Full definition in header files |
| Non-type params | Must be constexpr |
