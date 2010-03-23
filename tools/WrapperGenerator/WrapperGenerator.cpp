/* Copyright (C) 2010 Mobile Sorcery AB

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

#include <map>
#include <string>
#include <vector>
#include <stack>
#include <expat.h>
#include <stdarg.h>
#include <stdio.h>
#include <fstream>

using namespace std;

namespace WrapperGenerator  {
	void error(const char* fmt, ...) {
		va_list argptr;
		va_start(argptr, fmt);
		int res = vprintf(fmt, argptr);
		va_end(argptr);

		exit(1);
	}

	string genstr(const char * fmt, ...) {
		va_list argptr;
		va_start(argptr, fmt);
		char str[1024];
		int res = vsprintf(str, fmt, argptr);
		va_end(argptr);
		return str;
	}

	class Base;

	struct ParseNode {
		ParseNode() : parent(NULL) {
		}

		string getAttr(const string& name, bool force=true) const {
			map<string, string>::const_iterator iter = attributes.find(name);
			if(iter != attributes.end()) return iter->second;
			else if(force) {
				error(genstr("Attribute '%s' missing on line %d\n", name.c_str(), lineNumber).c_str());
			}
			return "";
		}

		int getIntAttr(const string& name, bool force=true) const {
			string attr = getAttr(name, force);
			return atoi(attr.c_str());
		}

		string name;
		map<string, string> attributes;
		vector<const ParseNode*> children;

		ParseNode *parent;

		Base* base;

		int lineNumber;
	};

	static map<string, const ParseNode*> sIdToNode;

	const ParseNode* getParseNodeFromId(const string& name) {
		map<string, const ParseNode*>::iterator iter = sIdToNode.find(name);
		if(iter != sIdToNode.end()) return iter->second;
		else error("Node id missing");
		return NULL;
	}

	const Base* parseType(const ParseNode& node,  bool& isconst) {
		string type = node.getAttr("type");
		isconst = type[type.length()-1] == 'c';
		if(isconst)
			type.resize(type.length()-1);
		const Base* retType = getParseNodeFromId(type)->base;
		return retType;
	}

	class Base {
	public:

#define WRAPPER_TYPES(m)\
	m(Namespace, "Namespace")\
	m(Function, "Function")\
	m(PointerType, "PointerType")\
	m(ReferenceType, "ReferenceType")\
	m(Typedef, "Typedef")\
	m(CvQualifiedType, "CvQualifiedType")\
	m(ArrayType, "ArrayType")\
	m(FundamentalType, "FundamentalType")\
	m(FunctionType, "FunctionType")\
	m(File, "File")

#define DECLARE_ENUM(typeName, typeStr) E##typeName,

		enum Type {
			WRAPPER_TYPES(DECLARE_ENUM)
		};

		Base(Type type) : mType(type) {
		}

		virtual string toString() const {
			return "";
		}

		virtual void fromParseNode(const ParseNode& node) = 0;

		Type getBaseType() const {
			return mType;
		}

	private:
		Type mType;
	};

	class File;

	class Location {
	public:
		Location(int line, const File* file) : mLine(line), mFile(file) {
		}

		int getLine() const {
			return mLine;
		}

		const File* getFile() const {
			return mFile;
		}

		string toString() const;

	private:
		int mLine;
		const File* mFile;
	};
	
	class Namespace : public Base {
	public:
		Namespace() : Base(ENamespace){
		}

		void addMember(const Base* id) {
			mMembers.push_back(id);
		}


		void fromParseNode(const ParseNode& node) {
			mName = node.getAttr("name");
			const string& members = node.getAttr("members");
			
			size_t startIndex = 0;
			size_t endIndex = 0;
			while((endIndex=members.find_first_of(" ", startIndex)) != string::npos) {
				string member = members.substr(startIndex, endIndex-startIndex);
				startIndex = endIndex + 1;
				mMembers.push_back(getParseNodeFromId(member)->base);
			}

			string context = node.getAttr("context", false);
			mContext = (context!="")?getParseNodeFromId(context)->base:NULL;
		}

		string toString() const {
			return genstr("%s", mName.c_str());			
		}

		const string& getName() const {
			return mName;
		}

		const Base* getContext() const {
			return mContext;
		}

		const vector<const Base*>& getMembers() const {
			return mMembers;
		}

	private:
		string mName;
		const Base *mContext;
		vector<const Base*> mMembers;
	};

	class Argument {
	public:
		void fromParseNode(const ParseNode& node) {
			mType = NULL;

			if(node.name == "Ellipsis") {
				mIsEllipsis = true;
				return;
			}

			bool isConst;
			mName = node.getAttr("name", false);
			mType = parseType(node, isConst);
			mLocation = new Location(node.getIntAttr("line"), (File*)getParseNodeFromId(node.getAttr("file"))->base);
			
		}

		const string& getName() const {
			return mName;
		}

		const Base* getType() const {
			return mType;
		}

		const Location* getLocation() const {
			return mLocation;
		}

		string toString() const {
			if(mIsEllipsis) return "...";
			else return genstr("%s %s", mType->toString().c_str(), mName.c_str());
		}

		bool isEllipsis() const {
			return mIsEllipsis;
		}

	protected:
		bool mIsEllipsis;
		string mName;
		const Base* mType;
		Location *mLocation;
	};

	class Function : public Base {
	public:
		Function() : Base(EFunction) {
		}

		void fromParseNode(const ParseNode& node) {
			mName = node.getAttr("name");
			mReturns = getParseNodeFromId(node.getAttr("returns"))->base;
			mContext = getParseNodeFromId(node.getAttr("context"))->base;
			mIsExtern = node.getAttr("extern", false) !="";
			for(size_t i = 0; i < node.children.size(); i++) {
				const ParseNode* pn = node.children[i];
				Argument* arg = new Argument();
				arg->fromParseNode(*pn);
				mArguments.push_back(arg);
			}

			mLocation = new Location(node.getIntAttr("line"), (File*)getParseNodeFromId(node.getAttr("file"))->base);
		}


		const string& getName() const {
			return mName;
		}

		const Base* getContext() const {
			return mContext;
		}

		const Base* getReturnType() const {
			return mReturns;
		}

		const Location* getLocation() const {
			return mLocation;
		}

		bool isExtern() const {
			return mIsExtern;
		}

		const vector<const Argument*>& getArguments() const {
			return mArguments;
		}

		string toString() const {
			string argumentList;
			for(size_t i = 0; i < mArguments.size(); i++) {
				argumentList += mArguments[i]->toString();
				if(i!=mArguments.size()-1) argumentList+=", ";
			}

			return genstr("%s %s(%s)", mReturns->toString().c_str(), mName.c_str(), argumentList.c_str());
		}

	private:
		string mName;
		const Base *mReturns;
		const Base *mContext;
		const Location* mLocation;
		bool mIsExtern;
		vector<const Argument*> mArguments;
	};

	class PointerType : public Base {
	public:
		PointerType() : Base(EPointerType) {
		}

		void fromParseNode(const ParseNode& node) {
			mType = parseType(node, mIsConst);
			if(!mType) {
				int a = 2;
			}

			mSize = node.getIntAttr("size");
			mAlign = node.getIntAttr("align");
		}

		const Base* getType() const {
			return mType;
		}

		bool isConst() const {
			return mIsConst;
		}

		virtual string toString() const {
			return genstr("%s%s*", (mIsConst?"const ":""), mType->toString().c_str());
		}

	protected:
		const Base* mType;
		bool mIsConst;
		int mSize, mAlign;
	};

	class ReferenceType : public PointerType {
	public:
		virtual string toString() const {
			return genstr("%s%s&", (mIsConst?"const ":""), mType->toString().c_str());
		}
	};

	class FunctionType : public Base {
	public:
		FunctionType() : Base(EFunctionType) {
		}

		void fromParseNode(const ParseNode& node) {
			mReturns = getParseNodeFromId(node.getAttr("returns"))->base;
		}

		const Base* getReturnType() const {
			return mReturns;
		}

		string toString() const {
			return genstr("%s", mReturns->toString().c_str());
		}

	private:
		const Base* mReturns;
	};

	class Typedef : public Base {
	public:
		Typedef(): Base(ETypedef) {
		}

		void fromParseNode(const ParseNode& node) {
			mName = node.getAttr("name");
			mContext = getParseNodeFromId(node.getAttr("context"))->base;
			mLocation = new Location(node.getIntAttr("line"), (File*)getParseNodeFromId(node.getAttr("file"))->base);
			mType = getParseNodeFromId(node.getAttr("type"))->base;
		}

		const string& getName() const {
			return mName;
		}

		const Base* getType() const {
			return mType;
		}

		const Base* getContext() const {
			return mContext;
		}

		const Location* getLocation() const {
			return mLocation;
		}

		string toString() const {
			return genstr("%s", mName.c_str());
		}

	private:
		string mName;
		const Base* mType;
		const Base* mContext;
		const Location *mLocation;
	};

	class FundamentalType : public Base {
	public:
		FundamentalType() : Base(EFundamentalType) {
		}

		void fromParseNode(const ParseNode& node) {
			mName = node.getAttr("name");
		}

		const string& getName() const {
			return mName;
		}

		string toString() const {
			return genstr("%s", mName.c_str());
		}

	private:
		string mName;
	};

	class ArrayType : public Base {
	public:
		ArrayType() : Base(EArrayType) {
		}

		void fromParseNode(const ParseNode& node) {
			bool isConst;
			mType = parseType(node, isConst);
			mMin = node.getIntAttr("min");
			mMaxString = node.getAttr("max");
			mMax = atoi(mMaxString.c_str());
			mAlign = node.getIntAttr("align");
		}


		const Base* getType() const {
			return mType;
		}

		int getMin() const {
			return mMin;
		}

		int getMax() const {
			return mMax;
		}

		int getAlign() const {
			return mAlign;
		}

		string toString() const {
			return genstr("%s[%s]", mType->toString().c_str(), mMaxString.c_str());
		}

	private: 
		string mMaxString;
		int mMin, mMax, mAlign;
		const Base* mType;
	};

	class CvQualifiedType : public Base {
	public:
		CvQualifiedType() : Base(ECvQualifiedType) {
		}

		void fromParseNode(const ParseNode& node) {
			mType = parseType(node, mIsConst);
			if(!mType) {
				int a = 2;
			}

			string c = node.getAttr("const", false);
			if(!mIsConst && c=="1")
				mIsConst = true;	
		}

		string toString() const {
			return genstr("%s%s*", (mIsConst?"const ":""), mType->toString().c_str());
		}

		const Base* getType() const {
			return mType;
		}

		bool isConst() const {
			return mIsConst;
		}

	private:
		const Base* mType;
		bool mIsConst;
	};


	class File : public Base {
	public:
		File() : Base(EFile) {
		}

		void fromParseNode(const ParseNode& node) {
			mName = node.getAttr("name");
		}

		const string& getName() const {
			return mName;
		}

		string toString() const {
			return genstr("%s", mName.c_str());
		}

	private:
		string mName;
	};

	string Location::toString() const {
		return genstr("%d:%s", mLine, mFile->toString());
	}

#define DEFINE_FACTORY(typeName, typeStr) \
	if(name==typeStr) return new typeName();

	Base* createNode(const string& name) {
		WRAPPER_TYPES(DEFINE_FACTORY)
		else return NULL;
	}

	class Backend {
	public:
		typedef multimap<string, const Base*> BasesMap;
		typedef BasesMap::const_iterator BasesIterator;

		void generate(const BasesMap& bases, const string& outputFile) {
			fstream stream;
			stream.open(outputFile.c_str(), ios::out);
			emit(bases, stream);
			stream.close();
		}
	protected:
		virtual void emit(const BasesMap& bases, fstream& stream) = 0;
	

	};

	class IDLBackend : public Backend {
	protected:

		// a bit messy this function...
		string getIDLType(const Base* base, bool isArgument=true) {
			string ret = "";
			if(base->getBaseType() == Base::EFundamentalType) {
				const FundamentalType* ft = (const FundamentalType*) base;
				string name = ft->getName();
				if(isArgument)
					ret += "in ";
				ret += name;
			} else if(base->getBaseType() == Base::ECvQualifiedType) {
				const CvQualifiedType* qt = (const CvQualifiedType*) base;
				ret += getIDLType(qt->getType());
			} else if(base->getBaseType() == Base::EPointerType) {
				const PointerType* pt = (const PointerType*) base;
				if(isArgument) {
					if(pt->isConst())
						ret += "in ";
					else
						ret += "out ";
				}

				const Base* pType = pt->getType();
				if(pType->getBaseType() == Base::EFundamentalType) {
					const FundamentalType* pfType = (const FundamentalType*)pType;
					if(pfType->getName() == "char")
						ret += "MAString";
					else if(pfType->getName() == "void") 
						ret += "MAAddress";
					else
						ret += getIDLType(pt->getType(), false);
				} else if(pType->getBaseType() == Base::EFunctionType){
					ret += "int"; // function address. We should probably be able to handle them...
				} else {
					ret += getIDLType(pt->getType(), false);
				}

			} else if(base->getBaseType() == Base::ETypedef) {
				const Typedef* td = (const Typedef*) base;
				ret += (isArgument?"in ":"") + td->getName();
			} else {
				error("don't know how to handle type\n");
			}

			return ret;
		}

		void emit(const BasesMap& bases, fstream& stream) {
			pair<BasesIterator, BasesIterator> typedefs = bases.equal_range("Typedef");
			for(BasesIterator td = typedefs.first; td!=typedefs.second; td++) {
				const Typedef* t = (const Typedef*)td->second;
				stream << "typedef " << getIDLType(t->getType(), false) << " " << t->getName() << ";\n";

			}

			pair<BasesIterator, BasesIterator> functions = bases.equal_range("Function");
			
			for(BasesIterator function = functions.first; function!=functions.second; function++) {
				const Function* func = (const Function*)function->second;
				string name = func->getName();
				stream << func->getReturnType()->toString() << name << "(";
				const std::vector<const Argument*>& args = func->getArguments();
				if(args.size()>4) {
				} else {
					for(int i = 0; i < args.size(); i++) {
						stream << getIDLType(args[i]->getType()) << " " << args[i]->getName();
						if(i != args.size()-1) stream << ", ";
					}
				}

				stream << ");\n";
			}

		}	
	};
}

using namespace WrapperGenerator;

char *readFileIntoMem(const char* filename, int *len) {
	FILE *file = fopen(filename, "rb");
	if(!file) return NULL;
	fseek(file, 0, SEEK_END);
	int length = ftell(file);
	fseek(file, 0, SEEK_SET);
	char *memory = new char[length];
	fread(memory, length, 1, file);
	fclose(file);
	*len = length;
	return memory;
}

static stack<ParseNode*> sParseStack;
static XML_Parser sXmlParser;

void
start(void *data, const char *el, const char **attr) {
	ParseNode* parseNode = new ParseNode();
	parseNode->name = el;
	sParseStack.push(parseNode);
	for (int i = 0; attr[i]; i += 2) {
		parseNode->attributes[attr[i]] = attr[i+1];
	}
	parseNode->base = WrapperGenerator::createNode(parseNode->name);
	parseNode->lineNumber = XML_GetCurrentLineNumber(sXmlParser);

}  /* End of start handler */

void
end(void *data, const char *el) {
	ParseNode* child = sParseStack.top();
	if(sParseStack.size()!=1) {
		sParseStack.pop();
		ParseNode* parent = sParseStack.top();
		child->parent = parent;
		parent->children.push_back(child);
	}

}  /* End of end handler */

int main(int argc, char **argv) {

	sXmlParser = XML_ParserCreate("UTF-8");
	XML_SetElementHandler(sXmlParser, start, end);

	int fileLength;
	char *file = readFileIntoMem("tests/sqlite.xml", &fileLength);
	XML_Parse(sXmlParser, file, fileLength, 1);
	delete file;
	XML_ParserFree(sXmlParser);

	const vector<const ParseNode*>& parseNodes = sParseStack.top()->children;

	// setup id map
	for(size_t i = 0; i < parseNodes.size(); i++) {
		string id = parseNodes[i]->getAttr("id", false);
		if(id!="") {
			WrapperGenerator::sIdToNode[id] = parseNodes[i];
		}
	}

	Backend::BasesMap bases;

	// bind pointers.
	for(size_t i = 0; i < parseNodes.size(); i++) {
		if(parseNodes[i]->base) {
			parseNodes[i]->base->fromParseNode(*(parseNodes[i]));	
			bases.insert(pair<string, const Base*>(parseNodes[i]->name, parseNodes[i]->base));
		}
	}

	// write info.
	for(size_t i = 0; i < parseNodes.size(); i++) {
		if(parseNodes[i]->base)
			printf("%s\n", parseNodes[i]->base->toString().c_str());
	}

	Backend *backend = new IDLBackend();

	backend->generate(bases, "tests/sqlite.idl");

}
