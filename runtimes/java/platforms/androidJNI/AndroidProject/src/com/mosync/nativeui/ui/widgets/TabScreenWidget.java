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

package com.mosync.nativeui.ui.widgets;

import java.util.HashMap;
import java.util.Map.Entry;

import android.graphics.drawable.Drawable;
import android.os.Build;
import android.util.Log;
import android.view.View;
import android.widget.ImageView;
import android.widget.TabHost;
import android.widget.TabHost.TabContentFactory;
import android.widget.TabHost.TabSpec;
import android.widget.TextView;

import com.mosync.internal.android.MoSyncThread;
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
	implements ScreenWidget.TitleChangedListener, ScreenWidget.IconChangedListener
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
	public int addChildAt(Widget child, int index)
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
			Log.e( "MoSync", "maWidgetInsertChild: TabScreen can only contain ScreenWidgets." );
			return IX_WIDGET.MAW_RES_INVALID_HANDLE;
		}

		ScreenWidget screen = (ScreenWidget) child;
		final View screenView = screen.getView( );

		TabHost tab = (TabHost) getView( );

		int indexOfNewTab = tab.getTabWidget( ).getChildCount( );
		TabSpec tabSpec = tab.newTabSpec( Integer.toString( indexOfNewTab ) );
		/**
		 * MOSYNC-3108: starting with ICS, the tab icon does not fit next to the label,
		 * when set using tabSpec indicators.
		 * To solve it, set the label text inside the TabWidget instead.
		 */
		setIndicators( tabSpec, screen.getTitle( ), screen.getIcon( ) );

		// Add the screen to the children list.
		child.setParent( this );
		m_children.add( indexOfNewTab, child );

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
		screen.setIconChangedListener( this );

		// Android 4.0 onwards: set the label's text on the tab widget,
		// so that both icon and text will appear.
		titleChanged(screen, screen.getTitle());

		return IX_WIDGET.MAW_RES_OK;
	}

	private void setIndicators(TabSpec tabSpec, String title, Drawable icon)
	{
		if( icon != null )
		{
			if ( Build.VERSION.SDK_INT < Build.VERSION_CODES.ICE_CREAM_SANDWICH )
			{
				// Android 2.1: both icon and text are shown (Label below the Icon as expected)
				tabSpec.setIndicator( title, icon );

			}
			else
			{
				// Android 4.0 onwards: If both icon and text are set, only the title is visible.
				tabSpec.setIndicator( "", icon );
			}
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
			if ( currentTabIndex >= m_tabIndexToScreen.size()
					||
				 currentTabIndex < 0 )
			{
				throw new InvalidPropertyValueException(property, value);
			}
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
	 * Get the currently shown tab.
	 * Used when launching the OptionsMenu.
	 * @return The currently active tab screen.
	 */
	public ScreenWidget getCurrentTabScreen()
	{
		TabHost tabHost = (TabHost) getView();
		int index = tabHost.getCurrentTab();

		for( Entry<ScreenWidget, Integer> entry : m_tabIndexToScreen.entrySet( ) )
		{
			if( entry.getValue( ) == index )
			{
				return entry.getKey();
			}
		}
		return null;
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
	public int removeChild(Widget child)
	{
		// There seems to be no sane way of removing
		// tabs on Android.
		Log.e( "MoSync", "It is currently not possible to remove children " +
			" from a tab screen on Android." );
		return IX_WIDGET.MAW_RES_ERROR;
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

	@Override
	public void iconChanged(ScreenWidget screen, Drawable newIcon)
	{
//		Log.e("@@MoSync","TabIcon changed");

		int tabIndex = m_tabIndexToScreen.get( screen );
		TabHost tabHost = (TabHost) getView( );
		View tabIndicatorView = tabHost.getTabWidget( ).getChildTabViewAt( tabIndex );

		ImageView icon = (ImageView) tabIndicatorView.findViewById(android.R.id.icon);
		icon.setImageDrawable(newIcon);
	}

	/**
	 * Check if this tab screen is shown.
	 * @return true if the tab screen is displayed, false otherwise.
	 */
	@Override
	public boolean isShown()
	{
		ScreenWidget currentScreen = MoSyncThread.getInstance().getUnconvertedCurrentScreen();
		return this.equals( currentScreen );
	}
}
