/* Copyright 2013 David Axmark

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

	http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
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
