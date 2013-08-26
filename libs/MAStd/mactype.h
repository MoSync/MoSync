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

/** \file mactype.h
* \brief Character classification.
*/

#ifndef MACTYPE_H
#define MACTYPE_H

#ifdef  __cplusplus
extern "C" {
#endif

#ifdef MAPIP

int _isctype(int c, int mask);

int isspace(int c);
int isupper(int c);
int islower(int c);
int isdigit(int c);
int isxdigit(int c);
int ispunct(int c);
int isalpha(int c);
int isalnum(int c);
int isprint(int c);
int isgraph(int c);
int iscntrl(int c);
int isleadbyte(int c);

int toupper(int c);
int tolower(int c);

int isascii(int c);

#else
#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif	//_CRT_SECURE_NO_WARNINGS
#include <ctype.h>

#endif	//MAPIP

#ifdef  __cplusplus
}
#endif

#endif	//MACTYPE_H
