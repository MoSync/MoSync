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
using System.Linq;

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

                View = mGrid;

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
                    InsertWidget(child, mStackPanels.Count, mGrid.RowDefinitions.Count - 1);
                });
            }

            /*
             * InsertChild function override
             * @param child The child that should be inserted into the grid.
             * @param index The index at which the child widget should be inserted.
             */
            public override void InsertChild(IWidget child, int index)
            {
                base.InsertChild(child, index);
                MoSync.Util.RunActionOnMainThreadSync(() =>
                {
                    InsertWidget(child, index, index + 1);
                });
            }

            /**
             * Inserts a widget into the visible structure. Handles the wrap/fill cases using StackPanels.
             * @param child The widget to be inserted.
             * @param sIndex The index at which the stack panel should be inserted inside the StackPanel array.
             * @param gIndex The index at which the grid row that will contain the widget should be inserted inside
             * the main grid.
             */
            private void InsertWidget(IWidget child, int sIndex, int gIndex)
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
                mStackPanels.Insert(sIndex, new StackPanel());

                int stackPanelIndex = sIndex;
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
                    mStackPanels[stackPanelIndex].Orientation = Orientation.Vertical;

                    // WRAP_CONT_V && WRAP_CONT_H
                    if (widget.WRAP_CONT_H)
                    {
                        mStackPanels[stackPanelIndex].Orientation = Orientation.Horizontal;
                    }
                }

                //Adding the children to the visible structure
                mGrid.RowDefinitions.Insert(gIndex, rowDef);

                ShiftWidgetRows(gIndex);

                if (stackPanelRequired)
                {
                    // If the stack panel container is required the widget gets added to that container
                    // and then this goes to the grid. Read above for the logical explanation.
                    mStackPanels[stackPanelIndex].Children.Add((widget.View as System.Windows.FrameworkElement));
                    Grid.SetColumn(mStackPanels[stackPanelIndex], 1);
                    Grid.SetRow(mStackPanels[stackPanelIndex], gIndex);

                    mGrid.Children.Add(mStackPanels[stackPanelIndex]);
                }
                else
                {
                    // If the stack panel container is not required the widget is added directly
                    // to the grid. Also the stack panel is added to the parent grid for future
                    // posible use. (in case the size policy changes after the child widget
                    // was added to the parent.
                    Grid.SetColumn((widget.View as FrameworkElement), 1);
                    Grid.SetRow(mStackPanels[stackPanelIndex], gIndex);
                    Grid.SetRow((widget.View as FrameworkElement), gIndex);

                    mGrid.Children.Add(mStackPanels[stackPanelIndex]);
                    mGrid.Children.Add(widget.View);
                }

                widget.RowNumber = gIndex;
                widget.ColumnNumber = 1;
            }

            /**
             * Shifts all the widget/stackpanel rows inside the current grid (starting from row gridIndex).
             * Used when inserting widgets into the main grid.
             * @param gridIndex The index of the first grid row that needs to be shifted.
             */
            private void ShiftWidgetRows(int gridIndex)
            {
                int columnIndex = 1;
                // the row at position mGrid.RowDefinitions.Count - 1 is a spacer row
                // the row at position mGrid.RowDefinitions.Count - 2 is a free row that will get filled
                // so we start at mGrid.RowDefinitions.Count - 3 - this is the first row where we need to shift
                // its content
                for (int i = mGrid.RowDefinitions.Count - 3; i >= gridIndex ; i--)
                {
                    // first get every control that is included in the current row
                    var controls = from d in mGrid.Children
                                   where Grid.GetColumn(d as FrameworkElement) == columnIndex
                                      && Grid.GetRow(d as FrameworkElement) == i
                                   select d;

                    // shift all the controls with one position
                    while (controls.Count() > 0)
                    {
                        int controlcount = controls.Count();
                        FrameworkElement control = controls.First() as FrameworkElement;
                        int controlRow = Grid.GetRow(control);
                        int newGridRow = controlRow + 1 < mGrid.RowDefinitions.Count - 1 ? controlRow + 1 : mGrid.RowDefinitions.Count - 2;
                        Grid.SetRow(control, newGridRow);
                    }

                    // go through all the widget and modify their row numbers
                    for (int j = 0; j < mChildren.Count; j++)
                    {
                        WidgetBaseWindowsPhone widget = mChildren[j] as WidgetBaseWindowsPhone;
                        if (widget.RowNumber == i)
                        {
                            int newGridRow = i + 1 < mGrid.RowDefinitions.Count - 1 ? i + 1 : mGrid.RowDefinitions.Count - 2;
                            widget.RowNumber = newGridRow;
                        }
                    }
                }
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

                    // in a vertical layout only column one is used for widgets (0 and 2 are padding columns)
                    int columnIndexToRemove = 1;
                    // the widget 'RowNumber' variable is always in sync with the widget position inside the grid
                    // so we'll take this as a reference (we can't use Grid.GetRow() method because
                    // the current widget might be inside a stack panel (so a row number isn't attached to its view)
                    int rowIndexToRemove = widget.RowNumber;
                    int rowCount = mGrid.RowDefinitions.Count;

                    if (rowCount > 0)
                    {
                        // first get every control from that row
                        var controls = from control in mGrid.Children
                                       where Grid.GetColumn(control as FrameworkElement) == columnIndexToRemove
                                          && Grid.GetRow(control as FrameworkElement) == rowIndexToRemove
                                       select control;

                        // remove all the controls present inside that grid cell
                        while (controls.Count() > 0)
                        {
                            FrameworkElement control = controls.First() as FrameworkElement;
                            mGrid.Children.Remove(control);
                        }

                        // then shift evey control after that index with one row
                        for (int i = rowIndexToRemove + 1; i < mGrid.RowDefinitions.Count; i++)
                        {
                            var currentControls = from d in mGrid.Children
                                                  where Grid.GetColumn(d as FrameworkElement) == columnIndexToRemove
                                                  && Grid.GetRow(d as FrameworkElement) == i
                                                  select d;
                            while (currentControls.Count() > 0)
                            {
                                FrameworkElement control = currentControls.First() as FrameworkElement;
                                int controlRow = Grid.GetRow(control);
                                Grid.SetRow(control, controlRow - 1 > 0 ? controlRow - 1 : 0);
                            }
                        }

                        // update all the widgets row numbers
                        for (int i = 0; i < mChildren.Count; i++)
                        {
                            WidgetBaseWindowsPhone currentWidget = mChildren[i] as WidgetBaseWindowsPhone;
                            if (currentWidget.RowNumber > rowIndexToRemove)
                            {
                                currentWidget.RowNumber--;
                            }
                        }

                        // lastly, remove the grid row
                        mGrid.RowDefinitions.RemoveAt(rowIndexToRemove);
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
                            View = mScrollViewer;
                        }
                        else
                        {
                            if (null != mScrollViewer)
                            {
                                View = mGrid;
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

            #region Property validation methods

            /**
             * Validates a property based on the property name and property value.
             * @param propertyName The name of the property to be checked.
             * @param propertyValue The value of the property to be checked.
             * @returns true if the property is valid, false otherwise.
             */
            public new static bool ValidateProperty(string propertyName, string propertyValue)
            {
                bool isPropertyValid = WidgetBaseWindowsPhone.ValidateProperty(propertyName, propertyValue);

                if (propertyName.Equals("childVerticalAlignment") || propertyName.Equals("childHorizontalAlignment"))
                {
                    int val;
                    if (!int.TryParse(propertyValue, out val))
                    {
                        isPropertyValid = false;
                    }

                    if (propertyName.Equals("childVerticalAlignment"))
                    {
                        if (!(propertyValue.Equals(MoSync.Constants.MAW_ALIGNMENT_BOTTOM) ||
                            propertyValue.Equals(MoSync.Constants.MAW_ALIGNMENT_TOP) ||
                            propertyValue.Equals(MoSync.Constants.MAW_ALIGNMENT_CENTER)))
                        {
                            isPropertyValid = false;
                        }
                    }
                    else if (propertyName.Equals("childHorizontalAlignment"))
                    {
                        if (!(propertyValue.Equals(MoSync.Constants.MAW_ALIGNMENT_LEFT) ||
                            propertyValue.Equals(MoSync.Constants.MAW_ALIGNMENT_RIGHT) ||
                            propertyValue.Equals(MoSync.Constants.MAW_ALIGNMENT_CENTER)))
                        {
                            isPropertyValid = false;
                        }
                    }
                }
                else if (propertyName.Equals("scrollable"))
                {
                    bool val;
                    if (!Boolean.TryParse(propertyValue, out val))
                    {
                        isPropertyValid = false;
                    }
                }

                return isPropertyValid;
            }

            #endregion
        } // end of VerticalLayout class
    } // end of NativeUI namespace
} // end of MoSync namespace