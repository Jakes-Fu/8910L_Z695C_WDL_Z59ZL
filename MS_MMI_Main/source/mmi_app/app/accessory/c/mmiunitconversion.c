/****************************************************************************
** File Name:      mmiunitconversion.c                              
** Author:           renyi.hu                                                    
** Date:            2010/04/21
** Copyright:      2010 Spreadtrum, Incorporated. All Rights Reserved.       
** Description:    This file is used to describe functions of unit conversion
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE               NAME             DESCRIPTION                             
** 21/04/2010     renyi.hu          Create

****************************************************************************/

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "mmi_app_accessory_trc.h"
#include "sci_types.h" 
#ifdef  MMI_UNITCONVERSION_SUPPORT
#include "window_parse.h"
#include "mmidisplay_data.h"
#include "mmk_msg.h"
#include "mmi_image.h"
#include "mmi_common.h"
#include "mmi_nv.h"
#include "mmi_theme.h"
#include "guicommon.h"
#include "guitext.h"
#include "guiedit.h"
#include "guimenu.h"
#include "mmipub.h"
#include "mmi_menutable.h"
#include "mmiacc_id.h"
#include "guilabel.h"
#include "mmiacc_image.h"
#include "mmiacc_text.h"
#include "guires.h"
#include "mmiacc_position.h"
#include "mmiunitconversion_export.h"
#include "guiform.h"
#include "guisetlist.h"
#include "mmicom_banner.h"

/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/
#ifdef CUSTOM_UNITCONVERSION_SUPPORT
#include "mmiacc_nv.h"

#define  MMIUC_NUMBER_MAX_LEN 12
#define  MMIUC_INPUT_MAX_LEN  (MMIUC_NUMBER_MAX_LEN)  //编辑框输入最大字符数
#define  MMIUC_OUTPUT_MAX_LEN MMIUC_NUMBER_MAX_LEN    //最大输出长度
#define  MMIUC_MAX_STR_BUFF_LEN 30    //字符buff大小

#define  MMIUC_CHAR_ZERO            '0'
#define  MMIUC_CHAR_ONE             '1'
#define  MMIUC_CHAR_TWO             '2'
#define  MMIUC_CHAR_THREE           '3'
#define  MMIUC_CHAR_FOUR            '4'
#define  MMIUC_CHAR_FIVE            '5'
#define  MMIUC_CHAR_SIX             '6'
#define  MMIUC_CHAR_SEVEN           '7'
#define  MMIUC_CHAR_EIGHT           '8'
#define  MMIUC_CHAR_NINE            '9'
#define  MMIUC_CHAR_SEMICOLON       '/'
#define  MMIUC_CHAR_NEGATIVE        '-'
#define  MMIUC_CHAR_POSITIVE        '+'
#define  MMIUC_CHAR_DOT             '.'

#define  MMIUC_EDIT_CTRL_1_INIT     0x0001
#define  MMIUC_EDIT_CTRL_1_CHANGED  0x0002

#define  MMIUC_EDIT_CTRL_2_INIT     0x0010
#define  MMIUC_EDIT_CTRL_2_CHANGED  0x0020

#define  MMIUC_EDIT_CTRL_3_INIT     0x0100
#define  MMIUC_EDIT_CTRL_3_CHANGED  0x0200

#define  MMIUC_EDIT_CTRL_4_INIT     0x1000
#define  MMIUC_EDIT_CTRL_4_CHANGED  0x2000


typedef enum
{
    UC_CONV_DIRECTION_POSITIVE,
    UC_CONV_DIRECTION_NEGATIVE,
    UC_CONV_DIRECTION_NUM
}UC_CONV_DIRECTION_E;

typedef enum
{
    UC_CONV_FORMAT_CHAR_ORIGINAL,
    UC_CONV_FORMAT_CHAR_CHANGED
}UC_CONV_FORMAT_CHAR_E;

typedef enum
{/*lint !e749*/
    CONVERTER_OPT_ROOT,/*lint !e749*/
    CONVERTER_OPT_NEW,
    CONVERTER_OPT_REMOVE,
    CONVERTER_OPT_MAX
}CONVERTER_OPTION_MENU_NODE_E;/*lint !e751*/


//最大自定义个数
typedef uint32      UC_CONV_SUBSET_TYPE;

/**--------------------------------------------------------------------------*
 **                         TYPE DEFINITION                                  *
 **--------------------------------------------------------------------------*/
//如果需要扩展该枚举,注意是否需要
//修改宏:MMIUC_WEIGHT_TYPE_NUM
typedef enum
{
    //重量转换类型
    MMIUC_TYPE_KG_POUND ,
    MMIUC_TYPE_POUND_KG ,
    MMIUC_TYPE_KG_OUNCE , 
    MMIUC_TYPE_OUNCE_KG  ,

    //长度转换类型
    MMIUC_TYPE_KM_MILE , 
    MMIUC_TYPE_MILE_KM  ,
    MMIUC_TYPE_M_YARD ,
    MMIUC_TYPE_YARD_M   ,
    MMIUC_TYPE_M_FOOT, 
    MMIUC_TYPE_FOOT_M ,   
    MMIUC_TYPE_CM_INCH , 
    MMIUC_TYPE_INCH_CM  ,

    MMIUC_TYPE_MAX
}UC_CONV_TYPE_E;

typedef enum
{
    UC_TYPE_WEIGHT,
    UC_TYPE_LENGTH,
    UC_TYPE_MAX
}UC_TYPE_E;

typedef enum
{
    UC_WEIGHT_KILOGRAM,
    UC_WEIGHT_POUND,
    UC_WEIGHT_OUNCE,
    UC_WEIGHT_MAX
}UC_WEIGHT_E;

typedef enum
{
    UC_LENGTH_KILOMETER,
    UC_LENGTH_METER,
    UC_LENGTH_CENTIMETER,
    UC_LENGTH_MILE,
    UC_LENGTH_YARD,
    UC_LENGTH_FOOT,
    UC_LENGTH_INCH,
    UC_LENGTH_MAX
}UC_LENGTH_E;

typedef enum
{
    UC_CONV_MAIN_TEMPERATURE,
    UC_CONV_MAIN_WEIGHT,
    UC_CONV_MAIN_LENGTH,
    UC_CONV_MAIN_AREA,
    UC_CONV_MAIN_VOLUME,
    UC_CONV_MAIN_CURRENCY,
    UC_CONV_MAIN_PREDEFINED_NUM,
    UC_CONV_MAIN_CUSTOMIZE_1 = UC_CONV_MAIN_PREDEFINED_NUM,
    UC_CONV_MAIN_TYPE_NUM = UC_CONV_MAIN_PREDEFINED_NUM + UC_CONV_CUSTOMIZE_MAX_NUM
}UC_CONV_MAIN_TYPE_E;

typedef enum
{
    UC_CONV_SUBSET_TEMPURE_F_C,
    UC_CONV_SUBSET_TEMPURE_F_K,
    UC_CONV_SUBSET_TEMPURE_C_K,
    UC_CONV_SUBSET_TEMPURE_TYPE_NUM
}UC_CONV_SUBSET_TEMPTURE_TYPE_E;

typedef enum
{
    UC_CONV_SUBSET_WEIGHT_POUND_KILO,
    UC_CONV_SUBSET_WEIGHT_CARAT_GRAM,
    UC_CONV_SUBSET_WEIGHT_OUNCE_GRAM,
    UC_CONV_SUBSET_WEIGHT_TROYOUNCE_GRAM,
    UC_CONV_SUBSET_WEIGHT_STONE_KILO,
    UC_CONV_SUBSET_WEIGHT_TYPE_NUM
}UC_CONV_SUBSET_WEIGHT_TYPE_E;

typedef enum
{
    UC_CONV_SUBSET_LENGHT_MILE_KILOMETRE,
    UC_CONV_SUBSET_LENGHT_YARD_METRE,
    UC_CONV_SUBSET_LENGHT_FOOT_METRE,
    UC_CONV_SUBSET_LENGHT_FOOT_CENTIMETRE,
    UC_CONV_SUBSET_LENGHT_INCH_CENTIMETRE,
    UC_CONV_SUBSET_LENGHT_TYPE_NUM
}UC_CONV_SUBSET_LENGHT_TYPE_E;

typedef enum
{
    UC_CONV_SUBSET_AREA_FOOT_METRE,
    UC_CONV_SUBSET_AREA_INCH_CENTIMETRE,
    UC_CONV_SUBSET_AREA_YARD_METRE,
    UC_CONV_SUBSET_AREA_MILE_KILOMETRE,
    UC_CONV_SUBSET_AREA_ACRE_HECTARE,
    UC_CONV_SUBSET_AREA_TYPE_NUM
}UC_CONV_SUBSET_AREA_TYPE_E;

typedef enum
{
    UC_CONV_SUBSET_VOLUME_INCH_CENTIMETRE,
    UC_CONV_SUBSET_VOLUME_FOOT_METRE,
    UC_CONV_SUBSET_VOLUME_YARD_METRE,
    UC_CONV_SUBSET_VOLUME_GALLON_LITRE,
    UC_CONV_SUBSET_VOLUME_TYPE_NUM
}UC_CONV_SUBSET_VOLUME_TYPE_E;

typedef enum
{
    UC_CONV_SUBSET_CUSTOMIZE_CONVERT,
    UC_CONV_SUBSET_CUSTOMIZE_NEW,
    UC_CONV_SUBSET_CUSTOMIZE_EDIT,
    UC_CONV_SUBSET_CUSTOMIZE_DELETE,
    UC_CONV_SUBSET_CUSTOMIZE_TYPE_NUM,
}UC_CONV_SUBSET_CUSTOMIZE_TYPE_E;

LOCAL UC_CONV_TYPE_E s_conversion_type=MMIUC_TYPE_MAX;//转换类型

/*
F = 32 + C*1.8
K = C + 273.15
1/1.8 = 0.55556
*/
LOCAL const char  *s_tempure_conversion_constant[UC_CONV_SUBSET_TEMPURE_TYPE_NUM][UC_CONV_DIRECTION_NUM]=
{
//  Fahrenheit          Celsius
    "-17.777778",        "32",
//  Kelvin              Fahrenheit
    "-459.67",          "255.372222",
//  Kelvin              Celsius
    "-273.15",          "273.15",
};

LOCAL const char  *s_tempure_conversion_rate[UC_CONV_SUBSET_TEMPURE_TYPE_NUM][UC_CONV_DIRECTION_NUM]=
{
//  Fahrenheit          Celsius
    "1/1.8",            "1.8",
//    "0.555556",          "1.8",
//  Kelvin              Fahrenheit
    "1.8",              "0.555556",
//  Kelvin              Celsius
    "1.0",              "1.0",
};

LOCAL const char  *s_weight_conversion_rate[UC_CONV_SUBSET_WEIGHT_TYPE_NUM][UC_CONV_DIRECTION_NUM]=
{
//  pound               Kilo
	"0.4536",           "2.204623",
//	"0.453592",         "2.204623",
//  Carat               Gram
    "0.2",              "5.0",
//  Ounce               Gram
    "28.35",            "0.035274",
//    "28.349523",        "0.035274",
//  Troy ounce          Gram
    "31.103477",        "0.032151",
//  Stone               Kilo
    "6.3502932",         "0.157473",
};

LOCAL const char  *s_length_conversion_rate[UC_CONV_SUBSET_LENGHT_TYPE_NUM][UC_CONV_DIRECTION_NUM]=
{
//  mile                km
    "1.609",            "0.621371",
//    "1.609344",         "0.621371",
//  yard                metre
    "0.9144",           "1.093613",
//  foot                metre
    "0.3048",           "3.28084",
//  foot                cm
    "30.48",            "0.032808",
//  inch                cm
    "2.5401",           "0.3937008",
};

LOCAL const char  *s_area_conversion_rate[UC_CONV_SUBSET_AREA_TYPE_NUM][UC_CONV_DIRECTION_NUM]=
{
//  foot2               metre2
    "0.0929",       "10.7639111056",
//    "0.09290304",       "10.7639111056",
//  inch2               cm2
    "6.4516",           "0.155",
//  yard2               metre2
    "0.8361274",         "1.19599",
//  mile2               km2
    "2.59",         "0.386102",
//    "2.589988",         "0.386102",
//  acre                hectare
    "0.4047",         "2.471054",
//    "0.404686",         "2.471054",
};

LOCAL const char  *s_volume_conversion_rate[UC_CONV_SUBSET_VOLUME_TYPE_NUM][UC_CONV_DIRECTION_NUM]=
{
//  inch3               cm3
    "16.387064",        "0.061024",
//  foot3               metre3
    "0.0283168",         "35.314667",
//  metre3              yard3
    "1.3079505",         "0.764555",
//  gallon              litre
    "3.78541",         "0.264172",
//    "3.785412",         "0.264172",
};

uint16 editCtrlState = 0x0000;

const uint16 converter_nv_len[] =
{
    sizeof(UC_CONV_CUSTOMIZE_ITEM_T) * UC_CONV_CUSTOMIZE_MAX_NUM,//MMINV_ACC_UNITCONV_CUSTOMIZE_ITEM_1
};

wchar  custDefaultFactor[] = {'1', '\/', '2','\0'};

LOCAL UC_CONV_CUSTOMIZE_T           custConvItem = {0, {0}};
LOCAL UC_CONV_CUSTOMIZE_ITEM_T      custConvChangeItem = {FALSE, {0}, {0}, {0}, {0}};
LOCAL uint32    custConvChangeItemId = 0;
LOCAL BOOLEAN custConvCanbeSave = FALSE;

LOCAL const double maxResultValue = 1.0e12;//MMIUC_OUTPUT_MAX_LEN;
LOCAL const double minResultValue = 0.0000000001;//(MMIUC_OUTPUT_MAX_LEN-2);1.0e-10;

LOCAL UC_CONV_FORMAT_CHAR_E InputFormat(
                            MMI_STRING_T const  *in_str_ptr,        //in
                            MMI_STRING_T        *in_str_format_ptr, //out
                            MMI_STRING_T        *calculate_str_ptr, //out
                            BOOLEAN             semicolon_enable,
                            BOOLEAN             sign_enable
                            );
LOCAL BOOLEAN InputIsValid(
							MMI_STRING_T const *in_str_ptr //in
							);
LOCAL BOOLEAN StrIsValid(
							MMI_STRING_T const *in_str_ptr //in
							);
LOCAL BOOLEAN  UnitConversion (
							UC_CONV_MAIN_TYPE_E  mainType,//in
							UC_CONV_SUBSET_TYPE  subType,//in
							UC_CONV_DIRECTION_E  direction,//in
							const MMI_STRING_T  *in_str_ptr, //in 
							MMI_STRING_T  *out_str_ptr//out
							);
LOCAL BOOLEAN  UnitConversionByRate(
									 const MMI_STRING_T  *in_str, //in 
									 const char *conv_rate, // in 
									 const char *conv_constant, // in 
                                     UC_CONV_DIRECTION_E  direction,//in
									 MMI_STRING_T  *out_str //out
									 );


LOCAL MMI_RESULT_E HandleUnitConvertMainTypeWinMsg(
    MMI_WIN_ID_T         win_id,
    MMI_MESSAGE_ID_E     msg_id,
    DPARAM               param
);
LOCAL MMI_RESULT_E HandleUnitConvertSubsetTypeWinMsg(
    MMI_WIN_ID_T         win_id,
    MMI_MESSAGE_ID_E     msg_id,
    DPARAM               param
);

LOCAL void UnitConv_SetMainTypeList(BOOLEAN beUpdate);
LOCAL void UnitConv_OpenSubsetTypeListWin(UC_CONV_MAIN_TYPE_E mainType);
LOCAL void UnitConv_SetSubsetTypeList(UC_CONV_MAIN_TYPE_E mainType, BOOLEAN beUpdate);
LOCAL void UnitConv_converterWin(UC_CONV_MAIN_TYPE_E mainType, UC_CONV_SUBSET_TYPE subType);
LOCAL void Init_ConverterWin(MMI_WIN_ID_T win_id, UC_CONV_MAIN_TYPE_E mainType, UC_CONV_SUBSET_TYPE subType);
LOCAL MMI_RESULT_E HandleUnitConverterWinMsg(
                                             MMI_WIN_ID_T         win_id,
                                             MMI_MESSAGE_ID_E     msg_id,
                                             DPARAM               param
                                             );

/*****************************************************************************/
//    Description : Converter list option window
//    Global resource dependence : 
//    Author: Cheney.Wu
//    Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleConverterListOptionWinMsg(
                                                   MMI_WIN_ID_T        win_id, 
                                                   MMI_MESSAGE_ID_E    msg_id, 
                                                   DPARAM              param
                                                   );

WINDOW_TABLE(UNITCONV_MIAN_WIN_TAB) =
{
    WIN_FUNC((uint32)HandleUnitConvertMainTypeWinMsg),
    WIN_ID(MMIUNITCONV_MAIN_WIN_ID),
    WIN_STATUSBAR,
    CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, MMIUNITCONV_MAIN_WIN_MENU_LIST_CTRL_ID),
    WIN_SOFTKEY(STXT_OPTION, STXT_SOFTKEY_OK_MK, TXT_COMMON_COMMON_BACK),
    END_WIN
};

WINDOW_TABLE(UNITCONV_SUBSET_WIN_TAB) =
{
    WIN_FUNC((uint32)HandleUnitConvertSubsetTypeWinMsg),
    WIN_ID(MMIUNITCONV_SUBSET_WIN_ID),
    WIN_STATUSBAR,
    CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, MMIUNITCONV_SUBSET_WIN_MENU_LIST_CTRL_ID),
    WIN_SOFTKEY(TXT_NULL, STXT_SOFTKEY_SELECT_MK, TXT_COMMON_COMMON_BACK),
    END_WIN
};

WINDOW_TABLE(UNITCONV_CONVERTER_WIN_TAB) =
{
    WIN_FUNC((uint32)HandleUnitConverterWinMsg),
    WIN_ID(MMIUNITCONV_CONVERTER_WIN_ID),
    WIN_STATUSBAR,
    CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER,MMIUNITCONV_CONVERTER_WIN_FORM_CTRL_ID),
    CHILD_EDIT_TEXT_CTRL(TRUE, (MMIUC_INPUT_MAX_LEN+1), MMIUNITCONV_CONVERTER_WIN_EDIT_1_CTRL_ID, MMIUNITCONV_CONVERTER_WIN_FORM_CTRL_ID),
    CHILD_EDIT_TEXT_CTRL(TRUE, (MMIUC_INPUT_MAX_LEN+1), MMIUNITCONV_CONVERTER_WIN_EDIT_2_CTRL_ID, MMIUNITCONV_CONVERTER_WIN_FORM_CTRL_ID),
    CHILD_EDIT_TEXT_CTRL(TRUE, (MMIUC_INPUT_MAX_LEN+1), MMIUNITCONV_CONVERTER_WIN_EDIT_3_CTRL_ID, MMIUNITCONV_CONVERTER_WIN_FORM_CTRL_ID),
    CHILD_EDIT_TEXT_CTRL(TRUE, (MMIUC_INPUT_MAX_LEN+1), MMIUNITCONV_CONVERTER_WIN_EDIT_4_CTRL_ID, MMIUNITCONV_CONVERTER_WIN_FORM_CTRL_ID),
    WIN_SOFTKEY(TXT_NULL, TXT_NULL, TXT_COMMON_COMMON_BACK),
    END_WIN
};

WINDOW_TABLE(MMIUNITCONV_CONVERTER_LIST_OPT_WIN_TAB) = 
{
    WIN_FUNC( (uint32)HandleConverterListOptionWinMsg),
        WIN_ID(MMIUNITCONV_CONVERTER_LIST_OPT_WIN_ID),
        WIN_STYLE( WS_HAS_TRANSPARENT),
#ifdef MMI_GUI_STYLE_MINISCREEN
        WIN_TITLE(TXT_NULL),
#endif
        WIN_SOFTKEY(TXT_COMMON_OK,TXT_NULL,STXT_RETURN),
        END_WIN
};
/**---------------------------------------------------------------------------*
 **                       FUNCTION BODY 
 **---------------------------------------------------------------------------*/
LOCAL void ConvDisplaybannerInfo(MMI_TEXT_ID_T text_id)
{
    GUI_POINT_T start_point = {0, 0};
    MMICOM_BANNER_INFO_T banner_info = {0};
    
    banner_info.banner_type = MMI_BANNER_TEXT;
    banner_info.start_point = start_point;
    banner_info.text1_id = text_id;
    MMIAPICOM_OpenBannerWin(PNULL, banner_info);
}
/*****************************************************************************/
//  Description :判断字符串是否合法
//  Global resource dependence : 
//  Author:renyi.hu
//  Note: 形式为整数或小数才合法
/*****************************************************************************/
LOCAL UC_CONV_FORMAT_CHAR_E InputFormat(
                            MMI_STRING_T const  *in_str_ptr,        //in
                            MMI_STRING_T        *in_str_format_ptr, //out
                            MMI_STRING_T        *calculate_str_ptr, //out
                            BOOLEAN             semicolon_enable,
                            BOOLEAN             sign_enable
                            )
{
    UC_CONV_FORMAT_CHAR_E result = UC_CONV_FORMAT_CHAR_ORIGINAL;
    int32       loop = 0;
    int32       dot_count = 0;
    wchar*      strFormatPtr = PNULL;
    wchar*      strCalculatePtr = PNULL;
    wchar*      strInputPtr = PNULL;
    uint16      strFormatLen = 0;
    uint16      strCalculateLen = 0;
    uint16      strInputLen = 0;
    uint16      semicolonCnt = 0;
    uint16      dotCnt = 0;

    //输入字串为空
    if(PNULL == in_str_ptr || 0 == in_str_ptr->wstr_len)
    {
        return UC_CONV_FORMAT_CHAR_ORIGINAL;
    }
    //输入字串超过最大
    if(in_str_ptr->wstr_len > MMIUC_INPUT_MAX_LEN)
    {
        return UC_CONV_FORMAT_CHAR_ORIGINAL;
    }
    //MMIAPICOM_Wstrncpy(inputFormatBuf, in_str_ptr->wstr_ptr, in_str_ptr->wstr_len);
    strFormatPtr = in_str_format_ptr->wstr_ptr;
    strFormatLen = 0;
    strCalculatePtr = calculate_str_ptr->wstr_ptr;
    strCalculateLen = 0;
    strInputPtr = in_str_ptr->wstr_ptr;
    strInputLen = in_str_ptr->wstr_len;

    //符号check
    if(MMIUC_CHAR_NEGATIVE == *strInputPtr || MMIUC_CHAR_POSITIVE == *strInputPtr)
    {
        if(sign_enable)
        {
            *(strFormatPtr++) = *(strInputPtr++);
            strFormatLen++;
            strInputLen--;
        }
    }

    if(strInputLen == 0)
    {
        return UC_CONV_FORMAT_CHAR_ORIGINAL;
    }

    for(loop = 0; loop < strInputLen; loop++)
    {
        switch(*strInputPtr)
        {
            case MMIUC_CHAR_ZERO:
                if(0 == loop)
                {
                    if(strInputLen == 1)
                    {
                        *(strFormatPtr++) = *(strInputPtr++);
                        strFormatLen++;
                    }
                    else
                    {
                        wchar aChar = *(strInputPtr + 1);
                        if(aChar >= MMIUC_CHAR_ZERO && aChar <= MMIUC_CHAR_NINE)
                        {
                            strInputPtr++;
                            loop++;
                            *(strFormatPtr++) = *(strInputPtr++);
                            strFormatLen++;
                            result = UC_CONV_FORMAT_CHAR_CHANGED;
                        }
                        else if(aChar == MMIUC_CHAR_SEMICOLON)
                        {
                            *(strFormatPtr++) = *(strInputPtr++);
                            strFormatLen++;
                            strInputPtr++;
                            loop++;
                            result = UC_CONV_FORMAT_CHAR_CHANGED;
                        }
                        else
                        {
                            *(strFormatPtr++) = *(strInputPtr++);
                            strFormatLen++;
                        }
                    }
                }
                else
                {
                    *(strFormatPtr++) = *(strInputPtr++);
                    strFormatLen++;
                }
                break;
            case MMIUC_CHAR_DOT:
                if(dotCnt < 1)
                {
                    if(0 == loop)
                    {
                        *(strFormatPtr++) = MMIUC_CHAR_ZERO;
                        strFormatLen ++;
                        result = UC_CONV_FORMAT_CHAR_CHANGED;
                    }
                    *(strFormatPtr++) = *(strInputPtr++);
                    strFormatLen++;
                    dotCnt++;
                }
                else
                {
                    strInputPtr++;
                    result = UC_CONV_FORMAT_CHAR_CHANGED;
                }
                break;
            case MMIUC_CHAR_SEMICOLON:
                if(semicolon_enable && semicolonCnt < 1)
                {
                    if(0 == loop)
                    {
                        *(strFormatPtr++) = MMIUC_CHAR_ONE;
                        strFormatLen ++;
                        result = UC_CONV_FORMAT_CHAR_CHANGED;
                    }
                    *(strFormatPtr++) = *(strInputPtr++);
                    strFormatLen++;
                    semicolonCnt++;
                    dotCnt = 0;
                }
                else
                {
                    strInputPtr++;
                    result = UC_CONV_FORMAT_CHAR_CHANGED;
                }
                break;
            case  MMIUC_CHAR_ONE:
            case  MMIUC_CHAR_TWO:
            case  MMIUC_CHAR_THREE:
            case  MMIUC_CHAR_FOUR:
            case  MMIUC_CHAR_FIVE:
            case  MMIUC_CHAR_SIX:
            case  MMIUC_CHAR_SEVEN:
            case  MMIUC_CHAR_EIGHT:
            case  MMIUC_CHAR_NINE:
                *(strFormatPtr++) = *(strInputPtr++);
                strFormatLen++;
                break;
            default:
                strInputPtr++;
                result = UC_CONV_FORMAT_CHAR_CHANGED;
                break;
        }
    }

    *strFormatPtr = 0;
    in_str_format_ptr->wstr_len = strFormatLen;
    if(strFormatLen > 0)
    {
        {
            MMIAPICOM_Wstrncpy(strCalculatePtr, in_str_format_ptr->wstr_ptr, strFormatLen);
            strCalculateLen = strFormatLen;
        }

        if(strCalculatePtr[strCalculateLen - 1] == MMIUC_CHAR_DOT
            || strCalculatePtr[strCalculateLen - 1] == MMIUC_CHAR_SEMICOLON
            || strCalculatePtr[strCalculateLen - 1] == MMIUC_CHAR_NEGATIVE
            || strCalculatePtr[strCalculateLen - 1] == MMIUC_CHAR_POSITIVE)
        {
            strCalculateLen--;
            strCalculatePtr[strCalculateLen] = 0;
        }
    }
    calculate_str_ptr->wstr_len = strCalculateLen;

    return result;
}

/**---------------------------------------------------------------------------*
 **                       FUNCTION BODY 
 **---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description :判断字符串是否合法
//  Global resource dependence : 
//  Author:renyi.hu
//  Note: 形式为整数或小数才合法
/*****************************************************************************/
LOCAL BOOLEAN InputIsValid(
							MMI_STRING_T const *in_str_ptr //in
							)
{
	int32 loop=0;
	int32 dot_count=0;

	if(PNULL==in_str_ptr)
	{
		return FALSE;
	}

	if(0==in_str_ptr->wstr_len)
	{
		return FALSE;
	}

	//第一个字符为'0'，但第2个字符不是'.'
	if(in_str_ptr->wstr_len>1)
	{
		if(MMIUC_CHAR_ZERO==in_str_ptr->wstr_ptr[0] && MMIUC_CHAR_DOT!=in_str_ptr->wstr_ptr[1])
		{
			return FALSE;
		}
	}

	//第一个字符为'.'，或者最后一个字符为'.'
	if(MMIUC_CHAR_DOT==in_str_ptr->wstr_ptr[0] || MMIUC_CHAR_DOT==in_str_ptr->wstr_ptr[in_str_ptr->wstr_len-1])
	{
		return FALSE;
	}

	for(loop=0;loop<in_str_ptr->wstr_len;loop++)
	{
		//不是'0'~'9'或者'.'
		if( (in_str_ptr->wstr_ptr[loop]<MMIUC_CHAR_ZERO || in_str_ptr->wstr_ptr[loop]>MMIUC_CHAR_NINE)
			&&MMIUC_CHAR_DOT!=in_str_ptr->wstr_ptr[loop])
		{
			return FALSE;
		}

		if(MMIUC_CHAR_DOT==in_str_ptr->wstr_ptr[loop])
		{
			dot_count++;
		}
	}

	if(dot_count>1)//小数点个数大于1
	{
		return FALSE;
	}
	
	return TRUE;
}
/*****************************************************************************/
// 	Description :判断字符串是否合法
//	Global resource dependence : 
//  Author:renyi.hu
//	Note: 形式为整数,分数或小数才合法
/*****************************************************************************/
LOCAL BOOLEAN InputIsValidCustomFactor(
							MMI_STRING_T const *in_str_ptr //in
							)
{
	int32 loop=0;
	int32 dot_count=0;
    int32 slash_count=0;

	if(PNULL==in_str_ptr)
	{
		return FALSE;
	}

	if(0==in_str_ptr->wstr_len)
	{
		return FALSE;
	}

    if(1 == in_str_ptr->wstr_len
        && MMIUC_CHAR_ZERO==in_str_ptr->wstr_ptr[0])
    {
        return FALSE;
    }
	//第一个字符为'0'，但第2个字符不是'.'
	if(in_str_ptr->wstr_len>1)
	{
		if(MMIUC_CHAR_ZERO==in_str_ptr->wstr_ptr[0] && MMIUC_CHAR_DOT!=in_str_ptr->wstr_ptr[1])
		{
			return FALSE;
		}
	}

	//第一个字符为'.'，或者最后一个字符为'.'
	if(MMIUC_CHAR_DOT==in_str_ptr->wstr_ptr[0] || MMIUC_CHAR_DOT==in_str_ptr->wstr_ptr[in_str_ptr->wstr_len-1])
	{
		return FALSE;
	}

	if(MMIUC_CHAR_SEMICOLON==in_str_ptr->wstr_ptr[0] || MMIUC_CHAR_SEMICOLON==in_str_ptr->wstr_ptr[in_str_ptr->wstr_len-1])
	{
		return FALSE;
	}

	for(loop=0;loop<in_str_ptr->wstr_len;loop++)
	{
		//不是'0'~'9'或者'.'
		if( (in_str_ptr->wstr_ptr[loop]<MMIUC_CHAR_ZERO || in_str_ptr->wstr_ptr[loop]>MMIUC_CHAR_NINE)
			&&MMIUC_CHAR_DOT!=in_str_ptr->wstr_ptr[loop]
			&&MMIUC_CHAR_SEMICOLON!=in_str_ptr->wstr_ptr[loop])
		{
			return FALSE;
		}

		if(MMIUC_CHAR_DOT==in_str_ptr->wstr_ptr[loop])
		{
			dot_count++;
		}
		if(MMIUC_CHAR_SEMICOLON==in_str_ptr->wstr_ptr[loop])
		{
			slash_count++;
            if(loop < in_str_ptr->wstr_len - 1)
            {
                if(MMIUC_CHAR_ZERO == in_str_ptr->wstr_ptr[loop + 1])
                {
                    return FALSE;
                }
            }
		}
	}

	if(dot_count > 1
        || slash_count > 1
        ||(slash_count > 0 && dot_count > 0))//小数点个数大于1
	{
		return FALSE;
	}
	
	return TRUE;
}

/*****************************************************************************/
// 	Description :判断字符串是否合法
//	Global resource dependence : 
//  Author:renyi.hu
//	Note: 形式为整数或小数才合法
/*****************************************************************************/
LOCAL BOOLEAN StrIsValid(
							MMI_STRING_T const *in_str_ptr //in
							)
{
	int32 loop=0;
	int32 dot_count=0;

	if(PNULL==in_str_ptr)
	{
		return TRUE;
	}

	if(0==in_str_ptr->wstr_len)
	{
		return TRUE;
	}

	//第一个字符为'0'，但第2个字符不是'.'
	if(in_str_ptr->wstr_len>1)
	{
		if(MMIUC_CHAR_ZERO==in_str_ptr->wstr_ptr[0] && MMIUC_CHAR_DOT!=in_str_ptr->wstr_ptr[1])
		{
			return FALSE;
		}
	}

	//第一个字符为'.'
	if(MMIUC_CHAR_DOT==in_str_ptr->wstr_ptr[0])
	{
		return FALSE;
	}

	for(loop=0;loop<in_str_ptr->wstr_len;loop++)
	{
		//不是'0'~'9'或者'.'
		if( (in_str_ptr->wstr_ptr[loop]<MMIUC_CHAR_ZERO || in_str_ptr->wstr_ptr[loop]>MMIUC_CHAR_NINE)
			&&MMIUC_CHAR_DOT!=in_str_ptr->wstr_ptr[loop])
		{
			return FALSE;
		}

		if(MMIUC_CHAR_DOT==in_str_ptr->wstr_ptr[loop])
		{
			dot_count++;
		}
	}

	if(dot_count>1)//小数点个数大于1
	{
		return FALSE;
	}
	
	return TRUE;
}

/*****************************************************************************/
// 	Description :判断字符串是否合法
//	Global resource dependence : 
//  Author:renyi.hu
//	Note: 形式为整数,分数或小数才合法
/*****************************************************************************/
LOCAL BOOLEAN StrIsValidCustomFactor(
							MMI_STRING_T const *in_str_ptr //in
							)
{
	int32 loop=0;
	int32 dot_count=0;
    int32 slash_count = 0;

	if(PNULL==in_str_ptr)
	{
		return TRUE;
	}

	if(0==in_str_ptr->wstr_len)
	{
		return TRUE;
	}

	//第一个字符为'0'，但第2个字符不是'.'
	if(in_str_ptr->wstr_len>1)
	{
		if(MMIUC_CHAR_ZERO==in_str_ptr->wstr_ptr[0] && MMIUC_CHAR_DOT!=in_str_ptr->wstr_ptr[1])
		{
			return FALSE;
		}
	}

	//第一个字符为'.'
	if(MMIUC_CHAR_DOT==in_str_ptr->wstr_ptr[0] || MMIUC_CHAR_SEMICOLON == in_str_ptr->wstr_ptr[0])
	{
		return FALSE;
	}

	for(loop=0;loop<in_str_ptr->wstr_len;loop++)
	{
		//不是'0'~'9'或者'.'或者'/'
		if( (in_str_ptr->wstr_ptr[loop]<MMIUC_CHAR_ZERO || in_str_ptr->wstr_ptr[loop]>MMIUC_CHAR_NINE)
			&&MMIUC_CHAR_DOT!=in_str_ptr->wstr_ptr[loop]
			&& MMIUC_CHAR_SEMICOLON != in_str_ptr->wstr_ptr[loop])
		{
			return FALSE;
		}

		if(MMIUC_CHAR_DOT==in_str_ptr->wstr_ptr[loop])
		{
			dot_count++;
		}

        if(MMIUC_CHAR_SEMICOLON == in_str_ptr->wstr_ptr[loop])
        {
            slash_count++;

            if(loop < in_str_ptr->wstr_len - 1)
            {
                if(MMIUC_CHAR_ZERO == in_str_ptr->wstr_ptr[loop + 1])
                {
                    return FALSE;
                }
            }
        }
	}

	if(dot_count>1
        || slash_count > 1
        || (dot_count != 0 && slash_count != 0))//小数点个数大于1
	{
		return FALSE;
	}
	
	return TRUE;
}

/*****************************************************************************/
// 	Description : 将输入的字符串经过单位换算后以字符串输出
//	Global resource dependence : 
//  Author:renyi.hu
//	Note: 
/*****************************************************************************/
LOCAL BOOLEAN  UnitConversion (
							UC_CONV_MAIN_TYPE_E  mainType,//in
							UC_CONV_SUBSET_TYPE  subType,//in
							UC_CONV_DIRECTION_E  direction,//in
							const MMI_STRING_T  *in_str_ptr, //in 
							MMI_STRING_T  *out_str_ptr//out
							)
{
    BOOLEAN result = FALSE;
    char  s_default_constant = '0';


    SCI_ASSERT(in_str_ptr!=PNULL&&out_str_ptr!=PNULL);/*assert verified*/
    SCI_ASSERT(out_str_ptr->wstr_ptr!=PNULL);/*assert verified*/

    if(mainType >= UC_CONV_MAIN_TYPE_NUM
        || direction >= UC_CONV_DIRECTION_NUM)
    {
        return FALSE;
    }
    
    switch(mainType)
    {
        case UC_CONV_MAIN_TEMPERATURE:
            result = UnitConversionByRate(in_str_ptr,s_tempure_conversion_rate[subType][0],s_tempure_conversion_constant[subType][direction],direction,out_str_ptr);  
            break;

        case UC_CONV_MAIN_WEIGHT:
            result = UnitConversionByRate(in_str_ptr,s_weight_conversion_rate[subType][0],&s_default_constant,direction,out_str_ptr);  
            break;

        case UC_CONV_MAIN_LENGTH:
            result = UnitConversionByRate(in_str_ptr,s_length_conversion_rate[subType][0],&s_default_constant,direction,out_str_ptr);  
            break;

        case UC_CONV_MAIN_AREA:
            result = UnitConversionByRate(in_str_ptr,s_area_conversion_rate[subType][0],&s_default_constant,direction,out_str_ptr);  
            break;

        case UC_CONV_MAIN_VOLUME:
            result = UnitConversionByRate(in_str_ptr,s_volume_conversion_rate[subType][0],&s_default_constant,direction,out_str_ptr);  
            break;

        case UC_CONV_MAIN_CURRENCY:
            {
                UC_CONV_DIRECTION_E dir = UC_CONV_DIRECTION_NUM;
                MMI_STRING_T        curExchane = {0};
                char                exchane[MMIUC_INPUT_MAX_LEN + 1]={0};

                GUIEDIT_GetString(MMIUNITCONV_CONVERTER_WIN_EDIT_3_CTRL_ID, &curExchane);
                if(!InputIsValidCustomFactor(&curExchane))
                {
                    result = TRUE;
                }
                else
                {
                    if(UC_CONV_DIRECTION_POSITIVE == direction)
                    {
                        dir = UC_CONV_DIRECTION_NEGATIVE;
                    }
                    else if(UC_CONV_DIRECTION_NEGATIVE == direction)
                    {
                        dir = UC_CONV_DIRECTION_POSITIVE;
                    }
                    MMIAPICOM_WstrToStr(curExchane.wstr_ptr, (uint8*)&exchane);
                    result = UnitConversionByRate(in_str_ptr,exchane,&s_default_constant,dir,out_str_ptr);  
                }
            }
            break;

        default:
            if(mainType >= UC_CONV_MAIN_CUSTOMIZE_1 
                && mainType < UC_CONV_MAIN_TYPE_NUM)
            {
                MMI_STRING_T curFactor = {0};
                char factor[UC_CONV_CUSTOMIZE_FACTOR_LEN + 1]={0};

                curFactor.wstr_ptr = custConvItem.convItem[mainType - UC_CONV_MAIN_CUSTOMIZE_1].factor;
                curFactor.wstr_len = MMIAPICOM_Wstrlen(custConvItem.convItem[mainType - UC_CONV_MAIN_CUSTOMIZE_1].factor);
                if(InputIsValidCustomFactor(&curFactor))
                {
                    MMIAPICOM_WstrToStr(curFactor.wstr_ptr, (uint8*)&factor);
                    result = UnitConversionByRate(in_str_ptr,factor,&s_default_constant,direction,out_str_ptr);  
                }
                else
                {
                    result = TRUE;
                }
            }
            break;
    }

    return result;
}

/*****************************************************************************/
// 	Description : 根据换算率进行单位换算
//	Global resource dependence : 
//  Author:renyi.hu
//	Note: 
/*****************************************************************************/
LOCAL BOOLEAN  UnitConversionByRate(
									 const MMI_STRING_T  *in_str, //in 
									 const char *conv_rate, // in 
									 const char *conv_constant, // in 
                                     UC_CONV_DIRECTION_E  direction,//in
									 MMI_STRING_T  *out_str //out
									 )
{
    double      dResult = 0;
    double      dOpRate = 0;
    double      dOpRateNumerator = 0;
    double      dOpRateDenominator = 0;
    double      dOpInput = 0;
    double      dOpConstant = 0;
    char*       strStop = PNULL;
    char*       slashPtr = PNULL;
    char        in_buf[MMIUC_INPUT_MAX_LEN + 1]={0};
    char        out_buf[MMIUC_OUTPUT_MAX_LEN + 1]={0};
    char        buf[MMIUC_OUTPUT_MAX_LEN*2 + 1]={0};
    char        slash[2] = {'/','\0'};
    char        factor[UC_CONV_CUSTOMIZE_FACTOR_LEN + 1]={0};

    SCI_ASSERT(in_str!=PNULL && conv_rate!=PNULL && out_str!=PNULL);/*assert verified*/
    SCI_ASSERT(out_str->wstr_ptr!=PNULL);/*assert verified*/
    SCI_ASSERT(SCI_STRLEN(conv_rate) <= UC_CONV_CUSTOMIZE_FACTOR_LEN);

    SCI_MEMSET(factor, 0x0, sizeof(factor));
    SCI_STRCPY(factor, conv_rate);
    slashPtr = MMIAPICOM_Stristr((char*)factor, (char*)slash);
    if(slashPtr)
    {
        *slashPtr = '\0';
        dOpRateNumerator = strtod((char*)factor, (char**)&strStop);
        dOpRateDenominator = strtod((char*)(slashPtr + 1), (char**)&strStop);
    }
    else
    {
        dOpRate = strtod((char*)factor, (char**)&strStop);
    }

    MMIAPICOM_WstrToStr(in_str->wstr_ptr, (uint8*)&in_buf);
    dOpInput = strtod((char*)in_buf, (char**)&strStop);
    dOpConstant = strtod((char*)conv_constant, (char**)&strStop);
    if(dOpRate<1.0e-100&&dOpRate>-1.0e-100)
    {
        dOpRate = 0;
    }
    if(dOpRateNumerator<1.0e-100&&dOpRateNumerator>-1.0e-100)
    {
        dOpRateNumerator = 0;
    }
    if(dOpRateDenominator<1.0e-100&&dOpRateDenominator>-1.0e-100)
    {
        dOpRateDenominator = 0;
    }
    if(dOpInput<1.0e-100 && dOpInput>-1.0e-100)
    {
        dOpInput = 0;
    }
    if(dOpConstant<1.0e-100 && dOpConstant>-1.0e-100)
    {
        dOpConstant = 0;
    }

    if(slashPtr)
    {
        if(direction == UC_CONV_DIRECTION_POSITIVE)
        {
            if(dOpRateDenominator != 0)
            {
                dResult = (dOpInput*dOpRateNumerator/dOpRateDenominator);
            }
            else
            {
                return FALSE;
            }
        }
        else
        {
            if(dOpRateNumerator != 0)
            {
                dResult = (dOpInput*dOpRateDenominator/dOpRateNumerator);
            }
            else
            {
                return FALSE;
            }
        }
    }
    else
    {
        if(direction == UC_CONV_DIRECTION_POSITIVE)
        {
            dResult = (dOpRate*dOpInput);
        }
        else
        {
            if(dOpRate != 0)
            {
                dResult = (dOpInput/dOpRate);
            }
            else
            {
                return FALSE;
            }
        }
    }
    dResult += dOpConstant;

    if(dResult > maxResultValue)
    {
        return FALSE;
    }

    if(dResult<1.0e-100 && dResult>-1.0e-100)//add for CR177013
    {
        dResult = 0;   
    }
    
    if (dResult > 9.9e100||dResult < -(9.9e100))/*错误处理*/
    {
        return FALSE;					
    }
    else
    {
        if( dResult <= -1e13 || dResult >= 1e13 )
        {    
            sprintf((char*)out_buf, "%e", dResult);      
        }
        else if( dResult <= 1e-11 && dResult >= -1e-11 )     //去0和小数点的位置
        {
            if(dResult<1.0e-100 && dResult>-1.0e-100)   //0
            {
                out_buf[0] = '0';
            }
            else
            {
                sprintf((char*)out_buf, "%e", dResult);  
            }
            
        }
        else
        {     
            SCI_MEMSET(buf, 0, sizeof(buf));       
            // 尽可能长的打印，去除小数点的位置
            sprintf ( (char*)buf, "%-.*f", (MMIUC_OUTPUT_MAX_LEN - 2), dResult);    
            if( buf[0] == '-')
            {
                SCI_MEMCPY((char*)out_buf, (char*)buf, MMIUC_OUTPUT_MAX_LEN + 1) ; 
            }
            else
            {
                SCI_MEMCPY((char*)out_buf, (char*)buf, MMIUC_OUTPUT_MAX_LEN) ;     
            }   
            //去除result尾部的0和‘.’
            {
                int32 i=0;
                int32 j=0;
                // 寻找‘.’
                for(i = MMIUC_OUTPUT_MAX_LEN; i >= 0 ; i--)
                {
                    if(out_buf[i] == '.')
                    {   
                        break;
                    }          
                }
                
                //找到‘.’,处理尾部的‘0’
                if(i != -1)
                {
                    for(j = MMIUC_OUTPUT_MAX_LEN; j > i ; j--)
                    {
                        if(out_buf[j] == '0'|| out_buf[j] == 0)
                        {                    
                            out_buf[j] = 0;                           
                        }       
                        else
                        {
                            break;
                        }
                    }
                }
                
                //判断是否处理‘.’
                if(i == j)
                {
                    out_buf[i] = 0;
                }
                
            }
        }   
    }

    MMIAPICOM_StrToWstr((const uint8*)out_buf, out_str->wstr_ptr);
    out_str->wstr_len=MMIAPICOM_Wstrlen(out_str->wstr_ptr);

    return TRUE;
}

LOCAL void customItemMakeOrder(void)
{
    int i = 0;
    int j = 1;

    for(; i < UC_CONV_CUSTOMIZE_MAX_NUM; i++)
    {
        if(custConvItem.convItem[i].convEnable == FALSE)
        {
            if(j <= i)
            {
                j = i + 1;
            }
            for(; j < UC_CONV_CUSTOMIZE_MAX_NUM; j++)
            {
                if(custConvItem.convItem[j].convEnable == TRUE)
                {
                    SCI_MEMCPY(&custConvItem.convItem[i], &custConvItem.convItem[j], sizeof(UC_CONV_CUSTOMIZE_ITEM_T));
                    SCI_MEMSET(&custConvItem.convItem[j], 0, sizeof(UC_CONV_CUSTOMIZE_ITEM_T));
                    break;
                }
            }
        }
        if(j == UC_CONV_CUSTOMIZE_MAX_NUM)
        {
            break;
        }
    }
}

#define UC_CONV_MAIN_CUSTOMIZE_DEL (UC_CONV_MAIN_CUSTOMIZE_1 << 16 | UC_CONV_SUBSET_CUSTOMIZE_DELETE)

LOCAL MMI_RESULT_E HandleUnitConvertMainTypeWinMsg(
    MMI_WIN_ID_T         win_id,
    MMI_MESSAGE_ID_E     msg_id,
    DPARAM               param
)
{
    MMI_RESULT_E     result = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T    mainMenuCtrId = MMIUNITCONV_MAIN_WIN_MENU_LIST_CTRL_ID;
    
    switch (msg_id)
    {
        case MSG_OPEN_WINDOW:
            UnitConv_SetMainTypeList(FALSE);
            MMK_SetAtvCtrl(win_id, mainMenuCtrId);
            
            break;   
        case MSG_NOTIFY_FORM_SWITCH_ACTIVE:

            break;
        case MSG_NOTIFY_EDITBOX_UPDATE_STRNUM:

            break;
        case MSG_FULL_PAINT:
            //UnitConv_SetMainTypeList(TRUE);
            if(MMIAPICOM_StatusAreaSetViewHeaderinfoByTextId(STR_CNV_HDR_CONVERTER))
            {
                GUIWIN_UpdateStb();
            }
            break;
        case MSG_APP_OK:
        case MSG_CTL_OK:
            {
               MMK_CreateWin((uint32*)MMIUNITCONV_CONVERTER_LIST_OPT_WIN_TAB, PNULL);
            }
            break;
#if 0
            if(GUILIST_GetItemState((GUILIST_ITEM_T*) GUILIST_GetItemPtrByIndex (mainMenuCtrId, GUILIST_GetCurItemIndex(mainMenuCtrId)), GUIITEM_STATE_LEFT_FOCUS))
            {
                break;
            }
            if(custConvItem.convNumber >= UC_CONV_CUSTOMIZE_MAX_NUM)
            {
                ConvDisplaybannerInfo(STR_COV_MAX_NUM);
                break;
            }
            UnitConv_converterWin(UC_CONV_CUSTOMIZE_MAX_NUM, (UC_CONV_SUBSET_TYPE)UC_CONV_SUBSET_CUSTOMIZE_NEW);
#endif
            break;
        case MSG_CTL_MIDSK:
        case MSG_APP_WEB:
        {
            MMI_MENU_ID_T menu_id = 0;

            menu_id = GUILIST_GetCurItemIndex(mainMenuCtrId);
            switch(menu_id)
            {
                case UC_CONV_MAIN_TEMPERATURE:
                    UnitConv_OpenSubsetTypeListWin(UC_CONV_MAIN_TEMPERATURE);
                    break;
                case UC_CONV_MAIN_WEIGHT:
                    UnitConv_OpenSubsetTypeListWin(UC_CONV_MAIN_WEIGHT);
                    break;
                case UC_CONV_MAIN_LENGTH:
                    UnitConv_OpenSubsetTypeListWin(UC_CONV_MAIN_LENGTH);
                    break;
                case UC_CONV_MAIN_AREA:
                    UnitConv_OpenSubsetTypeListWin(UC_CONV_MAIN_AREA);
                    break;
                case UC_CONV_MAIN_VOLUME:
                    UnitConv_OpenSubsetTypeListWin(UC_CONV_MAIN_VOLUME);
                    break;
                case UC_CONV_MAIN_CURRENCY:
                    UnitConv_converterWin(UC_CONV_MAIN_CURRENCY, (UC_CONV_SUBSET_TYPE)0);
                    break;
                default:
                    if(menu_id >= UC_CONV_MAIN_CUSTOMIZE_1
                        && menu_id < UC_CONV_MAIN_TYPE_NUM)
                    {
                        if(GUILIST_GetItemState((GUILIST_ITEM_T*)GUILIST_GetItemPtrByIndex(mainMenuCtrId, menu_id), GUIITEM_STATE_LEFT_FOCUS))
                        {
                            UC_CONV_CUSTOMIZE_ITEM_T*   custItem = PNULL;

                            custItem = &custConvItem.convItem[menu_id - UC_CONV_MAIN_CUSTOMIZE_1];
                            SCI_MEMSET(custItem, 0, sizeof(UC_CONV_CUSTOMIZE_ITEM_T));
                            custItem->convEnable = FALSE;
                            custConvItem.convNumber--;

                            customItemMakeOrder();
                            MMI_WriteNVItem(MMINV_ACC_UNITCONV_CUSTOMIZE_ITEM, (void *)&custConvItem);
                            UnitConv_SetMainTypeList(TRUE);
                            MMK_SendMsg(win_id, MSG_FULL_PAINT,0);
                        }
                        else
                        {
                            UnitConv_converterWin(menu_id, (UC_CONV_SUBSET_TYPE)UC_CONV_SUBSET_CUSTOMIZE_CONVERT);
                        }
                    }
                    break;
            }
        }
            break;
        case MSG_CTL_CANCEL:
        case MSG_APP_CANCEL:
            if(GUILIST_GetItemState((GUILIST_ITEM_T*) GUILIST_GetItemPtrByIndex (mainMenuCtrId, GUILIST_GetCurItemIndex(mainMenuCtrId)), GUIITEM_STATE_LEFT_FOCUS))
            {
                MMK_SendMsg(win_id, MSG_APP_RIGHT, PNULL);
            }
            else
            {
                MMK_CloseWin(win_id);
            }
            break;
        case MSG_CLOSE_WINDOW:
            break;
        default:
            result = MMI_RESULT_FALSE;
            break;
    }

    return (result);
}


LOCAL void UnitConv_SetMainTypeList(BOOLEAN beUpdate)
{
    MMI_CTRL_ID_T               ctrl_id = MMIUNITCONV_MAIN_WIN_MENU_LIST_CTRL_ID;
    GUILIST_ITEM_T              item_t = {0};
    GUILIST_ITEM_DATA_T         item_data = {0};
    uint32                      i = 0;

    item_t.item_data_ptr = &item_data;
    item_data.softkey_id[LEFT_BUTTON] = STXT_OPTION;
    item_data.softkey_id[MIDDLE_BUTTON] = STXT_SOFTKEY_OK_MK;
    item_data.softkey_id[RIGHT_BUTTON] = TXT_COMMON_COMMON_BACK;

    CTRLLIST_SetSoftkeyType(ctrl_id, GUILIST_SK_TYPE_TEXT_ID, GUILIST_SK_TYPE_TEXT_ID, GUILIST_SK_TYPE_TEXT_ID);
    GUILIST_SetMaxItem(ctrl_id, UC_CONV_MAIN_TYPE_NUM,FALSE);
    GUILIST_SetListState(ctrl_id, GUILIST_STATE_SPLIT_LINE, FALSE);
    GUILIST_SetShortcutIndicatorIcon(ctrl_id, IMAGE_LISTITEM_SHORTCUT_42_LEFT_RED,IMAGE_NULL);
    GUILIST_SetQuickActionIconMarginAndWidth(ctrl_id, 8, 26);
    //if(!beUpdate)
    {
        item_t.item_style    = GUIITEM_STYLE_ONE_LINE_TEXT_MS;
        item_data.item_content[0].item_data_type     = GUIITEM_DATA_TEXT_ID;            

        //Temperature
        item_data.item_content[0].item_data.text_id = STR_CNV_TEMP;
        GUILIST_InsertItem(ctrl_id, &item_t, UC_CONV_MAIN_TEMPERATURE);

        //Weight
        item_data.item_content[0].item_data.text_id = STR_CNV_WEIGHT;
        GUILIST_InsertItem(ctrl_id, &item_t, UC_CONV_MAIN_WEIGHT);

        //Lenght
        item_data.item_content[0].item_data.text_id = STR_CNV_LENGTH;
        GUILIST_InsertItem(ctrl_id, &item_t, UC_CONV_MAIN_LENGTH);

        //Aera
        item_data.item_content[0].item_data.text_id = STR_CNV_AREA;
        GUILIST_InsertItem(ctrl_id, &item_t, UC_CONV_MAIN_AREA);

        //Weight
        item_data.item_content[0].item_data.text_id = STR_CNV_VOLUME;
        GUILIST_InsertItem(ctrl_id, &item_t, UC_CONV_MAIN_VOLUME);

        //Currency
        item_data.item_content[0].item_data.text_id = STR_CNV_LIST_CURRENCY;
        GUILIST_InsertItem(ctrl_id, &item_t, UC_CONV_MAIN_CURRENCY);
    }

    SCI_MEMSET(&item_data, 0, sizeof(GUILIST_ITEM_DATA_T));       
    item_t.item_style = GUIITEM_STYLE_ONE_LINE_TEXT_MS;
    item_t.left_item_style = GUIITEM_STYLE_ONE_LINE_TEXT_WITH_LEFT_FOCUS_MS;
    
    item_data.item_content[0].item_data_type = GUIITEM_DATA_TEXT_BUFFER;            
    item_data.softkey_id[LEFT_BUTTON] = STXT_OPTION;
    item_data.softkey_id[MIDDLE_BUTTON] = STXT_SOFTKEY_OK_MK;
    item_data.softkey_id[RIGHT_BUTTON] = TXT_COMMON_COMMON_BACK;
    item_data.lfocus_softkey_id[LEFT_BUTTON] = STXT_OPTION;
    item_data.lfocus_softkey_id[MIDDLE_BUTTON] = STXT_SOFTKEY_DELETE_MK;
    item_data.lfocus_softkey_id[RIGHT_BUTTON] = TXT_COMMON_COMMON_BACK;

    MMI_ReadNVItem(MMINV_ACC_UNITCONV_CUSTOMIZE_ITEM, (void *)&custConvItem);
    for(i = 0; i < UC_CONV_CUSTOMIZE_MAX_NUM; i++)
    {
        if(custConvItem.convItem[i].convEnable)
        {
            uint16 itemIndex = UC_CONV_MAIN_CUSTOMIZE_1 + i;

            item_data.item_content[0].item_data.text_buffer.wstr_ptr = custConvItem.convItem[i].title;
            item_data.item_content[0].item_data.text_buffer.wstr_len = MMIAPICOM_Wstrlen(custConvItem.convItem[i].title);
            if(CTRLLIST_GetItem(ctrl_id, itemIndex))
            {
                GUILIST_ReplaceItem(ctrl_id, &item_t, itemIndex);
            }
            else
            {
                GUILIST_InsertItem(ctrl_id, &item_t, itemIndex);
            }
            //GUILIST_SetItemShowLRTriangleIconByIndex(ctrl_id, itemIndex, TRUE, FALSE);
            GUILIST_SetItemHandleLeftRightByIndex(ctrl_id, itemIndex, TRUE, FALSE);
            CTRLLIST_SetItemQuickIconByIndex(ctrl_id, itemIndex, IMAGE_COMMON_LIST_DISMISS, IMAGE_NULL);

        }
    }
}

LOCAL void UnitConv_OpenSubsetTypeListWin(UC_CONV_MAIN_TYPE_E mainType)
{
    MMK_CreateWin((uint32*)UNITCONV_SUBSET_WIN_TAB, (ADD_DATA)mainType);
}

LOCAL MMI_RESULT_E HandleUnitConvertSubsetTypeWinMsg(
    MMI_WIN_ID_T         win_id,
    MMI_MESSAGE_ID_E     msg_id,
    DPARAM               param
)
{
    MMI_RESULT_E            result = MMI_RESULT_TRUE;
    UC_CONV_MAIN_TYPE_E     mainType = (UC_CONV_MAIN_TYPE_E)MMK_GetWinAddDataPtr(win_id);
    
    switch (msg_id)
    {
        case MSG_OPEN_WINDOW:
            UnitConv_SetSubsetTypeList(mainType, FALSE);
            MMK_SetAtvCtrl(win_id, MMIUNITCONV_SUBSET_WIN_MENU_LIST_CTRL_ID);
            
            break;   
        case MSG_LOSE_FOCUS:
            break;
        case MSG_GET_FOCUS:
            break;
        case MSG_NOTIFY_FORM_SWITCH_ACTIVE:

            break;
        case MSG_NOTIFY_EDITBOX_UPDATE_STRNUM:

            break;
        case MSG_FULL_PAINT:
            switch(mainType)
            {
                case UC_CONV_MAIN_TEMPERATURE:
                    MMIAPICOM_StatusAreaSetViewHeaderinfoByTextId(STR_CNV_HDR_TEMP);
                    break;
                case UC_CONV_MAIN_WEIGHT:
                    MMIAPICOM_StatusAreaSetViewHeaderinfoByTextId(STR_CNV_HDR_WEIGHT);
                    break;
                case UC_CONV_MAIN_LENGTH:
                    MMIAPICOM_StatusAreaSetViewHeaderinfoByTextId(STR_CNV_HDR_LENGTH);
                    break;
                case UC_CONV_MAIN_AREA:
                    MMIAPICOM_StatusAreaSetViewHeaderinfoByTextId(STR_CNV_HDR_AREA);
                    break;
                case UC_CONV_MAIN_VOLUME:
                    MMIAPICOM_StatusAreaSetViewHeaderinfoByTextId(STR_CNV_HDR_VOLUME);
                    break;
                case UC_CONV_MAIN_CURRENCY:
                    MMIAPICOM_StatusAreaSetViewHeaderinfoByTextId(STR_CNV_HDR_CURRENCY);
                    break;
                default:
                    break;
            }
            GUIWIN_UpdateStb();
            break;
//        case MSG_APP_OK:
        case MSG_CTL_MIDSK:
//        case MSG_CTL_OK:
        case MSG_APP_WEB:
        {
            MMI_MENU_ID_T menu_id = 0;
            MMI_MENU_GROUP_ID_T group_id = 0;

            menu_id = GUILIST_GetCurItemIndex(MMIUNITCONV_SUBSET_WIN_MENU_LIST_CTRL_ID);
            UnitConv_converterWin(mainType, (UC_CONV_SUBSET_TYPE)menu_id);
        }
            break;
        case MSG_CTL_CANCEL:
        case MSG_APP_CANCEL:
            MMK_CloseWin(win_id);
            break;
        case MSG_CLOSE_WINDOW:
            break;
        default:
            result = MMI_RESULT_FALSE;
            break;
    }

    return (result);
}

LOCAL void UnitConv_SetSubsetTypeList(UC_CONV_MAIN_TYPE_E mainType, BOOLEAN beUpdate)
{
    MMI_CTRL_ID_T               ctrl_id = MMIUNITCONV_SUBSET_WIN_MENU_LIST_CTRL_ID;
    GUILIST_ITEM_T              item_t = {0};
    GUILIST_ITEM_DATA_T         item_data = {0};

    item_t.item_data_ptr = &item_data;
    GUILIST_SetListState(ctrl_id, GUILIST_STATE_SPLIT_LINE, FALSE);

    switch(mainType)
    {
        case UC_CONV_MAIN_TEMPERATURE:
            GUILIST_SetMaxItem(ctrl_id, UC_CONV_SUBSET_TEMPURE_TYPE_NUM,FALSE);
            //if(!beUpdate)
            {
                item_t.item_style    = GUIITEM_STYLE_ONE_LINE_TEXT_MS;
                item_data.item_content[0].item_data_type     = GUIITEM_DATA_TEXT_ID;            

                //Fahrenheit-Celsius
                item_data.item_content[0].item_data.text_id = STR_CNV_FAHR_CEL;
                GUILIST_InsertItem(ctrl_id, &item_t, UC_CONV_SUBSET_TEMPURE_F_C);

                //Kelvin-Fahrenheit
                item_data.item_content[0].item_data.text_id = STR_CNV_KEL_FAHR;
                GUILIST_InsertItem(ctrl_id, &item_t, UC_CONV_SUBSET_TEMPURE_F_K);

                //Kelvin-Celsius
                item_data.item_content[0].item_data.text_id = STR_CNV_KEL_CEL;
                GUILIST_InsertItem(ctrl_id, &item_t, UC_CONV_SUBSET_TEMPURE_C_K);
            }
            break;
        case UC_CONV_MAIN_WEIGHT:
            GUILIST_SetMaxItem(ctrl_id, UC_CONV_SUBSET_WEIGHT_TYPE_NUM,FALSE);
            //if(!beUpdate)
            {
                item_t.item_style    = GUIITEM_STYLE_ONE_LINE_TEXT_MS;
                item_data.item_content[0].item_data_type     = GUIITEM_DATA_TEXT_ID;            

                //Pound-kilo
                item_data.item_content[0].item_data.text_id = STR_CNV_POUND_KILO;
                GUILIST_InsertItem(ctrl_id, &item_t, UC_CONV_SUBSET_WEIGHT_POUND_KILO);

                //Carat-gram
                item_data.item_content[0].item_data.text_id = STR_CNV_CARAT_GRAM;
                GUILIST_InsertItem(ctrl_id, &item_t, UC_CONV_SUBSET_WEIGHT_CARAT_GRAM);

                //Ounce-gram
                item_data.item_content[0].item_data.text_id = STR_CNV_OUNCE_GRAM;
                GUILIST_InsertItem(ctrl_id, &item_t, UC_CONV_SUBSET_WEIGHT_OUNCE_GRAM);

                //Troy ounce-gram
                item_data.item_content[0].item_data.text_id = STR_CNV_TROY_GRAM;
                GUILIST_InsertItem(ctrl_id, &item_t, UC_CONV_SUBSET_WEIGHT_TROYOUNCE_GRAM);

                //Stone-kilo
                item_data.item_content[0].item_data.text_id = STR_CNV_STONE_KILO;
                GUILIST_InsertItem(ctrl_id, &item_t, UC_CONV_SUBSET_WEIGHT_STONE_KILO);
            }
            break;
        case UC_CONV_MAIN_LENGTH:
            GUILIST_SetMaxItem(ctrl_id, UC_CONV_SUBSET_LENGHT_TYPE_NUM,FALSE);
            //if(!beUpdate)
            {
                item_t.item_style    = GUIITEM_STYLE_ONE_LINE_TEXT_MS;
                item_data.item_content[0].item_data_type     = GUIITEM_DATA_TEXT_ID;            

                //Mile-km
                item_data.item_content[0].item_data.text_id = STR_CNV_MILE_KM;
                GUILIST_InsertItem(ctrl_id, &item_t, UC_CONV_SUBSET_LENGHT_MILE_KILOMETRE);

                //Yard-metre
                item_data.item_content[0].item_data.text_id = STR_CNV_YARD_MTR;
                GUILIST_InsertItem(ctrl_id, &item_t, UC_CONV_SUBSET_LENGHT_YARD_METRE);

                //Foot-metre
                item_data.item_content[0].item_data.text_id = STR_CNV_FOOT_MTR;
                GUILIST_InsertItem(ctrl_id, &item_t, UC_CONV_SUBSET_LENGHT_FOOT_METRE);

                //Foot-cm
                item_data.item_content[0].item_data.text_id = STR_CNV_FOOT_CM;
                GUILIST_InsertItem(ctrl_id, &item_t, UC_CONV_SUBSET_LENGHT_FOOT_CENTIMETRE);

                //Inch-cm
                item_data.item_content[0].item_data.text_id = STR_CNV_INCH_CM;
                GUILIST_InsertItem(ctrl_id, &item_t, UC_CONV_SUBSET_LENGHT_INCH_CENTIMETRE);

            }
            break;
        case UC_CONV_MAIN_AREA:
            GUILIST_SetMaxItem(ctrl_id, UC_CONV_SUBSET_AREA_TYPE_NUM,FALSE);
            //if(!beUpdate)
            {
                item_t.item_style    = GUIITEM_STYLE_ONE_LINE_TEXT_MS;
                item_data.item_content[0].item_data_type     = GUIITEM_DATA_TEXT_ID;            

                //Foot2-metre2
                item_data.item_content[0].item_data.text_id = STR_CNV_SQF_SQM;
                GUILIST_InsertItem(ctrl_id, &item_t, UC_CONV_SUBSET_AREA_FOOT_METRE);

                //Inch2-cm2
                item_data.item_content[0].item_data.text_id = STR_CNV_SQINCH_SQCM;
                GUILIST_InsertItem(ctrl_id, &item_t, UC_CONV_SUBSET_AREA_INCH_CENTIMETRE);

                //Yard2-metre2
                item_data.item_content[0].item_data.text_id = STR_CNV_SQYARD_SQMTR;
                GUILIST_InsertItem(ctrl_id, &item_t, UC_CONV_SUBSET_AREA_YARD_METRE);

                //Mile2-km2
                item_data.item_content[0].item_data.text_id = STR_CNV_SQMILE_SQKM;
                GUILIST_InsertItem(ctrl_id, &item_t, UC_CONV_SUBSET_AREA_MILE_KILOMETRE);

                //Acre-hectare
                item_data.item_content[0].item_data.text_id = STR_CNV_ACRE_HECTARE;
                GUILIST_InsertItem(ctrl_id, &item_t, UC_CONV_SUBSET_AREA_ACRE_HECTARE);

            }
            break;
        case UC_CONV_MAIN_VOLUME:
            GUILIST_SetMaxItem(ctrl_id, UC_CONV_SUBSET_VOLUME_TYPE_NUM,FALSE);
            //if(!beUpdate)
            {
                item_t.item_style    = GUIITEM_STYLE_ONE_LINE_TEXT_MS;
                item_data.item_content[0].item_data_type     = GUIITEM_DATA_TEXT_ID;            

                //Inch3-cm3
                item_data.item_content[0].item_data.text_id = STR_CNV_CUINCH_CUCM;
                GUILIST_InsertItem(ctrl_id, &item_t, UC_CONV_SUBSET_VOLUME_INCH_CENTIMETRE);

                //Foot3-metre3
                item_data.item_content[0].item_data.text_id = STR_CNV_CUFEET_CUMTR;
                GUILIST_InsertItem(ctrl_id, &item_t, UC_CONV_SUBSET_VOLUME_FOOT_METRE);

                //Metre3-yard3
                item_data.item_content[0].item_data.text_id = STR_CNV_CUMTR_CUYARD;
                GUILIST_InsertItem(ctrl_id, &item_t, UC_CONV_SUBSET_VOLUME_YARD_METRE);

                //Gallon-litre
                item_data.item_content[0].item_data.text_id = STR_CNV_GLNUK_LTR;
                GUILIST_InsertItem(ctrl_id, &item_t, UC_CONV_SUBSET_VOLUME_GALLON_LITRE);

            }
            break;
        case UC_CONV_MAIN_CURRENCY:
            break;
        default:
            break;
    }
}

LOCAL void UnitConv_converterWin(UC_CONV_MAIN_TYPE_E mainType, UC_CONV_SUBSET_TYPE subType)
{
    MMK_CreateWin((uint32*)UNITCONV_CONVERTER_WIN_TAB, (ADD_DATA)(((uint32)mainType) << 16 | subType));
}

LOCAL MMI_RESULT_E HandleUnitConverterWinMsg(
    MMI_WIN_ID_T         win_id,
    MMI_MESSAGE_ID_E     msg_id,
    DPARAM               param
)
{
    MMI_RESULT_E            result = MMI_RESULT_TRUE;
    ADD_DATA                addData = MMK_GetWinAddDataPtr(win_id);
    UC_CONV_MAIN_TYPE_E     mainType = (UC_CONV_MAIN_TYPE_E)((uint32)addData >> 16);
    UC_CONV_SUBSET_TYPE     subType = (UC_CONV_SUBSET_TYPE)((uint32)addData & 0x00FF);
    UC_CONV_SUBSET_CUSTOMIZE_TYPE_E custConvType = (UC_CONV_SUBSET_CUSTOMIZE_TYPE_E)MMK_GetWinUserData(win_id);
    
    wchar                   dot_str[] = {'.'};
    static wchar            last_valid_input_str[MMIUC_INPUT_MAX_LEN + 1]; 
    static MMI_CTRL_ID_T    last_valid_input_ctrl = 0;

    switch (msg_id)
    {
        case MSG_OPEN_WINDOW:
            Init_ConverterWin(win_id, mainType, subType);
            MMK_SetAtvCtrl(win_id, MMIUNITCONV_CONVERTER_WIN_EDIT_1_CTRL_ID);
            break;   
        case MSG_FULL_PAINT:
            switch(mainType)
            {
                case UC_CONV_MAIN_TEMPERATURE:
                    MMIAPICOM_StatusAreaSetViewHeaderinfoByTextId(STR_CNV_HDR_TEMP);
                    break;
                case UC_CONV_MAIN_WEIGHT:
                    MMIAPICOM_StatusAreaSetViewHeaderinfoByTextId(STR_CNV_HDR_WEIGHT);
                    break;
                case UC_CONV_MAIN_LENGTH:
                    MMIAPICOM_StatusAreaSetViewHeaderinfoByTextId(STR_CNV_HDR_LENGTH);
                    break;
                case UC_CONV_MAIN_AREA:
                    MMIAPICOM_StatusAreaSetViewHeaderinfoByTextId(STR_CNV_HDR_AREA);
                    break;
                case UC_CONV_MAIN_VOLUME:
                    MMIAPICOM_StatusAreaSetViewHeaderinfoByTextId(STR_CNV_HDR_VOLUME);
                    break;
                case UC_CONV_MAIN_CURRENCY:
                    MMIAPICOM_StatusAreaSetViewHeaderinfoByTextId(STR_CNV_HDR_CURRENCY);
                    break;
                case UC_CONV_CUSTOMIZE_MAX_NUM:
                    if(subType == UC_CONV_SUBSET_CUSTOMIZE_NEW)
                    {
                        MMIAPICOM_StatusAreaSetViewHeaderinfoByTextId(STR_CNV_HDR_NEW_EXT01);
                    }
                    break;
                default:
                    if(mainType >= UC_CONV_MAIN_CUSTOMIZE_1 
                        && mainType < UC_CONV_MAIN_TYPE_NUM)
                    {
                        uint32 custIndex = mainType - UC_CONV_MAIN_CUSTOMIZE_1;

                        if(custConvType == UC_CONV_SUBSET_CUSTOMIZE_CONVERT)
                        {
                            MMIAPICOM_StatusAreaSetViewHeaderinfo(custConvItem.convItem[custIndex].title,
                                                MMIAPICOM_Wstrlen(custConvItem.convItem[custIndex].title));
                        }
                        else if(custConvType == UC_CONV_SUBSET_CUSTOMIZE_EDIT)
                        {
                            MMIAPICOM_StatusAreaSetViewHeaderinfoByTextId(STR_CNV_HDR_NEW_EXT01);
                        }              
                    }
                    break;
            }
            GUIWIN_UpdateStb();
            break;
        case MSG_LOSE_FOCUS:
            break;
        case MSG_GET_FOCUS:
            break;
        case MSG_NOTIFY_FORM_SWITCH_ACTIVE:

            break;
        case MSG_NOTIFY_EDITBOX_UPDATE_STRNUM:
        {
            MMI_STRING_T            in_str_ptr = {0};
            MMI_STRING_T            in_str_format_ptr = {0};
            MMI_STRING_T            calculate_str_ptr = {0};
            MMI_STRING_T            out_str_ptr = {0}; 
            UC_CONV_FORMAT_CHAR_E   charFormat = UC_CONV_FORMAT_CHAR_ORIGINAL;
            MMI_CTRL_ID_T           activeCtrlId = MMK_GetActiveCtrlId(win_id);
            BOOLEAN                 semicolon_enable = FALSE;
            BOOLEAN                 sign_enable = FALSE;
            wchar                   out_buffer[MMIUC_MAX_STR_BUFF_LEN + 1] = {0};  
            wchar                   inputFormatBuf[MMIUC_INPUT_MAX_LEN + 1] = {0}; 
            wchar                   inputCalculateBuf[MMIUC_INPUT_MAX_LEN + 1] = {0}; 
            uint16                  editInit = MMIUC_EDIT_CTRL_1_INIT;
//            MMI_CTRL_ID_T           tempCtrlId = MMIUNITCONV_CONVERTER_WIN_FORM_CTRL_ID;

            if(PNULL == param)
            {
                break;
            }

            editInit = MMIUC_EDIT_CTRL_1_INIT << ((((MMI_NOTIFY_T*)param)->src_id - MMIUNITCONV_CONVERTER_WIN_EDIT_1_CTRL_ID) * 4);
            if(editInit & editCtrlState)
            {
                editCtrlState &= ~editInit;
                break;
            }

            if(((MMI_NOTIFY_T*)param)->src_id != activeCtrlId)
            {
                break;
            }

            out_str_ptr.wstr_ptr = out_buffer;
            GUIEDIT_GetString(activeCtrlId, &in_str_ptr);

            SCI_MEMSET(inputFormatBuf, 0, sizeof(inputFormatBuf));
            in_str_format_ptr.wstr_ptr = inputFormatBuf;
            in_str_format_ptr.wstr_len = 0;
            SCI_MEMSET(inputCalculateBuf, 0, sizeof(inputCalculateBuf));
            calculate_str_ptr.wstr_ptr = inputCalculateBuf;
            calculate_str_ptr.wstr_len = 0;

            //do the convertion
            if(mainType < UC_CONV_MAIN_PREDEFINED_NUM 
                || (mainType >= UC_CONV_MAIN_PREDEFINED_NUM 
                        && subType == UC_CONV_SUBSET_CUSTOMIZE_CONVERT
                        && custConvType != UC_CONV_SUBSET_CUSTOMIZE_EDIT))
            {
                if(in_str_ptr.wstr_len > MMIUC_INPUT_MAX_LEN)
                {
                    SCI_MEMSET(last_valid_input_str, 0, (UC_CONV_CUSTOMIZE_FACTOR_LEN + 1)*sizeof(wchar));
                    MMIAPICOM_Wstrncpy(last_valid_input_str, in_str_ptr.wstr_ptr,MMIUC_INPUT_MAX_LEN);
                    GUIEDIT_SetString(activeCtrlId, last_valid_input_str, MMIUC_INPUT_MAX_LEN);
                    ConvDisplaybannerInfo(STR_COV_INPUT_LONG);
                    //MMK_SendMsg(win_id, MSG_FULL_PAINT,0);
                    break;
                }
                if(mainType == UC_CONV_MAIN_TEMPERATURE || mainType >= UC_CONV_MAIN_PREDEFINED_NUM)
                {
                    sign_enable = TRUE;
                }
                if(mainType == UC_CONV_MAIN_CURRENCY && activeCtrlId == MMIUNITCONV_CONVERTER_WIN_EDIT_3_CTRL_ID)
                {
                    semicolon_enable = TRUE;
                }
                charFormat = InputFormat(&in_str_ptr , &in_str_format_ptr, &calculate_str_ptr, semicolon_enable, sign_enable);

                if(UC_CONV_FORMAT_CHAR_CHANGED == charFormat)
                {
                    if(in_str_format_ptr.wstr_len > 0)
                    {
                        if(in_str_format_ptr.wstr_len > MMIUC_INPUT_MAX_LEN)
                        {
                            GUIEDIT_SetString(activeCtrlId, in_str_format_ptr.wstr_ptr, MMIUC_INPUT_MAX_LEN);
                            ConvDisplaybannerInfo(STR_COV_INPUT_LONG);
                        }
                        else
                        {
                            GUIEDIT_SetString(activeCtrlId, in_str_format_ptr.wstr_ptr, in_str_format_ptr.wstr_len);
                        }
                    }
                    else
                    {
                        GUIEDIT_ClearAllStr(activeCtrlId);
                    }
                }
                else
                {
                    if(MMIUNITCONV_CONVERTER_WIN_EDIT_1_CTRL_ID == activeCtrlId)
                    {
                        if(calculate_str_ptr.wstr_len > 0)
                        {
                            if(UnitConversion(mainType, subType, UC_CONV_DIRECTION_POSITIVE, &calculate_str_ptr, &out_str_ptr))
                            {
                                if(out_str_ptr.wstr_len == 0)
                                {
                                    GUIEDIT_ClearAllStr(MMIUNITCONV_CONVERTER_WIN_EDIT_2_CTRL_ID);
                                }
                                else
                                {
                                GUIEDIT_SetString(MMIUNITCONV_CONVERTER_WIN_EDIT_2_CTRL_ID, out_str_ptr.wstr_ptr, out_str_ptr.wstr_len);
                                }
                            }
                            else
                            {
                                GUIEDIT_SetString(MMIUNITCONV_CONVERTER_WIN_EDIT_1_CTRL_ID, calculate_str_ptr.wstr_ptr, calculate_str_ptr.wstr_len-1);
                                ConvDisplaybannerInfo(STR_COV_OUTPUT_LONG);
                            }
                        }
                        else
                        {
                            GUIEDIT_ClearAllStr(MMIUNITCONV_CONVERTER_WIN_EDIT_2_CTRL_ID);
                        }
                    }
                    else if(MMIUNITCONV_CONVERTER_WIN_EDIT_2_CTRL_ID == activeCtrlId)
                    {
                        if(calculate_str_ptr.wstr_len > 0)
                        {
                            if(UnitConversion(mainType, subType, UC_CONV_DIRECTION_NEGATIVE, &calculate_str_ptr, &out_str_ptr))
                            {
                                if(out_str_ptr.wstr_len == 0)
                                {
                                    GUIEDIT_ClearAllStr(MMIUNITCONV_CONVERTER_WIN_EDIT_1_CTRL_ID);
                                }
                                else
                                {
                                GUIEDIT_SetString(MMIUNITCONV_CONVERTER_WIN_EDIT_1_CTRL_ID, out_str_ptr.wstr_ptr, out_str_ptr.wstr_len);
                                }
                            }
                            else
                            {
                                GUIEDIT_SetString(MMIUNITCONV_CONVERTER_WIN_EDIT_2_CTRL_ID, calculate_str_ptr.wstr_ptr, calculate_str_ptr.wstr_len-1);
                                ConvDisplaybannerInfo(STR_COV_OUTPUT_LONG);
                            }
                        }
                        else
                        {
                            GUIEDIT_ClearAllStr(MMIUNITCONV_CONVERTER_WIN_EDIT_1_CTRL_ID);
                        }
                    }
                    else if(MMIUNITCONV_CONVERTER_WIN_EDIT_3_CTRL_ID == activeCtrlId)
                    {
                        MMI_STRING_T domestic = {0};

                        GUIEDIT_GetString(MMIUNITCONV_CONVERTER_WIN_EDIT_1_CTRL_ID,&domestic);
                        if(domestic.wstr_len > 0)
                        {
                            if(UnitConversion(mainType, subType, UC_CONV_DIRECTION_POSITIVE, &domestic, &out_str_ptr))
                            {
                                if(out_str_ptr.wstr_len > 0)
                                {
                                    GUIEDIT_SetString(MMIUNITCONV_CONVERTER_WIN_EDIT_2_CTRL_ID, out_str_ptr.wstr_ptr, out_str_ptr.wstr_len);
                                }
                                else
                                {
                                    GUIEDIT_ClearAllStr(MMIUNITCONV_CONVERTER_WIN_EDIT_2_CTRL_ID);
                                }
                            }
                            else
                            {
                                SCI_MEMSET(last_valid_input_str, 0, (UC_CONV_CUSTOMIZE_FACTOR_LEN + 1)*sizeof(wchar));
                                MMIAPICOM_Wstrncpy(last_valid_input_str, domestic.wstr_ptr,domestic.wstr_len);
                                GUIEDIT_SetString(MMIUNITCONV_CONVERTER_WIN_EDIT_1_CTRL_ID, last_valid_input_str, domestic.wstr_len - 1);
                                ConvDisplaybannerInfo(STR_COV_OUTPUT_LONG);
                            }
                        }
                    }
                    else
                    {
                    }
                }

            }
            //do the edition for customize
            else if(subType == UC_CONV_SUBSET_CUSTOMIZE_NEW
                        || custConvType == UC_CONV_SUBSET_CUSTOMIZE_EDIT)
            {
                if(MMIUNITCONV_CONVERTER_WIN_EDIT_4_CTRL_ID == activeCtrlId)
                {
                    charFormat = InputFormat(&in_str_ptr , &in_str_format_ptr, &calculate_str_ptr, TRUE, FALSE);
                    if(UC_CONV_FORMAT_CHAR_CHANGED == charFormat)
                    {
                        if(in_str_format_ptr.wstr_len > 0)
                        {
                            GUIEDIT_SetString(activeCtrlId, in_str_format_ptr.wstr_ptr, in_str_format_ptr.wstr_len);
                        }
                        else
                        {
                            GUIEDIT_ClearAllStr(activeCtrlId);
                        }
                    }
#if 0
                    else
                    {
                        if(0 != MMIAPICOM_Wstrcmp(custConvChangeItem.factor, in_str_ptr.wstr_ptr))
                        {
                            GUIWIN_SetSoftkeyBtnGray(win_id, MIDDLE_BUTTON, FALSE);
                        }
                        
                    }
#endif
                }
                else
                {
#if 0
                    if(MMIUNITCONV_CONVERTER_WIN_EDIT_1_CTRL_ID == activeCtrlId)
                    {
                        if(0 != MMIAPICOM_Wstrcmp(custConvChangeItem.title, in_str_ptr.wstr_ptr))
                        {
                            GUIWIN_SetSoftkeyBtnGray(win_id, MIDDLE_BUTTON, FALSE);
                        }
                    }
                    else if(MMIUNITCONV_CONVERTER_WIN_EDIT_2_CTRL_ID == activeCtrlId)
                    {
                        if(0 != MMIAPICOM_Wstrcmp(custConvChangeItem.inputName, in_str_ptr.wstr_ptr))
                        {
                            GUIWIN_SetSoftkeyBtnGray(win_id, MIDDLE_BUTTON, FALSE);
                        }
                    }
                    else if(MMIUNITCONV_CONVERTER_WIN_EDIT_3_CTRL_ID == activeCtrlId)
                    {
                        if(0 != MMIAPICOM_Wstrcmp(custConvChangeItem.outputName, in_str_ptr.wstr_ptr))
                        {
                            GUIWIN_SetSoftkeyBtnGray(win_id, MIDDLE_BUTTON, FALSE);
                        }
                    }
#endif
                    if(in_str_ptr.wstr_len <= MMIUC_INPUT_MAX_LEN)
                    {
                        SCI_MEMSET(last_valid_input_str, 0, (UC_CONV_CUSTOMIZE_FACTOR_LEN + 1)*sizeof(wchar));
                        GUIEDIT_GetString(activeCtrlId, &in_str_ptr);
                        if(in_str_ptr.wstr_ptr!=PNULL && in_str_ptr.wstr_len>0)
                        {
                            MMIAPICOM_Wstrncpy(last_valid_input_str, in_str_ptr.wstr_ptr,
                                in_str_ptr.wstr_len <= UC_CONV_CUSTOMIZE_FACTOR_LEN ? in_str_ptr.wstr_len : UC_CONV_CUSTOMIZE_FACTOR_LEN);
                        }
                    }
                    else
                    {
                        in_str_ptr.wstr_ptr = last_valid_input_str;
                        in_str_ptr.wstr_len = MMIAPICOM_Wstrlen(last_valid_input_str);

                        GUIEDIT_SetString( activeCtrlId, in_str_ptr.wstr_ptr, in_str_ptr.wstr_len);  
                    }
                }
                {
                    BOOLEAN bSave = TRUE;
                    MMI_STRING_T sInput = {0};

                    GUIEDIT_GetString(MMIUNITCONV_CONVERTER_WIN_EDIT_1_CTRL_ID, &sInput);
                    if(sInput.wstr_len == 0)
                    {
                        bSave = FALSE;
                    }
                    GUIEDIT_GetString(MMIUNITCONV_CONVERTER_WIN_EDIT_2_CTRL_ID, &sInput);
                    if(sInput.wstr_len == 0)
                    {
                        bSave = FALSE;
                    }
                    GUIEDIT_GetString(MMIUNITCONV_CONVERTER_WIN_EDIT_3_CTRL_ID, &sInput);
                    if(sInput.wstr_len == 0)
                    {
                        bSave = FALSE;
                    }
                    GUIEDIT_GetString(MMIUNITCONV_CONVERTER_WIN_EDIT_4_CTRL_ID, &sInput);
                    if(sInput.wstr_len == 0)
                    {
                        bSave = FALSE;
                    }
                    if(!InputIsValidCustomFactor(&sInput))
                    {
                        bSave = FALSE;
                    }
                    GUIWIN_SetSoftkeyBtnGray(win_id, MIDDLE_BUTTON, !bSave);
                    custConvCanbeSave = bSave;
                }
            }
        }

            MMK_SendMsg(win_id, MSG_FULL_PAINT,0);
            break;
        case MSG_APP_OK:
        case MSG_KEYUP_OK:
//        case MSG_CTL_OK:
        {
            MMI_STRING_T            in_str_ptr = {0};
            MMI_CTRL_ID_T           activeCtrlId = MMK_GetActiveCtrlId(win_id);
            GUIEDIT_GetString(activeCtrlId, &in_str_ptr);
            if(mainType < UC_CONV_MAIN_CURRENCY)
            {
                if(in_str_ptr.wstr_len > 0)
                {
                    MMK_CloseWin(win_id);
                }
            }
            else if(mainType == UC_CONV_MAIN_CURRENCY)
            {
            }
            else if(mainType >= UC_CONV_MAIN_CUSTOMIZE_1 && mainType < UC_CONV_MAIN_TYPE_NUM)
            {
                if(subType == UC_CONV_SUBSET_CUSTOMIZE_CONVERT
                    && custConvType != UC_CONV_SUBSET_CUSTOMIZE_EDIT)
                {
                    MMK_SetWinUserData(win_id, (void *)UC_CONV_SUBSET_CUSTOMIZE_EDIT);
                    Init_ConverterWin(win_id, mainType, UC_CONV_SUBSET_CUSTOMIZE_EDIT);
                    if(MMIUNITCONV_CONVERTER_WIN_EDIT_1_CTRL_ID == MMK_GetActiveCtrlId(win_id))
                    {
                        MMK_SetAtvCtrl(win_id, MMIUNITCONV_CONVERTER_WIN_EDIT_2_CTRL_ID);
                        MMK_SetAtvCtrl(win_id, MMIUNITCONV_CONVERTER_WIN_EDIT_1_CTRL_ID);
                    }
                    else
                    {
                        MMK_SetAtvCtrl(win_id, MMIUNITCONV_CONVERTER_WIN_EDIT_1_CTRL_ID);
                    }
                    MMK_SendMsg(win_id, MSG_FULL_PAINT,0);
                }
            }
        }
            break;
        case MSG_CTL_MIDSK:
        case MSG_APP_WEB:
            if(mainType >= UC_CONV_MAIN_CUSTOMIZE_1 && mainType < UC_CONV_MAIN_TYPE_NUM)
            {
                if(custConvCanbeSave && (subType == UC_CONV_SUBSET_CUSTOMIZE_NEW
                    || custConvType == UC_CONV_SUBSET_CUSTOMIZE_EDIT))
                {
                    UC_CONV_CUSTOMIZE_ITEM_T*   custItem = PNULL;
                    MMI_STRING_T                in_str_ptr = {0};
                    uint                        i = 0;
                    uint                        strLen = 0;

                    if(custConvItem.convNumber > UC_CONV_CUSTOMIZE_MAX_NUM)
                    {
                        break;
                    }
                    if(subType == UC_CONV_SUBSET_CUSTOMIZE_NEW)
                    {
                        custItem = &custConvItem.convItem[i];
                        while(custItem->convEnable)
                        {
                            custItem = &custConvItem.convItem[++i];
                        }
                    }
                    else if(custConvType == UC_CONV_SUBSET_CUSTOMIZE_EDIT)
                    {
                        custItem = &custConvItem.convItem[mainType - UC_CONV_MAIN_CUSTOMIZE_1];
                    }

                    //get the title
                    GUIEDIT_GetString(MMIUNITCONV_CONVERTER_WIN_EDIT_1_CTRL_ID, &in_str_ptr);
                    strLen = in_str_ptr.wstr_len <= UC_CONV_CUSTOMIZE_TITLE_LEN ? in_str_ptr.wstr_len : UC_CONV_CUSTOMIZE_TITLE_LEN;
                    MMIAPICOM_Wstrncpy(custItem->title, in_str_ptr.wstr_ptr, strLen);
                    custItem->title[strLen] = '\0';

                    //get the input
                    GUIEDIT_GetString(MMIUNITCONV_CONVERTER_WIN_EDIT_2_CTRL_ID, &in_str_ptr);
                    strLen = in_str_ptr.wstr_len <= UC_CONV_CUSTOMIZE_INPUT_NAME_LEN ? in_str_ptr.wstr_len : UC_CONV_CUSTOMIZE_INPUT_NAME_LEN;
                    MMIAPICOM_Wstrncpy(custItem->inputName, in_str_ptr.wstr_ptr, strLen);
                    custItem->inputName[strLen] = '\0';

                    //get the output
                    GUIEDIT_GetString(MMIUNITCONV_CONVERTER_WIN_EDIT_3_CTRL_ID, &in_str_ptr);
                    strLen = in_str_ptr.wstr_len <= UC_CONV_CUSTOMIZE_OUTPUT_NAME_LEN ? in_str_ptr.wstr_len : UC_CONV_CUSTOMIZE_OUTPUT_NAME_LEN;
                    MMIAPICOM_Wstrncpy(custItem->outputName, in_str_ptr.wstr_ptr, strLen);
                    custItem->outputName[strLen] = '\0';

                    //get the factor
                    GUIEDIT_GetString(MMIUNITCONV_CONVERTER_WIN_EDIT_4_CTRL_ID, &in_str_ptr);
                    strLen = in_str_ptr.wstr_len <= UC_CONV_CUSTOMIZE_FACTOR_LEN ? in_str_ptr.wstr_len : UC_CONV_CUSTOMIZE_FACTOR_LEN;
                    MMIAPICOM_Wstrncpy(custItem->factor, in_str_ptr.wstr_ptr, strLen);
                    custItem->factor[strLen] = '\0';

                    custItem->convEnable = TRUE;
                    if(subType == UC_CONV_SUBSET_CUSTOMIZE_NEW)
                    {
                        custConvItem.convNumber++;
                    }
                    MMI_WriteNVItem(MMINV_ACC_UNITCONV_CUSTOMIZE_ITEM, (void *)&custConvItem);
                    UnitConv_SetMainTypeList(TRUE);
                    if(subType == UC_CONV_SUBSET_CUSTOMIZE_NEW)
                    {
                        uint32 curSelected = 0;
                        curSelected = UC_CONV_MAIN_PREDEFINED_NUM + custConvItem.convNumber - 1;
                        GUILIST_SetSelectedItem(MMIUNITCONV_MAIN_WIN_MENU_LIST_CTRL_ID, curSelected, TRUE);
                        GUILIST_SetCurItemIndex(MMIUNITCONV_MAIN_WIN_MENU_LIST_CTRL_ID, curSelected);
                        MMK_CloseWin(win_id);
                    }
                    else if(custConvType == UC_CONV_SUBSET_CUSTOMIZE_EDIT)
                    {
                        MMK_SetWinUserData(win_id, (void *)UC_CONV_SUBSET_CUSTOMIZE_CONVERT);
                        Init_ConverterWin(win_id, mainType, UC_CONV_SUBSET_CUSTOMIZE_CONVERT);
                        MMK_SetAtvCtrl(win_id, MMIUNITCONV_CONVERTER_WIN_EDIT_1_CTRL_ID);
                        MMK_SendMsg(win_id, MSG_FULL_PAINT,0);
                    }
                    custConvCanbeSave = FALSE;
                }
            }
            break;
        case MSG_CTL_CANCEL:
        case MSG_APP_CANCEL:
            MMK_CloseWin(win_id);
            break;
        case MSG_CLOSE_WINDOW:
            break;
        default:
            result = MMI_RESULT_FALSE;
            break;
    }

    return (result);
}

#define MAKE_BREW_RGB(r,g,b) ((r >> 3)<<11 | (g>>2)<<5 | (b>>3))
LOCAL void Init_ConverterWin(MMI_WIN_ID_T win_id, UC_CONV_MAIN_TYPE_E mainType, UC_CONV_SUBSET_TYPE subType)
{
    MMI_CTRL_ID_T               form_id = MMIUNITCONV_CONVERTER_WIN_FORM_CTRL_ID;
    MMI_CTRL_ID_T               ctrl_id1 = MMIUNITCONV_CONVERTER_WIN_EDIT_1_CTRL_ID;
    MMI_CTRL_ID_T               ctrl_id2 = MMIUNITCONV_CONVERTER_WIN_EDIT_2_CTRL_ID;
    MMI_CTRL_ID_T               ctrl_id3 = MMIUNITCONV_CONVERTER_WIN_EDIT_3_CTRL_ID;
    MMI_CTRL_ID_T               ctrl_id4 = MMIUNITCONV_CONVERTER_WIN_EDIT_4_CTRL_ID;
    uint32                      visibleEditorNum = 0;
    GUI_BG_T  	                backgrand = {0};
    GUI_COLOR_T                 font_color_ptr = MMI_DEFAULT_TEXT_COLOR;

    editCtrlState = 0x0000;
    //style for form
    backgrand.bg_type = GUI_BG_COLOR;
    backgrand.color =  MMI_WHITE_COLOR; //MMI_BACKGROUND_COLOR;
    GUIFORM_SetStyle(form_id,GUIFORM_STYLE_NORMAL);
    GUIFORM_SetBg(form_id, &backgrand);
    GUIFORM_PermitChildFont(form_id,FALSE);
    GUIFORM_PermitChildBorder(form_id,FALSE);
    GUIFORM_PermitChildBg(form_id,FALSE);

    GUIEDIT_SetStyle(ctrl_id1, GUIEDIT_STYLE_MULTI_DYN_DOWN);
    GUIEDIT_SetStyle(ctrl_id2, GUIEDIT_STYLE_MULTI_DYN_DOWN);
    GUIEDIT_SetStyle(ctrl_id3, GUIEDIT_STYLE_MULTI_DYN_DOWN);
    GUIEDIT_SetStyle(ctrl_id4, GUIEDIT_STYLE_MULTI_DYN_DOWN);
    GUIEDIT_SetFontColor(ctrl_id1, MMI_BLACK_COLOR);
    GUIEDIT_SetFontColor(ctrl_id2, MMI_BLACK_COLOR);
    GUIEDIT_SetFontColor(ctrl_id3, MMI_BLACK_COLOR);
    GUIEDIT_SetFontColor(ctrl_id4, MMI_BLACK_COLOR);

    if(mainType < UC_CONV_MAIN_CURRENCY)
    {
        visibleEditorNum = 2;
        GUIEDIT_SetIm(ctrl_id1, GUIIM_TYPE_DIGITAL, GUIIM_TYPE_DIGITAL);
        GUIEDIT_SetIm(ctrl_id2, GUIIM_TYPE_DIGITAL, GUIIM_TYPE_DIGITAL);

        GUIEDIT_SetSoftkeyEx(ctrl_id1, LEFT_BUTTON, 1, TXT_NULL, TXT_NULL, PNULL, PNULL, EDIT_BUTTON_TEXT_ID,  PNULL);
        GUIEDIT_SetSoftkeyEx(ctrl_id1, MIDDLE_BUTTON, 0, TXT_NULL, TXT_NULL, IMAGE_NULL, IMAGE_NULL, EDIT_BUTTON_TEXT_ID,  PNULL);
        GUIEDIT_SetSoftkeyEx(ctrl_id1, RIGHT_BUTTON, 1, TXT_COMMON_COMMON_BACK, STR_LIST_DELETE, IMAGE_NULL, IMAGE_NULL, EDIT_BUTTON_TEXT_ID,  PNULL);
        GUIEDIT_SetSoftkeyEx(ctrl_id2, LEFT_BUTTON, 1, TXT_NULL, TXT_NULL, PNULL, PNULL, EDIT_BUTTON_TEXT_ID,  PNULL);
        GUIEDIT_SetSoftkeyEx(ctrl_id2, MIDDLE_BUTTON, 0, TXT_NULL, TXT_NULL, IMAGE_NULL, IMAGE_NULL, EDIT_BUTTON_TEXT_ID,  PNULL);
        GUIEDIT_SetSoftkeyEx(ctrl_id2, RIGHT_BUTTON, 1, TXT_COMMON_COMMON_BACK, STR_LIST_DELETE, IMAGE_NULL, IMAGE_NULL, EDIT_BUTTON_TEXT_ID,  PNULL);

        GUIFORM_SetChildDisplay(form_id, ctrl_id3, GUIFORM_CHILD_DISP_HIDE);
        GUIFORM_SetChildDisplay(form_id, ctrl_id4, GUIFORM_CHILD_DISP_HIDE);
    }
    else if(mainType == UC_CONV_MAIN_CURRENCY)
    {
        visibleEditorNum = 3;
        GUIEDIT_SetIm(ctrl_id1, GUIIM_TYPE_DIGITAL, GUIIM_TYPE_DIGITAL);
        GUIEDIT_SetIm(ctrl_id2, GUIIM_TYPE_DIGITAL, GUIIM_TYPE_DIGITAL);
        GUIEDIT_SetIm(ctrl_id3, GUIIM_TYPE_DIGITAL, GUIIM_TYPE_DIGITAL);

        GUIEDIT_SetSoftkeyEx(ctrl_id1, LEFT_BUTTON, 0, TXT_NULL, TXT_NULL, IMAGE_NULL, IMAGE_NULL, EDIT_BUTTON_TEXT_ID,  PNULL);
        GUIEDIT_SetSoftkeyEx(ctrl_id1, MIDDLE_BUTTON, 0, TXT_NULL, TXT_NULL, IMAGE_NULL, IMAGE_NULL, EDIT_BUTTON_TEXT_ID,  PNULL);
        GUIEDIT_SetSoftkeyEx(ctrl_id1, RIGHT_BUTTON, 1, TXT_COMMON_COMMON_BACK, STR_LIST_DELETE, IMAGE_NULL, IMAGE_NULL, EDIT_BUTTON_TEXT_ID,  PNULL);

        GUIEDIT_SetSoftkeyEx(ctrl_id2, LEFT_BUTTON, 0, TXT_NULL, TXT_NULL, IMAGE_NULL, IMAGE_NULL, EDIT_BUTTON_TEXT_ID,  PNULL);
        GUIEDIT_SetSoftkeyEx(ctrl_id2, MIDDLE_BUTTON, 0, TXT_NULL, TXT_NULL, IMAGE_NULL, IMAGE_NULL, EDIT_BUTTON_TEXT_ID,  PNULL);
        GUIEDIT_SetSoftkeyEx(ctrl_id2, RIGHT_BUTTON, 1, TXT_COMMON_COMMON_BACK, STR_LIST_DELETE, IMAGE_NULL, IMAGE_NULL, EDIT_BUTTON_TEXT_ID,  PNULL);


        GUIEDIT_SetSoftkeyEx(ctrl_id3, LEFT_BUTTON, 0, TXT_NULL, TXT_NULL, IMAGE_NULL, IMAGE_NULL, EDIT_BUTTON_TEXT_ID,  PNULL);
        GUIEDIT_SetSoftkeyEx(ctrl_id3, MIDDLE_BUTTON, 0, TXT_NULL, TXT_NULL, IMAGE_NULL, IMAGE_NULL, EDIT_BUTTON_TEXT_ID,  PNULL);
        GUIEDIT_SetSoftkeyEx(ctrl_id3, RIGHT_BUTTON, 1, TXT_COMMON_COMMON_BACK, STR_LIST_DELETE, IMAGE_NULL, IMAGE_NULL, EDIT_BUTTON_TEXT_ID,  PNULL);

        GUIFORM_SetChildDisplay(form_id, ctrl_id4, GUIFORM_CHILD_DISP_HIDE);
    }
    else
    {
        if(subType == UC_CONV_SUBSET_CUSTOMIZE_CONVERT)
        {
            visibleEditorNum = 2;

            GUIEDIT_SetIm(ctrl_id1, GUIIM_TYPE_DIGITAL, GUIIM_TYPE_DIGITAL);
            GUIEDIT_SetIm(ctrl_id2, GUIIM_TYPE_DIGITAL, GUIIM_TYPE_DIGITAL);

            GUIEDIT_SetSoftkeyEx(ctrl_id1, LEFT_BUTTON, 0, STXT_SOFTKEY_EDIT_MK, STXT_SOFTKEY_EDIT_MK, IMAGE_NULL, IMAGE_NULL, EDIT_BUTTON_TEXT_ID,  PNULL);
            GUIEDIT_SetSoftkeyEx(ctrl_id1, MIDDLE_BUTTON, 0, TXT_NULL, TXT_NULL, IMAGE_NULL, IMAGE_NULL, EDIT_BUTTON_TEXT_ID,  PNULL);
            GUIEDIT_SetSoftkeyEx(ctrl_id1, RIGHT_BUTTON, 1, TXT_COMMON_COMMON_BACK, STR_LIST_DELETE, IMAGE_NULL, IMAGE_NULL, EDIT_BUTTON_TEXT_ID,  PNULL);

            GUIEDIT_SetSoftkeyEx(ctrl_id2, LEFT_BUTTON, 0, STXT_SOFTKEY_EDIT_MK, STXT_SOFTKEY_EDIT_MK, IMAGE_NULL, IMAGE_NULL, EDIT_BUTTON_TEXT_ID,  PNULL);
            GUIEDIT_SetSoftkeyEx(ctrl_id2, MIDDLE_BUTTON, 0, TXT_NULL, TXT_NULL, IMAGE_NULL, IMAGE_NULL, EDIT_BUTTON_TEXT_ID,  PNULL);
            GUIEDIT_SetSoftkeyEx(ctrl_id2, RIGHT_BUTTON, 1, TXT_COMMON_COMMON_BACK, STR_LIST_DELETE, IMAGE_NULL, IMAGE_NULL, EDIT_BUTTON_TEXT_ID,  PNULL);

            CTRLBASEFLEX_ClearAllStr(ctrl_id3);
            CTRLBASEFLEX_ClearAllStr(ctrl_id4);
            GUIFORM_SetChildDisplay(form_id, ctrl_id3, GUIFORM_CHILD_DISP_HIDE);
            GUIFORM_SetChildDisplay(form_id, ctrl_id4, GUIFORM_CHILD_DISP_HIDE);
        }
        else if(subType == UC_CONV_SUBSET_CUSTOMIZE_NEW
            || subType == UC_CONV_SUBSET_CUSTOMIZE_EDIT)
        {
            visibleEditorNum = 4;

            GUIEDIT_SetIm(ctrl_id1, GUIIM_TYPE_ABC | GUIIM_TYPE_SMART | GUIIM_TYPE_DIGITAL | GUIIM_TYPE_ENGLISH,GUIIM_TYPE_ABC);
            GUIEDIT_SetIm(ctrl_id2, GUIIM_TYPE_ABC | GUIIM_TYPE_SMART | GUIIM_TYPE_DIGITAL | GUIIM_TYPE_ENGLISH,GUIIM_TYPE_ABC);
            GUIEDIT_SetIm(ctrl_id3, GUIIM_TYPE_ABC | GUIIM_TYPE_SMART | GUIIM_TYPE_DIGITAL | GUIIM_TYPE_ENGLISH,GUIIM_TYPE_ABC);
            GUIEDIT_SetIm(ctrl_id4, GUIIM_TYPE_DIGITAL, GUIIM_TYPE_DIGITAL);

            GUIEDIT_SetSoftkeyEx(ctrl_id1, LEFT_BUTTON, 0, TXT_NULL, TXT_NULL, IMAGE_NULL, IMAGE_NULL, EDIT_BUTTON_TEXT_ID,  PNULL);
            GUIEDIT_SetSoftkeyEx(ctrl_id1, MIDDLE_BUTTON, 0, STXT_SOFTKEY_SAVE_MK, TXT_NULL, IMAGE_NULL, IMAGE_NULL, EDIT_BUTTON_TEXT_ID,  PNULL);
            GUIEDIT_SetSoftkeyEx(ctrl_id1, RIGHT_BUTTON, 1, TXT_COMMON_COMMON_BACK, STR_LIST_DELETE, IMAGE_NULL, IMAGE_NULL, EDIT_BUTTON_TEXT_ID,  PNULL);

            GUIEDIT_SetSoftkeyEx(ctrl_id2, LEFT_BUTTON, 0, TXT_NULL, TXT_NULL, IMAGE_NULL, IMAGE_NULL, EDIT_BUTTON_TEXT_ID,  PNULL);
            GUIEDIT_SetSoftkeyEx(ctrl_id2, MIDDLE_BUTTON, 0, STXT_SOFTKEY_SAVE_MK, TXT_NULL, IMAGE_NULL, IMAGE_NULL, EDIT_BUTTON_TEXT_ID,  PNULL);
            GUIEDIT_SetSoftkeyEx(ctrl_id2, RIGHT_BUTTON, 1, TXT_COMMON_COMMON_BACK, STR_LIST_DELETE, IMAGE_NULL, IMAGE_NULL, EDIT_BUTTON_TEXT_ID,  PNULL);

            GUIEDIT_SetSoftkeyEx(ctrl_id3, LEFT_BUTTON, 0, TXT_NULL, TXT_NULL, IMAGE_NULL, IMAGE_NULL, EDIT_BUTTON_TEXT_ID,  PNULL);
            GUIEDIT_SetSoftkeyEx(ctrl_id3, MIDDLE_BUTTON, 0, STXT_SOFTKEY_SAVE_MK, TXT_NULL, IMAGE_NULL, IMAGE_NULL, EDIT_BUTTON_TEXT_ID,  PNULL);
            GUIEDIT_SetSoftkeyEx(ctrl_id3, RIGHT_BUTTON, 1, TXT_COMMON_COMMON_BACK, STR_LIST_DELETE, IMAGE_NULL, IMAGE_NULL, EDIT_BUTTON_TEXT_ID,  PNULL);

            GUIEDIT_SetSoftkeyEx(ctrl_id4, LEFT_BUTTON, 0, TXT_NULL, TXT_NULL, IMAGE_NULL, IMAGE_NULL, EDIT_BUTTON_TEXT_ID,  PNULL);
            GUIEDIT_SetSoftkeyEx(ctrl_id4, MIDDLE_BUTTON, 0, STXT_SOFTKEY_SAVE_MK, TXT_NULL, IMAGE_NULL, IMAGE_NULL, EDIT_BUTTON_TEXT_ID,  PNULL);
            GUIEDIT_SetSoftkeyEx(ctrl_id4, RIGHT_BUTTON, 1, TXT_COMMON_COMMON_BACK, STR_LIST_DELETE, IMAGE_NULL, IMAGE_NULL, EDIT_BUTTON_TEXT_ID,  PNULL);
            //if(subType == UC_CONV_SUBSET_CUSTOMIZE_NEW)
            {
                GUIWIN_SetSoftkeyBtnGray(win_id, MIDDLE_BUTTON, TRUE);
                custConvCanbeSave = FALSE;
            }
            /*else
            {
                GUIWIN_SetSoftkeyBtnGray(win_id, MIDDLE_BUTTON, TRUE);
                custConvCanbeSave = FALSE;
            }*/

            GUIFORM_SetChildDisplay(form_id, ctrl_id3, GUIFORM_CHILD_DISP_NORMAL);
            GUIFORM_SetChildDisplay(form_id, ctrl_id4, GUIFORM_CHILD_DISP_NORMAL);

            GUIEDIT_SetMaxLen(ctrl_id1,MMIUC_INPUT_MAX_LEN);
            GUIEDIT_SetMaxLen(ctrl_id2,MMIUC_INPUT_MAX_LEN);
            GUIEDIT_SetMaxLen(ctrl_id3,MMIUC_INPUT_MAX_LEN);
            GUIEDIT_SetMaxLen(ctrl_id4,MMIUC_INPUT_MAX_LEN);
        }
    }

    switch(mainType)
    {
        case UC_CONV_MAIN_TEMPERATURE:
            if(subType == UC_CONV_SUBSET_TEMPURE_F_C)
            {
                CTRLBASEEDIT_SetLabelId(win_id, ctrl_id1, STR_CNV_FAHRENHEIT, FALSE);
                CTRLBASEEDIT_SetLabelId(win_id, ctrl_id2, STR_CNV_CELSIUS, FALSE);
            }
            else if(subType == UC_CONV_SUBSET_TEMPURE_F_K)
            {
                CTRLBASEEDIT_SetLabelId(win_id, ctrl_id1, STR_CNV_KELVIN, FALSE);
                CTRLBASEEDIT_SetLabelId(win_id, ctrl_id2, STR_CNV_FAHRENHEIT, FALSE);
            }
            else if(subType == UC_CONV_SUBSET_TEMPURE_C_K)
            {
                CTRLBASEEDIT_SetLabelId(win_id, ctrl_id1, STR_CNV_KELVIN, FALSE);
                CTRLBASEEDIT_SetLabelId(win_id, ctrl_id2, STR_CNV_CELSIUS, FALSE);
            }
            break;
        case UC_CONV_MAIN_WEIGHT:
            if(subType == UC_CONV_SUBSET_WEIGHT_POUND_KILO)
            {
                CTRLBASEEDIT_SetLabelId(win_id, ctrl_id1, STR_CNV_POUND, FALSE);
                CTRLBASEEDIT_SetLabelId(win_id, ctrl_id2, STR_CNV_KILO, FALSE);
            }
            else if(subType == UC_CONV_SUBSET_WEIGHT_CARAT_GRAM)
            {
                CTRLBASEEDIT_SetLabelId(win_id, ctrl_id1, STR_CNV_CARAT, FALSE);
                CTRLBASEEDIT_SetLabelId(win_id, ctrl_id2, STR_CNV_GRAM, FALSE);
            }
            else if(subType == UC_CONV_SUBSET_WEIGHT_OUNCE_GRAM)
            {
                CTRLBASEEDIT_SetLabelId(win_id, ctrl_id1, STR_CNV_OUNCE, FALSE);
                CTRLBASEEDIT_SetLabelId(win_id, ctrl_id2, STR_CNV_GRAM, FALSE);
            }
            else if(subType == UC_CONV_SUBSET_WEIGHT_TROYOUNCE_GRAM)
            {
                CTRLBASEEDIT_SetLabelId(win_id, ctrl_id1, STR_CNV_TROYOZ, FALSE);
                CTRLBASEEDIT_SetLabelId(win_id, ctrl_id2, STR_CNV_GRAM, FALSE);
            }
            else if(subType == UC_CONV_SUBSET_WEIGHT_STONE_KILO)
            {
                CTRLBASEEDIT_SetLabelId(win_id, ctrl_id1, STR_CNV_STONE, FALSE);
                CTRLBASEEDIT_SetLabelId(win_id, ctrl_id2, STR_CNV_KILO, FALSE);
            }
            break;
        case UC_CONV_MAIN_LENGTH:
            if(subType == UC_CONV_SUBSET_LENGHT_MILE_KILOMETRE)
            {
                CTRLBASEEDIT_SetLabelId(win_id, ctrl_id1, STR_CNV_MILE, FALSE);
                CTRLBASEEDIT_SetLabelId(win_id, ctrl_id2, STR_CNV_KM, FALSE);
            }
            else if(subType == UC_CONV_SUBSET_LENGHT_YARD_METRE)
            {
                CTRLBASEEDIT_SetLabelId(win_id, ctrl_id1, STR_CNV_YARD, FALSE);
                CTRLBASEEDIT_SetLabelId(win_id, ctrl_id2, STR_CNV_METER, FALSE);
            }
            else if(subType == UC_CONV_SUBSET_LENGHT_FOOT_METRE)
            {
                CTRLBASEEDIT_SetLabelId(win_id, ctrl_id1, STR_CNV_FOOT, FALSE);
                CTRLBASEEDIT_SetLabelId(win_id, ctrl_id2, STR_CNV_METER, FALSE);
            }
            else if(subType == UC_CONV_SUBSET_LENGHT_FOOT_CENTIMETRE)
            {
                CTRLBASEEDIT_SetLabelId(win_id, ctrl_id1, STR_CNV_FOOT, FALSE);
                CTRLBASEEDIT_SetLabelId(win_id, ctrl_id2, STR_CNV_CM, FALSE);
            }
            else if(subType == UC_CONV_SUBSET_LENGHT_INCH_CENTIMETRE)
            {
                CTRLBASEEDIT_SetLabelId(win_id, ctrl_id1, STR_CNV_INCH, FALSE);
                CTRLBASEEDIT_SetLabelId(win_id, ctrl_id2, STR_CNV_CM, FALSE);
            }
            break;
        case UC_CONV_MAIN_AREA:
            if(subType == UC_CONV_SUBSET_AREA_FOOT_METRE)
            {
                CTRLBASEEDIT_SetLabelId(win_id, ctrl_id1, STR_CNV_SQFOOT, FALSE);
                CTRLBASEEDIT_SetLabelId(win_id, ctrl_id2, STR_CNV_SQMTR, FALSE);
            }
            else if(subType == UC_CONV_SUBSET_AREA_INCH_CENTIMETRE)
            {
                CTRLBASEEDIT_SetLabelId(win_id, ctrl_id1, STR_CNV_SQINCH, FALSE);
                CTRLBASEEDIT_SetLabelId(win_id, ctrl_id2, STR_CNV_SQCM, FALSE);
            }
            else if(subType == UC_CONV_SUBSET_AREA_YARD_METRE)
            {
                CTRLBASEEDIT_SetLabelId(win_id, ctrl_id1, STR_CNV_SQYARD, FALSE);
                CTRLBASEEDIT_SetLabelId(win_id, ctrl_id2, STR_CNV_SQMTR, FALSE);
            }
            else if(subType == UC_CONV_SUBSET_AREA_MILE_KILOMETRE)
            {
                CTRLBASEEDIT_SetLabelId(win_id, ctrl_id1, STR_CNV_SQMILE, FALSE);
                CTRLBASEEDIT_SetLabelId(win_id, ctrl_id2, STR_CNV_SQKM, FALSE);
            }
            else if(subType == UC_CONV_SUBSET_AREA_ACRE_HECTARE)
            {
                CTRLBASEEDIT_SetLabelId(win_id, ctrl_id1, STR_CNV_ACRE, FALSE);
                CTRLBASEEDIT_SetLabelId(win_id, ctrl_id2, STR_CNV_HECTARE, FALSE);
            }
            break;
        case UC_CONV_MAIN_VOLUME:
            if(subType == UC_CONV_SUBSET_VOLUME_INCH_CENTIMETRE)
            {
                CTRLBASEEDIT_SetLabelId(win_id, ctrl_id1, STR_CNV_CUINCH, FALSE);
                CTRLBASEEDIT_SetLabelId(win_id, ctrl_id2, STR_CNV_CUCM, FALSE);
            }
            else if(subType == UC_CONV_SUBSET_VOLUME_FOOT_METRE)
            {
                CTRLBASEEDIT_SetLabelId(win_id, ctrl_id1, STR_CNV_CUFEET, FALSE);
                CTRLBASEEDIT_SetLabelId(win_id, ctrl_id2, STR_CNV_CUMTR, FALSE);
            }
            else if(subType == UC_CONV_SUBSET_VOLUME_YARD_METRE)
            {
                CTRLBASEEDIT_SetLabelId(win_id, ctrl_id1, STR_CNV_CUMTR, FALSE);
                CTRLBASEEDIT_SetLabelId(win_id, ctrl_id2, STR_CNV_CUYARD, FALSE);
            }
            else if(subType == UC_CONV_SUBSET_VOLUME_GALLON_LITRE)
            {
                CTRLBASEEDIT_SetLabelId(win_id, ctrl_id1, STR_CNV_GLNUK, FALSE);
                CTRLBASEEDIT_SetLabelId(win_id, ctrl_id2, STR_CNV_LITRE, FALSE);
            }
            break;
        case UC_CONV_MAIN_CURRENCY:
        {
            wchar  defaultExchange[] = {'1'};
            CTRLBASEEDIT_SetLabelId(win_id, ctrl_id1, STR_CURR_CONV_DTAIL_TO_DOM, FALSE);
            CTRLBASEEDIT_SetLabelId(win_id, ctrl_id2, STR_CURR_CONV_DTAIL_TO_FOR, FALSE);
            CTRLBASEEDIT_SetLabelId(win_id, ctrl_id3, STR_CURR_CONV_EXCHANGE_EXT01, FALSE);
            GUIEDIT_SetString(ctrl_id3, defaultExchange, 1);

            editCtrlState = MMIUC_EDIT_CTRL_3_INIT;
        }
            break;
        case UC_CONV_CUSTOMIZE_MAX_NUM:
            if(subType == UC_CONV_SUBSET_CUSTOMIZE_NEW)
            {
                UC_CONV_CUSTOMIZE_ITEM_T*   custItem = PNULL;
                uint                        i = 0;
                CTRLBASEEDIT_SetLabelId(win_id, ctrl_id1, STR_CNV_TITLE, FALSE);
                CTRLBASEEDIT_SetLabelId(win_id, ctrl_id2, STR_CNV_FROM, FALSE);
                CTRLBASEEDIT_SetLabelId(win_id, ctrl_id3, STR_CNV_TO, FALSE);
                CTRLBASEEDIT_SetLabelId(win_id, ctrl_id4, STR_CNV_FACTOR, FALSE);
                GUIEDIT_SetString(ctrl_id4, custDefaultFactor, MMIAPICOM_Wstrlen(custDefaultFactor));
                editCtrlState = MMIUC_EDIT_CTRL_4_INIT;

                custItem = &custConvItem.convItem[i];
                while(custItem->convEnable)
                {
                    custItem = &custConvItem.convItem[++i];
                }
                custConvChangeItemId = i;
                SCI_MEMSET((char*)&custConvChangeItem, 0x0, sizeof(UC_CONV_CUSTOMIZE_ITEM_T));
                MMIAPICOM_Wstrcpy(custConvChangeItem.factor, custDefaultFactor);

            }
            break;
        default:
            if(mainType >= UC_CONV_MAIN_CUSTOMIZE_1 
                && mainType < UC_CONV_MAIN_TYPE_NUM)
            {
                uint32 custIndex = mainType - UC_CONV_MAIN_CUSTOMIZE_1;
                custConvChangeItemId = custIndex;
                if(subType == UC_CONV_SUBSET_CUSTOMIZE_CONVERT)
                {
                    CTRLBASEFLEX_ClearAllStr(ctrl_id1);
                    CTRLBASEEDIT_SetLabel(win_id,
                                    ctrl_id1,
                                    custConvItem.convItem[custIndex].inputName,
                                    MMIAPICOM_Wstrlen(custConvItem.convItem[custIndex].inputName),
                                    FALSE);
                    CTRLBASEFLEX_ClearAllStr(ctrl_id2);
                    CTRLBASEEDIT_SetLabel(win_id,
                                    ctrl_id2,
                                    custConvItem.convItem[custIndex].outputName,
                                    MMIAPICOM_Wstrlen(custConvItem.convItem[custIndex].outputName),
                                    FALSE);
                    CTRLBASEFLEX_ClearAllStr(ctrl_id3);
                    CTRLBASEEDIT_SetLabelId(win_id, ctrl_id3, TXT_NULL, TRUE);
                    CTRLBASEFLEX_ClearAllStr(ctrl_id4);
                    CTRLBASEEDIT_SetLabelId(win_id, ctrl_id4, TXT_NULL, TRUE);
                }
                else if(subType == UC_CONV_SUBSET_CUSTOMIZE_EDIT)
                {
                    CTRLBASEFLEX_ClearAllStr(ctrl_id1);
                    CTRLBASEEDIT_SetLabelId(win_id, ctrl_id1, STR_CNV_TITLE, FALSE);
                    GUIEDIT_SetString(ctrl_id1, custConvItem.convItem[custIndex].title, MMIAPICOM_Wstrlen(custConvItem.convItem[custIndex].title));

                    CTRLBASEFLEX_ClearAllStr(ctrl_id2);
                    CTRLBASEEDIT_SetLabelId(win_id, ctrl_id2, STR_CNV_FROM, FALSE);
                    GUIEDIT_SetString(ctrl_id2, custConvItem.convItem[custIndex].inputName, MMIAPICOM_Wstrlen(custConvItem.convItem[custIndex].inputName));

                    CTRLBASEFLEX_ClearAllStr(ctrl_id3);
                    CTRLBASEEDIT_SetLabelId(win_id, ctrl_id3, STR_CNV_TO, FALSE);
                    GUIEDIT_SetString(ctrl_id3, custConvItem.convItem[custIndex].outputName, MMIAPICOM_Wstrlen(custConvItem.convItem[custIndex].outputName));

                    CTRLBASEFLEX_ClearAllStr(ctrl_id4);
                    CTRLBASEEDIT_SetLabelId(win_id, ctrl_id4, STR_CNV_FACTOR, FALSE);
                    GUIEDIT_SetString(ctrl_id4, custConvItem.convItem[custIndex].factor, MMIAPICOM_Wstrlen(custConvItem.convItem[custIndex].factor));

                    SCI_MEMCPY((char*)&custConvChangeItem, (char*)&custConvItem.convItem[custConvChangeItemId], sizeof(UC_CONV_CUSTOMIZE_ITEM_T)) ; 
                    editCtrlState = MMIUC_EDIT_CTRL_1_INIT | MMIUC_EDIT_CTRL_2_INIT | MMIUC_EDIT_CTRL_3_INIT | MMIUC_EDIT_CTRL_4_INIT;
                }
            }
            break;
    }
}
/*****************************************************************************/
// 	Description : the entry of unit conversion
//	Global resource dependence : 
//  Author:renyi.hu
//	Note: 
/*****************************************************************************/
PUBLIC void MMIAPIUNITCONV_OpenMainWin(void)
{
    MMK_CreateWin((uint32*)UNITCONV_MIAN_WIN_TAB, PNULL);
}

#else //CUSTOM_UNITCONVERSION_SUPPORT
#ifdef MAINLCD_SIZE_128X160
#define  MMIUC_INPUT_MAX_LEN  4  //编辑框输入最大字符数
#define  MMIUC_OUTPUT_MAX_LEN 9    //最大输出长度
#else
#define  MMIUC_INPUT_MAX_LEN  7  //编辑框输入最大字符数
#define  MMIUC_OUTPUT_MAX_LEN 13    //最大输出长度
#endif
#define  MMIUC_PRECISION_NUM  9  //换算结果保留小数位数
#define  MMIUC_MAX_EXPAND_NUM  10000000  //数值扩大的上限
#define  MMIUC_DECIMAL_NUM 10  
#define  MMIUC_MAX_STR_BUFF_LEN 30    //字符buff大小
#define  MMIUC_INTEGER_BUFF_LEN 10    //存放转换率整数部分buff大小
#define  MMIUC_DECIMAL_BUFF_LEN 10   //存放转换率小数部分buff大小
#define  MMIUC_MOD_BUFF_LEN  15  //存放余数buff大小

#define  MMIUC_DOT_CHAR  		'.'
#define  MMIUC_ZERO_CHAR  		'0'
#define  MMIUC_FIVE_CHAR  		'5'
#define  MMIUC_NINE_CHAR  		'9'

#define 	MMIUC_LENGTH_TYPE_NUM  4   //长度转换类型数量
#define 	MMIUC_WEIGHT_TYPE_NUM  2   //重量转换类型数量

/**--------------------------------------------------------------------------*
 **                         TYPE DEFINITION                                  *
 **--------------------------------------------------------------------------*/
//如果需要扩展该枚举,注意是否需要
//修改宏:MMIUC_WEIGHT_TYPE_NUM
typedef enum
{
    //重量转换类型
    MMIUC_TYPE_KG_POUND ,
    MMIUC_TYPE_POUND_KG ,
    MMIUC_TYPE_KG_OUNCE , 
    MMIUC_TYPE_OUNCE_KG  ,

    //长度转换类型
    MMIUC_TYPE_KM_MILE , 
    MMIUC_TYPE_MILE_KM  ,
    MMIUC_TYPE_M_YARD ,
    MMIUC_TYPE_YARD_M   ,
    MMIUC_TYPE_M_FOOT, 
    MMIUC_TYPE_FOOT_M ,   
    MMIUC_TYPE_CM_INCH , 
    MMIUC_TYPE_INCH_CM  ,

    MMIUC_TYPE_MAX
}UC_CONV_TYPE_E;

typedef enum
{
    UC_TYPE_WEIGHT,
    UC_TYPE_LENGTH,
    UC_TYPE_MAX
}UC_TYPE_E;

typedef enum
{
    UC_WEIGHT_KILOGRAM,
    UC_WEIGHT_POUND,
    UC_WEIGHT_OUNCE,
    UC_WEIGHT_MAX
}UC_WEIGHT_E;

typedef enum
{
    UC_LENGTH_KILOMETER,
    UC_LENGTH_METER,
    UC_LENGTH_CENTIMETER,
    UC_LENGTH_MILE,
    UC_LENGTH_YARD,
    UC_LENGTH_FOOT,
    UC_LENGTH_INCH,
    UC_LENGTH_MAX
}UC_LENGTH_E;

/*---------------------------------------------------------------------------*/
/*                          LOCAL DATA DECLARE                               */
/*---------------------------------------------------------------------------*/
LOCAL UC_TYPE_E s_uc_type=UC_TYPE_MAX;
LOCAL BOOLEAN s_type_pressed = FALSE;

#ifdef MMI_PDA_SUPPORT
LOCAL uint16 s_picker1_selected = 0;
LOCAL uint16 s_picker2_selected = 0;
LOCAL wchar s_result_buffer[MMIUC_OUTPUT_MAX_LEN + 1] = {0};  

LOCAL const char  *s_weight_conversion_rate[UC_WEIGHT_MAX][UC_WEIGHT_MAX]=
{
	"1.0",           "2.2046",      "35.2740",	                
	"0.4536",    "1.0",             "16.0",
	"0.0283",    "0.0625",      "1.0",
};

LOCAL const char  *s_length_conversion_rate[UC_LENGTH_MAX][UC_LENGTH_MAX]=//CR147456
{    
	"1.0",                "1000.0",     "100000.0",    "0.62137",             "1093.6",          "3280.8",           "39370.0",                  
	"0.001",	         "1.0",            "100.0",          "0.00062137",      "1.0936",          "3.2808",            "39.37",                                   
        "0.00001",       "0.01",          "1.0",                "0.0000062137",  "0.010936",     "0.032808",        "0.3937",               
        "1.6093",         "1609.3",      "160934.4",     "1.0",                    "1760.0",         "5280.0",            "63360.0",                          
	"0.0009144",   "0.9144",    "91.44",           "0.0005682",       "1.0",                "3.0",                   "36.0",                   
	"0.0003048",   "0.3048",    "30.48",            "0.0001894",      "0.3333",         "1.0",                   "12.0",                      
	"0.0000254",   "0.0254",    "2.5400",         "0.00001578",      "0.02778",     "0.08333",          "1.0",                  
};

const  uint32 s_weight_text_id[UC_WEIGHT_MAX] =
{
    TXT_UC_KILOGRAM,
    TXT_UC_POUND,
    TXT_UC_OUNCE,
};

const  uint32 s_length_text_id[UC_LENGTH_MAX] =
{    
    TXT_UC_KILOMETER,
    TXT_UC_METER,    
    TXT_UC_CENTIMETER,    
    TXT_UC_MILE,
    TXT_UC_YARD,
    TXT_UC_FOOT,
    TXT_UC_INCH,
};

#else
LOCAL UC_CONV_TYPE_E s_conversion_type=MMIUC_TYPE_MAX;//转换类型
LOCAL BOOLEAN s_conversion_type_pressed = FALSE;

//不同单位之间的换算关系，对应关系参看枚举UC_CONV_TYPE_E；
LOCAL const char  *s_conversion_rate[]=
{
	"2.2046",	
	"0.4536",
	"35.2740",
	"0.0283",
	
	"0.62137",//CR147456
	"1.6093",
	"1.0936",
	"0.9144",
	"3.2808",
	"0.3048",
	"0.3937",
	"2.5400",
};

const  uint32 s_weight_text_id[MMIUC_WEIGHT_TYPE_NUM] =
{
    TXT_UC_KILOGRAM_POUND,
    TXT_UC_KILOGRAM_OUNCE,
};

const  uint32 s_length_text_id[MMIUC_LENGTH_TYPE_NUM] =
{
    TXT_UC_KILOMETER_MILE,
    TXT_UC_METER_YARD,
    TXT_UC_METER_FOOT,
    TXT_UC_CENTIMETER_INCH,
};
#endif

//LOCAL uint32 s_last_setlist_index=0;//记录上次setlist index的值
/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
/*****************************************************************************/
// 	Description :判断字符串是否合法
//	Global resource dependence : 
//  Author:renyi.hu
//	Note: 形式为整数或小数才合法
/*****************************************************************************/
LOCAL BOOLEAN InputIsValid(
							MMI_STRING_T const *in_str_ptr //in
							);

/*****************************************************************************/
// 	Description :判断字符串是否合法
//	Global resource dependence : 
//  Author:renyi.hu
//	Note: 形式为整数或小数才合法
/*****************************************************************************/
LOCAL BOOLEAN StrIsValid(
							MMI_STRING_T const *in_str_ptr //in
							);

/*****************************************************************************/
//  Description : display uc type
//  Global resource dependence : none
//  Author: paul.huang
//  Note:
/*****************************************************************************/
LOCAL void DisplayUcType(GUIOWNDRAW_INFO_T *owner_draw_ptr);

/*****************************************************************************/
//  Description : handle owndraw tp down
//  Global resource dependence :
//  Author: paul.huang
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleOwnDrawTpDown(
                                MMI_HANDLE_T ctrl_handle,
                                GUI_POINT_T tp_point
                                );

/*****************************************************************************/
//  Description : handle owndraw tp up
//  Global resource dependence :
//  Author: paul.huang
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleOwnDrawTpUp(
                              MMI_HANDLE_T ctrl_handle,
                              GUI_POINT_T tp_point
                              );

/*****************************************************************************/
//  Description : handle owndraw tp move
//  Global resource dependence :
//  Author: paul.huang
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleOwnDrawTpMove(
                                MMI_HANDLE_T ctrl_handle,
                                GUI_POINT_T tp_point
                                );

/*****************************************************************************/
//  Discription:  handle change type win
//  Global resource dependence: none 
//  Author:paul.huang
//  Note:
//
/*****************************************************************************/
LOCAL MMI_RESULT_E   HandleChangeTypeWinMsg(
                                          MMI_WIN_ID_T win_id, 
                                          MMI_MESSAGE_ID_E msg_id, 
                                          DPARAM param             
                                          );

#ifdef MMI_PDA_SUPPORT
 /*****************************************************************************/
// 	Description : 将输入的字符串经过单位换算后以字符串输出
//	Global resource dependence : 
//  Author:renyi.hu
//	Note: 
/*****************************************************************************/
LOCAL BOOLEAN  UnitConversion (
							uint16  selected1,//in
							uint16  selectde2,//in
							const MMI_STRING_T  *in_str_ptr, //in 
							MMI_STRING_T  *out_str_ptr//out
							);

/*****************************************************************************/
// 	Description : 根据换算率进行单位换算
//	Global resource dependence : 
//  Author:renyi.hu
//	Note: 
/*****************************************************************************/
 LOCAL BOOLEAN  UnitConversionByRate(
									 const MMI_STRING_T  *in_str, //in 
									 const char *conv_rate, // in 
									 MMI_STRING_T  *out_str //out
									 );

/*****************************************************************************/
//  Description : display result
//  Global resource dependence : none
//  Author: paul.huang
//  Note:
/*****************************************************************************/
LOCAL void DisplayResultString(GUIOWNDRAW_INFO_T *owner_draw_ptr);
/*****************************************************************************/
//  Description : Display H Line
//  Global resource dependence : none
//  Author: renyi.hu
//  Note:
/*****************************************************************************/
LOCAL void DisplayLine(void);
#else
/*****************************************************************************/
// 	Description : 设置单位换算显示时各种ctrl、字体等的属性-》只用于计算自适应str使用
//	Global resource dependence :
//  Author:liyan.zhu
//	Note:CR152753
/*****************************************************************************/
LOCAL MMI_RESULT_E ComputeUnitCoversionAdjustSet(
												 MMI_STRING_T    *in_str_ptr,
												 MMI_STRING_T    *out_str_ptr   //可显示下的宽度            
												 );

/*****************************************************************************/
// 	Description : 获取一个字符串显示时的自适应string
//	Global resource dependence :
//  Author:liyan.zhu
//	Note:CR152753
/*****************************************************************************/
LOCAL MMI_RESULT_E ComputAdjustStr(
								   MMI_STRING_T    *in_str_ptr,
								   uint16          ctrl_width,//string显示要小于的宽度
								   GUISTR_STYLE_T  font_style,//字体大小等描述
								   GUISTR_STATE_T  state,//字符串类型
								   MMI_STRING_T    *out_str_ptr   //可显示下的宽度            
);
/*****************************************************************************/
//  Description : 转换字符串到整数
//  Global resource dependence : 
//  Author:renyi.hu
//  Note: 若字符串为小数形式，则扩大成整数形式，并记录
//        下扩大的倍数
/*****************************************************************************/
LOCAL void  StrToInt32(
                        const MMI_STRING_T  *in_str_ptr, //in 
                        UC_STR_TO_NUM_T  *out_num  //out
                        );

/*****************************************************************************/
// 	Description : 去掉小数点后无效0
//	Global resource dependence : 
//  Author:renyi.hu
//	Note: 
/*****************************************************************************/
  LOCAL  void  DiscardStringInvalidZero(
  										MMI_STRING_T *in_str  //in & out
  										);  

/*****************************************************************************/
// 	Description : 对字符串4舍5入
//	Global resource dependence : 
//  Author:renyi.hu
//	Note: 
/*****************************************************************************/
  LOCAL  void  DiscardStringRedundance(
										  MMI_STRING_T *in_str,//in & out
										  uint32 num  // in,最多保留小数位数
										  );
 
/*****************************************************************************/
// 	Description : 根据扩大倍数移动小数点位置
//	Global resource dependence : 
//  Author:renyi.hu
//	Note: 
/*****************************************************************************/
LOCAL  void  MoveDotByExpand(
								MMI_STRING_T *in_str,//in & out
								uint32 expand  //in
								);

/*****************************************************************************/
// 	Description : 转换整数结果到字符串。
//	Global resource dependence : 
//  Author:renyi.hu
//	Note: 如果整数经过放大了的，则加上小数点
/*****************************************************************************/
LOCAL void ConversionResultToString(
										UC_STR_TO_NUM_T * in_num, // in 
										MMI_STRING_T  *out_str_ptr  //out
										);

/*****************************************************************************/
// 	Description : 将输入的字符串经过单位换算后以字符串输出
//	Global resource dependence : 
//  Author:renyi.hu
//	Note: 
/*****************************************************************************/
LOCAL void  UnitConversion (
							UC_CONV_TYPE_E type,//in
							const MMI_STRING_T  *in_str_ptr, //in 
							MMI_STRING_T  *out_str_ptr//out
							);

/*****************************************************************************/
// 	Description : 根据换算率进行单位换算
//	Global resource dependence : 
//  Author:renyi.hu
//	Note: 
/*****************************************************************************/
 LOCAL void  UnitConversionByRate(
									 UC_STR_TO_NUM_T *in_num, //in 
									 const char *conv_rate, // in 
									 MMI_STRING_T  *out_str //out
									 );

/*****************************************************************************/
// 	Description : 根据转换类型设置文本显示标签文本
//	Global resource dependence : 
//  Author:renyi.hu
//	Note: 
/*****************************************************************************/
  LOCAL  void SetTextLabelText(
							  UC_CONV_TYPE_E type // in 
							  );

/*****************************************************************************/
//  Description : display conversion type
//  Global resource dependence : none
//  Author: paul.huang
//  Note:
/*****************************************************************************/
LOCAL void DisplayUcConversionType(GUIOWNDRAW_INFO_T *owner_draw_ptr);

/*****************************************************************************/
//  Discription:  handle change conversion type win
//  Global resource dependence: none 
//  Author:paul.huang
//  Note:
//
/*****************************************************************************/
LOCAL MMI_RESULT_E   HandleChangeConversionTypeWinMsg(
                                          MMI_WIN_ID_T win_id, 
                                          MMI_MESSAGE_ID_E msg_id, 
                                          DPARAM param             
                                          );
#endif

/*****************************************************************************/
// 	Description : 长度重量单位换算窗口消息处理函数
//	Global resource dependence :
//  Author:renyi.hu
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleUnitConvertWinMsg(
    MMI_WIN_ID_T         win_id,
    MMI_MESSAGE_ID_E     msg_id,
    DPARAM               param
);

/*---------------------------------------------------------------------------*/
/*                          GLOBE DATA DECLARE                               */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          LOCAL DATA DECLARE                               */
/*---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         Constant Variables                                *
 **---------------------------------------------------------------------------*/
WINDOW_TABLE(UNITCONV_CHANGE_TYPE_WIN_TAB) = 
{ 
    WIN_FUNC((uint32) HandleChangeTypeWinMsg ),
    WIN_ID(MMIUNITCONV_CHANGE_TYPE_WIN_ID),
    WIN_TITLE(TXT_UC_SWITCH_UNIT),  
    CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E,MMIUNITCONV_CHANGE_TYPE_CTRL_ID ),
#ifdef MMI_PDA_SUPPORT
    WIN_SOFTKEY(TXT_NULL, TXT_NULL, STXT_CANCEL),
#else
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_CANCEL),
#endif
    END_WIN    
 };

#ifdef MMI_PDA_SUPPORT
WINDOW_TABLE(UNITCONV_MIAN_WIN_TAB) =
{
    WIN_FUNC((uint32)HandleUnitConvertWinMsg),
    WIN_ID(MMIUNITCONV_MAIN_WIN_ID),
    WIN_STATUSBAR,
    WIN_TITLE(TXT_UC_UNIT_CONVERSION),
    CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER, MMIUNITCONV_MAIN_WIN_FORM_CTRL_ID),    
        CHILD_OWNDRAW_CTRL(TRUE, MMIUNITCONV_OWNDRAW_TYPE_CTRL_ID, MMIUNITCONV_MAIN_WIN_FORM_CTRL_ID, (uint32)DisplayUcType),

        CHILD_FORM_CTRL(TRUE, GUIFORM_LAYOUT_SBS, MMIUNITCONV_MAIN_WIN_FORM1_CTRL_ID, MMIUNITCONV_MAIN_WIN_FORM_CTRL_ID),
            CHILD_EDIT_TEXT_CTRL(TRUE, MMIUC_INPUT_MAX_LEN, MMIUNITCONV_EDIT1_CTRL_ID, MMIUNITCONV_MAIN_WIN_FORM1_CTRL_ID),
            CHILD_LABEL_CTRL(GUILABEL_ALIGN_MIDDLE, FALSE, MMIUNITCONV_UNIT_LABEL1_CTRL_ID, MMIUNITCONV_MAIN_WIN_FORM1_CTRL_ID),
            CHILD_OWNDRAW_CTRL(FALSE, MMIUNITCONV_OWNDRAW_RESULT_CTRL_ID, MMIUNITCONV_MAIN_WIN_FORM1_CTRL_ID, (uint32)DisplayResultString),

        CHILD_FORM_CTRL(TRUE, GUIFORM_LAYOUT_SBS, MMIUNITCONV_MAIN_WIN_FORM2_CTRL_ID, MMIUNITCONV_MAIN_WIN_FORM_CTRL_ID),
            CHILD_EDIT_PICKER_CTRL(TRUE, MMIUNITCONV_EDIT_PICKER1_CTRL_ID, MMIUNITCONV_MAIN_WIN_FORM2_CTRL_ID),
            CHILD_EDIT_PICKER_CTRL(TRUE, MMIUNITCONV_EDIT_PICKER2_CTRL_ID, MMIUNITCONV_MAIN_WIN_FORM2_CTRL_ID),
            
    END_WIN    
};
#else
WINDOW_TABLE(UNITCONV_CHANGE_CONVERSION_TYPE_WIN_TAB) = 
{ 
    WIN_FUNC((uint32) HandleChangeConversionTypeWinMsg ),
    WIN_ID(MMIUNITCONV_CHANGE_CONVERSION_TYPE_WIN_ID),
    WIN_TITLE(TXT_UC_SWITCH_UNIT),  
    CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E,MMIUNITCONV_CHANGE_TYPE_CTRL_ID ),
#ifdef MMI_PDA_SUPPORT
    WIN_SOFTKEY(TXT_NULL, TXT_NULL, STXT_CANCEL),
#else
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_CANCEL),
#endif
    END_WIN    
 };

WINDOW_TABLE(UNITCONV_MIAN_WIN_TAB) =
{
    WIN_FUNC((uint32)HandleUnitConvertWinMsg),
    WIN_ID(MMIUNITCONV_MAIN_WIN_ID),
#ifndef MMI_ONLY_IDLE_DISP_STATUSBAR
    WIN_STATUSBAR,
#endif
    WIN_TITLE(TXT_UC_UNIT_CONVERSION),
    CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER, MMIUNITCONV_MAIN_WIN_FORM_CTRL_ID),    
        CHILD_OWNDRAW_CTRL(TRUE, MMIUNITCONV_OWNDRAW_TYPE_CTRL_ID, MMIUNITCONV_MAIN_WIN_FORM_CTRL_ID, (uint32)DisplayUcType),
        CHILD_OWNDRAW_CTRL(TRUE, MMIUNITCONV_OWNDRAW_CONVERSION_TYPE_CTRL_ID, MMIUNITCONV_MAIN_WIN_FORM_CTRL_ID, (uint32)DisplayUcConversionType),

    CHILD_FORM_CTRL(TRUE, GUIFORM_LAYOUT_SBS, MMIUNITCONV_MAIN_WIN_FORM1_CTRL_ID, MMIUNITCONV_MAIN_WIN_FORM_CTRL_ID),
        CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT, FALSE, MMIUNITCONV_UNIT_LABEL1_CTRL_ID, MMIUNITCONV_MAIN_WIN_FORM1_CTRL_ID),
        CHILD_EDIT_TEXT_CTRL(TRUE, MMIUC_MAX_STR_BUFF_LEN, MMIUNITCONV_EDIT1_CTRL_ID, MMIUNITCONV_MAIN_WIN_FORM1_CTRL_ID),

    CHILD_FORM_CTRL(TRUE, GUIFORM_LAYOUT_SBS, MMIUNITCONV_MAIN_WIN_FORM2_CTRL_ID, MMIUNITCONV_MAIN_WIN_FORM_CTRL_ID),
        CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT, FALSE, MMIUNITCONV_UNIT_LABEL2_CTRL_ID, MMIUNITCONV_MAIN_WIN_FORM2_CTRL_ID),
        CHILD_EDIT_TEXT_CTRL(TRUE, MMIUC_MAX_STR_BUFF_LEN, MMIUNITCONV_EDIT2_CTRL_ID, MMIUNITCONV_MAIN_WIN_FORM2_CTRL_ID),

    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, TXT_COMMON_COMMON_BACK),
    END_WIN    
};
#endif

/**---------------------------------------------------------------------------*
 **                       FUNCTION BODY 
 **---------------------------------------------------------------------------*/
/*****************************************************************************/
// 	Description :判断字符串是否合法
//	Global resource dependence : 
//  Author:renyi.hu
//	Note: 形式为整数或小数才合法
/*****************************************************************************/
LOCAL BOOLEAN InputIsValid(
							MMI_STRING_T const *in_str_ptr //in
							)
{
	int32 loop=0;
	int32 dot_count=0;

	if(PNULL==in_str_ptr)
	{
		return FALSE;
	}

	if(0==in_str_ptr->wstr_len)
	{
		return FALSE;
	}

	//第一个字符为'0'，但第2个字符不是'.'
	if(in_str_ptr->wstr_len>1)
	{
		if(MMIUC_ZERO_CHAR==in_str_ptr->wstr_ptr[0] && MMIUC_DOT_CHAR!=in_str_ptr->wstr_ptr[1])
		{
			return FALSE;
		}
	}

	//第一个字符为'.'，或者最后一个字符为'.'
	if(MMIUC_DOT_CHAR==in_str_ptr->wstr_ptr[0] || MMIUC_DOT_CHAR==in_str_ptr->wstr_ptr[in_str_ptr->wstr_len-1])
	{
		return FALSE;
	}

	for(loop=0;loop<in_str_ptr->wstr_len;loop++)
	{
		//不是'0'~'9'或者'.'
		if( (in_str_ptr->wstr_ptr[loop]<MMIUC_ZERO_CHAR || in_str_ptr->wstr_ptr[loop]>MMIUC_NINE_CHAR)
			&&MMIUC_DOT_CHAR!=in_str_ptr->wstr_ptr[loop])
		{
			return FALSE;
		}

		if(MMIUC_DOT_CHAR==in_str_ptr->wstr_ptr[loop])
		{
			dot_count++;
		}
	}

	if(dot_count>1)//小数点个数大于1
	{
		return FALSE;
	}
	
	return TRUE;
}

/*****************************************************************************/
// 	Description :判断字符串是否合法
//	Global resource dependence : 
//  Author:renyi.hu
//	Note: 形式为整数或小数才合法
/*****************************************************************************/
LOCAL BOOLEAN StrIsValid(
							MMI_STRING_T const *in_str_ptr //in
							)
{
	int32 loop=0;
	int32 dot_count=0;

	if(PNULL==in_str_ptr)
	{
		return TRUE;
	}

	if(0==in_str_ptr->wstr_len)
	{
		return TRUE;
	}

	//第一个字符为'0'，但第2个字符不是'.'
	if(in_str_ptr->wstr_len>1)
	{
		if(MMIUC_ZERO_CHAR==in_str_ptr->wstr_ptr[0] && MMIUC_DOT_CHAR!=in_str_ptr->wstr_ptr[1])
		{
			return FALSE;
		}
	}

	//第一个字符为'.'
	if(MMIUC_DOT_CHAR==in_str_ptr->wstr_ptr[0])
	{
		return FALSE;
	}

	for(loop=0;loop<in_str_ptr->wstr_len;loop++)
	{
		//不是'0'~'9'或者'.'
		if( (in_str_ptr->wstr_ptr[loop]<MMIUC_ZERO_CHAR || in_str_ptr->wstr_ptr[loop]>MMIUC_NINE_CHAR)
			&&MMIUC_DOT_CHAR!=in_str_ptr->wstr_ptr[loop])
		{
			return FALSE;
		}

		if(MMIUC_DOT_CHAR==in_str_ptr->wstr_ptr[loop])
		{
			dot_count++;
		}
	}

	if(dot_count>1)//小数点个数大于1
	{
		return FALSE;
	}
	
	return TRUE;
}

/*****************************************************************************/
//  Description : display uc type
//  Global resource dependence : none
//  Author: paul.huang
//  Note:
/*****************************************************************************/
LOCAL void DisplayUcType(GUIOWNDRAW_INFO_T *owner_draw_ptr)
{
    GUI_RECT_T    rect = {0};
    GUI_RECT_T    display_rect = {0};
    GUI_LCD_DEV_INFO    dev_info = {0};
    GUISTR_STYLE_T      text_style      = {0}; /*lint !e64*/
    GUISTR_STATE_T      state = GUISTR_STATE_ALIGN|GUISTR_STATE_WORDBREAK|GUISTR_STATE_SINGLE_LINE;  
    MMI_STRING_T        type_string = {0};
    uint16 width = 0;
    uint16 height = 0;
    MMITHEME_FORM_T    form_theme = {0};
    MMI_HANDLE_T    win_handle = 0;
    
    IGUICTRL_GetRect(MMK_GetCtrlPtr(MMIUNITCONV_OWNDRAW_TYPE_CTRL_ID), &rect);

    if(s_type_pressed)
    {
        GUIRES_DisplayImg(
                PNULL,
                &rect,
                PNULL,
                owner_draw_ptr->win_handle,
                IMAGE_CONVERTER_MENU_PRESSED,
                (const GUI_LCD_DEV_INFO*)&dev_info);
    }
    else
    {
        GUIRES_DisplayImg(
                PNULL,
                &rect,
                PNULL,
                owner_draw_ptr->win_handle,
                IMAGE_CONVERTER_MENU_DEFAULT,
                (const GUI_LCD_DEV_INFO*)&dev_info);

    }

    //画边框
    if(MMIUNITCONV_OWNDRAW_TYPE_CTRL_ID == MMK_GetActiveCtrlId(owner_draw_ptr->win_handle))  
    {
        MMITHEME_GetFormTheme(&form_theme);
        GUI_DisplayBorder(rect, rect, &form_theme.active_child.border, &dev_info);   
    }

    switch(s_uc_type)
    {
        case UC_TYPE_WEIGHT:
            MMI_GetLabelTextByLang(TXT_UC_WEIGHT, &type_string);
            break;

        case UC_TYPE_LENGTH:
            MMI_GetLabelTextByLang(TXT_UC_LENGTH, &type_string);
            break;

        default:
            break;
    }

//@leon.wang modify for QQVGA and QCIF
#ifdef MMI_UNITCONV_MINI_SUPPORT
    text_style.align = ALIGN_LVMIDDLE;
    display_rect = rect;
    display_rect.left += MMIUC_OWNDRAW_TITLE_MARGIN;
#else
    text_style.align = ALIGN_HVMIDDLE;
    display_rect = rect;
#endif
    text_style.font = MMIUC_FONT_SIZE;
    text_style.font_color = MMI_WHITE_COLOR;

    GUISTR_DrawTextToLCDInRect( 
        (const GUI_LCD_DEV_INFO *)&dev_info,
        (const GUI_RECT_T      *)&display_rect,       //the fixed display area
        (const GUI_RECT_T      *)&display_rect,       //用户要剪切的实际区域
        (const MMI_STRING_T    *)&type_string,
        &text_style,
        state,
        GUISTR_TEXT_DIR_AUTO
        );

    GUIRES_GetImgWidthHeight(&width, &height, IMAGE_CONVERTER_MENUICON_DEFAULT, owner_draw_ptr->win_handle);
    rect.top += (rect.bottom - rect.top + 1 -height)/2;
#ifdef MMI_UNITCONV_MINI_SUPPORT
    rect.left = rect.right - width;
#else
    rect.left = rect.right - width - 7;
#endif

    if(s_type_pressed)
    {
        GUIRES_DisplayImg(
                PNULL,
                &rect,
                PNULL,
                owner_draw_ptr->win_handle,
                IMAGE_CONVERTER_MENUICON_PRESSED,
                (const GUI_LCD_DEV_INFO*)&dev_info);   
    }
    else
    {
        GUIRES_DisplayImg(
                PNULL,
                &rect,
                PNULL,
                owner_draw_ptr->win_handle,
                IMAGE_CONVERTER_MENUICON_DEFAULT,
                (const GUI_LCD_DEV_INFO*)&dev_info);    
    }
    //modify for NEWM00226567
#ifdef MID_SOFTKEY_USE_IMG_SUPPORT
    GUIWIN_SetSoftkeyTextId(owner_draw_ptr->win_handle, TXT_COMMON_OK, TXT_NULL, STXT_RETURN, TRUE);
	GUIWIN_SeSoftkeytButtonIconId(owner_draw_ptr->win_handle, IMAGE_NULL, 1, TRUE);
#else
	GUIWIN_SetSoftkeyTextId(owner_draw_ptr->win_handle, TXT_COMMON_OK, TXT_NULL, STXT_RETURN, TRUE);
#endif
    win_handle = MMK_GetFocusWinHandle();
    if(MMK_IsWinIncludeTitle(win_handle))
    {
        GUIWIN_UpdateTitle(win_handle);
    }
}

/*****************************************************************************/
//  Description : handle owndraw tp down
//  Global resource dependence :
//  Author: paul.huang
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleOwnDrawTpDown(
                                MMI_HANDLE_T ctrl_handle,
                                GUI_POINT_T tp_point
                                )
{
    if(IGUICTRL_GetCtrlHandle(MMK_GetCtrlPtr(MMIUNITCONV_OWNDRAW_TYPE_CTRL_ID)) == ctrl_handle)
    {
        s_type_pressed = TRUE;
    }
#ifndef MMI_PDA_SUPPORT
    else if(IGUICTRL_GetCtrlHandle(MMK_GetCtrlPtr(MMIUNITCONV_OWNDRAW_CONVERSION_TYPE_CTRL_ID)) == ctrl_handle)
    {
        s_conversion_type_pressed = TRUE;
    }
#endif

    MMK_SendMsg(ctrl_handle, MSG_CTL_PAINT, PNULL);

    return MMI_RESULT_TRUE;
}

/*****************************************************************************/
//  Description : handle owndraw tp up
//  Global resource dependence :
//  Author: paul.huang
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleOwnDrawTpUp(
                              MMI_HANDLE_T ctrl_handle,
                              GUI_POINT_T tp_point
                              )
{
    GUI_RECT_T    rect = {0};

    if(IGUICTRL_GetCtrlHandle(MMK_GetCtrlPtr(MMIUNITCONV_OWNDRAW_TYPE_CTRL_ID)) == ctrl_handle)
    {
        s_type_pressed = FALSE;
    }
#ifndef MMI_PDA_SUPPORT
    else if(IGUICTRL_GetCtrlHandle(MMK_GetCtrlPtr(MMIUNITCONV_OWNDRAW_CONVERSION_TYPE_CTRL_ID)) == ctrl_handle)
    {
        s_conversion_type_pressed = FALSE;
    }   
#endif

    MMK_SendMsg(ctrl_handle, MSG_CTL_PAINT, PNULL);

    IGUICTRL_GetRect(MMK_GetCtrlPtr(ctrl_handle), &rect);
    
    if (GUI_PointIsInRect(tp_point, rect))
    {
        if(IGUICTRL_GetCtrlHandle(MMK_GetCtrlPtr(MMIUNITCONV_OWNDRAW_TYPE_CTRL_ID)) == ctrl_handle)
        {
            MMK_CreatePubListWin((uint32 *)UNITCONV_CHANGE_TYPE_WIN_TAB, PNULL);
        }
#ifndef MMI_PDA_SUPPORT
        else if(IGUICTRL_GetCtrlHandle(MMK_GetCtrlPtr(MMIUNITCONV_OWNDRAW_CONVERSION_TYPE_CTRL_ID)) == ctrl_handle)
        {
            MMK_CreatePubListWin((uint32 *)UNITCONV_CHANGE_CONVERSION_TYPE_WIN_TAB, PNULL);
        }    
#endif
    }

    return MMI_RESULT_TRUE;
}

/*****************************************************************************/
//  Description : handle owndraw tp move
//  Global resource dependence :
//  Author: paul.huang
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleOwnDrawTpMove(
                                MMI_HANDLE_T ctrl_handle,
                                GUI_POINT_T tp_point
                                )
{
    GUI_RECT_T    rect = {0};
    
    IGUICTRL_GetRect(MMK_GetCtrlPtr(ctrl_handle), &rect);
    
    if (!GUI_PointIsInRect(tp_point, rect))
    {
        if(IGUICTRL_GetCtrlHandle(MMK_GetCtrlPtr(MMIUNITCONV_OWNDRAW_TYPE_CTRL_ID)) == ctrl_handle)
        {
            s_type_pressed = FALSE;
        }
#ifndef MMI_PDA_SUPPORT
        else if(IGUICTRL_GetCtrlHandle(MMK_GetCtrlPtr(MMIUNITCONV_OWNDRAW_CONVERSION_TYPE_CTRL_ID)) == ctrl_handle)
        {
            s_conversion_type_pressed = FALSE;
        }   
#endif
        
        MMK_SendMsg(ctrl_handle, MSG_CTL_PAINT, PNULL);
    }

    return MMI_RESULT_TRUE;
}

/*****************************************************************************/
//  Discription:  handle change type win
//  Global resource dependence: none 
//  Author:paul.huang
//  Note:
//
/*****************************************************************************/
LOCAL MMI_RESULT_E   HandleChangeTypeWinMsg(
                                          MMI_WIN_ID_T win_id, 
                                          MMI_MESSAGE_ID_E msg_id, 
                                          DPARAM param             
                                          )
{
    MMI_RESULT_E    result = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T   ctrl_id  = MMIUNITCONV_CHANGE_TYPE_CTRL_ID;

    switch(msg_id)
    {
        case MSG_OPEN_WINDOW:        
            GUILIST_SetMaxItem(ctrl_id,UC_TYPE_MAX, FALSE );
            MMIAPISET_AppendListItemByTextId( TXT_UC_WEIGHT, TXT_COMMON_OK, ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO );
            MMIAPISET_AppendListItemByTextId( TXT_UC_LENGTH, TXT_COMMON_OK, ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO );
            GUILIST_SetSelectedItem(ctrl_id, s_uc_type, TRUE);
            GUILIST_SetCurItemIndex(ctrl_id,s_uc_type);        
            MMK_SetAtvCtrl(win_id, ctrl_id);
            break;
        
        case MSG_APP_OK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
        case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        case MSG_CTL_MIDSK:
        case MSG_CTL_OK:
        case MSG_APP_WEB:
            {
                uint16  cur_select_item = 0;
                cur_select_item = GUILIST_GetCurItemIndex(ctrl_id);
                switch (cur_select_item)
                {
                    case UC_TYPE_WEIGHT:
                        s_uc_type = UC_TYPE_WEIGHT;
                        break;
                    
                case UC_TYPE_LENGTH:
                    s_uc_type = UC_TYPE_LENGTH;
                    break;
                    
                    default:
                        break;
                }            
                MMK_CloseWin(win_id);
            }
            break;
        
        case MSG_GET_FOCUS: 
            MMK_SetAtvCtrl(win_id, ctrl_id);
            break;
        
        case MSG_APP_CANCEL:
        case MSG_CTL_CANCEL:
            MMK_CloseWin(win_id);
            break;
            
        case MSG_CLOSE_WINDOW:
            break;
            
        default:
            result = MMI_RESULT_FALSE;
            break;
    }
    return result;
}

#ifdef MMI_PDA_SUPPORT
 /*****************************************************************************/
// 	Description : 将输入的字符串经过单位换算后以字符串输出
//	Global resource dependence : 
//  Author:renyi.hu
//	Note: 
/*****************************************************************************/
LOCAL BOOLEAN  UnitConversion (
							uint16  selected1,//in
							uint16  selectde2,//in
							const MMI_STRING_T  *in_str_ptr, //in 
							MMI_STRING_T  *out_str_ptr//out
							)
{
    BOOLEAN result = FALSE;
    SCI_ASSERT(in_str_ptr!=PNULL&&out_str_ptr!=PNULL);/*assert verified*/
    SCI_ASSERT(out_str_ptr->wstr_ptr!=PNULL);/*assert verified*/

    switch(s_uc_type)
    {
        case UC_TYPE_WEIGHT:
            result = UnitConversionByRate(in_str_ptr,s_weight_conversion_rate[selected1][selectde2],out_str_ptr);  
            break;

        case UC_TYPE_LENGTH:
            result = UnitConversionByRate(in_str_ptr,s_length_conversion_rate[selected1][selectde2],out_str_ptr);  
            break;

        default:
            break;
    }

    return result;
}

/*****************************************************************************/
// 	Description : 根据换算率进行单位换算
//	Global resource dependence : 
//  Author:renyi.hu
//	Note: 
/*****************************************************************************/
 LOCAL BOOLEAN  UnitConversionByRate(
									 const MMI_STRING_T  *in_str, //in 
									 const char *conv_rate, // in 
									 MMI_STRING_T  *out_str //out
									 )
{
    double  dResult = 0;
    double    dOp1 = 0;
    double    dOp2 = 0;
    char * strStop = PNULL;
    char in_buf[MMIUC_INPUT_MAX_LEN + 1]={0};
    char out_buf[MMIUC_OUTPUT_MAX_LEN + 1]={0};
    char buf[MMIUC_OUTPUT_MAX_LEN*2 + 1]={0};

    SCI_ASSERT(in_str!=PNULL && conv_rate!=PNULL && out_str!=PNULL);/*assert verified*/
    SCI_ASSERT(out_str->wstr_ptr!=PNULL);/*assert verified*/

    MMIAPICOM_WstrToStr(in_str->wstr_ptr, (uint8*)&in_buf);

    dOp1 = strtod((char*)conv_rate, (char**)&strStop);
    dOp2 = strtod((char*)in_buf, (char**)&strStop);
    if(dOp1<1.0e-100&&dOp1>-1.0e-100)
    {
        dOp1 = 0;
    }
    if(dOp2<1.0e-100 && dOp2>-1.0e-100)
    {
        dOp2 = 0;
    }

    dResult = dOp1*dOp2;
    if(dResult<1.0e-100 && dResult>-1.0e-100)//add for CR177013
    {
        dResult = 0;   
    }
    
    if (dResult > 9.9e100||dResult < -(9.9e100))/*错误处理*/
    {
        return FALSE;					
    }
    else
    {
        if( dResult <= -1e13 || dResult >= 1e13 )
        {    
            sprintf((char*)out_buf, "%e", dResult);      
        }
        else if( dResult <= 1e-11 && dResult >= -1e-11 )     //去0和小数点的位置
        {
            if(dResult<1.0e-100 && dResult>-1.0e-100)   //0
            {
                out_buf[0] = '0';
            }
            else
            {
                sprintf((char*)out_buf, "%e", dResult);  
            }
            
        }
        else
        {     
            SCI_MEMSET(buf, 0, MMIUC_OUTPUT_MAX_LEN*2 + 1);       
            // 尽可能长的打印，去除小数点的位置
            sprintf ( (char*)buf, "%-.*f", (MMIUC_OUTPUT_MAX_LEN - 2), dResult);    
            if( buf[0] == '-')
            {
                SCI_MEMCPY((char*)out_buf, (char*)buf, MMIUC_OUTPUT_MAX_LEN + 1) ; 
            }
            else
            {
                SCI_MEMCPY((char*)out_buf, (char*)buf, MMIUC_OUTPUT_MAX_LEN) ;     
            }   
            //去除result尾部的0和‘.’
            {
                int32 i=0;
                int32 j=0;
                // 寻找‘.’
                for(i = MMIUC_OUTPUT_MAX_LEN; i >= 0 ; i--)
                {
                    if(out_buf[i] == '.')
                    {   
                        break;
                    }          
                }
                
                //找到‘.’,处理尾部的‘0’
                if(i != -1)
                {
                    for(j = MMIUC_OUTPUT_MAX_LEN; j > i ; j--)
                    {
                        if(out_buf[j] == '0'|| out_buf[j] == 0)
                        {                    
                            out_buf[j] = 0;                           
                        }       
                        else
                        {
                            break;
                        }
                    }
                }
                
                //判断是否处理‘.’
                if(i == j)
                {
                    out_buf[i] = 0;
                }
                
            }
        }   
    }

    MMIAPICOM_StrToWstr((const uint8*)out_buf, out_str->wstr_ptr);
    out_str->wstr_len=MMIAPICOM_Wstrlen(out_str->wstr_ptr);

    return TRUE;
}

LOCAL void  InitPickerContent(MMI_STRING_T* weight_string, MMI_STRING_T* height_string)
{
    uint8 i = 0;

    for(i=0; i<UC_WEIGHT_MAX; i++)
    {
        MMI_GetLabelTextByLang(s_weight_text_id[i], &weight_string[i]);
    }

    for(i=0; i<UC_LENGTH_MAX; i++)
    {
        MMI_GetLabelTextByLang(s_length_text_id[i], &height_string[i]);
    }

    return;
}

/*****************************************************************************/
//  Description : display result
//  Global resource dependence : none
//  Author: paul.huang
//  Note:
/*****************************************************************************/
LOCAL void DisplayResultString(GUIOWNDRAW_INFO_T *owner_draw_ptr)
{
    GUI_RECT_T    rect = {0};
    GUI_LCD_DEV_INFO    dev_info = {0};
    MMITHEME_FORM_T    form_theme = {0};
    GUISTR_STYLE_T      text_style      = {0}; /*lint !e64*/
    GUISTR_STATE_T      state = GUISTR_STATE_ALIGN|GUISTR_STATE_WORDBREAK|GUISTR_STATE_SINGLE_LINE;  
    MMI_STRING_T        result_string = {0};
    uint16 string_len = 0;
    
    IGUICTRL_GetRect(MMK_GetCtrlPtr(MMIUNITCONV_OWNDRAW_RESULT_CTRL_ID), &rect);
    //边框与EDIT控件一致
    MMITHEME_GetFormTheme(&form_theme);
    GUI_FillRect(&dev_info,rect,MMI_WINDOW_BACKGROUND_COLOR);
    GUI_DisplayBorder(rect, rect, &form_theme.child.border, &dev_info);   
    //与EDIT控件坐标一致
    rect.left += MMIUC_EDIT_MARGIN + form_theme.child.border.width;
    rect.right -= MMIUC_EDIT_MARGIN + form_theme.child.border.width;
    rect.top += MMIUC_EDIT_MARGIN + form_theme.child.border.width;
    rect.bottom -= MMIUC_EDIT_MARGIN + form_theme.child.border.width;

    string_len = MMIAPICOM_Wstrlen(s_result_buffer);

    if(string_len)
    {
        text_style.align = ALIGN_LVMIDDLE;
        text_style.font = MMIUC_FONT_SIZE;
        text_style.font_color = MMI_BLACK_COLOR;

        result_string.wstr_ptr = s_result_buffer;
        result_string.wstr_len = string_len;

        while(GUI_GetStringWidth(text_style.font, result_string.wstr_ptr,result_string.wstr_len) > (rect.right - rect.left))
        {           
            text_style.font-= 1;   
        } 

        GUISTR_DrawTextToLCDInRect( 
            (const GUI_LCD_DEV_INFO *)&dev_info,
            (const GUI_RECT_T      *)&rect,       //the fixed display area
            (const GUI_RECT_T      *)&rect,       //用户要剪切的实际区域
            (const MMI_STRING_T    *)&result_string,
            &text_style,
            state,
            GUISTR_TEXT_DIR_AUTO
            );    
    }
}
/*****************************************************************************/
//  Description : Display H Line
//  Global resource dependence : none
//  Author: renyi.hu
//  Note:
/*****************************************************************************/
LOCAL void DisplayLine(void)
{
    GUI_RECT_T    picker1_rect = {0};
    GUI_RECT_T    picker2_rect = {0};
    GUI_RECT_T    edit_rect = {0};
    GUI_LCD_DEV_INFO    dev_info = {0};
    uint16 x1=0;
    uint16 x2=0;
    uint16 y1=0;
    uint16 y2=0;
    //GUIFORM_ForceLayout(MMIUNITCONV_MAIN_WIN_FORM_CTRL_ID);
    IGUICTRL_GetRect(MMK_GetCtrlPtr(MMIUNITCONV_EDIT_PICKER1_CTRL_ID), &picker1_rect);
    IGUICTRL_GetRect(MMK_GetCtrlPtr(MMIUNITCONV_EDIT_PICKER2_CTRL_ID), &picker2_rect);
    IGUICTRL_GetRect(MMK_GetCtrlPtr(MMIUNITCONV_EDIT1_CTRL_ID), &edit_rect);
    x1=picker1_rect.left;
    x2=picker2_rect.right;
    y1=edit_rect.bottom+(picker1_rect.top-edit_rect.bottom)/2;
    LCD_DrawHLine(&dev_info,x1,y1,x2,MMI_DARK_GRAY_COLOR);
    x1=picker1_rect.right+1;
    y2=picker1_rect.bottom;
    LCD_DrawVLine(&dev_info,x1,y1,y2,MMI_DARK_GRAY_COLOR);
    
}

/*****************************************************************************/
// 	Description : 长度重量单位换算窗口消息处理函数
//	Global resource dependence :
//  Author:renyi.hu
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleUnitConvertWinMsg(
    MMI_WIN_ID_T         win_id,
    MMI_MESSAGE_ID_E     msg_id,
    DPARAM               param
)
{
    MMI_RESULT_E     result = MMI_RESULT_TRUE;
    wchar  dot_str[] = {'.'};
    static wchar last_valid_input_str[MMIUC_INPUT_MAX_LEN + 1];    
    MMI_STRING_T  text_label  = {0};
    char     str[2] = {0};
    wchar    wstr[2] = {0};
    uint16   wstr_length = 0;
    MMI_STRING_T  weight_string[UC_WEIGHT_MAX] = {0};
    MMI_STRING_T  height_string[UC_LENGTH_MAX] = {0};
    static UC_TYPE_E current_type = UC_TYPE_MAX;     
    GUIFORM_CHILD_WIDTH_T width = {(45), GUIFORM_CHILD_WIDTH_PERCENT};   
    GUIFORM_CHILD_HEIGHT_T height = {0};
    uint16 picker_height = 0;
    uint16 picker_item_height = 0;
    uint16 v_space = MMIUC_FORM_VSAPCE;
    uint16 v_space_h = MMIUC_FORM_HSAPCE;
    uint16 margin = MMIUC_FORM_MARGIN;
    GUI_BOTH_RECT_T client_rect = {0};
    GUI_FONT_ALL_T active_font = {0};
    GUI_FONT_ALL_T normal_font = {0};
    GUIOWNDRAW_TP_HANDLER_T     tp_handler = {0};
    uint16 picker_space_h=1;
    
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        {
            wchar           forbid_char_value[]= {0x0d, 0x0a};
            SCI_MEMSET(last_valid_input_str, 0, (MMIUC_INPUT_MAX_LEN + 1)*sizeof(wchar));
            GUIFORM_IsSlide(MMIUNITCONV_MAIN_WIN_FORM_CTRL_ID, FALSE);
            GUIFORM_SetMargin(MMIUNITCONV_MAIN_WIN_FORM_CTRL_ID,margin);  
            if(MMITHEME_IsMainScreenLandscape())
            {
                GUIFORM_SetSpace(MMIUNITCONV_MAIN_WIN_FORM_CTRL_ID, PNULL, &v_space_h);
            }
            else
            {
                GUIFORM_SetSpace(MMIUNITCONV_MAIN_WIN_FORM_CTRL_ID, PNULL, &v_space);                
            }            
            GUIFORM_PermitChildBg(MMIUNITCONV_MAIN_WIN_FORM_CTRL_ID, FALSE);
            GUIFORM_PermitChildBg(MMIUNITCONV_MAIN_WIN_FORM1_CTRL_ID, FALSE);
            
            height.type = GUIFORM_CHILD_HEIGHT_FIXED;
            height.add_data = MMIUC_OWNDRAW_HEIGHT;
            GUIFORM_SetChildHeight(MMIUNITCONV_MAIN_WIN_FORM_CTRL_ID, MMIUNITCONV_OWNDRAW_TYPE_CTRL_ID, &height);
            height.type = GUIFORM_CHILD_HEIGHT_FIXED;
            height.add_data = MMIUC_EDIT_FORM_HEIGHT;
            GUIFORM_SetChildHeight(MMIUNITCONV_MAIN_WIN_FORM_CTRL_ID, MMIUNITCONV_MAIN_WIN_FORM1_CTRL_ID, &height);
            height.type = GUIFORM_CHILD_HEIGHT_FIXED;
#ifndef MAINLCD_DEV_SIZE_160X128 
            height.add_data = MMIUC_EDIT_FORM_HEIGHT - 4;
#else
            height.add_data = MMIUC_EDIT_FORM_HEIGHT - 3;
#endif
            GUIFORM_SetChildHeight(MMIUNITCONV_MAIN_WIN_FORM1_CTRL_ID, MMIUNITCONV_EDIT1_CTRL_ID, &height);
            client_rect = MMITHEME_GetWinClientBothRect(win_id);
            if(MMITHEME_IsMainScreenLandscape())
            {
                picker_height = client_rect.h_rect.bottom - client_rect.h_rect.top +1 - 2*v_space_h - 2*margin;
                picker_item_height = MMIUC_PICKER_HEIGHT_H;
            }
            else
            {
                picker_height = client_rect.v_rect.bottom - client_rect.v_rect.top +1 - 2*v_space - 2*margin;
                picker_item_height = MMIUC_PICKER_HEIGHT_V;
            }
            picker_height -= MMIUC_OWNDRAW_HEIGHT + MMIUC_EDIT_FORM_HEIGHT;
            GUIEDIT_SetPickerHeight(MMIUNITCONV_EDIT_PICKER1_CTRL_ID, picker_height);
            GUIEDIT_SetPickerHeight(MMIUNITCONV_EDIT_PICKER2_CTRL_ID, picker_height);

            tp_handler.tp_down  = HandleOwnDrawTpDown;
            tp_handler.tp_move = HandleOwnDrawTpMove;
            tp_handler.tp_up = HandleOwnDrawTpUp;

            GUIOWNDRAW_SetCallback(MMIUNITCONV_OWNDRAW_TYPE_CTRL_ID, &tp_handler);   
        
            sprintf(str, "=");
            wstr_length = SCI_STRLEN(str);
            MMIAPICOM_StrToWstr((uint8*)str, wstr);
            text_label.wstr_ptr = wstr;
            text_label.wstr_len = wstr_length;
            GUILABEL_SetText(MMIUNITCONV_UNIT_LABEL1_CTRL_ID, &text_label, FALSE);        
            GUILABEL_SetFont(MMIUNITCONV_UNIT_LABEL1_CTRL_ID, MMIUC_FONT_SIZE, MMITHEME_GetCurThemeFontColor(MMI_THEME_LABEL_FONT));
            //set edit style
            GUIEDIT_SetStyle(MMIUNITCONV_EDIT1_CTRL_ID, GUIEDIT_STYLE_SINGLE);
            GUIEDIT_SetAlign(MMIUNITCONV_EDIT1_CTRL_ID, ALIGN_LVMIDDLE);
			
            GUIEDIT_SetIm(MMIUNITCONV_EDIT1_CTRL_ID,GUIIM_TYPE_DIGITAL,GUIIM_TYPE_DIGITAL);
            GUIEDIT_ConfigDisableChar(MMIUNITCONV_EDIT1_CTRL_ID, forbid_char_value, sizeof(forbid_char_value)/sizeof(wchar));
            GUIEDIT_ConfigImCustomKey(MMIUNITCONV_EDIT1_CTRL_ID, GUIIM_TYPE_DIGITAL, GUIIM_CUSTOM_KEY_STAR, dot_str, 1);
            GUIEDIT_SetImTag(MMIUNITCONV_EDIT1_CTRL_ID, GUIIM_TAG_DOT_NUM);
			
            GUIEDIT_SetMaxLen(MMIUNITCONV_EDIT1_CTRL_ID,MMIUC_INPUT_MAX_LEN);
            GUIEDIT_SetFont(MMIUNITCONV_EDIT1_CTRL_ID, MMIUC_FONT_SIZE);
            GUIEDIT_SetMargin(MMIUNITCONV_EDIT1_CTRL_ID, MMIUC_EDIT_MARGIN, MMIUC_EDIT_MARGIN);

            GUIFORM_SetChildWidth(MMIUNITCONV_MAIN_WIN_FORM1_CTRL_ID, MMIUNITCONV_EDIT1_CTRL_ID, &width);
            GUIFORM_SetChildWidth(MMIUNITCONV_MAIN_WIN_FORM1_CTRL_ID, MMIUNITCONV_OWNDRAW_RESULT_CTRL_ID, &width);

            current_type = UC_TYPE_WEIGHT;
            s_uc_type = UC_TYPE_WEIGHT;
            s_picker1_selected = 0;
            s_picker2_selected = 0;
            
            InitPickerContent(weight_string, height_string);
            
            GUIEDIT_SetPickerContent(MMIUNITCONV_EDIT_PICKER1_CTRL_ID, weight_string, UC_WEIGHT_MAX);
            GUIEDIT_SetPickerLoop(MMIUNITCONV_EDIT_PICKER1_CTRL_ID, TRUE);
            GUIEDIT_SetPickerLineHeight(MMIUNITCONV_EDIT_PICKER1_CTRL_ID, picker_item_height);            
            GUIEDIT_SetPickerSelectedItem(MMIUNITCONV_EDIT_PICKER1_CTRL_ID, s_picker1_selected);  

            GUIEDIT_SetPickerContent(MMIUNITCONV_EDIT_PICKER2_CTRL_ID, weight_string, UC_WEIGHT_MAX);
            GUIEDIT_SetPickerLoop(MMIUNITCONV_EDIT_PICKER2_CTRL_ID, TRUE);
            GUIEDIT_SetPickerLineHeight(MMIUNITCONV_EDIT_PICKER2_CTRL_ID, picker_item_height);            
            GUIEDIT_SetPickerSelectedItem(MMIUNITCONV_EDIT_PICKER2_CTRL_ID, s_picker2_selected);  

            normal_font.color = MMITHEME_GetCurThemeFontColor(MMI_THEME_PICKER_NORMAL);
            normal_font.font = MMIUC_FONT_SIZE;            
            active_font.color = MMITHEME_GetCurThemeFontColor(MMI_THEME_PICKER_CUR);
            active_font.font = MMIUC_FONT_SIZE;
            GUIEDIT_SetPickerFont(MMIUNITCONV_EDIT_PICKER1_CTRL_ID, &normal_font, &active_font);
            GUIEDIT_SetPickerFont(MMIUNITCONV_EDIT_PICKER2_CTRL_ID, &normal_font, &active_font);

            GUIEDIT_SetClipboardEnabled(MMIUNITCONV_EDIT1_CTRL_ID,FALSE);

            GUIFORM_SetSpace(MMIUNITCONV_MAIN_WIN_FORM2_CTRL_ID,&picker_space_h,PNULL);
            
            MMK_SetAtvCtrl(win_id, MMIUNITCONV_OWNDRAW_TYPE_CTRL_ID);

            s_type_pressed = FALSE;
            SCI_MEMSET(s_result_buffer, 0, sizeof(s_result_buffer));
        }
        break;

   case MSG_LCD_SWITCH:
       client_rect = MMITHEME_GetWinClientBothRect(win_id);
       if(MMITHEME_IsMainScreenLandscape())
       {
           picker_height = client_rect.h_rect.bottom - client_rect.h_rect.top +1 - 2*v_space_h - 2*margin;
           picker_item_height = MMIUC_PICKER_HEIGHT_H;
       }
       else
       {
           picker_height = client_rect.v_rect.bottom - client_rect.v_rect.top +1 - 2*v_space - 2*margin;
           picker_item_height = MMIUC_PICKER_HEIGHT_V;
       }
       picker_height -= MMIUC_OWNDRAW_HEIGHT + MMIUC_EDIT_FORM_HEIGHT;
       GUIEDIT_SetPickerHeight(MMIUNITCONV_EDIT_PICKER1_CTRL_ID, picker_height);
       GUIEDIT_SetPickerHeight(MMIUNITCONV_EDIT_PICKER2_CTRL_ID, picker_height);       
       GUIEDIT_SetPickerLineHeight(MMIUNITCONV_EDIT_PICKER1_CTRL_ID, picker_item_height);           
       GUIEDIT_SetPickerLineHeight(MMIUNITCONV_EDIT_PICKER2_CTRL_ID, picker_item_height);            

       if(MMITHEME_IsMainScreenLandscape())
       {
           GUIFORM_SetSpace(MMIUNITCONV_MAIN_WIN_FORM_CTRL_ID, PNULL, &v_space_h);
       }
       else
       {
           GUIFORM_SetSpace(MMIUNITCONV_MAIN_WIN_FORM_CTRL_ID, PNULL, &v_space);                
       }
       break;

   case MSG_LOSE_FOCUS:
        s_type_pressed = FALSE;
        break;
        
   case MSG_GET_FOCUS:
        if(current_type != s_uc_type)
        {
            wchar  out_buffer[1] = {0}; 
            
            current_type = s_uc_type;            
            InitPickerContent(weight_string, height_string);
            s_picker1_selected = 0;
            s_picker2_selected = 0;

            switch (s_uc_type)
            {
                case UC_TYPE_WEIGHT:
                    GUIEDIT_SetPickerContent(MMIUNITCONV_EDIT_PICKER1_CTRL_ID, weight_string, UC_WEIGHT_MAX);
                    GUIEDIT_SetPickerSelectedItem(MMIUNITCONV_EDIT_PICKER1_CTRL_ID, s_picker1_selected);  
                    GUIEDIT_SetPickerContent(MMIUNITCONV_EDIT_PICKER2_CTRL_ID, weight_string, UC_WEIGHT_MAX);
                    GUIEDIT_SetPickerSelectedItem(MMIUNITCONV_EDIT_PICKER2_CTRL_ID, s_picker2_selected);  
                    break;

                case UC_TYPE_LENGTH:
                    GUIEDIT_SetPickerContent(MMIUNITCONV_EDIT_PICKER1_CTRL_ID, height_string, UC_LENGTH_MAX);
                    GUIEDIT_SetPickerSelectedItem(MMIUNITCONV_EDIT_PICKER1_CTRL_ID, s_picker1_selected);  
                    GUIEDIT_SetPickerContent(MMIUNITCONV_EDIT_PICKER2_CTRL_ID, height_string, UC_LENGTH_MAX);
                    GUIEDIT_SetPickerSelectedItem(MMIUNITCONV_EDIT_PICKER2_CTRL_ID, s_picker2_selected);  
                    break;

                default:
                    break;
            }              
            GUIEDIT_SetString( MMIUNITCONV_EDIT1_CTRL_ID, out_buffer, 1);
            SCI_MEMSET(s_result_buffer, 0, sizeof(s_result_buffer));
        }
        break;

    case MSG_NOTIFY_EDITBOX_PICKER_SELECTED:
        {
            MMI_STRING_T in_str_ptr = {0};              
            MMI_STRING_T out_str_ptr = {0};                
            wchar        out_buffer[MMIUC_OUTPUT_MAX_LEN + 1] = {0};  
            MMI_STRING_T        text = {0};
            
            out_str_ptr.wstr_ptr = out_buffer;
            if(MMIUNITCONV_EDIT_PICKER1_CTRL_ID == ((MMI_NOTIFY_T*) param)->src_id)
            {
                s_picker1_selected = GUIEDIT_GetPickerSelectedItem(MMIUNITCONV_EDIT_PICKER1_CTRL_ID);
            } 
            else if(MMIUNITCONV_EDIT_PICKER2_CTRL_ID == ((MMI_NOTIFY_T*) param)->src_id)
            {
                s_picker2_selected = GUIEDIT_GetPickerSelectedItem(MMIUNITCONV_EDIT_PICKER2_CTRL_ID);
            }
            GUIEDIT_GetString(MMIUNITCONV_EDIT1_CTRL_ID, &in_str_ptr);
            if(0 < in_str_ptr.wstr_len)
            {
                if(UnitConversion(s_picker1_selected, s_picker2_selected, &in_str_ptr, &out_str_ptr))        
                {
                    SCI_MEMSET(s_result_buffer, 0, sizeof(s_result_buffer));
                    MMIAPICOM_Wstrncpy(s_result_buffer, out_buffer, sizeof(s_result_buffer));
                }
                else
                {
                    MMI_GetLabelTextByLang(TXT_ERROR, &text);
                    MMIAPICOM_Wstrncpy(s_result_buffer, text.wstr_ptr, sizeof(s_result_buffer));
                }
                MMK_SendMsg(win_id, MSG_FULL_PAINT,0);
            }
        }
        break;

    case MSG_NOTIFY_EDITBOX_UPDATE_STRNUM:
        {
            MMI_STRING_T in_str_ptr = {0};              
            MMI_STRING_T out_str_ptr = {0};        
            wchar        out_buffer[MMIUC_OUTPUT_MAX_LEN + 1] = {0};
            wchar        in_buffer[2] = {0}; 
            MMI_CTRL_ID_T ctrl_id = MMK_GetActiveCtrlId(win_id);
            static BOOLEAN is_recovery_last_input = FALSE;            
            MMI_STRING_T        text = {0};
            
            if(PNULL == param)
            {
                break;
            }
            if(((MMI_NOTIFY_T*)param)->src_id != ctrl_id)
            {
                break;
            }
            //if  recovering last input,do nothing
            if(is_recovery_last_input)    
            {
                is_recovery_last_input = FALSE;
                break;
            }      
            
            out_str_ptr.wstr_ptr = out_buffer;
            GUIEDIT_GetString(ctrl_id, &in_str_ptr);

            if(!StrIsValid(&in_str_ptr))
            {
                uint16 length = MMIAPICOM_Wstrlen(last_valid_input_str);
	            if((in_str_ptr.wstr_len == 2)
                 &&(MMIUC_ZERO_CHAR==in_str_ptr.wstr_ptr[0])
                 &&(in_str_ptr.wstr_ptr[1]>=MMIUC_ZERO_CHAR && in_str_ptr.wstr_ptr[1]<=MMIUC_NINE_CHAR))
	            {
		            
                    MMIAPICOM_Wstrncpy(in_buffer,in_str_ptr.wstr_ptr,in_str_ptr.wstr_len);
                    in_buffer[0] = in_str_ptr.wstr_ptr[1];
                    in_buffer[1] = 0;
                    GUIEDIT_SetString( ctrl_id, in_buffer, 1); 
                    GUIEDIT_GetString(ctrl_id, &in_str_ptr);
	            }
                else if((in_str_ptr.wstr_len == 1)
                 &&(MMIUC_DOT_CHAR==in_str_ptr.wstr_ptr[0])
                 )
	            {
                    in_buffer[0] = '0';
                    in_buffer[1] = in_str_ptr.wstr_ptr[0];
                    GUIEDIT_SetString( ctrl_id, in_buffer, 2); 
                    GUIEDIT_GetString(ctrl_id, &in_str_ptr);
	            }
                else if(length != 0)
                {
                    in_str_ptr.wstr_ptr = last_valid_input_str;
                    in_str_ptr.wstr_len = length;
                    GUIEDIT_SetString( ctrl_id, in_str_ptr.wstr_ptr, in_str_ptr.wstr_len);  
                    is_recovery_last_input = TRUE;
                }
                else if(in_str_ptr.wstr_len!=0)
                {
                    GUIEDIT_ClearAllStr(ctrl_id);
                    is_recovery_last_input = TRUE;
                }
            }
            if (InputIsValid(&in_str_ptr))
            {     
                if(!UnitConversion(s_picker1_selected, s_picker2_selected, &in_str_ptr, &out_str_ptr))        
                {
                    MMI_GetLabelTextByLang(TXT_ERROR, &text);
                    MMIAPICOM_Wstrncpy(out_buffer, text.wstr_ptr, sizeof(out_buffer));
                }
            }
            else    //invalid input, clear result
            {
                out_str_ptr.wstr_ptr[0] = 0;
                out_str_ptr.wstr_len = 1;
            }
            
            SCI_MEMSET(last_valid_input_str, 0, (MMIUC_INPUT_MAX_LEN + 1)*sizeof(wchar));
            GUIEDIT_GetString(ctrl_id, &in_str_ptr);
            if(in_str_ptr.wstr_ptr!=PNULL && in_str_ptr.wstr_len>0)
            {
                MMIAPICOM_Wstrncpy(last_valid_input_str, in_str_ptr.wstr_ptr,
                    in_str_ptr.wstr_len <= MMIUC_INPUT_MAX_LEN ? in_str_ptr.wstr_len : MMIUC_INPUT_MAX_LEN);
            }
                
            if(MMIUNITCONV_EDIT1_CTRL_ID == ctrl_id)
            {
                SCI_MEMSET(s_result_buffer, 0, sizeof(s_result_buffer));
                MMIAPICOM_Wstrncpy(s_result_buffer, out_buffer, sizeof(s_result_buffer));
            }
            else
            {}
            MMK_SendMsg(win_id, MSG_FULL_PAINT,0);
        }
        break;

    case MSG_END_FULL_PAINT:
        DisplayLine();
        break;

    case MSG_APP_OK:
    case MSG_CTL_MIDSK:
    case MSG_CTL_OK:
    case MSG_APP_WEB:
        {
            MMI_CTRL_ID_T ctrl_id = MMK_GetActiveCtrlId(win_id);
            
            if(MMIUNITCONV_OWNDRAW_TYPE_CTRL_ID == ctrl_id)
            {
                MMK_CreatePubListWin((uint32 *)UNITCONV_CHANGE_TYPE_WIN_TAB, PNULL);
            }
        }
        break;
        
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        {
            MMK_CloseWin(win_id);
        }
        break;

    case MSG_CLOSE_WINDOW:
        break;
        
    default:
        result = MMI_RESULT_FALSE;
        break;
    }

    return (result);
}

#else
 /*****************************************************************************/
 //  Description : 转换字符串到整数
 //  Global resource dependence : 
 //  Author:renyi.hu
 //  Note: 若字符串为小数形式，则扩大成整数形式，并记录
 //        下扩大的倍数
 /*****************************************************************************/
 LOCAL void  StrToInt32(
                         const MMI_STRING_T  *in_str_ptr, //in 
                         UC_STR_TO_NUM_T  *out_num  //out
                         )
 {
     wchar dot=MMIUC_DOT_CHAR;
     int16 dot_position=-1;
     int32 loop=0;
     wchar buff[MMIUC_MAX_STR_BUFF_LEN + 1]={0};
     MMI_STRING_T  str_ptr={0};
     str_ptr.wstr_ptr=buff;
 
     SCI_ASSERT(in_str_ptr!=PNULL&&out_num!=PNULL);/*assert verified*/  /*lint !e831*/
     if(!InputIsValid(in_str_ptr))
     {
         return ;
     }
 
     MMIAPICOM_CopyStr2Buf(buff,in_str_ptr,MMIUC_MAX_STR_BUFF_LEN);
     str_ptr.wstr_len=in_str_ptr->wstr_len;  /*lint !e613*/
 
     //找出小数点位置
     for(loop=0;loop<str_ptr.wstr_len;loop++)
     {
         if(str_ptr.wstr_ptr[loop]==dot)
         {
             dot_position=loop;
             break;
         }
     }
     
     if(dot_position>0)//有小数点则去掉，并记下去掉小数点后数值扩大的倍数
     {
         for(loop=dot_position+1;loop<str_ptr.wstr_len;loop++)
         {
             str_ptr.wstr_ptr[loop-1]=str_ptr.wstr_ptr[loop];
         }
         str_ptr.wstr_len-=1;
         str_ptr.wstr_ptr[str_ptr.wstr_len]=0;
         MMIAPICOM_WstrToNum(str_ptr.wstr_ptr,str_ptr.wstr_len,&out_num->num);  /*lint !e613*/
         
         for(loop=0;loop<str_ptr.wstr_len-dot_position;loop++)  
         {
             out_num->expand=out_num->expand*MMIUC_DECIMAL_NUM;  /*lint !e613*/
         }
     }
     else
     {//没有小数点则直接转成整数。扩大倍数则为1.
         MMIAPICOM_WstrToNum(str_ptr.wstr_ptr,str_ptr.wstr_len,&out_num->num);   /*lint !e613*/
         out_num->expand=1;   /*lint !e613*/
     }
 
     //SCI_TRACE_LOW:"StrToInt32:num=[%d],expand=[%d]"
     SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIUNITCONVERSION_1477_112_2_18_2_2_24_103,(uint8*)"dd",out_num->num,out_num->expand);   
 }

 /*****************************************************************************/
// 	Description : 去掉小数点后无效0
//	Global resource dependence : 
//  Author:renyi.hu
//	Note: 
/*****************************************************************************/
  LOCAL  void  DiscardStringInvalidZero(
  										MMI_STRING_T *in_str  //in & out
  										)
 {
 	int32 loop=0;
	
	if(PNULL==in_str)
	{
		return ;
	}

	//找到小数点位置
	for(loop=0;loop<in_str->wstr_len;loop++)
	{
		if(MMIUC_DOT_CHAR==in_str->wstr_ptr[loop])
		{
			break;
		}
	}

	//没有找到小数点
	if(loop==in_str->wstr_len)
	{
		return ;
	}

	//去掉最后面无效的'0'
	loop=in_str->wstr_len-1;
	while(in_str->wstr_ptr[loop]==MMIUC_ZERO_CHAR)
	{
		in_str->wstr_ptr[loop]=0;
		in_str->wstr_len--;
		loop--;
	}
	
	//如果最后的字符是'.'，则去掉。
	if(MMIUC_DOT_CHAR==in_str->wstr_ptr[loop])
	{
		in_str->wstr_ptr[loop]=0;
		in_str->wstr_len--;
	}
 }
  
/*****************************************************************************/
// 	Description : 对字符串4舍5入
//	Global resource dependence : 
//  Author:renyi.hu
//	Note: 
/*****************************************************************************/
  LOCAL  void  DiscardStringRedundance(
										  MMI_STRING_T *in_str,//in & out
										  uint32 num  // in,最多保留小数位数
										  )
  {
  	int32 loop=0;
	uint16 dot_position=0;
	wchar validChar=0;

	if(PNULL==in_str)
	{
		return ;
	}

	//找到小数点位置
	for(loop=0;loop<in_str->wstr_len;loop++)
	{
		if(MMIUC_DOT_CHAR==in_str->wstr_ptr[loop])
		{
			break;
		}
	}
	dot_position=loop;
	
	if(dot_position==in_str->wstr_len)//没有找到小数点,不用取舍
	{
		return ;
	}

	if(in_str->wstr_len-dot_position-1<=num)/*lint !e574*///小数位数小于等于num,不用取舍
	{
		return ;
	}

	validChar=in_str->wstr_ptr[dot_position+num+1];//validChar大小决定是否进位
	//SCI_TRACE_LOW:"UC:pre_validChar=[%d]"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIUNITCONVERSION_1567_112_2_18_2_2_24_104,(uint8*)"d",in_str->wstr_ptr[dot_position+num]);
	//SCI_TRACE_LOW:"UC:validChar=[%d]"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIUNITCONVERSION_1568_112_2_18_2_2_24_105,(uint8*)"d",validChar);
	
	//该条件下直接把后面的丢弃
	if(validChar<MMIUC_FIVE_CHAR&&validChar>=MMIUC_ZERO_CHAR)
	{
		in_str->wstr_ptr[dot_position+num+1]=0;
		in_str->wstr_len=dot_position+1+num;
	}
	//该条件下需要进位
	else if(validChar<=MMIUC_NINE_CHAR&&validChar>=MMIUC_FIVE_CHAR)
	{
		in_str->wstr_ptr[dot_position+num+1]=0;
		in_str->wstr_len=dot_position+1+num;
		
		for(loop=in_str->wstr_len-1;loop>=0;loop--)
		{
			if(MMIUC_DOT_CHAR != in_str->wstr_ptr[loop])
			{
				in_str->wstr_ptr[loop]+=1;
			}
			else
			{
				continue;
			}
			
			if(in_str->wstr_ptr[loop]>MMIUC_NINE_CHAR)//进为后大于9，则需减去10
			{
				in_str->wstr_ptr[loop]-=MMIUC_DECIMAL_NUM;
			}
			else
			{//小于或等于9无需向前进位了，退出循环
				break;
			}
		}
	}
	
	//最高位进位了
	if(-1==loop)
	{
		for(loop=in_str->wstr_len-1;loop>=0;loop--)
		{
			in_str->wstr_ptr[loop+1]=in_str->wstr_ptr[loop];
		}
		in_str->wstr_ptr[0]='1';
		in_str->wstr_len++;
	}

	//SCI_TRACE_LOW:"UC:lastchar=[%d]"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIUNITCONVERSION_1615_112_2_18_2_2_24_106,(uint8*)"d",in_str->wstr_ptr[in_str->wstr_len-1]);
  }

/*****************************************************************************/
// 	Description : 根据扩大倍数移动小数点位置
//	Global resource dependence : 
//  Author:renyi.hu
//	Note: 
/*****************************************************************************/
LOCAL  void  MoveDotByExpand(
								MMI_STRING_T *in_str,//in & out
								uint32 expand  //in
								)
{
	int32 dot_position=0;
	int32 count=0;
	int32 loop=0;

	if(PNULL==in_str)
	{
		return ;
	}

	//找出需要移动的位数
	while(expand>1)
	{
		expand=expand/MMIUC_DECIMAL_NUM;
		count++;
	}
	
	if(0==count)
	{
		return ;
	}

	//找出小数点位置
	for(dot_position=0;dot_position<in_str->wstr_len;dot_position++)
	{
		if(MMIUC_DOT_CHAR==in_str->wstr_ptr[dot_position])
		{
			break;
		}
	}

	//没有找到小数点则在最后添加一个。
	if(dot_position==in_str->wstr_len)
	{
		in_str->wstr_ptr[dot_position]=MMIUC_DOT_CHAR;
		in_str->wstr_len+=1;
	}
	
	
	while(count>0)
	{
		if(dot_position>1)//该条件下无需在前面补0
		{
			in_str->wstr_ptr[dot_position]=in_str->wstr_ptr[dot_position-1];
			dot_position--;
			count--;
		}
		else
		{
			break;
		}
	}
	
	if(0==count)
	{
		in_str->wstr_ptr[dot_position]=MMIUC_DOT_CHAR;
	}
	else
	{//需要在前面补0
		in_str->wstr_ptr[dot_position]=in_str->wstr_ptr[dot_position-1];
		
		for(loop=in_str->wstr_len;loop>0;loop--)
		{
			in_str->wstr_ptr[loop-1+count]=in_str->wstr_ptr[loop-1];
		}
		for(loop=0;loop<count+1;loop++)
		{
			in_str->wstr_ptr[loop]=MMIUC_ZERO_CHAR;
		}
		in_str->wstr_ptr[1]=MMIUC_DOT_CHAR;
		in_str->wstr_len+=count;
	}
}

/*****************************************************************************/
// 	Description : 转换整数结果到字符串。
//	Global resource dependence : 
//  Author:renyi.hu
//	Note: 如果整数经过放大了的，则加上小数点
/*****************************************************************************/
LOCAL void ConversionResultToString(
										UC_STR_TO_NUM_T * in_num, // in 
										MMI_STRING_T  *out_str_ptr  //out
										)
{
	uint8 str[MMIUC_MAX_STR_BUFF_LEN + 1]={0};
	uint8 str_len=0;
	int32 loop =0;
	int32 diff=0;
	int32 precision=0;
	uint32 count=1;
	uint32 decimal_count=0;
	uint32 dot_position=0;
	uint32 num=0;
	uint32 expand=1;

	//SCI_ASSERT(in_num!=PNULL && out_str_ptr!=PNULL);/*assert verified*/
	//SCI_ASSERT(out_str_ptr->wstr_ptr!=PNULL);/*assert verified*/
    if(in_num == PNULL || out_str_ptr == PNULL || out_str_ptr->wstr_ptr == PNULL)
    {
        return;
    }
    
	num=in_num->num;
	expand=in_num->expand;

	//计算出小数点后位数
	while(expand>1)
	{
		expand=expand/MMIUC_DECIMAL_NUM;
		precision++;
	}
	
	//该条件下这里先进行 4舍5入，
	//将整数后面的无效位变为0
	if(precision>MMIUC_PRECISION_NUM)
	{
		for(loop=0;loop<precision-MMIUC_PRECISION_NUM;loop++)
		{
			count=count*MMIUC_DECIMAL_NUM;
		}

		if( (num%count) >=count/2)
		{
			num=num-(num%count)+count;//进位后的数，无效位全是0了。
		}
		else
		{
			num=num-(num%count);
		}
	}

	MMIAPICOM_Int2Str(num,str,MMIUC_MAX_STR_BUFF_LEN);
	
	str_len=SCI_STRLEN((char*)str);
	diff=str_len-precision;
	
	if(diff>0)
	{//字符串长度大于小数点位数，直接在相应位置添加'.'
		for(loop=str_len;loop>str_len-precision;loop--)
		{
			str[loop]=str[loop-1];
		}
		str[loop]=MMIUC_DOT_CHAR;
		str_len=SCI_STRLEN((char*)str);
		out_str_ptr->wstr_len=str_len;
		for(loop=0;loop<str_len;loop++)
		{
			out_str_ptr->wstr_ptr[loop]=str[loop];
		}
	}
	else
	{//前面需要补0了
		diff=precision-str_len;
		out_str_ptr->wstr_ptr[0]=MMIUC_ZERO_CHAR;
		out_str_ptr->wstr_ptr[1]=MMIUC_DOT_CHAR;
		out_str_ptr->wstr_len+=2;
		for(loop=0;loop<diff;loop++)
		{
			out_str_ptr->wstr_ptr[2+loop]=MMIUC_ZERO_CHAR;
			out_str_ptr->wstr_len++;
		}

		for(loop=0;loop<str_len;loop++)
		{
			out_str_ptr->wstr_ptr[2+diff+loop]=str[loop];
			out_str_ptr->wstr_len++;
		}
	}

	//下面的代码将结果后面的0去掉；
	loop=out_str_ptr->wstr_len-1;
	//不用担心将整数后面的0也去掉了,
	//上面的转换在整数后加了'.'
	while(out_str_ptr->wstr_ptr[loop]==MMIUC_ZERO_CHAR)
	{
		out_str_ptr->wstr_ptr[loop]=0;
		out_str_ptr->wstr_len--;
		loop--;
	}
	if(MMIUC_DOT_CHAR==out_str_ptr->wstr_ptr[loop])//如果最后的字符是'.'，则去掉。
	{
		out_str_ptr->wstr_ptr[loop]=0;
		out_str_ptr->wstr_len--;
	}

	//最多显示UNITCONV_PRECISION 位小数
	for(loop=0;loop<out_str_ptr->wstr_len;loop++)
	{
		if(MMIUC_DOT_CHAR==out_str_ptr->wstr_ptr[loop])
		{
			break;
		}
		dot_position++;
	}
	
	loop++;
	for(;loop<out_str_ptr->wstr_len;loop++)
	{
		decimal_count++;
	}
	if(decimal_count>MMIUC_PRECISION_NUM)
	{
		out_str_ptr->wstr_ptr[dot_position+MMIUC_PRECISION_NUM+1]=0;
		out_str_ptr->wstr_len=out_str_ptr->wstr_len-(decimal_count-MMIUC_PRECISION_NUM);
	}
}

/*****************************************************************************/
// 	Description : 将输入的字符串经过单位换算后以字符串输出
//	Global resource dependence : 
//  Author:renyi.hu
//	Note: 
/*****************************************************************************/
LOCAL void  UnitConversion (
							UC_CONV_TYPE_E  type,//in
							const MMI_STRING_T  *in_str_ptr, //in 
							MMI_STRING_T  *out_str_ptr//out
							)
{
	UC_STR_TO_NUM_T out_num={0};
// 	uint32 conv_result=0;
	out_num.expand=1;
	
	//SCI_ASSERT(in_str_ptr!=PNULL&&out_str_ptr!=PNULL);/*assert verified*/
	//SCI_ASSERT(out_str_ptr->wstr_ptr!=PNULL);/*assert verified*/
    if(in_str_ptr == PNULL || out_str_ptr == PNULL || out_str_ptr->wstr_ptr == PNULL)
    {
        return;
    }
    
	StrToInt32(in_str_ptr,&out_num);

	UnitConversionByRate(&out_num,s_conversion_rate[type],out_str_ptr);
	
}

/*****************************************************************************/
// 	Description : 根据换算率进行单位换算
//	Global resource dependence : 
//  Author:renyi.hu
//	Note: 
/*****************************************************************************/
 LOCAL void  UnitConversionByRate(
									 UC_STR_TO_NUM_T *in_num, //in 
									 const char *conv_rate, // in 
									 MMI_STRING_T  *out_str //out
									 )
{
	int32 loop=0;
	int32 i=0;
	UC_STR_TO_NUM_T out;
	uint8 rate_len=0;
	wchar integer_buff[MMIUC_INTEGER_BUFF_LEN]={0};
	uint8 interer_len=0;
	wchar dot_buff[MMIUC_DECIMAL_BUFF_LEN]={0};
	uint8 dot_len=0;
	uint32 count=1;
	uint32 mod_array[MMIUC_MOD_BUFF_LEN]={0};
	uint8 mod_array_num=0;
	uint32 remian_1=0;
	uint32 remian_gene_1=1;
	UC_STR_TO_NUM_T result_interger={0};
	UC_STR_TO_NUM_T result_decimal={0};
	wchar result_decimal_buff[MMIUC_MAX_STR_BUFF_LEN + 1]={0};
	wchar result_interger_buff[MMIUC_MAX_STR_BUFF_LEN + 1]={0};
	MMI_STRING_T  result_decimal_str={0};
	MMI_STRING_T  result_interger_str={0};
	uint32 num=0;
	uint32 expand=1;
	

	SCI_ASSERT(in_num!=PNULL && conv_rate!=PNULL && out_str!=PNULL);/*assert verified*/
	SCI_ASSERT(out_str->wstr_ptr!=PNULL);/*assert verified*/
		
	num=in_num->num;
	expand=in_num->expand;
	rate_len=SCI_STRLEN(conv_rate);
	result_decimal_str.wstr_ptr=result_decimal_buff;
	result_decimal_str.wstr_len=0;
	result_decimal.expand=1;
	result_interger_str.wstr_ptr=result_interger_buff;
	result_interger_str.wstr_len=0;
	result_interger.expand=1;

	//将转换率的整数部分和小数部分分开
	while(conv_rate[loop]!=MMIUC_DOT_CHAR)
	{
		integer_buff[loop]=conv_rate[loop];
		loop++;
		interer_len++;
	}
	
	loop++;
	for(;loop<rate_len;loop++)
	{
		dot_buff[i]=conv_rate[loop];
		i++;
		dot_len++;
	}

	//将输入扩大较大值，注意不能越界，运算时可以提高精度
	while(num<MMIUC_MAX_EXPAND_NUM&&expand<MMIUC_MAX_EXPAND_NUM)
	{
		num=num*MMIUC_DECIMAL_NUM;
		expand=expand*MMIUC_DECIMAL_NUM;
	}


	MMIAPICOM_WstrToNum(integer_buff,interer_len,&out.num);

	out.num=out.num*num;
	out.expand=expand;

	//计算出整数部分并记录下余数部分。
	for(loop=0;loop<dot_len;loop++)
	{
		count=count*MMIUC_DECIMAL_NUM;
		out.num=out.num+num*(dot_buff[loop]-MMIUC_ZERO_CHAR)/count;
		mod_array[loop]=(num*(dot_buff[loop]-MMIUC_ZERO_CHAR))%count;
		mod_array_num++;
	}
	//剩余部分为:remian_1/remian_gene_1；
	remian_gene_1=count;
	for(loop=mod_array_num;loop>0;loop--)
	{
		remian_1+=mod_array[loop-1]*(remian_gene_1/count);
		count=count/MMIUC_DECIMAL_NUM;
	}

	result_interger.num=out.num+remian_1/remian_gene_1;//最后得到的整数
	result_interger.expand=1;
	result_decimal.num=remian_1%remian_gene_1;//最后得到的小数，不过是扩大成整数后的
	result_decimal.expand=remian_gene_1;//扩大倍数
	
	ConversionResultToString(&result_decimal,&result_decimal_str);
	ConversionResultToString(&result_interger,&result_interger_str);

	//SCI_TRACE_LOW:"UC:result_interger.num=[%d]"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIUNITCONVERSION_1958_112_2_18_2_2_25_107,(uint8*)"d",result_interger.num);
	//SCI_TRACE_LOW:"UC:result_decimal.num=[%d]"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIUNITCONVERSION_1959_112_2_18_2_2_25_108,(uint8*)"d",result_decimal.num);
	//SCI_TRACE_LOW:"UC:result_decimal.expand=[%d]"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIUNITCONVERSION_1960_112_2_18_2_2_25_109,(uint8*)"d",result_decimal.expand);

	for(loop=0;loop<result_decimal_str.wstr_len;loop++)
	{
		if(MMIUC_DOT_CHAR==result_decimal_str.wstr_ptr[loop])
		{
			break;
		}
	}
	
	//将整数和小数连接到一起
	if(loop!=result_decimal_str.wstr_len)
	{
		for(;loop<result_decimal_str.wstr_len;loop++)
		{
			result_interger_str.wstr_ptr[result_interger_str.wstr_len++]=result_decimal_str.wstr_ptr[loop];
		}
	}
	
	//还不是最后结果，result_interger_str被扩大了out.expand倍，还原之
	MoveDotByExpand(&result_interger_str,out.expand);
	
	MMIAPICOM_Wstrncpy(out_str->wstr_ptr,result_interger_str.wstr_ptr,result_interger_str.wstr_len);
	out_str->wstr_len=result_interger_str.wstr_len;
	
	DiscardStringRedundance(out_str,MMIUC_PRECISION_NUM);
	DiscardStringInvalidZero(out_str);
}

/*****************************************************************************/
// 	Description : 根据转换类型设置label 控件文本
//	Global resource dependence : 
//  Author:renyi.hu
//	Note: 
/*****************************************************************************/
  LOCAL  void SetTextLabelText(
							  UC_CONV_TYPE_E type // in 
							  )
 {
    //  	MMI_STRING_T  text_type  = {0};
    //MMI_STRING_T  text_label0  = {0};
    MMI_STRING_T  text_label1  = {0};
    MMI_STRING_T  text_label2  = {0};

    switch(type)
    {
        case MMIUC_TYPE_KG_POUND:
            {
                MMI_GetLabelTextByLang(TXT_UC_KILOGRAM, &text_label1);
                MMI_GetLabelTextByLang(TXT_UC_POUND, &text_label2);            
            }
            break;

        case MMIUC_TYPE_POUND_KG:
            {
                MMI_GetLabelTextByLang(TXT_UC_POUND, &text_label2);
                MMI_GetLabelTextByLang(TXT_UC_KILOGRAM, &text_label1);
            }
            break;
            
        case MMIUC_TYPE_KG_OUNCE:
            {            
                MMI_GetLabelTextByLang(TXT_UC_KILOGRAM, &text_label1);
                MMI_GetLabelTextByLang(TXT_UC_OUNCE, &text_label2);
            }
            break;
            
        case MMIUC_TYPE_OUNCE_KG:
            {
                MMI_GetLabelTextByLang(TXT_UC_OUNCE, &text_label2);
                MMI_GetLabelTextByLang(TXT_UC_KILOGRAM, &text_label1);
            }
            break;
            
        case MMIUC_TYPE_KM_MILE:
            {            
                MMI_GetLabelTextByLang(TXT_UC_KILOMETER, &text_label1);
                MMI_GetLabelTextByLang(TXT_UC_MILE, &text_label2);
            }
            break;
            
        case MMIUC_TYPE_MILE_KM:
            {
                MMI_GetLabelTextByLang(TXT_UC_KILOMETER, &text_label1);
                MMI_GetLabelTextByLang(TXT_UC_MILE, &text_label2);
            }
            break;
            
        case MMIUC_TYPE_M_YARD:
            {
                MMI_GetLabelTextByLang(TXT_UC_METER, &text_label1);
                MMI_GetLabelTextByLang(TXT_UC_YARD, &text_label2);
            }
            break;
            
        case MMIUC_TYPE_YARD_M:
            {
                MMI_GetLabelTextByLang(TXT_UC_METER, &text_label1);
                MMI_GetLabelTextByLang(TXT_UC_YARD, &text_label2);
            }
            break;
            
        case MMIUC_TYPE_M_FOOT:
            {
                MMI_GetLabelTextByLang(TXT_UC_METER, &text_label1);
                MMI_GetLabelTextByLang(TXT_UC_FOOT, &text_label2);
            }
            break;
            
        case MMIUC_TYPE_FOOT_M:
            {
                MMI_GetLabelTextByLang(TXT_UC_METER, &text_label1);
                MMI_GetLabelTextByLang(TXT_UC_FOOT, &text_label2);
            }
            break;
            
        case MMIUC_TYPE_CM_INCH:
            {
                MMI_GetLabelTextByLang(TXT_UC_CENTIMETER, &text_label1);
                MMI_GetLabelTextByLang(TXT_UC_INCH, &text_label2);
            }
            break;
            
        case MMIUC_TYPE_INCH_CM:
            {
                MMI_GetLabelTextByLang(TXT_UC_CENTIMETER, &text_label1);
                MMI_GetLabelTextByLang(TXT_UC_INCH, &text_label2);
            }
            break;
            
        default:
            break;
    }

    GUILABEL_SetText(MMIUNITCONV_UNIT_LABEL1_CTRL_ID, &text_label1, FALSE);
    GUILABEL_SetText(MMIUNITCONV_UNIT_LABEL2_CTRL_ID, &text_label2, FALSE);
 }

/*****************************************************************************/
//  Description : display conversion type
//  Global resource dependence : none
//  Author: paul.huang
//  Note:
/*****************************************************************************/
LOCAL void DisplayUcConversionType(GUIOWNDRAW_INFO_T *owner_draw_ptr)
{
    GUI_RECT_T    rect = {0};
    GUI_RECT_T    display_rect ={0};
    GUI_LCD_DEV_INFO    dev_info = {0};
    GUISTR_STYLE_T      text_style      = {0}; /*lint !e64*/
    GUISTR_STATE_T      state = GUISTR_STATE_ALIGN|GUISTR_STATE_WORDBREAK|GUISTR_STATE_SINGLE_LINE;  
    MMI_STRING_T        type_string = {0};
    uint16 width = 0;
    uint16 height = 0;
    MMITHEME_FORM_T    form_theme = {0};
    
    IGUICTRL_GetRect(MMK_GetCtrlPtr(MMIUNITCONV_OWNDRAW_CONVERSION_TYPE_CTRL_ID), &rect);
    
    if(s_conversion_type_pressed)
    {
        GUIRES_DisplayImg(
                PNULL,
                &rect,
                PNULL,
                owner_draw_ptr->win_handle,
                IMAGE_CONVERTER_MENU_PRESSED,
                (const GUI_LCD_DEV_INFO*)&dev_info);
    }
    else
    {
        GUIRES_DisplayImg(
                PNULL,
                &rect,
                PNULL,
                owner_draw_ptr->win_handle,
                IMAGE_CONVERTER_MENU_DEFAULT,
                (const GUI_LCD_DEV_INFO*)&dev_info);

    }

    //画边框
    if(MMIUNITCONV_OWNDRAW_CONVERSION_TYPE_CTRL_ID == MMK_GetActiveCtrlId(owner_draw_ptr->win_handle))  
    {
        MMITHEME_GetFormTheme(&form_theme);
        GUI_DisplayBorder(rect, rect, &form_theme.active_child.border, &dev_info);   
    }

    switch(s_conversion_type)
    {
        case MMIUC_TYPE_KG_POUND: 
        case MMIUC_TYPE_POUND_KG:            
            MMI_GetLabelTextByLang(TXT_UC_KILOGRAM_POUND, &type_string);
            break;
            
        case MMIUC_TYPE_KG_OUNCE:
        case MMIUC_TYPE_OUNCE_KG:            
            MMI_GetLabelTextByLang(TXT_UC_KILOGRAM_OUNCE, &type_string);
            break;

        case MMIUC_TYPE_KM_MILE:
        case MMIUC_TYPE_MILE_KM:            
            MMI_GetLabelTextByLang(TXT_UC_KILOMETER_MILE, &type_string);
            break;
            
        case MMIUC_TYPE_M_YARD:
        case MMIUC_TYPE_YARD_M:            
            MMI_GetLabelTextByLang(TXT_UC_METER_YARD, &type_string);
            break;
            
        case MMIUC_TYPE_M_FOOT:
        case MMIUC_TYPE_FOOT_M:              
            MMI_GetLabelTextByLang(TXT_UC_METER_FOOT, &type_string);
            break;
            
        case MMIUC_TYPE_CM_INCH:
        case MMIUC_TYPE_INCH_CM:            
            MMI_GetLabelTextByLang(TXT_UC_CENTIMETER_INCH, &type_string);
            break;

        default:
            break;
    }

    //@leon.wang modify for QQVGA and QCIF
#ifdef MMI_UNITCONV_MINI_SUPPORT
    text_style.align = ALIGN_LVMIDDLE;
    display_rect = rect;
    display_rect.left += MMIUC_OWNDRAW_TITLE_MARGIN;
#else
    text_style.align = ALIGN_HVMIDDLE;
    display_rect = rect;
#endif
    text_style.font = MMIUC_FONT_SIZE;
    text_style.font_color = MMI_WHITE_COLOR;

    GUISTR_DrawTextToLCDInRect( 
        (const GUI_LCD_DEV_INFO *)&dev_info,
        (const GUI_RECT_T      *)&display_rect,       //the fixed display area
        (const GUI_RECT_T      *)&display_rect,       //用户要剪切的实际区域
        (const MMI_STRING_T    *)&type_string,
        &text_style,
        state,
        GUISTR_TEXT_DIR_AUTO
        );

    GUIRES_GetImgWidthHeight(&width, &height, IMAGE_CONVERTER_MENUICON_DEFAULT, owner_draw_ptr->win_handle);
    rect.top += (rect.bottom - rect.top + 1 -height)/2;
#ifdef MMI_UNITCONV_MINI_SUPPORT
    rect.left = rect.right - width;
#else
    rect.left = rect.right - width - 7;
#endif

    if(s_conversion_type_pressed)
    {
        GUIRES_DisplayImg(
                PNULL,
                &rect,
                PNULL,
                owner_draw_ptr->win_handle,
                IMAGE_CONVERTER_MENUICON_PRESSED,
                (const GUI_LCD_DEV_INFO*)&dev_info);   
    }
    else
    {
        GUIRES_DisplayImg(
                PNULL,
                &rect,
                PNULL,
                owner_draw_ptr->win_handle,
                IMAGE_CONVERTER_MENUICON_DEFAULT,
                (const GUI_LCD_DEV_INFO*)&dev_info);    
    }
#ifdef MID_SOFTKEY_USE_IMG_SUPPORT
    GUIWIN_SetSoftkeyTextId(owner_draw_ptr->win_handle, TXT_COMMON_OK, TXT_NULL, STXT_RETURN, TRUE);
	GUIWIN_SeSoftkeytButtonIconId(owner_draw_ptr->win_handle, IMAGE_NULL, 1, TRUE);
#else
	GUIWIN_SetSoftkeyTextId(owner_draw_ptr->win_handle, TXT_COMMON_OK, TXT_NULL, STXT_RETURN, TRUE);
#endif
}

/*****************************************************************************/
//  Discription:  handle change conversion type win
//  Global resource dependence: none 
//  Author:paul.huang
//  Note:
//
/*****************************************************************************/
LOCAL MMI_RESULT_E   HandleChangeConversionTypeWinMsg(
                                          MMI_WIN_ID_T win_id, 
                                          MMI_MESSAGE_ID_E msg_id, 
                                          DPARAM param             
                                          )
{
    MMI_RESULT_E                result = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T ctrl_id  = MMIUNITCONV_CHANGE_TYPE_CTRL_ID;
    uint16        cur_select_item = 0;
    uint8    i = 0;
    uint16    pos = 0;

    switch(msg_id)
    {
        case MSG_OPEN_WINDOW:      
            switch (s_uc_type)
            {
                case UC_TYPE_WEIGHT:
                    GUILIST_SetMaxItem(ctrl_id,MMIUC_WEIGHT_TYPE_NUM, FALSE );
                    for(i = 0; i< MMIUC_WEIGHT_TYPE_NUM; i++)
                    {
                        MMIAPISET_AppendListItemByTextId( s_weight_text_id[i], TXT_COMMON_OK, ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO );
                    }
                    pos = (s_conversion_type - MMIUC_TYPE_KG_POUND)/2;/*lint !e656 */
                    GUILIST_SetSelectedItem(ctrl_id, pos, TRUE);
                    GUILIST_SetCurItemIndex(ctrl_id,pos);        
                    break;

                case UC_TYPE_LENGTH:
                    GUILIST_SetMaxItem(ctrl_id,MMIUC_LENGTH_TYPE_NUM, FALSE );
                    for(i = 0; i< MMIUC_LENGTH_TYPE_NUM; i++)
                    {
                        MMIAPISET_AppendListItemByTextId( s_length_text_id[i], TXT_COMMON_OK, ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO );
                    }
                    pos = (s_conversion_type - MMIUC_TYPE_KM_MILE)/2;/*lint !e656 */
                    GUILIST_SetSelectedItem(ctrl_id, pos, TRUE);
                    GUILIST_SetCurItemIndex(ctrl_id,pos);        
                    break;

                default:
                    break;
            }     
            MMK_SetAtvCtrl(win_id, ctrl_id);
            break;
        
        case MSG_APP_OK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
        case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        case MSG_CTL_MIDSK:
        case MSG_CTL_OK:
        case MSG_APP_WEB:
            cur_select_item = GUILIST_GetCurItemIndex(ctrl_id);

            switch (s_uc_type)
            {
                case UC_TYPE_WEIGHT:
                    s_conversion_type=MMIUC_TYPE_KG_POUND+2*cur_select_item;
                    break;

                case UC_TYPE_LENGTH:                    
                    s_conversion_type=MMIUC_TYPE_KM_MILE+2*cur_select_item;
                    break;

                default:
                    break;
            }        
            MMK_CloseWin(win_id);
            break;
        
        case MSG_GET_FOCUS: 
            MMK_SetAtvCtrl(win_id, ctrl_id);
            break;
        
        case MSG_APP_CANCEL:
        case MSG_CTL_CANCEL:
            MMK_CloseWin(win_id);
            break;
            
        case MSG_CLOSE_WINDOW:
            break;
            
        default:
            result = MMI_RESULT_FALSE;
            break;
    }
    return result;
}
/*****************************************************************************/
// 	Description : 设置单位换算显示时各种ctrl、字体等的属性-》只用于计算自适应str使用
//	Global resource dependence :
//  Author:liyan.zhu
//	Note:CR152753
/*****************************************************************************/
LOCAL MMI_RESULT_E ComputeUnitCoversionAdjustSet(
												 MMI_STRING_T    *in_str_ptr,
												 MMI_STRING_T    *out_str_ptr   //可显示下的宽度            
)
{
	GUISTR_STYLE_T  font_style = {0};
	GUISTR_STATE_T  state = GUISTR_STATE_ALIGN|GUISTR_STATE_ANGLE;
	uint16 ctrl_width = (float)(MMI_MAINSCREEN_RIGHT_MAX_PIXEL) * (1.00 - (float)MMIUC_LABEL_WIDTH_PERCENT/100.00) 
		                 - MMIUC_FORM_VSAPCE - MMIUC_FORM_MARGIN * 6 ;
	font_style.font = MMIUC_FONT_SIZE;
    return ComputAdjustStr(in_str_ptr,ctrl_width,font_style,state,out_str_ptr);
}

/*****************************************************************************/
// 	Description : 获取一个字符串显示时的自适应string
//	Global resource dependence :
//  Author:liyan.zhu
//	Note:CR152753
/*****************************************************************************/
LOCAL MMI_RESULT_E ComputAdjustStr(
										   MMI_STRING_T    *in_str_ptr,
										   uint16          ctrl_width,//string显示要小于的宽度
										   GUISTR_STYLE_T  font_style,//字体大小等描述
										   GUISTR_STATE_T  state,//字符串类型
										   MMI_STRING_T    *out_str_ptr   //可显示下的宽度            
)
{
	MMI_RESULT_E     result = MMI_RESULT_TRUE;
	uint16 str_width = GUISTR_GetStringWidth(&font_style, in_str_ptr, state);
	if (PNULL == in_str_ptr)
	{
		return MMI_RESULT_FALSE;
	}
    
	while(str_width > ctrl_width)				
	{//计算自适应str					
		in_str_ptr->wstr_len  = in_str_ptr->wstr_len - 1;
		str_width = GUISTR_GetStringWidth(&font_style, in_str_ptr, state);
	}				

	out_str_ptr = in_str_ptr;

	return result;
}
/*****************************************************************************/
// 	Description : 长度重量单位换算窗口消息处理函数
//	Global resource dependence :
//  Author:renyi.hu
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleUnitConvertWinMsg(
    MMI_WIN_ID_T         win_id,
    MMI_MESSAGE_ID_E     msg_id,
    DPARAM               param
)
{
    MMI_RESULT_E     result = MMI_RESULT_TRUE;
    wchar  dot_str[] = {'.'};
    static wchar last_valid_input_str[MMIUC_MAX_STR_BUFF_LEN + 1];
    static UC_TYPE_E current_type = UC_TYPE_MAX;
    static UC_CONV_TYPE_E current_conversion_type = MMIUC_TYPE_MAX;
    GUIFORM_CHILD_WIDTH_T width = {MMIUC_LABEL_WIDTH_PERCENT, GUIFORM_CHILD_WIDTH_PERCENT};   
    GUIFORM_CHILD_HEIGHT_T height = {0};
    uint16 v_space = MMIUC_FORM_VSAPCE;
    uint16 v_space_h = MMIUC_FORM_HSAPCE;
    uint16 margin = MMIUC_FORM_MARGIN;
    GUIOWNDRAW_TP_HANDLER_T     tp_handler = {0};
    static MMI_CTRL_ID_T  pre_active_ctrl_id = 0;
    BOOLEAN         is_disp_im_icon = FALSE;
    BOOLEAN         is_disp_num_info = FALSE;
    
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        {
            wchar           forbid_char_value[]= {0x0d, 0x0a, 0x40, 0x26,0x3f, 0x23, 0x20, 0x27, 0x2c};
            SCI_MEMSET(last_valid_input_str, 0, (MMIUC_MAX_STR_BUFF_LEN + 1)*sizeof(wchar));
                
            current_type = UC_TYPE_WEIGHT;
            s_uc_type = UC_TYPE_WEIGHT;
            current_conversion_type = MMIUC_TYPE_KG_POUND;
            s_conversion_type = MMIUC_TYPE_KG_POUND;            
            s_type_pressed = FALSE;
            s_conversion_type_pressed = FALSE;

            GUIFORM_IsSlide(MMIUNITCONV_MAIN_WIN_FORM_CTRL_ID, FALSE);
            GUIFORM_SetMargin(MMIUNITCONV_MAIN_WIN_FORM_CTRL_ID,margin);  
            if(MMITHEME_IsMainScreenLandscape())
            {
                GUIFORM_SetSpace(MMIUNITCONV_MAIN_WIN_FORM_CTRL_ID, PNULL, &v_space_h);
            }
            else
            {
                GUIFORM_SetSpace(MMIUNITCONV_MAIN_WIN_FORM_CTRL_ID, PNULL, &v_space);                
            }            
            GUIFORM_SetMargin(MMIUNITCONV_MAIN_WIN_FORM1_CTRL_ID, 0);
            GUIFORM_SetMargin(MMIUNITCONV_MAIN_WIN_FORM2_CTRL_ID, 0);
            GUIFORM_PermitChildBg(MMIUNITCONV_MAIN_WIN_FORM1_CTRL_ID, FALSE);
            GUIFORM_PermitChildBg(MMIUNITCONV_MAIN_WIN_FORM2_CTRL_ID, FALSE);
            GUIFORM_SetChildWidth( MMIUNITCONV_MAIN_WIN_FORM1_CTRL_ID,  MMIUNITCONV_UNIT_LABEL1_CTRL_ID,  &width );
            GUIFORM_SetChildWidth( MMIUNITCONV_MAIN_WIN_FORM2_CTRL_ID,  MMIUNITCONV_UNIT_LABEL2_CTRL_ID,  &width );  

            height.type = GUIFORM_CHILD_HEIGHT_FIXED;
            height.add_data = MMIUC_OWNDRAW_HEIGHT;
            GUIFORM_SetChildHeight(MMIUNITCONV_MAIN_WIN_FORM_CTRL_ID, MMIUNITCONV_OWNDRAW_TYPE_CTRL_ID, &height);
            GUIFORM_SetChildHeight(MMIUNITCONV_MAIN_WIN_FORM_CTRL_ID, MMIUNITCONV_OWNDRAW_CONVERSION_TYPE_CTRL_ID, &height);
            height.type = GUIFORM_CHILD_HEIGHT_FIXED;
            height.add_data = MMIUC_EDIT_FORM_HEIGHT;
            GUIFORM_SetChildHeight(MMIUNITCONV_MAIN_WIN_FORM_CTRL_ID, MMIUNITCONV_MAIN_WIN_FORM1_CTRL_ID, &height);
            GUIFORM_SetChildHeight(MMIUNITCONV_MAIN_WIN_FORM_CTRL_ID, MMIUNITCONV_MAIN_WIN_FORM2_CTRL_ID, &height);
            height.type = GUIFORM_CHILD_HEIGHT_FIXED;
            height.add_data = MMIUC_EDIT_FORM_HEIGHT - 4;
            GUIFORM_SetChildHeight(MMIUNITCONV_MAIN_WIN_FORM1_CTRL_ID, MMIUNITCONV_EDIT1_CTRL_ID, &height);
            GUIFORM_SetChildHeight(MMIUNITCONV_MAIN_WIN_FORM1_CTRL_ID, MMIUNITCONV_UNIT_LABEL1_CTRL_ID, &height);
            GUIFORM_SetChildHeight(MMIUNITCONV_MAIN_WIN_FORM2_CTRL_ID, MMIUNITCONV_EDIT2_CTRL_ID, &height);
            GUIFORM_SetChildHeight(MMIUNITCONV_MAIN_WIN_FORM1_CTRL_ID, MMIUNITCONV_UNIT_LABEL2_CTRL_ID, &height);

            tp_handler.tp_down  = HandleOwnDrawTpDown;
            tp_handler.tp_move = HandleOwnDrawTpMove;
            tp_handler.tp_up = HandleOwnDrawTpUp;

            GUIOWNDRAW_SetCallback(MMIUNITCONV_OWNDRAW_TYPE_CTRL_ID, &tp_handler);   
            GUIOWNDRAW_SetCallback(MMIUNITCONV_OWNDRAW_CONVERSION_TYPE_CTRL_ID, &tp_handler);   

            SetTextLabelText(s_conversion_type);
                
            //set edit style
            GUIEDIT_SetStyle(MMIUNITCONV_EDIT1_CTRL_ID, GUIEDIT_STYLE_SINGLE);
            GUIEDIT_SetAlign(MMIUNITCONV_EDIT1_CTRL_ID, ALIGN_LVMIDDLE);
            GUIEDIT_SetStyle(MMIUNITCONV_EDIT2_CTRL_ID, GUIEDIT_STYLE_SINGLE);
            GUIEDIT_SetAlign(MMIUNITCONV_EDIT2_CTRL_ID, ALIGN_LVMIDDLE);

            GUIEDIT_SetIm(MMIUNITCONV_EDIT1_CTRL_ID,GUIIM_TYPE_DIGITAL,GUIIM_TYPE_DIGITAL);
            GUIEDIT_ConfigDisableChar(MMIUNITCONV_EDIT1_CTRL_ID, forbid_char_value, sizeof(forbid_char_value)/sizeof(wchar));
            GUIEDIT_ConfigImCustomKey(MMIUNITCONV_EDIT1_CTRL_ID, GUIIM_TYPE_DIGITAL, GUIIM_CUSTOM_KEY_STAR, dot_str, 1);
            GUIEDIT_SetImTag(MMIUNITCONV_EDIT1_CTRL_ID, GUIIM_TAG_DOT_NUM);

            GUIEDIT_SetIm(MMIUNITCONV_EDIT2_CTRL_ID,GUIIM_TYPE_DIGITAL,GUIIM_TYPE_DIGITAL);
            GUIEDIT_ConfigDisableChar(MMIUNITCONV_EDIT2_CTRL_ID, forbid_char_value, sizeof(forbid_char_value)/sizeof(wchar));
            GUIEDIT_ConfigImCustomKey(MMIUNITCONV_EDIT2_CTRL_ID, GUIIM_TYPE_DIGITAL, GUIIM_CUSTOM_KEY_STAR, dot_str, 1);
            GUIEDIT_SetImTag(MMIUNITCONV_EDIT2_CTRL_ID, GUIIM_TAG_DOT_NUM);
            //modify for NEWM00226567
            GUIEDIT_SetSoftkey(MMIUNITCONV_EDIT1_CTRL_ID, 0, 1, TXT_NULL,TXT_NULL, PNULL);
            GUIEDIT_SetSoftkey(MMIUNITCONV_EDIT2_CTRL_ID, 0, 1, TXT_NULL,TXT_NULL, PNULL);
            //over
            GUIEDIT_SetMaxLen(MMIUNITCONV_EDIT1_CTRL_ID,MMIUC_INPUT_MAX_LEN);
            is_disp_im_icon = TRUE;
            is_disp_num_info = TRUE;
            GUIEDIT_SetDispImIcon(MMIUNITCONV_EDIT1_CTRL_ID,&is_disp_im_icon,&is_disp_num_info);
            GUIEDIT_SetMaxLen(MMIUNITCONV_EDIT2_CTRL_ID,MMIUC_MAX_STR_BUFF_LEN);
            is_disp_im_icon = FALSE;
            is_disp_num_info = FALSE;
            GUIEDIT_SetDispImIcon(MMIUNITCONV_EDIT2_CTRL_ID,&is_disp_im_icon,&is_disp_num_info);

            GUIEDIT_SetFont(MMIUNITCONV_EDIT1_CTRL_ID, MMIUC_FONT_SIZE);            
            GUIEDIT_SetFont(MMIUNITCONV_EDIT2_CTRL_ID, MMIUC_FONT_SIZE); 
            GUIEDIT_SetMargin(MMIUNITCONV_EDIT1_CTRL_ID, MMIUC_EDIT_MARGIN, MMIUC_EDIT_MARGIN);            
            GUIEDIT_SetMargin(MMIUNITCONV_EDIT2_CTRL_ID, MMIUC_EDIT_MARGIN, MMIUC_EDIT_MARGIN);
            GUILABEL_SetFont(MMIUNITCONV_UNIT_LABEL1_CTRL_ID, MMIUC_FONT_SIZE, MMITHEME_GetCurThemeFontColor(MMI_THEME_LABEL_FONT));
            GUILABEL_SetFont(MMIUNITCONV_UNIT_LABEL2_CTRL_ID, MMIUC_FONT_SIZE, MMITHEME_GetCurThemeFontColor(MMI_THEME_LABEL_FONT));

            GUIEDIT_SetClipboardEnabled(MMIUNITCONV_EDIT1_CTRL_ID,FALSE);
            GUIEDIT_SetClipboardEnabled(MMIUNITCONV_EDIT2_CTRL_ID,FALSE);
			GUIFORM_PermitChildFont(MMIUNITCONV_MAIN_WIN_FORM1_CTRL_ID,FALSE);
			GUIFORM_PermitChildFont(MMIUNITCONV_MAIN_WIN_FORM2_CTRL_ID,FALSE);
            MMK_SetAtvCtrl(win_id, MMIUNITCONV_OWNDRAW_TYPE_CTRL_ID);
            pre_active_ctrl_id = MMIUNITCONV_OWNDRAW_TYPE_CTRL_ID;
        }
        break;   

    case MSG_LOSE_FOCUS:
        s_type_pressed = FALSE;
        s_conversion_type_pressed = FALSE;
         break;

    case MSG_GET_FOCUS:
        if(current_type != s_uc_type)
        {
            wchar  out_buffer[1] = {0}; 

            current_type = s_uc_type;           
            
            switch (s_uc_type)
            {
                case UC_TYPE_WEIGHT:
                    current_conversion_type = MMIUC_TYPE_KG_POUND;
                    s_conversion_type = MMIUC_TYPE_KG_POUND;   
                    break;

                case UC_TYPE_LENGTH:                    
                    current_conversion_type = MMIUC_TYPE_KM_MILE;
                    s_conversion_type = MMIUC_TYPE_KM_MILE;   
                    break;

                default:
                    break;
            }   
            GUIEDIT_SetString( MMIUNITCONV_EDIT1_CTRL_ID, out_buffer, 1);             
            GUIEDIT_SetString( MMIUNITCONV_EDIT2_CTRL_ID, out_buffer, 1);
        }
        else
        {  
            if (current_conversion_type !=s_conversion_type)
            {
                MMI_STRING_T in_str_ptr = {0};
                MMI_STRING_T out_str_ptr = {0};
                wchar        out_buffer[MMIUC_MAX_STR_BUFF_LEN + 1] = {0};
                current_conversion_type = s_conversion_type;
                out_str_ptr.wstr_ptr = out_buffer;
                GUIEDIT_GetString(MMIUNITCONV_EDIT1_CTRL_ID, &in_str_ptr);
                if (0 != in_str_ptr.wstr_len)
                {
                   UnitConversion(s_conversion_type, &in_str_ptr, &out_str_ptr);
				   ComputeUnitCoversionAdjustSet(&out_str_ptr,&out_str_ptr);
                   GUIEDIT_SetString(MMIUNITCONV_EDIT2_CTRL_ID, out_str_ptr.wstr_ptr, out_str_ptr.wstr_len);
                }
                else
                {
                   GUIEDIT_SetString( MMIUNITCONV_EDIT2_CTRL_ID, out_buffer, 1);
                }  
            }
        }
        SetTextLabelText(s_conversion_type); 
        break;

    case MSG_NOTIFY_FORM_SWITCH_ACTIVE:
        {
            wchar  out_buffer[1] = {0};  
            MMI_CTRL_ID_T ctrl_id = MMK_GetActiveCtrlId(win_id);
            if(MMIUNITCONV_EDIT1_CTRL_ID == pre_active_ctrl_id 
                ||MMIUNITCONV_EDIT2_CTRL_ID == pre_active_ctrl_id
                ||MMIUNITCONV_EDIT1_CTRL_ID == ctrl_id 
                ||MMIUNITCONV_EDIT2_CTRL_ID == ctrl_id )
            {
                if(MMIUNITCONV_EDIT1_CTRL_ID == ctrl_id )
                {
                    if(1 == (s_conversion_type%2) )
                    {
                        s_conversion_type -= 1;          
                        current_conversion_type -= 1;
                    }                 
                    //clear edit box
                    GUIEDIT_SetMaxLen(MMIUNITCONV_EDIT1_CTRL_ID,MMIUC_INPUT_MAX_LEN);
                    is_disp_im_icon = TRUE;
                    is_disp_num_info = TRUE;
                    GUIEDIT_SetDispImIcon(MMIUNITCONV_EDIT1_CTRL_ID,&is_disp_im_icon,&is_disp_num_info);
                    GUIEDIT_SetMaxLen(MMIUNITCONV_EDIT2_CTRL_ID,MMIUC_MAX_STR_BUFF_LEN);
                    is_disp_im_icon = FALSE;
                    is_disp_num_info = FALSE;
                    GUIEDIT_SetDispImIcon(MMIUNITCONV_EDIT2_CTRL_ID,&is_disp_im_icon,&is_disp_num_info);
                    GUIEDIT_SetString( MMIUNITCONV_EDIT2_CTRL_ID, out_buffer, 1);                
                    GUIEDIT_SetString( MMIUNITCONV_EDIT1_CTRL_ID, out_buffer, 1);
                    SCI_MEMSET(last_valid_input_str, 0, (MMIUC_MAX_STR_BUFF_LEN + 1)*sizeof(wchar));
                    MMK_SendMsg(win_id, MSG_FULL_PAINT,0);
                }
                else if( MMIUNITCONV_EDIT2_CTRL_ID == ctrl_id)
                {
                    if(0 == (s_conversion_type%2) )
                    {
                        s_conversion_type += 1;          
                        current_conversion_type += 1;
                    }                
                    //clear edit box
                    GUIEDIT_SetMaxLen(MMIUNITCONV_EDIT2_CTRL_ID,MMIUC_INPUT_MAX_LEN);
                    is_disp_im_icon = TRUE;
                    is_disp_num_info = TRUE;
                    GUIEDIT_SetDispImIcon(MMIUNITCONV_EDIT2_CTRL_ID,&is_disp_im_icon,&is_disp_num_info);
                    GUIEDIT_SetMaxLen(MMIUNITCONV_EDIT1_CTRL_ID,MMIUC_MAX_STR_BUFF_LEN);
                    is_disp_im_icon = FALSE;
                    is_disp_num_info = FALSE;
                    GUIEDIT_SetDispImIcon(MMIUNITCONV_EDIT1_CTRL_ID,&is_disp_im_icon,&is_disp_num_info);
                    GUIEDIT_SetString( MMIUNITCONV_EDIT2_CTRL_ID, out_buffer, 1);                
                    GUIEDIT_SetString( MMIUNITCONV_EDIT1_CTRL_ID, out_buffer, 1);
                    SCI_MEMSET(last_valid_input_str, 0, (MMIUC_MAX_STR_BUFF_LEN + 1)*sizeof(wchar));                
                    MMK_SendMsg(win_id, MSG_FULL_PAINT,0);
                }
            }
            pre_active_ctrl_id = ctrl_id;
       }
       break;

   case MSG_NOTIFY_EDITBOX_UPDATE_STRNUM:
    {
        MMI_STRING_T in_str_ptr = {0};              
        MMI_STRING_T out_str_ptr = {0};        
        wchar        out_buffer[MMIUC_MAX_STR_BUFF_LEN + 1] = {0};  
        wchar        in_buffer[2] = {0}; 
        MMI_CTRL_ID_T ctrl_id = MMK_GetActiveCtrlId(win_id);
        static BOOLEAN is_recovery_last_input = FALSE;
        if(PNULL == param)
        {
            break;
        }
        if(((MMI_NOTIFY_T*)param)->src_id != ctrl_id)
        {
            break;
        }
        //if  recovering last input,do nothing
        if(is_recovery_last_input)    
        {
            is_recovery_last_input = FALSE;
            break;
        }      
        
        out_str_ptr.wstr_ptr = out_buffer;
        GUIEDIT_GetString(ctrl_id, &in_str_ptr);

        if(!StrIsValid(&in_str_ptr))
        {
            uint16 length = MMIAPICOM_Wstrlen(last_valid_input_str);
            if((in_str_ptr.wstr_len == 2)
             &&(MMIUC_ZERO_CHAR==in_str_ptr.wstr_ptr[0])
             &&(in_str_ptr.wstr_ptr[1]>=MMIUC_ZERO_CHAR && in_str_ptr.wstr_ptr[1]<=MMIUC_NINE_CHAR))
            { 
                MMIAPICOM_Wstrncpy(in_buffer,in_str_ptr.wstr_ptr,in_str_ptr.wstr_len);
                in_buffer[0] = in_str_ptr.wstr_ptr[1];
                in_buffer[1] = 0;
                GUIEDIT_SetString( ctrl_id, in_buffer, 1); 
                GUIEDIT_GetString( ctrl_id, &in_str_ptr);
	        }
            else if((in_str_ptr.wstr_len == 1)
             &&(MMIUC_DOT_CHAR==in_str_ptr.wstr_ptr[0])
             )
            {
                in_buffer[0] = '0';
                in_buffer[1] = in_str_ptr.wstr_ptr[0];
                GUIEDIT_SetString( ctrl_id, in_buffer, 2); 
                GUIEDIT_GetString(ctrl_id, &in_str_ptr);
            }
            else if(length != 0)
            {
                in_str_ptr.wstr_ptr = last_valid_input_str;
                in_str_ptr.wstr_len = length;
                GUIEDIT_SetString( ctrl_id, in_str_ptr.wstr_ptr, in_str_ptr.wstr_len);  
                is_recovery_last_input = TRUE;
            }
            else if(in_str_ptr.wstr_len!=0)
            {
                GUIEDIT_ClearAllStr(ctrl_id);
                is_recovery_last_input = TRUE;
            }
        }
        if (InputIsValid(&in_str_ptr))
        {                  
            UnitConversion(s_conversion_type, &in_str_ptr, &out_str_ptr);
        }
        else    //invalid input, clear result
        {
            out_str_ptr.wstr_ptr[0] = 0;
            out_str_ptr.wstr_len = 1;
        }
        
        SCI_MEMSET(last_valid_input_str, 0, (MMIUC_MAX_STR_BUFF_LEN + 1)*sizeof(wchar));
        GUIEDIT_GetString(ctrl_id, &in_str_ptr);
        if(in_str_ptr.wstr_ptr!=PNULL && in_str_ptr.wstr_len>0)
        {
            MMIAPICOM_Wstrncpy(last_valid_input_str, in_str_ptr.wstr_ptr,
                in_str_ptr.wstr_len <= MMIUC_MAX_STR_BUFF_LEN ? in_str_ptr.wstr_len : MMIUC_MAX_STR_BUFF_LEN);
        }
            
        if(MMIUNITCONV_EDIT1_CTRL_ID == ctrl_id)
        {
			ComputeUnitCoversionAdjustSet(&out_str_ptr,&out_str_ptr);
            GUIEDIT_SetString( MMIUNITCONV_EDIT2_CTRL_ID, out_str_ptr.wstr_ptr, out_str_ptr.wstr_len);                
        }
        else if(MMIUNITCONV_EDIT2_CTRL_ID == ctrl_id)
        {
			ComputeUnitCoversionAdjustSet(&out_str_ptr,&out_str_ptr);
            GUIEDIT_SetString( MMIUNITCONV_EDIT1_CTRL_ID, out_str_ptr.wstr_ptr, out_str_ptr.wstr_len);
        }
        else
        {}
        MMK_SendMsg(win_id, MSG_FULL_PAINT,0);
    }
    break;

    case MSG_FULL_PAINT:
        break;

    case MSG_APP_OK:
    case MSG_CTL_MIDSK:
    case MSG_CTL_OK:
    case MSG_APP_WEB:
        {
            MMI_CTRL_ID_T ctrl_id = MMK_GetActiveCtrlId(win_id);
            
            if(MMIUNITCONV_OWNDRAW_TYPE_CTRL_ID == ctrl_id)
            {
                MMK_CreatePubListWin((uint32 *)UNITCONV_CHANGE_TYPE_WIN_TAB, PNULL);
            }
            else if(MMIUNITCONV_OWNDRAW_CONVERSION_TYPE_CTRL_ID == ctrl_id)
            {
                MMK_CreatePubListWin((uint32 *)UNITCONV_CHANGE_CONVERSION_TYPE_WIN_TAB, PNULL);
            }
        }
        break;
        
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        {
            MMK_CloseWin(win_id);
        }
        break;

    case MSG_CLOSE_WINDOW:
        break;
        
    default:
        result = MMI_RESULT_FALSE;
        break;
    }

    return (result);
}
#endif

/*****************************************************************************/
// 	Description : the entry of unit conversion
//	Global resource dependence : 
//  Author:renyi.hu
//	Note: 
/*****************************************************************************/
PUBLIC void MMIAPIUNITCONV_OpenMainWin(void)
{
    MMK_CreateWin((uint32*)UNITCONV_MIAN_WIN_TAB, PNULL);
}
#endif //CUSTOM_UNITCONVERSION_SUPPORT
#else
/*****************************************************************************/
// 	Description : the entry of unit conversion
//	Global resource dependence : 
//  Author:renyi.hu
//	Note: 
/*****************************************************************************/
 PUBLIC void MMIAPIUNITCONV_OpenMainWin(void)
 {
 }
#endif//MMI_UNITCONVERSION_SUPPORT
 
 /*****************************************************************************/
 // 	Description : insert dyna menu node by label
 //	Global resource dependence : 
 //  Author:liqing.peng
 //	Note:
 /*****************************************************************************/
 LOCAL void InsertConverterMenuNodeByLabel(
                                           MMI_CTRL_ID_T    ctrl_id,        //控件id
                                           MMI_TEXT_ID_T   	text_id,        //插入的节点的文本
                                           MMI_IMAGE_ID_T   img_id,
                                           uint16          	node_id,        //节点编号
                                           uint16          	parent_node_id, //父节点编号
                                           uint16          	index           //位置
                                           )
 {
     MMI_STRING_T            kstring = {0};
     GUIMENU_DYNA_ITEM_T     node_item = {0};
     
     MMI_GetLabelTextByLang(text_id, &kstring);
     node_item.item_text_ptr = &kstring;
#ifdef MMI_PDA_SUPPORT
     if(img_id != 0)
     {
         node_item.select_icon_id = img_id;
     }
#endif
     GUIMENU_InsertNode(index,node_id,parent_node_id,&node_item,ctrl_id);
 }

/*****************************************************************************/
//    Description : Converter list option window
//    Global resource dependence : 
//    Author: Cheney.Wu
//    Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleConverterListOptionWinMsg(
                                                   MMI_WIN_ID_T        win_id, 
                                                   MMI_MESSAGE_ID_E    msg_id, 
                                                   DPARAM              param
                                                   )
{
    uint16                              i                   = 0;
    uint16                              node_id             = 0;
    MMI_RESULT_E                        ret_val             = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T                       ctrl_id             = MMIUNITCONV_CONVERTER_OPT_LIST_CTRL_ID;
    uint16                              index               = 0;
    MMI_MENU_ID_T   menu_id = 0;
    MMI_CTRL_ID_T   mainMenuCtrId = MMIUNITCONV_MAIN_WIN_MENU_LIST_CTRL_ID;
    
    menu_id = GUILIST_GetCurItemIndex(mainMenuCtrId);
    
    switch( msg_id )
    {
    case MSG_OPEN_WINDOW:
        GUIMENU_CreatDynamic(PNULL,win_id,ctrl_id,GUIMENU_STYLE_POPUP);
        
        i=0;
#ifdef MMI_PDA_SUPPORT
        InsertConverterMenuNodeByLabel(ctrl_id, STR_LIST_NEW_CONVERTER, 0, CONVERTER_OPT_NEW, 0, i++);
        InsertConverterMenuNodeByLabel(ctrl_id, STR_LIST_DELETE, 0, CONVERTER_OPT_REMOVE, 0, i++);
#else
        InsertConverterMenuNodeByLabel(ctrl_id, STR_LIST_NEW_CONVERTER, 0, CONVERTER_OPT_NEW, 0, i++);
        InsertConverterMenuNodeByLabel(ctrl_id, STR_LIST_DELETE, 0,CONVERTER_OPT_REMOVE, 0, i++);
#endif

        if(menu_id < UC_CONV_MAIN_CUSTOMIZE_1)
        {
            GUIMENU_SetNodeVisible(ctrl_id, 1, 0, FALSE);
        }
        
        GUIMENU_SetDynamicMenuSoftkey(ctrl_id, TXT_NULL, STXT_SOFTKEY_SELECT_MK, TXT_COMMON_COMMON_BACK);
        MMK_SetAtvCtrl(win_id, ctrl_id);
        break;
        
    case MSG_APP_WEB:
    case MSG_CTL_MIDSK:
        node_id = GUIMENU_GetCurNodeId(ctrl_id);
        switch( node_id )
        {
        case CONVERTER_OPT_NEW:
            UnitConv_converterWin(UC_CONV_CUSTOMIZE_MAX_NUM, (UC_CONV_SUBSET_TYPE)UC_CONV_SUBSET_CUSTOMIZE_NEW);
            MMK_CloseWin(win_id);
            break;
            
        case CONVERTER_OPT_REMOVE:
            if(menu_id >= UC_CONV_MAIN_CUSTOMIZE_1
                && menu_id < UC_CONV_MAIN_TYPE_NUM)
            {
                UC_CONV_CUSTOMIZE_ITEM_T*   custItem = PNULL;
                
                custItem = &custConvItem.convItem[menu_id - UC_CONV_MAIN_CUSTOMIZE_1];
                SCI_MEMSET(custItem, 0, sizeof(UC_CONV_CUSTOMIZE_ITEM_T));
                custItem->convEnable = FALSE;
                custConvItem.convNumber--;
                
                customItemMakeOrder();
                MMI_WriteNVItem(MMINV_ACC_UNITCONV_CUSTOMIZE_ITEM, (void *)&custConvItem);
                UnitConv_SetMainTypeList(TRUE);
                MMK_SendMsg(win_id, MSG_FULL_PAINT,0);
            }
            MMK_CloseWin(win_id);
            break;

        default:
            break;
        }
        break;

    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        break;

    case MSG_CLOSE_WINDOW:
       break;

    default:
        ret_val = MMI_RESULT_FALSE;
        break;
    }

    return ret_val;
}

/*Edit by script, ignore 2 case. Fri Apr 27 09:38:48 2012*/ //IGNORE9527
