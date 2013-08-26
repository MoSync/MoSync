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
