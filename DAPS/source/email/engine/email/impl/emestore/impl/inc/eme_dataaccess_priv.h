/*==============================================================================
    FILE NAME   : eme_dataaccess_priv.h
    MODULE NAME : 


    GENERAL DESCRIPTION:
        Contain private structure and constant definitions.

    TechFaith Software Confidential Proprietary
    Copyright (c) 2006-2007 by TechFaith Software. All Rights Reserved.
================================================================================
    Revision History

    Modification                  Tracking
       Date          Author        Number          Description of changes
    ----------   --------------   --------   ----------------------------------
    2007-09-19   Create           01         Create

==============================================================================*/
#ifndef EME_DATAACCESS_PRIV_H
#define EME_DATAACCESS_PRIV_H

/*------------------------------------------------------------------------------
 *    Include Files
 *----------------------------------------------------------------------------*/
#include "mmiemail_def.h"
#include "eme_dataaccess_api.h"
#include "eme_utils_api.h"

#define EMEFS_FILE_MAX_LEN             64

#define EMEFS_FILE_ACTINDEX            "actindex.tmf"
#define EMEFS_FILE_BOXINDEX            "boxindex.tmf"
#define EMEFS_FILE_MSGINDEX            "msgindex.tmf"
#define EMEFS_FILE_ACTDATA             "actdata.tmf"
/*
 *	Email file system definitions
 */
#define EMEFS_UDISK_ROOT               EME_DataAccess_GetPath(EFSTYPE_UDISK_ROOT)
#define EMEFS_SDCARD1_ROOT             EME_DataAccess_GetPath(EFSTYPE_SDCARD1_ROOT)
#define EMEFS_SDCARD2_ROOT             EME_DataAccess_GetPath(EFSTYPE_SDCARD2_ROOT)

#define EMEFS_ACT_ROOT_DIR             EME_DataAccess_GetPath(EFSTYPE_ACT_ROOT_DIR)
#define EMEFS_ACT_INDEX_FILE           EME_DataAccess_GetPath(EFSTYPE_ACT_INDEX_FILE)

#define EMEFS_TEMP_UDISK_DIR           EME_DataAccess_GetPath(EFSTYPE_TEMP_UDISK_DIR)
#define EMEFS_TEMP_SDCARD1_DIR         EME_DataAccess_GetPath(EFSTYPE_TEMP_SDCARD1_DIR)
#define EMEFS_TEMP_SDCARD2_DIR         EME_DataAccess_GetPath(EFSTYPE_TEMP_SDCARD2_DIR)
#define EMEFS_TEMP_ROOT_DIR            EME_DataAccess_GetTempDir(NULL)

#define EMEFS_UDISK_EML_ROOT_DIR       EME_DataAccess_GetPath(EFSTYPE_UDISK_EML_ROOT_DIR)
#define EMEFS_SDCARD1_EML_ROOT_DIR     EME_DataAccess_GetPath(EFSTYPE_SDCARD1_EML_ROOT_DIR)
#define EMEFS_SDCARD2_EML_ROOT_DIR     EME_DataAccess_GetPath(EFSTYPE_SDCARD2_EML_ROOT_DIR)

////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
#define EMEFS_ACCONT_NAME              "act%04x"

/*email data file*/
#define EMEFS_MSG_DATA_FILE_UDISK      EME_DataAccess_GetPath(EFSTYPE_MSG_DATA_FILE_UDISK)
#define EMEFS_MSG_DATA_FILE_SDCARD1    EME_DataAccess_GetPath(EFSTYPE_MSG_DATA_FILE_SDCARD1)
#define EMEFS_MSG_DATA_FILE_SDCARD2    EME_DataAccess_GetPath(EFSTYPE_MSG_DATA_FILE_SDCARD2)


#define EMEFS_MSG_DATA_FOLDER_UDISK    EME_DataAccess_GetPath(EFSTYPE_MSG_DATA_FOLDER_UDISK)
#define EMEFS_MSG_DATA_FOLDER_SDCARD1  EME_DataAccess_GetPath(EFSTYPE_MSG_DATA_FOLDER_SDCARD1)
#define EMEFS_MSG_DATA_FOLDER_SDCARD2  EME_DataAccess_GetPath(EFSTYPE_MSG_DATA_FOLDER_SDCARD2)

/*account info file*/
#define EMEFS_ACT_DATA_FILE            EME_DataAccess_GetPath(EFSTYPE_ACT_DATA_FILE)
#define EMEFS_BOX_INDEX_FILE           EME_DataAccess_GetPath(EFSTYPE_BOX_INDEX_FILE)
#define EMEFS_MSG_INDEX_FILE           EME_DataAccess_GetPath(EFSTYPE_MSG_INDEX_FILE)
#define EMEFS_ACT_UIDL_FILE            EME_DataAccess_GetPath(EFSTYPE_ACT_UIDL_FILE)
#define EMEFS_ACT_UID_FILE             EME_DataAccess_GetPath(EFSTYPE_ACT_UID_FILE)
#define EMEFS_ACT_SIG_FILE             EME_DataAccess_GetPath(EFSTYPE_ACT_SIG_FILE)
#define EMEFS_ACT_DATA_FOLDER          EME_DataAccess_GetPath(EFSTYPE_ACT_DATA_FOLDER)



typedef enum
{
   EFSTYPE_UDISK_ROOT = 0,
   EFSTYPE_SDCARD1_ROOT,
   EFSTYPE_SDCARD2_ROOT,
   
   EFSTYPE_ACT_ROOT_DIR,
   EFSTYPE_ACT_INDEX_FILE,
   
   EFSTYPE_TEMP_UDISK_DIR,
   EFSTYPE_TEMP_SDCARD1_DIR,
   EFSTYPE_TEMP_SDCARD2_DIR,
   
   EFSTYPE_UDISK_EML_ROOT_DIR,
   EFSTYPE_SDCARD1_EML_ROOT_DIR,
   EFSTYPE_SDCARD2_EML_ROOT_DIR,
   
   /*email data file*/
   EFSTYPE_MSG_DATA_FILE_UDISK,
   EFSTYPE_MSG_DATA_FILE_SDCARD1,
   EFSTYPE_MSG_DATA_FILE_SDCARD2,
   
   EFSTYPE_MSG_DATA_FOLDER_UDISK,
   EFSTYPE_MSG_DATA_FOLDER_SDCARD1,
   EFSTYPE_MSG_DATA_FOLDER_SDCARD2,
   
   /*account info file*/
   EFSTYPE_ACT_DATA_FILE,
   EFSTYPE_BOX_INDEX_FILE,
   EFSTYPE_MSG_INDEX_FILE,
   EFSTYPE_ACT_UIDL_FILE,
   EFSTYPE_ACT_UID_FILE,
   EFSTYPE_ACT_SIG_FILE,
   EFSTYPE_ACT_DATA_FOLDER,
   
   EFSTYPE_MAX
}EFSType;
WE_CHAR* EME_DataAccess_GetPath(EFSType eType);

#endif /* EME_DATAACCESS_PRIV_H */
