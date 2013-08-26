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

#ifdef MAPIP
#ifndef MAARG_H
#define MAARG_H

typedef __builtin_va_list	__gnuc_va_list;
typedef __gnuc_va_list		va_list;

#define va_start(v,l)		__builtin_stdarg_start((v),l)
#define va_end			__builtin_va_end
#define va_arg			__builtin_va_arg

#endif //MAARG_H
#else
#include <stdarg.h>
#endif //MAVM
