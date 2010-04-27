#include "Server.h"
#include <ma.h>

using namespace MAUtil;

void ServerListener::serverAcceptFailed(Server* server, int result) {}

Server::~Server() {
	close();
}

int Server::getAddr(MAConnAddr* dst) {
	return maConnGetAddr(mServ, dst);
}

int Server::start(const char* url) {
	mServ = maConnect(url);
	if(mServ < 0)
		return mServ;
	Environment::getEnvironment().setConnListener(mServ, this);
	return 1;
}

bool Server::isOpen() const {
	return mServ > 0;
}

void Server::close() {
	if(mServ > 0) {
		maConnClose(mServ);
		mServ = 0;
	}
}

int Server::accept(ConnectionListener* cl) {
	mCl = cl;
	return maAccept(mServ);
}

void Server::connEvent(const MAConnEventData& data) {
	MAASSERT(data.opType == CONNOP_ACCEPT);
	MAASSERT(data.handle == mServ);
	int res = data.result;
	if(res > 0) {
		Connection* c = new Connection(mCl, res);
		mListener->serverAccepted(this, c);
	}
	if(res < 0) {
		mListener->serverAcceptFailed(this, res);
	}		
}
