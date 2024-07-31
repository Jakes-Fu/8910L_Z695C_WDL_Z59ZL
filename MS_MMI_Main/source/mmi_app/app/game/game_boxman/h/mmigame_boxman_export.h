/*****************************************************************************
** File Name:      mmigame_boxman_export.h                                                   *
** Author:                                                                   *
** Date:           14/02/2012                                               *
** Copyright:      2003 Spreadtrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to describe the internal api of game              *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 04/03/2006    Annie.an            Create
** 14/02/2012    xiyuan.ma           Edit
******************************************************************************/

#ifndef _MMIGAME_BOXMAN_EXPORT_H_
#define _MMIGAME_BOXMAN_EXPORT_H_
#ifdef GAME_BOXMAN_SUPPORT
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
        
#define MMIGMBOXMAN_OpenGameWin   MMIAPIGMBOXMAN_OpenGameWin

#endif //MMI_FUNC_COMPATIBLE_SUPPORT
       
/*****************************************************************************/
//   FUNCTION:     MMIGMQUT_OpenGameWin
//  Description :   
//
//  Global resource dependence : 
//   Author:      
//  Note:   
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIGMBOXMAN_OpenGameWin(void);

#ifdef MMI_TASK_MANAGER
/*****************************************************************************/
//  Description : Stop game box
//  Global resource dependence : 
//  Author:fen.xie
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIGMBOX_Stop(BOOLEAN is_minimize);

#endif

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif
#endif
#endif  

