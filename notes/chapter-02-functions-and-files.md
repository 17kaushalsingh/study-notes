# Chapter 2: Functions and Files

---

## 2.1 -- Introduction to Functions

A **function** is a reusable sequence of statements designed to do a particular job. Functions allow you to split programs into small, modular chunks that are easier to organize, test, and use.

### Function Anatomy

Every function has two parts:

1. **Function header** -- declares the function's return type, name, and parameters
2. **Function body** -- the curly braces `{}` and the statements inside them

```cpp
returnType functionName() // function header
{
    // function body -- statements go here
}
```

### How Functions Execute

When a program encounters a **function call**, execution jumps to the top of the called function. When the function ends, execution returns to the point just after the function call.

- The function that initiates the call is the **caller**
- The function being called is the **callee** (or called function)

```cpp
#include <iostream>

void doPrint() // doPrint is the callee
{
    std::cout << "In doPrint()\n";
}

int main()
{
    std::cout << "Starting main()\n";
    doPrint(); // main is the caller; execution jumps to doPrint()
    std::cout << "Ending main()\n";
    return 0;
}
```

Output:
```
Starting main()
In doPrint()
Ending main()
```

### Functions Calling Functions

Functions can call other functions, and those functions can call yet more functions. Execution always returns to the correct caller when a function finishes.

### Important Rules

- **Do not forget the parentheses `()`** when calling a function. Omitting them may cause the program to not compile or to silently skip the function call.
- **C++ does not support nested function definitions** -- you cannot define a function inside another function.

> **Key Takeaway:** Functions are the fundamental building blocks of C++ programs. Every C++ program must have a `main()` function, which is called automatically when the program starts.

---

## 2.2 -- Function Return Values (Value-Returning Functions)

A **value-returning function** is a function that returns a value back to the caller. The return type (specified before the function name) tells the compiler what type of value will be returned.

### The return Statement

The `return` keyword, followed by an expression, causes the function to return the evaluated value to the caller. This is called **return by value** -- the value is copied back to the caller.

```cpp
#include <iostream>

int getValueFromUser()
{
    std::cout << "Enter an integer: ";
    int input{};
    std::cin >> input;
    return input; // return the value the user entered
}

int main()
{
    int num{ getValueFromUser() }; // return value used to initialize num
    std::cout << num << " doubled is: " << num * 2 << '\n';
    return 0;
}
```

### Rules for Value-Returning Functions

1. A value-returning function **must** return a value of the appropriate type (except `main()`). Failing to do so causes **undefined behavior**.
2. A function can only return **one** value each time it is called.
3. The `main()` function will implicitly return `0` if no return statement is provided, but it is best practice to explicitly return a value.

### Return Values from main()

The return value of `main()` is called a **status code** (or exit code). By convention:
- `0` means the program ran successfully
- Non-zero means failure

For maximum portability, use the macros from `<cstdlib>`:

```cpp
#include <cstdlib>

int main()
{
    return EXIT_SUCCESS; // equivalent to return 0
    // return EXIT_FAILURE; // equivalent to a non-zero return
}
```

### The DRY Principle

**DRY** stands for **Don't Repeat Yourself**. If you need the same logic in multiple places, put it in a function and call that function.

```cpp
#include <iostream>

int getValueFromUser()
{
    std::cout << "Enter an integer: ";
    int input{};
    std::cin >> input;
    return input;
}

int main()
{
    int x{ getValueFromUser() }; // first call
    int y{ getValueFromUser() }; // reuse the same function
    std::cout << x << " + " << y << " = " << x + y << '\n';
    return 0;
}
```

> **Key Takeaway:** Value-returning functions let you pass data from a called function back to its caller. Always ensure your value-returning functions return a value on every code path, or you risk undefined behavior.

---

## 2.3 -- Void Functions (Non-Value Returning Functions)

A **void function** is a function that does not return a value. Use the keyword `void` as the return type.

```cpp
#include <iostream>

void printHi()
{
    std::cout << "Hi" << '\n';
}

int main()
{
    printHi(); // call the void function
    return 0;
}
```

### Key Rules

- Void functions **automatically return** to the caller at the end of the function body. No explicit `return` statement is needed.
- You can use an empty `return;` statement to **exit early** from a void function, but do **not** put an empty `return;` at the end -- it is redundant.
- You **cannot** return a value from a void function. Doing so causes a compile error.
- You **cannot** use a void function call where a value is expected.

```cpp
// ERROR: std::cout expects a value, void functions don't provide one
std::cout << printHi(); // compile error!
```

> **Best Practice:** Do not put a `return;` statement at the end of a void function. It is unnecessary and clutters the code.

> **Key Takeaway:** Use void functions when you need a function to perform an action (like printing output) but don't need it to return data to the caller.

---

## 2.4 -- Introduction to Function Parameters and Arguments

**Parameters** are variables declared in a function's header that act as placeholders for values the caller provides. **Arguments** are the actual values passed to the function when it is called.

```cpp
#include <iostream>

// x and y are parameters
int add(int x, int y)
{
    return x + y;
}

int main()
{
    // 6 and 7 are arguments
    std::cout << add(6, 7) << '\n'; // prints 13
    return 0;
}
```

### Pass by Value

When a function is called, each argument is **copied** into its corresponding parameter via **copy initialization**. This mechanism is called **pass by value**. Changes made to a parameter inside the function do **not** affect the original argument.

```cpp
#include <iostream>

void printDouble(int value)
{
    value = value * 2; // modifies local copy, NOT the original
    std::cout << value << '\n';
}

int main()
{
    int x{ 5 };
    printDouble(x); // prints 10
    std::cout << x;  // still prints 5 -- x is unchanged
    return 0;
}
```

### Multiple Parameters

Each parameter must have its type explicitly specified, separated by commas:

```cpp
int add(int x, int y) // correct -- each has its own type
```

### Arguments Can Be Expressions

Arguments are evaluated before being passed:

```cpp
add(1 + 2, 3 * 4) // equivalent to add(3, 12)
```

### Unreferenced Parameters

If a parameter is not used in the function body, leave it unnamed to avoid compiler warnings:

```cpp
void doSomething(int /*count*/) // unnamed parameter with comment
{
    // count is not used
}
```

> **Best Practice:** When a function parameter exists but is not used in the body of the function, do not give it a name. Use a comment to document what it would have been.

> **Key Takeaway:** Parameters and arguments are the primary mechanism for passing data *into* a function. Under pass by value, the function works on copies -- the originals are safe.

---

## 2.5 -- Introduction to Local Scope

### Local Variables

Variables defined inside a function body (including function parameters) are called **local variables**. They are created at their point of definition and destroyed at the end of the enclosing block (the closing brace `}`).

```cpp
#include <iostream>

int add(int x, int y) // x and y created here, destroyed when function ends
{
    return x + y;
}

int main()
{
    int a{ 5 };  // a created here
    int b{ 6 };  // b created here
    std::cout << add(a, b) << '\n';
    return 0;
}   // a and b destroyed here
```

### Lifetime vs. Scope

These are distinct concepts:

| Concept  | Property    | Meaning |
|----------|-------------|---------|
| Lifetime | Runtime     | When the object is created and destroyed in memory |
| Scope    | Compile-time| Where in the source code the identifier can be accessed |

A local variable's scope extends from its point of definition to the end of its enclosing block.

### Functional Separation

Each function has its own independent scope. Variables named `x` in `main()` are completely separate from parameters named `x` in another function:

```cpp
#include <iostream>

void doIt(int x, int y)
{
    x = 3;  // modifies doIt's x, not main's x
    std::cout << x << '\n'; // prints 3
}

int main()
{
    int x{ 1 };
    int y{ 2 };
    doIt(x, y);
    std::cout << x << '\n'; // still prints 1
    return 0;
}
```

### Temporary Objects

The compiler sometimes creates **temporary (anonymous) objects** to hold intermediate results (such as the return value of a function). These are destroyed at the end of the full expression in which they are created.

> **Best Practice:** Define local variables as close to their first use as reasonable. This makes code easier to read and reduces the chance of using an uninitialized variable.

> **Key Takeaway:** Local variables live only within their enclosing block. Different functions can use the same variable names without conflict because each function has its own scope.

---

## 2.6 -- Why Functions Are Useful, and How to Use Them Effectively

### Five Key Benefits of Functions

1. **Organization** -- Break complex programs into smaller, manageable pieces.
2. **Reusability** -- Write once, call many times. Avoids code duplication.
3. **Testing** -- Self-contained functions are easier to test and verify independently.
4. **Extensibility** -- Change behavior in one place and it applies everywhere the function is called.
5. **Abstraction** -- Callers only need to know *what* a function does (name, inputs, outputs), not *how* it works.

### When to Create a Function

- Code that appears more than once should be a function.
- Code that has a well-defined set of inputs and outputs.
- Code that performs a distinct, identifiable task.
- When a function grows too long or complex, split it into sub-functions.

### Effective Function Design

**Keep functions short:** A function that fits on one screen (roughly 20-40 lines) is easiest to understand.

**One task per function:** Each function should do exactly one thing. If you can't name the function clearly, it may be doing too much.

**Separate I/O from computation:** Do not mix calculation logic with input/output in the same function. Have calculation functions return values and let the caller decide what to do with them.

```cpp
// Good: separate functions
int calculateResult(int x, int y)
{
    return x * 2 + y;
}

void printResult(int result)
{
    std::cout << "Result: " << result << '\n';
}

int main()
{
    int result{ calculateResult(3, 4) };
    printResult(result);
    return 0;
}
```

> **Key Takeaway:** Functions improve organization, reusability, testing, extensibility, and abstraction. Design each function to do one thing well, and keep computation separate from I/O.

---

## 2.7 -- Forward Declarations and Definitions

### The Problem

If you call a function before the compiler has seen its definition, you get a compile error:

```cpp
#include <iostream>

int main()
{
    std::cout << add(3, 4) << '\n'; // ERROR: compiler doesn't know about add() yet
    return 0;
}

int add(int x, int y)
{
    return x + y;
}
```

### The Solution: Forward Declarations

A **forward declaration** (or **function prototype**) tells the compiler about a function's existence before its definition. It consists of the return type, name, and parameter types, followed by a semicolon:

```cpp
#include <iostream>

int add(int x, int y); // forward declaration (function prototype)

int main()
{
    std::cout << add(3, 4) << '\n'; // OK: compiler knows add exists
    return 0;
}

int add(int x, int y) // actual definition
{
    return x + y;
}
```

> **Best Practice:** Keep parameter names in your forward declarations for documentation purposes.

### Declarations vs. Definitions

| Term | What it does | Example |
|------|-------------|---------|
| Declaration | Tells the compiler an identifier exists and its type | `int add(int x, int y);` |
| Definition  | Implements the function or instantiates a variable | `int add(int x, int y) { return x + y; }` |
| Pure declaration | A declaration that is not a definition | The prototype above |

**All definitions are declarations, but not all declarations are definitions.**

### The One Definition Rule (ODR)

The ODR has three parts:

1. **Within a file:** Each function, variable, type, or template can have only **one definition**.
2. **Within a program:** Each function and variable can have only **one definition** across all files.
3. **Exception:** Types, templates, and inline functions/variables can have definitions in multiple files, as long as they are **identical**.

| Violation | Result |
|-----------|--------|
| Part 1    | Compiler error |
| Part 2    | Linker error |
| Part 3    | Undefined behavior |

### Forward Declaration Without Definition

If you forward-declare a function and call it, but never define it anywhere:
- **Compilation succeeds** (the compiler trusts the declaration)
- **Linking fails** with an "unresolved external symbol" error

> **Key Takeaway:** Forward declarations let you call functions before they are defined. This is essential for multi-file programs and resolving circular dependencies. Always be aware of the One Definition Rule.

---

## 2.8 -- Programs with Multiple Code Files

### Why Use Multiple Files?

As programs grow, splitting code across multiple `.cpp` files improves organization and enables code reuse.

### How Multi-File Compilation Works

The compiler **compiles each file independently**. It does not remember anything from previously compiled files. This means:

- Files can be compiled in any order
- Only modified files need recompilation
- Each file must independently `#include` any headers it needs

### Example: Splitting a Program

**add.cpp:**
```cpp
int add(int x, int y)
{
    return x + y;
}
```

**main.cpp:**
```cpp
#include <iostream>

int add(int x, int y); // forward declaration needed!

int main()
{
    std::cout << "3 + 4 = " << add(3, 4) << '\n';
    return 0;
}
```

### Compiling Multiple Files

**Command line (g++):**
```bash
g++ main.cpp add.cpp -o main
```

**IDE:** Ensure all `.cpp` files are added to the project.

### Common Errors

- **"unresolved external symbol"** -- Usually means a `.cpp` file is not added to the project or build command.
- Missing forward declarations in the calling file.

> **Warning:** Never use `#include "add.cpp"` to include a source file. This causes ODR violations and other problems. Only include header files.

> **Key Takeaway:** Each `.cpp` file is compiled independently. Use forward declarations (or header files) to let one file know about functions defined in another. The linker connects everything together.

---

## 2.9 -- Naming Collisions and an Introduction to Namespaces

### Naming Collisions

A **naming collision** (or naming conflict) occurs when two identical identifiers are introduced in a way the compiler or linker cannot distinguish.

- Same name in the **same file** --> compiler error
- Same name in **different files** that get linked together --> linker error

### Namespaces

A **namespace** provides a named scope region. Identifiers declared inside a namespace are distinct from identically-named identifiers in other namespaces.

### The std Namespace

All standard library identifiers (like `cout`, `cin`, `endl`) are defined inside the `std` namespace. That is why we write `std::cout` -- we are using the **scope resolution operator** `::` to specify that `cout` belongs to the `std` namespace.

### The Global Namespace

Any identifier not defined inside a class, function, or namespace belongs to the **global namespace** (also called the global scope).

### Using Qualified Names

A **qualified name** includes its namespace prefix:

```cpp
std::cout << "Hello\n"; // std::cout is a qualified name
```

> **Warning:** Avoid `using namespace std;` (or any using-directive) at the top of a file or in a header. This defeats the purpose of namespaces and can reintroduce naming collisions.

```cpp
#include <iostream>

using namespace std; // BAD PRACTICE -- avoid this

int cout() // user-defined function
{
    return 5;
}

int main()
{
    cout << "Hello"; // ERROR: ambiguous -- std::cout or ::cout()?
    return 0;
}
```

> **Best Practice:** Always use explicit namespace prefixes (e.g., `std::cout`) to access identifiers in a namespace.

> **Key Takeaway:** Namespaces prevent naming collisions by grouping identifiers into named scopes. Always qualify standard library names with `std::` and never use `using namespace std;`.

---

## 2.10 -- Introduction to the Preprocessor

The **preprocessor** is a program that processes your source code *before* the compiler sees it. It scans for **directives** -- lines that begin with `#` -- and performs text substitutions and conditional inclusions.

### Translation

The entire process of preprocessing, compiling, and linking is called **translation**. After preprocessing, the resulting code is called a **translation unit**.

### #include Directive

`#include` replaces itself with the entire contents of the specified file:

```cpp
#include <iostream> // replaced with the contents of the iostream header
```

### #define Directive (Object-Like Macros)

**With substitution text:**
```cpp
#define MY_NAME "Alex"

std::cout << "My name is " << MY_NAME; // MY_NAME is replaced with "Alex"
```

> **Best Practice:** Avoid `#define` macros with substitution text. Use `const` or `constexpr` variables instead (covered later).

**Without substitution text (used for conditional compilation):**
```cpp
#define ENABLE_DEBUG // defines the identifier with no value
```

> **Convention:** Macro names should be ALL_UPPERCASE_WITH_UNDERSCORES.

### Conditional Compilation

```cpp
#define PRINT_JOE

int main()
{
#ifdef PRINT_JOE
    std::cout << "Joe\n";   // compiled -- PRINT_JOE is defined
#endif

#ifdef PRINT_BOB
    std::cout << "Bob\n";   // excluded -- PRINT_BOB is NOT defined
#endif

#ifndef PRINT_BOB
    std::cout << "Not Bob\n"; // compiled -- PRINT_BOB is NOT defined
#endif

    return 0;
}
```

You can also use the more readable forms:
```cpp
#if defined(PRINT_JOE)   // same as #ifdef PRINT_JOE
#if !defined(PRINT_BOB)  // same as #ifndef PRINT_BOB
```

### The #if 0 Trick

Use `#if 0` to exclude a block of code from compilation (useful for "commenting out" code that contains multi-line comments):

```cpp
#if 0
    std::cout << "This code is excluded\n";
    /* This multi-line comment won't cause problems */
#endif
```

Change `#if 0` to `#if 1` to re-enable the block.

### Scope of #define

- Directives are resolved **top-to-bottom** within a file.
- Directives in one `.cpp` file do **not** affect other `.cpp` files.
- Directives defined inside a function still apply to the rest of the file (the preprocessor does not understand C++ scope rules).

> **Key Takeaway:** The preprocessor handles `#include`, `#define`, and conditional compilation before the compiler sees the code. Prefer modern C++ alternatives (const, constexpr, inline) over macros whenever possible.

---

## 2.11 -- Header Files

**Header files** (`.h` or `.hpp` extension) contain declarations (typically forward declarations) that can be `#include`d into multiple source files. They eliminate the need to manually write forward declarations in every file.

### Basic Example

**add.h:**
```cpp
#ifndef ADD_H
#define ADD_H

int add(int x, int y); // forward declaration

#endif
```

**add.cpp:**
```cpp
#include "add.h"

int add(int x, int y)
{
    return x + y;
}
```

**main.cpp:**
```cpp
#include "add.h"
#include <iostream>

int main()
{
    std::cout << "3 + 4 = " << add(3, 4) << '\n';
    return 0;
}
```

### Angle Brackets vs. Double Quotes

| Syntax | Use For | Search Behavior |
|--------|---------|-----------------|
| `#include <header>` | Standard library and system headers | Searches system include directories |
| `#include "header.h"` | Your own project headers | Searches project directory first, then system directories |

### Recommended Include Order

Include headers in this order to catch missing dependencies early:

1. The paired header file (e.g., `add.cpp` includes `add.h` first)
2. Other project headers
3. Third-party library headers
4. Standard library headers

### Critical Rules

- **Never `#include` a `.cpp` file.** This causes ODR violations and other problems.
- **Do not put function or variable definitions in header files** (for now). Only put declarations.
- **Do not rely on transitive includes.** If your file uses `std::cout`, include `<iostream>` directly, even if another header you include happens to include it.
- **Each header should have a paired `.cpp` file** with the same base name.

> **Key Takeaway:** Header files are the standard mechanism for sharing declarations across multiple source files. Always use header guards, include your own headers with double quotes, and never include `.cpp` files.

---

## 2.12 -- Header Guards

### The Problem: Duplicate Definitions

If a header file is included more than once in the same translation unit (directly or indirectly through other headers), any definitions in that header will be duplicated, causing compiler errors.

### The Solution: Header Guards

**Header guards** (or **include guards**) are preprocessor directives that prevent a header's contents from being included more than once in a single translation unit:

```cpp
#ifndef SQUARE_H
#define SQUARE_H

int square(int x); // declaration

#endif
```

**How it works:**

1. First time `square.h` is included: `SQUARE_H` is not defined, so `#ifndef` passes, `SQUARE_H` gets defined, and the header content is included.
2. Second time `square.h` is included: `SQUARE_H` is already defined, so `#ifndef` fails, and the entire header content is skipped.

### Naming Convention

Use the full filename in UPPER_CASE with underscores:

| Header File | Guard Name |
|-------------|------------|
| `square.h`  | `SQUARE_H` |
| `my_math.h` | `MY_MATH_H` |
| `graphics_util.h` | `GRAPHICS_UTIL_H` |

### Header Guards vs. Multiple Files

Header guards prevent duplicate inclusion **within a single translation unit** (a single `.cpp` file after preprocessing). They do **not** prevent a header from being included once in multiple different `.cpp` files -- and that is correct behavior.

This is why function **definitions** should go in `.cpp` files, not headers. If a header with a function definition is included by two different `.cpp` files, each translation unit gets its own copy, and the linker reports a duplicate definition error.

### #pragma once

`#pragma once` is a simpler, non-standard alternative supported by most modern compilers:

```cpp
#pragma once

int square(int x);
```

While widely supported, it is not part of the C++ standard and may not work with all compilers.

> **Best Practice:** Always add header guards to every header file, even if you think it will only be included once. Use the traditional `#ifndef` / `#define` / `#endif` pattern for maximum portability.

> **Key Takeaway:** Header guards prevent the same header from being processed multiple times within a single translation unit. They are essential for avoiding duplicate definition errors. Always use them in every header file you write.
