/*
 * Copyright 2003 Aplix Corporation. All Rights Reserved.
 *
 * JBlend and all Aplix- or JBlend-related trademarks and logos are
 * trademarks or registered trademarks of Aplix in the U.S., Japan and
 * other countries.
 */
/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmi_app_ia_java_trc.h"
#include "std_header.h"
#ifdef JAVA_SUPPORT_IA
#include "jktypes.h"
#include "jksoftkey.h"
#include "jkvideopermissiondialog.h"
#include "kjava_sys_gfx.h"

#include "os_api.h"
//#include "mmiset.h"
#include "mmiset_export.h"

/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
#define SOFTKEY_HARD_MAX         (3)
#define MMIJAVA_WHITE_COLOR 0xFFFF
#define MMIJAVA_BLACK_COLOR 0x0000
#define LEFT_EDGE   0x1
#define RIGHT_EDGE  0x2
#define BOTTOM_EDGE 0x4
#define TOP_EDGE    0x8

#define INSIDE(a)   (!a)
#define REJECT(a,b) (a&b)
#define ACCEPT(a,b) (!(a|b))

#define JAVA_MIN_FLOAT_NUM_0 -0.000001
#define JAVA_MAX_FLOAT_NUM_0  0.000001
 /**-------------------------------------------------------------------------*
 **                         TYPE AND CONSTANT                                *
 **--------------------------------------------------------------------------*/
// typedef struct _JkAjcGraphicsContextTag
// {
//     KJavaImage*     offscreen;
//     KJavaClip       clip;
//     KJavaPixel      drawColor;       
//     uint32    trueColor;
//     int32             lineStyle;
//     KJavaFont       *font;
//     uint32    attribute;
// }JkAjcGraphicsContext;

/**--------------------------------------------------------------------------*
 **                         STATIC DEFINITION                                *
 **--------------------------------------------------------------------------*/
static JKWChar      button_labels   [SOFTKEY_HARD_MAX][20] = {0};
static JKSint32     button_labelLens[SOFTKEY_HARD_MAX] = {0};
static JKSint32     scrollState = 0;
static JKSint32     focusState = 0;
static BOOLEAN      flagChangeLabel = FALSE;

/**--------------------------------------------------------------------------*
 **                         EXTERNAL DECLARE                                 *
 **--------------------------------------------------------------------------*/
extern KJavaFont *cacheDefaultFont;


/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : 画滚动箭头
//  Global resource dependence :
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void drawScrollArrow( 
                KJavaImage* pVirtual,
                KJavaClip*  softClip, 
                JKSint32    state
                );

/*****************************************************************************/
//  Description : 画线
//  Global resource dependence :
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void _ref_drawLine (
                           KJavaImage   *dst,
                           KJavaClip    *clip,
                           KJavaPixel   color,
                           int32   x1,
                           int32   y1,
                           int32   x2,
                           int32   y2
                           );

/*****************************************************************************/
//  Description : _ref_fillRect
//  Global resource dependence :
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void _ref_fillRect (
				KJavaImage	*dst,
				KJavaClip	*clip,
				KJavaPixel	color,
				int32 	 x,
				int32 	 y,
				int32 	 w,
				int32 	 h
				);


/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : JkSoftkeySetLabel
//  Global resource dependence :
//  Author: 
//  Note:
/*****************************************************************************/
void JkSoftkeySetLabel( const JKWChar *label, const JKSint32 length,
                       JKSint32 location )
{
    int32 i = 0;
    MMISET_LANGUAGE_TYPE_E java_language_type = 0;
    //SCI_TRACE_LOW:"[JkSoftkeySetLabel] (label, length, location) = (0x%08X,%d,%d)"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,JKSOFTKEY_115_112_2_18_2_22_39_327,(uint8*)"ddd", label,length, location);
        
    if (label == JK_NULL  ||  length <= 0  ||
        location <= 0  ||  SOFTKEY_HARD_MAX <= location)
    {
        return;
    }
    else
    {    
        MMIAPISET_GetLanguageType(&java_language_type);
        if(java_language_type == MMISET_LANGUAGE_SIMP_CHINESE)
        {
            if (length == 4 && label[0] == 'M' && label[1] == 'e' && label[2] == 'n' && label[3] == 'u') 
            {          
                button_labelLens[location] = 2;
                button_labels[location][0] = 0x83DC;
                button_labels[location][1] = 0x5355;
                button_labels[location][2] = 0x00;
            } 
            else if (length == 4 && label[0] == 'E' && label[1] == 'x' && label[2] == 'i' && label[3] == 't') 
            {         
                button_labelLens[location] = 2;
                button_labels[location][0] = 0x9000;
                button_labels[location][1] = 0x51FA;
                button_labels[location][2] = 0x00;
            } 
            else if (length == 4 && label[0] == 'B' && label[1] == 'a' && label[2] == 'c' && label[3] == 'k') 
            {
                button_labelLens[location] = 2;
                button_labels[location][0] = 0x540E;
                button_labels[location][1] = 0x9000;
                button_labels[location][2] = 0x00;
            }
            else if (length == 4 && label[0] == 'b' && label[1] == 'a' && label[2] == 'c' && label[3] == 'k') 
            {
                button_labelLens[location] = 2;
                button_labels[location][0] = 0x540E;
                button_labels[location][1] = 0x9000;
                button_labels[location][2] = 0x00;
            }
            else if (length == 4 && label[0] == 's' && label[1] == 'a' && label[2] == 'v' && label[3] == 'e') 
            {
                button_labelLens[location] = 2;
                button_labels[location][0] = 0x4FDD;
                button_labels[location][1] = 0x5B58;
                button_labels[location][2] = 0x00;
            }
            else if (length == 6 && label[0] == 'C' && label[1] == 'a' && label[2] == 'n' && label[3] == 'c' && label[4] == 'e' && label[5] == 'l') 
            {
                button_labelLens[location] = 2;
                button_labels[location][0] = 0x53D6;
                button_labels[location][1] = 0x6D88;
                button_labels[location][2] = 0x00;
            }
            else if (length == 4 && label[0] == 'D' && label[1] == 'o' && label[2] == 'n' && label[3] == 'e') 
            {
                button_labelLens[location] = 2;
                button_labels[location][0] = 0x5B8C;
                button_labels[location][1] = 0x6210;
                button_labels[location][2] = 0x00;
            }
            else
            {
                button_labelLens[location] = length;     
                for(i=0;i<length;i++)
                {
                    button_labels[location][i] = label[i];
                }
            }
        }
        else
        {
            button_labelLens[location] = length;        
            for(i=0; i<length; i++)
            {
                button_labels[location][i] = label[i];
            }
        }   
       
    }
    flagChangeLabel = 1;
        
}

/*****************************************************************************/
//  Description : JkSoftkeyResetLabels
//  Global resource dependence :
//  Author: 
//  Note:
/*****************************************************************************/
void JkSoftkeyResetLabels( void )
{
    int32 i = 0;
    
    //SCI_TRACE_LOW:"[JkSoftkeyResetLabels] enter."
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,JKSOFTKEY_207_112_2_18_2_22_39_328,(uint8*)"");
        
    for(i=0;i<SOFTKEY_HARD_MAX;i++)
    {
        button_labelLens[i] = 0;
    }
    
    flagChangeLabel = TRUE;
}

/*****************************************************************************/
//  Description : JkSoftkeySetScrollState
//  Global resource dependence :
//  Author: 
//  Note:
/*****************************************************************************/
void JkSoftkeySetScrollState( JKSint32 state )
{
    //SCI_TRACE_LOW:"[JkSoftkeySetScrollState] (state) = (%d) "
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,JKSOFTKEY_223_112_2_18_2_22_39_329,(uint8*)"d",state);
    scrollState = state;
}

/*****************************************************************************/
//  Description : JkSoftkeySetScrollPosition
//  Global resource dependence :
//  Author: 
//  Note:
/*****************************************************************************/
void JkSoftkeySetScrollPosition( const JKT_Rect *viewport, JKSint32 contentWidth, JKSint32 contentHeight )
{
    
}

/*****************************************************************************/
//  Description : JkSoftkeySetFocusState
//  Global resource dependence :
//  Author: 
//  Note:
/*****************************************************************************/
void JkSoftkeySetFocusState( JKSint32 state )
{
    //SCI_TRACE_LOW:"[JkSoftkeySetFocusState] (focusstate) = (%d) "
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,JKSOFTKEY_242_112_2_18_2_22_39_330,(uint8*)"d",focusState);
    focusState = state;
}

/*****************************************************************************/
//  Description : JkSoftkeyGetFocusState
//  Global resource dependence :
//  Author: 
//  Note:
/*****************************************************************************/
JKSint32 JkSoftkeyGetFocusState( void )
{
    //SCI_TRACE_LOW:"[JkSoftkeyGetFocusState] focusState = (%d)"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,JKSOFTKEY_252_112_2_18_2_22_39_331,(uint8*)"d",focusState);
        
    return focusState;
}


/*****************************************************************************/
//  Description : JkSoftkeyPaint
//  Global resource dependence :
//  Author: 
//  Note:
/*****************************************************************************/
void JkSoftkeyPaint( void )
{
    KJavaImage      *pVirtual = PNULL;
    KJavaClip       *softClip = PNULL;
    KJavaClip       lBnClip = {0};
    KJavaClip       rBnClip = {0};
    int32           mX = 0,mY = 0,mW = 0,mH = 0;
    int32           lBnW = 0,rBnW = 0,dl = 0;
    KJavaGfxInfo    kjavaGfxInfo = {0};
    int32           startX = 0,startY = 0;
    int32           state = 0;
#if 1  
    //SCI_TRACE_LOW:"[JkSoftkeyPaint] enter."
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,JKSOFTKEY_274_112_2_18_2_22_39_332,(uint8*)"");
        
    kjava_gfx_getInfo(&kjavaGfxInfo);   
    pVirtual = kjavaGfxInfo.offscreen;
    
    if(kjavaGfxInfo.numSoftkeys == 0)
    {
        return;
    }
    
    softClip = &kjavaGfxInfo.softKeyRegion;
    
    kjava_gfx_paintVirtualBegin();
    
    /* 屏蔽下面的if - else - 关键字, 因为如果flagChangeLabel=FALSE,那么softkey不会再重画，从而前一次
    * 的Scroll Indicator没有被擦除,导致再次画的Scroll Indicator不能真实的反映实际情况 */
   if(flagChangeLabel) 
   {
    
        mX = softClip->x;
        mY = softClip->y;
        mW = softClip->width;
        mH = softClip->height;

        lBnClip.x       = mX + 3;
        lBnClip.y       = mY + 1;
        lBnClip.width   = mW/2 - 15;
        lBnClip.height  = mH - 1;
        rBnClip.x       = mX + mW/2 + 12;
        rBnClip.y       = mY + 1;
        rBnClip.width   = mW/2 - 14;
        rBnClip.height  = mH - 1;

        dl=(mH - 15)/2;



        /* 下面一行画软件行为黑色，由于后面又画一张图片，所以填充软件行为黑色没有必要，去掉提高效率 */
          _ref_fillRect(pVirtual,softClip,MMIJAVA_WHITE_COLOR,0,0,pVirtual->width,pVirtual->height);    
          _ref_drawLine(pVirtual,softClip,MMIJAVA_BLACK_COLOR,mX,mY,mX+mW,mY);


        /* Draw Left SoftKey */
        if(button_labelLens[1] != 0) 
        {            
    
            lBnW = JkFontGetStringWidth(cacheDefaultFont, button_labels[1], button_labelLens[1]); 

            if(lBnW < (mW/2 - 15))
            {            
                startX = mX + 3;
                startY = mY + dl;            
            }
            else
            {
                startX = mX + 1;
                startY = mY + dl;                        
            }


            kjava_gfx_drawChars((KJavaImage*)pVirtual,(KJavaClip*)&lBnClip,cacheDefaultFont,
                button_labels[1],button_labelLens[1],
                startX, startY, MMIJAVA_BLACK_COLOR );  //blackColor);

    
        }

        /* Draw Right SoftKey */
        if(button_labelLens[2] != 0) 
        {
            startX = 0;
            startY = 0;
    
            rBnW = JkFontGetStringWidth(cacheDefaultFont, button_labels[2], button_labelLens[2]);

            if(rBnW < (mW/2 - 14))
            {            
                startX = mX + mW - 2 - rBnW;
                startY = mY + dl;            
            }
            else
            {
                startX = mX + mW/2 + 12;
                startY = mY + dl;                        
            }

            kjava_gfx_drawChars((KJavaImage*)pVirtual,(KJavaClip*)&rBnClip,cacheDefaultFont,
                button_labels[2],button_labelLens[2],
                startX, startY, MMIJAVA_BLACK_COLOR );  //blackColor);

    
        }
   }
   else 
   {
        /* only modify scrollArrow*/

        _ref_fillRect(pVirtual,softClip,MMIJAVA_WHITE_COLOR,(softClip->width/2)-10,softClip->y+1,20,softClip->height-2);
        
    }
    
    {
        state = scrollState|focusState;        
        if(state != 0)
        {
            drawScrollArrow(pVirtual, softClip, state);//Triangle version
        }
        
    }
    
    flagChangeLabel = FALSE;
    
    kjava_gfx_paintVirtualEnd(1, (KJavaClip*)softClip);//解决softkey刷屏问题
#endif        
}

/*****************************************************************************/
//  Description : JkVideoDrawPermissionDialog
//  Global resource dependence :
//  Author: 
//  Note:
/*****************************************************************************/
JKSint32 JkVideoDrawPermissionDialog(JKGraphicsContext g, JKSint32 index, JKUint32 keyCode,  
                                     const JKWChar * titleStr, JKSint32 titleSize,
                                     const JKWChar * messageStr, JKSint32 messageSize)
{
    //SCI_TRACE_LOW:"[JkVideoDrawPermissionDialog] (g, index, keyCode, titleStr, titleSize, messageStr, messageSize) (0x%08X, %d, %d, 0x%08X, %d, 0x%08X, %d)"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,JKSOFTKEY_398_112_2_18_2_22_40_333,(uint8*)"ddddddd",g, index, keyCode, titleStr, titleSize, messageStr, messageSize);
        
    return (JKSint32)0;
}


/*****************************************************************************/
//  Description : Draw Triangle Style ScrollArrow
//  Global resource dependence :
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void drawScrollArrow(KJavaImage *pVirtual, KJavaClip *softClip, JKSint32 state)
{
    int32 mX = 0,mY = 0,mW = 0,mH = 0;
    int32 dl = 0;
    
    //SCI_TRACE_LOW:"[JkSoftkeyPaint-drawScrollArrow] enter."
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,JKSOFTKEY_413_112_2_18_2_22_40_334,(uint8*)"");
        
    mX = softClip->x;
    mY = softClip->y;
    mW = softClip->width;
    mH = softClip->height;
    dl = (mH-15)/2;
    
    //up
    if(state&1 )
    {
        _ref_drawLine(pVirtual,softClip,MMIJAVA_BLACK_COLOR,mX+mW/2,mY+dl+2,mX+mW/2+3,mY+dl+5);       
        _ref_drawLine(pVirtual,softClip,MMIJAVA_BLACK_COLOR,mX+mW/2,mY+dl+2,mX+mW/2-3,mY+dl+5);       
        _ref_drawLine(pVirtual,softClip,MMIJAVA_BLACK_COLOR,mX+mW/2-3,mY+dl+5,mX+mW/2+3,mY+dl+5);
    }
    //down
    if(state&2)
    {
        _ref_drawLine(pVirtual,softClip,MMIJAVA_BLACK_COLOR,mX+mW/2,mY+dl+13,mX+mW/2+3,mY+dl+10);     
        _ref_drawLine(pVirtual,softClip,MMIJAVA_BLACK_COLOR,mX+mW/2,mY+dl+13,mX+mW/2-3,mY+dl+10);     
        _ref_drawLine(pVirtual,softClip,MMIJAVA_BLACK_COLOR,mX+mW/2-3,mY+dl+10,mX+mW/2+3,mY+dl+10);   
    }
    //right
    if(state&4)
    {
        _ref_drawLine(pVirtual,softClip,MMIJAVA_BLACK_COLOR,mX+mW/2+9,mY+dl+7,mX+mW/2+6,mY+dl+4);     
        _ref_drawLine(pVirtual,softClip,MMIJAVA_BLACK_COLOR,mX+mW/2+9,mY+dl+7,mX+mW/2+6,mY+dl+10);        
        _ref_drawLine(pVirtual,softClip,MMIJAVA_BLACK_COLOR,mX+mW/2+6,mY+dl+4,mX+mW/2+6,mY+dl+10);    
    }
    //left
    if(state&8)
    {
        _ref_drawLine(pVirtual,softClip,MMIJAVA_BLACK_COLOR,mX+mW/2-9,mY+dl+7,mX+mW/2-6,mY+dl+4);     
        _ref_drawLine(pVirtual,softClip,MMIJAVA_BLACK_COLOR,mX+mW/2-9,mY+dl+7,mX+mW/2-6,mY+dl+10);        
        _ref_drawLine(pVirtual,softClip,MMIJAVA_BLACK_COLOR,mX+mW/2-6,mY+dl+4,mX+mW/2-6,mY+dl+10);    
    }
    
}


/*****************************************************************************/
//  Description : _ref_fillRect
//  Global resource dependence :
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void _ref_fillRect (
				KJavaImage	*dst,
				KJavaClip	*clip,
				KJavaPixel	color,
				int32 	 x,
				int32	 y,
				int32 	 w,
				int32 	 h)
{
	int32 	bound_x1 = 0, bound_x2 = 0, bound_y1 = 0, bound_y2 = 0;
	int32 	draw_x1 = 0, draw_x2 = 0, draw_y1 = 0, draw_y2 = 0;
	int32 	draw_w = 0, offset_w = 0, max = 0;
	KJavaPixel	*pDstData = PNULL;

	if(w <= 0 || h <= 0)
    {   
		return;
    }
	

	if(clip == 0) 
    {
		bound_x1 = bound_y1 = 0;
		bound_x2 = dst->width;
		bound_y2 = dst->height;
	} 
    else 
    {
		bound_x1 = clip->x;
		bound_x2 = clip->x + clip->width;
		bound_y1 = clip->y;
		bound_y2 = clip->y + clip->height;
		if(bound_x1 < 0)
        {        
			bound_x1 = 0;
        }
		if(bound_x2 > dst->width)
        {        
			bound_x2 = dst->width;
        }
		if(bound_y1 < 0)
        {        
			bound_y1 = 0;
        }
		if(bound_y2 > dst->height)
        {        
			bound_y2 = dst->height;
        }
	}

	draw_x1 = x;
	draw_x2 = x+w;
	draw_y1 = y;
	draw_y2 = y+h;

	if(draw_x1 < bound_x1)
    {    
		draw_x1 = bound_x1;
    }
	if(draw_x2 > bound_x2)
    {    
		draw_x2 = bound_x2;
    }

	if(draw_y1 < bound_y1)
    {    
		draw_y1 = bound_y1;
    }
	if(draw_y2 > bound_y2)
    {
        draw_y2 = bound_y2;
    }

	pDstData = (KJavaPixel*)dst->data + draw_x1 + draw_y1*dst->width;
	draw_w = draw_x2 - draw_x1;
	offset_w = dst->width - draw_w;

	while(draw_y1 < draw_y2) 
    {
		max = draw_w;
		while(max-- > 0) 
        {
			*pDstData++ = color;
		}
		pDstData += offset_w;
		draw_y1++;
	}
	
	return;
	
}


/*****************************************************************************/
//  Description : 画线
//  Global resource dependence :
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void _ref_drawLine (
                           KJavaImage   *dst,
                           KJavaClip    *clip,
                           KJavaPixel   color,
                           int32   x1,
                           int32   y1,
                           int32   x2,
                           int32   y2)
{
    
    int32 size = 0, inc = 0;
    int32 x_max = 0, y_max = 0, x_min = 0, y_min = 0;
    
    /* variable used to clip line*/
    uint8 code1=0x00,code2=0x00;
    int32 done = 0;
    float m = 0;
    
    KJavaPixel* uptr;  //unit ptr
    
    if(clip == 0) 
    {
        x_min = 0;
        y_min = 0;
        x_max = dst->width-1;
        y_max = dst->height-1;
    } 
    else 
    {
        x_min = clip->x;
        y_min = clip->y;
        x_max = clip->x + clip->width  - 1;
        y_max = clip->y + clip->height - 1;
        if (x_min < 0) 
        {
            x_min = 0;
        }
        if (y_min < 0)
        {
            y_min = 0;
        }
        if (x_max >= dst->width )
        {
            x_max = dst->width  - 1;
        }
        if (y_max >= dst->height)  
        {
            y_max = dst->height - 1;
        }
    }
    
    /* Boundary checking ,find real start point and end point
       Cohen-Sutherland clipline algorithm                                        */
    
    while (!done) 
    {
        
        m=0;
        code1 = 0x00;
        code2 = 0x00;
        
        if(x1 < x_min) 
        {
            code1 |= LEFT_EDGE;
        }
        if(x1 > x_max) 
        {
            code1 |= RIGHT_EDGE;
        }
        if(y1 < y_min) 
        {
            code1 |= TOP_EDGE;
        }
        if(y1 > y_max) 
        {
            code1 |= BOTTOM_EDGE;
        }
        
        if(x2 < x_min) 
        {
            code2 |= LEFT_EDGE;
        }
        if(x2 > x_max) 
        {
            code2 |= RIGHT_EDGE;
        }
        if(y2 < y_min) 
        {
            code2 |= TOP_EDGE;
        }
        if(y2 > y_max) 
        {
            code2 |= BOTTOM_EDGE;
        }
        
        if(ACCEPT(code1,code2))
        {
            done = 1;  /* Line is completely inside the window */
        }
        else if(REJECT(code1,code2))
        {
            return; /* Line is completely outside the window */
        }
        else 
        {
            if(INSIDE(code1)) 
            {          /* Ensure point1 is outside the window */
                size=code1;code1=code2;code2=size;
                size=x1;x1=x2;x2=size;
                size=y1;y1=y2;y2=size;
            }
            
            if (x2 != x1)
            {               
                 m = (float)(y2 - y1) / (x2 - x1);    //lint !e414        
            }
            
            if(code1 & LEFT_EDGE) 
            {
                y1 += (int32)((x_min - x1) * m);
                x1 = x_min;
            }
            else if (code1 & RIGHT_EDGE) 
            {
                y1 += (int32)((x_max - x1) * m);
                x1 = x_max;
            }
            else if (code1 & TOP_EDGE)
            {
                if((x1 != x2) && ((m > 1) || (m < -1)))
                {
                    x1 += (int32)((y_min - y1) / m);
                }
                y1 = y_min;
                
            }
            else if (code1 & BOTTOM_EDGE) 
            {
                if((x1 != x2) && ((m > 1) || (m < -1)))
                {
                    x1 += (int32)((y_max - y1) / m);
                }               
                y1 = y_max;
               
            }
        }
    }
    
    
    /* Vertical line */
    if(x1 == x2) 
    {
        
        if (y1 > y2) 
        {                  /* always y2>y1 */
            size=y1;
            y1=y2;
            y2=size;
        }
        
        uptr = (KJavaPixel*)dst->data + y1 * dst->width + x1;
        for (size = y2-y1; size >= 0; size--) 
        {
            *uptr = color;
            uptr += dst->width;
        }
    }
    /* Horizonal line */
    else if(y1 == y2) 
    {
        if (x1 > x2) 
        {                  /* always x2>x1 */
            size = x1;
            x1 = x2;
            x2 = size;
        }
        
        uptr = (KJavaPixel*)dst->data + y1 * dst->width + x1;
        for (size = x2-x1; size >= 0; size--) 
        {
            *uptr = color;
            uptr ++;
        }
    }
    /* Draw general line (Bresenham algorithm) */
    else 
    {
        int32 count=0;
        
        int32 dx = x2 - x1,dy = y2 - y1;
        
        if (dx < 0) 
        {
            dx = -dx;
        }
        if (dy < 0)
        {
            dy = -dy;
        }
        
        if (dx > dy) 
        { /* x coordination base */
            
            
            if (x1 > x2) 
            {                    /* Start point x1 must < x2 */
                size=x1;
                x1=x2;
                x2=size;
                size=y1;
                y1=y2;
                y2=size;
            }
            
            inc = dst->width; //y increment step
            if (y1 > y2) 
            {
                inc = -inc;
            }
            uptr = (KJavaPixel*)dst->data + y1 * dst->width + x1;
            for (size = dx; size >= 0; size--) 
            {
                *uptr = color;
                count += dy;
                uptr ++;
                if (count >= dx) 
                {
                    count -= dx;
                    uptr += inc;
                }
            }
        }
        else 
        { /* y coordination base */
            
            
            if (y1 > y2) 
            {                    /* Start point y1 must < y2 */
                size=x1;
                x1=x2;
                x2=size;
                size=y1;
                y1=y2;
                y2=size;
            }
            
            inc = 1; //x increment step
            if (x1 > x2) 
            {
                inc = -1;
            }
            uptr = (KJavaPixel*)dst->data + y1 * dst->width + x1;
            for (size = dy; size >= 0; size--) 
            {
                *uptr = color;
                count += dx;
                uptr += dst->width;
                if (count >= dy) 
                {
                    count -= dy;
                    uptr += inc;
                }
            }
        }
    }
    
    return;
}

#endif
