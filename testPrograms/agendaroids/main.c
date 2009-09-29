/*
agendaroids.c

An asteroid shooting game with vector graphics for the Agenda VR3
Linux-based PDA.

by Bill Kendrick
bill@newbreedsoftware.com
http://www.newbreedsoftware.com/agendaroids/

May 18, 2001 - March 7, 2002


Modified for Mobile Author by Fredrik Eldh
fredrik.eldh@mobilesorcery.com

March 15, 2006 - ???
*/

#include "agendaroids.h"
#include <mastdlib.h>

#define USE_DELAY
#define USE_STORE

/* --- MAIN --- */

int MAMain()
{
	int done;
	//FILE * fi;
	//char statefile[256], buf[256];

	gScreenSize = maGetScrSize();
	WIDTH = EXTENT_X(gScreenSize);
	HEIGHT = EXTENT_Y(gScreenSize);

	setup();


	/* Set defaults: */

	score = 0;
	high = 0;
	game_pending = 0;


	/* Load state from disk: */

	/*snprintf(statefile, sizeof(statefile), "%s/.agendaroids-state",
	getenv("HOME"));

	fi = fopen(statefile, "r");
	if (fi != NULL)
	{
	*//* Skip comment line: */

	/*fgets(buf, sizeof(buf), fi);


	*//* Grab statefile version: */

	/*fgets(buf, sizeof(buf), fi);
	buf[strlen(buf) - 1] = '\0';

	if (strcmp(buf, STATE_FORMAT_VERSION) != 0)
	{
	fprintf(stderr, "Agendaroids state file format has been updated.\n"
	"Old game state is unreadable.  Sorry!\n");
	}
	else
	{
	game_pending = fgetc(fi);
	lives = fgetc(fi);
	level = fgetc(fi);
	player_alive = fgetc(fi);
	player_die_timer = fgetc(fi);
	fread(&score, sizeof(int), 1, fi);
	fread(&high, sizeof(int), 1, fi);
	fread(&x, sizeof(int), 1, fi);
	fread(&y, sizeof(int), 1, fi);
	fread(&xm, sizeof(int), 1, fi);
	fread(&ym, sizeof(int), 1, fi);
	fread(&angle, sizeof(int), 1, fi);
	fread(bullets, sizeof(bullet_type), NUM_BULLETS, fi);
	fread(asteroids, sizeof(asteroid_type), NUM_ASTEROIDS, fi);
	fread(bits, sizeof(bit_type), NUM_BITS, fi);
	}

	fclose(fi);
	}*/

#ifdef USE_STORE
	highStore = maOpenStore("highScore", MAS_CREATE_IF_NECESSARY);
	maReadStore(highStore, highplaceholder);
	if(maGetDataSize(highplaceholder)==sizeof(high))
	{
		maReadData(highplaceholder, &high, 0, sizeof(high));
	}
	else
	{
		maDestroyObject(highplaceholder);
		maCreateData(highplaceholder, sizeof(high));
		maWriteData(highplaceholder, &high, 0, sizeof(high));
		maWriteStore(highStore, highplaceholder);
	}
#endif	//USE_STORE

	/* Main app loop! */

	do
	{
		done = title();

		if (!done)
		{
			done = game();
		}
	}
	while (!done);


	/* Save state: */

	/*fi = fopen(statefile, "w");
	if (fi == NULL)
	{
	perror(statefile);
	}
	else
	{
	fprintf(fi, "Agendaroids State File\n");
	fprintf(fi, "%s\n", STATE_FORMAT_VERSION);

	fputc(game_pending, fi);
	fputc(lives, fi);
	fputc(level, fi);
	fputc(player_alive, fi);
	fputc(player_die_timer, fi);
	fwrite(&score, sizeof(int), 1, fi);
	fwrite(&high, sizeof(int), 1, fi);
	fwrite(&x, sizeof(int), 1, fi);
	fwrite(&y, sizeof(int), 1, fi);
	fwrite(&xm, sizeof(int), 1, fi);
	fwrite(&ym, sizeof(int), 1, fi);
	fwrite(&angle, sizeof(int), 1, fi);
	fwrite(bullets, sizeof(bullet_type), NUM_BULLETS, fi);
	fwrite(asteroids, sizeof(asteroid_type), NUM_ASTEROIDS, fi);
	fwrite(bits, sizeof(bit_type), NUM_BITS, fi);

	fclose(fi);
	}*/

	//finish();

	maExit(0);
	return 0;
}

void my_uitoa(char* dst, unsigned int src) { //unsigned int to ascii
	char buffer[13];
	int pos=0;
	int i=0;
	while(1) {
		buffer[pos] = (char) src % 10 + '0';
		if(src < 10)
			break;
		src /= 10;
		pos++;
	}
	for(; pos>=0; i++,pos--) {
		dst[i] = buffer[pos];
	}
	dst[i] = 0;
}

void my_sprinti(char* dst, const char* src, int i) {
	int pos=0;
	while(src[pos]) {
		dst[pos] = src[pos];
		pos++;
	}
	my_uitoa(dst+pos, i);
}

/* Title screen: */

int title(void)
{
	int done, quit;
	int i, snapped, angle, size, counter, x, y, xm, ym, xoff;
	//XEvent event;
	//KeySym key;
	const char * titlestr = "AGENDAROIDS";
	char str[20];
	letter_type letters[11];


	/* Reset letters: */

	snapped = 0;

	for (i = 0; i < 11; i++)
	{
		letters[i].x = (rand() % 160);
		letters[i].y = (rand() % 240);
		letters[i].xm = 0;
		letters[i].ym = 0;
	}

	xoff = (WIDTH - 160) / 2;

	x = (rand() % WIDTH);
	y = (rand() % HEIGHT);
	xm = (rand() % 4) + 2;
	ym = (rand() % 10) - 5;

	counter = 0;
	angle = 0;
	size = 40;

	done = 0;
	quit = 0;

	do
	{
#ifdef USE_DELAY
		int now, then;
		long time_padding;
		then = maGetMilliSecondCount();
#endif
		counter++;


		/* Rotate rock: */

		angle = ((angle + 2) % 360);


		/* Make rock grow: */

		if ((counter % 3) == 0)
		{
			if (size > 1)
				size--;
		}


		/* Move rock: */

		x = x + xm;

		if (x >= WIDTH)
			x = x - WIDTH;

		y = y + ym;

		if (y >= HEIGHT)
			y = y - HEIGHT;
		else if (y < 0)
			y = y + HEIGHT;


		/* Handle events: */

		//while (XPending(display))
		//{
		//XNextEvent(display, &event);

		//if (event.type == KeyPress)
		{
			int key = maGetKeys();

			if (key == MAK_FIRE)
			{
				done = 1;
			}
			/*else if (key == XK_Escape)  //handled by VM
			{
			done = 1;
			quit = 1;
			}*/
		}
		/*else if (event.type == ButtonPress)
		{
		if (event.xbutton.x <= 16 &&
		event.xbutton.y >= HEIGHT - 16)
		{
		*//* Close box - Quit! */

		/*done = 1;
		quit = 1;
		}
		else if (event.xbutton.x >= (WIDTH / 2) - 5 &&
		event.xbutton.x <= (WIDTH / 2) + 5 &&
		event.xbutton.y >= (HEIGHT - 10))
		{
		*//* Button mode toggle: */

		/*buttonmode = !buttonmode;
		}
		else if (event.xbutton.x >= (WIDTH - 16) &&
		event.xbutton.y >= (HEIGHT - 16))
		{
		*//* Sound toggle: */

		/*sound = !sound;
		}
		else if (event.xbutton.x >= 55 + xoff &&
		event.xbutton.x <= 90 + xoff &&
		event.xbutton.y >= 180 && event.xbutton.y <= 195)
		{
		*//* Start! */

		/*game_pending = 0;
		done = 1;
		}
		else if (event.xbutton.x >= 40 + xoff &&
		event.xbutton.x <= 95 + xoff &&
		event.xbutton.y >= 200 && event.xbutton.y <= 215 &&
		game_pending)
		{
		done = 1;
		}
		}
		}*/


		/* Move title characters: */

		if (snapped < 11)
		{
			for (i = 0; i < 11; i++)
			{
				letters[i].x = letters[i].x + letters[i].xm;
				letters[i].y = letters[i].y + letters[i].ym;


				/* Home in on final spot! */

				if (letters[i].x > (i * 14) && letters[i].xm > -4)
					letters[i].xm--;
				else if (letters[i].x < (i * 14) && letters[i].xm < 4)
					letters[i].xm++;

				if (letters[i].y > 100 && letters[i].ym > -4)
					letters[i].ym--;
				else if (letters[i].y < 100 && letters[i].ym < 4)
					letters[i].ym++;


				/* Snap into place: */

				if (letters[i].x >= (i * 14) - 8 &&
					letters[i].x <= (i * 14) + 8 &&
					letters[i].y >= 92 &&
					letters[i].y <= 108 &&
					(letters[i].xm != 0 ||
					letters[i].ym != 0))
				{
					letters[i].x = i * 14;
					letters[i].xm = 0;

					letters[i].y = 100;
					letters[i].ym = 0;

					snapped++;
				}
			}
		}


		/* Draw screen: */

		/* (Erase first) */
		maSetColor(0);
		maFillRect(0, 0, WIDTH, HEIGHT);


		/* (Title) */

		for (i = 0; i < 11; i++)
		{
			draw_char(titlestr[i], letters[i].x + xoff, letters[i].y, 10);
		}


		/* (Credits) */

		if (snapped == 11)
		{
			draw_text("BY BILL KENDRICK", 16 + xoff, 140, 5);
			draw_text("NEW BREED SOFTWARE", 8 + xoff, 155, 5);

			//sprintf(str, "HIGH %.6d", high);
			my_sprinti(str, "HIGH ", high);
			draw_text(str, 32 + xoff, 5, 5);
			draw_text(str, 33 + xoff, 6, 5);

			if (score != 0 && (score != high || (counter % 20) < 10))
			{
				if (game_pending == 0)
					//sprintf(str, "LAST %.6d", score);
					my_sprinti(str, "LAST ", score);
				else
					//sprintf(str, "SCR  %.6d", score);
					my_sprinti(str, "SCR ", score);
				draw_text(str, 32 + xoff, 25, 5);
				draw_text(str, 33 + xoff, 26, 5);
			}
		}


		//draw_text("START", 55 + xoff, 180, 5);

		if (game_pending)
			draw_text("CONTINUE", 40 + xoff, 200, 5);


		/* (Giant rock) */

		/*
		draw_segment(40 / size, 0, 30 / size, 30, x, y, angle);
		draw_segment(30 / size, 30, 40 / size, 55, x, y, angle);
		draw_segment(40 / size, 55, 25 / size, 90, x, y, angle);
		draw_segment(25 / size, 90, 40 / size, 120, x, y, angle);
		draw_segment(40 / size, 120, 35 / size, 130, x, y, angle);
		draw_segment(35 / size, 130, 40 / size, 160, x, y, angle);
		draw_segment(40 / size, 160, 30 / size, 200, x, y, angle);
		draw_segment(30 / size, 200, 45 / size, 220, x, y, angle);
		draw_segment(45 / size, 220, 25 / size, 265, x, y, angle);
		draw_segment(25 / size, 265, 30 / size, 300, x, y, angle);
		draw_segment(30 / size, 300, 45 / size, 335, x, y, angle);
		draw_segment(45 / size, 335, 40 / size, 0, x, y, angle);
		*/


		/* (Agenda logo!) */
		/* (i'm very proud!) */

		draw_segment(35 / size, 90 + 35, 30 / size, 90 + 20, x, y, angle);
		draw_segment(30 / size, 90 + 20, 22 / size, 90 + 27, x, y, angle);
		draw_segment(22 / size, 90 + 27, 26 / size, 90 + 45, x, y, angle);
		draw_segment(26 / size, 90 + 45, 35 / size, 90 + 35, x, y, angle);

		draw_segment(30 / size, 90 + 20, 40 / size, 90 + 16, x, y, angle);
		draw_segment(40 / size, 90 + 16, 38 / size, 84, x, y, angle);
		draw_segment(38 / size, 84, 28 / size, 82, x, y, angle);
		draw_segment(28 / size, 82, 30 / size, 90 + 20, x, y, angle);

		draw_segment(28 / size, 82, 32 / size, 58, x, y, angle);
		draw_segment(32 / size, 58, 22 / size, 37, x, y, angle);
		draw_segment(22 / size, 37, 14 / size, 74, x, y, angle);
		draw_segment(14 / size, 74, 28 / size, 82, x, y, angle);

		draw_segment(22 / size, 37, 35 / size, 24, x, y, angle);
		draw_segment(35 / size, 24, 31 / size, 360 - 7, x, y, angle);
		draw_segment(31 / size, 360 - 7, 19 / size, 360 - 15, x, y, angle);
		draw_segment(19 / size, 360 - 15, 22 / size, 37, x, y, angle);

		draw_segment(19 / size, 360 - 15, 10 / size, 360 - 33, x, y, angle);
		draw_segment(10 / size, 360 - 33, 24 / size, 360 - 68, x, y, angle);
		draw_segment(24 / size, 360 - 68, 30 / size, 360 - 53, x, y, angle);
		draw_segment(30 / size, 360 - 53, 19 / size, 360 - 15, x, y, angle);

		draw_segment(10 / size, 360 - 33, 10 / size, 30, x, y, angle);
		draw_segment(10 / size, 30, 12 / size, 180 - 24, x, y, angle);
		draw_segment(12 / size, 180 - 24, 11 / size, 180 + 20, x, y, angle);
		draw_segment(11 / size, 180 + 20, 10 / size, 360 - 33, x, y, angle);

		draw_segment(11 / size, 180 + 20, 30 / size, 180 + 10, x, y, angle);
		draw_segment(30 / size, 180 + 10, 37 / size, 180 + 39, x, y, angle);
		draw_segment(37 / size, 180 + 39, 26 / size, 180 + 65, x, y, angle);
		draw_segment(26 / size, 180 + 65, 11 / size, 180 + 20, x, y, angle);

		draw_segment(26 / size, 180 + 65, 24 / size, 360 - 68, x, y, angle);
		draw_segment(24 / size, 360 - 68, 38 / size, 360 - 76, x, y, angle);
		draw_segment(38 / size, 360 - 76, 39 / size, 180 + 73, x, y, angle);
		draw_segment(39 / size, 180 + 73, 26 / size, 180 + 65, x, y, angle);

		draw_segment(30 / size, 360 - 53, 36 / size, 360 - 41, x, y, angle);
		draw_segment(36 / size, 360 - 41, 43 / size, 360 - 50, x, y, angle);
		draw_segment(43 / size, 360 - 50, 37 / size, 360 - 61, x, y, angle);
		draw_segment(37 / size, 360 - 61, 30 / size, 360 - 53, x, y, angle);

		draw_segment(36 / size, 360 - 41, 34 / size, 360 - 35, x, y, angle);
		draw_segment(34 / size, 360 - 35, 42 / size, 360 - 28, x, y, angle);
		draw_segment(42 / size, 360 - 28, 44 / size, 360 - 33, x, y, angle);
		draw_segment(44 / size, 360 - 33, 36 / size, 360 - 41, x, y, angle);




		/* (Close box) */

		draw_closebox();


		/* (Button mode toggle) */

		//draw_buttontoggle();


		/* (Sound toggle) */

		draw_soundtoggle();


		/* Flush and pause! */

		maUpdateScreen();

#ifdef USE_DELAY
		now = maGetMilliSecondCount();
		time_padding = 20 - (now - then);
		if (time_padding > 0)
			maWait(time_padding);
#endif
	}
	while (!done);

	return(quit);
}



/* --- GAME --- */

int game(void)
{
	int done, quit, counter;
	int i, j;
	int num_asteroids_alive;
	//XEvent event;
	//KeySym key;
	int key;
	int left_pressed, right_pressed, up_pressed, shift_pressed;
	char str[10];


	done = 0;
	quit = 0;
	counter = 0;

	want_sound = 0;
	snd_pitch = 100;
	snd_dur = 0;

	if (game_pending == 0)
	{
		lives = 3;
		score = 0;

		player_alive = 1;
		player_die_timer = 0;
		angle = 90;
		x = (WIDTH / 2) << 4;
		y = (HEIGHT / 2) << 4;
		xm = 0;
		ym = 0;

		level = 1;
		reset_level();
	}

	game_pending = 1;

	do
	{
#ifdef USE_DELAY
		int now, then;
		long time_padding;
		then = maGetMilliSecondCount();
#endif
		counter++;


		/* Handle events: */

		//while (XPending(display))
		{
			//XNextEvent(display, &event);

			//if (event.type == KeyPress ||
			//event.type == KeyRelease)
			{

				left_pressed = 0;
				right_pressed = 0;
				up_pressed = 0;
				shift_pressed = 0;
				//key = XLookupKeysym((XKeyEvent *)&event, 0);
				key = maGetKeys();

				//if (event.type == KeyPress)
				{
					//if (key == XK_Escape)
					{
						/* Quit! */

						//done = 1;
					}


					/* Key press... */

					//if (buttonmode == 0)
					{
						if (key & MAK_RIGHT)//== XK_Right)
						{
							/* Rotate CW */

							left_pressed = 0;
							right_pressed = 1;
						}
						else if (key & MAK_LEFT)//== XK_Left)
						{
							/* Rotate CCW */

							left_pressed = 1;
							right_pressed = 0;
						}
						if (key & MAK_UP)//== XK_Up ||
							//key == XK_Page_Up)
						{
							/* Thrust! */

							up_pressed = 1;
						}
						if ((key == MAK_FIRE)//XK_space ||
							/*key == XK_Page_Down)*/ &&
							player_alive)
						{
							/* Fire a bullet! */

							add_bullet(x >> 4, y >> 4, angle, xm, ym);
						}
					}
					/*else if (buttonmode == 1)
					{
					if (key == XK_Page_Up)
					{
					// Rotate CW

					left_pressed = 0;
					right_pressed = 1;
					}
					else if (key == XK_Page_Down)
					{
					// Rotate CCW

					left_pressed = 1;
					right_pressed = 0;
					}
					else if (key == XK_Left)
					{
					// Thrust!

					up_pressed = 1;
					}
					else if (key == XK_Right &&
					player_alive)
					{
					// Fire a bullet!

					add_bullet(x >> 4, y >> 4, angle, xm, ym);
					}
					}*/

					if (key & MAK_DOWN)//== XK_Shift_L ||
						//key == XK_Shift_R)
					{
						/* Respawn now (if applicable) */

						shift_pressed = 1;
					}
				}
				/*else if (event.type == KeyRelease)
				{
				// Key release...

				if (buttonmode == 0)
				{
				if (key == XK_Right)
				{
				right_pressed = 0;
				}
				else if (key == XK_Left)
				{
				left_pressed = 0;
				}
				else if (key == XK_Up ||
				key == XK_Page_Up)
				{
				up_pressed = 0;
				}
				}
				else if (buttonmode == 1)
				{
				if (key == XK_Page_Up)
				{
				right_pressed = 0;
				}
				else if (key == XK_Page_Down)
				{
				left_pressed = 0;
				}
				else if (key == XK_Left)
				{
				up_pressed = 0;
				}
				}

				if (key == XK_Shift_L ||
				key == XK_Shift_R)
				{
				// Respawn now (if applicable)

				shift_pressed = 1;
				}
				}*/
			}
			/*else if (event.type == ButtonPress)
			{
			if (event.xbutton.x <= 16 &&
			event.xbutton.y >= 224)
			{
			// Close box - Quit!

			done = 1;
			}
			else if (event.xbutton.x >= 75 &&
			event.xbutton.x <= 85 &&
			event.xbutton.y >= 230)
			{
			// Button mode toggle:

			buttonmode = !buttonmode;
			}
			else if (event.xbutton.x >= 144 &&
			event.xbutton.y >= 224)
			{
			// Sound toggle:

			sound = !sound;
			}
			}*/
		}


		/* Rotate ship: */

		if (right_pressed)
		{
			angle = angle - 12;
			if (angle < 0)
				angle = angle + 360;
		}
		else if (left_pressed)
		{
			angle = angle + 12;
			if (angle >= 360)
				angle = angle - 360;
		}


		/* Thrust ship: */

		if (up_pressed)
		{
			xm = xm + ((fast_cos(angle >> 3) * 3) >> 10);
			ym = ym - ((fast_sin(angle >> 3) * 3) >> 10);
		}
		else
		{
			if ((counter % 10) == 0)
			{
				xm = (xm * 7) / 8;
				ym = (ym * 7) / 8;
			}
		}


		/* Handle player death: */

		if (player_alive == 0)
		{
			player_die_timer--;

			if (player_die_timer <= 0)
			{
				if (lives > 0)
				{
					/* Reset player: */

					player_die_timer = 0;
					angle = 90;
					x = (WIDTH / 2) << 4;
					y = (HEIGHT / 2) << 4;
					xm = 0;
					ym = 0;


					/* Only bring player back when it's alright to! */

					player_alive = 1;

					if (!shift_pressed)
					{
						for (i = 0; i < NUM_ASTEROIDS && player_alive; i++)
						{
							if (asteroids[i].alive)
							{
								if (asteroids[i].x >= (x >> 4) - 50 &&
									asteroids[i].x <= (x >> 4) + 50 &&
									asteroids[i].y >= (y >> 4) - 40 &&
									asteroids[i].y <= (y >> 4) + 40)
								{
									/* If any asteroid is too close for comfort,
									don't bring ship back yet! */

									player_alive = 0;
								}
							}
						}
					}
				}
				else
				{
					done = 1;
					game_pending = 0;
				}
			}
		}


#ifdef ERASE_ALL
		/* Erase screen: */
		maSetColor(0);
		maFillRect(0, 0, WIDTH, HEIGHT);

#else
		/* Erase ship: */

		XFillRectangle(display, window, erasegc,
			(x >> 4) - 11, (y >> 4) - 11, 22, 22);


		/* Erase bullets: */

		for (i = 0; i < NUM_BULLETS; i++)
		{
			if (bullets[i].timer >= 0)
			{
				XFillRectangle(display, window, erasegc,
					bullets[i].x - 5, bullets[i].y - 5,
					10, 10);
			}
		}


		/* Erase asteroids: */

		for (i = 0; i < NUM_ASTEROIDS; i++)
		{
			if (asteroids[i].alive)
			{
				XFillRectangle(display, window, erasegc,
					(asteroids[i].x -
					(asteroids[i].size * AST_RADIUS) - 1),
					(asteroids[i].y -
					(asteroids[i].size * AST_RADIUS) - 1),
					asteroids[i].size * AST_RADIUS * 2 + 2,
					asteroids[i].size * AST_RADIUS * 2 + 2);
			}
		}


		/* Erase bits: */

		for (i = 0; i < NUM_BITS; i++)
		{
			if (bits[i].timer > 0)
			{
				XDrawLine(display, window, erasegc,
					bits[i].x, bits[i].y,
					bits[i].x + bits[i].xm, bits[i].y + bits[i].ym);
			}
		}
#endif


		/* Move ship: */

		x = x + xm;
		y = y + ym;


		/* Wrap ship around edges of screen: */

		if (x >= (WIDTH << 4))
			x = x - (WIDTH << 4);
		else if (x < 0)
			x = x + (WIDTH << 4);

		if (y >= (HEIGHT << 4))
			y = y - (HEIGHT << 4);
		else if (y < 0)
			y = y + (HEIGHT << 4);


		/* Move bullets: */

		for (i = 0; i < NUM_BULLETS; i++)
		{
			if (bullets[i].timer >= 0)
			{
				/* Bullet wears out: */

				bullets[i].timer--;


				/* Move bullet: */

				bullets[i].x = bullets[i].x + bullets[i].xm;
				bullets[i].y = bullets[i].y + bullets[i].ym;


				/* Wrap bullet around edges of screen: */

				if (bullets[i].x >= WIDTH)
					bullets[i].x = bullets[i].x - WIDTH;
				else if (bullets[i].x < 0)
					bullets[i].x = bullets[i].x + WIDTH;

				if (bullets[i].y >= HEIGHT)
					bullets[i].y = bullets[i].y - HEIGHT;
				else if (bullets[i].y < 0)
					bullets[i].y = bullets[i].y + HEIGHT;


				/* Check for collision with any asteroids! */

				for (j = 0; j < NUM_ASTEROIDS; j++)
				{
					if (bullets[i].timer > 0 && asteroids[j].alive)
					{
						if ((bullets[i].x + 5 >=
							asteroids[j].x - asteroids[j].size * AST_RADIUS) &&
							(bullets[i].x - 5<=
							asteroids[j].x + asteroids[j].size * AST_RADIUS) &&
							(bullets[i].y + 5 >=
							asteroids[j].y - asteroids[j].size * AST_RADIUS) &&
							(bullets[i].y - 5 <=
							asteroids[j].y + asteroids[j].size * AST_RADIUS))
						{
							/* Remove bullet! */

							bullets[i].timer = 0;


							hurt_asteroid(j, bullets[i].xm, bullets[i].ym,
								asteroids[j].size * 3);
						}
					}
				}
			}
		}


		/* Move asteroids: */

		num_asteroids_alive = 0;

		for (i = 0; i < NUM_ASTEROIDS; i++)
		{
			if (asteroids[i].alive)
			{
				num_asteroids_alive++;

				/* Move asteroid: */

				if ((counter % 4) == 0)
				{
					asteroids[i].x = asteroids[i].x + asteroids[i].xm;
					asteroids[i].y = asteroids[i].y + asteroids[i].ym;
				}


				/* Wrap asteroid around edges of screen: */

				if (asteroids[i].x >= WIDTH)
					asteroids[i].x = asteroids[i].x - WIDTH;
				else if (asteroids[i].x < 0)
					asteroids[i].x = asteroids[i].x + WIDTH;

				if (asteroids[i].y >= HEIGHT)
					asteroids[i].y = asteroids[i].y - HEIGHT;
				else if (asteroids[i].y < 0)
					asteroids[i].y = asteroids[i].y + HEIGHT;


				/* Rotate asteroid: */

				asteroids[i].angle = (asteroids[i].angle +
					asteroids[i].angle_m);


				/* Wrap rotation angle... */

				if (asteroids[i].angle < 0)
					asteroids[i].angle = asteroids[i].angle + 360;
				else if (asteroids[i].angle >= 360)
					asteroids[i].angle = asteroids[i].angle - 360;


				/* See if we collided with the player: */

				if (asteroids[i].x >= (x >> 4) - 10 &&
					asteroids[i].x <= (x >> 4) + 10 &&
					asteroids[i].y >= (y >> 4) - 10 &&
					asteroids[i].y <= (y >> 4) + 10 &&
					player_alive)
				{
					hurt_asteroid(i, xm >> 4, ym >> 4, NUM_BITS);

					player_alive = 0;
					player_die_timer = 30;

					lives--;

					if (lives == 0)
					{
						player_die_timer = 100;
					}
				}
			}
		}


		/* Move bits: */

		for (i = 0; i < NUM_BITS; i++)
		{
			if (bits[i].timer > 0)
			{
				/* Countdown bit's lifespan: */

				bits[i].timer--;


				/* Move the bit: */

				bits[i].x = bits[i].x + bits[i].xm;
				bits[i].y = bits[i].y + bits[i].ym;


				/* Wrap bit around edges of screen: */

				if (bits[i].x >= WIDTH)
					bits[i].x = bits[i].x - WIDTH;
				else if (bits[i].x < 0)
					bits[i].x = bits[i].x + WIDTH;

				if (bits[i].y >= HEIGHT)
					bits[i].y = bits[i].y - HEIGHT;
				else if (bits[i].y < 0)
					bits[i].y = bits[i].y + HEIGHT;
			}
		}


		/* Draw ship: */

		if (player_alive)
		{
			draw_segment(10, 0,
				5, 135,
				x >> 4, y >> 4,
				angle);

			draw_segment(5, 135,
				0, 0,
				x >> 4, y >> 4,
				angle);

			draw_segment(0, 0,
				5, 225,
				x >> 4, y >> 4,
				angle);

			draw_segment(5, 225,
				10, 0,
				x >> 4, y >> 4,
				angle);


			/* Draw flame: */

			if (up_pressed)
			{
				draw_segment(0, 0,
					(rand() % 10), 180,
					x >> 4, y >> 4,
					angle);
			}
		}


		/* Draw bullets: */

		for (i = 0; i < NUM_BULLETS; i++)
		{
			if (bullets[i].timer >= 0)
			{
				draw_thick_line(bullets[i].x - (rand() % 5),
					bullets[i].y - (rand() % 5),
					bullets[i].x + (rand() % 5),
					bullets[i].y + (rand() % 5));

				draw_thick_line(bullets[i].x + (rand() % 5),
					bullets[i].y - (rand() % 5),
					bullets[i].x - (rand() % 5),
					bullets[i].y + (rand() % 5));
			}
		}


		/* Draw asteroids: */

		for (i = 0; i < NUM_ASTEROIDS; i++)
		{
			if (asteroids[i].alive)
			{
				draw_asteroid(asteroids[i].size,
					asteroids[i].x, asteroids[i].y,
					asteroids[i].angle,
					asteroids[i].shape);
			}
		}


		/* Draw bits: */

		for (i = 0; i < NUM_BITS; i++)
		{
			if (bits[i].timer > 0)
			{
				maSetColor(-1);
				maLine(bits[i].x, bits[i].y,
					bits[i].x + bits[i].xm,
					bits[i].y + bits[i].ym);
			}
		}


		/* Draw score: */

		my_sprinti(str, "", score);
		draw_text(str, 3, 3, 7);
		draw_text(str, 4, 4, 7);


		/* Level: */

		my_sprinti(str, "", level);
		draw_text(str, WIDTH / 2 - 5, 3, 7);
		draw_text(str, WIDTH / 2 - 4, 4, 7);


		/* Draw lives: */

		for (i = 0; i < lives; i++)
		{
			draw_segment(8, 0,
				4, 135,
				WIDTH - 10 - i * 10, 10,
				90);

			draw_segment(4, 135,
				0, 0,
				WIDTH - 10 - i * 10, 10,
				90);

			draw_segment(0, 0,
				4, 225,
				WIDTH - 10 - i * 10, 10,
				90);

			draw_segment(4, 225,
				8, 0,
				WIDTH - 10 - i * 10, 10,
				90);
		}


		/* Zooming level effect: */

		if (text_zoom > 0)
		{
			text_zoom--;

			draw_text(zoom_str, (WIDTH - (strlen(zoom_str) * text_zoom)) / 2,
				(HEIGHT - text_zoom) / 2,
				text_zoom);
		}


		/* Game over? */

		if (player_alive == 0 && lives == 0)
		{
			if (player_die_timer > 7)
			{
				draw_text("GAME OVER",
					(WIDTH - 9 * player_die_timer) / 2,
					(HEIGHT - player_die_timer) / 2,
					player_die_timer);
			}
			else
			{
				draw_text("GAME OVER",
					48 + (WIDTH - 160) / 2, 116 + (HEIGHT - 240) / 2, 7);
			}
		}


		/* Draw close box: */

		draw_closebox();


		/* Draw button mode toggle: */

		//draw_buttontoggle();


		/* Draw sound toggle: */

		draw_soundtoggle();


		/* Go to next level? */

		if (num_asteroids_alive == 0)
		{
			level++;

			reset_level();
		}


		/* Play any sounds: */

		if (want_sound)
		{
			want_sound = 0;

			//do_playsound();
		}



		/* Flush and pause! */
		maUpdateScreen();
		//XSync(display, 0);
#ifdef USE_DELAY
		now = maGetMilliSecondCount();

		time_padding = 20 - (now - then);
		if (time_padding > 0)
		{
			maWait(time_padding);
		}
#endif
	}
	while (!done);


	/* Record, if a high score: */

#ifdef USE_STORE
	if (score >= high)
	{
		high = score;
		maWriteData(highplaceholder, &high, 0, sizeof(high));
		maWriteStore(highStore, highplaceholder);
	}
#endif	//USE_STORE

	return(quit);
}


void finish(void)
{
	/*XKeyboardControl vals;


	// Return display's bell pitch/duration to normal!

	vals.bell_pitch = kbstate.bell_pitch;
	vals.bell_duration = kbstate.bell_duration;

	XChangeKeyboardControl(display, KBBellPitch | KBBellDuration, &vals);
	XFlush(display);

	XDestroyWindow(display, window);
	XCloseDisplay(display);*/
}


void setup(void)
{
	//Window rootwindow;
	//int screen, black, white;
	//XSetWindowAttributes attr;
	//XWMHints wmhints;
	//char wname[64];
	//unsigned long attr_mask;
	//struct timeval tp;


	/* Options: */

	score = 0;
	sound = 0;//1;

	/*#ifdef REAL_KEYBOARD
	buttonmode = 0;
	#else
	buttonmode = 1;
	#endif*/


	/* Seed random number generator: */

	//gettimeofday(&tp, NULL);
	//srand(tp.tv_sec + tp.tv_usec);


	/* Connect to X server: */

	/*display = XOpenDisplay(NULL);
	if (display == NULL)
	{
	fprintf(stderr, "Can't connect to display!\n");
	exit(1);
	}

	screen = DefaultScreen(display);
	rootwindow = RootWindow(display, screen);*/


	/* Get primitive colors: */

	/*#ifndef INVERSE
	black = BlackPixel(display, screen);
	white = WhitePixel(display, screen);
	#else
	white = BlackPixel(display, screen);
	black = WhitePixel(display, screen);
	#endif*/


	/* Get current keyboard state (so we can put bell values back!) */

	//XGetKeyboardControl(display, &kbstate);


	/* Open window: */

	/*attr.event_mask = (KeyPressMask | KeyReleaseMask | ButtonPressMask |
	ExposureMask | VisibilityChangeMask);

	attr.border_pixel = black;
	attr.background_pixel = white;
	attr_mask = CWEventMask | CWBackPixel | CWBorderPixel;

	window = XCreateWindow(display, rootwindow, 0, 0, WIDTH, HEIGHT, 0,
	DefaultDepthOfScreen(DefaultScreenOfDisplay(display)),
	InputOutput, DefaultVisual(display, screen),
	attr_mask, &attr);*/


	/* Set input hints (so we appear in the window lists).
	Need to do this because of a bug in VRSM.  Not bad to do anyway, tho */

	/*wmhints.input = True;
	wmhints.flags |= InputHint;
	XSetWMHints(display, window, &wmhints);

	sprintf(wname, "Agendaroids");
	XChangeProperty(display, window, XA_WM_NAME, XA_STRING, 8,
	PropModeReplace, wname, strlen(wname));*/



	/* Create primitive graphics contexts: */

	//drawgc = CreateGC(display, window, black, black);
	//erasegc = CreateGC(display, window, white, black);


	/* Bring window up! */

	/*XMapWindow(display, window);
	XMapRaised(display, window);
	XSync(display, 0);*/
}


/* Create a graphics context: */

/*GC CreateGC(Display *display, Drawable drawable, unsigned long forecolor,
unsigned long backcolor)
{
XGCValues xgcvalues;
GC gc;

xgcvalues.foreground = forecolor;
xgcvalues.background = backcolor;
gc = XCreateGC(display,drawable,(GCForeground | GCBackground),
&xgcvalues);

return(gc);
}*/


/* Fast approximate-integer, table-based cosine! Whee! */

int fast_cos(int angle)
{
	angle = (angle % 45);
	while(angle < 0) {
		angle += 45;
	}

	if (angle < 12)
		return(trig[angle]);
	else if (angle < 23)
		return(-trig[10 - (angle - 12)]);
	else if (angle < 34)
		return(-trig[angle - 22]);
	else
		return(trig[45 - angle]);
}


/* Sine based on fast cosine... */

int fast_sin(int angle)
{
	return(- fast_cos((angle + 11) % 45));
}


/* Draw a line: */

void draw_line(int x1, int y1, int x2, int y2)
{
	maSetColor(-1);
	maLine(x1, y1, x2, y2);

	if (x1 < 0 || x2 < 0)
	{
		maLine(x1 + WIDTH, y1, x2 + WIDTH, y2);
	}
	else if (x1 >= WIDTH || x2 >= WIDTH)
	{
		maLine(x1 - WIDTH, y1, x2 - WIDTH, y2);
	}

	if (y1 < 0 || y2 < 0)
	{
		maLine(x1, y1 + HEIGHT, x2, y2 + HEIGHT);
	}
	else if (y1 >= HEIGHT || y2 >= HEIGHT)
	{
		maLine(x1, y1 - HEIGHT, x2, y2 - HEIGHT);
	}
}


/* Draw a line segment, rotated around a center point: */

void draw_segment(int r1, int a1,
				  int r2, int a2,
				  int cx, int cy, int a)
{
	draw_line(((fast_cos((a1 + a) >> 3) * r1) >> 10) + cx,
		cy - ((fast_sin((a1 + a) >> 3) * r1) >> 10),
		((fast_cos((a2 + a) >> 3) * r2) >> 10) + cx,
		cy - ((fast_sin((a2 + a) >> 3) * r2) >> 10));
}


/* Add a bullet: */

void add_bullet(int x, int y, int a, int xm, int ym)
{
	int i, found;

	found = -1;

	for (i = 0; i < NUM_BULLETS && found == -1; i++)
	{
		if (bullets[i].timer <= 0)
			found = i;
	}

	if (found != -1)
	{
		bullets[found].timer = 25;

		bullets[found].x = x;
		bullets[found].y = y;

		bullets[found].xm = ((fast_cos(a >> 3) * 5) >> 10) + (xm >> 4);
		bullets[found].ym = - ((fast_sin(a >> 3) * 5) >> 10) + (ym >> 4);
	}
}


/* Add an asteroid: */

void add_asteroid(int x, int y, int xm, int ym, int size)
{
	int i, found;


	/* Find a slot: */

	found = -1;

	for (i = 0; i < NUM_ASTEROIDS && found == -1; i++)
	{
		if (asteroids[i].alive == 0)
			found = i;
	}


	/* Hack: No asteroids should be stationary! */

	while (xm == 0)
	{
		xm = (rand() % 3) - 1;
	}


	if (found != -1)
	{
		asteroids[found].alive = 1;

		asteroids[found].x = x;
		asteroids[found].y = y;
		asteroids[found].xm = xm;
		asteroids[found].ym = ym;

		asteroids[found].angle = (rand() % 360);
		asteroids[found].angle_m = (rand() % 6) - 3;

		if(size <= 0)
			//Exit(112);  //size is used for division, must not be 0!
			size = 1;

		asteroids[found].size = size;

		for (i = 0; i < AST_SIDES; i++)
		{
			asteroids[found].shape[i].radius = (rand() % 3);
			asteroids[found].shape[i].angle = i * 60 + (rand() % 40);
		}
	}
}


/* Add a bit: */

void add_bit(int x, int y, int xm, int ym)
{
	int i, found;

	found = -1;

	for (i = 0; i < NUM_BITS && found == -1; i++)
	{
		if (bits[i].timer <= 0)
			found = i;
	}


	if (found != -1)
	{
		bits[found].timer = 16;

		bits[found].x = x;
		bits[found].y = y;
		bits[found].xm = xm;
		bits[found].ym = ym;
	}
}


/* Draw an asteroid: */

void draw_asteroid(int size, int x, int y, int angle, shape_type * shape)
{
	int i;


	/* Square asteroid: */

	/*
	draw_segment((size * AST_RADIUS), 45,
	(size * AST_RADIUS), 135,
	x, y,
	angle);

	draw_segment((size * AST_RADIUS), 135,
	(size * AST_RADIUS), 225,
	x, y,
	angle);

	draw_segment((size * AST_RADIUS), 225,
	(size * AST_RADIUS), 315,
	x, y,
	angle);

	draw_segment((size * AST_RADIUS), 315,
	(size * AST_RADIUS), 45,
	x, y,
	angle);
	*/


	/*
	draw_segment((size * AST_RADIUS), 0,
	(size * AST_RADIUS) / 3, 20,
	x, y,
	angle);

	draw_segment((size * AST_RADIUS) / 3, 20,
	(size * AST_RADIUS) / 2, 50,
	x, y,
	angle);

	draw_segment((size * AST_RADIUS) / 2, 50,
	(size * AST_RADIUS), 90,
	x, y,
	angle);

	draw_segment((size * AST_RADIUS), 90,
	(size * AST_RADIUS) / 2, 160,
	x, y,
	angle);

	draw_segment((size * AST_RADIUS) / 2, 160,
	(size * AST_RADIUS) / 3, 240,
	x, y,
	angle);

	draw_segment((size * AST_RADIUS) / 3, 240,
	(size * AST_RADIUS), 260,
	x, y,
	angle);

	draw_segment((size * AST_RADIUS), 260,
	(size * AST_RADIUS) / 2, 350,
	x, y,
	angle);

	draw_segment((size * AST_RADIUS) / 2, 350,
	(size * AST_RADIUS), 0,
	x, y,
	angle);
	*/


	for (i = 0; i < AST_SIDES - 1; i++)
	{
		draw_segment((size * (AST_RADIUS - shape[i].radius)),
			shape[i].angle,
			(size * (AST_RADIUS - shape[i + 1].radius)),
			shape[i + 1].angle,
			x, y,
			angle);
	}

	draw_segment((size * (AST_RADIUS - shape[AST_SIDES - 1].radius)),
		shape[AST_SIDES - 1].angle,
		(size * (AST_RADIUS - shape[0].radius)),
		shape[0].angle,
		x, y,
		angle);
}


/* Queue a sound! */

void playsound(int pitch, int duration)
{
	want_sound = 1;
	snd_pitch = pitch;
	snd_dur = duration;
}


/* Play the sound: */

void do_playsound(void)
{
	/*XKeyboardControl vals;

	if (sound)
	{
	vals.bell_pitch = snd_pitch;
	vals.bell_duration = snd_dur;
	vals.bell_percent = 100;

	XChangeKeyboardControl(display,
	(KBBellPitch | KBBellDuration | KBBellPercent),
	&vals);
	XBell(display, 100);
	XFlush(display);
	}*/
}


/* Break an asteroid and add an explosion: */

void hurt_asteroid(int j, int xm, int ym, int exp_size)
{
	int k;

	add_score(100 / (asteroids[j].size + 1));

	if (asteroids[j].size > 1)
	{
		/* Break the rock into two smaller ones! */

		add_asteroid(asteroids[j].x,
			asteroids[j].y,
			((asteroids[j].xm + xm) / 2),
			(asteroids[j].ym + ym),
			asteroids[j].size - 1);

		add_asteroid(asteroids[j].x,
			asteroids[j].y,
			(asteroids[j].xm + xm),
			((asteroids[j].ym + ym) / 2),
			asteroids[j].size - 1);
	}


	/* Make the original go away: */

	asteroids[j].alive = 0;


	/* Add explosion: */

	playsound(1000 / (asteroids[j].size + 1), 10);

	for (k = 0; k < exp_size; k++)
	{
		add_bit((asteroids[j].x -
			(asteroids[j].size * AST_RADIUS) +
			(rand() % (AST_RADIUS * 2))),
			(asteroids[j].y -
			(asteroids[j].size * AST_RADIUS) +
			(rand() % (AST_RADIUS * 2))),
			((rand() % (asteroids[j].size * 3)) -
			(asteroids[j].size) +
			((xm + asteroids[j].xm) / 3)),
			((rand() % (asteroids[j].size * 3)) -
			(asteroids[j].size) +
			((ym + asteroids[j].ym) / 3)));
	}
}


/* Increment score: */

void add_score(int amount)
{
	/* See if they deserve a new life: */

	if (score / ONEUP_SCORE < (score + amount) / ONEUP_SCORE)
	{
		lives++;
		strcpy(zoom_str, "EXTRA LIFE");
		text_zoom = ZOOM_START;
	}



	/* Add to score: */

	score = score + amount;


	/* Erase old score: */
	maSetColor(0);
	maFillRect(0, 0, WIDTH, HEIGHT);
}


/* Draw a character: */

void draw_char(char c, int x, int y, int r)
{
	int i, v;

	/* Which vector is this character? */

	v = -1;
	if (c >= '0' && c <= '9')
		v = (c - '0');
	else if (c >= 'A' && c <= 'Z')
		v = (c - 'A') + 10;


	if (v != -1)
	{
		for (i = 0; i < 5; i++)
		{
			if (char_vectors[v][i][0] != -1)
			{
				draw_line(x + (char_vectors[v][i][0] * r),
					y + (char_vectors[v][i][1] * r),
					x + (char_vectors[v][i][2] * r),
					y + (char_vectors[v][i][3] * r));
			}
		}
	}
}


void draw_text(const char * str, int x, int y, int s)
{
	int i;

	for (i = 0; i < (int)strlen(str); i++)
		draw_char(str[i], i * (s + 3) + x, y, s);
}


/* Draw the close box at the bottom left of the screen: */

void draw_closebox(void)
{
	draw_line( 0, HEIGHT - 16, 15, HEIGHT - 16);
	draw_line(15, HEIGHT - 16, 15, HEIGHT - 1);
	draw_line(15, HEIGHT - 1,  0, HEIGHT - 1);
	draw_line( 0, HEIGHT - 1,  0, HEIGHT - 16);

	draw_line( 0, HEIGHT - 16, 15, HEIGHT - 1);
	draw_line( 0, HEIGHT - 1, 15, HEIGHT - 16);
}


/* Draw button mode toggle: */

/*void draw_buttontoggle(void)
{
if (buttonmode == 1)
draw_text("D", WIDTH / 2 - 3, HEIGHT - 11, 5);
else if (buttonmode == 0)
draw_text("A", WIDTH / 2 - 3, HEIGHT - 11, 5);
}*/


/* Draw the sound toggle switch: */

void draw_soundtoggle(void)
{
	draw_line(WIDTH - 16, HEIGHT - 16, WIDTH - 1, HEIGHT - 16);
	draw_line(WIDTH - 1, HEIGHT - 16, WIDTH - 1, HEIGHT - 1);
	draw_line(WIDTH - 1, HEIGHT - 1, WIDTH - 16, HEIGHT - 1);
	draw_line(WIDTH - 16, HEIGHT - 1, WIDTH - 16, HEIGHT - 16);


	draw_line(WIDTH - 5, HEIGHT - 15, WIDTH - 5, HEIGHT - 2);
	draw_line(WIDTH - 5, HEIGHT - 2, WIDTH - 10, HEIGHT - 5);
	draw_line(WIDTH - 10, HEIGHT - 5, WIDTH - 10, HEIGHT - 10);
	draw_line(WIDTH - 10, HEIGHT - 10, WIDTH - 5, HEIGHT - 15);

	if (sound == 0)
	{
		draw_thick_line(WIDTH - 16, HEIGHT - 1, WIDTH - 1, HEIGHT - 16);
	}
}



void draw_thick_line(int x1, int y1, int x2, int y2)
{
	draw_line(x1, y1, x2, y2);
	draw_line(x1 + 1, y1 + 1, x2 + 1, y2 + 1);
}


void reset_level(void)
{
	int i;


	for (i = 0; i < NUM_BULLETS; i++)
		bullets[i].timer = 0;

	for (i = 0; i < NUM_ASTEROIDS; i++)
		asteroids[i].alive = 0;

	for (i = 0; i < NUM_BITS; i++)
		bits[i].timer = 0;

	for (i = 0; i < (level + 1) && i < 10; i++)
	{
		add_asteroid(/* x */ (rand() % WIDTH),
			/* y */ (rand() % 20) + (220 * (rand() % 2)),
			/* xm */ ((rand() % 5) - 2) * 3,
			/* ym */ (rand() % 5) - 2,
			/* size */ (rand() % 3) + 2);
	}


	my_sprinti(zoom_str, "LEVEL ", level);

	text_zoom = ZOOM_START;
}

