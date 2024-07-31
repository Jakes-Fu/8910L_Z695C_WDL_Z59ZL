/*****************************************************************************
** File Name:      mmi_mainmenu_data.c                                       *
** Author:                                                                   *
** Date:           07/18/2012                                                *
** Copyright:      2004 Spreatrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to describe punctuation                 *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 07/2012      hua.fang              Creat
******************************************************************************/

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "ctrlmainmenu_export.h"
#include "mmi_mainmenu_data.h"
#include "mmi_mainmenu_synchronize.h"

#include "mmi_text.h"
#include "mmi_image.h"
#include "mmiset_text.h"
#include "guimenu.h"
#include "mmi_menutable.h"

#include "mmitheme_mainmenu.h"
#include "mmi_mainmenu_item_func.h"
#include "mmi_mainmenu_export.h"

#include "mmi_modu_main.h"
#include "mmi_mainmenu_matrix.h"
#ifdef JAVA_SUPPORT_MYRIAD
#include "mmijava_export.h"
#endif


// 根据LCD size区分
#if defined(MAINLCD_SIZE_320X480)
#include "mmi_mainmenu_data_320x480.c"

#elif defined MAINLCD_SIZE_240X400
#include "mmi_mainmenu_data_240x400.c"

#elif defined MAINLCD_SIZE_240X320
#include "mmi_mainmenu_data_240X320.c"

#elif defined MAINLCD_SIZE_176X220
#include "mmi_mainmenu_data_176x220.c"

#elif defined MAINLCD_SIZE_128X160
#include "mmi_mainmenu_data_128x160.c"

#else
#error
#endif

#ifdef JAVA_SUPPORT_MYRIAD
#include "mmijava_common.h"
#endif

#include "variant_config_nv.h"

/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         TYPE AND CONSTANT                                 */
/*---------------------------------------------------------------------------*/

// for user to modify
typedef struct
{
    uint32 menu_id;
    MMI_STRING_T str_info;
} MMIMAINEMENU_ITEM_TEXT_T;

/**--------------------------------------------------------------------------*
 **                         STATIC DEFINITION                                *
 **--------------------------------------------------------------------------*/

static MMI_LINK_NODE_T *s_user_def_text_list_ptr = PNULL;
static MMI_LINK_NODE_T *s_invisable_item_list_ptr = PNULL;
static MMI_LINK_NODE_T *s_disable_item_list_ptr = PNULL;

// 记录所有的item id
static MMIMAINEMENU_ITEM_LIST_T s_item_list = {0};

/**--------------------------------------------------------------------------*
 **                         EXTERNAL DEFINITION                              *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         GLOBAL DEFINITION                                *
 **--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/


/**--------------------------------------------------------------------------*
 **                         GLOBAL DEFINITION                                *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : 增加一个Item
//  Global resource dependence :
//  Author: xiaoqing.lu
//  RETRUN:  返回指针
//  Note:
/*****************************************************************************/
LOCAL void AddItemNode(
    MMI_LINK_NODE_T **list_head_ptr,
    uint32 node
)
{

    if (PNULL == *list_head_ptr)
    {
        *list_head_ptr = MMILINK_CreateHead(node);
    }
    else
    {
        MMI_LINK_NODE_T *tmp_node_ptr = MMILINK_CreateNode(node);
        MMILINK_AddNodeBeforeBaseNode(*list_head_ptr, tmp_node_ptr);
    }

    return;
}

/*****************************************************************************/
//  Description : 增加一个Item
//  Global resource dependence :
//  Author: xiaoqing.lu
//  RETRUN:  返回指针
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN SearchItemInTextList(
    MMI_LINK_NODE_T const *node_ptr,
    uint32 condition1,
    uint32 condition2
)
{
    if (condition1 == ((MMIMAINEMENU_ITEM_TEXT_T *)node_ptr->data)->menu_id)
    {
        return TRUE;
    }

    return FALSE;
}

/*****************************************************************************/
//  Description : 增加一个Item
//  Global resource dependence :
//  Author: xiaoqing.lu
//  RETRUN:  返回指针
//  Note:
/*****************************************************************************/
LOCAL void FreeItemTextNode(
    MMI_LINK_NODE_T const *node_ptr
)
{
    MMIMAINEMENU_ITEM_TEXT_T *item_text_ptr = PNULL;

    if (PNULL == node_ptr)
    {
        return;
    }

    item_text_ptr = (MMIMAINEMENU_ITEM_TEXT_T *)node_ptr->data;

    if (PNULL != item_text_ptr)
    {
        SCI_FREE(item_text_ptr->str_info.wstr_ptr);
        SCI_FREE(item_text_ptr);
    }

    return;
}

/*****************************************************************************/
//  Description : 增加一个Item
//  Global resource dependence :
//  Author: xiaoqing.lu
//  RETRUN:  返回指针
//  Note:
/*****************************************************************************/
PUBLIC void MMIMAINMENU_SetItemText(
    uint32 menu_id,
    MMI_STRING_T *str_ptr
)
{
    MMIMAINEMENU_ITEM_TEXT_T *item_text_ptr = PNULL;
    MMI_LINK_NODE_T *node_ptr = PNULL;

    if (PNULL == str_ptr)
    {
        return;
    }

    if (PNULL != s_user_def_text_list_ptr)
    {
        node_ptr = MMILINK_SearchNode(s_user_def_text_list_ptr, TRUE, SearchItemInTextList, menu_id, 0);

        if (PNULL != node_ptr) // 首先删除
        {
            s_user_def_text_list_ptr = MMILINK_DestroyNode(node_ptr, s_user_def_text_list_ptr, FreeItemTextNode);
        }
    }

    item_text_ptr = SCI_ALLOC_APP(sizeof(MMIMAINEMENU_ITEM_TEXT_T));
    SCI_MEMSET(item_text_ptr, 0, sizeof(MMIMAINEMENU_ITEM_TEXT_T));

    item_text_ptr->menu_id = menu_id;
    item_text_ptr->str_info.wstr_len = str_ptr->wstr_len;

    if (0 != str_ptr->wstr_len)
    {
        item_text_ptr->str_info.wstr_ptr = SCI_ALLOC_APP((str_ptr->wstr_len+1) * sizeof(wchar));
        SCI_MEMSET(item_text_ptr->str_info.wstr_ptr, 0, (str_ptr->wstr_len+1) * sizeof(wchar));
        SCI_MEMCPY(item_text_ptr->str_info.wstr_ptr, str_ptr->wstr_ptr, str_ptr->wstr_len * sizeof(wchar));
    }

    AddItemNode(&s_user_def_text_list_ptr, (uint32)item_text_ptr);

    return;
}

/*****************************************************************************/
//  Description : 增加一个Item
//  Global resource dependence :
//  Author: xiaoqing.lu
//  RETRUN:  返回指针
//  Note:
/*****************************************************************************/
PUBLIC void MMIMAINMENU_SetItemInvisiable(
    uint32 menu_id,
    BOOLEAN is_true
)
{
	MMI_LINK_NODE_T *node_ptr = PNULL;

    if (PNULL != s_invisable_item_list_ptr)
    {
		node_ptr = MMILINK_SearchNode(s_invisable_item_list_ptr, TRUE, PNULL, menu_id, 0);
    }

    if (is_true)
    {
		if (PNULL == node_ptr)
		{
	        AddItemNode(&s_invisable_item_list_ptr, (uint32)menu_id);
		}
    }
    else
    {
		if (PNULL != node_ptr)
		{
            s_invisable_item_list_ptr = MMILINK_DestroyNode(node_ptr, s_invisable_item_list_ptr, PNULL);
		}
    }

    return;
}

/*****************************************************************************/
//  Description :
//  Global resource dependence :
//  Author: xiaoqing.lu
//  RETRUN:
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIMAINMENU_IsItemInvisiable(
    uint32 menu_id
)
{
    BOOLEAN is_invisiable = FALSE;

    if (PNULL != s_disable_item_list_ptr)
    {
        MMI_LINK_NODE_T *node_ptr = MMILINK_SearchNode(s_invisable_item_list_ptr, TRUE, PNULL, menu_id, 0);

        if (PNULL != node_ptr)
        {
            is_invisiable = TRUE;
        }
    }

    return is_invisiable;
}

/*****************************************************************************/
//  Description : 增加一个Item
//  Global resource dependence :
//  Author: xiaoqing.lu
//  RETRUN:  返回指针
//  Note:
/*****************************************************************************/
PUBLIC void MMIMAINMENU_SetItemDisable(
    uint32 menu_id,
    BOOLEAN is_true
)
{
	MMI_LINK_NODE_T *node_ptr = PNULL;

    if (PNULL != s_disable_item_list_ptr)
    {
        node_ptr = MMILINK_SearchNode(s_disable_item_list_ptr, TRUE, PNULL, menu_id, 0);
    }

    if (is_true)
    {
		if (PNULL == node_ptr)
		{
	        AddItemNode(&s_disable_item_list_ptr, (uint32)menu_id);
		}
    }
    else
    {
        if (PNULL != node_ptr)
        {
            s_disable_item_list_ptr = MMILINK_DestroyNode(node_ptr, s_disable_item_list_ptr, PNULL);
        }
    }

    return;
}

/*****************************************************************************/
//  Description :
//  Global resource dependence :
//  Author: xiaoqing.lu
//  RETRUN:
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIMAINMENU_IsItemDisable(
    uint32 menu_id
)
{
    BOOLEAN is_disable = FALSE;

    if (PNULL != s_disable_item_list_ptr)
    {
        MMI_LINK_NODE_T *node_ptr = MMILINK_SearchNode(s_disable_item_list_ptr, TRUE, PNULL, menu_id, 0);

        if (PNULL != node_ptr)
        {
            is_disable = TRUE;
        }
    }

    return is_disable;
}

/*****************************************************************************/
//  Description :
//  Global resource dependence :
//  Author: xiaoqing.lu
//  RETRUN:  返回指针
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIMAINMENU_GetItem(
    MMIMAINEMENU_ITEM_LIST_T *item_list_ptr, // in
    uint16 index,  // in
    MMIMAINMENU_ITEM_INFO_T *item_ptr // in/out
)
{
    BOOLEAN is_find = FALSE;
    uint16 i = 0;
    MMI_LINK_NODE_T *node_ptr = PNULL;

    if (PNULL == item_list_ptr || PNULL == item_ptr)
    {
        return FALSE;
    }

    node_ptr = item_list_ptr->list_head_ptr;

    for (i = 0; i < item_list_ptr->item_count; i++)
    {
        if (PNULL == node_ptr)
        {
            break;
        }

        if (i == index)
        {
            is_find = TRUE;
            item_ptr->group_id = ((MMIMAINMENU_ITEM_INFO_T *)node_ptr->data)->group_id;
            item_ptr->menu_id = ((MMIMAINMENU_ITEM_INFO_T *)node_ptr->data)->menu_id;
            break;
        }

        node_ptr = node_ptr->next_ptr;
    }

    return is_find;
}

/*****************************************************************************/
//  Description :
//  Global resource dependence :
//  Author: xiaoqing.lu
//  RETRUN:  返回指针
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIMAINMENU_GetItemData(
    uint32 menu_id,
    uint32 group_id,
    CTRLMAINMENU_ITEM_T *item_ptr
)
{
    uint32 i = 0, count = 0;
    BOOLEAN is_item_exist = FALSE;
    MMI_LINK_NODE_T *node_ptr = PNULL;

    if (PNULL == item_ptr)
    {
        return FALSE;
    }

    count = sizeof(s_mainmenu_item_data) / sizeof(MMI_MAINMENU_ITEM_T);

    item_ptr->menu_id = menu_id;
    item_ptr->group_id = group_id;
#ifdef DYNAMIC_MAINMENU_SUPPORT
    if (DYNAMIC_MAINMENU_DEFAULT_GROUP_ID == group_id)
    {
        GUIMAINMENU_DY_ITEM_T item_info = {0};
		BOOLEAN is_udisk_run = FALSE;

        is_item_exist = MMIMENU_DySlideGetDispInfo(menu_id, &item_info);
		is_udisk_run  = MMIAPIUDISK_UdiskIsRun();
        if(!is_item_exist)
		{
			return FALSE;		
		}
		if (is_udisk_run)
        {
			item_ptr->icon.type = CTRL_ICON_ID;
            item_ptr->icon.data.icon_id = IMAGE_MY_APPLICATION;//IMAGE_MAINMENU_ICON_UNKNOWN;

            item_ptr->text.type = CTRL_TEXT_ID;
            item_ptr->text.data.text_id = TXT_NULL;
			return TRUE;	         
        }

        MMIMENU_DySlideGetIcon(menu_id, PNULL);

        if (is_item_exist && (PNULL != item_info.ori_data_ptr))
        {
            item_ptr->text.type = CTRL_TEXT_BUFFER_USER;
            item_ptr->text.data.str_info.wstr_len = item_info.ori_data_ptr->dynamic_menu_info.text_len;
            item_ptr->text.data.str_info.wstr_ptr = item_info.ori_data_ptr->dynamic_menu_info.text;

            if (item_info.ori_data_ptr->dynamic_menu_info.has_icon)
            {
                item_ptr->icon.type = CTRL_ICON_BUFFER;
                item_ptr->icon.data.buffer.data = item_info.ori_data_ptr->dynamic_menu_info.icon_ptr;
                item_ptr->icon.data.buffer.height = item_info.ori_data_ptr->dynamic_menu_info.icon_height;
                item_ptr->icon.data.buffer.width = item_info.ori_data_ptr->dynamic_menu_info.icon_width;
            }
            else
            {
                item_ptr->icon.type = CTRL_ICON_ID;
                item_ptr->icon.data.icon_id = IMAGE_MY_APPLICATION;
            }

            item_ptr->active_icon.type = CTRL_ICON_ID;
            item_ptr->active_icon.data.icon_id = IMAGE_MY_APPLICATION;

            item_ptr->call_back_start = (CTRLMAINMENU_ITEM_FUNC)item_info.ori_data_ptr->dynamic_menu_info.link_function_ptr;
            item_ptr->call_back_delete = (CTRLMAINMENU_ITEM_FUNC)item_info.ori_data_ptr->dynamic_menu_info.delete_function_ptr;
            item_ptr->param_data1 = (uint32)item_info.ori_data_ptr->dynamic_menu_info.param1ptr;
            item_ptr->param_data2 = (uint32)item_info.ori_data_ptr->dynamic_menu_info.param2ptr;
            
            if((10000 < item_ptr->menu_id) && (item_ptr->menu_id < 10007))
            {
                uint8         is_show_uninstall_option = 0;
                EFS_NvitemRead(APP_NV_SHOW_UNINSTALL_PRELOAD_GAME, sizeof(uint8), &is_show_uninstall_option);
                if(0 != is_show_uninstall_option)
                {
                    item_ptr->is_uninstallable = TRUE;
                }
                SCI_TRACE_LOW("Cheney Check mainmenu preload 2  item_ptr->is_uninstallable = %d", item_ptr->is_uninstallable);
            }
            else
            {
                item_ptr->is_uninstallable = item_info.ori_data_ptr->is_uninstallable;
            }
        }
    }
    else 
#endif
    if (0 == group_id)
    {
        for (i = 0; i < count; i++)
        {
            if (item_ptr->menu_id != s_mainmenu_item_data[i].menu_id)
            {
                continue;
            }

            is_item_exist = TRUE;

            item_ptr->active_icon.type = CTRL_ICON_ID;
            item_ptr->active_icon.data.icon_id = s_mainmenu_item_data[i].active_img_id;

            item_ptr->icon.type = CTRL_ICON_ID;
            item_ptr->icon.data.icon_id = s_mainmenu_item_data[i].icon_id;

            item_ptr->call_back_start = s_mainmenu_item_data[i].call_back;
            item_ptr->param_data1 = s_mainmenu_item_data[i].user_data;

            if (PNULL != s_user_def_text_list_ptr)
            {
                node_ptr = MMILINK_SearchNode(s_user_def_text_list_ptr, TRUE, SearchItemInTextList, menu_id, 0);
            }

            if (PNULL != node_ptr)
            {
                MMIMAINEMENU_ITEM_TEXT_T *item_text_ptr = (MMIMAINEMENU_ITEM_TEXT_T *)node_ptr->data;
                item_ptr->text.type = CTRL_TEXT_BUFFER_USER;
                item_ptr->text.data.str_info.wstr_len = item_text_ptr->str_info.wstr_len;
                item_ptr->text.data.str_info.wstr_ptr = item_text_ptr->str_info.wstr_ptr;
            }
            else
            {
                item_ptr->text.type = CTRL_TEXT_ID;
                item_ptr->text.data.text_id = s_mainmenu_item_data[i].text_id;
            }

            item_ptr->is_uninstallable = FALSE;

            break;
        }
    }

    if (PNULL != s_disable_item_list_ptr)
    {
        node_ptr = MMILINK_SearchNode(s_disable_item_list_ptr, TRUE, PNULL, menu_id, 0);
    }

    if (PNULL != node_ptr)
    {
        item_ptr->is_disable = TRUE;
    }

    if (PNULL != s_invisable_item_list_ptr)
    {
        node_ptr = MMILINK_SearchNode(s_invisable_item_list_ptr, TRUE, PNULL, menu_id, 0);
    }

    if (PNULL != node_ptr)
    {
        item_ptr->is_invisiable = TRUE;
    }

    return is_item_exist;
}

/*****************************************************************************/
//  Description : 增加一个Item
//  Global resource dependence :
//  Author: xiaoqing.lu
//  RETRUN:  返回指针
//  Note:
/*****************************************************************************/
PUBLIC MMIMAINEMENU_ITEM_LIST_T *MMIMAINMENU_GetItemList(void)
{
    uint32 i = 0, count = 0;
    MMIMAINMENU_ITEM_INFO_T *item_info_ptr = PNULL;
    MMI_LINK_NODE_T *node_ptr = PNULL;

    MMIMAINMENU_ClearItemList();

    count = sizeof(s_mainmenu_item_data) / sizeof(MMI_MAINMENU_ITEM_T);

    s_item_list.item_count = count;

    // 静态
    for (i = 0; i < count; i++)
    {
        item_info_ptr = SCI_ALLOC_APP(sizeof(MMIMAINMENU_ITEM_INFO_T));

        SCI_MEMSET(item_info_ptr, 0, sizeof(MMIMAINMENU_ITEM_INFO_T));

        item_info_ptr->menu_id = s_mainmenu_item_data[i].menu_id;
        item_info_ptr->group_id = 0;

        AddItemNode(&s_item_list.list_head_ptr, (uint32)item_info_ptr);
    }

    // 动态
#ifdef DYNAMIC_MAINMENU_SUPPORT
    if (MMIMENU_DyGetAllItemData())
    {
        uint32 dy_item_cnt = 0;
        MMI_IMAGE_ID_T select_icon_id = 0;

        dy_item_cnt = MMIMENU_DyGetItemCount();

        for (i = 0; i < dy_item_cnt; i++)
        {
            BOOLEAN is_get_info = FALSE;
            GUIMAINMENU_DY_ITEM_T item_info = {0};

            is_get_info = MMIMENU_DyListGetDispInfoEx(i, &item_info);

            if (is_get_info && PNULL != item_info.ori_data_ptr)
            {
                item_info_ptr = SCI_ALLOC_APP(sizeof(MMIMAINMENU_ITEM_INFO_T));

                SCI_MEMSET(item_info_ptr, 0, sizeof(MMIMAINMENU_ITEM_INFO_T));

                item_info_ptr->menu_id = item_info.ori_data_ptr->menu_id;
                item_info_ptr->group_id = DYNAMIC_MAINMENU_DEFAULT_GROUP_ID;

                s_item_list.item_count++;
                AddItemNode(&s_item_list.list_head_ptr, (uint32)item_info_ptr);
            }
        }
    }
#endif//DYNAMIC_MAINMENU_SUPPORT

    return &s_item_list;
}

/*****************************************************************************/
//  Description : Get Matrix Item List
//  Global resource dependence :
//  Author: 
//  RETRUN:  返回指针
//  Note:
/*****************************************************************************/
PUBLIC MMIMAINEMENU_ITEM_LIST_T *MMIMAINMENU_GetMatrixItemList(
    BOOLEAN is_install_mode
)
{
    uint32 i = 0, j = 0, st_item_cnt = 0, dy_item_cnt = 0;
    MMIMAINMENU_ITEM_INFO_T *item_info_ptr = PNULL;
    BOOLEAN is_add = FALSE;

    MMIMAINMENU_ClearItemList();

    s_item_list.item_count = 0;

    // 静态
    st_item_cnt = sizeof(s_mainmenu_item_data) / sizeof(MMI_MAINMENU_ITEM_T);
    
    for (i = 0; i < st_item_cnt; i++)
    {
        is_add = FALSE;
        
        item_info_ptr = SCI_ALLOC_APP(sizeof(MMIMAINMENU_ITEM_INFO_T));

        if(PNULL != item_info_ptr)
        {
            SCI_MEMSET(item_info_ptr, 0, sizeof(MMIMAINMENU_ITEM_INFO_T));

            for(j = 0; j < MMIMAINMENU_GetItemCount(); j++)
            {
                if(MMIMAINMENU_GetMenuIdByIndex(j) == s_mainmenu_item_data[i].menu_id)
                {
                    is_add = TRUE;
                    break;
                }
            }

            if(is_add)
            {
                item_info_ptr->menu_id = s_mainmenu_item_data[i].menu_id;
                item_info_ptr->group_id = 0;
                item_info_ptr->suite_id = -1;

                s_item_list.item_count++;
                AddItemNode(&s_item_list.list_head_ptr, (uint32)item_info_ptr);
            }
            else
            {
                SCI_FREE(item_info_ptr);
            }
        }
    }

    if(is_install_mode)
    {
        item_info_ptr = SCI_ALLOC_APP(sizeof(MMIMAINMENU_ITEM_INFO_T));
        if(PNULL != item_info_ptr)
        {
            SCI_MEMSET(item_info_ptr, 0, sizeof(MMIMAINMENU_ITEM_INFO_T));

            item_info_ptr->menu_id = s_mainmenu_item_data[st_item_cnt - 1].menu_id;
            item_info_ptr->group_id = 0;
            item_info_ptr->suite_id = -1;

            s_item_list.item_count++;
            AddItemNode(&s_item_list.list_head_ptr, (uint32)item_info_ptr);
        }
    }

    // 动态
#ifdef DYNAMIC_MAINMENU_SUPPORT
    if (MMIMENU_DyGetAllItemData())
    {
        dy_item_cnt = MMIMENU_DyGetItemCount();

        for (i = 0; i < dy_item_cnt; i++)
        {
            BOOLEAN is_get_info = FALSE;
            BOOLEAN is_preinstall = FALSE;
            GUIMAINMENU_DY_ITEM_T item_info = {0};

            is_add = FALSE;

            is_get_info = MMIMENU_DyListGetDispInfoEx(i, &item_info);

            if (is_get_info && PNULL != item_info.ori_data_ptr)
            {
                item_info_ptr = SCI_ALLOC_APP(sizeof(MMIMAINMENU_ITEM_INFO_T));

                if(PNULL != item_info_ptr)
                {
                    SCI_MEMSET(item_info_ptr, 0, sizeof(MMIMAINMENU_ITEM_INFO_T));

                    item_info_ptr->menu_id = item_info.ori_data_ptr->menu_id;
                    item_info_ptr->group_id = DYNAMIC_MAINMENU_DEFAULT_GROUP_ID;
                    item_info_ptr->suite_id = item_info.ori_data_ptr->suite_id;
                    is_preinstall = !(item_info.ori_data_ptr->is_uninstallable);

                    if(is_preinstall)
                    {
                        for(j = 0; j < MMIMAINMENU_GetItemCount(); j++)
                        {
                            if(MMIMAINMENU_GetMenuIdByIndex(j) == item_info_ptr->menu_id)
                            {
                                is_add = TRUE;
                                break;
                            }
                        }
                    }
                    else
                    {
                        is_add = TRUE;
                    }

                    if(is_add)
                    {
                        s_item_list.item_count++;
                        AddItemNode(&s_item_list.list_head_ptr, (uint32)item_info_ptr);
#ifdef JAVA_SUPPORT_MYRIAD
                        if(-1 != item_info_ptr->suite_id)
                        {
                            MMIJAVA_EnvironmentInit();
                            if(is_preinstall || MMIAPIJAVA_isMIDletSuiteAvailable(item_info_ptr->suite_id))
                            {
                                MMIMAINMENU_SetItemDisable(item_info_ptr->menu_id, FALSE);
                            }
                            else
                            {
                                MMIMAINMENU_SetItemDisable(item_info_ptr->menu_id, TRUE);
                            }
                        }
#endif
                    }
                    else
                    {
                        SCI_FREE(item_info_ptr);
                    }
                }
            }
        }
    }
#endif//DYNAMIC_MAINMENU_SUPPORT

    return &s_item_list;
}

/*****************************************************************************/
//  Description : 增加一个Item
//  Global resource dependence :
//  Author: xiaoqing.lu
//  RETRUN:  返回指针
//  Note:
/*****************************************************************************/
PUBLIC void MMIMAINMENU_ClearItemList(void)
{
    if (s_item_list.item_count == 0)
    {
        return;
    }

    s_item_list.item_count = 0;

    if (PNULL != s_item_list.list_head_ptr)
    {
        MMILINK_DestroyLink(s_item_list.list_head_ptr, MMIMAINMENU_FreeItemNode);
        s_item_list.list_head_ptr = PNULL;
    }

    return;
}

/*****************************************************************************/
//  Description : 增加一个Item
//  Global resource dependence :
//  Author: xiaoqing.lu
//  RETRUN:  返回指针
//  Note:
/*****************************************************************************/
PUBLIC void MMIMAINMENU_AddItemNode(
    MMI_LINK_NODE_T **list_head_ptr,
    uint32 node
)
{
    AddItemNode(list_head_ptr, node);
}

/*****************************************************************************/
//  Description : free一个Item
//  Global resource dependence :
//  Author: xiaoqing.lu
//  RETRUN:
//  Note:
/*****************************************************************************/
PUBLIC void MMIMAINMENU_FreeItemNode(
    MMI_LINK_NODE_T const *node_ptr
)
{
    MMIMAINMENU_ITEM_INFO_T *item_node_ptr = PNULL;

    if (PNULL != node_ptr)
    {
        item_node_ptr = (MMIMAINMENU_ITEM_INFO_T *)node_ptr->data;

        if (PNULL != item_node_ptr)
        {
            SCI_FREE(item_node_ptr);
        }
    }
}


/*****************************************************************************/
//  Description : free一个Item
//  Global resource dependence :
//  Author: xiaoqing.lu
//  RETRUN:
//  Note:
/*****************************************************************************/
PUBLIC void MMIMAINMENU_FreePageNode(
    MMI_LINK_NODE_T const *node_ptr
)
{
    MMIMAINEMENU_ITEM_LIST_T *item_node_ptr = PNULL;

    if (PNULL != node_ptr)
    {
        item_node_ptr = (MMIMAINEMENU_ITEM_LIST_T *)node_ptr->data;

        if (PNULL != item_node_ptr)
        {
        	if (PNULL != item_node_ptr->list_head_ptr)
        	{
            	MMILINK_DestroyLink(item_node_ptr->list_head_ptr, MMIMAINMENU_FreeItemNode);
        	}
            SCI_FREE(item_node_ptr);
        }
    }
}

/*****************************************************************************/
//  Description : read menu item info
//  Global resource dependence :
//  Author:Xiaoqing.Lu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIMAINMENU_ReadAllMenuItemInfo(
    MMIMAINMENU_ITEM_INFO_T *item_ptr,
    uint32 *num_ptr
)
{
    MMIMAINEMENU_ITEM_LIST_T *item_list_ptr = PNULL;
    MMI_LINK_NODE_T          *node_ptr = PNULL;
	uint16 item_index = 0;
	uint16 i = 0;

    if ( PNULL == item_ptr )
    {
        return FALSE;
    }

    // 拿原始item数据
    item_list_ptr = MMIMAINMENU_GetItemList();
    if (PNULL == item_list_ptr)
    {
        return FALSE;
    }

    node_ptr = item_list_ptr->list_head_ptr;

    for (i = 0; i < item_list_ptr->item_count; i++)
    {
        if (PNULL == node_ptr)
        {
            break;
        }

        if (0 != ((MMIMAINMENU_ITEM_INFO_T *)node_ptr->data)->menu_id )
        {
            item_ptr[item_index].group_id = ((MMIMAINMENU_ITEM_INFO_T *)node_ptr->data)->group_id;
            item_ptr[item_index].menu_id = ((MMIMAINMENU_ITEM_INFO_T *)node_ptr->data)->menu_id;

            item_index++;
        }

        node_ptr = node_ptr->next_ptr;
    }

    if (PNULL != num_ptr)
    {
        *num_ptr = item_index;
    }

    return TRUE;
}
