# 05 Web Frameworks, Build Tools, and Dependency Injection

## Web Frameworks

Java web frameworks provide HTTP routing, request handling, dependency management, configuration, and integration with persistence, security, and serialization.

General request flow:

```text
HTTP request -> router -> controller/handler -> service -> repository -> response serializer
```

Evaluation criteria:

- Boot time
- Runtime footprint
- Ecosystem depth
- Testing model
- Deployment target

## Spring (Spring Boot)

Spring Boot is the roadmap's recommended framework because it standardizes a large portion of enterprise Java development.

```java
@RestController
@RequestMapping("/api/books")
public class BookController {
    @GetMapping("/{id}")
    public Map<String, Object> getBook(@PathVariable long id) {
        return Map.of("id", id, "title", "Distributed Systems");
    }
}
```

Key concepts:

- Auto-configuration
- Dependency injection
- Starter dependencies
- Embedded servers
- Actuator

## Spring Boot is recommended

This roadmap note is prescriptive rather than purely descriptive. The recommendation is practical:

- It has the broadest enterprise adoption in the Java ecosystem.
- It integrates cleanly with persistence, security, messaging, validation, and testing stacks.
- Its conventions reduce setup cost and let learners focus on backend design rather than framework assembly.

For most learners, Spring Boot is the highest-leverage first framework because it exposes the common patterns that recur in Java backend work.

## Play Framework

Play emphasizes reactive web application development and historically strong Scala/Java integration.

Conceptual handler flow:

```text
route -> action -> async processing -> result
```

It is suitable when non-blocking patterns and actor/reactive models are first-class concerns.

## Quarkus

Quarkus targets cloud-native and container-first workloads with fast startup and low memory usage.

It is especially relevant for:

- Kubernetes workloads
- GraalVM native compilation paths
- Fast dev mode feedback loops

## Javalin

Javalin is lightweight and pragmatic. It is useful for smaller services, prototypes, and explicit minimal APIs.

```java
Javalin app = Javalin.create().start(7000);
app.get("/ping", ctx -> ctx.result("pong"));
```

## Build Tools

Build tools automate compilation, testing, dependency resolution, packaging, and lifecycle orchestration.

Abstract pipeline:

```text
resolve dependencies -> compile -> test -> package -> publish
```

## Maven

Maven is convention-oriented and XML-driven.

`pom.xml`

```xml
<project>
  <modelVersion>4.0.0</modelVersion>
  <groupId>com.example</groupId>
  <artifactId>demo</artifactId>
  <version>1.0.0</version>
</project>
```

Strengths:

- Predictable lifecycle
- Large enterprise adoption
- Strong plugin ecosystem

## Gradle

Gradle is more programmable and incremental.

`build.gradle`

```groovy
plugins {
    id 'java'
}

repositories {
    mavenCentral()
}
```

Strengths:

- Flexible DSL
- Incremental builds
- Strong multi-module support

## Bazel

Bazel emphasizes reproducibility, hermeticity, and large-scale monorepo builds.

It is often chosen where:

- Build graph size is large
- Caching and remote execution matter
- Cross-language monorepo consistency is required

## Dependency Injection

Dependency Injection decouples construction from use.

Without DI:

```java
class OrderService {
    private final PaymentClient client = new PaymentClient();
}
```

With DI:

```java
class OrderService {
    private final PaymentClient client;

    OrderService(PaymentClient client) {
        this.client = client;
    }
}
```

Benefits:

- Testability
- Configurability
- Reduced coupling

In frameworks such as Spring:

```java
@Service
public class OrderService {
    private final PaymentClient paymentClient;

    public OrderService(PaymentClient paymentClient) {
        this.paymentClient = paymentClient;
    }
}
```
