/*
Copyright (C) 2011 MoSync AB

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License,
version 2, as published by the Free Software Foundation.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
MA 02110-1301, USA.
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
protected:
	Widget* mListView;
};

}
}


#endif
