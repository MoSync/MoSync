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
#include <stack>
#include <cstdio>
#include <cstdarg>
#include <cstring>
#include <cstdlib>
#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/stat.h>


#include <helpers/attribute.h>
#include <helpers/mkdir.h>
#include <filelist/filelist.h>


using namespace std;

#if defined (__MACH__) || defined(__APPLE__) 
#define PLATFORM_OSX
#define ONLY_ON_OSX
#else
#define ONLY_ON_OSX GCCATTRIB(noreturn)
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
"   -cert           the name of the certificate to use for signing the app.\n"
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

static void error(const char* fmt, ...) PRINTF_ATTRIB(1, 2) GCCATTRIB(noreturn);

#define ERR_COPY_SRC_COULD_NOT_BE_OPENED -1
#define ERR_COPY_DST_COULD_NOT_BE_OPENED -2
#define ERR_COPY_READ_FAILED -3
#define ERR_COPY_WRITE_FAILED -4

int copyFile ( const char *src, const char* dst );
void copyFilesRecurse ( void );
void copyFilesCallback ( const char *path );
void copyFilesRecursively( const char *src_dir, const char *dst_dir );
void error(const char* fmt, ...);
const string& validateArgument(const string& arg);
void replaceTemplateDefine(string &templateFile, const string &whatToReplace, const string &replacement);
void readFileIntoMem(string& output, const char* filename);
string filterWhiteSpace(const string& str);
void generate();
void writeMemIntoFile(const char* filename, const char *mem, size_t len);
void build(void) ONLY_ON_OSX;
void run(void) ONLY_ON_OSX;


// returns total amount of bytes written.
// on failure, returns an error code, and errno will be set.
int copyFile(const char *src, const char* dst) {
	char buffer[1024];
	size_t bytesRead, bytesWritten;
	size_t bufferIndex = 0;	
	int res = 0;
	FILE *fd_src, *fd_dst;

	fd_src = fopen(src, "rb");
	if(fd_src==NULL) return ERR_COPY_SRC_COULD_NOT_BE_OPENED;
	fd_dst = fopen(dst,"wb");
	if(fd_dst==NULL) {
		fclose(fd_src);
		return ERR_COPY_DST_COULD_NOT_BE_OPENED;
	}

	int eof = 0;
	while(!eof) {
		if((bytesRead = fread(buffer, 1, 1024, fd_src)) != 1024) {
			if(!feof(fd_src)) {
				res = ERR_COPY_READ_FAILED; 
				break;
			} else {
				eof = 1;
			}
		}
		if((bytesWritten=fwrite(&buffer[bufferIndex], 1, bytesRead, fd_dst)) != bytesRead) {
			res = ERR_COPY_WRITE_FAILED;
			break;
		}
		res += (int)bytesWritten;
	}

	fclose(fd_src);
	fclose(fd_dst);
	return res;
}

static stack<string> sDestinationDirectory;
static stack<string> sSourceDirectory;

void copyFilesRecurse();

void copyFilesCallback(const char *path) {
	if(path[0] == '.') return;
	const char* slash = strrchr(path, '/');
	const char *fn;
	if(slash)
		fn = slash + 1;
	else fn = path;

	string p = sSourceDirectory.top() + string("/") + fn;
	path = p.c_str();

    int isDir = isDirectory(path);
    if(isDir < 0) {
        error("Could not scan source file/dir %s\n", path);
    } else if(isDir == 1) {
		sDestinationDirectory.push(sDestinationDirectory.top() + string("/") + fn);
#ifdef WIN32
		sSourceDirectory.push(sSourceDirectory.top() + string("/") + fn);
#else
		sSourceDirectory.push(path);
#endif

		copyFilesRecurse();
	} else {
		string srcFile = path;		
		string dstFile = sDestinationDirectory.top() + string("/") + fn;
		int errCode;
		if((errCode = copyFile(srcFile.c_str(), dstFile.c_str()))<0) {
			
			error("Could not copy file %s. Error code: %d, errno: %i (%s)\n", path, errCode, errno, strerror(errno));
		}
	}
}

void copyFilesRecurse() {
	_mkdir(sDestinationDirectory.top().c_str());
	printf("src: %s\n", sSourceDirectory.top().c_str());
	printf("dst: %s\n", sDestinationDirectory.top().c_str());

	if(scanDirectory((sSourceDirectory.top()+"/*").c_str(), copyFilesCallback) != 0) 
		error("Could not scan source directory.\n");
	sSourceDirectory.pop();
	sDestinationDirectory.pop();
}


void copyFilesRecursively(const char *src_dir, const char *dst_dir) {
	sSourceDirectory.push(src_dir);
	sDestinationDirectory.push(dst_dir);
	copyFilesRecurse();
}

static void error(const char* fmt, ...) {
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
	if (whatToReplace != replacement) {
		while((index=templateFile.find(whatToReplace))!=string::npos) {
			size_t endOfReplacement = index+whatToReplace.length();
			templateFile = templateFile.substr(0, index) + replacement + templateFile.substr(endOfReplacement, templateFile.size()-endOfReplacement);
		}
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
	int res = fread(&output[0], length, 1, file);
	if(res != 1) error("Could not read file %s\n", filename);
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
			newString += "";
		}
	}
	return newString;
}

void generate() {
	string pbxTemplateFile, plistTemplateFile;

	string inputFolder = validateArgument("input");
	string outputFolder = validateArgument("output");
	
	copyFilesRecursively(inputFolder.c_str(), outputFolder.c_str());

	string pbxInput = outputFolder + string("/") + "project.pbxprojtemplate";
	string plistInput = outputFolder + string("/") + "project.plisttemplate";

	readFileIntoMem(pbxTemplateFile, pbxInput.c_str());
	readFileIntoMem(plistTemplateFile, plistInput.c_str());

	string projectName = filterWhiteSpace(validateArgument("project-name"));

	string xcodeProjectPath = outputFolder + "/" + projectName + ".xcodeproj";
	string pbxOutput = xcodeProjectPath + "/" + "project.pbxproj";
	
	_mkdir(xcodeProjectPath.c_str());	

	string plistOutput = outputFolder + "/" + projectName + ".plist";

	replaceTemplateDefine(pbxTemplateFile, "__PROJECT_NAME__", projectName);
	replaceTemplateDefine(pbxTemplateFile, "iPhone Developer", validateArgument("cert"));
	replaceTemplateDefine(plistTemplateFile, "__VERSION__", validateArgument("version"));
	replaceTemplateDefine(plistTemplateFile, "__COMPANY_NAME__", filterWhiteSpace(validateArgument("company-name")));

	writeMemIntoFile(pbxOutput.c_str(), pbxTemplateFile.c_str(), pbxTemplateFile.length());
	writeMemIntoFile(plistOutput.c_str(), plistTemplateFile.c_str(), plistTemplateFile.length());
}

#ifdef PLATFORM_OSX
void build() {
	execl("/Developer/usr/bin/xcodebuild", "/Developer/usr/bin/xcodebuild", "-project",
		validateArgument("input").c_str(), NULL, (char*) 0);
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
			if(i>=argc) error("Missing value for option %s.\n", argv[i-1]);
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
