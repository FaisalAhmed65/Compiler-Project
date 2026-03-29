# TS2C Compiler — Project Documentation

## 1. Project Overview

**TS2C** is a **TypeScript-to-C translator** that compiles a subset of TypeScript into valid C99 code. The compiler implements a complete pipeline:

1. **Lexical Analysis** (Flex) → tokenizes input
2. **Syntax Analysis** (Bison) → parses tokens into an Abstract Syntax Tree (AST)
3. **Semantic Analysis** → builds the AST with type information
4. **Intermediate Code Generation** → converts AST to Three-Address Code (TAC)
5. **Optimization** → applies 4 passes of TAC optimization
6. **Code Generation** → emits the final C99 source

---

## 2. Architecture & Components

### 2.1 Lexer (`ts_lexer.l`)
- **Generator:** Flex 2.5.4
- **Input:** TypeScript source (`.ts`)
- **Output:** Token stream (Bison tokens: `TOK_LET`, `TOK_CONST`, `TOK_FUNCTION`, etc.)
- **Special handling:** `console.log` matched as single `TOK_PRINT` token

### 2.2 Parser (`parser.y`)
- **Generator:** Bison (GNU yacc)
- **Input:** Token stream
- **Output:** Abstract Syntax Tree (AST)
- **Grammar:** Full statement types (var/const, functions, if/else, while, for, blocks, expressions)
- **Operator precedence:** Fully declared with `%left`, `%right`, `%nonassoc`
- **YYSTYPE union:** `dval` (double), `ival` (int), `sval` (string), `node` (AST), `tval` (TypeScript type enum)

### 2.3 Abstract Syntax Tree (`ast.h`, `ast.c`)
- **Single universal node type:** `ASTNode` struct with:
  - `type` (21 node types: `NODE_PROGRAM`, `NODE_VAR_DECL`, `NODE_FUNC_DECL`, `NODE_IF`, `NODE_WHILE`, `NODE_FOR`, `NODE_BLOCK`, `NODE_CALL`, `NODE_RETURN`, etc.)
  - `ts_type` (TypeScript type: `TS_NUMBER`, `TS_BOOL`, `TS_STRING`, `TS_VOID`, `TS_NULL`)
  - `str_val`, `num_val`, `bool_val` (literal values)
  - `op[4]` (operator for binary/unary ops)
  - `left`, `right`, `extra` (child pointers)
  - `next` (sibling list)
  - `is_const` (const flag)
- **API:** constructors for all 21 node types, `ast_print()` for debug dump, `ast_free()` for cleanup

### 2.4 Three-Address Code Generation (`codegen.h`, `codegen.c`)
- **Two-pass design:**
  - **Pass 1 (`generate_tac`):** Walk AST recursively, emit TAC instructions into linked list
  - **Pass 2 (`emit_c_code`):** Two-pass emitter: (A) emit top-level code in `main()`, (B) emit function definitions
- **28 TAC opcodes:**
  - Assignments: `TAC_ASSIGN`
  - Arithmetic: `TAC_ADD`, `TAC_SUB`, `TAC_MUL`, `TAC_DIV`, `TAC_MOD`, `TAC_NEG`
  - Relational: `TAC_EQ`, `TAC_NEQ`, `TAC_LT`, `TAC_GT`, `TAC_LEQ`, `TAC_GEQ`
  - Logical: `TAC_AND`, `TAC_OR`, `TAC_NOT`
  - Control: `TAC_LABEL`, `TAC_GOTO`, `TAC_IF_FALSE`, `TAC_RETURN`
  - Functions: `TAC_FUNC_BEGIN`, `TAC_FUNC_END`, `TAC_PARAM`, `TAC_CALL`
  - I/O: `TAC_PRINT`, `TAC_DECLARE`, `TAC_NOP`
- **Temporary variables:** `_t0`, `_t1`, `_t2`, ...
- **Labels:** `L0`, `L1`, `L2`, ...

### 2.5 Optimizer (`optimizer.h`, `optimizer.c`)
- **4-pass optimization pipeline:**

  **Pass 1: Constant Folding**
  - Evaluates binary operations where both operands are numeric constants
  - Example: `3 + 4` → rewrite as `TAC_ASSIGN _t5 = 7`

  **Pass 2: Constant Propagation**
  - Maintains `const_map[name → value]` for all constant assignments
  - When a variable is used as an operand, substitute its known constant value
  - Example: `const x = 10; sum = x + y` → substitute `10` for `x`

  **Pass 3: Copy Propagation**
  - Tracks simple assignments `x = y` (copies), rewires subsequent uses
  - Example: `sum = _t0; z = sum` → rewrite as `z = _t0`

  **Pass 4: Dead Code Elimination**
  - Identifies instructions whose destination (temp variable) is never used after
  - Removes those instructions
  - Example: `_t99 = 5 + 3` (if `_t99` never read) → delete

---

## 3. Build Process (Windows)

### 3.1 Prerequisites
- **Flex 2.5.4** (installed at `C:\Program Files (x86)\GnuWin32\bin\flex.exe`)
- **Bison** (installed at `C:\Program Files (x86)\GnuWin32\bin\bison.exe`)
- **GCC** (MSYS2 build: `gcc.exe` on PATH)

### 3.2 Build Steps (in order)

**Step 1: Clean generated files**
```powershell
Remove-Item lex.yy.c,lex.yy.o,parser.tab.c,parser.tab.h,parser.tab.o,ast.o,codegen.o,optimizer.o,ts2c.exe,output.c,result.exe -ErrorAction SilentlyContinue
```

**Step 2: Run Bison** (generates `parser.tab.c`, `parser.tab.h`)
```powershell
$env:BISON_PKGDATADIR="C:\PROGRA~2\GnuWin32\share\bison"
bison -d parser.y
```
*Note: Must set `BISON_PKGDATADIR` to short path `C:\PROGRA~2\...` to avoid m4 macro processor bug with spaces in `Program Files`.*

**Step 3: Run Flex** (generates `lex.yy.c`)
```powershell
flex ts_lexer.l
```

**Step 4: Compile all C files** (generates `.o` object files)
```powershell
gcc -Wall -g -c parser.tab.c lex.yy.c ast.c codegen.c optimizer.c
```

**Step 5: Link compiler executable**
```powershell
gcc -o ts2c.exe parser.tab.o lex.yy.o ast.o codegen.o optimizer.o -lm
```

### 3.3 Full Build One-Liner
```powershell
Remove-Item lex.yy.c,lex.yy.o,parser.tab.c,parser.tab.h,parser.tab.o,ast.o,codegen.o,optimizer.o,ts2c.exe,output.c,result.exe -ErrorAction SilentlyContinue; $env:BISON_PKGDATADIR="C:\PROGRA~2\GnuWin32\share\bison"; bison -d parser.y; flex ts_lexer.l; gcc -Wall -g -c parser.tab.c lex.yy.c ast.c codegen.c optimizer.c; gcc -o ts2c.exe parser.tab.o lex.yy.o ast.o codegen.o optimizer.o -lm
```

---

## 4. Using the Compiler

### 4.1 Basic Usage
```powershell
.\ts2c.exe <input.ts> <output.c>
```

### 4.2 Example
```powershell
.\ts2c.exe test_input.ts output.c
```

### 4.3 Output
The compiler prints:
1. **AST dump** — all parsed nodes with types and values
2. **TAC (before optimization)** — intermediate code instructions
3. **Optimization log** — constant folding, constant propagation, copy propagation, dead code elimination results
4. **TAC (after optimization)** — optimized instruction list
5. **Generated C code** written to `output.c`

---

## 5. TypeScript Language Supported

### 5.1 Variable Declarations
```typescript
let x: number = 10;
const y: boolean = true;
let z: string = "hello";
```

### 5.2 Types
- `number` → `double` in C
- `boolean` → `int` in C (0/1)
- `string` → `char*` in C
- `void` → `void` in C
- `null` → `0` in C

### 5.3 Literals
- Numbers: `10`, `3.14`, `-5`
- Booleans: `true`, `false`
- Strings: `"hello"`
- Null: `null`

### 5.4 Operators
- **Arithmetic:** `+`, `-`, `*`, `/`, `%`
- **Relational:** `<`, `>`, `<=`, `>=`, `==`, `!=`
- **Logical:** `&&`, `||`, `!`
- **Assignment:** `=`

### 5.5 Statements
- **Variable declaration:** `let x = 5;`, `const PI = 3.14;`
- **If/else:** `if (cond) { ... } else { ... }`
- **While loop:** `while (cond) { ... }`
- **For loop:** `for (let i = 0; i < 10; i++) { ... }`
- **Function declaration:** `function add(a: number, b: number): number { return a + b; }`
- **Function call:** `result = add(3, 4);`
- **Return:** `return value;`
- **Block:** `{ ... }`
- **Print:** `console.log(expr);` → `printf(...)`

---

## 6. Example: Complete Translation

### 6.1 Input TypeScript (`test_input.ts`)
```typescript
let x: number = 10;
let y: number = 20;
console.log(x + y);

function add(a: number, b: number): number {
    return a + b;
}

let result: number = add(3, 4);
console.log(result);
```

### 6.2 Generated C (`output.c`)
```c
/* Generated by TS2C compiler */
#include <stdio.h>
#include <string.h>

double add(double a, double b);

int main(void) {
    double x = 10;
    double y = 20;
    printf("%g\n", (double)(x + y));
    double result;
    double _t0 = add(3, 4);
    result = _t0;
    printf("%g\n", (double)(_t0));
    return 0;
}

double add(double a, double b) {
    double _t1 = a + b;
    return _t1;
}
```

### 6.3 Runtime Output
```
30
7
```

---

## 7. Three-Address Code (TAC) Example

For the expression `sum = (x + 10) * y;`, TAC looks like:
```
_t0 = x + 10      (addition)
_t1 = _t0 * y     (multiplication)
sum = _t1         (assignment)
```

For control flow (`if` statement):
```
_t2 = x < 5
if (!(_t2)) goto L0
  printf(...)
goto L1
L0:
  printf(...)
L1:
```

---

## 8. Optimization Example

### 8.1 Before Optimization (TAC)
```
_t0 = 3 + 4
result = _t0
printf(..., _t0)
```

### 8.2 After Optimization
```
result = 7         (constant folded: 3+4=7)
printf(..., 7)     (const propagation: _t0=7 → use 7)
```

---

## 9. File Structure

```
Compiler Project/
├── ts_lexer.l           (Flex lexer specification)
├── parser.y             (Bison parser specification)
├── ast.h / ast.c        (AST node definitions & API)
├── codegen.h / codegen.c (TAC generation & C emission)
├── optimizer.h / optimizer.c (4-pass optimizer)
│
├── test_input.ts        (Example TypeScript input)
│
├── Makefile             (Linux/macOS build — not used on Windows)
├── PROJECT_EXPLANATION.md (This file)
│
├── [Generated during build:]
├── parser.tab.c / parser.tab.h  (Bison output)
├── lex.yy.c             (Flex output)
├── *.o                  (Object files)
├── ts2c.exe             (Compiled compiler)
│
├── [Generated by compiler:]
└── output.c             (Translated C source)
```

---

## 10. Known Limitations & Future Work

### 10.1 Current Limitations
- **No arrays or pointers** (all `null` maps to `0`, not `NULL`)
- **No classes or objects**
- **No async/await**
- **No type narrowing or advanced type inference**
- **Limited error recovery** (parser may fail on ambiguous code)
- **No separate compilation** (single-pass)

### 10.2 Possible Extensions
- Array support with `[]` notation
- Pointer types and references
- Struct/union support
- Better error messages with line numbers
- Import/module system
- Incremental compilation

---

## 11. Troubleshooting

### Issue: `m4: cannot open 'Files': No such file or directory`
**Cause:** Bison path has spaces (`C:\Program Files (x86)\...`), and m4 can't parse it.
**Fix:** Always run `$env:BISON_PKGDATADIR="C:\PROGRA~2\GnuWin32\share\bison"` before `bison -d parser.y`.

### Issue: `lex.yy.c` not generated or "can't open lex.yy.c"
**Cause:** Flex bug with `-o` flag on GnuWin32.
**Fix:** Run `flex ts_lexer.l` (without `-o`), defaults to `lex.yy.c`.

### Issue: Linker error "undefined reference to..."
**Cause:** Missing object file in link command or typo in filename.
**Fix:** Ensure all 5 object files are listed: `parser.tab.o lex.yy.o ast.o codegen.o optimizer.o`

### Issue: Parser/semantic errors on valid TypeScript
**Cause:** Some grammar features not yet supported or Bison shift/reduce conflicts.
**Fix:** Simplify the input or check for unsupported syntax (e.g., no array literals).

---

## 12. Building & Running End-to-End

### Quick Start (Windows PowerShell)
```powershell
# 1. Clean
Remove-Item lex.yy.c,lex.yy.o,parser.tab.c,parser.tab.h,parser.tab.o,ast.o,codegen.o,optimizer.o,ts2c.exe,output.c,result.exe -ErrorAction SilentlyContinue

# 2. Build
$env:BISON_PKGDATADIR="C:\PROGRA~2\GnuWin32\share\bison"; bison -d parser.y
flex ts_lexer.l
gcc -Wall -g -c parser.tab.c lex.yy.c ast.c codegen.c optimizer.c
gcc -o ts2c.exe parser.tab.o lex.yy.o ast.o codegen.o optimizer.o -lm

# 3. Compile TypeScript
.\ts2c.exe test_input.ts output.c

# 4. Run generated C
gcc -o result.exe output.c -lm
.\result.exe
```

---

## Summary

**TS2C** demonstrates a complete compiler pipeline from lexing to code generation with optimization. The project is suitable for:
- Learning compiler design fundamentals
- Understanding lexer/parser generators (Flex/Bison)
- Studying intermediate code and optimization techniques
- Translating high-level to low-level code

All components are self-contained in a small codebase (~2000 lines total), making it an excellent reference for compiler construction.
