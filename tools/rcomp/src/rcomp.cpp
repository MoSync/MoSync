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
//#line 1 "rcomp.yacc"

#include <assert.h>
#include <ctype.h>
#include <string.h>

#if defined(__MSVCDOTNET__) || defined(__TOOLS2__)
#include <fstream>
#include <iostream>
using namespace std;
using std::cout;
using std::endl;
#else //!__MSVCDOTNET__
#include <fstream.h>
#endif //__MSVCDOTNET__

#ifdef __VC32__
#pragma warning( disable : 4065 ) // C4065: switch statement contains 'default' but no 'case' labels
#pragma warning( disable : 4102 ) // C4102: 'yyerrlab1' : unreferenced label
#pragma warning( disable : 4127 ) // C4127: conditional expression is constant
#pragma warning( disable : 4244 ) // C4244: '=' : conversion from 'int' to 'short', possible loss of data
#endif //__VC32__

#include "resource.h"
#include "parser.h"

int yylex();
void yyerror(const char* string, ...);
int yywrap();
#define YYDEBUG 1
extern int yylineno;

#include "rcomp.hpp"
#include "datatype.h"
#include "mem.h"
#include "rcbinstr.h"
#include "rcscan.h"
#include "errorhan.h"
#include "fileacc.h"
#include "version.h"
#include "ctable.h"
#include "localise.h"
#include "main.h"

#if defined(__VC32__) && !defined(_DEBUG)
#pragma warning( disable : 4702 )	// unreachable code
#pragma warning( disable : 4102 )	// 'yyerrlabel' : unreferenced label
#pragma warning( disable : 4244 )	// '=' : conversion from 'int' to 'short', possible loss of data
#endif



String::CharacterSet CharacterSetID( const String & character_set_name );
void asUTF8(char* aUtf8, int aUnicode);
void SetIdFromName( const String & NameStatementValue);
void CheckStructUsage();

unsigned short & d = MemCheckControl::iLogMemory;

StructHeader *		pSH;

StructHeaderArray * 	pSHA;	// Used in resource struct handling functions.
ResourceHeader *	pResourceHeader;
ResourceItemArray *	pCurrentRIA;
StringArray * pUsedLabelsArray = new StringArray();
int			verbose;
String::CharacterSet	SourceCharacterSet = String::CP1252;
String::CharacterSet	TargetCharacterSet = String::CP1252;
unsigned short		logmemorysetting;
int *			pCurrentLineNumber;
FileLineManager *	pFileLineHandler;
NameIdMap *		pResourceNameIds;
long			CurrentEnumValue;
String			CurrentEnumName;
char			TempStr[300];
rcscan * pScan;

int CurrentIdStep=1;
long CurrentId=0;
int FormatIdAsHex=0;	// defaults to decimal, changes in SetIdFromName

unsigned long Uid2=0;
unsigned long Uid3=0;



const String	Divider("*******************************************");

#define REGISTER_LINE	ErrorHandler::Register(pFileLineHandler->GetCurrentFile(), pFileLineHandler->GetErrorLine(* pCurrentLineNumber))

// Convert a string containing a character literal in aQuoted
// into a value suitable for LCHAR_LITERAL
static void SetCharacterLiteral(char* aOut, const String& aQuoted)
	{
	    UTF16 first;
	    int length=1;
	    if (aQuoted.Length() < 1 ) 
	               { 
                        REGISTER_LINE;
                        ErrorHandler::OutputErrorLine("Warning: Empty Character literal");
                       }
	    if (aQuoted.Length() > 1 ) 
	 	       {
                        REGISTER_LINE;
                        ErrorHandler::OutputErrorLine("Error: String Literal length greater than 1");
			exit(1);
                       }
	    if (aQuoted.Export(&first, length, SourceCharacterSet)==0)
			{
			REGISTER_LINE;
			ErrorHandler::OutputErrorLine("Warning: Ignoring trailing characters in character literal");
			}
	    sprintf(aOut, "%d", first);
	}



/* Line 189 of yacc.c  */
//#line 205 "rcomp.tab.cacc"

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
     L_UID_TWO = 282,
     L_UID_THREE = 283,
     L_RLS_STRING = 284,
     L_RLS_STRING8 = 285,
     L_RLS_DOUBLE = 286,
     L_RLS_BYTE = 287,
     L_RLS_WORD = 288,
     L_RLS_LONG = 289,
     L_MULTI = 290,
     L_TAG_START = 291,
     L_TAG_END = 292,
     L_TAG_COMMAND = 293,
     L_TAG_WORD = 294,
     L_TAG_NEW_LINE = 295,
     L_LABEL = 296,
     L_NUM_NATURAL = 297,
     L_NUM_FLOAT = 298,
     L_NATURAL_EXPR = 299,
     L_ENUM = 300,
     L_LEN = 301,
     L_CHAR_LITERAL = 302,
     L_STRING_LITERAL = 303,
     UMINUS = 304
   };
#endif



#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
{

/* Line 214 of yacc.c  */
//#line 132 "rcomp.yacc"

	char				Value[1024*8];
	TValueMaybeRls			ValueMaybeRls;
	unsigned long 			Id;
	StructItem *			pStructItem;
	SimpleStructItem *		pSimpleStructItem;
	ArrayStructItem *   	pArrayStructItem;
	StructArrayStructItem *	pStructArrayStructItem;
	StringArray *			pStringArray;
	long					NumInitialiser;
	TRlsQualifiers		RlsQualifiers;
	TRlsType			RlsType;



/* Line 214 of yacc.c  */
//#line 306 "rcomp.tab.cacc"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif


/* Copy the second part of user declarations.  */


/* Line 264 of yacc.c  */
//#line 318 "rcomp.tab.cacc"

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
#define YYLAST   303

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  66
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  61
/* YYNRULES -- Number of rules.  */
#define YYNRULES  152
/* YYNRULES -- Number of states.  */
#define YYNSTATES  255

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   304

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
      58,    59,    51,    49,    65,    50,     2,    52,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,    55,
      61,    60,    62,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    64,     2,    63,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    57,    53,    56,     2,     2,     2,     2,
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
      45,    46,    47,    48,    54
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint16 yyprhs[] =
{
       0,     0,     3,     5,     8,    11,    12,    15,    18,    21,
      24,    27,    30,    32,    35,    38,    40,    41,    45,    50,
      56,    62,    66,    71,    72,    74,    76,    78,    80,    82,
      87,    91,    98,   101,   107,   109,   111,   113,   115,   117,
     119,   121,   123,   125,   127,   129,   131,   133,   135,   137,
     139,   141,   147,   150,   154,   159,   165,   169,   171,   173,
     176,   178,   182,   187,   193,   198,   201,   204,   206,   210,
     213,   217,   222,   226,   227,   231,   233,   235,   237,   242,
     248,   252,   257,   261,   265,   271,   273,   277,   281,   284,
     286,   290,   294,   297,   299,   301,   303,   305,   307,   311,
     313,   315,   319,   323,   327,   331,   335,   338,   342,   344,
     347,   349,   351,   353,   357,   360,   363,   366,   369,   372,
     375,   377,   381,   386,   390,   393,   395,   399,   402,   407,
     412,   417,   422,   427,   432,   434,   439,   441,   443,   444,
     446,   448,   450,   452,   454,   456,   458,   459,   463,   466,
     467,   469,   471
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int8 yyrhs[] =
{
      67,     0,    -1,    68,    -1,    68,    69,    -1,    68,   124,
      -1,    -1,    71,    70,    -1,    85,    70,    -1,   109,    70,
      -1,   107,    70,    -1,   110,    70,    -1,   111,    70,    -1,
     112,    -1,   108,    70,    -1,   116,    70,    -1,    55,    -1,
      -1,    72,    73,    56,    -1,     3,    41,   123,    57,    -1,
       3,    41,    81,   123,    57,    -1,     3,    41,    46,   123,
      57,    -1,    73,    74,    55,    -1,    73,   124,    74,    55,
      -1,    -1,    75,    -1,    78,    -1,    82,    -1,    83,    -1,
      76,    -1,    76,    58,   102,    59,    -1,    76,    60,   100,
      -1,    76,    58,   102,    59,    60,   104,    -1,    77,    41,
      -1,    77,    61,   103,    62,    41,    -1,    13,    -1,    12,
      -1,    14,    -1,    15,    -1,    16,    -1,    17,    -1,    11,
      -1,    22,    -1,    25,    -1,    23,    -1,    26,    -1,    21,
      -1,    24,    -1,    18,    -1,    19,    -1,    20,    -1,    79,
      -1,    79,    60,    57,   101,    56,    -1,    80,    63,    -1,
      80,   102,    63,    -1,    46,    81,    80,    63,    -1,    46,
      81,    80,   102,    63,    -1,    77,    41,    64,    -1,    13,
      -1,    12,    -1,     3,    41,    -1,    84,    -1,    46,    81,
      84,    -1,     3,    41,    64,    63,    -1,     3,    41,    64,
     102,    63,    -1,    86,    57,    88,    56,    -1,     8,    87,
      -1,     9,    87,    -1,    87,    -1,     4,    41,    41,    -1,
       4,    41,    -1,    88,    89,    55,    -1,    88,   124,    89,
      55,    -1,    88,     1,    55,    -1,    -1,    41,    60,   100,
      -1,    90,    -1,    91,    -1,    93,    -1,    41,    60,    57,
      56,    -1,    41,    60,    57,   101,    56,    -1,    92,    88,
      56,    -1,    41,    60,    41,    57,    -1,    94,    95,    56,
      -1,    94,    95,     1,    -1,    41,    60,    57,    41,    57,
      -1,    96,    -1,    96,    65,    97,    -1,    96,    65,     1,
      -1,    88,    56,    -1,    98,    -1,    97,    65,    98,    -1,
      99,    88,    56,    -1,    41,    57,    -1,    43,    -1,    47,
      -1,   104,    -1,   102,    -1,   100,    -1,   101,    65,   100,
      -1,   103,    -1,    42,    -1,   103,    49,   103,    -1,   103,
      50,   103,    -1,   103,    51,   103,    -1,   103,    52,   103,
      -1,   103,    53,   103,    -1,    50,   103,    -1,    58,   103,
      59,    -1,   105,    -1,   105,   104,    -1,    48,    -1,   106,
      -1,    41,    -1,    61,   103,    62,    -1,     5,    41,    -1,
       5,    48,    -1,    27,   103,    -1,    28,   103,    -1,    10,
      41,    -1,     6,   102,    -1,     7,    -1,   113,   115,    56,
      -1,   113,   115,    56,    55,    -1,    45,    41,    57,    -1,
      45,    57,    -1,    41,    -1,    41,    60,   100,    -1,   123,
     114,    -1,   115,    65,   123,   114,    -1,   120,   118,   117,
     104,    -1,   120,   118,   117,    47,    -1,   122,   119,   117,
      43,    -1,   121,   119,   117,    42,    -1,   121,   119,   117,
      47,    -1,    41,    -1,    61,    42,    62,   119,    -1,   119,
      -1,    35,    -1,    -1,    29,    -1,    30,    -1,    32,    -1,
      33,    -1,    34,    -1,    31,    -1,   124,    -1,    -1,    36,
     125,    37,    -1,   125,   126,    -1,    -1,    40,    -1,    38,
      -1,    39,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   182,   182,   189,   190,   191,   195,   196,   197,   198,
     199,   200,   201,   202,   203,   207,   214,   221,   225,   231,
     237,   245,   249,   253,   256,   257,   258,   259,   262,   263,
     268,   273,   289,   297,   312,   313,   314,   315,   319,   325,
     329,   336,   340,   344,   345,   346,   347,   348,   349,   350,
     353,   354,   372,   375,   380,   388,   399,   407,   408,   411,
     416,   417,   423,   426,   435,   451,   452,   458,   461,   474,
     483,   484,   485,   486,   489,   493,   494,   495,   498,   503,
     531,   537,   548,   552,   556,   569,   570,   571,   574,   579,
     580,   583,   589,   603,   604,   609,   610,   613,   623,   632,
     635,   639,   640,   641,   642,   643,   644,   652,   655,   656,
     667,   668,   669,   748,   794,   799,   811,   822,   844,   868,
     879,   888,   889,   892,   899,   909,   914,   925,   926,   933,
     950,   966,   973,   980,   991,  1012,  1018,  1023,  1029,  1036,
    1038,  1043,  1045,  1047,  1052,  1060,  1061,  1065,  1069,  1071,
    1075,  1076,  1077
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
  "L_BUF16", "L_TEXT16", "L_LTEXT16", "L_UID_TWO", "L_UID_THREE",
  "L_RLS_STRING", "L_RLS_STRING8", "L_RLS_DOUBLE", "L_RLS_BYTE",
  "L_RLS_WORD", "L_RLS_LONG", "L_MULTI", "L_TAG_START", "L_TAG_END",
  "L_TAG_COMMAND", "L_TAG_WORD", "L_TAG_NEW_LINE", "L_LABEL",
  "L_NUM_NATURAL", "L_NUM_FLOAT", "L_NATURAL_EXPR", "L_ENUM", "L_LEN",
  "L_CHAR_LITERAL", "L_STRING_LITERAL", "'+'", "'-'", "'*'", "'/'", "'|'",
  "UMINUS", "';'", "'}'", "'{'", "'('", "')'", "'='", "'<'", "'>'", "']'",
  "'['", "','", "$accept", "source", "statement_list", "statement",
  "maybe_semicolon", "struct_statement", "struct_statement_start",
  "struct_item_list", "struct_item", "simple_struct_item",
  "simple_struct_item_start", "data_type", "array_struct_item",
  "array_struct_item_base", "array_struct_item_start", "len_declaration",
  "struct_type_struct_item", "struct_array_struct_item",
  "struct_array_struct_item_base", "resource_statement",
  "resource_statement_start", "resource_statement_start_names",
  "resource_item_list", "resource_item", "resource_simple_array_item",
  "struct_resource_item", "struct_resource_item_start",
  "struct_array_resource_item", "struct_array_resource_item_start",
  "struct_array_resource_item_list_top",
  "struct_array_resource_item_list_top_start",
  "struct_array_resource_item_list",
  "struct_array_resource_item_list_item",
  "struct_array_resource_item_list_item_start", "simple_initialiser",
  "simple_initialiser_list", "natural_expression",
  "natural_expression_numeric", "string_expression",
  "string_expression_item", "character_code_expression", "name_statement",
  "uidX_statement", "character_set_statement", "offset_statement",
  "system_statement", "enum_statement", "enum_statement_start",
  "enum_list_entry", "enum_list", "rls_item_statement", "rls_label",
  "rls_qualifiers", "rls_cardinality", "rls_string_item", "rls_num_item",
  "rls_float_item", "maybe_comment_tag", "comment_tag", "tag_line",
  "tag_word", 0
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
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,   301,   302,   303,    43,
      45,    42,    47,   124,   304,    59,   125,   123,    40,    41,
      61,    60,    62,    93,    91,    44
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    66,    67,    68,    68,    68,    69,    69,    69,    69,
      69,    69,    69,    69,    69,    70,    70,    71,    72,    72,
      72,    73,    73,    73,    74,    74,    74,    74,    75,    75,
      75,    75,    76,    76,    77,    77,    77,    77,    77,    77,
      77,    77,    77,    77,    77,    77,    77,    77,    77,    77,
      78,    78,    79,    79,    79,    79,    80,    81,    81,    82,
      83,    83,    84,    84,    85,    86,    86,    86,    87,    87,
      88,    88,    88,    88,    89,    89,    89,    89,    90,    90,
      91,    92,    93,    93,    94,    95,    95,    95,    96,    97,
      97,    98,    99,   100,   100,   100,   100,   101,   101,   102,
     103,   103,   103,   103,   103,   103,   103,   103,   104,   104,
     105,   105,   105,   106,   107,   107,   108,   108,   109,   110,
     111,   112,   112,   113,   113,   114,   114,   115,   115,   116,
     116,   116,   116,   116,   117,   118,   118,   119,   119,   120,
     120,   121,   121,   121,   122,   123,   123,   124,   125,   125,
     126,   126,   126
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     1,     2,     2,     0,     2,     2,     2,     2,
       2,     2,     1,     2,     2,     1,     0,     3,     4,     5,
       5,     3,     4,     0,     1,     1,     1,     1,     1,     4,
       3,     6,     2,     5,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     5,     2,     3,     4,     5,     3,     1,     1,     2,
       1,     3,     4,     5,     4,     2,     2,     1,     3,     2,
       3,     4,     3,     0,     3,     1,     1,     1,     4,     5,
       3,     4,     3,     3,     5,     1,     3,     3,     2,     1,
       3,     3,     2,     1,     1,     1,     1,     1,     3,     1,
       1,     3,     3,     3,     3,     3,     2,     3,     1,     2,
       1,     1,     1,     3,     2,     2,     2,     2,     2,     2,
       1,     3,     4,     3,     2,     1,     3,     2,     4,     4,
       4,     4,     4,     4,     1,     4,     1,     1,     0,     1,
       1,     1,     1,     1,     1,     1,     0,     3,     2,     0,
       1,     1,     1
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       5,     0,     2,     1,     0,     0,     0,     0,   120,     0,
       0,     0,     0,     0,   139,   140,   144,   141,   142,   143,
     149,     0,     3,    16,    23,    16,     0,    67,    16,    16,
      16,    16,    16,    12,   146,    16,   138,   138,   138,     4,
     146,    69,   114,   115,   100,     0,     0,   119,    99,    65,
      66,   118,   116,   117,     0,     0,   124,    15,     6,     0,
       7,    73,     9,    13,     8,    10,    11,     0,     0,   145,
      14,   137,     0,     0,   136,     0,     0,    58,    57,   146,
     146,     0,    68,   106,     0,     0,     0,     0,     0,     0,
     147,   151,   152,   150,   148,   123,     0,    40,    35,    34,
      36,    37,    38,    39,    47,    48,    49,    45,    41,    43,
      46,    42,    44,     0,    17,     0,    24,    28,     0,    25,
      50,     0,    26,    27,    60,     0,     0,   121,   146,   125,
     127,     0,   134,     0,     0,     0,     0,     0,    18,   107,
     101,   102,   103,   104,   105,    59,     0,    21,     0,     0,
      32,     0,     0,    52,     0,     0,     0,     0,    64,     0,
      75,    76,    73,    77,    73,     0,   122,     0,     0,   138,
     112,   130,   110,     0,   129,   108,   111,   132,   133,   131,
      20,    19,     0,     0,     0,     0,    61,     0,    93,    94,
      30,    96,    95,    56,     0,     0,    53,    22,    72,     0,
      70,     0,     0,     0,    85,     0,   128,   126,   135,     0,
     109,    62,     0,     0,     0,    54,     0,    29,     0,    97,
       0,   112,     0,    74,    80,    88,    83,    82,     0,    71,
     113,    63,    55,     0,    33,    51,     0,    81,   112,    78,
       0,    87,     0,    86,    89,    73,    31,    98,    84,    79,
      92,     0,     0,    90,    91
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     1,     2,    22,    58,    23,    24,    59,   115,   116,
     117,   118,   119,   120,   121,    80,   122,   123,   124,    25,
      26,    27,   126,   159,   160,   161,   162,   163,   164,   203,
     204,   243,   244,   245,   219,   220,   191,    48,   192,   175,
     176,    28,    29,    30,    31,    32,    33,    34,   130,    67,
      35,   133,    73,    74,    36,    37,    38,    68,    69,    54,
      94
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -156
static const yytype_int16 yypact[] =
{
    -156,    20,   215,  -156,   -18,    15,    21,    41,  -156,    13,
      13,    63,    41,    41,  -156,  -156,  -156,  -156,  -156,  -156,
    -156,     4,  -156,    27,  -156,    27,    59,  -156,    27,    27,
      27,    27,    27,  -156,    -5,    27,   -21,    37,    37,  -156,
      17,    78,  -156,  -156,  -156,    41,    41,  -156,   145,  -156,
    -156,  -156,   145,   145,    74,    66,  -156,  -156,  -156,   132,
    -156,  -156,  -156,  -156,  -156,  -156,  -156,    32,    96,  -156,
    -156,  -156,    97,   118,  -156,   118,   118,  -156,  -156,    -5,
      -5,   124,  -156,  -156,    81,    41,    41,    41,    41,    41,
    -156,  -156,  -156,  -156,  -156,  -156,   141,  -156,  -156,  -156,
    -156,  -156,  -156,  -156,  -156,  -156,  -156,  -156,  -156,  -156,
    -156,  -156,  -156,    80,  -156,   129,  -156,    62,   -19,  -156,
     125,   -26,  -156,  -156,  -156,   190,    10,   131,    -5,   127,
    -156,   128,  -156,    23,    -9,   146,   134,   135,  -156,  -156,
      34,    34,   164,   164,  -156,   162,   250,  -156,    41,   236,
     165,    41,   171,  -156,   172,   183,   184,   180,  -156,   186,
    -156,  -156,  -156,  -156,  -156,   211,  -156,    96,   236,    37,
    -156,  -156,  -156,    41,  -156,   -20,  -156,  -156,  -156,  -156,
    -156,  -156,   -24,   213,   214,    31,  -156,   197,  -156,  -156,
    -156,  -156,  -156,  -156,    76,   236,  -156,  -156,  -156,    60,
    -156,    11,    18,    12,   192,   203,  -156,  -156,  -156,   121,
    -156,  -156,   196,   162,   165,  -156,   217,   221,   241,  -156,
      40,   228,   119,  -156,  -156,  -156,  -156,  -156,    14,  -156,
    -156,  -156,  -156,   -20,  -156,  -156,   236,  -156,   230,  -156,
      44,  -156,   231,   224,  -156,  -156,  -156,  -156,  -156,  -156,
    -156,   249,    24,  -156,  -156
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -156,  -156,  -156,  -156,   202,  -156,  -156,  -156,   166,  -156,
    -156,   147,  -156,  -156,   149,   179,  -156,  -156,   150,  -156,
    -156,   155,  -155,   133,  -156,  -156,  -156,  -156,  -156,  -156,
    -156,  -156,    48,  -156,  -141,    79,    -6,   -10,  -127,  -156,
    -156,  -156,  -156,  -156,  -156,  -156,  -156,  -156,   136,  -156,
    -156,   -32,  -156,   -33,  -156,  -156,  -156,   -30,    -2,  -156,
    -156
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -1
static const yytype_uint8 yytable[] =
{
      39,    47,    52,    53,    75,    76,   174,   201,   190,   202,
      81,   156,   156,   226,    71,   241,    44,     5,    44,   156,
       3,   170,   150,    40,    45,   156,    45,   207,   172,    77,
      78,    20,    46,   177,    46,    83,    84,   153,   178,   211,
      72,   173,   151,   134,   135,    55,    20,    20,   210,   136,
     137,   157,   157,    20,    20,   242,    41,   125,   223,   157,
      20,    56,    42,    79,   170,   157,   158,   224,   227,    43,
     171,   172,    71,    44,   225,   140,   141,   142,   143,   144,
     254,    45,    57,    44,   173,    87,    88,    89,   127,    46,
     252,    45,    77,    78,   215,   247,   235,   128,   167,    46,
     249,   221,    44,   188,    51,   236,   246,   189,   172,   236,
      45,    90,    91,    92,    93,   154,    61,   222,    46,    82,
     148,   173,   149,    95,   165,    85,    86,    87,    88,    89,
      85,    86,    87,    88,    89,    96,   208,   129,   218,   131,
     139,   194,   187,    97,    98,    99,   100,   101,   102,   103,
     104,   105,   106,   107,   108,   109,   110,   111,   112,   132,
     238,    44,   188,   209,    49,    50,   189,   172,    20,    45,
      85,    86,    87,    88,    89,   239,   212,    46,   113,   216,
     173,   138,   145,   230,   147,   152,   166,   168,   114,   179,
     169,   180,   181,    96,    85,    86,    87,    88,    89,   165,
     165,    97,    98,    99,   100,   101,   102,   103,   104,   105,
     106,   107,   108,   109,   110,   111,   112,    89,     4,     5,
       6,     7,     8,     9,    10,    11,   182,    60,   195,   193,
      62,    63,    64,    65,    66,   196,   113,    70,   197,   198,
     199,   200,    12,    13,    14,    15,    16,    17,    18,    19,
     165,    20,   157,   183,   213,   214,   217,   228,   229,   231,
      21,    97,    98,    99,   100,   101,   102,   103,   104,   105,
     106,   107,   108,   109,   110,   111,   112,   170,    44,   188,
     232,   233,   234,   189,   172,   237,    45,   248,   250,   251,
     242,   155,   146,   184,    46,   185,   186,   173,   205,   253,
       0,   240,     0,   206
};

static const yytype_int16 yycheck[] =
{
       2,     7,    12,    13,    37,    38,   133,   162,   149,   164,
      40,     1,     1,     1,    35,     1,    42,     4,    42,     1,
       0,    41,    41,    41,    50,     1,    50,   168,    48,    12,
      13,    36,    58,    42,    58,    45,    46,    63,    47,    63,
      61,    61,    61,    75,    76,    41,    36,    36,   175,    79,
      80,    41,    41,    36,    36,    41,    41,    59,   199,    41,
      36,    57,    41,    46,    41,    41,    56,    56,    56,    48,
      47,    48,    35,    42,    56,    85,    86,    87,    88,    89,
      56,    50,    55,    42,    61,    51,    52,    53,    56,    58,
     245,    50,    12,    13,    63,   236,    56,    65,   128,    58,
      56,    41,    42,    43,    41,    65,   233,    47,    48,    65,
      50,    37,    38,    39,    40,   121,    57,    57,    58,    41,
      58,    61,    60,    57,   126,    49,    50,    51,    52,    53,
      49,    50,    51,    52,    53,     3,   169,    41,    62,    42,
      59,   151,   148,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    21,    22,    23,    24,    25,    26,    41,
      41,    42,    43,   173,     9,    10,    47,    48,    36,    50,
      49,    50,    51,    52,    53,    56,   182,    58,    46,   185,
      61,    57,    41,    62,    55,    60,    55,    60,    56,    43,
      62,    57,    57,     3,    49,    50,    51,    52,    53,   201,
     202,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    21,    22,    23,    24,    25,    26,    53,     3,     4,
       5,     6,     7,     8,     9,    10,    64,    25,    57,    64,
      28,    29,    30,    31,    32,    63,    46,    35,    55,    55,
      60,    55,    27,    28,    29,    30,    31,    32,    33,    34,
     252,    36,    41,     3,    41,    41,    59,    65,    55,    63,
      45,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    21,    22,    23,    24,    25,    26,    41,    42,    43,
      63,    60,    41,    47,    48,    57,    50,    57,    57,    65,
      41,   125,   113,   146,    58,   146,   146,    61,   165,   251,
      -1,   222,    -1,   167
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,    67,    68,     0,     3,     4,     5,     6,     7,     8,
       9,    10,    27,    28,    29,    30,    31,    32,    33,    34,
      36,    45,    69,    71,    72,    85,    86,    87,   107,   108,
     109,   110,   111,   112,   113,   116,   120,   121,   122,   124,
      41,    41,    41,    48,    42,    50,    58,   102,   103,    87,
      87,    41,   103,   103,   125,    41,    57,    55,    70,    73,
      70,    57,    70,    70,    70,    70,    70,   115,   123,   124,
      70,    35,    61,   118,   119,   119,   119,    12,    13,    46,
      81,   123,    41,   103,   103,    49,    50,    51,    52,    53,
      37,    38,    39,    40,   126,    57,     3,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    21,    22,    23,
      24,    25,    26,    46,    56,    74,    75,    76,    77,    78,
      79,    80,    82,    83,    84,   124,    88,    56,    65,    41,
     114,    42,    41,   117,   117,   117,   123,   123,    57,    59,
     103,   103,   103,   103,   103,    41,    81,    55,    58,    60,
      41,    61,    60,    63,   102,    74,     1,    41,    56,    89,
      90,    91,    92,    93,    94,   124,    55,   123,    60,    62,
      41,    47,    48,    61,   104,   105,   106,    42,    47,    43,
      57,    57,    64,     3,    77,    80,    84,   102,    43,    47,
     100,   102,   104,    64,   103,    57,    63,    55,    55,    60,
      55,    88,    88,    95,    96,    89,   114,   100,   119,   103,
     104,    63,   102,    41,    41,    63,   102,    59,    62,   100,
     101,    41,    57,   100,    56,    56,     1,    56,    65,    55,
      62,    63,    63,    60,    41,    56,    65,    57,    41,    56,
     101,     1,    41,    97,    98,    99,   104,   100,    57,    56,
      57,    65,    88,    98,    56
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
# define YYINITDEPTH 2
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
//#line 182 "rcomp.yacc"
    {	if(verbose)	{	MOFF; cout << Divider << "\n" << Divider << endl; MON; }
										;}
    break;

  case 15:

/* Line 1455 of yacc.c  */
//#line 208 "rcomp.yacc"
    {
		// This is my gift to the world: no more "syntax error" for adding
		// an extra semicolon at the end of a struct or resource.
	    REGISTER_LINE;
	    ErrorHandler::OutputErrorLine("Warning: unnecessary semicolon");
		;}
    break;

  case 17:

/* Line 1455 of yacc.c  */
//#line 222 "rcomp.yacc"
    {	if(verbose) { MOFF; cout << Divider << "\n" << * pSH << Divider << endl; MON;}	;}
    break;

  case 18:

/* Line 1455 of yacc.c  */
//#line 226 "rcomp.yacc"
    {	if(verbose) {	MOFF;cout << "struct_statement_start                     " << (yyvsp[(2) - (4)].Value) << endl; MON;}
											pSH = new StructHeader((yyvsp[(2) - (4)].Value));
											REGISTER_LINE;
											pG->SHA.Add(pSH);
										;}
    break;

  case 19:

/* Line 1455 of yacc.c  */
//#line 232 "rcomp.yacc"
    {	if(verbose) {	RCTypeArray Types; MOFF;cout << "struct_statement_start                     " << (yyvsp[(2) - (5)].Value) << " " << Types.GetName((yyvsp[(3) - (5)].Id)) << endl; MON;}
											pSH = new StructHeader((yyvsp[(2) - (5)].Value), (yyvsp[(3) - (5)].Id));
											REGISTER_LINE;
											pG->SHA.Add(pSH);
										;}
    break;

  case 20:

/* Line 1455 of yacc.c  */
//#line 238 "rcomp.yacc"
    {	if(verbose) {	MOFF;cout << "struct_statement_start                     " << (yyvsp[(2) - (5)].Value) << " (WORD)" << endl; MON;}
											pSH = new StructHeader((yyvsp[(2) - (5)].Value), L_WORD);
											REGISTER_LINE;
											pG->SHA.Add(pSH);
										;}
    break;

  case 21:

/* Line 1455 of yacc.c  */
//#line 245 "rcomp.yacc"
    {	if(verbose) {	MOFF;cout << "struct_item_list                           Adding struct_item." << endl; MON;}
											REGISTER_LINE;
											pSH->iSIA.Add((yyvsp[(2) - (3)].pStructItem));
										;}
    break;

  case 22:

/* Line 1455 of yacc.c  */
//#line 249 "rcomp.yacc"
    {	if(verbose) {	MOFF;cout << "tagged struct_item_list                    Adding struct_item." << endl; MON;}
											REGISTER_LINE;
											pSH->iSIA.Add((yyvsp[(3) - (4)].pStructItem));
										;}
    break;

  case 28:

/* Line 1455 of yacc.c  */
//#line 262 "rcomp.yacc"
    {	(yyval.pStructItem) = (yyvsp[(1) - (1)].pSimpleStructItem);;}
    break;

  case 29:

/* Line 1455 of yacc.c  */
//#line 264 "rcomp.yacc"
    {	if(verbose) { MOFF;cout << "    Limit: " << (yyvsp[(3) - (4)].Value) << endl; MON;}
											(yyvsp[(1) - (4)].pSimpleStructItem)->iLengthLimit = (yyvsp[(3) - (4)].Value);
											(yyval.pStructItem) = (yyvsp[(1) - (4)].pSimpleStructItem);
										;}
    break;

  case 30:

/* Line 1455 of yacc.c  */
//#line 269 "rcomp.yacc"
    {	if(verbose) { MOFF;cout << "    Default: " << (yyvsp[(3) - (3)].Value) << endl; MON;}
											(yyvsp[(1) - (3)].pSimpleStructItem)->iDefault = (yyvsp[(3) - (3)].Value);
											(yyval.pStructItem) = (yyvsp[(1) - (3)].pSimpleStructItem);
										;}
    break;

  case 31:

/* Line 1455 of yacc.c  */
//#line 274 "rcomp.yacc"
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
										;}
    break;

  case 32:

/* Line 1455 of yacc.c  */
//#line 289 "rcomp.yacc"
    {	if(verbose) 
											{		
												   RCTypeArray Types;
														   MOFF;cout << "simple_struct_item                         " << Types.GetName((yyvsp[(1) - (2)].Id)) << " " << (yyvsp[(2) - (2)].Value) << endl; MON;
											}
											(yyval.pSimpleStructItem) = new SimpleStructItem((yyvsp[(1) - (2)].Id),(yyvsp[(2) - (2)].Value)); 
											assert((yyval.pSimpleStructItem) != NULL);
										;}
    break;

  case 33:

/* Line 1455 of yacc.c  */
//#line 298 "rcomp.yacc"
    {	if(verbose) 
											{			   RCTypeArray Types;
														   MOFF;cout << "simple_struct_item                         " << Types.GetName((yyvsp[(1) - (5)].Id)) << " " << (yyvsp[(5) - (5)].Value) << endl; MON;
											}
											String s(NumericValue::ltoa((yyvsp[(3) - (5)].NumInitialiser)));
											(yyval.pSimpleStructItem) = new SimpleStructItem((yyvsp[(1) - (5)].Id),(yyvsp[(5) - (5)].Value),s);
											assert((yyval.pSimpleStructItem) != NULL);
										;}
    break;

  case 34:

/* Line 1455 of yacc.c  */
//#line 312 "rcomp.yacc"
    {	(yyval.Id) = L_BYTE;;}
    break;

  case 35:

/* Line 1455 of yacc.c  */
//#line 313 "rcomp.yacc"
    {	(yyval.Id) = L_WORD;;}
    break;

  case 36:

/* Line 1455 of yacc.c  */
//#line 314 "rcomp.yacc"
    {	(yyval.Id) = L_LONG;;}
    break;

  case 37:

/* Line 1455 of yacc.c  */
//#line 315 "rcomp.yacc"
    {	(yyval.Id) = L_DOUBLE;;}
    break;

  case 38:

/* Line 1455 of yacc.c  */
//#line 320 "rcomp.yacc"
    { 
	    (yyval.Id) = ( TargetCharacterSet == String::Unicode ) ? L_TEXT16: L_TEXT8;
	    REGISTER_LINE;
	    ErrorHandler::OutputErrorLine("Warning: Deprecated use of zero-terminated TEXT - use LTEXT instead");
	    ;}
    break;

  case 39:

/* Line 1455 of yacc.c  */
//#line 326 "rcomp.yacc"
    {
	    (yyval.Id) = ( TargetCharacterSet == String::Unicode ) ? L_LTEXT16: L_LTEXT8;
	    ;}
    break;

  case 40:

/* Line 1455 of yacc.c  */
//#line 330 "rcomp.yacc"
    { 
	    (yyval.Id) = ( TargetCharacterSet == String::Unicode ) ? L_BUF16: L_BUF8;
	    ;}
    break;

  case 41:

/* Line 1455 of yacc.c  */
//#line 336 "rcomp.yacc"
    {	(yyval.Id) = L_TEXT8;
											REGISTER_LINE;
											ErrorHandler::OutputErrorLine("Warning: Deprecated use of zero-terminated TEXT8 - use LTEXT8 instead");
										;}
    break;

  case 42:

/* Line 1455 of yacc.c  */
//#line 340 "rcomp.yacc"
    {	(yyval.Id) = L_TEXT16;
											REGISTER_LINE;
											ErrorHandler::OutputErrorLine("Warning: Deprecated use of zero-terminated TEXT16 - use LTEXT16 instead");
										;}
    break;

  case 43:

/* Line 1455 of yacc.c  */
//#line 344 "rcomp.yacc"
    {	(yyval.Id) = L_LTEXT8;;}
    break;

  case 44:

/* Line 1455 of yacc.c  */
//#line 345 "rcomp.yacc"
    {	(yyval.Id) = L_LTEXT16;;}
    break;

  case 45:

/* Line 1455 of yacc.c  */
//#line 346 "rcomp.yacc"
    {	(yyval.Id) = L_BUF8;;}
    break;

  case 46:

/* Line 1455 of yacc.c  */
//#line 347 "rcomp.yacc"
    {	(yyval.Id) = L_BUF16;;}
    break;

  case 47:

/* Line 1455 of yacc.c  */
//#line 348 "rcomp.yacc"
    {	(yyval.Id) = L_LINK;;}
    break;

  case 48:

/* Line 1455 of yacc.c  */
//#line 349 "rcomp.yacc"
    {	(yyval.Id) = L_LLINK;;}
    break;

  case 49:

/* Line 1455 of yacc.c  */
//#line 350 "rcomp.yacc"
    {	(yyval.Id) = L_SRLINK;;}
    break;

  case 50:

/* Line 1455 of yacc.c  */
//#line 353 "rcomp.yacc"
    {	(yyval.pStructItem) = (yyvsp[(1) - (1)].pArrayStructItem);;}
    break;

  case 51:

/* Line 1455 of yacc.c  */
//#line 355 "rcomp.yacc"
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
										;}
    break;

  case 52:

/* Line 1455 of yacc.c  */
//#line 372 "rcomp.yacc"
    {	if(verbose) {	MOFF;cout << "array_struct_item_base                     with no size" << endl;MON;}
											(yyval.pArrayStructItem) =(yyvsp[(1) - (2)].pArrayStructItem);
										;}
    break;

  case 53:

/* Line 1455 of yacc.c  */
//#line 376 "rcomp.yacc"
    {	if(verbose) {	MOFF;cout << "array_struct_item_base                     with size " << (yyvsp[(2) - (3)].Value) << endl;MON;}
											(yyvsp[(1) - (3)].pArrayStructItem)->iSize = (yyvsp[(2) - (3)].Value);
											(yyval.pArrayStructItem) = (yyvsp[(1) - (3)].pArrayStructItem);
										;}
    break;

  case 54:

/* Line 1455 of yacc.c  */
//#line 381 "rcomp.yacc"
    {	if(verbose) 
												{		 	RCTypeArray Types;
														 	MOFF;cout << "array_struct_item_base                     with LenType " << Types.GetName((yyvsp[(2) - (4)].Id)) << endl;MON;
												}
											(yyvsp[(3) - (4)].pArrayStructItem)->iLenType = (yyvsp[(2) - (4)].Id);
											(yyval.pArrayStructItem) = (yyvsp[(3) - (4)].pArrayStructItem);
										;}
    break;

  case 55:

/* Line 1455 of yacc.c  */
//#line 389 "rcomp.yacc"
    {	if(verbose) 
												{		 	RCTypeArray Types;
														 	MOFF;cout << "array_struct_item_base                     with size " << (yyvsp[(4) - (5)].Value) << " and LenType " << Types.GetName((yyvsp[(2) - (5)].Id)) << endl;MON;
												}
											(yyvsp[(3) - (5)].pArrayStructItem)->iLenType = (yyvsp[(2) - (5)].Id);
											(yyvsp[(3) - (5)].pArrayStructItem)->iSize = (yyvsp[(4) - (5)].Value); 
											(yyval.pArrayStructItem) = (yyvsp[(3) - (5)].pArrayStructItem); 
										;}
    break;

  case 56:

/* Line 1455 of yacc.c  */
//#line 399 "rcomp.yacc"
    {	if(verbose) 
												{		 	RCTypeArray Types; 
														 	MOFF;cout << "array_struct_item_start                    " << Types.GetName((yyvsp[(1) - (3)].Id)) << " " << (yyvsp[(2) - (3)].Value) << endl;MON;
												}
											(yyval.pArrayStructItem) = new ArrayStructItem((yyvsp[(1) - (3)].Id), (yyvsp[(2) - (3)].Value));
										;}
    break;

  case 57:

/* Line 1455 of yacc.c  */
//#line 407 "rcomp.yacc"
    {	(yyval.Id) = L_BYTE;;}
    break;

  case 58:

/* Line 1455 of yacc.c  */
//#line 408 "rcomp.yacc"
    {	(yyval.Id) = L_WORD;;}
    break;

  case 59:

/* Line 1455 of yacc.c  */
//#line 411 "rcomp.yacc"
    {	if(verbose) {	MOFF;cout << "struct_type_struct_item                    " << (yyvsp[(2) - (2)].Value) << endl;MON;}
											(yyval.pStructItem) = new StructTypeStructItem((yyvsp[(2) - (2)].Value));
										;}
    break;

  case 60:

/* Line 1455 of yacc.c  */
//#line 416 "rcomp.yacc"
    {	(yyval.pStructItem) = (yyvsp[(1) - (1)].pStructArrayStructItem);;}
    break;

  case 61:

/* Line 1455 of yacc.c  */
//#line 418 "rcomp.yacc"
    {	if(verbose) {	RCTypeArray Types; MOFF;cout << "struct_array_struct_item                   - Setting Size to " << Types.GetName((yyvsp[(2) - (3)].Id)) << endl;MON;}
											(yyvsp[(3) - (3)].pStructArrayStructItem)->iLenType = (yyvsp[(2) - (3)].Id); (yyval.pStructItem) = (yyvsp[(3) - (3)].pStructArrayStructItem);
										;}
    break;

  case 62:

/* Line 1455 of yacc.c  */
//#line 423 "rcomp.yacc"
    {	if(verbose) {	MOFF;cout << "struct_array_struct_item_base              " << (yyvsp[(2) - (4)].Value) << endl;MON;}
											(yyval.pStructArrayStructItem) = new StructArrayStructItem((yyvsp[(2) - (4)].Value));
										;}
    break;

  case 63:

/* Line 1455 of yacc.c  */
//#line 427 "rcomp.yacc"
    {	if(verbose) {	MOFF;cout << "struct_array_struct_item_base              " << (yyvsp[(2) - (5)].Value) << " " << (yyvsp[(4) - (5)].Value) << endl;MON;}
											(yyval.pStructArrayStructItem) = new StructArrayStructItem((yyvsp[(2) - (5)].Value), (yyvsp[(4) - (5)].Value));
										;}
    break;

  case 64:

/* Line 1455 of yacc.c  */
//#line 436 "rcomp.yacc"
    {	
	    pResourceHeader->AddDefault();
	    CurrentId+=CurrentIdStep;
	    if(verbose) { MOFF;cout << "Resource ID "<< CurrentId << endl << Divider << "\n" << * pResourceHeader << Divider << endl;MON;}
	    pResourceHeader->SetResourceId(*pResourceNameIds,CurrentId,FormatIdAsHex);
	    pG->Index.Add(pResourceHeader);
		
		CheckStructUsage();

		pUsedLabelsArray->Empty();

	    pResourceHeader = NULL;
	    ;}
    break;

  case 65:

/* Line 1455 of yacc.c  */
//#line 451 "rcomp.yacc"
    {;}
    break;

  case 66:

/* Line 1455 of yacc.c  */
//#line 453 "rcomp.yacc"
    {	
	    if(verbose) { MOFF;cout << "resource_statement_start                   LOCAL" << endl;MON;}
		    assert(pResourceHeader != NULL);
		    pResourceHeader->iLocal = 1;
	    ;}
    break;

  case 67:

/* Line 1455 of yacc.c  */
//#line 458 "rcomp.yacc"
    {;}
    break;

  case 68:

/* Line 1455 of yacc.c  */
//#line 461 "rcomp.yacc"
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
											pG->AllIdentifiers.Add(new String((yyvsp[(3) - (3)].Value))); // Add label to store
										;}
    break;

  case 69:

/* Line 1455 of yacc.c  */
//#line 474 "rcomp.yacc"
    {	if(verbose) {	MOFF;cout << "resource_statement_start_names             " << (yyvsp[(2) - (2)].Value) << " <Resource not named>" << endl;MON;}
											assert(pResourceHeader == NULL);
											pResourceHeader = new ResourceHeader;
											pCurrentRIA = & (pResourceHeader->iRIA);
											REGISTER_LINE;
											pCurrentRIA->FillFromStruct((yyvsp[(2) - (2)].Value));
										;}
    break;

  case 70:

/* Line 1455 of yacc.c  */
//#line 483 "rcomp.yacc"
    {	if(verbose) {	MOFF;cout << "resource_item_list" << endl;MON;};}
    break;

  case 71:

/* Line 1455 of yacc.c  */
//#line 484 "rcomp.yacc"
    {	if(verbose) {	MOFF;cout << "tagged resource_item_list" << endl;MON;};}
    break;

  case 72:

/* Line 1455 of yacc.c  */
//#line 485 "rcomp.yacc"
    {	yyerrok; yyclearin; ;}
    break;

  case 74:

/* Line 1455 of yacc.c  */
//#line 489 "rcomp.yacc"
    {	if(verbose) {	MOFF;cout << "resource_item                              " << (yyvsp[(1) - (3)].Value) << " " << (yyvsp[(3) - (3)].Value) << endl;MON;}
											REGISTER_LINE;/****************************************************************/
											pCurrentRIA->Set((yyvsp[(1) - (3)].Value), (yyvsp[(3) - (3)].Value));
										;}
    break;

  case 78:

/* Line 1455 of yacc.c  */
//#line 499 "rcomp.yacc"
    {	
	    if (verbose) 
		{ MOFF;cout << "resource_simple_array_item                 " << (yyvsp[(1) - (4)].Value) << endl;MON;} 
	    ;}
    break;

  case 79:

/* Line 1455 of yacc.c  */
//#line 504 "rcomp.yacc"
    {	
	    if (verbose) 
		{ MOFF;cout << "resource_simple_array_item                 " << (yyvsp[(1) - (5)].Value) << " with simple_initialiser_list" << endl;MON;}
	    REGISTER_LINE;
	    pCurrentRIA->Set((yyvsp[(1) - (5)].Value), * (yyvsp[(4) - (5)].pStringArray));
	    delete (yyvsp[(4) - (5)].pStringArray);
	    ;}
    break;

  case 80:

/* Line 1455 of yacc.c  */
//#line 532 "rcomp.yacc"
    {	if(verbose) {	MOFF;cout << "struct_resource_item" << endl;MON;}
											pCurrentRIA = pG->RIAStack.Pop();
										;}
    break;

  case 81:

/* Line 1455 of yacc.c  */
//#line 537 "rcomp.yacc"
    {	if(verbose) {	MOFF;cout << "struct_resource_item_start                 " << (yyvsp[(1) - (4)].Value) << " " << (yyvsp[(3) - (4)].Value) << endl;MON;}
											REGISTER_LINE;
											pCurrentRIA->Set((yyvsp[(1) - (4)].Value), (yyvsp[(3) - (4)].Value));
											String * thisLabel = new String((yyvsp[(1) - (4)].Value));
											pUsedLabelsArray->Add(thisLabel);
											// in here add the label to a temp store
											pG->RIAStack.Push(pCurrentRIA);
											pCurrentRIA = pCurrentRIA->Find((yyvsp[(1) - (4)].Value))->GetRIA();
										;}
    break;

  case 82:

/* Line 1455 of yacc.c  */
//#line 549 "rcomp.yacc"
    {	if(verbose) {	MOFF;cout << "struct_array_resource_item" << endl;MON;}
											pG->SRIStack.Pop();
										;}
    break;

  case 83:

/* Line 1455 of yacc.c  */
//#line 553 "rcomp.yacc"
    {	pG->SRIStack.Pop();;}
    break;

  case 84:

/* Line 1455 of yacc.c  */
//#line 556 "rcomp.yacc"
    {	if(verbose) {	MOFF;cout << "struct_array_resource_item_start           " << (yyvsp[(1) - (5)].Value) << " " << (yyvsp[(4) - (5)].Value) << endl;MON;}
											ResourceItem * p = pCurrentRIA->Find((yyvsp[(1) - (5)].Value));
											pG->SRIStack.Push(p);
											REGISTER_LINE;
											String * thisLabel = new String((yyvsp[(1) - (5)].Value));
											pUsedLabelsArray->Add(thisLabel);
											// in here add the label to a temp store
											p->Set((yyvsp[(4) - (5)].Value));
											pG->RIAStack.Push(pCurrentRIA);
											pCurrentRIA = p->GetRIA();
										;}
    break;

  case 88:

/* Line 1455 of yacc.c  */
//#line 574 "rcomp.yacc"
    {	if(verbose) {	MOFF;cout << "struct_array_resource_item_list_top        " << endl;MON;}
											pCurrentRIA = pG->RIAStack.Pop();
										;}
    break;

  case 91:

/* Line 1455 of yacc.c  */
//#line 584 "rcomp.yacc"
    {	if(verbose) {	MOFF;cout << "struct_array_resource_item_list_item       " << endl;MON;}
											pCurrentRIA = pG->RIAStack.Pop();
										;}
    break;

  case 92:

/* Line 1455 of yacc.c  */
//#line 589 "rcomp.yacc"
    {	if(verbose) {	MOFF;cout << "struct_array_resource_item_list_item_start " << (yyvsp[(1) - (2)].Value) << endl;MON;}
											ResourceItem * p = pG->SRIStack.Peek();
											REGISTER_LINE;
											p->Set((yyvsp[(1) - (2)].Value));
											pG->RIAStack.Push(pCurrentRIA);
											pCurrentRIA = p->GetRIA();
										;}
    break;

  case 94:

/* Line 1455 of yacc.c  */
//#line 605 "rcomp.yacc"
    { 
	    // convert literal to unsigned long value of 1st character
		SetCharacterLiteral((yyval.Value), (yyvsp[(1) - (1)].Value));
	    ;}
    break;

  case 97:

/* Line 1455 of yacc.c  */
//#line 614 "rcomp.yacc"
    {
	    if(verbose) 
		{	
		MOFF;cout << "simple_initialiser_list                    - single string " << (yyvsp[(1) - (1)].Value) << endl;MON;
		}
		
	    (yyval.pStringArray) = new StringArray;
	    (yyval.pStringArray)->Add(new String((yyvsp[(1) - (1)].Value)) );
	    ;}
    break;

  case 98:

/* Line 1455 of yacc.c  */
//#line 624 "rcomp.yacc"
    {	if(verbose) {	MOFF;cout << "simple_initialiser_list                    - part of list " << (yyvsp[(3) - (3)].Value) << endl;MON;}
											assert((yyvsp[(1) - (3)].pStringArray) != NULL);
											(yyvsp[(1) - (3)].pStringArray)->Add(new String((yyvsp[(3) - (3)].Value) ) );
											(yyval.pStringArray) = (yyvsp[(1) - (3)].pStringArray);
										;}
    break;

  case 99:

/* Line 1455 of yacc.c  */
//#line 632 "rcomp.yacc"
    {	String s(NumericValue::ltoa((yyvsp[(1) - (1)].NumInitialiser)) ); strcpy((yyval.Value), s.GetAssertedNonEmptyBuffer() ); ;}
    break;

  case 100:

/* Line 1455 of yacc.c  */
//#line 635 "rcomp.yacc"
    {	if(verbose) {	MOFF;cout << "Converting number " << (yyvsp[(1) - (1)].Value) << endl;MON;}
											REGISTER_LINE;
											NumericValue v((yyvsp[(1) - (1)].Value), L_LONG); (yyval.NumInitialiser) = (long)v.GetULong();
										;}
    break;

  case 101:

/* Line 1455 of yacc.c  */
//#line 639 "rcomp.yacc"
    {	(yyval.NumInitialiser) = (yyvsp[(1) - (3)].NumInitialiser) + (yyvsp[(3) - (3)].NumInitialiser);	;}
    break;

  case 102:

/* Line 1455 of yacc.c  */
//#line 640 "rcomp.yacc"
    {	(yyval.NumInitialiser) = (yyvsp[(1) - (3)].NumInitialiser) - (yyvsp[(3) - (3)].NumInitialiser);	;}
    break;

  case 103:

/* Line 1455 of yacc.c  */
//#line 641 "rcomp.yacc"
    {	(yyval.NumInitialiser) = (yyvsp[(1) - (3)].NumInitialiser) * (yyvsp[(3) - (3)].NumInitialiser);	;}
    break;

  case 104:

/* Line 1455 of yacc.c  */
//#line 642 "rcomp.yacc"
    {	(yyval.NumInitialiser) = (yyvsp[(1) - (3)].NumInitialiser) / (yyvsp[(3) - (3)].NumInitialiser);	;}
    break;

  case 105:

/* Line 1455 of yacc.c  */
//#line 643 "rcomp.yacc"
    {	(yyval.NumInitialiser) = (yyvsp[(1) - (3)].NumInitialiser) | (yyvsp[(3) - (3)].NumInitialiser);	;}
    break;

  case 106:

/* Line 1455 of yacc.c  */
//#line 644 "rcomp.yacc"
    {	if (!NumericValue::CheckSigned((yyvsp[(2) - (2)].NumInitialiser),L_LONG))
																	{
																	REGISTER_LINE;
																	ErrorHandler::OutputErrorLine("Signed value too low");
																	exit(1);
																	}
																	(yyval.NumInitialiser) = - (yyvsp[(2) - (2)].NumInitialiser);		
																;}
    break;

  case 107:

/* Line 1455 of yacc.c  */
//#line 652 "rcomp.yacc"
    {	(yyval.NumInitialiser) = (yyvsp[(2) - (3)].NumInitialiser);		;}
    break;

  case 109:

/* Line 1455 of yacc.c  */
//#line 656 "rcomp.yacc"
    {
	    if (strlen((yyval.Value))+strlen((yyvsp[(2) - (2)].Value)) > sizeof((yyval.Value))-1)
		{
		REGISTER_LINE;
		ErrorHandler::OutputErrorLine("String expression is too long");
		exit(1);
		}
	    strcat((yyval.Value), (yyvsp[(2) - (2)].Value));
	    ;}
    break;

  case 112:

/* Line 1455 of yacc.c  */
//#line 670 "rcomp.yacc"
    {
		const char * fileName = (*ErrorHandler::GetFileName()).GetBuffer();
		int lineNumber = ErrorHandler::GetLineNumber();
		QualifiedString * thisLabel = new QualifiedString((yyvsp[(1) - (1)].Value), new String(fileName), lineNumber);
		// store the label in the UsedIdentifiers array for checking
		// whether label was declared
		pG->UsedIdentifiers.Add(thisLabel);

	    if (pG->EnumValues.IsStored((yyvsp[(1) - (1)].Value)))
			{
			sprintf((yyval.Value), "%d", (int)(pG->EnumValues.FindId((yyvsp[(1) - (1)].Value))));
			}
		else if (pG->RlsNameIndex.count((yyvsp[(1) - (1)].Value))) // if rls item has already been defined
			{
			// Found a reference to an rls_string.
			RlsValue &rv = pG->RlsValues[pG->RlsNameIndex[(yyvsp[(1) - (1)].Value)]];
			++rv.iCitationCount; // iCitationCount counts the number of times this rls value has been referneced
			// Warn for multiple uses if 'multi' keyword not used.
			if (1 < rv.iCitationCount && rv.iCardinality == ERlsCardinalitySingle)
				{
				Message * message = pG->Messages.GetEntry(LT_001);
				String fileLine = *(rv.iFileName);
				if(message->GetActivated())
				{
					pGL->AddWarningToStore(fileLine, rv.iLineNumber, message->GetMessageOutput());
				}
				REGISTER_LINE;
				if (!pG->WarningMultiExplained)
					{
					Message * message = pG->Messages.GetEntry(LT_002);
					fileLine = String(*(pFileLineHandler->GetCurrentFile()));
					if(message->GetActivated())
						{
						pGL->AddWarningToStore(fileLine, pFileLineHandler->GetErrorLine(* pCurrentLineNumber), message->GetMessageOutput());
						pG->WarningMultiExplained = true;
						}
					}
				}
			switch (rv.iType)
				{
				// Strings and numbers are just copied to the next layer up.
			case ERlsString:
			case ERlsString8:
			case ERlsByte:
			case ERlsWord:
			case ERlsLong:
			case ERlsDouble:
				strcpy((yyval.Value), rv.iValue.GetBuffer());
				break;
				// Anything else is a character: this is converted to a number.
			case ERlsStringChar:
			case ERlsByteChar:
			case ERlsWordChar:
			case ERlsLongChar:
				SetCharacterLiteral((yyval.Value), rv.iValue);
				break;
			default:
				Message * message = pG->Messages.GetEntry(LT_031);
				if(message->GetActivated())
					{
					ErrorHandler::OutputErrorLine(message->GetMessageOutput());
					exit(1);
					}
				break;
				}
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
	    ;}
    break;

  case 113:

/* Line 1455 of yacc.c  */
//#line 749 "rcomp.yacc"
    {	
	    REGISTER_LINE;
	    if((yyvsp[(2) - (3)].NumInitialiser) < 0 || ((yyvsp[(2) - (3)].NumInitialiser) > 255 && TargetCharacterSet != String::Unicode))
		{
		    ErrorHandler::OutputErrorLine("Character code must be a number in the range 0 to 255.");
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
	    ;}
    break;

  case 114:

/* Line 1455 of yacc.c  */
//#line 795 "rcomp.yacc"
    {
	    REGISTER_LINE;
	    SetIdFromName((yyvsp[(2) - (2)].Value));
	    ;}
    break;

  case 115:

/* Line 1455 of yacc.c  */
//#line 800 "rcomp.yacc"
    {
	    REGISTER_LINE;
	    SetIdFromName((yyvsp[(2) - (2)].Value));
	    ;}
    break;

  case 116:

/* Line 1455 of yacc.c  */
//#line 812 "rcomp.yacc"
    {
		REGISTER_LINE;
		if ((yyvsp[(2) - (2)].NumInitialiser) == 0)
			{ ErrorHandler::OutputErrorLine("UID2 must be non-zero"); exit(1); }
		if (Uid2 != 0)
			{ ErrorHandler::OutputErrorLine("Warning: overwriting previous UID2 value"); }
		Uid2=(yyvsp[(2) - (2)].NumInitialiser);
		if(verbose) 
			{ MOFF;cout << "uidX_statement  UID2                       " << Uid2 << endl;MON;}
		;}
    break;

  case 117:

/* Line 1455 of yacc.c  */
//#line 823 "rcomp.yacc"
    {
		REGISTER_LINE;
		if ((yyvsp[(2) - (2)].NumInitialiser) == 0)
			{ ErrorHandler::OutputErrorLine("UID3 must be non-zero"); exit(1); }
		if (Uid3 != 0)
			{ ErrorHandler::OutputErrorLine("Warning: overwriting previous UID3 value"); }
		Uid3=(yyvsp[(2) - (2)].NumInitialiser);
		if(verbose) 
			{ MOFF;cout << "uidX_statement  UID3                       " << Uid3 << endl;MON;}
		;}
    break;

  case 118:

/* Line 1455 of yacc.c  */
//#line 844 "rcomp.yacc"
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
										;}
    break;

  case 119:

/* Line 1455 of yacc.c  */
//#line 868 "rcomp.yacc"
    {	if(verbose) {	RCTypeArray Types;
															MOFF;cout << "offset_statement                           " << (yyvsp[(2) - (2)].Value) << endl;MON; }
											REGISTER_LINE;
										 	CurrentId=((long) NumericValue((yyvsp[(2) - (2)].Value), L_LONG).GetULong() );
										;}
    break;

  case 120:

/* Line 1455 of yacc.c  */
//#line 879 "rcomp.yacc"
    {	if(verbose) {	MOFF;cout << "system_statement" << endl;MON;}
											CurrentIdStep=-1;
										;}
    break;

  case 123:

/* Line 1455 of yacc.c  */
//#line 893 "rcomp.yacc"
    {	
	    if(verbose) 
		{ MOFF;cout << "enum_statement" << endl;MON;} 
	    CurrentEnumName = (yyvsp[(2) - (3)].Value);
	    CurrentEnumValue=0;
	    ;}
    break;

  case 124:

/* Line 1455 of yacc.c  */
//#line 900 "rcomp.yacc"
    {	
	    if(verbose) 
		{ MOFF;cout << "enum_statement" << endl;MON;} 
	    CurrentEnumName = "";
	    CurrentEnumValue=0;
	    ;}
    break;

  case 125:

/* Line 1455 of yacc.c  */
//#line 910 "rcomp.yacc"
    {	
		pG->EnumValues.Add((yyvsp[(1) - (1)].Value), CurrentEnumValue++);
		pG->AllIdentifiers.Add(new String((yyvsp[(1) - (1)].Value))); // Add label to store
		;}
    break;

  case 126:

/* Line 1455 of yacc.c  */
//#line 915 "rcomp.yacc"
    {	
	    CurrentEnumValue = atol((yyvsp[(3) - (3)].Value));
	    pG->EnumValues.Add((yyvsp[(1) - (3)].Value), CurrentEnumValue);
	    CurrentEnumValue++;			// Increment so that next field has value ($3+1)
		pG->AllIdentifiers.Add(new String((yyvsp[(1) - (3)].Value))); // Add label to store
	    ;}
    break;

  case 129:

/* Line 1455 of yacc.c  */
//#line 934 "rcomp.yacc"
    {
		pG->RlsNameIndex[(yyvsp[(3) - (4)].Value)] = pG->RlsValues.size();
		pG->RlsValues.push_back(RlsValue(ErrorHandler::GetFileName(),
			ErrorHandler::GetLineNumber(), (yyvsp[(4) - (4)].Value), (yyvsp[(1) - (4)].RlsType),
			(yyvsp[(2) - (4)].RlsQualifiers).iCardinality, (yyvsp[(2) - (4)].RlsQualifiers).iMaxLength));
		if((yyvsp[(2) - (4)].RlsQualifiers).iMaxLength
			< String((yyvsp[(4) - (4)].Value)).ExportLength(TargetCharacterSet,SourceCharacterSet))
			{
			Message * message = pG->Messages.GetEntry(LT_032);
			if(message->GetActivated())
				{
				ErrorHandler::OutputErrorLine(message->GetMessageOutput());
				exit(1);
				}
			}
		;}
    break;

  case 130:

/* Line 1455 of yacc.c  */
//#line 951 "rcomp.yacc"
    {
		Message * message = pG->Messages.GetEntry(LT_033);
		String fileName = *(pFileLineHandler->GetCurrentFile());
		int lineNumber = pFileLineHandler->GetErrorLine(* pCurrentLineNumber);
		if(message->GetActivated())
			{
			pGL->AddWarningToStore(fileName, lineNumber, message->GetMessageOutput());
			}
		//...
		/* Produce a warning "rls_string used for character constant: use rls_long, rls_word or rls_byte" */
		pG->RlsNameIndex[(yyvsp[(3) - (4)].Value)] = pG->RlsValues.size();
		pG->RlsValues.push_back(RlsValue(ErrorHandler::GetFileName(),
			ErrorHandler::GetLineNumber(), (yyvsp[(4) - (4)].Value), ERlsStringChar,
			(yyvsp[(2) - (4)].RlsQualifiers).iCardinality));
		;}
    break;

  case 131:

/* Line 1455 of yacc.c  */
//#line 967 "rcomp.yacc"
    {
		pG->RlsNameIndex[(yyvsp[(3) - (4)].Value)] = pG->RlsValues.size();
		pG->RlsValues.push_back(RlsValue(ErrorHandler::GetFileName(),
			ErrorHandler::GetLineNumber(), (yyvsp[(4) - (4)].Value), (yyvsp[(1) - (4)].RlsType),
			(yyvsp[(2) - (4)].RlsQualifiers).iCardinality));
		;}
    break;

  case 132:

/* Line 1455 of yacc.c  */
//#line 974 "rcomp.yacc"
    {
		pG->RlsNameIndex[(yyvsp[(3) - (4)].Value)] = pG->RlsValues.size();
		pG->RlsValues.push_back(RlsValue(ErrorHandler::GetFileName(),
			ErrorHandler::GetLineNumber(), (yyvsp[(4) - (4)].Value), (yyvsp[(1) - (4)].RlsType),
			(yyvsp[(2) - (4)].RlsQualifiers).iCardinality));
		;}
    break;

  case 133:

/* Line 1455 of yacc.c  */
//#line 981 "rcomp.yacc"
    {
		TRlsType rlsCharType = (yyvsp[(1) - (4)].RlsType) == ERlsByte? ERlsByteChar
			: ( (yyvsp[(1) - (4)].RlsType) ==  ERlsWord? ERlsWordChar : ERlsLongChar );
		pG->RlsNameIndex[(yyvsp[(3) - (4)].Value)] = pG->RlsValues.size();
		pG->RlsValues.push_back(RlsValue(ErrorHandler::GetFileName(),
			ErrorHandler::GetLineNumber(), (yyvsp[(4) - (4)].Value), rlsCharType,
			(yyvsp[(2) - (4)].RlsQualifiers).iCardinality));
		;}
    break;

  case 134:

/* Line 1455 of yacc.c  */
//#line 992 "rcomp.yacc"
    {
		// Register line even if no warning here so that
		// the rls_ item knows which line the label was on.
		// Without this, the line registered would be the
		// line following the declaration.
		REGISTER_LINE;
		strcpy((yyval.Value), (yyvsp[(1) - (1)].Value));

		if (pG->RlsNameIndex.count((yyvsp[(1) - (1)].Value)) != 0)
			{
			Message * message = pG->Messages.GetEntry(LT_003);
			if(message->GetActivated())
				{
				ErrorHandler::OutputErrorLine(message->GetMessageOutput());
				}
			}
		pG->AllIdentifiers.Add(new String((yyvsp[(1) - (1)].Value))); // Add label to store
		;}
    break;

  case 135:

/* Line 1455 of yacc.c  */
//#line 1013 "rcomp.yacc"
    {
		NumericValue v((yyvsp[(2) - (4)].Value), L_LONG);
		(yyval.RlsQualifiers).iMaxLength = v.GetULong();
		(yyval.RlsQualifiers).iCardinality = (yyvsp[(4) - (4)].RlsQualifiers).iCardinality;
		;}
    break;

  case 136:

/* Line 1455 of yacc.c  */
//#line 1019 "rcomp.yacc"
    { (yyval.RlsQualifiers) = (yyvsp[(1) - (1)].RlsQualifiers); ;}
    break;

  case 137:

/* Line 1455 of yacc.c  */
//#line 1024 "rcomp.yacc"
    {
		(yyval.RlsQualifiers).iMaxLength = 0xFFFFFFF;
		(yyval.RlsQualifiers).iCardinality = ERlsCardinalityMultiple;
		;}
    break;

  case 138:

/* Line 1455 of yacc.c  */
//#line 1029 "rcomp.yacc"
    {
		(yyval.RlsQualifiers).iMaxLength = 0xFFFFFFF;
		(yyval.RlsQualifiers).iCardinality = ERlsCardinalitySingle;
		;}
    break;

  case 139:

/* Line 1455 of yacc.c  */
//#line 1037 "rcomp.yacc"
    { (yyval.RlsType) = ERlsString; ;}
    break;

  case 140:

/* Line 1455 of yacc.c  */
//#line 1039 "rcomp.yacc"
    { (yyval.RlsType) = ERlsString8; ;}
    break;

  case 141:

/* Line 1455 of yacc.c  */
//#line 1044 "rcomp.yacc"
    { (yyval.RlsType) = ERlsByte; ;}
    break;

  case 142:

/* Line 1455 of yacc.c  */
//#line 1046 "rcomp.yacc"
    { (yyval.RlsType) = ERlsWord; ;}
    break;

  case 143:

/* Line 1455 of yacc.c  */
//#line 1048 "rcomp.yacc"
    { (yyval.RlsType) = ERlsLong; ;}
    break;

  case 144:

/* Line 1455 of yacc.c  */
//#line 1053 "rcomp.yacc"
    { (yyval.RlsType) = ERlsDouble; ;}
    break;

  case 147:

/* Line 1455 of yacc.c  */
//#line 1065 "rcomp.yacc"
    {ErrorHandler::Register(pFileLineHandler->GetCurrentFile(), pFileLineHandler->GetErrorLine(*pCurrentLineNumber)); ;}
    break;

  case 150:

/* Line 1455 of yacc.c  */
//#line 1075 "rcomp.yacc"
    { pGL->StoreComment((yyvsp[(1) - (1)].Value));	;}
    break;

  case 151:

/* Line 1455 of yacc.c  */
//#line 1076 "rcomp.yacc"
    { pGL->StoreComment((yyvsp[(1) - (1)].Value));	;}
    break;

  case 152:

/* Line 1455 of yacc.c  */
//#line 1077 "rcomp.yacc"
    { pGL->StoreComment((yyvsp[(1) - (1)].Value));	;}
    break;



/* Line 1455 of yacc.c  */
//#line 3037 "rcomp.tab.cacc"
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
//#line 1080 "rcomp.yacc"


// Function section
// ================

void asUTF8(char* aUtf8, int aUnicode)
	{
	if ( aUnicode > 0xffff )
		{
		if ( aUnicode > 0x10ffff )
		{
		ErrorHandler::OutputErrorLine("Surrogate character code must be a number in the range 0x10000 to 0x10ffff");
		exit(1);		
		}
		
		UTF16 high = (UTF16)(0xD7C0 + (aUnicode >> 10));			// high surrogate
		UTF16 low = (UTF16)(0xDC00 | (aUnicode & 0x3FF));			// low surrogate
	
		*aUtf8++ =(char)(0xe0|(high>>12));
		*aUtf8++ =(char)(0x80|((high>>6)&0x3f));
		*aUtf8++ =(char)(0x80|(high&0x3f));
		*aUtf8++ =(char)(0xe0|(low>>12));
		*aUtf8++ =(char)(0x80|((low>>6)&0x3f));
		*aUtf8   =(char)(0x80|(low&0x3f));
		}
	else if ((aUnicode & 0xff80) == 0x0000)
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

static void RlsUnusedWarnings()
	{
	TNameIndex::iterator end = pG->RlsNameIndex.end();
	for (TNameIndex::iterator i = pG->RlsNameIndex.begin(); i != end; ++i)
		{
		int index = i->second;
		RlsValue& v = pG->RlsValues[index];
		if (v.iCitationCount == 0)
			{
			Message * message = pG->Messages.GetEntry(LT_004);
			String fileLine = *(v.iFileName);
			if(message->GetActivated())
				{
				pGL->AddWarningToStore(fileLine, v.iLineNumber, message->GetMessageOutput());
				}
			}
		}
	}

int ParseSourceFile(FILE* aFile, unsigned short aYYDebug)
	{
	// Set up various global pointers which refer to the pG structure
	pSHA = & (pG->SHA);
	pFileLineHandler = & (pG->FileLineHandler);
	pResourceNameIds = & (pG->ResourceNameIds);

	pScan = new rcscan(pG->FileLineHandler, aFile);

	yydebug = aYYDebug;
	pCurrentLineNumber = &yylineno;
	int ReturnValue = yyparse();

	RlsUnusedWarnings();

	int bScanErrorFound = pScan->ErrorWasFound();

	delete pScan;
	pScan = NULL;

	if(ReturnValue != 0)
		return ReturnValue;
	
	if(bScanErrorFound)
		return 1;
	
	return 0;	// successful parse - parse tree now in the pG data structure
	}


void CheckStructUsage()
	{
	ResourceItemArrayIterator	nextRI( *pCurrentRIA);
	ResourceItem * pRI;
	while ( ( pRI = nextRI() ) != NULL)
		{
		int resourceItemType = pRI->GetResourceItemType();
		String resourceItemLabel = pRI->GetLabel();
		if( (resourceItemType == EStructTypeResourceItem) || (resourceItemType == EStructArrayResourceItem) )
			{
			StringArrayIterator nextLabel( *pUsedLabelsArray);
			String * pLabel;
			bool flag = false;
			while ( ( ( pLabel = nextLabel() ) != NULL ) && (! flag) )
				{
				StringLess stringCompare;
				if( !stringCompare(resourceItemLabel,*pLabel) && !stringCompare(*pLabel,resourceItemLabel) )
					{
					flag = true;
					}
				}
#if 0	//disable useless warning
			if(! flag)
				{
				if(resourceItemType == EStructTypeResourceItem)
					{
					Message * message = pG->Messages.GetEntry(LT_046);
					if(message->GetActivated())
						{
						String comment = message->GetMessageOutput();
						comment += "'";
						comment += resourceItemLabel;
						comment += "'";
						ErrorHandler::OutputErrorLine(comment);
						}
					}
				else
					{
					Message * message = pG->Messages.GetEntry(LT_047);
					if(message->GetActivated())
						{
						String comment = message->GetMessageOutput();
						comment += "'";
						comment += resourceItemLabel;
						comment += "'";
						ErrorHandler::OutputErrorLine(comment);
						}
					}
				}
#endif	// 0
			}
		}
	}

int yywrap()
{
  return 1;
}

/* Called by yyparse on error */
#include <stdarg.h>
void yyerror (const char *s, ...)
{
  va_list list;
  va_start(list, s);
  pScan->yyerror(const_cast<char*>(s), list);
  va_end(list);
}



