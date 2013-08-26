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
	 * The class of the view to create.
	 * 
	 * @param viewType
	 */
	public DefaultFactory(Class<?> viewType)
	{
		m_viewType = viewType;
	}
	
	/**
	 * @see AbstractViewFactory.create.
	 */
	@Override
	public Widget create(Context context, int handle)
	{
		try
		{
			// Get the constructor that takes a context and create the view.
			Constructor<?> constructor = m_viewType.getConstructor( Context.class );
			View v = (View) constructor.newInstance( context );
			return new Widget( handle, v );
		}
		catch(Exception e)
		{
			// Just ignore the exception
			return null;
		}
	}
}
