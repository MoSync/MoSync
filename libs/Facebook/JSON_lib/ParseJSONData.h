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
 * JSONParser.h
 *
 *  Created on: Jun 30, 2011
 *      Author: gabi
 */

#ifndef JSONPARSER_H_
#define JSONPARSER_H_

#include <MAUtil/Set.h>

#include "YAJLDom.h"
#include "GraphAPI/GetFacebookObjects/FacebookObjects/Utilities.h"
#include "LOG.h"

class Album;
class Comment;
class Checkin;
class Event;
class Group;
class Link;
class Page;
class Post;
class Photo;
class StatusMessage;
class User;
class Video;
class Note;

/*
 * function that parses the JSON data retrieved from server and and fills an Album object with it.
 */
void parseJSONData(Album &album, YAJLDom::Value* result);
void parseJSONData(Album &album, YAJLDom::Value* result, const MAUtil::Set<MAUtil::String> &fields);

/*
 * function that parses the JSON data retrieved from server and fills an Comment object with it.
 */
void parseJSONData(Comment &comment, YAJLDom::Value* result);
void parseJSONData(Comment &comment, YAJLDom::Value* result, const MAUtil::Set<MAUtil::String> &fields);

/*
 * function that parses the JSON data retrieved from server and fills an Checkin object with it.
 */
void parseJSONData(Checkin &checkin, YAJLDom::Value* result);
void parseJSONData(Checkin &checkin, YAJLDom::Value* result, const MAUtil::Set<MAUtil::String> &fields);

/*
 * function that parses the JSON data retrieved from server and fills an Event object with it.
 */
void parseJSONData(Event &event, YAJLDom::Value* result);
void parseJSONData(Event &event, YAJLDom::Value* result, const MAUtil::Set<MAUtil::String> &fields);

/*
 * function that parses the JSON data retrieved from server and fills an Group object with it.
 */
void parseJSONData(Group &group, YAJLDom::Value* result);
void parseJSONData(Group &group, YAJLDom::Value* result, const MAUtil::Set<MAUtil::String> &fields);

/*
 * function that parses the JSON data retrieved from server and fills an Link object with it.
 */
void parseJSONData(Link &link, YAJLDom::Value* result);
void parseJSONData(Link &link, YAJLDom::Value* result, const MAUtil::Set<MAUtil::String> &fields);

//void parseJSONData(Page &page, YAJLDom::Value* result);

/*
 * function that parses the JSON data retrieved from server and fills an Post object with it.
 */
void parseJSONData(Post &post, YAJLDom::Value* result);
void parseJSONData(Post &post, YAJLDom::Value* result, const MAUtil::Set<MAUtil::String> &fields);

/*
 * function that parses the JSON data retrieved from server and fills an Place object with it.
 */
void parseJSONData(Place &place, YAJLDom::Value* result);
void parseJSONData(Place &place, YAJLDom::Value* result, const MAUtil::Set<MAUtil::String> &fields);

/*
 * function that parses the JSON data retrieved from server and fills an Photo object with it.
 */
void parseJSONData(Photo &photo, YAJLDom::Value* result);
void parseJSONData(Photo &photo, YAJLDom::Value* result, const MAUtil::Set<MAUtil::String> &fields);

/*
 * function that parses the JSON data retrieved from server and fills an StatusMessage object with it.
 */
void parseJSONData(StatusMessage &msg, YAJLDom::Value* result);
void parseJSONData(StatusMessage &msg, YAJLDom::Value* result, const MAUtil::Set<MAUtil::String> &fields);

/*
 * function that parses the JSON data retrieved from server and fills an User object with it.
 */
void parseJSONData(User &user, YAJLDom::Value* result);
void parseJSONData(User &user, YAJLDom::Value* result, const MAUtil::Set<MAUtil::String> &fields);

/*
 * function that parses the JSON data retrieved from server and fills an Video object with it.
 */
void parseJSONData(Video &video, YAJLDom::Value* result);
void parseJSONData(Video &video, YAJLDom::Value* result, const MAUtil::Set<MAUtil::String> &fields);

/*
 * function that parses the JSON data retrieved from server and fills an CategoryData object with it.
 */
void parseJSONData(CategoryData &like, YAJLDom::Value* result);
void parseJSONData(CategoryData &like, YAJLDom::Value* result, const MAUtil::Set<MAUtil::String> &fields);

/*
 * function that parses the JSON data retrieved from server and fills an Note object with it.
 */
void parseJSONData(Note &notes, YAJLDom::Value* result);
void parseJSONData(Note &video, YAJLDom::Value* result, const MAUtil::Set<MAUtil::String> &fields);

/*
 * function that parses the JSON data retrieved from server and fills an Action object with it.
 */
void parseJSONData(Action &action, YAJLDom::Value* result);
void parseJSONData(Action &action, YAJLDom::Value* result, const MAUtil::Set<MAUtil::String> &fields);

/*
 * function that parses the JSON data retrieved from server and fills an IdNamePair object with it.
 */
void parseJSONData(IdNamePair &notes, YAJLDom::Value* result);
void parseJSONData(IdNamePair &notes, YAJLDom::Value* result, const MAUtil::Set<MAUtil::String> &fields);

/*
 * function that parses the JSON data retrieved from server and fills an EventResponse object with it.
 */
void parseJSONData(EventResponse &event, YAJLDom::Value* result);
void parseJSONData(EventResponse &event, YAJLDom::Value* result, const MAUtil::Set<MAUtil::String> &fields);


/*
 * function that parses JSON data and fills a string with it
 * @param str - the resulting string
 * @param tag - the name of a JSON node.
 * @param result - the JSON data.
 */
bool extract(MAUtil::String &str, const char *tag, YAJLDom::Value* result);

/*
 * function that parses JSON data and fills a IdNamePair object with it
 * @param val - the IdNamePair
 * @param tag - the name of a JSON node.
 * @param result - the JSON data.
 */
bool extract(IdNamePair &val, const char *tag, YAJLDom::Value* result);

/*
 * function converts a JSON value to an int.
 * @param val - the resulting int value
 * @param tag - the name of a JSON node.
 * @param result - the JSON data.
 */
bool extract(int &val, const char *tag, YAJLDom::Value* result);

/*
 * function that parses JSON data and fills a Venue object with it.
 * @param val - the Venue object to be filled.
 * @param tag - the name of a JSON node.
 * @param result - the JSON data.
 */
bool extract(Venue &val, const char *tag, YAJLDom::Value* result);

/*
 * function that parses JSON data and fills a vector with it.
 * @param vector - the Vector object to be filled.
 * @param tag - the name of a JSON node.
 * @param result - the JSON data.
 * @params fields - the fields of the object that need to be parsed.
 */
template<class ObjectType>
bool extract(MAUtil::Vector<ObjectType> &vector, const char *tag, YAJLDom::Value* result,
	MAUtil::Set<MAUtil::String> &fields)
{
	vector.clear();

	YAJLDom::Value* data = result->getValueForKey(tag);

	if(NULL == data)
	{
		return false;
	}

	for (int i = 0; i < data->getNumChildValues(); i++)
	{
		YAJLDom::Value *jsonElement = data->getValueByIndex(i);

		ObjectType object;
		parseJSONData(object, jsonElement, fields);
		vector.add(object);
	}

	return vector.size()>0;
}

/*
 * function that parses JSON data and fills a vector with it.
 * @param vector - the Vector object to be filled.
 * @param tag - the name of a JSON node.
 * @param result - the JSON data.
 */
template<class ObjectType>
bool extract(MAUtil::Vector<ObjectType> &vector, const char *tag, YAJLDom::Value* result)
{
	vector.clear();

	YAJLDom::Value* data = result->getValueForKey(tag);

	if(NULL == data)
	{
		return false;
	}

	for (int i = 0; i < data->getNumChildValues(); i++)
	{
		YAJLDom::Value *jsonElement = data->getValueByIndex(i);

		ObjectType object;
		parseJSONData(object, jsonElement);
		vector.add(object);
	}

	return vector.size()>0;
}


#endif /* JSONPARSER_H_ */
