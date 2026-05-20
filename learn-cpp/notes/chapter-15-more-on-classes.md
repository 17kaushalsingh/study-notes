# Chapter 15: More on Classes

## 15.1 The Hidden "this" Pointer and Member Function Chaining

Every member function has a hidden `this` pointer — a const pointer to the current object:

```cpp
class Simple {
    int m_id{};
public:
    void setID(int id) { this->m_id = id; }  // explicit this->
    void setID2(int id) { m_id = id; }       // implicit this-> (same thing)
};
```

### Member Function Chaining

Return `*this` to enable chaining:

```cpp
class Calc {
    int m_value{};
public:
    Calc& add(int v) { m_value += v; return *this; }
    Calc& sub(int v) { m_value -= v; return *this; }
    Calc& mult(int v) { m_value *= v; return *this; }
    int getValue() const { return m_value; }
};

// Chained calls:
Calc c{};
c.add(5).sub(3).mult(4);  // (5 - 3) * 4 = 8
```

### Resetting an Object

```cpp
void reset() { *this = {}; }  // assign default-constructed object to self
```

---

## 15.2 Classes and Header Files

### Typical Layout

```cpp
// date.h
#ifndef DATE_H
#define DATE_H

class Date {
    int m_year{};
    int m_month{};
    int m_day{};

public:
    Date(int year, int month, int day);  // declaration only
    void print() const;

    int getYear() const { return m_year; }  // trivial — define inline
};

#endif
```

```cpp
// date.cpp
#include "date.h"
#include <iostream>

Date::Date(int year, int month, int day)  // definition with Date:: scope
    : m_year{ year }, m_month{ month }, m_day{ day } {}

void Date::print() const {
    std::cout << m_year << '/' << m_month << '/' << m_day;
}
```

### Rules
- **Trivial member functions** (getters, one-liners): define inside the class (implicitly inline)
- **Non-trivial member functions**: declare in header, define in .cpp file
- Use `ClassName::` prefix when defining outside the class

---

## 15.3 Nested Types (Member Types)

```cpp
class Fruit {
public:
    enum Type { apple, banana, cherry };  // nested enum

private:
    Type m_type{};
    int m_amount{};

public:
    Fruit(Type type, int amount) : m_type{type}, m_amount{amount} {}
};

Fruit f{ Fruit::apple, 5 };  // access nested type with Fruit::
```

---

## 15.4 Introduction to Destructors

```cpp
class NetworkConnection {
    int m_id{};
public:
    NetworkConnection(int id) : m_id{id} {
        std::cout << "Opening connection " << m_id << '\n';
    }

    ~NetworkConnection() {  // destructor
        std::cout << "Closing connection " << m_id << '\n';
    }
};
```

### Key Facts
- Named `~ClassName()` — no parameters, no return type
- Called automatically when the object is destroyed
- Destruction order: **reverse** of construction (LIFO)
- If you don't write one, the compiler generates an implicit empty destructor
- Don't call destructors explicitly

### RAII (Resource Acquisition Is Initialization)

The idiom of acquiring resources in the constructor and releasing them in the destructor:
- Constructor: open file, allocate memory, connect to network
- Destructor: close file, free memory, disconnect

> **Key benefit**: Cleanup happens automatically — no forgetting to release resources.

---

## 15.5 Class Templates with Member Functions

```cpp
template <typename T>
class Pair {
    T m_first{};
    T m_second{};
public:
    Pair(T first, T second) : m_first{first}, m_second{second} {}

    T getFirst() const { return m_first; }
    T getSecond() const { return m_second; }
};
```

Defining member functions outside the class:

```cpp
template <typename T>
T Pair<T>::getFirst() const {
    return m_first;
}
```

> **Reminder**: Template definitions must be in header files.

---

## 15.6 Static Member Variables

Static members are **shared** across all instances:

```cpp
class Something {
public:
    static inline int s_value{ 1 };  // C++17: inline static in class
};

// Access via class name:
std::cout << Something::s_value;
```

### Pre-C++17 (Out-of-class Definition)

```cpp
class Something {
public:
    static int s_value;  // declaration
};
int Something::s_value{ 1 };  // definition (in .cpp file)
```

### Example: Auto-incrementing IDs

```cpp
class Widget {
    static inline int s_nextId{ 0 };
    int m_id{};
public:
    Widget() : m_id{ s_nextId++ } {}
    int getId() const { return m_id; }
};
```

---

## 15.7 Static Member Functions

```cpp
class IDGenerator {
    static inline int s_nextId{ 0 };
public:
    static int getNextId() { return s_nextId++; }  // no object needed
};

int id{ IDGenerator::getNextId() };  // called on the class, not an object
```

- **No `this` pointer** — they don't operate on any instance
- Can only access static members (not instance members)
- Called using `ClassName::functionName()`

---

## 15.8 Friend Non-Member Functions

```cpp
class Value {
    int m_value{};
public:
    Value(int v) : m_value{v} {}
    friend bool isEqual(const Value& a, const Value& b);  // friend declaration
};

bool isEqual(const Value& a, const Value& b) {
    return a.m_value == b.m_value;  // can access private m_value!
}
```

- `friend` grants a non-member function access to private members
- The function is NOT a member of the class
- Common use: overloading `operator<<`

---

## 15.9 Friend Classes and Friend Member Functions

```cpp
class Storage {
    int m_value{};
    friend class Display;  // Display can access Storage's privates
};

class Display {
public:
    void show(const Storage& s) {
        std::cout << s.m_value;  // OK: Display is a friend of Storage
    }
};
```

> **Best Practice**: Use friendship sparingly — it breaks encapsulation. Prefer public interfaces when possible.

---

## 15.10 Ref Qualifiers

Control which value category can call a member function:

```cpp
class Foo {
public:
    void doSomething() & { }   // only callable on lvalues
    void doSomething() && { }  // only callable on rvalues
};
```

---

## Chapter 15 — Quick Reference Card

| Concept | Rule |
|---------|------|
| `this` pointer | Implicit; points to current object |
| Function chaining | Return `*this` by reference |
| Header vs .cpp | Trivial functions in header; complex in .cpp |
| Destructor | `~ClassName()` — auto cleanup (RAII) |
| Static members | Shared across all instances; `inline static` in C++17 |
| Static functions | No `this`; access only static members |
| Friend functions | Access private members; use sparingly |
| Template classes | Define in header files |
