#include "sci_types.h"
#include "kjava_sys_gfx.h"

#ifdef __cplusplus
extern "C"{
#endif

//#define KJF_TAHOMA                  0
//#define KJF_ROMAN                   1
#define KJF_FACES_COUNT             3

#define KJF_PLAIN            		0 // 0x0000
#define KJF_BOLD            	 	1 // 0x0001
#define KJF_ITALIC           		2 // 0x0010
#define KJF_ITALIC_BOLD      		3 // 0x0011
#define KJF_UNDERLINE        		4 // 0x0100
#define KJF_UNDERLINE_BOLD   		5 // 0x0101
#define KJF_UNDERLINE_ITALIC   		6 // 0x0110
#define KJF_UNDERLINE_ITALIC_BOLD   7 // 0x0111
#define KJF_STYLES_COUNT            8 // 0x1000

#define KJF_SMALL                   1
#define KJF_MEDIUM                  2
#define KJF_LARGE                   3
#define KJF_SIZES_COUNT             3


#define MAX_KJAVA_GFX_FONTS          KJF_STYLES_COUNT*KJF_FACES_COUNT*KJF_SIZES_COUNT


//***********************************************************************************************
//  Description:KJF_Init
//  Author:huangjc
//  Date:2007-09-03
//  Note:
//***********************************************************************************************
void KJF_Init(int32 srcW, int32 scrH);
void KJF_Fini(void);

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
KJavaFont* KJF_GetSuitableFont(int32 iFace, int32 iStyle, int32 iSize, int32 iDefault);

//***********************************************************************************************
//  Description:KJF_GetCharsWidth
//  Author:huangjc
//  Date:2007-09-03
//  Note:
//***********************************************************************************************
int32 KJF_GetCharsWidth(const KJavaFont* kjf, const KJavaUnicode *pszText, int32 nTextLen);

//***********************************************************************************************
//  Description:KJF_GetCountChars
//  Author:huangjc
//  Date:2007-09-03
//  Note:
//***********************************************************************************************
int32 KJF_GetCountChars(const KJavaFont *kjf, 
                      const KJavaUnicode *str, 
                      int32 length, 
                      int32 width);

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
                    const KJavaPixel color);

void jblendia_gfx_timer(void * param);
#ifdef __cplusplus
}
#endif
