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

/** \file ScrollPane.h Copyright (c) Mobile Sorcery AB 2005-2006 **/

#ifndef _SE_MSAB_MAUI_SCROLLPANE_H_
#define _SE_MSAB_MAUI_SCROLLPANE_H_

#include "Widget.h"

namespace MAUI {

	class ScrollPane : public Widget {
	public:
		ScrollPane(int x, int y, int width, int height, Widget *parent=NULL);
		ScrollPane(int x, int y, int width, int height, Widget *parent, int offsetX, int offsetY, bool scrollBarX, bool scrollBarY);
		~ScrollPane();

		void draw();

		void setOffsetX(int offsetX);
		void setOffsetY(int offsetY);
		void setScrollBarX(bool scrollBarX);
		void setScrollBarY(bool scrollBarY);
		int getOffsetX() const;
		int getOffsetY() const;
		bool getScrollBarX() const;
		bool getScrollBarY() const;

	protected:
		int offsetX;
		int offsetY;
		bool scrollBarX;
		bool scrollBarY;
	};

}

#endif /* _SE_MSAB_MAUI_SCROLLPANE_H_ */
