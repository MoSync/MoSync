/* Copyright (C) 2011 MoSync AB

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License,
version 2, as published by the Free Software Foundation.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
MA 02110-1301, USA.
*/
/**
 * @file MoSyncRelativeLayout.cs
 * @author Ciprian Filipas
 *
 * @brief This represents the Relative Layout Widget implementation for the NativeUI
 *        component on Windows Phone 7, language c#
 *
 * @platform WP 7.1
 **/

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
