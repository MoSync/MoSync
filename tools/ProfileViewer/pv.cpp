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
#include "helpers/attribute.h"
#include "helpers/types.h"

using namespace std;

// defines
#define ASSERT(a) do { if(!(a)) fatalError(); } while(0)

// types

// variables
static SDL_Surface* sScreen = NULL;
static ProfNode* sRoot = NULL;
static ProfNode* sCurrentNode;
static int sMaxLevel = 0;

// only valid during parsing
static int sParseLevel, sNodeCount;
static ProfNode* sCurrentParent;

// functions
static void XMLCALL opStart(void *data, const char *el, const char **attr);
static void XMLCALL opEnd(void *data, const char *el);
static float parseFloat(const char* str);
static int parseInt(const char* str);
static void ATTRIB(noreturn) fatalError();

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

struct SLICE {
	uint color;
	float fraction;
};
static const float TRIGGER_FRACTION = 0.01f;	// 1%

static void dumpNode(ProfNode* node) {
	printf("%s\n", node->name.c_str());
}

static void drawPie() {
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

	vector<SLICE> slices;
	uint color = BLUE;
	{
		SLICE s = { color, sCurrentNode->localTime / sCurrentNode->totalTime };
		slices.push_back(s);
	}
	uint colorIndex = 0;
	const ProfSet& ch(sCurrentNode->children);
	for(ProfSet::iterator itr = ch.begin(); itr != ch.end(); itr++) {
		const ProfNode* node(*itr);
		float fraction = node->totalTime / sCurrentNode->totalTime;
		if(fraction > TRIGGER_FRACTION) {
			SLICE s = { color, fraction };
			slices.push_back(s);
			colorIndex++;
			if(colorIndex >= nColors)
				colorIndex = 0;
		}
	}
	printf("%u slices constructed\n", slices.size());

	int centerX = sScreen->w / 2, centerY = sScreen->h / 2;
	int radius = MIN(centerX, centerY);

	// draw slices
	Sint16 angle = 0;
	for(size_t i=0; i<slices.size(); i++) {
		const SLICE& s(slices[i]);
		Sint16 nextAngle = (Sint16)(angle + s.fraction * 360);
		filledPieColor(sScreen, centerX, centerY, radius, angle, nextAngle, s.color);
		angle = nextAngle;
	}
	// draw the rest
	filledPieColor(sScreen, centerX, centerY, radius, angle, 360, RED);

	SDL_Flip(sScreen);
}

#undef main
extern "C" int main(int argc, const char** argv) {
	const char* filename = argv[1];

	// todo: draw "loading"

	loadProfile(filename);
	
	// init SDL
	SDL_Init(SDL_INIT_VIDEO);
	sScreen = SDL_SetVideoMode(640, 480, 32, SDL_SWSURFACE);

	sCurrentNode = sRoot;
	drawPie();

	while(true) {
		SDL_Event e;
		SDL_WaitEvent(&e);
		if(e.type == SDL_QUIT)
			break;
	}
	return 0;
}
