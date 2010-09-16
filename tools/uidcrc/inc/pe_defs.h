// PE_DEFS.H
//
// Copyright (c) 1995-1999 Symbian Ltd.  All rights reserved.
//

#if !defined(__PE_DEFS_H__)
#define __PE_DEFS_H__

// A few additions to winnnt.h

#define IMAGE_FILE_MINIMAL_OBJECT			0x0010
#define IMAGE_FILE_UPDATE_OBJECT			0x0020
#define IMAGE_FILE_16BIT_MACHINE			0x0040
#define IMAGE_FILE_PATCH					0x0400
#define IMAGE_FILE_MACHINE_I860				0x014d

#define IMAGE_LIBRARY_PROCESS_INIT			0x0001
#define IMAGE_LIBRARY_PROCESS_TERM			0x0002
#define IMAGE_LIBRARY_THREAD_INIT			0x0004
#define IMAGE_LIBRARY_THREAD_TERM			0x0008

#define IMAGE_LOADER_FLAGS_BREAK_ON_LOAD	0x0001
#define IMAGE_LOADER_FLAGS_DEBUG_ON_LOAD	0x0002 

#define IMAGE_SCN_LNK_OVERLAY				0x00000400 


//
// Image Format	(copied entire from winnt.h)
//

#define IMAGE_DOS_SIGNATURE                 0x5A4D      // MZ
#define IMAGE_OS2_SIGNATURE                 0x454E      // NE
#define IMAGE_OS2_SIGNATURE_LE              0x454C      // LE
#define IMAGE_NT_SIGNATURE                  0x00004550  // PE00

typedef struct _IMAGE_DOS_HEADER {      // DOS .EXE header
    TUint16   e_magic;                     // Magic number
    TUint16   e_cblp;                      // TUint8 on last page of file
    TUint16   e_cp;                        // Pages in file
    TUint16   e_crlc;                      // Relocations
    TUint16   e_cparhdr;                   // Size of header in paragraphs
    TUint16   e_minalloc;                  // Minimum extra paragraphs needed
    TUint16   e_maxalloc;                  // Maximum extra paragraphs needed
    TUint16   e_ss;                        // Initial (relative) SS value
    TUint16   e_sp;                        // Initial SP value
    TUint16   e_csum;                      // Checksum
    TUint16   e_ip;                        // Initial IP value
    TUint16   e_cs;                        // Initial (relative) CS value
    TUint16   e_lfarlc;                    // File address of relocation table
    TUint16   e_ovno;                      // Overlay number
    TUint16   e_res[4];                    // Reserved TUint16s
    TUint16   e_oemid;                     // OEM identifier (for e_oeminfo)
    TUint16   e_oeminfo;                   // OEM information; e_oemid specific
    TUint16   e_res2[10];                  // Reserved TUint16s
    TUint32   e_lfanew;                    // File address of new exe header
  } IMAGE_DOS_HEADER, *PIMAGE_DOS_HEADER;

typedef struct _IMAGE_OS2_HEADER {			// OS/2 .EXE header
    TUint16		ne_magic;                    // Magic number
    TUint8		ne_ver;                      // Version number
    TUint8		ne_rev;                      // Revision number
    TUint16		ne_enttab;                   // Offset of Entry Table
    TUint16		ne_cbenttab;                 // Number of TUint8 in Entry Table
    TUint32		ne_crc;                      // Checksum of whole file
    TUint16		ne_flags;                    // Flag TUint16
    TUint16		ne_autodata;                 // Automatic data segment number
    TUint16		ne_heap;                     // Initial heap allocation
    TUint16		ne_stack;                    // Initial stack allocation
    TUint32   ne_csip;                     // Initial CS:IP setting
    TUint32   ne_sssp;                     // Initial SS:SP setting
    TUint16   ne_cseg;                     // Count of file segments
    TUint16   ne_cmod;                     // Entries in Module Reference Table
    TUint16   ne_cbnrestab;                // Size of non-resident name table
    TUint16   ne_segtab;                   // Offset of Segment Table
    TUint16   ne_rsrctab;                  // Offset of Resource Table
    TUint16   ne_restab;                   // Offset of resident name table
    TUint16   ne_modtab;                   // Offset of Module Reference Table
    TUint16   ne_imptab;                   // Offset of Imported Names Table
    TUint32   ne_nrestab;                  // Offset of Non-resident Names Table
    TUint16   ne_cmovent;                  // Count of movable entries
    TUint16   ne_align;                    // Segment alignment shift count
    TUint16   ne_cres;                     // Count of resource segments
    TUint8   ne_exetyp;                   // Target Operating system
    TUint8   ne_flagsothers;              // Other .EXE flags
    TUint16   ne_pretthunks;               // offset to return thunks
    TUint16   ne_psegrefTUint8;             // offset to segment ref. TUint8
    TUint16   ne_swaparea;                 // Minimum code swap area size
    TUint16   ne_expver;                   // Expected Windows version number
  } IMAGE_OS2_HEADER, *PIMAGE_OS2_HEADER;

//
// File header format.
//

typedef struct _IMAGE_FILE_HEADER
	{
	TUint16	Machine;
    TUint16	NumberOfSections;
    TUint32	TimeDateStamp;
    TUint32	PointerToSymbolTable;
    TUint32	NumberOfSymbols;
    TUint16	SizeOfOptionalHeader;
    TUint16	Characteristics;
	} IMAGE_FILE_HEADER, *PIMAGE_FILE_HEADER;

#define IMAGE_SIZEOF_FILE_HEADER             20

#define IMAGE_FILE_RELOCS_STRIPPED			0x0001  // Relocation info stripped from file.
#define IMAGE_FILE_EXECUTABLE_IMAGE			0x0002  // File is executable  (i.e. no unresolved externel references).
#define IMAGE_FILE_LINE_NUMS_STRIPPED		0x0004  // Line nunbers stripped from file.
#define IMAGE_FILE_LOCAL_SYMS_STRIPPED		0x0008  // Local symbols stripped from file.
#define IMAGE_FILE_CHAR_REVERSED_LO			0x0080  // TUint8 of machine TUint16 are reversed.
#define IMAGE_FILE_32BIT_MACHINE			0x0100  // 32 bit word machine.
#define IMAGE_FILE_DEBUG_STRIPPED			0x0200  // Debugging info stripped from file in .DBG file
#define IMAGE_FILE_SYSTEM					0x1000  // System File.
#define IMAGE_FILE_DLL						0x2000  // File is a DLL.
#define IMAGE_FILE_CHAR_REVERSED_HI			0x8000  // Char of machine short are reversed.

#define IMAGE_FILE_MACHINE_UNKNOWN           0
#define IMAGE_FILE_MACHINE_I386              0x14c   // Intel 386.
#define IMAGE_FILE_MACHINE_R3000             0x162   // MIPS little-endian, 0540 big-endian
#define IMAGE_FILE_MACHINE_R4000             0x166   // MIPS little-endian
#define IMAGE_FILE_MACHINE_ALPHA             0x184   // Alpha_AXP
#define IMAGE_FILE_MACHINE_POWERPC           0x1F0   // IBM PowerPC Little-Endian

//
// Directory format.
//
typedef struct _IMAGE_DATA_DIRECTORY
	{
    TUint32	VirtualAddress;
    TUint32	Size;
	} IMAGE_DATA_DIRECTORY, *PIMAGE_DATA_DIRECTORY;

#define IMAGE_NUMBEROF_DIRECTORY_ENTRIES    16

//
// Optional header format.
//
typedef struct _IMAGE_OPTIONAL_HEADER
	{
    //
    // Standard fields.
    //

	TUint16	Magic;
    TUint8	MajorLinkerVersion;
    TUint8	MinorLinkerVersion;
    TUint32	SizeOfCode;
    TUint32	SizeOfInitializedData;
    TUint32	SizeOfUninitializedData;
    TUint32	AddressOfEntryPoint;
    TUint32	BaseOfCode;
    TUint32	BaseOfData;

    //
    // NT additional fields.
    //

    TUint32	ImageBase;
    TUint32	SectionAlignment;
    TUint32	FileAlignment;
    TUint16	MajorOperatingSystemVersion;
    TUint16	MinorOperatingSystemVersion;
    TUint16	MajorImageVersion;
    TUint16	MinorImageVersion;
    TUint16	MajorSubsystemVersion;
    TUint16	MinorSubsystemVersion;
    TUint32	Reserved1;
    TUint32	SizeOfImage;
    TUint32	SizeOfHeaders;
    TUint32	CheckSum;
    TUint16	Subsystem;
    TUint16	DllCharacteristics;
    TUint32	SizeOfStackReserve;
    TUint32	SizeOfStackCommit;
    TUint32	SizeOfHeapReserve;
    TUint32	SizeOfHeapCommit;
    TUint32	LoaderFlags;
    TUint32	NumberOfRvaAndSizes;
    IMAGE_DATA_DIRECTORY DataDirectory[IMAGE_NUMBEROF_DIRECTORY_ENTRIES];
	} IMAGE_OPTIONAL_HEADER, *PIMAGE_OPTIONAL_HEADER;

typedef struct _IMAGE_ROM_OPTIONAL_HEADER {
    TUint16   Magic;
    TUint8   MajorLinkerVersion;
    TUint8   MinorLinkerVersion;
    TUint32  SizeOfCode;
    TUint32  SizeOfInitializedData;
    TUint32  SizeOfUninitializedData;
    TUint32  AddressOfEntryPoint;
    TUint32  BaseOfCode;
    TUint32  BaseOfData;
    TUint32  BaseOfBss;
    TUint32  GprMask;
    TUint32  CprMask[4];
    TUint32  GpValue;
} IMAGE_ROM_OPTIONAL_HEADER, *PIMAGE_ROM_OPTIONAL_HEADER;

#define IMAGE_SIZEOF_ROM_OPTIONAL_HEADER      56
#define IMAGE_SIZEOF_STD_OPTIONAL_HEADER      28
#define IMAGE_SIZEOF_NT_OPTIONAL_HEADER      224

#define IMAGE_NT_OPTIONAL_HDR_MAGIC        0x10b
#define IMAGE_ROM_OPTIONAL_HDR_MAGIC       0x107

typedef struct _IMAGE_NT_HEADERS {
    TUint32 Signature;
    IMAGE_FILE_HEADER FileHeader;
    IMAGE_OPTIONAL_HEADER OptionalHeader;
} IMAGE_NT_HEADERS, *PIMAGE_NT_HEADERS;

typedef struct _IMAGE_ROM_HEADERS {
    IMAGE_FILE_HEADER FileHeader;
    IMAGE_ROM_OPTIONAL_HEADER OptionalHeader;
} IMAGE_ROM_HEADERS, *PIMAGE_ROM_HEADERS;

#define IMAGE_FIRST_SECTION( ntheader ) ((PIMAGE_SECTION_HEADER)        \
    ((TUint32)ntheader +                                                  \
     FIELD_OFFSET( IMAGE_NT_HEADERS, OptionalHeader ) +                 \
     ((PIMAGE_NT_HEADERS)(ntheader))->FileHeader.SizeOfOptionalHeader   \
    ))


// Subsystem Values

#define IMAGE_SUBSYSTEM_UNKNOWN              0   // Unknown subsystem.
#define IMAGE_SUBSYSTEM_NATIVE               1   // Image doesn't require a subsystem.
#define IMAGE_SUBSYSTEM_WINDOWS_GUI          2   // Image runs in the Windows GUI subsystem.
#define IMAGE_SUBSYSTEM_WINDOWS_CUI          3   // Image runs in the Windows character subsystem.
#define IMAGE_SUBSYSTEM_OS2_CUI              5   // image runs in the OS/2 character subsystem.
#define IMAGE_SUBSYSTEM_POSIX_CUI            7   // image run  in the Posix character subsystem.


// Directory Entries

#define IMAGE_DIRECTORY_ENTRY_EXPORT         0   // Export Directory
#define IMAGE_DIRECTORY_ENTRY_IMPORT         1   // Import Directory
#define IMAGE_DIRECTORY_ENTRY_RESOURCE       2   // Resource Directory
#define IMAGE_DIRECTORY_ENTRY_EXCEPTION      3   // Exception Directory
#define IMAGE_DIRECTORY_ENTRY_SECURITY       4   // Security Directory
#define IMAGE_DIRECTORY_ENTRY_BASERELOC      5   // Base Relocation Table
#define IMAGE_DIRECTORY_ENTRY_DEBUG          6   // Debug Directory
#define IMAGE_DIRECTORY_ENTRY_COPYRIGHT      7   // Description String
#define IMAGE_DIRECTORY_ENTRY_GLOBALPTR      8   // Machine Value (MIPS GP)
#define IMAGE_DIRECTORY_ENTRY_TLS            9   // TLS Directory
#define IMAGE_DIRECTORY_ENTRY_LOAD_CONFIG   10   // Load Configuration Directory

//
// Section header format.
//

#define IMAGE_SIZEOF_SHORT_NAME              8

typedef struct _IMAGE_SECTION_HEADER {
    TUint8 		Name[IMAGE_SIZEOF_SHORT_NAME];
    union	{
            TUint32   PhysicalAddress;
            TUint32   VirtualSize;
    		} Misc;
    TUint32		VirtualAddress;
    TUint32		SizeOfRawData;
    TUint32		PointerToRawData;
    TUint32		PointerToRelocations;
    TUint32		PointerToLinenumbers;
    TUint16		NumberOfRelocations;
    TUint16		NumberOfLinenumbers;
    TUint32		Characteristics;
} IMAGE_SECTION_HEADER, *PIMAGE_SECTION_HEADER;

#define IMAGE_SIZEOF_SECTION_HEADER          40

#define IMAGE_SCN_TYPE_NO_PAD                0x00000008  // Reserved.

#define IMAGE_SCN_CNT_CODE                   0x00000020  // Section contains code.
#define IMAGE_SCN_CNT_INITIALIZED_DATA       0x00000040  // Section contains initialized data.
#define IMAGE_SCN_CNT_UNINITIALIZED_DATA     0x00000080  // Section contains uninitialized data.

#define IMAGE_SCN_LNK_OTHER                  0x00000100  // Reserved.
#define IMAGE_SCN_LNK_INFO                   0x00000200  // Section contains comments or some other type of information.
#define IMAGE_SCN_LNK_REMOVE                 0x00000800  // Section contents will not become part of image.
#define IMAGE_SCN_LNK_COMDAT                 0x00001000  // Section contents comdat.

#define IMAGE_SCN_ALIGN_1Byte               0x00100000  //
#define IMAGE_SCN_ALIGN_2Byte               0x00200000  //
#define IMAGE_SCN_ALIGN_4Byte               0x00300000  //
#define IMAGE_SCN_ALIGN_8Byte               0x00400000  //
#define IMAGE_SCN_ALIGN_16Byte              0x00500000  // Default alignment if no others are specified.
#define IMAGE_SCN_ALIGN_32Byte              0x00600000  //
#define IMAGE_SCN_ALIGN_64Byte              0x00700000  //

#define IMAGE_SCN_MEM_DISCARDABLE            0x02000000  // Section can be discarded.
#define IMAGE_SCN_MEM_NOT_CACHED             0x04000000  // Section is not cachable.
#define IMAGE_SCN_MEM_NOT_PAGED              0x08000000  // Section is not pageable.
#define IMAGE_SCN_MEM_SHARED                 0x10000000  // Section is shareable.
#define IMAGE_SCN_MEM_EXECUTE                0x20000000  // Section is executable.
#define IMAGE_SCN_MEM_READ                   0x40000000  // Section is readable.
#define IMAGE_SCN_MEM_WRITE                  0x80000000  // Section is writeable.

//
// Symbol format.
//

typedef struct _IMAGE_SYMBOL {
    union {
        TUint8    ShortName[8];
        struct {
            TUint32   Short;     // if 0, use LongName
            TUint32   Long;      // offset into string table
        } Name;
        TUint8   *LongName[2];
    } N;
    TUint32   Value;
    TInt16   SectionNumber;
    TUint16    Type;
    TUint8    StorageClass;
    TUint8    NumberOfAuxSymbols;
} IMAGE_SYMBOL;
typedef IMAGE_SYMBOL *PIMAGE_SYMBOL;

#define IMAGE_SIZEOF_SYMBOL                  18

//
// Section values.
//
// Symbols have a section number of the section in which they are
// defined. Otherwise, section numbers have the following meanings:
//

#define IMAGE_SYM_UNDEFINED           (TInt16)0           // Symbol is undefined or is common.
#define IMAGE_SYM_ABSOLUTE            (TInt16)-1          // Symbol is an absolute value.
#define IMAGE_SYM_DEBUG               (TInt16)-2          // Symbol is a special debug item.

//
// Type (fundamental) values.
//

#define IMAGE_SYM_TYPE_NULL                  0           // no type.
#define IMAGE_SYM_TYPE_VOID                  1           //
#define IMAGE_SYM_TYPE_TUint8                  2           // type character.
#define IMAGE_SYM_TYPE_TInt16                 3           // type TInt16 integer.
#define IMAGE_SYM_TYPE_INT                   4           //
#define IMAGE_SYM_TYPE_TUint32                  5           //
#define IMAGE_SYM_TYPE_FLOAT                 6           //
#define IMAGE_SYM_TYPE_DOUBLE                7           //
#define IMAGE_SYM_TYPE_STRUCT                8           //
#define IMAGE_SYM_TYPE_UNION                 9           //
#define IMAGE_SYM_TYPE_ENUM                  10          // enumeration.
#define IMAGE_SYM_TYPE_MOE                   11          // member of enumeration.
#define IMAGE_SYM_TYPE_BYTE                  12          //
#define IMAGE_SYM_TYPE_WORD                  13          //
#define IMAGE_SYM_TYPE_UINT                  14          //
#define IMAGE_SYM_TYPE_DWORD                 15          //

//
// Type (derived) values.
//

#define IMAGE_SYM_DTYPE_NULL                 0           // no derived type.
#define IMAGE_SYM_DTYPE_POINTER              1           // pointer.
#define IMAGE_SYM_DTYPE_FUNCTION             2           // function.
#define IMAGE_SYM_DTYPE_ARRAY                3           // array.

//
// Storage classes.
//

#define IMAGE_SYM_CLASS_END_OF_FUNCTION      (TUint8 )-1
#define IMAGE_SYM_CLASS_NULL                 0
#define IMAGE_SYM_CLASS_AUTOMATIC            1
#define IMAGE_SYM_CLASS_EXTERNAL             2
#define IMAGE_SYM_CLASS_STATIC               3
#define IMAGE_SYM_CLASS_REGISTER             4
#define IMAGE_SYM_CLASS_EXTERNAL_DEF         5
#define IMAGE_SYM_CLASS_LABEL                6
#define IMAGE_SYM_CLASS_UNDEFINED_LABEL      7
#define IMAGE_SYM_CLASS_MEMBER_OF_STRUCT     8
#define IMAGE_SYM_CLASS_ARGUMENT             9
#define IMAGE_SYM_CLASS_STRUCT_TAG           10
#define IMAGE_SYM_CLASS_MEMBER_OF_UNION      11
#define IMAGE_SYM_CLASS_UNION_TAG            12
#define IMAGE_SYM_CLASS_TYPE_DEFINITION      13
#define IMAGE_SYM_CLASS_UNDEFINED_STATIC     14
#define IMAGE_SYM_CLASS_ENUM_TAG             15
#define IMAGE_SYM_CLASS_MEMBER_OF_ENUM       16
#define IMAGE_SYM_CLASS_REGISTER_PARAM       17
#define IMAGE_SYM_CLASS_BIT_FIELD            18
#define IMAGE_SYM_CLASS_BLOCK                100
#define IMAGE_SYM_CLASS_FUNCTION             101
#define IMAGE_SYM_CLASS_END_OF_STRUCT        102
#define IMAGE_SYM_CLASS_FILE                 103
// new
#define IMAGE_SYM_CLASS_SECTION              104
#define IMAGE_SYM_CLASS_WEAK_EXTERNAL        105

// type packing constants

#define N_BTMASK                            017
#define N_TMASK                             060
#define N_TMASK1                            0300
#define N_TMASK2                            0360
#define N_BTSHFT                            4
#define N_TSHIFT                            2

// MACROS

// Basic Type of  x
#define BTYPE(x) ((x) & N_BTMASK)

// Is x a pointer?
#ifndef ISPTR
#define ISPTR(x) (((x) & N_TMASK) == (IMAGE_SYM_DTYPE_POINTER << N_BTSHFT))
#endif

// Is x a function?
#ifndef ISFCN
#define ISFCN(x) (((x) & N_TMASK) == (IMAGE_SYM_DTYPE_FUNCTION << N_BTSHFT))
#endif

// Is x an array?

#ifndef ISARY
#define ISARY(x) (((x) & N_TMASK) == (IMAGE_SYM_DTYPE_ARRAY << N_BTSHFT))
#endif

// Is x a structure, union, or enumeration TAG?
#ifndef ISTAG
#define ISTAG(x) ((x)==IMAGE_SYM_CLASS_STRUCT_TAG || (x)==IMAGE_SYM_CLASS_UNION_TAG || (x)==IMAGE_SYM_CLASS_ENUM_TAG)
#endif

#ifndef INCREF
#define INCREF(x) ((((x)&~N_BTMASK)<<N_TSHIFT)|(IMAGE_SYM_DTYPE_POINTER<<N_BTSHFT)|((x)&N_BTMASK))
#endif
#ifndef DECREF
#define DECREF(x) ((((x)>>N_TSHIFT)&~N_BTMASK)|((x)&N_BTMASK))
#endif

//
// Auxiliary entry format.
//

typedef union _IMAGE_AUX_SYMBOL {
    struct {
        TUint32    TagIndex;                      // struct, union, or enum tag index
        union {
            struct {
                TUint16    Linenumber;             // declaration line number
                TUint16    Size;                   // size of struct, union, or enum
            } LnSz;
           TUint32    TotalSize;
        } Misc;
        union {
            struct {                            // if ISFCN, tag, or .bb
                TUint32    PointerToLinenumber;
                TUint32    PointerToNextFunction;
            } Function;
            struct {                            // if ISARY, up to 4 dimen.
                TUint16     Dimension[4];
            } Array;
        } FcnAry;
        TUint16    TvIndex;                        // tv index
    } Sym;
    struct {
        TUint8    Name[IMAGE_SIZEOF_SYMBOL];
    } File;
    struct {
        TUint32   Length;                         // section length
        TUint16    NumberOfRelocations;            // number of relocation entries
        TUint16    NumberOfLinenumbers;            // number of line numbers
        TUint32   CheckSum;                       // checksum for communal
        TInt16   Number;                         // section number to associate with
        TUint8    Selection;                      // communal selection type
    } Section;
} IMAGE_AUX_SYMBOL;
typedef IMAGE_AUX_SYMBOL *PIMAGE_AUX_SYMBOL;

#define IMAGE_SIZEOF_AUX_SYMBOL              18

//
// Communal selection types.
//

#define IMAGE_COMDAT_SELECT_NODUPLICATES   1
#define IMAGE_COMDAT_SELECT_ANY            2
#define IMAGE_COMDAT_SELECT_SAME_SIZE      3
#define IMAGE_COMDAT_SELECT_EXACT_MATCH    4
#define IMAGE_COMDAT_SELECT_ASSOCIATIVE    5

#define IMAGE_WEAK_EXTERN_SEARCH_NOLIBRARY 1
#define IMAGE_WEAK_EXTERN_SEARCH_LIBRARY   2
#define IMAGE_WEAK_EXTERN_SEARCH_ALIAS     3


//
// Relocation format.
//

typedef struct _IMAGE_RELOCATION {
    TUint32   VirtualAddress;
    TUint32   SymbolTableIndex;
    TUint16    Type;
} IMAGE_RELOCATION;
typedef IMAGE_RELOCATION *PIMAGE_RELOCATION;

#define IMAGE_SIZEOF_RELOCATION              10

//
// I386 relocation types.
//

#define IMAGE_REL_I386_ABSOLUTE              0           // Reference is absolute, no relocation is necessary
#define IMAGE_REL_I386_DIR16                 01          // Direct 16-bit reference to the symbols virtual address
#define IMAGE_REL_I386_REL16                 02          // PC-relative 16-bit reference to the symbols virtual address
#define IMAGE_REL_I386_DIR32                 06          // Direct 32-bit reference to the symbols virtual address
#define IMAGE_REL_I386_DIR32NB               07          // Direct 32-bit reference to the symbols virtual address, base not included
#define IMAGE_REL_I386_SEG12                 011         // Direct 16-bit reference to the segment-selector bits of a 32-bit virtual address
#define IMAGE_REL_I386_SECTION               012
#define IMAGE_REL_I386_SECREL                013
#define IMAGE_REL_I386_REL32                 024         // PC-relative 32-bit reference to the symbols virtual address

//
// MIPS relocation types.
//

#define IMAGE_REL_MIPS_ABSOLUTE              0           // Reference is absolute, no relocation is necessary
#define IMAGE_REL_MIPS_REFHALF               01
#define IMAGE_REL_MIPS_REFTUint16               02
#define IMAGE_REL_MIPS_JMPADDR               03
#define IMAGE_REL_MIPS_REFHI                 04
#define IMAGE_REL_MIPS_REFLO                 05
#define IMAGE_REL_MIPS_GPREL                 06
#define IMAGE_REL_MIPS_LITERAL               07
#define IMAGE_REL_MIPS_SECTION               012
#define IMAGE_REL_MIPS_SECREL                013
#define IMAGE_REL_MIPS_REFTUint16NB             042
#define IMAGE_REL_MIPS_PAIR                  045

//
// Alpha Relocation types.
//

#define IMAGE_REL_ALPHA_ABSOLUTE             0x0
#define IMAGE_REL_ALPHA_REFTUint32              0x1
#define IMAGE_REL_ALPHA_REFQUAD              0x2
#define IMAGE_REL_ALPHA_GPREL32              0x3
#define IMAGE_REL_ALPHA_LITERAL              0x4
#define IMAGE_REL_ALPHA_LITUSE               0x5
#define IMAGE_REL_ALPHA_GPDISP               0x6
#define IMAGE_REL_ALPHA_BRADDR               0x7
#define IMAGE_REL_ALPHA_HINT                 0x8
#define IMAGE_REL_ALPHA_INLINE_REFTUint32       0x9
#define IMAGE_REL_ALPHA_REFHI                0xA
#define IMAGE_REL_ALPHA_REFLO                0xB
#define IMAGE_REL_ALPHA_PAIR                 0xC
#define IMAGE_REL_ALPHA_MATCH                0xD
#define IMAGE_REL_ALPHA_SECTION              0xE
#define IMAGE_REL_ALPHA_SECREL               0xF
#define IMAGE_REL_ALPHA_REFTUint32NB            0x10

//
// IBM PowerPC relocation types.
//

#define IMAGE_REL_PPC_ABSOLUTE 0x0000  // NOP
#define IMAGE_REL_PPC_ADDR64   0x0001  // 64-bit address
#define IMAGE_REL_PPC_ADDR32   0x0002  // 32-bit address
#define IMAGE_REL_PPC_ADDR24   0x0003  // 26-bit address, shifted left 2 (branch absolute)
#define IMAGE_REL_PPC_ADDR16   0x0004  // 16-bit address
#define IMAGE_REL_PPC_ADDR14   0x0005  // 16-bit address, shifted left 2 (load doubleTUint16)
#define IMAGE_REL_PPC_REL24    0x0006  // 26-bit PC-relative offset, shifted left 2 (branch relative)
#define IMAGE_REL_PPC_REL14    0x0007  // 16-bit PC-relative offset, shifted left 2 (br cond relative)
#define IMAGE_REL_PPC_TOCREL16 0x0008  // 16-bit offset from TOC base
#define IMAGE_REL_PPC_TOCREL14 0x0009  // 16-bit offset from TOC base, shifted left 2 (load doubleTUint16)

#define IMAGE_REL_PPC_ADDR32NB 0x000A  // 32-bit addr w/o image base
#define IMAGE_REL_PPC_SECREL   0x000B  // va of containing section (as in an image sectionhdr)
#define IMAGE_REL_PPC_SECTION  0x000C  // sectionheader number
#define IMAGE_REL_PPC_IFGLUE   0x000D  // substitute TOC restore instruction iff symbol is glue code
#define IMAGE_REL_PPC_IMGLUE   0x000E  // symbol is glue code; virtual address is TOC restore instruction

#define IMAGE_REL_PPC_TYPEMASK 0x00FF  // mask to isolate above values in IMAGE_RELOCATION.Type

// Flag bits in IMAGE_RELOCATION.TYPE

#define IMAGE_REL_PPC_NEG      0x0100  // subtract reloc value rather than adding it
#define IMAGE_REL_PPC_BRTAKEN  0x0200  // fix branch prediction bit to predict branch taken
#define IMAGE_REL_PPC_BRNTAKEN 0x0400  // fix branch prediction bit to predict branch not taken
#define IMAGE_REL_PPC_TOCDEFN  0x0800  // toc slot defined in file (or, data in toc)

//
// Based relocation format.
//

typedef struct _IMAGE_BASE_RELOCATION {
    TUint32   VirtualAddress;
    TUint32   SizeOfBlock;
//  TUint16    TypeOffset[1];
} IMAGE_BASE_RELOCATION, *PIMAGE_BASE_RELOCATION;

#define IMAGE_SIZEOF_BASE_RELOCATION         8

//
// Based relocation types.
//

#define IMAGE_REL_BASED_ABSOLUTE              0
#define IMAGE_REL_BASED_HIGH                  1
#define IMAGE_REL_BASED_LOW                   2
#define IMAGE_REL_BASED_HIGHLOW               3
#define IMAGE_REL_BASED_HIGHADJ               4
#define IMAGE_REL_BASED_MIPS_JMPADDR          5

//
// Line number format.
//

typedef struct _IMAGE_LINENUMBER {
    union {
        TUint32   SymbolTableIndex;               // Symbol table index of function name if Linenumber is 0.
        TUint32   VirtualAddress;                 // Virtual address of line number.
    } Type;
    TUint16    Linenumber;                         // Line number.
} IMAGE_LINENUMBER;
typedef IMAGE_LINENUMBER *PIMAGE_LINENUMBER;

#define IMAGE_SIZEOF_LINENUMBER              6

//
// Archive format.
//

#define IMAGE_ARCHIVE_START_SIZE             8
#define IMAGE_ARCHIVE_START                  "!<arch>\n"
#define IMAGE_ARCHIVE_END                    "`\n"
#define IMAGE_ARCHIVE_PAD                    "\n"
#define IMAGE_ARCHIVE_LINKER_MEMBER          "/               "
#define IMAGE_ARCHIVE_TUint32NAMES_MEMBER       "//              "

typedef struct _IMAGE_ARCHIVE_MEMBER_HEADER {
    TUint8     Name[16];                          // File member name - `/' terminated.
    TUint8     Date[12];                          // File member date - decimal.
    TUint8     UserID[6];                         // File member user id - decimal.
    TUint8     GroupID[6];                        // File member group id - decimal.
    TUint8     Mode[8];                           // File member mode - octal.
    TUint8     Size[10];                          // File member size - decimal.
    TUint8     EndHeader[2];                      // String to end header.
} IMAGE_ARCHIVE_MEMBER_HEADER, *PIMAGE_ARCHIVE_MEMBER_HEADER;

#define IMAGE_SIZEOF_ARCHIVE_MEMBER_HDR      60

//
// DLL support.
//

//
// Export Format
//

typedef struct _IMAGE_EXPORT_DIRECTORY {
    TUint32   Characteristics;
    TUint32   TimeDateStamp;
    TUint16    MajorVersion;
    TUint16    MinorVersion;
    TUint32   Name;
    TUint32   Base;
    TUint32   NumberOfFunctions;
    TUint32   NumberOfNames;
    TUint32  **AddressOfFunctions;
    TUint32  **AddressOfNames;
    TUint16   **AddressOfNameOrdinals;
} IMAGE_EXPORT_DIRECTORY, *PIMAGE_EXPORT_DIRECTORY;

//
// Import Format
//

typedef struct _IMAGE_IMPORT_BY_NAME {
    TUint16    Hint;
    TUint8    Name[1];
} IMAGE_IMPORT_BY_NAME, *PIMAGE_IMPORT_BY_NAME;

typedef struct _IMAGE_THUNK_DATA {
    union {
        TUint32 *Function;
        TUint32 Ordinal;
        PIMAGE_IMPORT_BY_NAME AddressOfData;
    } u1;
} IMAGE_THUNK_DATA, *PIMAGE_THUNK_DATA;

#define IMAGE_ORDINAL_FLAG 0x80000000
#define IMAGE_SNAP_BY_ORDINAL(Ordinal) ((Ordinal & IMAGE_ORDINAL_FLAG) != 0)
#define IMAGE_ORDINAL(Ordinal) (Ordinal & 0xffff)

typedef struct _IMAGE_IMPORT_DESCRIPTOR {
    TUint32   Characteristics;
    TUint32   TimeDateStamp;
    TUint32   ForwarderChain;
    TUint32   Name;
    PIMAGE_THUNK_DATA FirstThunk;
} IMAGE_IMPORT_DESCRIPTOR, *PIMAGE_IMPORT_DESCRIPTOR;


//
// Debug Format
//

typedef struct _IMAGE_DEBUG_DIRECTORY {
    TUint32   characteristics;
    TUint32   TimeDateStamp;
    TUint16    MajorVersion;
    TUint16    MinorVersion;
    TUint32   Type;
    TUint32   SizeOfData;
    TUint32   AddressOfRawData;
    TUint32   PointerToRawData;
} IMAGE_DEBUG_DIRECTORY, *PIMAGE_DEBUG_DIRECTORY;

#define IMAGE_DEBUG_TYPE_UNKNOWN          0
#define IMAGE_DEBUG_TYPE_COFF             1
#define IMAGE_DEBUG_TYPE_CODEVIEW         2
#define IMAGE_DEBUG_TYPE_FPO              3
#define IMAGE_DEBUG_TYPE_MISC             4
#define IMAGE_DEBUG_TYPE_EXCEPTION        5
#define IMAGE_DEBUG_TYPE_FIXUP            6
#define IMAGE_DEBUG_TYPE_OMAP_TO_SRC      7
#define IMAGE_DEBUG_TYPE_OMAP_FROM_SRC    8

typedef struct _IMAGE_COFF_SYMBOLS_HEADER {
    TUint32   NumberOfSymbols;
    TUint32   LvaToFirstSymbol;
    TUint32   NumberOfLinenumbers;
    TUint32   LvaToFirstLinenumber;
    TUint32   RvaToFirstTUint8OfCode;
    TUint32   RvaToLastTUint8OfCode;
    TUint32   RvaToFirstTUint8OfData;
    TUint32   RvaToLastTUint8OfData;
} IMAGE_COFF_SYMBOLS_HEADER, *PIMAGE_COFF_SYMBOLS_HEADER;

#define FRAME_FPO   0
#define FRAME_TRAP  1
#define FRAME_TSS   2

typedef struct _FPO_DATA {
    TUint32       ulOffStart;             // offset 1st TUint8 of function code
    TUint32       cbProcSize;             // # TUint8 in function
    TUint32       cdwLocals;              // # TUint8 in locals/4
    TUint16        cdwParams;              // # TUint8 in params/4
    TUint16        cbProlog : 8;           // # TUint8 in prolog
    TUint16        cbRegs   : 3;           // # regs saved
    TUint16        fHasSEH  : 1;           // TRUE if SEH in func
    TUint16        fUseBP   : 1;           // TRUE if EBP has been allocated
    TUint16        reserved : 1;           // reserved for future use
    TUint16        cbFrame  : 2;           // frame type
} FPO_DATA, *PFPO_DATA;
#define SIZEOF_RFPO_DATA 16


#define IMAGE_DEBUG_MISC_EXENAME    1

typedef struct _IMAGE_DEBUG_MISC {
    TUint32       DataType;               // type of misc data, see defines
    TUint32       Length;                 // total length of record, rounded to four
                                        // TUint8 multiple.
    TBool     Unicode;                // TRUE if data is unicode string
    TUint8        Reserved[ 3 ];
    TUint8        Data[ 1 ];              // Actual data
} IMAGE_DEBUG_MISC, *PIMAGE_DEBUG_MISC;


//
// Function table extracted from MIPS/ALPHA images.  Does not contain
// information needed only for runtime support.  Just those fields for
// each entry needed by a debugger.
//

typedef struct _IMAGE_FUNCTION_ENTRY {
    TUint32   StartingAddress;
    TUint32   EndingAddress;
    TUint32   EndOfPrologue;
} IMAGE_FUNCTION_ENTRY, *PIMAGE_FUNCTION_ENTRY;

//
// Debugging information can be stripped from an image file and placed
// in a separate .DBG file, whose file name part is the same as the
// image file name part (e.g. symbols for CMD.EXE could be stripped
// and placed in CMD.DBG).  This is indicated by the IMAGE_FILE_DEBUG_STRIPPED
// flag in the characteristics field of the file header.  The beginning of
// the .DBG file contains the following structure which captures certain
// information from the image file.  This allows a debug to proceed even if
// the original image file is not accessable.  This header is followed by
// zero of more IMAGE_SECTION_HEADER structures, followed by zero or more
// IMAGE_DEBUG_DIRECTORY structures.  The latter structures and those in
// the image file contain file offsets relative to the beginning of the
// .DBG file.
//
// If symbols have been stripped from an image, the IMAGE_DEBUG_MISC structure
// is left in the image file, but not mapped.  This allows a debugger to
// compute the name of the .DBG file, from the name of the image in the
// IMAGE_DEBUG_MISC structure.
//

typedef struct _IMAGE_SEPARATE_DEBUG_HEADER {
    TUint16        Signature;
    TUint16        Flags;
    TUint16        Machine;
    TUint16        characteristics;
    TUint32       TimeDateStamp;
    TUint32       CheckSum;
    TUint32       ImageBase;
    TUint32       SizeOfImage;
    TUint32       NumberOfSections;
    TUint32       ExportedNamesSize;
    TUint32       DebugDirectorySize;
    TUint32       Reserved[ 3 ];          // Note: reserved[0] is actually the section alignment
} IMAGE_SEPARATE_DEBUG_HEADER, *PIMAGE_SEPARATE_DEBUG_HEADER;

#define IMAGE_SEPARATE_DEBUG_SIGNATURE 0x4944

#endif
