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

#include "Screen.h"

namespace Base {
	extern SDL_Surface* gBackBuffer;
}

namespace MoRE {

//static SDL_Surface* sPhoneScreen = 0;
static SDL_Surface* sWindowSurface = 0;

void setWindowSurface(SDL_Surface* s) {
	sWindowSurface = s;
}

SDL_Surface* getWindowSurface() {
	return sWindowSurface;
}

void setPhoneScreen(SDL_Surface* s) {
	//sPhoneScreen = s;
}

SDL_Surface* getPhoneScreen() {
	//return sPhoneScreen;
	return Base::gBackBuffer;
}

}
