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

#include <ma.h>
#include <mastring.h>

static void LOG(const char* msg) {
	maWriteLog(msg, strlen(msg));
}

int MAMain() {
	// check to see if we're the original.
	MAHandle program;
	if(maFindLabel("label") > 0) {
		LOG("write\n");
		// if so, save program file to store.
		MAHandle h = maOpenStore("program", MAS_CREATE_IF_NECESSARY);
		maWriteStore(h, 1);
		maCloseStore(h, 0);
		program = 1;
	} else {
		LOG("read\n");
		// otherwise, read program file from store.
		MAHandle h = maOpenStore("program", 0);
		program = maCreatePlaceholder();
		maReadStore(h, program);
		maCloseStore(h, 0);
	}

	LOG("load\n");
	maLoadProgram(program, 0);
	return 1;
}
