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

// Instruction definition file for MA Java Conversion system

//-----------------------------------------------
// Disassembler control codes
//-----------------------------------------------
// %b = signed byte
// %w = signed short
// %c = 1 byte constant index
// %C = 2 byte constant index
// %n = unsigned 8 bit number
// %o = offset from pc
//-----------------------------------------------

//-----------------------------------------------
// Instruction info
//-----------------------------------------------
// bit type				meaning
//-----------------------------------------------
// 00. BC_PUSH1			code pushes 1 on stack
// 01. BC_PUSH2			code pushes 2 on stack
// 02. BC_POP1			code pops 1 off stack
// 03. BC_POP2			code pops 2 off stack
// 04. BC_POP3			code pops 2 off stack
// 05. BC_POP4			code pops 2 off stack
// 06. BC_REF			handles a reference
// 07. BC_LOCAL			moves to a local variable
// 08. BC_CONDJMP		code conditional jumps
// 09. BC_JMP			code jumps
// 10. BC_CALL			code calls
// 11. BC_RET			return instruction
// 12. BC_SWITCH		a switch
// 13. BC_UNDEF			undefined instruction
// 14. BC_INVOKE		invoke instruction
// 15. BC_WIDE			can be wide
// 16. BC_BAD			bad opcode
//-----------------------------------------------

//			opcode string			enum			 flag  len perm

INSTRDEF(	"nop",					NOP=0,				0,	1,	0)
INSTRDEF(	"aconst_null",			ACONST_NULL,		0,	1,	0)
INSTRDEF(	"iconst_m1",			ICONST_M1,			0,	1,	0)
INSTRDEF(	"iconst_0",				ICONST_0,			0,	1,	0)
INSTRDEF(	"iconst_1",				ICONST_1,			0,	1,	0)
INSTRDEF(	"iconst_2",				ICONST_2,			0,	1,	0)
INSTRDEF(	"iconst_3",				ICONST_3,			0,	1,	0)
INSTRDEF(	"iconst_4",				ICONST_4,			0,	1,	0)
INSTRDEF(	"iconst_5",				ICONST_5,			0,	1,	0)
INSTRDEF(	"lconst_0",				LCONST_0,			0,	1,	BC_BAD)
INSTRDEF(	"lconst_1",				LCONST_1,			0,	1,	BC_BAD)
INSTRDEF(	"fconst_0",				FCONST_0,			0,	1,	BC_BAD)
INSTRDEF(	"fconst_1",				FCONST_1,			0,	1,	BC_BAD)
INSTRDEF(	"fconst_2",				FCONST_2,			0,	1,	BC_BAD)
INSTRDEF(	"dconst_0",				DCONST_0,			0,	1,	BC_BAD)
INSTRDEF(	"dconst_1",				DCONST_1,			0,	1,	BC_BAD)
INSTRDEF(	"bipush %b",			BIPUSH,				0,	2,	0)
INSTRDEF(	"sipush %w",			SIPUSH,				0,	3,	0)
INSTRDEF(	"ldc %c",				LDC,				0,	2,	0)
INSTRDEF(	"ldc_w %C",				LDC_W,				0,	3,	0)
INSTRDEF(	"ldc2_w %C",			LDC2_W,				0,	3,	BC_BAD)
INSTRDEF(	"iload %n",				ILOAD,				0,	2,	0)
INSTRDEF(	"lload",				LLOAD,				0,	2,	BC_BAD)
INSTRDEF(	"fload ",				FLOAD,				0,	2,	BC_BAD)
INSTRDEF(	"dload",				DLOAD,				0,	2,	BC_BAD)
INSTRDEF(	"aload %n",				ALOAD,				0,	2,	0)
INSTRDEF(	"iload_0",				ILOAD_0,			0,	1,	0)
INSTRDEF(	"iload_1",				ILOAD_1,			0,	1,	0)
INSTRDEF(	"iload_2",				ILOAD_2,			0,	1,	0)
INSTRDEF(	"iload_3",				ILOAD_3,			0,	1,	0)
INSTRDEF(	"lload_0",				LLOAD_0,			0,	1,	BC_BAD)
INSTRDEF(	"lload_1",				LLOAD_1,			0,	1,	BC_BAD)
INSTRDEF(	"lload_2",				LLOAD_2,			0,	1,	BC_BAD)
INSTRDEF(	"lload_3",				LLOAD_3,			0,	1,	BC_BAD)
INSTRDEF(	"fload_0",				FLOAD_0,			0,	1,	BC_BAD)
INSTRDEF(	"fload_1",				FLOAD_1,			0,	1,	BC_BAD)
INSTRDEF(	"fload_2",				FLOAD_2,			0,	1,	BC_BAD)
INSTRDEF(	"fload_3",				FLOAD_3,			0,	1,	BC_BAD)
INSTRDEF(	"dload_0",				DLOAD_0,			0,	1,	BC_BAD)
INSTRDEF(	"dload_1",				DLOAD_1,			0,	1,	BC_BAD)
INSTRDEF(	"dload_2",				DLOAD_2,			0,	1,	BC_BAD)
INSTRDEF(	"dload_3",				DLOAD_3,			0,	1,	BC_BAD)
INSTRDEF(	"aload_0",				ALOAD_0,			0,	1,	0)
INSTRDEF(	"aload_1",				ALOAD_1,			0,	1,	0)
INSTRDEF(	"aload_2",				ALOAD_2,			0,	1,	0)
INSTRDEF(	"aload_3",				ALOAD_3,			0,	1,	0)
INSTRDEF(	"iaload",				IALOAD,				0,	1,	0)
INSTRDEF(	"laload",				LALOAD,				0,	1,	BC_BAD)
INSTRDEF(	"faload",				FALOAD,				0,	1,	BC_BAD)
INSTRDEF(	"daload",				DALOAD,				0,	1,	BC_BAD)
INSTRDEF(	"aaload",				AALOAD,				0,	1,	0)
INSTRDEF(	"baload",				BALOAD,				0,	1,	0)
INSTRDEF(	"caload",				CALOAD,				0,	1,	0)
INSTRDEF(	"saload",				SALOAD,				0,	1,	0)
INSTRDEF(	"istore",				ISTORE,				0,	2,	0)
INSTRDEF(	"lstore",				LSTORE,				0,	2,	BC_BAD)
INSTRDEF(	"fstore",				FSTORE,				0,	2,	BC_BAD)
INSTRDEF(	"dstore",				DSTORE,				0,	2,	BC_BAD)
INSTRDEF(	"astore %n",			ASTORE,				0,	2,	0)
INSTRDEF(	"istore_0",				ISTORE_0,			0,	1,	0)
INSTRDEF(	"istore_1",				ISTORE_1,			0,	1,	0)
INSTRDEF(	"istore_2",				ISTORE_2,			0,	1,	0)
INSTRDEF(	"istore_3",				ISTORE_3,			0,	1,	0)
INSTRDEF(	"lstore_0",				LSTORE_0,			0,	1,	BC_BAD)
INSTRDEF(	"lstore_1",				LSTORE_1,			0,	1,	BC_BAD)
INSTRDEF(	"lstore_2",				LSTORE_2,			0,	1,	BC_BAD)
INSTRDEF(	"lstore_3",				LSTORE_3,			0,	1,	BC_BAD)
INSTRDEF(	"fstore_0",				FSTORE_0,			0,	1,	BC_BAD)
INSTRDEF(	"fstore_1",				FSTORE_1,			0,	1,	BC_BAD)
INSTRDEF(	"fstore_2",				FSTORE_2,			0,	1,	BC_BAD)
INSTRDEF(	"fstore_3",				FSTORE_3,			0,	1,	BC_BAD)
INSTRDEF(	"dstore_0",				DSTORE_0,			0,	1,	BC_BAD)
INSTRDEF(	"dstore_1",				DSTORE_1,			0,	1,	BC_BAD)
INSTRDEF(	"dstore_2",				DSTORE_2,			0,	1,	BC_BAD)
INSTRDEF(	"dstore_3",				DSTORE_3,			0,	1,	BC_BAD)
INSTRDEF(	"astore_0",				ASTORE_0,			0,	1,	0)
INSTRDEF(	"astore_1",				ASTORE_1,			0,	1,	0)
INSTRDEF(	"astore_2",				ASTORE_2,			0,	1,	0)
INSTRDEF(	"astore_3",				ASTORE_3,			0,	1,	0)
INSTRDEF(	"iastore",				IASTORE,			0,	1,	0)
INSTRDEF(	"lastore",				LASTORE,			0,	1,	BC_BAD)
INSTRDEF(	"fastore",				FASTORE,			0,	1,	BC_BAD)
INSTRDEF(	"dastore",				DASTORE,			0,	1,	BC_BAD)
INSTRDEF(	"aastore",				AASTORE,			0,	1,	0)
INSTRDEF(	"bastore",				BASTORE,			0,	1,	0)
INSTRDEF(	"castore",				CASTORE,			0,	1,	0)
INSTRDEF(	"sastore",				SASTORE,			0,	1,	0)
INSTRDEF(	"pop",					POP,				0,	1,	0)
INSTRDEF(	"pop2",					POP2,				0,	1,	BC_BAD)
INSTRDEF(	"dup",					DUP,				0,	1,	0)
INSTRDEF(	"dup_x1",				DUP_X1,				0,	1,	0)
INSTRDEF(	"dup_x2",				DUP_X2,				0,	1,	BC_BAD)
INSTRDEF(	"dup2",					DUP2,				0,	1,	BC_BAD)
INSTRDEF(	"dup2_x1",				DUP2_X1,			0,	1,	0)
INSTRDEF(	"dup2_x2",				DUP2_X2,			0,	1,	BC_BAD)
INSTRDEF(	"swap",					SWAP,				0,	1,	0)
INSTRDEF(	"iadd",					IADD,				0,	1,	0)
INSTRDEF(	"ladd",					LADD,				0,	1,	BC_BAD)
INSTRDEF(	"fadd",					FADD,				0,	1,	BC_BAD)
INSTRDEF(	"dadd",					DADD,				0,	1,	BC_BAD)
INSTRDEF(	"isub",					ISUB,				0,	1,	0)
INSTRDEF(	"lsub",					LSUB,				0,	1,	BC_BAD)
INSTRDEF(	"fsub",					FSUB,				0,	1,	BC_BAD)
INSTRDEF(	"dsub",					DSUB,				0,	1,	BC_BAD)
INSTRDEF(	"imul",					IMUL,				0,	1,	0)
INSTRDEF(	"lmul",					LMUL,				0,	1,	BC_BAD)
INSTRDEF(	"fmul",					FMUL,				0,	1,	BC_BAD)
INSTRDEF(	"dmul",					DMUL,				0,	1,	BC_BAD)
INSTRDEF(	"idiv",					IDIV,				0,	1,	0)
INSTRDEF(	"ldiv",					LDIV,				0,	1,	BC_BAD)
INSTRDEF(	"fdiv",					FDIV,				0,	1,	BC_BAD)
INSTRDEF(	"ddiv",					DDIV,				0,	1,	BC_BAD)
INSTRDEF(	"irem",					IREM,				0,	1,	0)
INSTRDEF(	"lrem",					LREM,				0,	1,	BC_BAD)
INSTRDEF(	"frem",					FREM,				0,	1,	BC_BAD)
INSTRDEF(	"drem",					DREM,				0,	1,	BC_BAD)
INSTRDEF(	"ineg",					INEG,				0,	1,	0)
INSTRDEF(	"lneg",					LNEG,				0,	1,	BC_BAD)
INSTRDEF(	"fneg",					FNEG,				0,	1,	BC_BAD)
INSTRDEF(	"dneg",					DNEG,				0,	1,	BC_BAD)
INSTRDEF(	"ishl",					ISHL,				0,	1,	0)
INSTRDEF(	"lshl",					LSHL,				0,	1,	BC_BAD)
INSTRDEF(	"ishr",					ISHR,				0,	1,	0)
INSTRDEF(	"lshr",					LSHR,				0,	1,	BC_BAD)
INSTRDEF(	"iushr",				IUSHR,				0,	1,	0)
INSTRDEF(	"lushr",				LUSHR,				0,	1,	BC_BAD)
INSTRDEF(	"iand",					IAND,				0,	1,	0)
INSTRDEF(	"land",					LAND,				0,	1,	BC_BAD)
INSTRDEF(	"ior",					IOR,				0,	1,	0)
INSTRDEF(	"lor",					LOR,				0,	1,	BC_BAD)
INSTRDEF(	"ixor",					IXOR,				0,	1,	0)
INSTRDEF(	"lxor",					LXOR,				0,	1,	BC_BAD)
INSTRDEF(	"iinc %n %n",			IINC,				0,	3,	0)
INSTRDEF(	"i2l",					I2L,				0,	1,	BC_BAD)
INSTRDEF(	"i2f",					I2F,				0,	1,	BC_BAD)
INSTRDEF(	"i2d",					I2D,				0,	1,	BC_BAD)
INSTRDEF(	"l2i",					L2I,				0,	1,	BC_BAD)
INSTRDEF(	"l2f",					L2F,				0,	1,	BC_BAD)
INSTRDEF(	"l2d",					L2D,				0,	1,	BC_BAD)
INSTRDEF(	"f2i",					F2I,				0,	1,	BC_BAD)
INSTRDEF(	"f2l",					F2L,				0,	1,	BC_BAD)
INSTRDEF(	"f2d",					F2D,				0,	1,	BC_BAD)
INSTRDEF(	"d2i",					D2I,				0,	1,	BC_BAD)
INSTRDEF(	"d2l",					D2L,				0,	1,	BC_BAD)
INSTRDEF(	"d2f",					D2F,				0,	1,	BC_BAD)
INSTRDEF(	"i2b",					I2B,				0,	1,	0)
INSTRDEF(	"i2c",					I2C,				0,	1,	0)
INSTRDEF(	"i2s",					I2S,				0,	1,	0)
INSTRDEF(	"lcmp",					LCMP,				0,	1,	BC_BAD)
INSTRDEF(	"fcmpl",				FCMPL,				0,	1,	BC_BAD)
INSTRDEF(	"fcmpg",				FCMPG,				0,	1,	BC_BAD)
INSTRDEF(	"dcmpl",				DCMPL,				0,	1,	BC_BAD)
INSTRDEF(	"dcmpg",				DCMPG,				0,	1,	BC_BAD)
INSTRDEF(	"ifeq",					IFEQ,				0,	3,	0)
INSTRDEF(	"ifne",					IFNE,				0,	3,	0)
INSTRDEF(	"iflt",					IFLT,				0,	3,	0)
INSTRDEF(	"ifge",					IFGE,				0,	3,	0)
INSTRDEF(	"ifgt",					IFGT,				0,	3,	0)
INSTRDEF(	"ifle",					IFLE,				0,	3,	0)
INSTRDEF(	"if_icmpeq %o",			IF_ICMPEQ,			0,	3,	0)
INSTRDEF(	"if_icmpne %o",			IF_ICMPNE,			0,	3,	0)
INSTRDEF(	"if_icmplt %o",			IF_ICMPLT,			0,	3,	0)
INSTRDEF(	"if_icmpge %o",			IF_ICMPGE,			0,	3,	0)
INSTRDEF(	"if_icmpgt %o",			IF_ICMPGT,			0,	3,	0)
INSTRDEF(	"if_icmple %o",			IF_ICMPLE,			0,	3,	0)
INSTRDEF(	"if_acmpeq %o",			IF_ACMPEQ,			0,	3,	0)
INSTRDEF(	"if_acmpne %o",			IF_ACMPNE,			0,	3,	0)
INSTRDEF(	"goto %o",				GOTO,				0,	3,	0)
INSTRDEF(	"jsr %o",				JSR,				0,	3,	0)
INSTRDEF(	"ret",					RET,				0,	2,	0)
INSTRDEF(	"tableswitch",			TABLESWITCH,		0,	99,	0)
INSTRDEF(	"lookupswitch",			LOOKUPSWITCH,		0,	99,	0)
INSTRDEF(	"ireturn",				IRETURN,			0,	1,	0)
INSTRDEF(	"lreturn",				LRETURN,			0,	1,	BC_BAD)
INSTRDEF(	"freturn",				FRETURN,			0,	1,	BC_BAD)
INSTRDEF(	"dreturn",				DRETURN,			0,	1,	BC_BAD)
INSTRDEF(	"areturn",				ARETURN,			0,	1,	0)
INSTRDEF(	"return",				RETURN,				0,	1,	0)
INSTRDEF(	"getstatic %p",			GETSTATIC,			0,	3,	0)
INSTRDEF(	"putstatic %p",			PUTSTATIC,			0,	3,	0)
INSTRDEF(	"getfield %p",			GETFIELD,			0,	3,	0)
INSTRDEF(	"putfield %p",			PUTFIELD,			0,	3,	0)
INSTRDEF(	"invokevirtual %p",		INVOKEVIRTUAL,		0,	3,	0)
INSTRDEF(	"invokenonvirtual %p",	INVOKENONVIRTUAL,	0,	3,	0)  // invokespecial
INSTRDEF(	"invokestatic %p",		INVOKESTATIC,		0,	3,	0)
INSTRDEF(	"invokeinterface",		INVOKEINTERFACE,	0,	5,	BC_BAD)	// illegal
INSTRDEF(	"unused!!!",			UNUSED1,			0,	1,	BC_BAD)
INSTRDEF(	"new",					NEW,				0,	3,	0)
INSTRDEF(	"newarray %t",			NEWARRAY,			0,	2,	0)
INSTRDEF(	"anewarray %p",			ANEWARRAY,			0,	3,	0)
INSTRDEF(	"arraylength",			ARRAYLENGTH,		0,	1,	0)
INSTRDEF(	"athrow",				ATHROW,				0,	1,	BC_BAD)
INSTRDEF(	"checkcast",			CHECKCAST,			0,	3,	0)
INSTRDEF(	"instanceof",			INSTANCEOF,			0,	3,	0)
INSTRDEF(	"monitorenter",			MONITORENTER,		0,	1,	BC_BAD)
INSTRDEF(	"monitorexit",			MONITOREXIT,		0,	1,	BC_BAD)
INSTRDEF(	"wide",					WIDE,				0,	1,	0)
INSTRDEF(	"multianewarray",		MULTIANEWARRAY,		0,	4,	0)
INSTRDEF(	"ifnull %p",			IFNULL,				0,	3,	0)
INSTRDEF(	"ifnonnull %p",			IFNONNULL,			0,	3,	0)
INSTRDEF(	"goto_w",				GOTO_W,				0,	5,	0)
INSTRDEF(	"jsr_w",				JSR_W,				0,	5,	0)
INSTRDEF(	"breakpoint",			BREAKPOINT,			0,	1,	BC_BAD)
INSTRDEF(	"BADOP",				BADOP00,			0,	-1,	BC_BAD)
INSTRDEF(	"BADOP",				BADOP01,			0,	-1,	BC_BAD)
INSTRDEF(	"BADOP",				BADOP02,			0,	-1,	BC_BAD)
INSTRDEF(	"BADOP",				BADOP03,			0,	-1,	BC_BAD)
INSTRDEF(	"BADOP",				BADOP04,			0,	-1,	BC_BAD)
INSTRDEF(	"BADOP",				BADOP05,			0,	-1,	BC_BAD)
INSTRDEF(	"BADOP",				BADOP06,			0,	-1,	BC_BAD)
INSTRDEF(	"BADOP",				BADOP07,			0,	-1,	BC_BAD)
INSTRDEF(	"BADOP",				BADOP08,			0,	-1,	BC_BAD)
INSTRDEF(	"BADOP",				BADOP09,			0,	-1,	BC_BAD)
INSTRDEF(	"BADOP",				BADOP10,			0,	-1,	BC_BAD)
INSTRDEF(	"BADOP",				BADOP11,			0,	-1,	BC_BAD)
INSTRDEF(	"BADOP",				BADOP12,			0,	-1,	BC_BAD)
INSTRDEF(	"BADOP",				BADOP13,			0,	-1,	BC_BAD)
INSTRDEF(	"BADOP",				BADOP14,			0,	-1,	BC_BAD)
INSTRDEF(	"BADOP",				BADOP15,			0,	-1,	BC_BAD)
INSTRDEF(	"BADOP",				BADOP16,			0,	-1,	BC_BAD)
INSTRDEF(	"BADOP",				BADOP17,			0,	-1,	BC_BAD)
INSTRDEF(	"BADOP",				BADOP18,			0,	-1,	BC_BAD)
INSTRDEF(	"BADOP",				BADOP19,			0,	-1,	BC_BAD)
INSTRDEF(	"BADOP",				BADOP20,			0,	-1,	BC_BAD)
INSTRDEF(	"BADOP",				BADOP21,			0,	-1,	BC_BAD)
INSTRDEF(	"BADOP",				BADOP22,			0,	-1,	BC_BAD)
INSTRDEF(	"BADOP",				BADOP23,			0,	-1,	BC_BAD)
INSTRDEF(	"BADOP",				BADOP24,			0,	-1,	BC_BAD)
INSTRDEF(	"BADOP",				BADOP25,			0,	-1,	BC_BAD)
INSTRDEF(	"BADOP",				BADOP26,			0,	-1,	BC_BAD)
INSTRDEF(	"BADOP",				BADOP27,			0,	-1,	BC_BAD)
INSTRDEF(	"BADOP",				BADOP28,			0,	-1,	BC_BAD)
INSTRDEF(	"BADOP",				BADOP29,			0,	-1,	BC_BAD)
INSTRDEF(	"BADOP",				BADOP30,			0,	-1,	BC_BAD)
INSTRDEF(	"BADOP",				BADOP31,			0,	-1,	BC_BAD)
INSTRDEF(	"BADOP",				BADOP32,			0,	-1,	BC_BAD)
INSTRDEF(	"BADOP",				BADOP33,			0,	-1,	BC_BAD)
INSTRDEF(	"BADOP",				BADOP34,			0,	-1,	BC_BAD)
INSTRDEF(	"impdep1",				IMPDEP1,			0,	-1,	BC_BAD)
INSTRDEF(	"impdep2",				IMPDEP2,			0,	-1,	BC_BAD)
INSTRDEF(	"BADOP",				BADOP40,			0,	-1,	BC_BAD) // 240
INSTRDEF(	"BADOP",				BADOP41,			0,	-1,	BC_BAD)
INSTRDEF(	"BADOP",				BADOP42,			0,	-1,	BC_BAD)
INSTRDEF(	"BADOP",				BADOP43,			0,	-1,	BC_BAD)
INSTRDEF(	"BADOP",				BADOP44,			0,	-1,	BC_BAD)
INSTRDEF(	"BADOP",				BADOP45,			0,	-1,	BC_BAD)
INSTRDEF(	"BADOP",				BADOP46,			0,	-1,	BC_BAD)
INSTRDEF(	"BADOP",				BADOP47,			0,	-1,	BC_BAD)
INSTRDEF(	"BADOP",				BADOP48,			0,	-1,	BC_BAD)
INSTRDEF(	"BADOP",				BADOP49,			0,	-1,	BC_BAD)
INSTRDEF(	"BADOP",				BADOP50,			0,	-1,	BC_BAD)
INSTRDEF(	"BADOP",				BADOP51,			0,	-1,	BC_BAD)
INSTRDEF(	"BADOP",				BADOP52,			0,	-1,	BC_BAD)
INSTRDEF(	"BADOP",				BADOP53,			0,	-1,	BC_BAD)
INSTRDEF(	"BADOP",				BADOP54,			0,	-1,	BC_BAD)
INSTRDEF(	"BADOP",				BADOP55,			0,	-1,	BC_BAD) // 255


