//-----------------------------------------------------------------------
// <copyright file="Page.xaml.cs" company="Larry Olson">
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
    using Media;

    /// <summary>
    /// A Page of a Silvelight Application.
    /// </summary>
    public partial class Page : UserControl
    {
        /// <summary>
        /// Initializes a new instance of the Page class.
        /// </summary>
        public Page()
        {
            InitializeComponent();
        }

        /// <summary>
        /// Event handler for the Button on the Page.
        /// </summary>
        /// <param name="sender">
        /// The button which was clicked.
        /// </param>
        /// <param name="e">
        /// The state when this event was generated.
        /// </param>
        private void OpenMedia(object sender, RoutedEventArgs e)
        {
            OpenFileDialog ofd = new OpenFileDialog();
            ofd.ShowDialog();

            Mp3MediaStreamSource mediaSource = new Mp3MediaStreamSource(ofd.File.OpenRead());
            me.SetSource(mediaSource);
        }
    }
}
