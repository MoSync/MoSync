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
#include <matime.h>
#include <conprint.h>
#include <maassert.h>

extern "C" int MAMain() {
	printf("Local time:\n");
	time_t myTime = maLocalTime();
	for(int i = 0; i < 366*8 ; i++){
		printf("%s\n", sprint_time(myTime));
		myTime -= (60 * 60 * 24);
	}
	FREEZE;
}
