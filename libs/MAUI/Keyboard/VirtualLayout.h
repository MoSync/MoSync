/**
 * @brief Represents a keyboard layout, which is defined as a
 *        set of rows that contains keys.
 * @file VirtualLayout.h
 * @author Mattias Frånberg
 */
#ifndef VIRTUALLAYOUT_H_
#define VIRTUALLAYOUT_H_

#include <MAUI/Widget.h>

#include <MAUtil/String.h>
#include <MAUtil/Vector.h>

namespace MAUI {

/* Forward declerations */
class VirtualKey;
class VirtualLayoutRow;
class SkinSet;

/**
 * Default number of pixels between keys.
 */
#define VIRTUAL_LAYOUT_HSPACE 0
#define VIRTUAL_LAYOUT_VSPACE 0

/**
 * This class represents a single keyboard layout that
 * can be drawn on the screen. 
 */
class VirtualLayout
: public MAUI::Widget
{
public:
	/**
	 *
     *
	 * @param name A name of the layout, used to be able to switch
	 *             between layouts in a single keyboard.
	 * @param hspace Amount of horizontal space between the buttons.
	 * @param vspace Amount of vertical space between the rows.
	 */
	VirtualLayout(const char *name,
				  unsigned int hspace = VIRTUAL_LAYOUT_HSPACE,
				  unsigned int vspace = VIRTUAL_LAYOUT_VSPACE);
	~VirtualLayout();

	/**
	 * Returns the name of the layout.
	 *
	 * @return the name of the layout.
	 */
	MAUtil::String getName() const;

	/**
	 * Adds a key to this layout on the specified row.
	 *
	 * @param row The row on which the key should be added.
	 * @param key The key that will be added.
	 */
	void addKey(unsigned int row, VirtualKey *key);

	/**
	 * Returns the key at the specified position assuming
	 * that (0, 0) is at the top left corner of this layout.
	 *
	 * @param x X coordinate of the pressed position.
	 * @param y Y coordinate of the pressed position.
	 * @return the key at the specified position.
	 */
	VirtualKey *getKey(int x, int y) const;

	/**
	 * Returns the height of this layout in pixels.
	 */
	unsigned int getLayoutHeight() const;

	/**
	 * Sets the horizontal space between the keys.
	 *
	 * @param hspace Horizontal space between the keys.
	 */
	void setHorizontalSpace(unsigned int hspace);

	/**
	 * Sets the Vertical space between the key rows.
	 *
	 * @param vspace Vertical space between the keys.
	 */
	void setVerticalSpace(unsigned int vspace);

	/**
	 * Sets the relative height of the row.
	 *
	 * @param row index of the row the set the height.
	 * @param height Height.
	 */
	void setRowHeight(unsigned int row, float height);

	/**
	 * Toggles the shifting.
	 */
	void toggleShift();

	/**
	 * Returns true if the layout is shifted.
	 *
	 * @return true if the layout is shifted.
	 */
	bool shift() const;

	/**
	 * @see Widget::draw.
	 */
	void draw(bool forceDraw);

	/**
	 * @see Widget::drawWidget.
	 */
	void drawWidget();

private:
	/**
	 * Computes the total height of the rows in this keyboard,
	 * specified in units of row.
	 * 
	 * @return The total height of the rows in this keyboard.
	 */
	float totalHeight() const;

	/**
	 * Computes how many pixels a single unit of key and row
	 * should occupy based on:
	 * - The number of keys.
	 * - The number of rows.
	 * - The height of the layout.
	 *
	 * NOTE: Modifies the internal state of this class.
	 */
	void computeWidthHeight();

	/**
	 * Resizes the row vector so that size elements
	 * will fit in it, any allocated positions will
	 * be initialized to null.
	 */
	void resizeRows(unsigned int size);

	/**
	 * Returns the width of the given key.
	 *
	 * @param key The key to get the width of.
	 * @return the width of the given key.
	 */
	int keyWidth(const VirtualKey *key) const;

	/**
	 * Returns the height of the given row.
	 *
	 * @param row The row to get the height of.
	 * @return the height of the given row.
	 */
	int rowHeight(const VirtualLayoutRow *row) const;

	/**
	 * Returns the number of pixels that this row should
	 * be shifted to the right in order to be centered.
	 *
	 * @param row The row that should be centered.
	 * @return The number of pixels to the left of this row.
	 */
	int getRowAlignment(VirtualLayoutRow *row) const;

	/**
	 * Draws the keys in the given row on the screen. It is
	 * assumed that (0, 0) is the top left corner of this row.
	 * Hence Gfx_translate should be called accordingly, prior
	 * to this call.
	 *
	 * @param row The row of keys that will be drawn.
	 */
	void drawKeys(VirtualLayoutRow *row) const;

	/**
	 * The name of the keyboard layout.
	 */
	MAUtil::String m_name;

	/**
	 * A list of rows containing keys.
	 */
	MAUtil::Vector< VirtualLayoutRow * > m_rows;

	/**
	 * Horizontal space between the keys.
	 */
	unsigned int m_hspace;

	/**
	 * Vertical space between the keys.
	 */
	unsigned int m_vspace;

	/**
	 * Indicates whether keys are shifted or not.
	 */
	bool m_shift;

	/**
	 * The width in pixels of 1.0 unit of key.
	 */
	int m_keyWidthPixels;

	/**
	 * The height in pixels of 1.0 unit of row.
	 */
	int m_rowHeightPixels;

	/**
	 * Skins used for this keyboard.
	 */
	SkinSet *m_skins;
};


/**
 * This class represents a row of keys, the purpose
 * 
 */
class VirtualLayoutRow
{
public:
	VirtualLayoutRow();
	~VirtualLayoutRow();

	/**
	 * Adds a key to this row.
	 *
	 * @param Adds a key to this row.
	 */
	int addKey(VirtualKey *key);

	/**
	 * Returns the key with the given index.
	 *
	 * @param The index of the key to be returned.
	 * @returns The key at the given index.
	 */
	VirtualKey * getKey(unsigned int index) const;

	/**
	 * Returns the number of keys in this row.
	 *
	 * @returns the number of keys.
	 */
	unsigned int numKeys() const;

	/**
	 * Returns the summed width of all the keys.
	 * There is no horizontal spacing included.
	 *
	 * @return the summed width of all the keys.
	 */
	int totalWidth() const;

	/**
	 * Sets the height of this row in units of row.
	 *
	 * @param height The new height of the row.
	 */
	void setHeight(float height);

	/**
	 * Returns the height of this row in units of row.
	 *
	 * @return the height of this row in units of row.
	 */
	float getHeight() const;

private:
	/**
	 * A list of the keys that are contained in this row.
	 */
	MAUtil::Vector< VirtualKey * > m_keys;

	/**
	 * Relative height of the row.
	 */
	float m_height;
};

} /* Namespace end */

#endif /* VIRTUALKEYLAYOUT_H_ */
