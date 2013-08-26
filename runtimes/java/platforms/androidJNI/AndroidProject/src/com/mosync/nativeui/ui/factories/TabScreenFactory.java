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

package com.mosync.nativeui.ui.factories;

import android.app.Activity;
import android.util.Log;
import android.view.ViewGroup;
import android.view.ViewGroup.LayoutParams;
import android.widget.FrameLayout;
import android.widget.LinearLayout;
import android.widget.TabHost;
import android.widget.TabHost.OnTabChangeListener;
import android.widget.TabWidget;

import com.mosync.internal.android.EventQueue;
import com.mosync.internal.android.MoSyncThread;
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
			MoSyncThread mosyncThread = MoSyncThread.getInstance();

			// Save the current tab screen.
			mosyncThread.setCurrentScreen(m_tabScreenHandle);

			// Recreate options menu
			Log.e("@@MoSync", "TabChangeListener onTabChanged invalidate options menu");
			mosyncThread.invalidateOptionsMenu(mosyncThread.getActivity());

			// Assumes that the tag of the tab is the same as it's handle.
			EventQueue.getDefault( ).postWidgetTabChangedEvent(
					m_tabScreenHandle,
					Integer.parseInt( tabTag ) );
		}
	}
}