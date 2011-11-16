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
using System.Collections.Generic;

namespace MoSync
{
	public class GraphicsModule : ISyscallModule, IIoctlModule
	{
		public WriteableBitmap mFrontBuffer;
		public WriteableBitmap mBackBuffer;
		public WriteableBitmap mCurrentDrawTarget;
		private Rect mClipRect = new Rect();
		public int mCurrentDrawTargetIndex = MoSync.Constants.HANDLE_SCREEN;
		private uint mCurrentColor = 0xff000000;
		private System.Windows.Media.Color mCurrentWindowsColor;
		private double mCurrentFontSize = 24;

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

			mClipRect.X = 0.0;
			mClipRect.Y = 0.0;
			mClipRect.Width = screenWidth;
			mClipRect.Height = screenHeight;

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
				mClipRect.X = x;
				mClipRect.Y = y;
				mClipRect.Width = w;
				mClipRect.Height = h;
			};

			syscalls.maGetClipRect = delegate(int cliprect)
			{
				Memory mem = core.GetDataMemory();
				mem.WriteInt32(cliprect + MoSync.Struct.MARect.left, (int)mClipRect.X);
				mem.WriteInt32(cliprect + MoSync.Struct.MARect.top, (int)mClipRect.Y);
				mem.WriteInt32(cliprect + MoSync.Struct.MARect.width, (int)mClipRect.Width);
				mem.WriteInt32(cliprect + MoSync.Struct.MARect.height, (int)mClipRect.Height);
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
				GraphicsUtil.Point p1 = new GraphicsUtil.Point(x1, y1);
				GraphicsUtil.Point p2 = new GraphicsUtil.Point(x2, y2);
				if(!GraphicsUtil.ClipLine(p1, p2, (int)mClipRect.X, (int)(mClipRect.X+mClipRect.Width),
					(int)mClipRect.Y, (int)(mClipRect.Y+mClipRect.Height)))
					return;

				mCurrentDrawTarget.DrawLine((int)p1.x, (int)p1.y, (int)p2.x, (int)p2.y, (int)mCurrentColor);
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
					newPoints[i * 2 + 0] = core.GetDataMemory().ReadInt32(points + i * 8 + MoSync.Struct.MAPoint2d.x);
					newPoints[i * 2 + 1] = core.GetDataMemory().ReadInt32(points + i * 8 + MoSync.Struct.MAPoint2d.y);
				}
				newPoints[count * 2 + 0] = core.GetDataMemory().ReadInt32(points + MoSync.Struct.MAPoint2d.x);
				newPoints[count * 2 + 1] = core.GetDataMemory().ReadInt32(points + MoSync.Struct.MAPoint2d.y);
				mCurrentDrawTarget.FillPolygon(newPoints, (int)mCurrentColor);
			};

			syscalls.maFillTriangleStrip = delegate(int points, int count)
			{

				int[] xcoords = new int[count];
				int[] ycoords = new int[count];

				for (int i = 0; i < count; i++)
				{
					xcoords[i] = core.GetDataMemory().ReadInt32(points + i * 8 + MoSync.Struct.MAPoint2d.x);
					ycoords[i] = core.GetDataMemory().ReadInt32(points + i * 8 + MoSync.Struct.MAPoint2d.y);
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
				int srcRectX = dataMemory.ReadInt32(srcRectPtr + MoSync.Struct.MARect.left);
				int srcRectY = dataMemory.ReadInt32(srcRectPtr + MoSync.Struct.MARect.top);
				int srcRectW = dataMemory.ReadInt32(srcRectPtr + MoSync.Struct.MARect.width);
				int srcRectH = dataMemory.ReadInt32(srcRectPtr + MoSync.Struct.MARect.height);
				int dstPointX = dataMemory.ReadInt32(dstPointPtr + MoSync.Struct.MAPoint2d.x);
				int dstPointY = dataMemory.ReadInt32(dstPointPtr + MoSync.Struct.MAPoint2d.y);

				Rect srcRect = new Rect(srcRectX, srcRectY, srcRectW, srcRectH);
				Rect dstRect = new Rect(dstPointX, dstPointY, srcRectW, srcRectH);
				// mCurrentDrawTarget.Blit(dstRect, src, srcRect, WriteableBitmapExtensions.BlendMode.Alpha);

				GraphicsUtil.DrawImageRegion(mCurrentDrawTarget, dstPointX, dstPointY, srcRect, src, transformMode);
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
				int dstX = dataMemory.ReadInt32(_dstPoint + MoSync.Struct.MAPoint2d.x);
				int dstY = dataMemory.ReadInt32(_dstPoint + MoSync.Struct.MAPoint2d.y);
				int srcRectX = dataMemory.ReadInt32(_srcRect + MoSync.Struct.MARect.left);
				int srcRectY = dataMemory.ReadInt32(_srcRect + MoSync.Struct.MARect.top);
				int srcRectW = dataMemory.ReadInt32(_srcRect + MoSync.Struct.MARect.width);
				int srcRectH = dataMemory.ReadInt32(_srcRect + MoSync.Struct.MARect.height);
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
				int srcRectX = dataMemory.ReadInt32(_srcRect + MoSync.Struct.MARect.left);
				int srcRectY = dataMemory.ReadInt32(_srcRect + MoSync.Struct.MARect.top);
				int srcRectW = dataMemory.ReadInt32(_srcRect + MoSync.Struct.MARect.width);
				int srcRectH = dataMemory.ReadInt32(_srcRect + MoSync.Struct.MARect.height);
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
				Memory mem = core.GetDataMemory();
				mem.WriteInt32(info + MoSync.Struct.MAFrameBufferInfo.sizeInBytes, mBackBuffer.PixelWidth * mBackBuffer.PixelHeight * 4);
				mem.WriteInt32(info + MoSync.Struct.MAFrameBufferInfo.bytesPerPixel, 4);
				mem.WriteInt32(info + MoSync.Struct.MAFrameBufferInfo.bitsPerPixel, 32);
				mem.WriteUInt32(info + MoSync.Struct.MAFrameBufferInfo.redMask, 0x00ff0000);
				mem.WriteUInt32(info + MoSync.Struct.MAFrameBufferInfo.redBits, 8);
				mem.WriteUInt32(info + MoSync.Struct.MAFrameBufferInfo.redShift, 16);
				mem.WriteUInt32(info + MoSync.Struct.MAFrameBufferInfo.greenMask, 0x0000ff00);
				mem.WriteUInt32(info + MoSync.Struct.MAFrameBufferInfo.greenBits, 8);
				mem.WriteUInt32(info + MoSync.Struct.MAFrameBufferInfo.greenShift, 8);
				mem.WriteUInt32(info + MoSync.Struct.MAFrameBufferInfo.blueMask, 0x000000ff);
				mem.WriteUInt32(info + MoSync.Struct.MAFrameBufferInfo.blueBits, 8);
				mem.WriteUInt32(info + MoSync.Struct.MAFrameBufferInfo.blueShift, 0);
				mem.WriteInt32(info + MoSync.Struct.MAFrameBufferInfo.width, mBackBuffer.PixelWidth);
				mem.WriteInt32(info + MoSync.Struct.MAFrameBufferInfo.height, mBackBuffer.PixelHeight);
				mem.WriteInt32(info + MoSync.Struct.MAFrameBufferInfo.pitch, mBackBuffer.PixelWidth * 4);
				mem.WriteUInt32(info + MoSync.Struct.MAFrameBufferInfo.supportsGfxSyscalls, 0);
				return 1;
			};
		}
	}
}
