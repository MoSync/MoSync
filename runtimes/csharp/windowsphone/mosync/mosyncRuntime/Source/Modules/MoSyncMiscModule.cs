using System;
using System.IO;
using Microsoft.Phone.Info;
using System.Windows;
using System.Collections.Generic;
using System.Net.NetworkInformation;
using System.Globalization;

namespace MoSync
{
    public class SystemPropertyManager
    {
        public delegate String SystemPropertyProvider(String key);
        public static Dictionary<String, SystemPropertyProvider> mSystemPropertyProviders =
            new Dictionary<string, SystemPropertyProvider>();

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

            syscalls.maResetBacklight = delegate()
            {
            };

			syscalls.maVibrate = delegate(int _ms)
			{
				if (mVibrateController == null)
					mVibrateController = Microsoft.Devices.VibrateController.Default;

				if (_ms < 0)
					return _ms;
				else if (_ms == 0)
					mVibrateController.Stop();
				else
					mVibrateController.Start(TimeSpan.FromMilliseconds(_ms));

				return 0;
			};

            syscalls.maLoadProgram = delegate(int _data, int _reload)
            {
#if REBUILD
                throw new Exception("maLoadProgram not available in rebuild mode");
#else
                Resource res = runtime.GetResource(MoSync.Constants.RT_BINARY, _data);
                //Memory mem = (Memory)res.GetInternalObject();
				Stream mem = (Stream)res.GetInternalObject();
                MoSync.Machine.SetLoadProgram(mem, _reload != 0);
                throw new Util.ExitException(0);
#endif
            };
        }

        public void Init(Ioctls ioctls, Core core, Runtime runtime)
        {
            // add system property providers
            SystemPropertyManager.mSystemPropertyProviders.Clear();

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

            ioctls.maGetSystemProperty = delegate(int _key, int _buf, int _size)
            {
                String key = core.GetDataMemory().ReadStringAtAddress(_key);
                String value = MoSync.SystemPropertyManager.GetSystemProperty(key);
                if (value == null)
                    return -2;
                if (value.Length + 1 <= _size)
                    core.GetDataMemory().WriteStringAtAddress(_buf, value, _size);
                return value.Length + 1;
            };
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

            // imsi
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

            // in case of no information return empty
            return "not available";
        }

    } // end class MiscModule
} // end namespace MoSync
