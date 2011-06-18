#ifndef _EVAL_H_
#define _EVAL_H_

// old expression evaluator
// author: Niklas Nummelin


#include <stdio.h>
#include <MAUtil/Vector.h>
#include <MAUtil/Stack.h>
#include <MAUtil/String.h>
#include <MAUtil/Map.h>
#include <math.h>

using namespace MAUtil;
//using namespace stdext;

typedef unsigned char byte;
typedef unsigned short word;
typedef unsigned int dword;

// enumerator for the different token types
enum eTokenType 
{
	MINUS,
	PLUS,
	DIV,
	MUL,
	LPAREN,
	RPAREN,
	NUMBER,
	FUNCTION,
	VARIABLE,
	END,
	NONE
};

// macro to check if a character is a number
#define IS_NUMBER(x) (((x)>='0')&&((x)<='9'))
#define IS_LETTER(x) ((((x)>='a')&&((x)<='z'))||(((x)>='A')&&((x)<='Z')))
#define IS_WHITESPACE(x) ((x)==' '||(x)=='\t'||(x)=='\n')
#define IS_EVALUATABLE_LEFT(x) ((x) == NUMBER || (x) == VARIABLE || (x) == FUNCTION || (x)==LPAREN)
#define IS_EVALUATABLE_RIGHT(x) ((x) == NUMBER || (x) == VARIABLE || (x)==RPAREN)

struct Function 
{
	Function(void *ptr, int num_arguments, const char *name);
	void *ptr;
	int num_arguments;
	char name[256];
};

struct Variable 
{
	Variable(float value, const char *name);
	float value;
	char name[256];
};

// struct used to hold the different tokens of the parsed infix string
// as numbers is found they are stored in the num field (unused for other 
// tokens)
struct Token 
{
	Token(eTokenType type=NONE);
	Token(eTokenType type, float num);
	Token(eTokenType type, Variable *var);
	Token(eTokenType type, Function *func);
	eTokenType type;

	union 
	{
		float num; // used if token is a number.
		Variable *var; // used if token is a variable.
		Function *func; // used if token is a function
	};
};

// temp, should have dynamical amount of arguments 
typedef float (*float_func_ptr) (float a);

class Scope 
{
public:
	void setVariable(const char *name, float v);
	Variable* getVariable(const char *name);
	void setFunction(const char *name, void *ptr, int args);
	Function* getFunction(const char *name);

private:
	Map<String, Variable*> variables;
	Map<String, Function*> functions;
};

// class used to parse, compile and evaluate an expression
class Expression {
public:
	Expression(Scope &scope);
	~Expression();

	// this function takes a string containing an arithmetic 
	// expression in infix notation and compiles it into an 
	// internal reverse polish notation representation.
	// which can then easily be evaluated (and re-evaluated)
	// This is done by first tokenizing the string 
	// and then parse the tokens, converting 
	// the infix notation to rpn.
	bool compile(const char *string);

	// Used to print the resulting rpn expression.
	void print();

	// This function evaluates the rpn expression
	// this is done using a stack. Whenever a value
	// is found they are put onto the stack.
	// The operators pops their arguments from the
	// stack and pushes their result onto the stack.
	// When the expression has been evaluated only one
	// value is remaining on the stack, this is the
	// result, which is returned.
	float evaluate();

private:

	// Internal helper function for the compilation
	// This is where the actual compilation is done.
	// It is done recursivly for each parenthesis.
	int _compile(int start, Vector<Token>& tokens, Vector<Token>& rpn);
	Vector<Token> rpn;
	
	Scope &scope;
};

#endif /* _EVAL_H_ */
