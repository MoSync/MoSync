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

#ifndef _LAYOUTSCREEN_H_
#define _LAYOUTSCREEN_H_

#include <MAUI/Screen.h>
#include <MAUI/Layout.h>

using namespace MAUI;

class LayoutScreen : public Screen {
public:
	LayoutScreen(Screen *previous);
	~LayoutScreen();
	void keyPressEvent(int keyCode, int nativeCode);
	void pointerPressEvent(MAPoint2d point);
	void pointerReleaseEvent(MAPoint2d point);

private:
	Screen *previous;
	Layout *mainLayout;
	Widget *softKeys;
};

#endif	//_LAYOUTSCREEN_H_
