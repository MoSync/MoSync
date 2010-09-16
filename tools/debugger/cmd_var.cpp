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

#include <string>
#include <set>
#include <vector>
#include <stdio.h>
#include <queue>

#include "config.h"
#include "helpers/log.h"

#include "helpers.h"
#include "globals.h"
#include "commandInterface.h"
#include "old_expr.h"
#include "StubConnection.h"

#include "expression.h"

using namespace std;

void var_create(const string& args);
void var_delete(const string& args);
void var_update(const string& args);
void var_evaluate_expression(const string& args);
void var_show_attributes(const string& args);

void var_info_expression(const string& args);
void var_info_num_children(const string& args);
void var_info_num_children(const string& args);
void var_info_path_expression(const string& args);
void var_info_type(const string& args);
void var_list_children(const string& args);
void var_set_format(const string& args);
void var_show_format(const string& args);

//******************************************************************************
// Callbacks
//******************************************************************************

namespace Callback {
	static void varEECreate(const Value* value, const char *err);
	static void varEEUpdate(const Value* value, const char *err);

	static void varCreate();
	static void varUpdate();
	static void varEvaluateExpression();
	//static void regUpdate(const Registers& r);

	static void varListChildren();
};

//******************************************************************************
// types
//******************************************************************************

enum ExpUpdateResult {
	EXP_PARSE_ERROR,
	EXP_OUT_OF_SCOPE,
	EXP_EVAL_FAILED,
	EXP_OK
};

class Expression {
public:
	Expression(int frameAddr, const string& exprText) : mFrameAddr(frameAddr), mExprText(exprText), mExprTree(NULL), mIsValid(false) {}
	virtual ~Expression() {}

	virtual ExpUpdateResult update(ExpressionCallback ecb);
	bool updated() const { return mUpdated; }
	void outdate() { mUpdated = false; }

	// fredrik: removed mValue.size() check for vars with children..
	bool ok() const { return /*mValue.size() > 0 && */ mType.size() > 0; }

	const string& type() const { return mType; }
	const string& value() const { return mValue; }
	bool simpleType() const { return mSimpleType; }

	void setValid(bool v=true) { mIsValid = v; }
	bool isValid() const { return mIsValid; }

	const string& getExprText() { return mExprText; }
	void setExprTree(ExpressionTree* expr) { mExprTree = expr; }

	void updateData(std::string _value, std::string _type, bool _simpleType) {
		if(mValue != _value || mType != _type || mSimpleType != _simpleType) {
			mValue = _value;
			mType = _type;
			mSimpleType = _simpleType;
			mUpdated = true;
		} else {
			mUpdated = false;
		}
		mIsValid = true;
	}

protected:
	const int mFrameAddr;
	std::string mExprText;
	ExpressionTree *mExprTree;

	string mType, mValue;
	bool mSimpleType;
	bool mUpdated;
	bool mIsValid;

	friend /* static */ void Callback::varEECreate(const Value* value, const char *err);
	friend /* static */ void Callback::varEEUpdate(const Value* value, const char *err);
	friend struct Variable;
};


#if 0	//unknown syntax
class AddrExp : public Expression {
public:
	AddrExp(int start, int end);
};
#endif

struct Variable {

	~Variable();

	string name;
	string localName;
	Expression* exp;
	//vector<Variable> children;
	//map<string, Variable> children;
	map<int, Variable> children;

	TypeBase::PrintFormat printFormat;
	bool mHasCreatedChildren;

	bool outOfScope;

	void addArray(const char* dataAdress, const ArrayType *arrayType,  bool shouldCreateChildren, bool invalidMemory=false);
	void addStruct(const char* dataAdress, const StructType *arrayType, bool shouldCreateChildren, bool isPointer=false, bool invalidMemory=false);
	void addPointer(const char* dataAdress, const PointerType *arrayType, bool shouldCreateChildren, bool invalidMemory=false);
};



//******************************************************************************
// helper function declarations
//******************************************************************************

static Variable* findVariable(const string& name);
static bool printUpdateItem(bool comma, Variable* var);

//******************************************************************************
// statics
//******************************************************************************

static int sAutoNameCounter = 0;
static map<string, Variable*> sVariableMap;
static map<string, Variable*> sRootVariableMap;

static Variable* sVar = NULL;
static Expression* sExp;
static void (*sUpdateCallback)();
static PrintValueSimplicity sPrintValueSimplicity;
static queue<Variable*> sUpdateQueue;

//******************************************************************************
// type function definitions
//******************************************************************************
Variable::~Variable() {
	//	for(size_t i = 0; i < children.size(); i++) {
	for(map<int, Variable>::iterator i = children.begin(); i != children.end(); i++) {
		//sVariableMap.erase(children[i].name);
		sVariableMap.erase(i->second.name);
	}
	sVariableMap.erase(name);
}

void Variable::addArray(const char* dataAdress, const ArrayType *arrayType, bool shouldCreateChildren, bool invalidMemory) {
	const TypeBase* deref = ((const ArrayType*)arrayType)->mElemType->resolve();
	deref = convertConstType(deref);

	std::string type = getType(arrayType, false);
	std::string value = ""; 
	this->exp->updateData(value, type, arrayType->isSimpleValue());

	for(int i = 0; i < arrayType->mLength; i++) {
		Variable& var = children[i];
		StringPrintFunctor spf;
		spf("[%d]", i);
		string varName = spf.getString();
		spf.reset();
		//Variable& var = children[varName];
		var.name = name+"."+varName;
		var.outOfScope = false;
		var.printFormat = TypeBase::eNatural;
	}

	//children.resize(arrayType->mLength);
	if(!shouldCreateChildren) { 
		mHasCreatedChildren = false;
		return; 
	}
	mHasCreatedChildren = true;


	for(int i = 0; i < arrayType->mLength; i++) {
		StringPrintFunctor spf;
		spf("[%d]", i);
		string varName = spf.getString();
		spf.reset();

		//		Variable& var = children[varName];
		Variable& var = children[i];
		{
			/*
			var.outOfScope = false;
			var.printFormat = TypeBase::eNatural;

			//StringPrintFunctor spf;
			spf("%s.%s", name.c_str(), varName.c_str());
			var.name = spf.getString();
			spf.reset();
			*/
			var.localName = varName;

			spf("(%s)[%d]", exp->mExprText.c_str(), i);
			var.exp = new Expression(exp->mFrameAddr, spf.getString());
			spf.reset();
		}

		sVariableMap[var.name] = &var;

		if(deref->type() == TypeBase::eArray) {
			var.addArray(NULL, (ArrayType*)deref, false, invalidMemory);
		} else if(deref->type() == TypeBase::eStruct) {
			var.addStruct(NULL, (StructType*)deref, false, invalidMemory);
		} else if(deref->type() == TypeBase::ePointer) {
			var.addPointer(NULL, (PointerType*)deref, false, invalidMemory);
		} else {
			value = "";
			if(!invalidMemory && dataAdress) value = getValue(deref, dataAdress+i*deref->size(), var.printFormat);
			var.exp->updateData(
				value, 
				getType(deref, false), 
				deref->isSimpleValue());
		}
	}	
}

void Variable::addPointer(const char* dataAdress, const PointerType *pointerType, bool shouldCreateChildren, bool invalidMemory) {
	const TypeBase* deref = ((const PointerType*)pointerType)->deref()->resolve();
	deref = convertConstType(deref);

	std::string type = getType(pointerType, false);
	std::string value = "";

	if(dataAdress)
		value = getValue(pointerType, dataAdress, printFormat);
	bool simpleType = pointerType->isSimpleValue();

	this->exp->updateData(value, type, simpleType);

	if(deref->type() == TypeBase::eBuiltin && ((Builtin*)deref)->mSubType==Builtin::eVoid) {
		// if it's a void-pointer we don't know the size of the data it is pointing to, thus we don´t give the variable a child.
		return;		
	}

	//children.resize(1);
	if(!shouldCreateChildren) { 
		if(deref->type() != TypeBase::eStruct) {
			StringPrintFunctor spf;
			spf("*(%s)", localName.c_str());
			string varName = spf.getString();
			spf.reset();
			children[0].localName = varName;
		} else {
			addStruct(NULL, (StructType*)deref, false, true, true);	
		}

		mHasCreatedChildren = false;	
		return; 
	}
	mHasCreatedChildren = true;

	if(deref->type() != TypeBase::eStruct) {
		StringPrintFunctor spf;
		spf("*(%s)", localName.c_str());
		string varName = spf.getString();
		spf.reset();

		//		Variable& var = children[varName];
		Variable& var = children[0];
		{
			var.outOfScope = false;
			var.printFormat = TypeBase::eNatural;

			//StringPrintFunctor spf;
			spf("%s.%s", name.c_str(), varName.c_str());
			var.name = spf.getString();
			spf.reset();

			var.localName = varName;

			spf("*(%s)", exp->mExprText.c_str());
			var.exp = new Expression(exp->mFrameAddr, spf.getString());
			spf.reset();
		}
		if(deref->type() == TypeBase::eArray) {
			var.addArray(NULL, (ArrayType*)deref, false);
		} else if(deref->type() == TypeBase::ePointer) {
			var.addPointer(NULL, (PointerType*)deref, false);
		} else {
			value = "";
			if(!invalidMemory) {
				int addr = *((int*)dataAdress);
				value = getValue(pointerType, &gMemBuf[addr], printFormat);
			}
			var.exp->updateData(
				value, 
				getType(deref, false), 
				deref->isSimpleValue());
		}
		sVariableMap[var.name] = &var;
	}  else {

		if(invalidMemory) {
			addStruct(NULL, (StructType*)deref, true, true, invalidMemory);
		} else {
			int addr = *((int*)dataAdress);
			addStruct(&gMemBuf[addr], (StructType*)deref, true, true, invalidMemory);
		}
	}

}

static bool isVTablePointer(const TypeBase* deref) {
	if(deref->type() == TypeBase::ePointer) {
		const PointerType* pt = (const PointerType*)deref;
		const TypeBase *t = pt->mTarget->resolve();
		if(t->type() == TypeBase::eBuiltin) {
			const Builtin* bi = (const Builtin*)t;
			if(bi->mSubType == Builtin::eVTablePtr) return true;
		}
	}
	return false;
}

void Variable::addStruct(const char* dataAdress, const StructType *structType, bool shouldCreateChildren, bool isPointer, bool invalidMemory) {
	const vector<BaseClass>& bases = structType->getBases();
	const vector<DataMember>& dataMembers = structType->getDataMembers();

	std::string value = ""; 
	if(!isPointer) {
		std::string type = getType(structType, false);
		this->exp->updateData(value, type, structType->isSimpleValue());
	}

	//	children.resize(bases.size()+dataMembers.size());

	// add private/public/protected variables..

	for(size_t i = 0; i < dataMembers.size(); i++) {
		const TypeBase* deref = dataMembers[i].type->resolve();
		deref = convertConstType(deref);
		if(isVTablePointer(deref)) continue;

		string virtualVarName = getVisibilityString(dataMembers[i].visibility);
		Variable& virtualVar = children[bases.size()+(int)dataMembers[i].visibility];
		virtualVar.localName = virtualVarName;
		virtualVar.exp = NULL;
		virtualVar.name = name + "." + virtualVarName;
		virtualVar.mHasCreatedChildren = true;
		virtualVar.outOfScope = false;
		sVariableMap[virtualVar.name] = &virtualVar;
	}

	if(!shouldCreateChildren) { 
		mHasCreatedChildren = false;
		//return; 
	}

	mHasCreatedChildren = true;


	for(size_t i = 0; i < bases.size(); i++) {
		const TypeBase* deref = bases[i].type->resolve();
		string varName = ((StructType*)deref)->mName.c_str();

		Variable& var = children[i];
		{
			var.outOfScope = false;
			var.printFormat = TypeBase::eNatural;

			string type = getType(deref, false);

			StringPrintFunctor spf;
			spf("%s.%s", name.c_str(), varName.c_str());
			var.name = spf.getString();
			spf.reset();

			var.localName = varName;

			//if(isPointer) spf("((%s *)(%s))", type.c_str(), exp->mExprText.c_str());
			if(isPointer) spf("((%s)(*(%s)))", type.c_str(), exp->mExprText.c_str());
			else spf("((%s)(%s))", type.c_str(), exp->mExprText.c_str());
			var.exp = new Expression(exp->mFrameAddr, spf.getString());
			spf.reset();
		}

		sVariableMap[var.name] = &var;

		if(shouldCreateChildren)
			var.addStruct(dataAdress+bases[i].offset, (const StructType*)deref, false, false, invalidMemory);
	}

	for(size_t i = 0; i < dataMembers.size(); i++) {
		const TypeBase* deref = dataMembers[i].type->resolve();
		deref = convertConstType(deref);

		if(isVTablePointer(deref)) continue;

		string virtualVarName = getVisibilityString(dataMembers[i].visibility);
		Variable& virtualVar = children[bases.size()+(int)dataMembers[i].visibility];

		string varName = dataMembers[i].name;
		sVariableMap[virtualVar.name] = &virtualVar;

		Variable& var = virtualVar.children[i];
		{
			var.outOfScope = false;
			var.printFormat = TypeBase::eNatural;

			string type = getType(deref, false);

			StringPrintFunctor spf;
			spf("%s.%s", virtualVar.name.c_str(),  varName.c_str());
			var.name = spf.getString();
			spf.reset();

			var.localName = varName;

			if(isPointer) spf("(%s)->%s", exp->mExprText.c_str(), dataMembers[i].name.c_str());
			else spf("(%s).%s", exp->mExprText.c_str(), dataMembers[i].name.c_str());
			var.exp = new Expression(exp->mFrameAddr, spf.getString());
			spf.reset();
		}
		sVariableMap[var.name] = &var;

		if(shouldCreateChildren) {
			if(deref->type() == TypeBase::eArray) {
				var.addArray(NULL, (ArrayType*)deref, false, invalidMemory);
			} else if(deref->type() == TypeBase::eStruct) {
				var.addStruct(NULL, (StructType*)deref, false, false, invalidMemory);
			} else if(deref->type() == TypeBase::ePointer) {
				var.addPointer(NULL, (PointerType*)deref, false, invalidMemory);
			} else {
				value = "";
				if(dataAdress && !invalidMemory) value = getValue(deref, dataAdress+(dataMembers[i].offsetBits>>3), var.printFormat);
				var.exp->updateData(
					value, 
					getType(deref, false), 
					deref->isSimpleValue());
			}
		}
	}
}

static void Callback::varEECreate(const Value* v, const char *err) {
	if(err) { 
		error("%s", err); 
		return; 
	}

	//const SYM& sym = v->getSymbol();
	const TypeBase* typeBase = convertConstType(v->getTypeBase());

	if(v->isType()) {
		error("Variable is a type.");
		return;
	}

	if(typeBase->type() == TypeBase::eArray) {
		sVar->addArray((const char*)v->getDataAddress(), (const ArrayType*)typeBase, true, !v->isDereferencable());
	} else if(typeBase->type() == TypeBase::eStruct) {
		sVar->addStruct((const char*)v->getDataAddress(), (const StructType*)typeBase, true, false, false);
	} else if(typeBase->type() == TypeBase::ePointer) {
		sVar->addPointer((const char*)v->getDataAddress(), (const PointerType*)typeBase, true, !v->isDereferencable());	

	} else {
		std::string type = getType(typeBase, false);
		std::string value = getValue(typeBase, v->getDataAddress(), sVar->printFormat);
		bool simpleType = typeBase->isSimpleValue();
		sExp->updateData(value, type, simpleType);
	}
	sUpdateCallback();
}

static void Callback::varEEUpdate(const Value* v, const char *err) {
	if(err) { 
		error("%s", err); 
		return; 
	}

	const TypeBase* typeBase = convertConstType(v->getTypeBase());

	if(v->isType()) {
		error("Variable is a type.");
		return;
	}

	std::string type = getType(typeBase, false);
	bool simpleType = typeBase->isSimpleValue();
	std::string value = "";

	if(v->getType() == TypeBase::eArray) {
	} else if(v->getType() == TypeBase::eStruct) {
	} else {
		value = getValue(typeBase, v->getDataAddress(), sVar->printFormat);
	}
	sExp->updateData(value, type, simpleType);
	sUpdateCallback();
}

static void resetValidness() {
	map<string, Variable*>::iterator i = sVariableMap.begin();
	for(;i!=sVariableMap.end(); i++) {
		if(i->second->exp)
			i->second->exp->setValid(false);
	}
}

ExpUpdateResult Expression::update(ExpressionCallback ecb) {
	static bool first = true;
	if(first) {
		first = false;
		StubConnection::addContinueListener(resetValidness);
	}

	sExp = this;

	// TODO: urgah, this optimization doesn't work just yet?
	//if(mIsValid) { sUpdateCallback(); return; }

	//asynchronous
	//problematic; either calls the callback or error().
	//we want to handle the error ourselves, lest we have a memory leak (or worse).
	if(!mExprTree) {
		try {
			mExprTree = ExpressionParser::parse(mExprText.c_str());
		} catch(ParseException& e) {
			error("%s", e.what());
			return EXP_PARSE_ERROR;
		}
	} else {
		// variable is already created. check if any variable is out of scope in this expression. If so, set it as out of scope and skip updation.
		map<std::string, SYM>& symbols = mExprTree->getSymbols();
		map<std::string, SYM>::iterator i = symbols.begin();
		for(; i!=symbols.end(); i++) {
			const SYM::Scope& s = i->second.scope;
			switch(s.type) {
				case SYM::Scope::eGlobal:
					// do nothing, always in scope.
					break;
				case SYM::Scope::eStatic: {
					// must be in the current file scope.
					ASSERT_REG;
					int fileScope = getFileScope(r.pc);
					if(fileScope==-1 || fileScope!=s.fileScope) { sVar->outOfScope = true; mUpdated = true; return EXP_OUT_OF_SCOPE; }
										  }
										  break;
				case SYM::Scope::eLocal: {
					// must be in the current scope.
					ASSERT_REG;
					if(r.pc<s.start || r.pc>s.end) { sVar->outOfScope = true; mUpdated = true; return EXP_OUT_OF_SCOPE; }
										 }
										 break;
			}
		}	
		sVar->outOfScope = false;
	}

	if(mExprTree) {
		stackEvaluateExpressionTree(mExprTree, mFrameAddr, ecb);
	} else {
		error("Parsing of expression failed.");
		return EXP_EVAL_FAILED;
	}

	return EXP_OK;
}

//******************************************************************************
// helper function definitions
//******************************************************************************
static Variable* findVariable(const string& name) {
	map<string, Variable*>::iterator itr = sVariableMap.find(name);
	if(itr == sVariableMap.end())
		return NULL;
	else
		return itr->second;
}

//******************************************************************************
// create
//******************************************************************************

void var_create(const string& args) {
	//split args
	vector<string> argv;
	splitArgs(args, argv);
	if(argv.size() != 3) {
		error("Wrong number of arguments");
		return;
	}

	//name
	string& name(argv[0]);
	string realName;
	if(name == "-") {
		char buffer[32];
		sprintf(buffer, "var%i", ++sAutoNameCounter);
		realName = buffer;
	} else {
		realName = name;
	}
	Variable* oldVar = findVariable(realName);
	if(oldVar != NULL) {
		error("Variable name already taken");
		return;
	}

	_ASSERT(sVar == NULL);
	sVar = new Variable;
	sVar->printFormat = TypeBase::eNatural;
	sVar->name = realName;
	sVar->outOfScope = false;

	//frame-addr
	string& frameAddrString(argv[1]);
	int frameAddr;
	if(frameAddrString == "*") {
		frameAddr = -1;//Stack::currentFrame().framePtr;
	} else {
		int len;
		int res = sscanf(frameAddrString.c_str(), "%i%n", &frameAddr, &len);
		if(res != 1 || len != (int)frameAddrString.size()) {
			error("Bad frame address format");
			return;
		}
	}

	//expression
	string& exp(argv[2]);

	sVar->exp = new Expression(frameAddr, exp);

	sVar->localName = exp;

	/*
	if(exp[0] == '$') {	//register
	int reg;
	if(!parseArgRegName(exp.substr(1), &reg))
	return;
	sVar->exp = new RegExp(frameAddr, reg);
	#if 0
	} else if(exp[0] == '*') {	//address (-range)
	int start, end, len;
	int res = sscanf(exp.c_str() + 1, "%i-%i%n", &start, &end, &len);
	if(res == 2 || len == exp.size()) {
	sVar->exp = new AddrExp(frameAddr, start, end);
	} else {
	//try address only
	res = sscanf(exp.c_str() + 1, "%i%n", &start, &len);
	if(res != 1 || len != exp.size()) {
	error("Bad expression address format");
	return;
	}
	sVar->exp = new AddrExp(frameAddr, start, start);
	}
	#endif
	} else {	//variable-expression
	sVar->exp = new VarExp(frameAddr, exp);
	}
	*/

	sUpdateCallback = Callback::varCreate;
	sVar->exp->update(Callback::varEECreate);
}

static void Callback::varCreate() {
	if(!sVar->exp->ok()) {
		delete sVar->exp;
		error("Could not evaluate expression");
		return;
	}

	oprintDone();

	oprintf(",name=\"%s\",numchild=\"%"PFZT"\",type=\"%s\",value=\"",
		sVar->name.c_str(), sVar->children.size(), sVar->exp->type().c_str());
	if(sVar->exp->value()!="")
		oprintf("%s", sVar->exp->value().c_str());
	else
		oprintf("{...}");
	oprintf("\"\n");

	sVar->exp->outdate();
	sVariableMap[sVar->name] = sVar;
	sRootVariableMap[sVar->name] = sVar;
	sVar = NULL;
	commandComplete();
}

//******************************************************************************
// delete
//******************************************************************************

void var_delete(const string& args) {
	bool childrenOnly = false;
	//split args
	vector<string> argv;
	splitArgs(args, argv);
	string name;
	if(argv.size() == 1) {
		name = argv[0];
	} else if(argv.size() == 2) {
		if(argv[0] != "-c") {
			error("Bad argument");
			return;
		}
		name = argv[1];
		childrenOnly = true;
	} else {
		error("Bad number of arguments");
		return;
	}

	//find/delete
	Variable *v = findVariable(name);
	if(!v) {
		error("Variable does not exist");
		return;
	} else {
		sRootVariableMap.erase(name);
		delete v;
	}

	oprintDone();
	commandComplete();
}

//******************************************************************************
// update
//******************************************************************************
void var_update(const string& args) {
	sPrintValueSimplicity = eNameOnly;	//default
	//split args
	vector<string> argv;
	splitArgs(args, argv);
	string name;
	if(argv.size() == 1) {
		name = argv[0];
	} else if(argv.size() == 2) {
		name = argv[1];
		if(!parseArgPVS(argv[0], &sPrintValueSimplicity))
			return;
	} else {
		error("Bad number of arguments");
		return;
	}

	if(name == "*") {	//all variables...
		for(map<string, Variable*>::iterator i =
			sRootVariableMap.begin(); i!=sRootVariableMap.end(); i++) {
				sUpdateQueue.push(i->second);
		}
	} else {
		Variable* var = findVariable(name);
		if(!var) {
			error("Variable does not exist");
			return;
		}
		sUpdateQueue.push(var);
	}
	sUpdateCallback = Callback::varUpdate;
	Callback::varUpdate();
}

static void Callback::varUpdate() {
	if(!sUpdateQueue.empty()) {
		Variable *v = NULL;

		do {
			v = sUpdateQueue.front();
			sUpdateQueue.pop();
			//_ASSERT(sVar == NULL);
			sVar = v;
			if(v->exp)
				v->exp->update(Callback::varEEUpdate);
			//sVar = NULL;
		} while(v->outOfScope && !sUpdateQueue.empty());

		if(v && !v->outOfScope) {
			if(v->mHasCreatedChildren)
				for(map<int, Variable>::iterator i = v->children.begin(); i!=v->children.end(); i++) {
					sUpdateQueue.push(&i->second);
				}
				if(!v->exp) {
					varUpdate();
				}
				return;

		}
	}

	oprintDone();
	oprintf(",changelist=[");
	map<string, Variable*>::iterator itr = sVariableMap.begin();
	bool comma = false;
	while(itr != sVariableMap.end()) {
		comma |= printUpdateItem(comma, itr->second);
		itr++;
	}
	oprintf("]\n");
	sVar = NULL;
	commandComplete();
}

static void printValue(Variable *var) {
	switch(sPrintValueSimplicity) {
	case eNameOnly:
		break;
	case eSimpleValues:
		oprintf(",value=\"");
		if(var->exp) {
			if(var->exp->simpleType())
				oprintf("%s", var->exp->value().c_str());
			else
				oprintf("{...}");
		}
		oprintf("\"");
		break;
	case eAllValues:
		oprintf(",value=\"%s\"", var->exp->value().c_str());
		break;
	}
}

static void printListChildrenItem(Variable* var) {
	oprintf("{name=\"%s\"", var->name.c_str());
	oprintf(",numchild=\"%"PFZT"\"", var->children.size());
	printValue(var);
	oprintf(",type=\"%s\"", (!var->exp)?"":var->exp->type().c_str());	
	oprintf(",exp=\"%s\"", var->localName.c_str());
	oprintf("}");
	if(var->exp)
		var->exp->outdate();
}


//returns true if anything was printed
static bool printUpdateItem(bool comma, Variable* var) {
	if(var->exp && var->exp->updated()) {
		if(comma) oprintf(",");
		var->exp->outdate();
		oprintf("{name=\"%s\"", var->name.c_str());
		printValue(var);
		oprintf(",in_scope=\"%s\",type_changed=\"false\"", (var->outOfScope==false)?"true":"false");
		oprintf("}");
		return true;
	}

	/*
	if(!var->outOfScope && var->mHasCreatedChildren) {
	//for(size_t i = 0; i < var->children.size(); i++)
	for(map<string, Variable>::iterator i = var->children.begin(); i!=var->children.end(); i++) {
	comma |= printUpdateItem(comma, &i->second);
	}
	}
	*/

	return false;
}

//******************************************************************************
// evaluate-expression
//******************************************************************************

// should be able to handle -f parameter to set format of output without changing the format in the variable.
void var_evaluate_expression(const string& args) {
	//split args
	vector<string> argv;
	splitArgs(args, argv);
	string name;
	if(argv.size() == 1) {
		name = argv[0];
	} else {
		error("Bad number of arguments");
		return;
	}

	Variable* var = findVariable(name);
	if(!var) {
		error("Variable does not exist");
		return;
	}

	_ASSERT(sVar == NULL);
	sVar = var;
	if(/*!var->exp->isValid()*/!var->exp->updated()) {
		sUpdateCallback = Callback::varEvaluateExpression;
		ExpUpdateResult res = var->exp->update(Callback::varEEUpdate);
		if(res == EXP_OUT_OF_SCOPE) {
			error("Variable out of scope.");
		}
	}
	else
		Callback::varEvaluateExpression();
}

static void Callback::varEvaluateExpression() {
	oprintDone();
	oprintf(",value=\"");
	if(sVar->exp->simpleType())
		oprintf("%s", sVar->exp->value().c_str());
	else
		oprintf("{...}");
	oprintf("\"\n");
	sVar = NULL;
	commandComplete();
}

//******************************************************************************
// show-attributes
//******************************************************************************
void var_show_attributes(const string& args) {
	oprintDone();
	oprintf(",status=noneditable\n");
	commandComplete();
}

//******************************************************************************
// show and set format
//******************************************************************************
static Variable* getAndValidateVariable(const string& args) {
	vector<string> argv;
	splitArgs(args, argv);
	string name;
	if(argv.size() == 1) {
		name = argv[0];
	} else {
		error("Bad number of arguments");
		return NULL;
	}

	Variable* var = findVariable(name);
	if(!var) {
		error("Missing variable");
		return NULL;
	}

	return var;	
}

void var_show_format(const string& args) {
	Variable *var = getAndValidateVariable(args);
	if(!var) return;

	string format;
	switch(var->printFormat) {
		case TypeBase::eNatural: format = "natural"; break;
		case TypeBase::eHexadecimal: format = "hexadecimal"; break;
		case TypeBase::eBinary: format = "binary"; break;
		case TypeBase::eDecimal: format = "decimal"; break;
		case TypeBase::eOctal: format = "octal"; break;
		default: error("Invalid format"); return;
	}
	oprintDone();
	oprintf(",format=\"%s\"\n", format.c_str());
	commandComplete();
}

void var_set_format(const string& args) {
	vector<string> argv;
	splitArgs(args, argv);
	string name;
	string format;
	if(argv.size() == 2) {
		name = argv[0];
		format = argv[1];
	} else {
		error("Bad number of arguments");
		return;
	}


	Variable* var = findVariable(name);
	if(!var) {
		error("Missing variable");
		return;
	}

	if(format == "natural") var->printFormat = TypeBase::eNatural;
	else if(format == "hexadecimal") var->printFormat = TypeBase::eHexadecimal;
	else if(format == "binary") var->printFormat = TypeBase::eBinary;
	else if(format == "decimal") var->printFormat = TypeBase::eDecimal;
	else if(format == "octal") var->printFormat = TypeBase::eOctal;
	else { error("Invalid format"); return; }

	oprintDoneLn();
	commandComplete();
}

void var_list_children(const string& args) {
	vector<string> argv;
	splitArgs(args, argv);
	string name;
	if(argv.size() == 1) {
		name = argv[0];
	}
	else if(argv.size() == 2) {
		name = argv[1];
		if(!parseArgPVS(argv[0], &sPrintValueSimplicity)) {
			error("Invalid argument.");
			return;
		}
	} else {
		error("Invalid number of arguments.");	
		return;
	}

	Variable *var = findVariable(name);
	if(!var) {
		error("Missing variable.");
		return;
	}
	_ASSERT(sVar == NULL);

	sUpdateQueue.push(var);

	sUpdateCallback = Callback::varListChildren;
	sVar = var;

	Callback::varListChildren();
}

static void Callback::varListChildren() {
	if(!sUpdateQueue.empty()) {
		Variable *v = sUpdateQueue.back();
		sUpdateQueue.pop();
		if(v->exp) {
			v->exp->update(Callback::varEECreate);
			return;
		}
	}

	oprintDone();
	oprintf(",numchild=\"%"PFZT"\",children=[", sVar->children.size());

	for(map<int, Variable>::iterator i = sVar->children.begin(); i!=sVar->children.end(); i++)
	{
		Variable& varc = i->second;
		if(i!=sVar->children.begin()) oprintf(",");
		oprintf("child=");
		printListChildrenItem(&varc);
	}
	oprintf("]\n");
	sVar = NULL;
	commandComplete();
}

void var_info_expression(const string& args) {
	Variable *var = getAndValidateVariable(args);
	if(!var) return;
	oprintDone();
	oprintf(",lang=\"C++\",exp=\"%s\"\n", var->localName.c_str());
	commandComplete();	
}

void var_info_path_expression(const string& args) {
	Variable *var = getAndValidateVariable(args);
	if(!var) return;
	oprintDone();
	oprintf(",lang=\"C++\",exp=\"%s\"\n", var->exp->getExprText().c_str());
	commandComplete();	

}
void var_info_num_children(const string& args) {
	Variable *var = getAndValidateVariable(args);
	if(!var) return;
	oprintDone();
	oprintf(",numchild=\"%"PFZT"\"\n", var->children.size());
	commandComplete();
}

void var_info_type(const string& args) {
	Variable *var = getAndValidateVariable(args);
	if(!var) return;
	oprintDone();
	oprintf(",type=\"%s\"\n", var->exp->type().c_str());
	commandComplete();
}

void varErrorFunction() {
	if(sVar) {

		if(sRootVariableMap.find(sVar->name) == sRootVariableMap.end() &&
			sVariableMap.find(sVar->name) == sVariableMap.end()) {
				delete sVar;
		}
		sVar = NULL;
	}
}
