# Chapter 24: Inheritance

## 24.1 Introduction to Inheritance

Inheritance models an **"is-a"** relationship: a Derived class IS A type of Base class.

```cpp
class Person {
    std::string m_name;
    int m_age{};
public:
    Person(std::string_view name, int age) : m_name{name}, m_age{age} {}
    std::string_view getName() const { return m_name; }
    int getAge() const { return m_age; }
};

class Employee : public Person {   // Employee IS A Person
    int m_employeeId{};
public:
    Employee(std::string_view name, int age, int id)
        : Person{name, age}, m_employeeId{id} {}
    int getId() const { return m_employeeId; }
};
```

- Derived class inherits all members (variables and functions) from the base
- Derived class can add its own members
- Inheritance chains: `Employee` → `Person` → `LivingThing`

---

## 24.2 Basic Inheritance in C++

```cpp
class Base {
public:
    int m_id{};
    Base(int id) : m_id{id} {}
};

class Derived : public Base {  // public inheritance
    double m_cost{};
public:
    Derived(int id, double cost)
        : Base{id}         // call Base constructor
        , m_cost{cost}
    {}
};
```

---

## 24.3 Order of Construction of Derived Classes

1. Memory allocated for derived object (enough for both Base and Derived parts)
2. **Base constructor** called first
3. **Derived constructor** called second
4. Control returns to caller

Destruction is in **reverse order**: Derived destructor first, then Base destructor.

---

## 24.4 Constructors and Initialization

```cpp
class Derived : public Base {
public:
    Derived(int id, double cost)
        : Base{ id }    // MUST call Base constructor in initializer list
        , m_cost{ cost }
    {}
};
```

- Derived class **cannot directly initialize** base class members
- Must call the appropriate base constructor in the member initializer list
- If no base constructor is specified, the default base constructor is called

---

## 24.5 Inheritance and Access Specifiers

| Access | Base class | Derived class | Outside |
|--------|-----------|---------------|---------|
| `public` | Yes | Yes | Yes |
| `protected` | Yes | Yes | **No** |
| `private` | Yes | **No** | **No** |

### Types of Inheritance

| Inheritance type | public members become | protected become | private become |
|-----------------|----------------------|-----------------|---------------|
| `public` (most common) | public | protected | inaccessible |
| `protected` | protected | protected | inaccessible |
| `private` | private | private | inaccessible |

> **Best Practice**: Almost always use `public` inheritance. Protected/private inheritance is rare.

---

## 24.6 Adding New Functionality to a Derived Class

Simply add new members:

```cpp
class Derived : public Base {
    int m_extra{};
public:
    void newFunction() {
        std::cout << "Only Derived has this!\n";
    }
};
```

---

## 24.7 Calling Inherited Functions and Overriding Behavior

### Overriding a Base Function

```cpp
class Base {
public:
    void identify() const { std::cout << "Base\n"; }
};

class Derived : public Base {
public:
    void identify() const {          // overrides Base::identify()
        std::cout << "Derived\n";
    }
};

Derived d;
d.identify();          // "Derived"
d.Base::identify();    // "Base" — explicitly call base version
```

### Calling the Base Version from Override

```cpp
class Derived : public Base {
public:
    void identify() const {
        Base::identify();               // call base version first
        std::cout << " (Derived)\n";    // then add to it
    }
};
```

---

## 24.8 Hiding Inherited Functionality

### Changing Access Level

```cpp
class Base {
public:
    int m_value{};
};

class Derived : public Base {
private:
    using Base::m_value;  // make m_value private in Derived
};
```

### Deleting Functions

```cpp
class Derived : public Base {
public:
    void unwantedFunction() = delete;
};
```

---

## 24.9 Multiple Inheritance

A class can inherit from multiple base classes:

```cpp
class Teacher : public Person, public Employee {
    // inherits from both Person AND Employee
};
```

### The Diamond Problem

```
    Person
   /      \
Teacher  Employee
   \      /
    TA (Teaching Assistant)
```

`TA` has TWO copies of `Person` — ambiguous! Solution: **virtual inheritance** (Chapter 25).

> **Best Practice**: Avoid multiple inheritance when possible — it adds complexity. Use interfaces (abstract classes) instead.

---

## Chapter 24 — Quick Reference Card

| Concept | Rule |
|---------|------|
| Inheritance type | Almost always `public` |
| Construction order | Base first, then Derived |
| Destruction order | Derived first, then Base |
| Init base members | Call Base constructor in init list |
| Override function | Same name/signature in Derived |
| Call base version | `Base::functionName()` |
| `protected` | Accessible in derived, not outside |
| Multiple inheritance | **Avoid** if possible |
