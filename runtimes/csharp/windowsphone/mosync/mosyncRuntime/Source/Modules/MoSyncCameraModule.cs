using System;
using System.IO;
using System.Collections.Generic;
using Microsoft.Devices;
using System.Threading;
using System.Windows;
using System.Windows.Media.Imaging;
using System.Windows.Media;
using Microsoft.Phone.Controls;

namespace MoSync
{
    public class CameraModule : IIoctlModule
    {
		/**
         * The camera object.
         */
		private PhotoCamera mCamera = null;

        /**
         * The view finder, in this case a video brush.
         */
		private VideoBrush mVideoBrush = null;

        /**
         * Event handler declaration for the camera snapshot.
         */
		private EventHandler<ContentReadyEventArgs> mCameraSnapshotDelegate = null;

        /**
         * The native ui widget used for drawing the camera image.
         */
        private NativeUI.CameraPreview mCameraPrev = null;

        /**
         * The camera type.
         */
        private CameraType mCameraType = CameraType.Primary;

        /**
         * The flash mode used.
         */
        private FlashMode mFlashMode = FlashMode.Off;

        /**
         * Used to check if the camera is initialized or not.
         */
        bool isCameraInitialized = false;

		bool GetCameraFormat(int index, out System.Windows.Size dim)
		{
			dim = new System.Windows.Size();

            // if the camera is not initialized, we cannot access any of its properties
            if (!isCameraInitialized)
            {
                return false;
            }

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

        /**
         * Initializing the ioctls.
         */
		public void Init(Ioctls ioctls, Core core, Runtime runtime)
		{
            mCamera = new PhotoCamera(mCameraType);
            mVideoBrush = new VideoBrush();

			runtime.RegisterCleaner(delegate()
			{
                if (null != mCamera)
                {
                    mCamera.Dispose();
                    mCamera = null;
                }
			});

			// this should be set according to the orientation of
			// the device I guess.

            // we need to handle the camera orientation by hand
            PhoneApplicationPage currentPage = (((PhoneApplicationFrame)Application.Current.RootVisual).Content as PhoneApplicationPage);

            // we need to handle the initial page orientation
            double rotation = mCamera.Orientation;
            if (currentPage.Orientation == PageOrientation.LandscapeLeft)
            {
                rotation -= 90;
            }
            else if (currentPage.Orientation == PageOrientation.LandscapeRight)
            {
                rotation += 90;
            }
            mVideoBrush.RelativeTransform = new CompositeTransform()
            {
                CenterX = 0.5,
                CenterY = 0.5,
                Rotation = rotation
            };

            // on orientation changed, we need to rotate the video brush
            currentPage.OrientationChanged += new System.EventHandler<OrientationChangedEventArgs>(
                delegate(object o, OrientationChangedEventArgs args)
                {
                    rotation = mCamera.Orientation;
                    if (args.Orientation == PageOrientation.LandscapeLeft)
                    {
                        rotation -= 90;
                    }
                    else if (args.Orientation == PageOrientation.LandscapeRight)
                    {
                        rotation += 90;
                    }

                    mVideoBrush.RelativeTransform = new CompositeTransform()
                    {
                        CenterX = 0.5,
                        CenterY = 0.5,
                        Rotation = rotation
                    };
                });

            /**
             * Stores an output format in fmm parameter.
             * @param _index int the index of the required format.
             * @param _fmt int the momory address at which to write the output format dimensions.
             *
             * Note: the _index should be greater than 0 and smaller than the number of camera formats.
             */
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

            /**
		    * Returns the number of different output formats supported by the current device's camera.
		    * \< 0 if there is no camera support.
		    * 0 if there is camera support, but the format is unknown.
		    */
			ioctls.maCameraFormatNumber = delegate()
			{
                // if the camera is not initialized, we cannot access any of its properties
                if (!isCameraInitialized)
                {
                    // because the cammera is supported but not initialized, we return 0
                    return 0;
                }

				IEnumerable<System.Windows.Size> res = mCamera.AvailableResolutions;
				if (res == null) return 0;
				IEnumerator<System.Windows.Size> resolutions = res.GetEnumerator();
				resolutions.MoveNext();
				int number = 0;
				while (resolutions.Current != null)
				{
					number++;
                    resolutions.MoveNext();
                    if (resolutions.Current == new System.Windows.Size(0, 0))
                        break;
				}
				return number;
			};

            /**
             * Starts the viewfinder and the camera
             */
			ioctls.maCameraStart = delegate()
			{
                initCamera();

                MoSync.Util.RunActionOnMainThreadSync(() =>
                {
                    mCameraPrev.StartViewFinder();
                });

				return 0;
			};

            /**
             * stops the view finder and the camera.
             */
			ioctls.maCameraStop = delegate()
			{
                MoSync.Util.RunActionOnMainThreadSync(() =>
                {
                    mCameraPrev.StopViewFinder();
                });

				return 0;
			};

            /**
             * Adds a previewWidget to the camera controller in devices that support native UI.
             */
			ioctls.maCameraSetPreview = delegate(int _widgetHandle)
			{
                // if the camera is not initialized, we need to initialize it before
                // setting the preview
                if (!isCameraInitialized)
                {
                    initCamera();
                }

				IWidget w = runtime.GetModule<NativeUIModule>().GetWidget(_widgetHandle);
				if (w.GetType() != typeof(MoSync.NativeUI.CameraPreview))
				{
					return MoSync.Constants.MA_CAMERA_RES_FAILED;
				}
				mCameraPrev = (NativeUI.CameraPreview)w;
                mCameraPrev.SetViewFinderContent(mVideoBrush);

                return MoSync.Constants.MA_CAMERA_RES_OK;
			};

            /**
             * Returns the number of available Camera on the device.
             */
			ioctls.maCameraNumber = delegate()
			{
                if (PhotoCamera.IsCameraTypeSupported(CameraType.FrontFacing) && PhotoCamera.IsCameraTypeSupported(CameraType.Primary))
                    return 2;
                else if (PhotoCamera.IsCameraTypeSupported(CameraType.FrontFacing) || PhotoCamera.IsCameraTypeSupported(CameraType.Primary))
                    return 1;
                return 0;
			};

            /**
             * Captures an image and stores it as a new data object in the
             * supplied placeholder.
             * @param _formatIndex int the required format.
             * @param _placeHolder int the placeholder used for storing the image.
             */
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

            /**
             * Sets the property represented by the string situated at the
             * _property address with the value situated at the _value address.
             * @param _property int the property name address
             * @param _value int the value address
             *
             * Note: the fallowing properties are not available on windows phone
             *      MA_CAMERA_FOCUS_MODE, MA_CAMERA_IMAGE_FORMAT, MA_CAMERA_ZOOM,
             *      MA_CAMERA_MAX_ZOOM.
             */
            ioctls.maCameraSetProperty = delegate(int _property, int _value)
			{
                // if the camera is not initialized, we cannot access any of its properties
                if (!isCameraInitialized)
                {
                    return MoSync.Constants.MA_CAMERA_RES_PROPERTY_NOTSUPPORTED;
                }

                String property = core.GetDataMemory().ReadStringAtAddress(_property);
                String value = core.GetDataMemory().ReadStringAtAddress(_value);

                if (property.Equals(MoSync.Constants.MA_CAMERA_FLASH_MODE))
                {
                    if (value.Equals(MoSync.Constants.MA_CAMERA_FLASH_ON))
                    {
                        mCamera.FlashMode = FlashMode.On;
                        mFlashMode = FlashMode.On;
                    }
                    else if (value.Equals(MoSync.Constants.MA_CAMERA_FLASH_OFF))
                    {
                        mCamera.FlashMode = FlashMode.Off;
                        mFlashMode = FlashMode.Off;
                    }
                    else if (value.Equals(MoSync.Constants.MA_CAMERA_FLASH_AUTO))
                    {
                        mCamera.FlashMode = FlashMode.Auto;
                        mFlashMode = FlashMode.Auto;
                    }
                    else return MoSync.Constants.MA_CAMERA_RES_INVALID_PROPERTY_VALUE;

                    return MoSync.Constants.MA_CAMERA_RES_OK;
                }
                else if (property.Equals(MoSync.Constants.MA_CAMERA_FOCUS_MODE))
                {
                    return MoSync.Constants.MA_CAMERA_RES_PROPERTY_NOTSUPPORTED;
                }
                else if (property.Equals(MoSync.Constants.MA_CAMERA_IMAGE_FORMAT))
                {
                    return MoSync.Constants.MA_CAMERA_RES_PROPERTY_NOTSUPPORTED;
                }
                else if (property.Equals(MoSync.Constants.MA_CAMERA_ZOOM))
                {
                    return MoSync.Constants.MA_CAMERA_RES_PROPERTY_NOTSUPPORTED;
                }
                else if (property.Equals(MoSync.Constants.MA_CAMERA_MAX_ZOOM))
                {
                    return MoSync.Constants.MA_CAMERA_RES_PROPERTY_NOTSUPPORTED;
                }
                else return MoSync.Constants.MA_CAMERA_RES_PROPERTY_NOTSUPPORTED;
			};

            /**
             * Selects a camera from the avalable ones;
             * in this eigther the back or the front camera is
             * chosen
             */

			ioctls.maCameraSelect = delegate(int _camera)
			{
                // if the camera is not initialized, we cannot access any of its properties
                if (!isCameraInitialized)
                {
                    return MoSync.Constants.MA_CAMERA_RES_FAILED;
                }

                if ( MoSync.Constants.MA_CAMERA_CONST_BACK_CAMERA == _camera)
                {
                    if (mCamera.CameraType != CameraType.Primary)
                    {
                        mCameraType = CameraType.Primary;
                        initCamera();
                    }
                }
                else if (MoSync.Constants.MA_CAMERA_CONST_FRONT_CAMERA == _camera)
                {
                    if (mCamera.CameraType != CameraType.FrontFacing)
                    {
                        mCameraType = CameraType.FrontFacing;
                        initCamera();
                    }
                }
                else return MoSync.Constants.MA_CAMERA_RES_FAILED;

                return MoSync.Constants.MA_CAMERA_RES_OK;
			};

            /**
             * Retrieves the specified property value in the given buffer.
             * @param _property int the address for the property string
             * @param _value int the address for the property value string (the buffer)
             * @param _bufSize int the buffer size
             */
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
                else if (property.Equals(MoSync.Constants.MA_CAMERA_ZOOM_SUPPORTED))
                {
                    core.GetDataMemory().WriteStringAtAddress(
                        _value,
                        "false",
                        _bufSize);
                }
                else if (property.Equals(MoSync.Constants.MA_CAMERA_FLASH_SUPPORTED))
                {
                    core.GetDataMemory().WriteStringAtAddress(
                        _value,
                        "true",
                        _bufSize);
                }
                else return MoSync.Constants.MA_CAMERA_RES_PROPERTY_NOTSUPPORTED;
				return 0;
			};

			ioctls.maCameraRecord = delegate(int _stopStartFlag)
			{
                return MoSync.Constants.MA_CAMERA_RES_FAILED;
			};
		}

        private void initCamera()
        {
            mCamera.Dispose();
            mCamera = null;
            mCamera = new PhotoCamera(mCameraType);

            isCameraInitialized = false;

            AutoResetEvent are = new AutoResetEvent(false);
            mCamera.Initialized += new EventHandler<CameraOperationCompletedEventArgs>(
                delegate(object o, CameraOperationCompletedEventArgs args)
                {
                    try
                    {
                        mCamera.FlashMode = mFlashMode;
                        isCameraInitialized = true;
                        are.Set();
                    }
                    catch { }
                });

            MoSync.Util.RunActionOnMainThreadSync(() =>
            {
                mVideoBrush.SetSource(mCamera);
            });
            // we need to wait until the camere is initialized before doing other operations
            // with it or getting/setting its properties
            are.WaitOne();
        }
	} // end class CameraModule
} // end namespace MoSync
