/******************************************************************************
 ** File Name:      dspvb_as.h                                                             *
 ** Author:         Benjamin.Wang                                             *
 ** DATE:           01/07/2006                                                *
 ** Copyright:      2006 Spreadtrum, Incoporated. All Rights Reserved.         *
 ** Description:    DSP audioservice adapter layer.* 
 **                                                                           *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 01/07/2006     Benjamin.Wang    Create.                                   *
 ******************************************************************************/
  
/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/ 

#ifndef _DSPVB_AS_H_
#define _DSPVB_AS_H_



/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    extern   "C" 
    {
#endif

/**---------------------------------------------------------------------------*
 **                         Macro Definition                                   *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         Data Structures                                   *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         Global Variables                                  *
 **---------------------------------------------------------------------------*/


/**---------------------------------------------------------------------------*
 **                         Constant Variables                                *
 **---------------------------------------------------------------------------*/
 

/**---------------------------------------------------------------------------*
 **                         Function Definitions                              *
 **---------------------------------------------------------------------------*/
/*****************************************************************************/
/*****************************************************************************/
//  Description:    This function is used to register LAYER1 logical VB device.
//  Author:         Johnson.sun
//  Note:  used for calling condition.
//****************************************************************************/
PUBLIC HAUDIODEV LAYER1_RegAudioDevice(void);

/*****************************************************************************/
//  Description:    This function is used to register MP4 logical VB device.
//  Author:         Johnson.sun
//  Note:   used for playing mp4, whose audio control by dsp.
//****************************************************************************/
PUBLIC HAUDIODEV MP4_RegAudioDevice(void);

/*****************************************************************************/
//  Description:    This function is used to register RECORD logical VB device.
//  Author:         Johnson.sun
//  Note:   used for general record condition.
//****************************************************************************/
PUBLIC HAUDIODEV RECORD_RegAudioDevice(void);

/*****************************************************************************/
//  Description:    This function is used to register MP4RECORD logical VB device.
//  Author:         Johnson.sun
//  Note:  used for mp4 photograph condition.
//****************************************************************************/
PUBLIC HAUDIODEV MP4RECORD_RegAudioDevice(void);

/*****************************************************************************/
//  Description:    This function is used to register LOOPBACK logical VB device.
//  Author:         
//  Note:  used for calling condition in loopback.
//****************************************************************************/
PUBLIC HAUDIODEV LOOPBACK_RegAudioDevice (void);

/*****************************************************************************/
//  Description:    This function is used to reg device used to get dsp data.
//  Author:         Shujing.Dong
//  Note:
//****************************************************************************/
PUBLIC HAUDIODEV DSPDATA_RegAudioDevice(void);


 /*****************************************************************************/
//  Description:    This function transmit the voice recorded data to uplayer buffer
//  Author:         guangtao.zhi
//    Note:           
/*****************************************************************************/
extern void DSPVB_Record_ReadData(uint16 *puiDestData, uint32 uiCount, uint32 *puiDataLen);
/*****************************************************************************/
//  Description:    This function is used to start voice record.
//  Author:        guangtao.zhi
//  Note:
//****************************************************************************/
extern void DSPVB_StartRecord(uint32 uiSampleRate, RECORD_CALLBACK callback_fun);
/*****************************************************************************/
//  Description:    This function is used to stop voice record.
//  Author:        guangtao.zhi
//  Note:
//****************************************************************************/
extern void DSPVB_StopRecord();
/*****************************************************************************/
//  Description:    This function is used to start voice pcm dump.
//  Author:        guangtao.zhi
//  Note:
//****************************************************************************/
extern void DSPVB_PcmDumpEnable();
/*****************************************************************************/
//  Description:    This function is used to stop pcm dump.
//  Author:        guangtao.zhi
//  Note:
//****************************************************************************/
extern void DSPVB_PcmDumpStop();
/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
    
#endif  // End of dspvb_as.h

#endif // End of _DSPVB_AS_H_