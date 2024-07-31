/*****************************************************************************
** File Name:      toneClient.h                                             *
** Author:                                                                   *
** Date:           08/07/2006                                                *
** Copyright:      2003 Spreadtrum, Incoporated. All Rights Reserved.        *
** Description:    This file is used to describe mmi file manage             *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               */


#ifndef __TONECONTROL_H__
#define __TONECONTROL_H__
#ifdef JAVA_SUPPORT_IA
/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "jkmediacontrol.h"
#include "sci_types.h"
#include "amtone.h"
/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
typedef struct tagToneParser
{
    JKUint8* u8Data;
    JKUint32 s32DataLen;
    AmToneSeqHandle   hToneSeqHandle;
    JKUint32 duration;
}ToneParser;
/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/

PUBLIC JKSint32 toneToMidi(uint8*pData,int length,ToneParser* tp);
/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif
#endif /* __TONECONTROL_H__ */


