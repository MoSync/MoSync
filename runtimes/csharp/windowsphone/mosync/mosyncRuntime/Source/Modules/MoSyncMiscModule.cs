using System;
using System.IO;
using Microsoft.Phone.Info;
using System.Windows;
using System.Diagnostics;
using System.Collections.Generic;
using System.Net.NetworkInformation;
using System.Globalization;
using Microsoft.Xna.Framework.GamerServices;
using Microsoft.Xna.Framework.Media;
using System.Windows.Media.Imaging;


namespace MoSync
{
    public class SystemPropertyManager
    {
        public delegate String SystemPropertyProvider(String key);
        private static Dictionary<String, SystemPropertyProvider> mSystemPropertyProviders =
            new Dictionary<string, SystemPropertyProvider>();


		public static void ClearSystemPropertyProviders()
		{
			mSystemPropertyProviders.Clear();
		}

        public static void RegisterSystemPropertyProvider(String key, SystemPropertyProvider provider)
        {
            mSystemPropertyProviders.Add(key, provider);
        }

        public static String GetSystemProperty(String key)
        {
            SystemPropertyProvider provider;
            if (mSystemPropertyProviders.TryGetValue(key, out provider) == false)
                return null;
            return provider(key);
        }
    } // end class SystemPropertyManager



    public class MiscModule : ISyscallModule, IIoctlModule
    {
		private Microsoft.Devices.VibrateController mVibrateController = null;
		private Runtime mRuntime = null;
        private Core mCore = null;

        public delegate void Delegate_SaveImageToCameraRoll(int imageHandle, String imageName, Resource imageResource);

        public void Init(Syscalls syscalls, Core core, Runtime runtime)
        {
            // maybe use some pretty reflection mechanism to find all syscall implementations here..
            syscalls.maCheckInterfaceVersion = delegate(int hash)
            {
                if (MoSync.Constants.MoSyncHash != (uint)hash)
                    MoSync.Util.CriticalError("Invalid hash!");
                return hash;
            };

            syscalls.maPanic = delegate(int code, int str)
            {
                String message = core.GetDataMemory().ReadStringAtAddress(str);
                MoSync.Util.CriticalError(message + "\ncode: " + code);
            };

            syscalls.maExit = delegate(int res)
            {
                MoSync.Util.Exit(res);
            };

            DateTime startDate = System.DateTime.Now;
            syscalls.maGetMilliSecondCount = delegate()
            {
                System.TimeSpan offset = (System.DateTime.Now - startDate);

                return offset.Milliseconds + (offset.Seconds + (offset.Minutes + (offset.Hours + offset.Days * 24) * 60) * 60) * 1000;
            };

            syscalls.maTime = delegate()
            {
                return (int)Util.ToUnixTimeUtc(System.DateTime.Now);
            };

            syscalls.maLocalTime = delegate()
            {
                return (int)Util.ToUnixTime(System.DateTime.Now);
            };

			syscalls.maCreatePlaceholder = delegate()
			{
				Resource res = new Resource(null, MoSync.Constants.RT_PLACEHOLDER, true);
				return runtime.AddResource(res);
			};

			syscalls.maDestroyPlaceholder = delegate(int res)
			{
				if (!runtime.GetResource(0, res).IsDynamicPlaceholder())
					MoSync.Util.CriticalError("maDestroyPlaceholder can only be used on handles created by maCreatePlaceholder.");
				runtime.RemoveResource(res);
			};

            syscalls.maFindLabel = delegate(int _name)
            {
                String name = core.GetDataMemory().ReadStringAtAddress(_name);
                int res;
                if (runtime.mLabels.TryGetValue(name, out res))
                    return res;
                else
                    return -1;
            };

            /*
             * PhoneApplicationService.Current.UserIdleDetectionMode
             * Disabling this will stop the screen from timing out and locking.
             * Discussion: this needs to be re-enabled for the backlight to work
             *             so an maStartBacklight should be needed for WP7;
             *             what about maToggleBacklight(bool)?
             *
             * We have maWakeLock instead on Windows Phone, Android, iOS.
             */
            syscalls.maResetBacklight = delegate()
            {
            };

			syscalls.maVibrate = delegate(int _ms)
			{
				if (mVibrateController == null)
					mVibrateController = Microsoft.Devices.VibrateController.Default;

				// more than 5 seconds aren't allowed..
				if (_ms > 5000)
					_ms = 5000;

				if (_ms < 0)
					return _ms;
				else if (_ms == 0)
					mVibrateController.Stop();
				else
					mVibrateController.Start(TimeSpan.FromMilliseconds(_ms));

				return 1;
			};

            syscalls.maLoadProgram = delegate(int _data, int _reload)
            {
#if REBUILD
				throw new Exception("maLoadProgram not available in rebuild mode");
#elif !LIB
				Resource res = runtime.GetResource(MoSync.Constants.RT_BINARY, _data);
				//Memory mem = (Memory)res.GetInternalObject();
				Stream mem = (Stream)res.GetInternalObject();
				MoSync.Machine.SetLoadProgram(mem, _reload != 0);
				throw new Util.ExitException(0);
#endif
            };
        }

		/*
		private void OnAlertMessageBoxClosed(IAsyncResult ar)
		{
			int? buttonIndex = Guide.EndShowMessageBox(ar);

			Memory eventData = new Memory(8);
			eventData.WriteInt32(MoSync.Struct.MAEvent.type, MoSync.Constants.EVENT_TYPE_ALERT);
			eventData.WriteInt32(MoSync.Struct.MAEvent.alertButtonIndex, (int)(buttonIndex + 1));

			mRuntime.PostEvent(new Event(eventData));
		}
		*/

        public void Init(Ioctls ioctls, Core core, Runtime runtime)
        {
			mRuntime = runtime;
            mCore = core;
            /**
             * Register system properties
             */
            SystemPropertyManager.SystemPropertyProvider myDelegateForDeviceInfo = new SystemPropertyManager.SystemPropertyProvider(getDeviceInfo);
            SystemPropertyManager.RegisterSystemPropertyProvider("mosync.imei",              myDelegateForDeviceInfo);
            SystemPropertyManager.RegisterSystemPropertyProvider("mosync.imsi",              myDelegateForDeviceInfo);
            SystemPropertyManager.RegisterSystemPropertyProvider("mosync.iso-639-1",         myDelegateForDeviceInfo);
            SystemPropertyManager.RegisterSystemPropertyProvider("mosync.iso-639-2",         myDelegateForDeviceInfo);
            SystemPropertyManager.RegisterSystemPropertyProvider("mosync.device",            myDelegateForDeviceInfo);
            SystemPropertyManager.RegisterSystemPropertyProvider("mosync.device.name",       myDelegateForDeviceInfo);
            SystemPropertyManager.RegisterSystemPropertyProvider("mosync.device.UUID",       myDelegateForDeviceInfo);
            SystemPropertyManager.RegisterSystemPropertyProvider("mosync.device.OS",         myDelegateForDeviceInfo);
            SystemPropertyManager.RegisterSystemPropertyProvider("mosync.device.OS.version", myDelegateForDeviceInfo);
            SystemPropertyManager.RegisterSystemPropertyProvider("mosync.network.type",      myDelegateForDeviceInfo);

            ioctls.maWriteLog = delegate(int src, int size)
            {
                byte[] bytes = new byte[size];
                core.GetDataMemory().ReadBytes(bytes, src, size);
                MoSync.Util.Log(bytes);
                return 0;
            };

			ioctls.maMessageBox = delegate(int _caption, int _message)
			{
				String message = core.GetDataMemory().ReadStringAtAddress(_message);
				String caption = core.GetDataMemory().ReadStringAtAddress(_caption);
				MoSync.Util.ShowMessage(message, false, caption);
				return 0;
			};

			ioctls.maTextBox = delegate(int _title, int _inText, int _outText, int _maxSize, int _constraints)
			{
				bool passwordMode = false;
				if ((_constraints & MoSync.Constants.MA_TB_FLAG_PASSWORD) != 0)
					passwordMode = true;

				if ((_constraints & MoSync.Constants.MA_TB_TYPE_MASK) != MoSync.Constants.MA_TB_TYPE_ANY)
					return MoSync.Constants.MA_TB_RES_TYPE_UNAVAILABLE;

				try
				{
					Guide.BeginShowKeyboardInput(Microsoft.Xna.Framework.PlayerIndex.One,
						core.GetDataMemory().ReadWStringAtAddress(_title), "",
						core.GetDataMemory().ReadWStringAtAddress(_inText),
						delegate(IAsyncResult result)
						{
							string text = Guide.EndShowKeyboardInput(result);

							Memory eventData = new Memory(12);
							eventData.WriteInt32(MoSync.Struct.MAEvent.type, MoSync.Constants.EVENT_TYPE_TEXTBOX);
							int res = MoSync.Constants.MA_TB_RES_OK;
							int len = 0;
							if (text == null)
							{
								res = MoSync.Constants.MA_TB_RES_CANCEL;
							}
							else
							{
								len = text.Length;
							}

							eventData.WriteInt32(MoSync.Struct.MAEvent.textboxResult, res);
							eventData.WriteInt32(MoSync.Struct.MAEvent.textboxLength, len);
							core.GetDataMemory().WriteWStringAtAddress(_outText, text, _maxSize);
							mRuntime.PostEvent(new Event(eventData));
						},
						null
						, passwordMode);
				}
				catch (Exception)
				{
					return -1;
				}

				return 0;
			};

            /**
             * @author: Ciprian Filipas
             * @brief: The maAlert ioctl implementation.
             * @note: On WP7 only 2 buttons are available, OK and CANCEL. Also if the buttons get null values from
             *        MoSync WP7 platform will automatically add the OK button. Regarding these facts the _b2 button will
             *        be ignored in the current implementation.
             */
            ioctls.maAlert = delegate(int _title, int _message, int _b1, int _b2, int _b3)
            {
                String title = "", message = "";

                if( 0 != _title )
                    title = core.GetDataMemory().ReadStringAtAddress(_title);
                if( 0 != _message )
                    message = core.GetDataMemory().ReadStringAtAddress(_message);

                if (0 != _b3)
                {
                    MoSync.Util.RunActionOnMainThreadSync(() =>
                        {
                            MessageBoxResult result = MessageBox.Show(message, title, MessageBoxButton.OKCancel);
                            if (result == MessageBoxResult.OK)
                            {
                                Memory eventData = new Memory(8);
                                const int MAWidgetEventData_eventType = 0;
                                const int MAWidgetEventData_eventArgumentValue = 4;

                                //write 1 down since the buttone clicked is the first one
                                eventData.WriteInt32(MAWidgetEventData_eventType, MoSync.Constants.EVENT_TYPE_ALERT);
                                eventData.WriteInt32(MAWidgetEventData_eventArgumentValue, 1);
                                //Posting a CustomEvent
                                mRuntime.PostEvent(new Event(eventData));
                            }
                            else if (result == MessageBoxResult.Cancel)
                            {
                                Memory eventData = new Memory(8);
                                const int MAWidgetEventData_eventType = 0;
                                const int MAWidgetEventData_eventArgumentValue = 4;

                                //write 1 down since the buttone clicked is the first one
                                eventData.WriteInt32(MAWidgetEventData_eventType, MoSync.Constants.EVENT_TYPE_ALERT);
                                eventData.WriteInt32(MAWidgetEventData_eventArgumentValue, 3);
                                //Posting a CustomEvent
                                mRuntime.PostEvent(new Event(eventData));
                            }
                        }
                    );
                }
                else
                {
                    MoSync.Util.RunActionOnMainThreadSync(() =>
                        {
                            MessageBox.Show(message, title, MessageBoxButton.OK);

                            // Since the only way to exit the messageBox is by pressing OK there is no
                            // need for a result object.

                            Memory eventData = new Memory(8);
                            const int MAWidgetEventData_eventType = 0;
                            const int MAWidgetEventData_eventArgumentValue = 4;

                            //write 1 down since the buttone clicked is the first one
                            eventData.WriteInt32(MAWidgetEventData_eventType, MoSync.Constants.EVENT_TYPE_ALERT);
                            eventData.WriteInt32(MAWidgetEventData_eventArgumentValue, 1);
                            //Posting a CustomEvent
                            mRuntime.PostEvent(new Event(eventData));
                        }
                    );
                }

                return 0;
            };

            ioctls.maGetSystemProperty = delegate(int _key, int _buf, int _size)
            {
                String key = core.GetDataMemory().ReadStringAtAddress(_key);
                String value = MoSync.SystemPropertyManager.GetSystemProperty(key);
                if (value == null)
                    return -2;
                if (value.Length + 1 <= _size)
                {
                    if(key.Equals("mosync.network.type"))
                    {
                        /**
                         * This code converts the result return by the GetSystemProperty
                         * for the "mosync.network.type" key to be supported by the current
                         * MoSync SDK 3.0
                         */
                        if (value.ToLower().Contains("wireless"))
                        {
                            value = "wifi";
                        }
                        else if(value.ToLower().Contains("ethernet"))
                        {
                            value = "ethernet";
                        }
                        else if(value.ToLower().Contains("mobilebroadbandgsm"))
                        {
                            value = "2g";
                        }
                        else if (value.ToLower().Contains("mobilebroadbandcdma"))
                        {
                            value = "3g";
                        }
                    }
                    core.GetDataMemory().WriteStringAtAddress(_buf, value, _size);
                }
                return value.Length + 1;
            };

			ioctls.maWakeLock = delegate(int flag)
			{
				if (MoSync.Constants.MA_WAKE_LOCK_ON == flag)
				{
					Microsoft.Phone.Shell.PhoneApplicationService.Current.
						UserIdleDetectionMode =
							Microsoft.Phone.Shell.IdleDetectionMode.Enabled;
				}
				else
				{
					Microsoft.Phone.Shell.PhoneApplicationService.Current.
						UserIdleDetectionMode =
							Microsoft.Phone.Shell.IdleDetectionMode.Disabled;
				}
				return 1;
			};

            // validates image input data and dispaches a delegate to save the image to camera roll
            ioctls.maSaveImageToDeviceGallery = delegate(int imageHandle, int imageNameAddr)
            {
                int returnCode = MoSync.Constants.MA_MEDIA_RES_IMAGE_EXPORT_FAILED;

                //Get the resource with the specified handle
                Resource res = mRuntime.GetResource(MoSync.Constants.RT_IMAGE, imageHandle);
                String imageName = mCore.GetDataMemory().ReadStringAtAddress(imageNameAddr);

                if ( (null != res) && !String.IsNullOrEmpty(imageName))
                {
                    object[] myArray = new object[3];
                    myArray[0] = imageHandle;
                    myArray[1] = imageName;
                    myArray[2] = res;

                    Deployment.Current.Dispatcher.BeginInvoke(
                        new Delegate_SaveImageToCameraRoll(SaveImageToCameraRoll),myArray);

                    returnCode = MoSync.Constants.MA_MEDIA_RES_OK;
                }

                return returnCode;
            };
        }

        private void SaveImageToCameraRoll(int imageHandle, String imageName, Resource imageResource)
        {
            MediaLibrary library = new MediaLibrary();
            MemoryStream targetStream = new MemoryStream();

            int mediaType = MoSync.Constants.MA_MEDIA_TYPE_IMAGE;
            int mediaHandle = imageHandle;
            int eventReturnCode = MoSync.Constants.MA_MEDIA_RES_OK;

            try
            {
                WriteableBitmap data = (WriteableBitmap)imageResource.GetInternalObject();
                data.SaveJpeg(targetStream, data.PixelWidth, data.PixelHeight, 0, 100);
                data = null;

                library.SavePictureToCameraRoll(imageName, targetStream.GetBuffer()).Dispose();
            }
            catch (Exception e)
            {
                System.Diagnostics.Debug.WriteLine(e.ToString());
                eventReturnCode = MoSync.Constants.MA_MEDIA_RES_IMAGE_EXPORT_FAILED;
            }
            finally
            {
                library.Dispose();
                targetStream.Dispose();
                PostMediaEvent(mediaType, mediaHandle, eventReturnCode);
            }
        }

        private void PostMediaEvent(int mediaType, int mediaHandle, int returnCode)
        {
            const int MAEventData_eventType = 0;
            const int MAEventData_mediaType = 4;
            const int MAEventData_mediaHandle = 8;
            const int MAEventData_operationResultCode = 12;

            Memory eventData = new Memory(16);

            eventData.WriteInt32(MAEventData_eventType, MoSync.Constants.EVENT_TYPE_MEDIA_EXPORT_FINISHED);
            eventData.WriteInt32(MAEventData_mediaType, mediaType);
            eventData.WriteInt32(MAEventData_mediaHandle, mediaHandle);
            eventData.WriteInt32(MAEventData_operationResultCode, returnCode);
            mRuntime.PostEvent(new Event(eventData));
        }

        /**
         * Retrieves the values of MoSync System Properties
         */
        public static string getDeviceInfo(string key)
        {
            // imei
            if (key.Equals("mosync.imei"))
            {
                string sImei = string.Empty;
                object objImei;

                if (Microsoft.Phone.Info.UserExtendedProperties.TryGetValue("ANID", out objImei))
                {
                    sImei = (string)objImei;
                    return sImei;
                }
            }

            // imsi - not available in WP7.1
            if (key.Equals("mosync.imsi"))
            {
                //TODO
            }

            // lang
            if (key.Equals("mosync.iso-639-1"))
            {
                return System.Globalization.CultureInfo.CurrentCulture.ToString();
            }

            // lantg
            if (key.Equals("mosync.iso-639-2"))
            {
                return System.Globalization.CultureInfo.CurrentCulture.Name;
            }

            // device info
            if (key.Equals("mosync.device"))
            {
                string device = DeviceStatus.DeviceManufacturer + ";" +
                                DeviceStatus.DeviceFirmwareVersion;
                                // + model name
                return device;
            }

            // device name
            if (key.Equals("mosync.device.name"))
            {
                return DeviceStatus.DeviceName;
            }

            //Note: to get a result requires ID_CAP_IDENTITY_DEVICE
            // to be added to the capabilities of the WMAppManifest
            // this will then warn users in marketplace
            if (key.Equals("mosync.device.UUID"))
            {
                byte[] sUUID = null;
                object uniqueId;
                if (DeviceExtendedProperties.TryGetValue("DeviceUniqueId", out uniqueId))
                {
                    sUUID = (byte[])uniqueId;
                    return BitConverter.ToString(sUUID);
                }
            }

            // device OS
            if (key.Equals("mosync.device.OS"))
            {
                return Environment.OSVersion.ToString();
            }

            // device OS version
            if (key.Equals("mosync.device.OS.version"))
            {
                return Environment.OSVersion.Version.ToString();
            }

            // network connection type
            if (key.Equals("mosync.network.type"))
            {
                return Microsoft.Phone.Net.NetworkInformation.NetworkInterface.NetworkInterfaceType.ToString();
            }

            // in case of no information
            return "not available";
        }

    } // end class MiscModule
} // end namespace MoSync
