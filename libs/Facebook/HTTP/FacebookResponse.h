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

/*
 * FacebookResponse.h
 */

#ifndef FACEBOOKRESPONSE_H_
#define FACEBOOKRESPONSE_H_

#include <maapi.h>
#include "../JSON_lib/YAJLDom.h"
#include "HttpResponse.h"

using namespace MAUtil;

class FacebookResponse : public HttpResponse {
public:
public:
	FacebookResponse(int code, int dataSize, const byte* data);
	virtual YAJLDom::Value* getJsonData() const;
	virtual MAHandle getImageData() const;
};

#endif /* FACEBOOKRESPONSE_H_ */
