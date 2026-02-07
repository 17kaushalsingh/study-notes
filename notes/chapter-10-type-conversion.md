# Chapter 10: Type Conversion, Type Aliases, and Type Deduction

## 10.1 Implicit Type Conversion

Implicit conversion happens automatically when a value of one type is used where another is expected:

```cpp
double d{ 3 };           // int → double
if (5)                    // int → bool
double result{ 4.0 / 3 }; // int 3 → double 3.0
```

### Standard Conversion Categories
1. **Numeric promotions** — small types → int/double (value-preserving)
2. **Numeric conversions** — other numeric type changes (may lose data)
3. **Qualification conversions** — adding/removing const
4. **Pointer conversions** — between pointer types

---

## 10.2 Floating-Point and Integral Promotion

Promotion converts narrower types to wider types **without data loss**.

### Integral Promotions
These all promote to `int`:
- `bool` → `int` (false=0, true=1)
- `char`, `signed char`, `unsigned char` → `int`
- `short`, `unsigned short` → `int`

### Floating-Point Promotion
- `float` → `double`

```cpp
void printInt(int x) { std::cout << x; }
printInt('a');    // char promoted to int (97)
printInt(true);   // bool promoted to int (1)

void printDouble(double d) { std::cout << d; }
printDouble(3.5f); // float promoted to double
```

> **Key**: Not all widening conversions are promotions. `int` → `long` is a numeric conversion, not a promotion.

---

## 10.3 Numeric Conversions

Conversions that are NOT promotions. May lose data:

| Conversion | Risk |
|-----------|------|
| `int` → `float` | Loss of precision for large values |
| `double` → `float` | Loss of precision |
| `double` → `int` | Fractional part dropped |
| `int` → `short` | Value may overflow |
| `int` → `unsigned` | Negative values wrap around |

---

## 10.4 Narrowing Conversions and List Initialization

A **narrowing conversion** is one where the destination type can't represent all values of the source.

### List Initialization Prevents Narrowing

```cpp
int x{ 3.5 };      // COMPILE ERROR — narrowing!
int y = 3.5;        // Warning only — silently truncates to 3
int z( 3.5 );       // Warning only — silently truncates to 3
```

### Constexpr Exception

If the value is `constexpr` and fits in the destination, it's allowed:

```cpp
constexpr int n{ 5 };
unsigned int u{ n };    // OK: 5 fits in unsigned int
constexpr int neg{ -5 };
unsigned int u2{ neg }; // ERROR: -5 doesn't fit
```

### When Narrowing is Intentional

```cpp
double d{ 5.0 };
int i{ static_cast<int>(d) };  // explicit — no warning
```

---

## 10.5 Arithmetic Conversions

When binary operators have operands of different types, the compiler converts using these priority rules:

1. If either operand is `long double` → other becomes `long double`
2. If either is `double` → other becomes `double`
3. If either is `float` → other becomes `float`
4. Otherwise, integral promotion first, then:
   - If both same sign → smaller converts to larger
   - If unsigned rank ≥ signed rank → signed converts to unsigned
   - If signed can hold all unsigned values → unsigned converts to signed
   - Otherwise → both convert to unsigned version of the signed type

> **Warning**: Mixing signed and unsigned in arithmetic often converts signed to unsigned, causing bugs:
> ```cpp
> std::cout << 5u - 10;  // huge number (unsigned wraparound!)
> ```

---

## 10.6 Explicit Type Conversion (Casting)

### static_cast (Preferred)

```cpp
static_cast<new_type>(expression)

int i{ static_cast<int>(3.5) };           // 3
char ch{ static_cast<char>(97) };          // 'a'
double d{ static_cast<double>(5) / 3 };    // 1.666...
```

### Other Casts (Brief Overview)

| Cast | Use |
|------|-----|
| `static_cast` | Safe compile-time type conversion |
| `dynamic_cast` | Safe downcasting in inheritance |
| `const_cast` | Add/remove const (avoid!) |
| `reinterpret_cast` | Raw bit reinterpretation (dangerous!) |
| `(type)expr` | C-style cast — **avoid** (unsafe, no checks) |

> **Best Practice**: Always use `static_cast` for intentional type conversions. Never use C-style casts.

---

## 10.7 Typedefs and Type Aliases

### Type Alias (Modern — Preferred)

```cpp
using Distance = double;
using VecPairSI = std::vector<std::pair<std::string, int>>;
```

### Typedef (Legacy)

```cpp
typedef double Distance;
typedef std::vector<std::pair<std::string, int>> VecPairSI;
```

### When to Use Type Aliases
- Simplify complex types: `using StringMap = std::map<std::string, std::string>;`
- Platform independence: Fixed-width integers (`int32_t` is a typedef)
- Documentation: `using StudentId = int;`

### Naming Convention
Start with a capital letter: `Distance`, `StudentId`, `StringMap`

> **Warning**: Type aliases are NOT distinct types — `Distance` and `double` are interchangeable. The compiler won't prevent mixing `Miles` and `Kilometers` if both alias `double`.

---

## 10.8 Type Deduction with auto

```cpp
auto d{ 5.0 };       // double
auto i{ 1 + 2 };     // int
auto s{ "hello"sv };  // std::string_view
```

### auto Drops const and References

```cpp
const int x{ 5 };
auto y{ x };          // int (const dropped!)
const auto z{ x };    // const int (const reapplied)
```

### String Literal Gotcha

```cpp
auto s{ "hello" };    // const char* — NOT std::string!
auto s{ "hello"s };   // std::string (with using namespace std::string_literals)
auto s{ "hello"sv };  // std::string_view
```

> **Best Practice**: Use `auto` when the type doesn't matter or is obvious from context. Use explicit types when you need a specific type or clarity matters.

---

## 10.9 Type Deduction for Functions

```cpp
auto add(int x, int y) {
    return x + y;       // return type deduced as int
}
```

### Limitations
- All `return` statements must return the same type
- Forward declarations can't use `auto` return types
- Reduces readability (caller can't see the return type)

### Trailing Return Type

```cpp
auto add(int x, int y) -> int {  // explicit return type after ->
    return x + y;
}
```

> **Best Practice**: Avoid `auto` return types for normal functions (makes API harder to understand). Use when required by templates.

---

## Chapter 10 — Quick Reference Card

| Concept | Rule |
|---------|------|
| Narrowing conversions | Use brace init `{}` to catch them |
| Explicit conversion | Use `static_cast<>()` |
| C-style casts `(int)x` | **Avoid** — unsafe |
| `auto` drops const | Use `const auto` if needed |
| `auto` with strings | Deduces `const char*`, not `std::string` |
| Type aliases | `using Name = Type;` (prefer over typedef) |
| Mixed signed/unsigned | Signed may convert to unsigned — danger! |
