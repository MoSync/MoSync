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

        public int GetStackPointer()
        {
            return 0;
        }

        public int GetStackValue(int offset)
        {
            return mDataMemory.ReadInt32(GetStackPointer() - offset);
        }

        // will reset the program.
        public void Init()
        {
        }

        public void Run()
        {

        }

        public Memory GetDataMemory()
        {
            return mDataMemory;
        }

        public int GetCustomEventDataPointer()
        {
            return mCustomEventPointer;
        }

        protected Runtime mRuntime = null;
        protected Memory mDataMemory;
        protected uint mDataSegmentSize;
        protected uint mDataSegmentMask;
        protected int mCustomEventPointer;
        protected bool mRunning = false;
    }
}
