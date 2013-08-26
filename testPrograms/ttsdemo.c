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
#include "ttsdemo.png.h"
#include "ttsdemo_splash.png.h"

int main() {
	Handle splash = 0;
	CreateImage(splash, splash_png, sizeof(splash_png));
	DrawImage(splash, 0, 0);
	UpdateScreen();
	do {
		Wait(WAIT_KEY);
		if(GetKeys() & MAK_FIRE)
			break;
	} while(1);
	DestroyObject(splash);

	Handle backg = 0;
	CreateImage(backg, ttsdemo_png, sizeof(ttsdemo_png));
	DrawImage(backg, 0, 0);
	UpdateScreen();

	int oldKeys = GetKeys();
	while(1) {
		Wait(WAIT_KEY);
		int newKeys = GetKeys();
		int downedKeys = newKeys & (~oldKeys);
		oldKeys = newKeys;

		if(downedKeys)
			StopSpeaking();

		if(downedKeys & MAK_FIRE) {
			StartSpeaking("Du står i korsningen Götgatan Åsögatan riktning Slussen. En meter till "
				"vänster om dig är ett övergångställe, för passage över Götgatan med "
				"tryckknapp för gångtrafikanter. Vid trottoarkanten löper en cykelväg.");
		}
		if(downedKeys & MAK_DOWN) {
			StartSpeaking("Tre meter bakom dig i riktning Skanstull är ett övergångställe för "
				"passage över Åsögatan.");
		}
		if(downedKeys & MAK_UP) {
			StartSpeaking("Trottoaren rakt framfortsätter 50 meter till nästa korsning Folkungagatan. "
				"På andra sidan av övergångstället finns ingångar till tunnelbanestation "
				"medborgarplatsen.");
		}
		/*if(downedKeys & MAK_RIGHT) {
		StartSpeaking("Am I right? Of course I'm right.");
		}
		if(downedKeys & MAK_LEFT) {
		StartSpeaking("No one is left behind.");
		}*/
	}
	return 0;
}

