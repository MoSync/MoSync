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

import java.util.ArrayList;
import java.util.List;

import android.view.View;
import android.view.ViewGroup;
import android.widget.ScrollView;

import com.mosync.nativeui.util.LayoutParamsSetter;
import com.mosync.nativeui.util.properties.InvalidPropertyValueException;
import com.mosync.nativeui.util.properties.PropertyConversionException;

/**
 * This subclass of Widget represents the behavior of a Widget
 * that can layout other widgets.
 * 
 * @author fmattias
 */
public class Layout extends Widget
{
	/**
	 * A list of the children of this widget. This information
	 * is perhaps needed if we destroy the layout before
	 * destroying all of the children.
	 */
	List<Widget> m_children = new ArrayList<Widget>( );
	ScrollView m_scrollview = null;
	boolean m_scrollable = false;
	

	/**
	 * Constructor.
	 * 
	 * @param handle handle Integer handle corresponding to this instance.
	 * @param view A layout wrapped by this widget.
	 */
	public Layout(int handle, ViewGroup view)
	{
		super( handle, view );
	}
	
	public void setIsScrollable(boolean s) 
	{
		if(m_scrollable == s) return;
		
		m_scrollable = s;
		
		if(s == true) 
		{	
			m_scrollview = new ScrollView( getView( ).getContext() );
			Widget w = getParent();
			if(w != null) 
			{
				ViewGroup g = (ViewGroup) w.getView( );

				for(int i = 0; i < g.getChildCount( ); i++)
				{
					View v = g.getChildAt( i );
					if( getView( ) == v )
					{
						g.removeViewAt(i);
						g.addView( m_scrollview, i );
						break;
					}
				}
			}
			
			m_scrollview.addView( getView( ) );	
		} 
		else
		{
			m_scrollview.removeView( getView( ) );
			
			Widget w = getParent();
			if(w != null) 
			{
				ViewGroup g = (ViewGroup) w.getView( );
				
				for(int i = 0; i < g.getChildCount( ); i++)
				{
					View v = g.getChildAt( i );
					if( m_scrollview == v )
					{
						g.addView( getView( ) , i );
						break;
					}
				}		
			}
		}
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
	public void addChildAt(Widget child, int index)
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
		ViewGroup layout = getView( );
		layout.addView( child.getRootView( ), listIndex );
	}

	/**
	 * Check if a widget is already in the children list.
	 * @param child The widget to be checked.
	 * @return true if the widget is already in the list of children.
	 */
	public Boolean containsChild(Widget child)
	{
		return m_children.contains(child);
	}

	/**
	 * Returns the children of this layout.
	 * 
	 * Note: Modifications to the returned list of children will
	 * not affect the layout.
	 * 
	 * @return a list of children for this layout.
	 */
	public List<Widget> getChildren()
	{
		return new ArrayList<Widget>( m_children );
	}
	
	/**
	 * Updates the Android layout params for the given child, according
	 * to the parameters specified in the child. Only the parameters that
	 * are supported for the layout will be taken into consideration.
	 * 
	 * @param child The child for which to update the layout params.
	 */
	public void updateLayoutParamsForChild(Widget child)
	{
		// Set layout params for the child
		ViewGroup.LayoutParams nativeLayoutParams = createNativeLayoutParams( child.getLayoutParams( ) );
		LayoutParamsSetter.setPossibleParams( child.getLayoutParams( ), nativeLayoutParams );
		
		View childView = child.getView();
		
		childView.setLayoutParams( nativeLayoutParams );
		
		View rootView = child.getRootView();
		if( childView != rootView )
		{
			rootView.setLayoutParams( new ViewGroup.LayoutParams( nativeLayoutParams.width, nativeLayoutParams.height ) );
		}
	}
	
	/**
	 * Removes a child form this layout, by default
	 * the view of the child is removed from the wrapped
	 * ViewGroup.
	 * 
	 * The child's parent will be set to null.
	 * 
	 * @param child
	 */
	public void removeChild(Widget child)
	{
		child.setParent( null );
		m_children.remove( child );
		ViewGroup layout = getView( );
		layout.removeView( child.getRootView( ) );
	}
	
	/**
	 * Updates the layout params for the given child
	 * to those suitable for this layout.
	 * 
	 * @param mosyncLayoutParams The mosync layout params to create a specific layout params for.
	 */
	public ViewGroup.LayoutParams createNativeLayoutParams(LayoutParams mosyncLayoutParams)
	{
		return new ViewGroup.LayoutParams( mosyncLayoutParams.getWidth( ), mosyncLayoutParams.getHeight( ) );
	}
	
	/**
	 * @see Widget.isLayout.
	 */
	public boolean isLayout()
	{
		return true;
	}
	
	/**
	 * @see Widget.getView.
	 */
	@Override
	public ViewGroup getView()
	{
		// ViewGroup is a covariant return type to View
		return (ViewGroup) super.getView( );
	}
	
	/**
	 * @see Widget.getView.
	 */
	@Override	 
	public ViewGroup getRootView()
	{
		if(m_scrollable == true) 
		{
			// ViewGroup is a covariant return type to View
			return (ViewGroup) m_scrollview;
		} 
		else 
		{
			return (ViewGroup) super.getView( );			
		}
	}
	
	@Override
	public boolean setProperty(String property, String value)
			throws PropertyConversionException, InvalidPropertyValueException
	{
		if( super.setProperty( property, value ) )
		{
			return true;
		}
		
		if( property.equals( "isScrollable" ) ) 
		{
			this.setIsScrollable( value.equals( "true" ) );
		}
		else
		{
			return false;
		}
		
		return true;
	}
}
