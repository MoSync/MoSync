#ifndef __ID_HEADS_H__
#define __ID_HEADS_H__

/*
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <time.h>
#include "ff.h"
#include <ctype.h>
#include <math.h>
#include <sdl_config.h>
*/
#include <conprint.h>
#include <mastdlib.h>
#include <mastring.h>
#include "ff.h"
#include <stdint.h>
#include <madmath.h>
#include <maheap.h>
#include <MAFS/File.h>
void perror(const char *str);
//int fprintf( FILE * stream, const char * format, ... );
#define fprintf(x, y, ...) printf(y)
extern FILE *stderr;
#define exit(x) maExit(x)
#define EXIT_FAILURE 1
#define EXIT_SUCCESS 0
#define time(x) maGetMilliSecondCount()
#define labs(x) abs(x)
char *strdup(const char *s1);


#ifdef __cplusplus
typedef enum { false, true } boolean;
#else
typedef enum { false, true } boolean;
#endif

#include "version.h"

/* ------------------------------------------------------------------------ */

#ifndef SPEAR

#ifndef UPLOAD

#ifdef WOLF92
#  include "gfxv_wl6_92.h"
#else
#  include "gfxv_wl6.h"
#endif
  
#else
#include "gfxv_wl1.h"
#endif

#include "audiowl6.h"

#else /* SPEAR */

#ifndef SPEARDEMO
#include "gfxv_sod.h"
#else /* SPEARDEMO */
#include "gfxv_sdm.h"
#endif /* SPEARDEMO */

#include "audiosod.h"

#endif /* SPEAR */

/* ---------------- */


//typedef uint8_t		byte; // maapi.h defines this
typedef uint16_t	word;
typedef uint32_t	longword;
typedef uint32_t	dword;

typedef long fixed;

typedef void * memptr;

#include "misc.h"

#include "vi_comm.h"
#include "sd_comm.h"

#include "id_ca.h"
#include "id_vh.h"
#include "id_us.h"

extern const byte gamepal[];

int MS_CheckParm(const char *string);
#ifdef __cplusplus
extern "C"{
#endif
void Quit(const char *error);
#ifdef __cplusplus
}
#endif
#define TickBase	70	/* 70Hz per tick */

#undef PI
#define PI		3.1415926535897932384626433832795028841971693993751058209749445920

#define	MAXTICS		10
#define DEMOTICS	4

extern int tics;

#define mapwidth	64
#define mapheight	64

#endif
