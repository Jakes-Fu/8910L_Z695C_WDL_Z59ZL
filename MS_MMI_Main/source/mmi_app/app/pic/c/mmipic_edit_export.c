/*****************************************************************************
** File Name:      mmipic_edit_export.c                                                  *
** Author:                                                                   *
** Date:           01/10/2008                                                *
** Copyright:      2008 Spreadtrum, Incorporated. All Rights Reserved.       *
** Description:    This file is used to describe pic editor                  *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 01/2008       Vincent.shang       Create                                  *
******************************************************************************/


/**--------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/
#ifdef WIN32
#include "std_header.h"
#endif

#include "sci_types.h"
//#include "mmipic_edit_export.h"
//macro replace
//#ifdef MMI_PIC_EDITOR_ENABLE
#ifdef PIC_EDITOR_SUPPORT
#include "mmipic_edit.h"
//enter piceditor disable 3D;exit piceditor enable 3D
#include "mmi_default.h"

/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
**                         LOCAL DEFINITION                                 *
**--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          TYPE AND CONSTANT                                */
/*---------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
**                         EXTERNAL DECLARE                                 *
**--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
**                         GLOBAL DEFINITION                                *
**--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : exit from pic editor
//  Global resource dependence : none
//  Author: vincent.shang
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIPIC_ExitFromPicEditor(void)
{
    MMIPIC_ExitFromPicEditor();
}
/*****************************************************************************/
//  Description : init pic module
//  Global resource dependence : 
//  Author: vincent
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIPIC_InitModule(void)
{
    MMIPIC_InitModule();
}
/*****************************************************************************/
//  Description : open pic editor 
//  Global resource dependence : 
//  Author: vincent
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIPIC_OpenPicEditorMainWin(
                                              const wchar    *full_path_ptr,     //in
                                              uint16         full_path_len      //in:×Ö½ÚÊý
                                              )
{
//enter piceditor disable 3D;exit piceditor enable 3D
    //MMI_Enable3DMMI(FALSE);
#ifdef UI_P3D_SUPPORT
    MMI_Disable3DEffect(MMI_3D_EFFECT_PIC_EDITOR);
#endif
    return (MMIPIC_OpenPicEditorMainWin(full_path_ptr,full_path_len));
}
//lijinghua add it for pic crop
PUBLIC void MMIAPIPICCROP_OnlySaveOrgImgToUndoBuf(void)
{
//zhuyongjing change
	//MMIPICCROP_OnlySaveOrgImgToUndoBuf();
        MMIPIC_SaveToUndoBuf();
//change end
}
PUBLIC void MMIAPIPICCROP_CopyOutputImgDataToOrgImgData(void)
{
	MMIPICCROP_CopyOutputImgDataToOrgImgData();
}
PUBLIC void MMIAPIPICCROP_CopyOutputRectToOrgRect(void)
{
	MMIPICCROP_CopyOutputRectToOrgRect();
}
PUBLIC void MMIAPIPICCROP_RecordChangedTimes(void)
{
	MMIPICCROP_RecordChangedTimes();
}
//lijinghua end it
#else

PUBLIC void MMIAPIPIC_ExitFromPicEditor(void)
{
    
}

PUBLIC void MMIAPIPIC_InitModule(void)
{
    
}

PUBLIC BOOLEAN MMIAPIPIC_OpenPicEditorMainWin(uint16 *full_path_ptr, uint16 full_path_len)
{
    return TRUE;
}

PUBLIC void MMIAPIPICCROP_OnlySaveOrgImgToUndoBuf(void)
{

}

PUBLIC void MMIAPIPICCROP_CopyOutputImgDataToOrgImgData(void)
{

}

PUBLIC void MMIAPIPICCROP_CopyOutputRectToOrgRect(void)
{

}

PUBLIC void MMIAPIPICCROP_RecordChangedTimes(void)
{

}
#endif
