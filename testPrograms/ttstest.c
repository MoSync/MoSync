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

