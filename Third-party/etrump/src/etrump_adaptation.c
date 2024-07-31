#ifdef VECTOR_FONT_SUPPORT_ETRUMP
#define FT_CONFIG_OPTION_NO_ASSEMBLER

#include "guicommon.h"
#include "spml_api.h"
//#include "freetype/Freetype.h"

#include "etrump/etrump.h"
#include /*lint -e766*/"et_common/et_common.h"
#include "et_font.h"


#define ETRUMP_FONT_LOCK_NAME "et_font_lock"
#define ET_MAX_STR_LENGTH     96

unsigned char mem_pool[POOL_SIZE];

typedef struct
{
	SCI_MUTEX_PTR     lock;
    FT_Library        library;
    uint32            fontNumber;
	uint32            scale_ratio[32];
	// 宽度根据最合适的字库计算，以解决多字库中字符混排的问题
    FT_Font**         allFonts;
	// 高度使用用户当前选中的字库
    // current font is set by user
    // which is the first priority to match fonts
    FT_Font*          curFont;
	// 当前语言
	MMISET_LANGUAGE_TYPE_E lang_type;
    SPML_TEXT_SIZE_T  fontSize;
}ET_FONT_INFO;


static ET_FONT_INFO g_et_font_info;

static int etrump_wstrlen(const wchar *str)
{
	int len;
	for (len = 0; *str++; )
	{
		len++;
	}
	return len;
}


/************************************************************************/
/* static functions                                                     */
/************************************************************************/
static void etrump_set_font_param_nolock(const SPML_TEXT_PARAM_T* param_ptr)
{
	SPML_COLOR_T tc = param_ptr->font_color;
	SPML_COLOR_T bc = param_ptr->bg_color;
	SPML_COLOR_T ec = param_ptr->edge_color;
	uint8 alpha = param_ptr->edge_alpha;
	SPML_COLOR_T uc = param_ptr->line_color;
	SPML_TEXT_SIZE_T size = param_ptr->size;
	SPML_TEXT_FORMAT_T format = param_ptr->format;
	uint32 i;

	if( g_et_font_info.fontSize != size ) {
		g_et_font_info.fontSize = size;
		for (i = 0; i < g_et_font_info.fontNumber; i++)
		{
			size = g_et_font_info.fontSize * g_et_font_info.scale_ratio[i] / 64;
			FT_Font_Set_Pixel_Size(g_et_font_info.allFonts[i], (int)size, (int)size);
		}
	}

	for (i = 0; i < g_et_font_info.fontNumber; i++)
	{
		int style = Regular;
#ifdef NOT_PERMIT_GLYPH_SPACE
		FT_Font_Set_WidthGap(g_et_font_info.allFonts[i], 0);
#else
		FT_Font_Set_WidthGap(g_et_font_info.allFonts[i], param_ptr->glyph_space);
#endif

		FT_Font_Set_Text_Color( g_et_font_info.allFonts[i], 
				FT_RGB( ((tc >> 11) << 3) & 0xff, ((tc >> 5) << 2) & 0xff, (tc << 3) & 0xff ) );
		
		FT_Font_Set_Back_Color( g_et_font_info.allFonts[i],
			FT_RGB( ((bc >> 11) << 3) & 0xff, ((bc >> 5) << 2) & 0xff, (bc << 3) & 0xff ) );
		
		FT_Font_Set_Edge_Color( g_et_font_info.allFonts[i],
			FT_RGB( ((ec >> 11) << 3) & 0xff, ((ec >> 5) << 2) & 0xff, (ec << 3) & 0xff ) );
		
		FT_Font_Set_Underline_Color( g_et_font_info.allFonts[i],
			FT_RGB( ((uc >> 11) << 3) & 0xff, ((uc >> 5) << 2) & 0xff, (uc << 3) & 0xff ) );
				
		if( format & TEXT_FORMAT_BOLD )
			style |= Bold;
		if( format & TEXT_FORMAT_ITALIC )
			style |= Italic;
		if( format & TEXT_FORMAT_UNDERLINE )
			style |= Underline;
		if( format & TEXT_FORMAT_EDGE ) {
			style |= Crochet;
			FT_Font_Set_Edge_Alpha(g_et_font_info.allFonts[i], alpha);
		}
		FT_Font_Set_Font_Style(g_et_font_info.allFonts[i], style);

		if( format & TEXT_FORMAT_BACKGROUND ) {
			//背景不透明，绘制背景色
			FT_Font_Set_Back_Mode(g_et_font_info.allFonts[i], 0);
		}
		else {
			//背景透明，背景色不起作用
			FT_Font_Set_Back_Mode(g_et_font_info.allFonts[i], 1);
		}
		
		if( format & TEXT_FORMAT_INVERSE ) {
			tc = ~tc;
			bc = ~bc;
			
			FT_Font_Set_Back_Mode(g_et_font_info.allFonts[i], 0);
			FT_Font_Set_Text_Color(g_et_font_info.allFonts[i], 
				FT_RGB( ((tc >> 11) << 3) & 0xff, ((tc >> 5) << 2) & 0xff, (tc << 3) & 0xff ) );
			FT_Font_Set_Back_Color(g_et_font_info.allFonts[i],
				FT_RGB( ((bc >> 11) << 3) & 0xff, ((bc >> 5) << 2) & 0xff, (bc << 3) & 0xff ) );
		}
	}
}

static void etrump_get_text_metrics_nolock(const SPML_TEXT_PARAM_T* param_ptr, 
								           const wchar*             text_ptr,  
								           uint16                   len,   
                                           SPML_TEXT_METRICS_T*     metrics_ptr)
{
	int lineGap = 0;

	lineGap = FT_Font_Get_Text_LineGap( g_et_font_info.curFont);
	lineGap = lineGap > 0 ? lineGap : 0;

	metrics_ptr->width = FT_Font_Get_Text_Width( g_et_font_info.curFont, (FT_WCHAR*)text_ptr, len, 0 );
	metrics_ptr->height = FT_Font_Get_Text_Height( g_et_font_info.curFont ) - lineGap;
}
#if defined (MULTI_LAN_MYANMAR_ZAWGYI_SUPPORT)
static void etrump_myanmar_rule_convert(wchar *textPtr, uint16 len)
{
	uint16 i = 0;
	for(i = 0; i < len; i++)
	{
		if((textPtr[i] >= 0x1000) && (textPtr[i] <= 0x109F))
		{
			/*Zawgyi显示时，编码段使用0xef00-0xef9f,unicode规范中缅甸语编码段为0x1000-0x109f,存在0xDF00的对应的关系*/
			textPtr[i] = textPtr[i] + 0xDF00;
			SPMLAPI_TestChar(textPtr[i]);
		}
	}
}
#endif

/************************************************************************/
/* public functions                                                     */
/************************************************************************/
extern uint16 MMIRES_GetFontLibNum( void );			/*lint -esym(526,MMIRES_GetFontLibNum) */
extern BOOLEAN MMIRES_GetFontLibPtr(						/*lint -esym(526,MMIRES_GetFontLibPtr) */
									uint16 index,
									uint8** lib_ptr, 
									uint32* lib_size
                                   );
extern BOOLEAN MMIRES_IsNeedBidi( void );    /*lint -esym(526,MMIRES_IsNeedBidi) */

/*****************************************************************************/
// 	Description : init spml
//	Note:初始化显示引擎
/*****************************************************************************/
PUBLIC BOOLEAN SPMLAPI_Init( const SPML_INIT_DATA_T* init_ptr )
{
	uint32 i;
	FT_Face face = NULL;
	ET_Platform platform;					/*lint !e522*/

	// create mutex
	g_et_font_info.lock = SCI_CreateMutex(ETRUMP_FONT_LOCK_NAME, SCI_INHERIT);

	g_et_font_info.fontNumber = MMIRES_GetFontLibNum();
	if( g_et_font_info.fontNumber == 0 )
		return FALSE;

	g_et_font_info.allFonts = SCI_ALLOC( g_et_font_info.fontNumber * sizeof(FT_Font*) );

	memset( (void*)&platform, 0, sizeof(ET_Platform) );
	ET_Mempool_Initialize( mem_pool, POOL_SIZE, &platform.alloc, &platform.free );		/*lint !e718 !e746 */	/*lint -esym(526,ET_Mempool_Initialize) */	/*lint -esym(628,ET_Mempool_Initialize) */
	ET_Platform_Initialize( &platform );												/*lint !e718 !e746 */	/*lint -esym(526,ET_Platform_Initialize) */	/*lint -esym(628,ET_Platform_Initialize) */
	FT_Init_FreeType( &g_et_font_info.library );
	FT_Font_Support_Multiple_Faces(1);

	for( i = 0; i < g_et_font_info.fontNumber; i++ ) {
		MMIRES_GetFontLibPtr(i, &et_font_list[i]->fontdata, &et_font_list[i]->datalen);
		FT_Font_Create_From_Memory( &g_et_font_info.allFonts[i],
			g_et_font_info.library,
			et_font_list[i]->fontdata,
			et_font_list[i]->datalen );
		FT_Font_Set_Pixel_Size(g_et_font_info.allFonts[i], 24, 24);

		face = FT_Font_Get_Face(g_et_font_info.allFonts[i]);
		g_et_font_info.scale_ratio[i] = (face->units_per_EM << 6) / (face->ascender - face->descender);
	}

	g_et_font_info.curFont = g_et_font_info.allFonts[0];

	return TRUE;
}


/*****************************************************************************/
// 	Description : load font
//	Note:加载T卡上的字体
/*****************************************************************************/
PUBLIC BOOLEAN SPMLAPI_LoadFont( uint32 lib_size )
{
	return FALSE;
}


/*****************************************************************************/
// 	Description : unload font
//	Note:卸载T卡上的字体
/*****************************************************************************/
PUBLIC BOOLEAN SPMLAPI_UnloadFont()
{
	return FALSE;
}


/************************************************************************/
//  Description : set current language
/************************************************************************/
PUBLIC void SPMLAPI_SetCurrentLanguage(MMISET_LANGUAGE_TYPE_E lang_type)
{
	uint32 i = 0, j = 0;

	SCI_GetMutex(g_et_font_info.lock, SCI_WAIT_FOREVER);

	g_et_font_info.lang_type = lang_type;
	for( i = 0; i < g_et_font_info.fontNumber; i++ ) {
		for( j = 0; et_font_list[i]->supportLanguages[j] < MMISET_MAX_LANGUAGE; j++ ) {
			if( lang_type == et_font_list[i]->supportLanguages[j] ) {
				g_et_font_info.curFont = g_et_font_info.allFonts[i];
				SCI_PutMutex(g_et_font_info.lock);
				return;
			}
		}
	}

	SCI_PutMutex(g_et_font_info.lock);
}


/*****************************************************************************/
// 	Description : get text direction
//	Note:获得文字的方向, TEXT_DIR_LTR or TEXT_DIR_RTL, 如果系统中没有阿拉伯语
//       或希伯来语等RTL语言, 恒返回TEXT_DIR_LTR
/*****************************************************************************/
PUBLIC SPML_TEXT_DIR_T SPMLAPI_GetTextDirection(const wchar* text_ptr, //unicode字符串
												int32 len              //字符个数
												)
{
	SPML_TEXT_DIR_T dir = TEXT_DIR_LTR;

	SCI_GetMutex(g_et_font_info.lock, SCI_WAIT_FOREVER);

	if (MMIRES_IsNeedBidi())
	{
		dir = FT_Font_Get_Paragraph_Direction(text_ptr, len);/*lint !e605*/
	}

	SCI_PutMutex(g_et_font_info.lock);
	return dir;
}


/*****************************************************************************/
// 	Description : is word break char
//	Note:是否为按词断行字符,在按词断行时起作用,一般的标点符号都是
/*****************************************************************************/
PUBLIC BOOLEAN SPMLAPI_IsWordBreakChar( wchar ch )
{
	return FALSE;
}


/*****************************************************************************/
// 	Description : get char script
//	Note:获得字符的script
/*****************************************************************************/
PUBLIC SPML_SCRIPT_E SPMLAPI_GetCharScript( wchar ch )
{
	return TEXT_SCRIPT_MAX;
}


/*****************************************************************************/
// 	Description : line break
//	Note:断行函数, 分为按像素断行和按词断行, 返回显示宽度中能显示的字符个数
/*****************************************************************************/
PUBLIC uint16 SPMLAPI_LineBreak(const SPML_TEXT_PARAM_T* param_ptr,     //显示参数
							    const wchar*             text_ptr,      //unicode字符串
							    uint16                   len,           //字符个数
							    uint16                   disp_width,    //显示宽度
							    uint16*                  out_width_ptr, //out, 能显示的字符的宽度
							    BOOLEAN                  by_word        //是否按词断行
								)
{
	int i = 0;
	int unichar_num = 0;
	int wstrLen = etrump_wstrlen(text_ptr);
// 	FT_FaceRec_* *   face  =     FT_Font_Get_Face( g_et_font_info.curFont);
#if defined (MULTI_LAN_MYANMAR_ZAWGYI_SUPPORT)
	wchar temp_text_ptr[ET_MAX_STR_LENGTH] = {0};
#endif
	len = len > wstrLen ? wstrLen : len;
	len = len > ET_MAX_STR_LENGTH ? ET_MAX_STR_LENGTH : len;
	for( i = 0; i < len; i++ ) {
		if( text_ptr[i] == 0x000A )
			break;
	}
	len = (i == len) ? i : i+1;

	SCI_GetMutex(g_et_font_info.lock, SCI_WAIT_FOREVER);

	etrump_set_font_param_nolock( param_ptr );
#if defined (MULTI_LAN_MYANMAR_ZAWGYI_SUPPORT)
	memcpy(temp_text_ptr,text_ptr,len*sizeof(wchar));
	etrump_myanmar_rule_convert(temp_text_ptr,len);
	out_width_ptr = FT_Font_Get_Text_Width_Variant( g_et_font_info.curFont,
			(FT_WCHAR*)temp_text_ptr, len, disp_width, by_word, &unichar_num );
#else
	*out_width_ptr = FT_Font_Get_Text_Width_Variant( g_et_font_info.curFont,
			(FT_WCHAR*)text_ptr, len, disp_width, by_word, &unichar_num );
#endif
	/*
	  BUG:76283.76215
	  sometimes  the out_width_ptr is not accurate, we need recalculate the string with length-1.
	  zhewei
	*/
// 	if ((!by_word)&&(unichar_num>0)&&(*out_width_ptr>disp_width))
// 	{
// 		int	len_ex = unichar_num;
// 		int re_cal_wd = 0; 
// 		do
// 		{
// 			//the calculated length should decrease 1
// 			re_cal_wd = FT_Font_Get_Text_Width( g_et_font_info.curFont, (FT_WCHAR*)text_ptr, len_ex-- , 0 );
// 		}while((re_cal_wd >disp_width)&&(len_ex>0));
// 		unichar_num = len_ex;
// 		if (re_cal_wd >*out_width_ptr)
// 		{
// 			*out_width_ptr = re_cal_wd;
// 		}
// 		// if disp_width can't contain first char, return 0 unicode_num and 0 pixel width.
// 		if( unichar_num == 1 && disp_width < *out_width_ptr ) {
// 			*out_width_ptr = 0;
// 			unichar_num = 0;
// 		}
// 
// 	}

	SCI_PutMutex(g_et_font_info.lock);
	return unichar_num;
}


/*****************************************************************************/
// 	Description : get cursor coordinate
//	Note:根据光标位置获得光标x坐标
/*****************************************************************************/
PUBLIC int32 SPMLAPI_GetCursorCoordinate(const SPML_TEXT_PARAM_T* param_ptr, //显示参数
                                         const wchar*             text_ptr,  //unicode字符串
                                         uint16                   len,       //字符个数
                                         int32                    pos        //光标位置
                                         )
{
	int text_width;
	int align_offset;
	int32 xpos = 0;
	int dir = TEXT_DIR_LTR;
#if defined (MULTI_LAN_MYANMAR_ZAWGYI_SUPPORT)
	wchar temp_text_ptr[ET_MAX_STR_LENGTH] = {0};
#endif

	SCI_GetMutex(g_et_font_info.lock, SCI_WAIT_FOREVER);

	len = len > ET_MAX_STR_LENGTH ? ET_MAX_STR_LENGTH : len;
	etrump_set_font_param_nolock( param_ptr );
#if defined (MULTI_LAN_MYANMAR_ZAWGYI_SUPPORT)
	memcpy(temp_text_ptr,text_ptr,len*sizeof(wchar));
	etrump_myanmar_rule_convert(temp_text_ptr,len);
	dir = (param_ptr->dir == TEXT_DIR_AUTO) ? FT_Font_Get_Line_Direction(temp_text_ptr, len) : param_ptr->dir;/*lint !e605*/
	FT_Font_Set_Text_Order(g_et_font_info.curFont, dir);

	text_width = FT_Font_Get_Text_Width(g_et_font_info.curFont, (FT_WCHAR*)temp_text_ptr, len, 0);
#else
	dir = (param_ptr->dir == TEXT_DIR_AUTO) ? FT_Font_Get_Line_Direction(text_ptr, len) : param_ptr->dir;/*lint !e605*/
	FT_Font_Set_Text_Order(g_et_font_info.curFont, dir);

	text_width = FT_Font_Get_Text_Width(g_et_font_info.curFont, (FT_WCHAR*)text_ptr, len, 0);
#endif
	if (param_ptr->format & TEXT_FORMAT_CENTER)
	{
		align_offset = (param_ptr->display_rect.right + 1 + param_ptr->display_rect.left - text_width) / 2;
	}
	else if (param_ptr->format & TEXT_FORMAT_RIGHT)
	{
		align_offset = param_ptr->display_rect.right - text_width;
	}
	else
	{
		align_offset = param_ptr->display_rect.left;
	}
#if defined (MULTI_LAN_MYANMAR_ZAWGYI_SUPPORT)
	xpos = align_offset + FT_Font_Get_Offset( g_et_font_info.curFont, (FT_WCHAR*)temp_text_ptr, len, pos );
#else
	xpos = align_offset + FT_Font_Get_Offset( g_et_font_info.curFont, (FT_WCHAR*)text_ptr, len, pos );
#endif
	SCI_PutMutex(g_et_font_info.lock);
	return xpos;
}


/*****************************************************************************/
// 	Description : get cursor pos
//	Note:根据光标x坐标获得光标位置并微调光标位置
/*****************************************************************************/
PUBLIC int32 SPMLAPI_GetCursorInfo(const SPML_TEXT_PARAM_T* param_ptr, //显示参数
                                   const wchar*             text_ptr,  //unicode字符串
                                   uint16                   len,       //字符个数
                                   BOOLEAN                  is_left,   //left or right
                                   int32*                   pos_ptr    //光标原位置, in\out
                                   )
{
	int dir = TEXT_DIR_AUTO;
#if defined (MULTI_LAN_MYANMAR_ZAWGYI_SUPPORT)
	wchar temp_text_ptr[ET_MAX_STR_LENGTH] = {0};
#endif

	SCI_GetMutex(g_et_font_info.lock, SCI_WAIT_FOREVER);

	len = len > ET_MAX_STR_LENGTH ? ET_MAX_STR_LENGTH : len;
	etrump_set_font_param_nolock( param_ptr );
#if defined (MULTI_LAN_MYANMAR_ZAWGYI_SUPPORT)
	memcpy(temp_text_ptr,text_ptr,len*sizeof(wchar));
	etrump_myanmar_rule_convert(temp_text_ptr,len);
	dir = (param_ptr->dir == TEXT_DIR_AUTO) ? FT_Font_Get_Line_Direction(temp_text_ptr, len) : param_ptr->dir;/*lint !e605*/
	FT_Font_Set_Text_Order(g_et_font_info.curFont, dir);

	if (is_left)
	{
		*pos_ptr = FT_Font_Prev_Cursor(g_et_font_info.curFont, (FT_WCHAR*)temp_text_ptr, len, (*pos_ptr+1));
	}
	else
	{
		*pos_ptr = FT_Font_Next_Cursor(g_et_font_info.curFont, (FT_WCHAR*)temp_text_ptr, len, (*pos_ptr-1));
	}

#else
	dir = (param_ptr->dir == TEXT_DIR_AUTO) ? FT_Font_Get_Line_Direction(text_ptr, len) : param_ptr->dir;/*lint !e605*/
	FT_Font_Set_Text_Order(g_et_font_info.curFont, dir);

	if (is_left)
	{
		*pos_ptr = FT_Font_Prev_Cursor(g_et_font_info.curFont, (FT_WCHAR*)text_ptr, len, (*pos_ptr+1));
	}
	else
	{
		*pos_ptr = FT_Font_Next_Cursor(g_et_font_info.curFont, (FT_WCHAR*)text_ptr, len, (*pos_ptr-1));
	}
#endif
	SCI_PutMutex(g_et_font_info.lock);
	return 0;
}


/*****************************************************************************/
// 	Description : get cursor pos
//	Global resource dependence : 
//  Author:James.Zhang
//	Note:根据光标x坐标获得光标位置
/*****************************************************************************/
PUBLIC int32 SPMLAPI_GetCursorPos(const SPML_TEXT_PARAM_T* param_ptr,   //显示参数
                                  const wchar*             text_ptr,    //unicode字符串
                                  uint16                   len,         //字符个数
                                  int32                    pos,         //光标原位置
                                  int32                    x,           //光标x坐标
                                  BOOLEAN                  *is_find_ptr //in/out:may PNULL
                                  )
{
	int text_width;
	int align_offset;
	int x_offset;
	BOOLEAN find_ptr;
	int32 cursor = 0;
	int dir = TEXT_DIR_AUTO;
#if defined (MULTI_LAN_MYANMAR_ZAWGYI_SUPPORT)
	wchar temp_text_ptr[ET_MAX_STR_LENGTH] = {0};
#endif

	SCI_GetMutex(g_et_font_info.lock, SCI_WAIT_FOREVER);

	len = len > ET_MAX_STR_LENGTH ? ET_MAX_STR_LENGTH : len;
	etrump_set_font_param_nolock( param_ptr );
#if defined (MULTI_LAN_MYANMAR_ZAWGYI_SUPPORT)
	memcpy(temp_text_ptr,text_ptr,len*sizeof(wchar));
	etrump_myanmar_rule_convert(temp_text_ptr,len);
	dir = (param_ptr->dir == TEXT_DIR_AUTO) ? FT_Font_Get_Line_Direction(temp_text_ptr, len) : param_ptr->dir;/*lint !e605*/
	FT_Font_Set_Text_Order(g_et_font_info.curFont, dir);

	text_width = FT_Font_Get_Text_Width(g_et_font_info.curFont, (FT_WCHAR*)temp_text_ptr, len, 0);
#else

	dir = (param_ptr->dir == TEXT_DIR_AUTO) ? FT_Font_Get_Line_Direction(text_ptr, len) : param_ptr->dir;/*lint !e605*/
	FT_Font_Set_Text_Order(g_et_font_info.curFont, dir);

	text_width = FT_Font_Get_Text_Width(g_et_font_info.curFont, (FT_WCHAR*)text_ptr, len, 0);
#endif	
	if (param_ptr->format & TEXT_FORMAT_CENTER)
	{
		align_offset = (param_ptr->display_rect.right + 1 + param_ptr->display_rect.left - text_width) / 2;
	}
	else if (param_ptr->format & TEXT_FORMAT_RIGHT)
	{
		align_offset = param_ptr->display_rect.right - text_width;
	}
	else
	{
		align_offset = param_ptr->display_rect.left;
	}

	x_offset = x - align_offset;

	if (0 <= x_offset && x_offset <= text_width)
	{
		find_ptr = TRUE;
	}
	else
	{
		find_ptr = FALSE;
	}
	if (is_find_ptr)
	{
		*is_find_ptr = find_ptr;
	}
#if defined (MULTI_LAN_MYANMAR_ZAWGYI_SUPPORT)
	cursor = FT_Font_Get_Cursor(g_et_font_info.curFont, (FT_WCHAR*)temp_text_ptr, len, x_offset);
#else
	cursor = FT_Font_Get_Cursor(g_et_font_info.curFont, (FT_WCHAR*)text_ptr, len, x_offset);
#endif
	SCI_PutMutex(g_et_font_info.lock);

	return cursor;
}


/*==============================================================================
Description: 根据光标x坐标获得光标位置
Note: 同时获得光标与字符的相对位置信息
==============================================================================*/
PUBLIC BOOLEAN SPMLAPI_GetCursorPosEx(const SPML_TEXT_PARAM_T* param_ptr,   //显示参数
                                      const wchar*             text_ptr,    //unicode字符串
                                      uint16                   len,         //字符个数
                                      int32                    x,           //光标x坐标
                                      int32        *nearest_chr_idx_ptr,
                                      int32        *under_chr_idx_ptr,
                                      int32        *under_len_ptr
                                      )
{
	BOOLEAN is_find = FALSE; 
	int32 cursor = 0; 
#if defined (MULTI_LAN_MYANMAR_ZAWGYI_SUPPORT)
	wchar temp_text_ptr[ET_MAX_STR_LENGTH] = {0};
#endif
	
	if ( NULL == nearest_chr_idx_ptr 
		|| NULL == under_chr_idx_ptr 
		|| NULL == under_len_ptr ) 
	{ 
		return FALSE;                         
	} 
	
	if( NULL == param_ptr || NULL == text_ptr || 0 == len ) 
	{ 
		*nearest_chr_idx_ptr = -1; 
		return is_find; 
	} 
#if defined (MULTI_LAN_MYANMAR_ZAWGYI_SUPPORT)
	memcpy(temp_text_ptr,text_ptr,len*sizeof(wchar));
	etrump_myanmar_rule_convert(temp_text_ptr,len);
	cursor = SPMLAPI_GetCursorPos(param_ptr, temp_text_ptr, len, 0, x, &is_find);
#else
	
	cursor = SPMLAPI_GetCursorPos(param_ptr, text_ptr, len, 0, x, &is_find); 
#endif	
	*nearest_chr_idx_ptr = (cursor > 0)? cursor - 1 : 0; 
	*under_chr_idx_ptr = cursor - 1; 
	*under_len_ptr = 1; 
	
	return is_find;
}


/*****************************************************************************/
// 	Description : get text metrics
//	Note:获得字符串宽高
/*****************************************************************************/
PUBLIC int32 SPMLAPI_GetTextMetrics(const SPML_TEXT_PARAM_T* param_ptr,  //显示参数
                                    const wchar*             text_ptr,   //unicode字符串
                                    uint16                   len,        //字符个数
                                    SPML_TEXT_METRICS_T*     metrics_ptr //out, 字符串的宽高
                                    )
{
#if defined (MULTI_LAN_MYANMAR_ZAWGYI_SUPPORT)
	wchar temp_text_ptr[ET_MAX_STR_LENGTH] = {0};
#endif

	SCI_GetMutex(g_et_font_info.lock, SCI_WAIT_FOREVER);

	len = len > ET_MAX_STR_LENGTH ? ET_MAX_STR_LENGTH : len;
	etrump_set_font_param_nolock( param_ptr );
#if defined (MULTI_LAN_MYANMAR_ZAWGYI_SUPPORT)
	memcpy(temp_text_ptr,text_ptr,len*sizeof(wchar));
	etrump_myanmar_rule_convert(temp_text_ptr,len);
	etrump_get_text_metrics_nolock(param_ptr, temp_text_ptr, len, metrics_ptr);
#else

	etrump_get_text_metrics_nolock(param_ptr, text_ptr, len, metrics_ptr);
#endif
	SCI_PutMutex(g_et_font_info.lock);
	return 0;
}


/*****************************************************************************/
// 	Description : draw text
//	Note:绘制字符串
/*****************************************************************************/
PUBLIC int32 SPMLAPI_DrawText(const SPML_TEXT_PARAM_T* param_ptr,  //显示参数
                              const wchar*             text_ptr,   //unicode字符串
                              uint16                   len,        //字符个数
                              SPML_TEXT_METRICS_T*     metrics_ptr //out, 字符串的宽高, 可以为PNULL
                              )
{
	FT_BMP ftbmp;
	int16 x = 0, y = 0;
	SPML_TEXT_METRICS_T metrics;
	int32 count = 0;
	SPML_RECT_T clip_rect;
	int charStyle[ET_MAX_STR_LENGTH] = { 0 };
	int i, j, dir;
	int needFadeout = 0;
#if defined (MULTI_LAN_MYANMAR_ZAWGYI_SUPPORT)
	wchar temp_text_ptr[ET_MAX_STR_LENGTH] = {0};
#endif	
	int wstrLen = etrump_wstrlen(text_ptr);

	len = len > wstrLen ? wstrLen : len;
	len = len > ET_MAX_STR_LENGTH ? ET_MAX_STR_LENGTH : len;

	SCI_GetMutex(g_et_font_info.lock, SCI_WAIT_FOREVER);

	clip_rect.left   = MAX(param_ptr->clip_rect.left,   0);
	clip_rect.top    = MAX(param_ptr->clip_rect.top,    0);
	clip_rect.right  = MIN(param_ptr->clip_rect.right,  param_ptr->buffer_width - 1);
	clip_rect.bottom = MIN(param_ptr->clip_rect.bottom, param_ptr->buffer_height - 1);

	clip_rect.left   = MAX(clip_rect.left,   param_ptr->display_rect.left);
	clip_rect.top    = MAX(clip_rect.top,    param_ptr->display_rect.top);
	clip_rect.right  = MIN(clip_rect.right,  param_ptr->display_rect.right);
	clip_rect.bottom = MIN(clip_rect.bottom, param_ptr->display_rect.bottom);

	ftbmp.Width = param_ptr->display_rect.right - clip_rect.left + 1;
	ftbmp.Height = param_ptr->display_rect.bottom - clip_rect.top + 1;
	ftbmp.displayWidth = clip_rect.right - clip_rect.left + 1;
	ftbmp.displayHeight = clip_rect.bottom - clip_rect.top + 1;

	ftbmp.BitsPerPixel = (param_ptr->buffer_type == SPML_BUF_ARGB888) ? 32 : 16;
	ftbmp.BytesPerLine = (ftbmp.BitsPerPixel >> 3) * param_ptr->buffer_width;
	ftbmp.Base = (unsigned char*)((char*)param_ptr->buffer_ptr)
		+ clip_rect.top * ftbmp.BytesPerLine + clip_rect.left * (ftbmp.BitsPerPixel >> 3);

	etrump_set_font_param_nolock( param_ptr );
#if defined (MULTI_LAN_MYANMAR_ZAWGYI_SUPPORT)
	memcpy(temp_text_ptr,text_ptr,len*sizeof(wchar));
	etrump_myanmar_rule_convert(temp_text_ptr,len);
	etrump_get_text_metrics_nolock(param_ptr, temp_text_ptr, len, &metrics);
#else

	etrump_get_text_metrics_nolock(param_ptr, text_ptr, len, &metrics);
#endif
	if( metrics_ptr ) {
		metrics_ptr->width = metrics.width;
		metrics_ptr->height = metrics.height;
	}

	if ( (param_ptr->format & TEXT_FORMAT_ELLIPSIS)
		&& metrics.width > param_ptr->display_rect.right - param_ptr->display_rect.left + 1) {
		FT_Font_Set_FadeoutWidth(g_et_font_info.curFont, param_ptr->size);
		needFadeout = 1;
	}
	else {
		FT_Font_Set_FadeoutWidth(g_et_font_info.curFont, 0);
	}
#if defined (MULTI_LAN_MYANMAR_ZAWGYI_SUPPORT)
	dir = (param_ptr->dir == TEXT_DIR_AUTO) ? SPMLAPI_GetTextDirection(temp_text_ptr, len) : param_ptr->dir;
#else
	dir = (param_ptr->dir == TEXT_DIR_AUTO) ? SPMLAPI_GetTextDirection(text_ptr, len) : param_ptr->dir;
#endif
	if( dir == TEXT_DIR_RTL ) {
		FT_Font_Set_Text_Order( g_et_font_info.curFont, 1 );
		x = 0;
		if (param_ptr->format & TEXT_FORMAT_CENTER)
		{
			ftbmp.Width -= (param_ptr->display_rect.right - param_ptr->display_rect.left + 1 - metrics.width) / 2;
		}
		else if ( (param_ptr->format & TEXT_FORMAT_RIGHT)
			|| needFadeout )
		{
		}
		else
		{
			ftbmp.Width -= (param_ptr->display_rect.right - param_ptr->display_rect.left + 1 - metrics.width);
		}
	}
	else {
		FT_Font_Set_Text_Order( g_et_font_info.curFont, 0 );
		x = param_ptr->display_rect.left - clip_rect.left;
		if (needFadeout)
		{
		}
		else if (param_ptr->format & TEXT_FORMAT_CENTER)
		{
			x += (param_ptr->display_rect.right - param_ptr->display_rect.left + 1 - metrics.width) / 2;
		}
		else if (param_ptr->format & TEXT_FORMAT_RIGHT)
		{
			x += (param_ptr->display_rect.right - param_ptr->display_rect.left + 1 - metrics.width);
		}
	}

	y = param_ptr->display_rect.top - clip_rect.top + 1;
	if (param_ptr->format & TEXT_FORMAT_BOTTOM)
	{
		y += (param_ptr->display_rect.bottom - param_ptr->display_rect.top - metrics.height);
	}
	else if (param_ptr->format & TEXT_FORMAT_MIDDLE)
	{
		y += (param_ptr->display_rect.bottom - param_ptr->display_rect.top - metrics.height) / 2;
	}

	if (param_ptr->region_num > 0)
	{
		memset(charStyle , 0, sizeof(int) * len);
		for (i = 0; i < param_ptr->region_num; i++)
		{
			int style = 0;
			SPML_COLOR_T lineColor = param_ptr->line_color;
			SPML_COLOR_T regionColor = param_ptr->region_ptr[i].style.color;

			if (param_ptr->region_ptr[i].style.format & TEXT_FORMAT_INVERSE)
				style |= ReverseColor;
			if (param_ptr->region_ptr[i].style.format & TEXT_FORMAT_BOLD)
				style |= Bold;
			if (param_ptr->region_ptr[i].style.format & TEXT_FORMAT_ITALIC)
				style |= Italic;
			if (param_ptr->region_ptr[i].style.format & TEXT_FORMAT_BOLD)
				style |= Bold;
			if (param_ptr->region_ptr[i].style.format & TEXT_FORMAT_UNDERLINE) {
				style |= Underline;
				FT_Font_Set_Underline_Color(g_et_font_info.curFont, 
						FT_RGB( ((lineColor >> 11) << 3) & 0xff, ((lineColor >> 5) << 2) & 0xff, (lineColor << 3) & 0xff ));
			}

			if (style == 0)
			{
				style = Highlight;
				FT_Font_Set_Region_Color(g_et_font_info.curFont, 
					FT_RGB( ((regionColor >> 11) << 3) & 0xff, ((regionColor >> 5) << 2) & 0xff, (regionColor << 3) & 0xff ));
			}
				for (j = param_ptr->region_ptr[i].start; j < param_ptr->region_ptr[i].end; j++)
				{
					charStyle[j] = style;
				}
			}

		FT_Font_Set_Char_Style(g_et_font_info.curFont, charStyle);
	}

	//MMI_CheckAllocatedMemInfo(_D_FILE_NAME, _D_FILE_LINE);
#if defined (MULTI_LAN_MYANMAR_ZAWGYI_SUPPORT)
	count = FT_Font_Text_Out( g_et_font_info.curFont, &ftbmp, x, y, (FT_WCHAR*)temp_text_ptr, len );
#else
	count = FT_Font_Text_Out( g_et_font_info.curFont, &ftbmp, x, y, (FT_WCHAR*)text_ptr, len );
#endif
	//MMI_CheckAllocatedMemInfo(_D_FILE_NAME, _D_FILE_LINE);

	FT_Font_Set_Char_Style(g_et_font_info.curFont, NULL);

	SCI_PutMutex(g_et_font_info.lock);
	return count;
}


/*****************************************************************************/
// 	Description : test char
//	Note:是否支持渲染该unicode
/*****************************************************************************/
PUBLIC BOOLEAN SPMLAPI_TestChar( wchar ch )
{
	FT_Face face = NULL;
	uint32 i;
	int gindex = 0;

	SCI_GetMutex(g_et_font_info.lock, SCI_WAIT_FOREVER);

	for (i = 0; i < g_et_font_info.fontNumber; i++)
	{
		face = FT_Font_Get_Face(g_et_font_info.allFonts[i]);
		gindex = FT_Get_Char_Index(face, ch);
		if (gindex)
		{
			break;
		}
	}
	
	SCI_PutMutex(g_et_font_info.lock);
	return gindex ? TRUE : FALSE;
}
#endif // VECTOR_FONT_SUPPORT_ETRUMP


