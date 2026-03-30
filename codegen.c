#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"
#include "codegen.h"

/* ── Counter state ───────────────────────────────────────────── */
static int temp_count  = 0;
static int label_count = 0;

/* ── TAC list head / tail ─────────────────────────────────────── */
static TAC *tac_head = NULL;
static TAC *tac_tail = NULL;

/* ═══════════════════════════════════════════════════════════════
   Internal helpers
   ═══════════════════════════════════════════════════════════════ */

static char *new_temp(void) {
    char buf[32];
    snprintf(buf, sizeof(buf), "_t%d", temp_count++);
    return strdup(buf);
}

static char *new_label(void) {
    char buf[32];
    snprintf(buf, sizeof(buf), "L%d", label_count++);
    return strdup(buf);
}

static char *safe_dup(const char *s) {
    return s ? strdup(s) : NULL;
}

/* Append one instruction to the tail of the list */
static TAC *emit(TACOp op, const char *result, const char *arg1,
                 const char *arg2) {
    TAC *t = (TAC *)calloc(1, sizeof(TAC));
    if (!t) { fprintf(stderr, "codegen: out of memory\n"); exit(1); }
    t->op     = op;
    t->result = safe_dup(result);
    t->arg1   = safe_dup(arg1);
    t->arg2   = safe_dup(arg2);

    if (!tac_head) { tac_head = tac_tail = t; }
    else           { tac_tail->next = t; tac_tail = t; }
    return t;
}

/* Translate a TS type to a C type string */
static const char *ts_type_to_c(TSType t) {
    switch (t) {
        case TYPE_NUMBER:  return "double";
        case TYPE_BOOLEAN: return "int";
        case TYPE_STRING:  return "char*";
        case TYPE_VOID:    return "void";
        case TYPE_NULL:    return "void*";
        default:           return "double"; /* default to number  */
    }
}

/* ═══════════════════════════════════════════════════════════════
   Recursive TAC generation
   Returns the name of the "result" variable (or NULL for stmts)
   ═══════════════════════════════════════════════════════════════ */
static char *gen_node(ASTNode *node);

/* ── generate BINOP ──────────────────────────────────────────── */
static char *gen_binop(ASTNode *n) {
    char *l = gen_node(n->left);
    char *r = gen_node(n->right);
    char *t = new_temp();
    TACOp op;
    if      (strcmp(n->op,"+" )==0) op = TAC_ADD;
    else if (strcmp(n->op,"-" )==0) op = TAC_SUB;
    else if (strcmp(n->op,"*" )==0) op = TAC_MUL;
    else if (strcmp(n->op,"/" )==0) op = TAC_DIV;
    else if (strcmp(n->op,"%" )==0) op = TAC_MOD;
    else if (strcmp(n->op,"==")==0) op = TAC_EQ;
    else if (strcmp(n->op,"!=")==0) op = TAC_NEQ;
    else if (strcmp(n->op,"<" )==0) op = TAC_LT;
    else if (strcmp(n->op,">" )==0) op = TAC_GT;
    else if (strcmp(n->op,"<=")==0) op = TAC_LEQ;
    else if (strcmp(n->op,">=")==0) op = TAC_GEQ;
    else if (strcmp(n->op,"&&")==0) op = TAC_AND;
    else if (strcmp(n->op,"||")==0) op = TAC_OR;
    else                             op = TAC_ADD; /* fallback    */
    TAC *ins = emit(op, t, l, r);
    ins->ts_type = n->ts_type;
    free(l); free(r);
    return t;
}

/* ── generate a numeric leaf as a string ─────────────────────── */
static char *num_to_str(double v) {
    char buf[64];
    if (v == (int)v) snprintf(buf, sizeof(buf), "%d",   (int)v);
    else             snprintf(buf, sizeof(buf), "%.6g", v);
    return strdup(buf);
}

/* ── main recursive generator ────────────────────────────────── */
static char *gen_node(ASTNode *node) {
    if (!node) return NULL;

    switch (node->type) {

    /* ── Literals ── */
    case NODE_NUMBER_LIT:
        return num_to_str(node->num_val);

    case NODE_STRING_LIT: {
        /* Return as a quoted string literal token */
        char *s = (char *)malloc(strlen(node->str_val) + 3);
        sprintf(s, "\"%s\"", node->str_val);
        return s;
    }

    case NODE_BOOL_LIT:
        return strdup(node->bool_val ? "1" : "0");

    case NODE_NULL_LIT:
        return strdup("0");   /* TS null maps to 0 in C (no pointer types) */

    /* ── Identifier ── */
    case NODE_IDENTIFIER:
        return safe_dup(node->str_val);

    /* ── Binary / Unary ── */
    case NODE_BINOP:
        return gen_binop(node);

    case NODE_UNOP: {
        char *operand = gen_node(node->left);
        char *t = new_temp();
        TACOp op = (strcmp(node->op,"!") == 0) ? TAC_NOT : TAC_NEG;
        TAC *ins = emit(op, t, operand, NULL);
        ins->ts_type = node->ts_type;
        free(operand);
        return t;
    }

    /* ── Assignment ── */
    case NODE_ASSIGN: {
        char *val = gen_node(node->right);
        char *dst = safe_dup(node->left->str_val);
        emit(TAC_ASSIGN, dst, val, NULL);
        free(val);
        return dst;
    }

    /* ── Variable declaration ── */
    case NODE_VAR_DECL: {
        TAC *decl = emit(TAC_DECLARE, node->str_val, NULL, NULL);
        decl->ts_type  = node->ts_type;
        decl->is_const = node->is_const;
        if (node->left) {
            char *init = gen_node(node->left);
            TAC *asgn = emit(TAC_ASSIGN, node->str_val, init, NULL);
            asgn->ts_type = node->ts_type;
            free(init);
        }
        return NULL;
    }

    /* ── If statement ── */
    case NODE_IF_STMT: {
        char *cond   = gen_node(node->left);
        char *l_else = new_label();
        char *l_end  = new_label();

        emit(TAC_IF_FALSE, l_else, cond, NULL);
        free(cond);

        gen_node(node->right);                  /* then block     */

        if (node->extra) {
            emit(TAC_GOTO, l_end, NULL, NULL);
            TAC *lbl = emit(TAC_LABEL, l_else, NULL, NULL);
            (void)lbl;
            gen_node(node->extra);              /* else block     */
            emit(TAC_LABEL, l_end, NULL, NULL);
        } else {
            emit(TAC_LABEL, l_else, NULL, NULL);
        }

        free(l_else); free(l_end);
        return NULL;
    }

    /* ── While statement ── */
    case NODE_WHILE_STMT: {
        char *l_start = new_label();
        char *l_end   = new_label();

        emit(TAC_LABEL, l_start, NULL, NULL);
        char *cond = gen_node(node->left);
        emit(TAC_IF_FALSE, l_end, cond, NULL);
        free(cond);

        gen_node(node->right);                  /* body           */
        emit(TAC_GOTO, l_start, NULL, NULL);
        emit(TAC_LABEL, l_end, NULL, NULL);

        free(l_start); free(l_end);
        return NULL;
    }

    /* ── For statement ── */
    case NODE_FOR_STMT: {
        char *l_start = new_label();
        char *l_end   = new_label();

        gen_node(node->left);                   /* init           */
        emit(TAC_LABEL, l_start, NULL, NULL);

        char *cond = gen_node(node->right);
        emit(TAC_IF_FALSE, l_end, cond, NULL);
        free(cond);

        /* body is node->extra->next if update exists */
        ASTNode *body   = NULL;
        ASTNode *update = NULL;
        if (node->extra) {
            /* extra is update clause, extra->next is body */
            update = node->extra;
            body   = node->extra->next;
        }
        gen_node(body);
        gen_node(update);

        emit(TAC_GOTO, l_start, NULL, NULL);
        emit(TAC_LABEL, l_end, NULL, NULL);

        free(l_start); free(l_end);
        return NULL;
    }

    /* ── Return ── */
    case NODE_RETURN_STMT: {
        char *val = gen_node(node->left);
        emit(TAC_RETURN, NULL, val, NULL);
        free(val);
        return NULL;
    }

    /* ── Block ── */
    case NODE_BLOCK: {
        ASTNode *s = node->left;
        while (s) { gen_node(s); s = s->next; }
        return NULL;
    }

    /* ── Expression statement ── */
    case NODE_EXPR_STMT: {
        char *v = gen_node(node->left);
        free(v);
        return NULL;
    }

    /* ── console.log ── */
    case NODE_PRINT_STMT: {
        ASTNode *arg = node->left;
        while (arg) {
            char *v = gen_node(arg);
            emit(TAC_PRINT, NULL, v, NULL);
            free(v);
            arg = arg->next;
        }
        return NULL;
    }

    /* ── Function declaration ── */
    case NODE_FUNC_DECL: {
        TAC *begin = emit(TAC_FUNC_BEGIN, node->str_val, NULL, NULL);
        begin->ts_type = node->ts_type;

        /* Parameters */
        ASTNode *p = node->left;
        while (p) {
            TAC *par = emit(TAC_PARAM, p->str_val, NULL, NULL);
            par->ts_type = p->ts_type;
            p = p->next;
        }

        gen_node(node->right);                  /* body           */
        emit(TAC_FUNC_END, node->str_val, NULL, NULL);
        return NULL;
    }

    /* ── Function call ── */
    case NODE_CALL: {
        /* Push arguments */
        int argc = 0;
        ASTNode *a = node->left;
        while (a) {
            char *av = gen_node(a);
            emit(TAC_PARAM, av, NULL, NULL);
            free(av);
            argc++;
            a = a->next;
        }
        char *t = new_temp();
        char cnt[16]; snprintf(cnt, sizeof(cnt), "%d", argc);
        TAC *call = emit(TAC_CALL, t, node->str_val, cnt);
        call->ts_type = node->ts_type;
        return t;
    }

    /* ── Program root ── */
    case NODE_PROGRAM: {
        ASTNode *s = node->left;
        while (s) { gen_node(s); s = s->next; }
        return NULL;
    }

    default:
        return NULL;
    }
}

/* ═══════════════════════════════════════════════════════════════
   Public: generate_tac
   ═══════════════════════════════════════════════════════════════ */
TAC *generate_tac(ASTNode *root) {
    temp_count  = 0;
    label_count = 0;
    tac_head    = NULL;
    tac_tail    = NULL;
    gen_node(root);
    return tac_head;
}

/* ═══════════════════════════════════════════════════════════════
   Public: print_tac
   ═══════════════════════════════════════════════════════════════ */
static const char *op_name(TACOp op) {
    switch (op) {
        case TAC_ASSIGN:    return "=";
        case TAC_ADD:       return "+";
        case TAC_SUB:       return "-";
        case TAC_MUL:       return "*";
        case TAC_DIV:       return "/";
        case TAC_MOD:       return "%";
        case TAC_NEG:       return "neg";
        case TAC_NOT:       return "not";
        case TAC_EQ:        return "==";
        case TAC_NEQ:       return "!=";
        case TAC_LT:        return "<";
        case TAC_GT:        return ">";
        case TAC_LEQ:       return "<=";
        case TAC_GEQ:       return ">=";
        case TAC_AND:       return "&&";
        case TAC_OR:        return "||";
        case TAC_LABEL:     return "label";
        case TAC_GOTO:      return "goto";
        case TAC_IF_FALSE:  return "ifFalse";
        case TAC_PARAM:     return "param";
        case TAC_CALL:      return "call";
        case TAC_RETURN:    return "return";
        case TAC_FUNC_BEGIN:return "func_begin";
        case TAC_FUNC_END:  return "func_end";
        case TAC_DECLARE:   return "declare";
        case TAC_PRINT:     return "print";
        case TAC_NOP:       return "nop";
        default:            return "?";
    }
}

void print_tac(TAC *list) {
    int idx = 0;
    for (TAC *t = list; t; t = t->next, idx++) {
        printf("%4d: ", idx);
        switch (t->op) {
            case TAC_LABEL:
                printf("%s:\n", t->result);
                break;
            case TAC_GOTO:
                printf("  goto %s\n", t->result);
                break;
            case TAC_IF_FALSE:
                printf("  ifFalse %s goto %s\n", t->arg1, t->result);
                break;
            case TAC_FUNC_BEGIN:
                printf("--- begin %s ---\n", t->result);
                break;
            case TAC_FUNC_END:
                printf("--- end %s ---\n", t->result);
                break;
            case TAC_DECLARE:
                printf("  declare %s %s%s\n",
                       ts_type_to_c(t->ts_type), t->result,
                       t->is_const ? " (const)" : "");
                break;
            case TAC_PARAM:
                printf("  param %s\n", t->result ? t->result : t->arg1);
                break;
            case TAC_CALL:
                printf("  %s = call %s, %s\n",
                       t->result, t->arg1, t->arg2);
                break;
            case TAC_RETURN:
                printf("  return %s\n", t->arg1 ? t->arg1 : "");
                break;
            case TAC_PRINT:
                printf("  print %s\n", t->arg1);
                break;
            case TAC_ASSIGN:
                printf("  %s = %s\n", t->result, t->arg1);
                break;
            case TAC_NEG:
                printf("  %s = neg %s\n", t->result, t->arg1);
                break;
            case TAC_NOT:
                printf("  %s = not %s\n", t->result, t->arg1);
                break;
            case TAC_NOP:
                printf("  nop\n");
                break;
            default:
                printf("  %s = %s %s %s\n",
                       t->result ? t->result : "_",
                       t->arg1   ? t->arg1   : "_",
                       op_name(t->op),
                       t->arg2   ? t->arg2   : "_");
                break;
        }
    }
}

/* ═══════════════════════════════════════════════════════════════
   Public: free_tac
   ═══════════════════════════════════════════════════════════════ */
void free_tac(TAC *list) {
    while (list) {
        TAC *next = list->next;
        free(list->result);
        free(list->arg1);
        free(list->arg2);
        free(list);
        list = next;
    }
}

/* ═══════════════════════════════════════════════════════════════
   Pass 2: emit_c_code
   Converts the (optimised) TAC list into a C source file.

   Strategy (two-pass output):
     1. Forward declarations for all user-defined functions.
     2. Synthesised main() containing every top-level TAC
        instruction (those NOT inside any FUNC_BEGIN/FUNC_END).
     3. Function definitions in the order they appear in TAC.

   This guarantees that calls such as result=add(3,4) which follow
   function definitions in the source are still emitted inside
   main() rather than in global scope.
   ═══════════════════════════════════════════════════════════════ */

/* Symbol table for declared variables (needed for C type declarations) */
#define MAX_VARS 512
static struct { char name[64]; const char *ctype; int is_const; } var_table[MAX_VARS];
static int var_count = 0;

static void var_table_reset(void) { var_count = 0; }

static void var_table_add(const char *name, const char *ctype, int is_const) {
    if (var_count >= MAX_VARS) return;
    strncpy(var_table[var_count].name, name, 63);
    var_table[var_count].ctype    = ctype;
    var_table[var_count].is_const = is_const;
    var_count++;
}

static const char *var_table_ctype(const char *name) {
    for (int i = 0; i < var_count; i++)
        if (strcmp(var_table[i].name, name) == 0)
            return var_table[i].ctype;
    return "double"; /* default */
}

static int var_declared(const char *name) {
    for (int i = 0; i < var_count; i++)
        if (strcmp(var_table[i].name, name) == 0)
            return 1;
    return 0;
}

/* Check if a string is a literal (starts with digit, quote, or minus) */
static int is_literal(const char *s) {
    if (!s) return 0;
    return (s[0]=='"' || s[0]=='\'' ||
            (s[0]=='-' && (s[1]>='0'&&s[1]<='9')) ||
            (s[0]>='0' && s[0]<='9') ||
            strcmp(s,"NULL")==0 ||
            strcmp(s,"1")==0 ||
            strcmp(s,"0")==0);
}

/* ── emit_one: emit a single TAC instruction as C (non-function ops) ── */
static void emit_one(FILE *out, TAC *t, TAC *list, int indent) {
    /* indentation helper */
#define IND() do { for (int _i = 0; _i < indent; _i++) fprintf(out, "    "); } while(0)

    switch (t->op) {

    /* TAC_DECLARE is intentionally NOT handled here.
       The caller loop handles it directly so it can merge with
       the immediately following ASSIGN (needed for const). */
    case TAC_DECLARE:
        break;

    case TAC_ASSIGN: {
        if (t->result && t->result[0] == '_' && !var_declared(t->result)) {
            var_table_add(t->result, "double", 0);
            IND(); fprintf(out, "double %s = %s;\n", t->result, t->arg1);
        } else {
            IND(); fprintf(out, "%s = %s;\n", t->result, t->arg1);
        }
        break;
    }

    case TAC_ADD: case TAC_SUB: case TAC_MUL: case TAC_DIV: case TAC_MOD:
    case TAC_EQ:  case TAC_NEQ: case TAC_LT:  case TAC_GT:
    case TAC_LEQ: case TAC_GEQ: case TAC_AND: case TAC_OR: {
        static const char *cop[] = {
            "=","+","-","*","/","%","neg","!",
            "==","!=","<",">","<=",">=","&&","||"
        };
        const char *op_str = cop[t->op - TAC_ASSIGN];
        if (!var_declared(t->result)) {
            var_table_add(t->result, "double", 0);
            IND(); fprintf(out, "double %s = %s %s %s;\n",
                t->result, t->arg1, op_str, t->arg2);
        } else {
            IND(); fprintf(out, "%s = %s %s %s;\n",
                t->result, t->arg1, op_str, t->arg2);
        }
        break;
    }

    case TAC_NEG: {
        if (!var_declared(t->result)) {
            var_table_add(t->result, "double", 0);
            IND(); fprintf(out, "double %s = -%s;\n", t->result, t->arg1);
        } else {
            IND(); fprintf(out, "%s = -%s;\n", t->result, t->arg1);
        }
        break;
    }

    case TAC_NOT: {
        if (!var_declared(t->result)) {
            var_table_add(t->result, "int", 0);
            IND(); fprintf(out, "int %s = !%s;\n", t->result, t->arg1);
        } else {
            IND(); fprintf(out, "%s = !%s;\n", t->result, t->arg1);
        }
        break;
    }

    case TAC_LABEL:
        fprintf(out, "%s:;\n", t->result);
        break;

    case TAC_GOTO:
        IND(); fprintf(out, "goto %s;\n", t->result);
        break;

    case TAC_IF_FALSE:
        IND(); fprintf(out, "if (!(%s)) goto %s;\n", t->arg1, t->result);
        break;

    case TAC_RETURN:
        IND();
        if (t->arg1) fprintf(out, "return %s;\n", t->arg1);
        else         fprintf(out, "return;\n");
        break;

    case TAC_PRINT:
        IND();
        if (t->arg1 && t->arg1[0] == '"')
            fprintf(out, "printf(\"%%s\\n\", %s);\n", t->arg1);
        else
            fprintf(out, "printf(\"%%g\\n\", (double)(%s));\n", t->arg1);
        break;

    case TAC_PARAM:
        break;  /* handled by TAC_CALL (backward scan) / FUNC_BEGIN (signature) */

    case TAC_CALL: {
        IND();
        if (t->result && !is_literal(t->result)) {
            if (!var_declared(t->result)) {
                var_table_add(t->result, "double", 0);
                fprintf(out, "double %s = %s(", t->result, t->arg1);
            } else {
                fprintf(out, "%s = %s(", t->result, t->arg1);
            }
        } else {
            fprintf(out, "%s(", t->arg1);
        }
        /* Collect call-site arguments: last n_args PARAMs before this CALL */
        int n_args = t->arg2 ? atoi(t->arg2) : 0;
        TAC *params_arr[64];
        int  param_cnt = 0;
        for (TAC *p = list; p && p != t; p = p->next)
            if (p->op == TAC_PARAM && param_cnt < 64)
                params_arr[param_cnt++] = p;
        int start = param_cnt - n_args;
        for (int i = start; i < param_cnt; i++) {
            if (i > start) fprintf(out, ", ");
            const char *pval = params_arr[i]->result
                             ? params_arr[i]->result
                             : params_arr[i]->arg1;
            fprintf(out, "%s", pval);
        }
        fprintf(out, ");\n");
        break;
    }

    case TAC_NOP:
    default:
        break;
    }
#undef IND
}

/* ── emit_declare: emit a DECLARE (with optional merged ASSIGN).
   Returns the TAC node that should become the new loop cursor
   (either t itself, or t->next if the ASSIGN was consumed). ── */
static TAC *emit_declare(FILE *out, TAC *t, int indent) {
    const char *ctype = ts_type_to_c(t->ts_type);
    for (int _i = 0; _i < indent; _i++) fprintf(out, "    ");
    if (t->is_const) fprintf(out, "const %s %s", ctype, t->result);
    else             fprintf(out, "%s %s", ctype, t->result);

    /* Merge with immediately following ASSIGN for the same variable.
       This is required for 'const' variables (cannot assign after declaration). */
    if (t->next && t->next->op == TAC_ASSIGN &&
        t->next->result && strcmp(t->next->result, t->result) == 0) {
        fprintf(out, " = %s;\n", t->next->arg1);
        return t->next;  /* caller should advance past the consumed ASSIGN */
    }
    fprintf(out, ";\n");
    return t;
}

void emit_c_code(TAC *list, const char *out_path) {
    FILE *out = out_path ? fopen(out_path, "w") : stdout;
    if (!out) {
        fprintf(stderr, "Cannot open output file: %s\n", out_path);
        return;
    }

    /* ── Headers ── */
    fprintf(out, "/* Generated by TS2C compiler */\n");
    fprintf(out, "#include <stdio.h>\n");
    fprintf(out, "#include <string.h>\n\n");

    /* ── Forward declarations for every user-defined function ── */
    for (TAC *t = list; t; t = t->next) {
        if (t->op != TAC_FUNC_BEGIN) continue;
        const char *ret = ts_type_to_c(t->ts_type);
        fprintf(out, "%s %s(", ret, t->result);
        int first = 1;
        for (TAC *p = t->next; p && p->op == TAC_PARAM && p->result; p = p->next) {
            if (!first) fprintf(out, ", ");
            fprintf(out, "%s %s", ts_type_to_c(p->ts_type), p->result);
            first = 0;
        }
        fprintf(out, ");\n");
    }
    fprintf(out, "\n");

    /* ── Check whether the source already defines main() ── */
    int has_explicit_main = 0;
    for (TAC *t = list; t; t = t->next)
        if (t->op == TAC_FUNC_BEGIN && t->result && strcmp(t->result, "main") == 0)
            { has_explicit_main = 1; break; }

    /* ════════════════════════════════════════════════════════
       PASS A – Emit synthesised main() with all top-level code.
       Top-level = every TAC instruction NOT between a
       FUNC_BEGIN / FUNC_END pair.
       ════════════════════════════════════════════════════════ */
    if (!has_explicit_main) {
        /* Build var_table for top-level scope */
        var_table_reset();
        {
            int depth = 0;
            for (TAC *t = list; t; t = t->next) {
                if (t->op == TAC_FUNC_BEGIN) { depth++; continue; }
                if (t->op == TAC_FUNC_END)   { depth--; continue; }
                if (!depth && t->op == TAC_DECLARE)
                    var_table_add(t->result, ts_type_to_c(t->ts_type), t->is_const);
            }
        }

        fprintf(out, "int main(void) {\n");

        int depth = 0;
        for (TAC *t = list; t; t = t->next) {
            if (t->op == TAC_FUNC_BEGIN) { depth++; continue; }
            if (t->op == TAC_FUNC_END)   { depth--; continue; }
            if (depth) continue;   /* inside a function – skip in this pass */

            /* Handle DECLARE: may merge with immediately-following ASSIGN
               (required for 'const' variables) */
            if (t->op == TAC_DECLARE) {
                t = emit_declare(out, t, 1);
                continue;
            }

            emit_one(out, t, list, 1);
        }

        fprintf(out, "    return 0;\n}\n\n");
    }

    /* ════════════════════════════════════════════════════════
       PASS B – Emit every user-defined function definition.
       ════════════════════════════════════════════════════════ */
    {
        int depth = 0;
        for (TAC *t = list; t; t = t->next) {

            if (t->op == TAC_FUNC_BEGIN) {
                depth++;
                /* Build var_table for this function's scope */
                var_table_reset();
                for (TAC *q = t->next; q && q->op != TAC_FUNC_END; q = q->next)
                    if (q->op == TAC_DECLARE)
                        var_table_add(q->result, ts_type_to_c(q->ts_type), q->is_const);

                /* Emit function header */
                const char *ret = ts_type_to_c(t->ts_type);
                fprintf(out, "%s %s(", ret, t->result);
                int first = 1;
                for (TAC *p = t->next; p && p->op == TAC_PARAM && p->result; p = p->next) {
                    if (!first) fprintf(out, ", ");
                    fprintf(out, "%s %s", ts_type_to_c(p->ts_type), p->result);
                    first = 0;
                }
                fprintf(out, ") {\n");
                continue;
            }

            if (t->op == TAC_FUNC_END) {
                depth--;
                fprintf(out, "}\n\n");
                continue;
            }

            if (!depth) continue;  /* top-level instruction – skip in this pass */

            /* Function parameter PARAMs (right after FUNC_BEGIN) are already
               in the signature; TAC_PARAM is a no-op in emit_one. */

            /* DECLARE inside function body: merge with adjacent ASSIGN */
            if (t->op == TAC_DECLARE) {
                t = emit_declare(out, t, 1);
                continue;
            }

            emit_one(out, t, list, 1);
        }
    }

    if (out_path) fclose(out);
}
