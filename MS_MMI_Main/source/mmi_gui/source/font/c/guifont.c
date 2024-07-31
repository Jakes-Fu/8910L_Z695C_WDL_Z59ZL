/*****************************************************************************
** File Name:      guifont.c                                                 *
** Author:                                                                   *
** Date:           21/05/2004                                                *
** Copyright:      2004 Spreadtrum, Incoporated. All Rights Reserved.         *
** Description:    This file provide functions about font liabrary and       *
**                  display string                                           *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 21/05/2004       Jim.Zhang           Create                               *
******************************************************************************/

#ifndef _GUI_FONT_C_
#define _GUI_FONT_C_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmi_gui_trc.h"
#include "sci_types.h"
#include "guilcd.h"
#include "guifont.h"
#include "guicommon.h"
#include "mmi_theme.h"
#include "mmi_textfun.h"
#include "mmi_resource_def.h"
#include "guiblock.h"
#include "gui_unicode.h"
#include "mmi_base_common.h"

#include "spml_api.h"

/**---------------------------------------------------------------------------*
 **                         Functions                                         *
 **---------------------------------------------------------------------------*/

/*******************************************************************************************/
// 	Description : 获取文字宽度
//	Global resource dependence :
//  Author: James.Zhang
//	Note: 
/*********************************************************************************************/
uint8 GUI_GetFontWidth(//
                        GUI_FONT_T	font_type,      //字库类型
                        uint16 unicode
                        )
{
    //尽量少使用该函数, 多国语言下计算会错误
    return GUI_GetStringWidthEx( font_type, &unicode, 1, 0 );
}


/*******************************************************************************************/
// 	Description : 获取光标对齐方式
//	Global resource dependence :
//  Author: James.Zhang
//	Note: 光标只支持水平方向的对齐
/*********************************************************************************************/
LOCAL SPML_TEXT_FORMAT_T ConvertCursorAlign(
                                            GUI_ALIGN_E       align
                                            )
{
    SPML_TEXT_FORMAT_T format = 0;

    switch(align)
    {
    case ALIGN_LEFT:                                    // Align in left
        format |= TEXT_FORMAT_LEFT;
        break;
    case ALIGN_RIGHT:                                   // Align in right
        format |= TEXT_FORMAT_RIGHT;
        break;
    case ALIGN_HMIDDLE:                                 // Align in mediacy
        format |= TEXT_FORMAT_CENTER;
        break;
    default:
        break;
    }

    return format;
}

#ifndef SCRIPTEASY_SUPPORT
/*******************************************************************************************/
// 	Description : 获得光标的x坐标
//	Global resource dependence :
//  Author: James.Zhang
//	Note: 
/*********************************************************************************************/
PUBLIC int16 GUI_GetCusorCoordinate(//绝对坐标
                                    GUI_ALIGN_E       align,
                                    const GUI_RECT_T* rect_ptr,
                                    int8              dir,
                                    const wchar*      str_ptr,
                                    uint16            str_len,
                                    uint16            cursor_pos,
                                    GUI_FONT_T	      font_type,
                                    uint16            char_space
                                    )
{
    int16 x = 0;
    SPML_TEXT_PARAM_T param = {0};
    MMISET_LANGUAGE_TYPE_E   language_type = (MMISET_LANGUAGE_TYPE_E)MMISET_GetCurrentLanguageType();
    MMI_STRING_T string = {0};
    wchar  *str_buf = PNULL;
    uint16 buf_len = 0;

	if (PNULL == rect_ptr)
	{
        //SCI_TRACE_LOW:"== GUI_GetCusorCoordinate == param rect_ptr is PNULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,GUIFONT_96_112_2_18_3_17_1_147,(uint8*)"");
        return x;
	}

    if (PNULL == str_ptr || 0 == str_len )
    {
        if (ALIGN_HMIDDLE == align)
        {
            return (rect_ptr->left + rect_ptr->right) / 2;
        }
        else if (MMI_DIRECTION_RIGHT == MMITHEME_GetDisDirection()
                    || ALIGN_RIGHT == align
                    || ALIGN_RVMIDDLE == align)
        {
            return (rect_ptr->right);
        }
        else
        {
            return (rect_ptr->left);
        }
    }

    param.format |= ConvertCursorAlign( align );
    param.display_rect = param.clip_rect = *rect_ptr;
    param.glyph_space = char_space;
    param.dir  = dir;
    param.size = GUI_ConvertGUIFontToSize( font_type );

    if ( TEXT_DIR_RTL == param.dir )
    {
        param.format |= TEXT_FORMAT_RIGHT;
    }
//    param.format |= TEXT_FORMAT_BOLD;

    string.wstr_len = str_len;
    string.wstr_ptr = str_ptr;

    //当显示语言为arabic\urdu\farsi时，在显示时统一将所有的数字0123456789
    //按照与native digit的offset 0x0630做个转换，这部分需要从输入法移到显示中统一处理
    if(/*(MMISET_LANGUAGE_URDU == language_type) 
        ||*/ (MMISET_LANGUAGE_PERSIAN == language_type)
        || (MMISET_LANGUAGE_ARABIC == language_type))
    {
        buf_len = string.wstr_len;
        str_buf = SCI_ALLOC_APP(buf_len * sizeof(wchar));
        if(PNULL != str_buf)
        {
            SCI_MEMSET(str_buf, 0, buf_len * sizeof(wchar));
            if(GUI_ConvertDisplayString(&string, str_buf, buf_len,language_type))
            {
                string.wstr_len = buf_len;
                string.wstr_ptr = str_buf;
            }
        }
    }

    x = (int16)SPMLAPI_GetCursorCoordinate( &param, string.wstr_ptr, str_len, cursor_pos );

    if (PNULL != str_buf)
    {
        SCI_FREE(str_buf);
        str_buf = PNULL;
    }

    return x;
}

/*******************************************************************************************/
// 	Description : 获得光标的x坐标并微调光标位置
//	Global resource dependence :
//  Author: James.Zhang
//	Note: 
/*********************************************************************************************/
PUBLIC int16 GUI_GetCursorInfo(
                               GUI_ALIGN_E       align,
                               const GUI_RECT_T	*rect_ptr,
                               int8				 dir,
                               const wchar		*str_ptr,
                               uint16            str_len,
                               BOOLEAN           is_left,
                               GUI_FONT_T	     font_type,
                               uint16            char_space,
                               int32*            cursor_ptr
                               )
{
    int16 x = 0;
    SPML_TEXT_PARAM_T param = {0};
    
	if (PNULL == rect_ptr)
	{
        //SCI_TRACE_LOW:"== GUI_GetCursorInfo == param rect_ptr is PNULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,GUIFONT_149_112_2_18_3_17_1_148,(uint8*)"");
        return x;
	}

    if (PNULL == str_ptr || 0 == str_len )
    {
        return (rect_ptr->left);
    }
    
    param.format |= ConvertCursorAlign( align );
    param.display_rect = param.clip_rect = *rect_ptr;
    param.glyph_space = char_space;
    param.dir  = dir;
    param.size = GUI_ConvertGUIFontToSize( font_type );
    
    if ( TEXT_DIR_RTL == param.dir )
    {
        param.format |= TEXT_FORMAT_RIGHT;
    }
    
     x = (int16)SPMLAPI_GetCursorInfo( &param, str_ptr, str_len, is_left, cursor_ptr );

    return x;
}

/*******************************************************************************************/
// 	Description : 获得光标索引
//	Global resource dependence :
//  Author: James.Zhang
//	Note: 
/*********************************************************************************************/
PUBLIC uint16 GUI_GetCursorPos(//相对位置
                               GUI_ALIGN_E      align,
							   const GUI_RECT_T	*rect_ptr,      //in:
							   int8				dir,            //in:
							   const wchar		*str_ptr,       //in:
							   uint16           str_len,        //in:
							   uint16           cursor_pos,	    //in:相对位置
							   int16            cursor_x,	    //in:绝对坐标
							   GUI_FONT_T	    font_type,      //in:
							   uint16           char_space,     //in:
                               BOOLEAN          *is_find_ptr    //in/out:may PNULL
							   )
{
    int32               pos = 0;
    SPML_TEXT_PARAM_T   param = {0};

    if ( PNULL == rect_ptr || PNULL == str_ptr || 0 == str_len )
    {
        if (PNULL != is_find_ptr)
        {
            *is_find_ptr = FALSE;
        }

        return 0;
    }

    param.format |= ConvertCursorAlign( align );
    param.display_rect = param.clip_rect = *rect_ptr;
    param.glyph_space = char_space;
    param.dir  = dir;
    param.size = GUI_ConvertGUIFontToSize( font_type );

    if ( TEXT_DIR_RTL == param.dir )
    {
        param.format |= TEXT_FORMAT_RIGHT;
    }

    pos = SPMLAPI_GetCursorPos( &param, str_ptr, str_len, cursor_pos, cursor_x,is_find_ptr);

    if ( pos >= 0 )
    {
        cursor_pos =(uint16)pos;
    }

    return cursor_pos;
}



/*==============================================================================
Description: 获得光标索引
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note: 增强型，同时获得光标与字符的相对位置关系
==============================================================================*/
PUBLIC BOOLEAN GUI_GetCursorPosEx(
                                  GUI_ALIGN_E      align,
                                  const GUI_RECT_T *rect_ptr,
                                  int8         dir,
                                  const wchar  *str_ptr,
                                  uint16       str_len,
                                  int16        cursor_x,
                                  GUI_FONT_T   font_type,
                                  uint16       char_space,
                                  int32        *nearest_chr_idx_ptr,
                                  int32        *under_chr_idx_ptr,
                                  int32        *under_len_ptr
                                  )
{
    BOOLEAN is_find = FALSE;
    SPML_TEXT_PARAM_T param = {0};

    if (PNULL == nearest_chr_idx_ptr
        || PNULL == under_chr_idx_ptr
        || PNULL == under_len_ptr)
    {
        return FALSE;
    }

    if (PNULL == rect_ptr || PNULL == str_ptr || 0 == str_len)
    {
        *nearest_chr_idx_ptr = -1;
        return is_find;
    }

    param.format |= ConvertCursorAlign(align);
    param.display_rect = param.clip_rect = *rect_ptr;
    param.glyph_space = char_space;
    param.dir  = dir;
    param.size = GUI_ConvertGUIFontToSize(font_type);

    if ( TEXT_DIR_RTL == param.dir )
    {
        param.format |= TEXT_FORMAT_RIGHT;
    }

    is_find = SPMLAPI_GetCursorPosEx(&param,
                                     str_ptr,
                                     str_len,
                                     cursor_x,
                                     nearest_chr_idx_ptr,
                                     under_chr_idx_ptr,
                                     under_len_ptr);

    return is_find;
}

#endif /* SCRIPTEASY_SUPPORT */

//增加缓冲区进行优化
#define FONT_CACHE_MASK 0xFF
#define FONT_CACHE_SIZE (FONT_CACHE_MASK + 1)

typedef struct
{
    uint16 ch;
    uint8  ch_width;
    uint8  font_size;
} GUI_FONT_CACHE_T;

LOCAL GUI_FONT_CACHE_T s_han_font_cache = {0};
LOCAL GUI_FONT_CACHE_T s_other_font_cache[FONT_CACHE_SIZE] = {0};



/*****************************************************************************/
//  Description : 
//  当显示语言为arabic\urdu\farsi时，在显示时统一将所有的数字0123456789
//  按照与native digit的offset 0x0630做个转换，这部分需要从输入法移到显示中统一处理
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN GUI_ConvertDisplayString(const MMI_STRING_T *str_ptr, wchar *dest_buf, uint16 buf_len,uint8 language_type)
{
    int i = 0;
    BOOLEAN b_ret = FALSE;
    //MMISET_LANGUAGE_TYPE_E    language_type = MMISET_MAX_LANGUAGE;

    if(PNULL != str_ptr 
        && PNULL != str_ptr->wstr_ptr 
        && PNULL != dest_buf 
        && str_ptr->wstr_len <= buf_len)
    {
        //当显示语言为arabic\urdu\farsi时，在显示时统一将所有的数字0123456789
        /*
        1)当显示语言为arabic按照与native digit的offset 0x0630做个转换，这部分需要从输入法移到显示中统一处理
        2)当语言为urdu\farsi，offset 0x06c0做个转换
        */
        if(MMISET_LANGUAGE_ARABIC == (MMISET_LANGUAGE_TYPE_E)language_type)
        {
            for(i = 0; i < str_ptr->wstr_len; i++)
            {
                if((str_ptr->wstr_ptr[i] >= '0') && (str_ptr->wstr_ptr[i] <= '9'))
                {
                    /*Arabic number1234 change to Arabic indic number--for MS*/
                    dest_buf[i] = str_ptr->wstr_ptr[i] + 0x0630;
                }
                else
                {
                    dest_buf[i] = str_ptr->wstr_ptr[i];
                }
            }
        }
        else if((MMISET_LANGUAGE_URDU ==(MMISET_LANGUAGE_TYPE_E)language_type)||(MMISET_LANGUAGE_PERSIAN ==(MMISET_LANGUAGE_TYPE_E) language_type))
            {
                for(i = 0; i < str_ptr->wstr_len; i++)
                {
                    if((str_ptr->wstr_ptr[i] >= '0') && (str_ptr->wstr_ptr[i] <= '9'))
                    {
                        /*Urdu/Faris number1234 --for MS*/
                        dest_buf[i] = str_ptr->wstr_ptr[i] + 0x06c0;
                    }
                    else
                    {
                        dest_buf[i] = str_ptr->wstr_ptr[i];
                    }
                }
            }
        

        b_ret = TRUE;

    }

    return b_ret;
}


#ifndef SCRIPTEASY_SUPPORT
/*******************************************************************************************/
// 	Description : 获得字符串的宽度
//	Global resource dependence :
//  Author: James.Zhang
//	Note: 字形间距可理解为之前的字间距, 但不允许用户自己计算, 必须通过该函数
/*********************************************************************************************/
PUBLIC uint16 GUI_GetStringWidthEx(
                                   GUI_FONT_T	font_type,     
                                   const wchar* str_ptr,
                                   uint16 str_len,
                                   uint16 char_space
                                   )
{
    SPML_TEXT_METRICS_T metrics = {0};
    SPML_TEXT_PARAM_T param = {0};
    
    MMISET_LANGUAGE_TYPE_E   language_type = (MMISET_LANGUAGE_TYPE_E)MMISET_GetCurrentLanguageType();
    MMI_STRING_T string = {0};
    wchar  *str_buf = PNULL;
    uint16 buf_len = 0;
    uint16 string_width = 0;

    if ( str_ptr == PNULL || str_len == 0 ) 
    {
        return 0;
    }
    
    string.wstr_len = str_len;
    string.wstr_ptr = str_ptr;

    do
    {
        //当显示语言为arabic\urdu\farsi时，在显示时统一将所有的数字0123456789
        //按照与native digit的offset 0x0630做个转换，这部分需要从输入法移到显示中统一处理
        if(/*(MMISET_LANGUAGE_URDU == language_type) 
            ||*/ (MMISET_LANGUAGE_PERSIAN == language_type)
            || (MMISET_LANGUAGE_ARABIC == language_type))
        {
            buf_len = string.wstr_len;
            str_buf = SCI_ALLOC_APP(buf_len * sizeof(wchar));
            if(PNULL != str_buf)
            {
                SCI_MEMSET(str_buf, 0, buf_len * sizeof(wchar));
                if(GUI_ConvertDisplayString(&string, str_buf, buf_len,language_type))
                {
                    string.wstr_len = buf_len;
                    string.wstr_ptr = str_buf;
                }
            }
        }

    param.glyph_space = char_space;
    param.dir = TEXT_DIR_AUTO;
    param.size = GUI_ConvertGUIFontToSize( font_type );
    
    if ( 1 == str_len )
    {
            wchar ch = string.wstr_ptr[0];
        GUI_FONT_CACHE_T* cache_ptr = PNULL;
        
        //汉字宽高都一样
        if (  (ch >= 0x2e80 && ch <= 0x9fff)
            || (ch >= 0xf900 && ch <= 0xfaff)
            || (ch >= 0xff00 && ch <= 0xffef))
        {
            ch = UNICODE_HANZI;
            
            cache_ptr = &s_han_font_cache;
        }
        else
        {
            cache_ptr = &s_other_font_cache[ch & FONT_CACHE_MASK];
        }
        
        if ( cache_ptr->ch == ch
            && cache_ptr->font_size == param.size
            && 0 != ch )
        {
                string_width = cache_ptr->ch_width;
                break;
        }
        else
        {
            SPMLAPI_GetTextMetrics( &param, &ch, str_len, &metrics );
            
            cache_ptr->ch        = ch;
            cache_ptr->font_size = param.size;
            cache_ptr->ch_width  = metrics.width;
            
                string_width =  metrics.width;
                break;
        }
    }
    else
    {
            SPMLAPI_GetTextMetrics( &param, string.wstr_ptr, str_len, &metrics );
            
            string_width = metrics.width;
            break;
        }
        
    }while(0);
        
    if (PNULL != str_buf)
    {
        SCI_FREE(str_buf);
        str_buf = PNULL;
    }

    return string_width;
}
#endif /* SCRIPTEASY_SUPPORT */

/*******************************************************************************************/
// 	Description : 获得字符串的宽度
//	Global resource dependence :
//  Author: yongwei.he
//	Note:   
/*********************************************************************************************/
PUBLIC uint16 GUI_GetStringWidth(
                                 GUI_FONT_T	font_type,     
                                 const wchar* str_ptr,
                                 uint16 str_len
                                 )

{
#ifdef SCRIPTEASY_SUPPORT
    return GUI_GetStringWidthEx( font_type, str_ptr, str_len, 0 );
#else	
    //存在连字和叠加文字时, 必须精确计算, 否则可以逐字进行估算
#ifdef COMPLEX_TEXT_SCRIPT
    return GUI_GetStringWidthEx( font_type, str_ptr, str_len, 0 );
#else

    uint16  w_char = 0;
    uint16  str_width = 0;
    uint32  i = 0;
    uint8   char_width = 0;
    
    if((PNULL == str_ptr) || (0 == str_len) || (font_type >= GUI_MAX_FONT_NUM))
    {
        return 0;
    }

    for( ; i < str_len; i++ )
    {
        w_char = str_ptr[i];

        //james, \0返回
        if( 0 == w_char )
        {
            break;
        }
        
        char_width = GUI_GetFontWidth( font_type, w_char );
        
        str_width += char_width;
    }
    
    return str_width;
#endif
#endif /* SCRIPTEASY_SUPPORT */
}

/*****************************************************************************/
//  Description : get font height by font type
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC uint16 GUIFONT_GetHeight(
                                GUI_FONT_T  font_type
                                )
{
    return GUI_ConvertGUIFontToSize( font_type );
}

/*******************************************************************************************/
// 	Description : 获得字体的高度
//	Global resource dependence :
//  Author: panda.luo
//	Note:   2007-09-27 replace second param is_ucs2 to unicode by yongwei.he
/*********************************************************************************************/
uint8 GUI_GetFontHeight(                        //返回指定字体的高度
                        GUI_FONT_T	font_type,   //字库类型
                        uint16 unicode
                        )
{
    //尽量少使用该函数, 多国语言下计算会错误
    return GUI_GetStringHeight( font_type, &unicode, 1 );
}

/*******************************************************************************************/
// 	Description : 获得字符串的高度
//	Global resource dependence :
//  Author: yongwei.he
//	Note:   字符串的高度为所有字符的最大高度
/*********************************************************************************************/
#ifndef SCRIPTEASY_SUPPORT
PUBLIC uint16 GUI_GetStringHeight(
                                  GUI_FONT_T   font_type,     
                                  const wchar* str_ptr,
                                  uint16       str_len
                                  )
{
//    SPML_TEXT_METRICS_T metrics = {0};
    SPML_TEXT_PARAM_T param = {0};
    
    if ( str_ptr == 0 || str_len == 0 ) 
    {
        return 0;
    }

    param.dir = TEXT_DIR_AUTO;
    param.size = GUI_ConvertGUIFontToSize( font_type );

    return param.size;
    
    //优化多语显示, 删除精确计算, 可以通过调整字体库让多语字体稍小一点
    //存在连字和叠加文字时, 必须精确计算, 否则可以逐字进行估算
// #ifdef COMPLEX_TEXT_SCRIPT
// 
//     SPMLAPI_GetTextMetrics( &param, str_ptr, str_len, &metrics);
//     
//     return metrics.height;
//     
// #else
// 
//     return param.size;
// 
// #endif
}
#endif /* #ifndef SCRIPTEASY_SUPPORT */

/*******************************************************************************************/
// 	Description : 获得字符串的高度扩展
//	Global resource dependence :
//  Author: zhewei.tang
//	Note:   字符串的高度为所有字符的最大高度
/*********************************************************************************************/
PUBLIC uint16 GUI_GetStringHeightEx(
                                  GUI_FONT_T   font_type,     
                                  const wchar* str_ptr,
                                  uint16       str_len
                                  )
{
    SPML_TEXT_METRICS_T metrics = {0};
    SPML_TEXT_PARAM_T param = {0};
    
    if ( str_ptr == PNULL || str_len == 0 ) 
    {
        return 0;
    }
    
    param.dir = TEXT_DIR_AUTO;
    param.size = GUI_ConvertGUIFontToSize( font_type );    

    SPMLAPI_GetTextMetrics( &param, str_ptr, str_len, &metrics );
    
    return metrics.height;

}

/*******************************************************************************************/
// 	Description : whether is the new line char
//	Global resource dependence :
//  Author: Great.Tian
//	Note:
/*********************************************************************************************/
PUBLIC BOOLEAN GUI_IsNewlineChar(
                                 uint16 char_code //[IN] the char code, 用uint16可以同时支持ASCII和UINCODE
                                 )
{
    // SCI_TRACE_LOW("guifont.c: enter function GUI_IsNewlineChar()");

    if (CR_CHAR == char_code || LF_CHAR == char_code)
    {
        return (TRUE);
    }
    else
    {
        return (FALSE);
    }
}

/*******************************************************************************************/
// 	Description :
//	Global resource dependence :
//  Author: 
//	Note:
/*********************************************************************************************/
uint16 GUI_ConvertGUIFontToSize( GUI_FONT_T font )
{    
    uint16 font_size = GUI_MAX_FONT_NUM - 1;
    
    if ( font < font_size )
    {
        font_size = font;
    }

    return font_size;
}

/*******************************************************************************************/
// 	Description :
//	Global resource dependence :
//  Author: 
//	Note:
/*********************************************************************************************/
void GUI_ReplaceChar(
                     MMI_STRING_T *src_str_ptr, 
                     uint16 src_char,
                     uint16 des_char
                     )
{    
    uint16 i = 0;
    wchar  *temp_str = PNULL;
    uint16 sum_len = 0;

    if (PNULL != src_str_ptr)
    {
        sum_len = src_str_ptr->wstr_len;
        temp_str = src_str_ptr->wstr_ptr;
        
        for ( i = 0; i < sum_len; i++ )
        {
            if ( src_char == *temp_str)
            {
                *temp_str = des_char; //替换为空格
            }
            
            temp_str++;
        }

    }

    return;
}

/*******************************************************************************************/
// 	Description :
//	Global resource dependence :
//  Author: 
//	Note:
/*********************************************************************************************/
GUI_FONT_T GUI_ConvertSizeToGUIFont( uint16 size )
{
    GUI_FONT_T font_size = GUI_MAX_FONT_NUM - 1;
    
    if ( size < font_size )
    {
        font_size = (GUI_FONT_T)size;
    }

    return font_size;
}

#ifdef ZDT_EDIT_NUM_IMG_SUPPORT
// added by bao T101023 for MTK Font
typedef struct _MTK_FONT_INDEX_t
{
	uint16       unicode;   // 字符的unicode 
	uint8        width;     // 字符的宽度
	uint8        height;    // 字符的高度
	uint32       offset;  // 字符的指针
	uint32        img_id;
}MTK_FONT_INDEX_T;

typedef struct RangeData_t
{
    uint32 nMin;
    uint32 nMax;
}RangeData;

typedef struct RangeInfo
{
    uint16  nNoOfRanges;
    const RangeData *pRangeData;

} RangeDetails;

typedef struct _CustFontData
{
    uint8 nHeight;
    uint8 nWidth;
    uint8 nEquiDistant;
    uint8 nCharBytes;
    uint16 nMaxChars;
    uint8 *pWidthArray;
    uint32 *pOffsetArray;
    uint8 *pDataArray;
    uint32 *pRange;
    const RangeDetails *pRangeDetails;
} sCustFontData;

#define	WR_TO_LCD(lcd_ptr, x, y, width, color)	\
{\
		*( (lcd_ptr) + ( (y)*(width) + (x) ) ) = (color) ;\
}

#define	WR_TO_LCD_WO(lcd_ptr, x, y, color)	\
{\
		*( (lcd_ptr) + ( (y) + (x) ) ) = (color) ;\
}

#define CheckPixelBase(codedata_ptr, offset, offset_bit)	(BOOLEAN)(codedata_ptr[offset] & offset_bit)

#define WRITE_FONT_1IN8_PIXEL(codedata8bit, bit_mask, lcd_buffer_ptr, index, color)  \
            {if (codedata8bit & bit_mask) lcd_buffer_ptr[index] = color;}

#define WRITE_FONT_8_PIXEL(codedata8bit, lcd_buffer_ptr, color)  \
    {   \
        WRITE_FONT_1IN8_PIXEL(codedata8bit, 0x80, lcd_buffer_ptr, 0, color);   \
        WRITE_FONT_1IN8_PIXEL(codedata8bit, 0x40, lcd_buffer_ptr, 1, color);   \
        WRITE_FONT_1IN8_PIXEL(codedata8bit, 0x20, lcd_buffer_ptr, 2, color);   \
        WRITE_FONT_1IN8_PIXEL(codedata8bit, 0x10, lcd_buffer_ptr, 3, color);   \
        WRITE_FONT_1IN8_PIXEL(codedata8bit, 0x08, lcd_buffer_ptr, 4, color);   \
        WRITE_FONT_1IN8_PIXEL(codedata8bit, 0x04, lcd_buffer_ptr, 5, color);   \
        WRITE_FONT_1IN8_PIXEL(codedata8bit, 0x02, lcd_buffer_ptr, 6, color);   \
        WRITE_FONT_1IN8_PIXEL(codedata8bit, 0x01, lcd_buffer_ptr, 7, color);   \
    }


/*angle function */
typedef struct gui_font_angle_func_tag
{
    void (*DrawTOLCD)( const struct gui_font_angle_func_tag* pFunc, GUIFONT_EFFECT_T effect, GUI_COLOR_T *lcd_buffer_ptr, uint16 lcdwidth, uint16 lcdheight, int16 x0, int16 y0, GUI_RECT_T rect, uint8 fontwidth, uint8 fontheight, uint8 *codedata_ptr, GUI_COLOR_T color );
    void (*GetFontPixelPos)( int16 *x_ptr, int16 *y_ptr, int16 x0, int16 y0, uint16 i, uint16 j );
    void (*MapPixel)( int16 *x_ptr, int16 *y_ptr, int16 i, int16 j, GUI_RECT_T box );
    void (*MapPixelReverse)( int16 *i_ptr, int16 *j_ptr, uint16 *width_ptr, uint16 *height_ptr, int16 x, int16 y, GUI_RECT_T box );
    void (*NextFontPos)( int16 *x_ptr, int16 *y_ptr, uint8 space, uint8 fontwidth );

}GUI_FONT_ANGLE_FUNC_T;
/*lint -e785*/

/*****************************************************************************/
// 	Description : readjust rectangle
//	Global resource dependence : none
//  Author: xiaoqing.lu
//	Note: 
/*****************************************************************************/
LOCAL GUI_RECT_T GetTrimRect(
                             GUI_RECT_T rect1, 
                             GUI_RECT_T rect2
                             )
{
    GUI_RECT_T trim_rect = {0};

    trim_rect.left = MAX(rect1.left, rect2.left);
    trim_rect.top = MAX(rect1.top, rect2.top);
    trim_rect.right = MIN(rect1.right, rect2.right);
    trim_rect.bottom = MIN(rect1.bottom, rect2.bottom);

    return (trim_rect);
}

LOCAL GUI_RECT_T ReadjustBox(
                             GUI_RECT_T rect, 
                             uint16 lcdwidth, 
                             uint16 lcdheight
                             )
{
    GUI_RECT_T box = {0};

    // 调整显示区域
    if( rect.left < 0 )
    {
        rect.left = 0;
    }
    if( rect.right < 0 )
    {
        rect.right = 0;
    }
    if( rect.top < 0 )
    {
        rect.top = 0;
    }
    if( rect.bottom < 0 )
    {
        rect.bottom = 0;
    }
    box.left = (int16)((rect.left >= lcdwidth ) ? lcdwidth-1 : rect.left);
    box.right = (int16)((rect.right >= lcdwidth ) ? lcdwidth-1 : rect.right);
    box.top = (int16)((rect.top >= lcdheight ) ? lcdheight-1 : rect.top);
    box.bottom = (int16)((rect.bottom >= lcdheight ) ? lcdheight-1 : rect.bottom);

    return box;
}

LOCAL BOOLEAN GetFontPos(
                         const GUI_FONT_ANGLE_FUNC_T* pFunc,
                         int16 *x_ptr,
                         int16 *y_ptr,
                         uint16 char_unicode,
                         GUI_RECT_T box,
                         uint8 fontwidth,
                         uint8 fontheight,
                         uint8 line_space
                         )
{
    int16 i = 0, j = 0;
    uint16 width = 0, height = 0;

	SCI_ASSERT(x_ptr != PNULL && y_ptr != PNULL);
    
    pFunc->MapPixelReverse( &i, &j, &width, &height, *x_ptr, *y_ptr, box );
    
	// A ASCII char data.
    if (GUI_IsLatinUnicode(char_unicode))
    {
        if (CR_CHAR == char_unicode)    //回车符
        {
            i = 0;
            j += fontheight + line_space;
            pFunc->MapPixel( x_ptr, y_ptr, i, j, box );
            return TRUE;
        }
        else if(LF_CHAR == char_unicode)    //换行符
        {
            i = 0;
            j += fontheight + line_space;
            pFunc->MapPixel( x_ptr, y_ptr, i, j, box );
            return TRUE;
        }      
    }

    //忽略特殊处理
    if ((i+fontwidth) > width)
    {	
        i = 0;
        j += fontheight + line_space;
        pFunc->MapPixel( x_ptr, y_ptr, i, j, box );
    }
    return FALSE;
}

/*****************************************************************************/
// 	Description : Get the start point in font library for a char.
//	Global resource dependence : none
//  Author: Jim.zhang
//	Note: return the index in font library   
/*****************************************************************************/
LOCAL uint32 GetFontIndex(
						 uint16 char_unicode,			//text to display
						 GUI_FONT_T  font		//font pointer	
						 )
{
    uint8  gb_array[2] = {0};
    //uint8  ucs_array[2] = {0};
    uint32 data_index = 0;

    if (GUI_IsLatinUnicode(char_unicode))
    {
        data_index = GUI_GetLatinFontIndex(char_unicode);
    }
    else//if(GUI_IsCJKUnicode(char_unicode))
    {
        //ucs_array[0] = (uint8)((char_unicode & 0xFF00)>>8);
       // ucs_array[1] = (uint8)(char_unicode & 0x00FF);

        GUI_WstrToGB((uint8 *)gb_array, &char_unicode, 1);

        data_index = (gb_array[0] - 0xA1) * 0x5E + gb_array[1] - 0xA1;
    }

    return ( data_index );
}

LOCAL int16 GetImgYPosition(
                          GUI_RECT_T  rect,         // the fixed display area
                          const wchar *str_ptr,     // string to display
                          uint16      len,          // length of string to display
                          uint16      char_space,   // space between two chars.
                          uint16  img_width,	        // font type
                          GUI_ALIGN_E align,        // align type
                          uint16      line_space,    // space between 2 lines
                          BOOLEAN     is_single_line // 
                          )
{
    int16 y = 0; // 一定要支持负区域
    uint16 line_num = 0;
    uint16 line_num_max = 0;
    uint16 str_height = 0;
    uint16 font_height = 0;
    uint16 height = 0;

	SCI_ASSERT(str_ptr != PNULL);

    font_height = MMITHEME_PHONE_IMG_HEIGHT;
    
    if (ALIGN_TOP == align 
        || ALIGN_LEFT == align 
        || ALIGN_RIGHT == align 
        || ALIGN_HMIDDLE == align 
        || (int16)font_height >= (rect.bottom - rect.top + 1))
    {
        y = rect.top;
    }
    else if (is_single_line)
    {
        str_height = font_height;
        switch (align)
        {
        case ALIGN_BOTTOM:
            y = (rect.bottom - (int16)str_height + 1);
            break;
            
        case ALIGN_VMIDDLE:
        case ALIGN_LVMIDDLE:
        case ALIGN_RVMIDDLE:
        case ALIGN_HVMIDDLE:
            y = ((rect.bottom + rect.top - (int16)str_height + 1)>>1);
            break;
        default:
            y = rect.top;
            break;
        }   
    }
    else
    {
        // 多行显示并且align是其他的状态时
        line_num = GUI_CalculatePhImgLinesByPixelNum((uint16)(rect.right-rect.left+1),
            str_ptr,
            len,
            MMITHEME_PHONE_IMG_WIDTH,
            (uint8)char_space,
            TRUE
            );

        height = font_height + line_space;
        if (0 != height)
        {
            line_num_max = (uint16)((rect.bottom - rect.top + 1 + (int16)line_space)/(int16)height);
        }
        else
        {
            SCI_TRACE_LOW("[GetImgYPosition guistring.c] len = %d, str_ptr = %x", len, str_ptr);
        }
        
        if (line_num > line_num_max)
        {
            line_num = line_num_max;
        }

        str_height = (uint16)(line_num * (font_height+line_space) - line_space);

        if ( (int16)str_height < (rect.bottom - rect.top + 1))
        {
            switch (align)
            {
            case ALIGN_BOTTOM:
                y = (rect.bottom - (int16)str_height + 1);
                break;
                
            case ALIGN_VMIDDLE:
            case ALIGN_LVMIDDLE:
            case ALIGN_RVMIDDLE:
            case ALIGN_HVMIDDLE:
                y = ((rect.bottom + rect.top - (int16)str_height + 1)>>1);
                break;
            default:
                y = rect.top;
                break;
            }    
        }
        else
        {
            y = rect.top;
        }
    }

    return y; 
}

LOCAL uint16 GetImgXPosition(
                    GUI_RECT_T  rect, 		    //the fixed display area
					GUI_ALIGN_E align,
                    uint16      pixel_num
                    )
{
    uint16 x = 0;
  
    if (pixel_num < (rect.right - rect.left))
    {
        switch (align)
        {
        case ALIGN_RIGHT:
        case ALIGN_RVMIDDLE:
            x = rect.right - pixel_num + 1;
            break;

        case ALIGN_HMIDDLE:
        case ALIGN_HVMIDDLE:
            x = (rect.right + rect.left - pixel_num + 1)>>1;
            break;

        default:
            x = rect.left;
            break;
        }    
    }
    else
    {
        x = rect.left;
    }

    return x;
}

/*****************************************************************************/
// 	Description : 检查是否超出边界
//	Global resource dependence : none
//  Author: Bin.ji
//	Note: 
/*****************************************************************************/
LOCAL GUI_RECT_T AdjustCHRectByAngle(
                                     GUI_RECT_T box,
                                     GUI_ANGLE_E angle,
                                     uint16 x1, 
                                     uint16 y1, 
                                     uint16 ch_height
                                     )
{
    GUI_RECT_T actual_rect = {0};

    if (0 == ch_height)
    {
        return actual_rect;
    }

    //calc display rect
	if (ANGLE_0 == angle)
	{
		actual_rect = box;
		actual_rect.bottom = y1 + ch_height - 1;
		if (y1 == box.top)
		{
			actual_rect.right = x1 - 1;
            actual_rect.right = MAX(actual_rect.left, actual_rect.right);
		}
	}
	else if (ANGLE_180 == angle)
	{
		actual_rect = box;
		actual_rect.top = y1 - ch_height + 1;
		if (y1 == box.bottom)
		{
			actual_rect.left = x1 + 1;
		}
	}
	else if (ANGLE_90 == angle)
	{
		actual_rect = box;
		actual_rect.left = x1 - ch_height + 1;
		if (x1 == box.right)
		{
			actual_rect.bottom = y1 - 1;
            actual_rect.bottom = MAX(actual_rect.top, actual_rect.bottom);
		}
	}
	else if (ANGLE_270 == angle)
	{
		actual_rect = box;
		actual_rect.right = x1 + ch_height - 1;
		if (x1 == box.left)
		{
			actual_rect.top = y1 + 1;
		}
	}

    return actual_rect;
}

LOCAL void GetFontPixelPos(
                           GUI_ANGLE_E  angle,  //旋转角度
                           uint16       *x_ptr, //out, 计算得到的位置
                           uint16       *y_ptr, //out, 计算得到的位置
                           uint16       x0,     //原点坐标
                           uint16       y0,     //原点坐标
                           uint16       i,      //相对坐标
                           uint16       j       //相对坐标
                           )
{
    switch (angle)
    {
    case ANGLE_0:
        *x_ptr = x0 + i;
        *y_ptr = y0 + j;
        break;
        
    case ANGLE_90:
        *x_ptr = x0 - j;
        *y_ptr = y0 + i;
        break;

    case ANGLE_180:
        *x_ptr = x0 - i;
        *y_ptr = y0 - j;
        break;

    case ANGLE_270:
    default:
        *x_ptr = x0 + j;
        *y_ptr = y0 - i;
        break;
    }    
}

LOCAL void DrawMTkTOLCD(
                     GUIFONT_EFFECT_T effect,        //特殊效果
                     GUI_COLOR_T *lcd_buffer_ptr,    //显示buffer
                     uint16 lcdwidth,                //lcd宽度
                     uint16 lcdheight,               //lcd高度
                     GUI_ANGLE_E angle,              //旋转角度
                     uint16 x0,                      //字符左上角x坐标
                     uint16 y0,                      //字符左上角y坐标
                     uint8 fontwidth,                //字符宽度
                     uint8 fontheight,               //字符高度
                     uint8 *codedata_ptr,            //字符数据
                     GUI_COLOR_T color               //颜色
                     )
{
    uint16 	i, j;		
    uint8 *code_ptr;												
    uint16 x = x0, y=y0 ;
    uint32 temp  = 0;
    uint32 ytemp = 0;
    BOOLEAN result = FALSE;

    code_ptr = codedata_ptr;

    if (FONT_EFFECT_REVERSE_EDGE == effect)
    {
        //边缘反色显示效果
       for (j = 0; j<(fontheight * fontwidth ); j++)
       {
            temp = j/8;
            result = CheckPixelBase(codedata_ptr,temp,(0x01<<((j%8)&0x07)));

                if (result)
                {                
                    GetFontPixelPos(angle, &x, &y, x0, y0, j%fontwidth, j/fontwidth);
                    ytemp = y*lcdwidth;

                    if (0 < x)
                    {
                        WR_TO_LCD_WO(lcd_buffer_ptr, (x - 1), ytemp,0xEFBE /*~color*/);	
                    }
                    if ((lcdwidth - 1) > x)
                    {
                        WR_TO_LCD_WO(lcd_buffer_ptr, (x + 1), ytemp, 0xEFBE /*~color*/);
                    }
                    if (0 < y)
                    {
                        WR_TO_LCD_WO(lcd_buffer_ptr, x, ytemp - lcdwidth, 0xEFBE /*~color*/);	
                    }
                    if ((lcdheight - 1) > y)
                    {
                        WR_TO_LCD_WO(lcd_buffer_ptr, x, ytemp + lcdwidth, 0xEFBE /*~color*/);
                    }
                }
        }															
    }
    
   for (j = 0; j<(fontheight * fontwidth ); j++)
   {
        temp = j/8;
      
        result = CheckPixelBase(codedata_ptr,temp,(0x01<<((j%8)&0x07)));
        if (result)
        {
            GetFontPixelPos(angle, &x, &y, x0, y0, j%fontwidth, j/fontwidth);
            WR_TO_LCD(lcd_buffer_ptr, x, y, lcdwidth, color);
        }
        
    }		
}

LOCAL BOOLEAN CheckFontPos(
                           const GUI_FONT_ANGLE_FUNC_T* pFunc,
                           int16 x,
                           int16 y,
                           GUI_RECT_T box,
                           BOOLEAN spchar_result,
                           uint16 fontwidth,
                           uint16 fontheight
                           )
{
    int16 i = 0, j = 0;
    uint16 width = 0, height = 0;

    if ((x < box.left) || (x > box.right) || (y < box.top) || (y > box.bottom))
    {
        return TRUE;
    }
    
    if (!spchar_result)
    {    
        pFunc->MapPixelReverse( &i, &j, &width, &height, x, y, box );

        if ((i + fontwidth) > width || (j + fontheight) > height)
            return TRUE;
    }
    return FALSE;
}

LOCAL void DrawTOBuf(
					 const GUI_FONT_ANGLE_FUNC_T* pFunc,
                     GUIFONT_EFFECT_T effect,        //特殊效果
                     GUI_COLOR_T *lcd_buffer_ptr,    //显示buffer
                     uint16 lcdwidth,                //lcd宽度
                     uint16 lcdheight,               //lcd高度
                     int16 x0,                      //字符左上角x坐标
                     int16 y0,                      //字符左上角y坐标
                     GUI_RECT_T rect,
                     uint8 fontwidth,                //字符宽度
                     uint8 fontheight,               //字符高度
                     uint8 *codedata_ptr,            //字符数据
                     GUI_COLOR_T color,               //颜色
                     GUI_COLOR_T  edge_color
                     )
{
	uint32 		i = 0, j = 0;		
	uint8		*code_ptr = codedata_ptr;
	int16		x = x0, y = y0;
	int32		temp  = 0;
	int32		ytemp = 0;
    GUI_POINT_T point = {0};

	
	SCI_ASSERT(lcd_buffer_ptr != PNULL && codedata_ptr != PNULL);

	code_ptr = codedata_ptr;
	
	if(FONT_EFFECT_NONE != effect)
	{
		if(FONT_EFFECT_REVERSE_EDGE == effect)
		{
			edge_color = (GUI_COLOR_T)(~color);
		}
		else if(FONT_EFFECT_WIHTE_EDGE == effect)
		{
			edge_color = 0xFFFF;
		}
		else if(FONT_EFFECT_CUSTOM == effect)
		{
			edge_color = edge_color;
		}
		else
		{
			SCI_PASSERT(FALSE,("DrawTOLCDAngle0: effect %d is error!",effect));
		}

        //边缘显示效果
        for (j = 0; j<(fontheight * fontwidth ); j++) 											
	{		
            temp = j/8;
            if (CheckPixelBase (codedata_ptr, temp, (0x01<<((j%8)&0x07))))
            {                
                pFunc->GetFontPixelPos( &x, &y, x0, y0, j%fontwidth, j/fontwidth);
                point.x = x;
                point.y = y;
                if (GUI_PointIsInRect(point, rect))
                {
                    if (0 < x)
                        WR_TO_LCD(lcd_buffer_ptr, (x - 1), y, lcdwidth, (GUI_COLOR_T)(edge_color));	
                    if ((lcdwidth - 1) > x)
                        WR_TO_LCD(lcd_buffer_ptr, (x + 1), y, lcdwidth, (GUI_COLOR_T)(edge_color));
                    if (0 < y)
                        WR_TO_LCD(lcd_buffer_ptr, x, y - 1, lcdwidth, (GUI_COLOR_T)(edge_color));	
                    if ((lcdheight - 1) > y)
                        WR_TO_LCD(lcd_buffer_ptr, x, y + 1, lcdwidth, (GUI_COLOR_T)(edge_color));
                }
            }
        }
    }

    /* Draw it ..*/		
    for (j = 0; j<(fontheight * fontwidth ); j++) 											
    {		
        temp = j/8;
        if (CheckPixelBase (codedata_ptr, temp, (0x01<<((j%8)&0x07)))) 												
        {		
            pFunc->GetFontPixelPos( &x, &y, x0, y0, j%fontwidth, j/fontwidth);
            point.x = x;
            point.y = y;
            if (GUI_PointIsInRect(point, rect))
            {
                WR_TO_LCD(lcd_buffer_ptr, x, y, lcdwidth, color);
            }
        }																																	
    }
}

/*******************************************************************************************/
// 	Description : 显示字符
//	Global resource dependence :
//  Author: bin.ji
//	Note:
/*********************************************************************************************/
LOCAL void DrawTOLCDAngle0(
                           const GUI_FONT_ANGLE_FUNC_T* pFunc,
                           GUIFONT_EFFECT_T effect,        //特殊效果
                           GUI_COLOR_T *lcd_buffer_ptr,    //显示buffer
                           uint16 lcdwidth,                //lcd宽度
                           uint16 lcdheight,               //lcd高度
                           int16 x0,                      //字符左上角x坐标
                           int16 y0,                      //字符左上角y坐标
                           GUI_RECT_T rect,
                           uint8 fontwidth,                //字符宽度
                           uint8 fontheight,               //字符高度
                           uint8 *codedata_ptr,            //字符数据
                           GUI_COLOR_T color               //颜色
                           )
{
	uint32 		i = 0, j = 0;		
	uint8		*code_ptr = PNULL;
	int16		x = x0, y = y0;
	GUI_COLOR_T edge_color = color;
	int32		temp  = 0;
	uint32 		width = lcdwidth;
    GUI_POINT_T point = {0};
    GUI_RECT_T  ch_rect = {0};

	SCI_ASSERT(lcd_buffer_ptr != PNULL && codedata_ptr != PNULL);

	code_ptr = codedata_ptr;
	
	if(FONT_EFFECT_NONE != effect)
	{
		if(FONT_EFFECT_REVERSE_EDGE == effect)
		{
			edge_color = (GUI_COLOR_T)(~color);
		}
		else if(FONT_EFFECT_WIHTE_EDGE == effect)
		{
			edge_color = 0xFFFF;
		}
		else
		{
			SCI_PASSERT(FALSE,("DrawTOLCDAngle0: effect %d is error!",effect));
		}
		//SCI_TRACE_LOW("[GUI LABEL]: edge_color = 0x%x", edge_color);
        //边缘显示效果
            for (j = 0; j<(fontheight * fontwidth ); j++) 											
            {		
                temp = j/8;
                if (CheckPixelBase (codedata_ptr, temp, (0x01<<((j%8)&0x07))))
                {                
                    pFunc->GetFontPixelPos( &x, &y, x0, y0, j%fontwidth, j/fontwidth);
                    point.x = x;
                    point.y = y;
                    if (GUI_PointIsInRect(point, rect))
                    {
                        if (0 < x)
                            WR_TO_LCD(lcd_buffer_ptr, (x - 1), y, width, (GUI_COLOR_T)(~color));	
                        if ((lcdwidth - 1) > x)
                            WR_TO_LCD(lcd_buffer_ptr, (x + 1), y, width, (GUI_COLOR_T)(~color));
                        if (0 < y)
                            WR_TO_LCD(lcd_buffer_ptr, x, y - 1, width, (GUI_COLOR_T)(~color));	
                        if ((lcdheight - 1) > y)
                            WR_TO_LCD(lcd_buffer_ptr, x, y + 1, width, (GUI_COLOR_T)(~color));
                    }
                }
            }
    }

    ch_rect.left = x0;
    ch_rect.top = y0;
    ch_rect.right = x0 + fontwidth - 1;
    ch_rect.bottom = y0 + fontheight - 1;
    if (!GUI_RectIsCovered(rect, ch_rect))
    {
        /* Draw it ..*/		
        for (j = 0; j<(fontheight * fontwidth ); j++) 											
        {		
            temp = j/8;
            if (CheckPixelBase (codedata_ptr, temp, (0x01<<((j%8)&0x07)))) 												
            {		
                pFunc->GetFontPixelPos( &x, &y, x0, y0, j%fontwidth, j/fontwidth);
                point.x = x;
                point.y = y;
                if (GUI_PointIsInRect(point, rect))
                {
                    WR_TO_LCD(lcd_buffer_ptr, x, y, lcdwidth, color);
                }
            }																																	
        }
    }
    else
    /* Draw it ..*/		
    {
        for (j = 0; j<(fontheight * fontwidth ); j++) 											
        {		
            temp = j/8;
            if (CheckPixelBase (codedata_ptr, temp, (0x01<<((j%8)&0x07)))) 												
            {		
                pFunc->GetFontPixelPos( &x, &y, x0, y0, j%fontwidth, j/fontwidth);
                WR_TO_LCD(lcd_buffer_ptr, x, y, lcdwidth, color);
            }																																	
        }
    }
}
/*******************************************************************************************/
// 	Description : 显示字符
//	Global resource dependence :
//  Author: bin.ji
//	Note:
/*********************************************************************************************/
LOCAL void DrawTOLCDAngleOther(
                               const GUI_FONT_ANGLE_FUNC_T* pFunc,
                               GUIFONT_EFFECT_T effect,        //特殊效果
                               GUI_COLOR_T *lcd_buffer_ptr,    //显示buffer
                               uint16 lcdwidth,                //lcd宽度
                               uint16 lcdheight,               //lcd高度
                               int16 x0,                      //字符左上角x坐标
                               int16 y0,                      //字符左上角y坐标
                               GUI_RECT_T rect,
                               uint8 fontwidth,                //字符宽度
                               uint8 fontheight,               //字符高度
                               uint8 *codedata_ptr,            //字符数据
                               GUI_COLOR_T color               //颜色
                               )
{
	uint32 		i = 0, j = 0;		
	uint8		*code_ptr = PNULL;
	int16		x = x0, y = y0;
	GUI_COLOR_T edge_color = color;
	int32		temp  = 0;
	uint32 		width = lcdwidth;
    GUI_POINT_T point = {0};

	SCI_ASSERT(lcd_buffer_ptr != PNULL && codedata_ptr != PNULL);

	code_ptr = codedata_ptr;
	
	if(FONT_EFFECT_NONE != effect)
	{
		if(FONT_EFFECT_REVERSE_EDGE == effect)
		{
			edge_color = (GUI_COLOR_T)(~color);
		}
		else if(FONT_EFFECT_WIHTE_EDGE == effect)
		{
			edge_color = 0xFFFF;
		}
		else
		{
			SCI_PASSERT(FALSE,("DrawTOLCDAngleOther: effect %d is error!",effect));
		}
		//SCI_TRACE_LOW("[GUI LABEL]: edge_color = 0x%x", edge_color);
        //边缘显示效果
        for (j = 0; j<(fontheight * fontwidth ); j++) 											
	{		
            temp = j/8;
            if (CheckPixelBase (codedata_ptr, temp, (0x01<<((j%8)&0x07))))
            {                
                pFunc->GetFontPixelPos( &x, &y, x0, y0, j%fontwidth, j/fontwidth);
                point.x = x;
                point.y = y;
                if (GUI_PointIsInRect(point, rect))
                {
                if (0 < x)
                    WR_TO_LCD(lcd_buffer_ptr, (x - 1), y, width, (GUI_COLOR_T)(~color));	
                if ((lcdwidth - 1) > x)
                    WR_TO_LCD(lcd_buffer_ptr, (x + 1), y, width, (GUI_COLOR_T)(~color));
                if (0 < y)
                    WR_TO_LCD(lcd_buffer_ptr, x, y - 1, width, (GUI_COLOR_T)(~color));	
                if ((lcdheight - 1) > y)
                    WR_TO_LCD(lcd_buffer_ptr, x, y + 1, width, (GUI_COLOR_T)(~color));
                }
            }
	}
    }

    /* Draw it ..*/		
    for (j = 0; j<(fontheight * fontwidth ); j++) 											
    {		
        temp = j/8;
        if (CheckPixelBase (codedata_ptr, temp, (0x01<<((j%8)&0x07)))) 												
        {		
            pFunc->GetFontPixelPos( &x, &y, x0, y0, j%fontwidth, j/fontwidth);
            point.x = x;
            point.y = y;
            if (GUI_PointIsInRect(point, rect))
            {
                WR_TO_LCD(lcd_buffer_ptr, x, y, lcdwidth, color);
            }
        }																																	
    }
}

/*******************************************************************************************/
// 	Description : 根据pixel相对位置得到pixel绝对位置
//	Global resource dependence :
//  Author: bin.ji
//	Note:
/*********************************************************************************************/
LOCAL void GetFontPixelPosAngle0(
                                 int16       *x_ptr, //out, 计算得到的位置
                                 int16       *y_ptr, //out, 计算得到的位置
                                 int16       x0,     //原点坐标
                                 int16       y0,     //原点坐标
                                 uint16       i,      //相对坐标
                                 uint16       j       //相对坐标
                                 )
{
    SCI_ASSERT(x_ptr != PNULL && y_ptr != PNULL);
    
    *x_ptr = (int16)(x0 + i);
    *y_ptr = (int16)(y0 + j);
    
}

/*******************************************************************************************/
// 	Description : 根据pixel相对位置得到pixel绝对位置
//	Global resource dependence :
//  Author: bin.ji
//	Note:
/*********************************************************************************************/
LOCAL void GetFontPixelPosAngle90(
                                  int16       *x_ptr, //out, 计算得到的位置
                                  int16       *y_ptr, //out, 计算得到的位置
                                  int16       x0,     //原点坐标
                                  int16       y0,     //原点坐标
                                  uint16       i,      //相对坐标
                                  uint16       j       //相对坐标
                                  )
{
    SCI_ASSERT(x_ptr != PNULL && y_ptr != PNULL);
    
    *x_ptr = (int16)(x0 - j);
    *y_ptr = (int16)(y0 + i);
}

/*******************************************************************************************/
// 	Description : 根据pixel相对位置得到pixel绝对位置
//	Global resource dependence :
//  Author: bin.ji
//	Note:
/*********************************************************************************************/
LOCAL void GetFontPixelPosAngle180(
                                   int16       *x_ptr, //out, 计算得到的位置
                                   int16       *y_ptr, //out, 计算得到的位置
                                   int16       x0,     //原点坐标
                                   int16       y0,     //原点坐标
                                   uint16       i,      //相对坐标
                                   uint16       j       //相对坐标
                                   )
{
    SCI_ASSERT(x_ptr != PNULL && y_ptr != PNULL);
    *x_ptr = (int16)(x0 - i);
    *y_ptr = (int16)(y0 - j);   
}

/*******************************************************************************************/
// 	Description : 根据pixel相对位置得到pixel绝对位置
//	Global resource dependence :
//  Author: bin.ji
//	Note:
/*********************************************************************************************/
LOCAL void GetFontPixelPosAngle270(
                                   int16       *x_ptr, //out, 计算得到的位置
                                   int16       *y_ptr, //out, 计算得到的位置
                                   int16       x0,     //原点坐标
                                   int16       y0,     //原点坐标
                                   uint16       i,      //相对坐标
                                   uint16       j       //相对坐标
                                   )
{
    SCI_ASSERT(x_ptr != PNULL && y_ptr != PNULL);
    
    *x_ptr = (int16)(x0 + j);
    *y_ptr = (int16)(y0 - i);
}


/*******************************************************************************************/
// 	Description : 根据pixel相对位置得到pixel绝对位置
//	Global resource dependence :
//  Author: bin.ji
//	Note:
/*********************************************************************************************/
LOCAL void MapPixelAngle0(
                          int16       *x_ptr, //out, 计算得到的位置
                          int16       *y_ptr, //out, 计算得到的位置
                          int16       i,      //相对坐标
                          int16       j,       //相对坐标
                          GUI_RECT_T  box     //原点坐标
                          )
{
    SCI_ASSERT(x_ptr != PNULL && y_ptr != PNULL);
    
    *x_ptr = (int16)(box.left + i);
    *y_ptr = (int16)(box.top + j);
}

/*******************************************************************************************/
// 	Description : 根据pixel相对位置得到pixel绝对位置
//	Global resource dependence :
//  Author: bin.ji
//	Note:
/*********************************************************************************************/
LOCAL void MapPixelAngle90(
                          int16       *x_ptr, //out, 计算得到的位置
                          int16       *y_ptr, //out, 计算得到的位置
                          int16       i,      //相对坐标
                          int16       j,       //相对坐标
                          GUI_RECT_T  box     //原点坐标
                          )
{
    SCI_ASSERT(x_ptr != PNULL && y_ptr != PNULL);
    
    *x_ptr = (int16)(box.right - j);
    *y_ptr = (int16)(box.top + i);
}

/*******************************************************************************************/
// 	Description : 根据pixel相对位置得到pixel绝对位置
//	Global resource dependence :
//  Author: bin.ji
//	Note:
/*********************************************************************************************/
LOCAL void MapPixelAngle180(
                            int16       *x_ptr, //out, 计算得到的位置
                            int16       *y_ptr, //out, 计算得到的位置
                            int16       i,      //相对坐标
                            int16       j,       //相对坐标
                            GUI_RECT_T  box     //原点坐标
                            )
{
    SCI_ASSERT(x_ptr != PNULL && y_ptr != PNULL);
    
    *x_ptr = (int16)(box.right - i);
    *y_ptr = (int16)(box.bottom - j);
}

/*******************************************************************************************/
// 	Description : 根据pixel相对位置得到pixel绝对位置
//	Global resource dependence :
//  Author: bin.ji
//	Note:
/*********************************************************************************************/
LOCAL void MapPixelAngle270(
                            int16       *x_ptr, //out, 计算得到的位置
                            int16       *y_ptr, //out, 计算得到的位置
                            int16       i,      //相对坐标
                            int16       j,       //相对坐标
                            GUI_RECT_T  box     //原点坐标
                            )
{
    SCI_ASSERT(x_ptr != PNULL && y_ptr != PNULL);
    
    *x_ptr = (int16)(box.left + j);
    *y_ptr = (int16)(box.bottom - i);
}

/*******************************************************************************************/
// 	Description : 根据pixel绝对位置得到pixel相对位置
//	Global resource dependence :
//  Author: bin.ji
//	Note:
/*********************************************************************************************/
LOCAL void MapPixelReverseAngle0(
                                 int16        *i_ptr,      //相对坐标
                                 int16        *j_ptr,       //相对坐标
                                 uint16       *width_ptr,
                                 uint16       *height_ptr,
                                 int16        x, //out, 计算得到的位置
                                 int16        y, //out, 计算得到的位置
                                 GUI_RECT_T   box     //原点坐标
                                 )
{
    SCI_ASSERT(i_ptr != PNULL && j_ptr != PNULL && width_ptr != PNULL && height_ptr != PNULL);
    
    
    *i_ptr = (int16)(x - box.left);
    *j_ptr = (int16)(y - box.top);
    *width_ptr = (int16)((box.right + 1 - box.left));
    *height_ptr = (int16)((box.bottom + 1 - box.top));
}

/*******************************************************************************************/
// 	Description : 根据pixel绝对位置得到pixel相对位置
//	Global resource dependence :
//  Author: bin.ji
//	Note:
/*********************************************************************************************/
LOCAL void MapPixelReverseAngle90(
                                  int16        *i_ptr,      //相对坐标
                                  int16        *j_ptr,       //相对坐标
                                  uint16       *width_ptr,
                                  uint16       *height_ptr,
                                  int16        x, //out, 计算得到的位置
                                  int16        y, //out, 计算得到的位置
                                  GUI_RECT_T   box     //原点坐标
                                  )
{
    SCI_ASSERT(i_ptr != PNULL && j_ptr != PNULL && width_ptr != PNULL && height_ptr != PNULL);
    
    *i_ptr = (int16)(y - box.top);
    *j_ptr = (int16)(box.right - x);
    *width_ptr = (int16)((box.bottom + 1 - box.top));
    *height_ptr = (int16)((box.right + 1 - box.left));
}

/*******************************************************************************************/
// 	Description : 根据pixel绝对位置得到pixel相对位置
//	Global resource dependence :
//  Author: bin.ji
//	Note:
/*********************************************************************************************/
LOCAL void MapPixelReverseAngle180(
                                   int16        *i_ptr,      //相对坐标
                                   int16        *j_ptr,       //相对坐标
                                   uint16       *width_ptr,
                                   uint16       *height_ptr,
                                   int16        x, //out, 计算得到的位置
                                   int16        y, //out, 计算得到的位置
                                   GUI_RECT_T   box     //原点坐标
                                   )
{
    SCI_ASSERT(i_ptr != PNULL && j_ptr != PNULL && width_ptr != PNULL && height_ptr != PNULL);
    
    
    *i_ptr = (int16)(box.right - x);
    *j_ptr = (int16)(box.bottom - y);
    *width_ptr = (int16)((box.right + 1 - box.left));
    *height_ptr = (int16)((box.bottom + 1 - box.top));
}

/*******************************************************************************************/
// 	Description : 根据pixel绝对位置得到pixel相对位置
//	Global resource dependence :
//  Author: bin.ji
//	Note:
/*********************************************************************************************/
LOCAL void MapPixelReverseAngle270(
                                   int16        *i_ptr,      //相对坐标
                                   int16        *j_ptr,       //相对坐标
                                   uint16       *width_ptr,
                                   uint16       *height_ptr,
                                   int16        x, //out, 计算得到的位置
                                   int16        y, //out, 计算得到的位置
                                   GUI_RECT_T   box     //原点坐标
                                   )
{
    SCI_ASSERT(i_ptr != PNULL && j_ptr != PNULL && width_ptr != PNULL && height_ptr != PNULL);
    
    *i_ptr = (int16)(box.bottom - y);
    *j_ptr = (int16)(x - box.left);
    //*i_ptr = (int16)(x - box.left);
    //*j_ptr = (int16)(box.bottom - y);
    
    *width_ptr = (int16)((box.bottom + 1 - box.top));
    *height_ptr = (int16)((box.right + 1 - box.left));
}


/*****************************************************************************/
// 	Description : 得到下一个字符位置（不检查有效性）
//	Global resource dependence : none
//  Author: bin.ji
//	Note: 
/*****************************************************************************/
LOCAL void NextFontPosAngle0(
                             int16 *x_ptr,        //out,
                             int16 *y_ptr,        //out,
                             uint8 space,
                             uint8 fontwidth
                             )
{
    *x_ptr = (int16)(*x_ptr+space+fontwidth);
}

/*****************************************************************************/
// 	Description : 得到下一个字符位置（不检查有效性）
//	Global resource dependence : none
//  Author: bin.ji
//	Note: 
/*****************************************************************************/
LOCAL void NextFontPosAngle90(
                              int16 *x_ptr,        //out,
                              int16 *y_ptr,        //out,
                              uint8 space,
                              uint8 fontwidth
                              )
{
    *y_ptr = (int16)(*y_ptr+space+fontwidth);
}

/*****************************************************************************/
// 	Description : 得到下一个字符位置（不检查有效性）
//	Global resource dependence : none
//  Author: bin.ji
//	Note: 
/*****************************************************************************/
LOCAL void NextFontPosAngle180(
                               int16 *x_ptr,        //out,
                               int16 *y_ptr,        //out,
                               uint8 space,
                               uint8 fontwidth
                               )
{
    *x_ptr = (int16)(*x_ptr-space-fontwidth);
}

/*****************************************************************************/
// 	Description : 得到下一个字符位置（不检查有效性）
//	Global resource dependence : none
//  Author: bin.ji
//	Note: 
/*****************************************************************************/
LOCAL void NextFontPosAngle270(
                               int16 *x_ptr,        //out,
                               int16 *y_ptr,        //out,
                               uint8 space,
                               uint8 fontwidth
                               )
{
    *y_ptr = (int16)(*y_ptr-space-fontwidth);
}



const GUI_FONT_ANGLE_FUNC_T angle_func_table[4] =
{
    {
        DrawTOLCDAngle0,
        GetFontPixelPosAngle0,   
        MapPixelAngle0,
        MapPixelReverseAngle0,
        NextFontPosAngle0
    },
    {
        DrawTOLCDAngleOther,
        GetFontPixelPosAngle90,   
        MapPixelAngle90,
        MapPixelReverseAngle90,
        NextFontPosAngle90
    },
    {
        DrawTOLCDAngleOther,
        GetFontPixelPosAngle180,   
        MapPixelAngle180,
        MapPixelReverseAngle180,
        NextFontPosAngle180
    },
    {
        DrawTOLCDAngleOther,
        GetFontPixelPosAngle270,
        MapPixelAngle270,
        MapPixelReverseAngle270,
        NextFontPosAngle270
    },

};

PUBLIC BOOLEAN MTK_Ucs_IsFont(GUI_FONT_T font, uint16   unicode)
{
      uint32 rang_start_ucs = 0;
      uint32 rang_end_ucs = 0;
      uint32 i = 0;
      uint8  is_in_rang = 0;

      if((unicode >= 0x30 && unicode <= 0x39)
        || unicode == 0x23
        || unicode == 0x2A
        || unicode == 0x2B
        || unicode == 0x41
        || unicode == 0x4D
        || unicode == 0x50
        || unicode == 0x57
        || unicode == 0x70
        || unicode == 0x77
      )
      {
        is_in_rang = 1;
      }

       if(is_in_rang == 1)
       {
           return TRUE;
       }

	return FALSE;
}

PUBLIC BOOLEAN MTK_GetFontCharInfo(
                                                        GUI_FONT_T font,
								uint16   unicode,	    //character to display
								MTK_FONT_INDEX_T   * mtk_ch
							)
{

       if(MTK_Ucs_IsFont(font,unicode))
       {
           mtk_ch->height = MMITHEME_PHONE_IMG_HEIGHT;
           mtk_ch->width = MMITHEME_PHONE_IMG_WIDTH;
           mtk_ch->unicode = unicode;
           mtk_ch->offset = 0;
           mtk_ch->img_id = MTK_GetImgID(unicode);
           return TRUE;
       }
       else
       {
           mtk_ch->height = MMITHEME_PHONE_IMG_HEIGHT;
           mtk_ch->width = MMITHEME_PHONE_IMG_WIDTH;
           mtk_ch->unicode = unicode;
           mtk_ch->offset = 0;
           mtk_ch->img_id = MTK_GetImgID(unicode);
       }


	return FALSE;
}

LOCAL int32 MTK_ShowStringInGeneralRect( 
                                         GUIFONT_EFFECT_T effect, //特殊效果
                                         GUI_COLOR_T  edge_color,
                                         GUI_ANGLE_E  angle,      //旋转角度
                                         GUI_COLOR_T  *lcdbuf_ptr,    //显示buffer
                                         uint16       lcdwidth,   //lcd宽度
                                         uint16       lcdheight,  //lcd高度
                                         GUI_RECT_T   box, 		//the fixed display area
                                         const wchar  *text_ptr,  //string to display
                                         uint16       len,   		//length of string to display
                                         uint16       space, 	    //space between two chars.
                                         GUI_COLOR_T  color,      //string color
                                         GUI_FONT_T   font,		//font pointer		
                                         uint8        line_space,  //space between 2 lines		
                                         uint8 state
                                         )
{ 
    int16 i = 0;
    int16	x1=box.left, y1=box.top;
    uint16 char_unicode = 0;
    uint16 temp_char_unicode = 0;
    uint16	hanzi_height = 0;
    BOOLEAN spchar_result = FALSE;  //CR, LF等特殊字符
    BOOLEAN exceed_result = FALSE;   //是否超出边界
    uint8	fontwidth = 0, fontheight = 0;
    uint32 img_id = PNULL;
    const GUI_FONT_ANGLE_FUNC_T* pFunc = PNULL;
    GUI_RECT_T	actual_rect = {0};
    MTK_FONT_INDEX_T mtk_char_info = {0};
    
    #if 0
    if(box.right<=box.left)
	box.left -=box.right;
    #endif
    
    SCI_ASSERT(lcdbuf_ptr !=  PNULL);
    //SCI_ASSERT(box.right >= box.left);
    //SCI_ASSERT(box.bottom >= box.top);
    
    if (PNULL == text_ptr)
    {
        return 1;
    }

    actual_rect = box;
    
    pFunc = &angle_func_table[angle];

    //左上角的坐标
    x1 = (int16)((ANGLE_0 == angle || ANGLE_270 == angle) ? box.left : box.right);
    y1 = (int16)((ANGLE_0 == angle || ANGLE_90 == angle) ? box.top : box.bottom);

    temp_char_unicode = 0;

    for(i = 0; i < len; i++) 
    {
        char_unicode = text_ptr[i];
        
        //字符串以'\0'结束
        if( 0 == char_unicode )
        {
            break;
        }

        if ((CR_CHAR == temp_char_unicode)
            &&(LF_CHAR == char_unicode))
        {
            temp_char_unicode = char_unicode;
            continue;
        }

        temp_char_unicode = char_unicode;
        if(MTK_GetFontCharInfo(font,char_unicode, &mtk_char_info))
        {
            fontwidth =  mtk_char_info.width;
            fontheight = mtk_char_info.height;
            img_id = mtk_char_info.img_id;
        }
        else
        {
            return 2;
        }
        
	// 行高不等于某一个字符的高度
	hanzi_height = fontheight;

        // 判断回车换行是否使用空格替代
        if (((CR_CHAR == temp_char_unicode) || (LF_CHAR == char_unicode)))
        {
            char_unicode = BL_CHAR;
            if(MTK_GetFontCharInfo(font,char_unicode, &mtk_char_info))
            {
                fontwidth =  mtk_char_info.width;
                fontheight = mtk_char_info.height;
                img_id = mtk_char_info.img_id;
            }
            else
            {
                return 3;
            }
            spchar_result = FALSE;
        }

        //check font pos
        spchar_result = GetFontPos(pFunc,&x1,&y1,char_unicode,box,fontwidth,(MAX(fontheight, hanzi_height)), line_space);

        exceed_result = CheckFontPos(pFunc,x1,y1,box,spchar_result,fontwidth,fontheight);  

        if (exceed_result)
            break;

        if (!spchar_result)
        {
            #if 0
		if (FONT_EFFECT_CUSTOM != effect)
		{
			pFunc->DrawTOLCD(pFunc,effect, lcdbuf_ptr,  lcdwidth, lcdheight,x1, y1,actual_rect, fontwidth,fontheight,code_ptr, color);
		}
		else
		{
			DrawTOBuf(pFunc,effect, lcdbuf_ptr, lcdwidth, lcdheight, x1, y1, actual_rect, fontwidth,fontheight, code_ptr, color, edge_color);
		}
            #else
                DrawMTKImgBuf( lcdbuf_ptr, lcdwidth, lcdheight, x1, y1, actual_rect, fontwidth,fontheight, img_id, color, edge_color);
            #endif

            pFunc->NextFontPos(&x1,&y1,(uint8)space,fontwidth);
        }
    }

	//calc display rect
    actual_rect = AdjustCHRectByAngle(box, angle, x1, y1, hanzi_height);

    return 0;
}

LOCAL int32 MTK_ShowStringInSpecialRect( 
                                         GUISTR_STYLE_T style,
                                         GUI_COLOR_T  *lcdbuf_ptr,    //显示buffer
                                         uint16       lcdwidth,   //lcd宽度
                                         uint16       lcdheight,  //lcd高度
                                         GUI_RECT_T   box, 		    //the fixed display area
                                         GUI_RECT_T   disp_rect,      //用户要剪切的实际区域
                                         const wchar  *text_ptr,      //string to display
                                         uint16       len   		    //length of string to display
                                         )
{
    int16 i = 0;
    int16	x1=box.left, y1=box.top;
    uint16 char_unicode = 0;
    uint16 temp_char_unicode = 0;
    uint16	hanzi_height = 0;
    BOOLEAN spchar_result = FALSE;  //CR, LF等特殊字符
    BOOLEAN exceed_result = FALSE;   //是否超出边界
    uint8	fontwidth = 0, fontheight = 0;
    uint32	img_id = PNULL;
    const GUI_FONT_ANGLE_FUNC_T* pFunc = PNULL;
    GUI_RECT_T	actual_rect = {0};
    GUI_RECT_T	lcd_rect = {0};
    MTK_FONT_INDEX_T mtk_char_info = {0};
    
    SCI_ASSERT(lcdbuf_ptr !=  PNULL);
    //SCI_ASSERT(box.right >= box.left);
    //SCI_ASSERT(box.bottom >= box.top);
    
    if (PNULL == text_ptr)
    {
        return 1;
    }
    lcd_rect.left = 0;
    lcd_rect.top = 0;
    lcd_rect.right = lcdwidth - 1;
    lcd_rect.bottom = lcdheight - 1;

    actual_rect = GetTrimRect(box, disp_rect);
    
    pFunc = &angle_func_table[style.angle];

    //左上角的坐标
    x1 = (int16)((ANGLE_0 == style.angle || ANGLE_270 == style.angle) ? box.left : box.right);
    y1 = (int16)((ANGLE_0 == style.angle || ANGLE_90 == style.angle) ? box.top : box.bottom);

    temp_char_unicode = 0;

    for(i = 0; i < len; i++) 
    {
        char_unicode = text_ptr[i];
        
        //字符串以'\0'结束
        if( 0 == char_unicode )
        {
            break;
        }

        if ((CR_CHAR == temp_char_unicode)
            &&(LF_CHAR == char_unicode))
        {
            temp_char_unicode = char_unicode;
            continue;
        }

        temp_char_unicode = char_unicode;
        if(MTK_GetFontCharInfo(style.font,char_unicode, &mtk_char_info))
        {
            fontwidth =  mtk_char_info.width;
            fontheight = mtk_char_info.height;
            img_id = mtk_char_info.img_id;
        }
        else
        {
            return 2;
        }
        
	// 行高不等于某一个字符的高度
	hanzi_height = fontwidth;

        // 判断回车换行是否使用空格替代
        if (((CR_CHAR == temp_char_unicode) || (LF_CHAR == char_unicode)))
        {
            char_unicode = BL_CHAR;
            if(MTK_GetFontCharInfo(style.font,char_unicode, &mtk_char_info))
            {
                fontwidth =  mtk_char_info.width;
                fontheight = mtk_char_info.height;
                img_id = mtk_char_info.img_id;
            }
            else
            {
                return 3;
            }
            spchar_result = FALSE;
        }
        //check font pos
        spchar_result = GetFontPos(pFunc,&x1,&y1,char_unicode,box,fontwidth,(MAX(fontheight, hanzi_height)), style.line_space);

        exceed_result = CheckFontPos(pFunc,x1,y1,box,spchar_result,fontwidth,fontheight);  

        if (exceed_result)
            break;

        if (!spchar_result)
        {
            #if 0
        	if (FONT_EFFECT_CUSTOM != style.effect)
        	{
        		pFunc->DrawTOLCD(pFunc,style.effect, lcdbuf_ptr,  lcdwidth, lcdheight,x1, y1,actual_rect,fontwidth,fontheight,code_ptr, style.font_color);
        	}
        	else
        	{
        		DrawTOBuf(pFunc,style.effect, lcdbuf_ptr, lcdwidth, lcdheight, x1, y1, actual_rect, fontwidth,fontheight, code_ptr, style.font_color, style.edge_color);
        	}
            #else
        		DrawMTKImgBuf( lcdbuf_ptr, lcdwidth, lcdheight, x1, y1, actual_rect, fontwidth,fontheight, img_id, style.font_color, style.edge_color);
            #endif

            pFunc->NextFontPos(&x1,&y1,(uint8)style.char_space,fontwidth);
        }
    }

    //calc display rect
    actual_rect = AdjustCHRectByAngle(box, style.angle, x1, y1, hanzi_height);

	return 0;
}

PUBLIC int32 MTK_DrawTextInline(
                               GUISTR_STYLE_T * style_ptr,
                               GUI_COLOR_T  *lcdbuf_ptr,    //显示buffer
                               uint16       lcdwidth,       //lcd宽度
                               uint16       lcdheight,      //lcd高度
                               GUI_RECT_T   disp_rect, 		    //the fixed display area
                               GUI_RECT_T   clip_rect,      //用户要剪切的实际区域
                               const wchar  *text_ptr,      //string to display
                               uint16       len   		    //length of string to display
                               )
{ 
    GUI_RECT_T trim_rect = {0};// 该区域为lcd_rect, box, disp_rect三个区域的共同区域.该区域为lcd上实际显示字符串的区域
    GUISTR_STYLE_T style = *style_ptr;
    int32 res = 0;
    
    SCI_ASSERT(text_ptr != PNULL);
    SCI_ASSERT(lcdbuf_ptr !=  PNULL);
        
    trim_rect = GetTrimRect(clip_rect, disp_rect); // 获取用户输入的box和lcd区域的剪切区域

    if (text_ptr == PNULL || len == 0)
    {
        return 1;
    }

    if (GUI_EqualRect(trim_rect, disp_rect))
    {
            res = MTK_ShowStringInGeneralRect(style.effect,style.edge_color,style.angle,lcdbuf_ptr,lcdwidth,lcdheight,disp_rect,
            text_ptr,len,style.char_space,style.font_color,style.font,style.line_space, 1);
    }
    else
    {
        res = MTK_ShowStringInSpecialRect(style,lcdbuf_ptr,lcdwidth,lcdheight,disp_rect, clip_rect, text_ptr, len);
    }
    
    return res;
}

PUBLIC int32 PhImg_DrawText(
                              GUISTR_STYLE_T * style_ptr,
                              SPML_TEXT_PARAM_T* param_ptr,  //显示参数
                              const wchar*             text_ptr,   //unicode字符串
                              uint16                   len,        //字符个数
                              SPML_TEXT_METRICS_T*     metrics_ptr //out, 字符串的宽高, 可以为PNULL
                              )
{
    GUI_RECT_T trim_rect = {0};// 该区域为lcd_rect, box, disp_rect三个区域的共同区域.该区域为lcd上实际显示字符串的区域
    GUISTR_STYLE_T style = *style_ptr;
    int32 res = 0;
    uint16          x = 0;
    uint16          y = 0;
    uint16          pos = 0;
    uint16          font_height = 0;
    uint16          char_num = 0;
    uint16          pixel_num = 0;
    int32            i = 0;
    int32            j = 0;
    int32            k = 0;
    GUI_RECT_T box = param_ptr->display_rect;
    GUI_RECT_T disp_rect;
    uint8 * tmp_ptr = PNULL;
    uint16 tmp_len = 0;
    SPML_TEXT_METRICS_T tmp_metrics = {0};
    SPML_TEXT_FORMAT_T tmp_format = 0;
    SCI_ASSERT(text_ptr != PNULL);
    
    font_height = MMITHEME_PHONE_IMG_HEIGHT;
    y = GetImgYPosition(box,text_ptr,len,style.char_space,MMITHEME_PHONE_IMG_WIDTH,style.align,style.line_space,TRUE);
    
    if ( GUI_GetPhImgInfoInLine(text_ptr,
        (uint16)(len),
        MMITHEME_PHONE_IMG_WIDTH,
        (uint8)style.char_space,
        (uint16)(box.right-box.left+1),
        &char_num, 
        &pixel_num,
        TRUE
        ) )
    {
        x = GetImgXPosition(box,style.align,pixel_num);
        box.left = x;
        box.top = y;
        metrics_ptr->height = font_height;
        metrics_ptr->width = pixel_num;
    }
    
    trim_rect = GetTrimRect(param_ptr->clip_rect, box); // 获取用户输入的box和lcd区域的剪切区域

    if (text_ptr == PNULL || len == 0)
    {
        return 1;
    }
    
    tmp_ptr = &text_ptr[0];
    tmp_len = len;

    disp_rect = box;
    if(MTK_DrawTextInline( style_ptr, param_ptr->buffer_ptr,param_ptr->buffer_width,param_ptr->buffer_height, disp_rect,param_ptr->clip_rect,tmp_ptr,tmp_len))
    {
        SPMLAPI_DrawText( param_ptr, tmp_ptr, tmp_len, &tmp_metrics );
    }
}

PUBLIC int16 GUI_GetImgCusorCoordinate(//绝对坐标
                                    GUI_ALIGN_E       align,
                                    const GUI_RECT_T* rect_ptr,
                                    int8              dir,
                                    const wchar*      str_ptr,
                                    uint16            str_len,
                                    uint16            cursor_pos,
                                    uint16	      img_width,
                                    uint16            char_space
                                    )
{
    SPML_TEXT_PARAM_T param = {0};
    int text_width;
    int align_offset;
    int32 xpos = 0;
    uint16          font_width = MMITHEME_PHONE_IMG_WIDTH;
    uint16          char_num = 0;
    uint16          pixel_num = 0;
    GUI_RECT_T box = {0};
    uint16          x = 0;
    uint16          y = 0;
    uint16          pos = 0;
    
    if (PNULL == rect_ptr)
    {
        //SCI_TRACE_LOW:"== GUI_GetCusorCoordinate == param rect_ptr is PNULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,GUIFONT_96_112_2_18_3_17_1_147,(uint8*)"");
        return x;
    }

    if (PNULL == str_ptr || 0 == str_len )
    {
        x = GetImgXPosition(*rect_ptr,align,0);
        return x;
    }

    if ( GUI_GetPhImgInfoInLine(str_ptr,
        (uint16)(str_len),
        font_width,
        (uint8)char_space,
        (uint16)(rect_ptr->right-rect_ptr->left+1),
        &char_num, 
        &pixel_num,
        TRUE
        ) )
    {
        x = GetImgXPosition(*rect_ptr,align,pixel_num);
        xpos =  x + (cursor_pos * (font_width+char_space));
    }
    else
    {
        x = GetImgXPosition(*rect_ptr,align,0);
        xpos = x;
    }
    
    return xpos;

}
/*******************************************************************************************/
// 	Description : 获得光标的x坐标并微调光标位置
//	Global resource dependence :
//  Author: James.Zhang
//	Note: 
/*********************************************************************************************/
PUBLIC int16 GUI_GetImgCursorInfo(
                               GUI_ALIGN_E       align,
                               const GUI_RECT_T	*rect_ptr,
                               int8				 dir,
                               const wchar		*str_ptr,
                               uint16            str_len,
                               BOOLEAN           is_left,
                               GUI_FONT_T	     font_type,
                               uint16            char_space,
                               int32*            cursor_ptr
                               )
{
    #if 0
    int16 x = 0;
    SPML_TEXT_PARAM_T param = {0};
    
	if (PNULL == rect_ptr)
	{
        //SCI_TRACE_LOW:"== GUI_GetCursorInfo == param rect_ptr is PNULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,GUIFONT_149_112_2_18_3_17_1_148,(uint8*)"");
        return x;
	}

    if (PNULL == str_ptr || 0 == str_len )
    {
        return (rect_ptr->left);
    }
    
    param.format |= ConvertCursorAlign( align );
    param.display_rect = param.clip_rect = *rect_ptr;
    param.glyph_space = char_space;
    param.dir  = dir;
    param.size = MMITHEME_PHONE_IMG_WIDTH;
    
    if ( TEXT_DIR_RTL == param.dir )
    {
        param.format |= TEXT_FORMAT_RIGHT;
    }
    
     x = (int16)SPMLAPI_GetCursorInfo( &param, str_ptr, str_len, is_left, cursor_ptr );

    return x;
    #else
        return 0;
    #endif
}

/*******************************************************************************************/
// 	Description : 获得光标索引
//	Global resource dependence :
//  Author: James.Zhang
//	Note: 
/*********************************************************************************************/
PUBLIC uint16 GUI_GetImgCursorPos(//相对位置
                               GUI_ALIGN_E      align,
							   const GUI_RECT_T	*rect_ptr,      //in:
							   int8				dir,            //in:
							   const wchar		*str_ptr,       //in:
							   uint16           str_len,        //in:
							   uint16           cursor_pos,	    //in:相对位置
							   int16            cursor_x,	    //in:绝对坐标
							   GUI_FONT_T	    font_type,      //in:
							   uint16           char_space,     //in:
                               BOOLEAN          *is_find_ptr    //in/out:may PNULL
							   )
{
    SPML_TEXT_PARAM_T param = {0};
    int text_width;
    int align_offset;
    uint16 xpos = 0;
    uint16          font_width = MMITHEME_PHONE_IMG_WIDTH;
    uint16          char_num = 0;
    uint16          pixel_num = 0;
    GUI_RECT_T box = {0};
    uint16          x = 0;
    uint16          y = 0;
    uint16          pos;
    
    if (PNULL == rect_ptr)
    {
        //SCI_TRACE_LOW:"== GUI_GetCusorCoordinate == param rect_ptr is PNULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,GUIFONT_96_112_2_18_3_17_1_147,(uint8*)"");
        return x;
    }

    if (PNULL == str_ptr || 0 == str_len )
    {
        xpos = 0;
        return xpos;
    }
    
    xpos = str_len;
    if ( GUI_GetPhImgInfoInLine(str_ptr,
        (uint16)(str_len),
        font_width,
        (uint8)char_space,
        (uint16)(rect_ptr->right-rect_ptr->left+1),
        &char_num, 
        &pixel_num,
        TRUE
        ) )
    {
        x = GetImgXPosition(*rect_ptr,align,pixel_num);
    }
    else
    {
        x = GetImgXPosition(*rect_ptr,align,0);
    }

    switch (align)
    {
        case ALIGN_RIGHT:
        case ALIGN_RVMIDDLE:
            if(x >= cursor_x)
            {
                xpos = 0;
            }
            else
            {
                pos = (cursor_x - x + char_space+1)/(font_width+char_space);
                if(pos <= str_len)
                {
                    xpos = pos;
                }
                else
                {
                    xpos = str_len;
                }
            }
            break;

        case ALIGN_HMIDDLE:
        case ALIGN_HVMIDDLE:
            xpos = str_len;
            break;

        default:
            if(cursor_pos <= str_len)
            {
                xpos = cursor_pos;
            }
            else
            {
                xpos = str_len;
            }
            break;
    }
    
    return xpos;

}



/*==============================================================================
Description: 获得光标索引
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note: 增强型，同时获得光标与字符的相对位置关系
==============================================================================*/

#endif

#endif  //_GUI_FONT_C_
