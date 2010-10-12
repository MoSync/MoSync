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
// RCOMP.CPP
// Generated from RCOMP.Y

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

//#include "resource.h"
//#include "parser.h"
#include "RESOURCE.H"
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
void SetCharacterLiteral(char* aOut, const String& aQuoted)
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

%}

%union {
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
}

%token <Id> L_STRUCT L_RESOURCE L_NAME L_OFFSET L_SYSTEM L_GLOBAL L_LOCAL L_CHARACTER_SET 
%token <Id> L_BUF L_WORD L_BYTE L_LONG L_DOUBLE L_TEXT L_LTEXT L_LINK L_LLINK L_SRLINK
%token <Id> L_BUF8 L_TEXT8 L_LTEXT8 L_BUF16 L_TEXT16 L_LTEXT16 L_UID_TWO L_UID_THREE
%token <Id> L_RLS_STRING L_RLS_STRING8 L_RLS_DOUBLE
%token <Id> L_RLS_BYTE L_RLS_WORD L_RLS_LONG
%token <Id> L_MULTI
%token <Id> L_TAG_START L_TAG_END 
%token <Value> L_TAG_COMMAND L_TAG_WORD L_TAG_NEW_LINE
%token <Value> L_LABEL L_NUM_NATURAL L_NUM_FLOAT L_NATURAL_EXPR L_ENUM
%token <Value> L_LEN
%token <Value> L_CHAR_LITERAL L_STRING_LITERAL

%type <Id>			data_type len_declaration
%type <pStructItem>		struct_item array_struct_item simple_struct_item
%type <pSimpleStructItem>	simple_struct_item_start
%type <pStructItem>		struct_type_struct_item  struct_array_struct_item
%type <pArrayStructItem>	array_struct_item_start array_struct_item_base
%type <pStructArrayStructItem>	struct_array_struct_item_base
%type <Value>			simple_initialiser natural_expression
%type <Value>			character_code_expression string_expression string_expression_item
%type <pStringArray>		simple_initialiser_list
%type <NumInitialiser>		natural_expression_numeric
%type <RlsQualifiers>		rls_qualifiers rls_cardinality
%type <Value>		rls_label
%type <RlsType>		rls_string_item rls_num_item rls_float_item

%left	'+' '-'
%left	'*' '/'
%left	'|'
%left	UMINUS

%%

/*****************************************************************/
/* TOP-MOST RULE                                                 */
/*****************************************************************/
source :	statement_list				{	if(verbose)	{	MOFF; cout << Divider << "\n" << Divider << endl; MON; }
										}
;
/*****************************************************************/
/* statement-list and statement                                  */
/*****************************************************************/
statement_list:
	statement_list statement
|	statement_list comment_tag
|	/* Nothing */
;

statement:
	struct_statement maybe_semicolon
|	resource_statement maybe_semicolon
|   character_set_statement maybe_semicolon
|	name_statement maybe_semicolon
|	offset_statement maybe_semicolon
|	system_statement maybe_semicolon
|   enum_statement
|	uidX_statement maybe_semicolon
|	rls_item_statement maybe_semicolon
;

maybe_semicolon:
		';'
		{
		// This is my gift to the world: no more "syntax error" for adding
		// an extra semicolon at the end of a struct or resource.
	    REGISTER_LINE;
	    ErrorHandler::OutputErrorLine("Warning: unnecessary semicolon");
		}
|
;

/******************************************************************/
/* STRUCT statement                                               */
/******************************************************************/
struct_statement:
	struct_statement_start struct_item_list '}'
										{	if(verbose) { MOFF; cout << Divider << "\n" << * pSH << Divider << endl; MON;}	}
;
struct_statement_start:
	L_STRUCT L_LABEL maybe_comment_tag '{'
										{	if(verbose) {	MOFF;cout << "struct_statement_start                     " << $2 << endl; MON;}
											pSH = new StructHeader($2);
											REGISTER_LINE;
											pG->SHA.Add(pSH);
										}
|	L_STRUCT L_LABEL len_declaration maybe_comment_tag '{'
										{	if(verbose) {	RCTypeArray Types; MOFF;cout << "struct_statement_start                     " << $2 << " " << Types.GetName($3) << endl; MON;}
											pSH = new StructHeader($2, $3);
											REGISTER_LINE;
											pG->SHA.Add(pSH);
										}
|	L_STRUCT L_LABEL L_LEN maybe_comment_tag '{'
										{	if(verbose) {	MOFF;cout << "struct_statement_start                     " << $2 << " (WORD)" << endl; MON;}
											pSH = new StructHeader($2, L_WORD);
											REGISTER_LINE;
											pG->SHA.Add(pSH);
										}
;
struct_item_list:
	struct_item_list struct_item ';'	{	if(verbose) {	MOFF;cout << "struct_item_list                           Adding struct_item." << endl; MON;}
											REGISTER_LINE;
											pSH->iSIA.Add($2);
										}
|	struct_item_list comment_tag struct_item ';'	{	if(verbose) {	MOFF;cout << "tagged struct_item_list                    Adding struct_item." << endl; MON;}
											REGISTER_LINE;
											pSH->iSIA.Add($3);
										}
|	/* Nothing */
;
struct_item:
	simple_struct_item
|	array_struct_item
|	struct_type_struct_item
|	struct_array_struct_item
;
simple_struct_item:
	simple_struct_item_start			{	$$ = $1;}
|	simple_struct_item_start '(' natural_expression ')'
										{	if(verbose) { MOFF;cout << "    Limit: " << $3 << endl; MON;}
											$1->iLengthLimit = $3;
											$$ = $1;
										}
|	simple_struct_item_start '=' simple_initialiser	/****************************************************************/
										{	if(verbose) { MOFF;cout << "    Default: " << $3 << endl; MON;}
											$1->iDefault = $3;
											$$ = $1;
										}
|	simple_struct_item_start '(' natural_expression ')' '=' string_expression /****************************************************************/
										{	if(verbose) { MOFF;cout << "    Limit: " << $3 << ", Default: " << $6 << endl; MON;}
											NumericValue Limit($3, L_LONG);
											if(String($6).ExportLength(TargetCharacterSet,SourceCharacterSet) > Limit.GetULong() )
											{
												REGISTER_LINE;
												ErrorHandler::OutputErrorLine("Text length exceeds specified limit");
												exit(1);
											}
											$1->iLengthLimit = $3;
											$1->iDefault = $6;
											$$ = $1;
										}

;
simple_struct_item_start:
	data_type L_LABEL					{	if(verbose) 
											{		
												   RCTypeArray Types;
														   MOFF;cout << "simple_struct_item                         " << Types.GetName($1) << " " << $2 << endl; MON;
											}
											$$ = new SimpleStructItem($1,$2); 
											assert($$ != NULL);
										}
|	data_type '<' natural_expression_numeric '>' L_LABEL
										{	if(verbose) 
											{			   RCTypeArray Types;
														   MOFF;cout << "simple_struct_item                         " << Types.GetName($1) << " " << $5 << endl; MON;
											}
											String s(NumericValue::ltoa($3));
											$$ = new SimpleStructItem($1,$5,s);
											assert($$ != NULL);
										}
;

/* Note that generic text identifiers are converted to their explicit
   8 or 16-bit forms at this point, depending on the target character set.
*/
data_type:
	L_BYTE								{	$$ = L_BYTE;}
|	L_WORD								{	$$ = L_WORD;}
|	L_LONG								{	$$ = L_LONG;}
|	L_DOUBLE							{	$$ = L_DOUBLE;}



|	L_TEXT	
	    { 
	    $$ = ( TargetCharacterSet == String::Unicode ) ? L_TEXT16: L_TEXT8;
	    REGISTER_LINE;
	    ErrorHandler::OutputErrorLine("Warning: Deprecated use of zero-terminated TEXT - use LTEXT instead");
	    }
|	L_LTEXT	
	    {
	    $$ = ( TargetCharacterSet == String::Unicode ) ? L_LTEXT16: L_LTEXT8;
	    }
|	L_BUF	
	    { 
	    $$ = ( TargetCharacterSet == String::Unicode ) ? L_BUF16: L_BUF8;
	    }



|	L_TEXT8								{	$$ = L_TEXT8;
											REGISTER_LINE;
											ErrorHandler::OutputErrorLine("Warning: Deprecated use of zero-terminated TEXT8 - use LTEXT8 instead");
										}
|	L_TEXT16							{	$$ = L_TEXT16;
											REGISTER_LINE;
											ErrorHandler::OutputErrorLine("Warning: Deprecated use of zero-terminated TEXT16 - use LTEXT16 instead");
										}
|	L_LTEXT8							{	$$ = L_LTEXT8;}
|	L_LTEXT16							{	$$ = L_LTEXT16;}
|	L_BUF8								{	$$ = L_BUF8;}
|	L_BUF16								{	$$ = L_BUF16;}
|	L_LINK								{	$$ = L_LINK;}
|	L_LLINK								{	$$ = L_LLINK;}
|	L_SRLINK							{	$$ = L_SRLINK;}
;
array_struct_item:
	array_struct_item_base				{	$$ = $1;}
|	array_struct_item_base '=' '{' simple_initialiser_list '}'
										{	if(verbose) {	MOFF;cout << "array_struct_item                          with simple_initialiser_list" << endl;MON;}
											$1->iDefaults = * $4;
											if($1->iSize.Length() > 0)
											{
												NumericValue v($1->iSize, L_LONG);
												REGISTER_LINE;
												if($4->Size()!=long(v.GetULong()))
												{
													ErrorHandler::OutputErrorLine("Size does not match number of initialisers");
													exit(1);
												}
											}
											$$ = $1;
											delete $4;
										}
;
array_struct_item_base:
	array_struct_item_start ']'			{	if(verbose) {	MOFF;cout << "array_struct_item_base                     with no size" << endl;MON;}
											$$ =$1;
										}
|	array_struct_item_start natural_expression ']'
										{	if(verbose) {	MOFF;cout << "array_struct_item_base                     with size " << $2 << endl;MON;}
											$1->iSize = $2;
											$$ = $1;
										}
|	L_LEN len_declaration array_struct_item_start ']'
										{	if(verbose) 
												{		 	RCTypeArray Types;
														 	MOFF;cout << "array_struct_item_base                     with LenType " << Types.GetName($2) << endl;MON;
												}
											$3->iLenType = $2;
											$$ = $3;
										}
|	L_LEN len_declaration array_struct_item_start natural_expression ']'
										{	if(verbose) 
												{		 	RCTypeArray Types;
														 	MOFF;cout << "array_struct_item_base                     with size " << $4 << " and LenType " << Types.GetName($2) << endl;MON;
												}
											$3->iLenType = $2;
											$3->iSize = $4; 
											$$ = $3; 
										}
;
array_struct_item_start:
	data_type L_LABEL '['				{	if(verbose) 
												{		 	RCTypeArray Types; 
														 	MOFF;cout << "array_struct_item_start                    " << Types.GetName($1) << " " << $2 << endl;MON;
												}
											$$ = new ArrayStructItem($1, $2);
										}
;
len_declaration:
	L_BYTE								{	$$ = L_BYTE;}
|	L_WORD								{	$$ = L_WORD;}
;
struct_type_struct_item:
	L_STRUCT L_LABEL					{	if(verbose) {	MOFF;cout << "struct_type_struct_item                    " << $2 << endl;MON;}
											$$ = new StructTypeStructItem($2);
										}
;
struct_array_struct_item:
	struct_array_struct_item_base		{	$$ = $1;}
|	L_LEN len_declaration struct_array_struct_item_base
										{	if(verbose) {	RCTypeArray Types; MOFF;cout << "struct_array_struct_item                   - Setting Size to " << Types.GetName($2) << endl;MON;}
											$3->iLenType = $2; $$ = $3;
										}
;
struct_array_struct_item_base:
	L_STRUCT L_LABEL '[' ']'			{	if(verbose) {	MOFF;cout << "struct_array_struct_item_base              " << $2 << endl;MON;}
											$$ = new StructArrayStructItem($2);
										}
|	L_STRUCT L_LABEL '[' natural_expression ']'
										{	if(verbose) {	MOFF;cout << "struct_array_struct_item_base              " << $2 << " " << $4 << endl;MON;}
											$$ = new StructArrayStructItem($2, $4);
										}
;
/*********************************************************************/
/* RESOURCE statement                                                */
/*********************************************************************/
resource_statement:
	resource_statement_start '{' resource_item_list '}'
	    {	
	    pResourceHeader->AddDefault();
	    CurrentId+=CurrentIdStep;
	    if(verbose) { MOFF;cout << "Resource ID "<< CurrentId << endl << Divider << "\n" << * pResourceHeader << Divider << endl;MON;}
	    pResourceHeader->SetResourceId(*pResourceNameIds,CurrentId,FormatIdAsHex);
	    pG->Index.Add(pResourceHeader);
		
		CheckStructUsage();

		pUsedLabelsArray->Empty();

	    pResourceHeader = NULL;
	    }
;
resource_statement_start:
	L_GLOBAL resource_statement_start_names {}	/* Ignore GLOBAL (obsolete feature).*/
|	L_LOCAL  resource_statement_start_names
	    {	
	    if(verbose) { MOFF;cout << "resource_statement_start                   LOCAL" << endl;MON;}
		    assert(pResourceHeader != NULL);
		    pResourceHeader->iLocal = 1;
	    }
|	resource_statement_start_names {}
;
resource_statement_start_names:
	L_RESOURCE L_LABEL L_LABEL			{	if(verbose) {	MOFF;cout << "resource_statement_start_names             " << $2 << " " << $3 << endl;MON;}
											assert(pResourceHeader == NULL);
											pResourceHeader = new ResourceHeader($3);
											pCurrentRIA = & (pResourceHeader->iRIA);
											REGISTER_LINE;
											if(pResourceNameIds->IsStored($3))
											{
												ErrorHandler::OutputErrorLine("Resource with this name encountered already");
												exit(1);
											}
											pCurrentRIA->FillFromStruct($2);
											pG->AllIdentifiers.Add(new String($3)); // Add label to store
										}
|	L_RESOURCE L_LABEL					{	if(verbose) {	MOFF;cout << "resource_statement_start_names             " << $2 << " <Resource not named>" << endl;MON;}
											assert(pResourceHeader == NULL);
											pResourceHeader = new ResourceHeader;
											pCurrentRIA = & (pResourceHeader->iRIA);
											REGISTER_LINE;
											pCurrentRIA->FillFromStruct($2);
										}
;
resource_item_list:
	resource_item_list resource_item ';'{	if(verbose) {	MOFF;cout << "resource_item_list" << endl;MON;}}
|	resource_item_list comment_tag resource_item ';'{	if(verbose) {	MOFF;cout << "tagged resource_item_list" << endl;MON;}}
|	resource_item_list error ';'		{	yyerrok; yyclearin; }
|	/* Nothing */
;
resource_item:
	L_LABEL '=' simple_initialiser		{	if(verbose) {	MOFF;cout << "resource_item                              " << $1 << " " << $3 << endl;MON;}
											REGISTER_LINE;/****************************************************************/
											pCurrentRIA->Set($1, $3);
										}
|	resource_simple_array_item
|	struct_resource_item
|	struct_array_resource_item
;
resource_simple_array_item:
	L_LABEL '=' '{' '}'                 
	    {	
	    if (verbose) 
		{ MOFF;cout << "resource_simple_array_item                 " << $1 << endl;MON;} 
	    }
|	L_LABEL '=' '{' simple_initialiser_list '}'
	    {	
	    if (verbose) 
		{ MOFF;cout << "resource_simple_array_item                 " << $1 << " with simple_initialiser_list" << endl;MON;}
	    REGISTER_LINE;
	    pCurrentRIA->Set($1, * $4);
	    delete $4;
	    }
;

/*---------------------------------------------------------------------------*/
/* A note about RIAStack, SRIStack and pCurrentRIA.                          */
/*                                                                           */
/*  RIA stands for Resource Item Array.                                      */
/*  SRI stands for Struct Array Resource Item.                               */
/*                                                                           */
/*  A push to RIAStack is made when dropping inside a STRUCT or STRUCT[] in  */
/*  order to set values for the components. When this happens pCurrentRIA is */
/*  set to the RIA for the STRUCT or last item of the STRUCT[].              */
/*                                                                           */
/*  pCurrentRIA is set to the result of popping from RIAStack when a closing */
/*  brace is reached.                                                        */
/*                                                                           */
/*  A push is made to SRIStack when going into an item for a STRUCT[]. On    */
/*  reaching a closing brace the STRUCT[] is popped off the SRIStack. An new */
/*  item may then be added to this array.                                    */
/*---------------------------------------------------------------------------*/
struct_resource_item:
	struct_resource_item_start resource_item_list '}'
										{	if(verbose) {	MOFF;cout << "struct_resource_item" << endl;MON;}
											pCurrentRIA = pG->RIAStack.Pop();
										}
;
struct_resource_item_start:
	L_LABEL '=' L_LABEL '{'	{	if(verbose) {	MOFF;cout << "struct_resource_item_start                 " << $1 << " " << $3 << endl;MON;}
											REGISTER_LINE;
											pCurrentRIA->Set($1, $3);
											String * thisLabel = new String($1);
											pUsedLabelsArray->Add(thisLabel);
											// in here add the label to a temp store
											pG->RIAStack.Push(pCurrentRIA);
											pCurrentRIA = pCurrentRIA->Find($1)->GetRIA();
										}
;
struct_array_resource_item:
	struct_array_resource_item_start struct_array_resource_item_list_top '}'
										{	if(verbose) {	MOFF;cout << "struct_array_resource_item" << endl;MON;}
											pG->SRIStack.Pop();
										}
|	struct_array_resource_item_start struct_array_resource_item_list_top error
										{	pG->SRIStack.Pop();}
;
struct_array_resource_item_start:
	L_LABEL '=' '{'	L_LABEL '{'			{	if(verbose) {	MOFF;cout << "struct_array_resource_item_start           " << $1 << " " << $4 << endl;MON;}
											ResourceItem * p = pCurrentRIA->Find($1);
											pG->SRIStack.Push(p);
											REGISTER_LINE;
											String * thisLabel = new String($1);
											pUsedLabelsArray->Add(thisLabel);
											// in here add the label to a temp store
											p->Set($4);
											pG->RIAStack.Push(pCurrentRIA);
											pCurrentRIA = p->GetRIA();
										}
;
struct_array_resource_item_list_top:
	struct_array_resource_item_list_top_start
|	struct_array_resource_item_list_top_start ',' struct_array_resource_item_list
|	struct_array_resource_item_list_top_start ',' error
;
struct_array_resource_item_list_top_start:
	resource_item_list '}'				{	if(verbose) {	MOFF;cout << "struct_array_resource_item_list_top        " << endl;MON;}
											pCurrentRIA = pG->RIAStack.Pop();
										}
;
struct_array_resource_item_list:
	struct_array_resource_item_list_item
|	struct_array_resource_item_list ',' struct_array_resource_item_list_item
;
struct_array_resource_item_list_item:
	struct_array_resource_item_list_item_start resource_item_list '}'
										{	if(verbose) {	MOFF;cout << "struct_array_resource_item_list_item       " << endl;MON;}
											pCurrentRIA = pG->RIAStack.Pop();
										}
;
struct_array_resource_item_list_item_start:
	L_LABEL '{'							{	if(verbose) {	MOFF;cout << "struct_array_resource_item_list_item_start " << $1 << endl;MON;}
											ResourceItem * p = pG->SRIStack.Peek();
											REGISTER_LINE;
											p->Set($1);
											pG->RIAStack.Push(pCurrentRIA);
											pCurrentRIA = p->GetRIA();
										}
;


/*****************************************************************/
/* simple_initialiser and simple_initialiser_list                */
/*****************************************************************/
simple_initialiser:
	L_NUM_FLOAT
|	L_CHAR_LITERAL  
	    { 
	    // convert literal to unsigned long value of 1st character
		SetCharacterLiteral($$, $1);
	    }
|	string_expression
|	natural_expression
; 
simple_initialiser_list:
	simple_initialiser  
	    {
	    if(verbose) 
		{	
		MOFF;cout << "simple_initialiser_list                    - single string " << $1 << endl;MON;
		}
		
	    $$ = new StringArray;
	    $$->Add(new String($1) );
	    }
|	simple_initialiser_list ',' simple_initialiser	
										{	if(verbose) {	MOFF;cout << "simple_initialiser_list                    - part of list " << $3 << endl;MON;}
											assert($1 != NULL);
											$1->Add(new String($3 ) );
											$$ = $1;
										}
;

natural_expression:
	natural_expression_numeric			{	String s(NumericValue::ltoa($1) ); strcpy($$, s.GetAssertedNonEmptyBuffer() ); }
;	
natural_expression_numeric:
	L_NUM_NATURAL						{	if(verbose) {	MOFF;cout << "Converting number " << $1 << endl;MON;}
											REGISTER_LINE;
											NumericValue v($1, L_LONG); $$ = (long)v.GetULong();
										}
|	natural_expression_numeric '+' natural_expression_numeric	{	$$ = $1 + $3;	}
|	natural_expression_numeric '-' natural_expression_numeric	{	$$ = $1 - $3;	}
|	natural_expression_numeric '*' natural_expression_numeric	{	$$ = $1 * $3;	}
|	natural_expression_numeric '/' natural_expression_numeric	{	$$ = $1 / $3;	}
|	natural_expression_numeric '|' natural_expression_numeric	{	$$ = $1 | $3;	}
|	'-' natural_expression_numeric %prec UMINUS					{	if (!NumericValue::CheckSigned($2,L_LONG))
																	{
																	REGISTER_LINE;
																	ErrorHandler::OutputErrorLine("Signed value too low");
																	exit(1);
																	}
																	$$ = - $2;		
																}
|	'(' natural_expression_numeric ')'							{	$$ = $2;		}
;
string_expression:
	string_expression_item  
|	string_expression_item string_expression {
	    if (strlen($$)+strlen($2) > sizeof($$)-1)
		{
		REGISTER_LINE;
		ErrorHandler::OutputErrorLine("String expression is too long");
		exit(1);
		}
	    strcat($$, $2);
	    }
;
string_expression_item:
	L_STRING_LITERAL
|	character_code_expression
|	L_LABEL 
	    {
		const char * fileName = (*ErrorHandler::GetFileName()).GetBuffer();
		int lineNumber = ErrorHandler::GetLineNumber();
		QualifiedString * thisLabel = new QualifiedString($1, new String(fileName), lineNumber);
		// store the label in the UsedIdentifiers array for checking
		// whether label was declared
		pG->UsedIdentifiers.Add(thisLabel);

	    if (pG->EnumValues.IsStored($1))
			{
			sprintf($$, "%d", (int)(pG->EnumValues.FindId($1)));
			}
		else if (pG->RlsNameIndex.count($1)) // if rls item has already been defined
			{
			// Found a reference to an rls_string.
			RlsValue &rv = pG->RlsValues[pG->RlsNameIndex[$1]];
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
				strcpy($$, rv.iValue.GetBuffer());
				break;
				// Anything else is a character: this is converted to a number.
			case ERlsStringChar:
			case ERlsByteChar:
			case ERlsWordChar:
			case ERlsLongChar:
				SetCharacterLiteral($$, rv.iValue);
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
	    }
;
character_code_expression:
	'<' natural_expression_numeric '>'  
	    {	
	    REGISTER_LINE;
	    if($2 < 0 || ($2 > 255 && TargetCharacterSet != String::Unicode))
		{
		    ErrorHandler::OutputErrorLine("Character code must be a number in the range 0 to 255.");
		    exit(1);
		}
	    if (TargetCharacterSet != String::Unicode)
		{
		* $$ = char($2); * ($$ + 1) = '\0'; 
		} 
	    else
		{
		if (SourceCharacterSet == String::CP1252)
		    {
		    if ( ($2 >= 0x80) && ($2 <= 0x9F ) ) // 80-9F are illegal Unicode values.
			{
			ErrorHandler::OutputErrorLine("Warning: Deprecated non-unicode value in source stream");
			}
		    * $$ = char(UnicodeEscape);
		    asUTF8($$ + 1, $2);
		    }
		else
		if (SourceCharacterSet == String::UTF8)
		    {
		    asUTF8($$, $2);
		    }
		else
		    {
		    // Unsatisfactory, but do people use other character sets?
		    if ($2 > 255)
			{
			ErrorHandler::OutputErrorLine("Don't know how to handle character > 255");
			}
		    * $$ = char($2); * ($$ + 1) = '\0'; 
		    }
		}
	    }
;


/*****************************************************************/
/* name_statement                                                */
/*****************************************************************/
name_statement:
	L_NAME L_LABEL
	    {
	    REGISTER_LINE;
	    SetIdFromName($2);
	    }
|	L_NAME L_STRING_LITERAL
	    {
	    REGISTER_LINE;
	    SetIdFromName($2);
	    }
;


/*****************************************************************/
/* uidX_statement                                                */
/*****************************************************************/
uidX_statement:
	L_UID_TWO natural_expression_numeric
		{
		REGISTER_LINE;
		if ($2 == 0)
			{ ErrorHandler::OutputErrorLine("UID2 must be non-zero"); exit(1); }
		if (Uid2 != 0)
			{ ErrorHandler::OutputErrorLine("Warning: overwriting previous UID2 value"); }
		Uid2=$2;
		if(verbose) 
			{ MOFF;cout << "uidX_statement  UID2                       " << Uid2 << endl;MON;}
		}
|	L_UID_THREE natural_expression_numeric
		{
		REGISTER_LINE;
		if ($2 == 0)
			{ ErrorHandler::OutputErrorLine("UID3 must be non-zero"); exit(1); }
		if (Uid3 != 0)
			{ ErrorHandler::OutputErrorLine("Warning: overwriting previous UID3 value"); }
		Uid3=$2;
		if(verbose) 
			{ MOFF;cout << "uidX_statement  UID3                       " << Uid3 << endl;MON;}
		}
;
 
 
/*****************************************************************/
/* character_set_statement                                       */
/* Defines the SOURCE character set. Note that Unicode is a      */
/* character set id, but we can't read Unicode source            */
/* (because LEX and YACC can't handle it)                        */
/*****************************************************************/

character_set_statement:
	L_CHARACTER_SET L_LABEL		{	if(verbose) {	MOFF;cout << "character_set_statement                    " << $2 << endl;MON;}
											REGISTER_LINE;
											SourceCharacterSet = CharacterSetID($2);
											if ( SourceCharacterSet == String::UNKNOWN )
											{
												String err = "Warning: Unrecognised character set name '";
												err += $2;
												err += "'";
												ErrorHandler::OutputErrorLine(err);
											}
											if ( SourceCharacterSet == String::Unicode )
											{
											    SourceCharacterSet = String::UNKNOWN;
												ErrorHandler::OutputErrorLine("Unicode source is unsupported");
											}
										}
;



/*****************************************************************/
/* offset_statement                                              */
/*****************************************************************/
offset_statement:
	L_OFFSET natural_expression			{	if(verbose) {	RCTypeArray Types;
															MOFF;cout << "offset_statement                           " << $2 << endl;MON; }
											REGISTER_LINE;
										 	CurrentId=((long) NumericValue($2, L_LONG).GetULong() );
										}
;

/*****************************************************************/
/* system_statement                                              */
/*****************************************************************/
system_statement:
	L_SYSTEM							{	if(verbose) {	MOFF;cout << "system_statement" << endl;MON;}
											CurrentIdStep=-1;
										}
;

/*****************************************************************/
/* enum_statement                                                */
/*****************************************************************/
enum_statement:
    enum_statement_start enum_list '}'
|	enum_statement_start enum_list '}' ';'
;
enum_statement_start:
	L_ENUM L_LABEL '{'					 
	    {	
	    if(verbose) 
		{ MOFF;cout << "enum_statement" << endl;MON;} 
	    CurrentEnumName = $2;
	    CurrentEnumValue=0;
	    }
|	L_ENUM '{'						
	    {	
	    if(verbose) 
		{ MOFF;cout << "enum_statement" << endl;MON;} 
	    CurrentEnumName = "";
	    CurrentEnumValue=0;
	    }
;

enum_list_entry:
	L_LABEL                             
		{	
		pG->EnumValues.Add($1, CurrentEnumValue++);
		pG->AllIdentifiers.Add(new String($1)); // Add label to store
		}
|	L_LABEL '=' simple_initialiser      
	    {	
	    CurrentEnumValue = atol($3);
	    pG->EnumValues.Add($1, CurrentEnumValue);
	    CurrentEnumValue++;			// Increment so that next field has value ($3+1)
		pG->AllIdentifiers.Add(new String($1)); // Add label to store
	    }
;


enum_list:
		maybe_comment_tag enum_list_entry
|		enum_list ',' maybe_comment_tag enum_list_entry
;

/************************/
/* rls_xxxx statement */
/************************/
rls_item_statement:
		rls_string_item rls_qualifiers rls_label string_expression
		{
		pG->RlsNameIndex[$3] = pG->RlsValues.size();
		pG->RlsValues.push_back(RlsValue(ErrorHandler::GetFileName(),
			ErrorHandler::GetLineNumber(), $4, $1,
			$2.iCardinality, $2.iMaxLength));
		if($2.iMaxLength
			< String($4).ExportLength(TargetCharacterSet,SourceCharacterSet))
			{
			Message * message = pG->Messages.GetEntry(LT_032);
			if(message->GetActivated())
				{
				ErrorHandler::OutputErrorLine(message->GetMessageOutput());
				exit(1);
				}
			}
		}
|		rls_string_item rls_qualifiers rls_label L_CHAR_LITERAL	/* This section is only for compatibility */
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
		pG->RlsNameIndex[$3] = pG->RlsValues.size();
		pG->RlsValues.push_back(RlsValue(ErrorHandler::GetFileName(),
			ErrorHandler::GetLineNumber(), $4, ERlsStringChar,
			$2.iCardinality));
		}
|		rls_float_item rls_cardinality rls_label L_NUM_FLOAT
		{
		pG->RlsNameIndex[$3] = pG->RlsValues.size();
		pG->RlsValues.push_back(RlsValue(ErrorHandler::GetFileName(),
			ErrorHandler::GetLineNumber(), $4, $1,
			$2.iCardinality));
		}
|		rls_num_item rls_cardinality rls_label L_NUM_NATURAL
		{
		pG->RlsNameIndex[$3] = pG->RlsValues.size();
		pG->RlsValues.push_back(RlsValue(ErrorHandler::GetFileName(),
			ErrorHandler::GetLineNumber(), $4, $1,
			$2.iCardinality));
		}
|		rls_num_item rls_cardinality rls_label L_CHAR_LITERAL
		{
		TRlsType rlsCharType = $1 == ERlsByte? ERlsByteChar
			: ( $1 ==  ERlsWord? ERlsWordChar : ERlsLongChar );
		pG->RlsNameIndex[$3] = pG->RlsValues.size();
		pG->RlsValues.push_back(RlsValue(ErrorHandler::GetFileName(),
			ErrorHandler::GetLineNumber(), $4, rlsCharType,
			$2.iCardinality));
		}
;

rls_label: L_LABEL
		{
		// Register line even if no warning here so that
		// the rls_ item knows which line the label was on.
		// Without this, the line registered would be the
		// line following the declaration.
		REGISTER_LINE;
		strcpy($$, $1);

		if (pG->RlsNameIndex.count($1) != 0)
			{
			Message * message = pG->Messages.GetEntry(LT_003);
			if(message->GetActivated())
				{
				ErrorHandler::OutputErrorLine(message->GetMessageOutput());
				}
			}
		pG->AllIdentifiers.Add(new String($1)); // Add label to store
		}

rls_qualifiers:
		'<' L_NUM_NATURAL '>' rls_cardinality
		{
		NumericValue v($2, L_LONG);
		$$.iMaxLength = v.GetULong();
		$$.iCardinality = $4.iCardinality;
		}
|		rls_cardinality
		{ $$ = $1; }
;

rls_cardinality:
		L_MULTI
		{
		$$.iMaxLength = 0xFFFFFFF;
		$$.iCardinality = ERlsCardinalityMultiple;
		}
|
		{
		$$.iMaxLength = 0xFFFFFFF;
		$$.iCardinality = ERlsCardinalitySingle;
		}
;

rls_string_item:
		L_RLS_STRING
		{ $$ = ERlsString; }
|		L_RLS_STRING8
		{ $$ = ERlsString8; }
;

rls_num_item:
		L_RLS_BYTE
		{ $$ = ERlsByte; }
|		L_RLS_WORD
		{ $$ = ERlsWord; }
|		L_RLS_LONG
		{ $$ = ERlsLong; }
;

rls_float_item:
		L_RLS_DOUBLE
		{ $$ = ERlsDouble; }
;

/************************/
/* comment tags         */
/************************/
maybe_comment_tag:
	comment_tag
|
;

comment_tag:
	L_TAG_START tag_line L_TAG_END {ErrorHandler::Register(pFileLineHandler->GetCurrentFile(), pFileLineHandler->GetErrorLine(*pCurrentLineNumber)); }
   ;

tag_line:
	tag_line tag_word

|
;

tag_word:
 	L_TAG_NEW_LINE	{ pGL->StoreComment($1);	}
|	L_TAG_COMMAND	{ pGL->StoreComment($1);	}
|	L_TAG_WORD		{ pGL->StoreComment($1);	}
;

%%

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

void RlsUnusedWarnings()
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


