%{
// RCOMPL.CPP
// Generated from RCOMP.L
//
// Copyright (c) 1997-1999 Symbian Ltd.  All rights reserved.
//

#include <limits.h>
#include "structst.h"
//#include "rcompl.hpp"
#include "rcomp.hpp"
#include "fileline.h"

void yyerror(const char* msg, ...);

#define VALUE_LEN   1024	// must match sizeof(YYSTYPE.Value)
char buf[VALUE_LEN];
char * pCh;
#define CHECK_APPEND(x)	\
    if (pCh-buf==VALUE_LEN-1) { yyerror("string too long - %c ignored", (x)); } else {*pCh++ = (x); }

int isCharLiteral;

//YYSTYPE yylval;

extern String		InputBaseName;
extern FileLineManager* pFileLineHandler;
extern int*		pCurrentLineNumber;
char RealLineNumber[200];

%}
    
Digit [0-9]
Lower [a-z]
Upper [A-Z]
Letter {Lower}|{Upper}
Alphanumeric {Letter}|{Digit}
AlphanumUscore {Alphanumeric}|_
HexChar [A-Fa-f0-9]
ExpChar [eE]
FileNameChar [^"]


%s string_rules	
				
%s file_line_rules

%s cpp_comment	
%s c_comment

%option noyywrap
%option yylineno
%%


<INITIAL>STRUCT				return L_STRUCT;
<INITIAL>RESOURCE				return L_RESOURCE;
<INITIAL>UID2				return L_UID2;
<INITIAL>UID3				return L_UID3;
<INITIAL>NAME					return L_NAME;
<INITIAL>CHARACTER_SET			return L_CHARACTER_SET;
<INITIAL>OFFSET				return L_OFFSET;
<INITIAL>SYSTEM				return L_SYSTEM;
<INITIAL>GLOBAL				return L_GLOBAL;
<INITIAL>LOCAL				return L_LOCAL;
<INITIAL>ENUM					return L_ENUM;
<INITIAL>enum					return L_ENUM;


<INITIAL>BUF					return L_BUF;
<INITIAL>BUF8					return L_BUF8;
<INITIAL>BUF16				return L_BUF16;
<INITIAL>WORD					return L_WORD;
<INITIAL>BYTE					return L_BYTE;
<INITIAL>LONG					return L_LONG;
<INITIAL>DOUBLE				return L_DOUBLE;
<INITIAL>TEXT					return L_TEXT;
<INITIAL>LTEXT				return L_LTEXT;
<INITIAL>TEXT8				return L_TEXT8;
<INITIAL>LTEXT8				return L_LTEXT8;
<INITIAL>TEXT16				return L_TEXT16;
<INITIAL>LTEXT16				return L_LTEXT16;
<INITIAL>LINK					return L_LINK;
<INITIAL>LLINK				return L_LLINK;
<INITIAL>SRLINK				return L_SRLINK;


<INITIAL>LEN					return L_LEN;


<INITIAL>\"					{ BEGIN(string_rules); pCh = buf; isCharLiteral=0; }
<INITIAL>'					{ BEGIN(string_rules); pCh = buf; isCharLiteral=1; }

<string_rules>\\\"		{ CHECK_APPEND('"'); }
<string_rules>\\'		{ CHECK_APPEND('\''); };

<string_rules>\\[rbfntv]	{ CHECK_APPEND( * ( strchr("\rr\bb\ff\nn\tt\vv\aa", yytext[1])-1));}

<string_rules>\\\\			{ CHECK_APPEND('\\'); }

<string_rules>\\\n			/* Escaped newline ignored*/ ;

<string_rules>\n		{ yyerror( isCharLiteral?"Unterminated character literal":"Unterminated string"); BEGIN 0; }

<string_rules>\"	{ 
    if (!isCharLiteral) 
	    {
	    *pCh = '\0'; BEGIN(0); strcpy( yylval.Value, buf); return L_STRING_LITERAL;
	    }
    CHECK_APPEND(*yytext);
    }

<string_rules>'		{ 
    if (isCharLiteral) 
	    {
	    *pCh = '\0'; BEGIN(0); strcpy( yylval.Value, buf); return L_CHAR_LITERAL;
	    }
    CHECK_APPEND(*yytext);
    }

<string_rules>.			{ CHECK_APPEND(*yytext); }


<INITIAL>{Letter}{AlphanumUscore}*    {	
    BEGIN(0);
    strcpy( yylval.Value, yytext);
    return L_LABEL;
    }

<INITIAL>{Digit}+					{	strcpy( yylval.Value, yytext); return L_NUM_NATURAL; }
<INITIAL>0x{HexChar}+					{	strcpy( yylval.Value, yytext); return L_NUM_NATURAL; }
<INITIAL>{Digit}+\.{Digit}+{ExpChar}{Digit}+		{	strcpy( yylval.Value, yytext); return L_NUM_FLOAT;}
<INITIAL>-{Digit}+\.{Digit}+{ExpChar}{Digit}+		{	strcpy( yylval.Value, yytext); return L_NUM_FLOAT;}
<INITIAL>{Digit}+\.{Digit}+				{	strcpy( yylval.Value, yytext); return L_NUM_FLOAT;}
<INITIAL>-{Digit}+\.{Digit}+				{	strcpy( yylval.Value, yytext); return L_NUM_FLOAT;}
<INITIAL>{Digit}+{ExpChar}{Digit}+			{	strcpy( yylval.Value, yytext); return L_NUM_FLOAT;}
<INITIAL>-{Digit}+{ExpChar}{Digit}+			{	strcpy( yylval.Value, yytext); return L_NUM_FLOAT;}

<INITIAL>^[#][ ]{Digit}+[ ]\"					{	BEGIN( file_line_rules); strcpy( RealLineNumber, yytext+2); }

<file_line_rules>\"$					{	BEGIN(0); // # <line> "" means start of base file.
											pFileLineHandler->SetBase( InputBaseName, * pCurrentLineNumber);
										}
										
<file_line_rules>{FileNameChar}*\"$		{	BEGIN(0); // # <line> <filename> means start of named base file.
											pFileLineHandler->SetInclude( yytext, * pCurrentLineNumber);
										}

<file_line_rules>{FileNameChar}*\"[ ]1([ ]3){0,1}$	{
											BEGIN(0); // # <line> <filename> 1 means start of an included file.
											pFileLineHandler->SetInclude( yytext, * pCurrentLineNumber);
										}

<file_line_rules>{FileNameChar}*\"[ ]2([ ]3){0,1}$	{
											BEGIN(0); // # <line> <filename> 2 means end of an included file and now at <line> in <filename>.
											pFileLineHandler->PostInclude( yytext, RealLineNumber, * pCurrentLineNumber);
										}



<INITIAL,file_line_rules>[ \t\n]	    ; // skipped

<INITIAL,file_line_rules>"//"		    { BEGIN(cpp_comment); }
<cpp_comment>\n			    { BEGIN(0); }
<cpp_comment>.			    ; // skipped

<INITIAL,file_line_rules>"/*"		    { BEGIN(c_comment); }
<c_comment>"*/"			    { BEGIN(0); }
<c_comment>\n			    ; // skipped
<c_comment>.			    ; // skipped

<INITIAL>[.{};=\[\],()+\-*/|<>]		return * yytext;


.                       { yyerror("*** Unknown character '%c' (value 0x%x) ", *yytext, *yytext);}
