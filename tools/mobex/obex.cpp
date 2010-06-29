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

#include <string>

#include "obex.h"
#include <helpers/smartie.h>

static int OBEXConnect(BtSppConnection& conn, u16* maxPacketSize, int fileSize);
static int OBEXPut(BtSppConnection& conn, u16 maxPacketSize,
									 const Array<char>& file, const Array<u16>& name);

/*static int OBEXSendSinglePacket(BtSppConnection& conn, u16 maxPacketSize,
															 const Array<char>& file, const Array<u16>& name,
															 std::string mimeType);
static int OBEXSendFirstPacket(BtSppConnection& conn, u16 maxPacketSize,
															 const Array<char>& file, const Array<u16>& name,
															 std::string mimeType, uint* pos);
static int OBEXSendMiddlePacket(BtSppConnection& conn, u16 maxPacketSize,
																const Array<char>& file, uint* pos);
static int OBEXSendLastPacket(BtSppConnection& conn, u16 maxPacketSize,
																const Array<char>& file, uint* pos);*/

static char HEX[] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
'a', 'b', 'c', 'd', 'e', 'f' };


static void dumpPacket(byte* p, int len) {
	LOG("%i bytes\n", len);
	char* buf = new char[len*3 + 5];
	int pos = 0;
	for (int i = 0; i < len; ) {
		int next = MIN(len, i + 16);
		for (; i < next; i++) {
			//printf("%02X", buffer[i]);
			buf[pos++] = HEX[(p[i] >> 4) & 0x0F];
			buf[pos++] = HEX[p[i] & 0x0F];

			if ((i % 4) == 3 && i < (next - 1))
				//printf(" ");
				buf[pos++] = ' ';
		}
		buf[pos++] = '\n';
	}
	buf[pos] = 0;
	LOG("%s", buf);
}


static int OBEXSendPacket(BtSppConnection& conn, u16 maxPacketSize, uint* fileOffset,
													const Array<char>& file, const Array<u16>& name,
													std::string mimeType);
static int OBEXDisconnect(BtSppConnection& conn);

//TODO: implement OBEXDisconnect().
int sendObject(const MABtAddr& address, const Array<char>& file, const Array<u16>& name,
							 int port, int maxPacketSize)
{
	LOG("Connecting...\n");
	Smartie<BtSppConnection> conn(createBtSppConnection(&address, port));
	TEST_LEZ(conn->connect());
	LOG("Connected!\n");
	u16 shortMPS = (u16)maxPacketSize;
	TEST_LEZ(OBEXConnect(*conn, &shortMPS, file.size()));
	TEST_LEZ(OBEXPut(*conn, shortMPS, file, name));
	TEST_LEZ(OBEXDisconnect(*conn));
	return 1;
}

static int OBEXDisconnect(BtSppConnection& conn) {
	LOG("OBEXDisconnect...\n");

	//send packet
	byte packet[3];
	packet[0] = 0x81;			// Disconnect
	packet[1] = 0x00;			// Packetlength Hi Byte
	packet[2] = 0x03;			// Packetlength Lo Byte
	TEST_LEZ(conn.write(packet, sizeof(packet)));

	//listen for server response
	byte ReceiveBufferA[3];
	TEST_LEZ(conn.readFully(ReceiveBufferA, 3));
	dumpPacket(ReceiveBufferA, 3);

	if(ReceiveBufferA[0] == 0xa0) {
		LOG("Disconnect successful.\n");
		return 1;
	} else {
		LOG("Unknown reply: 0x%02X\n", ReceiveBufferA[0]);
		return CONNERR_OBEX;
	}
}

//may decrease *maxPacketSize to match the remote device
//added the optional LENGTH header here. didn't do anything for the k800i prototype.
static int OBEXConnect(BtSppConnection& conn, u16* maxPacketSize, int fileSize) {
	LOG("OBEXConnect...\n");
	//send client request
	byte ConnectPacket[0x11];

	ConnectPacket[0] = 0x80;			// Connect
	ConnectPacket[1] = 0x00;			// Packetlength Hi Byte
	ConnectPacket[2] = 0x11;			// Packetlength Lo Byte
	ConnectPacket[3] = 0x10;			// Obex v1
	ConnectPacket[4] = 0x00;			// no flags
	ConnectPacket[5] = 0x20;			// 8k max packet size Hi Byte
	ConnectPacket[6] = 0x00;			// 8k max packet size Lo Byte

	ConnectPacket[7] = 0xC0;			// Optional Count header
	ConnectPacket[8] = 0;
	ConnectPacket[9] = 0;
	ConnectPacket[0xA] = 0;
	ConnectPacket[0xB] = 1;

	ConnectPacket[0xC] = 0xC3;			// Optional Length header
	ConnectPacket[0xD] = (byte)((fileSize >> 24) & 0xFF);
	ConnectPacket[0xE] = (byte)((fileSize >> 16) & 0xFF);
	ConnectPacket[0xF] = (byte)((fileSize >> 8) & 0xFF);
	ConnectPacket[0x10] = (byte)(fileSize & 0xFF);

	dumpPacket(ConnectPacket, sizeof(ConnectPacket));
	TEST_LEZ(conn.write(ConnectPacket, sizeof(ConnectPacket)));

	//listen for server response
	byte ReceiveBufferA[3];
	TEST_LEZ(conn.readFully(ReceiveBufferA, 3));
	dumpPacket(ReceiveBufferA, 3);

	if (ReceiveBufferA[0] == 160) // 0xa0
	{ 
		//success, decode rest of packet
		int plength = (0xff * ReceiveBufferA[1]) + ReceiveBufferA[2]; //length of packet is...
		DUMPINT(plength);

		//listen for rest of packet
		Array<byte> ReceiveBufferB(plength-3);
		TEST_LEZ(conn.readFully(ReceiveBufferB, plength-3));
		dumpPacket(ReceiveBufferB, plength-3);

		byte obver = ReceiveBufferB[0]; //server obex version (16 = v1.0)
		byte cflags = ReceiveBufferB[1]; //connect flags
		int maxpack = (0xff * (int)ReceiveBufferB[2]) + ReceiveBufferB[3]; //max packet size

		DUMPHEX(obver);
		DUMPHEX(cflags);
		DUMPINT(maxpack);
		*maxPacketSize = MIN(*maxPacketSize, maxpack);

		return 1;
	}	else {
		LOG("Unknown reply: 0x%02X\n", ReceiveBufferA[0]);
		return CONNERR_OBEX;
	}
}

static int OBEXPut(BtSppConnection& conn, u16 maxPacketSize,
									 const Array<char>& file, const Array<u16>& name)
{
	/*if(file.size() <= maxPacketSize) {	//if file fits in single packet
		TEST(OBEXSendSinglePacket(conn, maxPacketSize, file, name, ""));
	} else {
		uint pos = 0;
		TEST(OBEXSendFirstPacket(conn, maxPacketSize, file, name, "", &pos));
		while(pos + maxPacketSize < file.size()) {
			TEST(OBEXSendMiddlePacket(conn, maxPacketSize, file, &pos));
		}
		TEST(OBEXSendLastPacket(conn, maxPacketSize, file, &pos));
	}*/
	uint pos = 0;
	TEST_LEZ(OBEXSendPacket(conn, maxPacketSize, &pos, file, name, ""));
	while(pos < file.size()) {
		TEST_LEZ(OBEXSendPacket(conn, maxPacketSize, &pos, file, name, ""));
	}
	return 1;
}

/*static int OBEXSendSinglePacket(BtSppConnection& conn, u16 maxPacketSize,
															 const Array<char>& file, const Array<u16>& name,
															 std::string mimeType)
{
	uint offset=0;
	return OBEXSendPacket(conn, maxPacketSize, &offset, file, name, mimeType, true);
}

static int OBEXSendFirstPacket(BtSppConnection& conn, u16 maxPacketSize,
															 const Array<char>& file, const Array<u16>& name,
															 std::string mimeType, uint* pos)
{
	return OBEXSendPacket(conn, maxPacketSize, pos, file, name, mimeType, false);
}

static int OBEXSendMiddlePacket(BtSppConnection& conn, u16 maxPacketSize,
																const Array<char>& file, uint* pos)
{
	Array<u16> dummy(0);
	return OBEXSendPacket(conn, maxPacketSize, pos, file, dummy, "", false);
}

static int OBEXSendLastPacket(BtSppConnection& conn, u16 maxPacketSize,
																const Array<char>& file, uint* pos)
{
	Array<u16> dummy(0);
	return OBEXSendPacket(conn, maxPacketSize, pos, file, dummy, "", true);
}*/

//name and mimeType are optional, may be size 0.
static int OBEXSendPacket(BtSppConnection& conn, u16 maxPacketSize, uint* fileOffset,
													const Array<char>& file, const Array<u16>& name,
													std::string mimeType)
{
	printf("OBEXSendPacket 0x%x 0x%x\n", maxPacketSize, *fileOffset);
	//send client request

	int i;
	int offset;
	uint packetsize;
	bool firstPacket = *fileOffset == 0;

	int tTypeLen = mimeType.length();
	int typeheadsize;
	int typesizeHi;
	int typesizeLo;

	int tNameLength = name.size();
	int nameheadsize; 
	int namesizeHi;
	int namesizeLo;

	packetsize = 3;

	//Name Header
	if(tNameLength && firstPacket) {
		nameheadsize = (3 + (tNameLength*2) + 2); 
		namesizeHi = (nameheadsize & 0xff00) >> 8;
		namesizeLo = nameheadsize & 0x00ff;
		packetsize += nameheadsize;
	}

	//Type Header
	if(tTypeLen && firstPacket) {
		typeheadsize = 3 + tTypeLen+1;
		typesizeHi = (typeheadsize & 0xff00) >> 8;
		typesizeLo = typeheadsize & 0x00ff;
		packetsize += typeheadsize;
	}

	//Length Header
	if(firstPacket) {
		packetsize += 5;
	}

	//Body
	int fileLen = MIN(maxPacketSize - packetsize - 3, file.size() - *fileOffset);
	int fileheadsize = 3 + fileLen;
	int filesizeHi = (fileheadsize & 0xff00) >> 8;
	int filesizeLo = fileheadsize & 0x00ff;

	packetsize += fileheadsize;
	bool last = *fileOffset + fileLen == file.size();

	DEBUG_ASSERT(packetsize <= maxPacketSize);

	int packetsizeHi = (packetsize & 0xff00) >> 8;
	int packetsizeLo = packetsize & 0x00ff;

	Array<byte> tSendByte(packetsize);

	//PUT-final Header
	tSendByte[0] = last ? 0x82 : 0x02;					// Request type e.g. PUT (final)
	tSendByte[1] = byte(packetsizeHi);					// Packetlength Hi
	tSendByte[2] = byte(packetsizeLo);					// Packetlength Lo

	offset = 2;

	if(tNameLength && firstPacket) {
		//Name Header
		tSendByte[offset+1] = 0x01;								// HI for Name header		
		tSendByte[offset+2] = byte(namesizeHi);		// Length of Name header (2 bytes per char)
		tSendByte[offset+3] = byte(namesizeLo);		// Length of Name header (2 bytes per char)

		// Name + \0 in unicode
		memcpy(tSendByte + offset + 4, name, tNameLength*2);

		offset = offset + 3 + (tNameLength*2);
		tSendByte[offset+1] = 0x00;								// null term
		tSendByte[offset+2] = 0x00;								// null term

		offset = offset + 2;
	}

	if(tTypeLen && firstPacket) {
		//Type Header
		tSendByte[offset+1] = 0x42;								// HI for Type Header
		tSendByte[offset+2] = byte(typesizeHi);		// Length of Type Header
		tSendByte[offset+3] = byte(typesizeLo);		// Length of Type Header

		for (i=0;i<=(tTypeLen-1);i++) 
		{
			tSendByte[offset+4+i] = mimeType[i];
		}
		tSendByte[offset+3+tTypeLen+1] = 0x00;		// null terminator

		offset = offset+3+tTypeLen+1;
	}

	//Length Header
	if(firstPacket) {
		tSendByte[offset+1] = 0xC3;								// HI for Length Header
		int fileSize = file.size();
		DUMPHEX(fileSize);
		tSendByte[offset+2] = (byte)((fileSize >> 24) & 0xFF);
		tSendByte[offset+3] = (byte)((fileSize >> 16) & 0xFF);
		tSendByte[offset+4] = (byte)((fileSize >> 8) & 0xFF);
		tSendByte[offset+5] = (byte)(fileSize & 0xFF);
		offset += 5;
	}

	//Body
	tSendByte[offset+1] = last ? 0x49 : 0x48;		//HI (End of) Body
	tSendByte[offset+2] = byte(filesizeHi);			//
	tSendByte[offset+3] = byte(filesizeLo);			//1k payload + 3 for HI header

	dumpPacket(tSendByte, offset + 4);

	for (i=0;i<=(fileLen-1);i++) 
	{
		tSendByte[offset+4+i] = file[*fileOffset + i];
	}
	//tSendByte[offset+4+fileLen] = 0x00;					// null terminator

	offset = offset+3+fileLen;

	TEST_LEZ(conn.write(tSendByte, tSendByte.size()));
	printf("Sent %i bytes (%i bytes of file)\n", tSendByte.size(), fileLen);
	*fileOffset += fileLen;

	//listen for server response

	//TODO: can hang here forever waiting response...

	byte tArray4[3];
	TEST_LEZ(conn.readFully(tArray4, 3));
	dumpPacket(tArray4, 3);

	DUMPHEX(tArray4[0]);
	if((tArray4[0] & 0x7F) >= 0x30) {
		return CONNERR_OBEX;
	}

	return 1;
}
