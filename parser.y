/* ============================================================
   parser.y  –  Bison grammar for the TS2C compiler
   TypeScript → C translator
   ============================================================
   Builds an Abstract Syntax Tree (AST) from the token stream
   produced by ts_lexer.l.  After parsing, the AST root is
   passed to the code-generation pipeline.
   ============================================================ */

%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"
#include "codegen.h"
#include "optimizer.h"

/* Supplied by the lexer */
extern int  yylineno;
extern int  yylex(void);
extern FILE *yyin;

void yyerror(const char *msg);

/* The AST root filled in by the start rule */
ASTNode *ast_root = NULL;
%}

/* ── Semantic value union ─────────────────────────────────── */
%union {
    double   dval;   /* numeric literal                         */
    int      ival;   /* boolean / integer                       */
    char    *sval;   /* string / identifier                     */
    ASTNode *node;   /* any AST node                            */
    TSType   tval;   /* type annotation                         */
}

/* ── Token declarations ───────────────────────────────────── */
%token TOK_LET TOK_CONST TOK_VAR TOK_FUNCTION TOK_RETURN
%token TOK_IF TOK_ELSE TOK_WHILE TOK_FOR
%token TOK_PRINT TOK_NULL
%token TOK_TYPE_NUMBER TOK_TYPE_BOOLEAN TOK_TYPE_STRING TOK_TYPE_VOID
%token TOK_AND TOK_OR TOK_NOT
%token TOK_EQ TOK_NEQ TOK_LT TOK_GT TOK_LEQ TOK_GEQ

%token <dval> TOK_NUMBER
%token <sval> TOK_STRING TOK_ID
%token <ival> TOK_TRUE TOK_FALSE

/* ── Nonterminal types ────────────────────────────────────── */
%type <node> program stmt_list stmt
%type <node> var_decl func_decl param_list param_list_ne param
%type <node> block return_stmt if_stmt while_stmt for_stmt
%type <node> expr_stmt print_stmt
%type <node> expr assign_expr or_expr and_expr eq_expr rel_expr
%type <node> add_expr mul_expr unary_expr primary
%type <node> arg_list arg_list_ne
%type <tval> type_ann opt_type_ann opt_return_type

/* ── Operator precedence (lowest → highest) ─────────────────── */
%right '='
%left  TOK_OR
%left  TOK_AND
%left  TOK_EQ TOK_NEQ
%left  TOK_LT TOK_GT TOK_LEQ TOK_GEQ
%left  '+' '-'
%left  '*' '/' '%'
%right TOK_NOT UMINUS

%nonassoc TOK_ELSE    /* resolve dangling-else */

%%

/* ── Top level ─────────────────────────────────────────────── */
program
    : stmt_list
        {
            $$ = ast_new_node(NODE_PROGRAM);
            $$->left  = $1;
            $$->line  = 1;
            ast_root  = $$;
        }
    ;

stmt_list
    : /* empty */           { $$ = NULL; }
    | stmt_list stmt
        {
            /* Append $2 to the end of the sibling list             */
            if ($1 == NULL) {
                $$ = $2;
            } else {
                ASTNode *cur = $1;
                while (cur->next) cur = cur->next;
                cur->next = $2;
                $$ = $1;
            }
        }
    ;

/* ── Statement ─────────────────────────────────────────────── */
stmt
    : var_decl  ';'     { $$ = $1; }
    | func_decl         { $$ = $1; }
    | return_stmt ';'   { $$ = $1; }
    | if_stmt           { $$ = $1; }
    | while_stmt        { $$ = $1; }
    | for_stmt          { $$ = $1; }
    | print_stmt ';'    { $$ = $1; }
    | expr_stmt ';'     { $$ = $1; }
    | block             { $$ = $1; }
    | ';'               { $$ = NULL; }    /* empty statement       */
    ;

/* ── Variable declaration ───────────────────────────────────── */
var_decl
    : TOK_LET TOK_ID opt_type_ann '=' expr
        {
            $$ = ast_new_var_decl($2, $3, $5, 0, yylineno);
            free($2);
        }
    | TOK_LET TOK_ID opt_type_ann
        {
            $$ = ast_new_var_decl($2, $3, NULL, 0, yylineno);
            free($2);
        }
    | TOK_CONST TOK_ID opt_type_ann '=' expr
        {
            $$ = ast_new_var_decl($2, $3, $5, 1, yylineno);
            free($2);
        }
    | TOK_VAR TOK_ID opt_type_ann '=' expr
        {
            $$ = ast_new_var_decl($2, $3, $5, 0, yylineno);
            free($2);
        }
    | TOK_VAR TOK_ID opt_type_ann
        {
            $$ = ast_new_var_decl($2, $3, NULL, 0, yylineno);
            free($2);
        }
    ;

/* ── Optional type annotation  :  type ─────────────────────── */
opt_type_ann
    : /* empty */       { $$ = TYPE_UNKNOWN; }
    | ':' type_ann      { $$ = $2; }
    ;

type_ann
    : TOK_TYPE_NUMBER   { $$ = TYPE_NUMBER;  }
    | TOK_TYPE_BOOLEAN  { $$ = TYPE_BOOLEAN; }
    | TOK_TYPE_STRING   { $$ = TYPE_STRING;  }
    | TOK_TYPE_VOID     { $$ = TYPE_VOID;    }
    ;

/* ── Function declaration ───────────────────────────────────── */
func_decl
    : TOK_FUNCTION TOK_ID '(' param_list ')' opt_return_type block
        {
            $$ = ast_new_func_decl($2, $6, $4, $7, yylineno);
            free($2);
        }
    ;

opt_return_type
    : /* empty */       { $$ = TYPE_VOID; }
    | ':' type_ann      { $$ = $2; }
    ;

param_list
    : /* empty */           { $$ = NULL; }
    | param_list_ne         { $$ = $1;   }
    ;

param_list_ne
    : param                             { $$ = $1; }
    | param_list_ne ',' param
        {
            ASTNode *cur = $1;
            while (cur->next) cur = cur->next;
            cur->next = $3;
            $$ = $1;
        }
    ;

param
    : TOK_ID opt_type_ann
        {
            $$ = ast_new_param($1, $2, yylineno);
            free($1);
        }
    ;

/* ── Block ──────────────────────────────────────────────────── */
block
    : '{' stmt_list '}'  { $$ = ast_new_block($2, yylineno); }
    ;

/* ── return ─────────────────────────────────────────────────── */
return_stmt
    : TOK_RETURN expr   { $$ = ast_new_return($2, yylineno); }
    | TOK_RETURN        { $$ = ast_new_return(NULL, yylineno); }
    ;

/* ── if / else ──────────────────────────────────────────────── */
if_stmt
    : TOK_IF '(' expr ')' block
        {
            $$ = ast_new_if($3, $5, NULL, yylineno);
        }
    | TOK_IF '(' expr ')' block TOK_ELSE block
        {
            $$ = ast_new_if($3, $5, $7, yylineno);
        }
    | TOK_IF '(' expr ')' block TOK_ELSE if_stmt
        {
            $$ = ast_new_if($3, $5, $7, yylineno);
        }
    ;

/* ── while ──────────────────────────────────────────────────── */
while_stmt
    : TOK_WHILE '(' expr ')' block
        {
            $$ = ast_new_while($3, $5, yylineno);
        }
    ;

/* ── for ────────────────────────────────────────────────────── */
for_stmt
    : TOK_FOR '(' var_decl ';' expr ';' expr ')' block
        {
            $$ = ast_new_for($3, $5, $7, $9, yylineno);
        }
    | TOK_FOR '(' expr_stmt ';' expr ';' expr ')' block
        {
            $$ = ast_new_for($3, $5, $7, $9, yylineno);
        }
    ;

/* ── console.log ─────────────────────────────────────────────── */
print_stmt
    : TOK_PRINT '(' arg_list ')'
        {
            $$ = ast_new_print($3, yylineno);
        }
    ;

/* ── expression statement ───────────────────────────────────── */
expr_stmt
    : expr
        {
            ASTNode *n = ast_new_node(NODE_EXPR_STMT);
            n->left    = $1;
            n->line    = yylineno;
            $$ = n;
        }
    ;

/* ── Argument list ──────────────────────────────────────────── */
arg_list
    : /* empty */       { $$ = NULL; }
    | arg_list_ne       { $$ = $1;   }
    ;

arg_list_ne
    : expr                          { $$ = $1; }
    | arg_list_ne ',' expr
        {
            ASTNode *cur = $1;
            while (cur->next) cur = cur->next;
            cur->next = $3;
            $$ = $1;
        }
    ;

/* ── Expressions (operator precedence handled by Bison) ─────── */
expr        : assign_expr           { $$ = $1; } ;

assign_expr
    : or_expr                       { $$ = $1; }
    | TOK_ID '=' assign_expr
        {
            ASTNode *id = ast_new_ident($1, yylineno);
            $$ = ast_new_assign(id, $3, yylineno);
            free($1);
        }
    ;

or_expr
    : and_expr                      { $$ = $1; }
    | or_expr TOK_OR and_expr       { $$ = ast_new_binop("||", $1, $3, yylineno); }
    ;

and_expr
    : eq_expr                       { $$ = $1; }
    | and_expr TOK_AND eq_expr      { $$ = ast_new_binop("&&", $1, $3, yylineno); }
    ;

eq_expr
    : rel_expr                      { $$ = $1; }
    | eq_expr TOK_EQ  rel_expr      { $$ = ast_new_binop("==", $1, $3, yylineno); }
    | eq_expr TOK_NEQ rel_expr      { $$ = ast_new_binop("!=", $1, $3, yylineno); }
    ;

rel_expr
    : add_expr                      { $$ = $1; }
    | rel_expr TOK_LT  add_expr     { $$ = ast_new_binop("<",  $1, $3, yylineno); }
    | rel_expr TOK_GT  add_expr     { $$ = ast_new_binop(">",  $1, $3, yylineno); }
    | rel_expr TOK_LEQ add_expr     { $$ = ast_new_binop("<=", $1, $3, yylineno); }
    | rel_expr TOK_GEQ add_expr     { $$ = ast_new_binop(">=", $1, $3, yylineno); }
    ;

add_expr
    : mul_expr                      { $$ = $1; }
    | add_expr '+' mul_expr         { $$ = ast_new_binop("+",  $1, $3, yylineno); }
    | add_expr '-' mul_expr         { $$ = ast_new_binop("-",  $1, $3, yylineno); }
    ;

mul_expr
    : unary_expr                    { $$ = $1; }
    | mul_expr '*' unary_expr       { $$ = ast_new_binop("*",  $1, $3, yylineno); }
    | mul_expr '/' unary_expr       { $$ = ast_new_binop("/",  $1, $3, yylineno); }
    | mul_expr '%' unary_expr       { $$ = ast_new_binop("%",  $1, $3, yylineno); }
    ;

unary_expr
    : primary                       { $$ = $1; }
    | TOK_NOT unary_expr            { $$ = ast_new_unop("!",  $2, yylineno); }
    | '-'     unary_expr  %prec UMINUS
                                    { $$ = ast_new_unop("-",  $2, yylineno); }
    ;

primary
    : TOK_NUMBER
        { $$ = ast_new_number($1, yylineno); }
    | TOK_STRING
        { $$ = ast_new_string($1, yylineno); free($1); }
    | TOK_TRUE
        { $$ = ast_new_bool(1, yylineno); }
    | TOK_FALSE
        { $$ = ast_new_bool(0, yylineno); }
    | TOK_NULL
        { $$ = ast_new_null(yylineno); }
    | TOK_ID
        { $$ = ast_new_ident($1, yylineno); free($1); }
    | TOK_ID '(' arg_list ')'
        { $$ = ast_new_call($1, $3, yylineno); free($1); }
    | '(' expr ')'
        { $$ = $2; }
    ;

%%

/* ── Error handler ──────────────────────────────────────────── */
void yyerror(const char *msg) {
    fprintf(stderr, "Parse error at line %d: %s\n", yylineno, msg);
}

/* ── main ───────────────────────────────────────────────────── */
int main(int argc, char *argv[]) {
    if (argc > 1) {
        yyin = fopen(argv[1], "r");
        if (!yyin) {
            fprintf(stderr, "Cannot open file: %s\n", argv[1]);
            return 1;
        }
    } else {
        yyin = stdin;
    }

    printf("=== TS2C Compiler ===\n");

    /* 1. Parse – builds AST in ast_root */
    if (yyparse() != 0) {
        fprintf(stderr, "Compilation failed during parsing.\n");
        return 1;
    }

    /* 2. Print AST */
    printf("\n--- Abstract Syntax Tree ---\n");
    ast_print(ast_root, 0);

    /* 3. Generate Three-Address Code */
    printf("\n--- Three-Address Code (before optimisation) ---\n");
    TAC *tac_list = generate_tac(ast_root);
    print_tac(tac_list);

    /* 4. Optimise */
    printf("\n--- Three-Address Code (after optimisation) ---\n");
    tac_list = optimize_tac(tac_list);
    print_tac(tac_list);

    /* 5. Emit C code */
    printf("\n--- Generated C Code ---\n");
    emit_c_code(tac_list, argc > 2 ? argv[2] : NULL);

    /* Cleanup */
    free_tac(tac_list);
    ast_free(ast_root);
    if (yyin != stdin) fclose(yyin);

    return 0;
}
