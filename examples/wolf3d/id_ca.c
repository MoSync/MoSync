#include "wl_def.h"
typedef long ssize_t;

typedef struct
{
	/* 0-255 is a character, > is a pointer to a node */
	int bit0, bit1;
} huffnode;

/*
=============================================================================

						 GLOBAL VARIABLES

=============================================================================
*/

static word RLEWtag;

int mapon;

word	*mapsegs[MAPPLANES];
maptype	*mapheaderseg[NUMMAPS];
byte	*audiosegs[NUMSNDCHUNKS];
byte	*grsegs[NUMCHUNKS];

char extension[5];
#define gheadname "vgahead."
#define gfilename "vgagraph."
#define gdictname "vgadict."
#define mheadname "maphead."
#define gmapsname "gamemaps."
#define aheadname "audiohed."
#define afilename "audiot."
#define pfilename "vswap."

static long *grstarts;	/* array of offsets in vgagraph */
static long *audiostarts; /* array of offsets in audiot */

static huffnode grhuffman[256];

static int grhandle = -1;	/* handle to VGAGRAPH */
static int maphandle = -1;	/* handle to GAMEMAPS */
static int audiohandle = -1;	/* handle to AUDIOT */

/*
=============================================================================

					   LOW LEVEL ROUTINES

=============================================================================
*/

static void CA_CannotOpen(const char *string)
{
	char str[30];

	strcpy(str, "Can't open ");
	strcat(str, string);
	strcat(str, "!\n");
	Quit(str);
}

/*
==========================
=
= CA_WriteFile
=
= Writes a file from a memory buffer
=
==========================
*/

boolean CA_WriteFile(const char *filename, const void *ptr, long length)
{
	ssize_t l;
	int handle;

	handle = OpenWrite(filename);

	if (handle == -1)
		return false;

	l = WriteBytes(handle, (const byte *)ptr, length);
	if (l == -1) {
		perror("CA_FarWrite");
		return false;
	} else if (l == 0) {
		fprintf(stderr, "CA_FarWrite hit EOF?\n", 0);
		return false;
	} else if (l != length) {
		fprintf(stderr, "CA_FarWrite only wrote %d out of %ld\n", l, length);
		return false;
	}

	CloseWrite(handle);

	return true;
}

/*
==========================
=
= CA_LoadFile
=
= Allocate space for and load a file
=
==========================
*/

boolean CA_LoadFile(const char *filename, memptr *ptr)
{
	int handle;
	ssize_t l;
	long size;

	if ((handle = OpenRead(filename)) == -1)
		return false;

	size = ReadLength(handle);
	MM_GetPtr(ptr, size);

	l = ReadBytes(handle, (byte *)(*ptr), size);

	if (l == -1) {
		perror("CA_FarRead");
		return false;
	} else if (l == 0) {
		fprintf(stderr, "CA_FarRead hit EOF?\n");
		return false;
	} else if (l != size) {
		fprintf(stderr, "CA_FarRead only read %d out of %ld\n", l, size);
		return false;
	}

	CloseRead(handle);

	return true;
}

/*
============================================================================

		COMPRESSION routines

============================================================================
*/

/*
======================
=
= CAL_HuffExpand
= Length is the length of the EXPANDED data
=
======================
*/

/* From Ryan C. Gordon -- ryan_gordon@hotmail.com */
void CAL_HuffExpand(const byte *source, byte *dest, long length,
	const huffnode *htable)
{
	const huffnode *headptr;
	const huffnode *nodeon;
	byte      mask = 0x01;
	word      path;
	byte     *endoff = dest + length;

	nodeon = headptr = htable + 254;

	do {
		if (*source & mask)
			path = nodeon->bit1;
	        else
			path = nodeon->bit0;
       		mask <<= 1;
	        if (mask == 0x00) {
			mask = 0x01;
			source++;
	        }
		if (path < 256) {
			*dest = (byte)path;
			dest++;
			nodeon = headptr;
		} else
			nodeon = (htable + (path - 256));
	} while (dest != endoff);
}

/*
======================
=
= CAL_CarmackExpand
= Length is the length of the EXPANDED data
=
= Note: This function happens to implicity swap words' bytes around.
=       For maps, this happens to be the desired effect.
=
======================
*/

#define NEARTAG	0xa7
#define FARTAG	0xa8

void CAL_CarmackExpand(const byte *source, word *dest, word length)
{
	unsigned int offset;
	word *copyptr, *outptr;
	byte chhigh, chlow;
	const byte *inptr;

	length /= 2;

	inptr = source;
	outptr = dest;

	while (length) {
		chlow = *inptr++; /* count */
		chhigh = *inptr++;

		if (chhigh == NEARTAG) {
			if (!chlow) {
				/* have to insert a word containing the tag byte */
				*outptr++ = (chhigh << 8) | *inptr;
				inptr++;

				length--;
			} else {
				offset = *inptr;
				inptr++;

				copyptr = outptr - offset;

				length -= chlow;
				while (chlow--)
					*outptr++ = *copyptr++;
			}
		} else if (chhigh == FARTAG) {
			if (!chlow) {
				/* have to insert a word containing the tag byte */
				*outptr++ = (chhigh << 8) | *inptr;
				inptr++;

				length--;
			} else {
				offset = *inptr | (*(inptr+1) << 8);
				inptr += 2;

				copyptr = dest + offset;
				length -= chlow;
				while (chlow--)
					*outptr++ = *copyptr++;
			}
		} else {
			*outptr++ = (chhigh << 8) | chlow;
			length--;
		}
	}
}

/*
======================
=
= CA_RLEWexpand
= length is EXPANDED length
=
======================
*/

void CA_RLEWexpand(const word *source, word *dest, long length, word rlewtag)
{
	word value, count, i;
	word *end = dest + length / 2;

	/* expand it */
	do {
		value = *source++;

		if (value != rlewtag) {
			/* uncompressed */
			*dest++ = value;
		} else {
			/* compressed string */
			count = *source++;

			value = *source++;
			for (i = 0; i < count; i++)
				*dest++ = value;
		}
	} while (dest < end);
}

/*
=============================================================================

					 CACHE MANAGER ROUTINES

=============================================================================
*/

/*
======================
=
= CAL_SetupGrFile
=
======================
*/

static void CAL_SetupGrFile()
{
	char fname[13];
	int handle;
	byte *grtemp;
	int i;

/* load vgadict.ext (huffman dictionary for graphics files) */
	strcpy(fname, gdictname);
	strcat(fname, extension);

	handle = OpenRead(fname);
	if (handle == -1)
		CA_CannotOpen(fname);

	for (i = 0; i < 256; i++) {
		grhuffman[i].bit0 = ReadInt16(handle);
		grhuffman[i].bit1 = ReadInt16(handle);
	}

	CloseRead(handle);

/* load the data offsets from vgahead.ext */
	MM_GetPtr((memptr)&grstarts, (NUMCHUNKS+1)*4);
	MM_GetPtr((memptr)&grtemp, (NUMCHUNKS+1)*3);

	strcpy(fname, gheadname);
	strcat(fname, extension);

	handle = OpenRead(fname);
	if (handle == -1)
		CA_CannotOpen(fname);

	ReadBytes(handle, grtemp, (NUMCHUNKS+1)*3);

	for (i = 0; i < NUMCHUNKS+1; i++)
		grstarts[i] = (grtemp[i*3+0]<<0)|(grtemp[i*3+1]<<8)|(grtemp[i*3+2]<<16);

	MM_FreePtr((memptr)&grtemp);

	CloseRead(handle);

/* Open the graphics file, leaving it open until the game is finished */
	strcpy(fname, gfilename);
	strcat(fname, extension);

	grhandle = OpenRead(fname);
	if (grhandle == -1)
		CA_CannotOpen(fname);

/* load the pic headers into pictable */
	CA_CacheGrChunk(STRUCTPIC);

	grtemp = grsegs[STRUCTPIC];
	for (i = 0; i < NUMPICS; i++) {
		pictable[i].width = grtemp[i*4+0] | (grtemp[i*4+1] << 8);
		pictable[i].height = grtemp[i*4+2] | (grtemp[i*4+3] << 8);
	}

	CA_UnCacheGrChunk(STRUCTPIC);
}

/* ======================================================================== */

/*
======================
=
= CAL_SetupMapFile
=
======================
*/

static void CAL_SetupMapFile()
{
	int i;
	int handle;
	long pos;
	char fname[13];

	strcpy(fname, mheadname);
	strcat(fname, extension);

	handle = OpenRead(fname);
	if (handle == -1)
		CA_CannotOpen(fname);

	RLEWtag = ReadInt16(handle);

/* open the data file */
	strcpy(fname, gmapsname);
	strcat(fname, extension);

	maphandle = OpenRead(fname);
	if (maphandle == -1)
		CA_CannotOpen(fname);

/* load all map header */
	for (i = 0; i < NUMMAPS; i++)
	{
		pos = ReadInt32(handle);
		if (pos == 0) {
			mapheaderseg[i] = NULL;
			continue;
		}

		MM_GetPtr((memptr)&mapheaderseg[i], sizeof(maptype));
		MM_SetLock((memptr)&mapheaderseg[i], true);

		ReadSeek(maphandle, pos, SEEK_SET);

		mapheaderseg[i]->planestart[0] = ReadInt32(maphandle);
		mapheaderseg[i]->planestart[1] = ReadInt32(maphandle);
		mapheaderseg[i]->planestart[2] = ReadInt32(maphandle);

		mapheaderseg[i]->planelength[0] = ReadInt16(maphandle);
		mapheaderseg[i]->planelength[1] = ReadInt16(maphandle);
		mapheaderseg[i]->planelength[2] = ReadInt16(maphandle);
		mapheaderseg[i]->width = ReadInt16(maphandle);
		mapheaderseg[i]->height = ReadInt16(maphandle);
		ReadBytes(maphandle, (byte *)mapheaderseg[i]->name, 16);
	}

	CloseRead(handle);

/* allocate space for 2 64*64 planes */
	for (i = 0;i < MAPPLANES; i++) {
		MM_GetPtr((memptr)&mapsegs[i], 64*64*2);
		MM_SetLock((memptr)&mapsegs[i], true);
	}
}


/* ======================================================================== */

/*
======================
=
= CAL_SetupAudioFile
=
======================
*/

static void CAL_SetupAudioFile()
{
	int handle;
	long length;
	char fname[13];
	int i;

	strcpy(fname, aheadname);
	strcat(fname, extension);

	handle = OpenRead(fname);
	if (handle == -1)
		CA_CannotOpen(fname);

	length = ReadLength(handle);

	MM_GetPtr((memptr)&audiostarts, length);

	for (i = 0; i < (length/4); i++)
		audiostarts[i] = ReadInt32(handle);

	CloseRead(handle);

/* open the data file */

	strcpy(fname, afilename);
	strcat(fname, extension);

	audiohandle = OpenRead(fname);
	if (audiohandle == -1)
		CA_CannotOpen(fname);
}

/* ======================================================================== */

/*
======================
=
= CA_Startup
=
= Open all files and load in headers
=
======================
*/

void CA_Startup()
{
	CAL_SetupMapFile();
	CAL_SetupGrFile();
	CAL_SetupAudioFile();

	mapon = -1;
}

/*
======================
=
= CA_Shutdown
=
= Closes all files
=
======================
*/

void CA_Shutdown()
{
	CloseRead(maphandle);
	CloseRead(grhandle);
	CloseRead(audiohandle);
}

/* ======================================================================== */

/*
======================
=
= CA_CacheAudioChunk
=
======================
*/

void CA_CacheAudioChunk(int chunk)
{
	int pos, length;

	if (audiosegs[chunk])
		return;

	pos = audiostarts[chunk];
	length = audiostarts[chunk+1]-pos;

	ReadSeek(audiohandle, pos, SEEK_SET);

	MM_GetPtr((memptr)&audiosegs[chunk], length);

	ReadBytes(audiohandle, audiosegs[chunk], length);
}

void CA_UnCacheAudioChunk(int chunk)
{
	if (audiosegs[chunk] == 0) {
		fprintf(stderr, "Trying to free null audio chunk %d!\n", chunk);
		return;
	}

	MM_FreePtr((memptr *)&audiosegs[chunk]);
	audiosegs[chunk] = 0;
}

/*
======================
=
= CA_LoadAllSounds
=
======================
*/

void CA_LoadAllSounds()
{
	int start, i;

	for (start = STARTADLIBSOUNDS, i = 0; i < NUMSOUNDS; i++, start++)
		CA_CacheAudioChunk(start);
}

/* ======================================================================== */

/*
======================
=
= CAL_ExpandGrChunk
=
= Does whatever is needed with a pointer to a compressed chunk
=
======================
*/

static void CAL_ExpandGrChunk(int chunk, const byte *source)
{
	int tilecount = 0, i;
	long expanded = 0;

	int width = 0, height = 0;

	if (chunk >= STARTTILE8 && chunk < STARTEXTERNS)
	{
	/* expanded sizes of tile8 are implicit */
		expanded = 8*8*NUMTILE8;
		width = 8;
		height = 8;
		tilecount = NUMTILE8;
	} else if (chunk >= STARTPICS && chunk < STARTTILE8) {
		width = pictable[chunk - STARTPICS].width;
		height = pictable[chunk - STARTPICS].height;
		expanded = source[0]|(source[1]<<8)|(source[2]<<16)|(source[3]<<24);
		source += 4;
	} else {
	/* everything else has an explicit size longword */
		expanded = source[0]|(source[1]<<8)|(source[2]<<16)|(source[3]<<24);
		source += 4;
	}

/* allocate final space and decompress it */
	MM_GetPtr((void *)&grsegs[chunk], expanded);
	CAL_HuffExpand(source, grsegs[chunk], expanded, grhuffman);
	if (width && height) {
		if (tilecount) {
			for (i = 0; i < tilecount; i++)
				VL_DeModeXize(grsegs[chunk]+(width*height)*i, width, height);
		} else
			VL_DeModeXize(grsegs[chunk], width, height);
	}
}

/*
======================
=
= CA_CacheGrChunk
=
= Makes sure a given chunk is in memory, loadiing it if needed
=
======================
*/

void CA_CacheGrChunk(int chunk)
{
	long pos, compressed;
	byte *source;

	if (grhandle == -1)
		return;

	if (grsegs[chunk]) {
		return;
	}

/* load the chunk into a buffer */
	pos = grstarts[chunk];

	compressed = grstarts[chunk+1]-pos;

	ReadSeek(grhandle, pos, SEEK_SET);

	MM_GetPtr((memptr)&source, compressed);
	ReadBytes(grhandle, source, compressed);

	CAL_ExpandGrChunk(chunk, source);

	MM_FreePtr((memptr)&source);
}

void CA_UnCacheGrChunk(int chunk)
{
	if (grsegs[chunk] == 0) {
		fprintf(stderr, "Trying to free null pointer %d!\n", chunk);
		return;
	}

	MM_FreePtr((memptr)&grsegs[chunk]);

	grsegs[chunk] = NULL;
}

/* ======================================================================== */

/*
======================
=
= CA_CacheMap
=
======================
*/

void CA_CacheMap(int mapnum)
{
	long pos,compressed;
	int plane;
	byte *source;
	memptr buffer2seg;
	long expanded;

	mapon = mapnum;

/* load the planes into the already allocated buffers */

	for (plane = 0; plane < MAPPLANES; plane++)
	{
		pos = mapheaderseg[mapnum]->planestart[plane];
		compressed = mapheaderseg[mapnum]->planelength[plane];

		ReadSeek(maphandle, pos, SEEK_SET);

		MM_GetPtr((void *)&source, compressed);

		ReadBytes(maphandle, (byte *)source, compressed);

		expanded = source[0] | (source[1] << 8);

		MM_GetPtr(&buffer2seg, expanded);

/* NOTE: CarmackExpand implicitly fixes endianness, a RLEW'd only map
         would (likely) need to be swapped in CA_RLEWexpand

         Wolfenstein 3D/Spear of Destiny maps are always Carmack'd so this
         case is OK.  CA_RLEWexpand would need to be adjusted for Blake Stone
         and the like.
*/
		CAL_CarmackExpand(source+2, (word *)buffer2seg, expanded);
		MM_FreePtr((void *)&source);

		expanded = 64*64*2;
		CA_RLEWexpand(((word *)buffer2seg)+1, mapsegs[plane], expanded, RLEWtag);
		MM_FreePtr(&buffer2seg);
	}
}

/* ======================================================================== */

void MM_Startup()
{
}

void MM_Shutdown()
{
}

void MM_GetPtr(memptr *baseptr, unsigned long size)
{
	/* add some sort of linked list for purging */
	*baseptr = malloc(size);
}

void MM_FreePtr(memptr *baseptr)
{
	/* add some sort of linked list for purging, etc */
	free(*baseptr);
}

void MM_SetPurge(memptr *baseptr, int purge)
{
}

void MM_SetLock(memptr *baseptr, boolean locked)
{
}

void MM_SortMem()
{
}

static boolean PMStarted;

static int PageFile = -1;
int ChunksInFile, PMSpriteStart, PMSoundStart;

PageListStruct *PMPages;

static void PML_ReadFromFile(byte *buf, long offset, word length)
{
	if (!buf)
		Quit("PML_ReadFromFile: Null pointer");
	if (!offset)
		Quit("PML_ReadFromFile: Zero offset");

	if (ReadSeek(PageFile, offset, SEEK_SET) != offset)
	{
		// PageFile not accessale.. create a new
		char fname[13];
		int tempFile;
		strcpy(fname, pfilename);
		strcat(fname, extension);

		tempFile = OpenRead(fname);
		if(tempFile == -1)
			Quit("PML_ReadFromFile: Seek failed");
		if(ReadSeek(tempFile, offset, SEEK_SET) != offset)
		{
			Quit("Seek failed");
		}
		if (ReadBytes(tempFile, buf, length) != length)
			Quit("PML_ReadFromFile: Read failed");

		CloseRead(tempFile);
		return;

	}

	if (ReadBytes(PageFile, buf, length) != length)
		Quit("PML_ReadFromFile: Read failed");
}

static void PML_OpenPageFile()
{
	int i;
	PageListStruct *page;
	char fname[13];

	strcpy(fname, pfilename);
	strcat(fname, extension);

	PageFile = OpenRead(fname);
	if (PageFile == -1)
		Quit("PML_OpenPageFile: Unable to open page file");

	/* Read in header variables */
	ChunksInFile = ReadInt16(PageFile);
	PMSpriteStart = ReadInt16(PageFile);
	PMSoundStart = ReadInt16(PageFile);

	/* Allocate and clear the page list */
	MM_GetPtr((memptr)&PMPages, sizeof(PageListStruct) * ChunksInFile);
	MM_SetLock((memptr)&PMPages, true);

	memset(PMPages, 0, sizeof(PageListStruct) * ChunksInFile);

	/* Read in the chunk offsets */
	for (i = 0, page = PMPages; i < ChunksInFile; i++, page++) {
		page->offset = ReadInt32(PageFile);
	}

	/* Read in the chunk lengths */
	for (i = 0, page = PMPages; i < ChunksInFile; i++, page++) {
		page->length = ReadInt16(PageFile);
	}
}

static void PML_ClosePageFile()
{
	if (PageFile != -1)
		CloseRead(PageFile);

	if (PMPages) {
		int i;

		for (i = 0; i < ChunksInFile; i++) {
			PageListStruct *page;

			page = &PMPages[i];
			if (page->addr != NULL) {
				MM_FreePtr((memptr)&page->addr);
			}
		}

		MM_SetLock((memptr)&PMPages,false);
		MM_FreePtr((memptr)&PMPages);
	}
}

memptr PM_GetPage(int pagenum)
{
	PageListStruct *page;

	if (pagenum >= ChunksInFile)
		Quit("PM_GetPage: Invalid page request");

	page = &PMPages[pagenum];
	if (page->addr == NULL) {
		MM_GetPtr((memptr)&page->addr, PMPageSize);
		PML_ReadFromFile(page->addr, page->offset, page->length);
	}
	return page->addr;
}

void PM_FreePage(int pagenum)
{
	PageListStruct *page;

	if (pagenum >= ChunksInFile)
		Quit("PM_FreePage: Invalid page request");

	page = &PMPages[pagenum];
	if (page->addr != NULL) {
		MM_FreePtr((memptr)&page->addr);
		page->addr = NULL;
	}
}

void PM_Startup()
{
	if (PMStarted)
		return;

	PML_OpenPageFile();

	PMStarted = true;
}

void PM_Shutdown()
{
	if (!PMStarted)
		return;

	PML_ClosePageFile();
}
