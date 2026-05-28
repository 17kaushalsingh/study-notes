# Chapter 22: Move Semantics and Smart Pointers

## 22.1 Introduction to Smart Pointers and Move Semantics

**The Problem**: Raw `new`/`delete` leads to memory leaks and dangling pointers.

**The Solution**: Smart pointers — classes that manage dynamic memory and automatically clean up when they go out of scope (RAII).

---

## 22.2 R-value References

```cpp
int& lref{ x };     // lvalue reference — binds to lvalues
int&& rref{ 5 };    // rvalue reference — binds to rvalues (temporaries)
```

### Binding Rules

| Reference type | Binds to lvalues? | Binds to rvalues? |
|---------------|-------------------|-------------------|
| `T&` | Yes | No |
| `const T&` | Yes | Yes |
| `T&&` | No | Yes |

### Key Insight
- R-value references extend the lifetime of temporaries
- Enable overloading to distinguish temporaries from persistent objects
- **Important**: An r-value reference variable IS an lvalue when used in expressions!

---

## 22.3 Move Constructors and Move Assignment

### Move Constructor

```cpp
class MyString {
    char* m_data{};
    int m_length{};

public:
    // Move constructor — "steals" resources from the source
    MyString(MyString&& source) noexcept
        : m_data{ source.m_data }
        , m_length{ source.m_length }
    {
        source.m_data = nullptr;   // leave source in valid but empty state
        source.m_length = 0;
    }
};
```

### Move Assignment

```cpp
MyString& operator=(MyString&& source) noexcept {
    if (this == &source) return *this;  // self-assignment check

    delete[] m_data;                    // free existing resource

    m_data = source.m_data;            // steal from source
    m_length = source.m_length;
    source.m_data = nullptr;
    source.m_length = 0;

    return *this;
}
```

### How Move Differs from Copy

| Copy | Move |
|------|------|
| Allocates new memory | No allocation |
| Copies data | Transfers pointer ownership |
| Source unchanged | Source left empty (valid but unspecified) |
| Slow for large objects | Fast (constant time) |

> **Key**: Always mark move operations `noexcept` — enables compiler optimizations.

---

## 22.4 std::move

`std::move` casts an lvalue to an rvalue reference, enabling move semantics:

```cpp
#include <utility>

std::string a{ "Hello" };
std::string b{ std::move(a) };  // a is now empty, b owns "Hello"
```

- `std::move` doesn't actually move anything — it's just a cast
- After `std::move`, the source object is in a **valid but unspecified state**
- Don't use the source object's value after moving (only reassign or destroy)

### When to Use std::move
- Passing ownership of a resource
- Returning expensive local objects from functions (though copy elision often handles this)
- Swapping values efficiently

---

## 22.5 std::unique_ptr

**Exclusive ownership** — only one `unique_ptr` can own a resource at a time.

```cpp
#include <memory>

// Creating (prefer make_unique)
auto ptr{ std::make_unique<int>(42) };

// Accessing
std::cout << *ptr;        // dereference: 42
std::cout << ptr.get();   // raw pointer

// Transfer ownership
auto ptr2{ std::move(ptr) };  // ptr is now nullptr

// Checking
if (ptr2)                 // true if not null
    std::cout << *ptr2;
```

### Key Operations

```cpp
ptr.reset();              // delete resource and set to nullptr
ptr.reset(new int{99});   // delete old, take ownership of new
ptr.release();            // return raw pointer, release ownership (rare)
```

### Unique_ptr with Functions

```cpp
// Pass raw pointer when function doesn't need ownership
void useResource(const Resource* res);
useResource(ptr.get());

// Transfer ownership (rare)
void takeOwnership(std::unique_ptr<Resource> res);
takeOwnership(std::move(ptr));

// Return unique_ptr by value (factory pattern)
std::unique_ptr<Resource> createResource() {
    return std::make_unique<Resource>();
}
```

> **Best Practices**:
> - Use `std::make_unique<T>()` instead of `new`
> - Pass raw pointers or references to functions (not unique_ptr) unless transferring ownership
> - Never dynamically allocate a unique_ptr itself

---

## 22.6 std::shared_ptr

**Shared ownership** — multiple `shared_ptr` can own the same resource. The resource is freed when the LAST shared_ptr is destroyed.

```cpp
#include <memory>

auto ptr1{ std::make_shared<Resource>() };

{
    auto ptr2{ ptr1 };  // reference count: 2 (COPY, not move!)
    // ptr1 and ptr2 both point to same Resource
}  // ptr2 destroyed, reference count: 1

// ptr1 still valid, Resource still alive
```

### Reference Counting

```cpp
auto ptr{ std::make_shared<int>(42) };
ptr.use_count();  // 1

auto ptr2{ ptr };
ptr.use_count();  // 2

ptr2.reset();
ptr.use_count();  // 1
```

### Creating shared_ptr

```cpp
// PREFERRED — single allocation for object + control block
auto ptr{ std::make_shared<Resource>() };

// AVOID — two separate allocations
std::shared_ptr<Resource> ptr{ new Resource{} };
```

> **Warning**: Never create two independent shared_ptrs from the same raw pointer — causes double-deletion!
> ```cpp
> Resource* raw{ new Resource{} };
> std::shared_ptr<Resource> p1{ raw };
> std::shared_ptr<Resource> p2{ raw };  // BUG: both think they own it!
> ```

---

## 22.7 Circular Dependencies and std::weak_ptr

### The Problem

```cpp
struct A {
    std::shared_ptr<B> bPtr;
};
struct B {
    std::shared_ptr<A> aPtr;
};

auto a{ std::make_shared<A>() };
auto b{ std::make_shared<B>() };
a->bPtr = b;
b->aPtr = a;
// Neither can be freed! Circular reference = memory leak
```

### The Solution: std::weak_ptr

`std::weak_ptr` observes a shared resource **without** incrementing the reference count:

```cpp
struct B {
    std::weak_ptr<A> aPtr;  // doesn't keep A alive
};
```

### Using weak_ptr

```cpp
auto shared{ std::make_shared<Resource>() };
std::weak_ptr<Resource> weak{ shared };

// Can't dereference directly — must convert to shared_ptr first:
if (auto locked{ weak.lock() }) {  // returns shared_ptr (or nullptr if expired)
    std::cout << *locked;
}

weak.expired();  // true if the resource has been freed
```

---

## Smart Pointer Decision Guide

```
Do you need shared ownership?
├── No  → std::unique_ptr (default choice)
└── Yes → std::shared_ptr
         └── Breaking circular ref? → std::weak_ptr
```

---

## Chapter 22 — Quick Reference Card

| Concept | Rule |
|---------|------|
| Default smart pointer | `std::unique_ptr` |
| Creation | `std::make_unique<T>()` / `std::make_shared<T>()` |
| Move semantics | `std::move()` casts lvalue → rvalue |
| After moving | Object in valid but unspecified state |
| `noexcept` | Always mark move operations noexcept |
| Shared ownership | `std::shared_ptr` (reference counted) |
| Circular references | Use `std::weak_ptr` to break cycles |
| Pass to functions | Pass raw pointer/ref, not smart pointer |
| Raw `new`/`delete` | **Avoid** — use smart pointers |
