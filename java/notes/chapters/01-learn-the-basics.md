# 01 Learn the Basics

## Basic Syntax

Java source code is organized into classes, interfaces, enums, records, and package declarations. Compilation produces bytecode, which the JVM executes. A Java source file may contain multiple top-level types, but at most one public top-level type, and its name must match the file name.

```java
package notes.basics;

public class HelloWorld {
    public static void main(String[] args) {
        System.out.println("Hello, Java");
    }
}
```

Execution model:

```text
source (.java) -> javac -> bytecode (.class) -> JVM class loader -> verifier -> execution engine
```

Important syntax rules:

- Statements typically end with `;`.
- Blocks are delimited by `{}`.
- Java is case-sensitive.
- Identifiers cannot start with digits and cannot use reserved keywords.

## Data Types

Java has primitive and reference types.

Primitive types:

- `byte` 8-bit signed integer
- `short` 16-bit signed integer
- `int` 32-bit signed integer
- `long` 64-bit signed integer
- `float` 32-bit IEEE 754
- `double` 64-bit IEEE 754
- `char` 16-bit UTF-16 code unit
- `boolean` logical truth value

Reference types include arrays, classes, interfaces, enums, and records.

```java
int count = 42;
double temperature = 36.6;
boolean active = true;
char grade = 'A';
String name = "Ada"; // reference type
```

Semantically important distinction:

- Primitive variables store values directly.
- Reference variables store object references, not object bodies.

## Variables and Scopes

Variable categories:

- Local variables
- Method parameters
- Instance fields
- Static fields

Scope determines visibility; lifetime determines how long storage remains valid.

```java
public class ScopeDemo {
    private static int classCounter = 0;
    private int instanceCounter = 0;

    public void increment(int delta) {
        int localTotal = instanceCounter + delta;
        instanceCounter = localTotal;
        classCounter++;
    }
}
```

Rules:

- Local variables must be initialized before use.
- Fields receive default values.
- Narrow scope reduces accidental coupling.

## Conditionals

Conditionals control branching based on boolean expressions.

```java
int score = 83;

if (score >= 90) {
    System.out.println("A");
} else if (score >= 75) {
    System.out.println("B");
} else {
    System.out.println("C");
}
```

`switch` is useful for discrete alternatives.

```java
String role = "admin";

switch (role) {
    case "admin" -> System.out.println("Full access");
    case "editor" -> System.out.println("Write access");
    default -> System.out.println("Read access");
}
```

Use `switch` expressions when a branch computes a value:

```java
int day = 2;
String type = switch (day) {
    case 1, 7 -> "weekend";
    case 2, 3, 4, 5, 6 -> "weekday";
    default -> throw new IllegalArgumentException("Invalid day");
};
```

## Arrays

An array is a fixed-size indexed sequence of elements of one type.

```java
int[] values = {10, 20, 30};
System.out.println(values[1]); // 20
```

Key properties:

- Length is fixed after allocation.
- Elements are stored contiguously in logical index order.
- Primitive arrays avoid per-element boxing.

Pseudo-code for linear search:

```text
for i from 0 to n - 1
    if array[i] == target
        return i
return -1
```

## Loops

Loop forms:

- `for`
- enhanced `for`
- `while`
- `do-while`

```java
for (int i = 0; i < 5; i++) {
    System.out.println(i);
}

for (String item : new String[]{"A", "B", "C"}) {
    System.out.println(item);
}
```

Loop correctness depends on:

- Initialization
- Termination condition
- State update
- Invariant preservation

## Type Casting

Casts convert a value from one type to another.

Widening conversion is implicit:

```java
int x = 10;
double y = x;
```

Narrowing conversion is explicit and may lose information:

```java
double pi = 3.14159;
int truncated = (int) pi; // 3
```

Reference casts must be type-safe at runtime:

```java
Object obj = "text";
String s = (String) obj;
```

Invalid reference casts raise `ClassCastException`.

## Strings and Methods

`String` is immutable. Any apparent modification creates a new object.

```java
String name = "java";
String upper = name.toUpperCase();
System.out.println(name);  // java
System.out.println(upper); // JAVA
```

Methods define behavior.

```java
public static int square(int n) {
    return n * n;
}
```

Method signature includes:

- Method name
- Parameter types

Return type is not part of overload resolution by itself.

## Lifecycle of a Program

At a high level:

```text
write source -> compile -> load classes -> initialize statics -> invoke main -> allocate objects -> run -> terminate
```

More detailed execution phases:

1. Compilation by `javac`
2. Class loading by the JVM
3. Bytecode verification
4. Linking and initialization
5. `main` method invocation
6. Runtime execution with heap allocation, stack frames, and garbage collection

## Math Operations

Java supports arithmetic, increment, decrement, comparison, and modulus operations.

```java
int a = 7;
int b = 3;

System.out.println(a + b); // 10
System.out.println(a - b); // 4
System.out.println(a * b); // 21
System.out.println(a / b); // 2
System.out.println(a % b); // 1
```

Use `Math` for standard numeric utilities:

```java
double result = Math.sqrt(81.0);
int max = Math.max(7, 12);
```

Numerical caution:

- Integer division truncates toward zero.
- Floating-point arithmetic is approximate.
- For money, prefer `BigDecimal`.

```java
BigDecimal price = new BigDecimal("19.99");
BigDecimal tax = new BigDecimal("0.18");
BigDecimal total = price.add(price.multiply(tax));
```
