#include "mmi_app_kuro_trc.h"
#include "std_header.h"
#ifdef KURO_SUPPORT
/*****************************************************************************
** File Name:      mmikur_lyric.c                                            *
** Author:                                                                   *
** Date:           20/08/2007                                                *
** Description:    This file is used to describe kur player                  *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 08/2007       CMZS       Create                                  *
******************************************************************************/

#define _MMIKUR_LYRIC_C_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "sci_types.h"
#include "mmikur_internal.h"
#include "mmikur_lyric_internal.h"
#include "mmifmm_export.h"
#include "mmi_id.h"
#include "mmikur_id.h"
#include "mmi_appmsg.h"
#include "guifont.h"
#include "mmk_timer.h"
#include "guilabel.h"
//#include "mmimultim.h"
#include "block_mem.h"
#include "bswap.h"
#include "klrfunction.h"
#include "mmikuro_image.h"
#include "guilcd.h"
#include "mmikur.h"
#include "mmidisplay_data.h"
#include "mmi_default.h"
#include "guistring.h"

/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
#define LCD_WIDTH 240
#define LCD_HEIGHT 320
#define KLR_DELAY_TIME       300    //定义歌词需要提前显示的ms
#define KUR_COUNTER_TIME    30000
#define KLR_LINE_MAXLENGTH  100
#define KLR_RECT_WIDTH 25    //定义显示歌词的rect width
#define KLR_RECT_HEIGHT 310 //定义显示歌词的rect height
#define LYRIC_BUFFER_SIZE   (25*310* 2)//定义一行歌词所需要的点阵大小
#define KLR_FONT_HEIGHT 25
#define KLR_TOOLBAR_HEIGHT 50

#define KLR_LINES_PER_PAGE 6 //全屏显示歌词时每屏显示的行数

#define FASTALPHABLEND16BIT(pixelvalsrc,pixelvaldest) \
((((pixelvalsrc)& 0xF7DF)>>1) + (((pixelvalsrc)&0xE79C)>>2) + (((pixelvaldest)&0xE79C) >> 2))

//#define TRACE_KURU_LYRIC
/*---------------------------------------------------------------------------*/
/*                          TYPE AND CONSTANT                                */
/*---------------------------------------------------------------------------*/
/**--------------------------------------------------------------------------*
 **                         LOCAL DEFINITION                                 *
 **--------------------------------------------------------------------------*/
//buffer for read kur's picture file
LOCAL uint8 *s_kur_pic_buffer = 0;
LOCAL KLR_PIC_READINFO s_klr_pic_readinfo = {0};
LOCAL GUIIMG_BITMAP_T s_bitmap = {0}; 
LOCAL uint8 *s_kur_pic_buffer_next = PNULL; //指向当前用于读取图片数据的buffer
LOCAL int32 s_nLastLine1 = -1;
LOCAL int32 s_nLastPercent1 = -1;
LOCAL int32 s_nLastLine2 = -1;
LOCAL int32 s_nLastPercent2 = -1;
LOCAL int32 s_nLastDisplayPos1 = 0;
LOCAL int32 s_nLastDisplayPos2 = 0;
LOCAL uint16 *s_lyric_buf_line1 = {0};
LOCAL uint16 *s_lyric_buf_line2 = {0};
LOCAL uint32 s_nFontLength1 = 0; //font length
LOCAL uint32 s_nFontLength2 = 0; //font length
LOCAL KLR_DSP_MODE s_klr_dispmode = KLR_MODE_TWOLINE;
LOCAL BOOLEAN s_kur_bdispvolume = FALSE;
LOCAL BOOLEAN s_kur_bdisptoolbar = FALSE;

//}
/**--------------------------------------------------------------------------*
 **                         EXTERNAL DECLARE                                 *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         GLOBAL DEFINITION                                *
 **--------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : 获取显示一串字符的点阵宽度（只支持GB2编码)
//  Global resource dependence : 
//  Author:CMZS
//  Note: 
/*****************************************************************************/
LOCAL uint32 GetStrFontLength(char *pgbstr,GUI_FONT_T font);
/*****************************************************************************/
//  Description : 根据歌手获取歌词显示颜色
//  Global resource dependence : 
//  Author:CMZS
//  Note: 
/*****************************************************************************/
LOCAL GUI_COLOR_T GetLyricColor(int32 nSex);

/*****************************************************************************/
//  Description : 将一行歌词显示到buffer中（获取歌词点阵)
//  Global resource dependence : 
//  Author:CMZS
//  Note: 
/*****************************************************************************/
LOCAL void DrawLyricToBuffer(const wchar* szLyric, uint16 *pdspbuffer,uint16 nbufferwidth,uint16 nbufferheight,GUI_COLOR_T color);

/*****************************************************************************/
//  Description : alloc memory for read klr.
//  Author:cmzs
//  Note: 
/*****************************************************************************/
LOCAL uint8* AllocFileDataBuf(uint32  file_size);
/*****************************************************************************/
// Description :do 50% alpha blending for bmp in LCD buffer and a color
// Global resource dependence : none
// Author: Geng.Ren
// Note: 
/*****************************************************************************/
LOCAL void KLR_MistBmp(
                        GUI_LCD_DEV_INFO  *dev_info_ptr,
                        GUI_RECT_T        rect,
                        GUI_COLOR_T       color     //color to draw.
                        );
/*****************************************************************************/
//  Description : This function diplays the JPG image
//  Global resource dependence : none
//  Author: Xueli.lv
//  Note:   jibin modify 2004.11.18
/*****************************************************************************/
LOCAL BOOLEAN KLR_DisplayBMPImageExt(
                        GUI_LCD_DEV_INFO const *dev_info_ptr,   // block information,            
                        uint16       x_start,      // The top point x coordinate
                        uint16       y_start,      // the top point y coordinate
                        uint16       height,       // the height of the displayed image
                        uint16       width,        // the width of the displayed image
                        BOOLEAN      cut_down_flag, // indicate whether cut down the content
                                                    // of overflow the LCD; TRUE: cut down, FALSE: otherwise
                        uint8        *bmp_ptr,
                        GUI_RECT_T   *show_rect_ptr
                        );
/*****************************************************************************/
//  Description : This function display the bmp image.
//  Global resource dependence : none
//  Author: Xueli.lv
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN MMIKLR_DisplayBMPImageExt(
                GUI_LCD_DEV_INFO const *dev_info_ptr,   // block information,
                int16                   x_start,    // the image x coordinate
                int16                   y_start,    // the image y coordinate)
                GUI_RECT_T              *show_rect_ptr,
                GUIIMG_BITMAP_T         *bitmap_ptr
                );
/*****************************************************************************/
//  Description : display one line lyric from font buffer
//  Global resource dependence : 
//  Author:CMZS
//  return:所画到的位置
//  Note: 
/*****************************************************************************/
LOCAL int32 DrawBufferLyric(GUI_LCD_DEV_INFO* pLcdDevInfo, GUI_RECT_T rectLyric, uint16* lyric_font_buf, int32 nPercent,int nLastPos,int nLyricFontLength,GUI_COLOR_T color);
/*****************************************************************************/
//  Description : display klr lyric,will be call by MMIKUR_DisplayPlayAnim
//  Global resource dependence : 
//  Author:CMZS
//  Note: 
/*****************************************************************************/
LOCAL void DisplayKlrLyric(uint32 ncurtime,BOOLEAN bPicChanged,KLR_DSP_MODE nMode);
/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description : 获取显示一串字符的点阵宽度（只支持GB2编码)
//  Global resource dependence : 
//  Author:CMZS
//  Note: 
/*****************************************************************************/
LOCAL uint32 GetStrFontLength(char *pgbstr,GUI_FONT_T font)
{
    uint32 nLength = 0;
    uint8 nlen_gb = GUI_GetFontWidth(font,UNICODE_ASCII);
    uint8 nlen_ucs = GUI_GetFontWidth(font,UNICODE_HANZI);
    char *tmp_ptr = pgbstr;
	
	//arvinzhang:delete assert--had been checked
	//SCI_ASSERT(pgbstr != PNULL);

	if(pgbstr != PNULL)
	{
	    while(*tmp_ptr)
	    {
	        if((*tmp_ptr) & 0x80)
	        {
	            nLength += nlen_ucs;
	            tmp_ptr += 2;
	        }
	        else
	        {
	            nLength += nlen_gb;
	            tmp_ptr += 1;
	        }
	    }
	}

    return nLength;
}
/*****************************************************************************/
// Description :do 50% alpha blending for bmp in LCD buffer and a color
// Global resource dependence : none
// Author: Geng.Ren
// Note: 
/*****************************************************************************/
LOCAL void KLR_MistBmp(
                        GUI_LCD_DEV_INFO  *dev_info_ptr,
                        GUI_RECT_T        rect,
                        GUI_COLOR_T       color     //color to draw.
                        )
{
    uint16           i = 0, j=0;
    uint32          offset = {0};
    uint16          loop_width  = 0, loop_height = 0;
    IMG_DISPCTL_T   buff_info = {0};
    LCD_INFO_T      lcd_info = {0};

    if (GUILCD_GetInfo((LCD_ID_E)dev_info_ptr->lcd_id, &lcd_info) )
    {
        buff_info.bits_per_pixel = (uint16)(lcd_info.bits_per_pixel);
        buff_info.r_bitmask = (uint32)(lcd_info.r_bitmask);
        buff_info.g_bitmask = (uint32)(lcd_info.g_bitmask);
        buff_info.b_bitmask = (uint32)(lcd_info.b_bitmask);
        buff_info.width = (uint16)(lcd_info.lcd_width);
        buff_info.height = (uint16)(lcd_info.lcd_height);
        buff_info.dispmem = (uint8 *)(lcd_info.lcdbuff_ptr);
    }
    
    //Get the loop 's width and height.
    loop_width = rect.right > (buff_info.width -1) ? (buff_info.width - rect.left) : (rect.right - rect.left +1);
    loop_height = rect.bottom > (buff_info.height -1) ? buff_info.height : rect.bottom + 1;
    
    switch (buff_info.bits_per_pixel)
    {
    case 16:  //RGB565 (RGB555 is not supported currently)
        {
            uint16    *dataptr=(uint16*)(buff_info.dispmem);
            
            for (i=rect.top; i<loop_height; i++)
            {
                offset = i*buff_info.width +rect.left;
                for (j=0; j<loop_width; j++)
                {
                    *(dataptr+offset)=FASTALPHABLEND16BIT(color,*(dataptr+offset));
                    offset++;
                }
            }
        }
        break;
        
    case 24:
        {
            //add this part later.
        }
        
        break;
        
    default:
		//arvinzhang:delete assert
        //SCI_ASSERT(0); //unsupported color model
        //do nothing here
        break;
    }
    
    MMITHEME_StoreUpdateRect(dev_info_ptr, rect);
}
/*****************************************************************************/
//  Description : This function diplays the JPG image
//  Global resource dependence : none
//  Author: Xueli.lv
//  Note:   jibin modify 2004.11.18
/*****************************************************************************/
LOCAL BOOLEAN KLR_DisplayBMPImageExt(
                        GUI_LCD_DEV_INFO const *dev_info_ptr,   // block information,            
                        uint16       x_start,      // The top point x coordinate
                        uint16       y_start,      // the top point y coordinate
                        uint16       height,       // the height of the displayed image
                        uint16       width,        // the width of the displayed image
                        BOOLEAN      cut_down_flag, // indicate whether cut down the content
                                                    // of overflow the LCD; TRUE: cut down, FALSE: otherwise
                        uint8        *bmp_ptr,
                        GUI_RECT_T   *show_rect_ptr
                        )
{
    uint16      x_orig   = 0;
    uint16      y_orig   = 0;    
    uint16     *bmp_temp_ptr   = PNULL;
    uint16      i = 0;
    uint16      j = 0;
    uint16      lcd_width = 0;
    uint16      lcd_height = 0;    
    GUI_RECT_T  show_rect = {0};
    GUI_RECT_T  mainlcd_rect = {0};
    GUI_RECT_T  bmp_rect = {0};  
    GUI_COLOR_T *lcdbuff_ptr = PNULL;
    uint32 buf_width = 0;

    if (GUILCD_GetLogicWidthHeight(dev_info_ptr->lcd_id,&lcd_width,&lcd_height))
    {
        lcdbuff_ptr = GUIBLOCK_GetBuf((GUI_LCD_DEV_INFO *)dev_info_ptr);
        
        mainlcd_rect.left = 0;
        mainlcd_rect.top = 0;
        mainlcd_rect.right = (lcd_width - 1);
        mainlcd_rect.bottom = (lcd_height - 1);

        buf_width = lcd_width;
    }
	
	//arvinzhang modify:delete assert
    //SCI_ASSERT(PNULL != bmp_ptr);
    if(PNULL == bmp_ptr)
	{
		return FALSE;
	}

    bmp_rect.left = x_start;
    bmp_rect.top = y_start;
    bmp_rect.right = bmp_rect.left + width - 1;
    bmp_rect.bottom = bmp_rect.top + height - 1;
    if (PNULL != show_rect_ptr)
    {
        if (!GUI_IntersectRect(&show_rect, mainlcd_rect, *show_rect_ptr))
        {
            return FALSE;
        }
    }
    else
    {
        show_rect = mainlcd_rect;
    }
    if (!GUI_IntersectRect(&show_rect, show_rect, bmp_rect))
    {
        return FALSE;
    }
    
    bmp_temp_ptr     = (uint16*)bmp_ptr;

    
    i = show_rect.top - bmp_rect.top;  //行
    for (y_orig = show_rect.top; y_orig<=show_rect.bottom; y_orig++)
    {
        j = show_rect.left - bmp_rect.left;  //列
        for (x_orig = show_rect.left; x_orig<=show_rect.right; x_orig++)
        {

            *(lcdbuff_ptr + y_orig * buf_width + x_orig) = *(bmp_temp_ptr + width * i + j++);  

        }
        i++;    //行
        
    }
    MMITHEME_StoreUpdateRect(dev_info_ptr, show_rect);
    return TRUE;
}

/*****************************************************************************/
//  Description : This function display the bmp image.
//  Global resource dependence : none
//  Author: Xueli.lv
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN MMIKLR_DisplayBMPImageExt(
                GUI_LCD_DEV_INFO const *dev_info_ptr,   // block information,
                int16                   x_start,    // the image x coordinate
                int16                   y_start,    // the image y coordinate)
                GUI_RECT_T              *show_rect_ptr,
                GUIIMG_BITMAP_T         *bitmap_ptr
                )
{
    BOOLEAN         result = FALSE;
    uint16          *bmp_data_ptr =  PNULL;

	//arvinzhang modify:delete assert
    //SCI_ASSERT(PNULL != bitmap_ptr);
    //SCI_ASSERT(PNULL != bitmap_ptr->bit_ptr);

	if(PNULL != bitmap_ptr && PNULL != bitmap_ptr->bit_ptr)
	{
		bmp_data_ptr = (uint16*)bitmap_ptr->bit_ptr;
    	// Check the JPG image height and width

	    result = KLR_DisplayBMPImageExt(
	        dev_info_ptr,
	        x_start,
	        y_start,
	        bitmap_ptr->img_height,
	        bitmap_ptr->img_width,
	        TRUE,
	        (uint8*)bmp_data_ptr,
	        show_rect_ptr);
	}
    
    //SCI_TRACE_LOW:"[MMIKURO]: MMIKLR_DisplayBMPImageExt PMI Decode: Display JPG image staus: %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIKUR_LYRIC_364_112_2_18_2_31_38_43,(uint8*)"d", result);
    return result;
}
/*****************************************************************************/
//  Description : initialize KUR lyric
//  Global resource dependence : none
//  Author: CMZS
//  Note:
/*****************************************************************************/
PUBLIC void MMIKUR_InitLyric(void)
{
    //SCI_TRACE_LOW:"[MMIKURO]: MMIKUR_InitLyric"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIKUR_LYRIC_373_112_2_18_2_31_38_44,(uint8*)"");
    s_nLastLine1 = -1;
    s_nLastPercent1 = -1;
    s_nLastLine2 = -1;
    s_nLastPercent2 = -1;
}


/*****************************************************************************/
//  Description : stop lyric
//  Global resource dependence : 
//  Author: Jassmine
//  Note: 
/*****************************************************************************/
PUBLIC void MMIKUR_StopLyric(void)
{
    //SCI_TRACE_LOW:"[MMIKURO]: MMIKUR_StopLyric"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIKUR_LYRIC_387_112_2_18_2_31_39_45,(uint8*)"");
    //reset lyric info
    MMIKUR_InitLyric();
    //MMIKUR_ReSetPicReadInfo();
}

/*****************************************************************************/
//  Description : alloc memory for read klr.
//  Author:cmzs
//  Note: 
/*****************************************************************************/
LOCAL uint8* AllocFileDataBuf(
                                 uint32  file_size
                                 )
{
    //SCI_TRACE_LOW:"[MMIKURO]: AllocFileDataBuf, file_size = %d, s_kur_pic_buffer = %x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIKUR_LYRIC_400_112_2_18_2_31_39_46,(uint8*)"dd", file_size, s_kur_pic_buffer);
	//arvinzhang modify:delete assert
	//SCI_ASSERT(file_size <= BL_GetSize(BLOCK_MEM_POOL_KURO));
    //if(NULL != s_kur_pic_buffer)
    //{
    //   SCI_ASSERT(0);
    //}
    
#if 0    
    if(MMI_Is3DMMIEnable())
    {
        //MMI_Enable3DMMI(FALSE);             
    }
#endif
	if(file_size > BL_GetSize(BLOCK_MEM_POOL_KURO))
	{
		SCI_TRACE_LOW:"[MMIKURO]: AllocFileDataBuf fail: file_size > BL_GetSize(BLOCK_MEM_POOL_KURO"
		return NULL;
	}
	if(NULL != s_kur_pic_buffer)
	{
		SCI_TRACE_LOW:"[MMIKURO]: AllocFileDataBuf fail: NULL != s_kur_pic_buffer"
		return NULL;
	}
	
	if(NOT_USE == BL_GetStatus(BLOCK_MEM_POOL_KURO))
	{
	    s_kur_pic_buffer = MMI_BL_MALLOC(BLOCK_MEM_POOL_KURO);

		//SCI_ASSERT(NULL != s_kur_pic_buffer);
		if(NULL != s_kur_pic_buffer)
		{
	        SCI_MEMSET(s_kur_pic_buffer, 0, BL_GetSize(BLOCK_MEM_POOL_KURO));
		}
		return s_kur_pic_buffer;
	}
	else
	{
		SCI_TRACE_LOW:"[MMIKURO]: AllocFileDataBuf fail: NOT_USE != BL_GetStatus(BLOCK_MEM_POOL_KURO)"
		return NULL;
	}
	
    
}

/*****************************************************************************/
//  Description : Free buffer for picture display
//  Global resource dependence : 
//  Author:cmzs
//  Note: 
/*****************************************************************************/
PUBLIC void MMIKUR_FreeKlrBuffer()
{
    //SCI_TRACE_LOW:"[MMIKURO]: MMIKUR_FreeKlrBuffer"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIKUR_LYRIC_433_112_2_18_2_31_39_47,(uint8*)"");
       
     if(NULL != s_kur_pic_buffer)
    {
        MMI_BL_FREE(BLOCK_MEM_POOL_KURO);
        s_kur_pic_buffer = NULL;
        s_bitmap.bit_ptr = NULL;
    }
    if(PNULL != s_lyric_buf_line1)
    {
        SCI_FREE(s_lyric_buf_line1);
        s_lyric_buf_line1 = PNULL;
    }
    if(PNULL != s_lyric_buf_line2)
    {
        SCI_FREE(s_lyric_buf_line2);
        s_lyric_buf_line2 = PNULL;
    }
    MMIKLR_ReleaseLrc();

    //MMI_Enable3DMMI(TRUE);

}

/*****************************************************************************/
//  Description : alloc buffer for picture display
//  Global resource dependence : 
//  Author:cmzs
//  Note: 
/*****************************************************************************/
PUBLIC void MMIKUR_AllocKlrBuffer()
{
    //SCI_TRACE_LOW:"[MMIKURO]: MMIKUR_AllocKlrBuffer"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIKUR_LYRIC_463_112_2_18_2_31_39_48,(uint8*)"");
    s_bitmap.img_width = LCD_HEIGHT;
    s_bitmap.img_height = LCD_WIDTH;
    if(s_kur_pic_buffer == NULL)
    {   
        s_kur_pic_buffer = AllocFileDataBuf(2*KLR_PIC_DATASIZE);
    }
	if(NULL != s_kur_pic_buffer)
	{
	    s_bitmap.bit_ptr = s_kur_pic_buffer;
	    s_kur_pic_buffer_next = s_kur_pic_buffer + KLR_PIC_DATASIZE;

	    if(PNULL == s_lyric_buf_line1)
	    {
	        s_lyric_buf_line1 = SCI_ALLOC_APP(LYRIC_BUFFER_SIZE);
	    }
	    if(PNULL == s_lyric_buf_line2)
	    {
	        s_lyric_buf_line2 = SCI_ALLOC_APP(LYRIC_BUFFER_SIZE);
	    }
	}
}
/*****************************************************************************/
//  Description : 根据歌手获取歌词显示颜色
//  Global resource dependence : 
//  Author:CMZS
//  Note: 
/*****************************************************************************/
LOCAL GUI_COLOR_T GetLyricColor(int32 nSex)
{
    if (nSex == GENDER_FEMALE)
    {    
        return MMI_RED_COLOR;
    }
    else if (nSex == GENDER_ALL)
    {    
        return MMI_GREEN_COLOR;
    }
    else
    {    
        return MMI_PURPLE_COLOR;
    }
}
/*****************************************************************************/
//  Description : 将一行歌词显示到buffer中（获取歌词点阵)
//  Global resource dependence : 
//  Author:CMZS
//  Note: 
/*****************************************************************************/
LOCAL void DrawLyricToBuffer(const wchar* szLyric, uint16 *pdspbuffer,uint16 nbufferwidth,uint16 nbufferheight,GUI_COLOR_T color)
{

    GUI_RECT_T          rcBuffer = {0,0,0,0};    
    uint32              nLyricLength = MMIAPICOM_Wstrlen((wchar const *)szLyric) * 2;
    GUISTR_STYLE_T      text_style = {0};
    MMI_STRING_T        str_data = {0};
    GUISTR_STATE_T      state = GUISTR_STATE_ANGLE|GUISTR_STATE_WORDBREAK;
    GUISTR_BUF_T        des_buf = {0};
   
    rcBuffer.right = nbufferwidth;
    rcBuffer.bottom = nbufferheight;
    if(nLyricLength > 0 )
    {
        // text style
        text_style.align = ALIGN_LEFT;
        text_style.angle = ANGLE_90;
        text_style.font = MMI_DEFAULT_TEXT_FONT;
        text_style.font_color = color;
        text_style.char_space = 0;
        text_style.line_space = 0;

        // text buffer
        des_buf.buffer = pdspbuffer;
        des_buf.height = nbufferheight;
        des_buf.mem_width = des_buf.width = nbufferwidth;

        // text str
        str_data.wstr_len = nLyricLength;
        str_data.wstr_ptr = szLyric;
        GUISTR_DrawTextInRect( 
            &des_buf,
            &rcBuffer,
            &rcBuffer,
            &str_data,
            &text_style,
            state,
            GUISTR_TEXT_DIR_AUTO
            );     
    }
}

/*****************************************************************************/
//  Description : display one line lyric from font buffer
//  Global resource dependence : 
//  Author:CMZS
//  return:所画到的位置
//  Note: 
/*****************************************************************************/
LOCAL int32 DrawBufferLyric(GUI_LCD_DEV_INFO* pLcdDevInfo, GUI_RECT_T rectLyric, uint16* lyric_font_buf, int32 nPercent,int nLastPos,int nLyricFontLength,GUI_COLOR_T color)
{
    IMG_DISPCTL_T   buf_info = {0};
    GUI_RECT_T rcdsp = {0},rcBuffer = {0},rcUpdate = {0};
    int32 x = 0, y = 0;
    uint16 *plcdbuffer = NULL;
    uint16 *plinebuffer = {0},*plinelcd = {0};
    uint32 nFontLength = nLyricFontLength;
    LCD_INFO_T      lcd_info = {0};

    rcBuffer.left = 0;
    rcBuffer.top = 0;
    rcBuffer.right = rectLyric.right - rectLyric.left;
    rcBuffer.bottom = rectLyric.bottom - rectLyric.top;

    if (GUILCD_GetInfo(pLcdDevInfo->lcd_id, &lcd_info) )
    {
        buff_info.bits_per_pixel = (uint16)(lcd_info.bits_per_pixel);
        buff_info.r_bitmask = (uint32)(lcd_info.r_bitmask);
        buff_info.g_bitmask = (uint32)(lcd_info.g_bitmask);
        buff_info.b_bitmask = (uint32)(lcd_info.b_bitmask);
        buff_info.width = (uint16)(lcd_info.lcd_width);
        buff_info.height = (uint16)(lcd_info.lcd_height);
        buff_info.dispmem = (uint8 *)(lcd_info.lcdbuff_ptr);
    }

    plcdbuffer = (uint16*)buf_info.dispmem;
    
    rcdsp = rcBuffer;
    rcdsp.bottom = nPercent * (int32)nFontLength / 100; //因为取得的字符FONTSIZE是中文字符的SIZE，所以要除以2
    rcUpdate = rectLyric;

    if(rcdsp.top < 0)
    {    
        rcdsp.top = 0;
    }
    if(rcdsp.bottom > 310)
    {
        rcdsp.bottom = 310;
    }

    if(rcdsp.bottom > nLastPos)
    {
        rcUpdate.top = rcUpdate.top + nLastPos;
        rcUpdate.bottom = rcUpdate.top + rcdsp.bottom - nLastPos;

        for(y = nLastPos; y < rcdsp.bottom; y++)
        {
            plinebuffer = lyric_font_buf + y * rcBuffer.right;
            plinelcd = plcdbuffer + (rectLyric.top + y) * buf_info.width + rectLyric.left;
            for(x = 0; x < rcdsp.right; x++)
            {
                if(plinebuffer[x] > 0)
                {                
                    plinelcd[x] = color;
                }
            }
        }
        MMITHEME_StoreUpdateRect(pLcdDevInfo,rcUpdate);
    }


    return rcdsp.bottom;
}

/*****************************************************************************/
//  Description : display klr lyric,will be call by MMIKUR_DisplayPlayAnim
//  Global resource dependence : 
//  Author:CMZS
//  Note: 
/*****************************************************************************/
LOCAL void DisplayKlrLyric(uint32 ncurtime,BOOLEAN bPicChanged,KLR_DSP_MODE nMode)
{
      
    GUI_LCD_DEV_INFO    lcd_dev_info = {GUI_LCD_0, GUI_BLOCK_0};
    GUI_RECT_T          rectLyric1 = {25,5,25 + KLR_RECT_WIDTH,5 + KLR_RECT_HEIGHT};
    GUI_RECT_T          rectLyric2 = {0,5,KLR_RECT_WIDTH,5 + KLR_RECT_HEIGHT};
    char* line1 = NULL;
    char* line2 = NULL;
    uint16 lcd_width = LCD_WIDTH,font_height = KLR_FONT_HEIGHT,toolbar_width = KLR_TOOLBAR_HEIGHT;
    wchar wline1[KLR_LINE_MAXLENGTH] = {0},wline2[KLR_LINE_MAXLENGTH] = {0};
    int32 nLine1=0,nPercent1=0,nSex1=0,nLine2=0,nPercent2=0,nSex2=0,nlinelen = 0;
    GUI_COLOR_T color = MMI_WHITE_COLOR;

    lcd_dev_info.lcd_id   = GUI_MAIN_LCD_ID;
    lcd_dev_info.block_id = GUI_BLOCK_MAIN;

//get lyric
    if(KLR_MODE_TWOLINE == nMode)
    {    
        MMIKLR_ShowLyric2(ncurtime ,&nLine1, &nPercent1, &nSex1, &nLine2, &nPercent2, &nSex2);
    }
    else if(KLR_MODE_MUTILINE == nMode)
    {
        MMIKLR_ShowLyric3(ncurtime ,&nLine1, &nPercent1, &nSex1, &nLine2, &nPercent2, &nSex2);
    }
/*
    SCI_TRACE_LOW("[MMIKURO]: DisplayKlrLyric,ncurtime = %d,bPicChanged = %d,nMode = %d,nLine1 = %d,nPercent1 = %d,nSex1 = %d,nLine2 = %d,nPercent2 = %d, nSex2 = %d",
        ncurtime,bPicChanged,nMode,nLine1,nPercent1,nSex1,nLine2,nPercent2,nSex2); 
    //SCI_TRACE_LOW:"[MMIKURO]: DisplayKlrLyric,s_nLastLine1 = %d,s_nLastPercent1 = %d,s_nLastLine2 = %d,s_nLastPercent2 = %d,s_nLastDisplayPos1 = %d,s_nLastDisplayPos2 = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIKUR_LYRIC_649_112_2_18_2_31_39_49,(uint8*)"dddddd",s_nLastLine1,s_nLastPercent1,s_nLastLine2,s_nLastPercent2,s_nLastDisplayPos1,s_nLastDisplayPos2);
*/

    // draw lyric
    if (nLine1 >= 0)
    {
        if(nMode == KLR_MODE_MUTILINE)
        {
            rectLyric1.left = lcd_width - toolbar_width - font_height - (nLine1 % KLR_LINES_PER_PAGE) * 30;
            rectLyric1.right = rectLyric1.left + font_height;
        }

        //如果是相同串，则不用再显示
        if(s_nLastLine1 != nLine1 || bPicChanged) //new line
        {
            line1 = (char*)MMIKLR_GetLyric(nLine1);
            s_nLastDisplayPos1 = 0;
            if (line1 != NULL)// && (s_nLastLine1 != nLine1 || s_nLastPercent1 != nPercent1 || bPicChanged) )
            {
                if(!bPicChanged)
                {
                    MMIKLR_DisplayBMPImageExt(&lcd_dev_info, 0, 0,&rectLyric1, &s_bitmap);
                    KLR_MistBmp(&lcd_dev_info,rectLyric1,0);
                }

                nlinelen = strlen(line1);
                if(nlinelen > KLR_LINE_MAXLENGTH)
                    nlinelen = KLR_LINE_MAXLENGTH;
                SCI_MEMSET(wline1,0,(sizeof(wchar) * KLR_LINE_MAXLENGTH));
                
                s_nFontLength1 = GetStrFontLength(line1,MMI_DEFAULT_TEXT_FONT);                
                GUI_GBToWstr(wline1, (uint8*)line1, (uint16)nlinelen);
                        
                SCI_MEMSET(s_lyric_buf_line1,0,LYRIC_BUFFER_SIZE);
                DrawLyricToBuffer(wline1,s_lyric_buf_line1,KLR_RECT_WIDTH,KLR_RECT_HEIGHT,color);
                DrawBufferLyric(&lcd_dev_info,rectLyric1,s_lyric_buf_line1,100,0,s_nFontLength1,MMI_WHITE_COLOR);              
                color = GetLyricColor(nSex1);
                if(bPicChanged)
                {                
                    s_nLastDisplayPos1 = 0;
                }          

                s_nLastDisplayPos1 =DrawBufferLyric(&lcd_dev_info,rectLyric1,s_lyric_buf_line1,nPercent1,0,s_nFontLength1,color);
                
                s_nLastLine1 = nLine1;
                s_nLastPercent1 = nPercent1;
            }
        }
        else
        {
            color = GetLyricColor(nSex1);
            s_nLastDisplayPos1 =DrawBufferLyric(&lcd_dev_info,rectLyric1,s_lyric_buf_line1,nPercent1,s_nLastDisplayPos1,s_nFontLength1,color);
       
        }
    }

        
    if (nLine2 >= 0)
    {
        if(nMode == 2)
        {
            rectLyric2.left = lcd_width - toolbar_width - font_height - (nLine2 % 6) * 30;
            rectLyric2.right = rectLyric2.left + font_height;
        }

        if(s_nLastLine2 != nLine2 || bPicChanged) //new line
        {
            line2 = (char*)MMIKLR_GetLyric(nLine2);
            s_nLastDisplayPos2 = 0;
        //如果是相同串，则不用再显示
            if (line2 != NULL)
            {
                nlinelen = strlen(line2);
                if(nlinelen > KLR_LINE_MAXLENGTH)
                    nlinelen = KLR_LINE_MAXLENGTH;
                SCI_MEMSET(wline2,0,(sizeof(wchar) * KLR_LINE_MAXLENGTH));
                
                if(!bPicChanged)
                {
                    MMIKLR_DisplayBMPImageExt(&lcd_dev_info, 0, 0,&rectLyric2, &s_bitmap);
                    KLR_MistBmp(&lcd_dev_info,rectLyric2,0);
                }

                s_nFontLength2 = GetStrFontLength(line2,MMI_DEFAULT_TEXT_FONT);                
                GUI_GBToWstr(wline2, (uint8*)line2, (uint16)nlinelen);
                
                SCI_MEMSET(s_lyric_buf_line2,0,LYRIC_BUFFER_SIZE);
                DrawLyricToBuffer(wline2,s_lyric_buf_line2,KLR_RECT_WIDTH,KLR_RECT_HEIGHT,color);
                DrawBufferLyric(&lcd_dev_info,rectLyric2,s_lyric_buf_line2,100,0,s_nFontLength2,MMI_WHITE_COLOR);             
                color = GetLyricColor(nSex2);
                if(bPicChanged)
                {
                    s_nLastDisplayPos2 = 0;
                }
                s_nLastDisplayPos2 =DrawBufferLyric(&lcd_dev_info,rectLyric2,s_lyric_buf_line2,nPercent2,0,s_nFontLength2,color);
           
                s_nLastLine2 = nLine2;
                s_nLastPercent2 = nPercent2;
            }
        }
        else
        {
            color = GetLyricColor(nSex2);      
            s_nLastDisplayPos2 =DrawBufferLyric(&lcd_dev_info,rectLyric2,s_lyric_buf_line2,nPercent2,s_nLastDisplayPos2,s_nFontLength2,color);
        }
    }

}
/*****************************************************************************/
//  Description : Hide toolbar
//  Global resource dependence : 
//  Author:cmzs
//  Note: 
/*****************************************************************************/
PUBLIC void MMIKLR_HideToolBar()
{
    if(!s_klr_pic_readinfo.is_first_pic_read)
    {
        return;
    }
    if(s_kur_bdisptoolbar)
    {
        GUI_RECT_T rcTooBar = {192,0,239,320};
        GUI_LCD_DEV_INFO    lcd_dev_info = {GUI_LCD_0, GUI_BLOCK_0};

        MMIKLR_DisplayBMPImageExt(&lcd_dev_info, 0,0,&rcTooBar, &s_bitmap);
        if(s_klr_dispmode == KLR_MODE_MUTILINE)
        {       
            KLR_MistBmp(&lcd_dev_info,rcTooBar,0);
        }
    }
    s_kur_bdisptoolbar = FALSE;
}

/*****************************************************************************/
//  Description : Display toolbar
//  Global resource dependence : 
//  Author:cmzs
//  Note: 
/*****************************************************************************/
PUBLIC void MMIKLR_DisplayToolBar(BOOLEAN bRefresh)
{
    GUI_POINT_T         dis_point = {0};
    GUI_LCD_DEV_INFO    lcd_dev_info = {GUI_LCD_0, GUI_BLOCK_0};

    if(!s_klr_pic_readinfo.is_first_pic_read)
    {
        return;
    }
    //set lcd dev
    lcd_dev_info.lcd_id   = GUI_MAIN_LCD_ID;
    lcd_dev_info.block_id = GUI_BLOCK_MAIN;

    if(MMK_IsFocusWin(MMIKUR_MAIN_PLAY_WIN_ID))
    {
        GUI_RECT_T rcMode[] ={ {217,285,232,301},
                                {198,276,213,292},
                                {198,295,213,311},
                               };
        
        if(MMIKUR_GetAudioState() != MMIKUR_AUDIO_PLAYING)
        {   
            dis_point.x = 192;
            dis_point.y = 0;
            GUIRES_DisplayImg(&dis_point,
                    PNULL,
                    PNULL,
                    MMIKUR_MAIN_PLAY_WIN_ID,
                    KURO_TOOLBAR_PLAY,
                    &lcd_dev_info);
        }
        else
        {     
            dis_point.x = 192;
            dis_point.y = 0;
            GUIRES_DisplayImg(&dis_point,
                    PNULL,
                    PNULL,
                    MMIKUR_MAIN_PLAY_WIN_ID,
                    KURO_TOOLBAR_PAUSE,
                    &lcd_dev_info);
        }        

        dis_point.x = 192;
        dis_point.y = 36;
        GUIRES_DisplayImg(&dis_point,
                PNULL,
                PNULL,
                MMIKUR_MAIN_PLAY_WIN_ID,
                KURO_TOOLBAR_OTHER,
                &lcd_dev_info);
        LCD_DrawRect(&lcd_dev_info,rcMode[s_klr_dispmode],0xF800);
        
    }
    s_kur_bdisptoolbar = TRUE;
}

/*****************************************************************************/
//  Description : change Lyric Display Mode
//  Global resource dependence : 
//  Author:cmzs
//  Note: 
/*****************************************************************************/
PUBLIC void MMIKUR_GoToNextLyricMode()
{
    s_klr_dispmode++;
    s_klr_dispmode = s_klr_dispmode % KLR_MODE_MAX;
    MMIKUR_DisplayKlr(MMIKUR_MAIN_PLAY_WIN_ID,TRUE,s_kur_bdisptoolbar,s_klr_dispmode);
}

/*****************************************************************************/
//  Description : 背景图片读取处理函数
//  Global resource dependence : 
//  Author: xingdong.li
//  Note:
//  Return: 
/*****************************************************************************/
PUBLIC void MMIKUR_HandleReadPicMsg(MMI_WIN_ID_T win_id)
{
    MMIKLR_GetPicData_BlockByIndex(s_klr_pic_readinfo.pic_reading_index,
        s_klr_pic_readinfo.pic_read_buf_current,KLR_PIC_DATASIZE, KLR_PIC_DATASIZE,&s_klr_pic_readinfo.pic_read_finished);
    if(!s_klr_pic_readinfo.pic_read_finished)
    {
        MMK_PostMsg(win_id, MSG_KUR_START_READ_PIC, PNULL, NULL);
    }
    else
    {
        //SCI_TRACE_LOW:"[MMIKURO]: MSG_KUR_START_READ_PIC finished !!! pic_index = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIKUR_LYRIC_868_112_2_18_2_31_39_50,(uint8*)"d",s_klr_pic_readinfo.pic_reading_index);
    }
}

/*****************************************************************************/
//  Description : display klr lyric
//  Global resource dependence : 
//  Author:cmzs
//  Note: 
/*****************************************************************************/
PUBLIC void MMIKUR_DisplayKlr(MMI_WIN_ID_T win_id,BOOLEAN bRepaintAll,BOOLEAN bDspToolBar,KLR_DSP_MODE nMode)
{
    //SCI_TRACE_LOW:"[MMIKURO]: MMIKUR_DisplayKlr, bRepaintAll = %d,bDspToolBar = %d,nMode = %d,player_state = %d, audio_state = %d,read_finish = %d,is_first_read = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIKUR_LYRIC_879_112_2_18_2_31_39_51,(uint8*)"ddddddd",bRepaintAll,bDspToolBar,nMode,MMIKUR_GetPlayerState(),MMIKUR_GetAudioState(),s_klr_pic_readinfo.pic_read_finished,s_klr_pic_readinfo.is_first_pic_read);
    if(!s_klr_pic_readinfo.is_first_pic_read)
    {
        return;
    }
    if (MMK_IsFocusWin(win_id))
    {
       
        if(MMIKUR_GetAudioState() == MMIKUR_AUDIO_PLAYING || bRepaintAll)
        {
            uint32 nCurTime = MMIKURO_GetKurPlayedTime();
            int32 nNextPicIndex = 0;
            int32 nCurPicIndex = 0;
            BOOLEAN bPicChanged = FALSE;            
            uint32 nPicCount = 0;
            if(MMIKUR_GetAudioState() == MMIKUR_AUDIO_STOPPED)
            {            
                nCurTime = 0;
            }
            if(KLR_MODE_MUTILINE == nMode)
            {      
               
                if(s_klr_pic_readinfo.pic_dsp_index < 0)
                {
                    if(s_klr_pic_readinfo.pic_read_finished)
                    {
                       
                        bPicChanged = TRUE;
                        if(s_klr_pic_readinfo.pic_reading_index >= 0)
                        {
                            s_bitmap.bit_ptr = s_klr_pic_readinfo.pic_read_buf_current;
                            s_klr_pic_readinfo.pic_dsp_index = s_klr_pic_readinfo.pic_reading_index;
                        }
                    }
                }
            }
            else
            {
                nCurPicIndex = MMIKLR_GetPicIndex(nCurTime,&nNextPicIndex,&nPicCount); 
               
                //SCI_TRACE_LOW("[MMIKURO]: MMIKUR_DisplayKlr, nCurPicIndex = %d,pic_dsp_index = %d, pic_reading_index = %d, pic_readed_index1 = %d, pic_readed_index2 = %d,read_finish = %d",
                //    nCurPicIndex,s_klr_pic_readinfo.pic_dsp_index,s_klr_pic_readinfo.pic_reading_index,s_klr_pic_readinfo.pic_readed_index1,s_klr_pic_readinfo.pic_readed_index2,s_klr_pic_readinfo.pic_read_finished);
            
               if(s_klr_pic_readinfo.pic_read_finished && MMIKUR_AUDIO_STOPPED != MMIKUR_GetAudioState())
               {
                   int nReadIndex = nCurPicIndex;
                   if(s_klr_pic_readinfo.pic_reading_index >= 0)
                   {
                       if(s_klr_pic_readinfo.pic_read_buf_current == s_klr_pic_readinfo.pic_read_buf1)
                       {                      
                           s_klr_pic_readinfo.pic_readed_index1 = s_klr_pic_readinfo.pic_reading_index;
                       }
                       else
                       {                       
                           s_klr_pic_readinfo.pic_readed_index2 = s_klr_pic_readinfo.pic_reading_index;
                       }
                   }

                   if(nCurPicIndex == s_klr_pic_readinfo.pic_readed_index1 || nCurPicIndex == s_klr_pic_readinfo.pic_readed_index2)
                   {
                       nReadIndex = nNextPicIndex;
                   }

                   if(nReadIndex >= 0 && nReadIndex != s_klr_pic_readinfo.pic_readed_index1 
                       && nReadIndex != s_klr_pic_readinfo.pic_readed_index2)
                   {
                           s_klr_pic_readinfo.pic_reading_index = nReadIndex;
                           if(s_klr_pic_readinfo.pic_read_buf_current == s_klr_pic_readinfo.pic_read_buf1)
                           {                          
                               s_klr_pic_readinfo.pic_read_buf_current = s_klr_pic_readinfo.pic_read_buf2;
                           }
                           else
                           {                           
                               s_klr_pic_readinfo.pic_read_buf_current = s_klr_pic_readinfo.pic_read_buf1;
                           }
                           s_klr_pic_readinfo.pic_read_finished = FALSE;
                           MMK_PostMsg(win_id, MSG_KUR_START_READ_PIC, PNULL, NULL);
                   }
               }              
            }             

           if(s_klr_pic_readinfo.pic_dsp_index != nCurPicIndex)
           {
               if(MMIKUR_GetAudioState() == MMIKUR_AUDIO_STOPPED)
               {
                   BOOLEAN bAllLoaded = FALSE;
                   MMIKLR_GetPicData_BlockByIndex(nCurPicIndex,s_klr_pic_readinfo.pic_read_buf1,KLR_PIC_DATASIZE,KLR_PIC_DATASIZE,&bAllLoaded);
                   s_klr_pic_readinfo.pic_reading_index = nCurPicIndex;
                   s_klr_pic_readinfo.pic_read_finished = TRUE;
                   s_klr_pic_readinfo.pic_readed_index1 = nCurPicIndex;
               }
               if(nCurPicIndex == s_klr_pic_readinfo.pic_readed_index1 )
               {
                   bPicChanged = TRUE;
                   s_bitmap.bit_ptr = s_klr_pic_readinfo.pic_read_buf1;
                   s_klr_pic_readinfo.pic_dsp_index = nCurPicIndex;
               }
               if(nCurPicIndex == s_klr_pic_readinfo.pic_readed_index2 )
               {
                   bPicChanged = TRUE;
                   s_bitmap.bit_ptr = s_klr_pic_readinfo.pic_read_buf2;
                   s_klr_pic_readinfo.pic_dsp_index = nCurPicIndex;
               }
               //SCI_TRACE_LOW:"[MMIKURO]: MMIKUR_DisplayKlr, nCurPicIndex = %d,pic_dsp_index = %d, pic_reading_index = %d, pic_readed_index1 = %d, pic_readed_index2 = %d,read_finish = %d, bPicChanged = %d"
               SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIKUR_LYRIC_983_112_2_18_2_31_40_52,(uint8*)"ddddddd",nCurPicIndex,s_klr_pic_readinfo.pic_dsp_index,s_klr_pic_readinfo.pic_reading_index,s_klr_pic_readinfo.pic_readed_index1,s_klr_pic_readinfo.pic_readed_index2,s_klr_pic_readinfo.pic_read_finished,bPicChanged);
           }

            //display picture
            if(bPicChanged || bRepaintAll) //如果图片改变了,要重画整个图片.
            {
                GUI_LCD_DEV_INFO    lcd_dev_info = {GUI_LCD_0, GUI_BLOCK_0};
                if(s_bitmap.bit_ptr != PNULL)
                {               
                    MMIKLR_DisplayBMPImageExt(&lcd_dev_info, 0, 0, PNULL,&s_bitmap);                    
                }

                if(nMode == KLR_MODE_MUTILINE)
                {
                    GUI_RECT_T rcLcd = {0,0,LCD_WIDTH,LCD_HEIGHT};
                    KLR_MistBmp(&lcd_dev_info,rcLcd,0);
                }
                else if(nMode == KLR_MODE_TWOLINE)
                {                   
                    GUI_RECT_T rcLyric = {0,0,50,LCD_HEIGHT};    
                    KLR_MistBmp(&lcd_dev_info,rcLyric,0);
                   
                }
                if(s_kur_bdisptoolbar)
                {               
                    MMIKLR_DisplayToolBar(TRUE);
                }
                if(s_kur_bdispvolume)
                {
                    MMIKLR_DisplayKurVolumeWin(TRUE);
                }
            }
            if(nMode == KLR_MODE_TWOLINE || nMode == KLR_MODE_MUTILINE)
            {            
                DisplayKlrLyric(nCurTime,bPicChanged | bRepaintAll,nMode);
            }
            //  播放30秒后计数
            if (nCurTime >= KUR_COUNTER_TIME)
            {            
                MMIKUR_AddKgcCount();
            }

        }
    }
}

/*****************************************************************************/
//  Description : display kuro volume window
//  Global resource dependence : 
//  Author:cmzs
//  Note: 
/*****************************************************************************/
PUBLIC void MMIKLR_DisplayKurVolumeWin(BOOLEAN bRefresh)
{
    uint cur_volume = MMIKUR_GetVolume();

    if(!s_klr_pic_readinfo.is_first_pic_read)
    {
        return;
    }
    MMIKLR_HideToolBar();
    if(!s_kur_bdispvolume || bRefresh)
    {
        MMI_WIN_ID_T win_id = MMIKUR_MAIN_PLAY_WIN_ID;
        GUI_POINT_T ptDsp = {190,90};
        GUI_LCD_DEV_INFO    lcd_dev_info = {GUI_LCD_0, GUI_BLOCK_0};
        GUI_RECT_T rcVolume = {206,133,211,198};
        uint16 nVolumeHeight = rcVolume.bottom - rcVolume.top;
        //set lcd dev
        lcd_dev_info.lcd_id   = GUI_MAIN_LCD_ID;
        lcd_dev_info.block_id = GUI_BLOCK_MAIN; 

        //display volume bg
        IMG_EnableTransparentColor(TRUE);
        //display volume bg
        GUIRES_DisplayImg(&ptDsp,
                PNULL,
                PNULL,
                win_id,
                IMAGE_KURO_VOLUME_BG,
                &lcd_dev_info);
        if(0 < cur_volume)
        {
            nVolumeHeight = nVolumeHeight * cur_volume/ MMISET_VOL_MAX;
            rcVolume.top = rcVolume.bottom - nVolumeHeight;
            LCD_FillRect(&lcd_dev_info,rcVolume,0xF800);
        }
        
        IMG_EnableTransparentColor(FALSE);

        s_kur_bdispvolume = TRUE;
    }
}
/*****************************************************************************/
//  Description : display kuro volume window
//  Global resource dependence : 
//  Author:cmzs
//  Note: 
/*****************************************************************************/
PUBLIC void MMIKLR_HideKurVolumeWin()
{
    if(!s_klr_pic_readinfo.is_first_pic_read)
    {
        return;
    }
    if(s_kur_bdispvolume)
    {
        GUI_RECT_T rcVolume = {190,90,230,230};
        GUI_LCD_DEV_INFO    lcd_dev_info = {GUI_LCD_0, GUI_BLOCK_0};
        MMIKLR_DisplayBMPImageExt(&lcd_dev_info, 0,0,&rcVolume, &s_bitmap);
        if(s_klr_dispmode == KLR_MODE_MUTILINE)
        {        
            KLR_MistBmp(&lcd_dev_info,rcVolume,0);
        }
    }
    s_kur_bdispvolume = FALSE;
}

/*****************************************************************************/
//  Description : 重置图片读取信息
//  Global resource dependence : 
//  Author: songbin.zeng
//  Note:
//  Return: 
/*****************************************************************************/
PUBLIC void MMIKUR_ReSetPicReadInfo(void)
{
    //SCI_TRACE_LOW:"[MMIKURO]: MMIKUR_ReSetPicReadInfo"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIKUR_LYRIC_1104_112_2_18_2_31_40_53,(uint8*)"");
    s_klr_pic_readinfo.pic_read_buf1 = s_kur_pic_buffer;
    s_klr_pic_readinfo.pic_read_buf2 = s_kur_pic_buffer + KLR_PIC_DATASIZE;
    s_klr_pic_readinfo.pic_read_buf_current = s_kur_pic_buffer;
    s_klr_pic_readinfo.pic_read_finished = FALSE;
    s_klr_pic_readinfo.pic_readed_index1 = -1;
    s_klr_pic_readinfo.pic_readed_index2 = -1;
    s_klr_pic_readinfo.pic_reading_index = -1;
    s_klr_pic_readinfo.pic_dsp_index = -1;
    s_klr_pic_readinfo.is_first_pic_read = FALSE;
}

/*****************************************************************************/
//  Description : 读入第一张图片(一次读入)
//  Global resource dependence : 
//  Author: songbin.zeng
//  Note:
//  Return: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIKUR_ReadFirstPic()
{
    //这里会设置为finish == 1
    BOOLEAN bAllLoaded = FALSE;
    uint32 nCurTime = MMIKURO_GetKurPlayedTime();
    uint32 nPicIndex = 0;
    int32 nNextIndex = 0;
    uint32 nPicCount = 0;
    BOOLEAN result = FALSE;
    nPicIndex = MMIKLR_GetPicIndex(nCurTime,&nNextIndex,&nPicCount);

    if(nPicIndex <= nPicCount &&
       0 < nPicCount)
    {
        if(MMIKLR_GetPicData_BlockByIndex(nPicIndex,s_klr_pic_readinfo.pic_read_buf_current,KLR_PIC_DATASIZE,KLR_PIC_DATASIZE,&bAllLoaded))
        {
            if(bAllLoaded)
            {
                s_klr_pic_readinfo.pic_reading_index = nPicIndex;
                s_klr_pic_readinfo.pic_read_finished = TRUE;
                s_klr_pic_readinfo.is_first_pic_read = TRUE;
                result = TRUE;
            }
        }
    }
    //SCI_TRACE_LOW:"[MMIKURO]: MMIKUR_ReadFirstPic, nPicIndex = %d, nPicCount = %d, result = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIKUR_LYRIC_1146_112_2_18_2_31_40_54,(uint8*)"ddd",nPicIndex,nPicCount,result);
    return result;
}
/*****************************************************************************/
//  Description : 歌词播放模式
//  Global resource dependence : 
//  Author: apple.zhang
//  Note:
//  Return: 
/*****************************************************************************/
PUBLIC KLR_DSP_MODE MMIKUR_GetKlrMode()
{
    return s_klr_dispmode;
}
/*****************************************************************************/
//  Description : 音量调节面板是否显示
//  Global resource dependence : 
//  Author: apple.zhang
//  Note:
//  Return: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIKUR_GetbdispVolume()
{
    return s_kur_bdispvolume;
}
/*****************************************************************************/
//  Description : 控制调节面板是否显示
//  Global resource dependence : 
//  Author: apple.zhang
//  Note:
//  Return: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIKUR_Getbdisptoolbar()
{
    return s_kur_bdisptoolbar;
}
/*****************************************************************************/
//  Description : 设置控制调节面板是否显示
//  Global resource dependence : 
//  Author: apple.zhang
//  Note:
//  Return: 
/*****************************************************************************/
PUBLIC void MMIKUR_Setbdisptoolbar(BOOLEAN is_bdisptoolbar)
{
    s_kur_bdisptoolbar = is_bdisptoolbar;
}
/*****************************************************************************/
//  Description : 背景图片是否读取OK
//  Global resource dependence : 
//  Author: xingdong.li
//  Note:
//  Return: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIKUR_IsBackgroundReady(void)
{
    //SCI_TRACE_LOW:"[MMIKURO]: MMIKUR_IsBackgroundReady, result = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIKUR_LYRIC_1192_112_2_18_2_31_40_55,(uint8*)"d",s_klr_pic_readinfo.is_first_pic_read);
    return s_klr_pic_readinfo.is_first_pic_read;
}
#endif
