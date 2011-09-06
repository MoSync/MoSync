/*
 * ListScreen.h
 *
 *  Created on: Aug 5, 2011
 *      Author: gabi
 */

#ifndef LISTSCREEN_H_
#define LISTSCREEN_H_

#include <MAUtil/Environment.h>

#include "FacebookDemoScreen.h"
#include <NativeUI/WidgetEventListener.h>
#include "ListItem.h"
#include "FacebookDemoScreen.h"
#include <NativeUI/Widgets.h>

namespace FacebookDemoGUI
{

class ListScreen: public FacebookDemoScreen, public NativeUI::ListViewListener, public NativeUI::ButtonListener
{
public:
	ListScreen(FacebookDemoScreen *prev);
	ListScreen();

	virtual void show();

	void add(ListItem *btn);

	bool isEmpty() const;

	void clear();
	void clearScreenAfterLosingFocus(bool clear);

	virtual ~ListScreen();


    virtual void listViewItemClicked(NativeUI::ListView* listView, NativeUI::ListViewItem* listViewItem);

    virtual void buttonClicked(Widget* button);

private:
	virtual int addChild(NativeUI::Widget* widget);

private:
	void initialize();

protected:
	virtual void back();

protected:
	NativeUI::VerticalLayout 	*mLayout;
	NativeUI::ListView 			*mList;
	NativeUI::Button			*mBackButton;

	bool 						mClearScreenAfterLosingFocus;
};

}//namespace FacebookDemoGUI


#endif /* MENUSCREEN_H_ */
