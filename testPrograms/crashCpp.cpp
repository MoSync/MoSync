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
#include <maassert.h>

class Failer {
public:
void recursiveFail(int i) {
	if(i == 0)
		maPanic((int)this, "Fail!");
	else
		recursiveFail(i - 1);
		//f2(i);
}
void f2(int i) {
	maPanic(i, "Fail!");
}
};

extern "C" int MAMain() {
	Failer f;
	f.recursiveFail(4);

	maUpdateScreen();

	maDrawText(0, 40, "Image destroyed.");
	maUpdateScreen();

	maDrawText(0, 60, "Data created in its place.");
	maUpdateScreen();

	FREEZE;
}
