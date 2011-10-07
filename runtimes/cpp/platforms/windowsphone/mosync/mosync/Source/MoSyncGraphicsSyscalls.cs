using System;
using System.IO;
using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.GamerServices;
using System.Windows;
using System.Windows.Media.Imaging;
using System.Threading;
using System.Windows.Media;

namespace MoSync
{	
	public class GraphicsSyscalls : ISyscallGroup 
    {
        public WriteableBitmap mFrontBuffer;
        public WriteableBitmap mBackBuffer;
        public WriteableBitmap mCurrentDrawTarget;
        public int mCurrentDrawTargetIndex;
        private uint mCurrentColor;

        public void Init(Syscalls syscalls, Core core, Runtime runtime)
        {
            test_mosync.MainPage mainPage = runtime.GetMainPage();

            mBackBuffer = new WriteableBitmap(
                (int)mainPage.ViewPortContainer.Width,
                (int)mainPage.ViewPortContainer.Height);
            mFrontBuffer = new WriteableBitmap(
                (int)mainPage.ViewPortContainer.Width,
                (int)mainPage.ViewPortContainer.Height);

            mainPage.ViewPort.Source = mFrontBuffer;

            mCurrentDrawTarget = mBackBuffer;
            mCurrentDrawTargetIndex = MoSync.Constants.HANDLE_SCREEN;

            mCurrentColor = 0xff000000;

            syscalls.maSetColor = delegate(int rgb)
            {
                int oldColor = (int)mCurrentColor;
                mCurrentColor = 0xff000000 | (uint)(rgb & 0xffffff);
                return oldColor;
            };

            syscalls.maUpdateScreen = delegate()
            {
                mFrontBuffer.Blit(new Rect(0, 0, mBackBuffer.PixelWidth, mBackBuffer.PixelHeight),
                                    mBackBuffer,
                                    new Rect(0, 0, mFrontBuffer.PixelWidth, mFrontBuffer.PixelHeight));

                //mFrontBuffer.(mBackBuffer.Pixels);

                using (AutoResetEvent are = new AutoResetEvent(false))
                {
                    Deployment.Current.Dispatcher.BeginInvoke(() =>
                    {
                        mFrontBuffer.Invalidate();
                        are.Set();
                    });
                    are.WaitOne();
                }
            };

            syscalls.maFillRect = delegate(int x, int y, int w, int h)
            {
                mCurrentDrawTarget.FillRectangle(0, 0, w, h, (int)mCurrentColor);
            };

            syscalls.maLine = delegate(int x1, int y1, int x2, int y2)
            {
                mCurrentDrawTarget.DrawLine(x1, y1, x2, y2, (int)mCurrentColor);
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

            syscalls.maGetScrSize = delegate() { 
                return MoSync.Util.CreateExtent(mBackBuffer.PixelWidth, mBackBuffer.PixelHeight);
            };

            syscalls.maGetImageSize = delegate(int handle)
            {
                Resource res = runtime.GetResource(MoSync.Constants.RT_IMAGE, handle);
                BitmapSource src = (BitmapSource)res.GetInternalObject();
                int w = 0, h = 0;
                using (AutoResetEvent are = new AutoResetEvent(false))
                {
                    Deployment.Current.Dispatcher.BeginInvoke(() =>
                    {
                        w = src.PixelWidth;
                        h = src.PixelHeight;
                        are.Set();
                    });
                    are.WaitOne();
                }
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
                int srcRectX = dataMemory.ReadInt32(srcRectPtr+0);
                int srcRectY = dataMemory.ReadInt32(srcRectPtr+4);
                int srcRectW = dataMemory.ReadInt32(srcRectPtr+8);
                int srcRectH = dataMemory.ReadInt32(srcRectPtr+12);
                int dstPointX = dataMemory.ReadInt32(dstPointPtr+0);
                int dstPointY = dataMemory.ReadInt32(dstPointPtr+4);

                Rect srcRect = new Rect(srcRectX, srcRectY, srcRectW, srcRectH);
                Rect dstRect = new Rect(dstPointX, dstPointY, srcRectW, srcRectH);
                mCurrentDrawTarget.Blit(dstRect, src, srcRect, WriteableBitmapExtensions.BlendMode.Alpha);
            };

            syscalls.maCreateDrawableImage = delegate(int placeholder, int width, int height)
            {
                Resource res = runtime.GetResource(MoSync.Constants.RT_PLACEHOLDER, placeholder);
                res.SetResourceType(MoSync.Constants.RT_IMAGE);
                WriteableBitmap bitmap = null;
                using (AutoResetEvent are = new AutoResetEvent(false))
                {
                    Deployment.Current.Dispatcher.BeginInvoke(() =>
                    {
                        bitmap = new WriteableBitmap(width, height);
                        are.Set();
                    });
                    are.WaitOne();
                }


                if (bitmap == null) return MoSync.Constants.RES_OUT_OF_MEMORY;
                res.SetInternalObject(bitmap);
                return MoSync.Constants.RES_OK;
            };
        }
	}
}

