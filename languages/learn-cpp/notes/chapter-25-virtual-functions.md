# Chapter 25: Virtual Functions

## 25.1 Pointers and References to the Base Class

A base pointer/reference can point to a derived object:

```cpp
Derived d{ 5 };
Base& ref{ d };      // OK: Derived IS-A Base
Base* ptr{ &d };     // OK

ref.getName();       // calls Base::getName() — NOT Derived!
```

Without virtual functions, the **type of the pointer/reference** determines which function is called, not the actual object type.

---

## 25.2 Virtual Functions and Polymorphism

The `virtual` keyword enables **runtime polymorphism** — the actual object type determines which function is called:

```cpp
class Base {
public:
    virtual std::string_view getName() const { return "Base"; }
};

class Derived : public Base {
public:
    std::string_view getName() const override { return "Derived"; }
};

Derived d;
Base& ref{ d };
ref.getName();    // "Derived" — virtual dispatch!
```

### How It Works
- The compiler checks the **actual object type** at runtime
- Calls the **most-derived** version of the function
- Only works through **pointers or references** (not direct objects)

### Key Rules
- If Base marks a function `virtual`, all overrides in derived classes are **implicitly virtual**
- Override must match: same name, parameters, const-ness, and return type
- **Never** call virtual functions from constructors/destructors

---

## 25.3 The override and final Specifiers

### override (Always Use)

```cpp
class Derived : public Base {
public:
    std::string_view getName() const override { return "Derived"; }
    //                                ^^^^^^^^ compiler checks this IS an override
};
```

Without `override`, a typo or mismatched signature creates a NEW function instead of overriding — a silent bug.

### final

```cpp
class A {
public:
    virtual void func() final { }  // no class can override this
};

class B final : public A {        // no class can inherit from B
};
```

### Covariant Return Types

An override can return a pointer/reference to a more derived type:

```cpp
class Base {
public:
    virtual Base* clone() const { return new Base(*this); }
};
class Derived : public Base {
public:
    Derived* clone() const override { return new Derived(*this); }  // OK
};
```

---

## 25.4 Virtual Destructors

```cpp
class Base {
public:
    virtual ~Base() = default;  // MUST be virtual if class is used polymorphically
};

class Derived : public Base {
    int* m_data{};
public:
    ~Derived() { delete m_data; }
};

Base* ptr{ new Derived{} };
delete ptr;  // without virtual destructor: ONLY ~Base() called = LEAK!
             // with virtual destructor: ~Derived() called first, then ~Base() = correct
```

> **Rule**: If a class has ANY virtual function, give it a **virtual destructor**.

---

## 25.5 Early Binding vs Late Binding

| Binding | When resolved | Mechanism | Performance |
|---------|--------------|-----------|-------------|
| Early (static) | Compile time | Direct function call | Faster |
| Late (dynamic) | Runtime | Virtual table lookup | Slightly slower |

---

## 25.6 The Virtual Table (vtable)

- Each class with virtual functions gets a **vtable** — an array of function pointers
- Each object has a hidden **vptr** pointing to its class's vtable
- When a virtual function is called through a pointer/reference, the vptr is followed to the vtable, which contains the correct function address
- Adds one pointer of overhead per object

---

## 25.7 Pure Virtual Functions, Abstract Classes, and Interfaces

### Pure Virtual Function

```cpp
class Animal {
public:
    virtual std::string_view speak() const = 0;  // = 0 makes it pure virtual
    virtual ~Animal() = default;
};
```

### Abstract Base Class
- Contains at least one pure virtual function
- **Cannot be instantiated** directly
- Derived classes MUST implement all pure virtual functions (or remain abstract)

### Interface Class
An abstract class with **only** pure virtual functions and no data:

```cpp
class IErrorLog {
public:
    virtual bool openLog(std::string_view filename) = 0;
    virtual bool closeLog() = 0;
    virtual bool writeError(std::string_view msg) = 0;
    virtual ~IErrorLog() = default;
};
```

Convention: prefix interface names with `I`.

---

## 25.8 Virtual Base Classes

Solves the **diamond problem** in multiple inheritance:

```cpp
class PoweredDevice {};

class Scanner : virtual public PoweredDevice {};   // virtual inheritance
class Printer : virtual public PoweredDevice {};   // virtual inheritance

class Copier : public Scanner, public Printer {};  // only ONE PoweredDevice
```

---

## 25.9 Object Slicing

When a derived object is assigned to a base object BY VALUE, the derived portion is **sliced off**:

```cpp
Derived d{5};
Base b{ d };         // SLICED — only Base part copied!
```

### Prevention
- Pass/store by **reference or pointer**, never by value:
  ```cpp
  void print(const Base& b);       // reference — no slicing
  std::vector<Base*> v;             // pointers — no slicing
  std::vector<std::unique_ptr<Base>> v;  // smart pointers — best
  ```

---

## 25.10 Dynamic Casting

Safely downcast a base pointer/reference to a derived type:

```cpp
Base* basePtr{ new Derived{} };

Derived* derivedPtr{ dynamic_cast<Derived*>(basePtr) };
if (derivedPtr)  // nullptr if cast fails
    derivedPtr->derivedOnlyFunction();

delete basePtr;
```

### With References

```cpp
try {
    Derived& derivedRef{ dynamic_cast<Derived&>(baseRef) };
} catch (std::bad_cast&) {
    // cast failed
}
```

### Requirements
- Base class must have at least one virtual function (needs RTTI)
- Has runtime overhead

> **Best Practice**: Prefer virtual functions over dynamic_cast. Use dynamic_cast only when you need derived-class-specific functionality.

---

## 25.11 Printing Inherited Classes Using operator<<

```cpp
class Base {
    friend std::ostream& operator<<(std::ostream& out, const Base& b) {
        return b.print(out);   // delegate to virtual function
    }
    virtual std::ostream& print(std::ostream& out) const {
        out << "Base";
        return out;
    }
};

class Derived : public Base {
    std::ostream& print(std::ostream& out) const override {
        out << "Derived";
        return out;
    }
};
```

> **Pattern**: Make `operator<<` a non-virtual friend that calls a virtual `print()` function.

---

## Chapter 25 — Quick Reference Card

| Concept | Rule |
|---------|------|
| Virtual functions | Use for runtime polymorphism |
| `override` | **Always** use on overrides |
| Virtual destructor | **Required** if any virtual functions exist |
| Pure virtual (`= 0`) | Makes class abstract |
| Interface class | Only pure virtual functions + virtual destructor |
| Object slicing | Use references/pointers, never pass by value |
| `dynamic_cast` | Prefer virtual functions instead |
| vtable overhead | One pointer per object |
| Call in constructor | **Never** call virtual functions from constructors |
