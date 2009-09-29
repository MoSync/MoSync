/*  Copyright 2001 Rien Croonenborghs, Ben Kibbey, Shaun Jackman, Ivan Brozovic

    This file is part of lcab.
    lcab is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.
    lcab is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.
    You should have received a copy of the GNU General Public License
    along with lcab; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

#ifndef MYTYPESH
#include "mytypes.h"
#endif
#ifndef CSTRUCTH
#include "cstruct.h"
#endif

#ifndef CFILEH
#define CFILEH

struct mycfile
{
	struct cfile cf;
	struct mycfile *next;
};

void cfile_init( struct cfile *cf,int usize, int index, char *filename , char *origFilename);

void cfile_uoffset( struct cfile *cf, int offset );


#endif
