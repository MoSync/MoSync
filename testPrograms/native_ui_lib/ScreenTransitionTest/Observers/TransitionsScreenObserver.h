/* Copyright 2013 David Axmark

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

	http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

/**
 * @file FirstScreenObserver.h
 * @author Bogdan Iusco & Mircea Vasiliniuc
 * @date 20 Nov 2012
 *
 * @brief First screen observer.
 */
#ifndef TRANSITIONS_SCREEN_OBSERVER_H_
#define TRANSITIONS_SCREEN_OBSERVER_H_

namespace ScreenTransitionTest
{
	class TransitionsScreenObserver
	{
	public:
		/**
		 * Tell the observer to show the last screen.
		 */
		virtual void showLastScreen(int transType) = 0;
	};
} // end of ScreenTransitionTest


#endif /* TRANSITIONS_SCREEN_OBSERVER_H_ */
