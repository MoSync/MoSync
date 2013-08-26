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

#ifndef _UTIL_H_
#define _UTIL_H_

#include <MAUI/Widget.h>
#include <MAUI/Label.h>
#include <MAUI/Layout.h>
#include <MAUI/Font.h>

using namespace MAUI;

#define PADDING 5

void setLabelPadding(Widget *w);
Label* createLabel(const char *str, int height=32);
Widget* createSoftKeyBar(int height, const char *left, const char *right);
Layout* createMainLayout(const char *left, const char *right);

extern Font *gFont;
extern WidgetSkin *gSkin;
extern int scrWidth;
extern int scrHeight;

#endif	//_UTIL_H_
