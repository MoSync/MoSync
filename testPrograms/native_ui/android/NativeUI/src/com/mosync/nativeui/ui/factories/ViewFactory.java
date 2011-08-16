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

import java.util.HashMap;

import com.mosync.nativeui.core.Types;
import com.mosync.nativeui.ui.widgets.Widget;

import android.content.Context;
import android.view.ViewGroup;

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
	 * @return The created widget, or null if a widget could not be created.
	 */
	public static Widget createView(String type, Context context)
	{
		if( m_factoryMap.containsKey( type ) )
		{
			Widget newWidget = m_factoryMap.get( type ).create( context, 0 );
			newWidget.getView( ).setLayoutParams( new ViewGroup.LayoutParams( ViewGroup.LayoutParams.WRAP_CONTENT , ViewGroup.LayoutParams.WRAP_CONTENT ) );
			return m_factoryMap.get( type ).create( context, 0 );
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