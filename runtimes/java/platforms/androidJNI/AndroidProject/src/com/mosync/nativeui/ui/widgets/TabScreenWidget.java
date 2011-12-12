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

import java.util.HashMap;
import java.util.Map.Entry;

import android.graphics.drawable.Drawable;
import android.util.Log;
import android.view.View;
import android.widget.ImageView;
import android.widget.TabHost;
import android.widget.TabHost.TabContentFactory;
import android.widget.TabHost.TabSpec;
import android.widget.TextView;

import com.mosync.internal.generated.IX_WIDGET;
import com.mosync.nativeui.util.properties.IntConverter;
import com.mosync.nativeui.util.properties.InvalidPropertyValueException;
import com.mosync.nativeui.util.properties.PropertyConversionException;

/**
 * A screen that allows navigation between screens using
 * tabs.
 *
 * @author fmattias
 */
public class TabScreenWidget extends ScreenWidget
	implements ScreenWidget.TitleChangedListener
{
	/**
	 * Map from a screen widget to its tab index in this
	 * screen.
	 */
	private HashMap<ScreenWidget, Integer> m_tabIndexToScreen = new HashMap<ScreenWidget, Integer>( );

	/**
	 * Constructor
	 *
	 * @param handle handle Integer handle corresponding to this instance.
	 * @param view A TabHost wrapped by this widget.
	 */
	public TabScreenWidget(int handle, TabHost tabHost)
	{
		super( handle, tabHost );
	}

	@Override
	public void addChildAt(Widget child, int index)
	{
		// We cannot support adding tabs anywhere right now,
		// since the Android api does not support it.
		if( index != -1 )
		{
			Log.w( "MoSync", "Adding tab to a specific index other than last is" +
					" currently not supported on Android." );
		}

		if( !( child instanceof ScreenWidget ) )
		{
			return;
		}
		ScreenWidget screen = (ScreenWidget) child;
		final View screenView = screen.getView( );

		TabHost tab = (TabHost) getView( );

		int indexOfNewTab = tab.getTabWidget( ).getChildCount( );
		TabSpec tabSpec = tab.newTabSpec( Integer.toString( indexOfNewTab ) );
		setIndicators( tabSpec, screen.getTitle( ), screen.getIcon( ) );

		// Provides the tab with its content.
		tabSpec.setContent( new TabContentFactory( ) {
			@Override
			public View createTabContent(String tag)
			{
				return screenView;
			}
		});

		tab.addTab( tabSpec );
		m_tabIndexToScreen.put( screen, indexOfNewTab );
		screen.setTitleChangedListener( this );
	}

	private void setIndicators(TabSpec tabSpec, String title, Drawable icon)
	{
		if( icon != null )
		{
			tabSpec.setIndicator( title, icon );
		}
		else
		{
			tabSpec.setIndicator( title );
		}
	}

	@Override
	public boolean setProperty(String property, String value)
			throws PropertyConversionException, InvalidPropertyValueException
	{
		if( super.setProperty( property, value ) )
		{
			return true;
		}

		TabHost tabHost = (TabHost) getView( );
		if( property.equals( IX_WIDGET.MAW_TAB_SCREEN_CURRENT_TAB ) )
		{
			int currentTabIndex = IntConverter.convert( value );
			tabHost.setCurrentTab( currentTabIndex );
		}

		return true;
	}

	@Override
	public String getProperty(String property)
	{
		if( property.equals( IX_WIDGET.MAW_TAB_SCREEN_CURRENT_TAB ) )
		{
			TabHost tabHost = (TabHost) getView( );
			return Integer.toString( tabHost.getCurrentTab( ) );
		}
		else
		{
			return super.getProperty( property );
		}
	}

	/**
	 * Passes on the back event to the currently active tab.
	 */
	@Override
	public boolean handleBack()
	{
		TabHost tabHost = (TabHost) getView( );
		int currentTabIndex = tabHost.getCurrentTab( );

		// This set is likely to be small so we can safely iterate over it
		for( Entry<ScreenWidget, Integer> entry : m_tabIndexToScreen.entrySet( ) )
		{
			if( entry.getValue( ) == currentTabIndex )
			{
				entry.getKey( ).handleBack( );
				return true;
			}
		}

		return false;
	}

	@Override
	public void removeChild(Widget child)
	{
		// There seems to be no sane way of removing
		// tabs on Android.
		Log.e( "MoSync", "It is currently not possible to remove children " +
			" from a tab screen on Android." );
		return;
	}

	@Override
	public void titleChanged(ScreenWidget screen, String newTitle)
	{
		int tabIndex = m_tabIndexToScreen.get( screen );
		TabHost tabHost = (TabHost) getView( );
		View tabIndicatorView = tabHost.getTabWidget( ).getChildTabViewAt( tabIndex );
		if( tabIndicatorView == null )
		{
			return;
		}

		TextView tabTitle = (TextView) tabIndicatorView.findViewById( android.R.id.title );
		tabTitle.setText( newTitle );
	}
}