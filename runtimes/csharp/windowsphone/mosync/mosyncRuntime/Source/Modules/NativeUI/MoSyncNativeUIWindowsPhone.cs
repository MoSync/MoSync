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
        public class ScreenSize
        {
            public static int SCREEN_HEIGHT = 800;
            public static int SCREEN_WIDTH = 480;
        }
        public class WidgetBaseWindowsPhone : WidgetBase
        {
            protected UIElement mView;
            public bool fillSpaceHorizontalyEnabled;
            public bool fillSpaceVerticalyEnabled;

            public UIElement View
            {
                get { return mView; }
                set { mView = value; }
            }

            //MAW_WIDGET_LEFT implementation
            [MoSyncWidgetProperty(MoSync.Constants.MAW_WIDGET_LEFT)]
            public double Left
            {
                get { return (double)mView.GetValue(Canvas.LeftProperty); }
                set
                {
                    mView.SetValue(Canvas.LeftProperty, value);
                }
            }

            //MAW_WIDGET_TOP implementation
            [MoSyncWidgetProperty(MoSync.Constants.MAW_WIDGET_TOP)]
            public double Top
            {
                get { return (double)mView.GetValue(Canvas.TopProperty); }
                set
                {
                    mView.SetValue(Canvas.TopProperty, value);
                }
            }

            //MAW_WIDGET_WIDTH implementation
            [MoSyncWidgetProperty(MoSync.Constants.MAW_WIDGET_WIDTH)]
            public double Width
            {
                get { return (double)mView.GetValue(Canvas.WidthProperty); }
                set
                {
                    if (!value.Equals(-1))
                    {
                        mView.SetValue(Canvas.WidthProperty, value);
                    }
                    else
                    {
                        mView.SetValue(Canvas.HorizontalAlignmentProperty, HorizontalAlignment.Stretch);
                        fillSpaceHorizontalyEnabled = true;
                    }
                }
            }

            //MAW_WIDGET_HEIGHT implementation
            [MoSyncWidgetProperty(MoSync.Constants.MAW_WIDGET_HEIGHT)]
            public double Height
            {
                get { return (double)mView.GetValue(Canvas.HeightProperty); }
                set
                {
                    if (!value.Equals(-1))
                    {
                        mView.SetValue(Canvas.HeightProperty, value);
                    }
                    else
                    {
                        mView.SetValue(Canvas.VerticalAlignmentProperty, VerticalAlignment.Stretch);
                        fillSpaceVerticalyEnabled = true;
                    }
                }
            }

            //MAW_WIDGET_BACKGROUND_COLOR implementation
            [MoSyncWidgetProperty(MoSync.Constants.MAW_WIDGET_BACKGROUND_COLOR)]
            public string BackgroundColor
            {
                set
                {
                    System.Windows.Media.SolidColorBrush brush;
                    MoSync.Util.convertStringToColor(value, out brush);
                    if (View is System.Windows.Controls.Control) ((System.Windows.Controls.Control)View).Background = brush;
                    else if (View is System.Windows.Controls.Panel) ((System.Windows.Controls.Panel)View).Background = brush;
                }
            }

            //MAW_WIDGET_ENABLED implementation
            [MoSyncWidgetProperty(MoSync.Constants.MAW_WIDGET_ENABLED)]
            public string Enabled
            {
                set
                {
                    Type objType = mView.GetType();
                    if (objType.GetProperty("IsEnabled") != null)
                    {
                        bool val = Boolean.Parse(value);
                        var property = objType.GetProperty("IsEnabled");
                        property.SetValue(mView, val, null);
                    }
                }
                get
                {
                    Type objType = mView.GetType();
                    if (objType.GetProperty("IsEnabled") != null)
                    {
                        var property = objType.GetProperty("IsEnabled");
                        return (string)(property.GetValue(mView, null));
                    }
                    return null;
                }
            }

            //MAW_WIDGET_VISIBLE implementation
            [MoSyncWidgetProperty(MoSync.Constants.MAW_WIDGET_VISIBLE)]
            public string Visible
            {
                set
                {
                    bool val = Boolean.Parse(value);
                    if (false == val) mView.Visibility = Visibility.Collapsed;
                    else mView.Visibility = Visibility.Visible;
                }
                get
                {
                    if (Visibility.Collapsed == mView.Visibility)
                    {
                        return "false";
                    }
                    else
                    {
                        return "true";
                    }
                }
            }

            //MAW_WIDGET_BACKGROUND_GRADIENT implementation
            [MoSyncWidgetProperty(MoSync.Constants.MAW_WIDGET_BACKGROUND_GRADIENT)]
            public string BackgroundGradient
            {
                set
                {
                    System.Windows.Media.GradientStop firstGradientStop = new System.Windows.Media.GradientStop();
                    System.Windows.Media.GradientStop secondGradientStop = new System.Windows.Media.GradientStop();

                    System.Windows.Media.SolidColorBrush firstBrush;
                    Util.convertStringToColor(value.Split(',')[0], out firstBrush);

                    System.Windows.Media.SolidColorBrush secondBrush;
                    Util.convertStringToColor(value.Split(',')[1], out secondBrush);

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

            //MAW_WIDGET_ALPHA implementation
            [MoSyncWidgetProperty(MoSync.Constants.MAW_WIDGET_ALPHA)]
            public string Alpha
            {
                set
                {
                    double val = double.Parse(value);
                    mView.Opacity = val;
                }
            }

            public WidgetBaseWindowsPhone()
                : base()
            {
            }
        }

        public class NativeUIWindowsPhone : UIManager
        {
            private PhoneApplicationFrame mFrame;
            public NativeUIWindowsPhone()
                : base()
            {
                // this should always be a PhoneApplicationFrame.
                mFrame = (PhoneApplicationFrame)Application.Current.RootVisual;
            }
        }
    }
}