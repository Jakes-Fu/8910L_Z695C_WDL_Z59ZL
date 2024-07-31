/*****************************************************************************
** File Name:      mmigame_link_export.h                                                   *
** Author:                                                                   *
** Date:           04/03/2006                                               *
** Copyright:      2003 Spreadtrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to describe the internal api of game              *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 04/03/2006      Annie.an      Create
******************************************************************************/

#ifndef _MMIGAME_LINK_EXPORT_H_
#define _MMIGAME_LINK_EXPORT_H_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#ifdef GAME_LINK_SUPPORT
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

#define MMIGMLINK_OpenGameWin   MMIAPIGMLINK_OpenGameWin

#endif //MMI_FUNC_COMPATIBLE_SUPPORT

/*****************************************************************************/
//  Description : MMIGMQUT_OpenGameWin
//       
//  Global resource dependence : 
//   Author:        taul.zhu
//   RETRUN:   
//  Note:   
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIGMLINK_OpenGameWin(void);

#ifdef MMI_TASK_MANAGER
 /*****************************************************************************/
//  Description : Stop game link
//  Global resource dependence : 
//  Author:fen.xie
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIGMLink_Stop(BOOLEAN is_minimize);
#endif

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif
 
#endif
#endif  

