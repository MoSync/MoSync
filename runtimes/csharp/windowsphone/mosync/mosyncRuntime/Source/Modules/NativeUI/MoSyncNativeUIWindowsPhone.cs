using Microsoft.Phone.Controls;
using System.Windows.Controls;
using System.Windows;
using System.Windows.Navigation;
using System;
using System.Text.RegularExpressions;

namespace MoSync
{
    public class SreenSize
    {
        public static int SCREEN_HEIGHT = 800;
        public static int SCREEN_WIDTH = 480;
    }
    public class WidgetBaseWindowsPhone : WidgetBase
    {
        protected UIElement mView;

        public UIElement View
        {
            get { return mView; }
            set { mView = value; }
        }

        [MoSyncWidgetProperty(MoSync.Constants.MAW_WIDGET_LEFT)]
        public double Left
        {
            get { return (double)mView.GetValue(Canvas.LeftProperty); }
            set
            {
                mView.SetValue(Canvas.LeftProperty, value);
            }
        }

        [MoSyncWidgetProperty(MoSync.Constants.MAW_WIDGET_TOP)]
        public double Top
        {
            get { return (double)mView.GetValue(Canvas.TopProperty); }
            set
            {
                mView.SetValue(Canvas.TopProperty, value);
            }
        }

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
            }
        }

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
            }
        }

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

        public WidgetBaseWindowsPhone()
            : base()
        {
        }
    };

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

            mRowDef.Height = new System.Windows.GridLength(1, System.Windows.GridUnitType.Auto);

            mSpacerUp.Height = new System.Windows.GridLength(1, System.Windows.GridUnitType.Auto);
            mSpacerDown.Height = new System.Windows.GridLength(1, System.Windows.GridUnitType.Auto);
            mSpacerRight.Width = new System.Windows.GridLength(1, System.Windows.GridUnitType.Auto);
            mSpacerLeft.Width = new System.Windows.GridLength(1, System.Windows.GridUnitType.Auto);

            mGrid.RowDefinitions.Add(mSpacerUp);
            mGrid.RowDefinitions.Add(mRowDef);
            mGrid.RowDefinitions.Add(mSpacerDown);

            mGrid.ColumnDefinitions.Add(mSpacerLeft);
            mGrid.ColumnDefinitions.Add(mSpacerRight);

            mView = mGrid;

            //mGrid.ShowGridLines = true; //uncomment to see the grid lines (for debugging)

            ChildVerticalAlignment = MoSync.Constants.MAW_ALIGNMENT_TOP;
            ChildHorizontalAlignment = MoSync.Constants.MAW_ALIGNMENT_LEFT;
        }

        //add child
        public override void AddChild(IWidget child)
        {
            base.AddChild(child);
            MoSync.Util.RunActionOnMainThreadSync(() =>
                {
                    WidgetBaseWindowsPhone widget = (child as WidgetBaseWindowsPhone);
                    ColumnDefinition columnDef = new ColumnDefinition();

                    columnDef.Width = new System.Windows.GridLength(1, System.Windows.GridUnitType.Auto);
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
                double val = Double.Parse(value);
                mPaddingBottom = val;
                mSpacerDown.Height = new GridLength(mPaddingBottom);
            }
        }

        //MAW_HORIZONTAL_LAYOUT_PADDING_TOP implementation
        [MoSyncWidgetProperty(MoSync.Constants.MAW_HORIZONTAL_LAYOUT_PADDING_TOP)]
        public String PaddingTop
        {
            set
            {
                double val = Double.Parse(value);
                mPaddingTop = val;
                mSpacerUp.Height = new GridLength(mPaddingTop);
            }
        }

        //MAW_HORIZONTAL_LAYOUT_PADDING_LEFT implementation
        [MoSyncWidgetProperty(MoSync.Constants.MAW_HORIZONTAL_LAYOUT_PADDING_LEFT)]
        public String PaddingLeft
        {
            set
            {
                double val = Double.Parse(value);
                mPaddingLeft = val;
                mSpacerLeft.Width = new GridLength(mPaddingLeft);
            }
        }

        //MAW_HORIZONTAL_LAYOUT_PADDING_RIGHT implementation
        [MoSyncWidgetProperty(MoSync.Constants.MAW_HORIZONTAL_LAYOUT_PADDING_RIGHT)]
        public String PaddingRight
        {
            set
            {
                double val = Double.Parse(value);
                mPaddingRight = val;
                mSpacerRight.Width = new GridLength(mPaddingRight);
            }
        }
    }

    // The button class
    public class Button : WidgetBaseWindowsPhone
    {
        //The text content of the button
        protected String mText;

        //the vertical alignment of the content
        protected VerticalAlignment mTextVerticalAlignment;

        //the horizontal alignment of the content
        protected HorizontalAlignment mTextHorizontalAlignment;

        //the button controll
        protected System.Windows.Controls.Button mButton;

        //the button constructor
        public Button()
        {
            //initializing the button controll
            mButton = new System.Windows.Controls.Button();
            //set the view of the current widget as the previously instantiated button controll
            View = mButton;
        }

        //MAW_BUTTON_TEXT property implementation
        [MoSyncWidgetProperty(MoSync.Constants.MAW_BUTTON_TEXT)]
        public String Text
        {
            set
            {
                mText = value;
                mButton.Content = mText;
            }
            get
            {
                return mText;
            }
        }

        //MAW_BUTTON_TEXT_HORIZONTAL_ALIGNMENT property implementation
        [MoSyncWidgetProperty(MoSync.Constants.MAW_BUTTON_TEXT_HORIZONTAL_ALIGNMENT)]
        public String TextHorizontalAlignment
        {
            set
            {
                //doing the proper behaviour for the required value
                String a = value.ToString();
                if (value.ToString().Equals(MoSync.Constants.MAW_ALIGNMENT_LEFT))
                {
                    mTextHorizontalAlignment = HorizontalAlignment.Left;
                    mButton.HorizontalContentAlignment = mTextHorizontalAlignment;
                }
                else
                {
                    if (value.ToString().Equals(MoSync.Constants.MAW_ALIGNMENT_RIGHT))
                    {
                        mTextHorizontalAlignment = HorizontalAlignment.Right;
                        mButton.HorizontalContentAlignment = mTextHorizontalAlignment;
                    }
                    else
                    {
                        if (value.ToString().Equals(MoSync.Constants.MAW_ALIGNMENT_CENTER))
                        {
                            mTextHorizontalAlignment = HorizontalAlignment.Center;
                            mButton.HorizontalContentAlignment = mTextHorizontalAlignment;
                        }
                    }
                }
            }
        }

        //MAW_BUTTON_TEXT_VERTICAL_ALIGNMENT property implementation
        [MoSyncWidgetProperty(MoSync.Constants.MAW_BUTTON_TEXT_VERTICAL_ALIGNMENT)]
        public String TextVerticalAlignment
        {
            set
            {
                if (value.ToString().Equals(MoSync.Constants.MAW_ALIGNMENT_TOP))
                {
                    mTextVerticalAlignment = VerticalAlignment.Top;
                    mButton.VerticalContentAlignment = mTextVerticalAlignment;
                }
                else
                {
                    if (value.ToString().Equals(MoSync.Constants.MAW_ALIGNMENT_BOTTOM))
                    {
                        mTextVerticalAlignment = VerticalAlignment.Bottom;
                        mButton.VerticalContentAlignment = mTextVerticalAlignment;
                    }
                    else
                    {
                        if (value.ToString().Equals(MoSync.Constants.MAW_ALIGNMENT_CENTER))
                        {
                            mTextVerticalAlignment = VerticalAlignment.Center;
                            mButton.VerticalContentAlignment = mTextVerticalAlignment;
                        }
                    }
                }
            }
        }

        //MAW_BUTTON_FONT_COLOR property implementation
        [MoSyncWidgetProperty(MoSync.Constants.MAW_BUTTON_FONT_COLOR)]
        public String FontColor
        {
            set
            {
                System.Windows.Media.SolidColorBrush brush;
                MoSync.Util.convertStringToColor(value, out brush);
                mButton.Foreground = brush;
            }
        }

        //MAW_BUTTON_FONT_SIZE property implementation
        [MoSyncWidgetProperty(MoSync.Constants.MAW_BUTTON_FONT_SIZE)]
        public String FontSize
        {
            set
            {
                double size = double.Parse(value);
                mButton.FontSize = size;
            }
        }
    }

    public class WebView : WidgetBaseWindowsPhone
    {
        protected WebBrowser mWebBrowser;
        protected String mHardHook = "";
        protected String mSoftHook = "";

        [MoSyncWidgetProperty(MoSync.Constants.MAW_WEB_VIEW_URL)]
        public String Url
        {
            set
            {

                if (value.StartsWith("javascript:"))
                {
                    int startIndex = "javascript:".Length;
                    String script = value.Substring(startIndex, value.Length - startIndex);
                    // mWebBrowser.InvokeScript("eval", new string[]{script});
                    mWebBrowser.InvokeScript("execScript", new string[] { script });
                }
                else
                {
                    Uri uri = new Uri(value, UriKind.Relative);
                    mWebBrowser.Navigate(uri);
                }
            }
        }

        [MoSyncWidgetProperty(MoSync.Constants.MAW_WEB_VIEW_HTML)]
        public String Html
        {
            set
            {
                mWebBrowser.NavigateToString(value);
            }
        }

        [MoSyncWidgetProperty(MoSync.Constants.MAW_WEB_VIEW_HARD_HOOK)]
        public String HardHook
        {
            set
            {
                mHardHook = value;
            }

            get
            {
                return mHardHook;
            }
        }

        [MoSyncWidgetProperty(MoSync.Constants.MAW_WEB_VIEW_SOFT_HOOK)]
        public String SoftHook
        {
            set
            {
                mSoftHook = value;
            }

            get
            {
                return mSoftHook;
            }
        }
        public WebView()
        {
            mWebBrowser = new Microsoft.Phone.Controls.WebBrowser();
            mView = mWebBrowser;
            mWebBrowser.IsScriptEnabled = true;

            mWebBrowser.ScriptNotify += new EventHandler<NotifyEventArgs>(
                delegate(object from, NotifyEventArgs args)
                {
                    String str = args.Value;
                    MoSync.Util.Log(str);

                    int hookType = 0;

                    if (Regex.IsMatch(str, mHardHook))
                    {
                        hookType = MoSync.Constants.MAW_CONSTANT_HARD;
                    }
                    else if (Regex.IsMatch(str, mSoftHook))
                    {
                        hookType = MoSync.Constants.MAW_CONSTANT_SOFT;
                    }
                    else
                    {
                        return;
                    }

                    Memory eventData = new Memory(16);
                    const int MAWidgetEventData_eventType = 0;
                    const int MAWidgetEventData_widgetHandle = 4;
                    const int MAWidgetEventData_hookType = 8;
                    const int MAWidgetEventData_urlData = 12;

                    Memory urlData = new Memory(str.Length + 1);
                    urlData.WriteStringAtAddress(0, str, str.Length + 1);

                    eventData.WriteInt32(MAWidgetEventData_eventType, MoSync.Constants.MAW_EVENT_WEB_VIEW_HOOK_INVOKED);
                    eventData.WriteInt32(MAWidgetEventData_widgetHandle, mHandle);
                    eventData.WriteInt32(MAWidgetEventData_hookType, hookType);
                    eventData.WriteInt32(MAWidgetEventData_urlData, mRuntime.AddResource(
                        new Resource(urlData, MoSync.Constants.RT_BINARY)));
                    mRuntime.PostCustomEvent(MoSync.Constants.EVENT_TYPE_WIDGET, eventData);
                }
            );

        }
    }

    public class Screen : WidgetBaseWindowsPhone, IScreen
    {
        protected PhoneApplicationPage mPage;
        public Screen()
        {
            mPage = new PhoneApplicationPage();
            mView = mPage;
        }

        public override void AddChild(IWidget child)
        {
            base.AddChild(child);
            WidgetBaseWindowsPhone w = (WidgetBaseWindowsPhone)child;
            MoSync.Util.RunActionOnMainThreadSync(() =>
            {
                mPage.Content = w.View;
                ((System.Windows.FrameworkElement)w.View).Width = 480;
                ((System.Windows.FrameworkElement)w.View).Height = 800;
            });
        }

        public void Show()
        {
            MoSync.Util.RunActionOnMainThreadSync(() =>
            {
                PhoneApplicationFrame frame = (PhoneApplicationFrame)Application.Current.RootVisual;
                frame.Content = mPage;
            });
        }
    }

    public class NativeUIWindowsPhone : NativeUI
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
