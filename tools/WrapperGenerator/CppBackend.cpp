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

#include "CPPBackend.h"
#include <vector>
using namespace std;

struct MoSyncArgument {
	bool isHandle;
	const Base* type;
};

void CPPBackend::emit(const BasesMap& bases, fstream& stream) {
	pair<BasesIterator, BasesIterator> functions = bases.equal_range("Function");	

	for(BasesIterator function = functions.first; function!=functions.second; function++) {
		const Function* func = (const Function*)function->second;
		string name = func->getName();
		
		/**
		 * Never versions of gcc includes some builtin functions,
		 * ignore these.
		 */
		if(name.find_first_of("__builtin") == 0)
		{
			continue;
		}
		
		const Base* ret = func->getReturnType();
		string returnString = ret->toString();
		bool returnsHandle = false;
		bool returnsConstPtr = false;
		if(ret->getBaseType() == Base::EPointerType) {
			const PointerType* pret = (const PointerType*)ret;
			returnsHandle = true;
			returnsConstPtr = pret->isConst();
		}

		//vector<MoSyncArgument> arguments;

		const std::vector<const Argument*>& args = func->getArguments();
		
		/*
		for(int i = 0; i < args.size(); i++) {
			MoSyncArgument ma;
			ma.isHandle = false;

			if(args[i]->isPointer()) {
				const PointerType* pt = (const PointerType*)args[i]->getType();
				if(!pt->isConst()) {
					hasHandlesAsArguments = true;
					ma.isHandle = true;
				}
			}
			ma.type = args[i]->getType()->resolveFully();
			arguments.push_back(ma);
		}
		*/

		if(returnsHandle || func->hasHandleArguments()) {

			stream << "static ";

			if(returnsHandle) stream << "MAHandle ";
			else {
				stream << ret->toString() << " ";	
			}

			stream << func->getName() << "Handle(";
			for(size_t i = 0; i < args.size(); i++) {
				if(args[i]->usesHandle()) stream << "MAHandle h" << args[i]->getName();
				else stream << args[i]->getType()->resolveFully()->toString() << " " << args[i]->getName();
				if(i != args.size()-1) stream << ", ";
			}
			stream << ") {\n";

			if(returnsHandle) {
				stream << "\tMAHandle placeholder = maCreatePlaceholder();\n";
			}

			for(size_t i = 0; i < args.size(); i++) {
				if(args[i]->usesHandle()) {
					string streamName = "s" + args[i]->getName();

					stream << "\tStream* " << streamName << " = " << "gSyscall->resources.get_RT_BINARY(" << "h" << args[i]->getName() << ");\n";
					string typeName = args[i]->getType()->resolveFully()->toString();
					stream << "\t" << typeName << " " << args[i]->getName() << " = (" << typeName << ") " << streamName << ".ptr();\n";
				}
			}


			stream << "\t";

			if(!returnsHandle && (ret->toString()!="void")) {
				stream << "return ";
			} else {
				if(returnsHandle)
					stream << ret->toString() << " ret = ";
			}

			stream << func->getName() << "(";
			for(size_t i = 0; i < args.size(); i++) {
				stream << args[i]->getName();
				if(i != args.size()-1) stream << ", ";
			}
			stream << ");\n";

			if(returnsHandle) {
				if( func->getRangeExpression() == "") System::error("Return type is of pointer type, needs range attribute");
				if(!returnsConstPtr)
					System::error("Returns non-const pointer. Can't handle yet!");
				stream << "\treturn SYSCALL_THIS->resources.add_RT_BINARY(placeholder, new MemStreamC(ret, " << func->getRangeExpression() << "));\n";
			}


			stream << "}\n\n";
		}

	}

}	