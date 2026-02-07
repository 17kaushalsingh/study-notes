# Chapter 7: Scope, Duration, and Linkage

## 7.1 Compound Statements (Blocks)

A **block** (compound statement) is a group of zero or more statements enclosed in `{}`:

```cpp
int add(int x, int y) {   // function body = block
    return x + y;
}

if (value) {               // if-block
    doA();
    doB();
}
```

- No semicolon needed after the closing `}`
- Blocks can be nested inside other blocks
- **Nesting level** = how deep the nesting goes

> **Best Practice**: Keep nesting to 3 levels or less. If deeper, refactor into sub-functions.

---

## 7.2 User-Defined Namespaces and the Scope Resolution Operator

### Creating Namespaces

```cpp
namespace Foo {
    int doSomething(int x, int y) {
        return x + y;
    }
}

namespace Goo {
    int doSomething(int x, int y) {
        return x - y;
    }
}

int main() {
    Foo::doSomething(4, 3);  // calls Foo's version
    Goo::doSomething(4, 3);  // calls Goo's version
    ::globalFunc();           // :: alone = global namespace
}
```

### Nested Namespaces (C++17)

```cpp
// C++17 shorthand:
namespace Foo::Goo {
    int add(int x, int y) { return x + y; }
}
// Access: Foo::Goo::add(1, 2);
```

### Identifier Resolution Order
1. Current namespace
2. Enclosing namespaces (outward)
3. Global namespace

> **Warning**: Never add custom functionality to the `std` namespace — this is undefined behavior.

---

## 7.3 Local Variables

- **Scope**: From point of declaration to end of enclosing block
- **Duration**: Automatic (created at definition, destroyed at end of block)
- **Linkage**: None (not visible outside the block)

```cpp
int main() {
    int x{ 5 };  // x created here
    {
        int y{ 3 };  // y created here
        // x and y both accessible
    }  // y destroyed here
    // only x accessible
}  // x destroyed here
```

> **Best Practice**: Define variables in the most limited scope possible.

---

## 7.4 Introduction to Global Variables

Global variables are declared **outside** any function:

```cpp
int g_x{};             // global variable (zero-initialized)
const int g_y{ 42 };   // global constant

int main() {
    g_x = 10;          // accessible anywhere after declaration
}
```

- **Scope**: From declaration to end of the file
- **Duration**: Static (created at program start, destroyed at program end)
- **Naming convention**: Prefix with `g_` (e.g., `g_mode`, `g_count`)

---

## 7.5 Variable Shadowing (Name Hiding)

When a variable in an inner scope has the same name as one in an outer scope, the inner variable **shadows** (hides) the outer one:

```cpp
int value{ 5 };    // global

int main() {
    int value{ 7 };        // shadows global 'value'
    std::cout << value;    // prints 7
    std::cout << ::value;  // prints 5 (global via ::)
}
```

> **Best Practice**: Avoid variable shadowing. Use different names to prevent confusion. Enable compiler warnings for shadowing (`-Wshadow`).

---

## 7.6 Internal Linkage

An identifier with **internal linkage** is visible only within its own translation unit (source file).

### Making Things Internal

```cpp
// Non-const globals: use static keyword
static int g_x{};                 // internal linkage
static int add(int x, int y);     // internal function

// const and constexpr: internal by default!
const int g_y{ 1 };               // internal by default
constexpr int g_z{ 2 };           // internal by default
```

> **Best Practice**: Give identifiers internal linkage when you have an explicit reason to disallow access from other files.

---

## 7.7 External Linkage and Variable Forward Declarations

An identifier with **external linkage** is visible across multiple files.

### Functions — External by Default

```cpp
// a.cpp
void sayHi() { std::cout << "Hi!\n"; }

// main.cpp
void sayHi();   // forward declaration
int main() { sayHi(); }
```

### Global Variables — Using extern

```cpp
// constants.cpp
int g_x{ 2 };                       // external by default (non-const)
extern const int g_y{ 3 };          // must use extern to make const external
extern constexpr int g_z{ 3 };      // external constexpr (limited utility)

// main.cpp (forward declarations — no initializer!)
extern int g_x;
extern const int g_y;
// constexpr CANNOT be forward-declared
```

> **Key Rule**: `extern` with an initializer = definition. `extern` without initializer = forward declaration.

---

## 7.8 Why (Non-Const) Global Variables Are Evil

Problems with non-const globals:
1. **Any function can modify them** — hard to track state changes
2. **Hidden dependencies** — function behavior depends on external state
3. **Testing difficulty** — can't test functions in isolation
4. **Debugging nightmare** — "who changed this value?"

> **Best Practice**: Avoid non-const global variables. Use local variables, pass parameters, or use classes to encapsulate state.

Acceptable globals: `const` / `constexpr` globals are fine (they can't be modified).

---

## 7.9 Inline Functions and Variables

### Modern Meaning

In modern C++, `inline` means **"multiple definitions are allowed"** (NOT "please inline this function"):

```cpp
// math.h
inline int add(int x, int y) {  // OK in header — inline allows multiple definitions
    return x + y;
}
```

### Inline Variables (C++17)

```cpp
// constants.h
inline constexpr double pi{ 3.14159 };  // can be #included in multiple files
```

> **Best Practice**: Don't use `inline` to request inline expansion — compilers decide this automatically.

---

## 7.10 Sharing Global Constants Across Multiple Files

### Best Approach (C++17): Inline constexpr in a Header

```cpp
// constants.h
#ifndef CONSTANTS_H
#define CONSTANTS_H

namespace Constants {
    inline constexpr double pi{ 3.14159265358979 };
    inline constexpr double gravity{ 9.8 };
    inline constexpr double avogadro{ 6.0221413e23 };
}

#endif
```

```cpp
// main.cpp
#include "constants.h"
double circumference{ 2.0 * radius * Constants::pi };
```

- All files share the **same** variable (not separate copies)
- Changes only require recompiling files that include the header
- Works with `constexpr` (compile-time evaluation)

---

## 7.11 Static Local Variables

Adding `static` to a local variable changes its duration from automatic to **static** (persists for the entire program):

```cpp
int generateID() {
    static int s_id{ 0 };   // initialized once, persists across calls
    return s_id++;
}

int main() {
    std::cout << generateID() << '\n';  // 0
    std::cout << generateID() << '\n';  // 1
    std::cout << generateID() << '\n';  // 2
}
```

- Initialization happens **only once** (first time the line is reached)
- Scope remains block-level (only accessible in the function)
- **Naming convention**: Prefix with `s_` (e.g., `s_count`)

> **Best Practice**: `const static` for caching expensive computations = good. Non-const static for state tracking = avoid (reduces predictability).

---

## 7.12 Scope, Duration, and Linkage — Master Summary Table

| Variable Type | Scope | Duration | Linkage |
|---------------|-------|----------|---------|
| Local variable `int x;` | Block | Automatic | None |
| Static local `static int s_x;` | Block | Static | None |
| Dynamic local `int* x{new int{}};` | Block | Dynamic | None |
| Function parameter `void f(int x)` | Block | Automatic | None |
| Non-const global `int g_x;` | Global | Static | External |
| Internal global `static int g_x;` | Global | Static | Internal |
| Const global `const int g_x{1};` | Global | Static | Internal |
| Extern const `extern const int g_x{1};` | Global | Static | External |
| Inline constexpr `inline constexpr int g_x{1};` | Global | Static | External |

---

## 7.13 Using Declarations and Using Directives

### Using Declaration (imports one name)

```cpp
using std::cout;
cout << "Hello\n";  // no need for std:: prefix
```

### Using Directive (imports entire namespace)

```cpp
using namespace std;  // imports EVERYTHING from std
cout << "Hello\n";
```

> **Best Practice**: Avoid `using namespace std;` (or any broad using directive) — it defeats the purpose of namespaces and can cause naming collisions. Prefer explicit `std::` prefix or targeted `using` declarations in limited scopes.

---

## 7.14 Unnamed and Inline Namespaces

### Unnamed Namespaces

```cpp
namespace {  // no name
    int doSomething() { return 42; }  // treated as if internal linkage
}
```

All content in an unnamed namespace is effectively given internal linkage. This is the modern C++ alternative to `static` for file-scoped declarations.

### Inline Namespaces

```cpp
inline namespace V2 {
    void doSomething() { /* v2 implementation */ }
}

namespace V1 {
    void doSomething() { /* v1 implementation */ }
}

doSomething();      // calls V2 (inline namespace is the default)
V1::doSomething();  // explicitly calls V1
```

Useful for API versioning — the inline namespace is the "default" version.

---

## Chapter 7 — Quick Reference Card

| Concept | Rule |
|---------|------|
| Variable scope | Define in the most limited scope possible |
| Global variables | Avoid non-const globals |
| Shared constants | Use `inline constexpr` in a header (C++17) |
| Static locals | Use `s_` prefix; prefer const static |
| `using namespace std;` | **Avoid** — use explicit `std::` |
| Internal linkage | `static` keyword or unnamed namespace |
| External linkage | `extern` keyword for forward declarations |
| Nesting depth | Keep to 3 levels max |
