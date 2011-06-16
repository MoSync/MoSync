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
 * @file GLView.h
 * @author TODO: Add author.
 *
 * Class for OpenGL views.
 */

#ifndef MOSYNC_UI_GL_VIEW_H_
#define MOSYNC_UI_GL_VIEW_H_

#include "Widget.h"

namespace MoSync
{
	namespace UI
	{

	/**
	 * Class for OpenGL views.
	 */
	class GLView : public Widget
	{
	public:
		/**
		 * Constructor.
		 */
		GLView();

		/**
		 * Destructor.
		 */
		virtual ~GLView();
	};

	} // namespace UI
} // namespace MoSync

#endif
