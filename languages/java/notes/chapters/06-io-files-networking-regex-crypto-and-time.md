# 06 I/O, Files, Networking, Regex, Crypto, and Time

## I/O Operations

Java I/O is built around streams, readers/writers, channels, and buffers.

Conceptual split:

- Byte-oriented I/O
- Character-oriented I/O

```java
try (InputStream in = new FileInputStream("input.bin")) {
    int b;
    while ((b = in.read()) != -1) {
        System.out.println(b);
    }
}
```

Use buffered APIs for efficiency.

## File Operations

The `java.nio.file` package is the modern foundation for file work.

```java
Path path = Path.of("notes.txt");
Files.writeString(path, "hello");
String text = Files.readString(path);
```

Useful operations:

- Existence checks
- Directory traversal
- Copy/move/delete
- Metadata inspection

```java
Files.createDirectories(Path.of("data/archive"));
Files.copy(Path.of("a.txt"), Path.of("b.txt"), StandardCopyOption.REPLACE_EXISTING);
```

## Networking

Java networking spans sockets, HTTP clients, and higher-level frameworks.

TCP socket example:

```java
try (Socket socket = new Socket("example.com", 80);
     PrintWriter out = new PrintWriter(socket.getOutputStream(), true);
     BufferedReader in = new BufferedReader(new InputStreamReader(socket.getInputStream()))) {

    out.println("GET / HTTP/1.1");
    out.println("Host: example.com");
    out.println();
}
```

Modern HTTP client:

```java
HttpClient client = HttpClient.newHttpClient();
HttpRequest request = HttpRequest.newBuilder()
        .uri(URI.create("https://example.com"))
        .build();
HttpResponse<String> response = client.send(request, HttpResponse.BodyHandlers.ofString());
```

## Regular Expressions

Regular expressions describe string patterns declaratively.

```java
Pattern pattern = Pattern.compile("^[A-Za-z0-9_]+$");
Matcher matcher = pattern.matcher("user_123");
boolean ok = matcher.matches();
```

Practical cautions:

- Complex regexes become unreadable quickly
- Catastrophic backtracking is possible
- Domain validation often needs more than regex alone

## Cryptography

Java provides cryptographic primitives through JCA/JCE.

Hashing example:

```java
MessageDigest digest = MessageDigest.getInstance("SHA-256");
byte[] hash = digest.digest("secret".getBytes(StandardCharsets.UTF_8));
```

Important distinctions:

- Hashing is not encryption
- Encryption is reversible with a key
- Password storage should use password hashing algorithms, not plain SHA-256

Pseudo-code for password handling:

```text
generate random salt
derive key with strong password hashing function
store algorithm + parameters + salt + derived hash
on login, recompute and compare in constant time
```

## Date and Time

Use `java.time`, not legacy `Date`/`Calendar`, for new code.

```java
LocalDate date = LocalDate.now();
LocalDate nextWeek = date.plusDays(7);
Instant timestamp = Instant.now();
ZonedDateTime kolkata = ZonedDateTime.now(ZoneId.of("Asia/Kolkata"));
```

Semantically distinct types:

- `LocalDate`: date without time zone
- `LocalDateTime`: local date-time without zone
- `Instant`: absolute point in time
- `ZonedDateTime`: date-time with zone rules

Always model time with the narrowest correct abstraction.
