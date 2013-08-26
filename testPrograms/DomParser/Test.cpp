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

#include <ma.h>
#include <MAUtil/DomParser.h>
#include <MAUtil/XMLDataProvider.h>
#include "MAHeaders.h"

#include <MAUI/Engine.h>
#include <MAUI/Font.h>
#include <MAUI/Label.h>
#include <MAUI/ListBox.h>
#include <MAUI/Image.h>
#include <MAUI/Layout.h>
#include <MAUI/ImageGenerators.h>
#include <MAUtil/Moblet.h>

using namespace MAUI;
using namespace MAUtil;
using namespace Dom;

class ListBoxIdleListener : public IdleListener {
public:
	ListBoxIdleListener(ListBox* listBox) {
		this->listBox = listBox;	
		startTime = maGetMilliSecondCount();
	}

	void idle() {
		if(maGetMilliSecondCount()-startTime < delay) {
			return;
		}

		startTime = maGetMilliSecondCount();

		if(key == MAK_UP || key == MAK_2) {
			listBox->setPrev();
		} else 
		if(key == MAK_DOWN || key == MAK_8 ) {
			listBox->setNext();
		}

		if(delay > 50)delay -= 50;
	}

	ListBox* listBox;
	int delay;
	int startTime;
	int key;
	
};


class MyMoblet : public Moblet {
public:
	ListBox *listBox;
	ListBox *navigator;
	ListBoxIdleListener* idleListener;
	Label *label;
	XMLDataProvider *dataProviderDesc;


	void keyPressEvent(int keyCode) {
		if(navigator->getSelectedIndex() == 0) {
			if(keyCode == MAK_UP || keyCode== MAK_DOWN || keyCode == MAK_2 || keyCode == MAK_8) {
				this->addIdleListener(idleListener);

				idleListener->startTime = maGetMilliSecondCount();
				if(keyCode == MAK_UP || keyCode == MAK_2) {
					listBox->setPrev();
				} else 
				if(keyCode == MAK_DOWN || keyCode == MAK_8) {
					listBox->setNext();
				}

				idleListener->startTime = maGetMilliSecondCount();
				idleListener->delay = 300;
				idleListener->key   = keyCode;
			}	
		}
		if( keyCode == MAK_FIRE ) {
			if(navigator->getSelectedIndex() == 0) {
				navigator->setNext();
				int index = listBox->getSelectedIndex();
				const Vector<Node*>& nodes = dataProviderDesc->getData();
				const Node* n = nodes[index];
				switch(n->getType()) {
					case Node::ATTRIBUTE:
						{
							Attribute* attribute = (Attribute*)n;
							label->setCaption(attribute->getValue());
						}
						break;
					case Node::CDATA:
						{
							CData* cdata = (CData*)n;
							label->setCaption(cdata->getCDATA());
						}
						break;
					case Node::ELEMENT:
							Element* elem = (Element*)n;
							label->setCaption(elem->getCDATA());
						break;
				}			
			} else {
				navigator->setPrev();
			}
		}
	}

	void keyReleaseEvent(int keyCode) {
		if(idleListener->key != keyCode) return;
		if(keyCode == MAK_UP || keyCode == MAK_DOWN || keyCode == MAK_2 || keyCode == MAK_8) {
			this->removeIdleListener(idleListener);
			idleListener->delay = 300;
			idleListener->key   = 0;
		}
	}

	MyMoblet() {
		//Document *document = DomParser::parseToDocument(XML_FILE);
		Document *document = DomParser::parseToDocument("http://feeds.feedburner.com/timeblogs/nerd_world?format=xml");
		if(!document) {
			//printf("could not read doc from internet");
			maExit(1);
		}
		XMLDataProvider *dataProvider = new XMLDocDataProvider(document, "/channel/item/title");
		dataProviderDesc = new XMLDocDataProvider(document, "/channel/item/description");

		Font *font = new MAUI::Font(FONT, 0xffffff);
		Engine& engine = Engine::getSingleton();
		engine.setSystemFont(font);
		Extent screenSize = maGetScrSize();
		int scrWidth = EXTENT_X(screenSize);
		int scrHeight = EXTENT_Y(screenSize);
		Handle b = BACK;
		Image *back = new Image(0, 0, scrWidth, scrHeight, NULL, BACK, false);
		navigator = new ListBox(0, 0, scrWidth, scrHeight, back, LBO_HORIZONTAL, LBWM_ANIMATED);
		listBox = new ListBox(0, 0, scrWidth, scrHeight, navigator, LBO_VERTICAL, LBWM_ANIMATED);
		listBox->setDataProvider(dataProvider);

		label = new Label(0, 0, scrWidth, scrHeight, navigator, "", 0, false, font);
		label->setHorizontalAlignment(2);
		label->setHorizontalAlignment(2);


		engine.setMain(back);
		idleListener = new ListBoxIdleListener(listBox);
	}
};

extern "C" int MAMain() {
	MyMoblet moblet;
	MyMoblet::run(&moblet);
	return 0;
}