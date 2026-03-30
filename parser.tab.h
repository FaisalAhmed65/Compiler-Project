
/* A Bison parser, made by GNU Bison 2.4.1.  */

/* Skeleton interface for Bison's Yacc-like parsers in C
   
      Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004, 2005, 2006
   Free Software Foundation, Inc.
   
   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.
   
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.
   
   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */


/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     TOK_LET = 258,
     TOK_CONST = 259,
     TOK_VAR = 260,
     TOK_FUNCTION = 261,
     TOK_RETURN = 262,
     TOK_IF = 263,
     TOK_ELSE = 264,
     TOK_WHILE = 265,
     TOK_FOR = 266,
     TOK_PRINT = 267,
     TOK_NULL = 268,
     TOK_TYPE_NUMBER = 269,
     TOK_TYPE_BOOLEAN = 270,
     TOK_TYPE_STRING = 271,
     TOK_TYPE_VOID = 272,
     TOK_AND = 273,
     TOK_OR = 274,
     TOK_NOT = 275,
     TOK_EQ = 276,
     TOK_NEQ = 277,
     TOK_LT = 278,
     TOK_GT = 279,
     TOK_LEQ = 280,
     TOK_GEQ = 281,
     TOK_NUMBER = 282,
     TOK_STRING = 283,
     TOK_ID = 284,
     TOK_TRUE = 285,
     TOK_FALSE = 286,
     UMINUS = 287
   };
#endif



#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
{

/* Line 1676 of yacc.c  */
#line 21 "parser.y"

    double   dval;   /* numeric literal                         */
    int      ival;   /* boolean / integer                       */
    char    *sval;   /* string / identifier                     */
    ASTNode *node;   /* any AST node                            */
    TSType   tval;   /* type annotation                         */



/* Line 1676 of yacc.c  */
#line 94 "parser.tab.h"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif

extern YYSTYPE yylval;


