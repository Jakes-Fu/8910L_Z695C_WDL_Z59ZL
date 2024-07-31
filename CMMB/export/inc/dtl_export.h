/******************************************************************************
 ** File Name:      dtl_export.h                                              *
 ** Author:         Hanbing Zhang                                             *
 ** DATE:           1/11/2007                                                 *
 ** Copyright:      2004 Spreatrum, Incoporated. All Rights Reserved.         *
 ** Description:    This file define data transfer layer interface            *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 1/11/2007     Hanbing.Zhang        create                                 *
 ******************************************************************************/

#ifndef _DTL_EXPORT_H
#define _DTL_EXPORT_H

#include "kd.h"

/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/


/**---------------------------------------------------------------------------*
 **                         Function Propertype                               *
 **---------------------------------------------------------------------------*/
/************************************************************************
* \fn  void DTL_TurnoffSeineLdo(void)
* \param void
* \return void 
* \brief  turn off seine ldo
*             
* \author Hanbing.Zhang
* \note   Nothing.  
************************************************************************/
void DTL_TurnoffSeineLdo(void);

/************************************************************************
* \fn  KDboolean DTL_IdentifyLCDId(KDuint32  lcdid_addr,
*                                  KDuint32 *lcd_id1_ptr,
*                                  KDuint32 *lcd_id2_ptr
*                                  );
* \param uint32 lcdid_addr lcd id address
* \param uint32 *lcd_id1_ptr when cd high,read  it
* \param uint32 *lcd_id2_ptr when cd low,read it
* \param uint32 sim_smd_ptr smd 1:found,0:no
* \return void 
* \brief  get lcd id
*             
* \author Hanbing.Zhang
* \note   Nothing.  
************************************************************************/
KDboolean DTL_IdentifyLCDId(KDuint32  lcdid_addr,
                            KDuint32 *lcd_id1_ptr,
                            KDuint32 *lcd_id2_ptr,
                            KDuint32 *sim_smd_ptr
                            );

/************************************************************************
* \fn  void DTL_RecvInt(void)
* \param  void
* \return void
* \brief  handle dtl receive int
*
* \author Hanbing.Zhang
* \note   Nothing
************************************************************************/
void DTL_RecvInt(void);

/************************************************************************
* \fn  void DTLSVC_Init(void)
* \param  void
* \return void 
* \brief  init for create dtlsvc task 
*
* \author Hanbing.Zhang
* \note   for static task
************************************************************************/
void DTLSVC_Init(void);

/************************************************************************
* \fn  void DTLSVC_Close(void)
* \param  void
* \return void 
* \brief  exit dtlsvc task 
*
* \author Hanbing.Zhang
* \note   for static task
************************************************************************/
void DTLSVC_Close(void);

/************************************************************************
* \fn  void* DTLSVC_Task(
*                       void    *argv
*                       )
* \param  argv
* \return void 
* \brief  dtl service task
*             
* \author Hanbing.Zhang
* \note   Nothing.  
************************************************************************/
void* DTLSVC_Task(
                 void    *argv
                 );

/************************************************************************
* \fn  void DTL_VLCDCInit(void);
* \param void
* \return void 
* \brief  init v lcdc
*             
* \author Hanbing.Zhang
* \note   Nothing.  
************************************************************************/
void DTL_VLCDCInit(void);

/************************************************************************
* \fn  KDuint32 DTL_ReadVLCDC(KDuint32 cd_status)
* \param cd_status
* \return result 
* \brief  read v lcdc
*             
* \author gang.li
* \note   call this after DTL_VLCDCInit.  
************************************************************************/
KDuint32 DTL_ReadVLCDC(KDuint32 cd_status);

/************************************************************************
* \fn void DTL_WriteVLCDC(KDuint32 cd_status, KDuint32 data)
* \param cd_status
* \param data write data
* \return result 
* \brief  write v lcdc
*             
* \author gang.li
* \note   call this after DTL_VLCDCInit.  
************************************************************************/
void DTL_WriteVLCDC(KDuint32 cd_status, KDuint32 data);

/************************************************************************
* \fn void DTL_VLCDCClose(void);
* \param void
* \return void 
* \brief  close v lcdc
*             
* \author Hanbing.Zhang
* \note   Nothing.  
************************************************************************/
void DTL_VLCDCClose(void);

/************************************************************************
* \fn KDuint32 DTL_ReadLCDID(KDuint32 wcmd_cd_status,
*                      KDuint32 lcdid_addr);
* \param cd_status write command cd status
* \param lcdid_addr lcd id address
* \return lcd id
* \brief  read v lcdc id
*             
* \author gang.li
* \note   call this after DTL_VLCDCInit.  
************************************************************************/
KDuint32 DTL_ReadLCDID(KDuint32 wcmd_cd_status,
                       KDuint32 lcdid_addr);
#endif
