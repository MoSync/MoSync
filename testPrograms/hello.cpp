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

int array[128];

struct Base {
	int a;
	virtual void run() {
	}
};

struct Inherited : public Base {
	float b[2];

	void run() {
	}
};

Inherited i;
Inherited *ii = &i;

double add(double a, int b) {
	return a + b;
}

int MAMain() {
	maSetColor(0xFFFFFF);
	maDrawText(0,0, "Hello World!");
	maUpdateScreen();
	
	i.a = 2;
	i.b[0] = 2.341;
	i.b[1] = 3.14159;

	i.run();
	add(1, 2);

	//Freeze
	Freeze(0);
	return 0;
}
