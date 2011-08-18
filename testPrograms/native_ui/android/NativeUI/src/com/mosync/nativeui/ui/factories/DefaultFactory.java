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
