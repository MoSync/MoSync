
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

/* Line 1676 of yacc.c  */
#line 132 "rcomp.yacc"

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



/* Line 1676 of yacc.c  */
#line 117 "rcomp.tab.hacc"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif

extern YYSTYPE yylval;


