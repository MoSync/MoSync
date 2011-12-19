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
#include <map>
#include <fstream>
#include <iomanip>
#include <string.h>
#include <stdlib.h>
#include <idl-common/idl-common.h>
#include <idl-common/stringFunctions.h>
#include <helpers/mkdir.h>
#include <filelist/copyfiles.h>
#include <filelist/filelist.h>

using namespace std;

static int generateConfig(const char* output, const char* idlFile);
static void outputExtensionHeader(const char* output, const Interface& inf);
static void outputExtensionSource(const char* output, const Interface& inf);
static bool performTask(const std::string mode, const vector<std::string>& arguments);


static std::map<std::string, Interface*> sInterfaces;
Interface* parseOrGetCachedInterface(const std::string& filename)
{
	map<string, Interface*>::iterator iter;
	if((iter = sInterfaces.find(filename)) == sInterfaces.end())
	{
		vector<string> dummy;
		return sInterfaces[filename] = new Interface(parseInterface(dummy, filename));
	}

	return iter->second;
}

const char* mosyncdir() {
	static const char* md = NULL;
	if(!md) {
		md	= getenv("MOSYNCDIR");
		if(!md) {
			printf("MOSYNCDIR is not set!\n");
			exit(1);
		}
	}
	return md;
}


void sh(const char* cmd, bool hideOutput, const char* shownCmdLine) {
	if (!shownCmdLine) {
		printf("%s\n", cmd);
	} else {
		printf("%s\n", shownCmdLine);
	}

	fflush(stdout);
	fflush(stderr);

	std::string finalCmd(cmd);
	if(hideOutput) {
		// Supress output from stdout and stderr
#ifdef WIN32
		finalCmd += "> nul 2>&1";
#else
		finalCmd += "> /dev/null 2>&1";
#endif
	}

	int res = system(finalCmd.c_str());

	fflush(stdout);
	fflush(stderr);
	if(res != 0) {
		printf("System error %i\n", res);
		exit(res);
	}
}

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


			_mkdir(arguments[0].c_str());
			std::string apiOutput = arguments[0] + "/mosync/lib";
			_mkdir((arguments[0] + "/mosync").c_str());
			_mkdir(apiOutput.c_str());

			return generateConfig(apiOutput.c_str(), arguments[1].c_str()) == 0;
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

class TasksMode : public Mode
{
	public:
		const char* getName()
		{
			return "tasks";
		}

		const char* getHelpText()
		{
			return 	"Prints available tasks.";
		}

		bool performTask(const vector<string>& arguments)
		{
			for(size_t i = 0; i < sModes.size(); i++)
			{
				printf("%s\n", sModes[i]->getName());
			}

			return true;
		}
};

class BuildWpDevProjectMode : public Mode
{
	public:
		const char* getName()
		{
			return "build-wp-dev-project";
		}

		const char* getHelpText()
		{
			return 	"Builds the initial project for developing a windows phone extension.\n"
					"Also builds bindings once (equivalent to running using mode build-wp-bindings).\n"
					"This should only be used once when you start creating an implementation for\n"
					"windows phone 7.\n"
					"usage: mosync-extension build-wp-dev-project <output-folder> <input-idl>";
		}

		bool performTask(const vector<string>& arguments)
		{
			if(arguments.size() < 2)
				return false;

			vector<string> dummy;
			Interface* interface = parseOrGetCachedInterface(arguments[1]); //new Interface(parseInterface(dummy, arguments[1]));
			string className = interface->name;

			_mkdir(arguments[0].c_str());

			string outputPath = (arguments[0]+"/wp/src");

			_mkdir((arguments[0]+"/wp").c_str());
			_mkdir(outputPath.c_str());

			std::string extensionProjDir = std::string(mosyncdir()) + "/profiles/runtimes/winphone/1/mosyncExtensionTemplate";
			std::string extensionProjDst = outputPath + "/extension";
			std::string extensionCSProjFileSrc = extensionProjDir + "/mosyncExtensionTemplate.csproj";
			std::string extensionCSProjFileDst = extensionProjDst + "/mosyncExtensionTemplate.csproj";

			std::string mosyncProjDir = std::string(mosyncdir()) + "/profiles/runtimes/winphone/1/template";
			std::string mosyncProjDst = outputPath + "/mosync";
			std::string mosyncProjFileSrc = mosyncProjDir + "/mosync.csproj";
			std::string mosyncProjFileDst = mosyncProjDst + "/mosync.csproj";

			// copy extension library template and application
			copyFilesRecursively(extensionProjDir.c_str(), extensionProjDst.c_str());
			copyFilesRecursively(mosyncProjDir.c_str(), mosyncProjDst.c_str());


			std::string cmd =
				"winphone-builder -build-extension-devenv"
				" -runtime-path ..\\mosync\\Libraries\\mosyncRuntime\\mosyncRuntime.dll"
				" -project-name " + className +
				" -input-file " + extensionCSProjFileSrc +
				" -output-file " + extensionCSProjFileDst;

			sh(cmd.c_str(), false, NULL);

			cmd =
				"winphone-builder"
				" -output-type interpreted"
				" -input-file " + mosyncProjFileSrc +
				" -output-file " + mosyncProjFileDst +
				" -ref " + className + " ..\\..\\lib\\Bin\\Debug\\" + className + ".dll";

			sh(cmd.c_str(), false, NULL);


			string extLoaderPath = mosyncProjDst + "/MoSyncExtensionsLoader.cs";
			ofstream extLoaderSrc(extLoaderPath.c_str());

			extLoaderSrc << "namespace MoSync\n";
			extLoaderSrc << "{\n";
			extLoaderSrc << "\tpublic static class ExtensionsLoader\n";
			extLoaderSrc << "\t{\n";
			extLoaderSrc << "\t\tpublic static void Load()\n";
			extLoaderSrc << "\t\t{\n";
			extLoaderSrc << "\t\t\tSystem.Reflection.Assembly.Load(\"" << className << "\");\n";
			extLoaderSrc << "\t\t}\n";
			extLoaderSrc << "\t}\n";
			extLoaderSrc << "}\n";

			if(!::performTask("build-wp-bindings", arguments))
				return false;
			return ::performTask("build-wp-impl-template", arguments);
		}
};

class BuildWpBindingsMode : public Mode
{
	public:
		const char* getName()
		{
			return "build-wp-bindings";
		}

		const char* getHelpText()
		{
			return 	"Builds the bindings for the windows phone runtime. "
					"If the interface to the extension has changed this needs to be used.\n"
					"Note that build-wp-dev-project builds these initially.\n"
					"usage: mosync-extension build-wp-bindings <output-folder> <input-idl>";
		}

		bool performTask(const vector<string>& arguments)
		{
			if(arguments.size() < 2)
				return false;

			vector<string> dummy;
			Interface* interface = parseOrGetCachedInterface(arguments[1]); //new Interface(parseInterface(dummy, arguments[1]));
			string className = interface->name;

			_mkdir(arguments[0].c_str());

			string outputPath = (arguments[0]+"/wp/src/extension");
			string outputFile = outputPath + "/MoSyncExtensionGenerated.cs";

			_mkdir((arguments[0]+"/wp/src").c_str());
			_mkdir(outputPath.c_str());
			printf("outputs windows phone base to: %s\n", outputFile.c_str());
			ofstream stream(outputFile.c_str());

			stream << "// note: this file is generated, do not modify.\n";
			stream << "using MoSync;\n";
			stream << "using System;\n";
			stream << "public partial class " << className << " : IExtensionModule\n";
			stream << "{\n";

			streamCSharpFunctionDelegates(stream, *interface);
			stream << "\n";

			stream << "\tpublic String GetName()\n";
			stream << "\t{\n";
			stream << "\t\treturn \"" << interface->name << "\";\n";
			stream << "\t}\n\n";

			stream << "\tpublic uint GetHash()\n";
			stream << "\t{\n";
			stream << "\t\treturn 0x" << setfill('0') << setw(8) << hex << calculateChecksum(*interface) << dec << ";\n";
			stream << "\t}\n\n";

			stream << "\tpublic long Invoke(MoSync.Core core, int id, int a, int b, int c)\n";
			stream << "\t{\n";
			stream << "\t\tlong result = MoSync.Constants.MA_EXTENSION_FUNCTION_UNAVAILABLE;\n";
			stream << "\t\tswitch(id)\n";
			stream << "\t\t{\n";
			for(size_t i = 0; i < interface->functions.size(); i++)
			{
				streamIoctlFunctionInvokeCSharp(stream, interface->functions[i], *interface, "", "core.");
			}
			stream << "\t\t}\n";
			stream << "\t\treturn result;\n";
			stream << "\t}\n";
			stream << "}\n";

			flushStream(stream);
			return true;
		}
};

class ListInstalledMode : public Mode
{
	public:
		const char* getName()
		{
			return "list-installed";
		}

		const char* getHelpText()
		{
			return 	"List all of the installed extensions and platform availability.\n"
					"usage: mosync-extension list-installed\n";
		}

		static std::vector<std::string> sCurrentExtensions;
		static std::vector<std::string> sCurrentPlatforms;

		static void extensionsListing(const char* filename)
		{
			std::string fn = std::string(filename);

			if(isDirectory(filename) && fn != "." && fn != "..")
			{
				sCurrentExtensions.push_back(std::string(filename));
			}
		}


		static void platformsListing(const char* filename)
		{
			std::string fn = std::string(filename);

			if(isDirectory(filename) && fn != "." && fn != ".." && fn != "mosync")
			{
				sCurrentPlatforms.push_back(std::string(filename));
			}
		}


		bool performTask(const vector<string>& arguments)
		{
			std::string extensionsDir = std::string(mosyncdir()) + "/extensions";

			sCurrentExtensions.clear();
			scanDirectory((extensionsDir + "/*").c_str(), extensionsListing);

			for(size_t i = 0 ; i < sCurrentExtensions.size(); i++)
			{
				printf("%s ", sCurrentExtensions[i].c_str());

				sCurrentPlatforms.clear();
				scanDirectory((extensionsDir + "/" + sCurrentExtensions[i] + "/*").c_str(), platformsListing);

				for(size_t j = 0; j < sCurrentPlatforms.size(); j++)
				{
					if(j != 0)
						printf(" ");
					printf("%s", sCurrentPlatforms[j].c_str());
				}

				printf("\n");
			}

			return true;
		}
};

std::vector<std::string> ListInstalledMode::sCurrentExtensions;
std::vector<std::string> ListInstalledMode::sCurrentPlatforms;

class BuildWpImplTemplateMode : public Mode
{
	public:
		const char* getName()
		{
			return "build-wp-impl-template";
		}

		const char* getHelpText()
		{
			return 	"Builds a temlate for the implementations."
					"Should only be run once, it will overwrite the file called MoSyncExtension.cs.\n"
					"Note that build-wp-dev-project builds these initially.\n"
					"usage: mosync-extension build-wp-impl-template <output-folder> <input-idl>";
		}

		bool performTask(const vector<string>& arguments)
		{
			if(arguments.size() < 2)
				return false;

			vector<string> dummy;
			Interface* interface = parseOrGetCachedInterface(arguments[1]); //new Interface(parseInterface(dummy, arguments[1]));
			string className = interface->name;

			_mkdir(arguments[0].c_str());

			_mkdir((arguments[0]+"/wp/src").c_str());
			string outputPath = (arguments[0]+"/wp/src/extension");

			_mkdir(outputPath.c_str());

			string outputFile = outputPath + "/MoSyncExtension.cs";

			printf("outputs windows phone template impl to: %s\n", outputFile.c_str());
			ofstream stream(outputFile.c_str());

			stream << "using MoSync;\n";
			stream << "using System;\n";
			stream << "public partial class " << className << " : IExtensionModule\n";
			stream << "{\n";
			stream << "\tpublic void Init(Core core, Runtime runtime)\n";
			stream << "\t{\n";

			Interface& maapi = *interface;
			for(size_t j=0; j<maapi.functions.size(); j++) {
				const Function& f(maapi.functions[j]);

				//maapiFile << "\t\tcase " << f.number << ":\n";
				std::string returnType = getCSharpType(maapi, f.returnType, true);
				stream << "\t\t" << f.name << " = delegate(";
				for(size_t k=0; k<f.args.size(); k++) {
					const Argument& a(f.args[k]);
					if(k != 0)
						stream << ", ";

					std::string argType = getCSharpType(maapi, a.type, a.in);
					stream << argType << " _" << a.name;
				}
				stream << ")\n";
				stream << "\t\t{\n";
				stream << "\t\t\t// todo: add implementation.\n";
				stream << "\t\t\treturn MoSync.Constants.MA_EXTENSION_FUNCTION_UNAVAILABLE;\n";
				stream << "\t\t};\n";
			}

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
	sModes.push_back(new TasksMode());
	sModes.push_back(new BuildWpDevProjectMode());
	sModes.push_back(new BuildWpBindingsMode());
	sModes.push_back(new BuildWpImplTemplateMode());
	sModes.push_back(new ListInstalledMode());
}


static bool performTask(const std::string mode, const vector<std::string>& arguments)
{
	for(size_t i = 0 ; i < sModes.size(); i++)
	{
		if(std::string(sModes[i]->getName()) == mode)
		{
			if(!sModes[i]->performTask(arguments))
			{
				printf("%s\n", sModes[i]->getHelpText());
				return false;
			}

			return true;
		}
	}

	return false;
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

		if(performTask(mode, arguments))
		{
			return 0;
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
	Interface* interface = parseOrGetCachedInterface(idlFile); //new Interface(parseInterface(dummy, idlFile));

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

	stream << "#include \"" << inf.name << ".h\"\n\n";

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
