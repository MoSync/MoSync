#ifndef UTIL_H
#define UTIL_H

#include <map>
#include <string>

void copyFile(const char* dst, const char* src);
void appendFile(const char* dst, const char* src);

std::string readFileToString(const char* src);

typedef std::map<std::string, std::string> TemplateMap;

void applyTemplate(const char* dst, const char* src, const TemplateMap&);

// convert all \ to /
void toSlashes(std::string&);

// add / to end of string, if one wasn't already there.
void toDir(std::string&);

#endif	//UTIL_H
