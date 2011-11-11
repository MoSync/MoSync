using System;
using System.IO;
using System.Collections.Generic;
using System.Windows.Resources;
using System.Windows;

// This is the core that interprets mosync
// byte code (produced by pipe-tool).
// It has dependencies on some idl compiler
// generated code, such as the SyscallInvoker

namespace MoSync
{
    public class CoreNative : Core
    {
        protected CoreNativeSyscallInvoker mSyscallInvoker;
        protected int sp;
        protected int __dbl_high;

        public CoreNative()
        {
        }

        public void InitData(String dataName, int fileSize, int dataSegmentSize)
        {
            mDataMemory = new Memory(dataSegmentSize);

            StreamResourceInfo dataSectionInfo = Application.GetResourceStream(new Uri("RebuildData\\" + dataName, UriKind.Relative));

            if (dataSectionInfo == null || dataSectionInfo.Stream == null)
            {
                MoSync.Util.CriticalError("No data_section.bin file available!");
            }

            Stream dataSection = dataSectionInfo.Stream;
            mDataMemory.WriteFromStream(0, dataSection, fileSize);
            dataSection.Close();

            int customEventDataSize = 60;
            sp -= customEventDataSize;
            mCustomEventPointer = dataSegmentSize - customEventDataSize;
        }


        public override void Init()
        {
            base.Init();
            Start();

            if (mRuntime == null)
                MoSync.Util.CriticalError("No runtime!");

            mSyscallInvoker = new CoreNativeSyscallInvoker(this, mRuntime.GetSyscalls());
        }

        public int SetReturnValue(int value)
        {
            return value;
        }

        public int SetReturnValue(double value)
        {
            ulong ret = (ulong)BitConverter.DoubleToInt64Bits(value);
            __dbl_high = (int)(ret >> 32);
            return (int)(ret & 0xffffffffL);
        }

        public int SetReturnValue(float value)
        {
            return BitConverter.ToInt32(BitConverter.GetBytes(value), 0);
        }

        public int SetReturnValue(long value)
        {
            __dbl_high = (int)(((ulong)value) >> 32);
            return (int)(value & 0xffffffff);
        }

        public override int GetStackPointer()
        {
            return sp;
        }

        public void MoSyncDiv0()
        {
            MoSync.Util.CriticalError("Division by zero!");
        }

        public virtual void Main()
        {
        }

        public override void Run()
        {
            Main();
        }
    }
}
