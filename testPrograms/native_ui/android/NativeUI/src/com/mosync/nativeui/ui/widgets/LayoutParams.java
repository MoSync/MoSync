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

package com.mosync.nativeui.ui.widgets;

import android.view.Gravity;
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
	public int horizontalAlignment = Gravity.LEFT;
	
	/**
	 * The vertical alignment of a widget.
	 */
	public int verticalAlignment = Gravity.TOP;
	
	/**
	 * The margins of this layout params.
	 */
	public int marginLeft   = 0;
	public int marginTop    = 0;
	public int marginRight  = 0;
	public int marginBottom = 0;
	
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