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


import android.app.Activity;
import android.os.Bundle;
import android.widget.FrameLayout;

public class MainActivity extends Activity
{
    /** Called when the activity is first created. */
    @Override
    public void onCreate(Bundle savedInstanceState)
    {
        super.onCreate( savedInstanceState );
        
        FrameLayout rootWidget = new FrameLayout( this );
        setContentView( rootWidget );
        
        MoSync mosync = new MoSync( this, rootWidget );
        
        int layout = mosync.maWidgetCreate( Types.WIDGET_TYPE_LAYOUT_VERTICAL );
        mosync.maWidgetAdd( Types.WIDGET_ROOT, layout );
        
        int header = mosync.maWidgetCreate( Types.WIDGET_TYPE_LABEL );
        mosync.maWidgetSetProperty(header, Types.WIDGET_PROPERTY_HEIGHT, "60" );
        mosync.maWidgetSetProperty(header, Types.WIDGET_PROPERTY_TEXT, "Header" );
        mosync.maWidgetSetProperty(header, Types.WIDGET_PROPERTY_HALIGNMENT, "center" );
        mosync.maWidgetSetProperty(header, Types.WIDGET_PROPERTY_VALIGNMENT, "center" );
        mosync.maWidgetSetProperty(header, Types.WIDGET_PROPERTY_FONT_COLOR, "ffffff" );
        mosync.maWidgetAdd( layout , header );
        
        int list = mosync.maWidgetCreate( Types.WIDGET_TYPE_LIST );
        mosync.maWidgetAdd( layout, list );

        for(int i = 0; i < 10; i++)
        {
        	int listItem = mosync.maWidgetCreate( Types.WIDGET_TYPE_BUTTON );
        	mosync.maWidgetSetProperty( listItem, Types.WIDGET_PROPERTY_TEXT, "Click me " + i );
        	mosync.maWidgetSetProperty( listItem, Types.WIDGET_PROPERTY_HEIGHT, "60" );
        	mosync.maWidgetSetProperty( listItem, Types.WIDGET_PROPERTY_WIDTH, "100" );
        	mosync.maWidgetAdd( list, listItem );
        }
    }
}