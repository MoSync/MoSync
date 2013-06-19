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

import java.util.HashMap;

import android.app.Activity;
import android.widget.ImageView;

import com.mosync.internal.generated.IX_OPENGL_ES_MA;
import com.mosync.internal.generated.IX_WIDGET;
import com.mosync.nativeui.ui.widgets.FrameLayout;
import com.mosync.nativeui.ui.widgets.ImageWidget;
import com.mosync.nativeui.ui.widgets.Widget;

/**
 * The ViewFactory is a singleton that is used to create
 * native widgets.
 *
 * If a view does not have any custom code used during
 * creation then the DefaultFactory can be used.
 *
 * @author fmattias
 */
public class ViewFactory
{
	/**
	 * Mapping between types and their factory.
	 */
	private static HashMap<String, AbstractViewFactory> m_factoryMap = new HashMap<String, AbstractViewFactory>( );

	/**
	 * Initializes the factory map.
	 *
	 * XXX: Perhaps this can be done in a more elegant way, so that
	 *      contributions can be made without changing this class.
	 */
	static
	{
		init( );
	}

	/**
	 * Creates a view in the given context of the given type.
	 *
	 * @param type The type of the widget to be created.
	 * @param context The current context.
	 * @param handle The handle for the widget that will be created.
	 * @return The created widget, or null if a widget could not be created.
	 */
	public static Widget createView(String type, Activity context, int handle)
	{
		if( m_factoryMap.containsKey( type ) )
		{
			return m_factoryMap.get( type ).create( context, handle );
		}
		else
		{
			return null;
		}
	}

	/**
	 * Returns true if there is a factory for the given type.
	 *
	 * @param type The type to check for a factory.
	 *
	 * @return true if there is a factory associated with the type,
	 *         false otherwise.
	 */
	public static boolean typeExists(String type)
	{
		return m_factoryMap.containsKey( type );
	}

	/**
	 * Initializes the factory with the listed type -> factory mappings.
	 */
	public static void init()
	{
		if( m_factoryMap.size( ) > 0 )
		{
			return;
		}

		addFactory( IX_WIDGET.MAW_BUTTON, new ButtonFactory( ) );
		addFactory( IX_WIDGET.MAW_IMAGE_BUTTON, new ImageButtonFactory( ) );
		addFactory( IX_WIDGET.MAW_LABEL, new LabelFactory( ) );
		addFactory( IX_WIDGET.MAW_VERTICAL_LAYOUT, new VerticalLayoutFactory( ) );
		addFactory( IX_WIDGET.MAW_HORIZONTAL_LAYOUT, new HorizontalLayoutFactory( ) );
		addFactory( IX_WIDGET.MAW_LIST_VIEW, new ListFactory( ) );
		addFactory( IX_WIDGET.MAW_LIST_VIEW_ITEM, new ListItemFactory( ) );
		addFactory( IX_WIDGET.MAW_SEARCH_BAR , new SearchBarFactory( ) );
		addFactory( IX_WIDGET.MAW_SCREEN, new ScreenFactory( ) );
		addFactory( IX_WIDGET.MAW_WEB_VIEW, new WebViewFactory( ) );
		addFactory( IX_WIDGET.MAW_TAB_SCREEN, new TabScreenFactory( ) );
		addFactory( IX_WIDGET.MAW_GL_VIEW, new EGLViewFactory( IX_OPENGL_ES_MA.MA_GL_API_GL1 ) );
		addFactory( IX_WIDGET.MAW_GL2_VIEW, new EGLViewFactory( IX_OPENGL_ES_MA.MA_GL_API_GL2 ) );
		addFactory( IX_WIDGET.MAW_IMAGE, new DefaultFactory( ImageView.class, ImageWidget.class ) );
		addFactory( IX_WIDGET.MAW_RELATIVE_LAYOUT, new DefaultFactory( android.widget.FrameLayout.class, FrameLayout.class ) );
		addFactory( IX_WIDGET.MAW_CHECK_BOX, new CheckBoxFactory( ) );
		addFactory( IX_WIDGET.MAW_EDIT_BOX, new EditBoxFactory( ) );
		addFactory( IX_WIDGET.MAW_STACK_SCREEN, new StackScreenFactory( ) );
		addFactory( IX_WIDGET.MAW_CAMERA_PREVIEW, new CameraPreviewFactory( ) );
		addFactory( IX_WIDGET.MAW_PROGRESS_BAR, new ProgressBarFactory( ) );
		addFactory( IX_WIDGET.MAW_ACTIVITY_INDICATOR, new ActivityIndicatorFactory( ) );
		addFactory( IX_WIDGET.MAW_SLIDER, new SliderFactory( ) );
		addFactory( IX_WIDGET.MAW_DATE_PICKER, new DatePickerFactory( ) );
		addFactory( IX_WIDGET.MAW_TIME_PICKER, new TimePickerFactory( ) );
		addFactory( IX_WIDGET.MAW_VIDEO_VIEW, new VideoViewFactory( ) );
		addFactory( IX_WIDGET.MAW_TOGGLE_BUTTON, new ToggleButtonFactory( ) );
		addFactory( IX_WIDGET.MAW_MODAL_DIALOG, new DialogFactory( ) );
		addFactory( IX_WIDGET.MAW_NAV_BAR, new NavigationBarFactory( ) );
		addFactory( IX_WIDGET.MAW_RADIO_BUTTON, new RadioButtonFactory( ) );
		addFactory( IX_WIDGET.MAW_RADIO_GROUP, new RadioGroupFactory( ) );
		addFactory( IX_WIDGET.MAW_RATING_BAR, new RatingBarFactory( ) );
		addFactory( IX_WIDGET.MAW_LIST_VIEW_SECTION,  new ListSectionFactory( ) );
		addFactory( IX_WIDGET.MAW_NUMBER_PICKER, new NumberPickerFactory( ) );
		addFactory( IX_WIDGET.MAW_CUSTOM_PICKER, new CustomPickerFactory( ) );
	}

	/**
	 * Adds a mapping between a type and a factory.
	 *
	 * @param type The type to add a factory for.
	 * @param factory A factory for the type.
	 */
	protected static void addFactory(String type, AbstractViewFactory factory)
	{
		m_factoryMap.put( type, factory );
	}

}