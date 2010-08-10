#ifndef __PATCHER_H__
#define __PATCHER_H__

/**
 * Patches the code section
 *
 *
 */
void CodePatcher ( byte* mem_cs, 
                   const int* mem_cp,
                   int entry, 
                   int cs_size );

#endif