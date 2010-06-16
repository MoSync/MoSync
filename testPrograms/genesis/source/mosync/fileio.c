#include "shared.h"
#include "MAHeaders.h"
/*
    Load a normal file, or ZIP/GZ archive.
    Returns NULL if an error occured.
*/
uint8 *load_archive(char *filename, int *file_size)
{
    int size = 0;
    uint8 *buf = NULL;
	
	size = maGetDataSize(R_GAME);
	buf = (uint8*)malloc(size);
	maReadData(R_GAME, buf, 0, size);
	
	*file_size = size;
	return buf;
}

