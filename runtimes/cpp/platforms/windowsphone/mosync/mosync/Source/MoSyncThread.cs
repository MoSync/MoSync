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

class MoSyncThread
{
    private MoSync.CoreInterpreted mCore;
    private MoSync.Runtime mRuntime;
    private Thread mThread = null;

    public MoSyncThread()
    {
    }

    public void Init(Stream program, Stream resources, test_mosync.MainPage mainPage)
    {
        mCore = new MoSync.CoreInterpreted();
        mCore.LoadProgram(program);
        mRuntime = new MoSync.Runtime(mCore, mainPage);
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

    private void Run()
    {
        mCore.Run();
    }

    public void StartThread()
    {
        mThread = new Thread(new ThreadStart(Run));
        // Start the thread
        mThread.Start();
    }

    public void StopThread()
    {
        mCore.Stop();
        mThread.Join();
    }

    public static void CreateAndStart(String programFile, String resourceFile, test_mosync.MainPage mainPage)
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

        MoSyncThread mosyncThread = new MoSyncThread();
        mosyncThread.Init(programResInfo.Stream, resources, mainPage);
        mosyncThread.StartThread();
    }
}