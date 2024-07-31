/*
 * Copyright 2003 Aplix Corporation. All Rights Reserved.
 *
 * JBlend and all Aplix- or JBlend-related trademarks and logos are
 * trademarks or registered trademarks of Aplix in the U.S., Japan and
 * other countries.
 */

#ifndef JAS_GFX_FONT_H
#define JAS_GFX_FONT_H




typedef enum
{
    JAS_GFX_FONT_SIZE_UNKNOWN = 0,
    JAS_GFX_FONT_SIZE_SMALL   = 1,
    JAS_GFX_FONT_SIZE_MEDIUM  = 2,
    JAS_GFX_FONT_SIZE_LARGE   = 3,
    JAS_GFX_FONT_SIZE_NUM
} JAS_GFX_FONT_SIZE_E;


typedef enum
{
    JAS_GFX_FONT_STYLE_UNKNOWN   = -1,
    JAS_GFX_FONT_STYLE_PLAIN     = 0,
    JAS_GFX_FONT_STYLE_BOLD      = 1,
    JAS_GFX_FONT_STYLE_ITALIC    = 2,
    JAS_GFX_FONT_STYLE_UNDERLINE = 4,


    JAS_GFX_FONT_STYLE_NUM       = 9
} JAS_GFX_FONT_STYLE_E;


typedef enum
{
    JAS_GFX_FONT_FACE_UNKNOWN      = -1,
    JAS_GFX_FONT_FACE_DEFAULT    = 0,		
    JAS_GFX_FONT_FACE_MONOSPACE    = 1,
    JAS_GFX_FONT_FACE_PROPORTIONAL = 2,
    JAS_GFX_FONT_FACE_NUM
} JAS_GFX_FONT_FACE_E;




typedef struct
{
	int log_font_id;   
	JAS_GFX_FONT_FACE_E face;
	JAS_GFX_FONT_STYLE_E style;
	JAS_GFX_FONT_SIZE_E size;
	int leading; 
	int ascent;  
	int descent; 
	int height;  
} JAS_GFX_FONT_S;



typedef enum
{
    JAS_GFX_FONT_TYPE_UNKNOWN     = 0,
    JAS_GFX_FONT_TYPE_DEFAULT     = 1,
    JAS_GFX_FONT_TYPE_STATIC_TEXT = 2,
    JAS_GFX_FONT_TYPE_INPUT_TEXT  = 3,
    JAS_GFX_FONT_TYPE_NUM
} JAS_GFX_FONT_TYPE_E;

typedef struct
{
    int leading;
    int ascent;
    int descent;
    int height;
} JAS_GFX_FONT_DETAIL_S;


/*****************************************************************************/
//  Description : jas_GfxFontInit
//  Global resource dependence : 
//  Author:siyuan
//  Note: 
/*****************************************************************************/
PUBLIC void jas_GfxFontInit();
/*****************************************************************************/
//  Description : to create font and init it with default value
//  Global resource dependence : 
//  Author:siyuan
//  Note: 
/*****************************************************************************/
PUBLIC void jas_GfxGetDefaultFont(JAS_GFX_FONT_TYPE_E type, JAS_GFX_FONT_S *font);
/*****************************************************************************/
//  Description : to create font and init it
//  Global resource dependence : 
//  Author:siyuan
//  Note: 
/*****************************************************************************/
PUBLIC void jas_GfxGetFont(JAS_GFX_FONT_S *font);
/**
* \brief Draw a character string into a given offscreen buffer with 
*        designated clipping rectangle, font, and pixel color.
*        
* \param screen the designated offscreen buffer.
* \param log_font_id      the given font handle.
* \param string    the given UCS2 string.
* \param length    The number of characters in given UCS2 string.
* \param x         the x coordinate of the string in offscreen buffer.
* \param y         the y coordinate of the string in offscreen buffer.
* \param clip      the clipping range for drawing a string.
* \param color     the color of the string. it should be the platform pixel
*                  format. (Maybe RGB: 565, 666, or 888)
*/
PUBLIC void jas_GfxDrawString(JAS_GFX_SCREEN_BUFF_S *screen,uint log_font_id,const wchar *wstr,int length,int x,int y,const JAS_GFX_CLIP_S *clip,JAS_GFX_PIXEL color);
/*****************************************************************************/
//  Description : get string pixel width by given font
//  Global resource dependence : 
//  Author:siyuan
//  Note: 
/*****************************************************************************/
PUBLIC void jas_GfxGetStringWidth(uint log_font_id,const wchar *wstr,int length,uint *width);
/*****************************************************************************/
//  Description : get chars count by given length and font 
//  Global resource dependence : 
//  Author:siyuan
//  Note: 
/*****************************************************************************/
PUBLIC void jas_GfxGetCharCount(uint log_font_id, const wchar *wstr,int length,int width,uint16 *count);



#endif /* JAS_GFX_FONT_H */
