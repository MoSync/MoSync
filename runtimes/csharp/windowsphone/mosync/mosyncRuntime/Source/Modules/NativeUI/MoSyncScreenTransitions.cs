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
         * MoSyncScreenTransitions::isTransitionAvailable method so that it
         * validates the new transitions as available on WP.
         */
        sealed public class MoSyncScreenTransitions
        {
            static public void doShowWithScreenTransition(Grid nextPage, int screenTransitionType)
            {
                //Sets the content of the mainPage.xaml as our screen content with transitions/animation.
                switch (screenTransitionType)
                {
                    case MoSync.Constants.MAW_TRANSITION_TYPE_NONE:
                        PhoneApplicationPage page = (PhoneApplicationPage)((PhoneApplicationFrame)Application.Current.RootVisual).Content;
                        page.Content = nextPage;
                        break;
                    case MoSync.Constants.MAW_TRANSITION_TYPE_SLIDE_RIGHT:
                        MoSyncScreenTransitions.doSlideRightTransition(nextPage);
                        break;
                    case MoSync.Constants.MAW_TRANSITION_TYPE_SLIDE_LEFT:
                        MoSyncScreenTransitions.doSlideLeftTransition(nextPage);
                        break;
                    case MoSync.Constants.MAW_TRANSITION_TYPE_SWIVEL_IN:
                        MoSyncScreenTransitions.doSwivelInTransition(nextPage);
                        break;
                    case MoSync.Constants.MAW_TRANSITION_TYPE_SWIVEL_OUT:
                        MoSyncScreenTransitions.doSwivelOutTransition(nextPage);
                        break;
                    case MoSync.Constants.MAW_TRANSITION_TYPE_TURNSTILE_FOREWARD:
                        MoSyncScreenTransitions.doTurnstileForewardTransition(nextPage);
                        break;
                    case MoSync.Constants.MAW_TRANSITION_TYPE_TURNSTILE_BACKWARD:
                        MoSyncScreenTransitions.doTurnstileBackwardTransition(nextPage);
                        break;
                    default:
                        // It should not reach this point.
                        break;
                }
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
                // TODO Find a better solution to validate platform availability for screen transitions.
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

            /**
             * Applies transition to the current page and calls the given delegate in a specific moment
             * (before/after animation) in order to distingues between transition applied on the current
             * page or the next.
             *
             * @param aDelegate a delegate invoked in different moments, depending on the transition type.
             * @param transitionType a transition type.
             */
            static public void doStackScreenTransition(Delegate_AddChildDelegate aDelegate, int transitionType)
            {
                if (null != aDelegate)
                {
                    TransitionElement transition = null;
                    bool doTransitionOnCurrentScreen = false;
                    switch (transitionType)
                    {
                        case MoSync.Constants.MAW_TRANSITION_TYPE_SLIDE_RIGHT:
                             transition = new SlideTransition();
                            (transition as SlideTransition).Mode = SlideTransitionMode.SlideRightFadeIn;
                            break;
                        case MoSync.Constants.MAW_TRANSITION_TYPE_SLIDE_LEFT:
                            transition = new SlideTransition();
                            (transition as SlideTransition).Mode = SlideTransitionMode.SlideLeftFadeIn;
                            break;
                        case MoSync.Constants.MAW_TRANSITION_TYPE_SWIVEL_IN:
                            transition = new SwivelTransition();
                            (transition as SwivelTransition).Mode = SwivelTransitionMode.FullScreenIn;
                            break;
                        case MoSync.Constants.MAW_TRANSITION_TYPE_SWIVEL_OUT:
                            transition = new SwivelTransition();
                            (transition as SwivelTransition).Mode = SwivelTransitionMode.FullScreenOut;
                            doTransitionOnCurrentScreen = true;
                            break;
                        case MoSync.Constants.MAW_TRANSITION_TYPE_TURNSTILE_FOREWARD:
                            transition = new TurnstileTransition();
                            (transition as TurnstileTransition).Mode = TurnstileTransitionMode.ForwardOut;
                            doTransitionOnCurrentScreen = true;
                            break;
                        case MoSync.Constants.MAW_TRANSITION_TYPE_TURNSTILE_BACKWARD:
                            transition = new TurnstileTransition();
                            (transition as TurnstileTransition).Mode = TurnstileTransitionMode.BackwardIn;
                            break;
                        case MoSync.Constants.MAW_TRANSITION_TYPE_NONE:
                        default:
                            aDelegate();
                            return;
                    }
                    PhoneApplicationPage page = (PhoneApplicationPage)((PhoneApplicationFrame)Application.Current.RootVisual).Content;

                    ITransition transInterf = transition.GetTransition(page);
                    transInterf.Completed += delegate
                    {
                        transInterf.Stop();
                        if (doTransitionOnCurrentScreen)
                        {
                            aDelegate();
                        }
                    };
                    transInterf.Begin();
                    if (!doTransitionOnCurrentScreen)
                    {
                        aDelegate();
                    }
                }
            }

            /**
             * Sets the content of the mainPage.xaml as our screen content with slide left transition.
             *
             * @param nextPage the next screen/content that will be displayed.
             */
            static private void doSlideLeftTransition(Grid nextPage)
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
            static private void doSlideRightTransition(Grid nextPage)
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
            static private void doSwivelInTransition(Grid nextPage)
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
            static private void doSwivelOutTransition(Grid nextPage)
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
            static private void doTurnstileForewardTransition(Grid nextPage)
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
            static private void doTurnstileBackwardTransition(Grid nextPage)
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
        }
    }
}
