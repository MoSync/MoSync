//********************************************************************************
//								MoSync asm routines
//********************************************************************************

	.sourcefile 'crtlib.s'

	.line 0

//****************************************
//				Globals
//****************************************

	.data
	.globl	__memtop
	.align 4
__memtop:
	.word	0
	
	.globl	__stacktop
__stacktop:
	.word	0

//****************************************
//			Start up code
//****************************************

	.code
	.align 4
	.global crt0_startup

.func crt0_startup, 3, void
	.line 1

	ld	[&__memtop],i0		// Save top of memory
	
	sub i0, #16			// move stack down memory 16 bytes
	ld	[&__stacktop],i0	// Save top of memory

	ld  sp,i0			// set stack top
	sub i0,i1			// make i0 into heap_bottom
	sub i0,i2			// i0 is now start of heap
	ld  i1,i2			// make i1 into heap_size

.ifdefglobal _override_heap_init_crt0
{		
	call &_override_heap_init_crt0
}
.ifndefglobal _override_heap_init_crt0
{
	call &_ansi_heap_init_crt0	// Heap init
}

	ld	i0,&__global_ctor_chain 		//constructor chain
	call &_crt_tor_chain

// Deal with different MAMain entry symbols

.ifdefglobal _MAMain
{		
	call &_MAMain
	.set mainok=1
}

.ifdefglobal _Z6MAMainv
{
	call &_Z6MAMainv
	.set mainok=1
}

.ifdefglobal _Z6MAMainiPPc
{
	call &_Z6MAMainiPPc
	.set mainok=1
}

.ifndef mainok
{
	.print "Unresolved symbol: MAMain"
	.exit
}

	ld	[sp,0], r14		// save return value

	ld	i0,&__global_dtor_chain			// destructor chain
	call &_crt_tor_chain

	ld	i0, [sp,0]
	call &_maExit
	ret

//****************************************
//			Start up code
//****************************************
/*
	.code
	.align 4
	.global crt0_startup_basic

.func crt0_startup_basic
	.line 2

	ld	[&__memtop],i0		// Save top of memory
	
	sub i0, #16				// move stack down memory 16 bytes
	ld	[&__stacktop],i0	// Save top of memory

	ld  sp,i0			// set stack top

	call &MAMain
	ld	[sp,0], r14		// save return value

	ld	i0, [sp,0]
	call &maExit
	ret
*/
/*
//****************************************
//			  setjmp
//****************************************

//.if	(__java_static == 0)
//{
	.globl setjmp

.func setjmp
	.line 3
	ld	[i0,0],sp
	ld	[i0,4],rt
	ld	[i0,8],fr
	ld	[i0,12],d0
	ld	[i0,16],d1
	ld	[i0,20],d2
	ld	[i0,24],d3
	ld	[i0,28],d4
	ld	[i0,32],d5
	ld	[i0,36],d6
	ld	[i0,40],d7
	xor	zr,zr
	ld	r14, zr
	ret

//}

//****************************************
//			longjmp
//****************************************

//.if	(__java_static == 0)
//{
	.globl longjmp

.func longjmp
	.line 4
	ld	sp,[i0,0]
	ld	rt,[i0,4]
	ld	fr,[i0,8]
	ld	d0,[i0,12]
	ld	d1,[i0,16]
	ld	d2,[i0,20]
	ld	d3,[i0,24]
	ld	d4,[i0,28]
	ld	d5,[i0,32]
	ld	d6,[i0,36]
	ld	d7,[i0,40]
	ld	r14,i1
	ret
	call &maExit

//}
*/
