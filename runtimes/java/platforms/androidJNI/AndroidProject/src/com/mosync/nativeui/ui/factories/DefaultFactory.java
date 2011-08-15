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

import java.lang.reflect.Constructor;

import com.mosync.nativeui.ui.widgets.Widget;

import android.app.Activity;
import android.content.Context;
import android.view.View;

/**
 * A default factory for widget types that does not
 * require any custom creation code.
 * 
 * @author fmattias
 */
public class DefaultFactory implements AbstractViewFactory
{
	/**
	 * Class of the view to create.
	 */
	private Class<?> m_viewType;
	
	/**
	 * Type of widget class to use.
	 */
	private Class<?> m_widgetType;
	
	/**
	 * Creates a default factory for a view of type viewType.
	 * It then passes it to the default Widget constructor.
	 * 
	 * @param viewType Type of view to create using its Context
	 *                 constructor.
	 */
	public DefaultFactory(Class<?> viewType)
	{
		this( viewType, Widget.class );
	}
	
	/**
	 * Creates default factory for a view of type viewType and
	 * passes it to a subclass of Widget with a constructor that
	 * takes a handle and a view of type viewType.
	 * 
	 * @param viewType Type of view to create using its Context
	 *                 constructor.
	 * @param widgetType Type of widget to pass the instantiated view to.
	 */
	public DefaultFactory(Class<?> viewType, Class<?> widgetType)
	{
		m_viewType = viewType;
		m_widgetType = widgetType;
	}
	
	/**
	 * @see AbstractViewFactory.create.
	 */
	@Override
	public Widget create(Activity activity, int handle)
	{
		try
		{
			// Get the constructor that takes a context and create the view.
			Constructor<?> viewConstructor = m_viewType.getConstructor( Context.class );
			View v = (View) viewConstructor.newInstance( activity );
			
			// Get the widget constructor and pass the view and the handle.
			Constructor<?> widgetConstructor = m_widgetType.getConstructor( int.class, m_viewType );
			return (Widget) widgetConstructor.newInstance( handle, v );
		}
		catch(Exception e)
		{
			// Just ignore the exception
			return null;
		}
	}
}
