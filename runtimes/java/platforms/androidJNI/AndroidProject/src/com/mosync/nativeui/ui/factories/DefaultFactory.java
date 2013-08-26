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
