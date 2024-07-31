/****************************************************************************
** File Name:      mmiqq_ui.c(gui.c)                                                *
** Author:                                                                 *
** Date:             05/11/2010                                              *
** Copyright:      2003 Spreadtrum, Incorporated. All Rights Reserved.       *
** Description:    This file is used to describe the system  of QQ             *
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             *
** 05/2010       fen.xie         Create
** 
****************************************************************************/
#ifdef QQ_SUPPORT

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#ifdef WIN32
#include "std_header.h"
#endif
#include "gui.h"
#include "qskinmanger.h"
//#include "qfile.h"
#include "guiblock.h"
#include "guicommon.h"
#include "guires.h"
#include "guifont.h"
#include "guistring.h"
#include "mmidisplay_data.h"

/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
#define MMIQQ_TEXT_LINE_SPACE   (10)
#define UNICODE_WITH_MAX_HEIGHT     (0x64E6)//"擦"字的unicode值

/*---------------------------------------------------------------------------*/
/*                          TYPE AND CONSTANT                                */
/*---------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         STATIC DEFINITION                                *
 **--------------------------------------------------------------------------*/
LOCAL QRect     s_clip_rect = {0};
//qq中文本的字体信息
LOCAL QQ_FONT   s_text_font_info = {0x0000, MMI_DEFAULT_NORMAL_FONT};

/**--------------------------------------------------------------------------*
 **                         EXTERNAL DECLARE                                 *
 **--------------------------------------------------------------------------*/
//extern const QU16	TransferTab_idx2code[105];

/**--------------------------------------------------------------------------*
 **                         GLOBAL DEFINITION                                *
 **--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description: set rect
//  Global resource dependence :   
//  Author: TENCENT
//  Note: 
/*****************************************************************************/
PUBLIC void SETAEERECT(QRect* rect_ptr, QU16 x, QU16 y, QU16 dx, QU16 dy)
{
    if (PNULL == rect_ptr)
    {
        return;
    }
    
    rect_ptr->x = x;
    rect_ptr->y = y;
    rect_ptr->dx = dx;
    rect_ptr->dy = dy;
}
/*****************************************************************************/
//  Description: IDISPLAY_GetGDIBuffer
//  Global resource dependence :  
//  Author: TENCENT
//  Note: 
/*****************************************************************************/
PUBLIC QU16*  IDISPLAY_GetGDIBuffer(void)
{
    GUI_LCD_DEV_INFO dev = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
    return GUIBLOCK_GetBuf(&dev);
}
/*****************************************************************************/
//  Description: IDISPLAY_DrawLine
//  Global resource dependence :  
//  Author: TENCENT
//  Note: 
/*****************************************************************************/
PUBLIC void IDISPLAY_DrawLine(IDisplay* pIDisplay, QU16 x1, QU16 y1, QU16 x2, QU16 y2, RGBVAL color)
{
    GUI_LCD_DEV_INFO dev = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
    LCD_DrawLine(&dev, x1, y1, x2, y2, color);
}
/*****************************************************************************/
//  Description: IDISPLAY_FillRect
//  Global resource dependence :  
//  Author: TENCENT
//  Note: 
/*****************************************************************************/
PUBLIC void IDISPLAY_FillRect(IDisplay* pIDisplay, QRect* rect_ptr, RGBVAL color)
{
    GUI_LCD_DEV_INFO dev = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
    GUI_RECT_T 	box = {0};

    if (PNULL == rect_ptr)
    {
        return;
    }
    
    box = GUI_CreateRect(rect_ptr->x, rect_ptr->y, (rect_ptr->x + rect_ptr->dx - 1), (rect_ptr->y + rect_ptr->dy - 1));
    LCD_FillRect(&dev, box, color);
}

/*基本操作*/
/*****************************************************************************/
//  Description: IDISPLAY_DrawRect
//  Global resource dependence :  
//  Author: TENCENT
//  Note: 
/*****************************************************************************/
PUBLIC void IDISPLAY_DrawRect(IDisplay* pIDisplay, QRect* rect_ptr, RGBVAL color1, RGBVAL color2, int flag)
{
    GUI_RECT_T 	box = {0};
    GUI_LCD_DEV_INFO dev = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
    
    if (PNULL == rect_ptr)
    {
        return;
    }
    box = GUI_CreateRect(rect_ptr->x, rect_ptr->y, (rect_ptr->x + rect_ptr->dx - 1), (rect_ptr->y + rect_ptr->dy - 1));

    if(flag & IDF_RECT_FILL)
    {
        LCD_FillRect(&dev, box, color2);
    }
    
    if(flag & IDF_RECT_FRAME)
    {
        LCD_DrawRect(&dev, box, color1);
    }
}

/*****************************************************************************/
//  Description: IDISPLAY_SetColor
//  Global resource dependence :  
//  Author: TENCENT
//  Note: 
/*****************************************************************************/
PUBLIC void IDISPLAY_SetColor(IDisplay* pIDisplay, int type, RGBVAL color)
{
    s_text_font_info.color = color;
}

/*****************************************************************************/
//  Description: IDISPLAY_MeasureText2
//  Global resource dependence :  
//  Author: TENCENT
//  Note: 
/*****************************************************************************/
PUBLIC void IDISPLAY_MeasureText2(AECHAR* text_ptr,int* width_ptr, int* height_ptr)
{
    if (width_ptr != PNULL)
    {
        *width_ptr = IDISPLAY_MeasureText(0, 0, text_ptr);
    }        
   
    if (height_ptr != PNULL)
    {
        *height_ptr = IDISPLAY_GetFontHeight(0, 0, 0);
    }
}

/*****************************************************************************/
//  Description: IDISPLAY_MeasureText
//  Global resource dependence :  
//  Author: TENCENT
//  Note: 
/*****************************************************************************/
PUBLIC uint16 IDISPLAY_MeasureText(IDisplay* pIDisplay, int type, AECHAR* text_ptr)
{
    if (PNULL == text_ptr)
    {
        return 0;
    }

    return GUI_GetStringWidth(s_text_font_info.font, text_ptr, MMIAPICOM_Wstrlen(text_ptr));
}

/*****************************************************************************/
//  Description: IDISPLAY_GetFontMetrics
//  Global resource dependence :  
//  Author: TENCENT
//  Note: 
/*****************************************************************************/
PUBLIC uint8 IDISPLAY_GetFontMetrics(IDisplay* pIDisplay, GUI_FONT_T font, QU16 unicode, QU16 size)
{
    return GUI_GetFontHeight(s_text_font_info.font, UNICODE_WITH_MAX_HEIGHT);  //"擦"的高度               	
}

/*****************************************************************************/
//  Description: IDISPLAY_GetFontHeight
//  Global resource dependence :  
//  Author: TENCENT
//  Note: 
/*****************************************************************************/
PUBLIC uint8 IDISPLAY_GetFontHeight(IDisplay* pIDisplay, GUI_FONT_T font, QU16 unicode)
{
    return GUI_GetFontHeight(s_text_font_info.font, UNICODE_WITH_MAX_HEIGHT);  //"擦"的高度                    
}

/*****************************************************************************/
//  Description: IDISPLAY_Update
//  Global resource dependence :  
//  Author: TENCENT
//  Note: 
/*****************************************************************************/
PUBLIC void IDISPLAY_Update(IDisplay* pIDisplay)
{
 
}

/*****************************************************************************/
//  Description: IDISPLAY_SetClipRect
//  Global resource dependence :  s_clip_rect
//  Author: TENCENT
//  Note: 
/*****************************************************************************/
PUBLIC void IDISPLAY_SetClipRect(IDisplay* pIDisplay, QRect* rect_ptr)
{
    if (PNULL == rect_ptr)
    {
        return;
    }
    
    s_clip_rect.x = rect_ptr->x;
    s_clip_rect.y = rect_ptr->y;
    s_clip_rect.dx = rect_ptr->dx;
    s_clip_rect.dy = rect_ptr->dy;
}

/*****************************************************************************/
//  Description: IDISPLAY_GetClipY
//  Global resource dependence :  
//  Author: TENCENT
//  Note: 
/*****************************************************************************/
PUBLIC QU16 IDISPLAY_GetClipY(void)
{
    return s_clip_rect.y;
}

/*****************************************************************************/
//  Description: IDISPLAY_GetClipBY
//  Global resource dependence :  
//  Author: TENCENT
//  Note: 
/*****************************************************************************/
PUBLIC QU16 IDISPLAY_GetClipBY(void)
{
    return (QU16)(s_clip_rect.y + s_clip_rect.dy);
}

/*****************************************************************************/
//  Description: IDISPLAY_GetClipRect
//  Global resource dependence :  
//  Author: TENCENT
//  Note: 
/*****************************************************************************/
PUBLIC void IDISPLAY_GetClipRect(IDisplay* pIDisplay, QRect* rect_ptr)
{
    if (PNULL == rect_ptr)
    {
        return;
    }
    
    rect_ptr->x = rect_ptr->y = 0;
    rect_ptr->dx = SCR_W;
    rect_ptr->dy = SCR_H;
}


/*****************************************************************************/
//  Description: IDISPLAY_GetFontWidth
//  Global resource dependence :  
//  Author: TENCENT
//  Note: 
/*****************************************************************************/
PUBLIC uint8 IDISPLAY_GetFontWidth(IDisplay* pIDisplay, GUI_FONT_T font, QU16 unicode)
{
	return GUI_GetFontWidth(s_text_font_info.font, unicode);                  
}

/*****************************************************************************/
//  Description: IDISPLAY_MeasureTextEx
//  Global resource dependence :  
//  Author: TENCENT
//  Note: 
/*****************************************************************************/
PUBLIC uint16 IDISPLAY_MeasureTextEx(IDisplay* pIDisplay, int type, AECHAR* text_ptr, int len, uint16 width, int* num_ptr)
{	
    uint16 str_len = 0;
    uint16  str_width = 0;

   if (PNULL == text_ptr)
    {
        if(num_ptr != PNULL) 
        {
            *num_ptr = 0;
        }
        return 0;
    }

    str_len = MMIAPICOM_Wstrlen(text_ptr);
    if ((len > str_len) || (len <= 0))
    { 
        len = str_len;
    }
	
    str_width = GUI_GetStringWidth(s_text_font_info.font, text_ptr, len);
	
    if(width > str_width)
    {
        if(num_ptr != PNULL) 
        {
            *num_ptr = len;
        }
       return str_width;
    }
    else
    {
        int32 i = 0;
        for(i = len - 1; i > 0; i --)
        {
            str_width -= GUI_GetFontWidth(s_text_font_info.font, text_ptr[i]);
            if(width > str_width)
            {
                if(num_ptr != PNULL) 
                {
                    *num_ptr = i;
                }
                return str_width;
            }
        }
    }

    return 0;
}


/*****************************************************************************/
//  Description: IDISPLAY_DrawText
//  Global resource dependence :  
//  Author: TENCENT
//  Note: 
/*****************************************************************************/
PUBLIC void IDISPLAY_DrawText(IDisplay* pIDisplay, GUI_FONT_T font, AECHAR* text_ptr, int len, QU16 x, QU16 y, QRect* rect_ptr, int flag)
{
	
    GUI_LCD_DEV_INFO dev = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
    int strlength = 0;
    GUI_POINT_T startpoint = {0};
    GUI_RECT_T 	box = {0};
    //uint8 height = 0;
    QU16 w =0, h = 0;
    MMI_STRING_T    str = {0};
    GUISTR_STYLE_T        str_style = {0};
    GUISTR_STATE_T        str_state = GUISTR_STATE_ALIGN | GUISTR_STATE_WORDBREAK;

    if(PNULL == text_ptr)
    {
        return;
    }
	
    flag &= (~IDF_TEXT_TRANSPARENT);
    strlength = MMIAPICOM_Wstrlen(text_ptr);
    if ((len > strlength) || (len <= 0)) 
    {
        len = strlength;
    }

    str.wstr_ptr = text_ptr;
    str.wstr_len = len;
    str_style.font = s_text_font_info.font;
    str_style.font_color = s_text_font_info.color;
        
    if(0 == flag)
    {
        startpoint.x = x;
        startpoint.y = y; 
        //LCD_DisplayString(&dev, startpoint, pText, len, 0, s_text_font_info.color, s_text_font_info.font, 0);
        //GUISTR_DrawTextToLCDMultiLine(&dev, &startpoint, &str, &str_style, str_state);
        GUISTR_DrawTextToLCDSingleLine(&dev, &startpoint, &str, &str_style, str_state);//MS00204212
    }
    else if(rect_ptr != PNULL)
    {
        x = rect_ptr->x;  
        y = rect_ptr->y;
        w = rect_ptr->dx; 
        h = rect_ptr->dy;

        box = GUI_CreateRect(x, y, x + w, y + h);
        //@fen.xie :先控制box的范围不超出当前LCD尺寸(例,显示"QQ登录"时导致出现黑块)
        box.bottom =  MIN(box.bottom, SCR_H-1);
        box.right =  MIN(box.right, SCR_W-1);
      
        str_style.line_space = MMIQQ_TEXT_LINE_SPACE;
	
	if(flag&IDF_ALIGN_CENTER)
        {
            //LCD_DisplayStringByAlign(&dev, box, pText, len, 0, s_text_font_info.color, s_text_font_info.font, 8/*ALIGN_HVMIDDLE*/, 10);
            str_style.align = ALIGN_HVMIDDLE;
            GUISTR_DrawTextToLCDInRect(&dev, &box, &box, &str, &str_style, str_state, GUISTR_TEXT_DIR_AUTO);
        }
        else if(flag&IDF_ALIGN_RIGHT)
        {
            //LCD_DisplayStringByAlign(&dev, box, pText, len, 0, s_text_font_info.color, s_text_font_info.font, 7/*ALIGN_RVMIDDLE*/, 10);
            str_style.align = ALIGN_RVMIDDLE;
            GUISTR_DrawTextToLCDInRect(&dev, &box, &box, &str, &str_style, str_state, GUISTR_TEXT_DIR_AUTO);
        }
        else
        {
            //LCD_DisplayStringByAlign(&dev, box, pText, len, 0, s_text_font_info.color, s_text_font_info.font, 6/*ALIGN_LVMIDDLE*/, 10);
            str_style.align = ALIGN_LVMIDDLE;
            GUISTR_DrawTextToLCDInRect(&dev, &box, &box, &str, &str_style, str_state, GUISTR_TEXT_DIR_AUTO);
        }

    }
}


/*遮罩效果实现*/
/*****************************************************************************/
//  Description: QDrawCoverInRect
//  Global resource dependence :  
//  Author: TENCENT
//  Note: 
/*****************************************************************************/
PUBLIC void QDrawCoverInRect(IDisplay* pIDisplay, QU16 x,QU16 y,QU16 w,QU16 h)
{
/*	QU16 i, j;
	QU16* p;
	GUI_LCD_DEV_INFO dev = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};

	if(x >= SCR_W || y >= SCR_H){
		return;
	}
	if(x+w > SCR_W){
		w = SCR_W-x;
	}
	if(y+h > SCR_H){
		h = SCR_H-y;
	}
	p = (QU16*)GUIBLOCK_GetBuf(&dev);
	if(p == QNULL) return;
	
	p += x+y*SCR_W;

	for(i = 0; i < h; i ++){
		for(j = 0; j < w ;j ++){
			p[j] = (p[j]>>2)&0x39e7;
		}
		p += SCR_W;
	}
*/
}

/* 渐变效果实现*/
/*****************************************************************************/
//  Description: IDISPLAY_DrawVGraded
//  Global resource dependence :  
//  Author: TENCENT
//  Note: 
/*****************************************************************************/
PUBLIC void IDISPLAY_DrawVGraded(IDisplay* pIDisplay, QU16 x1, QU16 y1, QU16 x2, QU16 y2, RGBVAL color1, RGBVAL color2)
{
    int lineHigh = 0; 
    int n = 0; 

    uint16 r = 0, g = 0, b = 0;
    uint16 r1 = 0, g1 = 0, b1 = 0;
    uint16 r2 = 0, g2 = 0, b2 = 0;
    uint16 rstep = 0, gstep = 0, bstep = 0;

    r2 = QGetRGBVAL_R(color2);
    r1 = QGetRGBVAL_R(color1);
    g2 = QGetRGBVAL_G(color2);
    g1 = QGetRGBVAL_G(color1);
    b2 = QGetRGBVAL_B(color2);
    b1 = QGetRGBVAL_B(color1);

    lineHigh = y2 - y1; 
	
    if(r2 > r1)
    {
        rstep =  ((r2 -r1)<<8)/lineHigh;
    }
    else
    {
        rstep =  ((r1 -r2)<<8)/lineHigh;
    }

    if(g2 > g1)
    {
        gstep =  ((g2 - g1)<<8)/lineHigh;
    }
    else
    {
        gstep =  ((g1 - g2)<<8)/lineHigh;
    }

    if(b2 > b1)
    {
        bstep =  ((b2 - b1)<<8)/lineHigh;
    }
    else
    {
        bstep =  ((b1 - b2)<<8)/lineHigh;
    }

    for (n = 0; n < lineHigh; n++)
    {
        if(r2 > r1)
        {
            r =  r1 + ((rstep*n)>>8);
        }
        else
        {
            r =  r1 - ((rstep*n)>>8);
        }

        if(g2 > g1)
        {
            g =  g1 + ((gstep*n)>>8);
        }
        else
        {
            g =  g1 - ((gstep*n)>>8);
        }

        if(b2 > b1)
        {
            b =  b1 + ((bstep*n)>>8);
        }
        else
        {
            b =  b1 - ((bstep*n)>>8);
        }
		
        IDISPLAY_DrawLine(0, (QU16)x1, (QU16)(y1+n), (QU16)(x2), (QU16)(y1+n), (RGBVAL)(r<<11|g<<5|b));
    }
    IDISPLAY_DrawLine(0, (QU16)x1, (QU16)y2, (QU16)(x2), (QU16)y2, (RGBVAL)color2);
}

/* 选择项高亮实现*/
/*****************************************************************************/
//  Description: QDrawChoose
//  Global resource dependence :  
//  Author: TENCENT
//  Note: 
/*****************************************************************************/
PUBLIC void QDrawChoose(IDisplay* pIDisplay,QSkinManger* skin_manger_ptr, QU16 x1, QU16 y1, QU16 x2, QU16 y2)
{
    //GUI_LCD_DEV_INFO dev = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
    RGBVAL clrBegin = QCHOOSE_TRANS_BEGIN(skin_manger_ptr);
    RGBVAL clrEnd = QCHOOSE_TRANS_END(skin_manger_ptr);

    IDISPLAY_DrawVGraded(pIDisplay, x1+1, y1 + 1, x2, y2, clrBegin, clrEnd);
    QDisplay_DrawRoundRect(pIDisplay, x1,  y1, (QU16)(x2 - x1 + 1), (QU16)(y2 - y1 + 1), clrBegin);

    //IDISPLAY_DrawVGraded(&dev,x1+1, y1 + 1, x2, y2,clrBegin,clrEnd);
    //QDisplay_DrawRoundRect(&dev, x1, y1, x2 - x1, y2 - y1,clrBegin);
}

/*绘制三角*/
/*****************************************************************************/
//  Description: QDrawDelta
//  Global resource dependence :  
//  Author: TENCENT
//  Note: 
/*****************************************************************************/
PUBLIC void QDrawDelta(IDisplay* pIDisplay, QU16 x,QU16 y,QU16 h, RGBVAL fillColor, QU8 flag)
{	
    QU16 i = 0, d = 0;	
    QU16 sx = 0, ex = 0, sy = 0, ey = 0;
    GUI_LCD_DEV_INFO dev = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};

    switch(flag)
    {
    case QDIRECT_UP:
        d = (QU16)(2*h-2);
        for(i = 0;i < h;i++)
        {
            sx = x + i ; 
            ex = x +d -i; 
            sy = ey = y - i;
            LCD_DrawLine(&dev, sx, sy, ex, ey, fillColor);
        }
        break;
        
    case QDIRECT_DOWN:
        d = 2*h-2;
        for(i = 0;i < h;i++)
        {
            sx = x + i ; 
            ex = x +d -i; 
            sy = ey = y + i;
            LCD_DrawLine(&dev, sx, sy, ex, ey, fillColor);
        }
        break;
        
    case QDIRECT_LEFT:
        d = 2*h-2;
        for(i = 0;i < h;i++)
        {
            sx = ex = x-i; 
            sy = y + i; 
            ey = y + d - i; 
            LCD_DrawLine(&dev, sx, sy, ex, ey, fillColor);
        }
        break;
        
    case QDIRECT_RIGHT:
        d = 2*h-2;
        for(i = 0;i < h;i++)
        {
            sx = ex = x+i; 
            sy = y + i; 
            ey = y + d - i; 
            LCD_DrawLine(&dev, sx, sy, ex, ey, fillColor);
        }
        break;
        
    default:
        break;
    }

}

/*****************************************************************************/
//  Description: PointInRect1
//  Global resource dependence :  
//  Author: TENCENT
//  Note: 
/*****************************************************************************/
PUBLIC int PointInRect1(QS16 x1,QS16 y1,QS16 x,QS16 y,QS16 w,QS16 h)
{
    if ((x1 < x) || (y1 < y) || (x1 > x+w) || (y1 > y+h))
    {
        return 0;//false
    }

    return 1;//true
}


/****************遮罩效果相关************************/
/*****************************************************************************/
//  Description: QChangeColor
//  Global resource dependence :  
//  Author: TENCENT
//  Note: 
/*****************************************************************************/
PUBLIC void QChangeColor(AEERect* rect_ptr, RGBVAL color1, RGBVAL color2)
{
    QU16* p = PNULL;
    QU16 s = 0, i = 0, j = 0;
    GUI_LCD_DEV_INFO dev = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};

    if(PNULL == rect_ptr)
    {
        return;
    }

    p = (QU16*)GUIBLOCK_GetBuf(&dev);
    if(PNULL == p)
    {
        return;
    }
    
    s = color1;
    for(i = 0; i < SCR_H; i++)
    {
        if ((i >= rect_ptr->y) && (i < (rect_ptr->y + rect_ptr->dy)))
        {
            for(j = 0 ; j < SCR_W; j++)
            {
                if((j >= rect_ptr->x) && (j < rect_ptr->x +rect_ptr->dx))
                {
                    if(p[j] == s)
                    {
                        p[j] = 0xffff;//white color
                    }
                }
            }
        }
        p+=SCR_W;
    }
}
/*****************************************************************************/
//  Description: QDisplay_DrawRoundRect
//  Global resource dependence :  
//  Author: TENCENT
//  Note: 
/*****************************************************************************/
PUBLIC void QDisplay_DrawRoundRect(IDisplay* pIDisplay,QU16 x,QU16 y,QU16 w,QU16 h,RGBVAL color)
{
    GUI_LCD_DEV_INFO dev = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
    QU32 x2 = 0, y2 = 0, tmpx1 = 0, tmpy1 = 0, tmpx2 = 0, tmpy2 = 0;

    x2 = x+w-1;   
    y2 = y+h-1;
    tmpx1 = x+1; 
    tmpy1 = y+1;
    tmpx2 = x2-1;  
    tmpy2 = y2-1;

    LCD_DrawLine(&dev, tmpx1, y, tmpx2, y, color); //上
    LCD_DrawLine(&dev, tmpx1, y2, tmpx2, y2, color); //下
    LCD_DrawLine(&dev, x, tmpy1, x, tmpy2, color); //左
    LCD_DrawLine(&dev, x2, tmpy1, x2, tmpy2, color); //右
}

/*****************************************************************************/
//  Description: GetCharH
//  Global resource dependence :  
//  Author: TENCENT
//  Note: 
/*****************************************************************************/
PUBLIC QU16 GetCharH(void)
{
    QU16 height = 0;
    
    height = IDISPLAY_GetFontHeight(0, 0, 0);
    
    if(height < 16) 
    {
        height = 16;
    }
    return (QU16)(height + 2);
}

/*****************************************************************************/
//  Description: QGetTitleH
//  Global resource dependence :  
//  Author: TENCENT
//  Note: 
/*****************************************************************************/
PUBLIC int QGetTitleH(void)
{
    int char_h = 0;
    char_h = IDISPLAY_GetFontHeight(0, 0, 0);
    char_h = (char_h < 16? 16 : char_h);
    return char_h + 4;
}
/*****************************************************************************/
//  Description: QDrawTitle
//  Global resource dependence :  
//  Author: TENCENT
//  Note: 
/*****************************************************************************/
PUBLIC void QDrawTitle(IDisplay* pIDisplay, QSkinManger* skin_manger_ptr, AECHAR* title_ptr)
{
    AEERect rect = {0};
    GUI_LCD_DEV_INFO dev = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};

    if (PNULL == title_ptr)
    {
        return;
    }
    
    rect.x = rect.y = 0; 
    rect.dx = SCR_W; 
    rect.dy = QGetTitleH();

    IDISPLAY_DrawVGraded(
            (IDisplay*)&dev, (rect.x), (rect.y), (rect.x+rect.dx), (rect.y+rect.dy),
            QTAB_TRANS_BEGIN(skin_manger_ptr), QTAB_TRANS_END(skin_manger_ptr));
    
    IDISPLAY_SetColor((IDisplay*)&dev, CLR_USER_TEXT, QTAB_FONT(skin_manger_ptr));
    IDISPLAY_DrawText((IDisplay*)&dev, AEE_FONT_NORMAL, title_ptr, -1, 0, 0, &rect, IDF_ALIGN_CENTER|IDF_ALIGN_MIDDLE);
}

// 返回字高和表情最大的那个
/*****************************************************************************/
//  Description: QGetLineH
//  Global resource dependence :  
//  Author: TENCENT
//  Note: 
/*****************************************************************************/
PUBLIC int QGetLineH(IDisplay* pIDisplay)
{
    int nLineH = IDISPLAY_GetFontHeight(pIDisplay, AEE_FONT_NORMAL, 0);

    return ((nLineH >= 16)? nLineH: 16);
}

/*****************************************************************************/
//  Description: QWStrdupEx
//  Global resource dependence :  
//  Author: TENCENT
//  Note: NEED to free the value of return
/*****************************************************************************/
PUBLIC QU16* QWStrdupEx(QU16* text_ptr, int w)
{
    QU16* des = PNULL;
    QU16 srclen = 0, deslen = 0, tmp = 0;
    BOOLEAN  bNeedPoint = FALSE;
    QU16 wszPoint[4] = {'.', '.', '.', 0x00};

   if(PNULL == text_ptr)
    {
        return PNULL;
    }
	
    srclen=(QU16)MMIAPICOM_Wstrlen((AECHAR*)text_ptr);

    if(IDISPLAY_MeasureText(0, 0, text_ptr)  > w)
    {
        bNeedPoint = TRUE;		
        w -= IDISPLAY_MeasureText(0, 0, wszPoint);
        for(deslen = 0, tmp=0; deslen < srclen; deslen++)
        {
            tmp += IDISPLAY_GetFontWidth(0, 0, text_ptr[deslen]);
            if(tmp > w) 
            {
                break;
            }
        }
        deslen += 3;
    }
    else
    {
        bNeedPoint = FALSE;		
        deslen = srclen;
    }

    des=(QU16*)QMalloc(deslen*2+2);
    if(PNULL == des) 
    {
        return PNULL;
    }
    SCI_MEMSET(des, 0, (deslen*2+2));

    if(bNeedPoint)
    {
    	SCI_MEMCPY((AECHAR*)des, (AECHAR*)text_ptr, (deslen-3)*2);
    	MMIAPICOM_Wstrcat((AECHAR*)des, (AECHAR*)wszPoint);	
    }
    else
    {
    	SCI_MEMCPY(des, text_ptr, deslen*2);
    }

    return des;
}

// 暂时放这里，文件log


//QU32 QGetTickTime(void);

/*****************************************************************************/
//  Description: IIMAGE_DrawBuffer
//  Global resource dependence :  
//  Author: TENCENT
//  Note: 
/*****************************************************************************/
PUBLIC void IIMAGE_DrawBuffer(QIImage* p, int x, int y, void* data_ptr, int size)
{
    GUI_POINT_T point = {0, 0};
    GUI_LCD_DEV_INFO dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};

    point.x = (int16)x;
    point.y = (int16)y;
    	
    //	QWriteFileLog("width = %d, height = %d, framenum = %d, type = %d", img_info.width, img_info.height, img_info.frame_num, img_info.img_type);

    GUIRES_DisplayImgByPtr((uint8*)data_ptr, (uint32)size, &point, NULL, NULL, &dev_info);	
}
/*****************************************************************************/
//  Description: QGetImgInfo
//  Global resource dependence :  
//  Author: TENCENT
//  Note: 
/*****************************************************************************/
PUBLIC unsigned char QGetImgInfo(unsigned char* img_ptr,unsigned short size,unsigned long* width_ptr,unsigned long* height_ptr)
{
    //unsigned long i,tmp;
   // unsigned char type;
    uint16 width = 0, height = 0;

    //type = QIMG_TYPE_ERR;
    
    if(width_ptr != 0)
    {
        *width_ptr = 0;
    }
    
    if(height_ptr != 0)
    {
        *height_ptr = 0;
    }
    
    if ((PNULL == img_ptr) || (PNULL == width_ptr) || (PNULL == height_ptr) || (size < 4))
    {
        return QIMG_TYPE_ERR;
    }
    
    *width_ptr = 0;
    *height_ptr = 0;

    if (('G' == img_ptr[0]) && ('I' == img_ptr[1]) && ('F' == img_ptr[2]))
    { // gif
        if(size <13)
        {
            return QIMG_TYPE_ERR;
        }
        
        GUIRES_GetImgWidthHeightByPtr((uint8 *)img_ptr, &width, &height, (uint32)size);
        *width_ptr = width;
        *height_ptr = height;
        return QIMG_TYPE_GIF;
    }
    return QIMG_TYPE_ERR;	
}
#endif


