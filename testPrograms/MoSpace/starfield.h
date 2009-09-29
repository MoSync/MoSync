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

typedef struct {
	int x;
	int y;
} Star;

#define NUM_STARS 64
#define NUM_LAYERS 3

Star stars[NUM_LAYERS][NUM_STARS];
int star_colors[NUM_LAYERS];
#define RGB(r, g, b) (((r) << 16) | ((g) << 8) | (b))
Extent screen_size;

void initStars() {
	int i,j;
	screen_size = maGetScrSize();
	for(i = 0; i < NUM_LAYERS; i++) {
		for(j = 0; j < NUM_STARS; j++) {
			stars[i][j].x = rand() % EXTENT_X(screen_size);
			stars[i][j].y = rand() % EXTENT_Y(screen_size);
		}
		star_colors[i] = 255-(NUM_LAYERS-1-i)*80;
	}
}

void updateStars() {
	int i,j;
	int c;
	Star* s;

	for(i = 0; i < NUM_LAYERS; i++) {
		s = (Star *) &stars[i][0];
		j = NUM_STARS;
		c = star_colors[i];
		maSetColor(RGB(c,c,c));
		
		while(j--) {
			s->y+=i+1;
			if(s->y > EXTENT_Y(screen_size)) {
				s->y = 0;
			}
			maPlot(s->x, s->y);
			s++;
		}
	}
}

