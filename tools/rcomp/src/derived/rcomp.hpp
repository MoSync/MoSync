/* A Bison parser, made by GNU Bison 2.3.  */

/* Skeleton interface for Bison's Yacc-like parsers in C

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
/* Line 1489 of yacc.c.  */

YYSTYPE;
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif

extern YYSTYPE yylval;

