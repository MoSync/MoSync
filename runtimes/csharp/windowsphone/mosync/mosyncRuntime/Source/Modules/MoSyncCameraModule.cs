using System;
using System.IO;
using System.Collections.Generic;
using Microsoft.Devices;
using System.Threading;
using System.Windows.Media.Imaging;
using System.Windows.Media;

namespace MoSync
{
    public class CameraModule : IIoctlModule
    {
		/*
		private PhotoCamera mCamera = null;
		private VideoBrush mVideoBrush = null;
		private EventHandler<ContentReadyEventArgs> mCameraSnapshotDelegate = null;

		bool GetCameraFormat(int index, out System.Windows.Size dim)
		{
			dim = new System.Windows.Size();
			IEnumerable<System.Windows.Size> res = mCamera.AvailableResolutions;
			if (res == null) return false;
			IEnumerator<System.Windows.Size> resolutions = res.GetEnumerator();
			resolutions.MoveNext();
			int number = 0;
			while (resolutions.Current != null)
			{
				if (index == number)
					break;
				number++;
			}

			dim = resolutions.Current;
			return true;
		}
		*/

		public void Init(Ioctls ioctls, Core core, Runtime runtime)
		{
			// Fix later :)
			/*
			mCamera = new PhotoCamera(CameraType.Primary);
			mVideoBrush = new VideoBrush();
			mVideoBrush.SetSource(mCamera);
			mVideoBrush.Stretch = Stretch.Uniform;

			runtime.RegisterCleaner(delegate()
			{
				mCamera.Dispose();
				mCamera = null;
			});

			// this should be set according to the orientation of
			// the device I guess.
			mVideoBrush.RelativeTransform = new CompositeTransform()
			{
				CenterX = 0.5,
				CenterY = 0.5,
				Rotation = 90
			};

			ioctls.maCameraFormat = delegate(int _index, int _fmt)
			{
				System.Windows.Size dim;
				if (GetCameraFormat(_index, out dim) == false)
					return MoSync.Constants.MA_CAMERA_RES_FAILED;

				core.GetDataMemory().WriteInt32(_fmt + MoSync.Struct.MA_CAMERA_FORMAT.width,
					(int)dim.Width);
				core.GetDataMemory().WriteInt32(_fmt + MoSync.Struct.MA_CAMERA_FORMAT.height,
					(int)dim.Height);

				return MoSync.Constants.MA_CAMERA_RES_OK;
			};

			ioctls.maCameraFormatNumber = delegate()
			{
				IEnumerable<System.Windows.Size> res = mCamera.AvailableResolutions;
				if (res == null) return 0;
				IEnumerator<System.Windows.Size> resolutions = res.GetEnumerator();
				resolutions.MoveNext();
				int number = 0;
				while (resolutions.Current != null)
				{
					number++;
				}
				return number;
			};

			ioctls.maCameraStart = delegate()
			{
				return 0;
			};

			ioctls.maCameraStop = delegate()
			{
				return 0;
			};

			ioctls.maCameraSetPreview = delegate(int _widgetHandle)
			{
				// something like
				// videoBrush = ((CameraViewFinder)runtime.GetModule<MoSyncNativeUIModule>.GetWidget(_widgetHandle)).GetBrush();
				// videoBrush.SetSource(mCamera)
				IWidget w = runtime.GetModule<NativeUIModule>().GetWidget(_widgetHandle);
				if (w.GetType() != typeof(MoSync.NativeUI.CameraPreview))
				{
					return MoSync.Constants.MA_CAMERA_RES_FAILED;
				}
				NativeUI.CameraPreview prev = (NativeUI.CameraPreview)w;
				System.Windows.Controls.Canvas canvas = prev.GetViewFinderCanvas();
				MoSync.Util.RunActionOnMainThreadSync(() =>
				{
					canvas.Background = mVideoBrush;
				});

				return 0;
			};

			ioctls.maCameraSelect = delegate(int _cameraNumber)
			{
				CameraType cameraType = CameraType.Primary;
				if(_cameraNumber == MoSync.Constants.MA_CAMERA_CONST_BACK_CAMERA)
				{
					cameraType = CameraType.Primary;
				}
				else if(_cameraNumber == MoSync.Constants.MA_CAMERA_CONST_FRONT_CAMERA)
				{
					cameraType = CameraType.FrontFacing;
				}

				if(mCamera==null || mCamera.CameraType != cameraType)
				{
					mCamera = new PhotoCamera(cameraType);
					if(mVideoBrush == null)
						mVideoBrush = new VideoBrush();
					mVideoBrush.SetSource(mCamera);
				}

				return 0;
			};

			ioctls.maCameraNumber = delegate()
			{
				// front facing and back facing is the standard I believe.
				return 2;
			};

			ioctls.maCameraSnapshot = delegate(int _formatIndex, int _placeHolder)
			{
				AutoResetEvent are = new AutoResetEvent(false);

				System.Windows.Size dim;
				if (GetCameraFormat(_formatIndex, out dim) == false)
					return MoSync.Constants.MA_CAMERA_RES_FAILED;

				mCamera.Resolution = dim;

				if (mCameraSnapshotDelegate != null)
					mCamera.CaptureImageAvailable -= mCameraSnapshotDelegate;
				mCameraSnapshotDelegate = delegate(object o, ContentReadyEventArgs args)
				{
					MoSync.Util.RunActionOnMainThreadSync(() =>
					{
						Resource res = runtime.GetResource(MoSync.Constants.RT_PLACEHOLDER, _placeHolder);
						Stream data = args.ImageStream;
						MemoryStream dataMem = new MemoryStream((int)data.Length);
						MoSync.Util.CopySeekableStreams(data, 0, dataMem, 0, (int)data.Length);
						res.SetInternalObject(dataMem);
					});
					are.Set();
				};

				mCamera.CaptureImageAvailable += mCameraSnapshotDelegate;

				mCamera.CaptureImage();

				are.WaitOne();
				return 0;
			};

			ioctls.maCameraSetProperty = delegate(int _property, int _value)
			{
				return 0;
			};

			ioctls.maCameraSelect = delegate(int _camera)
			{
				return 0;
			};

			ioctls.maCameraGetProperty = delegate(int _property, int _value, int _bufSize)
			{
				String property = core.GetDataMemory().ReadStringAtAddress(_property);

				if (property.Equals(MoSync.Constants.MA_CAMERA_MAX_ZOOM))
				{
					core.GetDataMemory().WriteStringAtAddress(
						_value,
						"0",
						_bufSize);
				}

				return 0;
			};

			ioctls.maCameraRecord = delegate(int _stopStartFlag)
			{
				return 0;
			};

			*/
		}
	} // end class CameraModule
} // end namespace MoSync
