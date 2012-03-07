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
 * @file MoSyncVerticalLayout.cs
 * @author Ciprian Filipas
 *
 * @brief This represents the Vertical Layout Widget implementation for the NativeUI
 *        component on Windows Phone 7, language c#
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
        //VerticalLayout class
        public class VerticalLayout : WidgetBaseWindowsPhone
        {
            //The Grid that will be used as the VerticalLayout
            protected System.Windows.Controls.Grid mGrid;

            protected System.Windows.Controls.ScrollViewer mScrollViewer;

            //Padding information
            protected double mPaddingBottom;
            protected double mPaddingTop;
            protected double mPaddingLeft;
            protected double mPaddingRight;

            //Row definitions used as spacers for the padding system
            protected RowDefinition mSpacerUp;
            protected RowDefinition mSpacerDown;
            protected ColumnDefinition mSpacerLeft;
            protected ColumnDefinition mSpacerRight;

            //The main column of the Grid object (an vertical layout is a grid with one column)
            protected ColumnDefinition mColDef;

            //the inter widget spacer unit type
            protected GridUnitType mGridUnitType;

            //the inter widget spacer value
            protected int mGridUnitTypeValue;

            //boolean which indicates that the paddings are set by the user
            protected bool mPaddingSetByUser;

            /**
             * The constructor
             */
            public VerticalLayout()
            {
                mGrid = new System.Windows.Controls.Grid();

                mColDef = new ColumnDefinition();
                mSpacerUp = new RowDefinition();
                mSpacerDown = new RowDefinition();

                mSpacerRight = new ColumnDefinition();
                mSpacerLeft = new ColumnDefinition();

                mColDef.Width = new System.Windows.GridLength(1, System.Windows.GridUnitType.Star);

                mSpacerUp.Height = new System.Windows.GridLength(0);
                mSpacerDown.Height = new System.Windows.GridLength(0);
                mSpacerRight.Width = new System.Windows.GridLength(0);
                mSpacerLeft.Width = new System.Windows.GridLength(0);

                mGrid.RowDefinitions.Add(mSpacerUp);
                mGrid.RowDefinitions.Add(mSpacerDown);

                mGrid.ColumnDefinitions.Add(mSpacerLeft);
                mGrid.ColumnDefinitions.Add(mColDef);
                mGrid.ColumnDefinitions.Add(mSpacerRight);

                this.fillSpaceVerticalyEnabled = false;
                this.fillSpaceHorizontalyEnabled = false;

                mGridUnitType = GridUnitType.Auto;
                mGridUnitTypeValue = 1;

                mPaddingSetByUser = false;

                mView = mGrid;

                mGrid.Margin = new Thickness(0.0);
//#if DEBUG
//                mGrid.ShowGridLines = true;
//#endif
            }

            /**
             * AddChild function override
             * @param child IWidget the "child" widget that needs to be added
             */
            public override void AddChild(IWidget child)
            {
                base.AddChild(child);
                MoSync.Util.RunActionOnMainThreadSync(() =>
                {
                    WidgetBaseWindowsPhone widget = (child as WidgetBaseWindowsPhone);
                    RowDefinition rowDef = new RowDefinition(); //The row definition for the widget

                    //In order to be able to set various properties on the layout
                    //there appeared the need for two spacers, top spacer, and
                    //bottom spacer. For each widget one of each would be added
                    //to the layout.

                    RowDefinition upperSpacerRowDef = new RowDefinition(); //The row definition for the upper spacer
                    RowDefinition bottomSpacerRowDef = new RowDefinition(); //The row definition for the bottom spacer

                    if (widget.fillSpaceVerticalyEnabled) //FillSpaceVerticaly
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
                        rowDef.Height = new System.Windows.GridLength(1, System.Windows.GridUnitType.Star);
                        mGridUnitType = GridUnitType.Pixel;
                        mGridUnitTypeValue = 0;
                    }
                    else if(double.IsNaN( widget.Height ) == false) //Absolute value
                    {
                        rowDef.Height = new System.Windows.GridLength(widget.Height, System.Windows.GridUnitType.Pixel);
                    }
                    else //Wrap content as default
                    {
                        //Check if the paddings are not set by the user
                        if (false == mPaddingSetByUser)
                        {
                            setPaddingSpacers(0, GridUnitType.Pixel);
                        }
                        rowDef.Height = new System.Windows.GridLength(1, System.Windows.GridUnitType.Auto);
                        mGridUnitType = GridUnitType.Pixel;
                        mGridUnitTypeValue = 0;
                    }

                    mGrid.RowDefinitions.Insert(mGrid.RowDefinitions.Count - 1, upperSpacerRowDef);
                    mGrid.RowDefinitions.Insert(mGrid.RowDefinitions.Count - 1, rowDef);
                    mGrid.RowDefinitions.Insert(mGrid.RowDefinitions.Count - 1, bottomSpacerRowDef);

                    Grid.SetColumn((widget.View as System.Windows.FrameworkElement), 1);
                    Grid.SetRow((widget.View as System.Windows.FrameworkElement), mGrid.RowDefinitions.Count - 3);

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
                    int x = Grid.GetRow((widget.View) as System.Windows.FrameworkElement);
                    mGrid.RowDefinitions.RemoveAt(x + 1);
                    mGrid.RowDefinitions.RemoveAt(x);
                    mGrid.RowDefinitions.RemoveAt(x - 1);
                    mGrid.Children.Remove((child as WidgetBaseWindowsPhone).View);
                });
                base.RemoveChild(child);
            }

            /**
             * MAW_VERTICAL_LAYOUT_CHILD_HORIZONTAL_ALIGNMENT implementation
             */
            [MoSyncWidgetProperty(MoSync.Constants.MAW_VERTICAL_LAYOUT_CHILD_HORIZONTAL_ALIGNMENT)]
            public String ChildHorizontalAlignment
            {
                set
                {
                    if (value.Equals(MoSync.Constants.MAW_ALIGNMENT_LEFT))
                    {
                        mGrid.HorizontalAlignment = HorizontalAlignment.Left;
                        mSpacerRight.Width = new GridLength(1, GridUnitType.Star);
                        mSpacerLeft.Width = new GridLength(0);
                    }
                    else if (value.Equals(MoSync.Constants.MAW_ALIGNMENT_RIGHT))
                    {
                        mGrid.HorizontalAlignment = HorizontalAlignment.Right;
                        mSpacerRight.Width = new GridLength(0);
                        mSpacerLeft.Width = new GridLength(1, GridUnitType.Star);
                    }
                    else if (value.Equals(MoSync.Constants.MAW_ALIGNMENT_CENTER))
                    {
                        mGrid.HorizontalAlignment = HorizontalAlignment.Center;
                        mSpacerRight.Width = new GridLength(1, GridUnitType.Auto);
                        mSpacerLeft.Width = new GridLength(1, GridUnitType.Auto);
                    }
                    else throw new InvalidPropertyValueException();
                }
            }

            /**
             * MAW_VERTICAL_LAYOUT_CHILD_VERTICAL_ALIGNMENT implementation
             */
            [MoSyncWidgetProperty(MoSync.Constants.MAW_VERTICAL_LAYOUT_CHILD_VERTICAL_ALIGNMENT)]
            public String ChildVerticalAlignment
            {
                set
                {
                    if (value.Equals(MoSync.Constants.MAW_ALIGNMENT_BOTTOM))
                    {
                        mGrid.VerticalAlignment = VerticalAlignment.Bottom;
                        mSpacerUp.Height = new GridLength(1, GridUnitType.Star);
                        mSpacerDown.Height = new GridLength(0);

                        mPaddingSetByUser = true;
                    }
                    else if (value.Equals(MoSync.Constants.MAW_ALIGNMENT_TOP))
                    {
                        mGrid.VerticalAlignment = VerticalAlignment.Top;
                        mSpacerDown.Height = new GridLength(1, GridUnitType.Star);
                        mSpacerUp.Height = new GridLength(0);

                        mPaddingSetByUser = true;
                    }
                    else if (value.Equals(MoSync.Constants.MAW_ALIGNMENT_CENTER))
                    {
                        mGrid.VerticalAlignment = VerticalAlignment.Center;
                        mSpacerDown.Height = new GridLength(1, GridUnitType.Auto);
                        mSpacerUp.Height = new GridLength(1, GridUnitType.Auto);

                        mPaddingSetByUser = true;
                    }
                    else throw new InvalidPropertyValueException();
                }
            }

            /**
             * MAW_VERTICAL_LAYOUT_PADDING_BOTTOM implementation
             */
            [MoSyncWidgetProperty(MoSync.Constants.MAW_VERTICAL_LAYOUT_PADDING_BOTTOM)]
            public double PaddingBottom
            {
                set
                {
                    mSpacerDown.Height = new GridLength(value);
                    mPaddingSetByUser = true;
                }
            }

            /**
             * MAW_VERTICAL_LAYOUT_PADDING_TOP implementation
             */
            [MoSyncWidgetProperty(MoSync.Constants.MAW_VERTICAL_LAYOUT_PADDING_TOP)]
            public double PaddingTop
            {
                set
                {
                    mSpacerUp.Height = new GridLength(value);
                    mPaddingSetByUser = true;
                }
            }

            /**
             * MAW_VERTICAL_LAYOUT_PADDING_LEFT implementation
             */
            [MoSyncWidgetProperty(MoSync.Constants.MAW_VERTICAL_LAYOUT_PADDING_LEFT)]
            public double PaddingLeft
            {
                set
                {
                    mSpacerLeft.Width = new GridLength(value);
                }
            }

            /**
             * MAW_VERTICAL_LAYOUT_PADDING_RIGHT implementation
             */
            [MoSyncWidgetProperty(MoSync.Constants.MAW_VERTICAL_LAYOUT_PADDING_RIGHT)]
            public double PaddingRight
            {
                set
                {
                    mSpacerRight.Width = new GridLength(value);
                }
            }

            /**
             * MAW_VERTICAL_LAYOUT_SCROLLABLE implementation
             */
            [MoSyncWidgetProperty(MoSync.Constants.MAW_VERTICAL_LAYOUT_SCROLLABLE)]
            public string Scrollable
            {
                set
                {
                    bool val;
                    if (Boolean.TryParse(value, out val))
                    {
                        if (true == val)
                        {
                            mScrollViewer = new System.Windows.Controls.ScrollViewer();
                            mScrollViewer.Content = mGrid;
                            mView = mScrollViewer;
                        }
                        else
                        {
                            if (null != mScrollViewer)
                            {
                                mView = mGrid;
                            }
                        }
                    }
                    else throw new InvalidPropertyValueException();
                }
            }

            //Sets all the inter widget spacers to 0, star or auto
            private void setSpacers(int gridUnitValue, GridUnitType gridUnitType)
            {
                if (0 != mGrid.RowDefinitions[1].Height.Value || false == mGrid.RowDefinitions[1].Height.IsAbsolute)
                    mGrid.RowDefinitions[1].Height = new GridLength(0, GridUnitType.Pixel);
                if (0 != mGrid.RowDefinitions[mGrid.RowDefinitions.Count - 2].Height.Value || false == mGrid.RowDefinitions[mGrid.RowDefinitions.Count - 2].Height.IsAbsolute)
                    mGrid.RowDefinitions[mGrid.RowDefinitions.Count - 2].Height = new GridLength(0, GridUnitType.Pixel);

                int step = 0;

                for (int i = 3; i < mGrid.RowDefinitions.Count - 2; i++)
                {
                    step++;
                    if (2 >= step)
                    {
                        mGrid.RowDefinitions[i].Height = new GridLength(gridUnitValue, gridUnitType);
                    }
                    else step = 0;
                }
            }

            private void setPaddingSpacers(int gridUnitValue, GridUnitType gridUnitType)
            {
                mGrid.RowDefinitions[0].Height = new GridLength(gridUnitValue, gridUnitType);
                mGrid.RowDefinitions[mGrid.RowDefinitions.Count - 1].Height = new GridLength(gridUnitValue, gridUnitType);
            }
        }
    }
}
