# Chapter 23: Object Relationships

## 23.1 Overview of Relationships

| Relationship | Symbol | Meaning | Example |
|-------------|--------|---------|---------|
| Composition | "Part-of" | Part can't exist without whole | Heart is part of Body |
| Aggregation | "Has-a" | Part can exist independently | Car has a Driver |
| Association | "Uses-a" | Objects interact loosely | Doctor uses Patient |
| Dependency | "Depends-on" | Temporary interaction | Function uses Timer |

---

## 23.2 Composition

A **"part-of"** relationship where the whole manages the part's lifecycle:

```cpp
class Heart {};

class Body {
    Heart m_heart;   // Heart created when Body is created
                     // Heart destroyed when Body is destroyed
};
```

### Rules of Composition
- Part can only belong to **one** whole at a time
- Whole manages part's **creation and destruction**
- Part has **no knowledge** of the whole
- "Death relationship" — part dies with the whole

> Implemented using normal member variables.

---

## 23.3 Aggregation

A **"has-a"** relationship where parts can exist independently:

```cpp
class Teacher {};

class Department {
    std::vector<std::reference_wrapper<const Teacher>> m_teachers;
public:
    void add(const Teacher& t) { m_teachers.push_back(t); }
};
```

### Rules of Aggregation
- Part can belong to **multiple** wholes
- Whole does **not** manage part's lifecycle
- Part **can** exist without the whole

> Implemented using pointers or references.

---

## 23.4 Association

A **"uses-a"** relationship — objects interact but neither manages the other:

```cpp
class Patient;

class Doctor {
    std::vector<std::reference_wrapper<const Patient>> m_patients;
public:
    void addPatient(Patient& p);
};
```

- Unidirectional or bidirectional
- No ownership or lifecycle management
- Objects can interact with many others

---

## 23.5 Dependencies

The weakest relationship — one object temporarily uses another:

```cpp
class Timer {};

void doSomething() {
    Timer t;       // dependency: function depends on Timer
    // ... use t ...
}                  // t destroyed when function ends
```

- Typically seen in function parameters or local variables
- No persistent link between objects

---

## 23.6 Container Classes

Classes that hold and manage collections of objects:

```cpp
class IntArray {
    int* m_data{};
    int m_length{};
public:
    IntArray(int length) : m_data{new int[length]{}}, m_length{length} {}
    ~IntArray() { delete[] m_data; }

    int& operator[](int index) { return m_data[index]; }
    int getLength() const { return m_length; }
};
```

> **Best Practice**: Use standard library containers (`std::vector`, `std::array`, `std::map`) instead of writing your own.

---

## 23.7 std::initializer_list

Enable brace initialization for your classes:

```cpp
#include <initializer_list>

class IntArray {
    int* m_data{};
    int m_length{};
public:
    IntArray(std::initializer_list<int> list)
        : m_data{ new int[list.size()] }
        , m_length{ static_cast<int>(list.size()) }
    {
        int i{ 0 };
        for (auto elem : list)
            m_data[i++] = elem;
    }
};

IntArray arr{ 1, 2, 3, 4, 5 };  // uses initializer_list constructor
```

> **Warning**: When a class has both an initializer_list constructor and another matching constructor, brace initialization **prefers** the initializer_list constructor.

---

## Relationship Summary Table

| Relationship | Ownership | Part lifecycle | Can share parts? |
|-------------|-----------|---------------|-----------------|
| Composition | Whole owns part | Managed by whole | No |
| Aggregation | No ownership | Independent | Yes |
| Association | No ownership | Independent | Yes |
| Dependency | No ownership | Temporary | N/A |
