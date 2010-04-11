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

#ifndef DISASSEMBLER_H
#define DISASSEMBLER_H

/**
 * Disassembles one MoSync IL instruction
 *
 * @param ip      [in] Pointer to the instruction
 * @param mem_cs  [in] Pointer to data section
 * @param mem_cp  [in] Pointer to constant section
 * @param buff    [in] Pointer to char buffer which can hold one
 *                     dissassembled instruction.
 *
 * @return Size of the dissassembled instruction in bytes
 */
int disassemble_one ( const byte* ip, 
                      const byte* mem_cs, 
                      const int* mem_cp, 
                      char* buf );

/**
 * Disassembles one MoSync IL instruction
 *
 * @param ip      [in] Pointer to the instruction
 * @param mem_cs  [in] Pointer to data section
 * @param mem_cp  [in] Pointer to constant section
 * @param buff    [in] Pointer to char buffer which can hold one
 *                     dissassembled instruction.
 * @param op      [out] Opcode
 * @param op2     [out] Second opcode, incase op was a prefix opcode
 * @param rd      [out] Destination register
 * @param rs      [out] Source register
 * @param imm32   [out] Immediate
 *
 * @return Size of the dissassembled instruction in bytes
 */
int disassemble_one ( const byte* ip, 
                      const byte* mem_cs, 
                      const int* mem_cp, 
                      char* buf, 
                      byte& op, 
                      byte& op2, 
                      byte &rd, 
                      byte &rs, 
                      int &imm32 );

#endif	//DISASSEMBLER_H
