# Chapter 18: Iterators and Algorithms

## 18.1 Sorting an Array Using Selection Sort

Selection sort: repeatedly find the smallest element and swap it to the front.

```cpp
void selectionSort(std::vector<int>& arr) {
    for (std::size_t i{ 0 }; i < arr.size() - 1; ++i) {
        std::size_t minIdx{ i };
        for (std::size_t j{ i + 1 }; j < arr.size(); ++j) {
            if (arr[j] < arr[minIdx])
                minIdx = j;
        }
        std::swap(arr[i], arr[minIdx]);
    }
}
```

- O(n²) — inefficient for large data sets
- Use `std::sort` instead (O(n log n))

---

## 18.2 Introduction to Iterators

An **iterator** is an object that traverses a container, providing access to each element.

### Core Operations
- `*iter` — dereference (access current element)
- `++iter` — advance to next element

### begin() and end()

```cpp
std::vector<int> v{ 10, 20, 30 };

auto it{ v.begin() };   // iterator to first element
auto end{ v.end() };    // iterator to ONE PAST the last element

while (it != end) {
    std::cout << *it << ' ';  // dereference to get value
    ++it;
}
// Output: 10 20 30
```

> **Key**: `end()` points past the last element — it's a sentinel, not a valid element.

### Pointers as Iterators

Pointers are the simplest form of iterator:
```cpp
int arr[]{ 1, 2, 3 };
int* begin{ arr };                     // or &arr[0]
int* end{ arr + std::size(arr) };      // one past last
```

### Iterator Invalidation

> **Warning**: Modifying a container (push_back, erase, etc.) can **invalidate** existing iterators. Using an invalidated iterator = undefined behavior.

---

## 18.3 Introduction to Standard Library Algorithms

`#include <algorithm>`

### std::find — Find a value

```cpp
std::vector<int> v{ 1, 2, 3, 4, 5 };
auto result{ std::find(v.begin(), v.end(), 3) };
if (result != v.end())
    std::cout << "Found: " << *result;
```

### std::find_if — Find by condition

```cpp
auto result{ std::find_if(v.begin(), v.end(),
    [](int x) { return x % 2 == 0; }) };  // find first even number
```

### std::count / std::count_if

```cpp
int count{ std::count(v.begin(), v.end(), 3) };       // count 3s
int evenCount{ std::count_if(v.begin(), v.end(),
    [](int x) { return x % 2 == 0; }) };               // count even numbers
```

### std::sort

```cpp
std::sort(v.begin(), v.end());                         // ascending
std::sort(v.begin(), v.end(), std::greater<int>{});    // descending
std::sort(v.begin(), v.end(),
    [](int a, int b) { return a > b; });               // custom comparator
```

### std::for_each

```cpp
std::for_each(v.begin(), v.end(),
    [](int x) { std::cout << x << ' '; });
```

### Other Useful Algorithms

| Algorithm | Purpose |
|-----------|---------|
| `std::min_element` | Iterator to minimum value |
| `std::max_element` | Iterator to maximum value |
| `std::accumulate` | Sum all elements (`<numeric>`) |
| `std::reverse` | Reverse container |
| `std::copy` | Copy elements between ranges |
| `std::fill` | Fill with a value |
| `std::any_of` | True if any element matches predicate |
| `std::all_of` | True if all elements match |
| `std::none_of` | True if no elements match |

> **Best Practice**: Use standard library algorithms over hand-written loops — they're tested, optimized, and clearer.

---

## 18.4 Timing Your Code

```cpp
#include <chrono>

class Timer {
    using Clock = std::chrono::steady_clock;
    using Second = std::chrono::duration<double, std::ratio<1>>;

    std::chrono::time_point<Clock> m_begin{ Clock::now() };
public:
    void reset() { m_begin = Clock::now(); }
    double elapsed() const {
        return std::chrono::duration_cast<Second>(Clock::now() - m_begin).count();
    }
};

Timer t;
// ... code to time ...
std::cout << "Elapsed: " << t.elapsed() << "s\n";
```

---

## Chapter 18 — Quick Reference Card

| Concept | Rule |
|---------|------|
| Iterators | `begin()` to `end()` (exclusive) |
| Dereference | `*iter` to get value |
| `std::find` | Returns iterator or `end()` if not found |
| `std::sort` | O(n log n); pass custom comparator for order |
| Algorithms | Prefer over hand-written loops |
| Iterator invalidation | Don't modify container while iterating |
