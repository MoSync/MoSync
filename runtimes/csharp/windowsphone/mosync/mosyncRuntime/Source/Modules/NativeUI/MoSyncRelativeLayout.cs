using Microsoft.Phone.Controls;
using System.Windows.Controls;
using System.Windows;
using System.Windows.Navigation;
using System;
using System.Text.RegularExpressions;
using System.Reflection;

namespace MoSync
{
    namespace NativeUI
    {
        //RelativeLayout class
        public class RelativeLayout : WidgetBaseWindowsPhone
        {
            //Canvas is the content in which the children are aranged relatively to the parent
            protected System.Windows.Controls.Canvas mPanel;

            //Constructor
            public RelativeLayout()
            {
                mPanel = new System.Windows.Controls.Canvas();
                mView = mPanel;
            }

            //add child
            public override void AddChild(IWidget child)
            {
                base.AddChild(child);
                MoSync.Util.RunActionOnMainThreadSync(() =>
                {
                    WidgetBaseWindowsPhone widget = (child as WidgetBaseWindowsPhone);

                    mPanel.Children.Add(widget.View);
                });
            }
        }
    }
}
