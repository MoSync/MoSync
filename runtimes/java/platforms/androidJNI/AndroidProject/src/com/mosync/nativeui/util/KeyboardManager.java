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
