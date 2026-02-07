# Chapter 6: Operators

## 6.1 Operator Precedence and Associativity

### Precedence Table (Key Levels)

| Level | Assoc. | Operators | Description |
|-------|--------|-----------|-------------|
| 1 | L→R | `::` | Scope resolution |
| 2 | L→R | `()` `[]` `.` `->` postfix `++`/`--` | Postfix / function call |
| 3 | R→L | prefix `++`/`--` unary `+`/`-` `!` `~` `*` `&` `sizeof` `new` `delete` | Unary / prefix |
| 5 | L→R | `*` `/` `%` | Multiplicative |
| 6 | L→R | `+` `-` | Additive |
| 7 | L→R | `<<` `>>` | Bitwise shift / stream I/O |
| 9 | L→R | `<` `<=` `>` `>=` | Relational |
| 10 | L→R | `==` `!=` | Equality |
| 11 | L→R | `&` | Bitwise AND |
| 12 | L→R | `^` | Bitwise XOR |
| 13 | L→R | `\|` | Bitwise OR |
| 14 | L→R | `&&` | Logical AND |
| 15 | L→R | `\|\|` | Logical OR |
| 16 | R→L | `?:` `=` `+=` `-=` `*=` `/=` `%=` | Ternary / Assignment |
| 17 | L→R | `,` | Comma |

Lower number = higher precedence (evaluated first).

> **Best Practice**: Use parentheses to make complex expressions clear. Don't rely on memorizing the full table.

---

## 6.2 Arithmetic Operators

### Unary Operators

```cpp
int x{ 5 };
int y{ -x };   // unary minus: y = -5
int z{ +x };   // unary plus: z = 5 (rarely used)
```

### Binary Operators

| Operator | Symbol | Example |
|----------|--------|---------|
| Addition | `+` | `x + y` |
| Subtraction | `-` | `x - y` |
| Multiplication | `*` | `x * y` |
| Division | `/` | `x / y` |
| Remainder | `%` | `x % y` |

### Integer vs. Floating-Point Division

```cpp
// Integer division — fractional part DROPPED (not rounded)
int a{ 7 / 4 };              // a = 1

// Floating-point division — if at least one operand is float/double
double b{ 7.0 / 4 };         // b = 1.75

// Force float division with static_cast
int x{ 7 }, y{ 4 };
double c{ static_cast<double>(x) / y };  // c = 1.75
```

> **Warning**: Integer division by zero is **undefined behavior**. Floating-point `5.0 / 0.0` gives `Inf` on IEEE 754 systems.

### Compound Assignment

```cpp
x += 5;   // same as x = x + 5
x -= 3;   // same as x = x - 3
x *= 2;   // same as x = x * 2
x /= 4;   // same as x = x / 4
x %= 3;   // same as x = x % 3
```

---

## 6.3 Remainder and Exponentiation

### Remainder Operator `%`

```cpp
7 % 4;    // 3  (7 = 1*4 + 3)
12 % 5;   // 2  (12 = 2*5 + 2)
```

- Only works with **integer** operands
- Result takes the sign of the **first** operand: `-5 % 2 == -1`
- Test divisibility: `if (x % y == 0)` means x divides evenly by y
- Test odd/even: `if (x % 2 != 0)` — do NOT use `== 1` (fails for negative numbers)

### Exponentiation

C++ has **no** exponent operator! (`^` is bitwise XOR)

```cpp
#include <cmath>
double result{ std::pow(3.0, 4.0) };  // 3^4 = 81.0
```

> **Warning**: `std::pow()` uses floating-point math. Results may have rounding errors even with whole numbers.

---

## 6.4 Increment/Decrement Operators and Side Effects

### Prefix vs Postfix

| Form | Syntax | Behavior |
|------|--------|----------|
| Prefix | `++x` / `--x` | Increment/decrement, then return **new** value |
| Postfix | `x++` / `x--` | Copy original, increment/decrement, return **old** value |

```cpp
int x{ 5 };
int y{ ++x };   // x = 6, y = 6 (prefix: increment first)
int z{ x++ };   // z = 6, x = 7 (postfix: return old, then increment)
```

> **Best Practice**: Prefer **prefix** (`++x`) — it's more efficient (avoids creating a temporary copy).

### Side Effects and Undefined Behavior

```cpp
// DANGER: Don't use a variable with side effects more than once in one statement
int x{ 5 };
int y{ x + ++x };     // UNDEFINED BEHAVIOR!
add(x, ++x);           // UNDEFINED BEHAVIOR! (argument evaluation order unspecified)
```

> **Rule**: Never use a variable that has `++`/`--` applied more than once in the same expression.

---

## 6.5 The Comma Operator

```cpp
int x{ (1, 2) };   // x = 2 (evaluates left, discards, returns right)
```

- Lowest precedence of all operators
- Evaluates left operand, discards result, evaluates and returns right operand
- **Avoid using it** — comma in function calls and initializer lists is a separator, not the comma operator

---

## 6.6 The Conditional Operator (Ternary)

```cpp
condition ? expression_if_true : expression_if_false
```

```cpp
int max{ (x > y) ? x : y };           // inline max
std::cout << (isHappy ? ":-)" : ":-("); // conditional output

constexpr int size{ isLarge ? 100 : 50 }; // works in constexpr context!
```

### Parenthesization Rules

```cpp
// Always parenthesize the whole ternary in compound expressions:
std::cout << ((x > y) ? x : y) << '\n';

// Parenthesize the condition if it has operators:
int z{ (a > b) ? a : b };
```

> **Best Practice**: Use for simple one-line decisions. For complex logic, use if-else.

---

## 6.7 Relational Operators and Floating-Point Comparisons

### Relational Operators

| Operator | Meaning |
|----------|---------|
| `>` | Greater than |
| `<` | Less than |
| `>=` | Greater than or equal |
| `<=` | Less than or equal |
| `==` | Equal |
| `!=` | Not equal |

### The Floating-Point Comparison Problem

```cpp
double d1{ 100.0 - 99.99 };  // mathematically 0.01
double d2{ 10.0 - 9.99 };    // mathematically 0.01
if (d1 == d2)                 // FALSE! Due to rounding errors
```

> **Rule**: NEVER use `==` or `!=` with calculated floating-point values.

### Epsilon-Based Comparison

```cpp
#include <cmath>
#include <algorithm>

// Relative epsilon comparison (Knuth's method)
bool approximatelyEqual(double a, double b, double relEpsilon) {
    return std::abs(a - b) <=
           (std::max(std::abs(a), std::abs(b)) * relEpsilon);
}

// Combined absolute + relative epsilon (recommended)
bool approximatelyEqualAbsRel(double a, double b,
                               double absEpsilon = 1e-12,
                               double relEpsilon = 1e-8) {
    if (std::abs(a - b) <= absEpsilon)
        return true;
    return std::abs(a - b) <=
           (std::max(std::abs(a), std::abs(b)) * relEpsilon);
}
```

---

## 6.8 Logical Operators

### The Three Logical Operators

| Operator | Symbol | Example | Description |
|----------|--------|---------|-------------|
| NOT | `!` | `!x` | Flips true↔false |
| AND | `&&` | `x && y` | True only if BOTH true |
| OR | `\|\|` | `x \|\| y` | True if EITHER true |

### Short-Circuit Evaluation

```cpp
if (x != 0 && (y / x) > 2)  // Safe! If x==0, right side never evaluated
```

- `&&`: If left is false → result is false, right side skipped
- `||`: If left is true → result is true, right side skipped

> **Warning**: Don't put side effects (like `++x`) on the right side of `&&`/`||` — they may not execute!

### De Morgan's Laws

```
!(x && y)  ≡  !x || !y
!(x || y)  ≡  !x && !y
```

Tip: When distributing NOT, flip AND↔OR.

### Precedence Gotcha

```cpp
// WRONG: ! has higher precedence than >
if (!x > y)      // evaluates as (!x) > y

// CORRECT:
if (!(x > y))    // negate the comparison result
```

> **Best Practice**: Always parenthesize when mixing `&&` and `||`:
> ```cpp
> if ((a && b) || (c && d))  // clear intent
> ```

---

## Chapter 6 — Quick Reference Card

| Concept | Rule |
|---------|------|
| Division by zero (int) | Undefined behavior |
| Division by zero (float) | Inf or NaN |
| `%` with negatives | Result takes sign of first operand |
| `^` operator | Bitwise XOR, NOT exponentiation |
| Prefix vs postfix | Prefer prefix `++x` |
| Float equality | Never use `==`, use epsilon comparison |
| `&&` / `\|\|` | Short-circuit: right side may not execute |
| Complex expressions | Use parentheses liberally |
