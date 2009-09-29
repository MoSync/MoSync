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

#include "Part1.h"
#include <madmath.h>
#include "trig.h"

#define GRID_XSIZE 12
#define GRID_YSIZE 12

#define GRID_MAG 100.0

#define OBJ_X 50.0
#define OBJ_Y 0.0

#define OBJ_Z (4000.0 + 3000.0/((dtime*0.8)*(dtime)*dtime + 0.4) - dtime*200)


Part1::Part1() {
	
	Extent scrSize = maGetScrSize();
	screenWidth = EXTENT_X(scrSize);
	screenHeight = EXTENT_Y(scrSize);
	
	points = new GridPoint[GRID_XSIZE*GRID_YSIZE];
	// Generate grid
	int pointIdx = 0;
	for(int i = 0; i < GRID_YSIZE; i++) {
		for(int j = 0; j < GRID_XSIZE; j++) {
			points[pointIdx].set(
				(j-GRID_XSIZE/2)*GRID_MAG,
				(i-GRID_YSIZE/2)*GRID_MAG,
				0.0
			);
			pointIdx++;
		}
	}
}

void Part1::render(int time, double dtime) {
	// clear screen
	maSetColor(0x000000);
	maFillRect(0,0, screenWidth, screenHeight);
	// transform all points
	GridPoint* p = points;	
	
	double rotx = dtime;//dtime*2.0;
	double roty = 0.0;//dtime*4.0;
	double rotz = -1.1;//dtime*1.0;
	
	/// Do some precalculations for the rotations using fixed point math, and the macros for retrieving lookup table sinus values.
	double c1=cos(rotx), s1=sin(rotx),
		c2=cos(roty), s2=sin(roty),
		c3=cos(rotz), s3=sin(rotz);
	double xx, xy, xz, yx, yy, yz, zx, zy, zz;
	xx=(c2*c1);
	xy=(c2*s1);
	xz=s2;
	yx=(c3*s1)+s3*(s2*c1);
	yy=-c3*c1+s3*(s2*s1);
	yz=(-s3*c2);
	zx=(s3*s1)-((c3*s2)*c1);
	zy=(-s3*c1)-(c3*s2)*s1;
	zz=(c3*c2);


	for(int i = 0; i < GRID_XSIZE*GRID_YSIZE; i++) {
		// first, modify z coordinate using weir sines...
		double centerDist = sqrt(p->x*p->x + p->y*p->y);
		double ang = atan2(p->y, p->x);
		double tempx = p->x;
		double tempy = p->y;
		
		double hiAmp = (60.0*cos(3.14*sin(dtime*dtime*0.1))) - 30*cos(ang*8+dtime*0.1);
		double hiAmp2 = sin(tempx*10 + dtime*10);
		double hiAmp3 = cos(tempx*10 + dtime*10);

		double zoff1 = 40* sin(tempx*50)*hiAmp2 + 20* sin(tempx*80)*hiAmp3;
		if(hiAmp < 0) hiAmp = 0;
		
		double tempz = p->z + 60.0*sin(centerDist*0.01 + dtime*6.0) + hiAmp * cos(centerDist*0.07 + dtime*2.0) + zoff1;

		/// Rotate object.
		p->transx =(xx * tempx + xy * tempy + xz * tempz);
		p->transy =(yx * tempx + yy * tempy + yz * tempz);
		p->transz =(zx * tempx + zy * tempy + zz * tempz);
		/// Translate object.
		p->transx += OBJ_X;
		p->transy += OBJ_Y;
		p->transz += OBJ_Z;
		
		int z = p->transz;
		if(!z) z = 1;
		p->projx = screenWidth/2  + screenWidth * p->transx / z;
		p->projy = screenHeight/2 + screenWidth * p->transy / z;
		
		p++;
	}
	
	// Do actual rendering of lines
	
	maSetColor(0xffffff);
	
	// draw "vertical" lines
	for(int i = 0; i < GRID_XSIZE; i++) {
		for(int j = 0; j < GRID_YSIZE-1; j++) {
			int startIdx = i + j*GRID_XSIZE;
			int endIdx   = i + (j+1)*GRID_XSIZE;

			int x1 = points[startIdx].projx;
			int y1 = points[startIdx].projy;
			int x2 = points[endIdx].projx;
			int y2 = points[endIdx].projy;
			
			double cepAmp = 18*sin(dtime*2) - 12;
			double cepAmp2 = 18*cos(dtime*2) -12;
			if(cepAmp < 0) cepAmp = 0;
			if(cepAmp2 < 0) cepAmp2 = 0;
			
			x1 += cepAmp*sin(i*0.3 + dtime*20);
			x2 += cepAmp*cos(i*0.2 + dtime*36);

			y1 += cepAmp2*cos(i*0.1 + dtime*30);
			y2 += cepAmp2*sin(i*0.4 + dtime*12);
			
			int b = 190 + 60*cos(cepAmp + dtime*2.0 + i*0.3);
			int r = 50 + 30*sin(dtime*4 + j*0.1) - b/16;
			
			int col = b | (r << 8) | (r << 16);
			maSetColor(col);
			
			maLine(x1, y1, x2, y2);
		}
	}

	// draw "horizontal" lines
	for(int i = 0; i < GRID_YSIZE; i++) {
		for(int j = 0; j < GRID_XSIZE-1; j++) {
			int startIdx = j + i*GRID_XSIZE;
			int endIdx   = (j+1) + i*GRID_XSIZE;

			int x1 = points[startIdx].projx;
			int y1 = points[startIdx].projy;
			int x2 = points[endIdx].projx;
			int y2 = points[endIdx].projy;
			
			int b = 190 + 60*cos(dtime*2.0 + i*0.3);
			int r = 50 + 30*sin(dtime*4 + j*0.1);
			
			int col = b | (r << 8) | (r << 16);
			maSetColor(col);
			
			
			maLine(x1, y1, x2, y2);
		}
	}
	
}

