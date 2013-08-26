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

#include <stddef.h>
#include <string.h>

#include <helpers/types.h>
typedef u32 uint32_t;
typedef s32 int32_t;

extern "C" {
//#ifndef _STRING_H_
//const char* strrchr(const char* str, int c);
//#endif
size_t strlen(const char* str);
char* strcpy(char* dst, const char* src);
int strcmp(const char* a, const char* b);
int strncmp(const char *string1, const char *string2, size_t count);
int isdigit(int c);

int memcmp(const void* a, const void* b, size_t len);
void* memcpy(void* dst, const void* src, size_t len);
}
