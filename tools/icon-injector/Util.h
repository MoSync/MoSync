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

// convert all \ to /
void toSlashes(std::string&);

// convert all / to '\'
void toBackSlashes(std::string&);

// Makes sure a file name properly slashed
std::string file(const char* arg);
std::string file(std::string arg);

// Makes sure arguments with spaces are quoted
std::string arg(const char* arg);
std::string arg(std::string arg);

#endif	//UTIL_H
