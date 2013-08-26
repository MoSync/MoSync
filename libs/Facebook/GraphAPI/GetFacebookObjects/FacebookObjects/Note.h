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
 * @file Note.h
 * @author Gabriela Rata
 */

#ifndef NOTE_H_
#define NOTE_H_

#include <MAUtil/Vector.h>
#include "FacebookObject.h"
#include "Comment.h"
#include "Utilities.h"

/**
 * \brief Implements an Note.
 * See: http://developers.facebook.com/docs/reference/api/note/
 */
class Note: public FacebookObject
{
public:

	/**
	 * Setter/getter for the "from" field of an Note object.
	 * @param from - it represents the user/application that created the note.
	 * It contains an id and name.
	 */
	void setFrom(const IdNamePair & from);
	const IdNamePair &getFrom() const;

	/**
	 * Setter/getter for the "subject" field of an Note object.
	 * @param subject - the title of the note
	 */
	void setSubject(const MAUtil::String &subject);
	const MAUtil::String &getSubject() const;

	/**
	 * Setter/getter for the "message" field of an Note object.
	 * @param message - the content of the note.
	 */
	void setMessage(const MAUtil::String &message);
	const MAUtil::String &getMessage() const;

	/**
	 * Setter/getter for the "comments" field of an Note object.
	 * @param comments - comments made on the note.
	 */
	void setComments(const MAUtil::Vector<Comment> &comments);
	const MAUtil::Vector<Comment> &getComments() const;

private:
	MAUtil::String			mId;
	IdNamePair				mFrom;
	MAUtil::String			mSubject;
	MAUtil::String			mMessage;
	MAUtil::Vector<Comment> mComments;
};


#endif /* NOTE_H_ */
