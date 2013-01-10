using System;
using System.IO;
using System.Collections.Generic;
using Microsoft.Devices;
using System.Threading;
using System.Windows;
using System.Windows.Media.Imaging;
using System.Windows.Media;
using Microsoft.Phone.Controls;
using System.Windows.Controls;

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

            PhoneApplicationPage currentPage = (((PhoneApplicationFrame)Application.Current.RootVisual).Content as PhoneApplicationPage);

            // set the initial camera orientation in respect to the current page orientation
            SetInitialCameraOrientation(currentPage);
            // handle current page orientation and adjust the camera orientation accordingly
            HandleDeviceOrientation(currentPage);

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
                InitCamera();

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
                    InitCamera();
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

                        Stream data = null;
                        try
                        {
                            // as the camera always takes a snapshot in landscape left orientation,
                            // we need to rotate the resulting image 90 degrees for a current PortraitUp orientation
                            // and 180 degrees for a current LandscapeRight orientation
                            int rotateAngle = 0;
                            if (currentPage.Orientation == PageOrientation.PortraitUp)
                            {
                                rotateAngle = 90;

                            }
                            else if (currentPage.Orientation == PageOrientation.LandscapeRight)
                            {
                                rotateAngle = 180;
                            }
                            // if the current page is in a LandscapeLeft orientation, the orientation angle will be 0
                            data = RotateImage(args.ImageStream, rotateAngle);
                        }
                        catch
                        {
                            // the orientation angle was not a multiple of 90 - we keep the original image
                            data = args.ImageStream;
                        }
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
                        InitCamera();
                    }
                }
                else if (MoSync.Constants.MA_CAMERA_CONST_FRONT_CAMERA == _camera)
                {
                    if (mCamera.CameraType != CameraType.FrontFacing)
                    {
                        mCameraType = CameraType.FrontFacing;
                        InitCamera();

                        MoSync.Util.RunActionOnMainThreadSync(() =>
                            {
                                SetInitialCameraOrientation(currentPage);
                            }
                        );
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

        /**
         * Sets the output parameter 'dim' to the current camera available resolution or
         * returns false if the camera is not initialized or no resolutions are available.
         * @param index The index of the required format.
         * @param dim Output parameter that holds the current available camera resolution.
         */
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
                resolutions.MoveNext();
            }

            dim = resolutions.Current;
            return true;
        }

        /**
         * Sets the initial camera orientation by hand by rotating the video brush to match
         * the device orientation.
         * @param page The PhoneApplication page that is used as a reference for the camera orientation.
         */
        private void SetInitialCameraOrientation(PhoneApplicationPage page)
        {
            // we need to handle the initial page orientation
            double rotation = mCamera.Orientation;
            if (page.Orientation == PageOrientation.LandscapeLeft)
            {
                rotation -= 90;
                mVideoBrush.Stretch = Stretch.Uniform;
            }
            else if (page.Orientation == PageOrientation.LandscapeRight)
            {
                rotation += 90;
                mVideoBrush.Stretch = Stretch.Uniform;
            }
            else
            {
                if (mCamera.CameraType == CameraType.FrontFacing)
                    rotation += 180;
                mVideoBrush.Stretch = Stretch.Fill;
            }

            mVideoBrush.RelativeTransform = new CompositeTransform()
            {
                CenterX = 0.5,
                CenterY = 0.5,
                Rotation = rotation
            };
        }

        /**
         * Handles device orientation and sets the camera orientation accordingly.
         * @param page The PhoneApplication page that is used as a reference for the camera orientation.
         */
        private void HandleDeviceOrientation(PhoneApplicationPage page)
        {
            // on orientation changed, we need to rotate the video brush
            page.OrientationChanged += new System.EventHandler<OrientationChangedEventArgs>(
                delegate(object o, OrientationChangedEventArgs args)
                {
                    double rotation = mCamera.Orientation;
                    if (args.Orientation == PageOrientation.LandscapeLeft)
                    {
                        rotation -= 90;
                        mVideoBrush.Stretch = Stretch.Uniform;
                    }
                    else if (args.Orientation == PageOrientation.LandscapeRight)
                    {
                        rotation += 90;
                        mVideoBrush.Stretch = Stretch.Uniform;
                    }
                    else
                    {
                        if (mCamera.CameraType == CameraType.FrontFacing)
                            rotation += 180;
                        mVideoBrush.Stretch = Stretch.Fill;
                    }

                    mVideoBrush.RelativeTransform = new CompositeTransform()
                    {
                        CenterX = 0.5,
                        CenterY = 0.5,
                        Rotation = rotation
                    };
                });
        }

        /**
         * Reinitializes the camera: creates a new one, and adds it as a source for the
         * video brush.
         */
        private void InitCamera()
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

        /**
         * Because the camera captured image is always in landscape mode, we
         * need to rotate it. This is done by creating a writable bitmap from the image
         * stream and then translating it pixel by pixel into a new bitmap and then
         * into a new stream.
         * @param imageStream The image to be rotated as a data stream.
         * @param angle The angle of rotation (needs to be a multiple of 90).
         */
        private Stream RotateImage(Stream imageStream, int angle)
        {
            imageStream.Position = 0;
            // the angle must be a multiple of 90 degrees
            if (angle % 90 != 0 || angle < 0) throw new ArgumentException();
            if (angle % 360 == 0) return imageStream;

            BitmapImage bitmap = new BitmapImage();
            bitmap.SetSource(imageStream);
            WriteableBitmap wbSource = new WriteableBitmap(bitmap);

            // the target bitmap will contain the rotated image
            WriteableBitmap wbTarget = null;
            if (angle % 180 == 0)
            {
                // if the image is rotated 180 degrees, the width and height remain the same
                wbTarget = new WriteableBitmap(wbSource.PixelWidth, wbSource.PixelHeight);
            }
            else
            {
                wbTarget = new WriteableBitmap(wbSource.PixelHeight, wbSource.PixelWidth);
            }

            for (int x = 0; x < wbSource.PixelWidth; x++)
            {
                for (int y = 0; y < wbSource.PixelHeight; y++)
                {
                    switch (angle % 360)
                    {
                        case 90:
                            wbTarget.Pixels[(wbSource.PixelHeight - y - 1) + x * wbTarget.PixelWidth] = wbSource.Pixels[x + y * wbSource.PixelWidth];
                            break;
                        case 180:
                            wbTarget.Pixels[(wbSource.PixelWidth - x - 1) + (wbSource.PixelHeight - y - 1) * wbSource.PixelWidth] = wbSource.Pixels[x + y * wbSource.PixelWidth];
                            break;
                        case 270:
                            wbTarget.Pixels[y + (wbSource.PixelWidth - x - 1) * wbTarget.PixelWidth] = wbSource.Pixels[x + y * wbSource.PixelWidth];
                            break;
                    }
                }
            }

            // transform the rotated bitmap into a stream
            MemoryStream targetStream = new MemoryStream();
            wbTarget.SaveJpeg(targetStream, wbTarget.PixelWidth, wbTarget.PixelHeight, 0, 100);
            return targetStream;
        }

	} // end class CameraModule
} // end namespace MoSync
