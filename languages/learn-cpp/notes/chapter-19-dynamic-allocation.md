# Chapter 19: Dynamic Allocation

## 19.1 Dynamic Memory Allocation with new and delete

### Stack vs Heap

| Property | Stack | Heap |
|----------|-------|------|
| Size | Small (1-8 MB) | Large (GBs) |
| Allocation speed | Fast | Slower |
| Cleanup | Automatic (scope-based) | Manual (`delete`) |
| Access speed | Direct | Via pointer (slower) |
| Lifetime | Until scope ends | Until explicitly freed |

### new and delete

```cpp
int* ptr{ new int };         // allocate uninitialized int on heap
int* ptr2{ new int{ 5 } };  // allocate and initialize to 5
int* ptr3{ new int{} };     // allocate and value-initialize to 0

// Use the memory
*ptr2 = 10;

// Free the memory
delete ptr;
delete ptr2;
delete ptr3;

ptr = nullptr;    // prevent dangling pointer
ptr2 = nullptr;
ptr3 = nullptr;
```

### Memory Leaks

Memory leaks occur when dynamically allocated memory is never freed:

```cpp
void leak() {
    int* ptr{ new int{ 42 } };
    // ptr goes out of scope without delete — LEAK!
}

// Also a leak:
int* ptr{ new int{} };
ptr = new int{};     // original allocation is lost — LEAK!
```

### Best Practices

- Always `delete` before reassigning a pointer to new memory
- Set pointers to `nullptr` after deleting
- Avoid having multiple pointers to the same memory (ownership confusion)
- **Prefer smart pointers** (`std::unique_ptr`, `std::shared_ptr`) over raw `new`/`delete`

---

## 19.2 Dynamically Allocating Arrays

```cpp
int* arr{ new int[5]{ 1, 2, 3, 4, 5 } };  // allocate array of 5 ints

arr[0] = 10;      // access like normal array
arr[2] = 30;

delete[] arr;      // MUST use delete[] for arrays!
arr = nullptr;
```

> **Warning**: Use `delete[]` (not `delete`) for arrays. Using `delete` on an array = undefined behavior.

> **Best Practice**: Use `std::vector` instead of dynamically allocated arrays. It handles memory management automatically.

---

## 19.3 Destructors (Revisited)

When a class manages dynamic memory, it needs a destructor:

```cpp
class IntArray {
    int* m_data{};
    int m_length{};
public:
    IntArray(int length)
        : m_data{ new int[length]{} }, m_length{ length } {}

    ~IntArray() {
        delete[] m_data;  // free the dynamically allocated memory
    }
};
```

---

## 19.4 Pointers to Pointers

```cpp
int** ptrptr{ nullptr };  // pointer to a pointer to int

int value{ 5 };
int* ptr{ &value };
int** ptrptr2{ &ptr };

**ptrptr2;  // double dereference: 5
```

### Dynamic 2D Arrays

```cpp
int** arr{ new int*[rows] };   // array of row pointers
for (int i{ 0 }; i < rows; ++i)
    arr[i] = new int[cols]{};  // each row is an array

// Cleanup (reverse order):
for (int i{ 0 }; i < rows; ++i)
    delete[] arr[i];
delete[] arr;
```

> **Best Practice**: Use `std::vector<std::vector<int>>` instead. Or flatten to a 1D vector and compute indices.

---

## 19.5 Void Pointers

```cpp
void* ptr;  // can point to any type
int x{ 5 };
ptr = &x;

// Cannot dereference directly — must cast first:
std::cout << *static_cast<int*>(ptr);  // 5
```

- Void pointers lose type information
- Cannot do pointer arithmetic
- Must cast before dereferencing
- **Avoid** in modern C++ — use templates or `std::any` instead

---

## Chapter 19 — Quick Reference Card

| Concept | Rule |
|---------|------|
| `new` / `delete` | **Avoid** — use smart pointers or containers |
| Array deallocation | `delete[]` (not `delete`) |
| After delete | Set pointer to `nullptr` |
| Memory leaks | Delete before reassigning pointers |
| Dynamic arrays | Use `std::vector` instead |
| Void pointers | **Avoid** — use templates |
| Smart pointers | `std::unique_ptr`, `std::shared_ptr` (Ch 22) |
