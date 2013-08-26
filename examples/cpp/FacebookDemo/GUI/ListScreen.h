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

/*
 * ListScreen.h
 */

#ifndef LISTSCREEN_H_
#define LISTSCREEN_H_

#include <MAUtil/Environment.h>

#include "FacebookDemoScreen.h"

#include "ListItem.h"
#include "FacebookDemoScreen.h"
#include <NativeUI/Widgets.h>

namespace FacebookDemoGUI
{

class ListScreen: public FacebookDemoScreen, public NativeUI::ButtonListener, public NativeUI::ListViewListener
{
public:
	ListScreen(FacebookDemoScreen *prev);
	ListScreen();

	virtual void show();

	void clear();

	virtual void listViewItemClicked(
		NativeUI::ListView *listView,
		NativeUI::ListViewSection *listViewSection,
		NativeUI::ListViewItem *listViewItem);

	virtual void buttonClicked(Widget* button);

	virtual ~ListScreen();

private:
	void initialize();

protected:
	virtual void back();

};

}//namespace FacebookDemoGUI


#endif /* MENUSCREEN_H_ */
