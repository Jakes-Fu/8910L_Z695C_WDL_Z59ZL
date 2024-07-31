/******************************************************************************/
//File Name:        McfGdi.h
//Author:           wilsonzhu wiltan
//DATE:             10/01/2011
//Copyright:        2011 Tencent, Incoporated. All Rights Reserved.
//Description:      provide simple gdi interface for mcare
//Modifed:          wilsonzhu 01/10/2011
/******************************************************************************/
#ifndef _MCFGDI_H_
#define _MCFGDI_H_ 

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern  "C"
    {
#endif
/**---------------------------------------------------------------------------*
 **                         Include Files                                     *
 **---------------------------------------------------------------------------*/ 
#include "mcfcommon.h"

/**---------------------------------------------------------------------------*
 **                         Macro Declaration    							  *
 **---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/

//---------------------------------------------------------------------------
//FUNCTION: McfGDI_GetTextColor
//Descrption:获取当前字体颜色       
//Param:
//  IN:pColor:文字的颜色信息
//  OUT:MINT
//Return: MCF_GDI_SUCCESS,MCF_GDI_ERROR
//Note:
//----------------------------------------------------------------------------
MColor McfGDI_GetTextColor(MVOID);

//---------------------------------------------------------------------------
//FUNCTION: McfGdi_SetTextColor
//Descrption:设置当前字体颜色          
//Param:
//  IN:r:红色成分
//      g:绿色成分
//      b:蓝色成分
//  OUT:MINT
//Return: MCF_GDI_SUCCESS,MCF_GDI_ERROR
//Note:
//----------------------------------------------------------------------------
MINT McfGdi_SetTextColor(MBYTE r, MBYTE g, MBYTE b);

//---------------------------------------------------------------------------
//FUNCTION: McfGdi_SetTextColor_Int
//Descrption:设置当前字体颜色          
//Param:
//  IN:rgb:文字的颜色信息
//  OUT:MINT
//Return: MCF_GDI_SUCCESS,MCF_GDI_ERROR
//Note:
//----------------------------------------------------------------------------
MINT McfGdi_SetTextColor_Int(MColor rgb);

//---------------------------------------------------------------------------
//FUNCTION: McfGdi_MakeMColorFromRGB
//Descrption:从rgb转换成MColor       
//Param:
//  IN:r:红色成分
//      g:绿色成分
//      b:蓝色成分
//  OUT:MIntColor
//Return: MIntColor
//Note:
//----------------------------------------------------------------------------
MColor McfGdi_MakeMColorFromRGB(MBYTE r, MBYTE g, MBYTE b);

//---------------------------------------------------------------------------
//FUNCTION: McfGdi_SetFont
//Descrption:设置字体    
//Param:
//  IN:fontSize:字体大小
//      fontAttr:字体属性
//  OUT:MINT
//Return: MCF_GDI_SUCCESS,MCF_GDI_ERROR
//Note:
//----------------------------------------------------------------------------
MINT McfGdi_SetFont(MINT fontSize, MINT fontAttr);

//---------------------------------------------------------------------------
//FUNCTION: McfGdi_GetCharWidth
//Descrption:返回字符宽度 
//Param:
//  IN:MVOID
//  OUT:MINT
//Return: 字符宽度 
//Note:
//----------------------------------------------------------------------------
MINT McfGdi_GetCharWidth(MVOID);

//---------------------------------------------------------------------------
//FUNCTION: McfGdi_GetCharHeight
//Descrption:返回字符高度
//Param:
//  IN:MVOID
//  OUT:MINT
//Return: 字符高度
//Note:
//----------------------------------------------------------------------------
MINT McfGdi_GetCharHeight(MVOID);

//---------------------------------------------------------------------------
//FUNCTION: McfGdi_GetTextWidth
//Descrption:返回字符串长度
//Param:
//  IN:pText:需要测量的字符串
//  OUT:MINT
//Return: 返回字符串长度,< 0 表示失败      
//Note:
//----------------------------------------------------------------------------
MINT McfGdi_GetTextWidth(MWCHAR* pText);

//---------------------------------------------------------------------------
//FUNCTION: McfGdi_GetNTextWidth
//Descrption:获取指定个数字符串的长度       
//Param:
//  IN:pText:需要测量的字符串
//  n:需要测量的字符个数
//  OUT:MINT
//Return: 获取指定个数字符串的长度       ,< 0 表示失败      
//Note:
//----------------------------------------------------------------------------
MINT McfGdi_GetNTextWidth(MWCHAR* pText, MINT n);

//---------------------------------------------------------------------------
//FUNCTION: McfGdi_GetTextWidthHeight
//Descrption:获取字符串的长度、高度    
//Param:
//  IN:pText:需要测量的字符串
//  pWidth:测量的长度,需外部申请
//  pHeight:测量的高度,需外部申请
//  OUT:MINT
//Return:获取字符串的长度、高度           
//Note:
//----------------------------------------------------------------------------
MVOID McfGdi_GetTextWidthHeight(MWCHAR *pText, MINT* pWidth, MINT* pHeight);

//---------------------------------------------------------------------------
//FUNCTION: McfGdi_GetNTextWidthHeight
//Descrption:获取指定个数字符串的长度
//Param:
//  IN:pText:需要测量的字符串
//  count:测量字符串的长度
//  pWidth:测量的长度,需外部申请
//  pHeight:测量的高度,需外部申请
//  OUT:MINT
//Return:获取指定个数字符串的长度
//Note:
//----------------------------------------------------------------------------
MVOID McfGdi_GetNTextWidthHeight(MWCHAR *pText, MINT count, MINT* pWidth, MINT* pHeight);

//---------------------------------------------------------------------------
//FUNCTION: McfGdi_DrawText
//Descrption:描画字符串
//Param:
//  IN:x:开始画的X坐标点
//  y:开始画的Y坐标点
//  pcwszStr:字符串
//  OUT:MINT
//Return:MCF_GDI_SUCCESS, MCF_GDI_ERROR,MCF_GDI_PARAM_ERR
//Note:
//----------------------------------------------------------------------------
MINT McfGdi_DrawText(MINT x, MINT y, const MWCHAR* pcwszStr);

//---------------------------------------------------------------------------
//FUNCTION: McfGdi_DrawText
//Descrption:描画指定长度的字符串
//Param:
//  IN:x:开始画的X坐标点
//  y:开始画的Y坐标点
//  pcwszStr:字符串
//  nLen:指定长度
//  OUT:MINT
//Return:MCF_GDI_SUCCESS, MCF_GDI_ERROR,MCF_GDI_PARAM_ERR
//Note:
//----------------------------------------------------------------------------
MINT McfGdi_DrawTextN(MINT x, MINT y, const MWCHAR* pcwszStr, MINT nLen);

//---------------------------------------------------------------------------
//FUNCTION: McfGdi_DrawPoint
//Descrption:画点 
//Param:
//  IN:x:开始画的X坐标点
//  y:开始画的Y坐标点
//  pixel_color:画点的颜色 
//  OUT:MVOID
//Return:
//Note:
//----------------------------------------------------------------------------
MVOID McfGdi_DrawPoint(MINT x, MINT y, MColor pixel_color);

//---------------------------------------------------------------------------
//FUNCTION: McfGdi_DrawRect
//Descrption:画矩形，只是画border,x1,y1,x2,y2均会画出  
//Param:
//  IN:x1:开始画矩形的左上方的x坐标点
//  y1:开始画矩形的左上方的y坐标点
//  x2:矩形的右下方的x坐标点
//  y2:矩形的右下方的y坐标点
//  frame_color:边框颜色
//  OUT:MVOID
//Return:
//Note:
//----------------------------------------------------------------------------
MVOID McfGdi_DrawRect(MINT x1, MINT y1, MINT x2, MINT y2, MColor frame_color);

//---------------------------------------------------------------------------
//FUNCTION: McfGdi_FillRect
//Descrption:画矩形，填充整个矩形
//Param:
//  IN:x1:开始画矩形的左上方的x坐标点
//  y1:开始画矩形的左上方的y坐标点
//  x2:矩形的右下方的x坐标点
//  y2:矩形的右下方的y坐标点
//  rect_color:填充的颜色 
//  OUT:MVOID
//Return:
//Note:
//----------------------------------------------------------------------------
MVOID McfGdi_FillRect(MINT x1, MINT y1, MINT x2, MINT y2, MColor rect_color);

//---------------------------------------------------------------------------
//FUNCTION: McfGdi_DrawFrameRect
//Descrption:画矩形,填充并画border         
//Param:
//  IN:x1:开始画矩形的左上方的x坐标点
//  y1:开始画矩形的左上方的y坐标点
//  x2:矩形的右下方的x坐标点
//  y2:矩形的右下方的y坐标点
//  rect_color:填充的颜色 
//  frame_color:边框颜色
//  OUT:MVOID
//Return:
//Note:
//----------------------------------------------------------------------------
MVOID McfGdi_DrawFrameRect(MINT x1, MINT y1, MINT x2, MINT y2, MColor rect_color, MColor frame_color);

//---------------------------------------------------------------------------
//FUNCTION: McfGdi_FillGradRect
//Descrption:横向渐变画矩形 
//Param:
//  IN:x1:开始画矩形的左上方的x坐标点
//  y1:开始画矩形的左上方的y坐标点
//  x2:矩形的右下方的x坐标点
//  y2:矩形的右下方的y坐标点
//  start_color:开始颜色
//  end_color:结束颜色
//  OUT:MVOID
//Return:
//Note:
//----------------------------------------------------------------------------
MVOID McfGdi_FillGradRect(MINT x1, MINT y1, MINT x2, MINT y2, MColor start_color, MColor end_color);

//---------------------------------------------------------------------------
//FUNCTION: McfGdi_FillVGradRect
//Descrption:纵向渐变画矩形
//Param:
//  IN:x1:开始画矩形的左上方的x坐标点
//  y1:开始画矩形的左上方的y坐标点
//  x2:矩形的右下方的x坐标点
//  y2:矩形的右下方的y坐标点
//  start_color:开始颜色
//  end_color:结束颜色
//  OUT:MVOID
//Return:
//Note:
//----------------------------------------------------------------------------
MVOID McfGdi_FillVGradRect(MINT x1, MINT y1, MINT x2, MINT y2, MColor start_color, MColor end_color);

//---------------------------------------------------------------------------
//FUNCTION: McfGdi_DrawLine
//Descrption:画线 
//Param:
//  IN:x1:画线开始x坐标点
//  y1:画线开始y坐标点
//  x2:画线结束x坐标点
//  y2:画线结束y坐标点
//  line_color:画线的颜色 
//  OUT:MVOID
//Return:
//Note:
//----------------------------------------------------------------------------
MVOID McfGdi_DrawLine(MINT x1, MINT y1, MINT x2, MINT y2, MColor line_color);

//---------------------------------------------------------------------------
//FUNCTION: McfGdi_DrawCircle
//Descrption:画线 
//Param:
//  IN:x:圆点x坐标点
//  y:圆点y坐标点
//  r:画线结束x坐标点
//  y2:圆半径
//  circle_color:圆线颜色
//  OUT:MVOID
//Return:
//Note:
//----------------------------------------------------------------------------
MVOID McfGdi_DrawCircle(MINT x, MINT y, MINT r, MColor circle_color);

//---------------------------------------------------------------------------
//FUNCTION: McfGdi_GetScrWidth
//Descrption:获取屏幕宽度
//Param:
//  IN:MVOID
//  OUT:MINT
//Return:屏幕宽度
//Note:
//----------------------------------------------------------------------------
MINT McfGdi_GetScrWidth(MVOID);

//---------------------------------------------------------------------------
//FUNCTION: McfGdi_GetScrHeight
//Descrption:获取屏幕高度
//Param:
//  IN:MVOID
//  OUT:MINT
//Return:屏幕高度
//Note:
//----------------------------------------------------------------------------
MINT McfGdi_GetScrHeight(MVOID);

//---------------------------------------------------------------------------
//FUNCTION: McfGdi_BeginDraw
//Descrption:屏幕开始画图
//Param:
//  IN:MVOID
//  OUT:MVOID
//Return:MVOID
//Note:
//----------------------------------------------------------------------------
MVOID McfGdi_BeginDraw(MVOID);

//---------------------------------------------------------------------------
//FUNCTION: McfGdi_EndDraw
//Descrption:屏幕结束画图
//Param:
//  IN:MVOID
//  OUT:MVOID
//Return:MVOID
//Note:
//----------------------------------------------------------------------------
MVOID McfGdi_EndDraw(MVOID);

//---------------------------------------------------------------------------
//FUNCTION: McfGdi_EndDraw
//Descrption:获取当前屏幕的首地址 
//Param:
//  IN:MVOID
//  OUT:MBYTE*
//Return:MBYTE*
//Note:
//----------------------------------------------------------------------------
MBYTE* McfGdi_GetBufferPtr(MVOID);

//---------------------------------------------------------------------------
//FUNCTION: McfGdi_GetPixel
//Descrption:获取某个位置的像素点
//Param:
//  IN:x:像素点x坐标点
//  y:像素点y坐标点
//  OUT:MIntColor
//Return:MIntColor,失败 - 小于0
//Note:
//----------------------------------------------------------------------------
MColor McfGdi_GetPixel(MINT x, MINT y);

//---------------------------------------------------------------------------
//FUNCTION: McfGdi_PushClip
//Descrption:将当前裁剪区域压栈
//Param:
//  IN:MVOID
//  OUT:MVOID
//Return:MVOID
//Note:
//----------------------------------------------------------------------------
MVOID McfGdi_PushClip(MVOID);

//---------------------------------------------------------------------------
//FUNCTION: McfGdi_PopClip
//Descrption:将栈中的最顶层裁剪区域弹出 
//Param:
//  IN:MVOID
//  OUT:MVOID
//Return:MVOID
//Note:
//----------------------------------------------------------------------------
MVOID McfGdi_PopClip(MVOID );

//---------------------------------------------------------------------------
//FUNCTION: McfGdi_SetClip
//Descrption:设置剪切域
//Param:
//  IN:x1:设置剪切域的开始x坐标点
//  y1:设置剪切域的开始y坐标点
//  x2:设置剪切域的结束x坐标点
//  y2:设置剪切域的结束y坐标点
//  OUT:MVOID
//Return:MVOID
//Note:
//----------------------------------------------------------------------------
MVOID McfGdi_SetClip(MINT x1, MINT y1, MINT x2, MINT y2);

//---------------------------------------------------------------------------
//FUNCTION: McfGdi_GetClip
//Descrption:获取剪切域
//Param:
//  IN:x1:获取剪切域的开始x坐标点
//  y1:获取剪切域的开始y坐标点
//  x2:获取剪切域的结束x坐标点
//  y2:获取剪切域的结束y坐标点
//  OUT:MINT
//Return:MCF_GDI_SUCCESS, MCF_GDI_ERROR,MCF_GDI_PARAM_ERR
//Note:
//----------------------------------------------------------------------------
MINT McfGdi_GetClip(MINT* x, MINT* y, MINT* x2, MINT* y2);

//---------------------------------------------------------------------------
//FUNCTION: McfGdi_ResetClip
//Descrption:重置剪切域
//Param:
//  IN:MVOID
//  OUT:MVOID
//Return:MVOID
//Note:
//----------------------------------------------------------------------------
MVOID McfGdi_ResetClip(MVOID);

//---------------------------------------------------------------------------
//FUNCTION: McfGdi_DrawImage
//Descrption:画图
//Param:
//  IN:x:画图片开始的x坐标点
//  y:画图片开始的y坐标点
//  pImgData:图片buffer
//  nDataSize:图片buffer大小
//  OUT:MINT
//Return:MCF_GDI_SUCCESS, MCF_GDI_ERROR,MCF_GDI_PARAM_ERR
//Note:
//----------------------------------------------------------------------------
MINT McfGdi_DrawImage(MINT x, MINT y, const MBYTE* pImgData, MUINT nDataSize);

//---------------------------------------------------------------------------
//FUNCTION: McfGdi_GetImageInfo
//Descrption:获取图片信息  
//Param:
//  IN:pImgData:图片buffer
//  nDataSize:图片buffer大小
//  pMImageInfo:图片信息结构体,需外部申请
//  OUT:MINT
//Return:MCF_GDI_SUCCESS, MCF_GDI_ERROR,MCF_GDI_PARAM_ERR
//Note:
//----------------------------------------------------------------------------
MINT McfGdi_GetImageInfo(const MBYTE* pImageData, MUINT nDataSize, MImageInfo* pMImageInfo);

//---------------------------------------------------------------------------
//FUNCTION: McfGdi_Init
//Descrption:初始化GDI
//Param:
//  IN:MVOID
//  OUT:MVOID
//Return:MVOID
//Note:
//----------------------------------------------------------------------------
MVOID McfGdi_Init(MVOID);

//---------------------------------------------------------------------------
//FUNCTION: McfGdi_DeInit
//Descrption:反初始化GDI
//Param:
//  IN:MVOID
//  OUT:MVOID
//Return:MVOID
//Note:
//----------------------------------------------------------------------------
MVOID McfGdi_DeInit(MVOID);

//---------------------------------------------------------------------------
//FUNCTION: McfGdi_Refresh
//Descrption:刷新区域
//Param:
//  IN:x1:刷新区域的开始x坐标点
//  y1:刷新区域的开始y坐标点
//  x2:刷新区域的结束x坐标点
//  y2:刷新区域的结束y坐标点
//  OUT:MVOID
//Return:MVOID
//Note:
//----------------------------------------------------------------------------
MBOOL McfGdi_Refresh(MINT x1, MINT y1, MINT x2, MINT y2);

//---------------------------------------------------------------------------
//FUNCTION: McfGdi_IsDecodeGif
//Descrption:是否需要解码描画图片
//Param:
//  IN:MVOID
//  OUT:MBOOL
//Return:MBOOL
//Note:
//----------------------------------------------------------------------------
MBOOL McfGdi_IsDecodeGif(MVOID);

MVOID McfGdi_SetIsDecodeGif(MBOOL is_decode);

#if (defined SPREAD_PLAT_SC6800H) ||(defined SPREAD_PLAT_SC8800G) || (defined SPREAD_PLAT_SC6530) || (defined SPREAD_PLAT_SC8800G_PDA)

//---------------------------------------------------------------------------
//FUNCTION: McfGDI_GetGlobalFont
//Descrption:获取当前设置的字体
//Param:
//  IN:MVOID
//  OUT:MUINT
//Return:MUINT
//Note:
//----------------------------------------------------------------------------
MUINT McfGDI_GetGlobalFont(MVOID);

//---------------------------------------------------------------------------
//FUNCTION: McfGDI_GetGlobalColor
//Descrption:获取当前设置的字体颜色
//Param:
//  IN:MVOID
//  OUT:MUINT
//Return:MUINT
//Note:
//----------------------------------------------------------------------------
MUINT McfGDI_GetGlobalColor(MVOID);

//---------------------------------------------------------------------------
//FUNCTION: McfGDI_GetGlobalColor
//Descrption:设置当前的字体
//Param:
//  IN:font:字体
//  OUT:MVOID
//Return:MVOID
//Note:
//----------------------------------------------------------------------------
MVOID McfGDI_SetGlobalFont(MUINT font);


#endif

MVOID McfGDI_GetCurClip(MSHORT* pTop, MSHORT* pLeft, MSHORT* pBottom, MSHORT* pRight);

MVOID McfGdi_DisplayWarnning(MWCHAR *pMessage);

//新增API
MINT McfGdi_GetRGBFromMColor(MBYTE* pR, MBYTE* pG, MBYTE* pB, MColor nColor);


//---------------------------------------------------------------------------
//	FUNCTION: McfGdi_IsTouchScreen
//	Descrption:是否触屏
//	Param:
//          MVOID	   
//	Return: 
//          MTRUE:Yes
//		    MFALSE: No
//    Note:
//----------------------------------------------------------------------------
MBOOL McfGdi_IsTouchScreen(MVOID);

//---------------------------------------------------------------------------
//FUNCTION: McfGdi_Init
//Descrption:初始化GDI
//Param:
//  IN:MVOID
//  OUT:MVOID
//Return:MVOID
//Note:
//----------------------------------------------------------------------------
MVOID McfGdi_Reinit(MVOID);

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
    }
#endif


#endif //_MCFGDI_H_


