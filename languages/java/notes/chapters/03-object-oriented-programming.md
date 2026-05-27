# 03 Object-Oriented Programming

## Basics of OOP

Core OOP ideas in Java:

- Encapsulation
- Inheritance
- Polymorphism
- Abstraction

OOP is not merely a syntax style. It is a model for organizing state, behavior, and substitution rules.

## More about OOP

Good object models reduce invalid states and expose coherent operations.

Example domain model:

```java
public class Rectangle {
    private final int width;
    private final int height;

    public Rectangle(int width, int height) {
        if (width <= 0 || height <= 0) {
            throw new IllegalArgumentException("Dimensions must be positive");
        }
        this.width = width;
        this.height = height;
    }

    public int area() {
        return width * height;
    }
}
```

This design encodes invariants in construction rather than relying on callers to behave correctly.

## Method Chaining

Method chaining returns `this` to support fluent APIs.

```java
public class QueryBuilder {
    private String table;
    private String filter;

    public QueryBuilder from(String table) {
        this.table = table;
        return this;
    }

    public QueryBuilder where(String filter) {
        this.filter = filter;
        return this;
    }

    public String build() {
        return "SELECT * FROM " + table + " WHERE " + filter;
    }
}
```

## Enums

Enums define a finite set of instances.

```java
public enum Status {
    NEW, RUNNING, DONE, FAILED
}
```

Enums can contain state and behavior.

```java
public enum Operation {
    ADD {
        public int apply(int a, int b) { return a + b; }
    },
    SUBTRACT {
        public int apply(int a, int b) { return a - b; }
    };

    public abstract int apply(int a, int b);
}
```

## Final Keyword

`final` expresses non-reassignment or non-extension depending on context.

- final variable: assigned once
- final method: cannot be overridden
- final class: cannot be subclassed

```java
public final class Constants {
    public static final double PI = 3.141592653589793;
}
```

`final` improves reasoning but does not make referenced objects immutable.

## Object Lifecycle

Lifecycle phases:

1. Memory allocation
2. Field default initialization
3. Explicit field initialization
4. Constructor execution
5. Use during runtime
6. Becoming unreachable
7. Garbage collection

Pseudo-code:

```text
allocate object
zero-initialize fields
run field initializers
run constructor chain
object becomes usable
when unreachable -> eligible for GC
```

## Inheritance

Inheritance allows a subclass to reuse and specialize behavior from a superclass.

```java
class Animal {
    public void speak() {
        System.out.println("...");
    }
}

class Dog extends Animal {
    @Override
    public void speak() {
        System.out.println("Woof");
    }
}
```

Use inheritance only when a subtype relationship is semantically valid. Prefer composition when reuse is the primary goal.

## Abstraction

Abstraction hides implementation details behind behavioral contracts.

```java
abstract class PaymentProcessor {
    public abstract void process(double amount);
}
```

Abstraction matters because clients should depend on guarantees, not incidental implementation structure.

## Method Overloading / Overriding

Overloading:

```java
class Printer {
    void print(String s) {}
    void print(int n) {}
}
```

Overriding:

```java
class Base {
    void run() { System.out.println("Base"); }
}

class Derived extends Base {
    @Override
    void run() { System.out.println("Derived"); }
}
```

Overloading is compile-time resolution. Overriding participates in runtime dispatch.

## Static vs Dynamic Binding

- Static binding: chosen at compile time
- Dynamic binding: chosen at runtime based on actual object type

```java
Base ref = new Derived();
ref.run(); // dynamic dispatch -> Derived.run()
```

But static methods are not dynamically dispatched:

```java
class A { static void x() { System.out.println("A"); } }
class B extends A { static void x() { System.out.println("B"); } }

A ref = new B();
ref.x(); // A.x
```

## Encapsulation

Encapsulation restricts direct access to state and funnels mutation through controlled methods.

```java
public class Temperature {
    private double celsius;

    public void setCelsius(double celsius) {
        if (celsius < -273.15) {
            throw new IllegalArgumentException("Below absolute zero");
        }
        this.celsius = celsius;
    }
}
```

## Interfaces

Interfaces define contracts without committing clients to one implementation.

```java
interface Sorter {
    void sort(int[] values);
}
```

Interfaces support:

- Polymorphism
- Testability
- Decoupling

## Record

Records are concise data carriers with generated accessors, constructor, `equals`, `hashCode`, and `toString`.

```java
public record User(long id, String name, String email) {}
```

Use records for transparent immutable aggregates, not for entities requiring mutable lifecycle behavior.

## Packages

Packages provide namespacing and access control boundaries.

```java
package com.example.billing;
```

A coherent package layout reflects responsibility, not just folder depth.

## Initializer Block

Initializer blocks run during object or class initialization.

```java
public class InitExample {
    private int x;

    {
        x = 42; // instance initializer
    }

    static {
        System.out.println("Class initialized");
    }
}
```

Prefer constructors and field initializers unless a block materially improves clarity.

## Pass by Value / Pass by Reference

Java is strictly pass-by-value.

- For primitives, the value itself is copied.
- For objects, the reference value is copied.

```java
void change(int x) {
    x = 99;
}

void rename(StringBuilder sb) {
    sb.append("X");
}
```

`rename` can mutate the object through the copied reference, but cannot rebind the caller's variable.

## Annotations

Annotations attach metadata to program elements.

```java
@Deprecated
public void oldMethod() {}
```

Custom annotation:

```java
@interface Audited {
    String value();
}
```

Annotations are used by:

- Compilers
- Frameworks
- Reflection-based tooling
- Static analyzers

## Lambda Expressions

Lambdas express behavior as values.

```java
Runnable task = () -> System.out.println("Running");
```

They target functional interfaces.

```java
Comparator<String> byLength = (a, b) -> Integer.compare(a.length(), b.length());
```

Lambdas improve locality of behavior and reduce ceremony.

## Modules

The Java Platform Module System introduces explicit module boundaries.

`module-info.java`

```java
module com.example.app {
    requires java.sql;
    exports com.example.api;
}
```

Benefits:

- Strong encapsulation
- Explicit dependencies
- Better runtime images and deployment discipline
