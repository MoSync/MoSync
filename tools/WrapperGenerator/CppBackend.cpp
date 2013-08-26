/* Copyright 2013 David Axmark

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

	http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
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
				stream << "\tSYSCALL_THIS->resources.add_RT_BINARY(placeholder, new MemStreamC(ret, " << func->getRangeExpression() << "));\n";
				stream << "\treturn placeholder;\n";
			}


			stream << "}\n\n";
		}

	}

}
