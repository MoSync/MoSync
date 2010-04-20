#include <ma.h>
#include <MAFS/File.h>
#include <mavsprintf.h>
#include <conprint.h>
#include "MAHeaders.h"

#include "wl_def.h"

FILE *stderr;

int strcasecmp(const char *s1, const char *s2) {
	return stricmp(s1, s2);
}

void perror(const char *str) {
	maPanic(0, str);
}

/*
int fprintf( FILE * stream, const char * format, ... ) {
	int len;
	char temp[1024];
	va_list ap;
	va_start(ap, format);
	len = vsprintf(temp, format, ap);
	va_end(ap);
	printf("%s", temp);
	return len;
}
*/

int MAMain() {
	vwidth = 320;
	vheight = 200;
	gConsoleLogging = 1;
	gConsoleDisplay = 0;
	setCurrentFileSystem(R_FILESYSTEM, 0);
	return WolfMain(0, NULL);
	//return 0;
}
