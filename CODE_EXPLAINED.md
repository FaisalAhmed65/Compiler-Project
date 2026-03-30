# TS2C Compiler — Complete Code Explanation (Line-by-Line)

---

## File 1: `ts_lexer.l` (Flex Lexer)

### What It Does:
Converts raw TypeScript source code into a stream of **tokens** (keywords, operators, literals, identifiers). Each matched pattern returns a token code that the parser understands.

### Line-by-Line Breakdown:

```flex
%{
/* ============================================================
   ts_lexer.l  –  Flex lexer for the TS2C compiler
   ============================================================ */

#include <stdio.h>              /* standard I/O (printf, stderr)            */
#include <stdlib.h>             /* memory & string functions                */
#include <string.h>             /* string functions (strlen, strcpy, etc.)  */
#include "ast.h"                /* AST node definitions needed for types    */
#include "parser.tab.h"         /* token codes from Bison (TOK_LET, etc.)   */

int yylineno = 1;               /* line counter updated by lexer            */
%}
```
- `%{...%}` = C code block that Flex includes at top of generated `lex.yy.c`
- `#include "parser.tab.h"` = Bison auto-generates this with token definitions

```flex
%option noyywrap                /* don't wrap to next file when done        */
```
- Tells Flex to not call an external `yywrap()` function when EOF is reached

```flex
DIGIT      [0-9]               /* regex pattern: single digit               */
LETTER     [a-zA-Z_$]          /* regex pattern: letter, underscore, $      */
ID         {LETTER}({LETTER}|{DIGIT})*  /* identifier: letter(s) then digit(s) */
INT_NUM    {DIGIT}+             /* integer: one or more digits               */
FLOAT_NUM  {DIGIT}+"."{DIGIT}*  /* float: digits.digits                      */
STRING     \"([^\"\\]|\\.)*\"   /* string: quoted text with escapes          */
```
- These are **macro definitions** used in the rules below
- Pattern matching uses basic regex

```flex
%%                              /* START OF LEXER RULES SECTION              */

 /* ── Keywords ─────────────────────────────────────────────── */
"let"           { return TOK_LET;      }  /* when Flex sees "let", return token code */
"const"         { return TOK_CONST;    }  /* when Flex sees "const", return token    */
"var"           { return TOK_VAR;      }  /* same for "var"                          */
"function"      { return TOK_FUNCTION; }  /* same for "function"                     */
"return"        { return TOK_RETURN;   }  /* same for "return"                       */
"if"            { return TOK_IF;       }  /* same for "if"                           */
"else"          { return TOK_ELSE;     }  /* same for "else"                         */
"while"         { return TOK_WHILE;    }  /* same for "while"                        */
"for"           { return TOK_FOR;      }  /* same for "for"                          */
```
- Each keyword is matched exactly and returns a token code
- Parser uses these codes to know which keyword was matched

```flex
"true"          { yylval.ival = 1; return TOK_TRUE;  }  /* set value=1 for true */
"false"         { yylval.ival = 0; return TOK_FALSE; }  /* set value=0 for false */
"null"          { return TOK_NULL;     }  /* null keyword                          */
"npc"           { return TOK_NULL;     }  /* project-specific alias for null       */
"cap"           { yylval.ival = 0; return TOK_FALSE; }  /* project-specific alias for false */
"console.log"   { return TOK_PRINT;    }  /* special case: match the full string   */
```
- `yylval` = Bison's semantic value union (stores data with token)
- `TOK_TRUE` and `TOK_FALSE` set `ival` to 1 or 0

```flex
 /* ── Types ────────────────────────────────────────────────── */
"number"        { return TOK_TYPE_NUMBER;  }  /* return type token code    */
"boolean"       { return TOK_TYPE_BOOLEAN; }  /* return type token code    */
"string"        { return TOK_TYPE_STRING;  }  /* return type token code    */
"void"          { return TOK_TYPE_VOID;    }  /* return type token code    */
```
- These are type annotation keywords

```flex
 /* ── Literals ─────────────────────────────────────────────── */
{FLOAT_NUM}     { yylval.dval = atof(yytext); return TOK_NUMBER; }
```
- `{FLOAT_NUM}` = use the FLOAT_NUM pattern defined earlier
- `yytext` = the actual matched text from source
- `atof()` = convert text to double
- `yylval.dval` = store the numeric value in union field `dval`

```flex
{INT_NUM}       { yylval.dval = atof(yytext); return TOK_NUMBER; }
```
- Same as float but for integers
- Both return `TOK_NUMBER` (Bison doesn't distinguish int vs float)

```flex
{STRING}        {
                    /* strip surrounding quotes */
                    int len = strlen(yytext) - 2;  /* remove opening & closing " */
                    yylval.sval = (char*)malloc(len + 1);  /* allocate string buffer */
                    strncpy(yylval.sval, yytext + 1, len); /* copy middle part */
                    yylval.sval[len] = '\0';               /* null-terminate string */
                    return TOK_STRING;
                }
```
- Matched text includes quotes, so strip them: `"hello"` → `hello`
- `malloc(len+1)` = allocate space for string + null terminator
- `strncpy(dest, src, len)` = copy `len` bytes exactly

```flex
 /* ── Identifiers ──────────────────────────────────────────── */
{ID}            { yylval.sval = strdup(yytext); return TOK_ID; }
```
- Any identifier (variable name) gets duplicate-allocated and returned
- `strdup()` = allocate memory and copy string in one call

```flex
 /* ── Operators ────────────────────────────────────────────── */
"==="           { return TOK_EQ;  }  /* JavaScript === maps to == in C */
"!=="           { return TOK_NEQ; }  /* JavaScript !== maps to != in C */
"=="            { return TOK_EQ;  }  /* standard equality              */
"!="            { return TOK_NEQ; }  /* standard inequality            */
"<="            { return TOK_LEQ; }  /* less-than-or-equal             */
">="            { return TOK_GEQ; }  /* greater-than-or-equal          */
"<"             { return TOK_LT;  }  /* less-than                      */
">"             { return TOK_GT;  }  /* greater-than                   */
"&&"            { return TOK_AND; }  /* logical AND                    */
"||"            { return TOK_OR;  }  /* logical OR                     */
"!"             { return TOK_NOT; }  /* logical NOT                    */
```
- Comparison and logical operators map to token codes

```flex
"+"             { return '+'; }     /* arithmetic operators return their char directly */
"-"             { return '-'; }     /* minus (or negation)                           */
"*"             { return '*'; }     /* multiplication                                */
"/"             { return '/'; }     /* division                                      */
"%"             { return '%'; }     /* modulo                                        */
"="             { return '='; }     /* assignment                                    */
```
- Simple operators return their ASCII character code directly
- Bison treats `'+'` as token code (ASCII 43)

```flex
 /* ── Separators / Punctuation ─────────────────────────────── */
"("             { return '('; }     /* return ASCII code of ( */
")"             { return ')'; }     /* return ASCII code of ) */
"{"             { return '{'; }     /* left brace                    */
"}"             { return '}'; }     /* right brace                   */
";"             { return ';'; }     /* semicolon                     */
","             { return ','; }     /* comma                         */
":"             { return ':'; }     /* colon (used in type anno.)    */
```
- Punctuation returns ASCII codes

```flex
 /* ── Comments ─────────────────────────────────────────────– */
"//"[^\n]*      { /* single-line comment – discard */ }
```
- `//` followed by anything except newline
- Action is empty = discard (don't return token)

```flex
"/*"([^*]|\*+[^*/])*\*+"/"  { /* block comment – discard */
                    char *p = yytext;                   /* pointer to matched text */
                    while (*p) { if (*p++ == '\n') yylineno++; }  /* count newlines */
                }
```
- `/* ... */` block comments
- Must count `\n` chars inside comment to keep `yylineno` accurate
- `*p++` = dereference and increment pointer (postfix increment)

```flex
 /* ── Whitespace ───────────────────────────────────────────── */
\n              { yylineno++; }     /* newline: increment line counter */
[ \t\r]+        { /* ignore */ }    /* space, tab, carriage-return: discard */
```
- Track line numbers for error reporting
- Ignore spaces and tabs (whitespace is not a token)

```flex
 /* ── Catch-all (invalid token) ───────────────────────────– */
.               {
                    fprintf(stderr, "Lexer: unknown character '%s' at line %d\n",
                            yytext, yylineno);
                }
```
- `.` = any character not matched above
- Print error but don't stop (lexer continues)

---

## File 2: `parser.y` (Bison Parser)

### What It Does:
**Consumes tokens** from the lexer and builds an **Abstract Syntax Tree (AST)** by applying grammar rules. The parser knows which statements/expressions are valid based on the rules defined here.

### Line-by-Line Breakdown:

```bison
%{
#include <stdio.h>              /* standard I/O                             */
#include <stdlib.h>             /* memory functions                         */
#include <string.h>             /* string functions                         */
#include "ast.h"                /* AST node type definitions                */
#include "codegen.h"            /* code generation functions                */
#include "optimizer.h"          /* optimization functions                   */

extern int  yylineno;           /* line counter from lexer                  */
extern int  yylex(void);        /* function that calls lexer                */
extern FILE *yyin;              /* input file handle                        */

void yyerror(const char *msg);  /* error handler (defined at end)           */

ASTNode *ast_root = NULL;       /* parse tree root (filled by parser)       */
%}
```
- `%{...%}` = C code block included in generated `parser.tab.c`
- `extern` = declare variables/functions defined elsewhere
- `ast_root` = global variable to hold the complete AST after parsing

```bison
%union {
    double   dval;   /* numeric value (literal number)                   */
    int      ival;   /* boolean / integer value                          */
    char    *sval;   /* string / identifier (pointer to char)            */
    ASTNode *node;   /* AST node pointer                                 */
    TSType   tval;   /* TypeScript type enum                             */
}
```
- `%union {...}` = Bison's semantic value type
- Each grammar rule has a value of one of these types
- `yylval.<field>` = access the appropriate field for current token

```bison
%token TOK_LET TOK_CONST TOK_VAR TOK_FUNCTION TOK_RETURN
%token TOK_IF TOK_ELSE TOK_WHILE TOK_FOR
%token TOK_PRINT TOK_NULL
%token TOK_TYPE_NUMBER TOK_TYPE_BOOLEAN TOK_TYPE_STRING TOK_TYPE_VOID
%token TOK_AND TOK_OR TOK_NOT
%token TOK_EQ TOK_NEQ TOK_LT TOK_GT TOK_LEQ TOK_GEQ
```
- Declare all token types that Flex returns
- Bison generates numeric codes for each (256, 257, 258, ...)

```bison
%token <dval> TOK_NUMBER       /* TOK_NUMBER carries double value         */
%token <sval> TOK_STRING TOK_ID /* TOK_STRING and TOK_ID carry string     */
%token <ival> TOK_TRUE TOK_FALSE /* Bison remembers associated yylval */
```
- Declare which token types carry semantic values and which field

```bison
%type <node> program stmt_list stmt
%type <node> var_decl func_decl param_list param_list_ne param
%type <node> block return_stmt if_stmt while_stmt for_stmt
%type <node> expr_stmt print_stmt
%type <node> expr assign_expr or_expr and_expr eq_expr rel_expr
%type <node> add_expr mul_expr unary_expr primary
%type <node> arg_list arg_list_ne
```
- `%type <node>` = non-terminal symbol produces an AST node
- `%type <tval>` = type annotation produces a TSType
- These tell Bison which `%union` field holds each symbol's value

```bison
%right '='                      /* right-associative: a = b = c → a = (b = c) */
%left  TOK_OR                   /* left-associative:  a || b || c → (a||b)||c */
%left  TOK_AND
%left  TOK_EQ TOK_NEQ
%left  TOK_LT TOK_GT TOK_LEQ TOK_GEQ
%left  '+' '-'
%left  '*' '/' '%'
%right TOK_NOT UMINUS           /* right-associative: !! a → !(!a), -(-a)    */
%nonassoc TOK_ELSE              /* non-associative: resolve dangling-else    */
```
- **Operator precedence** (lowest to highest)
- **Associativity**:
  - `%left` = group left: `a + b + c` → `(a+b)+c`
  - `%right` = group right: `a = b = c` → `a=(b=c)`
  - `%nonassoc` = error on associativity (e.g., `a < b < c` is invalid)
- **Ordering**: operators listed first have lower precedence

```bison
%%                              /* START OF GRAMMAR RULES */

program
    : stmt_list                 /* a program is: a list of statements      */
        {
            $$ = ast_new_node(NODE_PROGRAM);  /* create root AST node (NODE_PROGRAM) */
            $$->left  = $1;     /* left child = first statement             */
            $$->line  = 1;      /* source line                              */
            ast_root  = $$;     /* save root globally                       */
        }
    ;
```
- `program : stmt_list` = grammar rule: a program consists of statements
- `{ ... }` = semantic action (C code executed when rule matches)
- `$$` = the result value (what this rule produces)
- `$1` = value of first symbol (`stmt_list`)
- Creates a `NODE_PROGRAM` root with statement list as left child

```bison
stmt_list
    : /* empty */               /* first alternative: empty (no statements) */
        { $$ = NULL; }          /* return NULL                              */
    | stmt_list stmt            /* second alternative: stmt_list followed stmt */
        {
            if ($1 == NULL) {   /* if stmt_list was empty                   */
                $$ = $2;        /* result is the single stmt                */
            } else {
                ASTNode *cur = $1;                  /* start at head of list */
                while (cur->next) cur = cur->next;  /* traverse to end      */
                cur->next = $2;                     /* append new stmt      */
                $$ = $1;                            /* return updated list  */
            }
        }
    ;
```
- **Left-recursive rule** to build sibling-linked statements
- Continuously appends new statements to `next` pointer chain

---

## File 3: `ast.h` & `ast.c` (Abstract Syntax Tree)

### What It Does:
Defines a **universal AST node structure** that represents all language constructs. Each node has:
- **Type** (what kind of construct: statement, expression, etc.)
- **Children** (left, right, extra for sub-expressions/statements)
- **Sibling link** (next for statement lists)
- **Metadata** (line numbers, types, literals)

### `ast.h` Line-by-Line:

```c
typedef enum {
    /* Statements */
    NODE_PROGRAM,           /* root of entire AST               */
    NODE_VAR_DECL,          /* let x = 5; or const y = 10;      */
    NODE_FUNC_DECL,         /* function add(a, b) { ... }       */
    NODE_RETURN_STMT,       /* return value;                    */
    NODE_IF_STMT,           /* if (cond) { ... } else { ... }   */
    NODE_WHILE_STMT,        /* while (cond) { ... }             */
    NODE_FOR_STMT,          /* for (i=0; i<10; i++) { ... }     */
    NODE_BLOCK,             /* { statement; statement; }        */
    NODE_EXPR_STMT,         /* expression as statement          */
    NODE_PRINT_STMT,        /* console.log(...)                 */

    /* Expressions */
    NODE_ASSIGN,            /* x = value                        */
    NODE_BINOP,             /* a + b, a < b, etc.               */
    NODE_UNOP,              /* !a, -a                           */
    NODE_CALL,              /* foo(arg1, arg2)                  */
    NODE_IDENTIFIER,        /* variable name                    */
    NODE_NUMBER_LIT,        /* 42, 3.14                         */
    NODE_STRING_LIT,        /* "hello"                          */
    NODE_BOOL_LIT,          /* true, false                      */
    NODE_NULL_LIT,          /* null, npc (project alias)        */

    NODE_PARAM,             /* function parameter               */
    NODE_ARG_LIST           /* argument list in function call   */
} NodeType;
```
- Enum of all possible node types
- Each construct gets its own type

```c
typedef enum {
    TYPE_UNKNOWN = 0,       /* type not yet determined          */
    TYPE_NUMBER,            /* C: double                        */
    TYPE_BOOLEAN,           /* C: int  (0 or 1)                 */
    TYPE_STRING,            /* C: char*                         */
    TYPE_VOID,              /* C: void (no return value)        */
    TYPE_NULL               /* C: mapped to 0                   */
} TSType;
```
- Enum of TypeScript types
- Used for type annotation and code generation

```c
typedef struct ASTNode {
    NodeType    type;           /* what kind is this node?              */
    TSType      ts_type;        /* what TS type does it have?           */

    /* Leaf data (for literals and identifiers) */
    char       *str_val;        /* string literal or identifier name    */
    double      num_val;        /* numeric literal value                */
    int         bool_val;       /* boolean literal (0 or 1)             */

    /* Operator storage (for binary and unary ops) */
    char        op[4];          /* "+", "-", "==", "!=", etc.           */

    /* Child nodes (tree structure) */
    ASTNode    *left;           /* first operand / condition / first stmt */
    ASTNode    *right;          /* second operand / body / then-branch   */
    ASTNode    *extra;          /* else-branch / for-update stmt         */

    /* Sibling list (for statement/argument lists) */
    ASTNode    *next;           /* next statement or argument            */

    /* Declaration metadata */
    int         is_const;       /* 1 if declared const, 0 if let/var    */

    /* Location for error reporting */
    int         line;           /* source file line number               */
} ASTNode;
```
- **Universal node structure** used for any construct
- **Tree structure** via left/right/extra pointers
- **Linked list** via next pointer
- Accommodates all different node types despite single struct

### `ast.c` Line-by-Line:

```c
static char *safe_strdup(const char *s) {
    if (!s) return NULL;                    /* handle NULL input */
    char *p = (char *)malloc(strlen(s) + 1); /* allocate size+1 for null term */
    if (!p) { fprintf(stderr, "ast: out of memory\n"); exit(1); }
    strcpy(p, s);                           /* copy string        */
    return p;                               /* return new copy    */
}
```
- Safe string duplication with NULL check
- `strlen(s) + 1` = need +1 for null terminator `\0`
- Exit on allocation failure

```c
ASTNode *ast_new_node(NodeType type) {
    ASTNode *n = (ASTNode *)calloc(1, sizeof(ASTNode)); /* allocate & zero-init */
    if (!n) { fprintf(stderr, "ast: out of memory\n"); exit(1); }
    n->type = type;                         /* set node type                */
    return n;
}
```
- Generic node allocator
- `calloc(1, size)` = allocate size bytes, zero-initialize (all fields = 0/NULL)
- All fields start NULL except type

```c
ASTNode *ast_new_number(double value, int line) {
    ASTNode *n  = ast_new_node(NODE_NUMBER_LIT); /* create number node       */
    n->num_val  = value;                    /* store the numeric value      */
    n->ts_type  = TYPE_NUMBER;              /* set type to number           */
    n->line     = line;                     /* store source line            */
    return n;
}
```
- Create a numeric literal node
- Store value, type, and source location

```c
ASTNode *ast_new_string(const char *value, int line) {
    ASTNode *n  = ast_new_node(NODE_STRING_LIT);
    n->str_val  = safe_strdup(value);       /* duplicate the string         */
    n->ts_type  = TYPE_STRING;
    n->line     = line;
    return n;
}
```
- Create a string literal node
- Allocate memory for the string content

```c
ASTNode *ast_new_binop(const char *op, ASTNode *left,
                       ASTNode *right, int line) {
    ASTNode *n = ast_new_node(NODE_BINOP);
    strncpy(n->op, op, 3);                  /* copy operator (max 3 chars)      */
    n->op[3]   = '\0';                      /* null-terminate the op string     */
    n->left    = left;                      /* left operand                     */
    n->right   = right;                     /* right operand                    */
    n->line    = line;
    
    /* Type inference for binary operators */
    if (strcmp(op,"==")==0 || strcmp(op,"!=")==0 ||  /* comparison ops produce bool */
        strcmp(op,"<")==0  || ... ||
        strcmp(op,"&&")==0 || strcmp(op,"||")==0)
        n->ts_type = TYPE_BOOLEAN;          /* result is boolean                */
    else
        n->ts_type = left ? left->ts_type : TYPE_UNKNOWN;  /* arithmetic: copy left type */
    return n;
}
```
- Create a binary operation node (e.g., `a + b`, `x == y`)
- **Type inference**: comparisons always yield `bool`, arithmetic preserves left operand type
- `strncpy(dest, src, n)` = copy max n chars, may not null-terminate if src is long

```c
ASTNode *ast_new_var_decl(const char *name, TSType t, ASTNode *init,
                          int is_const, int line) {
    ASTNode *n = ast_new_node(NODE_VAR_DECL);
    n->str_val = safe_strdup(name);         /* store variable name              */
    n->ts_type = t;                         /* store declared type              */
    n->left    = init;                      /* optional initializer expression  */
    n->is_const = is_const;                 /* 1 if const, 0 if let/var        */
    n->line    = line;
    return n;
}
```
- Create a variable declaration node
- Stores: name, type annotation, init expression, const flag

---

## File 4: `codegen.h` & `codegen.c` (Three-Address Code)

### What It Does:
**Pass 1:** Walk the AST and generate **Three-Address Code (TAC)** — a simplified intermediate representation with:
- Simple operations (`+`, `-`, `*`, etc.)
- Temporary variables (`_t0`, `_t1`, ...)
- Labels (`L0`, `L1`, ...)

**Pass 2:** Convert TAC into C code.

### `codegen.h` Line-by-Line:

```c
typedef enum {
    TAC_ASSIGN,         /* result = arg1                        */
    TAC_ADD,            /* result = arg1 + arg2                 */
    TAC_SUB,            /* result = arg1 - arg2                 */
    ... (28 opcodes total)
    TAC_PRINT,          /* print arg1                           */
    TAC_NOP             /* no-operation                         */
} TACOp;
```
- Enum of all TAC instruction types
- Each operation is documented with its semantics

```c
typedef struct TAC {
    TACOp        op;              /* which operation (TAC_ADD, etc.)   */
    char        *result;          /* destination variable / label      */
    char        *arg1;            /* first source operand              */
    char        *arg2;            /* second source operand (if needed) */
    int          ival;            /* metadata: arg count, etc.         */
    int          is_const;        /* 1 if result is declared const    */
    TSType       ts_type;         /* type of result                    */
    struct TAC  *next;            /* linked list chain                 */
} TAC;
```
- Single TAC instruction
- Linked list of instructions forms the TAC program

### `codegen.c` Key Sections:

```c
static char *new_temp(void) {
    char buf[32];
    snprintf(buf, sizeof(buf), "_t%d", temp_count++); /* format: _t0, _t1, _t2 */
    return strdup(buf);
}
```
- Generate unique temp variable names
- Increments global `temp_count` each time

```c
static TAC *emit(TACOp op, const char *result, const char *arg1,
                  const char *arg2) {
    TAC *t = (TAC *)calloc(1, sizeof(TAC));
    t->op     = op;
    t->result = safe_dup(result);
    t->arg1   = safe_dup(arg1);
    t->arg2   = safe_dup(arg2);
    
    /* Append to linked list */
    if (!tac_tail) tac_head = t;
    else tac_tail->next = t;
    tac_tail = t;
    
    return t;
}
```
- Create a TAC instruction and append to linked list
- `safe_dup()` = allocate and copy strings
- Maintains `tac_head` and `tac_tail` for efficient list building

```c
char *gen_node(ASTNode *node) {
    if (!node) return NULL;
    
    switch (node->type) {
        case NODE_NUMBER_LIT:
            /* Convert number to string for TAC */
            char buf[32];
            snprintf(buf, 32, "%g", node->num_val);
            return strdup(buf);
        
        case NODE_IDENTIFIER:
            /* Just return variable name */
            return safe_dup(node->str_val);
        
        case NODE_BINOP: {
            /* Generate code for left and right operands first */
            char *left_val  = gen_node(node->left);
            char *right_val = gen_node(node->right);
            
            /* Create temp for result */
            char *temp = new_temp();
            
            /* Map operator to TAC opcode and emit */
            TACOp op = /* determine based on node->op */;
            emit(op, temp, left_val, right_val);
            
            /* Return temp as result */
            return temp;
        }
        
        case NODE_IF_STMT: {
            /* Generate condition code */
            char *cond_val = gen_node(node->left);
            
            /* Create labels for if/else branches */
            char *else_label = new_label();
            char *end_label = new_label();
            
            /* Emit: if not condition, goto else_label */
            emit(TAC_IF_FALSE, else_label, cond_val, NULL);
            
            /* Generate then-branch */
            gen_node(node->right);
            
            /* Emit: goto end_label */
            emit(TAC_GOTO, end_label, NULL, NULL);
            
            /* Emit else-label */
            emit(TAC_LABEL, else_label, NULL, NULL);
            
            /* Generate else-branch if exists */
            if (node->extra) gen_node(node->extra);
            
            /* Emit end-label */
            emit(TAC_LABEL, end_label, NULL, NULL);
            
            return NULL;
        }
    }
}
```
- **Recursive code generation**
- Expression nodes return the variable/temp holding their result
- Statement nodes return NULL

```c
void emit_c_code(TAC *list, const char *out_path) {
    FILE *out = fopen(out_path, "w");
    
    /* PASS A: Emit main() with top-level code */
    fprintf(out, "int main(void) {\n");
    
    int depth = 0;
    for (TAC *t = list; t; t = t->next) {
        if (t->op == TAC_FUNC_BEGIN) { depth++; continue; }
        if (t->op == TAC_FUNC_END)   { depth--; continue; }
        if (depth) continue;  /* skip code inside functions */
        
        /* Emit top-level TAC instructions as C */
        emit_one(out, t, list, 1);
    }
    
    fprintf(out, "    return 0;\n}\n\n");
    
    /* PASS B: Emit function definitions */
    depth = 0;
    for (TAC *t = list; t; t = t->next) {
        if (t->op == TAC_FUNC_BEGIN) {
            /* Emit function signature */
            fprintf(out, "%s %s(...) {\n", ...);
            depth++;
            continue;
        }
        if (t->op == TAC_FUNC_END) {
            fprintf(out, "}\n");
            depth--;
            continue;
        }
        
        if (!depth) continue;  /* skip top-level */
        
        /* Emit function body instruction */
        emit_one(out, t, list, 1);
    }
    
    fclose(out);
}
```
- **Two-pass emitter**
- **Pass A**: Outputs all top-level code inside synthesized `main()`
- **Pass B**: Outputs function definitions
- This ensures function calls in `main()` work (forward declarations emitted first)

---

## File 5: `optimizer.h` & `optimizer.c` (Code Optimization)

### What It Does:
Apply **4 sequential optimization passes** to the TAC list to eliminate redundant code and simplify expressions.

### The 4 Passes:

```c
/* Pass 1: Constant Folding */
void pass_const_fold(TAC **head) {
    for (TAC *t = *head; t; t = t->next) {
        /* If binary operation with two numeric constants... */
        if (t->op == TAC_ADD && is_numeric(t->arg1) && is_numeric(t->arg2)) {
            /* Compute the result at compile time */
            double result = atof(t->arg1) + atof(t->arg2);
            
            /* Replace with simple assignment to constant */
            t->op = TAC_ASSIGN;
            t->arg2 = NULL;
            free(t->arg1);
            t->arg1 = format_number(result);
        }
    }
}
```
- When both operands are constants, compute the result at compile-time
- Example: `_t0 = 3 + 4;` becomes `_t0 = 7;`

```c
/* Pass 2: Constant Propagation */
void pass_const_prop(TAC **head) {
    /* Build map of all constant values */
    map<string, string> const_map;  /* variable_name -> value */
    
    for (TAC *t = *head; t; t = t->next) {
        if (t->op == TAC_ASSIGN && t->is_const && is_numeric(t->arg1)) {
            const_map[t->result] = t->arg1;  /* remember this const */
        }
        
        /* Replace uses of const variables with their values */
        if (const_map.count(t->arg1))
            t->arg1 = const_map[t->arg1];
        if (const_map.count(t->arg2))
            t->arg2 = const_map[t->arg2];
    }
}
```
- Track all variable assignments to constants
- Substitute known constants wherever the variable is used
- Example: `const x = 10; sum = x + y;` becomes `sum = 10 + y;`

```c
/* Pass 3: Copy Propagation */
void pass_copy_prop(TAC **head) {
    map<string, string> copy_map;  /* from_var -> to_var */
    
    for (TAC *t = *head; t; t = t->next) {
        if (t->op == TAC_ASSIGN && !is_literal(t->arg1)) {
            copy_map[t->result] = t->arg1;  /* remember: result = source */
        }
        
        /* Replace uses of copied vars with originals */
        if (copy_map.count(t->arg1))
            t->arg1 = copy_map[t->arg1];
        if (copy_map.count(t->arg2))
            t->arg2 = copy_map[t->arg2];
    }
}
```
- Track assignments like `x = y` (copies)
- Replace subsequent uses of `x` with `y`
- Example: `sum = _t0; z = sum;` becomes `z = _t0;`

```c
/* Pass 4: Dead Code Elimination */
void pass_dead_code(TAC **head) {
    /* Mark which variables are actually used after assignment */
    set<string> used;
    
    /* Scan backward to mark all used temps */
    for (TAC *t = *head; t; t = t->next) {
        if (t->arg1) used.insert(t->arg1);
        if (t->arg2) used.insert(t->arg2);
    }
    
    /* Remove instructions whose result is never used */
    for (TAC *t = *head; t; t = t->next) {
        if (t->result && t->result[0] == '_' && !used.count(t->result)) {
            t->op = TAC_NOP;  /* remove by marking as no-op */
        }
    }
}
```
- Identify temporary variables never used after creation
- Remove instructions that compute unused values
- Example: `_t99 = 5 + 3;` (if `_t99` never read) → deleted

---

## File 6: `test_input.ts` (Example TypeScript Input)

```typescript
let x: number = 10;              /* declare variable with type */
let y: number = 20;
const PI: number = 3;            /* const cannot be reassigned */
let flag: boolean = true;        /* boolean type */
let done: boolean = false;
let ptr: number = null;          /* null literal */

let sum: number;
let _t0 = x + y;                 /* temporary (for demo) */
sum = _t0;

/* Arithmetic and printing */
console.log(sum);                /* prints: 30 */
console.log("Hello from TS2C");

/* If/else statement */
if (x < y) {
    console.log("x is smaller");
} else {
    console.log("y is smaller or equal");
}

/* While loop (0 to 4) */
let i: number = 0;
while (i < 5) {
    console.log(i);
    i = i + 1;
}

/* For loop */
let total: number = 0;
let j: number = 0;
for (j = 0; j < 10; j = j + 1) {
    total = total + j;           /* accumulate sum */
}
console.log(total);              /* prints: 45 */

/* Function declarations */
function add(a: number, b: number): number {
    return a + b;
}

function isPositive(n: number): int {
    if (n > 0)
        return 1;
    else
        return 0;
}

/* Function calls */
let result: number = add(3, 4);
console.log(result);             /* prints: 7 */

let positive: int = isPositive(5);
console.log(positive);           /* prints: 1 */

/* Logical operators */
let bothTrue: int = flag && positive;   /* true && true = 1 */
let eitherTrue: int = done || flag;     /* false || true = 1 */
let notDone: int = !done;               /* !false = 1 */

console.log(bothTrue);
console.log(eitherTrue);
console.log(notDone);
```

---

## Summary of Data Flow

```
test_input.ts
    ↓ [Flex: ts_lexer.l]
Token stream (TOK_LET, TOK_CONST, TOK_ID, TOK_NUMBER, ...)
    ↓ [Bison: parser.y]
AST (Abstract Syntax Tree)
    ↓ [CodeGen Pass 1: generate_tac()]
TAC (Three-Address Code)
    ↓ [Optimizer: 4 passes]
Optimized TAC
    ↓ [CodeGen Pass 2: emit_c_code()]
output.c (Valid C99 code)
    ↓ [GCC: gcc output.c -lm]
result.exe
    ↓ [Run: result.exe]
Program output
```

Each stage transforms the code into a simpler, more processable form until finally it becomes executable C code.
