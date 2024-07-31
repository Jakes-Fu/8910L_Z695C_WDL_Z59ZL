/*
 *
 * Copyright  1990-2009 Sun Microsystems, Inc. All Rights Reserved.
 * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER
 * 
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License version
 * 2 only, as published by the Free Software Foundation.
 * 
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License version 2 for more details (a copy is
 * included at /legal/license.txt).
 * 
 * You should have received a copy of the GNU General Public License
 * version 2 along with this work; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA
 * 02110-1301 USA
 * 
 * Please contact Sun Microsystems, Inc., 4150 Network Circle, Santa
 * Clara, CA 95054 or visit www.sun.com if you need additional
 * information or have any questions.
 */


#ifdef __cplusplus
extern "C" {
#endif
    

#include "javacall_font.h"
//#include "guifont.h"
#include "mmi_modu_main.h"
#include "javacall_logging.h"
#include "guistring.h"

#define JAVA_CR_CHAR 0x000d
#define JAVA_LF_CHAR 0x000a

typedef struct {
    javacall_font_face   face;
    javacall_font_style  style;
    javacall_font_size   size;
    //GUI_FONT_T   font_num;
}FONT_PRO; 

static FONT_PRO  JavaCallFont;
/* hanchunyuan 添加欧元符号begin */
static uint8 g_euromoney_font14[] = {
							 0x00, 0x00, 0x07, 0x80, 0x08, 0x80, 0x10, 0x40,
							 0x10, 0x00, 0x3f, 0x00, 0x10, 0x00, 0x3f, 0x00, 
							 0x10, 0x00, 0x10, 0x40, 0x10, 0x40, 0x08, 0x80, 
							 0x07, 0x00, 0x00, 0x00
							 };


static uint8 g_euromoney_font16[] = {0x00, 0x00,
							 0x00, 0x00, 0x07, 0x80, 0x08, 0x80, 0x10, 0x40,
							 0x10, 0x00, 0x3f, 0x00, 0x10, 0x00, 0x3f, 0x00, 
							 0x10, 0x00, 0x10, 0x40, 0x10, 0x40, 0x08, 0x80, 
							 0x07, 0x00, 0x00, 0x00, 
							 0x00, 0x00
							 };
/* hanchunyuan 添加欧元符号end */
#if 0
/**********************************************************************************/
/* return relative coordinate to top-left box */
static void font_MapPixelReverse(int16        *i_ptr,      //相对坐标
                     int16        *j_ptr,       //相对坐标
                     uint16       *width_ptr,
                     uint16       *height_ptr,
                     int16        x, 
                     int16        y, 
                     GUI_RECT_T   box  )
{
        *i_ptr = x - box.left;
        *j_ptr = y - box.top;
        *width_ptr = (box.right + 1 - box.left);
        *height_ptr = (box.bottom + 1 - box.top);
}

static void font_MapPixel(int16       *x_ptr, //out, 计算得到的位置
              int16       *y_ptr, //out, 计算得到的位置
              int16       i,      //相对坐标
              int16       j,       //相对坐标
              GUI_RECT_T  box     
                    )
{
        *x_ptr = box.left + i;
        *y_ptr = box.top + j;
}

/**********************************************************************************/
static void font_GetFontInfo(int32  *offset_ptr,         //out character length in byte
                      uint8  **code_ptr,          //out bit map
                      uint8  *fontwidth_ptr,      //out
                      uint8  *fontheight_ptr,     //out
                      uint8  *text_ptr )          //in  character
{
    uint8   fontwidth = 0;
    uint8   fontheight = 0;
    uint16*  ucschar = text_ptr;
    extern uint8 g_euromoney_font14[];
    extern uint8 g_euromoney_font16[];
    
    //javacall_printf("font_GetFontInfo: char: %02x %02x", text_ptr, text_ptr+1);
#if 0 //Jackey, below is just for TCK, but I am not sure whether it do not nedded any more
    if ((0 == *(text_ptr)) && (0x80 == *(text_ptr + 1)))
    {   
        if (SONG_FONT_14 == JavaCallFont.font_num)
        {
            *fontwidth_ptr  = 14;
            *fontheight_ptr = 14; 
            *offset_ptr     = 2;
            *code_ptr       = g_euromoney_font14;       
        }
        else if (SONG_FONT_16 == JavaCallFont.font_num)
        {
            *fontwidth_ptr  = 16;
            *fontheight_ptr = 16; 
            *offset_ptr     = 2;
            *code_ptr       = g_euromoney_font16; 
        }
        javacall_printf("font_GetFontInfo:char(%c), wid=%d, hei=%d, num=%d",
            *text_ptr, fontwidth, fontheight, JavaCallFont.font_num);
        return;
    }
#endif
    fontwidth    = GUI_GetFontWidth(JavaCallFont.font_num,*ucschar);
    fontheight   = GUI_GetFontHeight(JavaCallFont.font_num,*ucschar);
    *code_ptr = GUI_GetFontArray(ucschar, 1, JavaCallFont.font_num);
    *offset_ptr = 2;
    //javacall_printf("font_GetFontInfo:AC char wid=%d, hei=%d, num=%d", fontwidth, fontheight, JavaCallFont.font_num);
    
    *fontwidth_ptr = fontwidth;
    *fontheight_ptr = fontheight;
}

/**********************************************************************************/
static BOOLEAN font_GetFontPos(int16	  *x_ptr,
                        int16	  *y_ptr,
                        uint8      *text_ptr,
                        GUI_RECT_T box,
                        uint8      fontwidth,
                        uint8      fontheight)
{
    int16 i = 0, j = 0;
    uint16 width = 0, height = 0;
    
    font_MapPixelReverse(&i, &j, &width, &height, *x_ptr, *y_ptr, box);
    
    if ((*text_ptr) == JAVA_CR_CHAR){  //回车符
	    i = 0;
        j += fontheight;
        font_MapPixel(x_ptr, y_ptr, i, j, box);
        return TRUE;
    } else if((*text_ptr) == JAVA_LF_CHAR){    //换行符
        i = 0;
        j += 2*fontheight;
        font_MapPixel(x_ptr, y_ptr, i, j, box);
        return TRUE;
    } else {      
	    return FALSE;
	}

}

/**********************************************************************************/

static BOOLEAN font_CheckFontPos(int16       x,			//绝对地址
                     int16       y,
                     GUI_RECT_T  box,
                     BOOLEAN     spchar_result,
                     uint16      fontwidth,
                     uint16      fontheight)
{
    int16 i = 0, j = 0;
    uint16 width = 0, height = 0;

    if ((x < box.left) || (x > box.right)){
        return TRUE;
    }
    
    if (!spchar_result){    
        font_MapPixelReverse(&i, &j, &width, &height, x, y, box);
        if ((i + fontwidth) > width)
            return TRUE;
    }
    return FALSE;
}
//if font bitmap at (i,j) is set
static BOOLEAN font_CheckPixelBaseLang(
                           uint8 fontwidth,
                           uint8 fontheight,
                           uint8 *codedata_ptr,
                           uint8 i/*width*/,
                           uint8 j/*height*/
                           )
{
    BOOLEAN result = FALSE;
    uint16 offset = 0;
    uint16 offset_bit = 0;

    offset = j * ((fontwidth + 7) / 8) + (i / 8);
    offset_bit = (0x80 >> (i & 0x07));
	result = (BOOLEAN)(codedata_ptr[offset] & offset_bit);
    return result;
}

static void font_DrawTOLCD(GUI_COLOR_T	  * lcd_buffer_ptr,		//显示buffer
	               uint16			lcdwidth,                //lcd宽度
    	           uint16			lcdheight,               //lcd高度
        	       uint16			x0,                      //字符左上角x坐标
            	   uint16			y0,                      //字符左上角y坐标
	               uint8			fontwidth,                //字符宽度
    	           uint8			fontheight,               //字符高度
        	       uint8		 *  codedata_ptr,            //字符数据
            	   GUI_COLOR_T		color,					//颜色
	               GUI_RECT_T		box)
{
	uint16 	i, j;		
	uint16  x = x0,   y = y0 ;

	for (j = 0; j<fontheight; j++) {		
		for (i = 0; i<fontwidth; i++) {						
			if ( font_CheckPixelBaseLang( fontwidth, fontheight, codedata_ptr, i, j) ) {
				x = x0+i;  
				y = y0+j;
				if ((y >= box.top )&&(box.bottom >= y)) {
					*( (lcd_buffer_ptr) + (y)*(lcdwidth) + (x) ) = (color) ;
				}
			}																																	
		}															
	}	
}
#endif
/**
 * Set font appearance params 
 * 
 * @param face The font face to be used
 * @param style The font style to be used
 * @param size The font size to be used
 * @return <tt>JAVACALL_OK</tt> if font set successfully, 
 *         <tt>JAVACALL_FAIL</tt> or negative value otherwise
 */
javacall_result javacall_font_set_font( javacall_font_face face, 
                                        javacall_font_style style, 
                                        javacall_font_size size) {
    JavaCallFont.face = face;
    JavaCallFont.style = style;
    JavaCallFont.size = size;

	/*switch(size) {
	case JAVACALL_FONT_SIZE_SMALL:
		JavaCallFont.font_num = MMI_DEFAULT_SMALL_FONT;//SONG_FONT_12;
		break;
	case JAVACALL_FONT_SIZE_MEDIUM:
		JavaCallFont.font_num = MMI_DEFAULT_NORMAL_FONT;//SONG_FONT_14;
		break;
	case JAVACALL_FONT_SIZE_LARGE:
		JavaCallFont.font_num = MMI_DEFAULT_BIG_FONT;//SONG_FONT_16;
		break;
	default:
		return JAVACALL_FALSE;
	}*/
	return JAVACALL_OK;
}

/**
 * Draws the first n characters to Offscreen memory image specified using the current font,
 * color.
 * 
 *
 * @param color color of font
 * @param clipX1 top left X coordinate of the clipping area where the pixel 
 *               (clipX1,clipY1) is the first top-left pixel in the clip area.
 *               clipX1 is guaranteeded to be larger or equal 0 and smaller or equal 
 *               than clipX2.
 * @param clipY1 top left Y coordinate of the clipping area where the pixel 
 *               (clipX1,clipY1) is the first top-left pixel in the clip area.
 *               clipY1 is guaranteeded to be larger or equal 0 and smaller or equal 
 *               than clipY2
 * @param clipX2 bottom right X coordinate of the clipping area where the pixel 
 *               (clipX2,clipY2) is the last bottom right pixel in the clip area.
 *               clipX2 is guaranteeded to be larger or equal than clipX1 and 
 *               smaller or equal than destBufferHoriz.
 * @param clipY2 bottom right Y coordinate of the clipping area where the pixel 
 *               (clipX2,clipY2) is the last bottom right pixel in the clip area
 *               clipY2 is guaranteeded to be larger or equal than clipY1 and 
 *               smaller or equal than destBufferVert.
 * @param destBuffer  where to draw the chars
 * @param destBufferHoriz horizontal size of destination buffer
 * @param destBufferVert  vertical size of destination buffer
 * @param x The x coordinate of the top left font coordinate 
 * @param y The y coordinate of the top left font coordinate 
 * @param text Pointer to the characters to be drawn
 * @param textLen The number of characters to be drawn
 * @return <tt>JAVACALL_OK</tt> if font rendered successfully, 
 *         <tt>JAVACALL_FAIL</tt> or negative value on error or not supported
 */
javacall_result javacall_font_draw(javacall_pixel   color, 
                        int                         clipX1, 
                        int                         clipY1, 
                        int                         clipX2, 
                        int                         clipY2,
                        javacall_pixel*             destBuffer, 
                        int                         destBufferHoriz, 
                        int                         destBufferVert,
                        int                         x, 
                        int                         y, 
                        const javacall_utf16*		text, 
                        int                         textLen)
{
    GUI_RECT_T  clip_rect = {0};
	GUI_RECT_T  disp_rect = {0};
    GUISTR_BUF_T des_buf_data = {0};
	GUISTR_STYLE_T style = {0};/*lint !e64 */
	GUISTR_STATE_T state = {0};/*lint !e64 */
	MMI_STRING_T string = {0};

	clip_rect.left = clipX1;
	clip_rect.right = clipX2 - 1;
	clip_rect.top = clipY1;
	clip_rect.bottom = clipY2 - 1;

	disp_rect = clip_rect;
	disp_rect.left = x;
	disp_rect.top = y;

	string.wstr_ptr = text;        /*lint !e605*/
	
	string.wstr_len = textLen;
	
    des_buf_data.buffer = destBuffer;
    des_buf_data.mem_width =  destBufferHoriz ;
    des_buf_data.width = destBufferHoriz;
    des_buf_data.height = destBufferVert;

	switch( JavaCallFont.size ) 
	{
	case JAVACALL_FONT_SIZE_SMALL:
        style.font = MMI_DEFAULT_SMALL_FONT;
		break;
	case JAVACALL_FONT_SIZE_MEDIUM:
        style.font = MMI_DEFAULT_NORMAL_FONT;
		break;
	case JAVACALL_FONT_SIZE_LARGE:
        style.font = MMI_DEFAULT_BIG_FONT;
		break;
	default:
		style.font = MMI_DEFAULT_NORMAL_FONT;
		break;
	}

	style.font_color = color;

	if ( JavaCallFont.style & JAVACALL_FONT_STYLE_BOLD )/*lint !e655*/
	{
		state |= GUISTR_STATE_BOLD; 
	}
	
	if ( JavaCallFont.style & JAVACALL_FONT_STYLE_ITALIC )/*lint !e655*/
	{
		state |= GUISTR_STATE_ITALIC;
	}
	
	if ( JavaCallFont.style & JAVACALL_FONT_STYLE_UNDERLINE )/*lint !e655*/
	{
		state |= GUISTR_STATE_UNDERLINE;
	}
	
    state |= GUISTR_STATE_SINGLE_LINE;
    GUISTR_DrawTextInRect(&des_buf_data, &disp_rect, &clip_rect, &string, &style, state, -1);

	return JAVACALL_OK;
	
#if 0
	int32	offset = 0;
	int16	x1=x, y1=y;
	uint8	fontwidth, fontheight;
    uint8	*code_ptr = PNULL;
    BOOLEAN spchar_result;         //CR, LF等特殊字符
    BOOLEAN exceed_result;         //是否超出边界
	GUI_RECT_T   box;
	int i;

    if (PNULL == text){
        javacall_printf(LOG_STR_LEAVE("string point is null"));
        return JAVACALL_FAIL;
    }

	box.left = (clipX1 >= clipX2 ) ? clipX2 : clipX1;
	box.right = (clipX2 >= clipX1 ) ? clipX2 :clipX1;
	box.top = (clipY1 >= clipY2 ) ? clipY2 : clipY1;
	box.bottom = (clipY2 >= clipY1 ) ? clipY2 : clipY1;
    box.right--;
    box.bottom--;
    //javacall_printf(LOG_STR("box:%d,%d,%d,%d"), box.left,box.top,box.right,box.bottom);
    if (0 > clipX1 || 0 > clipY2 || destBufferHoriz < clipX2 || destBufferVert < clipY2) {
        javacall_printf(LOG_STR("Clipping area (%d, %d, %d, %d) is out of bounds of the buffer (%d x %d)"),
            clipX1, clipY1, clipX2, clipY2, destBufferHoriz, destBufferVert);
    }

    for (i=0; i<textLen; i++){
        font_GetFontInfo(&offset, &code_ptr, &fontwidth, &fontheight, text+i);
        spchar_result = font_GetFontPos(&x1, &y1, text+i, box,fontwidth, fontheight);
        exceed_result = font_CheckFontPos(x1, y1, box, spchar_result, fontwidth, fontheight);  

        if ((!spchar_result)&&(!exceed_result)){
            font_DrawTOLCD(destBuffer, destBufferHoriz,destBufferVert, 
				x1, y1,fontwidth,fontheight, code_ptr,color,box);
       }
		x1 = x1 + fontwidth;
	}
    return JAVACALL_OK;
#endif
}
    
    
    
/**
 * Query for the font info structure for a given font specs
 *
 * @param face The font face to be used (Defined in <B>Font.java</B>)
 * @param style The font style to be used (Defined in
 * <B>Font.java</B>)
 * @param size The font size to be used. (Defined in <B>Font.java</B>)
 *
 * @param ascent return value of font's ascent
 * @param descent return value of font's descent
 * @param leading return value of font's leading 
 * 
 * @return <tt>JAVACALL_OK</tt> if successful, <tt>JAVACALL_FAIL</tt> or 
 *         <tt>JAVACALL_FAIL</tt> or negative value on error
 *
 */
javacall_result javacall_font_get_info( javacall_font_face  face, 
                                        javacall_font_style style, 
                                        javacall_font_size  size, 
                                        /*out*/ int* ascent,
                                        /*out*/ int* descent,
                                        /*out*/ int* leading) 
{
    int height;
    GUI_FONT_T gui_font;

	switch(size) {
	case JAVACALL_FONT_SIZE_SMALL:
        gui_font = MMI_DEFAULT_SMALL_FONT;
		break;
	case JAVACALL_FONT_SIZE_MEDIUM:
        gui_font = MMI_DEFAULT_NORMAL_FONT;
		break;
	case JAVACALL_FONT_SIZE_LARGE:
        gui_font = MMI_DEFAULT_BIG_FONT;
		break;
	default:
		return JAVACALL_FALSE;/*lint !e64 */
	}
    height = GUIFONT_GetHeight(gui_font);
	//*descent = (int)(gui_font * 0.1);
	*descent = (int)(height * 0.1);
	*ascent = height - *descent;
	*leading = 1;
    //javacall_printf("font info:%d, %d, %d", height, *ascent, *descent);
	return JAVACALL_OK;
}
        
/**
 * return the char width for the first n characters in charArray if
 * they were to be drawn in the font indicated by the parameters.
 *
 *
 * @param face The font face to be used
 * @param style The font style to be used
 * @param size The font size to be used
 * @param charArray The string to be measured
 * @param charArraySize The number of character to be measured
 * @return total advance width in pixels (a non-negative value)
 */
//原来只考虑细体的方法 
/*
int javacall_font_get_width(javacall_font_face     face, 
                            javacall_font_style    style, 
                            javacall_font_size     size,
                            const javacall_utf16* charArray, 
                            int                    charArraySize) {
    uint16 length=0;
    GUI_FONT_T fontType = MMI_DEFAULT_NORMAL_FONT;

    switch (size) {
        case JAVACALL_FONT_SIZE_SMALL:
            fontType = MMI_DEFAULT_SMALL_FONT;
            break;
        case JAVACALL_FONT_SIZE_MEDIUM:
            fontType = MMI_DEFAULT_NORMAL_FONT;
            break;
        case JAVACALL_FONT_SIZE_LARGE:
            fontType = MMI_DEFAULT_BIG_FONT;
            break;
    }
    length=GUI_GetStringWidth(fontType, charArray, charArraySize);
    return length;
}*/
//以下是增加考虑了粗体属性的计算宽度方法
int javacall_font_get_width(javacall_font_face     face, 
                            javacall_font_style    style, 
                            javacall_font_size     size,
                            const javacall_utf16* charArray, 
                            int                    charArraySize) {
    uint16 length=0;
    GUISTR_STYLE_T str_style = {0};
    GUISTR_STATE_T str_state = {0};
    MMI_STRING_T string = {0};

    string.wstr_ptr = charArray;        /*lint !e605*/
    string.wstr_len = charArraySize;

    switch(size) {
        case JAVACALL_FONT_SIZE_SMALL:
            str_style.font = MMI_DEFAULT_SMALL_FONT;
            break;
        case JAVACALL_FONT_SIZE_MEDIUM:
            str_style.font = MMI_DEFAULT_NORMAL_FONT;
            break;
        case JAVACALL_FONT_SIZE_LARGE:
            str_style.font = MMI_DEFAULT_BIG_FONT;
            break;
        default:
            str_style.font = MMI_DEFAULT_NORMAL_FONT;
            break;
    }

    if ( style & (uint32)JAVACALL_FONT_STYLE_BOLD ) {
        str_state |= GUISTR_STATE_BOLD; 
    }

    if ( style & (uint32)JAVACALL_FONT_STYLE_ITALIC ) {
        str_state |= GUISTR_STATE_ITALIC;
    }

    if ( style & (uint32)JAVACALL_FONT_STYLE_UNDERLINE ) {
        str_state |= GUISTR_STATE_UNDERLINE;
    }
    str_state |= GUISTR_STATE_SINGLE_LINE;

    length = GUISTR_GetStringWidth(&str_style,&string,str_state);

    return length;
}
    
#ifdef __cplusplus
} //extern "C"
#endif




