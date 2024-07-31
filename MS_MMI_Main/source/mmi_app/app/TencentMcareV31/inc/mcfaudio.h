/******************************************************************************/
//File Name:        McfAudio.h
//Author:           wilsonzhu wiltan
//DATE:             10/01/2011
//Copyright:        2011 Tencent, Incoporated. All Rights Reserved.
//Description:      provide simple audio interface for mcare
//Modifed:          nicklei 09/21/2011
//Modifed:          wilsonzhu 01/10/2011
/******************************************************************************/
#ifndef _MCFAUDIO_H_
#define _MCFAUDIO_H_

/*---------------------------------------------------------------------------*/
/*                         Compiler Flag                                     */
/*---------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern "C"
    {
#endif
/*---------------------------------------------------------------------------*/
/*                         Include Files                                     */
/*---------------------------------------------------------------------------*/
#include "mcfcommon.h"

/**---------------------------------------------------------------------------*
 **                         Function Declare                                  *
 **---------------------------------------------------------------------------*/

#ifdef SPREAD_NEW_AUDIO_UPDATE
#define AUDHAND_ERROR (-10)

//---------------------------------------------------------------------------
//FUNCTION: McfAud_PlayStream
//Descrption:������Ƶ����
//Param:
//  IN: pUser:
//      pAudioData:��Ƶ����
//      len:����
//      format:��ʽ
//      playStyle:��������
//      handler:�ص�����
//  OUT:MINT
//Return:
//  MCF_AUD_FAIL:����ʧ��
//  MCF_AUD_SUCCESS: ���ųɹ�  
//Note:
//----------------------------------------------------------------------------
MINT McfAud_PlayStream(MVOID* pUser, MVOID* pAudioData, MUINT len, MBYTE format, 
                            MBYTE playStyle, pfnMcfAudFunc handler);

//---------------------------------------------------------------------------
//FUNCTION: McfAud_StopStream
//Descrption:ֹͣ������Ƶ����
//Param:
//  IN: audHd:��Ƶ�ľ��
//  OUT:MINT
//Return:
//  MCF_AUD_FAIL:����ʧ��
//  MCF_AUD_SUCCESS: ���ųɹ�  
//Note:
//----------------------------------------------------------------------------
MINT McfAud_StopStream(MINT audHd);

//---------------------------------------------------------------------------
//FUNCTION: McfAud_PlayFile
//Descrption:������Ƶ�ļ�
//Param:
//  IN: pUser:
//      pFile:��Ƶ�ļ�
//      playStyle:��������,MCF_AUD_PLAYE_ONCE,MCF_AUD_PLAYE_REPEAT
//      pfnMcfAudFunc handler:�ص�����
//  OUT:MINT
//Return:
//  MCF_AUD_FAIL:����ʧ��
//  MCF_AUD_SUCCESS: ���ųɹ�  
//Note:
//----------------------------------------------------------------------------
MINT McfAud_PlayFile(MVOID* pUser, MSHORT* pFile, MBYTE playStyle, pfnMcfAudFunc handler);

//---------------------------------------------------------------------------
//FUNCTION: McfAud_StopFile
//Descrption:ֹͣ������Ƶ�ļ�
//Param:
//  IN: audHd:��Ƶ�ľ��
//  OUT:MINT
//Return:
//  MCF_AUD_FAIL:����ʧ��
//  MCF_AUD_SUCCESS: ���ųɹ�  
//Note:
//----------------------------------------------------------------------------
MINT McfAud_StopFile(MINT audHd);

//---------------------------------------------------------------------------
//FUNCTION: McfAud_Pause
//Descrption:��ͣ������Ƶ
//Param:
//  IN: audHd:��Ƶ�ľ��
//      pUser:
//      handler:�ص�����
//  OUT:MINT
//Return:
//  MCF_AUD_FAIL:����ʧ��
//  MCF_AUD_SUCCESS: ���ųɹ�  
//Note:
//----------------------------------------------------------------------------
MINT McfAud_Pause(MINT audHd, MVOID* pUser, pfnMcfAudFunc handler);

//---------------------------------------------------------------------------
//FUNCTION: McfAud_Resume
//Descrption:�ָ�������ͣ���ŵ���Ƶ
//Param:
//  IN: audHd:��Ƶ�ľ��
//      pUser:
//      handler:�ص�����
//  OUT:MINT
//Return:
//  MCF_AUD_FAIL:����ʧ��
//  MCF_AUD_SUCCESS: ���ųɹ�  
//Note:
//----------------------------------------------------------------------------
MINT McfAud_Resume(MINT audHd, MVOID* pUser, pfnMcfAudFunc handler);

//---------------------------------------------------------------------------
//FUNCTION: McfAud_StopAll
//Descrption:ֹͣ�������ڲ��ŵ���Ƶ
//Param:
//  IN: MVOID
//  OUT:MVOID
//Return:MVOID
//Note:
//----------------------------------------------------------------------------
MVOID McfAud_StopAll(MVOID);

//---------------------------------------------------------------------------
//FUNCTION: McfAud_CallBack
//Descrption:�ص�����
//Param:
//  IN: ret:���ŵķ���ֵ
//  OUT:MVOID
//Return:MVOID
//Note:
//----------------------------------------------------------------------------
MBOOL McfAud_CallBack(MUINT mocor_hanlde, MVOID* pParam);

//---------------------------------------------------------------------------
//FUNCTION: McfAud_IsRingEnable
//Descrption:��ȡMCARE�����Ŀ���״̬
//Param:
//  IN: 
//  OUT
//Return::����״ֵ̬
//Note:
//----------------------------------------------------------------------------
MBOOL McfAud_IsRingEnable(MVOID);

//---------------------------------------------------------------------------
//FUNCTION: McfAud_SetRingEnable
//Descrption:����MCARE�����Ŀ���״̬
//Param:
//  IN: ����״ֵ̬
//  OUT
//Return::
//Note:
//----------------------------------------------------------------------------
MVOID McfAud_SetRingEnable(MBOOL is_enbale);

MBOOL McfAud_DelAllHandle(MVOID);
MINT McfAud_AddIn(MVOID *pUser,pfnMcfAudFunc handler,MBYTE format, MINT mocor_handle);
//���ųɹ��Ժ��handle�Ĵ���
MBOOL McfAud_DelHandle(MINT audio_handle);
MINT McfAud_GetMocorHandler(MINT audio_hanlde);

#else//#ifdef SPREAD_NEW_AUDIO_UPDATE

//---------------------------------------------------------------------------
//FUNCTION: McfAud_PlayStream
//Descrption:������Ƶ����
//Param:
//  IN: pUser:
//      pAudioData:��Ƶ����
//      len:����
//      format:��ʽ
//      playStyle:��������
//      handler:�ص�����
//  OUT:MINT
//Return:
//  MCF_AUD_FAIL:����ʧ��
//  MCF_AUD_SUCCESS: ���ųɹ�  
//Note:
//----------------------------------------------------------------------------
MINT McfAud_PlayStream(MVOID* pUser, MVOID* pAudioData, MUINT len, MBYTE format, 
                            MBYTE playStyle, pfnMcfAudFunc handler);

//---------------------------------------------------------------------------
//FUNCTION: McfAud_StopStream
//Descrption:ֹͣ������Ƶ����
//Param:
//  IN: audHd:��Ƶ�ľ��
//  OUT:MINT
//Return:
//  MCF_AUD_FAIL:����ʧ��
//  MCF_AUD_SUCCESS: ���ųɹ�  
//Note:
//----------------------------------------------------------------------------
MINT McfAud_StopStream(MINT audHd);

//---------------------------------------------------------------------------
//FUNCTION: McfAud_PlayFile
//Descrption:������Ƶ�ļ�
//Param:
//  IN: pUser:
//      pFile:��Ƶ�ļ�
//      playStyle:��������,MCF_AUD_PLAYE_ONCE,MCF_AUD_PLAYE_REPEAT
//      pfnMcfAudFunc handler:�ص�����
//  OUT:MINT
//Return:
//  MCF_AUD_FAIL:����ʧ��
//  MCF_AUD_SUCCESS: ���ųɹ�  
//Note:
//----------------------------------------------------------------------------
MINT McfAud_PlayFile(MVOID* pUser, MSHORT* pFile, MBYTE playStyle, pfnMcfAudFunc handler);

//---------------------------------------------------------------------------
//FUNCTION: McfAud_StopFile
//Descrption:ֹͣ������Ƶ�ļ�
//Param:
//  IN: audHd:��Ƶ�ľ��
//  OUT:MINT
//Return:
//  MCF_AUD_FAIL:����ʧ��
//  MCF_AUD_SUCCESS: ���ųɹ�  
//Note:
//----------------------------------------------------------------------------
MINT McfAud_StopFile(MINT audHd);

//---------------------------------------------------------------------------
//FUNCTION: McfAud_Pause
//Descrption:��ͣ������Ƶ
//Param:
//  IN: audHd:��Ƶ�ľ��
//      pUser:
//      handler:�ص�����
//  OUT:MINT
//Return:
//  MCF_AUD_FAIL:����ʧ��
//  MCF_AUD_SUCCESS: ���ųɹ�  
//Note:
//----------------------------------------------------------------------------
MINT McfAud_Pause(MINT audHd, MVOID* pUser, pfnMcfAudFunc handler);

//---------------------------------------------------------------------------
//FUNCTION: McfAud_Resume
//Descrption:�ָ�������ͣ���ŵ���Ƶ
//Param:
//  IN: audHd:��Ƶ�ľ��
//      pUser:
//      handler:�ص�����
//  OUT:MINT
//Return:
//  MCF_AUD_FAIL:����ʧ��
//  MCF_AUD_SUCCESS: ���ųɹ�  
//Note:
//----------------------------------------------------------------------------
MINT McfAud_Resume(MINT audHd, MVOID* pUser, pfnMcfAudFunc handler);

//---------------------------------------------------------------------------
//FUNCTION: McfAud_StopAll
//Descrption:ֹͣ�������ڲ��ŵ���Ƶ
//Param:
//  IN: MVOID
//  OUT:MVOID
//Return:MVOID
//Note:
//----------------------------------------------------------------------------
MVOID McfAud_StopAll(MVOID);

//---------------------------------------------------------------------------
//FUNCTION: McfAud_CallBack
//Descrption:�ص�����
//Param:
//  IN: ret:���ŵķ���ֵ
//  OUT:MVOID
//Return:MVOID
//Note:
//----------------------------------------------------------------------------
MVOID McfAud_CallBack(MINT ret);

//---------------------------------------------------------------------------
//FUNCTION: McfAud_IsRingEnable
//Descrption:��ȡMCARE�����Ŀ���״̬
//Param:
//  IN: 
//  OUT
//Return::����״ֵ̬
//Note:
//----------------------------------------------------------------------------
MBOOL McfAud_IsRingEnable(MVOID);

//---------------------------------------------------------------------------
//FUNCTION: McfAud_SetRingEnable
//Descrption:����MCARE�����Ŀ���״̬
//Param:
//  IN: ����״ֵ̬
//  OUT
//Return::
//Note:
//----------------------------------------------------------------------------
MVOID McfAud_SetRingEnable(MBOOL is_enbale);

MBOOL McfAud_IsPlaying(MVOID);

#endif//#ifdef SPREAD_NEW_AUDIO_UPDATE

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
    }
#endif

#endif//_MCFAUDIO_H_

