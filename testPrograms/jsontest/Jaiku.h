/* Copyright (C) 2009 Mobile Sorcery AB

This program is free software; you can redistribute it and/or modify it under
the terms of the GNU General Public License, version 2, as published by
the Free Software Foundation.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
for more details.

You should have received a copy of the GNU General Public License
along with this program; see the file COPYING.  If not, write to the Free
Software Foundation, 59 Temple Place - Suite 330, Boston, MA
02111-1307, USA.
*/

#ifndef _JAIKU_H_
#define _JAIKU_H_

#include <MAUtil/Vector.h>
#include <MAUtil/String.h>

namespace Jaiku {
	
	string BaseUserInfo {
		String avatar;
		String firstName;
		String lastName;
		String nick;
		String url;
	};
	
	struct UserInfo : public BaseUserInfo {
		Vector<UserInfo> contacts;
	};

	struct StreamItem {
		String id;
		String title;
		String content;
		String icon;
		String url;
		String createdAt;
		String createdAtRelative;
		String comments;
		BaseUserInfo* user;
	};
	
	class UserStream {
		String title;
		String url;
		
	};
	
	
	
}

