%{
/* ============================================================
   ts_lexer.l  –  Flex lexer for the TS2C compiler
   TypeScript → C translator
   ============================================================
   This file is the Flex specification.  When compiled together
   with Bison (parser.y) it returns token codes so the parser
   can build an AST.  The original stand-alone printf() version
   has been replaced with proper token returns.
   ============================================================ */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"
#include "parser.tab.h"   /* token codes + YYSTYPE union from Bison */

int yylineno = 1;         /* current source line (kept updated) */
%}

%option noyywrap

DIGIT      [0-9]
LETTER     [a-zA-Z_$]
ID         {LETTER}({LETTER}|{DIGIT})*
INT_NUM    {DIGIT}+
FLOAT_NUM  {DIGIT}+"."{DIGIT}*
STRING     \"([^\"\\]|\\.)*\"

%%

 /* ── Keywords ─────────────────────────────────────────────── */
"let"           { return TOK_LET;      }
"const"         { return TOK_CONST;    }
"var"           { return TOK_VAR;      }
"function"      { return TOK_FUNCTION; }
"return"        { return TOK_RETURN;   }
"if"            { return TOK_IF;       }
"else"          { return TOK_ELSE;     }
"while"         { return TOK_WHILE;    }
"for"           { return TOK_FOR;      }
"true"          { yylval.ival = 1; return TOK_TRUE;  }
"false"         { yylval.ival = 0; return TOK_FALSE; }
"null"          { return TOK_NULL;     }
"npc"           { return TOK_NULL;     }   /* project alias for null */
"cap"           { yylval.ival = 0; return TOK_FALSE; } /* project alias */
"console.log"   { return TOK_PRINT;    }

 /* ── Types ────────────────────────────────────────────────── */
"number"        { return TOK_TYPE_NUMBER;  }
"boolean"       { return TOK_TYPE_BOOLEAN; }
"string"        { return TOK_TYPE_STRING;  }
"void"          { return TOK_TYPE_VOID;    }

 /* ── Literals ─────────────────────────────────────────────── */
{FLOAT_NUM}     { yylval.dval = atof(yytext); return TOK_NUMBER; }
{INT_NUM}       { yylval.dval = atof(yytext); return TOK_NUMBER; }

{STRING}        {
                    /* strip surrounding quotes */
                    int len = strlen(yytext) - 2;
                    yylval.sval = (char*)malloc(len + 1);
                    strncpy(yylval.sval, yytext + 1, len);
                    yylval.sval[len] = '\0';
                    return TOK_STRING;
                }

 /* ── Identifiers ──────────────────────────────────────────── */
{ID}            { yylval.sval = strdup(yytext); return TOK_ID; }

 /* ── Operators ────────────────────────────────────────────── */
"==="           { return TOK_EQ;  }
"!=="           { return TOK_NEQ; }
"=="            { return TOK_EQ;  }
"!="            { return TOK_NEQ; }
"<="            { return TOK_LEQ; }
">="            { return TOK_GEQ; }
"<"             { return TOK_LT;  }
">"             { return TOK_GT;  }
"&&"            { return TOK_AND; }
"||"            { return TOK_OR;  }
"!"             { return TOK_NOT; }
"+"             { return '+'; }
"-"             { return '-'; }
"*"             { return '*'; }
"/"             { return '/'; }
"%"             { return '%'; }
"="             { return '='; }

 /* ── Separators / Punctuation ─────────────────────────────── */
"("             { return '('; }
")"             { return ')'; }
"{"             { return '{'; }
"}"             { return '}'; }
";"             { return ';'; }
","             { return ','; }
":"             { return ':'; }

 /* ── Comments ─────────────────────────────────────────────── */
"//"[^\n]*      { /* single-line comment – discard */ }
"/*"([^*]|\*+[^*/])*\*+"/"  { /* block comment – discard */
                    char *p = yytext;
                    while (*p) { if (*p++ == '\n') yylineno++; }
                }

 /* ── Whitespace ───────────────────────────────────────────── */
\n              { yylineno++; }
[ \t\r]+        { /* ignore */ }

 /* ── Catch-all (invalid token) ───────────────────────────── */
.               {
                    fprintf(stderr, "Lexer: unknown character '%s' at line %d\n",
                            yytext, yylineno);
                }

%%
