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
	 * @param handle handle Integer handle corresponding to this instance.
	 * @param view A layout wrapped by this widget.
	 */
	public Layout(int handle, ViewGroup view)
	{
		super( handle, view );
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

		updateLayoutParamsForChild(child);
		
		// Add child to layout
		ViewGroup layout = getView( );
		layout.addView( child.getView( ) );
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
		child.getView( ).setLayoutParams( nativeLayoutParams );
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
