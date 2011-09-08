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

import java.util.Hashtable;
import java.util.Iterator;
import java.util.List;

import android.app.Activity;
import android.app.LauncherActivity.ListItem;
import android.graphics.Bitmap;
import android.test.IsolatedContext;
import android.util.Log;
import android.view.View;

import com.mosync.internal.android.MoSyncCameraController;
import com.mosync.internal.android.MoSyncHelpers;
import com.mosync.internal.android.MoSyncThread;
import com.mosync.internal.android.MoSyncFont.MoSyncFontHandle;
import com.mosync.internal.android.MoSyncThread.ImageCache;
import com.mosync.internal.android.MoSyncView;
import com.mosync.internal.generated.IX_WIDGET;
import com.mosync.java.android.MoSync;
import com.mosync.nativeui.ui.factories.CameraPreviewFactory;
import com.mosync.nativeui.ui.factories.ViewFactory;
import com.mosync.nativeui.ui.widgets.ButtonWidget;
import com.mosync.nativeui.ui.widgets.CameraPreviewWidget;
import com.mosync.nativeui.ui.widgets.LabelWidget;
import com.mosync.nativeui.ui.widgets.Layout;
import com.mosync.nativeui.ui.widgets.ListItemWidget;
import com.mosync.nativeui.ui.widgets.MoSyncScreenWidget;
import com.mosync.nativeui.ui.widgets.ScreenWidget;
import com.mosync.nativeui.ui.widgets.StackScreenWidget;
import com.mosync.nativeui.ui.widgets.Widget;
import com.mosync.nativeui.util.HandleTable;
import com.mosync.nativeui.util.properties.FeatureNotAvailableException;
import com.mosync.nativeui.util.properties.IntConverter;
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
	 * The MoSync thread object.
	 */
	MoSyncThread mMoSyncThread;
	
	/**
	 * A table that contains a mapping between a handle and a widget, in a
	 * mosync program a handle is the only reference to a widget.
	 */
	private HandleTable<Widget> m_widgetTable = new HandleTable<Widget>();
	
	/**
	 * Listener for changes in the root view of the screen.
	 */
	private RootViewReplacedListener m_rootViewReplacedListener = null;
	
	/**
	 * Reference to the last shown screen.
	 */
	private Widget m_currentScreen = null;
	
	/**
	 * Mapping between image handles and bitmaps.
	 */
	private static Hashtable<Integer, ImageCache> m_imageTable = null;
	
	/**
	 * Constructor.
	 * @param thread The MoSync thread.
	 * @param activity The Activity in which the widgets should be created.
	 */
	public NativeUI(MoSyncThread thread, Activity activity)
	{
		mMoSyncThread = thread;
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
	 * Gets the bitmap table, that can be modified.
	 *
	 * @return The bitmap table.
	 */
	public Hashtable<Integer, ImageCache> getImageTable()
	{
		return m_imageTable;
	}

	/**
	 * Sets the default MoSync canvas view, so that it is possible
	 * to switch back to it from native UI.
	 * 
	 * @param mosyncScreen The MoSync canvas view.
	 */
	public void setMoSyncScreen(MoSyncView mosyncScreen)
	{
		if( mosyncScreen != null )
		{
			MoSyncScreenWidget screenWidget = new MoSyncScreenWidget( 
					IX_WIDGET.MAW_CONSTANT_MOSYNC_SCREEN_HANDLE, 
					mosyncScreen ); 
			
			m_widgetTable.add( IX_WIDGET.MAW_CONSTANT_MOSYNC_SCREEN_HANDLE, 
					screenWidget );
		}
		else
		{
			// If there is no MoSyncView, we cannot provide one here
			m_widgetTable.remove( IX_WIDGET.MAW_CONSTANT_MOSYNC_SCREEN_HANDLE );
		}
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
		
		internalMaWidgetDestroy( widget );
		
		return IX_WIDGET.MAW_RES_OK;
	}
	
	/**
	 * Recursive function that destroys the given widget and 
	 * its children.
	 * 
	 * @param widgetToDestroy The widget to destroy.
	 */
	private void internalMaWidgetDestroy(Widget widgetToDestroy)
	{
		// Disconnect widget from widget tree
		Layout parent = (Layout) widgetToDestroy.getParent( );
		if( parent != null )
		{
			parent.removeChild( widgetToDestroy );
		}
		
		// Disconnect and destroy children
		if( widgetToDestroy.isLayout( ) )
		{
			Layout widgetToDestroyAsLayout = (Layout) widgetToDestroy;
			for(Widget child : widgetToDestroyAsLayout.getChildren( ))
			{
				internalMaWidgetDestroy( child );
			}
		}

		// Destroy widget
		m_widgetTable.remove( widgetToDestroy.getHandle( ) );
	}
	
	/**
	 * Internal function for the maWidgetAdd system call.
	 * Uses maWidgetInsertChild to add the element at the
	 * end.
	 * 
	 * Note: Should only be called on the UI thread.
	 */
	public int maWidgetAdd(int parentHandle, int childHandle)
	{
		return maWidgetInsertChild(parentHandle, childHandle, -1);
	}
	
	/**
	 * Internal function for the maWidgetInsertChild system call.
	 * Inserts a child at a specific position in the given parent, the parent
	 * must be of type Layout.
	 * 
	 * Note: Should only be called on the UI thread.
	 */
	public int maWidgetInsertChild(int parentHandle, int childHandle, int index)
	{
		if( parentHandle == childHandle )
		{
			Log.e( "MoSync", "maWidgetInsertChild: Child and parent are the same." );
			return IX_WIDGET.MAW_RES_ERROR;
		}
		
		Widget parent = m_widgetTable.get( parentHandle );
		Widget child = m_widgetTable.get( childHandle );
		
		if( child == null )
		{
			Log.e( "MoSync", "maWidgetInsertChild: Invalid child widget handle: " + childHandle );
			return IX_WIDGET.MAW_RES_INVALID_HANDLE;
		}
		if( parent == null )
		{
			Log.e( "MoSync", "maWidgetInsertChild: Invalid parent widget handle: " + parentHandle );
			return IX_WIDGET.MAW_RES_INVALID_HANDLE;
		}
		if( !parent.isLayout( ) )
		{
			Log.e( "MoSync", "maWidgetInsertChild: Parent " + parentHandle + " is not a layout." );
			return IX_WIDGET.MAW_RES_INVALID_LAYOUT;
		}
		if( index < -1 )
		{
			Log.e( "MoSync", "maWidgetInsertChild: Invalid index: " + index );
			return IX_WIDGET.MAW_RES_INVALID_INDEX;
		}
		
		Layout parentAsLayout = (Layout) parent;

		if ( child.getParent() != null )
		{
			Log.e( "MoSync", "maWidgetInsertChild: Child already has a parent." );
			return IX_WIDGET.MAW_RES_ERROR;
		}

		parentAsLayout.addChildAt( child, index );
		
		return IX_WIDGET.MAW_RES_OK;
	}
	
	/**
	 * Internal function for the maWidgetRemove system call.
	 * Removes a child from its parent, but keeps a
	 * reference to it.
	 * 
	 * Note: Should only be called on the UI thread.
	 */
	public int maWidgetRemove(int childHandle)
	{		
		Widget child = m_widgetTable.get( childHandle );
		if( child == null )
		{
			Log.e( "MoSync", "maWidgetRemove: Invalid child widget handle: " + childHandle );
			return IX_WIDGET.MAW_RES_INVALID_HANDLE;
		}
		
		Widget parent = child.getParent( );
		if( parent == null )
		{
			Log.e( "MoSync", "maWidgetRemove: Widget " + childHandle + " has no parent." );
			return IX_WIDGET.MAW_RES_INVALID_HANDLE;
		}
		
		if( !parent.isLayout( ) )
		{
			Log.e( "MoSync", "maWidgetRemove: Parent for " + childHandle + " is not a layout." );
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
	 * be done through the RootViewReplacedListener.
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
		if( !( screen instanceof ScreenWidget ) && !( screen instanceof MoSyncScreenWidget ) )
		{
			Log.e( "MoSync", "maWidgetScreenShow: Widget is not a screen: " + screenWidget );
			return IX_WIDGET.MAW_RES_INVALID_SCREEN;
		}
		
		if( m_rootViewReplacedListener != null )
		{
			//m_rootViewReplacedListener.rootViewReplaced( screen.getView( ) );
			m_rootViewReplacedListener.rootViewReplaced( screen.getRootView( ) );
		}
		m_currentScreen = screen;
		
		return IX_WIDGET.MAW_RES_OK;
	}
	
	/**
	 * Internal function for the maWidgetStackScreenPush system call.
	 * Takes out the stack screen from the widget table and pushes
	 * the screen to it.
	 * 
	 * Note: Should only be called on the UI thread.
	 */
	public int maWidgetStackScreenPush(int stackScreenHandle, int newScreenHandle)
	{

		Widget stackScreenWidget = m_widgetTable.get( stackScreenHandle );
		if( stackScreenWidget == null )
		{
			Log.e( "MoSync", "maWidgetStackScreenPush: Invalid stack screen handle: " + stackScreenHandle );
			return IX_WIDGET.MAW_RES_INVALID_HANDLE;
		}
		if( !(stackScreenWidget instanceof StackScreenWidget) )
		{
			Log.e( "MoSync", "maWidgetStackScreenPush: Widget is not a stack screen: " + stackScreenHandle );
			return IX_WIDGET.MAW_RES_ERROR;
		}
		
		Widget newScreenWidget = m_widgetTable.get( newScreenHandle );
		if( newScreenWidget == null )
		{
			Log.e( "MoSync", "maWidgetStackScreenPush: Invalid screen handle: " + newScreenHandle );
			return IX_WIDGET.MAW_RES_INVALID_HANDLE;
		}
		if( !(newScreenWidget instanceof ScreenWidget) )
		{
			Log.e( "MoSync", "maWidgetStackScreenPush: Widget is not a screen: " + newScreenHandle );
			return IX_WIDGET.MAW_RES_ERROR;
		}
		
		StackScreenWidget stackScreen = (StackScreenWidget) stackScreenWidget;
		ScreenWidget newScreen = (ScreenWidget) newScreenWidget;
		
		stackScreen.push( newScreen );
		
		return IX_WIDGET.MAW_RES_OK;
	}
	
	/**
	 * Internal function for the maWidgetStackScreenPop system call.
	 * Takes out the stack screen from the widget table and pops the
	 * current screen from it.
	 * 
	 * Note: Should only be called on the UI thread.
	 */
	public int maWidgetStackScreenPop(int stackScreenHandle)
	{
		Widget stackScreenWidget = m_widgetTable.get( stackScreenHandle );
		if( stackScreenWidget == null )
		{
			Log.e( "MoSync", "maWidgetStackScreenPop: Invalid stack screen handle: " + stackScreenHandle );
			return IX_WIDGET.MAW_RES_INVALID_HANDLE;
		}
		if( !(stackScreenWidget instanceof StackScreenWidget) )
		{
			Log.e( "MoSync", "maWidgetStackScreenPop: Widget is not a stack screen: " + stackScreenHandle );
			return IX_WIDGET.MAW_RES_ERROR;
		}
		
		StackScreenWidget stackScreen = (StackScreenWidget) stackScreenWidget;
		stackScreen.pop( );
		
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
		
		boolean result;

		// Send the typeface to the label, button widget or list view item.
		if ( key.compareTo( IX_WIDGET.MAW_LABEL_FONT_HANDLE ) == 0 )
		{
			MoSyncFontHandle currentFont = null;

			// Search the fondle in the list of fonts.
			try
			{
				currentFont = mMoSyncThread.getMoSyncFont(IntConverter.convert(value));
			} catch(PropertyConversionException pce)
			{
				Log.e( "MoSync", "Error while getting font handle with value '" + value + "Invalid property value");
				return IX_WIDGET.MAW_RES_INVALID_PROPERTY_VALUE;
			}

			if ( currentFont == null )
			{
				Log.e( "MoSync", "Error while getting font handle with value '" + value + " The handle was not found");
				return IX_WIDGET.MAW_RES_INVALID_PROPERTY_VALUE;
			}
			else
			{
				Log.e("MoSync", "Set font typeface to native ui widget");
				if ( widget instanceof LabelWidget )
				{
					LabelWidget labelWidget = (LabelWidget) widget;
					labelWidget.setFontTypeface(
							currentFont.getTypeface(),
							currentFont.getFontSize());
				}
				else if ( widget instanceof ButtonWidget )
				{
					ButtonWidget buttonWidget = (ButtonWidget) widget;
					buttonWidget.setFontTypeface(
							currentFont.getTypeface(),
							currentFont.getFontSize());
				}
				else if ( widget instanceof ListItemWidget )
				{
					ListItemWidget listItemWidget = (ListItemWidget) widget;
					listItemWidget.setFontTypeface(
							currentFont.getTypeface(),
							currentFont.getFontSize());
				}
				else
				{
					Log.e( "MoSync", "Error while setting property '" + key );
					return IX_WIDGET.MAW_RES_INVALID_PROPERTY_NAME;
				}
				return IX_WIDGET.MAW_RES_OK;
			}
		}

		try
		{
			result =  widget.setProperty( key, value );

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
		catch(FeatureNotAvailableException fnae)
		{
			Log.e("MoSync", "Feature not available exception: " + fnae.getMessage() );
			return IX_WIDGET.MAW_RES_FEATURE_NOT_AVAILABLE;
		}

		if( result )
		{
			return IX_WIDGET.MAW_RES_OK;
		}
		else
		{
			Log.e( "MoSync", "maWidgetSetProperty: Invalid property '" + key + "' on widget: " + widgetHandle );
			return IX_WIDGET.MAW_RES_INVALID_PROPERTY_NAME;
		}
	}

	/**
	 * Internal function for the maWidgetGetProperty system call.
	 * Gets a property on the given widget, by accessing it from
	 * the widget table and calling its getProperty method.
	 *
	 * Note: Should only be called on the UI thread.
	 */
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
		
		String result;
		try {
			result = widget.getProperty( key );
		}catch( FeatureNotAvailableException fnae)
		{
			Log.e("MoSync", "Feature not available exception: " + fnae.getMessage() );
			return IX_WIDGET.MAW_RES_FEATURE_NOT_AVAILABLE;
		}

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

	
	/**
	 * Called when the back button has been pressed.
	 */
	public void handleBack()
	{
		if( m_currentScreen != null )
		{
			m_currentScreen.handleBack( );
		}
	}
	
	/**
	 * Sets a listener for when the root view is changed.
	 * 
	 * @param rootViewReplacedListener The class that listens for changes.
	 */
	public void setRootViewReplacedListener(RootViewReplacedListener rootViewReplacedListener)
	{
		m_rootViewReplacedListener = rootViewReplacedListener;
	}
	
	public interface RootViewReplacedListener
	{
		/**
		 * Called when the root view has been replaced
		 * by another root view.
		 * 
		 * @param newRoot The new root view.
		 */
		void rootViewReplaced(View newRoot);
	}
	
	public Widget getCameraView(final int handle)
	{
		return m_widgetTable.get(handle);
	}
	
}
