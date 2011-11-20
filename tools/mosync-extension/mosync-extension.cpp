/* Copyright (C) 2011 MoSync AB

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

#include <stdio.h>
#include <vector>
#include <string>
#include <fstream>
#include <iomanip>
#include <string.h>
#include <stdlib.h>
#include <idl-common/idl-common.h>
#include <idl-common/stringFunctions.h>
#include <helpers/mkdir.h>

using namespace std;

static int generateConfig(const char* output, const char* idlFile);
static void outputExtensionHeader(const char* output, const Interface& inf);
static void outputExtensionSource(const char* output, const Interface& inf);

class Mode
{
	public:
		virtual const char* getName() = 0;
		virtual const char* getHelpText() = 0;
		virtual bool performTask(const vector<string>& arguments) = 0;
};

class BuildApiMode : public Mode
{
	public:
		const char* getName()
		{
			return "build-api";
		}

		const char* getHelpText()
		{
			return 	"Builds the header and source that contains the api for"
					"the programmer (The one that is called from MoSync source.)\n"
					"usage: mosync-extension build-api <output-folder> <input-idl>";
		}

		bool performTask(const vector<string>& arguments)
		{
			if(arguments.size() < 2)
				return false;

			return generateConfig(arguments[0].c_str(), arguments[1].c_str()) == 0;
		}
};

static vector<Mode*> sModes;

class HelpMode : public Mode
{
	public:
		const char* getName()
		{
			return "help";
		}

		const char* getHelpText()
		{
			return 	"Prints help about a specific mode.\n"
					"usage: mosync-extension help <mode>";
		}

		bool performTask(const vector<string>& arguments)
		{
			if(arguments.size() < 1)
				return false;

			string mode = arguments[0];

			for(size_t i = 0; i < sModes.size(); i++)
			{
				if(std::string(sModes[i]->getName()) == mode)
				{
					printf("%s\n", sModes[i]->getHelpText());
					return true;
				}
			}

			return false;
		}
};

class BuildWpMode : public Mode
{
	public:
		const char* getName()
		{
			return "build-wp";
		}

		const char* getHelpText()
		{
			return 	"Builds the base class for the windows phone runtime "
					"from which the extension should inherit."
					"usage: mosync-extension build-wp <output-folder> <input-idl>";
		}

		bool performTask(const vector<string>& arguments)
		{
			if(arguments.size() < 2)
				return false;

			vector<string> dummy;
			Interface* interface = new Interface(parseInterface(dummy, arguments[1]));
			string outputPath = (arguments[0]+"/wp");
			string outputFile = outputPath + "/" + interface->name + "Base.cs";
			_mkdir(outputPath.c_str());
			printf("outputs windows phone base to: %s\n", outputFile.c_str());
			ofstream stream(outputFile.c_str());
			stream << "class " << interface->name << "Base" << " : IExtensionModule\n";
			stream << "{\n";

			stream << "\tpublic String getName()\n";
			stream << "\t{\n";
			stream << "\t\treturn \"" << interface->name << "\";\n";
			stream << "\t}\n";
			stream << "}\n";

			flushStream(stream);
			return true;
		}
};


void initModes()
{
	sModes.clear();
	sModes.push_back(new BuildApiMode());
	sModes.push_back(new HelpMode());
	sModes.push_back(new BuildWpMode());
}

int main(int argc, const char** argv) {

	initModes();

	vector<string> arguments;

	// parse arguments
	for(int i=1; i<argc; i++) {
		arguments.push_back(argv[i]);
	}

	try {

	if(arguments.size() > 0) {

		string mode = arguments[0];
		arguments.erase(arguments.begin());
		for(size_t i = 0 ; i < sModes.size(); i++)
		{
			if(std::string(sModes[i]->getName()) == mode)
			{
				if(!sModes[i]->performTask(arguments))
				{
					printf("%s\n", sModes[i]->getHelpText());
					return 1;
				}

				return 0;
			}
		}

	}

	} catch(exception& e) {
		printf("Exception: %s\n", e.what());
	}

	printf("Usage: mosync-extension <mode> <options>+\n");
	printf("Example: mosync-extension build-api output test.idl\n");

	return 1;
}

static int generateConfig(const char* output, const char* idlFile) {
	vector<string> dummy;
	Interface* interface = new Interface(parseInterface(dummy, idlFile));

	outputExtensionHeader(output, *interface);
	outputExtensionSource(output, *interface);
	return 0;
}

string getExtensionHandleName(const Interface& inf, const Function& function)
{
	return "MAExtensionFunction_" + inf.name + "_" + function.name;
}

string getExtensionModuleHandleName(const Interface& inf)
{
	return "MAExtensionModule_" + inf.name;
}

static void outputExtensionHeader(const char* output, const Interface& inf) {
	ofstream stream((string(output)+"/"+inf.name+".h").c_str());
	string headerName = "MA_EXTENSION_" + inf.name;

	stream << "#ifndef " << headerName << "_H\n";
	stream << "#define " << headerName << "_H\n\n";

	if(inf.comment.size() != 0) {
		stream << inf.comment << "\n";
	}

	stream << "#include <ma.h>\n\n";

	stream << "#define MA_EXTENSION_" << inf.name << "_IDL_HASH ((int)0x" << setfill('0') << setw(8) <<
		hex << calculateChecksum(inf) << dec << ")\n\n";

	stream << "#define MA_EXTENSION_" << inf.name << "_ID \"" << inf.name << "\"\n\n";

	stream << "#ifdef __cplusplus\n"
		"extern \"C\" {\n"
		"#endif\n\n";

	streamCppDefs(stream, inf, 0, headerName);

	stream << "\n";

	stream << "void MAExtension_" << inf.name << "_Init();\n";
	stream << "MAExtensionModule MAExtension_" << inf.name << "_GetModule();\n";

	for(size_t i=0; i<inf.functions.size(); i++) {
		stream << "extern MAExtensionFunction " << getExtensionHandleName(inf, inf.functions[i]) << ";\n";
	}

	stream << "\n";

	for(size_t i=0; i<inf.functions.size(); i++) {
		streamIoctlFunction(stream, inf, inf.functions[i], "maExtensionFunctionInvoke", getExtensionHandleName(inf, inf.functions[i]));
	}

	stream << "#ifdef __cplusplus\n"
		"}\n"
		"#endif\n\n";

	stream << "#endif	//" + headerName + "_H\n";
	flushStream(stream);
}

static void outputExtensionSource(const char* output, const Interface& inf) {
	ofstream stream((string(output)+"/"+inf.name+".c").c_str());
	string headerName = "MA_EXTENSION_" + inf.name;

	stream << "#include <" << inf.name << ".h>\n\n";

	stream << "MAExtensionModule " << getExtensionModuleHandleName(inf) << " = MA_EXTENSION_MODULE_UNAVAILABLE;\n";
	for(size_t i=0; i<inf.functions.size(); i++) {
		stream << "MAExtensionFunction " << getExtensionHandleName(inf, inf.functions[i]) << " = MA_EXTENSION_FUNCTION_UNAVAILABLE;\n";
	}

	stream << "\n";

	stream << "void MAExtension_" << inf.name << "_Init() {\n";

	stream << "\t" << getExtensionModuleHandleName(inf) << " = " <<
		"maExtensionModuleLoad(MA_EXTENSION_" << inf.name << "_ID, MA_EXTENSION_" << inf.name << "_IDL_HASH);\n";

	for(size_t i=0; i<inf.functions.size(); i++) {
		stream << "\t" << getExtensionHandleName(inf, inf.functions[i]) << " = maExtensionFunctionLoad(" <<
				getExtensionModuleHandleName(inf) << ", " << toString(inf.functions[i].number) << ");\n";
	}

	stream << "}\n";

	stream << "MAExtensionModule MAExtension_" << inf.name << "_GetModule() {\n";
	stream << "\tif(" << getExtensionModuleHandleName(inf) << " == MA_EXTENSION_MODULE_UNAVAILABLE)\n";
	stream << "\t\tMAExtension_" << inf.name << "_Init();\n";
	stream << "\treturn " << getExtensionModuleHandleName(inf) << ";\n";
	stream << "}\n\n";

	flushStream(stream);
}

void streamInvokePrefix(ostream& stream, const Function& f) {
}
