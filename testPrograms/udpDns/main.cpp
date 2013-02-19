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
* This example shows how to do DNS over UDP.
*/

#include <maapi.h>
#include <MAUtil/Moblet.h>
#include <MAUtil/Connection.h>
#include <conprint.h>
#include <maassert.h>
#include <mastdlib.h>

using namespace MAUtil;

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;

#define BUFSIZE 512

#define EXAMPLE_DOMAIN "example.com"

static u16 htons(u16 x) {
	return (x << 8) | (x >> 8);
}

static u16 ntohs(u16 x) {
	return (x << 8) | (x >> 8);
}

class MyMoblet : public Moblet, private ConnectionListener {
private:
	char mBuffer[BUFSIZE];
	Connection mConn;
	bool mRepeat;
	int mCount;
	bool mInProgress;
	u16 mQueryId;
public:
	MyMoblet() : mConn(this) {
		mRepeat = false;
		mCount = 0;
		mInProgress = false;

		int res = mConn.connect("datagram://192.168.0.1:53");
		printf("connect: %i\n", res);
	}

	/**
	* Shows basic information on how to use the program
	*/
	void showInformation()
	{
#if 0
		printf("DNS query -\n");
		printf("   Press fire key or\n");
		printf("   touch the screen\n");
		printf("Toggle REPEAT -\n");
		printf("   Press 5 key\n");
		printf("To EXIT -\n");
		printf("   Press 0 key or\n");
		printf("   soft right key\n");
#endif
	}

	void start(const char* domain) {
		if(mInProgress)
			return;
		mInProgress = true;
		if(mRepeat) {
			mCount++;
			printf("%i\n", mCount);
		}
		printf("Preparing query packet...\n");
		sendQuery(domain);
	}

	virtual void connectFinished(Connection* conn, int result) {
		printf("connectFinished: %i\n", result);
		if(result < 0) {
			return;
		}
		start(EXAMPLE_DOMAIN);
	}

	virtual void connWriteFinished(Connection* conn, int result) {
		printf("connWriteFinished: %i\n", result);
		mConn.recv(mBuffer, sizeof(mBuffer));
	}

	virtual void connRecvFinished(Connection* conn, int result) {
		printf("connRecvFinished: %i\n", result);
		if(result < 0) {
			return;
		}
		parseReply(result);
		mInProgress = false;
		if(mRepeat)
			start(EXAMPLE_DOMAIN);
		else
			showInformation();
	}

	struct DNS_HEADER {
		u16 id;
		// bits in bit fields are ordered low-to-high significance.
		u8 rd : 1;
		u8 tc : 1;
		u8 aa : 1;
		u8 opcode : 4;
		u8 qr : 1;

		u8 rcode : 4;
		u8 z : 3;
		u8 ra : 1;

		u16 qdcount;
		u16 ancount;
		u16 nscount;
		u16 arcount;
	};

	void sendQuery(const char* domain) {
		// header
		//MAASSERT(sizeof(DNS_HEADER) == 12);
		printf("size: %i\n", (int)sizeof(DNS_HEADER));
		DNS_HEADER* hp = (DNS_HEADER*)mBuffer;
		DNS_HEADER& h(*hp);
		mQueryId = rand();
		printf("query id: 0x%x\n", mQueryId);
		h.id = htons(mQueryId);
		h.qr = 0;
		h.opcode = 0;
		h.tc = 0;
		h.rd = 1;
		h.z = 0;
		h.qdcount = htons(1);
		h.ancount = 0;
		h.nscount = 0;
		h.arcount = 0;

		// query section: name
		const char* src = domain;
		char* dst = mBuffer + sizeof(DNS_HEADER);
		while(*src) {
			const char* dot = strchr(src, '.');
			if(!dot) {
				int len = strlen(src);
				*(u8*)dst++ = (u8)len;
				memcpy(dst, src, len);
				dst += len;
				break;
			}
			if(dot == src)
				break;
			MAASSERT(dot > src);
			int len = dot - src;
			MAASSERT(len < 256);
			*(u8*)dst++ = (u8)len;
			memcpy(dst, src, len);
			dst += len;
			src = dot + 1;
		}
		*dst++ = 0;	// root null label
		// type A (01)
		*dst++ = 0;
		*dst++ = 1;
		// class IN (01)
		*dst++ = 0;
		*dst++ = 1;

		u32 packetLen = dst - mBuffer;
		mConn.write(mBuffer, packetLen);
	}

	void parseReply(u32 packetLen) {
		// ensure header holds sane values.
		if(packetLen < sizeof(DNS_HEADER)) {
			printf("Packet too small.\n");
			return;
		}
		const DNS_HEADER* hp = (DNS_HEADER*)mBuffer;
		const DNS_HEADER& h(*hp);
		if(ntohs(h.id) != mQueryId) {
			printf("Query ID mismatch: 0x%x != 0x%x\n", mQueryId, h.id);
			return;
		}
		if(!h.qr) {
			printf("Not a response!\n");
			return;
		}
		if(h.opcode != 0) {
			printf("Not a standard response!\n");
			return;
		}
		if(h.tc) {
			printf("Message truncated!\n");
			return;
		}
		if(h.rcode != 0) {
			printf("Response code 0x%x\n", h.rcode);
			return;
		}

		const u8* src = (u8*)mBuffer + sizeof(DNS_HEADER);
		const u8* end = (u8*)mBuffer + packetLen;

		// skip questions
		printf("%i questions\n", ntohs(h.qdcount));
		for(u16 i=0; i<ntohs(h.qdcount); i++) {
			if(!skipName(src, end))
				return;
			src += 4;
		}

		// parse answers
		printf("%i answers\n", ntohs(h.ancount));
		for(u16 i=0; i<ntohs(h.ancount); i++) {
			if(!skipName(src, end))
				return;
			src += 4;
			//printf("TTLx: %02x%02x%02x%02x\n", src[0], src[1], src[2], src[3]);
			u32 ttl = getU32(src);
			printf("TTL %i seconds (0x%x)\n", ttl, ttl);
			u16 rdlength = (*src++) << 8;
			rdlength |= (*src++);
			if(rdlength != 4) {
				printf("bad rdlength: 0x%x\n", rdlength);
				return;
			}
			printf("IP: %i.%i.%i.%i\n", src[0], src[1], src[2], src[3]);
		}
	}

	u8 getU8(const u8*& src) {
		u8 v = *src++;
		//printf("v: 0x%02x\n", v);
		return v;
	}

	u32 getU32(const u8*& src) {
		u32 v = 0;
		v |= (u32(getU8(src))) << 24;
#if 1
		v |= (u32(getU8(src))) << 16;
#else
		u8 v8 = getU8(src);
		printf("v8: 0x%02x\n", v8);
		u32 v32 = v8;
		printf("v32: 0x%x\n", v32);
		v |= v32 << 16;
		printf("v: 0x%x\n", v);
#endif
		v |= (u32(getU8(src))) << 8;
		v |= (u32(getU8(src)));
		return v;
	}

	bool skipName(const u8*& src, const u8* end) {
		do {
			if(src >= end) {
				printf("Packet too small.\n");
				return false;
			}
		} while(skipLabel(src));
		return true;
	}

	bool skipLabel(const u8*& src) {
		u8 len = *src++;
		if(len == 0)
			return false;
		if((len & 0xC0) == 0xC0) {	//pointer
			src++;
			return false;
		} else {
			src += len;
		}
		return true;
	}

	void keyPressEvent(int keyCode, int nativeCode) {
		if(keyCode == MAK_0 || keyCode == MAK_SOFTRIGHT || keyCode == MAK_BACK)
			maExit(0);
		if(keyCode == MAK_FIRE)
			start(EXAMPLE_DOMAIN);
		if(keyCode == MAK_5) {
			mRepeat = !mRepeat;
			mCount = 0;
			printf("Repeat: %s\n", mRepeat ? "on" : "off");
		}
	}

	/**
	* Moblet listener that has been fired when the screen has been pressed.
	*/
	void pointerPressEvent(MAPoint2d p) {
		start(EXAMPLE_DOMAIN);
	}
};

extern "C" int MAMain() {
	InitConsole();
	gConsoleLogging = 1;

	Moblet::run(new MyMoblet());
	return 0;
}
