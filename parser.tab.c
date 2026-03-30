
/* A Bison parser, made by GNU Bison 2.4.1.  */

/* Skeleton implementation for Bison's Yacc-like parsers in C
   
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

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "2.4.1"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1

/* Using locations.  */
#define YYLSP_NEEDED 0



/* Copy the first part of user declarations.  */

/* Line 189 of yacc.c  */
#line 1 "parser.y"

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


/* Line 189 of yacc.c  */
#line 93 "parser.tab.c"

/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* Enabling the token table.  */
#ifndef YYTOKEN_TABLE
# define YYTOKEN_TABLE 0
#endif


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

/* Line 214 of yacc.c  */
#line 21 "parser.y"

    double   dval;   /* numeric literal                         */
    int      ival;   /* boolean / integer                       */
    char    *sval;   /* string / identifier                     */
    ASTNode *node;   /* any AST node                            */
    TSType   tval;   /* type annotation                         */



/* Line 214 of yacc.c  */
#line 171 "parser.tab.c"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif


/* Copy the second part of user declarations.  */


/* Line 264 of yacc.c  */
#line 183 "parser.tab.c"

#ifdef short
# undef short
#endif

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 yytype_uint8;
#else
typedef unsigned char yytype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 yytype_int8;
#elif (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
typedef signed char yytype_int8;
#else
typedef short int yytype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 yytype_uint16;
#else
typedef unsigned short int yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short int yytype_int16;
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif ! defined YYSIZE_T && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

#ifndef YY_
# if YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(e) ((void) (e))
#else
# define YYUSE(e) /* empty */
#endif

/* Identity function, used to suppress warnings about constant conditions.  */
#ifndef lint
# define YYID(n) (n)
#else
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static int
YYID (int yyi)
#else
static int
YYID (yyi)
    int yyi;
#endif
{
  return yyi;
}
#endif

#if ! defined yyoverflow || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#     ifndef _STDLIB_H
#      define _STDLIB_H 1
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's `empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (YYID (0))
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined _STDLIB_H \
       && ! ((defined YYMALLOC || defined malloc) \
	     && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef _STDLIB_H
#    define _STDLIB_H 1
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
	 || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

/* Copy COUNT objects from FROM to TO.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(To, From, Count) \
      __builtin_memcpy (To, From, (Count) * sizeof (*(From)))
#  else
#   define YYCOPY(To, From, Count)		\
      do					\
	{					\
	  YYSIZE_T yyi;				\
	  for (yyi = 0; yyi < (Count); yyi++)	\
	    (To)[yyi] = (From)[yyi];		\
	}					\
      while (YYID (0))
#  endif
# endif

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)				\
    do									\
      {									\
	YYSIZE_T yynewbytes;						\
	YYCOPY (&yyptr->Stack_alloc, Stack, yysize);			\
	Stack = &yyptr->Stack_alloc;					\
	yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
	yyptr += yynewbytes / sizeof (*yyptr);				\
      }									\
    while (YYID (0))

#endif

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  3
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   182

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  46
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  31
/* YYNRULES -- Number of rules.  */
#define YYNRULES  81
/* YYNRULES -- Number of states.  */
#define YYNSTATES  152

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   287

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,    37,     2,     2,
      41,    42,    35,    33,    43,    34,     2,    36,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    40,    39,
       2,    32,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    44,     2,    45,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    38
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint16 yyprhs[] =
{
       0,     0,     3,     5,     6,     9,    12,    14,    17,    19,
      21,    23,    26,    29,    31,    33,    39,    43,    49,    55,
      59,    60,    63,    65,    67,    69,    71,    79,    80,    83,
      84,    86,    88,    92,    95,    99,   102,   104,   110,   118,
     126,   132,   142,   152,   157,   159,   160,   162,   164,   168,
     170,   172,   176,   178,   182,   184,   188,   190,   194,   198,
     200,   204,   208,   212,   216,   218,   222,   226,   228,   232,
     236,   240,   242,   245,   248,   250,   252,   254,   256,   258,
     260,   265
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int8 yyrhs[] =
{
      47,     0,    -1,    48,    -1,    -1,    48,    49,    -1,    50,
      39,    -1,    53,    -1,    59,    39,    -1,    60,    -1,    61,
      -1,    62,    -1,    63,    39,    -1,    64,    39,    -1,    58,
      -1,    39,    -1,     3,    29,    51,    32,    67,    -1,     3,
      29,    51,    -1,     4,    29,    51,    32,    67,    -1,     5,
      29,    51,    32,    67,    -1,     5,    29,    51,    -1,    -1,
      40,    52,    -1,    14,    -1,    15,    -1,    16,    -1,    17,
      -1,     6,    29,    41,    55,    42,    54,    58,    -1,    -1,
      40,    52,    -1,    -1,    56,    -1,    57,    -1,    56,    43,
      57,    -1,    29,    51,    -1,    44,    48,    45,    -1,     7,
      67,    -1,     7,    -1,     8,    41,    67,    42,    58,    -1,
       8,    41,    67,    42,    58,     9,    58,    -1,     8,    41,
      67,    42,    58,     9,    60,    -1,    10,    41,    67,    42,
      58,    -1,    11,    41,    50,    39,    67,    39,    67,    42,
      58,    -1,    11,    41,    64,    39,    67,    39,    67,    42,
      58,    -1,    12,    41,    65,    42,    -1,    67,    -1,    -1,
      66,    -1,    67,    -1,    66,    43,    67,    -1,    68,    -1,
      69,    -1,    29,    32,    68,    -1,    70,    -1,    69,    19,
      70,    -1,    71,    -1,    70,    18,    71,    -1,    72,    -1,
      71,    21,    72,    -1,    71,    22,    72,    -1,    73,    -1,
      72,    23,    73,    -1,    72,    24,    73,    -1,    72,    25,
      73,    -1,    72,    26,    73,    -1,    74,    -1,    73,    33,
      74,    -1,    73,    34,    74,    -1,    75,    -1,    74,    35,
      75,    -1,    74,    36,    75,    -1,    74,    37,    75,    -1,
      76,    -1,    20,    75,    -1,    34,    75,    -1,    27,    -1,
      28,    -1,    30,    -1,    31,    -1,    13,    -1,    29,    -1,
      29,    41,    65,    42,    -1,    41,    67,    42,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,    67,    67,    77,    78,    94,    95,    96,    97,    98,
      99,   100,   101,   102,   103,   108,   113,   118,   123,   128,
     137,   138,   142,   143,   144,   145,   150,   158,   159,   163,
     164,   168,   169,   179,   188,   193,   194,   199,   203,   207,
     215,   223,   227,   235,   243,   254,   255,   259,   260,   270,
     273,   274,   283,   284,   288,   289,   293,   294,   295,   299,
     300,   301,   302,   303,   307,   308,   309,   313,   314,   315,
     316,   320,   321,   322,   327,   329,   331,   333,   335,   337,
     339,   341
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "TOK_LET", "TOK_CONST", "TOK_VAR",
  "TOK_FUNCTION", "TOK_RETURN", "TOK_IF", "TOK_ELSE", "TOK_WHILE",
  "TOK_FOR", "TOK_PRINT", "TOK_NULL", "TOK_TYPE_NUMBER",
  "TOK_TYPE_BOOLEAN", "TOK_TYPE_STRING", "TOK_TYPE_VOID", "TOK_AND",
  "TOK_OR", "TOK_NOT", "TOK_EQ", "TOK_NEQ", "TOK_LT", "TOK_GT", "TOK_LEQ",
  "TOK_GEQ", "TOK_NUMBER", "TOK_STRING", "TOK_ID", "TOK_TRUE", "TOK_FALSE",
  "'='", "'+'", "'-'", "'*'", "'/'", "'%'", "UMINUS", "';'", "':'", "'('",
  "')'", "','", "'{'", "'}'", "$accept", "program", "stmt_list", "stmt",
  "var_decl", "opt_type_ann", "type_ann", "func_decl", "opt_return_type",
  "param_list", "param_list_ne", "param", "block", "return_stmt",
  "if_stmt", "while_stmt", "for_stmt", "print_stmt", "expr_stmt",
  "arg_list", "arg_list_ne", "expr", "assign_expr", "or_expr", "and_expr",
  "eq_expr", "rel_expr", "add_expr", "mul_expr", "unary_expr", "primary", 0
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[YYLEX-NUM] -- Internal token number corresponding to
   token YYLEX-NUM.  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,    61,    43,    45,    42,    47,    37,   287,    59,
      58,    40,    41,    44,   123,   125
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    46,    47,    48,    48,    49,    49,    49,    49,    49,
      49,    49,    49,    49,    49,    50,    50,    50,    50,    50,
      51,    51,    52,    52,    52,    52,    53,    54,    54,    55,
      55,    56,    56,    57,    58,    59,    59,    60,    60,    60,
      61,    62,    62,    63,    64,    65,    65,    66,    66,    67,
      68,    68,    69,    69,    70,    70,    71,    71,    71,    72,
      72,    72,    72,    72,    73,    73,    73,    74,    74,    74,
      74,    75,    75,    75,    76,    76,    76,    76,    76,    76,
      76,    76
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     1,     0,     2,     2,     1,     2,     1,     1,
       1,     2,     2,     1,     1,     5,     3,     5,     5,     3,
       0,     2,     1,     1,     1,     1,     7,     0,     2,     0,
       1,     1,     3,     2,     3,     2,     1,     5,     7,     7,
       5,     9,     9,     4,     1,     0,     1,     1,     3,     1,
       1,     3,     1,     3,     1,     3,     1,     3,     3,     1,
       3,     3,     3,     3,     1,     3,     3,     1,     3,     3,
       3,     1,     2,     2,     1,     1,     1,     1,     1,     1,
       4,     3
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       3,     0,     2,     1,     0,     0,     0,     0,    36,     0,
       0,     0,     0,    78,     0,    74,    75,    79,    76,    77,
       0,    14,     0,     3,     4,     0,     6,    13,     0,     8,
       9,    10,     0,     0,    44,    49,    50,    52,    54,    56,
      59,    64,    67,    71,    20,    20,    20,     0,    35,     0,
       0,     0,    45,    79,    72,     0,    45,    73,     0,     0,
       5,     7,    11,    12,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    16,     0,
      19,    29,     0,     0,     0,     0,     0,    46,    47,    51,
       0,    81,    34,    53,    55,    57,    58,    60,    61,    62,
      63,    65,    66,    68,    69,    70,    22,    23,    24,    25,
      21,     0,     0,     0,    20,     0,    30,    31,     0,     0,
       0,     0,    43,     0,    80,    15,    17,    18,    33,    27,
       0,    37,    40,     0,     0,    48,     0,     0,    32,     0,
       0,     0,    28,    26,    38,    39,     0,     0,     0,     0,
      41,    42
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     1,     2,    24,    25,    78,   110,    26,   137,   115,
     116,   117,    27,    28,    29,    30,    31,    32,    33,    86,
      87,    34,    35,    36,    37,    38,    39,    40,    41,    42,
      43
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -45
static const yytype_int16 yypact[] =
{
     -45,    53,    80,   -45,    30,    31,    33,    38,    -5,    28,
      36,    37,    39,   -45,   141,   -45,   -45,   -28,   -45,   -45,
     141,   -45,    -5,   -45,   -45,    40,   -45,   -45,    42,   -45,
     -45,   -45,    43,    50,   -45,   -45,    75,    77,   -16,    -7,
     -13,   -25,   -45,   -45,    56,    56,    56,    57,   -45,    -5,
      -5,   122,    -5,    58,   -45,    -5,    -5,   -45,    55,    27,
     -45,   -45,   -45,   -45,   141,   141,   141,   141,   141,   141,
     141,   141,   141,   141,   141,   141,   141,    35,    69,    70,
      74,    87,    76,    78,    83,    84,    86,    88,   -45,   -45,
      92,   -45,   -45,    77,   -16,    -7,    -7,   -13,   -13,   -13,
     -13,   -25,   -25,   -45,   -45,   -45,   -45,   -45,   -45,   -45,
     -45,    -5,    -5,    -5,    56,    96,    97,   -45,    73,    73,
      -5,    -5,   -45,    -5,   -45,   -45,   -45,   -45,   -45,    89,
      87,   121,   -45,   100,   102,   -45,    35,    73,   -45,    -1,
      -5,    -5,   -45,   -45,   -45,   -45,   101,   103,    73,    73,
     -45,   -45
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int8 yypgoto[] =
{
     -45,   -45,   123,   -45,    93,   -44,    11,   -45,   -45,   -45,
     -45,    29,    18,   -45,     9,   -45,   -45,   -45,   107,   104,
     -45,    -8,   109,   -45,    98,   108,   -39,     5,   -27,   -11,
     -45
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -1
static const yytype_uint8 yytable[] =
{
      48,    79,    80,    54,    55,    66,    67,     9,    13,    57,
      74,    75,    76,    56,    58,    14,    68,    69,    70,    71,
      72,    73,    15,    16,    17,    18,    19,    95,    96,    20,
       4,     5,     6,     7,     8,     9,    22,    10,    11,    12,
      13,    82,    83,    23,    88,   101,   102,    14,    88,   106,
     107,   108,   109,     3,    15,    16,    17,    18,    19,    44,
      45,    20,    46,   103,   104,   105,    21,    47,    22,    49,
     128,    23,    92,    97,    98,    99,   100,    50,    51,    60,
      52,    61,    62,     4,     5,     6,     7,     8,     9,    63,
      10,    11,    12,    13,    64,    65,    77,    91,    81,    56,
      14,   111,   112,   125,   126,   127,   113,    15,    16,    17,
      18,    19,   133,   134,    20,   135,   114,    23,   118,    21,
     119,    22,   120,   121,    23,     4,     5,     6,   122,   136,
     139,   123,   146,   147,   124,    13,   131,   132,   129,   140,
     130,   141,    14,   148,    84,   149,    59,   142,   145,    15,
      16,    17,    18,    19,    13,   143,    20,   144,    85,   138,
      90,    14,    93,    22,    89,     0,   150,   151,    15,    16,
      53,    18,    19,    94,     0,    20,     0,     0,     0,     0,
       0,     0,    22
};

static const yytype_int16 yycheck[] =
{
       8,    45,    46,    14,    32,    21,    22,     8,    13,    20,
      35,    36,    37,    41,    22,    20,    23,    24,    25,    26,
      33,    34,    27,    28,    29,    30,    31,    66,    67,    34,
       3,     4,     5,     6,     7,     8,    41,    10,    11,    12,
      13,    49,    50,    44,    52,    72,    73,    20,    56,    14,
      15,    16,    17,     0,    27,    28,    29,    30,    31,    29,
      29,    34,    29,    74,    75,    76,    39,    29,    41,    41,
     114,    44,    45,    68,    69,    70,    71,    41,    41,    39,
      41,    39,    39,     3,     4,     5,     6,     7,     8,    39,
      10,    11,    12,    13,    19,    18,    40,    42,    41,    41,
      20,    32,    32,   111,   112,   113,    32,    27,    28,    29,
      30,    31,   120,   121,    34,   123,    29,    44,    42,    39,
      42,    41,    39,    39,    44,     3,     4,     5,    42,    40,
       9,    43,   140,   141,    42,    13,   118,   119,    42,    39,
      43,    39,    20,    42,    51,    42,    23,   136,   139,    27,
      28,    29,    30,    31,    13,   137,    34,   139,    51,   130,
      56,    20,    64,    41,    55,    -1,   148,   149,    27,    28,
      29,    30,    31,    65,    -1,    34,    -1,    -1,    -1,    -1,
      -1,    -1,    41
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,    47,    48,     0,     3,     4,     5,     6,     7,     8,
      10,    11,    12,    13,    20,    27,    28,    29,    30,    31,
      34,    39,    41,    44,    49,    50,    53,    58,    59,    60,
      61,    62,    63,    64,    67,    68,    69,    70,    71,    72,
      73,    74,    75,    76,    29,    29,    29,    29,    67,    41,
      41,    41,    41,    29,    75,    32,    41,    75,    67,    48,
      39,    39,    39,    39,    19,    18,    21,    22,    23,    24,
      25,    26,    33,    34,    35,    36,    37,    40,    51,    51,
      51,    41,    67,    67,    50,    64,    65,    66,    67,    68,
      65,    42,    45,    70,    71,    72,    72,    73,    73,    73,
      73,    74,    74,    75,    75,    75,    14,    15,    16,    17,
      52,    32,    32,    32,    29,    55,    56,    57,    42,    42,
      39,    39,    42,    43,    42,    67,    67,    67,    51,    42,
      43,    58,    58,    67,    67,    67,    40,    54,    57,     9,
      39,    39,    52,    58,    58,    60,    67,    67,    42,    42,
      58,    58
};

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		(-2)
#define YYEOF		0

#define YYACCEPT	goto yyacceptlab
#define YYABORT		goto yyabortlab
#define YYERROR		goto yyerrorlab


/* Like YYERROR except do call yyerror.  This remains here temporarily
   to ease the transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  */

#define YYFAIL		goto yyerrlab

#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)					\
do								\
  if (yychar == YYEMPTY && yylen == 1)				\
    {								\
      yychar = (Token);						\
      yylval = (Value);						\
      yytoken = YYTRANSLATE (yychar);				\
      YYPOPSTACK (1);						\
      goto yybackup;						\
    }								\
  else								\
    {								\
      yyerror (YY_("syntax error: cannot back up")); \
      YYERROR;							\
    }								\
while (YYID (0))


#define YYTERROR	1
#define YYERRCODE	256


/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#define YYRHSLOC(Rhs, K) ((Rhs)[K])
#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)				\
    do									\
      if (YYID (N))                                                    \
	{								\
	  (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;	\
	  (Current).first_column = YYRHSLOC (Rhs, 1).first_column;	\
	  (Current).last_line    = YYRHSLOC (Rhs, N).last_line;		\
	  (Current).last_column  = YYRHSLOC (Rhs, N).last_column;	\
	}								\
      else								\
	{								\
	  (Current).first_line   = (Current).last_line   =		\
	    YYRHSLOC (Rhs, 0).last_line;				\
	  (Current).first_column = (Current).last_column =		\
	    YYRHSLOC (Rhs, 0).last_column;				\
	}								\
    while (YYID (0))
#endif


/* YY_LOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

#ifndef YY_LOCATION_PRINT
# if YYLTYPE_IS_TRIVIAL
#  define YY_LOCATION_PRINT(File, Loc)			\
     fprintf (File, "%d.%d-%d.%d",			\
	      (Loc).first_line, (Loc).first_column,	\
	      (Loc).last_line,  (Loc).last_column)
# else
#  define YY_LOCATION_PRINT(File, Loc) ((void) 0)
# endif
#endif


/* YYLEX -- calling `yylex' with the right arguments.  */

#ifdef YYLEX_PARAM
# define YYLEX yylex (YYLEX_PARAM)
#else
# define YYLEX yylex ()
#endif

/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)			\
do {						\
  if (yydebug)					\
    YYFPRINTF Args;				\
} while (YYID (0))

# define YY_SYMBOL_PRINT(Title, Type, Value, Location)			  \
do {									  \
  if (yydebug)								  \
    {									  \
      YYFPRINTF (stderr, "%s ", Title);					  \
      yy_symbol_print (stderr,						  \
		  Type, Value); \
      YYFPRINTF (stderr, "\n");						  \
    }									  \
} while (YYID (0))


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_value_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# else
  YYUSE (yyoutput);
# endif
  switch (yytype)
    {
      default:
	break;
    }
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (yytype < YYNTOKENS)
    YYFPRINTF (yyoutput, "token %s (", yytname[yytype]);
  else
    YYFPRINTF (yyoutput, "nterm %s (", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_stack_print (yytype_int16 *yybottom, yytype_int16 *yytop)
#else
static void
yy_stack_print (yybottom, yytop)
    yytype_int16 *yybottom;
    yytype_int16 *yytop;
#endif
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)				\
do {								\
  if (yydebug)							\
    yy_stack_print ((Bottom), (Top));				\
} while (YYID (0))


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_reduce_print (YYSTYPE *yyvsp, int yyrule)
#else
static void
yy_reduce_print (yyvsp, yyrule)
    YYSTYPE *yyvsp;
    int yyrule;
#endif
{
  int yynrhs = yyr2[yyrule];
  int yyi;
  unsigned long int yylno = yyrline[yyrule];
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
	     yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr, yyrhs[yyprhs[yyrule] + yyi],
		       &(yyvsp[(yyi + 1) - (yynrhs)])
		       		       );
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (yydebug)				\
    yy_reduce_print (yyvsp, Rule); \
} while (YYID (0))

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef	YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif



#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static YYSIZE_T
yystrlen (const char *yystr)
#else
static YYSIZE_T
yystrlen (yystr)
    const char *yystr;
#endif
{
  YYSIZE_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static char *
yystpcpy (char *yydest, const char *yysrc)
#else
static char *
yystpcpy (yydest, yysrc)
    char *yydest;
    const char *yysrc;
#endif
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYSIZE_T yyn = 0;
      char const *yyp = yystr;

      for (;;)
	switch (*++yyp)
	  {
	  case '\'':
	  case ',':
	    goto do_not_strip_quotes;

	  case '\\':
	    if (*++yyp != '\\')
	      goto do_not_strip_quotes;
	    /* Fall through.  */
	  default:
	    if (yyres)
	      yyres[yyn] = *yyp;
	    yyn++;
	    break;

	  case '"':
	    if (yyres)
	      yyres[yyn] = '\0';
	    return yyn;
	  }
    do_not_strip_quotes: ;
    }

  if (! yyres)
    return yystrlen (yystr);

  return yystpcpy (yyres, yystr) - yyres;
}
# endif

/* Copy into YYRESULT an error message about the unexpected token
   YYCHAR while in state YYSTATE.  Return the number of bytes copied,
   including the terminating null byte.  If YYRESULT is null, do not
   copy anything; just return the number of bytes that would be
   copied.  As a special case, return 0 if an ordinary "syntax error"
   message will do.  Return YYSIZE_MAXIMUM if overflow occurs during
   size calculation.  */
static YYSIZE_T
yysyntax_error (char *yyresult, int yystate, int yychar)
{
  int yyn = yypact[yystate];

  if (! (YYPACT_NINF < yyn && yyn <= YYLAST))
    return 0;
  else
    {
      int yytype = YYTRANSLATE (yychar);
      YYSIZE_T yysize0 = yytnamerr (0, yytname[yytype]);
      YYSIZE_T yysize = yysize0;
      YYSIZE_T yysize1;
      int yysize_overflow = 0;
      enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
      char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
      int yyx;

# if 0
      /* This is so xgettext sees the translatable formats that are
	 constructed on the fly.  */
      YY_("syntax error, unexpected %s");
      YY_("syntax error, unexpected %s, expecting %s");
      YY_("syntax error, unexpected %s, expecting %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s");
# endif
      char *yyfmt;
      char const *yyf;
      static char const yyunexpected[] = "syntax error, unexpected %s";
      static char const yyexpecting[] = ", expecting %s";
      static char const yyor[] = " or %s";
      char yyformat[sizeof yyunexpected
		    + sizeof yyexpecting - 1
		    + ((YYERROR_VERBOSE_ARGS_MAXIMUM - 2)
		       * (sizeof yyor - 1))];
      char const *yyprefix = yyexpecting;

      /* Start YYX at -YYN if negative to avoid negative indexes in
	 YYCHECK.  */
      int yyxbegin = yyn < 0 ? -yyn : 0;

      /* Stay within bounds of both yycheck and yytname.  */
      int yychecklim = YYLAST - yyn + 1;
      int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
      int yycount = 1;

      yyarg[0] = yytname[yytype];
      yyfmt = yystpcpy (yyformat, yyunexpected);

      for (yyx = yyxbegin; yyx < yyxend; ++yyx)
	if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR)
	  {
	    if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
	      {
		yycount = 1;
		yysize = yysize0;
		yyformat[sizeof yyunexpected - 1] = '\0';
		break;
	      }
	    yyarg[yycount++] = yytname[yyx];
	    yysize1 = yysize + yytnamerr (0, yytname[yyx]);
	    yysize_overflow |= (yysize1 < yysize);
	    yysize = yysize1;
	    yyfmt = yystpcpy (yyfmt, yyprefix);
	    yyprefix = yyor;
	  }

      yyf = YY_(yyformat);
      yysize1 = yysize + yystrlen (yyf);
      yysize_overflow |= (yysize1 < yysize);
      yysize = yysize1;

      if (yysize_overflow)
	return YYSIZE_MAXIMUM;

      if (yyresult)
	{
	  /* Avoid sprintf, as that infringes on the user's name space.
	     Don't have undefined behavior even if the translation
	     produced a string with the wrong number of "%s"s.  */
	  char *yyp = yyresult;
	  int yyi = 0;
	  while ((*yyp = *yyf) != '\0')
	    {
	      if (*yyp == '%' && yyf[1] == 's' && yyi < yycount)
		{
		  yyp += yytnamerr (yyp, yyarg[yyi++]);
		  yyf += 2;
		}
	      else
		{
		  yyp++;
		  yyf++;
		}
	    }
	}
      return yysize;
    }
}
#endif /* YYERROR_VERBOSE */


/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
#else
static void
yydestruct (yymsg, yytype, yyvaluep)
    const char *yymsg;
    int yytype;
    YYSTYPE *yyvaluep;
#endif
{
  YYUSE (yyvaluep);

  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  switch (yytype)
    {

      default:
	break;
    }
}

/* Prevent warnings from -Wmissing-prototypes.  */
#ifdef YYPARSE_PARAM
#if defined __STDC__ || defined __cplusplus
int yyparse (void *YYPARSE_PARAM);
#else
int yyparse ();
#endif
#else /* ! YYPARSE_PARAM */
#if defined __STDC__ || defined __cplusplus
int yyparse (void);
#else
int yyparse ();
#endif
#endif /* ! YYPARSE_PARAM */


/* The lookahead symbol.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;

/* Number of syntax errors so far.  */
int yynerrs;



/*-------------------------.
| yyparse or yypush_parse.  |
`-------------------------*/

#ifdef YYPARSE_PARAM
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void *YYPARSE_PARAM)
#else
int
yyparse (YYPARSE_PARAM)
    void *YYPARSE_PARAM;
#endif
#else /* ! YYPARSE_PARAM */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void)
#else
int
yyparse ()

#endif
#endif
{


    int yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       `yyss': related to states.
       `yyvs': related to semantic values.

       Refer to the stacks thru separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yytype_int16 yyssa[YYINITDEPTH];
    yytype_int16 *yyss;
    yytype_int16 *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    YYSIZE_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  yytoken = 0;
  yyss = yyssa;
  yyvs = yyvsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */
  yyssp = yyss;
  yyvsp = yyvs;

  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
	/* Give user a chance to reallocate the stack.  Use copies of
	   these so that the &'s don't force the real ones into
	   memory.  */
	YYSTYPE *yyvs1 = yyvs;
	yytype_int16 *yyss1 = yyss;

	/* Each stack pointer address is followed by the size of the
	   data in use in that stack, in bytes.  This used to be a
	   conditional around just the two extra args, but that might
	   be undefined if yyoverflow is a macro.  */
	yyoverflow (YY_("memory exhausted"),
		    &yyss1, yysize * sizeof (*yyssp),
		    &yyvs1, yysize * sizeof (*yyvsp),
		    &yystacksize);

	yyss = yyss1;
	yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyexhaustedlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
	goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
	yystacksize = YYMAXDEPTH;

      {
	yytype_int16 *yyss1 = yyss;
	union yyalloc *yyptr =
	  (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
	if (! yyptr)
	  goto yyexhaustedlab;
	YYSTACK_RELOCATE (yyss_alloc, yyss);
	YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
	if (yyss1 != yyssa)
	  YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
		  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
	YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yyn == YYPACT_NINF)
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = YYLEX;
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yyn == 0 || yyn == YYTABLE_NINF)
	goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the shifted token.  */
  yychar = YYEMPTY;

  yystate = yyn;
  *++yyvsp = yylval;

  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     `$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 2:

/* Line 1455 of yacc.c  */
#line 68 "parser.y"
    {
            (yyval.node) = ast_new_node(NODE_PROGRAM);
            (yyval.node)->left  = (yyvsp[(1) - (1)].node);
            (yyval.node)->line  = 1;
            ast_root  = (yyval.node);
        ;}
    break;

  case 3:

/* Line 1455 of yacc.c  */
#line 77 "parser.y"
    { (yyval.node) = NULL; ;}
    break;

  case 4:

/* Line 1455 of yacc.c  */
#line 79 "parser.y"
    {
            /* Append $2 to the end of the sibling list             */
            if ((yyvsp[(1) - (2)].node) == NULL) {
                (yyval.node) = (yyvsp[(2) - (2)].node);
            } else {
                ASTNode *cur = (yyvsp[(1) - (2)].node);
                while (cur->next) cur = cur->next;
                cur->next = (yyvsp[(2) - (2)].node);
                (yyval.node) = (yyvsp[(1) - (2)].node);
            }
        ;}
    break;

  case 5:

/* Line 1455 of yacc.c  */
#line 94 "parser.y"
    { (yyval.node) = (yyvsp[(1) - (2)].node); ;}
    break;

  case 6:

/* Line 1455 of yacc.c  */
#line 95 "parser.y"
    { (yyval.node) = (yyvsp[(1) - (1)].node); ;}
    break;

  case 7:

/* Line 1455 of yacc.c  */
#line 96 "parser.y"
    { (yyval.node) = (yyvsp[(1) - (2)].node); ;}
    break;

  case 8:

/* Line 1455 of yacc.c  */
#line 97 "parser.y"
    { (yyval.node) = (yyvsp[(1) - (1)].node); ;}
    break;

  case 9:

/* Line 1455 of yacc.c  */
#line 98 "parser.y"
    { (yyval.node) = (yyvsp[(1) - (1)].node); ;}
    break;

  case 10:

/* Line 1455 of yacc.c  */
#line 99 "parser.y"
    { (yyval.node) = (yyvsp[(1) - (1)].node); ;}
    break;

  case 11:

/* Line 1455 of yacc.c  */
#line 100 "parser.y"
    { (yyval.node) = (yyvsp[(1) - (2)].node); ;}
    break;

  case 12:

/* Line 1455 of yacc.c  */
#line 101 "parser.y"
    { (yyval.node) = (yyvsp[(1) - (2)].node); ;}
    break;

  case 13:

/* Line 1455 of yacc.c  */
#line 102 "parser.y"
    { (yyval.node) = (yyvsp[(1) - (1)].node); ;}
    break;

  case 14:

/* Line 1455 of yacc.c  */
#line 103 "parser.y"
    { (yyval.node) = NULL; ;}
    break;

  case 15:

/* Line 1455 of yacc.c  */
#line 109 "parser.y"
    {
            (yyval.node) = ast_new_var_decl((yyvsp[(2) - (5)].sval), (yyvsp[(3) - (5)].tval), (yyvsp[(5) - (5)].node), 0, yylineno);
            free((yyvsp[(2) - (5)].sval));
        ;}
    break;

  case 16:

/* Line 1455 of yacc.c  */
#line 114 "parser.y"
    {
            (yyval.node) = ast_new_var_decl((yyvsp[(2) - (3)].sval), (yyvsp[(3) - (3)].tval), NULL, 0, yylineno);
            free((yyvsp[(2) - (3)].sval));
        ;}
    break;

  case 17:

/* Line 1455 of yacc.c  */
#line 119 "parser.y"
    {
            (yyval.node) = ast_new_var_decl((yyvsp[(2) - (5)].sval), (yyvsp[(3) - (5)].tval), (yyvsp[(5) - (5)].node), 1, yylineno);
            free((yyvsp[(2) - (5)].sval));
        ;}
    break;

  case 18:

/* Line 1455 of yacc.c  */
#line 124 "parser.y"
    {
            (yyval.node) = ast_new_var_decl((yyvsp[(2) - (5)].sval), (yyvsp[(3) - (5)].tval), (yyvsp[(5) - (5)].node), 0, yylineno);
            free((yyvsp[(2) - (5)].sval));
        ;}
    break;

  case 19:

/* Line 1455 of yacc.c  */
#line 129 "parser.y"
    {
            (yyval.node) = ast_new_var_decl((yyvsp[(2) - (3)].sval), (yyvsp[(3) - (3)].tval), NULL, 0, yylineno);
            free((yyvsp[(2) - (3)].sval));
        ;}
    break;

  case 20:

/* Line 1455 of yacc.c  */
#line 137 "parser.y"
    { (yyval.tval) = TYPE_UNKNOWN; ;}
    break;

  case 21:

/* Line 1455 of yacc.c  */
#line 138 "parser.y"
    { (yyval.tval) = (yyvsp[(2) - (2)].tval); ;}
    break;

  case 22:

/* Line 1455 of yacc.c  */
#line 142 "parser.y"
    { (yyval.tval) = TYPE_NUMBER;  ;}
    break;

  case 23:

/* Line 1455 of yacc.c  */
#line 143 "parser.y"
    { (yyval.tval) = TYPE_BOOLEAN; ;}
    break;

  case 24:

/* Line 1455 of yacc.c  */
#line 144 "parser.y"
    { (yyval.tval) = TYPE_STRING;  ;}
    break;

  case 25:

/* Line 1455 of yacc.c  */
#line 145 "parser.y"
    { (yyval.tval) = TYPE_VOID;    ;}
    break;

  case 26:

/* Line 1455 of yacc.c  */
#line 151 "parser.y"
    {
            (yyval.node) = ast_new_func_decl((yyvsp[(2) - (7)].sval), (yyvsp[(6) - (7)].tval), (yyvsp[(4) - (7)].node), (yyvsp[(7) - (7)].node), yylineno);
            free((yyvsp[(2) - (7)].sval));
        ;}
    break;

  case 27:

/* Line 1455 of yacc.c  */
#line 158 "parser.y"
    { (yyval.tval) = TYPE_VOID; ;}
    break;

  case 28:

/* Line 1455 of yacc.c  */
#line 159 "parser.y"
    { (yyval.tval) = (yyvsp[(2) - (2)].tval); ;}
    break;

  case 29:

/* Line 1455 of yacc.c  */
#line 163 "parser.y"
    { (yyval.node) = NULL; ;}
    break;

  case 30:

/* Line 1455 of yacc.c  */
#line 164 "parser.y"
    { (yyval.node) = (yyvsp[(1) - (1)].node);   ;}
    break;

  case 31:

/* Line 1455 of yacc.c  */
#line 168 "parser.y"
    { (yyval.node) = (yyvsp[(1) - (1)].node); ;}
    break;

  case 32:

/* Line 1455 of yacc.c  */
#line 170 "parser.y"
    {
            ASTNode *cur = (yyvsp[(1) - (3)].node);
            while (cur->next) cur = cur->next;
            cur->next = (yyvsp[(3) - (3)].node);
            (yyval.node) = (yyvsp[(1) - (3)].node);
        ;}
    break;

  case 33:

/* Line 1455 of yacc.c  */
#line 180 "parser.y"
    {
            (yyval.node) = ast_new_param((yyvsp[(1) - (2)].sval), (yyvsp[(2) - (2)].tval), yylineno);
            free((yyvsp[(1) - (2)].sval));
        ;}
    break;

  case 34:

/* Line 1455 of yacc.c  */
#line 188 "parser.y"
    { (yyval.node) = ast_new_block((yyvsp[(2) - (3)].node), yylineno); ;}
    break;

  case 35:

/* Line 1455 of yacc.c  */
#line 193 "parser.y"
    { (yyval.node) = ast_new_return((yyvsp[(2) - (2)].node), yylineno); ;}
    break;

  case 36:

/* Line 1455 of yacc.c  */
#line 194 "parser.y"
    { (yyval.node) = ast_new_return(NULL, yylineno); ;}
    break;

  case 37:

/* Line 1455 of yacc.c  */
#line 200 "parser.y"
    {
            (yyval.node) = ast_new_if((yyvsp[(3) - (5)].node), (yyvsp[(5) - (5)].node), NULL, yylineno);
        ;}
    break;

  case 38:

/* Line 1455 of yacc.c  */
#line 204 "parser.y"
    {
            (yyval.node) = ast_new_if((yyvsp[(3) - (7)].node), (yyvsp[(5) - (7)].node), (yyvsp[(7) - (7)].node), yylineno);
        ;}
    break;

  case 39:

/* Line 1455 of yacc.c  */
#line 208 "parser.y"
    {
            (yyval.node) = ast_new_if((yyvsp[(3) - (7)].node), (yyvsp[(5) - (7)].node), (yyvsp[(7) - (7)].node), yylineno);
        ;}
    break;

  case 40:

/* Line 1455 of yacc.c  */
#line 216 "parser.y"
    {
            (yyval.node) = ast_new_while((yyvsp[(3) - (5)].node), (yyvsp[(5) - (5)].node), yylineno);
        ;}
    break;

  case 41:

/* Line 1455 of yacc.c  */
#line 224 "parser.y"
    {
            (yyval.node) = ast_new_for((yyvsp[(3) - (9)].node), (yyvsp[(5) - (9)].node), (yyvsp[(7) - (9)].node), (yyvsp[(9) - (9)].node), yylineno);
        ;}
    break;

  case 42:

/* Line 1455 of yacc.c  */
#line 228 "parser.y"
    {
            (yyval.node) = ast_new_for((yyvsp[(3) - (9)].node), (yyvsp[(5) - (9)].node), (yyvsp[(7) - (9)].node), (yyvsp[(9) - (9)].node), yylineno);
        ;}
    break;

  case 43:

/* Line 1455 of yacc.c  */
#line 236 "parser.y"
    {
            (yyval.node) = ast_new_print((yyvsp[(3) - (4)].node), yylineno);
        ;}
    break;

  case 44:

/* Line 1455 of yacc.c  */
#line 244 "parser.y"
    {
            ASTNode *n = ast_new_node(NODE_EXPR_STMT);
            n->left    = (yyvsp[(1) - (1)].node);
            n->line    = yylineno;
            (yyval.node) = n;
        ;}
    break;

  case 45:

/* Line 1455 of yacc.c  */
#line 254 "parser.y"
    { (yyval.node) = NULL; ;}
    break;

  case 46:

/* Line 1455 of yacc.c  */
#line 255 "parser.y"
    { (yyval.node) = (yyvsp[(1) - (1)].node);   ;}
    break;

  case 47:

/* Line 1455 of yacc.c  */
#line 259 "parser.y"
    { (yyval.node) = (yyvsp[(1) - (1)].node); ;}
    break;

  case 48:

/* Line 1455 of yacc.c  */
#line 261 "parser.y"
    {
            ASTNode *cur = (yyvsp[(1) - (3)].node);
            while (cur->next) cur = cur->next;
            cur->next = (yyvsp[(3) - (3)].node);
            (yyval.node) = (yyvsp[(1) - (3)].node);
        ;}
    break;

  case 49:

/* Line 1455 of yacc.c  */
#line 270 "parser.y"
    { (yyval.node) = (yyvsp[(1) - (1)].node); ;}
    break;

  case 50:

/* Line 1455 of yacc.c  */
#line 273 "parser.y"
    { (yyval.node) = (yyvsp[(1) - (1)].node); ;}
    break;

  case 51:

/* Line 1455 of yacc.c  */
#line 275 "parser.y"
    {
            ASTNode *id = ast_new_ident((yyvsp[(1) - (3)].sval), yylineno);
            (yyval.node) = ast_new_assign(id, (yyvsp[(3) - (3)].node), yylineno);
            free((yyvsp[(1) - (3)].sval));
        ;}
    break;

  case 52:

/* Line 1455 of yacc.c  */
#line 283 "parser.y"
    { (yyval.node) = (yyvsp[(1) - (1)].node); ;}
    break;

  case 53:

/* Line 1455 of yacc.c  */
#line 284 "parser.y"
    { (yyval.node) = ast_new_binop("||", (yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node), yylineno); ;}
    break;

  case 54:

/* Line 1455 of yacc.c  */
#line 288 "parser.y"
    { (yyval.node) = (yyvsp[(1) - (1)].node); ;}
    break;

  case 55:

/* Line 1455 of yacc.c  */
#line 289 "parser.y"
    { (yyval.node) = ast_new_binop("&&", (yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node), yylineno); ;}
    break;

  case 56:

/* Line 1455 of yacc.c  */
#line 293 "parser.y"
    { (yyval.node) = (yyvsp[(1) - (1)].node); ;}
    break;

  case 57:

/* Line 1455 of yacc.c  */
#line 294 "parser.y"
    { (yyval.node) = ast_new_binop("==", (yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node), yylineno); ;}
    break;

  case 58:

/* Line 1455 of yacc.c  */
#line 295 "parser.y"
    { (yyval.node) = ast_new_binop("!=", (yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node), yylineno); ;}
    break;

  case 59:

/* Line 1455 of yacc.c  */
#line 299 "parser.y"
    { (yyval.node) = (yyvsp[(1) - (1)].node); ;}
    break;

  case 60:

/* Line 1455 of yacc.c  */
#line 300 "parser.y"
    { (yyval.node) = ast_new_binop("<",  (yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node), yylineno); ;}
    break;

  case 61:

/* Line 1455 of yacc.c  */
#line 301 "parser.y"
    { (yyval.node) = ast_new_binop(">",  (yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node), yylineno); ;}
    break;

  case 62:

/* Line 1455 of yacc.c  */
#line 302 "parser.y"
    { (yyval.node) = ast_new_binop("<=", (yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node), yylineno); ;}
    break;

  case 63:

/* Line 1455 of yacc.c  */
#line 303 "parser.y"
    { (yyval.node) = ast_new_binop(">=", (yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node), yylineno); ;}
    break;

  case 64:

/* Line 1455 of yacc.c  */
#line 307 "parser.y"
    { (yyval.node) = (yyvsp[(1) - (1)].node); ;}
    break;

  case 65:

/* Line 1455 of yacc.c  */
#line 308 "parser.y"
    { (yyval.node) = ast_new_binop("+",  (yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node), yylineno); ;}
    break;

  case 66:

/* Line 1455 of yacc.c  */
#line 309 "parser.y"
    { (yyval.node) = ast_new_binop("-",  (yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node), yylineno); ;}
    break;

  case 67:

/* Line 1455 of yacc.c  */
#line 313 "parser.y"
    { (yyval.node) = (yyvsp[(1) - (1)].node); ;}
    break;

  case 68:

/* Line 1455 of yacc.c  */
#line 314 "parser.y"
    { (yyval.node) = ast_new_binop("*",  (yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node), yylineno); ;}
    break;

  case 69:

/* Line 1455 of yacc.c  */
#line 315 "parser.y"
    { (yyval.node) = ast_new_binop("/",  (yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node), yylineno); ;}
    break;

  case 70:

/* Line 1455 of yacc.c  */
#line 316 "parser.y"
    { (yyval.node) = ast_new_binop("%",  (yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node), yylineno); ;}
    break;

  case 71:

/* Line 1455 of yacc.c  */
#line 320 "parser.y"
    { (yyval.node) = (yyvsp[(1) - (1)].node); ;}
    break;

  case 72:

/* Line 1455 of yacc.c  */
#line 321 "parser.y"
    { (yyval.node) = ast_new_unop("!",  (yyvsp[(2) - (2)].node), yylineno); ;}
    break;

  case 73:

/* Line 1455 of yacc.c  */
#line 323 "parser.y"
    { (yyval.node) = ast_new_unop("-",  (yyvsp[(2) - (2)].node), yylineno); ;}
    break;

  case 74:

/* Line 1455 of yacc.c  */
#line 328 "parser.y"
    { (yyval.node) = ast_new_number((yyvsp[(1) - (1)].dval), yylineno); ;}
    break;

  case 75:

/* Line 1455 of yacc.c  */
#line 330 "parser.y"
    { (yyval.node) = ast_new_string((yyvsp[(1) - (1)].sval), yylineno); free((yyvsp[(1) - (1)].sval)); ;}
    break;

  case 76:

/* Line 1455 of yacc.c  */
#line 332 "parser.y"
    { (yyval.node) = ast_new_bool(1, yylineno); ;}
    break;

  case 77:

/* Line 1455 of yacc.c  */
#line 334 "parser.y"
    { (yyval.node) = ast_new_bool(0, yylineno); ;}
    break;

  case 78:

/* Line 1455 of yacc.c  */
#line 336 "parser.y"
    { (yyval.node) = ast_new_null(yylineno); ;}
    break;

  case 79:

/* Line 1455 of yacc.c  */
#line 338 "parser.y"
    { (yyval.node) = ast_new_ident((yyvsp[(1) - (1)].sval), yylineno); free((yyvsp[(1) - (1)].sval)); ;}
    break;

  case 80:

/* Line 1455 of yacc.c  */
#line 340 "parser.y"
    { (yyval.node) = ast_new_call((yyvsp[(1) - (4)].sval), (yyvsp[(3) - (4)].node), yylineno); free((yyvsp[(1) - (4)].sval)); ;}
    break;

  case 81:

/* Line 1455 of yacc.c  */
#line 342 "parser.y"
    { (yyval.node) = (yyvsp[(2) - (3)].node); ;}
    break;



/* Line 1455 of yacc.c  */
#line 2152 "parser.tab.c"
      default: break;
    }
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;

  /* Now `shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*------------------------------------.
| yyerrlab -- here on detecting error |
`------------------------------------*/
yyerrlab:
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
      {
	YYSIZE_T yysize = yysyntax_error (0, yystate, yychar);
	if (yymsg_alloc < yysize && yymsg_alloc < YYSTACK_ALLOC_MAXIMUM)
	  {
	    YYSIZE_T yyalloc = 2 * yysize;
	    if (! (yysize <= yyalloc && yyalloc <= YYSTACK_ALLOC_MAXIMUM))
	      yyalloc = YYSTACK_ALLOC_MAXIMUM;
	    if (yymsg != yymsgbuf)
	      YYSTACK_FREE (yymsg);
	    yymsg = (char *) YYSTACK_ALLOC (yyalloc);
	    if (yymsg)
	      yymsg_alloc = yyalloc;
	    else
	      {
		yymsg = yymsgbuf;
		yymsg_alloc = sizeof yymsgbuf;
	      }
	  }

	if (0 < yysize && yysize <= yymsg_alloc)
	  {
	    (void) yysyntax_error (yymsg, yystate, yychar);
	    yyerror (yymsg);
	  }
	else
	  {
	    yyerror (YY_("syntax error"));
	    if (yysize != 0)
	      goto yyexhaustedlab;
	  }
      }
#endif
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
	 error, discard it.  */

      if (yychar <= YYEOF)
	{
	  /* Return failure if at end of input.  */
	  if (yychar == YYEOF)
	    YYABORT;
	}
      else
	{
	  yydestruct ("Error: discarding",
		      yytoken, &yylval);
	  yychar = YYEMPTY;
	}
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (/*CONSTCOND*/ 0)
     goto yyerrorlab;

  /* Do not reclaim the symbols of the rule which action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;	/* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (yyn != YYPACT_NINF)
	{
	  yyn += YYTERROR;
	  if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
	    {
	      yyn = yytable[yyn];
	      if (0 < yyn)
		break;
	    }
	}

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
	YYABORT;


      yydestruct ("Error: popping",
		  yystos[yystate], yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  *++yyvsp = yylval;


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;

#if !defined(yyoverflow) || YYERROR_VERBOSE
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEMPTY)
     yydestruct ("Cleanup: discarding lookahead",
		 yytoken, &yylval);
  /* Do not reclaim the symbols of the rule which action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
		  yystos[*yyssp], yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  /* Make sure YYID is used.  */
  return YYID (yyresult);
}



/* Line 1675 of yacc.c  */
#line 345 "parser.y"


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

