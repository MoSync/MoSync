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

package com.mosync.nativeui.util;

import android.content.Context;
import android.view.View;
import android.view.inputmethod.InputMethodManager;

/**
 * Utility class for showing and hiding the virtual
 * keyboard for specific views.
 * 
 * @author fmattias
 */
public class KeyboardManager
{
	/**
	 * Shows the keyboard and binds it to the given view. If
	 * the view is not assigned to a window, the keyboard will
	 * not be shown.
	 * 
	 * @param view The view to show the keyboard for.
	 * 
	 * @return true if the keyboard could be shown, false otherwise.
	 */
	public static boolean showKeyboardFor(View view)
	{
		InputMethodManager manager = (InputMethodManager)
		view.getContext( ).getSystemService( Context.INPUT_METHOD_SERVICE );
		if( null == manager )
		{
			return false;
		}
		
		view.requestFocus( );
		manager.showSoftInput( view, InputMethodManager.SHOW_FORCED );
		
		return true;
	}
	
	/**
	 * Hides the keyboard from the given view. If the view is
	 * not assigned to a window, this request is ignored.
	 * 
	 * @param view The view to hide the keyboard from.
	 * 
	 * @return true if the keyboard could be hidden, false
	 * otherwise.
	 */
	public static boolean hideKeyboardFor(View view)
	{
		InputMethodManager manager = (InputMethodManager)
		view.getContext( ).getSystemService( Context.INPUT_METHOD_SERVICE );
		if( null == manager )
		{
			return false;
		}
		
		manager.hideSoftInputFromWindow( view.getWindowToken( ), 0 );
		
		return true;
	}	
}
