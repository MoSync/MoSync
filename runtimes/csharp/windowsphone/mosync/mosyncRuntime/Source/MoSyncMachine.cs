using System;
using System.Collections.Generic;
using System.Linq;
using System.Net;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Navigation;
using System.Windows.Shapes;
using Microsoft.Phone.Controls;
using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Content;
using Microsoft.Xna.Framework.Graphics;
using System.IO;
using System.Threading;
using System.Windows.Resources;

namespace MoSync
{

    public class Machine
    {
        private MoSync.Core mCore;
        private MoSync.Runtime mRuntime;
        private Thread mThread = null;

        public Machine()
        {
            // This tells the util subsystem which thread is the main thread.
            // Never make an instance of Program from another thread.
            MoSync.Util.InitStartupThread();
        }

        public void Init(MoSync.Core core, Stream resources)
        {
            mCore = core;
            mRuntime = new MoSync.Runtime(mCore);
            mCore.SetRuntime(mRuntime);

            if (resources != null)
            {
                if (!mRuntime.LoadResources(resources))
                {
                    MoSync.Util.CriticalError("Failed to load resources!");
                }
            }

            mCore.Init();
            mRuntime.Init();
        }

        private void ThreadEntry()
        {
            try
            {
                mCore.Run();
            }
            catch (Exception e)
            {
                MoSync.Util.CriticalError(e.ToString());
            };
        }

        public void Run()
        {
            mThread = new Thread(new ThreadStart(ThreadEntry));
            mThread.Start();
        }

        public void Stop()
        {
            mCore.Stop();
            mThread.Join();
        }

        public static Machine CreateInterpretedMachine(String programFile, String resourceFile)
        {
            StreamResourceInfo programResInfo = Application.GetResourceStream(new Uri(programFile, UriKind.Relative));
            StreamResourceInfo resourcesResInfo = Application.GetResourceStream(new Uri(resourceFile, UriKind.Relative));

            if (programResInfo == null || programResInfo.Stream == null)
            {
                MoSync.Util.CriticalError("No program file available!");
            }

            Stream resources = null;
            if (resourcesResInfo != null && resourcesResInfo.Stream != null)
                resources = resourcesResInfo.Stream;

            MoSync.Machine mosyncMachine = new MoSync.Machine();
            Core core = new MoSync.CoreInterpreted(programResInfo.Stream);
            mosyncMachine.Init(core, resources);
            return mosyncMachine;
        }

        public static Machine CreateNativeMachine(Core core, String resourceFile)
        {
            StreamResourceInfo resourcesResInfo = Application.GetResourceStream(new Uri(resourceFile, UriKind.Relative));
            Stream resources = null;
            if (resourcesResInfo != null && resourcesResInfo.Stream != null)
                resources = resourcesResInfo.Stream;

            MoSync.Machine mosyncMachine = new MoSync.Machine();
            mosyncMachine.Init(core, resources);
            return mosyncMachine;
        }
    }
}