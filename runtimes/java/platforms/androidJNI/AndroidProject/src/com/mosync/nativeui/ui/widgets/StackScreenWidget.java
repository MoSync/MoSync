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

package com.mosync.nativeui.ui.widgets;

import java.util.Stack;

import com.mosync.internal.android.EventQueue;
import com.mosync.internal.android.MoSyncThread;
import com.mosync.internal.generated.IX_WIDGET;
import com.mosync.nativeui.util.ScreenTransitions;
import com.mosync.nativeui.util.properties.BooleanConverter;
import com.mosync.nativeui.util.properties.InvalidPropertyValueException;
import com.mosync.nativeui.util.properties.PropertyConversionException;

import android.util.Log;
import android.view.ViewGroup;

/**
 * Implementation of a stacked screen, which is a virtual screen
 * that displays whatever screen is on top of the stack. It also
 * handles back events, by going back in the stack each time
 * the user presses back.
 *
 * @author fmattias
 *
 */
public class StackScreenWidget extends ScreenWidget
{
	/**
	 * A Stack of Screens, the top of the stack must always be the screen
	 * that is being shown.
	 */
	private Stack<ScreenWidget> m_screenStack = new Stack<ScreenWidget>( );

	/**
	 * Determines if the stack screen automatically handles back events.
	 */
	private boolean m_backEnabled = true;

	/**
	 * The type of the animated screen transition used when pushing a screen.
	 */
	private int m_pushTransitionType = IX_WIDGET.MAW_TRANSITION_TYPE_NONE;

	/**
	 * The duration of the animated screen transition used when pushing a
	 * screen, in milliseconds
	 */
	private int m_pushTransitionDuration = 0;

	/**
	 * The type of the animated screen transition used when popping a screen.
	 */
	private int m_popTransitionType = IX_WIDGET.MAW_TRANSITION_TYPE_NONE;

	/**
	 * The duration of the animated screen transition used when popping a
	 * screen, in milliseconds
	 */
	private int m_popTransitionDuration = 0;

	/**
	 * Constructor.
	 *
	 * @param handle A handle to this widget.
	 * @param view The root layout for the screen, other screens will be shown
	 *             inside it.
	 */
	public StackScreenWidget(int handle, ViewGroup rootView)
	{
		super( handle, rootView );
	}

	@Override
	public boolean setProperty(String property, String value)
			throws PropertyConversionException, InvalidPropertyValueException
	{
		if( super.setProperty( property, value ) )
		{
			return true;
		}

		if( property.equals( IX_WIDGET.MAW_STACK_SCREEN_BACK_BUTTON_ENABLED ) )
		{
			m_backEnabled = BooleanConverter.convert( value );
		}
		else if( property.equals( IX_WIDGET.MAW_STACK_SCREEN_PUSH_TRANSITION_TYPE ) )
		{
			int intValue = Integer.parseInt(value);
			if ( !ScreenTransitions.isScreenTransitionAvailable(intValue) )
			{
				throw new InvalidPropertyValueException( value, property );
			}
			m_pushTransitionType = intValue;
		}
		else if( property.equals( IX_WIDGET.MAW_STACK_SCREEN_PUSH_TRANSITION_DURATION ) )
		{
			int intValue = Integer.parseInt(value);
			if ( 0 >= intValue )
			{
				throw new InvalidPropertyValueException( value, property );
			}
			m_pushTransitionDuration = intValue;
		}
		else if( property.equals( IX_WIDGET.MAW_STACK_SCREEN_POP_TRANSITION_TYPE ) )
		{
			int intValue = Integer.parseInt(value);
			if ( !ScreenTransitions.isScreenTransitionAvailable(intValue) )
			{
				throw new InvalidPropertyValueException( value, property );
			}
			m_popTransitionType = intValue;
		}
		else if( property.equals( IX_WIDGET.MAW_STACK_SCREEN_POP_TRANSITION_DURATION ) )
		{
			int intValue = Integer.parseInt(value);
			if ( 0 >= intValue )
			{
				throw new InvalidPropertyValueException( value, property );
			}
			m_popTransitionDuration = intValue;
		}
		else
		{
			return false;
		}

		return true;
	}

	/**
	 * Pushes a screen onto the screen stack, and displays
	 * it.
	 *
	 * @param screen The new top of the screen stack.
	 */
	public void push(ScreenWidget screen)
	{
		// Do not animate the first push since this is already animated
		// by the show of the stack screen (if shown with transition).
		if ( 0 != m_screenStack.size() )
		{
			ScreenTransitions.applyScreenTransition(screen.getRootView(),
					m_pushTransitionType, m_pushTransitionDuration, true);
		}

		m_screenStack.push( screen );

		getView( ).removeAllViews( );

		//getView( ).addView( screen.getView( ) );
		screen.getRootView( ).clearFocus();
		getView( ).addView( screen.getRootView( ) );

		// Add the screen to the children list.
		screen.setParent( this );
		m_children.add( m_children.size( ), screen );
	}

	private void sendPopEvent() {
		if( m_screenStack.empty( ) )
		{
			return;
		}

		// Only pop and send events if the stack is not empty
		int poppedToScreenHandle = getHandle( );
		if( m_screenStack.size( ) >= 2 )
		{
			// Get the previous screen before the last one.
			poppedToScreenHandle = m_screenStack.get(m_screenStack.size()-2).getHandle();
		}

		EventQueue.getDefault( ).postWidgetStackScreenPoppedEvent(
				getHandle(),
				m_screenStack.peek( ).getHandle( ),
				poppedToScreenHandle );
	}

	/**
	 * Pops a screen from the screen stack, and displays
	 * the previous screen. If there is no previous screen,
	 * nothing will be shown.
	 */
	public void pop()
	{
		sendPopEvent();

		if( m_screenStack.empty( ) )
		{
			return;
		}

		// Because the views are removed before pop we need to apply the fade out transition
		// on the current screen before the view is removed.
		if ( IX_WIDGET.MAW_TRANSITION_TYPE_FADE_OUT == m_popTransitionType )
		{
			ScreenTransitions.applyScreenTransition(m_screenStack.peek().getView(),
				m_popTransitionType, m_popTransitionDuration, true);
		}
		// Remove current view
		getView( ).removeAllViews( );

		// Remove the last screen from the children list.
		m_screenStack.peek().setParent( null );
		m_children.remove( m_screenStack.peek() );

		m_screenStack.pop( );

		if( m_screenStack.empty( ) )
		{
			return;
		}

		ScreenWidget previousScreen = m_screenStack.peek( );
		if( previousScreen != null )
		{
			previousScreen.getView( ).clearFocus();
			if ( IX_WIDGET.MAW_TRANSITION_TYPE_FADE_OUT != m_popTransitionType )
			{
				ScreenTransitions.applyScreenTransition(previousScreen.getView(),
						m_popTransitionType, m_popTransitionDuration, true);
			}
			getView( ).addView( previousScreen.getView( ) );

			// Redraw the action bar each time a screen is shown - or manually by calling maActionBarRefresh().
			Log.e("@@MoSync", "NativeUI maWidgetScreenShow invalidate menu");
			MoSyncThread mosyncThread = MoSyncThread.getInstance();
			mosyncThread.invalidateOptionsMenu(mosyncThread.getActivity());

		}
	}

	public ScreenWidget getCurrentScreen()
	{
		if ( m_screenStack.empty() )
			return null;
		return m_screenStack.peek();
	}

	/**
	 * Handles the back operation by popping a screen
	 * from the stack.
	 */
	@Override
	public boolean handleBack()
	{
		if( !m_backEnabled )
		{
			return false;
		}

		if(m_screenStack.size( ) <= 1) {
			return false;
		}

		pop( );

		return true;
	}

	/**
	 * Check if this stack screen is shown.
	 * @return true if the stack screen is displayed, false otherwise.
	 */
	@Override
	public boolean isShown()
	{
		ScreenWidget currentScreen = MoSyncThread.getInstance().getUnconvertedCurrentScreen();
		return this.equals( currentScreen );
	}
}
