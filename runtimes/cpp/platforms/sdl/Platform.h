/* Copyright (C) 2009 Mobile Sorcery AB

This program is free software; you can redistribute it and/or modify it under
the terms of the GNU General Public License, version 2, as published by
the Free Software Foundation.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
for more details.

You should have received a copy of the GNU General Public License
along with this program; see the file COPYING.  If not, write to the Free
Software Foundation, 59 Temple Place - Suite 330, Boston, MA
02111-1307, USA.
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
