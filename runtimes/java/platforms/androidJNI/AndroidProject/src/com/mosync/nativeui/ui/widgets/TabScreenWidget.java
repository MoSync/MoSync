package com.mosync.nativeui.ui.widgets;

import com.mosync.nativeui.core.Types;
import com.mosync.nativeui.util.properties.IntConverter;
import com.mosync.nativeui.util.properties.PropertyConversionException;

import android.view.View;
import android.widget.TabHost;
import android.widget.TabHost.TabSpec;
import android.widget.TabHost.TabContentFactory;

/**
 * A screen that allows navigation between screens using
 * tabs.
 * 
 * @author fmattias
 */
public class TabScreenWidget extends ScreenWidget
{
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
	}

	@Override
	public boolean setProperty(String property, String value)
			throws PropertyConversionException
	{
		if( super.setProperty( property, value ) )
		{
			return true;
		}
		
		TabHost tabHost = (TabHost) getView( );
		if( property.equals( Types.WIDGET_PROPERTY_CURRENT_TAB ) )
		{
			int currentTabIndex = IntConverter.convert( value );			
			tabHost.setCurrentTab( currentTabIndex );
		}
		
		return true;
	}

	@Override
	public void removeChild(Widget child)
	{
		return;
	}
}