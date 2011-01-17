package com.mosync.nativeui.ui.factories;

import java.util.HashMap;

import android.app.Activity;
import android.webkit.WebView;
import android.widget.ImageView;

import com.mosync.nativeui.core.Types;
import com.mosync.nativeui.ui.egl.EGLView;
import com.mosync.nativeui.ui.widgets.GLWidget;
import com.mosync.nativeui.ui.widgets.ImageWidget;
import com.mosync.nativeui.ui.widgets.WebWidget;
import com.mosync.nativeui.ui.widgets.Widget;

/**
 * The ViewFactory is a singleton that is used to create
 * native widgets.
 * 
 * If a view does not have any custom code used during 
 * creation then the DefaultFactory can be used.
 * 
 * @author fmattias
 */
public class ViewFactory
{
	/**
	 * Mapping between types and their factory.
	 */
	private static HashMap<String, AbstractViewFactory> m_factoryMap = new HashMap<String, AbstractViewFactory>( );
	
	/**
	 * Initializes the factory map.
	 * 
	 * XXX: Perhaps this can be done in a more elegant way, so that
	 *      contributions can be made without changing this class.
	 */
	static
	{
		init( );
	}
	
	/**
	 * Creates a view in the given context of the given type.
	 * 
	 * @param type The type of the widget to be created.
	 * @param context The current context.
	 * @param handle The handle for the widget that will be created.
	 * @return The created widget, or null if a widget could not be created.
	 */
	public static Widget createView(String type, Activity context, int handle)
	{
		if( m_factoryMap.containsKey( type ) )
		{
			return m_factoryMap.get( type ).create( context, handle );
		}
		else
		{
			return null;
		}
	}
	
	/**
	 * Initializes the factory with the listed type -> factory mappings.
	 */
	public static void init()
	{
		if( m_factoryMap.size( ) > 0 )
		{
			return;
		}
		
		addFactory( Types.WIDGET_TYPE_BUTTON, new ButtonFactory( ) );
		addFactory( Types.WIDGET_TYPE_LABEL, new LabelFactory( ) );
		addFactory( Types.WIDGET_TYPE_LAYOUT_VERTICAL , new VerticalLayoutFactory( ) );
		addFactory( Types.WIDGET_TYPE_LIST, new ListFactory( ) );
		addFactory( Types.WIDGET_TYPE_SCREEN, new ScreenFactory( ) );
		addFactory( Types.WIDGET_TYPE_LIST_ITEM, new ListItemFactory( ) );
		addFactory( Types.WIDGET_TYPE_WEB, new DefaultFactory( WebView.class, WebWidget.class ) );
		addFactory( Types.WIDGET_TYPE_TAB_SCREEN, new TabScreenFactory( ) );
		addFactory( Types.WIDGET_TYPE_EGL, new DefaultFactory( EGLView.class, GLWidget.class ) );
		addFactory( Types.WIDGET_TYPE_IMAGE, new DefaultFactory( ImageView.class, ImageWidget.class ) );
	}
	
	/**
	 * Adds a mapping between a type and a factory.
	 * 
	 * @param type The type to add a factory for. 
	 * @param factory A factory for the type.
	 */
	protected static void addFactory(String type, AbstractViewFactory factory)
	{
		m_factoryMap.put( type, factory );
	}
}