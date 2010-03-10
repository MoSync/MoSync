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

#include "ArmRecompiler.h"

#define CACHE_LINE_SIZE 64 // bytes

#ifdef USE_ARM_RECOMPILER

using namespace avmplus;
using namespace Core;

#ifdef _WIN32
#include <windows.h>
#endif

//#define LOGC(x, ...)
//#define LOG(x, ...)

#define SETUP_DEFAULT_VISITOR_ELEM(inst) defaultVisitors[_##inst] = &ArmRecompiler::visit_##inst;

#define ARM_PC (assm.mInstructionCount)
#define ARM_PC_TO_ADDR(PC) ((int)assm.mipStart+PC*sizeof(AA::MDInstruction))
#define ARM_PC_ADDR ARM_PC_TO_ADDR(ARM_PC)

#define JUMP_GEN(addr)\
	{\
	addr&=mEnvironment.codeMask;\
	int ofs = mPipeToArmInstMap[addr] - (ARM_PC_ADDR);\
	assm.B(ofs);\
	}\

#define JUMP_IMM16(rd, rs, addr, cond) \
	{\
	AA::Register reg1 = loadRegister(rd, AA::R1);\
	AA::Register reg2 = loadRegister(rs, AA::R2);\
	assm.CMP(reg1, reg2);\
	assm.SET_CONDITION_CODE(cond);\
	JUMP_GEN(addr)\
	assm.SET_CONDITION_CODE(AA::AL);\
	}

#define CALL_IMM_ARM(imm32)\
{\
	int returnAddr = (mInstructions[0].ip+mInstructions[0].length);\
	AA::Register saveReg = getSaveRegister(REG_rt, AA::R1);\
	assm.MOV_imm32(saveReg, returnAddr);\
	saveRegister(REG_rt, saveReg);\
	assm.SET_CONDITION_CODE(AA::AL);\
	JUMP_GEN(imm32);\
}

//	assm.MOV_imm32(AA::R2, mEnvironment.dataMask & ~(sizeof(type) - 1));

//	assm.BIC_imm8(AA::R2, MEMORY_MASK, (sizeof(type)-1));

//	assm.MVN_imm8(AA::R2, (sizeof(type)-1));
//	assm.AND(AA::R2, AA::R2, MEMORY_MASK);

#define LOAD(LDRFUNC, rd, rs, addr, type)\
	{\
	assm.MOV_imm32(AA::R3, addr);\
	assm.ADD(AA::R3, AA::R3, loadRegister(rs, AA::R1));\
	assm.BIC_imm8(AA::R2, MEMORY_MASK(AA::R1), (sizeof(type)-1));\
	assm.AND(AA::R3, AA::R3, AA::R2);\
	assm.ADD(AA::R3, AA::R3, MEMORY_ADDR(AA::R1));\
	AA::Register saveReg = getSaveRegister(rd, AA::R2);\
	assm.LDRFUNC(saveReg, 0, AA::R3);\
	saveRegister(rd, saveReg);\
	}

#define STORE(STRFUNC, rd, rs, addr, type)\
	{\
	assm.MOV_imm32(AA::R3, addr);\
	assm.ADD(AA::R3, AA::R3, loadRegister(rd, AA::R1));\
	assm.BIC_imm8(AA::R2, MEMORY_MASK(AA::R1), (sizeof(type)-1));\
	assm.AND(AA::R3, AA::R3, AA::R2);\
	assm.ADD(AA::R3, AA::R3, MEMORY_ADDR(AA::R1));\
	assm.STRFUNC(loadRegister(rs, AA::R1), 0, AA::R3);\
	}

#define ARITHMETIC(ARITH_FUNC, rd, rs)\
	{\
	AA::Register saveReg = getSaveRegister(rd, AA::R1);\
	assm.ARITH_FUNC(saveReg,  loadRegister(rd, AA::R2), loadRegister(rs, AA::R3));\
	saveRegister(rd, saveReg);\
	}

#define ARITHMETIC_IMM_DIRECT(ARITH_FUNC, rd, imm32)\
	{\
	AA::Register saveReg = getSaveRegister(rd, AA::R0);\
	assm.ARITH_FUNC(saveReg, loadRegister(rd, AA::R2), imm32);\
	saveRegister(rd, saveReg);\
	}

#define ARITHMETIC_IMM(ARITH_FUNC, rd, imm32)\
	{\
	assm.MOV_imm32(AA::R1, imm32);\
	AA::Register saveReg = getSaveRegister(rd, AA::R0);\
	assm.ARITH_FUNC(saveReg, loadRegister(rd, AA::R2), AA::R1);\
	saveRegister(rd, saveReg);\
	}

#define ARITHMETIC_IMM_OPT(ARITH_FUNC, rd, imm32)\
	{\
	if((uint32)imm32<=0xff) ARITHMETIC_IMM_DIRECT(ARITH_FUNC##_imm8, rd, imm32)\
	else ARITHMETIC_IMM(ARITH_FUNC, rd, imm32)\
	}

#define LOAD_MEMORY_FUNC(func, memory_addr, arm_r, temp_r)\
	assm.MOV_imm32((AA::Register)temp_r, (int)memory_addr);\
	assm.func((AA::Register)arm_r, 0, (AA::Register)temp_r);\

#define LOAD_MEMORY(memory_addr, arm_r, temp_r) LOAD_MEMORY_FUNC(LDR, memory_addr, arm_r, temp_r);


#define SAVE_MEMORY(memory_addr, arm_r, temp_reg)\
	assm.MOV_imm32(temp_reg, (int)memory_addr);\
	assm.STR((AA::Register)arm_r, 0, (AA::Register)temp_reg);\

//assm.MOV_imm32(temp_reg, (int)mPipeToArmInstMap);

#define SET_PC(reg, temp_reg)\
		assm.MOV_imm32(temp_reg, mEnvironment.codeMask);\
		assm.AND(reg, reg, temp_reg);\
		assm.LSL_i(reg, reg, 2);\
		assm.ADD(reg, reg, PIPE_TO_ARM_MAP(temp_reg));\
		assm.LDR(AA::PC, 0, reg);\

//dangerous; can cause KERN-EXEC 3 on Symbian if done wrong
//#if defined(SYMBIAN) && !defined(__SERIES60_3X__)	//1//def _MSC_VER
//#define FUNC_CAST(func) ((int)(void*)func)
//#else
#define FUNC_CAST(func) (*(int*)&func)
//#endif

	void* MoSync::ArmRecompiler::allocateCodeMemory(int size) {
	#ifdef __SYMBIAN32__
		//return new (ELeave) byte[size];
		//return User::AllocL(size);
		if(mHeap == NULL) {
			//HACK: extra memory for the entryPoint
			int chunkSize = size + 1024 * 2;
			LOG("chunkSize %i -> %i\n", size, chunkSize);
			LHEL(mChunk.CreateLocalCode(chunkSize, chunkSize));
			LHEL(mChunk.Adjust(chunkSize));
			mHeap = UserHeap::ChunkHeap(mChunk, size + 1024);
			DEBUG_ASSERT(mHeap != NULL);
		}
		void* ptr = mHeap->AllocL(size);
		LOG("alloCode %i 0x%08x\n", size, ptr);
		return ptr;
	#else	// winmobile
		return VirtualAlloc(NULL, size, MEM_COMMIT, PAGE_EXECUTE_READWRITE);
	#endif
	}

	void MoSync::ArmRecompiler::freeCodeMemory(void *addr) {
#ifdef __SYMBIAN32__
		//delete (byte*)addr;
		//User::Free(addr);
		LOG("freeCode 0x%08x\n", addr);
		mHeap->Free(addr);
	#else	// winmobile
		VirtualFree(addr, 0, MEM_RELEASE);
	#endif
	}

	void MoSync::ArmRecompiler::flushInstructionCache(void *addr, int len) {
	#ifdef __SYMBIAN32__
	#ifndef __SERIES60_3X__
		// Adjust start and len to page boundaries
		addr = (void*) ((int)addr & ~0xFFF);
		len = (len+8192) & ~0xFFF;
	#endif
		void *end = (void*)((int)addr+len);
		LOGD("flushInstructionCache end: 0x%08x\n", end);
		User::IMB_Range(addr, end);
		LOGD("User::IMB_Range worked\n");
	#else // winmobile
		// This seems to work, but might not be correct
		// Might be better to call CacheSync(...)
		FlushInstructionCache(GetCurrentProcess(), 0, 0);
	#endif
	}

namespace MoSync {
	void ArmRecompiler::saveStackPointer(AA& assm, AA::Register temp) {
		SAVE_MEMORY(&mArmStackPointer, AA::SP, temp);
	}

	void ArmRecompiler::loadStackPointer(AA& assm, AA::Register temp) {
		LOAD_MEMORY(&mArmStackPointer, AA::SP, temp);
	}

	void ArmRecompiler::generateEntryPoint()
	{
#ifdef DEBUG_DISASM
		entryPoint.verboseFlag = true;
		assm.console->format("\ngenerating entry point\n\n");
#endif
		// allocate enough space..
		entryPoint.mipStart = (AA::MDInstruction*)allocateCodeMemory(64*sizeof(AA::MDInstruction));
		entryPoint.mip = entryPoint.mipStart;

		// save registers on stack
		int mask = (0xffff)&(~(AA::R0_mask|AA::SP_mask|AA::PC_mask));
		entryPoint.STMFD_bang(AA::SP, mask);

		saveStackPointer(entryPoint, AA::R1);

		// store mosync register address
		entryPoint.MOV_imm32(REGISTER_ADDR, (int)&mEnvironment.regs[0]);
		
		loadEnvironmentRegisters(entryPoint);

		loadStaticRegisters(entryPoint);

		// goto recompiled code
		entryPoint.MOV(AA::PC, AA::R0);
	}

	void ArmRecompiler::returnFromRecompiledCode()
	{
		saveStaticRegisters(assm);

		loadStackPointer(assm, AA::R0);

		int mask = (0xffff)&(~(AA::R0_mask|AA::SP_mask|AA::PC_mask));
		assm.LDMFD_bang(AA::SP, mask);

		// set ARM_PC of the instruction following the RET as return value
		assm.MOV(AA::R0, AA::PC);

		// RET
		assm.MOV(AA::PC, AA::LR);
	}


	void ArmRecompiler::emitOneArgFuncCall(OneArgFunc argFunc, int arg1) {
		saveStaticRegisters(assm);
		loadStackPointer(assm, AA::R3);
		assm.MOV_imm32(AA::R3, FUNC_CAST(argFunc));
		assm.MOV_imm32(AA::R0, (int)this);
		assm.MOV_imm32(AA::R1, (int)arg1);
		assm.MOV(AA::LR, AA::PC);
		assm.MOV(AA::PC, AA::R3);
		saveStackPointer(assm, AA::R3);
		loadStaticRegisters(assm);
	}

	void ArmRecompiler::emitTwoArgFuncCall(TwoArgFunc argFunc, int arg1, int arg2) {
		saveStaticRegisters(assm);
		loadStackPointer(assm, AA::R3);
		assm.MOV_imm32(AA::R3, FUNC_CAST(argFunc));
		assm.MOV_imm32(AA::R0, (int)this);
		assm.MOV_imm32(AA::R1, (int)arg1);
		assm.MOV_imm32(AA::R2, (int)arg2);
		assm.MOV(AA::LR, AA::PC);
		assm.MOV(AA::PC, AA::R3);
		saveStackPointer(assm, AA::R3);
		loadStaticRegisters(assm);
	}


	AA::Register ArmRecompiler::findStaticRegister(int msReg) {
	#ifdef NO_STATIC_REGISTERS
		return AA::Unknown;
	#endif

		for(int i = 0; i < NUM_STATICALLY_ALLOCATED_REGISTERS; i++) {
			if(registerMapping[i].msReg == msReg) {			
				return registerMapping[i].armReg;
			}
		}

		return AA::Unknown;
	}

	AA::Register ArmRecompiler::getStaticRegister(int i) {
		return registerMapping[i].armReg;
	}

	void ArmRecompiler::saveStaticRegister(AA &assm, int i) {
	#ifdef NO_STATIC_REGISTERS
		return;
	#endif
		assm.STR(registerMapping[i].armReg, ((registerMapping[i].msReg)<<2), REGISTER_ADDR);
	}

	void ArmRecompiler::loadStaticRegister(AA &assm, int i) {
	#ifdef NO_STATIC_REGISTERS
		return;
	#endif
		assm.LDR(registerMapping[i].armReg, ((registerMapping[i].msReg)<<2), REGISTER_ADDR);
	}

	void ArmRecompiler::saveStaticRegisters(AA &assm) {

	#ifdef NO_STATIC_REGISTERS
		return;
	#endif
		for(int i = 0; i < NUM_STATICALLY_ALLOCATED_REGISTERS; i++) {
			saveStaticRegister(assm, i);
		}
	}

	void ArmRecompiler::loadStaticRegisters(AA &assm) {

	#ifdef NO_STATIC_REGISTERS
		return;
	#endif
		for(int i = 0; i < NUM_STATICALLY_ALLOCATED_REGISTERS; i++) {
			loadStaticRegister(assm, i);
		}
	}

	AA::Register ArmRecompiler::getSaveRegister(int mosync_reg, AA::Register arm_r) {
		AA::Register armReg;
		if((armReg = findStaticRegister(mosync_reg))==AA::Unknown)
			return arm_r;
		else
			return armReg;
	}

	void ArmRecompiler::saveRegister(int mosync_reg, AA::Register arm_r)
	{
		AA::Register armReg;
		if((armReg = findStaticRegister(mosync_reg))==AA::Unknown)
			assm.STR((AA::Register)arm_r, ((mosync_reg)<<2), REGISTER_ADDR);
		else
			if(armReg!=arm_r)
				assm.MOV(armReg, arm_r);
	}

	AA::Register ArmRecompiler::loadRegister(int msreg, AA::Register armreg, bool shouldCopy) {
		AA::Register ret;
		if((ret=findStaticRegister(msreg))==AA::Unknown) {
			assm.LDR((AA::Register)armreg, ((msreg)<<2), REGISTER_ADDR);
			return armreg;
		} else {
			if(shouldCopy) {
				assm.MOV(armreg, ret);
				return armreg;
			} else {
				return ret;
			}
		}
	}

	void ArmRecompiler::visit_PUSH() {
		LOGC("PUSH\n");
		byte rd = mInstructions[0].rd;
		int imm32 = mInstructions[0].imm;

		AA::Register reg = loadRegister(REG_sp, AA::R1);
		assm.ADD(AA::R1, reg, MEMORY_ADDR(AA::R2));
		for(int i = rd; i < rd+imm32; i++) {
			assm.SUB_imm8(AA::R1, AA::R1, 4);
			AA::Register reg = loadRegister(i, AA::R2);
			assm.STR(reg, 0, AA::R1);
		}
		AA::Register saveReg = getSaveRegister(REG_sp, AA::R1);
		assm.SUB(saveReg, AA::R1, MEMORY_ADDR(AA::R2));
		saveRegister(REG_sp, saveReg);
	}

	void ArmRecompiler::visit_POP() {
		LOGC("POP\n");
		byte rd = mInstructions[0].rd;
		int imm32 = mInstructions[0].imm;

		AA::Register reg = loadRegister(REG_sp, AA::R1);
		assm.ADD(AA::R1, reg, MEMORY_ADDR(AA::R2));
		
		for(int i = rd; i > rd-imm32; i--) 
		{	
			AA::Register saveReg = getSaveRegister(i, AA::R2);
			assm.LDR(saveReg, 0, AA::R1);
			saveRegister(i, saveReg);
			assm.ADD_imm8(AA::R1, AA::R1, 4);
		}
		AA::Register saveReg = getSaveRegister(REG_sp, AA::R1);
		assm.SUB(saveReg, AA::R1, MEMORY_ADDR(AA::R2));
		saveRegister(REG_sp, saveReg);
	}

	void ArmRecompiler::visit_CALL() { 
		LOGC("CALL\n");
		byte rd = mInstructions[0].rd;

		int returnAddr = mInstructions[0].ip + mInstructions[0].length;
		AA::Register saveReg = getSaveRegister(REG_rt, AA::R1);
		assm.MOV_imm32(saveReg, returnAddr);
		saveRegister(REG_rt, saveReg);

		AA::Register reg = loadRegister(rd, AA::R1);
		assm.MOV_imm32(AA::R2, mEnvironment.codeMask, true);
		assm.AND(AA::R1, reg, AA::R2);

		//assm.MOV_imm32(AA::R0, (int)mPipeToArmInstMap); // r0 = pipeToArmInstMap
		
		assm.LSL_i(AA::R1, AA::R1, 2); // *sizeof(int)
		assm.ADD(AA::R0, PIPE_TO_ARM_MAP(AA::R3), AA::R1);           // r0 += r1
		assm.LDR(AA::R2, 0, AA::R0);                // r1 = *r0

		assm.MOV(AA::PC, AA::R2);
	}

	void ArmRecompiler::visit_CALLI() {
		LOGC("CALLI\n");
		int imm32 = mInstructions[0].imm;
		CALL_IMM_ARM(imm32)
	}

	void ArmRecompiler::visit_LDB() {
		LOGC("LDB\n");
		byte rd = mInstructions[0].rd;
		byte rs = mInstructions[0].rs;
		int imm32 = mInstructions[0].imm;		
		LOAD(LDRBS, rd, rs, imm32, char);
	}

	void ArmRecompiler::visit_STB() {
		LOGC("STB\n");
		byte rd = mInstructions[0].rd;
		byte rs = mInstructions[0].rs;
		int imm32 = mInstructions[0].imm;	
		STORE(STRB, rd, rs, imm32, char);
	}

	void ArmRecompiler::visit_LDH() {
		LOGC("LDH\n");
		byte rd = mInstructions[0].rd;
		byte rs = mInstructions[0].rs;
		int imm32 = mInstructions[0].imm;
		LOAD(LDRHS, rd, rs, imm32, short);
	}

	void ArmRecompiler::visit_STH() { 
		LOGC("STH\n");
		byte rd = mInstructions[0].rd;
		byte rs = mInstructions[0].rs;
		int imm32 = mInstructions[0].imm;
		STORE(STRH, rd, rs, imm32, short);
	}

	void ArmRecompiler::visit_LDW() {
		LOGC("LDW\n");
		byte rd = mInstructions[0].rd;
		byte rs = mInstructions[0].rs;
		int imm32 = mInstructions[0].imm;
		LOAD(LDR, rd, rs, imm32, int);
	}

	void ArmRecompiler::visit_STW() {
		LOGC("STW\n");
		byte rd = mInstructions[0].rd;
		byte rs = mInstructions[0].rs;
		int imm32 = mInstructions[0].imm;
		STORE(STR, rd, rs, imm32, int);
	}

	void ArmRecompiler::visit_LDI() {
		LOGC("LDI\n");
		byte rd = mInstructions[0].rd;
		int imm32 = mInstructions[0].imm;
		AA::Register saveReg = getSaveRegister(rd, AA::R1);
		assm.MOV_imm32(saveReg, imm32);
		saveRegister(rd, saveReg);
	}

	void ArmRecompiler::visit_LDR() {
		LOGC("LDR\n");
		byte rd = mInstructions[0].rd;
		byte rs = mInstructions[0].rs;
		AA::Register saveReg = getSaveRegister(rd, AA::R0);
		AA::Register reg = loadRegister(rs, saveReg);
		saveRegister(rd, reg);
	}

	void ArmRecompiler::visit_ADD() {
		LOGC("ADD\n");
		byte rd = mInstructions[0].rd;
		byte rs = mInstructions[0].rs;
		ARITHMETIC(ADD, rd, rs);
	}

	void ArmRecompiler::visit_ADDI() { 
		LOGC("ADDI\n");
		byte rd = mInstructions[0].rd;
		int imm32 = mInstructions[0].imm;
		if((uint32)imm32<=0xffff)
			ARITHMETIC_IMM_DIRECT(ADD_imm16, rd, imm32)
		else
			ARITHMETIC_IMM_OPT(ADD, rd, imm32)
	}

	void ArmRecompiler::visit_MUL() {
		LOGC("MUL\n");
		byte rd = mInstructions[0].rd;
		byte rs = mInstructions[0].rs;
		ARITHMETIC(MUL, rd, rs);
	}

	void ArmRecompiler::visit_MULI() {
		LOGC("MULI\n");
		byte rd = mInstructions[0].rd;
		int imm32 = mInstructions[0].imm;
		ARITHMETIC_IMM(MUL, rd, imm32);
	}

	void ArmRecompiler::visit_SUB() {
		LOGC("SUB\n");
		byte rd = mInstructions[0].rd;
		byte rs = mInstructions[0].rs;
		ARITHMETIC(SUB, rd, rs);
	}

	void ArmRecompiler::visit_SUBI() {
		LOGC("SUBI\n");
		byte rd = mInstructions[0].rd;
		int imm32 = mInstructions[0].imm;
		ARITHMETIC_IMM_OPT(SUB, rd, imm32);
	}

	void ArmRecompiler::visit_AND() {
		LOGC("AND\n");
		byte rd = mInstructions[0].rd;
		byte rs = mInstructions[0].rs;
		ARITHMETIC(AND, rd, rs);
	}

	void ArmRecompiler::visit_ANDI() {
		LOGC("ANDI\n");
		byte rd = mInstructions[0].rd;
		int imm32 = mInstructions[0].imm;
		ARITHMETIC_IMM_OPT(AND, rd, imm32);
	}

	void ArmRecompiler::visit_OR() { 
		LOGC("OR\n");
		byte rd = mInstructions[0].rd;
		byte rs = mInstructions[0].rs;
		ARITHMETIC(ORR, rd, rs);
	}

	void ArmRecompiler::visit_ORI() {
		LOGC("ORI\n");
		byte rd = mInstructions[0].rd;
		int imm32 = mInstructions[0].imm;
		ARITHMETIC_IMM_OPT(ORR, rd, imm32);
	}

	void ArmRecompiler::visit_XOR() {
		LOGC("XOR\n");
		byte rd = mInstructions[0].rd;
		byte rs = mInstructions[0].rs;
		ARITHMETIC(EOR, rd, rs);
	}

	void ArmRecompiler::visit_XORI() { 
		LOGC("XORI\n");
		byte rd = mInstructions[0].rd;
		int imm32 = mInstructions[0].imm;
		ARITHMETIC_IMM_OPT(EOR, rd, imm32);
	}

	// TODO implement division with assembly?
	void ArmRecompiler::divu(int rd, int rs) {
		*(unsigned long*)&mEnvironment.regs[rd] /= *(unsigned long*)&mEnvironment.regs[rs];
	}

	void ArmRecompiler::divui(int rd, int imm32) {
		*(unsigned long*)&mEnvironment.regs[rd] /= (unsigned long) imm32;
	}

	void ArmRecompiler::div(int rd, int rs) {
		mEnvironment.regs[rd] /= mEnvironment.regs[rs];
	}

	void ArmRecompiler::divi(int rd, int imm32) {
		mEnvironment.regs[rd] /= (long) imm32;
	}

	void ArmRecompiler::visit_DIVU() {
		LOGC("DIVU\n");
		byte rd = mInstructions[0].rd;
		byte rs = mInstructions[0].rs;
		emitTwoArgFuncCall(&ArmRecompiler::divu, rd, rs);
	}

	void ArmRecompiler::visit_DIVUI() {
		LOGC("DIVUI\n");
		byte rd = mInstructions[0].rd;
		int imm32 = mInstructions[0].imm;
		emitTwoArgFuncCall(&ArmRecompiler::divui, rd, imm32);
	}

	void ArmRecompiler::visit_DIV() {
		LOGC("DIV\n");
		byte rd = mInstructions[0].rd;
		byte rs = mInstructions[0].rs;
		emitTwoArgFuncCall(&ArmRecompiler::div, rd, rs);
	}

	void ArmRecompiler::visit_DIVI() {
		LOGC("DIVI\n");
		byte rd = mInstructions[0].rd;
		int imm32 = mInstructions[0].imm;
		emitTwoArgFuncCall(&ArmRecompiler::divi, rd, imm32);
	}

	void ArmRecompiler::visit_SLL() {
		LOGC("SLL\n");
		byte rd = mInstructions[0].rd;
		byte rs = mInstructions[0].rs;
		ARITHMETIC(LSL, rd, rs);
	}

	void ArmRecompiler::visit_SLLI() {
		LOGC("SLLI\n");
		byte rd = mInstructions[0].rd;
		int imm32 = mInstructions[0].imm;
		ARITHMETIC_IMM_DIRECT(LSL_i, rd, imm32);
	}
	
	void ArmRecompiler::visit_SRA() {
		LOGC("SRA\n");
		byte rd = mInstructions[0].rd;
		byte rs = mInstructions[0].rs;
		ARITHMETIC(ASR, rd, rs);
	}

	void ArmRecompiler::visit_SRAI() {
		LOGC("SRAI\n");
		byte rd = mInstructions[0].rd;
		int imm32 = mInstructions[0].imm;
		ARITHMETIC_IMM_DIRECT(ASR_i, rd, imm32);
	}

	void ArmRecompiler::visit_SRL() {
		LOGC("SRL\n");
		byte rd = mInstructions[0].rd;
		byte rs = mInstructions[0].rs;
		ARITHMETIC(LSR, rd, rs);
	}
	
	void ArmRecompiler::visit_SRLI() {
		LOGC("SRLI\n");
		byte rd = mInstructions[0].rd;
		int imm32 = mInstructions[0].imm;
		ARITHMETIC_IMM_DIRECT(LSR_i, rd, imm32);
	}

	void ArmRecompiler::visit_NOT() {
		LOGC("NOT\n");
		byte rd = mInstructions[0].rd;
		byte rs = mInstructions[0].rs;
		assm.MVN_imm8(AA::R0, 0);
		AA::Register reg = loadRegister(rs, AA::R1);
		AA::Register saveReg = getSaveRegister(rd, AA::R2);
		assm.EOR(saveReg, reg, AA::R0);
		saveRegister(rd, saveReg);
	}

	void ArmRecompiler::visit_NEG() {
		LOGC("NEG\n");
		byte rd = mInstructions[0].rd;
		byte rs = mInstructions[0].rs;

		// fugly???
		assm.MOV_imm8(AA::R0, 0);
		AA::Register reg = loadRegister(rs, AA::R1);
		AA::Register saveReg = getSaveRegister(rd, AA::R2);
		assm.SUB(saveReg, AA::R0, reg);
		saveRegister(rd, saveReg);
	}

	void ArmRecompiler::visit_RET() {
		LOGC("RET\n");

		assm.MOV_imm32(AA::R2, mEnvironment.codeMask);
		assm.AND(AA::R1, loadRegister(REG_rt, AA::R3), AA::R2);

		//assm.MOV_imm32(AA::R0, (int)mPipeToArmInstMap); // r0 = pipeToArmInstMap

		assm.LSL_i(AA::R1, AA::R1, 2); // *sizeof(int)
		assm.ADD(AA::R1, AA::R1, PIPE_TO_ARM_MAP(AA::R2));           // r0 += r1
		assm.LDR(AA::PC, 0, AA::R1);                // r1 = *r0
	}

	void ArmRecompiler::visit_JC_EQ() {
		LOGC("JC_EQ\n");
		byte rd = mInstructions[0].rd;
		byte rs = mInstructions[0].rs;
		int imm32 = mInstructions[0].imm;
		JUMP_IMM16(rd, rs, imm32, AA::EQ);
	}

	void ArmRecompiler::visit_JC_NE() {
		LOGC("JC_NE\n");
		byte rd = mInstructions[0].rd;
		byte rs = mInstructions[0].rs;
		int imm32 = mInstructions[0].imm;
		JUMP_IMM16(rd, rs, imm32, AA::NE);
	}

	void ArmRecompiler::visit_JC_GE() {
		LOGC("JC_GE\n");
		byte rd = mInstructions[0].rd;
		byte rs = mInstructions[0].rs;
		int imm32 = mInstructions[0].imm;
		JUMP_IMM16(rd, rs, imm32, AA::GE);
	}

	void ArmRecompiler::visit_JC_GEU() { 
		LOGC("JC_GEU\n");
		byte rd = mInstructions[0].rd;
		byte rs = mInstructions[0].rs;
		int imm32 = mInstructions[0].imm;
		JUMP_IMM16(rd, rs, imm32, AA::CS);
	}

	void ArmRecompiler::visit_JC_GT() {
		LOGC("JC_GT\n");
		byte rd = mInstructions[0].rd;
		byte rs = mInstructions[0].rs;
		int imm32 = mInstructions[0].imm;
		JUMP_IMM16(rd, rs, imm32, AA::GT);
	}

	void ArmRecompiler::visit_JC_GTU() {
		LOGC("JC_GTU\n");
		byte rd = mInstructions[0].rd;
		byte rs = mInstructions[0].rs;
		int imm32 = mInstructions[0].imm;
		JUMP_IMM16(rd, rs, imm32, AA::HI);
	}

	void ArmRecompiler::visit_JC_LE() { 
		LOGC("JC_LE\n");
		byte rd = mInstructions[0].rd;
		byte rs = mInstructions[0].rs;
		int imm32 = mInstructions[0].imm;
		JUMP_IMM16(rd, rs, imm32, AA::LE);
	}

	void ArmRecompiler::visit_JC_LEU() { 
		LOGC("JC_LEU\n");
		byte rd = mInstructions[0].rd;
		byte rs = mInstructions[0].rs;
		int imm32 = mInstructions[0].imm;
		JUMP_IMM16(rd, rs, imm32, AA::LS);
	}

	void ArmRecompiler::visit_JC_LT() {
		LOGC("JC_LT\n");
		byte rd = mInstructions[0].rd;
		byte rs = mInstructions[0].rs;
		int imm32 = mInstructions[0].imm;
		JUMP_IMM16(rd, rs, imm32, AA::LT);
	}

	void ArmRecompiler::visit_JC_LTU() {
		LOGC("JC_LTU\n");
		byte rd = mInstructions[0].rd;
		byte rs = mInstructions[0].rs;
		int imm32 = mInstructions[0].imm;
		JUMP_IMM16(rd, rs, imm32, AA::CC);
	}

	void ArmRecompiler::visit_JPI() {
		LOGC("JPI\n");
		int imm32 = mInstructions[0].imm;
		imm32&=mEnvironment.codeMask;
		assm.B(mPipeToArmInstMap[imm32] - (ARM_PC_ADDR) );        // jp imm32
	}

	void ArmRecompiler::visit_JPR() {
		LOGC("JPR\n");
		byte rd = mInstructions[0].rd;

		assm.MOV_imm32(AA::R2, mEnvironment.codeMask);
		assm.AND(AA::R1, loadRegister(rd, AA::R3), AA::R2);

		assm.LSL_i(AA::R1, AA::R1, 2); // *sizeof(int)
		//assm.MOV_imm32(AA::R0, (int)mPipeToArmInstMap); // r0 = pipeToArmInstMap
		assm.ADD(AA::R1, AA::R1, PIPE_TO_ARM_MAP(AA::R3));           // r0 += r1
		assm.LDR(AA::R2, 0, AA::R1);                // r1 = *r0
		assm.MOV(AA::PC, AA::R2);
	}

	void ArmRecompiler::visit_XB() {
		LOGC("XB\n");
		byte rd = mInstructions[0].rd;
		byte rs = mInstructions[0].rs;

		AA::Register saveReg = getSaveRegister(rd, AA::R2);
		if(rs<64) {
			SAVE_MEMORY(&mEnvironment.regs[rs], loadRegister(rs, AA::R3), AA::R1);
			LOAD_MEMORY_FUNC(LDRBS, &mEnvironment.regs[rs], saveReg, AA::R1);
		} else {
			LOAD_MEMORY_FUNC(LDRBS, &mEnvironment.regs[rs], saveReg, AA::R1);
		}	
		saveRegister(rd, saveReg);
	}

	void ArmRecompiler::visit_XH() {
		LOGC("XH\n");
		byte rd = mInstructions[0].rd;
		byte rs = mInstructions[0].rs;

		AA::Register saveReg = getSaveRegister(rd, AA::R2);
		if(rs<64) {
			SAVE_MEMORY(&mEnvironment.regs[rs], loadRegister(rs, AA::R3), AA::R1);
			LOAD_MEMORY_FUNC(LDRHS, &mEnvironment.regs[rs], saveReg, AA::R1);
		} else {		
			LOAD_MEMORY_FUNC(LDRHS, &mEnvironment.regs[rs], saveReg, AA::R1);
		}	
		saveRegister(rd, saveReg);
	}

	void ArmRecompiler::visit_SYSCALL() {
		LOGC("SYSCALL\n");

		InvokeSyscall_fptr fptr = &VMCore::invokeSysCall;
		int invokeSyscallAddr = FUNC_CAST(fptr);
		int syscallNumber = mInstructions[0].imm;
		//GEN_BREAKPOINT;

	//#if 1
	#if 0
		switch(syscallNumber) 
		{
	#include "invoke_syscall_arm_recompiler.h"
		}
	#else
		assm.MOV_imm32(AA::R0, (int)mEnvironment.core);
		assm.MOV_imm8(AA::R1, syscallNumber); // load syscall number into r1 (second param)

		assm.MOV_imm32(AA::R2, invokeSyscallAddr);
		//ERROR: LR gets overwritten here, so return_from_recompiled_code() can't work.

		saveStaticRegisters(assm);

		loadStackPointer(assm, AA::R3);

		assm.MOV(AA::LR, AA::PC);
		assm.MOV(AA::PC, AA::R2);
		
		saveStackPointer(assm, AA::R3);

		loadStaticRegisters(assm);
	#endif

		//assm.BX(AA::R2);

		//				if (VM_Yield)
		//				return ARM_PC;
		assm.MOV_imm32(AA::R1, (int)mEnvironment.VM_Yield);
		assm.LDR(AA::R0, 0, AA::R1);
		assm.CMP_imm8(AA::R0, 0);                   // r0 == 0

		int lastPc = ARM_PC;
		assm.B(0);
		//assm.mip++; // here we will inject a branch to be able to jump over the return.
		returnFromRecompiledCode();
		int newPc = ARM_PC;

		if(mPass!=1) {
			assm.mip = &assm.mipStart[lastPc];
			assm.SET_CONDITION_CODE(AA::EQ); 
			assm.B((newPc-(lastPc))*sizeof(AA::MDInstruction)); // inject branch
			assm.SET_CONDITION_CODE(AA::AL); 
			assm.mInstructionCount--;
			assm.mip = &assm.mipStart[newPc];
		}
	}

	void ArmRecompiler::visit_CASE() {
		LOGC("CASE\n");
		byte rd = mInstructions[0].rd;
		uint32 imm32 = mInstructions[0].imm;

		imm32 <<= 2;
		uint CaseStart = RECOMP_MEM(int, imm32, READ);
		uint CaseLength = RECOMP_MEM(int, imm32 + 1*sizeof(int), READ);
		assm.MOV_imm32(AA::R0, CaseStart);
		assm.MOV_imm32(AA::R1, CaseLength);
		assm.SUB(AA::R2, loadRegister(rd, AA::R2), AA::R0); // index

		assm.CMP(AA::R2, AA::R1);
		
		int label = ARM_PC;
		assm.B(0);

		assm.MOV_imm32(AA::R1, (int)((byte*)mEnvironment.mem_ds) + imm32);

		assm.ADD_imm8(AA::R2, AA::R2, 3);
		assm.LSL_i(AA::R2, AA::R2, 2); // *sizeof(int)
		assm.ADD(AA::R1, AA::R1, AA::R2); // addr
		assm.LDR(AA::R0, 0, AA::R1); // ip

		SET_PC(AA::R0, AA::R2);
			
		if(mPass!=1) {
			assm.SET_CONDITION_CODE(AA::HI);
			int continueLabel = ARM_PC; // jump over next branch
			assm.mInstructionCount--;
			assm.mip = &assm.mipStart[label];
			assm.B((continueLabel-label)*sizeof(AA::MDInstruction));
			assm.mip = &assm.mipStart[continueLabel];
			assm.SET_CONDITION_CODE(AA::AL);
		}

		assm.MOV_imm32(AA::R1, RECOMP_MEM(int, imm32 + 2*sizeof(int), READ));
		
		SET_PC(AA::R1, AA::R2);
	}
	
	void ArmRecompiler::visit_FAR() {
		LOGC("FAR\n");
		int op = mInstructions[0].op2;
		byte rd = mInstructions[0].rd;
		byte rs = mInstructions[0].rs;
		int imm32 = mInstructions[0].imm;

		switch(op) {
			case _CALLI: CALL_IMM_ARM(imm32) break;

			case _JC_EQ: 	JUMP_IMM16(rd, rs, imm32, AA::EQ); break;
			case _JC_NE:	JUMP_IMM16(rd, rs, imm32, AA::NE); break;
			case _JC_GE:	JUMP_IMM16(rd, rs, imm32, AA::GE); break;
			case _JC_GT:	JUMP_IMM16(rd, rs, imm32, AA::GT); break;
			case _JC_LE:	JUMP_IMM16(rd, rs, imm32, AA::LE); break;
			case _JC_LT:	JUMP_IMM16(rd, rs, imm32, AA::LT); break;
			case _JC_LTU:	JUMP_IMM16(rd, rs, imm32, AA::CC); break;
			case _JC_GEU:	JUMP_IMM16(rd, rs, imm32, AA::CS) break;
			case _JC_GTU:	JUMP_IMM16(rd, rs, imm32, AA::HI); break;
			case _JC_LEU:	JUMP_IMM16(rd, rs, imm32, AA::LS); break;

			case _JPI:		
			{
				JUMP_GEN(imm32);
			}
			break;
		}
	}
	
	ArmRecompiler::ArmRecompiler() :
		Recompiler<ArmRecompiler>(2) {
#ifdef __SYMBIAN32__
		mHeap = NULL;
		mPipeToArmInstMap = NULL;
		mInstructions = NULL;
#endif
		INSTRUCTIONS(SETUP_DEFAULT_VISITOR_ELEM);	
	}

	void ArmRecompiler::analyze() {
		int mRegisterCount[128];

		memset(mRegisterCount, 0, 128*sizeof(int));

		byte *loopWeights = new byte[mEnvironment.codeSize];
		memset(loopWeights, 0, mEnvironment.codeSize);
		const byte *ip = mEnvironment.mem_cs;
		const byte *endip = &ip[mEnvironment.codeSize];

		Instruction inst;

		do {
			const byte *lastIp = ip;

			inst.op2=_ENDOP;
			ip += decodeInstruction(ip, inst);

			byte op = inst.op;
			if(inst.op2!=_ENDOP) op = inst.op2;
			int imm32 = inst.imm;
			//int rd = inst.rd, 
				//rs = inst.rs;
			

			if(op==_JC_EQ ||
			   op==_JC_NE ||
			   op==_JC_GE ||
				op==_JC_GT ||
				op==_JC_LE ||
				op==_JC_LT ||
				op==_JC_LTU ||
				op==_JC_GEU ||
				op==_JC_GTU ||
				op==_JC_LEU ||
				op==_JPI) {
				int len = imm32 - (int)(lastIp-mEnvironment.mem_cs);
				if(len<0) {
					int i = imm32;
					for(int j = 0; j < -len; j++) { 
						loopWeights[i++]++;
					}
				}
			}
		} while(ip!=endip);

		ip = mEnvironment.mem_cs;
		do {
			int loopWeight = (int)loopWeights[(int)(ip-mEnvironment.mem_cs)];
			loopWeight = 1+loopWeight*loopWeight;
			//const byte *lastIp = ip;
			
			inst.rd = _ENDOP; inst.rs = _ENDOP;
			ip += decodeInstruction(ip, inst);

			if(inst.rd != _ENDOP) mRegisterCount[inst.rd]+=loopWeight;
			if(inst.rs != _ENDOP) mRegisterCount[inst.rs]+=loopWeight;

		} while(ip!=endip);

		FREE_ARM_REGISTERS;

		for(int i = 0; i < NUM_STATICALLY_ALLOCATED_REGISTERS; i++) {
			int max=0, maxIndex=0;
			for(int j = 0; j < 128; j++) {
				if(mRegisterCount[j]!=-1) {
					if(mRegisterCount[j]>max) {
						max = mRegisterCount[j];
						maxIndex = j;
					}
				}
			}
			registerMapping[i].armReg = freeArmRegisters[i];
			registerMapping[i].msReg = maxIndex;
			mRegisterCount[maxIndex] = -1;
		}

		delete loopWeights;

		LOG("Statically allocated registers:\n");
		for(int i = 0; i < NUM_STATICALLY_ALLOCATED_REGISTERS; i++) {
			LOG("Reg%d\n", registerMapping[i].msReg);
		}	
	}

	void ArmRecompiler::endPass() {
		if(mPass == mNumPasses) {
			generateEntryPoint();
		}
	}

	void ArmRecompiler::beginPass() {
		if(mPass==1) {			
			analyze();

			// reset
			assm.mip = tempInst;
			assm.mipStart = tempInst;
			assm.mInstructionCount = 0;
			assm.conditionCode = AA::AL;
			assm.immediatePool = 0;
			assm.immediatePoolCount = 0;
		} else {
			// allocate executable code memory
			mArmCodeSize = assm.mInstructionCount*sizeof(AA::MDInstruction);
			assm.mipStart = (AA::MDInstruction*) allocateCodeMemory(mArmCodeSize);
			assm.mip = assm.mipStart;

			// reset
			assm.mInstructionCount = 0;
			assm.conditionCode = AA::AL;
			assm.immediatePool = 0;
			assm.immediatePoolCount = 0;

#ifdef DEBUG_DISASM
			assm.verboseFlag = true;
#endif
			for(int i = 0; i < mEnvironment.codeSize; i++) mPipeToArmInstMap[i]+=(int)assm.mipStart;
		}
	}

	void ArmRecompiler::beginInstruction(int ip) {
		if(mPass==1) {			
			// reset
			assm.mip = tempInst;
			mPipeToArmInstMap[ip] = assm.mInstructionCount*sizeof(AA::MDInstruction);
		} else {
#ifdef DEBUG_DISASM
		char buf[1024];
		disassemble_one(&mEnvironment.mem_cs[ip], mEnvironment.mem_cs, mEnvironment.mem_cp, buf);
		assm.console->format("%s", buf);
#endif
		}

#ifdef LOG_STATE_CHANGE
			emitOneArgFuncCall(&ArmRecompiler::logStateChange, ip);
#endif	
	}
#ifdef LOG_STATE_CHANGE
	void ArmRecompiler::logStateChange(int ip) {
		mEnvironment.core->logStateChange(ip);
	}
#endif

	void ArmRecompiler::debugBreak(int a) {
#ifdef _WIN32	
		DebugBreak();
#endif
	}

	void ArmRecompiler::beginFunction(Function *f) {
		// align functions to cache line size.
		if(((this->assm.mInstructionCount<<2)&(CACHE_LINE_SIZE-1)) != 0) {
			while(((this->assm.mInstructionCount<<2)&(CACHE_LINE_SIZE-1)) != 0) {
				this->assm.IMM32(0);
				this->assm.mInstructionCount++;
			}
		}
	}

	void ArmRecompiler::endFunction(Function *f) {
	}

	int ArmRecompiler::run(int ip) {
		//LOG("ArmRecompiler::run(%i)\n", ip);
		if(mStopped) {
			LOG("Stopped, Recompiling..\n");
			Recompiler<ArmRecompiler>::recompile();
			//LOG("Finished recompiling..\n");
			ip = (int)mPipeToArmInstMap[mEnvironment.entryPoint];
			mStopped = false;
		}
		int arm_ip = ((int (*)(int))entryPoint.mipStart)(ip);
		return arm_ip;
	}

	int ArmRecompiler::shiftAriMatcher() {
		const Instruction& i1 = mInstructions[0];
		const Instruction& i2 = mInstructions[1];
		if(i1.rd == i2.rs && i2.rd == i2.rs) return 1;
		else return 0;
	}

	void ArmRecompiler::shiftAriVisitor() {
#ifdef _WIN32	
		DebugBreak();
#endif
	}

#if 0	
#ifdef __SYMBIAN32__	
static void MyExceptionHandlerL(TExcType aType)
{
	LOG("Exception type: %d", aType);
   User::Leave(KErrGeneral);
   
   /*
   switch (aType)
       {
       case EExcAccessViolation:
           {
          // console->Printf(_L("Access Voilation Exception\n"));
		   User::After(TTimeIntervalMicroSeconds32(1000000));
           User::Leave(KErrGeneral);
           break;        
           }
        //You can add more exceptions which are defined in e32const.h in TExcType Enumerator
       }
	   */
}
#endif	
#endif // 0	
	void ArmRecompiler::init(Core::VMCore *core, int *VM_Yield) {
		Recompiler<ArmRecompiler>::init(core, VM_Yield);
		LOG("initRecompilerVariables\n");
		assm.mipStart = NULL;
		mPipeToArmInstMap = NULL;
		entryPoint.mipStart = NULL;

#ifdef __SYMBIAN32__
		//mHeap = NULL;
#if 0
	#ifdef EKA2
	User::SetExceptionHandler(MyExceptionHandlerL, KExceptionFault|KExceptionInteger|KExceptionAbort|KExceptionKill|KExceptionFpe|KExceptionUserInterrupt);
	#else
	RThread().SetExceptionHandler(MyExceptionHandlerL, KExceptionFault|KExceptionInteger|KExceptionAbort|KExceptionKill|KExceptionFpe|KExceptionUserInterrupt);
	#endif		
#endif // 0
#endif // __SYMBIAN32__
		mPipeToArmInstMap = new
	#ifdef __SYMBIAN32__
		(ELeave)
	#endif
		AA::MDInstruction[mEnvironment.codeSize];

		byte shifts[] = {
			_SLLI,
			_SRAI,
			_SRLI
		};

		byte arithmetics[] = {
			_SLL,
			_SRA,
			_SRL,
			_AND,
			_OR,
			_XOR,
			_ADD,
			_MUL,
			_SUB,
			_LDR
		};

		byte pattern[3];
		pattern[2] = _NUL;
		for(unsigned int i = 0; i < sizeof(shifts); i++) {
			pattern[0] = shifts[i];
			for(unsigned int j = 0; j < sizeof(arithmetics); j++) {
				pattern[1] = arithmetics[j];				
				setPattern(&ArmRecompiler::shiftAriMatcher, &ArmRecompiler::shiftAriVisitor, pattern);
			}

		}

		mInstructions = new Instruction[mInstructionsToFetch];
	}

	void ArmRecompiler::close() {
		LOG("close\n");
		Recompiler<ArmRecompiler>::close();
		if(assm.mipStart) {
			LOG("freeCodeMemory(assm.mipStart\n");
			freeCodeMemory(assm.mipStart);
			assm.mipStart = NULL;
		}
		if(entryPoint.mipStart) {
			LOG("freeCodeMemory(entryPoint.mipStart\n");
			freeCodeMemory(entryPoint.mipStart);
			entryPoint.mipStart = NULL;
		}
	#ifdef __SYMBIAN32__
		LOG("if(mHeap != NULL)\n");
		if(mHeap != NULL) {
			LOG("heapClose\n");
			mHeap->Close();
			LOG("mHeap = NULL;");
			mHeap = NULL;
	#if 0	//fails; maybe mHeap->Close() does what we need.
			LOG("chunkClose start\n");
			mChunk.Close();
			LOG("chunkClose done\n");
	#endif
		}
	#endif
		LOG("if(mPipeToArmInstMap) (%i)\n", mPipeToArmInstMap);
		if(mPipeToArmInstMap)
			delete mPipeToArmInstMap;
	}

} // namespace MoSync

#endif // USE_ARM_RECOMPILER
