#include <MAUI/Font.h>
#include <MAUI/WidgetSkin.h>

#include <MAUtil/Graphics.h>
#include <MAUtil/Vector.h>

#include <MAUI/Keyboard/Util/SkinSet.h>
#include <MAUI/Keyboard/Util/ResourceManager.h>
#include <MAUI/Keyboard/VirtualKey.h>

#include "VirtualLayout.h"

#define DEFAULT_PADDING 5

namespace MAUI {

VirtualLayout::VirtualLayout(const char *name,
							 unsigned int hspace,
							 unsigned int vspace)
	: MAUI::Widget(0, 0, 0, 0, NULL)
{
	setPaddingLeft( DEFAULT_PADDING );
	setPaddingRight( DEFAULT_PADDING );
	setPaddingTop( DEFAULT_PADDING );
	setPaddingBottom( DEFAULT_PADDING );

	m_skins = ResourceManager::getSkins( "keyboard" );
	setSkin( m_skins->getSkin( "layout" ) );

	m_name = MAUtil::String( name );
	m_hspace = hspace;
	m_vspace = vspace;
	m_shift = false;
}

VirtualLayout::~VirtualLayout()
{
	for(int row = 0; row < m_rows.size( ); row++)
	{
		if(m_rows[row] != NULL)
		{
			delete m_rows[ row ];
		}
	}
}

MAUtil::String
VirtualLayout::getName() const
{
	return m_name;
}

void
VirtualLayout::addKey(unsigned int row, VirtualKey *key)
{
	/* If necessary add some rows, so that the total number
	 * adds up to the given row */
	if(m_rows.size() <= (int) row)
	{
		resizeRows( row + 1 );
	}

	/* Finally add the new row */
	if(m_rows[row] == NULL)
	{
		m_rows[row] = new VirtualLayoutRow( );
	}
	m_rows[row]->addKey( key );
}

VirtualKey *
VirtualLayout::getKey(int x, int y) const
{
	int startY = getPaddingTop( );
	int startX = getPaddingLeft( );

	/* Get the row that was clicked. */
	VirtualLayoutRow *clickedRow = NULL;
	for(int rowIndex = 0; rowIndex < m_rows.size( ); rowIndex++)
	{
		if( y >= startY && y <= startY + rowHeight( m_rows[ rowIndex ] ) )
		{
			clickedRow = m_rows[ rowIndex ];
			break;
		}
		startY += rowHeight( m_rows[ rowIndex ] ) + m_vspace;
	}

	if( clickedRow == NULL )
	{
		return NULL;
	}

	/* Set x and y to the top left corner of the first key in the row  */
	startX += getRowAlignment( clickedRow );

	/* For each key, see if the click is inside their bounding box */
	for(unsigned int keyIndex = 0; keyIndex < clickedRow->numKeys(); keyIndex++)
	{
		VirtualKey *key = clickedRow->getKey( keyIndex );
		MAUtil::Rect rect( startX, startY, keyWidth( key ), rowHeight( clickedRow ) );
		if( rect.contains(x, y) )
		{
			return key;
		}

		/* Move forward one key */
		startX += keyWidth( key ) + m_hspace;
	}

	return NULL;
}

void
VirtualLayout::setHorizontalSpace(unsigned int hspace)
{
	m_hspace = hspace;
}

void
VirtualLayout::setVerticalSpace(unsigned int vspace)
{
	m_vspace = vspace;
}

void 
VirtualLayout::resizeRows(unsigned int size)
{
	if(m_rows.size() < (int) size)
	{
		int currentSize = m_rows.size();
		m_rows.resize( size );

		/* Reset the extra rows to NULL */
		for(int i = currentSize; i < m_rows.size(); i++)
		{
			m_rows[i] = NULL;
		}
	}
}

unsigned int
VirtualLayout::getLayoutHeight() const
{
	return 0;
}

void VirtualLayout::drawWidget()
{

}

void VirtualLayout::draw(bool forceDraw)
{
	unsigned int y = 0;
	computeWidthHeight( );

	Gfx_pushMatrix();

	/* The background should be drawn before we move the padding */
	drawBackground();

	Gfx_translate( getPaddingLeft(), getPaddingTop() );
	for(int row = 0; row < m_rows.size(); row++)
	{
		VirtualLayoutRow *curRow = m_rows[ row ];

		Gfx_pushMatrix();

		/* Translate so that (0,0) is the top left
		 * corner of the row. */
		Gfx_translate( getRowAlignment( curRow ), y );
		drawKeys( curRow );

		Gfx_popMatrix();

		/* Move y coordinate to the next row */
		y += m_vspace + rowHeight( m_rows[ row ] );
	}

	Gfx_popMatrix();
}

void VirtualLayout::drawKeys(VirtualLayoutRow *row) const
{
	unsigned int x = 0;
	MAUI::Font *font = ResourceManager::getFont( "key" );

	for(unsigned int keyIndex = 0; keyIndex < row->numKeys( ); keyIndex++)
	{
		VirtualKey *key = row->getKey( keyIndex );
		MAUI::WidgetSkin *skin = m_skins->getSkin( "key" );
		MAUI::WidgetSkin::eType skinType = !key->pressed() ? MAUI::WidgetSkin::UNSELECTED
														   : MAUI::WidgetSkin::SELECTED;

		/* Draw the skin of the key */
		skin->draw( x, 0, keyWidth( key ), rowHeight( row ), skinType );

		/* Calculate center placement of the string */
		MAExtent labelBounds = font->getStringDimensions( key->getLabel( shift( ) ) );
		int startX = (keyWidth( key ) - EXTENT_X( labelBounds )) / 2;
		int startY = (rowHeight( row ) - EXTENT_Y(labelBounds)) / 2;

		/* Draw the string in the center of the button */
		font->drawString(key->getLabel( shift( ) ), x + startX, startY);

		/* Move to the next key */
		x += m_hspace + keyWidth( key );
	}
}

int
VirtualLayout::getRowAlignment(VirtualLayoutRow *row) const
{
	/* Calculate the total width of all the keys in the row */
	int rowWidth = (int) row->totalWidth( ) * m_keyWidthPixels + (row->numKeys() - 1) * m_hspace;

	/* The total space not occupied by keys (i.e. left and right padding) */
	int spaceLeft = getWidth() - (rowWidth + getPaddingLeft() + getPaddingRight());

	if(spaceLeft >= 0)
	{
		return spaceLeft / 2;
	}
	else
	{
		return 0;
	}
}

void
VirtualLayout::toggleShift()
{
	m_shift = !m_shift;
}

bool
VirtualLayout::shift() const
{
	return m_shift;
}

int
VirtualLayout::keyWidth(const VirtualKey *key) const
{
	return key->getWidth( ) * m_keyWidthPixels + (key->getWidth( ) - 1) * m_hspace;
}

int
VirtualLayout::rowHeight(const VirtualLayoutRow *row) const
{
	return (int) row->getHeight( ) * m_rowHeightPixels;
}

float
VirtualLayout::totalHeight() const
{
	float totalHeight = 0;
	for(int i = 0; i < m_rows.size( ); i++)
	{
		totalHeight += m_rows[ i ]->getHeight( );
	}

	return totalHeight;
}

void 
VirtualLayout::computeWidthHeight()
{
	int numRows = m_rows.size( );

	/* Uniform height of this widget minus the padding */
	float totalRowHeight = totalHeight( );
	int heightLeft = getHeight( ) - (getPaddingTop() + getPaddingBottom()) - (numRows - 1) * m_vspace;
	m_rowHeightPixels = (int) (((float) heightLeft) / totalRowHeight);

	/* Find the row with the maximum number of keys */
	int maxRowSize = 0;
	for(int i = 0; i < numRows; i++)
	{
		if( m_rows[ i ]->totalWidth( ) > maxRowSize )
		{
			maxRowSize = m_rows[ i ]->totalWidth( );
		}
	}
	
	if( maxRowSize > 0 )
	{
		/* Uniform width of this widget minus the padding */
		int widthLeft = getWidth( ) - (getPaddingLeft() + getPaddingRight()) - (maxRowSize - 1) * m_hspace;
		m_keyWidthPixels = widthLeft / maxRowSize;
	}
}

void
VirtualLayout::setRowHeight(unsigned int row, float height)
{
	if(m_rows.size() <= (int) row)
	{
		resizeRows( row + 1 );
	}

	if( m_rows[ row ] == NULL )
	{
		m_rows[ row ] = new VirtualLayoutRow( );
	}

	m_rows[ row ]->setHeight( height );
}

VirtualLayoutRow::VirtualLayoutRow()
: m_height( 1.0 )
{
}

VirtualLayoutRow::~VirtualLayoutRow()
{
	for(unsigned int i = 0; i < numKeys( ); i++)
	{
		delete m_keys[ i ];
	}
}

int
VirtualLayoutRow::addKey(VirtualKey *key)
{
	m_keys.add( key );
	return m_keys.size( );
}

VirtualKey *
VirtualLayoutRow::getKey(unsigned int index) const
{
	if( index < numKeys( ) )
	{
		return m_keys[index];
	}
	else
	{
		return NULL;
	}
}

unsigned int
VirtualLayoutRow::numKeys() const
{
	return m_keys.size( );
}

int
VirtualLayoutRow::totalWidth() const
{
	int width = 0;
	for(int keyIndex = 0; keyIndex < m_keys.size(); keyIndex++)
	{
		width += m_keys[ keyIndex ]->getWidth( );
	}
	return width;
}

float
VirtualLayoutRow::getHeight() const
{
	return m_height;
}

void
VirtualLayoutRow::setHeight(float height)
{
	m_height = height;
}


} /* Namespace end */

