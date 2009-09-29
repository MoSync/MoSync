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

#include "cdata.h"

// initialize the cabdata-structure
// set checksum for datablock
// checksums are computed at a later time and added then
void cdata_init( struct cdata *cd, int checksum ) 
{ 
	cd->checksum = checksum; 
}

// set number of compressed bytes in datablock
void cdata_ncbytes( struct cdata *cd, int ncb )
{ 
	cd->ncbytes = ncb; 
}

// set number of uncompressed bytes in datablock
void cdata_nubytes( struct cdata *cd, int nub ) 
{
	cd->nubytes = nub; 
}


