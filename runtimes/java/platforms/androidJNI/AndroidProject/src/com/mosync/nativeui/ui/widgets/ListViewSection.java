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

import java.util.ArrayList;

import android.app.Activity;
import android.util.Log;
import android.widget.ListView;

import com.mosync.internal.android.MoSyncThread;
import com.mosync.internal.android.MoSyncFont.MoSyncFontHandle;
import com.mosync.internal.generated.IX_WIDGET;
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
	static final int LIST_VIEW_SECTION_TYPE_ALPHABETICAL = 1;
	static final int LIST_VIEW_SECTION_TYPE_SEGMENTED = 2;
	static final int LIST_VIEW_SECTION_TYPE_NONE_SELECTED = 3;
	/**
	 * Section type: segmented or alphabetical.
	 */
	private int mSectionType = LIST_VIEW_SECTION_TYPE_NONE_SELECTED;

	/**
	 * Default header and footer appearance for segmented list views.
	 */
	static final String HEADER_DEFAULT_BACKGROUND_COLOR = "838B83";
	static final String FOOTER_DEFAULT_BACKGROUND_COLOR = "838B83";
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
			if ( mSectionType == LIST_VIEW_SECTION_TYPE_SEGMENTED )
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

		// Always add items before the footer.
		child.setParent(this);
		m_children.add( listIndex, child );
		Log.e("@@MoSync","Section add child at " + listIndex + " in mItems array -------");
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
		if ( property.equals("TYPE") )
		{
			if ( value.equals("segmented") )
			{
				createSegmentedSectionDefaultUI();
				mSectionType = LIST_VIEW_SECTION_TYPE_SEGMENTED;
			}
			else if( value.equals("alphabetical") )
			{
				mSectionType = LIST_VIEW_SECTION_TYPE_ALPHABETICAL;
			}
			else
			{
				Log.e("@@MoSync","maWidgetSetProperty invalid List View Section type");
				throw new InvalidPropertyValueException(property, value);
			}
		}
		else if( property.equals( IX_WIDGET.MAW_LIST_VIEW_SECTION_TITLE) )
		{
			if ( mSectionType != LIST_VIEW_SECTION_TYPE_ALPHABETICAL )
			{
				Log.e("@@MoSync",
						"maWidgetSetProperty section preview is available only on Alphabetical list type");
				throw new InvalidPropertyValueException(property, value);
			}
			mAlphabeticIndex = value.substring(0,1).toUpperCase();
		}
		else if( property.equals( IX_WIDGET.MAW_LIST_VIEW_SECTION_HEADER ) )
		{
			if ( mSectionType != LIST_VIEW_SECTION_TYPE_SEGMENTED )
			{
				Log.e("@@MoSync",
				"maWidgetSetProperty section header is available only on Segmented list type");
				throw new InvalidPropertyValueException(property, value);
			}
			setHeaderText(value);
		}
		else if( property.equals( IX_WIDGET.MAW_LIST_VIEW_SECTION_FOOTER ) )
		{
			if ( mSectionType != LIST_VIEW_SECTION_TYPE_SEGMENTED )
			{
				Log.e("@@MoSync",
				"maWidgetSetProperty section header is available only on Segmented list type");
				throw new InvalidPropertyValueException(property, value);
			}
			setFooterText(value);
		}
		else if( property.equals( "headerBackground") )
		{
			if ( mSectionType != LIST_VIEW_SECTION_TYPE_SEGMENTED )
			{
				Log.e("@@MoSync",
				"maWidgetSetProperty section header is available only on Segmented list type");
				throw new InvalidPropertyValueException(property, value);
			}
			mItems.get(0).setProperty(IX_WIDGET.MAW_WIDGET_BACKGROUND_COLOR, value);
		}
		else if( property.equals( "footerBackgrouond") )
		{
			if ( mSectionType != LIST_VIEW_SECTION_TYPE_SEGMENTED )
			{
				Log.e("@@MoSync",
				"maWidgetSetProperty section header is available only on Segmented list type");
				throw new InvalidPropertyValueException(property, value);
			}
			mItems.get( mItems.size()-1).setProperty(IX_WIDGET.MAW_WIDGET_BACKGROUND_COLOR, value);
		}
		else if( property.equals( "headerFontColor") )
		{
			if ( mSectionType != LIST_VIEW_SECTION_TYPE_SEGMENTED )
			{
				Log.e("@@MoSync",
				"maWidgetSetProperty section header is available only on Segmented list type");
				throw new InvalidPropertyValueException(property, value);
			}
			mItems.get(0).setProperty(IX_WIDGET.MAW_LIST_VIEW_ITEM_FONT_COLOR, value);
		}
		else if( property.equals( "headerFontSize") )
		{
			if ( mSectionType != LIST_VIEW_SECTION_TYPE_SEGMENTED )
			{
				Log.e("@@MoSync",
				"maWidgetSetProperty section header is available only on Segmented list type");
				throw new InvalidPropertyValueException(property, value);
			}
			mItems.get(0).setProperty(IX_WIDGET.MAW_LIST_VIEW_ITEM_FONT_SIZE, value);
		}
		else if( property.equals( "headerFont") )
		{
			if ( mSectionType != LIST_VIEW_SECTION_TYPE_SEGMENTED )
			{
				Log.e("@@MoSync",
				"maWidgetSetProperty section header is available only on Segmented list type");
				throw new InvalidPropertyValueException(property, value);
			}
			MoSyncFontHandle currentFont = null;

			// Search the handle in the list of fonts.
			currentFont = MoSyncThread.getMoSyncFont(IntConverter .convert(value));

			if ( currentFont != null )
			{
				mItems.get(0).setFontTypeface( currentFont.getTypeface(), currentFont.getFontSize() );
			}
			else
				throw new InvalidPropertyValueException(property, value);
		}
		else if( property.equals("headerAlignment") )
		{
			if ( mSectionType != LIST_VIEW_SECTION_TYPE_SEGMENTED )
			{
				Log.e("@@MoSync",
				"maWidgetSetProperty section header is available only on Segmented list type");
				throw new InvalidPropertyValueException(property, value);
			}
			mItems.get(0).setProperty("horizontalAlignment", value);
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
		if( property.equals( IX_WIDGET.MAW_LIST_VIEW_SECTION_TITLE ) )
		{
			return mAlphabeticIndex;
		}
		else if( property.equals( IX_WIDGET.MAW_LIST_VIEW_SECTION_HEADER ) )
		{
			return mItems.get(0).getProperty(IX_WIDGET.MAW_LIST_VIEW_ITEM_TEXT);
		}
		else if( property.equals( IX_WIDGET.MAW_LIST_VIEW_SECTION_FOOTER ) )
		{
			return mItems.get(mItems.size()).getProperty(IX_WIDGET.MAW_LIST_VIEW_ITEM_TEXT);
		}
		else
		{
			return "";
		}
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
	 * Add by default a header and a footer layout to a
	 * segmented section.
	 * Set different background colors and align labels to center.
	 */
	public void createSegmentedSectionDefaultUI()
	{
		// We don't need to later reffer to those objects using the widget handles.
		Widget headerWidget = ViewFactory.createView(
				IX_WIDGET.MAW_LIST_VIEW_ITEM,
				mAppActivity,
				MoSyncThread.getInstance().nativeCreatePlaceholder());

		headerWidget.setProperty(
				IX_WIDGET.MAW_WIDGET_BACKGROUND_COLOR,
				HEADER_DEFAULT_BACKGROUND_COLOR);
		ListItemWidget headerItem = (ListItemWidget) headerWidget;
		headerItem.alignLabelHorizontally(IX_WIDGET.MAW_ALIGNMENT_CENTER);
		headerItem.setProperty(
				IX_WIDGET.MAW_LIST_VIEW_ITEM_FONT_SIZE,
				Integer.toString(HEADER_DEFAULT_FONT_SIZE));

		Widget footerWidget = ViewFactory.createView(
				IX_WIDGET.MAW_LIST_VIEW_ITEM,
				mAppActivity,
				MoSyncThread.getInstance().nativeCreatePlaceholder() );

		footerWidget.setProperty(
				IX_WIDGET.MAW_WIDGET_BACKGROUND_COLOR,
				FOOTER_DEFAULT_BACKGROUND_COLOR);
		ListItemWidget footerItem = (ListItemWidget) footerWidget;
		footerItem.alignLabelHorizontally(IX_WIDGET.MAW_ALIGNMENT_CENTER);
		footerItem.setProperty(
				IX_WIDGET.MAW_LIST_VIEW_ITEM_FONT_SIZE,
				Integer.toString(FOOTER_DEFAULT_FONT_SIZE));

		headerItem.setItemType(ITEM_VIEW_TYPE_HEADER);
		mItems.add(0, headerItem);
		footerItem.setItemType(ITEM_VIEW_TYPE_FOOTER);
		mItems.add(1, footerItem);

		// Add the header and footer to the children widget list.
		m_children.add( 0, headerItem );
		m_children.add( 1, footerItem );
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
	 * Set the footer label text.
	 * @param text
	 */
	public void setFooterText(final String text)
	{
		mItems.get(mItems.size()-1).setProperty(
				IX_WIDGET.MAW_LIST_VIEW_ITEM_TEXT, text);
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