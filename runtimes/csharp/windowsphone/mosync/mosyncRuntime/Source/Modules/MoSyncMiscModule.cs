using System;
using System.IO;
using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.GamerServices;
using System.Windows;
using System.Collections.Generic;

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
    }

    public class MiscModule : ISyscallModule, IIoctlModule
    {
        public void Init(Syscalls syscalls, Core core, Runtime runtime)
        {
            SystemPropertyManager.mSystemPropertyProviders.Clear();

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
                return runtime.AddResource(new Resource(null, MoSync.Constants.RT_PLACEHOLDER));
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

            syscalls.maSoundPlay = delegate(int _sound_res, int _offset, int _size)
            {
                // not implemented, but I don't wanna throw exceptions.
                return -1;
            };

            syscalls.maLoadProgram = delegate(int _data, int _reload)
            {
#if REBUILD
                throw new Exception("maLoadProgram not available in rebuild mode");
#else
                Resource res = runtime.GetResource(MoSync.Constants.RT_BINARY, _data);
                Memory mem = (Memory)res.GetInternalObject();
                MoSync.Machine.SetLoadProgram(mem.GetStream(), _reload != 0);
                throw new Util.ExitException(0);
#endif
            };
        }

        public void Init(Ioctls ioctls, Core core, Runtime runtime)
        {
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
    }
}
