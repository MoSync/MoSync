using System;

// The Core is the base class for the program execution.
// It has an interface for getting access to the data memory
// and the stack pointer / stack values.
// This is the interface the runtime get access to.

namespace MoSync
{
    public class Core
    {
        public Core()
        {
        }

        public void SetRuntime(Runtime runtime)
        {
            mRuntime = runtime;
        }

        public void Start()
        {
            mRunning = true;
        }

        public void Stop()
        {
            mRunning = false;
        }

        public virtual int GetStackPointer()
        {
            return 0;
        }

        public int GetStackValue(int offset)
        {
            return mDataMemory.ReadInt32(GetStackPointer() + offset);
        }

        public int ExtractArgs(int address, int offset)
        {
            return mDataMemory.ReadInt32(address + offset);
        }

        // will reset the program.
        public virtual void Init()
        {
        }

        public virtual void Run()
        {
        }

#if !LIB
        public Memory GetDataMemory()
        {
            return mDataMemory;
        }
#else
        public SystemMemory GetDataMemory()
        {
            return mDataMemory;
        }
#endif

        public int GetCustomEventDataPointer()
        {
            return mCustomEventPointer;
        }

        protected Runtime mRuntime = null;
#if !LIB
        protected Memory mDataMemory;
#else
        protected SystemMemory mDataMemory;
#endif
        protected uint mDataSegmentSize;
        protected uint mDataSegmentMask;
        protected int mCustomEventPointer;
        protected bool mRunning = false;
    }
}
