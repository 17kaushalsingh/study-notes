# Chapter 5: Constants and Strings

## 5.1 Constant Variables (Named Constants)

A **constant** is a value that cannot be changed after initialization.

### Declaration

```cpp
const double gravity{ 9.8 };     // preferred: const before type
int const sidesInSquare{ 4 };    // "east const" style — less common
```

- Const variables **must** be initialized at declaration
- Values cannot be modified afterward
- Initializers CAN be non-constant (runtime) values

### Naming Conventions
- C-style: `EARTH_GRAVITY` (uppercase + underscores)
- C++ k-style: `kEarthGravity`
- **Recommended**: `earthGravity` (standard camelCase, since const vars behave like regular vars)

### Best Practices

> **Make variables const whenever possible** — reduces bugs, enables compiler optimizations.

- **Don't** use `const` on pass-by-value function parameters (adds clutter)
- **Don't** use `const` on return-by-value (compilers ignore it, may impede optimizations)
- **Prefer** `const` variables over `#define` macros (macros ignore scoping, can't be debugged)

---

## 5.2 Literals

Literals are values inserted directly into source code.

### Literal Suffixes

| Suffix | Type | Example |
|--------|------|---------|
| (none) | `int` | `42` |
| `u` or `U` | `unsigned int` | `42u` |
| `l` or `L` | `long` | `42L` |
| `ll` or `LL` | `long long` | `42LL` |
| `f` or `F` | `float` | `3.14f` |
| (none) | `double` | `3.14` |
| `L` | `long double` | `3.14L` |

> **Best Practice**: Use suffix `f` for float literals to avoid implicit double-to-float conversion.

### Magic Numbers

Avoid "magic numbers" — unexplained literal values in code. Use named constants instead:

```cpp
// Bad:
int maxStudents{ 30 };

// Good:
constexpr int maxClassSize{ 30 };
int maxStudents{ maxClassSize };
```

---

## 5.3 Numeral Systems

```cpp
int decimal     { 42 };      // base 10 (default)
int octal       { 052 };     // base 8 (prefix: 0)
int hex         { 0x2A };    // base 16 (prefix: 0x)
int binary      { 0b101010 };// base 2 (prefix: 0b, C++14)
```

### Digit Separators (C++14)

```cpp
int value{ 1'000'000 };       // easier to read, separators ignored
int bin{ 0b1010'0010 };       // works in any base
```

### Output in Different Bases

```cpp
std::cout << std::dec << 42 << '\n';  // 42
std::cout << std::oct << 42 << '\n';  // 52
std::cout << std::hex << 42 << '\n';  // 2a
```

---

## 5.4 The As-If Rule and Compile-Time Optimization

The **as-if rule** allows compilers to modify code in any way that doesn't change observable behavior.

Common optimizations:
- **Constant folding**: `3 + 4` replaced with `7` at compile time
- **Constant propagation**: Known values substituted where used
- **Dead code elimination**: Unreachable code removed

---

## 5.5 Constant Expressions

A **constant expression** is an expression that can be evaluated at compile time.

Requirements for a variable to be usable in a constant expression:
1. It must be `const` (or `constexpr`)
2. Its initializer must itself be a constant expression

```cpp
const int x{ 5 };          // constant expression (const + compile-time initializer)
const int y{ x + 1 };      // constant expression (x is a constant expression)

int z{ 5 };
const int w{ z };           // NOT a constant expression (z is not const)
```

---

## 5.6 Constexpr Variables

### const vs constexpr

| Aspect | `const` | `constexpr` |
|--------|---------|-------------|
| When value known | Compile-time **or** runtime | **Must** be compile-time |
| Compile-time use | Not guaranteed | Always guaranteed |
| Type support | All types | Must be literal types |
| Relationship | Not implicitly constexpr | Implicitly const |

### Usage

```cpp
constexpr double gravity{ 9.8 };     // OK: compile-time value
constexpr int sum{ 4 + 5 };          // OK: constant expression
constexpr int something{ sum };       // OK: constexpr is a constant expression

int age{};
std::cin >> age;
constexpr int myAge{ age };           // COMPILE ERROR: age not known at compile time
const int myAge2{ age };              // OK: const can hold runtime values
```

> **Best Practice**:
> - Use `constexpr` when the initializer is a constant expression
> - Use `const` when the value is determined at runtime
> - `constexpr` variables **cannot** be used with types like `std::string` or `std::vector`

---

## 5.7 Introduction to std::string

### Header

```cpp
#include <string>
```

### Creation and Assignment

```cpp
std::string name{};              // empty string
std::string greeting{ "Hello" }; // initialized with C-string literal
greeting = "World";              // reassignment
```

### Output and Input

```cpp
std::cout << greeting << '\n';   // output works normally

// Input with >>  (reads one word, stops at whitespace):
std::string name{};
std::cin >> name;                // "John Smith" → name = "John"

// Input with getline (reads full line):
std::getline(std::cin >> std::ws, name);  // "John Smith" → name = "John Smith"
```

> **Key**: `std::ws` discards leading whitespace. Always use it with `std::getline` after `std::cin >>` to avoid issues with leftover newlines in the buffer.

### String Length

```cpp
std::string name{ "Hello" };
std::cout << name.length();     // 5 (returns std::size_t — unsigned!)
// Use static_cast<int>(name.length()) to avoid signed/unsigned warnings
```

### String Concatenation

```cpp
std::string a{ "Hello" };
std::string b{ " World" };
std::string c{ a + b };         // "Hello World"
// Note: at least one operand must be std::string
// "Hello" + " World"  → ERROR (two C-string literals can't be added)
```

### String Literals Suffix

```cpp
using namespace std::string_literals;
auto s{ "Hello"s };             // type is std::string (not const char*)
```

> **Best Practice**: Don't pass `std::string` by value (expensive copy). Use `std::string_view` for read-only parameters.

---

## 5.8 Introduction to std::string_view

### Header

```cpp
#include <string_view>
```

### Why It Exists

`std::string` copies data on creation and when passed by value. `std::string_view` provides a **read-only, non-owning view** of an existing string — zero copy cost.

### Comparison

| Aspect | `std::string` | `std::string_view` |
|--------|---------------|---------------------|
| Ownership | Owns its data | Non-owning view |
| Copying | Expensive | Zero-cost (just a pointer + length) |
| Mutability | Mutable | Read-only |
| Lifetime | Independent | Depends on source string |
| `constexpr` | No | **Yes** |

### Usage

```cpp
#include <string_view>

void printSV(std::string_view str) {  // no copy!
    std::cout << str << '\n';
}

int main() {
    std::string_view sv{ "Hello, world!" };    // views the string literal
    printSV(sv);

    std::string s{ "Hello" };
    std::string_view sv2{ s };                  // views the std::string
    printSV(sv2);

    printSV("literal");                         // views the literal directly
    return 0;
}
```

### Implicit Conversion Rules

- `std::string` → `std::string_view`: **implicit** (cheap)
- `std::string_view` → `std::string`: **NOT implicit** (would be expensive copy)
  - Must use explicit conversion: `std::string{ sv }` or `static_cast<std::string>(sv)`

> **Best Practice**: Prefer `std::string_view` over `std::string` for function parameters that only need to read the string.

---

## 5.9 std::string_view (Part 2)

### Lifetime Dangers

`std::string_view` does NOT own its data. If the source is destroyed, the view **dangles**:

```cpp
std::string_view getView() {
    std::string s{ "Hello" };
    return std::string_view{ s };  // DANGER: s destroyed at end of function
}                                   // returned view is now dangling!
```

> **Warning**: Never return a `std::string_view` that refers to a local `std::string` variable.

### View Modification

You can shrink the view (but not the underlying string):

```cpp
std::string_view sv{ "Hello, world!" };
sv.remove_prefix(7);   // sv is now "world!"
sv.remove_suffix(1);   // sv is now "world"
// Original string is unchanged
```

### When to Use What

| Situation | Use |
|-----------|-----|
| Read-only function parameter | `std::string_view` |
| Need to modify the string | `std::string` |
| Need ownership of the data | `std::string` |
| Compile-time string constant | `constexpr std::string_view` |
| Return from function | `std::string` (usually) |

---

## Chapter 5 — Quick Reference Card

| Concept | Recommendation |
|---------|---------------|
| Named constants | Use `constexpr` for compile-time, `const` for runtime |
| Magic numbers | Replace with named constants |
| String parameters | Use `std::string_view` (read-only) |
| String ownership | Use `std::string` |
| Compile-time strings | `constexpr std::string_view` |
| Full-line input | `std::getline(std::cin >> std::ws, str)` |
| `#define` constants | **Avoid** — use `const`/`constexpr` instead |
