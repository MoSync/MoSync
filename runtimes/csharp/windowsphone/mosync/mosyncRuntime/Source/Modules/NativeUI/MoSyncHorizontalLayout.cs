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
            protected ColumnDefinition mSpacerLeft;
            protected ColumnDefinition mSpacerRight;

            //the main row of the Grid object (an horizontal layout is a grid with one row)
            protected RowDefinition mRowDef;

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

                mView = mGrid;
#if DEBUG
                mGrid.ShowGridLines = true;
#endif
            }

            //add child
            public override void AddChild(IWidget child)
            {
                base.AddChild(child);
                MoSync.Util.RunActionOnMainThreadSync(() =>
                {
                    WidgetBaseWindowsPhone widget = (child as WidgetBaseWindowsPhone);
                    ColumnDefinition columnDef = new ColumnDefinition();

                    if (widget.fillSpaceHorizontalyEnabled) columnDef.Width = new System.Windows.GridLength(1, System.Windows.GridUnitType.Star);
                    else columnDef.Width = new System.Windows.GridLength(1, System.Windows.GridUnitType.Auto);

                    mGrid.ColumnDefinitions.Insert(mGrid.ColumnDefinitions.Count - 1, columnDef);

                    mGrid.Children.Add(widget.View);

                    Grid.SetColumn((widget.View as System.Windows.FrameworkElement), mGrid.ColumnDefinitions.Count - 2);
                    Grid.SetRow((widget.View as System.Windows.FrameworkElement), 1);
                });
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
                        mSpacerRight.Width = new GridLength(1, GridUnitType.Star);
                        mSpacerLeft.Width = new GridLength(1, GridUnitType.Star);
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
                        mSpacerDown.Height = new GridLength(1, GridUnitType.Star);
                        mSpacerUp.Height = new GridLength(1, GridUnitType.Star);
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
                    }
                }
            }
        }
    }
}
