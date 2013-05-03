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
	 * Icon of this menu item.
	 */
	private Drawable m_icon = null;

	/**
	 * If true, the icon is taken from resources.
	 * If false it is a user defined icon.
	 */
	private Boolean m_iconResource = false;

	/**
	 * The icon resource id.
	 */
	private int m_iconResID = -1;

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
	 * @param handle handle Integer handle corresponding to this instance.
	 * @param view A screen wrapped by this widget.
	 */
	public OptionsMenuItem(int handle, String title, Drawable icon ){
		m_itemId = handle;
		m_title = title;
		m_icon = icon;
	}

	/**
	 * Constructor.
	 *
	 * @param handle handle Integer handle corresponding to this instance.
	 * @param iconRes The resource id of the icon. One of the
	 * MAW_OPTIONS_MENU_ICON_CONSTANT_ constants.
	 * @param view A screen wrapped by this widget.
	 */
	public OptionsMenuItem(int handle, String title, int iconRes){
		m_itemId = handle;
		m_title = title;
		m_iconResource = true;
		m_iconResID = iconRes;
	}

	public void setShowAsAction(int flag)
	{
		m_displayFlag = flag;
	}

	public int getShowActionFlag()
	{
		return m_displayFlag;
	}

	public Boolean hasIconFromResources()
	{
		return m_iconResource;
	}

	public Boolean hasIconPredefined()
	{
		return (m_iconResID != -1 ? true : false );
	}

	public String getTitle()
	{
		return m_title;
	}

	public int getId()
	{
		return m_itemId;
	}

	public Drawable getIcon()
	{
		return m_icon;
	}

	public int getIconResId()
	{
		return m_iconResID;
	}
}