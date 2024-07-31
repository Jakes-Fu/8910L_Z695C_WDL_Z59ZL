/*****************************************************************************
** File Name:      qsystem.h                                                  *
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
#ifndef QSYSTEM_H_
#define QSYSTEM_H_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mn_type.h"
#include "guiwin.h"
#include "mmk_timer.h"

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

/**--------------------------------------------------------------------------*
 **                         TYPE DEFINITION                                 *
 **--------------------------------------------------------------------------*/
typedef  int    ISoundPlayer;
typedef  int    ISound;

//ͨ�����ݽṹ
typedef struct 
{
    unsigned short 	wYear;
    unsigned short	nMonth;
    unsigned short	nDay;
    unsigned short	nHour;
    unsigned short	nMin;
    unsigned short	nSec;
}QTime;

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description :��������
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC void QPlayRecvMsg(ISoundPlayer* pSoundPlayer);
/*****************************************************************************/
//  Description :������
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC void QPlayVibrator(ISound* pSound);

/*****************************************************************************/
//  Description :
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC void ISOUNDPLAYER_Release(ISoundPlayer* pSoundPlayer);

/*****************************************************************************/
//  Description :
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC void ISOUND_Release(ISound* pSound);

/*****************************************************************************/
//  Description :����url
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC void QGoToUrl(char* url_ptr);

/*****************************************************************************/
//  Description :���Ͷ���
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC void QSendSms(char* addr_str_ptr,unsigned short* msg_wstr_ptr);

/*****************************************************************************/
//  Description :����绰 
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC void QMakeCall(char* num_ptr);

/*****************************************************************************/
//  Description :����ֻ�ϵͳʱ����Ϣ
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC void QGetDateTime(QTime* time_ptr);

/*****************************************************************************/
//  Description :update the idle status of qq
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC void QUPDATASTATUS(int status);

/*****************************************************************************/
//  Description: set s_is_login_selcet
//  Global resource dependence :  s_is_login_selcet
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC void MMIQQ_SetLoginSelect(BOOLEAN is_login_select);

/*****************************************************************************/
//  Description :���õ�ǰQQʹ�õ�sim��(sim1 or sim2)
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC void MMIQQ_SetSimSelect(MN_DUAL_SYS_E sim_select);

/*****************************************************************************/
//  Description :��õ�ǰQQʹ�õ�sim��(sim1 or sim2)
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC MN_DUAL_SYS_E MMIQQ_GetSimSelect(void);

/*****************************************************************************/
//  Description: �õ���ǰQQ��lcd �߼��Ƕ�
//  Global resource dependence :  s_qq_logic_angle
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
PUBLIC LCD_ANGLE_E MMIQQ_GetLogicAngle(void);

/*****************************************************************************/
//  Description :�õ�s_qq_is_use_wifi
//  Global resource dependence : none
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIQQ_GetIsUseWifi(void);

/*****************************************************************************/
//  Description : QQ��������״̬�Ƿ���׼����
//  Global resource dependence : none
//  Author: fen.xie
//  Note:
/*****************************************************************************/
BOOLEAN MMIQQ_IsGprsStatusReady(void);

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif
#endif
