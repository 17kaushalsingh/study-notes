# Chapter 13: Compound Types — Enums and Structs

## 13.1 Introduction to Program-Defined Types

C++ lets you create your own types:
- **Enumerated types**: `enum`, `enum class`
- **Class types**: `struct`, `class`, `union`

Type definitions don't create objects — they just define what the type looks like.

> **Convention**: Define types in global scope (not inside functions, unless only needed locally). Type names start with a capital letter.

---

## 13.2 Unscoped Enumerations

```cpp
enum Color {
    red,        // 0
    green,      // 1
    blue,       // 2
};

Color shirt{ blue };   // shirt = 2
```

### Enumerator Values
- By default: 0, 1, 2, 3, ...
- Can be explicitly set:

```cpp
enum Animal {
    cat = -3,
    dog,        // -2
    pig,        // -1
    horse = 5,
    giraffe,    // 6
    chicken,    // 7
};
```

### Problems with Unscoped Enums
- Enumerators leak into the surrounding scope (naming collisions)
- Implicitly convert to integers (less type-safe)
- Different enum types can be accidentally compared

---

## 13.3 Unscoped Enumerator Integral Conversions

```cpp
enum Color { red, green, blue };

int x{ red };                // OK: implicit enum → int conversion
Color c{ static_cast<Color>(2) };  // explicit int → enum (required)
```

---

## 13.4 Converting an Enumeration to and from a String

A common pattern using switch:

```cpp
constexpr std::string_view getColorName(Color color) {
    switch (color) {
        case red:   return "red";
        case green: return "green";
        case blue:  return "blue";
        default:    return "???";
    }
}
```

---

## 13.5 Introduction to Overloading the I/O Operators

```cpp
std::ostream& operator<<(std::ostream& out, Color color) {
    out << getColorName(color);
    return out;
}

// Now you can: std::cout << red;  // prints "red"
```

---

## 13.6 Scoped Enumerations (enum class) — PREFERRED

```cpp
enum class Color {
    red,
    green,
    blue,
};

Color shirt{ Color::blue };      // must use Color:: prefix
// int x{ Color::red };          // ERROR: no implicit conversion!
int x{ static_cast<int>(Color::red) };  // explicit conversion required
```

### Advantages over Unscoped Enums

| Feature | Unscoped `enum` | Scoped `enum class` |
|---------|----------------|---------------------|
| Namespace pollution | Yes | No (scoped) |
| Implicit int conversion | Yes | No (type-safe) |
| Cross-type comparison | Allowed | Prevented |

### C++20: using enum

```cpp
switch (color) {
    using enum Color;    // brings enumerators into scope
    case red: break;     // no need for Color:: prefix
    case green: break;
}
```

> **Best Practice**: Prefer `enum class` over `enum` for type safety.

---

## 13.7 Introduction to Structs

```cpp
struct Employee {
    int id{};
    int age{};
    double wage{};
};

Employee joe{};           // create an Employee
joe.age = 32;             // member access with dot operator
joe.wage = 60000.0;
```

- Members are accessed with the `.` (member selection) operator
- Each struct instance has its own copy of all members

---

## 13.8 Struct Aggregate Initialization

### List Initialization (Preferred)

```cpp
Employee joe{ 1, 32, 60000.0 };  // in declaration order
```

### Designated Initializers (C++20)

```cpp
Employee joe{ .id = 1, .age = 32, .wage = 60000.0 };
// Can skip members — they get value-initialized:
Employee frank{ .id = 2, .wage = 45000.0 };  // age = 0
```

- Must be in **declaration order**
- Unmentioned members get default/value initialized

### Copy Initialization

```cpp
Employee joe = { 1, 32, 60000.0 };  // also works, less preferred
```

### Assignment with Initializer List

```cpp
joe = { joe.id, 33, 66000.0 };  // update age and wage
```

> **Best Practice**: Add new struct members at the **bottom** to avoid breaking existing initializers.

---

## 13.9 Default Member Initialization

```cpp
struct Rectangle {
    double width{ 1.0 };     // default value
    double height{ 1.0 };    // default value
};

Rectangle r1{};              // 1.0 x 1.0
Rectangle r2{ 3.0, 4.0 };   // 3.0 x 4.0 (overrides defaults)
```

- Explicit initialization overrides default member values
- Provide defaults for members that have a natural default value

---

## 13.10 Passing and Returning Structs

### Passing Structs

```cpp
void printEmployee(const Employee& e) {  // pass by const reference
    std::cout << e.id << ' ' << e.age << ' ' << e.wage;
}
```

### Returning Structs

```cpp
Employee createEmployee(int id, int age, double wage) {
    return Employee{ id, age, wage };   // return by value (OK — move semantics)
}

// Or with designated initializers:
return { .id = id, .age = age, .wage = wage };
```

> **Best Practice**: Pass structs by `const&`. Return structs by value.

---

## 13.11 Struct Miscellany

### Structs with Other Structs (Nesting)

```cpp
struct Point {
    int x{};
    int y{};
};

struct Line {
    Point start{};
    Point end{};
};

Line line{ { 1, 2 }, { 5, 6 } };
```

### Struct Size and Padding

```cpp
struct Foo {
    char a{};     // 1 byte
    int b{};      // 4 bytes
    char c{};     // 1 byte
};
// sizeof(Foo) might be 12, not 6! (due to alignment padding)
```

Compilers add **padding** bytes for alignment. To minimize wasted space, order members from largest to smallest.

---

## 13.12 Member Selection with Pointers and References

```cpp
Employee* ptr{ &joe };
ptr->age = 35;         // arrow operator for pointer member access
(*ptr).age = 35;       // equivalent but uglier
```

| Access type | Syntax |
|------------|--------|
| Direct object | `obj.member` |
| Reference | `ref.member` |
| Pointer | `ptr->member` or `(*ptr).member` |

---

## 13.13 Class Templates

Templates work with structs too:

```cpp
template <typename T>
struct Pair {
    T first{};
    T second{};
};

Pair<int> p1{ 1, 2 };
Pair<double> p2{ 3.14, 2.71 };
```

### Multiple Template Types

```cpp
template <typename T, typename U>
struct Pair {
    T first{};
    U second{};
};

Pair<int, std::string> p{ 42, "hello" };
```

> **Tip**: Use `std::pair` from `<utility>` instead of writing your own.

---

## 13.14 Class Template Argument Deduction (CTAD)

C++17 allows the compiler to deduce template arguments:

```cpp
Pair p{ 1, 2 };           // CTAD: deduces Pair<int>
std::pair p2{ 1, 2.5 };   // deduces std::pair<int, double>
```

### Deduction Guides

For aggregate types, you may need to provide a deduction guide:

```cpp
template <typename T, typename U>
Pair(T, U) -> Pair<T, U>;   // deduction guide
```

> C++20 automatically generates deduction guides for aggregates.

---

## 13.15 Alias Templates

```cpp
template <typename T>
using Coord = Pair<T, T>;    // Coord<int> = Pair<int, int>

Coord<int> point{ 3, 4 };
```

---

## Chapter 13 — Quick Reference Card

| Concept | Rule |
|---------|------|
| Prefer | `enum class` over `enum` |
| Struct initialization | Use list init `{}` or designated initializers |
| Default members | Provide when a natural default exists |
| Pass structs | By `const&` (avoid copies) |
| Return structs | By value (move semantics) |
| Pointer member access | Use `->` operator |
| Struct padding | Order members largest → smallest |
| Class templates | Define in headers |
| CTAD | C++17+; may need deduction guides |
