using System;
using System.IO;
using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.GamerServices;
using System.Windows;

namespace MoSync
{
    public class MathModule : ISyscallModule, IIoctlModule
    {
        public void Init(Syscalls mSyscalls, Core mCore, Runtime mRuntime)
        {
            mSyscalls.__adddf3 = delegate(double a, double b) { return a + b; };
            mSyscalls.__subdf3 = delegate(double a, double b) { return a - b; };
            mSyscalls.__muldf3 = delegate(double a, double b) { return a * b; };
            mSyscalls.__divdf3 = delegate(double a, double b) { return a / b; };
            mSyscalls.__negdf2 = delegate(double a) { return -a; };
            mSyscalls.__fixdfsi = delegate(double a) { return (int)a; };
            mSyscalls.__fixunsdfsi = delegate(double a) { return (int)((uint)a); };
            mSyscalls.__floatsidf = delegate(int a) { return (double)a; };
            mSyscalls.__extendsfdf2 = delegate(float a) { return (double)a; };
            mSyscalls.dcmp = delegate(double a, double b) { return (a < b) ? -1 : ((a > b) ? 1 : 0); };

            mSyscalls.__addsf3 = delegate(float a, float b) { return a + b; };
            mSyscalls.__subsf3 = delegate(float a, float b) { return a - b; };
            mSyscalls.__mulsf3 = delegate(float a, float b) { return a * b; };
            mSyscalls.__divsf3 = delegate(float a, float b) { return a / b; };
            mSyscalls.__negsf2 = delegate(float a) { return -a; };
            mSyscalls.__fixsfsi = delegate(float a) { return (int)a; };
            mSyscalls.__fixunssfsi = delegate(float a) { return (int)((uint)a); };
            mSyscalls.__floatsisf = delegate(int a) { return (float)a; };
            mSyscalls.__truncdfsf2 = delegate(double a) { return (float)a; };
            mSyscalls.fcmp = delegate(float a, float b) { return (a < b) ? -1 : ((a > b) ? 1 : 0); };

            mSyscalls.sin = delegate(double v) { return System.Math.Sin(v); };
            mSyscalls.cos = delegate(double v) { return System.Math.Cos(v); };
            mSyscalls.tan = delegate(double v) { return System.Math.Tan(v); };
            mSyscalls.sqrt = delegate(double v) { return System.Math.Sqrt(v); };
        }

        public void Init(Ioctls ioctls, Core core, Runtime runtime)
        {
            ioctls.sinh = delegate(double d)
            {
                return BitConverter.DoubleToInt64Bits(Math.Sinh(d));
            };

            ioctls.cosh = delegate(double d)
            {
                return BitConverter.DoubleToInt64Bits(Math.Cosh(d));
            };

            ioctls.atanh = delegate(double d)
            {
                double value = (Math.Log(1.0 + d) - Math.Log(1.0 - d))/2.0;
                return BitConverter.DoubleToInt64Bits(value); 
            };

        }
	}
}

