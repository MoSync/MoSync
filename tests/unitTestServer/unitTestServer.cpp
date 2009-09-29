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

//listen. use a thread pool.

#include <stdio.h>
#include <conio.h>
#include <winsock2.h>
#include <vector>
#include <ThreadPool.h>
#include <FileStream.h>
#include <bluetooth/server.h>
#include "..\unitTest\conn_common.h"

ThreadPool gThreadPool;
char gServerData[DATA_SIZE];
char gClientData[DATA_SIZE];

void singleSocketSpinOff(SOCKET sock);
void socketSizeSpinOff(SOCKET sock);

#define TB(func) if(!(func)) { printf("%s failed\n", #func); return false; }

void MoSyncErrorExit(int) {
}

bool loadData() {
	int len;

	Base::FileStream c("../unitTest/client_data.bin");
	TB(c.isOpen());
	TB(c.length(len));
	TB(c.read(gClientData, DATA_SIZE));

	Base::FileStream s("../unitTest/server_data.bin");
	TB(s.isOpen());
	TB(s.length(len));
	TB(s.read(gServerData, DATA_SIZE));

	return true;
}

class Acceptor : public Runnable {
	short mPort;
	void (*mSpinOff)(SOCKET);
	static std::vector<SOCKET> mSockets;
public:
	Acceptor(short port, void (*spinOff)(SOCKET)) : mPort(port), mSpinOff(spinOff) {}

	static void closeAll() {
		for(size_t i=0; i<mSockets.size(); i++) {
			closesocket(mSockets[i]);
		}
	}

	void run() {
		//socket
		SOCKET servSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		if(servSock == INVALID_SOCKET) {
			printf("socket error %i\n", WSAGetLastError());
			return;
		}

		//bind
		sockaddr_in service;
		service.sin_family = AF_INET;
		service.sin_addr.s_addr = INADDR_ANY;
		service.sin_port = htons(mPort);

		if(bind(servSock, (SOCKADDR*)&service, sizeof(service)) == SOCKET_ERROR) {
			printf("bind error %i.\n", WSAGetLastError());
			closesocket(servSock);
			return;
		}

		//listen
		if(listen(servSock, SOMAXCONN) == SOCKET_ERROR) {
			printf("listen error %i.\n", WSAGetLastError());
			closesocket(servSock);
			return;
		}

		//accept
		mSockets.push_back(servSock);
		printf("Waiting...\n");
		while(true) {
			sockaddr_in remoteAddr;
			int addrLen = sizeof(remoteAddr);
			SOCKET aSock = accept(servSock, (SOCKADDR*)&remoteAddr, &addrLen);
			if(aSock == INVALID_SOCKET) {
				printf("accept error %i.\n", WSAGetLastError());
				closesocket(servSock);
				return;
			}
			printf("Connection accepted from %s:%i\n",
				inet_ntoa(remoteAddr.sin_addr), remoteAddr.sin_port);
			mSpinOff(servSock);
		}
	}
};

std::vector<SOCKET> Acceptor::mSockets;

class WriteReply : public Runnable {
	SOCKET mSock;
	const char mReply;
public:
	WriteReply(SOCKET sock, char reply) : mSock(sock), mReply(reply) {}

	void run() {
		int res = send(mSock, &mReply, 1, 0);
		if(res != 1) {
			printf("send error %i (WSA %i)\n", res, WSAGetLastError());
		}
		if(closesocket(mSock) == SOCKET_ERROR) {
			printf("closesocket error %i\n", WSAGetLastError());
		}
	}
};

class SockWrite : public Runnable {
	SOCKET mSock;
	const char* mSrc;
	const int mSize;
public:
	SockWrite(SOCKET sock, const char* src, int size) : mSock(sock), mSrc(src), mSize(size) {}

	void run() {
		int res = send(mSock, mSrc, mSize, 0);
		if(res != mSize) {
			printf("send error %i (WSA %i)\n", res, WSAGetLastError());
		}
	}
};

class ReadClientData : public Runnable {
	SOCKET mSock;
	char mReadBuffer[DATA_SIZE];
	int pos;
public:
	ReadClientData(SOCKET sock) : mSock(sock) {}

	void run() {
		pos = 0;
		while(pos < DATA_SIZE) {
			int res = recv(mSock, mReadBuffer + pos, DATA_SIZE - pos, 0);
			if(res == SOCKET_ERROR) {
				printf("recv error %i\n", WSAGetLastError());
				return;
			}
			pos += res;
			if(pos > DATA_SIZE) {
				printf("recv buffer overrun!\n");
				return;
			}
		}

		bool success = memcmp(mReadBuffer, gClientData, DATA_SIZE) == 0;
		gThreadPool.execute(new WriteReply(mSock, success ? 1 : 0));
	}
};

void singleSocketSpinOff(SOCKET sock) {
	gThreadPool.execute(new SockWrite(sock, gServerData, DATA_SIZE));
	gThreadPool.execute(new ReadClientData(sock));
}

void socketSizeSpinOff(SOCKET sock) {
	//gThreadPool.execute(new SockSizeWrite(sock, gServerData, DATA_SIZE));
}


void closeProgram(int sn) {
	printf("Signal %i\n", sn);
	Acceptor::closeAll();
	gThreadPool.close();
	exit(sn);
}


#undef main	//sdl.h
int main() {
	printf("Hello World.\n");
	Bluetooth::MABtInit();

	if(!loadData()) {
		return -1;
	}

	gThreadPool.execute(new Acceptor(SINGLE_SOCKET_PORT, singleSocketSpinOff));
	//gThreadPool.execute(new Acceptor(SOCKET_SIZE_PORT, socketSizeSpinOff));

	while(true) {
		int ch = _getch();
		printf("ch %i\n", ch);
		if(ch == '0' || ch == 3) {
			closeProgram(0);
		}
	}
	return 0;
}
