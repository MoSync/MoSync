#ifndef __VERSION_H__
#define __VERSION_H__

/*
 Change WMODE to point to the executable you would like to build:
 WL1 (Wolfenstein 3D Shareware): 0
 WL6 (Wolfenstein 3D Retail):    1
 SDM (Spear of Destiny Demo):    2
 SOD (Spear of Destiny Retail):  3

 WL3 not yet supported.
*/

/*
   The default is 0 (WL1).
   Change this to match the version you have (see above).
*/

#ifndef WMODE
#define WMODE 0
#endif

/* define WOLF92 if you have full version of Wolfenstein 3D dated 1992 */
//#define WOLF92

/* --- End of User-Modifiable Variables --- */

#if WMODE == 0
/* #define SPEAR */
/* #define SPEARDEMO */
#define UPLOAD
#define GAMENAME	"Wolfenstein 3D Shareware"
#define GAMEEXT		"wl1"
#define GAMETYPE	"WL1\0"

#elif WMODE == 1
/* #define SPEAR */
/* #define SPEARDEMO */
/* #define UPLOAD */
#define GAMENAME	"Wolfenstein 3D"
#define GAMEEXT		"wl6"
#define GAMETYPE	"WL6\0"

#elif WMODE == 2
#define SPEAR
#define SPEARDEMO
/* #define UPLOAD */
#define GAMENAME	"Spear of Destiny Demo"
#define GAMEEXT		"sdm"
#define GAMETYPE	"SDM\0"

#elif WMODE == 3
#define SPEAR
/* #define SPEARDEMO */
/* #define UPLOAD */
#define GAMENAME	"Spear of Destiny"
#define GAMEEXT		"sod"
#define GAMETYPE	"SOD\0"

#else
#error "please edit version.h and fix WMODE"
#endif

#define GAMEHDR		"WOLF3D\0\0"

#define	SAVTYPE		"SAV\0"
#define CFGTYPE		"CFG\0"

#endif
