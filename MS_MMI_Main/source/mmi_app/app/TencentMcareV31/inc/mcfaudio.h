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
//Descrption:播放音频数据
//Param:
//  IN: pUser:
//      pAudioData:音频数据
//      len:长度
//      format:格式
//      playStyle:播放类型
//      handler:回调函数
//  OUT:MINT
//Return:
//  MCF_AUD_FAIL:播放失败
//  MCF_AUD_SUCCESS: 播放成功  
//Note:
//----------------------------------------------------------------------------
MINT McfAud_PlayStream(MVOID* pUser, MVOID* pAudioData, MUINT len, MBYTE format, 
                            MBYTE playStyle, pfnMcfAudFunc handler);

//---------------------------------------------------------------------------
//FUNCTION: McfAud_StopStream
//Descrption:停止播放音频数据
//Param:
//  IN: audHd:音频的句柄
//  OUT:MINT
//Return:
//  MCF_AUD_FAIL:播放失败
//  MCF_AUD_SUCCESS: 播放成功  
//Note:
//----------------------------------------------------------------------------
MINT McfAud_StopStream(MINT audHd);

//---------------------------------------------------------------------------
//FUNCTION: McfAud_PlayFile
//Descrption:播放音频文件
//Param:
//  IN: pUser:
//      pFile:音频文件
//      playStyle:播放类型,MCF_AUD_PLAYE_ONCE,MCF_AUD_PLAYE_REPEAT
//      pfnMcfAudFunc handler:回调函数
//  OUT:MINT
//Return:
//  MCF_AUD_FAIL:播放失败
//  MCF_AUD_SUCCESS: 播放成功  
//Note:
//----------------------------------------------------------------------------
MINT McfAud_PlayFile(MVOID* pUser, MSHORT* pFile, MBYTE playStyle, pfnMcfAudFunc handler);

//---------------------------------------------------------------------------
//FUNCTION: McfAud_StopFile
//Descrption:停止播放音频文件
//Param:
//  IN: audHd:音频的句柄
//  OUT:MINT
//Return:
//  MCF_AUD_FAIL:播放失败
//  MCF_AUD_SUCCESS: 播放成功  
//Note:
//----------------------------------------------------------------------------
MINT McfAud_StopFile(MINT audHd);

//---------------------------------------------------------------------------
//FUNCTION: McfAud_Pause
//Descrption:暂停播放音频
//Param:
//  IN: audHd:音频的句柄
//      pUser:
//      handler:回调函数
//  OUT:MINT
//Return:
//  MCF_AUD_FAIL:播放失败
//  MCF_AUD_SUCCESS: 播放成功  
//Note:
//----------------------------------------------------------------------------
MINT McfAud_Pause(MINT audHd, MVOID* pUser, pfnMcfAudFunc handler);

//---------------------------------------------------------------------------
//FUNCTION: McfAud_Resume
//Descrption:恢复播放暂停播放的音频
//Param:
//  IN: audHd:音频的句柄
//      pUser:
//      handler:回调函数
//  OUT:MINT
//Return:
//  MCF_AUD_FAIL:播放失败
//  MCF_AUD_SUCCESS: 播放成功  
//Note:
//----------------------------------------------------------------------------
MINT McfAud_Resume(MINT audHd, MVOID* pUser, pfnMcfAudFunc handler);

//---------------------------------------------------------------------------
//FUNCTION: McfAud_StopAll
//Descrption:停止所有正在播放的音频
//Param:
//  IN: MVOID
//  OUT:MVOID
//Return:MVOID
//Note:
//----------------------------------------------------------------------------
MVOID McfAud_StopAll(MVOID);

//---------------------------------------------------------------------------
//FUNCTION: McfAud_CallBack
//Descrption:回调函数
//Param:
//  IN: ret:播放的返回值
//  OUT:MVOID
//Return:MVOID
//Note:
//----------------------------------------------------------------------------
MBOOL McfAud_CallBack(MUINT mocor_hanlde, MVOID* pParam);

//---------------------------------------------------------------------------
//FUNCTION: McfAud_IsRingEnable
//Descrption:获取MCARE声音的开关状态
//Param:
//  IN: 
//  OUT
//Return::开关状态值
//Note:
//----------------------------------------------------------------------------
MBOOL McfAud_IsRingEnable(MVOID);

//---------------------------------------------------------------------------
//FUNCTION: McfAud_SetRingEnable
//Descrption:设置MCARE声音的开关状态
//Param:
//  IN: 开关状态值
//  OUT
//Return::
//Note:
//----------------------------------------------------------------------------
MVOID McfAud_SetRingEnable(MBOOL is_enbale);

MBOOL McfAud_DelAllHandle(MVOID);
MINT McfAud_AddIn(MVOID *pUser,pfnMcfAudFunc handler,MBYTE format, MINT mocor_handle);
//播放成功以后的handle的处理
MBOOL McfAud_DelHandle(MINT audio_handle);
MINT McfAud_GetMocorHandler(MINT audio_hanlde);

#else//#ifdef SPREAD_NEW_AUDIO_UPDATE

//---------------------------------------------------------------------------
//FUNCTION: McfAud_PlayStream
//Descrption:播放音频数据
//Param:
//  IN: pUser:
//      pAudioData:音频数据
//      len:长度
//      format:格式
//      playStyle:播放类型
//      handler:回调函数
//  OUT:MINT
//Return:
//  MCF_AUD_FAIL:播放失败
//  MCF_AUD_SUCCESS: 播放成功  
//Note:
//----------------------------------------------------------------------------
MINT McfAud_PlayStream(MVOID* pUser, MVOID* pAudioData, MUINT len, MBYTE format, 
                            MBYTE playStyle, pfnMcfAudFunc handler);

//---------------------------------------------------------------------------
//FUNCTION: McfAud_StopStream
//Descrption:停止播放音频数据
//Param:
//  IN: audHd:音频的句柄
//  OUT:MINT
//Return:
//  MCF_AUD_FAIL:播放失败
//  MCF_AUD_SUCCESS: 播放成功  
//Note:
//----------------------------------------------------------------------------
MINT McfAud_StopStream(MINT audHd);

//---------------------------------------------------------------------------
//FUNCTION: McfAud_PlayFile
//Descrption:播放音频文件
//Param:
//  IN: pUser:
//      pFile:音频文件
//      playStyle:播放类型,MCF_AUD_PLAYE_ONCE,MCF_AUD_PLAYE_REPEAT
//      pfnMcfAudFunc handler:回调函数
//  OUT:MINT
//Return:
//  MCF_AUD_FAIL:播放失败
//  MCF_AUD_SUCCESS: 播放成功  
//Note:
//----------------------------------------------------------------------------
MINT McfAud_PlayFile(MVOID* pUser, MSHORT* pFile, MBYTE playStyle, pfnMcfAudFunc handler);

//---------------------------------------------------------------------------
//FUNCTION: McfAud_StopFile
//Descrption:停止播放音频文件
//Param:
//  IN: audHd:音频的句柄
//  OUT:MINT
//Return:
//  MCF_AUD_FAIL:播放失败
//  MCF_AUD_SUCCESS: 播放成功  
//Note:
//----------------------------------------------------------------------------
MINT McfAud_StopFile(MINT audHd);

//---------------------------------------------------------------------------
//FUNCTION: McfAud_Pause
//Descrption:暂停播放音频
//Param:
//  IN: audHd:音频的句柄
//      pUser:
//      handler:回调函数
//  OUT:MINT
//Return:
//  MCF_AUD_FAIL:播放失败
//  MCF_AUD_SUCCESS: 播放成功  
//Note:
//----------------------------------------------------------------------------
MINT McfAud_Pause(MINT audHd, MVOID* pUser, pfnMcfAudFunc handler);

//---------------------------------------------------------------------------
//FUNCTION: McfAud_Resume
//Descrption:恢复播放暂停播放的音频
//Param:
//  IN: audHd:音频的句柄
//      pUser:
//      handler:回调函数
//  OUT:MINT
//Return:
//  MCF_AUD_FAIL:播放失败
//  MCF_AUD_SUCCESS: 播放成功  
//Note:
//----------------------------------------------------------------------------
MINT McfAud_Resume(MINT audHd, MVOID* pUser, pfnMcfAudFunc handler);

//---------------------------------------------------------------------------
//FUNCTION: McfAud_StopAll
//Descrption:停止所有正在播放的音频
//Param:
//  IN: MVOID
//  OUT:MVOID
//Return:MVOID
//Note:
//----------------------------------------------------------------------------
MVOID McfAud_StopAll(MVOID);

//---------------------------------------------------------------------------
//FUNCTION: McfAud_CallBack
//Descrption:回调函数
//Param:
//  IN: ret:播放的返回值
//  OUT:MVOID
//Return:MVOID
//Note:
//----------------------------------------------------------------------------
MVOID McfAud_CallBack(MINT ret);

//---------------------------------------------------------------------------
//FUNCTION: McfAud_IsRingEnable
//Descrption:获取MCARE声音的开关状态
//Param:
//  IN: 
//  OUT
//Return::开关状态值
//Note:
//----------------------------------------------------------------------------
MBOOL McfAud_IsRingEnable(MVOID);

//---------------------------------------------------------------------------
//FUNCTION: McfAud_SetRingEnable
//Descrption:设置MCARE声音的开关状态
//Param:
//  IN: 开关状态值
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

