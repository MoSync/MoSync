/*
 * Facebook.cpp
 *
 * An Open Graph API implementation for MoSync. Can only be used with requests that don't
 * need authentication, or that need authentication as a user (there are, for example,
 * requests that need authentication as an application).
 *
 * Usage:
 *   FacebookQueryDownloadListener listener;
 *   Facebook fb("my-app-id");
 *   fb.addListener(&listener);
 *   MAUtil::String url = fb.getOAuthUrl()
 *
 * Then somehow show url to the user in a browser. This will give her the oppurtunity to
 * sign in to Facebook (if she isn't already), and to authorize your app. You should have
 * set up your web view to notify you on URL changes (or -- if you use a real browser --
 * have installed your app as the URL handler for URLs starting with fbconnect://).
 *
 * When you receive a URL starting with fbconnect://, you can read the access token
 * after "#access_token=" in that URL. Send that token to the API
 *
 *   fb.setAccessToken(accessToken);
 *
 * Then you can call any graph method
 *
 *   // Get the user's news feed
 *   Map<String, String> params
 *   fb.requestGraph("me/home", params, "GET");
 *
 * Your listener.queryFinished (or queryCancelled or queryError) will be called when
 * the request is done.
 *
 *  Created on: Feb 22, 2011
 *      Author: Magnus Hult <magnus@magnushult.se>
 */

#include <MAUtil/String.h>
#include <MAUtil/Map.h>
#include <mavsprintf.h>

#include "../JSON_lib/YAJLDom.h"
#include "Facebook.h"
#include "HttpQueryDownloader.h"
#include "HttpResponse.h"

#include "../GraphAPI/Publish/PublishListener.h"

#include "../GraphAPI/RetrieveDataListener.h"

using namespace MAUtil;

Facebook::Facebook(const String& appId, const MAUtil::Set<MAUtil::String>& permissions) :
	mAppId(appId),
	mConnectionListener(0),
	mPublishingListener(0)
{
//	mOAuthUrl = "https://graph.facebook.com/oauth/authorize?type=user_agent&";
//	mOAuthUrl += "redirect_uri=fbconnect%3A%2F%2Fsuccess&";
//	mOAuthUrl += "display=touch&";
//	mOAuthUrl += "client_id=" + mAppId;

	mOAuthUrl = "https://www.facebook.com/dialog/oauth?";
	mOAuthUrl += "redirect_uri=fbconnect%3A%2F%2Fsuccess&";
	mOAuthUrl += "response_type=token&";
	mOAuthUrl += "display=touch&";
	mOAuthUrl += "client_id=" + mAppId;

	initialize(permissions);
}

Facebook::Facebook(const String& appId, const MAUtil::Set<MAUtil::String> &permissions,
		const MAUtil::String &oAuthUrl):
		mAppId(appId),
		mConnectionListener(0),
		mPublishingListener(0)
{
	mOAuthUrl = oAuthUrl;
	initialize(permissions);
}

void Facebook::setPermissions(const MAUtil::Set<MAUtil::String>& permissions)
{

	if (permissions.size() > 0)
	{
		mOAuthUrl += "&scope=";
		MAUtil::Set<MAUtil::String>::ConstIterator test = permissions.begin();
		for(MAUtil::Set<MAUtil::String>::ConstIterator it = permissions.begin();
				it!=permissions.end(); ++it )
		{
			mOAuthUrl += *it;
			test = it;
			++test;
			if (test != permissions.end())
			{
				mOAuthUrl += ",";
			}
		}
	}
}

const String& Facebook::getOAuthUrl() const {
	return mOAuthUrl;
}

void Facebook::setAccessToken(const String& accessToken) {
	mAccessToken = accessToken;
}

const MAUtil::String &Facebook::getAccessToken() const
{
	return mAccessToken;
}

void Facebook::requestGraph(	FACEBOOK_REQUEST_TYPE type,
								FACEBOOK_RESPONSE_TYPE responseType,
								const String& path,
								int method,
								const String postdata)
{

	FacebookRequest *r = new FacebookRequest(	type,
												responseType,
												path,
												mAccessToken,
												method,
												postdata);
	request(r);
}

void Facebook::requestGraph(FACEBOOK_REQUEST_TYPE type,
							FACEBOOK_RESPONSE_TYPE responseType,
							const String& path,
							int method,
							const Map<String, String>& messageBodyParams,
							const String postdata)
{

	FacebookRequest *r = new FacebookRequest(	type,
												responseType,
												path,
												mAccessToken,
												method,
												postdata);
	if(messageBodyParams.size()>0)
	{
		r->setMessageBodyParams(messageBodyParams);
	}
	request(r);
}

void Facebook::requestGraph(	FACEBOOK_REQUEST_TYPE type,
								FACEBOOK_RESPONSE_TYPE responseType,
								const String& path,
								int method,
								const Map<String, String>& messageBodyParams,
								const Map<String, String>& requestHeaders,
								const String postdata)
{
	FacebookRequest *r = new FacebookRequest(	type,
												responseType,
												path,
												mAccessToken,
												method,
												postdata);
	if(messageBodyParams.size()>0)
	{
		r->setMessageBodyParams(messageBodyParams);
	}

	if(requestHeaders.size()>0)
	{
		r->setRequestHeaders(requestHeaders);
	}

	request(r);
}

void Facebook::response(HttpRequest* req, HttpResponse* res)
{
	bool success = res->getCode() == 200;

	FacebookRequest* fbRequest = (FacebookRequest*) req;

	if(!success)
	{
		if(fbRequest->getType() == RETRIEVE_DATA && mConnectionListener )
		{
			mConnectionListener->queryError(res->getCode(), fbRequest->getPath()); //403
		}
		else if((fbRequest->getType() == PUBLISH) && mPublishingListener )
		{
			mPublishingListener->queryError(res->getCode(), fbRequest->getPath()); //403
		}
		delete req;
		delete res;
		return;
	}

	FacebookResponse fbResponse(res->getCode(), res->getDataSize(), res->getData());

	if( (fbRequest->getType() == RETRIEVE_DATA) && mConnectionListener)
	{
		MAUtil::String connType;
		MAUtil::String id;
		extractConnectionTypeAndId(fbRequest, connType, id);

		if (fbRequest->getResponseType() == JSON)
		{
			MAUtil::YAJLDom::Value *jsonData = NULL;
			if(fbResponse.getDataSize()>0)
			{
				jsonData = fbResponse.getJsonData();
			}

			mConnectionListener->jsonDataReceived(jsonData, connType, id);
		}
		else if (fbRequest->getResponseType() == IMAGE)
		{
			mConnectionListener->imageReceived(fbResponse.getImageData(), connType, id);
		}
		delete req;
		delete res;
		return;
	}

	if((fbRequest->getType() == PUBLISH) && mPublishingListener)
	{
		if(fbResponse.getDataSize()==0)
		{
			mPublishingListener->queryError(fbResponse.getCode(), fbRequest->getPath());
			delete req;
			delete res;
			return;
		}

		MAUtil::YAJLDom::Value *jsonData = fbResponse.getJsonData();

		if( fbRequest->getResponseType() == STRING )
		{
			mPublishingListener->publishingResponseReceived(jsonData->toString(), fbRequest->getPath());
		}
		else if( fbRequest->getResponseType() == BOOLEAN )
		{
			mPublishingListener->publishingResponseReceived(jsonData->toBoolean(), fbRequest->getPath());
		}

	}

	delete req;
	delete res;
}

void Facebook::postMultipartFormData(
		const MAUtil::String &ID,
		const MAUtil::String &reqType,
		const byte  *pixels,
		int pixelsArraySize,
		const MAUtil::String &contentType,
		const MAUtil::String &fileType,
		const MAUtil::String &fileName,
		const MAUtil::String &message )
{

	const MAUtil::String path = ID + "/" + reqType;

	UploadRequest *req = new UploadRequest(path, mAccessToken, pixels, pixelsArraySize,
			contentType, fileName, message );

	request(req);
}

void Facebook::setRetrieveDataListener(RetrieveDataListener* listener)
{
	mConnectionListener = listener;
}

void Facebook::setPublishingListener(PublishingListener* listener)
{
	mPublishingListener = listener;
}

bool Facebook::extractConnectionTypeAndId(FacebookRequest *req, MAUtil::String &connectionType, MAUtil::String &id) const
{
	const MAUtil::String path = req->getPath();

	int found = path.findFirstOf('/');
	if(String::npos == found)
	{
		return false;
	}
	id = path.substr(0, found);
	connectionType = path.substr(found+1);

    return (connectionType.size()>0 && id.size()>0);
}

void Facebook::initialize(const MAUtil::Set<MAUtil::String>& permissions)
{
	mConnectionListener = NULL;
	mPublishingListener = NULL;
	setPermissions(permissions);
	HttpProtocol::addListener(this);
}
