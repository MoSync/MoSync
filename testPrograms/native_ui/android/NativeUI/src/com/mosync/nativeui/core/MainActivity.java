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
        super.onCreate(savedInstanceState);
        
        FrameLayout rootWidget = new FrameLayout( this );
        setContentView( rootWidget );
        
        MoSync mosync = new MoSync( this, rootWidget );
        
        int list = mosync.maWidgetCreate( Types.WIDGET_TYPE_LIST );
        mosync.maWidgetSetProperty( list, Types.WIDGET_PROPERTY_WIDTH, "-1" );
        mosync.maWidgetSetProperty( list, Types.WIDGET_PROPERTY_HEIGHT, "-1" );
        mosync.maWidgetAdd( Types.WIDGET_ROOT, list );
        
        
        for(int i = 0; i < 10; i++)
        {
        	int listItem = mosync.maWidgetCreate( Types.WIDGET_TYPE_BUTTON );
        	mosync.maWidgetSetProperty( listItem, Types.WIDGET_PROPERTY_TEXT, "Click me " + i );
        	mosync.maWidgetSetProperty( listItem, Types.WIDGET_PROPERTY_HEIGHT, "60" );
        	mosync.maWidgetSetProperty( listItem, Types.WIDGET_PROPERTY_WIDTH, "-1" );
        	mosync.maWidgetAdd( list, listItem );
        }
    }
}