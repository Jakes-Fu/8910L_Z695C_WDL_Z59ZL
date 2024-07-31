#ifdef KURO_SUPPORT
/*****************************************************************************
** File Name:      mmikur_lyric_internal.h                                   *
** Author:                                                                   *
** Date:           28/08/2007                                               *
** Description:    This file is used to mmikur_internal func                 *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 08/2007       cmzs               Create
******************************************************************************/

#ifndef _MMIKUR_LYRIC_INTERNAL_H_
#define _MMIKUR_LYRIC_INTERNAL_H_ 

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
//#include "mmiset.h"
#include "mmiset_export.h"

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif
/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
#define MMIMP3_LYRIC_SET_MAX            2
#define MMIMP3_LYRIC_SENTENCE_MAX_LEN   200

/**--------------------------------------------------------------------------*
 **                         TYPE AND CONSTANT                                *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : initialize KUR lyric
//  Global resource dependence : none
//  Author: CMZS
//  Note:
/*****************************************************************************/
PUBLIC void MMIKUR_InitLyric(void);

/*****************************************************************************/
//  Description : stop lyric
//  Global resource dependence : 
//  Author: cmzs
//  Note: 
/*****************************************************************************/
PUBLIC void MMIKUR_StopLyric(void);

/*****************************************************************************/
//  Description : get the kuru picture data by the index of picture.
//  Global resource dependence : 
//  Author:cmzs
//  Note:
/*****************************************************************************/
PUBLIC void MMIKUR_AllocKlrBuffer(void);
/*****************************************************************************/
//  Description : Free buffer for picture display
//  Global resource dependence : 
//  Author:cmzs
//  Note: 
/*****************************************************************************/
PUBLIC void MMIKUR_FreeKlrBuffer(void);

/*****************************************************************************/
//  Description : ����ͼƬ��ȡ��������
//  Global resource dependence : 
//  Author: xingdong.li
//  Note:
//  Return: 
/*****************************************************************************/
PUBLIC void MMIKUR_HandleReadPicMsg(MMI_WIN_ID_T win_id);
/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif
#endif

