# 07 Concurrency and Java Memory Model

## Concurrency

Concurrency is about multiple tasks making progress during overlapping periods. It is distinct from parallelism, though parallel execution may occur on multicore hardware.

Primary concerns:

- Safety
- Liveness
- Visibility
- Ordering

Pseudo-code race condition:

```text
shared counter = 0
thread A reads counter
thread B reads counter
thread A writes counter + 1
thread B writes counter + 1
result may be 1 instead of 2
```

## Threads

A thread is a unit of execution with its own call stack.

```java
Thread worker = new Thread(() -> {
    System.out.println("Running in " + Thread.currentThread().getName());
});
worker.start();
worker.join();
```

Use executors for task management in most production systems.

```java
ExecutorService pool = Executors.newFixedThreadPool(4);
pool.submit(() -> compute());
pool.shutdown();
```

## Virtual Threads

Virtual threads are lightweight JVM-managed threads designed for high-concurrency workloads, especially blocking I/O.

```java
try (var executor = Executors.newVirtualThreadPerTaskExecutor()) {
    Future<String> future = executor.submit(() -> "done");
    System.out.println(future.get());
}
```

Conceptually:

- Platform threads map more directly to OS threads
- Virtual threads decouple many logical tasks from scarce OS thread resources

## Java Memory Model

The Java Memory Model defines legal behavior for shared-memory programs, including visibility and ordering guarantees.

Key concepts:

- Happens-before
- Atomicity
- Visibility
- Reordering constraints

Synchronization establishes ordering:

```java
class Counter {
    private int value;

    public synchronized void increment() {
        value++;
    }

    public synchronized int getValue() {
        return value;
    }
}
```

## volatile keyword

`volatile` guarantees visibility and ordering for reads and writes to a variable, but not compound atomicity.

```java
class FlagHolder {
    private volatile boolean running = true;

    public void stop() {
        running = false;
    }

    public void runLoop() {
        while (running) {
            // work
        }
    }
}
```

`volatile` is appropriate for state flags and publication scenarios, but not for `count++`.

## Practical Concurrency Design

Prefer higher-level concurrency constructs over manual thread coordination when possible:

- `ExecutorService`
- `CompletableFuture`
- `BlockingQueue`
- `Semaphore`
- `CountDownLatch`
- Structured concurrency where available in the platform stack

Reasoning order:

1. Minimize shared mutable state
2. Prefer immutable objects
3. Use message passing or task isolation
4. Synchronize only where invariants truly require it
