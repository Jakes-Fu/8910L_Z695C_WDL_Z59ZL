
/****************************************************************************
** File Name:      mmigame_bubblebobble_func.c                            
** Author:          junxue.zhu                                                     
** Date:             01/06/2012  
** Copyright:      2004 Spreadtrum, Incorporated. All Rights Reserved.       
** Description:    This file is used to create windows of game_bubblebobble
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE               NAME             DESCRIPTION                             
** 01/06/2012    junxue.zhu           Create
****************************************************************************/
#define _MMIGAME_BUBBLEBOBBLE_FUNC_C_  

/**--------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/
#ifdef WIN32
#include "std_header.h"
#endif
#ifdef GAME_BUBBLEBOBBLE_SUPPORT
#include "window_parse.h"
#include "mmigame_internal.h"
#include "mmigame_bubblebobble_internal.h"
#include "mmigame_bubblebobble_nv.h"
#include "guilistbox.h"
#include "guimsgbox.h"
#include "mmiset_export.h"
#include "mmidisplay_data.h" 
#include "mmigame_link_id.h"
#include "guilcd.h"
#include "mmi_nv.h"
#include "mmigame_bubblebobble_menutable.h"


/**--------------------------------------------------------------------------*
**                         FUNCTION DECLARE                                                                           *
**--------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
**                       LOCAL DATA DECLARATION
**---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
**                       function bodies  -- local
**---------------------------------------------------------------------------*/
/*****************************************************************************/
//  FUNCTION:     MMIGMBUBBLEBOBBLE_AddTwoString
//  Description :   add two string 
//  Global resource dependence : 
//  Author:    junxue.zhu  
//  Note:   
/*****************************************************************************/
PUBLIC MMI_STRING_T MMIGMBUBBLEBOBBLE_AddTwoString(
                                             MMI_STRING_T first, 
                                             MMI_STRING_T second
                                             )
{
    MMI_STRING_T result = {0};
    uint16 j = 0;

    result.wstr_len = first.wstr_len + second.wstr_len;    
    result.wstr_ptr = (wchar*)SCI_ALLOC_APP( (result.wstr_len + 1) * sizeof(wchar) );

    if (PNULL == result.wstr_ptr)
    {
        result.wstr_len = 0;
        return result;
    }
    
    MMI_WSTRNCPY( result.wstr_ptr, result.wstr_len, first.wstr_ptr, first.wstr_len, first.wstr_len );

    j = first.wstr_len;
    
    MMI_WSTRNCPY( result.wstr_ptr+j, result.wstr_len-j, second.wstr_ptr, second.wstr_len, second.wstr_len );

    return result;
    
} 

/*****************************************************************************/
//  FUNCTION:      MMIGMBUBBLEBOBBLE_Init
//  Description :   
//  Global resource dependence : 
//  Author:       junxue.zhu
//  Note:   
/*****************************************************************************/
PUBLIC  BOOLEAN  MMIGMBUBBLEBOBBLE_Init(void)
{
    uint16 nv_return = 0;
    uint16 high_score[3];
    uint8 lever[6];
    
    return TRUE;    
}

/*****************************************************************************/
//   FUNCTION:      MMIGMQUT_InitModule
//  Description :   
//
//  Global resource dependence : 
//   Author:        taul.zhu
//  Note:   
/*****************************************************************************/
PUBLIC void MMIGMBUBBLEBOBBLE_InitModule(void)
{
    MMIGMBUBBLEBOBBLE_RegMenuGroup();  //初始化im menu
    MMIGMBUBBLEBOBBLE_RegNv();   //初始化im nv
    MMIGAME_BUBBLEBOBBLE_RegWinIdNameArr();
}

/*****************************************************************************/
//   FUNCTION:     MMIGMBUBBLE_AddTwoString
//  Description :    链接两个字串为一个
//
//  Global resource dependence : 
//   Author:      
//  Note:   
/*****************************************************************************/
PUBLIC MMI_STRING_T MMIGMBUBBLE_AddTwoString(
                                          MMI_STRING_T first, 
                                          MMI_STRING_T second
                                          )
{
    MMI_STRING_T result = {0};
    uint16 j = 0;
    result.wstr_len = first.wstr_len + second.wstr_len;
    result.wstr_ptr = (wchar*)SCI_ALLOC_APP( (result.wstr_len + 1) * sizeof(wchar) );

    if (PNULL == result.wstr_ptr)
    {
        result.wstr_len = 0;
        return result;
    }
    
    MMI_WSTRNCPY( result.wstr_ptr, result.wstr_len, first.wstr_ptr, first.wstr_len, first.wstr_len );
    j = first.wstr_len;   
    MMI_WSTRNCPY( result.wstr_ptr+j, result.wstr_len-j, second.wstr_ptr, second.wstr_len, second.wstr_len );
    return result;
    
} 
#endif


