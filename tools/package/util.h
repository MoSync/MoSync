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

#ifndef UTIL_H
#define UTIL_H

#include <map>
#include <set>
#include <vector>
#include <string>
#include <ios>
#include <ostream>

// All these functions exit() on error.

void copyFile(const char* dst, const char* src);
void appendFile(const char* dst, const char* src);
bool existsFile(const char* file);
bool existsDir(const char* dir);
void renameFile(const std::string& dst, const std::string& src);

std::string readFileToString(const char* src);

std::streamoff getFileSize(const char* filename);

// mallocs the buffer
void* readBinaryFile(const char* src, size_t& size);

void writeFile(const char* dst, const void* src, size_t size);

typedef std::map<std::string, std::string> TemplateMap;

void applyTemplate(const char* dst, const char* src, const TemplateMap&);

// convert all \ to /
void toSlashes(std::string&);

// convert all / to '\'
void toBackSlashes(std::string&);

// add / to end of string, if one wasn't already there.
void toDir(std::string&);

// Makes sure arguments with spaces are quoted
std::string arg(const char* arg);
std::string arg(std::string arg);

// Makes sure a command line argument is properly slashed and quoted
std::string file(const char* arg);
std::string file(std::string arg);

// returns the directory part of the name. assumes front-slashes.
std::string getDir(const std::string& name);

std::string fullpathString(const char* name);

std::string delim(std::vector<std::string>& input, const std::string& delim);

void split(std::vector<std::string>& out, const std::string& input, const std::string& delims);

// Writes a line that is "manifest-safe", by splitting it every 72 characters.
void write72line(std::ostream& output, const std::string& input);

// Stores the pointer as a name for the stream.
void setName(std::ios& i, const char* name);

// Aborts the program if the stream is not good().
// Reports the error using the name set by setName().
void beGood(std::ios& i);

bool streq(const char* a, const char* b);

#endif	//UTIL_H
