/* A Bison parser, made by GNU Bison 3.3.2.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2019 Free Software Foundation,
   Inc.

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

/* Undocumented macros, especially those whose name start with YY_,
   are private implementation details.  Do not rely on them.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "3.3.2"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* First part of user prologue.  */
#line 1 "tiger.y" /* yacc.c:337  */

#include <stdio.h>
#include "util.h"
#include "errormsg.h"
#include "symbol.h"
#include "absyn.h"

int yylex(void); /* function prototype */

A_exp absyn_root;

void yyerror(char *s)
{
 EM_error(EM_tokPos, "%s", s);
}

#line 87 "y.tab.c" /* yacc.c:337  */
# ifndef YY_NULLPTR
#  if defined __cplusplus
#   if 201103L <= __cplusplus
#    define YY_NULLPTR nullptr
#   else
#    define YY_NULLPTR 0
#   endif
#  else
#   define YY_NULLPTR ((void*)0)
#  endif
# endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* In a future release of Bison, this section will be replaced
   by #include "y.tab.h".  */
#ifndef YY_YY_Y_TAB_H_INCLUDED
# define YY_YY_Y_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    ID = 258,
    STRING = 259,
    INT = 260,
    COMMA = 261,
    COLON = 262,
    SEMICOLON = 263,
    LPAREN = 264,
    RPAREN = 265,
    LBRACK = 266,
    RBRACK = 267,
    LBRACE = 268,
    RBRACE = 269,
    DOT = 270,
    PLUS = 271,
    MINUS = 272,
    TIMES = 273,
    DIVIDE = 274,
    EQ = 275,
    NEQ = 276,
    LT = 277,
    LE = 278,
    GT = 279,
    GE = 280,
    AND = 281,
    OR = 282,
    ASSIGN = 283,
    ARRAY = 284,
    IF = 285,
    THEN = 286,
    ELSE = 287,
    WHILE = 288,
    FOR = 289,
    TO = 290,
    DO = 291,
    LET = 292,
    IN = 293,
    END = 294,
    OF = 295,
    BREAK = 296,
    NIL = 297,
    FUNCTION = 298,
    VAR = 299,
    TYPE = 300,
    LTS = 301,
    UMINUS = 302
  };
#endif
/* Tokens.  */
#define ID 258
#define STRING 259
#define INT 260
#define COMMA 261
#define COLON 262
#define SEMICOLON 263
#define LPAREN 264
#define RPAREN 265
#define LBRACK 266
#define RBRACK 267
#define LBRACE 268
#define RBRACE 269
#define DOT 270
#define PLUS 271
#define MINUS 272
#define TIMES 273
#define DIVIDE 274
#define EQ 275
#define NEQ 276
#define LT 277
#define LE 278
#define GT 279
#define GE 280
#define AND 281
#define OR 282
#define ASSIGN 283
#define ARRAY 284
#define IF 285
#define THEN 286
#define ELSE 287
#define WHILE 288
#define FOR 289
#define TO 290
#define DO 291
#define LET 292
#define IN 293
#define END 294
#define OF 295
#define BREAK 296
#define NIL 297
#define FUNCTION 298
#define VAR 299
#define TYPE 300
#define LTS 301
#define UMINUS 302

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED

union YYSTYPE
{
#line 19 "tiger.y" /* yacc.c:352  */

	int pos;
	int ival;
	string sval;
	A_exp exp;
	A_expList explist;
	A_var var;
	A_decList declist;
	A_dec  dec;
	A_efieldList efieldlist;
	A_efield  efield;
	A_namety namety;
	A_nametyList nametylist;
	A_fieldList fieldlist;
	A_field field;
	A_fundecList fundeclist;
	A_fundec fundec;
	A_ty ty;
	

#line 245 "y.tab.c" /* yacc.c:352  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_Y_TAB_H_INCLUDED  */



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
#else
typedef signed char yytype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 yytype_uint16;
#else
typedef unsigned short yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short yytype_int16;
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif ! defined YYSIZE_T
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif

#ifndef YY_ATTRIBUTE
# if (defined __GNUC__                                               \
      && (2 < __GNUC__ || (__GNUC__ == 2 && 96 <= __GNUC_MINOR__)))  \
     || defined __SUNPRO_C && 0x5110 <= __SUNPRO_C
#  define YY_ATTRIBUTE(Spec) __attribute__(Spec)
# else
#  define YY_ATTRIBUTE(Spec) /* empty */
# endif
#endif

#ifndef YY_ATTRIBUTE_PURE
# define YY_ATTRIBUTE_PURE   YY_ATTRIBUTE ((__pure__))
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# define YY_ATTRIBUTE_UNUSED YY_ATTRIBUTE ((__unused__))
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(E) ((void) (E))
#else
# define YYUSE(E) /* empty */
#endif

#if defined __GNUC__ && ! defined __ICC && 407 <= __GNUC__ * 100 + __GNUC_MINOR__
/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN \
    _Pragma ("GCC diagnostic push") \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")\
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# define YY_IGNORE_MAYBE_UNINITIALIZED_END \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
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
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
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
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
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

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYSIZE_T yynewbytes;                                            \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / sizeof (*yyptr);                          \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, (Count) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYSIZE_T yyi;                         \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  40
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   304

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  48
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  25
/* YYNRULES -- Number of rules.  */
#define YYNRULES  75
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  150

#define YYUNDEFTOK  2
#define YYMAXUTOK   302

/* YYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, with out-of-bounds checking.  */
#define YYTRANSLATE(YYX)                                                \
  ((unsigned) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
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
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint8 yyrline[] =
{
       0,    86,    86,    88,    89,    90,    91,    92,    93,    94,
      95,    96,    97,    98,    99,   100,   101,   102,   103,   104,
     105,   106,   107,   108,   109,   110,   111,   112,   113,   114,
     115,   116,   120,   121,   124,   125,   128,   129,   133,   137,
     138,   141,   142,   145,   146,   149,   150,   151,   154,   155,
     158,   159,   162,   163,   164,   167,   168,   171,   172,   175,
     176,   179,   182,   183,   186,   189,   190,   193,   194,   197,
     198,   199,   202,   203,   206,   207
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 0
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "ID", "STRING", "INT", "COMMA", "COLON",
  "SEMICOLON", "LPAREN", "RPAREN", "LBRACK", "RBRACK", "LBRACE", "RBRACE",
  "DOT", "PLUS", "MINUS", "TIMES", "DIVIDE", "EQ", "NEQ", "LT", "LE", "GT",
  "GE", "AND", "OR", "ASSIGN", "ARRAY", "IF", "THEN", "ELSE", "WHILE",
  "FOR", "TO", "DO", "LET", "IN", "END", "OF", "BREAK", "NIL", "FUNCTION",
  "VAR", "TYPE", "LTS", "UMINUS", "$accept", "program", "exp", "expseq",
  "actuals", "nonemptyactuals", "sequencing", "sequencing_exps", "lvalue",
  "one", "oneormore", "decs", "decs_nonempty", "decs_nonempty_s", "vardec",
  "rec", "rec_nonempty", "rec_one", "tydec", "tydec_one", "tyfields",
  "tyfields_nonempty", "ty", "fundec", "fundec_one", YY_NULLPTR
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[NUM] -- (External) token number corresponding to the
   (internal) symbol number NUM (which must be that of a token).  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,   301,   302
};
# endif

#define YYPACT_NINF -58

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-58)))

#define YYTABLE_NINF -1

#define yytable_value_is_error(Yytable_value) \
  (!!((Yytable_value) == (-1)))

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
      57,    11,   -58,   -58,    57,    57,    57,    57,     6,    20,
     -58,   -58,    15,   246,    -7,   -58,    -1,    57,    57,    22,
      25,   133,    45,    46,   -58,   -58,   202,    59,    30,    56,
      64,    68,    34,   -58,    20,   -58,   -58,    28,   -58,    49,
     -58,    57,    57,    57,    57,    57,    57,    57,    57,    57,
      57,    57,    57,    57,    57,    85,    13,    79,   -58,   218,
      77,    86,   -58,    95,   -58,    57,   -58,   -58,    57,    57,
      57,    93,    -5,    83,    57,   -58,   -58,   -58,    -6,    -6,
     -58,   -58,   269,   269,   269,   269,   269,   269,   279,   258,
     246,   234,   -58,    57,   -58,    65,    57,   -58,    22,   153,
     -58,   185,   246,   165,   104,   105,    57,    -2,    70,   -58,
     -58,    57,   246,   -58,    57,    57,    57,    97,   100,   -58,
      84,   246,   -58,   104,    73,   -58,   -58,   246,   -58,   246,
     101,   111,    -4,    57,   115,   127,    57,   125,   129,    57,
     246,   -58,   -58,   246,   104,   114,   246,   -58,    57,   246
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,    41,     6,     5,    32,     0,     0,     0,     0,    48,
      26,     4,     0,     2,     3,    45,    42,    34,     0,    57,
       0,    39,     0,     0,    33,     7,     0,     0,     0,     0,
       0,     0,     0,    49,    50,    53,    52,    62,    54,    72,
       1,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    36,     0,    35,     0,
       0,     0,    58,    59,    43,     0,     8,    21,     0,     0,
       0,     0,     0,     0,    32,    51,    63,    73,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    30,    31,
      22,     0,    46,     0,     9,    44,     0,    20,     0,    39,
      38,    24,    25,     0,    65,     0,     0,     0,     0,    47,
      37,     0,    61,    60,     0,     0,     0,     0,     0,    66,
       0,    55,    69,    65,     0,    64,    28,    29,    40,    23,
       0,     0,     0,     0,     0,     0,     0,    67,     0,     0,
      56,    70,    71,    27,     0,     0,    74,    68,     0,    75
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int8 yypgoto[] =
{
     -58,   -58,     0,    61,   -58,    47,   -58,   -57,   -58,   -58,
     -58,   -58,   108,   -58,   -58,   -58,    40,   -58,   106,   -58,
      21,     1,   -58,   107,   -58
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int8 yydefgoto[] =
{
      -1,    12,    99,    22,    57,    58,    23,    24,    14,    15,
      16,    32,    33,    34,    35,    61,    62,    63,    36,    37,
     118,   119,   125,    38,    39
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      13,   122,   105,   138,    21,    25,    26,    27,   100,    28,
      54,   123,    43,    44,    55,    40,   139,    56,    59,    93,
      17,    53,    18,   106,    19,    60,    20,   124,    64,    41,
      42,    43,    44,    45,    46,    47,    48,    49,    50,    51,
      52,    78,    79,    80,    81,    82,    83,    84,    85,    86,
      87,    88,    89,    90,    91,    66,    67,   128,    70,    71,
       1,     2,     3,    29,    30,    31,     4,    72,   101,   102,
     103,    73,    74,    31,     5,    41,    42,    43,    44,    45,
      46,    47,    48,    49,    50,    51,    52,     6,    92,    94,
       7,     8,    29,    56,     9,    69,   112,    96,    10,    11,
      97,    98,   104,   107,   131,   111,   121,   117,   120,   126,
     132,   127,   133,   135,   137,   129,   130,    41,    42,    43,
      44,    45,    46,    47,    48,    49,    50,    51,    52,   141,
     142,   144,   145,   140,   148,   108,   143,   136,   113,   146,
     110,    65,    75,    76,   134,   147,    77,     0,   149,    41,
      42,    43,    44,    45,    46,    47,    48,    49,    50,    51,
      52,   114,     0,     0,     0,     0,     0,     0,     0,    41,
      42,    43,    44,    45,    46,    47,    48,    49,    50,    51,
      52,    41,    42,    43,    44,    45,    46,    47,    48,    49,
      50,    51,    52,     0,     0,     0,     0,     0,     0,     0,
     116,    41,    42,    43,    44,    45,    46,    47,    48,    49,
      50,    51,    52,     0,     0,     0,     0,   115,    41,    42,
      43,    44,    45,    46,    47,    48,    49,    50,    51,    52,
      95,     0,     0,    68,    41,    42,    43,    44,    45,    46,
      47,    48,    49,    50,    51,    52,   109,     0,     0,     0,
      41,    42,    43,    44,    45,    46,    47,    48,    49,    50,
      51,    52,    41,    42,    43,    44,    45,    46,    47,    48,
      49,    50,    51,    52,    41,    42,    43,    44,    45,    46,
      47,    48,    49,    50,    51,    41,    42,    43,    44,    -1,
      -1,    -1,    -1,    -1,    -1,    41,    42,    43,    44,    45,
      46,    47,    48,    49,    50
};

static const yytype_int16 yycheck[] =
{
       0,     3,     7,     7,     4,     5,     6,     7,    65,     3,
      11,    13,    18,    19,    15,     0,    20,    17,    18,     6,
       9,    28,    11,    28,    13,     3,    15,    29,     3,    16,
      17,    18,    19,    20,    21,    22,    23,    24,    25,    26,
      27,    41,    42,    43,    44,    45,    46,    47,    48,    49,
      50,    51,    52,    53,    54,    10,    10,   114,    28,     3,
       3,     4,     5,    43,    44,    45,     9,     3,    68,    69,
      70,     3,    38,    45,    17,    16,    17,    18,    19,    20,
      21,    22,    23,    24,    25,    26,    27,    30,     3,    10,
      33,    34,    43,    93,    37,    36,    96,    20,    41,    42,
      14,     6,     9,    20,     7,    40,   106,     3,     3,    39,
      10,   111,    28,    40,     3,   115,   116,    16,    17,    18,
      19,    20,    21,    22,    23,    24,    25,    26,    27,    14,
       3,     6,     3,   133,    20,    74,   136,    36,    98,   139,
      93,     8,    34,    37,   123,   144,    39,    -1,   148,    16,
      17,    18,    19,    20,    21,    22,    23,    24,    25,    26,
      27,     8,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    16,
      17,    18,    19,    20,    21,    22,    23,    24,    25,    26,
      27,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      35,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    -1,    -1,    -1,    -1,    32,    16,    17,
      18,    19,    20,    21,    22,    23,    24,    25,    26,    27,
      12,    -1,    -1,    31,    16,    17,    18,    19,    20,    21,
      22,    23,    24,    25,    26,    27,    12,    -1,    -1,    -1,
      16,    17,    18,    19,    20,    21,    22,    23,    24,    25,
      26,    27,    16,    17,    18,    19,    20,    21,    22,    23,
      24,    25,    26,    27,    16,    17,    18,    19,    20,    21,
      22,    23,    24,    25,    26,    16,    17,    18,    19,    20,
      21,    22,    23,    24,    25,    16,    17,    18,    19,    20,
      21,    22,    23,    24,    25
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     3,     4,     5,     9,    17,    30,    33,    34,    37,
      41,    42,    49,    50,    56,    57,    58,     9,    11,    13,
      15,    50,    51,    54,    55,    50,    50,    50,     3,    43,
      44,    45,    59,    60,    61,    62,    66,    67,    71,    72,
       0,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    11,    15,    50,    52,    53,    50,
       3,    63,    64,    65,     3,     8,    10,    10,    31,    36,
      28,     3,     3,     3,    38,    60,    66,    71,    50,    50,
      50,    50,    50,    50,    50,    50,    50,    50,    50,    50,
      50,    50,     3,     6,    10,    12,    20,    14,     6,    50,
      55,    50,    50,    50,     9,     7,    28,    20,    51,    12,
      53,    40,    50,    64,     8,    32,    35,     3,    68,    69,
       3,    50,     3,    13,    29,    70,    39,    50,    55,    50,
      50,     7,    10,    28,    68,    40,    36,     3,     7,    20,
      50,    14,     3,    50,     6,     3,    50,    69,    20,    50
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    48,    49,    50,    50,    50,    50,    50,    50,    50,
      50,    50,    50,    50,    50,    50,    50,    50,    50,    50,
      50,    50,    50,    50,    50,    50,    50,    50,    50,    50,
      50,    50,    51,    51,    52,    52,    53,    53,    54,    55,
      55,    56,    56,    57,    57,    58,    58,    58,    59,    59,
      60,    60,    61,    61,    61,    62,    62,    63,    63,    64,
      64,    65,    66,    66,    67,    68,    68,    69,    69,    70,
      70,    70,    71,    71,    72,    72
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     1,     1,     1,     1,     1,     2,     3,     4,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       4,     3,     3,     6,     4,     4,     1,     8,     5,     6,
       3,     3,     0,     1,     0,     1,     1,     3,     3,     1,
       3,     1,     1,     3,     4,     1,     3,     4,     0,     1,
       1,     2,     1,     1,     1,     4,     6,     0,     1,     1,
       3,     3,     1,     2,     4,     0,     1,     3,     5,     1,
       3,     3,     1,     2,     7,     9
};


#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)
#define YYEMPTY         (-2)
#define YYEOF           0

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                    \
  do                                                              \
    if (yychar == YYEMPTY)                                        \
      {                                                           \
        yychar = (Token);                                         \
        yylval = (Value);                                         \
        YYPOPSTACK (yylen);                                       \
        yystate = *yyssp;                                         \
        goto yybackup;                                            \
      }                                                           \
    else                                                          \
      {                                                           \
        yyerror (YY_("syntax error: cannot back up")); \
        YYERROR;                                                  \
      }                                                           \
  while (0)

/* Error token number */
#define YYTERROR        1
#define YYERRCODE       256



/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)

/* This macro is provided for backward compatibility. */
#ifndef YY_LOCATION_PRINT
# define YY_LOCATION_PRINT(File, Loc) ((void) 0)
#endif


# define YY_SYMBOL_PRINT(Title, Type, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Type, Value); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo, int yytype, YYSTYPE const * const yyvaluep)
{
  FILE *yyoutput = yyo;
  YYUSE (yyoutput);
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyo, yytoknum[yytype], *yyvaluep);
# endif
  YYUSE (yytype);
}


/*---------------------------.
| Print this symbol on YYO.  |
`---------------------------*/

static void
yy_symbol_print (FILE *yyo, int yytype, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyo, "%s %s (",
             yytype < YYNTOKENS ? "token" : "nterm", yytname[yytype]);

  yy_symbol_value_print (yyo, yytype, yyvaluep);
  YYFPRINTF (yyo, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yytype_int16 *yybottom, yytype_int16 *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yytype_int16 *yyssp, YYSTYPE *yyvsp, int yyrule)
{
  unsigned long yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       yystos[yyssp[yyi + 1 - yynrhs]],
                       &yyvsp[(yyi + 1) - (yynrhs)]
                                              );
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, Rule); \
} while (0)

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
#ifndef YYINITDEPTH
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
static YYSIZE_T
yystrlen (const char *yystr)
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
static char *
yystpcpy (char *yydest, const char *yysrc)
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
            else
              goto append;

          append:
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

  return (YYSIZE_T) (yystpcpy (yyres, yystr) - yyres);
}
# endif

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
   about the unexpected token YYTOKEN for the state stack whose top is
   YYSSP.

   Return 0 if *YYMSG was successfully written.  Return 1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return 2 if the
   required number of bytes is too large to store.  */
static int
yysyntax_error (YYSIZE_T *yymsg_alloc, char **yymsg,
                yytype_int16 *yyssp, int yytoken)
{
  YYSIZE_T yysize0 = yytnamerr (YY_NULLPTR, yytname[yytoken]);
  YYSIZE_T yysize = yysize0;
  enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
  /* Internationalized format string. */
  const char *yyformat = YY_NULLPTR;
  /* Arguments of yyformat. */
  char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
  /* Number of reported tokens (one for the "unexpected", one per
     "expected"). */
  int yycount = 0;

  /* There are many possibilities here to consider:
     - If this state is a consistent state with a default action, then
       the only way this function was invoked is if the default action
       is an error action.  In that case, don't check for expected
       tokens because there are none.
     - The only way there can be no lookahead present (in yychar) is if
       this state is a consistent state with a default action.  Thus,
       detecting the absence of a lookahead is sufficient to determine
       that there is no unexpected or expected token to report.  In that
       case, just report a simple "syntax error".
     - Don't assume there isn't a lookahead just because this state is a
       consistent state with a default action.  There might have been a
       previous inconsistent state, consistent state with a non-default
       action, or user semantic action that manipulated yychar.
     - Of course, the expected token list depends on states to have
       correct lookahead information, and it depends on the parser not
       to perform extra reductions after fetching a lookahead from the
       scanner and before detecting a syntax error.  Thus, state merging
       (from LALR or IELR) and default reductions corrupt the expected
       token list.  However, the list is correct for canonical LR with
       one exception: it will still contain any token that will not be
       accepted due to an error action in a later state.
  */
  if (yytoken != YYEMPTY)
    {
      int yyn = yypact[*yyssp];
      yyarg[yycount++] = yytname[yytoken];
      if (!yypact_value_is_default (yyn))
        {
          /* Start YYX at -YYN if negative to avoid negative indexes in
             YYCHECK.  In other words, skip the first -YYN actions for
             this state because they are default actions.  */
          int yyxbegin = yyn < 0 ? -yyn : 0;
          /* Stay within bounds of both yycheck and yytname.  */
          int yychecklim = YYLAST - yyn + 1;
          int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
          int yyx;

          for (yyx = yyxbegin; yyx < yyxend; ++yyx)
            if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR
                && !yytable_value_is_error (yytable[yyx + yyn]))
              {
                if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
                  {
                    yycount = 1;
                    yysize = yysize0;
                    break;
                  }
                yyarg[yycount++] = yytname[yyx];
                {
                  YYSIZE_T yysize1 = yysize + yytnamerr (YY_NULLPTR, yytname[yyx]);
                  if (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM)
                    yysize = yysize1;
                  else
                    return 2;
                }
              }
        }
    }

  switch (yycount)
    {
# define YYCASE_(N, S)                      \
      case N:                               \
        yyformat = S;                       \
      break
    default: /* Avoid compiler warnings. */
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
# undef YYCASE_
    }

  {
    YYSIZE_T yysize1 = yysize + yystrlen (yyformat);
    if (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM)
      yysize = yysize1;
    else
      return 2;
  }

  if (*yymsg_alloc < yysize)
    {
      *yymsg_alloc = 2 * yysize;
      if (! (yysize <= *yymsg_alloc
             && *yymsg_alloc <= YYSTACK_ALLOC_MAXIMUM))
        *yymsg_alloc = YYSTACK_ALLOC_MAXIMUM;
      return 1;
    }

  /* Avoid sprintf, as that infringes on the user's name space.
     Don't have undefined behavior even if the translation
     produced a string with the wrong number of "%s"s.  */
  {
    char *yyp = *yymsg;
    int yyi = 0;
    while ((*yyp = *yyformat) != '\0')
      if (*yyp == '%' && yyformat[1] == 's' && yyi < yycount)
        {
          yyp += yytnamerr (yyp, yyarg[yyi++]);
          yyformat += 2;
        }
      else
        {
          yyp++;
          yyformat++;
        }
  }
  return 0;
}
#endif /* YYERROR_VERBOSE */

/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
{
  YYUSE (yyvaluep);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YYUSE (yytype);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}




/* The lookahead symbol.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Number of syntax errors so far.  */
int yynerrs;


/*----------.
| yyparse.  |
`----------*/

int
yyparse (void)
{
    int yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       'yyss': related to states.
       'yyvs': related to semantic values.

       Refer to the stacks through separate pointers, to allow yyoverflow
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
  int yytoken = 0;
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

  yyssp = yyss = yyssa;
  yyvsp = yyvs = yyvsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */
  goto yysetstate;


/*------------------------------------------------------------.
| yynewstate -- push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;


/*--------------------------------------------------------------------.
| yynewstate -- set current state (the top of the stack) to yystate.  |
`--------------------------------------------------------------------*/
yysetstate:
  *yyssp = (yytype_int16) yystate;

  if (yyss + yystacksize - 1 <= yyssp)
#if !defined yyoverflow && !defined YYSTACK_RELOCATE
    goto yyexhaustedlab;
#else
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = (YYSIZE_T) (yyssp - yyss + 1);

# if defined yyoverflow
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
# else /* defined YYSTACK_RELOCATE */
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
# undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
                  (unsigned long) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }
#endif /* !defined yyoverflow && !defined YYSTACK_RELOCATE */

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
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = yylex ();
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
      if (yytable_value_is_error (yyn))
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
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

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
| yyreduce -- do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

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
#line 86 "tiger.y" /* yacc.c:1652  */
    {absyn_root = (yyvsp[0].exp);}
#line 1469 "y.tab.c" /* yacc.c:1652  */
    break;

  case 3:
#line 88 "tiger.y" /* yacc.c:1652  */
    {(yyval.exp) = A_VarExp(EM_tokPos, (yyvsp[0].var));}
#line 1475 "y.tab.c" /* yacc.c:1652  */
    break;

  case 4:
#line 89 "tiger.y" /* yacc.c:1652  */
    {(yyval.exp) = A_NilExp(EM_tokPos);}
#line 1481 "y.tab.c" /* yacc.c:1652  */
    break;

  case 5:
#line 90 "tiger.y" /* yacc.c:1652  */
    {(yyval.exp) = A_IntExp(EM_tokPos, (yyvsp[0].ival));}
#line 1487 "y.tab.c" /* yacc.c:1652  */
    break;

  case 6:
#line 91 "tiger.y" /* yacc.c:1652  */
    {(yyval.exp) = A_StringExp(EM_tokPos, (yyvsp[0].sval));}
#line 1493 "y.tab.c" /* yacc.c:1652  */
    break;

  case 7:
#line 92 "tiger.y" /* yacc.c:1652  */
    {(yyval.exp) = A_OpExp(EM_tokPos, A_minusOp, A_IntExp(EM_tokPos, 0), (yyvsp[0].exp));}
#line 1499 "y.tab.c" /* yacc.c:1652  */
    break;

  case 8:
#line 93 "tiger.y" /* yacc.c:1652  */
    {(yyval.exp) = (yyvsp[-1].exp);}
#line 1505 "y.tab.c" /* yacc.c:1652  */
    break;

  case 9:
#line 94 "tiger.y" /* yacc.c:1652  */
    {(yyval.exp) = A_CallExp(EM_tokPos, S_Symbol((yyvsp[-3].sval)), (yyvsp[-1].explist));}
#line 1511 "y.tab.c" /* yacc.c:1652  */
    break;

  case 10:
#line 95 "tiger.y" /* yacc.c:1652  */
    {(yyval.exp) = A_OpExp(EM_tokPos, A_plusOp, (yyvsp[-2].exp), (yyvsp[0].exp));}
#line 1517 "y.tab.c" /* yacc.c:1652  */
    break;

  case 11:
#line 96 "tiger.y" /* yacc.c:1652  */
    {(yyval.exp) = A_OpExp(EM_tokPos, A_minusOp, (yyvsp[-2].exp), (yyvsp[0].exp));}
#line 1523 "y.tab.c" /* yacc.c:1652  */
    break;

  case 12:
#line 97 "tiger.y" /* yacc.c:1652  */
    {(yyval.exp) = A_OpExp(EM_tokPos, A_timesOp, (yyvsp[-2].exp), (yyvsp[0].exp));}
#line 1529 "y.tab.c" /* yacc.c:1652  */
    break;

  case 13:
#line 98 "tiger.y" /* yacc.c:1652  */
    {(yyval.exp) = A_OpExp(EM_tokPos, A_divideOp, (yyvsp[-2].exp), (yyvsp[0].exp));}
#line 1535 "y.tab.c" /* yacc.c:1652  */
    break;

  case 14:
#line 99 "tiger.y" /* yacc.c:1652  */
    {(yyval.exp) = A_OpExp(EM_tokPos, A_eqOp, (yyvsp[-2].exp), (yyvsp[0].exp));}
#line 1541 "y.tab.c" /* yacc.c:1652  */
    break;

  case 15:
#line 100 "tiger.y" /* yacc.c:1652  */
    {(yyval.exp) = A_OpExp(EM_tokPos, A_neqOp, (yyvsp[-2].exp), (yyvsp[0].exp));}
#line 1547 "y.tab.c" /* yacc.c:1652  */
    break;

  case 16:
#line 101 "tiger.y" /* yacc.c:1652  */
    {(yyval.exp) = A_OpExp(EM_tokPos, A_ltOp, (yyvsp[-2].exp), (yyvsp[0].exp));}
#line 1553 "y.tab.c" /* yacc.c:1652  */
    break;

  case 17:
#line 102 "tiger.y" /* yacc.c:1652  */
    {(yyval.exp) = A_OpExp(EM_tokPos, A_leOp, (yyvsp[-2].exp), (yyvsp[0].exp));}
#line 1559 "y.tab.c" /* yacc.c:1652  */
    break;

  case 18:
#line 103 "tiger.y" /* yacc.c:1652  */
    {(yyval.exp) = A_OpExp(EM_tokPos, A_gtOp, (yyvsp[-2].exp), (yyvsp[0].exp));}
#line 1565 "y.tab.c" /* yacc.c:1652  */
    break;

  case 19:
#line 104 "tiger.y" /* yacc.c:1652  */
    {(yyval.exp) = A_OpExp(EM_tokPos, A_geOp, (yyvsp[-2].exp), (yyvsp[0].exp));}
#line 1571 "y.tab.c" /* yacc.c:1652  */
    break;

  case 20:
#line 105 "tiger.y" /* yacc.c:1652  */
    {(yyval.exp) = A_RecordExp(EM_tokPos, S_Symbol((yyvsp[-3].sval)), (yyvsp[-1].efieldlist));}
#line 1577 "y.tab.c" /* yacc.c:1652  */
    break;

  case 21:
#line 106 "tiger.y" /* yacc.c:1652  */
    {(yyval.exp) = A_SeqExp(EM_tokPos, (yyvsp[-1].explist));}
#line 1583 "y.tab.c" /* yacc.c:1652  */
    break;

  case 22:
#line 107 "tiger.y" /* yacc.c:1652  */
    {(yyval.exp) = A_AssignExp(EM_tokPos, (yyvsp[-2].var), (yyvsp[0].exp));}
#line 1589 "y.tab.c" /* yacc.c:1652  */
    break;

  case 23:
#line 108 "tiger.y" /* yacc.c:1652  */
    {(yyval.exp) = A_IfExp(EM_tokPos, (yyvsp[-4].exp), (yyvsp[-2].exp), (yyvsp[0].exp));}
#line 1595 "y.tab.c" /* yacc.c:1652  */
    break;

  case 24:
#line 109 "tiger.y" /* yacc.c:1652  */
    {(yyval.exp) = A_IfExp(EM_tokPos, (yyvsp[-2].exp), (yyvsp[0].exp), NULL);}
#line 1601 "y.tab.c" /* yacc.c:1652  */
    break;

  case 25:
#line 110 "tiger.y" /* yacc.c:1652  */
    {(yyval.exp) = A_WhileExp(EM_tokPos, (yyvsp[-2].exp), (yyvsp[0].exp));}
#line 1607 "y.tab.c" /* yacc.c:1652  */
    break;

  case 26:
#line 111 "tiger.y" /* yacc.c:1652  */
    {(yyval.exp) = A_BreakExp(EM_tokPos);}
#line 1613 "y.tab.c" /* yacc.c:1652  */
    break;

  case 27:
#line 112 "tiger.y" /* yacc.c:1652  */
    {(yyval.exp) = A_ForExp(EM_tokPos, S_Symbol((yyvsp[-6].sval)), (yyvsp[-4].exp), (yyvsp[-2].exp), (yyvsp[0].exp));}
#line 1619 "y.tab.c" /* yacc.c:1652  */
    break;

  case 28:
#line 113 "tiger.y" /* yacc.c:1652  */
    {(yyval.exp) = A_LetExp(EM_tokPos, (yyvsp[-3].declist), (yyvsp[-1].exp));}
#line 1625 "y.tab.c" /* yacc.c:1652  */
    break;

  case 29:
#line 114 "tiger.y" /* yacc.c:1652  */
    {(yyval.exp) = A_ArrayExp(EM_tokPos, S_Symbol((yyvsp[-5].sval)), (yyvsp[-3].exp), (yyvsp[0].exp));}
#line 1631 "y.tab.c" /* yacc.c:1652  */
    break;

  case 30:
#line 115 "tiger.y" /* yacc.c:1652  */
    {(yyval.exp) = A_IfExp(EM_tokPos, (yyvsp[-2].exp), (yyvsp[0].exp), A_IntExp(EM_tokPos, 0));}
#line 1637 "y.tab.c" /* yacc.c:1652  */
    break;

  case 31:
#line 116 "tiger.y" /* yacc.c:1652  */
    {(yyval.exp) = A_IfExp(EM_tokPos, (yyvsp[-2].exp), A_IntExp(EM_tokPos, 1), (yyvsp[0].exp));}
#line 1643 "y.tab.c" /* yacc.c:1652  */
    break;

  case 32:
#line 120 "tiger.y" /* yacc.c:1652  */
    {(yyval.exp) = NULL;}
#line 1649 "y.tab.c" /* yacc.c:1652  */
    break;

  case 33:
#line 121 "tiger.y" /* yacc.c:1652  */
    {(yyval.exp) = A_SeqExp(EM_tokPos, (yyvsp[0].explist));}
#line 1655 "y.tab.c" /* yacc.c:1652  */
    break;

  case 34:
#line 124 "tiger.y" /* yacc.c:1652  */
    {(yyval.explist) = NULL;}
#line 1661 "y.tab.c" /* yacc.c:1652  */
    break;

  case 35:
#line 125 "tiger.y" /* yacc.c:1652  */
    {(yyval.explist) = (yyvsp[0].explist);}
#line 1667 "y.tab.c" /* yacc.c:1652  */
    break;

  case 36:
#line 128 "tiger.y" /* yacc.c:1652  */
    {(yyval.explist) = A_ExpList((yyvsp[0].exp), NULL);}
#line 1673 "y.tab.c" /* yacc.c:1652  */
    break;

  case 37:
#line 129 "tiger.y" /* yacc.c:1652  */
    {(yyval.explist) = A_ExpList((yyvsp[-2].exp), (yyvsp[0].explist));}
#line 1679 "y.tab.c" /* yacc.c:1652  */
    break;

  case 38:
#line 133 "tiger.y" /* yacc.c:1652  */
    {(yyval.explist) = A_ExpList((yyvsp[-2].exp), (yyvsp[0].explist));}
#line 1685 "y.tab.c" /* yacc.c:1652  */
    break;

  case 39:
#line 137 "tiger.y" /* yacc.c:1652  */
    {(yyval.explist) = A_ExpList((yyvsp[0].exp), NULL);}
#line 1691 "y.tab.c" /* yacc.c:1652  */
    break;

  case 40:
#line 138 "tiger.y" /* yacc.c:1652  */
    {(yyval.explist) = A_ExpList((yyvsp[-2].exp), (yyvsp[0].explist));}
#line 1697 "y.tab.c" /* yacc.c:1652  */
    break;

  case 41:
#line 141 "tiger.y" /* yacc.c:1652  */
    {(yyval.var) = A_SimpleVar(EM_tokPos, S_Symbol((yyvsp[0].sval)));}
#line 1703 "y.tab.c" /* yacc.c:1652  */
    break;

  case 42:
#line 142 "tiger.y" /* yacc.c:1652  */
    {(yyval.var) = (yyvsp[0].var);}
#line 1709 "y.tab.c" /* yacc.c:1652  */
    break;

  case 43:
#line 145 "tiger.y" /* yacc.c:1652  */
    {(yyval.var) = A_FieldVar(EM_tokPos, A_SimpleVar(EM_tokPos, S_Symbol((yyvsp[-2].sval))), S_Symbol((yyvsp[0].sval)));}
#line 1715 "y.tab.c" /* yacc.c:1652  */
    break;

  case 44:
#line 146 "tiger.y" /* yacc.c:1652  */
    {(yyval.var) = A_SubscriptVar(EM_tokPos, A_SimpleVar(EM_tokPos, S_Symbol((yyvsp[-3].sval))), (yyvsp[-1].exp));}
#line 1721 "y.tab.c" /* yacc.c:1652  */
    break;

  case 45:
#line 149 "tiger.y" /* yacc.c:1652  */
    {(yyval.var) = (yyvsp[0].var);}
#line 1727 "y.tab.c" /* yacc.c:1652  */
    break;

  case 46:
#line 150 "tiger.y" /* yacc.c:1652  */
    {(yyval.var) = A_FieldVar(EM_tokPos, (yyvsp[-2].var), S_Symbol((yyvsp[0].sval)));}
#line 1733 "y.tab.c" /* yacc.c:1652  */
    break;

  case 47:
#line 151 "tiger.y" /* yacc.c:1652  */
    {(yyval.var) = A_SubscriptVar(EM_tokPos, (yyvsp[-3].var), (yyvsp[-1].exp));}
#line 1739 "y.tab.c" /* yacc.c:1652  */
    break;

  case 48:
#line 154 "tiger.y" /* yacc.c:1652  */
    {(yyval.declist) = NULL;}
#line 1745 "y.tab.c" /* yacc.c:1652  */
    break;

  case 49:
#line 155 "tiger.y" /* yacc.c:1652  */
    {(yyval.declist) = (yyvsp[0].declist);}
#line 1751 "y.tab.c" /* yacc.c:1652  */
    break;

  case 50:
#line 158 "tiger.y" /* yacc.c:1652  */
    {(yyval.declist) = A_DecList((yyvsp[0].dec), NULL);}
#line 1757 "y.tab.c" /* yacc.c:1652  */
    break;

  case 51:
#line 159 "tiger.y" /* yacc.c:1652  */
    {(yyval.declist) = A_DecList((yyvsp[-1].dec), (yyvsp[0].declist));}
#line 1763 "y.tab.c" /* yacc.c:1652  */
    break;

  case 52:
#line 162 "tiger.y" /* yacc.c:1652  */
    {(yyval.dec) = A_TypeDec(EM_tokPos, (yyvsp[0].nametylist));}
#line 1769 "y.tab.c" /* yacc.c:1652  */
    break;

  case 53:
#line 163 "tiger.y" /* yacc.c:1652  */
    {(yyval.dec) = (yyvsp[0].dec);}
#line 1775 "y.tab.c" /* yacc.c:1652  */
    break;

  case 54:
#line 164 "tiger.y" /* yacc.c:1652  */
    {(yyval.dec) = A_FunctionDec(EM_tokPos, (yyvsp[0].fundeclist));}
#line 1781 "y.tab.c" /* yacc.c:1652  */
    break;

  case 55:
#line 167 "tiger.y" /* yacc.c:1652  */
    {(yyval.dec) = A_VarDec(EM_tokPos,S_Symbol((yyvsp[-2].sval)), S_Symbol(""), (yyvsp[0].exp));}
#line 1787 "y.tab.c" /* yacc.c:1652  */
    break;

  case 56:
#line 168 "tiger.y" /* yacc.c:1652  */
    {(yyval.dec) = A_VarDec(EM_tokPos, S_Symbol((yyvsp[-4].sval)), S_Symbol((yyvsp[-2].sval)), (yyvsp[0].exp));}
#line 1793 "y.tab.c" /* yacc.c:1652  */
    break;

  case 57:
#line 171 "tiger.y" /* yacc.c:1652  */
    {(yyval.efieldlist) = NULL;}
#line 1799 "y.tab.c" /* yacc.c:1652  */
    break;

  case 58:
#line 172 "tiger.y" /* yacc.c:1652  */
    {(yyval.efieldlist) = (yyvsp[0].efieldlist);}
#line 1805 "y.tab.c" /* yacc.c:1652  */
    break;

  case 59:
#line 175 "tiger.y" /* yacc.c:1652  */
    {(yyval.efieldlist) = A_EfieldList((yyvsp[0].efield), NULL);}
#line 1811 "y.tab.c" /* yacc.c:1652  */
    break;

  case 60:
#line 176 "tiger.y" /* yacc.c:1652  */
    {(yyval.efieldlist) = A_EfieldList((yyvsp[-2].efield), (yyvsp[0].efieldlist));}
#line 1817 "y.tab.c" /* yacc.c:1652  */
    break;

  case 61:
#line 179 "tiger.y" /* yacc.c:1652  */
    {(yyval.efield) = A_Efield(S_Symbol((yyvsp[-2].sval)), (yyvsp[0].exp));}
#line 1823 "y.tab.c" /* yacc.c:1652  */
    break;

  case 62:
#line 182 "tiger.y" /* yacc.c:1652  */
    {(yyval.nametylist) = A_NametyList((yyvsp[0].namety), NULL);}
#line 1829 "y.tab.c" /* yacc.c:1652  */
    break;

  case 63:
#line 183 "tiger.y" /* yacc.c:1652  */
    {(yyval.nametylist) = A_NametyList((yyvsp[-1].namety), (yyvsp[0].nametylist));}
#line 1835 "y.tab.c" /* yacc.c:1652  */
    break;

  case 64:
#line 186 "tiger.y" /* yacc.c:1652  */
    {(yyval.namety) = A_Namety(S_Symbol((yyvsp[-2].sval)), (yyvsp[0].ty));}
#line 1841 "y.tab.c" /* yacc.c:1652  */
    break;

  case 65:
#line 189 "tiger.y" /* yacc.c:1652  */
    {(yyval.fieldlist) = NULL;}
#line 1847 "y.tab.c" /* yacc.c:1652  */
    break;

  case 66:
#line 190 "tiger.y" /* yacc.c:1652  */
    {(yyval.fieldlist) = (yyvsp[0].fieldlist);}
#line 1853 "y.tab.c" /* yacc.c:1652  */
    break;

  case 67:
#line 193 "tiger.y" /* yacc.c:1652  */
    {(yyval.fieldlist) = A_FieldList(A_Field(EM_tokPos, S_Symbol((yyvsp[-2].sval)), S_Symbol((yyvsp[0].sval))), NULL);}
#line 1859 "y.tab.c" /* yacc.c:1652  */
    break;

  case 68:
#line 194 "tiger.y" /* yacc.c:1652  */
    {(yyval.fieldlist) = A_FieldList(A_Field(EM_tokPos, S_Symbol((yyvsp[-4].sval)), S_Symbol((yyvsp[-2].sval))), (yyvsp[0].fieldlist));}
#line 1865 "y.tab.c" /* yacc.c:1652  */
    break;

  case 69:
#line 197 "tiger.y" /* yacc.c:1652  */
    {(yyval.ty) = A_NameTy(EM_tokPos, S_Symbol((yyvsp[0].sval)));}
#line 1871 "y.tab.c" /* yacc.c:1652  */
    break;

  case 70:
#line 198 "tiger.y" /* yacc.c:1652  */
    {(yyval.ty) = A_RecordTy(EM_tokPos, (yyvsp[-1].fieldlist));}
#line 1877 "y.tab.c" /* yacc.c:1652  */
    break;

  case 71:
#line 199 "tiger.y" /* yacc.c:1652  */
    {(yyval.ty) = A_ArrayTy(EM_tokPos, S_Symbol((yyvsp[0].sval)));}
#line 1883 "y.tab.c" /* yacc.c:1652  */
    break;

  case 72:
#line 202 "tiger.y" /* yacc.c:1652  */
    {(yyval.fundeclist) = A_FundecList((yyvsp[0].fundec), NULL);}
#line 1889 "y.tab.c" /* yacc.c:1652  */
    break;

  case 73:
#line 203 "tiger.y" /* yacc.c:1652  */
    {(yyval.fundeclist) = A_FundecList((yyvsp[-1].fundec), (yyvsp[0].fundeclist));}
#line 1895 "y.tab.c" /* yacc.c:1652  */
    break;

  case 74:
#line 206 "tiger.y" /* yacc.c:1652  */
    {(yyval.fundec) = A_Fundec(EM_tokPos, S_Symbol((yyvsp[-5].sval)), (yyvsp[-3].fieldlist), S_Symbol(""), (yyvsp[0].exp));}
#line 1901 "y.tab.c" /* yacc.c:1652  */
    break;

  case 75:
#line 207 "tiger.y" /* yacc.c:1652  */
    {(yyval.fundec) = A_Fundec(EM_tokPos, S_Symbol((yyvsp[-7].sval)), (yyvsp[-5].fieldlist), S_Symbol((yyvsp[-2].sval)), (yyvsp[0].exp));}
#line 1907 "y.tab.c" /* yacc.c:1652  */
    break;


#line 1911 "y.tab.c" /* yacc.c:1652  */
      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */
  {
    const int yylhs = yyr1[yyn] - YYNTOKENS;
    const int yyi = yypgoto[yylhs] + *yyssp;
    yystate = (0 <= yyi && yyi <= YYLAST && yycheck[yyi] == *yyssp
               ? yytable[yyi]
               : yydefgoto[yylhs]);
  }

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYEMPTY : YYTRANSLATE (yychar);

  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
# define YYSYNTAX_ERROR yysyntax_error (&yymsg_alloc, &yymsg, \
                                        yyssp, yytoken)
      {
        char const *yymsgp = YY_("syntax error");
        int yysyntax_error_status;
        yysyntax_error_status = YYSYNTAX_ERROR;
        if (yysyntax_error_status == 0)
          yymsgp = yymsg;
        else if (yysyntax_error_status == 1)
          {
            if (yymsg != yymsgbuf)
              YYSTACK_FREE (yymsg);
            yymsg = (char *) YYSTACK_ALLOC (yymsg_alloc);
            if (!yymsg)
              {
                yymsg = yymsgbuf;
                yymsg_alloc = sizeof yymsgbuf;
                yysyntax_error_status = 2;
              }
            else
              {
                yysyntax_error_status = YYSYNTAX_ERROR;
                yymsgp = yymsg;
              }
          }
        yyerror (yymsgp);
        if (yysyntax_error_status == 2)
          goto yyexhaustedlab;
      }
# undef YYSYNTAX_ERROR
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
  /* Pacify compilers when the user code never invokes YYERROR and the
     label yyerrorlab therefore never appears in user code.  */
  if (0)
    YYERROR;

  /* Do not reclaim the symbols of the rule whose action triggered
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
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
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

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


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


#if !defined yyoverflow || YYERROR_VERBOSE
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif


/*-----------------------------------------------------.
| yyreturn -- parsing is finished, return the result.  |
`-----------------------------------------------------*/
yyreturn:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
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
  return yyresult;
}
