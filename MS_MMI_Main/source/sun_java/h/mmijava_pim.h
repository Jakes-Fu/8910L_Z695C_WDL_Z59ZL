/*****************************************************************************
** File Name:      mmijava_video.h                                          *
** Author:         hui.zhao                                               *
** Date:           6/2009                                                    *
** Copyright:      2009 Spreadtrum, Incorporated. All Rights Reserved.       *
** Description:                       *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 6/2009       hui.zhao            Create                                *
******************************************************************************/
#ifndef _MMIJAVA_PIM_H_
#define _MMIJAVA_PIM_H_

#include "sci_types.h"

typedef enum
{
   MMIJAVA_PIM_VCARD = 0,
   MMIJAVA_PIM_EVENT = 1,
   MMIJAVA_PIM_TODO = 2,
} MMIJAVA_PIM_TYPE_E;

#define MMIJAVA_PIM_SUCCESS 0
#define MMIJAVA_PIM_FAILE   -1
#define MMIJAVA_PIM_PENDING -2


#define PIM_DEBUG javacall_printf
/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/

/*******************************************************************************/
// Description :�����һ��vCard/vCalendar��Ŀ
// Parameter: type      [IN] 0����vCard, 1����Event��2����Todo��
//            lastItemId[IN] �ϴζ�����Ŀ��uuid, -1��ʾ��ͷ��ʼ��ȡ��
//            uuid_ptr      [OUT] vCard/vCalendar��Ŀ��uuid��
//            item_ptr      [IN/OUT]���ڱ���UTF-8�����vCard/vCalendar��Ŀ���ݣ�����
//                           ����vCard2.1/vCalendar 1.0�淶�Ĺ涨��item�Ĵ洢�ռ���Java���䣻
//            maxItemLen ��IN�� Java�����item�洢�ռ�Ĵ�С��
//
// Return : 0����ɹ�; -1������������Ѿ�����vCard/vCalendar��ĩβ�����ζ�ȡû�л�
//          ��vCard/vCalendar��Ŀ���ݡ�
/*****************************************************************************/
PUBLIC int32 MMIJAVA_PIM_Get_Next_Item(MMIJAVA_PIM_TYPE_E type,
									   int32             lastItemId,
									   int32             *uuid_ptr, 
									   uint8              *item_ptr,
									   uint32             maxItemLen
									   );

/*******************************************************************************/
// Description :�޸�һ��vCard/vCalendar��Ŀ
// Parameter: type [IN] 0����vCard, 1����Event��2����Todo
//            uuid [IN]vCard/vCalendar��Ŀ��uuid��
//            item_ptr[IN/OUT]���ڱ���UTF-8�����vCard/vCalendar��Ŀ���ݣ�����
//                           ����vCard2.1/vCalendar 1.0�淶�Ĺ涨��item�Ĵ洢�ռ���Java���䣻
//
// Return : 0����ɹ�; -1�������
/*****************************************************************************/
PUBLIC int32  MMIJAVA_PIM_Modify_Item(MMIJAVA_PIM_TYPE_E type, 
									  int32 uuid, 
									  uint8 *item_ptr
									  );

/*******************************************************************************/
// Description :���һ��vCard/vCalendar��Ŀ
// Parameter: type [IN] 0����vCard, 1����Event��2����Todo
//            uuid_ptr [OUT]vCard/vCalendar��Ŀ��uuid��
//            item_ptr[IN/OUT]���ڱ���UTF-8�����vCard/vCalendar��Ŀ���ݣ�����
//                           ����vCard2.1/vCalendar 1.0�淶�Ĺ涨��item�Ĵ洢�ռ���Java���䣻
//
// Return : 0����ɹ�; -1�������
/*****************************************************************************/
PUBLIC int32  MMIJAVA_PIM_Add_Item(MMIJAVA_PIM_TYPE_E type, 
									  int32 *uuid_ptr, 
									  uint8 *item_ptr
									  );

/*******************************************************************************/
// Description :ɾ��һ��vCard/vCalendar��Ŀ
// Parameter: type [IN] 0����vCard, 1����Event��2����Todo
//            uuid [IN]vCard/vCalendar��Ŀ��uuid��
// Return : 0����ɹ�; -1�������
/*****************************************************************************/
PUBLIC int32  MMIJAVA_PIM_Remove_Item(MMIJAVA_PIM_TYPE_E type, 
									  int32 uuid
									  );

#endif
