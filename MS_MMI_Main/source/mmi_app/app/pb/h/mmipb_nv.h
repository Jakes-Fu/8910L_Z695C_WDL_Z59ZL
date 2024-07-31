/*************************************************************************
 ** File Name:      mmipb_nv.h                                           *
 ** Author:         bruce.chi                                            *
 ** Date:           2006/09/25                                           *
 ** Copyright:      2006 Spreadtrum, Incorporated. All Rights Reserved.  *
 ** Description:     This file defines the function about nv             *
 *************************************************************************
 *************************************************************************
 **                        Edit History                                  *
 ** ---------------------------------------------------------------------*
 ** DATE           NAME             DESCRIPTION                          *
 ** 2006/09/25     bruce.chi        Create.                               *
*************************************************************************/

#ifndef _MMIPB_NV_H_
#define _MMIPB_NV_H_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmi_module.h"
#include "mmipb_common.h"
/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif

/**--------------------------------------------------------------------------*
 **                         TYPE AND CONSTANT                                *
 **--------------------------------------------------------------------------*/
// �ֻ��е绰����¼�������
#define MMINV_MAX_PHONEBOOK_RECORDS     MMIPB_NV_MAX_RECORD_NUM

 //for cr NEWMS00172066
#if 0 //def CMCC_UI_STYLE

#ifdef MMINV_MAX_PHONEBOOK_RECORDS
#undef MMINV_MAX_PHONEBOOK_RECORDS
#endif

#define MMINV_MAX_PHONEBOOK_RECORDS     1500
#endif

// �ֻ��е绰����������
//#define MMINV_MAX_PHONEBOOK_GROUP_NUM	10
//#define MMINV_MAX_PHONEBOOK_GROUP_NUM    32//(27 + MMI_DUAL_SYS_MAX)
		
typedef enum
{
        MMINV_PHONEBOOK_DISPLAY_MODE = MMI_MODULE_PB << 16,
        MMINV_PHONEBOOK_SAVE_POSTION,
        MMINV_PHONEBOOK_DISPLAY_OPTION,
        MMINV_PHONEBOOK_DISPLAY_PERIOD,
        MMINV_PHONEBOOK_UPDATE_PERIOD,
        MMINV_PHONEBOOK_SORT_BY_SURNAME,
        MMINV_PHONEBOOK_NEED_CREATE_GROUP,
        MMINV_PHONEBOOK_FIRST_ENTER,
        MMINV_PHONEBOOK_DISPLAY_DEBUG_MENU,
#ifdef MMIPB_SYNC_WITH_O365
		//first sync flag
		MMINV_PHONEBOOK_FIRST_SYNC,
		//MSA sign in flag
		MMINV_PHONEBOOK_MSA_SIGN_IN,
		//�ϴ���ͬ����ϵ�ʱ�䣬ÿ��ͬ����ϸ��´�ֵ
		MMINV_PHONEBOOK_HEARTBEAT,
		//�ϴ���ͬ����ϵ�ʱ�䣬ÿ��ͬ����ϸ��´�ֵ
		MMINV_PHONEBOOK_HEAVY_SYNCEDTIME,
		 //�ϴ���ͬ����ϵ�ʱ�䣬ÿ��ͬ����ϸ��´�ֵ
		MMINV_PHONEBOOK_LIGHT_SYNCEDTIME,
		//��¼�ϴ���ͬ����ȡ��O365����ϵ������
		MMINV_PHONEBOOK_HEAVY_LASTCOUNT,
#endif
#ifdef MMIPB_SIMCONTACT_SUPPORT
        SETTINGS_NV_PHONEBOOK_MEMSTATUS,
#endif
#ifdef MMIPB_GROUP_MANAGE_SUPPORT 
        MMINV_PHONEBOOK_GROUP_0, 

        // �绰��������Ϣ
        MMINV_PHONEBOOK_GROUP_MAX = MMINV_PHONEBOOK_GROUP_0 + MMIPB_USER_GROUP_NUM + MMIPB_SYSTEM_GROUP_NUM - 1,
#endif
        // �绰����¼
        MMINV_PHONEBOOK_MAIN_FIRST_ENTRY,			// 
        MMINV_PHONEBOOK_MAIN_LAST_ENTRY = MMINV_PHONEBOOK_MAIN_FIRST_ENTRY + MMINV_MAX_PHONEBOOK_RECORDS - 1,
#ifdef MMIPB_MOST_USED_SUPPORT        
        // ������ϵ��
        MMINV_PHONEBOOK_MOSTUSED_FIRST_ENTRY,			// 
        MMINV_PHONEBOOK_MOSTUSED_LAST_ENTRY = MMINV_PHONEBOOK_MOSTUSED_FIRST_ENTRY + MMINV_MAX_MOSTUSED_PHONEBOOK_RECORDS - 1
#endif
}PB_NV_ITEM_E;


/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                                 *
 **--------------------------------------------------------------------------*/

/*****************************************************************************/
// 	Description : register set module nv len and max item
//	Global resource dependence : none
//  Author: bruce.chi
//	Note:
/*****************************************************************************/
PUBLIC void MMIPB_RegNv(void);

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif //_MMICC_NV_H_

