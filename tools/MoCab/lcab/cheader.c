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

#include "cheader.h"

// initialize the cabinet header
// set sign (must be set to 'MSCF')
//     number of folders
//     number of files
//     flags (multiple folder/set)
//     set ID
//     cabinet ID
void cheader_init(struct cheader *ch, char *sign, 
		    int nfolders, int nfiles, 
		    int flags, int setID, 
		    int cabID)
{
	int i;

	for(i=0;i<4;++i) { ch->sig[i] = *(sign+i); }
	ch->res1 = 0;
	ch->res2 = 0;
	ch->res3 = 0;
	ch->versionMAJ = 1;
	ch->versionMIN = 3;
	ch->nfolders = nfolders;
	ch->nfiles = nfiles;
	ch->flags = flags;
	ch->setID = setID;
	ch->cabID = cabID;
}

// set total cabinet file size
void cheader_size(struct cheader *ch, int size) 
{ 
	ch->size = size; 
}

// offset where cabfile-entries start
void cheader_offsetfiles(struct cheader *ch, int offset) 
{ 	
	ch->offsetfiles = offset; 
}

