/*
#include <sys/stat.h>
#include <sys/time.h>
#include <unistd.h>
#include <fcntl.h>
      */
#include <matime.h>
#include <MAFS/File.h>

#include "wl_def.h"
extern const char* GameDir();
#ifndef O_BINARY
#define O_BINARY 0
#endif

//static struct timeval t0;
static unsigned long tc0;
int t0;

void set_TimeCount(unsigned long t)
{
	tc0 = t;

	//gettimeofday(&t0, NULL);
	t0 = maGetMilliSecondCount();
}

unsigned long get_TimeCount(void)
{
/*
	struct timeval t1;
	long secs, usecs;
	unsigned long tc;

	gettimeofday(&t1, NULL);

	secs = t1.tv_sec - t0.tv_sec;
	usecs = t1.tv_usec - t0.tv_usec;

	if (usecs < 0) {
		usecs += 1000000;
		secs--;
	}

	tc = tc0 + secs * 70 + (usecs * 70) / 1000000;

	return tc;
	*/
	return (maGetMilliSecondCount() - t0)*70/1000;
}

long filelength(int handle)
{
	/*
	struct stat buf;
	if (fstat(handle, &buf) == -1) {
		perror("filelength");
		exit(EXIT_FAILURE);
	}

	return buf.st_size;
	*/
	int len;
	FILE *file = (FILE*)handle;
	int pos = ftell(file);
	fseek(file, 0, SEEK_END);
	len = ftell(file);
	fseek(file, pos, SEEK_SET);
	return len;
}

/*
char *strlwr(char *s)
{
	char *p = s;

	while (*p) {
		*p = tolower(*p);
		p++;
	}

	return s;
}

char *itoa(int value, char *string, int radix)
{
	(void) radix;

//	 wolf3d only uses radix 10
	sprintf(string, "%d", value);
	return string;
}

char *ltoa(long value, char *string, int radix)
{
	(void) radix;

	// wolf3d only uses radix 10
	sprintf(string, "%ld", value);
	return string;
}

char *ultoa(unsigned long value, char *string, int radix)
{
	(void) radix;

	// wolf3d only uses radix 10
	sprintf(string, "%lu", value);
	return string;
}
*/


/* from Dan Olson */
static void put_dos2ansi(byte attrib)
{
	byte fore,back,blink=0,intens=0;

	fore = attrib&15;	// bits 0-3
	back = attrib&112; // bits 4-6
       	blink = attrib&128; // bit 7

	// Fix background, blink is either on or off.
	back = back>>4;

	// Fix foreground
	if (fore > 7) {
		intens = 1;
		fore-=8;
	}

	// Convert fore/back
	switch (fore) {
		case 0: // BLACK
			fore=30;
			break;
		case 1: // BLUE
			fore=34;
			break;
		case 2: // GREEN
			fore=32;
			break;
		case 3: // CYAN
			fore=36;
			break;
		case 4: // RED
			fore=31;
			break;
		case 5: // Magenta
			fore=35;
			break;
		case 6: // BROWN(yellow)
			fore=33;
			break;
		case 7: //GRAy
			fore=37;
			break;
	}

	switch (back) {
		case 0: // BLACK
			back=40;
			break;
		case 1: // BLUE
			back=44;
			break;
		case 2: // GREEN
			back=42;
			break;
		case 3: // CYAN
			back=46;
			break;
		case 4: // RED
			back=41;
			break;
		case 5: // Magenta
			back=45;
			break;
		case 6: // BROWN(yellow)
			back=43;
			break;
		case 7: //GRAy
			back=47;
			break;
	}
	if (blink)
		printf ("%c[%d;5;%dm%c[%dm", 27, intens, fore, 27, back);
	else
		printf ("%c[%d;25;%dm%c[%dm", 27, intens, fore, 27, back);
}

void DisplayTextSplash(const byte *text, int l)
{
	int i, x;

	//printf("%02X %02X %02X %02X\n", text[0], text[1], text[2], text[3]);
	text += 4;
	//printf("%02X %02X %02X %02X\n", text[0], text[1], text[2], text[3]);
	text += 2;

	for (x = 0; x < l; x++) {
		for (i = 0; i < 160; i += 2) {
			put_dos2ansi(text[160*x+i+2]);
			if (text[160*x+i+1] && text[160*x+i+1] != 160)
				printf("%c", text[160*x+i+1]);
			else
				printf(" ");
		}
		printf("%c[m", 27);
		printf("\n");
	}
}

/* ** */
#define BYTE_ORDER LITTLE_ENDIAN
uint16_t SwapInt16L(uint16_t i)
{
#if BYTE_ORDER == BIG_ENDIAN
	return ((uint16_t)i >> 8) | ((uint16_t)i << 8);
#else
	return i;
#endif
}

uint32_t SwapInt32L(uint32_t i)
{
#if BYTE_ORDER == BIG_ENDIAN
	return	((uint32_t)(i & 0xFF000000) >> 24) |
		((uint32_t)(i & 0x00FF0000) >>  8) |
		((uint32_t)(i & 0x0000FF00) <<  8) |
		((uint32_t)(i & 0x000000FF) << 24);
#else
	return i;
#endif
}

/* ** */

int OpenWrite(const char *fn)
{
/*
	int fp;
	fp = open(fn, O_CREAT|O_WRONLY|O_TRUNC|O_BINARY, S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP|S_IROTH|S_IWOTH);
	return fp;
	*/

	int f = ((int)fopen(fn, "wb"));
	if(!f) f = -1;
	return f;
}

int OpenWriteAppend(const char *fn)
{
/*
	int fp;
	fp = open(fn, O_CREAT|O_WRONLY|O_BINARY, S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP|S_IROTH|S_IWOTH);
	return fp;
	*/
	int f = ((int)fopen(fn, "wba"));
	if(!f) f = -1;
	return f;
}

void CloseWrite(int fp)
{
	//close(fp);
	if(fp>0)
		fclose((FILE*)fp);
}

int WriteSeek(int fp, int offset, int whence)
{
	//return lseek(fp, offset, whence);
	//return fseek((FILE*)fp, offset, whence);
	if(!fseek((FILE*)fp, offset, whence)) return offset;
	else return !offset;
}

int WritePos(int fp)
{
	//return lseek(fp, 0, SEEK_CUR);
	return fseek((FILE*)fp, 0, SEEK_CUR);
}

int WriteInt8(int fp, int8_t d)
{
	//return write(fp, (const char *) &d, 1);
	return fwrite(&d, 1, 1, (FILE*)fp);
}

int WriteInt16(int fp, int16_t d)
{
	int16_t b = SwapInt16L(d);
	//return write(fp, (const char *) &b, 2) / 2;
	return fwrite(&b, 2, 1, (FILE*)fp)/2;
}

int WriteInt32(int fp, int32_t d)
{
	int32_t b = SwapInt32L(d);
	//return write(fp, (const char *)&b, 4) / 4;
	return fwrite(&b, 4, 1, (FILE*)fp)/4;
}

int WriteBytes(int fp, const byte *d, int len)
{
	//return write(fp, (const char *)d, len);
	return fwrite(d, len, 1, (FILE*)fp);
}


int OpenRead(const char *fn)
{
	//chdir(GameDir());
	//fp = open(fn, O_RDONLY | O_BINARY);
	int f = (int)fopen(fn, "rb");
	if(!f) f = -1;
	return f;
}

void CloseRead(int fp)
{
	//close(fp);
	if(fp>0)
		fclose((FILE*)fp);
}

int ReadSeek(int fp, int offset, int whence)
{
	//return lseek(fp, offset, whence);
	if(!fseek((FILE*)fp, offset, whence)) return offset;
	else return !offset;
}

int ReadLength(int fp)
{
	return filelength(fp);
}

int8_t ReadInt8(int fp)
{
	byte d[1];

//	read(fp, (char *)d, 1);
	fread(d, 1, 1, (FILE*)fp);

	return d[0];
}

int16_t ReadInt16(int fp)
{
	byte d[2];

	//read(fp, (char *)d, 2);
	fread(d, 2, 1, (FILE*)fp);

	return (d[0]) | (d[1] << 8);
}

int32_t ReadInt32(int fp)
{
	byte d[4];

	//read(fp,(char *) d, 4);
	fread(d, 4, 1, (FILE*)fp);

	return (d[0]) | (d[1] << 8) | (d[2] << 16) | (d[3] << 24);
}

int ReadBytes(int fp, byte *d, int len)
{
	//return read(fp, (char *) d, len);
	return fread(d, len, 1, (FILE*)fp);
}
