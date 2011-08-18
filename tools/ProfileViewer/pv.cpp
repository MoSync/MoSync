/*
Copyright (C) 2011 MoSync AB

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License,
version 2, as published by the Free Software Foundation.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
MA 02110-1301, USA.
*/

// Use Expat and SDL to display a pie chart of a MoRE-generated function-time
// program profile.

#include <stdio.h>
#ifdef _MSC_VER
#include <io.h>
#endif
#include <fcntl.h>
#include <expat.h>
#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>
#include <vector>
#include "ProfNode.h"
#include "FlagCheck.h"
#include "SDL_gfxPrimitives.h"
#include "SDL_Pixel.h"
#include "helpers/attribute.h"
#include "helpers/types.h"

using namespace std;

// defines
#define ASSERT(a) do { if(!(a)) fatalError(); } while(0)
#define ST(sdlFunc) do { int res = (sdlFunc); ASSERT(res == 0); } while(0)

// constants
static const float TRIGGER_FRACTION = 0.005f;	// 0.5%

static const uint WHITE = 0xffffffff;
static const uint RED = 0xff0000ff;
static const uint BLUE = 0x8080ffff;
static const uint YELLOW = 0xffff00ff;
static const uint PINK = 0xff00ffff;
static const uint GREEN = 0x00ff00ff;

static const uint COLORS[] = {
	YELLOW, PINK, GREEN,
};
static const uint nColors = sizeof(COLORS) / sizeof(uint);

// types
struct SLICE {
	ProfNode* node;
	uint color;
	float fraction;
	Sint16 startAngle, endAngle;
};

// variables
static SDL_Surface* sScreen = NULL;
static SDL_Surface* sHitBuffer = NULL;
static TTF_Font *sFont = NULL; 
static int sFontHeight;
static ProfNode* sRoot = NULL;
static ProfNode* sCurrentNode;
static int sMaxLevel = 0;
// index into sSlices, +1.
static Uint32 sSliceIndex = 0;

// graphics cache
static vector<SLICE> sSlices;
int sRadius, sCenterX, sCenterY;

// only valid during parsing
static int sParseLevel, sNodeCount;
static ProfNode* sCurrentParent;

// functions
static void XMLCALL opStart(void *data, const char *el, const char **attr);
static void XMLCALL opEnd(void *data, const char *el);
static float parseFloat(const char* str);
static int parseInt(const char* str);
static void ATTRIB(noreturn) fatalError();
static void processMouseMotion(const SDL_MouseMotionEvent&);
static void processMouseClick(const SDL_MouseButtonEvent&);
static void processKeyDown(const SDL_KeyboardEvent& e);
static void drawTextf(int& y, bool left, const char* fmt, ...) PRINTF_ATTRIB(3, 4);

static bool streq(const char* a, const char* b) {
	return strcmp(a, b) == 0;
}

template<class T> const T& MIN(const T& a, const T& b) {
	return a < b ? a : b;
}

static void fatalError() {
	printf("SDL Error: %s\n", SDL_GetError());
	exit(1);
};

static float parseFloat(const char* str) {
	float f;
	int n;
	int res = sscanf(str, "%f%n", &f, &n);
	if(res == 1 && n == (int)strlen(str))
		return f;
	else
		fatalError();
}

static int parseInt(const char* str) {
	int i;
	int n;
	int res = sscanf(str, "%i%n", &i, &n);
	if(res == 1 && n == (int)strlen(str))
		return i;
	else
		fatalError();
}

static void XMLCALL opStart(void *data, const char *el, const char **attr) {
	if(!streq(el, "f")) {
		return;
	}
	string name;
	int count, nChildren;
	float totalTime, childrenTime, localTime;
	FlagCheck<ProfNode> f;

	// parse attributes
	for(int i=0; attr[i]; i+=2) {
		const char* k = attr[i];
		const char* v = attr[i+1];
		if(streq(k, "n")) {
			name = v;
			f.set(ProfNode::N);
		} else if(streq(k, "c")) {
			count = parseInt(v);
			f.set(ProfNode::C);
		} else if(streq(k, "ch")) {
			nChildren = parseInt(v);
			f.set(ProfNode::CH);
		} else if(streq(k, "t")) {
			totalTime = parseFloat(v);
			f.set(ProfNode::T);
		} else if(streq(k, "lt")) {
			localTime = parseFloat(v);
			f.set(ProfNode::LT);
		} else if(streq(k, "cht")) {
			childrenTime = parseFloat(v);
			f.set(ProfNode::CHT);
		}
	}
	ASSERT(f.check());

	ProfNode p = { sCurrentParent, name, count, nChildren,
		totalTime, childrenTime, localTime, ProfSet() };
	ProfNode* n = new ProfNode(p);
	if(!sCurrentParent) {
		ASSERT(!sRoot);
		sRoot = n;
	} else {
		sCurrentParent->children.insert(n);
	}
	ASSERT(sRoot);
	sCurrentParent = n;
	sParseLevel++;
	//printf("level %i\n", sParseLevel);
	sNodeCount++;
	if(sParseLevel > sMaxLevel)
		sMaxLevel = sParseLevel;
}
static void XMLCALL opEnd(void *data, const char *el) {
	if(!streq(el, "f")) {
		return;
	}
	sCurrentParent = sCurrentParent->parent;
	sParseLevel--;
	ASSERT(sParseLevel >= 0);
}

#define BUFF_SIZE 64*1024

/* Parse a document from the open file descriptor 'fd' until the parse
   is complete (the document has been completely parsed, or there's
   been an error), or the parse is stopped.
*/
static void parse_xml(XML_Parser p, int fd) {
  for (;;) {
    int bytes_read;
    enum XML_Status status;

    void *buff = XML_GetBuffer(p, BUFF_SIZE);
    ASSERT(buff);
    bytes_read = read(fd, buff, BUFF_SIZE);
    ASSERT(bytes_read >= 0);
    status = XML_ParseBuffer(p, bytes_read, bytes_read == 0);
		if(status != XML_STATUS_OK) {
			XML_Error e = XML_GetErrorCode(p);
			printf("XML_Error: %i\n", e);
			fatalError();
		}
    if (bytes_read == 0)
      return;
  }
}

static void loadProfile(const char* filename) {
	sParseLevel = 0;
	sCurrentParent = NULL;
	sNodeCount = 0;

	printf("Loading '%s'\n", filename);
	int fd = open(filename, O_RDONLY);
	
	XML_Parser p = XML_ParserCreate("UTF-8");
	XML_SetElementHandler(p, opStart, opEnd);
	parse_xml(p, fd);
	printf("Loaded %i nodes. Max level: %i\n", sNodeCount, sMaxLevel);
}

static void constructPie() {
	sSlices.clear();
	uint color = BLUE;	// for local time
	{
		SLICE s = { sCurrentNode, color,
			sCurrentNode->localTime / sCurrentNode->totalTime, 0,0 };
		sSlices.push_back(s);
	}
	uint colorIndex = 0;
	ProfSet& ch(sCurrentNode->children);
	for(ProfSet::iterator itr = ch.begin(); itr != ch.end(); itr++) {
		ProfNode* node(*itr);
		float fraction = node->totalTime / sCurrentNode->totalTime;
		printf("%s: %i\n", node->name.c_str(), (int)node->totalTime);
		if(fraction > TRIGGER_FRACTION) {
			SLICE s = { node, COLORS[colorIndex], fraction, 0,0 };
			sSlices.push_back(s);
			colorIndex++;
			if(colorIndex >= nColors)
				colorIndex = 0;
		}
	}
	printf("%"PFZT" slices constructed\n", sSlices.size());
}

static void drawTextf(int& y, bool left, const char* fmt, ...) {
	char buf[512];
	va_list args;
	va_start(args, fmt);
	vsprintf(buf, fmt, args);
	SDL_Color color = { 0xff, 0xff, 0xff, 0 };	// white
	SDL_Surface* text_surface = TTF_RenderText_Solid(sFont, buf, color);
	ASSERT(text_surface);
	int x;
	if(left) {
		x = 0;
	} else {
		x = sScreen->w - text_surface->w;
		y -= sFontHeight;
	}
	SDL_Rect rect = { (Sint16)x, (Sint16)y, (Uint16)text_surface->w, (Uint16)text_surface->h };
	ST(SDL_FillRect(sScreen, &rect, 0));
	SDL_BlitSurface(text_surface, NULL, sScreen, &rect);
	SDL_FreeSurface(text_surface);
	if(left) {
		y += sFontHeight;
	}
}

static void drawFuncText(const char* title, int& y, ProfNode* node) {
	drawTextf(y, true, "%s: %s", title, node->name.c_str());
	drawTextf(y, true, "Total: %i ms. Local: %i ms. Children: %i ms. Count: %i.",
		(int)node->totalTime, (int)node->localTime, (int)node->childrenTime,
		node->count);
}

static void drawPie() {
	sCenterX = sScreen->w / 2;
	sCenterY = sScreen->h / 2;
	sRadius = MIN(sCenterX, sCenterY);

	ST(SDL_FillRect(sScreen, NULL, 0));
	ST(SDL_FillRect(sHitBuffer, NULL, 0));

	// draw slices
	Sint16 angle = 0;
	for(size_t i=0; i<sSlices.size(); i++) {
		SLICE& s(sSlices[i]);
		s.startAngle = angle;
		s.endAngle = (Sint16)(angle + s.fraction * 360);
		ST(filledPieColor(sScreen, sCenterX, sCenterY, sRadius,
			s.startAngle, s.endAngle, s.color));
		ASSERT(i < 256);
		ST(filledPieRGBA(sHitBuffer, sCenterX, sCenterY, sRadius,
			s.startAngle, s.endAngle, (byte)i+1,0,0,0xff));
		angle = s.endAngle;
	}
	// draw the rest (sub-1% children)
	ST(filledPieColor(sScreen, sCenterX, sCenterY, sRadius, angle, 360, RED));

	int y;

	// draw call stack
	y = sScreen->h;
	ProfNode* node = sCurrentNode->parent;
	while(node) {
		drawTextf(y, false, "%s", node->name.c_str());
		node = node->parent;
	}

	// draw current info
	y = 0;
	drawFuncText("Current function", y, sCurrentNode);
	if(sSliceIndex != 0) {
		node = sSlices[sSliceIndex-1].node;
		if(node != sCurrentNode) {
			drawFuncText("Under cursor", y, node);
		}
	}

	ST(SDL_Flip(sScreen));
}

#undef main
extern "C" int main(int argc, const char** argv) {
	const char* filename = argv[1];

	// todo: loading screen

	loadProfile(filename);
	
	// init SDL
	SDL_Init(SDL_INIT_VIDEO);
	sScreen = SDL_SetVideoMode(640, 480, 32, SDL_SWSURFACE);
	ASSERT(sScreen);
	sHitBuffer = SDL_CreateRGBSurface(SDL_SWSURFACE, 640, 480, 8, 0xff,0,0,0);
	ASSERT(sHitBuffer);

	// load font
	ST(TTF_Init());
	char fontFileName[256];
	sprintf(fontFileName, "%s/bin/unifont-5.1.20080907.ttf", getenv("MOSYNCDIR"));
	sFont = TTF_OpenFont(fontFileName, 16);
	ASSERT(sFont);
	sFontHeight = TTF_FontLineSkip(sFont);

	sCurrentNode = sRoot;
	constructPie();
	drawPie();

	while(true) {
		SDL_Event e;
		SDL_WaitEvent(&e);
		if(e.type == SDL_QUIT)
			break;
		switch(e.type) {
		case SDL_MOUSEMOTION:
			processMouseMotion((SDL_MouseMotionEvent&)e);
			break;
		case SDL_MOUSEBUTTONDOWN:
			processMouseClick((SDL_MouseButtonEvent&)e);
			break;
		case SDL_KEYDOWN:
			processKeyDown((SDL_KeyboardEvent&)e);
			break;
		}
	}
	return 0;
}

// show name and stats on pie-slice mouse-over
// go down one level on click
static void processMouseMotion(const SDL_MouseMotionEvent& e) {
	Uint32 index;
	ST(SDL_GetPixel(sHitBuffer, e.x, e.y, &index));
	//printf("%ix%i: %i\n", e.x, e.y, sSliceIndex);
	ASSERT(index <= sSlices.size());
	if(index != sSliceIndex) {
		sSliceIndex = index;
		drawPie();
	}
}

static void setNode(ProfNode* node) {
	sCurrentNode = node;
	constructPie();
	sSliceIndex = 0;
	drawPie();
}

static void processMouseClick(const SDL_MouseButtonEvent& e) {
	if(e.button != 1)
		return;
	Uint32 index;
	ST(SDL_GetPixel(sHitBuffer, e.x, e.y, &index));
	if(index == 0)
		return;
	ProfNode* node = sSlices[index-1].node;
	if(node->children.empty() || node == sCurrentNode)
		return;
	setNode(node);
}

static void processKeyDown(const SDL_KeyboardEvent& e) {
	switch(e.keysym.sym) {
	case SDLK_BACKSPACE:
		if(sCurrentNode->parent)
			setNode(sCurrentNode->parent);
		break;
	//case SDLK_ESCAPE:
		//exit(0);
	default:
		break;
	}
}
