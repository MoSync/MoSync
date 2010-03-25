/* ------------------------------------------------------------------
 * Copyright (C) 2008 PacketVideo
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either
 * express or implied.
 * See the License for the specific language governing permissions
 * and limitations under the License.
 * -------------------------------------------------------------------
 */
/****************************************************************************************
Portions of this file are derived from the following 3GPP standard:

    3GPP TS 26.073
    ANSI-C code for the Adaptive Multi-Rate (AMR) speech codec
    Available from http://www.3gpp.org

(C) 2004, 3GPP Organizational Partners (ARIB, ATIS, CCSA, ETSI, TTA, TTC)
Permission to distribute, modify and use this file under the standard license
terms listed above has been obtained from the copyright holder.
****************************************************************************************/
/*

 Pathname: ./audio/gsm-amr/c/include/cor_h.h

     Date: 09/07/2000

------------------------------------------------------------------------------
 REVISION HISTORY

 Description: Created separate header file for cor_h_x. Sync'ed up with the
	      PV header template. Added include to cor_h_x.h for legacy files
	      that include cor_h.h when invoking cor_h_x function.

 Description: Added cor_h_x2.h in the include section.

 Description: Removed cnst.h in the Include section. Putting it here confuses
              Visual C++ when building the unit test. The source code
              includes cnst.h prior to cor_h.h.

 Description: Updated copyright year and pathname.

 Description: Updated function prototype to reflect
 inclusion of pOverflow flag.

 Description: Updated template to make it build in Symbian. Updated copyright
              year.

 Description: Moved _cplusplus #ifdef after Include section.

 Description:

------------------------------------------------------------------------------
 INCLUDE DESCRIPTION

 This file contains prototype declaration for cor_h function.

------------------------------------------------------------------------------
*/
/*----------------------------------------------------------------------------
; CONTINUE ONLY IF NOT ALREADY DEFINED
----------------------------------------------------------------------------*/
#ifndef	COR_H_H
#define	COR_H_H

/*----------------------------------------------------------------------------
; INCLUDES
----------------------------------------------------------------------------*/
#include "typedef.h"

#include "cor_h_x.h"				/* Used by legacy files */
#include "cor_h_x2.h"				/* Used by legacy files */

/*--------------------------------------------------------------------------*/
#ifdef __cplusplus
extern "C"
{
#endif

    /*----------------------------------------------------------------------------
    ; MACROS
    ; Define module specific macros here
    ----------------------------------------------------------------------------*/

    /*----------------------------------------------------------------------------
    ; DEFINES
    ; Include all pre-processor statements here.
    ----------------------------------------------------------------------------*/

    /*----------------------------------------------------------------------------
    ; EXTERNAL VARIABLES REFERENCES
    ; Declare variables used in this module but defined elsewhere
    ----------------------------------------------------------------------------*/

    /*----------------------------------------------------------------------------
    ; SIMPLE TYPEDEF'S
    ----------------------------------------------------------------------------*/

    /*----------------------------------------------------------------------------
    ; ENUMERATED TYPEDEF'S
    ----------------------------------------------------------------------------*/

    /*----------------------------------------------------------------------------
    ; STRUCTURES TYPEDEF'S
    ----------------------------------------------------------------------------*/

    /*----------------------------------------------------------------------------
    ; GLOBAL FUNCTION DEFINITIONS
    ; Function Prototype declaration
    ----------------------------------------------------------------------------*/
    void cor_h(
        Word16 h[],          /* (i) : impulse response of weighted synthesis
                                  filter                                  */
        Word16 sign[],       /* (i) : sign of d[n]                            */
        Word16 rr[][L_CODE], /* (o) : matrix of autocorrelation               */
        Flag  *pOverflow
    );

    /*----------------------------------------------------------------------------
    ; END
    ----------------------------------------------------------------------------*/
#ifdef __cplusplus
}
#endif

#endif

