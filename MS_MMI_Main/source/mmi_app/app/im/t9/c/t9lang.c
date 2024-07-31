/*******************************************************************************
;*******************************************************************************
;**                                                                           **
;**                    COPYRIGHT 2011 TEGIC COMMUNICATIONS                    **
;**                                                                           **
;**                TEGIC COMMUNICATIONS PROPRIETARY INFORMATION               **
;**                                                                           **
;**     This software is supplied under the terms of a license agreement      **
;**     or non-disclosure agreement with Tegic Communications and may not     **
;**     be copied or disclosed except in accordance with the terms of that    **
;**     agreement.                                                            **
;**                                                                           **
;*******************************************************************************
;**                                                                           **
;**     FileName: t9lang.c                                                    **
;**                                                                           **
;**  Description: File to include T9 language database files and access       **
;**               function                                                    **
;**                                                                           **
;*******************************************************************************
;******* 10 ****** 20 ****** 30 ****** 40 ****** 50 ****** 60 ****** 70 *******/

/******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 08/15/2011     haiwu.chen         edit                                    *
注意在这里完成T9多语的集成动作，把生成的东西集成于此
t9gen.c t9lang.c
******************************************************************************/


#include "t9.h"

#include "t9gen.c"       /* pull in main access function module */


//t9 alphabetic serial  pls add ldb in order
/*-----------------------------------< eof >----------------------------------*/


//t9chinese serial
#if 0
#if defined(IM_SIMP_CHINESE_SUPPORT) || defined(IM_CHINESE_S_SUPPORT)
#include "t9cgen.c"
#include "l0204b00.c"     /* simplified chinese  */
#include "l0204b01.c"
#include "l0204b02.c"
#include "l0204b08.c"
#include "l0204b09.c"
#include "l0204b10.c"
#include "l0204b11.c"
#include "l0204b17.c"
#include "l0204b18.c"
#include "l0204b19.c"
#include "l0204b25.c"
#include "l0204b26.c"
#include "l0204b27.c"
#include "l0204b37.c"
#include "l0204b38.c"
#include "l0204b41.c"
#include "l0204b42.c"
#include "l0204b43.c"
#include "l0204b46.c"
#include "l0204b47.c"
#include "l0204b48.c"
#include "l0204b49.c"
#include "l0204b50.c"
#include "l0204b51.c"
#include "l0204b52.c"
#include "l0204b59.c"
#include "l0204b60.c"
#include "l0204b61.c"
#include "l0204b64.c"
#endif
/*-----------------------------------< eof >----------------------------------*/
#if defined (IM_CHINESE_T_SUPPORT) || defined (IM_TRAD_CHINESE_SUPPORT)
#include "t9cgen.c"


#include "l0104b00.c"     /* traditional chinese  */
#include "l0104b01.c"
#include "l0104b08.c"
#include "l0104b09.c"
#include "l0104b10.c"
#include "l0104b17.c"
#include "l0104b18.c"
#include "l0104b25.c"
#include "l0104b26.c"
#include "l0104b27.c"
#include "l0104b37.c"
#include "l0104b41.c"
#include "l0104b42.c"
#include "l0104b46.c"
#include "l0104b47.c"
#include "l0104b48.c"
#include "l0104b49.c"
#include "l0104b59.c"
#include "l0104b60.c"
#include "l0104b64.c"
/*-----------------------------------< eof >----------------------------------*/
#endif

#endif
