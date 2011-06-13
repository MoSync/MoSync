#ifndef _LIST_SELECTION_SCREEN_H_
#define _LIST_SELECTION_SCREEN_H_

#include "Screen.h"

namespace MoSync {
namespace UI {

using MAUtil::String;
using MAUtil::Vector;

class ListSelectionScreen : public Screen {
public:
	ListSelectionScreen();
	void addStrings(Vector<String>& items);

	virtual void listboxItemSelected(UIItem* item, int index);
protected:
	Widget* mListView;
};

}
}


#endif
