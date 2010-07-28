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

#ifndef _RECOMPILER_H_
#define _RECOMPILER_H_

#include <config_platform.h>

#ifdef USE_ARM_RECOMPILER

#include <Core.h>
#include "Recompiler.h"
#include "disassembler.h"

/*
#define MIN(x, y) ((x)<(y)?(x):(y))
#define MAX(x, y) ((x)>(y)?(x):(y))
*/
#define DECLARE_DEFAULT_VISITOR_ELEM(inst) void visit_ ## inst();

#define RECOMP_RAW_MEMREF(type, addr) (*(type*)(((char*)mEnvironment.mem_ds) + (addr)))
#define RECOMP_MEMREF(type, addr) RECOMP_RAW_MEMREF(type, \
	(addr) & mEnvironment.dataMask & ~(sizeof(type) - 1))
#define RECOMP_MEM(type, addr, write) RECOMP_MEMREF(type, addr)

namespace MoSync {

	// struct used to hold a decoded mosync instruction
	struct Instruction {
		byte op; // operation
		byte op2; // if multiop (FAR)
		byte rd; // destination register
		byte rs; // source register
		int imm; // immediate value

		int ip; // address of the instruction
		byte length; // instruction length in bytes
	};

	class VMCoreInt;

	typedef void (Core::VMCore::*InvokeSyscall_fptr)(int);

	struct Environment {
		Core::VMCore *core;
		int *VM_Yield;
		int entryPoint;
		const byte *mem_cs;
		int codeSize;
		int codeMask;
		const int *mem_ds;
		int dataSize;
		int dataMask;
		const int *mem_cp;
		int constantPoolSize;
		int *regs;
	};

	// function pointer for the 



	template<typename T>
	class Recompiler {
	public:
		typedef void (T::*Visitor)();
		typedef int (T::*Matcher)();

		// make a tree for all sequences possible, and use it for early outs.
		struct InstructionPatternNode {
			InstructionPatternNode(InstructionPatternNode *parent=NULL, int depth=0) : depth(depth), matcher(0), visitor(0), parent(parent) {
				memset(children, 0, sizeof(InstructionPatternNode*));
			}

			int depth;
			Matcher matcher;
			Visitor visitor;
			InstructionPatternNode *children[Core::_ENDOP];
			InstructionPatternNode *parent;
		};

		Recompiler(int numPasses) :
			mInstructionsToFetch(1),
			mNumPasses(numPasses),
			mStopped(true) {
		}
		virtual ~Recompiler() {}

		// should be used from the constructor
		void setPattern(Matcher m, Visitor v, byte *sequence) {
			int length = 0;
			InstructionPatternNode *node = &mPatternNodeRoot;
			if(*sequence == Core::_NUL) {
				// error
			}

			while(*sequence != Core::_NUL) {
				node = node->children[*sequence] = new InstructionPatternNode(node, length+1);
				sequence++;
				length++;
			}

			node->matcher = m;
			node->visitor = v;
			if(length>mInstructionsToFetch) mInstructionsToFetch = length;
		}

		inline InstructionPatternNode* findLongestPattern(T* thisImpl) {
			InstructionPatternNode *node = &mPatternNodeRoot;
			int ipOfNextLabel;
			if(mNextLabel) ipOfNextLabel = mNextLabel->ip;
			else ipOfNextLabel = 0x7fffffff;
			for(int i = 0; i <= mInstructionsToFetch; i++) {
				InstructionPatternNode *n = node->children[mInstructions[i].op];
				if(n && mInstructions[i].ip < ipOfNextLabel) {
					node = n;
				} else {
					if(i != 0 && node->matcher) {
						if((thisImpl->*node->matcher)())
							return node;
					}
					node = node->parent;
					while(node) {
						if(node->matcher) {
							if((thisImpl->*node->matcher)())
								return node;
							else
								node = node->parent;
						} else return 0;
					}
					return 0;
				}
			}
			return 0;
		}

		virtual void init(Core::VMCore *core, int *VM_Yield) {
			mEnvironment.core = core;
			mEnvironment.VM_Yield = VM_Yield;
			mEnvironment.entryPoint = core->Head.EntryPoint;
			mEnvironment.mem_cs = core->mem_cs;
			mEnvironment.codeSize = core->Head.CodeLen;
			mEnvironment.codeMask = core->CODE_SEGMENT_SIZE-1;
			mEnvironment.mem_ds = core->mem_ds;
			mEnvironment.dataSize = core->Head.DataLen;
			mEnvironment.dataMask = core->DATA_SEGMENT_SIZE-1;
			mEnvironment.mem_cp = core->mem_cp;
			mEnvironment.constantPoolSize = core->Head.IntLen*4;
			mEnvironment.regs = core->regs;
		}

		virtual void close() {
			delete mInstructions;
		}

		struct Label {
			Label(int ip) : ip(ip), next(0) {
			}

			int ip;
			Label *next;
		};

		struct Function {
			Function(int start, int end) : 
				start(start), end(end), labels(0), next(0) {
				addLabel(start);	
			}
			Label *findLabel(int ip) {
				Label *l = labels;
				Label *potential = labels;
				while(l) {
					if(ip == l->ip) {
						return l;
						
					} else if(ip > l->ip) {
						potential = l;
					}
					l = l->next;
				}
				return potential;
			}

			Label *findNextLabel(int ip) {
				Label *l;
				if(l = findLabel(ip)) return l->next;
				else return 0;
			}

			void addLabel(int ip) {
				//Label *l = labels;
				if(!labels) {
					labels = new Label(ip);
				} else {
					Label *l = findLabel(ip);
					if(l->ip != ip) {
						Label *next = l->next;
						l->next = new Label(ip);
						l->next->next = next;
					}
				}
			}

			int start, end;
			Label *labels;
			Function *next;
		};

		Function* findFunctions() {
			int ip = 1;
			Function *start;
			Function *f = start = new Function(0, 0);
			Instruction inst;
			while(ip != mEnvironment.codeSize) {
				ip+=decodeInstruction(&mEnvironment.mem_cs[ip], inst);

				if(inst.op == Core::_FAR) inst.op = inst.op2;
				switch(inst.op) {
					case Core::_RET :
					{
endOfFunction:
						int ipOfInstruction = ip-inst.length;
						Label *l;
						l = f->findNextLabel(ipOfInstruction);
						if(!l) {
							f->end = ip;
							Function *next = new Function(ip, 0);
							f->next = next;
							f = f->next;
						}
					}
					break;
					case Core::_CASE: 
					{
						inst.imm<<=2;
						//uint CaseStart = RECOMP_MEM(int, inst.imm, READ);
						uint CaseLength = RECOMP_MEM(int, inst.imm + 1*sizeof(int), READ);
						int tableAddress = inst.imm + 3*sizeof(int);
						int defaultCaseAddress = RECOMP_MEM(int, inst.imm + 2*sizeof(int), READ);

						int maxJump = -1;
						for(size_t i = 0; i < CaseLength; i++) {
							int j = RECOMP_MEM(int, tableAddress+i*sizeof(int), READ);
							if(j > maxJump) j = maxJump;						
							f->addLabel(j);
						}
						f->addLabel(defaultCaseAddress);
						if(defaultCaseAddress>maxJump) maxJump=defaultCaseAddress;
				
						if(maxJump<ip) {
							goto endOfFunction;
						}
					}
					break;
					case Core::_JPI:
					if(inst.imm<ip) {
						f->addLabel(inst.imm);
						goto endOfFunction;
					}

					case Core::_JC_EQ:
					case Core::_JC_NE:
					case Core::_JC_GE:
					case Core::_JC_GEU:
					case Core::_JC_GT:
					case Core::_JC_GTU:
					case Core::_JC_LE:
					case Core::_JC_LEU:
					case Core::_JC_LT:
					case Core::_JC_LTU:
						f->addLabel(inst.imm);
					break;
					default: break;
				}
			}
			return start;
		}

		void printFunctions(Function *fb) {
			while(fb) {
				LOG("function (%x:%x)\n", fb->start, fb->end);
				fb = fb->next;
			}
		}

		void printFunctionsSize(Function *fb) {
			int size = 0;
			while(fb) {
				Label *l = fb->labels;
				size+=sizeof(Function);
				while(l) {
					size+=sizeof(Label);
					l = l->next;
				}
				fb = fb->next;
			}
			LOG("size of function bounds: %d\n", size);
		}

		void recompile() {
			mFunctions = findFunctions();
			//printFunctionsSize(mFunctions);
			//printFunctions(fb);
			for(mPass = 1; mPass <= mNumPasses; mPass++) { 
				int ip = 1, windowIp = 1;
				int numInstructions = 0;
				T* thisImpl = ((T*)this);

				mCurrentFunction = mFunctions;
				mNextLabel = mCurrentFunction->labels;
				if(mNextLabel) mNextLabel = mNextLabel->next;
				//thisImpl->beginInstruction(ip);
				thisImpl->beginFunction(mCurrentFunction);

				thisImpl->beginPass();
				while(ip != mEnvironment.codeSize) {
					/*
					ip+=decodeInstruction(&mEnvironment.mem_cs[ip], mInstructions[numInstructions]);
					(thisImpl->*defaultVisitors[mInstructions[0].op])();
					*/
					thisImpl->beginInstruction(ip);	

					// fetch instruction window
					for(; numInstructions < mInstructionsToFetch; numInstructions++) {
						if(windowIp==mEnvironment.codeSize) break;
						windowIp+=decodeInstruction(&mEnvironment.mem_cs[windowIp], mInstructions[numInstructions]);
					}
					if(numInstructions == 0) break;

					// find pattern
					//InstructionPatternNode* node = findLongestPattern(thisImpl);
					//InstructionPatternNode* node = 0;
					//if(!node) {
						(thisImpl->*defaultVisitors[mInstructions[0].op])();
						ip+=mInstructions[0].length;
						for(int i = 1; i < numInstructions; i++) {   
							mInstructions[i-1] = mInstructions[i];
						}		
						numInstructions -= 1;
					/*
					} else {
						// visit pattern
						(thisImpl->*node->visitor)();
						int nodeLen = node->depth;
						for(int i = nodeLen; i < numInstructions; i++) {
							mInstructions[i-nodeLen] = mInstructions[i];
						}
						numInstructions -= nodeLen;
					}
					*/

					if(ip>mCurrentFunction->end) { 
						thisImpl->endFunction(mCurrentFunction);
						mCurrentFunction = mCurrentFunction->next; 
						mNextLabel = mCurrentFunction->labels;
						if(mNextLabel) mNextLabel = mNextLabel->next;
						thisImpl->beginFunction(mCurrentFunction);
						thisImpl->beginInstruction(ip);
					}
					else if(mNextLabel && ip>mNextLabel->ip) {
						mNextLabel = mNextLabel->next;
					} 
	
				}
				thisImpl->endPass();
			}
		}

		virtual int run(int ip) = 0;

	protected:
		int decodeInstruction(const byte *ip, Instruction& inst) {
//		int Recompiler::decodeInstruction(const byte *ip, Instruction& inst) {
			inst.ip = (int)(ip-mEnvironment.mem_cs);
			inst.length = disassemble_one(ip, mEnvironment.mem_cs, mEnvironment.mem_cp, (char*)NULL, inst.op, inst.op2, inst.rd, inst.rs, inst.imm);
			return inst.length;
		}

		const byte *mIp;

		Instruction *mInstructions;

		InstructionPatternNode mPatternNodeRoot;
		int mInstructionsToFetch;

		Environment mEnvironment;

		int mNumPasses;
		int mPass;

		Visitor defaultVisitors[Core::_ENDOP];

		bool mStopped;

		Function *mFunctions;
		Function *mCurrentFunction;
		Label *mNextLabel;
	};

} // namespace MoSync

#endif	//USE_ARM_RECOMPILER

#endif
