# Chapter 9: Error Detection and Handling

## 9.1 Introduction to Testing Your Code

### Testing Strategies

- **Test small pieces** of code in isolation before integrating
- **Test boundary conditions**: edge cases, min/max values, empty inputs
- Write tests as you code, not just at the end
- **Informal testing**: Call functions with known inputs and verify outputs
- **Unit testing**: Formal automated tests (using frameworks like Google Test)

### What to Test

```cpp
// Test normal cases
assert(isLowerVowel('a') == true);
assert(isLowerVowel('q') == false);

// Test edge cases
assert(isLowerVowel('A') == false);  // uppercase
assert(isLowerVowel('0') == false);  // digit
```

---

## 9.2 Code Coverage

### Types of Coverage

| Coverage Type | Description |
|---------------|-------------|
| **Statement coverage** | Every statement executes at least once |
| **Branch coverage** | Every possible branch (if/else) path taken |
| **Loop coverage** | Test 0, 1, and 2+ iterations |

> **Best Practice**: Aim for 100% branch coverage. Statement coverage alone misses many bugs.

### Testing Loops

For each loop, test:
1. Loop body executes **0 times** (condition false initially)
2. Loop body executes **1 time**
3. Loop body executes **2+ times**

---

## 9.3 Common Semantic Errors in C++

| Error | Example |
|-------|---------|
| Conditional logic errors | `if (x >= 0)` when you meant `if (x > 0)` |
| Off-by-one errors | Loop runs one too many or too few times |
| Incorrect operator precedence | `x & y == 0` evaluates as `x & (y == 0)` |
| Floating-point comparison | Using `==` with floats |
| Integer division | `5 / 10` gives `0`, not `0.5` |
| Accidental null statements | `if (x == 5);` — semicolon makes empty body |
| Uninitialized variables | Reading before assigning |

---

## 9.4 Detecting and Handling Errors

### Error Handling Strategies

**1. Return an error code**
```cpp
int doSomething(int x) {
    if (x < 0)
        return -1;  // error indicator
    return x * 2;
}
```

**2. Use std::optional (C++17)**
```cpp
#include <optional>
std::optional<int> doSomething(int x) {
    if (x < 0)
        return std::nullopt;
    return x * 2;
}
```

**3. Throw an exception** (covered in Chapter 27)

**4. Use assertions** for programmer errors

### The Bouncer Pattern

Validate inputs at the top of a function and return early:

```cpp
void printName(std::string_view name) {
    if (name.empty()) return;     // bouncer: reject invalid input
    // proceed with valid input
    std::cout << name;
}
```

---

## 9.5 std::cin and Handling Invalid Input

### Common Input Problems

```cpp
int x{};
std::cin >> x;  // User types "hello" — extraction fails!
```

When extraction fails:
1. The variable is left **unchanged** (or zero-initialized)
2. `std::cin` enters a **failure state**
3. Future extractions are **silently skipped** until the error is cleared

### Robust Input Handling Pattern

```cpp
#include <iostream>
#include <limits>

int getInt() {
    while (true) {
        int x{};
        std::cout << "Enter an integer: ";
        std::cin >> x;

        if (std::cin.fail()) {               // check for failed extraction
            std::cin.clear();                 // clear error flags
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');  // discard bad input
            std::cout << "Invalid input. Try again.\n";
        } else {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');  // discard extra input
            return x;
        }
    }
}
```

### Key Functions

| Function | Purpose |
|----------|---------|
| `std::cin.fail()` | Returns true if last extraction failed |
| `std::cin.clear()` | Resets error flags |
| `std::cin.ignore(n, delim)` | Discards up to n characters until delimiter |
| `std::numeric_limits<std::streamsize>::max()` | Maximum stream size (discard entire buffer) |

---

## 9.6 Assert and static_assert

### Runtime Assert

```cpp
#include <cassert>

double calculateGrade(double score) {
    assert(score >= 0.0 && score <= 100.0 && "Score must be 0-100");
    // ... calculation
}
```

- If condition is false → prints error message with file/line → calls `std::abort()`
- **Disabled in release builds** when `NDEBUG` is defined
- Use for catching **programmer errors** (bugs), not user input errors

### Adding Messages to Assert

```cpp
// String literal always evaluates to true, so:
assert(found && "Car not found in database");
// If found is false: false && true = false → triggers assertion with message
```

### static_assert (Compile-Time)

```cpp
static_assert(sizeof(int) == 4, "int must be 4 bytes");
static_assert(sizeof(long) == 8, "long must be 8 bytes");
```

- Evaluated at **compile time** — zero runtime cost
- Condition must be a constant expression
- Compilation fails if the condition is false
- Message is optional in C++17+

> **Best Practice**:
> - Use `static_assert` over `assert` whenever the condition can be checked at compile time
> - Don't put side effects in assert expressions (they're removed in release builds!)
> - Use assertions liberally for preconditions and invariants

### Assert vs Error Handling

| Assertions | Error Handling |
|------------|----------------|
| Catch programmer bugs | Handle runtime conditions |
| Disabled in release | Always active |
| Call `abort()` on failure | Can recover gracefully |
| "This should never happen" | "This might happen" |

---

## Chapter 9 — Quick Reference Card

| Concept | Rule |
|---------|------|
| Testing | Test edge cases + boundary conditions |
| Branch coverage | Aim for 100% |
| Input validation | Use the cin.fail() → clear() → ignore() pattern |
| Assert | For programmer errors, not user errors |
| static_assert | Prefer over assert when possible |
| Side effects in assert | **Never** — they're removed in release |
| Error handling | Return codes, std::optional, or exceptions |
