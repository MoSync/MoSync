/*
Copyright (C) 2011 MoSync AB

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License,
version 2, as published by the Free Software Foundation.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
MA 02110-1301, USA.
*/

// -------------------------------------------------------
// Expression evaluator
//
// Author: Niklas Nummelin
//
// Description: Used to compile and evaluate floating point 
// expressions consisting of arithmetical operations, 
// functions and variables. 
//
// Todo: compile rpn expression into x86 fpu code 
// (should be pretty easy)
// -------------------------------------------------------


#include "eval.h"
#include <stdlib.h>

Function::Function(void *ptr, int num_arguments, const char *name) : ptr(ptr), num_arguments(num_arguments)  
{
	strcpy(this->name, name);
}

Variable::Variable(float value, const char *name) : value(value) 
{
	strcpy(this->name, name);
}


Token::Token(eTokenType type) : type(type) 
{
}

Token::Token(eTokenType type, float num) : type(type), num(num) 
{
}

Token::Token(eTokenType type, Variable *var) : type(type), var(var) 
{
}

Token::Token(eTokenType type, Function *func) : type(type), func(func) 
{
}


void Scope::setVariable(const char *name, float v) 
{
	Map<String, Variable*>::Iterator
		iter = variables.find(String(name));

	if(iter!=variables.end()) 
	{
		(*iter).second->value = v;
	} 
	else 
	{
		variables[name] = new Variable(v, name);
	}
}

Variable* Scope::getVariable(const char *name) 
{
	Map<String, Variable*>::Iterator
		iter = variables.find(String(name));
	if(iter!=variables.end())
		return iter->second;
	else
	{
		Variable *var = new Variable(0.0f, name);
		variables[name] = var;
		return var;
	}
}

void Scope::setFunction(const char *name, void *ptr, int args) 
{
	Map<String, Function*>::Iterator
		iter = functions.find(name);
	if(iter!=functions.end()) 
	{
		(*iter).second->ptr = ptr;
		(*iter).second->num_arguments = args;
	} 
	else 
	{
		functions[name] = new Function(ptr, args, name);	
	}
}

Function* Scope::getFunction(const char *name) 
{
	Map<String, Function*>::Iterator
		iter = functions.find(String(name));
	if(iter!=functions.end())
		return iter->second;
	else
	{
		Function *var = new Function(NULL, 0, name);
		functions[name] = var;
		return var;
	}
}

Expression::Expression(Scope &scope) : scope(scope) 
{
}

// this function takes a string containing an arithmetic 
// expression in infix notation and compiles it into an 
// internal reverse polish notation representation.
// which can then easily be evaluated (and re-evaluated)
// This is done by first tokenizing the string 
// and then parse the tokens, converting 
// the infix notation to rpn.
bool Expression::compile(const char *string) 
{
	const char *src = string;
	Vector<Token> tokens;

	// tokenize it and parse and convert numbers.
	while(*src) 
	{
		if(IS_WHITESPACE(*src)) 
		{
			src++;
			continue;
		}
		switch(*src) 
		{
		case '(': tokens.add(Token(LPAREN)); break;
		case ')': tokens.add(Token(RPAREN)); break;
		case '-': tokens.add(Token(MINUS)); break;
		case '+': tokens.add(Token(PLUS)); break;
		case '*': tokens.add(Token(MUL)); break;
		case '/': tokens.add(Token(DIV)); break;
		default:
			if(IS_NUMBER(*src)) 
				{
					char num[32];
					num[0] = *src;
					src++;
					char *num_dest = num+1;
					int numDots = 0;
					while(IS_NUMBER(*src)||*src=='.')
					{
						if(*src=='.')
						{
							numDots++;
							if(numDots>1)
							{
								// error
								printf("wrong amount of dots in constant number.");
								return false;
							}
						}

						*num_dest = *src;
						num_dest++;
						src++;
					}
					*num_dest = 0;
					float i = atof(num);
					tokens.add(Token(NUMBER, i));
					continue;
				}
			else if(IS_LETTER(*src)) 
			{
				char litteral[255];
				litteral[0] = *src;
				src++;
				char *litteral_dest = litteral+1;
				while(IS_LETTER(*src)||IS_NUMBER(*src)) 
				{
					*litteral_dest = *src;
					litteral_dest++;
					src++;
				}
				*litteral_dest = 0;

				while(IS_WHITESPACE(*src)) src++;

				if(*src=='(') 
				{
					tokens.add(Token(FUNCTION, scope.getFunction(litteral)));
					src++;
				} 
				else 
				{
					tokens.add(Token(VARIABLE, scope.getVariable(litteral)));
				}

				continue;					
			}
			else return false;
		}
		src++;
	}

	// compile! this is done recursivly 
	rpn.clear();
	_compile(0, tokens, rpn);

	rpn.add(Token(END));
	return true;
}

// Used to print the resulting rpn expression.
void Expression::print() 
{
	for(int i = 0; i < rpn.size(); i++) 
	{
		switch(rpn[i].type) 
		{
		case VARIABLE:
			printf("%s ", rpn[i].var->name);
			break;
		case FUNCTION:
			printf("%s ", rpn[i].func->name);
			break;
		case NUMBER:
			printf("%.2f ", rpn[i].num);
			break;
		case DIV:
			printf("/ ");
			break;
		case MUL:
			printf("* ");
			break;
		case MINUS:
			printf("- ");
			break;
		case PLUS:
			printf("+ ");
			break;
		case END:
			break;
		default:
			;// do nothing
		}
	}
	printf("\n");
}

// This function evaluates the rpn expression
// this is done using a stack. Whenever a value
// is found they are put onto the stack.
// The operators pops their arguments from the
// stack and pushes their result onto the stack.
// When the expression has been evaluated only one
// value is remaining on the stack, this is the
// result, which is returned.
float Expression::evaluate() 
{
	static Stack<float> values;
	values.clear();
	int i = -1;
	while(1)
	{
		i++;

		switch(rpn[i].type)
		{
		case NUMBER:
			values.push((float)rpn[i].num);
			break;
		case VARIABLE:
			values.push((float)rpn[i].var->value);
			break;
		case FUNCTION:
			{
				float a = values.peek(); values.pop();
				values.push(((float_func_ptr)rpn[i].func->ptr)(a));
				break;
			}
		case DIV:
			{
				float b = values.peek(); values.pop();
				float a = values.peek(); values.pop();
				if(b==0.0f) values.push(0);
				else values.push(a/b);
			}
			break;
		case MUL:
			{
				float b = values.peek(); values.pop();
				float a = values.peek(); values.pop();
				values.push(a*b);
			}
			break;
		case MINUS:
			{
				float b = values.peek(); values.pop();
				float a = values.peek(); values.pop();
				values.push(a-b);
			}
			break;
		case PLUS:
			{
				float b = values.peek(); values.pop();
				float a = values.peek(); values.pop();
				values.push(a+b);
			}
			break;
		case END:
			return values.peek();
		default:
			;// do nothing
		}
	}
}

// Internal helper function for the compilation
// This is where the actual compilation is done.
// It is done recursivly for each parenthesis.
int Expression::_compile(int start, Vector<Token>& tokens, Vector<Token>& rpn)
{
	int i;
	Stack<eTokenType> opStack;
	eTokenType lastTok = NONE;
	for(i = start; i < tokens.size(); i++) 
	{
		if(tokens[i].type == RPAREN) break;

		switch(tokens[i].type) 
		{
		case PLUS:
		case MINUS:
			// do some simple optimizations of the
			// expression (instead of having unary 
			// operations for these kind of series)
			if(lastTok==MINUS) 
			{
				if(tokens[i].type==MINUS) 
				{
					opStack.pop();
					opStack.push(PLUS);
					break;
				}
			} 
			else if(lastTok==PLUS) 
			{
				if(tokens[i].type==MINUS) 
				{
					opStack.pop();
					opStack.push(MINUS);
					break;
				}						
			} 
			else 
			{
				// if next token is a number and this
				// token is a minus (unary) then just negate the number
				if(	i+1<tokens.size() && 
					IS_EVALUATABLE_LEFT(tokens[i+1].type) &&
					!(IS_EVALUATABLE_RIGHT(lastTok)) && 
					tokens[i].type == MINUS) 
				{
					opStack.push(MINUS);
					rpn.add(Token(NUMBER, 0.0));
					break;
				}
			}
		case DIV:
		case MUL:
			// just push the operator on the operator stack.
			opStack.push(tokens[i].type);
			break;

		case LPAREN:
		case FUNCTION:
		case VARIABLE:
		case NUMBER:
			// if we found a left parenthesis lets recurse into it and increment our
			// index with the amount of tokens that has been parsed through the
			// recursion. Else just add the number to the rpn expression.
			if(tokens[i].type==LPAREN||tokens[i].type==FUNCTION) 
			{
				if(tokens[i].type==FUNCTION) 
				{
					int last_i = i; 
					i+=_compile(i+1, tokens, rpn)+1;
					rpn.add(tokens[last_i]);
				} 
				else 
				{
					i+=_compile(i+1, tokens, rpn)+1;
				}
			} 
			else 
			{
				rpn.add(tokens[i]);
			}

			// if our operator stack isn't empty lets peek the last operator
			// and see if it is time to add it to the rpn (according to predecence)
			if(!opStack.empty()) 
			{
				eTokenType lastOp = opStack.peek();
				eTokenType nextOp = NONE;
				if(i+1<tokens.size())
					nextOp = tokens[i+1].type;
				if((lastOp == PLUS || lastOp == MINUS) &&
					(nextOp == PLUS || nextOp == MINUS || nextOp == RPAREN || nextOp == NONE)) 
				{
					rpn.add(Token(lastOp));
					opStack.pop();
				}
				else if((lastOp == MUL || lastOp == DIV)) 
				{
					rpn.add(Token(lastOp));
					opStack.pop();
					if(nextOp == PLUS || nextOp == MINUS || nextOp == RPAREN || nextOp == NONE) 
					{	
						// nothing has lower predecence than these operators so just empty 
						// the operator stack and add the operators to the rpn expression
						while(opStack.empty()!=true) 
						{
							rpn.add(Token(opStack.peek()));
							opStack.pop();
						}					
					}
				}
			}
			break;
		case RPAREN:
		case END:
		case NONE:
			;	// do nothing
		}

		lastTok = tokens[i].type;
	}

	// add the remaining operators to the rpn expression
	while(opStack.empty()!=true) 
	{
		rpn.add(Token(opStack.peek()));
		opStack.pop();
	}

	return i-start;
}

Expression::~Expression() {
}



// temporary string used to hold the expression
/*
int main() {
	Scope scope;
	scope.setVariable("pi", 3.14159);
	scope.setVariable("x", 1.0);
	scope.setVariable("y", 2.0);
	scope.setVariable("c", 3.0);
	scope.setFunction("sin", sinf, 1);
	scope.setFunction("cos", cosf, 1);
	scope.setFunction("tan", tanf, 1);
	scope.setFunction("abs", fabsf, 1);
	Expression exp(scope);
	//exp.compile("x+sin(y*3.0)+c*abs(-pi)");
	exp.compile("pi*2+x*y");
	exp.print();
	printf("%.5f\n", exp.evaluate());

	return 0;
}
*/
