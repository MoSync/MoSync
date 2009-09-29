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

#ifndef CHECKSUMH
#include "checksum.h"
#endif
#include "cwrite.h"
//#include "checksum.h"

// write header of cabfile
int cheaderwrite(struct cheader *ch, FILE *fpout)
{
	int error=0;
  	if(!fwrite((char *) &ch->sig, 4, 1, fpout)) error=1;
       	if(!fwrite((char *) &ch->res1, 4, 1, fpout)) error=1;
       	if(!fwrite((char *) &ch->size, 4, 1, fpout)) error=1;
       	if(!fwrite((char *) &ch->res2, 4, 1, fpout)) error=1;
       	if(!fwrite((char *) &ch->offsetfiles, 4, 1, fpout)) error=1;
       	if(!fwrite((char *) &ch->res3, 4, 1, fpout)) error=1;
       	if(!fwrite((char *) &ch->versionMIN, 1, 1, fpout)) error=1;
       	if(!fwrite((char *) &ch->versionMAJ, 1, 1, fpout)) error=1;
       	if(!fwrite((char *) &ch->nfolders, 2, 1, fpout)) error=1;
       	if(!fwrite((char *) &ch->nfiles, 2, 1, fpout)) error=1;
       	if(!fwrite((char *) &ch->flags, 2, 1, fpout)) error=1;
       	if(!fwrite((char *) &ch->setID, 2, 1, fpout)) error=1;
       	if(!fwrite((char *) &ch->cabID, 2, 1, fpout)) error=1;

	return !error;
}      

// write folder-entry of cabfile
int cfolderwrite(struct cfolder *cf, FILE *fpout)
{
	int error=0;

  	if(!fwrite((char *) &cf->offsetdata, 4, 1, fpout)) error=1;
       	if(!fwrite((char *) &cf->ndatab, 2, 1, fpout)) error=1;
       	if(!fwrite((char *) &cf->typecomp, 2, 1, fpout)) error=1;

	return !error;
}


// writre file-entry of cabfile
int cfilewrite( struct cfile *cf, FILE *fpout )
{
	int error=0;

//printf("writing: %s (%d)\n", &cf->name, strlen( &cf->name ) );

	if(!fwrite((char *) &cf->usize, 4, 1, fpout)) error=1;
       	if(!fwrite((char *) &cf->uoffset, 4, 1, fpout)) error=1;
       	if(!fwrite((char *) &cf->index, 2, 1, fpout)) error=1;
       	if(!fwrite((char *) &cf->date, 2, 1, fpout)) error=1;
       	if(!fwrite((char *) &cf->time, 2, 1, fpout)) error=1;
       	if(!fwrite((char *) &cf->fattr, 2, 1, fpout)) error=1;
       	if(!fwrite((char *) &cf->name, strlen(cf->name)+1, 1, fpout)) error=1;

	return !error;
}


// calculate checksum for datablocks
// write cabdata-header and data itself
long writedata( struct cdata *cd, long pos, FILE *fpout, FILE *fptemp )
{
  	char *tmp;
	CHECKSUM csum=0;
  
  	fseek( fptemp,pos, SEEK_SET );
  	tmp = (byte *) calloc( cd->ncbytes, sizeof(byte) );
  	fread( tmp, cd->ncbytes, 1, fptemp );

     // compute checksum
	csum = compute_checksum( (byte *) &cd->ncbytes, 
				sizeof( cd->ncbytes ) + sizeof( cd->nubytes ), 
				compute_checksum( tmp, cd->ncbytes, 0 ) );
	cd->checksum = csum;
     // file header
  	fwrite((char *) &cd->checksum, 4, 1, fpout);
       	fwrite((char *) &cd->ncbytes, 2, 1, fpout);
	fwrite((char *) &cd->nubytes, 2, 1, fpout);
     // data
  	fwrite((byte *) tmp, cd->ncbytes, 1, fpout);

  	free( tmp );
  	pos = ftell( fptemp );
  	return pos;
}

long cdatawrite( struct cdata *cd, FILE *fpout, long pos, FILE *fptemp )
{
       	pos = writedata( cd, pos, fpout,fptemp );
	if(pos!=-1) 
	{ 
		return pos; 
	}
	else 
	{ 
		return pos;
	}
}


