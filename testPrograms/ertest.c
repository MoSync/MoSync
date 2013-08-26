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

#include "maapi.h"
#include "shared/print.h"
#include "shared/misc.h"

int MAMain() {
	const char* purl = "wap.mopix.se/test/test.comb";
	println("ER HTTP GET test");
	println(purl);
	maHttpGetToObject(purl, 1);
	maWait(WAIT_CONN);
	{
		int res = maConnState();
		printlnf("res %i", res);
		while(!(maGetEvent() == MAK_DOWN)) maWait(WAIT_EVENT);
		if(res <= 0)
			maExit(0);
	}
	maLoadProgram(1);  
	println("It didn't work.");
	while(1) maWait(WAIT_EVENT);
}

#include "shared/print.c"
#include "shared/misc.c"

