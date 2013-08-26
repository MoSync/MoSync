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
#include <conprint.h>
#include <mastring.h>
#include <mastdlib.h>

int MAMain() {
	double d;
	static const char str1[] = "4.56";
	static const char str2[] = "12389.5612323545034954378343";

	InitConsole();
	printf("Hello World.\n");
	printf("1: %s\n", str1);
	d = strtod(str1, NULL);
	printf("d: %f\n", d);
	printf("2: %s\n", str2);
	d = strtod(str2, NULL);
	printf("d: %f\n", d);

	//Freeze
	Freeze(0);
	return 0;
}
