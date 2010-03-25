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

//*********************************************************************************************
//				       			MoSync Code Tools API
//*********************************************************************************************

#ifdef __cplusplus
extern "C" {
#endif

//****************************************
// 			Opcode Constants
//****************************************

#define fetch_d 1		// rd
#define fetch_a	2		// address
#define fetch_s 4		// rs
#define fetch_i 8		// const imm
#define fetch_j 16		// imm 8
#define fetch_k 32		// syscall imm
#define fetch_f	64		// far address
#define fetch_c	128		// Case

#define fetch_ds	(fetch_d | fetch_s)
#define fetch_dsa	(fetch_d | fetch_s | fetch_a)
#define fetch_dsf	(fetch_d | fetch_s | fetch_f)
#define fetch_dsi	(fetch_d | fetch_s | fetch_i)
#define fetch_di	(fetch_d | fetch_i)
#define fetch_dj	(fetch_d | fetch_j)
#define fetch_dc	(fetch_d | fetch_c)

//****************************************
//			Extra Types
//****************************************

typedef unsigned char	uchar;
typedef unsigned short	ushort;
typedef unsigned int	uint;

//****************************************
//			Opcode Info
//****************************************

typedef struct
{
	const char *str;
	int flags;
	int farflag;
	int op;
	int rd;
	int rs;
	int imm;
	int rip;
	int len;
} DisasInfo;

void DisasGenReg();
void DisasInit(char *codemem, int *constpool);
const uchar * DisasDecode(DisasInfo *thisOpcode, const uchar *code_ip);
int DisasDecodeIP(DisasInfo *thisOpcode, int code_ip);
void DisasEmit(char *out, const char *Template, ...);
char * DisasRegName(int reg, int use_zero);
void DisasString(DisasInfo *thisOpcode, char *out);
int DisasInst(int ip, char *out);

#ifdef __cplusplus
}
#endif
