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
 * LOG.cpp
 *
 *  Created on: Jul 5, 2011
 *      Author: gabi
 */


#include "LOG.h"

void display(const Venue &venue, const MAUtil::String &prefix)
{
	MAUtil::String str = prefix + "display a Venue object:\n";
	LOG(str.c_str());

	str = prefix + "\t" + "country: %s";
	LOG(str.c_str(), venue.mCountry.c_str());

	str = prefix + "\t" + "state: %s";
	LOG(str.c_str(), venue.mState.c_str());

	str = prefix + "\t" + "city: %s";
	LOG(str.c_str(), venue.mCity.c_str());

	str = prefix + "\t" + "street: %s";
	LOG(str.c_str(), venue.mStreet.c_str());

	str = prefix + "\t" + "zip: %s";
	LOG(str.c_str(), venue.mZip.c_str());

	str = prefix + "\t" + "latitude: %s";
	LOG(str.c_str(), venue.mCoordinate.mLatitude.c_str());

	str = prefix + "\t" + "longitude: %s";
	LOG(str.c_str(), venue.mCoordinate.mLongitude.c_str());

	LOG("\n");
}

void display(const Place &place, const MAUtil::String &prefix)
{
	MAUtil::String str = prefix + "display a Place object:\n";
	LOG(str.c_str());

	str = prefix + "\t" + "id: %s";
	LOG(str.c_str(), place.mId.c_str());

	str = prefix + "\t" + "name: %s";
	LOG(str.c_str(), place.mName.c_str());

	str = prefix + "\t" + "location:\n";
	LOG(str.c_str());

	display(place.mLocation, prefix + "\t\t");
}

void display(const Action &action, const MAUtil::String &prefix)
{
	MAUtil::String str = prefix + "display a Action object:\n";
	LOG(str.c_str());
	str = prefix +"\t" + "name: %s";
	LOG(str.c_str(), action.mName.c_str());
	str = prefix + "\t" + "link: %s";
	LOG(str.c_str(), action.mLink.c_str());

	LOG("\n");
}

void display(const EventResponse &response, const MAUtil::String &prefix)
{
	MAUtil::String str = prefix + "display a EventResponse object:\n";
	LOG(str.c_str());

	str = prefix +"\t" + "id: %s";
	LOG(str.c_str(), response.mId.c_str());

	str = prefix + "\t" + "name: %s";
	LOG(str.c_str(), response.mName.c_str());

	str = prefix +"\t" + "rsvp_status: %s";
	LOG(str.c_str(), response.mRsvp_Status.c_str());
}

void display(const Checkin &checkin, const MAUtil::String &prefix)
{
	MAUtil::String str = prefix + "display a Checkin object:\n";
	LOG(str.c_str());

	str = prefix +"\t" + "id: %s";
	LOG(str.c_str(), checkin.getId().c_str());

	str =  prefix +"\t" + "from:\n";
	LOG(str.c_str());
	display(checkin.getFrom(), prefix +"\t\t");

	str = prefix +"\t" + "tags:\n";
	LOG(str.c_str());
	const MAUtil::Vector<IdNamePair> &tags = checkin.getTags();
	for(int i=0; i<tags.size(); ++i)
	{
		display(tags[i],  prefix +"\t\t");
	}

	str =  prefix +"\t" + "place:\n";
	LOG(str.c_str());
	display(checkin.getPlace(), prefix +"\t\t");

	str =  prefix +"\t" + "application:\n";
	LOG(str.c_str());
	display(checkin.getApplication(), prefix +"\t\t");

	str = prefix +"\t" + "likes:\n";
	LOG(str.c_str());
	const MAUtil::Vector<IdNamePair> &likes = checkin.getLikes();
	for(int i=0; i<likes.size(); ++i)
	{
		display(likes[i],  prefix +"\t\t");
	}

	str = prefix +"\t" + "comments:\n";
	LOG(str.c_str());
	const MAUtil::Vector<Comment> &comments = checkin.getComments();
	for(int i=0; i<comments.size(); ++i)
	{
		display(comments[i],  prefix +"\t\t");
	}

}

void display(const CategoryData &like, const MAUtil::String &prefix )
{
	MAUtil::String str = prefix + "display a Like/Movie/Music/Book/FriendList ect object:\n";
	LOG(str.c_str());

	str = prefix +"\t" + "id: %s";
	LOG(str.c_str(), like.mId.c_str());

	str = prefix +"\t" + "category: %s";
	LOG(str.c_str(), like.mCategory.c_str());

	str = prefix +"\t" + "name: %s";
	LOG(str.c_str(), like.mName.c_str());

	str = prefix +"\t" + "date created: %s";
	LOG(str.c_str(), like.mDateCreated.c_str());

	LOG("\n");
}

void display(const Comment &comment, const MAUtil::String &prefix)
{
	MAUtil::String str = prefix + "display a Comment object:\n";
	LOG(str.c_str());

	str = prefix +"\t" + "from: %s, %s";
	LOG(str.c_str(), comment.getFromField().mName.c_str(), comment.getFromField().mId.c_str());

	str = prefix +"\t" + "message: %s";
	LOG(str.c_str(), comment.getMessage().c_str());

	str = prefix +"\t" + "id: %s";
	LOG(str.c_str(), comment.getId().c_str());

	str = prefix +"\t" + "likes: %s";
	LOG(str.c_str(), comment.getLikes());

	LOG("\n");
}

void display(const Post &post, const MAUtil::String &prefix)
{
	MAUtil::String str = prefix + "display a Post object:\n";
	LOG(str.c_str());

	str = prefix +"\t" + "message: %s";
	LOG(str.c_str(), post.getMessage().c_str());

	str = prefix +"\t" + "name: %s";
	LOG(str.c_str(), post.getName().c_str());

	str = prefix +"\t" + "id: %s";
	LOG(str.c_str(), post.getId().c_str());

	str = prefix + "\t" + "object_id(video or photo uploaded): %s";
	LOG(str.c_str(), post.getUploadedVideoOrPhotoId().c_str());

	str = prefix +"\t" + "caption: %s";
	LOG(str.c_str(), post.getCaption().c_str());

	str = prefix +"\t" + "description: %s";
	LOG(str.c_str(), post.getDescription().c_str());

	str = prefix +"\t" + "from: %s, %s";
	LOG(str.c_str(), post.getFromField().mName.c_str(), post.getFromField().mId.c_str());

	str = prefix +"\t" + "link: %s";
	LOG(str.c_str(), post.getLink().c_str());

	const MAUtil::Vector<IdNamePair> &likes = post.getLikes();
	prefix +"\t" + "likes:";
	LOG(str.c_str());
	for( int i=0; i<likes.size(); i++)
	{
		display(likes[i], prefix + "\t\t");
	}

	const MAUtil::Vector<Action> &actions = post.getActions();
	str = prefix + "\tactions:\n";
	LOG(str.c_str());
	for( int i=0; i<actions.size(); i++)
	{
		display(actions[i], prefix + "\t\t");
	}

	const MAUtil::Vector<Comment> &comments = post.getComments();
	str = prefix + "\tcomments:\n";
	LOG(str.c_str());
	for( int i=0; i<comments.size(); i++)
	{
		display(comments[i], prefix + "\t\t");
	}

	LOG("\n\n");
}


void display(const User &user, const MAUtil::String &prefix)
{
	MAUtil::String str = prefix + "display a User object:\n";
	LOG(str.c_str());

	str = prefix +"\t" + "first name: %s";
	LOG(str.c_str(), user.getFirstName().c_str());

	str = prefix +"\t" + "last name: %s";
	LOG(str.c_str(), user.getLastName().c_str());

	str = prefix +"\t" + "full name: %s";
	LOG(str.c_str(), user.getName().c_str());

	str = prefix +"\t" + "user: %s";
	LOG(str.c_str(), user.getUsername().c_str());

	str = prefix +"\t" + "id: %s";
	LOG(str.c_str(), user.getId().c_str());

	str = prefix +"\t" + "gender: %s";
	LOG(str.c_str(), user.getGender().c_str());

	str = prefix +"\t" + "link: %s";
	LOG(str.c_str(), user.getLink().c_str());

	str = prefix +"\t" + "locale %s";
	LOG(str.c_str(), user.getLocale().c_str());

	LOG("\n");
}

void display(const Photo &photo, const MAUtil::String &prefix)
{
	MAUtil::String str = prefix + "display a Photo object:\n";
	LOG(str.c_str());

	str = prefix +"\t" + "id: %s";
	LOG(str.c_str(), photo.getId().c_str());

	str = prefix +"\t" + "from: %s, %s";
	LOG(str.c_str(), photo.getFrom().mName.c_str(), photo.getFrom().mId.c_str());

	str = prefix +"\t" + "thumbnail sized picture url: %s";
	LOG(str.c_str(), photo.getSmallSizedPicture().c_str());

	str = prefix +"\t" + "full sized picture url: %s";
	LOG(str.c_str(), photo.getFullSizedPicture().c_str());

	str = prefix +"\t" + "link: %s";
	LOG(str.c_str(), photo.getLink().c_str());

	str = prefix +"\t" + "position in album: %d";
	LOG(str.c_str(), photo.getPositionInAlbum());

	str = prefix +"\t" + "width: %d";
	LOG(str.c_str(), photo.getWidth());

	str = prefix +"\t" + "height: %d";
	LOG(str.c_str(), photo.getHeight());

	LOG("\n");
}

void display(const Video &video, const MAUtil::String &prefix)
{
	MAUtil::String str = prefix + "display a Video object:\n";
	LOG(str.c_str());

	str = prefix +"\t" + "id: %s";
	LOG(str.c_str(), video.getId().c_str());

	str = prefix +"\t" + "from: %s, %s";
	LOG(str.c_str(), video.getFrom().mName.c_str(), video.getFrom().mId.c_str());

	str = prefix +"\t" + "description: %s";
	LOG(str.c_str(), video.getDescription().c_str());

	str = prefix +"\t" + "name: %s";
	LOG(str.c_str(), video.getName().c_str());

	str = prefix +"\t" + "pictureUrl: %s";
	LOG(str.c_str(), video.getPictureUrl().c_str());

	str = prefix +"\t" + "source(link to download the video): %s";
	LOG(str.c_str(), video.getSource().c_str());

	const MAUtil::Vector<Comment> &comments = video.getComments();
	str = prefix +"\t" + "comments: %s";
	LOG(str.c_str());
	for(int i=0; i<comments.size(); i++)
	{
		display(comments[i],  prefix + "\t\t");
	}

	const MAUtil::Vector<IdNamePair> &tags = video.getTags();
	str = prefix +"\t" + "tags: %s";
	LOG(str.c_str());
	for(int i=0; i<tags.size(); i++)
	{
		display(tags[i], prefix + "\t\t");
	}

	LOG("\n");
}

void display(const Link &link, const MAUtil::String &prefix)
{
	MAUtil::String str = prefix + "display a Link object:\n";
	LOG(str.c_str());

	str = prefix +"\t" + "id: %s";
	LOG(str.c_str(), link.getId().c_str());

	str = prefix +"\t" + "from: %s, %s";
	LOG(str.c_str(), link.getFrom().mName.c_str(), link.getFrom().mId.c_str());

	str = prefix +"\t" + "name: %s";
	LOG(str.c_str(), link.getName().c_str());

	str = prefix +"\t" + "description: %s";
	LOG(str.c_str(), link.getDescription().c_str());

	str = prefix +"\t" + "message: %s";
	LOG(str.c_str(), link.getDescription().c_str());

	str = prefix +"\t" + "link: %s";
	LOG(str.c_str(), link.getLink().c_str());

	str = prefix +"\t" + "pictureUrl: %s";
	LOG(str.c_str(), link.getPicture().c_str());

	LOG("\n");
}

void display(const Group &group, const MAUtil::String &prefix)
{
	MAUtil::String str = prefix + "display a Group object:\n";
	LOG(str.c_str());

	str = prefix +"\t" + "id: %s";
	LOG(str.c_str(), group.getId().c_str());

	str = prefix +"\t" + "owner: %s";
	LOG(str.c_str(), group.getOwner().mName.c_str(), group.getOwner().mId.c_str());

	str = prefix +"\t" + "name: %s";
	LOG(str.c_str(), group.getName().c_str());

	str = prefix +"\t" + "description: %s";
	LOG(str.c_str(), group.getDescription().c_str());

	str = prefix +"\t" + "link: %s";
	LOG(str.c_str(), group.getLink().c_str());

	str = prefix +"\t" + "privacy: %s";
	LOG(str.c_str(), group.getPrivacy().c_str());

	LOG("\n");
}

void display(const Album &album, const MAUtil::String &prefix)
{
	MAUtil::String str = prefix + "display a Album object:\n";
	LOG(str.c_str());

	str = prefix +"\t" + "id: %s";
	LOG(str.c_str(), album.getId().c_str());

	str = prefix +"\t" + "from: %s, %s";
	LOG(str.c_str(), album.getFromField().mName.c_str(), album.getFromField().mId.c_str());

	str = prefix +"\t" + "cover photo id: %s";
	LOG(str.c_str(), album.getCoverPhotoId().c_str());

	str = prefix +"\t" + "description: %s";
	LOG(str.c_str(), album.getDescription().c_str());

	str = prefix +"\t" + "link: %s";
	LOG(str.c_str(), album.getLink().c_str());

	str = prefix +"\t" + "name: %s";
	LOG(str.c_str(), album.getName().c_str());

	str = prefix +"\t" + "count: %d";
	LOG(str.c_str(), album.getCount());

	LOG("\n");
}

void display(const Event &event, const MAUtil::String &prefix)
{
	MAUtil::String str = prefix + "display a Event object:\n";
	LOG(str.c_str());

	str = prefix +"\t" + "id: %s";
	LOG(str.c_str(), event.getId().c_str());

	str = prefix +"\t" + "name: %s";
	LOG(str.c_str(), event.getName().c_str());

	str = prefix +"\t" + "owner: %s";
	LOG(str.c_str(), event.getOwner().mName.c_str(), event.getOwner().mId.c_str());

	str = prefix +"\t" + "description: %s";
	LOG(str.c_str(), event.getDescription().c_str());

	str = prefix +"\t" + "location: %s";
	LOG(str.c_str(), event.getLocation().c_str());

	str = prefix +"\t" + "start time: %s";
	LOG(str.c_str(), event.getStartTime().c_str());

	str = prefix +"\t" + "end time: %s";
	LOG(str.c_str(), event.getEndTime().c_str());

	str = prefix + "\t" + "venue:\n";
	display(event.getVenue(), prefix + "\t\t");

	str = prefix +"\t" + "privacy: %s";
	LOG(str.c_str(), event.getPrivacy().c_str());

	str = prefix +"\t" + "description: %s";
	LOG(str.c_str(), event.getDescription().c_str());

	LOG("\n");
}

void display(const IdNamePair &id_name, const MAUtil::String &prefix)
{
	MAUtil::String str;

	str = prefix +"\t" + "id: %s";
	LOG(str.c_str(), id_name.mId.c_str());

	str = prefix +"\t" + "name: %s";
	LOG(str.c_str(), id_name.mName.c_str());

	LOG("\n");
}

void display(const Note &note, const MAUtil::String &prefix )
{
	MAUtil::String str = prefix + "display a Note object:\n";
	LOG(str.c_str());

	str = prefix +"\t" + "id: %s";
	LOG(str.c_str(), note.getId().c_str());

	str = prefix +"\t" + "from: %s, %s";
	LOG(str.c_str(), note.getFrom().mId.c_str(), note.getFrom().mName.c_str());

	str = prefix +"\t" + "message: %s";
	LOG(str.c_str(), note.getMessage().c_str());

	str = prefix +"\t" + "subject: %s";
	LOG(str.c_str(), note.getSubject().c_str());

	const MAUtil::Vector<Comment> &comments = note.getComments();
	str = prefix +"\t" + "comments:\n";
	LOG(str.c_str());

	for(int i=0; i<comments.size(); i++)
	{
		display(comments[i]);
	}

	LOG("\n");
}

void display(const StatusMessage &msg, const MAUtil::String &prefix)
{
	MAUtil::String str = prefix + "display a SatusMessage object:\n";
	LOG(str.c_str());

	str = prefix +"\t" + "id: %s";
	LOG(str.c_str(), msg.getId().c_str());

	str = prefix +"\t" + "from:\n";
	display(msg.getFrom(), prefix +"\t\t");

	str = prefix +"\t" + "message: %s";
	LOG(str.c_str(), msg.getMessage().c_str());

	LOG("\n");
}
