# Chapter 3: Debugging C++ Programs

---

## 3.1 -- Syntax and Semantic Errors

Programming errors fall into two broad categories: **syntax errors** and **semantic errors**.

### Syntax Errors

A **syntax error** occurs when you write code that violates the grammar rules of C++. The compiler catches these immediately and refuses to compile.

Common examples:
- Missing semicolons
- Mismatched parentheses or braces
- Invalid variable names (e.g., starting with a digit)
- Using non-existent operators

```cpp
int main()
{
    std::cout << "Hello"  // ERROR: missing semicolon
    return 0;
}
```

### Semantic Errors

A **semantic error** occurs when code is syntactically valid but does not do what the programmer intended. Semantic errors fall into two sub-categories:

**Compile-time semantic errors** (caught by the compiler):
- Using undeclared variables
- Type mismatches
- Invalid operations (e.g., assigning to a literal)

```cpp
int main()
{
    5 = 3; // ERROR: cannot assign to a literal
    return 0;
}
```

**Runtime semantic errors** (only discovered when the program runs):
- Division by zero (undefined behavior)
- Using uninitialized variables
- Logic errors (wrong operator, wrong formula)
- Unreachable code after a return statement

```cpp
int add(int x, int y)
{
    return x - y; // BUG: subtraction instead of addition
}
```

> **Key Takeaway:** Syntax errors are easy -- the compiler tells you exactly what is wrong. Semantic errors, especially runtime ones, are the real challenge. They require systematic debugging techniques to find and fix.

---

## 3.2 -- The Debugging Process

Debugging is the process of finding and removing errors (bugs) from your program. This lesson presents a systematic six-step approach.

### The Six Steps of Debugging

1. **Identify the root cause** -- Find the specific line or lines causing the incorrect behavior.
2. **Understand the problem** -- Figure out *why* the code is producing incorrect results.
3. **Determine the fix** -- Plan how to correct the issue.
4. **Apply the fix** -- Make the necessary code changes.
5. **Test the fix** -- Verify the problem is resolved.
6. **Regression test** -- Confirm that the fix did not introduce new bugs elsewhere.

### Example: Finding a Bug

```cpp
#include <iostream>

int add(int x, int y)
{
    return x - y; // Bug: wrong operator
}

int main()
{
    std::cout << "5 + 3 = " << add(5, 3) << '\n';
    return 0;
}
```

- **Expected output:** `5 + 3 = 8`
- **Actual output:** `5 + 3 = 2`
- **Root cause:** `return x - y` should be `return x + y`

### Key Insight

When debugging, remember: *something you thought was correct, isn't.* Debugging requires challenging your own assumptions about how the code behaves.

> **Best Practice:** Practice incremental development -- write small amounts of code, test frequently. When a bug appears, you know it is in the recently added code, drastically narrowing the search area.

> **Key Takeaway:** Debugging is a structured process, not random guessing. Follow the six steps systematically: identify, understand, plan, fix, test, and regression test.

---

## 3.3 -- A Strategy for Debugging

### Two Approaches to Finding Bugs

**1. Code Inspection:**
- Read through the source code looking for errors
- Works well for small programs
- Becomes impractical for large codebases
- Prone to failure because the programmer often makes the same wrong assumptions when reviewing

**2. Runtime Observation (Recommended):**

A three-step process:
1. **Reproduce the problem** consistently
2. **Run the program** and gather diagnostic information
3. **Iteratively narrow down** the location of the bug

### Reproducing the Problem

Before you can fix a bug, you must be able to **reproduce** it reliably. Document clear **reproduction steps** that trigger the bug every time (or at least frequently).

Without reproducible failure, you cannot confirm that a fix actually works.

### The Hi-Lo Narrowing Strategy

Debugging uses a binary-search-like approach to narrow down the problem location:

- If the program state is correct at point A, the bug occurs **after** point A
- If the program state is incorrect at point B, the bug occurs **before** point B
- Keep narrowing the range between "last known good" and "first known bad"

This is analogous to a number-guessing game where each guess eliminates half the remaining possibilities.

### Making Educated Guesses

With experience, you can develop intuition about likely bug locations based on:
- The nature of the symptoms
- The program's structure
- Recent code changes

> **Key Takeaway:** Debugging is detective work. Reproduce the bug reliably, then systematically narrow down where it occurs using a binary-search strategy. The goal is to find the exact line causing the problem.

---

## 3.4 -- Basic Debugging Tactics

Three fundamental tactics for manual debugging without specialized tools.

### Tactic 1: Commenting Out Code

Systematically comment out sections of code to isolate the bug:

- If the bug **disappears**, the commented code is the culprit
- If the bug **persists**, the commented code is innocent (can be excluded)
- If the bug **changes**, the commented code interacts with the problem

> **Warning:** Keep track of what you have commented out. Use version control to diff your changes so you do not accidentally leave debug modifications in your code.

### Tactic 2: Validating Code Flow

Insert output statements at function entry points to verify that functions are being called in the expected order and the expected number of times:

```cpp
#include <iostream>

int getValue()
{
    std::cerr << "getValue() called\n"; // debug trace
    std::cout << "Enter an integer: ";
    int input{};
    std::cin >> input;
    return input;
}
```

### Why std::cerr Instead of std::cout?

| Feature | `std::cout` | `std::cerr` |
|---------|-------------|-------------|
| Buffering | Buffered (output may be delayed) | Unbuffered (output appears immediately) |
| Purpose | Normal program output | Error/diagnostic output |
| Crash safety | May lose buffered output on crash | Output guaranteed before crash |

Using `std::cerr` for debug output ensures messages appear immediately and are clearly separated from normal program output.

### Tactic 3: Printing Variable Values

Output variable values at strategic points to verify they contain expected data:

```cpp
int main()
{
    int x{ getUserInput() };
    std::cerr << "main::x = " << x << '\n'; // debug: check value

    int result{ calculate(x) };
    std::cerr << "main::result = " << result << '\n'; // debug: check value

    printResult(result);
    return 0;
}
```

**Tips:**
- Always label your debug output clearly (include variable name and context)
- Place debug statements after key operations
- Work backward from incorrect output to find where values diverge from expectations

### Suppressing Code Formatters

If you use an auto-formatter like clang-format, wrap debug statements to prevent reformatting:

```cpp
// clang-format off
std::cerr << "debug: x = " << x << '\n';
// clang-format on
```

### Limitations of Print Debugging

1. Debug statements clutter the source code
2. Debug output clutters the program output
3. Adding/removing statements risks introducing new bugs
4. Statements must be removed after debugging (not reusable)

> **Key Takeaway:** Commenting out code, tracing execution flow, and printing variable values are simple but effective debugging techniques. Use `std::cerr` for all debug output. Be aware of the limitations -- more advanced tools exist.

---

## 3.5 -- More Debugging Tactics

This lesson addresses the limitations of basic print debugging and introduces improved approaches.

### Tactic 1: Conditional Debug Output

Use preprocessor directives to enable or disable all debug statements with a single change:

```cpp
#define ENABLE_DEBUG // comment this line out to disable all debug output

#include <iostream>

int getUserInput()
{
#ifdef ENABLE_DEBUG
    std::cerr << "getUserInput() called\n";
#endif
    std::cout << "Enter a number: ";
    int x{};
    std::cin >> x;
    return x;
}
```

**Advantages:**
- Debug statements are reusable (just uncomment `#define ENABLE_DEBUG`)
- Single point of control for all debug output
- Can be placed in a header file for multi-file projects

**Drawbacks:**
- Still clutters the code with `#ifdef` / `#endif` blocks
- Risk of typos in macro names

### Tactic 2: Logging

A **log** is a sequential, time-stamped record of events written to a **log file** on disk.

**Advantages over print debugging:**
- Debug output is separated from program output (written to a file)
- Log files can be shared with others for remote diagnosis
- Non-intrusive to normal program operation
- Widely used in professional software

**C++ provides `std::clog`** for logging output, but third-party libraries are typically preferred.

### Using a Logging Library (plog)

[plog](https://github.com/SergiusTheBest/plog) is a lightweight, header-only logging library:

```cpp
#include <plog/Log.h>
#include <plog/Initializers/RollingFileInitializer.h>

int getUserInput()
{
    PLOGD << "getUserInput() called"; // debug log message

    std::cout << "Enter a number: ";
    int x{};
    std::cin >> x;
    return x;
}

int main()
{
    plog::init(plog::debug, "Logfile.txt"); // initialize logger

    int x{ getUserInput() };
    PLOGD << "main::x = " << x;

    return 0;
}
```

Log output includes timestamps and function names automatically. To disable logging, change the severity level:

```cpp
plog::init(plog::none, "Logfile.txt"); // suppresses most messages
```

> **Best Practice:** For anything beyond trivial programs, use a logging library instead of manual print debugging. It keeps your code cleaner and your debug output organized.

> **Key Takeaway:** Conditional compilation and logging libraries solve the major limitations of basic print debugging. Logging separates debug output from program output and provides reusable, professional-grade diagnostics.

---

## 3.6 -- Using an Integrated Debugger: Stepping

An **integrated debugger** is a debugger built into your IDE that lets you control program execution and inspect program state without modifying your source code.

### Program State

The **program state** is the complete snapshot of a running program at any point in time: all variable values, the call stack, and the current point of execution.

### Prerequisites

Before using the debugger:
- Ensure your project uses a **debug build configuration** (not release)
- Disable compiler optimizations so the debugger can accurately map execution to your source code

### Stepping Commands

Stepping lets you execute your program one statement at a time, pausing after each to inspect the state.

#### Step Into (F11 in Visual Studio, Shift+F7 in Code::Blocks)

Executes the next statement. If that statement contains a function call, the debugger enters the function and pauses at its first line.

```cpp
#include <iostream>

void printValue(int value)
{
    std::cout << value << '\n';
}

int main()
{
    printValue(5);  // Step Into here jumps into printValue()
    return 0;
}
```

#### Step Over (F10 in Visual Studio, F7 in Code::Blocks)

Executes the next statement, but if it contains a function call, the entire function is executed without pausing inside it. Use this to skip over functions you trust are correct.

#### Step Out (Shift+F11 in Visual Studio, Ctrl+F7 in Code::Blocks)

Executes all remaining statements in the current function, then pauses after the function returns to its caller. Useful when you accidentally step into a function you did not want to inspect.

### When to Use Each Command

| Command | Use When |
|---------|----------|
| Step Into | You want to inspect what happens inside a function call |
| Step Over | You trust a function works correctly and want to skip it |
| Step Out | You accidentally entered a function and want to leave |

### Dealing with std::cout Buffering

During debugging, output from `std::cout` may not appear immediately due to buffering. Add this at the top of `main()` to force immediate output:

```cpp
std::cout << std::unitbuf; // enable automatic flushing for debugging
```

Remove it when you are done debugging.

> **Best Practice:** Learn to use the debugger early in your C++ journey. The time invested pays enormous dividends as your programs grow more complex.

> **Key Takeaway:** Stepping lets you execute your program line-by-line and observe the state at each step. Master Step Into, Step Over, and Step Out -- they are your primary navigation tools in the debugger.

---

## 3.7 -- Using an Integrated Debugger: Running and Breakpoints

### Run to Cursor

Executes the program until it reaches the line where your cursor is positioned, then pauses.

| IDE | Shortcut |
|-----|----------|
| Visual Studio | Ctrl+F10 or right-click > Run to Cursor |
| Code::Blocks | F4 |
| VS Code | Right-click > Run to Cursor |

This is the fastest way to jump to a specific point in your program without stepping through every line.

### Continue

Resumes execution from the current paused state until the program ends or hits another pause condition (like a breakpoint).

| IDE | Shortcut |
|-----|----------|
| Visual Studio | F5 |
| Code::Blocks | F8 |
| VS Code | F5 |

### Breakpoints

A **breakpoint** is a marker on a line of code that tells the debugger to pause execution whenever that line is about to execute.

#### Setting Breakpoints

| IDE | Methods |
|-----|---------|
| Visual Studio | F9, right-click menu, or click in the left margin |
| Code::Blocks | F5, right-click menu, or click right of line number |
| VS Code | F9 or click in the left margin |

Breakpoints appear as red circles or dots in the margin.

#### Breakpoints vs. Run to Cursor

| Feature | Run to Cursor | Breakpoint |
|---------|---------------|------------|
| Persistence | One-time use | Persists across runs |
| Reuse | Must re-select each time | Triggers automatically |
| Multiple points | Only one at a time | Can set many simultaneously |

### Set Next Statement (Jumping)

This feature changes the execution point to a different line, **skipping** or **re-executing** code.

| IDE | Shortcut |
|-----|----------|
| Visual Studio | Ctrl+Shift+F10 |
| Code::Blocks | Right-click > Set Next Statement |
| VS Code | Right-click > Jump to Cursor |

> **Warning:** Set Next Statement only moves the execution point -- it does NOT undo any state changes. Variables retain whatever values they had. This is fundamentally different from "step back."

> **Warning:** Never use Set Next Statement to jump between different functions. This causes undefined behavior and likely crashes.

### Practical Workflow

1. Set a breakpoint at the area of interest
2. Start debugging (the program runs and pauses at the breakpoint)
3. Inspect variables and program state
4. Use Continue, Step Into, or Step Over to proceed
5. Repeat until the bug is found

> **Key Takeaway:** Breakpoints are the most important debugger feature. They let you pause execution at any line without modifying code. Combine them with Continue and stepping commands for efficient debugging workflows.

---

## 3.8 -- Using an Integrated Debugger: Watching Variables

The ability to inspect variable values during execution is one of the debugger's most powerful features.

### Three Ways to Inspect Variables

#### 1. Hover Inspection

Simply hover your mouse cursor over a variable while the program is paused. A tooltip shows the current value.

#### 2. QuickWatch (Visual Studio)

Right-click a variable and select QuickWatch to see its value in a popup window.

#### 3. Watch Window

A dedicated panel where you can add variables to monitor continuously as you step through the program.

### Using the Watch Window

**Adding variables:**
- Type the variable name directly into the watch window, or
- Right-click a variable in the code and select "Add Watch"

**Accessing the watch window:**

| IDE | How to Open |
|-----|-------------|
| Visual Studio | Debug > Windows > Watch > Watch 1 |
| Code::Blocks | Debug > Debugging windows > Watches |
| VS Code | Appears automatically in left panel during debugging |

### Example Program

```cpp
#include <iostream>

int main()
{
    int x{ 1 };
    std::cout << x << ' ';

    x = x + 2;
    std::cout << x << ' ';

    x = x + 3;
    std::cout << x << ' ';

    return 0;
}
```

As you step through this program with `x` in the watch window, you observe:
- After `int x{ 1 };` --> x = 1
- After `x = x + 2;` --> x = 3
- After `x = x + 3;` --> x = 6

### Watch Window Features

- **Out-of-scope variables:** Shown as grayed out or "not available" but remain in the list
- **Expression evaluation:** You can type expressions like `x + 2` or `x * y` in the watch window to see computed results
- **The Locals window:** Automatically shows all local variables in the current scope (no manual adding needed)

> **Warning:** Make sure your project is compiled using a **debug build configuration**. Release builds may optimize away variables, causing the debugger to show incorrect or missing values.

> **Key Takeaway:** The watch window lets you monitor variable values in real time as you step through your program. Combined with breakpoints and stepping, it gives you complete visibility into your program's state.

---

## 3.9 -- Using an Integrated Debugger: The Call Stack

The **call stack** is a data structure that tracks all active function calls at any point during program execution. It shows you *how* the program arrived at the current line.

### How the Call Stack Works

- When a function is called, an entry is **pushed** onto the top of the stack
- When a function returns, its entry is **popped** off the stack
- The stack shows the chain of function calls from `main()` at the bottom to the currently executing function at the top

### Accessing the Call Stack Window

| IDE | How to Open |
|-----|-------------|
| Visual Studio | Debug > Windows > Call Stack |
| Code::Blocks | Debug > Debugging windows > Call stack |
| VS Code | Appears automatically in left panel during debugging |

### Example

```cpp
#include <iostream>

void a()
{
    std::cout << "a() called\n"; // breakpoint here
}

void b()
{
    std::cout << "b() called\n";
    a();
}

int main()
{
    a();
    b();
    return 0;
}
```

**When the breakpoint in `a()` is hit from `main()` calling `a()` directly:**

```
a()    -- line 5 (currently executing)
main() -- line 16 (will resume here when a() returns)
```

**When the breakpoint in `a()` is hit from `b()` calling `a()`:**

```
a()    -- line 5 (currently executing)
b()    -- line 12 (will resume here when a() returns)
main() -- line 17 (will resume here when b() returns)
```

### Reading the Call Stack

- Read **bottom to top** to trace the sequence of calls
- The **topmost** entry is the currently executing function
- The **line numbers** shown for each function indicate the **next line to execute** when control returns to that function
- Double-clicking a stack entry (in some IDEs) navigates to that location in the source code

### Practical Uses

- **Verify execution path:** Confirm your program reached a point through the expected sequence of calls
- **Debug unexpected calls:** If a function is called from an unexpected location, the call stack reveals the actual call chain
- **Understand recursion:** See how many recursive calls are active and their nesting depth

> **Key Takeaway:** The call stack shows the chain of function calls that led to the current execution point. It is essential for understanding program flow, especially in programs with many functions calling each other. Read the stack from bottom to top to trace the call sequence.

---

## Summary: Essential Debugging Toolkit

| Tool / Technique | When to Use |
|-----------------|-------------|
| Print debugging (`std::cerr`) | Quick checks in small programs |
| Conditional debug (`#ifdef`) | Reusable debug output across a project |
| Logging library | Professional projects; long-running programs |
| Step Into / Over / Out | Line-by-line execution inspection |
| Breakpoints | Pause at specific locations without code changes |
| Watch window | Monitor variable values during execution |
| Call stack | Understand the chain of function calls |

> **Final Takeaway:** Start with the debugger, not print statements. Learning to use breakpoints, stepping, the watch window, and the call stack early will save you enormous amounts of time as your programs grow in complexity. Reserve print debugging and logging for situations where a debugger is impractical (e.g., intermittent bugs, production environments).
