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
 * @file SecondScreenObserver.h
 * @author Bogdan Iusco & Mircea Vasiliniuc
 * @date 20 Nov 2012
 *
 * @brief Second screen observer.
 */
#ifndef SECOND_SCREEN_OBSERVER_H_
#define SECOND_SCREEN_OBSERVER_H_

namespace ScreenTransitionTest
{

	class LastScreenObserver
	{
	public:
		/**
		 * Tell the observer to hide the second screen.
		 */
		virtual void hideLastScreen() = 0;
	};
} //end of ScreenTransitionTest


#endif /* SECOND_SCREEN_OBSERVER_H_ */
