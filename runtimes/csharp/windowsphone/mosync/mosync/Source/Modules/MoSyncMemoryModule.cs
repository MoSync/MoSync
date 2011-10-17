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
                String str = core.GetDataMemory().ReadStringAtAddress(src);
                core.GetDataMemory().WriteStringAtAddress(dst, str, str.Length+1);
                return dst;
            };

            syscalls.strcmp = delegate(int str1, int str2)
            {
                return String.Compare(
                    core.GetDataMemory().ReadStringAtAddress(str1),
                    core.GetDataMemory().ReadStringAtAddress(str2)
                    );
            };

            syscalls.maCreateData = delegate(int placeholder, int size)
            {
                Memory mem = null;
                try {
                    mem = new Memory(size);
                } 
                catch(OutOfMemoryException e)
                {
                    MoSync.Util.Log(e.ToString());
                    return MoSync.Constants.RES_OUT_OF_MEMORY;
                }

                runtime.SetResource(placeholder, new Resource(mem, MoSync.Constants.RT_BINARY));
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

            };

            syscalls.maOpenStore = delegate(int name, int flags)
            {
                // Returns STERR_NONEXISTENT if !(flags & MAS_CREATE_IF_NECESSARY) and the store does not exist. Returns another STERR code if the store could not be opened for another reason.
                return MoSync.Constants.STERR_NONEXISTENT;
            };

            syscalls.maWriteStore = delegate(int store, int data)
            {
                //Returns > 0 on success, STERR_FULL if the storage system is full, or another STERR code if the write failed for another reason.
                return 1;
            };

            syscalls.maReadStore = delegate(int store, int placeholder)
            {
                //return RES_OUT_OF_MEMORY if failed.
                return MoSync.Constants.RES_OK;
            };

            syscalls.maCloseStore = delegate(int store, int delete)
            {
            };


        }
	}
}

