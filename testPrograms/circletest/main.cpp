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
