/*
Copyright (C) 2011 MoSync AB

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
 * @file Button.cpp
 * @author Mikael Kindborg
 *
 * Class for buttons.
 */

#include "Button.h"

namespace MoSync
{
	namespace UI
	{

	/**
	 * Constructor.
	 */
	Button::Button() :
		TextWidget(MAW_BUTTON)
	{
		// Set some common default values to
		// simplify when creating buttons.
		this->fillSpaceHorizontally();
		this->wrapContentVertically();
		this->setText("Click Me!");
		this->centerTextHorizontally();
		this->centerTextVertically();
	}

	/**
	 * Destructor.
	 */
	Button::~Button()
	{
	}

	} // namespace UI
} // namespace MoSync
