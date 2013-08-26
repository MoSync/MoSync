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

/**
 * @file LOG.h
 * @author Gabriela Rata
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

/**
   * \brief Functions for displaying a Facebook object.
   * It adds to the string parameter, all the fields of the object.
   */


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
