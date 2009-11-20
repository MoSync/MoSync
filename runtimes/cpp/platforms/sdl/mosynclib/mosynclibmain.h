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

#include "helpers/attribute.h"

#ifdef WIN32
#define DLLEXPORT ATTRIB(dllexport)
#define DLLIMPORT ATTRIB(dllimport)
#elif defined(LINUX)
#define DLLEXPORT ATTRIB(visibility("default"))
#define DLLIMPORT
#else
#error Unsupported platform!
#endif

#ifdef MOSYNC_DLL_EXPORT
#define MOSYNC_API DLLEXPORT
#elif defined(MOSYNC_DLL_IMPORT)
#define MOSYNC_API DLLIMPORT
#else
#define MOSYNC_API
#endif

typedef int (*mainfunc)();

extern "C" int MOSYNC_API mosyncLibMain(int argc, char** argv, mainfunc maMain);
