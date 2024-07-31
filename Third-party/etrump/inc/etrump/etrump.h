/////////////////////////////////////////////////////////////////////////
/// \file etrump/etrump.h
/// \brief The code to manage the instance code
/// \details
/// <br><br>
/// \author etrump
/// \version 1.00
/// \date 2008/11/27
///
/////////////////////////////////////////////////////////////////////////
#ifndef   _ETRUMP_H_
#define   _ETRUMP_H_

/// if the function FT_Font_Create_From_File or FT_Font_Create_From_Memory is invoked only
/// once, define this macro will make application faster.
//#define   STATIC_MODE

/*************************************************************************
	CRT显示器AV输出颜色校正
*************************************************************************/
//#define   CRT_AV_PORT

/// support complex language, such as Arabic, Uyghur.
#define   SUPPORT_COMPLEX_LANGUAGE

/*************************************************************************
	定义半透明字效果。
*************************************************************************/
//#define   SUPPORT_ALPHA_BLEND

#include  <ft2build.h>
#include  FT_FREETYPE_H
#include  FT_CACHE_MANAGER_H

typedef   int                FT_BOOL;
typedef   signed long        FT_2DOT6;
typedef   unsigned short     FT_WCHAR;
typedef   unsigned long      FT_COLOR;

/*************************************************************************
	FT2DOT6ToLong   定点数到整数
	LongToFT2DOT6   整数到定点数
*************************************************************************/
#define   FT2DOT6ToLong( x ) ( (x) >> 6 )
#define   LongToFT2DOT6( x ) ( (x) << 6 )

/// define color structure
#define   FT_RGB(r, g, b)    ( 0xff000000 | (((FT_COLOR)(r))<<16) | (((FT_COLOR)(g))<<8) | ((FT_COLOR)(b)) )
#define   FT_ARGB(a, r, g, b) ( (((FT_COLOR)(a))<<24) | (((FT_COLOR)(r))<<16) | (((FT_COLOR)(g))<<8) | ((FT_COLOR)(b)) )
#define   FT_GetAValue(rgb)  ((unsigned char)((rgb)>>24))  ///< get the alpha value from the given color
#define   FT_GetRValue(rgb)  ((unsigned char)((rgb)>>16))  ///< get the red value from the given color
#define   FT_GetGValue(rgb)  ((unsigned char)((rgb)>>8))   ///< get the green value from the given color
#define   FT_GetBValue(rgb)  ((unsigned char)(rgb))        ///< get the blue value from the given color

#ifdef    __cplusplus
extern "C" {
#endif

typedef enum FontStyle
{
	Regular      = 0,   ///< Regular 
	Bold         = 1,   ///< Bold 
	Italic       = 2,   ///< Italic 
	BoldItalic   = 3,   ///< Bold-Italic 
	Underline    = 4,   ///< Underline
	Crochet      = 8,   ///< Crochet 
	Hollow       = 16,  ///< Hollow
    ReverseColor = 32,  ///< ReverseColor
    Highlight    = 64,  ///< Highlight 
	Shadow       = 128, ///< Shadow
	Rotate       = 256  ///< Rotate
}FontStyle;


/* [Henry] text horizontal/ vertical alignment */
/*************************************************************************
	定义对齐格式
*************************************************************************/
//#define SUPPORT_TEXT_ALIGNMENT

#define TEXT_HORIZONTAL_ALIGNMENT_MASK		0x00FF
#define TEXT_VERTICAL_ALIGNMENT_MASK		0xFF00

typedef enum TextHorizontalAlignment
{
	TextHorizontalAlignmentLeft			= 0,
	TextHorizontalAlignmentCenter		= 1,
	TextHorizontalAlignmentRight		= 2,
	TextHorizontalAlignmentJustify		= 3,
	TextHorizontalAlignmentDistribute	= 4,
} TextHorizontalAlignment;

typedef enum TextVerticalAlignment
{
	TextVerticalAlignmentTop			= 0,
	TextVerticalAlignmentMiddle			= 1,
	TextVerticalAlignmentBottom			= 2,
	TextVerticalAlignmentBaseline		= 3,
} TextVerticalAlignment;

typedef enum TextLineBreakMode
{
	LineBreakByWordWrapping				= 0,
    LineBreakByCharWrapping				= 1,
    LineBreakByClipping					= 2,
    LineBreakByTruncatingHead			= 3,
    LineBreakByTruncatingTail			= 4,
    LineBreakByTruncatingMiddle			= 5,
} TextLineBreakMode;


#define BPP16_B_BITS  5  ///< the number bits of blue part
#define BPP16_G_BITS  6  ///< the number bits of green part
#define BPP16_R_BITS  5  ///< the number bits of red part
#define BPP16_A_BITS  0  ///< the number bits of alpha part

typedef struct  
{
	int            Width;         ///< the bitmap width    
	int            Height;        ///< the bitmap height
	int            displayWidth;  ///< max width allows to draw glyph
	int            displayHeight; ///< max height allows to draw glyph
	int            BitsPerPixel;  ///< bits per pixel
	int            BytesPerLine;  ///< bytes per line
	unsigned char  *Base;         ///< the pointer to the bitmap buffer
}FT_BMP;

typedef struct
{
	unsigned short  platform_id;
	unsigned short	encoding_id;
	unsigned short  language_id;
	unsigned short  name_id;

	unsigned char	*string;      /* this string is *not* null-terminated! */
	int				string_len;   /* in bytes */
} FT_FontName;

typedef struct 
{
	int offset_x;
	int offset_y;
	int shadow_color;
	int shadow_depth;   ///< shadow depth level, from 1 to 16
} FT_ShadowInfo;

typedef struct FT_FontRec FT_Font; ///< etrump font struct

/////////////////////////////////////////////////////////////////////////
/// \brief create font from file
/// \param[out] afont ::A handle to a font
/// \param[in] library ::A handle to a freetype library
/// \param[in] fontpath ::A handle to the font path
/// \return 1 means success and 0 means error
///////////////////////////////////////////////////////////////////////
FT_BOOL FT_Font_Create_From_File( FT_Font **afont, FT_Library library, const char *fontpath );

/////////////////////////////////////////////////////////////////////////
/// \brief create font from memory
/// \param[out] afont ::A handle to a font
/// \param[in] library ::A handle to a freetype library
/// \param[in] fontData ::A handle to the font memory data 
/// \param[in] fontpath ::A handle to the font data length
/// \return 1 means success and 0 means error
///////////////////////////////////////////////////////////////////////
FT_BOOL FT_Font_Create_From_Memory( FT_Font **afont, FT_Library library, FT_Byte *fontData, FT_UInt32 fontLen );

/////////////////////////////////////////////////////////////////////////
/// \brief create font from args
/// \param[out] afont ::A handle to a font
/// \param[in] library ::A handle to a freetype library
/// \param[in] args ::A handle to the font args 
/// \return 1 means success and 0 means error
///////////////////////////////////////////////////////////////////////
FT_BOOL FT_Font_Create_From_Args( FT_Font **afont, FT_Library library, FT_Open_Args *args );

/////////////////////////////////////////////////////////////////////////
/// \brief reset to default font
/// \param[in] font ::A handle to a font
/// \return 1 means success and 0 means error
///////////////////////////////////////////////////////////////////////
FT_BOOL FT_Font_Reset( FT_Font *font );

/////////////////////////////////////////////////////////////////////////
/// \brief destruct font
/// \param[in] font ::A handle to a font
/// \return void
///////////////////////////////////////////////////////////////////////
void    FT_Font_Delete( FT_Font *font );

/////////////////////////////////////////////////////////////////////////
/// \brief get sfnt name count
/// \param[in] font ::A handle to a font
/// \return sfnt name count
///////////////////////////////////////////////////////////////////////
int FT_Font_Get_Sfnt_Name_Count( FT_Font *font );

/////////////////////////////////////////////////////////////////////////
/// \brief get sfnt name
/// \param[in] font ::A handle to a font
/// \param[out] aname ::The FT_FontName structure
/// \return 1 means success and 0 means error
///////////////////////////////////////////////////////////////////////
FT_BOOL FT_Font_Get_Sfnt_Name( FT_Font *font, int idx, FT_FontName *aname );

/////////////////////////////////////////////////////////////////////////
/// \brief get font name
/// \param[in] font ::A handle to a font
/// \param[in] system_lang_id ::The platform-specific Language ID's assigned by Microsoft
/// \param[out] name ::The name string represented in ucs-2
/// \param[out] name_len ::The length of name string
/// \param[in] max_len ::Max len of name string
/// \return 1 means success and 0 means error
///////////////////////////////////////////////////////////////////////
FT_BOOL FT_Font_Get_Name( FT_Font *font, FT_WCHAR *name, int *name_len, int max_len );

/////////////////////////////////////////////////////////////////////////
/// \brief get font name by language id
/// \param[in] font ::A handle to a font
/// \param[in] system_lang_id ::The platform-specific Language ID's assigned by Microsoft
/// \param[out] name ::The name string represented in ucs-2
/// \param[out] name_len ::The length of name string
/// \param[in] max_len ::Max len of name string
/// \param[in] language_id ::the language id specified in M$ TrueType Spec
/// \return 1 means success and 0 means error
///////////////////////////////////////////////////////////////////////
FT_BOOL FT_Font_Get_Name_By_LanguageId( FT_Font *font, FT_WCHAR *name, int *name_len, int max_len, int language_id );

/////////////////////////////////////////////////////////////////////////
/// \brief external width for glyph
/// \param[in] font ::A handle to a font
/// \param[in] gap  ::The value of external width
/// \return 1 means true, 0 means error
///////////////////////////////////////////////////////////////////////
FT_BOOL FT_Font_Set_WidthGap (FT_Font *font, int gap);

/////////////////////////////////////////////////////////////////////////
/// \brief external width for glyph
/// \param[in] font ::A handle to a font
/// \return the external width
///////////////////////////////////////////////////////////////////////
int FT_Font_Get_WidthGap (FT_Font *font);

/////////////////////////////////////////////////////////////////////////
/// \brief external height for glyph
/// \param[in] font ::A handle to a font
/// \param[in] gap  ::The value of external height
/// \return 1 means true, 0 means error
///////////////////////////////////////////////////////////////////////
FT_BOOL FT_Font_Set_HeightGap (FT_Font *font, int gap);

/////////////////////////////////////////////////////////////////////////
/// \brief external height for glyph
/// \param[in] font ::A handle to a font
/// \return the external height
///////////////////////////////////////////////////////////////////////
int FT_Font_Get_HeightGap (FT_Font *font);

/////////////////////////////////////////////////////////////////////////
/// \brief set font horizontal resolution
/// \param[in] font ::A handle to a font
/// \param[in] dpix ::horizontal resolution value
/// \return 1 means success and 0 means error
///////////////////////////////////////////////////////////////////////
FT_BOOL FT_Font_Set_DpiX( FT_Font *font, int dpix );

/////////////////////////////////////////////////////////////////////////
/// \brief set font vertical resolution
/// \param[in] font ::A handle to a font
/// \param[in] dpiy ::vetical resolution value
/// \return 1 means success and 0 means error
///////////////////////////////////////////////////////////////////////
FT_BOOL FT_Font_Set_DpiY( FT_Font *font, int dpiy );

/////////////////////////////////////////////////////////////////////////
/// \brief set character size (physical unit)
/// \param[in] font ::A handle to a font
/// \param[in] ptx ::horizontal value
/// \param[in] pty ::vetical value
/// \return 1 means success and 0 means error
///////////////////////////////////////////////////////////////////////
FT_BOOL FT_Font_Set_Char_Size( FT_Font *font, FT_2DOT6 ptx, FT_2DOT6 pty );

/////////////////////////////////////////////////////////////////////////
/// \brief set characters size (pixel unit)
/// \param[in] font ::A handle to a font
/// \param[in] plx ::horizontal value
/// \param[in] ply ::vetical value
/// \return 1 means success and 0 means error
///////////////////////////////////////////////////////////////////////
FT_BOOL FT_Font_Set_Pixel_Size( FT_Font *font, int plx, int ply );

/////////////////////////////////////////////////////////////////////////
/// \brief set table size (default 3 space width)
/// \param[in] font ::A handle to a font
/// \param[in] width ::table size
/// \return void
///////////////////////////////////////////////////////////////////////
void FT_Font_Set_Table_Width( FT_Font *font, int width );

/////////////////////////////////////////////////////////////////////////
/// \brief set text color
/// \param[in] font ::A handle to a font
/// \param[in] color ::color value
/// \return 1 means success and 0 means error
///////////////////////////////////////////////////////////////////////
FT_BOOL FT_Font_Set_Text_Color( FT_Font *font, FT_COLOR  color );

/////////////////////////////////////////////////////////////////////////
/// \brief set shadow info, shadow depth from 1 to 16
/// \param[in] font ::A handle to a font
/// \param[in] shadow ::shadow info
/// \return 1 means success and 0 means error
///////////////////////////////////////////////////////////////////////
FT_BOOL FT_Font_Set_Shadow_Info( FT_Font *font, FT_ShadowInfo  shadow );

/////////////////////////////////////////////////////////////////////////
/// \brief set background color
/// \param[in] font ::A handle to a font
/// \param[in] color ::color value
/// \return 1 means success and 0 means error
///////////////////////////////////////////////////////////////////////
FT_BOOL FT_Font_Set_Back_Color( FT_Font *font, FT_COLOR  color );

/////////////////////////////////////////////////////////////////////////
/// \brief set edge color
/// \param[in] font ::A handle to a font
/// \param[in] color ::color value
/// \return 1 means success and 0 means error
///////////////////////////////////////////////////////////////////////
FT_BOOL FT_Font_Set_Edge_Color( FT_Font *font, FT_COLOR  color );

/////////////////////////////////////////////////////////////////////////
/// \brief set edge alpha
/// \param[in] font ::A handle to a font
/// \param[in] alpha ::alpha value
/// \return 1 means success and 0 means error
///////////////////////////////////////////////////////////////////////
FT_BOOL FT_Font_Set_Edge_Alpha( FT_Font *font, int alpha );

/////////////////////////////////////////////////////////////////////////
/// \brief set underline color
/// \param[in] font ::A handle to a font
/// \param[in] color ::color value
/// \return 1 means success and 0 means error
///////////////////////////////////////////////////////////////////////
FT_BOOL FT_Font_Set_Underline_Color( FT_Font *font, FT_COLOR  color );

/////////////////////////////////////////////////////////////////////////
/// \brief set font style
/// \param[in] font ::A handle to a font
/// \param[in] style ::style value
/// \return 1 means success and 0 means error
///////////////////////////////////////////////////////////////////////
FT_BOOL FT_Font_Set_Font_Style( FT_Font *font, int style );

/////////////////////////////////////////////////////////////////////////
/// \brief set smooth effect, nonsense for mono bitmap
/// \param[in] font ::A handle to a font
/// \param[in] bSmooth ::smooth value
/// \return 1 means success and 0 means error
///////////////////////////////////////////////////////////////////////
FT_BOOL FT_Font_Set_Smooth( FT_Font *font, FT_BOOL bSmooth );

/////////////////////////////////////////////////////////////////////////
/// \brief set background mode transparent, if set, the backcolor would be useless
/// \param[in] font ::A handle to a font
/// \param[in] bTransparent ::transparent value
/// \return 1 means success and 0 means error
///////////////////////////////////////////////////////////////////////
FT_BOOL FT_Font_Set_Back_Mode( FT_Font *font, FT_BOOL bTransparent );

/////////////////////////////////////////////////////////////////////////
/// \brief set antiTwinkle effect
/// \param[in] font ::A handle to a font
/// \param[in] bAntiTwinkle ::antiTwinkle value
/// \return 1 means success and 0 means error
///////////////////////////////////////////////////////////////////////
FT_BOOL FT_Font_Set_AntiTwinkle( FT_Font *font, FT_BOOL bAntiTwinkle );

/////////////////////////////////////////////////////////////////////////
/// \brief set gamma value, from 65536 to 131072, enhance the gray level
/// \brief the default value is 65536
/// \param[in] font ::A handle to a font
/// \param[in] gamma ::gamma value
/// \return 1 means success and 0 means error
///////////////////////////////////////////////////////////////////////
FT_BOOL FT_Font_Set_Gamma_Value( FT_Font *font, FT_Fixed gamma );

/////////////////////////////////////////////////////////////////////////
/// \brief set text order: right to left or left to right
/// \param[in] font ::A handle to a font
/// \param[in] bRightToLeft ::right to left value
/// \return 1 means success and 0 means error
///////////////////////////////////////////////////////////////////////
FT_BOOL FT_Font_Set_Text_Order( FT_Font *font, FT_BOOL bRightToLeft );

/////////////////////////////////////////////////////////////////////////
/// \brief set rotate angle in FT_Fixed format, if you want to rotate 45 
///        degree, you should set (45 << 6)
/// \param[in] font ::A handle to a font
/// \param[in] angle ::rotate angle
/// \return 1 means success and 0 means error
///////////////////////////////////////////////////////////////////////
FT_BOOL FT_Font_Set_Text_Angle( FT_Font *font, FT_Fixed angle );

/////////////////////////////////////////////////////////////////////////
/// \brief set blend monochrome format
/// \param[in] font ::A handle to a font
/// \param[in] blendMono ::blend monochrome when bitmap is gray format
/// \return void
///////////////////////////////////////////////////////////////////////
void FT_Font_Set_BlendMono( FT_Font* font, int blendMono );

/////////////////////////////////////////////////////////////////////////
/// \brief set fade out width
/// \param[in] font ::A handle to a font
/// \param[in] fadeoutWidth ::fade out width in pixels
/// \return void
///////////////////////////////////////////////////////////////////////
void FT_Font_Set_FadeoutWidth( FT_Font* font, int fadeoutWidth );

/////////////////////////////////////////////////////////////////////////
/// \brief set alpha blend, 
/// \param[in] font ::A handle to a font
/// \param[in] alphaBlend ::1 means support alpha blend, 0 means not support
/// \return void
///////////////////////////////////////////////////////////////////////
void FT_Font_Set_AlphaBlend( FT_Font* font, int alphaBlend );

/////////////////////////////////////////////////////////////////////////
/// \brief set a r g b bits value
/// \param[in] font ::A handle to a font
/// \param[in] a_bits ::alpha bits value
/// \param[in] r_bits ::red bits value
/// \param[in] g_bits ::green bits value
/// \param[in] b_bits ::blue bits value
/// \return void
///////////////////////////////////////////////////////////////////////
void FT_Font_Set_ARGB_Bits( FT_Font* font, int a_bits, int r_bits, int g_bits, int b_bits );

/////////////////////////////////////////////////////////////////////////
/// \brief get a r g b bits value
/// \param[in] font ::A handle to a font
/// \param[out] a_bits ::alpha bits value
/// \param[out] r_bits ::red bits value
/// \param[out] g_bits ::green bits value
/// \param[out] b_bits ::blue bits value
/// \return void
///////////////////////////////////////////////////////////////////////
void FT_Font_Get_ARGB_Bits( FT_Font* font, int* a_bits, int* r_bits, int* g_bits, int* b_bits );

/////////////////////////////////////////////////////////////////////////
/// \brief get blend monochrome format
/// \param[in] font ::A handle to a font
/// \return blendMono
///////////////////////////////////////////////////////////////////////
int FT_Font_Get_BlendMono( FT_Font* font);

/////////////////////////////////////////////////////////////////////////
/// \brief get alpha blend, 
/// \param[in] font ::A handle to a font
/// \return 1 means support alpha blend, 0 means not support
///////////////////////////////////////////////////////////////////////
int FT_Font_Get_AlphaBlend( FT_Font* font );

/////////////////////////////////////////////////////////////////////////
/// \brief get the value of the horizontal resolution
/// \param[in] font ::A handle to a font
/// \return the value of the horizontal resolution, 0 means error
///////////////////////////////////////////////////////////////////////
int FT_Font_Get_DpiX( FT_Font *font );

/////////////////////////////////////////////////////////////////////////
/// \brief get the value of the vertical resolution
/// \param[in] font ::A handle to a font
/// \return the value of the vertical resolution, 0 means error
///////////////////////////////////////////////////////////////////////
int FT_Font_Get_DpiY( FT_Font *font );

/////////////////////////////////////////////////////////////////////////                      
/// \brief get the FT_Face handle
/// \param[in] font ::A handle to a font
/// \return the FT_Face handle, NULL means error
///////////////////////////////////////////////////////////////////////                        
FT_Face FT_Font_Get_Face (FT_Font *font);   

/////////////////////////////////////////////////////////////////////////
/// \brief get the character size (physical unit)
/// \param[in] font ::A handle to a font
/// \param[out] ptx ::X value
/// \param[out] pty ::Y value
/// \return 1 means success, 0 means error
///////////////////////////////////////////////////////////////////////
FT_BOOL FT_Font_Get_Char_Size( FT_Font *font, FT_2DOT6 *ptx, FT_2DOT6 *pty );

/////////////////////////////////////////////////////////////////////////
/// \brief get the character size (pixel unit)
/// \param[in] font ::A handle to a font
/// \param[out] plx ::X value
/// \param[out] ply ::Y value
/// \return 1 means success, 0 means error
///////////////////////////////////////////////////////////////////////
FT_BOOL FT_Font_Get_Pixel_Size( FT_Font *font, int *plx, int *ply );

/////////////////////////////////////////////////////////////////////////
/// \brief get the table width (pixel unit, default 3 space width)
/// \param[in] font ::A handle to a font
/// \param[in] width ::Non-sense
/// \return the table width, 0 means error
///////////////////////////////////////////////////////////////////////
int FT_Font_Get_Table_Width( FT_Font *font, int width );

/////////////////////////////////////////////////////////////////////////
/// \brief get the font color
/// \param[in] font ::A handle to a font
/// \return the font color, 0 means error
///////////////////////////////////////////////////////////////////////
FT_COLOR FT_Font_Get_Text_Color( FT_Font *font );

/////////////////////////////////////////////////////////////////////////
/// \brief get the shadow info
/// \param[in] font ::A handle to a font
/// \return void
///////////////////////////////////////////////////////////////////////
void FT_Font_Get_Shadow_Info( FT_Font *font, FT_ShadowInfo* shadow );

/////////////////////////////////////////////////////////////////////////
/// \brief get the font background color
/// \param[in] font ::A handle to a font
/// \return the font background color, 0 means error
///////////////////////////////////////////////////////////////////////
FT_COLOR FT_Font_Get_Back_Color( FT_Font *font );

/////////////////////////////////////////////////////////////////////////
/// \brief get the font edge color
/// \param[in] font ::A handle to a font
/// \return the font edge color, 0 means error
///////////////////////////////////////////////////////////////////////
FT_COLOR FT_Font_Get_Edge_Color( FT_Font *font );

/////////////////////////////////////////////////////////////////////////
/// \brief get the font edge alpha
/// \param[in] font ::A handle to a font
/// \return the font edge alpha
///////////////////////////////////////////////////////////////////////
int FT_Font_Get_Edge_Alpha( FT_Font *font );

/////////////////////////////////////////////////////////////////////////
/// \brief get the font underline color
/// \param[in] font ::A handle to a font
/// \return the font underline color, 0 means error
///////////////////////////////////////////////////////////////////////
FT_COLOR FT_Font_Get_Underline_Color( FT_Font *font );

/////////////////////////////////////////////////////////////////////////
/// \brief get the font style
/// \param[in] font ::A handle to a font
/// \return the font style, 0 means error
///////////////////////////////////////////////////////////////////////
int FT_Font_Get_Font_Style( FT_Font *font );

/////////////////////////////////////////////////////////////////////////
/// \brief check if the font has smooth effect
/// \param[in] font ::A handle to a font
/// \return 1 means smooth, 0 means non-smooth
///////////////////////////////////////////////////////////////////////
FT_BOOL FT_Font_Get_Smooth( FT_Font *font );

/////////////////////////////////////////////////////////////////////////
/// \brief check if the font background transparent
/// \param[in] font ::A handle to a font
/// \return 1 means transparent, 0 means non-transparent
///////////////////////////////////////////////////////////////////////
FT_BOOL FT_Font_Get_Back_Mode( FT_Font *font );

/////////////////////////////////////////////////////////////////////////
/// \brief check if the font has the non-antitwinkle function
/// \param[in] font ::A handle to a font
/// \return 1 means non-antitwinkle, 0 means antitwinkle
///////////////////////////////////////////////////////////////////////
FT_BOOL FT_Font_Get_AntiTwinkle( FT_Font *font );

/////////////////////////////////////////////////////////////////////////
/// \brief get the gamma value
/// \param[in] font ::A handle to a font
/// \return the gamma value, 0 means error
///////////////////////////////////////////////////////////////////////
FT_Fixed FT_Font_Get_Gamma_Value( FT_Font *font );

/////////////////////////////////////////////////////////////////////////
/// \brief get the text order
/// \param[in] font ::A handle to a font
/// \return 1 means right to left, 0 means left to right
///////////////////////////////////////////////////////////////////////
FT_BOOL FT_Font_Get_Text_Order( FT_Font *font );

/////////////////////////////////////////////////////////////////////////
/// \brief get the rotate angle
/// \param[in] font ::A handle to a font
/// \return the rotate angle
///////////////////////////////////////////////////////////////////////
FT_Fixed FT_Font_Get_Text_Angle( FT_Font *font );

/////////////////////////////////////////////////////////////////////////
/// \brief get the width of the characters
/// \param[in] font ::A handle to a font
/// \param[in] wstr ::A handle to the characters
/// \param[in] len  ::The number of characters
/// \param[in] type ::Count text by 0:Grapheme others:Word
/// \return the value of character width
int FT_Font_Get_Text_Width( FT_Font *font, FT_WCHAR *wstr, int len, int type);

/////////////////////////////////////////////////////////////////////////
/// \brief get the width of the characters
/// \param[in] font      ::A handle to a font
/// \param[in] wstr      ::A handle to the characters
/// \param[in] len       ::The number of characters
/// \param[in] max_width ::The max width to contain the string
/// \param[in] by_word   ::if by_word == 0, not split the word when line break
/// \param[out] unichar_num ::The unichar number has been calculated
/// \return the value of character width
int FT_Font_Get_Text_Width_Variant( FT_Font *font, FT_WCHAR *wstr, 
								   int len, int max_width, int by_word,
								   int* unichar_num );

/////////////////////////////////////////////////////////////////////////
/// \brief get the width of the rectangle where the characters would be drawn
/// \param[in] font ::A handle to a font
/// \param[in] wstr ::A handle to characters
/// \param[in] len  ::The character lenth
/// \param[in] type ::Count text by 0:Grapheme others:Word
/// \return the value of character width
int FT_Font_Get_Text_Rect_Width( FT_Font *font, FT_WCHAR *wstr, int len, int type);

/////////////////////////////////////////////////////////////////////////
/// \brief get the height of the outputing text (pixel unit)
/// \param[in] font ::A handle to a font
/// \return the value of the text height(not include external height)
int FT_Font_Get_Text_Height( FT_Font *font );

/////////////////////////////////////////////////////////////////////////
/// \brief get the ascent of the font (pixel unit)
/// \param[in] font ::A handle to a font
/// \return the value of the font ascent
int FT_Font_Get_Ascent (FT_Font *font);

/////////////////////////////////////////////////////////////////////////
/// \brief get the descent of the font (pixel unit)
/// \param[in] font ::A handle to a font
/// \return the value of the font ascent
int FT_Font_Get_Descent (FT_Font *font);

/////////////////////////////////////////////////////////////////////////
/// \brief get the ascent of the font (pixel unit), not include the top space
/// \param[in] font ::A handle to a font
/// \return the value of the font ascent
int FT_Font_Get_Ascent2(FT_Font *font);

/////////////////////////////////////////////////////////////////////////
/// \brief get the descent of the font (pixel unit), not include the bottom space
/// \param[in] font ::A handle to a font
/// \return the value of the font ascent
int FT_Font_Get_Descent2(FT_Font *font);

//////////////////////////////////////////////////////////////////////////
/// \brief get the bounding box of a unichar
/// \param[in]font ::A handle to a font
/// \return the value of the bbox
FT_BBox FT_Font_Get_Char_BBox(FT_Font* font, FT_WCHAR unichar);

/////////////////////////////////////////////////////////////////////////
/// \brief get the max advance of the the font (pixel unit)
/// \param[in] font ::A handle to a font
/// \return the value of the font max advance
int FT_Font_Get_Max_Advance (FT_Font *font);

/////////////////////////////////////////////////////////////////////////
/// \brief get the line gap value
/// \param[in] font ::A handle to a font
/// \return the line gap value(not include external height)
///////////////////////////////////////////////////////////////////////
int FT_Font_Get_Text_LineGap( FT_Font *font );


/////////////////////////////////////////////////////////////////////////
/// \brief get the glyph bitmap of unichar
/// \param[in] font ::A handle to a font
/// \param[in] unichar ::A unicode char
/// \return the glyph bitmap slot
///////////////////////////////////////////////////////////////////////
FT_GlyphSlot FT_Font_Get_GlyphSlot(FT_Font* font, FT_WCHAR unichar);


/////////////////////////////////////////////////////////////////////////
/// \brief get the baseline
/// \param[in] font ::A handle to a font
/// \return the glyph baseline value
///////////////////////////////////////////////////////////////////////
int FT_Font_Get_BaseLine(FT_Font* font);


/////////////////////////////////////////////////////////////////////////
/// \brief get the unichar's ascent and descent
/// \param[in] font ::A handle to a font
/// \param[in] unichar ::A unichar char
/// \param[out] ascent ::The char's ascent
/// \param[out] descent ::The char's descent
/// \return void
///////////////////////////////////////////////////////////////////////
void FT_Font_Get_Char_Ascent_Descent(FT_Font* font, FT_WCHAR unichar, int* ascent, int* descent);


/////////////////////////////////////////////////////////////////////////
/// \brief draw only one line of horizontal texts to the desired bitmap
/// \param[in] font ::A handle to a font
/// \param[in] bmp  ::The handle to the bitmap
/// \param[in] x    ::The x offset of the bitmap
/// \param[in] y    ::The y offset of the bitmap
/// \param[in] wstr ::A handle to characters
/// \param[in] len  ::The character lenth
/// \return the real num of the drawing characters
int FT_Font_Text_Out( FT_Font *font, FT_BMP *bmp, int x, int y, FT_WCHAR *wstr, int len );

/////////////////////////////////////////////////////////////////////////
/// \brief draw only one line of horizontal texts to the desired bitmap,
///        just int 32 bit pixel for EGPS
/// \param[in] font ::A handle to a font
/// \param[in] bmp  ::The handle to the bitmap
/// \param[in] x    ::The x offset of the bitmap
/// \param[in] y    ::The y offset of the bitmap
/// \param[in] wstr ::A handle to characters
/// \param[in] len  ::The character lenth
/// \return the real number of the drawing characters
int FT_Font_Text_Out_EGPS32( FT_Font *font, FT_BMP *bmp, int x, int y, FT_WCHAR *wstr, int len );


#ifdef  SUPPORT_COMPLEX_LANGUAGE
/////////////////////////////////////////////////////////////////////////
/// \brief draw  multi-lines of horizontal texts to the desired bitmap
/// \param[in] font ::A handle to a font
/// \param[in] bmp  ::The handle to the bitmap
/// \param[in] x    ::The x offset of the bitmap
/// \param[in] y    ::The y offset of the bitmap
/// \param[in] wstr ::A handle to characters
/// \param[in] len  ::The character length
/// \return the real num of the drawing characters
int FT_Font_Draw_Text( FT_Font *font, FT_BMP *bmp, int x, int y, FT_WCHAR *wstr, int len );

void FT_Font_Set_Char_Style(FT_Font *font, int *charStyle);

void FT_Font_Set_Alignment( FT_Font *font, int horizontalAlignment, int verticalAlignment );

void FT_Font_Set_LineBreakMode( FT_Font *font, int lineBreakMode, int limitToNumberOfLines, int bAdjustFontSizeToFitWidth, int minimumFontSize );

// this function is written for tencent ios app 
FT_Bool FT_Font_Get_Fit_Size( FT_Font *font, int width, int maxHeight, FT_WCHAR *wstr, int len, int *fitWidth, int *fitHeight );

int  FT_Font_Draw_Text_In_Rect( FT_Font *font, FT_BMP *bmp, FT_WCHAR *wstr, int len );

typedef void* FT_BOUND;

/////////////////////////////////////////////////////////////////////////
/// \brief create a new bundary, used for complex language
/// \param[in] chars ::A handle to a characters needed to be splitted
/// \param[in] length ::The length of the characters
/// \param[in] bType ::The split unit: 0 means glyph unit, 1 means word unit
/// \return the font color, 0 means error
///////////////////////////////////////////////////////////////////////
FT_BOUND FT_Text_Boundary_New( const FT_WCHAR *chars, int length, int bType );
int FT_Text_Boundary_Next( FT_BOUND bound );
int FT_Text_Boundary_Prev( FT_BOUND bound );
void FT_Text_Boundary_Free( FT_BOUND bound );

int FT_Font_Get_Offset( FT_Font *font, FT_WCHAR *wstr, int len, int cursor);
int FT_Font_Get_Cursor( FT_Font *font, FT_WCHAR *wstr, int len, int offset);

int FT_Font_Prev_Cursor( FT_Font *font, FT_WCHAR *wstr, int len, int cursor);
int FT_Font_Next_Cursor( FT_Font *font, FT_WCHAR *wstr, int len, int cursor);

void FT_Font_Support_Multiple_Faces(int support);

//calculate text paragraph direction
int FT_Font_Get_Paragraph_Direction(FT_WCHAR* wstr, int len);

//calculate text line direction
int FT_Font_Get_Line_Direction(FT_WCHAR* wstr, int len);

void FT_Font_Set_Region_Color(FT_Font* font, FT_COLOR color);

#endif /* SUPPORT_COMPLEX_LANGUAGE */

#ifdef    __cplusplus
}
#endif
#endif /* _ETRUMP_H_ */

