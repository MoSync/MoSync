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

#include <stdio.h>

void GiveMeTwo(const char* s1, const char* s2);
const char* GiveMeBack(const char* s);

const char* GiveMeBack(const char* s)
{
	printf("%s\n", s);
	return s;
}

void GiveMeTwo(const char* s1, const char* s2)
{
	printf("%s\n", s1);
	printf("%s\n", s2);
}

int main()
{
	GiveMeTwo(GiveMeBack("One"), GiveMeBack("Two"));
}
