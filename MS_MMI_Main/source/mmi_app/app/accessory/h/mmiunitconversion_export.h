/*****************************************************************************
** File Name:        mmiunitconversion_export.h                                                
** Author:           renyi.hu  
** Date:             2010/04/21
** Copyright:        2010 Spreadtrum, Incoporated. All Rights Reserved.         *
** Description:      This file is used to describe DATA of unitconversion
******************************************************************************
**                         Important Edit History                                                                     *
** --------------------------------------------------------------------------*
** DATE              NAME           DESCRIPTION                              *
** 2010/04/21      renyi.hu          Create                                 *
******************************************************************************/
#ifndef _MMI_UNITCONVERSION_EXPORT_H_
#define _MMI_UNITCONVERSION_EXPORT_H_


/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif

#ifdef  MMI_UNITCONVERSION_SUPPORT

/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/

        
/**--------------------------------------------------------------------------*
 **                         TYPE DEFINITION                                  *
 **--------------------------------------------------------------------------*/
typedef struct
{
	uint32 num;//转换后的数值
	uint32 expand;//转换后数值扩大的倍数.
}UC_STR_TO_NUM_T;

#endif//MMI_UNITCONVERSION_SUPPORT

#ifdef CUSTOM_UNITCONVERSION_SUPPORT
#define UC_CONV_CUSTOMIZE_MAX_NUM           10

#define UC_CONV_CUSTOMIZE_TITLE_LEN         12
#define UC_CONV_CUSTOMIZE_INPUT_NAME_LEN    12
#define UC_CONV_CUSTOMIZE_OUTPUT_NAME_LEN   12
#define UC_CONV_CUSTOMIZE_FACTOR_LEN        12

typedef struct
{
    BOOLEAN     convEnable;
    uint16       title[UC_CONV_CUSTOMIZE_TITLE_LEN + 1];
    uint16       inputName[UC_CONV_CUSTOMIZE_INPUT_NAME_LEN + 1];
    uint16       outputName[UC_CONV_CUSTOMIZE_OUTPUT_NAME_LEN + 1];
    uint16       factor[UC_CONV_CUSTOMIZE_FACTOR_LEN + 1];
}UC_CONV_CUSTOMIZE_ITEM_T;

typedef struct
{
    uint32 convNumber;
    UC_CONV_CUSTOMIZE_ITEM_T    convItem[UC_CONV_CUSTOMIZE_MAX_NUM];
}UC_CONV_CUSTOMIZE_T;

PUBLIC void MMIAPIUNITCONV_RegModuleNv(void);

#endif
/**--------------------------------------------------------------------------*
 **                         GLOBAL FUNCTIONS
 **--------------------------------------------------------------------------*/
 /*****************************************************************************/
// 	Description : the entry of unit conversion
//	Global resource dependence : 
//  Author:renyi.hu
//	Note: 
/*****************************************************************************/
  PUBLIC void MMIAPIUNITCONV_OpenMainWin(void);

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif  


