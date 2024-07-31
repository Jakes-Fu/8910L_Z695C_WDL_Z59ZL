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
// Description :获得下一个vCard/vCalendar条目
// Parameter: type      [IN] 0代表vCard, 1代表Event，2代表Todo；
//            lastItemId[IN] 上次读到条目的uuid, -1表示从头开始读取；
//            uuid_ptr      [OUT] vCard/vCalendar条目的uuid；
//            item_ptr      [IN/OUT]用于保存UTF-8编码的vCard/vCalendar条目数据，编码
//                           符合vCard2.1/vCalendar 1.0规范的规定。item的存储空间由Java分配；
//            maxItemLen 【IN】 Java分配的item存储空间的大小；
//
// Return : 0代表成功; -1代表出错，或者已经读到vCard/vCalendar的末尾，本次读取没有获
//          得vCard/vCalendar条目数据。
/*****************************************************************************/
PUBLIC int32 MMIJAVA_PIM_Get_Next_Item(MMIJAVA_PIM_TYPE_E type,
									   int32             lastItemId,
									   int32             *uuid_ptr, 
									   uint8              *item_ptr,
									   uint32             maxItemLen
									   );

/*******************************************************************************/
// Description :修改一个vCard/vCalendar条目
// Parameter: type [IN] 0代表vCard, 1代表Event，2代表Todo
//            uuid [IN]vCard/vCalendar条目的uuid；
//            item_ptr[IN/OUT]用于保存UTF-8编码的vCard/vCalendar条目数据，编码
//                           符合vCard2.1/vCalendar 1.0规范的规定。item的存储空间由Java分配；
//
// Return : 0代表成功; -1代表出错。
/*****************************************************************************/
PUBLIC int32  MMIJAVA_PIM_Modify_Item(MMIJAVA_PIM_TYPE_E type, 
									  int32 uuid, 
									  uint8 *item_ptr
									  );

/*******************************************************************************/
// Description :添加一个vCard/vCalendar条目
// Parameter: type [IN] 0代表vCard, 1代表Event，2代表Todo
//            uuid_ptr [OUT]vCard/vCalendar条目的uuid；
//            item_ptr[IN/OUT]用于保存UTF-8编码的vCard/vCalendar条目数据，编码
//                           符合vCard2.1/vCalendar 1.0规范的规定。item的存储空间由Java分配；
//
// Return : 0代表成功; -1代表出错。
/*****************************************************************************/
PUBLIC int32  MMIJAVA_PIM_Add_Item(MMIJAVA_PIM_TYPE_E type, 
									  int32 *uuid_ptr, 
									  uint8 *item_ptr
									  );

/*******************************************************************************/
// Description :删除一个vCard/vCalendar条目
// Parameter: type [IN] 0代表vCard, 1代表Event，2代表Todo
//            uuid [IN]vCard/vCalendar条目的uuid；
// Return : 0代表成功; -1代表出错。
/*****************************************************************************/
PUBLIC int32  MMIJAVA_PIM_Remove_Item(MMIJAVA_PIM_TYPE_E type, 
									  int32 uuid
									  );

#endif
