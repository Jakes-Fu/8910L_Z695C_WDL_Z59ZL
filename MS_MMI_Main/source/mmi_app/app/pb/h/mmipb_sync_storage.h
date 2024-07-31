/*****************************************************************************
** File Name:      mmipb_sync_storage.h                                         *
** Author:         wang.kun                                                          *
** Date:           07/09/2015                                                           *
** Copyright:      2015 Spreadtrum, Incorporated. All Rights Reserved.  *
** Description:    This file deal with sync data storage .                                  *             
**
***************************************************************************/
#ifdef MMIPB_SYNC_WITH_O365

#ifndef _MMIPB_SYNC_STORAGE_H_
#define _MMIPB_SYNC_STORAGE_H_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmk_type.h"
#include "mmi_custom_define.h"
#include "mmipb_common.h"
#include "mmipb_adapt.h"
#include "mmipb_interface.h"
#include "mmipb_sync_datalist.h"

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
 **                         phone contacts update info                                                                           *
 **--------------------------------------------------------------------------*/


/**--------------------------------------------------------------------------*
 **                         office365 contacts update info                                                                           *
 **--------------------------------------------------------------------------*/

/********************************************************************************
** Description: This function is used to initalze for pb sync storage                      *
** Global Resource Dependency:                                                  *
** Author: wang.kun                                                     *
** Input:
** Note: 07/11/2015                                                                     *
*********************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_SYNC_NV_Initalize(void);

/********************************************************************************
** Description: This function is used to save one phone update item to file                      *
** Global Resource Dependency:                                                  *
** Author: wang.kun                                                     *
** Input:
** Note: 07/11/2015                                                                     *
*********************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_SYNC_Write_Phone_Update_Item(uint16* index, MMIPB_PHONE_UPDATE_INFO_T* item);

/********************************************************************************
** Description: This function is used to read one phone update item from file                      *
** Global Resource Dependency:                                                  *
** Author: wang.kun                                                     *
** Input:
** Note: 07/11/2015                                                                     *
*********************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_SYNC_Read_Phone_Update_Item(uint16 index, MMIPB_PHONE_UPDATE_INFO_T* item);

/********************************************************************************
** Description: This function is used to read  all phone update items from file                      *
** Global Resource Dependency:                                                  *
** Author: wang.kun                                                     *
** Input:
** Note: 07/11/2015                                                                     *
*********************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_SYNC_Read_All_Phone_Update_Item(void);

/********************************************************************************
** Description: This function is used to delete one phone update item in file                      *
** Global Resource Dependency:                                                  *
** Author: wang.kun                                                     *
** Input:
** Note: 07/11/2015                                                                     *
*********************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_SYNC_Del_Phone_Update_Item(uint16 index);

/********************************************************************************
** Description: This function is used to delete all phone update item in file                      *
** Global Resource Dependency:                                                  *
** Author: wang.kun                                                     *
** Input:
** Note: 07/11/2015                                                                     *
*********************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_SYNC_Del_All_Phone_Update_Item(void);

/* Del cause they were defined in mmipb_datalist.c by SPRD */
#if 1
/********************************************************************************
** Description: This function is used to reset items in NV                     *
** Global Resource Dependency:                                                  *
** Author: wang.kun                                                *
** Input:
** Note: 07/11/2015                                                                     *
*********************************************************************************/
PUBLIC BOOLEAN  MMIPB_SYNC_NV_Is_HeartBeat_On(void);

/********************************************************************************
** Description: This function is used to reset items in NV                     *
** Global Resource Dependency:                                                  *
** Author: wang.kun                                                     *
** Input:
** Note: 07/11/2015                                                                     *
*********************************************************************************/
PUBLIC BOOLEAN  MMIPB_SYNC_NV_Set_HeartBeat(BOOLEAN value);
#endif

/********************************************************************************
** Description: This function is used to reset items in NV                     *
** Global Resource Dependency:                                                  *
** Author: wang.kun                                                *
** Input:
** Note: 07/11/2015                                                                     *
*********************************************************************************/
PUBLIC BOOLEAN  MMIPB_SYNC_NV_Is_First_Sync(void);

/********************************************************************************
** Description: This function is used to reset items in NV                     *
** Global Resource Dependency:                                                  *
** Author: wang.kun                                                     *
** Input:
** Note: 07/11/2015                                                                     *
*********************************************************************************/
PUBLIC BOOLEAN  MMIPB_SYNC_NV_Set_First_Sync(BOOLEAN value);

/********************************************************************************
** Description: This function is used to reset items in NV                     *
** Global Resource Dependency:                                                  *
** Author: wang.kun                                                *
** Input:
** Note: 07/11/2015                                                                     *
*********************************************************************************/
PUBLIC BOOLEAN  MMIPB_SYNC_NV_Is_MSA_Sign_In(void);

/********************************************************************************
** Description: This function is used to reset items in NV                     *
** Global Resource Dependency:                                                  *
** Author: wang.kun                                                     *
** Input:
** Note: 07/11/2015                                                                     *
*********************************************************************************/
PUBLIC BOOLEAN  MMIPB_SYNC_NV_Set_MSA_Sign_In(BOOLEAN value);

/********************************************************************************
** Description: This function is used to reset items in NV                     *
** Global Resource Dependency:                                                  *
** Author: wang.kun                                                     *
** Input:
** Note: 07/11/2015                                                                     *
*********************************************************************************/
PUBLIC void MMIPB_SYNC_NV_Reset_SyncTimeInfo(void);


/********************************************************************************
** Description: This function is used to read sync time info from NV                     *
** Global Resource Dependency:                                                  *
** Author: wang.kun                                                     *
** Input:
** Note: 07/11/2015                                                                     *
*********************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_SYNC_NV_Read_SyncTimeInfo(MMIPB_SYNC_TIME_INFO_T *p_SyncTimeInfo);


/********************************************************************************
** Description: This function is used to write sync time info to NV                     *
** Global Resource Dependency:                                                  *
** Author: wang.kun                                                     *
** Input:
** Note: 07/11/2015                                                                     *
*********************************************************************************/
PUBLIC MMIPB_ERROR_E MMIPB_SYNC_NV_Write_SyncTimeInfo(MMIPB_SYNC_TIME_INFO_T *p_SyncTimeInfo);


#ifdef   __cplusplus
    }
#endif

#endif
#endif
