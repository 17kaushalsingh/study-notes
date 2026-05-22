# Chapter 1: C++ Basics

---

## 1.1 -- Statements and the Structure of a Program

### Statements

A **statement** is an instruction that causes the program to perform an action. Most statements in C++ end with a semicolon (`;`). A statement is the smallest independent unit of computation.

### Functions

A **function** is a collection of statements that execute sequentially. Every C++ program must include a special function named `main()` -- this is where execution begins.

### The Hello World Program

```cpp
#include <iostream>

int main()
{
    std::cout << "Hello world!";
    return 0;
}
```

**Line-by-line breakdown:**
- `#include <iostream>` -- preprocessor directive that includes the I/O library.
- `int main()` -- declares the main function, which returns an integer.
- `std::cout << "Hello world!";` -- outputs text to the console.
- `return 0;` -- returns 0 to the operating system (0 means success).

### Key Terminology

- **Characters**: individual written symbols (letters, digits, punctuation).
- **Text (strings)**: sequences of characters forming words and sentences.
- **Syntax error**: violating C++ grammar rules, which halts compilation.

> **Remember:** Compilers sometimes report errors on the line *after* the actual mistake. If you can't find an error on the reported line, check the preceding line.

> **Best Practice:** End most statements with a semicolon. Use descriptive function names (e.g., `calculateGrade()`, `printEmployee()`).

---

## 1.2 -- Comments

Comments are programmer-readable notes ignored by the compiler. They document code for human understanding.

### Two Comment Styles

#### Single-Line Comments (`//`)

```cpp
// This is a single-line comment
std::cout << "Hello";  // This prints Hello
```

Everything from `//` to the end of the line is ignored.

#### Multi-Line Comments (`/* */`)

```cpp
/* This is a multi-line comment.
   It can span several lines.
   Everything between the delimiters is ignored. */
```

> **Warning:** Multi-line comments **cannot be nested**. The first `*/` encountered ends the comment, which can cause unexpected compilation errors.

### Best Practices: The Three-Level Framework

1. **Library/Program/Function level** -- describe **what** the code accomplishes.
2. **Within implementation** -- describe **how** the code achieves its goal.
3. **Statement level** -- explain **why** the code does something (not what it does).

```cpp
// Bad comment (describes "what"):
// Set sight range to 0
sight = 0;

// Good comment (describes "why"):
// The player just drank a potion of blindness
sight = 0;
```

> **Key Takeaway:** "Comment your code liberally, and write your comments as if speaking to someone who has no idea what the code does." Prioritize clarity for future readers unfamiliar with the code.

---

## 1.3 -- Introduction to Objects and Variables

### Data, Values, and Literals

- **Data**: any information that can be moved, processed, or stored by a computer.
- **Value**: a single piece of data (a number, character, or text).
- **Literal**: a value placed directly in source code (e.g., `5`, `'H'`, `"Hello"`).

Quote rules:
- Character literals use single quotes: `'H'`
- Text (string) literals use double quotes: `"Hello"`
- Numbers are unquoted: `5`

### Objects and Variables

- **Object**: a region of storage (typically RAM) that holds a value and has associated properties.
- **Variable**: a named object.
- **Identifier**: the name used to reference a variable.

### Data Types

A **data type** tells the compiler what kind of value an object stores. It must be known at compile time and cannot change without recompilation.

```cpp
int x;    // define an integer variable named x
```

- `int` stores whole numbers (integers) without fractional parts.

### Defining Variables

A **definition statement** tells the compiler to create a variable:

```cpp
int x;       // define a single variable
int a, b;    // define two variables of the same type
```

> **Best Practice:** Define each variable on its own line with a descriptive comment, rather than combining multiple definitions in one statement.

### Common Mistakes

```cpp
int a, int b;     // ERROR: don't repeat the type with comma separation
int a, double b;  // ERROR: can't mix types in one definition statement
```

> **Key Takeaway:** Variables provide named access to memory storage. Each variable has an identifier, a type, and a value. The compiler and OS handle memory management details.

---

## 1.4 -- Variable Assignment and Initialization

### Assignment

Giving a variable a value *after* it has been defined, using the `=` (assignment) operator:

```cpp
int width;     // definition (no value yet)
width = 5;     // assignment
```

### Initialization

Providing a value at the point of creation (combines definition and assignment):

### Five Forms of Initialization

```cpp
int a;          // 1. Default-initialization: no value, indeterminate
int b = 5;      // 2. Copy-initialization: inherited from C
int c(6);       // 3. Direct-initialization: value in parentheses
int d{ 7 };     // 4. Direct-list-initialization: modern preferred form
int e{};        // 5. Value-initialization: empty braces, zeros the variable
```

### Why Prefer Direct-List-Initialization (Brace Initialization)?

It prevents **narrowing conversions** -- the compiler will error if data would be lost:

```cpp
int w1{ 4.5 };   // COMPILE ERROR: narrowing conversion not allowed
int w2 = 4.5;    // Compiles silently: truncates to 4 (data loss!)
int w3(4.5);     // Compiles silently: truncates to 4 (data loss!)
```

### The `[[maybe_unused]]` Attribute (C++17)

Suppresses warnings for intentionally unused variables:

```cpp
[[maybe_unused]] int x{ 5 };  // No warning even if x is never used
```

> **Best Practice:** Prefer `int x{ 5 };` (direct-list-initialization) for specific values and `int x{};` (value-initialization) for zero/default values. Initialize variables upon creation.

> **Warning:** Do not confuse the assignment operator (`=`) with the equality operator (`==`). Also beware: `int a, b = 5;` leaves `a` uninitialized -- only `b` gets the value 5.

> **Key Takeaway:** "Initialization provides an initial value for a variable at the point of creation. Assignment gives a value to an already-existing variable."

---

## 1.5 -- Introduction to iostream: cout, cin, and endl

### The I/O Library

Include `#include <iostream>` to access input/output functionality.

### std::cout -- Output to Console

Uses the **insertion operator** (`<<`) to send data to the console:

```cpp
#include <iostream>

int main()
{
    std::cout << "Hello world!";     // print text
    std::cout << 5;                   // print number
    int x{ 42 };
    std::cout << "x is: " << x;      // chain multiple outputs
    return 0;
}
```

### Newlines: std::endl vs '\n'

```cpp
std::cout << "Line 1" << std::endl;  // newline + flush buffer (slower)
std::cout << "Line 2" << '\n';       // newline only (faster, preferred)
```

- `std::endl` outputs a newline **and** flushes the output buffer.
- `'\n'` outputs only a newline without flushing.

> **Best Practice:** Prefer `'\n'` over `std::endl` for better performance. Use single quotes when standalone (`'\n'`), double quotes when embedded in text (`"Hello\n"`).

### Output Buffering

Data sent to `std::cout` enters a **buffer** before appearing on screen. The buffer is periodically flushed (sent to the console). Buffering improves performance by batching I/O operations.

### std::cin -- Input from Keyboard

Uses the **extraction operator** (`>>`) to read input into variables:

```cpp
#include <iostream>

int main()
{
    std::cout << "Enter a number: ";
    int x{};
    std::cin >> x;
    std::cout << "You entered " << x << '\n';
    return 0;
}
```

### Reading Multiple Values

```cpp
int x{}, y{};
std::cout << "Enter two numbers separated by a space: ";
std::cin >> x >> y;
std::cout << "Sum: " << x + y << '\n';
```

### How Extraction Works

1. Checks the input stream state.
2. Discards leading whitespace.
3. Waits for input if the buffer is empty (user presses Enter).
4. Extracts characters until encountering whitespace or invalid characters.
5. Converts extracted text to the target type, or sets the variable to 0 on failure.

> **Warning:** Using `/n` instead of `\n` is a common mistake and produces unexpected output. Also, if extraction fails, the variable is set to 0 and future extractions are blocked until the stream is cleared.

> **Key Takeaway:** `std::cout` and `<<` move data TO the console. `std::cin` and `>>` move data FROM the keyboard INTO variables.

---

## 1.6 -- Uninitialized Variables and Undefined Behavior

### Uninitialized Variables

A variable that has not been given a known value contains **garbage data** -- whatever happened to be in that memory location previously. C++ does **not** automatically initialize most variables.

```cpp
#include <iostream>

int main()
{
    int x;                      // uninitialized -- contains garbage
    std::cout << x << '\n';     // UNDEFINED BEHAVIOR: unpredictable output
    return 0;
}
```

### Key Terminology

- **Initialized**: variable receives a known value at the point of definition.
- **Assigned**: variable receives a known value after definition.
- **Uninitialized**: variable has never been given any known value.

### Undefined Behavior (UB)

**Undefined behavior** is the result of executing code whose behavior is not defined by the C++ language standard. Using the value of an uninitialized variable is undefined behavior.

Consequences of UB:
- Program may produce different results each time it runs.
- Program may consistently produce the wrong result.
- Program may appear to work correctly (but is still broken).
- Program may crash.
- Behavior may differ across compilers, machines, and build configurations.

> **Warning:** Debug builds may mask uninitialized variable problems by zeroing memory. Always test in release builds too.

### Related Concepts

- **Implementation-defined behavior**: the compiler determines specifics, which are documented and consistent for that compiler.
- **Unspecified behavior**: similar to implementation-defined but not documented.
- Both should be avoided for portable code.

> **Best Practice:** **Always initialize your variables.** The cost is minuscule, and it prevents one of the most common and hardest-to-debug classes of errors.

> **Key Takeaway:** "Take care to avoid all situations that result in undefined behavior, such as using an uninitialized variable."

---

## 1.7 -- Keywords and Naming Identifiers

### Keywords (Reserved Words)

C++ reserves **92 words** (as of C++23) for its own use. These include:

- Control flow: `if`, `else`, `while`, `for`, `switch`, `break`, `continue`, `return`
- Types: `int`, `float`, `double`, `char`, `bool`, `void`, `auto`
- Classes: `class`, `struct`, `enum`, `union`, `namespace`
- Memory: `new`, `delete`
- Logic: `true`, `false`, `and`, `or`, `not`

Keywords cannot be used as identifiers.

### Identifier Naming Rules

1. **Cannot be a keyword.**
2. **Only letters, digits, and underscores** -- no symbols or whitespace.
3. **Must begin with a letter or underscore** -- never a digit.
4. **Case-sensitive**: `nvalue`, `nValue`, and `NVALUE` are three distinct identifiers.

### Naming Conventions

```cpp
int value;              // good: starts with lowercase
int Value;              // unconventional: starts uppercase (reserve for types)

int my_variable_name;   // snake_case -- acceptable
int myVariableName;     // camelCase -- acceptable
// Pick one style and be consistent

int numApples;          // good: descriptive
int monstersKilled;     // good: specific and clear
int x;                  // acceptable for trivial/temporary use
int data;               // bad: too vague
```

### Naming Guidelines

- Begin variable and function names with a **lowercase letter**.
- User-defined types (structs, classes, enums) begin with an **uppercase letter**.
- Avoid names starting with an underscore (reserved for system/library use).
- Name length should be proportional to scope: short names for trivial loop variables, longer names for significant values.
- Match existing code style when working in established projects.

> **Key Takeaway:** "Code is read more often than it is written, so any time saved while writing the code is time that every reader, including future you, will waste while reading it." Prioritize clarity.

---

## 1.9 -- Introduction to Literals and Operators

### Literals

A **literal** (or literal constant) is a fixed value inserted directly into source code. It is compiled directly into the executable and cannot be changed.

```cpp
std::cout << 5 << '\n';           // 5 is an integer literal
std::cout << "Hello" << '\n';     // "Hello" is a string literal
```

**Distinction from variables:** Variables occupy memory with changeable values; literals are fixed values embedded in the executable.

### Operators

An **operator** is a symbol that performs an operation on zero or more input values (**operands**) to produce an output value (the **return value**).

### Operator Arity

| Arity | Operands | Example |
|---|---|---|
| Unary | 1 | `-5` (negation) |
| Binary | 2 | `3 + 4` |
| Ternary | 3 | `a ? b : c` (conditional) |
| Nullary | 0 | `throw` |

### Operator Precedence (PEMDAS/BODMAS)

```cpp
2 + 3 * 4     // evaluates to 14 (multiplication first)
(2 + 3) * 4   // evaluates to 20 (parentheses first)
```

### Return Values and Side Effects

- **Return value**: the output of an operation (`2 + 3` returns `5`).
- **Side effect**: an observable effect beyond the return value.

```cpp
x = 5;                           // side effect: assigns 5 to x; returns x
std::cout << "Hello";            // side effect: prints to console
```

### Chaining

Because `=` and `<<` return their left operand, they can be chained:

```cpp
x = y = 5;                       // y = 5 first, then x = y
std::cout << "Hello " << "world!";  // prints both strings
```

> **Key Takeaway:** Literals are fixed values embedded in code. Operators transform operands into return values and may produce side effects.

---

## 1.10 -- Introduction to Expressions

### What Is an Expression?

An **expression** is a non-empty sequence of literals, variables, operators, and function calls that calculates a value.

### Examples of Expressions

```cpp
2                    // literal expression, evaluates to 2
"Hello world!"       // string literal expression
x                    // variable expression, evaluates to x's value
2 + 3                // arithmetic expression, evaluates to 5
five()               // function call expression, evaluates to return value
```

### Using Expressions in Initialization

```cpp
int a{ 2 };             // literal
int b{ 2 + 3 };         // operator expression
int c{ (2 * 3) + 4 };   // compound expression
int d{ b };             // variable expression
int e{ five() };        // function call expression
```

> **Key Principle:** "Wherever a single value is expected in C++, you can use a value-producing expression instead."

### Expression Statements

An **expression statement** is an expression followed by a semicolon:

```cpp
x = 5;       // useful expression statement (has side effect: assignment)
2 * 3;       // valid but useless (result is discarded, compiler may warn)
```

### Terminology

- **Subexpression**: an expression used as an operand within a larger expression.
- **Full expression**: an expression that is not a subexpression.
- **Compound expression**: an expression containing two or more operators.

In `x = 4 + 5`:
- `x`, `4`, `5`, and `4 + 5` are all subexpressions.
- `x = 4 + 5` is the full expression.

> **Key Takeaway:** Expressions calculate values. Expression statements exist primarily for their side effects (assignment, output, etc.). The compiler may warn about expression statements that produce a value but discard it.
