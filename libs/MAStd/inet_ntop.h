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

#ifndef INET_NTOP_H
#define INET_NTOP_H

#ifdef __cplusplus
extern "C" {
#endif

#define INET_ADDRSTRLEN 16
#define INET6_ADDRSTRLEN 46
const char* inet_ntop4(const byte* src, char* dst, size_t size);
const char* inet_ntop6(const byte* src, char* dst, size_t size);

#ifdef __cplusplus
}
#endif

#endif	//INET_NTOP_H
