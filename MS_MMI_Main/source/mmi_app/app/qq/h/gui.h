/*****************************************************************************
** File Name:      gui.h                                                  *
** Author:                                                                   *
** Date:             11/05/2010                                                *
** Copyright:      2003 Spreadtrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to declare QQ GUI function and define global               *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 05/2010       fen.xie       Create
******************************************************************************/
#ifndef QQ_GUI_H
#define QQ_GUI_H

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "datatype.h"
#include "qskinmanger.h"
#include "mmi_font.h"
#include "guilcd.h"

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
#define QGDI_PNG_HEAD				 137
#define QGDI_PNG_PALETTEHEAD		"PLTE"
#define QGDI_PNG_DATAHEAD			"IDAT"
#define QGDI_BMP_HEAD				"BM"

#define MAKE_RGB(_r, _g, _b)     (RGBVAL)((_r>>3)<<11)|((_g>>2)<<5)|(_b>>3)

#define IDF_ALIGN_LEFT          0x00000010
#define IDF_ALIGN_CENTER        0x00000020
#define IDF_ALIGN_RIGHT         0x00000040
#define IDF_ALIGN_TOP           0x00000100
#define IDF_ALIGN_MIDDLE        0x00000200
#define IDF_ALIGN_BOTTOM        0x00000400
#define IDF_TEXT_TRANSPARENT    0x00008000

#define IDF_RECT_FRAME          0x00000001
#define IDF_RECT_FILL           0x00000002

#define TP_FRAME              0x00010000
#define TP_MULTILINE          0x00020000
#define TP_PASSWORD           0x00400000


#define AEE_TM_LETTERS        0x0003
#define AEE_TM_NUMBERS        0x0005
#define AEE_TM_PINYIN         0x0009  // Chinese Input

#define CLR_USER_TEXT            1
#define AEE_FONT_NORMAL          2

#define  QDrawCover(_a)  QDrawCoverInRect(_a, 0, 0, SCR_W, SCR_H)
#define  QGetDeltaH()  5
#define  QGetDeltaW()  3

/**--------------------------------------------------------------------------*
 **                         TYPE DEFINITION                                 *
 **--------------------------------------------------------------------------*/
typedef enum
{
    QDIRECT_UP = 0,
    QDIRECT_DOWN,
    QDIRECT_LEFT,
    QDIRECT_RIGHT,
    QDIRECT_MAX
}QDirect;

typedef enum
{
    QIMG_TYPE_ERR = 0,
    QIMG_TYPE_BMP,
    QIMG_TYPE_PNG,
    QIMG_TYPE_GIF,
    QIMG_TYPE_JPG,
    QIMG_TYPE_MAX
}QIMG_TYPE_E;

typedef int QIImage;/*@fen.xie modify from IImage->QIImage.IImage has been defined in other*/

typedef struct 
{
    unsigned short x;
    unsigned short y;
    unsigned short dx;
    unsigned short dy;
}QRect, AEERect;

typedef struct _QQ_FONT
{
    GUI_COLOR_T color;
    GUI_FONT_T 	font;
}QQ_FONT;

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description: set rect
//  Global resource dependence :  
//  Author: 
//  Note: 
/*****************************************************************************/
void SETAEERECT(QRect* rect_ptr, QU16 x, QU16 y, QU16 dx, QU16 dy);
/*****************************************************************************/
//  Description:QGetIDisplay
//  Global resource dependence :  
//  Author: 
//  Note: 
/*****************************************************************************/
IDisplay* QGetIDisplay(void);
/*****************************************************************************/
//  Description: IDISPLAY_DrawLine
//  Global resource dependence :  
//  Author: 
//  Note: 
/*****************************************************************************/
void IDISPLAY_DrawLine(IDisplay* pIDisplay, QU16 x1, QU16 y1, QU16 x2, QU16 y2, RGBVAL color);
/*****************************************************************************/
//  Description: IDISPLAY_FillRect
//  Global resource dependence :  
//  Author: 
//  Note: 
/*****************************************************************************/
void IDISPLAY_FillRect(IDisplay* pIDisplay, QRect* rect_ptr, RGBVAL color);
/*****************************************************************************/
//  Description: IDISPLAY_DrawRect
//  Global resource dependence :  
//  Author: 
//  Note: 
/*****************************************************************************/
void IDISPLAY_DrawRect(IDisplay* pIDisplay, QRect* rect_ptr, RGBVAL color1, RGBVAL color2, int flag);
/*****************************************************************************/
//  Description: IDISPLAY_SetColor
//  Global resource dependence :  
//  Author: 
//  Note: 
/*****************************************************************************/
void IDISPLAY_SetColor(IDisplay* pIDisplay, int type, RGBVAL color);
/*****************************************************************************/
//  Description: IDISPLAY_MeasureText2
//  Global resource dependence :  
//  Author: 
//  Note: 
/*****************************************************************************/
void IDISPLAY_MeasureText2(AECHAR* text_ptr,int* width_ptr, int* height_ptr);
/*****************************************************************************/
//  Description: IDISPLAY_MeasureText
//  Global resource dependence :  
//  Author: 
//  Note: 
/*****************************************************************************/
uint16	IDISPLAY_MeasureText(IDisplay* pIDisplay, int type, AECHAR* text_ptr);
/*****************************************************************************/
//  Description: IDISPLAY_GetFontMetrics
//  Global resource dependence :  
//  Author: 
//  Note: 
/*****************************************************************************/
uint8 IDISPLAY_GetFontMetrics(IDisplay* pIDisplay, GUI_FONT_T font, QU16 unicode, QU16 size);

/*****************************************************************************/
//  Description: IDISPLAY_GetFontHeight
//  Global resource dependence :  
//  Author: 
//  Note: 
/*****************************************************************************/
uint8	IDISPLAY_GetFontHeight(IDisplay* pIDisplay, GUI_FONT_T font, unsigned short unicode);
/*****************************************************************************/
//  Description: IDISPLAY_GetFontWidth
//  Global resource dependence :  
//  Author: 
//  Note: 
/*****************************************************************************/
uint8	IDISPLAY_GetFontWidth(IDisplay* pIDisplay, GUI_FONT_T font, unsigned short unicode);
/*****************************************************************************/
//  Description: IDISPLAY_MeasureTextEx
//  Global resource dependence :  
//  Author: 
//  Note: 
/*****************************************************************************/
uint16	IDISPLAY_MeasureTextEx(IDisplay* pIDisplay, int type, AECHAR* text_ptr, int len, uint16 width, int* num_ptr);

/*****************************************************************************/
//  Description: IDISPLAY_DrawText
//  Global resource dependence :  
//  Author: 
//  Note: 
/*****************************************************************************/
void IDISPLAY_DrawText(IDisplay* pIDisplay, GUI_FONT_T font, AECHAR* text_ptr, int len, QU16 x, QU16 y, QRect* rect_ptr, int flag);

/*****************************************************************************/
//  Description: IDISPLAY_GetClipBY
//  Global resource dependence :  
//  Author: 
//  Note: 
/*****************************************************************************/
QU16 IDISPLAY_GetClipBY(void);

/*遮罩效果实现*/
/*****************************************************************************/
//  Description: QDrawCoverInRect
//  Global resource dependence :  
//  Author: 
//  Note: 
/*****************************************************************************/
void QDrawCoverInRect(IDisplay* pIDisplay, QU16 x,QU16 y,QU16 w,QU16 h);

/* 渐变效果实现*/
/*****************************************************************************/
//  Description: IDISPLAY_DrawVGraded
//  Global resource dependence :  
//  Author: 
//  Note: 
/*****************************************************************************/
void IDISPLAY_DrawVGraded(IDisplay* pIDisplay, QU16 x1, QU16 y1, QU16 x2, QU16 y2, RGBVAL bColor, RGBVAL eColor);


/* 选择项高亮实现*/
/*****************************************************************************/
//  Description: QDrawChoose
//  Global resource dependence :  
//  Author: 
//  Note: 
/*****************************************************************************/
void QDrawChoose(IDisplay* pIDisplay,QSkinManger* skin_manger_ptr, QU16 x1, QU16 y1, QU16 x2, QU16 y2);


/*绘制三角*/
/*****************************************************************************/
//  Description: QDrawDelta
//  Global resource dependence :  
//  Author: 
//  Note: 
/*****************************************************************************/
void QDrawDelta(IDisplay* pIDisplay,QU16 x,QU16 y,QU16 h, RGBVAL fillColor, QU8 flag);

/*****************************************************************************/
//  Description: IDISPLAY_GetFontMetrics
//  Global resource dependence :  
//  Author: 
//  Note: 
/*****************************************************************************/
uint8 IDISPLAY_GetFontMetrics(IDisplay* pIDisplay, GUI_FONT_T font, QU16 unicode, QU16 size);

/*****************************************************************************/
//  Description: PointInRect1
//  Global resource dependence :  
//  Author: 
//  Note: 
/*****************************************************************************/
int PointInRect1(QS16 x1,QS16 y1,QS16 x,QS16 y,QS16 w,QS16 h);

/*****************************************************************************/
//  Description: IDISPLAY_Update
//  Global resource dependence :  
//  Author: 
//  Note: 
/*****************************************************************************/
void IDISPLAY_Update(IDisplay* pIDisplay);
/*****************************************************************************/
//  Description: IDISPLAY_SetClipRect
//  Global resource dependence :  
//  Author: 
//  Note: 
/*****************************************************************************/
void IDISPLAY_SetClipRect(IDisplay* pIDisplay, AEERect* rect_ptr);
/*****************************************************************************/
//  Description: IDISPLAY_GetClipRect
//  Global resource dependence :  
//  Author: 
//  Note: 
/*****************************************************************************/
void IDISPLAY_GetClipRect(IDisplay* pIDisplay, AEERect* rect_ptr);


/****************遮罩效果相关************************/
/*****************************************************************************/
//  Description: QChangeColor
//  Global resource dependence :  
//  Author: 
//  Note: 
/*****************************************************************************/
void QChangeColor(AEERect* rect_ptr, RGBVAL color1, RGBVAL color2);


/*圆角矩形实现*/
/*****************************************************************************/
//  Description: IDisplay_DrawRoundRect
//  Global resource dependence :  
//  Author: 
//  Note: 
/*****************************************************************************/
void IDisplay_DrawRoundRect(IDisplay* pIDisplay,QU16 x,QU16 y,QU16 w,QU16 h,RGBVAL c);
/*****************************************************************************/
//  Description: QDisplay_DrawRoundRect
//  Global resource dependence :  
//  Author: 
//  Note: 
/*****************************************************************************/
void QDisplay_DrawRoundRect(IDisplay* pIDisplay,QU16 x,QU16 y,QU16 w,QU16 h,RGBVAL color);


/*****************************************************************************/
//  Description: GetCharH
//  Global resource dependence :  
//  Author: 
//  Note: 
/*****************************************************************************/
QU16 GetCharH(void);
/*****************************************************************************/
//  Description: QGetTitleH
//  Global resource dependence :  
//  Author: 
//  Note: 
/*****************************************************************************/
int QGetTitleH(void);
/*****************************************************************************/
//  Description: PointInRect1
//  Global resource dependence :  
//  Author: 
//  Note: 
/*****************************************************************************/
int PointInRect1(QS16 x1,QS16 y1,QS16 x,QS16 y,QS16 w,QS16 h);
/*****************************************************************************/
//  Description: QDrawTitle
//  Global resource dependence :  
//  Author: 
//  Note: 
/*****************************************************************************/
void  QDrawTitle(IDisplay* pIDisplay, QSkinManger* skin_manger_ptr, AECHAR* title_ptr);

// 返回字高和表情最大的那个
/*****************************************************************************/
//  Description: QGetLineH
//  Global resource dependence :  
//  Author: 
//  Note: 
/*****************************************************************************/
int QGetLineH(IDisplay* pIDisplay);
/*****************************************************************************/
//  Description: QWStrdupEx
//  Global resource dependence :  
//  Author: 
//  Note: 
/*****************************************************************************/
QU16* QWStrdupEx(QU16* text_ptr, int w);
/*****************************************************************************/
//  Description: QGetTickTime
//  Global resource dependence :  
//  Author: 
//  Note: 
/*****************************************************************************/
QU32 QGetTickTime(void);
/*****************************************************************************/
//  Description: QWriteFileLog
//  Global resource dependence :  
//  Author: 
//  Note: 
/*****************************************************************************/
void QWriteFileLog(const char *psz,...);
/*****************************************************************************/
//  Description: QGetImgInfo
//  Global resource dependence :  
//  Author: 
//  Note: 
/*****************************************************************************/
unsigned char QGetImgInfo(unsigned char* img_ptr,unsigned short size,unsigned long* width_ptr,unsigned long* height_ptr);


/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif
