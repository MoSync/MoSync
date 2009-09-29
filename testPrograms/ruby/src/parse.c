/* A Bison parser, made from parse.y
   by GNU bison 1.35.  */

#define YYBISON 1  /* Identify Bison output.  */

# define	kCLASS	257
# define	kMODULE	258
# define	kDEF	259
# define	kUNDEF	260
# define	kBEGIN	261
# define	kRESCUE	262
# define	kENSURE	263
# define	kEND	264
# define	kIF	265
# define	kUNLESS	266
# define	kTHEN	267
# define	kELSIF	268
# define	kELSE	269
# define	kCASE	270
# define	kWHEN	271
# define	kWHILE	272
# define	kUNTIL	273
# define	kFOR	274
# define	kBREAK	275
# define	kNEXT	276
# define	kREDO	277
# define	kRETRY	278
# define	kIN	279
# define	kDO	280
# define	kDO_COND	281
# define	kDO_BLOCK	282
# define	kRETURN	283
# define	kYIELD	284
# define	kSUPER	285
# define	kSELF	286
# define	kNIL	287
# define	kTRUE	288
# define	kFALSE	289
# define	kAND	290
# define	kOR	291
# define	kNOT	292
# define	kIF_MOD	293
# define	kUNLESS_MOD	294
# define	kWHILE_MOD	295
# define	kUNTIL_MOD	296
# define	kRESCUE_MOD	297
# define	kALIAS	298
# define	kDEFINED	299
# define	klBEGIN	300
# define	klEND	301
# define	k__LINE__	302
# define	k__FILE__	303
# define	tIDENTIFIER	304
# define	tFID	305
# define	tGVAR	306
# define	tIVAR	307
# define	tCONSTANT	308
# define	tCVAR	309
# define	tINTEGER	310
# define	tFLOAT	311
# define	tSTRING_CONTENT	312
# define	tNTH_REF	313
# define	tBACK_REF	314
# define	tREGEXP_END	315
# define	tUPLUS	316
# define	tUMINUS	317
# define	tPOW	318
# define	tCMP	319
# define	tEQ	320
# define	tEQQ	321
# define	tNEQ	322
# define	tGEQ	323
# define	tLEQ	324
# define	tANDOP	325
# define	tOROP	326
# define	tMATCH	327
# define	tNMATCH	328
# define	tDOT2	329
# define	tDOT3	330
# define	tAREF	331
# define	tASET	332
# define	tLSHFT	333
# define	tRSHFT	334
# define	tCOLON2	335
# define	tCOLON3	336
# define	tOP_ASGN	337
# define	tASSOC	338
# define	tLPAREN	339
# define	tLPAREN_ARG	340
# define	tRPAREN	341
# define	tLBRACK	342
# define	tLBRACE	343
# define	tLBRACE_ARG	344
# define	tSTAR	345
# define	tAMPER	346
# define	tSYMBEG	347
# define	tSTRING_BEG	348
# define	tXSTRING_BEG	349
# define	tREGEXP_BEG	350
# define	tWORDS_BEG	351
# define	tQWORDS_BEG	352
# define	tSTRING_DBEG	353
# define	tSTRING_DVAR	354
# define	tSTRING_END	355
# define	tLOWEST	356
# define	tUMINUS_NUM	357
# define	tLAST_TOKEN	358

#line 13 "parse.y"


//#define YYDEBUG 1
#define YYERROR_VERBOSE 1
#ifndef YYSTACK_USE_ALLOCA
#define YYSTACK_USE_ALLOCA 0
#endif

#include "ruby.h"
#include "env.h"
#include "intern.h"
#include "node.h"
#include "st.h"
#include <stdio.h>
#include <errno.h>
#include <ctype.h>

#define YYMALLOC	rb_parser_malloc
#define YYREALLOC	rb_parser_realloc
#define YYCALLOC	rb_parser_calloc
#define YYFREE 	rb_parser_free
#define malloc	YYMALLOC
#define realloc	YYREALLOC
#define calloc	YYCALLOC
#define free	YYFREE
static void *rb_parser_malloc _((size_t));
static void *rb_parser_realloc _((void *, size_t));
static void *rb_parser_calloc _((size_t, size_t));
static void rb_parser_free _((void *));

#define yyparse ruby_yyparse
#define yylex ruby_yylex
#define yyerror ruby_yyerror
#define yylval ruby_yylval
#define yychar ruby_yychar
#define yydebug ruby_yydebug

#define ID_SCOPE_SHIFT 3
#define ID_SCOPE_MASK 0x07
#define ID_LOCAL    0x01
#define ID_INSTANCE 0x02
#define ID_GLOBAL   0x03
#define ID_ATTRSET  0x04
#define ID_CONST    0x05
#define ID_CLASS    0x06
#define ID_JUNK     0x07
#define ID_INTERNAL ID_JUNK

#define is_notop_id(id) ((id)>tLAST_TOKEN)
#define is_local_id(id) (is_notop_id(id)&&((id)&ID_SCOPE_MASK)==ID_LOCAL)
#define is_global_id(id) (is_notop_id(id)&&((id)&ID_SCOPE_MASK)==ID_GLOBAL)
#define is_instance_id(id) (is_notop_id(id)&&((id)&ID_SCOPE_MASK)==ID_INSTANCE)
#define is_attrset_id(id) (is_notop_id(id)&&((id)&ID_SCOPE_MASK)==ID_ATTRSET)
#define is_const_id(id) (is_notop_id(id)&&((id)&ID_SCOPE_MASK)==ID_CONST)
#define is_class_id(id) (is_notop_id(id)&&((id)&ID_SCOPE_MASK)==ID_CLASS)
#define is_junk_id(id) (is_notop_id(id)&&((id)&ID_SCOPE_MASK)==ID_JUNK)

#define is_asgn_or_id(id) ((is_notop_id(id)) && \
	(((id)&ID_SCOPE_MASK) == ID_GLOBAL || \
	 ((id)&ID_SCOPE_MASK) == ID_INSTANCE || \
	 ((id)&ID_SCOPE_MASK) == ID_CLASS))

NODE *ruby_eval_tree_begin = 0;
NODE *ruby_eval_tree = 0;

char *ruby_sourcefile;		/* current source file */
int   ruby_sourceline;		/* current line no. */

static int yylex();
static int yyerror();

static enum lex_state {
    EXPR_BEG,			/* ignore newline, +/- is a sign. */
    EXPR_END,			/* newline significant, +/- is a operator. */
    EXPR_ARG,			/* newline significant, +/- is a operator. */
    EXPR_CMDARG,		/* newline significant, +/- is a operator. */
    EXPR_ENDARG,		/* newline significant, +/- is a operator. */
    EXPR_MID,			/* newline significant, +/- is a operator. */
    EXPR_FNAME,			/* ignore newline, no reserved words. */
    EXPR_DOT,			/* right after `.' or `::', no reserved words. */
    EXPR_CLASS,			/* immediate after `class', no here document. */
} lex_state;
static NODE *lex_strterm;

#ifdef HAVE_LONG_LONG
typedef unsigned LONG_LONG stack_type;
#else
typedef unsigned long stack_type;
#endif

#define BITSTACK_PUSH(stack, n)	(stack = (stack<<1)|((n)&1))
#define BITSTACK_POP(stack)	(stack >>= 1)
#define BITSTACK_LEXPOP(stack)	(stack = (stack >> 1) | (stack & 1))
#define BITSTACK_SET_P(stack)	(stack&1)

static stack_type cond_stack = 0;
#define COND_PUSH(n)	BITSTACK_PUSH(cond_stack, n)
#define COND_POP()	BITSTACK_POP(cond_stack)
#define COND_LEXPOP()	BITSTACK_LEXPOP(cond_stack)
#define COND_P()	BITSTACK_SET_P(cond_stack)

static stack_type cmdarg_stack = 0;
#define CMDARG_PUSH(n)	BITSTACK_PUSH(cmdarg_stack, n)
#define CMDARG_POP()	BITSTACK_POP(cmdarg_stack)
#define CMDARG_LEXPOP()	BITSTACK_LEXPOP(cmdarg_stack)
#define CMDARG_P()	BITSTACK_SET_P(cmdarg_stack)

static int class_nest = 0;
static int in_single = 0;
static int in_def = 0;
static int compile_for_eval = 0;
static ID cur_mid = 0;
static int command_start = Qtrue;

static NODE *cond();
static NODE *logop();
static int cond_negative();

static NODE *newline_node();
static void fixpos();

static int value_expr0();
static void void_expr0();
static void void_stmts();
static NODE *remove_begin();
#define value_expr(node) value_expr0((node) = remove_begin(node))
#define void_expr(node) void_expr0((node) = remove_begin(node))

static NODE *block_append();
static NODE *list_append();
static NODE *list_concat();
static NODE *arg_concat();
static NODE *arg_prepend();
static NODE *literal_concat();
static NODE *new_evstr();
static NODE *evstr2dstr();
static NODE *call_op();
static int in_defined = 0;

static NODE *negate_lit();
static NODE *ret_args();
static NODE *arg_blk_pass();
static NODE *new_call();
static NODE *new_fcall();
static NODE *new_super();
static NODE *new_yield();

static NODE *gettable();
static NODE *assignable();
static NODE *aryset();
static NODE *attrset();
static void rb_backref_error();
static NODE *node_assign();

static NODE *match_gen();
static void local_push();
static void local_pop();
static int  local_append();
static int  local_cnt();
static int  local_id();
static ID  *local_tbl();
static ID   internal_id();

static struct RVarmap *dyna_push();
static void dyna_pop();
static int dyna_in_block();
static NODE *dyna_init();

static void top_local_init();
static void top_local_setup();

#define RE_OPTION_ONCE 0x80

#define NODE_STRTERM NODE_ZARRAY	/* nothing to gc */
#define NODE_HEREDOC NODE_ARRAY 	/* 1, 3 to gc */
#define SIGN_EXTEND(x,n) (((1<<(n)-1)^((x)&~(~0<<(n))))-(1<<(n)-1))
#define nd_func u1.id
#if SIZEOF_SHORT == 2
#define nd_term(node) ((signed short)(node)->u2.id)
#else
#define nd_term(node) SIGN_EXTEND((node)->u2.id, CHAR_BIT*2)
#endif
#define nd_paren(node) (char)((node)->u2.id >> CHAR_BIT*2)
#define nd_nest u3.id

/* Older versions of Yacc set YYMAXDEPTH to a very low value by default (150,
   for instance).  This is too low for Ruby to parse some files, such as
   date/format.rb, therefore bump the value up to at least Bison's default. */
#ifdef OLD_YACC
#ifndef YYMAXDEPTH
#define YYMAXDEPTH 10000
#endif
#endif


#line 209 "parse.y"
#ifndef YYSTYPE
typedef union {
    NODE *node;
    ID id;
    int num;
    struct RVarmap *vars;
} yystype;
# define YYSTYPE yystype
# define YYSTYPE_IS_TRIVIAL 1
#endif
#ifndef YYDEBUG
# define YYDEBUG 0
#endif



#define	YYFINAL		895
#define	YYFLAG		-32768
#define	YYNTBASE	132

/* YYTRANSLATE(YYLEX) -- Bison token number corresponding to YYLEX. */
#define YYTRANSLATE(x) ((unsigned)(x) <= 358 ? yytranslate[x] : 275)

/* YYTRANSLATE[YYLEX] -- Bison token number corresponding to YYLEX. */
static const short yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     130,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,   129,   117,     2,     2,     2,   115,   110,     2,
     128,   125,   113,   111,   126,   112,   124,   114,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,   105,   131,
     107,   103,   106,   104,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,   122,     2,   123,   109,     2,   127,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,   120,   108,   121,   118,     2,     2,     2,
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
       2,     2,     2,     2,     2,     2,     1,     3,     4,     5,
       6,     7,     8,     9,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    21,    22,    23,    24,    25,
      26,    27,    28,    29,    30,    31,    32,    33,    34,    35,
      36,    37,    38,    39,    40,    41,    42,    43,    44,    45,
      46,    47,    48,    49,    50,    51,    52,    53,    54,    55,
      56,    57,    58,    59,    60,    61,    62,    63,    64,    65,
      66,    67,    68,    69,    70,    71,    72,    73,    74,    75,
      76,    77,    78,    79,    80,    81,    82,    83,    84,    85,
      86,    87,    88,    89,    90,    91,    92,    93,    94,    95,
      96,    97,    98,    99,   100,   101,   102,   116,   119
};

#if YYDEBUG
static const short yyprhs[] =
{
       0,     0,     1,     4,     9,    12,    14,    16,    20,    23,
      24,    29,    33,    37,    41,    44,    48,    52,    56,    60,
      64,    65,    71,    76,    80,    84,    88,    95,   101,   107,
     113,   117,   121,   125,   129,   131,   133,   137,   141,   144,
     147,   149,   151,   153,   155,   158,   161,   164,   166,   171,
     176,   177,   178,   185,   188,   192,   197,   203,   208,   214,
     217,   220,   222,   226,   228,   232,   234,   237,   241,   244,
     247,   249,   251,   255,   258,   262,   264,   269,   273,   277,
     281,   285,   288,   290,   292,   297,   301,   305,   309,   313,
     316,   318,   320,   322,   325,   327,   331,   333,   335,   337,
     339,   341,   343,   345,   347,   349,   351,   352,   357,   359,
     361,   363,   365,   367,   369,   371,   373,   375,   377,   379,
     381,   383,   385,   387,   389,   391,   393,   395,   397,   399,
     401,   403,   405,   407,   409,   411,   413,   415,   417,   419,
     421,   423,   425,   427,   429,   431,   433,   435,   437,   439,
     441,   443,   445,   447,   449,   451,   453,   455,   457,   459,
     461,   463,   465,   467,   469,   471,   473,   475,   477,   479,
     481,   483,   485,   487,   489,   493,   499,   503,   510,   516,
     522,   528,   534,   539,   543,   547,   551,   555,   559,   563,
     567,   571,   575,   580,   585,   588,   591,   595,   599,   603,
     607,   611,   615,   619,   623,   627,   631,   635,   639,   643,
     646,   649,   653,   657,   661,   665,   666,   671,   677,   679,
     681,   683,   686,   689,   695,   698,   702,   706,   711,   716,
     723,   725,   727,   729,   732,   738,   741,   747,   752,   760,
     764,   766,   771,   775,   781,   789,   792,   798,   803,   810,
     818,   828,   832,   834,   835,   838,   840,   841,   845,   846,
     851,   854,   857,   859,   861,   865,   869,   874,   877,   879,
     881,   883,   885,   887,   889,   891,   893,   895,   896,   901,
     902,   908,   912,   916,   919,   924,   928,   932,   934,   939,
     943,   945,   946,   953,   956,   958,   961,   968,   975,   976,
     977,   985,   986,   987,   995,  1001,  1006,  1012,  1013,  1014,
    1024,  1025,  1032,  1033,  1034,  1043,  1044,  1050,  1051,  1058,
    1059,  1060,  1070,  1072,  1074,  1076,  1078,  1080,  1082,  1084,
    1086,  1089,  1091,  1093,  1095,  1097,  1103,  1105,  1108,  1110,
    1112,  1114,  1117,  1119,  1123,  1124,  1125,  1132,  1135,  1140,
    1145,  1148,  1153,  1158,  1162,  1165,  1167,  1168,  1169,  1176,
    1177,  1178,  1185,  1191,  1193,  1198,  1201,  1203,  1205,  1212,
    1214,  1216,  1218,  1220,  1223,  1225,  1228,  1230,  1232,  1234,
    1236,  1238,  1240,  1243,  1247,  1251,  1255,  1259,  1263,  1264,
    1268,  1270,  1273,  1277,  1281,  1282,  1286,  1287,  1290,  1291,
    1294,  1296,  1297,  1301,  1302,  1307,  1309,  1311,  1313,  1315,
    1318,  1320,  1322,  1324,  1326,  1330,  1332,  1334,  1337,  1340,
    1342,  1344,  1346,  1348,  1350,  1352,  1354,  1356,  1358,  1360,
    1362,  1364,  1366,  1368,  1370,  1372,  1373,  1378,  1381,  1386,
    1389,  1396,  1401,  1406,  1409,  1414,  1417,  1420,  1422,  1423,
    1425,  1427,  1429,  1431,  1433,  1435,  1439,  1443,  1445,  1449,
    1451,  1453,  1456,  1458,  1460,  1462,  1465,  1468,  1470,  1472,
    1473,  1479,  1481,  1484,  1487,  1489,  1493,  1497,  1499,  1501,
    1503,  1505,  1507,  1509,  1511,  1513,  1515,  1517,  1519,  1521,
    1522,  1524,  1525,  1527,  1528,  1530,  1532,  1534,  1536,  1538,
    1541
};
static const short yyrhs[] =
{
      -1,   133,   135,     0,   135,   218,   202,   221,     0,   136,
     269,     0,   274,     0,   137,     0,   136,   273,   137,     0,
       1,   137,     0,     0,    44,   159,   138,   159,     0,    44,
      52,    52,     0,    44,    52,    60,     0,    44,    52,    59,
       0,     6,   160,     0,   137,    39,   141,     0,   137,    40,
     141,     0,   137,    41,   141,     0,   137,    42,   141,     0,
     137,    43,   137,     0,     0,    46,   139,   120,   135,   121,
       0,    47,   120,   135,   121,     0,   154,   103,   142,     0,
     148,   103,   142,     0,   245,    83,   142,     0,   198,   122,
     167,   123,    83,   142,     0,   198,   124,    50,    83,   142,
       0,   198,   124,    54,    83,   142,     0,   198,    81,    50,
      83,   142,     0,   246,    83,   142,     0,   154,   103,   180,
       0,   148,   103,   166,     0,   148,   103,   180,     0,   140,
       0,   142,     0,   140,    36,   140,     0,   140,    37,   140,
       0,    38,   140,     0,   117,   142,     0,   164,     0,   140,
       0,   147,     0,   143,     0,    29,   170,     0,    21,   170,
       0,    22,   170,     0,   208,     0,   208,   124,   266,   172,
       0,   208,    81,   266,   172,     0,     0,     0,    90,   145,
     204,   146,   135,   121,     0,   265,   172,     0,   265,   172,
     144,     0,   198,   124,   266,   172,     0,   198,   124,   266,
     172,   144,     0,   198,    81,   266,   172,     0,   198,    81,
     266,   172,   144,     0,    31,   172,     0,    30,   172,     0,
     150,     0,    85,   149,   125,     0,   150,     0,    85,   149,
     125,     0,   152,     0,   152,   151,     0,   152,    91,   153,
       0,   152,    91,     0,    91,   153,     0,    91,     0,   153,
       0,    85,   149,   125,     0,   151,   126,     0,   152,   151,
     126,     0,   243,     0,   198,   122,   167,   123,     0,   198,
     124,    50,     0,   198,    81,    50,     0,   198,   124,    54,
       0,   198,    81,    54,     0,    82,    54,     0,   246,     0,
     243,     0,   198,   122,   167,   123,     0,   198,   124,    50,
       0,   198,    81,    50,     0,   198,   124,    54,     0,   198,
      81,    54,     0,    82,    54,     0,   246,     0,    50,     0,
      54,     0,    82,   155,     0,   155,     0,   198,    81,   155,
       0,    50,     0,    54,     0,    51,     0,   162,     0,   163,
       0,   157,     0,   239,     0,   158,     0,   241,     0,   159,
       0,     0,   160,   126,   161,   159,     0,   108,     0,   109,
       0,   110,     0,    65,     0,    66,     0,    67,     0,    73,
       0,   106,     0,    69,     0,   107,     0,    70,     0,    79,
       0,    80,     0,   111,     0,   112,     0,   113,     0,    91,
       0,   114,     0,   115,     0,    64,     0,   118,     0,    62,
       0,    63,     0,    77,     0,    78,     0,   127,     0,    48,
       0,    49,     0,    46,     0,    47,     0,    44,     0,    36,
       0,     7,     0,    21,     0,    16,     0,     3,     0,     5,
       0,    45,     0,    26,     0,    15,     0,    14,     0,    10,
       0,     9,     0,    35,     0,    20,     0,    25,     0,     4,
       0,    22,     0,    33,     0,    38,     0,    37,     0,    23,
       0,     8,     0,    24,     0,    29,     0,    32,     0,    31,
       0,    13,     0,    34,     0,     6,     0,    17,     0,    30,
       0,    11,     0,    12,     0,    18,     0,    19,     0,   154,
     103,   164,     0,   154,   103,   164,    43,   164,     0,   245,
      83,   164,     0,   198,   122,   167,   123,    83,   164,     0,
     198,   124,    50,    83,   164,     0,   198,   124,    54,    83,
     164,     0,   198,    81,    50,    83,   164,     0,   198,    81,
      54,    83,   164,     0,    82,    54,    83,   164,     0,   246,
      83,   164,     0,   164,    75,   164,     0,   164,    76,   164,
       0,   164,   111,   164,     0,   164,   112,   164,     0,   164,
     113,   164,     0,   164,   114,   164,     0,   164,   115,   164,
       0,   164,    64,   164,     0,   116,    56,    64,   164,     0,
     116,    57,    64,   164,     0,    62,   164,     0,    63,   164,
       0,   164,   108,   164,     0,   164,   109,   164,     0,   164,
     110,   164,     0,   164,    65,   164,     0,   164,   106,   164,
       0,   164,    69,   164,     0,   164,   107,   164,     0,   164,
      70,   164,     0,   164,    66,   164,     0,   164,    67,   164,
       0,   164,    68,   164,     0,   164,    73,   164,     0,   164,
      74,   164,     0,   117,   164,     0,   118,   164,     0,   164,
      79,   164,     0,   164,    80,   164,     0,   164,    71,   164,
       0,   164,    72,   164,     0,     0,    45,   270,   165,   164,
       0,   164,   104,   164,   105,   164,     0,   181,     0,   164,
       0,   274,     0,   147,   270,     0,   179,   271,     0,   179,
     126,    91,   164,   270,     0,   263,   271,     0,    91,   164,
     270,     0,   128,   274,   125,     0,   128,   170,   270,   125,
       0,   128,   208,   270,   125,     0,   128,   179,   126,   208,
     270,   125,     0,   274,     0,   168,     0,   147,     0,   179,
     178,     0,   179,   126,    91,   166,   178,     0,   263,   178,
       0,   263,   126,    91,   166,   178,     0,   179,   126,   263,
     178,     0,   179,   126,   263,   126,    91,   164,   178,     0,
      91,   166,   178,     0,   177,     0,   166,   126,   179,   178,
       0,   166,   126,   177,     0,   166,   126,    91,   166,   178,
       0,   166,   126,   179,   126,    91,   166,   178,     0,   263,
     178,     0,   263,   126,    91,   166,   178,     0,   166,   126,
     263,   178,     0,   166,   126,   179,   126,   263,   178,     0,
     166,   126,   263,   126,    91,   166,   178,     0,   166,   126,
     179,   126,   263,   126,    91,   166,   178,     0,    91,   166,
     178,     0,   177,     0,     0,   173,   174,     0,   170,     0,
       0,    86,   175,   125,     0,     0,    86,   171,   176,   125,
       0,    92,   166,     0,   126,   177,     0,   274,     0,   166,
       0,   179,   126,   166,     0,   179,   126,   166,     0,   179,
     126,    91,   166,     0,    91,   166,     0,   222,     0,   223,
       0,   226,     0,   227,     0,   228,     0,   231,     0,   244,
       0,   246,     0,    51,     0,     0,     7,   182,   134,    10,
       0,     0,    86,   140,   183,   270,   125,     0,    85,   135,
     125,     0,   198,    81,    54,     0,    82,    54,     0,   198,
     122,   167,   123,     0,    88,   167,   123,     0,    89,   262,
     121,     0,    29,     0,    30,   128,   170,   125,     0,    30,
     128,   125,     0,    30,     0,     0,    45,   270,   128,   184,
     140,   125,     0,   265,   210,     0,   209,     0,   209,   210,
       0,    11,   141,   199,   135,   201,    10,     0,    12,   141,
     199,   135,   202,    10,     0,     0,     0,    18,   185,   141,
     200,   186,   135,    10,     0,     0,     0,    19,   187,   141,
     200,   188,   135,    10,     0,    16,   141,   269,   215,    10,
       0,    16,   269,   215,    10,     0,    16,   269,    15,   135,
      10,     0,     0,     0,    20,   203,    25,   189,   141,   200,
     190,   135,    10,     0,     0,     3,   156,   247,   191,   134,
      10,     0,     0,     0,     3,    79,   140,   192,   272,   193,
     134,    10,     0,     0,     4,   156,   194,   134,    10,     0,
       0,     5,   157,   195,   249,   134,    10,     0,     0,     0,
       5,   260,   268,   196,   157,   197,   249,   134,    10,     0,
      21,     0,    22,     0,    23,     0,    24,     0,   181,     0,
     272,     0,   105,     0,    13,     0,   272,    13,     0,   272,
       0,   105,     0,    27,     0,   202,     0,    14,   141,   199,
     135,   201,     0,   274,     0,    15,   135,     0,   154,     0,
     148,     0,   274,     0,   108,   108,     0,    72,     0,   108,
     203,   108,     0,     0,     0,    28,   206,   204,   207,   135,
      10,     0,   147,   205,     0,   208,   124,   266,   169,     0,
     208,    81,   266,   169,     0,   265,   168,     0,   198,   124,
     266,   169,     0,   198,    81,   266,   168,     0,   198,    81,
     267,     0,    31,   168,     0,    31,     0,     0,     0,   120,
     211,   204,   212,   135,   121,     0,     0,     0,    26,   213,
     204,   214,   135,    10,     0,    17,   216,   199,   135,   217,
       0,   179,     0,   179,   126,    91,   166,     0,    91,   166,
       0,   202,     0,   215,     0,     8,   219,   220,   199,   135,
     218,     0,   274,     0,   166,     0,   180,     0,   274,     0,
      84,   154,     0,   274,     0,     9,   135,     0,   274,     0,
     242,     0,   239,     0,   241,     0,   224,     0,   225,     0,
     224,   225,     0,    94,   233,   101,     0,    95,   234,   101,
       0,    96,   234,    61,     0,    97,   129,   101,     0,    97,
     229,   101,     0,     0,   229,   230,   129,     0,   235,     0,
     230,   235,     0,    98,   129,   101,     0,    98,   232,   101,
       0,     0,   232,    58,   129,     0,     0,   233,   235,     0,
       0,   234,   235,     0,    58,     0,     0,   100,   236,   238,
       0,     0,    99,   237,   135,   121,     0,    52,     0,    53,
       0,    55,     0,   246,     0,    93,   240,     0,   157,     0,
      53,     0,    52,     0,    55,     0,    93,   234,   101,     0,
      56,     0,    57,     0,   116,    56,     0,   116,    57,     0,
      50,     0,    53,     0,    52,     0,    54,     0,    55,     0,
      33,     0,    32,     0,    34,     0,    35,     0,    49,     0,
      48,     0,   243,     0,   243,     0,    59,     0,    60,     0,
     272,     0,     0,   107,   248,   141,   272,     0,     1,   272,
       0,   128,   250,   270,   125,     0,   250,   272,     0,   252,
     126,   254,   126,   256,   259,     0,   252,   126,   254,   259,
       0,   252,   126,   256,   259,     0,   252,   259,     0,   254,
     126,   256,   259,     0,   254,   259,     0,   256,   259,     0,
     258,     0,     0,    54,     0,    53,     0,    52,     0,    55,
       0,    50,     0,   251,     0,   252,   126,   251,     0,    50,
     103,   166,     0,   253,     0,   254,   126,   253,     0,   113,
       0,    91,     0,   255,    50,     0,   255,     0,   110,     0,
      92,     0,   257,    50,     0,   126,   258,     0,   274,     0,
     244,     0,     0,   128,   261,   140,   270,   125,     0,   274,
       0,   263,   271,     0,   179,   271,     0,   264,     0,   263,
     126,   264,     0,   166,    84,   166,     0,    50,     0,    54,
       0,    51,     0,    50,     0,    54,     0,    51,     0,   162,
       0,    50,     0,    51,     0,   162,     0,   124,     0,    81,
       0,     0,   273,     0,     0,   130,     0,     0,   130,     0,
     126,     0,   131,     0,   130,     0,   272,     0,   273,   131,
       0,     0
};

#endif

#if YYDEBUG
/* YYRLINE[YYN] -- source line where rule number YYN was defined. */
static const short yyrline[] =
{
       0,   346,   346,   371,   391,   398,   399,   403,   407,   413,
     413,   417,   421,   428,   433,   437,   446,   455,   467,   479,
     483,   483,   497,   505,   509,   515,   540,   555,   567,   579,
     591,   596,   600,   605,   610,   613,   614,   618,   622,   626,
     630,   633,   640,   641,   642,   646,   650,   656,   657,   661,
     667,   667,   667,   682,   687,   699,   704,   716,   721,   733,
     738,   745,   746,   752,   753,   759,   763,   767,   771,   775,
     779,   785,   786,   792,   796,   802,   806,   810,   814,   818,
     822,   828,   834,   841,   845,   849,   853,   857,   861,   867,
     873,   880,   884,   887,   891,   895,   901,   902,   903,   904,
     909,   916,   917,   920,   924,   927,   931,   931,   937,   938,
     939,   940,   941,   942,   943,   944,   945,   946,   947,   948,
     949,   950,   951,   952,   953,   954,   955,   956,   957,   958,
     959,   960,   961,   962,   965,   965,   965,   965,   966,   966,
     966,   966,   966,   966,   966,   967,   967,   967,   967,   967,
     967,   967,   968,   968,   968,   968,   968,   968,   969,   969,
     969,   969,   969,   969,   969,   970,   970,   970,   970,   970,
     971,   971,   971,   971,   974,   978,   982,  1007,  1022,  1034,
    1046,  1058,  1063,  1068,  1073,  1086,  1099,  1103,  1107,  1111,
    1115,  1119,  1123,  1127,  1131,  1140,  1144,  1148,  1152,  1156,
    1160,  1164,  1168,  1172,  1176,  1180,  1184,  1188,  1192,  1196,
    1200,  1204,  1208,  1212,  1216,  1220,  1220,  1225,  1230,  1236,
    1243,  1244,  1249,  1253,  1258,  1262,  1269,  1273,  1277,  1282,
    1289,  1290,  1293,  1298,  1302,  1307,  1312,  1317,  1322,  1328,
    1332,  1335,  1339,  1343,  1348,  1353,  1358,  1363,  1368,  1373,
    1378,  1383,  1387,  1390,  1390,  1402,  1403,  1403,  1408,  1408,
    1415,  1421,  1425,  1428,  1432,  1438,  1442,  1446,  1452,  1453,
    1454,  1455,  1456,  1457,  1458,  1459,  1460,  1464,  1464,  1477,
    1477,  1482,  1486,  1490,  1494,  1502,  1511,  1515,  1519,  1523,
    1527,  1531,  1531,  1536,  1542,  1543,  1552,  1565,  1578,  1578,
    1578,  1588,  1588,  1588,  1598,  1605,  1609,  1613,  1613,  1613,
    1620,  1620,  1636,  1636,  1636,  1658,  1658,  1674,  1674,  1692,
    1692,  1692,  1707,  1711,  1715,  1719,  1725,  1732,  1733,  1734,
    1735,  1738,  1739,  1740,  1743,  1744,  1753,  1754,  1760,  1761,
    1764,  1765,  1769,  1773,  1779,  1779,  1779,  1794,  1803,  1807,
    1813,  1818,  1823,  1828,  1832,  1836,  1842,  1842,  1842,  1854,
    1854,  1854,  1868,  1875,  1876,  1880,  1886,  1887,  1890,  1901,
    1904,  1908,  1909,  1912,  1916,  1919,  1927,  1930,  1931,  1935,
    1938,  1951,  1952,  1958,  1964,  1987,  2020,  2024,  2030,  2034,
    2040,  2041,  2047,  2051,  2057,  2061,  2067,  2071,  2077,  2081,
    2087,  2088,  2088,  2099,  2099,  2120,  2121,  2122,  2123,  2126,
    2133,  2134,  2135,  2136,  2139,  2172,  2173,  2174,  2178,  2184,
    2185,  2186,  2187,  2188,  2189,  2190,  2191,  2192,  2193,  2194,
    2197,  2203,  2209,  2210,  2213,  2217,  2217,  2225,  2228,  2234,
    2240,  2244,  2248,  2252,  2256,  2260,  2264,  2268,  2272,  2278,
    2282,  2286,  2290,  2294,  2305,  2306,  2312,  2322,  2327,  2333,
    2334,  2337,  2346,  2357,  2358,  2361,  2371,  2375,  2378,  2388,
    2388,  2413,  2414,  2418,  2427,  2428,  2434,  2440,  2441,  2442,
    2445,  2446,  2447,  2448,  2451,  2452,  2453,  2456,  2457,  2460,
    2461,  2464,  2465,  2468,  2469,  2470,  2473,  2474,  2477,  2478,
    2481
};
#endif


#if (YYDEBUG) || defined YYERROR_VERBOSE

/* YYTNAME[TOKEN_NUM] -- String name of the token TOKEN_NUM. */
static const char *const yytname[] =
{
  "$", "error", "$undefined.", "kCLASS", "kMODULE", "kDEF", "kUNDEF", 
  "kBEGIN", "kRESCUE", "kENSURE", "kEND", "kIF", "kUNLESS", "kTHEN", 
  "kELSIF", "kELSE", "kCASE", "kWHEN", "kWHILE", "kUNTIL", "kFOR", 
  "kBREAK", "kNEXT", "kREDO", "kRETRY", "kIN", "kDO", "kDO_COND", 
  "kDO_BLOCK", "kRETURN", "kYIELD", "kSUPER", "kSELF", "kNIL", "kTRUE", 
  "kFALSE", "kAND", "kOR", "kNOT", "kIF_MOD", "kUNLESS_MOD", "kWHILE_MOD", 
  "kUNTIL_MOD", "kRESCUE_MOD", "kALIAS", "kDEFINED", "klBEGIN", "klEND", 
  "k__LINE__", "k__FILE__", "tIDENTIFIER", "tFID", "tGVAR", "tIVAR", 
  "tCONSTANT", "tCVAR", "tINTEGER", "tFLOAT", "tSTRING_CONTENT", 
  "tNTH_REF", "tBACK_REF", "tREGEXP_END", "tUPLUS", "tUMINUS", "tPOW", 
  "tCMP", "tEQ", "tEQQ", "tNEQ", "tGEQ", "tLEQ", "tANDOP", "tOROP", 
  "tMATCH", "tNMATCH", "tDOT2", "tDOT3", "tAREF", "tASET", "tLSHFT", 
  "tRSHFT", "tCOLON2", "tCOLON3", "tOP_ASGN", "tASSOC", "tLPAREN", 
  "tLPAREN_ARG", "tRPAREN", "tLBRACK", "tLBRACE", "tLBRACE_ARG", "tSTAR", 
  "tAMPER", "tSYMBEG", "tSTRING_BEG", "tXSTRING_BEG", "tREGEXP_BEG", 
  "tWORDS_BEG", "tQWORDS_BEG", "tSTRING_DBEG", "tSTRING_DVAR", 
  "tSTRING_END", "tLOWEST", "'='", "'?'", "':'", "'>'", "'<'", "'|'", 
  "'^'", "'&'", "'+'", "'-'", "'*'", "'/'", "'%'", "tUMINUS_NUM", "'!'", 
  "'~'", "tLAST_TOKEN", "'{'", "'}'", "'['", "']'", "'.'", "')'", "','", 
  "'`'", "'('", "' '", "'\\n'", "';'", "program", "@1", "bodystmt", 
  "compstmt", "stmts", "stmt", "@2", "@3", "expr", "expr_value", 
  "command_call", "block_command", "cmd_brace_block", "@4", "@5", 
  "command", "mlhs", "mlhs_entry", "mlhs_basic", "mlhs_item", "mlhs_head", 
  "mlhs_node", "lhs", "cname", "cpath", "fname", "fsym", "fitem", 
  "undef_list", "@6", "op", "reswords", "arg", "@7", "arg_value", 
  "aref_args", "paren_args", "opt_paren_args", "call_args", "call_args2", 
  "command_args", "@8", "open_args", "@9", "@10", "block_arg", 
  "opt_block_arg", "args", "mrhs", "primary", "@11", "@12", "@13", "@14", 
  "@15", "@16", "@17", "@18", "@19", "@20", "@21", "@22", "@23", "@24", 
  "@25", "@26", "primary_value", "then", "do", "if_tail", "opt_else", 
  "block_var", "opt_block_var", "do_block", "@27", "@28", "block_call", 
  "method_call", "brace_block", "@29", "@30", "@31", "@32", "case_body", 
  "when_args", "cases", "opt_rescue", "exc_list", "exc_var", "opt_ensure", 
  "literal", "strings", "string", "string1", "xstring", "regexp", "words", 
  "word_list", "word", "qwords", "qword_list", "string_contents", 
  "xstring_contents", "string_content", "@33", "@34", "string_dvar", 
  "symbol", "sym", "dsym", "numeric", "variable", "var_ref", "var_lhs", 
  "backref", "superclass", "@35", "f_arglist", "f_args", "f_norm_arg", 
  "f_arg", "f_opt", "f_optarg", "restarg_mark", "f_rest_arg", 
  "blkarg_mark", "f_block_arg", "opt_f_block_arg", "singleton", "@36", 
  "assoc_list", "assocs", "assoc", "operation", "operation2", 
  "operation3", "dot_or_colon", "opt_terms", "opt_nl", "trailer", "term", 
  "terms", "none", 0
};
#endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives. */
static const short yyr1[] =
{
       0,   133,   132,   134,   135,   136,   136,   136,   136,   138,
     137,   137,   137,   137,   137,   137,   137,   137,   137,   137,
     139,   137,   137,   137,   137,   137,   137,   137,   137,   137,
     137,   137,   137,   137,   137,   140,   140,   140,   140,   140,
     140,   141,   142,   142,   142,   142,   142,   143,   143,   143,
     145,   146,   144,   147,   147,   147,   147,   147,   147,   147,
     147,   148,   148,   149,   149,   150,   150,   150,   150,   150,
     150,   151,   151,   152,   152,   153,   153,   153,   153,   153,
     153,   153,   153,   154,   154,   154,   154,   154,   154,   154,
     154,   155,   155,   156,   156,   156,   157,   157,   157,   157,
     157,   158,   158,   159,   159,   160,   161,   160,   162,   162,
     162,   162,   162,   162,   162,   162,   162,   162,   162,   162,
     162,   162,   162,   162,   162,   162,   162,   162,   162,   162,
     162,   162,   162,   162,   163,   163,   163,   163,   163,   163,
     163,   163,   163,   163,   163,   163,   163,   163,   163,   163,
     163,   163,   163,   163,   163,   163,   163,   163,   163,   163,
     163,   163,   163,   163,   163,   163,   163,   163,   163,   163,
     163,   163,   163,   163,   164,   164,   164,   164,   164,   164,
     164,   164,   164,   164,   164,   164,   164,   164,   164,   164,
     164,   164,   164,   164,   164,   164,   164,   164,   164,   164,
     164,   164,   164,   164,   164,   164,   164,   164,   164,   164,
     164,   164,   164,   164,   164,   165,   164,   164,   164,   166,
     167,   167,   167,   167,   167,   167,   168,   168,   168,   168,
     169,   169,   170,   170,   170,   170,   170,   170,   170,   170,
     170,   171,   171,   171,   171,   171,   171,   171,   171,   171,
     171,   171,   171,   173,   172,   174,   175,   174,   176,   174,
     177,   178,   178,   179,   179,   180,   180,   180,   181,   181,
     181,   181,   181,   181,   181,   181,   181,   182,   181,   183,
     181,   181,   181,   181,   181,   181,   181,   181,   181,   181,
     181,   184,   181,   181,   181,   181,   181,   181,   185,   186,
     181,   187,   188,   181,   181,   181,   181,   189,   190,   181,
     191,   181,   192,   193,   181,   194,   181,   195,   181,   196,
     197,   181,   181,   181,   181,   181,   198,   199,   199,   199,
     199,   200,   200,   200,   201,   201,   202,   202,   203,   203,
     204,   204,   204,   204,   206,   207,   205,   208,   208,   208,
     209,   209,   209,   209,   209,   209,   211,   212,   210,   213,
     214,   210,   215,   216,   216,   216,   217,   217,   218,   218,
     219,   219,   219,   220,   220,   221,   221,   222,   222,   222,
     223,   224,   224,   225,   226,   227,   228,   228,   229,   229,
     230,   230,   231,   231,   232,   232,   233,   233,   234,   234,
     235,   236,   235,   237,   235,   238,   238,   238,   238,   239,
     240,   240,   240,   240,   241,   242,   242,   242,   242,   243,
     243,   243,   243,   243,   243,   243,   243,   243,   243,   243,
     244,   245,   246,   246,   247,   248,   247,   247,   249,   249,
     250,   250,   250,   250,   250,   250,   250,   250,   250,   251,
     251,   251,   251,   251,   252,   252,   253,   254,   254,   255,
     255,   256,   256,   257,   257,   258,   259,   259,   260,   261,
     260,   262,   262,   262,   263,   263,   264,   265,   265,   265,
     266,   266,   266,   266,   267,   267,   267,   268,   268,   269,
     269,   270,   270,   271,   271,   271,   272,   272,   273,   273,
     274
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN. */
static const short yyr2[] =
{
       0,     0,     2,     4,     2,     1,     1,     3,     2,     0,
       4,     3,     3,     3,     2,     3,     3,     3,     3,     3,
       0,     5,     4,     3,     3,     3,     6,     5,     5,     5,
       3,     3,     3,     3,     1,     1,     3,     3,     2,     2,
       1,     1,     1,     1,     2,     2,     2,     1,     4,     4,
       0,     0,     6,     2,     3,     4,     5,     4,     5,     2,
       2,     1,     3,     1,     3,     1,     2,     3,     2,     2,
       1,     1,     3,     2,     3,     1,     4,     3,     3,     3,
       3,     2,     1,     1,     4,     3,     3,     3,     3,     2,
       1,     1,     1,     2,     1,     3,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     0,     4,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     3,     5,     3,     6,     5,     5,
       5,     5,     4,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     4,     4,     2,     2,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     2,
       2,     3,     3,     3,     3,     0,     4,     5,     1,     1,
       1,     2,     2,     5,     2,     3,     3,     4,     4,     6,
       1,     1,     1,     2,     5,     2,     5,     4,     7,     3,
       1,     4,     3,     5,     7,     2,     5,     4,     6,     7,
       9,     3,     1,     0,     2,     1,     0,     3,     0,     4,
       2,     2,     1,     1,     3,     3,     4,     2,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     0,     4,     0,
       5,     3,     3,     2,     4,     3,     3,     1,     4,     3,
       1,     0,     6,     2,     1,     2,     6,     6,     0,     0,
       7,     0,     0,     7,     5,     4,     5,     0,     0,     9,
       0,     6,     0,     0,     8,     0,     5,     0,     6,     0,
       0,     9,     1,     1,     1,     1,     1,     1,     1,     1,
       2,     1,     1,     1,     1,     5,     1,     2,     1,     1,
       1,     2,     1,     3,     0,     0,     6,     2,     4,     4,
       2,     4,     4,     3,     2,     1,     0,     0,     6,     0,
       0,     6,     5,     1,     4,     2,     1,     1,     6,     1,
       1,     1,     1,     2,     1,     2,     1,     1,     1,     1,
       1,     1,     2,     3,     3,     3,     3,     3,     0,     3,
       1,     2,     3,     3,     0,     3,     0,     2,     0,     2,
       1,     0,     3,     0,     4,     1,     1,     1,     1,     2,
       1,     1,     1,     1,     3,     1,     1,     2,     2,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     0,     4,     2,     4,     2,
       6,     4,     4,     2,     4,     2,     2,     1,     0,     1,
       1,     1,     1,     1,     1,     3,     3,     1,     3,     1,
       1,     2,     1,     1,     1,     2,     2,     1,     1,     0,
       5,     1,     2,     2,     1,     3,     3,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     0,
       1,     0,     1,     0,     1,     1,     1,     1,     1,     2,
       0
};

/* YYDEFACT[S] -- default rule to reduce with in state S when YYTABLE
   doesn't specify something else to do.  Zero means the default is an
   error. */
static const short yydefact[] =
{
       1,     0,     0,     0,     0,     0,     0,   277,     0,     0,
     489,   298,   301,     0,   322,   323,   324,   325,   287,   290,
     355,   425,   424,   426,   427,     0,     0,   491,    20,     0,
     429,   428,   419,   276,   421,   420,   422,   423,   415,   416,
     432,   433,     0,     0,     0,     0,     0,   500,   500,    70,
     398,   396,   398,   398,   388,   394,     0,     0,     0,     2,
     489,     6,    34,    35,    43,    42,     0,    61,     0,    65,
      71,     0,    40,   218,     0,    47,   294,   268,   269,   380,
     381,   270,   271,   272,   273,   378,   379,   377,   430,   274,
       0,   275,   253,     5,     8,   322,   323,   287,   290,   355,
     491,    91,    92,     0,     0,     0,     0,    94,     0,   326,
       0,   430,   275,     0,   315,   143,   154,   144,   167,   140,
     160,   150,   149,   170,   171,   165,   148,   147,   142,   168,
     172,   173,   152,   141,   155,   159,   161,   153,   146,   162,
     169,   164,   163,   156,   166,   151,   139,   158,   157,   138,
     145,   136,   137,   134,   135,    96,    98,    97,   129,   130,
     127,   111,   112,   113,   116,   118,   114,   131,   132,   119,
     120,   124,   115,   117,   108,   109,   110,   121,   122,   123,
     125,   126,   128,   133,   469,   317,    99,   100,   468,     0,
     163,   156,   166,   151,   134,   135,    96,    97,   101,   103,
     105,    14,   102,   104,     0,     0,    41,     0,     0,     0,
     430,     0,   275,     0,   497,   496,   489,     0,   498,   490,
       0,     0,     0,   339,   338,     0,     0,   430,   275,     0,
       0,     0,   232,   219,   263,    45,   240,   500,   500,   474,
      46,    44,     0,    60,     0,   500,   354,    59,    38,     0,
       9,   492,   215,     0,     0,   194,     0,   195,   283,     0,
       0,     0,    61,   279,     0,   491,     0,   493,   493,   220,
     493,     0,   493,   471,     0,    69,     0,    75,    82,   412,
     411,   413,   410,     0,   409,     0,     0,     0,     0,     0,
       0,     0,   417,   418,    39,   209,   210,     4,   490,     0,
       0,     0,     0,     0,     0,     0,   344,   347,     0,    73,
       0,    68,    66,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   500,     0,     0,     0,   359,   356,   295,   382,     0,
       0,   350,    53,   293,     0,   312,    91,    92,    93,   417,
     418,     0,   435,   310,   434,     0,   500,     0,     0,     0,
     448,   488,   487,   319,   106,     0,   500,   283,   329,   328,
       0,   327,     0,     0,   500,     0,     0,     0,     0,     0,
       0,     0,     0,   499,     0,     0,   283,     0,   500,     0,
     307,   500,   260,     0,     0,   233,   262,     0,   235,   289,
       0,   256,   255,   254,   232,   491,   500,   491,     0,    11,
      13,    12,     0,   291,     0,     0,     0,     0,     0,     0,
       0,   281,    62,   491,   491,   221,   285,   495,   494,   222,
     495,   224,   495,   473,   286,   472,    81,     0,   500,     0,
     400,   403,   401,   414,   399,   383,   397,   384,   385,   386,
     387,     0,   390,   392,     0,   393,     0,     0,     7,    15,
      16,    17,    18,    19,    36,    37,   500,     0,    24,    32,
       0,    33,     0,    67,    74,    23,   174,   263,    31,   191,
     199,   204,   205,   206,   201,   203,   213,   214,   207,   208,
     184,   185,   211,   212,     0,   200,   202,   196,   197,   198,
     186,   187,   188,   189,   190,   480,   485,   481,   486,   253,
     353,     0,   480,   482,   481,   483,   500,   480,   481,   253,
     253,   500,   500,    25,   176,    30,   183,    50,    54,     0,
     437,     0,     0,    91,    92,    95,     0,     0,   500,     0,
     491,   453,   451,   450,   449,   452,   460,   464,   463,   459,
     448,     0,     0,   454,   500,   457,   500,   462,   500,     0,
     447,     0,     0,   278,   500,   500,   369,   500,   330,   174,
     484,   282,     0,   480,   481,   500,     0,     0,     0,   363,
       0,   305,   333,   332,   299,   331,   302,   484,   282,     0,
     480,   481,     0,     0,   239,   476,     0,   264,   261,   500,
       0,     0,   475,   288,     0,    40,     0,   258,     0,   252,
     500,     0,     0,     0,     0,     0,   226,    10,     0,   216,
       0,    22,   182,    62,     0,   225,     0,   264,    78,    80,
       0,   480,   481,     0,     0,   389,   391,   395,   192,   193,
     342,     0,   345,   340,   267,     0,    72,     0,     0,     0,
       0,   352,    57,   284,     0,     0,   231,   351,    55,   230,
     349,    49,   348,    48,   360,   357,   500,   313,     0,     0,
     284,   316,     0,     0,   491,     0,   439,     0,   443,   467,
       0,   445,   461,     0,   446,   465,   320,   107,   370,   371,
     500,   372,     0,   500,   336,     0,     0,   334,     0,   284,
       0,     0,     0,   304,   306,   365,     0,     0,     0,     0,
     284,     0,   500,     0,   237,   500,   500,     0,     0,   257,
       0,   245,   227,     0,   491,   500,   500,   228,     0,    21,
     280,   491,    76,     0,   405,   406,   407,   402,   408,   341,
       0,     0,     0,   265,   175,   217,    29,   180,   181,    58,
       0,    27,   178,    28,   179,    56,     0,     0,    51,     0,
     436,   311,   470,   456,     0,   318,   455,   500,   500,   466,
       0,   458,   500,   448,     0,     0,   374,   337,     0,     3,
     376,     0,   296,     0,   297,     0,   500,     0,     0,   308,
     234,     0,   236,   251,     0,   242,   500,   500,   259,     0,
       0,   292,   223,   404,   343,     0,   266,    26,   177,     0,
       0,     0,     0,   438,     0,   441,   442,   444,     0,     0,
     373,     0,    83,    90,     0,   375,     0,   364,   366,   367,
     362,   300,   303,     0,   500,   500,     0,   241,     0,   247,
     500,   229,   346,   361,   358,     0,   314,   500,     0,    89,
       0,   500,     0,   500,   500,     0,   238,   243,     0,   500,
       0,   246,    52,   440,   321,   484,    88,     0,   480,   481,
     368,   335,   309,   500,     0,   248,   500,    84,   244,     0,
     249,   500,   250,     0,     0,     0
};

static const short yydefgoto[] =
{
     893,     1,   375,   376,    60,    61,   422,   253,    62,   207,
      63,    64,   538,   676,   821,    65,    66,   261,    67,    68,
      69,    70,   208,   107,   108,   198,   199,   200,   201,   572,
     525,   187,    72,   424,   234,   266,   666,   667,   235,   617,
     243,   244,   413,   618,   728,   608,   405,   267,   481,    73,
     204,   433,   628,   220,   718,   221,   719,   602,   843,   542,
     539,   769,   368,   370,   571,   783,   256,   380,   594,   706,
     707,   226,   652,   307,   476,   751,    75,    76,   353,   532,
     767,   531,   766,   392,   590,   840,   575,   700,   785,   789,
      77,    78,    79,    80,    81,    82,    83,   289,   461,    84,
     291,   285,   283,   454,   644,   643,   747,    85,   284,    86,
      87,   210,    89,   211,   212,   363,   541,   561,   562,   563,
     564,   565,   566,   567,   568,   569,   779,   688,   189,   369,
     271,   268,   239,   113,   546,   520,   373,   217,   252,   439,
     381,   219,    93
};

static const short yypact[] =
{
  -32768,  2226,  5427,  7895,  8192,  3759,  5077,-32768,  6766,  6766,
    3630,-32768,-32768,  7994,  5633,  5633,-32768,-32768,  5633,  4392,
    4495,-32768,-32768,-32768,-32768,  6766,  4952,   -42,-32768,   -25,
  -32768,-32768,  3877,  3980,-32768,-32768,  4083,-32768,-32768,-32768,
  -32768,-32768,  7590,  7590,    43,  3040,  6766,  6869,  7590,  8291,
    4827,-32768,-32768,-32768,    33,    65,   319,  7693,  7590,-32768,
     350,   860,   505,-32768,-32768,    99,   103,-32768,   131,  8093,
  -32768,   122,  9396,   148,   457,   -28,    45,-32768,-32768,   193,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,    30,-32768,
     199,    31,    20,-32768,   860,-32768,-32768,-32768,   167,   170,
     -42,   143,   355,  6766,   188,  3171,   508,-32768,    44,-32768,
     503,-32768,-32768,    20,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,    18,    22,    47,    54,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,   293,   369,   424,-32768,   435,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,   438,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,   200,-32768,-32768,  2468,   294,   505,    78,   250,   543,
      26,   303,    34,    78,-32768,-32768,   350,    95,-32768,   261,
    6766,  6766,   349,-32768,-32768,   579,   384,   233,   237,  7590,
    7590,  7590,-32768,  9396,   330,-32768,-32768,   304,   316,-32768,
  -32768,-32768,  5319,-32768,  5736,  5633,-32768,-32768,-32768,   388,
  -32768,-32768,   292,   331,  3302,-32768,   617,   392,    38,  3040,
     336,   371,   381,   505,  7590,   -42,   403,   149,   405,-32768,
     428,   408,   405,-32768,   478,-32768,   625,   629,   633,-32768,
  -32768,-32768,-32768,   537,-32768,   553,   563,   736,   442,   572,
     449,   469,   497,   502,-32768,-32768,-32768,-32768,  3527,  6766,
    6766,  6766,  6766,  5427,  6766,  6766,-32768,-32768,  6972,-32768,
    3040,  8291,   450,  6972,  7590,  7590,  7590,  7590,  7590,  7590,
    7590,  7590,  7590,  7590,  7590,  7590,  7590,  7590,  7590,  7590,
    7590,  7590,  7590,  7590,  7590,  7590,  7590,  7590,  7590,  7590,
    8453,  6869,  8519,  8585,  8585,-32768,-32768,-32768,-32768,  7693,
    7693,-32768,   516,-32768,   292,   505,-32768,   634,-32768,-32768,
  -32768,   350,-32768,-32768,-32768,  8651,  6869,  8585,  2468,  6766,
     565,-32768,-32768,-32768,-32768,   599,   606,   120,-32768,-32768,
    2590,   603,  7590,  8717,  6869,  8783,  7590,  7590,  2834,   611,
    3424,  7075,   621,-32768,    14,    14,   239,  8849,  6869,  8915,
  -32768,   506,-32768,  7590,  5839,-32768,-32768,  5942,-32768,-32768,
     509,  5530,-32768,-32768,    99,   -42,   513,   216,   515,-32768,
  -32768,-32768,  5077,-32768,  7590,  3302,   534,  8717,  8783,  7590,
     536,-32768,   533,   -42,  9243,-32768,-32768,  7178,-32768,-32768,
    7590,-32768,  7590,-32768,-32768,-32768,   634,  8981,  6869,  9047,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,    53,-32768,-32768,   539,-32768,  7590,  7590,   860,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,    28,  7590,-32768,   544,
     550,-32768,   560,-32768,-32768,-32768,  8387,-32768,-32768,   392,
    1935,  1935,  1935,  1935,   712,   712,  9517,  9465,  1935,  1935,
    9448,  9448,   476,   476,  9379,   712,   712,   829,   829,   653,
     358,   358,   392,   392,   392,  2337,  4598,  2056,  4701,   170,
  -32768,   564,    67,-32768,   313,-32768,  4495,-32768,-32768,  1955,
    1955,    28,    28,-32768,  9396,-32768,  9396,-32768,-32768,   350,
  -32768,  6766,  2468,   458,   354,-32768,   170,   566,   170,   676,
      42,   592,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
     627,  2468,   350,-32768,   574,-32768,   581,   655,   582,   661,
  -32768,  5202,  5077,-32768,  7281,   697,-32768,   587,-32768,  2317,
    4186,  4289,   600,   124,   186,   697,   714,   717,  7590,   609,
      78,-32768,-32768,-32768,-32768,-32768,-32768,    58,   133,   613,
     243,   329,  6766,   658,-32768,-32768,  7590,   330,-32768,   626,
    7590,   330,-32768,-32768,  7590,  9295,   134,-32768,   637,-32768,
     643,   645,  6045,  8585,  8585,   647,-32768,-32768,  6766,  9396,
     662,-32768,  9396,   468,   657,-32768,  7590,-32768,   458,   354,
     651,    35,    73,  3302,   725,-32768,-32768,-32768,   392,   392,
  -32768,  7796,-32768,-32768,-32768,  7384,-32768,  7590,  7590,  7693,
    7590,-32768,   516,   391,  7693,  7693,-32768,-32768,   516,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,    28,-32768,   350,   776,
  -32768,-32768,   663,  7590,   -42,   783,-32768,   627,-32768,-32768,
     420,-32768,-32768,    13,-32768,-32768,-32768,-32768,   544,-32768,
     722,-32768,  2937,   787,-32768,  6766,   801,-32768,  7590,   190,
    7590,  7590,   802,-32768,-32768,-32768,  7487,  2712,  3424,  3424,
     396,    14,   506,  6148,-32768,   506,   506,  6251,   689,-32768,
    6354,-32768,-32768,    99,   216,   170,   170,-32768,    79,-32768,
  -32768,  9243,   649,   696,-32768,-32768,-32768,-32768,-32768,-32768,
     711,  3424,  7590,   702,  9396,  9396,-32768,  9396,  9396,-32768,
    7693,-32768,  9396,-32768,  9396,-32768,  3424,  3302,-32768,  2468,
  -32768,-32768,-32768,-32768,   704,-32768,-32768,   705,   582,-32768,
     592,-32768,   582,   565,  8390,    78,-32768,-32768,  3424,-32768,
  -32768,    78,-32768,  7590,-32768,  7590,   451,   823,   827,-32768,
  -32768,  7590,-32768,-32768,  7590,-32768,   713,   716,-32768,  7590,
     715,-32768,-32768,-32768,-32768,   833,-32768,-32768,  9396,   834,
     724,  3302,   838,-32768,   420,-32768,-32768,-32768,  2468,   804,
  -32768,   694,   629,   633,  2468,-32768,  2590,-32768,-32768,-32768,
  -32768,-32768,-32768,  3424,  9316,   506,  6457,-32768,  6560,-32768,
     506,-32768,-32768,-32768,-32768,   734,-32768,   582,   849,   634,
    9113,  6869,  9179,   606,   587,   850,-32768,-32768,  7590,   735,
    7590,-32768,-32768,-32768,-32768,    49,   354,   741,    80,   117,
  -32768,-32768,-32768,   506,  6663,-32768,   506,   649,-32768,  7590,
  -32768,   506,-32768,   865,   867,-32768
};

static const short yypgoto[] =
{
  -32768,-32768,  -307,   890,-32768,    40,-32768,-32768,    56,   158,
     -17,-32768,  -536,-32768,-32768,     1,    -2,  -190,    29,   803,
  -32768,   -37,  1085,   -36,   869,     2,-32768,   -21,-32768,-32768,
      -3,-32768,   -14,-32768,  1352,  -309,   -12,  -405,    41,-32768,
      -6,-32768,-32768,-32768,-32768,    -5,   265,    36,  -246,    23,
  -32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,   187,  -178,  -371,    12,
    -512,   220,  -455,-32768,-32768,-32768,  -227,-32768,   805,-32768,
  -32768,-32768,-32768,  -383,-32768,-32768,    15,-32768,-32768,-32768,
  -32768,-32768,-32768,   798,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,   639,  -195,-32768,-32768,-32768,    11,-32768,    32,
  -32768,   917,   874,  1185,  1055,-32768,-32768,   100,   322,   207,
  -32768,  -651,   208,-32768,  -605,-32768,  -340,  -313,-32768,-32768,
  -32768,     7,  -285,   500,  -236,-32768,-32768,   -29,   -51,   130,
      10,   826,   678
};


#define	YYLAST		9632


static const short yytable[] =
{
     233,   233,   186,   186,   233,   250,   586,   185,   246,   236,
     236,   223,   275,   236,   247,   232,   232,   202,   417,   232,
     218,   238,   238,   186,   596,   238,   109,   109,   255,   257,
     570,   297,   521,   233,   233,   388,   109,   202,   203,   781,
     294,   592,    94,   295,   296,   361,   345,   186,   265,   354,
     237,   237,   282,   343,   237,   272,   240,   547,   203,   241,
     -77,   549,   -86,   703,   206,   206,   206,   488,   358,   430,
     218,   345,   109,   712,   262,   582,   674,   675,   304,   305,
     351,   248,   778,   -86,   270,   782,   352,   246,   251,   599,
     456,   378,   109,   -85,   462,   254,   344,   258,   -79,  -425,
     650,   351,   263,  -424,   519,   557,   526,   529,   530,  -431,
     390,   450,   391,  -431,   350,   304,   305,   387,   364,   593,
     482,   429,   612,   558,   670,   672,   759,   306,  -426,   -83,
     -87,   548,   765,   -83,   -90,  -427,   651,   -90,   -77,   640,
     346,   -89,  -425,   -77,   214,   215,  -424,   519,   245,   526,
     664,   362,   451,   452,   -86,   612,   -75,   -82,   -88,   355,
     -77,   -77,   288,   548,   -81,   346,   -86,   213,   216,  -477,
     -85,  -426,   251,   781,   214,   215,   -79,  -480,  -427,   -86,
     -86,   -79,   645,   379,   -78,   -85,  -480,   389,    74,    74,
     110,   110,   548,   -77,   290,   209,   209,   209,   -79,   -79,
     225,   209,   209,   429,   811,   209,   308,   710,   214,   215,
     -85,   -85,   209,   548,   435,   233,   233,   295,   403,   857,
     570,   768,   -87,   -89,  -419,   313,   218,   -85,   233,  -326,
     233,   233,    74,   209,   209,   679,   276,   236,   356,   236,
     236,   -88,   357,   232,   209,   232,   414,   -87,   -87,   238,
     434,   238,   238,   691,   685,   694,   276,   309,   -83,   -80,
     727,  -481,   -90,  -477,   -89,  -419,   646,  -419,   -85,   711,
    -326,  -477,  -326,   793,   483,   437,   206,   206,   237,   438,
     237,   416,   349,   410,   838,   412,   415,    51,   262,   -87,
     209,   478,    74,   -84,   233,   242,   485,   623,   245,   486,
     489,   490,   491,   492,   493,   494,   495,   496,   497,   498,
     499,   500,   501,   502,   503,   504,   505,   506,   507,   508,
     509,   510,   511,   512,   513,   514,   374,   233,   699,   545,
     670,   672,   533,   535,   109,   534,   536,   518,   468,   262,
     624,   -83,   265,   473,   480,   -90,   251,   -89,   377,   480,
     799,   -85,   233,   382,   -87,   206,   206,   206,   206,   -75,
     474,   475,   518,   -82,   621,   -81,   625,   265,   579,   -77,
     233,   540,   534,   536,  -429,   292,   293,   233,   394,   395,
     518,  -478,   634,   635,   233,   265,   386,   735,   736,   233,
     233,    74,   393,   233,   518,   734,   665,   615,   441,   265,
     443,   627,   445,   396,   595,   595,   619,   209,   209,   400,
     629,   609,   717,   839,   403,   632,   -87,  -429,   620,   186,
     423,   -84,   314,   233,   518,   550,   233,   589,   233,   209,
     404,   209,   209,   202,   233,  -282,  -422,   -87,   612,   -79,
     419,    74,   407,   570,   518,   612,    74,   420,   421,   265,
    -428,   425,   648,   649,   203,   -79,   314,   469,   470,   471,
     472,   431,   822,   233,   825,   826,   702,   263,   391,   827,
     780,   337,   338,   339,   760,  -478,  -282,  -422,  -282,  -422,
     214,   215,  -481,  -478,  -484,    74,   209,   209,   209,   209,
      74,   209,   209,  -428,   -84,   209,   432,    74,   276,   682,
     209,    92,    92,   408,   -84,  -419,   -63,   661,    92,    92,
      92,   556,   557,   662,    92,    92,  -422,   -76,    92,   371,
     668,   858,   -76,   671,   673,    92,   436,   464,   209,   444,
     558,   440,   446,   559,   661,   438,   209,   209,   340,  -484,
     314,   304,   305,   459,   873,    92,    92,    92,  -419,   677,
     463,   697,   877,   209,   442,    74,   209,    92,   438,  -422,
     233,   466,   372,   612,   359,   360,   467,    74,   186,   186,
     465,   209,   686,   696,   233,    74,   484,    74,  -484,   341,
    -484,   342,  -484,   202,   365,   209,  -480,   335,   336,   337,
     338,   339,   233,   -64,   -72,   450,   233,   206,   209,   612,
     233,   705,   702,    92,   203,    92,   537,   834,   233,   573,
     480,   450,    74,   836,   574,   551,   578,   552,   553,   554,
     555,   450,   741,   733,   383,   366,   548,   367,   391,   609,
     450,   591,   603,   774,   613,   209,   451,   452,   453,   622,
     626,   233,   756,   754,   755,   757,   758,   761,   763,   223,
     762,   764,   451,   452,   455,   631,   556,   557,   206,   -72,
     397,   633,   451,   452,   457,   384,   604,   385,   647,   233,
    -263,   451,   452,   460,   109,   558,   655,   551,   559,   552,
     553,   554,   555,   810,   738,   656,   681,   663,   770,   680,
     812,   286,   287,   560,   757,   683,   762,   764,   427,   678,
     687,   398,   233,   399,    92,   692,   447,   690,   693,   233,
    -430,   695,   702,   233,  -275,  -283,   233,   314,   556,   557,
      92,    92,   805,   709,   713,   269,   273,   714,   209,    74,
    -284,   595,   327,   328,   807,   716,   720,   558,   233,   384,
     559,   428,    92,   817,    92,    92,   818,   448,    74,   449,
     230,  -430,   723,  -430,    92,  -275,  -283,  -275,  -283,    92,
     721,   206,   729,   806,   335,   336,   337,   338,   339,   730,
     732,  -284,   737,  -284,   742,   860,   314,   744,   745,   818,
     746,   233,   740,   739,    40,    41,   771,   844,   772,   209,
     233,   327,   328,   775,   450,   233,   788,   458,    92,    92,
      92,    92,    92,    92,    92,    92,   784,   109,    92,   209,
      92,   792,   794,    92,   808,   209,   861,   813,   862,   814,
     332,   333,   334,   335,   336,   337,   338,   339,  -264,   823,
      74,   824,   233,   841,   233,   451,   452,   842,   225,   846,
     851,    92,   848,   852,   853,   854,   209,   233,   856,    92,
      92,   209,   209,   869,   233,   872,   233,   518,   859,   874,
     882,   884,   265,   791,   887,   894,    92,   895,    92,    92,
     233,   750,   312,   114,   724,   233,   881,   348,   880,   188,
      92,   347,   684,   828,    92,   731,   298,     0,    92,    74,
      92,    59,   209,   314,   776,   777,     0,     0,    92,   299,
     300,   301,   302,   303,    74,    74,    74,     0,   327,   328,
       0,    92,     0,     0,     0,   406,   406,     0,    88,    88,
     111,   111,   111,   418,     0,    92,     0,     0,     0,     0,
     227,     0,     0,     0,     0,   260,     0,     0,    74,   334,
     335,   336,   337,   338,   339,     0,     0,   209,    92,     0,
       0,     0,     0,    74,    74,     0,    74,     0,     0,     0,
       0,     0,    88,     0,     0,     0,   277,     0,     0,     0,
       0,   831,     0,     0,     0,    74,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   277,   800,     0,     0,
     802,   803,     0,     0,     0,   260,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    74,     0,
       0,     0,     0,     0,     0,    74,     0,     0,     0,   269,
       0,    74,    88,    74,     0,     0,     0,     0,     0,     0,
      74,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    92,    92,     0,   269,     0,     0,     0,   209,     0,
       0,     0,     0,     0,   576,     0,    91,    91,   112,   112,
       0,    92,   269,     0,     0,     0,     0,     0,   228,     0,
       0,   847,   849,     0,     0,     0,   269,     0,     0,   406,
       0,     0,     0,     0,     0,     0,    71,    71,     0,     0,
       0,     0,     0,     0,   406,     0,     0,     0,   224,     0,
      91,     0,    92,     0,   278,     0,     0,     0,     0,   866,
     867,     0,     0,     0,     0,   871,     0,     0,     0,     0,
       0,    88,    92,     0,   278,     0,   269,     0,    92,     0,
      71,     0,     0,     0,   885,     0,     0,     0,     0,     0,
       0,     0,     0,    92,   426,     0,     0,     0,   888,   260,
       0,   890,     0,     0,   653,     0,   892,     0,     0,    92,
      91,     0,     0,     0,    92,    92,     0,     0,     0,     0,
       0,    88,     0,     0,     0,     0,    88,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    90,    90,     0,     0,
      71,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     260,     0,    92,     0,   669,    92,     0,   669,   669,   653,
     653,     0,     0,     0,     0,    88,     0,    92,    92,    92,
      88,     0,     0,     0,     0,     0,   669,    88,   277,     0,
      90,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   689,     0,   689,     0,   689,     0,     0,     0,
       0,    92,   701,   704,     0,   704,     0,     0,     0,    91,
      92,     0,     0,   704,     0,     0,    92,    92,     0,    92,
     577,     0,     0,     0,     0,     0,     0,     0,   585,     0,
     587,     0,     0,     0,     0,    88,     0,   406,    92,    71,
      90,     0,     0,     0,     0,     0,     0,    88,   406,     0,
       0,     0,     0,     0,     0,    88,     0,    88,     0,    91,
       0,     0,     0,     0,    91,   630,     0,     0,     0,     0,
       0,    92,     0,     0,     0,     0,     0,     0,    92,     0,
       0,     0,     0,     0,    92,     0,    92,     0,     0,    71,
       0,     0,    88,    92,    71,     0,     0,     0,     0,     0,
       0,     0,     0,    91,   653,     0,     0,     0,    91,     0,
       0,    92,     0,     0,     0,    91,   278,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   786,     0,
       0,   790,     0,    71,     0,     0,     0,     0,    71,    90,
       0,     0,     0,     0,     0,    71,     0,     0,     0,     0,
     406,     0,     0,   406,   406,     0,     0,     0,     0,     0,
       0,     0,     0,   669,   669,     0,     0,     0,     0,     0,
       0,     0,     0,    91,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    91,     0,     0,     0,    90,
       0,     0,     0,    91,    90,    91,     0,     0,     0,     0,
       0,     0,     0,    71,     0,   689,   689,     0,     0,    88,
     689,     0,     0,     0,     0,    71,     0,     0,     0,     0,
       0,     0,     0,    71,   704,    71,     0,     0,    88,     0,
      91,     0,     0,    90,   406,   406,     0,     0,    90,     0,
       0,     0,     0,     0,     0,    90,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      71,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   406,   406,     0,     0,     0,     0,   406,     0,
       0,     0,     0,   743,     0,   689,     0,     0,     0,   269,
       0,   576,   704,     0,     0,     0,     0,   406,     0,     0,
       0,     0,     0,    90,     0,     0,     0,     0,     0,     0,
      88,   406,     0,     0,   406,    90,     0,     0,   227,   406,
       0,     0,     0,    90,     0,    90,     0,     0,     0,     0,
       0,   401,   402,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   787,     0,     0,     0,     0,    91,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   796,   797,   798,
      90,     0,     0,     0,     0,     0,    91,     0,     0,    88,
       0,     0,     0,     0,     0,     0,     0,    71,     0,     0,
       0,     0,     0,     0,    88,    88,    88,     0,     0,     0,
       0,   815,     0,     0,     0,     0,    71,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   819,   820,     0,     0,
     479,     0,     0,     0,     0,   487,     0,     0,    88,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   835,     0,
       0,     0,     0,    88,    88,     0,    88,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    91,   748,
       0,   832,     0,     0,     0,    88,   228,     0,     0,     0,
       0,   855,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   863,     0,   864,    90,    71,     0,
       0,     0,     0,   865,     0,     0,   224,     0,    88,     0,
       0,     0,     0,   487,     0,    88,    90,     0,     0,     0,
       0,    88,     0,    88,     0,   605,   607,    91,     0,   611,
      88,     0,     0,   616,     0,     0,     0,     0,     0,     0,
       0,     0,    91,    91,    91,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    71,     0,   637,
       0,     0,   611,     0,   637,     0,     0,     0,     0,     0,
       0,     0,    71,    71,    71,     0,    91,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    91,    91,     0,    91,     0,     0,     0,    90,   654,
       0,     0,     0,     0,     0,     0,    71,     0,     0,   833,
       0,     0,     0,    91,     0,     0,     0,     0,     0,     0,
       0,    71,    71,     0,    71,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   830,
       0,     0,     0,    71,     0,     0,    91,     0,     0,     0,
       0,     0,     0,    91,     0,     0,     0,    90,     0,    91,
       0,    91,     0,     0,     0,     0,     0,     0,    91,     0,
       0,     0,    90,    90,    90,     0,    71,     0,     0,     0,
       0,     0,     0,    71,     0,     0,     0,     0,     0,    71,
       0,    71,     0,     0,     0,     0,   698,     0,    71,     0,
       0,     0,     0,     0,     0,     0,    90,     0,     0,     0,
     715,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    90,    90,     0,    90,  -500,     0,     0,   722,     0,
       0,     0,   725,  -500,  -500,  -500,   726,     0,  -500,  -500,
    -500,     0,  -500,    90,   607,     0,     0,     0,     0,     0,
       0,     0,  -500,     0,     0,     0,     0,     0,     0,     0,
       0,  -500,  -500,     0,  -500,  -500,  -500,  -500,  -500,   314,
  -32768,-32768,-32768,-32768,   319,   320,    90,   753,-32768,-32768,
       0,     0,     0,    90,   327,   328,     0,     0,     0,    90,
       0,    90,     0,     0,     0,     0,     0,     0,    90,     0,
       0,     0,     0,     0,     0,   773,  -500,     0,     0,     0,
       0,   330,   331,   332,   333,   334,   335,   336,   337,   338,
     339,     0,     0,     0,     0,     0,  -282,     0,     0,     0,
    -500,     0,     0,     0,  -282,  -282,  -282,     0,   637,     0,
    -282,  -282,     0,  -282,     0,   611,  -500,     0,     0,  -500,
    -500,     0,   611,   245,     0,  -500,  -500,     0,     0,     0,
       0,     0,  -282,  -282,     0,  -282,  -282,  -282,  -282,  -282,
       0,     0,     0,     0,   816,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    -282,  -282,  -282,  -282,  -282,  -282,  -282,  -282,  -282,  -282,
    -282,  -282,  -282,     0,     0,  -282,  -282,  -282,     0,   660,
       0,     0,     0,     0,     0,     0,     0,   837,     0,     0,
       0,     0,     0,     0,     0,     0,   845,     0,     0,   -88,
    -282,   850,  -282,  -282,  -282,  -282,  -282,  -282,  -282,  -282,
    -282,  -282,     0,     0,     0,     0,     0,  -282,  -282,     0,
    -282,  -282,   -80,     0,     0,     0,  -282,  -282,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   607,     0,
     611,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     883,     0,   886,     0,     0,     0,  -500,     2,     0,     3,
       4,     5,     6,     7,     0,     0,   611,     8,     9,     0,
       0,   891,    10,     0,    11,    12,    13,    14,    15,    16,
      17,     0,     0,     0,     0,    18,    19,    20,    21,    22,
      23,    24,     0,     0,    25,     0,     0,     0,     0,     0,
      26,    27,    28,    29,    30,    31,    32,    33,    34,    35,
      36,    37,    38,    39,     0,    40,    41,     0,    42,    43,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    44,     0,
       0,    45,    46,     0,    47,    48,     0,    49,     0,    50,
      51,    52,    53,    54,    55,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,  -484,     0,     0,
       0,     0,    56,    57,    58,  -484,  -484,  -484,     0,     0,
       0,  -484,  -484,     0,  -484,     0,  -500,  -500,     0,     0,
     657,     0,     0,  -484,     0,     0,     0,     0,     0,     0,
       0,     0,     0,  -484,  -484,     0,  -484,  -484,  -484,  -484,
    -484,   314,   315,   316,   317,   318,   319,   320,   321,   322,
     323,   324,   325,   326,     0,     0,   327,   328,     0,     0,
       0,  -484,  -484,  -484,  -484,  -484,  -484,  -484,  -484,  -484,
    -484,  -484,  -484,  -484,     0,     0,  -484,  -484,  -484,     0,
     659,   329,     0,   330,   331,   332,   333,   334,   335,   336,
     337,   338,   339,     0,     0,     0,     0,     0,     0,     0,
     -86,  -484,     0,  -484,  -484,  -484,  -484,  -484,  -484,  -484,
    -484,  -484,  -484,     0,     0,     0,     0,  -484,  -484,  -484,
       0,  -484,  -484,   -78,     0,     0,     0,  -484,  -484,     2,
       0,     3,     4,     5,     6,     7,  -500,  -500,  -500,     8,
       9,     0,     0,  -500,    10,     0,    11,    12,    13,    14,
      15,    16,    17,     0,     0,     0,     0,    18,    19,    20,
      21,    22,    23,    24,     0,     0,    25,     0,     0,     0,
       0,     0,    26,    27,    28,    29,    30,    31,    32,    33,
      34,    35,    36,    37,    38,    39,     0,    40,    41,     0,
      42,    43,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      44,     0,     0,    45,    46,     0,    47,    48,     0,    49,
       0,    50,    51,    52,    53,    54,    55,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    56,    57,    58,     0,     0,     0,
       0,     2,     0,     3,     4,     5,     6,     7,  -500,  -500,
    -500,     8,     9,     0,  -500,  -500,    10,     0,    11,    12,
      13,    14,    15,    16,    17,     0,     0,     0,     0,    18,
      19,    20,    21,    22,    23,    24,     0,     0,    25,     0,
       0,     0,     0,     0,    26,    27,    28,    29,    30,    31,
      32,    33,    34,    35,    36,    37,    38,    39,     0,    40,
      41,     0,    42,    43,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    44,     0,     0,    45,    46,     0,    47,    48,
       0,    49,     0,    50,    51,    52,    53,    54,    55,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    56,    57,    58,     0,
       0,     0,     0,     2,     0,     3,     4,     5,     6,     7,
    -500,  -500,  -500,     8,     9,     0,     0,  -500,    10,  -500,
      11,    12,    13,    14,    15,    16,    17,     0,     0,     0,
       0,    18,    19,    20,    21,    22,    23,    24,     0,     0,
      25,     0,     0,     0,     0,     0,    26,    27,    28,    29,
      30,    31,    32,    33,    34,    35,    36,    37,    38,    39,
       0,    40,    41,     0,    42,    43,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    44,     0,     0,    45,    46,     0,
      47,    48,     0,    49,     0,    50,    51,    52,    53,    54,
      55,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    56,    57,
      58,     0,     0,     0,     0,     2,     0,     3,     4,     5,
       6,     7,  -500,  -500,  -500,     8,     9,     0,     0,  -500,
      10,     0,    11,    12,    13,    14,    15,    16,    17,     0,
       0,     0,     0,    18,    19,    20,    21,    22,    23,    24,
       0,     0,    25,     0,     0,     0,     0,     0,    26,    27,
      28,    29,    30,    31,    32,    33,    34,    35,    36,    37,
      38,    39,     0,    40,    41,     0,    42,    43,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    44,     0,     0,    45,
      46,     0,    47,    48,     0,    49,     0,    50,    51,    52,
      53,    54,    55,     0,     0,     0,     0,     0,     2,     0,
       3,     4,     5,     6,     7,     0,  -500,  -500,     8,     9,
      56,    57,    58,    10,     0,    11,    12,    13,    14,    15,
      16,    17,     0,     0,  -500,  -500,    18,    19,    20,    21,
      22,    23,    24,     0,     0,    25,     0,     0,     0,     0,
       0,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,     0,    40,    41,     0,    42,
      43,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    44,
       0,     0,    45,    46,     0,    47,    48,     0,    49,     0,
      50,    51,    52,    53,    54,    55,     0,     0,     0,     0,
       0,     2,     0,     3,     4,     5,     6,     7,     0,     0,
       0,     8,     9,    56,    57,    58,    10,     0,    11,    12,
      13,    14,    15,    16,    17,     0,     0,  -500,  -500,    18,
      19,    20,    21,    22,    23,    24,     0,     0,    25,     0,
       0,     0,     0,     0,    26,    27,    28,    29,    30,    31,
      32,    33,    34,    35,    36,    37,    38,    39,     0,    40,
      41,     0,    42,    43,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    44,     0,     0,   259,    46,     0,    47,    48,
       0,    49,     0,    50,    51,    52,    53,    54,    55,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    56,    57,    58,     0,
       0,     0,     0,     0,     0,  -500,     0,     0,     0,     0,
    -500,  -500,     2,     0,     3,     4,     5,     6,     7,     0,
       0,     0,     8,     9,     0,     0,     0,    10,     0,    11,
      12,    13,    14,    15,    16,    17,     0,     0,     0,     0,
      18,    19,    20,    21,    22,    23,    24,     0,     0,    25,
       0,     0,     0,     0,     0,    26,    27,    28,    29,    30,
      31,    32,    33,    34,    35,    36,    37,    38,    39,     0,
      40,    41,     0,    42,    43,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    44,     0,     0,    45,    46,     0,    47,
      48,     0,    49,     0,    50,    51,    52,    53,    54,    55,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    56,    57,    58,
       0,     0,     0,     0,     0,     0,  -500,     0,     0,     0,
       0,  -500,  -500,     2,     0,     3,     4,     5,     6,     7,
       0,     0,     0,     8,     9,     0,     0,     0,    10,     0,
      11,    12,    13,    14,    15,    16,    17,     0,     0,     0,
       0,    18,    19,    20,    21,    22,    23,    24,     0,     0,
      25,     0,     0,     0,     0,     0,    26,    27,    28,    29,
      30,    31,    32,    33,    34,    35,    36,    37,    38,    39,
       0,    40,    41,     0,    42,    43,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    44,     0,     0,    45,    46,     0,
      47,    48,     0,    49,     0,    50,    51,    52,    53,    54,
      55,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    56,    57,
      58,     0,     0,  -500,     0,     2,     0,     3,     4,     5,
       6,     7,  -500,  -500,  -500,     8,     9,     0,     0,     0,
      10,     0,    11,    12,    13,    14,    15,    16,    17,     0,
       0,     0,     0,    18,    19,    20,    21,    22,    23,    24,
       0,     0,    25,     0,     0,     0,     0,     0,    26,    27,
      28,    29,    30,    31,    32,    33,    34,    35,    36,    37,
      38,    39,     0,    40,    41,     0,    42,    43,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    44,     0,     0,    45,
      46,     0,    47,    48,     0,    49,     0,    50,    51,    52,
      53,    54,    55,     0,     0,     0,     0,     0,     0,     0,
       3,     4,     5,     6,     7,     0,     0,     0,     8,     9,
      56,    57,    58,    10,     0,    11,    12,    13,    14,    15,
      16,    17,     0,     0,  -500,  -500,    18,    19,    20,    21,
      22,    23,    24,     0,     0,    25,     0,     0,     0,     0,
       0,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,     0,    40,    41,     0,    42,
      43,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    44,
       0,     0,    45,    46,     0,    47,    48,     0,    49,     0,
      50,    51,    52,    53,    54,    55,     0,     0,     0,     0,
       0,     0,     0,     3,     4,     5,     0,     7,     0,     0,
       0,     8,     9,    56,    57,    58,    10,     0,    11,    12,
      13,    14,    15,    16,    17,     0,     0,     0,   393,    18,
      19,    20,    21,    22,    23,    24,     0,     0,    25,     0,
       0,     0,     0,     0,     0,    27,     0,     0,    30,    31,
      32,    33,    34,    35,    36,    37,    38,    39,     0,    40,
      41,     0,    42,    43,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   205,     0,     0,   105,    46,     0,    47,    48,
       0,     0,     0,    50,    51,    52,    53,    54,    55,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    56,    57,    58,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     214,   215,   115,   116,   117,   118,   119,   120,   121,   122,
     123,   124,   125,   126,   127,   128,   129,   130,   131,   132,
     133,   134,   135,   136,   137,   138,     0,     0,   139,   140,
     141,   142,   143,   144,   145,   146,   147,   148,     0,     0,
       0,     0,     0,   149,   150,   151,   152,   153,   154,   155,
     156,    34,    35,   157,    37,     0,     0,     0,     0,     0,
       0,   158,   159,   160,   161,   162,   163,     0,   164,   165,
       0,     0,   166,     0,     0,     0,   167,   168,   169,   170,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     171,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   172,   173,   174,   175,   176,
     177,   178,   179,   180,   181,     0,     0,   182,     0,     0,
    -477,  -477,  -477,     0,  -477,     0,   183,   184,  -477,  -477,
       0,     0,     0,  -477,     0,  -477,  -477,  -477,  -477,  -477,
    -477,  -477,     0,  -477,     0,     0,  -477,  -477,  -477,  -477,
    -477,  -477,  -477,     0,     0,     0,     0,     0,     0,     0,
       0,     0,  -477,     0,     0,  -477,  -477,  -477,  -477,  -477,
    -477,  -477,  -477,  -477,  -477,     0,  -477,  -477,     0,  -477,
    -477,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,  -477,
       0,     0,  -477,  -477,     0,  -477,  -477,     0,  -477,  -477,
    -477,  -477,  -477,  -477,  -477,  -477,     0,     0,     0,     0,
       0,     0,     0,  -479,  -479,  -479,     0,  -479,     0,     0,
       0,  -479,  -479,  -477,  -477,  -477,  -479,  -477,  -479,  -479,
    -479,  -479,  -479,  -479,  -479,  -477,  -479,     0,     0,  -479,
    -479,  -479,  -479,  -479,  -479,  -479,     0,     0,     0,     0,
       0,     0,     0,     0,     0,  -479,     0,     0,  -479,  -479,
    -479,  -479,  -479,  -479,  -479,  -479,  -479,  -479,     0,  -479,
    -479,     0,  -479,  -479,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,  -479,     0,     0,  -479,  -479,     0,  -479,  -479,
       0,  -479,  -479,  -479,  -479,  -479,  -479,  -479,  -479,     0,
       0,     0,     0,     0,     0,     0,  -478,  -478,  -478,     0,
    -478,     0,     0,     0,  -478,  -478,  -479,  -479,  -479,  -478,
    -479,  -478,  -478,  -478,  -478,  -478,  -478,  -478,  -479,  -478,
       0,     0,  -478,  -478,  -478,  -478,  -478,  -478,  -478,     0,
       0,     0,     0,     0,     0,     0,     0,     0,  -478,     0,
       0,  -478,  -478,  -478,  -478,  -478,  -478,  -478,  -478,  -478,
    -478,     0,  -478,  -478,     0,  -478,  -478,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,  -478,     0,     0,  -478,  -478,
       0,  -478,  -478,     0,  -478,  -478,  -478,  -478,  -478,  -478,
    -478,  -478,     0,     0,     0,     0,     0,     0,     0,  -480,
    -480,  -480,     0,  -480,     0,     0,     0,  -480,  -480,  -478,
    -478,  -478,  -480,  -478,  -480,  -480,  -480,  -480,  -480,  -480,
    -480,  -478,     0,     0,     0,  -480,  -480,  -480,  -480,  -480,
    -480,  -480,     0,     0,     0,     0,     0,     0,     0,     0,
       0,  -480,     0,     0,  -480,  -480,  -480,  -480,  -480,  -480,
    -480,  -480,  -480,  -480,     0,  -480,  -480,     0,  -480,  -480,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,  -480,   708,
       0,  -480,  -480,     0,  -480,  -480,     0,  -480,  -480,  -480,
    -480,  -480,  -480,  -480,  -480,     0,     0,     0,     0,   -86,
       0,     0,  -481,  -481,  -481,     0,  -481,     0,     0,     0,
    -481,  -481,  -480,  -480,  -480,  -481,     0,  -481,  -481,  -481,
    -481,  -481,  -481,  -481,  -480,     0,     0,     0,  -481,  -481,
    -481,  -481,  -481,  -481,  -481,     0,     0,     0,     0,     0,
       0,     0,     0,     0,  -481,     0,     0,  -481,  -481,  -481,
    -481,  -481,  -481,  -481,  -481,  -481,  -481,     0,  -481,  -481,
       0,  -481,  -481,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,  -481,   660,     0,  -481,  -481,     0,  -481,  -481,     0,
    -481,  -481,  -481,  -481,  -481,  -481,  -481,  -481,     0,     0,
       0,     0,   -88,     0,     0,  -253,  -253,  -253,     0,  -253,
       0,     0,     0,  -253,  -253,  -481,  -481,  -481,  -253,     0,
    -253,  -253,  -253,  -253,  -253,  -253,  -253,  -481,     0,     0,
       0,  -253,  -253,  -253,  -253,  -253,  -253,  -253,     0,     0,
       0,     0,     0,     0,     0,     0,     0,  -253,     0,     0,
    -253,  -253,  -253,  -253,  -253,  -253,  -253,  -253,  -253,  -253,
       0,  -253,  -253,     0,  -253,  -253,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,  -253,     0,     0,  -253,  -253,     0,
    -253,  -253,     0,  -253,  -253,  -253,  -253,  -253,  -253,  -253,
    -253,     0,     0,     0,     0,     0,     0,     0,  -253,  -253,
    -253,     0,  -253,     0,     0,     0,  -253,  -253,  -253,  -253,
    -253,  -253,     0,  -253,  -253,  -253,  -253,  -253,  -253,  -253,
     242,     0,     0,     0,  -253,  -253,  -253,  -253,  -253,  -253,
    -253,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    -253,     0,     0,  -253,  -253,  -253,  -253,  -253,  -253,  -253,
    -253,  -253,  -253,     0,  -253,  -253,     0,  -253,  -253,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,  -253,     0,     0,
    -253,  -253,     0,  -253,  -253,     0,  -253,  -253,  -253,  -253,
    -253,  -253,  -253,  -253,     0,     0,     0,     0,     0,     0,
       0,  -482,  -482,  -482,     0,  -482,     0,     0,     0,  -482,
    -482,  -253,  -253,  -253,  -482,     0,  -482,  -482,  -482,  -482,
    -482,  -482,  -482,   245,     0,     0,     0,  -482,  -482,  -482,
    -482,  -482,  -482,  -482,     0,     0,     0,     0,     0,     0,
       0,     0,     0,  -482,     0,     0,  -482,  -482,  -482,  -482,
    -482,  -482,  -482,  -482,  -482,  -482,     0,  -482,  -482,     0,
    -482,  -482,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    -482,     0,     0,  -482,  -482,     0,  -482,  -482,     0,  -482,
    -482,  -482,  -482,  -482,  -482,  -482,  -482,     0,     0,     0,
       0,     0,     0,     0,  -483,  -483,  -483,     0,  -483,     0,
       0,     0,  -483,  -483,  -482,  -482,  -482,  -483,     0,  -483,
    -483,  -483,  -483,  -483,  -483,  -483,  -482,     0,     0,     0,
    -483,  -483,  -483,  -483,  -483,  -483,  -483,     0,     0,     0,
       0,     0,     0,     0,     0,     0,  -483,     0,     0,  -483,
    -483,  -483,  -483,  -483,  -483,  -483,  -483,  -483,  -483,     0,
    -483,  -483,     0,  -483,  -483,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,  -483,     0,     0,  -483,  -483,     0,  -483,
    -483,     0,  -483,  -483,  -483,  -483,  -483,  -483,  -483,  -483,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,  -483,  -483,  -483,
       0,     0,     0,     0,     0,     0,     0,     0,     0,  -483,
     115,   116,   117,   118,   119,   120,   121,   122,   123,   124,
     125,   126,   127,   128,   129,   130,   131,   132,   133,   134,
     135,   136,   137,   138,     0,     0,   139,   140,   141,   190,
     191,   192,   193,   146,   147,   148,     0,     0,     0,     0,
       0,   149,   150,   151,   152,   194,   195,   196,   156,   279,
     280,   197,   281,     0,     0,     0,     0,     0,     0,   158,
     159,   160,   161,   162,   163,     0,   164,   165,     0,     0,
     166,     0,     0,     0,   167,   168,   169,   170,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   171,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   172,   173,   174,   175,   176,   177,   178,
     179,   180,   181,     0,     0,   182,     0,     0,     0,     0,
       0,     0,     0,     0,   183,   115,   116,   117,   118,   119,
     120,   121,   122,   123,   124,   125,   126,   127,   128,   129,
     130,   131,   132,   133,   134,   135,   136,   137,   138,     0,
       0,   139,   140,   141,   190,   191,   192,   193,   146,   147,
     148,     0,     0,     0,     0,     0,   149,   150,   151,   152,
     194,   195,   196,   156,   249,     0,   197,     0,     0,     0,
       0,     0,     0,     0,   158,   159,   160,   161,   162,   163,
       0,   164,   165,     0,     0,   166,     0,     0,     0,   167,
     168,   169,   170,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   171,     0,    50,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   172,   173,
     174,   175,   176,   177,   178,   179,   180,   181,     0,     0,
     182,     0,     0,     0,     0,     0,     0,     0,     0,   183,
     115,   116,   117,   118,   119,   120,   121,   122,   123,   124,
     125,   126,   127,   128,   129,   130,   131,   132,   133,   134,
     135,   136,   137,   138,     0,     0,   139,   140,   141,   190,
     191,   192,   193,   146,   147,   148,     0,     0,     0,     0,
       0,   149,   150,   151,   152,   194,   195,   196,   156,     0,
       0,   197,     0,     0,     0,     0,     0,     0,     0,   158,
     159,   160,   161,   162,   163,     0,   164,   165,     0,     0,
     166,     0,     0,     0,   167,   168,   169,   170,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   171,     0,
      50,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   172,   173,   174,   175,   176,   177,   178,
     179,   180,   181,     0,     0,   182,     0,     0,     0,     0,
       0,     0,     0,     0,   183,   115,   116,   117,   118,   119,
     120,   121,   122,   123,   124,   125,   126,   127,   128,   129,
     130,   131,   132,   133,   134,   135,   136,   137,   138,     0,
       0,   139,   140,   141,   190,   191,   192,   193,   146,   147,
     148,     0,     0,     0,     0,     0,   149,   150,   151,   152,
     194,   195,   196,   156,     0,     0,   197,     0,     0,     0,
       0,     0,     0,     0,   158,   159,   160,   161,   162,   163,
       0,   164,   165,     0,     0,   166,     0,     0,     0,   167,
     168,   169,   170,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   171,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   172,   173,
     174,   175,   176,   177,   178,   179,   180,   181,     0,     0,
     182,     0,     3,     4,     5,     0,     7,     0,     0,   183,
       8,     9,     0,     0,     0,    10,     0,    11,    12,    13,
      95,    96,    16,    17,     0,     0,     0,     0,    97,    19,
      20,    21,    22,    23,    24,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    27,     0,     0,    30,    31,    32,
      33,    34,    35,    36,    37,    38,    39,     0,    40,    41,
       0,    42,    43,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   205,     0,     0,   105,    46,     0,    47,    48,     0,
     229,   230,    50,    51,    52,    53,    54,    55,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       3,     4,     5,     6,     7,    56,   231,    58,     8,     9,
       0,     0,     0,    10,   409,    11,    12,    13,    14,    15,
      16,    17,     0,     0,     0,     0,    18,    19,    20,    21,
      22,    23,    24,     0,     0,    25,     0,     0,     0,     0,
       0,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,     0,    40,    41,     0,    42,
      43,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    44,
       0,     0,    45,    46,     0,    47,    48,     0,    49,     0,
      50,    51,    52,    53,    54,    55,     0,     0,     0,     0,
       0,     0,     0,     3,     4,     5,     0,     7,     0,     0,
       0,     8,     9,    56,    57,    58,    10,     0,    11,    12,
      13,    14,    15,    16,    17,     0,     0,     0,     0,    18,
      19,    20,    21,    22,    23,    24,     0,     0,    25,     0,
       0,     0,     0,     0,     0,    27,     0,     0,    30,    31,
      32,    33,    34,    35,    36,    37,    38,    39,     0,    40,
      41,     0,    42,    43,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   205,     0,     0,   105,    46,     0,    47,    48,
       0,   614,   230,    50,    51,    52,    53,    54,    55,     0,
       0,     0,     0,     0,     0,     0,     3,     4,     5,     0,
       7,     0,     0,     0,     8,     9,    56,    57,    58,    10,
       0,    11,    12,    13,    95,    96,    16,    17,     0,     0,
       0,     0,    97,    19,    20,    21,    22,    23,    24,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    27,     0,
       0,    30,    31,    32,    33,    34,    35,    36,    37,    38,
      39,     0,    40,    41,     0,    42,    43,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   205,     0,     0,   105,    46,
       0,    47,    48,     0,   229,   230,    50,    51,    52,    53,
      54,    55,     0,     0,     0,     0,     0,     0,     0,     3,
       4,     5,     0,     7,     0,     0,     0,     8,     9,    56,
     231,    58,    10,     0,    11,    12,    13,    95,    96,    16,
      17,     0,     0,     0,     0,    97,    19,    20,    21,    22,
      23,    24,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    27,     0,     0,    30,    31,    32,    33,    34,    35,
      36,    37,    38,    39,     0,    40,    41,     0,    42,    43,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   205,     0,
       0,   105,   411,     0,    47,    48,     0,   229,   230,    50,
      51,    52,    53,    54,    55,     0,     0,     0,     0,     0,
       0,     0,     3,     4,     5,     0,     7,     0,     0,     0,
       8,     9,    56,   231,    58,    10,     0,    11,    12,    13,
      95,    96,    16,    17,     0,     0,     0,     0,    97,    98,
      99,    21,    22,    23,    24,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    27,     0,     0,    30,    31,    32,
      33,    34,    35,    36,    37,    38,    39,     0,    40,    41,
       0,    42,    43,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   205,     0,     0,   105,    46,     0,    47,    48,     0,
     606,   230,    50,    51,    52,    53,    54,    55,     0,     0,
       0,     0,     0,     0,     0,     3,     4,     5,     0,     7,
       0,     0,     0,     8,     9,    56,   231,    58,    10,     0,
      11,    12,    13,    95,    96,    16,    17,     0,     0,     0,
       0,    97,    98,    99,    21,    22,    23,    24,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    27,     0,     0,
      30,    31,    32,    33,    34,    35,    36,    37,    38,    39,
       0,    40,    41,     0,    42,    43,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   205,     0,     0,   105,    46,     0,
      47,    48,     0,   610,   230,    50,    51,    52,    53,    54,
      55,     0,     0,     0,     0,     0,     0,     0,     3,     4,
       5,     0,     7,     0,     0,     0,     8,     9,    56,   231,
      58,    10,     0,    11,    12,    13,    95,    96,    16,    17,
       0,     0,     0,     0,    97,    19,    20,    21,    22,    23,
      24,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      27,     0,     0,    30,    31,    32,    33,    34,    35,    36,
      37,    38,    39,     0,    40,    41,     0,    42,    43,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   205,     0,     0,
     105,    46,     0,    47,    48,     0,   606,   230,    50,    51,
      52,    53,    54,    55,     0,     0,     0,     0,     0,     0,
       0,     3,     4,     5,     0,     7,     0,     0,     0,     8,
       9,    56,   231,    58,    10,     0,    11,    12,    13,    95,
      96,    16,    17,     0,     0,     0,     0,    97,    98,    99,
      21,    22,    23,    24,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    27,     0,     0,    30,    31,    32,    33,
      34,    35,    36,    37,    38,    39,     0,    40,    41,     0,
      42,    43,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     205,     0,     0,   105,    46,     0,    47,    48,     0,   801,
     230,    50,    51,    52,    53,    54,    55,     0,     0,     0,
       0,     0,     0,     0,     3,     4,     5,     0,     7,     0,
       0,     0,     8,     9,    56,   231,    58,    10,     0,    11,
      12,    13,    95,    96,    16,    17,     0,     0,     0,     0,
      97,    98,    99,    21,    22,    23,    24,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    27,     0,     0,    30,
      31,    32,    33,    34,    35,    36,    37,    38,    39,     0,
      40,    41,     0,    42,    43,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   205,     0,     0,   105,    46,     0,    47,
      48,     0,   804,   230,    50,    51,    52,    53,    54,    55,
       0,     0,     0,     0,     0,     0,     0,     3,     4,     5,
       0,     7,     0,     0,     0,     8,     9,    56,   231,    58,
      10,     0,    11,    12,    13,    95,    96,    16,    17,     0,
       0,     0,     0,    97,    98,    99,    21,    22,    23,    24,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    27,
       0,     0,    30,    31,    32,    33,    34,    35,    36,    37,
      38,    39,     0,    40,    41,     0,    42,    43,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   205,     0,     0,   105,
      46,     0,    47,    48,     0,   809,   230,    50,    51,    52,
      53,    54,    55,     0,     0,     0,     0,     0,     0,     0,
       3,     4,     5,     0,     7,     0,     0,     0,     8,     9,
      56,   231,    58,    10,     0,    11,    12,    13,    95,    96,
      16,    17,     0,     0,     0,     0,    97,    98,    99,    21,
      22,    23,    24,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    27,     0,     0,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,     0,    40,    41,     0,    42,
      43,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   205,
       0,     0,   105,    46,     0,    47,    48,     0,   868,   230,
      50,    51,    52,    53,    54,    55,     0,     0,     0,     0,
       0,     0,     0,     3,     4,     5,     0,     7,     0,     0,
       0,     8,     9,    56,   231,    58,    10,     0,    11,    12,
      13,    95,    96,    16,    17,     0,     0,     0,     0,    97,
      98,    99,    21,    22,    23,    24,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    27,     0,     0,    30,    31,
      32,    33,    34,    35,    36,    37,    38,    39,     0,    40,
      41,     0,    42,    43,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   205,     0,     0,   105,    46,     0,    47,    48,
       0,   870,   230,    50,    51,    52,    53,    54,    55,     0,
       0,     0,     0,     0,     0,     0,     3,     4,     5,     0,
       7,     0,     0,     0,     8,     9,    56,   231,    58,    10,
       0,    11,    12,    13,    95,    96,    16,    17,     0,     0,
       0,     0,    97,    98,    99,    21,    22,    23,    24,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    27,     0,
       0,    30,    31,    32,    33,    34,    35,    36,    37,    38,
      39,     0,    40,    41,     0,    42,    43,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   205,     0,     0,   105,    46,
       0,    47,    48,     0,   889,   230,    50,    51,    52,    53,
      54,    55,     0,     0,     0,     0,     0,     0,     0,     3,
       4,     5,     0,     7,     0,     0,     0,     8,     9,    56,
     231,    58,    10,     0,    11,    12,    13,    14,    15,    16,
      17,     0,     0,     0,     0,    18,    19,    20,    21,    22,
      23,    24,     0,     0,    25,     0,     0,     0,     0,     0,
       0,    27,     0,     0,    30,    31,    32,    33,    34,    35,
      36,    37,    38,    39,     0,    40,    41,     0,    42,    43,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   205,     0,
       0,   105,    46,     0,    47,    48,     0,     0,     0,    50,
      51,    52,    53,    54,    55,     0,     0,     0,     0,     0,
       0,     0,     3,     4,     5,     0,     7,     0,     0,     0,
       8,     9,    56,    57,    58,    10,     0,    11,    12,    13,
      95,    96,    16,    17,     0,     0,     0,     0,    97,    19,
      20,    21,    22,    23,    24,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    27,     0,     0,    30,    31,    32,
      33,    34,    35,    36,    37,    38,    39,     0,    40,    41,
       0,    42,    43,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   205,     0,     0,   105,    46,     0,    47,    48,     0,
     264,     0,    50,    51,    52,    53,    54,    55,     0,     0,
       0,     0,     0,     0,     0,     3,     4,     5,     0,     7,
       0,     0,     0,     8,     9,    56,   231,    58,    10,     0,
      11,    12,    13,    14,    15,    16,    17,     0,     0,     0,
       0,    18,    19,    20,    21,    22,    23,    24,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    27,     0,     0,
      30,    31,    32,    33,    34,    35,    36,    37,    38,    39,
       0,    40,    41,     0,    42,    43,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   205,     0,     0,   105,    46,     0,
      47,    48,     0,   477,     0,    50,    51,    52,    53,    54,
      55,     0,     0,     0,     0,     0,     0,     0,     3,     4,
       5,     0,     7,     0,     0,     0,     8,     9,    56,   231,
      58,    10,     0,    11,    12,    13,    95,    96,    16,    17,
       0,     0,     0,     0,    97,    98,    99,    21,    22,    23,
      24,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      27,     0,     0,    30,    31,    32,    33,    34,    35,    36,
      37,    38,    39,     0,    40,    41,     0,    42,    43,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   205,     0,     0,
     105,    46,     0,    47,    48,     0,   588,     0,    50,    51,
      52,    53,    54,    55,     0,     0,     0,     0,     0,     0,
       0,     3,     4,     5,     0,     7,     0,     0,     0,     8,
       9,    56,   231,    58,    10,     0,    11,    12,    13,    95,
      96,    16,    17,     0,     0,     0,     0,    97,    98,    99,
      21,    22,    23,    24,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    27,     0,     0,    30,    31,    32,    33,
      34,    35,    36,    37,    38,    39,     0,    40,    41,     0,
      42,    43,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     205,     0,     0,   105,    46,     0,    47,    48,     0,   636,
       0,    50,    51,    52,    53,    54,    55,     0,     0,     0,
       0,     0,     0,     0,     3,     4,     5,     0,     7,     0,
       0,     0,     8,     9,    56,   231,    58,    10,     0,    11,
      12,    13,    95,    96,    16,    17,     0,     0,     0,     0,
      97,    98,    99,    21,    22,    23,    24,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    27,     0,     0,    30,
      31,    32,    33,    34,    35,    36,    37,    38,    39,     0,
      40,    41,     0,    42,    43,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   205,     0,     0,   105,    46,     0,    47,
      48,     0,   477,     0,    50,    51,    52,    53,    54,    55,
       0,     0,     0,     0,     0,     0,     0,     3,     4,     5,
       0,     7,     0,     0,     0,     8,     9,    56,   231,    58,
      10,     0,    11,    12,    13,    95,    96,    16,    17,     0,
       0,     0,     0,    97,    98,    99,    21,    22,    23,    24,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    27,
       0,     0,    30,    31,    32,    33,    34,    35,    36,    37,
      38,    39,     0,    40,    41,     0,    42,    43,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   205,     0,     0,   105,
      46,     0,    47,    48,     0,   752,     0,    50,    51,    52,
      53,    54,    55,     0,     0,     0,     0,     0,     0,     0,
       3,     4,     5,     0,     7,     0,     0,     0,     8,     9,
      56,   231,    58,    10,     0,    11,    12,    13,    95,    96,
      16,    17,     0,     0,     0,     0,    97,    98,    99,    21,
      22,    23,    24,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    27,     0,     0,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,     0,    40,    41,     0,    42,
      43,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   205,
       0,     0,   105,    46,     0,    47,    48,     0,   795,     0,
      50,    51,    52,    53,    54,    55,     0,     0,     0,     0,
       0,     0,     0,     3,     4,     5,     0,     7,     0,     0,
       0,     8,     9,    56,   231,    58,    10,     0,    11,    12,
      13,    95,    96,    16,    17,     0,     0,     0,     0,    97,
      98,    99,    21,    22,    23,    24,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    27,     0,     0,    30,    31,
      32,    33,    34,    35,    36,    37,    38,    39,     0,    40,
      41,     0,    42,    43,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   205,     0,     0,   105,    46,     0,    47,    48,
       0,     0,     0,    50,    51,    52,    53,    54,    55,     0,
       0,     0,     0,     0,     0,     0,     3,     4,     5,     0,
       7,     0,     0,     0,     8,     9,    56,   231,    58,    10,
       0,    11,    12,    13,    14,    15,    16,    17,     0,     0,
       0,     0,    18,    19,    20,    21,    22,    23,    24,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    27,     0,
       0,    30,    31,    32,    33,    34,    35,    36,    37,    38,
      39,     0,    40,    41,     0,    42,    43,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   205,     0,     0,   105,    46,
       0,    47,    48,     0,     0,     0,    50,    51,    52,    53,
      54,    55,     0,     0,     0,     0,     0,     0,     0,     3,
       4,     5,     0,     7,     0,     0,     0,     8,     9,    56,
     231,    58,    10,     0,    11,    12,    13,    95,    96,    16,
      17,     0,     0,     0,     0,    97,    98,    99,    21,    22,
      23,    24,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   100,     0,     0,    30,    31,    32,    33,    34,    35,
      36,    37,    38,    39,     0,    40,    41,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   222,     0,
       0,    45,    46,     0,    47,    48,     0,    49,     0,    50,
      51,    52,    53,    54,    55,     0,     0,     0,     3,     4,
       5,     0,     7,     0,   749,     0,     8,     9,     0,     0,
       0,    10,   106,    11,    12,    13,    95,    96,    16,    17,
       0,     0,     0,     0,    97,    98,    99,    21,    22,    23,
      24,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     100,     0,     0,    30,    31,   101,    33,    34,    35,   102,
      37,    38,    39,     0,    40,    41,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   103,     0,     0,   104,     0,     0,
     105,    46,     0,    47,    48,     0,     0,     0,    50,    51,
      52,    53,    54,    55,     0,     0,     0,     3,     4,     5,
       0,     7,     0,     0,     0,     8,     9,     0,     0,     0,
      10,   106,    11,    12,    13,    95,    96,    16,    17,     0,
       0,     0,     0,    97,    98,    99,    21,    22,    23,    24,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   100,
       0,     0,    30,    31,    32,    33,    34,    35,    36,    37,
      38,    39,     0,    40,    41,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   222,     0,     0,    45,
      46,     0,    47,    48,     0,    49,     0,    50,    51,    52,
      53,    54,    55,     0,     0,     0,     3,     4,     5,     0,
       7,     0,     0,     0,     8,     9,     0,     0,     0,    10,
     106,    11,    12,    13,    95,    96,    16,    17,     0,     0,
       0,     0,    97,    98,    99,    21,    22,    23,    24,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   100,     0,
       0,    30,    31,    32,    33,    34,    35,    36,    37,    38,
      39,     0,    40,    41,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   274,     0,     0,   310,    46,
       0,    47,    48,     0,   311,     0,    50,    51,    52,    53,
      54,    55,     0,     0,     0,     3,     4,     5,     0,     7,
       0,     0,     0,     8,     9,     0,     0,     0,    10,   106,
      11,    12,    13,    95,    96,    16,    17,     0,     0,     0,
       0,    97,    98,    99,    21,    22,    23,    24,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   100,     0,     0,
      30,    31,   101,    33,    34,    35,   102,    37,    38,    39,
       0,    40,    41,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   104,     0,     0,   105,    46,     0,
      47,    48,     0,     0,     0,    50,    51,    52,    53,    54,
      55,     0,     0,     0,     3,     4,     5,     0,     7,     0,
       0,     0,     8,     9,     0,     0,     0,    10,   106,    11,
      12,    13,    95,    96,    16,    17,     0,     0,     0,     0,
      97,    98,    99,    21,    22,    23,    24,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   100,     0,     0,    30,
      31,    32,    33,    34,    35,    36,    37,    38,    39,     0,
      40,    41,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   274,     0,     0,   105,    46,     0,    47,
      48,     0,     0,     0,    50,    51,    52,    53,    54,    55,
       0,     0,     0,     3,     4,     5,     0,     7,     0,     0,
       0,     8,     9,     0,     0,     0,    10,   106,    11,    12,
      13,    95,    96,    16,    17,     0,     0,     0,     0,    97,
      98,    99,    21,    22,    23,    24,     0,     0,     0,     0,
     657,     0,     0,     0,     0,   100,     0,     0,    30,    31,
      32,    33,    34,    35,    36,    37,    38,    39,     0,    40,
      41,   314,   315,   316,   317,   318,   319,   320,   321,   322,
     323,   324,   325,   326,     0,     0,   327,   328,     0,     0,
       0,     0,   829,     0,     0,   105,    46,     0,    47,    48,
       0,     0,     0,    50,    51,    52,    53,    54,    55,     0,
       0,   329,     0,   330,   331,   332,   333,   334,   335,   336,
     337,   338,   339,   515,   516,     0,   106,   517,     0,     0,
       0,     0,     0,  -219,     0,   158,   159,   160,   161,   162,
     163,     0,   164,   165,     0,     0,   166,     0,     0,     0,
     167,   168,   169,   170,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   171,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   172,
     173,   174,   175,   176,   177,   178,   179,   180,   181,   522,
     523,   182,     0,   524,     0,     0,     0,     0,     0,     0,
     183,   158,   159,   160,   161,   162,   163,     0,   164,   165,
       0,     0,   166,     0,     0,     0,   167,   168,   169,   170,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     171,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   172,   173,   174,   175,   176,
     177,   178,   179,   180,   181,   527,   523,   182,     0,   528,
       0,     0,     0,     0,     0,     0,   183,   158,   159,   160,
     161,   162,   163,     0,   164,   165,     0,     0,   166,     0,
       0,     0,   167,   168,   169,   170,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   171,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   172,   173,   174,   175,   176,   177,   178,   179,   180,
     181,   543,   516,   182,     0,   544,     0,     0,     0,     0,
       0,     0,   183,   158,   159,   160,   161,   162,   163,     0,
     164,   165,     0,     0,   166,     0,     0,     0,   167,   168,
     169,   170,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   171,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   172,   173,   174,
     175,   176,   177,   178,   179,   180,   181,   580,   516,   182,
       0,   581,     0,     0,     0,     0,     0,     0,   183,   158,
     159,   160,   161,   162,   163,     0,   164,   165,     0,     0,
     166,     0,     0,     0,   167,   168,   169,   170,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   171,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   172,   173,   174,   175,   176,   177,   178,
     179,   180,   181,   583,   523,   182,     0,   584,     0,     0,
       0,     0,     0,     0,   183,   158,   159,   160,   161,   162,
     163,     0,   164,   165,     0,     0,   166,     0,     0,     0,
     167,   168,   169,   170,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   171,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   172,
     173,   174,   175,   176,   177,   178,   179,   180,   181,   597,
     516,   182,     0,   598,     0,     0,     0,     0,     0,     0,
     183,   158,   159,   160,   161,   162,   163,     0,   164,   165,
       0,     0,   166,     0,     0,     0,   167,   168,   169,   170,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     171,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   172,   173,   174,   175,   176,
     177,   178,   179,   180,   181,   600,   523,   182,     0,   601,
       0,     0,     0,     0,     0,     0,   183,   158,   159,   160,
     161,   162,   163,     0,   164,   165,     0,     0,   166,     0,
       0,     0,   167,   168,   169,   170,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   171,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   172,   173,   174,   175,   176,   177,   178,   179,   180,
     181,   638,   516,   182,     0,   639,     0,     0,     0,     0,
       0,     0,   183,   158,   159,   160,   161,   162,   163,     0,
     164,   165,     0,     0,   166,     0,     0,     0,   167,   168,
     169,   170,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   171,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   172,   173,   174,
     175,   176,   177,   178,   179,   180,   181,   641,   523,   182,
       0,   642,     0,     0,     0,     0,     0,     0,   183,   158,
     159,   160,   161,   162,   163,     0,   164,   165,     0,     0,
     166,     0,     0,     0,   167,   168,   169,   170,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   171,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   172,   173,   174,   175,   176,   177,   178,
     179,   180,   181,   875,   516,   182,     0,   876,     0,     0,
       0,     0,     0,     0,   183,   158,   159,   160,   161,   162,
     163,     0,   164,   165,     0,     0,   166,     0,     0,     0,
     167,   168,   169,   170,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   171,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   172,
     173,   174,   175,   176,   177,   178,   179,   180,   181,   878,
     523,   182,     0,   879,     0,     0,     0,     0,     0,     0,
     183,   158,   159,   160,   161,   162,   163,     0,   164,   165,
       0,     0,   166,     0,     0,     0,   167,   168,   169,   170,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     171,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   172,   173,   174,   175,   176,
     177,   178,   179,   180,   181,     0,     0,   182,     0,     0,
       0,     0,     0,     0,     0,     0,   183,   314,   315,   316,
     317,   318,   319,   320,   321,   322,   323,   324,   325,   326,
       0,     0,   327,   328,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   329,     0,   330,
     331,   332,   333,   334,   335,   336,   337,   338,   339,   314,
     315,   316,   317,   318,   319,   320,   321,   322,   323,   324,
     325,   326,     0,   251,   327,   328,     0,     0,     0,  -219,
     314,   315,   316,   317,   318,   319,   320,   321,   322,   323,
     324,   325,   326,     0,     0,   327,   328,     0,     0,   329,
       0,   330,   331,   332,   333,   334,   335,   336,   337,   338,
     339,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     329,  -219,   330,   331,   332,   333,   334,   335,   336,   337,
     338,   339,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   603,   314,   315,   316,   317,   318,   319,   320,
     321,   322,   323,   324,   325,   326,     0,     0,   327,   328,
     314,   315,   316,   317,   318,   319,   320,   321,   322,   323,
     324,   325,   326,     0,     0,   327,   328,     0,     0,     0,
       0,     0,     0,   329,   658,   330,   331,   332,   333,   334,
     335,   336,   337,   338,   339,     0,     0,     0,     0,     0,
     329,     0,   330,   331,   332,   333,   334,   335,   336,   337,
     338,   339,   314,   315,   316,   317,   318,   319,   320,   321,
     322,   323,   324,-32768,-32768,     0,     0,   327,   328,   314,
     315,   316,   317,   318,   319,   320,   321,     0,   323,   324,
       0,     0,     0,     0,   327,   328,     0,     0,     0,     0,
       0,     0,     0,     0,   330,   331,   332,   333,   334,   335,
     336,   337,   338,   339,     0,     0,     0,     0,     0,     0,
       0,   330,   331,   332,   333,   334,   335,   336,   337,   338,
     339,   314,   315,   316,   317,   318,   319,   320,     0,     0,
     323,   324,     0,     0,     0,     0,   327,   328,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   330,   331,   332,   333,   334,   335,   336,
     337,   338,   339
};

static const short yycheck[] =
{
      14,    15,     5,     6,    18,    26,   389,     5,    20,    14,
      15,    13,    49,    18,    20,    14,    15,     6,   245,    18,
      10,    14,    15,    26,   395,    18,     3,     4,    42,    43,
     370,    60,   341,    47,    48,   213,    13,    26,     6,   690,
      57,    27,     2,    57,    58,     1,    26,    50,    47,   100,
      14,    15,    50,    81,    18,    48,    15,   366,    26,    18,
      25,   368,    13,   575,     8,     9,    10,   313,   104,   259,
      60,    26,    49,   585,    45,   384,   531,   532,    36,    37,
      92,    25,   687,    25,    48,   690,    92,    99,   130,   398,
     285,    13,    69,    13,   289,   120,   124,    54,    25,    81,
      72,   113,    46,    81,   340,    92,   342,   343,   344,    83,
      15,    58,    17,    83,    83,    36,    37,    83,   108,   105,
     310,    83,   407,   110,   529,   530,   662,    28,    81,   103,
      13,   367,   668,   103,   103,    81,   108,   103,   103,   448,
     120,   103,   124,   108,   130,   131,   124,   383,   128,   385,
      83,   107,    99,   100,   105,   440,   126,   126,    25,   103,
     125,   126,   129,   399,   126,   120,   108,     9,    10,    26,
     103,   124,   130,   824,   130,   131,   103,   128,   124,   130,
     131,   108,   129,   105,   126,   105,   128,   216,     1,     2,
       3,     4,   428,   126,   129,     8,     9,    10,   125,   126,
      13,    14,    15,    83,   125,    18,   103,    83,   130,   131,
     130,   131,    25,   449,   265,   229,   230,   231,    84,   824,
     560,   676,   105,   103,    81,   103,   216,   103,   242,    81,
     244,   245,    45,    46,    47,   542,    49,   242,    50,   244,
     245,   108,    54,   242,    57,   244,   245,   130,   131,   242,
     264,   244,   245,   566,   561,   568,    69,   126,    25,   126,
     126,   128,    25,   120,    25,   122,   461,   124,    25,    83,
     122,   128,   124,    83,   311,   126,   220,   221,   242,   130,
     244,   245,    83,   242,   796,   244,   245,    94,   259,   103,
     103,   308,   105,   103,   308,   128,   313,    81,   128,   313,
     314,   315,   316,   317,   318,   319,   320,   321,   322,   323,
     324,   325,   326,   327,   328,   329,   330,   331,   332,   333,
     334,   335,   336,   337,   338,   339,   126,   341,   574,   365,
     735,   736,   349,   350,   311,   349,   350,   340,   298,   310,
     124,   108,   341,   303,   308,   108,   130,   108,    54,   313,
     721,   108,   366,   103,    25,   299,   300,   301,   302,   126,
     304,   305,   365,   126,   415,   126,   417,   366,   382,   126,
     384,   361,   386,   387,    81,    56,    57,   391,   220,   221,
     383,    26,   433,   434,   398,   384,    83,   623,   624,   403,
     404,   204,   131,   407,   397,   622,    83,   411,   268,   398,
     270,   422,   272,    54,   394,   395,   411,   220,   221,    25,
     424,   404,   590,   796,    84,   429,   103,   124,   411,   422,
     128,    25,    64,   437,   427,   369,   440,   391,   442,   242,
     126,   244,   245,   422,   448,    81,    81,   108,   723,   126,
      52,   254,   126,   783,   447,   730,   259,    59,    60,   448,
      81,   120,   466,   467,   422,   126,    64,   299,   300,   301,
     302,   125,   769,   477,   777,   778,    15,   411,    17,   782,
      50,   113,   114,   115,    83,   120,   122,   122,   124,   124,
     130,   131,   128,   128,    26,   298,   299,   300,   301,   302,
     303,   304,   305,   124,   103,   308,   125,   310,   311,   550,
     313,     1,     2,   238,   108,    81,   125,   519,     8,     9,
      10,    91,    92,   519,    14,    15,    81,   126,    18,    81,
     526,   828,   126,   529,   530,    25,   123,    58,   341,   121,
     110,   126,    54,   113,   546,   130,   349,   350,    81,    81,
      64,    36,    37,   101,   857,    45,    46,    47,   124,   539,
     101,   572,   861,   366,   126,   368,   369,    57,   130,   124,
     574,    64,   124,   848,    56,    57,    64,   380,   571,   572,
     101,   384,   562,   571,   588,   388,   126,   390,   120,   122,
     122,   124,   124,   572,    81,   398,   128,   111,   112,   113,
     114,   115,   606,   125,   126,    58,   610,   541,   411,   884,
     614,    14,    15,   103,   572,   105,    90,   785,   622,    10,
     574,    58,   425,   791,     8,    50,    13,    52,    53,    54,
      55,    58,   636,   622,    81,   122,   862,   124,    17,   622,
      58,    10,   126,   684,   125,   448,    99,   100,   101,   126,
     125,   655,   659,   657,   658,   659,   660,   664,   665,   651,
     664,   665,    99,   100,   101,   121,    91,    92,   602,   126,
      81,   125,    99,   100,   101,   122,   401,   124,   129,   683,
     126,    99,   100,   101,   651,   110,   126,    50,   113,    52,
      53,    54,    55,   734,   628,   125,    10,   123,   678,   123,
     741,    52,    53,   128,   708,   103,   710,   711,    81,   541,
     126,   122,   716,   124,   204,    50,    81,   126,   126,   723,
      81,    50,    15,   727,    81,    81,   730,    64,    91,    92,
     220,   221,   727,   123,    10,    47,    48,    10,   541,   542,
      81,   721,    79,    80,   727,   126,   123,   110,   752,   122,
     113,   124,   242,   760,   244,   245,   760,   122,   561,   124,
      92,   122,   126,   124,   254,   122,   122,   124,   124,   259,
     602,   705,   125,   727,   111,   112,   113,   114,   115,   126,
     125,   122,   125,   124,   123,    81,    64,    52,    53,   793,
      55,   795,   125,   121,    59,    60,    10,   801,   125,   602,
     804,    79,    80,    10,    58,   809,     9,    61,   298,   299,
     300,   301,   302,   303,   304,   305,    84,   784,   308,   622,
     310,    10,    10,   313,   125,   628,   122,   121,   124,   108,
     108,   109,   110,   111,   112,   113,   114,   115,   126,   125,
     643,   126,   846,    10,   848,    99,   100,    10,   651,   126,
     125,   341,   126,    10,    10,   121,   659,   861,    10,   349,
     350,   664,   665,   846,   868,   121,   870,   860,    54,    10,
      10,   126,   861,   705,   123,     0,   366,     0,   368,   369,
     884,   651,    69,     4,   609,   889,   864,    79,   863,     5,
     380,    76,   560,   783,   384,   620,    60,    -1,   388,   702,
     390,     1,   705,    64,   687,   687,    -1,    -1,   398,    39,
      40,    41,    42,    43,   717,   718,   719,    -1,    79,    80,
      -1,   411,    -1,    -1,    -1,   237,   238,    -1,     1,     2,
       3,     4,     5,   245,    -1,   425,    -1,    -1,    -1,    -1,
      13,    -1,    -1,    -1,    -1,    45,    -1,    -1,   751,   110,
     111,   112,   113,   114,   115,    -1,    -1,   760,   448,    -1,
      -1,    -1,    -1,   766,   767,    -1,   769,    -1,    -1,    -1,
      -1,    -1,    45,    -1,    -1,    -1,    49,    -1,    -1,    -1,
      -1,   784,    -1,    -1,    -1,   788,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    69,   722,    -1,    -1,
     725,   726,    -1,    -1,    -1,   105,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   821,    -1,
      -1,    -1,    -1,    -1,    -1,   828,    -1,    -1,    -1,   341,
      -1,   834,   105,   836,    -1,    -1,    -1,    -1,    -1,    -1,
     843,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   541,   542,    -1,   366,    -1,    -1,    -1,   861,    -1,
      -1,    -1,    -1,    -1,   376,    -1,     1,     2,     3,     4,
      -1,   561,   384,    -1,    -1,    -1,    -1,    -1,    13,    -1,
      -1,   806,   807,    -1,    -1,    -1,   398,    -1,    -1,   401,
      -1,    -1,    -1,    -1,    -1,    -1,     1,     2,    -1,    -1,
      -1,    -1,    -1,    -1,   416,    -1,    -1,    -1,    13,    -1,
      45,    -1,   602,    -1,    49,    -1,    -1,    -1,    -1,   844,
     845,    -1,    -1,    -1,    -1,   850,    -1,    -1,    -1,    -1,
      -1,   204,   622,    -1,    69,    -1,   448,    -1,   628,    -1,
      45,    -1,    -1,    -1,   869,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   643,   254,    -1,    -1,    -1,   883,   259,
      -1,   886,    -1,    -1,   476,    -1,   891,    -1,    -1,   659,
     105,    -1,    -1,    -1,   664,   665,    -1,    -1,    -1,    -1,
      -1,   254,    -1,    -1,    -1,    -1,   259,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,     1,     2,    -1,    -1,
     105,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     310,    -1,   702,    -1,   526,   705,    -1,   529,   530,   531,
     532,    -1,    -1,    -1,    -1,   298,    -1,   717,   718,   719,
     303,    -1,    -1,    -1,    -1,    -1,   548,   310,   311,    -1,
      45,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   564,    -1,   566,    -1,   568,    -1,    -1,    -1,
      -1,   751,   574,   575,    -1,   577,    -1,    -1,    -1,   204,
     760,    -1,    -1,   585,    -1,    -1,   766,   767,    -1,   769,
     380,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   388,    -1,
     390,    -1,    -1,    -1,    -1,   368,    -1,   609,   788,   204,
     105,    -1,    -1,    -1,    -1,    -1,    -1,   380,   620,    -1,
      -1,    -1,    -1,    -1,    -1,   388,    -1,   390,    -1,   254,
      -1,    -1,    -1,    -1,   259,   425,    -1,    -1,    -1,    -1,
      -1,   821,    -1,    -1,    -1,    -1,    -1,    -1,   828,    -1,
      -1,    -1,    -1,    -1,   834,    -1,   836,    -1,    -1,   254,
      -1,    -1,   425,   843,   259,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   298,   676,    -1,    -1,    -1,   303,    -1,
      -1,   861,    -1,    -1,    -1,   310,   311,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   700,    -1,
      -1,   703,    -1,   298,    -1,    -1,    -1,    -1,   303,   204,
      -1,    -1,    -1,    -1,    -1,   310,    -1,    -1,    -1,    -1,
     722,    -1,    -1,   725,   726,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   735,   736,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   368,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   380,    -1,    -1,    -1,   254,
      -1,    -1,    -1,   388,   259,   390,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   368,    -1,   777,   778,    -1,    -1,   542,
     782,    -1,    -1,    -1,    -1,   380,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   388,   796,   390,    -1,    -1,   561,    -1,
     425,    -1,    -1,   298,   806,   807,    -1,    -1,   303,    -1,
      -1,    -1,    -1,    -1,    -1,   310,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     425,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   844,   845,    -1,    -1,    -1,    -1,   850,    -1,
      -1,    -1,    -1,   643,    -1,   857,    -1,    -1,    -1,   861,
      -1,   863,   864,    -1,    -1,    -1,    -1,   869,    -1,    -1,
      -1,    -1,    -1,   368,    -1,    -1,    -1,    -1,    -1,    -1,
     643,   883,    -1,    -1,   886,   380,    -1,    -1,   651,   891,
      -1,    -1,    -1,   388,    -1,   390,    -1,    -1,    -1,    -1,
      -1,   229,   230,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   702,    -1,    -1,    -1,    -1,   542,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   717,   718,   719,
     425,    -1,    -1,    -1,    -1,    -1,   561,    -1,    -1,   702,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   542,    -1,    -1,
      -1,    -1,    -1,    -1,   717,   718,   719,    -1,    -1,    -1,
      -1,   751,    -1,    -1,    -1,    -1,   561,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   766,   767,    -1,    -1,
     308,    -1,    -1,    -1,    -1,   313,    -1,    -1,   751,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   788,    -1,
      -1,    -1,    -1,   766,   767,    -1,   769,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   643,   644,
      -1,   784,    -1,    -1,    -1,   788,   651,    -1,    -1,    -1,
      -1,   821,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   834,    -1,   836,   542,   643,    -1,
      -1,    -1,    -1,   843,    -1,    -1,   651,    -1,   821,    -1,
      -1,    -1,    -1,   391,    -1,   828,   561,    -1,    -1,    -1,
      -1,   834,    -1,   836,    -1,   403,   404,   702,    -1,   407,
     843,    -1,    -1,   411,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   717,   718,   719,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   702,    -1,   437,
      -1,    -1,   440,    -1,   442,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   717,   718,   719,    -1,   751,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   766,   767,    -1,   769,    -1,    -1,    -1,   643,   477,
      -1,    -1,    -1,    -1,    -1,    -1,   751,    -1,    -1,   784,
      -1,    -1,    -1,   788,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   766,   767,    -1,   769,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   784,
      -1,    -1,    -1,   788,    -1,    -1,   821,    -1,    -1,    -1,
      -1,    -1,    -1,   828,    -1,    -1,    -1,   702,    -1,   834,
      -1,   836,    -1,    -1,    -1,    -1,    -1,    -1,   843,    -1,
      -1,    -1,   717,   718,   719,    -1,   821,    -1,    -1,    -1,
      -1,    -1,    -1,   828,    -1,    -1,    -1,    -1,    -1,   834,
      -1,   836,    -1,    -1,    -1,    -1,   574,    -1,   843,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   751,    -1,    -1,    -1,
     588,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   766,   767,    -1,   769,     0,    -1,    -1,   606,    -1,
      -1,    -1,   610,     8,     9,    10,   614,    -1,    13,    14,
      15,    -1,    17,   788,   622,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    27,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    36,    37,    -1,    39,    40,    41,    42,    43,    64,
      65,    66,    67,    68,    69,    70,   821,   655,    73,    74,
      -1,    -1,    -1,   828,    79,    80,    -1,    -1,    -1,   834,
      -1,   836,    -1,    -1,    -1,    -1,    -1,    -1,   843,    -1,
      -1,    -1,    -1,    -1,    -1,   683,    81,    -1,    -1,    -1,
      -1,   106,   107,   108,   109,   110,   111,   112,   113,   114,
     115,    -1,    -1,    -1,    -1,    -1,     0,    -1,    -1,    -1,
     105,    -1,    -1,    -1,     8,     9,    10,    -1,   716,    -1,
      14,    15,    -1,    17,    -1,   723,   121,    -1,    -1,   124,
     125,    -1,   730,   128,    -1,   130,   131,    -1,    -1,    -1,
      -1,    -1,    36,    37,    -1,    39,    40,    41,    42,    43,
      -1,    -1,    -1,    -1,   752,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      64,    65,    66,    67,    68,    69,    70,    71,    72,    73,
      74,    75,    76,    -1,    -1,    79,    80,    81,    -1,    83,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   795,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   804,    -1,    -1,   103,
     104,   809,   106,   107,   108,   109,   110,   111,   112,   113,
     114,   115,    -1,    -1,    -1,    -1,    -1,   121,   122,    -1,
     124,   125,   126,    -1,    -1,    -1,   130,   131,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   846,    -1,
     848,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     868,    -1,   870,    -1,    -1,    -1,     0,     1,    -1,     3,
       4,     5,     6,     7,    -1,    -1,   884,    11,    12,    -1,
      -1,   889,    16,    -1,    18,    19,    20,    21,    22,    23,
      24,    -1,    -1,    -1,    -1,    29,    30,    31,    32,    33,
      34,    35,    -1,    -1,    38,    -1,    -1,    -1,    -1,    -1,
      44,    45,    46,    47,    48,    49,    50,    51,    52,    53,
      54,    55,    56,    57,    -1,    59,    60,    -1,    62,    63,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    82,    -1,
      -1,    85,    86,    -1,    88,    89,    -1,    91,    -1,    93,
      94,    95,    96,    97,    98,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,     0,    -1,    -1,
      -1,    -1,   116,   117,   118,     8,     9,    10,    -1,    -1,
      -1,    14,    15,    -1,    17,    -1,   130,   131,    -1,    -1,
      43,    -1,    -1,    26,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    36,    37,    -1,    39,    40,    41,    42,
      43,    64,    65,    66,    67,    68,    69,    70,    71,    72,
      73,    74,    75,    76,    -1,    -1,    79,    80,    -1,    -1,
      -1,    64,    65,    66,    67,    68,    69,    70,    71,    72,
      73,    74,    75,    76,    -1,    -1,    79,    80,    81,    -1,
      83,   104,    -1,   106,   107,   108,   109,   110,   111,   112,
     113,   114,   115,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     103,   104,    -1,   106,   107,   108,   109,   110,   111,   112,
     113,   114,   115,    -1,    -1,    -1,    -1,   120,   121,   122,
      -1,   124,   125,   126,    -1,    -1,    -1,   130,   131,     1,
      -1,     3,     4,     5,     6,     7,     8,     9,    10,    11,
      12,    -1,    -1,    15,    16,    -1,    18,    19,    20,    21,
      22,    23,    24,    -1,    -1,    -1,    -1,    29,    30,    31,
      32,    33,    34,    35,    -1,    -1,    38,    -1,    -1,    -1,
      -1,    -1,    44,    45,    46,    47,    48,    49,    50,    51,
      52,    53,    54,    55,    56,    57,    -1,    59,    60,    -1,
      62,    63,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      82,    -1,    -1,    85,    86,    -1,    88,    89,    -1,    91,
      -1,    93,    94,    95,    96,    97,    98,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   116,   117,   118,    -1,    -1,    -1,
      -1,     1,    -1,     3,     4,     5,     6,     7,   130,   131,
      10,    11,    12,    -1,    14,    15,    16,    -1,    18,    19,
      20,    21,    22,    23,    24,    -1,    -1,    -1,    -1,    29,
      30,    31,    32,    33,    34,    35,    -1,    -1,    38,    -1,
      -1,    -1,    -1,    -1,    44,    45,    46,    47,    48,    49,
      50,    51,    52,    53,    54,    55,    56,    57,    -1,    59,
      60,    -1,    62,    63,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    82,    -1,    -1,    85,    86,    -1,    88,    89,
      -1,    91,    -1,    93,    94,    95,    96,    97,    98,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   116,   117,   118,    -1,
      -1,    -1,    -1,     1,    -1,     3,     4,     5,     6,     7,
     130,   131,    10,    11,    12,    -1,    -1,    15,    16,    17,
      18,    19,    20,    21,    22,    23,    24,    -1,    -1,    -1,
      -1,    29,    30,    31,    32,    33,    34,    35,    -1,    -1,
      38,    -1,    -1,    -1,    -1,    -1,    44,    45,    46,    47,
      48,    49,    50,    51,    52,    53,    54,    55,    56,    57,
      -1,    59,    60,    -1,    62,    63,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    82,    -1,    -1,    85,    86,    -1,
      88,    89,    -1,    91,    -1,    93,    94,    95,    96,    97,
      98,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   116,   117,
     118,    -1,    -1,    -1,    -1,     1,    -1,     3,     4,     5,
       6,     7,   130,   131,    10,    11,    12,    -1,    -1,    15,
      16,    -1,    18,    19,    20,    21,    22,    23,    24,    -1,
      -1,    -1,    -1,    29,    30,    31,    32,    33,    34,    35,
      -1,    -1,    38,    -1,    -1,    -1,    -1,    -1,    44,    45,
      46,    47,    48,    49,    50,    51,    52,    53,    54,    55,
      56,    57,    -1,    59,    60,    -1,    62,    63,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    82,    -1,    -1,    85,
      86,    -1,    88,    89,    -1,    91,    -1,    93,    94,    95,
      96,    97,    98,    -1,    -1,    -1,    -1,    -1,     1,    -1,
       3,     4,     5,     6,     7,    -1,     9,    10,    11,    12,
     116,   117,   118,    16,    -1,    18,    19,    20,    21,    22,
      23,    24,    -1,    -1,   130,   131,    29,    30,    31,    32,
      33,    34,    35,    -1,    -1,    38,    -1,    -1,    -1,    -1,
      -1,    44,    45,    46,    47,    48,    49,    50,    51,    52,
      53,    54,    55,    56,    57,    -1,    59,    60,    -1,    62,
      63,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    82,
      -1,    -1,    85,    86,    -1,    88,    89,    -1,    91,    -1,
      93,    94,    95,    96,    97,    98,    -1,    -1,    -1,    -1,
      -1,     1,    -1,     3,     4,     5,     6,     7,    -1,    -1,
      -1,    11,    12,   116,   117,   118,    16,    -1,    18,    19,
      20,    21,    22,    23,    24,    -1,    -1,   130,   131,    29,
      30,    31,    32,    33,    34,    35,    -1,    -1,    38,    -1,
      -1,    -1,    -1,    -1,    44,    45,    46,    47,    48,    49,
      50,    51,    52,    53,    54,    55,    56,    57,    -1,    59,
      60,    -1,    62,    63,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    82,    -1,    -1,    85,    86,    -1,    88,    89,
      -1,    91,    -1,    93,    94,    95,    96,    97,    98,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   116,   117,   118,    -1,
      -1,    -1,    -1,    -1,    -1,   125,    -1,    -1,    -1,    -1,
     130,   131,     1,    -1,     3,     4,     5,     6,     7,    -1,
      -1,    -1,    11,    12,    -1,    -1,    -1,    16,    -1,    18,
      19,    20,    21,    22,    23,    24,    -1,    -1,    -1,    -1,
      29,    30,    31,    32,    33,    34,    35,    -1,    -1,    38,
      -1,    -1,    -1,    -1,    -1,    44,    45,    46,    47,    48,
      49,    50,    51,    52,    53,    54,    55,    56,    57,    -1,
      59,    60,    -1,    62,    63,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    82,    -1,    -1,    85,    86,    -1,    88,
      89,    -1,    91,    -1,    93,    94,    95,    96,    97,    98,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   116,   117,   118,
      -1,    -1,    -1,    -1,    -1,    -1,   125,    -1,    -1,    -1,
      -1,   130,   131,     1,    -1,     3,     4,     5,     6,     7,
      -1,    -1,    -1,    11,    12,    -1,    -1,    -1,    16,    -1,
      18,    19,    20,    21,    22,    23,    24,    -1,    -1,    -1,
      -1,    29,    30,    31,    32,    33,    34,    35,    -1,    -1,
      38,    -1,    -1,    -1,    -1,    -1,    44,    45,    46,    47,
      48,    49,    50,    51,    52,    53,    54,    55,    56,    57,
      -1,    59,    60,    -1,    62,    63,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    82,    -1,    -1,    85,    86,    -1,
      88,    89,    -1,    91,    -1,    93,    94,    95,    96,    97,
      98,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   116,   117,
     118,    -1,    -1,   121,    -1,     1,    -1,     3,     4,     5,
       6,     7,   130,   131,    10,    11,    12,    -1,    -1,    -1,
      16,    -1,    18,    19,    20,    21,    22,    23,    24,    -1,
      -1,    -1,    -1,    29,    30,    31,    32,    33,    34,    35,
      -1,    -1,    38,    -1,    -1,    -1,    -1,    -1,    44,    45,
      46,    47,    48,    49,    50,    51,    52,    53,    54,    55,
      56,    57,    -1,    59,    60,    -1,    62,    63,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    82,    -1,    -1,    85,
      86,    -1,    88,    89,    -1,    91,    -1,    93,    94,    95,
      96,    97,    98,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
       3,     4,     5,     6,     7,    -1,    -1,    -1,    11,    12,
     116,   117,   118,    16,    -1,    18,    19,    20,    21,    22,
      23,    24,    -1,    -1,   130,   131,    29,    30,    31,    32,
      33,    34,    35,    -1,    -1,    38,    -1,    -1,    -1,    -1,
      -1,    44,    45,    46,    47,    48,    49,    50,    51,    52,
      53,    54,    55,    56,    57,    -1,    59,    60,    -1,    62,
      63,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    82,
      -1,    -1,    85,    86,    -1,    88,    89,    -1,    91,    -1,
      93,    94,    95,    96,    97,    98,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,     3,     4,     5,    -1,     7,    -1,    -1,
      -1,    11,    12,   116,   117,   118,    16,    -1,    18,    19,
      20,    21,    22,    23,    24,    -1,    -1,    -1,   131,    29,
      30,    31,    32,    33,    34,    35,    -1,    -1,    38,    -1,
      -1,    -1,    -1,    -1,    -1,    45,    -1,    -1,    48,    49,
      50,    51,    52,    53,    54,    55,    56,    57,    -1,    59,
      60,    -1,    62,    63,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    82,    -1,    -1,    85,    86,    -1,    88,    89,
      -1,    -1,    -1,    93,    94,    95,    96,    97,    98,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   116,   117,   118,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     130,   131,     3,     4,     5,     6,     7,     8,     9,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      21,    22,    23,    24,    25,    26,    -1,    -1,    29,    30,
      31,    32,    33,    34,    35,    36,    37,    38,    -1,    -1,
      -1,    -1,    -1,    44,    45,    46,    47,    48,    49,    50,
      51,    52,    53,    54,    55,    -1,    -1,    -1,    -1,    -1,
      -1,    62,    63,    64,    65,    66,    67,    -1,    69,    70,
      -1,    -1,    73,    -1,    -1,    -1,    77,    78,    79,    80,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      91,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   106,   107,   108,   109,   110,
     111,   112,   113,   114,   115,    -1,    -1,   118,    -1,    -1,
       3,     4,     5,    -1,     7,    -1,   127,   128,    11,    12,
      -1,    -1,    -1,    16,    -1,    18,    19,    20,    21,    22,
      23,    24,    -1,    26,    -1,    -1,    29,    30,    31,    32,
      33,    34,    35,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    45,    -1,    -1,    48,    49,    50,    51,    52,
      53,    54,    55,    56,    57,    -1,    59,    60,    -1,    62,
      63,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    82,
      -1,    -1,    85,    86,    -1,    88,    89,    -1,    91,    92,
      93,    94,    95,    96,    97,    98,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,     3,     4,     5,    -1,     7,    -1,    -1,
      -1,    11,    12,   116,   117,   118,    16,   120,    18,    19,
      20,    21,    22,    23,    24,   128,    26,    -1,    -1,    29,
      30,    31,    32,    33,    34,    35,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    45,    -1,    -1,    48,    49,
      50,    51,    52,    53,    54,    55,    56,    57,    -1,    59,
      60,    -1,    62,    63,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    82,    -1,    -1,    85,    86,    -1,    88,    89,
      -1,    91,    92,    93,    94,    95,    96,    97,    98,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,     3,     4,     5,    -1,
       7,    -1,    -1,    -1,    11,    12,   116,   117,   118,    16,
     120,    18,    19,    20,    21,    22,    23,    24,   128,    26,
      -1,    -1,    29,    30,    31,    32,    33,    34,    35,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    45,    -1,
      -1,    48,    49,    50,    51,    52,    53,    54,    55,    56,
      57,    -1,    59,    60,    -1,    62,    63,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    82,    -1,    -1,    85,    86,
      -1,    88,    89,    -1,    91,    92,    93,    94,    95,    96,
      97,    98,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     3,
       4,     5,    -1,     7,    -1,    -1,    -1,    11,    12,   116,
     117,   118,    16,   120,    18,    19,    20,    21,    22,    23,
      24,   128,    -1,    -1,    -1,    29,    30,    31,    32,    33,
      34,    35,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    45,    -1,    -1,    48,    49,    50,    51,    52,    53,
      54,    55,    56,    57,    -1,    59,    60,    -1,    62,    63,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    82,    83,
      -1,    85,    86,    -1,    88,    89,    -1,    91,    92,    93,
      94,    95,    96,    97,    98,    -1,    -1,    -1,    -1,   103,
      -1,    -1,     3,     4,     5,    -1,     7,    -1,    -1,    -1,
      11,    12,   116,   117,   118,    16,    -1,    18,    19,    20,
      21,    22,    23,    24,   128,    -1,    -1,    -1,    29,    30,
      31,    32,    33,    34,    35,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    45,    -1,    -1,    48,    49,    50,
      51,    52,    53,    54,    55,    56,    57,    -1,    59,    60,
      -1,    62,    63,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    82,    83,    -1,    85,    86,    -1,    88,    89,    -1,
      91,    92,    93,    94,    95,    96,    97,    98,    -1,    -1,
      -1,    -1,   103,    -1,    -1,     3,     4,     5,    -1,     7,
      -1,    -1,    -1,    11,    12,   116,   117,   118,    16,    -1,
      18,    19,    20,    21,    22,    23,    24,   128,    -1,    -1,
      -1,    29,    30,    31,    32,    33,    34,    35,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    45,    -1,    -1,
      48,    49,    50,    51,    52,    53,    54,    55,    56,    57,
      -1,    59,    60,    -1,    62,    63,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    82,    -1,    -1,    85,    86,    -1,
      88,    89,    -1,    91,    92,    93,    94,    95,    96,    97,
      98,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     3,     4,
       5,    -1,     7,    -1,    -1,    -1,    11,    12,   116,   117,
     118,    16,    -1,    18,    19,    20,    21,    22,    23,    24,
     128,    -1,    -1,    -1,    29,    30,    31,    32,    33,    34,
      35,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      45,    -1,    -1,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    -1,    59,    60,    -1,    62,    63,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    82,    -1,    -1,
      85,    86,    -1,    88,    89,    -1,    91,    92,    93,    94,
      95,    96,    97,    98,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,     3,     4,     5,    -1,     7,    -1,    -1,    -1,    11,
      12,   116,   117,   118,    16,    -1,    18,    19,    20,    21,
      22,    23,    24,   128,    -1,    -1,    -1,    29,    30,    31,
      32,    33,    34,    35,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    45,    -1,    -1,    48,    49,    50,    51,
      52,    53,    54,    55,    56,    57,    -1,    59,    60,    -1,
      62,    63,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      82,    -1,    -1,    85,    86,    -1,    88,    89,    -1,    91,
      92,    93,    94,    95,    96,    97,    98,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,     3,     4,     5,    -1,     7,    -1,
      -1,    -1,    11,    12,   116,   117,   118,    16,    -1,    18,
      19,    20,    21,    22,    23,    24,   128,    -1,    -1,    -1,
      29,    30,    31,    32,    33,    34,    35,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    45,    -1,    -1,    48,
      49,    50,    51,    52,    53,    54,    55,    56,    57,    -1,
      59,    60,    -1,    62,    63,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    82,    -1,    -1,    85,    86,    -1,    88,
      89,    -1,    91,    92,    93,    94,    95,    96,    97,    98,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   116,   117,   118,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   128,
       3,     4,     5,     6,     7,     8,     9,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    21,    22,
      23,    24,    25,    26,    -1,    -1,    29,    30,    31,    32,
      33,    34,    35,    36,    37,    38,    -1,    -1,    -1,    -1,
      -1,    44,    45,    46,    47,    48,    49,    50,    51,    52,
      53,    54,    55,    -1,    -1,    -1,    -1,    -1,    -1,    62,
      63,    64,    65,    66,    67,    -1,    69,    70,    -1,    -1,
      73,    -1,    -1,    -1,    77,    78,    79,    80,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    91,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   106,   107,   108,   109,   110,   111,   112,
     113,   114,   115,    -1,    -1,   118,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   127,     3,     4,     5,     6,     7,
       8,     9,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    21,    22,    23,    24,    25,    26,    -1,
      -1,    29,    30,    31,    32,    33,    34,    35,    36,    37,
      38,    -1,    -1,    -1,    -1,    -1,    44,    45,    46,    47,
      48,    49,    50,    51,    52,    -1,    54,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    62,    63,    64,    65,    66,    67,
      -1,    69,    70,    -1,    -1,    73,    -1,    -1,    -1,    77,
      78,    79,    80,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    91,    -1,    93,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   106,   107,
     108,   109,   110,   111,   112,   113,   114,   115,    -1,    -1,
     118,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   127,
       3,     4,     5,     6,     7,     8,     9,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    21,    22,
      23,    24,    25,    26,    -1,    -1,    29,    30,    31,    32,
      33,    34,    35,    36,    37,    38,    -1,    -1,    -1,    -1,
      -1,    44,    45,    46,    47,    48,    49,    50,    51,    -1,
      -1,    54,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    62,
      63,    64,    65,    66,    67,    -1,    69,    70,    -1,    -1,
      73,    -1,    -1,    -1,    77,    78,    79,    80,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    91,    -1,
      93,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   106,   107,   108,   109,   110,   111,   112,
     113,   114,   115,    -1,    -1,   118,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   127,     3,     4,     5,     6,     7,
       8,     9,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    21,    22,    23,    24,    25,    26,    -1,
      -1,    29,    30,    31,    32,    33,    34,    35,    36,    37,
      38,    -1,    -1,    -1,    -1,    -1,    44,    45,    46,    47,
      48,    49,    50,    51,    -1,    -1,    54,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    62,    63,    64,    65,    66,    67,
      -1,    69,    70,    -1,    -1,    73,    -1,    -1,    -1,    77,
      78,    79,    80,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    91,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   106,   107,
     108,   109,   110,   111,   112,   113,   114,   115,    -1,    -1,
     118,    -1,     3,     4,     5,    -1,     7,    -1,    -1,   127,
      11,    12,    -1,    -1,    -1,    16,    -1,    18,    19,    20,
      21,    22,    23,    24,    -1,    -1,    -1,    -1,    29,    30,
      31,    32,    33,    34,    35,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    45,    -1,    -1,    48,    49,    50,
      51,    52,    53,    54,    55,    56,    57,    -1,    59,    60,
      -1,    62,    63,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    82,    -1,    -1,    85,    86,    -1,    88,    89,    -1,
      91,    92,    93,    94,    95,    96,    97,    98,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
       3,     4,     5,     6,     7,   116,   117,   118,    11,    12,
      -1,    -1,    -1,    16,   125,    18,    19,    20,    21,    22,
      23,    24,    -1,    -1,    -1,    -1,    29,    30,    31,    32,
      33,    34,    35,    -1,    -1,    38,    -1,    -1,    -1,    -1,
      -1,    44,    45,    46,    47,    48,    49,    50,    51,    52,
      53,    54,    55,    56,    57,    -1,    59,    60,    -1,    62,
      63,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    82,
      -1,    -1,    85,    86,    -1,    88,    89,    -1,    91,    -1,
      93,    94,    95,    96,    97,    98,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,     3,     4,     5,    -1,     7,    -1,    -1,
      -1,    11,    12,   116,   117,   118,    16,    -1,    18,    19,
      20,    21,    22,    23,    24,    -1,    -1,    -1,    -1,    29,
      30,    31,    32,    33,    34,    35,    -1,    -1,    38,    -1,
      -1,    -1,    -1,    -1,    -1,    45,    -1,    -1,    48,    49,
      50,    51,    52,    53,    54,    55,    56,    57,    -1,    59,
      60,    -1,    62,    63,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    82,    -1,    -1,    85,    86,    -1,    88,    89,
      -1,    91,    92,    93,    94,    95,    96,    97,    98,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,     3,     4,     5,    -1,
       7,    -1,    -1,    -1,    11,    12,   116,   117,   118,    16,
      -1,    18,    19,    20,    21,    22,    23,    24,    -1,    -1,
      -1,    -1,    29,    30,    31,    32,    33,    34,    35,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    45,    -1,
      -1,    48,    49,    50,    51,    52,    53,    54,    55,    56,
      57,    -1,    59,    60,    -1,    62,    63,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    82,    -1,    -1,    85,    86,
      -1,    88,    89,    -1,    91,    92,    93,    94,    95,    96,
      97,    98,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     3,
       4,     5,    -1,     7,    -1,    -1,    -1,    11,    12,   116,
     117,   118,    16,    -1,    18,    19,    20,    21,    22,    23,
      24,    -1,    -1,    -1,    -1,    29,    30,    31,    32,    33,
      34,    35,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    45,    -1,    -1,    48,    49,    50,    51,    52,    53,
      54,    55,    56,    57,    -1,    59,    60,    -1,    62,    63,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    82,    -1,
      -1,    85,    86,    -1,    88,    89,    -1,    91,    92,    93,
      94,    95,    96,    97,    98,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,     3,     4,     5,    -1,     7,    -1,    -1,    -1,
      11,    12,   116,   117,   118,    16,    -1,    18,    19,    20,
      21,    22,    23,    24,    -1,    -1,    -1,    -1,    29,    30,
      31,    32,    33,    34,    35,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    45,    -1,    -1,    48,    49,    50,
      51,    52,    53,    54,    55,    56,    57,    -1,    59,    60,
      -1,    62,    63,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    82,    -1,    -1,    85,    86,    -1,    88,    89,    -1,
      91,    92,    93,    94,    95,    96,    97,    98,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,     3,     4,     5,    -1,     7,
      -1,    -1,    -1,    11,    12,   116,   117,   118,    16,    -1,
      18,    19,    20,    21,    22,    23,    24,    -1,    -1,    -1,
      -1,    29,    30,    31,    32,    33,    34,    35,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    45,    -1,    -1,
      48,    49,    50,    51,    52,    53,    54,    55,    56,    57,
      -1,    59,    60,    -1,    62,    63,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    82,    -1,    -1,    85,    86,    -1,
      88,    89,    -1,    91,    92,    93,    94,    95,    96,    97,
      98,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     3,     4,
       5,    -1,     7,    -1,    -1,    -1,    11,    12,   116,   117,
     118,    16,    -1,    18,    19,    20,    21,    22,    23,    24,
      -1,    -1,    -1,    -1,    29,    30,    31,    32,    33,    34,
      35,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      45,    -1,    -1,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    -1,    59,    60,    -1,    62,    63,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    82,    -1,    -1,
      85,    86,    -1,    88,    89,    -1,    91,    92,    93,    94,
      95,    96,    97,    98,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,     3,     4,     5,    -1,     7,    -1,    -1,    -1,    11,
      12,   116,   117,   118,    16,    -1,    18,    19,    20,    21,
      22,    23,    24,    -1,    -1,    -1,    -1,    29,    30,    31,
      32,    33,    34,    35,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    45,    -1,    -1,    48,    49,    50,    51,
      52,    53,    54,    55,    56,    57,    -1,    59,    60,    -1,
      62,    63,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      82,    -1,    -1,    85,    86,    -1,    88,    89,    -1,    91,
      92,    93,    94,    95,    96,    97,    98,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,     3,     4,     5,    -1,     7,    -1,
      -1,    -1,    11,    12,   116,   117,   118,    16,    -1,    18,
      19,    20,    21,    22,    23,    24,    -1,    -1,    -1,    -1,
      29,    30,    31,    32,    33,    34,    35,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    45,    -1,    -1,    48,
      49,    50,    51,    52,    53,    54,    55,    56,    57,    -1,
      59,    60,    -1,    62,    63,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    82,    -1,    -1,    85,    86,    -1,    88,
      89,    -1,    91,    92,    93,    94,    95,    96,    97,    98,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,     3,     4,     5,
      -1,     7,    -1,    -1,    -1,    11,    12,   116,   117,   118,
      16,    -1,    18,    19,    20,    21,    22,    23,    24,    -1,
      -1,    -1,    -1,    29,    30,    31,    32,    33,    34,    35,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    45,
      -1,    -1,    48,    49,    50,    51,    52,    53,    54,    55,
      56,    57,    -1,    59,    60,    -1,    62,    63,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    82,    -1,    -1,    85,
      86,    -1,    88,    89,    -1,    91,    92,    93,    94,    95,
      96,    97,    98,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
       3,     4,     5,    -1,     7,    -1,    -1,    -1,    11,    12,
     116,   117,   118,    16,    -1,    18,    19,    20,    21,    22,
      23,    24,    -1,    -1,    -1,    -1,    29,    30,    31,    32,
      33,    34,    35,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    45,    -1,    -1,    48,    49,    50,    51,    52,
      53,    54,    55,    56,    57,    -1,    59,    60,    -1,    62,
      63,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    82,
      -1,    -1,    85,    86,    -1,    88,    89,    -1,    91,    92,
      93,    94,    95,    96,    97,    98,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,     3,     4,     5,    -1,     7,    -1,    -1,
      -1,    11,    12,   116,   117,   118,    16,    -1,    18,    19,
      20,    21,    22,    23,    24,    -1,    -1,    -1,    -1,    29,
      30,    31,    32,    33,    34,    35,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    45,    -1,    -1,    48,    49,
      50,    51,    52,    53,    54,    55,    56,    57,    -1,    59,
      60,    -1,    62,    63,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    82,    -1,    -1,    85,    86,    -1,    88,    89,
      -1,    91,    92,    93,    94,    95,    96,    97,    98,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,     3,     4,     5,    -1,
       7,    -1,    -1,    -1,    11,    12,   116,   117,   118,    16,
      -1,    18,    19,    20,    21,    22,    23,    24,    -1,    -1,
      -1,    -1,    29,    30,    31,    32,    33,    34,    35,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    45,    -1,
      -1,    48,    49,    50,    51,    52,    53,    54,    55,    56,
      57,    -1,    59,    60,    -1,    62,    63,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    82,    -1,    -1,    85,    86,
      -1,    88,    89,    -1,    91,    92,    93,    94,    95,    96,
      97,    98,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     3,
       4,     5,    -1,     7,    -1,    -1,    -1,    11,    12,   116,
     117,   118,    16,    -1,    18,    19,    20,    21,    22,    23,
      24,    -1,    -1,    -1,    -1,    29,    30,    31,    32,    33,
      34,    35,    -1,    -1,    38,    -1,    -1,    -1,    -1,    -1,
      -1,    45,    -1,    -1,    48,    49,    50,    51,    52,    53,
      54,    55,    56,    57,    -1,    59,    60,    -1,    62,    63,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    82,    -1,
      -1,    85,    86,    -1,    88,    89,    -1,    -1,    -1,    93,
      94,    95,    96,    97,    98,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,     3,     4,     5,    -1,     7,    -1,    -1,    -1,
      11,    12,   116,   117,   118,    16,    -1,    18,    19,    20,
      21,    22,    23,    24,    -1,    -1,    -1,    -1,    29,    30,
      31,    32,    33,    34,    35,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    45,    -1,    -1,    48,    49,    50,
      51,    52,    53,    54,    55,    56,    57,    -1,    59,    60,
      -1,    62,    63,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    82,    -1,    -1,    85,    86,    -1,    88,    89,    -1,
      91,    -1,    93,    94,    95,    96,    97,    98,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,     3,     4,     5,    -1,     7,
      -1,    -1,    -1,    11,    12,   116,   117,   118,    16,    -1,
      18,    19,    20,    21,    22,    23,    24,    -1,    -1,    -1,
      -1,    29,    30,    31,    32,    33,    34,    35,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    45,    -1,    -1,
      48,    49,    50,    51,    52,    53,    54,    55,    56,    57,
      -1,    59,    60,    -1,    62,    63,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    82,    -1,    -1,    85,    86,    -1,
      88,    89,    -1,    91,    -1,    93,    94,    95,    96,    97,
      98,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     3,     4,
       5,    -1,     7,    -1,    -1,    -1,    11,    12,   116,   117,
     118,    16,    -1,    18,    19,    20,    21,    22,    23,    24,
      -1,    -1,    -1,    -1,    29,    30,    31,    32,    33,    34,
      35,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      45,    -1,    -1,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    -1,    59,    60,    -1,    62,    63,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    82,    -1,    -1,
      85,    86,    -1,    88,    89,    -1,    91,    -1,    93,    94,
      95,    96,    97,    98,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,     3,     4,     5,    -1,     7,    -1,    -1,    -1,    11,
      12,   116,   117,   118,    16,    -1,    18,    19,    20,    21,
      22,    23,    24,    -1,    -1,    -1,    -1,    29,    30,    31,
      32,    33,    34,    35,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    45,    -1,    -1,    48,    49,    50,    51,
      52,    53,    54,    55,    56,    57,    -1,    59,    60,    -1,
      62,    63,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      82,    -1,    -1,    85,    86,    -1,    88,    89,    -1,    91,
      -1,    93,    94,    95,    96,    97,    98,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,     3,     4,     5,    -1,     7,    -1,
      -1,    -1,    11,    12,   116,   117,   118,    16,    -1,    18,
      19,    20,    21,    22,    23,    24,    -1,    -1,    -1,    -1,
      29,    30,    31,    32,    33,    34,    35,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    45,    -1,    -1,    48,
      49,    50,    51,    52,    53,    54,    55,    56,    57,    -1,
      59,    60,    -1,    62,    63,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    82,    -1,    -1,    85,    86,    -1,    88,
      89,    -1,    91,    -1,    93,    94,    95,    96,    97,    98,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,     3,     4,     5,
      -1,     7,    -1,    -1,    -1,    11,    12,   116,   117,   118,
      16,    -1,    18,    19,    20,    21,    22,    23,    24,    -1,
      -1,    -1,    -1,    29,    30,    31,    32,    33,    34,    35,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    45,
      -1,    -1,    48,    49,    50,    51,    52,    53,    54,    55,
      56,    57,    -1,    59,    60,    -1,    62,    63,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    82,    -1,    -1,    85,
      86,    -1,    88,    89,    -1,    91,    -1,    93,    94,    95,
      96,    97,    98,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
       3,     4,     5,    -1,     7,    -1,    -1,    -1,    11,    12,
     116,   117,   118,    16,    -1,    18,    19,    20,    21,    22,
      23,    24,    -1,    -1,    -1,    -1,    29,    30,    31,    32,
      33,    34,    35,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    45,    -1,    -1,    48,    49,    50,    51,    52,
      53,    54,    55,    56,    57,    -1,    59,    60,    -1,    62,
      63,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    82,
      -1,    -1,    85,    86,    -1,    88,    89,    -1,    91,    -1,
      93,    94,    95,    96,    97,    98,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,     3,     4,     5,    -1,     7,    -1,    -1,
      -1,    11,    12,   116,   117,   118,    16,    -1,    18,    19,
      20,    21,    22,    23,    24,    -1,    -1,    -1,    -1,    29,
      30,    31,    32,    33,    34,    35,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    45,    -1,    -1,    48,    49,
      50,    51,    52,    53,    54,    55,    56,    57,    -1,    59,
      60,    -1,    62,    63,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    82,    -1,    -1,    85,    86,    -1,    88,    89,
      -1,    -1,    -1,    93,    94,    95,    96,    97,    98,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,     3,     4,     5,    -1,
       7,    -1,    -1,    -1,    11,    12,   116,   117,   118,    16,
      -1,    18,    19,    20,    21,    22,    23,    24,    -1,    -1,
      -1,    -1,    29,    30,    31,    32,    33,    34,    35,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    45,    -1,
      -1,    48,    49,    50,    51,    52,    53,    54,    55,    56,
      57,    -1,    59,    60,    -1,    62,    63,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    82,    -1,    -1,    85,    86,
      -1,    88,    89,    -1,    -1,    -1,    93,    94,    95,    96,
      97,    98,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     3,
       4,     5,    -1,     7,    -1,    -1,    -1,    11,    12,   116,
     117,   118,    16,    -1,    18,    19,    20,    21,    22,    23,
      24,    -1,    -1,    -1,    -1,    29,    30,    31,    32,    33,
      34,    35,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    45,    -1,    -1,    48,    49,    50,    51,    52,    53,
      54,    55,    56,    57,    -1,    59,    60,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    82,    -1,
      -1,    85,    86,    -1,    88,    89,    -1,    91,    -1,    93,
      94,    95,    96,    97,    98,    -1,    -1,    -1,     3,     4,
       5,    -1,     7,    -1,   108,    -1,    11,    12,    -1,    -1,
      -1,    16,   116,    18,    19,    20,    21,    22,    23,    24,
      -1,    -1,    -1,    -1,    29,    30,    31,    32,    33,    34,
      35,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      45,    -1,    -1,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    -1,    59,    60,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    79,    -1,    -1,    82,    -1,    -1,
      85,    86,    -1,    88,    89,    -1,    -1,    -1,    93,    94,
      95,    96,    97,    98,    -1,    -1,    -1,     3,     4,     5,
      -1,     7,    -1,    -1,    -1,    11,    12,    -1,    -1,    -1,
      16,   116,    18,    19,    20,    21,    22,    23,    24,    -1,
      -1,    -1,    -1,    29,    30,    31,    32,    33,    34,    35,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    45,
      -1,    -1,    48,    49,    50,    51,    52,    53,    54,    55,
      56,    57,    -1,    59,    60,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    82,    -1,    -1,    85,
      86,    -1,    88,    89,    -1,    91,    -1,    93,    94,    95,
      96,    97,    98,    -1,    -1,    -1,     3,     4,     5,    -1,
       7,    -1,    -1,    -1,    11,    12,    -1,    -1,    -1,    16,
     116,    18,    19,    20,    21,    22,    23,    24,    -1,    -1,
      -1,    -1,    29,    30,    31,    32,    33,    34,    35,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    45,    -1,
      -1,    48,    49,    50,    51,    52,    53,    54,    55,    56,
      57,    -1,    59,    60,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    82,    -1,    -1,    85,    86,
      -1,    88,    89,    -1,    91,    -1,    93,    94,    95,    96,
      97,    98,    -1,    -1,    -1,     3,     4,     5,    -1,     7,
      -1,    -1,    -1,    11,    12,    -1,    -1,    -1,    16,   116,
      18,    19,    20,    21,    22,    23,    24,    -1,    -1,    -1,
      -1,    29,    30,    31,    32,    33,    34,    35,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    45,    -1,    -1,
      48,    49,    50,    51,    52,    53,    54,    55,    56,    57,
      -1,    59,    60,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    82,    -1,    -1,    85,    86,    -1,
      88,    89,    -1,    -1,    -1,    93,    94,    95,    96,    97,
      98,    -1,    -1,    -1,     3,     4,     5,    -1,     7,    -1,
      -1,    -1,    11,    12,    -1,    -1,    -1,    16,   116,    18,
      19,    20,    21,    22,    23,    24,    -1,    -1,    -1,    -1,
      29,    30,    31,    32,    33,    34,    35,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    45,    -1,    -1,    48,
      49,    50,    51,    52,    53,    54,    55,    56,    57,    -1,
      59,    60,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    82,    -1,    -1,    85,    86,    -1,    88,
      89,    -1,    -1,    -1,    93,    94,    95,    96,    97,    98,
      -1,    -1,    -1,     3,     4,     5,    -1,     7,    -1,    -1,
      -1,    11,    12,    -1,    -1,    -1,    16,   116,    18,    19,
      20,    21,    22,    23,    24,    -1,    -1,    -1,    -1,    29,
      30,    31,    32,    33,    34,    35,    -1,    -1,    -1,    -1,
      43,    -1,    -1,    -1,    -1,    45,    -1,    -1,    48,    49,
      50,    51,    52,    53,    54,    55,    56,    57,    -1,    59,
      60,    64,    65,    66,    67,    68,    69,    70,    71,    72,
      73,    74,    75,    76,    -1,    -1,    79,    80,    -1,    -1,
      -1,    -1,    82,    -1,    -1,    85,    86,    -1,    88,    89,
      -1,    -1,    -1,    93,    94,    95,    96,    97,    98,    -1,
      -1,   104,    -1,   106,   107,   108,   109,   110,   111,   112,
     113,   114,   115,    50,    51,    -1,   116,    54,    -1,    -1,
      -1,    -1,    -1,   126,    -1,    62,    63,    64,    65,    66,
      67,    -1,    69,    70,    -1,    -1,    73,    -1,    -1,    -1,
      77,    78,    79,    80,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    91,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   106,
     107,   108,   109,   110,   111,   112,   113,   114,   115,    50,
      51,   118,    -1,    54,    -1,    -1,    -1,    -1,    -1,    -1,
     127,    62,    63,    64,    65,    66,    67,    -1,    69,    70,
      -1,    -1,    73,    -1,    -1,    -1,    77,    78,    79,    80,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      91,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   106,   107,   108,   109,   110,
     111,   112,   113,   114,   115,    50,    51,   118,    -1,    54,
      -1,    -1,    -1,    -1,    -1,    -1,   127,    62,    63,    64,
      65,    66,    67,    -1,    69,    70,    -1,    -1,    73,    -1,
      -1,    -1,    77,    78,    79,    80,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    91,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   106,   107,   108,   109,   110,   111,   112,   113,   114,
     115,    50,    51,   118,    -1,    54,    -1,    -1,    -1,    -1,
      -1,    -1,   127,    62,    63,    64,    65,    66,    67,    -1,
      69,    70,    -1,    -1,    73,    -1,    -1,    -1,    77,    78,
      79,    80,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    91,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   106,   107,   108,
     109,   110,   111,   112,   113,   114,   115,    50,    51,   118,
      -1,    54,    -1,    -1,    -1,    -1,    -1,    -1,   127,    62,
      63,    64,    65,    66,    67,    -1,    69,    70,    -1,    -1,
      73,    -1,    -1,    -1,    77,    78,    79,    80,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    91,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   106,   107,   108,   109,   110,   111,   112,
     113,   114,   115,    50,    51,   118,    -1,    54,    -1,    -1,
      -1,    -1,    -1,    -1,   127,    62,    63,    64,    65,    66,
      67,    -1,    69,    70,    -1,    -1,    73,    -1,    -1,    -1,
      77,    78,    79,    80,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    91,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   106,
     107,   108,   109,   110,   111,   112,   113,   114,   115,    50,
      51,   118,    -1,    54,    -1,    -1,    -1,    -1,    -1,    -1,
     127,    62,    63,    64,    65,    66,    67,    -1,    69,    70,
      -1,    -1,    73,    -1,    -1,    -1,    77,    78,    79,    80,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      91,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   106,   107,   108,   109,   110,
     111,   112,   113,   114,   115,    50,    51,   118,    -1,    54,
      -1,    -1,    -1,    -1,    -1,    -1,   127,    62,    63,    64,
      65,    66,    67,    -1,    69,    70,    -1,    -1,    73,    -1,
      -1,    -1,    77,    78,    79,    80,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    91,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   106,   107,   108,   109,   110,   111,   112,   113,   114,
     115,    50,    51,   118,    -1,    54,    -1,    -1,    -1,    -1,
      -1,    -1,   127,    62,    63,    64,    65,    66,    67,    -1,
      69,    70,    -1,    -1,    73,    -1,    -1,    -1,    77,    78,
      79,    80,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    91,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   106,   107,   108,
     109,   110,   111,   112,   113,   114,   115,    50,    51,   118,
      -1,    54,    -1,    -1,    -1,    -1,    -1,    -1,   127,    62,
      63,    64,    65,    66,    67,    -1,    69,    70,    -1,    -1,
      73,    -1,    -1,    -1,    77,    78,    79,    80,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    91,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   106,   107,   108,   109,   110,   111,   112,
     113,   114,   115,    50,    51,   118,    -1,    54,    -1,    -1,
      -1,    -1,    -1,    -1,   127,    62,    63,    64,    65,    66,
      67,    -1,    69,    70,    -1,    -1,    73,    -1,    -1,    -1,
      77,    78,    79,    80,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    91,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   106,
     107,   108,   109,   110,   111,   112,   113,   114,   115,    50,
      51,   118,    -1,    54,    -1,    -1,    -1,    -1,    -1,    -1,
     127,    62,    63,    64,    65,    66,    67,    -1,    69,    70,
      -1,    -1,    73,    -1,    -1,    -1,    77,    78,    79,    80,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      91,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   106,   107,   108,   109,   110,
     111,   112,   113,   114,   115,    -1,    -1,   118,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   127,    64,    65,    66,
      67,    68,    69,    70,    71,    72,    73,    74,    75,    76,
      -1,    -1,    79,    80,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   104,    -1,   106,
     107,   108,   109,   110,   111,   112,   113,   114,   115,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    -1,   130,    79,    80,    -1,    -1,    -1,    84,
      64,    65,    66,    67,    68,    69,    70,    71,    72,    73,
      74,    75,    76,    -1,    -1,    79,    80,    -1,    -1,   104,
      -1,   106,   107,   108,   109,   110,   111,   112,   113,   114,
     115,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     104,   126,   106,   107,   108,   109,   110,   111,   112,   113,
     114,   115,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   126,    64,    65,    66,    67,    68,    69,    70,
      71,    72,    73,    74,    75,    76,    -1,    -1,    79,    80,
      64,    65,    66,    67,    68,    69,    70,    71,    72,    73,
      74,    75,    76,    -1,    -1,    79,    80,    -1,    -1,    -1,
      -1,    -1,    -1,   104,   105,   106,   107,   108,   109,   110,
     111,   112,   113,   114,   115,    -1,    -1,    -1,    -1,    -1,
     104,    -1,   106,   107,   108,   109,   110,   111,   112,   113,
     114,   115,    64,    65,    66,    67,    68,    69,    70,    71,
      72,    73,    74,    75,    76,    -1,    -1,    79,    80,    64,
      65,    66,    67,    68,    69,    70,    71,    -1,    73,    74,
      -1,    -1,    -1,    -1,    79,    80,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   106,   107,   108,   109,   110,   111,
     112,   113,   114,   115,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   106,   107,   108,   109,   110,   111,   112,   113,   114,
     115,    64,    65,    66,    67,    68,    69,    70,    -1,    -1,
      73,    74,    -1,    -1,    -1,    -1,    79,    80,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   106,   107,   108,   109,   110,   111,   112,
     113,   114,   115
};
/* -*-C-*-  Note some compilers choke on comments on `#line' lines.  */
#line 3 "/usr/share/bison/bison.simple"

/* Skeleton output parser for bison,

   Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002 Free Software
   Foundation, Inc.

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
   Foundation, Inc., 59 Temple Place - Suite 330,
   Boston, MA 02111-1307, USA.  */

/* As a special exception, when this file is copied by Bison into a
   Bison output file, you may use that output file without restriction.
   This special exception was added by the Free Software Foundation
   in version 1.24 of Bison.  */

/* This is the parser code that is written into each bison parser when
   the %semantic_parser declaration is not specified in the grammar.
   It was written by Richard Stallman by simplifying the hairy parser
   used when %semantic_parser is specified.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

#if ! defined (yyoverflow) || defined (YYERROR_VERBOSE)

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# if YYSTACK_USE_ALLOCA
#  define YYSTACK_ALLOC alloca
# else
#  ifndef YYSTACK_USE_ALLOCA
#   if defined (alloca) || defined (_ALLOCA_H)
#    define YYSTACK_ALLOC alloca
#   else
#    ifdef __GNUC__
#     define YYSTACK_ALLOC __builtin_alloca
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's `empty if-body' warning. */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
# else
#  if defined (__STDC__) || defined (__cplusplus)
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   define YYSIZE_T size_t
#  endif
#  define YYSTACK_ALLOC malloc
#  define YYSTACK_FREE free
# endif
#endif /* ! defined (yyoverflow) || defined (YYERROR_VERBOSE) */


#if (! defined (yyoverflow) \
     && (! defined (__cplusplus) \
	 || (YYLTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  short yyss;
  YYSTYPE yyvs;
# if YYLSP_NEEDED
  YYLTYPE yyls;
# endif
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAX (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# if YYLSP_NEEDED
#  define YYSTACK_BYTES(N) \
     ((N) * (sizeof (short) + sizeof (YYSTYPE) + sizeof (YYLTYPE))	\
      + 2 * YYSTACK_GAP_MAX)
# else
#  define YYSTACK_BYTES(N) \
     ((N) * (sizeof (short) + sizeof (YYSTYPE))				\
      + YYSTACK_GAP_MAX)
# endif

/* Copy COUNT objects from FROM to TO.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if 1 < __GNUC__
#   define YYCOPY(To, From, Count) \
      __builtin_memcpy (To, From, (Count) * sizeof (*(From)))
#  else
#   define YYCOPY(To, From, Count)		\
      do					\
	{					\
	  register YYSIZE_T yyi;		\
	  for (yyi = 0; yyi < (Count); yyi++)	\
	    (To)[yyi] = (From)[yyi];		\
	}					\
      while (0)
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
	yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAX;	\
	yyptr += yynewbytes / sizeof (*yyptr);				\
      }									\
    while (0)

#endif


#if ! defined (YYSIZE_T) && defined (__SIZE_TYPE__)
# define YYSIZE_T __SIZE_TYPE__
#endif
#if ! defined (YYSIZE_T) && defined (size_t)
# define YYSIZE_T size_t
#endif
#if ! defined (YYSIZE_T)
# if defined (__STDC__) || defined (__cplusplus)
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# endif
#endif
#if ! defined (YYSIZE_T)
# define YYSIZE_T unsigned int
#endif

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		-2
#define YYEOF		0
#define YYACCEPT	goto yyacceptlab
#define YYABORT 	goto yyabortlab
#define YYERROR		goto yyerrlab1
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
      yychar1 = YYTRANSLATE (yychar);				\
      YYPOPSTACK;						\
      goto yybackup;						\
    }								\
  else								\
    { 								\
      yyerror ("syntax error: cannot back up");			\
      YYERROR;							\
    }								\
while (0)

#define YYTERROR	1
#define YYERRCODE	256


/* YYLLOC_DEFAULT -- Compute the default location (before the actions
   are run).

   When YYLLOC_DEFAULT is run, CURRENT is set the location of the
   first token.  By default, to implement support for ranges, extend
   its range to the last symbol.  */

#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)       	\
   Current.last_line   = Rhs[N].last_line;	\
   Current.last_column = Rhs[N].last_column;
#endif


/* YYLEX -- calling `yylex' with the right arguments.  */

#if YYPURE
# if YYLSP_NEEDED
#  ifdef YYLEX_PARAM
#   define YYLEX		yylex (&yylval, &yylloc, YYLEX_PARAM)
#  else
#   define YYLEX		yylex (&yylval, &yylloc)
#  endif
# else /* !YYLSP_NEEDED */
#  ifdef YYLEX_PARAM
#   define YYLEX		yylex (&yylval, YYLEX_PARAM)
#  else
#   define YYLEX		yylex (&yylval)
#  endif
# endif /* !YYLSP_NEEDED */
#else /* !YYPURE */
# define YYLEX			yylex ()
#endif /* !YYPURE */


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
} while (0)
/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
#endif /* !YYDEBUG */

/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef	YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   SIZE_MAX < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#if YYMAXDEPTH == 0
# undef YYMAXDEPTH
#endif

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif

#ifdef YYERROR_VERBOSE

# ifndef yystrlen
#  if defined (__GLIBC__) && defined (_STRING_H)
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
static YYSIZE_T
#   if defined (__STDC__) || defined (__cplusplus)
yystrlen (const char *yystr)
#   else
yystrlen (yystr)
     const char *yystr;
#   endif
{
  register const char *yys = yystr;

  while (*yys++ != '\0')
    continue;

  return yys - yystr - 1;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined (__GLIBC__) && defined (_STRING_H) && defined (_GNU_SOURCE)
#   include <string.h>
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
static char *
#   if defined (__STDC__) || defined (__cplusplus)
yystpcpy (char *yydest, const char *yysrc)
#   else
yystpcpy (yydest, yysrc)
     char *yydest;
     const char *yysrc;
#   endif
{
  register char *yyd = yydest;
  register const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif
#endif

#line 316 "/usr/share/bison/bison.simple"


/* The user can define YYPARSE_PARAM as the name of an argument to be passed
   into yyparse.  The argument should have type void *.
   It should actually point to an object.
   Grammar actions can access the variable by casting it
   to the proper pointer type.  */

#ifdef YYPARSE_PARAM
# if defined (__STDC__) || defined (__cplusplus)
#  define YYPARSE_PARAM_ARG void *YYPARSE_PARAM
#  define YYPARSE_PARAM_DECL
# else
#  define YYPARSE_PARAM_ARG YYPARSE_PARAM
#  define YYPARSE_PARAM_DECL void *YYPARSE_PARAM;
# endif
#else /* !YYPARSE_PARAM */
# define YYPARSE_PARAM_ARG
# define YYPARSE_PARAM_DECL
#endif /* !YYPARSE_PARAM */

/* Prevent warning if -Wstrict-prototypes.  */
#ifdef __GNUC__
# ifdef YYPARSE_PARAM
int yyparse (void *);
# else
int yyparse (void);
# endif
#endif

/* YY_DECL_VARIABLES -- depending whether we use a pure parser,
   variables are global, or local to YYPARSE.  */

#define YY_DECL_NON_LSP_VARIABLES			\
/* The lookahead symbol.  */				\
int yychar;						\
							\
/* The semantic value of the lookahead symbol. */	\
YYSTYPE yylval;						\
							\
/* Number of parse errors so far.  */			\
int yynerrs;

#if YYLSP_NEEDED
# define YY_DECL_VARIABLES			\
YY_DECL_NON_LSP_VARIABLES			\
						\
/* Location data for the lookahead symbol.  */	\
YYLTYPE yylloc;
#else
# define YY_DECL_VARIABLES			\
YY_DECL_NON_LSP_VARIABLES
#endif


/* If nonreentrant, generate the variables here. */

#if !YYPURE
YY_DECL_VARIABLES
#endif  /* !YYPURE */

int
yyparse (YYPARSE_PARAM_ARG)
     YYPARSE_PARAM_DECL
{
  /* If reentrant, generate the variables here. */
#if YYPURE
  YY_DECL_VARIABLES
#endif  /* !YYPURE */

  register int yystate;
  register int yyn;
  int yyresult;
  /* Number of tokens to shift before error messages enabled.  */
  int yyerrstatus;
  /* Lookahead token as an internal (translated) token number.  */
  int yychar1 = 0;

  /* Three stacks and their tools:
     `yyss': related to states,
     `yyvs': related to semantic values,
     `yyls': related to locations.

     Refer to the stacks thru separate pointers, to allow yyoverflow
     to reallocate them elsewhere.  */

  /* The state stack. */
  short	yyssa[YYINITDEPTH];
  short *yyss = yyssa;
  register short *yyssp;

  /* The semantic value stack.  */
  YYSTYPE yyvsa[YYINITDEPTH];
  YYSTYPE *yyvs = yyvsa;
  register YYSTYPE *yyvsp;

#if YYLSP_NEEDED
  /* The location stack.  */
  YYLTYPE yylsa[YYINITDEPTH];
  YYLTYPE *yyls = yylsa;
  YYLTYPE *yylsp;
#endif

#if YYLSP_NEEDED
# define YYPOPSTACK   (yyvsp--, yyssp--, yylsp--)
#else
# define YYPOPSTACK   (yyvsp--, yyssp--)
#endif

  YYSIZE_T yystacksize = YYINITDEPTH;


  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;
#if YYLSP_NEEDED
  YYLTYPE yyloc;
#endif

  /* When reducing, the number of symbols on the RHS of the reduced
     rule. */
  int yylen;

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
#if YYLSP_NEEDED
  yylsp = yyls;
#endif
  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed. so pushing a state here evens the stacks.
     */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyssp >= yyss + yystacksize - 1)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
	/* Give user a chance to reallocate the stack. Use copies of
	   these so that the &'s don't force the real ones into
	   memory.  */
	YYSTYPE *yyvs1 = yyvs;
	short *yyss1 = yyss;

	/* Each stack pointer address is followed by the size of the
	   data in use in that stack, in bytes.  */
# if YYLSP_NEEDED
	YYLTYPE *yyls1 = yyls;
	/* This used to be a conditional around just the two extra args,
	   but that might be undefined if yyoverflow is a macro.  */
	yyoverflow ("parser stack overflow",
		    &yyss1, yysize * sizeof (*yyssp),
		    &yyvs1, yysize * sizeof (*yyvsp),
		    &yyls1, yysize * sizeof (*yylsp),
		    &yystacksize);
	yyls = yyls1;
# else
	yyoverflow ("parser stack overflow",
		    &yyss1, yysize * sizeof (*yyssp),
		    &yyvs1, yysize * sizeof (*yyvsp),
		    &yystacksize);
# endif
	yyss = yyss1;
	yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyoverflowlab;
# else
      /* Extend the stack our own way.  */
      if (yystacksize >= YYMAXDEPTH)
	goto yyoverflowlab;
      yystacksize *= 2;
      if (yystacksize > YYMAXDEPTH)
	yystacksize = YYMAXDEPTH;

      {
	short *yyss1 = yyss;
	union yyalloc *yyptr =
	  (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
	if (! yyptr)
	  goto yyoverflowlab;
	YYSTACK_RELOCATE (yyss);
	YYSTACK_RELOCATE (yyvs);
# if YYLSP_NEEDED
	YYSTACK_RELOCATE (yyls);
# endif
# undef YYSTACK_RELOCATE
	if (yyss1 != yyssa)
	  YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;
#if YYLSP_NEEDED
      yylsp = yyls + yysize - 1;
#endif

      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
		  (unsigned long int) yystacksize));

      if (yyssp >= yyss + yystacksize - 1)
	YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  goto yybackup;


/*-----------.
| yybackup.  |
`-----------*/
yybackup:

/* Do appropriate processing given the current state.  */
/* Read a lookahead token if we need one and don't already have one.  */
/* yyresume: */

  /* First try to decide what to do without reference to lookahead token.  */

  yyn = yypact[yystate];
  if (yyn == YYFLAG)
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* yychar is either YYEMPTY or YYEOF
     or a valid token in external form.  */

  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = YYLEX;
    }

  /* Convert token to internal form (in yychar1) for indexing tables with */

  if (yychar <= 0)		/* This means end of input. */
    {
      yychar1 = 0;
      yychar = YYEOF;		/* Don't call YYLEX any more */

      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yychar1 = YYTRANSLATE (yychar);

#if YYDEBUG
     /* We have to keep this `#if YYDEBUG', since we use variables
	which are defined only if `YYDEBUG' is set.  */
      if (yydebug)
	{
	  YYFPRINTF (stderr, "Next token is %d (%s",
		     yychar, yytname[yychar1]);
	  /* Give the individual parser a way to print the precise
	     meaning of a token, for further debugging info.  */
# ifdef YYPRINT
	  YYPRINT (stderr, yychar, yylval);
# endif
	  YYFPRINTF (stderr, ")\n");
	}
#endif
    }

  yyn += yychar1;
  if (yyn < 0 || yyn > YYLAST || yycheck[yyn] != yychar1)
    goto yydefault;

  yyn = yytable[yyn];

  /* yyn is what to do for this token type in this state.
     Negative => reduce, -yyn is rule number.
     Positive => shift, yyn is new state.
       New state is final state => don't bother to shift,
       just return success.
     0, or most negative number => error.  */

  if (yyn < 0)
    {
      if (yyn == YYFLAG)
	goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }
  else if (yyn == 0)
    goto yyerrlab;

  if (yyn == YYFINAL)
    YYACCEPT;

  /* Shift the lookahead token.  */
  YYDPRINTF ((stderr, "Shifting token %d (%s), ",
	      yychar, yytname[yychar1]));

  /* Discard the token being shifted unless it is eof.  */
  if (yychar != YYEOF)
    yychar = YYEMPTY;

  *++yyvsp = yylval;
#if YYLSP_NEEDED
  *++yylsp = yylloc;
#endif

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  yystate = yyn;
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

     Otherwise, the following line sets YYVAL to the semantic value of
     the lookahead token.  This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];

#if YYLSP_NEEDED
  /* Similarly for the default location.  Let the user run additional
     commands if for instance locations are ranges.  */
  yyloc = yylsp[1-yylen];
  YYLLOC_DEFAULT (yyloc, (yylsp - yylen), yylen);
#endif

#if YYDEBUG
  /* We have to keep this `#if YYDEBUG', since we use variables which
     are defined only if `YYDEBUG' is set.  */
  if (yydebug)
    {
      int yyi;

      YYFPRINTF (stderr, "Reducing via rule %d (line %d), ",
		 yyn, yyrline[yyn]);

      /* Print the symbols being reduced, and their result.  */
      for (yyi = yyprhs[yyn]; yyrhs[yyi] > 0; yyi++)
	YYFPRINTF (stderr, "%s ", yytname[yyrhs[yyi]]);
      YYFPRINTF (stderr, " -> %s\n", yytname[yyr1[yyn]]);
    }
#endif

  switch (yyn) {

case 1:
#line 346 "parse.y"
{
			lex_state = EXPR_BEG;
                        top_local_init();
			if (ruby_class == rb_cObject) class_nest = 0;
			else class_nest = 1;
		    }
    break;
case 2:
#line 353 "parse.y"
{
			if (yyvsp[0].node && !compile_for_eval) {
                            /* last expression should not be void */
			    if (nd_type(yyvsp[0].node) != NODE_BLOCK) void_expr(yyvsp[0].node);
			    else {
				NODE *node = yyvsp[0].node;
				while (node->nd_next) {
				    node = node->nd_next;
				}
				void_expr(node->nd_head);
			    }
			}
			ruby_eval_tree = block_append(ruby_eval_tree, yyvsp[0].node);
                        top_local_setup();
			class_nest = 0;
		    }
    break;
case 3:
#line 375 "parse.y"
{
		        yyval.node = yyvsp[-3].node;
			if (yyvsp[-2].node) {
			    yyval.node = NEW_RESCUE(yyvsp[-3].node, yyvsp[-2].node, yyvsp[-1].node);
			}
			else if (yyvsp[-1].node) {
			    rb_warn("else without rescue is useless");
			    yyval.node = block_append(yyval.node, yyvsp[-1].node);
			}
			if (yyvsp[0].node) {
			    yyval.node = NEW_ENSURE(yyval.node, yyvsp[0].node);
			}
			fixpos(yyval.node, yyvsp[-3].node);
		    }
    break;
case 4:
#line 392 "parse.y"
{
			void_stmts(yyvsp[-1].node);
		        yyval.node = yyvsp[-1].node;
		    }
    break;
case 6:
#line 400 "parse.y"
{
			yyval.node = newline_node(yyvsp[0].node);
		    }
    break;
case 7:
#line 404 "parse.y"
{
			yyval.node = block_append(yyvsp[-2].node, newline_node(yyvsp[0].node));
		    }
    break;
case 8:
#line 408 "parse.y"
{
			yyval.node = yyvsp[0].node;
		    }
    break;
case 9:
#line 413 "parse.y"
{lex_state = EXPR_FNAME;}
    break;
case 10:
#line 414 "parse.y"
{
		        yyval.node = NEW_ALIAS(yyvsp[-2].node, yyvsp[0].node);
		    }
    break;
case 11:
#line 418 "parse.y"
{
		        yyval.node = NEW_VALIAS(yyvsp[-1].id, yyvsp[0].id);
		    }
    break;
case 12:
#line 422 "parse.y"
{
			char buf[3];

			sprintf(buf, "$%c", (char)yyvsp[0].node->nd_nth);
		        yyval.node = NEW_VALIAS(yyvsp[-1].id, rb_intern(buf));
		    }
    break;
case 13:
#line 429 "parse.y"
{
		        yyerror("can't make alias for the number variables");
		        yyval.node = 0;
		    }
    break;
case 14:
#line 434 "parse.y"
{
			yyval.node = yyvsp[0].node;
		    }
    break;
case 15:
#line 438 "parse.y"
{
			yyval.node = NEW_IF(cond(yyvsp[0].node), yyvsp[-2].node, 0);
		        fixpos(yyval.node, yyvsp[0].node);
			if (cond_negative(&yyval.node->nd_cond)) {
		            yyval.node->nd_else = yyval.node->nd_body;
		            yyval.node->nd_body = 0;
			}
		    }
    break;
case 16:
#line 447 "parse.y"
{
			yyval.node = NEW_UNLESS(cond(yyvsp[0].node), yyvsp[-2].node, 0);
		        fixpos(yyval.node, yyvsp[0].node);
			if (cond_negative(&yyval.node->nd_cond)) {
		            yyval.node->nd_body = yyval.node->nd_else;
		            yyval.node->nd_else = 0;
			}
		    }
    break;
case 17:
#line 456 "parse.y"
{
			if (yyvsp[-2].node && nd_type(yyvsp[-2].node) == NODE_BEGIN) {
			    yyval.node = NEW_WHILE(cond(yyvsp[0].node), yyvsp[-2].node->nd_body, 0);
			}
			else {
			    yyval.node = NEW_WHILE(cond(yyvsp[0].node), yyvsp[-2].node, 1);
			}
			if (cond_negative(&yyval.node->nd_cond)) {
			    nd_set_type(yyval.node, NODE_UNTIL);
			}
		    }
    break;
case 18:
#line 468 "parse.y"
{
			if (yyvsp[-2].node && nd_type(yyvsp[-2].node) == NODE_BEGIN) {
			    yyval.node = NEW_UNTIL(cond(yyvsp[0].node), yyvsp[-2].node->nd_body, 0);
			}
			else {
			    yyval.node = NEW_UNTIL(cond(yyvsp[0].node), yyvsp[-2].node, 1);
			}
			if (cond_negative(&yyval.node->nd_cond)) {
			    nd_set_type(yyval.node, NODE_WHILE);
			}
		    }
    break;
case 19:
#line 480 "parse.y"
{
			yyval.node = NEW_RESCUE(yyvsp[-2].node, NEW_RESBODY(0,yyvsp[0].node,0), 0);
		    }
    break;
case 20:
#line 484 "parse.y"
{
			if (in_def || in_single) {
			    yyerror("BEGIN in method");
			}
			local_push(0);
		    }
    break;
case 21:
#line 491 "parse.y"
{
			ruby_eval_tree_begin = block_append(ruby_eval_tree_begin,
						            NEW_PREEXE(yyvsp[-1].node));
		        local_pop();
		        yyval.node = 0;
		    }
    break;
case 22:
#line 498 "parse.y"
{
			if (in_def || in_single) {
			    rb_warn("END in method; use at_exit");
			}

			yyval.node = NEW_ITER(0, NEW_POSTEXE(), yyvsp[-1].node);
		    }
    break;
case 23:
#line 506 "parse.y"
{
			yyval.node = node_assign(yyvsp[-2].node, yyvsp[0].node);
		    }
    break;
case 24:
#line 510 "parse.y"
{
			value_expr(yyvsp[0].node);
			yyvsp[-2].node->nd_value = (yyvsp[-2].node->nd_head) ? NEW_TO_ARY(yyvsp[0].node) : NEW_ARRAY(yyvsp[0].node);
			yyval.node = yyvsp[-2].node;
		    }
    break;
case 25:
#line 516 "parse.y"
{
			value_expr(yyvsp[0].node);
			if (yyvsp[-2].node) {
			    ID vid = yyvsp[-2].node->nd_vid;
			    if (yyvsp[-1].id == tOROP) {
				yyvsp[-2].node->nd_value = yyvsp[0].node;
				yyval.node = NEW_OP_ASGN_OR(gettable(vid), yyvsp[-2].node);
				if (is_asgn_or_id(vid)) {
				    yyval.node->nd_aid = vid;
				}
			    }
			    else if (yyvsp[-1].id == tANDOP) {
				yyvsp[-2].node->nd_value = yyvsp[0].node;
				yyval.node = NEW_OP_ASGN_AND(gettable(vid), yyvsp[-2].node);
			    }
			    else {
				yyval.node = yyvsp[-2].node;
				yyval.node->nd_value = call_op(gettable(vid),yyvsp[-1].id,1,yyvsp[0].node);
			    }
			}
			else {
			    yyval.node = 0;
			}
		    }
    break;
case 26:
#line 541 "parse.y"
{
                        NODE *args;

			value_expr(yyvsp[0].node);
			args = arg_concat(yyvsp[0].node, yyvsp[-3].node);
			if (yyvsp[-1].id == tOROP) {
			    yyvsp[-1].id = 0;
			}
			else if (yyvsp[-1].id == tANDOP) {
			    yyvsp[-1].id = 1;
			}
			yyval.node = NEW_OP_ASGN1(yyvsp[-5].node, yyvsp[-1].id, args);
		        fixpos(yyval.node, yyvsp[-5].node);
		    }
    break;
case 27:
#line 556 "parse.y"
{
			value_expr(yyvsp[0].node);
			if (yyvsp[-1].id == tOROP) {
			    yyvsp[-1].id = 0;
			}
			else if (yyvsp[-1].id == tANDOP) {
			    yyvsp[-1].id = 1;
			}
			yyval.node = NEW_OP_ASGN2(yyvsp[-4].node, yyvsp[-2].id, yyvsp[-1].id, yyvsp[0].node);
		        fixpos(yyval.node, yyvsp[-4].node);
		    }
    break;
case 28:
#line 568 "parse.y"
{
			value_expr(yyvsp[0].node);
			if (yyvsp[-1].id == tOROP) {
			    yyvsp[-1].id = 0;
			}
			else if (yyvsp[-1].id == tANDOP) {
			    yyvsp[-1].id = 1;
			}
			yyval.node = NEW_OP_ASGN2(yyvsp[-4].node, yyvsp[-2].id, yyvsp[-1].id, yyvsp[0].node);
		        fixpos(yyval.node, yyvsp[-4].node);
		    }
    break;
case 29:
#line 580 "parse.y"
{
			value_expr(yyvsp[0].node);
			if (yyvsp[-1].id == tOROP) {
			    yyvsp[-1].id = 0;
			}
			else if (yyvsp[-1].id == tANDOP) {
			    yyvsp[-1].id = 1;
			}
			yyval.node = NEW_OP_ASGN2(yyvsp[-4].node, yyvsp[-2].id, yyvsp[-1].id, yyvsp[0].node);
		        fixpos(yyval.node, yyvsp[-4].node);
		    }
    break;
case 30:
#line 592 "parse.y"
{
		        rb_backref_error(yyvsp[-2].node);
			yyval.node = 0;
		    }
    break;
case 31:
#line 597 "parse.y"
{
			yyval.node = node_assign(yyvsp[-2].node, NEW_SVALUE(yyvsp[0].node));
		    }
    break;
case 32:
#line 601 "parse.y"
{
			yyvsp[-2].node->nd_value = (yyvsp[-2].node->nd_head) ? NEW_TO_ARY(yyvsp[0].node) : NEW_ARRAY(yyvsp[0].node);
			yyval.node = yyvsp[-2].node;
		    }
    break;
case 33:
#line 606 "parse.y"
{
			yyvsp[-2].node->nd_value = yyvsp[0].node;
			yyval.node = yyvsp[-2].node;
		    }
    break;
case 36:
#line 615 "parse.y"
{
			yyval.node = logop(NODE_AND, yyvsp[-2].node, yyvsp[0].node);
		    }
    break;
case 37:
#line 619 "parse.y"
{
			yyval.node = logop(NODE_OR, yyvsp[-2].node, yyvsp[0].node);
		    }
    break;
case 38:
#line 623 "parse.y"
{
			yyval.node = NEW_NOT(cond(yyvsp[0].node));
		    }
    break;
case 39:
#line 627 "parse.y"
{
			yyval.node = NEW_NOT(cond(yyvsp[0].node));
		    }
    break;
case 41:
#line 634 "parse.y"
{
			value_expr(yyval.node);
			yyval.node = yyvsp[0].node;
		    }
    break;
case 44:
#line 643 "parse.y"
{
			yyval.node = NEW_RETURN(ret_args(yyvsp[0].node));
		    }
    break;
case 45:
#line 647 "parse.y"
{
			yyval.node = NEW_BREAK(ret_args(yyvsp[0].node));
		    }
    break;
case 46:
#line 651 "parse.y"
{
			yyval.node = NEW_NEXT(ret_args(yyvsp[0].node));
		    }
    break;
case 48:
#line 658 "parse.y"
{
			yyval.node = new_call(yyvsp[-3].node, yyvsp[-1].id, yyvsp[0].node);
		    }
    break;
case 49:
#line 662 "parse.y"
{
			yyval.node = new_call(yyvsp[-3].node, yyvsp[-1].id, yyvsp[0].node);
		    }
    break;
case 50:
#line 668 "parse.y"
{
			yyval.vars = dyna_push();
			yyvsp[0].num = ruby_sourceline;
		    }
    break;
case 51:
#line 672 "parse.y"
{yyval.vars = ruby_dyna_vars;}
    break;
case 52:
#line 675 "parse.y"
{
			yyval.node = NEW_ITER(yyvsp[-3].node, 0, dyna_init(yyvsp[-1].node, yyvsp[-2].vars));
			nd_set_line(yyval.node, yyvsp[-5].num);
			dyna_pop(yyvsp[-4].vars);
		    }
    break;
case 53:
#line 683 "parse.y"
{
			yyval.node = new_fcall(yyvsp[-1].id, yyvsp[0].node);
		        fixpos(yyval.node, yyvsp[0].node);
		   }
    break;
case 54:
#line 688 "parse.y"
{
			yyval.node = new_fcall(yyvsp[-2].id, yyvsp[-1].node);
			if (yyvsp[0].node) {
			    if (nd_type(yyval.node) == NODE_BLOCK_PASS) {
				rb_compile_error("both block arg and actual block given");
			    }
			    yyvsp[0].node->nd_iter = yyval.node;
			    yyval.node = yyvsp[0].node;
			}
		        fixpos(yyval.node, yyvsp[-1].node);
		   }
    break;
case 55:
#line 700 "parse.y"
{
			yyval.node = new_call(yyvsp[-3].node, yyvsp[-1].id, yyvsp[0].node);
		        fixpos(yyval.node, yyvsp[-3].node);
		    }
    break;
case 56:
#line 705 "parse.y"
{
			yyval.node = new_call(yyvsp[-4].node, yyvsp[-2].id, yyvsp[-1].node);
			if (yyvsp[0].node) {
			    if (nd_type(yyval.node) == NODE_BLOCK_PASS) {
				rb_compile_error("both block arg and actual block given");
			    }
			    yyvsp[0].node->nd_iter = yyval.node;
			    yyval.node = yyvsp[0].node;
			}
		        fixpos(yyval.node, yyvsp[-4].node);
		   }
    break;
case 57:
#line 717 "parse.y"
{
			yyval.node = new_call(yyvsp[-3].node, yyvsp[-1].id, yyvsp[0].node);
		        fixpos(yyval.node, yyvsp[-3].node);
		    }
    break;
case 58:
#line 722 "parse.y"
{
			yyval.node = new_call(yyvsp[-4].node, yyvsp[-2].id, yyvsp[-1].node);
			if (yyvsp[0].node) {
			    if (nd_type(yyval.node) == NODE_BLOCK_PASS) {
				rb_compile_error("both block arg and actual block given");
			    }
			    yyvsp[0].node->nd_iter = yyval.node;
			    yyval.node = yyvsp[0].node;
			}
		        fixpos(yyval.node, yyvsp[-4].node);
		   }
    break;
case 59:
#line 734 "parse.y"
{
			yyval.node = new_super(yyvsp[0].node);
		        fixpos(yyval.node, yyvsp[0].node);
		    }
    break;
case 60:
#line 739 "parse.y"
{
			yyval.node = new_yield(yyvsp[0].node);
		        fixpos(yyval.node, yyvsp[0].node);
		    }
    break;
case 62:
#line 747 "parse.y"
{
			yyval.node = yyvsp[-1].node;
		    }
    break;
case 64:
#line 754 "parse.y"
{
			yyval.node = NEW_MASGN(NEW_LIST(yyvsp[-1].node), 0);
		    }
    break;
case 65:
#line 760 "parse.y"
{
			yyval.node = NEW_MASGN(yyvsp[0].node, 0);
		    }
    break;
case 66:
#line 764 "parse.y"
{
			yyval.node = NEW_MASGN(list_append(yyvsp[-1].node,yyvsp[0].node), 0);
		    }
    break;
case 67:
#line 768 "parse.y"
{
			yyval.node = NEW_MASGN(yyvsp[-2].node, yyvsp[0].node);
		    }
    break;
case 68:
#line 772 "parse.y"
{
			yyval.node = NEW_MASGN(yyvsp[-1].node, -1);
		    }
    break;
case 69:
#line 776 "parse.y"
{
			yyval.node = NEW_MASGN(0, yyvsp[0].node);
		    }
    break;
case 70:
#line 780 "parse.y"
{
			yyval.node = NEW_MASGN(0, -1);
		    }
    break;
case 72:
#line 787 "parse.y"
{
			yyval.node = yyvsp[-1].node;
		    }
    break;
case 73:
#line 793 "parse.y"
{
			yyval.node = NEW_LIST(yyvsp[-1].node);
		    }
    break;
case 74:
#line 797 "parse.y"
{
			yyval.node = list_append(yyvsp[-2].node, yyvsp[-1].node);
		    }
    break;
case 75:
#line 803 "parse.y"
{
			yyval.node = assignable(yyvsp[0].id, 0);
		    }
    break;
case 76:
#line 807 "parse.y"
{
			yyval.node = aryset(yyvsp[-3].node, yyvsp[-1].node);
		    }
    break;
case 77:
#line 811 "parse.y"
{
			yyval.node = attrset(yyvsp[-2].node, yyvsp[0].id);
		    }
    break;
case 78:
#line 815 "parse.y"
{
			yyval.node = attrset(yyvsp[-2].node, yyvsp[0].id);
		    }
    break;
case 79:
#line 819 "parse.y"
{
			yyval.node = attrset(yyvsp[-2].node, yyvsp[0].id);
		    }
    break;
case 80:
#line 823 "parse.y"
{
			if (in_def || in_single)
			    yyerror("dynamic constant assignment");
			yyval.node = NEW_CDECL(0, 0, NEW_COLON2(yyvsp[-2].node, yyvsp[0].id));
		    }
    break;
case 81:
#line 829 "parse.y"
{
			if (in_def || in_single)
			    yyerror("dynamic constant assignment");
			yyval.node = NEW_CDECL(0, 0, NEW_COLON3(yyvsp[0].id));
		    }
    break;
case 82:
#line 835 "parse.y"
{
		        rb_backref_error(yyvsp[0].node);
			yyval.node = 0;
		    }
    break;
case 83:
#line 842 "parse.y"
{
			yyval.node = assignable(yyvsp[0].id, 0);
		    }
    break;
case 84:
#line 846 "parse.y"
{
			yyval.node = aryset(yyvsp[-3].node, yyvsp[-1].node);
		    }
    break;
case 85:
#line 850 "parse.y"
{
			yyval.node = attrset(yyvsp[-2].node, yyvsp[0].id);
		    }
    break;
case 86:
#line 854 "parse.y"
{
			yyval.node = attrset(yyvsp[-2].node, yyvsp[0].id);
		    }
    break;
case 87:
#line 858 "parse.y"
{
			yyval.node = attrset(yyvsp[-2].node, yyvsp[0].id);
		    }
    break;
case 88:
#line 862 "parse.y"
{
			if (in_def || in_single)
			    yyerror("dynamic constant assignment");
			yyval.node = NEW_CDECL(0, 0, NEW_COLON2(yyvsp[-2].node, yyvsp[0].id));
		    }
    break;
case 89:
#line 868 "parse.y"
{
			if (in_def || in_single)
			    yyerror("dynamic constant assignment");
			yyval.node = NEW_CDECL(0, 0, NEW_COLON3(yyvsp[0].id));
		    }
    break;
case 90:
#line 874 "parse.y"
{
		        rb_backref_error(yyvsp[0].node);
			yyval.node = 0;
		    }
    break;
case 91:
#line 881 "parse.y"
{
			yyerror("class/module name must be CONSTANT");
		    }
    break;
case 93:
#line 888 "parse.y"
{
			yyval.node = NEW_COLON3(yyvsp[0].id);
		    }
    break;
case 94:
#line 892 "parse.y"
{
			yyval.node = NEW_COLON2(0, yyval.node);
		    }
    break;
case 95:
#line 896 "parse.y"
{
			yyval.node = NEW_COLON2(yyvsp[-2].node, yyvsp[0].id);
		    }
    break;
case 99:
#line 905 "parse.y"
{
			lex_state = EXPR_END;
			yyval.id = yyvsp[0].id;
		    }
    break;
case 100:
#line 910 "parse.y"
{
			lex_state = EXPR_END;
			yyval.id = yyvsp[0].id;
		    }
    break;
case 103:
#line 921 "parse.y"
{
			yyval.node = NEW_LIT(ID2SYM(yyvsp[0].id));
		    }
    break;
case 105:
#line 928 "parse.y"
{
			yyval.node = NEW_UNDEF(yyvsp[0].node);
		    }
    break;
case 106:
#line 931 "parse.y"
{lex_state = EXPR_FNAME;}
    break;
case 107:
#line 932 "parse.y"
{
			yyval.node = block_append(yyvsp[-3].node, NEW_UNDEF(yyvsp[0].node));
		    }
    break;
case 108:
#line 937 "parse.y"
{ yyval.id = '|'; }
    break;
case 109:
#line 938 "parse.y"
{ yyval.id = '^'; }
    break;
case 110:
#line 939 "parse.y"
{ yyval.id = '&'; }
    break;
case 111:
#line 940 "parse.y"
{ yyval.id = tCMP; }
    break;
case 112:
#line 941 "parse.y"
{ yyval.id = tEQ; }
    break;
case 113:
#line 942 "parse.y"
{ yyval.id = tEQQ; }
    break;
case 114:
#line 943 "parse.y"
{ yyval.id = tMATCH; }
    break;
case 115:
#line 944 "parse.y"
{ yyval.id = '>'; }
    break;
case 116:
#line 945 "parse.y"
{ yyval.id = tGEQ; }
    break;
case 117:
#line 946 "parse.y"
{ yyval.id = '<'; }
    break;
case 118:
#line 947 "parse.y"
{ yyval.id = tLEQ; }
    break;
case 119:
#line 948 "parse.y"
{ yyval.id = tLSHFT; }
    break;
case 120:
#line 949 "parse.y"
{ yyval.id = tRSHFT; }
    break;
case 121:
#line 950 "parse.y"
{ yyval.id = '+'; }
    break;
case 122:
#line 951 "parse.y"
{ yyval.id = '-'; }
    break;
case 123:
#line 952 "parse.y"
{ yyval.id = '*'; }
    break;
case 124:
#line 953 "parse.y"
{ yyval.id = '*'; }
    break;
case 125:
#line 954 "parse.y"
{ yyval.id = '/'; }
    break;
case 126:
#line 955 "parse.y"
{ yyval.id = '%'; }
    break;
case 127:
#line 956 "parse.y"
{ yyval.id = tPOW; }
    break;
case 128:
#line 957 "parse.y"
{ yyval.id = '~'; }
    break;
case 129:
#line 958 "parse.y"
{ yyval.id = tUPLUS; }
    break;
case 130:
#line 959 "parse.y"
{ yyval.id = tUMINUS; }
    break;
case 131:
#line 960 "parse.y"
{ yyval.id = tAREF; }
    break;
case 132:
#line 961 "parse.y"
{ yyval.id = tASET; }
    break;
case 133:
#line 962 "parse.y"
{ yyval.id = '`'; }
    break;
case 174:
#line 975 "parse.y"
{
			yyval.node = node_assign(yyvsp[-2].node, yyvsp[0].node);
		    }
    break;
case 175:
#line 979 "parse.y"
{
			yyval.node = node_assign(yyvsp[-4].node, NEW_RESCUE(yyvsp[-2].node, NEW_RESBODY(0,yyvsp[0].node,0), 0));
		    }
    break;
case 176:
#line 983 "parse.y"
{
			value_expr(yyvsp[0].node);
			if (yyvsp[-2].node) {
			    ID vid = yyvsp[-2].node->nd_vid;
			    if (yyvsp[-1].id == tOROP) {
				yyvsp[-2].node->nd_value = yyvsp[0].node;
				yyval.node = NEW_OP_ASGN_OR(gettable(vid), yyvsp[-2].node);
				if (is_asgn_or_id(vid)) {
				    yyval.node->nd_aid = vid;
				}
			    }
			    else if (yyvsp[-1].id == tANDOP) {
				yyvsp[-2].node->nd_value = yyvsp[0].node;
				yyval.node = NEW_OP_ASGN_AND(gettable(vid), yyvsp[-2].node);
			    }
			    else {
				yyval.node = yyvsp[-2].node;
				yyval.node->nd_value = call_op(gettable(vid),yyvsp[-1].id,1,yyvsp[0].node);
			    }
			}
			else {
			    yyval.node = 0;
			}
		    }
    break;
case 177:
#line 1008 "parse.y"
{
                        NODE *args;

			value_expr(yyvsp[0].node);
			args = arg_concat(yyvsp[0].node, yyvsp[-3].node);
			if (yyvsp[-1].id == tOROP) {
			    yyvsp[-1].id = 0;
			}
			else if (yyvsp[-1].id == tANDOP) {
			    yyvsp[-1].id = 1;
			}
			yyval.node = NEW_OP_ASGN1(yyvsp[-5].node, yyvsp[-1].id, args);
		        fixpos(yyval.node, yyvsp[-5].node);
		    }
    break;
case 178:
#line 1023 "parse.y"
{
			value_expr(yyvsp[0].node);
			if (yyvsp[-1].id == tOROP) {
			    yyvsp[-1].id = 0;
			}
			else if (yyvsp[-1].id == tANDOP) {
			    yyvsp[-1].id = 1;
			}
			yyval.node = NEW_OP_ASGN2(yyvsp[-4].node, yyvsp[-2].id, yyvsp[-1].id, yyvsp[0].node);
		        fixpos(yyval.node, yyvsp[-4].node);
		    }
    break;
case 179:
#line 1035 "parse.y"
{
			value_expr(yyvsp[0].node);
			if (yyvsp[-1].id == tOROP) {
			    yyvsp[-1].id = 0;
			}
			else if (yyvsp[-1].id == tANDOP) {
			    yyvsp[-1].id = 1;
			}
			yyval.node = NEW_OP_ASGN2(yyvsp[-4].node, yyvsp[-2].id, yyvsp[-1].id, yyvsp[0].node);
		        fixpos(yyval.node, yyvsp[-4].node);
		    }
    break;
case 180:
#line 1047 "parse.y"
{
			value_expr(yyvsp[0].node);
			if (yyvsp[-1].id == tOROP) {
			    yyvsp[-1].id = 0;
			}
			else if (yyvsp[-1].id == tANDOP) {
			    yyvsp[-1].id = 1;
			}
			yyval.node = NEW_OP_ASGN2(yyvsp[-4].node, yyvsp[-2].id, yyvsp[-1].id, yyvsp[0].node);
		        fixpos(yyval.node, yyvsp[-4].node);
		    }
    break;
case 181:
#line 1059 "parse.y"
{
			yyerror("constant re-assignment");
			yyval.node = 0;
		    }
    break;
case 182:
#line 1064 "parse.y"
{
			yyerror("constant re-assignment");
			yyval.node = 0;
		    }
    break;
case 183:
#line 1069 "parse.y"
{
		        rb_backref_error(yyvsp[-2].node);
			yyval.node = 0;
		    }
    break;
case 184:
#line 1074 "parse.y"
{
			value_expr(yyvsp[-2].node);
			value_expr(yyvsp[0].node);
			if (nd_type(yyvsp[-2].node) == NODE_LIT && FIXNUM_P(yyvsp[-2].node->nd_lit) &&
			    nd_type(yyvsp[0].node) == NODE_LIT && FIXNUM_P(yyvsp[0].node->nd_lit)) {
			    yyvsp[-2].node->nd_lit = rb_range_new(yyvsp[-2].node->nd_lit, yyvsp[0].node->nd_lit, Qfalse);
			    yyval.node = yyvsp[-2].node;
			}
			else {
			    yyval.node = NEW_DOT2(yyvsp[-2].node, yyvsp[0].node);
			}
		    }
    break;
case 185:
#line 1087 "parse.y"
{
			value_expr(yyvsp[-2].node);
			value_expr(yyvsp[0].node);
			if (nd_type(yyvsp[-2].node) == NODE_LIT && FIXNUM_P(yyvsp[-2].node->nd_lit) &&
			    nd_type(yyvsp[0].node) == NODE_LIT && FIXNUM_P(yyvsp[0].node->nd_lit)) {
			    yyvsp[-2].node->nd_lit = rb_range_new(yyvsp[-2].node->nd_lit, yyvsp[0].node->nd_lit, Qtrue);
			    yyval.node = yyvsp[-2].node;
			}
			else {
			    yyval.node = NEW_DOT3(yyvsp[-2].node, yyvsp[0].node);
			}
		    }
    break;
case 186:
#line 1100 "parse.y"
{
			yyval.node = call_op(yyvsp[-2].node, '+', 1, yyvsp[0].node);
		    }
    break;
case 187:
#line 1104 "parse.y"
{
		        yyval.node = call_op(yyvsp[-2].node, '-', 1, yyvsp[0].node);
		    }
    break;
case 188:
#line 1108 "parse.y"
{
		        yyval.node = call_op(yyvsp[-2].node, '*', 1, yyvsp[0].node);
		    }
    break;
case 189:
#line 1112 "parse.y"
{
			yyval.node = call_op(yyvsp[-2].node, '/', 1, yyvsp[0].node);
		    }
    break;
case 190:
#line 1116 "parse.y"
{
			yyval.node = call_op(yyvsp[-2].node, '%', 1, yyvsp[0].node);
		    }
    break;
case 191:
#line 1120 "parse.y"
{
			yyval.node = call_op(yyvsp[-2].node, tPOW, 1, yyvsp[0].node);
		    }
    break;
case 192:
#line 1124 "parse.y"
{
			yyval.node = call_op(call_op(yyvsp[-2].node, tPOW, 1, yyvsp[0].node), tUMINUS, 0, 0);
		    }
    break;
case 193:
#line 1128 "parse.y"
{
			yyval.node = call_op(call_op(yyvsp[-2].node, tPOW, 1, yyvsp[0].node), tUMINUS, 0, 0);
		    }
    break;
case 194:
#line 1132 "parse.y"
{
			if (yyvsp[0].node && nd_type(yyvsp[0].node) == NODE_LIT) {
			    yyval.node = yyvsp[0].node;
			}
			else {
			    yyval.node = call_op(yyvsp[0].node, tUPLUS, 0, 0);
			}
		    }
    break;
case 195:
#line 1141 "parse.y"
{
			yyval.node = call_op(yyvsp[0].node, tUMINUS, 0, 0);
		    }
    break;
case 196:
#line 1145 "parse.y"
{
		        yyval.node = call_op(yyvsp[-2].node, '|', 1, yyvsp[0].node);
		    }
    break;
case 197:
#line 1149 "parse.y"
{
			yyval.node = call_op(yyvsp[-2].node, '^', 1, yyvsp[0].node);
		    }
    break;
case 198:
#line 1153 "parse.y"
{
			yyval.node = call_op(yyvsp[-2].node, '&', 1, yyvsp[0].node);
		    }
    break;
case 199:
#line 1157 "parse.y"
{
			yyval.node = call_op(yyvsp[-2].node, tCMP, 1, yyvsp[0].node);
		    }
    break;
case 200:
#line 1161 "parse.y"
{
			yyval.node = call_op(yyvsp[-2].node, '>', 1, yyvsp[0].node);
		    }
    break;
case 201:
#line 1165 "parse.y"
{
			yyval.node = call_op(yyvsp[-2].node, tGEQ, 1, yyvsp[0].node);
		    }
    break;
case 202:
#line 1169 "parse.y"
{
			yyval.node = call_op(yyvsp[-2].node, '<', 1, yyvsp[0].node);
		    }
    break;
case 203:
#line 1173 "parse.y"
{
			yyval.node = call_op(yyvsp[-2].node, tLEQ, 1, yyvsp[0].node);
		    }
    break;
case 204:
#line 1177 "parse.y"
{
			yyval.node = call_op(yyvsp[-2].node, tEQ, 1, yyvsp[0].node);
		    }
    break;
case 205:
#line 1181 "parse.y"
{
			yyval.node = call_op(yyvsp[-2].node, tEQQ, 1, yyvsp[0].node);
		    }
    break;
case 206:
#line 1185 "parse.y"
{
			yyval.node = NEW_NOT(call_op(yyvsp[-2].node, tEQ, 1, yyvsp[0].node));
		    }
    break;
case 207:
#line 1189 "parse.y"
{
			yyval.node = match_gen(yyvsp[-2].node, yyvsp[0].node);
		    }
    break;
case 208:
#line 1193 "parse.y"
{
			yyval.node = NEW_NOT(match_gen(yyvsp[-2].node, yyvsp[0].node));
		    }
    break;
case 209:
#line 1197 "parse.y"
{
			yyval.node = NEW_NOT(cond(yyvsp[0].node));
		    }
    break;
case 210:
#line 1201 "parse.y"
{
			yyval.node = call_op(yyvsp[0].node, '~', 0, 0);
		    }
    break;
case 211:
#line 1205 "parse.y"
{
			yyval.node = call_op(yyvsp[-2].node, tLSHFT, 1, yyvsp[0].node);
		    }
    break;
case 212:
#line 1209 "parse.y"
{
			yyval.node = call_op(yyvsp[-2].node, tRSHFT, 1, yyvsp[0].node);
		    }
    break;
case 213:
#line 1213 "parse.y"
{
			yyval.node = logop(NODE_AND, yyvsp[-2].node, yyvsp[0].node);
		    }
    break;
case 214:
#line 1217 "parse.y"
{
			yyval.node = logop(NODE_OR, yyvsp[-2].node, yyvsp[0].node);
		    }
    break;
case 215:
#line 1220 "parse.y"
{in_defined = 1;}
    break;
case 216:
#line 1221 "parse.y"
{
		        in_defined = 0;
			yyval.node = NEW_DEFINED(yyvsp[0].node);
		    }
    break;
case 217:
#line 1226 "parse.y"
{
			yyval.node = NEW_IF(cond(yyvsp[-4].node), yyvsp[-2].node, yyvsp[0].node);
		        fixpos(yyval.node, yyvsp[-4].node);
		    }
    break;
case 218:
#line 1231 "parse.y"
{
			yyval.node = yyvsp[0].node;
		    }
    break;
case 219:
#line 1237 "parse.y"
{
			value_expr(yyvsp[0].node);
			yyval.node = yyvsp[0].node;
		    }
    break;
case 221:
#line 1245 "parse.y"
{
		        rb_warn("parenthesize argument(s) for future version");
			yyval.node = NEW_LIST(yyvsp[-1].node);
		    }
    break;
case 222:
#line 1250 "parse.y"
{
			yyval.node = yyvsp[-1].node;
		    }
    break;
case 223:
#line 1254 "parse.y"
{
			value_expr(yyvsp[-1].node);
			yyval.node = arg_concat(yyvsp[-4].node, yyvsp[-1].node);
		    }
    break;
case 224:
#line 1259 "parse.y"
{
			yyval.node = NEW_LIST(NEW_HASH(yyvsp[-1].node));
		    }
    break;
case 225:
#line 1263 "parse.y"
{
			value_expr(yyvsp[-1].node);
			yyval.node = NEW_NEWLINE(NEW_SPLAT(yyvsp[-1].node));
		    }
    break;
case 226:
#line 1270 "parse.y"
{
			yyval.node = yyvsp[-1].node;
		    }
    break;
case 227:
#line 1274 "parse.y"
{
			yyval.node = yyvsp[-2].node;
		    }
    break;
case 228:
#line 1278 "parse.y"
{
		        rb_warn("parenthesize argument for future version");
			yyval.node = NEW_LIST(yyvsp[-2].node);
		    }
    break;
case 229:
#line 1283 "parse.y"
{
		        rb_warn("parenthesize argument for future version");
			yyval.node = list_append(yyvsp[-4].node, yyvsp[-2].node);
		    }
    break;
case 232:
#line 1294 "parse.y"
{
		        rb_warn("parenthesize argument(s) for future version");
			yyval.node = NEW_LIST(yyvsp[0].node);
		    }
    break;
case 233:
#line 1299 "parse.y"
{
			yyval.node = arg_blk_pass(yyvsp[-1].node, yyvsp[0].node);
		    }
    break;
case 234:
#line 1303 "parse.y"
{
			yyval.node = arg_concat(yyvsp[-4].node, yyvsp[-1].node);
			yyval.node = arg_blk_pass(yyval.node, yyvsp[0].node);
		    }
    break;
case 235:
#line 1308 "parse.y"
{
			yyval.node = NEW_LIST(NEW_HASH(yyvsp[-1].node));
			yyval.node = arg_blk_pass(yyval.node, yyvsp[0].node);
		    }
    break;
case 236:
#line 1313 "parse.y"
{
			yyval.node = arg_concat(NEW_LIST(NEW_HASH(yyvsp[-4].node)), yyvsp[-1].node);
			yyval.node = arg_blk_pass(yyval.node, yyvsp[0].node);
		    }
    break;
case 237:
#line 1318 "parse.y"
{
			yyval.node = list_append(yyvsp[-3].node, NEW_HASH(yyvsp[-1].node));
			yyval.node = arg_blk_pass(yyval.node, yyvsp[0].node);
		    }
    break;
case 238:
#line 1323 "parse.y"
{
			value_expr(yyvsp[-1].node);
			yyval.node = arg_concat(list_append(yyvsp[-6].node, NEW_HASH(yyvsp[-4].node)), yyvsp[-1].node);
			yyval.node = arg_blk_pass(yyval.node, yyvsp[0].node);
		    }
    break;
case 239:
#line 1329 "parse.y"
{
			yyval.node = arg_blk_pass(NEW_SPLAT(yyvsp[-1].node), yyvsp[0].node);
		    }
    break;
case 241:
#line 1336 "parse.y"
{
			yyval.node = arg_blk_pass(list_concat(NEW_LIST(yyvsp[-3].node),yyvsp[-1].node), yyvsp[0].node);
		    }
    break;
case 242:
#line 1340 "parse.y"
{
                        yyval.node = arg_blk_pass(yyvsp[-2].node, yyvsp[0].node);
                    }
    break;
case 243:
#line 1344 "parse.y"
{
			yyval.node = arg_concat(NEW_LIST(yyvsp[-4].node), yyvsp[-1].node);
			yyval.node = arg_blk_pass(yyval.node, yyvsp[0].node);
		    }
    break;
case 244:
#line 1349 "parse.y"
{
                       yyval.node = arg_concat(list_concat(NEW_LIST(yyvsp[-6].node),yyvsp[-4].node), yyvsp[-1].node);
			yyval.node = arg_blk_pass(yyval.node, yyvsp[0].node);
		    }
    break;
case 245:
#line 1354 "parse.y"
{
			yyval.node = NEW_LIST(NEW_HASH(yyvsp[-1].node));
			yyval.node = arg_blk_pass(yyval.node, yyvsp[0].node);
		    }
    break;
case 246:
#line 1359 "parse.y"
{
			yyval.node = arg_concat(NEW_LIST(NEW_HASH(yyvsp[-4].node)), yyvsp[-1].node);
			yyval.node = arg_blk_pass(yyval.node, yyvsp[0].node);
		    }
    break;
case 247:
#line 1364 "parse.y"
{
			yyval.node = list_append(NEW_LIST(yyvsp[-3].node), NEW_HASH(yyvsp[-1].node));
			yyval.node = arg_blk_pass(yyval.node, yyvsp[0].node);
		    }
    break;
case 248:
#line 1369 "parse.y"
{
			yyval.node = list_append(list_concat(NEW_LIST(yyvsp[-5].node),yyvsp[-3].node), NEW_HASH(yyvsp[-1].node));
			yyval.node = arg_blk_pass(yyval.node, yyvsp[0].node);
		    }
    break;
case 249:
#line 1374 "parse.y"
{
			yyval.node = arg_concat(list_append(NEW_LIST(yyvsp[-6].node), NEW_HASH(yyvsp[-4].node)), yyvsp[-1].node);
			yyval.node = arg_blk_pass(yyval.node, yyvsp[0].node);
		    }
    break;
case 250:
#line 1379 "parse.y"
{
			yyval.node = arg_concat(list_append(list_concat(NEW_LIST(yyvsp[-8].node), yyvsp[-6].node), NEW_HASH(yyvsp[-4].node)), yyvsp[-1].node);
			yyval.node = arg_blk_pass(yyval.node, yyvsp[0].node);
		    }
    break;
case 251:
#line 1384 "parse.y"
{
			yyval.node = arg_blk_pass(NEW_SPLAT(yyvsp[-1].node), yyvsp[0].node);
		    }
    break;
case 253:
#line 1390 "parse.y"
{
			yyval.num = cmdarg_stack;
			CMDARG_PUSH(1);
		    }
    break;
case 254:
#line 1395 "parse.y"
{
			/* CMDARG_POP() */
		        cmdarg_stack = yyvsp[-1].num;
			yyval.node = yyvsp[0].node;
		    }
    break;
case 256:
#line 1403 "parse.y"
{lex_state = EXPR_ENDARG;}
    break;
case 257:
#line 1404 "parse.y"
{
		        rb_warn("don't put space before argument parentheses");
			yyval.node = 0;
		    }
    break;
case 258:
#line 1408 "parse.y"
{lex_state = EXPR_ENDARG;}
    break;
case 259:
#line 1409 "parse.y"
{
		        rb_warn("don't put space before argument parentheses");
			yyval.node = yyvsp[-2].node;
		    }
    break;
case 260:
#line 1416 "parse.y"
{
			yyval.node = NEW_BLOCK_PASS(yyvsp[0].node);
		    }
    break;
case 261:
#line 1422 "parse.y"
{
			yyval.node = yyvsp[0].node;
		    }
    break;
case 263:
#line 1429 "parse.y"
{
			yyval.node = NEW_LIST(yyvsp[0].node);
		    }
    break;
case 264:
#line 1433 "parse.y"
{
			yyval.node = list_append(yyvsp[-2].node, yyvsp[0].node);
		    }
    break;
case 265:
#line 1439 "parse.y"
{
			yyval.node = list_append(yyvsp[-2].node, yyvsp[0].node);
		    }
    break;
case 266:
#line 1443 "parse.y"
{
			yyval.node = arg_concat(yyvsp[-3].node, yyvsp[0].node);
		    }
    break;
case 267:
#line 1447 "parse.y"
{
			yyval.node = NEW_SPLAT(yyvsp[0].node);
		    }
    break;
case 276:
#line 1461 "parse.y"
{
			yyval.node = NEW_FCALL(yyvsp[0].id, 0);
		    }
    break;
case 277:
#line 1465 "parse.y"
{
			yyvsp[0].num = ruby_sourceline;
		    }
    break;
case 278:
#line 1470 "parse.y"
{
			if (yyvsp[-1].node == NULL)
			    yyval.node = NEW_NIL();
			else
			    yyval.node = NEW_BEGIN(yyvsp[-1].node);
			nd_set_line(yyval.node, yyvsp[-3].num);
		    }
    break;
case 279:
#line 1477 "parse.y"
{lex_state = EXPR_ENDARG;}
    break;
case 280:
#line 1478 "parse.y"
{
		        rb_warning("(...) interpreted as grouped expression");
			yyval.node = yyvsp[-3].node;
		    }
    break;
case 281:
#line 1483 "parse.y"
{
			yyval.node = yyvsp[-1].node;
		    }
    break;
case 282:
#line 1487 "parse.y"
{
			yyval.node = NEW_COLON2(yyvsp[-2].node, yyvsp[0].id);
		    }
    break;
case 283:
#line 1491 "parse.y"
{
			yyval.node = NEW_COLON3(yyvsp[0].id);
		    }
    break;
case 284:
#line 1495 "parse.y"
{
			if (yyvsp[-3].node && nd_type(yyvsp[-3].node) == NODE_SELF)
			    yyval.node = NEW_FCALL(tAREF, yyvsp[-1].node);
			else
			    yyval.node = NEW_CALL(yyvsp[-3].node, tAREF, yyvsp[-1].node);
			fixpos(yyval.node, yyvsp[-3].node);
		    }
    break;
case 285:
#line 1503 "parse.y"
{
		        if (yyvsp[-1].node == 0) {
			    yyval.node = NEW_ZARRAY(); /* zero length array*/
			}
			else {
			    yyval.node = yyvsp[-1].node;
			}
		    }
    break;
case 286:
#line 1512 "parse.y"
{
			yyval.node = NEW_HASH(yyvsp[-1].node);
		    }
    break;
case 287:
#line 1516 "parse.y"
{
			yyval.node = NEW_RETURN(0);
		    }
    break;
case 288:
#line 1520 "parse.y"
{
			yyval.node = new_yield(yyvsp[-1].node);
		    }
    break;
case 289:
#line 1524 "parse.y"
{
			yyval.node = NEW_YIELD(0, Qfalse);
		    }
    break;
case 290:
#line 1528 "parse.y"
{
			yyval.node = NEW_YIELD(0, Qfalse);
		    }
    break;
case 291:
#line 1531 "parse.y"
{in_defined = 1;}
    break;
case 292:
#line 1532 "parse.y"
{
		        in_defined = 0;
			yyval.node = NEW_DEFINED(yyvsp[-1].node);
		    }
    break;
case 293:
#line 1537 "parse.y"
{
			yyvsp[0].node->nd_iter = NEW_FCALL(yyvsp[-1].id, 0);
			yyval.node = yyvsp[0].node;
			fixpos(yyvsp[0].node->nd_iter, yyvsp[0].node);
		    }
    break;
case 295:
#line 1544 "parse.y"
{
			if (yyvsp[-1].node && nd_type(yyvsp[-1].node) == NODE_BLOCK_PASS) {
			    rb_compile_error("both block arg and actual block given");
			}
			yyvsp[0].node->nd_iter = yyvsp[-1].node;
			yyval.node = yyvsp[0].node;
		        fixpos(yyval.node, yyvsp[-1].node);
		    }
    break;
case 296:
#line 1556 "parse.y"
{
			yyval.node = NEW_IF(cond(yyvsp[-4].node), yyvsp[-2].node, yyvsp[-1].node);
		        fixpos(yyval.node, yyvsp[-4].node);
			if (cond_negative(&yyval.node->nd_cond)) {
		            NODE *tmp = yyval.node->nd_body;
		            yyval.node->nd_body = yyval.node->nd_else;
		            yyval.node->nd_else = tmp;
			}
		    }
    break;
case 297:
#line 1569 "parse.y"
{
			yyval.node = NEW_UNLESS(cond(yyvsp[-4].node), yyvsp[-2].node, yyvsp[-1].node);
		        fixpos(yyval.node, yyvsp[-4].node);
			if (cond_negative(&yyval.node->nd_cond)) {
		            NODE *tmp = yyval.node->nd_body;
		            yyval.node->nd_body = yyval.node->nd_else;
		            yyval.node->nd_else = tmp;
			}
		    }
    break;
case 298:
#line 1578 "parse.y"
{COND_PUSH(1);}
    break;
case 299:
#line 1578 "parse.y"
{COND_POP();}
    break;
case 300:
#line 1581 "parse.y"
{
			yyval.node = NEW_WHILE(cond(yyvsp[-4].node), yyvsp[-1].node, 1);
		        fixpos(yyval.node, yyvsp[-4].node);
			if (cond_negative(&yyval.node->nd_cond)) {
			    nd_set_type(yyval.node, NODE_UNTIL);
			}
		    }
    break;
case 301:
#line 1588 "parse.y"
{COND_PUSH(1);}
    break;
case 302:
#line 1588 "parse.y"
{COND_POP();}
    break;
case 303:
#line 1591 "parse.y"
{
			yyval.node = NEW_UNTIL(cond(yyvsp[-4].node), yyvsp[-1].node, 1);
		        fixpos(yyval.node, yyvsp[-4].node);
			if (cond_negative(&yyval.node->nd_cond)) {
			    nd_set_type(yyval.node, NODE_WHILE);
			}
		    }
    break;
case 304:
#line 1601 "parse.y"
{
			yyval.node = NEW_CASE(yyvsp[-3].node, yyvsp[-1].node);
		        fixpos(yyval.node, yyvsp[-3].node);
		    }
    break;
case 305:
#line 1606 "parse.y"
{
			yyval.node = yyvsp[-1].node;
		    }
    break;
case 306:
#line 1610 "parse.y"
{
			yyval.node = yyvsp[-1].node;
		    }
    break;
case 307:
#line 1613 "parse.y"
{COND_PUSH(1);}
    break;
case 308:
#line 1613 "parse.y"
{COND_POP();}
    break;
case 309:
#line 1616 "parse.y"
{
			yyval.node = NEW_FOR(yyvsp[-7].node, yyvsp[-4].node, yyvsp[-1].node);
		        fixpos(yyval.node, yyvsp[-7].node);
		    }
    break;
case 310:
#line 1621 "parse.y"
{
			if (in_def || in_single)
			    yyerror("class definition in method body");
			class_nest++;
			local_push(0);
		        yyval.num = ruby_sourceline;
		    }
    break;
case 311:
#line 1630 "parse.y"
{
		        yyval.node = NEW_CLASS(yyvsp[-4].node, yyvsp[-1].node, yyvsp[-3].node);
		        nd_set_line(yyval.node, yyvsp[-2].num);
		        local_pop();
			class_nest--;
		    }
    break;
case 312:
#line 1637 "parse.y"
{
			yyval.num = in_def;
		        in_def = 0;
		    }
    break;
case 313:
#line 1642 "parse.y"
{
		        yyval.num = in_single;
		        in_single = 0;
			class_nest++;
			local_push(0);
		    }
    break;
case 314:
#line 1650 "parse.y"
{
		        yyval.node = NEW_SCLASS(yyvsp[-5].node, yyvsp[-1].node);
		        fixpos(yyval.node, yyvsp[-5].node);
		        local_pop();
			class_nest--;
		        in_def = yyvsp[-4].num;
		        in_single = yyvsp[-2].num;
		    }
    break;
case 315:
#line 1659 "parse.y"
{
			if (in_def || in_single)
			    yyerror("module definition in method body");
			class_nest++;
			local_push(0);
		        yyval.num = ruby_sourceline;
		    }
    break;
case 316:
#line 1668 "parse.y"
{
		        yyval.node = NEW_MODULE(yyvsp[-3].node, yyvsp[-1].node);
		        nd_set_line(yyval.node, yyvsp[-2].num);
		        local_pop();
			class_nest--;
		    }
    break;
case 317:
#line 1675 "parse.y"
{
			yyval.id = cur_mid;
			cur_mid = yyvsp[0].id;
			in_def++;
			local_push(0);
		    }
    break;
case 318:
#line 1684 "parse.y"
{
			if (!yyvsp[-1].node) yyvsp[-1].node = NEW_NIL();
			yyval.node = NEW_DEFN(yyvsp[-4].id, yyvsp[-2].node, yyvsp[-1].node, NOEX_PRIVATE);
		        fixpos(yyval.node, yyvsp[-2].node);
		        local_pop();
			in_def--;
			cur_mid = yyvsp[-3].id;
		    }
    break;
case 319:
#line 1692 "parse.y"
{lex_state = EXPR_FNAME;}
    break;
case 320:
#line 1693 "parse.y"
{
			in_single++;
			local_push(0);
		        lex_state = EXPR_END; /* force for args */
		    }
    break;
case 321:
#line 1701 "parse.y"
{
			yyval.node = NEW_DEFS(yyvsp[-7].node, yyvsp[-4].id, yyvsp[-2].node, yyvsp[-1].node);
		        fixpos(yyval.node, yyvsp[-7].node);
		        local_pop();
			in_single--;
		    }
    break;
case 322:
#line 1708 "parse.y"
{
			yyval.node = NEW_BREAK(0);
		    }
    break;
case 323:
#line 1712 "parse.y"
{
			yyval.node = NEW_NEXT(0);
		    }
    break;
case 324:
#line 1716 "parse.y"
{
			yyval.node = NEW_REDO();
		    }
    break;
case 325:
#line 1720 "parse.y"
{
			yyval.node = NEW_RETRY();
		    }
    break;
case 326:
#line 1726 "parse.y"
{
			value_expr(yyvsp[0].node);
			yyval.node = yyvsp[0].node;
		    }
    break;
case 335:
#line 1747 "parse.y"
{
			yyval.node = NEW_IF(cond(yyvsp[-3].node), yyvsp[-1].node, yyvsp[0].node);
		        fixpos(yyval.node, yyvsp[-3].node);
		    }
    break;
case 337:
#line 1755 "parse.y"
{
			yyval.node = yyvsp[0].node;
		    }
    break;
case 341:
#line 1766 "parse.y"
{
			yyval.node = (NODE*)1;
		    }
    break;
case 342:
#line 1770 "parse.y"
{
			yyval.node = (NODE*)1;
		    }
    break;
case 343:
#line 1774 "parse.y"
{
			yyval.node = yyvsp[-1].node;
		    }
    break;
case 344:
#line 1780 "parse.y"
{
		        yyval.vars = dyna_push();
			yyvsp[0].num = ruby_sourceline;
		    }
    break;
case 345:
#line 1784 "parse.y"
{yyval.vars = ruby_dyna_vars;}
    break;
case 346:
#line 1787 "parse.y"
{
			yyval.node = NEW_ITER(yyvsp[-3].node, 0, dyna_init(yyvsp[-1].node, yyvsp[-2].vars));
			nd_set_line(yyval.node, yyvsp[-5].num);
			dyna_pop(yyvsp[-4].vars);
		    }
    break;
case 347:
#line 1795 "parse.y"
{
			if (yyvsp[-1].node && nd_type(yyvsp[-1].node) == NODE_BLOCK_PASS) {
			    rb_compile_error("both block arg and actual block given");
			}
			yyvsp[0].node->nd_iter = yyvsp[-1].node;
			yyval.node = yyvsp[0].node;
		        fixpos(yyval.node, yyvsp[-1].node);
		    }
    break;
case 348:
#line 1804 "parse.y"
{
			yyval.node = new_call(yyvsp[-3].node, yyvsp[-1].id, yyvsp[0].node);
		    }
    break;
case 349:
#line 1808 "parse.y"
{
			yyval.node = new_call(yyvsp[-3].node, yyvsp[-1].id, yyvsp[0].node);
		    }
    break;
case 350:
#line 1814 "parse.y"
{
			yyval.node = new_fcall(yyvsp[-1].id, yyvsp[0].node);
		        fixpos(yyval.node, yyvsp[0].node);
		    }
    break;
case 351:
#line 1819 "parse.y"
{
			yyval.node = new_call(yyvsp[-3].node, yyvsp[-1].id, yyvsp[0].node);
		        fixpos(yyval.node, yyvsp[-3].node);
		    }
    break;
case 352:
#line 1824 "parse.y"
{
			yyval.node = new_call(yyvsp[-3].node, yyvsp[-1].id, yyvsp[0].node);
		        fixpos(yyval.node, yyvsp[-3].node);
		    }
    break;
case 353:
#line 1829 "parse.y"
{
			yyval.node = new_call(yyvsp[-2].node, yyvsp[0].id, 0);
		    }
    break;
case 354:
#line 1833 "parse.y"
{
			yyval.node = new_super(yyvsp[0].node);
		    }
    break;
case 355:
#line 1837 "parse.y"
{
			yyval.node = NEW_ZSUPER();
		    }
    break;
case 356:
#line 1843 "parse.y"
{
		        yyval.vars = dyna_push();
			yyvsp[0].num = ruby_sourceline;
		    }
    break;
case 357:
#line 1847 "parse.y"
{yyval.vars = ruby_dyna_vars;}
    break;
case 358:
#line 1849 "parse.y"
{
			yyval.node = NEW_ITER(yyvsp[-3].node, 0, dyna_init(yyvsp[-1].node, yyvsp[-2].vars));
			nd_set_line(yyval.node, yyvsp[-5].num);
			dyna_pop(yyvsp[-4].vars);
		    }
    break;
case 359:
#line 1855 "parse.y"
{
		        yyval.vars = dyna_push();
			yyvsp[0].num = ruby_sourceline;
		    }
    break;
case 360:
#line 1859 "parse.y"
{yyval.vars = ruby_dyna_vars;}
    break;
case 361:
#line 1861 "parse.y"
{
			yyval.node = NEW_ITER(yyvsp[-3].node, 0, dyna_init(yyvsp[-1].node, yyvsp[-2].vars));
			nd_set_line(yyval.node, yyvsp[-5].num);
			dyna_pop(yyvsp[-4].vars);
		    }
    break;
case 362:
#line 1871 "parse.y"
{
			yyval.node = NEW_WHEN(yyvsp[-3].node, yyvsp[-1].node, yyvsp[0].node);
		    }
    break;
case 364:
#line 1877 "parse.y"
{
			yyval.node = list_append(yyvsp[-3].node, NEW_WHEN(yyvsp[0].node, 0, 0));
		    }
    break;
case 365:
#line 1881 "parse.y"
{
			yyval.node = NEW_LIST(NEW_WHEN(yyvsp[0].node, 0, 0));
		    }
    break;
case 368:
#line 1893 "parse.y"
{
		        if (yyvsp[-3].node) {
		            yyvsp[-3].node = node_assign(yyvsp[-3].node, NEW_GVAR(rb_intern("$!")));
			    yyvsp[-1].node = block_append(yyvsp[-3].node, yyvsp[-1].node);
			}
			yyval.node = NEW_RESBODY(yyvsp[-4].node, yyvsp[-1].node, yyvsp[0].node);
		        fixpos(yyval.node, yyvsp[-4].node?yyvsp[-4].node:yyvsp[-1].node);
		    }
    break;
case 370:
#line 1905 "parse.y"
{
			yyval.node = NEW_LIST(yyvsp[0].node);
		    }
    break;
case 373:
#line 1913 "parse.y"
{
			yyval.node = yyvsp[0].node;
		    }
    break;
case 375:
#line 1920 "parse.y"
{
			if (yyvsp[0].node)
			    yyval.node = yyvsp[0].node;
			else
			    /* place holder */
			    yyval.node = NEW_NIL();
		    }
    break;
case 378:
#line 1932 "parse.y"
{
			yyval.node = NEW_LIT(ID2SYM(yyvsp[0].id));
		    }
    break;
case 380:
#line 1939 "parse.y"
{
			NODE *node = yyvsp[0].node;
			if (!node) {
			    node = NEW_STR(rb_str_new(0, 0));
			}
			else {
			    node = evstr2dstr(node);
			}
			yyval.node = node;
		    }
    break;
case 382:
#line 1953 "parse.y"
{
			yyval.node = literal_concat(yyvsp[-1].node, yyvsp[0].node);
		    }
    break;
case 383:
#line 1959 "parse.y"
{
			yyval.node = yyvsp[-1].node;
		    }
    break;
case 384:
#line 1965 "parse.y"
{
			NODE *node = yyvsp[-1].node;
			if (!node) {
			    node = NEW_XSTR(rb_str_new(0, 0));
			}
			else {
			    switch (nd_type(node)) {
			      case NODE_STR:
				nd_set_type(node, NODE_XSTR);
				break;
			      case NODE_DSTR:
				nd_set_type(node, NODE_DXSTR);
				break;
			      default:
				node = NEW_NODE(NODE_DXSTR, rb_str_new(0, 0), 1, NEW_LIST(node));
				break;
			    }
			}
			yyval.node = node;
		    }
    break;
case 385:
#line 1988 "parse.y"
{
			int options = yyvsp[0].num;
			NODE *node = yyvsp[-1].node;
			if (!node) {
			    node = NEW_LIT(rb_reg_new("", 0, options & ~RE_OPTION_ONCE));
			}
			else switch (nd_type(node)) {
			  case NODE_STR:
			    {
				VALUE src = node->nd_lit;
				nd_set_type(node, NODE_LIT);
				node->nd_lit = rb_reg_new(RSTRING(src)->ptr,
							  RSTRING(src)->len,
							  options & ~RE_OPTION_ONCE);
			    }
			    break;
			  default:
			    node = NEW_NODE(NODE_DSTR, rb_str_new(0, 0), 1, NEW_LIST(node));
			  case NODE_DSTR:
			    if (options & RE_OPTION_ONCE) {
				nd_set_type(node, NODE_DREGX_ONCE);
			    }
			    else {
				nd_set_type(node, NODE_DREGX);
			    }
			    node->nd_cflag = options & ~RE_OPTION_ONCE;
			    break;
			}
			yyval.node = node;
		    }
    break;
case 386:
#line 2021 "parse.y"
{
			yyval.node = NEW_ZARRAY();
		    }
    break;
case 387:
#line 2025 "parse.y"
{
			yyval.node = yyvsp[-1].node;
		    }
    break;
case 388:
#line 2031 "parse.y"
{
			yyval.node = 0;
		    }
    break;
case 389:
#line 2035 "parse.y"
{
			yyval.node = list_append(yyvsp[-2].node, evstr2dstr(yyvsp[-1].node));
		    }
    break;
case 391:
#line 2042 "parse.y"
{
			yyval.node = literal_concat(yyvsp[-1].node, yyvsp[0].node);
		    }
    break;
case 392:
#line 2048 "parse.y"
{
			yyval.node = NEW_ZARRAY();
		    }
    break;
case 393:
#line 2052 "parse.y"
{
			yyval.node = yyvsp[-1].node;
		    }
    break;
case 394:
#line 2058 "parse.y"
{
			yyval.node = 0;
		    }
    break;
case 395:
#line 2062 "parse.y"
{
			yyval.node = list_append(yyvsp[-2].node, yyvsp[-1].node);
		    }
    break;
case 396:
#line 2068 "parse.y"
{
			yyval.node = 0;
		    }
    break;
case 397:
#line 2072 "parse.y"
{
			yyval.node = literal_concat(yyvsp[-1].node, yyvsp[0].node);
		    }
    break;
case 398:
#line 2078 "parse.y"
{
			yyval.node = 0;
		    }
    break;
case 399:
#line 2082 "parse.y"
{
			yyval.node = literal_concat(yyvsp[-1].node, yyvsp[0].node);
		    }
    break;
case 401:
#line 2089 "parse.y"
{
			yyval.node = lex_strterm;
			lex_strterm = 0;
			lex_state = EXPR_BEG;
		    }
    break;
case 402:
#line 2095 "parse.y"
{
			lex_strterm = yyvsp[-1].node;
		        yyval.node = NEW_EVSTR(yyvsp[0].node);
		    }
    break;
case 403:
#line 2100 "parse.y"
{
			yyval.node = lex_strterm;
			lex_strterm = 0;
			lex_state = EXPR_BEG;
			COND_PUSH(0);
			CMDARG_PUSH(0);
		    }
    break;
case 404:
#line 2108 "parse.y"
{
			lex_strterm = yyvsp[-2].node;
			COND_LEXPOP();
			CMDARG_LEXPOP();
			if ((yyval.node = yyvsp[-1].node) && nd_type(yyval.node) == NODE_NEWLINE) {
			    yyval.node = yyval.node->nd_next;
			    rb_gc_force_recycle((VALUE)yyvsp[-1].node);
			}
			yyval.node = new_evstr(yyval.node);
		    }
    break;
case 405:
#line 2120 "parse.y"
{yyval.node = NEW_GVAR(yyvsp[0].id);}
    break;
case 406:
#line 2121 "parse.y"
{yyval.node = NEW_IVAR(yyvsp[0].id);}
    break;
case 407:
#line 2122 "parse.y"
{yyval.node = NEW_CVAR(yyvsp[0].id);}
    break;
case 409:
#line 2127 "parse.y"
{
		        lex_state = EXPR_END;
			yyval.id = yyvsp[0].id;
		    }
    break;
case 414:
#line 2140 "parse.y"
{
		        lex_state = EXPR_END;
			if (!(yyval.node = yyvsp[-1].node)) {
			    yyerror("empty symbol literal");
			}
			else {
			    VALUE lit;

			    switch (nd_type(yyval.node)) {
			      case NODE_DSTR:
				nd_set_type(yyval.node, NODE_DSYM);
				break;
			      case NODE_STR:
				lit = yyval.node->nd_lit;
				if (RSTRING(lit)->len == 0) {
				    yyerror("empty symbol literal");
				    break;
				}
				if (strlen(RSTRING(lit)->ptr) == RSTRING(lit)->len) {
				    yyval.node->nd_lit = ID2SYM(rb_intern(RSTRING(yyval.node->nd_lit)->ptr));
				    nd_set_type(yyval.node, NODE_LIT);
				    break;
				}
				/* fall through */
			      default:
				yyval.node = NEW_NODE(NODE_DSYM, rb_str_new(0, 0), 1, NEW_LIST(yyval.node));
				break;
			    }
			}
		    }
    break;
case 417:
#line 2175 "parse.y"
{
			yyval.node = negate_lit(yyvsp[0].node);
		    }
    break;
case 418:
#line 2179 "parse.y"
{
			yyval.node = negate_lit(yyvsp[0].node);
		    }
    break;
case 424:
#line 2189 "parse.y"
{yyval.id = kNIL;}
    break;
case 425:
#line 2190 "parse.y"
{yyval.id = kSELF;}
    break;
case 426:
#line 2191 "parse.y"
{yyval.id = kTRUE;}
    break;
case 427:
#line 2192 "parse.y"
{yyval.id = kFALSE;}
    break;
case 428:
#line 2193 "parse.y"
{yyval.id = k__FILE__;}
    break;
case 429:
#line 2194 "parse.y"
{yyval.id = k__LINE__;}
    break;
case 430:
#line 2198 "parse.y"
{
			yyval.node = gettable(yyvsp[0].id);
		    }
    break;
case 431:
#line 2204 "parse.y"
{
			yyval.node = assignable(yyvsp[0].id, 0);
		    }
    break;
case 434:
#line 2214 "parse.y"
{
			yyval.node = 0;
		    }
    break;
case 435:
#line 2218 "parse.y"
{
			lex_state = EXPR_BEG;
		    }
    break;
case 436:
#line 2222 "parse.y"
{
			yyval.node = yyvsp[-1].node;
		    }
    break;
case 437:
#line 2225 "parse.y"
{yyerrok; yyval.node = 0;}
    break;
case 438:
#line 2229 "parse.y"
{
			yyval.node = yyvsp[-2].node;
			lex_state = EXPR_BEG;
		        command_start = Qtrue;
		    }
    break;
case 439:
#line 2235 "parse.y"
{
			yyval.node = yyvsp[-1].node;
		    }
    break;
case 440:
#line 2241 "parse.y"
{
			yyval.node = block_append(NEW_ARGS(yyvsp[-5].num, yyvsp[-3].node, yyvsp[-1].node), yyvsp[0].node);
		    }
    break;
case 441:
#line 2245 "parse.y"
{
			yyval.node = block_append(NEW_ARGS(yyvsp[-3].num, yyvsp[-1].node, 0), yyvsp[0].node);
		    }
    break;
case 442:
#line 2249 "parse.y"
{
			yyval.node = block_append(NEW_ARGS(yyvsp[-3].num, 0, yyvsp[-1].node), yyvsp[0].node);
		    }
    break;
case 443:
#line 2253 "parse.y"
{
			yyval.node = block_append(NEW_ARGS(yyvsp[-1].num, 0, 0), yyvsp[0].node);
		    }
    break;
case 444:
#line 2257 "parse.y"
{
			yyval.node = block_append(NEW_ARGS(0, yyvsp[-3].node, yyvsp[-1].node), yyvsp[0].node);
		    }
    break;
case 445:
#line 2261 "parse.y"
{
			yyval.node = block_append(NEW_ARGS(0, yyvsp[-1].node, 0), yyvsp[0].node);
		    }
    break;
case 446:
#line 2265 "parse.y"
{
			yyval.node = block_append(NEW_ARGS(0, 0, yyvsp[-1].node), yyvsp[0].node);
		    }
    break;
case 447:
#line 2269 "parse.y"
{
			yyval.node = block_append(NEW_ARGS(0, 0, 0), yyvsp[0].node);
		    }
    break;
case 448:
#line 2273 "parse.y"
{
			yyval.node = NEW_ARGS(0, 0, 0);
		    }
    break;
case 449:
#line 2279 "parse.y"
{
			yyerror("formal argument cannot be a constant");
		    }
    break;
case 450:
#line 2283 "parse.y"
{
                        yyerror("formal argument cannot be an instance variable");
		    }
    break;
case 451:
#line 2287 "parse.y"
{
                        yyerror("formal argument cannot be a global variable");
		    }
    break;
case 452:
#line 2291 "parse.y"
{
                        yyerror("formal argument cannot be a class variable");
		    }
    break;
case 453:
#line 2295 "parse.y"
{
			if (!is_local_id(yyvsp[0].id))
			    yyerror("formal argument must be local variable");
			else if (local_id(yyvsp[0].id))
			    yyerror("duplicate argument name");
			local_cnt(yyvsp[0].id);
			yyval.num = 1;
		    }
    break;
case 455:
#line 2307 "parse.y"
{
			yyval.num += 1;
		    }
    break;
case 456:
#line 2313 "parse.y"
{
			if (!is_local_id(yyvsp[-2].id))
			    yyerror("formal argument must be local variable");
			else if (local_id(yyvsp[-2].id))
			    yyerror("duplicate optional argument name");
			yyval.node = assignable(yyvsp[-2].id, yyvsp[0].node);
		    }
    break;
case 457:
#line 2323 "parse.y"
{
			yyval.node = NEW_BLOCK(yyvsp[0].node);
			yyval.node->nd_end = yyval.node;
		    }
    break;
case 458:
#line 2328 "parse.y"
{
			yyval.node = block_append(yyvsp[-2].node, yyvsp[0].node);
		    }
    break;
case 461:
#line 2338 "parse.y"
{
			if (!is_local_id(yyvsp[0].id))
			    yyerror("rest argument must be local variable");
			if (dyna_in_block()) {
			    rb_dvar_push(yyvsp[0].id, Qnil);
			}
			yyval.node = assignable(yyvsp[0].id, 0);
		    }
    break;
case 462:
#line 2347 "parse.y"
{
			if (dyna_in_block()) {
			    yyval.node = NEW_DASGN_CURR(internal_id(), 0);
			}
			else {
			    yyval.node = NEW_NODE(NODE_LASGN,0,0,local_append(0));
			}
		    }
    break;
case 465:
#line 2362 "parse.y"
{
			if (!is_local_id(yyvsp[0].id))
			    yyerror("block argument must be local variable");
			else if (local_id(yyvsp[0].id))
			    yyerror("duplicate block argument name");
			yyval.node = NEW_BLOCK_ARG(yyvsp[0].id);
		    }
    break;
case 466:
#line 2372 "parse.y"
{
			yyval.node = yyvsp[0].node;
		    }
    break;
case 468:
#line 2379 "parse.y"
{
			if (yyvsp[0].node && nd_type(yyvsp[0].node) == NODE_SELF) {
			    yyval.node = NEW_SELF();
			}
			else {
			    yyval.node = yyvsp[0].node;
		            value_expr(yyval.node);
			}
		    }
    break;
case 469:
#line 2388 "parse.y"
{lex_state = EXPR_BEG;}
    break;
case 470:
#line 2389 "parse.y"
{
			if (yyvsp[-2].node == 0) {
			    yyerror("can't define singleton method for ().");
			}
			else {
			    switch (nd_type(yyvsp[-2].node)) {
			      case NODE_STR:
			      case NODE_DSTR:
			      case NODE_XSTR:
			      case NODE_DXSTR:
			      case NODE_DREGX:
			      case NODE_LIT:
			      case NODE_ARRAY:
			      case NODE_ZARRAY:
				yyerror("can't define singleton method for literals");
			      default:
				value_expr(yyvsp[-2].node);
				break;
			    }
			}
			yyval.node = yyvsp[-2].node;
		    }
    break;
case 472:
#line 2415 "parse.y"
{
			yyval.node = yyvsp[-1].node;
		    }
    break;
case 473:
#line 2419 "parse.y"
{
			if (yyvsp[-1].node->nd_alen%2 != 0) {
			    yyerror("odd number list for Hash");
			}
			yyval.node = yyvsp[-1].node;
		    }
    break;
case 475:
#line 2429 "parse.y"
{
			yyval.node = list_concat(yyvsp[-2].node, yyvsp[0].node);
		    }
    break;
case 476:
#line 2435 "parse.y"
{
			yyval.node = list_append(NEW_LIST(yyvsp[-2].node), yyvsp[0].node);
		    }
    break;
case 496:
#line 2473 "parse.y"
{yyerrok;}
    break;
case 499:
#line 2478 "parse.y"
{yyerrok;}
    break;
case 500:
#line 2481 "parse.y"
{yyval.node = 0;}
    break;
}

#line 706 "/usr/share/bison/bison.simple"


  yyvsp -= yylen;
  yyssp -= yylen;
#if YYLSP_NEEDED
  yylsp -= yylen;
#endif

#if YYDEBUG
  if (yydebug)
    {
      short *yyssp1 = yyss - 1;
      YYFPRINTF (stderr, "state stack now");
      while (yyssp1 != yyssp)
	YYFPRINTF (stderr, " %d", *++yyssp1);
      YYFPRINTF (stderr, "\n");
    }
#endif

  *++yyvsp = yyval;
#if YYLSP_NEEDED
  *++yylsp = yyloc;
#endif

  /* Now `shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTBASE] + *yyssp;
  if (yystate >= 0 && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTBASE];

  goto yynewstate;


/*------------------------------------.
| yyerrlab -- here on detecting error |
`------------------------------------*/
yyerrlab:
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;

#ifdef YYERROR_VERBOSE
      yyn = yypact[yystate];

      if (yyn > YYFLAG && yyn < YYLAST)
	{
	  YYSIZE_T yysize = 0;
	  char *yymsg;
	  int yyx, yycount;

	  yycount = 0;
	  /* Start YYX at -YYN if negative to avoid negative indexes in
	     YYCHECK.  */
	  for (yyx = yyn < 0 ? -yyn : 0;
	       yyx < (int) (sizeof (yytname) / sizeof (char *)); yyx++)
	    if (yycheck[yyx + yyn] == yyx)
	      yysize += yystrlen (yytname[yyx]) + 15, yycount++;
	  yysize += yystrlen ("parse error, unexpected ") + 1;
	  yysize += yystrlen (yytname[YYTRANSLATE (yychar)]);
	  yymsg = (char *) YYSTACK_ALLOC (yysize);
	  if (yymsg != 0)
	    {
	      char *yyp = yystpcpy (yymsg, "parse error, unexpected ");
	      yyp = yystpcpy (yyp, yytname[YYTRANSLATE (yychar)]);

	      if (yycount < 5)
		{
		  yycount = 0;
		  for (yyx = yyn < 0 ? -yyn : 0;
		       yyx < (int) (sizeof (yytname) / sizeof (char *));
		       yyx++)
		    if (yycheck[yyx + yyn] == yyx)
		      {
			const char *yyq = ! yycount ? ", expecting " : " or ";
			yyp = yystpcpy (yyp, yyq);
			yyp = yystpcpy (yyp, yytname[yyx]);
			yycount++;
		      }
		}
	      yyerror (yymsg);
	      YYSTACK_FREE (yymsg);
	    }
	  else
	    yyerror ("parse error; also virtual memory exhausted");
	}
      else
#endif /* defined (YYERROR_VERBOSE) */
	yyerror ("parse error");
    }
  goto yyerrlab1;


/*--------------------------------------------------.
| yyerrlab1 -- error raised explicitly by an action |
`--------------------------------------------------*/
yyerrlab1:
  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
	 error, discard it.  */

      /* return failure if at end of input */
      if (yychar == YYEOF)
	YYABORT;
      YYDPRINTF ((stderr, "Discarding token %d (%s).\n",
		  yychar, yytname[yychar1]));
      yychar = YYEMPTY;
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */

  yyerrstatus = 3;		/* Each real token shifted decrements this */

  goto yyerrhandle;


/*-------------------------------------------------------------------.
| yyerrdefault -- current state does not do anything special for the |
| error token.                                                       |
`-------------------------------------------------------------------*/
yyerrdefault:
#if 0
  /* This is wrong; only states that explicitly want error tokens
     should shift them.  */

  /* If its default is to accept any token, ok.  Otherwise pop it.  */
  yyn = yydefact[yystate];
  if (yyn)
    goto yydefault;
#endif


/*---------------------------------------------------------------.
| yyerrpop -- pop the current state because it cannot handle the |
| error token                                                    |
`---------------------------------------------------------------*/
yyerrpop:
  if (yyssp == yyss)
    YYABORT;
  yyvsp--;
  yystate = *--yyssp;
#if YYLSP_NEEDED
  yylsp--;
#endif

#if YYDEBUG
  if (yydebug)
    {
      short *yyssp1 = yyss - 1;
      YYFPRINTF (stderr, "Error: state stack now");
      while (yyssp1 != yyssp)
	YYFPRINTF (stderr, " %d", *++yyssp1);
      YYFPRINTF (stderr, "\n");
    }
#endif

/*--------------.
| yyerrhandle.  |
`--------------*/
yyerrhandle:
  yyn = yypact[yystate];
  if (yyn == YYFLAG)
    goto yyerrdefault;

  yyn += YYTERROR;
  if (yyn < 0 || yyn > YYLAST || yycheck[yyn] != YYTERROR)
    goto yyerrdefault;

  yyn = yytable[yyn];
  if (yyn < 0)
    {
      if (yyn == YYFLAG)
	goto yyerrpop;
      yyn = -yyn;
      goto yyreduce;
    }
  else if (yyn == 0)
    goto yyerrpop;

  if (yyn == YYFINAL)
    YYACCEPT;

  YYDPRINTF ((stderr, "Shifting error token, "));

  *++yyvsp = yylval;
#if YYLSP_NEEDED
  *++yylsp = yylloc;
#endif

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

/*---------------------------------------------.
| yyoverflowab -- parser overflow comes here.  |
`---------------------------------------------*/
yyoverflowlab:
  yyerror ("parser stack overflow");
  yyresult = 2;
  /* Fall through.  */

yyreturn:
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
  return yyresult;
}
#line 2483 "parse.y"

#ifdef yystacksize
#undef YYMALLOC
#endif

#include "regex.h"
#include "util.h"

/* We remove any previous definition of `SIGN_EXTEND_CHAR',
   since ours (we hope) works properly with all combinations of
   machines, compilers, `char' and `unsigned char' argument types.
   (Per Bothner suggested the basic approach.)  */
#undef SIGN_EXTEND_CHAR
#if __STDC__
# define SIGN_EXTEND_CHAR(c) ((signed char)(c))
#else  /* not __STDC__ */
/* As in Harbison and Steele.  */
# define SIGN_EXTEND_CHAR(c) ((((unsigned char)(c)) ^ 128) - 128)
#endif
#define is_identchar(c) (SIGN_EXTEND_CHAR(c)!=-1&&(ISALNUM(c) || (c) == '_' || ismbchar(c)))

static char *tokenbuf = NULL;
static int   tokidx, toksiz = 0;

#define LEAVE_BS 1

static VALUE (*lex_gets)();	/* gets function */
static VALUE lex_input;		/* non-nil if File */
static VALUE lex_lastline;	/* gc protect */
static char *lex_pbeg;
static char *lex_p;
static char *lex_pend;

static int
yyerror(msg)
    const char *msg;
{
    char *p, *pe, *buf;
    int len, i;

    rb_compile_error("%s", msg);
    p = lex_p;
    while (lex_pbeg <= p) {
	if (*p == '\n') break;
	p--;
    }
    p++;

    pe = lex_p;
    while (pe < lex_pend) {
	if (*pe == '\n') break;
	pe++;
    }

    len = pe - p;
    if (len > 4) {
	buf = ALLOCA_N(char, len+2);
	MEMCPY(buf, p, char, len);
	buf[len] = '\0';
	rb_compile_error_append("%s", buf);

	i = lex_p - p;
	p = buf; pe = p + len;

	while (p < pe) {
	    if (*p != '\t') *p = ' ';
	    p++;
	}
	buf[i] = '^';
	buf[i+1] = '\0';
	rb_compile_error_append("%s", buf);
    }

    return 0;
}

static int heredoc_end;

int ruby_in_compile = 0;
int ruby__end__seen;

static VALUE ruby_debug_lines;
#ifdef YYMALLOC
static NODE *parser_heap;
#endif

static NODE*
yycompile(f, line)
    char *f;
    int line;
{
    int n;
    NODE *node = 0;
    struct RVarmap *vp, *vars = ruby_dyna_vars;

    ruby_in_compile = 1;
    if (!compile_for_eval && rb_safe_level() == 0 &&
	rb_const_defined(rb_cObject, rb_intern("SCRIPT_LINES__"))) {
	VALUE hash, fname;

	hash = rb_const_get(rb_cObject, rb_intern("SCRIPT_LINES__"));
	if (TYPE(hash) == T_HASH) {
	    fname = rb_str_new2(f);
	    ruby_debug_lines = rb_hash_aref(hash, fname);
	    if (NIL_P(ruby_debug_lines)) {
		ruby_debug_lines = rb_ary_new();
		rb_hash_aset(hash, fname, ruby_debug_lines);
	    }
	}
	if (line > 1) {
	    VALUE str = rb_str_new(0,0);
	    while (line > 1) {
		rb_ary_push(ruby_debug_lines, str);
		line--;
	    }
	}
    }

    ruby__end__seen = 0;
    ruby_eval_tree = 0;
    heredoc_end = 0;
    lex_strterm = 0;
    ruby_current_node = 0;
    ruby_sourcefile = rb_source_filename(f);
    n = yyparse();
    ruby_debug_lines = 0;
    compile_for_eval = 0;
    ruby_in_compile = 0;
    cond_stack = 0;
    cmdarg_stack = 0;
    command_start = 1;
    class_nest = 0;
    in_single = 0;
    in_def = 0;
    cur_mid = 0;

    vp = ruby_dyna_vars;
    ruby_dyna_vars = vars;
    lex_strterm = 0;
    while (vp && vp != vars) {
	struct RVarmap *tmp = vp;
	vp = vp->next;
	rb_gc_force_recycle((VALUE)tmp);
    }
    if (n == 0) node = ruby_eval_tree;
    else ruby_eval_tree_begin = 0;
    return node;
}

static int lex_gets_ptr;

static VALUE
lex_get_str(s)
    VALUE s;
{
    char *beg, *end, *pend;

    beg = RSTRING(s)->ptr;
    if (lex_gets_ptr) {
	if (RSTRING(s)->len == lex_gets_ptr) return Qnil;
	beg += lex_gets_ptr;
    }
    pend = RSTRING(s)->ptr + RSTRING(s)->len;
    end = beg;
    while (end < pend) {
	if (*end++ == '\n') break;
    }
    lex_gets_ptr = end - RSTRING(s)->ptr;
    return rb_str_new(beg, end - beg);
}

static VALUE
lex_getline()
{
    VALUE line = (*lex_gets)(lex_input);
    if (ruby_debug_lines && !NIL_P(line)) {
	rb_ary_push(ruby_debug_lines, line);
    }
    return line;
}

NODE*
rb_compile_string(f, s, line)
    const char *f;
    VALUE s;
    int line;
{
    lex_gets = lex_get_str;
    lex_gets_ptr = 0;
    lex_input = s;
    lex_pbeg = lex_p = lex_pend = 0;
    ruby_sourceline = line - 1;
    compile_for_eval = ruby_in_eval;

    return yycompile(f, line);
}

NODE*
rb_compile_cstr(f, s, len, line)
    const char *f, *s;
    int len, line;
{
    return rb_compile_string(f, rb_str_new(s, len), line);
}

NODE*
rb_compile_file(f, file, start)
    const char *f;
    VALUE file;
    int start;
{
    lex_gets = rb_io_gets;
    lex_input = file;
    lex_pbeg = lex_p = lex_pend = 0;
    ruby_sourceline = start - 1;

    return yycompile(f, start);
}

static inline int
nextc()
{
    int c;

    if (lex_p == lex_pend) {
	if (lex_input) {
	    VALUE v = lex_getline();

	    if (NIL_P(v)) return -1;
	    if (heredoc_end > 0) {
		ruby_sourceline = heredoc_end;
		heredoc_end = 0;
	    }
	    ruby_sourceline++;
	    lex_pbeg = lex_p = RSTRING(v)->ptr;
	    lex_pend = lex_p + RSTRING(v)->len;
	    lex_lastline = v;
	}
	else {
	    lex_lastline = 0;
	    return -1;
	}
    }
    c = (unsigned char)*lex_p++;
    if (c == '\r' && lex_p < lex_pend && *lex_p == '\n') {
	lex_p++;
	c = '\n';
    }

    return c;
}

static void
pushback(c)
    int c;
{
    if (c == -1) return;
    lex_p--;
}

#define was_bol() (lex_p == lex_pbeg + 1)
#define peek(c) (lex_p != lex_pend && (c) == *lex_p)

#define tokfix() (tokenbuf[tokidx]='\0')
#define tok() tokenbuf
#define toklen() tokidx
#define toklast() (tokidx>0?tokenbuf[tokidx-1]:0)

static char*
newtok()
{
    tokidx = 0;
    if (!tokenbuf) {
	toksiz = 60;
	tokenbuf = ALLOC_N(char, 60);
    }
    if (toksiz > 4096) {
	toksiz = 60;
	REALLOC_N(tokenbuf, char, 60);
    }
    return tokenbuf;
}

static void
tokadd(c)
    char c;
{
    tokenbuf[tokidx++] = c;
    if (tokidx >= toksiz) {
	toksiz *= 2;
	REALLOC_N(tokenbuf, char, toksiz);
    }
}

static int
read_escape()
{
    int c;

    switch (c = nextc()) {
      case '\\':	/* Backslash */
	return c;

      case 'n':	/* newline */
	return '\n';

      case 't':	/* horizontal tab */
	return '\t';

      case 'r':	/* carriage-return */
	return '\r';

      case 'f':	/* form-feed */
	return '\f';

      case 'v':	/* vertical tab */
	return '\13';

      case 'a':	/* alarm(bell) */
	return '\007';

      case 'e':	/* escape */
	return 033;

      case '0': case '1': case '2': case '3': /* octal constant */
      case '4': case '5': case '6': case '7':
	{
	    int numlen;

	    pushback(c);
	    c = scan_oct(lex_p, 3, &numlen);
	    lex_p += numlen;
	}
	return c;

      case 'x':	/* hex constant */
	{
	    int numlen;

	    c = scan_hex(lex_p, 2, &numlen);
	    if (numlen == 0) {
		yyerror("Invalid escape character syntax");
		return 0;
	    }
	    lex_p += numlen;
	}
	return c;

      case 'b':	/* backspace */
	return '\010';

      case 's':	/* space */
	return ' ';

      case 'M':
	if ((c = nextc()) != '-') {
	    yyerror("Invalid escape character syntax");
	    pushback(c);
	    return '\0';
	}
	if ((c = nextc()) == '\\') {
	    return read_escape() | 0x80;
	}
	else if (c == -1) goto eof;
	else {
	    return ((c & 0xff) | 0x80);
	}

      case 'C':
	if ((c = nextc()) != '-') {
	    yyerror("Invalid escape character syntax");
	    pushback(c);
	    return '\0';
	}
      case 'c':
	if ((c = nextc())== '\\') {
	    c = read_escape();
	}
	else if (c == '?')
	    return 0177;
	else if (c == -1) goto eof;
	return c & 0x9f;

      eof:
      case -1:
        yyerror("Invalid escape character syntax");
	return '\0';

      default:
	return c;
    }
}

static int
tokadd_escape(term)
    int term;
{
    int c;

    switch (c = nextc()) {
      case '\n':
	return 0;		/* just ignore */

      case '0': case '1': case '2': case '3': /* octal constant */
      case '4': case '5': case '6': case '7':
	{
	    int i;

	    tokadd('\\');
	    tokadd(c);
	    for (i=0; i<2; i++) {
		c = nextc();
		if (c == -1) goto eof;
		if (c < '0' || '7' < c) {
		    pushback(c);
		    break;
		}
		tokadd(c);
	    }
	}
	return 0;

      case 'x':	/* hex constant */
	{
	    int numlen;

	    tokadd('\\');
	    tokadd(c);
	    scan_hex(lex_p, 2, &numlen);
	    if (numlen == 0) {
		yyerror("Invalid escape character syntax");
		return -1;
	    }
	    while (numlen--)
		tokadd(nextc());
	}
	return 0;

      case 'M':
	if ((c = nextc()) != '-') {
	    yyerror("Invalid escape character syntax");
	    pushback(c);
	    return 0;
	}
	tokadd('\\'); tokadd('M'); tokadd('-');
	goto escaped;

      case 'C':
	if ((c = nextc()) != '-') {
	    yyerror("Invalid escape character syntax");
	    pushback(c);
	    return 0;
	}
	tokadd('\\'); tokadd('C'); tokadd('-');
	goto escaped;

      case 'c':
	tokadd('\\'); tokadd('c');
      escaped:
	if ((c = nextc()) == '\\') {
	    return tokadd_escape(term);
	}
	else if (c == -1) goto eof;
	tokadd(c);
	return 0;

      eof:
      case -1:
        yyerror("Invalid escape character syntax");
	return -1;

      default:
	if (c != '\\' || c != term)
	    tokadd('\\');
	tokadd(c);
    }
    return 0;
}

static int
regx_options()
{
    char kcode = 0;
    int options = 0;
    int c;

    newtok();
    while (c = nextc(), ISALPHA(c)) {
	switch (c) {
	  case 'i':
	    options |= RE_OPTION_IGNORECASE;
	    break;
	  case 'x':
	    options |= RE_OPTION_EXTENDED;
	    break;
	  case 'm':
	    options |= RE_OPTION_MULTILINE;
	    break;
	  case 'o':
	    options |= RE_OPTION_ONCE;
	    break;
	  case 'n':
	    kcode = 16;
	    break;
	  case 'e':
	    kcode = 32;
	    break;
	  case 's':
	    kcode = 48;
	    break;
	  case 'u':
	    kcode = 64;
	    break;
	  default:
	    tokadd(c);
	    break;
	}
    }
    pushback(c);
    if (toklen()) {
	tokfix();
	rb_compile_error("unknown regexp option%s - %s",
			 toklen() > 1 ? "s" : "", tok());
    }
    return options | kcode;
}

#define STR_FUNC_ESCAPE 0x01
#define STR_FUNC_EXPAND 0x02
#define STR_FUNC_REGEXP 0x04
#define STR_FUNC_QWORDS 0x08
#define STR_FUNC_SYMBOL 0x10
#define STR_FUNC_INDENT 0x20

enum string_type {
    str_squote = (0),
    str_dquote = (STR_FUNC_EXPAND),
    str_xquote = (STR_FUNC_EXPAND),
    str_regexp = (STR_FUNC_REGEXP|STR_FUNC_ESCAPE|STR_FUNC_EXPAND),
    str_sword  = (STR_FUNC_QWORDS),
    str_dword  = (STR_FUNC_QWORDS|STR_FUNC_EXPAND),
    str_ssym   = (STR_FUNC_SYMBOL),
    str_dsym   = (STR_FUNC_SYMBOL|STR_FUNC_EXPAND),
};

static void
dispose_string(str)
    VALUE str;
{
    xfree(RSTRING(str)->ptr);
    rb_gc_force_recycle(str);
}

static int
tokadd_string(func, term, paren, nest)
    int func, term, paren, *nest;
{
    int c;

    while ((c = nextc()) != -1) {
	if (paren && c == paren) {
	    ++*nest;
	}
	else if (c == term) {
	    if (!nest || !*nest) {
		pushback(c);
		break;
	    }
	    --*nest;
	}
	else if ((func & STR_FUNC_EXPAND) && c == '#' && lex_p < lex_pend) {
	    int c2 = *lex_p;
	    if (c2 == '$' || c2 == '@' || c2 == '{') {
		pushback(c);
		break;
	    }
	}
	else if (c == '\\') {
	    c = nextc();
	    switch (c) {
	      case '\n':
		if (func & STR_FUNC_QWORDS) break;
		if (func & STR_FUNC_EXPAND) continue;
		tokadd('\\');
		break;

	      case '\\':
		if (func & STR_FUNC_ESCAPE) tokadd(c);
		break;

	      default:
		if (func & STR_FUNC_REGEXP) {
		    pushback(c);
		    if (tokadd_escape(term) < 0)
			return -1;
		    continue;
		}
		else if (func & STR_FUNC_EXPAND) {
		    pushback(c);
		    if (func & STR_FUNC_ESCAPE) tokadd('\\');
		    c = read_escape();
		}
		else if ((func & STR_FUNC_QWORDS) && ISSPACE(c)) {
		    /* ignore backslashed spaces in %w */
		}
		else if (c != term && !(paren && c == paren)) {
		    tokadd('\\');
		}
	    }
	}
	else if (ismbchar(c)) {
	    int i, len = mbclen(c)-1;

	    for (i = 0; i < len; i++) {
		tokadd(c);
		c = nextc();
	    }
	}
	else if ((func & STR_FUNC_QWORDS) && ISSPACE(c)) {
	    pushback(c);
	    break;
	}
	if (!c && (func & STR_FUNC_SYMBOL)) {
	    func &= ~STR_FUNC_SYMBOL;
	    rb_compile_error("symbol cannot contain '\\0'");
	    continue;
	}
	tokadd(c);
    }
    return c;
}

#define NEW_STRTERM(func, term, paren) \
	rb_node_newnode(NODE_STRTERM, (func), (term) | ((paren) << (CHAR_BIT * 2)), 0)

static int
parse_string(quote)
    NODE *quote;
{
    int func = quote->nd_func;
    int term = nd_term(quote);
    int paren = nd_paren(quote);
    int c, space = 0;

    if (func == -1) return tSTRING_END;
    c = nextc();
    if ((func & STR_FUNC_QWORDS) && ISSPACE(c)) {
	do {c = nextc();} while (ISSPACE(c));
	space = 1;
    }
    if (c == term && !quote->nd_nest) {
	if (func & STR_FUNC_QWORDS) {
	    quote->nd_func = -1;
	    return ' ';
	}
	if (!(func & STR_FUNC_REGEXP)) return tSTRING_END;
	yylval.num = regx_options();
	return tREGEXP_END;
    }
    if (space) {
	pushback(c);
	return ' ';
    }
    newtok();
    if ((func & STR_FUNC_EXPAND) && c == '#') {
	switch (c = nextc()) {
	  case '$':
	  case '@':
	    pushback(c);
	    return tSTRING_DVAR;
	  case '{':
	    return tSTRING_DBEG;
	}
	tokadd('#');
    }
    pushback(c);
    if (tokadd_string(func, term, paren, &quote->nd_nest) == -1) {
	ruby_sourceline = nd_line(quote);
	rb_compile_error("unterminated string meets end of file");
	return tSTRING_END;
    }

    tokfix();
    yylval.node = NEW_STR(rb_str_new(tok(), toklen()));
    return tSTRING_CONTENT;
}

static int
heredoc_identifier()
{
    int c = nextc(), term, func = 0, len;

    if (c == '-') {
	c = nextc();
	func = STR_FUNC_INDENT;
    }
    switch (c) {
      case '\'':
	func |= str_squote; goto quoted;
      case '"':
	func |= str_dquote; goto quoted;
      case '`':
	func |= str_xquote;
      quoted:
	newtok();
	tokadd(func);
	term = c;
	while ((c = nextc()) != -1 && c != term) {
	    len = mbclen(c);
	    do {tokadd(c);} while (--len > 0 && (c = nextc()) != -1);
	}
	if (c == -1) {
	    rb_compile_error("unterminated here document identifier");
	    return 0;
	}
	break;

      default:
	if (!is_identchar(c)) {
	    pushback(c);
	    if (func & STR_FUNC_INDENT) {
		pushback('-');
	    }
	    return 0;
	}
	newtok();
	term = '"';
	tokadd(func |= str_dquote);
	do {
	    len = mbclen(c);
	    do {tokadd(c);} while (--len > 0 && (c = nextc()) != -1);
	} while ((c = nextc()) != -1 && is_identchar(c));
	pushback(c);
	break;
    }

    tokfix();
    len = lex_p - lex_pbeg;
    lex_p = lex_pend;
    lex_strterm = rb_node_newnode(NODE_HEREDOC,
				  rb_str_new(tok(), toklen()),	/* nd_lit */
				  len,				/* nd_nth */
				  lex_lastline);		/* nd_orig */
    return term == '`' ? tXSTRING_BEG : tSTRING_BEG;
}

static void
heredoc_restore(here)
    NODE *here;
{
    VALUE line = here->nd_orig;
    lex_lastline = line;
    lex_pbeg = RSTRING(line)->ptr;
    lex_pend = lex_pbeg + RSTRING(line)->len;
    lex_p = lex_pbeg + here->nd_nth;
    heredoc_end = ruby_sourceline;
    ruby_sourceline = nd_line(here);
    dispose_string(here->nd_lit);
    rb_gc_force_recycle((VALUE)here);
}

static int
whole_match_p(eos, len, indent)
    char *eos;
    int len, indent;
{
    char *p = lex_pbeg;
    int n;

    if (indent) {
	while (*p && ISSPACE(*p)) p++;
    }
    n= lex_pend - (p + len);
    if (n < 0 || (n > 0 && p[len] != '\n' && p[len] != '\r')) return Qfalse;
    if (strncmp(eos, p, len) == 0) return Qtrue;
    return Qfalse;
}

static int
here_document(here)
    NODE *here;
{
    int c, func, indent = 0;
    char *eos, *p, *pend;
    long len;
    VALUE str = 0;

    eos = RSTRING(here->nd_lit)->ptr;
    len = RSTRING(here->nd_lit)->len - 1;
    indent = (func = *eos++) & STR_FUNC_INDENT;

    if ((c = nextc()) == -1) {
      error:
	rb_compile_error("can't find string \"%s\" anywhere before EOF", eos);
	heredoc_restore(lex_strterm);
	lex_strterm = 0;
	return 0;
    }
    if (was_bol() && whole_match_p(eos, len, indent)) {
	heredoc_restore(lex_strterm);
	return tSTRING_END;
    }

    if (!(func & STR_FUNC_EXPAND)) {
	do {
	    p = RSTRING(lex_lastline)->ptr;
	    pend = lex_pend;
	    if (pend > p) {
		switch (pend[-1]) {
		  case '\n':
		    if (--pend == p || pend[-1] != '\r') {
			pend++;
			break;
		    }
		  case '\r':
		    --pend;
		}
	    }
	    if (str)
		rb_str_cat(str, p, pend - p);
	    else
		str = rb_str_new(p, pend - p);
	    if (pend < lex_pend) rb_str_cat(str, "\n", 1);
	    lex_p = lex_pend;
	    if (nextc() == -1) {
		if (str) dispose_string(str);
		goto error;
	    }
	} while (!whole_match_p(eos, len, indent));
    }
    else {
	newtok();
	if (c == '#') {
	    switch (c = nextc()) {
	      case '$':
	      case '@':
		pushback(c);
		return tSTRING_DVAR;
	      case '{':
		return tSTRING_DBEG;
	    }
	    tokadd('#');
	}
	do {
	    pushback(c);
	    if ((c = tokadd_string(func, '\n', 0, NULL)) == -1) goto error;
	    if (c != '\n') {
		yylval.node = NEW_STR(rb_str_new(tok(), toklen()));
		return tSTRING_CONTENT;
	    }
	    tokadd(nextc());
	    if ((c = nextc()) == -1) goto error;
	} while (!whole_match_p(eos, len, indent));
	str = rb_str_new(tok(), toklen());
    }
    heredoc_restore(lex_strterm);
    lex_strterm = NEW_STRTERM(-1, 0, 0);
    yylval.node = NEW_STR(str);
    return tSTRING_CONTENT;
}

#include "lex.c"

static void
arg_ambiguous()
{
    rb_warning("ambiguous first argument; put parentheses or even spaces");
}

#define IS_ARG() (lex_state == EXPR_ARG || lex_state == EXPR_CMDARG)

static int
yylex()
{
    register int c;
    int space_seen = 0;
    int cmd_state;
    enum lex_state last_state;

    if (lex_strterm) {
	int token;
	if (nd_type(lex_strterm) == NODE_HEREDOC) {
	    token = here_document(lex_strterm);
	    if (token == tSTRING_END) {
		lex_strterm = 0;
		lex_state = EXPR_END;
	    }
	}
	else {
	    token = parse_string(lex_strterm);
	    if (token == tSTRING_END || token == tREGEXP_END) {
		rb_gc_force_recycle((VALUE)lex_strterm);
		lex_strterm = 0;
		lex_state = EXPR_END;
	    }
	}
	return token;
    }
    cmd_state = command_start;
    command_start = Qfalse;
  retry:
    switch (c = nextc()) {
      case '\0':		/* NUL */
      case '\004':		/* ^D */
      case '\032':		/* ^Z */
      case -1:			/* end of script. */
	return 0;

	/* white spaces */
      case ' ': case '\t': case '\f': case '\r':
      case '\13': /* '\v' */
	space_seen++;
	goto retry;

      case '#':		/* it's a comment */
	while ((c = nextc()) != '\n') {
	    if (c == -1)
		return 0;
	}
	/* fall through */
      case '\n':
	switch (lex_state) {
	  case EXPR_BEG:
	  case EXPR_FNAME:
	  case EXPR_DOT:
	  case EXPR_CLASS:
	    goto retry;
	  default:
	    break;
	}
	command_start = Qtrue;
	lex_state = EXPR_BEG;
	return '\n';

      case '*':
	if ((c = nextc()) == '*') {
	    if ((c = nextc()) == '=') {
		yylval.id = tPOW;
		lex_state = EXPR_BEG;
		return tOP_ASGN;
	    }
	    pushback(c);
	    c = tPOW;
	}
	else {
	    if (c == '=') {
		yylval.id = '*';
		lex_state = EXPR_BEG;
		return tOP_ASGN;
	    }
	    pushback(c);
	    if (IS_ARG() && space_seen && !ISSPACE(c)){
		rb_warning("`*' interpreted as argument prefix");
		c = tSTAR;
	    }
	    else if (lex_state == EXPR_BEG || lex_state == EXPR_MID) {
		c = tSTAR;
	    }
	    else {
		c = '*';
	    }
	}
	switch (lex_state) {
	  case EXPR_FNAME: case EXPR_DOT:
	    lex_state = EXPR_ARG; break;
	  default:
	    lex_state = EXPR_BEG; break;
	}
	return c;

      case '!':
	lex_state = EXPR_BEG;
	if ((c = nextc()) == '=') {
	    return tNEQ;
	}
	if (c == '~') {
	    return tNMATCH;
	}
	pushback(c);
	return '!';

      case '=':
	if (was_bol()) {
	    /* skip embedded rd document */
	    if (strncmp(lex_p, "begin", 5) == 0 && ISSPACE(lex_p[5])) {
		for (;;) {
		    lex_p = lex_pend;
		    c = nextc();
		    if (c == -1) {
			rb_compile_error("embedded document meets end of file");
			return 0;
		    }
		    if (c != '=') continue;
		    if (strncmp(lex_p, "end", 3) == 0 &&
			(lex_p + 3 == lex_pend || ISSPACE(lex_p[3]))) {
			break;
		    }
		}
		lex_p = lex_pend;
		goto retry;
	    }
	}

	switch (lex_state) {
	  case EXPR_FNAME: case EXPR_DOT:
	    lex_state = EXPR_ARG; break;
	  default:
	    lex_state = EXPR_BEG; break;
	}
	if ((c = nextc()) == '=') {
	    if ((c = nextc()) == '=') {
		return tEQQ;
	    }
	    pushback(c);
	    return tEQ;
	}
	if (c == '~') {
	    return tMATCH;
	}
	else if (c == '>') {
	    return tASSOC;
	}
	pushback(c);
	return '=';

      case '<':
	c = nextc();
	if (c == '<' &&
	    lex_state != EXPR_END &&
	    lex_state != EXPR_DOT &&
	    lex_state != EXPR_ENDARG &&
	    lex_state != EXPR_CLASS &&
	    (!IS_ARG() || space_seen)) {
	    int token = heredoc_identifier();
	    if (token) return token;
	}
	switch (lex_state) {
	  case EXPR_FNAME: case EXPR_DOT:
	    lex_state = EXPR_ARG; break;
	  default:
	    lex_state = EXPR_BEG; break;
	}
	if (c == '=') {
	    if ((c = nextc()) == '>') {
		return tCMP;
	    }
	    pushback(c);
	    return tLEQ;
	}
	if (c == '<') {
	    if ((c = nextc()) == '=') {
		yylval.id = tLSHFT;
		lex_state = EXPR_BEG;
		return tOP_ASGN;
	    }
	    pushback(c);
	    return tLSHFT;
	}
	pushback(c);
	return '<';

      case '>':
	switch (lex_state) {
	  case EXPR_FNAME: case EXPR_DOT:
	    lex_state = EXPR_ARG; break;
	  default:
	    lex_state = EXPR_BEG; break;
	}
	if ((c = nextc()) == '=') {
	    return tGEQ;
	}
	if (c == '>') {
	    if ((c = nextc()) == '=') {
		yylval.id = tRSHFT;
		lex_state = EXPR_BEG;
		return tOP_ASGN;
	    }
	    pushback(c);
	    return tRSHFT;
	}
	pushback(c);
	return '>';

      case '"':
	lex_strterm = NEW_STRTERM(str_dquote, '"', 0);
	return tSTRING_BEG;

      case '`':
	if (lex_state == EXPR_FNAME) {
	    lex_state = EXPR_END;
	    return c;
	}
	if (lex_state == EXPR_DOT) {
	    if (cmd_state)
		lex_state = EXPR_CMDARG;
	    else
		lex_state = EXPR_ARG;
	    return c;
	}
	lex_strterm = NEW_STRTERM(str_xquote, '`', 0);
	return tXSTRING_BEG;

      case '\'':
	lex_strterm = NEW_STRTERM(str_squote, '\'', 0);
	return tSTRING_BEG;

      case '?':
	if (lex_state == EXPR_END || lex_state == EXPR_ENDARG) {
	    lex_state = EXPR_BEG;
	    return '?';
	}
	c = nextc();
	if (c == -1) {
	    rb_compile_error("incomplete character syntax");
	    return 0;
	}
	if (ISSPACE(c)){
	    if (!IS_ARG()){
		int c2 = 0;
		switch (c) {
		  case ' ':
		    c2 = 's';
		    break;
		  case '\n':
		    c2 = 'n';
		    break;
		  case '\t':
		    c2 = 't';
		    break;
		  case '\v':
		    c2 = 'v';
		    break;
		  case '\r':
		    c2 = 'r';
		    break;
		  case '\f':
		    c2 = 'f';
		    break;
		}
		if (c2) {
		    rb_warn("invalid character syntax; use ?\\%c", c2);
		}
	    }
	  ternary:
	    pushback(c);
	    lex_state = EXPR_BEG;
	    return '?';
	}
	else if (ismbchar(c)) {
	    rb_warn("multibyte character literal not supported yet; use ?\\%.3o", c);
	    goto ternary;
	}
	else if ((ISALNUM(c) || c == '_') && lex_p < lex_pend && is_identchar(*lex_p)) {
	    goto ternary;
	}
	else if (c == '\\') {
	    c = read_escape();
	}
	c &= 0xff;
	lex_state = EXPR_END;
	yylval.node = NEW_LIT(INT2FIX(c));
	return tINTEGER;

      case '&':
	if ((c = nextc()) == '&') {
	    lex_state = EXPR_BEG;
	    if ((c = nextc()) == '=') {
		yylval.id = tANDOP;
		lex_state = EXPR_BEG;
		return tOP_ASGN;
	    }
	    pushback(c);
	    return tANDOP;
	}
	else if (c == '=') {
	    yylval.id = '&';
	    lex_state = EXPR_BEG;
	    return tOP_ASGN;
	}
	pushback(c);
	if (IS_ARG() && space_seen && !ISSPACE(c)){
	    rb_warning("`&' interpreted as argument prefix");
	    c = tAMPER;
	}
	else if (lex_state == EXPR_BEG || lex_state == EXPR_MID) {
	    c = tAMPER;
	}
	else {
	    c = '&';
	}
	switch (lex_state) {
	  case EXPR_FNAME: case EXPR_DOT:
	    lex_state = EXPR_ARG; break;
	  default:
	    lex_state = EXPR_BEG;
	}
	return c;

      case '|':
	if ((c = nextc()) == '|') {
	    lex_state = EXPR_BEG;
	    if ((c = nextc()) == '=') {
		yylval.id = tOROP;
		lex_state = EXPR_BEG;
		return tOP_ASGN;
	    }
	    pushback(c);
	    return tOROP;
	}
	if (c == '=') {
	    yylval.id = '|';
	    lex_state = EXPR_BEG;
	    return tOP_ASGN;
	}
	if (lex_state == EXPR_FNAME || lex_state == EXPR_DOT) {
	    lex_state = EXPR_ARG;
	}
	else {
	    lex_state = EXPR_BEG;
	}
	pushback(c);
	return '|';

      case '+':
	c = nextc();
	if (lex_state == EXPR_FNAME || lex_state == EXPR_DOT) {
	    lex_state = EXPR_ARG;
	    if (c == '@') {
		return tUPLUS;
	    }
	    pushback(c);
	    return '+';
	}
	if (c == '=') {
	    yylval.id = '+';
	    lex_state = EXPR_BEG;
	    return tOP_ASGN;
	}
	if (lex_state == EXPR_BEG || lex_state == EXPR_MID ||
	    (IS_ARG() && space_seen && !ISSPACE(c))) {
	    if (IS_ARG()) arg_ambiguous();
	    lex_state = EXPR_BEG;
	    pushback(c);
	    if (ISDIGIT(c)) {
		c = '+';
		goto start_num;
	    }
	    return tUPLUS;
	}
	lex_state = EXPR_BEG;
	pushback(c);
	return '+';

      case '-':
	c = nextc();
	if (lex_state == EXPR_FNAME || lex_state == EXPR_DOT) {
	    lex_state = EXPR_ARG;
	    if (c == '@') {
		return tUMINUS;
	    }
	    pushback(c);
	    return '-';
	}
	if (c == '=') {
	    yylval.id = '-';
	    lex_state = EXPR_BEG;
	    return tOP_ASGN;
	}
	if (lex_state == EXPR_BEG || lex_state == EXPR_MID ||
	    (IS_ARG() && space_seen && !ISSPACE(c))) {
	    if (IS_ARG()) arg_ambiguous();
	    lex_state = EXPR_BEG;
	    pushback(c);
	    if (ISDIGIT(c)) {
		return tUMINUS_NUM;
	    }
	    return tUMINUS;
	}
	lex_state = EXPR_BEG;
	pushback(c);
	return '-';

      case '.':
	lex_state = EXPR_BEG;
	if ((c = nextc()) == '.') {
	    if ((c = nextc()) == '.') {
		return tDOT3;
	    }
	    pushback(c);
	    return tDOT2;
	}
	pushback(c);
	if (ISDIGIT(c)) {
	    yyerror("no .<digit> floating literal anymore; put 0 before dot");
	}
	lex_state = EXPR_DOT;
	return '.';

      start_num:
      case '0': case '1': case '2': case '3': case '4':
      case '5': case '6': case '7': case '8': case '9':
	{
	    int is_float, seen_point, seen_e, nondigit;

	    is_float = seen_point = seen_e = nondigit = 0;
	    lex_state = EXPR_END;
	    newtok();
	    if (c == '-' || c == '+') {
		tokadd(c);
		c = nextc();
	    }
	    if (c == '0') {
		int start = toklen();
		c = nextc();
		if (c == 'x' || c == 'X') {
		    /* hexadecimal */
		    c = nextc();
		    if (ISXDIGIT(c)) {
			do {
			    if (c == '_') {
				if (nondigit) break;
				nondigit = c;
				continue;
			    }
			    if (!ISXDIGIT(c)) break;
			    nondigit = 0;
			    tokadd(c);
			} while ((c = nextc()) != -1);
		    }
		    pushback(c);
		    tokfix();
		    if (toklen() == start) {
			yyerror("numeric literal without digits");
		    }
		    else if (nondigit) goto trailing_uc;
		    yylval.node = NEW_LIT(rb_cstr_to_inum(tok(), 16, Qfalse));
		    return tINTEGER;
		}
		if (c == 'b' || c == 'B') {
		    /* binary */
		    c = nextc();
		    if (c == '0' || c == '1') {
			do {
			    if (c == '_') {
				if (nondigit) break;
				nondigit = c;
				continue;
			    }
			    if (c != '0' && c != '1') break;
			    nondigit = 0;
			    tokadd(c);
			} while ((c = nextc()) != -1);
		    }
		    pushback(c);
		    tokfix();
		    if (toklen() == start) {
			yyerror("numeric literal without digits");
		    }
		    else if (nondigit) goto trailing_uc;
		    yylval.node = NEW_LIT(rb_cstr_to_inum(tok(), 2, Qfalse));
		    return tINTEGER;
		}
		if (c == 'd' || c == 'D') {
		    /* decimal */
		    c = nextc();
		    if (ISDIGIT(c)) {
			do {
			    if (c == '_') {
				if (nondigit) break;
				nondigit = c;
				continue;
			    }
			    if (!ISDIGIT(c)) break;
			    nondigit = 0;
			    tokadd(c);
			} while ((c = nextc()) != -1);
		    }
		    pushback(c);
		    tokfix();
		    if (toklen() == start) {
			yyerror("numeric literal without digits");
		    }
		    else if (nondigit) goto trailing_uc;
		    yylval.node = NEW_LIT(rb_cstr_to_inum(tok(), 10, Qfalse));
		    return tINTEGER;
		}
		if (c == '_') {
		    /* 0_0 */
		    goto octal_number;
		}
		if (c == 'o' || c == 'O') {
		    /* prefixed octal */
		    c = nextc();
		    if (c == '_') {
			yyerror("numeric literal without digits");
		    }
		}
		if (c >= '0' && c <= '7') {
		    /* octal */
		  octal_number:
	            do {
			if (c == '_') {
			    if (nondigit) break;
			    nondigit = c;
			    continue;
			}
			if (c < '0' || c > '7') break;
			nondigit = 0;
			tokadd(c);
		    } while ((c = nextc()) != -1);
		    if (toklen() > start) {
			pushback(c);
			tokfix();
			if (nondigit) goto trailing_uc;
			yylval.node = NEW_LIT(rb_cstr_to_inum(tok(), 8, Qfalse));
			return tINTEGER;
		    }
		    if (nondigit) {
			pushback(c);
			goto trailing_uc;
		    }
		}
		if (c > '7' && c <= '9') {
		    yyerror("Illegal octal digit");
		}
		else if (c == '.' || c == 'e' || c == 'E') {
		    tokadd('0');
		}
		else {
		    pushback(c);
		    yylval.node = NEW_LIT(INT2FIX(0));
		    return tINTEGER;
		}
	    }

	    for (;;) {
		switch (c) {
		  case '0': case '1': case '2': case '3': case '4':
		  case '5': case '6': case '7': case '8': case '9':
		    nondigit = 0;
		    tokadd(c);
		    break;

		  case '.':
		    if (nondigit) goto trailing_uc;
		    if (seen_point || seen_e) {
			goto decode_num;
		    }
		    else {
			int c0 = nextc();
			if (!ISDIGIT(c0)) {
			    pushback(c0);
			    goto decode_num;
			}
			c = c0;
		    }
		    tokadd('.');
		    tokadd(c);
		    is_float++;
		    seen_point++;
		    nondigit = 0;
		    break;

		  case 'e':
		  case 'E':
		    if (nondigit) {
			pushback(c);
			c = nondigit;
			goto decode_num;
		    }
		    if (seen_e) {
			goto decode_num;
		    }
		    tokadd(c);
		    seen_e++;
		    is_float++;
		    nondigit = c;
		    c = nextc();
		    if (c != '-' && c != '+') continue;
		    tokadd(c);
		    nondigit = c;
		    break;

		  case '_':	/* `_' in number just ignored */
		    if (nondigit) goto decode_num;
		    nondigit = c;
		    break;

		  default:
		    goto decode_num;
		}
		c = nextc();
	    }

	  decode_num:
	    pushback(c);
	    tokfix();
	    if (nondigit) {
		char tmp[30];
	      trailing_uc:
		sprintf(tmp, "trailing `%c' in number", nondigit);
		yyerror(tmp);
	    }
	    if (is_float) {
		double d = strtod(tok(), 0);
		if (errno == ERANGE) {
		    rb_warn("Float %s out of range", tok());
		    errno = 0;
		}
		yylval.node = NEW_LIT(rb_float_new(d));
		return tFLOAT;
	    }
	    yylval.node = NEW_LIT(rb_cstr_to_inum(tok(), 10, Qfalse));
	    return tINTEGER;
	}

      case ']':
      case '}':
      case ')':
	COND_LEXPOP();
	CMDARG_LEXPOP();
	lex_state = EXPR_END;
	return c;

      case ':':
	c = nextc();
	if (c == ':') {
	    if (lex_state == EXPR_BEG ||  lex_state == EXPR_MID ||
		lex_state == EXPR_CLASS || (IS_ARG() && space_seen)) {
		lex_state = EXPR_BEG;
		return tCOLON3;
	    }
	    lex_state = EXPR_DOT;
	    return tCOLON2;
	}
	if (lex_state == EXPR_END || lex_state == EXPR_ENDARG || ISSPACE(c)) {
	    pushback(c);
	    lex_state = EXPR_BEG;
	    return ':';
	}
	switch (c) {
	  case '\'':
	    lex_strterm = NEW_STRTERM(str_ssym, c, 0);
	    break;
	  case '"':
	    lex_strterm = NEW_STRTERM(str_dsym, c, 0);
	    break;
	  default:
	    pushback(c);
	    break;
	}
	lex_state = EXPR_FNAME;
	return tSYMBEG;

      case '/':
	if (lex_state == EXPR_BEG || lex_state == EXPR_MID) {
	    lex_strterm = NEW_STRTERM(str_regexp, '/', 0);
	    return tREGEXP_BEG;
	}
	if ((c = nextc()) == '=') {
	    yylval.id = '/';
	    lex_state = EXPR_BEG;
	    return tOP_ASGN;
	}
	pushback(c);
	if (IS_ARG() && space_seen) {
	    if (!ISSPACE(c)) {
		arg_ambiguous();
		lex_strterm = NEW_STRTERM(str_regexp, '/', 0);
		return tREGEXP_BEG;
	    }
	}
	switch (lex_state) {
	  case EXPR_FNAME: case EXPR_DOT:
	    lex_state = EXPR_ARG; break;
	  default:
	    lex_state = EXPR_BEG; break;
	}
	return '/';

      case '^':
	if ((c = nextc()) == '=') {
	    yylval.id = '^';
	    lex_state = EXPR_BEG;
	    return tOP_ASGN;
	}
	switch (lex_state) {
	  case EXPR_FNAME: case EXPR_DOT:
	    lex_state = EXPR_ARG; break;
	  default:
	    lex_state = EXPR_BEG; break;
	}
	pushback(c);
	return '^';

      case ';':
	command_start = Qtrue;
      case ',':
	lex_state = EXPR_BEG;
	return c;

      case '~':
	if (lex_state == EXPR_FNAME || lex_state == EXPR_DOT) {
	    if ((c = nextc()) != '@') {
		pushback(c);
	    }
	}
	switch (lex_state) {
	  case EXPR_FNAME: case EXPR_DOT:
	    lex_state = EXPR_ARG; break;
	  default:
	    lex_state = EXPR_BEG; break;
	}
	return '~';

      case '(':
	command_start = Qtrue;
	if (lex_state == EXPR_BEG || lex_state == EXPR_MID) {
	    c = tLPAREN;
	}
	else if (space_seen) {
	    if (lex_state == EXPR_CMDARG) {
		c = tLPAREN_ARG;
	    }
	    else if (lex_state == EXPR_ARG) {
		rb_warn("don't put space before argument parentheses");
		c = '(';
	    }
	}
	COND_PUSH(0);
	CMDARG_PUSH(0);
	lex_state = EXPR_BEG;
	return c;

      case '[':
	if (lex_state == EXPR_FNAME || lex_state == EXPR_DOT) {
	    lex_state = EXPR_ARG;
	    if ((c = nextc()) == ']') {
		if ((c = nextc()) == '=') {
		    return tASET;
		}
		pushback(c);
		return tAREF;
	    }
	    pushback(c);
	    return '[';
	}
	else if (lex_state == EXPR_BEG || lex_state == EXPR_MID) {
	    c = tLBRACK;
	}
	else if (IS_ARG() && space_seen) {
	    c = tLBRACK;
	}
	lex_state = EXPR_BEG;
	COND_PUSH(0);
	CMDARG_PUSH(0);
	return c;

      case '{':
	if (IS_ARG() || lex_state == EXPR_END)
	    c = '{';          /* block (primary) */
	else if (lex_state == EXPR_ENDARG)
	    c = tLBRACE_ARG;  /* block (expr) */
	else
	    c = tLBRACE;      /* hash */
	COND_PUSH(0);
	CMDARG_PUSH(0);
	lex_state = EXPR_BEG;
	return c;

      case '\\':
	c = nextc();
	if (c == '\n') {
	    space_seen = 1;
	    goto retry; /* skip \\n */
	}
	pushback(c);
	return '\\';

      case '%':
	if (lex_state == EXPR_BEG || lex_state == EXPR_MID) {
	    int term;
	    int paren;

	    c = nextc();
	  quotation:
	    if (!ISALNUM(c)) {
		term = c;
		c = 'Q';
	    }
	    else {
		term = nextc();
		if (ISALNUM(term) || ismbchar(term)) {
		    yyerror("unknown type of %string");
		    return 0;
		}
	    }
	    if (c == -1 || term == -1) {
		rb_compile_error("unterminated quoted string meets end of file");
		return 0;
	    }
	    paren = term;
	    if (term == '(') term = ')';
	    else if (term == '[') term = ']';
	    else if (term == '{') term = '}';
	    else if (term == '<') term = '>';
	    else paren = 0;

	    switch (c) {
	      case 'Q':
		lex_strterm = NEW_STRTERM(str_dquote, term, paren);
		return tSTRING_BEG;

	      case 'q':
		lex_strterm = NEW_STRTERM(str_squote, term, paren);
		return tSTRING_BEG;

	      case 'W':
		lex_strterm = NEW_STRTERM(str_dquote | STR_FUNC_QWORDS, term, paren);
		do {c = nextc();} while (ISSPACE(c));
		pushback(c);
		return tWORDS_BEG;

	      case 'w':
		lex_strterm = NEW_STRTERM(str_squote | STR_FUNC_QWORDS, term, paren);
		do {c = nextc();} while (ISSPACE(c));
		pushback(c);
		return tQWORDS_BEG;

	      case 'x':
		lex_strterm = NEW_STRTERM(str_xquote, term, paren);
		return tXSTRING_BEG;

	      case 'r':
		lex_strterm = NEW_STRTERM(str_regexp, term, paren);
		return tREGEXP_BEG;

	      case 's':
		lex_strterm = NEW_STRTERM(str_ssym, term, paren);
		lex_state = EXPR_FNAME;
		return tSYMBEG;

	      default:
		yyerror("unknown type of %string");
		return 0;
	    }
	}
	if ((c = nextc()) == '=') {
	    yylval.id = '%';
	    lex_state = EXPR_BEG;
	    return tOP_ASGN;
	}
	if (IS_ARG() && space_seen && !ISSPACE(c)) {
	    goto quotation;
	}
	switch (lex_state) {
	  case EXPR_FNAME: case EXPR_DOT:
	    lex_state = EXPR_ARG; break;
	  default:
	    lex_state = EXPR_BEG; break;
	}
	pushback(c);
	return '%';

      case '$':
	last_state = lex_state;
	lex_state = EXPR_END;
	newtok();
	c = nextc();
	switch (c) {
	  case '_':		/* $_: last read line string */
	    c = nextc();
	    if (is_identchar(c)) {
		tokadd('$');
		tokadd('_');
		break;
	    }
	    pushback(c);
	    c = '_';
	    /* fall through */
	  case '~':		/* $~: match-data */
	    local_cnt(c);
	    /* fall through */
	  case '*':		/* $*: argv */
	  case '$':		/* $$: pid */
	  case '?':		/* $?: last status */
	  case '!':		/* $!: error string */
	  case '@':		/* $@: error position */
	  case '/':		/* $/: input record separator */
	  case '\\':		/* $\: output record separator */
	  case ';':		/* $;: field separator */
	  case ',':		/* $,: output field separator */
	  case '.':		/* $.: last read line number */
	  case '=':		/* $=: ignorecase */
	  case ':':		/* $:: load path */
	  case '<':		/* $<: reading filename */
	  case '>':		/* $>: default output handle */
	  case '\"':		/* $": already loaded files */
	    tokadd('$');
	    tokadd(c);
	    tokfix();
	    yylval.id = rb_intern(tok());
	    return tGVAR;

	  case '-':
	    tokadd('$');
	    tokadd(c);
	    c = nextc();
	    if (is_identchar(c)) {
		tokadd(c);
	    }
	    else {
		pushback(c);
	    }
	  gvar:
	    tokfix();
	    yylval.id = rb_intern(tok());
	    /* xxx shouldn't check if valid option variable */
	    return tGVAR;

	  case '&':		/* $&: last match */
	  case '`':		/* $`: string before last match */
	  case '\'':		/* $': string after last match */
	  case '+':		/* $+: string matches last paren. */
	    if (last_state == EXPR_FNAME) {
		tokadd('$');
		tokadd(c);
		goto gvar;
	    }
	    yylval.node = NEW_BACK_REF(c);
	    return tBACK_REF;

	  case '1': case '2': case '3':
	  case '4': case '5': case '6':
	  case '7': case '8': case '9':
	    tokadd('$');
	    do {
		tokadd(c);
		c = nextc();
	    } while (ISDIGIT(c));
	    pushback(c);
	    if (last_state == EXPR_FNAME) goto gvar;
	    tokfix();
	    yylval.node = NEW_NTH_REF(atoi(tok()+1));
	    return tNTH_REF;

	  default:
	    if (!is_identchar(c)) {
		pushback(c);
		return '$';
	    }
	  case '0':
	    tokadd('$');
	}
	break;

      case '@':
	c = nextc();
	newtok();
	tokadd('@');
	if (c == '@') {
	    tokadd('@');
	    c = nextc();
	}
	if (ISDIGIT(c)) {
	    if (tokidx == 1) {
		rb_compile_error("`@%c' is not allowed as an instance variable name", c);
	    }
	    else {
		rb_compile_error("`@@%c' is not allowed as a class variable name", c);
	    }
	}
	if (!is_identchar(c)) {
	    pushback(c);
	    return '@';
	}
	break;

      case '_':
	if (was_bol() && whole_match_p("__END__", 7, 0)) {
	    ruby__end__seen = 1;
	    lex_lastline = 0;
	    return -1;
	}
	newtok();
	break;

      default:
	if (!is_identchar(c)) {
	    rb_compile_error("Invalid char `\\%03o' in expression", c);
	    goto retry;
	}

	newtok();
	break;
    }

    do {
	tokadd(c);
	if (ismbchar(c)) {
	    int i, len = mbclen(c)-1;

	    for (i = 0; i < len; i++) {
		c = nextc();
		tokadd(c);
	    }
	}
	c = nextc();
    } while (is_identchar(c));
    if ((c == '!' || c == '?') && is_identchar(tok()[0]) && !peek('=')) {
	tokadd(c);
    }
    else {
	pushback(c);
    }
    tokfix();

    {
	int result = 0;

	last_state = lex_state;
	switch (tok()[0]) {
	  case '$':
	    lex_state = EXPR_END;
	    result = tGVAR;
	    break;
	  case '@':
	    lex_state = EXPR_END;
	    if (tok()[1] == '@')
		result = tCVAR;
	    else
		result = tIVAR;
	    break;

	  default:
	    if (toklast() == '!' || toklast() == '?') {
		result = tFID;
	    }
	    else {
		if (lex_state == EXPR_FNAME) {
		    if ((c = nextc()) == '=' && !peek('~') && !peek('>') &&
			(!peek('=') || (lex_p + 1 < lex_pend && lex_p[1] == '>'))) {
			result = tIDENTIFIER;
			tokadd(c);
			tokfix();
		    }
		    else {
			pushback(c);
		    }
		}
		if (result == 0 && ISUPPER(tok()[0])) {
		    result = tCONSTANT;
		}
		else {
		    result = tIDENTIFIER;
		}
	    }

	    if (lex_state != EXPR_DOT) {
		struct kwtable *kw;

		/* See if it is a reserved word.  */
		kw = rb_reserved_word(tok(), toklen());
		if (kw) {
		    enum lex_state state = lex_state;
		    lex_state = kw->state;
		    if (state == EXPR_FNAME) {
			yylval.id = rb_intern(kw->name);
			return kw->id[0];
		    }
		    if (kw->id[0] == kDO) {
			if (COND_P()) return kDO_COND;
			if (CMDARG_P() && state != EXPR_CMDARG)
			    return kDO_BLOCK;
			if (state == EXPR_ENDARG)
			    return kDO_BLOCK;
			return kDO;
		    }
		    if (state == EXPR_BEG)
			return kw->id[0];
		    else {
			if (kw->id[0] != kw->id[1])
			    lex_state = EXPR_BEG;
			return kw->id[1];
		    }
		}
	    }

	    if (lex_state == EXPR_BEG ||
		lex_state == EXPR_MID ||
		lex_state == EXPR_DOT ||
		lex_state == EXPR_ARG ||
		lex_state == EXPR_CMDARG) {
		if (cmd_state) {
		    lex_state = EXPR_CMDARG;
		}
		else {
		    lex_state = EXPR_ARG;
		}
	    }
	    else {
		lex_state = EXPR_END;
	    }
	}
	yylval.id = rb_intern(tok());
	if (is_local_id(yylval.id) &&
	    last_state != EXPR_DOT &&
	    ((dyna_in_block() && rb_dvar_defined(yylval.id)) || local_id(yylval.id))) {
	    lex_state = EXPR_END;
	}
	return result;
    }
}

NODE*
rb_node_newnode(type, a0, a1, a2)
    enum node_type type;
    VALUE a0, a1, a2;
{
    NODE *n = (NODE*)rb_newobj();

    n->flags |= T_NODE;
    nd_set_type(n, type);
    nd_set_line(n, ruby_sourceline);
    n->nd_file = ruby_sourcefile;

    n->u1.value = a0;
    n->u2.value = a1;
    n->u3.value = a2;

    return n;
}

static enum node_type
nodetype(node)			/* for debug */
    NODE *node;
{
    return (enum node_type)nd_type(node);
}

static int
nodeline(node)
    NODE *node;
{
    return nd_line(node);
}

static NODE*
newline_node(node)
    NODE *node;
{
    NODE *nl = 0;
    if (node) {
	if (nd_type(node) == NODE_NEWLINE) return node;
        nl = NEW_NEWLINE(node);
        fixpos(nl, node);
        nl->nd_nth = nd_line(node);
    }
    return nl;
}

static void
fixpos(node, orig)
    NODE *node, *orig;
{
    if (!node) return;
    if (!orig) return;
    if (orig == (NODE*)1) return;
    node->nd_file = orig->nd_file;
    nd_set_line(node, nd_line(orig));
}

static void
parser_warning(node, mesg)
    NODE *node;
    const char *mesg;
{
    int line = ruby_sourceline;
    ruby_sourceline = nd_line(node);
    rb_warning(mesg);
    ruby_sourceline = line;
}

static void
parser_warn(node, mesg)
    NODE *node;
    const char *mesg;
{
    int line = ruby_sourceline;
    ruby_sourceline = nd_line(node);
    rb_warn(mesg);
    ruby_sourceline = line;
}

static NODE*
block_append(head, tail)
    NODE *head, *tail;
{
    NODE *end, *h = head;

    if (tail == 0) return head;

  again:
    if (h == 0) return tail;
    switch (nd_type(h)) {
      case NODE_NEWLINE:
	h = h->nd_next;
	goto again;
      case NODE_LIT:
      case NODE_STR:
	parser_warning(h, "unused literal ignored");
	return tail;
      default:
	h = end = NEW_BLOCK(head);
	end->nd_end = end;
	fixpos(end, head);
	head = end;
	break;
      case NODE_BLOCK:
	end = h->nd_end;
	break;
    }

    if (RTEST(ruby_verbose)) {
	NODE *nd = end->nd_head;
      newline:
	switch (nd_type(nd)) {
	  case NODE_RETURN:
	  case NODE_BREAK:
	  case NODE_NEXT:
	  case NODE_REDO:
	  case NODE_RETRY:
	    parser_warning(nd, "statement not reached");
	    break;

	case NODE_NEWLINE:
	    nd = nd->nd_next;
	    goto newline;

	  default:
	    break;
	}
    }

    if (nd_type(tail) != NODE_BLOCK) {
	tail = NEW_BLOCK(tail);
	tail->nd_end = tail;
    }
    end->nd_next = tail;
    h->nd_end = tail->nd_end;
    return head;
}

/* append item to the list */
static NODE*
list_append(list, item)
    NODE *list, *item;
{
    NODE *last;

    if (list == 0) return NEW_LIST(item);
    if (list->nd_next) {
	last = list->nd_next->nd_end;
    }
    else {
	last = list;
    }

    list->nd_alen += 1;
    last->nd_next = NEW_LIST(item);
    list->nd_next->nd_end = last->nd_next;
    return list;
}

/* concat two lists */
static NODE*
list_concat(head, tail)
    NODE *head, *tail;
{
    NODE *last;

    if (head->nd_next) {
	last = head->nd_next->nd_end;
    }
    else {
	last = head;
    }

    head->nd_alen += tail->nd_alen;
    last->nd_next = tail;
    if (tail->nd_next) {
	head->nd_next->nd_end = tail->nd_next->nd_end;
    }
    else {
	head->nd_next->nd_end = tail;
    }

    return head;
}

/* concat two string literals */
static NODE *
literal_concat(head, tail)
    NODE *head, *tail;
{
    enum node_type htype;

    if (!head) return tail;
    if (!tail) return head;

    htype = nd_type(head);
    if (htype == NODE_EVSTR) {
	NODE *node = NEW_DSTR(rb_str_new(0, 0));
	head = list_append(node, head);
    }
    switch (nd_type(tail)) {
      case NODE_STR:
	if (htype == NODE_STR) {
	    rb_str_concat(head->nd_lit, tail->nd_lit);
	    rb_gc_force_recycle((VALUE)tail);
	}
	else {
	    list_append(head, tail);
	}
	break;

      case NODE_DSTR:
	if (htype == NODE_STR) {
	    rb_str_concat(head->nd_lit, tail->nd_lit);
	    tail->nd_lit = head->nd_lit;
	    rb_gc_force_recycle((VALUE)head);
	    head = tail;
	}
	else {
	    nd_set_type(tail, NODE_ARRAY);
	    tail->nd_head = NEW_STR(tail->nd_lit);
	    list_concat(head, tail);
	}
	break;

      case NODE_EVSTR:
	if (htype == NODE_STR) {
	    nd_set_type(head, NODE_DSTR);
	    head->nd_alen = 1;
	}
	list_append(head, tail);
	break;
    }
    return head;
}

static NODE *
evstr2dstr(node)
    NODE *node;
{
    if (nd_type(node) == NODE_EVSTR) {
	node = list_append(NEW_DSTR(rb_str_new(0, 0)), node);
    }
    return node;
}

static NODE *
new_evstr(node)
    NODE *node;
{
    NODE *head = node;

  again:
    if (node) {
	switch (nd_type(node)) {
	  case NODE_STR: case NODE_DSTR: case NODE_EVSTR:
	    return node;
	  case NODE_NEWLINE:
	    node = node->nd_next;
	    goto again;
	}
    }
    return NEW_EVSTR(head);
}

static NODE *
call_op(recv, id, narg, arg1)
    NODE *recv;
    ID id;
    int narg;
    NODE *arg1;
{
    value_expr(recv);
    if (narg == 1) {
	value_expr(arg1);
	arg1 = NEW_LIST(arg1);
    }
    else {
	arg1 = 0;
    }
    return NEW_CALL(recv, id, arg1);
}

static NODE*
match_gen(node1, node2)
    NODE *node1;
    NODE *node2;
{
    local_cnt('~');

    value_expr(node1);
    value_expr(node2);
    if (node1) {
	switch (nd_type(node1)) {
	  case NODE_DREGX:
	  case NODE_DREGX_ONCE:
	    return NEW_MATCH2(node1, node2);

	  case NODE_LIT:
	    if (TYPE(node1->nd_lit) == T_REGEXP) {
		return NEW_MATCH2(node1, node2);
	    }
	}
    }

    if (node2) {
	switch (nd_type(node2)) {
	  case NODE_DREGX:
	  case NODE_DREGX_ONCE:
	    return NEW_MATCH3(node2, node1);

	  case NODE_LIT:
	    if (TYPE(node2->nd_lit) == T_REGEXP) {
		return NEW_MATCH3(node2, node1);
	    }
	}
    }

    return NEW_CALL(node1, tMATCH, NEW_LIST(node2));
}

static NODE*
gettable(id)
    ID id;
{
    if (id == kSELF) {
	return NEW_SELF();
    }
    else if (id == kNIL) {
	return NEW_NIL();
    }
    else if (id == kTRUE) {
	return NEW_TRUE();
    }
    else if (id == kFALSE) {
	return NEW_FALSE();
    }
    else if (id == k__FILE__) {
	return NEW_STR(rb_str_new2(ruby_sourcefile));
    }
    else if (id == k__LINE__) {
	return NEW_LIT(INT2FIX(ruby_sourceline));
    }
    else if (is_local_id(id)) {
	if (dyna_in_block() && rb_dvar_defined(id)) return NEW_DVAR(id);
	if (local_id(id)) return NEW_LVAR(id);
	/* method call without arguments */
#if 0
	/* Rite will warn this */
	rb_warn("ambiguous identifier; %s() or self.%s is better for method call",
		rb_id2name(id), rb_id2name(id));
#endif
	return NEW_VCALL(id);
    }
    else if (is_global_id(id)) {
	return NEW_GVAR(id);
    }
    else if (is_instance_id(id)) {
	return NEW_IVAR(id);
    }
    else if (is_const_id(id)) {
	return NEW_CONST(id);
    }
    else if (is_class_id(id)) {
	return NEW_CVAR(id);
    }
    rb_compile_error("identifier %s is not valid", rb_id2name(id));
    return 0;
}

static NODE*
assignable(id, val)
    ID id;
    NODE *val;
{
    value_expr(val);
    if (id == kSELF) {
	yyerror("Can't change the value of self");
    }
    else if (id == kNIL) {
	yyerror("Can't assign to nil");
    }
    else if (id == kTRUE) {
	yyerror("Can't assign to true");
    }
    else if (id == kFALSE) {
	yyerror("Can't assign to false");
    }
    else if (id == k__FILE__) {
	yyerror("Can't assign to __FILE__");
    }
    else if (id == k__LINE__) {
	yyerror("Can't assign to __LINE__");
    }
    else if (is_local_id(id)) {
	if (rb_dvar_curr(id)) {
	    return NEW_DASGN_CURR(id, val);
	}
	else if (rb_dvar_defined(id)) {
	    return NEW_DASGN(id, val);
	}
	else if (local_id(id) || !dyna_in_block()) {
	    return NEW_LASGN(id, val);
	}
	else{
	    rb_dvar_push(id, Qnil);
	    return NEW_DASGN_CURR(id, val);
	}
    }
    else if (is_global_id(id)) {
	return NEW_GASGN(id, val);
    }
    else if (is_instance_id(id)) {
	return NEW_IASGN(id, val);
    }
    else if (is_const_id(id)) {
	if (in_def || in_single)
	    yyerror("dynamic constant assignment");
	return NEW_CDECL(id, val, 0);
    }
    else if (is_class_id(id)) {
	if (in_def || in_single) return NEW_CVASGN(id, val);
	return NEW_CVDECL(id, val);
    }
    else {
	rb_compile_error("identifier %s is not valid", rb_id2name(id));
    }
    return 0;
}

static NODE *
aryset(recv, idx)
    NODE *recv, *idx;
{
    if (recv && nd_type(recv) == NODE_SELF)
	recv = (NODE *)1;
    else
	value_expr(recv);
    return NEW_ATTRASGN(recv, tASET, idx);
}

ID
rb_id_attrset(id)
    ID id;
{
    id &= ~ID_SCOPE_MASK;
    id |= ID_ATTRSET;
    return id;
}

static NODE *
attrset(recv, id)
    NODE *recv;
    ID id;
{
    if (recv && nd_type(recv) == NODE_SELF)
	recv = (NODE *)1;
    else
	value_expr(recv);
    return NEW_ATTRASGN(recv, rb_id_attrset(id), 0);
}

static void
rb_backref_error(node)
    NODE *node;
{
    switch (nd_type(node)) {
      case NODE_NTH_REF:
	rb_compile_error("Can't set variable $%d", node->nd_nth);
	break;
      case NODE_BACK_REF:
	rb_compile_error("Can't set variable $%c", (int)node->nd_nth);
	break;
    }
}

static NODE *
arg_concat(node1, node2)
    NODE *node1;
    NODE *node2;
{
    if (!node2) return node1;
    return NEW_ARGSCAT(node1, node2);
}

static NODE *
arg_add(node1, node2)
    NODE *node1;
    NODE *node2;
{
    if (!node1) return NEW_LIST(node2);
    if (nd_type(node1) == NODE_ARRAY) {
	return list_append(node1, node2);
    }
    else {
	return NEW_ARGSPUSH(node1, node2);
    }
}

static NODE*
node_assign(lhs, rhs)
    NODE *lhs, *rhs;
{
    if (!lhs) return 0;

    value_expr(rhs);
    switch (nd_type(lhs)) {
      case NODE_GASGN:
      case NODE_IASGN:
      case NODE_LASGN:
      case NODE_DASGN:
      case NODE_DASGN_CURR:
      case NODE_MASGN:
      case NODE_CDECL:
      case NODE_CVDECL:
      case NODE_CVASGN:
	lhs->nd_value = rhs;
	break;

      case NODE_ATTRASGN:
      case NODE_CALL:
	lhs->nd_args = arg_add(lhs->nd_args, rhs);
	break;

      default:
	/* should not happen */
	break;
    }

    return lhs;
}

static int
value_expr0(node)
    NODE *node;
{
    int cond = 0;

    while (node) {
	switch (nd_type(node)) {
	  case NODE_DEFN:
	  case NODE_DEFS:
	    parser_warning(node, "void value expression");
	    return Qfalse;

	  case NODE_RETURN:
	  case NODE_BREAK:
	  case NODE_NEXT:
	  case NODE_REDO:
	  case NODE_RETRY:
	    if (!cond) yyerror("void value expression");
	    /* or "control never reach"? */
	    return Qfalse;

	  case NODE_BLOCK:
	    while (node->nd_next) {
		node = node->nd_next;
	    }
	    node = node->nd_head;
	    break;

	  case NODE_BEGIN:
	    node = node->nd_body;
	    break;

	  case NODE_IF:
	    if (!value_expr(node->nd_body)) return Qfalse;
	    node = node->nd_else;
	    break;

	  case NODE_AND:
	  case NODE_OR:
	    cond = 1;
	    node = node->nd_2nd;
	    break;

	  case NODE_NEWLINE:
	    node = node->nd_next;
	    break;

	  default:
	    return Qtrue;
	}
    }

    return Qtrue;
}

static void
void_expr0(node)
    NODE *node;
{
    char *useless = 0;

    if (!RTEST(ruby_verbose)) return;

  again:
    if (!node) return;
    switch (nd_type(node)) {
      case NODE_NEWLINE:
	node = node->nd_next;
	goto again;

      case NODE_CALL:
	switch (node->nd_mid) {
	  case '+':
	  case '-':
	  case '*':
	  case '/':
	  case '%':
	  case tPOW:
	  case tUPLUS:
	  case tUMINUS:
	  case '|':
	  case '^':
	  case '&':
	  case tCMP:
	  case '>':
	  case tGEQ:
	  case '<':
	  case tLEQ:
	  case tEQ:
	  case tNEQ:
	    useless = rb_id2name(node->nd_mid);
	    break;
	}
	break;

      case NODE_LVAR:
      case NODE_DVAR:
      case NODE_GVAR:
      case NODE_IVAR:
      case NODE_CVAR:
      case NODE_NTH_REF:
      case NODE_BACK_REF:
	useless = "a variable";
	break;
      case NODE_CONST:
      case NODE_CREF:
	useless = "a constant";
	break;
      case NODE_LIT:
      case NODE_STR:
      case NODE_DSTR:
      case NODE_DREGX:
      case NODE_DREGX_ONCE:
	useless = "a literal";
	break;
      case NODE_COLON2:
      case NODE_COLON3:
	useless = "::";
	break;
      case NODE_DOT2:
	useless = "..";
	break;
      case NODE_DOT3:
	useless = "...";
	break;
      case NODE_SELF:
	useless = "self";
	break;
      case NODE_NIL:
	useless = "nil";
	break;
      case NODE_TRUE:
	useless = "true";
	break;
      case NODE_FALSE:
	useless = "false";
	break;
      case NODE_DEFINED:
	useless = "defined?";
	break;
    }

    if (useless) {
	int line = ruby_sourceline;

	ruby_sourceline = nd_line(node);
	rb_warn("useless use of %s in void context", useless);
	ruby_sourceline = line;
    }
}

static void
void_stmts(node)
    NODE *node;
{
    if (!RTEST(ruby_verbose)) return;
    if (!node) return;
    if (nd_type(node) != NODE_BLOCK) return;

    for (;;) {
	if (!node->nd_next) return;
	void_expr(node->nd_head);
	node = node->nd_next;
    }
}

static NODE *
remove_begin(node)
    NODE *node;
{
    NODE **n = &node;
    while (*n) {
	switch (nd_type(*n)) {
	  case NODE_NEWLINE:
	    n = &(*n)->nd_next;
	    continue;
	  case NODE_BEGIN:
	    *n = (*n)->nd_body;
	  default:
	    return node;
	}
    }
    return node;
}

static int
assign_in_cond(node)
    NODE *node;
{
    switch (nd_type(node)) {
      case NODE_MASGN:
	yyerror("multiple assignment in conditional");
	return 1;

      case NODE_LASGN:
      case NODE_DASGN:
      case NODE_GASGN:
      case NODE_IASGN:
	break;

      case NODE_NEWLINE:
      default:
	return 0;
    }

    switch (nd_type(node->nd_value)) {
      case NODE_LIT:
      case NODE_STR:
      case NODE_NIL:
      case NODE_TRUE:
      case NODE_FALSE:
	/* reports always */
	parser_warn(node->nd_value, "found = in conditional, should be ==");
	return 1;

      case NODE_DSTR:
      case NODE_XSTR:
      case NODE_DXSTR:
      case NODE_EVSTR:
      case NODE_DREGX:
      default:
	break;
    }
#if 0
    if (assign_in_cond(node->nd_value) == 0) {
	parser_warning(node->nd_value, "assignment in condition");
    }
#endif
    return 1;
}

static int
e_option_supplied()
{
    if (strcmp(ruby_sourcefile, "-e") == 0)
	return Qtrue;
    return Qfalse;
}

static void
warn_unless_e_option(node, str)
    NODE *node;
    const char *str;
{
    if (!e_option_supplied()) parser_warn(node, str);
}

static void
warning_unless_e_option(node, str)
    NODE *node;
    const char *str;
{
    if (!e_option_supplied()) parser_warning(node, str);
}

static NODE *cond0();

static NODE*
range_op(node)
    NODE *node;
{
    enum node_type type;

    if (!e_option_supplied()) return node;
    if (node == 0) return 0;

    value_expr(node);
    node = cond0(node);
    type = nd_type(node);
    if (type == NODE_NEWLINE) {
	node = node->nd_next;
	type = nd_type(node);
    }
    if (type == NODE_LIT && FIXNUM_P(node->nd_lit)) {
	warn_unless_e_option(node, "integer literal in conditional range");
	return call_op(node,tEQ,1,NEW_GVAR(rb_intern("$.")));
    }
    return node;
}

static int
literal_node(node)
    NODE *node;
{
    if (!node) return 1;	/* same as NODE_NIL */
    switch (nd_type(node)) {
      case NODE_LIT:
      case NODE_STR:
      case NODE_DSTR:
      case NODE_EVSTR:
      case NODE_DREGX:
      case NODE_DREGX_ONCE:
      case NODE_DSYM:
	return 2;
      case NODE_TRUE:
      case NODE_FALSE:
      case NODE_NIL:
	return 1;
    }
    return 0;
}

static NODE*
cond0(node)
    NODE *node;
{
    if (node == 0) return 0;
    assign_in_cond(node);

    switch (nd_type(node)) {
      case NODE_DSTR:
      case NODE_EVSTR:
      case NODE_STR:
	rb_warn("string literal in condition");
	break;

      case NODE_DREGX:
      case NODE_DREGX_ONCE:
	warning_unless_e_option(node, "regex literal in condition");
	local_cnt('_');
	local_cnt('~');
	return NEW_MATCH2(node, NEW_GVAR(rb_intern("$_")));

      case NODE_AND:
      case NODE_OR:
	node->nd_1st = cond0(node->nd_1st);
	node->nd_2nd = cond0(node->nd_2nd);
	break;

      case NODE_DOT2:
      case NODE_DOT3:
	node->nd_beg = range_op(node->nd_beg);
	node->nd_end = range_op(node->nd_end);
	if (nd_type(node) == NODE_DOT2) nd_set_type(node,NODE_FLIP2);
	else if (nd_type(node) == NODE_DOT3) nd_set_type(node, NODE_FLIP3);
	node->nd_cnt = local_append(internal_id());
	if (!e_option_supplied()) {
	    int b = literal_node(node->nd_beg);
	    int e = literal_node(node->nd_end);
	    if ((b == 1 && e == 1) || (b + e >= 2 && RTEST(ruby_verbose))) {
		parser_warn(node, "range literal in condition");
	    }
	}
	break;

      case NODE_DSYM:
	parser_warning(node, "literal in condition");
	break;

      case NODE_LIT:
	if (TYPE(node->nd_lit) == T_REGEXP) {
	    warn_unless_e_option(node, "regex literal in condition");
	    nd_set_type(node, NODE_MATCH);
	    local_cnt('_');
	    local_cnt('~');
	}
	else {
	    parser_warning(node, "literal in condition");
	}
      default:
	break;
    }
    return node;
}

static NODE*
cond(node)
    NODE *node;
{
    if (node == 0) return 0;
    value_expr(node);
    if (nd_type(node) == NODE_NEWLINE){
	node->nd_next = cond0(node->nd_next);
	return node;
    }
    return cond0(node);
}

static NODE*
logop(type, left, right)
    enum node_type type;
    NODE *left, *right;
{
    value_expr(left);
    if (left && nd_type(left) == type) {
	NODE *node = left, *second;
	while ((second = node->nd_2nd) != 0 && nd_type(second) == type) {
	    node = second;
	}
	node->nd_2nd = NEW_NODE(type, second, right, 0);
	return left;
    }
    return NEW_NODE(type, left, right, 0);
}

static int
cond_negative(nodep)
    NODE **nodep;
{
    NODE *c = *nodep;

    if (!c) return 0;
    switch (nd_type(c)) {
      case NODE_NOT:
	*nodep = c->nd_body;
	return 1;
      case NODE_NEWLINE:
	if (c->nd_next && nd_type(c->nd_next) == NODE_NOT) {
	    c->nd_next = c->nd_next->nd_body;
	    return 1;
	}
    }
    return 0;
}

static void
no_blockarg(node)
    NODE *node;
{
    if (node && nd_type(node) == NODE_BLOCK_PASS) {
	rb_compile_error("block argument should not be given");
    }
}

static NODE *
ret_args(node)
    NODE *node;
{
    if (node) {
	no_blockarg(node);
	if (nd_type(node) == NODE_ARRAY && node->nd_next == 0) {
	    node = node->nd_head;
	}
	if (node && nd_type(node) == NODE_SPLAT) {
	    node = NEW_SVALUE(node);
	}
    }
    return node;
}

static NODE *
new_yield(node)
    NODE *node;
{
    long state = Qtrue;

    if (node) {
        no_blockarg(node);
        if (nd_type(node) == NODE_ARRAY && node->nd_next == 0) {
            node = node->nd_head;
            state = Qfalse;
        }
        if (node && nd_type(node) == NODE_SPLAT) {
            state = Qtrue;
        }
    }
    else {
        state = Qfalse;
    }
    return NEW_YIELD(node, state);
}

static NODE*
negate_lit(node)
    NODE *node;
{
    switch (TYPE(node->nd_lit)) {
      case T_FIXNUM:
	node->nd_lit = LONG2FIX(-FIX2LONG(node->nd_lit));
	break;
      case T_BIGNUM:
	node->nd_lit = rb_funcall(node->nd_lit,tUMINUS,0,0);
	break;
      case T_FLOAT:
	RFLOAT(node->nd_lit)->value = -RFLOAT(node->nd_lit)->value;
	break;
      default:
	break;
    }
    return node;
}

static NODE *
arg_blk_pass(node1, node2)
    NODE *node1;
    NODE *node2;
{
    if (node2) {
	node2->nd_head = node1;
	return node2;
    }
    return node1;
}

static NODE*
arg_prepend(node1, node2)
    NODE *node1, *node2;
{
    switch (nd_type(node2)) {
      case NODE_ARRAY:
	return list_concat(NEW_LIST(node1), node2);

      case NODE_SPLAT:
	return arg_concat(node1, node2->nd_head);

      case NODE_BLOCK_PASS:
	node2->nd_body = arg_prepend(node1, node2->nd_body);
	return node2;

      default:
	rb_bug("unknown nodetype(%d) for arg_prepend", nd_type(node2));
    }
    return 0;			/* not reached */
}

static NODE*
new_call(r,m,a)
    NODE *r;
    ID m;
    NODE *a;
{
    if (a && nd_type(a) == NODE_BLOCK_PASS) {
	a->nd_iter = NEW_CALL(r,m,a->nd_head);
	return a;
    }
    return NEW_CALL(r,m,a);
}

static NODE*
new_fcall(m,a)
    ID m;
    NODE *a;
{
    if (a && nd_type(a) == NODE_BLOCK_PASS) {
	a->nd_iter = NEW_FCALL(m,a->nd_head);
	return a;
    }
    return NEW_FCALL(m,a);
}

static NODE*
new_super(a)
    NODE *a;
{
    if (a && nd_type(a) == NODE_BLOCK_PASS) {
	a->nd_iter = NEW_SUPER(a->nd_head);
	return a;
    }
    return NEW_SUPER(a);
}

static struct local_vars {
    ID *tbl;
    int nofree;
    int cnt;
    int dlev;
    struct RVarmap* dyna_vars;
    struct local_vars *prev;
} *lvtbl;

static void
local_push(top)
    int top;
{
    struct local_vars *local;

    local = ALLOC(struct local_vars);
    local->prev = lvtbl;
    local->nofree = 0;
    local->cnt = 0;
    local->tbl = 0;
    local->dlev = 0;
    local->dyna_vars = ruby_dyna_vars;
    lvtbl = local;
    if (!top) {
	/* preserve reference for GC, but link should be cut. */
	rb_dvar_push(0, (VALUE)ruby_dyna_vars);
	ruby_dyna_vars->next = 0;
    }
}

static void
local_pop()
{
    struct local_vars *local = lvtbl->prev;

    if (lvtbl->tbl) {
	if (!lvtbl->nofree) xfree(lvtbl->tbl);
	else lvtbl->tbl[0] = lvtbl->cnt;
    }
    ruby_dyna_vars = lvtbl->dyna_vars;
    xfree(lvtbl);
    lvtbl = local;
}

static ID*
local_tbl()
{
    lvtbl->nofree = 1;
    return lvtbl->tbl;
}

static int
local_append(id)
    ID id;
{
    if (lvtbl->tbl == 0) {
	lvtbl->tbl = ALLOC_N(ID, 4);
	lvtbl->tbl[0] = 0;
	lvtbl->tbl[1] = '_';
	lvtbl->tbl[2] = '~';
	lvtbl->cnt = 2;
	if (id == '_') return 0;
	if (id == '~') return 1;
    }
    else {
	REALLOC_N(lvtbl->tbl, ID, lvtbl->cnt+2);
    }

    lvtbl->tbl[lvtbl->cnt+1] = id;
    return lvtbl->cnt++;
}

static int
local_cnt(id)
    ID id;
{
    int cnt, max;

    if (id == 0) return lvtbl->cnt;

    for (cnt=1, max=lvtbl->cnt+1; cnt<max;cnt++) {
	if (lvtbl->tbl[cnt] == id) return cnt-1;
    }
    return local_append(id);
}

static int
local_id(id)
    ID id;
{
    int i, max;

    if (lvtbl == 0) return Qfalse;
    for (i=3, max=lvtbl->cnt+1; i<max; i++) {
	if (lvtbl->tbl[i] == id) return Qtrue;
    }
    return Qfalse;
}

static void
top_local_init()
{
    local_push(1);
    lvtbl->cnt = ruby_scope->local_tbl?ruby_scope->local_tbl[0]:0;
    if (lvtbl->cnt > 0) {
	lvtbl->tbl = ALLOC_N(ID, lvtbl->cnt+3);
	MEMCPY(lvtbl->tbl, ruby_scope->local_tbl, ID, lvtbl->cnt+1);
    }
    else {
	lvtbl->tbl = 0;
    }
    if (ruby_dyna_vars)
	lvtbl->dlev = 1;
    else
	lvtbl->dlev = 0;
}

static void
top_local_setup()
{
    int len = lvtbl->cnt;
    int i;

    if (len > 0) {
	i = ruby_scope->local_tbl?ruby_scope->local_tbl[0]:0;

	if (i < len) {
	    if (i == 0 || (ruby_scope->flags & SCOPE_MALLOC) == 0) {
		VALUE *vars = ALLOC_N(VALUE, len+1);
		if (ruby_scope->local_vars) {
		    *vars++ = ruby_scope->local_vars[-1];
		    MEMCPY(vars, ruby_scope->local_vars, VALUE, i);
		    rb_mem_clear(vars+i, len-i);
		}
		else {
		    *vars++ = (VALUE)ruby_scope;
		    rb_mem_clear(vars, len);
		}
		ruby_scope->local_vars = vars;
		ruby_scope->flags |= SCOPE_MALLOC;
	    }
	    else {
		VALUE *vars = ruby_scope->local_vars-1;
		REALLOC_N(vars, VALUE, len+1);
		ruby_scope->local_vars = vars+1;
		rb_mem_clear(ruby_scope->local_vars+i, len-i);
	    }
	    if (ruby_scope->local_tbl && ruby_scope->local_vars[-1] == 0) {
		xfree(ruby_scope->local_tbl);
	    }
	    ruby_scope->local_tbl = local_tbl();
	}
    }
    local_pop();
}

static struct RVarmap*
dyna_push()
{
    struct RVarmap* vars = ruby_dyna_vars;

    rb_dvar_push(0, 0);
    lvtbl->dlev++;
    return vars;
}

static void
dyna_pop(vars)
    struct RVarmap* vars;
{
    lvtbl->dlev--;
    ruby_dyna_vars = vars;
}

static int
dyna_in_block()
{
    return (lvtbl->dlev > 0);
}

static NODE *
dyna_init(node, pre)
    NODE *node;
    struct RVarmap *pre;
{
    struct RVarmap *post = ruby_dyna_vars;
    NODE *var;

    if (!node || !post || pre == post) return node;
    for (var = 0; post != pre && post->id; post = post->next) {
	var = NEW_DASGN_CURR(post->id, var);
    }
    return block_append(var, node);
}

int
ruby_parser_stack_on_heap()
{
#if defined(YYMALLOC)
    return Qfalse;
#else
    return Qtrue;
#endif
}

void
rb_gc_mark_parser()
{
#if defined YYMALLOC
    rb_gc_mark((VALUE)parser_heap);
#elif defined yystacksize
    if (yyvsp) rb_gc_mark_locations((VALUE *)yyvs, (VALUE *)yyvsp);
#endif

    if (!ruby_in_compile) return;

    rb_gc_mark_maybe((VALUE)yylval.node);
    rb_gc_mark(ruby_debug_lines);
    rb_gc_mark(lex_lastline);
    rb_gc_mark(lex_input);
    rb_gc_mark((VALUE)lex_strterm);
}

void
rb_parser_append_print()
{
    ruby_eval_tree =
	block_append(ruby_eval_tree,
		     NEW_FCALL(rb_intern("print"),
			       NEW_ARRAY(NEW_GVAR(rb_intern("$_")))));
}

void
rb_parser_while_loop(chop, split)
    int chop, split;
{
    if (split) {
	ruby_eval_tree =
	    block_append(NEW_GASGN(rb_intern("$F"),
				   NEW_CALL(NEW_GVAR(rb_intern("$_")),
					    rb_intern("split"), 0)),
				   ruby_eval_tree);
    }
    if (chop) {
	ruby_eval_tree =
	    block_append(NEW_CALL(NEW_GVAR(rb_intern("$_")),
				  rb_intern("chop!"), 0), ruby_eval_tree);
    }
    ruby_eval_tree = NEW_OPT_N(ruby_eval_tree);
}

static struct {
    ID token;
    char *name;
} op_tbl[] = {
    {tDOT2,	".."},
    {tDOT3,	"..."},
    {'+',	"+"},
    {'-',	"-"},
    {'+',	"+(binary)"},
    {'-',	"-(binary)"},
    {'*',	"*"},
    {'/',	"/"},
    {'%',	"%"},
    {tPOW,	"**"},
    {tUPLUS,	"+@"},
    {tUMINUS,	"-@"},
    {tUPLUS,	"+(unary)"},
    {tUMINUS,	"-(unary)"},
    {'|',	"|"},
    {'^',	"^"},
    {'&',	"&"},
    {tCMP,	"<=>"},
    {'>',	">"},
    {tGEQ,	">="},
    {'<',	"<"},
    {tLEQ,	"<="},
    {tEQ,	"=="},
    {tEQQ,	"==="},
    {tNEQ,	"!="},
    {tMATCH,	"=~"},
    {tNMATCH,	"!~"},
    {'!',	"!"},
    {'~',	"~"},
    {'!',	"!(unary)"},
    {'~',	"~(unary)"},
    {'!',	"!@"},
    {'~',	"~@"},
    {tAREF,	"[]"},
    {tASET,	"[]="},
    {tLSHFT,	"<<"},
    {tRSHFT,	">>"},
    {tCOLON2,	"::"},
    {'`',	"`"},
    {0,	0}
};

static st_table *sym_tbl;
static st_table *sym_rev_tbl;

void
Init_sym()
{
    sym_tbl = st_init_strtable_with_size(200);
    sym_rev_tbl = st_init_numtable_with_size(200);
}

static ID last_id = tLAST_TOKEN;

static ID
internal_id()
{
    return ID_INTERNAL | (++last_id << ID_SCOPE_SHIFT);
}

static int
is_special_global_name(m)
    const char *m;
{
    switch (*m) {
      case '~': case '*': case '$': case '?': case '!': case '@':
      case '/': case '\\': case ';': case ',': case '.': case '=':
      case ':': case '<': case '>': case '\"':
      case '&': case '`': case '\'': case '+':
      case '0':
	++m;
	break;
      case '-':
	++m;
	if (is_identchar(*m)) m += mbclen(*m);
	break;
      default:
	if (!ISDIGIT(*m)) return 0;
	do ++m; while (ISDIGIT(*m));
    }
    return !*m;
}

int
rb_symname_p(name)
    const char *name;
{
    const char *m = name;
    int localid = Qfalse;

    if (!m) return Qfalse;
    switch (*m) {
      case '\0':
	return Qfalse;

      case '$':
	if (is_special_global_name(++m)) return Qtrue;
	goto id;

      case '@':
	if (*++m == '@') ++m;
	goto id;

      case '<':
	switch (*++m) {
	  case '<': ++m; break;
	  case '=': if (*++m == '>') ++m; break;
	  default: break;
	}
	break;

      case '>':
	switch (*++m) {
	  case '>': case '=': ++m; break;
	}
	break;

      case '=':
	switch (*++m) {
	  case '~': ++m; break;
	  case '=': if (*++m == '=') ++m; break;
	  default: return Qfalse;
	}
	break;

      case '*':
	if (*++m == '*') ++m;
	break;

      case '+': case '-':
	if (*++m == '@') ++m;
	break;

      case '|': case '^': case '&': case '/': case '%': case '~': case '`':
	++m;
	break;

      case '[':
	if (*++m != ']') return Qfalse;
	if (*++m == '=') ++m;
	break;

      default:
	localid = !ISUPPER(*m);
      id:
	if (*m != '_' && !ISALPHA(*m) && !ismbchar(*m)) return Qfalse;
	while (is_identchar(*m)) m += mbclen(*m);
	if (localid) {
	    switch (*m) {
	      case '!': case '?': case '=': ++m;
	    }
	}
	break;
    }
    return *m ? Qfalse : Qtrue;
}

ID
rb_intern(name)
    const char *name;
{
    const char *m = name;
    ID id;
    int last;

    if (st_lookup(sym_tbl, (st_data_t)name, (st_data_t *)&id))
	return id;

    last = strlen(name)-1;
    id = 0;
    switch (*name) {
      case '$':
	id |= ID_GLOBAL;
	if (is_special_global_name(++m)) goto new_id;
	break;
      case '@':
	if (name[1] == '@') {
	    m++;
	    id |= ID_CLASS;
	}
	else {
	    id |= ID_INSTANCE;
	}
	m++;
	break;
      default:
	if (name[0] != '_' && ISASCII(name[0]) && !ISALNUM(name[0])) {
	    /* operators */
	    int i;

	    for (i=0; op_tbl[i].token; i++) {
		if (*op_tbl[i].name == *name &&
		    strcmp(op_tbl[i].name, name) == 0) {
		    id = op_tbl[i].token;
		    goto id_regist;
		}
	    }
	}

	if (name[last] == '=') {
	    /* attribute assignment */
	    char *buf = ALLOCA_N(char,last+1);

	    strncpy(buf, name, last);
	    buf[last] = '\0';
	    id = rb_intern(buf);
	    if (id > tLAST_TOKEN && !is_attrset_id(id)) {
		id = rb_id_attrset(id);
		goto id_regist;
	    }
	    id = ID_ATTRSET;
	}
	else if (ISUPPER(name[0])) {
	    id = ID_CONST;
        }
	else {
	    id = ID_LOCAL;
	}
	break;
    }
    if (!ISDIGIT(*m)) {
	while (m <= name + last && is_identchar(*m)) {
	    m += mbclen(*m);
	}
    }
    if (*m) id = ID_JUNK;
  new_id:
    id |= ++last_id << ID_SCOPE_SHIFT;
  id_regist:
    name = strdup(name);
    st_add_direct(sym_tbl, (st_data_t)name, id);
    st_add_direct(sym_rev_tbl, id, (st_data_t)name);
    return id;
}

char *
rb_id2name(id)
    ID id;
{
    char *name;

    if (id < tLAST_TOKEN) {
	int i = 0;

	for (i=0; op_tbl[i].token; i++) {
	    if (op_tbl[i].token == id)
		return op_tbl[i].name;
	}
    }

    if (st_lookup(sym_rev_tbl, id, (st_data_t *)&name))
	return name;

    if (is_attrset_id(id)) {
	ID id2 = (id & ~ID_SCOPE_MASK) | ID_LOCAL;

      again:
	name = rb_id2name(id2);
	if (name) {
	    char *buf = ALLOCA_N(char, strlen(name)+2);

	    strcpy(buf, name);
	    strcat(buf, "=");
	    rb_intern(buf);
	    return rb_id2name(id);
	}
	if (is_local_id(id2)) {
	    id2 = (id & ~ID_SCOPE_MASK) | ID_CONST;
	    goto again;
	}
    }
    return 0;
}

static int
symbols_i(key, value, ary)
    char *key;
    ID value;
    VALUE ary;
{
    rb_ary_push(ary, ID2SYM(value));
    return ST_CONTINUE;
}

/*
 *  call-seq:
 *     Symbol.all_symbols    => array
 *  
 *  Returns an array of all the symbols currently in Ruby's symbol
 *  table.
 *     
 *     Symbol.all_symbols.size    #=> 903
 *     Symbol.all_symbols[1,20]   #=> [:floor, :ARGV, :Binding, :symlink,
 *                                     :chown, :EOFError, :$;, :String, 
 *                                     :LOCK_SH, :"setuid?", :$<, 
 *                                     :default_proc, :compact, :extend, 
 *                                     :Tms, :getwd, :$=, :ThreadGroup,
 *                                     :wait2, :$>]
 */

VALUE
rb_sym_all_symbols()
{
    VALUE ary = rb_ary_new2(sym_tbl->num_entries);

    st_foreach(sym_tbl, symbols_i, ary);
    return ary;
}

int
rb_is_const_id(id)
    ID id;
{
    if (is_const_id(id)) return Qtrue;
    return Qfalse;
}

int
rb_is_class_id(id)
    ID id;
{
    if (is_class_id(id)) return Qtrue;
    return Qfalse;
}

int
rb_is_instance_id(id)
    ID id;
{
    if (is_instance_id(id)) return Qtrue;
    return Qfalse;
}

int
rb_is_local_id(id)
    ID id;
{
    if (is_local_id(id)) return Qtrue;
    return Qfalse;
}

int
rb_is_junk_id(id)
    ID id;
{
    if (is_junk_id(id)) return Qtrue;
    return Qfalse;
}

static void
special_local_set(c, val)
    char c;
    VALUE val;
{
    int cnt;

    top_local_init();
    cnt = local_cnt(c);
    top_local_setup();
    ruby_scope->local_vars[cnt] = val;
}

VALUE
rb_backref_get()
{
    VALUE *var = rb_svar(1);
    if (var) {
	return *var;
    }
    return Qnil;
}

void
rb_backref_set(val)
    VALUE val;
{
    VALUE *var = rb_svar(1);
    if (var) {
	*var = val;
    }
    else {
	special_local_set('~', val);
    }
}

VALUE
rb_lastline_get()
{
    VALUE *var = rb_svar(0);
    if (var) {
	return *var;
    }
    return Qnil;
}

void
rb_lastline_set(val)
    VALUE val;
{
    VALUE *var = rb_svar(0);
    if (var) {
	*var = val;
    }
    else {
	special_local_set('_', val);
    }
}

#ifdef YYMALLOC
#define HEAPCNT(n, size) ((n) * (size) / sizeof(YYSTYPE))
#define NEWHEAP() rb_node_newnode(NODE_ALLOCA, 0, (VALUE)parser_heap, 0)
#define ADD2HEAP(n, c, p) ((parser_heap = (n))->u1.node = (p), \
			   (n)->u3.cnt = (c), (p))

static void *
rb_parser_malloc(size)
    size_t size;
{
    size_t cnt = HEAPCNT(1, size);
    NODE *n = NEWHEAP();
    void *ptr = xmalloc(size);

    return ADD2HEAP(n, cnt, ptr);
}

static void *
rb_parser_calloc(nelem, size)
    size_t nelem, size;
{
    size_t cnt = HEAPCNT(nelem, size);
    NODE *n = NEWHEAP();
    void *ptr = xcalloc(nelem, size);

    return ADD2HEAP(n, cnt, ptr);
}

static void *
rb_parser_realloc(ptr, size)
    void *ptr;
    size_t size;
{
    NODE *n;
    size_t cnt = HEAPCNT(1, size);

    if (ptr && (n = parser_heap) != NULL) {
	do {
	    if (n->u1.node == ptr) {
		n->u1.node = ptr = xrealloc(ptr, size);
		if (n->u3.cnt) n->u3.cnt = cnt;
		return ptr;
	    }
	} while ((n = n->u2.node) != NULL);
    }
    n = NEWHEAP();
    ptr = xrealloc(ptr, size);
    return ADD2HEAP(n, cnt, ptr);
}

static void
rb_parser_free(ptr)
    void *ptr;
{
    NODE **prev = &parser_heap, *n;

    while ((n = *prev)) {
	if (n->u1.node == ptr) {
	    *prev = n->u2.node;
	    rb_gc_force_recycle((VALUE)n);
	    break;
	}
	prev = &n->u2.node;
    }
    xfree(ptr);
}
#endif
