
#include <ma.h>
#include <maheap.h>
#include <mastring.h>
#include "MAHeaders.h"

#define ERASE_ALL  /* erase all of screen at once, or one object at a time? */

#define STATE_FORMAT_VERSION "2006.03.15"


/*#ifndef QUARTER_VGA
#define WIDTH 160
#define HEIGHT 240
#else
#define WIDTH 232
#define HEIGHT 270
#endif*/
Extent gScreenSize;
int WIDTH;
int HEIGHT;

Handle highStore;

/* Constraints: */

#define NUM_BULLETS 4
#define NUM_ASTEROIDS 20
#define NUM_BITS 50
#define AST_RADIUS 5
#define AST_SIDES 6

#define ZOOM_START 20
#define ONEUP_SCORE 10000


/* Types: */

typedef struct letter_type {
	int x, y;
	int xm, ym;
} letter_type;

typedef struct bullet_type {
	int timer;
	int x, y;
	int xm, ym;
} bullet_type;

typedef struct shape_type {
	int radius;
	int angle;
} shape_type;

typedef struct asteroid_type {
	int alive, size;
	int x, y;
	int xm, ym;
	int angle, angle_m;
	shape_type shape[AST_SIDES];
} asteroid_type;

typedef struct bit_type {
	int timer;
	int x, y;
	int xm, ym;
} bit_type;


/* Globals: */

//Display *display;
//Window window;
//GC drawgc, erasegc;
//XKeyboardState kbstate;
bullet_type bullets[NUM_BULLETS];
asteroid_type asteroids[NUM_ASTEROIDS];
bit_type bits[NUM_BITS];
int sound, /*buttonmode,*/ want_sound, snd_pitch, snd_dur, text_zoom;
char zoom_str[24];
int x, y, xm, ym, angle;
int player_alive, player_die_timer;
int lives, score, high, level, game_pending;


/* Trig junk:  (thanks to Atari BASIC for this) */

int trig[12] = {
	1024,
	1014,
	984,
	935,
	868,
	784,
	685,
	572,
	448,
	316,
	117,
	0
};


/* Characters: */

int char_vectors[36][5][4] = {
	{
		/* 0 */
		{ 0, 0, 1, 0 },
		{ 1, 0, 1, 2 },
		{ 1, 2, 0, 2 },
		{ 0, 2, 0, 0 },
		{ -1, -1, -1, -1 }
	},

	{
		/* 1 */
		{ 1, 0, 1, 2 },
		{ -1, -1, -1, -1 },
		{ -1, -1, -1, -1 },
		{ -1, -1, -1, -1 },
		{ -1, -1, -1, -1 }
	},

	{
		/* 2 */
		{ 1, 0, 0, 0 },
		{ 1, 0, 1, 1 },
		{ 0, 1, 1, 1 },
		{ 0, 1, 0, 2 },
		{ 1, 2, 0, 2 },
	},

	{
		/* 3 */
		{ 0, 0, 1, 0 },
		{ 1, 0, 1, 2 },
		{ 0, 1, 1, 1 },
		{ 0, 2, 1, 2 },
		{ -1, -1, -1, -1 }
		},

		{
			/* 4 */
			{ 1, 0, 1, 2 },
			{ 0, 0, 0, 1 },
			{ 0, 1, 1, 1 },
			{ -1, -1, -1, -1 },
			{ -1, -1, -1, -1 }
		},

		{
			/* 5 */
			{ 1, 0, 0, 0 },
			{ 0, 0, 0, 1 },
			{ 0, 1, 1, 1 },
			{ 1, 1, 1, 2 },
			{ 1, 2, 0, 2 }
		},

		{
			/* 6 */
			{ 1, 0, 0, 0 },
			{ 0, 0, 0, 2 },
			{ 0, 2, 1, 2 },
			{ 1, 2, 1, 1 },
			{ 1, 1, 0, 1 }
		},

		{
			/* 7 */
			{ 0, 0, 1, 0 },
			{ 1, 0, 1, 2 },
			{ -1, -1, -1, -1 },
			{ -1, -1, -1, -1 },
			{ -1, -1, -1, -1 }
		},

		{
			/* 8 */
			{ 0, 0, 1, 0 },
			{ 0, 0, 0, 2 },
			{ 1, 0, 1, 2 },
			{ 0, 2, 1, 2 },
			{ 0, 1, 1, 1 }
		},

		{
			/* 9 */
			{ 1, 0, 1, 2 },
			{ 0, 0, 1, 0 },
			{ 0, 0, 0, 1 },
			{ 0, 1, 1, 1 },
			{ -1, -1, -1, -1 }
		},

		{
			/* A */
			{ 0, 2, 0, 1 },
			{ 0, 1, 1, 0 },
			{ 1, 0, 1, 2 },
			{ 0, 1, 1, 1 },
			{ -1, -1, -1, -1 }
		},

		{
			/* B */
			{ 0, 2, 0, 0 },
			{ 0, 0, 1, 0 },
			{ 1, 0, 0, 1 },
			{ 0, 1, 1, 2 },
			{ 1, 2, 0, 2 }
		},

		{
			/* C */
			{ 1, 0, 0, 0 },
			{ 0, 0, 0, 2 },
			{ 0, 2, 1, 2 },
			{ -1, -1, -1, -1 },
			{ -1, -1, -1, -1 }
		},

		{
			/* D */
			{ 0, 0, 1, 1 },
			{ 1, 1, 0, 2 },
			{ 0, 2, 0, 0 },
			{ -1, -1, -1, -1 },
			{ -1, -1, -1, -1 }
		},

		{
			/* E */
			{ 1, 0, 0, 0 },
			{ 0, 0, 0, 2 },
			{ 0, 2, 1, 2 },
			{ 0, 1, 1, 1 },
			{ -1, -1, -1, -1 }
		},

		{
			/* F */
			{ 1, 0, 0, 0 },
			{ 0, 0, 0, 2 },
			{ 0, 1, 1, 1 },
			{ -1, -1, -1, -1 },
			{ -1, -1, -1, -1 }    
		},

		{
			/* G */
			{ 1, 0, 0, 0 },
			{ 0, 0, 0, 2 },
			{ 0, 2, 1, 2 },
			{ 1, 2, 1, 1 },
			{ -1, -1, -1, -1 }
		},

		{
			/* H */
			{ 0, 0, 0, 2 },
			{ 1, 0, 1, 2 },
			{ 0, 1, 1, 1 },
			{ -1, -1, -1, -1 },
			{ -1, -1, -1, -1 }
		},

		{
			/* I */
			{ 1, 0, 1, 2 },
			{ -1, -1, -1, -1 },
			{ -1, -1, -1, -1 },
			{ -1, -1, -1, -1 },
			{ -1, -1, -1, -1 }
		},

		{
			/* J */
			{ 1, 0, 1, 2 },
			{ 1, 2, 0, 2 },
			{ 0, 2, 0, 1 },
			{ -1, -1, -1, -1 },
			{ -1, -1, -1, -1 }
		},

		{
			/* K */
			{ 0, 0, 0, 2 },
			{ 1, 0, 0, 1 },
			{ 1, 2, 0, 1 },
			{ -1, -1, -1, -1 },
			{ -1, -1, -1, -1 }
		},

		{
			/* L */
			{ 0, 0, 0, 2 },
			{ 0, 2, 1, 2 },
			{ -1, -1, -1, -1 },
			{ -1, -1, -1, -1 },
			{ -1, -1, -1, -1 }
		},

		{
			/* M */
			{ 0, 0, 0, 2 },
			{ 1, 0, 1, 2 },
			{ 0, 0, 1, 1 },
			{ 0, 1, 1, 0 },
			{ -1, -1, -1, -1 }
		},

		{
			/* N */
			{ 0, 2, 0, 0 },
			{ 0, 0, 1, 2 },
			{ 1, 2, 1, 0 },
			{ -1, -1, -1, -1 },
			{ -1, -1, -1, -1 }
		},

		{
			/* O */
			{ 0, 0, 1, 0 },
			{ 1, 0, 1, 2 },
			{ 1, 2, 0, 2 },
			{ 0, 2, 0, 0 },
			{ -1, -1, -1, -1 }
		},

		{
			/* P */
			{ 0, 2, 0, 0 },
			{ 0, 0, 1, 0 },
			{ 1, 0, 1, 1 },
			{ 1, 1, 0, 1 },
			{ -1, -1, -1, -1 }
		},

		{ 
			/* Q */
			{ 0, 0, 1, 0 },
			{ 1, 0, 1, 2 },
			{ 1, 2, 0, 2 },
			{ 0, 2, 0, 0 },
			{ 0, 1, 1, 2 }
		},

		{
			/* R */
			{ 0, 2, 0, 0 },
			{ 0, 0, 1, 0 },
			{ 1, 0, 1, 1 },
			{ 1, 1, 0, 1 },
			{ 0, 1, 1, 2 }
		},

		{
			/* S */
			{ 1, 0, 0, 0 },
			{ 0, 0, 0, 1 },
			{ 0, 1, 1, 1 },
			{ 1, 1, 1, 2 },
			{ 1, 2, 0, 2 }
		},

		{
			/* T */
			{ 0, 0, 1, 0 },
			{ 1, 0, 1, 2 },
			{ -1, -1, -1, -1 },
			{ -1, -1, -1, -1 },
			{ -1, -1, -1, -1 }
		},

		{
			/* U */
			{ 0, 0, 0, 2 },
			{ 0, 2, 1, 2 },
			{ 1, 2, 1, 0 },
			{ -1, -1, -1, -1 },
			{ -1, -1, -1, -1 }
		},

		{
			/* V */
			{ 0, 0, 0, 1 },
			{ 0, 1, 1, 2 },
			{ 1, 2, 1, 0 },
			{ -1, -1, -1, -1 },
			{ -1, -1, -1, -1 }
		},

		{
			/* W */
			{ 0, 0, 0, 2 },
			{ 1, 0, 1, 2 },
			{ 0, 1, 1, 2 },
			{ 0, 2, 1, 1 },
			{ -1, -1, -1, -1 }
		},

		{
			/* X */
			{ 0, 0, 1, 2 },
			{ 0, 2, 1, 0 },
			{ -1, -1, -1, -1 },
			{ -1, -1, -1, -1 },
			{ -1, -1, -1, -1 }
		},

		{
			/* Y */
			{ 0, 0, 1, 1 },
			{ 1, 0, 1, 2 },
			{ -1, -1, -1, -1 },
			{ -1, -1, -1, -1 },
			{ -1, -1, -1, -1 }
		},

		{
			/* Z */
			{ 0, 0, 1, 0 },
			{ 1, 0, 0, 2 },
			{ 0, 2, 1, 2 },
			{ -1, -1, -1, -1 },
			{ -1, -1, -1, -1 }
		}
};



/* Local function prototypes: */

int title(void);
int game(void);
void finish(void);
void setup(void);
//GC CreateGC(Display *display, Drawable drawable, unsigned long forecolor,
//	unsigned long backcolor);
int fast_cos(int v);
int fast_sin(int v);
void draw_line(int x1, int y1, int x2, int y2);
void draw_segment(int r1, int a1,
	int r2, int a2,
	int cx, int cy, int ang);
void add_bullet(int x, int y, int a, int xm, int ym);
void add_asteroid(int x, int y, int xm, int ym, int size);
void add_bit(int x, int y, int xm, int ym);
void draw_asteroid(int size, int x, int y, int angle, shape_type * shape);
void playsound(int pitch, int duration);
void do_playsound(void);
void hurt_asteroid(int j, int xm, int ym, int exp_size);
void add_score(int amount);
void draw_char(char c, int x, int y, int r);
void draw_text(const char * str, int x, int y, int s);
void draw_closebox(void);
//void draw_buttontoggle(void);
void draw_soundtoggle(void);
void draw_thick_line(int x1, int y1, int x2, int y2);
void reset_level(void);

