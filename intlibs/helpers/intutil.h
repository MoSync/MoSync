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

#ifndef INTUTIL_H
#define INTUTIL_H

#include <string>

//spawns a process, redirects \a in to stdin,
//and redirects stdout to \a out
//returns the process' return code, if it ran.
//a non-zero error code otherwise.
int execDoublePipe(const char* cmdline, const std::string& in, std::string& out);

#endif	//INTUTIL_H
