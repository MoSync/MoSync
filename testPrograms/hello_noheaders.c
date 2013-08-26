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

#define MAK_0 48
#define EVENT_TYPE_CLOSE 1
#define EVENT_TYPE_KEY_PRESSED 2

typedef struct EVENT {
	int type;
	int key;
} EVENT;

int maSetColor(int rgb);
void maDrawText(int left, int top, const char* str);
void maUpdateScreen();
int maGetEvent(EVENT* event);
void maWait(int timeout);

int MAMain() {
	maSetColor(0xFFFFFF);
	maDrawText(0,0, "Hello World!");
	maUpdateScreen();

	//Freeze
	while(1) {
		maWait(0);
		EVENT event;
		if(maGetEvent(&event)) {
			if(event.type == EVENT_TYPE_CLOSE)
				break;
			if(event.type == EVENT_TYPE_KEY_PRESSED) {
				if(event.key == MAK_0)
					break;
			}
		}
	}
	return 0;

}
