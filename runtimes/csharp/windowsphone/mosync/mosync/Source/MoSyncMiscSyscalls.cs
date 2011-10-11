using System;
using System.IO;
using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.GamerServices;
using System.Windows;

namespace MoSync
{
    public class MiscSyscalls : ISyscallGroup, IIoctlGroup
    {
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

            DateTime startDate = System.DateTime.Now;
            syscalls.maGetMilliSecondCount = delegate() {
                System.TimeSpan offset = (System.DateTime.Now - startDate);

                return offset.Milliseconds+(offset.Seconds+(offset.Minutes+(offset.Hours+offset.Days*24)*60)*60)*1000;
            };

            syscalls.maCreatePlaceholder = delegate()
            {
                return runtime.AddResource(new Resource(null, MoSync.Constants.RT_PLACEHOLDER));
            };

            syscalls.maResetBacklight = delegate()
            {
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
        }
	}
}

