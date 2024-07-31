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

//通用数据结构
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
//  Description :播放声音
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC void QPlayRecvMsg(ISoundPlayer* pSoundPlayer);
/*****************************************************************************/
//  Description :播放震动
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
//  Description :访问url
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC void QGoToUrl(char* url_ptr);

/*****************************************************************************/
//  Description :发送短信
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC void QSendSms(char* addr_str_ptr,unsigned short* msg_wstr_ptr);

/*****************************************************************************/
//  Description :拨打电话 
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC void QMakeCall(char* num_ptr);

/*****************************************************************************/
//  Description :获得手机系统时间信息
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
//  Description :设置当前QQ使用的sim卡(sim1 or sim2)
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC void MMIQQ_SetSimSelect(MN_DUAL_SYS_E sim_select);

/*****************************************************************************/
//  Description :获得当前QQ使用的sim卡(sim1 or sim2)
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC MN_DUAL_SYS_E MMIQQ_GetSimSelect(void);

/*****************************************************************************/
//  Description: 得到当前QQ的lcd 逻辑角度
//  Global resource dependence :  s_qq_logic_angle
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
PUBLIC LCD_ANGLE_E MMIQQ_GetLogicAngle(void);

/*****************************************************************************/
//  Description :得到s_qq_is_use_wifi
//  Global resource dependence : none
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIQQ_GetIsUseWifi(void);

/*****************************************************************************/
//  Description : QQ拨号链接状态是否已准备好
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
