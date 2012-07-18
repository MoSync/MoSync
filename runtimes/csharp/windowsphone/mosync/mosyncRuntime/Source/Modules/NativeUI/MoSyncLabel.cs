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
using System.Windows.Media;

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
             * The parent container of the native label.
             * This is used for the vertical alignement.
             */
            private System.Windows.Controls.Grid mParentLayout;

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
                mParentLayout = new System.Windows.Controls.Grid();

                RowDefinition _rowDef = new RowDefinition();
                _rowDef.Height = new GridLength(1, GridUnitType.Auto);
                RowDefinition _spacerUp = new RowDefinition();
                _rowDef.Height = new GridLength(1, GridUnitType.Auto);
                RowDefinition _spacerDown = new RowDefinition();
                _rowDef.Height = new GridLength(1, GridUnitType.Auto);

                mParentLayout.RowDefinitions.Add(_spacerUp);
                mParentLayout.RowDefinitions.Add(_rowDef);
                mParentLayout.RowDefinitions.Add(_spacerDown);

                mLabel = new System.Windows.Controls.TextBlock();
				mLabel.TextWrapping = TextWrapping.Wrap;

                /*
                 * We need to set some default values on the text block. For this, we use
                 * a predefined style.
                 * PhoneTextNormalStyle description:
                 * FontFamily: PhoneFontFamilyNormal
                 * FontSize: PhoneFontSizeNormal
                 * Foreground: PhoneForegroundBrush
                 * Margin: PhoneHorizontalMargin
                 */
                mLabel.Style = (Style)Application.Current.Resources["PhoneTextNormalStyle"];

                mParentLayout.Children.Add(mLabel);

                Grid.SetRow(mLabel, 1);

                mView = mParentLayout;
			}

            /**
             * Implementation of the Left property of the Label widget.
             */
            [MoSyncWidgetProperty(MoSync.Constants.MAW_WIDGET_LEFT)]
            public new double Left
            {
                get { return mLabel.Margin.Left; }
                set
                {
                    mLabel.Margin = new Thickness(value,
                        mLabel.Margin.Top,
                        mLabel.Margin.Right,
                        mLabel.Margin.Bottom);
                }
            }

            /**
             * Implementation of the Top property of the Label widget.
             */
            [MoSyncWidgetProperty(MoSync.Constants.MAW_WIDGET_TOP)]
            public new double Top
            {
                get { return mLabel.Margin.Top; }
                set
                {
                    mLabel.Margin = new Thickness(mLabel.Margin.Left,
                        value,
                        mLabel.Margin.Right,
                        mLabel.Margin.Bottom);
                }
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
                                mParentLayout.RowDefinitions[0].Height = new GridLength(0);
                                mParentLayout.RowDefinitions[2].Height = new GridLength(1, GridUnitType.Star);
                                mLabel.VerticalAlignment = VerticalAlignment.Top;
							    break;
						    case MoSync.Constants.MAW_ALIGNMENT_CENTER:
                                mParentLayout.RowDefinitions[0].Height = new GridLength(1, GridUnitType.Star);
                                mParentLayout.RowDefinitions[2].Height = new GridLength(1, GridUnitType.Star);
                                mLabel.VerticalAlignment = VerticalAlignment.Center;
							    break;
						    case MoSync.Constants.MAW_ALIGNMENT_BOTTOM:
                                mParentLayout.RowDefinitions[0].Height = new GridLength(1, GridUnitType.Star);
                                mParentLayout.RowDefinitions[2].Height = new GridLength(0);
                                mLabel.VerticalAlignment = VerticalAlignment.Bottom;
							    break;
                            default:
                                throw new InvalidPropertyValueException();
					    }
			    }
				get
				{
					return mLabel.VerticalAlignment.ToString();
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
                        // for some values better use the default size of the platform
                        mLabel.FontSize = size <= 0 ? 11 : size;
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
             * Accepts two values: 1 (meaning single line) and 0
             */
			[MoSyncWidgetProperty(MoSync.Constants.MAW_LABEL_MAX_NUMBER_OF_LINES)]
			public int maxNumberOfLines
			{
				set
				{
					if ( 0 == value )
					{
						mMaxNumberOfLines = 0;
						mLabel.TextWrapping = TextWrapping.NoWrap;
					}
                    else if (1 == value)
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
            public int FontHandle
            {
                set
                {
					FontModule.FontInfo fontInfo =
						mRuntime.GetModule<FontModule>().GetFont(value);

					mLabel.FontFamily = fontInfo.family;
					mLabel.FontWeight = fontInfo.weight;
					mLabel.FontStyle = fontInfo.style;
                }
            }
		}
    }
}
