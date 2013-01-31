/* Copyright (C) 2012 MoSync AB

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
package com.mosync.nativeui.ui.widgets;

import static com.mosync.nativeui.ui.widgets.ListLayout.ITEM_VIEW_TYPE_FOOTER;
import static com.mosync.nativeui.ui.widgets.ListLayout.ITEM_VIEW_TYPE_HEADER;
import static com.mosync.internal.generated.IX_WIDGET.MAW_LIST_VIEW_SECTION_TYPE_SEGMENTED;
import static com.mosync.internal.generated.IX_WIDGET.MAW_LIST_VIEW_SECTION_TYPE_ALPHABETICAL;
import java.util.ArrayList;

import android.app.Activity;
import android.util.Log;
import android.widget.ListView;

import com.mosync.internal.android.MoSyncThread;
import com.mosync.internal.android.MoSyncFont.MoSyncFontHandle;
import com.mosync.internal.generated.IX_WIDGET;
import com.mosync.nativeui.core.Types;
import com.mosync.nativeui.ui.factories.ViewFactory;
import com.mosync.nativeui.util.properties.IntConverter;
import com.mosync.nativeui.util.properties.InvalidPropertyValueException;
import com.mosync.nativeui.util.properties.PropertyConversionException;


/**
 * A Section is a container for ListItemWidgets.
 * Widget properties cannot be set on a Section.
 * Extend from layout so that maWidgetDestroy will destroy
 * all it's children.
 *
 * Two types of list views can parent a ListViewSection:
 *  - Alphabetical list.
 *  - Segmented list. When created it will automatically
 *  hold a header and a footer layout which are in fact
 *  customizable ListItemWidgets.
 *
 * @author emma
 */
public class ListViewSection extends Layout
{
	static final int LIST_VIEW_SECTION_TYPE_NONE_SELECTED = -1;

	/**
	 * Section type:
	 * MAW_LIST_VIEW_SECTION_TYPE_SEGMENTED
	 * MAW_LIST_VIEW_SECTION_TYPE_ALPHABETICAL
	 */
	private int mSectionType = LIST_VIEW_SECTION_TYPE_NONE_SELECTED;

	/**
	 * Default header and footer appearance for segmented list views.
	 */
	static final String HEADER_DEFAULT_BACKGROUND_COLOR = "696969";
	static final String FOOTER_DEFAULT_BACKGROUND_COLOR = "696969";
	static final int HEADER_DEFAULT_FONT_SIZE = 20;
	static final int FOOTER_DEFAULT_FONT_SIZE = 15;

	/**
	 * Listener for when the items are removed or added.
	 */
	private AdapterChangedListener mAdapterListener = null;

	/**
	 * Section content: list view items.
	 * For segmented list it will also include default header and footer.
	 */
	private ArrayList<ListItemWidget> mItems = new ArrayList<ListItemWidget>();

	/**
	 * The preview letter displayed on the fast scroll thumb.
	 * Used only on alphabetical lists.
	 */
	public String mAlphabeticIndex = "";

	private Activity mAppActivity = null;

	/**
	 * The section has an empty footer by default.
	 * The footer can be removed by explicitely setting the
	 * MAW_LIST_VIEW_SECTION_FOOTER property to an empty string.
	 */
	private Boolean mHasFooter = true;

	/**
	 * Constructor.
	 * Add by default a header and a footer layout.
	 * Set different background colors and align labels to center.
	 *
	 * @param handle Integer handle corresponding to this instance.
	 * @param listView
	 * @param activity
	 */
	public ListViewSection(
			int handle, ListView listView, Activity activity)
	{
		super( handle, listView );
		mAppActivity = activity;
	}

	public int getSectionType()
	{
		return mSectionType;
	}

	@Override
	public int addChildAt(Widget child, int index)
	{
		if (!(child instanceof ListItemWidget))
		{
			Log.e( "MoSync",
					"maWidgetInsertChild: ListViewSection can only contain ListItemWidgets." );
			return IX_WIDGET.MAW_RES_INVALID_HANDLE;
		}

		int listIndex = index;
		if( index == -1 )
		{
			listIndex = mItems.size();
			// If segmented list, add the item always before the footer,
			// before the last position.
			if ( (mSectionType == IX_WIDGET.MAW_LIST_VIEW_SECTION_TYPE_SEGMENTED && hasFooter())||
					(mSectionType == IX_WIDGET.MAW_LIST_VIEW_SECTION_TYPE_ALPHABETICAL
					&& hasFooter()) )
			{
				listIndex --;
			}
			else if( mSectionType == LIST_VIEW_SECTION_TYPE_NONE_SELECTED )
			{
				Log.e( "MoSync",
				"maWidgetInsertChild: ListViewSection has no type set." );
				return IX_WIDGET.MAW_RES_INVALID_HANDLE;
			}
		}
		child.setParent(this);
		m_children.add( listIndex, child );
		mItems.add(listIndex, (ListItemWidget)child);

		// Notify parent Segmented list of new item.
		if (mAdapterListener != null)
			mAdapterListener.itemAdded((ListItemWidget)child, this, listIndex);

		return IX_WIDGET.MAW_RES_OK;
	}

	@Override
	public int removeChild(Widget child)
	{
		child.setParent(null);
		m_children.remove(child);
		mItems.remove((ListItemWidget) child);

		// Notify Segmented list's adapter.
		if (mAdapterListener != null)
			mAdapterListener.itemRemoved((ListItemWidget)child);

		return IX_WIDGET.MAW_RES_OK;
	}

	@Override
	public boolean setProperty(String property, String value)
			throws PropertyConversionException, InvalidPropertyValueException
	{
		if ( property.equals( IX_WIDGET.MAW_LIST_VIEW_SECTION_TYPE ) )
		{

			int sectionType = IntConverter.convert(value);
			switch(sectionType)
			{
			case IX_WIDGET.MAW_LIST_VIEW_SECTION_TYPE_SEGMENTED:
				createSegmentedSectionDefaultUI();
				break;
			case IX_WIDGET.MAW_LIST_VIEW_SECTION_TYPE_ALPHABETICAL:
				break;
			default:
				Log.e("@@MoSync","maWidgetSetProperty invalid List View Section type");
				throw new InvalidPropertyValueException(property, value);
			}
			mSectionType = sectionType;
		}
		else if( property.equals( IX_WIDGET.MAW_LIST_VIEW_SECTION_TITLE) )
		{
			if ( mSectionType != IX_WIDGET.MAW_LIST_VIEW_SECTION_TYPE_ALPHABETICAL )
			{
				Log.e("@@MoSync",
						"maWidgetSetProperty section preview is available only on Alphabetical list type");
				throw new InvalidPropertyValueException(property, value);
			}
			mAlphabeticIndex = value;
			// Reload the preview letters on the parent list view.
			if ( getParent() != null )
			{
				ListLayout parent = (ListLayout) getParent();
				parent.setProperty(IX_WIDGET.MAW_LIST_VIEW_RELOAD_DATA, "");
			}
		}
		else if( property.equals( IX_WIDGET.MAW_LIST_VIEW_SECTION_HEADER )
				&& (mSectionType == IX_WIDGET.MAW_LIST_VIEW_SECTION_TYPE_ALPHABETICAL) )
		{
			setAlphaSectionHeader(value);
		}
		else if( property.equals( IX_WIDGET.MAW_LIST_VIEW_SECTION_FOOTER )
				&& (mSectionType == IX_WIDGET.MAW_LIST_VIEW_SECTION_TYPE_ALPHABETICAL) )
		{
			setAlphaSectionFooter(value);
		}
		else if ( mSectionType == IX_WIDGET.MAW_LIST_VIEW_SECTION_TYPE_SEGMENTED )
		{
			if( property.equals( IX_WIDGET.MAW_LIST_VIEW_SECTION_HEADER ) )
			{
				// Just update header text. The header row is created by default
				// when the section is created.
				setHeaderText(value);
			}
			else if( property.equals( IX_WIDGET.MAW_LIST_VIEW_SECTION_FOOTER ) )
			{
				setFooterText(value);
			}
			else if( property.equals( IX_WIDGET.MAW_LIST_VIEW_SECTION_HEADER_BACKGROUND) )
			{
				mItems.get(0).setProperty(IX_WIDGET.MAW_WIDGET_BACKGROUND_COLOR, value);
			}
			else if( property.equals( IX_WIDGET.MAW_LIST_VIEW_SECTION_FOOTER_BACKGROUND) )
			{
				mItems.get(mItems.size()-1).setProperty(IX_WIDGET.MAW_WIDGET_BACKGROUND_COLOR, value);
			}
			else if( property.equals( IX_WIDGET.MAW_LIST_VIEW_SECTION_HEADER_FONT_COLOR) )
			{
				mItems.get(0).setProperty(IX_WIDGET.MAW_LIST_VIEW_ITEM_FONT_COLOR, value);
			}
			else if( property.equals( IX_WIDGET.MAW_LIST_VIEW_SECTION_FOOTER_FONT_COLOR) )
			{
				mItems.get(mItems.size()-1).setProperty(IX_WIDGET.MAW_LIST_VIEW_ITEM_FONT_COLOR, value);
			}
			else if( property.equals( IX_WIDGET.MAW_LIST_VIEW_SECTION_HEADER_FONT_SIZE) )
			{
				mItems.get(0).setProperty(IX_WIDGET.MAW_LIST_VIEW_ITEM_FONT_SIZE, value);
			}
			else if( property.equals( IX_WIDGET.MAW_LIST_VIEW_SECTION_FOOTER_FONT_SIZE) )
			{
				mItems.get(mItems.size()-1).setProperty(IX_WIDGET.MAW_LIST_VIEW_ITEM_FONT_SIZE, value);
			}
			else if( property.equals( IX_WIDGET.MAW_LIST_VIEW_SECTION_HEADER_FONT_HANDLE) ||
					property.equals(IX_WIDGET.MAW_LIST_VIEW_SECTION_FOOTER_FONT_HANDLE) )
			{
				MoSyncFontHandle currentFont = null;

				// Search the handle in the list of fonts.
				currentFont = MoSyncThread.getMoSyncFont(IntConverter .convert(value));

				if ( currentFont != null )
				{
					if ( property.equals(IX_WIDGET.MAW_LIST_VIEW_SECTION_HEADER_FONT_HANDLE) )
						mItems.get(0).
							setFontTypeface( currentFont.getTypeface(), currentFont.getFontSize() );
					else
						mItems.get(mItems.size()-1).
							setFontTypeface( currentFont.getTypeface(), currentFont.getFontSize() );
				}
				else
					throw new InvalidPropertyValueException(property, value);
			}
			else if( property.equals(IX_WIDGET.MAW_LIST_VIEW_SECTION_HEADER_HORIZONTAL_ALIGNMENT) )
			{
				mItems.get(0).setProperty(Types.WIDGET_PROPERTY_HORIZONTAL_ALIGNMENT , value);
			}
			else if( property.equals(IX_WIDGET.MAW_LIST_VIEW_SECTION_FOOTER_HORIZONTAL_ALIGNMENT) )
			{
				mItems.get(mItems.size()-1).setProperty(Types.WIDGET_PROPERTY_HORIZONTAL_ALIGNMENT , value);
			}
			else if( property.equals(IX_WIDGET.MAW_LIST_VIEW_SECTION_HEADER_VERTICAL_ALIGNMENT) )
			{
				mItems.get(0).setProperty(Types.WIDGET_PROPERTY_VERTICAL_ALIGNMENT, value);
			}
			else if( property.equals(IX_WIDGET.MAW_LIST_VIEW_SECTION_FOOTER_VERTICAL_ALIGNMENT) )
			{
				mItems.get(mItems.size()-1).setProperty(Types.WIDGET_PROPERTY_VERTICAL_ALIGNMENT , value);
			}
		}
		else
		{
			// No widget/layout properties are inherited.
			return false;
		}

		return true;
	}

	/**
	 * @see Widget.getProperty.
	 */
	@Override
	public String getProperty(String property)
	{
		if( property.equals( IX_WIDGET.MAW_LIST_VIEW_SECTION_TITLE )
				&& mSectionType == IX_WIDGET.MAW_LIST_VIEW_SECTION_TYPE_ALPHABETICAL)
		{
			return mAlphabeticIndex;
		}
		else if( property.equals( IX_WIDGET.MAW_LIST_VIEW_SECTION_HEADER )
				&& mSectionType != LIST_VIEW_SECTION_TYPE_NONE_SELECTED)
		{
			if (!mItems.isEmpty())
				return mItems.get(0).getProperty(IX_WIDGET.MAW_LIST_VIEW_ITEM_TEXT);
		}
		else if( property.equals( IX_WIDGET.MAW_LIST_VIEW_SECTION_FOOTER )
				&& mSectionType != LIST_VIEW_SECTION_TYPE_NONE_SELECTED)
		{
			if (!mItems.isEmpty())
				return mItems.get(mItems.size()).getProperty(IX_WIDGET.MAW_LIST_VIEW_ITEM_TEXT);
		}
		else if ( property.equals( IX_WIDGET.MAW_LIST_VIEW_SECTION_TYPE ) )
		{
			return String.valueOf(mSectionType);
		}
		return "";
	}

	/**
	 * Sets a listener that is called when the items of the section
	 * are changed.
	 *
	 * @param adapterListener A listener for item changes.
	 */
	public void setAdapterListener(AdapterChangedListener adapterListener)
	{
		mAdapterListener = adapterListener;
	}

	/**
	 * Create a header or a footer widget.
	 * @param itemType header or footer type:
	 *  - ITEM_VIEW_TYPE_HEADER or
	 *  - ITEM_VIEW_TYPE_FOOTER
	 * @return The list item widget.
	 */
	public ListItemWidget createSectionItem(int itemType)
	{
		// We don't need to later reffer to those objects using the widget handles.
		Widget rowWidget = ViewFactory.createView(
				IX_WIDGET.MAW_LIST_VIEW_ITEM,
				mAppActivity,
				MoSyncThread.getInstance().nativeCreatePlaceholder());
		ListItemWidget itemWidget = (ListItemWidget) rowWidget;
		itemWidget.alignLabelHorizontally(IX_WIDGET.MAW_ALIGNMENT_CENTER);

		if ( itemType == ITEM_VIEW_TYPE_HEADER )
		{
			rowWidget.setProperty(
					IX_WIDGET.MAW_WIDGET_BACKGROUND_COLOR,
					HEADER_DEFAULT_BACKGROUND_COLOR);
			itemWidget.setProperty(
					IX_WIDGET.MAW_LIST_VIEW_ITEM_FONT_SIZE,
					Integer.toString(HEADER_DEFAULT_FONT_SIZE));
			itemWidget.alignLabelHorizontally(
					IX_WIDGET.MAW_ALIGNMENT_LEFT);
			itemWidget.setItemType(ITEM_VIEW_TYPE_HEADER);
		}
		else if ( itemType == ITEM_VIEW_TYPE_FOOTER)
		{
			rowWidget.setProperty(
					IX_WIDGET.MAW_WIDGET_BACKGROUND_COLOR,
					FOOTER_DEFAULT_BACKGROUND_COLOR);
			itemWidget.setProperty(
					IX_WIDGET.MAW_LIST_VIEW_ITEM_FONT_SIZE,
					Integer.toString(FOOTER_DEFAULT_FONT_SIZE));
			itemWidget.alignLabelHorizontally(
					IX_WIDGET.MAW_ALIGNMENT_LEFT);
			itemWidget.setItemType(ITEM_VIEW_TYPE_FOOTER);
		}
		else
		{
			return null;
		}
		return itemWidget;
	}

	/**
	 * Add by default a header and a footer layout to a
	 * segmented section.
	 * Set different background colors and align labels to center.
	 */
	public void createSegmentedSectionDefaultUI()
	{
		addSectionHeader();
		addSectionFooter();
	}

	/**
	 * Add by default a header layout to a section.
	 * Set different background colors and align labels to center.
	 */
	public void addSectionHeader()
	{
		ListItemWidget headerItem = createSectionItem(ITEM_VIEW_TYPE_HEADER);
		// Add the header to the children widget list.
		mItems.add(0, headerItem);
		m_children.add( 0, headerItem );
	}

	/**
	 * Add by default a footer layout to a section.
	 * Set different background colors and align labels to center.
	 */
	public void addSectionFooter()
	{
		ListItemWidget footerItem = createSectionItem(ITEM_VIEW_TYPE_FOOTER);
		// Add the footer to the children widget list.
		mItems.add(mItems.size(), footerItem);
		m_children.add( m_children.size(), footerItem );
	}

	/**
	 * Check if this section contains a header list item.
	 * @return
	 */
	public boolean hasHeader()
	{
		if ( !mItems.isEmpty()
				&& mItems.get(0) != null
				&& (mItems.get(0).getItemType() == ITEM_VIEW_TYPE_HEADER) )
		{
			return true;
		}
		return false;
	}

	/**
	 * Check if this section contains a footer list item.
	 * @return
	 */
	public boolean hasFooter()
	{
		return mHasFooter;
	}

	/**
	 * Set the header for an alphabetical section.
	 * Create a header row if one does not exist,
	 * or update it.
	 * @param text
	 */
	public void setAlphaSectionHeader(String text)
	{
		// Check if header already exists.
		if ( hasHeader() )
		{
			if ( !text.isEmpty() )
			{
				// Just update the header text.
				setHeaderText(text);
			}
			else
			{
				// If the new value is null, remove the header row.
				m_children.remove(0);
				mItems.remove(0);

				// Notify list adapter.
				if (mAdapterListener != null)
					mAdapterListener.itemRemoved(mItems.get(0));
			}
		}
		else
		{
			// Create and add the header before the first item - if that exists.
			addSectionHeader();
			setHeaderText(text);
			if (mAdapterListener != null)
				mAdapterListener.itemAdded(mItems.get(0), this, 0);
		}
	}

	/**
	 * Set the header for an alphabetical section.
	 * Create a header row if one does not exist,
	 * or update it.
	 * @param text
	 */
	public void setAlphaSectionFooter(String text)
	{
		// Check if header already exists.
		if ( hasFooter() )
		{
			if ( !text.isEmpty() )
			{
				// Just update the footer text.
				setFooterText(text);
			}
			else
			{
				// If the new value is null, remove the footer row.
				m_children.remove(mItems.get(mItems.size()-1));
				mItems.remove(mItems.get(mItems.size()-1));

				// Notify list adapter.
				if (mAdapterListener != null)
					mAdapterListener.itemRemoved(mItems.get(mItems.size()-1));
			}
		}
		else
		{
			// Create and add the footer after the last item - if that exists.
			addSectionFooter();
			setFooterText(text);
			if (mAdapterListener != null)
				mAdapterListener.itemAdded(mItems.get(mItems.size()), this, mItems.size());
		}
	}

	/**
	 * Remove all items from this section.
	 */
	public void emptySection()
	{
		mItems.clear();
		m_children.clear();
	}

	/**
	 * Get items count,including header and footer.
	 * @return The section item count.
	 */
	public int itemsCount()
	{
		return mItems.size();
	}

	/**
	 * Set the header label text.
	 * @param text
	 */
	public void setHeaderText(final String text)
	{
		mItems.get(0).setProperty(
				IX_WIDGET.MAW_LIST_VIEW_ITEM_TEXT, text);
	}

	/**
	 * Set the footer label text to a segmented section.
	 * The footer is added by default, so if the text
	 * is an empty string, remove the footer.
	 * @param text
	 */
	public void setFooterText(final String text)
	{
		if ( text.isEmpty() )
		{
			// Remove the footer row.
			mHasFooter = false;
			m_children.remove(mItems.get(mItems.size()-1));

			// Notify list adapter.
			if (mAdapterListener != null)
				mAdapterListener.itemRemoved(mItems.get(mItems.size()-1));
			mItems.remove(mItems.get(mItems.size()-1));
		}
		else
		{
			// Add a footer row if it was previously removed.
			if (!mHasFooter)
			{
				mHasFooter = true;
				addSectionFooter();
				// Notify list adapter.
				if (mAdapterListener != null)
					mAdapterListener.itemAdded(
							mItems.get(mItems.size()-1), this, mItems.size()-1);
			}

			mItems.get(mItems.size()-1).setProperty(
					IX_WIDGET.MAW_LIST_VIEW_ITEM_TEXT, text);
		}
	}

	/**
	 * Get item inside section.
	 * @param position The item position.
	 * @return The ListItemWidget object.
	 */
	public ListItemWidget getItem(int position)
	{
		if ( position > mItems.size() )
			return null;
		return mItems.get(position);
	}

	/**
	 * A listener class for when the items of the section change,
	 * so that the adapter of the list can be updated.
	 *
	 * emma
	 */
	public interface AdapterChangedListener
	{
		/**
		 * Called when an item from the section has removed.
		 *
		 * @param item The item that was removed from the section.
		 */
		void itemRemoved(ListItemWidget item);

		/**
		 * Called when an item was added to the section.
		 *
		 * @param item The new item.
		 * @param section The section.
		 * @param index The item index in section.
		 */
		void itemAdded(ListItemWidget item, ListViewSection section, int index);
	}
}