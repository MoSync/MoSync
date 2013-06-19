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
import android.view.ViewGroup;
import android.view.ViewGroup.LayoutParams;
import android.widget.FrameLayout;
import android.widget.LinearLayout;
import android.widget.TabHost;
import android.widget.TabHost.OnTabChangeListener;
import android.widget.TabWidget;

import com.mosync.internal.android.EventQueue;
import com.mosync.nativeui.ui.widgets.TabScreenWidget;
import com.mosync.nativeui.ui.widgets.Widget;

/**
 * Creates a screen that contains other screens
 * as tabs.
 *
 * @author fmattias
 */
public class TabScreenFactory implements AbstractViewFactory
{
	/**
	 * Creates the layout specified by com.android.internal.R.layout.tab_content
	 * since we at the moment cannot use XML resources.
	 */
	@Override
	public Widget create(Activity activity, int handle)
	{
	     // Construct the tab host that fills the whole screen.
        TabHost tabHost = new TabHost( activity, null );
        tabHost.setLayoutParams(
        		new LinearLayout.LayoutParams(
                        LayoutParams.MATCH_PARENT, LayoutParams.MATCH_PARENT ) );

        // Create the visual structure of the tab host
        LinearLayout tabRootView = createTabRoot( activity );
        tabHost.addView( tabRootView );

        // Must be called when using views instead of activities to fill
        // a tabs content
        tabHost.setup( );

        tabHost.setOnTabChangedListener( new TabChangeListener( handle ) );

		return new TabScreenWidget( handle, tabHost );
	}

	/**
	 * Creates the visual layout of the whole tabbed screen.
	 *
	 * @param activity Activity to create the widgets in.
	 *
	 * @return the visual layout of the whole tabbed screen.
	 */
	private LinearLayout createTabRoot(Activity activity)
	{
        LinearLayout tabRootView = new LinearLayout( activity );
        tabRootView.setOrientation( LinearLayout.VERTICAL );
        tabRootView.setLayoutParams(
        		new ViewGroup.LayoutParams(
						LayoutParams.MATCH_PARENT, LayoutParams.MATCH_PARENT ) );

		tabRootView.addView( createTabWidget( activity ) );
		tabRootView.addView( createTabContent( activity ) );

		return tabRootView;
	}

	/**
	 * Creates the widget that holds the content of a
	 * tab.
	 *
	 * @param activity Activity to create the widgets in.
	 *
	 * @return the widget that holds the content of a tab.
	 */
	private FrameLayout createTabContent(Activity activity)
	{
		FrameLayout tabContent = new FrameLayout( activity );
        tabContent.setId( android.R.id.tabcontent );
        LinearLayout.LayoutParams tabContentParams = new LinearLayout.LayoutParams( LayoutParams.MATCH_PARENT, 0 );
        tabContentParams.weight = 1;
        tabContent.setLayoutParams( tabContentParams );

        return tabContent;
	}

	/**
	 * Creates the widget that holds the actual tabs.
	 *
	 * @param activity Activity to create the widgets in.
	 *
	 * @return the widget that holds the actual tabs.
	 */
	private TabWidget createTabWidget(Activity activity)
	{
        TabWidget tabWidget = new TabWidget( activity );
        tabWidget.setId( android.R.id.tabs );
        LinearLayout.LayoutParams tabWidgetParams =
			new LinearLayout.LayoutParams(
					LayoutParams.MATCH_PARENT, LayoutParams.WRAP_CONTENT );
        tabWidgetParams.weight = 0;
        tabWidget.setLayoutParams( tabWidgetParams );

        return tabWidget;
	}

	public class TabChangeListener implements OnTabChangeListener
	{

		private int m_tabScreenHandle;

		public TabChangeListener(int tabScreenHandle)
		{
			m_tabScreenHandle = tabScreenHandle;
		}

		@Override
		public void onTabChanged(String tabTag)
		{
			// Assumes that the tag of the tab is the same as it's handle.
			EventQueue.getDefault( ).postWidgetTabChangedEvent(
					m_tabScreenHandle,
					Integer.parseInt( tabTag ) );
		}
	}
}