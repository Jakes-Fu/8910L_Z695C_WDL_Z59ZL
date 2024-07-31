/*****************************************************************************
** File Name:      mmigame_bubblebobble_export.h                                                   *
** Author:                                                                   *
** Date:           01/06/2012                                               *
** Copyright:      2003 Spreadtrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to describe the export api of game              *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 01/06/2012    junxue.zhu           Create
******************************************************************************/

#ifndef _MMIGAME_BUBBLEBOBBLE_EXPORT_H_
#define _MMIGAME_BUBBLEBOBBLE_EXPORT_H_
#ifdef GAME_BUBBLEBOBBLE_SUPPORT
/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/


/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif

/**---------------------------------------------------------------------------*
** Macro Declaration *
**---------------------------------------------------------------------------*/
/*****************************************************************************/
// 如下宏定义用以实现对外接口更名前后的兼容
/*****************************************************************************/
#ifdef MMI_FUNC_COMPATIBLE_SUPPORT
        
#define MMIGMBUBBLEBOBBLE_OpenGameWin   MMIAPIGMBUBBLEBOBBLE_OpenGameWin

#endif //MMI_FUNC_COMPATIBLE_SUPPORT
       
/*****************************************************************************/
//  FUNCTION:     MMIAPIGMBUBBLEBOBBLE_OpenGameWin
//  Description :   open bubble win
//  Global resource dependence : 
//  Author:       junxue.zhu
//  Note:   
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIGMBUBBLEBOBBLE_OpenGameWin(void);

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif
#endif
#endif  

