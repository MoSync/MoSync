package com.mosync.nativeui.core;

import static com.mosync.internal.generated.IX_WIDGET.WIDGET_ERROR;
import static com.mosync.internal.generated.IX_WIDGET.WIDGET_OK;
import static com.mosync.internal.generated.IX_WIDGET.WIDGET_ROOT;

import java.util.Hashtable;

import android.app.Activity;
import android.graphics.Bitmap;
import android.util.Log;
import android.view.View;

import com.mosync.internal.android.MoSyncThread.ImageCache;
import com.mosync.java.android.MoSync;
import com.mosync.nativeui.ui.factories.ViewFactory;
import com.mosync.nativeui.ui.widgets.Layout;
import com.mosync.nativeui.ui.widgets.ScreenWidget;
import com.mosync.nativeui.ui.widgets.Widget;
import com.mosync.nativeui.util.HandleTable;
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
		int nextHandle = m_widgetTable.getNextHandle( );
		Widget widget = ViewFactory.createView( type, m_activity, nextHandle );
		
		if( widget != null )
		{	
			m_widgetTable.add( nextHandle, widget );
			return nextHandle;
		}
		else
		{
			return WIDGET_ERROR;
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
			return WIDGET_OK;
		}
		else
		{
			return WIDGET_ERROR;
		}
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
		Widget parent = m_widgetTable.get( parentHandle );
		Widget child = m_widgetTable.get( childHandle );
		
		if( child == null || parent == null )
		{
			return WIDGET_ERROR;
		}

		if( parent.isLayout( ) )
		{
			Layout parentAsLayout = (Layout) parent;
			parentAsLayout.addChild( child );
			return WIDGET_OK;
		}
		else
		{
			return WIDGET_ERROR;
		}
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
		if( childHandle == WIDGET_ROOT )
		{
			return WIDGET_ERROR;
		}
		
		Widget parent = m_widgetTable.get( parentHandle );
		Widget child = m_widgetTable.get( childHandle );
		
		if( child == null || parent == null )
		{
			return WIDGET_ERROR;
		}
		
		if( parent.isLayout( ) )
		{
			Layout parentAsLayout = (Layout) parent;
			parentAsLayout.removeChild( child );
			return WIDGET_OK;
		}
		else
		{
			return WIDGET_ERROR;
		}
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
		if( screen != null && screen instanceof ScreenWidget )
		{
			m_currentScreen = (ScreenWidget) screen;
			return WIDGET_OK;
		}
		else
		{
			return WIDGET_ERROR;
		}
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
			return WIDGET_ERROR;
		}
		
		try
		{
			widget.setProperty( key, value );
			return WIDGET_OK;
		}
		catch(PropertyConversionException pce)
		{
			Log.e( this.getClass( ).getCanonicalName( ) , 
				"Error while converting property: " + pce.getMessage( ), pce );
			return WIDGET_ERROR;
		}
	}
	
	public int maWidgetGetProperty(int widgetHandle, String key, int memBuffer, int memBufferSize)
	{
		Widget widget = m_widgetTable.get( widgetHandle );
		if( widget == null )
		{
			return WIDGET_ERROR;
		}
		
		String result = widget.getProperty( key );
		Log.i("MoSync", "maWidgetGetProperty return: " + result);
		
		if( result.length( ) + 1 > memBufferSize )
		{
			return WIDGET_ERROR;
		}
		
		byte[] ba = result.getBytes();
		
		MoSync.mMoSyncThread.mMemDataSection.mark( );
		MoSync.mMoSyncThread.mMemDataSection.position( memBuffer );
		MoSync.mMoSyncThread.mMemDataSection.put( ba );
		MoSync.mMoSyncThread.mMemDataSection.put( (byte)0 );
		MoSync.mMoSyncThread.mMemDataSection.reset( );
		
		return result.length( );
	}
}
