/**
 * @brief Defines a virtual keyboard class that can be used
 *        on touch devices.
 * @file VirtualKeyboard.h
 * @author Mattias Frånberg
 */

#ifndef VIRTUALKEYBOARD_H_
#define VIRTUALKEYBOARD_H_

#include <MAUI/Widget.h>
#include <MAUI/EditBox.h>

#include <MAUtil/CharInput.h>
#include <MAUtil/Environment.h>
#include <MAUtil/HashMap.h>
#include <MAUtil/Vector.h>

#include <MAUI/Keyboard/VirtualLayout.h>
#include <MAUI/Keyboard/VirtualKey.h>

namespace MAUI {

/**
 * Maps characters to MAK_ keycodes, or
 * MAK_UNKNOWN if no keycode for the specific
 * character exists.
 */
extern int CHAR_TO_EVENT_MAP[];

/**
 * VirtualKeyboard represents a virtual keyboard. It is designed
 * to only use one virtual keyboard at a time, and request its
 * service whenever it is needed. The class sends events to the
 * underlying event system once a key is pressed.
 *
 * The VirtualKeyboard can be said to consist of a set of layouts,
 * which can be switched between by pressing keys.
 */
class VirtualKeyboard
: public MAUI::Widget,
  public MAUtil::PointerListener,
  public MAUtil::TimerListener
{
public:
	/**
	 * Instantiates a virtual keyboard without any layouts.
	 */
	VirtualKeyboard();
	~VirtualKeyboard();

	/**
	 * Adds a layout to this keyboard.
	 *
	 * @param layout The layout that will be added.
	 */
	void addLayout(VirtualLayout *layout);

	/**
	 * Resets the keyboard to the starting state, i.e.
	 * the default layout.
	 */
	void reset();

	/* Implementation of Widget interface. */

	/**
	 * @see MAUI::Widget::drawWidget().
	 */
	void drawWidget();

	/* Implementation of PointerListener interface. */

	/**
	 * @see MAUtil::PointerListener::pointerPressEvent.
	 */
	void pointerPressEvent(MAPoint2d p);

	/**
	 * @see MAUtil::PointerListener::pointerMoveEvent.
	 */
	void pointerMoveEvent(MAPoint2d p);

	/**
	 * @see MAUtil::PointerListener::pointerReleaseEvent.
	 */
	void pointerReleaseEvent(MAPoint2d p);

	/**
	 * @see MAUtil::TimerListener::runTimerEvent.
	 */
	void runTimerEvent();

private:
	/**
	 * @param layoutName The name of the layout to change to.
	 * @returns true if we could change to the new layout, false
	 *          otherwise.
	 */
	bool changeLayout(const MAUtil::String &layoutName);

	/**
	 * Container for the layouts as mapping from the layout name
	 * to the actual layout instance.
	 */
	MAUtil::HashMap< MAUtil::String, VirtualLayout * > m_keyLayouts;

	/**
	 * The currently active key layout.
	 */
	VirtualLayout *m_curLayout;

	/**
	 * A list of pressed keys.
	 */
	MAUtil::Vector< VirtualKey * > m_pressedKeys;

	/**
	 * Indicates whether the keyboard is currently being shown.
	 */
	bool m_isShown;
};

} /* Namespace end */

#endif /* VIRTUALKEYBOARD_H_ */
