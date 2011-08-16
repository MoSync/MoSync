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

/**
 * Simple class for accessing the constants related
 * to different types.
 * 
 * @author fmattias
 */
public class Types
{
	public static final int WIDGET_ROOT = -100;
	
	public static final String WIDGET_TYPE_BUTTON = "Button";
	public static final String WIDGET_TYPE_LABEL = "Label";
	public static final String WIDGET_TYPE_LIST = "ListBox";
	public static final String WIDGET_TYPE_LAYOUT_VERTICAL = "VerticalLayout";
	public static final String WIDGET_TYPE_LAYOUT_HORIZONTAL = "HorizontalLayout";
	
	public static final String WIDGET_PROPERTY_WIDTH = "width";
	public static final String WIDGET_PROPERTY_HEIGHT = "height";
	
	public static final String WIDGET_PROPERTY_PADDING_LEFT = "left";
	public static final String WIDGET_PROPERTY_PADDING_TOP = "top";
	public static final String WIDGET_PROPERTY_PADDING_RIGHT = "right";
	public static final String WIDGET_PROPERTY_PADDING_BOTTOM = "bottom";
	
	public static final String WIDGET_PROPERTY_HALIGNMENT = "halignment";
	public static final String WIDGET_PROPERTY_VALIGNMENT = "valignment";
	public static final String WIDGET_PROPERTY_TEXT = "text";
	
	public static final String WIDGET_PROPERTY_BACKGROUND_COLOR = "backgroundColor";
	public static final String WIDGET_PROPERTY_FONT_COLOR = "fontColor";
	
	public static final String WIDGET_HORIZONTAL_LEFT = "left";
	public static final String WIDGET_HORIZONTAL_CENTER = "center";
	public static final String WIDGET_HORIZONTAL_RIGHT = "right";
	
	public static final String WIDGET_VERTICAL_TOP = "top";
	public static final String WIDGET_VERTICAL_CENTER = "center";
	public static final String WIDGET_VERTICAL_BOTTOM = "bottom";
	
	public static final int WIDGET_OK = 0;
	public static final int WIDGET_ERROR = -1;
}
