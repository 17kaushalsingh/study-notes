# 08 Functional Programming and Streams

## Functional Programming

Functional programming in Java is not purity in the Haskell sense. It is primarily a style centered on first-class behavior, immutability where practical, declarative transformation, and minimized side effects.

```java
List<Integer> numbers = List.of(1, 2, 3, 4);
int sum = numbers.stream().mapToInt(Integer::intValue).sum();
```

## Functional Composition

Composition combines simpler functions into richer behavior.

```java
Function<Integer, Integer> doubleIt = x -> x * 2;
Function<Integer, Integer> squareIt = x -> x * x;
Function<Integer, Integer> squareThenDouble = squareIt.andThen(doubleIt);
```

Mathematically:

```text
(f compose g)(x) = f(g(x))
```

Composition improves modular reasoning and reuse.

## High Order Functions

Higher-order functions take functions as arguments or return them.

```java
static List<Integer> transform(List<Integer> input, Function<Integer, Integer> mapper) {
    List<Integer> out = new ArrayList<>();
    for (int value : input) {
        out.add(mapper.apply(value));
    }
    return out;
}
```

Usage:

```java
List<Integer> doubled = transform(List.of(1, 2, 3), x -> x * 2);
```

## Functional Interfaces

A functional interface has exactly one abstract method.

Examples:

- `Runnable`
- `Supplier<T>`
- `Consumer<T>`
- `Function<T, R>`
- `Predicate<T>`

```java
Predicate<String> nonEmpty = s -> !s.isBlank();
```

Default and static methods are allowed without breaking functional-interface status.

## Stream API

Streams model data-processing pipelines.

```java
List<String> names = List.of("Ada", "Alan", "Grace", "Linus");
List<String> filtered = names.stream()
        .filter(name -> name.length() <= 4)
        .sorted()
        .toList();
```

Pipeline phases:

- Source
- Intermediate operations
- Terminal operation

Common intermediate operations:

- `map`
- `filter`
- `flatMap`
- `distinct`
- `sorted`

Terminal operations:

- `collect`
- `reduce`
- `forEach`
- `count`
- `findFirst`

Reduction example:

```java
int product = List.of(1, 2, 3, 4).stream()
        .reduce(1, (a, b) -> a * b);
```

Stream discipline:

- Avoid side effects in intermediate operations
- Prefer clarity over clever one-liners
- Use parallel streams only with evidence and understanding of cost
