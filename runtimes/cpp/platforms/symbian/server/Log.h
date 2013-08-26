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

#ifndef LOG_H
#define LOG_H

#include <e32base.h>

typedef unsigned char byte;
#define CBP (const byte*)
#define CCP (const char*)

void LogV(const char* fmt, VA_LIST args);
void InitLog();

__inline void Log(const char* fmt, ...) {
	VA_LIST argptr;
	VA_START(argptr, fmt);
	LogV(fmt, argptr);
}

#define LOG Log

#endif	//LOG_H
