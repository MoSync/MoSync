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
	void requestListFocus();
protected:
	Widget* mListView;
};

}
}


#endif
