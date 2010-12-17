package com.mosync;


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
        
        NativeUI mosync = new NativeUI( this, rootWidget );
        
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