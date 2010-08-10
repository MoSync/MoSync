

#include <config_platform.h>
#include <helpers/TranslateSyscall.h>
#include "../CoreCommon.h"
#include "../Core.h"
#include "../disassembler.h"



                      

struct ILOp
{
	byte m_op[2];
	byte m_rd;
	byte m_rs;
	int  m_imm32;
};

struct SyscallReplacementOp
{
	byte         m_syscID;
	const char * m_name;	
	byte         m_replacement[2];
};


static SyscallReplacementOp *g_syscallTB[256];
static SyscallReplacementOp g_syscallMap[] = 
{ 
	/* Double float ops */
	{ -1, "__adddf3"   , { _DADD, 0 } },
	{ -1, "__subdf3"   , { _DSUB, 0 } },
	{ -1, "__muldf3"   , { _DMUL, 0 } },
	{ -1, "__divdf3"   , { _DDIV, 0 } },
	{ -1, "__negdf2"   , { _DNEG, 0 } },
	{ -1, "d2f"        , { _D2F, 0 }  },
	{ -1, "__fixdfsi"  , { _D2I, 0 }  },
	{ -1, "__floatsidf", { _I2D, 0 }  },
	{ -1, "dcmp"       , { _DCMP, 0 } },
	{ -1, "__eqdf2"    , { _DCMP, 0 } },
	{ -1, "__nedf2"    , { _DCMP, 0 } },
	{ -1, "__gedf2"    , { _DCMP, 0 } },
	{ -1, "__gtdf2"    , { _DCMP, 0 } },
	{ -1, "__ledf2"    , { _DCMP, 0 } },
	{ -1, "__ltdf2"    , { _DCMP, 0 } },
	
	/* Single float ops */
	{ -1, "__addsf3"   , { _FADD, 0 } },
	{ -1, "__subsf3"   , { _FSUB, 0 } },
	{ -1, "__mulsf3"   , { _FMUL, 0 } },
	{ -1, "__divsf3"   , { _FDIV, 0 } },
	{ -1, "__negsf2"   , { _FNEG, 0 } },
	{ -1, "f2d"        , { _F2D, 0 }  },
	{ -1, "__fixsfsi"  , { _F2I, 0 }  },
	{ -1, "__floatsisf", { _I2F, 0 }  },
	{ -1, "__extendsfdf2", { _F2D, 0 }},
	{ -1, "__truncdfsf2", { _D2F, 0 } },	
	{ -1, "fcmp"       , { _FCMP, 0 } },
	{ -1, "__eqsf2"    , { _FCMP, 0 } },
	{ -1, "__nesf2"    , { _FCMP, 0 } },
	{ -1, "__gesf2"    , { _FCMP, 0 } },
	{ -1, "__gtsf2"    , { _FCMP, 0 } },
	{ -1, "__lesf2"    , { _FCMP, 0 } },
	{ -1, "__ltsf2"    , { _FCMP, 0 } },
	
	/* Trigonometric and other math functions*/
	{ -1, "sin"        , { _DSIN, 0 } },
	{ -1, "cos"        , { _DCOS, 0 } },
	{ -1, "tan"        , { _DTAN, 0 } },
	{ -1, "sqrt"       , { _DSQRT, 0 }},
	
	/* Long long (64 bit) ops */
	{ -1, "__muldi3"    , { _LMUL, 0 } },	
}; 




static void InitTables ( void )
{
	static bool done = false;
	
	if ( done == true )
		return;
	
	// Reset translation table	
	memset( g_syscallTB, 0, sizeof( g_syscallTB ) );	
	
	// Go through all the syscalls and find
	// their IDs
	int mapSize = sizeof( g_syscallMap )/sizeof( SyscallReplacementOp );
	for ( int i = 0; i < mapSize; i++ )
	{
		int id = translateSyscallID( g_syscallMap[i].m_name );
		if ( id < 0 )
			continue;
			
		DEBUG_ASSERT( id < 256 );		
		g_syscallMap[i].m_syscID = id;
		g_syscallTB[id] = g_syscallMap+i;
	}	
}


static void PatchSyscall ( byte *ip, const ILOp &ins )
{
	// Check if this is anything we can patch
	DEBUG_ASSERT( ins.m_imm32 < 256 );
	if ( g_syscallTB[ins.m_imm32] == NULL )
		return;
		 
	// Patch code
	ip[0] = g_syscallTB[ins.m_imm32]->m_replacement[0];
	ip[1] = g_syscallTB[ins.m_imm32]->m_replacement[1];
}

/**
 * Patches the code section
 *
 *
 */
void CodePatcher ( byte* mem_cs, 
                   const int* mem_cp,
                   int cs_start, 
                   int cs_size )
{
	ILOp op;
	
	// Initialize the patch tables
	InitTables( );

	// Search through code section
	for ( int ip = cs_start; ip < cs_size; )	
	{
		// Decode instruction
		int ins_size = disassemble_one( mem_cs+ip, mem_cs, mem_cp, 
		                                NULL, op.m_op[0], op.m_op[1], 
		                                op.m_rd, op.m_rs, op.m_imm32 );

		// Possible patch target
		switch ( op.m_op[0] )
		{
			case _SYSCALL: PatchSyscall( mem_cs+ip, op ); break;
		}		
		
		// Update instruction pointer 
		ip += ins_size;
	}
}
