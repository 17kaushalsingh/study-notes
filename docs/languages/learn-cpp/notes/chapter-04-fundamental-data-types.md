# Chapter 4: Fundamental Data Types

## 4.1 Introduction to Fundamental Data Types

C++ provides a set of built-in (**fundamental**) data types that tell the compiler how to interpret memory contents.

### The Complete Table of Fundamental Types

| Category | Types | Purpose | Example |
|----------|-------|---------|---------|
| Floating Point | `float`, `double`, `long double` | Numbers with fractional parts | `3.14159` |
| Boolean | `bool` | True/false values | `true` |
| Character | `char`, `wchar_t`, `char8_t` (C++20), `char16_t`, `char32_t` | Single text characters | `'c'` |
| Integer | `short`, `int`, `long`, `long long` | Whole numbers | `64` |
| Null Pointer | `std::nullptr_t` | Null pointer representation | `nullptr` |
| Void | `void` | No type / incomplete type | n/a |

### Important Terminology

- **Integral types**: Includes `bool`, character types, AND integer types — all stored as integers in memory
- **Standard integer types**: Only `short`, `int`, `long`, `long long` (and their unsigned variants)
- The `_t` suffix means "type" (e.g., `std::nullptr_t`, `size_t`)

---

## 4.2 Void

- `void` means "no type"
- Most commonly used for functions that don't return a value
- You **cannot** declare variables of type `void`
- Other uses covered later (void pointers)

---

## 4.3 Object Sizes and the sizeof Operator

### Memory Basics
- A **bit** is the smallest unit (0 or 1)
- A **byte** = 8 bits (standard addressable unit)
- Each byte has a unique memory address

### sizeof Operator

```cpp
#include <iostream>

int main() {
    std::cout << "bool:\t\t" << sizeof(bool) << " bytes\n";
    std::cout << "char:\t\t" << sizeof(char) << " bytes\n";
    std::cout << "int:\t\t" << sizeof(int) << " bytes\n";
    std::cout << "double:\t\t" << sizeof(double) << " bytes\n";
    return 0;
}
```

- `sizeof` returns a value of type `std::size_t` (an unsigned integer)
- `sizeof(char)` is always 1 byte

---

## 4.4 Signed Integers

### Types and Minimum Sizes

| Type | Minimum Size | Typical Size |
|------|-------------|--------------|
| `short` | 16 bits | 16 bits |
| `int` | 16 bits | **32 bits** |
| `long` | 32 bits | 32 or 64 bits |
| `long long` | 64 bits | 64 bits |

### Ranges (by size)

| Size | Range |
|------|-------|
| 8-bit | -128 to 127 |
| 16-bit | -32,768 to 32,767 |
| 32-bit | -2,147,483,648 to 2,147,483,647 |
| 64-bit | -9.2 quintillion to 9.2 quintillion |

Formula: **-(2^(n-1))** to **(2^(n-1)) - 1**

### Integer Division

```cpp
int x{ 8 };
int y{ 5 };
std::cout << x / y; // prints 1, NOT 1.6 (fractional part dropped!)
```

> **Warning**: Integer division drops the fractional part entirely — it does NOT round.

### Overflow = Undefined Behavior

```cpp
int x{ 2147483647 }; // max value for 32-bit int
x = x + 1;           // UNDEFINED BEHAVIOR - overflow!
```

> **Best Practice**: Use `int` when the value fits. Use `long long` when you need larger values.

---

## 4.5 Unsigned Integers, and Why to Avoid Them

### Declaration

```cpp
unsigned short us;
unsigned int ui;
unsigned long ul;
unsigned long long ull;
```

### Range

An n-bit unsigned integer holds: **0** to **(2^n) - 1**
- 8-bit: 0 to 255
- 16-bit: 0 to 65,535

### The Wraparound Problem

Unlike signed overflow (undefined behavior), unsigned overflow wraps around:

```cpp
unsigned short x{ 65535 }; // max 16-bit value
x = 65536;                  // wraps to 0
x = 65537;                  // wraps to 1

unsigned short y{ 0 };
y = -1;                     // wraps to 65535!
```

### Why to AVOID Unsigned

**Problem 1: Easy underflow**
```cpp
unsigned int x{ 2 };
unsigned int y{ 3 };
std::cout << x - y; // prints 4294967295 (NOT -1!)
```

**Problem 2: Mixed signed/unsigned comparisons**
```cpp
signed int s{ -1 };
unsigned int u{ 1 };
if (s < u)  // FALSE! -1 converts to 4294967295
    std::cout << "this won't print";
```

> **Best Practice**: Favor signed numbers for quantities and math operations. Avoid mixing signed and unsigned.

### Acceptable Uses of Unsigned
- Bit manipulation
- Algorithms requiring wraparound (encryption, hashing)
- Array indexing (when required by APIs)
- Embedded systems with tight memory

---

## 4.6 Fixed-Width Integers and size_t

### Fixed-Width Types (from `<cstdint>`)

| Type | Size | Range |
|------|------|-------|
| `std::int8_t` | 1 byte | -128 to 127 |
| `std::int16_t` | 2 bytes | -32,768 to 32,767 |
| `std::int32_t` | 4 bytes | -2.1B to 2.1B |
| `std::int64_t` | 8 bytes | ±9.2 quintillion |

Unsigned versions: `std::uint8_t`, `std::uint16_t`, `std::uint32_t`, `std::uint64_t`

```cpp
#include <cstdint>

std::int32_t x{ 32767 };
x = x + 1; // Always safe — guaranteed 32-bit
```

> **Warning**: `std::int8_t` and `std::uint8_t` behave like `char` on most systems — printing them shows characters, not numbers!

### Fast and Least Types (Avoid)

- `std::int_fast32_t` — fastest type with at least 32 bits
- `std::int_least32_t` — smallest type with at least 32 bits
- These are implementation-defined — avoid for portable code

### std::size_t

```cpp
#include <cstddef>
std::size_t s{ sizeof(int) }; // sizeof returns std::size_t
```

- Unsigned integral type for object sizes
- 32-bit on 32-bit systems, 64-bit on 64-bit systems

> **Best Practice**:
> - Use `int` for values that always fit
> - Use `std::int#_t` when you need guaranteed size
> - Avoid `short`, `long` — prefer fixed-width types

---

## 4.7 Introduction to Scientific Notation

- `1.2e4` = 1.2 × 10^4 = 12000
- `5.0e-2` = 5.0 × 10^-2 = 0.05
- Used internally by floating-point numbers

---

## 4.8 Floating Point Numbers

### Types and Precision

| Type | Size | Significant Digits |
|------|------|--------------------|
| `float` | 4 bytes | 6-9 digits |
| `double` | 8 bytes | 15-18 digits |
| `long double` | 8-16 bytes | 15-36 digits |

### Rounding Errors — A Critical Concept

```cpp
double d1{ 1.0 };
double d2{ 0.1 + 0.1 + 0.1 + 0.1 + 0.1 + 0.1 + 0.1 + 0.1 + 0.1 + 0.1 };
// d2 == 0.99999999999999989, NOT exactly 1.0!
```

> **Key Takeaway**: Many decimal numbers (like 0.1) cannot be represented exactly in binary. Never assume floating-point numbers are exact!

### Special Values (IEEE 754)

- **Inf** / **-Inf**: Result of dividing by zero (e.g., `5.0 / 0.0`)
- **NaN**: "Not a Number" (e.g., `0.0 / 0.0`)
- **NaN is not equal to anything**, including itself

> **Best Practice**: Favor `double` over `float`. Float's limited precision causes frequent inaccuracies. Never compare floating-point values directly for equality.

---

## 4.9 Boolean Values

```cpp
bool b1{ true };   // stored as 1
bool b2{ false };  // stored as 0
```

### Printing Booleans

```cpp
std::cout << true;                  // prints 1
std::cout << false;                 // prints 0

std::cout << std::boolalpha;       // enable text output
std::cout << true;                  // prints "true"
std::cout << false;                 // prints "false"
std::cout << std::noboolalpha;     // back to numeric
```

### Input

```cpp
bool b{};
std::cin >> b;                     // only accepts 0 or 1

std::cin >> std::boolalpha;
std::cin >> b;                     // accepts "true" or "false" (case-sensitive)
```

### Negation

```cpp
bool b1{ !true };   // false
bool b2{ !false };  // true
```

---

## 4.10 Introduction to if Statements

```cpp
if (condition)
    statement_true;
else
    statement_false;
```

- Condition evaluates to `true` (non-zero) or `false` (zero)
- Non-zero integers implicitly convert to `true`
- Zero converts to `false`

```cpp
if (4)  // 4 is non-zero, so this is true
    std::cout << "this prints\n";
```

---

## 4.11 Chars

### ASCII Table (Key Values)

| Code | Character | Code | Character |
|------|-----------|------|-----------|
| 48-57 | '0' to '9' | 65-90 | 'A' to 'Z' |
| 97-122 | 'a' to 'z' | 32 | space |

### Initialization

```cpp
char ch1{ 'a' };    // preferred — character literal
char ch2{ 97 };     // works but less readable
// Note: char ch{5} is NOT the same as char ch{'5'} (ASCII 53)!
```

### Escape Sequences

| Sequence | Meaning |
|----------|---------|
| `\n` | Newline |
| `\t` | Tab |
| `\\` | Backslash |
| `\'` | Single quote |
| `\"` | Double quote |
| `\0` | Null terminator |

### Single vs Double Quotes

```cpp
char ch{ 'a' };      // single quotes = char literal (one character)
const char* s{ "a" }; // double quotes = string literal (char sequence + \0)
```

### Reading Input

```cpp
char ch{};
std::cin >> ch;           // reads one char, skips leading whitespace
std::cin.get(ch);         // reads one char INCLUDING whitespace
```

---

## 4.12 Introduction to Type Conversion and static_cast

### Implicit Conversion

The compiler automatically converts between types when needed:

```cpp
void printDouble(double d) { std::cout << d; }
printDouble(5);  // int 5 implicitly converted to double 5.0
```

> **Warning**: Converting `double` to `int` loses the fractional part and generates a compiler warning.

### Explicit Conversion with static_cast

```cpp
static_cast<new_type>(expression)
```

```cpp
char ch{ 97 };
std::cout << ch;                        // prints 'a'
std::cout << static_cast<int>(ch);      // prints 97

double d{ 5.5 };
int i{ static_cast<int>(d) };          // i = 5, no warning
```

> **Best Practice**:
> - Use `static_cast` when you intentionally want to convert types
> - Prefer brace initialization `{}` to catch unsafe implicit conversions at compile time

---

## Chapter 4 — Quick Reference Card

| Concept | Recommendation |
|---------|---------------|
| Default integer | Use `int` |
| Large integers | Use `long long` |
| Guaranteed size | Use `std::int32_t` etc. |
| Floating point | Use `double` (not `float`) |
| Unsigned integers | **Avoid** for quantities |
| Type conversion | Use `static_cast<>()` |
| Float comparison | **Never** use `==` directly |
| Integer division | Drops fractional part (not rounded) |
