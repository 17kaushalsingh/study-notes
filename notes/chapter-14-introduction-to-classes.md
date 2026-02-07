# Chapter 14: Introduction to Classes

## 14.1 Introduction to Object-Oriented Programming

OOP bundles **data** and **functions** that operate on that data into a single unit (a class). Key concepts:
- **Encapsulation**: Hide implementation details, expose a clean interface
- **Class invariant**: A condition that must always be true for an object to be valid

---

## 14.2 Introduction to Classes

```cpp
class Date {
public:
    int m_year{};
    int m_month{};
    int m_day{};
};

Date today{ 2024, 1, 15 };
```

### Struct vs Class

| Feature | `struct` | `class` |
|---------|----------|---------|
| Default access | Public | **Private** |
| Convention | Data-only aggregates | Data + behavior, with invariants |
| Constructors | Usually none (aggregate) | Usually has constructors |

> **Use struct** for passive data bundles. **Use class** when you need encapsulation, invariants, or member functions.

---

## 14.3 Member Functions

```cpp
class Date {
public:
    int m_year{};
    int m_month{};
    int m_day{};

    void print() {  // member function
        std::cout << m_year << '/' << m_month << '/' << m_day;
    }
};

Date today{ 2024, 1, 15 };
today.print();  // calls member function on 'today' object
```

- Member functions can access all members of the class (including private)
- Called using the dot operator on an object

---

## 14.4 Const Class Objects and Const Member Functions

```cpp
class Date {
    int m_year{};
public:
    int getYear() const { return m_year; }  // const member function
    void setYear(int y) { m_year = y; }     // non-const (modifies)
};

const Date birthday{ 2000, 6, 15 };
birthday.getYear();   // OK: const function on const object
// birthday.setYear(2001);  // ERROR: can't call non-const on const object
```

> **Best Practice**: Make member functions `const` if they don't modify the object. This allows them to be called on const objects/references.

---

## 14.5 Public and Private Members

```cpp
class Employee {
private:                    // only accessible inside the class
    std::string m_name{};
    int m_id{};

public:                     // accessible by anyone
    void setName(std::string_view name) { m_name = name; }
    std::string_view getName() const { return m_name; }
};
```

### Naming Convention
- Private members: `m_` prefix (e.g., `m_name`, `m_id`)
- Public members: no prefix

---

## 14.6 Access Functions (Getters/Setters)

```cpp
class Fraction {
    int m_num{};
    int m_den{ 1 };
public:
    int getNum() const { return m_num; }       // getter
    void setNum(int num) { m_num = num; }      // setter
};
```

> **Best Practice**: Don't provide getters/setters for everything. Only expose what's needed. Setters should validate invariants.

---

## 14.7 Member Functions Returning References

```cpp
class Employee {
    std::string m_name{};
public:
    const std::string& getName() const { return m_name; }  // returns const ref
};
```

- Return `const&` for read-only access to expensive-to-copy members
- Be careful about dangling references if the object is destroyed

---

## 14.8 The Benefits of Data Hiding (Encapsulation)

Why make data private:
1. **Control access**: Validate inputs through setters
2. **Maintain invariants**: Prevent invalid states
3. **Change implementation**: Internal changes don't break external code
4. **Easier debugging**: Fewer places can modify data

---

## 14.9 Introduction to Constructors

```cpp
class Fraction {
    int m_num{};
    int m_den{ 1 };

public:
    Fraction() {}                              // default constructor
    Fraction(int num, int den = 1)             // parameterized constructor
        : m_num{ num }, m_den{ den } {}
};

Fraction f1;            // calls default constructor
Fraction f2{ 3, 4 };   // calls parameterized constructor
Fraction f3{ 5 };      // den defaults to 1
```

### Constructor Rules
- Same name as the class
- No return type
- Usually public
- Constructors don't create objects — they initialize them
- Cannot be const (they must modify the object)

---

## 14.10 Constructor Member Initializer Lists

```cpp
class Foo {
    int m_x{};
    int m_y{};

public:
    Foo(int x, int y)
        : m_x{ x }    // initialize m_x with x
        , m_y{ y }     // initialize m_y with y
    {
        // constructor body (for additional setup)
    }
};
```

### Why Use Initializer Lists
- **Required** for `const` members and references (can't assign)
- More efficient than assigning in the body
- The object is considered **initialized** after the initializer list completes

> **Critical**: Members initialize in **declaration order** (order in the class), NOT the order in the initializer list!

---

## 14.11 Default Constructors

A **default constructor** takes no arguments (or has all default parameters):

```cpp
class Foo {
public:
    Foo() = default;  // explicitly request compiler-generated default
};
```

- If you provide NO constructors, the compiler generates an implicit default constructor
- If you provide ANY constructor, the implicit default is NOT generated
- Use `= default` to get both a custom constructor AND a default constructor

---

## 14.12 Delegating Constructors

One constructor can call another:

```cpp
class Employee {
    std::string m_name{};
    int m_id{};
public:
    Employee(std::string_view name, int id)
        : m_name{ name }, m_id{ id } {}

    Employee(std::string_view name)
        : Employee{ name, 0 } {}     // delegates to the 2-param version
};
```

---

## 14.13 Temporary Class Objects

```cpp
Fraction f{ add(Fraction{ 1, 2 }, Fraction{ 3, 4 }) };
//         ^^^^^^^^^^^^^^^^^^^^^^  ^^^^^^^^^^^^^^^^
//         temporary objects (destroyed at end of expression)
```

---

## 14.14 Introduction to the Copy Constructor

```cpp
class Fraction {
public:
    Fraction(const Fraction& other)       // copy constructor
        : m_num{ other.m_num }
        , m_den{ other.m_den }
    {}
};

Fraction f1{ 3, 4 };
Fraction f2{ f1 };      // copy constructor called
```

### When the Copy Constructor is Called
1. Direct initialization from another object: `Fraction f2{ f1 };`
2. Pass by value
3. Return by value

### Implicit Copy Constructor
- Compiler generates one if you don't write one
- Performs **memberwise copy** (copies each member)
- Usually sufficient — write your own only for special cases (like managing raw pointers)

> **Rule of Three/Five**: If you need a custom copy constructor, you likely also need a custom destructor and copy assignment operator (+ move constructor/assignment in C++11).

---

## 14.15 Class Initialization and Copy Elision

The compiler can optimize away unnecessary copies:

```cpp
Fraction f{ Fraction{ 1, 2 } };  // copy may be elided (guaranteed in C++17)
```

**Copy elision** means the compiler constructs the object directly in the destination, skipping the copy constructor entirely.

---

## 14.16 Converting Constructors and explicit

A **converting constructor** allows implicit conversion:

```cpp
class Fraction {
public:
    Fraction(int num, int den = 1);  // converting constructor
};

void print(Fraction f);
print(5);  // int 5 implicitly converted to Fraction{5, 1}
```

### The explicit Keyword

```cpp
class Fraction {
public:
    explicit Fraction(int num, int den = 1);  // no implicit conversion
};

print(5);              // ERROR: no implicit conversion
print(Fraction{ 5 });  // OK: explicit construction
```

> **Best Practice**: Make constructors with a single non-defaulted parameter `explicit` to prevent unexpected implicit conversions.

---

## 14.17 Constexpr Aggregates and Classes

```cpp
class Pair {
    int m_x{};
    int m_y{};
public:
    constexpr Pair(int x, int y) : m_x{x}, m_y{y} {}
    constexpr int getX() const { return m_x; }
};

constexpr Pair p{ 3, 4 };        // compile-time construction
static_assert(p.getX() == 3);    // compile-time evaluation
```

---

## Chapter 14 — Quick Reference Card

| Concept | Rule |
|---------|------|
| struct vs class | struct = data only; class = data + behavior |
| Default access | struct = public; class = private |
| Const member functions | Mark with `const` if they don't modify state |
| Member initializer list | Prefer over assignment; list in declaration order |
| Default constructor | Use `= default` if you also have other constructors |
| Copy constructor | Implicit one is usually sufficient |
| `explicit` | Use on single-parameter constructors |
| Private members | Use `m_` prefix |
