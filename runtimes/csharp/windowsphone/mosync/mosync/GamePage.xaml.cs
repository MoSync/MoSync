#define USE_THREAD_LOCKED_VERSION

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

		bool mShouldDrawFrame = false;
		MoSync.Machine mMachine;
		MoSync.Runtime mRuntime;
		MoSync.OpenGLESModule mOpenGLESModule;
		bool mOnDrawInitialized = false;

        public GamePage()
        {
            InitializeComponent();

            // Get the content manager from the application
            contentManager = (Application.Current as App).Content;

            // Create a timer for this page
            timer = new GameTimer();
			timer.UpdateInterval = TimeSpan.FromTicks(166667);
			timer.Update += OnUpdate;
#if USE_THREAD_LOCKED_VERSION
			timer.FrameAction += PreDraw;
#else
#endif
        }

		ManualResetEvent DrawCompleteEvent = new ManualResetEvent(true);
		AutoResetEvent DrawRequiredEvent = new AutoResetEvent(false);

		GraphicsDevice MoSyncGraphicsDevice;

		//RenderTarget2D mBackBuffer;
		//SpriteBatch SpriteBatch;

        protected override void OnNavigatedTo(NavigationEventArgs e)
        {
            // Set the sharing mode of the graphics device to turn on XNA rendering
            SharedGraphicsDeviceManager.Current.GraphicsDevice.SetSharingMode(true);

			mMachine = ((App)Application.Current).GetMachine();
			mRuntime = mMachine.GetRuntime();
			mOpenGLESModule = mRuntime.GetModule<MoSync.OpenGLESModule>();
			//mOpenGLESModule.InitWithGraphicsDevice(SharedGraphicsDeviceManager.Current.GraphicsDevice);
			MoSyncGraphicsDevice = SharedGraphicsDeviceManager.Current.GraphicsDevice;
			MoSyncGraphicsDevice.PresentationParameters.IsFullScreen = true;
			SharedGraphicsDeviceManager.Current.SynchronizeWithVerticalRetrace = false;
			mOpenGLESModule.InitWithGraphicsDevice(MoSyncGraphicsDevice);

            // TODO: use this.content to load your game content here
//			mBackBuffer = new RenderTarget2D(MoSyncGraphicsDevice, SharedGraphicsDeviceManager.DefaultBackBufferWidth, SharedGraphicsDeviceManager.DefaultBackBufferHeight);
//			SpriteBatch = new SpriteBatch(SharedGraphicsDeviceManager.Current.GraphicsDevice);
//			MoSyncGraphicsDevice.SetRenderTarget(mBackBuffer);

#if USE_THREAD_LOCKED_VERSION
			mOpenGLESModule.SetOnUpdateScreenAction(() =>
			{
				mShouldDrawFrame = true;
				if (!mOnDrawInitialized)
					timer.Draw += OnDraw;
				mOnDrawInitialized = true;
				DrawRequiredEvent.Set();
				DrawCompleteEvent.WaitOne();
				mShouldDrawFrame = false;
			});

			MoSync.Util.SetPreRunOnMainThreadAction(() =>
			{
				mShouldDrawFrame = false;
				timer.FrameAction -= PreDraw;
				DrawRequiredEvent.Set();
			});

			MoSync.Util.SetPostRunOnMainThreadAction(() =>
			{
				mShouldDrawFrame = false;
				timer.FrameAction += PreDraw;
				DrawRequiredEvent.Set();
			});
#else
			mOpenGLESModule.SetOnUpdateScreenAction(() =>
			{
				timer.Draw += OnDraw;
			});
#endif

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


#if USE_THREAD_LOCKED_VERSION
		private void PreDraw(object sender, EventArgs e)
		{
			DrawCompleteEvent.Set();
			DrawCompleteEvent.Reset();

			if (!DrawRequiredEvent.WaitOne(100))
			{
				mShouldDrawFrame = false;
			}

			if(!mShouldDrawFrame)
				GameTimer.SuppressFrame();
		}

        /// <summary>
        /// Allows the page to draw itself.
        /// </summary>
        private void OnDraw(object sender, GameTimerEventArgs e)
        {
			// content will be presented right after we return from this function
        }
#else
        private void OnDraw(object sender, GameTimerEventArgs e)
        {
			timer.Draw -= OnDraw;
        }
#endif
	}
}