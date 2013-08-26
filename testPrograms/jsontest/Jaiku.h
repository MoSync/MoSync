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

