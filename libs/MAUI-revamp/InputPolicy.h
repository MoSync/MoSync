/* Copyright (C) 2010 MoSync AB

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

#ifndef _INPUT_POLICY_H_
#define _INPUT_POLICY_H_

#include <maapi.h>

namespace MAUI {
	class Widget;

	enum Direction {
			LEFT,
			RIGHT,
			UP,
			DOWN,
			NONE
	};

	Direction mapKeyCodeToDirection(int keyCode);

	class InputPolicy {
	public:
		InputPolicy(Widget* owner);
		void setOwner(Widget *widget);
		Widget* getOwner();

	// returns false if focus should be changed.
		virtual bool keyPressed(int keyCode, int nativeCode);
		virtual bool keyReleased(int keyCode, int nativeCode);

	// returns false if focus should be lost, true otherwise.
		virtual bool pointerPressed(MAPoint2d p, int id);
		virtual bool pointerMoved(MAPoint2d p, int id);
		virtual bool pointerReleased(MAPoint2d p, int id);

	protected:
		Widget *mOwner;
	};


	class DefaultInputPolicy : public InputPolicy {
	public:
		DefaultInputPolicy(Widget* owner);
		virtual bool keyPressed(int keyCode, int nativeCode);
		virtual bool keyReleased(int keyCode, int nativeCode);
		virtual bool pointerPressed(MAPoint2d p, int id);
		virtual bool pointerMoved(MAPoint2d p, int id);
		virtual bool pointerReleased(MAPoint2d p, int id);

	protected:
	};


}

#endif // _INPUT_POLICY_H_
