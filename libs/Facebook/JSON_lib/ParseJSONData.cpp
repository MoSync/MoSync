/*
 * JSONParser.cpp
 *
 *  Created on: Jun 30, 2011
 *      Author: gabi
 */

#include <MAUtil/util.h>

#include "ParseJSONData.h"

#include "../GraphAPI/GetFacebookObjects/FacebookObjects/Album.h"
#include "../GraphAPI/GetFacebookObjects/FacebookObjects/Comment.h"
#include "../GraphAPI/GetFacebookObjects/FacebookObjects/Checkin.h"
#include "../GraphAPI/GetFacebookObjects/FacebookObjects/Event.h"
#include "../GraphAPI/GetFacebookObjects/FacebookObjects/Group.h"
#include "../GraphAPI/GetFacebookObjects/FacebookObjects/Link.h"
#include "../GraphAPI/GetFacebookObjects/FacebookObjects/Post.h"
#include "../GraphAPI/GetFacebookObjects/FacebookObjects/Photo.h"
#include "../GraphAPI/GetFacebookObjects/FacebookObjects/StatusMessage.h"
#include "../GraphAPI/GetFacebookObjects/FacebookObjects/User.h"
#include "../GraphAPI/GetFacebookObjects/FacebookObjects/Video.h"
#include "../GraphAPI/GetFacebookObjects/FacebookObjects/Note.h"
#include "../GraphAPI/GetFacebookObjects/FacebookObjects/Utilities.h"
#include "../GraphAPI/GetFacebookObjects/FacebookObjects/User.h"

#include "../LOG.h"

/*
 * function that parses the JSON data retrieved from server and and fills an Album object with it.
 */
void parseJSONData(Album &album, MAUtil::YAJLDom::Value* result)
{
	MAUtil::String str;
	IdNamePair id_name;

	extract(str, "id", result);
	album.setId(str);

	extract(id_name, "from", result);
	album.setFromField(id_name);

	if(extract(str, "name", result))
	{
		album.setName(str);
	}

	if(extract(str, "description", result))
	{
		album.setDescription(str);
	}

//	extract(str, "link", result);
//	album.setLink(str);

	if(extract(str, "cover_photo", result))
	{
		album.setCoverPhotoId(str);
	}

	int count = 0;
	extract(count, "count", result);
	album.setCount(count);

}

void parseJSONData(Album &album, MAUtil::YAJLDom::Value* result, const MAUtil::Set<MAUtil::String> &fields)
{
	MAUtil::String str;
	IdNamePair id_name;

	if( fields.find("id") != fields.end() && extract(str, "id", result))
	{
		album.setId(str);
	}

	if( fields.find("from") != fields.end() && extract(id_name, "from", result) )
	{
		album.setFromField(id_name);
	}

	if( fields.find("name") != fields.end() && extract(str, "name", result))
	{
		album.setName(str);
	}


	if( fields.find("description") != fields.end() && extract(str, "description", result))
	{
		album.setDescription(str);
	}

	if( fields.find("link") != fields.end() && extract(str, "link", result))
	{
		album.setLink(str);
	}

	if( fields.find("cover_photo") != fields.end() && extract(str, "cover_photo", result))
	{
		album.setCoverPhotoId(str);
	}

	int count = 0;
	if( fields.find("count") != fields.end() && extract(count, "count", result))
	{
		album.setCount(count);
	}
}

/*
 * function that parses the JSON data retrieved from server and fills an Comment object with it.
 */
void parseJSONData(Comment &comment, MAUtil::YAJLDom::Value* result)
{
	MAUtil::String str;

	extract(str, "id", result);
	comment.setId(str);

	IdNamePair id_name;
	extract(id_name, "from", result);
	comment.setFromField(id_name);

	extract(str, "message", result);
	comment.setMessage(str);

	int likes = 0;
	extract(likes, "likes", result);
	comment.setLikes(likes);
}

void parseJSONData(Comment &comment, MAUtil::YAJLDom::Value* result, const MAUtil::Set<MAUtil::String> &fields)
{
	MAUtil::String str;

	if(fields.find("id") != fields.end() && extract(str, "id", result) )
	{
		comment.setId(str);
	}

	if(fields.find("from") != fields.end())
	{
		IdNamePair id_name;
		if(extract(id_name, "from", result))
		{
			comment.setFromField(id_name);
		}
	}

	if(fields.find("message") != fields.end() && extract(str, "message", result))
	{
		comment.setMessage(str);
	}

	if(fields.find("likes") != fields.end())
	{
		int likes = 0;
		if(extract(likes, "likes", result))
		{
			comment.setLikes(likes);
		}
	}
}

/*
 * function that parses the JSON data retrieved from server and fills an Checkin object with it.
 */
void parseJSONData(Checkin &checkin, MAUtil::YAJLDom::Value* result)
{
	MAUtil::String str;

	extract(str, "id", result);
	checkin.setId(str);

	IdNamePair id_name;
	extract(id_name, "from", result);
	checkin.setFrom(id_name);

	extract(str, "message", result);
	checkin.setMessage(str);

	MAUtil::YAJLDom::Value *tagsValue = result->getValueForKey("tags");
	if(tagsValue)
	{
		MAUtil::Vector<IdNamePair> tags;
		extract(tags, "data", tagsValue);
	}

	MAUtil::YAJLDom::Value *placeValue = result->getValueForKey("place");
	if(placeValue)
	{
		Place place;
		parseJSONData(place, placeValue);
		checkin.setPlace(place);
	}

	IdNamePair app;
	extract(app, "application", result);
	checkin.setApplication(app);

	MAUtil::YAJLDom::Value *likesValue = result->getValueForKey("likes");
	if(likesValue)
	{
		MAUtil::Vector<IdNamePair> likes;
		extract(likes, "data", likesValue);
		checkin.setLikes(likes);
	}

	MAUtil::YAJLDom::Value *commentsValue = result->getValueForKey("comments");
	if(commentsValue)
	{
		MAUtil::Vector<Comment> comments;
		extract(comments, "data", commentsValue);
		checkin.setComments(comments);
	}
}

void parseJSONData(Checkin &checkin, MAUtil::YAJLDom::Value* result, const MAUtil::Set<MAUtil::String> &fields)
{
	MAUtil::String str;

	if(fields.find("id") != fields.end() && extract(str, "id", result))
	{
		checkin.setId(str);
	}

	if(fields.find("from") != fields.end())
	{
		IdNamePair id_name;
		if(extract(id_name, "from", result))
		{
			checkin.setFrom(id_name);
		}
	}

	if(fields.find("message") != fields.end() && extract(str, "message", result))
	{
		checkin.setMessage(str);
	}

	if(fields.find("tags") != fields.end())
	{
		MAUtil::YAJLDom::Value *tagsValue = result->getValueForKey("tags");
		if(tagsValue)
		{
			MAUtil::Vector<IdNamePair> tags;
			if(extract(tags, "data", tagsValue))
			{
				checkin.setTags(tags);
			}

		}
	}

	if(fields.find("place") != fields.end())
	{
		MAUtil::YAJLDom::Value *placeValue = result->getValueForKey("place");
		if(placeValue)
		{
			Place place;
			parseJSONData(place, placeValue);
			checkin.setPlace(place);
		}
	}

	if(fields.find("application") != fields.end() )
	{
		IdNamePair app;
		if(extract(app, "application", result))
		{
			checkin.setApplication(app);
		}
	}

	if(fields.find("likes") != fields.end())
	{
		MAUtil::YAJLDom::Value *likesValue = result->getValueForKey("likes");
		if(likesValue)
		{
			MAUtil::Vector<IdNamePair> likes;
			if(extract(likes, "data", likesValue))
			{
				checkin.setLikes(likes);
			}
		}
	}


	if(fields.find("comments") != fields.end())
	{
		MAUtil::YAJLDom::Value *commentsValue = result->getValueForKey("comments");
		if(commentsValue)
		{
			MAUtil::Vector<Comment> comments;
			if(extract(comments, "data", commentsValue))
			{
				checkin.setComments(comments);
			}
		}
	}
}

/*
 * function that parses the JSON data retrieved from server and fills an Event object with it.
 */
void parseJSONData(Event &event, MAUtil::YAJLDom::Value* result)
{
	MAUtil::String str;

	extract(str, "id", result);
	event.setId(str);

	IdNamePair id_name;
	extract(id_name, "owner", result);
	event.setOwner(id_name);

	extract(str, "name", result);
	event.setName(str);

	extract(str, "description", result);
	event.setName(str);

	extract(str, "start_time", result);
	event.setStartTime(str);

	extract(str, "end_time", result);
	event.setEndTime(str);

	extract(str, "location", result);
	event.setLocation(str);

	Venue venue;
	extract(venue, "venue", result);
	event.setVenue(venue);

	extract(str, "privacy", result);
	event.setPrivacy(str);
}

void parseJSONData(Event &event, MAUtil::YAJLDom::Value* result, const MAUtil::Set<MAUtil::String> &fields)
{
	MAUtil::String str;

	if(fields.find("id") != fields.end() && extract(str, "id", result))
	{
		event.setId(str);
	}

	if(fields.find("owner") != fields.end())
	{
		IdNamePair id_name;
		if(extract(id_name, "owner", result))
		{
			event.setOwner(id_name);
		}
	}

	if(fields.find("name") != fields.end() && extract(str, "name", result))
	{
		event.setName(str);
	}

	if(fields.find("description") != fields.end() && extract(str, "description", result))
	{
		event.setDescription(str);
	}

	if(fields.find("start_time") != fields.end() && extract(str, "start_time", result))
	{
		event.setStartTime(str);
	}

	if(fields.find("end_time") != fields.end() && extract(str, "end_time", result))
	{
		event.setEndTime(str);
	}

	if(fields.find("location") != fields.end() && extract(str, "location", result))
	{
		event.setLocation(str);
	}

	if(fields.find("venue") != fields.end())
	{
		Venue venue;
		if(extract(venue, "venue", result))
		{
			event.setVenue(venue);
		}
	}

	if(fields.find("privacy") != fields.end() && extract(str, "privacy", result))
	{
		event.setPrivacy(str);
	}
}

/*
 * function that parses the JSON data retrieved from server and fills an Group object with it.
 */
void parseJSONData(Group &group, MAUtil::YAJLDom::Value* result)
{
	MAUtil::String str;

	extract(str, "id", result);
	group.setId(str);

	IdNamePair name_pair;
	extract(name_pair, "owner", result);
	group.setOwner(name_pair);

	extract(str, "name", result);
	group.setName(str);

	extract(str, "description", result);
	group.setDescription(str);

	extract(str, "privacy", result);
	group.setPrivacy(str);
}

void parseJSONData(Group &group, MAUtil::YAJLDom::Value* result, const MAUtil::Set<MAUtil::String> &fields)
{
	MAUtil::String str;

	if(fields.find("id") != fields.end() && extract(str, "id", result))
	{
		group.setId(str);
	}

	if(fields.find("owner") != fields.end())
	{
		IdNamePair name_pair;
		if(extract(name_pair, "owner", result))
		{
			group.setOwner(name_pair);
		}
	}

	if(fields.find("name") != fields.end() && extract(str, "name", result))
	{
		group.setName(str);
	}

	if(fields.find("description") != fields.end() && extract(str, "description", result))
	{
		group.setDescription(str);
	}

	if(fields.find("privacy") != fields.end() && extract(str, "privacy", result))
	{
		group.setPrivacy(str);
	}
}

/*
 * function that parses the JSON data retrieved from server and fills an Link object with it.
 */
void parseJSONData(Link &link, MAUtil::YAJLDom::Value* result)
{
	MAUtil::String str;

	extract(str, "id", result);
	link.setId(str);

	IdNamePair id_name;
	extract(str, "from", result);
	link.setFrom(id_name);

	extract(str, "message", result);
	link.setMessage(str);

	extract(str, "picture", result);
	link.setPicture(str);

	extract(str, "link", result);
	link.setLink(str);

	extract(str, "name", result);
	link.setName(str);

	extract(str, "description", result);
	link.setDescription(str);

	MAUtil::Vector<Comment> comments;
	extract(comments, "comments", result);
	link.setComments(comments);
}

void parseJSONData(Link &link, MAUtil::YAJLDom::Value* result, const MAUtil::Set<MAUtil::String> &fields)
{
	MAUtil::String str;

	if(fields.find("id") != fields.end() && extract(str, "id", result))
	{
		link.setId(str);
	}

	if(fields.find("from") != fields.end())
	{
		IdNamePair id_name;
		if(extract(str, "from", result))
		{
			link.setFrom(id_name);
		}
	}

	if(fields.find("message") != fields.end() && extract(str, "message", result))
	{
		link.setMessage(str);
	}

	if(fields.find("picture") != fields.end() && extract(str, "picture", result))
	{
		link.setPicture(str);
	}

	if(fields.find("link") != fields.end() && extract(str, "link", result))
	{
		link.setLink(str);
	}

	if(fields.find("name") != fields.end() && extract(str, "name", result))
	{
		link.setName(str);
	}

	if(fields.find("description") != fields.end() && extract(str, "description", result))
	{
		link.setDescription(str);
	}

	if(fields.find("comments") != fields.end())
	{
		MAUtil::Vector<Comment> comments;
		if(extract(comments, "comments", result))
		{
			link.setComments(comments);
		}
	}
}

/*
 * function that parses the JSON data retrieved from server and fills an Post object with it.
 */
void parseJSONData(Post &post, MAUtil::YAJLDom::Value* result)
{
	MAUtil::String str;
	IdNamePair id_name;

	if(extract(str, "id", result))
	{
		post.setId(str);
	}

	if(extract(id_name, "from", result))
	{
		post.setFromfield(id_name);
	}

	if(extract(id_name, "to", result))
	{
		post.setToField(id_name);
	}

	if(extract(str, "message", result))
	{
		post.setMessage(str);
	}

	if(extract(str, "picture", result))
	{
		post.setPictureUrl(str);
	}

	if(extract(str, "link", result))
	{
		post.setLink(str);
	}

	if(extract(str, "name", result))
	{
		post.setName(str);
	}

	if(extract(str, "caption", result))
	{
		post.setCaption(str);
	}

	if(extract(str, "description", result))
	{
		post.setDescription(str);
	}

	if(extract(str, "source", result))
	{
		post.setVideoUrl(str);
	}

	MAUtil::Vector<Action> actions;
	if(extract(actions, "actions", result))
	{
		post.setActions(actions);
	}

	if(extract(str, "type", result))
	{
		post.setType(str);
	}

	MAUtil::YAJLDom::Value *likesValue = result->getValueForKey("likes");
	if(likesValue)
	{
		MAUtil::Vector<IdNamePair> likes;
		if(extract(likes, "data", likesValue))
		{
			post.setLikes(likes);
		}
	}

	MAUtil::YAJLDom::Value *commentsValue = result->getValueForKey("comments");
	MAUtil::Vector<Comment> comments;
	if(extract(comments, "data", commentsValue))
	{
		post.setComments(comments);
	}


	if(extract(id_name, "application", result))
	{
		post.setApplication(id_name);
	}

	//object_id
	int object_id = 0;
	if(extract(object_id, "object_id", result))
	{
		post.setUploadedVideoOrPhotoId(MAUtil::integerToString(object_id));
	}
}

void parseJSONData(Post &post, MAUtil::YAJLDom::Value* result, const MAUtil::Set<MAUtil::String> &fields)
{
	MAUtil::String str;
	IdNamePair id_name;

	if(fields.find("id") != fields.end() && extract(str, "id", result))
	{
		post.setId(str);
	}

	if(fields.find("from") != fields.end() && extract(id_name, "from", result))
	{
		post.setFromfield(id_name);
	}

	if(fields.find("to") != fields.end() && extract(id_name, "to", result))
	{
		post.setToField(id_name);
	}

	if(fields.find("message")!= fields.end() && extract(str, "message", result))
	{
		post.setMessage(str);
	}

	if(fields.find("picture")!= fields.end() && extract(str, "picture", result))
	{
		post.setPictureUrl(str);
	}

	if(fields.find("link")!= fields.end() && extract(str, "link", result))
	{
		post.setLink(str);
	}

	if(fields.find("name")!= fields.end() && extract(str, "name", result))
	{
		post.setName(str);
	}

	if(fields.find("caption")!= fields.end() && extract(str, "caption", result))
	{
		post.setCaption(str);
	}

	if(fields.find("description")!= fields.end() && extract(str, "description", result))
	{
		post.setDescription(str);
	}

	if(fields.find("source")!= fields.end() && extract(str, "source", result))
	{
		post.setVideoUrl(str);
	}

	if(fields.find("actions")!= fields.end())
	{
		MAUtil::Vector<Action> actions;
		if(extract(actions, "actions", result))
		{
			post.setActions(actions);
		}
	}

	if(fields.find("type")!= fields.end() && extract(str, "type", result))
	{
		post.setType(str);
	}

	if(fields.find("likes")!= fields.end())
	{
		MAUtil::YAJLDom::Value *likesValue = result->getValueForKey("likes");
		if(likesValue)
		{
			MAUtil::Vector<IdNamePair> likes;
			if(extract(likes, "data", likesValue))
			{
				post.setLikes(likes);
			}
		}
	}

	if(fields.find("comments")!= fields.end())
	{
		MAUtil::YAJLDom::Value *commentsValue = result->getValueForKey("comments");
		MAUtil::Vector<Comment> comments;
		if(extract(comments, "data", commentsValue))
		{
			post.setComments(comments);
		}
	}

	if(fields.find("application")!= fields.end() && extract(id_name, "application", result))
	{
		post.setApplication(id_name);
	}

	//object_id
	int object_id = 0;
	if(fields.find("object_id")!= fields.end() && extract(object_id, "object_id", result))
	{
		post.setUploadedVideoOrPhotoId(MAUtil::integerToString(object_id));
	}
}

/*
 * function that parses the JSON data retrieved from server and fills an Place object with it.
 */
void parseJSONData(Place &place, MAUtil::YAJLDom::Value* result)
{
	MAUtil::String str;

	extract(place.mId, "id", result);
	extract(place.mName, "name", result);
	extract(place.mLocation, "location", result);
}

void parseJSONData(Place &place, MAUtil::YAJLDom::Value* result, const MAUtil::Set<MAUtil::String> &fields)
{
	MAUtil::String str;

	if(fields.find("id") != fields.end())
	{
		extract(place.mId, "id", result);
	}

	if(fields.find("name") != fields.end())
	{
		extract(place.mName, "name", result);
	}
	if(fields.find("location") != fields.end())
	{
		extract(place.mLocation, "location", result);
	}
}

/*
 * function that parses the JSON data retrieved from server and fills an Photo object with it.
 */
void parseJSONData(Photo &photo, MAUtil::YAJLDom::Value* result)
{
	MAUtil::String str;
	IdNamePair id_name;

	extract(str, "id", result);
	photo.setId(str);

	extract(id_name, "from", result);
	photo.setFrom(id_name);

	extract(str, "name", result);
	photo.setName(str);

	extract(str, "picture", result);
	photo.setSmallSizedPicture(str);

	extract(str, "source", result);
	photo.setFullSizedPicture(str);

	int iData=0;
	if(extract(iData, "height", result))
		photo.setHeight(iData);

	if(extract(iData, "width", result))
		photo.setWidth(iData);

	if(extract(str, "link", result))
		photo.setLink(str);

	if(extract(iData, "position", result))
		photo.setPositionInAlbum(iData);
}

void parseJSONData(Photo &photo, MAUtil::YAJLDom::Value* result, const MAUtil::Set<MAUtil::String> &fields)
{
	MAUtil::String str;
	IdNamePair id_name;

	if(fields.find("id") != fields.end() && extract(str, "id", result))
	{
		photo.setId(str);
	}

	if(fields.find("from") != fields.end() && extract(id_name, "from", result))
	{
		photo.setFrom(id_name);
	}

	if(fields.find("name") != fields.end() && extract(str, "name", result))
	{
		photo.setName(str);
	}

	if(fields.find("picture") != fields.end() && extract(str, "picture", result) )
	{
		photo.setSmallSizedPicture(str);
	}

	if(fields.find("source") != fields.end() && extract(str, "source", result))
	{
		photo.setFullSizedPicture(str);
	}

	int iData=0;
	if(fields.find("height") != fields.end() && extract(iData, "height", result))
	{
		photo.setHeight(iData);
	}

	if(fields.find("width") != fields.end() && extract(iData, "width", result))
	{
		photo.setWidth(iData);
	}

	if(fields.find("position") != fields.end() && extract(iData, "position", result))
	{
		photo.setPositionInAlbum(iData);
	}

	if(fields.find("link") != fields.end() && extract(str, "link", result))
	{
		photo.setLink(str);
	}
}

/*
 * function that parses the JSON data retrieved from server and fills an StatusMessage object with it.
 */
void parseJSONData(StatusMessage &msg, MAUtil::YAJLDom::Value* result)
{
	MAUtil::String str;

	extract(str, "id", result);
	msg.setId(str);

	IdNamePair id_name;
	extract(id_name, "from", result);
	msg.setFrom(id_name);

	extract(str, "message", result);
	msg.setMessage(str);
}

void parseJSONData(StatusMessage &msg, MAUtil::YAJLDom::Value* result, const MAUtil::Set<MAUtil::String> &fields)
{
	MAUtil::String str;

	if(fields.find("id") != fields.end() && extract(str, "id", result))
	{
		msg.setId(str);
	}

	if( fields.find("from") != fields.end() )
	{
		IdNamePair id_name;
		if(extract(id_name, "from", result))
		{
			msg.setFrom(id_name);
		}
	}

	if( fields.find("message") != fields.end() && extract(str, "message", result))
	{
		msg.setMessage(str);
	}
}

/*
 * function that parses the JSON data retrieved from server and fills an User object with it.
 */
void parseJSONData(User &user, MAUtil::YAJLDom::Value* result)
{
	MAUtil::YAJLDom::Value* data = NULL;

	data = result->getValueForKey("id");
	if(data)
	{
		user.setId(data->toString());
	}

	data = result->getValueForKey("name");
	if(data)
	{
		user.setName(data->toString());
	}

	data = result->getValueForKey("first_name");
	if(data)
	{
		user.setFirstName(data->toString());
	}

	data = result->getValueForKey("last_name");
	if(data)
	{
		user.setLastName(data->toString());
	}
	data = result->getValueForKey("link");
	if(data)
	{
		user.setLink(data->toString());
	}

	data = result->getValueForKey("username");
	if(data)
	{
		user.setUsername(data->toString());
	}

	data = result->getValueForKey("gender");
	if(data)
	{
		user.setGender(data->toString());
	}

	data = result->getValueForKey("locale");
	{
		user.setLocale(data->toString());
	}
}

void parseJSONData(User &user, MAUtil::YAJLDom::Value* result, const MAUtil::Set<MAUtil::String> &fields)
{
	MAUtil::String str;

	if(fields.find("id") != fields.end() && extract(str, "id", result))
	{
		user.setId(str);
	}

	if(fields.find("name") != fields.end() && extract(str, "name", result))
	{
		user.setName(str);
	}

	if(fields.find("first_name") != fields.end() && extract(str, "first_name", result))
	{
		user.setFirstName(str);
	}

	if(fields.find("last_name") != fields.end() && extract(str, "last_name", result))
	{
		user.setFirstName(str);
	}

	if(fields.find("link") != fields.end() && extract(str, "link", result))
	{
		user.setLink(str);
	}

	if(fields.find("username") != fields.end() && extract(str, "username", result))
	{
		user.setUsername(str);
	}

	if(fields.find("gender") != fields.end() && extract(str, "gender", result))
	{
		user.setGender(str);
	}

	if(fields.find("locale") != fields.end() && extract(str, "locale", result))
	{
		user.setLocale(str);
	}
}

/*
 * function that parses the JSON data retrieved from server and fills an Video object with it.
 */
void parseJSONData(Video &video, MAUtil::YAJLDom::Value* result)
{
	MAUtil::String str;
	IdNamePair id_name;

	extract(str, "id", result);
	video.setId(str);

	extract(id_name, "from", result);
	video.setFrom(id_name);

	extract(str, "name", result);
	video.setName(str);

//	extract(str, "description", result);
//	video.setDescription(str);

//	extract(str, "picture", result);
//	video.setPictureUrl(str);

//	extract(str, "source", result);
//	video.setSource(str);

//	MAUtil::Vector<Comment> comments;
//	extract(comments, "comments",result);
//	video.setComments(comments);

	MAUtil::YAJLDom::Value *tags = result->getValueForKey("tags");
	if(tags)
	{
		MAUtil::Vector<IdNamePair> tagsVector;
		extract(tagsVector, "data", tags);
		video.setTags(tagsVector);
	}
}

void parseJSONData(Video &video, MAUtil::YAJLDom::Value* result, const MAUtil::Set<MAUtil::String> &fields)
{
	MAUtil::String str;
	IdNamePair id_name;

	if(fields.find("id") != fields.end() && extract(str, "id", result))
	{
		video.setId(str);
	}

	if(fields.find("from") != fields.end() && extract(id_name, "from", result))
	{
		video.setFrom(id_name);
	}

	if(fields.find("name") != fields.end() && extract(str, "name", result))
	{
		video.setName(str);
	}

	if(fields.find("description") != fields.end() && extract(str, "description", result))
	{
		video.setDescription(str);
	}

	if(fields.find("picture") != fields.end() && extract(str, "picture", result))
	{
		video.setPictureUrl(str);
	}

	if(fields.find("source") != fields.end() && extract(str, "source", result))
	{
		video.setSource(str);
	}

	if(fields.find("comments") != fields.end())
	{
		MAUtil::Vector<Comment> comments;
		if(extract(comments, "comments",result))
		{
			video.setComments(comments);
		}
	}

	if(fields.find("tags") != fields.end())
	{
		MAUtil::YAJLDom::Value *tags = result->getValueForKey("tags");
		if(tags)
		{
			MAUtil::Vector<IdNamePair> tagsVector;
			if(extract(tagsVector, "data", tags))
			{
				video.setTags(tagsVector);
			}
		}
	}
}

/*
 * function that parses the JSON data retrieved from server and fills an CategoryData object with it.
 */
void parseJSONData(CategoryData &like, MAUtil::YAJLDom::Value* result)
{
	extract(like.mId, "id", result);
	extract(like.mName, "name", result);
	extract(like.mCategory, "category", result);
	extract(like.mDateCreated, "created_time", result);
}

void parseJSONData(CategoryData &like, MAUtil::YAJLDom::Value* result, const MAUtil::Set<MAUtil::String> &fields)
{
	if(fields.find("id") != fields.end())
	{
		extract(like.mId, "id", result);
	}

	if(fields.find("name") != fields.end())
	{
		extract(like.mName, "name", result);
	}

	if(fields.find("category") != fields.end())
	{
		extract(like.mCategory, "category", result);
	}

	if(fields.find("created_time") != fields.end())
	{
		extract(like.mDateCreated, "created_time", result);
	}
}

/*
 * function that parses the JSON data retrieved from server and fills an Note object with it.
 */
void parseJSONData(Note &note, MAUtil::YAJLDom::Value* result)
{
	MAUtil::String str;
	IdNamePair name_id;

	extract(str, "id", result);
	note.setId(str);

	extract(name_id, "from", result);
	note.setFrom(name_id);

	extract(str, "subject", result);
	note.setSubject(str);

	extract(str, "message", result);
	note.setMessage(str);

	MAUtil::Vector<Comment> comments;
	extract(comments, "comments",result);
	note.setComments(comments);
}

void parseJSONData(Note &note, MAUtil::YAJLDom::Value* result, const MAUtil::Set<MAUtil::String> &fields)
{
	MAUtil::String str;
	IdNamePair name_id;

	if(fields.find("id") != fields.end() && extract(str, "id", result))
	{
		note.setId(str);
	}

	if(fields.find("from") != fields.end() && extract(name_id, "from", result))
	{
		note.setFrom(name_id);
	}

	if(fields.find("subject") != fields.end() && extract(str, "subject", result))
	{
		note.setSubject(str);
	}

	if(fields.find("message") != fields.end() && extract(str, "message", result))
	{
		note.setMessage(str);
	}

	if(fields.find("comments") != fields.end())
	{
		MAUtil::Vector<Comment> comments;
		if(extract(comments, "comments",result))
		{
			note.setComments(comments);
		}
	}
}

/*
 * function that parses the JSON data retrieved from server and fills an Action object with it.
 */
void parseJSONData(Action &action, MAUtil::YAJLDom::Value* data)
{
	action.mLink.clear();
	action.mName.clear();

	if(data)
	{
		MAUtil::YAJLDom::Value *temp = data->getValueForKey("name");
		if(temp)
		{
			action.mName = temp->toString();
		}

		temp = data->getValueForKey("link");
		if(temp)
		{
			action.mLink = temp->toString();
		}
	}
}

void parseJSONData(Action &action, MAUtil::YAJLDom::Value* data,  const MAUtil::Set<MAUtil::String> &fields)
{
	action.mLink.clear();
	action.mName.clear();

	if(fields.find("name")!= fields.end())
	{
		extract(action.mName, "name", data);
	}

	if(fields.find("link") != fields.end())
	{
		extract(action.mLink, "link", data);
	}

//	if(data)
//	{
//		MAUtil::YAJLDom::Value *temp = data->getValueForKey("name");
//		if(temp)
//		{
//			action.mName = temp->toString();
//		}
//
//		temp = data->getValueForKey("link");
//		if(temp)
//		{
//			action.mLink = temp->toString();
//		}
//	}
}

/*
 * function that parses the JSON data retrieved from server and fills an IdNamePair object with it.
 */
void parseJSONData(IdNamePair &val, MAUtil::YAJLDom::Value* data)
{
	val.mId.clear();
	val.mName.clear();

	if(data)
	{
		MAUtil::YAJLDom::Value *temp = data->getValueForKey("name");
		if(temp)
		{
			val.mName = temp->toString();
		}

		temp = data->getValueForKey("id");
		if(temp)
		{
			val.mId = temp->toString();
		}
	}
}

void parseJSONData(IdNamePair &val, MAUtil::YAJLDom::Value* data, const MAUtil::Set<MAUtil::String> &fields)
{
	val.mId.clear();
	val.mName.clear();

	MAUtil::String str;
	if(fields.find("name")!= fields.end())
	{
		extract(val.mName, "name", data);
	}
	if(fields.find("id") != fields.end())
	{
		extract(val.mId, "name", data);
	}

//	if(data)
//	{
//		MAUtil::YAJLDom::Value *temp = data->getValueForKey("name");
//		if(temp)
//		{
//			val.mName = temp->toString();
//		}
//
//		temp = data->getValueForKey("id");
//		if(temp)
//		{
//			val.mId = temp->toString();
//		}
//	}
}

/*
 * function that parses the JSON data retrieved from server and fills an EventResponse object with it.
 */
void parseJSONData(EventResponse &event, MAUtil::YAJLDom::Value* result)
{
	event.mId.clear();
	event.mName.clear();
	event.mRsvp_Status.clear();

	if(NULL == result)
	{
		return;
	}
	MAUtil::YAJLDom::Value *temp = result->getValueForKey("id");
	if(temp)
	{
		event.mId = temp->toString();
	}
	temp = result->getValueForKey("name");
	if(temp)
	{
		event.mName = temp->toString();
	}
	temp = result->getValueForKey("rsvp_status");
	if(temp)
	{
		event.mRsvp_Status = temp->toString();
	}
}

void parseJSONData(EventResponse &event, MAUtil::YAJLDom::Value* result, const MAUtil::Set<MAUtil::String> &fields)
{
	event.mId.clear();
	event.mName.clear();
	event.mRsvp_Status.clear();

	if(NULL == result)
	{
		return;
	}

	if(fields.find("id") != fields.end())
	{
		extract(event.mId, "id", result);
	}

	if(fields.find("name") != fields.end())
	{
		extract(event.mName, "name", result);
	}

	if(fields.find("rsvp_status") != fields.end())
	{
		extract(event.mRsvp_Status, "rsvp_status", result);
	}

//	MAUtil::YAJLDom::Value *temp = result->getValueForKey("id");
//	if(temp)
//	{
//		event.mId = temp->toString();
//	}
//	temp = result->getValueForKey("name");
//	if(temp)
//	{
//		event.mName = temp->toString();
//	}
//	temp = result->getValueForKey("rsvp_status");
//	if(temp)
//	{
//		event.mRsvp_Status = temp->toString();
//	}
}

/*
 * function that parses JSON data and fills a string with it
 * @param str - the resulting string
 * @param tag - the name of a JSON node.
 * @param result - the JSON data.
 */
bool extract(MAUtil::String &str, const char *tag, MAUtil::YAJLDom::Value* result)
{
	MAUtil::YAJLDom::Value* data = NULL;
	str.clear();

	data = result->getValueForKey(tag);
	if(data)
	{
		str = data->toString();
	}

	return data!=NULL;
}

/*
 * function that parses JSON data and fills a IdNamePair object with it
 * @param val - the IdNamePair
 * @param tag - the name of a JSON node.
 * @param result - the JSON data.
 */
bool extract(int &val, const char *tag, MAUtil::YAJLDom::Value* result)
{
	val = 0;
	MAUtil::YAJLDom::Value* data = NULL;
	data = result->getValueForKey(tag);

	if(data)
	{
		val = data->toInt();
	}
	return data!=NULL;
}

/*
 * function converts a JSON value to an int.
 * @param val - the resulting int value
 * @param tag - the name of a JSON node.
 * @param result - the JSON data.
 */
bool extract(IdNamePair &val, const char *tag, MAUtil::YAJLDom::Value* result)
{
	val.mId.clear();
	val.mName.clear();

	MAUtil::YAJLDom::Value* data = NULL;
	data = result->getValueForKey(tag);

	if(NULL == data)
	{
		return false;
	}

	MAUtil::YAJLDom::Value *temp = data->getValueForKey("id");
	if(temp)
	{
		val.mId = temp->toString();
	}

	temp = data->getValueForKey("name");
	if(temp)
	{
		val.mName = temp->toString();
	}
	return true;
}

/*
 * function that parses JSON data and fills a Venue object with it.
 * @param val - the Venue object to be filled.
 * @param tag - the name of a JSON node.
 * @param result - the JSON data.
 */
bool extract(Venue &venue, const char *tag, MAUtil::YAJLDom::Value* result)
{
	venue.clear();

	MAUtil::YAJLDom::Value* data = NULL;
	data = result->getValueForKey(tag);

	if(data)
	{
		MAUtil::YAJLDom::Value *temp = NULL;

//		temp = data->getValueForKey("street");
//		if(temp)
//		{
//			venue.mStreet = temp->toString();
//		}

		temp = data->getValueForKey("city");
		if(temp)
		{
			venue.mCity = temp->toString();
		}

//		temp = data->getValueForKey("state");
//		if(temp)
//		{
//			venue.mState = temp->toString();
//		}

		temp = data->getValueForKey("country");
		if(temp)
		{
			venue.mCountry = temp->toString();
		}

//		temp = data->getValueForKey("latitude");
//		if(temp)
//		{
//			venue.mCoordinate.mLatitude = temp->toString();
//		}
//
//		temp = data->getValueForKey("longitude");
//		if(temp)
//		{
//			venue.mCoordinate.mLongitude = temp->toString();
//		}
	}
	return data!=NULL;
}