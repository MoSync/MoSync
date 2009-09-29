/*  Copyright 2001 Rien Croonenborghs, Ben Kibbey, Shaun Jackman

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
#define CSTRUCTH

struct cheader
{
	byte sig[4];			
	dword res1;				
	dword size;			
	dword res2;				
	dword offsetfiles;		
	dword res3;				
	byte versionMIN;	
	byte versionMAJ;		
	word nfolders;			
	word nfiles;			
	word flags;				
	word setID;			
	word cabID;				
};

struct cfolder
{
	dword offsetdata;		
	word ndatab;			
	word typecomp;			
};

struct cfile
{
	dword usize;			
	dword uoffset;			
	word index;				
	word date;				
	word time;				
	word fattr;				
	byte name[MAXSIZE];		
};

struct cdata
{
	dword checksum;			
	word ncbytes;			
	word nubytes;			
};

#endif
