/*
Copyright (C) 2011 MoSync AB

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

/**
 * @file ActionBar.cpp
 * @author Emma Tresanszki
 *
 * \brief The ActionBar singleton class is used for showing
 * and customizing the appearance of the action bar.
 * The Action bar is unique per application, and it will not
 * be treated as a regular widget.
 *
 * NOTE: Available on Android only.
 */

#include "ActionBar.h"

namespace NativeUI
{
	/**
	 * Initialize the singleton variable to NULL.
	 */
	ActionBar* ActionBar::sInstance = NULL;

	/**
	 * Constructor is protected since this is a singleton.
	 * (subclasses can still create instances).
	 */
	ActionBar::ActionBar()
	{
	}

	/**
	 * Destructor.
	 */
	ActionBar::~ActionBar()
	{
	}

	/**
	 * Return the single instance of this class.
	 */
	ActionBar* ActionBar::getInstance()
	{
		if (NULL == ActionBar::sInstance)
		{
			ActionBar::sInstance = new ActionBar();
		}

		return sInstance;
	}

	/**
	 * Destroy the single instance of this class.
	 * Call this method only when the application will exit.
	 */
	void ActionBar::destroyInstance()
	{
		delete ActionBar::sInstance;
	}

	bool ActionBar::setEnabled(bool enableActionBar)
	{
		if ( maActionBarSetEnabled(enableActionBar ? 1 : 0) == MAW_RES_OK )
			return true;
		return false;
	}

	void ActionBar::refresh()
	{
		maActionBarRefresh();
	}

	void ActionBar::show(bool showActionBar)
	{
		maActionBarSetVisibility(showActionBar ? 1 : 0);
	}

	int ActionBar::getHeight()
	{
		return maActionBarGetHeight();
	}

	bool ActionBar::isShowing()
	{
		return ( maActionBarIsShowing() == 1 ? true : false);
	}

	void ActionBar::enableTitle(bool enable)
	{
		maActionBarShowTitleEnabled(enable ? 1 : 0);
	}

	void ActionBar::setTitle(const MAUtil::String title)
	{
		maActionBarSetTitle(title.c_str());
	}

	void ActionBar::enableLogo(bool enable)
	{
		maActionBarShowLogoEnabled(enable ? 1 : 0);
	}

	void ActionBar::setIcon(const MAHandle handle)
	{
		maActionBarSetIcon(handle);
	}

	void ActionBar::enableHomeButton(bool enable)
	{
		maActionBarSetHomeButtonEnabled(enable ? 1 : 0);
	}

	void ActionBar::enableHomeButtonAsUp(bool enable)
	{
		maActionBarSetDisplayHomeAsUpEnabled(enable ? 1 : 0);
	}

	void ActionBar::setBackgroundImage(const MAHandle image)
	{
		maActionBarSetBackgroundImage(image);
	}
}
