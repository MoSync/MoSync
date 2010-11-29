#ifndef UTIL_H
#define UTIL_H

#include <map>
#include <string>

// All these functions exit() on error.

void copyFile(const char* dst, const char* src);
void appendFile(const char* dst, const char* src);

std::string readFileToString(const char* src);

size_t getFileSize(const char* filename);

// mallocs the buffer
void* readBinaryFile(const char* src, size_t& size);

void writeFile(const char* dst, const void* src, int size);

typedef std::map<std::string, std::string> TemplateMap;

void applyTemplate(const char* dst, const char* src, const TemplateMap&);

// convert all \ to /
void toSlashes(std::string&);

// add / to end of string, if one wasn't already there.
void toDir(std::string&);

std::string fullpathString(const char* name);

#endif	//UTIL_H
