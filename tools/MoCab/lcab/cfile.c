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

#include "cfile.h"

// initialize the cabfile-structure
// set filesize
//     index
//     filename
void cfile_init( struct cfile *cf, int usize, int _index, char *filename, char *origFilename )
{
	struct stat attributes;
	struct tm *fts;	// file last modified timestamp

	cf->usize = usize;
	cf->index = _index;
	stat(origFilename,&attributes);
	fts=localtime(&(attributes.st_mtime));
	printf("cfileInit: %s localtime:\n",filename);

					// it can simply be -80
	cf->date = ( ( fts->tm_year + 1900 - 1980 ) << 9 ) + ( (fts->tm_mon+1) << 5 ) + ( fts->tm_mday);

	cf->time = ( fts->tm_hour << 11 ) + ( fts->tm_min << 5 ) + ( fts->tm_sec / 2 );

	//cf->fattr = ATTR_e | ATTR_a;  // default ATTR_e is not good idea
	cf->fattr = ATTR_a;

	strcpy((char*)cf->name, filename);
	cf->name[strlen(filename)] = STRINGTERM;
}

// uncompressed offset where file resides in datablocks
void cfile_uoffset( struct cfile *cf, int offset )
{
	cf->uoffset = offset;
}
