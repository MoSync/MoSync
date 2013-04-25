//-----------------------------------------------------------------------
// <copyright file="MainPage.xaml.cs" company="Larry Olson">
// (c) Copyright Larry Olson.
// This source is subject to the Microsoft Public License (Ms-PL)
// See http://code.msdn.microsoft.com/ManagedMediaHelpers/Project/License.aspx
// All other rights reserved.
// </copyright>
//-----------------------------------------------------------------------
namespace Mp3MediaStreamSourceWP7Demo
{
    using System;
    using System.IO;
    using System.Net;
    using System.Windows;
    using Media;
    using Microsoft.Phone.Controls;

    /// <summary>
    /// The MainPage of the application.
    /// </summary>
    public partial class MainPage : PhoneApplicationPage
    {
        /// <summary>
        /// REPLACE ME.
        /// This field should be filled in with wherever your sample mp3 media file is hosted.
        /// </summary>
			private static string mediaFileLocation = "HTTP://localhost:5002/mobilesorcery2.mp3";

        /// <summary>
        /// The HTTP WebRequest that will get a media stream.
        /// </summary>
        private static HttpWebRequest request = null;

        /// <summary>
        /// Hold onto the MediaStreamSource to callback.
        /// </summary>
        private static Mp3MediaStreamSource mss = null;

        /// <summary>
        ///  Initializes a new instance of the MainPage class.
        /// </summary>
        public MainPage()
        {
            InitializeComponent();
        }
        
        /// <summary>
        /// Handles the HTTP WebRequests' callback.
        /// </summary>
        /// <param name="asyncResult">the result of the callback</param>
        private void RequestCallback(IAsyncResult asyncResult)
        {
            HttpWebResponse response = request.EndGetResponse(asyncResult) as HttpWebResponse;
            Stream s = response.GetResponseStream();
            mss = new Mp3MediaStreamSource(s, response.ContentLength);
            Deployment.Current.Dispatcher.BeginInvoke(
                () =>
                {
                    this.wp7AudioElement.Volume = 100;
                    this.wp7AudioElement.SetSource(mss);
                });
        }

        /// <summary>
        /// Handles the button click.
        /// </summary>
        /// <param name="sender">the button</param>
        /// <param name="e">the events args</param>
        private void Button_Click(object sender, RoutedEventArgs e)
        {
            request = WebRequest.CreateHttp(MainPage.mediaFileLocation);
            
            // NOTICE 
            // Makes this demo code easier but I wouldn't do this on a live phone as it will cause the whole 
            // file to download into memory at once.
            //
            // Instead, use the asynchronous methods and read the stream in the backgound and dispatch its
            // data as needed to the ReportGetSampleCompleted call on the UIThread.
            request.AllowReadStreamBuffering = true; 
            IAsyncResult result = request.BeginGetResponse(new AsyncCallback(this.RequestCallback), null);
        }
    }
}