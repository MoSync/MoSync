using System;
using System.IO;
using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.GamerServices;
using System.Windows;
using System.Windows.Media.Imaging;
using System.Threading;
using System.Windows.Media;
using System.Windows.Controls;
using Microsoft.Phone.Controls;

namespace MoSync
{
	public class GraphicsModule : ISyscallModule, IIoctlModule
	{
		public WriteableBitmap mFrontBuffer;
		public WriteableBitmap mBackBuffer;
		public WriteableBitmap mCurrentDrawTarget;
		public int mCurrentDrawTargetIndex = MoSync.Constants.HANDLE_SCREEN;
		private uint mCurrentColor = 0xff000000;
		private System.Windows.Media.Color mCurrentWindowsColor;
		private double mCurrentFontSize = 24;

		public static void DrawImageRegion(WriteableBitmap dst, int left, int top,
				Rect srcRect, WriteableBitmap img, int transformMode)
		{
			int width = (int)srcRect.Width,
				height = (int)srcRect.Height,
				u = (int)srcRect.X,
				v = (int)srcRect.Y;

			int bpp = 1,
				srcPitchX,
				srcPitchY,
				transTopLeftX,
				transTopLeftY,
				transBottomRightX,
				transBottomRightY,
				transWidth,
				transHeight;

			int dirHorizontalX = 0,
				dirHorizontalY = 0,
				dirVerticalX = 0,
				dirVerticalY = 0;

			switch (transformMode)
			{
				case MoSync.Constants.TRANS_NONE:
					srcPitchX = bpp;
					srcPitchY = img.PixelWidth;
					transTopLeftX = u;
					transTopLeftY = v;
					transBottomRightX = u + width - 1;
					transBottomRightY = v + height - 1;
					transWidth = width;
					transHeight = height;
					dirHorizontalX = 1;
					dirVerticalY = 1;
					break;
				case MoSync.Constants.TRANS_ROT90:
					srcPitchX = -img.PixelWidth;
					srcPitchY = bpp;
					transTopLeftX = u;
					transTopLeftY = v + height - 1;
					transBottomRightX = u + width - 1;
					transBottomRightY = v;
					transWidth = height;
					transHeight = width;
					dirHorizontalY = -1;
					dirVerticalX = 1;
					break;
				case MoSync.Constants.TRANS_ROT180:
					srcPitchX = -bpp;
					srcPitchY = -img.PixelWidth;
					transTopLeftX = u + width - 1;
					transTopLeftY = v + height - 1;
					transBottomRightX = u;
					transBottomRightY = v;
					transWidth = width;
					transHeight = height;
					dirHorizontalX = -1;
					dirVerticalY = -1;
					break;
				case MoSync.Constants.TRANS_ROT270:
					srcPitchX = img.PixelWidth;
					srcPitchY = -bpp;
					transTopLeftX = u + width - 1;
					transTopLeftY = v;
					transBottomRightX = u;
					transBottomRightY = v + height - 1;
					transWidth = height;
					transHeight = width;
					dirHorizontalY = 1;
					dirVerticalX = -1;
					break;
				case MoSync.Constants.TRANS_MIRROR:
					srcPitchX = -bpp;
					srcPitchY = img.PixelWidth;
					transTopLeftX = u + width - 1;
					transTopLeftY = v;
					transBottomRightX = u;
					transBottomRightY = v + height - 1;
					transWidth = width;
					transHeight = height;
					dirHorizontalX = -1;
					dirVerticalY = 1;
					break;
				case MoSync.Constants.TRANS_MIRROR_ROT90:
					srcPitchX = -img.PixelWidth;
					srcPitchY = -bpp;
					transTopLeftX = u + width - 1;
					transTopLeftY = v + height - 1;
					transBottomRightX = u;
					transBottomRightY = v;
					transWidth = height;
					transHeight = width;
					dirHorizontalY = -1;
					dirVerticalX = -1;
					break;
				case MoSync.Constants.TRANS_MIRROR_ROT180:
					srcPitchX = bpp;
					srcPitchY = -img.PixelWidth;
					transTopLeftX = u;
					transTopLeftY = v + height - 1;
					transBottomRightX = u + width - 1;
					transBottomRightY = v;
					transWidth = width;
					transHeight = height;
					dirHorizontalX = 1;
					dirVerticalY = -1;
					break;
				case MoSync.Constants.TRANS_MIRROR_ROT270:
					srcPitchX = img.PixelWidth;
					srcPitchY = bpp;
					transTopLeftX = u;
					transTopLeftY = v;
					transBottomRightX = u + width - 1;
					transBottomRightY = v + height - 1;
					transWidth = height;
					transHeight = width;
					dirHorizontalY = 1;
					dirVerticalX = 1;
					break;
				default:
					MoSync.Util.CriticalError("Invalid transform!");
					return;
			}

			if (transWidth <= 0 || transHeight <= 0) return;

			if (transTopLeftX >= img.PixelWidth) { MoSync.Util.CriticalError("Source rect error!"); }
			else if (transTopLeftX < 0) { MoSync.Util.CriticalError("Source rect error!"); }
			if (transTopLeftY >= img.PixelHeight) { MoSync.Util.CriticalError("Source rect error!"); }
			else if (transTopLeftY < 0) { MoSync.Util.CriticalError("Source rect error!"); }
			if (transBottomRightX < 0) { MoSync.Util.CriticalError("Source rect error!"); }
			else if (transBottomRightX >= img.PixelWidth) { MoSync.Util.CriticalError("Source rect error!"); }
			if (transBottomRightY < 0) { MoSync.Util.CriticalError("Source rect error!"); }
			else if (transBottomRightY >= img.PixelHeight) { MoSync.Util.CriticalError("Source rect error!"); }

			if (transWidth <= 0 || transHeight <= 0) return;

			/*
			if (left >= clipRect.x + clipRect.width)
				return;
			else if(left < clipRect.x) {
				transTopLeftX += (clipRect.x - left)*dirHorizontalX;
				transTopLeftY += (clipRect.x - left)*dirHorizontalY;
				transWidth -= clipRect.x - left;
				left = clipRect.x;
			}
			if (top >= clipRect.y + clipRect.height)
				return;
			else if(top < clipRect.y) {
				transTopLeftX += (clipRect.y - top)*dirVerticalX;
				transTopLeftY += (clipRect.y - top)*dirVerticalY;
				transHeight -= clipRect.y - top;
				top = clipRect.y;
			}
			if(left + transWidth < clipRect.x)
				return;
			else if(left + transWidth >= clipRect.x + clipRect.width)
				transWidth -= (left + transWidth) - (clipRect.x + clipRect.width);
			if(top + height < clipRect.y)
				return;
			else if(top + transHeight >= clipRect.y + clipRect.height)
				transHeight -= (top + transHeight) - (clipRect.y + clipRect.height);
			*/

			if (transWidth <= 0 || transHeight <= 0) return;

			/*
			unsigned char *dst = &data[left*bytesPerPixel + top*pitch];
			unsigned char *src = &img->data[transTopLeftX*img->bytesPerPixel + transTopLeftY*img->pitch];
			*/

			int dstPixel = left + top * dst.PixelWidth;
			int srcPixel = transTopLeftX + transTopLeftY * img.PixelWidth;

			while (transHeight-- != 0)
			{
				int x = transWidth;
				int dstScan = dstPixel;
				int srcScan = srcPixel;
				while (x-- != 0)
				{
					uint srcCol = (uint)img.Pixels[srcScan];
					uint dstCol = (uint)dst.Pixels[dstScan];
					uint sa = (((srcCol) & 0xff000000) >> 24);

					if (sa == 255)
					{
						dst.Pixels[dstScan] = (int)srcCol;
					}
					else if (sa == 0)
					{
						dst.Pixels[dstScan] = (int)dstCol;
					}
					else
					{
						uint sr = (((srcCol) & 0x00ff0000) >> 16);
						uint sg = (((srcCol) & 0x0000ff00) >> 8);
						uint sb = (((srcCol) & 0x000000ff) >> 0);

						uint dr = (((dstCol) & 0x00ff0000) >> 16);
						uint dg = (((dstCol) & 0x0000ff00) >> 8);
						uint db = (((dstCol) & 0x000000ff) >> 0);

						dst.Pixels[dstScan] = (int)(
									(((dr + (((sr - dr) * (sa)) >> 8)) << 16) & 0x00ff0000) |
									(((dg + (((sg - dg) * (sa)) >> 8)) << 8) & 0x0000ff00) |
									(((db + (((sb - db) * (sa)) >> 8)) << 0) & 0x000000ff));
					}
					srcScan += srcPitchX;
					dstScan++;
				}
				srcPixel += srcPitchY;
				dstPixel += dst.PixelWidth;
			}
		}

		protected void InvalidateWriteableBitmapOnMainThread(WriteableBitmap bitmap)
		{
			MoSync.Util.RunActionOnMainThreadSync(() =>
			{
				bitmap.Invalidate();
			});
		}

		public void Init(Syscalls syscalls, Core core, Runtime runtime)
		{
			PhoneApplicationFrame frame = (PhoneApplicationFrame)Application.Current.RootVisual;
			double screenWidth = System.Windows.Application.Current.Host.Content.ActualWidth;
			double screenHeight = System.Windows.Application.Current.Host.Content.ActualHeight;
			if ((int)screenHeight == 0)
				throw new Exception("screenHeight");
			PhoneApplicationPage mainPage = (PhoneApplicationPage)frame.Content;
			Image mainImage = new Image();
			mainPage.Width = screenWidth;
			mainPage.Height = screenHeight;
			mainImage.Width = screenWidth;
			mainImage.Height = screenHeight;
			mainPage.Content = mainImage;

			// no apparent effect on memory leaks.
			runtime.RegisterCleaner(delegate()
			{
				MoSync.Util.RunActionOnMainThreadSync(() =>
				{
					mainPage.Content = null;
				});
			});

			mBackBuffer = new WriteableBitmap(
				(int)screenWidth,
				(int)screenHeight);
			mFrontBuffer = new WriteableBitmap(
				(int)screenWidth,
				(int)screenHeight);

			mainImage.Source = mFrontBuffer;
			mCurrentDrawTarget = mBackBuffer;

			mCurrentWindowsColor = System.Windows.Media.Color.FromArgb(0xff,
						(byte)(mCurrentColor >> 16),
						(byte)(mCurrentColor >> 8),
						(byte)(mCurrentColor));

			syscalls.maSetColor = delegate(int rgb)
			{
				int oldColor = (int)mCurrentColor;
				mCurrentColor = 0xff000000 | (uint)(rgb & 0xffffff);
				mCurrentWindowsColor = System.Windows.Media.Color.FromArgb(0xff,
						(byte)(mCurrentColor >> 16),
						(byte)(mCurrentColor >> 8),
						(byte)(mCurrentColor));
				return oldColor & 0xffffff;
			};

			syscalls.maSetClipRect = delegate(int x, int y, int w, int h)
			{
			};

			syscalls.maGetClipRect = delegate(int cliprect)
			{
			};

			syscalls.maPlot = delegate(int x, int y)
			{
				mCurrentDrawTarget.SetPixel(x, y, (int)mCurrentColor);
			};

			syscalls.maUpdateScreen = delegate()
			{
				System.Array.Copy(mBackBuffer.Pixels, mFrontBuffer.Pixels, mFrontBuffer.PixelWidth * mFrontBuffer.PixelHeight);
				InvalidateWriteableBitmapOnMainThread(mFrontBuffer);
			};

			syscalls.maFillRect = delegate(int x, int y, int w, int h)
			{
				mCurrentDrawTarget.FillRectangle(x, y, x + w, y + h, (int)mCurrentColor);
			};

			syscalls.maLine = delegate(int x1, int y1, int x2, int y2)
			{
				mCurrentDrawTarget.DrawLine(x1, y1, x2, y2, (int)mCurrentColor);
			};

			TextBlock textBlock = new TextBlock();
			textBlock.FontSize = mCurrentFontSize;

			syscalls.maDrawText = delegate(int left, int top, int str)
			{
				String text = core.GetDataMemory().ReadStringAtAddress(str);
				if (text.Length == 0) return;

				MoSync.Util.RunActionOnMainThreadSync(() =>
				{
					textBlock.Text = text;
					textBlock.Foreground = new SolidColorBrush(mCurrentWindowsColor);
					WriteableBitmap b = new WriteableBitmap(textBlock, null);
					mCurrentDrawTarget.Blit(new Rect(left, top, b.PixelWidth, b.PixelHeight),
						b,
						new Rect(0, 0, b.PixelWidth, b.PixelHeight));
				});
			};

			syscalls.maGetTextSize = delegate(int str)
			{
				String text = core.GetDataMemory().ReadStringAtAddress(str);
				int textWidth = 0;
				int textHeight = 0;

				MoSync.Util.RunActionOnMainThreadSync(() =>
				{
					textBlock.Text = text;
					textWidth = (int)textBlock.ActualWidth;
					textHeight = (int)textBlock.ActualHeight;
				});

				return MoSync.Util.CreateExtent(textWidth, textHeight);
			};

			syscalls.maDrawTextW = delegate(int left, int top, int str)
			{
				String text = core.GetDataMemory().ReadWStringAtAddress(str);
				if (text.Length == 0) return;

				MoSync.Util.RunActionOnMainThreadSync(() =>
				{
					textBlock.Text = text;
					textBlock.Foreground = new SolidColorBrush(mCurrentWindowsColor);
					WriteableBitmap b = new WriteableBitmap(textBlock, null);
					Rect dstRect = new Rect(left, top, b.PixelWidth, b.PixelHeight);
					Rect srcRect = new Rect(0, 0, b.PixelWidth, b.PixelHeight);
					// cliprect..
					Rect clipRect = new Rect(0, 0, mBackBuffer.PixelWidth, mBackBuffer.PixelHeight);
					clipRect.Intersect(dstRect);
					if (clipRect.IsEmpty == true)
					{
						return;
					}

					mCurrentDrawTarget.Blit(dstRect,
						b,
						srcRect);
				});
			};

			syscalls.maGetTextSizeW = delegate(int str)
			{
				String text = core.GetDataMemory().ReadWStringAtAddress(str);
				int textWidth = 0;
				int textHeight = 0;

				MoSync.Util.RunActionOnMainThreadSync(() =>
				{
					textBlock.Text = text;
					textWidth = (int)textBlock.ActualWidth;
					textHeight = (int)textBlock.ActualHeight;
				});

				return MoSync.Util.CreateExtent(textWidth, textHeight);
			};

			syscalls.maFillTriangleFan = delegate(int points, int count)
			{
				int[] newPoints = new int[count * 2 + 2];
				for (int i = 0; i < count; i++)
				{
					newPoints[i * 2 + 0] = core.GetDataMemory().ReadInt32(points + i * 8);
					newPoints[i * 2 + 1] = core.GetDataMemory().ReadInt32(points + i * 8 + 4);
				}
				newPoints[count * 2 + 0] = core.GetDataMemory().ReadInt32(points + 0);
				newPoints[count * 2 + 1] = core.GetDataMemory().ReadInt32(points + 4);
				mCurrentDrawTarget.FillPolygon(newPoints, (int)mCurrentColor);
			};

			syscalls.maFillTriangleStrip = delegate(int points, int count)
			{

				int[] xcoords = new int[count];
				int[] ycoords = new int[count];

				for (int i = 0; i < count; i++)
				{
					xcoords[i] = core.GetDataMemory().ReadInt32(points + i * 8);
					ycoords[i] = core.GetDataMemory().ReadInt32(points + i * 8 + 4);
				}

				for (int i = 2; i < count; i++)
				{
					mCurrentDrawTarget.FillTriangle(
						xcoords[i - 2], ycoords[i - 2],
						xcoords[i - 1], ycoords[i - 1],
						xcoords[i - 0], ycoords[i - 0],
						(int)mCurrentColor);
				}
			};

			syscalls.maSetDrawTarget = delegate(int drawTarget)
			{
				int oldDrawTarget = mCurrentDrawTargetIndex;
				if (drawTarget == mCurrentDrawTargetIndex) return oldDrawTarget;
				if (drawTarget == MoSync.Constants.HANDLE_SCREEN)
				{
					mCurrentDrawTarget = mBackBuffer;
					mCurrentDrawTargetIndex = drawTarget;
					return oldDrawTarget;
				}

				Resource res = runtime.GetResource(MoSync.Constants.RT_IMAGE, drawTarget);
				mCurrentDrawTarget = (WriteableBitmap)res.GetInternalObject();
				mCurrentDrawTargetIndex = drawTarget;
				return oldDrawTarget;
			};

			syscalls.maGetScrSize = delegate()
			{
				return MoSync.Util.CreateExtent(mBackBuffer.PixelWidth, mBackBuffer.PixelHeight);
			};

			syscalls.maGetImageSize = delegate(int handle)
			{
				Resource res = runtime.GetResource(MoSync.Constants.RT_IMAGE, handle);
				BitmapSource src = (BitmapSource)res.GetInternalObject();
				int w = 0, h = 0;

				MoSync.Util.RunActionOnMainThreadSync(() =>
				{
					w = src.PixelWidth;
					h = src.PixelHeight;
				});

				return MoSync.Util.CreateExtent(w, h);
			};

			syscalls.maDrawImage = delegate(int image, int left, int top)
			{
				Resource res = runtime.GetResource(MoSync.Constants.RT_IMAGE, image);
				WriteableBitmap src = (WriteableBitmap)res.GetInternalObject();
				Rect srcRect = new Rect(0, 0, src.PixelWidth, src.PixelHeight);
				Rect dstRect = new Rect(left, top, src.PixelWidth, src.PixelHeight);
				mCurrentDrawTarget.Blit(dstRect, src, srcRect, WriteableBitmapExtensions.BlendMode.Alpha);
			};

			syscalls.maDrawImageRegion = delegate(int image, int srcRectPtr, int dstPointPtr, int transformMode)
			{
				Resource res = runtime.GetResource(MoSync.Constants.RT_IMAGE, image);
				WriteableBitmap src = (WriteableBitmap)res.GetInternalObject();

				Memory dataMemory = core.GetDataMemory();
				int srcRectX = dataMemory.ReadInt32(srcRectPtr + 0);
				int srcRectY = dataMemory.ReadInt32(srcRectPtr + 4);
				int srcRectW = dataMemory.ReadInt32(srcRectPtr + 8);
				int srcRectH = dataMemory.ReadInt32(srcRectPtr + 12);
				int dstPointX = dataMemory.ReadInt32(dstPointPtr + 0);
				int dstPointY = dataMemory.ReadInt32(dstPointPtr + 4);

				Rect srcRect = new Rect(srcRectX, srcRectY, srcRectW, srcRectH);
				Rect dstRect = new Rect(dstPointX, dstPointY, srcRectW, srcRectH);
				// mCurrentDrawTarget.Blit(dstRect, src, srcRect, WriteableBitmapExtensions.BlendMode.Alpha);

				DrawImageRegion(mCurrentDrawTarget, dstPointX, dstPointY, srcRect, src, transformMode);
			};

			syscalls.maCreateDrawableImage = delegate(int placeholder, int width, int height)
			{
				Resource res = runtime.GetResource(MoSync.Constants.RT_PLACEHOLDER, placeholder);
				res.SetResourceType(MoSync.Constants.RT_IMAGE);
				WriteableBitmap bitmap = null;

				MoSync.Util.RunActionOnMainThreadSync(() =>
				{
					bitmap = new WriteableBitmap(width, height);
				});

				if (bitmap == null) return MoSync.Constants.RES_OUT_OF_MEMORY;
				res.SetInternalObject(bitmap);
				return MoSync.Constants.RES_OK;
			};

			syscalls.maCreateImageRaw = delegate(int _placeholder, int _src, int _size, int _alpha)
			{
				int width = MoSync.Util.ExtentX(_size);
				int height = MoSync.Util.ExtentY(_size);

				WriteableBitmap bitmap = null;
				MoSync.Util.RunActionOnMainThreadSync(() =>
					{
						bitmap = new WriteableBitmap(width, height);
					});

				//core.GetDataMemory().ReadIntegers(bitmap.Pixels, _src, width * height);
				bitmap.FromByteArray(core.GetDataMemory().GetData(), _src, width * height * 4);
				if (_alpha == 0)
				{
					int[] pixels = bitmap.Pixels;
					int numPixels = width * height;
					for (int i = 0; i < numPixels; i++)
					{
						pixels[i] = (int)((uint)pixels[i] | 0xff000000);
					}
				}

				runtime.SetResource(_placeholder,
					new Resource(
						bitmap,
						MoSync.Constants.RT_IMAGE
						)
					);
				return MoSync.Constants.RES_OK;
			};

			syscalls.maDrawRGB = delegate(int _dstPoint, int _src, int _srcRect, int _scanlength)
			{
				Memory dataMemory = core.GetDataMemory();
				int dstX = dataMemory.ReadInt32(_dstPoint + 0);
				int dstY = dataMemory.ReadInt32(_dstPoint + 4);
				int srcRectX = dataMemory.ReadInt32(_srcRect + 0);
				int srcRectY = dataMemory.ReadInt32(_srcRect + 4);
				int srcRectW = dataMemory.ReadInt32(_srcRect + 8);
				int srcRectH = dataMemory.ReadInt32(_srcRect + 12);
				int[] pixels = mCurrentDrawTarget.Pixels;
				// todo: clipRect

				_scanlength *= 4; // sizeof(int)

				for (int h = 0; h < srcRectH; h++)
				{
					int pixelIndex = dstY * mCurrentDrawTarget.PixelWidth + dstX;
					int address = _src + (srcRectY + h) * _scanlength;
					for (int w = 0; w < srcRectW; w++)
					{
						uint srcPixel = dataMemory.ReadUInt32(address);
						uint dstPixel = (uint)pixels[pixelIndex];

						uint srcPixelR = (srcPixel & 0x00ff0000) >> 16;
						uint srcPixelG = (srcPixel & 0x0000ff00) >> 8;
						uint srcPixelB = (srcPixel & 0x000000ff) >> 0;
						uint srcPixelA = (srcPixel & 0xff000000) >> 24;
						uint dstPixelR = (dstPixel & 0x00ff0000) >> 16;
						uint dstPixelG = (dstPixel & 0x0000ff00) >> 8;
						uint dstPixelB = (dstPixel & 0x000000ff) >> 0;
						uint dstPixelA = (dstPixel & 0xff000000) >> 24;

						dstPixelR += ((srcPixelR - dstPixelR) * srcPixelA) / 255;
						dstPixelG += ((srcPixelG - dstPixelG) * srcPixelA) / 255;
						dstPixelB += ((srcPixelB - dstPixelB) * srcPixelA) / 255;

						dstPixel = (dstPixelA << 24) | (dstPixelR << 16) | (dstPixelG << 8) | (dstPixelB);
						pixels[pixelIndex] = (int)dstPixel;

						address += 4;
						pixelIndex++;
					}

					dstY++;
				}
			};

			syscalls.maGetImageData = delegate(int _image, int _dst, int _srcRect, int _scanlength)
			{
				Resource res = runtime.GetResource(MoSync.Constants.RT_IMAGE, _image);
				WriteableBitmap src = (WriteableBitmap)res.GetInternalObject();
				Memory dataMemory = core.GetDataMemory();
				int srcRectX = dataMemory.ReadInt32(_srcRect + 0);
				int srcRectY = dataMemory.ReadInt32(_srcRect + 4);
				int srcRectW = dataMemory.ReadInt32(_srcRect + 8);
				int srcRectH = dataMemory.ReadInt32(_srcRect + 12);
				int lineDst = _dst;
				byte[] data = src.ToByteArray(srcRectY * src.PixelWidth,
					srcRectH * src.PixelWidth);
				byte[] coreArray = dataMemory.GetData();
				for (int y = 0; y < srcRectH; y++)
				{
					System.Array.Copy(data, y * src.PixelWidth * 4, coreArray,
						lineDst, src.PixelWidth * 4);
					lineDst += _scanlength * 4;
				}
			};

			syscalls.maCreateImageFromData = delegate(int _placeholder, int _data, int _offset, int _size)
			{
				Resource res = runtime.GetResource(MoSync.Constants.RT_BINARY, _data);
				Memory mem = (Memory)res.GetInternalObject();

				Stream s = mem.GetStream(_offset, _size);
				WriteableBitmap bitmap = MoSync.Util.CreateWriteableBitmapFromStream(s);
				s.Close();
				runtime.SetResource(
					_placeholder,
					new Resource(
						bitmap,
						MoSync.Constants.RT_IMAGE
						)
				);

				return MoSync.Constants.RES_OK;
			};
		}

		protected Syscalls.Delegate_maUpdateScreen mOldUpdateScreenImplementation;
		public void Init(Ioctls ioctls, Core core, Runtime runtime)
		{

			ioctls.maFrameBufferInit = delegate(int frameBufferPointer)
			{
				Syscalls syscalls = runtime.GetSyscalls();
				mOldUpdateScreenImplementation = syscalls.maUpdateScreen;

				syscalls.maUpdateScreen = delegate()
				{
					int[] dst = mFrontBuffer.Pixels;
					Memory mem = core.GetDataMemory();
					for (int i = 0; i < dst.Length; i++)
					{
						dst[i] = (int)(0xff000000 | mem.ReadUInt32(frameBufferPointer + i * 4));
					}

					InvalidateWriteableBitmapOnMainThread(mFrontBuffer);
				};

				return 1;
			};

			ioctls.maFrameBufferClose = delegate()
			{
				Syscalls syscalls = runtime.GetSyscalls();
				syscalls.maUpdateScreen = mOldUpdateScreenImplementation;
				return 1;
			};

			ioctls.maFrameBufferGetInfo = delegate(int info)
			{
				const int MAFrameBufferInfo_sizeInBytes = 0;
				const int MAFrameBufferInfo_bytesPerPixel = 4;
				const int MAFrameBufferInfo_bitsPerPixel = 8;
				const int MAFrameBufferInfo_redMask = 12;
				const int MAFrameBufferInfo_redShift = 16;
				const int MAFrameBufferInfo_redBits = 20;
				const int MAFrameBufferInfo_greenMask = 24;
				const int MAFrameBufferInfo_greenShift = 28;
				const int MAFrameBufferInfo_greenBits = 32;
				const int MAFrameBufferInfo_blueMask = 36;
				const int MAFrameBufferInfo_blueShift = 40;
				const int MAFrameBufferInfo_blueBits = 44;
				const int MAFrameBufferInfo_width = 48;
				const int MAFrameBufferInfo_height = 52;
				const int MAFrameBufferInfo_pitch = 56;
				const int MAFrameBufferInfo_supportsGfxSyscalls = 60;

				Memory mem = core.GetDataMemory();
				mem.WriteInt32(info + MAFrameBufferInfo_sizeInBytes, mBackBuffer.PixelWidth * mBackBuffer.PixelHeight * 4);
				mem.WriteInt32(info + MAFrameBufferInfo_bytesPerPixel, 4);
				mem.WriteInt32(info + MAFrameBufferInfo_bitsPerPixel, 32);
				mem.WriteUInt32(info + MAFrameBufferInfo_redMask, 0x00ff0000);
				mem.WriteUInt32(info + MAFrameBufferInfo_redBits, 8);
				mem.WriteUInt32(info + MAFrameBufferInfo_redShift, 16);
				mem.WriteUInt32(info + MAFrameBufferInfo_greenMask, 0x0000ff00);
				mem.WriteUInt32(info + MAFrameBufferInfo_greenBits, 8);
				mem.WriteUInt32(info + MAFrameBufferInfo_greenShift, 8);
				mem.WriteUInt32(info + MAFrameBufferInfo_blueMask, 0x000000ff);
				mem.WriteUInt32(info + MAFrameBufferInfo_blueBits, 8);
				mem.WriteUInt32(info + MAFrameBufferInfo_blueShift, 0);
				mem.WriteInt32(info + MAFrameBufferInfo_width, mBackBuffer.PixelWidth);
				mem.WriteInt32(info + MAFrameBufferInfo_height, mBackBuffer.PixelHeight);
				mem.WriteInt32(info + MAFrameBufferInfo_pitch, mBackBuffer.PixelWidth * 4);
				mem.WriteUInt32(info + MAFrameBufferInfo_supportsGfxSyscalls, 0);
				return 1;
			};
		}
	}
}
