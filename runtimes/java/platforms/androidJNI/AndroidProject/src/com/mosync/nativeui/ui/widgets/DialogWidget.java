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

package com.mosync.nativeui.ui.widgets;

import android.app.AlertDialog;
import android.content.DialogInterface;
import android.content.DialogInterface.OnDismissListener;
import android.view.ViewGroup;
import com.mosync.internal.android.EventQueue;
import com.mosync.internal.generated.IX_WIDGET;
import com.mosync.nativeui.core.Types;
import com.mosync.nativeui.util.properties.InvalidPropertyValueException;
import com.mosync.nativeui.util.properties.PropertyConversionException;

/**
 * A dialog widget that represents the root of a widget tree.
 * It acts like a layout: contains widgets, and can be dismissed
 * by using one of the two buttons: Negative/Positive.
 *
 *
 * @author emma
 */
public class DialogWidget extends Layout
{
	/**
	 * The alert dialog instance.
	 */
	private AlertDialog m_dialog;
	private AlertDialog.Builder m_dialogBuilder;

	/**
	 * The container layout.
	 */
	private ViewGroup m_container;

	/**
	 * The title of this dialog.
	 */
	private String m_title = "";

	/**
	 * Constructor
	 *
	 * @param handle handle Integer handle corresponding to this instance.
	 * @param view A screen wrapped by this widget.
	 */
	public DialogWidget(int handle, AlertDialog.Builder builder, ViewGroup view)
	{
		super( handle, view );
		m_dialogBuilder = builder;
		m_dialog = m_dialogBuilder.create();
		m_dialog.setOnDismissListener(new OnDismissListener() {
			@Override
			public void onDismiss(DialogInterface dialog) {
				EventQueue.getDefault( ).postWidgetEvent(
						IX_WIDGET.MAW_EVENT_DIALOG_DISMISSED,
						getHandle(),
						0,
						0);
			}
		});

		m_container = view;
	}

	public void show()
	{
		m_dialog.show();
	}

	public void hide()
	{
		if ( m_dialog.isShowing() )
			m_dialog.dismiss();
	}

	/**
	 * Returns true if the given widget is in fact a dialog.
	 *
	 * @return
	 */
	public boolean isDialog()
	{
		return true;
	}

	/**
	 * Adds a child to this layout at the given position. By
	 * default the view will be added to the wrapped ViewGroup.
	 *
	 * @param child The child to be added.
	 * @param index The index where to add the child. The child will have
	 *        this index after it has been added, the index starts at 0. If
	 *        the index is -1 it will be added at the end.
	 */
	@Override
	public int addChildAt(Widget child, int index)
	{
		int listIndex = index;
		if( index == -1 )
		{
			listIndex = m_children.size( );
		}

		child.setParent( this );
		m_children.add( listIndex, child );

		updateLayoutParamsForChild( child );

		// Add child to layout
		m_container.addView(child.getRootView(), listIndex);
		m_dialogBuilder.setView(m_container);

		return IX_WIDGET.MAW_RES_OK;
	}

	/**
	 * Removes a child form this layout, by default
	 * the view of the child is removed from the wrapped
	 * ViewGroup.
	 * The child's parent will be set to null.
	 *
	 * @param child
	 */
	@Override
	public int removeChild(Widget child)
	{
		child.setParent( null );
		m_children.remove( child );
		m_container.removeView( child.getRootView( ) );

		return IX_WIDGET.MAW_RES_OK;
	}

	/**
	 * @see Widget.setProperty.
	 */
	@Override
	public boolean setProperty(String property, String value)
			throws PropertyConversionException, InvalidPropertyValueException
	{
		if( property.equals( IX_WIDGET.MAW_MODAL_DIALOG_TITLE ) )
		{
			m_title = value;
			m_dialog.setTitle(value);
		}
		// Do not get any other properties besides those ones.
		// Because the size and the position of a dialog cannot be set.
		else if ( property.equals( IX_WIDGET.MAW_WIDGET_BACKGROUND_GRADIENT ) )
		{
			super.setProperty(property, value);
		}
		else if ( property.equals( IX_WIDGET.MAW_WIDGET_ALPHA ) )
		{
			super.setProperty(property, value);
		}
		else if ( property.equals( IX_WIDGET.MAW_WIDGET_BACKGROUND_COLOR ) )
		{
			super.setProperty(property, value);
		}
		else if ( property.equals(  Types.BACKGROUND_IMAGE ) )
		{
			super.setProperty(property, value);
		}
		else
		{
			return false;
		}

		return true;
	}

	/**
	 * @see Widget.getProperty.
	 */
	@Override
	public String getProperty(String property)
	{
		// Get only those properties.
		if( property.equals( IX_WIDGET.MAW_MODAL_DIALOG_TITLE ) )
		{
			return m_title;
		}
		else if ( property.equals( IX_WIDGET.MAW_WIDGET_VISIBLE ) )
		{
			return String.valueOf( m_dialog.isShowing() );
		}
		else
		{
			return INVALID_PROPERTY_NAME;
		}
	}

	/**
	 * @see Layout.updateLayoutParams.
	 */
	@Override
	public ViewGroup.LayoutParams createNativeLayoutParams(LayoutParams mosyncLayoutParams)
	{
		return new android.widget.FrameLayout.LayoutParams( mosyncLayoutParams.getWidth( ) , mosyncLayoutParams.getHeight( ) );
	}

	/**
	 * Returns the title of this dialog.
	 *
	 * @return the title of this dialog.
	 */
	public String getTitle()
	{
		return m_title;
	}

}
