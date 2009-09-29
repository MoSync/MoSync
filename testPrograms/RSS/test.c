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

#include <maapi.h>
#include "MAHeaders.h"

#include <MAUI/MAUIFrame.h>
#include <MAUI/MAUIButton.h>
#include <MAUI/MAUIImage.h>
#include <MAUI/MAUILabel.h>
#include <MAUI/MAUIListBox.h>
#include <MAUI/MAUIScrollPane.h>

//#include <MAUtil/MaStringMap.h>
#include <MAUI/MAUIEngine.h>
#include <MAUI/Font.h>

#include <MAUI/DataProvider/MAUIQueryResult.h>
#include <MAUI/DataProvider/MAUIDataProvider.h>
#include "RSSQueryResult.h"
#include "RSSDataProvider.h"

#define EXTENT(x, y) ((Extent)((((int)(x)) << 16) | ((y) & 0xFFFF)))

MAUIEngine *engine = 0;
MAUIWidget *frame = 0, *frameArticle = 0, *scrollPane = 0;
MAUIWidget *listbox = 0;
MAUIWidget *label = 0;

char* strings[] = {
"item 1",
"item 2",
"item 3",
"item 4",
"item 5",
"item 6"
};


void MAUIScrollPane_KeyFunc(MAUIWidget *w, MAUIKeyEvent *e)
{
	int keys = e->keys;
	int offset = 	MAUIScrollPane_getOffsetY(w);

	if(keys&MAK_DOWN)
	{
		offset-=4;
		MAUIScrollPane_setOffsetY(w, offset);
	}
	if(keys&MAK_UP)
	{
		offset+=4;
		if(offset>0) offset = 0;
		MAUIScrollPane_setOffsetY(w, offset);
	}
	if(keys&MAK_FIRE)
	{
		MAUIEngine_setMain(engine, frame);
		MAUIEngine_setFocus(engine, frame);
	}
}

void itemChosenListener(MAUIWidget *listBox)
{
	MAUILabel_setRowIndex(label, MAUIListBox_getSelectedIndex(listBox));
	MAUILabel_updateFromDataProvider(label);
	MAUIEngine_setMain(engine, frameArticle);
	MAUIEngine_setFocus(engine, frameArticle);
}

int MAMain()
{
	MAUIDataProvider* dataProvider = RSSDataProvider_create("");

	frame = MAUIImage_create(0,0,176,208);//, back);
	MAUIImage_setResource(frame,R_LOGO);

	frameArticle = MAUIImage_create(0,0,176,208);
	MAUIImage_setResource(frameArticle,R_BACK);

	scrollPane = MAUIScrollPane_create(1,1,176,208);
	//frame = MAUIWidget_create(5, 60, 176-10, 90);
	
	MAUIWidget_addKeyFunc(scrollPane, MAUIScrollPane_KeyFunc);

	label = MAUILabel_create(0, 0, 176, 808);
	MAUILabel_setDataProvider(label, dataProvider);
	MAUILabel_setQueryString(label, "items");
	MAUILabel_setColumnName(label, "description");
	MAUILabel_setTransparent(label, 1);
	MAUILabel_setQueryInterval(label, 10);
	MAUIWidget_add(scrollPane, label);
	MAUIWidget_add(frameArticle, scrollPane);

	listbox = MAUIListBox_create(0, 26, 176, 150);
	MAUIListBox_addItemChosenListener(listbox, itemChosenListener);
	MAUIListBox_setDataProvider(listbox, dataProvider);
	MAUIListBox_setColumnName(listbox, "title");
	MAUIListBox_setQueryString(listbox,"items");
	MAUIListBox_setQueryInterval(listbox, 10);
	MAUIWidget_add(frame, listbox);

	engine = MAUIEngine_create(frame);
	MAUIEngine_setFocus(engine, frame);

	MAUIEngine_run(engine);
	MAUIEngine_destroy(engine);

	return 0;
}

#include <MAUtil/MAPtrVector.c>
#include <MAUtil/MAXML.c>
#include <MAUtil/MAStream.c>
#include <MAUtil/MaStringMap.c>
#include <MAUtil/MaArray.c>

#include <MAUI/MAUIEngine.c>
#include <MAUI/MAUIWidget.c>
#include <MAUI/MAUIFrame.c>
#include <MAUI/MAUIButton.c>
#include <MAUI/MAUIImage.c>
#include <MAUI/MAUILabel.c>
#include <MAUI/MAUIListBox.c>
#include <MAUI/MAUIScrollPane.c>
#include <MAUI/Font.c>
#include <MAUI/DataProvider/MAUIQueryResult.c>
#include <MAUI/DataProvider/MAUIDataProvider.c>
#include "RSSQueryResult.c"
#include "RSSDataProvider.c"
#include "MAFeed.c"
