//-----------------------------------------------------------------------
// <copyright file="App.xaml.cs" company="Larry Olson">
// (c) Copyright Larry Olson.
// This source is subject to the Microsoft Public License (Ms-PL)
// See http://code.msdn.microsoft.com/ManagedMediaHelpers/Project/License.aspx
// All other rights reserved.
// </copyright>
//-----------------------------------------------------------------------

namespace Mp3MediaStreamSourceDemo
{
    using System;
    using System.Collections.Generic;
    using System.Linq;
    using System.Net;
    using System.Windows;
    using System.Windows.Controls;
    using System.Windows.Documents;
    using System.Windows.Input;
    using System.Windows.Media;
    using System.Windows.Media.Animation;
    using System.Windows.Shapes;

    /// <summary>
    /// A Silverlight Application 
    /// </summary>
    public partial class App : Application
    {
        /// <summary>
        /// Initializes a new instance of the App class.
        /// </summary>
        public App()
        {
            this.Startup += this.Application_Startup;
            this.Exit += this.Application_Exit;
            this.UnhandledException += this.Application_UnhandledException;

            InitializeComponent();
        }

        /// <summary>
        /// Occurs when an application is started.
        /// </summary>
        /// <param name="sender">
        /// Sender of the event.
        /// </param>
        /// <param name="e">
        /// State when the event was generated.
        /// </param>
        private void Application_Startup(object sender, StartupEventArgs e)
        {
            this.RootVisual = new Page();
        }

        /// <summary>
        /// Occurs just before an application shuts down.
        /// </summary>
        /// <param name="sender">
        /// Sender of the event.
        /// </param>
        /// <param name="e">
        /// State when the event was generated.
        /// </param>
        private void Application_Exit(object sender, EventArgs e)
        {
        }

        /// <summary>
        /// Handle exceptions not handled by the application.
        /// </summary>
        /// <param name="sender">
        /// Sender of the event.
        /// </param>
        /// <param name="e">
        /// State when the event was generated.
        /// </param>
        private void Application_UnhandledException(object sender, ApplicationUnhandledExceptionEventArgs e)
        {
            // If the app is running outside of the debugger then report the exception using
            // the browser's exception mechanism. On IE this will display it a yellow alert 
            // icon in the status bar and Firefox will display a script error.
            if (!System.Diagnostics.Debugger.IsAttached)
            {
                // NOTE: This will allow the application to continue running after an exception has been thrown
                // but not handled. 
                // For production applications this error handling should be replaced with something that will 
                // report the error to the website and stop the application.
                e.Handled = true;

                try
                {
                    string errorMsg = e.ExceptionObject.Message + e.ExceptionObject.StackTrace;
                    errorMsg = errorMsg.Replace('"', '\'').Replace("\r\n", @"\n");

                    System.Windows.Browser.HtmlPage.Window.Eval("throw new Error(\"Unhandled Error in Silverlight 2 Application " + errorMsg + "\");");
                }
                catch (Exception)
                {
                }
            }
        }
    }
}
