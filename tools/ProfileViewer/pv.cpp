// Use Expat and SDL to display a pie chart of a MoRE-generated function-time
// program profile.

#include <stdio.h>
#include <io.h>
#include <fcntl.h>
#include <expat.h>
#include <SDL.h>
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
static const float TRIGGER_FRACTION = 0.01f;	// 1%

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

static bool streq(const char* a, const char* b) {
	return strcmp(a, b) == 0;
}

template<class T> const T& MIN(const T& a, const T& b) {
	return a < b ? a : b;
}

static void fatalError() {
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
		totalTime, childrenTime, localTime };
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
void parse_xml(XML_Parser p, int fd) {
  for (;;) {
    int bytes_read;
    enum XML_Status status;

    void *buff = XML_GetBuffer(p, BUFF_SIZE);
    ASSERT(buff);
    bytes_read = _read(fd, buff, BUFF_SIZE);
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

void loadProfile(const char* filename) {
	sParseLevel = 0;
	sCurrentParent = NULL;
	sNodeCount = 0;

	printf("Loading '%s'\n", filename);
	int fd = _open(filename, _O_RDONLY);
	
	XML_Parser p = XML_ParserCreate("UTF-8");
	XML_SetElementHandler(p, opStart, opEnd);
	parse_xml(p, fd);
	printf("Loaded %i nodes. Max level: %i\n", sNodeCount, sMaxLevel);
}

static void dumpNode(ProfNode* node) {
	printf("%s\n", node->name.c_str());
}

static void constructPie() {
	sSlices.clear();
	uint color = BLUE;	// for local time
	{
		SLICE s = { sCurrentNode, color,
			sCurrentNode->localTime / sCurrentNode->totalTime };
		sSlices.push_back(s);
	}
	uint colorIndex = 0;
	ProfSet& ch(sCurrentNode->children);
	for(ProfSet::iterator itr = ch.begin(); itr != ch.end(); itr++) {
		ProfNode* node(*itr);
		float fraction = node->totalTime / sCurrentNode->totalTime;
		if(fraction > TRIGGER_FRACTION) {
			SLICE s = { node, color, fraction };
			sSlices.push_back(s);
			colorIndex++;
			if(colorIndex >= nColors)
				colorIndex = 0;
		}
	}
	printf("%u slices constructed\n", sSlices.size());
}

void drawPie() {
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

	// todo: draw text

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
	sHitBuffer = SDL_CreateRGBSurface(SDL_SWSURFACE, 640, 480, 8, 0xff,0,0,0);

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
		}
	}
	return 0;
}

// show name and stats on pie-slice mouse-over
// go down one level on click
static void processMouseMotion(const SDL_MouseMotionEvent& e) {
	Uint32 index;
	ST(SDL_GetPixel(sHitBuffer, e.x, e.y, &index));
	printf("%ix%i: %i\n", e.x, e.y, sSliceIndex);
	ASSERT(index <= sSlices.size());
	if(index != sSliceIndex) {
		sSliceIndex = index;
		drawPie();
	}
}

static void processMouseClick(const SDL_MouseButtonEvent&) {
}
