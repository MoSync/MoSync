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
#ifndef CHEADERH
#include "cheader.h"
#endif
#ifndef CFOLDERH
#include "cfolder.h"
#endif
#ifndef CDATAH
#include "cdata.h"
#endif
#ifndef CFILEH
#include "cfile.h"
#endif
#ifndef CWRITEH
#include "cwrite.h"
#endif

long sizefile( char *filename );
int number_of_datablocks( int nof );
char *strippath( char *filename );
char *changepath( char *filename );
int maketempfile( int nof, FILE *fp );
int recurse_directory(const char *file, int *n);

char *inputfiles[MAXINPUTFILES];
char *outputfile;

/**************************************************************************
                              HELPERS
 **************************************************************************/

/* calculate filesize in bytes for filename
   could've used stat here, but returns blocksize */
long sizefile( char *filename )
{
        long size=0;
        FILE *fp = fopen(filename,"rb");
        if(!fp)
        {
                printf("Error: could not open %s\n", filename);
                exit(1);
        }
        while(fgetc(fp)!=EOF)
        {
                ++size;
        }
        fclose(fp);
        return size;
}

/* calculate the number of datablocks we will need:
   cabinet files are written in blocks of 32768 bytes */
int number_of_datablocks(int nof)
{
        int i, size=0;
        for(i=0;i<nof;++i)
        {
                size += sizefile( inputfiles[i] );
        }
        return size/DATABLOCKSIZE+1 ;
}

/* strips the path from a filename */
char *strippath( char *filename )
{
	char *tmp;
	if( !( tmp = strrchr( filename, '/' ) ) ) tmp = filename;
	else *tmp++;
	return tmp;
}

/* replace "/" in filepath with "\" (= windows path seperator) */
char *changepath( char *filename )
{
	char *tmp;
	int i = 0;
	int j = 0;

	tmp = (char *) calloc( strlen(filename)+1, sizeof(char) );
	for( i=0; i<strlen(filename); ++i )
	{
		if( filename[i] != '/' ) tmp[j] = filename[i]; 
		else tmp[j] = '\\';
		++j;
	}
	tmp[j] = STRINGTERM;
	return tmp;
}

/* make a tempfile containing all intput files concatenated */
int maketempfile( int nof, FILE *fp )
{
	int i;
	FILE *fp2;
	char *tmp;

	for( i=0; i<nof; ++i )
	{
		fp2 = fopen( inputfiles[i], "rb" );
		if( fp2 == NULL ) { printf("error: could not open %s\n", inputfiles[i]); return 0; }
		else
		{
			tmp = (byte *) calloc( sizefile( inputfiles[i] ), sizeof( byte ) );
			fread( tmp, sizefile( inputfiles[i] ), 1, fp2 );
			fwrite( (byte *) tmp, sizefile( inputfiles[i] ), 1, fp );
			free( tmp );
			fclose( fp2 );
		}
	}
	
	return 1;
}

/* print the uasge of lcab */
void usage(char **argv)
{
    printf("usage: %s [options] inputfiles outputfile.cab\n", strippath(argv[0])); 
    printf("\nlcab will create a Cabinet File from inputfiles.\n");
    printf("Inputfiles can also be directories together with option -r.\n");
    printf("\nOptions:\n");
    printf("     -r        loop recursive through directories\n");
    printf("     -n        create files without pathnames\n");
    printf("     -q        be quiet!\n");
    printf("\n%s v%s (2003) by Rien (rien@geekshop.be)\n", strippath(argv[0]), VERSION);
    exit(0);
}

/* loop through a file (actually it's a dir) */
int recurse_directory(const char *file, int *n)
{
    DIR *d;
    struct dirent *dp;
    int i = *n;

    if ((d = opendir(file)) == NULL)
	return 1;

    while ((dp = readdir(d)) != NULL) {
	struct stat st;
	char buf[FILENAME_MAX];

	if (strcmp(dp->d_name, ".") == 0 || strcmp(dp->d_name, "..") == 0)
	    continue;

	snprintf(buf, sizeof(buf), "%s/%s", file, dp->d_name);

	if (stat(buf, &st) == -1)
	    continue;

	if (S_ISDIR(st.st_mode)) {
	    recurse_directory(buf, &i);
	    continue;
	}

	/* add to inputfiles here, we don't want a dir to be added, we're just interested in the files */
	inputfiles[i] = realloc(inputfiles[i], (i + 2) * sizeof(char *));
	inputfiles[i++] = strdup(buf);
    }

    closedir(d);
    *n = i;
    return 0;
}


/**************************************************************************
                              MAIN
 **************************************************************************/

int main(int argc, char *argv[])
{
	FILE *fp, *fptmp;
	int i, mysize2;
	long mysize = 0;
	int nof=0;				/* number of files */
	int nod=0;				/* number of datablocks */
	int offsetdata = 0;
	long pos = 0;
	int opt;
	int quiet = 0;

	int makenopath = 0; 			/* create no path */
	int recursive = 0;			/* recursive through dirs */

	struct cheader mycheader;
	struct cfolder mycfolder;
	struct mydatablock *ptrdbs = NULL;
	struct mydatablock *mydatablocks = NULL;
	struct mycfile *ptrcfs = NULL;
	struct mycfile *prevcfile = NULL;
	struct mycfile *mycfiles = NULL;

// with or without paths ? 
// with or without recursive dirs ?

	while ((opt = getopt(argc, argv, "nrhq")) != EOF) {
	    switch (opt) {
		case 'q':
		    quiet = 1;
		    break;
		case 'r':
		    recursive = 1;
		    break;
		case 'n':
		    makenopath = 1;
		    break;
		case 'h':
		default:
		    usage(argv);
	    }
	}

	if (argc - optind < 2)
	    usage(argv);

	printf("%s v%s (2003) by Rien (rien@geekshop.be)\n", strippath(argv[0]), VERSION);

// list files to add
	nof = argc-2-makenopath-recursive; 
	for (i = 0; optind < argc - 1; optind++) 
	{
		struct stat st;

	    	if (stat(argv[optind], &st) == -1) 
		{
			warn("%s", argv[optind]);
			continue;
	    	}

		/* if the inputfile starts with a .. continue, but warn the user */
		if( !strncmp( argv[optind], "..", 2 ) ) printf("##### WARNING : %s has a .. in front of it! #####\n", argv[optind] );

	    	/* if the given input file argument is a dir, loop through it */
	    	if (S_ISDIR(st.st_mode) && recursive) 
		{
			recurse_directory(argv[optind], &i);
			continue;
	    	}

	    	inputfiles[i] = realloc(inputfiles[i], (i + 2) * sizeof(char *));
	    	inputfiles[i++] = strdup(argv[optind]);
	}
	
	inputfiles[i] = NULL;
	
	/* we need all files, don't skip the last one (nof doesn't count from 1) */
	nof = i;


// init 
	nod = number_of_datablocks(nof);
	outputfile = (char *) calloc(MAXSIZE,sizeof(char));
	strcpy( outputfile, argv[argc - 1]);

// let the user see something
	printf("nopath          : "); makenopath ? printf("yes\n") : printf("no\n");
	printf("recursive       : "); recursive ? printf("yes\n") : printf("no\n");
	printf("quiet           : "); quiet ? printf("yes\n") : printf("no\n");

	if (!quiet) 
	{
		printf("inputfiles      : ");
	    	for(i=0;i<nof;++i) printf("%s ", inputfiles[i]);
		printf("\noutputfile      : %s\n", outputfile);
	}

// HEADER INIT
	cheader_init( &mycheader, "MSCF", 1, nof, 0, 1234, 0 );
	cheader_offsetfiles( &mycheader, CFI_START );
// HEADER SIZE PART 1	
	mysize2 = CFI_START + nof*16;									/* first part of cfile struct = 16 bytes */
	for( i=0; i<nof; ++i )
		mysize2 += strlen( makenopath? strippath( inputfiles[i] ) : changepath( inputfiles[i] ) ) + 1;		/* + 1 for \0 ! */
	mysize2 += nod*8;										/* first part cdata struct is 8 bytes */

// FOLDER
	cfolder_init( &mycfolder, nod, 0 );
// FOLDER->offsetdata
	for( i=0; i<nof; ++i )
		offsetdata += 16 + strlen( makenopath ? strippath( inputfiles[i] ) : changepath( inputfiles[i] ) ) + 1;
	cfolder_offsetdata( &mycfolder, CFI_START + offsetdata );


// DATABLOCKS
	mydatablocks = (struct mydatablock *) calloc( 1, sizeof(struct mydatablock) );
	mydatablocks->next = NULL;
	ptrdbs = mydatablocks;

	for( i=0; i<nof; ++i )
		mysize += sizefile( inputfiles[i] );

	cdata_init( &ptrdbs->cd, 0 );
	if( mysize < DATABLOCKSIZE )
	{
		cdata_ncbytes( &ptrdbs->cd, mysize );
		cdata_nubytes( &ptrdbs->cd, mysize );
		mysize2 += mysize;							/* header part 2 */
	}	
	else
	{
		for( i=0; i<nod; ++i )
		{
			if( i != nod-1 )
			{
				cdata_ncbytes( &ptrdbs->cd, DATABLOCKSIZE );
				cdata_nubytes( &ptrdbs->cd, DATABLOCKSIZE );
				mysize2 += DATABLOCKSIZE;				/* header part 2 */
			
				ptrdbs->next = (struct mydatablock *) calloc( 1, sizeof(struct mydatablock) );
				ptrdbs = ptrdbs->next;
				ptrdbs->next = NULL;
				cdata_init( &ptrdbs->cd, 0 );
			}
			else
			{
				mysize -= DATABLOCKSIZE * (nod-1);
				cdata_ncbytes( &ptrdbs->cd, mysize );
				cdata_nubytes( &ptrdbs->cd, mysize );
				mysize2 += mysize;					/* header part 2 */
			}
			
		}
	}


// HEADER SIZE PART 2
	cheader_size( &mycheader, mysize2 );

	for( i=0; i<nof; ++i )
		mysize += sizefile( inputfiles[i] );
	if (!quiet ) printf("cabfile         : %d bytes (approx. %.2f Kbytes)\n", (int) mysize, (float) mysize/1024 );

// FILES
	mycfiles = (struct mycfile *) calloc( 1, sizeof( struct mycfile ) );
	mycfiles->next = NULL;
	ptrcfs = mycfiles;

	for( i=0; i<nof; ++i )
	{
		cfile_init( &ptrcfs->cf, sizefile( inputfiles[i] ), 0, makenopath ? strippath( inputfiles[i] ) : changepath( inputfiles[i] ),inputfiles[i] );

		// offset 1st cfile = 0, 2nd = offset 1st + filesize 1st, etc..
		if( i==0 ) cfile_uoffset( &ptrcfs->cf, 0 );
		else
		{
			cfile_uoffset( &ptrcfs->cf, prevcfile->cf.uoffset + sizefile( inputfiles[i-1] ) );
		}

		if( i < nof-1 )
		{
			prevcfile = ptrcfs;
			ptrcfs->next = (struct mycfile *) calloc( 1, sizeof( struct mycfile ) );
			prevcfile = ptrcfs;
			ptrcfs = ptrcfs->next;
			ptrcfs->next = NULL;		
		}
	}

// WRITE
	fptmp = fopen( "lcab.tmp", "wb" );
	if( fptmp == NULL ) { printf("error: could not open lcab.tmp for writing\n"); return 0; }
	else
	{
		if( !maketempfile( nof, fptmp ) ) { printf("error: could not make tempfile"); return 0; }
		else if(!quiet) printf("tmp,");
		fclose( fptmp );
	}
	fp = fopen( outputfile, "wb" );
	if( fp == NULL ) { printf("error: could not open %s for writing\n", outputfile); return 0; }
	else
	{
		if( !cheaderwrite( &mycheader, fp ) ) { printf("error: could not write header\n"); fclose( fp ); return 0; }
		if( !quiet ) printf("header,");
		if( !cfolderwrite( &mycfolder, fp ) ) { printf("error: could not write folder\n"); fclose( fp ); return 0; }
		if( ! quiet ) printf("folder,");

		ptrcfs = mycfiles;
		if( !cfilewrite( &ptrcfs->cf, fp ) ) { printf("error: could not write header\n"); fclose( fp ); return 0; }
		if( !quiet ) printf(".");
		while( ptrcfs->next != NULL )
		{
			ptrcfs = ptrcfs->next;
			if( !cfilewrite( &ptrcfs->cf, fp ) ) { printf("error: could not write header\n"); fclose( fp ); return 0; }
			if( !quiet ) printf(".");
		} 


		fptmp = fopen( "lcab.tmp", "rb" );
		if( fptmp == NULL ) { printf("error: could not open lcab.tmp for writing\n"); return 0; }
		else
		{
			ptrdbs = mydatablocks;
			pos = cdatawrite( &ptrdbs->cd, fp, pos, fptmp );
			if( pos == -1 )
			{
				printf("error: could not write datablock at pos: %ld\n", pos);	
				fclose(fptmp);
				fclose(fp);
				if( remove( "lcab.tmp" ) ) printf("could not remove lcab.tmp\n");		
			}
			while( ptrdbs->next != NULL )
			{
				ptrdbs = ptrdbs->next;
				pos = cdatawrite( &ptrdbs->cd, fp, pos, fptmp );
				if( pos == -1 )
				{
					printf("error: could not write datablock at pos: %ld\n", pos);	
					fclose(fptmp);
					fclose(fp);
					if( remove( "lcab.tmp" ) ) printf("could not remove lcab.tmp\n");		
				}
			}

			fclose(fptmp);
		}

		fclose( fp );

	}
	if( !quiet ) printf("\n");	


	if( remove( "lcab.tmp" ) ) printf("could not remove lcab.tmp\n");

// cleanup
	for(i=0;i<nof;++i)
		free( inputfiles[i] );
	free( outputfile );

	printf("done\n");

	return 0;
}
