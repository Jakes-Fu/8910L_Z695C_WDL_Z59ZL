/******************************************************************************
 ** File Name:      dtl_recovery.c                                            *
 ** Author:         Hanbing.Zhang                                             *
 ** DATE:           25/06/2008                                                *
 ** Copyright:      2004 Spreadtrum, Incorporated. All Rights Reserved.       *
 ** Description:    dtl error callback handle                                 *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME                 DESCRIPTION                           *
 ** 25/06/2008      Hanbing.Zhang        Create.                              *
 *****************************************************************************/

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "dtl_recovery.h"
#include "dtl.h"

/**---------------------------------------------------------------------------*
 **                         Debugging Flag                                    *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         Macro Definition                                   *
 **---------------------------------------------------------------------------*/
LOCAL DTL_RECOVER_CALLBACK s_dtl_recovery_callback_func = PNULL;
LOCAL uint32 s_dtl_recovery_msg_id = 0;
/**---------------------------------------------------------------------------*
 **                      Function Declaration                                 *
 **---------------------------------------------------------------------------*/

/************************************************************************
* \fn  void DTLR_SetCallback(DTL_RECOVER_CALLBACK callback)
* \param  DTL_RECOVER_CALLBACK
* \return void 
* \brief  set callback function pointer
*             
* \author Hanbing.Zhang
* \note   Nothing.  
************************************************************************/
void DTLR_SetCallback(DTL_RECOVER_CALLBACK callback)
{
    if( callback )
    {
        s_dtl_recovery_callback_func = callback;
    }
}

/************************************************************************
* \fn  DTL_RECOVER_CALLBACK DTLR_GetCallback(void)
* \param  void
* \return DTL_RECOVER_CALLBACK 
* \brief  get callback function pointer
*             
* \author Hanbing.Zhang
* \note   Nothing.  
************************************************************************/
DTL_RECOVER_CALLBACK DTLR_GetCallback(void)
{
    return s_dtl_recovery_callback_func;
}

/************************************************************************
* \fn  void DTLR_DelCallback(void)
* \param  void
* \return void 
* \brief  delete callback function
*             
* \author Hanbing.Zhang
* \note   Nothing.  
************************************************************************/
void DTLR_DelCallback(void)
{
    s_dtl_recovery_callback_func = PNULL;
    
}

/************************************************************************
* \fn  void DTLR_SetParam(KDuint32 msg_id);
* \param  msg_id
* \return void 
* \brief  set param
*             
* \author Hanbing.Zhang
* \note   Nothing.  
************************************************************************/
void DTLR_SetParam(KDuint32 msg_id)
{
    s_dtl_recovery_msg_id = msg_id;
}

/************************************************************************
* \fn  KDuint32 DTLR_GetParam(void);
* \param  param
* \return void 
* \brief  get param
*             
* \author Hanbing.Zhang
* \note   Nothing.  
************************************************************************/
KDuint32 DTLR_GetParam(void)
{
    return s_dtl_recovery_msg_id;
}

/************************************************************************
* \fn  KDboolean DTLR_Recovery(DTL_ERR_INFO_E err_type)
* \param DTL_ERR_INFO_E err_type
* \return void 
* \brief  call callback 
*             
* \author Hanbing.Zhang
* \note   Nothing.  
************************************************************************/
void DTLR_Recovery(DTL_ERR_INFO_E err_type)
{
    DTL_RECOVER_CALLBACK callback = PNULL;
    
    DTL_SetErrorInfo(err_type);
    callback = DTLR_GetCallback();
    if(callback)
    {        
        callback(DTLR_GetParam(),err_type,0,0);
    }
}


