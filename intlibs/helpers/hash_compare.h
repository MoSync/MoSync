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

#ifndef HASH_COMPARE_H
#define HASH_COMPARE_H

namespace __gnu_cxx {
        template<> struct hash<std::string>
        {
                size_t operator()(const std::string& __s) const
                { return __stl_hash_string(__s.c_str()); }
        };
#ifdef DARWIN
	template<class T> struct hash<T*> {
		size_t operator()(T* ptr) const {
			return hash<size_t>()((size_t)ptr);
		}
	};
#endif	//DARWIN
}
template<class T> class hash_compare : public hash<T> {};

#endif	//HASH_COMPARE_H
