/* Copyright 2013 David Axmark

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

	http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

// /**
//  * @file ActionBar.cpp
//  * @author Emma Tresanszki
//  *
//  * \brief The ActionBar singleton class is used for showing
//  * and customizing the appearance of the action bar.
//  * The Action bar is unique per application, and it will not
//  * be treated as a regular widget.
//  *
//  * NOTE: Available on Android only.
//  */

// #include "ActionBar.h"

// namespace NativeUI
// {
// 	/**
// 	 * Initialize the singleton variable to NULL.
// 	 */
// 	ActionBar* ActionBar::sInstance = NULL;

// 	/**
// 	 * Constructor is protected since this is a singleton.
// 	 * (subclasses can still create instances).
// 	 */
// 	ActionBar::ActionBar()
// 	{
// 	}

// 	/**
// 	 * Destructor.
// 	 */
// 	ActionBar::~ActionBar()
// 	{
// 	}

// 	/**
// 	 * Return the single instance of this class.
// 	 */
// 	ActionBar* ActionBar::getInstance()
// 	{
// 		if (NULL == ActionBar::sInstance)
// 		{
// 			ActionBar::sInstance = new ActionBar();
// 		}

// 		return sInstance;
// 	}

// 	/**
// 	 * Destroy the single instance of this class.
// 	 * Call this method only when the application will exit.
// 	 */
// 	void ActionBar::destroyInstance()
// 	{
// 		delete ActionBar::sInstance;
// 	}

// 	bool ActionBar::setEnabled(bool enableActionBar)
// 	{
// 		if ( maActionBarSetEnabled(enableActionBar ? 1 : 0) == MAW_RES_OK )
// 			return true;
// 		return false;
// 	}

// 	void ActionBar::refresh()
// 	{
// 		maActionBarRefresh();
// 	}

// 	void ActionBar::show(bool showActionBar)
// 	{
// 		maActionBarSetVisibility(showActionBar ? 1 : 0);
// 	}

// 	int ActionBar::getHeight()
// 	{
// 		return maActionBarGetHeight();
// 	}

// 	bool ActionBar::isShowing()
// 	{
// 		return ( maActionBarIsShowing() == 1 ? true : false);
// 	}

// 	void ActionBar::enableTitle(bool enable)
// 	{
// 		maActionBarShowTitleEnabled(enable ? 1 : 0);
// 	}

// 	void ActionBar::setTitle(const MAUtil::String title)
// 	{
// 		maActionBarSetTitle(title.c_str());
// 	}

// 	void ActionBar::enableLogo(bool enable)
// 	{
// 		maActionBarShowLogoEnabled(enable ? 1 : 0);
// 	}

// 	void ActionBar::setIcon(const MAHandle handle)
// 	{
// 		maActionBarSetIcon(handle);
// 	}

// 	void ActionBar::enableHomeButton(bool enable)
// 	{
// 		maActionBarSetHomeButtonEnabled(enable ? 1 : 0);
// 	}

// 	void ActionBar::enableHomeButtonAsUp(bool enable)
// 	{
// 		maActionBarSetDisplayHomeAsUpEnabled(enable ? 1 : 0);
// 	}

// 	void ActionBar::setBackgroundImage(const MAHandle image)
// 	{
// 		maActionBarSetBackgroundImage(image);
// 	}
// }
