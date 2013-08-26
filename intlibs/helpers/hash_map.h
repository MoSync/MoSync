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

#ifndef HASH_MAP_H
#define HASH_MAP_H

#ifdef __GNUC__
#ifdef HAVE_TR1
#include <unordered_map>
#define hash_map std::unordered_map
#else
#include <ext/hash_map>
using namespace __gnu_cxx;
#include "hash_compare.h"
#endif	//HAVE_TR1
#elif defined(_MSC_VER)
#include <hash_map>
using namespace stdext;
#else
#error Unsupported platform!
#endif

#endif	//HASH_MAP_H
