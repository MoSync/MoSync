package com.mosync.nativeui.ui.factories;

import android.app.Activity;
import android.widget.FrameLayout;
import android.widget.LinearLayout;
import android.widget.TabHost;
import android.widget.TabWidget;
import android.view.ViewGroup;
import android.view.ViewGroup.LayoutParams;

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
        TabHost tabHost = new TabHost( activity );
        tabHost.setLayoutParams(
        		new LinearLayout.LayoutParams(
                        LayoutParams.FILL_PARENT, LayoutParams.FILL_PARENT ) );
 
        // Create the visual structure of the tab host
        LinearLayout tabRootView = createTabRoot( activity );
        tabHost.addView( tabRootView );
        
        // Must be called when using views instead of activities to fill
        // a tabs content
        tabHost.setup( );
        
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
        				LayoutParams.FILL_PARENT, LayoutParams.FILL_PARENT ) );
		
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
        LinearLayout.LayoutParams tabContentParams = new LinearLayout.LayoutParams( LayoutParams.FILL_PARENT, 0 );
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
        				LayoutParams.FILL_PARENT, LayoutParams.WRAP_CONTENT );
        tabWidgetParams.weight = 0;
        tabWidget.setLayoutParams( tabWidgetParams );
        
        return tabWidget;
	}
}