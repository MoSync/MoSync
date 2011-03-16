package com.mosync.nativeui.core;

import java.util.Hashtable;

import android.app.Activity;
import android.graphics.Bitmap;
import android.util.Log;
import android.view.View;

import com.mosync.internal.android.MoSyncThread;
import com.mosync.internal.android.MoSyncThread.ImageCache;
import com.mosync.internal.generated.IX_WIDGET;
import com.mosync.java.android.MoSync;
import com.mosync.nativeui.ui.factories.ViewFactory;
import com.mosync.nativeui.ui.widgets.Layout;
import com.mosync.nativeui.ui.widgets.ScreenWidget;
import com.mosync.nativeui.ui.widgets.Widget;
import com.mosync.nativeui.util.HandleTable;
import com.mosync.nativeui.util.properties.InvalidPropertyValueException;
import com.mosync.nativeui.util.properties.PropertyConversionException;

/**
 * This class contains the implementation of the NativeUI system calls
 * for Android.
 * 
 * @author fmattias
 */
public class NativeUI
{
	/**
	 * Context of the main activity.
	 */
	private Activity m_activity;
	
	/**
	 * A table that contains a mapping between a handle and a widget, in a
	 * mosync program a handle is the only reference to a widget.
	 */
	private HandleTable<Widget> m_widgetTable = new HandleTable<Widget>();
	
	/**
	 * The screen that is currently shown, null if none.
	 */
	private ScreenWidget m_currentScreen = null;
	
	/**
	 * Mapping between image handles and bitmaps.
	 */
	private static Hashtable<Integer, ImageCache> m_imageTable = null;
	
	/**
	 * Constructor.
	 * 
	 * @param activity The Activity in which the widgets should be created.
	 */
	public NativeUI(Activity activity)
	{
		m_activity = activity;
	}
	
	/**
	 * Returns the bitmap for the given handle.
	 * 
	 * @param handle Integer handle for the bitmap to get.
	 * @return the bitmap for the given handle, or null if
	 *         it does not exist.
	 */
	public static Bitmap getBitmap(int handle)
	{
		if( m_imageTable != null && m_imageTable.containsKey( handle ) )
		{
			return m_imageTable.get( handle ).mBitmap;
		}
		else
		{
			return null;
		}
	}
	
	/**
	 * Sets the bitmap table, that is used to access mosync image
	 * resources.
	 * 
	 * @param imageTable The new bitmap table.
	 */
	public static void setImageTable(Hashtable<Integer, ImageCache> imageTable)
	{
		m_imageTable = imageTable;
	}
	
	/**
	 * Returns the view that currently should be shown. I.e.
	 * the view corresponding to the screen that was last shown.
	 * 
	 * @return The current root view.
	 */
	public View getRootView()
	{
		return m_currentScreen.getView( ); 
	}
	
	/**
	 * Internal function for the maWidgetCreate system call.
	 * It uses the ViewFactory to create a widget of the
	 * given type, puts it in the handle table and returns it.
	 * 
	 * Note: Should only be called on the UI thread.
	 */
	public int maWidgetCreate(String type)
	{
		if( !ViewFactory.typeExists( type ) )
		{
			Log.e( "MoSync", "maWidgetCreate: Unknown type: " + type );
			return IX_WIDGET.MAW_RES_INVALID_TYPE_NAME;
		}
		
		int nextHandle = m_widgetTable.getNextHandle( );
		Widget widget = ViewFactory.createView( type, m_activity, nextHandle );
		
		if( widget != null )
		{	
			m_widgetTable.add( nextHandle, widget );
			return nextHandle;
		}
		else
		{
			Log.e("MoSync", "maWidgetCreate: Error while creating widget: " + type );
			return IX_WIDGET.MAW_RES_ERROR;
		}
	}
	
	/**
	 * Internal function for the maWidgetDestroy system call.
	 * Destroys the given widget handle and all of its
	 * children.
	 * 
	 * Note: Should only be called on the UI thread.
	 */
	public int maWidgetDestroy(int widgetHandle)
	{
		Widget widget = m_widgetTable.get( widgetHandle );
		if( widget == null )
		{
			Log.e( "MoSync", "maWidgetDestroy: Invalid widget handle: " + widgetHandle );
			return IX_WIDGET.MAW_RES_INVALID_HANDLE;
		}
		
		Layout parent = (Layout)  widget.getParent( );
		if( parent != null )
		{
			parent.removeChild( widget );
		}
			
		m_widgetTable.remove( widgetHandle );
		return IX_WIDGET.MAW_RES_OK;
	}
	
	/**
	 * Internal function for the maWidgetAdd system call.
	 * Adds a child to the given parent, the parent
	 * must be of type Layout.
	 * 
	 * Note: Should only be called on the UI thread.
	 */
	public int maWidgetAdd(int parentHandle, int childHandle)
	{
		if( parentHandle == childHandle )
		{
			Log.e( "MoSync", "maWidgetAdd: Child and parent are the same." );
			return IX_WIDGET.MAW_RES_ERROR;
		}
		
		Widget parent = m_widgetTable.get( parentHandle );
		Widget child = m_widgetTable.get( childHandle );
		
		if( child == null )
		{
			Log.e( "MoSync", "maWidgetAdd: Invalid child widget handle: " + childHandle );
			return IX_WIDGET.MAW_RES_INVALID_HANDLE;
		}
		if( parent == null )
		{
			Log.e( "MoSync", "maWidgetAdd: Invalid parent widget handle: " + parentHandle );
			return IX_WIDGET.MAW_RES_INVALID_HANDLE;
		}
		if( !parent.isLayout( ) )
		{
			Log.e( "MoSync", "maWidgetAdd: Parent " + parentHandle + " is not a layout." );
			return IX_WIDGET.MAW_RES_INVALID_LAYOUT;
		}
		
		Layout parentAsLayout = (Layout) parent;
		parentAsLayout.addChild( child );
		
		return IX_WIDGET.MAW_RES_OK;
	}
	
	/**
	 * Internal function for the maWidgetRemove system call.
	 * Removes a child from the given parent, but keeps a
	 * reference to it.
	 * 
	 * Note: Should only be called on the UI thread.
	 */
	public int maWidgetRemove(int parentHandle, int childHandle)
	{		
		Widget parent = m_widgetTable.get( parentHandle );
		Widget child = m_widgetTable.get( childHandle );
		
		if( child == null )
		{
			Log.e( "MoSync", "maWidgetRemove: Invalid child widget handle: " + childHandle );
			return IX_WIDGET.MAW_RES_INVALID_HANDLE;
		}
		if( parent == null )
		{
			Log.e( "MoSync", "maWidgetRemove: Invalid parent widget handle: " + parentHandle );
			return IX_WIDGET.MAW_RES_INVALID_HANDLE;
		}
		
		if( !parent.isLayout( ) )
		{
			Log.e( "MoSync", "maWidgetRemove: Parent " + parentHandle + " is not a layout." );
			return IX_WIDGET.MAW_RES_INVALID_LAYOUT;
		}

		Layout parentAsLayout = (Layout) parent;
		parentAsLayout.removeChild( child );
		
		return IX_WIDGET.MAW_RES_OK;
	}
	
	/**
	 * Internal function for the maWidgetScreenShow system call.
	 * Sets the root widget to the root of the given screen, but
	 * it does not actually call setContentView, this should
	 * be done by the caller.
	 * 
	 * Note: Should only be called on the UI thread.
	 */
	public int maWidgetScreenShow(int screenWidget)
	{
		Widget screen = m_widgetTable.get( screenWidget );
		if( screen == null )
		{
			Log.e( "MoSync", "maWidgetScreenShow: Invalid screen handle: " + screenWidget );
			return IX_WIDGET.MAW_RES_INVALID_HANDLE;
		}
		if( !( screen instanceof ScreenWidget ) )
		{
			Log.e( "MoSync", "maWidgetScreenShow: Widget is not a screen: " + screenWidget );
			return IX_WIDGET.MAW_RES_INVALID_SCREEN;
		}

		m_currentScreen = (ScreenWidget) screen;
		return IX_WIDGET.MAW_RES_OK;
	}
	
	/**
	 * Internal function for the maWidgetSetProperty system call.
	 * Sets a property on the given widget, by accessing it from
	 * the widget table and calling its setProperty method.
	 * 
	 * Note: Should only be called on the UI thread.
	 */
	public int maWidgetSetProperty(int widgetHandle, String key, String value)
	{
		Widget widget = m_widgetTable.get( widgetHandle );
		if( widget == null )
		{
			Log.e( "MoSync", "maWidgetSetProperty: Invalid child widget handle: " + widgetHandle );
			return IX_WIDGET.MAW_RES_INVALID_HANDLE;
		}
		
		try
		{
			if( widget.setProperty( key, value ) )
			{
				return IX_WIDGET.MAW_RES_OK;
			}
			else
			{
				Log.e( "MoSync", "maWidgetSetProperty: Invalid property '" + key + "' on widget: " + widgetHandle );
				return IX_WIDGET.MAW_RES_INVALID_PROPERTY_NAME;
			}
		}
		catch(PropertyConversionException pce)
		{
			Log.e( "MoSync", "Error while converting property value '" + value + "': " + pce.getMessage( ) );
			return IX_WIDGET.MAW_RES_INVALID_PROPERTY_VALUE;
		}
		catch(InvalidPropertyValueException ipve)
		{
			Log.e( "MoSync", "Error while setting property: " + ipve.getMessage( ) );
			return IX_WIDGET.MAW_RES_INVALID_PROPERTY_VALUE;
		}
	}
	
	public int maWidgetGetProperty(
		int widgetHandle, 
		String key, 
		int memBuffer, 
		int memBufferSize)
	{
		Widget widget = m_widgetTable.get( widgetHandle );
		if( widget == null )
		{
			Log.e( "MoSync", "maWidgetGetProperty: Invalid child widget handle: " + widgetHandle );
			return IX_WIDGET.MAW_RES_INVALID_HANDLE;
		}
		
		String result = widget.getProperty( key );
		if( result.length( ) <= 0 )
		{
			Log.e( "MoSync", "maWidgetGetProperty: Invalid or empty property '" + 
					key + "' on widget: " + widgetHandle );
			return IX_WIDGET.MAW_RES_INVALID_PROPERTY_NAME;
		}
		
		if( result.length( ) + 1 > memBufferSize )
		{
			Log.e( "MoSync", "maWidgetGetProperty: Buffer size " + memBufferSize + 
					" too short to hold buffer of size: " + result.length( ) + 1 );
			return IX_WIDGET.MAW_RES_INVALID_STRING_BUFFER_SIZE;
		}
		
		byte[] ba = result.getBytes();

		// Write string to MoSync memory.
		MoSyncThread mosyncThread = ((MoSync) m_activity).getMoSyncThread( );
		mosyncThread.mMemDataSection.position( memBuffer );
		mosyncThread.mMemDataSection.put( ba );
		mosyncThread.mMemDataSection.put( (byte)0 );
		
		return result.length( );
	}
}
