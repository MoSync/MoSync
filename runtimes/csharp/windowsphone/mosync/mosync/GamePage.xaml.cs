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

namespace test_mosync
{
    public partial class GamePage : PhoneApplicationPage
    {
        ContentManager contentManager;
        GameTimer timer;

		MoSync.Machine mMachine;
		MoSync.Runtime mRuntime;
		MoSync.OpenGLESModule mOpenGLESModule;

        public GamePage()
        {
            InitializeComponent();

            // Get the content manager from the application
            contentManager = (Application.Current as App).Content;

            // Create a timer for this page
            timer = new GameTimer();
			//timer.UpdateInterval = new TimeSpan(333333);
			timer.UpdateInterval = TimeSpan.FromTicks(166667);
			timer.FrameAction += SuppressFrame;
			timer.Update += OnUpdate;
			timer.Draw += OnDraw;
        }

		AutoResetEvent DrawRequiredEvent = new AutoResetEvent(false);
		AutoResetEvent DrawCompleteEvent = new AutoResetEvent(false);
		GraphicsDevice MoSyncGraphicsDevice;
		SpriteBatch SpriteBatch;

        protected override void OnNavigatedTo(NavigationEventArgs e)
        {
            // Set the sharing mode of the graphics device to turn on XNA rendering
            SharedGraphicsDeviceManager.Current.GraphicsDevice.SetSharingMode(true);

			mMachine = ((App)Application.Current).GetMachine();
			mRuntime = mMachine.GetRuntime();
			mOpenGLESModule = mRuntime.GetModule<MoSync.OpenGLESModule>();
			//mOpenGLESModule.InitWithGraphicsDevice(SharedGraphicsDeviceManager.Current.GraphicsDevice);
			MoSyncGraphicsDevice = SharedGraphicsDeviceManager.Current.GraphicsDevice;
			MoSyncGraphicsDevice.PresentationParameters.IsFullScreen = false;
	
			mOpenGLESModule.InitWithGraphicsDevice(MoSyncGraphicsDevice);

            // TODO: use this.content to load your game content here
			SpriteBatch = new SpriteBatch(SharedGraphicsDeviceManager.Current.GraphicsDevice);

			mOpenGLESModule.SetOnUpdateScreenAction(() =>
			{
				// it's time to draw.
				// stop supressing frames.
				timer.FrameAction -= SuppressFrame;

				// wait for draw to complete.
				DrawCompleteEvent.WaitOne();
			});


			base.OnNavigatedTo(e);

			// Start the timer
			timer.Start();
        }

        protected override void OnNavigatedFrom(NavigationEventArgs e)
        {
            // Stop the timer
            timer.Stop();

            // Set the sharing mode of the graphics device to turn off XNA rendering
            SharedGraphicsDeviceManager.Current.GraphicsDevice.SetSharingMode(false);
            base.OnNavigatedFrom(e);
        }

		private void OnUpdate(object sender, GameTimerEventArgs e)
		{
		}

		private void SuppressFrame(object sender, EventArgs e)
		{
			GameTimer.SuppressFrame();
		}

#if true
		private void CompleteFrame(object sender, EventArgs e)
		{
			// Remove this frame action
			timer.FrameAction -= CompleteFrame;

			// Supress this frame and set the following frames to be supressed (until further notice)
			GameTimer.SuppressFrame();
			timer.FrameAction += SuppressFrame;

			// Notify the MoSync thread that the draw is complete.
			DrawCompleteEvent.Set();
		}
#endif

        /// <summary>
        /// Allows the page to draw itself.
        /// </summary>
        private void OnDraw(object sender, GameTimerEventArgs e)
        {
#if true	
			// Set a frame action that notifies the mosync thread that the draw is complete.
			timer.FrameAction += CompleteFrame;
#endif
        }
    }
}