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

/* 
 * File:   AnalyseCode.h
 * Author: Ali Mosavian
 *
 * Created on March 25, 2010, 6:36 PM
 */
#ifndef __ANALYSECODE_H__
#define	__ANALYSECODE_H__

#define MAX_LOOP_LIST 1024


/**
 * Contains the result of the analysis
 *
 */
typedef struct AnalysisContext_
{
    SYMBOL *baseSym;
    int baseIP;
    int baseEIP;

    int loopIP[MAX_LOOP_LIST];
    int prevLoopIP[MAX_LOOP_LIST];
    int loopCountIP[MAX_LOOP_LIST];

    int loopCount;
} AnalysisContext;



/**
 * Analyzes a function for loops and such.
 *
 * @param ctx Pointer to a analysis context
 * @param sym Pointer to the function symbol
 */
void TestAnalyse ( AnalysisContext *ctx, SYMBOL *func );


#endif	/* __ANALYSECODE_H__ */

