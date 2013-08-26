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

#ifndef _MEMFUNCS_H_
#define _MEMFUNCS_H_

#define ZEROMEM(mem, size) memset(mem, 0, size)
#define MEMCPY(dst, src, size) memcpy(dst, src, size)
#define MEMSET(mem, val, size) memset(mem, val, size)
#define MEMCMP(a, b, size) memcmp(a, b, size)

#endif // _MEMFUNCS_H_
