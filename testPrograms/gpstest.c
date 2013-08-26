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

