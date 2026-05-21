# 02 Classes and Objects

## Classes and Objects

A class is a type definition; an object is a runtime instance of that type.

```java
public class BankAccount {
    String owner;
    double balance;
}

BankAccount account = new BankAccount();
account.owner = "Ada";
account.balance = 1000.0;
```

Class design separates:

- State: fields
- Behavior: methods
- Invariants: properties that must remain true

## Attributes and Methods

Attributes are fields that represent object state. Methods operate on that state.

```java
public class Counter {
    private int value;

    public void increment() {
        value++;
    }

    public int getValue() {
        return value;
    }
}
```

Well-designed methods preserve invariants:

```text
invariant: balance >= 0
deposit(amount): require amount > 0
withdraw(amount): require 0 < amount <= balance
```

## Access Specifiers

Java access control shapes encapsulation boundaries.

- `public`: visible everywhere
- `protected`: visible in package and subclasses
- package-private: visible in package only
- `private`: visible in declaring class only

```java
public class AccessExample {
    public int a = 1;
    protected int b = 2;
    int c = 3;       // package-private
    private int d = 4;
}
```

Prefer the least permissive access that still supports the intended API.

## Static Keyword

`static` binds a member to the class rather than to instances.

```java
public class IdGenerator {
    private static int nextId = 1;

    public static int issueId() {
        return nextId++;
    }
}
```

Conceptually:

- Instance member access requires an object.
- Static member access requires class loading, not object construction.

Common uses:

- Constants
- Utility methods
- Factories
- Shared counters or caches

## Nested Classes

Java supports several nested type forms:

- Static nested classes
- Inner classes
- Local classes
- Anonymous classes

```java
public class Outer {
    private int x = 10;

    static class StaticNested {
        void show() {
            System.out.println("No implicit outer instance");
        }
    }

    class Inner {
        void show() {
            System.out.println(x); // can access outer instance state
        }
    }
}
```

Design tradeoffs:

- Static nested classes reduce accidental coupling to outer instances.
- Inner classes are useful when behavior is tightly bound to a specific outer object.
- Anonymous classes were historically common for callbacks; lambdas now cover many such cases.
