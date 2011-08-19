/* Copyright (C) 2009 Mobile Sorcery AB

This program is free software; you can redistribute it and/or modify it under
the terms of the GNU General Public License, version 2, as published by
the Free Software Foundation.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
for more details.

You should have received a copy of the GNU General Public License
along with this program; see the file COPYING.  If not, write to the Free
Software Foundation, 59 Temple Place - Suite 330, Boston, MA
02111-1307, USA.
*/

#include "File.h"
#include <maheap.h>
#include <mactype.h>

#ifndef USE_NEWLIB
#include <conprint.h>
#include <mavsprintf.h>
#else
#include <string.h>
#endif

// broken header files in linux/native
int sprintf(char *buf, const char *fmt, ...);

#define LOG(x, ...) //lprintfln

// ---------------------------------------------------------------------------------------
// Virtual file system
// Use Bundle tool to build a file system and load it as a resource
// then use void setCurrentFileSystem(MAHandle fileSystem) to set the file system
// and freeCurrentFileSystem to free the built directory tree.
// ---------------------------------------------------------------------------------------

#define VOL_TYPE_DIRECTORY 0
#define VOL_TYPE_FILE 1

typedef struct VolumeEntry_t {
	unsigned char type;
	char* name;
	int numChildren;
	struct VolumeEntry_t* children;
	int dataOffset;
	int dataLength;
} VolumeEntry;


#define BUFFER_SIZE 8*1024

#define MODE_READ (1<<0)
#define MODE_WRITE (1<<1)
#define MODE_APPEND (1<<2)

#define TYPE_READONLY 0
#define TYPE_READWRITE 1
#define TYPE_WRITEONLY 2

#define RES_EOF 1
#define RES_ERR 2

struct MA_FILE_t {
	// used for readonly
	VolumeEntry *volEntry;
	int filePtr;
	// ----------------------

	int bufferStart;
	unsigned char *buffer;
	int bufferSize;

	int type;
	int modeFlags;
	
	// when (mode&MODE_WRITE)!=0
	MAHandle store;

	int resultFlags;
};

#define BIG_ENDIAN 0
#define LITTLE_ENDIAN 1

static int sWrongEndian = 0;
#define FLIP_TO_ENDIAN_SHORT(x) \
	if(sWrongEndian) x = (((x&0x00ff)<<8)|((x&0xff00)>>8)

#define FLIP_TO_ENDIAN_INT(x) \
	if(sWrongEndian) x = (((x&0x000000ff)<<24)|((x&0x0000ff00)<<8)|((x&0xff000000)>>24)|((x&0x00ff0000)>>8));

typedef struct {
	int magic;
	int startOfVolumes;
	int startOfData;
} BundleHeader;

static MAHandle sCurrentFileSystem = 0;
static VolumeEntry* sRoot = NULL;
static BundleHeader sHeader;

static int sCaseSensitive = 0;

static int readString(MAHandle fileSystem, int *offset, char **outString) {
	char tempString[255];
	char *c = tempString;
	char lastByte;
	int i = 0;
	do {
		maReadData(fileSystem, c, (*offset)++, 1);	
		lastByte = *c;
		c++;
		i++;
	} while(lastByte);

	*outString = (char*)malloc(i);
	memcpy(*outString, tempString, i);
	return i;
}

static VolumeEntry *findFileRecursively(const char *filename, VolumeEntry *root) {
	int endOfChild = 0;
	VolumeEntry *findFile = NULL;
	int i;
	const char *c = filename;
	if(filename[0]=='.' && (filename[1]=='\\' || filename[1]=='/')) {
		filename+=2;
		c+=2;
	}

	LOG("fFR filename: '%s', type: %i", root->name, root->type);
	if(root->type == VOL_TYPE_FILE) {
		if(sCaseSensitive) {
			if(strcmp(filename, root->name)==0)
				return root;
		} else {
			const char *a = filename;
			const char *b = root->name;
			while(*a && *b) {
				if(toupper((int)*a)!=toupper((int)*b)) return NULL;
				a++;
				b++;
			}
			if(*a=='\0' && *b=='\0')
				return root;
			return NULL;
		}
	}

	while(1) {
		if((*c)=='\\' || (*c)=='/') break;
		else if((*c)==0) break;
		endOfChild++;
		c++;
	}
	c++;
	if((*c)==0) {
		if(strncmp(root->name, filename, endOfChild)==0)
			return root;
		else
			return NULL;
	}
	else if(strncmp(root->name, filename, endOfChild)==0) {
		for(i = 0; i < root->numChildren; i++) {
			findFile = findFileRecursively(c, &root->children[i]);
			if(findFile) {
				return findFile;
			}
		}
		return NULL;
	}
	else
		return NULL;
}

static VolumeEntry* findFile(const char *filename, VolumeEntry *root) {
	int i;

	LOG("%i children", root->numChildren);
	
//	if(root->numChildren != 1) maPanic(0, "Invalid file system root");
//	root = &root->children[0];

	for(i = 0; i < root->numChildren; i++) {
		VolumeEntry *ve = NULL;
		LOG("fFR %i", i);
		ve = findFileRecursively(filename, &root->children[i]);
		
		if(ve) {
			return ve;
		}
	}
	return NULL;
}

#if 0
static void printVolumeEntriesRecursively(VolumeEntry *vol, int level) {
	int i;
	if(vol->numChildren==0) {
		printf("%s %d %d", vol->name, vol->dataOffset, vol->dataLength);	
		return;
	}
	printf("+%s", vol->name);
	for(i = 0; i < vol->numChildren; i++) {
		printVolumeEntriesRecursively(&vol->children[i], level+1);
	}
	printf("-%s", vol->name);
}
#endif

static void freeVolumeEntriesRecursively(VolumeEntry *vol) {
	int i;
	if(vol->type==VOL_TYPE_FILE) {
		return;
	}
	for(i = 0; i < vol->numChildren; i++) {
		freeVolumeEntriesRecursively(&vol->children[i]);
	}
	if(vol->children) free(vol->children);
}

static void readVolumeEntriesRecursively(MAHandle fileSystem, int *offset, VolumeEntry *vol) {
	int child;

	// read type
	maReadData(fileSystem, &vol->type, *offset, 1);
	(*offset)++;

	// read name
	readString(fileSystem, offset, &vol->name);

	switch(vol->type) {
		case VOL_TYPE_DIRECTORY: // directory 
			maReadData(fileSystem, &vol->numChildren, *offset, 4);
			(*offset)+=4;
			FLIP_TO_ENDIAN_INT(vol->numChildren);
			
			vol->children = (VolumeEntry*)malloc(sizeof(VolumeEntry)*vol->numChildren);
			for(child = 0; child < vol->numChildren; child++) {
				readVolumeEntriesRecursively(fileSystem, offset, &vol->children[child]);				
			}

			break;
		case VOL_TYPE_FILE: // file
			maReadData(fileSystem, &vol->dataOffset, *offset, 4);
			(*offset)+=4;
			FLIP_TO_ENDIAN_INT(vol->dataOffset);
			vol->dataOffset += sHeader.startOfData;

			maReadData(fileSystem, &vol->dataLength, *offset, 4);
			(*offset)+=4;
			FLIP_TO_ENDIAN_INT(vol->dataLength);

			vol->children = 0;
			vol->numChildren = 0;

			break;
	}
}

static void buildDirectoryTree(MAHandle fileSystem) {
	int offset;

	maReadData(fileSystem, &sHeader, 0, sizeof(BundleHeader));

	LOG("0x%08x", sHeader.magic);
	if(sHeader.magic != 0x12345678) {
		// wrong endian, needs flipping;
		sWrongEndian = 1;
		LOG("endian flip!");
	} else {
		sWrongEndian = 0;
	}

	FLIP_TO_ENDIAN_INT(sHeader.startOfVolumes);
	FLIP_TO_ENDIAN_INT(sHeader.startOfData);
	offset = sHeader.startOfVolumes;
	
	sRoot = (VolumeEntry*)malloc(sizeof(VolumeEntry));
	readVolumeEntriesRecursively(fileSystem, &offset, sRoot);
	
//	printVolumeEntriesRecursively(sRoot, 0);
}

void freeCurrentFileSystem(void) {
	if(sRoot) {
		freeVolumeEntriesRecursively(sRoot);
		sRoot = NULL;
	}
}

void setCurrentFileSystem(MAHandle fileSystem, int caseSensitive) {
	freeCurrentFileSystem();
	buildDirectoryTree(fileSystem);
	sCurrentFileSystem = fileSystem;
	sCaseSensitive = caseSensitive;
}

// ---------------------------------------------------------------------------------------

// dont know about these yet
/*
int remove ( const char * filename ) {
	return 0;
}

int rename ( const char * oldname, const char * newname ) {
	return 0;
}

FILE * tmpfile ( void ) {
	return NULL;
}

char * tmpnam ( char * str ) {
	return NULL;
};
*/
//------
static void filterStoreName(char *dst, int max, const char *filename) {
	while(*filename) {
		if(*filename=='\\' || *filename=='/' || *filename=='%') {
			dst+=sprintf(dst, "%%%02x", (int)*filename++);
		}
		else 
			*dst++ = *filename++;
	}
	*dst++ = 0;
}

static int getModeFlags(const char *mode) {
	int modeflags = 0;
	while(*mode) {
		if(*mode == 'r') {
			modeflags |= MODE_READ;
		} else if(*mode =='w') {
			modeflags |= MODE_WRITE;
		} else if(*mode =='a') {
			modeflags |= MODE_WRITE|MODE_APPEND;
		} else if(*mode == '+') {
			modeflags |= MODE_READ|MODE_WRITE;
		} 
		mode++;
	}
	return modeflags;
}

static BOOL storeExists(const char *filename) {
	char sfilename[1024];
	MAHandle store;
	filterStoreName(sfilename, 1024, filename);
	store = maOpenStore(sfilename, 0);
//	lprintfln("store: %d, '%s'", store, sfilename);
	if(store>0) maCloseStore(store, 0);
	//return store!=STERR_NONEXISTENT;
	return store>0;
}


static MA_FILE* openReadWrite(const char *filename, int modeFlags) {
	MA_FILE *file;
	char sfilename[1024];
	MAHandle store;
	MAHandle data=0;
	int size = BUFFER_SIZE;
	int realLength = 0;

	filterStoreName(sfilename, 1024, filename);
	store = maOpenStore(sfilename, MAS_CREATE_IF_NECESSARY);
	if(!store) return NULL;

	file = (MA_FILE*) malloc(sizeof(MA_FILE));
	if(modeFlags&(MODE_READ|MODE_WRITE)) 
		file->type = TYPE_READWRITE;
	else 
		file->type = TYPE_WRITEONLY;
	file->modeFlags = modeFlags;
	
	if(modeFlags&(MODE_READ|MODE_APPEND)) {
		data = maCreatePlaceholder();
		maReadStore(store, data);
		realLength = size = maGetDataSize(data);
	}

	file->buffer = (unsigned char*) malloc(size);
	if(!file->buffer) {
		free(file->buffer);
		free(file);
		return NULL;
	}

	file->bufferStart = 0;
	file->bufferSize = size;
	file->volEntry = (VolumeEntry*) malloc(sizeof(VolumeEntry));
	file->filePtr = 0;
	file->volEntry->dataOffset = 0;
	file->volEntry->dataLength = realLength;
	file->resultFlags = 0;

	if(modeFlags&(MODE_READ|MODE_APPEND)) {
		maReadData(data, file->buffer, 0, size);
		maDestroyObject(data);
		if(modeFlags&MODE_APPEND) {
			file->filePtr = size;
		}
	}

	file->store = store;
	file->volEntry->type=VOL_TYPE_FILE;

	return file;
}

static MA_FILE* openRead(const char *filename, int modeFlags) {
	VolumeEntry* volEntry;
	MA_FILE *file;

	if(!sRoot) {
		LOG("filesystem not initialized");
		return NULL;
	}

	volEntry = findFile(filename, sRoot);
	if(!volEntry) {
		LOG("couldn't find file");
		return NULL;
	}

	file = (MA_FILE*) malloc(sizeof(MA_FILE));
	file->volEntry = volEntry;
	file->filePtr = volEntry->dataOffset;
	file->modeFlags = modeFlags;
	file->type = TYPE_READONLY;

	file->buffer = (unsigned char*) malloc(BUFFER_SIZE);
	file->bufferSize = BUFFER_SIZE;
	file->bufferStart = 0x7fffffff;
	file->resultFlags = 0;
	//maReadData(currentFileSystem, file->data, file->volEntry->dataOffset, file->volEntry->dataLength);
	//maReadData(currentFileSystem, file->buffer, file->volEntry->DataOffset, BUFFER_SIZE);

 	return file;
}

MA_FILE * MA_fopen ( const char * filename, const char * mode ) {
	int modeFlags = getModeFlags(mode);
	LOG("fopen('%s', '%s')", filename, mode);
	if(!modeFlags) {
		LOG("no modeflags");
		return NULL;
	}
	if(storeExists(filename) || (modeFlags&MODE_WRITE)) {
		LOG("attempt openReadWrite");
		return openReadWrite(filename, modeFlags);
	} else {
		LOG("attempt openRead");
		return openRead(filename, modeFlags);
	}
}

#define MIN(x, y) ((x)<(y)?(x):(y))

static void readFromBuffer(void *ptr, MA_FILE *stream, int bytesToRead) { 
	unsigned char *dst = (unsigned char*)ptr;

	while(bytesToRead>0) {
		int startInBuffer = stream->filePtr - stream->bufferStart;
		int bytesToReadFromBuffer = MIN((stream->bufferSize - (startInBuffer)), bytesToRead);

		if(startInBuffer<0 || bytesToReadFromBuffer<=0)
		{	
			int bytesToBuffer;
			if(bytesToRead>stream->bufferSize)
			{
				maReadData(sCurrentFileSystem, dst, stream->filePtr, bytesToRead);
				stream->filePtr+=bytesToRead;
				return;
			}
			bytesToBuffer = MIN((stream->volEntry->dataOffset+stream->volEntry->dataLength) - stream->filePtr,
				stream->bufferSize);
			maReadData(sCurrentFileSystem, stream->buffer, stream->filePtr, bytesToBuffer);
			stream->bufferStart = stream->filePtr;
		} else {
			if(bytesToReadFromBuffer<17) {
// if less than 17 bytes are read, then we do an unrolled read (no need for calls nor loops)
#define CASE(num) case num: *dst++=stream->buffer[startInBuffer++];
				switch(bytesToReadFromBuffer) {
						CASE(16)
						CASE(15)
						CASE(14)
						CASE(13)
						CASE(12)
						CASE(11)
						CASE(10)
						CASE(9)
						CASE(8)
						CASE(7)
						CASE(6)
						CASE(5)
						CASE(4)
						CASE(3)
						CASE(2)
						CASE(1)
						default:
						break;
				}
			} else {
				memcpy(dst, &stream->buffer[startInBuffer], bytesToReadFromBuffer);
				dst+=bytesToReadFromBuffer;
			}
			stream->filePtr+=bytesToReadFromBuffer;
			bytesToRead-=bytesToReadFromBuffer;
			
		}
	}

	//maReadData(currentFileSystem, ptr, stream->filePtr, bytesToRead);
}


size_t MA_fread ( void * ptr, size_t size, size_t count, MA_FILE * stream) {
	VolumeEntry *vol = stream->volEntry;
	int bytesToRead;
	LOG("fread(%x, %d, %d, %x)", (int)ptr, (int)size, (int)count, (int)stream);
	if(stream->type==TYPE_WRITEONLY || vol->type!=VOL_TYPE_FILE) {
		stream->resultFlags|=RES_ERR;
		return EOF;
	}

	bytesToRead = size*count;
	if(bytesToRead<=0) {
		stream->resultFlags|=RES_ERR;
		return EOF;
	}

	if(stream->filePtr+bytesToRead > vol->dataOffset+vol->dataLength) {
		bytesToRead -= ((stream->filePtr+bytesToRead) - 
			(vol->dataOffset+vol->dataLength));

		// round to element size
		count = (bytesToRead/size);
		bytesToRead = count*size;

		stream->resultFlags|=RES_EOF;
		if(bytesToRead<=0) {
			stream->resultFlags|=RES_ERR;
			return EOF;
		}
	}

	//maReadData(currentFileSystem, ptr, stream->filePtr, bytesToRead);
	readFromBuffer(ptr, stream, bytesToRead); 
	//stream->filePtr+=bytesToRead;
	return count; 
}

size_t MA_fwrite ( const void * ptr, size_t size, size_t count, MA_FILE * stream) {
	VolumeEntry *vol = stream->volEntry;
	int bytesToWrite;
	LOG("fwrite(%x, %d, %d, %x)", (int)ptr, (int)size, (int)count, (int)stream);
	if(stream->type==TYPE_READONLY || vol->type!=VOL_TYPE_FILE) return -1;
	
	bytesToWrite = size*count;
	if(bytesToWrite <= 0) {
		stream->resultFlags|=RES_ERR;
		return EOF;
	}
	if(stream->filePtr+bytesToWrite > vol->dataOffset+stream->bufferSize) {
		byte *newBuffer;
		int lastBufferSize = stream->bufferSize;
		while(stream->filePtr+bytesToWrite >  vol->dataOffset+stream->bufferSize) stream->bufferSize<<=1;
		newBuffer = (byte*)malloc(stream->bufferSize);
		if(!newBuffer) {
			stream->resultFlags|=RES_ERR;
			return EOF;
		}
		memcpy(newBuffer, stream->buffer, lastBufferSize);
		free(stream->buffer);
		stream->buffer = newBuffer;
	}

	memcpy(&stream->buffer[stream->filePtr], ptr, bytesToWrite);
	stream->filePtr+=bytesToWrite;
	
	if(stream->filePtr>stream->volEntry->dataLength) {
		vol->dataLength = stream->filePtr;
	}

	return count; 
}


int MA_feof ( MA_FILE * stream ) {
	LOG("feof(%x)", (int)stream);
	return stream->resultFlags&RES_EOF;
}

int MA_ferror ( MA_FILE * stream ) {
	LOG("ferror(%x)", (int)stream);
	return stream->resultFlags&RES_ERR;
}


int MA_fclose ( MA_FILE * stream ) {
	LOG("fclose(%x)", (int)stream);
	if(stream) {
		if(stream->modeFlags&MODE_WRITE) {
			MAHandle data = maCreatePlaceholder();
			maCreateData(data, stream->volEntry->dataLength);
			maWriteData(data, stream->buffer, 0, stream->volEntry->dataLength);
			maWriteStore(stream->store, data);
			maDestroyObject(data);
			maCloseStore(stream->store, 0);
			free(stream->volEntry);
		}

		free(stream->buffer);
		free(stream);
		return 0;
	}
	else {
		stream->resultFlags|=RES_ERR;
		return EOF;
	}
}

int MA_fseek ( MA_FILE * stream, long int offset, int origin ) {
	int newOffset;
	int dataOffset = stream->volEntry->dataOffset;
	int dataLength = stream->volEntry->dataLength;
	LOG("fseek(%x, %d, %d)", (int)stream, (int)offset, origin);
	stream->resultFlags&=~RES_EOF;
	switch(origin) {
		case SEEK_SET: 
			newOffset = dataOffset+offset; 
			break;
		case SEEK_CUR: 
			newOffset = stream->filePtr+offset; 
			break;
		case SEEK_END: 
			newOffset = dataOffset+dataLength+offset; 
			break;
		default:
			return 1;
	}

	if(newOffset>=dataOffset && newOffset<=dataOffset+dataLength) {
		stream->filePtr = newOffset;
		return 0;
	} 

	return 1;
}

void MA_rewind( MA_FILE *stream ) {
	LOG("rewind(%x)", (int)stream);
	stream->resultFlags&=~RES_ERR;
	MA_fseek ( stream , 0L , SEEK_SET );
}

int MA_fgetc ( MA_FILE * stream ) {
	char c;
	LOG("fgetc(%x)", (int)stream);
	if(MA_fread(&c, 1, 1, stream) == (size_t)EOF) return EOF;
	else return (int)c;
}

char * MA_fgets ( char * str, int num, MA_FILE * stream ) {
	char *str_ptr;
	int eofReached;
	LOG("fgets(%x, %d, %x)", (int)str, num, (int)stream);
	num-=1;
	if(num<=0) return NULL; 
	str_ptr = str;
	eofReached = 0;
	while(num--) {
		int c = MA_fgetc(stream);
		if((c==0) || (c=='\n') || (c == EOF)) {
			eofReached = 1;
			break;
		}
		*str_ptr++ = (char)c;
	}
	if(str_ptr==str && eofReached) return NULL;
	*str_ptr++ = 0;
	return str;
}

int MA_getc ( MA_FILE * stream ) {
	char c;
	LOG("getc(%x)", (int)stream);
	if(MA_fread(&c, 1, 1, stream) == (size_t)EOF) return EOF;
	else return (int)c;
}

int MA_fputc ( int character, MA_FILE * stream ) {
	char c = (char)character;
	LOG("fputc(%c, %x)", character, (int)stream);
	if(fwrite(&c, 1, 1, stream)!=1) {
		return EOF;
	}
	else return character;
}

int MA_fputs ( const char * str, MA_FILE * stream ) {
	LOG("fputs('%s', %x)", str, (int)stream);
	while(*str) {
		if(fputc(*str++, stream)==EOF) {
			return EOF;
		}
	}
	return 1;
}

int MA_putc ( int character, MA_FILE * stream ) {
	char c = (char)character;
	LOG("fputc(%c, %x)", character, (int)stream);
	if(fwrite(&c, 1, 1, stream)!=1) {
		return EOF;
	}
	else return character;
}


long int MA_ftell ( MA_FILE * stream ) {
	LOG("fputc(%x)", (int)stream);
	return (stream->filePtr-stream->volEntry->dataOffset);
}

int MA_fgetpos ( MA_FILE * stream, fpos_t * position ) {
	LOG("fgetpos(%x, %x)", (int)stream, (int)position);
	*position = (fpos_t)(stream->filePtr-stream->volEntry->dataOffset);
	return 0;
}

int MA_fsetpos ( MA_FILE * stream, const fpos_t * pos ) {
	int newOffset = (int) *pos;
	//eofReached = 0;
	LOG("fsetpos(%x, %x)", (int)stream, (int)pos);
	stream->resultFlags&=~RES_EOF;
	if(newOffset>=0 && newOffset<stream->volEntry->dataLength) {
		stream->filePtr = stream->volEntry->dataOffset+newOffset;	
		return 0;
	} 
	else return 1;
}
