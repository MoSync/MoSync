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

typedef int (*mainfunc)();

//__attribute__((dllimport)) 
int mosyncLibMain(int argc, char** argv, mainfunc maMain);

int MAMain();

#ifdef WIN32
#include <windows.h>

//int main(int argc, char** argv) {
int APIENTRY WinMain(HINSTANCE a, HINSTANCE b, LPSTR c, int d) {
#else
int main() {
#endif
	return mosyncLibMain(0, 0, MAMain);
}
