#ifndef CODEGEN_H
#define CODEGEN_H

/* ============================================================
   codegen.h  –  Three-Address Code (TAC) generation
   TS2C Compiler  |  TypeScript → C translator
   ============================================================ */

#include "ast.h"

/* ── TAC instruction opcodes ─────────────────────────────────── */
typedef enum {
    TAC_ASSIGN,         /* result = arg1                         */
    TAC_ADD,            /* result = arg1 + arg2                  */
    TAC_SUB,            /* result = arg1 - arg2                  */
    TAC_MUL,            /* result = arg1 * arg2                  */
    TAC_DIV,            /* result = arg1 / arg2                  */
    TAC_MOD,            /* result = arg1 % arg2                  */
    TAC_NEG,            /* result = -arg1                        */
    TAC_NOT,            /* result = !arg1                        */
    TAC_EQ,             /* result = arg1 == arg2                 */
    TAC_NEQ,            /* result = arg1 != arg2                 */
    TAC_LT,             /* result = arg1 <  arg2                 */
    TAC_GT,             /* result = arg1 >  arg2                 */
    TAC_LEQ,            /* result = arg1 <= arg2                 */
    TAC_GEQ,            /* result = arg1 >= arg2                 */
    TAC_AND,            /* result = arg1 && arg2                 */
    TAC_OR,             /* result = arg1 || arg2                 */
    TAC_LABEL,          /* label:                                */
    TAC_GOTO,           /* goto label                            */
    TAC_IF_FALSE,       /* if !arg1 goto label                   */
    TAC_PARAM,          /* param arg1  (push call argument)      */
    TAC_CALL,           /* result = call funcname, n             */
    TAC_RETURN,         /* return arg1                           */
    TAC_FUNC_BEGIN,     /* begin  funcname                       */
    TAC_FUNC_END,       /* end    funcname                       */
    TAC_DECLARE,        /* declare varname  type  [const]        */
    TAC_PRINT,          /* print arg1                            */
    TAC_NOP             /* no-operation (placeholder)            */
} TACOp;

/* ── A single TAC instruction ────────────────────────────────── */
typedef struct TAC {
    TACOp        op;
    char        *result;    /* destination variable / label      */
    char        *arg1;      /* first source operand              */
    char        *arg2;      /* second source operand (or NULL)   */
    int          ival;      /* integer metadata (e.g., arg count)*/
    int          is_const;  /* 1 if result was declared const    */
    TSType       ts_type;   /* type of result                    */
    struct TAC  *next;      /* linked-list chain                 */
} TAC;

/* ── Public API ──────────────────────────────────────────────── */

/* Walk the AST and produce a TAC instruction list */
TAC    *generate_tac  (ASTNode *root);

/* Print the TAC list to stdout */
void    print_tac     (TAC *list);

/* Free all instructions */
void    free_tac      (TAC *list);

/* Emit final C source code from the (optimised) TAC list.
   If out_path is NULL the output goes to stdout.             */
void    emit_c_code   (TAC *list, const char *out_path);

#endif /* CODEGEN_H */
