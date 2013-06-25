/* Copyright (C) 2011 MoSync AB

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

import android.graphics.drawable.Drawable;
import android.view.MenuItem;

/**
 * This class is not a widget, it's just a container for menu items.
 * It holds information like: title and icon.
 * Starting with Honeycomb, the ActionBar replaces the options menu,
 * as newest devices do not have the options hardware button.
 *
 * @author emma
 */
public class OptionsMenuItem
{
	/**
	 * Title of this menu item.
	 */
	private String m_title = "";

	/**
	 * Custom icon of this menu item.
	 */
	private Drawable m_customIcon = null;

	/**
	 * The id of the predefined icon for this menu item.
	 */
	private int m_predefinedIconID = -1;

	/**
	 * The item id.
	 * Internal placeholder for ActionBar menu items.
	 * Index in menu for options menu items.
	 */
	private int m_itemId = -1;

	/**
	 * Sets how an item should display in the presence of an Action Bar.
	 */
	private int m_displayFlag = MenuItem.SHOW_AS_ACTION_IF_ROOM;

	/**
	 * Constructor
	 *
	 * @param handle Integer handle corresponding to this instance.
	 * @param title The text that will be displayed on the menu item.
	 * @param icon The icon that will be displayed on the menu item.
	 */
	public OptionsMenuItem(int handle, String title, Drawable icon ){
		m_itemId = handle;
		m_title = title;
		if ( icon != null )
		{
			m_customIcon = icon;
		}
	}

	/**
	 * Constructor.
	 *
	 * @param handle Integer handle corresponding to this instance.
	 * @param title The text that will be displayed on the menu item.
	 * @param predefinedIconId The resource id of the predefined icon
	 * that will be displayed on this menu item. One of the
	 * MAW_OPTIONS_MENU_ICON_CONSTANT_ constants.
	 */
	public OptionsMenuItem(int handle, String title, int predefinedIconId){
		m_itemId = handle;
		m_title = title;
		m_predefinedIconID = predefinedIconId;
	}

	public void setShowAsAction(int flag)
	{
		m_displayFlag = flag;
	}

	public int getShowActionFlag()
	{
		return m_displayFlag;
	}

	public Boolean hasCustomIcon()
	{
		return (m_customIcon != null ? true : false);
	}

	public Boolean hasPredefinedIcon()
	{
		return (m_predefinedIconID != -1 ? true : false );
	}

	public String getTitle()
	{
		return m_title;
	}

	public int getId()
	{
		return m_itemId;
	}

	public Drawable getCustomIcon()
	{
		return m_customIcon;
	}

	public int getPredefinedIconID()
	{
		return m_predefinedIconID;
	}
}