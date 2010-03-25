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
//					       			Instruction Parser Tables
//*********************************************************************************************

//****************************************
// 				Constants
//****************************************

/*

#define fetch_d 	1		// rd
#define fetch_a		2		// address
#define fetch_s 	4		// rs
#define fetch_i 	8		// const imm
#define fetch_j 	16		// imm 8
#define fetch_k 	32		// syscall imm
#define fetch_f		64		// far address
#define fetch_c		128		// case

#define fetch_ds	(fetch_d | fetch_s)
#define fetch_dsa	(fetch_d | fetch_s | fetch_a)
#define fetch_dsf	(fetch_d | fetch_s | fetch_f)
#define fetch_dsi	(fetch_d | fetch_s | fetch_i)
#define fetch_di	(fetch_d | fetch_i)
#define fetch_dj	(fetch_d | fetch_j)

*/

// New version

			// Far, Opcode,    Instruction string,	Fetch flags

INSTR_MACRO(	0, _NOP,       ";NOP",			    "nop",		0			,inst_ignor				)
INSTR_MACRO(	0, _PUSH,      "PUSH x",			"push",		fetch_ds	,inst_push				)
INSTR_MACRO(	0, _POP,       "POP y",				"pop",		fetch_ds	,inst_pop				)
INSTR_MACRO(	0, _CALL,      "CALL d",			"call",		fetch_d		,inst_ignor				)
INSTR_MACRO(	0, _CALLI,     "CALL a",			"call",		fetch_a		,inst_ignor				)
INSTR_MACRO(	0, _LDB,       "LD.B d,[m]",		"",			fetch_dsi	,inst_rd_unk			)
INSTR_MACRO(	0, _STB,       "LD.B [n],s",		"",			fetch_dsi	,inst_ignor				)
INSTR_MACRO(	0, _LDH,       "LD.H d,[m]",		"",			fetch_dsi	,inst_rd_unk			)
INSTR_MACRO(	0, _STH,       "LD.H [n],s",		"",			fetch_dsi	,inst_ignor				)
INSTR_MACRO(	0, _LDW,       "LD d,[m]",			"",			fetch_dsi	,inst_rd_unk			)
INSTR_MACRO(	0, _STW,       "LD [n],s",			"",			fetch_dsi	,inst_ignor				)
INSTR_MACRO(	0, _LDI,       "LD d,i",			"",			fetch_di	,inst_rd_imm			)
INSTR_MACRO(	0, _LDR,       "LD d,q",			"",			fetch_ds	,inst_rd_rs				)
INSTR_MACRO(	0, _ADD,       "ADD d,q",			"",			fetch_ds	,inst_arith_rd_rs		)
INSTR_MACRO(	0, _ADDI,      "ADD d,i",			"",			fetch_di	,inst_arith_rd_imm		)
INSTR_MACRO(	0, _MUL,       "MUL d,q",			"",			fetch_ds	,inst_arith_rd_rs		)
INSTR_MACRO(	0, _MULI,      "MUL d,i",			"",			fetch_di	,inst_arith_rd_imm		)
INSTR_MACRO(	0, _SUB,       "SUB d,q",			"",			fetch_ds	,inst_arith_rd_rs		)
INSTR_MACRO(	0, _SUBI,      "SUB d,i",			"",			fetch_di	,inst_arith_rd_imm		)
INSTR_MACRO(	0, _AND,       "AND d,q",			"",			fetch_ds	,inst_arith_rd_rs		)
INSTR_MACRO(	0, _ANDI,      "AND d,i",			"",			fetch_di	,inst_arith_rd_imm		)
INSTR_MACRO(	0, _OR,        "OR d,q", 			"",			fetch_ds	,inst_arith_rd_rs		)
INSTR_MACRO(	0, _ORI,       "OR d,i", 			"",			fetch_di	,inst_arith_rd_imm		)
INSTR_MACRO(	0, _XOR,       "XOR d,q",			"",			fetch_ds	,inst_arith_rd_rs		)
INSTR_MACRO(	0, _XORI,      "XOR d,i",			"",			fetch_di	,inst_arith_rd_imm		)
INSTR_MACRO(	0, _DIVU,      "DIVU d,q",			"",			fetch_ds	,inst_arith_rd_rs		)
INSTR_MACRO(	0, _DIVUI,     "DIVU d,i",			"",			fetch_di	,inst_arith_rd_imm		)
INSTR_MACRO(	0, _DIV,       "DIV d,q",			"",			fetch_ds	,inst_arith_rd_rs		)
INSTR_MACRO(	0, _DIVI,      "DIV d,i",			"",			fetch_di	,inst_arith_rd_imm		)
INSTR_MACRO(	0, _SLL,       "SLL d,s",			"",			fetch_ds	,inst_ignor	)
INSTR_MACRO(	0, _SLLI,      "SLL d,j",			"",			fetch_dj	,inst_ignor	)
INSTR_MACRO(	0, _SRA,       "SRA d,s",			"",			fetch_ds	,inst_ignor	)
INSTR_MACRO(	0, _SRAI,      "SRA d,j",			"",			fetch_dj	,inst_ignor	)
INSTR_MACRO(	0, _SRL,       "SRL d,s",			"",			fetch_ds	,inst_ignor	)
INSTR_MACRO(	0, _SRLI,      "SRL d,j",			"",			fetch_dj	,inst_ignor	)
INSTR_MACRO(	0, _NOT,       "NOT d,s",			"",			fetch_ds	,inst_ignor	)
INSTR_MACRO(	0, _NEG,       "NEG d,s",			"",			fetch_ds	,inst_ignor	)
INSTR_MACRO(	0, _RET,	   "RET",				"",			0			,inst_ignor	)
INSTR_MACRO(	0, _JC_EQ,     "JC EQ,d,s,a",		"",			fetch_dsa	,inst_ignor	)
INSTR_MACRO(	0, _JC_NE,     "JC NE,d,s,a",		"",			fetch_dsa	,inst_ignor	)
INSTR_MACRO(	0, _JC_GE,     "JC GE,d,s,a",		"",			fetch_dsa	,inst_ignor	)
INSTR_MACRO(	0, _JC_GEU,    "JC GEU,d,s,a",		"",			fetch_dsa	,inst_ignor	)
INSTR_MACRO(	0, _JC_GT,     "JC GT,d,s,a",		"",			fetch_dsa	,inst_ignor	)
INSTR_MACRO(	0, _JC_GTU,    "JC GTU,d,s,a",		"",			fetch_dsa	,inst_ignor	)
INSTR_MACRO(	0, _JC_LE,     "JC LE,d,s,a",		"",			fetch_dsa	,inst_ignor	)
INSTR_MACRO(	0, _JC_LEU,    "JC LEU,d,s,a",		"",			fetch_dsa	,inst_ignor	)
INSTR_MACRO(	0, _JC_LT,     "JC LT,d,s,a",		"",			fetch_dsa	,inst_ignor	)
INSTR_MACRO(	0, _JC_LTU,    "JC LTU,d,s,a",		"",			fetch_dsa	,inst_ignor	)
INSTR_MACRO(	0, _JPI,       "JP a",				"",			fetch_a		,inst_ignor	)
INSTR_MACRO(	0, _JPR,       "JP d",				"",			fetch_d		,inst_ignor	)
INSTR_MACRO(	0, _XB,        "XB d,s",			"",			fetch_ds	,inst_ignor	)
INSTR_MACRO(	0, _XH,        "XH d,s",			"",			fetch_ds	,inst_ignor	)
INSTR_MACRO(	0, _SYSCALL,   "SYSCALL k",			"",			fetch_k		,inst_ignor	)
INSTR_MACRO(	0, _CASE,      "CASE d,c",			"",			fetch_dc	,inst_ignor	)
INSTR_MACRO(	0, _FAR,        "FAR",				"",			0		    ,inst_ignor	)

//Far Instructions
/*
INSTR_MACRO(	1, _JC_EQ,     "FAR JC EQ d,s,a",	fetch_dsf	)
INSTR_MACRO(	1, _JC_NE,     "FAR JC NE d,s,a",	fetch_dsf	)
INSTR_MACRO(	1, _JC_GE,     "FAR JC GE d,s,a",	fetch_dsf	)
INSTR_MACRO(	1, _JC_GEU,    "FAR JC GEU d,s,a",	fetch_dsf	)
INSTR_MACRO(	1, _JC_GT,     "FAR JC GT d,s,a",	fetch_dsf	)
INSTR_MACRO(	1, _JC_GTU,    "FAR JC GTU d,s,a",	fetch_dsf	)
INSTR_MACRO(	1, _JC_LE,     "FAR JC LE d,s,a",	fetch_dsf	)
INSTR_MACRO(	1, _JC_LEU,    "FAR JC LEU d,s,a",	fetch_dsf	)
INSTR_MACRO(	1, _JC_LT,     "FAR JC LT d,s,a",	fetch_dsf	)
INSTR_MACRO(	1, _JC_LTU,    "FAR JC LTU d,s,a",	fetch_dsf	)
INSTR_MACRO(	1, _JPI,       "FAR JPI a",			fetch_f		)
INSTR_MACRO(	1, _CALLI,     "FAR CALLI a",		fetch_f		)
INSTR_MACRO(  -1, 0,          "last",				0			)
*/
