using System;
using System.IO;
using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.GamerServices;
using System.Windows;

namespace MoSync
{
    public class MemoryModule : ISyscallModule
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

            syscalls.strcpy = delegate(int dst, int src)
            {
                byte[] mem = core.GetDataMemory().GetData();
                int origDst = dst;
                src--;
                do
                {
                    src++;
                    mem[dst] = mem[src];
                    dst++;
                } while (mem[src] != 0);
                return origDst;
            };

            syscalls.strcmp = delegate(int str1, int str2)
            {
                byte[] mem = core.GetDataMemory().GetData();
                while (mem[str1] != 0 && mem[str1] == mem[str2])
                {
                    str1++;
                    str2++;
                }
                return (mem[str1] - mem[str2]);
            };

            syscalls.maCreateData = delegate(int placeholder, int size)
            {
                Memory mem = null;
                try
                {
                    mem = new Memory(size);
                }
                catch (OutOfMemoryException e)
                {
                    MoSync.Util.Log(e);
                    return MoSync.Constants.RES_OUT_OF_MEMORY;
                }

				Resource res = runtime.GetResource(MoSync.Constants.RT_PLACEHOLDER, placeholder);
				res.SetInternalObject(mem);
				res.SetResourceType(MoSync.Constants.RT_BINARY);
                return MoSync.Constants.RES_OK;
            };

            syscalls.maWriteData = delegate(int data, int src, int offset, int size)
            {
                Resource res = runtime.GetResource(MoSync.Constants.RT_BINARY, data);
                Memory mem = (Memory)res.GetInternalObject();
                mem.WriteMemoryAtAddress(offset, core.GetDataMemory(), src, size);
            };

            syscalls.maReadData = delegate(int data, int dst, int offset, int size)
            {
                Resource res = runtime.GetResource(MoSync.Constants.RT_BINARY, data);
                Memory mem = (Memory)res.GetInternalObject();
                core.GetDataMemory().WriteMemoryAtAddress(dst, mem, offset, size);
            };

            syscalls.maGetDataSize = delegate(int data)
            {
                Resource res = runtime.GetResource(MoSync.Constants.RT_BINARY, data);
                Memory mem = (Memory)res.GetInternalObject();
                return mem.GetSizeInBytes();
            };

            syscalls.maCopyData = delegate(int _params)
            {
                throw new Exception("maCopyData not implemented");
            };
        }
    }
}
