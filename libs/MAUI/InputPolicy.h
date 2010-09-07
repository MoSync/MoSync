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
		void setOwner(Widget *widget);
		Widget* getOwner();

	// returns true if focus should be changed.
		virtual bool keyPressed(int keyCode, int nativeCode);
		virtual bool keyReleased(int keyCode, int nativeCode);

	// returns true if focus should be lost.
	// what about: returns true if focus should be held after release.
		virtual bool pointerPressed(MAPoint2d p, int id);
		virtual bool pointerMoved(MAPoint2d p, int id);
		virtual bool pointerReleased(MAPoint2d p, int id);

	protected:
		Widget *owner;
		Widget *focus;
	};


	class DefaultInputPolicy : public InputPolicy {
	public:
		virtual bool keyPressed(int keyCode, int nativeCode);
		virtual bool keyReleased(int keyCode, int nativeCode);
		virtual bool pointerPressed(MAPoint2d p, int id);
		virtual bool pointerMoved(MAPoint2d p, int id);
		virtual bool pointerReleased(MAPoint2d p, int id);

	protected:
	};


}

#endif // _INPUT_POLICY_H_
