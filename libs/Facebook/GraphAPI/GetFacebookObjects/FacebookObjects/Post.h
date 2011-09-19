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
 * @file Post.h
 * @author Gabriela Rata
 */

#ifndef POST_H_
#define POST_H_

#include "FacebookObject.h"
#include "Utilities.h"
#include "Comment.h"

/**
 * \brief Implements a Post Facebook object type. It represents an individual
 * entry in a profile's feed.
 * See: http://developers.facebook.com/docs/reference/api/post/
 */
class Post: public FacebookObject
{
public:
	/**
	 * Setter/getter for the "from" field of an Post object.
	 * @param from - it represents the person/application that created the post. It contains an id and name.
	 */
	void setFromfield(const IdNamePair &from);
	const IdNamePair &getFromField() const;

	/**
	 * Setter/getter for the "to" field of an Post object.
	 * @param to - profiles mentioned or targeted in this post. It contains an id and name.
	 */
	void setToField(const IdNamePair &to);
	const IdNamePair &getToField() const;

	/**
	 * Setter/getter for the "message" field of an Post object.
	 * @param message - the message of the post
	 */
	void setMessage(const MAUtil::String &message);
	const MAUtil::String &getMessage() const;

	/**
	 * Setter/getter for the "picture" field of an Post object.
	 * @param picture - a link to the picture included with this post
	 */
	void setPictureUrl(const MAUtil::String &url);
	const MAUtil::String &getPictureUrl() const;

	/**
	 * Setter/getter for the "link" field of an Post object.
	 * @param link - the link attached to this post
	 */
	void setLink(const MAUtil::String &link);
	const MAUtil::String &getLink() const;

	/**
	 * Setter/getter for the "name" field of an Post object.
	 * @param name - the name of the link
	 */
	void setName(const MAUtil::String &name);
	const MAUtil::String &getName() const;

	/**
	 * Setter/getter for the "caption" field of an Post object.
	 * @param caption - a description of the link (appears beneath the link caption)
	 */
	void setCaption(const MAUtil::String &caption);
	const MAUtil::String &getCaption() const;

	/**
	 * Setter/getter for the "description" field of an Post object.
	 * @param desc - the description of the album
	 */
	void setDescription(const MAUtil::String &desc);
	const MAUtil::String &getDescription() const;

	/**
	 * Setter/getter for the "source" field of an Post object.
	 * @param url - a URL to a Flash movie or video file to be embedded within the post
	 */
	void setVideoUrl(const MAUtil::String &url);
	const MAUtil::String &getVideoUrl() const;

	/**
	 * Setter/getter for the "object_id" field of an Post object.
	 * @param id - the Facebook object id for an uploaded photo or video
	 */
	void setUploadedVideoOrPhotoId(const MAUtil::String &id);
	const MAUtil::String &getUploadedVideoOrPhotoId() const;

	/**
	 * Setter/getter for the "actions" field of an Post object.
	 * @param actions - a Vector of available actions on the post
	 * (including commenting, liking, and an optional app-specified action)
	 */
	void setActions(const MAUtil::Vector<Action> &actions);
	const MAUtil::Vector<Action> &getActions() const;

	/**
	 * Setter/getter for the "type" field of an Post object.
	 * @param type - a string indicating the type for this post (including link, photo, video)
	 */
	void  setType(const MAUtil::String &type);
	const MAUtil::String &getType() const;

	/**
	 * Setter/getter for the "likes" field of an Post object.
	 * @param likes - likes for this post
	 */
	void setLikes(const MAUtil::Vector<IdNamePair> &likes);
	const MAUtil::Vector<IdNamePair> &getLikes() const;

	/**
	 * Setter/getter for the "comments" field of an Post object.
	 * @param comments - comments for this post
	 */
	void setComments(MAUtil::Vector<Comment> &comments);
	const MAUtil::Vector<Comment> &getComments() const;

	/**
	 * Setter/getter for the "application" field of an Post object.
	 * @param applicationInfo - information about the application this post came from
	 */
	void setApplication(const IdNamePair &applicationInfo);
	const IdNamePair &getApplication() const;

private:
	IdNamePair				mFrom;
	IdNamePair				mTo;
	MAUtil::String			mMessage;
	MAUtil::String			mPictureUrl;
	MAUtil::String			mLink;
	MAUtil::String			mName;
	MAUtil::String			mCaption;
	MAUtil::String			mDescription;
	MAUtil::String			mVideoUrl;
	MAUtil::Vector<Action>	mActions;
	MAUtil::String			mPrivacy;
	MAUtil::String			mType;
	MAUtil::Vector<IdNamePair>	mLikes;
	MAUtil::Vector<Comment> mComments;
	MAUtil::String			mUploadedFileId;
	IdNamePair				mApplication;
};

#endif /* POST_H_ */
