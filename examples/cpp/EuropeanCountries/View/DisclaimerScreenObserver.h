/*
 Copyright (C) 2012 MoSync AB

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
 * @file DisclaimerScreenObserver.h
 * @author Bogdan Iusco
 *
 * @brief Observer for DisclaimerScreen.
 */

#ifndef EC_DISCLAIMER_SCREEN_OBSERVER_H_
#define EC_DISCLAIMER_SCREEN_OBSERVER_H_

namespace EuropeanCountries
{
	/**
	 * Observer for DisclaimerScreen.
	 */
	class DisclaimerScreenObserver
	{
	public:
		/**
		 * Show country info screen.
		 * Called when user taps the back button.
		 */
		virtual void showCountryInfoScreen() = 0;
	};

} // end of EuropeanCountries

#endif /* EC_DISCLAIMER_SCREEN_OBSERVER_H_ */
