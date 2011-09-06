/*
 * Connections.h
 *
 *  Created on: Jul 20, 2011
 *      Author: gabi
 */

#ifndef CONNECTIONS_H_
#define CONNECTIONS_H_

/*
 * All of the objects in the Facebook are connected to each other via relationships.
 * Those those relationships are called connections in Graph API.
 * The connections between objects can be retrieved by using the URL:
 * https://graph.facebook.com/ID/CONNECTION_TYPE
 * The connections supported for each type of Facebook object are listed below:
 */

class Link;
class Album;
class Checkin;
class Comment;
class Event;
class FriendList;
class Group;
class Note;
class Photo;
class Post;
class StatusMessage;
class User;
class Video;

template<class ObjType>
struct Connections;

/*
 * Template class for retrieving the available connections for an Album object
 */
template<>
struct Connections<Album>
{
	static const char *photos()
	{
		return "photos";
	}
	static const char *likes()
	{
		return "likes";
	}
	static const char *comments()
	{
		return "comments";
	}
	static const char *picture()
	{
		return "picture";
	}
};

/*
 * Template class for retrieving the available connections for an Checkin object
 */
template<>
struct Connections<Checkin>
{
	static const char *comments()
	{
		return "comments";
	}
	static const char *likes()
	{
		return "likes";
	}
};

/*
 * Template class for retrieving the available connections for an Comment object
 */
template<>
struct Connections<Comment>
{
	static const char *likes()
	{
		return "likes";
	}
};

/*
 * Template class for retrieving the available connections for an Event object
 */
template<>
struct Connections<Event>
{
	static const char *feed()
	{
		return "feed";
	}
	static const char *noreply()
	{
		return "noreply";
	}
	static const char *maybe()
	{
		return "maybe";
	}
	static const char *invited()
	{
		return "invited";
	}
	static const char *attending()
	{
		return "attending";
	}
	static const char *declined()
	{
		return "declined";
	}
	static const char *picture()
	{
		return "picture";
	}
};

/*
 * Template class for retrieving the available connections for an FriendList object
 */
template<>
struct Connections<FriendList>
{
	static const char *members()
	{
		return "members";
	}
};

/*
 * Template class for retrieving the available connections for an Group object
 */
template<>
struct Connections<Group>
{
	static const char *feed()
	{
		return "feed";
	}
	static const char *members()
	{
		return "members";
	}
	static const char *picture()
	{
		return "picture";
	}
};

/*
 * Template class for retrieving the available connections for an Link object
 */
template<>
struct Connections<Link>
{
	static const char *comments()
	{
		return "comments";
	}
	static const char *likes()
	{
		return "likes";
	}
};

/*
 * Template class for retrieving the available connections for an Note object
 */
template<>
struct Connections<Note>
{
	static const char *comments()
	{
		return "comments";
	}
	static const char *likes()
	{
		return "likes";
	}
};

/*
 * Template class for retrieving the available connections for an Photo object
 */
template<>
struct Connections<Photo>
{
	static const char *comments()
	{
		return "comments";
	}
	static const char *likes()
	{
		return "likes";
	}
	static const char *picture()
	{
		return "picture";
	}
	static const char *tags()
	{
		return "tags";
	}
};

/*
 * Template class for retrieving the available connections for an Post object
 */
template<>
struct Connections<Post>
{
	static const char *comments()
	{
		return "comments";
	}
	static const char *likes()
	{
		return "likes";
	}
};

/*
 * Template class for retrieving the available connections for an StatusMessage object
 */
template<>
struct Connections<StatusMessage>
{
	static const char *comments()
	{
		return "comments";
	}
	static const char *likes()
	{
		return "likes";
	}
};

/*
 * Template class for retrieving the available connections for an User object
 */
template<>
struct Connections<User>
{
	static const char *accounts()
	{
		return "accounts";
	}
	static const char *activities()
	{
		return "activities";
	}
	static const char *albums()
	{
		return "albums";
	}
	static const char *books()
	{
		return "books";
	}
	static const char *checkins()
	{
		return "checkins";
	}
	static const char *events()
	{
		return "events";
	}
	static const char *feed()
	{
		return "feed";
	}
	static const char *friendlists()
	{
		return "friendlists";
	}
	static const char *friends()
	{
		return "friends";
	}
	static const char *games()
	{
		return "games";
	}
	static const char *groups()
	{
		return "groups";
	}
	static const char *home()
	{
		return "home";
	}
	static const char *interests()
	{
		return "interests";
	}
	static const char *likes()
	{
		return "likes";
	}
	static const char *links()
	{
		return "links";
	}
	static const char *music()
	{
		return "music";
	}
	static const char *notes()
	{
		return "notes";
	}
	static const char *permissions()
	{
		return "permissions";
	}
	static const char *photos()
	{
		return "photos";
	}
	static const char *picture()
	{
		return "picture";
	}
	static const char *posts()
	{
		return "posts";
	}
	static const char *statuses()
	{
		return "statuses";
	}
	static const char *television()
	{
		return "television";
	}
	static const char *videos()
	{
		return "videos";
	}
};
/*
 * Template class for retrieving the available connections for an Video object
 */
template<>
struct Connections<Video>
{
	static const char *comments()
	{
		return "comments";
	}
	static const char *likes()
	{
		return "likes";
	}
	static const char *picture()
	{
		return "picture";
	}
};




#endif /* CONNECTIONS_H_ */
