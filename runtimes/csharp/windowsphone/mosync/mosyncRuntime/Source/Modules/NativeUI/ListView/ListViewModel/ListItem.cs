using System;
using System.Net;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Documents;
using System.Windows.Ink;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Animation;
using System.Windows.Shapes;
using System.ComponentModel;

namespace MoSync
{
    namespace NativeUI
    {
        public class ListItem : INotifyPropertyChanged
        {
            public event PropertyChangedEventHandler PropertyChanged;

            #region Private members

            private Brush mBackgroundColor;
            private Grid mContent;

            // if the values are not set, the size of the Item is the same
            // as the size of its content (mContent)
            private double mHeight;
            private double mWidth;

            #endregion

            #region Constructors

            public ListItem()
            {
                mBackgroundColor = new SolidColorBrush(Colors.Black);
                mContent = new Grid();
            }

            #endregion

            #region Properties

            public double Height
            {
                get
                {
                    return mHeight;
                }
                set
                {
                    mHeight = value;
                    OnPropertyChanged("Height");
                }
            }

            public double Width
            {
                get
                {
                    return mWidth;
                }
                set
                {
                    mWidth = value;
                    OnPropertyChanged("Width");
                }
            }

            public Grid Content
            {
                get
                {
                    return mContent;
                }
                set
                {
                    mContent = value;
                    OnPropertyChanged("Content");
                }
            }

            public string GroupBy
            {
                get;
                set;
            }

            public Brush BackgroundColor
            {
                get
                {
                    return mBackgroundColor;
                }
                set
                {
                    mBackgroundColor = value;
                    OnPropertyChanged("BackgroundColor");
                }
            }

            #endregion

            #region Private methods

            private void OnPropertyChanged(string property)
            {
                if (PropertyChanged != null)
                {
                    PropertyChanged(this, new PropertyChangedEventArgs(property));
                }
            }

            #endregion
        }
    } // end of NativeUI namespace
} // end of MoSync namespace