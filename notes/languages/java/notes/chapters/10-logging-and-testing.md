# 10 Logging and Testing

## Logging Frameworks

Logging is part of observability, diagnosis, security auditing, and operational control.

A useful log event should answer:

- What happened?
- Where did it happen?
- When did it happen?
- Under which contextual identifiers?
- How severe is it?

## SLF4J

SLF4J is a facade, not a concrete logger.

```java
private static final Logger log = LoggerFactory.getLogger(OrderService.class);

log.info("Order {} accepted for user {}", orderId, userId);
```

Facade-based design decouples application code from backend implementation.

## Logback

Logback is a common SLF4J backend with structured configuration and appenders.

Typical use:

- Console logging in development
- Rolling file appenders in services
- JSON logging in containerized systems

## Log4j2

Log4j2 is another mature backend emphasizing performance and flexible configuration.

Understand the backend because:

- Async logging behavior matters
- Deployment configuration matters
- Security posture matters

## TinyLog

TinyLog is a lighter alternative for smaller applications or simpler setups.

Choose it when operational complexity is low and a minimal dependency surface is valuable.

## Testing

Testing is evidence collection about program behavior under specified conditions.

Testing layers:

- Unit tests
- Integration tests
- Behavioral/acceptance tests
- Performance tests

## Unit Testing

Unit tests isolate small units of behavior.

```java
class Calculator {
    int add(int a, int b) {
        return a + b;
    }
}
```

Test:

```java
@Test
void addsTwoNumbers() {
    Calculator calculator = new Calculator();
    assertEquals(5, calculator.add(2, 3));
}
```

## JUnit

JUnit is the dominant unit testing framework in modern Java projects.

Typical concepts:

- Test methods
- Assertions
- Lifecycle callbacks
- Parameterized tests

```java
@ParameterizedTest
@CsvSource({"2,3,5", "10,5,15"})
void addWorks(int a, int b, int expected) {
    assertEquals(expected, new Calculator().add(a, b));
}
```

## TestNG

TestNG provides flexible grouping, configuration, and execution models, and remains relevant in some enterprise stacks.

Use cases often include:

- Legacy suites
- Config-heavy test orchestration
- Existing organizational tooling alignment

## Integration Testing

Integration tests validate collaboration across components.

Examples:

- Controller to service to repository
- Persistence against a real database
- External API contract checks

Pseudocode:

```text
start application context
seed test database
invoke API
assert database state and HTTP response
```

## REST Assured

REST Assured is used to test HTTP APIs declaratively.

```java
given()
    .baseUri("http://localhost:8080")
.when()
    .get("/api/books/1")
.then()
    .statusCode(200);
```

## JMeter

JMeter is used for load and performance testing.

Performance testing questions:

- What is the latency distribution?
- Where is saturation?
- How does the system degrade?
- Are bottlenecks CPU, I/O, lock, or database related?

## Behavior Testing

Behavior testing describes expected behavior in business-facing language.

## Cucumber-JVM

Cucumber-JVM connects executable specifications to Java step definitions.

Gherkin:

```text
Feature: Account withdrawal
  Scenario: Withdraw within balance
    Given an account balance of 100
    When the user withdraws 30
    Then the remaining balance should be 70
```

## Mocking

Mocking isolates collaborators when direct integration is unnecessary or undesirable for the test objective.

Use mocks for:

- Slow or flaky dependencies
- Rare edge-case control
- Interaction verification

Do not use mocks to compensate for poor architecture.

## Mockito

Mockito is the most common mocking library in Java.

```java
PaymentClient client = mock(PaymentClient.class);
when(client.charge(100)).thenReturn(true);

OrderService service = new OrderService(client);
assertTrue(service.pay(100));
verify(client).charge(100);
```

Good tests are:

- Deterministic
- Focused
- Readable
- Sensitive to real regressions
