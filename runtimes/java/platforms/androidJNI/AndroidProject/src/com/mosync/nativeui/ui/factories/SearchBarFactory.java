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

package com.mosync.nativeui.ui.factories;

import android.app.Activity;
import android.content.Context;
import android.os.IBinder;
import android.view.Gravity;
import android.view.KeyEvent;
import android.view.View;
import android.view.View.OnClickListener;
import android.view.View.OnKeyListener;
import android.view.inputmethod.InputMethodManager;
import android.widget.EditText;
import android.widget.ImageButton;
import android.widget.ImageView;
import android.widget.LinearLayout;

import com.mosync.internal.android.EventQueue;
import com.mosync.internal.generated.IX_WIDGET;
import com.mosync.nativeui.ui.widgets.SearchBarWidget;
import com.mosync.nativeui.ui.widgets.Widget;
import com.mosync.nativeui.util.DensityIndependentPixelConverter;

/**
 * Creates a search bar widget.
 * 
 * Note on terminology: The widget is a MoSync object 
 * and the view is an Android UI object. A widget references
 * a view. It is the view that is displayed.
 * 
 * @author Mikael
 */
public class SearchBarFactory implements AbstractViewFactory
{
	/**
	 * Called by the widget factory to create this widget.
	 * @param activity The activity.
	 * @param handle The widget handle.
	 * @return The search bar widget.
	 */
	@Override
	public Widget create(Activity activity, int handle)
	{
		return new SearchBarWidget(
			handle, 
			createSearchBarView(activity, handle));
	}
	
	/**
	 * Creates a view for the search bar widget. Contains
	 * a text edit box and a search button.
	 * @param activity The activity.
	 * @param handle The widget handle.
	 * @return The search bar view.
	 */
	private View createSearchBarView(final Activity activity, final int handle)
	{
		// Variable to hold layout parameters.
		LinearLayout.LayoutParams layoutParams;
		
		// Pixel density converter.
		DensityIndependentPixelConverter pixelConverter =
			new DensityIndependentPixelConverter(activity);

		// Pixel values.
		int horizontalMargin = pixelConverter.toDisplayPixels(5);
		int verticalMargin = pixelConverter.toDisplayPixels(3);
		int searchButtonPixelSize = pixelConverter.toDisplayPixels(55);
		
		// Create layout to hold search bar components.
		// This is the main view of the widget.
        LinearLayout searchBarLayout = new LinearLayout(activity);
        searchBarLayout.setBaselineAligned(false);
        searchBarLayout.setGravity(Gravity.CENTER_VERTICAL);
        // This was used for debugging.
        //searchBarLayout.setBackgroundColor(Color.BLUE);
        searchBarLayout.setOrientation(LinearLayout.HORIZONTAL);
        
        // Create the edit box.
        final EditText editBox = new EditText(activity);
        editBox.setMaxLines(1);
        layoutParams = new LinearLayout.LayoutParams(
    		LinearLayout.LayoutParams.FILL_PARENT,
    		LinearLayout.LayoutParams.WRAP_CONTENT, 
    		1);
        layoutParams.setMargins(
    		horizontalMargin, 
    		verticalMargin, 
    		horizontalMargin, 
    		verticalMargin);
        editBox.setLayoutParams(layoutParams);
        searchBarLayout.addView(editBox);
        
        // Create the search button.
        final ImageButton button = new ImageButton(activity);
        button.setImageResource(android.R.drawable.ic_menu_search);
        button.setScaleType(ImageView.ScaleType.CENTER_INSIDE);
        layoutParams = new LinearLayout.LayoutParams(
    		LinearLayout.LayoutParams.WRAP_CONTENT,
    		LinearLayout.LayoutParams.WRAP_CONTENT, 
    		0);
        layoutParams.setMargins(
        	0, 
        	verticalMargin, 
        	horizontalMargin, 
        	verticalMargin);
        button.setLayoutParams(layoutParams);
        button.setAdjustViewBounds(true);
        button.setMaxWidth(searchButtonPixelSize);
        button.setMaxHeight(searchButtonPixelSize);
        button.setOnClickListener(new OnClickListener()
        {
    		public void onClick(View v)
    		{
    			// Hide on-screen keyboard.
    			InputMethodManager manager = (InputMethodManager)
    				activity.getSystemService(Context.INPUT_METHOD_SERVICE);
    			IBinder windowToken = editBox.getWindowToken();
    			if (null != manager && null != windowToken)
    			{
    				manager.hideSoftInputFromWindow(
    					editBox.getWindowToken(), 
    					0);
    			}
    			  
    			// Post event to the MoSYnc event queue.
    			EventQueue eventQueue = EventQueue.getDefault( );
    			eventQueue.postWidgetEvent(IX_WIDGET.MAW_EVENT_CLICKED, handle);
    		}
        });
        searchBarLayout.addView(button);
        
        editBox.setOnKeyListener( new OnKeyListener( ) {
			@Override
			public boolean onKey(View v, int keyCode, KeyEvent event)
			{
				if( (event.getAction( ) == KeyEvent.ACTION_DOWN)
						&& (keyCode == KeyEvent.KEYCODE_ENTER) )
				{
					button.performClick( );
					return true;
				}
				else
				{
					return false;
				}
			}
		});
        
		return searchBarLayout;
	}
}
