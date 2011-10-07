using System;
using System.IO;
using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.GamerServices;
using System.Windows;

namespace MoSync
{
    public class MiscSyscalls : ISyscallGroup
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

            syscalls.maWait = delegate(int time)
            {

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
	}
}

