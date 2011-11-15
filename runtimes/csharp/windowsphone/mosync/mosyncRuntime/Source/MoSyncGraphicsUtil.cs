using System;
using System.IO;
using System.IO.IsolatedStorage;
using System.Windows;
using System.Threading;
using System.Windows.Media.Imaging;
using System.Collections.Generic;

using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.GamerServices;

namespace MoSync
{
	public class GraphicsUtil
	{
		private enum ClipResult
		{
			BOTH_IN = 0,
			BOTH_OUT,
			A_IN,
			B_IN
		};

		public struct Point
		{

			public Point(double x, double y)
			{
				this.x = x;
				this.y = y;
			}

			public void Set(Point p)
			{
				x = p.x;
				y = p.y;
			}

			public double x, y;
		};

		private static ClipResult clipTopLine(Point a, Point b, Point res, double top)
		{
			if (a.y < top && b.y < top) return ClipResult.BOTH_OUT;
			ClipResult clipResult;
			if ((a.y >= top && b.y < top))
			{
				clipResult = ClipResult.A_IN;
			}
			else if ((a.y < top && b.y >= top))
			{
				clipResult = ClipResult.B_IN;
			}
			else
			{
				return ClipResult.BOTH_IN;
			}

			if (Math.Abs(b.y - a.y) < 1.0) return ClipResult.BOTH_OUT;
			res.x = a.x + (b.x - a.x) * (top - a.y) / (b.y - a.y);
			res.y = top;
			return clipResult;
		}

		private static ClipResult clipBottomLine(Point a, Point b, Point res, double bottom)
		{
			if (a.y > bottom && b.y > bottom) return ClipResult.BOTH_OUT;
			ClipResult clipResult;
			if ((a.y <= bottom && b.y > bottom))
			{
				clipResult = ClipResult.A_IN;
			}
			else if ((a.y > bottom && b.y <= bottom))
			{
				clipResult = ClipResult.B_IN;
			}
			else
			{
				return ClipResult.BOTH_IN;
			}

			if (Math.Abs(b.y - a.y) < 1.0) return ClipResult.BOTH_OUT;
			res.x = a.x + (b.x - a.x) * (bottom - a.y) / (b.y - a.y);
			res.y = bottom;
			return clipResult;
		}

		private static ClipResult clipLeftLine(Point a, Point b, Point res, double left)
		{
			if (a.x < left && b.x < left) return ClipResult.BOTH_OUT;
			ClipResult clipResult;
			if ((a.x >= left && b.x < left))
			{
				clipResult = ClipResult.A_IN;
			}
			else if ((a.x < left && b.x >= left))
			{
				clipResult = ClipResult.B_IN;
			}
			else
			{
				return ClipResult.BOTH_IN;
			}

			if (Math.Abs(b.x - a.x) < 1.0) return ClipResult.BOTH_OUT;
			res.x = left;
			res.y = a.y + (b.y - a.y) * ((left - a.x) / (b.x - a.x));
			return clipResult;
		}

		private static ClipResult clipRightLine(Point a, Point b, Point res, double right)
		{
			if (a.x > right && b.x > right) return ClipResult.BOTH_OUT;
			ClipResult clipResult;
			if ((a.x <= right && b.x > right))
			{
				clipResult = ClipResult.A_IN;
			}
			else if ((a.x > right && b.x <= right))
			{
				clipResult = ClipResult.B_IN;
			}
			else
			{
				return ClipResult.BOTH_IN;
			}

			if (Math.Abs(b.x - a.x) < 1.0) return ClipResult.BOTH_OUT;
			res.x = right;
			res.y = a.y + (b.y - a.y) * ((right - a.x) / (b.x - a.x));
			return clipResult;
		}

		public static bool ClipLine(Point a, Point b, int left, int right, int top, int bottom)
		{
			Point clippedPoint = new Point();
			ClipResult res;
			res = clipLeftLine(a, b, clippedPoint, left);
			switch (res)
			{
				case ClipResult.A_IN: b.Set(clippedPoint); break;
				case ClipResult.B_IN: a.Set(clippedPoint); break;
				case ClipResult.BOTH_OUT: return false;
				case ClipResult.BOTH_IN: break;
			}
			res = clipRightLine(a, b, clippedPoint, right);
			switch (res)
			{
				case ClipResult.A_IN: b.Set(clippedPoint); break;
				case ClipResult.B_IN: a.Set(clippedPoint); break;
				case ClipResult.BOTH_OUT: return false;
				case ClipResult.BOTH_IN: break;
			}

			res = clipTopLine(a, b, clippedPoint, top);
			switch (res)
			{
				case ClipResult.A_IN: b.Set(clippedPoint); break;
				case ClipResult.B_IN: a.Set(clippedPoint); break;
				case ClipResult.BOTH_OUT: return false;
				case ClipResult.BOTH_IN: break;
			}

			res = clipBottomLine(a, b, clippedPoint, bottom);
			switch (res)
			{
				case ClipResult.A_IN: b.Set(clippedPoint); break;
				case ClipResult.B_IN: a.Set(clippedPoint); break;
				case ClipResult.BOTH_OUT: return false;
				case ClipResult.BOTH_IN: break;
			}

			return true;
		}

		private static List<Point> clippedPoints1 = new List<Point>();
		private static List<Point> clippedPoints2 = new List<Point>();

		private static void clipPolygonTop(List<Point> src, List<Point> dst, double top)
		{
			dst.Clear();
			for (int i = 0; i < src.Count; i++)
			{
				int next = (i + 1) % src.Count;

				Point clippedPoint = new Point();
				ClipResult clipResult =
					clipTopLine(src[i], src[next], clippedPoint, top);
				switch (clipResult)
				{
					case ClipResult.A_IN:
						dst.Add(src[i]);
						dst.Add(clippedPoint);
						break;
					case ClipResult.B_IN:
						dst.Add(clippedPoint);
						break;
					case ClipResult.BOTH_IN:
						dst.Add(src[i]);
						break;
					case ClipResult.BOTH_OUT:
						break;
				}
			}
		}

		private static void clipPolygonLeft(List<Point> src, List<Point> dst, double left)
		{
			dst.Clear();
			for (int i = 0; i < src.Count; i++)
			{
				int next = (i + 1) % src.Count;

				Point clippedPoint = new Point();
				ClipResult clipResult =
					clipLeftLine(src[i], src[next], clippedPoint, left);
				switch (clipResult)
				{
					case ClipResult.A_IN:
						dst.Add(src[i]);
						dst.Add(clippedPoint);
						break;
					case ClipResult.B_IN:
						dst.Add(clippedPoint);
						break;
					case ClipResult.BOTH_IN:
						dst.Add(src[i]);
						break;
					case ClipResult.BOTH_OUT:
						break;
				}
			}
		}

		private static void clipPolygonRight(List<Point> src, List<Point> dst, double right)
		{
			dst.Clear();
			for (int i = 0; i < src.Count; i++)
			{
				int next = (i + 1) % src.Count;

				Point clippedPoint = new Point();
				ClipResult clipResult =
					clipRightLine(src[i], src[next], clippedPoint, right);
				switch (clipResult)
				{
					case ClipResult.A_IN:
						dst.Add(src[i]);
						dst.Add(clippedPoint);
						break;
					case ClipResult.B_IN:
						dst.Add(clippedPoint);
						break;
					case ClipResult.BOTH_IN:
						dst.Add(src[i]);
						break;
					case ClipResult.BOTH_OUT:
						break;
				}
			}
		}

		private static void clipPolygonBottom(List<Point> src, List<Point> dst, double bottom)
		{
			dst.Clear();
			for (int i = 0; i < src.Count; i++)
			{
				int next = (i + 1) % src.Count;

				Point clippedPoint = new Point();
				ClipResult clipResult =
					clipBottomLine(src[i], src[next], clippedPoint, bottom);
				switch (clipResult)
				{
					case ClipResult.A_IN:
						dst.Add(src[i]);
						dst.Add(clippedPoint);
						break;
					case ClipResult.B_IN:
						dst.Add(clippedPoint);
						break;
					case ClipResult.BOTH_IN:
						dst.Add(src[i]);
						break;
					case ClipResult.BOTH_OUT:
						break;
				}
			}
		}

		public static List<Point> ClipPolygon(List<Point> points, double left, double top, double right, double bottom)
		{
			clipPolygonTop(points, clippedPoints2, top);
			if (clippedPoints2.Count < 3) return null;
			clipPolygonLeft(clippedPoints2, clippedPoints1, left);
			if (clippedPoints1.Count < 3) return null;
			clipPolygonRight(clippedPoints1, clippedPoints2, right);
			if (clippedPoints2.Count < 3) return null;
			clipPolygonBottom(clippedPoints2, clippedPoints1, bottom);
			if (clippedPoints1.Count < 3) return null;
			return clippedPoints1;
		}

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
	}
}
