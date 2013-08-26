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

class Syscall {
private:
#ifdef MOBILEAUTHOR
public:
#endif
SDL_Surface* loadImage(MemStream& s);
SDL_Surface* loadSprite(SDL_Surface* surface, ushort left, ushort top,
	ushort width, ushort height, ushort cx, ushort cy);

public:
		struct STARTUP_SETTINGS {
			STARTUP_SETTINGS ( ) {
				showScreen = true;
				haveSkin   = true; 
				id         = NULL;
				iconPath   = NULL;
				resmem     = ((uint)-1);
			}

			bool showScreen;
			const char* id;
			const char *iconPath;
			uint resmem;
			MoRE::DeviceProfile profile;
			bool haveSkin;
#ifdef EMULATOR
			uint timeout;
#endif
		};

	Syscall(const STARTUP_SETTINGS&);
	Syscall(int width, int height, const STARTUP_SETTINGS&);

#ifdef EMULATOR
public:
#define PIMIMPL_H
#include "pimImpl.h"
HashMapNoDelete<PimItem> mPimItems;
#endif	//EMULATOR
