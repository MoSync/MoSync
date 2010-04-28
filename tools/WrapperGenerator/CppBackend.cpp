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
		const Base* ret = func->getReturnType();
		string returnString = ret->toString();
		bool returnsHandle = false;
		bool hasHandlesAsArguments = false;
		if(ret->getBaseType() == Base::EPointerType) {
			const PointerType* pret = (const PointerType*)ret;
			returnsHandle = true;
		}

		vector<MoSyncArgument> arguments;

		const std::vector<const Argument*>& args = func->getArguments();
		for(int i = 0; i < args.size(); i++) {
			MoSyncArgument ma;
			ma.isHandle = false;

			const Base* type = args[i]->getType();
			if(type->getBaseType() == Base::ETypedef) type = ((const Typedef*)type)->resolveFully();
			if(type->getBaseType() == Base::EPointerType) {
				const PointerType* pt = (const PointerType*)type;
				if(!pt->isConst()) {
					hasHandlesAsArguments = true;
					ma.isHandle = true;
				}
			}
			ma.type = type;
			arguments.push_back(ma);
		}

		if(returnsHandle || hasHandlesAsArguments) {

			if(returnsHandle) stream << "MAHandle ";
			else {
				stream << ret->toString() << " ";	
			}

			stream << func->getName() << "Handle(";
			for(int i = 0; i < arguments.size(); i++) {
				if(arguments[i].isHandle) stream << "MAHandle h" << args[i]->getName();
				else stream << arguments[i].type->toString() << " " << args[i]->getName();
				if(i != args.size()-1) stream << ", ";
			}
			stream << ") {\n";

			if(returnsHandle) {
				stream << "\tMAHandle placeholder = maCreatePlaceholder();\n";
			}

			for(int i = 0; i < arguments.size(); i++) {
				if(arguments[i].isHandle) {
					string streamName = "s" + args[i]->getName();

					stream << "\tStream* " << streamName << " = " << "gSyscall->resources.get_RT_BINARY(" << "h" << args[i]->getName() << ");\n";
					string typeName = arguments[i].type->toString();
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
			for(int i = 0; i < arguments.size(); i++) {
				stream << args[i]->getName();
				if(i != args.size()-1) stream << ", ";
			}
			stream << ");\n";

			if(returnsHandle) {
				// MemStream(char* buf, size)
				stream << "\treturn SYSCALL_THIS->resources.add_RT_BINARY(placeholder, new MemStream(ret, " << "range" << "));\n";
			}


			stream << "}\n\n";
		}

	}

}	