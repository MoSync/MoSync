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

#include <MAUI/Label.h>
#include <MAUI/Layout.h>
#include <MAUI/ListBox.h>
#include "Util.h"

Font *gFont;
WidgetSkin *gSkin;
int gScreenWidth;
int gScreenHeight;

void setLabelPadding(Widget *w) {
	w->setPaddingLeft(PADDING);
	w->setPaddingBottom(PADDING);
	w->setPaddingRight(PADDING);
	w->setPaddingTop(PADDING);
}

Label* createLabel(const char *str) {
	Label *label;
	label = new Label(0,0, gScreenWidth-PADDING*2, 0, NULL, str, 0, gFont);
	label->setSkin(gSkin);
	label->setAutoSizeY(true);
	setLabelPadding(label);
	return label;
}

Widget* createSoftKeyBar(int height, const char *left, const char *right) {
	Layout *layout = new Layout(0, 0, gScreenWidth, height, NULL, 2, 1);
	Label *label;

	label = new Label(0,0, gScreenWidth/2, height, NULL, left, 0, gFont);
	label->setHorizontalAlignment(Label::HA_LEFT);
	label->setVerticalAlignment( Label::VA_CENTER );
	setLabelPadding(label);
	layout->add(label);

	label = new Label(0,0, gScreenWidth/2, height, NULL, right, 0, gFont);
	label->setHorizontalAlignment(Label::HA_RIGHT);
	label->setVerticalAlignment( Label::VA_CENTER );
	setLabelPadding(label);
	layout->add(label);

	return layout;
}
