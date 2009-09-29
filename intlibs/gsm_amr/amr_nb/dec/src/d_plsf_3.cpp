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
------------------------------------------------------------------------------



 Pathname: ./audio/gsm-amr/c/src/d_plsf_3.c
 Functions: D_plsf_3

     Date: 01/31/2002

------------------------------------------------------------------------------
 REVISION HISTORY

 Description: Updated to accept new parameter, Flag *pOverflow.  Placed
 file in the proper PV Software template.

 Description:
 (1) Removed "count.h" and "basic_op.h" and replaced with individual include
     files (add.h, sub.h, etc.)

 Description:
 (1) Removed optimization -- mult(i, 3, pOverflow) is NOT the same as adding
     i to itself 3 times.  The reason is because the mult function does a
     right shift by 15, which will obliterate smaller numbers.

 Description:
 (1) Logic error -- a comparison against zero should have been DOES NOT EQUAL
 zero, but instead, was translated into EQUALS ZERO.  The bug has been fixed.

 Description:  Replaced "int" and/or "char" with OSCL defined types.

 Description: Added #ifdef __cplusplus around extern'ed table.

 Description:

 ------------------------------------------------------------------------------
 INPUT AND OUTPUT DEFINITIONS

 Inputs:
    st -- Pointer to type struct D_plsfState
    mode -- enum Mode -- coder mode
    bfi -- Word16 -- bad frame indicator (set to 1 if a bad frame is received)
    indice -- Pointer to type Word16 -- quantization indices of
                                        3 submatrices, Q0

 Outputs:
    st -- Pointer to type struct D_plsfState
    lsp1_q -- Pointer to type Word16 -- quantized 1st LSP vector Q15
    pOverflow -- Pointer to type Flag -- Flag set when overflow occurs

 Returns:
    None.

 Global Variables Used:
    None

 Local Variables Needed:
    None

------------------------------------------------------------------------------
 FUNCTION DESCRIPTION

 PURPOSE: Decodes the LSP parameters using the received quantization
          indices.1st order MA prediction and split by 3 vector
          quantization (split-VQ)

------------------------------------------------------------------------------
 REQUIREMENTS



------------------------------------------------------------------------------
 REFERENCES

 d_plsf_3.c, UMTS GSM AMR speech codec, R99 - Version 3.2.0, March 2, 2001

------------------------------------------------------------------------------
 PSEUDO-CODE



------------------------------------------------------------------------------
 RESOURCES USED
   When the code is written for a specific target processor the
     the resources used should be documented below.

 STACK USAGE: [stack count for this module] + [variable to represent
          stack usage for each subroutine called]

     where: [stack usage variable] = stack usage for [subroutine
         name] (see [filename].ext)

 DATA MEMORY USED: x words

 PROGRAM MEMORY USED: x words

 CLOCK CYCLES: [cycle count equation for this module] + [variable
           used to represent cycle count for each subroutine
           called]

     where: [cycle count variable] = cycle count for [subroutine
        name] (see [filename].ext)

------------------------------------------------------------------------------
*/


/*----------------------------------------------------------------------------
; INCLUDES
----------------------------------------------------------------------------*/
#include "d_plsf.h"
#include "typedef.h"
#include "basic_op.h"
#include "lsp_lsf.h"
#include "reorder.h"
#include "copy.h"

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
    ; Include all pre-processor statements here. Include conditional
    ; compile variables also.
    ----------------------------------------------------------------------------*/
#define ALPHA     29491     /* ALPHA    ->  0.9                            */
#define ONE_ALPHA 3277      /* ONE_ALPHA-> (1.0-ALPHA)                     */


    /*----------------------------------------------------------------------------
    ; LOCAL FUNCTION DEFINITIONS
    ; Function Prototype declaration
    ----------------------------------------------------------------------------*/


    /*----------------------------------------------------------------------------
    ; LOCAL STORE/BUFFER/POINTER DEFINITIONS
    ; Variable declaration - defined here and used outside this module
    ----------------------------------------------------------------------------*/

    /*----------------------------------------------------------------------------
    ; EXTERNAL FUNCTION REFERENCES
    ; Declare functions defined elsewhere and referenced in this module
    ----------------------------------------------------------------------------*/

    /*----------------------------------------------------------------------------
    ; EXTERNAL GLOBAL STORE/BUFFER/POINTER REFERENCES
    ; Declare variables used in this module but defined elsewhere
    ----------------------------------------------------------------------------*/
    /* Codebooks of LSF prediction residual */
    extern const Word16 mean_lsf_3[];

    extern const Word16 pred_fac_3[];

    extern const Word16 dico1_lsf_3[];
    extern const Word16 dico2_lsf_3[];
    extern const Word16 dico3_lsf_3[];

    extern const Word16 mr515_3_lsf[];
    extern const Word16 mr795_1_lsf[];

    extern const Word16 past_rq_init[];

    /*--------------------------------------------------------------------------*/
#ifdef __cplusplus
}
#endif

/*----------------------------------------------------------------------------
; FUNCTION CODE
----------------------------------------------------------------------------*/

void D_plsf_3(
    D_plsfState *st,   /* i/o: State struct                               */
    enum Mode mode,    /* i  : coder mode                                 */
    Word16 bfi,        /* i  : bad frame indicator (set to 1 if a         */
    /*      bad frame is received)                     */
    Word16 * indice,   /* i  : quantization indices of 3 submatrices, Q0  */
    Word16 * lsp1_q,   /* o  : quantized 1st LSP vector,              Q15 */
    Flag  *pOverflow   /* o : Flag set when overflow occurs               */
)
{
    Word16 i;
    Word16 index;

    const Word16 *p_cb1;
    const Word16 *p_cb2;
    const Word16 *p_cb3;
    const Word16 *p_dico;
    Word16 temp;

    Word16 lsf1_r[M];
    Word16 lsf1_q[M];

    if (bfi != 0)   /* if bad frame */
    {
        /* use the past LSFs slightly shifted towards their mean */

        for (i = 0; i < M; i++)
        {
            /* lsfi_q[i] = ALPHA*past_lsf_q[i] + ONE_ALPHA*mean_lsf[i]; */
            temp =
                mult(
                    st->past_lsf_q[i],
                    ALPHA,
                    pOverflow);

            index =
                mult(
                    mean_lsf_3[i],
                    ONE_ALPHA,
                    pOverflow);

            lsf1_q[i] =
                add(
                    index,
                    temp,
                    pOverflow);
        }

        /* estimate past quantized residual to be used in next frame */
        if (mode != MRDTX)
        {
            for (i = 0; i < M; i++)
            {
                /* temp  = mean_lsf[i] +  past_r2_q[i] * PRED_FAC; */

                temp =
                    mult(
                        st->past_r_q[i],
                        pred_fac_3[i],
                        pOverflow);

                temp =
                    add(
                        mean_lsf_3[i],
                        temp,
                        pOverflow);

                st->past_r_q[i] =
                    sub(
                        lsf1_q[i],
                        temp,
                        pOverflow);
            }

        } /* if (mode == MRDTX) */
        else
        {
            for (i = 0; i < M; i++)
            {
                /* temp  = mean_lsf[i] +  past_r2_q[i]; */

                temp =
                    add(
                        mean_lsf_3[i],
                        st->past_r_q[i],
                        pOverflow);

                st->past_r_q[i] =
                    sub(
                        lsf1_q[i],
                        temp,
                        pOverflow);
            }
        }

    } /* if (bfi != 0) */

    else  /* if good LSFs received */
    {
        if ((mode == MR475) || (mode == MR515))
        {   /* MR475, MR515 */
            p_cb1 = dico1_lsf_3;
            p_cb2 = dico2_lsf_3;
            p_cb3 = mr515_3_lsf;
        }
        else if (mode == MR795)
        {   /* MR795 */
            p_cb1 = mr795_1_lsf;
            p_cb2 = dico2_lsf_3;
            p_cb3 = dico3_lsf_3;
        }
        else
        {   /* MR59, MR67, MR74, MR102, MRDTX */
            p_cb1 = dico1_lsf_3;
            p_cb2 = dico2_lsf_3;
            p_cb3 = dico3_lsf_3;
        }

        /* decode prediction residuals from 3 received indices */

        index = *indice++;

        /* temp = 3*index; */
        temp = add(index, index, pOverflow);
        temp = add(temp, index, pOverflow);

        p_dico = &p_cb1[temp];

        lsf1_r[0] = *p_dico++;
        lsf1_r[1] = *p_dico++;
        lsf1_r[2] = *p_dico++;

        index = *indice++;

        if (mode == MR475 || mode == MR515)
        {   /* MR475, MR515 only using every second entry */
            index =
                shl(
                    index,
                    1,
                    pOverflow);
        }

        /* temp = 3*index */
        temp = add(index, index, pOverflow);
        temp = add(index, temp, pOverflow);

        p_dico = &p_cb2[temp];

        lsf1_r[3] = *p_dico++;
        lsf1_r[4] = *p_dico++;
        lsf1_r[5] = *p_dico++;

        index = *indice++;

        temp =
            shl(
                index,
                2,
                pOverflow);

        p_dico = &p_cb3[temp];

        lsf1_r[6] = *p_dico++;
        lsf1_r[7] = *p_dico++;
        lsf1_r[8] = *p_dico++;
        lsf1_r[9] = *p_dico++;

        /* Compute quantized LSFs and update the past quantized residual */

        if (mode != MRDTX)
        {
            for (i = 0; i < M; i++)
            {
                temp =
                    mult(
                        st->past_r_q[i],
                        pred_fac_3[i],
                        pOverflow);

                temp =
                    add(
                        mean_lsf_3[i],
                        temp,
                        pOverflow);

                lsf1_q[i] =
                    add(
                        lsf1_r[i],
                        temp,
                        pOverflow);

                st->past_r_q[i] = lsf1_r[i];
            }
        }
        else
        {
            for (i = 0; i < M; i++)
            {
                temp =
                    add(
                        mean_lsf_3[i],
                        st->past_r_q[i],
                        pOverflow);

                lsf1_q[i] =
                    add(
                        lsf1_r[i],
                        temp,
                        pOverflow);

                st->past_r_q[i] = lsf1_r[i];
            }
        }

    }

    /* verification that LSFs has minimum distance of LSF_GAP Hz */

    Reorder_lsf(
        lsf1_q,
        LSF_GAP,
        M,
        pOverflow);

    Copy(
        lsf1_q,
        st->past_lsf_q,
        M);

    /*  convert LSFs to the cosine domain */

    Lsf_lsp(
        lsf1_q,
        lsp1_q,
        M,
        pOverflow);

    return;
}

/*
------------------------------------------------------------------------------
 FUNCTION NAME: Init_D_plsf_3
------------------------------------------------------------------------------
 INPUT AND OUTPUT DEFINITIONS

 Inputs:
    st = pointer to a structure of type D_plsfState
    index = Word16, past_rq_init[] index [0, 7]

 Outputs:
    st = pointer to a structure of type D_plsfState

 Returns:
    None

 Global Variables Used:
    None.

 Local Variables Needed:
    None.

------------------------------------------------------------------------------
 FUNCTION DESCRIPTION

 This function initializes the D_plsfState structure.

------------------------------------------------------------------------------
 REQUIREMENTS

 None.

------------------------------------------------------------------------------
 REFERENCES

 d_plsf_3.c, UMTS GSM AMR speech codec, R99 - Version 3.2.0, March 2, 2001

------------------------------------------------------------------------------
 PSEUDO-CODE

------------------------------------------------------------------------------
 RESOURCES USED [optional]

 When the code is written for a specific target processor the
 the resources used should be documented below.

 HEAP MEMORY USED: x bytes

 STACK MEMORY USED: x bytes

 CLOCK CYCLES: (cycle count equation for this function) + (variable
                used to represent cycle count for each subroutine
                called)
     where: (cycle count variable) = cycle count for [subroutine
                                     name]

------------------------------------------------------------------------------
 CAUTION [optional]
 [State any special notes, constraints or cautions for users of this function]

------------------------------------------------------------------------------
*/
void Init_D_plsf_3(
    D_plsfState *st,      /* i/o: State struct                */
    Word16       index    /* i  : past_rq_init[] index [0, 7] */)
{
    Copy(
        &past_rq_init[index * M],
        st->past_r_q,
        M);
}
