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
//Descrption:��ȡ��ǰ������ɫ       
//Param:
//  IN:pColor:���ֵ���ɫ��Ϣ
//  OUT:MINT
//Return: MCF_GDI_SUCCESS,MCF_GDI_ERROR
//Note:
//----------------------------------------------------------------------------
MColor McfGDI_GetTextColor(MVOID);

//---------------------------------------------------------------------------
//FUNCTION: McfGdi_SetTextColor
//Descrption:���õ�ǰ������ɫ          
//Param:
//  IN:r:��ɫ�ɷ�
//      g:��ɫ�ɷ�
//      b:��ɫ�ɷ�
//  OUT:MINT
//Return: MCF_GDI_SUCCESS,MCF_GDI_ERROR
//Note:
//----------------------------------------------------------------------------
MINT McfGdi_SetTextColor(MBYTE r, MBYTE g, MBYTE b);

//---------------------------------------------------------------------------
//FUNCTION: McfGdi_SetTextColor_Int
//Descrption:���õ�ǰ������ɫ          
//Param:
//  IN:rgb:���ֵ���ɫ��Ϣ
//  OUT:MINT
//Return: MCF_GDI_SUCCESS,MCF_GDI_ERROR
//Note:
//----------------------------------------------------------------------------
MINT McfGdi_SetTextColor_Int(MColor rgb);

//---------------------------------------------------------------------------
//FUNCTION: McfGdi_MakeMColorFromRGB
//Descrption:��rgbת����MColor       
//Param:
//  IN:r:��ɫ�ɷ�
//      g:��ɫ�ɷ�
//      b:��ɫ�ɷ�
//  OUT:MIntColor
//Return: MIntColor
//Note:
//----------------------------------------------------------------------------
MColor McfGdi_MakeMColorFromRGB(MBYTE r, MBYTE g, MBYTE b);

//---------------------------------------------------------------------------
//FUNCTION: McfGdi_SetFont
//Descrption:��������    
//Param:
//  IN:fontSize:�����С
//      fontAttr:��������
//  OUT:MINT
//Return: MCF_GDI_SUCCESS,MCF_GDI_ERROR
//Note:
//----------------------------------------------------------------------------
MINT McfGdi_SetFont(MINT fontSize, MINT fontAttr);

//---------------------------------------------------------------------------
//FUNCTION: McfGdi_GetCharWidth
//Descrption:�����ַ���� 
//Param:
//  IN:MVOID
//  OUT:MINT
//Return: �ַ���� 
//Note:
//----------------------------------------------------------------------------
MINT McfGdi_GetCharWidth(MVOID);

//---------------------------------------------------------------------------
//FUNCTION: McfGdi_GetCharHeight
//Descrption:�����ַ��߶�
//Param:
//  IN:MVOID
//  OUT:MINT
//Return: �ַ��߶�
//Note:
//----------------------------------------------------------------------------
MINT McfGdi_GetCharHeight(MVOID);

//---------------------------------------------------------------------------
//FUNCTION: McfGdi_GetTextWidth
//Descrption:�����ַ�������
//Param:
//  IN:pText:��Ҫ�������ַ���
//  OUT:MINT
//Return: �����ַ�������,< 0 ��ʾʧ��      
//Note:
//----------------------------------------------------------------------------
MINT McfGdi_GetTextWidth(MWCHAR* pText);

//---------------------------------------------------------------------------
//FUNCTION: McfGdi_GetNTextWidth
//Descrption:��ȡָ�������ַ����ĳ���       
//Param:
//  IN:pText:��Ҫ�������ַ���
//  n:��Ҫ�������ַ�����
//  OUT:MINT
//Return: ��ȡָ�������ַ����ĳ���       ,< 0 ��ʾʧ��      
//Note:
//----------------------------------------------------------------------------
MINT McfGdi_GetNTextWidth(MWCHAR* pText, MINT n);

//---------------------------------------------------------------------------
//FUNCTION: McfGdi_GetTextWidthHeight
//Descrption:��ȡ�ַ����ĳ��ȡ��߶�    
//Param:
//  IN:pText:��Ҫ�������ַ���
//  pWidth:�����ĳ���,���ⲿ����
//  pHeight:�����ĸ߶�,���ⲿ����
//  OUT:MINT
//Return:��ȡ�ַ����ĳ��ȡ��߶�           
//Note:
//----------------------------------------------------------------------------
MVOID McfGdi_GetTextWidthHeight(MWCHAR *pText, MINT* pWidth, MINT* pHeight);

//---------------------------------------------------------------------------
//FUNCTION: McfGdi_GetNTextWidthHeight
//Descrption:��ȡָ�������ַ����ĳ���
//Param:
//  IN:pText:��Ҫ�������ַ���
//  count:�����ַ����ĳ���
//  pWidth:�����ĳ���,���ⲿ����
//  pHeight:�����ĸ߶�,���ⲿ����
//  OUT:MINT
//Return:��ȡָ�������ַ����ĳ���
//Note:
//----------------------------------------------------------------------------
MVOID McfGdi_GetNTextWidthHeight(MWCHAR *pText, MINT count, MINT* pWidth, MINT* pHeight);

//---------------------------------------------------------------------------
//FUNCTION: McfGdi_DrawText
//Descrption:�軭�ַ���
//Param:
//  IN:x:��ʼ����X�����
//  y:��ʼ����Y�����
//  pcwszStr:�ַ���
//  OUT:MINT
//Return:MCF_GDI_SUCCESS, MCF_GDI_ERROR,MCF_GDI_PARAM_ERR
//Note:
//----------------------------------------------------------------------------
MINT McfGdi_DrawText(MINT x, MINT y, const MWCHAR* pcwszStr);

//---------------------------------------------------------------------------
//FUNCTION: McfGdi_DrawText
//Descrption:�軭ָ�����ȵ��ַ���
//Param:
//  IN:x:��ʼ����X�����
//  y:��ʼ����Y�����
//  pcwszStr:�ַ���
//  nLen:ָ������
//  OUT:MINT
//Return:MCF_GDI_SUCCESS, MCF_GDI_ERROR,MCF_GDI_PARAM_ERR
//Note:
//----------------------------------------------------------------------------
MINT McfGdi_DrawTextN(MINT x, MINT y, const MWCHAR* pcwszStr, MINT nLen);

//---------------------------------------------------------------------------
//FUNCTION: McfGdi_DrawPoint
//Descrption:���� 
//Param:
//  IN:x:��ʼ����X�����
//  y:��ʼ����Y�����
//  pixel_color:�������ɫ 
//  OUT:MVOID
//Return:
//Note:
//----------------------------------------------------------------------------
MVOID McfGdi_DrawPoint(MINT x, MINT y, MColor pixel_color);

//---------------------------------------------------------------------------
//FUNCTION: McfGdi_DrawRect
//Descrption:�����Σ�ֻ�ǻ�border,x1,y1,x2,y2���ử��  
//Param:
//  IN:x1:��ʼ�����ε����Ϸ���x�����
//  y1:��ʼ�����ε����Ϸ���y�����
//  x2:���ε����·���x�����
//  y2:���ε����·���y�����
//  frame_color:�߿���ɫ
//  OUT:MVOID
//Return:
//Note:
//----------------------------------------------------------------------------
MVOID McfGdi_DrawRect(MINT x1, MINT y1, MINT x2, MINT y2, MColor frame_color);

//---------------------------------------------------------------------------
//FUNCTION: McfGdi_FillRect
//Descrption:�����Σ������������
//Param:
//  IN:x1:��ʼ�����ε����Ϸ���x�����
//  y1:��ʼ�����ε����Ϸ���y�����
//  x2:���ε����·���x�����
//  y2:���ε����·���y�����
//  rect_color:������ɫ 
//  OUT:MVOID
//Return:
//Note:
//----------------------------------------------------------------------------
MVOID McfGdi_FillRect(MINT x1, MINT y1, MINT x2, MINT y2, MColor rect_color);

//---------------------------------------------------------------------------
//FUNCTION: McfGdi_DrawFrameRect
//Descrption:������,��䲢��border         
//Param:
//  IN:x1:��ʼ�����ε����Ϸ���x�����
//  y1:��ʼ�����ε����Ϸ���y�����
//  x2:���ε����·���x�����
//  y2:���ε����·���y�����
//  rect_color:������ɫ 
//  frame_color:�߿���ɫ
//  OUT:MVOID
//Return:
//Note:
//----------------------------------------------------------------------------
MVOID McfGdi_DrawFrameRect(MINT x1, MINT y1, MINT x2, MINT y2, MColor rect_color, MColor frame_color);

//---------------------------------------------------------------------------
//FUNCTION: McfGdi_FillGradRect
//Descrption:���򽥱仭���� 
//Param:
//  IN:x1:��ʼ�����ε����Ϸ���x�����
//  y1:��ʼ�����ε����Ϸ���y�����
//  x2:���ε����·���x�����
//  y2:���ε����·���y�����
//  start_color:��ʼ��ɫ
//  end_color:������ɫ
//  OUT:MVOID
//Return:
//Note:
//----------------------------------------------------------------------------
MVOID McfGdi_FillGradRect(MINT x1, MINT y1, MINT x2, MINT y2, MColor start_color, MColor end_color);

//---------------------------------------------------------------------------
//FUNCTION: McfGdi_FillVGradRect
//Descrption:���򽥱仭����
//Param:
//  IN:x1:��ʼ�����ε����Ϸ���x�����
//  y1:��ʼ�����ε����Ϸ���y�����
//  x2:���ε����·���x�����
//  y2:���ε����·���y�����
//  start_color:��ʼ��ɫ
//  end_color:������ɫ
//  OUT:MVOID
//Return:
//Note:
//----------------------------------------------------------------------------
MVOID McfGdi_FillVGradRect(MINT x1, MINT y1, MINT x2, MINT y2, MColor start_color, MColor end_color);

//---------------------------------------------------------------------------
//FUNCTION: McfGdi_DrawLine
//Descrption:���� 
//Param:
//  IN:x1:���߿�ʼx�����
//  y1:���߿�ʼy�����
//  x2:���߽���x�����
//  y2:���߽���y�����
//  line_color:���ߵ���ɫ 
//  OUT:MVOID
//Return:
//Note:
//----------------------------------------------------------------------------
MVOID McfGdi_DrawLine(MINT x1, MINT y1, MINT x2, MINT y2, MColor line_color);

//---------------------------------------------------------------------------
//FUNCTION: McfGdi_DrawCircle
//Descrption:���� 
//Param:
//  IN:x:Բ��x�����
//  y:Բ��y�����
//  r:���߽���x�����
//  y2:Բ�뾶
//  circle_color:Բ����ɫ
//  OUT:MVOID
//Return:
//Note:
//----------------------------------------------------------------------------
MVOID McfGdi_DrawCircle(MINT x, MINT y, MINT r, MColor circle_color);

//---------------------------------------------------------------------------
//FUNCTION: McfGdi_GetScrWidth
//Descrption:��ȡ��Ļ���
//Param:
//  IN:MVOID
//  OUT:MINT
//Return:��Ļ���
//Note:
//----------------------------------------------------------------------------
MINT McfGdi_GetScrWidth(MVOID);

//---------------------------------------------------------------------------
//FUNCTION: McfGdi_GetScrHeight
//Descrption:��ȡ��Ļ�߶�
//Param:
//  IN:MVOID
//  OUT:MINT
//Return:��Ļ�߶�
//Note:
//----------------------------------------------------------------------------
MINT McfGdi_GetScrHeight(MVOID);

//---------------------------------------------------------------------------
//FUNCTION: McfGdi_BeginDraw
//Descrption:��Ļ��ʼ��ͼ
//Param:
//  IN:MVOID
//  OUT:MVOID
//Return:MVOID
//Note:
//----------------------------------------------------------------------------
MVOID McfGdi_BeginDraw(MVOID);

//---------------------------------------------------------------------------
//FUNCTION: McfGdi_EndDraw
//Descrption:��Ļ������ͼ
//Param:
//  IN:MVOID
//  OUT:MVOID
//Return:MVOID
//Note:
//----------------------------------------------------------------------------
MVOID McfGdi_EndDraw(MVOID);

//---------------------------------------------------------------------------
//FUNCTION: McfGdi_EndDraw
//Descrption:��ȡ��ǰ��Ļ���׵�ַ 
//Param:
//  IN:MVOID
//  OUT:MBYTE*
//Return:MBYTE*
//Note:
//----------------------------------------------------------------------------
MBYTE* McfGdi_GetBufferPtr(MVOID);

//---------------------------------------------------------------------------
//FUNCTION: McfGdi_GetPixel
//Descrption:��ȡĳ��λ�õ����ص�
//Param:
//  IN:x:���ص�x�����
//  y:���ص�y�����
//  OUT:MIntColor
//Return:MIntColor,ʧ�� - С��0
//Note:
//----------------------------------------------------------------------------
MColor McfGdi_GetPixel(MINT x, MINT y);

//---------------------------------------------------------------------------
//FUNCTION: McfGdi_PushClip
//Descrption:����ǰ�ü�����ѹջ
//Param:
//  IN:MVOID
//  OUT:MVOID
//Return:MVOID
//Note:
//----------------------------------------------------------------------------
MVOID McfGdi_PushClip(MVOID);

//---------------------------------------------------------------------------
//FUNCTION: McfGdi_PopClip
//Descrption:��ջ�е����ü����򵯳� 
//Param:
//  IN:MVOID
//  OUT:MVOID
//Return:MVOID
//Note:
//----------------------------------------------------------------------------
MVOID McfGdi_PopClip(MVOID );

//---------------------------------------------------------------------------
//FUNCTION: McfGdi_SetClip
//Descrption:���ü�����
//Param:
//  IN:x1:���ü�����Ŀ�ʼx�����
//  y1:���ü�����Ŀ�ʼy�����
//  x2:���ü�����Ľ���x�����
//  y2:���ü�����Ľ���y�����
//  OUT:MVOID
//Return:MVOID
//Note:
//----------------------------------------------------------------------------
MVOID McfGdi_SetClip(MINT x1, MINT y1, MINT x2, MINT y2);

//---------------------------------------------------------------------------
//FUNCTION: McfGdi_GetClip
//Descrption:��ȡ������
//Param:
//  IN:x1:��ȡ������Ŀ�ʼx�����
//  y1:��ȡ������Ŀ�ʼy�����
//  x2:��ȡ������Ľ���x�����
//  y2:��ȡ������Ľ���y�����
//  OUT:MINT
//Return:MCF_GDI_SUCCESS, MCF_GDI_ERROR,MCF_GDI_PARAM_ERR
//Note:
//----------------------------------------------------------------------------
MINT McfGdi_GetClip(MINT* x, MINT* y, MINT* x2, MINT* y2);

//---------------------------------------------------------------------------
//FUNCTION: McfGdi_ResetClip
//Descrption:���ü�����
//Param:
//  IN:MVOID
//  OUT:MVOID
//Return:MVOID
//Note:
//----------------------------------------------------------------------------
MVOID McfGdi_ResetClip(MVOID);

//---------------------------------------------------------------------------
//FUNCTION: McfGdi_DrawImage
//Descrption:��ͼ
//Param:
//  IN:x:��ͼƬ��ʼ��x�����
//  y:��ͼƬ��ʼ��y�����
//  pImgData:ͼƬbuffer
//  nDataSize:ͼƬbuffer��С
//  OUT:MINT
//Return:MCF_GDI_SUCCESS, MCF_GDI_ERROR,MCF_GDI_PARAM_ERR
//Note:
//----------------------------------------------------------------------------
MINT McfGdi_DrawImage(MINT x, MINT y, const MBYTE* pImgData, MUINT nDataSize);

//---------------------------------------------------------------------------
//FUNCTION: McfGdi_GetImageInfo
//Descrption:��ȡͼƬ��Ϣ  
//Param:
//  IN:pImgData:ͼƬbuffer
//  nDataSize:ͼƬbuffer��С
//  pMImageInfo:ͼƬ��Ϣ�ṹ��,���ⲿ����
//  OUT:MINT
//Return:MCF_GDI_SUCCESS, MCF_GDI_ERROR,MCF_GDI_PARAM_ERR
//Note:
//----------------------------------------------------------------------------
MINT McfGdi_GetImageInfo(const MBYTE* pImageData, MUINT nDataSize, MImageInfo* pMImageInfo);

//---------------------------------------------------------------------------
//FUNCTION: McfGdi_Init
//Descrption:��ʼ��GDI
//Param:
//  IN:MVOID
//  OUT:MVOID
//Return:MVOID
//Note:
//----------------------------------------------------------------------------
MVOID McfGdi_Init(MVOID);

//---------------------------------------------------------------------------
//FUNCTION: McfGdi_DeInit
//Descrption:����ʼ��GDI
//Param:
//  IN:MVOID
//  OUT:MVOID
//Return:MVOID
//Note:
//----------------------------------------------------------------------------
MVOID McfGdi_DeInit(MVOID);

//---------------------------------------------------------------------------
//FUNCTION: McfGdi_Refresh
//Descrption:ˢ������
//Param:
//  IN:x1:ˢ������Ŀ�ʼx�����
//  y1:ˢ������Ŀ�ʼy�����
//  x2:ˢ������Ľ���x�����
//  y2:ˢ������Ľ���y�����
//  OUT:MVOID
//Return:MVOID
//Note:
//----------------------------------------------------------------------------
MBOOL McfGdi_Refresh(MINT x1, MINT y1, MINT x2, MINT y2);

//---------------------------------------------------------------------------
//FUNCTION: McfGdi_IsDecodeGif
//Descrption:�Ƿ���Ҫ�����軭ͼƬ
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
//Descrption:��ȡ��ǰ���õ�����
//Param:
//  IN:MVOID
//  OUT:MUINT
//Return:MUINT
//Note:
//----------------------------------------------------------------------------
MUINT McfGDI_GetGlobalFont(MVOID);

//---------------------------------------------------------------------------
//FUNCTION: McfGDI_GetGlobalColor
//Descrption:��ȡ��ǰ���õ�������ɫ
//Param:
//  IN:MVOID
//  OUT:MUINT
//Return:MUINT
//Note:
//----------------------------------------------------------------------------
MUINT McfGDI_GetGlobalColor(MVOID);

//---------------------------------------------------------------------------
//FUNCTION: McfGDI_GetGlobalColor
//Descrption:���õ�ǰ������
//Param:
//  IN:font:����
//  OUT:MVOID
//Return:MVOID
//Note:
//----------------------------------------------------------------------------
MVOID McfGDI_SetGlobalFont(MUINT font);


#endif

MVOID McfGDI_GetCurClip(MSHORT* pTop, MSHORT* pLeft, MSHORT* pBottom, MSHORT* pRight);

MVOID McfGdi_DisplayWarnning(MWCHAR *pMessage);

//����API
MINT McfGdi_GetRGBFromMColor(MBYTE* pR, MBYTE* pG, MBYTE* pB, MColor nColor);


//---------------------------------------------------------------------------
//	FUNCTION: McfGdi_IsTouchScreen
//	Descrption:�Ƿ���
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
//Descrption:��ʼ��GDI
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


