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
using Microsoft.Phone;

namespace MoSync
{
    namespace NativeUI
    {
        public class ApplicationBarButton : WidgetBaseWindowsPhone
        {
            protected System.Windows.Media.Imaging.BitmapImage mIcon;
            protected System.Uri mIconUri;
            protected System.String mText;
            protected int iconID;

            public ApplicationBarButton()
            {
                Random rdGen = new Random();
                iconID = rdGen.Next();
                mView = null;
            }

            [MoSyncWidgetProperty(MoSync.Constants.MAW_APPLICATION_BAR_BUTTON_ICON)]
            public String Icon
            {
                set
                {
                    int val;
                    if (Int32.TryParse(value, out val))
                    {
                        Resource res = mRuntime.GetResource(MoSync.Constants.RT_IMAGE, val);
                        if (null != res)
                        {
                            System.Windows.Media.Imaging.WriteableBitmap bmp = (res.GetInternalObject() as System.Windows.Media.Imaging.WriteableBitmap);

                            System.IO.MemoryStream stream = new System.IO.MemoryStream();

                            System.Windows.Media.Imaging.Extensions.SaveJpeg(bmp, stream, bmp.PixelWidth, bmp.PixelHeight, 0, 100);

                            String path = "applicationBarButton" + iconID.ToString() + ".png";

                            mIconUri = new Uri(path, UriKind.Relative); //create the file uri

                            System.IO.IsolatedStorage.IsolatedStorageFileStream fs =  System.IO.IsolatedStorage.IsolatedStorageFile.GetUserStoreForApplication().CreateFile(path);
                            stream.WriteTo(fs);
                        }
                    }
                }
            }

            [MoSyncWidgetProperty(MoSync.Constants.MAW_APPLICATION_BAR_BUTTON_TEXT)]
            public String Text
            {
                set
                {
                    mText = value;
                }
                get
                {
                    return mText;
                }
            }

            public Uri iconUri()
            {
                return mIconUri;
            }
        }
    }
}
