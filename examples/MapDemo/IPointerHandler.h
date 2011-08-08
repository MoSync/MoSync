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

#ifndef IPOINTERHANDLER_H_
#define IPOINTERHANDLER_H_

namespace MapDemoUI
{
	//=========================================================================
	// Interface
	//
	class IPointerHandler
	//=========================================================================
	{
	public:
		virtual bool handlePointerPress( MAPoint2d point ) = 0;
		virtual bool handlePointerMove( MAPoint2d point ) = 0;
		virtual bool handlePointerRelease( MAPoint2d point ) = 0;
	};
}

#endif 	// IPOINTERHANDLER_H_
