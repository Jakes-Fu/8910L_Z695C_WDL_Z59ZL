/****************************************************************************
** File Name:      ctrllistbox.c                                            *
** Author:         xiaoqing.lu                                              *
** Date:           24/07/2012                                               *
** Copyright:      2012 Spreadtrum, Incoporated. All Rights Reserved.       *
** Description:    This file is used to describe the listbox control.       *
*****************************************************************************
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             *
** 24/07/2012     xiaoqing.lu      Create
****************************************************************************/

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmi_gui_trc.h"
#include "mmk_app.h"
#include "mmi_port.h"
#include "ctrllist_export.h"
#include "ctrllistbox.h"
#include "mmk_msg.h"
#include "guilistbox.h"

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/



LOCAL GUILIST_ITEM_T s_item_to_user = {0};



/**---------------------------------------------------------------------------*
**                         Functions                                         *
**---------------------------------------------------------------------------*/


/******************************************************************************
Description : Get the item state
Global resource dependence :
Author: xiaoqing.lu
Note: 分几类情况
1 显示数字序号，采用相应的不带序号的style，设置has num状态
2 check/radio，采用相应的不带select的style
3 split style，设置disable mark/disable select状态
******************************************************************************/
LOCAL BOOLEAN SetItemStyle(
    CTRLLIST_ITEM_T *dest_item_ptr,
    GUIITEM_STYLE_E style_id
)
{
    BOOLEAN is_valid_style = TRUE;

    if (PNULL == dest_item_ptr)
    {
        return FALSE;
    }

    switch (style_id)
    {
    case GUIITEM_STYLE_DEFAULT:
        //do nothing...
        break;
        
    case GUIITEM_STYLE_ONE_LINE_TEXT_SPLIT:
        dest_item_ptr->state |= GUIITEM_STATE_DISABLE_MARK;
        dest_item_ptr->state |= GUIITEM_STATE_SPLIT;
        break;
        
        //number
#ifndef MMI_THEME_LIST_MINI_SUPPORT
        
    case GUIITEM_STYLE_ONE_LINE_NUMBER_TEXT:
    case GUIITEM_STYLE_TWO_LINE_NUMBER_TEXT_AND_TEXT:
    case GUIITEM_STYLE_ONE_TWO_LINE_NUMBER_TEXT_AND_TEXT:
        dest_item_ptr->state |= GUIITEM_STATE_HAS_NUM;
        break;
#endif

//zmt add start
    case GUIITEM_SYTLE_ZMT_PUBLISH_LIST_MS:
    case GUIITEM_SYTLE_ZMT_BOOK_LIST_MS:
    case GUIITEM_SYTLE_ZMT_UNIT_LIST_MS:
    case GUIITEM_SYTLE_ZMT_LISTEN_SET_MS:
    case GUIITEM_STYLE_POETRY_GRADE_LIST_MS:
    case GUIITEM_STYLE_POETRY_ITEM_LIST_MS:
    case GUIITEM_STYLE_FORMULA_ITEM_LIST_MS:
    case GUIITEM_STYLE_CLASS_MAIN_LIST_MS:
    case GUIITEM_STYLE_CLASS_SECTION_LIST_MS:
    case GUIITEM_STYLE_PINYIN_TABLE_LIST_MS:
//zmt add end
        
        //check
    case GUIITEM_STYLE_ONE_LINE_CHECK:
	case GUIITEM_STYLE_MULTI_CHOICE_PICKER_TEXT:
	case GUIITEM_STYLE_MULTI_CHOICE_PICKER_TWO_LINE_TEXT_AND_TEXT:		
    case GUIITEM_STYLE_MULTI_CHOICE_PICKER_TWO_LINE_TEXT_AND_TEXT_2:
    case GUIITEM_STYLE_MULTI_CHOICE_PICKER_TWO_LINE_BIGICON_TEXT_AND_TEXT:
    case GUIITEM_STYLE_ONE_LINE_CHECK_EXT:
    case GUIITEM_STYLE_ONE_LINE_CHECK_ICON:
    //add for single item choice picker style.
    case GUIITEM_STYLE_ONE_LINE_CHECK_ICON_MS:
    //End
    case GUIITEM_STYLE_TWO_LINE_CHECK_ICON:
    case GUIITEM_STYLE_TWO_LINE_CHECK_ANIM_TEXT_AND_TEXT:
    case GUIITEM_STYLE_TWO_LINE_ICON_TWO_TEXT_AND_TEXT_NORMAL:
#ifndef MMI_THEME_LIST_MINI_SUPPORT
    case GUIITEM_STYLE_TWO_LINE_CHECK_ANIM_TEXT:
#endif
    case GUIITEM_STYLE_TWO_LINE_ANIM_AND_TWO_ICON_TEXT_AND_ICON_2:
#ifdef MMI_PDA_SUPPORT
    case GUIITEM_STYLE_TWO_LINE_TWO_ICON_TEXT_AND_ICON:
#endif
        //do nothing
        break;

   case GUIITEM_STYLE_MULTI_CHOICE_PICKER_AVATAR_AND_TEXT:
#ifndef MMIPB_RMVITEM_SUPPORT
        dest_item_ptr->state |= GUIITEM_STATE_HAS_AVATAR_CHECKBOX;
#endif
        break;  
   case GUIITEM_STYLE_MULTI_CHOICE_PICKER_INITIAL_AND_TEXT:
	   dest_item_ptr->state |= GUIITEM_STATE_HAS_INITIAL_CHECKBOX;
	   break;
        //toggle
   case GUIITEM_STYLE_TOGGLE_TWO_LINE_TEXT_TEXT:
   case GUIITEM_STYLE_TOGGLE_ONE_LINE_TEXT:
   //[toggle and quick action list]
    case GUIITEM_STYLE_TOGGLE_TWO_LINE_TEXT_TEXT_MS:
    case GUIITEM_STYLE_TOGGLE_TWO_LINE_TEXT_TEXT_WITH_LEFT_FOCUS_MS:
   //[end]
        dest_item_ptr->state |= GUIITEM_STATE_HAS_TOGGLE_MARK;
        break;
        
        //readio
    case GUIITEM_STYLE_ONE_LINE_RADIO:
    case GUIITEM_STYLE_ONE_LINE_RADIO_EXT:
    case GUIITEM_STYLE_ONE_LINE_RADIO_ICON:
    case GUIITEM_STYLE_TWO_LINE_RADIO_ANIM_TEXT_AND_TEXT:
#ifndef MMI_THEME_LIST_MINI_SUPPORT
    case GUIITEM_STYLE_TWO_LINE_RADIO:
    case GUIITEM_STYLE_TWO_LINE_RADIO_ANIM_TEXT:
#endif
        //do nothing
        break;
        
	case GUIITEM_STYLE_ONE_LINE_ICON_TEXT_WITHOUT_EFFECT_STR:
    case GUIITEM_STYLE_ONE_LINE_TEXT_ICON:
    case GUIITEM_STYLE_ONE_LINE_ICON_TEXT_ICON:
    case GUIITEM_STYLE_ONE_LINE_TEXT:
    case GUIITEM_STYLE_ONE_LINE_ICON_TEXT:
    case GUIITEM_STYLE_ONE_LINE_ICON_TWO_TEXT:
    case GUIITEM_STYLE_ONE_LINE_ICON_TEXT_AND_TEXT:
#ifndef MMI_THEME_LIST_MINI_SUPPORT
    case GUIITEM_STYLE_HALF_LINE_TEXT_ICON:
    case GUIITEM_STYLE_ONE_LINE_ICON_TEXT_ICON_EXT:
    case GUIITEM_STYLE_ONE_LINE_TEXT_DEFAULT_FONT:
    case GUIITEM_STYLE_ONE_LINE_TWO_ICON_TEXT:
#endif
#ifndef MMI_THEME_LIST_MINI_SUPPORT
    case GUIITEM_STYLE_ONE_LINE_TWO_ICON_TWO_TEXT:
    case GUIITEM_STYLE_ONE_LINE_BUTTON_TEXT:
    case GUIITEM_STYLE_ONE_LINE_TWO_BUTTON_TEXT:
#endif
    case GUIITEM_STYLE_ONE_LINE_SMALL_TEXT_AND_TEXT:
#ifndef MMI_THEME_LIST_MINI_SUPPORT
    case GUIITEM_STYLE_ONE_LINE_SMALL_TEXT_TEXT_ICON:
    case GUIITEM_STYLE_ONE_LINE_SMALL_TEXT_TEXT_TWO_ICON:
#endif
    case GUIITEM_STYLE_TWO_LINE_TEXT_AND_TEXT:
	case GUIITEM_STYLE_RADIO_TWO_LINE_TEXT_TEXT:
    case GUIITEM_STYLE_RADIO_ONE_LINE_TEXT:
    case GUIITEM_STYLE_TWO_LINE_TEXT_AND_TEXT_AND_TEXT_AND_TEXT:
    case GUIITEM_STYLE_TWO_LINE_ICON_TEXT_AND_TEXT:
#ifndef MMI_THEME_LIST_MINI_SUPPORT
    case GUIITEM_STYLE_TWO_LINE_TWO_ICON_TEXT_AND_TEXT:
    case GUIITEM_STYLE_TWO_LINE_ICON_TEXT_AND_TWO_TEXT:
    case GUIITEM_STYLE_TWO_LINE_ICON_TEXT_AND_TWO_TEXT_SMALL:
#endif
    case GUIITEM_STYLE_TWO_LINE_TWO_ICON_TEXT_AND_TWO_TEXT:
    case GUIITEM_STYLE_TWO_LINE_ICON_TWO_TEXT_AND_TEXT:
    case GUIITEM_STYLE_TWO_LINE_ICON_TWO_TEXT_AND_TEXT_SMALL:
    case GUIITEM_STYLE_TWO_LINE_TWO_ICON_TWO_TEXT_AND_TEXT:
    case GUIITEM_STYLE_TWO_LINE_ANIM_TEXT_AND_TEXT:
    case GUIITEM_STYLE_TWO_LINE_ANIM_TEXT_AND_TEXT_2:
#ifndef MMI_THEME_LIST_MINI_SUPPORT
    case GUIITEM_STYLE_TWO_LINE_ANIM_TEXT_AND_TEXT_ICON:
    case GUIITEM_STYLE_TWO_LINE_ANIM_TEXT_AND_TEXT_3ICON:
#endif
#ifndef MMI_THEME_LIST_MINI_SUPPORT
    case GUIITEM_STYLE_TWO_LINE_ANIM_2TEXT_AND_ICON:
#endif
    case GUIITEM_STYLE_TWO_LINE_TEXT_AND_TEXT_ANIM:
#ifndef MMI_THEME_LIST_MINI_SUPPORT
    case GUIITEM_STYLE_TWO_LINE_ANIM_TEXT_AND_TEXT_ANIM:
#endif
    case GUIITEM_STYLE_TWO_LINE_TEXT_2ANIM_AND_TEXT:
    case GUIITEM_STYLE_ONE_TWO_LINE_ICON_TEXT_AND_TEXT:
#ifndef MMI_THEME_LIST_MINI_SUPPORT
    case GUIITEM_STYLE_ONE_TWO_LINE_TWO_ICON_TEXT_AND_TEXT:
    case GUIITEM_STYLE_ONE_TWO_LINE_ICON_TEXT_AND_TWO_TEXT:
    case GUIITEM_STYLE_ONE_TWO_LINE_TWO_ICON_TEXT_AND_TWO_TEXT:
    case GUIITEM_STYLE_ONE_TWO_ANIM_TEXT_AND_TEXT_3ICON:
#endif
    case GUIITEM_STYLE_THREE_LINE_TEXT:
#ifndef MMI_THEME_LIST_MINI_SUPPORT
    case GUIITEM_STYLE_THREE_LINE_TEXT_ICON:
#endif
    case GUIITEM_STYLE_TWO_LINE_TEXT_ICON_AND_TWO_TEXT_SMALL:
#ifndef MMI_THEME_LIST_MINI_SUPPORT
    case GUIITEM_STYLE_TWO_LINE_ANIM_TWO_TEXT_AND_TEXT_ICON:
    case GUIITEM_STYLE_TWO_LINE_ANIM_TWO_TEXT_AND_TEXT:
    case GUIITEM_STYLE_TWO_LINE_ANIM_ICON_TEXT_AND_ICON_2TEXT:
#endif
    case GUIITEM_STYLE_TWO_LINE_ANIM_TEXT:
        
#ifndef MMI_THEME_LIST_MINI_SUPPORT
    case GUIITEM_STYLE_TWO_LINE_2TEXT_2ANIM_AND_TEXT:
    case GUIITEM_STYLE_TWO_LINE_ANIM_ICON_TWO_TEXT_AND_ICON_TEXT:
#ifdef MMI_PDA_SUPPORT
    case GUIITEM_STYLE_ONE_LINE_TEXT_TEXT_ICON:
    case GUIITEM_STYLE_ONE_LINE_ICON_TWO_TEXT_ICON:
#endif
    case GUIITEM_STYLE_ONE_LINE_LEFT_SPACE_TEXT:
#endif
    case GUIITEM_STYLE_TWO_LINE_TEXT_ANIM_AND_TEXT:
#ifndef MMI_THEME_LIST_MINI_SUPPORT
#ifdef QBTHEME_SUPPORT
    case GUIITEM_STYLE_THREE_LINE_TEXT_TEXT_ANIM:
    case GUIITEM_STYLE_ONE_LINE_ANIM_TEXT_ICON:
#endif
#endif
#ifndef MMI_THEME_LIST_MINI_SUPPORT
#ifdef SEARCH_SUPPORT
    case GUIITEM_STYLE_TWO_LINE_BIGICON_TEXT:
#endif
    case GUIITEM_STYLE_TWO_LINE_ANIM_TWO_TEXT_AND_TEXT_2:
    case GUIITEM_STYLE_TWO_LINE_ANIM_TWO_TEXT_AND_TEXT_3_SCROLL:
   #if defined(ZDT_PHILIPS_DIAL_SEARCH_STYLE) //   A20210129
    case GUIITEM_STYLE_TWO_LINE_ANIM_TWO_TEXT_AND_TEXT_3_SCROLL_NEW:
   #endif
#endif
    /* for customize style */
    case GUIITEM_STYLE_CUSTOMIZE:
        //do nothing...
        break;
      
    case GUIITEM_STYLE_ONE_LINE_TEXT_MS:
    case GUIITEM_STYLE_ONE_LINE_TEXT_WITH_LEFT_FOCUS_MS:
    case GUIITEM_STYLE_ONE_LINE_TEXT_WITH_RIGHT_FOCUS_MS:
    case GUIITEM_STYLE_ONE_LINE_ICON_TEXT_MS:
    case GUIITEM_STYLE_ONE_LINE_BIGICON_TEXT_MS:
    case GUIITEM_STYLE_ONE_LINE_ICON_TEXT_AND_TEXT_MS:
    case GUIITEM_STYLE_ONE_LINE_TEXT_AND_TEXT_MS:
    case GUIITEM_STYLE_ONE_LINE_TEXT_AND_TEXT_MS_EXT:
//added by du.yuxuan
    case GUIITEM_STYLE_ONE_LINE_2TEXT_MS:
//end
    case GUIITEM_STYLE_ONE_LINE_SUB_TEXT_MS:
    case GUIITEM_STYLE_TWO_LINE_TEXT_AND_TEXT_MS:
    case GUIITEM_STYLE_TWO_LINE_TEXT_AND_TEXT_2_MS:
    //add by James li begin
    case GUIITEM_STYLE_TWO_LINE_TEXT_AND_TEXT_2_MS_FOR_ALARM:
     //add by James li end   
    case GUIITEM_STYLE_TWO_LINE_2TEXT_AND_TEXT_MS:
    case GUIITEM_STYLE_THREE_PARTS_2TEXT_AND_TEXT_MS:
    case GUIITEM_STYLE_TWO_LINE_2TEXT_AND_TEXT_ICON_MS:
    case GUIITEM_STYLE_TWO_LINE_2TEXT_ICON_AND_TEXT_ICON_MS:
    case GUIITEM_STYLE_2TEXT_AND_TEXT_WITH_LEFT_FOCUS_MS:
    //add
    case GUIITEM_STYLE_TWO_LINE_TEXT_AND_2TEXT_WITH_LEFT_FOCUS_MS:
    //End
    case GUIITEM_STYLE_2TEXT_AND_TEXT_WITH_RIGHT_FOCUS_MS:
    case GUIITEM_STYLE_TWO_LINE_ANIM_TEXT_AND_TEXT_MS:
    case GUIITEM_STYLE_TWO_LINE_ICON_TEXT_AND_TEXT_MS:
    case GUIITEM_STYLE_TWO_LINE_ICON_TEXT_AND_TEXT_WITH_LEFT_FOCUS_MS:
    case GUIITEM_STYLE_TWO_LINE_ICON_TEXT_AND_TEXT_WITH_RIGHT_FOCUS_MS:
    case GUIITEM_STYLE_TWO_LINE_TEXT_AND_2TEXT_MS:
    case GUIITEM_STYLE_TWO_LINE_TEXT_AND_2TEXT_MS_EXT:
    case GUIITEM_STYLE_TWO_LINE_BIGICON_TEXT_AND_2TEXT_MS:
    case GUIITEM_STYLE_TWO_LINE_BIGICON_TEXT_AND_TEXT_2_WITH_LEFT_FOCUS_MS:
    case GUIITEM_STYLE_TWO_LINE_BIGICON_TEXT_AND_TEXT_2_WITH_RIGHT_FOCUS_MS:
    //[indicate sim card in call log]
    case GUIITEM_STYLE_TWO_LINE_TEXT_ICON_AND_TEXT_MS:
    //[end]
        //do nothing...
        break;

    //add for pb_avatar_text_item_quick_action
    case GUIITEM_STYLE_PB_ONE_LINE_AVATAR_TEXT_LEFT_FOCUS_MS:
        //do nothing...
        break;

    case GUIITEM_STYLE_ONE_LINE_BIGICON_TEXT_WITH_MASK_MS:
    case GUIITEM_STYLE_TWO_LINE_BIGICON_TEXT_AND_TEXT_WITH_MASK_MS:
#ifndef MMIPB_RMVITEM_SUPPORT
        dest_item_ptr->state |= GUIITEM_STATE_HAS_AVATAR_MASK;
#endif
        break;
    case GUIITEM_STYLE_ONE_LINE_BIGICON_TEXT_WITH_LEFT_FOCUS_MS:
    case GUIITEM_STYLE_ONE_LINE_BIGICON_TEXT_WITH_RIGHT_FOCUS_MS:
    case GUIITEM_STYLE_TWO_LINE_BIGICON_TEXT_AND_TEXT_WITH_LEFT_FOCUS_MS:
    case GUIITEM_STYLE_TWO_LINE_BIGICON_TEXT_AND_TEXT_WITH_RIGHT_FOCUS_MS:
        dest_item_ptr->state |= GUIITEM_STATE_HAS_AVATAR_MASK;//Bug508635
        break;

    case GUIITEM_STYLE_ONE_LINE_INITIALTEXT_TEXT_WITH_MASK_MS:
    case GUIITEM_STYLE_TWO_LINE_INITIALTEXT_TEXT_AND_TEXT_WITH_MASK_MS:
    case GUIITEM_STYLE_ONE_LINE_INITIALTEXT_TEXT_WITH_LEFT_FOCUS_MS:
    case GUIITEM_STYLE_ONE_LINE_INITIALTEXT_TEXT_WITH_RIGHT_FOCUS_MS:
    case GUIITEM_STYLE_TWO_LINE_INITIALTEXT_TEXT_AND_TEXT_WITH_LEFT_FOCUS_MS:
    case GUIITEM_STYLE_TWO_LINE_INITIALTEXT_TEXT_AND_TEXT_WITH_RIGHT_FOCUS_MS:
        dest_item_ptr->state |= GUIITEM_STATE_HAS_INITIAL_MASK;
        break;

    case GUIITEM_STYLE_TWO_LINE_TEXT_AND_TEXT_WITH_SIMSEL_MS:
    case GUIITEM_STYLE_TWO_LINE_TEXT_AND_TEXT_2_WITH_SIMSEL_MS:
        dest_item_ptr->state |= GUIITEM_STATE_HAS_SIM_SELECTION;
        break;

    case GUIITEM_STYLE_ONE_LINE_BIGICON_TEXT_WITH_SIMSEL_MS:
    case GUIITEM_STYLE_TWO_LINE_BIGICON_TEXT_AND_TEXT_WITH_SIMSEL_MS:
        dest_item_ptr->state |= GUIITEM_STATE_HAS_SIM_SELECTION;
        dest_item_ptr->state |= GUIITEM_STATE_HAS_AVATAR_MASK;
        break;
    case GUIITEM_STYLE_ONE_LINE_INITIALTEXT_TEXT_WITH_SIMSEL_MS:
    case GUIITEM_STYLE_TWO_LINE_INITIALTEXT_TEXT_AND_TEXT_WITH_SIMSEL_MS:
        dest_item_ptr->state |= GUIITEM_STATE_HAS_SIM_SELECTION;
        dest_item_ptr->state |= GUIITEM_STATE_HAS_INITIAL_MASK;
        break;
	case GUIITEM_STYLE_TWO_LINE_BIGICON_TEXT_AND_TEXT_WITH_2_MASK_MS:
    case GUIITEM_STYLE_TWO_LINE_BIGICON_TEXT_AND_TEXT_WITH_2_MASK_RIGHT_FOCUS_MS:
	case GUIITEM_STYLE_TWO_LINE_BIGICON_TEXT_AND_TEXT_WITH_2_MASK_LEFT_FOCUS_MS:
#ifndef MMIPB_RMVITEM_SUPPORT
		dest_item_ptr->state |= GUIITEM_STATE_HAS_AVATAR_MASK;
		dest_item_ptr->state |= GUIITEM_STATE_HAS_ACCESSORY_MASK;
#endif
		break;
	case GUIITEM_STYLE_TWO_LINE_INITIALTEXT_TEXT_AND_TEXT_WITH_2_MASK_MS:
#ifndef MMIPB_RMVITEM_SUPPORT
        break;
#endif
	case GUIITEM_STYLE_TWO_LINE_INITIALTEXT_TEXT_AND_TEXT_WITH_2_MASK_LEFT_FOCUS_MS:
	case GUIITEM_STYLE_TWO_LINE_INITIALTEXT_TEXT_AND_TEXT_WITH_2_MASK_RIGHT_FOCUS_MS:
#ifndef MMIPB_RMVITEM_SUPPORT
		dest_item_ptr->state |= GUIITEM_STATE_HAS_INITIAL_MASK;
		dest_item_ptr->state |= GUIITEM_STATE_HAS_ACCESSORY_MASK;
#endif
		break;
	case GUIITEM_STYLE_TWO_LINE_TEXT_AND_TEXT_ANIM_MS:
		break;
    default:
        is_valid_style = FALSE;
        break;
    }
    
    if (!is_valid_style)
    {
        //SCI_ASSERT(0);
		return FALSE;
    }
    
    //查表对应
    dest_item_ptr->style_ptr = THEMELIST_GetStyle(style_id);
    dest_item_ptr->style_id = style_id;
    
    if (GUIITEM_STYLE_CUSTOMIZE != style_id)
    {
        SCI_ASSERT(PNULL != dest_item_ptr->style_ptr); /*assert verified*/ 
    }
    
    
    return TRUE;
}

/*****************************************************************************/
//  Description :
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN ConvertItem(CTRLLIST_ITEM_T *dest_item_ptr, GUILIST_ITEM_T *src_item_ptr)
{
    BOOLEAN is_style_set = FALSE;

    if (PNULL == dest_item_ptr || PNULL == src_item_ptr)
    {
        return FALSE;
    }

    dest_item_ptr->state = src_item_ptr->item_state;
    is_style_set = SetItemStyle(dest_item_ptr, src_item_ptr->item_style);

    if (!is_style_set)
    {
        return FALSE;
    }

    dest_item_ptr->style_id = src_item_ptr->item_style;
    dest_item_ptr->normal_style_id = src_item_ptr->item_style;
    dest_item_ptr->left_style_id = src_item_ptr->left_item_style;
    dest_item_ptr->right_style_id = src_item_ptr->right_item_style;
    dest_item_ptr->data_ptr = src_item_ptr->item_data_ptr;
    dest_item_ptr->first_letter = src_item_ptr->first_letter;
    dest_item_ptr->item_group = src_item_ptr->item_group;
    dest_item_ptr->group_idx = src_item_ptr->group_idx;
    dest_item_ptr->user_data = src_item_ptr->user_data;
	
    return TRUE;
}
/*****************************************************************************/
//  Description :
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN GUILIST_AppendItem(
    MMI_CTRL_ID_T         ctrl_id, // control id
    const GUILIST_ITEM_T *item_ptr    // 指定的Item
)
{
    CTRLLIST_ITEM_T new_item = {0};
    BOOLEAN is_convert = FALSE;

    is_convert = ConvertItem(&new_item, item_ptr);

    if (!is_convert)
    {
        return FALSE;
    }

    return CTRLLIST_AppendItem(ctrl_id, &new_item);
}
/*****************************************************************************/
//  Description :
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN GUILIST_InsertItem(
    MMI_CTRL_ID_T      ctrl_id,       // control id
    const GUILIST_ITEM_T *item_ptr,   // appointed Item
    uint16              pos               // appointed Position, from 0 to total_item_num
)
{
    CTRLLIST_ITEM_T new_item = {0};
    BOOLEAN is_convert = FALSE;

    is_convert = ConvertItem(&new_item, item_ptr);

    if (!is_convert)
    {
        return FALSE;
    }

    return CTRLLIST_InsertItem(ctrl_id, &new_item, pos);
}
/*****************************************************************************/
//  Description :
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN GUILIST_ReplaceItem(
    MMI_CTRL_ID_T        ctrl_id,            // control id
    const GUILIST_ITEM_T  *replace_item_ptr, // appointed Item
    uint16            pos                    //  appointed Position, from 0 to total_item_num-1
)
{
    CTRLLIST_ITEM_T new_item = {0};
    BOOLEAN is_convert = FALSE;

    is_convert = ConvertItem(&new_item, replace_item_ptr);

    if (!is_convert)
    {
        return FALSE;
    }

    return CTRLLIST_ReplaceItem(ctrl_id, &new_item, pos);
}
/*****************************************************************************/
//  Description :
//  Global resource dependence :
//  Author:
//  Note: 为了兼容
/*****************************************************************************/
PUBLIC const GUILIST_ITEM_T *GUILIST_GetItemPtrByIndex(
    MMI_CTRL_ID_T ctrl_id,
    uint16        index  //from 0 to total_item_num-1
)
{
    CTRLLIST_ITEM_T *get_item_ptr = CTRLLIST_GetItem(ctrl_id, index);

    if (PNULL == get_item_ptr)
    {
        return PNULL;
    }

    s_item_to_user.item_style = get_item_ptr->style_id;
    s_item_to_user.item_state = get_item_ptr->state;
    s_item_to_user.item_data_ptr = get_item_ptr->data_ptr;
    s_item_to_user.first_letter = get_item_ptr->first_letter;
    s_item_to_user.item_group = get_item_ptr->item_group;
    s_item_to_user.group_idx = get_item_ptr->group_idx;
    s_item_to_user.user_data = get_item_ptr->user_data;

    return &s_item_to_user;
}

/*****************************************************************************/
//  Description :设置item风格
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN GUILIST_SetItemStyle(
    MMI_CTRL_ID_T     ctrl_id, // control id
    GUIITEM_STYLE_E item_style,
    uint16          item_index
)
{
    BOOLEAN is_replaced = TRUE;
	THEMELIST_ITEM_STYLE_T *item_style_ptr = PNULL;

	item_style_ptr = THEMELIST_GetStyle(item_style);

	if (PNULL == item_style_ptr)
	{
		return FALSE;
	}

	CTRLLIST_SetItemStyle(ctrl_id, item_index, item_style_ptr, item_style);

    return is_replaced;
}

/*****************************************************************************/
//  Description : 设置item显示风格
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC void GUILIST_SetItemStyleEx(
    MMI_CTRL_ID_T     ctrl_id, // control id
    uint16          item_index,
    THEMELIST_ITEM_STYLE_T *item_style_ptr
)
{
    CTRLLIST_SetItemStyle(ctrl_id, item_index, item_style_ptr, 0);
}

/*****************************************************************************/
//  Description :
//  Global resource dependence :
//  Author:
//  Note: 为了兼容
/*****************************************************************************/
PUBLIC const THEMELIST_ITEM_STYLE_T *GUILIST_GetItemStyle(
    MMI_CTRL_ID_T ctrl_id,
    uint16        index  //from 0 to total_item_num-1
)
{
    THEMELIST_ITEM_STYLE_T *item_style_ptr = PNULL;
    CTRLLIST_ITEM_T *get_item_ptr = CTRLLIST_GetItem(ctrl_id, index);

    if (PNULL == get_item_ptr)
    {
        return PNULL;
    }

    item_style_ptr = get_item_ptr->style_ptr;

    return item_style_ptr;
}

/*****************************************************************************/
//  Description :
//  Global resource dependence :
//  Author:
//  Note: 为了兼容
/*****************************************************************************/
PUBLIC const uint32 GUILIST_GetItemStyleID(
    MMI_CTRL_ID_T ctrl_id,
    uint16        index  //from 0 to total_item_num-1
)
{
    THEMELIST_ITEM_STYLE_T *item_style_ptr = PNULL;
    CTRLLIST_ITEM_T *get_item_ptr = CTRLLIST_GetItem(ctrl_id, index);

    if (PNULL == get_item_ptr)
    {
        return PNULL;
    }

    return get_item_ptr->style_id;
}

/*****************************************************************************/
//  Description : Get the item state
//  Global resource dependence :
//  Author:xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN GUILIST_GetItemState(
    const GUILIST_ITEM_T *item_ptr,
    GUIITEM_STATE_T src_state
)
{
    if (PNULL == item_ptr)
    {
        return FALSE;
    }

    return LIST_GetState(item_ptr->item_state, src_state);
}

/*****************************************************************************/
//  Description : set the item state
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC void GUILIST_SetItemState(
    GUILIST_ITEM_T *item_ptr,
    GUIITEM_STATE_T  src_state,
    BOOLEAN is_true
)
{
    if (PNULL != item_ptr)
    {
        LIST_SetState(&item_ptr->item_state, src_state, is_true);
    }
}



