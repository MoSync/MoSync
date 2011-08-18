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

package com.mosync.nativeui.core;

import com.mosync.nativeui.ui.factories.ViewFactory;
import com.mosync.nativeui.ui.widgets.Layout;
import com.mosync.nativeui.ui.widgets.Widget;
import com.mosync.nativeui.util.HandleTable;
import com.mosync.nativeui.util.properties.PropertyConversionException;

import android.content.Context;
import android.util.Log;
import android.view.ViewGroup;

public class MoSync
{
	/**
	 * Context of the main activity.
	 */
	private Context m_context;
	
	/**
	 * A table that contains a mapping between a handle and a widget, in a
	 * mosync program a handle is the only reference to a widget.
	 */
	private HandleTable<Widget> m_widgetTable = new HandleTable<Widget>();
	
	/**
	 * 
	 * 
	 * @param context The context that the widgets should be created in.
	 * @param rootWidget The root widget in this context, usually a FrameLayout.
	 */
	public MoSync(Context context, ViewGroup rootWidget)
	{
		m_context = context;
		m_widgetTable.add( Types.WIDGET_ROOT, new Layout( Types.WIDGET_ROOT, rootWidget ) );
	}
	
	public int maWidgetCreate(String type)
	{
		Widget widget = ViewFactory.createView( type, m_context );
		
		if( widget != null )
		{	
			int handle = m_widgetTable.add( widget );
			return handle;
		}
		else
		{
			return Types.WIDGET_ERROR;
		}
	}
	
	public int maWidgetDestroy(int widgetHandle)
	{
		Widget widget = m_widgetTable.get( widgetHandle );
		if( widget != null )
		{
			/**
			 * XXX: Destroy all children?
			 */
			Layout parent = (Layout)  widget.getParent( );
			if( parent != null )
			{
				Widget child = m_widgetTable.get( widgetHandle );
				parent.removeChild( child );
			}
			
			m_widgetTable.remove( widgetHandle );
			return Types.WIDGET_OK;
		}
		else
		{
			return Types.WIDGET_ERROR;
		}
	}
	
	public int maWidgetAdd(int parentHandle, int childHandle)
	{
		Widget parent = m_widgetTable.get( parentHandle );
		Widget child = m_widgetTable.get( childHandle );
		
		if( child == null || parent == null )
		{
			return Types.WIDGET_ERROR;
		}

		if( parent.isLayout( ) )
		{
			Layout parentAsLayout = (Layout) parent;
			parentAsLayout.addChild( child );
			return Types.WIDGET_OK;
		}
		else
		{
			return Types.WIDGET_ERROR;
		}
	}
	
	public int maWidgetRemove(int parentHandle, int childHandle)
	{
		if( childHandle == Types.WIDGET_ROOT )
		{
			return Types.WIDGET_ERROR;
		}
		
		Widget parent = m_widgetTable.get( parentHandle );
		Widget child = m_widgetTable.get( childHandle );
		
		if( child == null || parent == null )
		{
			return Types.WIDGET_ERROR;
		}
		
		if( parent.isLayout( ) )
		{
			Layout parentAsLayout = (Layout) parent;
			parentAsLayout.removeChild( child );
			return Types.WIDGET_OK;
		}
		else
		{
			return Types.WIDGET_ERROR;
		}
	}
	
	public int maWidgetSetProperty(int widgetHandle, String key, String value)
	{
		Widget widget = m_widgetTable.get( widgetHandle );
		if( widget == null )
		{
			return Types.WIDGET_ERROR;
		}
		
		try
		{
			widget.setProperty( key, value );
			return Types.WIDGET_OK;
		}
		catch(PropertyConversionException pce)
		{
			Log.e( this.getClass( ).getCanonicalName( ) , "Error while converting property: " + pce.getMessage( ), pce );
			return Types.WIDGET_ERROR;
		}
	}
}
