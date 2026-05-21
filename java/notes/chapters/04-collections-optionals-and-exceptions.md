# 04 Collections, Optionals, and Exceptions

## Array vs ArrayList

Array:

- Fixed size
- Can store primitives directly
- Lower overhead

ArrayList:

- Dynamic resizing
- Stores object references
- Richer API

```java
int[] fixed = {1, 2, 3};
List<Integer> dynamic = new ArrayList<>();
dynamic.add(1);
dynamic.add(2);
```

Use arrays for dense low-level storage; use `ArrayList` for most general-purpose variable-size sequences.

## Set

A `Set` models uniqueness.

```java
Set<String> tags = new HashSet<>();
tags.add("java");
tags.add("java");
System.out.println(tags.size()); // 1
```

Common variants:

- `HashSet`: average O(1) membership
- `LinkedHashSet`: preserves insertion order
- `TreeSet`: sorted order, O(log n) operations

## Map

A `Map<K, V>` models key-value association.

```java
Map<String, Integer> inventory = new HashMap<>();
inventory.put("pen", 10);
inventory.put("book", 5);
```

Pseudo-code for frequency counting:

```text
for each token in input
    map[token] = map.getOrDefault(token, 0) + 1
```

## Queue

Queue semantics are typically FIFO.

```java
Queue<String> queue = new ArrayDeque<>();
queue.offer("A");
queue.offer("B");
System.out.println(queue.poll()); // A
```

Common uses:

- Scheduling
- Producer-consumer pipelines
- Breadth-first traversal

## Stack

A stack is LIFO.

```java
Deque<Integer> stack = new ArrayDeque<>();
stack.push(10);
stack.push(20);
System.out.println(stack.pop()); // 20
```

Do not default to legacy `Stack`; `Deque` is typically preferable.

## Dequeue

The roadmap label likely means `Deque`. A `Deque` supports insertion and removal at both ends.

```java
Deque<String> deque = new ArrayDeque<>();
deque.addFirst("middle");
deque.addLast("end");
deque.addFirst("start");
```

Applications:

- Sliding windows
- Undo/redo
- Work stealing

## Iterator

Iterators traverse collections incrementally.

```java
Iterator<String> it = List.of("a", "b", "c").iterator();
while (it.hasNext()) {
    System.out.println(it.next());
}
```

They abstract traversal without exposing collection internals.

## Collections

The Java Collections Framework provides common interfaces and algorithms.

Core interfaces:

- `Collection`
- `List`
- `Set`
- `Queue`
- `Deque`
- `Map`

Utility algorithms:

```java
List<Integer> data = new ArrayList<>(List.of(3, 1, 2));
Collections.sort(data);
Collections.reverse(data);
```

## Generic Collections

Generics provide compile-time type safety.

```java
List<String> names = new ArrayList<>();
names.add("Ada");
```

Without generics, clients would need casts and would lose type guarantees.

Bounded wildcards:

```java
double sum(List<? extends Number> values) {
    double total = 0;
    for (Number value : values) {
        total += value.doubleValue();
    }
    return total;
}
```

PECS rule:

- Producer extends
- Consumer super

## Optionals

`Optional<T>` models optional presence explicitly.

```java
Optional<String> maybeName = Optional.ofNullable(findName());
String result = maybeName.orElse("unknown");
```

Use `Optional` primarily for return types where absence is part of the API contract.

Avoid:

- `Optional` fields in ordinary domain models without strong reason
- `Optional` parameters

## Exception Handling

Exceptions represent abnormal control flow.

```java
try {
    int value = Integer.parseInt("12x");
} catch (NumberFormatException ex) {
    System.out.println("Invalid number");
} finally {
    System.out.println("Cleanup");
}
```

Categories:

- Checked exceptions
- Unchecked exceptions
- Errors

Design guidance:

- Throw specific exceptions
- Preserve causal chains
- Use exceptions for exceptional situations, not ordinary branching

Try-with-resources:

```java
try (BufferedReader reader = Files.newBufferedReader(Path.of("data.txt"))) {
    System.out.println(reader.readLine());
}
```
