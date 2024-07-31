/***************************************************************************************
** File Name:      dtl_recovery.h                                                      *
** DATE:           25/06/2008                                                          *
** Copyright:      2008 Spreadtrum, Incorporated. All Rights Reserved.                 *
** Description:                                                                        *
****************************************************************************************/


#ifndef _DTL_RECOVERY_H
#define _DTL_RECOVERY_H

/**------------------------------------------------------------------------------------*
 **                         Dependencies           	                                   *
 **------------------------------------------------------------------------------------*/
#include "kd.h"

/**------------------------------------------------------------------------------------*
 **                        Compiler Flag                                               *
 **------------------------------------------------------------------------------------*/
#ifdef __cplusplus
extern "C"
{
#endif


/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/

//dtl error info 
typedef enum dtl_error_info_tag
{
    DTL_ERR_POWERWARN,
    DTL_ERR_GETCMD,
    DTL_ERR_SENDCMD,
    DTL_ERR_GETDATA,
    DTL_ERR_SENDDATA,
    DTL_ERR_SEINE_ARM_ASSERT,
    DTL_ERR_SEINE_DSP_ASSERT,
    //caution:the upper is serious error,if not serious,add item to the tail
    DTL_ERR_SPIDMAREAD_TIMEOUT,
    DTL_ERR_SPIDMAWRITE_TIMEOUT,
    DTL_ERR_MAX
}DTL_ERR_INFO_E;

/**---------------------------------------------------------------------------*
 **                         Function Propertype                               *
 **---------------------------------------------------------------------------*/
/************************************************************************
* \fn  void (* DTL_RECOVER_CALLBACK)(ack_id ,p0,p1,p2)
* \param  ack_id ,p0,p1,p2
* \return void 
* \brief  callback function when dtl error 
*             
* \author Hanbing.Zhang
* \note   Nothing.  
************************************************************************/
typedef void (* DTL_RECOVER_CALLBACK)(    
    KDuint32 ack_id, 
    KDuint32 para0,
    KDuint32 para1,
    KDuint32 para2
    );

/************************************************************************
* \fn  void DTLR_SetCallback(DTL_RECOVER_CALLBACK callback)
* \param  DTL_RECOVER_CALLBACK
* \return void 
* \brief  set callback function pointer
*             
* \author Hanbing.Zhang
* \note   Nothing.  
************************************************************************/
void DTLR_SetCallback(DTL_RECOVER_CALLBACK callback);

/************************************************************************
* \fn  DTL_RECOVER_CALLBACK DTLR_GetCallback(void)
* \param  void
* \return DTL_RECOVER_CALLBACK 
* \brief  get callback function pointer
*             
* \author Hanbing.Zhang
* \note   Nothing.  
************************************************************************/
DTL_RECOVER_CALLBACK DTLR_GetCallback(void);

/************************************************************************
* \fn  void DTLR_DelCallback(void)
* \param  void
* \return void 
* \brief  delete callback function
*             
* \author Hanbing.Zhang
* \note   Nothing.  
************************************************************************/
void DTLR_DelCallback(void);

/************************************************************************
* \fn  void DTLR_SetParam(KDuint32 msg_id);
* \param  msg_id
* \return void 
* \brief  set param
*             
* \author Hanbing.Zhang
* \note   Nothing.  
************************************************************************/
void DTLR_SetParam(KDuint32 msg_id);

/************************************************************************
* \fn  KDuint32 DTLR_GetParam(void);
* \param  param
* \return void 
* \brief  get param
*             
* \author Hanbing.Zhang
* \note   Nothing.  
************************************************************************/
KDuint32 DTLR_GetParam(void);

#ifdef __cplusplus
}
#endif


#endif