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
//	       			MoSync Code Analyser
//*********************************************************************************************

#include "compile.h"


//*********************************************************************************************
//	New Version
//*********************************************************************************************

#undef CODE_ANALYSIS
#ifdef CODE_ANALYSIS

#define DEBUG_CODEFLOW



/**
 * Set the current function that will be analyzed.
 *
 * @param sym Pointer to the function symbol
 */
static void AnalyseInitContext ( AnalysisContext *ctx, SYMBOL *sym )
{
#ifdef DEBUG_CODEFLOW
    printf("Tracing CodeFlow: %s\n", sym->Name);
    #endif

    ctx->baseIP  = sym->Value;
    ctx->baseEIP = sym->EndIP;

    if (sym->Type != SECT_code)
        ErrorOnIP(Error_Fatal, ctx->baseIP, "SetFunctionBase: symbol not in code");

    ctx->baseSym = sym;

    memset(ctx->loopCountIP, 0, sizeof(ctx->loopCountIP));
    ctx->loopCount = 0;
}


/**
 * Checks to see if the the passes instruction pointer
 * is a known loop, if so it increases the IPs loop count.
 * If not, it's added as a new loop.
 *
 * @param ctx Pointer to a analysis context
 * @param ip  The instruciton pointer to check
 * @param prev_ip The previous ip
 *
 * @return 1 if the ip has been looped over more than 10 times.
 *         0 otherwise.
 */
static int AnalyseCheckLoopPoints ( AnalysisContext *ctx, int ip, int prev_ip )
{
    int n;

    // Search for loop matches

    for (n=0;n<ctx->loopCount;n++)
    {
        if ((ctx->loopIP[n] == ip) && (ctx->prevLoopIP[n] == prev_ip))
        {
            if (ctx->loopCountIP[n]++ > 10)
                return 1;

            return 0;
        }
    }

    // Add a loop to the list
    ctx->loopIP[n] = ip;
    ctx->prevLoopIP[n] = prev_ip;
    ctx->loopCountIP[n] = 0;

    if (ctx->loopCount == MAX_LOOP_LIST)
        ErrorOnIP(Error_Fatal, ip, "CheckLoopPoints: reached MAX_LOOP_LIST");

    ctx->loopCount++;
    return 0;
}


/**
 * Returns a register "value". If the reg is
 * larger then 31, then it is a constant register
 * and the constant will be fetched.
 *
 * @param reglist Register array
 * @param reg     The register id
 *
 * @return Register "value"
 */
extern int g_ConstTable[];
static int AnalyseGetReg ( int *reglist, int reg )
{
    int v;

    if (reg >= 128)
    {
        printf("AnalyseGetReg: Illegal register\n");
        return 0;
    }

    if (reg >= 32)
        v = g_ConstTable[reg];
    else
        v = reglist[reg];

    printf("get %s = %d\n", DecodeRegName(reg, 0), v);
    return v;
}


/**
 * Set a register value.
 *
 * @param reglist Register array
 * @param reg     The register id
 * @param value   The value to set the reg to
 *
 */
static void AnalyseSetReg ( int *reglist, int reg, int value )
{
    if (reg >= 32)
    {
        printf("AnalyseSetReg: Illegal set register\n");
        return;
    }

    reglist[reg] = value;

#ifdef DEBUG_CODEFLOW
    printf("set %s = %d\n", DecodeRegName(reg, 0), value);
#endif
}


#define AOP(opcode)	case opcode:
#define AEND		break;
#define REGJUMP		AnalyserAddTarget(imm);

#define DU ((unsigned int) d)
#define SU ((unsigned int) s)
#define IU ((unsigned int) imm)

/**
 * Attempts to emulate the arithmetical instructions
 *
 * @param thisOp Pointer to instruction
 * @param reglist Register array
 *
 * @return 0 If bad opcode
 *         1 Otherwise
 */
static int AnalyseTrace ( OpcodeInfo *thisOp, int *reglist )
{
    int flags = thisOp->flags;
    int imm = thisOp->imm;

    int s,d,sd,sp;

    s = d = 0;

    sp = reglist[REG_sp];

    if (flags & fetch_d)
        d = AnalyseGetReg(reglist, thisOp->rd);

    if (flags & fetch_s)
        s = AnalyseGetReg(reglist, thisOp->rs);

    sd = d;

    switch (thisOp->op)
    {
        AOP(_ADD)	d += s;		AEND;
        AOP(_ADDI)	d += imm;	AEND;
        AOP(_SUB)	d -= s;		AEND;
        AOP(_SUBI)	d -= imm;	AEND;
        AOP(_MUL)	d *= s;		AEND;
        AOP(_MULI)	d *= imm;	AEND;
        AOP(_AND)	d &= s;		AEND;
        AOP(_ANDI)	d &= imm;	AEND;
        AOP(_OR)	d |= s;		AEND;
        AOP(_ORI)	d |= imm;	AEND;
        AOP(_XOR)	d ^= s;		AEND;
        AOP(_XORI)	d ^= imm;	AEND;
        AOP(_DIVU)	d = DU / SU;	AEND;
        AOP(_DIVUI)	d = DU / IU;	AEND;
        AOP(_DIV)	d /= s;		AEND;
        AOP(_DIVI)	d /= imm;	AEND;
        AOP(_SLL)	d = DU << SU;	AEND;
        AOP(_SLLI)	d = d << IU;	AEND;
        AOP(_SRA)	d >>= s;	AEND;
        AOP(_SRAI)	d >>= imm;	AEND;
        AOP(_SRL)	d = DU >> SU;	AEND;
        AOP(_SRLI)	d = DU >> IU;	AEND;
        AOP(_NOT)	d = ~s;		AEND;
        AOP(_NEG)	d = -s;		AEND;
        AOP(_PUSH)	sp -= 4 * s;	AEND;
        AOP(_POP)	sp += 4 * s;	AEND;
        AOP(_CALL)			AEND;
        AOP(_CALLI)			AEND;
        AOP(_LDB)	d = 0xffffffff;	AEND;
        AOP(_LDH)	d = 0xffffffff;	AEND;
        AOP(_LDW)	d = 0xffffffff;	AEND;
        AOP(_STB)			AEND;
        AOP(_STH)			AEND;
        AOP(_STW)			AEND;
        AOP(_LDI)	d = imm;	AEND;
        AOP(_LDR)	d = s;		AEND;
        AOP(_XB)	d = (int)((char) s);	AEND;
        AOP(_XH)	d = (int)((short) s);	AEND;
        AOP(_SYSCALL)	d = 0xffffffff;	AEND;
        //AOP(_CASE)			AEND;
        //AOP(_JPR)			AEND;

    default:
        printf("Bad instruction in analyser %d\n",thisOp->op);
        return 0;
    }

    if (flags & fetch_d)
        AnalyseSetReg(reglist, thisOp->rd, d);

    reglist[REG_sp] = sp;
    return 1;
}


/**
 * Attempts to find loops in a path by emulating the instructions
 * and labeling back edges as loops.
 *
 * Note: There is a better way of finding loops by using
 *       basic blocks and dominators. That will also reveil
 *       loop headers. Having calculated that, SSA can be
 *       calculated much easier as well.
 *
 * @param ctx       Pointer to a analysis context
 * @param ip        The instruciton pointer to check
 * @param prev_ip   The previous ip 
 * @param depth     The current recursion depth
 * @param in_regs   Used to emulate the registers.
 */
static void AnalyseComputePath ( AnalysisContext *ctx, int ip, int prev_ip,
                                 int depth, int *in_regs )
{
    int n;
    int regvalue[32];
    OpcodeInfo thisOp;
    static char disstr[1024];

    // Sanity checking

    if (depth > 64)
        ErrorOnIP(Error_System, ip, "ComputePath: depth exceeded");

    // Copy the incoming register set to the local register set

    memcpy(regvalue, in_regs, sizeof(regvalue));

    // Since prev_ip is the source and ip is the destination,
    // check if the we have been encountered them before.

    if (prev_ip)
        if (AnalyseCheckLoopPoints(ctx, ip, prev_ip))
            return;

    // Ok this path has not been encountered before

    while(1)
    {
        if (ip > ctx->baseEIP)
            break;

        prev_ip = ip;
        ip = DecodeOpcodeIP(&thisOp, ip);

    #ifdef DEBUG_CODEFLOW
        DecodeAsmLabel(prev_ip, disstr);

        if (disstr[0])
            printf("%s:\n", disstr);

        for (n=0;n<depth+16;n++)
            printf(" ");

        DecodeAsmString(&thisOp, disstr);
        printf("%d: %s\n", prev_ip, disstr);
    #endif

        switch (thisOp.op)
        {
            case _JC_EQ:
            case _JC_NE:
            case _JC_GE:
            case _JC_GEU:
            case _JC_GT:
            case _JC_GTU:
            case _JC_LE:
            case _JC_LEU:
            case _JC_LT:
            case _JC_LTU:
            {
                int v = 0;
                int rd = AnalyseGetReg(regvalue, thisOp.rd);
                int rs = AnalyseGetReg(regvalue, thisOp.rs);

                if ((thisOp.op == _JC_EQ) && (rd == rs)) v = 1;
                if ((thisOp.op == _JC_NE) && (rd != rs)) v = 1;
                if ((thisOp.op == _JC_GT) && (rd >  rs)) v = 1;
                if ((thisOp.op == _JC_GE) && (rd >= rs)) v = 1;
                if ((thisOp.op == _JC_LT) && (rd <  rs)) v = 1;
                if ((thisOp.op == _JC_LE) && (rd <= rs)) v = 1;

                if ((thisOp.op == _JC_GTU) && ((uint) rd >  (uint) rs)) v = 1;
                if ((thisOp.op == _JC_GEU) && ((uint) rd >= (uint) rs)) v = 1;
                if ((thisOp.op == _JC_LTU) && ((uint) rd <  (uint) rs)) v = 1;
                if ((thisOp.op == _JC_LEU) && ((uint) rd <= (uint) rs)) v = 1;

                // Compute the alternative path from the jump
                if (v)
                    AnalyseComputePath(ctx, thisOp.imm, prev_ip, depth+1, regvalue);

                break;
            }

            case _JPI:
            {
                // Follow the jump ip

                ip = thisOp.imm;

                // Make sure this is not infinite

                if (AnalyseCheckLoopPoints(ctx, ip, prev_ip))
                    return;

                break;
            }

            case _CASE:
            {
                int start, len, data_ip, def_ip, i;
                int lab_ip;

                data_ip = thisOp.imm;

                start  = GetDataMemLong(data_ip++);
                len    = GetDataMemLong(data_ip++);
                def_ip = GetDataMemLong(data_ip++);	// default case

                for (i=0;i<len+1;i++)
                {
                    lab_ip = GetDataMemLong(data_ip++);

                    // Follow this case
                    AnalyseComputePath(ctx, lab_ip, prev_ip, depth+1, regvalue);
                }

                // Follow the default ip

                ip = def_ip;
                break;
            }

            case _RET:
                return;			// Return we are done

            default:
                AnalyseTrace(&thisOp, regvalue);
                break;
        }
    }
    return;
}



/**
 * Analyzes a function for loops and such.
 *
 * @param ctx Pointer to a analysis context
 * @param sym Pointer to the function symbol
 */
void TestAnalyse ( AnalysisContext *ctx, SYMBOL *func )
{
    int regvalue[32];

    memset(regvalue, 0, sizeof(regvalue));

    if (func)
    {
        AnalyseInitContext(ctx, func);
        AnalyseComputePath(ctx, 0, 0, 0, regvalue);
    }
}



#endif //CODE_ANALYSIS





//*********************************************************************************************
//	Old Version
//*********************************************************************************************

#if 0 /* OLD VERSION */

#define MAX_JUMP_TARGETS	100

int HitRet = 0;
int LogJmp = 0;

AnalyserInfo *AnalyseInfo;
AnalyserInfo AnalyseStack[MAX_JUMP_TARGETS];

int AnalyseSP = 0;
int AnalyseEntry = 0;

//****************************************
//		  Analyse Instruction
//****************************************

void OldAnalyseInit(AnalyserInfo *thisInfo, int ip)
{
	// Clear the master tracer info

	memset(thisInfo, 0, sizeof(AnalyserInfo));

	// reset the stacks

	AnalyseSP = 0;
	AnalyseEntry = 0;

	// Clear the tracer stack

	memset(AnalyseStack, 0, sizeof(AnalyseStack));

	thisInfo->ip = ip;
}

//****************************************
//
//****************************************

void OldAnalyserAddTarget(int ip)
{
	int n;

	if (AnalyseEntry >= MAX_JUMP_TARGETS)
	{
		printf("out of analyser space\n");
		return;
	}

	for (n=0;n<AnalyseEntry;n++)
	{
		// Already in stack ? if so exit

		if (AnalyseStack[n].ip == ip)
			return;
	}

	memcpy(AnalyseStack[AnalyseEntry].regvalue, AnalyseInfo->regvalue, sizeof(int) * 32);
	AnalyseStack[AnalyseEntry].ip = ip;

	AnalyseEntry++;
}

//****************************************
//		  Select Analyse Info
//****************************************

void OldAnalyseSelect(AnalyserInfo *thisInfo)
{
	AnalyseInfo = thisInfo;
}

//****************************************
// 	 Mark a register has been used
//****************************************

void OldAnalyseMarkReg(int reg)
{
	if (reg >= 32)
		return;

	AnalyseInfo->regcount[reg]++;
}

//****************************************
//	  Find out if a reg is used/active
//****************************************

int OldAnalyseRegCount(int reg)
{
	if (reg >= 32)
		return 1;

	return AnalyseInfo->regcount[reg];
}

//****************************************
//		  Analyse Instruction
//****************************************

int OldAnalyseInstruction(OpcodeInfo *thisOpcode)
{
	int flags = thisOpcode->flags;
	//int imm = thisOpcode->imm;
	int rd,rs;
	int v;

	// Get dest reg statistics

	if (flags & fetch_d)
	{
		rd = thisOpcode->rd;

		// Check that destinatiom reg is not a contant

		if (rd >= 32)
		{
			printf("Verfier: Constant register cannot be a destination\n");
			return 0;
		}

		// Increment usage of register

		AnalyseMarkReg(rd);

	}
	// Get source reg statistics

	if (flags & fetch_s)
	{
		rs = thisOpcode->rs;

		// Check if register is defined, otherwise error

		if (!AnalyseRegCount(rs))
		{
			printf("Verfier: Source register used is in an undefined state\n");
			return 0;
		}

		// Increment usage of register

		AnalyseMarkReg(rs);
	}

	v = AnalyseTrace(thisOpcode);
	return v;
}

//****************************************
//
//****************************************

extern int g_ConstTable[];

int OldAnalyseGetReg(int reg)
{
	if (reg >= 128)
	{
		printf("Verfier: Illegal register\n");
		return 0;
	}

	if (reg >= 32)
		return g_ConstTable[reg];

	return AnalyseInfo->regcount[reg];
}

//****************************************
//
//****************************************

void OldAnalyseSetReg(int reg, int value)
{
	if (reg >= 32)
	{
		printf("Verfier: Illegal set register\n");
		return;
	}

	AnalyseInfo->regcount[reg] = value;
}

//****************************************
//		  Analyse Trace
//****************************************

#define AOP(opcode)		case opcode:
#define AEND			break;
#define REGJUMP			AnalyserAddTarget(imm);

#define DU ((unsigned int) d)
#define SU ((unsigned int) s)
#define IU ((unsigned int) imm)

int OldAnalyseTrace(OpcodeInfo *thisOpcode)
{
	int flags = thisOpcode->flags;
	int imm = thisOpcode->imm;

	int s,d,sd;

	s = d = 0;

	if (flags & fetch_d)
		d = AnalyseGetReg(thisOpcode->rd);

	if (flags & fetch_s)
		s = AnalyseGetReg(thisOpcode->rs);

	sd = d;

	HitRet = -1;
	LogJmp = -1;

	switch (thisOpcode->op)
	{
		AOP(_ADD)	d += s;			AEND;
		AOP(_ADDI)	d += imm;		AEND;
		AOP(_SUB)	d -= s;			AEND;
		AOP(_SUBI)	d -= imm;		AEND;
		AOP(_MUL)	d *= s;			AEND;
		AOP(_MULI)	d *= imm;		AEND;
		AOP(_AND)	d &= s;			AEND;
		AOP(_ANDI)	d &= imm;		AEND;
		AOP(_OR)	d |= s;			AEND;
		AOP(_ORI)	d |= imm;		AEND;
		AOP(_XOR)	d ^= s;			AEND;
		AOP(_XORI)	d ^= imm;		AEND;
		AOP(_DIVU)	d = DU / SU;	AEND;
		AOP(_DIVUI)	d = DU / IU;	AEND;
		AOP(_DIV)	d /= s;			AEND;
		AOP(_DIVI)	d /= imm;		AEND;
		AOP(_SLL)	d = DU << SU;	AEND;
		AOP(_SLLI)	d = d << IU;	AEND;
		AOP(_SRA)	d >>= s;		AEND;
		AOP(_SRAI)	d >>= imm;		AEND;
		AOP(_SRL)	d = DU >> SU;	AEND;
		AOP(_SRLI)	d = DU >> IU;	AEND;
		AOP(_NOT)	d = ~s;			AEND;
		AOP(_NEG)	d = -s;			AEND;

		AOP(_PUSH)
			AnalyseSetReg(REG_sp, AnalyseGetReg(REG_sp) - 4 * s);
		AEND;

		AOP(_POP)
			AnalyseSetReg(REG_sp, AnalyseGetReg(REG_sp) + 4 * s);
		AEND;

		AOP(_CALL)					AEND;
		AOP(_CALLI)					AEND;

		AOP(_LDB)	d = 0xffffffff;	AEND;
		AOP(_LDH)	d = 0xffffffff;	AEND;
		AOP(_LDW)	d = 0xffffffff;	AEND;
		AOP(_STB)					AEND;
		AOP(_STH)					AEND;
		AOP(_STW)					AEND;

		AOP(_LDI)	d = imm;		AEND;
		AOP(_LDR)	d = s;			AEND;

		AOP(_RET)	HitRet=1;		AEND;

		AOP(_JC_EQ)	 REGJUMP		AEND;
		AOP(_JC_NE)	 REGJUMP 		AEND;
		AOP(_JC_GE)	 REGJUMP 		AEND;
		AOP(_JC_GT)	 REGJUMP 		AEND;
		AOP(_JC_LE)	 REGJUMP 		AEND;
		AOP(_JC_LT)	 REGJUMP 		AEND;

		AOP(_JC_LTU) REGJUMP 		AEND;
		AOP(_JC_GEU) REGJUMP 		AEND;
		AOP(_JC_GTU) REGJUMP 		AEND;
		AOP(_JC_LEU) REGJUMP 		AEND;

		AOP(_JPI)	 REGJUMP 		AEND;
		AOP(_JPR)					AEND;

		AOP(_XB) d = (int)((char) s);	AEND;
		AOP(_XH) d = (int)((short) s);	AEND;

		AOP(_SYSCALL)				AEND;
		AOP(_CASE)					AEND;

	default:
		printf("Bad instruction in analyser %d\n",thisOpcode->op);
		return 0;

	}

	if (flags & fetch_d)
		if (sd != d)
			AnalyseSetReg(thisOpcode->rd, d);

	return 1;
}

#endif
