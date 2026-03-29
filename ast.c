/* ============================================================
   ast.c  –  Abstract Syntax Tree implementation
   TS2C Compiler  |  TypeScript → C translator
   ============================================================ */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"

/* ── Internal helpers ─────────────────────────────────────── */
static char *safe_strdup(const char *s) {
    if (!s) return NULL;
    char *p = (char *)malloc(strlen(s) + 1);
    if (!p) { fprintf(stderr, "ast: out of memory\n"); exit(1); }
    strcpy(p, s);
    return p;
}

/* ── Generic node allocator ─────────────────────────────────── */
ASTNode *ast_new_node(NodeType type) {
    ASTNode *n = (ASTNode *)calloc(1, sizeof(ASTNode));
    if (!n) { fprintf(stderr, "ast: out of memory\n"); exit(1); }
    n->type = type;
    return n;
}

/* ── Literal constructors ───────────────────────────────────── */
ASTNode *ast_new_number(double value, int line) {
    ASTNode *n  = ast_new_node(NODE_NUMBER_LIT);
    n->num_val  = value;
    n->ts_type  = TYPE_NUMBER;
    n->line     = line;
    return n;
}

ASTNode *ast_new_string(const char *value, int line) {
    ASTNode *n  = ast_new_node(NODE_STRING_LIT);
    n->str_val  = safe_strdup(value);
    n->ts_type  = TYPE_STRING;
    n->line     = line;
    return n;
}

ASTNode *ast_new_bool(int value, int line) {
    ASTNode *n  = ast_new_node(NODE_BOOL_LIT);
    n->bool_val = value;
    n->ts_type  = TYPE_BOOLEAN;
    n->line     = line;
    return n;
}

ASTNode *ast_new_null(int line) {
    ASTNode *n = ast_new_node(NODE_NULL_LIT);
    n->ts_type = TYPE_NULL;
    n->line    = line;
    return n;
}

ASTNode *ast_new_ident(const char *name, int line) {
    ASTNode *n = ast_new_node(NODE_IDENTIFIER);
    n->str_val = safe_strdup(name);
    n->line    = line;
    return n;
}

/* ── Expression constructors ────────────────────────────────── */
ASTNode *ast_new_binop(const char *op, ASTNode *left,
                       ASTNode *right, int line) {
    ASTNode *n = ast_new_node(NODE_BINOP);
    strncpy(n->op, op, 3);
    n->op[3]   = '\0';
    n->left    = left;
    n->right   = right;
    n->line    = line;
    /* Basic type propagation */
    if (strcmp(op,"==")==0 || strcmp(op,"!=")==0 ||
        strcmp(op,"<")==0  || strcmp(op,">")==0  ||
        strcmp(op,"<=")==0 || strcmp(op,">=")==0 ||
        strcmp(op,"&&")==0 || strcmp(op,"||")==0 ||
        strcmp(op,"!")==0)
        n->ts_type = TYPE_BOOLEAN;
    else
        n->ts_type = left ? left->ts_type : TYPE_UNKNOWN;
    return n;
}

ASTNode *ast_new_unop(const char *op, ASTNode *operand, int line) {
    ASTNode *n = ast_new_node(NODE_UNOP);
    strncpy(n->op, op, 3);
    n->op[3]   = '\0';
    n->left    = operand;
    n->line    = line;
    n->ts_type = (strcmp(op,"!") == 0) ? TYPE_BOOLEAN
                                       : (operand ? operand->ts_type : TYPE_UNKNOWN);
    return n;
}

ASTNode *ast_new_assign(ASTNode *target, ASTNode *value, int line) {
    ASTNode *n = ast_new_node(NODE_ASSIGN);
    n->left    = target;
    n->right   = value;
    n->line    = line;
    n->ts_type = value ? value->ts_type : TYPE_UNKNOWN;
    return n;
}

/* ── Statement constructors ─────────────────────────────────── */
ASTNode *ast_new_var_decl(const char *name, TSType t,
                          ASTNode *init, int is_const, int line) {
    ASTNode *n  = ast_new_node(NODE_VAR_DECL);
    n->str_val  = safe_strdup(name);
    n->ts_type  = t;
    n->left     = init;         /* initialiser expression         */
    n->is_const = is_const;
    n->line     = line;
    return n;
}

ASTNode *ast_new_if(ASTNode *cond, ASTNode *then_b,
                    ASTNode *else_b, int line) {
    ASTNode *n = ast_new_node(NODE_IF_STMT);
    n->left    = cond;
    n->right   = then_b;
    n->extra   = else_b;
    n->line    = line;
    return n;
}

ASTNode *ast_new_while(ASTNode *cond, ASTNode *body, int line) {
    ASTNode *n = ast_new_node(NODE_WHILE_STMT);
    n->left    = cond;
    n->right   = body;
    n->line    = line;
    return n;
}

ASTNode *ast_new_for(ASTNode *init, ASTNode *cond,
                     ASTNode *update, ASTNode *body, int line) {
    ASTNode *n = ast_new_node(NODE_FOR_STMT);
    n->left    = init;
    n->right   = cond;
    n->extra   = update;
    /* body hangs off extra->next to avoid adding another field */
    if (update) update->next = body;
    else        n->extra     = body;   /* no update clause        */
    n->line    = line;
    return n;
}

ASTNode *ast_new_return(ASTNode *value, int line) {
    ASTNode *n = ast_new_node(NODE_RETURN_STMT);
    n->left    = value;
    n->line    = line;
    return n;
}

ASTNode *ast_new_block(ASTNode *stmts, int line) {
    ASTNode *n = ast_new_node(NODE_BLOCK);
    n->left    = stmts;         /* head of sibling-linked list    */
    n->line    = line;
    return n;
}

ASTNode *ast_new_func_decl(const char *name, TSType ret_type,
                           ASTNode *params, ASTNode *body, int line) {
    ASTNode *n = ast_new_node(NODE_FUNC_DECL);
    n->str_val = safe_strdup(name);
    n->ts_type = ret_type;
    n->left    = params;        /* param list (sibling chain)     */
    n->right   = body;          /* body block                     */
    n->line    = line;
    return n;
}

ASTNode *ast_new_call(const char *name, ASTNode *args, int line) {
    ASTNode *n = ast_new_node(NODE_CALL);
    n->str_val = safe_strdup(name);
    n->left    = args;          /* argument list (sibling chain)  */
    n->line    = line;
    return n;
}

ASTNode *ast_new_print(ASTNode *args, int line) {
    ASTNode *n = ast_new_node(NODE_PRINT_STMT);
    n->left    = args;
    n->line    = line;
    return n;
}

ASTNode *ast_new_param(const char *name, TSType t, int line) {
    ASTNode *n = ast_new_node(NODE_PARAM);
    n->str_val = safe_strdup(name);
    n->ts_type = t;
    n->line    = line;
    return n;
}

/* ── Type name helper ───────────────────────────────────────── */
const char *type_to_str(TSType t) {
    switch (t) {
        case TYPE_NUMBER:  return "number";
        case TYPE_BOOLEAN: return "boolean";
        case TYPE_STRING:  return "string";
        case TYPE_VOID:    return "void";
        case TYPE_NULL:    return "null";
        default:           return "unknown";
    }
}

/* ── Node type name helper (for printing) ───────────────────── */
static const char *node_name(NodeType t) {
    switch (t) {
        case NODE_PROGRAM:     return "Program";
        case NODE_VAR_DECL:    return "VarDecl";
        case NODE_FUNC_DECL:   return "FuncDecl";
        case NODE_RETURN_STMT: return "Return";
        case NODE_IF_STMT:     return "If";
        case NODE_WHILE_STMT:  return "While";
        case NODE_FOR_STMT:    return "For";
        case NODE_BLOCK:       return "Block";
        case NODE_EXPR_STMT:   return "ExprStmt";
        case NODE_PRINT_STMT:  return "Print";
        case NODE_ASSIGN:      return "Assign";
        case NODE_BINOP:       return "BinOp";
        case NODE_UNOP:        return "UnOp";
        case NODE_CALL:        return "Call";
        case NODE_IDENTIFIER:  return "Ident";
        case NODE_NUMBER_LIT:  return "Number";
        case NODE_STRING_LIT:  return "String";
        case NODE_BOOL_LIT:    return "Bool";
        case NODE_NULL_LIT:    return "Null";
        case NODE_PARAM:       return "Param";
        case NODE_ARG_LIST:    return "ArgList";
        default:               return "?";
    }
}

/* ── Pretty-print (for debugging) ──────────────────────────── */
void ast_print(ASTNode *node, int indent) {
    if (!node) return;

    for (int i = 0; i < indent; i++) printf("  ");

    printf("[%s", node_name(node->type));

    switch (node->type) {
        case NODE_NUMBER_LIT:  printf(" %.6g",  node->num_val);       break;
        case NODE_STRING_LIT:  printf(" \"%s\"", node->str_val);      break;
        case NODE_BOOL_LIT:    printf(" %s", node->bool_val?"true":"false"); break;
        case NODE_NULL_LIT:    printf(" null");                        break;
        case NODE_IDENTIFIER:  printf(" %s",  node->str_val);         break;
        case NODE_BINOP:
        case NODE_UNOP:        printf(" '%s'", node->op);              break;
        case NODE_VAR_DECL:
        case NODE_FUNC_DECL:
        case NODE_CALL:
        case NODE_PARAM:       printf(" %s",  node->str_val);         break;
        default: break;
    }

    if (node->ts_type != TYPE_UNKNOWN)
        printf(" :%s", type_to_str(node->ts_type));

    printf("]\n");

    /* Recurse into children */
    ast_print(node->left,  indent + 1);
    ast_print(node->right, indent + 1);
    ast_print(node->extra, indent + 1);

    /* Recurse into siblings */
    ast_print(node->next, indent);
}

/* ── Memory cleanup ─────────────────────────────────────────── */
void ast_free(ASTNode *node) {
    if (!node) return;
    free(node->str_val);
    ast_free(node->left);
    ast_free(node->right);
    ast_free(node->extra);
    ast_free(node->next);
    free(node);
}
