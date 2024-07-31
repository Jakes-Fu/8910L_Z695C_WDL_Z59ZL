/*
 * Copyright 2003 Aplix Corporation. All Rights Reserved.
 *
 * JBlend and all Aplix- or JBlend-related trademarks and logos are
 * trademarks or registered trademarks of Aplix in the U.S., Japan and
 * other countries.
 */

#include "mmi_app_ia_java_trc.h"
#include "mmi_custom_define.h"

#include "jwe_log.h"

#include "jas_gfx.h"
#include "jas_gfx_font.h"

#include "spml_api.h"
#include "os_import.h"
#include "os_api.h"

typedef union
{
    uint16 font_id;

    struct
    {
    uint16  face: 4,/*lint !e46*/
            size: 4,/*lint !e46*/
            style:4,/*lint !e46*/
            type: 4;/*lint !e46*/
    } attr;
} JAS_GFX_LOGICAL_FONT_ID_U;


typedef struct
{
    JAS_GFX_FONT_SIZE_E     real_size;
    JAS_GFX_FONT_STYLE_E    real_style;
    JAS_GFX_FONT_FACE_E     real_face;
    int     leading; 
    int     ascent;  
    int     descent; 
    int     height;  
	
    GUI_FONT_T	     native_font;
   GUISTR_STATE_T	     native_state;
} JAS_GFX_LOGICAL_FONT_S;



/*static  JAS_GFX_LOGICAL_FONT_S s_dflt_log_font_dflt = {0};//s_log_font_small == s_dflt_log_font_dflt 
static  JAS_GFX_LOGICAL_FONT_S s_log_font_large = {0};
static  JAS_GFX_LOGICAL_FONT_S s_log_font_medium = {0};*/


static const JAS_GFX_LOGICAL_FONT_S s_dflt_log_font_dflt ={JAS_GFX_FONT_SIZE_SMALL,
	JAS_GFX_FONT_STYLE_PLAIN,
	JAS_GFX_FONT_FACE_DEFAULT,
	0,
	MMI_DEFAULT_SMALL_FONT_SIZE, 
	0,
	MMI_DEFAULT_SMALL_FONT_SIZE,
	MMI_DEFAULT_SMALL_FONT,
	GUISTR_STATE_NONE};
static const JAS_GFX_LOGICAL_FONT_S s_dflt_log_font_dflt_bold ={JAS_GFX_FONT_SIZE_SMALL,
	JAS_GFX_FONT_STYLE_BOLD,
	JAS_GFX_FONT_FACE_DEFAULT,
	0,
	MMI_DEFAULT_SMALL_FONT_SIZE,
	0,
	MMI_DEFAULT_SMALL_FONT_SIZE,
	MMI_DEFAULT_SMALL_FONT,
	GUISTR_STATE_BOLD};
static const JAS_GFX_LOGICAL_FONT_S s_dflt_log_font_dflt_italic ={JAS_GFX_FONT_SIZE_SMALL,
	JAS_GFX_FONT_STYLE_ITALIC ,
	JAS_GFX_FONT_FACE_DEFAULT,
	0,
	MMI_DEFAULT_SMALL_FONT_SIZE,
	0,
	MMI_DEFAULT_SMALL_FONT_SIZE,
	MMI_DEFAULT_SMALL_FONT,
	GUISTR_STATE_ITALIC};
static const JAS_GFX_LOGICAL_FONT_S s_dflt_log_font_dflt_underline ={JAS_GFX_FONT_SIZE_SMALL,
	JAS_GFX_FONT_STYLE_UNDERLINE ,
	JAS_GFX_FONT_FACE_DEFAULT,
	0,
	MMI_DEFAULT_SMALL_FONT_SIZE,
	0,
	MMI_DEFAULT_SMALL_FONT_SIZE,
	MMI_DEFAULT_SMALL_FONT,
	GUISTR_STATE_UNDERLINE};
static const JAS_GFX_LOGICAL_FONT_S s_dflt_log_font_dflt_bold_italic ={JAS_GFX_FONT_SIZE_SMALL,
	JAS_GFX_FONT_STYLE_ITALIC ,
	JAS_GFX_FONT_FACE_DEFAULT,
	0,
	MMI_DEFAULT_SMALL_FONT_SIZE, 
	0,
	MMI_DEFAULT_SMALL_FONT_SIZE,
	MMI_DEFAULT_SMALL_FONT,
	GUISTR_STATE_BOLD|GUISTR_STATE_ITALIC};
static const JAS_GFX_LOGICAL_FONT_S s_dflt_log_font_dflt_bold_underline ={JAS_GFX_FONT_SIZE_SMALL,
	JAS_GFX_FONT_STYLE_UNDERLINE ,
	JAS_GFX_FONT_FACE_DEFAULT,
	0,
	MMI_DEFAULT_SMALL_FONT_SIZE, 
	0,
	MMI_DEFAULT_SMALL_FONT_SIZE,
	MMI_DEFAULT_SMALL_FONT,
	GUISTR_STATE_BOLD|GUISTR_STATE_UNDERLINE};
static const JAS_GFX_LOGICAL_FONT_S s_dflt_log_font_dflt_italic_underline ={JAS_GFX_FONT_SIZE_SMALL,
	JAS_GFX_FONT_STYLE_ITALIC ,
	JAS_GFX_FONT_FACE_DEFAULT,
	0,
	MMI_DEFAULT_SMALL_FONT_SIZE,
	0,
	MMI_DEFAULT_SMALL_FONT_SIZE,
	MMI_DEFAULT_SMALL_FONT,
	GUISTR_STATE_ITALIC|GUISTR_STATE_UNDERLINE};
static const JAS_GFX_LOGICAL_FONT_S s_dflt_log_font_dflt_bold_italic_underline ={JAS_GFX_FONT_SIZE_SMALL,
	JAS_GFX_FONT_STYLE_UNDERLINE ,
	JAS_GFX_FONT_FACE_DEFAULT,
	0,
	MMI_DEFAULT_SMALL_FONT_SIZE,
	0,
	MMI_DEFAULT_SMALL_FONT_SIZE,
	MMI_DEFAULT_SMALL_FONT,
	GUISTR_STATE_BOLD|GUISTR_STATE_ITALIC|GUISTR_STATE_UNDERLINE};

static const JAS_GFX_LOGICAL_FONT_S s_dflt_log_font_medium ={JAS_GFX_FONT_SIZE_MEDIUM,
	JAS_GFX_FONT_STYLE_PLAIN ,
	JAS_GFX_FONT_FACE_DEFAULT,
	0,
	MMI_DEFAULT_NORMAL_FONT_SIZE,
	0,
	MMI_DEFAULT_NORMAL_FONT_SIZE, 
	MMI_DEFAULT_NORMAL_FONT,GUISTR_STATE_NONE};
static const JAS_GFX_LOGICAL_FONT_S s_dflt_log_font_medium_bold ={JAS_GFX_FONT_SIZE_MEDIUM,
	JAS_GFX_FONT_STYLE_BOLD,
	JAS_GFX_FONT_FACE_DEFAULT,
	0,
	MMI_DEFAULT_NORMAL_FONT_SIZE, 
	0,
	MMI_DEFAULT_NORMAL_FONT_SIZE,
	MMI_DEFAULT_NORMAL_FONT,
	GUISTR_STATE_BOLD};
static const JAS_GFX_LOGICAL_FONT_S s_dflt_log_font_medium_italic ={JAS_GFX_FONT_SIZE_MEDIUM,
	JAS_GFX_FONT_STYLE_ITALIC ,
	JAS_GFX_FONT_FACE_DEFAULT,
	0,
	MMI_DEFAULT_NORMAL_FONT_SIZE,
	0,
	MMI_DEFAULT_NORMAL_FONT_SIZE,
	MMI_DEFAULT_NORMAL_FONT,
	GUISTR_STATE_ITALIC};
static const JAS_GFX_LOGICAL_FONT_S s_dflt_log_font_medium_underline ={JAS_GFX_FONT_SIZE_MEDIUM,
	JAS_GFX_FONT_STYLE_UNDERLINE ,
	JAS_GFX_FONT_FACE_DEFAULT,
	0,
	MMI_DEFAULT_NORMAL_FONT_SIZE, 
	0,
	MMI_DEFAULT_NORMAL_FONT_SIZE,
	MMI_DEFAULT_NORMAL_FONT,
	GUISTR_STATE_UNDERLINE};
static const JAS_GFX_LOGICAL_FONT_S s_dflt_log_font_medium_bold_italic ={JAS_GFX_FONT_SIZE_MEDIUM,
	JAS_GFX_FONT_STYLE_ITALIC ,
	JAS_GFX_FONT_FACE_DEFAULT,
	0,
	MMI_DEFAULT_NORMAL_FONT_SIZE, 
	0,
	MMI_DEFAULT_NORMAL_FONT_SIZE,
	MMI_DEFAULT_NORMAL_FONT,
	GUISTR_STATE_BOLD|GUISTR_STATE_ITALIC};
static const JAS_GFX_LOGICAL_FONT_S s_dflt_log_font_medium_bold_underline ={JAS_GFX_FONT_SIZE_MEDIUM,
	JAS_GFX_FONT_STYLE_UNDERLINE ,
	JAS_GFX_FONT_FACE_DEFAULT,
	0,
	MMI_DEFAULT_NORMAL_FONT_SIZE, 
	0,
	MMI_DEFAULT_NORMAL_FONT_SIZE,
	MMI_DEFAULT_NORMAL_FONT,
	GUISTR_STATE_BOLD|GUISTR_STATE_UNDERLINE};
static const JAS_GFX_LOGICAL_FONT_S s_dflt_log_font_medium_italic_underline ={JAS_GFX_FONT_SIZE_MEDIUM,
	JAS_GFX_FONT_STYLE_ITALIC ,
	JAS_GFX_FONT_FACE_DEFAULT,
	0,
	MMI_DEFAULT_NORMAL_FONT_SIZE, 
	0,
	MMI_DEFAULT_NORMAL_FONT_SIZE,
	MMI_DEFAULT_NORMAL_FONT,
	GUISTR_STATE_ITALIC|GUISTR_STATE_UNDERLINE};
static const JAS_GFX_LOGICAL_FONT_S s_dflt_log_font_medium_bold_italic_underline ={JAS_GFX_FONT_SIZE_MEDIUM,
	JAS_GFX_FONT_STYLE_UNDERLINE ,
	JAS_GFX_FONT_FACE_DEFAULT,
	0,
	MMI_DEFAULT_NORMAL_FONT_SIZE, 
	0,
	MMI_DEFAULT_NORMAL_FONT_SIZE,
	MMI_DEFAULT_NORMAL_FONT,
	GUISTR_STATE_BOLD|GUISTR_STATE_ITALIC|GUISTR_STATE_UNDERLINE};

static  const JAS_GFX_LOGICAL_FONT_S s_dflt_log_font_large ={JAS_GFX_FONT_SIZE_LARGE,
	JAS_GFX_FONT_STYLE_PLAIN ,
	JAS_GFX_FONT_FACE_DEFAULT,
	0,
	MMI_DEFAULT_BIG_FONT_SIZE, 
	0,
	MMI_DEFAULT_BIG_FONT_SIZE,
	MMI_DEFAULT_BIG_FONT,
	GUISTR_STATE_NONE};
static  const JAS_GFX_LOGICAL_FONT_S s_dflt_log_font_large_bold ={JAS_GFX_FONT_SIZE_LARGE,
	JAS_GFX_FONT_STYLE_BOLD,
	JAS_GFX_FONT_FACE_DEFAULT,
	0,
	MMI_DEFAULT_BIG_FONT_SIZE, 
	0,
	MMI_DEFAULT_BIG_FONT_SIZE,
	MMI_DEFAULT_BIG_FONT,
	GUISTR_STATE_BOLD};
static  const JAS_GFX_LOGICAL_FONT_S s_dflt_log_font_large_italic ={JAS_GFX_FONT_SIZE_LARGE,
	JAS_GFX_FONT_STYLE_ITALIC,
	JAS_GFX_FONT_FACE_DEFAULT,
	0,
	MMI_DEFAULT_BIG_FONT_SIZE, 
	0,
	MMI_DEFAULT_BIG_FONT_SIZE,
	MMI_DEFAULT_BIG_FONT,
	GUISTR_STATE_ITALIC};
static const JAS_GFX_LOGICAL_FONT_S s_dflt_log_font_large_underline ={JAS_GFX_FONT_SIZE_LARGE,
	JAS_GFX_FONT_STYLE_UNDERLINE,
	JAS_GFX_FONT_FACE_DEFAULT,
	0,
	MMI_DEFAULT_BIG_FONT_SIZE, 
	0,
	MMI_DEFAULT_BIG_FONT_SIZE,
	MMI_DEFAULT_BIG_FONT,
	GUISTR_STATE_UNDERLINE};
static const JAS_GFX_LOGICAL_FONT_S s_dflt_log_font_large_bold_italic ={JAS_GFX_FONT_SIZE_LARGE,
	JAS_GFX_FONT_STYLE_ITALIC ,
	JAS_GFX_FONT_FACE_DEFAULT,
	0,
	MMI_DEFAULT_BIG_FONT_SIZE,
	0,
	MMI_DEFAULT_BIG_FONT_SIZE,
	MMI_DEFAULT_BIG_FONT,
	GUISTR_STATE_BOLD|GUISTR_STATE_ITALIC};
static const JAS_GFX_LOGICAL_FONT_S s_dflt_log_font_large_bold_underline ={JAS_GFX_FONT_SIZE_LARGE,
	JAS_GFX_FONT_STYLE_UNDERLINE ,
	JAS_GFX_FONT_FACE_DEFAULT,
	0,
	MMI_DEFAULT_BIG_FONT_SIZE, 
	0,
	MMI_DEFAULT_BIG_FONT_SIZE,
	MMI_DEFAULT_BIG_FONT,
	GUISTR_STATE_BOLD|GUISTR_STATE_UNDERLINE};
static const JAS_GFX_LOGICAL_FONT_S s_dflt_log_font_large_italic_underline ={JAS_GFX_FONT_SIZE_LARGE,
	JAS_GFX_FONT_STYLE_ITALIC ,
	JAS_GFX_FONT_FACE_DEFAULT,
	0,
	MMI_DEFAULT_BIG_FONT_SIZE, 
	0,
	MMI_DEFAULT_BIG_FONT_SIZE,
	MMI_DEFAULT_BIG_FONT,
	GUISTR_STATE_ITALIC|GUISTR_STATE_UNDERLINE};
static const JAS_GFX_LOGICAL_FONT_S s_dflt_log_font_large_bold_italic_underline ={JAS_GFX_FONT_SIZE_LARGE,
	JAS_GFX_FONT_STYLE_UNDERLINE ,
	JAS_GFX_FONT_FACE_DEFAULT,
	0,
	MMI_DEFAULT_BIG_FONT_SIZE, 
	0,
	MMI_DEFAULT_BIG_FONT_SIZE,
	MMI_DEFAULT_BIG_FONT,
	GUISTR_STATE_BOLD|GUISTR_STATE_ITALIC|GUISTR_STATE_UNDERLINE};

static const JAS_GFX_LOGICAL_FONT_S *
s_dflt_log_fonts[JAS_GFX_FONT_TYPE_NUM - 1] =
{
    &s_dflt_log_font_dflt, 
    &s_dflt_log_font_dflt, 
    &s_dflt_log_font_dflt  
};

static const JAS_GFX_LOGICAL_FONT_S *
s_log_fonts[JAS_GFX_FONT_FACE_NUM]
[JAS_GFX_FONT_SIZE_NUM - 1]
[JAS_GFX_FONT_STYLE_NUM - 1] =
{

	/* default face */
	/* small size  */
/*	&s_dflt_log_font_dflt, &s_dflt_log_font_dflt, &s_dflt_log_font_dflt,
	&s_dflt_log_font_dflt, &s_dflt_log_font_dflt, &s_dflt_log_font_dflt,
	&s_dflt_log_font_dflt, &s_dflt_log_font_dflt, */
	&s_dflt_log_font_dflt, &s_dflt_log_font_dflt_bold,&s_dflt_log_font_dflt_italic,
	&s_dflt_log_font_dflt_bold_italic ,&s_dflt_log_font_dflt_underline,&s_dflt_log_font_dflt_bold_underline,
	&s_dflt_log_font_dflt_italic_underline, &s_dflt_log_font_dflt_bold_italic_underline, 

	/* medium size */
/*	&s_log_font_medium, &s_log_font_medium, &s_log_font_medium,
	&s_log_font_medium, &s_log_font_medium, &s_log_font_medium,
	&s_log_font_medium, &s_log_font_medium, */
	&s_dflt_log_font_medium,&s_dflt_log_font_medium_bold,&s_dflt_log_font_medium_italic,
	&s_dflt_log_font_medium_bold_italic,&s_dflt_log_font_medium_underline,&s_dflt_log_font_medium_bold_underline,
	&s_dflt_log_font_medium_italic_underline,&s_dflt_log_font_medium_bold_italic_underline,
	
	/* large size  */
/*	&s_log_font_large,     &s_log_font_large,     &s_log_font_large,
	&s_log_font_large,     &s_log_font_large,     &s_log_font_large,
	&s_log_font_large,     &s_log_font_large,     */
	&s_dflt_log_font_large,&s_dflt_log_font_large_bold,&s_dflt_log_font_large_italic,
	&s_dflt_log_font_large_bold_italic,&s_dflt_log_font_large_underline,&s_dflt_log_font_large_bold_underline,
	&s_dflt_log_font_large_italic_underline,&s_dflt_log_font_large_bold_italic_underline,



	/* monospace face */
	/* small size  */
/*	&s_dflt_log_font_dflt, &s_dflt_log_font_dflt, &s_dflt_log_font_dflt,
	&s_dflt_log_font_dflt, &s_dflt_log_font_dflt, &s_dflt_log_font_dflt,
	&s_dflt_log_font_dflt, &s_dflt_log_font_dflt, */
	&s_dflt_log_font_dflt, &s_dflt_log_font_dflt_bold,&s_dflt_log_font_dflt_italic,
	&s_dflt_log_font_dflt_bold_italic ,&s_dflt_log_font_dflt_underline,&s_dflt_log_font_dflt_bold_underline,
	&s_dflt_log_font_dflt_italic_underline, &s_dflt_log_font_dflt_bold_italic_underline, 
	
	/* medium size */
/*	&s_log_font_medium, &s_log_font_medium, &s_log_font_medium,
	&s_log_font_medium, &s_log_font_medium, &s_log_font_medium,
	&s_log_font_medium, &s_log_font_medium,*/
	&s_dflt_log_font_medium,&s_dflt_log_font_medium_bold,&s_dflt_log_font_medium_italic,
	&s_dflt_log_font_medium_bold_italic,&s_dflt_log_font_medium_underline,&s_dflt_log_font_medium_bold_underline,
	&s_dflt_log_font_medium_italic_underline,&s_dflt_log_font_medium_bold_italic_underline,
	
	/* large size  */
/*	&s_log_font_large,     &s_log_font_large,     &s_log_font_large,
	&s_log_font_large,     &s_log_font_large,     &s_log_font_large,
	&s_log_font_large,     &s_log_font_large,     */
	&s_dflt_log_font_large,&s_dflt_log_font_large_bold,&s_dflt_log_font_large_italic,
	&s_dflt_log_font_large_bold_italic,&s_dflt_log_font_large_underline,&s_dflt_log_font_large_bold_underline,
	&s_dflt_log_font_large_italic_underline,&s_dflt_log_font_large_bold_italic_underline,


	/* proportional face */
	/* small size  */
/*	&s_dflt_log_font_dflt, &s_dflt_log_font_dflt, &s_dflt_log_font_dflt,
	&s_dflt_log_font_dflt, &s_dflt_log_font_dflt, &s_dflt_log_font_dflt,
	&s_dflt_log_font_dflt, &s_dflt_log_font_dflt,*/
	&s_dflt_log_font_dflt, &s_dflt_log_font_dflt_bold,&s_dflt_log_font_dflt_italic,
	&s_dflt_log_font_dflt_bold_italic ,&s_dflt_log_font_dflt_underline,&s_dflt_log_font_dflt_bold_underline,
	&s_dflt_log_font_dflt_italic_underline, &s_dflt_log_font_dflt_bold_italic_underline, 
	
	/* medium size */
/*	&s_log_font_medium, &s_log_font_medium, &s_log_font_medium,
	&s_log_font_medium, &s_log_font_medium, &s_log_font_medium,
	&s_log_font_medium, &s_log_font_medium, */
	&s_dflt_log_font_medium,&s_dflt_log_font_medium_bold,&s_dflt_log_font_medium_italic,
	&s_dflt_log_font_medium_bold_italic,&s_dflt_log_font_medium_underline,&s_dflt_log_font_medium_bold_underline,
	&s_dflt_log_font_medium_italic_underline,&s_dflt_log_font_medium_bold_italic_underline,
	/* large size  */
/*	&s_log_font_large,     &s_log_font_large,     &s_log_font_large,
	&s_log_font_large,     &s_log_font_large,     &s_log_font_large,
	&s_log_font_large,     &s_log_font_large */
	&s_dflt_log_font_large,&s_dflt_log_font_large_bold,&s_dflt_log_font_large_italic,
	&s_dflt_log_font_large_bold_italic,&s_dflt_log_font_large_underline,&s_dflt_log_font_large_bold_underline,
	&s_dflt_log_font_large_italic_underline,&s_dflt_log_font_large_bold_italic_underline
};

/*****************************************************************************/
//  Description : jasGfxGetLogicFonts
//  Global resource dependence : 
//  Author:siyuan
//  Note: 
/*****************************************************************************/
LOCAL void jasGfxGetLogicFonts(uint log_font_id,
                                    const JAS_GFX_LOGICAL_FONT_S **font)
{
	JAS_GFX_LOGICAL_FONT_ID_U log_font_id_u;
	
	
	if (PNULL == font)
	{
	    return;
	}

	log_font_id_u.font_id = log_font_id;
	
	//JWE_LOG(jasGfxGetLogicFonts, ("log_font_id = %d",log_font_id));     


	if (log_font_id_u.attr.type > JAS_GFX_FONT_TYPE_UNKNOWN &&
	    log_font_id_u.attr.type < JAS_GFX_FONT_TYPE_NUM)
	{

		/*default font type*/
	    *font = (JAS_GFX_LOGICAL_FONT_S *)
	            s_dflt_log_fonts[log_font_id_u.attr.type - 1];
	}
	else
	{
	    /*special font type for JkFontCreate*/
	    if (JAS_GFX_FONT_SIZE_UNKNOWN == log_font_id_u.attr.size ||
	        JAS_GFX_FONT_SIZE_NUM <= log_font_id_u.attr.size ||
	        JAS_GFX_FONT_STYLE_NUM <= log_font_id_u.attr.style ||
	        //JAS_GFX_FONT_FACE_UNKNOWN >= log_font_id_u.attr.face ||
	        JAS_GFX_FONT_FACE_NUM <= log_font_id_u.attr.face)
	    {
	        JWE_LOG(jasGfxGetLogicFonts, ("error "));     
	        *font = NULL;
	        return;
	    }
		
	    *font = s_log_fonts[log_font_id_u.attr.face]
	                       [log_font_id_u.attr.size - 1]
	                       [log_font_id_u.attr.style];

	}
}

/*****************************************************************************/
//  Description : jas_GfxFontInit
//  Global resource dependence : 
//  Author:siyuan
//  Note: 
/*****************************************************************************/
PUBLIC void jas_GfxFontInit()
{
	//jasGfxInitLogicFonts();
}

/*****************************************************************************/
//  Description : jas_GfxFontFini
//  Global resource dependence : 
//  Author:siyuan
//  Note: 
/*****************************************************************************/
PUBLIC void jas_GfxFontFini()
{
	//jasGfxCleanLogicFonts();//No need clean
}

/*****************************************************************************/
//  Description : to create font and init it with default value
//  Global resource dependence : 
//  Author:siyuan
//  Note: 
/*****************************************************************************/
PUBLIC void jas_GfxGetDefaultFont(JAS_GFX_FONT_TYPE_E type, JAS_GFX_FONT_S *font)
{
	
	JAS_GFX_LOGICAL_FONT_ID_U       id;
	const JAS_GFX_LOGICAL_FONT_S*   p_log_font = PNULL;
	
	JWE_LOG(jas_GfxGetDefaultFont, ("type=%d", type));
	
	if (JAS_GFX_FONT_TYPE_NUM == type || PNULL == font)
	{
	    return;
	}

	if (JAS_GFX_FONT_TYPE_UNKNOWN == type)
	{
	    type = JAS_GFX_FONT_TYPE_DEFAULT;
	}
	
	id.font_id = 0;
	id.attr.type = type;
	font->log_font_id = id.font_id;

	jasGfxGetLogicFonts(font->log_font_id, &p_log_font);

    if (PNULL != p_log_font)
    {
        font->size = p_log_font->real_size;
	    font->style = p_log_font->real_style;
	    font->face = p_log_font->real_face;
	    font->ascent = p_log_font->ascent;
	    font->descent = p_log_font->descent;
	    font->height = p_log_font->height;
    }

	return;
}


/*****************************************************************************/
//  Description : to create font and init it
//  Global resource dependence : 
//  Author:siyuan
//  Note: 
/*****************************************************************************/
PUBLIC void jas_GfxGetFont(JAS_GFX_FONT_S *font)
{
	JAS_GFX_LOGICAL_FONT_ID_U       id;
	const JAS_GFX_LOGICAL_FONT_S*   p_log_font = PNULL;
    
	JWE_LOG(jas_GfxGetFont, ("enter"));     
	
	if (PNULL == font ||
	    JAS_GFX_FONT_FACE_NUM == font->face ||
	    JAS_GFX_FONT_STYLE_NUM == font->style ||
	    JAS_GFX_FONT_SIZE_NUM == font->size)
	{
	    return;
	}

	if (JAS_GFX_FONT_FACE_UNKNOWN == font->face)
	{
	    font->face = JAS_GFX_FONT_FACE_DEFAULT;
	}
	if (JAS_GFX_FONT_STYLE_UNKNOWN == font->style)
	{
	    font->style = JAS_GFX_FONT_STYLE_PLAIN;
	}
	if (JAS_GFX_FONT_SIZE_UNKNOWN == font->size)
	{
	    font->size = JAS_GFX_FONT_SIZE_SMALL;
	}
 

	id.font_id = 0;
	id.attr.face = font->face;
	id.attr.size = font->size;
	id.attr.style = font->style;
	font->log_font_id = id.font_id;
  
	jasGfxGetLogicFonts(font->log_font_id, &p_log_font);

    if (PNULL != p_log_font)
    {
        font->size = p_log_font->real_size;
	    font->style = p_log_font->real_style;
	    font->face = p_log_font->real_face;
	    font->ascent = p_log_font->ascent;
	    font->descent = p_log_font->descent;
	    font->height = p_log_font->height;
	    font->leading = p_log_font->leading;
    }

}

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
PUBLIC void jas_GfxDrawString(JAS_GFX_SCREEN_BUFF_S *screen,uint log_font_id,const wchar *wstr,int length,int x,int y,const JAS_GFX_CLIP_S *clip,JAS_GFX_PIXEL color)
{
	const JAS_GFX_LOGICAL_FONT_S *  p_log_font = PNULL;
	GUI_RECT_T                      clip_rect = {0};
	GUI_RECT_T                      font_rect = {0};
	//GUISTR_BUF_T                    des_buf_data = {0};
	//GUISTR_STYLE_T                  str_style = {0};
	GUISTR_STATE_T                  str_state = GUISTR_STATE_SINGLE_LINE;
	//MMI_STRING_T                    str    = {0};
    SPML_TEXT_PARAM_T               param            = {0};
    SPML_TEXT_METRICS_T             metrics_data = {0};

	//JWE_LOG(jas_GfxDrawString, ("enter screen=0x%x log_font_id=%d length=%d x=%d y=%d color=0x%x",screen, log_font_id,length ,x ,y,color));     
	/* Invalid argument. */
	if (PNULL == screen || PNULL == wstr || PNULL == clip)
	{
	    return;
	}

	jasGfxGetLogicFonts(log_font_id, &p_log_font);

	/* log_font_id is invalid. */
	if (PNULL == p_log_font)
	{
	    return;
	}

    #if 0
	/* Do nothing is no characters are covered by the clip area. */
	if (x > (clip->x + clip->w) || /* Clip too left. Should consider RTL. */
	    (y + p_log_font->height) < clip->y || /* Clip too low. */
	    y > (clip->y + clip->h))/* Clip too high. */
	{
	    return;
	}
    #endif
	clip_rect = GUI_CreateRect(clip->x, clip->y, clip->w + clip->x -1, clip->h + clip->y -1);
	font_rect = GUI_CreateRect(x, y, screen->width - 1, screen->height - 1);
    #if 0
	des_buf_data.buffer = (GUI_COLOR_T*)screen->data;
    des_buf_data.mem_width = des_buf_data.width  = screen->width;
	des_buf_data.width  = screen->width;
	des_buf_data.height = screen->height;

	str_style.font       = p_log_font->native_font;
	str_style.font_color = color;

	str.wstr_ptr =(wchar *)wstr;
	str.wstr_len = length;
	
	str_state |= p_log_font->native_state;
    GUISTR_DrawTextInRect(&des_buf_data, &font_rect, &clip_rect, &str, &str_style, str_state, -1 );

    #else
	str_state |= p_log_font->native_state;

    param.buffer_ptr = (GUI_COLOR_T*)screen->data;
    param.buffer_width = screen->width;
    param.buffer_height = screen->height;
    
    param.display_rect = font_rect;
    param.clip_rect = clip_rect;
    param.font_color = color;
    param.size = p_log_font->native_font;

    //粗体
    if ( str_state & GUISTR_STATE_BOLD )
    {
       param.format |= TEXT_FORMAT_BOLD;
    }
    //斜体
    if ( str_state & GUISTR_STATE_ITALIC )
    {
        param.format |= TEXT_FORMAT_ITALIC;
    }
    //下划线
    if ( str_state & GUISTR_STATE_UNDERLINE )
    {
        param.format |= TEXT_FORMAT_UNDERLINE;
    }
//	SCI_TRACE_LOW("java jas_gfx_font jas_GfxDrawString before SPMLAPI_DrawText, length= %d ,time= %d", length, OS_GetTickCount());
    SPMLAPI_DrawText(&param,wstr,length,&metrics_data);    
//	SCI_TRACE_LOW("java jas_gfx_font jas_GfxDrawString after SPMLAPI_DrawText, time=%d",OS_GetTickCount());
    #endif
}

/*****************************************************************************/
//  Description : get string pixel width by given font
//  Global resource dependence : 
//  Author:siyuan
//  Note: 
/*****************************************************************************/
PUBLIC void jas_GfxGetStringWidth(uint log_font_id,const wchar *wstr,int length,uint *width)
{
	uint                            nWidth =0;
	const JAS_GFX_LOGICAL_FONT_S*   p_log_font = PNULL;
	
	//JWE_LOG(jas_GfxGetStringWidth, ("enter log_font_id=%d wstr=0x%x length=%d ",log_font_id,wstr,length));   

    if(PNULL == width)
    {
        return;
    }
    
	if (PNULL == wstr)
	{
	    *width=0;
        return;
	}
	
	jasGfxGetLogicFonts(log_font_id, &p_log_font);


	if (PNULL == p_log_font)
	{
	    *width=0;
        return;
	}
//	SCI_TRACE_LOW("java jas_gfx_font  jas_GfxGetStringWidth before GUI_GetStringWidth, length= %d time=%d", length, OS_GetTickCount());
    nWidth = GUI_GetStringWidth((GUI_FONT_T)p_log_font->native_font,wstr,length);  
//	SCI_TRACE_LOW("java jas_gfx_font  jas_GfxGetStringWidth after GUI_GetStringWidth, width=%d time=%d", nWidth, OS_GetTickCount());
	*width=nWidth;
}


/*Need optimize!*/
/*****************************************************************************/
//  Description : get chars count by given length and font 
//  Global resource dependence : 
//  Author:siyuan
//  Note: 
/*****************************************************************************/
PUBLIC void jas_GfxGetCharCount(uint log_font_id, const wchar *wstr,int length,int width,uint16 *count)
{

	int32 i = 0;
	const JAS_GFX_LOGICAL_FONT_S *  p_log_font = PNULL;

    int32 temp_width = 0;
    int32 length1 = 0;
    int32 length2 = 0 ;
    int32 j = 0;
    
//	SCI_TRACE_LOW("java jas_gfx_font  jas_GfxGetCharCount enter, length= %d, width = %d ", length, width );
    if(PNULL == count)
    {
        return;
    }
    
	if (PNULL == wstr)
	{
	    *count=0;
        return;
	}

	if(0 == width)
	{
	    *count=0;	    
         return;
	}
	
	jasGfxGetLogicFonts(log_font_id, &p_log_font);

	/* log_font_id is invalid. */
	if (PNULL == p_log_font)
	{
	    *count=0;
        return;
	}


	//check max length
	temp_width = GUI_GetStringWidth((GUI_FONT_T)p_log_font->native_font,wstr,length) ;
	if( temp_width <= width )
	{
	    *count = length;	    
//        SCI_TRACE_LOW("java jas_gfx_font jas_GfxGetCharCount leave, count = %d, width = %d" ,length, temp_width);
        return;
	}
		
/*  old code
	for(i = length; i > 0; i--) 
	{
		if(GUI_GetStringWidth((GUI_FONT_T)p_log_font->native_font,wstr,i) <= width)
		{	
			*count = i;
            //SCI_TRACE_LOW:"java jas_gfx_font jas_GfxGetCharCount end, loop = %d time=%d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,JAS_GFX_FONT_711_112_2_18_2_22_1_5,(uint8*)"dd",length - i ,OS_GetTickCount());
			return;
		}
	}
*/
//	SCI_TRACE_LOW("java jas_gfx_font jas_GfxGetCharCount binary search start, length = %d time=%d",length ,OS_GetTickCount());

	//binary search
    length1 = 1;
    length2 = length ;
	while(length1 + 1 < length2)
	{
	    i = (length1 + length2) >> 1;
	    temp_width = GUI_GetStringWidth((GUI_FONT_T)p_log_font->native_font,wstr,i) ;
		if(temp_width > width)
		{	
			length2 = i;
		}
		else
		{
		    length1 = i;
		}
	}
//    SCI_TRACE_LOW("JAVA JAS_GFX_FONT jas_GfxGetCharCount binary search end i = %d width= %d time =%d", i, temp_width, OS_GetTickCount()); 
    
    if(temp_width > width)
    {   
        for(j = i - 1; j > 0; j--) 
        {
            //--
            if(GUI_GetStringWidth((GUI_FONT_T)p_log_font->native_font,wstr,j) <= width)
            {   
                *count = j;
//                SCI_TRACE_LOW("java jas_gfx_font jas_GfxGetCharCount end, loop = %d time=%d",i - j ,OS_GetTickCount());
                return;
        
            }
        }
    }
    else
    {
        for(j = i + 1; j < length; j++) 
        {
            //++
            if(GUI_GetStringWidth((GUI_FONT_T)p_log_font->native_font,wstr,j) > width)
            {   
                *count = j - 1;
//                SCI_TRACE_LOW("java jas_gfx_font jas_GfxGetCharCount end, loop = %d time=%d", j - i ,OS_GetTickCount());
                return;
        
            }
        }
    }
	*count = i;	
//    SCI_TRACE_LOW("java jas_gfx_font jas_GfxGetCharCount leave, count = %d" ,*count);
    return;
}

