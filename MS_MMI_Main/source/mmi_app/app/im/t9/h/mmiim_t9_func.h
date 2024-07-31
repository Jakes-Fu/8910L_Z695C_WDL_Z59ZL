/*****************************************************************************
** File Name:      mmiim_t9_func.h                                           *
** Author:         haiwu.chen                                                *
** Date:           24/04/2012                                                *
** Copyright:      2007 Spreadtrum, Incorporated. All Rights Reserved.       *
** Description:                                                              *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 04/2012        haiwu.chen            Create                               *
******************************************************************************/
#ifdef IM_ENGINE_T9

#ifndef _MMIIM_T9_FUNC_H_
#define _MMIIM_T9_FUNC_H_


#if defined(__cplusplus)
extern "C" {
#endif

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "t9.h"
#include "t9api.h"
#include "mmiim_t9_base.h"
#include "mmiim_t9.h"
#ifdef T9CHINESE
#include "mmiim_t9_chs.h"
#endif

/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------*/
/*                         TYPE AND CONSTANT                                 */
/*---------------------------------------------------------------------------*/


/**--------------------------------------------------------------------------*
 **                         GLOBAL DECLARE                                   *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : init ldb lang data info
//  Global resource dependence : none
//  Author:haiwu.chen
//  Note: 读取输入法的LDB info
/*****************************************************************************/
PUBLIC BOOLEAN MMIIM_T9_InitLDBInfo(MMIIM_T9_DATA_T *data_ptr);

/*****************************************************************************/
//  Description : This function is used to init t9 by lang
//  lobal resource dependence : none
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
PUBLIC T9STATUS MMIIM_T9_Init_Alpha(MMIIM_T9_DATA_T *data_ptr);

/*****************************************************************************/
//  Description : This function is used to terminate t9 by data_ptr
//  lobal resource dependence : none
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
PUBLIC T9STATUS MMIIM_T9_Ter_Alpha(MMIIM_T9_DATA_T *data_ptr);

/*****************************************************************************/
//  Description : set input mode type, smart or multitap mode
//  Global resource dependence : none
//  Author: haiwu.chen
//  Note: 处理是否multitap or smart 模式
/*****************************************************************************/
PUBLIC T9STATUS T9FARCALL MMIIM_T9_SetMode(MMIIM_T9_DATA_T *data_ptr);

/*****************************************************************************/
//  Description : set input capital type
//  Global resource dependence : none
//  Author: haiwu.chen
//  Note: 处理是否capital or not 模式
/*****************************************************************************/
PUBLIC T9STATUS T9FARCALL MMIIM_T9_SetCapital(MMIIM_T9_DATA_T *data_ptr);

/*****************************************************************************/
//  Description : handle t9 reset text buf
//  Global resource dependence : none
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
PUBLIC T9STATUS T9FARCALL MMIIM_T9_ResetTextBuf(T9FieldInfo *pFieldInfo);

/*****************************************************************************/
//  Description : handle t9 add text string
//  Global resource dependence : none
//  Author: haiwu.chen
//  Note: 
/*****************************************************************************/
PUBLIC T9STATUS T9FARCALL MMIIM_T9_AddTextString(
    T9FieldInfo *pFieldInfo,
    const T9SYMB *psBuf,
    T9UINT nLen
);

/*****************************************************************************/
//  Description : get t9 client language by engine language
//  Global resource dependence : none
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
PUBLIC uint16 MMIIM_T9_GetEngineLangByClientLang(GUIIM_LANGUAGE_T client_lang);

/*****************************************************************************/
//  Description : set language database 
//  Global resource dependence : none
//  Author:  haiwu.chen
//  Note:
/*****************************************************************************/
PUBLIC T9STATUS MMIIM_T9_SetT9Language(MMIIM_T9_DATA_T *data_ptr);

/*****************************************************************************/
//  Description : is it this language chinese language
//  Global resource dependence : none
//  Author: haiwu.chen
//  Note: chinese contain: simple chinese and traditional chinese
/*****************************************************************************/
PUBLIC BOOLEAN MMIIM_T9_IsChinese(GUIIM_LANGUAGE_T lang);

/*****************************************************************************/
//  Description : is it this language alphabetic language
//  Global resource dependence : none
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIIM_T9_IsAlphabetic(GUIIM_LANGUAGE_T lang);

/*****************************************************************************/
//  Description : handle t9 key function
//  Global resource dependence : none
//  Author: haiwu.chen
//  Note: 处理按键消息
/*****************************************************************************/
PUBLIC T9STATUS T9FARCALL MMIIM_T9_HandleKey(T9FieldInfo *pFieldInfo, T9KEY eKeyID);

/*****************************************************************************/
//  Description : MMIIM_T9_AWActivateWordLeft
//  Global resource dependence : none
//  Author: haiwu.chen
//  Note: active a left-word when user delete chars
/*****************************************************************************/
PUBLIC T9STATUS T9FARCALL MMIIM_T9_AWActivateWordLeft(MMIIM_T9_DATA_T *data_ptr);

/*****************************************************************************/
//  Description : handle t9 add explicit char
//  Global resource dependence : none
//  Author: haiwu.chen
//  Note: key_index, MMIIM_KEY_1:0, MMIIM_KEY_2:1
//  input_sym用于26键直接输入的按键字符; 
/*****************************************************************************/
PUBLIC T9STATUS T9FARCALL MMIIM_T9_AddExplicitChar(
    T9FieldInfo *pFieldInfo,
    T9SYMB input_sym
);

/*****************************************************************************/
//  Description : handle t9 reset text buf
//  Global resource dependence : none
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
PUBLIC T9STATUS T9FARCALL MMIIM_T9_Cursor(
    T9FieldInfo *pFieldInfo,
    T9CA eType, 
    T9UINT nCount
);

/*****************************************************************************/
//  Description : handle t9 reset text buf
//  Global resource dependence : none
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
PUBLIC T9STATUS T9FARCALL MMIIM_T9_TimeOut(
    T9FieldInfo *pFieldInfo,
    T9MINOR mTimerID
);

/*****************************************************************************/
//  Description : handle t9 add udb custom word
//  Global resource dependence : none
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
PUBLIC T9STATUS T9FARCALL MMIIM_T9_AddUdbCustomWord(
    MMIIM_T9_DATA_T *data_ptr,
    T9SYMB *psBuf, 
    T9MINOR mBufLen
);

#ifdef T9CHINESE
/*****************************************************************************/
//  Description : This function is used to init t9 chinese by data_ptr
//  lobal resource dependence : none
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
PUBLIC T9STATUS MMIIM_T9_Init_Chs(MMIIM_T9_CHS_DATA_T *data_ptr);

/*****************************************************************************/
//  Description : This function is used to terminate t9 chinese by data_ptr
//  lobal resource dependence : none
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
PUBLIC T9STATUS MMIIM_T9_Ter_Chs(MMIIM_T9_CHS_DATA_T *data_ptr);

/*****************************************************************************/
//  Description : set input mode type, smart or multitap mode
//  Global resource dependence : none
//  Author: haiwu.chen
//  Note: smart and stroke mode
/*****************************************************************************/
PUBLIC T9STATUS T9FARCALL MMIIM_T9_SetMode_Chs(MMIIM_T9_CHS_DATA_T *data_ptr);

/*****************************************************************************/
//  Description : handle t9 specify phonetic spelling
//  Global resource dependence : none
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
PUBLIC T9STATUS T9FARCALL MMIIM_T9_SpecifySpell(
    MMIIM_T9_CHS_DATA_T *data_ptr, 
    T9UINT nValue
);
/*****************************************************************************/
//  Description : init chinese ldb lang data info
//  Global resource dependence : none
//  Author:
//  Note: 读取中文输入法的LDB info
/*****************************************************************************/
PUBLIC BOOLEAN MMIIM_T9_InitCHSLDBInfo(MMIIM_T9_CHS_DATA_T *data_ptr);
/*****************************************************************************/
//  Description : get chinese core data, and set to adapt bar rect
//  Global resource dependence : none
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
PUBLIC void MMIIM_T9_GetChsCoreData(MMIIM_T9_CHS_DATA_T *data_ptr);



#endif



/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/


#endif //#ifndef _MMIIM_T9_FUNC_H_

#endif //#ifdef IM_ENGINE_T9