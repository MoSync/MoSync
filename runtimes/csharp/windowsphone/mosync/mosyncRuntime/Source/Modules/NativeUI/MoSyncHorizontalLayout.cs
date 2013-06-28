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
using System.Linq;

namespace MoSync
{
    namespace NativeUI
    {
        //HorizintalLayout class
        public class HorizontalLayout : WidgetBaseWindowsPhone
        {
            //the Grid that will be used as the HorizontalLayout
            protected System.Windows.Controls.Grid mGrid;

            private System.Collections.Generic.List<StackPanel> mStackPanels;

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

            /**
             * The constructor
             */
            public HorizontalLayout()
            {
                mGrid = new System.Windows.Controls.Grid();

                // The row definitions
                // The row that will contain the widgets
                mRowDef = new RowDefinition();

                // The rows used for padding
                mSpacerUp = new RowDefinition();
                mSpacerDown = new RowDefinition();

                // The columns used for padding
                mSpacerRight = new ColumnDefinition();
                mSpacerLeft = new ColumnDefinition();

                // Setting the main row to star (fill the layout)
                mRowDef.Height = new System.Windows.GridLength(1, System.Windows.GridUnitType.Star);

                // Setting the paddings to 0
                mSpacerUp.Height = new System.Windows.GridLength(0);
                mSpacerDown.Height = new System.Windows.GridLength(0);
                mSpacerRight.Width = new System.Windows.GridLength(0);
                mSpacerLeft.Width = new System.Windows.GridLength(0);

                // Add the left and right padding spacers
                mGrid.ColumnDefinitions.Add(mSpacerLeft);
                mGrid.ColumnDefinitions.Add(mSpacerRight);

                // Add the top and bottom padding spacers. In the middle it goes the main row.
                mGrid.RowDefinitions.Add(mSpacerUp);
                mGrid.RowDefinitions.Add(mRowDef);
                mGrid.RowDefinitions.Add(mSpacerDown);

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
                    InsertWidget(child, mStackPanels.Count, mGrid.ColumnDefinitions.Count - 1);
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
             * @param gIndex The index at which the grid column that will contain the widget should be inserted inside
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
                //  - column.width (1 x star)
                //  - row.Height (1 x star)
                //  - stack panel  (yes)
                //  - orientation  (vertical)
                //
                // Wrap V & Wrap H :
                //  - column.width (1 x auto)
                //  - row.Height (1 x star)
                //  - stack panel  (yes)
                //  - orientation  (vertical)
                //
                // Fill V & Wrap H :
                //  - column.width (1 x auto)
                //  - row.Height (1 x star)
                //  - stack panel  (yes)
                //  - orientation  (horizontal)
                //
                // Fill V & Fill H :
                //  - column.width (1 x star)
                //  - row.Height (1 x star)
                //  - stack panel  (No)
                //  - orientation  (none)
                WidgetBaseWindowsPhone widget = (child as WidgetBaseWindowsPhone);
                ColumnDefinition columnDef = new ColumnDefinition(); //The column definition for the widget

                // Adding a new container
                mStackPanels.Insert(sIndex, new StackPanel());

                // by default, the stack panel orientation is Vertical (inside a horizontal layout it should
                // be Horizontal
                mStackPanels[sIndex].Orientation = Orientation.Horizontal;

                bool stackPanelRequired = true;

                // The column for the widget has the default GridUnitType set on 1 x Auto
                // meaning that it will "wrap" around the widget forcing it to wrap around
                // its content on the horizontal axis
                columnDef.Width = new System.Windows.GridLength(1, System.Windows.GridUnitType.Auto);

                if (widget.FILL_SPACE_H) //Fill space Horizontally
                {
                    // The column for the widget gets the Width value set on 1 x Star
                    // meaning that it will "fill" the space available on the horizontal
                    // axis forcing the widget to do the same
                    columnDef.Width = new System.Windows.GridLength(1, System.Windows.GridUnitType.Star);

                    // FILL_SPACE_V && FILL_SPACE_H
                    if (widget.FILL_SPACE_V)
                    {
                        // If this gets set on false the widget will not get added
                        // to the stack panel container for the time being.
                        stackPanelRequired = false;
                    }
                    // FILL_SPACE_H && WRAP_CONT_V
                    else if (widget.WRAP_CONT_V)
                    {
                        mStackPanels[sIndex].Orientation = Orientation.Vertical;
                    }
                }
                // WRAP_CONT_H
                else if (widget.WRAP_CONT_H)
                {
                    mStackPanels[sIndex].Orientation = Orientation.Vertical;
                    // WRAP_CONT_V && WRAP_CONT_H
                    if (widget.FILL_SPACE_V)
                    {
                        mStackPanels[sIndex].Orientation = Orientation.Horizontal;
                    }
                }

                //Adding the children to the visible structure
                mGrid.ColumnDefinitions.Insert(gIndex, columnDef);

                ShiftWidgetColumns(gIndex);

                if (stackPanelRequired)
                {
                    // If the stack panel container is required the widget gets added to that container
                    // and then this goes to the grid. Read above for the logical explanation.
                    mStackPanels[sIndex].Children.Add((widget.View as System.Windows.FrameworkElement));
                    Grid.SetRow(mStackPanels[sIndex], 1);
                    Grid.SetColumn(mStackPanels[sIndex], gIndex);

                    mGrid.Children.Add(mStackPanels[sIndex]);
                }
                else
                {
                    // If the stack panel container is not required the widget is added directly
                    // to the grid. Also the stack panel is added to the parent grid for future
                    // posible use. (in case the size policy changes after the child widget
                    // was added to the parent.
                    Grid.SetRow((widget.View as FrameworkElement), 1);
                    Grid.SetColumn(mStackPanels[sIndex], gIndex);
                    Grid.SetColumn((widget.View as FrameworkElement), gIndex);

                    mGrid.Children.Add(mStackPanels[sIndex]);
                    mGrid.Children.Add(widget.View);
                }

                widget.ColumnNumber = gIndex;
                widget.RowNumber = 1;
            }

            /**
             * Shifts all the widget/stackpanel columns inside the current grid (starting from column gridIndex).
             * Used when inserting widgets into the main grid.
             * @param gridIndex The index of the first grid column that needs to be shifted.
             */
            private void ShiftWidgetColumns(int gridIndex)
            {
                int rowIndex = 1;
                // the column at position mGrid.ColumnDefinitions.Count - 1 is a spacer column
                // the column at position mGrid.ColumnDefinitions.Count - 2 is a free column that will get filled
                // so we start at mGrid.ColumnDefinitions.Count - 3 - this is the first column where we need to shift
                // its content
                int columnds = mGrid.ColumnDefinitions.Count;
                for (int i = mGrid.ColumnDefinitions.Count - 3; i >= gridIndex; i--)
                {
                    // first get every control that is included in the current column
                    var controls = from d in mGrid.Children
                                   where Grid.GetColumn(d as FrameworkElement) == i
                                      && Grid.GetRow(d as FrameworkElement) == rowIndex
                                   select d;

                    // shift all the controls with one position
                    while (controls.Count() > 0)
                    {
                        int controlcount = controls.Count();
                        FrameworkElement control = controls.First() as FrameworkElement;
                        int controlColumn = Grid.GetColumn(control);
                        int newGridColumn = controlColumn + 1 < mGrid.ColumnDefinitions.Count - 1
                            ? controlColumn + 1 : mGrid.ColumnDefinitions.Count - 2;
                        Grid.SetColumn(control, newGridColumn);
                    }

                    // go through all the widget and modify their column numbers
                    for (int j = 0; j < mChildren.Count; j++)
                    {
                        WidgetBaseWindowsPhone widget = mChildren[j] as WidgetBaseWindowsPhone;
                        if (widget.ColumnNumber == i)
                        {
                            int newGridColumn = i + 1 < mGrid.ColumnDefinitions.Count - 1
                                ? i + 1 : mGrid.ColumnDefinitions.Count - 2;
                            widget.ColumnNumber = newGridColumn;
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

                    // in a horizontal layout only row one is used for widgets (0 and 2 are padding rows)
                    int rowIndexToRemove = 1;
                    // the widget 'ColumnNumber' variable is always in sync with the widget position inside the grid
                    // so we'll take this as a reference (we can't use Grid.GetColumn() method because
                    // the current widget might be inside a stack panel (so a column number isn't attached to its view)
                    int columnIndexToRemove = widget.ColumnNumber;
                    int columnCount = mGrid.ColumnDefinitions.Count;

                    if (columnCount > 0)
                    {
                        // first get every control from that column
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

                        // then shift evey control after that index with one column
                        for (int i = columnIndexToRemove + 1; i < mGrid.ColumnDefinitions.Count; i++)
                        {
                            var currentControls = from d in mGrid.Children
                                                  where Grid.GetColumn(d as FrameworkElement) == i
                                                  && Grid.GetRow(d as FrameworkElement) == rowIndexToRemove
                                                  select d;
                            while (currentControls.Count() > 0)
                            {
                                FrameworkElement control = currentControls.First() as FrameworkElement;
                                int controlColumn = Grid.GetColumn(control);
                                Grid.SetColumn(control, controlColumn - 1 > 0 ? controlColumn - 1 : 0);
                            }
                        }

                        // update all the widgets column numbers
                        for (int i = 0; i < mChildren.Count; i++)
                        {
                            WidgetBaseWindowsPhone currentWidget = mChildren[i] as WidgetBaseWindowsPhone;
                            if (currentWidget.ColumnNumber > columnIndexToRemove)
                            {
                                currentWidget.ColumnNumber--;
                            }
                        }

                        // lastly, remove the grid column
                        mGrid.ColumnDefinitions.RemoveAt(columnIndexToRemove);
                    }
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

                        for (int i = 0; i < this.mChildren.Count; i++)
                        {
                            if (true == (this.mChildren[i] as WidgetBaseWindowsPhone).FILL_SPACE_H)
                            {
                                mSpacerRight.Width = new GridLength(1, GridUnitType.Auto);
                                mSpacerLeft.Width = new GridLength(1, GridUnitType.Auto);
                                break;
                            }
                        }
                    }
                    else throw new InvalidPropertyValueException();
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
                    else throw new InvalidPropertyValueException();
                }
            }

            //MAW_HORIZONTAL_LAYOUT_PADDING_BOTTOM implementation
            [MoSyncWidgetProperty(MoSync.Constants.MAW_HORIZONTAL_LAYOUT_PADDING_BOTTOM)]
            public double PaddingBottom
            {
                set
                {
                    mSpacerDown.Height = new GridLength(value);
                }
            }

            //MAW_HORIZONTAL_LAYOUT_PADDING_TOP implementation
            [MoSyncWidgetProperty(MoSync.Constants.MAW_HORIZONTAL_LAYOUT_PADDING_TOP)]
            public double PaddingTop
            {
                set
                {
                    mSpacerUp.Height = new GridLength(value);
                }
            }

            //MAW_HORIZONTAL_LAYOUT_PADDING_LEFT implementation
            [MoSyncWidgetProperty(MoSync.Constants.MAW_HORIZONTAL_LAYOUT_PADDING_LEFT)]
            public double PaddingLeft
            {
                set
                {
                    mSpacerLeft.Width = new GridLength(value);
                }
            }

            //MAW_HORIZONTAL_LAYOUT_PADDING_RIGHT implementation
            [MoSyncWidgetProperty(MoSync.Constants.MAW_HORIZONTAL_LAYOUT_PADDING_RIGHT)]
            public double PaddingRight
            {
                set
                {
                    mSpacerRight.Width = new GridLength(value);
                }
            }

            /*
             * Sets the orientation of the stack panel container
             *
             * @param1: int index - the index of the column
             * @param2: Orientation orientation - the Orientation value
             */
            public void SetContainerOrientation(IWidget widget, int index, Orientation orientation)
            {
                // Take 1 away from index since the number of columns
                // is higher with 1 then the number of stack panel containers.
                // The extra column is the left padding.
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
             * @param2: int index - the index of the column
             */
            public bool RemoveWidgetFromStackPanelContainer(IWidget widget, int index)
            {
                // Take 1 away from index since the number of columns
                // is higher with 1 then the number of stack panel containers.
                // The extra column is the left padding.
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

                if (propertyName.Equals("childHorizontalAlignment"))
                {
                    if (!(propertyValue.Equals(MoSync.Constants.MAW_ALIGNMENT_LEFT) ||
                        propertyValue.Equals(MoSync.Constants.MAW_ALIGNMENT_RIGHT) ||
                        propertyValue.Equals(MoSync.Constants.MAW_ALIGNMENT_CENTER)))
                    {
                        isPropertyValid = false;
                    }
                }
                else if (propertyName.Equals("childVerticalAlignment"))
                {
                    if (!(propertyValue.Equals(MoSync.Constants.MAW_ALIGNMENT_BOTTOM) ||
                        propertyValue.Equals(MoSync.Constants.MAW_ALIGNMENT_TOP) ||
                        propertyValue.Equals(MoSync.Constants.MAW_ALIGNMENT_CENTER)))
                    {
                        isPropertyValid = false;
                    }
                }

                return isPropertyValid;
            }

            #endregion
        } // end of HorizontalLayout class
    } // end of NativeUI namespace
} // end of MoSync namespace
