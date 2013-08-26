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

#ifndef SLD_H
#define SLD_H

#if 0
#include <MAUtil/String.h>
#include <MAUtil/Vector.h>
using namespace MAUtil;
#else
//#error Unsupported string/vector system.

#include <string>
#include <vector>

#endif

//Returns false on failure.
#if 0
bool loadFunctionMap(const char* filename);
void clearFunctionMap();
#endif

bool loadSLD(const char* filename);
void clearSLD();

struct LineMapping {
	int ip, line;
	size_t file;

	bool operator<(const LineMapping& o) const {
		return ip < o.ip;
	}
};

//Returns true on success. In that case, outLine and outFile are valid.
bool mapIp(int inIp, int& outLine, std::string& outFile);

bool mapIpEx(int inIp, LineMapping& lm);

struct FuncMapping {
	int start, stop;
	std::string name;
	std::string mangledName;
};

//Returns a pointer to a FuncMapping or NULL, if ip is not in any function.
const FuncMapping* mapFunctionEx(int ip);

//Returns a pointer to a function name or NULL, if ip is not in any function.
const char* mapFunction(int ip);

//Returns the starting address of a function, or < 0.
int mapFunctionStart(int ip);

//Returns the address of a function, if an exact match is found. < 0 otherwise.
//TODO: add scope. (for static functions,
//which may have different implementations in different compilation units.)
int mapFunction(const char* name);

//Returns the address of a variable, if an exact match is found. < 0 otherwise.
int mapVariable(const char* name, int scope);

//Returns the address of the SLD entry immediately following the one specified by address.
//Each SLD entry has a unique address.
//Fails if address doesn't exist in the SLD table or if it's the last entry.
//Returns <0 on failure.
int nextSldEntry(int address);

#define ERR_NOFILE -2
#define ERR_NOLINE -3
#define ERR_NOMAP -4

//Returns the starting addressses (in 'addresses') of the code represented by the specified source code position,
//if such an address is found. < 0 otherwise.
//Specifically, ERR_NOFILE if the file isn't mapped,
//ERR_NOLINE if the line number is out of bounds,
//or ERR_NOMAP if the map is totally empty.
int mapFileLine(const char* filename, int lineNumber, std::vector<int>& addresses);
const char* getMapFileLineError(int ret);

struct FileMapping {
	int scope;
	std::string name;
};

//Returns the list of files loaded.
const std::vector<FileMapping>& sldFiles();

#endif	//SLD_H
