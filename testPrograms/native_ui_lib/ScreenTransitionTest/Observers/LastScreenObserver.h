/*
 Copyright (C) 2013 MoSync AB

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
