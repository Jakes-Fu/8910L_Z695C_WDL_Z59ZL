#ifndef _MMISFR_MAIN_API_H_
#define _MMISFR_MAIN_API_H_

/**--------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/


/**--------------------------------------------------------------------------*
**                         FUNCTION DEFINITION                              *
**--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : Retry when failed
//  Param : @is_in_register: current process, register or update
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC void MMICMCCDM_Update (int loop);

/*****************************************************************************/
//  Description :Retry to update when successful
//  Param :@first_update: Time interval is different when first
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC void MMICMCCDM_ReTry(int seconds);

#endif

