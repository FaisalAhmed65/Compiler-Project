/* ============================================================
   optimizer.c  –  TAC optimization passes
   TS2C Compiler  |  TypeScript → C translator
   ============================================================
   Passes (applied in order):
     Pass 1 – Constant Folding
     Pass 2 – Constant Propagation
     Pass 3 – Copy Propagation
     Pass 4 – Dead Code Elimination
   Each pass returns the (possibly modified) list head.
   ============================================================ */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "optimizer.h"

/* ── Utility: check if a string represents a numeric constant ── */
static int is_numeric_const(const char *s) {
    if (!s) return 0;
    const char *p = s;
    if (*p == '-') p++;
    if (!*p) return 0;
    int has_dot = 0;
    while (*p) {
        if (*p == '.') { if (has_dot) return 0; has_dot = 1; }
        else if (*p < '0' || *p > '9') return 0;
        p++;
    }
    return 1;
}

static double str_to_num(const char *s) { return atof(s); }

static char *num_to_str_opt(double v) {
    char buf[64];
    if (v == (long long)v) snprintf(buf, sizeof(buf), "%lld", (long long)v);
    else                   snprintf(buf, sizeof(buf), "%.10g", v);
    return strdup(buf);
}

static void replace_str(char **field, const char *new_val) {
    free(*field);
    *field = new_val ? strdup(new_val) : NULL;
}

/* ═══════════════════════════════════════════════════════════════
   Pass 1 – Constant Folding
   For every arithmetic/relational binary instruction where both
   operands are numeric literals, replace the instruction with a
   simple assignment of the computed value.
   ═══════════════════════════════════════════════════════════════ */
static TAC *pass_const_fold(TAC *list) {
    for (TAC *t = list; t; t = t->next) {
        if (!is_numeric_const(t->arg1) || !is_numeric_const(t->arg2))
            continue;

        double a = str_to_num(t->arg1);
        double b = str_to_num(t->arg2);
        double r = 0;
        int    is_bool = 0;

        switch (t->op) {
            case TAC_ADD: r = a + b; break;
            case TAC_SUB: r = a - b; break;
            case TAC_MUL: r = a * b; break;
            case TAC_DIV:
                if (b == 0.0) continue;   /* avoid div-by-zero fold */
                r = a / b; break;
            case TAC_MOD:
                if ((long long)b == 0) continue;
                r = (double)((long long)a % (long long)b); break;
            case TAC_EQ:  r = (a == b); is_bool = 1; break;
            case TAC_NEQ: r = (a != b); is_bool = 1; break;
            case TAC_LT:  r = (a <  b); is_bool = 1; break;
            case TAC_GT:  r = (a >  b); is_bool = 1; break;
            case TAC_LEQ: r = (a <= b); is_bool = 1; break;
            case TAC_GEQ: r = (a >= b); is_bool = 1; break;
            default: continue;
        }

        /* Rewrite as TAC_ASSIGN with constant value */
        char *val = num_to_str_opt(r);
        replace_str(&t->arg1, val);
        free(val);
        replace_str(&t->arg2, NULL);
        t->op = TAC_ASSIGN;
        if (is_bool) t->ts_type = TYPE_BOOLEAN;

        printf("[Optimizer] Constant fold: %s\n", t->result);
    }
    return list;
}

/* ═══════════════════════════════════════════════════════════════
   Pass 2 – Constant Propagation
   If we see  x = <const>  and later  y = ... x ...  (with no
   intervening assignment to x), replace x with <const>.
   ═══════════════════════════════════════════════════════════════ */
#define MAX_CONSTS 256
static struct { char name[64]; char value[64]; } const_map[MAX_CONSTS];
static int const_map_size = 0;

static void const_map_clear(void)       { const_map_size = 0; }
static const char *const_map_get(const char *name) {
    for (int i = 0; i < const_map_size; i++)
        if (strcmp(const_map[i].name, name) == 0)
            return const_map[i].value;
    return NULL;
}
static void const_map_set(const char *name, const char *value) {
    for (int i = 0; i < const_map_size; i++)
        if (strcmp(const_map[i].name, name) == 0) {
            strncpy(const_map[i].value, value, 63);
            return;
        }
    if (const_map_size >= MAX_CONSTS) return;
    strncpy(const_map[const_map_size].name,  name,  63);
    strncpy(const_map[const_map_size].value, value, 63);
    const_map_size++;
}
static void const_map_invalidate(const char *name) {
    for (int i = 0; i < const_map_size; i++)
        if (strcmp(const_map[i].name, name) == 0) {
            /* shift remaining entries down */
            memmove(&const_map[i], &const_map[i+1],
                    (const_map_size - i - 1) * sizeof(const_map[0]));
            const_map_size--;
            return;
        }
}

static void subst_if_const(char **field) {
    if (!*field) return;
    const char *cv = const_map_get(*field);
    if (cv) {
        printf("[Optimizer] Const propagate: %s → %s\n", *field, cv);
        replace_str(field, cv);
    }
}

static TAC *pass_const_prop(TAC *list) {
    const_map_clear();
    for (TAC *t = list; t; t = t->next) {
        /* Substitute known constants into arguments */
        subst_if_const(&t->arg1);
        subst_if_const(&t->arg2);

        /* Record new constant if this is an ASSIGN of a literal */
        if (t->op == TAC_ASSIGN && t->result && is_numeric_const(t->arg1)) {
            const_map_set(t->result, t->arg1);
        } else if (t->result) {
            /* Any other assignment invalidates known value */
            const_map_invalidate(t->result);
        }

        /* Function boundaries: flush map */
        if (t->op == TAC_FUNC_BEGIN || t->op == TAC_FUNC_END ||
            t->op == TAC_LABEL)
            const_map_clear();
    }
    return list;
}

/* ═══════════════════════════════════════════════════════════════
   Pass 3 – Copy Propagation
   If  x = y  (y is not a literal) and x is used before y changes,
   replace uses of x with y.
   ═══════════════════════════════════════════════════════════════ */
static TAC *pass_copy_prop(TAC *list) {
    /* Simple single-pass: maintain copy map */
    char copy_src[MAX_CONSTS][64];
    char copy_dst[MAX_CONSTS][64];
    int  copy_count = 0;

    #define copy_get(n) ({ \
        const char *__r = NULL; \
        for(int __i=0;__i<copy_count;__i++) \
            if(strcmp(copy_dst[__i],(n))==0){__r=copy_src[__i];break;} \
        __r; })

    for (TAC *t = list; t; t = t->next) {
        /* Substitute copies in args */
        if (t->arg1) {
            for (int i = 0; i < copy_count; i++)
                if (strcmp(copy_dst[i], t->arg1) == 0 && !is_numeric_const(t->arg1)) {
                    printf("[Optimizer] Copy propagate: %s → %s\n",
                           t->arg1, copy_src[i]);
                    replace_str(&t->arg1, copy_src[i]);
                    break;
                }
        }
        if (t->arg2) {
            for (int i = 0; i < copy_count; i++)
                if (strcmp(copy_dst[i], t->arg2) == 0 && !is_numeric_const(t->arg2)) {
                    replace_str(&t->arg2, copy_src[i]);
                    break;
                }
        }

        /* Record or invalidate copies */
        if (t->op == TAC_ASSIGN && t->result && t->arg1 &&
            !is_numeric_const(t->arg1)) {
            if (copy_count < MAX_CONSTS) {
                strncpy(copy_dst[copy_count], t->result, 63);
                strncpy(copy_src[copy_count], t->arg1, 63);
                copy_count++;
            }
        } else if (t->result) {
            /* Invalidate any copy whose src or dst matches result */
            for (int i = 0; i < copy_count; ) {
                if (strcmp(copy_dst[i], t->result) == 0 ||
                    strcmp(copy_src[i], t->result) == 0) {
                    memmove(&copy_dst[i], &copy_dst[i+1],
                            (copy_count-i-1)*64);
                    memmove(&copy_src[i], &copy_src[i+1],
                            (copy_count-i-1)*64);
                    copy_count--;
                } else i++;
            }
        }

        if (t->op == TAC_FUNC_BEGIN || t->op == TAC_FUNC_END ||
            t->op == TAC_LABEL)
            copy_count = 0;
    }
    return list;
}

/* ═══════════════════════════════════════════════════════════════
   Pass 4 – Dead Code Elimination
   Remove TAC_ASSIGN / arithmetic instructions whose result
   (_tN temporaries) is never used as arg1 or arg2 anywhere
   after their definition.
   ═══════════════════════════════════════════════════════════════ */

/* Check if 'name' appears as arg1/arg2 anywhere after 'start' */
static int used_after(TAC *start, const char *name) {
    if (!name) return 0;
    for (TAC *t = start; t; t = t->next) {
        if (t->arg1   && strcmp(t->arg1,   name) == 0) return 1;
        if (t->arg2   && strcmp(t->arg2,   name) == 0) return 1;
        if (t->result && strcmp(t->result, name) == 0 &&
            t->op != TAC_ASSIGN && t->op != TAC_ADD  &&
            t->op != TAC_SUB   && t->op != TAC_MUL  &&
            t->op != TAC_DIV   && t->op != TAC_MOD  &&
            t->op != TAC_NEG   && t->op != TAC_NOT  &&
            t->op != TAC_EQ    && t->op != TAC_NEQ  &&
            t->op != TAC_LT    && t->op != TAC_GT   &&
            t->op != TAC_LEQ   && t->op != TAC_GEQ  &&
            t->op != TAC_AND   && t->op != TAC_OR)
            return 1;  /* used as destination in non-arithmetic = live */
    }
    return 0;
}

static TAC *pass_dead_code(TAC *list) {
    TAC *prev = NULL, *t = list;
    while (t) {
        /* Only eliminate temp variables (_tN) */
        int is_temp = t->result && t->result[0] == '_' && t->result[1] == 't';
        int removable = (t->op == TAC_ASSIGN || t->op == TAC_ADD ||
                         t->op == TAC_SUB    || t->op == TAC_MUL ||
                         t->op == TAC_DIV    || t->op == TAC_MOD ||
                         t->op == TAC_NEG    || t->op == TAC_NOT ||
                         t->op == TAC_EQ     || t->op == TAC_NEQ ||
                         t->op == TAC_LT     || t->op == TAC_GT  ||
                         t->op == TAC_LEQ    || t->op == TAC_GEQ ||
                         t->op == TAC_AND    || t->op == TAC_OR);

        if (is_temp && removable && !used_after(t->next, t->result)) {
            printf("[Optimizer] Dead code remove: %s\n", t->result);
            TAC *dead = t;
            t = t->next;
            if (prev) prev->next = t;
            else       list      = t;
            dead->next = NULL;
            free(dead->result); free(dead->arg1); free(dead->arg2);
            free(dead);
        } else {
            prev = t;
            t = t->next;
        }
    }
    return list;
}

/* ═══════════════════════════════════════════════════════════════
   Public: optimize_tac  –  run all passes
   ═══════════════════════════════════════════════════════════════ */
TAC *optimize_tac(TAC *list) {
    printf("[Optimizer] Running constant folding...\n");
    list = pass_const_fold(list);

    printf("[Optimizer] Running constant propagation...\n");
    list = pass_const_prop(list);

    printf("[Optimizer] Running copy propagation...\n");
    list = pass_copy_prop(list);

    printf("[Optimizer] Running dead code elimination...\n");
    list = pass_dead_code(list);

    printf("[Optimizer] Done.\n");
    return list;
}
