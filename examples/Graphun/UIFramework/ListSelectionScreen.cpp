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
}
}
