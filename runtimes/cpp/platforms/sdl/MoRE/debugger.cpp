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

#include "../config_platform.h"
#include <helpers/helpers.h>
#include <core/debugger.h>
#include <SDL/SDL_net.h>

namespace Debugger {
	IPaddress ip;
	TCPsocket server_tcpsock = 0;
	TCPsocket client_tcpsock = 0;
	byte *gMemDS;
	unsigned int gSizeMemDS;
	byte *gMemCS;
	unsigned int gSizeMemCS;
	int *gRegs;
	unsigned int *gPc;

	bool init(int port, int *regs, byte *mem_ds, unsigned int size_mem_ds, byte *mem_cs,
		unsigned int size_mem_cs, unsigned int *pc)
	{
		/* Initialize the network */
		if ( SDLNet_Init() < 0 )
			exit(1);

		if(SDLNet_ResolveHost(&ip,NULL,port) == -1) {
			//printf("SDLNet_ResolveHost: %s\n", SDLNet_GetError());
			return false;
		}

		server_tcpsock = SDLNet_TCP_Open(&ip);
		if(!server_tcpsock) {
			//printf("SDLNet_TCP_Open: %s\n", SDLNet_GetError());
			return false;
		}

		gRegs = regs;
		gMemDS = mem_ds;
		gSizeMemDS = size_mem_ds;
		gMemCS = mem_cs;
		gSizeMemDS = size_mem_cs;
		gPc = pc;

		return true;
	}

	int readBytes(void *b, int num) {
		byte* bp = (byte*)b;
		int result = 0;
		while(result!=num) {
			int res = SDLNet_TCP_Recv(client_tcpsock,bp,num-result);
			if(res < 0)
				return res;
			result += res;
			bp += res;
		}
		return result;
	}

	void waitForClient() {
		if(client_tcpsock) return;
		while(!(client_tcpsock=SDLNet_TCP_Accept(server_tcpsock)));
	}

	bool readUnsignedInt(unsigned int &i) {
		return readBytes(&i, 4)>0;
	}


	bool sendUnsignedInt(unsigned int i) {
		int result;
		result=SDLNet_TCP_Send(client_tcpsock,&i,4);
		return result>0;
	}

	bool readInt(int &i) {
		return readBytes(&i, 4)>0;
	}


	bool sendInt(int i) {
		int result;
		result=SDLNet_TCP_Send(client_tcpsock,&i,4);
		return result>0;
	}

	bool readByte(unsigned char &i) {
		return readBytes(&i, 1)>0;
	}

	bool sendByte(unsigned char i) {
		int result;
		result=SDLNet_TCP_Send(client_tcpsock,&i,1);
		return result>0;
	}

	void close() {
//		if(client_tcpsock) 
//			SDLNet_TCP_Close(client_tcpsock);
		if(server_tcpsock)
			SDLNet_TCP_Close(server_tcpsock);
	}

	enum DebugCommands {
		CMD_NULL		= 0,
		CMD_ECHO		= 1,
		CMD_GET_INFO	= 2,
		CMD_GET_REG		= 3,
		CMD_SET_REG		= 4,
		CMD_GET_MEM		= 5,
		CMD_SET_MEM		= 6,
		CMD_RUN			= 7,
		CMD_QUIT		= 8
	};

	BreakResult brk() {
		waitForClient();

		while(1) {
			unsigned int command;
			if(!readUnsignedInt(command)) {
				return BRK_READ_ERR;
			}

			switch(command) {
				case CMD_NULL:
					break;
				case CMD_ECHO:
					{
						unsigned int val;
						if(!readUnsignedInt(val)) {
							return BRK_READ_ERR;
						}

						if(!sendUnsignedInt(val)) {
							return BRK_SEND_ERR;
						}
					}
					break;

// returns: platformid 
//			total size of memory allocated for data
//			total size of memory allocated for code
				case CMD_GET_INFO:
					if(!sendUnsignedInt(1)) {
						return BRK_SEND_ERR;
					}
					if(!sendUnsignedInt(gSizeMemDS)) {
						return BRK_SEND_ERR;
					}
					if(!sendUnsignedInt(gSizeMemCS)) {
						return BRK_SEND_ERR;
					}
					break;

// returns: data of all registers
				case CMD_GET_REG:
					for(int i = 0; i < 31; i++) 
						if(!sendInt(gRegs[i]))
							return BRK_SEND_ERR;
					if(!sendUnsignedInt(*gPc))
						return BRK_SEND_ERR;
					break;

// recieves: data of all registers
				case CMD_SET_REG:
					for(int i = 0; i < 32; i++) 
						if(!readInt(gRegs[i]))
							return BRK_READ_ERR;
					if(!readUnsignedInt(*gPc))
						return BRK_READ_ERR;
					break;

// recieves:	address (1xxxxxxx=data, 2xxxxxxx=code)
//				length
// returns:		length amount of bytes
				case CMD_GET_MEM:
					{
						unsigned int address, length;
						if(!readUnsignedInt(address)) {
							return BRK_READ_ERR;
						}

						if(!readUnsignedInt(length)) {
							return BRK_READ_ERR;
						}

						int seg = (address)>>28;
						address&=0x0fffffff;
						switch(seg) {
							case 1:
								while(length--) 
									if(!sendByte(gMemDS[address++]))
										return BRK_SEND_ERR;
								break;
							case 2:
								while(length--) 
									if(!sendByte(gMemCS[address++]))
										return BRK_SEND_ERR;
								break;
						}
					}
					break;

// recieves:	address (1xxxxxxx=data, 2xxxxxxx=code)
//				length
				case CMD_SET_MEM:
					{
						unsigned int address, length;
						if(!readUnsignedInt(address)) {
							return BRK_READ_ERR;
						}
						if(!readUnsignedInt(length)) {
							return BRK_READ_ERR;
						}
						int seg = (address)>>28;
						address&=0x0fffffff;
						switch(seg) {
							case 1:
								while(length--) 
									if(!readByte(gMemDS[address++]))
										return BRK_READ_ERR;
								break;
							case 2:
								while(length--) 
									if(!readByte(gMemCS[address++]))
										return BRK_READ_ERR;
								break;
						}
					}
					break;
				case CMD_RUN:
					return BRK_CONTINUE;	
				case CMD_QUIT:
					return BRK_QUIT;
			}
		}
	}
};
