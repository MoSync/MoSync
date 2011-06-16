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
 * @file GLView.cpp
 * @author Mikael Kindborg
 *
 * Class for horizontal layout of widgets.
 */

#include "GLView.h"

namespace MoSync
{
	namespace UI
	{

	/**
	 * Constructor.
	 */
	GLView::GLView() :
		Widget(MAW_GL_VIEW)
	{
		// Set some common default values.
		this->fillSpaceVertically();
		this->fillSpaceHorizontally();
	}

	/**
	 * Destructor.
	 */
	GLView::~GLView()
	{
	}

	} // namespace UI
} // namespace MoSync
