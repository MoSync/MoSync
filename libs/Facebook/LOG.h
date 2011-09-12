/* Copyright (C) 2011 MoSync AB

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
 * LOG.h
 *
 *  Created on: Jul 1, 2011
 *      Author: gabi
 */

#ifndef LOG_H_
#define LOG_H_

#include "GraphAPI/GetFacebookObjects/FacebookObjects/Post.h"
#include "GraphAPI/GetFacebookObjects/FacebookObjects/Photo.h"
#include "GraphAPI/GetFacebookObjects/FacebookObjects/User.h"
#include "GraphAPI/GetFacebookObjects/FacebookObjects/Video.h"
#include "GraphAPI/GetFacebookObjects/FacebookObjects/Link.h"
#include "GraphAPI/GetFacebookObjects/FacebookObjects/Group.h"
#include "GraphAPI/GetFacebookObjects/FacebookObjects/Album.h"
#include "GraphAPI/GetFacebookObjects/FacebookObjects/Event.h"
#include "GraphAPI/GetFacebookObjects/FacebookObjects/User.h"
#include "GraphAPI/GetFacebookObjects/FacebookObjects/Note.h"
#include "GraphAPI/GetFacebookObjects/FacebookObjects/Checkin.h"
#include "GraphAPI/GetFacebookObjects/FacebookObjects/StatusMessage.h"
#include "GraphAPI/GetFacebookObjects/FacebookObjects/Utilities.h"

#include <mavsprintf.h>
#include <MAUtil/String.h>

#define LOG lprintfln


void display(const Venue &venue, const MAUtil::String &prefix = "\t");
void display(const Place &place, const MAUtil::String &prefix = "\t");
void display(const Action &action, const MAUtil::String &prefix = "\t");
void display(const CategoryData &like, const MAUtil::String &prefix = "\t");
void display(const EventResponse &like, const MAUtil::String &prefix = "\t");
void display(const IdNamePair &id_name, const MAUtil::String &prefix = "\t");

void display(const Comment &comment, const MAUtil::String &prefix = "\t");
void display(const Post &post, const MAUtil::String &prefix = "\t");
void display(const User &user, const MAUtil::String &prefix = "\t");
void display(const Photo &photo, const MAUtil::String &prefix = "\t");
void display(const Video &video, const MAUtil::String &prefix = "\t");
void display(const Link &link, const MAUtil::String &prefix = "\t");
void display(const Group &group, const MAUtil::String &prefix = "\t");
void display(const Album &album, const MAUtil::String &prefix = "\t");
void display(const Event &event, const MAUtil::String &prefix = "\t");
void display(const Checkin &checkin, const MAUtil::String &prefix = "\t");

void display(const Note &note, const MAUtil::String &prefix = "\t");
void display(const StatusMessage &msg, const MAUtil::String &prefix = "\t");

#endif /* LOG_H_ */
