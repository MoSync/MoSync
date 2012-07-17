using System;
using System.IO;
using System.Collections.Generic;

// This is the core that interprets mosync
// byte code (produced by pipe-tool).
// It has dependencies on some idl compiler
// generated code, such as the SyscallInvoker

namespace MoSync
{
    public class CoreInterpreted : Core
    {
        protected ProgramHeader mProgramHeader = new ProgramHeader();
        protected int mIp;
        protected byte[] mProgramMemory;
        protected uint mProgramSegmentSize;
        protected uint mProgramSegmentMask;
        protected int[] mConstantPool;
        protected int[] mRegisters = new int[128];
        protected SyscallInvoker mSyscallInvoker;

        protected Stream mProgramFile;

        public class ProgramHeader
        {
            public uint mMagic;
            public uint mProgramLength;
            public uint mDataLength;
            public uint mDataSize;
            public uint mStackSize;
            public uint mHeapSize;
            public uint mAppCode;
            public uint mAppID;
            public uint mEntryPoint;
            public uint mIntLength;
        }

        public class Op
        {
            public const byte NUL = 0;
            public const byte PUSH = 1;
            public const byte POP = 2;
            public const byte CALL = 3;
            public const byte CALLI = 4;
            public const byte LDB = 5;
            public const byte STB = 6;
            public const byte LDH = 7;
            public const byte STH = 8;
            public const byte LDW = 9;
            public const byte STW = 10;
            public const byte LDI = 11;
            public const byte LDR = 12;
            public const byte ADD = 13;
            public const byte ADDI = 14;
            public const byte MUL = 15;
            public const byte MULI = 16;
            public const byte SUB = 17;
            public const byte SUBI = 18;
            public const byte AND = 19;
            public const byte ANDI = 20;
            public const byte OR = 21;
            public const byte ORI = 22;
            public const byte XOR = 23;
            public const byte XORI = 24;
            public const byte DIVU = 25;
            public const byte DIVUI = 26;
            public const byte DIV = 27;
            public const byte DIVI = 28;
            public const byte SLL = 29;
            public const byte SLLI = 30;
            public const byte SRA = 31;
            public const byte SRAI = 32;
            public const byte SRL = 33;
            public const byte SRLI = 34;
            public const byte NOT = 35;
            public const byte NEG = 36;
            public const byte RET = 37;
            public const byte JC_EQ = 38;
            public const byte JC_NE = 39;
            public const byte JC_GE = 40;
            public const byte JC_GEU = 41;
            public const byte JC_GT = 42;
            public const byte JC_GTU = 43;
            public const byte JC_LE = 44;
            public const byte JC_LEU = 45;
            public const byte JC_LT = 46;
            public const byte JC_LTU = 47;
            public const byte JPI = 48;
            public const byte JPR = 49;
            public const byte XB = 50;
            public const byte XH = 51;
            public const byte SYSCALL = 52;
            public const byte CASE = 53;
            public const byte FAR = 54;
            public const byte END = 55;
        }

        public class Reg
        {
            public const int ZERO = 0;
            public const int SP = 1;
            public const int RT = 2;
            public const int FR = 3;

            public const int D0 = 4;
            public const int D1 = 5;
            public const int D2 = 6;
            public const int D3 = 7;
            public const int D4 = 8;
            public const int D5 = 9;
            public const int D6 = 10;
            public const int D7 = 11;

            public const int I0 = 12;
            public const int I1 = 13;
            public const int I2 = 14;
            public const int I3 = 15;

            public const int R0 = 16;
            public const int R1 = 17;
            public const int R2 = 18;
            public const int R3 = 19;
            public const int R4 = 20;
            public const int R5 = 21;
            public const int R6 = 22;
            public const int R7 = 23;
            public const int R8 = 24;
            public const int R9 = 25;
            public const int R10 = 26;
            public const int R11 = 27;
            public const int R12 = 28;
            public const int R13 = 29;
            public const int R14 = 30;
            public const int R15 = 31;
        }

        public CoreInterpreted(Stream programFile)
        {
            mProgramFile = programFile;
        }


        public override void Init()
        {
            LoadProgram(mProgramFile);

            base.Init();
            Start();
            mIp = (int)mProgramHeader.mEntryPoint;

            if (mRuntime == null)
                MoSync.Util.CriticalError("No runtime!");

            mSyscallInvoker = new SyscallInvoker(this, mRuntime.GetSyscalls());
        }

        public int GetRegisterValue(int reg)
        {
            return mRegisters[reg];
        }

        public void SetReturnValue(int value)
        {
            mRegisters[Reg.R14] = value;
        }

        public void SetReturnValue(double value)
        {
            ulong ret = (ulong)BitConverter.DoubleToInt64Bits(value);
            mRegisters[Reg.R14] = (int)(ret & 0xffffffffL);
            mRegisters[Reg.R15] = (int)(ret >> 32);
        }

        public void SetReturnValue(float value)
        {
            mRegisters[Reg.R14] = BitConverter.ToInt32(BitConverter.GetBytes(value), 0);
        }

        public void SetReturnValue(long value)
        {
            mRegisters[Reg.R14] = (int)(value & 0xffffffff);
            mRegisters[Reg.R15] = (int)(((ulong)value) >> 32);
        }

        public override int GetStackPointer()
        {
            return mRegisters[Reg.SP];
        }

        private void GenerateConstantTable()
        {
            int p = 0;
            for (p = 0; p < 32; p++)
            {
                mRegisters[p] = 0;
            }

            for (int n = 1; n < 17; n++)
            {
                mRegisters[p++] = n;
                mRegisters[p++] = (-n);
            }

            int mask = 0x20;

            for (int n = 0; n < 32 - 5; n++)
            {
                mRegisters[p++] = (mask - 1);
                mRegisters[p++] = (mask);
                mask <<= 1;
            }

            mask = 0x10;

            for (int n = 0; n < 10; n++)
            {
                mRegisters[p++] = (int)(mask ^ 0xffffffff);
                mask <<= 1;
            }
        }

        protected void LoadProgram(Stream program)
        {
            mProgramHeader.mMagic = Util.StreamReadUint32(program);
            mProgramHeader.mProgramLength = Util.StreamReadUint32(program);
            mProgramHeader.mDataLength = Util.StreamReadUint32(program);
            mProgramHeader.mDataSize = Util.StreamReadUint32(program);
            mProgramHeader.mStackSize = Util.StreamReadUint32(program);
            mProgramHeader.mHeapSize = Util.StreamReadUint32(program);
            mProgramHeader.mAppCode = Util.StreamReadUint32(program);
            mProgramHeader.mAppID = Util.StreamReadUint32(program);
            mProgramHeader.mEntryPoint = Util.StreamReadUint32(program);
            mProgramHeader.mIntLength = Util.StreamReadUint32(program);

            if (mProgramHeader.mMagic != 0x5844414d)
                Util.CriticalError("Invalid magic!");


            mIp = (int)mProgramHeader.mEntryPoint;


            mProgramSegmentSize = Util.NextPowerOfTwo(2, mProgramHeader.mProgramLength);
            mProgramSegmentMask = mProgramSegmentSize - 1;
            mProgramMemory = new byte[mProgramSegmentSize];

            for (int i = 0; i < mProgramHeader.mProgramLength; i++)
            {
                mProgramMemory[i] = Util.StreamReadUint8(program);
            }

            mDataSegmentSize = Util.NextPowerOfTwo(2, mProgramHeader.mDataSize);
            mDataSegmentMask = mDataSegmentSize - 1;
            mDataMemory = new Memory((int)mDataSegmentSize); // new int[mDataSegmentSize >> 2];

            for (int i = 0; i < mProgramHeader.mDataLength; i++)
            {
                mDataMemory.WriteUInt8(i, Util.StreamReadUint8(program));
            }


            mConstantPool = new int[mProgramHeader.mIntLength];
            for (int i = 0; i < mProgramHeader.mIntLength; i++)
            {
                mConstantPool[i] = Util.StreamReadInt32(program);
            }

            GenerateConstantTable();

            uint customEventDataSize = 60;
            mRegisters[Reg.SP] = (int)(mDataSegmentSize - customEventDataSize - 4);
            mRegisters[Reg.I0] = (int)mDataSegmentSize;
            mRegisters[Reg.I1] = (int)mProgramHeader.mStackSize;
            mRegisters[Reg.I2] = (int)mProgramHeader.mHeapSize;
            mCustomEventPointer = (int)(mDataSegmentSize - customEventDataSize);
        }


        // ---------------- BEGIN FUGLY DEBUG FACILITIES ----------------------------
        struct StateChange
        {
            public int type; // 0 = reg, 1 = mem
            public int ip, instCount;
            public int reg;
            public int address;
            public int before, after;
        };

        private bool mDebugLogStateChangesInitialized = false;
        private int[] mOldRegisters = new int[128];
        private List<StateChange> mStateChanges = new List<StateChange>();
        private int mStateChangeInstCount = 0;
        private byte[] mStateChangeOldMemory;

        private void DebugWriteStateChanges()
        {
            System.Text.StringBuilder res = new System.Text.StringBuilder();

            foreach (StateChange s in mStateChanges)
            {
                if (s.type == 0)
                {
                    res.Append(String.Format("{0:x}, {1:d}: REG{2:d}: {3:x} != {4:x}\n",
                        s.ip, s.instCount, s.reg, s.before, s.after));
                }
                else
                {
                    res.Append(String.Format("{0:x}, {1:d}: ADDR(0x{2:x}): {3:x} != {4:x}\n",
                        s.ip, s.instCount, s.address, s.before, s.after));
                }
            }

            MoSync.Util.WriteTextToFile(res.ToString(), "stateChanges.txt", FileMode.Append);
        }

        private void DebugLogStateChanges()
        {
            const int STATE_BUFFER_SIZE = 1024;

            if (!mDebugLogStateChangesInitialized)
            {
                mDebugLogStateChangesInitialized = true;
                System.Array.Copy(mRegisters, mOldRegisters, 128);
                mStateChangeInstCount = 0;
                MoSync.Util.WriteTextToFile("", "stateChanges.txt", FileMode.Create);
                mStateChangeOldMemory = new byte[mDataMemory.GetSizeInBytes()];
                mDataMemory.ReadBytes(mStateChangeOldMemory, 0, mDataMemory.GetSizeInBytes());
            }

            int i = 128;
            mStateChangeInstCount++;
            while ((i--) != 0)
            {
                if (mOldRegisters[i] != mRegisters[i])
                {
                    StateChange stateChange = new StateChange();
                    stateChange.type = 0;
                    stateChange.ip = mIp;
                    stateChange.reg = i;
                    stateChange.before = mOldRegisters[i];
                    stateChange.after = mRegisters[i];
                    stateChange.instCount = mStateChangeInstCount;
                    mStateChanges.Add(stateChange);
                    if (mStateChanges.Count >= STATE_BUFFER_SIZE)
                    {
                        DebugWriteStateChanges();
                        mStateChanges.Clear();
                    }

                    mOldRegisters[i] = mRegisters[i];
                }
            }

            for (i = 0; i < mStateChangeOldMemory.Length; i++)
            {
                byte newByte = mDataMemory.ReadUInt8(i);
                if (mStateChangeOldMemory[i] != newByte)
                {
                    StateChange stateChange = new StateChange();
                    stateChange.type = 1;
                    stateChange.ip = mIp;
                    stateChange.address = i;
                    stateChange.before = mStateChangeOldMemory[i];
                    stateChange.after = newByte;
                    stateChange.instCount = mStateChangeInstCount;
                    mStateChanges.Add(stateChange);
                    if (mStateChanges.Count >= STATE_BUFFER_SIZE)
                    {
                        DebugWriteStateChanges();
                        mStateChanges.Clear();
                    }

                    mStateChangeOldMemory[i] = newByte;
                }
            }
        }

        public void DebugDumpState()
        {
            System.Text.StringBuilder res = new System.Text.StringBuilder();
            for (int i = 0; i < mRegisters.Length; i++)
            {
                res.Append(String.Format("REG{0:d}: {1:x}\n", i, mRegisters[i]));
            }

            int dataMemoryLength = mDataMemory.GetSizeInBytes();
            for (int i = 0; i < dataMemoryLength; i++)
            {
                res.Append(String.Format("ADDR({0:x}): {1:x}\n", i, mDataMemory.ReadUInt8(i)));
            }

            int constantPoolSize = mConstantPool.Length;
            for (int i = 0; i < constantPoolSize; i++)
            {
                res.Append(String.Format("CONSTANT({0:d}): {1:x}\n", i, mConstantPool[i]));
            }

            for (int i = 0; i < mProgramMemory.Length; i++)
            {
                res.Append(String.Format("CODE({0:x}): {1:x}\n", i, mProgramMemory[i]));
            }

            MoSync.Util.WriteTextToFile(res.ToString(), "initialstate.txt", FileMode.Create);
        }

        // ---------------- END FUGLY DEBUG FACILITIES ----------------------------


        public override void Run()
        {
            int imm32;
            byte rd;
            byte rs;

            int oldIp = 0;

            //DebugDumpState();

            //while(mRunning)
            while (true)
            {
                //DebugLogStateChanges();
                /*
                if (mIp == 16754)
                {
                    int a = 2;
                }
                */

                oldIp = mIp;

                byte op = mProgramMemory[mIp++];
                switch (op)
                {
                    case 0:
                        break;

                    case Op.PUSH: // PUSH
                        rd = mProgramMemory[mIp++];
                        imm32 = mProgramMemory[mIp++];
                        if (rd < 2 || rd + imm32 > 32)
                            MoSync.Util.CriticalError("Hell");
                        do
                        {
                            mRegisters[Reg.SP] -= 4;
                            mDataMemory.WriteInt32(mRegisters[Reg.SP], mRegisters[rd]);
                            rd++;
                            imm32--;
                        } while (imm32 != 0);
                        break;

                    case Op.POP: // POP
                        rd = mProgramMemory[mIp++];
                        imm32 = mProgramMemory[mIp++];
                        if (rd > 31 || rd - imm32 < 1)
                            MoSync.Util.CriticalError("Hell");
                        do
                        {
                            mRegisters[rd] = mDataMemory.ReadInt32(mRegisters[Reg.SP]);
                            mRegisters[Reg.SP] += 4;
                            rd--;
                            imm32--;
                        } while (imm32 != 0);
                        break;

                    case Op.CALL: // CALL
                        rd = mProgramMemory[mIp++];
                        mRegisters[Reg.RT] = mIp;
                        mIp = mRegisters[rd];
                        break;

                    case Op.CALLI: // CALLI
                        imm32 = ((int)mProgramMemory[mIp++] << 8) | ((int)mProgramMemory[mIp++]);
                        //console.log("calli addr: " + callAddr);
                        mRegisters[Reg.RT] = mIp;
                        mIp = imm32;
                        break;

                    case Op.LDB: // LDB
                        {
                            rd = mProgramMemory[mIp++];
                            rs = mProgramMemory[mIp++];
                            imm32 = ((imm32 = mProgramMemory[mIp++]) > 127) ?
                                mConstantPool[(((imm32 & 127) << 8) | mProgramMemory[mIp++])] :
                                mConstantPool[imm32];
                            //mRegisters[rd] = mDataMemory.ReadUInt8(mRegisters[rs] + imm32);
                            mRegisters[rd] = mDataMemory.ReadInt8(mRegisters[rs] + imm32);
                        }
                        break;

                    case Op.STB: // STB
                        {
                            rd = mProgramMemory[mIp++];
                            rs = mProgramMemory[mIp++];
                            imm32 = ((imm32 = mProgramMemory[mIp++]) > 127) ?
                                mConstantPool[(((imm32 & 127) << 8) | mProgramMemory[mIp++])] :
                                mConstantPool[imm32];
                            mDataMemory.WriteUInt8(mRegisters[rd] + imm32, (byte)mRegisters[rs]);
                        }
                        break;

                    case Op.LDH: // LDH
                        {
                            rd = mProgramMemory[mIp++];
                            rs = mProgramMemory[mIp++];
                            imm32 = ((imm32 = mProgramMemory[mIp++]) > 127) ?
                                mConstantPool[(((imm32 & 127) << 8) | mProgramMemory[mIp++])] :
                                mConstantPool[imm32];
                            mRegisters[rd] = mDataMemory.ReadInt16(mRegisters[rs] + imm32);
                        }
                        break;

                    case Op.STH: // STH
                        {
                            rd = mProgramMemory[mIp++];
                            rs = mProgramMemory[mIp++];
                            imm32 = ((imm32 = mProgramMemory[mIp++]) > 127) ?
                                mConstantPool[(((imm32 & 127) << 8) |
                                mProgramMemory[mIp++])] :
                                mConstantPool[imm32];
                            mDataMemory.WriteUInt16(mRegisters[rd] + imm32, (ushort)mRegisters[rs]);
                        }
                        break;

                    case Op.LDW: // LDW
                        rd = mProgramMemory[mIp++];
                        rs = mProgramMemory[mIp++];
                        imm32 = ((imm32 = mProgramMemory[mIp++]) > 127) ?
                            mConstantPool[(((imm32 & 127) << 8) | mProgramMemory[mIp++])] :
                            mConstantPool[imm32];
                        mRegisters[rd] = mDataMemory.ReadInt32(mRegisters[rs] + imm32);
                        break;

                    case Op.STW: // STW
                        {
                            rd = mProgramMemory[mIp++];
                            rs = mProgramMemory[mIp++];
                            imm32 = ((imm32 = mProgramMemory[mIp++]) > 127) ?
                                mConstantPool[(((imm32 & 127) << 8) | mProgramMemory[mIp++])] :
                                mConstantPool[imm32];
                            mDataMemory.WriteInt32(mRegisters[rd] + imm32, mRegisters[rs]);
                        }
                        break;

                    case Op.LDI: // LDI
                        rd = mProgramMemory[mIp++];
                        imm32 = ((imm32 = mProgramMemory[mIp++]) > 127) ?
                            mConstantPool[(((imm32 & 127) << 8) | mProgramMemory[mIp++])] :
                            mConstantPool[imm32];
                        mRegisters[rd] = imm32;
                        break;

                    case Op.LDR: // LDR
                        rd = mProgramMemory[mIp++];
                        rs = mProgramMemory[mIp++];
                        mRegisters[rd] = mRegisters[rs];
                        break;

                    case Op.ADD: // ADD
                        rd = mProgramMemory[mIp++];
                        rs = mProgramMemory[mIp++];
                        mRegisters[rd] += mRegisters[rs];
                        break;

                    case Op.ADDI: // ADDI
                        rd = mProgramMemory[mIp++];
                        imm32 = ((imm32 = mProgramMemory[mIp++]) > 127) ? mConstantPool[(((imm32 & 127) << 8) | mProgramMemory[mIp++])] : mConstantPool[imm32];
                        mRegisters[rd] += imm32;
                        break;

                    case Op.MUL: // MUL
                        rd = mProgramMemory[mIp++];
                        rs = mProgramMemory[mIp++];
                        mRegisters[rd] *= mRegisters[rs];
                        break;

                    case Op.MULI: // MULI
                        rd = mProgramMemory[mIp++];
                        imm32 = ((imm32 = mProgramMemory[mIp++]) > 127) ? mConstantPool[(((imm32 & 127) << 8) | mProgramMemory[mIp++])] : mConstantPool[imm32];
                        mRegisters[rd] *= imm32;
                        break;

                    case Op.SUB: // SUB
                        rd = mProgramMemory[mIp++];
                        rs = mProgramMemory[mIp++];
                        mRegisters[rd] -= mRegisters[rs];
                        break;


                    case Op.SUBI: // SUBI
                        rd = mProgramMemory[mIp++];
                        imm32 = ((imm32 = mProgramMemory[mIp++]) > 127) ? mConstantPool[(((imm32 & 127) << 8) | mProgramMemory[mIp++])] : mConstantPool[imm32];
                        mRegisters[rd] -= imm32;
                        break;

                    case Op.AND: // AND
                        rd = mProgramMemory[mIp++];
                        rs = mProgramMemory[mIp++];
                        mRegisters[rd] &= mRegisters[rs];
                        break;

                    case Op.ANDI: // ANDI
                        rd = mProgramMemory[mIp++];
                        imm32 = ((imm32 = mProgramMemory[mIp++]) > 127) ? mConstantPool[(((imm32 & 127) << 8) | mProgramMemory[mIp++])] : mConstantPool[imm32];
                        mRegisters[rd] &= imm32;
                        break;

                    case Op.OR: // OR
                        rd = mProgramMemory[mIp++];
                        rs = mProgramMemory[mIp++];
                        mRegisters[rd] |= mRegisters[rs];
                        break;

                    case Op.ORI: // ORI
                        rd = mProgramMemory[mIp++];
                        imm32 = ((imm32 = mProgramMemory[mIp++]) > 127) ? mConstantPool[(((imm32 & 127) << 8) | mProgramMemory[mIp++])] : mConstantPool[imm32];
                        mRegisters[rd] |= imm32;
                        break;

                    case Op.XOR: // XOR
                        rd = mProgramMemory[mIp++];
                        rs = mProgramMemory[mIp++];
                        mRegisters[rd] ^= mRegisters[rs];
                        break;

                    case Op.XORI: // XORI
                        rd = mProgramMemory[mIp++];
                        imm32 = ((imm32 = mProgramMemory[mIp++]) > 127) ? mConstantPool[(((imm32 & 127) << 8) | mProgramMemory[mIp++])] : mConstantPool[imm32];
                        mRegisters[rd] ^= imm32;
                        break;

                    case Op.DIVU: // DIVU
                        rd = mProgramMemory[mIp++];
                        rs = mProgramMemory[mIp++];
                        mRegisters[rd] = (int)((uint)(mRegisters[rd]) / ((uint)mRegisters[rs]));
                        break;

                    case Op.DIVUI: // DIVUI
                        rd = mProgramMemory[mIp++];
                        imm32 = ((imm32 = mProgramMemory[mIp++]) > 127) ? mConstantPool[(((imm32 & 127) << 8) | mProgramMemory[mIp++])] : mConstantPool[imm32];
                        mRegisters[rd] = (int)((uint)(mRegisters[rd]) / ((uint)imm32));
                        break;

                    case Op.DIV: // DIV
                        rd = mProgramMemory[mIp++];
                        rs = mProgramMemory[mIp++];
                        mRegisters[rd] /= mRegisters[rs];
                        break;

                    case Op.DIVI: // DIVI
                        rd = mProgramMemory[mIp++];
                        imm32 = ((imm32 = mProgramMemory[mIp++]) > 127) ? mConstantPool[(((imm32 & 127) << 8) | mProgramMemory[mIp++])] : mConstantPool[imm32];
                        mRegisters[rd] /= imm32;
                        break;

                    case Op.SLL: // SLL
                        rd = mProgramMemory[mIp++];
                        rs = mProgramMemory[mIp++];
                        mRegisters[rd] <<= mRegisters[rs];
                        break;

                    case Op.SLLI: // SLLI
                        rd = mProgramMemory[mIp++];
                        imm32 = mProgramMemory[mIp++];
                        mRegisters[rd] <<= imm32;
                        break;

                    case Op.SRA: // SRA
                        rd = mProgramMemory[mIp++];
                        rs = mProgramMemory[mIp++];
                        mRegisters[rd] >>= mRegisters[rs];
                        break;

                    case Op.SRAI: // SRAI
                        rd = mProgramMemory[mIp++];
                        imm32 = mProgramMemory[mIp++];
                        mRegisters[rd] >>= imm32;
                        break;

                    case Op.SRL: // SRL
                        rd = mProgramMemory[mIp++];
                        rs = mProgramMemory[mIp++];
                        mRegisters[rd] = (int)((uint)mRegisters[rd] >> mRegisters[rs]);
                        break;

                    case Op.SRLI: // SRLI
                        rd = mProgramMemory[mIp++];
                        imm32 = mProgramMemory[mIp++];
                        mRegisters[rd] = (int)((uint)mRegisters[rd] >> imm32);
                        break;

                    case Op.NOT: // NOT
                        rd = mProgramMemory[mIp++];
                        rs = mProgramMemory[mIp++];
                        mRegisters[rd] = ~mRegisters[rs];
                        break;

                    case Op.NEG: // NEG
                        rd = mProgramMemory[mIp++];
                        rs = mProgramMemory[mIp++];
                        mRegisters[rd] = -mRegisters[rs];
                        break;

                    case Op.RET: // RET
                        mIp = mRegisters[Reg.RT];
                        break;

                    case Op.JC_EQ: // JC_EQ
                        rd = mProgramMemory[mIp++];
                        rs = mProgramMemory[mIp++];
                        if (mRegisters[rd] == mRegisters[rs])
                        {
                            imm32 = ((int)mProgramMemory[mIp++] << 8) | ((int)mProgramMemory[mIp++]);
                            mIp = imm32;
                        }
                        else
                        {
                            mIp += 2;
                        }
                        break;

                    case Op.JC_NE: // JC_NE
                        rd = mProgramMemory[mIp++];
                        rs = mProgramMemory[mIp++];
                        if (mRegisters[rd] != mRegisters[rs])
                        {
                            imm32 = ((int)mProgramMemory[mIp++] << 8) | ((int)mProgramMemory[mIp++]);
                            mIp = imm32;
                        }
                        else
                        {
                            mIp += 2;
                        }
                        break;

                    case Op.JC_GE: // JC_GE
                        rd = mProgramMemory[mIp++];
                        rs = mProgramMemory[mIp++];
                        if (mRegisters[rd] >= mRegisters[rs])
                        {
                            imm32 = ((int)mProgramMemory[mIp++] << 8) | ((int)mProgramMemory[mIp++]);
                            mIp = imm32;
                        }
                        else
                        {
                            mIp += 2;
                        }
                        break;

                    case Op.JC_GEU: // JC_GEU
                        rd = mProgramMemory[mIp++];
                        rs = mProgramMemory[mIp++];
                        if ((uint)mRegisters[rd] >= (uint)mRegisters[rs])
                        {
                            imm32 = ((int)mProgramMemory[mIp++] << 8) | ((int)mProgramMemory[mIp++]);
                            mIp = imm32;
                        }
                        else
                        {
                            mIp += 2;
                        }
                        break;

                    case Op.JC_GT: // JC_GT
                        rd = mProgramMemory[mIp++];
                        rs = mProgramMemory[mIp++];
                        if (mRegisters[rd] > mRegisters[rs])
                        {
                            imm32 = ((int)mProgramMemory[mIp++] << 8) | ((int)mProgramMemory[mIp++]);
                            mIp = imm32;
                        }
                        else
                        {
                            mIp += 2;
                        }
                        break;

                    case Op.JC_GTU: // JC_GTU
                        rd = mProgramMemory[mIp++];
                        rs = mProgramMemory[mIp++];
                        if ((uint)mRegisters[rd] > (uint)mRegisters[rs])
                        {
                            imm32 = ((int)mProgramMemory[mIp++] << 8) | ((int)mProgramMemory[mIp++]);
                            mIp = imm32;
                        }
                        else
                        {
                            mIp += 2;
                        }
                        break;

                    case Op.JC_LE: // JC_LE
                        rd = mProgramMemory[mIp++];
                        rs = mProgramMemory[mIp++];
                        if (mRegisters[rd] <= mRegisters[rs])
                        {
                            imm32 = ((int)mProgramMemory[mIp++] << 8) | ((int)mProgramMemory[mIp++]);
                            mIp = imm32;
                        }
                        else
                        {
                            mIp += 2;
                        }
                        break;

                    case Op.JC_LEU: // JC_LEU
                        rd = mProgramMemory[mIp++];
                        rs = mProgramMemory[mIp++];
                        if ((uint)mRegisters[rd] <= (uint)mRegisters[rs])
                        {
                            imm32 = ((int)mProgramMemory[mIp++] << 8) | ((int)mProgramMemory[mIp++]);
                            mIp = imm32;
                        }
                        else
                        {
                            mIp += 2;
                        }
                        break;

                    case Op.JC_LT: // JC_LT
                        rd = mProgramMemory[mIp++];
                        rs = mProgramMemory[mIp++];
                        if (mRegisters[rd] < mRegisters[rs])
                        {
                            imm32 = ((int)mProgramMemory[mIp++] << 8) | ((int)mProgramMemory[mIp++]);
                            mIp = imm32;
                        }
                        else
                        {
                            mIp += 2;
                        }
                        break;

                    case Op.JC_LTU: // JC_LTU
                        rd = mProgramMemory[mIp++];
                        rs = mProgramMemory[mIp++];
                        if ((uint)mRegisters[rd] < (uint)mRegisters[rs])
                        {
                            imm32 = ((int)mProgramMemory[mIp++] << 8) | ((int)mProgramMemory[mIp++]);
                            mIp = imm32;
                        }
                        else
                        {
                            mIp += 2;
                        }
                        break;

                    case Op.JPI: // JPI
                        imm32 = ((int)mProgramMemory[mIp++] << 8) | ((int)mProgramMemory[mIp++]);
                        mIp = imm32;
                        break;

                    case Op.JPR: // JPR
                        rd = mProgramMemory[mIp++];
                        mIp = mRegisters[rd];
                        break;

                    case Op.XB: // XB
                        rd = mProgramMemory[mIp++];
                        rs = mProgramMemory[mIp++];
                        mRegisters[rd] = (int)(((mRegisters[rs] & 0x80) == 0) ?
                            ((uint)mRegisters[rs] & 0xff) :
                            ((uint)mRegisters[rs] | 0xffffff00));
                        break;

                    case Op.XH: // XH
                        rd = mProgramMemory[mIp++];
                        rs = mProgramMemory[mIp++];
                        mRegisters[rd] = (int)(((mRegisters[rs] & 0x8000) == 0) ?
                            ((uint)mRegisters[rs] & 0xffff) :
                            ((uint)mRegisters[rs] | 0xffff0000));
                        break;

                    case Op.SYSCALL: // SYSCALL
                        imm32 = mProgramMemory[mIp++];
#if false//DEBUG
                        if(imm32 > 4)
                        Util.Log("Syscall "+imm32+": "+mRegisters[CoreInterpreted.Reg.I0]+" "+
                            mRegisters[CoreInterpreted.Reg.I1] + " " +
                            mRegisters[CoreInterpreted.Reg.I2] + " " +
                            mRegisters[CoreInterpreted.Reg.I3]);
#endif
                        mSyscallInvoker.InvokeSyscall(imm32);
                        break;

                    case Op.CASE: // CASE
                        rd = mProgramMemory[mIp++];
                        imm32 = ((int)mProgramMemory[mIp++] << 16) | ((int)mProgramMemory[mIp++] << 8) | ((int)mProgramMemory[mIp++]);
                        imm32 <<= 2;
                        uint caseStart = mDataMemory.ReadUInt32(imm32); //(uint)mDataMemory[imm32];
                        uint caseLength = mDataMemory.ReadUInt32(imm32 + 4); //(uint)mDataMemory[imm32 + 1];
                        uint index = ((uint)mRegisters[rd] - (uint)caseStart);
                        if (index <= caseLength)
                        {
                            int tableAddr = imm32 + 3 * 4; // 3*sizeof(int)
                            mIp = mDataMemory.ReadInt32(tableAddr + (int)index * 4); // mDataMemory[tableAddr + index];
                        }
                        else
                        {
                            int defaultCaseAddr = mDataMemory.ReadInt32(imm32 + 2 * 4); // mDataMemory[imm32 + 2]; // 2*sizeof(int)
                            mIp = defaultCaseAddr;
                        }
                        break;

                    case Op.FAR: // FAR
                        op = mProgramMemory[mIp++];
                        switch (op)
                        {
                            case Op.CALLI: // CALLI
                                imm32 = ((int)mProgramMemory[mIp++] << 16) | ((int)mProgramMemory[mIp++] << 8) | ((int)mProgramMemory[mIp++]);
                                mRegisters[Reg.RT] = mIp;
                                mIp = imm32;
                                break;

                            case Op.JC_EQ: // JC_EQ
                                rd = mProgramMemory[mIp++];
                                rs = mProgramMemory[mIp++];
                                if (mRegisters[rd] == mRegisters[rs])
                                {
                                    imm32 = ((int)mProgramMemory[mIp++] << 16) | ((int)mProgramMemory[mIp++] << 8) | ((int)mProgramMemory[mIp++]);
                                    mIp = imm32;
                                }
                                else
                                {
                                    mIp += 3;
                                }
                                break;


                            case Op.JC_NE: // JC_NE
                                rd = mProgramMemory[mIp++];
                                rs = mProgramMemory[mIp++];
                                if ((mRegisters[rd]) != (mRegisters[rs]))
                                {
                                    imm32 = ((int)mProgramMemory[mIp++] << 16) | ((int)mProgramMemory[mIp++] << 8) | ((int)mProgramMemory[mIp++]);
                                    mIp = imm32;
                                }
                                else
                                {
                                    mIp += 3;
                                }
                                break;

                            case Op.JC_GE: // JC_GE
                                rd = mProgramMemory[mIp++];
                                rs = mProgramMemory[mIp++];
                                if (mRegisters[rd] >= mRegisters[rs])
                                {
                                    imm32 = ((int)mProgramMemory[mIp++] << 16) | ((int)mProgramMemory[mIp++] << 8) | ((int)mProgramMemory[mIp++]);
                                    mIp = imm32;
                                }
                                else
                                {
                                    mIp += 3;
                                }
                                break;

                            case Op.JC_GEU: // JC_GEU
                                rd = mProgramMemory[mIp++];
                                rs = mProgramMemory[mIp++];
                                if (((uint)mRegisters[rd]) >= ((uint)mRegisters[rs]))
                                {
                                    imm32 = ((int)mProgramMemory[mIp++] << 16) | ((int)mProgramMemory[mIp++] << 8) | ((int)mProgramMemory[mIp++]);
                                    mIp = imm32;
                                }
                                else
                                {
                                    mIp += 3;
                                }
                                break;

                            case Op.JC_GT: // JC_GT
                                rd = mProgramMemory[mIp++];
                                rs = mProgramMemory[mIp++];
                                if (mRegisters[rd] > mRegisters[rs])
                                {
                                    imm32 = ((int)mProgramMemory[mIp++] << 16) | ((int)mProgramMemory[mIp++] << 8) | ((int)mProgramMemory[mIp++]);
                                    mIp = imm32;
                                }
                                else
                                {
                                    mIp += 3;
                                }
                                break;

                            case Op.JC_GTU: // JC_GTU
                                rd = mProgramMemory[mIp++];
                                rs = mProgramMemory[mIp++];
                                if (((uint)mRegisters[rd]) > ((uint)mRegisters[rs]))
                                {
                                    imm32 = ((int)mProgramMemory[mIp++] << 16) | ((int)mProgramMemory[mIp++] << 8) | ((int)mProgramMemory[mIp++]);
                                    mIp = imm32;
                                }
                                else
                                {
                                    mIp += 3;
                                }
                                break;

                            case Op.JC_LE: // JC_LE
                                rd = mProgramMemory[mIp++];
                                rs = mProgramMemory[mIp++];
                                if (mRegisters[rd] <= mRegisters[rs])
                                {
                                    imm32 = ((int)mProgramMemory[mIp++] << 16) | ((int)mProgramMemory[mIp++] << 8) | ((int)mProgramMemory[mIp++]);
                                    mIp = imm32;
                                }
                                else
                                {
                                    mIp += 3;
                                }
                                break;

                            case Op.JC_LEU: // JC_LEU
                                rd = mProgramMemory[mIp++];
                                rs = mProgramMemory[mIp++];
                                if (((uint)mRegisters[rd]) <= ((uint)mRegisters[rs]))
                                {
                                    imm32 = ((int)mProgramMemory[mIp++] << 16) | ((int)mProgramMemory[mIp++] << 8) | ((int)mProgramMemory[mIp++]);
                                    mIp = imm32;
                                }
                                else
                                {
                                    mIp += 3;
                                }
                                break;

                            case Op.JC_LT: // JC_LT
                                rd = mProgramMemory[mIp++];
                                rs = mProgramMemory[mIp++];
                                if (mRegisters[rd] < mRegisters[rs])
                                {
                                    imm32 = ((int)mProgramMemory[mIp++] << 16) | ((int)mProgramMemory[mIp++] << 8) | ((int)mProgramMemory[mIp++]);
                                    mIp = imm32;
                                }
                                else
                                {
                                    mIp += 3;
                                }
                                break;

                            case Op.JC_LTU: // JC_LTU
                                rd = mProgramMemory[mIp++];
                                rs = mProgramMemory[mIp++];
                                if (((uint)mRegisters[rd]) < ((uint)mRegisters[rs]))
                                {
                                    imm32 = ((int)mProgramMemory[mIp++] << 16) | ((int)mProgramMemory[mIp++] << 8) | ((int)mProgramMemory[mIp++]);
                                    mIp = imm32;
                                }
                                else
                                {
                                    mIp += 3;
                                }
                                break;

                            case Op.JPI: // JPI
                                imm32 = ((int)mProgramMemory[mIp++] << 16) | ((int)mProgramMemory[mIp++] << 8) | ((int)mProgramMemory[mIp++]);
                                mIp = imm32;
                                break;
                        }
                        break;

                }
            }
        }
    }
}
