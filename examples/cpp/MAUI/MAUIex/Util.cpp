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

#include <MAUI/Label.h>
#include <MAUI/Layout.h>
#include <MAUI/ListBox.h>
#include "Util.h"

Font *gFont;
WidgetSkin *gSkin;
int scrWidth;
int scrHeight;

void setLabelPadding(Widget *w) {
	w->setPaddingLeft(PADDING);
	w->setPaddingBottom(PADDING);
	w->setPaddingRight(PADDING);
	w->setPaddingTop(PADDING);
}

Label* createLabel(const char *str, int height) {
	Label *label;
	label = new Label(0,0, scrWidth-PADDING*2, height, NULL, str, 0, gFont);
	label->setSkin(gSkin);
	setLabelPadding(label);
	return label;
}

Widget* createSoftKeyBar(int height, const char *left, const char *right) {
	Layout *layout = new Layout(0, 0, scrWidth, height, NULL, 2, 1);
	Label *label;

	label = new Label(0,0, scrWidth/2, height, NULL, left, 0, gFont);
	label->setHorizontalAlignment(Label::HA_LEFT);
	label->setDrawBackground(false);
	setLabelPadding(label);
	layout->add(label);

	label = new Label(0,0, scrWidth/2, height, NULL, right, 0, gFont);
	label->setDrawBackground(false);
	label->setHorizontalAlignment(Label::HA_RIGHT);
	setLabelPadding(label);
	layout->add(label);

	return layout;
}

// first child is listbox
Layout* createMainLayout(const char *left, const char *right) {
	Layout *mainLayout = new Layout(0, 0, scrWidth, scrHeight, NULL, 1, 2);

	Widget *softKeys = createSoftKeyBar(30, left, right);
	ListBox* listBox = new ListBox(0, 0, scrWidth, scrHeight-softKeys->getHeight(), mainLayout, ListBox::LBO_VERTICAL, ListBox::LBA_LINEAR, true);
	listBox->setSkin(gSkin);
	listBox->setPaddingLeft(5);
	listBox->setPaddingRight(5);
	listBox->setPaddingTop(15);
	listBox->setPaddingBottom(15);

	mainLayout->add(softKeys);

	mainLayout->setSkin(NULL);
	mainLayout->setDrawBackground(true);
	mainLayout->setBackgroundColor(0);

	return mainLayout;
}
