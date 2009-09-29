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
#include <mastring.h>
#include <mavsprintf.h>
#include "MAHeaders.h"
#include "Font.c"

#include <mastdlib.h>

#ifndef _WIN32
#include "mavsprintf.h"
#endif

//#include "madmath.h"
#include <madmath.h>

#ifdef _WIN32
//#include <conio.h>
#endif

#include "Math/Vector3.c"

int scrW, scrH;

int sinTab[1024] = {
0,
402,804,1206,1608,2010,2412,2813,3215,3617,4018,4419,4821,5221,5622,6023,6423,
6823,7223,7622,8022,8421,8819,9218,9615,10013,10410,10807,11203,11599,11995,12390,12785,
13179,13573,13966,14358,14750,15142,15533,15923,16313,16702,17091,17479,17866,18252,18638,19023,
19408,19791,20174,20557,20938,21319,21699,22078,22456,22833,23210,23585,23960,24334,24707,25079,
25450,25820,26189,26557,26924,27290,27655,28019,28382,28744,29105,29465,29823,30181,30537,30892,
31247,31599,31951,32302,32651,32999,33346,33691,34035,34378,34720,35061,35400,35737,36074,36409,
36742,37075,37406,37735,38063,38390,38715,39039,39361,39682,40001,40319,40635,40950,41263,41574,
41885,42193,42500,42805,43109,43411,43711,44010,44307,44603,44896,45189,45479,45768,46055,46340,
46623,46905,47185,47463,47739,48014,48287,48558,48827,49094,49360,49623,49885,50145,50403,50659,
50913,51165,51415,51664,51910,52155,52397,52638,52876,53113,53347,53580,53810,54039,54265,54490,
54712,54933,55151,55367,55581,55793,56003,56211,56416,56620,56821,57021,57218,57413,57606,57796,
57985,58171,58355,58537,58717,58894,59069,59242,59413,59582,59748,59912,60074,60234,60391,60546,
60699,60849,60997,61143,61287,61428,61567,61704,61838,61970,62100,62227,62352,62474,62595,62713,
62828,62941,63052,63161,63267,63370,63472,63570,63667,63761,63853,63942,64029,64114,64196,64275,
64353,64427,64500,64570,64637,64702,64765,64825,64883,64938,64991,65042,65090,65135,65178,65219,
65257,65293,65326,65357,65385,65411,65435,65456,65474,65490,65504,65515,65523,65530,65533,65535,
65533,65530,65523,65515,65504,65490,65474,65456,65435,65411,65385,65357,65326,65293,65257,65219,
65178,65135,65090,65042,64991,64938,64883,64825,64765,64702,64637,64570,64500,64427,64353,64275,
64196,64114,64029,63942,63853,63761,63667,63570,63472,63370,63267,63161,63052,62941,62828,62713,
62595,62474,62352,62227,62100,61970,61838,61704,61567,61428,61287,61143,60997,60849,60699,60546,
60391,60234,60074,59912,59748,59582,59413,59242,59069,58894,58717,58537,58355,58171,57985,57796,
57606,57413,57218,57021,56821,56620,56416,56211,56003,55793,55581,55367,55151,54933,54712,54490,
54265,54039,53810,53580,53347,53113,52876,52638,52397,52155,51910,51664,51415,51165,50913,50659,
50403,50145,49885,49623,49360,49094,48827,48558,48287,48014,47739,47463,47185,46905,46623,46340,
46055,45768,45479,45189,44896,44603,44307,44010,43711,43411,43109,42805,42500,42193,41885,41574,
41263,40950,40635,40319,40001,39682,39361,39039,38715,38390,38063,37735,37406,37075,36742,36409,
36074,35737,35400,35061,34720,34378,34035,33691,33346,32999,32651,32302,31951,31599,31247,30892,
30537,30181,29823,29465,29105,28744,28382,28019,27655,27290,26924,26557,26189,25820,25450,25079,
24707,24334,23960,23585,23210,22833,22456,22078,21699,21319,20938,20557,20174,19791,19408,19023,
18638,18252,17866,17479,17091,16702,16313,15923,15533,15142,14750,14358,13966,13573,13179,12785,
12390,11995,11599,11203,10807,10410,10013,9615,9218,8819,8421,8022,7622,7223,6823,6423,
6023,5622,5221,4821,4419,4018,3617,3215,2813,2412,2010,1608,1206,804,402,0,
-402,-804,-1206,-1608,-2010,-2412,-2813,-3215,-3617,-4018,-4419,-4821,-5221,-5622,-6023,-6423,
-6823,-7223,-7622,-8022,-8421,-8819,-9218,-9615,-10013,-10410,-10807,-11203,-11599,-11995,-12390,-12785,
-13179,-13573,-13966,-14358,-14750,-15142,-15533,-15923,-16313,-16702,-17091,-17479,-17866,-18252,-18638,-19023,
-19408,-19791,-20174,-20557,-20938,-21319,-21699,-22078,-22456,-22833,-23210,-23585,-23960,-24334,-24707,-25079,
-25450,-25820,-26189,-26557,-26924,-27290,-27655,-28019,-28382,-28744,-29105,-29465,-29823,-30181,-30537,-30892,
-31247,-31599,-31951,-32302,-32651,-32999,-33346,-33691,-34035,-34378,-34720,-35061,-35400,-35737,-36074,-36409,
-36742,-37075,-37406,-37735,-38063,-38390,-38715,-39039,-39361,-39682,-40001,-40319,-40635,-40950,-41263,-41574,
-41885,-42193,-42500,-42805,-43109,-43411,-43711,-44010,-44307,-44603,-44896,-45189,-45479,-45768,-46055,-46340,
-46623,-46905,-47185,-47463,-47739,-48014,-48287,-48558,-48827,-49094,-49360,-49623,-49885,-50145,-50403,-50659,
-50913,-51165,-51415,-51664,-51910,-52155,-52397,-52638,-52876,-53113,-53347,-53580,-53810,-54039,-54265,-54490,
-54712,-54933,-55151,-55367,-55581,-55793,-56003,-56211,-56416,-56620,-56821,-57021,-57218,-57413,-57606,-57796,
-57985,-58171,-58355,-58537,-58717,-58894,-59069,-59242,-59413,-59582,-59748,-59912,-60074,-60234,-60391,-60546,
-60699,-60849,-60997,-61143,-61287,-61428,-61567,-61704,-61838,-61970,-62100,-62227,-62352,-62474,-62595,-62713,
-62828,-62941,-63052,-63161,-63267,-63370,-63472,-63570,-63667,-63761,-63853,-63942,-64029,-64114,-64196,-64275,
-64353,-64427,-64500,-64570,-64637,-64702,-64765,-64825,-64883,-64938,-64991,-65042,-65090,-65135,-65178,-65219,
-65257,-65293,-65326,-65357,-65385,-65411,-65435,-65456,-65474,-65490,-65504,-65515,-65523,-65530,-65533,-65535,
-65533,-65530,-65523,-65515,-65504,-65490,-65474,-65456,-65435,-65411,-65385,-65357,-65326,-65293,-65257,-65219,
-65178,-65135,-65090,-65042,-64991,-64938,-64883,-64825,-64765,-64702,-64637,-64570,-64500,-64427,-64353,-64275,
-64196,-64114,-64029,-63942,-63853,-63761,-63667,-63570,-63472,-63370,-63267,-63161,-63052,-62941,-62828,-62713,
-62595,-62474,-62352,-62227,-62100,-61970,-61838,-61704,-61567,-61428,-61287,-61143,-60997,-60849,-60699,-60546,
-60391,-60234,-60074,-59912,-59748,-59582,-59413,-59242,-59069,-58894,-58717,-58537,-58355,-58171,-57985,-57796,
-57606,-57413,-57218,-57021,-56821,-56620,-56416,-56211,-56003,-55793,-55581,-55367,-55151,-54933,-54712,-54490,
-54265,-54039,-53810,-53580,-53347,-53113,-52876,-52638,-52397,-52155,-51910,-51664,-51415,-51165,-50913,-50659,
-50403,-50145,-49885,-49623,-49360,-49094,-48827,-48558,-48287,-48014,-47739,-47463,-47185,-46905,-46623,-46340,
-46055,-45768,-45479,-45189,-44896,-44603,-44307,-44010,-43711,-43411,-43109,-42805,-42500,-42193,-41885,-41574,
-41263,-40950,-40635,-40319,-40001,-39682,-39361,-39039,-38715,-38390,-38063,-37735,-37406,-37075,-36742,-36409,
-36074,-35737,-35400,-35061,-34720,-34378,-34035,-33691,-33346,-32999,-32651,-32302,-31951,-31599,-31247,-30892,
-30537,-30181,-29823,-29465,-29105,-28744,-28382,-28019,-27655,-27290,-26924,-26557,-26189,-25820,-25450,-25079,
-24707,-24334,-23960,-23585,-23210,-22833,-22456,-22078,-21699,-21319,-20938,-20557,-20174,-19791,-19408,-19023,
-18638,-18252,-17866,-17479,-17091,-16702,-16313,-15923,-15533,-15142,-14750,-14358,-13966,-13573,-13179,-12785,
-12390,-11995,-11599,-11203,-10807,-10410,-10013,-9615,-9218,-8819,-8421,-8022,-7622,-7223,-6823,-6423,
-6023,-5622,-5221,-4821,-4419,-4018,-3617,-3215,-2813,-2412,-2010,-1608,-1206,-804,-402
};
// 16-bit fixed-point sine table. 

//#define M_PI ((double)3.141592653589)

//int sinTab[256];

#define SINE(x) (sinTab[(x)&0x3ff])
#define COSINE(x) (sinTab[(256+(x))&0x3ff])

/*void initSinTab() {
	int i = 0; 

	for(i = 0; i < 256; i++) {
		double t = sin(((double)i*M_PI)/256.0);
		sinTab[i] = (int)(t*65535.0);
	}
} */

/*void clear() {
	Extent scrSize = maGetScrSize();
	int scrW = EXTENT_X(scrSize);
	int scrH = EXTENT_Y(scrSize);

	maSetColor(0);
	maFillRect(0,0,scrW,scrH);
} */

#define NUMBOBS 64
#define MULANG(x) ((x)*3)



typedef struct Vector3int_t {
	int x, y, z;
} Vector3int;

#define NUMVBALLS 7*7

double random() {
	return ((double)rand())/(double)RAND_MAX;
}

void generateSphere(Vector3int* dest, int num, double size) {
	int i;
	for(i=0; i < num; i++) {
		Vector3 tmp;
		tmp.x = random()-0.5;
		tmp.y = random()-0.5;
		tmp.z = random()-0.5;

		Vec3_normalize(&tmp);

		dest[i].x = (int)(tmp.x*size);
		dest[i].y = (int)(tmp.y*size);
		dest[i].z = (int)(tmp.z*size);
	}
}

void generateRandPoints(Vector3int *dest, int num, int max) {
	int i;
	for(i = 0; i < num; i++) {
		dest[i].x = (rand()%max)-(rand()%max);
		dest[i].y = (rand()%max)-(rand()%max);
		dest[i].z = (rand()%max)-(rand()%max);
	}
}

void generatePlane(Vector3int* dest, int num, int size) {

	int s = (int)sqrt(num);
	int i,j;
	int ballIdx = 0;

	for(i=0; i < s; i++) {
		int x = (i-s/2)*size;
		for(j = 0; j < s; j++) {
			int y = (j-s/2)*size;
			dest[ballIdx].x = x;
			dest[ballIdx].y = y;
			dest[ballIdx].z = (COSINE((x)/10)>>6)+(COSINE((y)/10)>>6);
			ballIdx++;
		}
	}
}

void generateCircle(Vector3int* dest, int num, int size) {
}


void rotateX(int ang, Vector3int* src, Vector3int* dest) {
	// x rotation
	dest->y = (src->y*COSINE(ang) - src->z*SINE  (ang))>>16;
	dest->z = (src->y*SINE  (ang) + src->z*COSINE(ang))>>16;
	dest->x = src->x;
}


void rotateY(int ang, Vector3int* src, Vector3int* dest) {
	// y rotation
	dest->z = (src->z*COSINE(ang) - src->x*SINE  (ang))>>16;
	dest->x = (src->z*SINE  (ang) + src->x*COSINE(ang))>>16;
	dest->y = src->y;
}


void rotateZ(int ang, Vector3int* src, Vector3int* dest) {
	// z rotation
	dest->x = (src->x*COSINE(ang) - src->y*SINE  (ang))>>16;
	dest->y = (src->x*SINE  (ang) + src->y*COSINE(ang))>>16;
	dest->z = src->z;
}


void rotatePoints(int xq, int yq, int zq, Vector3int* src, Vector3int* dest, int numPoints) {
	int i;

	for(i=0; i < numPoints; i++) {
		Vector3int temp1,temp2;

		rotateX(xq, &src[i], &temp1);
		rotateY(yq, &temp1, &temp2);
		rotateZ(zq, &temp2, &dest[i]);


	}
}

Vector3int rotPoints[NUMVBALLS];


void quickSort(Vector3int* point, int* indexList, int lo, int hi) {
	int i=lo, j=hi, h;
	int x=point[indexList[(lo+hi)/2]].z;
	//int s;

	do {
		while (point[indexList[i]].z > x) i++; 
		while (point[indexList[j]].z < x) j--;
		if (i<=j) {
			h = indexList[i];
			indexList[i] = indexList[j];
			indexList[j] = h;

			i++; j--;
		}
	} while (i<=j);

	if (lo<j) quickSort(point, indexList, lo, j);
	if (i<hi) quickSort(point, indexList, i, hi);
}

void quickSortWrap(Vector3int* point, int* indexList, int numPoints) {
	quickSort(point, indexList, 0, numPoints-1);
}


int indexList[NUMVBALLS];
char dispString[256];

Vector3int morphPoints[NUMVBALLS];


void initIndices() {
	int i;
	for(i = 0; i < NUMVBALLS;i++) {
		indexList[i] = i;
		//rotPoints[i].z += zOff;
	} 
}


void renderPoints(Vector3int* point, int numPoints, int zOff, int xAng, int yAng, int zAng) {

	int i;//, j, temp;
	Extent scrSize = maGetScrSize();
	int scrW = EXTENT_X(scrSize);
	int scrH = EXTENT_Y(scrSize);

	//ang++;

	// rotate points

	//rotatePoints(ang,ang*2,-ang*2, point, rotPoints, numPoints);
	rotatePoints(xAng, yAng, zAng, point, rotPoints, numPoints);
		
	// sort points by z


	//radixSort(rotPoints, indexList, numPoints);
	quickSortWrap(rotPoints, indexList, numPoints);

	// draw them as balls

	for(i = 0; i < numPoints; i++) {
		Vector3int* p = &rotPoints[indexList[i]];
		if(p->z+zOff > 0) {
			int scrX = (scrW>>1) + (p->x<<8)/(p->z+zOff)-8;
			int scrY = (scrH>>1) + (p->y<<8)/(p->z+zOff)+6;
			maDrawImage(1,scrX,scrY);
		}
	}
}

void renderMorph(Vector3int* src, Vector3int* dest, int numPoints, int alpha, int zOff, int xAng, int yAng, int zAng) {
	int i;//, j, temp;
/*	Extent scrSize = maGetScrSize();
	int scrW = EXTENT_X(scrSize);
	int scrH = EXTENT_Y(scrSize); */

	//ang+=4;

	if(alpha > 255) alpha = 255;
	if(alpha < 0) alpha = 0;

	for(i = 0; i < numPoints; i++) {
		morphPoints[i].x = src[i].x+(((dest[i].x-src[i].x)*alpha)>>8);
		morphPoints[i].y = src[i].y+(((dest[i].y-src[i].y)*alpha)>>8);
		morphPoints[i].z = src[i].z+(((dest[i].z-src[i].z)*alpha)>>8);
	}

	//rotatePoints(ang,ang*2,-ang, point, morphPoints, numPoints);

	renderPoints(morphPoints, numPoints, zOff, xAng, yAng, zAng);
}

void maDrawSineString(Handle font, const char* str, int y, int amp, int freq) {

	static int scroll = 0;
	static int scrollVal = 0;
	static int charIdx = 0;

	MAFont* f = &fonts[font];
	int charsPerLine = f->charsPerLine; //5
	MARect srcRect = {0, 0, f->charWidth, f->charHeight};
	MAPoint2d destPoint = {0,y};

	int i = 0;
	int l = strlen(str);

	int numCharacters = (scrW/f->charWidth) + 1;

//	while(*str[(charIdx+il]) {
	for(i = 0; i < numCharacters; i++) {
		int idx;
		int row;
		int col;
		if(charIdx+i >= l) {
			charIdx = 0;
			break;
		}

		if(str[charIdx+i] == ' ') continue;
		
		idx = ((int)str[charIdx+i])-f->asciiStart; // 6
		row = idx/charsPerLine; // 0
		col = idx % charsPerLine;

		srcRect.left = col * f->charWidth;
		srcRect.top = row * f->charHeight;

		destPoint.x = i*f->charWidth - scrollVal;
		destPoint.y = y + ( SINE(((i+charIdx)<<5)+(scroll<<3)) >> 10);

		maDrawImageRegion(f->handle, &srcRect, &destPoint, 0);
	}

	scroll++;
	scrollVal = scroll & 31;

	if(scrollVal == 0) {
		charIdx++;
	}
}

void clear() {
	Extent scrSize = maGetScrSize();
	int scrW = EXTENT_X(scrSize);
	int scrH = EXTENT_Y(scrSize);
	maSetColor(0);
	maFillRect(0,0,scrW,scrH);
}

void drawCircle(int sx, int sy, int r, int col)
{
	int x,x1,y,n;
	
	x = r << 8;
	y = 0;

	maSetColor(col);
	
	for (n=0;n<100;n++)
	{
		int px = x;
		int py = y;
		x1 = x - (y >> 4);
		y  = y + (x1 >> 4);
		x = x1; 

		maLine(sx+(px>>8), sy+(py>>8), sx+(x>>8), sy+(y>>8));

		px = x;
		py = y;
	
		//maPlot(sx + (x>>8), sy + (y>>8));
		
	}
}

/*
Handle maInitLayer(Handle tileResource, Handle mapResource, int layerSizeX, int layerSizeY);
void maDisposeLayer(Handle layer);
void maSetMap(Handle layer, Handle srcMapResource, int destX, int destY);
void maChangeTileSet(Handle layer, Handle tileResource);
void maDrawLayer(Handle layerHand, int offsetX, int offsetY);
void maSetTile(Handle layer, int x, int y, int tileNumber);
*/

void testTiles() {
	int x = 0, y = 0;
	Handle lHand = maInitLayer(4,5,8,8);
	Handle lHand2 = maInitLayer(4,5,8,8);
	maSetMap(lHand, 5, -2, -2);
	maSetTile(lHand, 0,0, 42);

	maSetTile(lHand2, 2,2, 0);


	while(1) {

		//int keys = maGetKeys();
		int keys = 0;
		
		if(keys & MAK_LEFT)
			x--;
		if(keys & MAK_RIGHT)
			x++;
		if(keys & MAK_UP)
			y--;
		if(keys & MAK_DOWN)
			y++;

		maSetColor(0);
		maFillRect(0, 0, EXTENT_X(maGetScrSize()), EXTENT_Y(maGetScrSize()));

		maDrawLayer(lHand, 0, 0);
		maDrawLayer(lHand2, x, y);
		maUpdateScreen();
		//x++;
		//x%=100;
	}
}

#define EXTENT(x, y) ((Extent)((((int)(x)) << 16) | ((y) & 0xFFFF)))

int MAMain() {

	if(maCheckInterfaceVersion(MAIDL_HASH) != (int)MAIDL_HASH) {
		maPanic(1, "Interface version mismatch!");
	}
	return 0;
}
#if 0
	EVENT event;
	char buf[256];
	int frames = 0;
	int fps = 0;
	int startTime = 0, sTime = 0;
	
	Extent scrSize = maGetScrSize();
	Vector3int ball[NUMVBALLS];
	Vector3int plane[NUMVBALLS];
	int t = 0;

	Handle fHand1 = maInitFont(6, EXTENT(32,32), EXTENT(320,200), 65);
	Handle fHand2 = maInitFont(font2, EXTENT(8,8), EXTENT(320,24), 32);

	scrW = EXTENT_X(scrSize);
	scrH = EXTENT_Y(scrSize);

	if(maCheckInterfaceVersion(MAIDL_HASH) != (int)MAIDL_HASH) {
		maPanic(1, "Interface version mismatch!");
	}

	maSoundPlay(music1);


	//testTiles();

	maDrawText(10,10,"hej");
	maUpdateScreen();

	generateSphere(ball, NUMVBALLS, 4000);
	generatePlane(plane, NUMVBALLS, 1000);

	//initSinTab();

	//int frames = 0;

	sTime = startTime = maGetMilliSecondCount();
	sprintf(buf, "fps 0");

	initIndices();

	
	
/*void maSoundInit(int chan, Handle sound_res, int loopCount);
void maSoundStop(int chan);
void maSoundStart(int chan); */

	while(1) {

		//int i;
		//int t1 = t;//(t*100)/256;

		int i;
		int time = maGetMilliSecondCount()-sTime;
		int t1 = time/4;
		//int ang = time/4;

		clear();

		if(t1 < 1200) 
			for(i=0; i < NUMBOBS; i++) {
				int j = MULANG(i);
				int x = scrW/2 + (COSINE((4*j)+t1*3) >> 10)-8;
				int y = (scrH-(65535>>11)-32) + (SINE  ((5*j)+t1*2) >> 11)-8;
				maDrawImage(1,x,y);
				//maDrawImage(1,scrW-x,y);
			} 
		else
			renderMorph(plane, ball, NUMVBALLS, SINE(time/20)/128, 20020, time/3, time/4, -time/5);

		//clear();

		//maDrawImage(3,0,0);

		//maDrawImage(6,0,0);

		//maDrawChar(fHand, 'D', 2,2);
		maDrawString(fHand1, "MOSYNC", -(EXTENT_X(maGetStringExtents(fHand1, "MOSYNC"))>>1) + (scrW>>1)+(SINE(time>>1)>>11),2);
		maDrawString(fHand2, "Copyright (c) 2006", 2, scrH-20);
		maDrawString(fHand2, "Mobile Sorcery AB", 2, scrH-10);
		//maDrawSineString(fHand1, "              MOBILE SORCERY BRINGS YOU ANOTHER LIGHTSPEED RELEASE CALLED MOSYNC                ", 100, 50, 10);

#ifdef _WIN32
		maWait(10);
#endif
		t++;

		frames++;

		if(frames > 25) {
			if(time-startTime > 0) {
				fps = ((frames*1000)/(time-startTime));
				startTime = time;
				frames = 0;
				sprintf(buf, "fps %d", fps);
			}

		}


		//drawCircle(scrW>>1, scrH>>1, scrW>>1, 0xff00ff);

		
		//maDrawText(4,4, buf);
		maDrawString(fHand2, buf, 4, 4);
	
		maUpdateScreen();

		while(maGetEvent(&event) != 0) {
			if(event.type == EVENT_TYPE_CLOSE) {
				maExit(0);
			}
		}
	}
}
#endif
