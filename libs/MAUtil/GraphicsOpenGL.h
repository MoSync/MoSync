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

/** \file Graphics.h
 * \brief Translation/clipping stack for MoSync graphics.
 *
 * This file contains a set of graphics functions that closely mirror the ones available as MoSync syscalls. However, these
 * functions make use of a transformation stack similar to ones commonly found in other graphics libraries. It simplifies
 * managing transformations, removing the need to manually keep track of the current transformation state.
 * Currently, only translation is supported.
 *
 * \author Patrick Broman
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

	MAGraphicsDriver* Gfx_getDriverOpenGL(void);
	
#ifdef __cplusplus
}
#endif

#endif

