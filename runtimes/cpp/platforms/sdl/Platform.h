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

#ifndef _PLATFORM_H_
#define _PLATFORM_H_

#include <SDL/SDL.h>
#include <string>

#include "Skinning/DeviceProfile.h"
#include "Skinning/DeviceSkin.h"

#include "config_platform.h"

#include <bluetooth/discovery.h>

#include "pim.h"
#include "helpers/CPP_IX_PIM.h"

#ifdef EMULATOR
#define FILESYSTEM_CHROOT 1
#define FILESYSTEM_DIR "filesystem"
#else
#define FILESYSTEM_CHROOT 0
#endif	//EMULATOR

namespace Core {
	class VMCore;
}

#define VSV_ARGPTR_DECL , va_list argptr
#define VSV_ARGPTR_USE , argptr

#endif
