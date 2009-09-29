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

#include "config_platform.h"

#include "helpers.h"

#define USE_VAR_INT
#define STATIC //static
#define REGS_128	//256

#ifdef REGS_128
#define NREGS 128
#define FETCH_CODEBYTE_FAST mem_cs[IP++]
#else
#define NREGS 256
#error This looks bad...
#define FETCH_CODEBYTE_FAST FETCH_CODEBYTE
#endif	//REGS_128

#define FETCH_CODEBYTE (((int)mem_cs[IP++]) & 0x0ff)

#define OPC(opcode)	case opcode: DEBUG(""+Integer.toHexString(IP-1)+": "+opcode+" "+#opcode);
#define EOP	DEBUG("\n"); break

#define REG(nn)	regs[nn]

#define	RD	regs[rd]
#define	RS	regs[rs]
//#define	RDU	(*(unsigned long*)&regs[rd])
//#define	RSU	(*(unsigned long*)&regs[rs])

//#define IMMU	((unsigned long) imm32)
#define IMM	((int) imm32)

#define G_CLOSING mCanvas.mClosing
#define JMP_ARG(arg) /*if(timeToClose()) return IP; */IP = arg;

#define	JMP_IMM	JMP_ARG(IMM);
#define	JMP_RD	JMP_ARG(RD);

#define	CALL_IMM	REG(REG_rt) = (int)IP; JMP_IMM;
#define	CALL_RD		REG(REG_rt) = (int)IP; JMP_RD;

#ifdef _DEBUG
#define FETCH_RD	rd = FETCH_CODEBYTE_FAST; DEBUG(" rd"+rd+"("); _debug_hex(REG(rd)); DEBUG(")");
#define FETCH_RS	rs = FETCH_CODEBYTE_FAST; DEBUG(" rs"+rs+"("); _debug_hex(REG(rs)); DEBUG(")");
#else
#define FETCH_RD	rd = FETCH_CODEBYTE_FAST;
#define FETCH_RS	rs = FETCH_CODEBYTE_FAST;
#endif	//_DEBUG


#ifdef USE_VAR_INT
#define FETCH_INT_BASE imm32 = FETCH_CODEBYTE_FAST; if(imm32<0)\
 {imm32=((imm32&127)<<8)+FETCH_CODEBYTE;}
#define FETCH_CONST FETCH_INT_BASE DEBUG(" c["+imm32+"]");\
	imm32=mem_cp[imm32]; DEBUG("("+imm32+")");
#define FETCH_INT FETCH_INT_BASE DEBUG(" i"+imm32);

#else

#define FETCH_CONST	imm32 = FETCH_CODEBYTE; imm32=(imm32<<8)+FETCH_CODEBYTE;\
	imm32=mem_cp[imm32]; DEBUG(" c["+imm32+"]");
#define FETCH_INT imm32 = FETCH_CODEBYTE; imm32=(imm32<<8)+FETCH_CODEBYTE; DEBUG(" i"+imm32);
#endif	//USE_VAR_INT


#define FETCH_IMM16	imm32 = FETCH_CODEBYTE << 8; imm32 += FETCH_CODEBYTE;\
	DEBUG(" m"+imm32+"(0x"+Integer.toHexString(imm32)+")");

#define FETCH_IMM24	imm32 = FETCH_CODEBYTE << 16; imm32 += FETCH_CODEBYTE << 8;\
	imm32 += FETCH_CODEBYTE; DEBUG(" i"+imm32+"(0x"+Integer.toHexString(imm32)+")");

/*#define FETCH_IMM32	imm32 = FETCH_CODEBYTE_FAST << 24; imm32 += FETCH_CODEBYTE << 16;\
	imm32 += FETCH_CODEBYTE << 8;	imm32 += FETCH_CODEBYTE;\
	DEBUG(" m"+imm32+"(0x"+Integer.toHexString(imm32)+")");*/


#define FETCH_IMM8	imm32 = FETCH_CODEBYTE; DEBUG(" n"+imm32);

#define FETCH_RD_RS			FETCH_RD FETCH_RS
#define FETCH_RD_CONST		FETCH_RD FETCH_CONST
#define FETCH_RD_RS_CONST	FETCH_RD FETCH_RS FETCH_CONST
#define FETCH_RD_RS_ADDR	FETCH_RD FETCH_RS FETCH_IMM16


// !!>> Added ARH - pls check 16-01-2007
#define FETCH_RD_RS_ADDR24	FETCH_RD FETCH_RS FETCH_IMM24
// !!<<

#define FETCH_RD_IMM8		FETCH_RD FETCH_IMM8

#define UINT(a) ((long)(a) & 0x0ffffffffL)
#define OPU(a, oper, b) (UINT(a) oper UINT(b))

#define ARITH(a, oper, b) DEBUG("\t"+(a)+" "+#oper+" "+(b)+" = "); a oper##= (b);\
	DEBUG(""+(a));
#define ARITHU(a, oper, b) DEBUG("\t"+UINT(a)+" "+#oper+" "+UINT(b)+" = ");\
	a = (int)OPU(a, oper, b); DEBUG(""+UINT(a));

#define DIVIDE(a, b) if((b) == 0) BIG_PHAT_ERROR; else { ARITH(a, /, b); }
#define DIVIDEU(a, b) if((b) == 0) BIG_PHAT_ERROR; else { ARITHU(a, /, b); }


#define RBYTE(a, ret) RBYTE_BASE(a, ret, byte)
#define RCHAR(a, ret) RBYTE_BASE(a, ret, char)

#ifdef DEBUG_MEMORY

#define RBYTE_BASE(a, ret, type) { ret = (type)CORE.rbyte(a); }
#define WBYTE(a, b) CORE.wbyte(a,b)
#define WBYTE_RAW(a, b) CORE.wbyte(a,(byte)b)
#define RINT(addr) CORE.rint(addr)
#define WINT(addr, i) CORE.wint(addr, i)
#define RSHORT(a, ret) { ret = (short)CORE.rshort(a); }
#define WSHORT(a, s) CORE.wshort(a,s)

#else	//DEBUG_MEMORY

#define RBYTE_BASE(a, ret, type) { \
	int addr = (a);\
	ret = (type)((mem_ds[INTPART(addr)] >> (BYTEPART(addr) << 3)) & 0x0ff); \
}

#define WBYTE_RAW(a, b) {\
	int addr = (a);\
	int shift = BYTEPART(addr) << 3;\
	mem_ds[INTPART(addr)] = (mem_ds[INTPART(addr)] & ~(0x0ff << shift)) |\
	(((b) << shift));\
}

#define WBYTE(a, b) WBYTE_RAW(a, (((int)b) & 0x0ff))

#define RINT(addr) mem_ds[INTPART(addr)]
#define WINT(addr, i) mem_ds[INTPART(addr)] = i;

#define RSHORT(a, ret) {\
	int addr = (a);\
	ret = (short)((mem_ds[INTPART(addr)] >> (BYTEPART(addr) << 3)) & (0x0ffff));\
		}

#define WSHORT(a, s) {\
	int addr = (a);\
	int shift = BYTEPART(addr) << 3;\
	mem_ds[INTPART(addr)] = (mem_ds[INTPART(addr)] & ~(0x0ffff << shift)) |\
	((((int)s) & (0x0ffff)) << shift);\
	}

#endif	//DEBUG_MEMORY
