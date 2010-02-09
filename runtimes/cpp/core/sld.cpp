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

#include <stdio.h>
#include <set>
//#include <functional>

#define CONFIG_H	//HACK
#define LOGGING_ENABLED
#include <helpers/helpers.h>
#include <helpers/intutil.h>

#include "sld.h"

#define BUFSIZE 1024

using namespace std;

#define String std::string
#define Vector std::vector

#ifndef _MSC_VER
#define stricmp strcasecmp
#endif 

struct linemap_file_line_less
//	: public std::binary_function<LineMapping, LineMapping, bool>
{	// functor for operator<
	bool operator()(const LineMapping& _Left, const LineMapping& _Right) const
	{	// apply operator< to operands
		if(_Left.file != _Right.file)
			return _Left.file < _Right.file;
		else
			return _Left.line < _Right.line;
	}
};

struct funcmap_start_less
//	: public std::binary_function<FuncMapping*, FuncMapping*, bool>
{
	bool operator()(const FuncMapping* const& l, const FuncMapping* const& r) const
	{
		return l->start < r->start;
	}
};

struct funcmap_name_less
//	: public std::binary_function<FuncMapping*, FuncMapping*, bool>
{
	bool operator()(const FuncMapping* const& l, const FuncMapping* const& r) const
	{
		return l->name < r->name;
	}
};

static Vector<FileMapping> gFiles;

static std::set<LineMapping> sLineSet;	//maps addresses to lines

//maps lines to addresses.
//multiple addresses may map to the same line.
//this map will only contain the first.
typedef std::multiset<LineMapping, linemap_file_line_less> AddressSet;
static AddressSet gAddressSet;

typedef set<FuncMapping*, funcmap_start_less> FuncMapAddr;
static FuncMapAddr sFuncMapAddr;

//should probably be a hash set, for fastest lookup
typedef set<FuncMapping*, funcmap_name_less> FuncMapName;
static FuncMapName sFuncMapName;

struct VarMapping {
	int scope;
	int start;
	String name;
};
//TODO: make into a set, for faster lookup.
Vector<VarMapping> gVarMap;


class File {
public:
	File(const char* filename) : file(fopen(filename, "r")) {}
	~File() {
		if(file)
			fclose(file);
	}
	FILE* file;
};

//returns true on success
static bool readLine(char* dst, int maxLen, File& file) {
	if(!file.file)
		return false;
	char* res = fgets(dst, maxLen-1, file.file);
	if(res == NULL)
		return false;
	int end = strlen(res) - 1;
	if(res[end] == '\n')
		res[end] = 0;
	return true;
}

const FuncMapping* mapFunctionEx(int ip) {
	FuncMapping temp;
	temp.start = ip;
	FuncMapAddr::const_iterator itr = sFuncMapAddr.lower_bound(&temp);
	if(itr == sFuncMapAddr.end())
		return NULL;
	if((*itr)->start > ip) {
		if(itr == sFuncMapAddr.begin())
			return NULL;
		itr--;
	}
	DEBUG_ASSERT((*itr)->start <= ip);
	if((*itr)->stop >= ip)
		return *itr;
	else
		return NULL;
}

const char* mapFunction(int ip) {
	const FuncMapping* fm = mapFunctionEx(ip);
	if(fm == NULL)
		return NULL;
	return fm->name.c_str();
}

int mapFunctionStart(int ip) {
	const FuncMapping* fm = mapFunctionEx(ip);
	if(fm == NULL)
		return -1;
	return fm->start;
}

#if 0
int mapFunction(const char* name) {
	for(size_t i=0; i<gFuncMap.size(); i++) {
		FuncMapping& fm(gFuncMap[i]);
		if(strcmp(name, fm.name.c_str()) == 0)
			return fm.start;
	}
	return -1;
}
#endif
int mapFunction(const char* name) {
	FuncMapping temp;
	temp.name = name;
	FuncMapName::const_iterator itr = sFuncMapName.find(&temp);
	if(itr == sFuncMapName.end())
		return -1;
	else
		return (*itr)->start;
}

int mapVariable(const char* name, int scope) {
	for(size_t i=0; i<gVarMap.size(); i++) {
		VarMapping& vm(gVarMap[i]);
		if(scope == vm.scope && strcmp(name, vm.name.c_str()) == 0)
			return vm.start;
	}
	return -1;
}

int nextSldEntry(int address) {
	LineMapping key;
	key.ip = address;
	set<LineMapping>::iterator itr = sLineSet.find(key);
	if(itr == sLineSet.end())
		return -1;
	itr++;
	if(itr == sLineSet.end())
		return -1;
	return itr->ip;
}

void clearSLD() {
	FuncMapAddr::iterator itr = sFuncMapAddr.begin();
	while(itr != sFuncMapAddr.end()) {
		delete *itr;
		itr++;
	}
	sFuncMapAddr.clear();
	sFuncMapName.clear();
	sLineSet.clear();
	gAddressSet.clear();
	gFiles.clear();
}

static int fileIndexFromScope(int scope) {
	for(size_t i=0; i<gFiles.size(); i++) {
		if(gFiles[i].scope == scope)
			return i;
	}
	return -1;
}

bool loadSLD(const char* filename) {
	clearSLD();

	File file(filename);
	char buffer[BUFSIZE];

	//read files
	int lastIndex = 0;
	TEST(readLine(buffer, BUFSIZE, file));
	FAILIF(strcmp(buffer, "Files") != 0);
	while(1) {
		TEST(readLine(buffer, BUFSIZE, file));

		FileMapping fm;
		int index, nameStartPoint;
		if(sscanf(buffer, "%i:%i%n", &index, &fm.scope, &nameStartPoint) != 2)
			break;
		if(buffer[nameStartPoint] != ':')
			break;
		nameStartPoint++;
		lastIndex++;
		index++;
		if(index != lastIndex)
			return 1;
		fm.name = buffer + nameStartPoint;

		//transform to unix-style paths for easy handling in the rest of the program.
		for(size_t i=0; i<fm.name.size(); i++) {
			if(fm.name[i] == '\\')
				fm.name[i] = '/';
		}
#ifdef LINUX
		//windows absolute paths cannot be parsed by unix programs
		if(fm.name[1] == ':') {
			fm.name[1] = '_';
		}
#endif
		gFiles.push_back(fm);
	}
	//LOG("Found %i files\n", gFiles.size());

	//read address map
	FAILIF(strcmp(buffer, "SLD") != 0);
	while(1) {
		TEST(readLine(buffer, BUFSIZE, file));
		LineMapping m;
		if(sscanf(buffer, "%x:%i:%"PFZT"i", &m.ip, &m.line, &m.file) != 3)
			break;
		std::pair<std::set<LineMapping>::iterator, bool> res = sLineSet.insert(m);
		TEST(res.second);
		gAddressSet.insert(m);
	}
	//LOG("Found %i lines\n", gLineMap.size());

	//read function map
	FAILIF(strcmp(buffer, "FUNCTIONS") != 0);
	//_ZN12CustomScreenC2EPN4MAUI6ScreenE 00000000,000000d7
	int lastStop = -1;
	while(1) {
		TEST(readLine(buffer, BUFSIZE, file));
		FuncMapping fm;
		int nameLen;
		if(sscanf(buffer, "%*s%n %x,%x", &nameLen, &fm.start, &fm.stop) != 2)
			break;
		buffer[nameLen] = 0;
		if(strcmp(buffer, "CDTOR") == 0)
			continue;
		if(lastStop > fm.start || fm.stop < fm.start) {
			LOG("Error in SLD file at line \"%s\".\n", buffer);
			FAIL;
		}
		lastStop = fm.stop;
		FuncMapping* fmp = new FuncMapping(fm);
		if(buffer[0] == '_')
			fmp->name = buffer + 1;	//skip the extra '_'.
		else
			fmp->name = buffer;
		sFuncMapAddr.insert(fmp);
		sFuncMapName.insert(fmp);
	}

	//read variable map
	FAILIF(strcmp(buffer, "VARIABLES") != 0);
	int lastStart = -1;
	while(1) {
		TEST(readLine(buffer, BUFSIZE, file));
		VarMapping vm;
		int nameLen;
		if(sscanf(buffer, "%*s%n %i %x", &nameLen, &vm.scope, &vm.start) != 2)
			break;
		buffer[nameLen] = 0;
#if 0	//Check disabled because source doesn't conform. We'll live.
		if(lastStart > vm.start) {
			LOG("Error in SLD file at line \"%s\".\n", buffer);
			FAIL;
		}
#endif
		vm.scope = fileIndexFromScope(vm.scope);
		FAILIF(vm.scope < 0);
		lastStart = vm.start;
		if(buffer[0] == '_') {	//because we seem to be getting a few too many variables.
			vm.name = buffer + 1;	//skip the extra '_'.
			gVarMap.push_back(vm);
		}
	}

	//demangle function names, all at once
	std::string mangledNames;
	std::string demangledNames;
	std::string mosyncDir = getenv("MOSYNCDIR");

	for(FuncMapAddr::iterator itr = sFuncMapAddr.begin(); itr != sFuncMapAddr.end(); itr++) {
		(*itr)->mangledName = (*itr)->name;
		mangledNames += (*itr)->name;
		mangledNames += "\n";
	}
	int res = execDoublePipe((mosyncDir + "/bin/c++filt").c_str(), mangledNames, demangledNames);
	if(res != 0) {
		LOG("Error calling c++filt: %i\n", res);
		FAIL;
	}
	//parse demangled names
	size_t pos = 0;
	for(FuncMapAddr::iterator itr = sFuncMapAddr.begin(); itr != sFuncMapAddr.end(); itr++) {
		size_t nextEOL = demangledNames.find('\n', pos);
		DEBUG_ASSERT(nextEOL != std::string::npos);
#ifdef WIN32
#define EXTRA_EOL 1
#else
#define EXTRA_EOL 0
#endif
		(*itr)->name = demangledNames.substr(pos, nextEOL - (pos + EXTRA_EOL)).c_str();
		pos = nextEOL + 1;
	}
	return true;
}

bool mapIpEx(int inIp, LineMapping& lm) {
	//find mapping with ip equal to or less than inIp.
	LineMapping key;
	key.ip = inIp;
	set<LineMapping>::const_iterator itr = sLineSet.lower_bound(key);
	//lower_bound returns >=, instead of <=. So we do this:
	if(itr == sLineSet.end()) {
		if(sLineSet.empty())
			return false;
		itr--;
	} else if(itr->ip > inIp) {
		itr--;
		if(itr == sLineSet.begin())
			return false;
	}

	lm = *itr;
	return true;
}

bool mapIp(int inIp, int& outLine, String& outFile) {
	LineMapping lm;
	if(!mapIpEx(inIp, lm))
		return false;
	outLine = lm.line;
	outFile = gFiles[lm.file].name;
	return true;
}

int mapFileLine(const char* filename, int lineNumber, vector<int>& addresses) {
	if(gAddressSet.size() == 0 || gFiles.size() == 0) {
		return ERR_NOMAP;
	}
	size_t fileIndex;
	for(fileIndex=0; fileIndex<gFiles.size(); fileIndex++) {
		if(stricmp(gFiles[fileIndex].name.c_str(), filename) == 0)	//hack
			break;
	}
	if(fileIndex == gFiles.size())
		return ERR_NOFILE;

	LineMapping lm;
	lm.file = fileIndex;
	lm.line = lineNumber;
	
	AddressSet::const_iterator itr = gAddressSet.lower_bound(lm);

	addresses.clear();

	set<int> foundFunctions;

	// find first valid line
	while(itr!=gAddressSet.end() && itr->file==fileIndex && itr->line<lineNumber) { 
		itr++;
	}
	if(itr==gAddressSet.end()) {
		return ERR_NOLINE;
	}

	lineNumber = itr->line;

	while(itr!=gAddressSet.end() && itr->file==fileIndex && itr->line==lineNumber) {
		const FuncMapping* fm = mapFunctionEx(itr->ip);
		if(foundFunctions.find(fm->start) == foundFunctions.end()) {
			addresses.push_back(itr->ip);
			foundFunctions.insert(fm->start);
		}
		itr++;
	}

	if(addresses.size() == 0)
		return ERR_NOLINE;
	
	return 0;
}

const std::vector<FileMapping>& sldFiles() {
	return gFiles;
}
