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

#include "ma.h"
#include "maarg.h"
#include "mastring.h"
#include "mavsprintf.h"
#include "maheap.h"
#include "conprint.h"
#include "IX_FILE.h"

// Console width, in characters
// Note that not all characters on a line may be visible on a given device, due
// to varying screen widths
#define CONSOLE_WIDTH 47

typedef struct
{
	char line[CONSOLE_WIDTH + 1];
} ConLine;

typedef struct ConData_t
{
	// True if console layer is initialized
	int initialized;
	// The height of the console, in lines
	int height;
	// The index in the ring buffer of the top line on screen
	int firstLine;
	// The height of the font used, in pixels
	int fontHeight;
	// HACK: We postpone line feeds at very end of output to avoid having a
	// (usually) empty line at the bottom of the (admittedly tiny) display
	// This flag indicates whether or not such a line feed was postponed during
	// the last output call
	// If this hack makes you cry, feel free to remove it
	int postponedLineFeed;
	// The size of the screen, in pixels
	MAExtent screenSize;
	// The line (y) and column (x) position of the cursor
	MAPoint2d cursorPos;
	// The text line ring buffer
	ConLine* lines;
} ConData;

// Private data for the console
static ConData sConsole = { 0, 0, 0, 0, 0, EXTENT(0, 0), { 0, 0 }, NULL };

// These are global variables documented in conprint.h
int gConsoleTextColor = 0x00FF00;	//green
int gConsoleBackgroundColor = 0;	//black
int gConsoleLogging = 1;
MAHandle gConsoleFile = 0;
int gConsoleDisplay = 1;

static void FeedLine(void)
{
	sConsole.cursorPos.y++;
	sConsole.cursorPos.x = 0;

	if (sConsole.cursorPos.y >= sConsole.height)
	{
		memset(sConsole.lines[sConsole.firstLine].line, 0, sizeof(ConLine));
		sConsole.firstLine = (sConsole.firstLine + 1) % sConsole.height;
		sConsole.cursorPos.y = sConsole.height - 1;
	}
}

void InitConsole(void)
{
	int i;

	sConsole.screenSize = maGetScrSize();
	maSetClipRect(0, 0, EXTENT_X(sConsole.screenSize), EXTENT_Y(sConsole.screenSize));
	sConsole.fontHeight = EXTENT_Y(maGetTextSize("gl"));
	sConsole.height = EXTENT_Y(sConsole.screenSize) / sConsole.fontHeight;
	sConsole.cursorPos.x = 0;
	sConsole.cursorPos.y = sConsole.height - 1;
	sConsole.firstLine = 0;
	sConsole.postponedLineFeed = 0;

	sConsole.lines = (ConLine*)malloc(sizeof(ConLine) * sConsole.height);
	for (i = 0;  i < sConsole.height;  i++)
		memset(sConsole.lines[i].line, 0, sizeof(ConLine));

	sConsole.initialized = 1;
}

void DisplayConsole(void)
{
	int n, index;

	if (gConsoleDisplay == 0)
		return;

	maSetClipRect(0, 0, EXTENT_X(sConsole.screenSize), EXTENT_Y(sConsole.screenSize));

	maSetColor(gConsoleBackgroundColor);
	maFillRect(0, 0, EXTENT_X(sConsole.screenSize), EXTENT_Y(sConsole.screenSize));

	maSetColor(gConsoleTextColor);
	for (n = 0;  n < sConsole.height;  n++)
	{
		index = (n + sConsole.firstLine) % sConsole.height;
		maDrawText(0, n * sConsole.fontHeight, sConsole.lines[index].line);
	}

	maUpdateScreen();
}

#define MIN(a,b) ((a) > (b) ? (b) : (a))
void PrintConsole(const char *str)
{
	int length, pos = 0;
	char* line;

	if (gConsoleLogging)
	{
		static char prefix[] = "PrintConsole: ";
		maWriteLog(prefix, strlen(prefix));

		length = strlen(str);
		if (length > 0)
		{
			maWriteLog(str, length);
			if (str[length - 1] != '\n')
				maWriteLog("\n", 1);
		}
	}
	if(gConsoleFile > 0) {
		int res = maFileWrite(gConsoleFile, str, strlen(str));
		if(res < 0) {
			maPanic(res, "PrintConsole maFileWrite");
		}
	}

	if (!sConsole.initialized)
		InitConsole();

	if (sConsole.postponedLineFeed)
	{
		FeedLine();
		sConsole.postponedLineFeed = 0;
	}

	while (str[pos] != '\0')
	{
		if (str[pos] == '\r')
			sConsole.cursorPos.x = 0;
		else if (str[pos] == '\n')
		{
			if (str[pos + 1] == '\0')
				sConsole.postponedLineFeed = 1;
			else
				FeedLine();
		}
		else
		{
			line = sConsole.lines[(sConsole.cursorPos.y + sConsole.firstLine) %
				sConsole.height].line;
			line[sConsole.cursorPos.x++] = str[pos];

			if (sConsole.cursorPos.x >= CONSOLE_WIDTH)
			{
				if (str[pos + 1] == '\0')
					sConsole.postponedLineFeed = 1;
				else
					FeedLine();
			}
		}

		pos++;
	}

	DisplayConsole();
}

#define PRINTF_BUFSIZE 2048
int vprintf(const char *fmt, va_list args)
{
	char buf[PRINTF_BUFSIZE];
	int len;

	buf[0] = 0;

	len = vsprintf(buf, fmt, args);

	if(len >= PRINTF_BUFSIZE) {
		maPanic(1, "printf buffer overrun!");
	}

	PrintConsole(buf);

	len = strlen(buf);
	return len;
}

int printf(const char *fmt, ...)
{
	va_list args;
	int len;

	va_start(args, fmt);
	len = vprintf(fmt, args);
	va_end(args);

	return len;
}

int puts(const char* str)
{
	PrintConsole(str);
	FeedLine();
	return 0;
}

int putchar(int character)
{
	char temp[2];
	temp[0] = character;
	temp[1] = 0;
	PrintConsole(temp);
	return character;
}
