/******************************************************************************
 ** File Name:   greeneye2_tcard_log.h                                                *
 ** Author:                                                     *
 ** DATE:                                                         *
 ** Copyright:      2005 Spreadtrum, Incoporated. All Rights Reserved.         *
 ** Description:                       *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                                                                           *
 ** --------------------------------------------------------------------------*
 ** DATE           NAME             DESCRIPTION                               *
 ******************************************************************************/

#ifndef GREENEYE2_TCARD_LOG_H
#define GREENEYE2_TCARD_LOG_H

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif

/**---------------------------------------------------------------------------*
 **                         Data Structures                                   *
 **---------------------------------------------------------------------------*/
typedef enum 
{
    GPS_TCARDLOG_TYPE_ALL = 0,
    GPS_TCARDLOG_TYPE_MAX,
} GPS_TCARDLOG_TYPE_E;

/**---------------------------------------------------------------------------*
 **                         Function Prototypes                               *
 **---------------------------------------------------------------------------*/
PUBLIC void Gps_CardLogOpen(GPS_TCARDLOG_TYPE_E type);
PUBLIC void Gps_CardLogClose (GPS_TCARDLOG_TYPE_E type);
PUBLIC void Gps_CardLogWrite (GPS_TCARDLOG_TYPE_E type, const uint8 *buffer, uint32 length);
PUBLIC void Gps_Cardlog_task_init(void);
PUBLIC void Gps_Cardlog_task_deinit(void);
/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
}
#endif /* __cplusplus */ 

#endif

