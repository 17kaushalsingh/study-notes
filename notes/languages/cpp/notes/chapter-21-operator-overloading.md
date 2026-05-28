# Chapter 21: Operator Overloading

## 21.1 Introduction to Operator Overloading

Operator overloading lets you define how operators work with your custom types.

### Rules
- At least one operand must be a user-defined type
- Can't change operator precedence, associativity, or operand count
- Can't create new operators
- Can't overload: `?:`, `sizeof`, `::`, `.`, `.*`, `typeid`, casts

### Three Ways to Overload

| Method | When to use |
|--------|-------------|
| Friend function | Binary operators where left operand isn't your class |
| Member function | Operators that modify the left operand |
| Normal function | When you don't need private member access |

---

## 21.2-21.3 Overloading Arithmetic Operators

### As Friend Functions

```cpp
class Cents {
    int m_cents{};
public:
    Cents(int cents) : m_cents{cents} {}

    friend Cents operator+(const Cents& a, const Cents& b) {
        return Cents{ a.m_cents + b.m_cents };
    }
};

Cents sum{ Cents{5} + Cents{10} };  // 15 cents
```

### As Normal Functions (using getters)

```cpp
Cents operator+(const Cents& a, const Cents& b) {
    return Cents{ a.getCents() + b.getCents() };
}
```

> **Return by value** for arithmetic operators (they create new values).

---

## 21.4 Overloading the I/O Operators

### operator<< (Output)

```cpp
class Point {
    double m_x{}, m_y{}, m_z{};
public:
    Point(double x, double y, double z) : m_x{x}, m_y{y}, m_z{z} {}

    friend std::ostream& operator<<(std::ostream& out, const Point& p) {
        out << "Point(" << p.m_x << ", " << p.m_y << ", " << p.m_z << ")";
        return out;  // return stream for chaining
    }
};

std::cout << Point{1, 2, 3};  // Point(1, 2, 3)
```

### operator>> (Input)

```cpp
friend std::istream& operator>>(std::istream& in, Point& p) {
    double x{}, y{}, z{};
    if (in >> x >> y >> z)       // extract to temporaries first
        p = Point{ x, y, z };   // only assign if ALL succeeded
    return in;
}
```

> **Best Practice**: Extract to temporaries first, then assign — prevents partial/corrupted state.

---

## 21.5 Overloading Using Member Functions

```cpp
class Cents {
    int m_cents{};
public:
    Cents(int cents) : m_cents{cents} {}

    // Member function: left operand is implicit (*this)
    Cents operator+(const Cents& rhs) const {
        return Cents{ m_cents + rhs.m_cents };
    }
};
```

### When to Use Member vs Friend

| Operator | Implement as |
|----------|-------------|
| `=`, `[]`, `()`, `->` | **Must** be member functions |
| `<<`, `>>` | **Must** be non-member (friend) |
| Unary (`+`, `-`, `!`) | Member function |
| Binary arithmetic | Friend or normal function |
| Comparison | Friend or member |

---

## 21.6 Overloading Unary Operators

```cpp
class Cents {
    int m_cents{};
public:
    Cents operator-() const { return Cents{ -m_cents }; }   // negation
    bool operator!() const { return m_cents == 0; }          // logical NOT
};
```

---

## 21.7 Overloading Comparison Operators

```cpp
class Cents {
    int m_cents{};
public:
    friend bool operator==(const Cents& a, const Cents& b) {
        return a.m_cents == b.m_cents;
    }
    friend bool operator!=(const Cents& a, const Cents& b) {
        return !(a == b);    // reuse operator==
    }
    friend bool operator<(const Cents& a, const Cents& b) {
        return a.m_cents < b.m_cents;
    }
    friend bool operator>(const Cents& a, const Cents& b) {
        return b < a;        // reuse operator<
    }
    friend bool operator<=(const Cents& a, const Cents& b) {
        return !(a > b);
    }
    friend bool operator>=(const Cents& a, const Cents& b) {
        return !(a < b);
    }
};
```

### C++20: Spaceship Operator `<=>`

```cpp
#include <compare>

class Cents {
    int m_cents{};
public:
    auto operator<=>(const Cents&) const = default;  // generates ALL 6 comparisons!
};
```

---

## 21.8 Overloading Increment/Decrement

```cpp
class Digit {
    int m_digit{};
public:
    Digit& operator++() {        // PREFIX: ++d
        ++m_digit;
        return *this;
    }

    Digit operator++(int) {      // POSTFIX: d++ (dummy int parameter)
        Digit temp{ *this };
        ++(*this);
        return temp;
    }
};
```

---

## 21.9 Overloading the Subscript Operator

```cpp
class IntList {
    int m_list[10]{};
public:
    int& operator[](int index) {
        return m_list[index];  // returns reference for assignment
    }
    const int& operator[](int index) const {
        return m_list[index];  // const version for const objects
    }
};

IntList list;
list[3] = 42;          // calls non-const version
```

---

## 21.10 Overloading the Parenthesis Operator (Functors)

```cpp
class Accumulator {
    int m_total{};
public:
    int operator()(int value) {  // callable like a function!
        m_total += value;
        return m_total;
    }
};

Accumulator acc;
acc(5);   // 5
acc(10);  // 15
```

Objects with `operator()` are called **functors** — useful as custom comparators for `std::sort`.

---

## 21.11 Overloading Typecasts

```cpp
class Cents {
    int m_cents{};
public:
    operator int() const { return m_cents; }  // convert Cents to int
};

Cents c{ 42 };
int x{ static_cast<int>(c) };  // 42
```

> **Best Practice**: Make conversion operators `explicit` to prevent implicit conversions.

---

## 21.12 Overloading the Assignment Operator

```cpp
class MyString {
    char* m_data{};
    int m_length{};
public:
    MyString& operator=(const MyString& rhs) {
        if (this == &rhs)       // self-assignment check!
            return *this;

        delete[] m_data;        // free existing memory

        m_length = rhs.m_length;
        m_data = new char[m_length + 1];
        std::copy_n(rhs.m_data, m_length + 1, m_data);

        return *this;
    }
};
```

> **Always** check for self-assignment when overloading `operator=` with dynamic memory.

---

## 21.13 Shallow vs Deep Copying

### Shallow Copy (Default)
Copies pointer values — both objects point to **same** memory:
```
Original: m_data → [H][e][l][l][o]
Copy:     m_data → [H][e][l][l][o]  ← SAME memory!
```
**Problem**: When one is destroyed, the other has a **dangling pointer**.

### Deep Copy
Allocates new memory and copies the data:
```
Original: m_data → [H][e][l][l][o]
Copy:     m_data → [H][e][l][l][o]  ← DIFFERENT memory
```

### Rule of Three
If your class needs any of these, it probably needs all three:
1. **Destructor**
2. **Copy constructor**
3. **Copy assignment operator**

### Rule of Five (C++11)
Add:
4. **Move constructor**
5. **Move assignment operator**

---

## Chapter 21 — Quick Reference Card

| Concept | Rule |
|---------|------|
| `=`, `[]`, `()`, `->` | Must be member functions |
| `<<`, `>>` | Must be non-member (friend) |
| Arithmetic ops | Return by value |
| Assignment ops | Return `*this` by reference |
| Self-assignment | Always check in `operator=` |
| All 6 comparisons | Use `<=>` in C++20 |
| Functors | Objects with `operator()` |
| Rule of Three/Five | Custom destructor → need copy/move ops too |
| Shallow vs Deep | Deep copy when managing dynamic memory |
