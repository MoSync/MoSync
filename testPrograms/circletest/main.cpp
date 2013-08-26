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

#include <ma.h>

int sqrTable[256];
void initMulTable() {
	for(int i = 0; i < 256; i++) {
		sqrTable[i] = i*i;
	}
}

void fillCircle(int x, int y, int r, int col)
{
	if(r<0) r = -r;
	int r2 = sqrTable[r];
	int startx = r;
	maSetColor(col);	
	/*
	if(r < 8) {
	*/
		// draw scanlines
		for(int i = 0; i <= r; i++)
		{
			for(int j = startx; j >= 0; j--)
			{
				if(sqrTable[i]+sqrTable[j] <= r2) {
					maLine(x-j, y-i, x+j, y-i);
					maLine(x-j, y+i, x+j, y+i);
					startx = j;
					break;
				}
			}
		}	
		/*
	} else{
		// draw filled rect and fill missing parts with scan lines
		int bound = (r*7071067)/10000000;	
		maFillRect(x-bound, y-bound, bound<<1, bound<<1);
		for(int i = 0; i <= bound; i++) {
			for(int j = startx; j >= bound; j--) {
				if(sqrTable[i]+sqrTable[j] <= r2) {
					maLine(x-j, y-i, x-bound, y-i);
					maLine(x+j, y-i, x+bound, y-i);
					maLine(x-j, y+i, x-bound, y+i);
					maLine(x+j, y+i, x+bound, y+i);			
					maLine(x-i, y-j, x-i, y-bound);
					maLine(x-i, y+j, x-i, y+bound);
					maLine(x+i, y-j, x+i, y-bound);
					maLine(x+i, y+j, x+i, y+bound);
					startx = j;
					break;								
				}			
			}
		}
	}
	*/
}

#define RGB(r, g, b) (((r)<<16)|((g)<<8)|(b))


extern "C" int MAMain() {
	Extent e = maGetScrSize();
	int width = EXTENT_X(e);
	int height = EXTENT_Y(e);
	
	initMulTable();
	while(1)
	{
		EVENT event;
		while(maGetEvent(&event)!=0) {
			switch(event.type) {
				case EVENT_TYPE_CLOSE:
				case EVENT_TYPE_KEY_PRESSED:
						maExit(0);
					break;
			}
		}
		
		double time = maGetMilliSecondCount()*0.0001;
	
		maSetColor(0);
		maFillRect(0, 0, width, height);
		
		int deltax = width/28;
		int deltay = height/40;
		int sx = deltax<<1;
		int sy = deltay;
		
		for(int y = 0; y < 20; y++)
		{
			sx = deltax;
			for(int x = 0; x <= 14; x++)
			{
				int xx = 7-x;
				if(xx<0) xx = -xx;
				int yy = 10-y;
				if(yy<0) yy = -yy;				
				int dist = sqrTable[xx] + sqrTable[yy];
				
				double frac = sin(time*10+dist*0.01);	
				int col = (int)(frac*128.0 + 128.0);
				fillCircle(sx, sy, (int)(frac*deltax), RGB(col, col, col)); 
				sx+=deltax<<1;
			}
			sy+=deltay<<1;
		}
		
		/*
		for(int i = 0; i < 30; i++) {
			time += ((double)i*0.01);
			fillCircle(120 + (int)(cos(time*1.2)*100), 160 + (int)(sin(time*1.3)*100), (int)(sin(time*2)*8)+16, 0x080000*i);
		}
		*/
		
		maUpdateScreen();
	}
	
	return 0;
}
