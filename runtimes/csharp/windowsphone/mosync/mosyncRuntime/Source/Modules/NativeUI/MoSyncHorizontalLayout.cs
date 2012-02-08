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
/**
 * @file MoSyncHorizontalLayout.cs
 * @author Ciprian Filipas
 *
 * @brief This represents the Horizontal Layout Widget implementation
 *        for the NativeUI component on Windows Phone 7, language c#
 *
 * @platform WP 7.1
 **/

using Microsoft.Phone.Controls;
using System.Windows.Controls;
using System.Windows;
using System.Windows.Navigation;
using System;
using System.Text.RegularExpressions;
using System.Reflection;

namespace MoSync
{
    namespace NativeUI
    {
        //HorizintalLayout class
        public class HorizontalLayout : WidgetBaseWindowsPhone
        {
            //the Grid that will be used as the HorizontalLayout
            protected System.Windows.Controls.Grid mGrid;

            //padding information
            protected double mPaddingBottom;
            protected double mPaddingTop;
            protected double mPaddingLeft;
            protected double mPaddingRight;

            //row definitions used as spacers for the padding system
            protected RowDefinition mSpacerUp;
            protected RowDefinition mSpacerDown;

            //column definitions used as spacers for the padding system
            protected ColumnDefinition mSpacerLeft;
            protected ColumnDefinition mSpacerRight;

            //the main row of the Grid object (an horizontal layout is a grid with one row)
            protected RowDefinition mRowDef;

            //the inter widget spacer unit type
            protected GridUnitType mGridUnitType;

            //the inter widget spacer value
            protected int mGridUnitTypeValue;

            //boolean which indicates that the paddings are set by the user
            protected bool mPaddingSetByUser;

            //the constructor
            public HorizontalLayout()
            {
                mGrid = new System.Windows.Controls.Grid();

                mRowDef = new RowDefinition();
                mSpacerUp = new RowDefinition();
                mSpacerDown = new RowDefinition();

                mSpacerRight = new ColumnDefinition();
                mSpacerLeft = new ColumnDefinition();

                mRowDef.Height = new System.Windows.GridLength(1, System.Windows.GridUnitType.Star);

                mSpacerUp.Height = new System.Windows.GridLength(0);
                mSpacerDown.Height = new System.Windows.GridLength(0);
                mSpacerRight.Width = new System.Windows.GridLength(0);
                mSpacerLeft.Width = new System.Windows.GridLength(0);

                mGrid.RowDefinitions.Add(mSpacerUp);
                mGrid.RowDefinitions.Add(mRowDef);
                mGrid.RowDefinitions.Add(mSpacerDown);

                mGrid.ColumnDefinitions.Add(mSpacerLeft);
                mGrid.ColumnDefinitions.Add(mSpacerRight);

                mGridUnitType = GridUnitType.Auto;
                mGridUnitTypeValue = 1;

                this.fillSpaceVerticalyEnabled = false;
                this.fillSpaceHorizontalyEnabled = false;

                mPaddingSetByUser = false;

                mView = mGrid;

                mGrid.Margin = new Thickness(0);
//#if DEBUG
//              mGrid.ShowGridLines = true;
//#endif
            }

            //add child
            public override void AddChild(IWidget child)
            {
                base.AddChild(child);
                MoSync.Util.RunActionOnMainThreadSync(() =>
                {
                    WidgetBaseWindowsPhone widget = (child as WidgetBaseWindowsPhone);
                    ColumnDefinition columnDef = new ColumnDefinition(); //The column definition for the widget

                    //In order to be able to set various properties on the layout
                    //there appeared the need for two spacers, left spacer, and
                    //right spacer. For each widget one of each would be added
                    //to the layout.

                    ColumnDefinition leftSpacerColumnDef = new ColumnDefinition();
                    ColumnDefinition rightSpacerColumnDef = new ColumnDefinition();

                    if (widget.fillSpaceHorizontalyEnabled)
                    {
                        //Check if the paddings are not set by the user
                        if (false == mPaddingSetByUser)
                        {
                            setPaddingSpacers(0, GridUnitType.Pixel);
                        }
                        else
                        {
                            setPaddingSpacers(1, GridUnitType.Auto);
                        }
                        columnDef.Width = new System.Windows.GridLength(1, System.Windows.GridUnitType.Star);
                        mGridUnitType = GridUnitType.Pixel;
                        mGridUnitTypeValue = 0;
                    }
                    else if (double.IsNaN(widget.Width) == false) //Absolute value
                    {
                        columnDef.Width = new System.Windows.GridLength(widget.Width, System.Windows.GridUnitType.Pixel);
                    }
                    else //Wrapt content as default
                    {
                        //Check if the paddings are not set by the user
                        if (false == mPaddingSetByUser)
                        {
                            setPaddingSpacers(0, GridUnitType.Pixel);
                        }
                        columnDef.Width = new System.Windows.GridLength(1, System.Windows.GridUnitType.Auto);
                        mGridUnitType = GridUnitType.Pixel;
                        mGridUnitTypeValue = 0;
                    }

                    mGrid.ColumnDefinitions.Insert(mGrid.ColumnDefinitions.Count - 1, leftSpacerColumnDef);
                    mGrid.ColumnDefinitions.Insert(mGrid.ColumnDefinitions.Count - 1, columnDef);
                    mGrid.ColumnDefinitions.Insert(mGrid.ColumnDefinitions.Count - 1, rightSpacerColumnDef);

                    Grid.SetRow((widget.View as System.Windows.FrameworkElement), 1);
                    Grid.SetColumn((widget.View as System.Windows.FrameworkElement), mGrid.ColumnDefinitions.Count - 3);

                    if (widget.View is FrameworkElement)
                    {
                        (widget.View as FrameworkElement).HorizontalAlignment = this.mGrid.HorizontalAlignment;
                        (widget.View as FrameworkElement).VerticalAlignment = this.mGrid.VerticalAlignment;
                    }

                    mGrid.Children.Add(widget.View);

                    setSpacers(mGridUnitTypeValue, mGridUnitType);
                });
            }

            /**
             * The RemoveChild implementation
             * @param index int the index of the "child" widget that will be removed
             */
            public override void RemoveChild(int index)
            {
                if (0 <= index && mChildren.Count > index)
                {
                    IWidget child = mChildren[index];

                    if (null != child)
                    {
                        RemoveChild(child);
                    }
                }
            }

            /**
            * The RemoveChild implementation
            * @param child IWidget the "child" widget that will be removed
            */
            public override void RemoveChild(IWidget child)
            {
                MoSync.Util.RunActionOnMainThreadSync(() =>
                {
                    WidgetBaseWindowsPhone widget = (child as WidgetBaseWindowsPhone);
                    int x = Grid.GetColumn((widget.View) as System.Windows.FrameworkElement);
                    mGrid.ColumnDefinitions.RemoveAt(x + 1);
                    mGrid.ColumnDefinitions.RemoveAt(x);
                    mGrid.ColumnDefinitions.RemoveAt(x - 1);
                    mGrid.Children.Remove((child as WidgetBaseWindowsPhone).View);
                });
                base.RemoveChild(child);
            }

            //MAW_HORIZONTAL_LAYOUT_CHILD_HORIZONTAL_ALIGNMENT implementation
            [MoSyncWidgetProperty(MoSync.Constants.MAW_HORIZONTAL_LAYOUT_CHILD_HORIZONTAL_ALIGNMENT)]
            public String ChildHorizontalAlignment
            {
                set
                {
                    if (value.Equals(MoSync.Constants.MAW_ALIGNMENT_LEFT))
                    {
                        mGrid.HorizontalAlignment = HorizontalAlignment.Left;
                        mSpacerRight.Width = new GridLength(1, GridUnitType.Star);
                        mSpacerLeft.Width = new GridLength(0);

                        mPaddingSetByUser = true;
                    }
                    else if (value.Equals(MoSync.Constants.MAW_ALIGNMENT_RIGHT))
                    {
                        mGrid.HorizontalAlignment = HorizontalAlignment.Right;
                        mSpacerRight.Width = new GridLength(0);
                        mSpacerLeft.Width = new GridLength(1, GridUnitType.Star);

                        mPaddingSetByUser = true;
                    }
                    else if (value.Equals(MoSync.Constants.MAW_ALIGNMENT_CENTER))
                    {
                        mGrid.HorizontalAlignment = HorizontalAlignment.Center;
                        mSpacerRight.Width = new GridLength(1, GridUnitType.Star);
                        mSpacerLeft.Width = new GridLength(1, GridUnitType.Star);

                        for (int i = 0; i < this.mChildren.Count; i++)
                        {
                            if (true == (this.mChildren[i] as WidgetBaseWindowsPhone).fillSpaceHorizontalyEnabled)
                            {
                                mSpacerRight.Width = new GridLength(1, GridUnitType.Auto);
                                mSpacerLeft.Width = new GridLength(1, GridUnitType.Auto);
                                break;
                            }
                        }

                        mPaddingSetByUser = true;
                    }
                }
            }

            //MAW_HORIZONTAL_LAYOUT_CHILD_VERTICAL_ALIGNMENT implementation
            [MoSyncWidgetProperty(MoSync.Constants.MAW_HORIZONTAL_LAYOUT_CHILD_VERTICAL_ALIGNMENT)]
            public String ChildVerticalAlignment
            {
                set
                {
                    if (value.Equals(MoSync.Constants.MAW_ALIGNMENT_BOTTOM))
                    {
                        mGrid.VerticalAlignment = VerticalAlignment.Bottom;
                        mSpacerUp.Height = new GridLength(1, GridUnitType.Star);
                        mSpacerDown.Height = new GridLength(0);
                    }
                    else if (value.Equals(MoSync.Constants.MAW_ALIGNMENT_TOP))
                    {
                        mGrid.VerticalAlignment = VerticalAlignment.Top;

                        mSpacerDown.Height = new GridLength(1, GridUnitType.Star);
                        mSpacerUp.Height = new GridLength(0);
                    }
                    else if (value.Equals(MoSync.Constants.MAW_ALIGNMENT_CENTER))
                    {
                        mGrid.VerticalAlignment = VerticalAlignment.Center;

                        mSpacerDown.Height = new GridLength(1, GridUnitType.Auto);
                        mSpacerUp.Height = new GridLength(1, GridUnitType.Auto);
                    }
                }
            }

            //MAW_HORIZONTAL_LAYOUT_PADDING_BOTTOM implementation
            [MoSyncWidgetProperty(MoSync.Constants.MAW_HORIZONTAL_LAYOUT_PADDING_BOTTOM)]
            public String PaddingBottom
            {
                set
                {
                    double val;
                    if(Double.TryParse(value, out val))
                    {
                        mPaddingBottom = val;
                        mSpacerDown.Height = new GridLength(mPaddingBottom);
                    }
                }
            }

            //MAW_HORIZONTAL_LAYOUT_PADDING_TOP implementation
            [MoSyncWidgetProperty(MoSync.Constants.MAW_HORIZONTAL_LAYOUT_PADDING_TOP)]
            public String PaddingTop
            {
                set
                {
                    double val;
                    if (Double.TryParse(value, out val))
                    {
                        mPaddingTop = val;
                        mSpacerUp.Height = new GridLength(mPaddingTop);
                    }
                }
            }

            //MAW_HORIZONTAL_LAYOUT_PADDING_LEFT implementation
            [MoSyncWidgetProperty(MoSync.Constants.MAW_HORIZONTAL_LAYOUT_PADDING_LEFT)]
            public String PaddingLeft
            {
                set
                {
                    double val;
                    if (Double.TryParse(value, out val))
                    {
                        mPaddingLeft = val;
                        mSpacerLeft.Width = new GridLength(mPaddingLeft);

                        mPaddingSetByUser = true;
                    }
                }
            }

            //MAW_HORIZONTAL_LAYOUT_PADDING_RIGHT implementation
            [MoSyncWidgetProperty(MoSync.Constants.MAW_HORIZONTAL_LAYOUT_PADDING_RIGHT)]
            public String PaddingRight
            {
                set
                {
                    double val;
                    if (Double.TryParse(value, out val))
                    {
                        mPaddingRight = val;
                        mSpacerRight.Width = new GridLength(mPaddingRight);

                        mPaddingSetByUser = true;
                    }
                }
            }

            //Sets all the inter widget spacers to 0 or star
            public void setSpacers(int gridUnitValue, GridUnitType gridUnitType)
            {
                if(0 != mGrid.ColumnDefinitions[1].Width.Value || false == mGrid.ColumnDefinitions[1].Width.IsAbsolute)
                    mGrid.ColumnDefinitions[1].Width = new GridLength(0, GridUnitType.Pixel);
                if (0 != mGrid.ColumnDefinitions[mGrid.ColumnDefinitions.Count - 2].Width.Value || false == mGrid.ColumnDefinitions[mGrid.ColumnDefinitions.Count - 2].Width.IsAbsolute)
                    mGrid.ColumnDefinitions[mGrid.ColumnDefinitions.Count - 2].Width = new GridLength(0, GridUnitType.Pixel);

                int step = 0;

                for (int i = 3; i < mGrid.ColumnDefinitions.Count - 2; i++)
                {
                    step++;
                    if (2 >= step)
                    {
                        mGrid.ColumnDefinitions[i].Width = new GridLength(gridUnitValue, gridUnitType);
                    }
                    else step = 0;
                }
            }

            private void setPaddingSpacers(int gridUnitValue, GridUnitType gridUnitType)
            {
                mGrid.ColumnDefinitions[0].Width = new GridLength(gridUnitValue, gridUnitType);
                mGrid.ColumnDefinitions[mGrid.ColumnDefinitions.Count - 1].Width = new GridLength(gridUnitValue, gridUnitType);
            }

            private void writeStatus()
            {
                String s = "Horizontal ";
                for (int i = 0; i < mGrid.ColumnDefinitions.Count; i++)
                {
                    s += mGrid.ColumnDefinitions[i].Width.Value.ToString();
                    s += " ";
                    s += mGrid.ColumnDefinitions[i].Width.GridUnitType.ToString();
                    s += " | ";
                }
                MessageBox.Show(s);
            }
        }
    }
}
