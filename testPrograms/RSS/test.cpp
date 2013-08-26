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

#include <maapi.h>
#include "MAFeed.h"

#include "MAHeaders.h"
#include <MAUI/Engine.h>
#include "MAUI/Label.h"
#include "MAUI/Image.h"
#include "MAUI/ListBox.h"
#include "MAUI/ScrollPane.h"
#include "MAUI/EditBox.h"
#include "RSSQueryResult.h"
#include "RSSDataProvider.h"
#include "conprint.h"

using namespace MAUI;

Label *label;
ListBox *listbox;
Image *article;
Image *menu;
ScrollPane *scrollPane;
EditBox *editBox;
DataProvider *dataProvider;

class RSSItemChosenListener : public ItemChosenListener {
public:
	void onItemChosen(ListBox *sender, Widget * /*chosenWidget*/) {
		label->setRowIndex(sender->getSelectedIndex());
		label->updateFromDataProvider();
		Engine &engine = Engine::getSingleton();
		engine.setMain(article);
		engine.setFocus(scrollPane);
	}
};

class MenuKeyListener : public KeyListener {
	public:
		void onKeyEvent(Widget *sender, KeyEvent *ke) {
			Engine &engine = Engine::getSingleton();
			if(ke->pressed&MAK_FIRE)
			{
				engine.setFocus(editBox);
			}
		}
};

class EditBoxKeyListener : public KeyListener {
	public:
		void onKeyEvent(Widget *sender, KeyEvent *ke) {
			Engine &engine = Engine::getSingleton();
			if(ke->pressed&MAK_FIRE)
			{
				RSSDataProvider *d = new RSSDataProvider(((EditBox*)sender)->getCaption());
				if(d->hasFeed()) 
				{
					if(dataProvider)
						delete dataProvider;

					dataProvider = d;
					label->setDataProvider(d);
					listbox->setDataProvider(d);
					listbox->updateFromDataProvider();
				}
				engine.setFocus(listbox);
			}
		}
};

class ScrollPaneKeyListener : public KeyListener{
	public:
		void onKeyEvent(Widget *sender, KeyEvent *ke) {
		int keys = ke->keys;
		ScrollPane *s = ((ScrollPane*)sender);
		Engine &engine = Engine::getSingleton();
		int offset = s->getOffsetY();

		if(keys&MAK_DOWN)
		{
			offset-=2;
			s->setOffsetY(offset);
		}
		if(keys&MAK_UP)
		{
			offset+=2;
			if(offset>0) offset = 0;
			s->setOffsetY(offset);
		}
		if(ke->pressed&MAK_LEFT)
		{
			engine.setMain(menu);
			engine.setFocus(listbox);
		}		
		}
};

extern "C" 
{
	int MAMain()
	{

		Engine &engine = Engine::getSingleton();

		engine.setSystemFont(SYSTEM_FONT);

		menu = new Image(0, 0, 176, 208, NULL, R_LOGO);

		article = new Image(0, 0, 176, 208, NULL, R_BACK);
		
		scrollPane = new ScrollPane(0, 0, 176, 208, article, 0, 0, false, true);
		scrollPane->addKeyListener(new ScrollPaneKeyListener());

		dataProvider = new RSSDataProvider("http://rss.cnn.com/rss/cnn_topstories.rss");

		label = new Label(0, 0, 176, 208, scrollPane, "", 0, true);
		label->setDataProvider(dataProvider);
		label->setQueryString("items");
		label->setColumnName("description");
		label->setQueryInterval(30);

		listbox = new ListBox(0, 25, 176, 150, menu);
		listbox->setDataProvider(dataProvider);
		listbox->setQueryString("items");
		listbox->setColumnName("title");
		listbox->setQueryInterval(30);
		listbox->addItemChosenListener(new RSSItemChosenListener());
		listbox->addKeyListener(new MenuKeyListener());
		listbox->setItemChosenedKey(MAK_RIGHT);

		editBox = new EditBox(0, 178, 176, 10, menu, "", 0, false);
		editBox->setCaption("http://www.aftonbladet.se/rss/nyheter.xml");
		editBox->addKeyListener(new EditBoxKeyListener());

		engine.setMain(menu);
		engine.setFocus(listbox);

		engine.run();

		return 0;
	}
}