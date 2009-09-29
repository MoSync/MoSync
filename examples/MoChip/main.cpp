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

/** \file main.cpp
*
* This file contains the main example program source.
*
* Shows how to create an advanced application called MoChip (chip 8 emulator).
*
* Copyright (c) Mobile Sorcery AB 2005-2007
*
* \author Niklas Nummelin
*
*/
#include <ma.h>
#include <mastdlib.h>
#include <mavsprintf.h>
#include "MAHeaders.h"

#define CHIP8_MEM_BYTES 4096
#define PROGRAM_MEMORY_START 0x200

typedef unsigned char byte;

byte memory[CHIP8_MEM_BYTES];
int regs[16];

int indexRegister;
int	programCounter;
int soundTimer;
int delayTimer;
int cycles;

#define STACK_SIZE 16
int stack[STACK_SIZE];
int stackPointer = 0;

#define DIRTY_FLAG (1<<31)
#define PIXEL_LIT (1<<0)

#define CHIP8_SCREEN_WIDTH 64
#define CHIP8_SCREEN_WIDTH_MASK CHIP8_SCREEN_WIDTH-1
#define CHIP8_SCREEN_HEIGHT 32
#define CHIP8_SCREEN_HEIGHT_MASK CHIP8_SCREEN_HEIGHT-1
#define CHIP8_SCREEN_SIZE CHIP8_SCREEN_WIDTH*CHIP8_SCREEN_HEIGHT

#define SCHIP_SCREEN_WIDTH 128
#define SCHIP_SCREEN_WIDTH_MASK SCHIP_SCREEN_WIDTH-1
#define SCHIP_SCREEN_HEIGHT 64
#define SCHIP_SCREEN_HEIGHT_MASK SCHIP_SCREEN_HEIGHT-1
#define SCHIP_SCREEN_SIZE SCHIP_SCREEN_WIDTH*SCHIP_SCREEN_HEIGHT

int currentScreenSize;
int currentScreenWidth;
int currentScreenWidthMask;
int currentScreenWidthShift;
int currentScreenHeight;
int currentScreenHeightMask;
int currentScreenHeightShift;
int *screen;

void (*instructionTable[16])(int);

#define CHIP8_FONT_SIZE 0x50
#define CHIP8_CHAR_SIZE 4*5
   byte chip8_font[CHIP8_FONT_SIZE]=
     {
         0xf9,0x99,0xf2,0x62,0x27,
         0xf1,0xf8,0xff,0x1f,0x1f,
         0x99,0xf1,0x1f,0x8f,0x1f,
         0xf8,0xf9,0xff,0x12,0x44,
         0xf9,0xf9,0xff,0x9f,0x1f,
         0xf9,0xf9,0x9e,0x9e,0x9e,
         0xf8,0x88,0xfe,0x99,0x9e,
         0xf8,0xf8,0xff,0x8f,0x88,
   }; 
    
#define SCHIP_FONT_SIZE 10*10
#define SCHIP_CHAR_SIZE 8*10
   byte schip_font[SCHIP_FONT_SIZE]=
     {
	 0x3C, 0x7E, 0xC3, 0xC3, 0xC3, 0xC3, 0xC3, 0xC3, 0x7E, 0x3C,
	 0x18, 0x38, 0x58, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x3C,
	 0x3E, 0x7F, 0xC3, 0x06, 0x0C, 0x18, 0x30, 0x60, 0xFF, 0xFF,
	 0x3C, 0x7E, 0xC3, 0x03, 0x0E, 0x0E, 0x03, 0xC3, 0x7E, 0x3C,
	 0x06, 0x0E, 0x1E, 0x36, 0x66, 0xC6, 0xFF, 0xFF, 0x06, 0x06, 
	 0xFF, 0xFF, 0xC0, 0xC0, 0xFC, 0xFE, 0x03, 0xC3, 0x7E, 0x3C,
	 0x3E, 0x7C, 0xC0, 0xC0, 0xFC, 0xFE, 0xC3, 0xC3, 0x7E, 0x3C,
	 0xFF, 0xFF, 0x03, 0x06, 0x0C, 0x18, 0x30, 0x60, 0x60, 0x60,
	 0x3C, 0x7E, 0xC3, 0xC3, 0x7E, 0x7E, 0xC3, 0xC3, 0x7E, 0x3C,
	 0x3C, 0x7E, 0xC3, 0xC3, 0x7F, 0x3F, 0x03, 0x03, 0x3E, 0x7C,
     }; 

class EventHandler {
private:

public:
	bool left_pressed, right_pressed, up_pressed, down_pressed, fire_pressed;
	bool left, right, up, down, fire;
	bool quit;

	bool keys[16];

	EventHandler() {
		left = false;
		right = false; 
		up = false;
		down = false;
		fire = false;
		quit = false;

		for(int i = 0; i < 16; i++) keys[i] = false;
	}

	void updateEvents() {
		left_pressed = right_pressed = up_pressed = down_pressed = fire_pressed = false;
		MAEvent event;
		/// update key states
		while((maGetEvent(&event))!=0) {
			if(event.type == EVENT_TYPE_CLOSE) {
				quit = true;
			} else if(event.type == EVENT_TYPE_KEY_PRESSED) {
				switch(event.key) {
				case MAK_0:
					keys[0] = true; 
					break;
				case MAK_1:
					keys[1] = true; 
					break;
				case MAK_2:
					keys[2] = true;
					break;
				case MAK_3:
					keys[3] = true;
					break;
				case MAK_4:
					keys[4] = true; 
					break;
				case MAK_5:
					keys[5] = true;
					break;
				case MAK_6:
					keys[6] = true; 
					break;
				case MAK_7:
					keys[7] = true;
					break;
				case MAK_8:
					keys[8] = true; 
					break;
				case MAK_9:
					keys[9] = true;
					break;
				case MAK_POUND:
					keys[10] = true; 
					break;
				case MAK_STAR:
					keys[11] = true;
					break;
				case MAK_FIRE:
					fire = true;
					fire_pressed = true;
					keys[12] = true;
					break;
				case MAK_LEFT:
					left = true;
					left_pressed = true;
					keys[13] = true;
					break;
				case MAK_RIGHT:
					right = true;
					right_pressed = true;
					keys[14] = true;
					break;
				case MAK_UP:
					up = true;
					up_pressed = true;
					keys[15] = true;
					break;
				}
			}
			else if(event.type == EVENT_TYPE_KEY_RELEASED) {
				switch(event.key) {
				case MAK_0:
					keys[0] = false; 
					break;
				case MAK_1:
					keys[1] = false; 
					break;
				case MAK_2:
					keys[2] = false;
					break;
				case MAK_3:
					keys[3] = false;
					break;
				case MAK_4:
					keys[4] = false; 
					break;
				case MAK_5:
					keys[5] = false;
					break;
				case MAK_6:
					keys[6] = false; 
					break;
				case MAK_7:
					keys[7] = false;
					break;
				case MAK_8:
					keys[8] = false; 
					break;
				case MAK_9:
					keys[9] = false;
					break;
				case MAK_POUND:
					keys[10] = false; 
					break;
				case MAK_STAR:
					keys[11] = false;
					break;
				case MAK_FIRE:
					fire = false;
					keys[12] = false;
					break;
				case MAK_LEFT:
					left = false;
					keys[13] = false;
					break;
				case MAK_RIGHT:
					right = false;
					keys[14] = false;
					break;
				case MAK_UP:
					up = false;
					keys[15] = false;
					break;

				}
			}
		}

	}
};


EventHandler event;

void drawShadowedText(int x, int y, int col, char *text) {
	maSetColor(0);
	maDrawText(x-1, y-1, text);
	maSetColor(col);
	maDrawText(x, y, text);
}

void drawOutLinedRect(int x1, int y1, int x2, int y2, int outLineCol, int bkgCol) {
	maSetColor(bkgCol);
	maFillRect(x1, y1, x2-x1, y2-y1);
	maSetColor(outLineCol);
	maLine(x1-1, y1-1, x2, y1-1);
	maLine(x1-1, y2, x2, y2);
	maLine(x1-1, y1-1, x1-1, y2);
	maLine(x2, y1-1, x2, y2);
}

//00xx extended instruction set

//00Cx  scdown x  	 Scroll the screen down x lines  	 Super only, not implemented
// do one for each version...
#define INST_EXT_SCDOWN_START 0xc0
#define INST_EXT_SCDOWN_END 0xcf

void inst_ext_scdown(int opcode) {
}

//00E0 	cls 	Clear the screen 	
#define INST_EXT_CLS 0xe0
void inst_ext_cls(int opcode) {
	memset(screen, 0, currentScreenSize*sizeof(int));
	for(int i = 0; i < currentScreenSize; i++) screen[i] |= DIRTY_FLAG;
}

//00EE 	rts 	return from subroutine call 	
#define INST_EXT_RTS 0xee
void inst_ext_rts(int opcode) {
	stackPointer--;
	programCounter = stack[stackPointer];
}

//00FB 	scright 	scroll screen 4 pixels right 	Super only,not implemented
#define INST_EXT_SCRIGHT 0xfb
void inst_ext_scright(int opcode) {
}

//00FC 	scleft 	scroll screen 4 pixels left 	Super only,not implemented
#define INST_EXT_SCLEFT 0xfc
void inst_ext_scleft(int opcode) {
}

//00FE 	low 	disable extended screen mode 	Super only
#define INST_EXT_LOW 0xfe
void inst_ext_low(int opcode) {
}

//00FF 	high 	enable extended screen mode (128 x 64) 	Super only 
#define INST_EXT_HIGH 0xff
void inst_ext_high(int opcode) {
}

void inst_ext_dummy(int opcode) {

}

void (*inst_ext_table[256])(int);
void init_inst_ext_table() {
	for(int i = 0; i < 256; i++) {
		inst_ext_table[i] = inst_ext_dummy;
	}	
	
	for(int i = INST_EXT_SCDOWN_START; i <= INST_EXT_SCDOWN_END; i++) {
		inst_ext_table[i] = inst_ext_scdown;
	}
	inst_ext_table[INST_EXT_CLS] = inst_ext_cls;
	inst_ext_table[INST_EXT_RTS] = inst_ext_rts;
	inst_ext_table[INST_EXT_SCRIGHT] = inst_ext_scright;
	inst_ext_table[INST_EXT_SCLEFT] = inst_ext_scleft;
	inst_ext_table[INST_EXT_LOW] = inst_ext_low;
	inst_ext_table[INST_EXT_HIGH] = inst_ext_high;
}

#define INST_EXT 0
void inst_ext(int opcode) {
	int ext_opcode = opcode&0x00ff;
	inst_ext_table[ext_opcode](opcode);
}

//1xxx  	 jmp xxx  	 jump to address xxx  
#define INST_JMP 1
void inst_jmp(int opcode) {
	programCounter = opcode&0xfff;
}

// 2xxx  	 jsr xxx  	 jump to subroutine at address xxx  	 16 levels maximum
#define INST_JSR 2
void inst_jsr(int opcode) {
	stack[stackPointer++] = programCounter;
	programCounter = opcode&0xfff;
}

//3rxx  	 skeq vr,xx  	 skip if register r = constant  	
#define INST_SKEQ_CONST 3 
void inst_skeq_const(int opcode) {
	if(regs[(opcode&0x0f00)>>8]==(opcode&0x00ff)) programCounter+=2;
}

//4rxx 	skne vr,xx 	skip if register r <> constant 	
#define INST_SKNE_CONST 4
void inst_skne_const(int opcode) {
	if(regs[(opcode&0x0f00)>>8]!=(opcode&0x00ff)) programCounter+=2;
}

//5ry0 	skeq vr,vy 	skip if register r = register y 	
#define INST_SKEQ_REG 5 
void inst_skeq_reg(int opcode) {
	if(regs[(opcode&0x0f00)>>8]==regs[(opcode&0x00f0)>>4]) programCounter+=2;
}

//6rxx 	mov vr,xx 	move constant to register r 	
#define INST_MOV_CONST 6 
void inst_mov_const(int opcode) {
	regs[(opcode&0x0f00)>>8] = opcode&0x00ff;
}

//7rxx 	add vr,vx 	add constant to register r 	No carry generated
#define INST_ADD_CONST 7
void inst_add_const(int opcode) {
	int vr = (opcode&0x0f00)>>8;
	regs[vr] += opcode&0x00ff;
	regs[vr] &= 0xff;
}

// extended register instruction set

// 8ry0 	mov vr,vy 	move register vy into vr 
#define INST_REG_MOV 0
void inst_reg_mov(int opcode) {
	regs[(opcode&0x0f00)>>8] = regs[(opcode&0x00f0)>>4];
}

// 8ry1 	or rx,ry 	or register vy into register vx 	
#define INST_REG_OR 1
void inst_reg_or(int opcode) {
	regs[(opcode&0x0f00)>>8] |= regs[(opcode&0x00f0)>>4];
}

// 8ry2 	and rx,ry 	and register vy into register vx 
#define INST_REG_AND 2
void inst_reg_and(int opcode) {
	regs[(opcode&0x0f00)>>8] &= regs[(opcode&0x00f0)>>4];
}

// 8ry3  	xor rx,ry	exclusive or register ry into register rx  	
#define INST_REG_XOR 3
void inst_reg_xor(int opcode) {
	regs[(opcode&0x0f00)>>8] ^= regs[(opcode&0x00f0)>>4];
}

// 8ry4 	add vr,vy 	add register vy to vr,carry in vf 	
#define INST_REG_ADD 4
void inst_reg_add(int opcode) {
	int vr = (opcode&0x0f00)>>8;
	regs[vr] += regs[(opcode&0x00f0)>>4];
	regs[0xf] = (regs[vr]&0xff00)?1:0;
	regs[vr] &= 0xff;
}

// 8ry5 	sub vr,vy 	subtract register vy from vr,borrow in vf 				vf set to 1 if borroesws
#define INST_REG_SUB 5
#define MSB_INT_SHIFT (sizeof(int)<<3) //(sizeof(int)<<3)
#define HAS_BORROW(x) ((int)(x<0))

void inst_reg_sub(int opcode) {
	int vr = (opcode&0x0f00)>>8;
	regs[vr] -= regs[(opcode&0x00f0)>>4];
	regs[0xf] = !HAS_BORROW(regs[vr]);
	regs[vr] &= 0xff;
}

// 8r06 	shr vr 		shift register vy right, bit 0 goes into register vf 	
#define INST_REG_SHR 6
void inst_reg_shr(int opcode) {
	int vr = (opcode&0x0f00)>>8;
	regs[0xf] = regs[vr]&0x1;
	regs[vr] = (((unsigned int)regs[vr])>>1);
}

// 8ry7 	rsb vr,vy 	subtract register vr from register vy, result in vr 	vf set to 1 if borrows
#define INST_REG_RSB 7
void inst_reg_rsb(int opcode) {
	int vr = (opcode&0x0f00)>>8;
	regs[vr] = regs[(opcode&0x00f0)>>4] - regs[vr];
	regs[0xf] = !HAS_BORROW(regs[vr]);
	regs[vr] &= 0xff;
}

// 8r0e 	shl vr 		shift register vr left,bit 7 goes into register vf
#define INST_REG_SHL 0xe
void inst_reg_shl(int opcode) {
	int vr = (opcode&0x0f00)>>8;
	regs[0xf] = (((regs[vr])>>7)&0x1);
	regs[vr] <<= 1;
	regs[vr] &= 0xff;
}

void inst_reg_dummy(int opcode) {

}

void (*inst_reg_table[16])(int);
void init_inst_reg_table() {
	for(int i = 0; i < 16; i++) {
		inst_reg_table[i] = inst_reg_dummy;
	}
	inst_reg_table[INST_REG_MOV] = inst_reg_mov;
	inst_reg_table[INST_REG_OR] = inst_reg_or;
	inst_reg_table[INST_REG_AND] = inst_reg_and;
	inst_reg_table[INST_REG_XOR] = inst_reg_xor;
	inst_reg_table[INST_REG_ADD] = inst_reg_add;
	inst_reg_table[INST_REG_SUB] = inst_reg_sub;
	inst_reg_table[INST_REG_SHR] = inst_reg_shr;
	inst_reg_table[INST_REG_RSB] = inst_reg_rsb;
	inst_reg_table[INST_REG_SHL] = inst_reg_shl;
}

#define INST_REG 8
void inst_reg(int opcode) {
	inst_reg_table[opcode&0x000f](opcode);
}

// 9ry0  	 skne rx,ry  	 skip if register rx <> register ry
#define INST_SKNE_REG 9
void inst_skne_reg(int opcode) {
	if(regs[(opcode&0x0f00)>>8]!=regs[(opcode&0x00f0)>>4]) programCounter+=2;
}

//axxx  	 mvi xxx  	 Load index register with constant xxx
#define INST_MVI 10
void inst_mvi(int opcode) {
	indexRegister = opcode&0x0fff;
}

// bxxx  	 jmi xxx  	 Jump to address xxx+register v0
#define INST_JMI 11
void inst_jmi(int opcode) {
	programCounter = (opcode&0xfff) + regs[0]; 
}

// crxx  	 rand vr,xxx     	 vr = random number less than or equal to xxx
#define INST_RAND 12
void inst_rand(int opcode) {
	regs[(opcode&0x0f00)>>8] = rand()%((opcode&0x00ff)+1);
}

// drys  	 sprite rx,ry,s  	 Draw sprite at screen location rx,ry height s  	 
// Sprites stored in memory at location in index register, maximum 8 bits wide. 
// Wraps around the screen. If when drawn, clears a pixel, 
// vf is set to 1 otherwise it is zero. All drawing is xor drawing 
// (e.g. it toggles the screen pixels)

// dry0  	 xsprite rx,ry  	 
// Draws extended sprite at screen location rx,ry  	 
// As above,but sprite is always 16 x 16. Superchip only, not yet implemented
#define INST_SPRITE 13
void inst_sprite(int opcode) {
	// sprite
	int x = regs[(opcode&0x0f00)>>8];
	int y = regs[(opcode&0x00f0)>>4];
	int height = opcode&0x000f;			
	if(height==0) height = 16;	
	byte *spritePtr = &memory[indexRegister];
	int sx;
	x&=currentScreenWidthMask;
	y&=currentScreenHeightMask;
	int y_ofs;
	regs[0xf] = 0;
	for(int i = 0; i < height; i++) {
		y_ofs = (y<<currentScreenWidthShift);
		sx = x;
		for(int j = 0; j < 8; j++) {
			int spritePixelLit = ((*spritePtr)>>(7-j))&0x1;
			if(spritePixelLit) {
				screen[sx+y_ofs]^=PIXEL_LIT;
				screen[sx+y_ofs]|=DIRTY_FLAG;
			}
			
			if(!(screen[sx+y_ofs]&=PIXEL_LIT)) {
				regs[0xf] |= 1;
			}
			
			sx=(sx+1)&currentScreenWidthMask;
		}
		spritePtr++;
		y=(y+1)&currentScreenHeightMask;
	}
}

//ek9e  skpr k  skip if key (register rk) pressed  	 The key is a key number, see the chip-8 documentation
//eka1 	skup k 	skip if key (register rk) not pressed 	
#define INST_SKIP_KEY 14
void inst_skip_key(int opcode) {
	switch(opcode&0x000f)
	{
	case 0xe: if(event.keys[(opcode&0x0f00)>>8]) programCounter+=2; break;
	case 0x1: if(!event.keys[(opcode&0x0f00)>>8]) programCounter+=2; break;
	}
}

// extented instruction set 2

// fr07  	gdelay vr  	get delay timer into vr  	
#define INST_EXT2_GDELAY 0x07
void inst_ext2_gdelay(int opcode) {
	regs[(opcode&0x0f00)>>8] = delayTimer;
}

// fr0a 	key vr 		wait for for keypress,put key in register vr 	
#define INST_EXT2_KEY 0x0a
void inst_ext2_key(int opcode) {
	int i;
	while(1) {
		for(i = 0; i < 16; i++) {
			if(event.keys[i]) {
				break;
			}
		}
		if(i!=16) break;

		maWait(0);
		event.updateEvents();

		if(event.quit) maExit(0);
	}

	regs[(opcode&0x0f00)>>8] = i;
}

// fr15 	sdelay vr 	set the delay timer to vr 	
#define INST_EXT2_SDELAY 0x17
void inst_ext2_sdelay(int opcode) {
	delayTimer = regs[(opcode&0x0f00)>>8];
}

// fr18 	ssound vr 	set the sound timer to vr 	
#define INST_EXT2_SSOUND 0x18
void inst_ext2_ssound(int opcode) {
	soundTimer = regs[(opcode&0x0f00)>>8];
}

// fr1e 	adi vr 		add register vr to the index register 	
#define INST_EXT2_ADI 0x1e
void inst_ext2_adi(int opcode) {
	indexRegister += regs[(opcode&0x0f00)>>8];
}

// fr29 	font vr 	point I to the sprite for hexadecimal character in vr 	Sprite is 5 bytes high
#define INST_EXT2_FONT 0x29
void inst_ext2_font(int opcode) {
	indexRegister = 5*(regs[(opcode&0x0f00)>>8]);
}

// fr30 	xfont vr 	point I to the sprite for hexadecimal character in vr 	Sprite is 10 bytes high,Super only
#define INST_EXT2_XFONT 0x30
void inst_ext2_xfont(int opcode) {
	indexRegister = CHIP8_FONT_SIZE + (10)*(regs[(opcode&0x0f00)>>8]);
	indexRegister &= 0x0fff;
}

// fr33 	bcd vr 		store the bcd representation of register vr at location I,I+1,I+2 	Doesn't change I
#define INST_EXT2_BCD 0x33
void inst_ext2_bcd(int opcode) {
	int vr = regs[(opcode&0x0f00)>>8];
	memory[indexRegister] = (byte) (vr%10);
	vr/=10;
	memory[indexRegister+1] = (byte) (vr%10);
	vr/=10;
	memory[indexRegister+2] = (byte) (vr%10);
}

// fr55 	str v0-vr 	store registers v0-vr at location I onwards 	
// I is incremented to point to the next location on. e.g. I = I + r + 1
#define INST_EXT2_STR 0x55
void inst_ext2_str(int opcode) {
	int vr = (opcode&0x0f00)>>8;
	byte *dst = &memory[indexRegister];
	int *src = regs;
	for(int i = 0; i < vr; i++) {
		*dst++ = *src++;
	}
	indexRegister+=vr+1;
}

// fx65 	ldr v0-vr 	load registers v0-vr from location I onwards 	as above.
#define INST_EXT2_LDR 0x65
void inst_ext2_ldr(int opcode) {
	int vr = (opcode&0x0f00)>>8;
	int *dst = regs;
	byte *src = &memory[indexRegister];
	for(int i = 0; i < vr; i++) {
		*dst++ = *src++;
	}
	indexRegister+=vr+1;
}

void inst_ext2_dummy(int opcode) {
	
}

void (*inst_ext2_table[256])(int);
void init_inst_ext2_table() {
	for(int i = 0; i < 256; i++) {
		inst_ext2_table[i] = inst_ext2_dummy;
	}

	inst_ext2_table[INST_EXT2_GDELAY] = inst_ext2_gdelay;
	inst_ext2_table[INST_EXT2_KEY] = inst_ext2_key;
	inst_ext2_table[INST_EXT2_SDELAY] = inst_ext2_sdelay;
	inst_ext2_table[INST_EXT2_SSOUND] = inst_ext2_ssound;
	inst_ext2_table[INST_EXT2_ADI] = inst_ext2_adi;
	inst_ext2_table[INST_EXT2_FONT] = inst_ext2_font;
	inst_ext2_table[INST_EXT2_XFONT] = inst_ext2_xfont;
	inst_ext2_table[INST_EXT2_BCD] = inst_ext2_bcd;
	inst_ext2_table[INST_EXT2_STR] = inst_ext2_str;
	inst_ext2_table[INST_EXT2_LDR] = inst_ext2_ldr;
}

#define INST_EXT2 15
void inst_ext2(int opcode) {
	inst_ext2_table[opcode&0x00ff](opcode);
}

int calcShift(int a) {
	int s = 0;
	while(!(a&0x1)) {
		s++;
		a>>=1;
	}
	return s;
}

#define MODE_CHIP8 0
#define MODE_SCHIP 1
void initEmulator(int mode) {

	memcpy(memory, chip8_font, CHIP8_FONT_SIZE);

	if(mode == MODE_SCHIP) {
		memcpy(memory, schip_font+CHIP8_FONT_SIZE, SCHIP_FONT_SIZE);
		screen = new int[SCHIP_SCREEN_SIZE];
		currentScreenSize = SCHIP_SCREEN_SIZE;
		currentScreenWidth = SCHIP_SCREEN_WIDTH;
		currentScreenHeight = SCHIP_SCREEN_HEIGHT;
		currentScreenWidthMask = SCHIP_SCREEN_WIDTH_MASK;
		currentScreenHeightMask = SCHIP_SCREEN_HEIGHT_MASK;
	}
	else {
		screen = new int[CHIP8_SCREEN_SIZE];
		currentScreenSize = CHIP8_SCREEN_SIZE;
		currentScreenWidth = CHIP8_SCREEN_WIDTH;
		currentScreenHeight = CHIP8_SCREEN_HEIGHT;
		currentScreenWidthMask = CHIP8_SCREEN_WIDTH_MASK;
		currentScreenHeightMask = CHIP8_SCREEN_HEIGHT_MASK;
	}
	currentScreenWidthShift = calcShift(currentScreenWidth);
	currentScreenHeightShift = calcShift(currentScreenHeight);

	memset(regs, 0, 16*sizeof(int));
	memset(screen, 0, currentScreenSize*sizeof(int));

	programCounter = PROGRAM_MEMORY_START;
	stackPointer = 0;
	cycles = 0;
	indexRegister = delayTimer = soundTimer = 0;

	instructionTable[INST_EXT] = inst_ext;
	instructionTable[INST_JMP] = inst_jmp;
	instructionTable[INST_JSR] = inst_jsr;
	instructionTable[INST_SKEQ_CONST] = inst_skeq_const;
	instructionTable[INST_SKNE_CONST] = inst_skne_const;
	instructionTable[INST_SKEQ_REG] = inst_skeq_reg;
	instructionTable[INST_MOV_CONST] = inst_mov_const;
	instructionTable[INST_ADD_CONST] = inst_add_const;
	instructionTable[INST_REG] = inst_reg;
	instructionTable[INST_SKNE_REG] = inst_skne_reg;
	instructionTable[INST_MVI] = inst_mvi;
	instructionTable[INST_JMI] = inst_jmi;
	instructionTable[INST_RAND] = inst_rand;
	instructionTable[INST_SPRITE] = inst_sprite;
	instructionTable[INST_SKIP_KEY] = inst_skip_key;
	instructionTable[INST_EXT2] = inst_ext2;

	init_inst_ext_table();
	init_inst_reg_table();
	init_inst_ext2_table();
}

void closeEmulator() {
	delete screen;
}

void loadProgram() {
	int programSize = maGetDataSize(MOCHIP_GAME);
	if(PROGRAM_MEMORY_START + programSize > CHIP8_MEM_BYTES) {
		// something is really wrong
		return;
	}
	maReadData(MOCHIP_GAME, memory + PROGRAM_MEMORY_START, 0, programSize);
}

void drawMonitor();

int currentOpcode;
void executeOpcode() {
	// fetch opcode
	currentOpcode = (memory[programCounter]<<8)+(memory[programCounter+1]);
	
	//drawMonitor();

	// increment program counter
	programCounter += 2;

	// execute instruction
	instructionTable[currentOpcode>>12](currentOpcode);

	cycles++;

	if(delayTimer>0) delayTimer--;
	// if(soundTimer) if(--soundTimer==0) // turn off sound;
}

void drawMonitor() {
	MAExtent e = maGetScrSize();
	int w = EXTENT_X(e);
	int h = EXTENT_Y(e);

	maSetColor(0);
	maFillRect(0, 0, w, h);

	MAExtent fontSize = maGetTextSize("A");
	int sy = 2;
	char tempString[255];
	maSetColor(0xff0000);
	for(int i = 0; i < 16; i++){
		sprintf(tempString, "r%d: %d", i+1, regs[i]);
		maDrawText(w>>1, sy, tempString);
		sy+=EXTENT_Y(fontSize);
	}

	sy = 2;
	sprintf(tempString, "pc: %d", programCounter);
	maDrawText(2, sy, tempString);
	sy+=EXTENT_Y(fontSize);
	sprintf(tempString, "opcode: %x", currentOpcode);
	maDrawText(2, sy, tempString);
	sy+=EXTENT_Y(fontSize);
	sprintf(tempString, "sp: %d", stackPointer);
	maDrawText(2, sy, tempString);
	sy+=EXTENT_Y(fontSize);
	sprintf(tempString, "cycles: %d", cycles);
	maDrawText(2, sy, tempString);
	sy+=EXTENT_Y(fontSize);
	sprintf(tempString, "indexReg: %d", indexRegister);
	maDrawText(2, sy, tempString);
	sy+=EXTENT_Y(fontSize);
	sprintf(tempString, "soundTimer: %d", soundTimer);
	maDrawText(2, sy, tempString);
	sy+=EXTENT_Y(fontSize);
	sprintf(tempString, "delayTimer: %d", delayTimer);
	maDrawText(2, sy, tempString);
	sy+=EXTENT_Y(fontSize);

	while(1) {
		maWait(0);
		event.updateEvents();
		if(event.fire_pressed) break;
		maUpdateScreen();
	}
}

void updateScreen() {
	MAExtent e = maGetScrSize();
	int w = EXTENT_X(e);
	int h = EXTENT_Y(e);

	int xsize = 4;
	int ysize = 4;
	int sw = currentScreenWidth*xsize;
	int sh = currentScreenHeight*ysize;

	int index = 0;
	int startX = (w>>1) - (sw>>1);
	int startY = (h>>1) - (sh>>1);
	int x = startX;
	int y = startY;

	for(int j = 0; j < currentScreenHeight; j++) {
		x = startX;
		for(int i = 0; i < currentScreenWidth; i++) {
			if(screen[index]&DIRTY_FLAG) 
			{
				if(screen[index]&PIXEL_LIT) maSetColor(0xffffff);
				else maSetColor(0x000000);
				maFillRect(x, y, xsize, ysize);
				screen[index]&=(~DIRTY_FLAG);
			}
			x+=xsize;
			index++;
		}
		y+=ysize;
	}
	/*
	maSetColor(0xff0000);
	maLine(startX, startY, startX+sw, startY);
	maLine(startX, startY+sh, startX+sw,  startY+sh);
	maLine(startX, startY, startX, startY+sh);
	maLine(startX+sw, startY, startX+sw, startY+sh);
	*/

	/// keep the backlight alive
	//maResetBacklight();
}

extern "C" {
	/**
	* \brief The entry point.
	*/

	int MAMain()
	{
		MAExtent e = maGetScrSize();
		int w = EXTENT_X(e);
		int h = EXTENT_Y(e);

		loadProgram();
		initEmulator(MODE_CHIP8);

		int timeToNextUpdate = maGetMilliSecondCount()+(50);

		/// wait for key press or close event
		while(1) {
			executeOpcode();
			if(maGetMilliSecondCount()>timeToNextUpdate) {
				timeToNextUpdate = maGetMilliSecondCount()+(50);
				updateScreen();
				maUpdateScreen();
				event.updateEvents();
				if(event.quit) {
					break;
				}	
			}
		}

		return 0;
	}
}