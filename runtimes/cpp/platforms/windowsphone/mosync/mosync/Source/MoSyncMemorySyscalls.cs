using System;
using System.IO;
using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.GamerServices;
using System.Windows;

namespace MoSync
{
    public class MemorySyscalls : ISyscallGroup
    {
        public void Init(Syscalls syscalls, Core core, Runtime runtime)
        {
            syscalls.memset = delegate(int dst, int val, int num)
            {
                core.GetDataMemory().FillRange(dst, (byte)val, num);
                return dst;
            };

            syscalls.memcpy = delegate(int dst, int src, int num)
            {
                core.GetDataMemory().WriteMemoryAtAddress(dst, core.GetDataMemory(), src, num);
                return dst;
            };
        }
	}
}

