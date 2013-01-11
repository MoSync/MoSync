/* Copyright (C) 2013 MoSync AB

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
 * @file MoSyncScreenTransitions.cs
 * @author Mircea Vasiliniuc
 *
 * @brief This holds the Screen transitions available in
 *          #WPToolkit.
 * @platform WP 7
 **/

using System;
using Microsoft.Phone.Controls;
using System.Windows.Controls;
using System.Windows;

namespace MoSync
{
    namespace NativeUI
    {
        /*
         * When adding a new screen transition make sure you modify the
         * ScreenTransitions::isScreenTransitionAvailable method so that it
         * validates the new transitions as available on Android.
         */
        public class MoSyncScreenTransitions
        {
            /**
             * Sets the content of the mainPage.xaml as our screen content with slide left transition.
             *
             * @param nextPage the next screen/content that will be displayed.
             */
            static public void doSlideLeftTransition(Grid nextPage)
            {
                PhoneApplicationPage page = (PhoneApplicationPage)((PhoneApplicationFrame)Application.Current.RootVisual).Content;

                TransitionElement transition = new SlideTransition();
                (transition as SlideTransition).Mode = SlideTransitionMode.SlideLeftFadeIn;

                ITransition transInterf = transition.GetTransition(page);
                transInterf.Completed += delegate
                {
                    transInterf.Stop();
                };
                transInterf.Begin();
                page.Content = nextPage;
            }

            /**
             * Sets the content of the mainPage.xaml as our screen content with slide right transition.
             *
             * @param nextPage the next screen/content that will be displayed.
             */
            static public void doSlideRightTransition(Grid nextPage)
            {
                PhoneApplicationPage page = (PhoneApplicationPage)((PhoneApplicationFrame)Application.Current.RootVisual).Content;

                TransitionElement transition = new SlideTransition();
                (transition as SlideTransition).Mode = SlideTransitionMode.SlideRightFadeIn;

                ITransition transInterf = transition.GetTransition(page);
                transInterf.Completed += delegate
                {
                    transInterf.Stop();
                };
                transInterf.Begin();
                page.Content = nextPage;
            }

            /**
             * Sets the content of the mainPage.xaml as our screen content with a swivel in transition.
             *
             * @param nextPage the next screen/content that will be displayed.
             */
            static public void doSwivelInTransition(Grid nextPage)
            {
                PhoneApplicationPage page = (PhoneApplicationPage)((PhoneApplicationFrame)Application.Current.RootVisual).Content;

                TransitionElement transition = new SwivelTransition();
                (transition as SwivelTransition).Mode = SwivelTransitionMode.FullScreenIn;

                ITransition transInterf = transition.GetTransition(page);
                transInterf.Completed += delegate
                {
                    transInterf.Stop();
                };
                transInterf.Begin();
                page.Content = nextPage;
            }

            /**
             * Sets the content of the mainPage.xaml as our screen content with a swivel out transition.
             *
             * @param nextPage the next screen/content that will be displayed.
             */
            static public void doSwivelOutTransition(Grid nextPage)
            {
                PhoneApplicationPage page = (PhoneApplicationPage)((PhoneApplicationFrame)Application.Current.RootVisual).Content;

                TransitionElement transition = new SwivelTransition();
                (transition as SwivelTransition).Mode = SwivelTransitionMode.FullScreenOut;

                ITransition transInterf = transition.GetTransition(page);
                transInterf.Completed += delegate
                {
                    transInterf.Stop();
                    page.Content = nextPage;
                };
                transInterf.Begin();
            }

            /**
             * Sets the content of the mainPage.xaml as our screen content with a tunstile foreward transition.
             *
             * @param nextPage the next screen/content that will be displayed.
             */
            static public void doTurnstileForewardTransition(Grid nextPage)
            {
                PhoneApplicationPage page = (PhoneApplicationPage)((PhoneApplicationFrame)Application.Current.RootVisual).Content;

                TransitionElement transition = new TurnstileTransition();
                (transition as TurnstileTransition).Mode = TurnstileTransitionMode.ForwardOut;

                ITransition transInterf = transition.GetTransition(page);
                transInterf.Completed += delegate
                {
                    transInterf.Stop();
                    page.Content = nextPage;
                };
                transInterf.Begin();
            }

            /**
             * Sets the content of the mainPage.xaml as our screen content with a tunstile backward transition.
             *
             * @param nextPage the next screen/content that will be displayed.
             */
            static public void doTurnstileBackwardTransition(Grid nextPage)
            {
                PhoneApplicationPage page = (PhoneApplicationPage)((PhoneApplicationFrame)Application.Current.RootVisual).Content;

                TransitionElement transition = new TurnstileTransition();
                (transition as TurnstileTransition).Mode = TurnstileTransitionMode.BackwardIn;

                ITransition transInterf = transition.GetTransition(page);
                transInterf.Completed += delegate
                {
                    transInterf.Stop();
                };
                transInterf.Begin();
                page.Content = nextPage;
            }

            /**
             * Checks if the screen transititon given as parameter is available on WP.
             * Screen transition can be found \link #MA_TRANSITION_TYPE_NONE here \endlink.
             *
             * @param screenTransitionType a transition type.
             *
             * @return True if the screen transititon given as parameter is available on WP, false otherwise.
             */
            static public bool isTransitionAvailable(int screenTransitionType)
            {
                // TODO Find a better solution to validate platform availabiliry for screen transitions.
                switch (screenTransitionType)
                {
                    case MoSync.Constants.MAW_TRANSITION_TYPE_SLIDE_RIGHT:
                    case MoSync.Constants.MAW_TRANSITION_TYPE_SLIDE_LEFT:
                    case MoSync.Constants.MAW_TRANSITION_TYPE_SWIVEL_IN:
                    case MoSync.Constants.MAW_TRANSITION_TYPE_SWIVEL_OUT:
                    case MoSync.Constants.MAW_TRANSITION_TYPE_TURNSTILE_FOREWARD:
                    case MoSync.Constants.MAW_TRANSITION_TYPE_TURNSTILE_BACKWARD:
                    case MoSync.Constants.MAW_TRANSITION_TYPE_NONE:
                        return true;
                    default:
                        return false;
                }
            }
        }
    }
}
