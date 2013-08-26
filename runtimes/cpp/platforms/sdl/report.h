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

//Functions for sending data to the parent process

#include "report_codes.h"

void reportSetFd(int fd);
void report(int opcode, const void* buf=NULL, int count=0);
void reportClose();
void reportIp(int r, const char* message);

#define REPORT(string) { static const char s[] = string; LOG("%s\n", s);\
	report(REPORT_STRING, s, sizeof(s)-1); }
