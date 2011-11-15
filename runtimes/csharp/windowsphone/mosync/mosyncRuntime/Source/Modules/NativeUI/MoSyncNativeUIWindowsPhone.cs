using Microsoft.Phone.Controls;
using System.Windows.Controls;
using System.Windows;
using System.Windows.Navigation;
using System;
using System.Text.RegularExpressions;
using System.Reflection;

namespace MoSync
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
                if(false == val) mView.Visibility = Visibility.Collapsed;
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
    };

    public class Label : WidgetBaseWindowsPhone
    {
        protected System.Windows.Controls.TextBlock mLabel;

        protected int mMaxNumberOfLines;
        public Label()
        {
			//the label
			mLabel = new System.Windows.Controls.TextBlock();
			mView = mLabel;
			mMaxNumberOfLines = 0;
			mLabel.TextWrapping = TextWrapping.NoWrap;
		}

		[MoSyncWidgetProperty(MoSync.Constants.MAW_LABEL_TEXT)]
		public String Text
		{
			set
			{
				mLabel.Text = value;
			}
			get
			{
				return mLabel.Text;
			}
		}

        [MoSyncWidgetProperty(MoSync.Constants.MAW_LABEL_TEXT_VERTICAL_ALIGNMENT)]
        public String textVerticalAlignment
        {
            set
            {
                switch (value)
                {
                    case MoSync.Constants.MAW_ALIGNMENT_TOP:
                        mLabel.VerticalAlignment = VerticalAlignment.Top;
                        break;
                    case MoSync.Constants.MAW_ALIGNMENT_CENTER:
                        mLabel.VerticalAlignment = VerticalAlignment.Center;
                        break;
                    case MoSync.Constants.MAW_ALIGNMENT_BOTTOM:
                        mLabel.VerticalAlignment = VerticalAlignment.Bottom;
                        break;
                }
            }
            get
            {
                return mLabel.Text;
            }
        }

        [MoSyncWidgetProperty(MoSync.Constants.MAW_LABEL_TEXT_HORIZONTAL_ALIGNMENT)]
        public String textHorizontalAlignment
        {
            set
            {
                switch (value)
                {
                    case MoSync.Constants.MAW_ALIGNMENT_LEFT:
                        mLabel.TextAlignment = TextAlignment.Left;

                        break;
                    case MoSync.Constants.MAW_ALIGNMENT_RIGHT:
                        mLabel.TextAlignment = TextAlignment.Right;
                        break;
                    case MoSync.Constants.MAW_ALIGNMENT_CENTER:
                        mLabel.TextAlignment = TextAlignment.Center;
                        break;
                }
            }
            get
            {
                return mLabel.TextAlignment.ToString();
            }
        }

        [MoSyncWidgetProperty(MoSync.Constants.MAW_LABEL_FONT_SIZE)]
        public String fontSize
        {
            set
            {
                //todo
                double size = Double.Parse(value);
                mLabel.FontSize = size;
            }
        }

        [MoSyncWidgetProperty(MoSync.Constants.MAW_LABEL_FONT_COLOR)]
        public String fontColor
        {
            set
            {
                System.Windows.Media.SolidColorBrush brush;
                MoSync.Util.convertStringToColor(value, out brush);
                mLabel.Foreground = brush;

            }
        }

        [MoSyncWidgetProperty(MoSync.Constants.MAW_LABEL_MAX_NUMBER_OF_LINES)]
        public String maxNumberOfLines
        {
            set
            {
                int val = Int32.Parse(value);
                if (val == 0)
                {
                    mMaxNumberOfLines = 0;
                    mLabel.TextWrapping = TextWrapping.NoWrap;
                }
                if (val == 1)
                {
                    mMaxNumberOfLines = 1;
                    mLabel.TextWrapping = TextWrapping.Wrap;
                }
            }
        }
    }

    public class CheckBox : WidgetBaseWindowsPhone
    {
        protected System.Windows.Controls.CheckBox mCheckBox;

        public CheckBox()
        {
            mCheckBox = new System.Windows.Controls.CheckBox();

            mView = mCheckBox;

            mCheckBox.Click += new RoutedEventHandler(
               delegate(Object from, RoutedEventArgs evt)
               {
                   Memory eventData = new Memory(8);

                   const int MAWidgetEventData_eventType = 0;
                   const int MAWidgetEventData_widgetHandle = 4;

                   eventData.WriteInt32(MAWidgetEventData_eventType, MoSync.Constants.MAW_EVENT_CLICKED);
                   eventData.WriteInt32(MAWidgetEventData_widgetHandle, mHandle);
                   mRuntime.PostCustomEvent(MoSync.Constants.EVENT_TYPE_WIDGET, eventData);
               });
        }

        public virtual void setState(bool state)
        {
            mCheckBox.IsChecked = state;
        }

        [MoSyncWidgetProperty(MoSync.Constants.MAW_CHECK_BOX_CHECKED)]
        public String Checked
        {
            get
            {
                return mCheckBox.IsChecked.ToString();
            }

            set
            {
                bool val = bool.Parse(value);
                mCheckBox.IsChecked = val;
            }
        }
    }

     public class EditBox : WidgetBaseWindowsPhone
     {
         protected System.Windows.Controls.TextBox mEditBox;
         protected bool mInitialCapsWord;
         protected bool mInitialCapsSentence;

         protected void setInputMode(System.Windows.Input.InputScopeNameValue scopeValue)
         {
             System.Windows.Input.InputScope keyboard = new System.Windows.Input.InputScope();
             System.Windows.Input.InputScopeName scopeName = new System.Windows.Input.InputScopeName();

             scopeName.NameValue = scopeValue;
             keyboard.Names.Add(scopeName);
             mEditBox.InputScope = keyboard;
         }

         public EditBox()
         {
             mEditBox = new System.Windows.Controls.TextBox();
             View = mEditBox;
             mInitialCapsWord = false;
             mInitialCapsSentence = false;
         }

         [MoSyncWidgetProperty(MoSync.Constants.MAW_EDIT_BOX_TEXT)]
         public String Text
         {
             set
             {
                 mEditBox.Text = value;
             }
             get
             {
                 return mEditBox.Text;
             }
         }

         [MoSyncWidgetProperty(MoSync.Constants.MAW_EDIT_BOX_PLACEHOLDER)]
         public String Placeholder
         {
             set
             {
                 mEditBox.Text = value;
             }
         }

         //showKeyboard "true" or "false"
         [MoSyncWidgetProperty(MoSync.Constants.MAW_EDIT_BOX_SHOW_KEYBOARD)]
         public String ShowKeyboard
         {
             set
             {
                 //todo: implement
             }
         }


         //validvalue Either 'password' or 'text'.
         [MoSyncWidgetProperty(MoSync.Constants.MAW_EDIT_BOX_EDIT_MODE)]
         public String EditMode
         {
             set
             {
                 if (value != "password" && value != "text")
                     return;

                 System.Windows.Input.InputScope keyboard = new System.Windows.Input.InputScope();
                 System.Windows.Input.InputScopeName scopeName = new System.Windows.Input.InputScopeName();
                 if (value == "password")
                 {
                     scopeName.NameValue = System.Windows.Input.InputScopeNameValue.Password;
                 }
                 if (value == "text")
                 {
                     scopeName.NameValue = System.Windows.Input.InputScopeNameValue.Text;
                 }
                 keyboard.Names.Add(scopeName);
                 mEditBox.InputScope = keyboard;
             }
         }

         [MoSyncWidgetProperty(MoSync.Constants.MAW_EDIT_BOX_INPUT_MODE)]
         public String InputMode
         {
             set
             {
                 int inputType = Int32.Parse(value);
                 switch (inputType)
                 {
                     case 0:             //MAW_EDIT_BOX_TYPE_ANY
                         setInputMode(System.Windows.Input.InputScopeNameValue.Default);
                         break;
                     case 1:             //MAW_EDIT_BOX_TYPE_EMAILADDR
                         setInputMode(System.Windows.Input.InputScopeNameValue.EmailSmtpAddress);
                         break;
                     case 2:             //MAW_EDIT_BOX_TYPE_NUMERIC
                         setInputMode(System.Windows.Input.InputScopeNameValue.NumberFullWidth);
                         break;
                     case 3:             //MAW_EDIT_BOX_TYPE_PHONENUMBER
                         setInputMode(System.Windows.Input.InputScopeNameValue.TelephoneNumber);
                         break;
                     case 4:             //MAW_EDIT_BOX_TYPE_URL
                         setInputMode(System.Windows.Input.InputScopeNameValue.Url);
                         break;
                     case 5:             //MAW_EDIT_BOX_TYPE_DECIMAL
                         setInputMode(System.Windows.Input.InputScopeNameValue.Digits); //???????
                         break;
                     case 6:             //MAW_EDIT_BOX_TYPE_SINGLE_LINE
                         mEditBox.TextWrapping = TextWrapping.NoWrap;
                         break;
                 }
             }
         }
     }


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

            //mGrid.ShowGridLines = true; //uncomment to see the grid lines (for debugging)
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

    //VerticalLayout class
    public class VerticalLayout : WidgetBaseWindowsPhone
    {
        //the Grid that will be used as the VerticalLayout
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

        //the main column of the Grid object (an vertical layout is a grid with one column)
        protected ColumnDefinition mColDef;

        //the constructor
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

            mView = mGrid;
            //mGrid.ShowGridLines = true; //uncomment to see the grid lines (for debugging)
        }

        //add child
        public override void AddChild(IWidget child)
        {
            base.AddChild(child);
            MoSync.Util.RunActionOnMainThreadSync(() =>
            {
                WidgetBaseWindowsPhone widget = (child as WidgetBaseWindowsPhone);
                RowDefinition rowDef = new RowDefinition();

                if(widget.fillSpaceVerticalyEnabled) rowDef.Height = new System.Windows.GridLength(1, System.Windows.GridUnitType.Star);
                else rowDef.Height = new System.Windows.GridLength(1, System.Windows.GridUnitType.Auto);

                mGrid.RowDefinitions.Insert(mGrid.RowDefinitions.Count - 1, rowDef);

                mGrid.Children.Add(widget.View);

                Grid.SetColumn((widget.View as System.Windows.FrameworkElement), 1);
                Grid.SetRow((widget.View as System.Windows.FrameworkElement), mGrid.RowDefinitions.Count - 2);
            });
        }

        //MAW_VERTICAL_LAYOUT_CHILD_HORIZONTAL_ALIGNMENT implementation
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
                    mSpacerRight.Width = new GridLength(1, GridUnitType.Star);
                    mSpacerLeft.Width = new GridLength(1, GridUnitType.Star);
                }
            }
        }

        //MAW_VERTICAL_LAYOUT_CHILD_VERTICAL_ALIGNMENT implementation
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
                    mSpacerDown.Height = new GridLength(1, GridUnitType.Star);
                    mSpacerUp.Height = new GridLength(1, GridUnitType.Star);
                }
            }
        }

        //MAW_VERTICAL_LAYOUT_PADDING_BOTTOM implementation
        [MoSyncWidgetProperty(MoSync.Constants.MAW_VERTICAL_LAYOUT_PADDING_BOTTOM)]
        public String PaddingBottom
        {
            set
            {
                double val = Double.Parse(value);
                mPaddingBottom = val;
                mSpacerDown.Height = new GridLength(mPaddingBottom);
            }
        }

        //MAW_VERTICAL_LAYOUT_PADDING_TOP implementation
        [MoSyncWidgetProperty(MoSync.Constants.MAW_VERTICAL_LAYOUT_PADDING_TOP)]
        public String PaddingTop
        {
            set
            {
                double val = Double.Parse(value);
                mPaddingTop = val;
                mSpacerUp.Height = new GridLength(mPaddingTop);
            }
        }

        //MAW_VERTICAL_LAYOUT_PADDING_LEFT implementation
        [MoSyncWidgetProperty(MoSync.Constants.MAW_VERTICAL_LAYOUT_PADDING_LEFT)]
        public String PaddingLeft
        {
            set
            {
                double val = Double.Parse(value);
                mPaddingLeft = val;
                mSpacerLeft.Width = new GridLength(mPaddingLeft);
            }
        }

        //MAW_VERTICAL_LAYOUT_PADDING_RIGHT implementation
        [MoSyncWidgetProperty(MoSync.Constants.MAW_VERTICAL_LAYOUT_PADDING_RIGHT)]
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

    //RelativeLayout class
    public class RelativeLayout : WidgetBaseWindowsPhone
    {
        //Canvas is the content in which the children are aranged relatively to the parent
        protected System.Windows.Controls.Canvas mPanel;

        //Constructor
        public RelativeLayout()
        {
            mPanel = new System.Windows.Controls.Canvas();
            mView = mPanel;
        }

        //add child
        public override void AddChild(IWidget child)
        {
            base.AddChild(child);
            MoSync.Util.RunActionOnMainThreadSync(() =>
            {
                WidgetBaseWindowsPhone widget = (child as WidgetBaseWindowsPhone);

                mPanel.Children.Add(widget.View);
            });
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
            mButton.HorizontalAlignment = HorizontalAlignment.Left;
            mButton.VerticalAlignment = VerticalAlignment.Top;

            fillSpaceVerticalyEnabled = false;
            fillSpaceHorizontalyEnabled = false;

            mButton.Click += new RoutedEventHandler(
                delegate(Object from, RoutedEventArgs evt)
                {
                    Memory eventData = new Memory(8);

                    const int MAWidgetEventData_eventType = 0;
                    const int MAWidgetEventData_widgetHandle = 4;

                    eventData.WriteInt32(MAWidgetEventData_eventType, MoSync.Constants.MAW_EVENT_POINTER_PRESSED);
                    eventData.WriteInt32(MAWidgetEventData_widgetHandle, mHandle);
                    mRuntime.PostCustomEvent(MoSync.Constants.EVENT_TYPE_WIDGET, eventData);
                });
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
                ((System.Windows.FrameworkElement)w.View).Width = ScreenSize.SCREEN_WIDTH;
                ((System.Windows.FrameworkElement)w.View).Height = ScreenSize.SCREEN_HEIGHT;
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