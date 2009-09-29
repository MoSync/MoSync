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

#include "maapi.h"

#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define RGB(r, g, b) (((r) << 16) | ((g) << 8) | (b))

Extent DrawTextLine(int line, char* text);
void i2a(int v, char *str);

int main() {
	int line = 0;
	int res;
	char buffer[1024], buf[32];

	DrawTextLine(line++, "Press any key");
	Wait(WAIT_KEY);

	DrawTextLine(line++, "Opening");
	GPSOpen();
	Wait(WAIT_GPSOPEN);
	i2a(GPSOpenState(), buf);
	DrawTextLine(line++, buf);
	if(GPSOpenState() < 0) {
		DrawTextLine(line++, "Open error");
		goto end;
	}
	DrawTextLine(line++, "Opened, reading");
	GPSRead(buffer, sizeof(buffer));
	Wait(WAIT_GPSREAD);
	res = GPSReadState();
	if(res < 0) {
		DrawTextLine(line++, "Read error");
		i2a(res, buf);
		DrawTextLine(line++, buf);
		goto end;
	}
	i2a(res, buf);
	DrawTextLine(line++, buf);
	DrawTextLine(line++, buffer);

	DrawTextLine(line++, "Closing");
	GPSClose();
	DrawTextLine(line++, "Done");

end:
	//wait for keypress, then exit
	DrawTextLine(line++, "PAK to quit");
	Wait(WAIT_KEY);
	Exit(0);
	return 0;
}

Extent DrawTextLine(int line, char* text) {
	Extent res = DrawText(10, line*10+20, text);
	UpdateScreen();
	return res;
}

void i2a(int a, char *str)
{
	int v;
	if(a < 0) {
		*str++ = '-';
		a = -a;
	}

	if (a >= 100000)
	{
		v = a / 100000;
		*str++ = v + '0';
		a -= v * 100000;
	}

	if (a >= 10000)
	{
		v = a / 10000;
		*str++ = v + '0';
		a -= v * 10000;
	}


	if (a >= 1000)
	{
		v = a / 1000;
		*str++ = v + '0';
		a -= v * 1000;
	}

	if (a >= 100)
	{
		v = a / 100;
		*str++ = v + '0';
		a -= v * 100;
	}

	if (a >= 10)
	{
		v = a / 10;
		*str++ = v + '0';
		a -= v * 10;
	}

	*str++ = a + '0';
}

