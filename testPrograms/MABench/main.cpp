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

#include <maapi.h>
#include "MAHeaders.h"
#include <conprint.h>
#include <mastdlib.h>
#include "MAUtil/Vector.h"
#include "MAUtil/String.h"

using namespace MAUtil;

char num[40];
const char *getStrFromInt(int i) {
	sprintf(num, "%d", i);
	return num;
}

class BenchmarkCase {
public:
	BenchmarkCase(const String& name) {
		this->name = name;
		defaultInfo = "No Info";
	}
	virtual ~BenchmarkCase() {}
	virtual void init() {
	}

	virtual void close() {
	}

	virtual const String& getInfo() {
		return defaultInfo;
	}

	virtual void run() = 0;
	const String& getName() {
		return name;
	}

private:
	String defaultInfo;
	String name;
};

class Benchmark {
public:
	Benchmark(const String& name) {
		this->name = name;
		srand(0);
	}

	void run() {
		printf(	"----------------------------\n"
				"Benchmark: %s\n"
				"----------------------------\n", name.c_str());

		int initTime, resTime;
		for(int i = 0; i < benchmarkCases.size(); i++) {
			benchmarkCases[i]->init();
			initTime = maGetMilliSecondCount();
			benchmarkCases[i]->run();
			resTime = maGetMilliSecondCount()-initTime;
			benchmarkCases[i]->close();			
			printf(	"----------------------------\n"
					"Case: %s\n"
					"Info: %s\n"
					"Time: %d ms\n"
					"----------------------------\n", 
					benchmarkCases[i]->getName().c_str(),
					benchmarkCases[i]->getInfo().c_str(),
					resTime);
		}
	}

	void addBenchmarkCase(BenchmarkCase *bmc) {
		benchmarkCases.add(bmc);	
	}

private:
	Vector<BenchmarkCase*> benchmarkCases;
	String name;
};


class LinesBenchmarkCase : public BenchmarkCase {
public:
	LinesBenchmarkCase(int w, int h, int n) : 
//		BenchmarkCase("Lines Drawn to Screen Benchmark"), 
		BenchmarkCase("maLine"), 
		w(w), 
		h(h), 
		numLines(n) {
			infoString = "";
			infoString += "Drawing ";
			infoString += getStrFromInt(n);
			infoString += " lines to the screen.";
	}
	
	void init() {
		randomNumbers = new int[numLines*2];
		for(int i = 0; i < numLines*2; i++) {
			if((i&0x1)==0) {
				randomNumbers[i] = rand()%w;
			}
			else {
				randomNumbers[i] = rand()%h;
			}
		}
		maSetDrawTarget(0);
	}

	const String& getInfo() {
		return infoString;
	}

	void close() {
		delete []randomNumbers;

	}

	void run() {	
		for(int i = 0; i < numLines-1; i+=2) {
			maLine(randomNumbers[i], randomNumbers[i+1], randomNumbers[i+2], randomNumbers[i+3]);
		}
	}
private:
	int w, h;
	int numLines;
	int *randomNumbers;
	String infoString;
};

class PlotsBenchmarkCase : public BenchmarkCase {
public:
	PlotsBenchmarkCase(int w, int h, int n) : 
//		BenchmarkCase("Lines Drawn to Screen Benchmark"), 
		BenchmarkCase("maPlot"), 
		w(w), 
		h(h), 
		numPlots(n) {
			infoString = "";
			infoString += "Drawing ";
			infoString += getStrFromInt(n);
			infoString += " plots to the screen.";
	
	}
	
	void init() {
		randomNumbers = new int[numPlots];
		for(int i = 0; i < numPlots; i++) {
			if((i&0x1)==0) {
				randomNumbers[i] = rand()%w;
			}
			else {
				randomNumbers[i] = rand()%h;
			}
		}
		maSetDrawTarget(0);
	}

	void close() {
		delete []randomNumbers;

	}

	const String& getInfo() {
		return infoString;
	}

	void run() {	
		for(int i = 0; i < numPlots; i++) {
			maPlot(randomNumbers[i], randomNumbers[i+1]);
		}
	}
private:
	int w, h;
	int numPlots;
	int *randomNumbers;
	String infoString;
};

class FillRectBenchmarkCase : public BenchmarkCase {
public:
	FillRectBenchmarkCase(int w, int h, int n) : 
//		BenchmarkCase("Lines Drawn to Screen Benchmark"), 
		BenchmarkCase("maFillRect"), 
		w(w), 
		h(h), 
		numRects(n) {
			infoString = "";
			infoString += "Drawing ";
			infoString +=  getStrFromInt(n);
			infoString += " filled rects to the screen.";
	
	}
	
	void init() {
		randomNumbers = new int[numRects*4];
		// initialize random number so that the rects never gets larger than the screen.
		for(int i = 0; i < numRects*4;) {
				randomNumbers[i++] = rand()%w;
				randomNumbers[i] = rand()%w;
				if((randomNumbers[i-1] + randomNumbers[i])>=w) {
					randomNumbers[i-1] -= w - (randomNumbers[i-1] + randomNumbers[i]);
				}
				i++;
				randomNumbers[i++] = rand()%h;
				randomNumbers[i] = rand()%h;
				if((randomNumbers[i-1] + randomNumbers[i])>=h) {
					randomNumbers[i-1] -= h - (randomNumbers[i-1] + randomNumbers[i]);
				}
				i++;
		}
		maSetDrawTarget(0);
	}

	void close() {
		delete []randomNumbers;

	}

	const String& getInfo() {
		return infoString;
	}

	void run() {	
		for(int i = 0; i < numRects*4; i+=4) {
			maFillRect(randomNumbers[i], randomNumbers[i+2], randomNumbers[i+1], randomNumbers[i+3]);
		}
	}
private:
	int w, h;
	int numRects;
	int *randomNumbers;
	String infoString;
};

class ImageDrawBenchmarkCase : public BenchmarkCase {
public:
	ImageDrawBenchmarkCase(int w, int h, int iw, int ih, int n) : 
//		BenchmarkCase("Lines Drawn to Screen Benchmark"), 
		BenchmarkCase("maDrawImage"), 
		w(w), 
		h(h), 
		imageWidth(iw),
		imageHeight(ih),
		numImages(n) {
			infoString = "";
			infoString += "Drawing ";
			infoString +=  getStrFromInt(n);
			infoString += " ";
			infoString += getStrFromInt(iw);
			infoString += "x";
			infoString += getStrFromInt(ih);
			infoString += " images to the screen.";
	}
	
	void init() {
		maCreateDrawableImage(IMAGE_RES, imageWidth, imageHeight);
		maSetDrawTarget(IMAGE_RES);
		for(int i = 0; i < imageHeight; i++) {
			for(int j = 0; j < imageWidth; j++) {
				maSetColor(i*j);
				maPlot(j, i);
			}
		}
		maSetDrawTarget(0);
	}

	void close() {
		maDestroyObject(IMAGE_RES);
	}

	const String& getInfo() {
		return infoString;
	}

	void run() {	
		for(int i = 0; i < numImages; i++) {
			maDrawImage(IMAGE_RES, 0, 0);
		}
	}
private:
	int w, h;
	int imageWidth, imageHeight;
	int numImages;
	String infoString;
};

class ImageDrawRegionBenchmarkCase : public BenchmarkCase {
public:
	ImageDrawRegionBenchmarkCase(int w, int h, int iw, int ih, int n) : 
//		BenchmarkCase("Lines Drawn to Screen Benchmark"), 
		BenchmarkCase("maDrawImageRegion"), 
		w(w), 
		h(h), 
		imageWidth(iw),
		imageHeight(ih),
		numImages(n) {
			infoString = "";
			infoString += "Drawing ";
			infoString +=  getStrFromInt(n);
			infoString += " regions from a ";
			infoString += getStrFromInt(iw);
			infoString += "x";
			infoString += getStrFromInt(ih);
			infoString += " image to the screen.";
	}
	
	void init() {
		maCreateDrawableImage(IMAGE_RES, imageWidth, imageHeight);
		maSetDrawTarget(IMAGE_RES);
		for(int i = 0; i < imageHeight; i++) {
			for(int j = 0; j < imageWidth; j++) {
				maSetColor(i*j);
				maPlot(j, i);
			}
		}
		srcRect.width = imageWidth>>1;
		srcRect.height = imageHeight>>1;
		srcRect.left = 0;
		srcRect.top = 0;
		maSetDrawTarget(0);
	}

	void close() {
		maDestroyObject(IMAGE_RES);
	}

	const String& getInfo() {
		return infoString;
	}

	void run() {
		dstPoint.x = 0;
		dstPoint.y = 0;
		for(int i = 0; i < numImages; i++) {
			maDrawImageRegion(IMAGE_RES, &srcRect, &dstPoint, 0);
		}
	}
private:
	int w, h;
	int imageWidth, imageHeight;
	int numImages;
	MARect srcRect;
	MAPoint2d dstPoint;
	String infoString;
};

class DrawRGBBenchmarkCase : public BenchmarkCase {
public:
	DrawRGBBenchmarkCase(int w, int h, int iw, int ih, int n) : 
//		BenchmarkCase("Lines Drawn to Screen Benchmark"), 
		BenchmarkCase("maDrawRGB"), 
		w(w), 
		h(h), 
		imageWidth(iw),
		imageHeight(ih),
		numImages(n) {
			infoString = "";
			infoString += "Drawing ";
			infoString +=  getStrFromInt(n);
			infoString += " ";
			infoString += getStrFromInt(iw);
			infoString += "x";
			infoString += getStrFromInt(ih);
			infoString += " int buffers to the screen.";
	}
	
	void init() {
		image = new int[imageWidth*imageHeight];
	}

	void close() {
		maDestroyObject(IMAGE_RES);
		delete []image;
	}

	const String& getInfo() {
		return infoString;
	}

	void run() {	
		MAPoint2d p = {0, 0};
		MARect r = {0, 0, imageWidth, imageHeight};
		for(int i = 0; i < numImages; i++) {
			maDrawRGB(&p, image, &r, imageWidth);
		}
	}
private:
	int w, h;
	int imageWidth, imageHeight;
	int numImages;
	int *image;
	String infoString;
};

extern "C" 
{
	int MAMain()
	{
		Extent e = maGetScrSize();
		Benchmark b("Graphics Benchmark");
		b.addBenchmarkCase(new LinesBenchmarkCase(EXTENT_X(e), EXTENT_Y(e), 100000));
		b.addBenchmarkCase(new PlotsBenchmarkCase(EXTENT_X(e), EXTENT_Y(e), 100000));
		b.addBenchmarkCase(new FillRectBenchmarkCase(EXTENT_X(e), EXTENT_Y(e), 100000));
		b.addBenchmarkCase(new ImageDrawBenchmarkCase(EXTENT_X(e), EXTENT_Y(e), 128, 128, 100000));
		b.addBenchmarkCase(new ImageDrawRegionBenchmarkCase(EXTENT_X(e), EXTENT_Y(e), 128, 128, 10000));
		b.addBenchmarkCase(new DrawRGBBenchmarkCase(EXTENT_X(e), EXTENT_Y(e), 128, 128, 1000));
		b.run();
		while(maGetEvent()!=EVENT_CLOSE) {

			maUpdateScreen();
		}

		return 0;
	}
}
