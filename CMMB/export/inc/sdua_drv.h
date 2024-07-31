/******************************************************************************
 ** File Name:      sdua_drv.h                                                *
 ** Author:         Zhang Hanbing                                             *
 ** DATE:           06/12/2009                                                *
 ** Copyright:      2009 Spreadtrum, Incorporated. All Rights Reserved.       *
 ** Description:    mbbms communication driver between mobile and sd card     *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 06/12/2009     Zhang Hanbing        Create                                *
 ******************************************************************************/

  
#ifndef _SDUA_DRV_H
#define _SDUA_DRV_H

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "sci_types.h"

#ifdef __cplusplus
extern   "C"
{
#endif

/**---------------------------------------------------------------------------*
 **                         MACRO Definations                                 *
 **---------------------------------------------------------------------------*/
#define CMCC_IHS_DATA_V1_LEN  32
/**---------------------------------------------------------------------------*
 **                         Data Structures                                   *
 **---------------------------------------------------------------------------*/
typedef struct sdua_config_tag
{
    uint32 slot_num;
    uint32 begin_sector;
    uint32 sector_num;
}SDUA_CONFIG_T;

//communication head
typedef struct sdua_comm_head_tag
{
    uint8  ihs[CMCC_IHS_DATA_V1_LEN];  //identification head string
    uint32 data_len; 
}SDUA_COMM_HEAD_T;


//communication data
#ifdef WIN32
typedef  struct  sdua_comm_data_tag
#else
typedef __packed struct  sdua_comm_data_tag
#endif
{
    uint8  frame_tag;
    uint32 length;
    //uint8* payload;
}SDUA_COMM_DATA_T;


typedef enum sdua_opt_result_tag
{
    SDUA_SUCCESS,
    SDUA_READ_FAIL,
    SDUA_WRITE_FAIL,
    SDUA_HEADER_ERROR,
    SDUA_SIZE_ERROR,
    SDUA_NEED_RETRY,
    SDUA_MAX
}SDUA_OPT_RESULT_E;

/**---------------------------------------------------------------------------*
 **                         Global Variables                                  *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         Constant Variables                                *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         Function Prototypes                               *
 **---------------------------------------------------------------------------*/
/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/    
#ifdef __cplusplus
}
#endif

#endif  
