/**********************************************************************

  inits.c -

  $Author: dave $
  $Date: 2003/12/19 03:58:57 $
  created at: Tue Dec 28 16:01:58 JST 1993

  Copyright (C) 1993-2003 Yukihiro Matsumoto

**********************************************************************/

#include "ruby.h"

void Init_Array _((void));
void Init_Bignum _((void));
void Init_Binding _((void));
void Init_Comparable _((void));
void Init_Dir _((void));
void Init_Enumerable _((void));
void Init_Exception _((void));
void Init_syserr _((void));
void Init_eval _((void));
void Init_load _((void));
void Init_Proc _((void));
void Init_Thread _((void));
void Init_File _((void));
void Init_GC _((void));
void Init_Hash _((void));
void Init_IO _((void));
void Init_Math _((void));
void Init_marshal _((void));
void Init_Numeric _((void));
void Init_Object _((void));
void Init_pack _((void));
void Init_Precision _((void));
void Init_sym _((void));
void Init_process _((void));
void Init_Random _((void));
void Init_Range _((void));
void Init_Regexp _((void));
void Init_signal _((void));
void Init_String _((void));
void Init_Struct _((void));
void Init_Time _((void));
void Init_var_tables _((void));
void Init_version _((void));

void Init_MoSync _((void));

void
rb_call_inits()
{
    MOSYNC_TIME(Init_sym());
    MOSYNC_TIME(Init_var_tables());
    MOSYNC_TIME(Init_Object());
    MOSYNC_TIME(Init_Comparable());
    MOSYNC_TIME(Init_Enumerable());
    MOSYNC_TIME(Init_Precision());
    MOSYNC_TIME(Init_eval());
    MOSYNC_TIME(Init_String());
    MOSYNC_TIME(Init_Exception());
    //MOSYNC_TIME(Init_Thread());
    MOSYNC_TIME(Init_Numeric());
    //MOSYNC_TIME(Init_Bignum());
    //MOSYNC_TIME(Init_syserr());
    MOSYNC_TIME(Init_Array());
    //MOSYNC_TIME(Init_Hash());
    MOSYNC_TIME(Init_Struct());
    //MOSYNC_TIME(Init_Regexp());
    //MOSYNC_TIME(Init_pack());
    MOSYNC_TIME(Init_Range());
    MOSYNC_TIME(Init_IO());
    //MOSYNC_TIME(Init_Dir());
    //MOSYNC_TIME(Init_Time());
    MOSYNC_TIME(Init_Random());
    //MOSYNC_TIME(Init_signal());
    //MOSYNC_TIME(Init_process());
    MOSYNC_TIME(Init_load());
    //MOSYNC_TIME(Init_Proc());
    //MOSYNC_TIME(Init_Binding());
    MOSYNC_TIME(Init_Math());
    MOSYNC_TIME(Init_GC());
    //MOSYNC_TIME(Init_marshal());
    MOSYNC_TIME(Init_version());
    
    //Initialize MoSync extensions
    MOSYNC_TIME(Init_MoSync());
	//printf("rb_call_inits done");
}
