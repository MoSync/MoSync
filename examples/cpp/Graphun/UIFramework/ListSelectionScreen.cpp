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
#include <IX_WIDGET.h>
#include "ListSelectionScreen.h"

namespace MoSync {
namespace UI {

	ListSelectionScreen::ListSelectionScreen() {

		mListView = new Widget("ListView");
		mListView->setProperty("width", "-1");
		mListView->setProperty("height", "-1");

		setMainWidget(mListView);


	}

	void ListSelectionScreen::addStrings(Vector<String>& items) {
		for(int i = 0; i < items.size(); i++) {
			Widget* item = new Widget("ListViewItem");
			item->setProperty("text", items[i].c_str());
			mListView->addChild(item);
		}
	}

	void ListSelectionScreen::listboxItemSelected(UIItem* item, int index) {

	}

	void ListSelectionScreen::requestListFocus()
	{
		maWidgetSetProperty(mListView->getHandle(), MAW_LIST_VIEW_REQUEST_FOCUS, "true");
	}
}
}
