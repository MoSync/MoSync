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

//*********************************************************************************************
//				       			MoSync Code Tools API
//*********************************************************************************************

#ifdef __cplusplus
extern "C" {
#endif

//****************************************
// 			Opcode Constants
//****************************************

#define fetch_d 1		// rd
#define fetch_a	2		// address
#define fetch_s 4		// rs
#define fetch_i 8		// const imm
#define fetch_j 16		// imm 8
#define fetch_k 32		// syscall imm
#define fetch_f	64		// far address
#define fetch_c	128		// Case

#define fetch_ds	(fetch_d | fetch_s)
#define fetch_dsa	(fetch_d | fetch_s | fetch_a)
#define fetch_dsf	(fetch_d | fetch_s | fetch_f)
#define fetch_dsi	(fetch_d | fetch_s | fetch_i)
#define fetch_di	(fetch_d | fetch_i)
#define fetch_dj	(fetch_d | fetch_j)
#define fetch_dc	(fetch_d | fetch_c)

//****************************************
//			Extra Types
//****************************************

typedef unsigned char	uchar;
typedef unsigned short	ushort;
typedef unsigned int	uint;

//****************************************
//			Opcode Info
//****************************************

typedef struct
{
	const char *str;
	int flags;
	int farflag;
	int op;
	int rd;
	int rs;
	int imm;
	int rip;
	int len;
} DisasInfo;

void DisasGenReg(void);
void DisasInit(char *codemem, int *constpool);
const uchar * DisasDecode(DisasInfo *thisOpcode, const uchar *code_ip);
int DisasDecodeIP(DisasInfo *thisOpcode, int code_ip);
void DisasEmit(char *out, const char *Template, ...);
char * DisasRegName(int reg, int use_zero);
void DisasString(DisasInfo *thisOpcode, char *out);
int DisasInst(int ip, char *out);

#ifdef __cplusplus
}
#endif
