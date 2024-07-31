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
#include "std_header.h"
#ifdef JAVA_SUPPORT_IA
#include "guifont.h"
#include "mmijava_internal.h"


/**--------------------------------------------------------------------------*
 **                         STATIC DEFINITION                                *
 **--------------------------------------------------------------------------*/
 #ifdef KJAVA_USE_STATIC_ASCII_FONT

static KJavaFont kjavaFonts[KJF_SIZES_COUNT] = 
{    
    {
        0,//         id;
        KJAVA_FONT_FACE_MONOSPACE,//face;
        KJAVA_FONT_STYLE_PLAIN,//style;
        KJAVA_FONT_SIZE_SMALL, // size;
        0,//  char_leading;
        MMI_DEFAULT_SMALL_FONT_SIZE ,//char_ascent;
        0,//char_descent;
        MMI_DEFAULT_SMALL_FONT_SIZE ,//char_height;
        MMI_DEFAULT_SMALL_FONT//reserved;        
    },  //small
    {
        0,//         id;
        KJAVA_FONT_FACE_MONOSPACE,//face;
        KJAVA_FONT_STYLE_PLAIN,//style;
        KJAVA_FONT_SIZE_MEDIUM, // size;
        0,//  char_leading;
        MMI_DEFAULT_NORMAL_FONT_SIZE ,//char_ascent;
        0,//char_descent;
        MMI_DEFAULT_NORMAL_FONT_SIZE ,//char_height;
        MMI_DEFAULT_NORMAL_FONT//reserved;
    }, //medium
    {
        0,//         id;
        KJAVA_FONT_FACE_MONOSPACE,//face;
        KJAVA_FONT_STYLE_PLAIN,//style;
        KJAVA_FONT_SIZE_LARGE, // size;
        0,//  char_leading;
    	MMI_DEFAULT_BIG_FONT_SIZE,//char_ascent;
        0,//char_descent;
    	MMI_DEFAULT_BIG_FONT_SIZE,//char_height;
        MMI_DEFAULT_BIG_FONT //reserved;
    }//large
       
};

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
//***********************************************************************************************
//  Description:KJFimpl_CreateFont
//  Author:huangjc
//  Date:2007-09-03
//  Note:
//***********************************************************************************************
LOCAL void KJFimpl_CreateFont(int32 iFace, int32 iStyle, int32 iSize, KJavaFont* kjf);

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
//***********************************************************************************************
//  Description:KJF_Init
//  Author:huangjc
//  Date:2007-09-03
//  Note:
//***********************************************************************************************
void KJF_Init(int32 srcW, int32 scrH)
{
    int32 iFace = 0, iStyle = 0, iSize = 0;
    int32 i = 0;
    KJavaFont* kjf = PNULL;  
  

    kjf = kjavaFonts;
    
    for(iFace=0; iFace<KJF_FACES_COUNT; ++iFace)
    {
        for(iStyle=0; iStyle<KJF_STYLES_COUNT; ++iStyle)
        {
            for(iSize=0; iSize<KJF_SIZES_COUNT; ++iSize)
            {
                kjf->id = i;
                KJFimpl_CreateFont(iFace, iStyle, iSize, kjf);
                ++kjf;
                ++i;
            }
        }
    }
    
}



//***********************************************************************************************
//  Description:KJF_GetCharsWidth
//  Author:huangjc
//  Date:2007-09-03
//  Note:
//***********************************************************************************************
int32 KJF_GetCharsWidth(const KJavaFont* kjf, const KJavaUnicode *pszText, int32 nTextLen)
{
    int32 charsWidth = 0;
   // int32 i = 0;
    
    charsWidth = GUI_GetStringWidth((GUI_FONT_T)kjf->reserved,pszText,nTextLen);    
    return charsWidth;
    
}

//***********************************************************************************************
//  Description:KJF_GetCountChars
//  Author:huangjc
//  Date:2007-09-03
//  Note:
//***********************************************************************************************
int32 KJF_GetCountChars(const KJavaFont *kjf, 
                      const KJavaUnicode *str, 
                      int32 length, 
                      int32 width)
{
    int32 charsWidth = 0;
    int32 charsCount = 0;
    int32 i = 0;
    
    //SCI_TRACE_LOW("KJF_GetCountChars length  = %d, width = %d", length, width);
    for(i = 0; i < length; i++) 
    {
        
       // if(str[i] < 128) 
      //  {
            charsWidth += GUI_GetFontWidth((GUI_FONT_T)kjf->reserved,(uint16)str[i]);
      //  }
     //   else 
     //   {
	//		charsWidth += GUI_GetFontWidth((GUI_FONT_T)kjf->reserved, 1);
     //   }
        
        if(charsWidth > width)
        {
            JWE_LOG(KJF_GetCountChars, ("[KJF_GetCountChars]: charsCount =%d", charsCount))
                return charsCount;
        }
        else 
        {
            charsCount++;
        }
    }
    JWE_LOG(KJF_GetCountChars, ("[KJF_GetCountChars]: charsCount =%d", charsCount))
        
    return (int)charsCount;
}


//***********************************************************************************************
//  Description:KJF_DrawChars
//  Author:huangjc
//  Date:2007-09-03
//  Note:
//***********************************************************************************************
void KJF_DrawChars (KJavaImage      *offscreen, 
                    const KJavaClip *clip, 
                    const KJavaFont *kjfFont, 
                    const KJavaUnicode *pszText, 
                    int32 nTextLen, 
                    int32 text_x, 
                    int32 text_y, 
                    const KJavaPixel color)
{
    const KJavaUnicode *pText = pszText;
    int32 i = 0;
    int32 widthText = 0, heightText= 0;
    GUI_RECT_T  Rclip = {0}; 
    GUI_RECT_T    Rtext = {0}; 
    GUI_RECT_T   Rdst = {0};
    GUI_RECT_T   rcDst = {0};
    GUI_RECT_T   rcClip2 = {0};


    GUISTR_STYLE_T      text_style = {0};
    // MMI_STRING_T        str_data = {0};
    GUISTR_STATE_T      state = GUISTR_STATE_EFFECT|GUISTR_STATE_WORDBREAK|GUISTR_STATE_SINGLE_LINE;
    GUISTR_BUF_T        des_buf = {0};
    MMI_STRING_T	str_ptr = {0};
	
    if(clip == NULL)
    {
        return;
    }
    Rclip.left = 0;
    Rclip.right = offscreen->width;
    Rclip.top = 0;
    Rclip.bottom = offscreen->height;
    //SCI_TRACE_LOW("KJF_DrawChars enter time = %d, line = %d",SCI_GetTickCount(),__LINE__);
    
    rcClip2.left = clip->x;
    rcClip2.right = clip->x+clip->width;
    rcClip2.top = clip->y;
    rcClip2.bottom = clip->y+clip->height;
    
    GUI_IntersectRect(&rcDst, Rclip, rcClip2);
    Rclip = rcDst;
    
    while(text_x < 0)
    {
      //  if(pszText[i] > 0x80)
     //   {
     //       pText += 1;
	//		text_x+= GUI_GetFontWidth((GUI_FONT_T)kjfFont->reserved, 1);
    //    }
    //    else
   //     {
            pText += 1;
			text_x += GUI_GetFontWidth((GUI_FONT_T)kjfFont->reserved,(uint16)pszText[i]);
   //     }
        i++;
    }
    
    widthText = KJF_GetCharsWidth(kjfFont, pszText, nTextLen);
    heightText = GUI_GetFontHeight((GUI_FONT_T)kjfFont->reserved, TRUE);
    Rtext.left = text_x;
    Rtext.right = text_x+widthText;
    Rtext.top = text_y;
    Rtext.bottom = text_y+heightText;
    
    
    GUI_IntersectRect(&Rdst, Rclip, Rtext);
    
    if( (Rdst.bottom < Rdst.top) || (Rdst.right < Rdst.left) )
    {
        return;
    }
  
/*
   GUI_ShowStringOffset //LCD_DisplayStringInRectExt(
       FONT_EFFECT_NONE,
       0,
       ANGLE_0,
       (GUI_COLOR_T*)(offscreen->data),
       offscreen->width,
       offscreen->height,
       Rdst,
       (wchar*)pszText,
       nTextLen,
       0,
       0,
       color,
       (GUI_FONT_T)kjfFont->reserved,
       0);
   //SCI_TRACE_LOW("KJF_DrawChars enter time = %d, line = %d",SCI_GetTickCount(),__LINE__);
*/
	  str_ptr.wstr_len = nTextLen;
	  str_ptr.wstr_ptr = (wchar *)pszText;
	  
	  des_buf.buffer =  (GUI_COLOR_T*)(offscreen->data);
	  des_buf.height = offscreen->width;
	  des_buf.mem_width = des_buf.width = offscreen->height;
	  
	  text_style.align = ALIGN_LEFT; 
	  text_style.font =(GUI_FONT_T) kjfFont->reserved;
	  text_style.font_color = color;
	  
         GUISTR_DrawTextInRect( 
             &des_buf,
             &Rdst,
             &Rdst,
             &str_ptr,
             &text_style,
             state,
             GUISTR_TEXT_DIR_AUTO  );


}


/**
* \brief Find the suitable system font with the given criteria 
*        (size, face, and style).
*
* The function finds the suitable system font and return its font handle. Note
* that the function should use the last parameter for default font with the 
* this given criteria (size == 0, style == 0, and face == 0).
* 
* \param iFace      the font face. 
*                  
*                  One of the following is designated.
*                  <ul>
*                      <li>KJAVA_FONT_FACE_MONOSPACE</li>
*                      <li>KJAVA_FONT_FACE_PROPORTIONALE</li>
*                  </ul>
*
* \param iStyle     the font style.
*
*                  One of the following is designated.
*                  <ul>
*                      <li>KJAVA_FONT_STYLE_PLAIN</li>
*                      <li>KJAVA_FONT_STYLE_BOLD</li>
*                      <li>KJAVA_FONT_STYLE_ITALIC</li>
*                      <li>KJAVA_FONT_STYLE_UNDERLINE</li>
*                  </ul>
*
* \param iSize      the size for the given criteria.
*
*                  One of the following is designated.
*                  <ul>
*                      <li>KJAVA_FONT_SIZE_SMALL</li>
*                      <li>KJAVA_FONT_SIZE_MEDIUM</li>
*                      <li>KJAVA_FONT_SIZE_LARGE</li>
*                  </ul>
*
* \param defaultFont   the default font.
*                  
*                  One of the following is designated.
*                  <ul>
*                      <li>KJAVA_FONT_TYPE_DEFAULT</li>
*                      <li>KJAVA_FONT_TYPE_STATIC_TEXT</li>
*                      <li>KJAVA_FONT_TYPE_INPUT_TEXT</li>
*                  </ul>
*
* \return Return a handle identifying the designated font. If it's failed to
*         find suitable font, the function returns default system font at
*         least.
*/
KJavaFont* KJF_GetSuitableFont(int32 iFace, int32 iStyle, int32 iSize, int32 iDefault)
{
   //  int32 i = 0;
  //  KJavaFont* kjf = PNULL;
    
    JWE_LOG(KJF_GetSuitableFont, ("[KJFimpl_GetSuitableFont] iFace=%d, iStyle=%d, iSize=%d, iDefault=%d\n", iFace, iStyle, iSize, iDefault));
    switch(iSize) 
    {
    case KJAVA_FONT_SIZE_SMALL:
        return  &kjavaFonts[0];  	
    case KJAVA_FONT_SIZE_MEDIUM:
        return  &kjavaFonts[1];  
    case KJAVA_FONT_SIZE_LARGE:
        return  &kjavaFonts[2];  
    default:
	return &kjavaFonts[0];
    }  
  
}

//***********************************************************************************************
//  Description:KJFimpl_CreateFont
//  Author:huangjc
//  Date:2007-09-03
//  Note:
//***********************************************************************************************
LOCAL void KJFimpl_CreateFont(int32 iFace, int32 iStyle, int32 iSize, KJavaFont* kjf)
{
    kjf->face          = 0;
    kjf->style         = 0;
    kjf->size          = 0;
    JWE_LOG(KJFimpl_CreateFont,("kjava font size is %d",iSize))
    switch(iSize)
    {       
    case KJAVA_FONT_SIZE_SMALL:            
        kjf->reserved      = MMI_DEFAULT_SMALL_FONT/*SONG_FONT_18*/;
        break;
    case KJAVA_FONT_SIZE_MEDIUM:           
        kjf->reserved      = MMI_DEFAULT_NORMAL_FONT/*UNICODE_FONT_24*/;
        break;        
    case KJAVA_FONT_SIZE_LARGE:           
        kjf->reserved      = MMI_DEFAULT_BIG_FONT/*SONG_FONT_14*/;
        break;
    default:
        kjf->reserved      = MMI_DEFAULT_NORMAL_FONT;
        break; 
    }

   kjf->char_height   =  GUIFONT_GetHeight(kjf->reserved);
   kjf->char_descent = 0;//(int)((kjf->char_height * 0.1)+1);
   kjf->char_ascent = kjf->char_height - kjf->char_descent;
   kjf->char_leading = 0;

  // SCI_TRACE_LOW("KJFimpl_CreateFont kjf->char_height = %d, kjf->char_descent = %d,kjf->size = %d",kjf->char_height,kjf->char_descent,kjf->size);
}
#endif

#endif
