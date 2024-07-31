/*****************************************************************************
** File Name:      qq2009.h                                                  *
** Author:                                                                   *
** Date:             11/05/2010                                                *
** Copyright:      2003 Spreadtrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to declare QQ function and define global               *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 05/2010       fen.xie       Create
******************************************************************************/
#ifndef __QQAPP_H__
#define __QQAPP_H__

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmi_module.h"
#include "mmk_type.h"
#include "datatype.h"

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif
/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
#define QQSTATE_ERROR		-1	 //����
#define QQSTATE_ONLINE		0    //����
#define QQSTATE_AWAYLINE	1	 //�뿪
#define QQSTATE_OFFLINE		2	 //����	
#define QQSTATE_INVISIBLE	3    //����
#define QQSTATE_HAVEMSG		4    //����Ϣ


#define QQSOUND_ON			1
#define QQSOUND_OFF			2

/**--------------------------------------------------------------------------*
 **                         TYPE DEFINITION                                 *
 **--------------------------------------------------------------------------*/

typedef enum
{
	MMIQQ_CTRL_ID_START = (MMI_MODULE_QQ << 16),
	MMIQQ_CTRL_ID_EDITBOX,
	MMIQQ_CTRL_ID_SELECT_SIM,
	MMIQQ_MAX_CTRL_ID
		
}MMIQQ_CTRL_ID_E;


typedef enum
{
    MMIQQ_BUDDY_STAT_ONLINE = 0,
    MMIQQ_BUDDY_STAT_AWAY,
    MMIQQ_BUDDY_STAT_OFFLINE,
    MMIQQ_BUDDY_STAT_INVISIBLE,
    MMIQQ_BUDDY_STAT_HAVEMSG,
    MMIQQ_BUDDY_STAT_MAX
} MMIQQ_BUDDY_STAT_E;


typedef enum
{
	MMIQQ_WIN_ID_START = (MMI_MODULE_QQ << 16),
	MMIQQ_WIN_ID_QQMAIN,
	MMIQQ_WIN_ID_EDITBOX,
	MMIQQ_WIN_ID_SELECT_SIM,
	MMIQQ_MAX_WIN_ID
		
}MMIQQ_WINDOW_ID_E;

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description: ��QQ
//  Global resource dependence :  
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC void MMIQQ_EntryQQ(void);

/*****************************************************************************/
//  Description: �ͷ�QQ(������)��ص�memory
//  Global resource dependence :  
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC void MMIQQ_ReleaseMemory(void);

/*****************************************************************************/
//  Description: ����QQ(������)��ص�memory
//  Global resource dependence :  
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIQQ_CreateMemory(void);

/*****************************************************************************/
//  Description: �ر�QQ
//  Global resource dependence :  
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC void ExitQQ( void );

/*****************************************************************************/
//  Description: QGetMqq
//  Global resource dependence :  s_qq_manage_ptr
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC void* QGetMqq(void);

/*****************************************************************************/
//  Description: QGetSCR_W
//  Global resource dependence :  
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC int QGetSCR_W(void);

/*****************************************************************************/
//  Description: QGetSCR_H
//  Global resource dependence :  
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC int QGetSCR_H(void);

/*****************************************************************************/
//  Description: QQ_IsRunning
//  Global resource dependence :  
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN QQ_IsRunning(void);

/*****************************************************************************/
//  Description :����ʱ����
//  Global resource dependence : none
//  Author: TENCENT
//  Note: 
/*****************************************************************************/
PUBLIC QU32 QTimeInterval(QTime a,QTime b);

/*****************************************************************************/
//  Description :����ʱ����
//  Global resource dependence : none
//  Author: TENCENT
//  Note: 
/*****************************************************************************/
PUBLIC QU32 QTimeAllDay(QTime a);

/*****************************************************************************/
//  Description :�õ���ǰϵͳʱ���ʱ���
//  Global resource dependence : none
//  Author: TENCENT
//  Note: 
/*****************************************************************************/
PUBLIC QU32 QGetSystemTime(void);

/*****************************************************************************/
//  Description :����ֻ�ϵͳʱ��ĵ���������msֵ
//  Global resource dependence : none
//  Author: TENCENT
//  Note: 
/*****************************************************************************/
PUBLIC QU32 GETTIMEMS(void);

/*****************************************************************************/
//  Description :ʱ���ת��Ϊϵͳʱ��
//  Global resource dependence : none
//  Author: TENCENT
//  Note: 
/*****************************************************************************/
PUBLIC QTime QStamp2SysTime(QU32 t);

/*****************************************************************************/
//  Description: �õ���ǰQQϵͳ�ļ��ɴ洢���̷���������Ϣ
//  Global resource dependence :  
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
PUBLIC wchar* MMIQQ_GetDeviceName(uint16* name_len_ptr);
/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif
