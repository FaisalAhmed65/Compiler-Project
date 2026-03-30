#ifndef AST_H
#define AST_H

/* ── Node type enumeration ─────────────────────────────────── */
typedef enum {
    /* Statements */
    NODE_PROGRAM,           /* root node – list of statements   */
    NODE_VAR_DECL,          /* let / const declaration           */
    NODE_FUNC_DECL,         /* function declaration              */
    NODE_RETURN_STMT,       /* return <expr>                     */
    NODE_IF_STMT,           /* if / else                         */
    NODE_WHILE_STMT,        /* while loop                        */
    NODE_FOR_STMT,          /* for loop                          */
    NODE_BLOCK,             /* { ... }  statement list           */
    NODE_EXPR_STMT,         /* expression used as statement      */
    NODE_PRINT_STMT,        /* console.log(...)                  */

    /* Expressions */
    NODE_ASSIGN,            /* x = expr                          */
    NODE_BINOP,             /* expr op expr                      */
    NODE_UNOP,              /* ! expr  or  -expr                 */
    NODE_CALL,              /* functionCall(args)                */
    NODE_IDENTIFIER,        /* variable name                     */
    NODE_NUMBER_LIT,        /* integer / float literal           */
    NODE_STRING_LIT,        /* "hello"                           */
    NODE_BOOL_LIT,          /* true / false                      */
    NODE_NULL_LIT,          /* null / npc                        */

    /* Parameter list node */
    NODE_PARAM,             /* single function parameter         */
    NODE_ARG_LIST           /* argument list in a call           */
} NodeType;

/* ── Type tags ─────────────────────────────────────────────── */
typedef enum {
    TYPE_UNKNOWN = 0,
    TYPE_NUMBER,
    TYPE_BOOLEAN,
    TYPE_STRING,
    TYPE_VOID,
    TYPE_NULL
} TSType;

/* ── Forward declaration ───────────────────────────────────── */
typedef struct ASTNode ASTNode;

/* ── The universal node structure ─────────────────────────── */
struct ASTNode {
    NodeType    type;           /* what kind of node this is     */
    TSType      ts_type;        /* inferred / declared TS type   */

    /* ── Leaf data ── */
    char       *str_val;        /* identifier name / string lit  */
    double      num_val;        /* numeric literal value         */
    int         bool_val;       /* 1 = true, 0 = false           */

    /* ── Operator (for BINOP / UNOP) ── */
    char        op[4];          /* "+", "-", "==", "!=", …       */

    /* ── Children  (generic child slots) ── */
    ASTNode    *left;           /* first operand / condition      */
    ASTNode    *right;          /* second operand / body          */
    ASTNode    *extra;          /* else-branch / for-update       */

    /* ── Sibling list (used for statement lists, arg lists) ── */
    ASTNode    *next;

    /* ── Variable declaration extras ── */
    int         is_const;       /* 1 if declared with 'const'    */

    /* ── Source location (for error messages) ── */
    int         line;
};

/* ── Constructor helpers ───────────────────────────────────── */
ASTNode *ast_new_node     (NodeType type);
ASTNode *ast_new_number   (double value, int line);
ASTNode *ast_new_string   (const char *value, int line);
ASTNode *ast_new_bool     (int value, int line);
ASTNode *ast_new_null     (int line);
ASTNode *ast_new_ident    (const char *name, int line);
ASTNode *ast_new_binop    (const char *op, ASTNode *left, ASTNode *right, int line);
ASTNode *ast_new_unop     (const char *op, ASTNode *operand, int line);
ASTNode *ast_new_assign   (ASTNode *target, ASTNode *value, int line);
ASTNode *ast_new_var_decl (const char *name, TSType t, ASTNode *init,
                           int is_const, int line);
ASTNode *ast_new_if       (ASTNode *cond, ASTNode *then_b,
                           ASTNode *else_b, int line);
ASTNode *ast_new_while    (ASTNode *cond, ASTNode *body, int line);
ASTNode *ast_new_for      (ASTNode *init, ASTNode *cond,
                           ASTNode *update, ASTNode *body, int line);
ASTNode *ast_new_return   (ASTNode *value, int line);
ASTNode *ast_new_block    (ASTNode *stmts, int line);
ASTNode *ast_new_func_decl(const char *name, TSType ret_type,
                           ASTNode *params, ASTNode *body, int line);
ASTNode *ast_new_call     (const char *name, ASTNode *args, int line);
ASTNode *ast_new_print    (ASTNode *args, int line);
ASTNode *ast_new_param    (const char *name, TSType t, int line);

/* ── Utility ─────────────────────────────────────────────── */
void     ast_print        (ASTNode *node, int indent);
void     ast_free         (ASTNode *node);
const char *type_to_str   (TSType t);

#endif /* AST_H */
