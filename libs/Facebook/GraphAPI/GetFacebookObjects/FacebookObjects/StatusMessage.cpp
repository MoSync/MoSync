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

/*
 * StatusMessage.cpp
 */


#include "StatusMessage.h"

/**
 * Setter/getter for the "from" field of an StatusMessage object.
 * @param from - it represents the person/application that created the status message.
 * It contains an id and name.
 */
void StatusMessage::setFrom(const IdNamePair &from)
{
	mFrom = from;
}
const IdNamePair &StatusMessage::getFrom() const
{
	return mFrom;
}

/**
 * Setter/getter for the "message" field of an StatusMessage object.
 * @param message - the status message content.
 */
void StatusMessage::setMessage(const MAUtil::String &message)
{
	mMessage = message;
}
const MAUtil::String &StatusMessage::getMessage() const
{
	return mMessage;
}
