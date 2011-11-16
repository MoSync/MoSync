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
        private readonly bool mRebuild;

        private Machine(bool rebuild)
        {
            mRebuild = rebuild;
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
            if (System.Diagnostics.Debugger.IsAttached)
                CoreRun();
            else
            {
                try
                {
                    CoreRun();
                }
                catch (Exception e)
                {
                    System.Diagnostics.Debug.WriteLine(e.StackTrace);
                    MoSync.Util.CriticalError(e.ToString());
                };
            }
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

        private void LoadProgram(Stream s)
        {
            Core core = new MoSync.CoreInterpreted(s);
            mCore = core;
            mRuntime = new MoSync.Runtime(mCore);
            mCore.SetRuntime(mRuntime);

            mCore.Init();   // moves s.Position
            if (s.Position == s.Length)
                s = null;
            if (s != null)
            {
                if (!mRuntime.LoadResources(s))
                {
                    MoSync.Util.CriticalError("Failed to load resources!");
                }
            }

            mRuntime.Init();
        }

        private void LoadProgram(String programFile, String resourceFile)
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

            Core core = new MoSync.CoreInterpreted(programResInfo.Stream);
            Init(core, resources);
            programResInfo.Stream.Close();
            if (resources != null)
                resources.Close();
        }

        public static Machine CreateInterpretedMachine(String programFile, String resourceFile)
        {
            MoSync.Machine mosyncMachine = new MoSync.Machine(false);
            mosyncMachine.LoadProgram(programFile, resourceFile);
            return mosyncMachine;
        }

        public static Machine CreateNativeMachine(Core core, String resourceFile)
        {
            StreamResourceInfo resourcesResInfo = Application.GetResourceStream(new Uri(resourceFile, UriKind.Relative));
            Stream resources = null;
            if (resourcesResInfo != null && resourcesResInfo.Stream != null)
                resources = resourcesResInfo.Stream;

            MoSync.Machine mosyncMachine = new MoSync.Machine(true);
            mosyncMachine.Init(core, resources);
            if (resources != null)
                resources.Close();
            return mosyncMachine;
        }

        private void CoreRun()
        {
            if(mRebuild) {
                mCore.Run();
                mRuntime.RunCleaners();
                return;
            }
            while (true)
            {
                try
                {
                    mCore.Run();
                }
                catch (MoSync.Util.ExitException e)
                {
                    mRuntime.RunCleaners();
                    if (mLoadProgramStream != null)
                    {
                        Stream s = mLoadProgramStream;
                        mLoadProgramStream = null;
                        Util.RunActionOnMainThreadSync(
                            delegate() { LoadProgram(s); });
                        s.Close();
                        continue;
                    }
                    else if (mLoadProgramFlag)
                    {   // reload original program
                        mLoadProgramFlag = false;
                        Util.RunActionOnMainThreadSync(
                            delegate() { LoadProgram("program", "resources"); });
                        continue;
                    }
                    else
                    {   // no reload
                        throw e;
                    }
                }
            }
        }


        private static Stream mLoadProgramStream = null;
        private static bool mLoadProgramFlag = false;

        public static void SetLoadProgram(Stream comboStream, bool reloadFlag)
        {
            if (mLoadProgramStream != null)
                throw new Exception("SetLoadProgram");
            mLoadProgramStream = comboStream;
            mLoadProgramFlag |= reloadFlag;
        }
    }
}
