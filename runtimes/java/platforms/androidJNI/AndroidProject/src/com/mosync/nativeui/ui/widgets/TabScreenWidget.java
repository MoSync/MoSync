package com.mosync.nativeui.ui.widgets;

import java.util.HashMap;

import android.view.View;
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
public class TabScreenWidget extends ScreenWidget implements ScreenWidget.TitleChangedListener
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
	public void addChild(Widget child)
	{
		if( !( child instanceof ScreenWidget ) )
		{
			return;
		}
		ScreenWidget screen = (ScreenWidget) child;
		final View screenView = screen.getView( );
		
		TabHost tab = (TabHost) getView( );
		
		TabSpec tabSpec = tab.newTabSpec( Integer.toString( screen.getHandle( ) ) );
		
		if( screen.getIcon( ) != null )
		{
			tabSpec.setIndicator( screen.getTitle( ), screen.getIcon( ) );
		}
		else
		{
			tabSpec.setIndicator( screen.getTitle( ) );
		}
		
		// Provides the tab with its content.
		tabSpec.setContent( new TabContentFactory( ) {
			@Override
			public View createTabContent(String tag)
			{
				return screenView;
			}
		});
		
		
		tab.addTab( tabSpec );
		m_tabIndexToScreen.put( screen, tab.getTabWidget( ).getChildCount( ) - 1 );
		screen.setTitleChangedListener( this );
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
	public void removeChild(Widget child)
	{
		m_tabIndexToScreen.remove( child );
		
		return;
	}

	@Override
	public void titleChanged(ScreenWidget screen, String newTitle)
	{
		int tabIndex = m_tabIndexToScreen.get( screen );
		TabHost tabHost = (TabHost) getView( );
		View tabIndicatorView = (TextView) tabHost.getTabWidget( ).getChildTabViewAt( tabIndex );
		if( tabIndicatorView == null )
		{
			return;
		}
		
		TextView tabTitle = (TextView) tabIndicatorView.findViewById( android.R.id.title );
		tabTitle.setText( newTitle );
	}
}