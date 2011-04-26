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

/** 
 * \file GraphicsOpenGL.h
 * \brief Hardware accelerated implementation of the graphics API.
 * \author Niklas Nummelin
 *
 */

#define _NO_OLDNAMES	//avoid conflicts with y1()
#include <ma.h>

#ifndef _SE_MSAB_MAUTIL_GRAPHICS_OPENGL_H_
#define _SE_MSAB_MAUTIL_GRAPHICS_OPENGL_H_

#include "Graphics.h"

#ifdef __cplusplus
extern "C" {
#endif

	//MAGraphicsDriver* Gfx_getDriverOpenGL(void);

	// if nativeUIOpenGLView is -1 we are running in fullscreen mode.
	void Gfx_useDriverOpenGL(int nativeUIOpenGLView);


#ifdef __cplusplus
}
#endif

#endif

