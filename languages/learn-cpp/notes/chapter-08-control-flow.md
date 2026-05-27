# Chapter 8: Control Flow

## 8.1 Control Flow Introduction

By default, statements execute sequentially (top to bottom). **Control flow statements** alter this order:

| Category | Statements |
|----------|-----------|
| Conditional | `if`, `else`, `switch` |
| Loops | `while`, `do-while`, `for`, range-based `for` |
| Jumps | `break`, `continue`, `return`, `goto` |
| Halts | `std::exit()`, `std::abort()` |
| Exceptions | `try`, `catch`, `throw` |

---

## 8.2-8.3 If Statements and Common Problems

```cpp
if (condition) {
    // executes if true
} else if (otherCondition) {
    // executes if first false, this true
} else {
    // executes if all above false
}
```

### Common Mistakes

```cpp
// MISTAKE 1: Using = instead of ==
if (x = 5)      // ASSIGNS 5 to x, always true!
if (x == 5)     // CORRECT: compares x to 5

// MISTAKE 2: Dangling else
if (x >= 0)
    if (x <= 20)
        std::cout << x << " is between 0 and 20\n";
else  // This else belongs to inner if, not outer!
    std::cout << x << " is negative\n";

// FIX: Use braces!
if (x >= 0) {
    if (x <= 20)
        std::cout << x << " is between 0 and 20\n";
} else {
    std::cout << x << " is negative\n";
}
```

### Null Statements

```cpp
if (condition);    // semicolon = null statement, body does nothing!
```

---

## 8.4 Constexpr If Statements (C++17)

```cpp
constexpr double gravity{ 9.8 };

if constexpr (gravity == 9.8)     // evaluated at COMPILE TIME
    std::cout << "Earth\n";
else if constexpr (gravity == 1.6)
    std::cout << "Moon\n";
```

- Condition must be a constant expression
- Non-matching branches are **not compiled** (discarded entirely)
- Useful in templates to conditionally compile code based on types

---

## 8.5-8.6 Switch Statements

```cpp
switch (expression) {    // expression must be integral or enum type
    case value1:
        // statements
        break;           // exits the switch
    case value2:
    case value3:         // fallthrough: handles both value2 and value3
        // statements
        break;
    default:
        // if no case matches
        break;
}
```

### Key Rules
- Case values must be **constant expressions** (literals or constexpr)
- All case values must be **unique**
- Without `break`, execution **falls through** to the next case
- Use `[[fallthrough]]` attribute to indicate intentional fallthrough (suppresses warnings)

### Switch Scoping

```cpp
switch (x) {
    case 1:
        int y{ 5 };  // ERROR: can't initialize variable in a case
        break;
    case 2:
        // y is in scope but may be uninitialized!
        break;
}

// FIX: Use a block
switch (x) {
    case 1: {            // explicit block
        int y{ 5 };      // OK
        break;
    }
    case 2:
        break;
}
```

---

## 8.7 Goto Statements

```cpp
goto labelName;     // jumps to label
// ...
labelName:          // label declaration
    statement;
```

> **Best Practice**: **Avoid goto**. It creates spaghetti code. Use loops, break, continue, or functions instead.

---

## 8.8 While Loops

```cpp
while (condition) {
    // body executes while condition is true
    // condition checked BEFORE each iteration
}
```

### Intentional Infinite Loops

```cpp
while (true) {
    // runs forever until break/return
    if (done)
        break;
}
```

> **Best Practice**: Use signed integers for loop counters (unsigned can cause infinite loops when counting down).

---

## 8.9 Do-While Loops

```cpp
do {
    // body executes at least ONCE
    // condition checked AFTER each iteration
} while (condition);   // note the semicolon!
```

Use when you need the body to execute before checking the condition (e.g., input validation):

```cpp
int choice{};
do {
    std::cout << "Enter 1 or 2: ";
    std::cin >> choice;
} while (choice != 1 && choice != 2);
```

---

## 8.10 For Loops

```cpp
for (init; condition; end-expression) {
    // body
}

// Equivalent while loop:
{
    init;
    while (condition) {
        // body
        end-expression;
    }
}
```

### Examples

```cpp
// Count 0-9
for (int i{ 0 }; i < 10; ++i)
    std::cout << i << ' ';

// Count down
for (int i{ 9 }; i >= 0; --i)
    std::cout << i << ' ';

// Step by 2
for (int i{ 0 }; i <= 10; i += 2)
    std::cout << i << ' ';

// Multiple variables
for (int x{ 0 }, y{ 9 }; x < 10; ++x, --y)
    std::cout << x << ' ' << y << '\n';
```

> **Best Practice**: Prefer `<` over `!=` in for-loop conditions to avoid infinite loops if the counter skips the target value.

### When to Use For vs While
- **For loop**: When you have an obvious counter variable
- **While loop**: When there's no counter (e.g., event-driven loops)

---

## 8.11 Break and Continue

### Break

```cpp
while (true) {
    int x{};
    std::cin >> x;
    if (x == 0)
        break;           // exits the loop entirely
    std::cout << x << '\n';
}
```

### Continue

```cpp
for (int i{ 0 }; i < 10; ++i) {
    if (i % 2 == 0)
        continue;         // skips to next iteration
    std::cout << i << ' '; // prints odd numbers only
}
```

> **Warning**: In `while`/`do-while`, `continue` skips to the condition check — make sure counter updates happen before the `continue` or you'll get an infinite loop!

---

## 8.12 Halts (Exiting Early)

```cpp
#include <cstdlib>

std::exit(0);      // clean exit (destructors for static objects called)
std::abort();      // abnormal termination (no cleanup)
std::terminate();  // called for unhandled exceptions (calls abort)
```

> **Best Practice**: Only use `std::exit()` or `std::abort()` if there's no clean way to return from `main()`. Prefer returning from `main()` for normal termination.

---

## 8.13-8.15 Random Number Generation

### The Problem with rand()

```cpp
#include <cstdlib>
std::srand(static_cast<unsigned int>(std::time(nullptr)));
int x{ std::rand() };  // OLD way — avoid!
```

Problems: low quality randomness, limited range, biased distribution.

### Modern C++: Mersenne Twister (Recommended)

```cpp
#include <random>

// Create and seed the generator
std::mt19937 mt{ std::random_device{}() };

// Create a distribution
std::uniform_int_distribution die6{ 1, 6 };  // inclusive range [1, 6]

// Generate random numbers
for (int i{ 0 }; i < 10; ++i)
    std::cout << die6(mt) << ' ';
```

### Better Seeding with seed_seq

```cpp
std::random_device rd{};
std::seed_seq ss{ rd(), rd(), rd(), rd(), rd(), rd(), rd(), rd() };
std::mt19937 mt{ ss };
```

### Global Random Number Helper (Random.h pattern)

```cpp
// Random.h
#ifndef RANDOM_H
#define RANDOM_H

#include <random>

namespace Random {
    inline std::mt19937 generate() {
        std::random_device rd{};
        std::seed_seq ss{ rd(), rd(), rd(), rd(), rd(), rd(), rd(), rd() };
        return std::mt19937{ ss };
    }

    inline std::mt19937 mt{ generate() };

    inline int get(int min, int max) {
        return std::uniform_int_distribution{ min, max }(mt);
    }
}

#endif
```

Usage:
```cpp
#include "Random.h"
int roll{ Random::get(1, 6) };
```

> **Key Rules**:
> - Seed only once (at program start)
> - Don't recreate the generator for each random number
> - Use a fixed seed during debugging for reproducible results

---

## Chapter 8 — Quick Reference Card

| Concept | Rule |
|---------|------|
| if with `=` | Use `==` for comparison, not `=` |
| Switch expression | Must be integral or enum type |
| Switch fallthrough | Use `break` or `[[fallthrough]]` |
| Goto | **Avoid entirely** |
| Loop counters | Use signed integers |
| For vs While | For = counter; While = no counter |
| Random numbers | Use `std::mt19937` + distributions |
| `std::rand()` | **Avoid** — use `<random>` instead |
