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

#ifndef _MOSYNC_EXTENSION_H_
#define _MOSYNC_EXTENSION_H_

#define SYSCALL(ret, func) ret func

#define REG(r) gRegs[r]

extern int* gRegs;
extern void* gMemDs;

template<class Dst, class Src> Dst union_cast(Src s) {
	union {
		Dst d;
		Src s;
	} u;
	u.s = s;
	return u.d;
}
#define MAKE(type, a) union_cast<type>(a)

#endif	//_MOSYNC_EXTENSION_H_
