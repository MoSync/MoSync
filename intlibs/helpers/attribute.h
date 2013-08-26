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

#if defined(__GNUC__) || defined(__SYMBIAN32__)

#ifdef _android
#define ATTRIBUTE(a, func) ((func))
#else
#define ATTRIBUTE(a, func)  func __attribute__ ((a))
#endif

#define ATTRIB(a) __attribute__ ((a))
#define GCCATTRIB(a) __attribute__ ((a))
#elif defined(_MSC_VER)
#define ATTRIBUTE(a, func)  __declspec (a) func
#define ATTRIB(a) __declspec (a)
#define GCCATTRIB(a)
#else
#error Unsupported compiler!
#endif

#if defined(__SYMBIAN32__)
#define PRINTF_ATTRIB(a,b)
#else
#define PRINTF_ATTRIB(a,b) GCCATTRIB(format(printf, a, b))
#endif
