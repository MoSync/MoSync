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

import com.mosync.nativeui.util.LayoutParamsSetter;

import android.view.ViewGroup;

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

	/**
	 * Constructor.
	 * 
	 * @param handle Handle to this layout.
	 * @param view The layout that is wrapped.
	 */
	public Layout(int handle, ViewGroup view)
	{
		super(handle, view);
	}
	
	/**
	 * Adds a child to this layout, by default the view
	 * of the child is added to the wrapped ViewGroup.
	 * 
	 * The child's parent will be set to this layout.
	 * 
	 * @param child The child of to be added.
	 */
	public void addChild(Widget child)
	{
		child.setParent( this );
		m_children.add( child );

		// Set layout params for the child
		ViewGroup.LayoutParams nativeLayoutParams = createNativeLayoutParams( child.getLayoutParams( ) );
		LayoutParamsSetter.setPossibleParams( child.getLayoutParams( ), nativeLayoutParams );
		child.getView( ).setLayoutParams( nativeLayoutParams );
		
		// Add child to layout
		ViewGroup layout = getView( );
		layout.addView( child.getView( ) );
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
		layout.removeView( child.getView( ) );
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
}
