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

#include "helpers/attribute.h"

#ifdef WIN32
#define DLLEXPORT dllexport
#define DLLIMPORT dllimport
#elif defined(LINUX) || defined (DARWIN)
#define DLLEXPORT visibility("default")
#define DLLIMPORT
#endif

#ifdef MOSYNC_DLL_EXPORT
#define MOSYNC_API DLLEXPORT
#elif defined(MOSYNC_DLL_IMPORT)
#define MOSYNC_API DLLIMPORT
#else
#define MOSYNC_API
#endif

typedef int (*mainfunc)();

extern "C" int ATTRIBUTE(MOSYNC_API, mosyncLibMain(int argc, char** argv, mainfunc maMain));
