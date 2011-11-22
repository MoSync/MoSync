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
					double size = 0;
					if (double.TryParse(value, out size))
					{
						mLabel.FontSize = size;
					}
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
					int val = -1;
					if (!int.TryParse(value, out val))
					{
						return;
					}
					if ( 0 == val )
					{
						mMaxNumberOfLines = 0;
						mLabel.TextWrapping = TextWrapping.NoWrap;
					}
					else if ( 1 == val )
					{
						mMaxNumberOfLines = 1;
						mLabel.TextWrapping = TextWrapping.Wrap;
					}
				}
			}
		}
    }
}
