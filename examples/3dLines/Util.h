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
extern int gScreenWidth;
extern int gScreenHeight;

#endif	//_UTIL_H_
