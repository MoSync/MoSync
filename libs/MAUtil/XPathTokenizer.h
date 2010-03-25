/* Copyright (C) 2009 Mobile Sorcery AB

This program is free software; you can redistribute it and/or modify it under
the terms of the GNU General Public License, version 2, as published by
the Free Software Foundation.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
for more details.

You should have received a copy of the GNU General Public License
along with this program; see the file COPYING.  If not, write to the Free
Software Foundation, 59 Temple Place - Suite 330, Boston, MA
02111-1307, USA.
*/

/** \file XPathTokenizer.h
*
* XPath expression tokenizer
*
* \author Patrick Broman and Niklas Nummelin
*
*/

#ifndef _SE_MSAB_MAUTIL_XPATH_TOKENIZER_H_
#define _SE_MSAB_MAUTIL_XPATH_TOKENIZER_H_

/*#ifdef MOBILEAUTHOR
#include <stdio.h>
#endif
*/

#include "Tokenizer.h"
#include "DomParser.h"

using namespace MAUtil::Dom;

namespace MAUtil {
	namespace XPath {

		enum eTokenType {
			TOKEN_NUMBER,
			TOKEN_LITERAL,
			TOKEN_SLASH,
			TOKEN_DSLASH,
			TOKEN_OPERATOR,
			TOKEN_LBRACE,
			TOKEN_RBRACE,
			TOKEN_ELEM_IDENT,
			TOKEN_ATTR_IDENT,
			TOKEN_WILD_ELEM_IDENT,
			TOKEN_WILD_ATTR_IDENT,
			TOKEN_FUNCTION,
			TOKEN_PARENT,
			TOKEN_CURRENT,
			TOKEN_LAST
		};

		class TokenOperator : public TokenFixed {
		public:
			TokenOperator() : TokenFixed(TOKEN_OPERATOR) {
				patterns.add("and");
				patterns.add("or");
				patterns.add("mod");
				patterns.add("div");
				patterns.add("<=");
				patterns.add(">=");
				patterns.add("!=");
				patterns.add("=");
				patterns.add("+");
				patterns.add("-");
				patterns.add("*");
			}
		};

		class TokenLiteral : public TokenMatcher {
		public:
			TokenLiteral() : TokenMatcher(TOKEN_LITERAL) {
			}

			unsigned int match(const char* str) {
				const char* initialStr = str;
				char firstType; 
				if((*str)!='\"' && (*str)!='\'') return 0;
				firstType = *str;
				str++;
				while(*str != firstType) {
					str++;
				}
				str++;
				return (unsigned int)(str-initialStr);
			}
		};

		class TokenElemIdent : public TokenMatcher {
		private:
			inline bool isValidChar(char c) {
				if((c>='a' && c<='z') || (c>='A' && c<='Z') || (c>='0' && c<='9') ||
					(c=='.') || (c=='-') || (c=='_') || (c==':')) return true;
				else return false;
			}

			inline bool isValidFirstChar(char c) {
				if((c>='a' && c<='z') || (c>='A' && c<='Z') || (c=='_') || (c==':')) return true;
				else return false;
			}

		public:
			TokenElemIdent(eTokenType type=TOKEN_ELEM_IDENT) : TokenMatcher(type) {
			}

			unsigned int match(const char* str) {
				const char* initialStr = str;
				if(!isValidFirstChar(*str))	return 0;
				str++;
				while(isValidChar(*str)) {
					str++;
				}
				return (unsigned int)(str-initialStr);
			}
		};

		class TokenAttrIdent : public TokenElemIdent {
		public:
			TokenAttrIdent() : TokenElemIdent(TOKEN_ATTR_IDENT){
			}
			unsigned int match(const char* str) {
				if(*str == '@') return TokenElemIdent::match(++str)+1;
				return 0;
			}
		};

		class TokenFunction : public TokenElemIdent {
		public:
			TokenFunction() : TokenElemIdent(TOKEN_FUNCTION){
			}
			unsigned int match(const char* str) {
				unsigned int matchLen;
				if((matchLen = TokenElemIdent::match(str)) == 0) return 0;			
				const char* initialStr = str;
				str+=matchLen;
				while(*str==' ' && *str!='(') str++;
				while(*str==' ' && *str!=')') str++;
				return (unsigned int)(str-initialStr);
			}
		};

		class TokenNumber : public TokenMatcher {
		private:
			inline bool isNum(char c) {
				if(c>='0' && c<='9') return true;
				else return false;
			}

		public:
			TokenNumber() : TokenMatcher(TOKEN_NUMBER) {
			}

			unsigned int match(const char* str) {
				const char* initialStr = str;

				if((*str)=='.') {
					str++;
					if(!isNum(*str)) {
						return 0;
					}
					str++;
					while(*str && isNum(*str)) {
						str++;
					}
					return (unsigned int)(str-initialStr);
				} else {
					if(!isNum(*str)) {
						return 0;
					}
					str++;
					while(*str && isNum(*str)) {
						str++;
					}
					if((*str)=='.') {
						str++;
						while(*str && isNum(*str)) {
							str++;
						}
					} 
					return (unsigned int)(str-initialStr);
				}
			}
		};

		class XPathTokenizer : public Tokenizer{
		public:

			XPathTokenizer() {
				tokenMatchers.reserve(TOKEN_LAST);
				tokenMatchers.add(new TokenNumber());
				tokenMatchers.add(new TokenLiteral());
				tokenMatchers.add(new TokenOperator());
				tokenMatchers.add(new TokenFixed(TOKEN_LBRACE, "["));
				tokenMatchers.add(new TokenFixed(TOKEN_RBRACE, "]"));
				tokenMatchers.add(new TokenFixed(TOKEN_SLASH, "/"));
				tokenMatchers.add(new TokenFixed(TOKEN_DSLASH, "//"));
				tokenMatchers.add(new TokenElemIdent());
				tokenMatchers.add(new TokenAttrIdent());
				tokenMatchers.add(new TokenFixed(TOKEN_WILD_ELEM_IDENT, "*"));
				tokenMatchers.add(new TokenFixed(TOKEN_WILD_ATTR_IDENT, "@*"));
				tokenMatchers.add(new TokenFunction());
				tokenMatchers.add(new TokenFixed(TOKEN_WILD_ELEM_IDENT, ".."));
				tokenMatchers.add(new TokenFixed(TOKEN_WILD_ATTR_IDENT, "."));
			}

		};

		class XPathStep {
		public:
			virtual bool selectNodes(Vector<Node*>& input, Vector<Node*>& output) = 0;
		};

		class XPathStepFindRoot : public XPathStep {
		public:
			bool selectNodes(Vector<Node*>& input, Vector<Node*>& output) {
				Vector_each(Node*, node, input) {
					Element *parent = (Element*)*node;
					if(parent == NULL) return false;
					Element *n = (Element*)parent;
					while(n) {
						parent = n;		
						n = n->getParent();
					}
					output.add(parent);
				}
				return true;
			}
		};

		class XPathStepFindCurrent: public XPathStep {
		public:
			bool selectNodes(Vector<Node*>& input, Vector<Node*>& output) {
				Vector_each(Node*, node, input) {
					output.add(*node);
				}
				return true;
			}
		};

		class XPathStepFindParent: public XPathStep {
		public:
			bool selectNodes(Vector<Node*>& input, Vector<Node*>& output) {
				Vector_each(Node*, node, input) {
					Element *parent = ((Element*)*node)->getParent();
					if(parent != NULL) {
						output.add(parent);
					} else {
						return false;;
					}
				}
				return true;
			}
		};


		class XPathStepDescendantsElem : public XPathStep {
		public:
			XPathStepDescendantsElem(const String& name)
				: name(name)
			{
			}

			bool selectNodes(Vector<Node*>& input, Vector<Node*>& output) {
				Vector_each(Node*, node, input) {
					if((*node)->getType() == Node::ELEMENT) {
						recursiveSelectNodes((Element*)*node, output);
					}
				}
				return true;
			}
		private:
			void recursiveSelectNodes(Element *node, Vector<Node*>& output) {	
				Vector<Node*> nodes = node->getChildren();
				node->getElementsWithName(name, output);

				for(int i = 0; i < nodes.size(); i++) {
					if(nodes[i]->getType() == Node::ELEMENT)
						recursiveSelectNodes((Element*)nodes[i], output);
				}
			}

			String name;
		};

		class XPathStepChildElem : public XPathStep {
		public:
			XPathStepChildElem(const String& name)
				: name(name)
			{
			}

			bool selectNodes(Vector<Node*>& input, Vector<Node*>& output) {
				Vector_each(Node*, node, input) {
					if((*node)->getType() == Node::ELEMENT) {
						((Element*)*node)->getElementsWithName(name, output);
					}
				}
				return true;
			}
		private:
			String name;
		};

		class XPathStepDescendantsAttr : public XPathStep {
		public:
			XPathStepDescendantsAttr(const String& name)
				: name(name)
			{
			}

			bool selectNodes(Vector<Node*>& input, Vector<Node*>& output) {
				Vector_each(Node*, node, input) {
					if((*node)->getType() == Node::ELEMENT) {
						recursiveSelectNodes((Element*)*node, output);
					}
				}
				return true;
			}
		private:

			void recursiveSelectNodes(Element *node, Vector<Node*>& output) {	
				Vector<Node*> nodes = node->getChildren();
				node->getAttributesWithName(name, output);

				for(int i = 0; i < nodes.size(); i++) {
					if(nodes[i]->getType() == Node::ELEMENT)
						recursiveSelectNodes((Element*)nodes[i], output);
				}
			}
			String name;
		};

		class XPathStepChildAttr : public XPathStep {
		public:
			XPathStepChildAttr(const String& name)
				: name(name)
			{
			}

			bool selectNodes(Vector<Node*>& input, Vector<Node*>& output) {
				Vector_each(Node*, node, input) {
					if((*node)->getType() == Node::ELEMENT) {
						((Element*)*node)->getAttributesWithName(name, output);
					}
				}
				return true;
			}
		private:
			String name;
		};

		class XPathExpression {
		public:
			XPathExpression() {
			}

			XPathExpression(const String &exp) {
				compile(exp);
			}

			enum eXPathState {
				STATE_DESC,
				STATE_CHILD,
			};

			bool compile(const String &exp) {

				Vector<Token*> tokens;
				steps.clear();

				if(!xpathTokenizer.tokenize(exp, tokens)) {
					return false;
				}

				char tempBuffer[1024];

				eXPathState state;

				for(int token = 0; token < tokens.size(); token++) {
					const char *buf = tokens[token]->getStart();
					int len = tokens[token]->getLength();
					strncpy(tempBuffer, buf, len);
					tempBuffer[len] = 0;
					switch(tokens[token]->getTokenType()) {
						case TOKEN_NUMBER:
						case TOKEN_LITERAL:
						case TOKEN_OPERATOR:
						case TOKEN_LBRACE:
							{
								const char *startOfExp = tokens[token+1]->getStart();
								while(tokens[token]->getTokenType() != TOKEN_RBRACE) {
									token++;
									if(token>=tokens.size()) 
									{
										return false;
									}
								}
								const char *endOfExp = tokens[token-1]->getStart() + tokens[token-1]->getLength();
								int len = endOfExp-startOfExp;
								strncpy(tempBuffer, startOfExp, len);
								tempBuffer[len] = 0;						
							}
							break;
						case TOKEN_SLASH:
							state = STATE_CHILD;
							break;
						case TOKEN_DSLASH:
							state = STATE_DESC;
							if(token == 0) {
								// go to document root
								steps.add(new XPathStepFindRoot());
							}
							break;
						case TOKEN_ELEM_IDENT:
							switch(state) {
						case STATE_CHILD: steps.add(new XPathStepChildElem(tempBuffer)); break;
						case STATE_DESC: steps.add(new XPathStepDescendantsElem(tempBuffer)); break;
							}
							break;
						case TOKEN_ATTR_IDENT:
							switch(state) {
						case STATE_CHILD: steps.add(new XPathStepChildAttr(&tempBuffer[1])); break;
						case STATE_DESC: steps.add(new XPathStepDescendantsAttr(&tempBuffer[1])); break;
							}
							break;
						case TOKEN_WILD_ELEM_IDENT:
							switch(state) {
						case STATE_CHILD: steps.add(new XPathStepChildElem("*")); break;
						case STATE_DESC: steps.add(new XPathStepDescendantsElem("*")); break;
							}
							break;
						case TOKEN_WILD_ATTR_IDENT:
							switch(state) {
						case STATE_CHILD: steps.add(new XPathStepChildAttr("*")); break;
						case STATE_DESC: steps.add(new XPathStepDescendantsAttr("*")); break;
							}
							break;
						case TOKEN_FUNCTION:
						case TOKEN_PARENT:
							steps.add(new XPathStepFindParent());
							break;
						case TOKEN_CURRENT:
							// step is pretty unnecessary here, 
							// but use steps.add(new XPathStepFindCurrent()); 
							/// if you really want to.
							break;
						default:
							break;
					}
				}

				return true;
			}

			bool execute(Node *startNode, Vector<Node*> &result) {
				//printf(" ////////////////// XPathTokenizer.execute(%x)\n", startNode);

				if(steps.size() == 0) return false;
				if(!startNode) {
					result.clear();
					return false;
				}

				Vector<Node*> *nodeSetBefore, *nodeSetAfter;
				nodeSetBefore = new Vector<Node*>();
				Vector<Node*> *initialNodeSetBefore = nodeSetBefore;
				nodeSetAfter = &result;
				nodeSetBefore->add(startNode);


				for(int i = 0; i < steps.size(); i++) {
					nodeSetAfter->clear();

					if(!steps[i]->selectNodes(*nodeSetBefore, *nodeSetAfter)) {
						return false;
					}

					Vector<Node*> *temp = nodeSetAfter;
					nodeSetAfter = nodeSetBefore;
					nodeSetBefore = temp;
				}

				if((steps.size()&0x1) == 0) {
					result = *nodeSetBefore; 
				} 

				delete initialNodeSetBefore;

				return true;
			}

		private:
			static XPathTokenizer xpathTokenizer;
			Vector<XPathStep*> steps;
		};
	} // namespace XPath
} // namespace MAUtil

#endif
