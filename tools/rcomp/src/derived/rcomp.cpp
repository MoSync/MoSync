/* A Bison parser, made by GNU Bison 2.3.  */

/* Skeleton implementation for Bison's Yacc-like parsers in C

Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004, 2005, 2006
Free Software Foundation, Inc.

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2, or (at your option)
any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor,
Boston, MA 02110-1301, USA.  */

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
#define YYBISON_VERSION "2.3"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Using locations.  */
#define YYLSP_NEEDED 0



/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
/* Put the tokens into the symbol table, so that GDB and other debuggers
know about them.  */
enum yytokentype {
	L_STRUCT = 258,
	L_RESOURCE = 259,
	L_NAME = 260,
	L_OFFSET = 261,
	L_SYSTEM = 262,
	L_GLOBAL = 263,
	L_LOCAL = 264,
	L_CHARACTER_SET = 265,
	L_BUF = 266,
	L_WORD = 267,
	L_BYTE = 268,
	L_LONG = 269,
	L_DOUBLE = 270,
	L_TEXT = 271,
	L_LTEXT = 272,
	L_LINK = 273,
	L_LLINK = 274,
	L_SRLINK = 275,
	L_BUF8 = 276,
	L_TEXT8 = 277,
	L_LTEXT8 = 278,
	L_BUF16 = 279,
	L_TEXT16 = 280,
	L_LTEXT16 = 281,
	L_UID2 = 282,
	L_UID3 = 283,
	L_LABEL = 284,
	L_NUM_NATURAL = 285,
	L_NUM_FLOAT = 286,
	L_NATURAL_EXPR = 287,
	L_ENUM = 288,
	L_LEN = 289,
	L_CHAR_LITERAL = 290,
	L_STRING_LITERAL = 291,
	UMINUS = 292
};
#endif
/* Tokens.  */
#define L_STRUCT 258
#define L_RESOURCE 259
#define L_NAME 260
#define L_OFFSET 261
#define L_SYSTEM 262
#define L_GLOBAL 263
#define L_LOCAL 264
#define L_CHARACTER_SET 265
#define L_BUF 266
#define L_WORD 267
#define L_BYTE 268
#define L_LONG 269
#define L_DOUBLE 270
#define L_TEXT 271
#define L_LTEXT 272
#define L_LINK 273
#define L_LLINK 274
#define L_SRLINK 275
#define L_BUF8 276
#define L_TEXT8 277
#define L_LTEXT8 278
#define L_BUF16 279
#define L_TEXT16 280
#define L_LTEXT16 281
#define L_UID2 282
#define L_UID3 283
#define L_LABEL 284
#define L_NUM_NATURAL 285
#define L_NUM_FLOAT 286
#define L_NATURAL_EXPR 287
#define L_ENUM 288
#define L_LEN 289
#define L_CHAR_LITERAL 290
#define L_STRING_LITERAL 291
#define UMINUS 292




/* Copy the first part of user declarations.  */


// RCOMP.CPP
// Generated from RCOMP.Y
//
// Copyright (c) 1997-1999 Symbian Ltd.  All rights reserved.
//

#include <assert.h>
#include <ctype.h>
#include <fstream>
#include "resource.h"
#include "rcstack.h"
#include "rcompl.hpp"
#include "rcomp.hpp"
#include "datatype.h"
#include "mem.h"
#include "rcbinstr.h"
#include "indextab.h"
#include "linkarra.h"
#include "numval.h"
//#include "rcscan.h"
#include "fileline.h"
#include "errorhan.h"
#include "fileacc.h"
#include "version.h"
#include "ctable.h"
#include "main.h"
#include <stdarg.h>

using namespace std;

#if defined(__VC32__) && !defined(_DEBUG)
#pragma warning( disable : 4702 )	// unreachable code
#pragma warning( disable : 4102 )	// 'yyerrlabel' : unreferenced label
#pragma warning( disable : 4244 )	// '=' : conversion from 'int' to 'short', possible loss of data
#endif



String::CharacterSet CharacterSetID( const String & character_set_name );
void asUTF8(char* aUtf8, int aUnicode);
void SetIdFromName( const String & NameStatementValue);


unsigned short & d = MemCheckControl::iLogMemory;

StructHeader *		pSH;

StructHeaderArray * 	pSHA;	// Used in resource struct handling functions.
ResourceHeader *	pResourceHeader;
ResourceItemArray *	pCurrentRIA;
int			verbose;
String::CharacterSet	SourceCharacterSet = String::CP1252;
String::CharacterSet	TargetCharacterSet = String::CP1252;
unsigned short		logmemorysetting;
int *			pCurrentLineNumber;
FileLineManager *	pFileLineHandler;
NameIdMap *		pResourceNameIds;
LinkArray *		pLinks;
long			CurrentEnumValue;
String			CurrentEnumName;
char			TempStr[300];

extern unsigned long uid2, uid3;
extern bool uid2Set, uid3Set;
bool errorWasFound = false;

void yyerror(const char* fmt, ...) {
	va_list va;
	va_start(va, fmt);
	fprintf(stderr, "%s:%d: ", pFileLineHandler->GetCurrentFile().GetBuffer(), pFileLineHandler->GetErrorLine(yylineno));
	vfprintf(stderr, fmt, va);
	va_end(va);
	errorWasFound = true;
}


int CurrentIdStep=1;
long CurrentId=0;
int FormatIdAsHex=0;	// defaults to decimal, changes in SetIdFromName


const String	Divider("*******************************************");

#define REGISTER_LINE	ErrorHandler::Register(pFileLineHandler->GetCurrentFile(), pFileLineHandler->GetErrorLine(* pCurrentLineNumber))



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

#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE

{
	char				Value[1024];
	unsigned long 			Id;
	StructItem *			pStructItem;
	SimpleStructItem *		pSimpleStructItem;
	ArrayStructItem *   	pArrayStructItem;
	StructArrayStructItem *	pStructArrayStructItem;
	StringArray *			pStringArray;
	long					NumInitialiser;
}
/* Line 187 of yacc.c.  */

YYSTYPE;
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif



/* Copy the second part of user declarations.  */


/* Line 216 of yacc.c.  */


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
YYID (int i)
#else
static int
YYID (i)
int i;
#endif
{
	return i;
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
	yytype_int16 yyss;
	YYSTYPE yyvs;
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
# define YYSTACK_RELOCATE(Stack)					\
	do									\
{									\
	YYSIZE_T yynewbytes;						\
	YYCOPY (&yyptr->Stack, Stack, yysize);				\
	Stack = &yyptr->Stack;						\
	yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
	yyptr += yynewbytes / sizeof (*yyptr);				\
}									\
	while (YYID (0))

#endif

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  3
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   222

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  54
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  48
/* YYNRULES -- Number of rules.  */
#define YYNRULES  121
/* YYNRULES -- Number of states.  */
#define YYNSTATES  197

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   292

#define YYTRANSLATE(YYX)						\
	((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] =
{
	0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
	2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
	2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
	2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
	46,    47,    39,    37,    53,    38,     2,    40,     2,     2,
	2,     2,     2,     2,     2,     2,     2,     2,     2,    45,
	49,    48,    50,     2,     2,     2,     2,     2,     2,     2,
	2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
	2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
	2,    52,     2,    51,     2,     2,     2,     2,     2,     2,
	2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
	2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
	2,     2,     2,    44,    41,    43,     2,     2,     2,     2,
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
	35,    36,    42
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
YYRHS.  */
static const yytype_uint16 yyprhs[] =
{
	0,     0,     3,     5,     8,     9,    11,    13,    15,    17,
	19,    21,    23,    27,    31,    36,    41,    45,    46,    48,
	50,    52,    54,    56,    61,    65,    72,    75,    81,    83,
	85,    87,    89,    91,    93,    95,    97,    99,   101,   103,
	105,   107,   109,   111,   113,   115,   121,   124,   128,   133,
	139,   143,   145,   147,   150,   152,   156,   161,   167,   172,
	175,   178,   181,   184,   186,   190,   193,   197,   201,   202,
	206,   208,   210,   212,   217,   223,   227,   232,   236,   240,
	246,   248,   252,   256,   259,   261,   265,   269,   272,   274,
	276,   278,   280,   282,   284,   288,   290,   292,   296,   300,
	304,   308,   312,   315,   319,   321,   324,   326,   328,   332,
	335,   338,   341,   344,   346,   350,   355,   359,   362,   364,
	368,   370
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int8 yyrhs[] =
{
	55,     0,    -1,    56,    -1,    56,    57,    -1,    -1,    58,
	-1,    72,    -1,    95,    -1,    94,    -1,    96,    -1,    97,
	-1,    98,    -1,    59,    60,    43,    -1,     3,    29,    44,
	-1,     3,    29,    68,    44,    -1,     3,    29,    34,    44,
	-1,    60,    61,    45,    -1,    -1,    62,    -1,    65,    -1,
	69,    -1,    70,    -1,    63,    -1,    63,    46,    89,    47,
	-1,    63,    48,    87,    -1,    63,    46,    89,    47,    48,
	91,    -1,    64,    29,    -1,    64,    49,    90,    50,    29,
	-1,    13,    -1,    12,    -1,    14,    -1,    15,    -1,    16,
	-1,    17,    -1,    11,    -1,    22,    -1,    25,    -1,    23,
	-1,    26,    -1,    21,    -1,    24,    -1,    18,    -1,    19,
	-1,    20,    -1,    66,    -1,    66,    48,    44,    88,    43,
	-1,    67,    51,    -1,    67,    89,    51,    -1,    34,    68,
	67,    51,    -1,    34,    68,    67,    89,    51,    -1,    64,
	29,    52,    -1,    13,    -1,    12,    -1,     3,    29,    -1,
	71,    -1,    34,    68,    71,    -1,     3,    29,    52,    51,
	-1,     3,    29,    52,    89,    51,    -1,    73,    44,    75,
	43,    -1,    27,    90,    -1,    28,    90,    -1,     8,    74,
	-1,     9,    74,    -1,    74,    -1,     4,    29,    29,    -1,
	4,    29,    -1,    75,    76,    45,    -1,    75,     1,    45,
	-1,    -1,    29,    48,    87,    -1,    77,    -1,    78,    -1,
	80,    -1,    29,    48,    44,    43,    -1,    29,    48,    44,
	88,    43,    -1,    79,    75,    43,    -1,    29,    48,    29,
	44,    -1,    81,    82,    43,    -1,    81,    82,     1,    -1,
	29,    48,    44,    29,    44,    -1,    83,    -1,    83,    53,
	84,    -1,    83,    53,     1,    -1,    75,    43,    -1,    85,
	-1,    84,    53,    85,    -1,    86,    75,    43,    -1,    29,
	44,    -1,    31,    -1,    35,    -1,    29,    -1,    91,    -1,
	89,    -1,    87,    -1,    88,    53,    87,    -1,    90,    -1,
	30,    -1,    90,    37,    90,    -1,    90,    38,    90,    -1,
	90,    39,    90,    -1,    90,    40,    90,    -1,    90,    41,
	90,    -1,    38,    90,    -1,    46,    90,    47,    -1,    92,
	-1,    92,    91,    -1,    36,    -1,    93,    -1,    49,    90,
	50,    -1,     5,    29,    -1,     5,    36,    -1,    10,    29,
	-1,     6,    89,    -1,     7,    -1,    99,   101,    43,    -1,
	99,   101,    43,    45,    -1,    33,    29,    44,    -1,    33,
	44,    -1,    29,    -1,    29,    48,    87,    -1,   100,    -1,
	101,    53,   100,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
	0,   129,   129,   136,   137,   140,   141,   142,   143,   144,
	145,   146,   153,   157,   162,   168,   176,   180,   183,   184,
	185,   186,   189,   190,   195,   200,   216,   223,   238,   239,
	240,   241,   245,   251,   255,   262,   266,   270,   271,   272,
	273,   274,   275,   276,   279,   280,   298,   301,   306,   314,
	325,   333,   334,   337,   342,   343,   349,   352,   361,   370,
	371,   374,   375,   381,   384,   396,   405,   406,   407,   410,
	414,   415,   416,   419,   424,   453,   459,   467,   471,   475,
	485,   486,   487,   490,   495,   496,   499,   505,   519,   520,
	534,   552,   553,   556,   565,   574,   577,   581,   582,   583,
	584,   585,   586,   587,   590,   591,   602,   603,   606,   657,
	662,   678,   702,   713,   722,   723,   726,   733,   743,   744,
	754,   755
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
	"$end", "error", "$undefined", "L_STRUCT", "L_RESOURCE", "L_NAME",
	"L_OFFSET", "L_SYSTEM", "L_GLOBAL", "L_LOCAL", "L_CHARACTER_SET",
	"L_BUF", "L_WORD", "L_BYTE", "L_LONG", "L_DOUBLE", "L_TEXT", "L_LTEXT",
	"L_LINK", "L_LLINK", "L_SRLINK", "L_BUF8", "L_TEXT8", "L_LTEXT8",
	"L_BUF16", "L_TEXT16", "L_LTEXT16", "L_UID2", "L_UID3", "L_LABEL",
	"L_NUM_NATURAL", "L_NUM_FLOAT", "L_NATURAL_EXPR", "L_ENUM", "L_LEN",
	"L_CHAR_LITERAL", "L_STRING_LITERAL", "'+'", "'-'", "'*'", "'/'", "'|'",
	"UMINUS", "'}'", "'{'", "';'", "'('", "')'", "'='", "'<'", "'>'", "']'",
	"'['", "','", "$accept", "source", "statement_list", "statement",
	"struct_statement", "struct_statement_start", "struct_item_list",
	"struct_item", "simple_struct_item", "simple_struct_item_start",
	"data_type", "array_struct_item", "array_struct_item_base",
	"array_struct_item_start", "len_declaration", "struct_type_struct_item",
	"struct_array_struct_item", "struct_array_struct_item_base",
	"resource_statement", "resource_statement_start",
	"resource_statement_start_names", "resource_item_list", "resource_item",
	"resource_simple_array_item", "struct_resource_item",
	"struct_resource_item_start", "struct_array_resource_item",
	"struct_array_resource_item_start",
	"struct_array_resource_item_list_top",
	"struct_array_resource_item_list_top_start",
	"struct_array_resource_item_list",
	"struct_array_resource_item_list_item",
	"struct_array_resource_item_list_item_start", "simple_initialiser",
	"simple_initialiser_list", "natural_expression",
	"natural_expression_numeric", "string_expression",
	"string_expression_item", "character_code_expression", "name_statement",
	"character_set_statement", "offset_statement", "system_statement",
	"enum_statement", "enum_statement_start", "enum_list_entry", "enum_list", 0
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
	285,   286,   287,   288,   289,   290,   291,    43,    45,    42,
	47,   124,   292,   125,   123,    59,    40,    41,    61,    60,
	62,    93,    91,    44
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
	0,    54,    55,    56,    56,    57,    57,    57,    57,    57,
	57,    57,    58,    59,    59,    59,    60,    60,    61,    61,
	61,    61,    62,    62,    62,    62,    63,    63,    64,    64,
	64,    64,    64,    64,    64,    64,    64,    64,    64,    64,
	64,    64,    64,    64,    65,    65,    66,    66,    66,    66,
	67,    68,    68,    69,    70,    70,    71,    71,    72,    72,
	72,    73,    73,    73,    74,    74,    75,    75,    75,    76,
	76,    76,    76,    77,    77,    78,    79,    80,    80,    81,
	82,    82,    82,    83,    84,    84,    85,    86,    87,    87,
	87,    87,    87,    88,    88,    89,    90,    90,    90,    90,
	90,    90,    90,    90,    91,    91,    92,    92,    93,    94,
	94,    95,    96,    97,    98,    98,    99,    99,   100,   100,
	101,   101
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
	0,     2,     1,     2,     0,     1,     1,     1,     1,     1,
	1,     1,     3,     3,     4,     4,     3,     0,     1,     1,
	1,     1,     1,     4,     3,     6,     2,     5,     1,     1,
	1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
	1,     1,     1,     1,     1,     5,     2,     3,     4,     5,
	3,     1,     1,     2,     1,     3,     4,     5,     4,     2,
	2,     2,     2,     1,     3,     2,     3,     3,     0,     3,
	1,     1,     1,     4,     5,     3,     4,     3,     3,     5,
	1,     3,     3,     2,     1,     3,     3,     2,     1,     1,
	1,     1,     1,     1,     3,     1,     1,     3,     3,     3,
	3,     3,     2,     3,     1,     2,     1,     1,     3,     2,
	2,     2,     2,     1,     3,     4,     3,     2,     1,     3,
	1,     3
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
	4,     0,     2,     1,     0,     0,     0,     0,   113,     0,
	0,     0,     0,     0,     0,     3,     5,    17,     6,     0,
	63,     8,     7,     9,    10,    11,     0,     0,    65,   109,
	110,    96,     0,     0,   112,    95,    61,    62,   111,    59,
	60,     0,   117,     0,    68,   118,   120,     0,    52,    51,
	0,    13,     0,    64,   102,     0,     0,     0,     0,     0,
	0,   116,     0,    34,    29,    28,    30,    31,    32,    33,
	41,    42,    43,    39,    35,    37,    40,    36,    38,     0,
	12,     0,    18,    22,     0,    19,    44,     0,    20,    21,
	54,     0,     0,   114,     0,    15,    14,   103,    97,    98,
	99,   100,   101,    53,     0,    16,     0,     0,    26,     0,
	0,    46,     0,     0,     0,    58,     0,    70,    71,    68,
	72,    68,    90,    88,    89,   106,     0,   119,    92,    91,
	104,   107,   115,   121,     0,     0,     0,     0,    55,     0,
	24,    50,     0,     0,    47,    67,     0,    66,     0,     0,
	0,    80,     0,   105,    56,     0,     0,     0,    48,     0,
	23,     0,    93,     0,    90,     0,    69,    75,    83,    78,
	77,     0,   108,    57,    49,     0,    27,    45,     0,    76,
	90,    73,     0,    82,     0,    81,    84,    68,    25,    94,
	79,    74,    87,     0,     0,    85,    86
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
	-1,     1,     2,    15,    16,    17,    43,    81,    82,    83,
	84,    85,    86,    87,    52,    88,    89,    90,    18,    19,
	20,    91,   116,   117,   118,   119,   120,   121,   150,   151,
	185,   186,   187,   162,   163,   128,    35,   129,   130,   131,
	21,    22,    23,    24,    25,    26,    46,    47
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
STATE-NUM.  */
#define YYPACT_NINF -126
static const yytype_int16 yypact[] =
{
	-126,    16,    55,  -126,    -9,    50,   123,    46,  -126,    64,
	64,    68,    46,    46,   -15,  -126,  -126,  -126,  -126,    49,
	-126,  -126,  -126,  -126,  -126,  -126,    84,    22,    87,  -126,
	-126,  -126,    46,    46,  -126,   164,  -126,  -126,  -126,   164,
	164,    66,  -126,   120,  -126,    77,  -126,   -26,  -126,  -126,
	85,  -126,   109,  -126,  -126,    81,    46,    46,    46,    46,
	46,  -126,   126,  -126,  -126,  -126,  -126,  -126,  -126,  -126,
	-126,  -126,  -126,  -126,  -126,  -126,  -126,  -126,  -126,    30,
	-126,   116,  -126,    78,    38,  -126,   114,   -23,  -126,  -126,
	-126,     8,    -5,   138,    84,  -126,  -126,  -126,    34,    34,
	143,   143,  -126,   133,   153,  -126,    46,    -5,   136,    46,
	146,  -126,   140,   147,   145,  -126,   150,  -126,  -126,  -126,
	-126,  -126,  -126,  -126,  -126,  -126,    46,  -126,  -126,  -126,
	-17,  -126,  -126,  -126,    39,   167,   169,    40,  -126,   152,
	-126,  -126,    67,    -5,  -126,  -126,    65,  -126,     9,    10,
	12,   154,   110,  -126,  -126,   155,   133,   136,  -126,   157,
	161,   181,  -126,    28,   168,   151,  -126,  -126,  -126,  -126,
	-126,     7,  -126,  -126,  -126,   -17,  -126,  -126,    -5,  -126,
	170,  -126,    59,  -126,   171,   158,  -126,  -126,  -126,  -126,
	-126,  -126,  -126,   184,    11,  -126,  -126
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
	-126,  -126,  -126,  -126,  -126,  -126,  -126,  -126,  -126,  -126,
	112,  -126,  -126,   113,   139,  -126,  -126,   115,  -126,  -126,
	148,  -115,  -126,  -126,  -126,  -126,  -126,  -126,  -126,  -126,
	-126,    27,  -126,   -89,    56,    -7,   -11,  -125,  -126,  -126,
	-126,  -126,  -126,  -126,  -126,  -126,   128,  -126
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
positive, shift that token.  If negative, reduce the rule which
number is the opposite.  If zero, do what YYDEFACT says.
If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -1
static const yytype_uint8 yytable[] =
{
	34,    39,    40,   127,   148,   153,   149,    31,   183,   113,
	113,   113,   113,   169,    41,    32,     3,    93,   140,   125,
	27,    54,    55,    33,   122,    31,   123,    94,   111,    42,
	124,   125,   126,    32,    48,    49,   184,   114,   114,   114,
	114,    33,    48,    49,   126,    98,    99,   100,   101,   102,
	188,   115,   167,   168,   196,   170,    50,   166,     4,     5,
	6,     7,     8,     9,    10,    11,    51,   108,     5,    31,
	31,   177,   194,    58,    59,    60,    31,    32,    32,    28,
	112,   178,    12,    13,    32,    33,    33,   109,    14,   189,
	154,   158,    33,    44,   164,    31,   123,    38,   142,   139,
	124,   125,   191,    32,    56,    57,    58,    59,    60,   165,
	61,    33,   178,    45,   126,   152,    53,   161,    56,    57,
	58,    59,    60,    62,   106,    92,   107,   155,    97,    95,
	159,    63,    64,    65,    66,    67,    68,    69,    70,    71,
	72,    73,    74,    75,    76,    77,    78,    56,    57,    58,
	59,    60,    29,    96,    79,   103,   135,    36,    37,    30,
	172,   105,   110,    80,    63,    64,    65,    66,    67,    68,
	69,    70,    71,    72,    73,    74,    75,    76,    77,    78,
	180,    31,   123,   132,    60,   134,   124,   125,   141,    32,
	143,   144,   145,   146,   181,   147,   156,    33,   157,   160,
	126,    56,    57,    58,    59,    60,   173,   171,   174,   175,
	176,   193,   179,   184,   190,   192,   136,   137,   104,   138,
	195,   182,   133
};

static const yytype_uint8 yycheck[] =
{
	7,    12,    13,    92,   119,   130,   121,    30,     1,     1,
	1,     1,     1,     1,    29,    38,     0,    43,   107,    36,
	29,    32,    33,    46,    29,    30,    31,    53,    51,    44,
	35,    36,    49,    38,    12,    13,    29,    29,    29,    29,
	29,    46,    12,    13,    49,    56,    57,    58,    59,    60,
	175,    43,    43,    43,    43,    43,    34,   146,     3,     4,
	5,     6,     7,     8,     9,    10,    44,    29,     4,    30,
	30,    43,   187,    39,    40,    41,    30,    38,    38,    29,
	87,    53,    27,    28,    38,    46,    46,    49,    33,   178,
	51,    51,    46,    44,    29,    30,    31,    29,   109,   106,
	35,    36,    43,    38,    37,    38,    39,    40,    41,    44,
	44,    46,    53,    29,    49,   126,    29,    50,    37,    38,
	39,    40,    41,     3,    46,    48,    48,   134,    47,    44,
	137,    11,    12,    13,    14,    15,    16,    17,    18,    19,
	20,    21,    22,    23,    24,    25,    26,    37,    38,    39,
	40,    41,    29,    44,    34,    29,     3,     9,    10,    36,
	50,    45,    48,    43,    11,    12,    13,    14,    15,    16,
	17,    18,    19,    20,    21,    22,    23,    24,    25,    26,
	29,    30,    31,    45,    41,    52,    35,    36,    52,    38,
	44,    51,    45,    48,    43,    45,    29,    46,    29,    47,
	49,    37,    38,    39,    40,    41,    51,    53,    51,    48,
	29,    53,    44,    29,    44,    44,   104,   104,    79,   104,
	193,   165,    94
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
	0,    55,    56,     0,     3,     4,     5,     6,     7,     8,
	9,    10,    27,    28,    33,    57,    58,    59,    72,    73,
	74,    94,    95,    96,    97,    98,    99,    29,    29,    29,
	36,    30,    38,    46,    89,    90,    74,    74,    29,    90,
	90,    29,    44,    60,    44,    29,   100,   101,    12,    13,
	34,    44,    68,    29,    90,    90,    37,    38,    39,    40,
	41,    44,     3,    11,    12,    13,    14,    15,    16,    17,
	18,    19,    20,    21,    22,    23,    24,    25,    26,    34,
	43,    61,    62,    63,    64,    65,    66,    67,    69,    70,
	71,    75,    48,    43,    53,    44,    44,    47,    90,    90,
	90,    90,    90,    29,    68,    45,    46,    48,    29,    49,
	48,    51,    89,     1,    29,    43,    76,    77,    78,    79,
	80,    81,    29,    31,    35,    36,    49,    87,    89,    91,
	92,    93,    45,   100,    52,     3,    64,    67,    71,    89,
	87,    52,    90,    44,    51,    45,    48,    45,    75,    75,
	82,    83,    90,    91,    51,    89,    29,    29,    51,    89,
	47,    50,    87,    88,    29,    44,    87,    43,    43,     1,
	43,    53,    50,    51,    51,    48,    29,    43,    53,    44,
	29,    43,    88,     1,    29,    84,    85,    86,    91,    87,
	44,    43,    44,    53,    75,    85,    43
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
yy_stack_print (yytype_int16 *bottom, yytype_int16 *top)
#else
static void
yy_stack_print (bottom, top)
yytype_int16 *bottom;
yytype_int16 *top;
#endif
{
	YYFPRINTF (stderr, "Stack now");
	for (; bottom <= top; ++bottom)
		YYFPRINTF (stderr, " %d", *bottom);
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
		fprintf (stderr, "   $%d = ", yyi + 1);
		yy_symbol_print (stderr, yyrhs[yyprhs[yyrule] + yyi],
			&(yyvsp[(yyi + 1) - (yynrhs)])
			);
		fprintf (stderr, "\n");
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



/* The look-ahead symbol.  */
int yychar;

/* The semantic value of the look-ahead symbol.  */
YYSTYPE yylval;

/* Number of syntax errors so far.  */
int yynerrs;



/*----------.
| yyparse.  |
`----------*/

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
	int yyn;
	int yyresult;
	/* Number of tokens to shift before error messages enabled.  */
	int yyerrstatus;
	/* Look-ahead token as an internal (translated) token number.  */
	int yytoken = 0;
#if YYERROR_VERBOSE
	/* Buffer for error messages, and its allocated size.  */
	char yymsgbuf[128];
	char *yymsg = yymsgbuf;
	YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

	/* Three stacks and their tools:
	`yyss': related to states,
	`yyvs': related to semantic values,
	`yyls': related to locations.

	Refer to the stacks thru separate pointers, to allow yyoverflow
	to reallocate them elsewhere.  */

	/* The state stack.  */
	yytype_int16 yyssa[YYINITDEPTH];
	yytype_int16 *yyss = yyssa;
	yytype_int16 *yyssp;

	/* The semantic value stack.  */
	YYSTYPE yyvsa[YYINITDEPTH];
	YYSTYPE *yyvs = yyvsa;
	YYSTYPE *yyvsp;



#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

	YYSIZE_T yystacksize = YYINITDEPTH;

	/* The variables used to return semantic value and location from the
	action routines.  */
	YYSTYPE yyval;


	/* The number of symbols on the RHS of the reduced rule.
	Keep to zero when no symbol should be popped.  */
	int yylen = 0;

	YYDPRINTF ((stderr, "Starting parse\n"));

	yystate = 0;
	yyerrstatus = 0;
	yynerrs = 0;
	yychar = YYEMPTY;		/* Cause a token to be read.  */

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
			YYSTACK_RELOCATE (yyss);
			YYSTACK_RELOCATE (yyvs);

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

	goto yybackup;

	/*-----------.
	| yybackup.  |
	`-----------*/
yybackup:

	/* Do appropriate processing given the current state.  Read a
	look-ahead token if we need one and don't already have one.  */

	/* First try to decide what to do without reference to look-ahead token.  */
	yyn = yypact[yystate];
	if (yyn == YYPACT_NINF)
		goto yydefault;

	/* Not known => get a look-ahead token if don't already have one.  */

	/* YYCHAR is either YYEMPTY or YYEOF or a valid look-ahead symbol.  */
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

	if (yyn == YYFINAL)
		YYACCEPT;

	/* Count tokens shifted since error; after three, turn off error
	status.  */
	if (yyerrstatus)
		yyerrstatus--;

	/* Shift the look-ahead token.  */
	YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

	/* Discard the shifted token unless it is eof.  */
	if (yychar != YYEOF)
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

		{	if(verbose)	{	MOFF; cout << Divider << "\n" << Divider << endl; MON; }
		}
		break;

	case 12:

		{	if(verbose) { MOFF; cout << Divider << "\n" << * pSH << Divider << endl; MON;}	}
		break;

	case 13:

		{	if(verbose) {	MOFF;cout << "struct_statement_start                     " << (yyvsp[(2) - (3)].Value) << endl; MON;}
		pSH = new StructHeader((yyvsp[(2) - (3)].Value));
		REGISTER_LINE;
		pG->SHA.Add(pSH);
		}
		break;

	case 14:

		{	if(verbose) {	RCTypeArray Types; MOFF;cout << "struct_statement_start                     " << (yyvsp[(2) - (4)].Value) << " " << Types.GetName((yyvsp[(3) - (4)].Id)) << endl; MON;}
		pSH = new StructHeader((yyvsp[(2) - (4)].Value), (yyvsp[(3) - (4)].Id));
		REGISTER_LINE;
		pG->SHA.Add(pSH);
		}
		break;

	case 15:

		{	if(verbose) {	MOFF;cout << "struct_statement_start                     " << (yyvsp[(2) - (4)].Value) << " (WORD)" << endl; MON;}
		pSH = new StructHeader((yyvsp[(2) - (4)].Value), L_WORD);
		REGISTER_LINE;
		pG->SHA.Add(pSH);
		}
		break;

	case 16:

		{	if(verbose) {	MOFF;cout << "struct_item_list                           Adding struct_item." << endl; MON;}
		REGISTER_LINE;
		pSH->iSIA.Add((yyvsp[(2) - (3)].pStructItem));
		}
		break;

	case 22:

		{	(yyval.pStructItem) = (yyvsp[(1) - (1)].pSimpleStructItem);}
		break;

	case 23:

		{	if(verbose) { MOFF;cout << "    Limit: " << (yyvsp[(3) - (4)].Value) << endl; MON;}
		(yyvsp[(1) - (4)].pSimpleStructItem)->iLengthLimit = (yyvsp[(3) - (4)].Value);
		(yyval.pStructItem) = (yyvsp[(1) - (4)].pSimpleStructItem);
		}
		break;

	case 24:

		{	if(verbose) { MOFF;cout << "    Default: " << (yyvsp[(3) - (3)].Value) << endl; MON;}
		(yyvsp[(1) - (3)].pSimpleStructItem)->iDefault = (yyvsp[(3) - (3)].Value);
		(yyval.pStructItem) = (yyvsp[(1) - (3)].pSimpleStructItem);
		}
		break;

	case 25:

		{	if(verbose) { MOFF;cout << "    Limit: " << (yyvsp[(3) - (6)].Value) << ", Default: " << (yyvsp[(6) - (6)].Value) << endl; MON;}
		NumericValue Limit((yyvsp[(3) - (6)].Value), L_LONG);
		if(String((yyvsp[(6) - (6)].Value)).ExportLength(TargetCharacterSet,SourceCharacterSet) > Limit.GetULong() )
		{
			REGISTER_LINE;
			ErrorHandler::OutputErrorLine("Text length exceeds specified limit");
			exit(1);
		}
		(yyvsp[(1) - (6)].pSimpleStructItem)->iLengthLimit = (yyvsp[(3) - (6)].Value);
		(yyvsp[(1) - (6)].pSimpleStructItem)->iDefault = (yyvsp[(6) - (6)].Value);
		(yyval.pStructItem) = (yyvsp[(1) - (6)].pSimpleStructItem);
		}
		break;

	case 26:

		{	if(verbose) 
		{			   RCTypeArray Types;
		MOFF;cout << "simple_struct_item                         " << Types.GetName((yyvsp[(1) - (2)].Id)) << " " << (yyvsp[(2) - (2)].Value) << endl; MON;
		}
		(yyval.pSimpleStructItem) = new SimpleStructItem((yyvsp[(1) - (2)].Id),(yyvsp[(2) - (2)].Value)); 
		assert((yyval.pSimpleStructItem) != NULL);
		}
		break;

	case 27:

		{	if(verbose) 
		{			   RCTypeArray Types;
		MOFF;cout << "simple_struct_item                         " << Types.GetName((yyvsp[(1) - (5)].Id)) << " " << (yyvsp[(5) - (5)].Value) << endl; MON;
		}
		String s(NumericValue::ltoa((yyvsp[(3) - (5)].NumInitialiser)));
		(yyval.pSimpleStructItem) = new SimpleStructItem((yyvsp[(1) - (5)].Id),(yyvsp[(5) - (5)].Value),s);
		assert((yyval.pSimpleStructItem) != NULL);
		}
		break;

	case 28:

		{	(yyval.Id) = L_BYTE;}
		break;

	case 29:

		{	(yyval.Id) = L_WORD;}
		break;

	case 30:

		{	(yyval.Id) = L_LONG;}
		break;

	case 31:

		{	(yyval.Id) = L_DOUBLE;}
		break;

	case 32:

		{ 
			(yyval.Id) = ( TargetCharacterSet == String::Unicode ) ? L_TEXT16: L_TEXT8;
			REGISTER_LINE;
			ErrorHandler::OutputErrorLine("Warning: Deprecated use of zero-terminated TEXT - use LTEXT instead");
		}
		break;

	case 33:

		{
			(yyval.Id) = ( TargetCharacterSet == String::Unicode ) ? L_LTEXT16: L_LTEXT8;
		}
		break;

	case 34:

		{ 
			(yyval.Id) = ( TargetCharacterSet == String::Unicode ) ? L_BUF16: L_BUF8;
		}
		break;

	case 35:

		{	(yyval.Id) = L_TEXT8;
		REGISTER_LINE;
		ErrorHandler::OutputErrorLine("Warning: Deprecated use of zero-terminated TEXT8 - use LTEXT8 instead");
		}
		break;

	case 36:

		{	(yyval.Id) = L_TEXT16;
		REGISTER_LINE;
		ErrorHandler::OutputErrorLine("Warning: Deprecated use of zero-terminated TEXT16 - use LTEXT16 instead");
		}
		break;

	case 37:

		{	(yyval.Id) = L_LTEXT8;}
		break;

	case 38:

		{	(yyval.Id) = L_LTEXT16;}
		break;

	case 39:

		{	(yyval.Id) = L_BUF8;}
		break;

	case 40:

		{	(yyval.Id) = L_BUF16;}
		break;

	case 41:

		{	(yyval.Id) = L_LINK;}
		break;

	case 42:

		{	(yyval.Id) = L_LLINK;}
		break;

	case 43:

		{	(yyval.Id) = L_SRLINK;}
		break;

	case 44:

		{	(yyval.pStructItem) = (yyvsp[(1) - (1)].pArrayStructItem);}
		break;

	case 45:

		{	if(verbose) {	MOFF;cout << "array_struct_item                          with simple_initialiser_list" << endl;MON;}
		(yyvsp[(1) - (5)].pArrayStructItem)->iDefaults = * (yyvsp[(4) - (5)].pStringArray);
		if((yyvsp[(1) - (5)].pArrayStructItem)->iSize.Length() > 0)
		{
			NumericValue v((yyvsp[(1) - (5)].pArrayStructItem)->iSize, L_LONG);
			REGISTER_LINE;
			if((yyvsp[(4) - (5)].pStringArray)->Size()!=long(v.GetULong()))
			{
				ErrorHandler::OutputErrorLine("Size does not match number of initialisers");
				exit(1);
			}
		}
		(yyval.pStructItem) = (yyvsp[(1) - (5)].pArrayStructItem);
		delete (yyvsp[(4) - (5)].pStringArray);
		}
		break;

	case 46:

		{	if(verbose) {	MOFF;cout << "array_struct_item_base                     with no size" << endl;MON;}
		(yyval.pArrayStructItem) =(yyvsp[(1) - (2)].pArrayStructItem);
		}
		break;

	case 47:

		{	if(verbose) {	MOFF;cout << "array_struct_item_base                     with size " << (yyvsp[(2) - (3)].Value) << endl;MON;}
		(yyvsp[(1) - (3)].pArrayStructItem)->iSize = (yyvsp[(2) - (3)].Value);
		(yyval.pArrayStructItem) = (yyvsp[(1) - (3)].pArrayStructItem);
		}
		break;

	case 48:

		{	if(verbose) 
		{		 	RCTypeArray Types;
		MOFF;cout << "array_struct_item_base                     with LenType " << Types.GetName((yyvsp[(2) - (4)].Id)) << endl;MON;
		}
		(yyvsp[(3) - (4)].pArrayStructItem)->iLenType = (yyvsp[(2) - (4)].Id);
		(yyval.pArrayStructItem) = (yyvsp[(3) - (4)].pArrayStructItem);
		}
		break;

	case 49:

		{	if(verbose) 
		{		 	RCTypeArray Types;
		MOFF;cout << "array_struct_item_base                     with size " << (yyvsp[(4) - (5)].Value) << " and LenType " << Types.GetName((yyvsp[(2) - (5)].Id)) << endl;MON;
		}
		(yyvsp[(3) - (5)].pArrayStructItem)->iLenType = (yyvsp[(2) - (5)].Id);
		(yyvsp[(3) - (5)].pArrayStructItem)->iSize = (yyvsp[(4) - (5)].Value); 
		(yyval.pArrayStructItem) = (yyvsp[(3) - (5)].pArrayStructItem); 
		}
		break;

	case 50:

		{	if(verbose) 
		{		 	RCTypeArray Types; 
		MOFF;cout << "array_struct_item_start                    " << Types.GetName((yyvsp[(1) - (3)].Id)) << " " << (yyvsp[(2) - (3)].Value) << endl;MON;
		}
		(yyval.pArrayStructItem) = new ArrayStructItem((yyvsp[(1) - (3)].Id), (yyvsp[(2) - (3)].Value));
		}
		break;

	case 51:

		{	(yyval.Id) = L_BYTE;}
		break;

	case 52:

		{	(yyval.Id) = L_WORD;}
		break;

	case 53:

		{	if(verbose) {	MOFF;cout << "struct_type_struct_item                    " << (yyvsp[(2) - (2)].Value) << endl;MON;}
		(yyval.pStructItem) = new StructTypeStructItem((yyvsp[(2) - (2)].Value));
		}
		break;

	case 54:

		{	(yyval.pStructItem) = (yyvsp[(1) - (1)].pStructArrayStructItem);}
		break;

	case 55:

		{	if(verbose) {	RCTypeArray Types; MOFF;cout << "struct_array_struct_item                   - Setting Size to " << Types.GetName((yyvsp[(2) - (3)].Id)) << endl;MON;}
		(yyvsp[(3) - (3)].pStructArrayStructItem)->iLenType = (yyvsp[(2) - (3)].Id); (yyval.pStructItem) = (yyvsp[(3) - (3)].pStructArrayStructItem);
		}
		break;

	case 56:

		{	if(verbose) {	MOFF;cout << "struct_array_struct_item_base              " << (yyvsp[(2) - (4)].Value) << endl;MON;}
		(yyval.pStructArrayStructItem) = new StructArrayStructItem((yyvsp[(2) - (4)].Value));
		}
		break;

	case 57:

		{	if(verbose) {	MOFF;cout << "struct_array_struct_item_base              " << (yyvsp[(2) - (5)].Value) << " " << (yyvsp[(4) - (5)].Value) << endl;MON;}
		(yyval.pStructArrayStructItem) = new StructArrayStructItem((yyvsp[(2) - (5)].Value), (yyvsp[(4) - (5)].Value));
		}
		break;

	case 58:

		{	
			pResourceHeader->AddDefault();
			CurrentId+=CurrentIdStep;
			if(verbose) { MOFF;cout << "Resource ID "<< CurrentId << endl << Divider << "\n" << * pResourceHeader << Divider << endl;MON;}
			pResourceHeader->SetResourceId(*pResourceNameIds,CurrentId,FormatIdAsHex);
			pG->Index.Add(pResourceHeader);
			pResourceHeader = NULL;
		}
		break;

	case 59:

		{	uid2Set = true; uid2 = (yyvsp[(2) - (2)].NumInitialiser); }
		break;

	case 60:

		{	uid3Set = true; uid3 = (yyvsp[(2) - (2)].NumInitialiser); }
		break;

	case 62:

		{	
			if(verbose) { MOFF;cout << "resource_statement_start                   LOCAL" << endl;MON;}
			assert(pResourceHeader != NULL);
			pResourceHeader->iLocal = 1;
		}
		break;

	case 64:

		{	if(verbose) {	MOFF;cout << "resource_statement_start_names             " << (yyvsp[(2) - (3)].Value) << " " << (yyvsp[(3) - (3)].Value) << endl;MON;}
		assert(pResourceHeader == NULL);
		pResourceHeader = new ResourceHeader((yyvsp[(3) - (3)].Value));
		pCurrentRIA = & (pResourceHeader->iRIA);
		REGISTER_LINE;
		if(pResourceNameIds->IsStored((yyvsp[(3) - (3)].Value)))
		{
			ErrorHandler::OutputErrorLine("Resource with this name encountered already");
			exit(1);
		}
		pCurrentRIA->FillFromStruct((yyvsp[(2) - (3)].Value));
		}
		break;

	case 65:

		{	if(verbose) {	MOFF;cout << "resource_statement_start_names             " << (yyvsp[(2) - (2)].Value) << " <Resource not named>" << endl;MON;}
		assert(pResourceHeader == NULL);
		pResourceHeader = new ResourceHeader;
		pCurrentRIA = & (pResourceHeader->iRIA);
		REGISTER_LINE;
		pCurrentRIA->FillFromStruct((yyvsp[(2) - (2)].Value));
		}
		break;

	case 66:

		{	if(verbose) {	MOFF;cout << "resource_item_list" << endl;MON;}}
		break;

	case 67:

		{	yyerrok; yyclearin; }
		break;

	case 69:

		{	if(verbose) {	MOFF;cout << "resource_item                              " << (yyvsp[(1) - (3)].Value) << " " << (yyvsp[(3) - (3)].Value) << endl;MON;}
		REGISTER_LINE;/****************************************************************/
		pCurrentRIA->Set((yyvsp[(1) - (3)].Value), (yyvsp[(3) - (3)].Value));
		}
		break;

	case 73:

		{	
			if (verbose) 
			{ MOFF;cout << "resource_simple_array_item                 " << (yyvsp[(1) - (4)].Value) << endl;MON;} 
		}
		break;

	case 74:

		{	
			if (verbose) 
			{ MOFF;cout << "resource_simple_array_item                 " << (yyvsp[(1) - (5)].Value) << " with simple_initialiser_list" << endl;MON;}
			REGISTER_LINE;
			pCurrentRIA->Set((yyvsp[(1) - (5)].Value), * (yyvsp[(4) - (5)].pStringArray));
			delete (yyvsp[(4) - (5)].pStringArray);
		}
		break;

	case 75:

		{	if(verbose) {	MOFF;cout << "struct_resource_item" << endl;MON;}
		pCurrentRIA = pG->RIAStack.Pop();
		}
		break;

	case 76:

		{	if(verbose) {	MOFF;cout << "struct_resource_item_start                 " << (yyvsp[(1) - (4)].Value) << " " << (yyvsp[(3) - (4)].Value) << endl;MON;}
		REGISTER_LINE;
		pCurrentRIA->Set((yyvsp[(1) - (4)].Value), (yyvsp[(3) - (4)].Value));
		pG->RIAStack.Push(pCurrentRIA);
		pCurrentRIA = pCurrentRIA->Find((yyvsp[(1) - (4)].Value))->GetRIA();
		}
		break;

	case 77:

		{	if(verbose) {	MOFF;cout << "struct_array_resource_item" << endl;MON;}
		pG->SRIStack.Pop();
		}
		break;

	case 78:

		{	pG->SRIStack.Pop();}
		break;

	case 79:

		{	if(verbose) {	MOFF;cout << "struct_array_resource_item_start           " << (yyvsp[(1) - (5)].Value) << " " << (yyvsp[(4) - (5)].Value) << endl;MON;}
		ResourceItem * p = pCurrentRIA->Find((yyvsp[(1) - (5)].Value));
		pG->SRIStack.Push(p);
		REGISTER_LINE;
		p->Set((yyvsp[(4) - (5)].Value));
		pG->RIAStack.Push(pCurrentRIA);
		pCurrentRIA = p->GetRIA();
		}
		break;

	case 83:

		{	if(verbose) {	MOFF;cout << "struct_array_resource_item_list_top        " << endl;MON;}
		pCurrentRIA = pG->RIAStack.Pop();
		}
		break;

	case 86:

		{	if(verbose) {	MOFF;cout << "struct_array_resource_item_list_item       " << endl;MON;}
		pCurrentRIA = pG->RIAStack.Pop();
		}
		break;

	case 87:

		{	if(verbose) {	MOFF;cout << "struct_array_resource_item_list_item_start " << (yyvsp[(1) - (2)].Value) << endl;MON;}
		ResourceItem * p = pG->SRIStack.Peek();
		REGISTER_LINE;
		p->Set((yyvsp[(1) - (2)].Value));
		pG->RIAStack.Push(pCurrentRIA);
		pCurrentRIA = p->GetRIA();
		}
		break;

	case 89:

		{ 
			// convert literal to unsigned long value of 1st character
			String s((yyvsp[(1) - (1)].Value));
			unsigned short first;
			int length=1;
			if (s.Export(&first, length, SourceCharacterSet)==0)
			{
				REGISTER_LINE;
				ErrorHandler::OutputErrorLine("Warning: Ignoring trailing characters in character literal");
			}
			//_ltoa(first,$$,10);
			sprintf(yyval.Value, "%u", first);
		}
		break;

	case 90:

		{
			if (pG->EnumValues.IsStored((yyvsp[(1) - (1)].Value)))
			{
				//_ltoa(pG->EnumValues.FindId($1),$$,10);
				//sprintf(yyval.Value, "%lu", pG->EnumValues.FindId(yypvt[0].Value));
				sprintf(yyval.Value, "%lu", pG->EnumValues.FindId((yyvsp[(1) - (1)].Value)));
			}
			else
			{
				/*
				Could be a reference to another resource, perhaps even a forward reference:
				the OverwriteLink functions do FindId again when writing out the data.
				Sadly this also permits things which are really syntax errors, inadvertently
				converting labels into string literals..
				*/
			}
		}
		break;

	case 93:

		{
			if(verbose) 
			{	
				MOFF;cout << "simple_initialiser_list                    - single string " << (yyvsp[(1) - (1)].Value) << endl;MON;
			}
			(yyval.pStringArray) = new StringArray;
			(yyval.pStringArray)->Add(new String((yyvsp[(1) - (1)].Value)) );
		}
		break;

	case 94:

		{	if(verbose) {	MOFF;cout << "simple_initialiser_list                    - part of list " << (yyvsp[(3) - (3)].Value) << endl;MON;}
		assert((yyvsp[(1) - (3)].pStringArray) != NULL);
		(yyvsp[(1) - (3)].pStringArray)->Add(new String((yyvsp[(3) - (3)].Value) ) );
		(yyval.pStringArray) = (yyvsp[(1) - (3)].pStringArray);
		}
		break;

	case 95:

		{	String s(NumericValue::ltoa((yyvsp[(1) - (1)].NumInitialiser)) ); strcpy((yyval.Value), s.GetBuffer() ); }
		break;

	case 96:

		{	if(verbose) {	MOFF;cout << "Converting number " << (yyvsp[(1) - (1)].Value) << endl;MON;}
		REGISTER_LINE;
		NumericValue v((yyvsp[(1) - (1)].Value), L_LONG); (yyval.NumInitialiser) = v.GetLong();
		}
		break;

	case 97:

		{	(yyval.NumInitialiser) = (yyvsp[(1) - (3)].NumInitialiser) + (yyvsp[(3) - (3)].NumInitialiser);	}
		break;

	case 98:

		{	(yyval.NumInitialiser) = (yyvsp[(1) - (3)].NumInitialiser) - (yyvsp[(3) - (3)].NumInitialiser);	}
		break;

	case 99:

		{	(yyval.NumInitialiser) = (yyvsp[(1) - (3)].NumInitialiser) * (yyvsp[(3) - (3)].NumInitialiser);	}
		break;

	case 100:

		{	(yyval.NumInitialiser) = (yyvsp[(1) - (3)].NumInitialiser) / (yyvsp[(3) - (3)].NumInitialiser);	}
		break;

	case 101:

		{	(yyval.NumInitialiser) = (yyvsp[(1) - (3)].NumInitialiser) | (yyvsp[(3) - (3)].NumInitialiser);	}
		break;

	case 102:

		{	(yyval.NumInitialiser) = - (yyvsp[(2) - (2)].NumInitialiser);		}
		break;

	case 103:

		{	(yyval.NumInitialiser) = (yyvsp[(2) - (3)].NumInitialiser);		}
		break;

	case 105:

		{
			if (strlen((yyval.Value))+strlen((yyvsp[(2) - (2)].Value)) > sizeof((yyval.Value))-1)
			{
				REGISTER_LINE;
				ErrorHandler::OutputErrorLine("String expression is too long");
				exit(1);
			}
			strcat((yyval.Value), (yyvsp[(2) - (2)].Value));
		}
		break;

	case 108:

		{	
			REGISTER_LINE;
			if((yyvsp[(2) - (3)].NumInitialiser) < 0 || ((yyvsp[(2) - (3)].NumInitialiser) > 255 && TargetCharacterSet != String::Unicode))
			{
				ErrorHandler::OutputErrorLine("Character code must be a number in the range 0 to 255.");
				exit(1);
			}
			if((yyvsp[(2) - (3)].NumInitialiser) > 0xffff)
			{
				ErrorHandler::OutputErrorLine("Unicode character code must be a number in the range 0 to 65535.");
				exit(1);
			}
			if (TargetCharacterSet != String::Unicode)
			{
				* (yyval.Value) = char((yyvsp[(2) - (3)].NumInitialiser)); * ((yyval.Value) + 1) = '\0'; 
			} 
			else
			{
				if (SourceCharacterSet == String::CP1252)
				{
					if ( ((yyvsp[(2) - (3)].NumInitialiser) >= 0x80) && ((yyvsp[(2) - (3)].NumInitialiser) <= 0x9F ) ) // 80-9F are illegal Unicode values.
					{
						ErrorHandler::OutputErrorLine("Warning: Deprecated non-unicode value in source stream");
					}
					* (yyval.Value) = char(UnicodeEscape);
					asUTF8((yyval.Value) + 1, (yyvsp[(2) - (3)].NumInitialiser));
				}
				else
					if (SourceCharacterSet == String::UTF8)
					{
						asUTF8((yyval.Value), (yyvsp[(2) - (3)].NumInitialiser));
					}
					else
					{
						// Unsatisfactory, but do people use other character sets?
						if ((yyvsp[(2) - (3)].NumInitialiser) > 255)
						{
							ErrorHandler::OutputErrorLine("Don't know how to handle character > 255");
						}
						* (yyval.Value) = char((yyvsp[(2) - (3)].NumInitialiser)); * ((yyval.Value) + 1) = '\0'; 
					}
			}
		}
		break;

	case 109:

		{
			REGISTER_LINE;
			SetIdFromName((yyvsp[(2) - (2)].Value));
		}
		break;

	case 110:

		{
			REGISTER_LINE;
			SetIdFromName((yyvsp[(2) - (2)].Value));
		}
		break;

	case 111:

		{	if(verbose) {	MOFF;cout << "character_set_statement                    " << (yyvsp[(2) - (2)].Value) << endl;MON;}
		REGISTER_LINE;
		SourceCharacterSet = CharacterSetID((yyvsp[(2) - (2)].Value));
		if ( SourceCharacterSet == String::UNKNOWN )
		{
			String err = "Warning: Unrecognised character set name '";
			err += (yyvsp[(2) - (2)].Value);
			err += "'";
			ErrorHandler::OutputErrorLine(err);
		}
		if ( SourceCharacterSet == String::Unicode )
		{
			SourceCharacterSet = String::UNKNOWN;
			ErrorHandler::OutputErrorLine("Unicode source is unsupported");
		}
		}
		break;

	case 112:

		{	if(verbose) {	RCTypeArray Types;
		MOFF;cout << "offset_statement                           " << (yyvsp[(2) - (2)].Value) << endl;MON; }
		REGISTER_LINE;
		CurrentId=((long) NumericValue((yyvsp[(2) - (2)].Value), L_LONG).GetULong() );
		}
		break;

	case 113:

		{	if(verbose) {	MOFF;cout << "system_statement" << endl;MON;}
		CurrentIdStep=-1;
		}
		break;

	case 116:

		{	
			if(verbose) 
			{ MOFF;cout << "enum_statement" << endl;MON;} 
			CurrentEnumName = (yyvsp[(2) - (3)].Value);
			CurrentEnumValue=0;
		}
		break;

	case 117:

		{	
			if(verbose) 
			{ MOFF;cout << "enum_statement" << endl;MON;} 
			CurrentEnumName = "";
			CurrentEnumValue=0;
		}
		break;

	case 118:

		{	pG->EnumValues.Add((yyvsp[(1) - (1)].Value), CurrentEnumValue++);}
		break;

	case 119:

		{	
			CurrentEnumValue = atol((yyvsp[(3) - (3)].Value));
			pG->EnumValues.Add((yyvsp[(1) - (3)].Value), CurrentEnumValue);
			CurrentEnumValue++;			// Increment so that next field has value ($3+1)
		}
		break;

	case 120:

		{}
		break;

	case 121:

		{}
		break;


		/* Line 1267 of yacc.c.  */

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
		/* If just tried and failed to reuse look-ahead token after an
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

	/* Else will try to reuse look-ahead token after shifting the error
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

	if (yyn == YYFINAL)
		YYACCEPT;

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

#ifndef yyoverflow
	/*-------------------------------------------------.
	| yyexhaustedlab -- memory exhaustion comes here.  |
	`-------------------------------------------------*/
yyexhaustedlab:
	yyerror (YY_("memory exhausted"));
	yyresult = 2;
	/* Fall through.  */
#endif

yyreturn:
	if (yychar != YYEOF && yychar != YYEMPTY)
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





// Function section
// ================

void asUTF8(char* aUtf8, int aUnicode)
{
	if ((aUnicode & 0xff80) == 0x0000)
	{
		*aUtf8 = (char)aUnicode;
	}
	else if ((aUnicode & 0xf800) == 0x0000)
	{
		*aUtf8++ =(char)(0xc0|(aUnicode>>6));
		*aUtf8   =(char)(0x80|(aUnicode&0x3f));
	}
	else
	{
		*aUtf8++ =(char)(0xe0|(aUnicode>>12));
		*aUtf8++ =(char)(0x80|((aUnicode>>6)&0x3f));
		*aUtf8   =(char)(0x80|(aUnicode&0x3f));
	}
	*++aUtf8 = '\0';
}


String::CharacterSet CharacterSetID( const String & character_set_name )
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Return a character set ID from a character set name.  The value UNKNOWN
// is returned if the name is not recognised.
// ----------------------------------------------------------------------------
{
	String::CharacterSet ids[] = { String::ISOLatin1, String::ASCII, String::CP1252
		, String::CP850, String::ShiftJIS, String::Unicode
		, String::UTF8
		, String::UNKNOWN
	};
	String names[] = { "ISOLATIN1", "ASCII", "CP1252", "CP850", "SHIFTJIS", "UNICODE", "UTF8" };

	for ( int i=0; ids[i]!=String::UNKNOWN; i++ )
	{
		if ( names[i] == character_set_name ) return ids[i];
	}

	return String::UNKNOWN;

} // end of CharacterSetID code

void SetIdFromName( const String & NameStatementValue)
{
	// space 	0
	// A		1
	// B		2
	// ...
	// Z		26
	//
	// ABCD corresponds to the number 4321 which becomes ( (4*27 + 3) * 27 + 2) * 27 + 1.

	if(verbose) 
	{ MOFF;cout << "name_statement                             " << NameStatementValue << endl;MON;}
	if ( NameStatementValue.Length() > 4)
	{
		ErrorHandler::OutputErrorLine( "Name must be no longer than four characters");
		exit( 1);
	}

	long NewId = 0;

	for( unsigned long i = 0; i < NameStatementValue.Length(); i++)
	{
		NewId *= 27;
		if ( isalpha( NameStatementValue[i]) )
			NewId += toupper( NameStatementValue[i]) - 'A' + 1;
	}

	CurrentId = NewId << 12;
	FormatIdAsHex = 1;
	if(verbose) 
	{ MOFF;cout << "Current id                                " << CurrentId << endl;MON;}
}


int ParseSourceFile(FILE* aFile, unsigned short aYYDebug)
{
	// Set up various global pointers which refer to the pG structure
	pSHA = & (pG->SHA);
	pFileLineHandler = & (pG->FileLineHandler);
	pResourceNameIds = & (pG->ResourceNameIds);
	pLinks = & (pG->Links);
	/*
	rcscan * pScan = new rcscan(pG->FileLineHandler, aFile);
	yy_parse * pParse = new yy_parse(1000);

	if (pScan==NULL || pParse==NULL)
	return -4;	// KErrNoMemory;

	pParse->yydebug = aYYDebug;
	pCurrentLineNumber = & pScan->yylineno;

	int ReturnValue = pParse->yyparse(pScan);
	*/

	pCurrentLineNumber = & yylineno;
	yyrestart(aFile);
	int ReturnValue = yyparse();

	//	int bScanErrorFound = pScan->ErrorWasFound();
	int bScanErrorFound = errorWasFound;

	//	delete pScan;
	//	delete pParse;

	if(ReturnValue != 0)
		return ReturnValue;

	if(bScanErrorFound)
		return 1;

	return 0;	// successful parse - parse tree now in the pG data structure
}



