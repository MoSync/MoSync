/*
 * GetPermissions.h
 *
 *  Created on: Jul 22, 2011
 *      Author: gabi
 */

#ifndef GETPERMISSIONS_H_
#define GETPERMISSIONS_H_


#include <MAUtil/Set.h>

class Album;
class Checkin;
class Comment;
class Event;
class FriendList;
class Group;
class Link;
class Note;
class Page;
class Photo;
class Post;
class StatusMessage;
class User;
class Video;

class GetPermissions
{
public:
	static void forAllPosibleRequests(MAUtil::Set<MAUtil::String> &permissions)
	{
		//publish. Needed for all object types
		permissions.insert("publish_stream");

		//retrieve data from Facebook
		permissions.insert("read_stream");

		//Album and Photo
		permissions.insert("user_photos");
		permissions.insert("friends_photos");
		permissions.insert("photo_upload");

		//Checkins
		permissions.insert("user_checkins");
		permissions.insert("friends_checkins");

		//Event
		permissions.insert("user_events");
		permissions.insert("friends_events");
		permissions.insert("rsvp_event");

		//FriendList
		permissions.insert("read_friendlists");
		permissions.insert("manage_friendlists");

		//Group
		permissions.insert("user_groups");
		permissions.insert("friends_groups");

		//Note
		permissions.insert("create_note");
		permissions.insert("user_notes");
		permissions.insert("friends_notes");

		//Photo
		permissions.insert("user_photo_video_tags");
		permissions.insert("friends_photo_video_tags");

		//User
		permissions.insert("publish_checkins");
		permissions.insert("create_event");
		permissions.insert("publish_actions");
		permissions.insert("share_item");

			//retrieve User data:
			//1. User fields
		permissions.insert("user_likes");
		permissions.insert("friends_likes");

		permissions.insert("user_about_me");
		permissions.insert("friends_about_me");

		permissions.insert("user_birthday");
		permissions.insert("friends_birthday");

		permissions.insert("user_education_history");
		permissions.insert("friends_education_history");

		permissions.insert("user_hometown");
		permissions.insert("friends_hometown");

		permissions.insert("user_relationship_details");
		permissions.insert("friends_relationship_details");

		permissions.insert("user_relationships");
		permissions.insert("friends_relationships");

		permissions.insert("user_religion_politics");
		permissions.insert("friends_religion_politics");

		permissions.insert("user_website");
		permissions.insert("friends_website");

		permissions.insert("user_work_history");
		permissions.insert("friends_work_history");

			//2. User connections
		permissions.insert("user_activities");
		permissions.insert("friends_activities");

		permissions.insert("user_interests");
		permissions.insert("friends_interests");

		//Video
		permissions.insert("user_videos");
		permissions.insert("video_upload");
	}
};

/*
 * template class used retrieving the extra permissions (the permissions needed besides the access_token )
 * Those permissions are needed for certain connections or to publish on Facebook.
 * GetPermissionsFor template is specialized for every type of Facebook object.
 */
template<class T>
class GetPermissionsFor;

/*
 * template class for retrieving the permissions needed for a connection or to or to make a publish request on a Album object.
 */
template<>
class GetPermissionsFor<Album>
{
public:
	/*
	 * Adds to the "permissions" vector all permissions needed to make any type of request on
	 * an Album object.
	 */
	static void allPosibleRequests(MAUtil::Set<MAUtil::String> &permissions)
	{
		permissions.insert("user_photos");
		permissions.insert("photo_upload");
		permissions.insert("friends_photos");
		permissions.insert("publish_stream");
	}

	/*
	 * Adds to the "permissions" set all permissions needed to ask for connections.
	 */
	static void retrievingData(MAUtil::Set<MAUtil::String> &permissions)
	{
		permissions.insert("user_photos");
		permissions.insert("friends_photos");
	}

	/*
	 * Adds to the "permissions" set all permissions needed publish on a Album object.
	 */
	static void publishingData(MAUtil::Set<MAUtil::String> &permissions)
	{
		permissions.insert("publish_stream");
		permissions.insert("photo_upload");
	}
};

/*
 * template class for retrieving the permissions needed for a connection or to make a publish request on a Checkin object.
 */
template<>
class GetPermissionsFor<Checkin>
{
public:
	/*
	 * Adds to the "permissions" set all permissions needed to make any type of request on
	 * a Checkin object.
	 */
	static void allPosibleRequests(MAUtil::Set<MAUtil::String> &permissions)
	{
		permissions.insert("user_checkins");
		permissions.insert("friends_checkins");
		permissions.insert("publish_stream");
	}

	/*
	 * Adds to the "permissions" set all permissions needed to ask for connections.
	 */
	static void retrievingData(MAUtil::Set<MAUtil::String> &permissions)
	{
		permissions.insert("user_checkins");
		permissions.insert("friends_checkins");
	}

	/*
	 * Adds to the "permissions" set all permissions needed publish on a Checkin object.
	 */
	static void publishingData(MAUtil::Set<MAUtil::String> &permissions)
	{
		permissions.insert("publish_stream");
	}
};

/*
 * template class for retrieving the permissions needed for a connection or to make a publish request on a Comment object.
 */
template<>
class GetPermissionsFor<Comment>
{
public:
	/*
	 * Adds to the "permissions" set all permissions needed to make any type of request on
	 * a Comment object.
	 */
	static void allPosibleRequests(MAUtil::Set<MAUtil::String> &permissions)
	{
		permissions.insert("publish_stream");
	}

	/*
	 * No extra permissions needed to retrieve a connection from a Comment object.
	 */
	static void retrievingData(MAUtil::Set<MAUtil::String> &permissions)
	{

	}

	/*
	 * Adds to the "permissions" set all permissions needed publish on a Comment object.
	 */
	static void publishingData(MAUtil::Set<MAUtil::String> &permissions)
	{
		permissions.insert("publish_stream");
	}

};

/*
 * template class for retrieving the permissions needed for a connection or to make a publish request on a Event object.
 */
template<>
class GetPermissionsFor<Event>
{
public:
	/*
	 * Adds to the "permissions" set all permissions needed to make any type of request on
	 * a Event object.
	 */
	static void allPosibleRequests(MAUtil::Set<MAUtil::String> &permissions)
	{
		permissions.insert("user_events");
		permissions.insert("friends_events");
		permissions.insert("publish_stream");
		permissions.insert("rsvp_event");
	}

	/*
	 * Adds to the "permissions" set all permissions needed to ask for connections.
	 */
	static void retrievingData(MAUtil::Set<MAUtil::String> &permissions)
	{
		permissions.insert("user_events");
		permissions.insert("friends_events");
	}

	/*
	 * Adds to the "permissions" set all permissions needed publish on a Event object.
	 */
	static void publishingData(MAUtil::Set<MAUtil::String> &permissions)
	{
		permissions.insert("publish_stream");
	}

	struct Publish
	{
		/*
		 * Adds to the "permissions" set the permission needed publish on an Event's wall.
		 */
		static void onWall(MAUtil::Set<MAUtil::String> &permissions)
		{
			permissions.insert("publish_stream");
		}

		/*
		 * Adds to the "permissions" set the permission needed to response to an event.
		 */
		static void responseToEvent(MAUtil::Set<MAUtil::String> &permissions)
		{
			permissions.insert("rsvp_event");
		}
	};
};

/*
 * template class for retrieving the permissions needed for a connection or to make a publish request on a FriendList object.
 */
template<>
class GetPermissionsFor<FriendList>
{
public:

	/*
	 * Adds to the "permissions" set all permissions needed to make any type of request on
	 * a FriendList object.
	 */
	static void allPosibleRequests(MAUtil::Set<MAUtil::String> &permissions)
	{
		permissions.insert("read_friendlists");
		permissions.insert("manage_friendlists");
	}

	/*
	 * Adds to the "permissions" set all permissions needed to ask for connections.
	 */
	static void retrievingData(MAUtil::Set<MAUtil::String> &permissions)
	{
		permissions.insert("read_friendlists");
	}

	/*
	 * Adds to the "permissions" set all permissions needed publish on a FriendList object.
	 */
	static void publishingData(MAUtil::Set<MAUtil::String> &permissions)
	{
		permissions.insert("manage_friendlists");
	}
};

/*
 * template class for retrieving the permissions needed for a connection or to make a publish request on a Group object.
 */
template<>
class GetPermissionsFor<Group>
{
public:
	/*
	 * Adds to the "permissions" set all permissions needed to make any type of request on
	 * a Group object.
	 */
	static void allPosibleRequests(MAUtil::Set<MAUtil::String> &permissions)
	{
		permissions.insert("user_groups");
		permissions.insert("friends_groups");
		permissions.insert("publish_stream");
	}

	/*
	 * Adds to the "permissions" set all permissions needed to ask for connections.
	 */
	static void retrievingData(MAUtil::Set<MAUtil::String> &permissions)
	{
		permissions.insert("user_groups");
		permissions.insert("friends_groups");
	}

	/*
	 * Adds to the "permissions" set all permissions needed publish on a Group object.
	 */
	static void publishingData(MAUtil::Set<MAUtil::String> &permissions)
	{
		permissions.insert("publish_stream");
	}
};

/*
 * template class for retrieving the permissions needed for a connection or to make a publish request on a Link object.
 */
template<>
class GetPermissionsFor<Link>
{
public:
	/*
	 * Adds to the "permissions" set all permissions needed to make any type of request on
	 * a Link object.
	 */
	static void allPosibleRequests(MAUtil::Set<MAUtil::String> &permissions)
	{
		permissions.insert("read_stream");
		permissions.insert("publish_stream");
	}

	/*
	 * Adds to the "permissions" set all permissions needed to ask for connections.
	 */
	static void retrievingData(MAUtil::Set<MAUtil::String> &permissions)
	{
		permissions.insert("user_groups");
		permissions.insert("friends_groups");
	}

	/*
	 * Adds to the "permissions" set all permissions needed publish on a Link object.
	 */
	static void publishingData(MAUtil::Set<MAUtil::String> &permissions)
	{
		permissions.insert("publish_stream");
	}
};

/*
 * template class for retrieving the permissions needed for a connection or to make a publish request on a Note object.
 */
template<>
class GetPermissionsFor<Note>
{
public:
	/*
	 * Adds to the "permissions" set all permissions needed to make any type of request on
	 * a Note object.
	 */
	static void allPosibleRequests(MAUtil::Set<MAUtil::String> &permissions)
	{
		permissions.insert("create_note");
		permissions.insert("user_notes");
		permissions.insert("friends_notes");
		permissions.insert("publish_stream");
	}

	/*
	 * Adds to the "permissions" set all permissions needed to ask for connections.
	 */
	static void retrievingData(MAUtil::Set<MAUtil::String> &permissions)
	{
		permissions.insert("user_notes");
		permissions.insert("friends_notes");
	}

	/*
	 * Adds to the "permissions" set all permissions needed publish on a Note object.
	 */
	static void publishingData(MAUtil::Set<MAUtil::String> &permissions)
	{
		permissions.insert("create_note");
		permissions.insert("publish_stream");
	}
};

/*
 * template class for retrieving the permissions needed for a connection or to make a publish request on a Photo object.
 */
template<>
class GetPermissionsFor<Photo>
{
public:
	/*
	 * Adds to the "permissions" set all permissions needed to make any type of request on
	 * a Photo object.
	 */
	static void allPosibleRequests(MAUtil::Set<MAUtil::String> &permissions)
	{
		permissions.insert("user_photos");
		permissions.insert("friends_photos");
		permissions.insert("user_photo_video_tags");
		permissions.insert("friends_photo_video_tags");
		permissions.insert("publish_stream");
	}

	/*
	 * Adds to the "permissions" set all permissions needed to ask for connections.
	 */
	static void retrievingData(MAUtil::Set<MAUtil::String> &permissions)
	{
		permissions.insert("user_photos");
		permissions.insert("friends_photos");
		permissions.insert("user_photo_video_tags");
		permissions.insert("friends_photo_video_tags");
	}

	/*
	 * Adds to the "permissions" set all permissions needed publish on a Photo object.
	 */
	static void publishingData(MAUtil::Set<MAUtil::String> &permissions)
	{
		permissions.insert("publish_stream");
		//permissions.insert("photo_upload"); //hm???
	}
};

/*
 * template class for retrieving the permissions needed for a connection or to make a publish request on a Post object.
 */
template<>
class GetPermissionsFor<Post>
{
public:
	/*
	 * Adds to the "permissions" set all permissions needed to make any type of request on
	 * a Post object.
	 */
	static void allPosibleRequests(MAUtil::Set<MAUtil::String> &permissions)
	{
		permissions.insert("read_stream");
		permissions.insert("publish_stream");
	}

	/*
	 * Adds to the "permissions" set all permissions needed to ask for connections.
	 */
	static void retrievingData(MAUtil::Set<MAUtil::String> &permissions)
	{
		permissions.insert("read_stream");
		//permissions.insert("manage_pages");
	}

	/*
	 * Adds to the "permissions" set all permissions needed publish on a Post object.
	 */
	static void publishingData(MAUtil::Set<MAUtil::String> &permissions)
	{
		permissions.insert("publish_stream");
	}
};

/*
 * template class for retrieving the permissions needed for a connection or to make a publish request on a StatusMessage object.
 */
template<>
class GetPermissionsFor<StatusMessage>
{
public:
	/*
	 * Adds to the "permissions" set all permissions needed to make any type of request on
	 * a StatusMessage object.
	 */
	static void allPosibleRequests(MAUtil::Set<MAUtil::String> &permissions)
	{
		permissions.insert("publish_stream");
	}

	/*
	 * Adds to the "permissions" set all permissions needed to ask for connections.
	 */
	static void retrievingData(MAUtil::Set<MAUtil::String> &permissions)
	{

	}

	/*
	 * Adds to the "permissions" set all permissions needed publish on a StatusMessage object.
	 */
	static void publishingData(MAUtil::Set<MAUtil::String> &permissions)
	{
		permissions.insert("publish_stream");
	}
};

/*
 * template class for retrieving the permissions needed for a connection or to make a publish request on a User object.
 */
template<>
class GetPermissionsFor<User>
{
public:
	/*
	 * Adds to the "permissions" set all permissions needed to make any type of request on
	 * a User object.
	 */
	static void allPosibleRequests(MAUtil::Set<MAUtil::String> &permissions)
	{
		retrievingData(permissions);
		publishingData(permissions);
	}

	/*
	 * Adds to the "permissions" set all permissions needed to ask for connections.
	 */
	static void retrievingData(MAUtil::Set<MAUtil::String> &permissions)
	{
		Retrieve::onlyConnections(permissions);
		Retrieve::onlyFields(permissions);
	}

	/*
	 * Adds to the "permissions" set all permissions needed publish on a User object.
	 */
	static void publishingData(MAUtil::Set<MAUtil::String> &permissions)
	{
		permissions.insert("publish_stream");
		permissions.insert("publish_checkins");
		permissions.insert("create_event");
		permissions.insert("manage_friendlists");
		permissions.insert("photo_upload");
		permissions.insert("share_item");
		permissions.insert("publish_actions");
	}

	struct Retrieve
	{
		/*
		 * Adds to the "permissions" set all permissions needed to retrieve the fields of a User object.
		 */
		static void onlyFields(MAUtil::Set<MAUtil::String> &permissions)
		{
			permissions.insert("user_likes");
			permissions.insert("friends_likes");

			permissions.insert("user_about_me");
			permissions.insert("friends_about_me");

			permissions.insert("user_birthday");
			permissions.insert("friends_birthday");

			permissions.insert("user_education_history");
			permissions.insert("friends_education_history");

			permissions.insert("user_hometown");
			permissions.insert("friends_hometown");

			permissions.insert("user_relationship_details");
			permissions.insert("friends_relationship_details");

			permissions.insert("user_relationships");
			permissions.insert("friends_relationships");

			permissions.insert("user_religion_politics");
			permissions.insert("friends_religion_politics");

			permissions.insert("user_website");
			permissions.insert("friends_website");

			permissions.insert("user_work_history");
			permissions.insert("friends_work_history");
		}

		/*
		 * Adds to the "permissions" set all permissions needed to retrieve the connections of a User object.
		 */
		static void onlyConnections(MAUtil::Set<MAUtil::String> &permissions)
		{
			//permissions.insert("manage_pages");

			permissions.insert("user_activities");
			permissions.insert("friends_activities");

			permissions.insert("user_photos");
			permissions.insert("friends_photos");

			permissions.insert("user_likes");
			permissions.insert("friends_likes");

			permissions.insert("user_checkins");
			permissions.insert("friends_checkins");

			permissions.insert("user_events");
			permissions.insert("friends_events");

			permissions.insert("user_relationships");

			permissions.insert("read_stream");

			permissions.insert("user_groups");
			permissions.insert("friends_groups");

			permissions.insert("user_interests");
			permissions.insert("friends_interests");

			permissions.insert("user_photo_video_tags");
			permissions.insert("friends_photo_video_tags");
		}

		struct Field
		{
			/*
			 * Adds to the "permissions" set the permission needed to to retrieve the "languages" field of an User object.
			 */
			static void languages(MAUtil::Set<MAUtil::String> &permissions)
			{
				permissions.insert("user_likes");
			}

			/*
			 * Adds to the "permissions" set the permission needed to to retrieve the "bio" field of an User object.
			 */
			static void bio(MAUtil::Set<MAUtil::String> &permissions)
			{
				permissions.insert("user_about_me");
				permissions.insert("friends_about_me");
			}

			/*
			 * Adds to the "permissions" set the permission needed to to retrieve the "birthday" field of an User object.
			 */
			static void birthday(MAUtil::Set<MAUtil::String> &permissions)
			{
				permissions.insert("user_birthday");
				permissions.insert("friends_birthday");
			}

			/*
			 * Adds to the "permissions" set the permission needed to to retrieve the "education" field of an User object.
			 */
			static void education(MAUtil::Set<MAUtil::String> &permissions)
			{
				permissions.insert("user_education_history");
				permissions.insert("friends_education_history");
			}

			/*
			 * Adds to the "permissions" set the permission needed to to retrieve the "hometown" field of an User object.
			 */
			static void hometown(MAUtil::Set<MAUtil::String> &permissions)
			{
				permissions.insert("user_hometown");
				permissions.insert("friends_hometown");
			}

			/*
			 * Adds to the "permissions" set the permission needed to to retrieve the "interested_in" field of an User object.
			 */
			static void interested_in(MAUtil::Set<MAUtil::String> &permissions)
			{
				permissions.insert("user_relationship_details");
				permissions.insert("friends_relationship_details");
			}

			/*
			 * Adds to the "permissions" set the permission needed to to retrieve the "favorite_teams" field of an User object.
			 */
			static void favorite_teams(MAUtil::Set<MAUtil::String> &permissions)
			{
				permissions.insert("user_likes");
				permissions.insert("friends_likes");
			}

			/*
			 * Adds to the "permissions" set the permission needed to to retrieve the "favorite_athletes" field of an User object.
			 */
			static void favorite_athletes(MAUtil::Set<MAUtil::String> &permissions)
			{
				permissions.insert("user_likes");
				permissions.insert("friends_likes");
			}

			/*
			 * Adds to the "permissions" set the permission needed to to retrieve the "quotes" field of an User object.
			 */
			static void quotes(MAUtil::Set<MAUtil::String> &permissions)
			{
				permissions.insert("user_about_me");
				permissions.insert("friends_about_me");
			}

			/*
			 * Adds to the "permissions" set the permission needed to to retrieve the "relationship_status" field of an User object.
			 */
			static void relationship_status(MAUtil::Set<MAUtil::String> &permissions)
			{
				permissions.insert("user_relationships");
				permissions.insert("friends_relationships");
			}

			/*
			 * Adds to the "permissions" set the permission needed to to retrieve the "religion" field of an User object.
			 */
			static void religion(MAUtil::Set<MAUtil::String> &permissions)
			{
				permissions.insert("user_religion_politics");
				permissions.insert("friends_religion_politics");
			}

			/*
			 * Adds to the "permissions" set the permission needed to to retrieve the "significant_other" field of an User object.
			 */
			static void significant_other(MAUtil::Set<MAUtil::String> &permissions)
			{
				permissions.insert("user_relationship_details");
				permissions.insert("friends_relationship_details");
			}

			/*
			 * Adds to the "permissions" set the permission needed to to retrieve the "website" field of an User object.
			 */
			static void website(MAUtil::Set<MAUtil::String> &permissions)
			{
				permissions.insert("user_website");
				permissions.insert("friends_website");
			}

			/*
			 * Adds to the "permissions" set the permission needed to to retrieve the "work" field of an User object.
			 */
			static void work(MAUtil::Set<MAUtil::String> &permissions)
			{
				permissions.insert("user_work_history");
				permissions.insert("friends_work_history");
			}

		};

		struct Connection
		{
			static void accounts(MAUtil::Set<MAUtil::String> &permissions)
			{
				permissions.insert("manage_pages");
			}

			/*
			 * Adds to the "permissions" set the permission needed to to retrieve the "activities" connection of an User object.
			 */
			static void activities(MAUtil::Set<MAUtil::String> &permissions)
			{
				permissions.insert("user_activities");
				permissions.insert("friends_activities");
			}

			/*
			 * Adds to the "permissions" set the permission needed to to retrieve the "albums" connection of an User object.
			 */
			static void albums(MAUtil::Set<MAUtil::String> &permissions)
			{
				permissions.insert("user_photos");
				permissions.insert("friends_photos");
			}

			/*
			 * Adds to the "permissions" set the permission needed to to retrieve the "books" connection of an User object.
			 */
			static void books(MAUtil::Set<MAUtil::String> &permissions)
			{
				permissions.insert("user_likes");
				permissions.insert("friends_likes");
			}

			/*
			 * Adds to the "permissions" set the permission needed to to retrieve the "checkins" connection of an User object.
			 */
			static void checkins(MAUtil::Set<MAUtil::String> &permissions)
			{
				permissions.insert("user_checkins");
				permissions.insert("friends_checkins");
			}

			/*
			 * Adds to the "permissions" set the permission needed to to retrieve the "events" connection of an User object.
			 */
			static void events(MAUtil::Set<MAUtil::String> &permissions)
			{
				permissions.insert("user_events");
				permissions.insert("friends_events");
			}

			/*
			 * Adds to the "permissions" set the permission needed to to retrieve the "family" connection of an User object.
			 */
			static void family(MAUtil::Set<MAUtil::String> &permissions)
			{
				permissions.insert("user_relationships");
			}

			/*
			 * Adds to the "permissions" set the permission needed to to retrieve the "feed" connection of an User object.
			 */
			static void feed(MAUtil::Set<MAUtil::String> &permissions)
			{
				permissions.insert("read_stream");
			}

			/*
			 * Adds to the "permissions" set the permission needed to to retrieve the "games" connection of an User object.
			 */
			static void games(MAUtil::Set<MAUtil::String> &permissions)
			{
				permissions.insert("user_likes");
			}

			/*
			 * Adds to the "permissions" set the permission needed to to retrieve the "groups" connection of an User object.
			 */
			static void groups(MAUtil::Set<MAUtil::String> &permissions)
			{
				permissions.insert("user_groups");
				permissions.insert("friends_groups");
			}

			/*
			 * Adds to the "permissions" set the permission needed to to retrieve the "home" connection of an User object.
			 */
			static void home(MAUtil::Set<MAUtil::String> &permissions)
			{
				permissions.insert("read_stream");
			}

			/*
			 * Adds to the "permissions" set the permission needed to to retrieve the "interests" connection of an User object.
			 */
			static void interests(MAUtil::Set<MAUtil::String> &permissions)
			{
				permissions.insert("user_interests");
				permissions.insert("friends_interests");
			}

			/*
			 * Adds to the "permissions" set the permission needed to to retrieve the "likes" connection of an User object.
			 */
			static void likes(MAUtil::Set<MAUtil::String> &permissions)
			{
				permissions.insert("user_likes");
				permissions.insert("friends_likes");
			}

			/*
			 * Adds to the "permissions" set the permission needed to to retrieve the "links" connection of an User object.
			 */
			static void links(MAUtil::Set<MAUtil::String> &permissions)
			{
				permissions.insert("read_stream");
			}

			/*
			 * Adds to the "permissions" set the permission needed to to retrieve the "movies" connection of an User object.
			 */
			static void movies(MAUtil::Set<MAUtil::String> &permissions)
			{
				permissions.insert("user_likes");
				permissions.insert("friends_likes");
			}

			/*
			 * Adds to the "permissions" set the permission needed to to retrieve the "music" connection of an User object.
			 */
			static void music(MAUtil::Set<MAUtil::String> &permissions)
			{
				permissions.insert("user_likes");
				permissions.insert("friends_likes");
			}

			/*
			 * Adds to the "permissions" set the permission needed to to retrieve the "notes" connection of an User object.
			 */
			static void notes(MAUtil::Set<MAUtil::String> &permissions)
			{
				permissions.insert("read_stream");
			}

			/*
			 * Adds to the "permissions" set the permission needed to to retrieve the "photos" connection of an User object.
			 */
			static void photos(MAUtil::Set<MAUtil::String> &permissions)
			{
				permissions.insert("user_photo_video_tags");
				permissions.insert("friends_photo_video_tags");
			}

			/*
			 * Adds to the "permissions" set the permission needed to to retrieve the "statuses" connection of an User object.
			 */
			static void statuses(MAUtil::Set<MAUtil::String> &permissions)
			{
				permissions.insert("read_stream");
			}

			/*
			 * Adds to the "permissions" set the permission needed to to retrieve the "tagged" connection of an User object.
			 */
			static void tagged(MAUtil::Set<MAUtil::String> &permissions)
			{
				permissions.insert("read_stream");
			}

			/*
			 * Adds to the "permissions" set the permission needed to to retrieve the "television" connection of an User object.
			 */
			static void television(MAUtil::Set<MAUtil::String> &permissions)
			{
				permissions.insert("user_likes");
				permissions.insert("friends_likes");
			}

			/*
			 * Adds to the "permissions" set the permission needed to to retrieve the "videos" connection of an User object.
			 */
			static void videos(MAUtil::Set<MAUtil::String> &permissions)
			{
				permissions.insert("user_videos");
				permissions.insert("friends_videos");
			}
		};
	};

	struct Publish
	{
		/*
		 * Adds to the "permissions" set the permission needed to publish an album.
		 */
		static void album(MAUtil::Set<MAUtil::String> &permissions)
		{
			permissions.insert("publish_stream");
		}

		/*
		 * Adds to the "permissions" set the permission needed to to publish a checkin.
		 */
		static void checkin(MAUtil::Set<MAUtil::String> &permissions)
		{
			permissions.insert("publish_checkins");
		}

		/*
		 * Adds to the "permissions" set, the permission needed to to to publish an event.
		 */
		static void event(MAUtil::Set<MAUtil::String> &permissions)
		{
			permissions.insert("create_event");
		}

		/*
		 * Adds to the "permissions" set the permission needed to publish a friendList.
		 */
		static void friendList(MAUtil::Set<MAUtil::String> &permissions)
		{
			permissions.insert("manage_friendlists");
		}

		/*
		 * Adds to the "permissions" set the permission needed needed to publish on wall.
		 */
		static void onWall(MAUtil::Set<MAUtil::String> &permissions)
		{
			permissions.insert("publish_stream");
			permissions.insert("photo_upload");
			permissions.insert("share_item");
			permissions.insert("publish_actions");
		}
	};
};

/*
 * template class for retrieving the permissions needed for a connection or to make a publish request on a Video object.
 */
template<>
class GetPermissionsFor<Video>
{
public:
	/*
	 * Adds to the "permissions" set all permissions needed to make any type of request on
	 * a Video object.
	 */
	static void allPosibleRequests(MAUtil::Set<MAUtil::String> &permissions)
	{
		permissions.insert("user_videos");
		permissions.insert("publish_stream");
		permissions.insert("video_upload");
	}

	/*
	 * Adds to the "permissions" set all permissions needed to ask for connections.
	 */
	static void retrievingData(MAUtil::Set<MAUtil::String> &permissions)
	{
		permissions.insert("user_videos");
	}

	/*
	 * Adds to the "permissions" set all permissions needed publish on a Video object.
	 */
	static void publishingData(MAUtil::Set<MAUtil::String> &permissions)
	{
		permissions.insert("publish_stream");
		permissions.insert("video_upload");
	}
};


#endif /* GETPERMISSIONS_H_ */
