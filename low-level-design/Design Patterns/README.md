# Design Patterns

C++ implementations of common design patterns with progressive examples.

---

## 1. Strategy Pattern (`strategy.cpp`)

### Intent
Define a family of algorithms, encapsulate each one, and make them interchangeable.

### Structure
```
Walk (Interface)          Talk (Interface)
├── NormalWalk            ├── NormalTalk
├── WalkWithWheels        └── NoTalk
└── NoWalk

Robot
├── Walk* walkBehavior
└── Talk* talkBehavior
```

### Key Points
- **Composition over inheritance**: Robot HAS-A walk/talk behavior
- **Runtime flexibility**: Behaviors can be swapped at runtime
- **Avoids class explosion**: No need for WheelTalkingRobot, WheelSilentRobot, etc.

### When to Use
- Multiple algorithms for a task
- Avoid conditional statements for behavior selection
- Need to swap behavior at runtime

### Quick Example
```cpp
Robot* r1 = new Robot(new WalkWithWheels(), new NormalTalk());
Robot* r2 = new Robot(new NoWalk(), new NoTalk());
```

---

## 2. Singleton Pattern (`singleton.cpp`)

### Intent
Ensure a class has only one instance and provide global access to it.

### Progressive Implementations

| Version | Description | Thread-Safe |
|---------|-------------|-------------|
| Regular class | Multiple instances possible | N/A |
| Private constructor + getter | Still creates multiple instances | No |
| Static instance check | Basic singleton | No |
| Mutex locking | Thread-safe | Yes |
| Double-checked locking | Optimized thread-safe | Yes |
| Eager initialization | Instance created at load time | Yes |

### Key Implementation
```cpp
class SingletonClass {
private:
    static SingletonClass* obj;
    SingletonClass() {}  // Private constructor

public:
    static SingletonClass* getInstance() {
        if (obj == nullptr) {
            obj = new SingletonClass();
        }
        return obj;
    }
};
SingletonClass* SingletonClass::obj = nullptr;
```

### Thread-Safe (Double-Checked Locking)
```cpp
static SingletonClass* getInstance() {
    if (obj == nullptr) {                    // First check (no lock)
        std::lock_guard<std::mutex> lock(mtx);
        if (obj == nullptr) {                // Second check (with lock)
            obj = new SingletonClass();
        }
    }
    return obj;
}
```

### Eager vs Lazy Initialization
| Type | When Created | Pros | Cons |
|------|--------------|------|------|
| **Lazy** | On first `getInstance()` | Memory efficient | Thread-safety complexity |
| **Eager** | At program start | Thread-safe by default | Uses memory even if unused |

---

## 3. Factory Pattern (`factory.cpp`)

### Three Variants Implemented

### A. Simple Factory
```
BurgerFactory
└── createObject(type) → BasicBurger / StandardBurger / PremiumBurger
```
- Single factory class with conditional creation
- Not a true "pattern", more of an idiom
- **Limitation**: Adding types requires modifying factory

### B. Factory Method
```
BurgerFactory (Abstract)
├── KingBurgers → Basic, Standard, Premium (normal dough)
└── SinghBurgers → Basic, Standard, Premium (multigrain)
```
- Each factory subclass decides which product to create
- **Key**: Subclasses override the factory method
- **Use when**: One product family, multiple variants

### C. Abstract Factory
```
MealFactory (Abstract)
├── createBurger()
└── createGarlicBread()

KingsFastFood                    SinghsFastFood
├── ClassicBurger                ├── MultigrainClassicBurger
├── CheeseBurger                 ├── MultigrainCheeseBurger
├── ClassicGarlicBread           ├── MultigrainClassicGarlicBread
└── CheeseGarlicBread            └── MultigrainCheeseGarlicBread
```
- Creates **families of related objects**
- **Use when**: Multiple product types that must be consistent

### Comparison Table

| Aspect | Simple Factory | Factory Method | Abstract Factory |
|--------|---------------|----------------|------------------|
| **Creates** | One product type | One product type | Multiple product types |
| **Extensibility** | Modify factory | Add new factory subclass | Add new factory subclass |
| **Complexity** | Low | Medium | High |
| **Use case** | Simple creation | Product variants | Product families |

---

## 4. Observer Pattern (`observer.cpp`)

### Intent
Define a one-to-many dependency between objects so that when one object (subject) changes state, all its dependents (observers) are notified and updated automatically.

### Structure
```
I_Observer (Interface)           I_Observable (Interface)
└── update()                     ├── add(observer)
                                 ├── remove(observer)
                                 └── notify()

C_Observer : I_Observer          C_Observable : I_Observable
├── subject: I_Observable*       └── observers: unordered_set<I_Observer*>
└── update()
```

### Key Points
- **Loose coupling**: Subject knows nothing about observers except they implement `I_Observer`
- **Dynamic relationships**: Observers can be added/removed at runtime
- **Broadcast communication**: Subject notifies all registered observers without knowing their concrete types

### When to Use
- When a change to one object requires changing others, and you don't know how many objects need to change
- When an object should notify other objects without making assumptions about who those objects are
- Implementing event handling systems, MVC patterns, or pub-sub mechanisms

### Implementation Details
```cpp
class I_Observer {
public:
    virtual void update() = 0;
};

class I_Observable {
public:
    virtual void add(I_Observer* observer) = 0;
    virtual void remove(I_Observer* observer) = 0;
    virtual void notify() = 0;
};

class C_Observable : public I_Observable {
    std::unordered_set<I_Observer*> observers;
public:
    void notify() override {
        for (auto &observer : observers) {
            observer->update();
        }
    }
};
```

### Quick Example
```cpp
C_Observable* subject = new C_Observable();
C_Observer* observer1 = new C_Observer(subject);
C_Observer* observer2 = new C_Observer(subject);

subject->add(observer1);
subject->add(observer2);
subject->notify();    // Both observers receive update

subject->remove(observer1);
subject->notify();    // Only observer2 receives update
```

### Push vs Pull Model
| Model | Description | Pros | Cons |
|-------|-------------|------|------|
| **Push** | Subject sends data in `update(data)` | Observers get exactly what they need | Subject must know what observers need |
| **Pull** | Observers query subject after `update()` | Observers pull only what they need | May require multiple calls to subject |

*Current implementation uses a simple notification (neither push nor pull with data).*

---

## Quick Revision Questions

1. **Strategy vs Inheritance?**
   - Strategy uses composition, allows runtime changes
   - Inheritance is static, leads to class explosion

2. **Why double-checked locking?**
   - First check avoids lock overhead when instance exists
   - Second check ensures thread-safety during creation

3. **When Abstract Factory over Factory Method?**
   - When you need to create multiple related products that must be consistent

4. **How to add a new burger type in Abstract Factory?**
   - Add method to `MealFactory` interface
   - Implement in all concrete factories

5. **Observer: Why use an interface for Observer?**
   - Allows subject to work with any observer type
   - Enables loose coupling between subject and observers
   - New observer types can be added without modifying subject

6. **When to use Observer over direct method calls?**
   - When you don't know at compile time which objects need to react
   - When the number of dependent objects may change dynamically
   - When you want to avoid tight coupling between communicating objects

---

## Compile & Run
```bash
g++ -o strategy strategy.cpp && ./strategy
g++ -o singleton singleton.cpp && ./singleton
g++ -o factory factory.cpp && ./factory
g++ -o observer observer.cpp && ./observer
```
