/**
 * @file MoSyncLabel.cs
 * @author Rata Gabriela
 *
 * @brief This represents the Label Widget implementation for the NativeUI
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
        /**
         * Class defining a Label widget.
         */
		public class Label : WidgetBaseWindowsPhone
		{
			/**
            * The TextBlock widget, that will contain the text.
            */
            protected System.Windows.Controls.TextBlock mLabel;

            /**
             * mMaxNumberOfLines: can take two values: 1 and 0. For 1 the label is single line.
             * 0 means that the label is multiline.
             */
			protected int mMaxNumberOfLines;

            /**
             * Constructor
             */
			public Label()
			{
				mMaxNumberOfLines = 0;

                mLabel = new System.Windows.Controls.TextBlock();
				mLabel.TextWrapping = TextWrapping.NoWrap;

                mView = mLabel;
			}

            /**
             * Implementation of the Text property
             * set: sets the text on the label
             * get: returns the text displayed on the label
             */
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

            /**
             * Implementation of the textVerticalAlignment property
             * Sets the vertical alignment of the text displayed on the label
             */
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

            /**
             * Implementation of the textHorizontalAlignment property
             * Sets the horizontal alignment of the text displayed on the label
             */
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

            /**
             * Implementation of the fontSize property
             * Sets the font size of the text displayed on the label
             */
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

            /**
             * Implementation of the fontColor property
             * Sets the font color of the text displayed on the label
             */
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

            /**
             * Implementation of the maxNumberOfLines property
             * set: sets if the label is single or multiline.
             * Accepts two values: 1 (meaning single line) and 1
             */
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

            /**
             * The implementation of the "FontHandle" property. 
             * Sets the font handle used to display the item's text
             */
            [MoSyncWidgetProperty(MoSync.Constants.MAW_LABEL_FONT_HANDLE)]
            public String FontHandle
            {
                set
                {

                }
            }
		}
    }
}
