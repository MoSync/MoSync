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

int main() {
	int line = 0;

	DrawTextLine(line++, "Press any key");
	Wait(WAIT_KEY);

	DrawTextLine(line++, "Hello World!");
	StartSpeaking("Gå i riktning mot Stockholm, sväng vänster vid Sickla Industriväg. Följ trappan som ligger till höger om Atlas Copco huset. Ta vänster vid Skrädderifirman in till Marcusplatsen. Dieselverkstaden ligger till vänster, gå in genom huvudentrén, ta trappan upp så finns vi innanför dörren till höger."
		"Saltsjöbanan från Slussen till Sickla station."
		"Följ trappan som ligger till höger om Atlas Copco huset. Ta vänster vid Skrädderifirman in till Marcusplatsen. Dieselverkstaden ligger till vänster, gå in genom huvudentrén, ta trappan upp och dörren till höger."
		"Det finns även skyltar mot Dieselverkstaden som man kan följa.");
	DrawTextLine(line++, "started");
	if(!IsSpeaking()) {
		DrawTextLine(line++, "Not speaking!");
		goto end;
	}
	Wait(WAIT_TTS);
	DrawTextLine(line++, "done");
	if(IsSpeaking()) {
		DrawTextLine(line++, "Still speaking!");
		goto end;
	}

end:
	//wait for keypress, then exit
	DrawTextLine(line++, "PAK to quit");
	Wait(WAIT_KEY);
	DrawTextLine(line++, "Quitting...");
	Exit(0);
	DrawTextLine(line++, "Exit returned!");
	return 0;
}

Extent DrawTextLine(int line, char* text) {
	Extent res = DrawText(10, line*10+20, text);
	UpdateScreen();
	return res;
}

