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
 * @brief This represents the Button Widget implementation for the NativeUI
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
		public class ImageButton : WidgetBaseWindowsPhone
		{
			protected System.Windows.Controls.Button mButton;

			protected System.Windows.Controls.Grid mGrid;
			protected RowDefinition mRow;
			protected ColumnDefinition mColumn1;
			protected ColumnDefinition mColumn2;

			protected System.Windows.Controls.TextBlock mText;
			protected System.Windows.Controls.Image mForegroundImage;

			protected System.Windows.Controls.Image mBackgroundImage;
			protected System.Windows.Media.Stretch mStretch;

			private void createTextBlock()
			{
				mText = new System.Windows.Controls.TextBlock();
				mText.TextWrapping = TextWrapping.NoWrap;

				mText.Margin = new Thickness(10);
				mText.VerticalAlignment = VerticalAlignment.Top;
				mText.HorizontalAlignment = HorizontalAlignment.Center;
				mText.TextAlignment = TextAlignment.Center;
			}

			private void createForegroundImageLayout()
			{
				mGrid = new System.Windows.Controls.Grid();

				mForegroundImage = new System.Windows.Controls.Image();
				mForegroundImage.VerticalAlignment = VerticalAlignment.Center;
				mForegroundImage.HorizontalAlignment = HorizontalAlignment.Left;

				mColumn1 = new ColumnDefinition();
				mColumn1.Width = new GridLength(1, GridUnitType.Auto);

				mColumn2 = new ColumnDefinition();
				mColumn2.Width = new GridLength(1, GridUnitType.Star);

				mRow = new RowDefinition();
				mRow.Height = new GridLength(1, GridUnitType.Auto);

				mGrid.RowDefinitions.Add(mRow);
				mGrid.ColumnDefinitions.Add(mColumn1);
				mGrid.ColumnDefinitions.Add(mColumn2);

				Grid.SetRow(mForegroundImage, 0);
				Grid.SetColumn(mForegroundImage, 0);

				Grid.SetRow(mText, 0);
				Grid.SetColumnSpan(mText, 1);
				Grid.SetColumn(mText, 1);

				mGrid.Children.Add(mForegroundImage);
				mGrid.Children.Add(mText);
			}

			private void createBackgroundImage()
			{
				mBackgroundImage = new System.Windows.Controls.Image();
				mBackgroundImage.VerticalAlignment = VerticalAlignment.Top;
				mBackgroundImage.HorizontalAlignment = HorizontalAlignment.Left;
			}

			public ImageButton()
			{
				mButton = new System.Windows.Controls.Button();
				mStretch = new System.Windows.Media.Stretch();
				mStretch = System.Windows.Media.Stretch.None;

				this.createBackgroundImage();
				this.createTextBlock();
				this.createForegroundImageLayout();
 
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

			//MAW_BUTTON_TEXT property implementation
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

			[MoSyncWidgetProperty(MoSync.Constants.MAW_IMAGE_BUTTON_TEXT_HORIZONTAL_ALIGNMENT)]
			public string TextHorizontalAlignment
			{
				set
				{
					if (value.Equals(MoSync.Constants.MAW_ALIGNMENT_LEFT))
					{
						mButton.HorizontalContentAlignment = HorizontalAlignment.Left;
					}
					else
					{
						if (value.Equals(MoSync.Constants.MAW_ALIGNMENT_RIGHT))
						{
							mButton.HorizontalContentAlignment = HorizontalAlignment.Right;
						}
						else
						{
							if (value.Equals(MoSync.Constants.MAW_ALIGNMENT_CENTER))
							{
								mButton.HorizontalContentAlignment = HorizontalAlignment.Center;
							}
						}
					}
				}
			}

			[MoSyncWidgetProperty(MoSync.Constants.MAW_BUTTON_TEXT_VERTICAL_ALIGNMENT)]
			public String TextVerticalAlignment
			{
				set
				{
					if (value.Equals(MoSync.Constants.MAW_ALIGNMENT_TOP))
					{
						mButton.VerticalContentAlignment = VerticalAlignment.Top;
					}
					else
					{
						if (value.Equals(MoSync.Constants.MAW_ALIGNMENT_BOTTOM))
						{
							mButton.VerticalContentAlignment = VerticalAlignment.Bottom;
						}
						else
						{
							if (value.Equals(MoSync.Constants.MAW_ALIGNMENT_CENTER))
							{
								mButton.VerticalContentAlignment = VerticalAlignment.Center;
							}
						}
					}
				}
			}

			[MoSyncWidgetProperty(MoSync.Constants.MAW_IMAGE_BUTTON_FONT_COLOR)]
			public String FontColor
			{
				set
				{
					System.Windows.Media.SolidColorBrush brush;
					MoSync.Util.convertStringToColor(value, out brush);
					mButton.Foreground = brush;
				}
			}

			[MoSyncWidgetProperty(MoSync.Constants.MAW_IMAGE_BUTTON_FONT_COLOR)]
			public String FontSize
			{
				set
				{
					double size;
					if (double.TryParse(value, out size))
					{
						mButton.FontSize = size;
					}
				}
			}

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
						mText.TextAlignment = TextAlignment.Left;

						mForegroundImage.Width = mText.Height;
						mForegroundImage.Height = mText.Height;
						mForegroundImage.Margin = new Thickness(mText.Margin.Left, mText.Margin.Top, 0, mText.Margin.Bottom);

						mForegroundImage.Stretch = mStretch;

						System.Windows.Media.Imaging.BitmapSource bmpSource =
							(System.Windows.Media.Imaging.BitmapSource)(res.GetInternalObject());

						mForegroundImage.Source = bmpSource;
					}
				}
			}

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
						mBackgroundImage.Width = this.Height;
						mBackgroundImage.Height = this.Width;

						mStretch = System.Windows.Media.Stretch.Fill;
						mBackgroundImage.Stretch = mStretch;

						System.Windows.Media.Imaging.BitmapSource bmpSource =
							(System.Windows.Media.Imaging.BitmapSource)(res.GetInternalObject());

						mBackgroundImage.Source = bmpSource;
					}
				}
			}
		}
	}
}
