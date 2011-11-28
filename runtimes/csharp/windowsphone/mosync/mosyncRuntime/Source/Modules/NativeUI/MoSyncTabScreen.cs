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

namespace MoSync
{
    namespace NativeUI
    {
        public class TabScreen : Screen
        {
            protected Microsoft.Phone.Controls.Pivot mPivot;

            public TabScreen()
            {
                mPivot = new Microsoft.Phone.Controls.Pivot();
                mPivot.Title = "My Application";
                View = mPivot;
            }

            public override void AddChild(IWidget child)
            {
                base.AddChild(child);
                if (child is Screen)
                {
                    MoSync.Util.RunActionOnMainThreadSync(() =>
                        {
                            Microsoft.Phone.Controls.PivotItem pivItem = new Microsoft.Phone.Controls.PivotItem();
                            pivItem.Height = 200;
                            //pivItem.Content = child;
                            pivItem.Header = ((child as Screen).View as Microsoft.Phone.Controls.PhoneApplicationPage).Title;
                            mPivot.Items.Add(pivItem);
                        }
                    );
                }
            }
        }
    }
}
