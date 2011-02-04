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
