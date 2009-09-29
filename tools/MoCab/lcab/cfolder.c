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

#include "cfolder.h"

// initialize the cabinet folder
// set number of blocks
//     type of compression
void cfolder_init( struct cfolder *cf, int nod, int type )
{
  	cf->ndatab   = nod; 
  	cf->typecomp = type;	// = 0 (no compression at this point)
}

// offset in cabinet file where datablocks start
void cfolder_offsetdata( struct cfolder *cf, int offset ) 
{ 
	cf->offsetdata = offset; 
}





 

