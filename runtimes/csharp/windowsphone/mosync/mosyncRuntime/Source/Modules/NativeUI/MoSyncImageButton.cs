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
 * @file MoSyncImageButton.cs
 * @author Rata Gabriela
 *
 * @brief This represents the ImageButton Widget implementation for the NativeUI
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
		 * The ImageButton class defining a ImageButton button
		 * The  button can display a text, a text and a image or text, foreground image and background image.
		 */
		public class ImageButton : WidgetBaseWindowsPhone
		{
			/**
			 * The native Button 
			 */
			protected System.Windows.Controls.Button mButton;

			/**
			 * A Grid that holds the foreground image and the text
			 */
			protected System.Windows.Controls.Grid mGrid;

			/**
			 * A TextBlock object that holds the text
			 */
			protected System.Windows.Controls.TextBlock mText;

			/**
			 * The foreground image
			 */
			protected System.Windows.Controls.Image mForegroundImage;

			/**
			 * Strech object, that defines if the foreground image should be streched, and the streching mode
			 */
			protected System.Windows.Media.Stretch mStretchForeground;

			/**
			 * The background image
			 */          
			protected System.Windows.Controls.Image mBackgroundImage;

			/**
			 * Strech object, that defines if the background image should be streched, and the streching mode
			 */
			protected System.Windows.Media.Stretch mStretchBackground;

			/**
			 * Function that creates the TextBlock object and setts the alignment of the text
			 */
			private void createTextBlock()
			{
				mText = new System.Windows.Controls.TextBlock();
				mText.TextWrapping = TextWrapping.NoWrap;

				mText.VerticalAlignment = VerticalAlignment.Center;
				mText.HorizontalAlignment = HorizontalAlignment.Center;
				mText.TextAlignment = TextAlignment.Center;
			}

			/**
			 * Function that that creates the foreground image
			 */
			private void createForegroundImage()
			{
                /**
                 * Create the foreground image and display it in the center of the button
                 */
				mForegroundImage = new System.Windows.Controls.Image();
				mForegroundImage.VerticalAlignment = VerticalAlignment.Center;
				mForegroundImage.HorizontalAlignment = HorizontalAlignment.Center;

               
			}

            /**
             * Function for creating the Grid.  Arranges the text and the foreground and background images on it.
             */
            private void AddWidgetsToGrid()
            {
               /**
                * Add the foreground image
                */
                Grid.SetRow(mForegroundImage, 0);
                Grid.SetColumn(mForegroundImage, 0);

                /**
                 * Add the TextBlock widget
                 */
                Grid.SetRow(mText, 0);
                Grid.SetColumn(mText, 0);


                /**
                 * Add the background image
                 */
                Grid.SetRow(mBackgroundImage, 0);
                Grid.SetColumn(mBackgroundImage, 0);

                /**
                 * Make the text, the foreground and background image children of the mGrid object
                 */
                mGrid.Children.Add(mBackgroundImage);
                mGrid.Children.Add(mForegroundImage);
                mGrid.Children.Add(mText);
            }

			/**
			 * Function that creates the background image
			 */
			private void createBackgroundImage()
			{
				mBackgroundImage = new System.Windows.Controls.Image();
				mBackgroundImage.VerticalAlignment = VerticalAlignment.Center;
				mBackgroundImage.HorizontalAlignment = HorizontalAlignment.Center;
			}

			/**
			 * Constructor
			 */
			public ImageButton()
			{
				mButton = new System.Windows.Controls.Button();

				mStretchForeground = new System.Windows.Media.Stretch();
				mStretchForeground = System.Windows.Media.Stretch.None;

				mStretchBackground = new System.Windows.Media.Stretch();
				mStretchBackground = System.Windows.Media.Stretch.Fill;

				this.createBackgroundImage();
				this.createTextBlock();
				this.createForegroundImage();
                
                /**
                 * Grid object that holds the text and the foreground and background images
                 * It has only one row and one column
                 * It will display whe widgets one on top of each other
                 */
                mGrid = new System.Windows.Controls.Grid();                
                this.AddWidgetsToGrid();

				mButton.Content = mGrid;

				mButton.HorizontalAlignment = HorizontalAlignment.Left;
				mButton.VerticalAlignment = VerticalAlignment.Top;

				fillSpaceVerticalyEnabled = false;
				fillSpaceHorizontalyEnabled = false;

				//the click handle the button component
				mButton.Click += new RoutedEventHandler(
					delegate(Object from, RoutedEventArgs evt)
					{
						//create a Memory object of 8 Bytes
						Memory eventData = new Memory(8);

						//starting with the 0 Byte we write the eventType
						const int MAWidgetEventData_eventType = 0;
						//starting with the 4th Byte we write the widgetHandle
						const int MAWidgetEventData_widgetHandle = 4;

						eventData.WriteInt32(MAWidgetEventData_eventType, MoSync.Constants.MAW_EVENT_CLICKED);
						eventData.WriteInt32(MAWidgetEventData_widgetHandle, mHandle);
						//posting a CustomEvent
						mRuntime.PostCustomEvent(MoSync.Constants.EVENT_TYPE_WIDGET, eventData);
			});

				mView = mButton;
			}

			/**
			 * Implementation of the Text property
			 */
			[MoSyncWidgetProperty(MoSync.Constants.MAW_IMAGE_BUTTON_TEXT)]
			public String Text
			{
				set
				{
					mText.Text = value;
				}

				get
				{
					return mText.Text;
				}
			}

			/**
			 * Implementation of the TextHorizontalAlignment property. 
			 * Sets the horizontal alignment of the widget
			 */
			[MoSyncWidgetProperty(MoSync.Constants.MAW_IMAGE_BUTTON_TEXT_HORIZONTAL_ALIGNMENT)]
			public string TextHorizontalAlignment
			{
				set
				{
					if (value.Equals(MoSync.Constants.MAW_ALIGNMENT_LEFT))
					{
						mText.TextAlignment = TextAlignment.Left;
						mButton.HorizontalContentAlignment = HorizontalAlignment.Left;
					}
					else
					{
						if (value.Equals(MoSync.Constants.MAW_ALIGNMENT_RIGHT))
						{
							mText.TextAlignment = TextAlignment.Right;
							mButton.HorizontalContentAlignment = HorizontalAlignment.Right;
						}
						else
						{
							if (value.Equals(MoSync.Constants.MAW_ALIGNMENT_CENTER))
							{
								mText.TextAlignment = TextAlignment.Center;
								mButton.HorizontalContentAlignment = HorizontalAlignment.Center;
							}
						}
					}
				}
			}

			/**
			 * Implementation of the TextVerticalAlignment property. 
			 * Sets the vertical alignment of the widget
			 */
			[MoSyncWidgetProperty(MoSync.Constants.MAW_IMAGE_BUTTON_TEXT_VERTICAL_ALIGNMENT)]
			public String TextVerticalAlignment
			{
				set
				{
					if (value.Equals(MoSync.Constants.MAW_ALIGNMENT_TOP))
					{
						mText.VerticalAlignment = VerticalAlignment.Top; //todo: rewrite this
						mButton.VerticalContentAlignment = VerticalAlignment.Top;                       
					}
					else
					{
						if (value.Equals(MoSync.Constants.MAW_ALIGNMENT_BOTTOM))
						{
							mText.VerticalAlignment = VerticalAlignment.Bottom;
							mButton.VerticalContentAlignment = VerticalAlignment.Bottom;                            
						}
						else
						{
							if (value.Equals(MoSync.Constants.MAW_ALIGNMENT_CENTER))
							{
								mText.VerticalAlignment = VerticalAlignment.Center;
								mButton.VerticalContentAlignment = VerticalAlignment.Center;
							}
						}
					}
				}
			}

			/**
			 * Implementation of the FontColor property. 
			 * Sets the font color of the text displayed on the widget
			 */
			[MoSyncWidgetProperty(MoSync.Constants.MAW_IMAGE_BUTTON_FONT_COLOR)]
			public String FontColor
			{
				set
				{
					System.Windows.Media.SolidColorBrush brush;
					MoSync.Util.convertStringToColor(value, out brush);
					mText.Foreground = brush;
				}
			}

			/**
			 * Implementation of the FontSize property. 
			 * Sets the font size of the text displayed on the widget
			 */
			[MoSyncWidgetProperty(MoSync.Constants.MAW_IMAGE_BUTTON_FONT_SIZE)]
			public String FontSize
			{
				set
				{
					double size;
					if (double.TryParse(value, out size))
					{
						mText.FontSize = size;
					}
				}
			}

			/**
			 * Implementation of the Image property. 
			 * Sets the foreground image on the button
			 */
			[MoSyncWidgetProperty(MoSync.Constants.MAW_IMAGE_BUTTON_IMAGE)]
			public String Image
			{
				set
				{
					int val = 0;
					if (!int.TryParse(value, out val))
					{
						return;
					}
					Resource res = mRuntime.GetResource(MoSync.Constants.RT_IMAGE, val);
					if (null != res && res.GetInternalObject() != null)
					{

                        /**
                         * Set the height and width of the foreground image
                         * The image will be scaled so that the width and height are equal to the TextBlock height.
                         */
                        mForegroundImage.Width = mText.Height;
						mForegroundImage.Height = mText.Height;

						mForegroundImage.Stretch = mStretchForeground;

						System.Windows.Media.Imaging.BitmapSource bmpSource =
							(System.Windows.Media.Imaging.BitmapSource)(res.GetInternalObject());

						mForegroundImage.Source = bmpSource;
					}
				}
			}

			/**
			 * Implementation of the BackgroundImage property. 
			 * Sets the background image on the button
			 */
			[MoSyncWidgetProperty(MoSync.Constants.MAW_IMAGE_BUTTON_BACKGROUND_IMAGE)]
			public String BackgroundImage
			{
				set
				{
					int val = 0;
					if (!int.TryParse(value, out val))
					{
						return;
					}
					Resource res = mRuntime.GetResource(MoSync.Constants.RT_IMAGE, val);
					if (null != res && res.GetInternalObject() != null)
					{
						mBackgroundImage.Width = mButton.Width;
						mBackgroundImage.Height = mButton.Height;

						mBackgroundImage.Stretch = mStretchBackground;

						System.Windows.Media.Imaging.BitmapSource bmpSource =
							(System.Windows.Media.Imaging.BitmapSource)(res.GetInternalObject());

						mBackgroundImage.Source = bmpSource;
					}
				}
			}

            [MoSyncWidgetProperty(MoSync.Constants.MAW_IMAGE_BUTTON_FONT_HANDLE)]
            public String FontHandle
            {
                set
                {
                   
                }
            }
		}
	}
}
