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

#ifndef _LABELSCREEN_H_
#define _LABELSCREEN_H_

class LabelScreen : public Screen {
public:
	LabelScreen(Screen *previous);
	~LabelScreen();
	void keyPressEvent(int keyCode, int nativeCode);

	void pointerPressEvent(MAPoint2d point);
	void pointerReleaseEvent(MAPoint2d point);
private:
	Screen *previous;
	ListBox *listBox;
	Layout *mainLayout;
	Widget *softKeys;
};

#endif	//_LABELSCREEN_H_
