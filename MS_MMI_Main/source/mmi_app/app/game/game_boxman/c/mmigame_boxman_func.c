
/****************************************************************************
** File Name:      mmigame_boxman_func.c                            
** Author:             Annie.an                                                     
** Date:            2006/04/01
** Copyright:      2004 Spreadtrum, Incorporated. All Rights Reserved.       
** Description:    This file is used to create windows of game_boxman
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE               NAME             DESCRIPTION                             
** 04/01/2006     annie.an          Create
** 
****************************************************************************/
#define _MMIGAME_BOXMAN_FUNC_C_  

/**--------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/
#ifdef WIN32
#include "std_header.h"
#endif
#ifdef GAME_BOXMAN_SUPPORT
#include "window_parse.h"
//#include "mmigame.h"
#include "mmigame_internal.h"
#include "mmigame_boxman_internal.h"
#include "mmigame_boxman_nv.h"
#include "guilistbox.h"
#include "guimsgbox.h"
//#include "mmiset.h"
#include "mmiset_export.h"
//#include "mmiacc_winmain.h"
#include "mmidisplay_data.h" 
#include "mmigame_link_id.h"
//#include "mmigame_link_image.h"
#include "guilcd.h"
#include "mmi_nv.h"
#include "mmigame_boxman_menutable.h"


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
//   FUNCTION:     MMIGAME_AddTwoString
//  Description :   add two string 
//
//  Global resource dependence : 
//   Author:      
//  Note:   
/*****************************************************************************/
PUBLIC MMI_STRING_T MMIGMBOXMAN_AddTwoString(
                                             MMI_STRING_T first, 
                                             MMI_STRING_T second
                                             )
{
    MMI_STRING_T result = {0};
    uint16 j = 0;
//    uint16 i = 0, j = 0;
    
//     result.is_ucs2 = first.is_ucs2 || second.is_ucs2;
//     if (result.is_ucs2 && !first.is_ucs2) result.length = first.length *2;
//     else result.length = first.length;
//     if (result.is_ucs2 && !second.is_ucs2) result.length += second.length *2;
//     else result.length += second.length;

    result.wstr_len = first.wstr_len + second.wstr_len;
    
    result.wstr_ptr = (wchar*)SCI_ALLOC_APP( (result.wstr_len + 1) * sizeof(wchar) );

    if (PNULL == result.wstr_ptr)
    {
        result.wstr_len = 0;
        return result;
    }
    
    // add first string    
//     if (result.is_ucs2 && !first.is_ucs2 )
//     {
//         for(i =0,j=0; i<first.length; i++)          // change to ucs2
//         {
//             result.str_ptr[j++] = 0;                    // first byte is 0
//             result.str_ptr[j++] = first.str_ptr[i];
//         }       // end of for
//     }else
//     {
//         MMI_MEMCPY( result.str_ptr, result.length, first.str_ptr, first.length, first.length);
//         j = first.length;
//     }

    MMI_WSTRNCPY( result.wstr_ptr, result.wstr_len, first.wstr_ptr, first.wstr_len, first.wstr_len );
    j = first.wstr_len;
    
    // add second string
//     if (result.is_ucs2 && !second.is_ucs2 )
//     {
//         for(i =0; i<second.length; i++)          // change to ucs2
//         {
//             result.str_ptr[j++] = 0;                    // first byte is 0
//             result.str_ptr[j++] = second.str_ptr[i];
//         }       // end of for
//     }else 
//     {
//         MMI_MEMCPY( result.str_ptr+j, result.length-j, second.str_ptr, second.length, second.length);
//     }

    MMI_WSTRNCPY( result.wstr_ptr+j, result.wstr_len-j, second.wstr_ptr, second.wstr_len, second.wstr_len );

    return result;
    
} 
/*****************************************************************************/
//   FUNCTION:      MMIGMBOXMAN_Init
//  Description :   
//
//  Global resource dependence : 
//   Author:        taul.zhu
//  Note:   
/*****************************************************************************/
PUBLIC  BOOLEAN  MMIGMBOXMAN_Init(void)
{
    uint16 nv_return = 0;
    uint16 high_score[3];
    uint8 lever[6];
    
    MMINV_READ(MMINV_GMBOXMAN_LEVER_SET, (uint16*)lever, nv_return);
    if (MN_RETURN_SUCCESS != nv_return)
    {
        SCI_MEMSET(&lever[0], 0, 6);
        MMINV_WRITE(MMINV_GMBOXMAN_LEVER_SET, &lever[0]);
    }
    
    
    MMINV_READ(MMINV_GMBOXMAN_HIGH_SCORE, ( int16*)high_score, nv_return);   
    if (MN_RETURN_SUCCESS != nv_return)
    {
        SCI_MEMSET(&high_score[0], 0, 6);
        MMINV_WRITE(MMINV_GMBOXMAN_HIGH_SCORE, &high_score[0]);
    }
    
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
PUBLIC void MMIGMBOXMAN_InitModule(void)
{
    MMIGMBOXMAN_RegMenuGroup();  //初始化im menu
    MMIGMBOXMAN_RegNv();   //初始化im nv
    MMIGAME_BOXMAN_RegWinIdNameArr();
#ifdef MMI_TASK_MANAGER
    MMIGMBOX_RegAppletInfo();
#endif
}
#endif


