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

//
// Bundle.cpp : Defines the entry point for a console application
// that creates a file bundle out of a file tree. MAFS is used to
// read/unpack the bundle on the device.
//

#include "File.h"
#include <vector>
#include <cstring>
#include <stdlib.h>


// The magic number is to determine byte order
// when unpacking the bundle, and is also used
// as the version number of the bundle file.

// Version 1 magic number.
//#define MAGIC 0x12345678

// Version 2 magic number.
#define MAGIC 0x22345678

// List of input files.
std::vector<std::string> gInFiles;

// The total max size of file data is 65 MB.
// TODO: Look into using a dynamic buffer.
// Is there error checking for this?
// 65 MB may be a lot on mobile devices today,
// but perhaps not in the future ;)
#define MAX_DATA_SIZE 65536*1024

// Global memory buffer that contains all the file data.
unsigned char gFileData[MAX_DATA_SIZE];

// An offset into the gFileData buffer. Updated when reading
// files. Holds the total size of the file data when all files
// have been read.
int gFileDataPtr = 0;

int gStartOfData = 0;
FILE *gOutFile = 0;

// Header consists of four 32 bit values.
#define START_OF_VOLUME_ENTRIES 16

// Flag that controls casing of file names.
// 0 = Do not change case (default).
// 1 = File names all upper case.
// 2 = File names all lower case.
int changeCase = 0;

/*
// Documentation of data formats.

// Header format.
struct BundleHeader {
	int magic;
	int startOfVolumes;
	int startOfData;
	int checksum; // Adler32 checksum
}

struct VolumeEntryFile {
	byte volumeEntryType; // 0 = directory, 1 = file
	char *name;
	int dataOffset;
	int dataLength;
};

struct VolumeEntryDirectory {
	byte volumeEntryType; // 0 = directory, 1 = file
	char *name;
	int numVolumeEntries;
};
*/

static char to_upper(char c) {

	if(c>='a' && c<='z')
		return c-'a'+'A';
	else
		return c;
}

static char to_lower(char c) {
	if(c>='A' && c <='Z')
		return c-'A'+'a';
	else
		return c;
}

// Largest prime smaller than 65536
#define MOD_ADLER 65521

/**
 * @param data Pointer to data buffer.
 * @param length The length of the data in bytes.
 * @return The Adler32 checksum value.
 */
static unsigned long adler32(unsigned char* data, int len)
{
	unsigned long a = 1;
	unsigned long b = 0;

	for (int index = 0; index < len; ++index)
	{
		a = (a + data[index]) % MOD_ADLER;
		b = (b + a) % MOD_ADLER;
	}

	return (b << 16) | a;
}

struct VolumeEntry {
	unsigned char type;
	std::string name;
	std::vector<VolumeEntry*> children;
	int dataOffset;
	int dataLength;
};

VolumeEntry *g_root;

static int readFile(std::string name) {
	FILE *file = fopen(name.c_str(), "rb");
	if(!file) {
		printf("failure reading '%s'\n", name.c_str());
		exit(1);
	}

	fseek(file, 0, SEEK_END);
	int len = ftell(file);
	fseek(file, 0, SEEK_SET);
	int res = fread(&gFileData[gFileDataPtr], 1, len, file);
	if(res != len) {
		printf("failure reading '%s'\n", name.c_str());
		exit(1);
	}
	gFileDataPtr+=len;
	return len;
}

static void saveFileData() {
	gStartOfData = ftell(gOutFile);
	fwrite(gFileData, 1, gFileDataPtr, gOutFile);
}

static void writeHeader() {
	// Magic number.
	int magic = MAGIC;

	// Compute checksum.
	unsigned long checksum = adler32(gFileData, gFileDataPtr);

	// Start of the volume entries.
	int startOfVolumes = START_OF_VOLUME_ENTRIES; // sizeof(BundleHeader);

	// Move file pointer to start of file and write header fields.
	fseek(gOutFile, 0, SEEK_SET);
	fwrite(&magic, 4, 1, gOutFile);
	fwrite(&startOfVolumes, 4, 1, gOutFile);
	fwrite(&gStartOfData, 4, 1, gOutFile);
	fwrite(&checksum, 4, 1, gOutFile);
}

static void saveVolumeEntries(VolumeEntry *root) {
	fwrite(&root->type, 1, 1, gOutFile);
	int sizeOfName = root->name.size()+1;
	fwrite(root->name.c_str(), 1, sizeOfName, gOutFile);
	int numVolumeEntries;

	switch(root->type) {
		case 0:
			numVolumeEntries = root->children.size();
			fwrite(&numVolumeEntries, 4, 1, gOutFile);
			for(size_t i = 0; i < root->children.size(); i++) {
				saveVolumeEntries(root->children[i]);
			}
			break;
		case 1:
			int dataOffset = root->dataOffset;
			fwrite(&dataOffset, 4, 1, gOutFile);
			int dataLength = root->dataLength;
			fwrite(&dataLength, 4, 1, gOutFile);
			break;
	}
}

void parse(File file, VolumeEntry *vol);

static void parseDirectory(File file, VolumeEntry *vol)
{
	std::list<File> l = file.listFiles( );

	// Go through file and directories
	for ( std::list<File>::iterator it = l.begin( ); it != l.end( ); ++it )
	{
		// Skip self & backward references
		if ( it->isSelfOrBackRef( ) == true )
			continue;

		VolumeEntry *childVol = new VolumeEntry;
		vol->children.push_back(childVol);
		parse( *it, childVol );
	}

	if(file.isDirectory()) {
		printf("- %s\n", file.getName( ).c_str( ));
	}
}

void parse(File file, VolumeEntry *vol) {
	if ( file.isDirectory( ) == true )
	{
		vol->name = file.getName();

		for(size_t i = 0; i < vol->name.size(); i++)
		{
			if ( changeCase == 1 )
				vol->name[i] = to_upper( vol->name[i] );
			else if(changeCase == 2)
				vol->name[i] = to_lower( vol->name[i] );
		}

		printf("+ %s\n", vol->name.c_str());

		vol->type = 0;
		parseDirectory(file, vol);
	}
	else
	{
		//printf("\"%s\"\n", file.getName().c_str());
		vol->name = file.getName();

		for(size_t i = 0; i < vol->name.size(); i++) {
			if ( changeCase == 1 )
				vol->name[i] = to_upper( vol->name[i] );
			else if ( changeCase == 2 )
				vol->name[i] = to_lower( vol->name[i] );
		}

		printf("++ %s\n", vol->name.c_str());

		vol->type = 1;
		vol->dataOffset = gFileDataPtr;
		vol->dataLength = readFile(file.getAbsolutePath().c_str());
		return;
	}
}

int main(int argc, char **argv)
{
	// Print usage info if no args are supplied.
	if(argc<2) {
		printf(	"MAUtil::MAFS Bundle tool\n\n"
				"This tool is used to build a binary image of a folder on a desktop computer.\n\n"
				"Usage:\n"
				"bundle <parameters>\n\n"
				"Parameters:\n"
				"  -in <input file or folder> the input files or folders to add to the\n"
				"                             image (multiple -in directives may be added).\n"
				"  -out <output file>         the name of the image to be created (only one).\n"
				"  -toUpper/-toLower          change case of all file names to upper or lower\n"
				"                             case.\n\n"
				"Example:\n"
				"  bundle -in data -out anotherworld.bun -toLower\n"
				);
	}

	// Parse command params.
	for(int i = 1; i < argc; i++) {
		if(strcmp(argv[i], "-in")==0) {
			i++;
			if(i>=argc) {
				printf("not enough parameters for -in");
				return 1;
			}
			gInFiles.push_back(argv[i]);
		}
		else if(strcmp(argv[i], "-out")==0) {
			if(gOutFile) {
				printf("cannot have multiple out files.");
				return 1;
			}

			i++;
			if(i>=argc) {
				printf("not enough parameters for -out");
				return 1;
			}
			gOutFile = fopen(argv[i], "wb");
		}
		else if(strcmp(argv[i], "-toUpper")==0) {
			changeCase = 1;
		}
		else if(strcmp(argv[i], "-toLower")==0) {
			changeCase = 2;
		} else {
			printf("invalid argument");
			return 1;
		}
	}

	// Sanity checks.
	if (gInFiles.size()==0) {
		printf("No input files.");
		return 1;
	}
	if (!gOutFile) {
		printf("Could not create output file.");
		return 1;
	}

	// Buld data structure that holds files and directories.
	VolumeEntry* root = new VolumeEntry;
	root->name = "Root";
	root->type = 0; // directory

	for(size_t i = 0; i < gInFiles.size(); i++) {
		File file(gInFiles[i]);
		if(file.isDirectory()) {
			parseDirectory(file, root);
		} else {
			VolumeEntry *child = new VolumeEntry;
			root->children.push_back(child);
			parse(file, child);
		}
	}

	// Write volume entires and actual file data.
	fseek(gOutFile, START_OF_VOLUME_ENTRIES, SEEK_SET);
	saveVolumeEntries(root);
	saveFileData();

	// Write header.
	writeHeader();

	// Close output file.
	fclose(gOutFile);

	return 0;
}
