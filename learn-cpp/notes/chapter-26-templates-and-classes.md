# Chapter 26: Templates and Classes

## 26.1 Template Classes

```cpp
template <typename T>
class Array {
    T* m_data{};
    int m_length{};

public:
    Array(int length) : m_data{new T[length]{}}, m_length{length} {}
    ~Array() { delete[] m_data; }

    T& operator[](int index) { return m_data[index]; }
    int getLength() const { return m_length; }
};

// Usage:
Array<int> intArr{10};
Array<double> dblArr{5};
```

### Member Functions Outside the Class

```cpp
template <typename T>
T& Array<T>::operator[](int index) {
    assert(index >= 0 && index < m_length);
    return m_data[index];
}
```

### Where to Put Template Code

Templates must be fully visible where they're used:

| Approach | Method |
|----------|--------|
| **All-in-header** (preferred) | Put everything in `.h` file |
| `.inl` file | Move definitions to `.inl`, `#include` at bottom of header |
| Explicit instantiation | `template class Array<int>;` in a `.cpp` file |

---

## 26.2 Template Non-Type Parameters

```cpp
template <typename T, int size>
class StaticArray {
    T m_array[size]{};
public:
    T& operator[](int index) { return m_array[index]; }
};

StaticArray<int, 12> intArray;
StaticArray<double, 4> dblArray;
```

Non-type parameters must be `constexpr`.

---

## 26.3 Function Template Specialization

Override a template function for a specific type:

```cpp
template <typename T>
void print(const T& value) {
    std::cout << value;
}

// Specialization for double — print with more precision
template <>
void print<double>(const double& value) {
    std::cout << std::scientific << value;
}
```

---

## 26.4 Class Template Specialization

Override an entire class for a specific type:

```cpp
// Primary template
template <typename T>
class Storage8 {
    T m_array[8]{};
public:
    T& operator[](int index) { return m_array[index]; }
};

// Specialization for bool — pack 8 bools into 1 byte
template <>
class Storage8<bool> {
    unsigned char m_data{};
public:
    void set(int index, bool value) {
        if (value)
            m_data |= (1 << index);
        else
            m_data &= ~(1 << index);
    }
    bool get(int index) const {
        return (m_data >> index) & 1;
    }
};
```

### When to Specialize
- Performance optimization (like bool packing above)
- Different internal representation for certain types
- Different behavior needed for specific types

---

## 26.5 Partial Template Specialization

Specialize for a subset of template parameters:

```cpp
// Primary template
template <typename T, int size>
class StaticArray { /* ... */ };

// Partial specialization — any T, but size = 14 specifically
template <typename T>
class StaticArray<T, 14> {
    // special implementation for arrays of size 14
};
```

### Partial Specialization for Pointers

```cpp
// Primary template
template <typename T>
class Storage {
    T m_value{};
public:
    Storage(T value) : m_value{value} {}
};

// Specialization for pointer types
template <typename T>
class Storage<T*> {
    T* m_value{};
public:
    Storage(T* value) : m_value{ new T{*value} } {}  // deep copy
    ~Storage() { delete m_value; }
};
```

---

## Chapter 26 — Quick Reference Card

| Concept | Rule |
|---------|------|
| Template class location | Full definition in header file |
| Outside member functions | Need `template <typename T>` prefix |
| Non-type parameters | Must be constexpr |
| Full specialization | `template <>` with specific type |
| Partial specialization | Specialize some parameters, leave others generic |
| When to specialize | Performance optimization or different behavior |
