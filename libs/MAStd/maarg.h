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

/** \file maarg.h
* \brief Variadric functions

* VAARGS
* Functions in C take a variable amount of arguments, functions
* such as printf and sprintf, these function use VAARGS which allow
* a function to read the arguments passed from an ellipsis parameter
* (...).
* Note all functions are implemented as macros.
*/

#ifdef MAPIP

#ifndef MAARG_H
#define MAARG_H

typedef __builtin_va_list	__gnuc_va_list;
typedef __gnuc_va_list		va_list;

/**
* va_start
* Sets the va parameter to the first parameter
*/

#define va_start(v,l)		__builtin_stdarg_start((v),l)

/**
* va_end
* A house keeping function that must be used when your finished with
* va args.
*/

#define va_end				__builtin_va_end

/**
* va_arg
* Accesses a functions variable argument list
*/

#define va_arg				__builtin_va_arg

#define va_copy(d,s)	__builtin_va_copy((d),(s))

#endif //MAARG_H
#else
#include <stdarg.h>			/* WIN32*/
#endif
