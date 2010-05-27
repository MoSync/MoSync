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
#include <stdarg.h>

using namespace std;

#if defined (__MACH__) || defined(__APPLE__) 
#define PLATFORM_OSX
#endif

static const char sInfo[] =
" iphone-builder\n"
" author: Romain Chalant and Niklas Nummelin\n"
"\n"
" usage:\n"
" iphone-builder <mode> <mode-specific-options>\n"
"\n"
" modes:\n"
"   generate        generates an xcode project.\n"
"   build           builds an xcode project using xcode (mac only).\n"
"   run             runs an iphone application built using xcode (mac only).\n"
"\n"
" generate options:\n"
"   -pbx-template   specifies the input pbxproj template.\n"
"   -plist-template specifies the input plist template.\n"
"   -project-name   the name of the project.\n"
"   -company-name   the name of the company.\n"
"   -version        the version (used for knowing when to update on the app store).\n"
"   -output         specifies the output folder of the xcode project (the project name will be used as the name for the files).\n"
"\n"
" build options:\n"
"   -input          specifies the filename of the xcode project.\n"
"   -output         specifies where to put the output.\n"
"\n"
" run options:\n"
"   -input          specifies where the application is.\n"
"\n";

static map<string, string> sArguments;

void error(const char* fmt, ...) {
	va_list argptr;
	va_start(argptr, fmt);
	vprintf(fmt, argptr);
	va_end(argptr);

	exit(1);
}

const string& validateArgument(const string& arg) {
	map<string,string>::iterator i = sArguments.find(arg);
	if(i==sArguments.end()) error("Missing argument %s.\n", arg.c_str());
	return i->second;
}

// slow implementation, but fast enough.
void replaceTemplateDefine(string &templateFile, const string &whatToReplace, const string &replacement) {
	size_t index;
	while((index=templateFile.find(whatToReplace))!=string::npos) {
		size_t endOfReplacement = index+whatToReplace.length();
		templateFile = templateFile.substr(0, index) + replacement + templateFile.substr(endOfReplacement, templateFile.size()-endOfReplacement);
	}
}

void readFileIntoMem(string& output, const char* filename) {
	output.clear();
	FILE *file = fopen(filename, "rb");
	if(!file) error("Could not open file %s\n", filename);
	fseek(file, 0, SEEK_END);
	int length = ftell(file);
	fseek(file, 0, SEEK_SET);
	output.resize(length);
	fread(&output[0], length, 1, file);
	fclose(file);
}

void writeMemIntoFile(const char* filename, const char *mem, size_t len) {
	FILE *file = fopen(filename, "wb");
	if(!file) error("Could not open file %s\n", filename);
	fwrite(mem, len, 1, file);
	fclose(file);
}

string filterWhiteSpace(const string& str) {
	string newString = "";
	for(size_t i = 0; i < str.length(); i++) {
		unsigned char s = str[i];
		if(s > 32 && s < 127) {
			newString += str[i];
		} else {
			newString += "_";
		}
	}
	return newString;
}

void generate() {
	string pbxTemplateFile, plistTemplateFile;
	readFileIntoMem(pbxTemplateFile, validateArgument("pbx-template").c_str());
	readFileIntoMem(plistTemplateFile, validateArgument("plist-template").c_str());

	string outputFolder = validateArgument("output").c_str();
	string projectName = filterWhiteSpace(validateArgument("project-name"));

	string pbxOutput = outputFolder + "/" + projectName + ".pbxproj";
	string plistName = projectName + "-Info";
	string plistOutput = outputFolder + "/" + plistName + ".plist";

	replaceTemplateDefine(pbxTemplateFile, "__PROJECT_NAME__", projectName);
	replaceTemplateDefine(pbxTemplateFile, "__PLIST_NAME__", plistName);
	replaceTemplateDefine(plistTemplateFile, "__VERSION__", validateArgument("version"));
	replaceTemplateDefine(plistTemplateFile, "__COMPANY_NAME__", filterWhiteSpace(validateArgument("company")));

	writeMemIntoFile(pbxOutput.c_str(), pbxTemplateFile.c_str(), pbxTemplateFile.length());
	writeMemIntoFile(plistOutput.c_str(), plistTemplateFile.c_str(), plistTemplateFile.length());
}

#ifdef PLATFORM_OSX
void build() {
}

void run() {
}
#else
void build() {
	error("Incompatible platform for mode 'build'.\n");
}
void run() {
	error("Incompatible platform for mode 'run'.\n");
}
#endif


int main(int argc, char **argv) {
	if(argc<2) {
		error("%s", sInfo);
	}

	// parse arguments
	for(int i = 2; i < argc; i++) {
		if(argv[i][0] == '-') {
			i++;
			if(i>=argc) error("Missing value for option %s", argv[i-1]);
			sArguments[&argv[i-1][1]] = argv[i];
		} else {
			error("Invalid option %s", argv[i]);
		}
	}

	// get mode
	string mode;
	mode = argv[1];

	if(mode == "generate") {
		generate();
	} else if(mode == "build") {
		build();
	} else if(mode == "run") {
		run();
	}


}