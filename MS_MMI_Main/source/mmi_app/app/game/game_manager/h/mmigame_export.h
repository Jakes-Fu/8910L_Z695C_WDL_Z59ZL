/*****************************************************************************
** File Name:      mmigame_export.h                                                  *
** Author:                                                                   *
** Date:           04/03/2006                                               *
** Copyright:      2003 Spreadtrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to describe csk2 text input               *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 04/2006      annie.an         Create
******************************************************************************/

#ifndef _MMIGAME_EXPORT_H_
#define _MMIGAME_EXPORT_H_
#ifdef GAME_SUPPORT
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

#define MMIGAME_OpenMainMenuWindow   MMIAPIGAME_OpenMainMenuWindow

#endif //MMI_FUNC_COMPATIBLE_SUPPORT

/*****************************************************************************/
// 	Description :  Go to the game main menu
//	Global resource dependence : 
//  Author:ycd
//	Note: 
/*****************************************************************************/
PUBLIC void MMIAPIGAME_OpenMainMenuWindow(void);

/*****************************************************************************/
//  Description :  Reset the game data
//  Global resource dependence : 
//  Author:Annie
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIGAME_RsetGameData(void);

/*****************************************************************************/
//   FUNCTION:     MMIGAME_Init
// 	Description :   Init NV of GAME
//
//	Global resource dependence : 
//   Author:      
//	Note:   
/*****************************************************************************/
PUBLIC BOOLEAN  MMIAPIGAME_InitSetting(void);

/*****************************************************************************/
// 	Description :  MMIAPIGAME_InitModule
//	Global resource dependence : 
//  Author:ycd
//	Note: 
/*****************************************************************************/
PUBLIC void MMIAPIGAME_InitModule(void);

/*****************************************************************************/
//   FUNCTION:     MMIAPIGAME_ClearScreenColor
// 	Description :   
//
//	Global resource dependence : 
//   Author:      
//	Note:   
/*****************************************************************************/
PUBLIC void MMIAPIGAME_ClearScreenColor(uint16 color);

#ifdef   __cplusplus
    }
#endif

#endif

#endif
