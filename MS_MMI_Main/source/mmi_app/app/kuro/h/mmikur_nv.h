#ifdef KURO_SUPPORT
/*************************************************************************
 ** File Name:      mmikur_nv.h                                          *
 ** Author:         cmzs                                           *
 ** Date:           2007/10/18                                           *
 ** Description:     This file defines the function about nv             *
 *************************************************************************
 *************************************************************************
 **                        Edit History                                  *
 ** ---------------------------------------------------------------------*
 ** DATE           NAME             DESCRIPTION                          *
 ** 2007/10/18     cmzs               Create.                             *
*************************************************************************/

#ifndef _MMIKUR_NV_H_
#define _MMIKUR_NV_H_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "sci_types.h"
#include "mmi_module.h"
#include "klrfunction.h"
/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif

#define MMINV_READ(_item_id,_addr,_ret_value)\
_ret_value = MMI_ReadNVItem(_item_id, _addr)

#define MMINV_WRITE(_item_id,_addr) \
             MMI_WriteNVItem(_item_id, _addr)

/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
#define MMINV_MAX_KGC_RECORDS 250
/**--------------------------------------------------------------------------*
 **                         TYPE AND CONSTANT                                *
 **--------------------------------------------------------------------------*/
typedef enum
{
    MMINV_KUR_SET_INFO = MMI_MODULE_KURO << 16,
    MMINV_KDB_SYS_DAT,
    MMINV_KURO_KGC_COUNT,
    MMINV_KURO_KGC_FIRST_ENTRY,
    MMINV_KURO_KGC_LAST_ENTRY = MMINV_KURO_KGC_FIRST_ENTRY + MMINV_MAX_KGC_RECORDS -1,
    MMIKUR_MAX_NV_ITEM_NUM
} MMIKUR_NV_ITEM_E;


/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description :read KGC infos in NV by MMI
//  Global resource dependence :  
//  Author:Cmzs
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIKURO_NV_ReadKgcInfo(                                      //RETURN:
                              uint16                        rid,        //IN:
                              KGC_INFO  *item_ptr   //OUT:
                              );

/*****************************************************************************/
//  Description :write Kuro Kgc Info in NV by MMI
//  Global resource dependence :  
//  Author:cmzs 
//  Note:
/*****************************************************************************/ 
PUBLIC BOOLEAN MMIKURO_NV_WriteKgcInfo(                                         //RETURN:
                                BOOLEAN                 is_need_sync,   //IN:
                                uint16                  rid,            //IN:
                                KGC_INFO        *item_ptr       //IN:
                                );
                                
/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif //_MMIKUR_NV_H_
#endif

