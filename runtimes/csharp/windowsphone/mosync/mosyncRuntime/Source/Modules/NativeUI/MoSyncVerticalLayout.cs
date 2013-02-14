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

            private System.Collections.Generic.List<StackPanel> mStackPanels;

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

            /**
             * The constructor
             */
            public VerticalLayout()
            {
                mGrid = new System.Windows.Controls.Grid();

                // The column definitions
                // The column that will contain the widgets
                mColDef = new ColumnDefinition();

                // The columns used for padding
                mSpacerRight = new ColumnDefinition();
                mSpacerLeft = new ColumnDefinition();

                // The rows used for padding
                mSpacerUp = new RowDefinition();
                mSpacerDown = new RowDefinition();

                // Setting the main column to star (fill the layout)
                mColDef.Width = new System.Windows.GridLength(1, System.Windows.GridUnitType.Star);

                // Setting the paddings to 0
                mSpacerUp.Height = new System.Windows.GridLength(0);
                mSpacerDown.Height = new System.Windows.GridLength(0);
                mSpacerRight.Width = new System.Windows.GridLength(0);
                mSpacerLeft.Width = new System.Windows.GridLength(0);

                // Add the top and bottom padding spacers
                mGrid.RowDefinitions.Add(mSpacerUp);
                mGrid.RowDefinitions.Add(mSpacerDown);

                // Add the left and right padding spacers. In the middle it goes the main column.
                mGrid.ColumnDefinitions.Add(mSpacerLeft);
                mGrid.ColumnDefinitions.Add(mColDef);
                mGrid.ColumnDefinitions.Add(mSpacerRight);

                mView = mGrid;

                mGrid.Margin = new Thickness(0.0);

                mStackPanels = new System.Collections.Generic.List<StackPanel>();

                setHorizontalSizePolicyFlags(true, false);
                setVerticalSizePolicyFlags(true, false);
//#if DEBUG
                //mGrid.ShowGridLines = true;
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
                    // The logic behind this layout system is to create a stack panel for each
                    // MoSync widget. The stack panel is used for wrapping and filling size policies.
                    // When the widget size policy states that it should fill on both horizontal and
                    // vertical axis the stack panel is not used in the structure but added to the
                    // grid for future usage.
                    //
                    // Wrap V & Fill H :
                    //  - row.height (1 x auto)
                    //  - column.width (1 x star)
                    //  - stack panel  (yes)
                    //  - orientation  (vertical)
                    //
                    // Wrap V & Wrap H :
                    //  - row.height (1 x auto)
                    //  - column.width (1 x star)
                    //  - stack panel  (yes)
                    //  - orientation  (horizontal)
                    //
                    // Fill V & Wrap H :
                    //  - row.height (1 x star)
                    //  - column.width (1 x star)
                    //  - stack panel  (yes)
                    //  - orientation  (horizontal)
                    //
                    // Fill V & Fill H :
                    //  - row.height (1 x star)
                    //  - column.width (1 x star)
                    //  - stack panel  (No)
                    //  - orientation  (none)
                    WidgetBaseWindowsPhone widget = (child as WidgetBaseWindowsPhone);
                    RowDefinition rowDef = new RowDefinition(); //The row definition for the widget

                    // Adding a new container
                    mStackPanels.Add(new StackPanel());

                    int stackPanelIndex = mStackPanels.Count - 1;
                    bool stackPanelRequired = true;

                    // The row for the widget has the default GridUnitType set on 1 x Auto
                    // meaning that it will "wrap" around the widget forcing it to wrap around
                    // its content on the vertical axis
                    rowDef.Height = new System.Windows.GridLength(1, System.Windows.GridUnitType.Auto);

                    if (widget.FILL_SPACE_V) //FillSpaceVerticaly
                    {
                        // The row for the widget gets the Height value set on 1 x Star
                        // meaning that it will "fill" the space available on the vertical axis
                        // forcing the widget to do the same
                        rowDef.Height = new System.Windows.GridLength(1, System.Windows.GridUnitType.Star);

                        // FILL_SPACE_V && FILL_SPACE_H
                        if (widget.FILL_SPACE_H)
                        {
                            // If this gets set on false the widget will not get added
                            // to the stack panel container for the time being.
                            stackPanelRequired = false;
                        }
                        // FILL_SPACE_V && WRAP_CONT_H
                        else if (widget.WRAP_CONT_H)
                        {
                            mStackPanels[stackPanelIndex].Orientation = Orientation.Horizontal;
                        }
                    }
                    // WRAP_CONT_V
                    else if (widget.WRAP_CONT_V)
                    {
                        mStackPanels[mStackPanels.Count - 1].Orientation = Orientation.Vertical;

                        // WRAP_CONT_V && WRAP_CONT_H
                        if (widget.WRAP_CONT_H)
                        {
                            mStackPanels[mStackPanels.Count - 1].Orientation = Orientation.Horizontal;
                        }
                    }

                    //Adding the children to the visible structure

                    mGrid.RowDefinitions.Insert(mGrid.RowDefinitions.Count - 1, rowDef);

                    if (stackPanelRequired)
                    {
                        // If the stack panel container is required the widget gets added to that container
                        // and then this goes to the grid. Read above for the logical explanation.
                        mStackPanels[stackPanelIndex].Children.Add((widget.View as System.Windows.FrameworkElement));
                        Grid.SetColumn(mStackPanels[stackPanelIndex], 1);
                        Grid.SetRow(mStackPanels[stackPanelIndex], mGrid.RowDefinitions.Count - 2);
                        mGrid.Children.Add(mStackPanels[stackPanelIndex]);
                    }
                    else
                    {
                        // If the stack panel container is not required the widget is added directly
                        // to the grid. Also the stack panel is added to the parent grid for future
                        // posible use. (in case the size policy changes after the child widget
                        // was added to the parent.
                        Grid.SetColumn((widget.View as FrameworkElement), 1);
                        Grid.SetRow(mStackPanels[stackPanelIndex], mGrid.RowDefinitions.Count - 2);
                        Grid.SetRow((widget.View as FrameworkElement), mGrid.RowDefinitions.Count - 2);
                        mGrid.Children.Add(mStackPanels[stackPanelIndex]);
                        mGrid.Children.Add(widget.View);
                    }

                    widget.RowNumber = mGrid.RowDefinitions.Count - 2;
                    widget.ColumnNumber = 1;
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
                    FrameworkElement fw = (widget.View) as System.Windows.FrameworkElement;

                    if (mGrid.Children.Contains(mStackPanels[widget.RowNumber - 1]))
                    {
                        if (RemoveWidgetFromStackPanelContainer(child, widget.RowNumber))
                        {
                            mGrid.Children.Remove(fw);
                            mGrid.Children.Remove(mStackPanels[widget.RowNumber - 1]);
                        }
                    }
                    if (mGrid.Children.Contains(fw))
                    {
                        int x = widget.RowNumber;

                        if (x < mGrid.RowDefinitions.Count)
                        {
                            mGrid.RowDefinitions.RemoveAt(x);
                            mGrid.Children.Remove(fw);
                        }
                    }
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

            /*
             * Sets the orientation of the stack panel container
             *
             * @param1: int index - the index of the row
             * @param2: Orientation orientation - the Orientation value
             */
            public void SetContainerOrientation(IWidget widget, int index, Orientation orientation)
            {
                // Take 1 away from index since the number of columns
                // is higher with 1 then the number of stack panel containers.
                // The extra row is the top padding.
                index -= 1;

                FrameworkElement fElem = (widget as WidgetBaseWindowsPhone).View as FrameworkElement;
                if (index < mStackPanels.Count && index >= 0)
                {
                    if (!mStackPanels[index].Children.Contains(fElem) &&
                       mStackPanels[index].Children.Count == 0 &&
                       mGrid.Children.Contains(fElem))
                    {
                        mGrid.Children.Remove(fElem);
                        mStackPanels[index].Children.Add(fElem);
                    }

                    mStackPanels[index].Orientation = orientation;
                }
            }

            /*
             * Removes the widget from the stack panel container. Required
             * when using FILL_SPACE_V && FILL_SPACE_H on a widget
             *
             * @param1: IWidget widget - the widget
             * @param2: int index - the index of the row
             */
            public bool RemoveWidgetFromStackPanelContainer(IWidget widget, int index)
            {
                // Take 1 away from index since the number of columns
                // is higher with 1 then the number of stack panel containers.
                // The extra row is the t top padding.
                index -= 1;
                FrameworkElement fElem = (widget as WidgetBaseWindowsPhone).View as FrameworkElement;
                if (mStackPanels[index].Children.Contains(fElem))
                {
                    mStackPanels[index].Children.Remove(fElem);
                    mGrid.Children.Add(fElem);
                    return true;
                }

                return false;
            }
        }
    }
}
