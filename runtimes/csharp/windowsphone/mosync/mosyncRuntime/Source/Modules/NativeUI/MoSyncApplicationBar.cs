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
        public class ApplicationBar : WidgetBaseWindowsPhone
        {
            protected Microsoft.Phone.Shell.ApplicationBar mApplicationBar;

            public ApplicationBar()
            {
                mApplicationBar = new Microsoft.Phone.Shell.ApplicationBar();

                mView = null;
            }

            public override void AddChild(IWidget child)
            {
                if (child is ApplicationBarButton)
                {
                    Microsoft.Phone.Shell.ApplicationBarIconButton button = new Microsoft.Phone.Shell.ApplicationBarIconButton();
                    button.IconUri = new Uri((child as ApplicationBarButton).IconUri, UriKind.Relative);
                    button.Text = (child as ApplicationBarButton).Text;

                    mApplicationBar.Buttons.Add(button);
                }
                else if (child is ApplicationBarMenuItem)
                {
                    Microsoft.Phone.Shell.ApplicationBarMenuItem menuItem = new Microsoft.Phone.Shell.ApplicationBarMenuItem();
                    menuItem.Text = (child as ApplicationBarMenuItem).Text;
                    mApplicationBar.MenuItems.Add(menuItem);
                }
            }

            public Microsoft.Phone.Shell.ApplicationBar GetApplicationBar()
            {
                return mApplicationBar;
            }
        }
    }
}
