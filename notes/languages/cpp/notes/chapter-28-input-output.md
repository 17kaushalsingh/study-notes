# Chapter 28: Input and Output (I/O)

## 28.1 Input and Output Streams

### Stream Class Hierarchy

```
ios_base
  └── ios
       ├── istream (input)
       │    ├── ifstream (file input)
       │    └── istringstream (string input)
       ├── ostream (output)
       │    ├── ofstream (file output)
       │    └── ostringstream (string output)
       └── iostream (both)
            ├── fstream (file both)
            └── stringstream (string both)
```

### Standard Stream Objects

| Object | Type | Purpose | Buffered? |
|--------|------|---------|-----------|
| `std::cin` | `istream` | Standard input (keyboard) | Yes |
| `std::cout` | `ostream` | Standard output (screen) | Yes |
| `std::cerr` | `ostream` | Error output | No |
| `std::clog` | `ostream` | Log output | Yes |

---

## 28.2 Input with istream

### Extraction Operator >>

```cpp
int x{};
std::cin >> x;          // reads one value, skips whitespace
std::cin >> x >> y;     // chained extraction
```

### getline()

```cpp
std::string line{};
std::getline(std::cin >> std::ws, line);  // reads entire line
```

### Other Input Functions

```cpp
char ch{};
std::cin.get(ch);       // reads one character (including whitespace)
std::cin.get(buf, 32);  // reads up to 31 chars into buffer
std::cin.ignore(100, '\n');  // discard up to 100 chars until newline
std::cin.peek();        // look at next char without extracting
std::cin.putback(ch);   // put a char back into the stream
```

---

## 28.3 Output with ostream and ios

### Formatting

```cpp
#include <iomanip>

// Precision
std::cout << std::setprecision(4) << 3.14159;  // 3.142

// Width and fill
std::cout << std::setw(10) << std::setfill('*') << 42;  // ********42

// Alignment
std::cout << std::left << std::setw(10) << "hello";     // hello

// Base
std::cout << std::hex << 255;    // ff
std::cout << std::oct << 255;    // 377
std::cout << std::dec << 255;    // 255
std::cout << std::showbase;      // prefix: 0xff, 0377

// Boolean
std::cout << std::boolalpha << true;  // "true"
```

---

## 28.4 Stream Classes for Strings

```cpp
#include <sstream>

// Output string stream
std::ostringstream oss{};
oss << "Hello " << 42 << ' ' << 3.14;
std::string result{ oss.str() };  // "Hello 42 3.14"

// Input string stream — parse values from a string
std::istringstream iss{ "42 3.14 hello" };
int i{};
double d{};
std::string s{};
iss >> i >> d >> s;  // i=42, d=3.14, s="hello"

// Bidirectional string stream
std::stringstream ss{};
ss << "12345";
int n{};
ss >> n;  // n = 12345
```

### Number ↔ String Conversion

```cpp
// Number to string
std::ostringstream oss{};
oss << 42;
std::string s{ oss.str() };  // "42"

// Or simply:
std::string s{ std::to_string(42) };  // C++11

// String to number
std::istringstream iss{ "42" };
int n{};
iss >> n;  // n = 42

// Or simply:
int n{ std::stoi("42") };  // C++11
```

### Clearing a Stringstream

```cpp
ss.str("");     // clear the buffer
ss.clear();     // reset error flags
```

---

## 28.5 Stream States and Input Validation

### Stream State Flags

| Flag | Meaning |
|------|---------|
| `goodbit` | Everything OK |
| `badbit` | Fatal error |
| `eofbit` | End of file reached |
| `failbit` | Non-fatal error (e.g., type mismatch) |

```cpp
if (std::cin.fail())     // check for failure
if (std::cin.good())     // check all is well
if (std::cin)            // shorthand for !fail()
if (!std::cin)           // shorthand for fail()
```

### Input Validation Pattern

```cpp
int getInteger() {
    while (true) {
        int x{};
        std::cout << "Enter an integer: ";
        std::cin >> x;

        if (std::cin.fail()) {
            std::cin.clear();  // reset error flags
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Invalid input. Try again.\n";
        } else {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            return x;
        }
    }
}
```

---

## 28.6 Basic File I/O

### Writing to a File

```cpp
#include <fstream>

std::ofstream outf{ "sample.txt" };

if (!outf) {
    std::cerr << "Could not open file!\n";
    return 1;
}

outf << "Line 1\n";
outf << "Line 2\n";
// File closes automatically when outf goes out of scope
```

### Reading from a File

```cpp
std::ifstream inf{ "sample.txt" };

if (!inf) {
    std::cerr << "Could not open file!\n";
    return 1;
}

// Read line by line
std::string line{};
while (std::getline(inf, line))
    std::cout << line << '\n';
```

### File Modes

```cpp
// Append to existing file (don't erase)
std::ofstream outf{ "log.txt", std::ios::app };

// Binary mode
std::ofstream outf{ "data.bin", std::ios::binary };

// Read and write
std::fstream f{ "data.txt", std::ios::in | std::ios::out };
```

| Mode | Description |
|------|-------------|
| `std::ios::app` | Append to end |
| `std::ios::in` | Open for reading |
| `std::ios::out` | Open for writing |
| `std::ios::trunc` | Erase existing contents |
| `std::ios::binary` | Binary mode |

### Explicit Open/Close

```cpp
std::ifstream inf;
inf.open("file.txt");
// ... use file ...
inf.close();
```

---

## 28.7 Random File I/O

```cpp
std::fstream f{ "data.txt", std::ios::in | std::ios::out };

// Get current position
auto pos{ f.tellg() };  // input position
auto pos2{ f.tellp() }; // output position

// Seek to position
f.seekg(14, std::ios::beg);    // go to byte 14 from beginning
f.seekg(-5, std::ios::end);    // go to 5 bytes before end
f.seekg(3, std::ios::cur);     // move 3 bytes forward from current

// Seek modes
// std::ios::beg — from beginning
// std::ios::cur — from current position
// std::ios::end — from end
```

---

## Chapter 28 — Quick Reference Card

| Concept | Code |
|---------|------|
| Read a line | `std::getline(std::cin >> std::ws, str)` |
| Set precision | `std::cout << std::setprecision(n)` |
| Number → string | `std::to_string(42)` |
| String → number | `std::stoi("42")`, `std::stod("3.14")` |
| Read file | `std::ifstream inf{"file.txt"}` |
| Write file | `std::ofstream outf{"file.txt"}` |
| Append to file | `std::ofstream outf{"file.txt", std::ios::app}` |
| Check stream | `if (!stream)` to detect errors |
| Clear stream | `stream.clear()` then `stream.ignore(...)` |
| Flush output | `std::flush` or `std::endl` |
