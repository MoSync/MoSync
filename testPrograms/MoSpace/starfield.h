/* Copyright 2013 David Axmark

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

	http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
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

