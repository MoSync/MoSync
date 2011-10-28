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
using Microsoft.Phone.Controls;

namespace HelloButton
{
    public partial class MainPage : PhoneApplicationPage
    {

        private string benchRes;
        // Constructor
        public MainPage()
        {
            InitializeComponent();
        }

        private void StartButton_Click(object sender, RoutedEventArgs e)
        {

            StartButton.Content = "Running...";

            StropBench sb = new StropBench();
            benchRes = sb.bench();
            MainText.Text = benchRes;

            StartButton.Content = "Done!";
   
        }

    }
}