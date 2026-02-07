# Chapter 20: Functions

## 20.1 Function Pointers

A function pointer stores the address of a function:

```cpp
int add(int x, int y) { return x + y; }

int (*fcnPtr)(int, int){ &add };  // pointer to function taking 2 ints, returning int
int result{ fcnPtr(3, 4) };       // call via pointer: 7

// Using auto (much cleaner):
auto fcnPtr2{ &add };
```

### Callback Functions

```cpp
void sort(std::vector<int>& v, bool (*compare)(int, int)) {
    // uses compare function pointer to decide ordering
}

bool ascending(int a, int b) { return a > b; }
bool descending(int a, int b) { return a < b; }

sort(v, ascending);   // pass function as argument
sort(v, descending);
```

### std::function (Modern Alternative)

```cpp
#include <functional>

std::function<int(int, int)> fcn{ &add };
fcn(3, 4);  // 7
```

> **Best Practice**: Use `auto` for function pointers, or `std::function` for more flexibility. Raw function pointer syntax is hard to read.

---

## 20.2 The Stack and the Heap

### The Call Stack

When a function is called:
1. A **stack frame** is pushed (return address, arguments, locals)
2. The function executes
3. The stack frame is popped on return

### Stack Overflow

Too many nested calls (deep recursion) exhausts the limited stack space (~1-8 MB):

```cpp
void infinite() { infinite(); }  // STACK OVERFLOW!
```

### Heap

- Used for `new`/`delete` allocations
- Much larger than the stack
- Manual management required
- Accessed via pointers

---

## 20.3 Recursion

A function that calls itself:

```cpp
int factorial(int n) {
    if (n <= 1)         // base case
        return 1;
    return n * factorial(n - 1);  // recursive case
}

factorial(5);  // 5 * 4 * 3 * 2 * 1 = 120
```

### Fibonacci (with memoization)

```cpp
int fibonacci(int n) {
    static std::vector<int> cache(n + 1, 0);  // memoization
    if (n <= 1) return n;
    if (cache[n] != 0) return cache[n];
    cache[n] = fibonacci(n - 1) + fibonacci(n - 2);
    return cache[n];
}
```

### Recursion vs Iteration

| Aspect | Recursion | Iteration |
|--------|-----------|-----------|
| Readability | Often cleaner for tree/divide-and-conquer | Better for simple loops |
| Performance | Slower (function call overhead) | Faster |
| Risk | Stack overflow | None |
| Use when | Problem naturally recursive | Otherwise |

> **Best Practice**: Prefer iteration unless recursion significantly simplifies the code.

---

## 20.4 Command Line Arguments

```cpp
int main(int argc, char* argv[]) {
    // argc: argument count (including program name)
    // argv: array of C-style string arguments

    std::cout << "Program: " << argv[0] << '\n';

    for (int i{ 1 }; i < argc; ++i)
        std::cout << "Arg " << i << ": " << argv[i] << '\n';
}
```

```bash
./program hello world
# argc = 3
# argv[0] = "./program"
# argv[1] = "hello"
# argv[2] = "world"
```

### Converting String Arguments

```cpp
#include <sstream>

std::stringstream convert{ argv[1] };
int value{};
if (!(convert >> value))
    value = 0;  // conversion failed
```

---

## 20.5 Ellipsis (and Why to Avoid Them)

```cpp
#include <cstdarg>

double average(int count, ...) {  // C-style variadic function
    va_list list;
    va_start(list, count);
    double sum{ 0 };
    for (int i{ 0 }; i < count; ++i)
        sum += va_arg(list, int);
    va_end(list);
    return sum / count;
}
```

> **Avoid ellipsis entirely** — no type safety, no count safety. Use variadic templates, `std::initializer_list`, or function overloading instead.

---

## 20.6 Introduction to Lambdas

```cpp
// Basic lambda
auto add{ [](int x, int y) { return x + y; } };
add(3, 4);  // 7

// Lambda with explicit return type
auto divide{ [](int x, int y) -> double {
    return static_cast<double>(x) / y;
} };

// Generic lambda (C++14)
auto print{ [](const auto& value) {
    std::cout << value << '\n';
} };
```

### Lambda Syntax

```
[captures](parameters) -> returnType { body }
```

| Part | Required | Description |
|------|----------|-------------|
| `[captures]` | Yes | Variables from enclosing scope |
| `(parameters)` | Optional | Function parameters |
| `-> returnType` | Optional | Explicit return type |
| `{ body }` | Yes | Function body |

### Using Lambdas with Algorithms

```cpp
std::vector<int> v{ 5, 2, 8, 1, 9 };

std::sort(v.begin(), v.end(),
    [](int a, int b) { return a < b; });

auto it{ std::find_if(v.begin(), v.end(),
    [](int x) { return x > 5; }) };
```

---

## 20.7 Lambda Captures

### Capture by Value

```cpp
int x{ 5 };
auto lambda{ [x]() { std::cout << x; } };  // captures a COPY of x
```

- Creates a const copy (can't modify by default)
- Use `mutable` to modify the copy:

```cpp
auto counter{ [count = 0]() mutable { return ++count; } };
```

### Capture by Reference

```cpp
int x{ 5 };
auto lambda{ [&x]() { x = 10; } };  // modifies original x
lambda();  // x is now 10
```

### Default Captures

```cpp
[=]    // capture ALL used variables by value
[&]    // capture ALL used variables by reference
[=, &x]  // all by value EXCEPT x by reference
[&, x]   // all by reference EXCEPT x by value
```

### Init Captures (C++14)

```cpp
auto lambda{ [value = expensive_calc()]() {
    std::cout << value;
} };
```

> **Best Practice**:
> - Capture only what you need
> - Prefer explicit captures over default `[=]`/`[&]`
> - Use `[&]` only when you need to modify outer variables
> - Beware of dangling references (captured variable must outlive lambda)

---

## Chapter 20 — Quick Reference Card

| Concept | Rule |
|---------|------|
| Function pointers | Use `auto` or `std::function` |
| Recursion | Need base case; prefer iteration when simpler |
| Stack overflow | Deep recursion risk; ~1-8 MB stack limit |
| Command line args | `argc` = count, `argv[]` = C-strings |
| Ellipsis `...` | **Avoid** — no type safety |
| Lambda syntax | `[captures](params) -> ret { body }` |
| Default captures | Prefer explicit over `[=]`/`[&]` |
| Mutable lambda | `[x]() mutable { ++x; }` |
