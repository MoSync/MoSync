package com.mosync.nativeui.ui.widgets;

import java.util.ArrayList;
import java.util.List;

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
		ViewGroup layout = asViewGroup( );
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
		ViewGroup layout = asViewGroup( );
		layout.removeView( child.getView( ) );
	}
	
	/**
	 * @see Widget.setProperty.
	 */
	public boolean setProperty(int property, String value)
	{
		return super.setProperty( property, value );
	}
	
	/**
	 * @see Widget.isLayout.
	 */
	public boolean isLayout()
	{
		return true;
	}
	
	/**
	 * Returns the wrapped ViewGroup.
	 * 
	 * @return the wrapped ViewGroup.
	 */
	protected ViewGroup asViewGroup()
	{
		return (ViewGroup) getView( );
	}
}
