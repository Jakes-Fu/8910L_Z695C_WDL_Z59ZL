/*! @file guilistbox.h
@brief ���ļ�������listbox�ؼ��Ķ���ӿں���������
@author xiaoqing.lu
@version 1.0
@date 03/23/2004
@remarks Copyright: 2011 Spreadtrum, Incorporated. All Rights Reserved.
@details
@note
-# 03/2004        xiaoqing.lu      Create
-# 02/12/2006     Xiaoqing.Lu      Modify
*******************************************************************************/

#ifndef  _GUILISTBOX_H_
#define  _GUILISTBOX_H_

/*----------------------------------------------------------------------------*/
/*                         DEPENDANCY                                         */
/*----------------------------------------------------------------------------*/
#include "ctrllist_export.h"

/*----------------------------------------------------------------------------*/
/*                         Compiler Flag                                      */
/*----------------------------------------------------------------------------*/
#ifdef __cplusplus
extern   "C"
{
#endif


/*----------------------------------------------------------------------------*/
/*                         Constant Variables                                 */
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/*                         Macro Declaration                                  */
/*----------------------------------------------------------------------------*/
/*! @def GUILIST_STRING_MAX_NUM
@brief ��������ַ��������ú���list�ؼ��в�û��ʹ�ã�����app���ý϶࣬���������ַ��������С
*/
#define GUILIST_STRING_MAX_NUM  60



/*! @struct GUILIST_ITEM_T
@brief �ؼ�ITEM����
@note ��һ��split֮ǰ���ǵ�0�飬֮����ǵ�1�飬��һ��split�����ǵ�1��
*/
typedef struct
{
    GUIITEM_STYLE_E              item_style;    /*!< item����*/
    GUIITEM_STYLE_E              left_item_style;  /*!<LeftFocus item����*/
    GUIITEM_STYLE_E              right_item_style; /*!<RightFocus item����*/
    GUIITEM_STATE_T              item_state;    /*!< item״̬*/
    GUILIST_ITEM_DATA_T         *item_data_ptr; /*!< item����*/

    uint16                       first_letter;  /*!< ������ĸ*/
    uint16                       item_group;    /*!< item�Ĺ��࣬ͬһ������Ϊһ�࣬������ͬ����1��ʼ��0��ʾ��ʹ�ñ�������Ҫ����*/
    uint16                       group_idx;     /*!< item�����������鼴split��Ȼ�ָ��γɵ� */

    uint32                       user_data;     /*!< �û��Զ�������*/

} GUILIST_ITEM_T;




/*----------------------------------------------------------------------------*/
/*                         Function Declare                                   */
/*----------------------------------------------------------------------------*/
/***************************************************************************//*!
@brief ���ÿؼ��������������ڴ�
@author xiaoqing.lu
@param ctrl_id  [in]            �ؼ�ID
@param max_item_num [in]    �ؼ��������
@param is_dynamic_item_data [in]    �Ƿ�̬����
@retval true/false
@return ���سɹ���ʧ��
@note ��Ϊ��̬����ʱ���ؼ���ʾÿ��item�������򴰿�Ҫ����
*******************************************************************************/
#define  GUILIST_SetMaxItem CTRLLIST_SetMaxItem

/***************************************************************************//*!
@brief �������ѡ�е�ITEM��
@author xiaoqing.lu
@param ctrl_id  [in]     �ؼ�ID
@param mark_cnt_max  [in]    ���ѡ����
@retval true/false
@return ���سɹ���ʧ��
@note
*******************************************************************************/
#define GUILIST_SetMaxSelectedItem CTRLLIST_SetMaxSelectedItem

/***************************************************************************//*!
@brief ���ÿؼ�title��ʾ���
@author xiaoqing.lu
@param ctrl_id  [in]     �ؼ�ID
@param title_style [in] ���
@retval true/false
@return ���سɹ���ʧ��
@note
*******************************************************************************/
#define GUILIST_SetTitleStyle CTRLLIST_SetTitleStyle

/***************************************************************************//*!
@brief ����check list�ؼ���check icon�Ƿ����ڶ���list style
@author jinju.ma
@param ctrl_id  [in]     �ؼ�ID
@param is_used  [in]     �Ƿ����ڶ���list style
@note
*******************************************************************************/
#define GUILIST_SetIsCheckUsedForMultiLine CTRLLIST_SetIsCheckUsedForMultiLine

/***************************************************************************//*!
@brief ����list�ؼ��Ƿ�����end item��softkey���б߾�
@author 
@param ctrl_id  [in]     �ؼ�ID
@param is_left  [in]     �Ƿ�����end item��softkey���б߾�
@note
*******************************************************************************/
#define GUILIST_SetIsPermitBottomMargin CTRLLIST_SetIsPermitBottomMargin

/***************************************************************************//*!
@brief ����check list�ؼ���check icon�Ƿ���ʾ�����
@author jinju.ma
@param ctrl_id  [in]     �ؼ�ID
@param is_left  [in]     �Ƿ���ʾ�����
@note
*******************************************************************************/
#define GUILIST_SetCheckIconLeftFlag CTRLLIST_SetCheckIconLeftFlag

/***************************************************************************//*!
@brief set the list mark display position
@author jinju.ma
@param ctrl_id      [in]     �ؼ�ID
@param left_margin  [in]     ��߾�
@param top_margin   [in]     �ϱ߾�
@note
*******************************************************************************/
#define GUILIST_SetCheckIconMargin CTRLLIST_SetCheckIconMargin

/***************************************************************************//*!
@brief set the list use special check icon
@author jinju.ma
@param ctrl_id      		[in]     �ؼ�ID
@param is_use_special_icon  [in]     �Ƿ�ʹ�������ѡͼ��
@note
*******************************************************************************/
#define GUILIST_SetUseSpecialCheckIconFlag CTRLLIST_SetUseSpecialCheckIconFlag

/***************************************************************************//*!
@brief set special check icon for the list
@author jinju.ma
@param ctrl_id      	  [in]     �ؼ�ID
@param selected_image     [in]     ѡ��ͼ��
@param unselected_image   [in]     δѡ��ͼ��
@note
*******************************************************************************/
#define GUILIST_SetSpecialCheckIcon CTRLLIST_SetSpecialCheckIcon

/***************************************************************************//*!
@brief ����list������͸������ʾ
@author Yanyan.an
@param ctrl_id          [in]  �ؼ�ID
@param is_transpant     [in]  �Ƿ���ʾ����͸��
@param alpha            [in]  list����͸����
@note
*******************************************************************************/
#define GUILIST_SetBgTransparent CTRLLIST_SetBgTransparent

/***************************************************************************//*!
@brief Append�ؼ���
@author xiaoqing.lu
@param ctrl_id  [in]        �ؼ�ID
@param item_ptr Item [in]   ����ָ��
@retval true/false
@return ���سɹ���ʧ��
@note
*******************************************************************************/
PUBLIC BOOLEAN GUILIST_AppendItem(
    MMI_CTRL_ID_T         ctrl_id, // control id
    const GUILIST_ITEM_T *item_ptr    // ָ����Item
);


/***************************************************************************//*!
@brief Insert�ؼ���
@author xiaoqing.lu
@param ctrl_id  [in]    �ؼ�ID
@param item_ptr [in]    ����ָ��
@param pos  [in]    �ƶ�Insertλ��
@retval true/false
@return ���سɹ���ʧ��
@note posС�������
*******************************************************************************/
PUBLIC BOOLEAN GUILIST_InsertItem(
    MMI_CTRL_ID_T      ctrl_id,       // control id
    const GUILIST_ITEM_T *item_ptr,   // appointed Item
    uint16              pos               // appointed Position, from 0 to total_item_num
);

/***************************************************************************//*!
@brief Removeһ��Item
@author xiaoqing.lu
@param ctrl_id  [in]    �ؼ�ID
@param pos  [in]    �ƶ���ɾ�������������
@retval true/false
@return ���سɹ���ʧ��
@note
*******************************************************************************/
#define GUILIST_RemoveItem CTRLLIST_RemoveItem


/***************************************************************************//*!
@brief Remove����Item��
@author xiaoqing.lu
@param ctrl_id  [in]    �ؼ�ID
@retval true/false
@return ���سɹ���ʧ��
@note
*******************************************************************************/
#define GUILIST_RemoveAllItems CTRLLIST_RemoveAllItems

/***************************************************************************//*!
@brief Replaceһ��Item
@author xiaoqing.lu
@param ctrl_id  [in]    �ؼ�ID
@param replace_item_ptr [in] �����滻������
@param pos  [in]    �ƶ����滻��item����
@retval true/false
@return ���سɹ���ʧ��
@note
*******************************************************************************/
PUBLIC BOOLEAN GUILIST_ReplaceItem(
    MMI_CTRL_ID_T        ctrl_id,            // control id
    const GUILIST_ITEM_T  *replace_item_ptr, // appointed Item
    uint16            pos                    //  appointed Position, from 0 to total_item_num-1
);


/***************************************************************************//*!
@brief ���ÿؼ������ϸ����
@author yelianna
@param ctrl_id  [in]    �ؼ�ID
@param item_data_ptr [in]   �������õ�����
@param pos  [in]    �ƶ������õ�item����
@retval true/false
@return ���سɹ���ʧ��
@note
*******************************************************************************/
#define GUILIST_SetItemData CTRLLIST_SetItemData

/***************************************************************************//*!
@brief ����Item��������
@author yelianna
@param ctrl_id  [in]    �ؼ�ID
@param item_content_ptr  [in]   �������õ�����
@param item_index   [in]    �ƶ������õ�item����
@param item_content_index [in]  �������ݵ����꣨item_content_ptrΪ���飩
@retval true/false
@return ���سɹ���ʧ��
@note
*******************************************************************************/
#define GUILIST_SetItemContent CTRLLIST_SetItemContent

/***************************************************************************//*!
@brief ����Item��������
@author xiaoqing.lu
@param ctrl_id  [in]    �ؼ�ID
@param item_content_ptr  [in]   �������õ�����
@param item_index   [in]    �ƶ������õ�item����
@param item_content_index [in]  �������ݵ����꣨item_content_ptrΪ���飩
@param is_syn_decode    [in]    �Ƿ��첽
@param is_update [in]   �Ƿ�����ˢ��
@retval true/false
@return ���سɹ���ʧ��
@note
*******************************************************************************/
#define GUILIST_SetItemContentExt CTRLLIST_SetItemContentExt

/***************************************************************************//*!
@brief ��ñ�ѡ�е��������
@author xiaoqing.lu
@param ctrl_id  [in]    �ؼ�ID
@param idx_ptr [in/out] ���ڴ���������������飬����Ϊ��
@param array_len    [in]    �����С
@retval uint16����
@return ���ر�ѡ�е�������
@note �û������ȷ����������鵽��Ӧ�ö����ô�����ȵ��øýӿڻ��������Ȼ���ٷ����ڴ��ȡ���屻ѡ�����������
*******************************************************************************/
#define GUILIST_GetSelectedItemIndex CTRLLIST_GetSelectedItemIndex


/***************************************************************************//*!
@brief ����ĳһ��Ϊѡ����
@author xiaoqing.lu
@param ctrl_id  [in]    �ؼ�ID
@param item_pos  [in]   �����õ�����
@param is_selected  [in]    TRUE : selected; FALSE: unselected
@retval true/false
@return ���سɹ���ʧ��
@note
*******************************************************************************/
#define GUILIST_SetSelectedItem CTRLLIST_SetSelectedItem

/***************************************************************************//*!
@brief ���һ��Itemָ��
@author James.Zhang
@param ctrl_id  [in]    �ؼ�ID
@param index  [in]  ����
@retval ָ��
@return ����ĳһ���ָ��
@note
*******************************************************************************/
PUBLIC const GUILIST_ITEM_T *GUILIST_GetItemPtrByIndex(
    MMI_CTRL_ID_T ctrl_id,
    uint16        index  //from 0 to total_item_num-1
);


/***************************************************************************//*!
@brief ��õ�ǰ������Item����
@author xiaoqing.lu
@param ctrl_id  [in]    �ؼ�ID
@retval uint16
@return ����Item����
@note
*******************************************************************************/
#define GUILIST_GetCurItemIndex CTRLLIST_GetCurItemIndex

/***************************************************************************//*!
@brief ���õ�ǰ����������
@author  xiaoqing.lu
@param ctrl_id  [in]    �ؼ�ID
@param current_index    [in]    �����õ�������
@retval true/false
@return ���سɹ���ʧ��
@note
*******************************************************************************/
#define GUILIST_SetCurItemIndex CTRLLIST_SetCurItemIndex

/***************************************************************************//*!
@brief �ı���ʾ����
@author Guanyong.Zhang
@param ctrl_id  [in]    �ؼ�ID
@param type [in]    ��ʾ����
@param is_flash [in]    �Ƿ�����ˢ��
@retval true/false
@return ���سɹ���ʧ��
@note
*******************************************************************************/
#define GUILIST_ChangeDisplayType CTRLLIST_ChangeDisplayType


/***************************************************************************//*!
@brief ���һҳ��ʾ������
@author xiaoqing.lu
@param ctrl_id  [in]    �ؼ�ID
@retval uint16
@return ��������
@note
*******************************************************************************/
#define GUILIST_GetPageItemNum CTRLLIST_GetPageItemNum


/***************************************************************************//*!
@brief ��ȡ����Item����
@author xiaoqing.lu
@param ctrl_id  [in]    �ؼ�ID
@retval uint16
@return ����������
@note
*******************************************************************************/
#define GUILIST_GetTopItemIndex CTRLLIST_GetTopItemIndex

/***************************************************************************//*!
@brief ���ö��˵�Item
@author xiaoqing.lu
@param ctrl_id  [in]    �ؼ�ID
@param top_index    [in]    �����õ�Item
@retval true/false
@return ���سɹ���ʧ��
@note
*******************************************************************************/
#define GUILIST_SetTopItemIndex CTRLLIST_SetTopItemIndex

/***************************************************************************//*!
@brief I��ǰItem�Ƿ���Ч
@author Lianxiang
@param ctrl_id  [in]    �ؼ�ID
@retval true/false
@return ���سɹ���ʧ��
@note ��Ч��Item���һ���ʾ������ִ�а��������ʲ���
*******************************************************************************/
#define GUILIST_IsCurrItemInvalid CTRLLIST_IsCurrItemInvalid

/***************************************************************************//*!
@brief �Ƿ���ʾTitle��Ϣ
@author xiaoqing.lu
@param ctrl_id  [in]    �ؼ�ID
@param title_index_type [in]    Title��Ϣ��ʾ������
@retval void
@return void
@note
*******************************************************************************/
#define GUILIST_SetTitleIndexType CTRLLIST_SetTitleIndexType

/***************************************************************************//*!
@brief �Ƿ���Ҫ��ʾ������
@author xiaoqing.lu
@param ctrl_id  [in]    �ؼ�ID
@param is_need_prgbar   [in]    TRUE����ʾ��FALSE������ʾ
@retval void
@return void
@note
*******************************************************************************/
#define GUILIST_SetNeedPrgbar CTRLLIST_SetNeedPrgbar

/***************************************************************************//*!
@brief �������Item��
@author xiaoqing.lu
@param ctrl_id [in] �ؼ�ID
@retval uint16
@return ����������
@note
*******************************************************************************/
#define GUILIST_GetTotalItemNum CTRLLIST_GetTotalItemNum

/***************************************************************************//*!
@brief ���������ѡ��
@author hanbing.zhang
@param ctrl_id  [in]    �ؼ�ID
@param is_select_all    [in]    TRUE�����б�ѡ�У�FALSE��ȡ��ѡ������
@retval void
@return void
@note �������ѡ�����У���ô������������ӵ�һ����ʼѡ��
*******************************************************************************/
#define GUILIST_SetAllSelected CTRLLIST_SetAllSelected

/***************************************************************************//*!
@brief  ���ÿؼ��Ƿ���Ա��
@author hanbing.zhang
@param ctrl_id  [in]    �ؼ�ID
@param is_enable_mark   [in]    TRUE�����Ա���ǣ�FALSE�����ܱ����
@retval true/false
@return ���سɹ���ʧ��
@note
*******************************************************************************/
#define GUILIST_SetTextListMarkable CTRLLIST_SetTextListMarkable

/***************************************************************************//*!
@brief  ���ÿؼ�Itemǰ�����ֵ���ʼֵ
@author James.Zhang
@param ctrl_id  [in]    �ؼ�ID
@param num_start_index  [in]    ��ʼֵ
@retval void
@return void
@note ϵͳֻ����1-20�����ֵ�ͼƬ��ԴID��list�ؼ�ʹ��
*******************************************************************************/
#define GUILIST_SetNumberListStartIndex CTRLLIST_SetNumberListStartIndex

/***************************************************************************//*!
@brief ����Softkey�Ƿ���Ӧ�ÿ�����ʾ
@author hanbing.zhang
@param ctrl_id  [in]    �ؼ�ID
@param is_own_softkey   [in]    TRUE:�����Զ���softkey��FALSE:ʹ�ÿؼ�softkey
@retval void
@return void
@note
*******************************************************************************/
#define GUILIST_SetOwnSofterKey CTRLLIST_SetOwnSofterKey

/*****************************************************************************/
//  Description : ���ÿؼ��Ƿ���Ҫ�ָ���
//  Global resource dependence :
//  Author:xiaoqing.lu
//  Note:
/*****************************************************************************/
#define GUILIST_SetNeedSplidLine CTRLLIST_SetNeedSplidLine

/*****************************************************************************//*!
@brief Set the item state
@author xiaoqing.lu
@param ctrl_id  [in]    �ؼ�ID
@param item_index [in]      item ����
@param src_state [in]       Ҫ���õ�״̬
@retval true/false
@return TRUE�����óɹ�����ʧ��
@note
*****************************************************************************/
#define GUILIST_SetItemStateById CTRLLIST_SetItemStateById

/***************************************************************************//*!
@brief ���ÿؼ���״̬
@author James.Zhang
@param list_ptr [in]    �ؼ�ָ��
@param src_state    [in]    Ҫ���õ�״̬
@param is_true  [in]     ����/ȡ������
@retval void
@return void
@note
*******************************************************************************/
#define GUILIST_SetListState CTRLLIST_SetListState

/***************************************************************************//*!
@brief ��ÿؼ���ĳ��״̬�Ƿ�����
@author James.Zhang
@param list_ptr [in]    �ؼ�ָ��
@param src_state    [in]    Ҫ��õ�״̬
@retval true/false
@return TRUE���ؼ����и�״̬��FALSE���ؼ�û�и�״̬
@note
*******************************************************************************/
#define GUILIST_GetListState CTRLLIST_GetListState

/***************************************************************************//*!
@brief ��ÿؼ���ĳ��״̬�Ƿ�����
@author James.Zhang
@param ctrl_id  [in]    �ؼ�ID
@param src_state [in]       Ҫ��õ�״̬
@retval true/false
@return TRUE���ؼ����и�״̬��FALSE���ؼ�û�и�״̬
@note
*******************************************************************************/
#define GUILIST_GetListStateById CTRLLIST_GetListStateById

/***************************************************************************//*!
@brief  ��ÿؼ�����û��Զ�������
@author peng.chen
@param iguictrl_ptr [in]    �ؼ�ָ��
@param index     [in]   Ҫ��õ�Item����
@param user_data_ptr    [in]    �û����ݣ�����Ϊ�κ����ͣ��û��Զ���
@retval true/false
@return ���سɹ���ʧ��
@note
*******************************************************************************/
#define GUILIST_GetItemData CTRLLIST_GetItemData

/***************************************************************************//*!
@brief  ���ÿؼ�����û��Զ�������
@author peng.chen
@param iguictrl_ptr [in]    �ؼ�ָ��
@param index     [in]   Ҫ��õ�Item����
@param user_data_ptr    [in]    �û����ݣ�����Ϊ�κ����ͣ��û��Զ���
@retval true/false
@return ���سɹ���ʧ��
@note
*******************************************************************************/
#define GUILIST_SetItemUserData CTRLLIST_SetItemUserData

/***************************************************************************//*!
@brief ������ǰItem����
@author xiaoqing.lu
@param ctrl_id [in] �ؼ�ID
@retval void
@return void
@note ��ɾ��ĳһ����ʱ�����ܴ��ڵ�ǰItem���ô���������ͨ���ýӿڿ��Խ���
*******************************************************************************/
#define GUILIST_ValidCurItemPos CTRLLIST_ValidCurItemPos

/***************************************************************************//*!
@brief ���ÿؼ��ı���ɫ
@author James.Zhang
@param ctrl_id [in] �ؼ�ID
@param back_color [in]  ����ɫ
@retval true/false
@return ���سɹ���ʧ��
@note
*******************************************************************************/
#define GUILIST_SetBgColor CTRLLIST_SetBgColor

/***************************************************************************//*!
@brief ����List�ؼ���Ĭ��ͼƬID
@author xiaoqing.lu
@param ctrl_id [in] �ؼ�ID
@param def_img_id [in]  ��������е�Ĭ��ͼƬ
@param err_img_id [in]  ����ʧ�ܺ��Ĭ��ͼƬ
@retval true/false
@return ���سɹ���ʧ��
@note ����Ĭ��ͼƬ����Ϊ��̬����ͼƬʱ����Ƕ�Ķ����ؼ�����
*******************************************************************************/
#define GUILIST_SetDefaultIcon CTRLLIST_SetDefaultIcon

/***************************************************************************//*!
@brief ����Item�Ƿ���ʾ����Ч��
@author xiaoqing.lu
@param ctrl_id [in] �ؼ�ID
@param item_index [in] �ؼ�����
@param is_misted [in] �Ƿ��б���Ч��
@retval true/false
@return ��������֮ǰ��״̬
@note
*******************************************************************************/
#define GUILIST_SetItemMisted CTRLLIST_SetItemMisted


/***************************************************************************//*!
@brief �����Ƿ���Ҫ��ʾ������
@author xiaoqing.lu
@param ctrl_id [in] �ؼ�ID
@param is_true [in] TRUEҪ��ʾ��������FALSE��Ҫ��ʾ��Ĭ��ΪTRUE
@note
*******************************************************************************/
#define GUILIST_SetNeedHiLightBar CTRLLIST_SetNeedHiLightBar


/***************************************************************************//*!
@brief  ���ÿ��б���Ϣ
@author James.Zhang
@param ctrl_id [in] �ؼ�ID
@param empty_ptr [in] ���б���Ϣ
@retval void
@return void
@note ���ڴ���open��Ϣʱ����
*******************************************************************************/
#define GUILIST_SetEmptyInfo CTRLLIST_SetEmptyInfo

/***************************************************************************//*!
@brief �����Ƿ���ʾ�������Ļ���
@author xiaoqing.lu
@param ctrl_id [in] �ؼ�ID
@param is_true [in] TRUE��ʾ��FALSE����ʾ��Ĭ��ΪTRUE
@retval void
@return void
@note ����ӿ������Ƿ���Ҫ�������ϵĻ���
*******************************************************************************/
#define GUILIST_SetNeedPrgbarBlock CTRLLIST_SetNeedPrgbarBlock


/***************************************************************************//*!
@brief ���ñ���ͼƬ��ԴID
@author xiaoqing.lu
@param ctrl_id [in] �ؼ�ID
@param image_id [in] ͼƬ��ԴID
@param is_transparent [in] �Ƿ�͸����ʾ
@retval true/false
@return ���سɹ���ʧ��
@note
*******************************************************************************/
#define GUILIST_SetBgImage CTRLLIST_SetBgImage

/*****************************************************************************/
//  Description : set the user bg
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
#define GUILIST_SetUserBg CTRLLIST_SetUserBg

/***************************************************************************//*!
@brief ������ʾ������ʱ���Ƿ�ͬ�����룬Ĭ�����첽����
@author xiaoqing.lu
@param ctrl_id [in] �ؼ�ID
@param is_sync_decode [in] TRUE��ͬ����FALSE���첽
@retval true/false
@return ��������֮ǰ��״̬
@note ��������֮ǰ��״̬
*******************************************************************************/
#define GUILIST_SetSyncDecodeAnim CTRLLIST_SetSyncDecodeAnim

/***************************************************************************//*!
@brief �����Ƿ�ѭ������
@author xiaoqing.lu
@param ctrl_id [in] �ؼ�ID
@param is_cyc_scroll [in] TRUE��ѭ����FALSE����ѭ��
@note Ĭ��ʱѭ�����������ؼ���ѭ������������������/����ʱ���ᷢ����Ϣ��������
*******************************************************************************/
#define GUILIST_SetCycScroll CTRLLIST_SetCycScroll

/***************************************************************************//*!
@brief �����Ƿ���Ӧ���Ұ���
@author James.Zhang
@param ctrl_id [in] �ؼ�ID
@param is_handle [in] TRUE����Ӧ��FALSE������Ӧ
@note Ĭ�ϲ���Ӧ���Ұ���
*******************************************************************************/
#define GUILIST_SetHandleLeftRight CTRLLIST_SetHandleLeftRight

/***************************************************************************//*!
@brief �����Ƿ�������ʾ�߿�
@author Jassmine
@param ctrl_id      [in] �ؼ�ID
@param is_permit    [in] �Ƿ�������ʾ
@return �����Ƿ�ɹ�
@note
*******************************************************************************/
#define GUILIST_PermitBorder CTRLLIST_PermitBorder

/***************************************************************************//*!
@brief ���ÿؼ����������ɫ
@author James.Zhang
@param ctrl_id [in] �ؼ�ID
@param font [in] ����
@param font_color [in] ������ɫ
@retval true/false
@return ���سɹ���ʧ��
@note  Ĭ�ϲ���Ҫ����.��GUILIST_STATE_TEXT_STYLE״̬ͬʱʹ��
*******************************************************************************/
#define GUILIST_SetTextFont CTRLLIST_SetTextFont

/*****************************************************************************//*!
@brief ����list text font color
@author xiaoqing.lu
@param ctrl_id  [in]    �ؼ�ID
@param font_color [in]      item ������ɫ
@retval true/false
@return TRUE�����óɹ�����ʧ��
@note Ĭ�ϲ���Ҫ����, ��GUIITEM_STATE_COSTMER_FONT_COLOR״̬ͬʱʹ��
*****************************************************************************/
#define GUILIST_SetItemTextFontColor CTRLLIST_SetItemTextFontColor

/***************************************************************************//*!
@brief ���ÿؼ�����
@author Jassmine
@param ctrl_handle [in] �ؼ�HANDLE
@param rect_ptr [in] ����
@retval void
@return void
@note
*******************************************************************************/
#define GUILIST_SetRect CTRLLIST_SetRect

/***************************************************************************//*!
@brief ���ÿؼ��Ƿ�֧��*,\#�л�ҳ
@author Jassmine
@param ctrl_handle [in] �ؼ�HANDLE
@param is_handle [in] TRUE:֧�֣�FALSE:��֧��
@retval true/false
@return ���سɹ���ʧ��
@note Ĭ��֧��
*******************************************************************************/
#define GUILIST_SetHandleStarHash CTRLLIST_SetHandleStarHash

/***************************************************************************//*!
@brief �����Ƿ�֧��slide״̬
@author Xiaoqing.Lu
@param ctrl_id [in] �ؼ�HANDLE
@param is_slide_used [in] TRUE:֧�֣�FALSE:��֧��
@retval true/false
@return ���سɹ���ʧ��
@note  Ĭ�����������֧��slide״̬
*******************************************************************************/
#define GUILIST_SetSlideState CTRLLIST_SetSlideState

/***************************************************************************//*!
@brief ��ȡ�Ƿ�֧��slide״̬
@author Xiaoming.Ren
@param ctrl_id [in] �ؼ�HANDLE
@retval true/false
*******************************************************************************/
#define GUILIST_GetSlideState CTRLLIST_GetSlideState

/***************************************************************************//*!
@brief ���ö���Item��ƫ��
@author bin.ji
@param ctrl_id [in] �ؼ�ID
@param offset_y [in] Y�����ƫ��
@retval void
@return void
@note
*******************************************************************************/
#define GUILIST_SetTopItemOffset CTRLLIST_SetTopItemOffset

/***************************************************************************//*!
@brief get top item offset
@author xiaoqinglu
@param ctrl_id [in] �ؼ�ID
@param offset_y_ptr [in/out] Y�����ƫ��
@retval true/false
@return ���سɹ���ʧ��
@note
*******************************************************************************/
#define GUILIST_GetTopItemOffset CTRLLIST_GetTopItemOffset

/***************************************************************************//*!
@brief �����Ƿ�֧��3d
@author James.Zhang
@param ctrl_id [in] �ؼ�ID
@param is_disable_3d [in] TRUE֧�֣�FALSE��֧��
@retval true/false
@return ���سɹ���ʧ��
@note Ĭ��֧��
*******************************************************************************/
#define GUILIST_SetDisable3D CTRLLIST_SetDisable3D

/***************************************************************************//*!
@brief ����Item���
@author xiaoqing.lu
@param ctrl_id [in] �ؼ�ID
@param item_style [in] ���
@param item_index [in] �����õ�����
@retval true/false
@return ���سɹ���ʧ��
@note
*******************************************************************************/
PUBLIC BOOLEAN GUILIST_SetItemStyle(
    MMI_CTRL_ID_T     ctrl_id, // control id
    GUIITEM_STYLE_E item_style,
    uint16          item_index
);

/*****************************************************************************/
//  Description :
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC void GUILIST_SetItemStyleEx(
    MMI_CTRL_ID_T     ctrl_id, // control id
    uint16          item_index,
    THEMELIST_ITEM_STYLE_T *item_style_ptr
);

/*****************************************************************************/
//  Description : set scroll bar style
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
#define GUILIST_SetListScrollBarStyle CTRLLIST_SetListScrollBarStyle

/***************************************************************************//*!
@brief ���ñ߾�
@author xiaoqing.lu
@param ctrl_id [in] �ؼ�ID
@param left_space [in] ��߾�
@param right_space [in] �ұ߾�
@retval true/false
@return ���سɹ���ʧ��
@note
*******************************************************************************/
#define GUILIST_SetMargin CTRLLIST_SetMargin

/***************************************************************************//*!
@brief ������Ƕ�������ı���ͼ
@author xiaoqing.lu
@param ctrl_id [in] �ؼ�ID
@param image_id [in] ��ԴͼƬID
@retval true/false
@return ���سɹ���ʧ��
@note
*******************************************************************************/
#define GUILIST_SetPrgBarImageId CTRLLIST_SetPrgBarImageId

/***************************************************************************//*!
@brief ����Item�Ƿ�һ�
@author xiaoqing.lu
@param ctrl_id [in] �ؼ�ID
@param item_index [in] ����
@param is_gray [in] �Ƿ�һ�
@retval true/false
@return ���سɹ���ʧ��
@note �����ûһ���Item������ִ�а��������ʲ���
*******************************************************************************/
#define GUILIST_SetItemGray CTRLLIST_SetItemGray

/***************************************************************************//*!
@brief ����softkey������
@author 
@param ctrl_id [in] �ؼ�ID
@param 
@param 
@retval 
@note ��Ҫ֧��softkey�ĸ��������ʾ
*******************************************************************************/
#define GUILIST_SetSoftkeyType CTRLLIST_SetSoftkeyType

/***************************************************************************//*!
@brief ����softkey
@author 
@param ctrl_id [in] �ؼ�ID
@param 
@param 
@param 
@param 
@retval true/false
@return ���سɹ���ʧ��
@note
*******************************************************************************/
#define GUILIST_SetItemSoftkey CTRLLIST_SetItemSoftkey

/***************************************************************************//*!
@brief ����ITEM�Ƿ���Ч
@author xiaoqing.lu
@param ctrl_id [in] �ؼ�ָ��
@param pos [in] ����
@param is_invalid [in] �Ƿ���Ч
@retval true/false
@return ���سɹ���ʧ��
@note ��������Ч��Item������ִ�а��������ʲ���
*******************************************************************************/
#define GUILIST_SetItemInvalid CTRLLIST_SetItemInvalid

/***************************************************************************//*!
@brief ���Item����
@author xiaoqing.lu
@param ctrl_id [in] �ؼ�ID
@param item_index [in]  ����
@param rect_ptr [in/out] ���ε�ָ��
@retval true/false
@return ���سɹ���ʧ��
@note Item���������ʱ����ı�
*******************************************************************************/
#define GUILIST_GetItemRect CTRLLIST_GetItemRect

/***************************************************************************//*!
@brief �ı��ѡ���Ĭ��softkey ID
@author xiaoqing.lu
@param ctrl_id [in] �ؼ�ID
@param text_cancel_id  [in] TEXT ID
@param text_select_id [in] TEXT ID
@param text_return_id [in] TEXT ID
@retval void
@return void
@note
*******************************************************************************/
#define GUILIST_ChangeCheckListDefSoftkeyID CTRLLIST_ChangeCheckListDefSoftkeyID

/***************************************************************************//*!
@brief �ı䵥ѡ���Ĭ��softkey ID
@author xiaoqing.lu
@param ctrl_id [in] �ؼ�ID
@param text_ok_id [in] TEXT ID
@param text_return_id  [in/out] TEXT ID
@retval void
@return void
@note
*******************************************************************************/
#define GUILIST_ChangeRadioListDefSoftkeyID CTRLLIST_ChangeRadioListDefSoftkeyID

/*****************************************************************************/
//  Description : start auto switch timer
//  Global resource dependence :
//  Author:xiaoqing.lu
//  Note:
/*****************************************************************************/
#define GUILIST_SetSwitchItemTime CTRLLIST_SetSwitchItemTime

/*****************************************************************************/
//  Description : start auto switch timer
//  Global resource dependence :
//  Author:xiaoqing.lu
//  Note:
/*****************************************************************************/
#define GUILIST_StartSwitchItem CTRLLIST_StartSwitchItem

/*****************************************************************************/
//  Description : stop auto switch timer
//  Global resource dependence :
//  Author:xiaoqing.lu
//  Note:
/*****************************************************************************/
#define GUILIST_StopSwitchItem CTRLLIST_StopSwitchItem

/*****************************************************************************/
//  Description : �����Ƿ��Զ��л�item
//  Global resource dependence :
//  Author:xiaoqing.lu
//  Note: Ĭ��ʱ���Զ��л�
/*****************************************************************************/
#define GUILIST_SetAutoSwitchItem CTRLLIST_SetAutoSwitchItem

/*****************************************************************************/
//  Description : ����ܵ�ҳ��
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note: ÿҳ��ʾ����Ŀ����list����item�߶ȼ�������
/*****************************************************************************/
#define GUILIST_GetTotalPageNum CTRLLIST_GetTotalPageNum

/*****************************************************************************/
//  Description : ��õ�ǰҳ������
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note: �Ե�ǰҳ��������һ��ItemΪ׼����
/*****************************************************************************/
#define GUILIST_GetCurrentPageIndex CTRLLIST_GetCurrentPageIndex

/***************************************************************************//*!
@brief ��ȡ������item index
@author lianxiang.zhou
@param ctrl_id [in] �ؼ�ID
@param gross_index  [in] TEXT ID
@param pure_index_ptr [in] TEXT ID
@retval true/false
@return �����Ƿ����˴�����index
@note ��ν�����ģ�������split���
*******************************************************************************/
#define GUILIST_GetPureIndex CTRLLIST_GetPureIndex

/***************************************************************************//*!
@brief �����Ƿ�֧�ַָ�������Ч����Ĭ�ϲ�֧��
@author xiaoqing.lu
@param ctrl_id [in] �ؼ�ID
@param is_true  [in] ���û�ȡ������
@retval true/false
@return
@note
*******************************************************************************/
#define GUILIST_SetSplitOnside CTRLLIST_SetSplitOnside

/***************************************************************************//*!
@brief ���ÿؼ��������������ڴ�
@author Xiaoqing.lu
@param base_win_handle  [in] Must valid win handle, if not, create failed
@param base_ctrl_handle [in] You can set 0
@param ctrl_id          [in] You can set 0
@param init_data_ptr    [in] if null, will create ctrl by default param
@retval 0 or valid handle
@return ����0������Ч�Ŀؼ�ID
@note ע�⣬����ʼ������init_data_ptrΪ��ʱ��������һ���յĿؼ����ÿؼ��������Item
*******************************************************************************/
#define GUILIST_CreateListBox CTRLLIST_CreateListBox


/***************************************************************************//*!
@brief ���ÿؼ��������������ڴ�
@author Xiaoqing.lu
@param ctrl_id          [in] ctrl ID
@retval true or false
@return
@note
*******************************************************************************/
#define GUILIST_IsTpDown CTRLLIST_IsTpDown

/***************************************************************************//*!
@brief ���ÿؼ��������������ڴ�
@author Xiaoqing.lu
@param ctrl_id          [in] ctrl ID
@retval item number
@return item number
@note
*******************************************************************************/
#define GUILIST_GetSelectedItemNum CTRLLIST_GetSelectedItemNum

/*****************************************************************************/
//  Description : �����Ƿ����û�built item layer
//  Global resource dependence :
//  Author:xiaoqing.lu
//  Note: Ĭ��FALSE,�ؼ��Զ�built
/*****************************************************************************/
#define GUILIST_SetCusBltItemLayer CTRLLIST_SetCusBltItemLayer

/*****************************************************************************//*!
@brief ���ÿؼ���Item��ʾ��layer
@author Xiaoqing.lu
@param ctrl_id              [in] ctrl ID
@param lcd_dev_info_ptr     [in] lcd_dev_info_ptr ID
@retval item number
@return item number
@note
*****************************************************************************/
#define GUILIST_SetItemLayer CTRLLIST_SetItemLayer

/*****************************************************************************//*!
@brief ���ÿؼ���Item��ʾ��layer
@author Xiaoqing.lu
@param ctrl_id              [in] ctrl ID
@param get_data_func     [in] func ptr
@retval true/false
@return true/false
@note
*****************************************************************************/
#define GUILIST_RegGetDataFunc CTRLLIST_RegGetDataFunc

/*****************************************************************************/
//  Description : set the state
//  Global resource dependence :
//  Author:James.Zhang
//  Note:����list֧�ֺ��򻬶���Ĭ�ϲ�֧��
/*****************************************************************************/
#define GUILIST_SetItemHorMove CTRLLIST_SetItemHorMove

/*****************************************************************************/
//  Description : GUILIST_SetItemLayer
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
#define GUILIST_SetItemLayer CTRLLIST_SetItemLayer

/***************************************************************************//*!
@brief ���ý�ֹtip��ʾ��״̬
@author Lianxiang.Zhou
@param ctrl_id       [in] ctrl ID
@param is_forbid     [in] �Ƿ��ֹtip
@retval true/false
@return true/false
@note Ĭ������ʾ��
*******************************************************************************/
#define GUILIST_SetForbidTip CTRLLIST_SetForbidTip

/***************************************************************************//*!
@brief ���Listλ����Ϣ
@author xiaoqing.lu
@param ctrl_id  [in]    �ؼ�ID
@retval
@return
@note
*******************************************************************************/
#define GUILIST_GetCurPosInfo CTRLLIST_GetCurPosInfo

/***************************************************************************//*!
@brief ����Listλ����Ϣ
@author xiaoqing.lu
@param ctrl_id  [in]    �ؼ�ID
@retval
@return
@note
*******************************************************************************/
#define GUILIST_SetCurPosInfo CTRLLIST_SetCurPosInfo

/*****************************************************************************/
//  Description : set check list default softkey id
//  Global resource dependence :
//  Author:xiaoqing.lu
//  Note:
/*****************************************************************************/
#define GUILIST_ChangeCheckListSoftkeyIndex CTRLLIST_ChangeCheckListSoftkeyIndex
/***************************************************************************//*!
@brief set if need custom right softkey id when at latest 1 is marked
@author yanyan.an
@param ctrl_id                  [in]    �ؼ�ID
@param is_custom_right_softkey  [in]    �Ƿ���Ҫ���������������ʾͼ����ı�
@retval
@return
@note
*******************************************************************************/
#define GUILIST_SetNeedCustomRightSK CTRLLIST_SetNeedCustomRightSK

/***************************************************************************//*!
@brief set check list right softkey id if at least 1 is marked
@author yanyan.an
@param ctrl_id          [in]    �ؼ�ID
@param text_marked_id   [in]    selected num����0��״̬�£��Ҽ���ʾ���ı�
@param image_marked_id  [in]    selected num����0��״̬�£��Ҽ���ʾ��ͼ��
@retval
@return
@note
*******************************************************************************/
#define GUILIST_ChangeCheckListRightSoftkey CTRLLIST_ChangeCheckListRightSoftkey

/***************************************************************************//*!
@brief set quick action softkey id when in left focus
@author yanyan.an
@param ctrl_id              [in]    �ؼ�ID
@param index                [in]    ������ 
@param lfocus_left_sk_id    [in]    ������left focus״̬�£������ʾ���ı�   
@param lfocus_mid_sk_id     [in]    ������left focus״̬�£��м����ʾ���ı� 
@param lfocus_right_sk_id   [in]    ������left focus״̬�£��Ҽ���ʾ���ı�
@retval
@return
@note
*******************************************************************************/
#define GUILIST_ChangeLeftQuickSoftkey CTRLLIST_ChangeLeftQuickSoftkey
/***************************************************************************//*!
@brief set quick action softkey id when in right focus
@author yanyan.an
@param ctrl_id              [in]    �ؼ�ID
@param index                [in]    ������ 
@param rfocus_left_sk_id    [in]    ������right focus״̬�£������ʾ���ı�   
@param rfocus_mid_sk_id     [in]    ������right focus״̬�£��м���ʾ���ı� 
@param rfocus_right_sk_id   [in]    ������right focus״̬�£��Ҽ���ʾ���ı�
@retval
@return
@note
*******************************************************************************/
#define GUILIST_ChangeRightQuickSoftkey CTRLLIST_ChangeRightQuickSoftkey

/*****************************************************************************/
//  Description : Get the item state
//  Global resource dependence :
//  Author:James.Zhang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN GUILIST_GetItemState(
    const GUILIST_ITEM_T *item_ptr,
    GUIITEM_STATE_T src_state
);

/*****************************************************************************/
//  Description : set the item state
//  Global resource dependence :
//  Author:James.Zhang
//  Note:
/*****************************************************************************/
PUBLIC void GUILIST_SetItemState(
    GUILIST_ITEM_T *item_ptr,
    GUIITEM_STATE_T  src_state,
    BOOLEAN is_true
);

/*****************************************************************************/
//  Description :
//  Global resource dependence :
//  Author:
//  Note: Ϊ�˼���
/*****************************************************************************/
PUBLIC const THEMELIST_ITEM_STYLE_T *GUILIST_GetItemStyle(
    MMI_CTRL_ID_T ctrl_id,
    uint16        index  //from 0 to total_item_num-1
);

/*****************************************************************************/
//  Description :
//  Global resource dependence :
//  Author:
//  Note: Ϊ�˼���
/*****************************************************************************/
PUBLIC const uint32 GUILIST_GetItemStyleID(
    MMI_CTRL_ID_T ctrl_id,
    uint16        index  //from 0 to total_item_num-1
);

/*Microsoft project need to support+++*/
/*****************************************************************************//*!
@brief ����list item content Custom font color
@author 
@param ctrl_id          [in]  �ؼ�ID
@param content_index    [in]  ������������
@param font_color       [in]  item ������ɫ
@param alpha            [in]  normal״̬�µ�������ɫ͸����
@param font_color_focus [in]  item focused ������ɫ
@param alpha_focus      [in]  focused״̬�µ�������ɫ͸����
@retval true/false
@return TRUE�����óɹ�����ʧ��
@note Ĭ�ϲ���Ҫ����, ��GUIITEM_STATE_COSTMER_FONT_COLOR״̬ͬʱʹ�ã�ר��΢����Ŀʹ�� 
*****************************************************************************/
#define GUILIST_SetItemContentCustomFontColor CTRLLIST_SetItemContentCustomFontColor
/*Microsoft project need to support+++*/
/*****************************************************************************//*!
@brief ����list item content Opacity font color
@author 
@param ctrl_id          [in]  �ؼ�ID
@param content_index    [in]  ������������
@param alpha            [in]  normal״̬�µ�������ɫ͸����
@param alpha_focus      [in]  focused״̬�µ�������ɫ͸����
@retval true/false
@return TRUE�����óɹ�����ʧ��
@note Ĭ�ϲ���Ҫ���� 
*****************************************************************************/
#define GUILIST_SetItemContentOpacityFontColor CTRLLIST_SetItemContentOpacityFontColor
/***************************************************************************//*!
@brief set quick icon
@author Yanyan.an
@param ctrl_id      	    [in]     �ؼ�ID
@param left_focus_image     [in]     ��fucosͼ��
@param right_focus_image    [in]     ��fucosͼ��
@note
*******************************************************************************/
#define GUILIST_SetQuickActionIcon CTRLLIST_SetQuickActionIcon

/***************************************************************************//*!
@brief set quick icon margin, width
@author Yanyan.an
@param ctrl_id      	    [in]     �ؼ�ID
@param margin               [in]     quickͼƬ�ı߾�
@param width                [in]     quickͼƬ�Ŀ��
@note
*******************************************************************************/
#define GUILIST_SetQuickActionIconMarginAndWidth CTRLLIST_SetQuickActionIconMarginAndWidth
/***************************************************************************//*!
@brief set avatar and initial mask icon's image_id, left_margin, icon_width
@author Yanyan.an
@param ctrl_id      	            [in]     �ؼ�ID
@param avatar_mask_img_id           [in]     avatar maskͼƬ
@param avatar_mask_left_margin      [in]     avatar maskͼƬ����߾�
@param avatar_mask_width            [in]     avatar maskͼƬ�Ŀ��
@param initial_mask_img_id          [in]     initial maskͼƬ
@param initial_mask_left_margin     [in]     initial maskͼƬ����߾�
@param initial_mask_width           [in]     initial maskͼƬ�Ŀ��
@note
*******************************************************************************/
#define GUILIST_SetAvatarMaskIconParam CTRLLIST_SetAvatarMaskIconParam

/***************************************************************************//*!
@brief set item left & right shortcut indicator icon
@author Yanyan.an
@param ctrl_id      	       [in]     �ؼ�ID
@param left_shortcut_image     [in]     ��ָʾͼ��
@param right_shortcut_image    [in]     ��ָʾͼ��
@note
*******************************************************************************/
#define GUILIST_SetShortcutIndicatorIcon CTRLLIST_SetShortcutIndicatorIcon

/***************************************************************************//*!
@brief  set quick sim select icon
@author Yanyan.an
@param  ctrl_id              [in]     �ؼ�ID
@param  simsel_image         [in]     simSelͼ��
@note
*******************************************************************************/
#define GUILIST_SetQuickSimSelIcon CTRLLIST_SetQuickSimSelIcon
/***************************************************************************//*!
@brief  set list if show left right triangle icon
@author Yanyan.an
@param  ctrl_id      	    [in]     �ؼ�ID
@param  is_show_left_side   [in]     focus״̬���Ƿ���ʾ������ͼ��
@param  is_show_right_side  [in]     focus״̬���Ƿ���ʾ������ͼ��
@note   only used in focus state
*******************************************************************************/
#define GUILIST_SetShowLRTriangleIcon CTRLLIST_SetShowLRTriangleIcon

/***************************************************************************//*!
@brief  set item show left right triangle icon status
@author Yanyan.an
@param  ctrl_id      	     [in]   �ؼ�ID
@param  index  	             [in]	����
@param  is_show_left_side    [in]   focus״̬���Ƿ���ʾ������ͼ��
@param  is_show_right_side   [in]   focus״̬���Ƿ���ʾ������ͼ��
@note   only used in focus state
*******************************************************************************/
#define GUILIST_SetItemShowLRTriangleIconByIndex CTRLLIST_SetItemShowLRTriangleIconByIndex

/***************************************************************************//*!
@brief  set item if handle left key , right key
@author Yanyan.an
@param  ctrl_id      	[in]     �ؼ�ID
@param  index  	        [in]	 ����
@param  is_handle_left  [in]     �Ƿ���Ӧ���
@param  is_handle_right [in]     �Ƿ���Ӧ�Ҽ�
@note   only used in focus state
*******************************************************************************/
#define GUILIST_SetItemHandleLeftRightByIndex CTRLLIST_SetItemHandleLeftRightByIndex

/***************************************************************************//*!
@brief  set item quick icon
@author Yanyan.an
@param  ctrl_id      	    [in]     �ؼ�ID
@param  index  	            [in]	 ����
@param  left_quick_image    [in]     item left quick image
@param  right_quick_image   [in]     item right quick image
@note   only used in focus state
/*****************************************************************************/
#define GUILIST_SetItemQuickIconByIndex CTRLLIST_SetItemQuickIconByIndex

/***************************************************************************//*!
@brief   set item accessory image
@author 
@param  ctrl_id      	    [in]     �ؼ�ID
@param  index  	            [in]	 ����
@param  image_accessory_id  [in]     item accessory image
@note   
/*****************************************************************************/
#define  GUILIST_SetItemAccessoryImageByIndex CTRLLIST_SetItemAccessoryImageByIndex


#define    GUILIST_SetCurItemLeftFocus  CTRLLIST_SetCurItemLeftFocus
/***************************************************************************//*!
@brief  set list if handle left key , right key
@author Yanyan.an
@param  ctrl_id      	[in]     �ؼ�ID
@param  is_handle_left  [in]     �Ƿ���Ӧ���
@param  is_handle_right [in]     �Ƿ���Ӧ�Ҽ�
@note   only used in focus state
*******************************************************************************/
#define GUILIST_SetHandleLeftRightEx CTRLLIST_SetHandleLeftRightEx
/***************************************************************************//*!
@brief  set list if show toggle icon
@author Yanyan.an
@param  ctrl_id      	[in]     �ؼ�ID
@param  item_index      [in]     �ؼ�������
@param  is_on           [in]     �Ƿ���
@note   only used in focus state
*******************************************************************************/
#define GUILIST_SetToggleState CTRLLIST_SetToggleState
/***************************************************************************//*!
@brief  set list if show toggle icon
@author Yanyan.an
@param  ctrl_id      	[in]     �ؼ�ID
@param  item_index      [in]     �ؼ�������
@param  is_on           [out]     �Ƿ���
@note   only used in focus state
*******************************************************************************/
#define GUILIST_GetToggleState CTRLLIST_GetToggleState
/***************************************************************************//*!
@brief  set list accessory iamge& on item location
@param  ctrl_id      	[in]     �ؼ�ID
@param  is_accessory_iamge      [in]  �Ƿ���Ҫ����ͼƬ
@param  iamge_accessory_id           [in]     ͼƬID
@param  iamge_accessory_index           [in]     ͼƬ���λ��
@note   only used in focus state
*******************************************************************************/
#define  GUILIST_SetListIsNeedAccessoryImage CTRLLIST_SetListIsNeedAccessoryIamge
/*Microsoft project need to support---*/

/*----------------------------------------------------------------------------*/
/*                         Compiler Flag                                      */
/*----------------------------------------------------------------------------*/
#ifdef   __cplusplus
}
#endif

#endif /* END _GUILISTBOX_H_  */


/*@}*/


