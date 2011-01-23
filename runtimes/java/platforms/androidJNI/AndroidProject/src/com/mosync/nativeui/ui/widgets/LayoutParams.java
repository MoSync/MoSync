package com.mosync.nativeui.ui.widgets;

import android.view.ViewGroup;

/**
 * A meta class for layout params, in the sense that it contains
 * the parameters included in most of the layout params types.
 * It is then converted to the appropriate layout params when
 * it is added to a layout.
 * 
 * @author fmattias
 */
public class LayoutParams extends ViewGroup.LayoutParams
{
	/**
	 * Represents something that is not set.
	 */
	public static final int EMPTY = -100;
	
	/**
	 * The horizontal alignment of a widget.
	 */
	public int horizontalAlignment = -1;
	
	/**
	 * The vertical alignment of a widget.
	 */
	public int verticalAlignment = -1;
	
	/**
	 * The margins of this layout params.
	 */
	public int marginLeft   = 0;
	public int marginTop    = 0;
	public int marginRight  = 0;
	public int marginBottom = 0;
	
	/**
	 * Constructor.
	 */
	public LayoutParams()
	{
		super( WRAP_CONTENT, WRAP_CONTENT );
	}
	
	/**
	 * Returns the width specified in the given layout params.
	 * 
	 * @param layoutParams
	 * @return the width specified in the given layout params.
	 */
	public int getWidth()
	{
		return width;
	}
	
	/**
	 * Returns the height specified in the given layout params.
	 * 
	 * @param layoutParams
	 * @return the height specified in the given layout params.
	 */
	public int getHeight()
	{
		return height;
	}
	
	/**
	 * Returns the horizontal alignment, i.e. one of
	 * Gravity.LEFT, Gravity.CENTER_HORIZONTAL or Gravity.RIGHT.
	 * 
	 * @return
	 */
	public int getHorizontalAlignment()
	{
		return horizontalAlignment;
	}
	
	/**
	 * Returns the vertical alignment, i.e. one of
	 * Gravity.TOP, Gravity.CENTER_VERTICAL or Gravity.BOTTOM.
	 * 
	 * @return
	 */
	public int getVerticalAlignment()
	{
		return verticalAlignment;
	}
	
	/**
	 * Returns the left margin.
	 * 
	 * @return the left margin.
	 */
	public int getMarginLeft()
	{
		return marginLeft;
	}
	
	/**
	 * Returns the top margin.
	 * 
	 * @return the top margin.
	 */
	public int getMarginTop()
	{
		return marginTop;
	}
	
	/**
	 * Returns the right margin.
	 * 
	 * @return the right margin.
	 */
	public int getMarginRight()
	{
		return marginRight;
	}
	
	/**
	 * Returns the bottom margin.
	 * 
	 * @return the bottom margin.
	 */
	public int getMarginBottom()
	{
		return marginBottom;
	}
}