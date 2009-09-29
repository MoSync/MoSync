/*
#include <windows.h>
#include <windowsx.h>
#include <ddraw.h>
#include <stdio.h>
#include <string.h>
*/
#include <ma.h>
#include <mastring.h>
#include <mavsprintf.h>
#include <maheap.h>

#include "MAUtil\File.h"
#include "vecx.h"
#include "MAHeaders.h"
#include "event.h"

#ifdef _WIN32
#define einline __inline
#else
#define einline inline
#endif
/* a global string buffer user for message output */

char gbuffer[1024];

static const char *appname = "vecx";
static const char *romname = "rom.dat";

/*
static HWND                hwnd;
static LPDIRECTDRAW        lpdd;
static LPDIRECTDRAWSURFACE lpdd_primary = 0;
static DDSURFACEDESC       ddsd;
static POINT			   scr_offset = {-1, -1};
*/

static MAPoint2d			   scr_offset = {-1, -1};
static MAPoint2d			   prev_scr_offset = {-1, -1};

static long screen_x;
static long screen_y;
static long scl_factor;
static long scl_recip;

static long bytes_per_pixel;
static int color_set[VECTREX_COLORS];

enum {
	EMU_TIMER = 20 /* the emulators heart beats at 20 milliseconds */
};

static void osint_updatescale (void)
{
	long sclx, scly;

	sclx = ALG_MAX_X / screen_x;
	scly = ALG_MAX_Y / screen_y;

	if (sclx > scly) {
		scl_factor = sclx;
	} else {
		scl_factor = scly;
	}
	scl_recip = 0xffff/scl_factor;
}

static int osint_defaults (void)
{
	unsigned b;
	FILE *rom_file;

	Extent e = maGetScrSize();
	screen_x = EXTENT_X(e);
	screen_y = EXTENT_Y(e);

	osint_updatescale ();

	/* load up the rom */

	rom_file = fopen (romname, "rb");

	if (rom_file == NULL) {
		lprintfln("cannot open '%s'", romname);
		return 1;
	}

	b = fread (rom, 1, sizeof (rom), rom_file);

	if (b < sizeof (rom)) {
		lprintfln ("read %d bytes from '%s'. need %d bytes.",
			b, romname, sizeof (rom));
		return 1;
	}
	fclose (rom_file);

	/* the cart is empty by default */

	for (b = 0; b < sizeof (cart); b++) {
		cart[b] = 0;
	}

	return 0;
}

static void osint_parse_cmdline (char *cmdline)
{
	int c = 0;
	char ch;

	while ((ch = cmdline[c]) != '\0') {
		if (ch == '-') {
			/* a flag coming up */

			ch = cmdline[++c];

			if (ch == 'c' || ch == 'C') {
				int cartname_len = 0;
				char cartname[256];

				/* skip any blank space */

				do {
					ch = cmdline[++c];
				} while (ch != '\0' && isspace (ch));

				while (ch != '\0' && isspace (ch) == 0) {
					cartname[cartname_len++] = ch;
					ch = cmdline[++c];
				}

				cartname[cartname_len] = '\0';

				if (cartname_len > 0) {
					FILE *cartfile;

					cartfile = fopen (cartname, "rb");
				
					if (cartfile != NULL) {
						fread (cart, 1, sizeof (cart), cartfile);
						fclose (cartfile);
					} else {
						lprintfln(gbuffer, "cannot open '%s'", cartname);
					}
				}
			} else if (ch == 'x' || ch == 'X') {
				long sizex = 0;

				/* skip any blank space */

				do {
					ch = cmdline[++c];
				} while (ch != '\0' && isspace (ch));

				while (ch != '\0' && isdigit (ch)) {
					sizex *= 10;
					sizex += ch - '0';
					ch = cmdline[++c];
				}

				if (sizex > 10 && sizex < 4000) {
					screen_x = sizex;
					osint_updatescale ();
				}
			} else if (ch == 'y' || ch == 'Y') {
				long sizey = 0;

				/* skip any blank space */

				do {
					ch = cmdline[++c];
				} while (ch != '\0' && isspace (ch));

				while (ch != '\0' && isdigit (ch)) {
					sizey *= 10;
					sizey += ch - '0';
					ch = cmdline[++c];
				}

				if (sizey > 10 && sizey < 4000) {
					screen_y = sizey;
					osint_updatescale ();
				}
			}
		} else {
			/* skip past a character that we don't understand */

			c++;
		}
	}
}

static void osint_maskinfo (int mask, int *shift, int *precision)
{
	*shift = 0;

	while ((mask & 1L) == 0) {
		mask >>= 1;
		(*shift)++;
	}

	*precision = 0;

	while ((mask & 1L) != 0) {
		mask >>= 1;
		(*precision)++;
	}
}

static void osint_gencolors (void)
{
	int c;
	int rcomp, gcomp, bcomp;
	int rsh, rpr;
	int gsh, gpr;
	int bsh, bpr;

	bytes_per_pixel = 4;
	osint_maskinfo (0x00ff0000, &rsh, &rpr);
	osint_maskinfo (0x0000ff00, &gsh, &gpr);
	osint_maskinfo (0x000000ff, &bsh, &bpr);

	for (c = 0; c < VECTREX_COLORS; c++) {
		rcomp = c * 256 / VECTREX_COLORS;
		gcomp = c * 256 / VECTREX_COLORS;
		bcomp = c * 256 / VECTREX_COLORS;

		color_set[c] =	(( rcomp >> (8 - rpr)) << rsh) |
						(( gcomp >> (8 - gpr)) << gsh) |
						(( bcomp >> (8 - bpr)) << bsh);
	}
}

static einline void osint_clearscreen (void)
{
	maSetColor(0);
	maFillRect(0, 0, screen_x, screen_y);
}

/* draw a line with a slope between 0 and 1.
 * x is the "driving" axis. x0 < x1 and y0 < y1.
 */
/*
static void osint_linep01 (long x0, long y0, long x1, long y1, unsigned char color)
{
	long dx, dy;
	long i0, i1;
	long j, e;
	unsigned char *ptr;

	dx = x1 - x0;
	dy = y1 - y0;

	i0 = x0 / scl_factor;
	i1 = x1 / scl_factor;
	j  = y0 / scl_factor;

	e = dy * (scl_factor - (x0 % scl_factor)) -
		dx * (scl_factor - (y0 % scl_factor));

	dx *= scl_factor;
	dy *= scl_factor;

	ptr = osint_pixelptr (i0, j);

	for (; i0 <= i1; i0++) {
		memcpy (ptr, color_set + color, bytes_per_pixel);

		if (e >= 0) {
			ptr += ddsd.lPitch;
			e -= dx;
		}

		e += dy;
		ptr += bytes_per_pixel;
	}
}
*/
/* draw a line with a slope between 1 and +infinity.
 * y is the "driving" axis. y0 < y1 and x0 < x1.
 */
/*
static void osint_linep1n (long x0, long y0, long x1, long y1, unsigned char color)
{
	long dx, dy;
	long i0, i1;
	long j, e;
	unsigned char *ptr;

	dx = x1 - x0;
	dy = y1 - y0;

	i0 = y0 / scl_factor;
	i1 = y1 / scl_factor;
	j  = x0 / scl_factor;

	e = dx * (scl_factor - (y0 % scl_factor)) -
		dy * (scl_factor - (x0 % scl_factor));

	dx *= scl_factor;
	dy *= scl_factor;

	ptr = osint_pixelptr (j, i0);

	for (; i0 <= i1; i0++) {
		memcpy (ptr, color_set + color, bytes_per_pixel);

		if (e >= 0) {
			ptr += bytes_per_pixel;
			e -= dy;
		}

		e += dx;
		ptr += ddsd.lPitch;
	}
}
*/
/* draw a line with a slope between 0 and -1.
 * x is the "driving" axis. x0 < x1 and y1 < y0.
 */
/*
static void osint_linen01 (long x0, long y0, long x1, long y1, unsigned char color)
{
	long dx, dy;
	long i0, i1;
	long j, e;
	unsigned char *ptr;

	dx = x1 - x0;
	dy = y0 - y1;

	i0 = x0 / scl_factor;
	i1 = x1 / scl_factor;
	j  = y0 / scl_factor;

	e = dy * (scl_factor - (x0 % scl_factor)) -
		dx * (y0 % scl_factor);

	dx *= scl_factor;
	dy *= scl_factor;

	ptr = osint_pixelptr (i0, j);

	for (; i0 <= i1; i0++) {
		memcpy (ptr, color_set + color, bytes_per_pixel);

		if (e >= 0) {
			ptr -= ddsd.lPitch;
			e -= dx;
		}

		e += dy;
		ptr += bytes_per_pixel;
	}
}
*/
/* draw a line with a slope between -1 and -infinity.
 * y is the "driving" axis. y0 < y1 and x1 < x0.
 */
/*
static void osint_linen1n (long x0, long y0, long x1, long y1, unsigned char color)
{
	long dx, dy;
	long i0, i1;
	long j, e;
	unsigned char *ptr;

	dx = x0 - x1;
	dy = y1 - y0;

	i0 = y0 / scl_factor;
	i1 = y1 / scl_factor;
	j  = x0 / scl_factor;

	e = dx * (scl_factor - (y0 % scl_factor)) -
		dy * (x0 % scl_factor);

	dx *= scl_factor;
	dy *= scl_factor;

	ptr = osint_pixelptr (j, i0);

	for (; i0 <= i1; i0++) {
		memcpy (ptr, color_set + color, bytes_per_pixel);

		if (e >= 0) {
			ptr -= bytes_per_pixel;
			e -= dy;
		}

		e += dx;
		ptr += ddsd.lPitch;
	}
}*/

static einline void osint_line (long x0, long y0, long x1, long y1, unsigned char color)
{
	maSetColor(color_set[color]);
	maLine((x0*scl_recip)>>16, (y0*scl_recip)>>16, (x1*scl_recip)>>16, (y1*scl_recip)>>16);
	/*
	if (x1 > x0) {
		if (y1 > y0) {
			if ((x1 - x0) > (y1 - y0)) {
				osint_linep01 (x0, y0, x1, y1, color);
			} else {
				osint_linep1n (x0, y0, x1, y1, color);
			}
		} else {
			if ((x1 - x0) > (y0 - y1)) {
				osint_linen01 (x0, y0, x1, y1, color);
			} else {
				osint_linen1n (x1, y1, x0, y0, color);
			}
		}
	} else {
		if (y1 > y0) {
			if ((x0 - x1) > (y1 - y0)) {
				osint_linen01 (x1, y1, x0, y0, color);
			} else {
				osint_linen1n (x0, y0, x1, y1, color);
			}
		} else {
			if ((x0 - x1) > (y0 - y1)) {
				osint_linep01 (x1, y1, x0, y0, color);
			} else {
				osint_linep1n (x1, y1, x0, y0, color);
			}
		}
	}
	*/
}

void osint_render (void)
{
	long v;
//	POINT prev_scr_offset;
//	IDirectDrawSurface_Lock (lpdd_primary, NULL, &ddsd, DDLOCK_WAIT, NULL);

	prev_scr_offset = scr_offset;
	scr_offset.x = 0;
	scr_offset.y = 0;
//	ClientToScreen (hwnd, &scr_offset);

	if (scr_offset.x != prev_scr_offset.x ||
		scr_offset.y != prev_scr_offset.y) {
		osint_clearscreen ();
	} else {
		for (v = 0; v < vector_erse_cnt; v++) {
			if (vectors_erse[v].color != VECTREX_COLORS) {
				osint_line (vectors_erse[v].x0, vectors_erse[v].y0,
							vectors_erse[v].x1, vectors_erse[v].y1, 0);
			}
		}
	}

	for (v = 0; v < vector_draw_cnt; v++) {
		osint_line (vectors_draw[v].x0, vectors_draw[v].y0,
					vectors_draw[v].x1, vectors_draw[v].y1,
					vectors_draw[v].color);
	}

//	IDirectDrawSurface_Unlock (lpdd_primary, NULL);
}


int updateEvents()
{
	EventHandler_updateEvents();

	if(fire) snd_regs[14] &= ~0x01;
	else snd_regs[14] |= 0x01;
	if(pound) snd_regs[14] &= ~0x02;
	else snd_regs[14] |= 0x02;
	if(zero) snd_regs[14] &= ~0x04;
	else snd_regs[14] |= 0x04;
	if(star) snd_regs[14] &= ~0x08;
	else snd_regs[14] |= 0x08;

	if(left) alg_jch0 = 0x00;
	else if(right) alg_jch0 = 0xff;
	else alg_jch0 = 0x80;

	if(up) alg_jch1 = 0xff;
	else if(down) alg_jch1 = 0x00;
	else alg_jch1 = 0x80;

	scr_offset.x = 0;
	scr_offset.y = 0;

	return !quit;
//	osint_clearscreen ();
}

void osint_emuloop (void)
{
	int timeToNextUpdate;
//	MSG Msg;
	unsigned timeout; /* timing flag */

	/* reset the vectrex hardware */

	vecx_reset ();

	/* startup the emulator's heart beat */
	//SetTimer (hwnd, 1, EMU_TIMER, NULL);

	timeToNextUpdate = maGetMilliSecondCount()+EMU_TIMER;


	while (1) {
		timeout = 0;

		vecx_emu ((VECTREX_MHZ / 1000) * EMU_TIMER, 0);

		while(maGetMilliSecondCount()<timeToNextUpdate) maSleep(1); 
		maUpdateScreen();
		if(!updateEvents()) break;
		timeToNextUpdate = maGetMilliSecondCount()+EMU_TIMER;
	}
}

int MAMain() {
	setCurrentFileSystem(FILESYSTEM);

	if (osint_defaults ()) {
		return 1;
	}

	osint_parse_cmdline ("-c FROGGER.BIN");

	/* determine a set of colors to use based */

	osint_gencolors ();

	/* message loop handler and emulator code */

	osint_emuloop ();

	freeCurrentFileSystem();

	return 0;
}
