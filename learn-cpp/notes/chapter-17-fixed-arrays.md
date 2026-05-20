# Chapter 17: Fixed-Size Arrays — std::array and C-Style Arrays

## 17.1 Introduction to std::array

```cpp
#include <array>

std::array<int, 5> arr{ 1, 2, 3, 4, 5 };   // fixed size of 5

// CTAD (C++17)
constexpr std::array data{ 9, 7, 5, 3, 1 }; // deduced: std::array<int, 5>
```

### Key Properties
- Size must be a **compile-time constant** (constexpr, literal, enum)
- Cannot be resized after creation
- Full `constexpr` support (unlike vector)
- Aggregate type — uses aggregate initialization

### std::array vs std::vector

| Feature | `std::array` | `std::vector` |
|---------|-------------|---------------|
| Size | Fixed (compile-time) | Dynamic (runtime) |
| constexpr | Full support | Limited |
| Performance | Slightly better (stack) | Slightly less (heap) |
| Use when | Size known at compile time | Size may change |

---

## 17.2 std::array Length and Indexing

```cpp
std::array<int, 5> arr{ 10, 20, 30, 40, 50 };

arr.size();     // 5 (constexpr)
arr[0];         // 10 — no bounds check
arr.at(2);      // 30 — with bounds check (throws on out-of-bounds)
arr.front();    // 10
arr.back();     // 50
```

---

## 17.3 Passing and Returning std::array

### The Problem

```cpp
void print(const std::array<int, 5>& arr);  // only works for size 5!
```

The size is part of the type — `std::array<int, 5>` and `std::array<int, 10>` are different types.

### Solution: Function Templates

```cpp
template <typename T, std::size_t N>
void print(const std::array<T, N>& arr) {
    for (const auto& elem : arr)
        std::cout << elem << ' ';
    std::cout << '\n';
}
```

Or with C++20 abbreviated templates:
```cpp
void print(const auto& arr) {  // accepts any array type/size
    for (const auto& elem : arr)
        std::cout << elem << ' ';
}
```

---

## 17.4 std::array of Class Types and Brace Elision

```cpp
struct Point { int x; int y; };

// Full braces:
std::array<Point, 3> arr{{ {1,2}, {3,4}, {5,6} }};

// Brace elision (allowed for aggregates):
std::array<Point, 3> arr{ 1, 2, 3, 4, 5, 6 };
```

---

## 17.5 Arrays of References via std::reference_wrapper

You can't create an array of references directly, but `std::reference_wrapper` works:

```cpp
#include <functional>

int a{1}, b{2}, c{3};
std::array<std::reference_wrapper<int>, 3> arr{ a, b, c };
arr[0].get() = 10;  // modifies a
```

---

## 17.6 std::array and Enumerations

```cpp
enum Color { red, green, blue, max_colors };

std::array<std::string_view, max_colors> colorNames{
    "red", "green", "blue"
};

std::cout << colorNames[red];  // "red"
```

---

## 17.7-17.8 C-Style Arrays and Decay

### Declaration

```cpp
int arr[5]{};           // 5 ints, zero-initialized
int primes[]{ 2, 3, 5, 7, 11 };  // size deduced from initializer
```

### Array Decay

When used in most expressions, C-style arrays **decay** to a pointer to their first element:

```cpp
int arr[5]{ 1, 2, 3, 4, 5 };
int* ptr{ arr };          // arr decays to &arr[0]

// The DANGER: size information is lost!
sizeof(arr);   // 20 (5 ints × 4 bytes)
sizeof(ptr);   // 8  (just a pointer — size info gone!)
```

### Decay in Functions

```cpp
void print(int* arr, int size) {  // receives pointer, not array
    // sizeof(arr) would be 8 (pointer size), NOT array size!
    for (int i{ 0 }; i < size; ++i)
        std::cout << arr[i] << ' ';
}

int arr[5]{ 1, 2, 3, 4, 5 };
print(arr, 5);  // must pass size separately!
```

> **Best Practice**: Avoid C-style arrays. Use `std::array` or `std::vector` instead.

---

## 17.9 Pointer Arithmetic and Subscripting

```cpp
int arr[]{ 10, 20, 30, 40, 50 };
int* ptr{ arr };

*(ptr + 0);  // 10 — same as ptr[0]
*(ptr + 1);  // 20 — same as ptr[1]
*(ptr + 2);  // 30 — same as ptr[2]

// ptr[n] is syntactic sugar for *(ptr + n)
```

Pointer arithmetic scales by the size of the pointed-to type:
- For `int*`: `ptr + 1` advances by `sizeof(int)` bytes (usually 4)
- For `double*`: `ptr + 1` advances by `sizeof(double)` bytes (usually 8)

### Iterator Pattern

```cpp
const int* begin{ arr };
const int* end{ arr + std::size(arr) };
for (; begin != end; ++begin)
    std::cout << *begin << ' ';
```

---

## 17.10-17.11 C-Style Strings

```cpp
char name[]{ "Hello" };       // array of 6 chars (includes null terminator \0)
const char* str{ "World" };   // pointer to string literal

std::cout << name;            // prints "Hello" (special treatment for char*)
```

- C-style strings are null-terminated (`\0`) char arrays
- String literals have static duration (persist for entire program)
- **Prefer** `std::string` or `std::string_view` over C-style strings

---

## 17.12-17.13 Multidimensional Arrays

### C-Style 2D Array

```cpp
int arr[3][4]{        // 3 rows, 4 columns
    { 1, 2, 3, 4 },
    { 5, 6, 7, 8 },
    { 9, 10, 11, 12 }
};
arr[1][2];  // row 1, col 2 = 7
```

### Multidimensional std::array

```cpp
// 2D array: 3 rows, 4 columns
std::array<std::array<int, 4>, 3> arr{{
    { 1, 2, 3, 4 },
    { 5, 6, 7, 8 },
    { 9, 10, 11, 12 }
}};
arr[1][2];  // 7
```

> Note the order: `std::array<std::array<COLS>, ROWS>` — inner array is columns, outer is rows.

---

## Chapter 17 — Quick Reference Card

| Concept | Rule |
|---------|------|
| `std::array` | Fixed size, compile-time, constexpr-friendly |
| `std::vector` | Dynamic size, runtime |
| C-style arrays | **Avoid** — use std::array/vector |
| Array decay | C-style arrays lose size info when passed |
| `ptr[n]` | Same as `*(ptr + n)` |
| C-style strings | **Avoid** — use std::string/string_view |
| Passing std::array | Template function for generic size |
| Multidimensional | `std::array<std::array<T, cols>, rows>` |
