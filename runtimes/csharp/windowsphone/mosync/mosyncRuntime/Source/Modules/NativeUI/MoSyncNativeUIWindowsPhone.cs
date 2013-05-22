﻿/* Copyright (C) 2011 MoSync AB

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
 * @file MoSyncNativeUIWindowsPhone.cs
 * @author Niklas Nummelin, Ciprian Filipas
 *
 * @brief This represents the WidgetBaseWindowsPhone implementation for the NativeUI
 *         component on Windows Phone 7, the base class for all the custom MoSync Widgets,
 *         language c#
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
using System.Collections.Generic;
using System.Threading;

namespace MoSync
{
    namespace NativeUI
    {
        public class WidgetBaseWindowsPhone : WidgetBase
        {
            /**
             * The basic widget view. Every UI widget inherits from UIElement.
             */
            private UIElement mView;

            //Width
            protected double mWidth;

            //Height
            protected double mHeight;

            #region Constructor

            public WidgetBaseWindowsPhone()
                : base()
            {
                setHorizontalSizePolicyFlags(false, true);
                setVerticalSizePolicyFlags(false, true);
                RowNumber = -1;
                ColumnNumber = -1;
            }

            #endregion

            #region View property

            public UIElement View
            {
                get { return mView; }
                set
                {
                    mView = value;
                }
            }

            #endregion

            #region Widget operation queue methods

            /**
             * Runs all the operations from the operation queue. Must be run on the UI thread.
             */
            public new void RunOperationQueue()
            {
                // run all the pending operations from the widget operation queue
                while (mOperationQueue.Count > 0)
                {
                    WidgetOperation currentOperation = mOperationQueue.Dequeue();
                    RunOperation(currentOperation);
                }
            }

            /**
             * Runs a WidgetOperation on the current widget.
             * @param operation The widget operation (could be a ADD, INSERT, REMOVE, SET or GET) that
             * needs to be applied on the current widget.
             */
            public new void RunOperation(WidgetOperation operation)
            {
                switch (operation.Type)
                {
                    case WidgetOperation.OperationType.SET:
                        SetProperty(operation);
                        break;
                    case WidgetOperation.OperationType.ADD:
                        AddChild(operation);
                        break;
                    case WidgetOperation.OperationType.INSERT:
                        InsertChild(operation);
                        break;
                    case WidgetOperation.OperationType.REMOVE:
                        RemoveChild(operation);
                        break;
                    case WidgetOperation.OperationType.GET:
                        // do nothing - when a get syscall is called and the widget is not created,
                        // the last valid set value is returned and if it doesn't exist, we wait for
                        // the widget creation and run the get operation. A get operation never
                        // gets into the queue
                        break;
                }
            }

            /**
             * Runs a set property operation on the current widget.
             * @param operation The widget operation that needs to be applied on the current widget.
             */
            protected void SetProperty(WidgetOperation operation)
            {
                PropertyInfo pinfo;
                MoSyncWidgetPropertyAttribute pattr = GetPropertyAttribute(operation.Property, out pinfo);
                Exception exception = null;
                try
                {
                    SetProperty(pinfo, operation.Value);
                }
                catch (Exception e)
                {
                    exception = e;
                }
                if (null != exception)
                    if (exception.InnerException is InvalidPropertyValueException)
                        throw new InvalidPropertyValueException();
            }

            /**
             * Runs a add child operation on the current widget.
             * @param operation The widget operation that needs to be applied on the current widget.
             */
            protected void AddChild(WidgetOperation operation)
            {
                IWidget child = mRuntime.GetModule<NativeUIModule>().GetWidgetSync(operation.Handle);
                child.SetParent(this);
                this.AddChild(child);
            }

            /**
             * Runs a insert child operation on the current widget.
             * @param operation The widget operation that needs to be applied on the current widget.
             */
            protected void InsertChild(WidgetOperation operation)
            {
                IWidget child = mRuntime.GetModule<NativeUIModule>().GetWidgetSync(operation.Handle);
                child.SetParent(this);
                this.InsertChild(child, operation.Index);
            }

            /**
             * Runs a remove child operation on the current widget.
             * @param operation The widget operation that needs to be applied on the current widget.
             */
            protected void RemoveChild(WidgetOperation operation)
            {
                IWidget child = mRuntime.GetModule<NativeUIModule>().GetWidgetSync(operation.Handle);
                child.RemoveFromParent();
            }

            #endregion

            #region Properties and methods for setting/getting FILL/WRAP and setting the detault FILL/WRAP values

            //Size policies
            public bool FILL_SPACE_V
            {
                get;
                protected set;
            }

            public bool FILL_SPACE_H
            {
                get;
                protected set;
            }

            public bool WRAP_CONT_V
            {
                get;
                protected set;
            }

            public bool WRAP_CONT_H
            {
                get;
                protected set;
            }

            // The position inside the parent grid.
            // Default values -1

            public int RowNumber
            {
                get;
                set;
            }

            public int ColumnNumber
            {
                get;
                set;
            }

            protected void setHorizontalSizePolicyFlags(bool fillSpaceHorizontally, bool wrapContentHorizontally)
            {
                FILL_SPACE_H =  fillSpaceHorizontally;
                WRAP_CONT_H = wrapContentHorizontally;
            }

            protected void setVerticalSizePolicyFlags(bool fillSpaceVertically, bool wrapContentVertically)
            {
                FILL_SPACE_V = fillSpaceVertically;
                WRAP_CONT_V = wrapContentVertically;
            }

            #endregion

            #region Basic widget properties

            /**
             * MAW_WIDGET_LEFT implementation
             */
            [MoSyncWidgetProperty(MoSync.Constants.MAW_WIDGET_LEFT)]
            public double Left
            {
                get { return (double)mView.GetValue(Canvas.LeftProperty); }
                set
                {
                    mView.SetValue(Canvas.LeftProperty, value);
                }
            }

            /**
             * MAW_WIDGET_TOP implementation
             */
            [MoSyncWidgetProperty(MoSync.Constants.MAW_WIDGET_TOP)]
            public double Top
            {
                get { return (double)mView.GetValue(Canvas.TopProperty); }
                set
                {
                    mView.SetValue(Canvas.TopProperty, value);
                }
            }

            /**
             * MAW_WIDGET_WIDTH implementation
             */
            [MoSyncWidgetProperty(MoSync.Constants.MAW_WIDGET_WIDTH)]
            public double Width
            {
                get { return (double)mView.GetValue(Canvas.WidthProperty); }
                set
                {
                    if (value != mWidth)
                    {
                        if (0 <= value)
                        {
                            mView.SetValue(Canvas.WidthProperty, value);
                            mWidth = value;
                            setHorizontalSizePolicyFlags(false, false);
                        }
                        else if (MoSync.Constants.MAW_CONSTANT_FILL_AVAILABLE_SPACE == value)
                        {
                            FillSpaceHorizontally();
                        }
                        else if (MoSync.Constants.MAW_CONSTANT_WRAP_CONTENT == value)
                        {
                            WrapContentHorizontally();
                        }
                    }
                }
            }

            /**
             * MAW_WIDGET_HEIGHT implementation
             */
            [MoSyncWidgetProperty(MoSync.Constants.MAW_WIDGET_HEIGHT)]
            public double Height
            {
                get { return (double)mView.GetValue(Canvas.HeightProperty); }
                set
                {
                    if (value != mHeight)
                    {
                        if (0 <= value)
                        {
                            mView.SetValue(Canvas.HeightProperty, value);
                            mHeight = value;
                            setVerticalSizePolicyFlags(false, false);
                        }
                        else if (MoSync.Constants.MAW_CONSTANT_FILL_AVAILABLE_SPACE == value)
                        {
                            FillSpaceVertically();
                        }
                        else if (MoSync.Constants.MAW_CONSTANT_WRAP_CONTENT == value)
                        {
                            WrapContentVertically();
                        }
                    }
                }
            }

            /**
             * MAW_WIDGET_BACKGROUND_COLOR implementation
             */
            [MoSyncWidgetProperty(MoSync.Constants.MAW_WIDGET_BACKGROUND_COLOR)]
            public string BackgroundColor
            {
                set
                {
                    System.Windows.Media.SolidColorBrush brush;
                    MoSync.Util.ConvertStringToColor(value, out brush);
                    if (View is System.Windows.Controls.Control) ((System.Windows.Controls.Control)View).Background = brush;
                    else if (View is System.Windows.Controls.Panel) ((System.Windows.Controls.Panel)View).Background = brush;
                }
            }

            /**
             * MAW_WIDGET_ENABLED implementation
             */
            [MoSyncWidgetProperty(MoSync.Constants.MAW_WIDGET_ENABLED)]
            public string Enabled
            {
                set
                {
                    Type objType = mView.GetType();
                    if (objType.GetProperty("IsEnabled") != null)
                    {
                        bool val;
                        if (Boolean.TryParse(value, out val))
                        {
                            var property = objType.GetProperty("IsEnabled");
                            property.SetValue(mView, val, null);
                        }
                        else throw new InvalidPropertyValueException();
                    }
                }
                get
                {
                    Type objType = mView.GetType();
                    if (objType.GetProperty("IsEnabled") != null)
                    {
                        var property = objType.GetProperty("IsEnabled");
                        return (property.GetValue(mView, null)).ToString();
                    }
                    return null;
                }
            }

            /**
             * MAW_WIDGET_VISIBLE implementation
             */
            [MoSyncWidgetProperty(MoSync.Constants.MAW_WIDGET_VISIBLE)]
            public string Visible
            {
                set
                {
                    bool val;
                    if (Boolean.TryParse(value, out val))
                    {
                        if (false == val)
                        {
                            mView.IsHitTestVisible = false;
                            mView.Opacity = 0.0;
                        }
                        else
                        {
                            mView.IsHitTestVisible = true;
                            mView.Opacity = 1.0;
                        }
                    }
                    else throw new InvalidPropertyValueException();
				}
				get
				{
				    if (false == mView.IsHitTestVisible)
                    {
                        return "false";
                    }
                    else
                    {
                        return "true";
                    }
                }
            }

            /**
             * MAW_WIDGET_BACKGROUND_GRADIENT implementation
             */
            [MoSyncWidgetProperty(MoSync.Constants.MAW_WIDGET_BACKGROUND_GRADIENT)]
            public string BackgroundGradient
            {
                set
                {
                    System.Windows.Media.GradientStop firstGradientStop = new System.Windows.Media.GradientStop();
                    System.Windows.Media.GradientStop secondGradientStop = new System.Windows.Media.GradientStop();

                    System.Windows.Media.SolidColorBrush firstBrush;
                    Util.ConvertStringToColor(value.Split(',')[0], out firstBrush);

                    System.Windows.Media.SolidColorBrush secondBrush;
                    Util.ConvertStringToColor(value.Split(',')[1], out secondBrush);

                    firstGradientStop.Color = firstBrush.Color;
                    secondGradientStop.Color = secondBrush.Color;

                    firstGradientStop.Offset = 0.0;
                    secondGradientStop.Offset = 0.5;

                    System.Windows.Media.LinearGradientBrush brush = new System.Windows.Media.LinearGradientBrush();

                    brush.StartPoint = new Point(0.5, 0);
                    brush.EndPoint = new Point(0.5, 1);
                    brush.GradientStops.Add(firstGradientStop);
                    brush.GradientStops.Add(secondGradientStop);

                    if (View is System.Windows.Controls.Control) ((System.Windows.Controls.Control)View).Background = brush;
                    else if (View is System.Windows.Controls.Panel) ((System.Windows.Controls.Panel)View).Background = brush;
                }
            }

            /**
             * MAW_WIDGET_ALPHA implementation
             */
            [MoSyncWidgetProperty(MoSync.Constants.MAW_WIDGET_ALPHA)]
            public double Alpha
            {
                set
                {
                    mView.Opacity = value;
                }
                get
                {
                    return mView.Opacity;
                }
            }

            #endregion

            #region Validate property region

            public static bool ValidateProperty(String propertyName, String propertyValue)
            {
                bool isPropertyValid = true;
                if (propertyName.Equals("enabled") || propertyName.Equals("visible"))
                {
                    bool val;
                    if (!Boolean.TryParse(propertyValue, out val)) isPropertyValid = false;
                }
                else if (propertyName.Equals("backgroundColor"))
                {
                    try
                    {
                        System.Windows.Media.SolidColorBrush brush;
                        MoSync.Util.ConvertStringToColor(propertyValue, out brush);
                    }
                    catch (InvalidPropertyValueException)
                    {
                        isPropertyValid = false;
                    }
                }
                else if (propertyName.Equals("backgroundGradient"))
                {
                    try
                    {
                        System.Windows.Media.GradientStop firstGradientStop = new System.Windows.Media.GradientStop();
                        System.Windows.Media.GradientStop secondGradientStop = new System.Windows.Media.GradientStop();

                        System.Windows.Media.SolidColorBrush firstBrush;
                        Util.ConvertStringToColor(propertyValue.Split(',')[0], out firstBrush);

                        System.Windows.Media.SolidColorBrush secondBrush;
                        Util.ConvertStringToColor(propertyValue.Split(',')[1], out secondBrush);
                    }
                    catch (InvalidPropertyValueException)
                    {
                        isPropertyValid = false;
                    }
                }

                return isPropertyValid;
            }

            #endregion

            #region Helper UI methods (wrapping and filling)

            /**
             * \brief This functions is called on the wrap content horizontally case
             */
            private void WrapContentHorizontally()
            {
                mView.ClearValue(Canvas.WidthProperty);
                mView.SetValue(Canvas.HorizontalAlignmentProperty, HorizontalAlignment.Center);
                setHorizontalSizePolicyFlags(false, true);
                mWidth = MoSync.Constants.MAW_CONSTANT_WRAP_CONTENT;

                //In case of setting the widget property after it is added
                //to a parent widget

                // WRAP_CONT_H in a HorizontalLayout
                if (this.GetParent() is HorizontalLayout)
                {
                    HorizontalLayout parent = (this.GetParent() as HorizontalLayout);
                    Grid content = (parent.View as Grid);

                    content.ColumnDefinitions[ColumnNumber].Width = new GridLength(1, GridUnitType.Auto);
                    parent.SetContainerOrientation(this, ColumnNumber, Orientation.Vertical);

                    if (WRAP_CONT_V)
                    {
                        parent.SetContainerOrientation(this, ColumnNumber, Orientation.Horizontal);
                    }
                }
                // WRAP_CONT_H in a VerticalLayout
                else if (this.GetParent() is VerticalLayout)
                {
                    VerticalLayout parent = (this.GetParent() as VerticalLayout);
                    Grid content = (parent.View as Grid);

                    parent.SetContainerOrientation(this, ColumnNumber, Orientation.Horizontal);

                    if (WRAP_CONT_V)
                    {
                        parent.SetContainerOrientation(this, ColumnNumber, Orientation.Vertical);
                    }
                }
            }

            /**
             * \brief This functions is called on the fill space horizontally case
             */
            private void FillSpaceHorizontally()
            {
                mView.ClearValue(Canvas.WidthProperty);
                mView.SetValue(Canvas.HorizontalAlignmentProperty, HorizontalAlignment.Stretch);
                setHorizontalSizePolicyFlags(true, false);
                mWidth = MoSync.Constants.MAW_CONSTANT_FILL_AVAILABLE_SPACE;

                //In case of setting the widget property after it is added
                //to a parent widget

                // FILL_SPACE_H in a HorizontalLayout
                if (this.GetParent() is HorizontalLayout)
                {
                    HorizontalLayout parent = (this.GetParent() as HorizontalLayout);
                    Grid content = (parent.View as Grid);

                    content.ColumnDefinitions[ColumnNumber].Width = new GridLength(1, GridUnitType.Star);
                    parent.SetContainerOrientation(this, ColumnNumber, Orientation.Vertical);

                    if (FILL_SPACE_V && parent.RemoveWidgetFromStackPanelContainer(this, ColumnNumber))
                    {
                        Grid.SetRow(View as FrameworkElement, RowNumber);
                        Grid.SetColumn(View as FrameworkElement, ColumnNumber);
                    }
                }
                // FILL_SPACE_H in a VerticalLayout
                else if (this.GetParent() is VerticalLayout)
                {
                    VerticalLayout parent = (this.GetParent() as VerticalLayout);
                    Grid content = (parent.View as Grid);

                    content.RowDefinitions[RowNumber].Height = new GridLength(1, GridUnitType.Auto);
                    parent.SetContainerOrientation(this, RowNumber, Orientation.Vertical);

                    if (FILL_SPACE_V && parent.RemoveWidgetFromStackPanelContainer(this, RowNumber))
                    {
                        Grid.SetColumn(View as FrameworkElement, ColumnNumber);
                        Grid.SetRow(View as FrameworkElement, RowNumber);
                    }
                }
            }


            /**
             * \brief This functions is called on the wrap content vertically case
             */
            private void WrapContentVertically()
            {
                mView.ClearValue(Canvas.HeightProperty);
                mView.SetValue(Canvas.VerticalAlignmentProperty, VerticalAlignment.Center);
                setVerticalSizePolicyFlags(false, true);
                mHeight = MoSync.Constants.MAW_CONSTANT_WRAP_CONTENT;

                //In case of setting the widget property after it is added
                //to a parent widget

                // WRAP_CONT_V in a HorizontalLayout
                if (this.GetParent() is HorizontalLayout)
                {
                    HorizontalLayout parent = (this.GetParent() as HorizontalLayout);
                    Grid content = (parent.View as Grid);

                    parent.SetContainerOrientation(this, ColumnNumber, Orientation.Vertical);

                    if (WRAP_CONT_H)
                    {
                        content.ColumnDefinitions[ColumnNumber].Width = new GridLength(1, GridUnitType.Auto);
                        parent.SetContainerOrientation(this, ColumnNumber, Orientation.Horizontal);
                    }
                }
                // WRAP_CONT_V in a VerticalLayout
                else if (this.GetParent() is VerticalLayout)
                {
                    VerticalLayout parent = (this.GetParent() as VerticalLayout);
                    Grid content = (parent.View as Grid);

                    content.RowDefinitions[RowNumber].Height = new GridLength(1, GridUnitType.Auto);
                    parent.SetContainerOrientation(this, ColumnNumber, Orientation.Vertical);

                    if (WRAP_CONT_V)
                    {
                        parent.SetContainerOrientation(this, ColumnNumber, Orientation.Horizontal);
                    }
                }
            }

            /**
             * \brief This functions is called on the fill stace vertically case
             */
            private void FillSpaceVertically()
            {
                mView.ClearValue(Canvas.HeightProperty);
                mView.SetValue(Canvas.VerticalAlignmentProperty, VerticalAlignment.Stretch);
                setVerticalSizePolicyFlags(true, false);
                mHeight = MoSync.Constants.MAW_CONSTANT_FILL_AVAILABLE_SPACE;

                //In case of setting the widget property after it is added
                //to a parent widget

                // FILL_SPACE_V in a HorizontalLayout
                if (this.GetParent() is HorizontalLayout)
                {
                    HorizontalLayout parent = (this.GetParent() as HorizontalLayout);
                    Grid content = (parent.View as Grid);

                    content.ColumnDefinitions[ColumnNumber].Width = new GridLength(1, GridUnitType.Auto);
                    parent.SetContainerOrientation(this, ColumnNumber, Orientation.Horizontal);

                    if (FILL_SPACE_H && parent.RemoveWidgetFromStackPanelContainer(this, ColumnNumber))
                    {
                        content.ColumnDefinitions[ColumnNumber].Width = new GridLength(1, GridUnitType.Star);
                        Grid.SetRow(View as FrameworkElement, RowNumber);
                        Grid.SetColumn(View as FrameworkElement, ColumnNumber);
                    }
                }
                // FILL_SPACE_V in a VerticalLayout
                else if (this.GetParent() is VerticalLayout)
                {
                    VerticalLayout parent = (this.GetParent() as VerticalLayout);
                    Grid content = (parent.View as Grid);

                    content.RowDefinitions[RowNumber].Height = new GridLength(1, GridUnitType.Star);
                    parent.SetContainerOrientation(this, RowNumber, Orientation.Horizontal);

                    if (FILL_SPACE_H && parent.RemoveWidgetFromStackPanelContainer(this, RowNumber))
                    {
                        Grid.SetColumn(View as FrameworkElement, ColumnNumber);
                        Grid.SetRow(View as FrameworkElement, RowNumber);
                    }
                }
            }

            #endregion
        }

        #region UIManagers

        public class NativeUIWindowsPhone : UIManager
        {
            private PhoneApplicationFrame mFrame;
            public NativeUIWindowsPhone()
                : base()
            {
                //This should always be a PhoneApplicationFrame.
                mFrame = (PhoneApplicationFrame)Application.Current.RootVisual;
            }
        }

        #endregion
    }
}
