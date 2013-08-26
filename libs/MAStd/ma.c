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

#include "ma.h"

void crt_tor_chain(int* ptr);

void ErrorExit(const char * str, int v)
{
	// Display something
	maDrawText(0, v, str);
	maExit(-1);
}

typedef void (*voidfunc)(void);
void crt_tor_chain(int* ptr) {
	if(ptr == NULL)
		return;
	while(*ptr) {
		voidfunc fp = (voidfunc)*ptr;
		fp();
		ptr++;
	}
}

#ifdef MAPIP
void atexit(void* arg);
void atexit(void* arg)
{
}
#endif
