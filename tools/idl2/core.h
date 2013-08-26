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

//The order of these things is supremely important.
//If any is changed, all must be recompiled.

#define INSTRUCTIONS(m)\
	m(NULOP)\
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
	m(FAR)\
	m(ENDOP)\

#define REGISTERS(m)\
	m(REG_zero)\
	m(REG_sp)\
	m(REG_rt)\
	m(REG_fr)\
	m(REG_d0)\
	m(REG_d1)\
	m(REG_d2)\
	m(REG_d3)\
	m(REG_d4)\
	m(REG_d5)\
	m(REG_d6)\
	m(REG_d7)\
	m(REG_i0)\
	m(REG_i1)\
	m(REG_i2)\
	m(REG_i3)\
	m(REG_r0)\
	m(REG_r1)\
	m(REG_r2)\
	m(REG_r3)\
	m(REG_r4)\
	m(REG_r5)\
	m(REG_r6)\
	m(REG_r7)\
	m(REG_r8)\
	m(REG_r9)\
	m(REG_r10)\
	m(REG_r11)\
	m(REG_r12)\
	m(REG_r13)\
	m(REG_r14)\
	m(REG_r15)
