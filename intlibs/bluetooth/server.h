/* Copyright (C) 2009 Mobile Sorcery AB

This program is free software; you can redistribute it and/or modify it under
the terms of the GNU General Public License, version 2, as published by
the Free Software Foundation.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
for more details.

You should have received a copy of the GNU General Public License
along with this program; see the file COPYING.  If not, write to the Free
Software Foundation, 59 Temple Place - Suite 330, Boston, MA
02111-1307, USA.
*/

#ifndef SERVER_H
#define SERVER_H

#include "connection.h"

#define ANY_PORT (-1)

class BtSppServer : public Closable {
public:
	//BtSppServer();

	//Returns the opened port number (1-30) or a CONNERR code.
	//The UUID is registered with the SDP system.
	int open(const MAUUID& uuid, const char* name, int port = ANY_PORT);

	//Stores the Bluetooth address of an open server socket.
	int getAddr(MAConnAddr& addr);

	//Stores the address of a new, pre-connected BtSppConnection in connP.
	//This connection should be deleted when no longer needed.
	//Returns >0 or CONNERR code.
	int accept(BtSppConnection*& connP);

	void close();

	virtual ~BtSppServer() { close(); }

private:
#ifdef WIN32
	SOCKET mSock;
	SOCKADDR_BTH mAddr;
#endif
};

#endif	//SERVER_H
