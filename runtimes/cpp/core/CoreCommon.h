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

// This file is shared by the debugger.

#ifndef _CORE_COMMON_H_
#define _CORE_COMMON_H_

#define DATA_MEMORY_START 0x00000000
#define INSTRUCTION_MEMORY_START 0x40000000
#define ADDRESS_MASK 0x3fffffff

//****************************************
//		 Instruction descripters
//****************************************
#define INSTRUCTIONS(m)\
	m(PUSH)\
	m(POP)\
	m(CALL)\
	m(CALLI)\
	m(LDB)\
	m(STB)\
	m(LDH)\
	m(STH)\
	m(LDW)\
	m(STW)\
	m(LDI)\
	m(LDR)\
	m(ADD)\
	m(ADDI)\
	m(MUL)\
	m(MULI)\
	m(SUB)\
	m(SUBI)\
	m(AND)\
	m(ANDI)\
	m(OR)\
	m(ORI)\
	m(XOR)\
	m(XORI)\
	m(DIVU)\
	m(DIVUI)\
	m(DIV)\
	m(DIVI)\
	m(SLL)\
	m(SLLI)\
	m(SRA)\
	m(SRAI)\
	m(SRL)\
	m(SRLI)\
	m(NOT)\
	m(NEG)\
	m(RET)\
	m(JC_EQ)\
	m(JC_NE)\
	m(JC_GE)\
	m(JC_GEU)\
	m(JC_GT)\
	m(JC_GTU)\
	m(JC_LE)\
	m(JC_LEU)\
	m(JC_LT)\
	m(JC_LTU)\
	m(JPI)\
	m(JPR)\
	m(XB)\
	m(XH)\
	m(SYSCALL)\
	m(CASE)\
	m(FAR)

#define ENUM_INSTRUCTION_ELEM(inst) _ ## inst,
enum
{
	_NUL = 0,
	INSTRUCTIONS(ENUM_INSTRUCTION_ELEM)
	_ENDOP
};

//****************************************
// 			Register enums
//****************************************

enum
{
	REG_zero,
	REG_sp,
	REG_rt,
	REG_fr,
	REG_d0,
	REG_d1,
	REG_d2,
	REG_d3,
	REG_d4,
	REG_d5,
	REG_d6,
	REG_d7,
	REG_i0,
	REG_i1,
	REG_i2,
	REG_i3,

	REG_r0,
	REG_r1,
	REG_r2,
	REG_r3,
	REG_r4,
	REG_r5,
	REG_r6,
	REG_r7,
	REG_r8,
	REG_r9,
	REG_r10,
	REG_r11,
	REG_r12,
	REG_r13,
	REG_r14,
	REG_r15
};

#endif	//_CORE_COMMON_H_
