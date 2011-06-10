#include <stdio.h>
#include <vector>
#include <string>
#include <fstream>
#include <iomanip>

#include <idl-common/idl-common.h>

using namespace std;

int generateInvoker(const char* input, const char* output);
void outputInvoker(const char* output, const Interface&);
void outputNativeHeader(const char* output, const Interface&);

int main(int argc, const char** argv) {
	// parse arguments
	const char* input;
	const char* output;

	if(input && output) {
		return generateInvoker(input, output);
	} else {
		printf("Usage: mx-invoker -i <input.idl> -o <output directory>\n");
		return 1;
	}
}

int generateInvoker(const char* input, const char* output) {
	vector<string> dummy;
	try {
		Interface inf = parseInterface(dummy, input);
		outputInvoker(output, inf);
		outputNativeHeader(output, inf);
	} catch(exception& e) {
		printf("Exception: %s\n", e.what());
		return 1;
	}
}

void outputInvoker(const char* output, const Interface& inf) {
	ofstream stream((string(output)+"/invoke-extension.cpp").c_str());
	stream << "#include \"invoke-extension.h\"\t//found in /mosync/ext-include/\n"
		"#ínclude \"cpp_mx_"<<inf.name<<".h\"\n"
		"\n";
	streamInvokeSyscall(stream, inf, false);
	stream << "\n"
		"static VoidFunction sFunctions[] = {\n";
	for(size_t i=0; i<inf.functions.size(); i++) {
		const Function& f(inf.functions[i]);
		stream << "\tinvoke_" << f.name << ",\n";
	}
	stream << "};\n"
		"\n"
		"VoidFunction* DLLEXPORT getFunctions(int* numFunctions) {\n"
		"\t*numFunctions = sizeof(sFunctions) / sizeof(*sFunctions);\n"
		"\treturn sFunctions;\n"
		"}\n"
		"\n"
		"int getIdlHash() {\n"
		"\t return "<< setfill('0') << setw(8) <<	hex << calculateChecksum(inf)<<";\n"
		"}\n";
}

void streamInvokePrefix(ostream& stream, const Function& f) {
	stream << "static void invoke_"<<f.name<<"() ";
}

void outputNativeHeader(const char* output, const Interface& inf) {
	ofstream stream((string(output)+"/cpp_mx_"+inf.name+".h").c_str());
}
