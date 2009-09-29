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

#include "checksum.h"

// compute the checksum for the datablock
CHECKSUM compute_checksum(byte *in, word ncbytes, CHECKSUM seed)
{
	int no_ulongs;
	CHECKSUM csum=0;
	byte *stroom;
	CHECKSUM temp;

	no_ulongs = ncbytes / 4;
	csum = seed;
	stroom = in;

	while(no_ulongs-->0)
	{
		temp = ((CHECKSUM) (*stroom++));
		temp |= (((CHECKSUM) (*stroom++)) << 8);
		temp |= (((CHECKSUM) (*stroom++)) << 16);
		temp |= (((CHECKSUM) (*stroom++)) << 24);

		csum ^= temp;
	}

	temp = 0;
	switch(ncbytes%4)
	{
		case 3: temp |= (((CHECKSUM) (*stroom++)) << 16);
		case 2: temp |= (((CHECKSUM) (*stroom++)) << 8);
		case 1: temp |= ((CHECKSUM) (*stroom++));
		default: break;
	}
	
	csum ^= temp;

	return csum;	
}
