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