# 09 Database Access and Persistence

## Database Access

Database access in Java typically spans:

- Low-level JDBC
- ORM frameworks
- Repository abstractions

Core concerns:

- Connection management
- Transaction boundaries
- Mapping between relational and object models
- Query performance

## JDBC

JDBC is the foundational database API.

```java
String sql = "SELECT id, name FROM users WHERE id = ?";

try (Connection conn = dataSource.getConnection();
     PreparedStatement stmt = conn.prepareStatement(sql)) {

    stmt.setLong(1, 10L);

    try (ResultSet rs = stmt.executeQuery()) {
        while (rs.next()) {
            long id = rs.getLong("id");
            String name = rs.getString("name");
            System.out.println(id + " " + name);
        }
    }
}
```

Best practices:

- Use `PreparedStatement`
- Close resources deterministically
- Treat transactions explicitly

## Hibernate

Hibernate is a widely used ORM that maps Java objects to relational tables.

```java
@Entity
public class User {
    @Id
    private Long id;
    private String name;
}
```

ORM value proposition:

- Reduced boilerplate
- Entity lifecycle management
- Object graph navigation

Risks:

- N+1 query problems
- Hidden SQL costs
- Impedance mismatch between object graphs and relational schemas

## Spring Data JPA

Spring Data JPA builds repository abstractions over JPA providers such as Hibernate.

```java
public interface UserRepository extends JpaRepository<User, Long> {
    List<User> findByNameContaining(String token);
}
```

Strengths:

- Fast CRUD development
- Convention-based querying
- Integration with Spring transactions and DI

It improves productivity, but engineers still need SQL literacy and query inspection discipline.

## EBean

EBean is another ORM/data access framework emphasizing developer productivity and active-record-like ergonomics in some use cases.

Conceptual tradeoff:

- Less boilerplate for simple CRUD
- Framework coupling may be stronger
- Team familiarity may be lower than Hibernate/JPA

## Transaction Reasoning

Pseudo-code for a transfer:

```text
begin transaction
read source balance
read destination balance
if source balance < amount
    rollback
    fail
debit source
credit destination
commit
```

This demonstrates why persistence is not just storage. It is correctness under failure, concurrency, and partial execution.
