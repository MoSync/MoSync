%option yylineno
%{
// Copyright (c) 1997-2009 Nokia Corporation and/or its subsidiary(-ies). 
// All rights reserved.
// This component and the accompanying materials are made available
// under the terms of the License "Eclipse Public License v1.0"
// which accompanies this distribution, and is available
// at the URL "http://www.eclipse.org/legal/epl-v10.html".
//
// Initial Contributors:
// Nokia Corporation - initial contribution.
//
// Contributors:
// 
// Description:
// RCOMPL.INL
// Generated from RCOMP.L


#include <limits.h>
#include <string.h>
#include "main.h"
#include "STRUCTST.H"
#include "Parser.h"
#include "localise.h"

#define YY_SKIP_YYWRAP 1
#define YY_NEVER_INTERACTIVE 1
int yywrap();
void yyerror(const char* string, ...);

// Redefine YY_INPUT so we can parse binary data.
#undef YY_INPUT
#define YY_INPUT(buf, result, max_size) (result = new_yy_input(buf, max_size))

int new_yy_input(char *buf, int max_size)
{
  int result;
   if(yyin != NULL) {
		result = fread(buf, 1, max_size, yyin);
	}
   else if((pG->StdInBuffer != NULL) && (pG->StdInBufLength > 0)) {
		int left = pG->StdInBufLength - pG->StdInfBufPos ;
		if(left == 0)
			return YY_NULL ;
		result = (max_size < left) ? max_size : left ; 
		memcpy(buf,&(pG->StdInBuffer[pG->StdInfBufPos]),result); 
		pG->StdInfBufPos += result ;
   }

   if (result == 0)
      return YY_NULL;
   
   const unsigned char BOM[] = {0xef , 0xbb, 0xbf, 0x0 };
   // check for utf8 (BOM) header in buf
   for (int i = 0; i < result-3; i++) {
      if (0 == memcmp(&buf[i],BOM,3)) {
		memset(&buf[i],0x20,3);
      }
   }
   return result;
}

#include "rcomp.hpp"
#include "FILELINE.H"

#define VALUE_LEN   (1024*8)	// must match sizeof(YYSTYPE.Value)
char buf[VALUE_LEN];
char * pCh;
#define CHECK_APPEND(x)	\
    if (pCh-buf==VALUE_LEN-1) { yyerror("string too long - %c ignored", (x)); } else {*pCh++ = (x); }

int isCharLiteral;

extern String		InputBaseName;
extern FileLineManager* pFileLineHandler;
extern int*		pCurrentLineNumber;
char RealLineNumber[200];

#include "ERRORHAN.H"

#define REGISTER_LINE	ErrorHandler::Register(pFileLineHandler->GetCurrentFile(), pFileLineHandler->GetErrorLine(* pCurrentLineNumber))


%}
    
Digit [0-9]
Lower [a-z]
Upper [A-Z]
Letter {Lower}|{Upper}
Alphanumeric {Letter}|{Digit}
LetterOrUnderscore {Letter}|_
AlphanumUscore {Alphanumeric}|_
HexChar [A-Fa-f0-9]
ExpChar [eE]
FileNameChar [^"]

%s string_rules
	/* Rule set for string literals.			        */
	/* n.b. Exclusive rule sets i.e. %x are available in MKS only	*/
	/* so they are not used here; hence all the <INITIAL>'s.        */
				
%s file_line_rules
	/* Rule set for file_line_directive.*/

%s cpp_comment
	/* C++ comment to end of line */

%s c_comment
	/* C comment */

%s comment_tag
	/* Doxygen-style comment tag */

%%


	/* Translations section */
	/* ==================== */

	/*******************************************/
	/* Main keywords                           */
	/*******************************************/
<INITIAL>STRUCT				return L_STRUCT;
<INITIAL>RESOURCE				return L_RESOURCE;
<INITIAL>NAME					return L_NAME;
<INITIAL>CHARACTER_SET			return L_CHARACTER_SET;
<INITIAL>OFFSET				return L_OFFSET;
<INITIAL>SYSTEM				return L_SYSTEM;
<INITIAL>GLOBAL				return L_GLOBAL;
<INITIAL>LOCAL				return L_LOCAL;
<INITIAL>ENUM					return L_ENUM;
<INITIAL>enum					return L_ENUM;
<INITIAL>UID2					return L_UID_TWO;
<INITIAL>UID3					return L_UID_THREE;
<INITIAL>rls_string		return L_RLS_STRING;
<INITIAL>rls_string8		return L_RLS_STRING8;
<INITIAL>rls_double		return L_RLS_DOUBLE;
<INITIAL>rls_byte		return L_RLS_BYTE;
<INITIAL>rls_word		return L_RLS_WORD;
<INITIAL>rls_long		return L_RLS_LONG;
<INITIAL>multi		return L_MULTI;

	/*******************************************/
	/* Types                                   */
	/*******************************************/
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


	/*******************************************/
	/* Others                                  */
	/*******************************************/
<INITIAL>LEN					return L_LEN;


	/*******************************************/
	/* String & character literals             */
	/*******************************************/
<INITIAL>\"					{ BEGIN(string_rules); pCh = buf; isCharLiteral=0; }
<INITIAL>'					{ BEGIN(string_rules); pCh = buf; isCharLiteral=1; }

	/* Escaped single- and double-quotes.*/
<string_rules>\\\"		{ CHECK_APPEND('"'); }
<string_rules>\\'		{ CHECK_APPEND('\''); };

	/* Convert escaped character into corresponding actual character e.g. \t to tab. */
<string_rules>\\[rbfntv]	{ CHECK_APPEND( * ( strchr("\rr\bb\ff\nn\tt\vv\aa", yytext[1])-1));}

	/* Escaped backslash */
<string_rules>\\\\			{ CHECK_APPEND('\\'); }

<string_rules>\\\n				/* Escaped newline ignored*/ ;

	/* End of line before terminating double-quotes.*/
<string_rules>(\r)?\n		{ yyerror( isCharLiteral?"Unterminated character literal":"Unterminated string"); BEGIN 0; }

	/* End of string reached.*/
<string_rules>\"	{ 
    if (!isCharLiteral) 
	    {
	    *pCh = '\0'; BEGIN(0); strcpy( yylval.Value, buf); 
		return L_STRING_LITERAL; 
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

	/* Anything other than \n is stored.*/
<string_rules>.			{ CHECK_APPEND(*yytext); }


	/*******************************************/
	/* Labels                                  */
	/*******************************************/
<INITIAL>{LetterOrUnderscore}{AlphanumUscore}*    {	
    BEGIN(0);
    strcpy( yylval.Value, yytext);
    return L_LABEL;
    }

	/*******************************************/
	/* Numbers                                 */
	/*******************************************/
<INITIAL>{Digit}+					{	strcpy( yylval.Value, yytext); return L_NUM_NATURAL; }
<INITIAL>0x{HexChar}+					{	strcpy( yylval.Value, yytext); return L_NUM_NATURAL; }
<INITIAL>{Digit}+\.{Digit}+{ExpChar}{Digit}+		{	strcpy( yylval.Value, yytext); return L_NUM_FLOAT;}
<INITIAL>-{Digit}+\.{Digit}+{ExpChar}{Digit}+		{	strcpy( yylval.Value, yytext); return L_NUM_FLOAT;}
<INITIAL>{Digit}+\.{Digit}+				{	strcpy( yylval.Value, yytext); return L_NUM_FLOAT;}
<INITIAL>-{Digit}+\.{Digit}+				{	strcpy( yylval.Value, yytext); return L_NUM_FLOAT;}
<INITIAL>{Digit}+{ExpChar}{Digit}+			{	strcpy( yylval.Value, yytext); return L_NUM_FLOAT;}
<INITIAL>-{Digit}+{ExpChar}{Digit}+			{	strcpy( yylval.Value, yytext); return L_NUM_FLOAT;}

	/*******************************************/
	/* file_line_directive                     */
	/*******************************************/
<INITIAL>[#][ ]{Digit}+[ ]\"					{	BEGIN(file_line_rules); strcpy( RealLineNumber, yytext+2); }

<file_line_rules>\"(\r)?$					{	BEGIN(0); // # <line> "" means start of base file.
											pFileLineHandler->SetBase( InputBaseName, * pCurrentLineNumber);
										}
										
<file_line_rules>{FileNameChar}*\"(\r)?$		{	BEGIN(0); // # <line> <filename> means @ line <line> of named base file.
											pFileLineHandler->PostInclude( yytext, RealLineNumber, * pCurrentLineNumber);
										}

<file_line_rules>{FileNameChar}*\"[ ]1([ ]3){0,1}(\r)?$	{
											BEGIN(0); // # <line> <filename> 1 means start of an included file.
											pFileLineHandler->SetInclude( yytext, * pCurrentLineNumber);
										}

<file_line_rules>{FileNameChar}*\"[ ]2([ ]3){0,1}(\r)?$	{
											BEGIN(0); // # <line> <filename> 2 means end of an included file and now at <line> in <filename>.
											pFileLineHandler->PostInclude( yytext, RealLineNumber, * pCurrentLineNumber);
										}



	/*******************************************/
	/* White space                             */
	/*******************************************/
<INITIAL,file_line_rules>[ \t]			; // skipped
<INITIAL>[\n\r]							; // skipped
<file_line_rules>[\n\r]					; // skipped
<INITIAL,file_line_rules>"//"			{ BEGIN(cpp_comment); }
<cpp_comment>[\n\r]						{ BEGIN(0); }
<cpp_comment>.			    ;			// skipped
<INITIAL,file_line_rules>"/*"/[^&\n]	{ BEGIN(c_comment); }
<INITIAL,file_line_rules>"/*"[\n\r]		{ BEGIN(c_comment); }
<c_comment>"*/"							{ BEGIN(0); }
<c_comment>[\n\r]						; // skipped
<c_comment>.							; // skipped

	/*******************************************/
	/* Comment tags                            */
	/*******************************************/

<INITIAL,file_line_rules>[ \t]*"/*&"	{ 
										BEGIN(comment_tag);
										pGL->SetStart(*(pFileLineHandler->GetCurrentFile()), pFileLineHandler->GetErrorLine(* pCurrentLineNumber));
										return L_TAG_START; 
										}	// any comment beginning with a slash followed by a star followed by an ampersand
<comment_tag>"*/"						{ 
										BEGIN(0); 
										return L_TAG_END; 
										}
<comment_tag>"@"[^* \t\r\n]+			{ strcpy( yylval.Value, yytext);	return L_TAG_COMMAND; }
<comment_tag>[^*@ \t\n\r][^ *\t\r\n]*	{ strcpy( yylval.Value, yytext);	return L_TAG_WORD; }
<comment_tag>[\n\r]						{ strcpy( yylval.Value, "\n");		return L_TAG_NEW_LINE; }
<comment_tag>[* \t]						;

	/*******************************************/
	/* Special single characters               */
	/*******************************************/
<INITIAL>[.{};=\[\],()+\-*/|<>]		return * yytext;


	/*******************************************/
	/* Everything else cannot be recognised    */
	/*******************************************/
.                       { yyerror("*** Unknown character '%c' (value 0x%x) ", *yytext, *yytext);}
