/**
 * @brief Defines a class that represents a single key on a virtual keyboard.
 * @file VirtualKey.h
 * @author Mattias Frånberg
 */

#ifndef VIRTUALKEY_H_
#define VIRTUALKEY_H_

#include <MAUtil/String.h>
#include <MAUI/WidgetSkin.h>

namespace MAUI {

/**
 * Max length of the string outputted when a key is pressed.
 */
#define VIRTUAL_KEY_STR_LEN 6
#define BUFFER_LENGTH 30

/**
 * This class represents a key on a virtual keyboard. It holds
 * information about what string that should be outputted when
 * it is pressed, or what action should be taken. The state of
 * the key if it is pressed or not.
 */
class VirtualKey
{
private:
	/**
	 * The default width of a key specified in the number
	 * of keys it should span.
	 */
	static const unsigned int DEFAULT_KEY_WIDTH = 1;

public:
	enum Type {
		NONE,			/* No type */
		KEY, 			/* An ordinary key */
		HIDE, 			/* Hides the virtual keyboard */
		REMOVE, 		/* Removes a character */
		SHIFT, 			/* Activates shift */
		LINE_FEED, 		/* Represents a line feed */
		LEFT,			/* Movement keys */
		RIGHT,
		UP,
		DOWN,
		CHANGE_LAYOUT 	/* Changes the layout */
	};

	/**
	 * constructor
	 *
	 * @param label The string that will be shown on the key.
	 * @param chars The characters that are supported by the key.
	 * @param keyType The type or action of the key.
	 * @param newLayout If the key changes the layout, this is the
	 *                  name of the new layout.
	 * @param width The width of the key.
	 */
	VirtualKey(const char *label,
			   const char *outputString,
			   VirtualKey::Type keyType = KEY,
			   const char *newLayout = NULL,
			   int width = DEFAULT_KEY_WIDTH,
			   WidgetSkin *icon = NULL
			   );

	/**
	 * Returns the width of the key in pixels.
	 *
	 * @return the width of the key.
	 */
	int getWidth() const;

	/**
	 * Returns the height of the key in pixels.
	 *
	 * @return the height of the key.
	 */
	float getHeight() const;

	/**
	 * Returns the type of this key.
	 *
	 * @returns the type of this key.
	 */
	VirtualKey::Type getType() const;

	/**
	 * Returns the label that is displayed for this key.
	 *
	 * @return The label.
	 */
	const char *getLabel(bool shift = false) const;

	/**
	 * Returns the label that is displayed for this key.
	 *
	 * @return The label.
	 */
	WidgetSkin *getIcon() const;

	/**
	 * Returns the string that should be outputted when this
	 * key is pressed.
	 *
	 * @param shift Determines if the shift key is held down
	 * 				or not.
	 *
	 * @return the string that should be outputted when this
	 *         key is pressed.
	 */
	const char *getOutputString(bool shift = false) const;

	/**
	 * Returns the name of the layout that should be
	 * shown when this key is pressed. Will be an empty
	 * string if no layout is available.
	 *
	 * @returns Returns the name of the layout that should be
	 * shown when this key is pressed. Will be an empty
	 * string if no layout is available.
	 */
	MAUtil::String getLayout() const;

	/**
	 * Simulates a press to this key.
	 */
	void press();

	/**
	 * Simulates a release of the press to this key.
	 */
	void unpress();

	/**
	 * Returns true if the key is pressed.
	 *
	 * @return true if the key is pressed, false otherwise.
	 */
	bool pressed() const;

	/**
	 * Returns the number of miliseconds that key is has
	 * been pressed.
	 *
	 * @return the number of miliseconds that key is has
	 * been pressed.
	 */
	int timePressed() const;

private:
	/**
	 * Shifts all characters in a given string.
	 *
	 * @param chars The chars that will be shifted.
	 * @param shiftedChars The shifted chars will be stored here.
	 */
	void shiftChars(const char *chars, char *shiftedChars);

	/**
	 * The label that is shown on the key button.
	 */
	char m_label[VIRTUAL_KEY_STR_LEN];

	/**
	 * An icon that can be drawn instead of a label.
	 */
	WidgetSkin *m_icon;

	/**
	 * The label that is shown on the key button when shift
	 * has been pressed.
	 */
	char m_shiftedLabel[VIRTUAL_KEY_STR_LEN];

	/**
	 * The string that will be written when the key is pressed.
	 */
	char m_output[VIRTUAL_KEY_STR_LEN];

	/**
	 * The string that will be outputted when the key is shifted.
	 */
	char m_shiftedOutput[VIRTUAL_KEY_STR_LEN];

	/**
	 * The type or action of the key.
	 */
	VirtualKey::Type m_keyType;

	/**
	 * If this key changes the layout, this is the
	 * name of the new layout.
	 */
	MAUtil::String m_newLayout;

	/**
	 * Width of the key.
	 */
	int m_width;

	/**
	 * The number of miliseconds that this key has been pressed.
	 * Is negative if the key is not pressed.
	 */
	int m_timePressed;

};

} /* Namespace end */

#endif /* VIRTUALKEY_H_ */
