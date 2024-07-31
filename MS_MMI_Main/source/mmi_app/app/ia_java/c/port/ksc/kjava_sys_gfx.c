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
#include "kjava_sys_gfx.h"
#include "guilcd.h"
#include "mmi_default.h"
#include "mmk_timer.h"
#include "mmijava_internal.h"
#include "guiimg.h"
#include "kjava_sys_gfx_font_intra.h"
#include "jblend_platform.h"
#include "guires.h"
#include "mmitheme_update.h"

#include "guistring.h"

#include "jas_gfx.h"
#include "jas_gfx_font.h"
#include "os_api.h"
#include "img_dec_interface.h"
#include "guiimg.h"


#include "guistring.h"
#ifdef KVM_ENABLE_GAMECANVAS_OPT
#include <jkvideo.h>    /*for jkoffscreenhandle*/
/*for JBLENDIA_GAMECANVAS_TIMEOUT_TIMER*/
#endif
#include "guiform.h"
#include "priority_app.h"
#include "jblend_config.h"
#ifdef JAVA_SUPPORT_SHRINK_RAM
#include "kjava_shrink_ram.h"
#endif

/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
 //#define JAVA_SUPPORT_FONT_CACHE
//#define KJAVA_USE_STATIC_ASCII_FONT

#define QUICKPAINT
#define QUICKPAINT_INTERVAL    50


#define MMIJAVA_TASK_PRORITY_LOW    PRI_MMIJAVA_TASK_LOW //SCI_PRIORITY_IDLE
#define MMIJAVA_TASK_PRORITY_HIGH   PRI_MMIJAVA_TASK_HIGH //(SCI_PRIORITY_LOWEST - 2)

/**--------------------------------------------------------------------------*
 **                         EXTERN DEFINITION                                 *
 **--------------------------------------------------------------------------*/
#ifdef JAVA_SUPPORT_MEDIA_CAMERA    
extern JKSint32 visialbe_id ;
#endif
extern BOOLEAN    g_jvm_hold_lcd;
extern BLOCK_ID   g_AKveId;

/**--------------------------------------------------------------------------*
 **                         STATIC DEFINITION                                *
 **--------------------------------------------------------------------------*/
LOCAL LCD_INFO_T LCDInfo;  //for lcd info local value
LOCAL KJavaPixel* kjava_virtualScreenPixels = PNULL;
LOCAL KJavaImage s_kjava_virtualScreenImage = {0};
LOCAL int32 screenWidth = 0;
LOCAL int32 screenHeight = 0;
LOCAL KJavaGfxInfo     s_kjavaGfxInfo = {0};
#ifdef QUICKPAINT
LOCAL uint32 s_pre_frame_tick = 0;
LOCAL uint32 s_cur_frame_tick = 0;
LOCAL uint8 s_timer_id = 0;
LOCAL BOOLEAN  is_jvm_hold_offscreen= FALSE;
#endif

uint16  src_width = 0;
uint16  src_height = 0;

#ifdef KJAVA_USE_STATIC_ASCII_FONT
#include "kjava_embedded_font.h"
#define MAX_KJAVA_STATIC_FONTS                     1
#define KJAVA_STATIC_FONT_ID 				0

static KJavaFont        kjavaFonts[MAX_KJAVA_STATIC_FONTS];
void kjava_drawfont( KJavaImage* dst, KJavaImage* src, KJavaClip* clip, int x, int y, unsigned short color);

#else

#if defined(JAVA_SUPPORT_FONT_CACHE)
#define KJAVA_SYS_GFX_DEFAULT_FONT_CHAR_NUM (128)
KJavaImage     dynamic_FontList[KJAVA_SYS_GFX_DEFAULT_FONT_CHAR_NUM] = {0};
KJavaPixel   font_image_cache[14*1024] = {0};//this is  for SONG_FONT_14 size by 精确计算cache_font_total_size == 9170 pixels
static BOOLEAN isDefaultFontCached = FALSE;

static void kjava_sys_gfx_loadCacheFont(void);
void kjava_drawfont( KJavaImage* dst, KJavaImage* src, KJavaClip* clip, int x, int y, unsigned short color);
#endif

/* For normal logic fonts. */
typedef struct KJavaFontItem
{
    KJavaFont *font;
    struct KJavaFontItem *next;
} KJavaFontItem;

#define KJAVA_FONT_TYPE_NUM  3
#define KJAVA_FONT_TYPE_IDX_DEFAULT     0
#define KJAVA_FONT_TYPE_IDX_STATIC_TEXT 1
#define KJAVA_FONT_TYPE_IDX_INPUT_TEXT  2
#ifndef JAVA_SUPPORT_SHRINK_RAM
static KJavaFont s_default_fonts[KJAVA_FONT_TYPE_NUM];
#else
DEFINE_STATIC_SHRINK_VAR_1_DIM(KJavaFont,s_default_fonts)
#endif
static KJavaFontItem *s_logic_fonts = NULL;
#endif

#ifdef KVM_ENABLE_GAMECANVAS_OPT
#define JBLENDIA_GAMECANVAS_TIMER_BASE_ID		250
#define JBLENDIA_GAMECANVAS_TIMEOUT_TIMER		(JBLENDIA_GAMECANVAS_TIMER_BASE_ID+1)

int g_Game_timer_Running = 0;
static int m_pendingFlush = 0;
static unsigned int m_timeCount = 0;
uint8  m_Gamecanvas_timer = SCI_NULL;
static BOOLEAN  m_Gamecanvas_timer_isCreate = SCI_FALSE;

#endif


#ifdef FS_PROFILE	
extern int  g_vm_exit ;
#endif

#ifdef JAVA_SUPPORT_SHRINK_RAM
#ifndef KJAVA_USE_STATIC_ASCII_FONT
BEGIN_DEFINE_SHRINK_VAR_INIT(kjava_sys_gfx)
	SHRINK_VAR_MEMORY_1_DIM(KJavaFont,s_default_fonts,KJAVA_FONT_TYPE_NUM)
END_DEFINE_SHRINK_VAR_INIT

BEGIN_DEFINE_SHRINK_VAR_FINI(kjava_sys_gfx)
	FREE_SHRINK_VAR_MEMORY(s_default_fonts)
END_DEFINE_SHRINK_VAR_FINI
#endif
#endif
/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/

#ifdef JAVA_SUPPORT_FONT_CACHE

//uint32 cache_font_total_size = 0;
 
static void kjava_sys_gfx_loadCacheFont(void)
{
	/*Cache 128 ascii char for KJAVA_FONT_TYPE_DEFAULT */
	
	uint16 i = 0;
	KJavaPixel* font_data ;

	wchar 		string[1] ={ 0 };
	uint 			font_width = 0;
	uint 			font_height = 0;
	uint 			font_data_size = 0;

	JAS_GFX_CLIP_S font_rect = {0};
	JAS_GFX_SCREEN_BUFF_S font_screen = {0};
	JAS_GFX_FONT_S font = {0};
	
	//SCI_TRACE_LOW:"kjava_sys_gfx_loadCacheFont enter isDefaultFontCached=%d"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,KJAVA_SYS_GFX_151_112_2_18_2_23_18_373,(uint8*)"d",isDefaultFontCached);

	if ( !isDefaultFontCached )
	{
		font_data = font_image_cache;
		jas_GfxGetDefaultFont(JAS_GFX_FONT_TYPE_DEFAULT,&font);
		
		for( i = 0;i < KJAVA_SYS_GFX_DEFAULT_FONT_CHAR_NUM; i++ )
		{
			switch(i)
			{
				//these ascii is null ,so set it null directly!
				case  9:
				case  10:
				case  13:
				case  28:
				case  29:
				case  30:
				case  31:
					dynamic_FontList[i].data = NULL;
					break;
				default:
				{
					string[0] = i;
					
				 	jas_GfxGetStringWidth(font.log_font_id,&string[0],1,&font_width);
					if(font_width ==0)
					{
						dynamic_FontList[i].data = NULL;
						break;
					}

					font_height = GUIFONT_GetHeight(MMI_DEFAULT_SMALL_FONT);
					if(font_height ==0)
					{
						dynamic_FontList[i].data = NULL;
						break;
					}		
					
					dynamic_FontList[i].width = font_width;
					dynamic_FontList[i].height = font_height;
					dynamic_FontList[i].transparency = 1;
					dynamic_FontList[i].isMutable = 1;
					dynamic_FontList[i].data = font_data;

					font_data_size = font_width * font_height;
					font_data = font_data + font_data_size;//font_data => next font_data ptr
					//cache_font_total_size += font_data_size;
					
					font_rect.x = 0;
					font_rect.y = 0;
					font_rect.w = font_width;
					font_rect.h = font_height;
					
					font_screen.data = (JAS_GFX_PIXEL *)dynamic_FontList[i].data;
					font_screen.width =font_width;
					font_screen.height =font_height;

					jas_GfxDrawString(&font_screen, font.log_font_id, (const wchar*)&string[0],1, 0, 0, &font_rect, (JAS_GFX_PIXEL)0xffff);
					break;
				}
			}
		}

		isDefaultFontCached = TRUE;
		//SCI_TRACE_LOW("kjava_sys_gfx_loadCacheFont cache_font_total_size=%d",cache_font_total_size); 
	}

}
#endif


//***********************************************************************************************
//  Description:jblendia_gfx_timer
//  Author:huangjc
//  Date:2007-09-03
//  Note:
//***********************************************************************************************
#ifdef KVM_ENABLE_GAMECANVAS_OPT

/*Adding for the JBlend GameCanvas repaint() optimization*/
#ifndef CONFIG_GAMECANVAS_DIRECT_SCREEN_FLUSH_PENDING_COUNT
#define CONFIG_GAMECANVAS_DIRECT_SCREEN_FLUSH_PENDING_COUNT  3
#endif

extern JKBool AmNotifyRepaintEvent(void);
extern signed int ajdx_drawImage(JKOffScreenHandle, const JKT_Rect*, JKSint32, JKSint32, KJavaImage* );

void JVM_UpdateGameCanvasScreenTimer(void * param)	
{
   //SCI_TRACE_LOW:"JVM_UpdateGameCanvasScreenTimer Enter"
   SCI_TRACE_ID(TRACE_TOOL_CONVERT,KJAVA_SYS_GFX_240_112_2_18_2_23_18_374,(uint8*)"");
    ++m_timeCount ;

    if ( m_pendingFlush )
    {
        //JB_LOG( JVM_UpdateGameCanvasScreenTimer, ( " m_pendingFlush = %d", m_pendingFlush ) );
        ++m_pendingFlush;

        if ( m_pendingFlush > ( CONFIG_GAMECANVAS_DIRECT_SCREEN_FLUSH_PENDING_COUNT + 1 ) )
        {
            m_pendingFlush = 0;
            if ( ajms_arc_getState() == AJSC_VMSTATE_STARTED )
            {
               // JB_LOG( JVM_UpdateGameCanvasScreenTimer, ( "invoke AmNotifyRepaintEvent" ) );
                AmNotifyRepaintEvent();  // Repaint the the midlet 
            }
        }
    }
   //SCI_TRACE_LOW:"JVM_UpdateGameCanvasScreenTimer AmNotifyRepaintEvent"
   SCI_TRACE_ID(TRACE_TOOL_CONVERT,KJAVA_SYS_GFX_258_112_2_18_2_23_18_375,(uint8*)"");
    if ( g_Game_timer_Running )
    {
	   MMIAPIJAVA_SendSignal(MSG_JAVA_PAINT,PNULL);    
    }
    //SCI_TRACE_LOW:"JVM_UpdateGameCanvasScreenTimer Leave"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,KJAVA_SYS_GFX_263_112_2_18_2_23_18_376,(uint8*)"");
    return ;
}
void JkVideoDrawOffScreenDirectly( JKOffScreenHandle hImage, const JKT_Rect* newClip, int* bPending )
{
    KJavaImage* pImage = ( KJavaImage* ) hImage;
    static unsigned int timerCount = 0;
    KJavaImage* pRealScreen = ( KJavaImage* ) kjava_gfx_getVirtualScreen();

    //    JB_LOG(JkVideoDrawOffScreenDirectly,(" enter timerCount = %d, m_timeCount = %d ", timerCount, m_timeCount));
    //SCI_TRACE_LOW:"JkVideoDrawOffScreenDirectly timerCount = %d, m_timeCount = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,KJAVA_SYS_GFX_273_112_2_18_2_23_18_377,(uint8*)"dd",timerCount, m_timeCount);

    if ( timerCount != m_timeCount )
    {
        timerCount = m_timeCount;  
        m_pendingFlush = 0;

        kjava_gfx_paintVirtualBegin();
        ajdx_drawImage( pRealScreen, newClip, newClip->x, newClip->y, pImage );
        kjava_gfx_paintVirtualEnd( 1, ( const KJavaClip * ) newClip );        
        *bPending = 0;
    }
    else
    {
        if ( m_pendingFlush == 0 )
            m_pendingFlush = 1 ;
        *bPending = 1 ;
    }
   //SCI_TRACE_LOW:"JkVideoDrawOffScreenDirectly Leave"
   SCI_TRACE_ID(TRACE_TOOL_CONVERT,KJAVA_SYS_GFX_291_112_2_18_2_23_18_378,(uint8*)"");
}   

int JkVideoIsDrawOffScreenDirectlyPending(void)
{
	 //SCI_TRACE_LOW:"JkVideoIsDrawOffScreenDirectlyPending Enter"
	 SCI_TRACE_ID(TRACE_TOOL_CONVERT,KJAVA_SYS_GFX_296_112_2_18_2_23_18_379,(uint8*)"");
    return  m_pendingFlush;
	 //SCI_TRACE_LOW:"JkVideoIsDrawOffScreenDirectlyPending Leave"
	 SCI_TRACE_ID(TRACE_TOOL_CONVERT,KJAVA_SYS_GFX_298_112_2_18_2_23_18_380,(uint8*)"");
}
/*End of Adding*/
#endif 

void jblendia_gfx_timer(void * param)
{
    
#ifdef QUICKPAINT   
    uint32           cur_tick = 0;
	//GUI_RECT_T       lcd_rect =MMITHEME_GetFullScreenRect();
    GUI_LCD_DEV_INFO dev_info = {0};
    AJ_S32           state = ajms_arc_getState() ;
    uint8*           source_ptr = PNULL;
    uint8*           dest_ptr = PNULL;
    int              copy_len = 0;
    GUI_RECT_T       clip_rect = {0};

    if(AJSC_VMSTATE_NONE == state ||!MMIDEFAULT_IsBacklightOn()||(s_timer_id != *((uint8 *) param)))//增加jvm状态的限制
    {
        //SCI_TRACE_LOW:"java is not running,just return"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,KJAVA_SYS_GFX_317_112_2_18_2_23_18_381,(uint8*)"");
        MMK_StopTimer(s_timer_id);
        s_timer_id = 0;
        return;
    }

    cur_tick= SCI_GetTickCount();
    
    if( (s_cur_frame_tick != s_pre_frame_tick) && ((cur_tick - s_cur_frame_tick) > QUICKPAINT_INTERVAL) && MMK_IsFocusWin(MMIJAVA_MIDLET_WIN_ID))
    {
        if(is_jvm_hold_offscreen)
        {
            MMIAPIJAVA_SendSignal(MSG_JAVA_PAINT,PNULL);
            return;
        }
        else
        {
            if ((PNULL != kjava_virtualScreenPixels) && MMK_IsFocusWin(MMIJAVA_MIDLET_WIN_ID))
            {
                clip_rect.right = screenWidth -1;
                clip_rect.bottom  = screenHeight -1;
                #if 0
        		LCD_CopyMemToMainLcdBufNoKeyColor(
        		kjava_virtualScreenPixels,
        		screenWidth,
        		screenHeight,
        		0,
        		0,
        		FALSE);
                #else
                source_ptr = (uint8*)kjava_virtualScreenPixels;
                dest_ptr =  LCDInfo.lcdbuff_ptr;
                copy_len = (screenHeight*LCDInfo.lcd_width) << 1;
                if(PNULL !=dest_ptr)
                {
                    SCI_MEMCPY(dest_ptr, source_ptr, copy_len);
                }
                #endif
        		MMITHEME_StoreUpdateRect(&dev_info, clip_rect);//不直接Invalidate_lcd，而是置脏，由app统一刷，防止先刷出java,在出现mmi的窗口
                //GUILCD_Invalidate(GUI_MAIN_LCD_ID,0);
            }
        }
        s_cur_frame_tick = cur_tick;
        s_pre_frame_tick = s_cur_frame_tick;
        s_timer_id = 0;
    }

#endif  
}

//***********************************************************************************************
//  Description:jblendia_gfx_timer
//  Author:huangjc
//  Date:2007-09-03
//  Note:
//***********************************************************************************************
PUBLIC void MMIAPIJAVA_CreateWinPaintTimer(void)
{
#ifdef KVM_ENABLE_GAMECANVAS_OPT	
    if(m_Gamecanvas_timer != 0)
   {
	    MMK_StopTimer(m_Gamecanvas_timer);
        m_Gamecanvas_timer = MMK_CreateWinTimer(MMIJAVA_MIDLET_WIN_ID, JBLENDIA_GAMECANVAS_TIMEOUT_TIMER, SCI_FALSE);
        //SCI_TRACE_LOW:"MMIAPIJAVA_CreateWinPaintTimer %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,KJAVA_SYS_GFX_380_112_2_18_2_23_19_382,(uint8*)"d", s_timer_id);
	 
   }
   else
   {
   	m_Gamecanvas_timer = MMK_CreateWinTimer(MMIJAVA_MIDLET_WIN_ID, JBLENDIA_GAMECANVAS_TIMEOUT_TIMER, SCI_FALSE);
   }	
#endif

#ifdef QUICKPAINT
    //JWE_LOG(MMIAPIJAVA_CreateWinPaintTimer, ("MMK_CreateWinTimer %d", s_timer_id));       
    if(s_timer_id != 0)
    {
        MMK_StopTimer(s_timer_id);
        s_timer_id = 0;
    }
    
    s_timer_id = MMK_CreateWinTimer(MMIJAVA_MIDLET_WIN_ID, QUICKPAINT_INTERVAL*2, SCI_FALSE);
#endif 
}

//***********************************************************************************************
//  Description:update screen
//  Author:huangjc
//  Date:2007-09-03
//  Note:
//***********************************************************************************************
PUBLIC void MMIJAVA_gfx_update(void)
{
	GUI_RECT_T       lcd_rect = MMITHEME_GetFullScreenRect();
    GUI_LCD_DEV_INFO dev_info = {0};
    AJ_S32           state = ajms_arc_getState() ;

     //SCI_TRACE_LOW:"MMIJAVA_gfx_update enter  s_is_display_logo=%d  s_is_still_display_logo=%d"
     SCI_TRACE_ID(TRACE_TOOL_CONVERT,KJAVA_SYS_GFX_413_112_2_18_2_23_19_383,(uint8*)"dd", MMIJAVA_IsDisplayLogo(), MMIJAVA_IsStillDisplayLogo());
	/*Java Logo*/
    if(AJSC_VMSTATE_NONE == state)//增加jvm状态的限制
    {
        //SCI_TRACE_LOW:"java is not running,just return"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,KJAVA_SYS_GFX_417_112_2_18_2_23_19_384,(uint8*)"");
        return;
    }
    
    if(MMIJAVA_IsDisplayLogo()||MMIJAVA_IsStillDisplayLogo())
    {
        {
            GUI_POINT_T  dis_point = {0,0};
            GUI_LCD_DEV_INFO dev_info_ptr = {0,0};   		    
            GUIRES_DisplayImg(&dis_point,
            PNULL,
            PNULL,
            MMIJAVA_MIDLET_WIN_ID,
            IA_JAVA_LOGO,
            &dev_info_ptr);	 
        }  
	    return;
    }

    
	/*Java Logo*/
    if (PNULL != kjava_virtualScreenPixels)
    {   
        GUILCD_SetLogicAngle(GUI_MAIN_LCD_ID,GUILCD_GetInitAngle(GUI_MAIN_LCD_ID));//避免get focus后，受到其他窗口的影响
		LCD_CopyMemToMainLcdBufNoKeyColor(
		kjava_virtualScreenPixels,
		screenWidth,
		screenHeight,
		0,
		0,
		FALSE);

		MMITHEME_StoreUpdateRect(&dev_info, lcd_rect);//不直接Invalidate_lcd，而是置脏，由app统一刷，防止先刷出java,在出现mmi的窗口
        //GUILCD_Invalidate(GUI_MAIN_LCD_ID,0);

    }
    
}

/*****************************************************************************/
//  Description : Initialize GFX component, needs to:
//  Global resource dependence :
//  Author: 
//  Note:
/*****************************************************************************/
void kjava_gfx_init(void)
{
    GUI_LCD_DEV_INFO    lcd_dev_info = {GUI_MAIN_LCD_ID, 0};
    BOOLEAN             error;
    uint16              width,height;
#ifndef KJAVA_USE_STATIC_ASCII_FONT
    int                 cnt;
    JAS_GFX_FONT_S      font;
#endif
    uint16              image_width = 0,image_height = 0;

    #ifdef VIRTUAL_KEY_BOARD
    GUIRES_GetImgWidthHeight(&image_width,&image_height, IMAGE_JAVA_KEYBOARD_BG,MMIJAVA_MIDLET_WIN_ID);
    //SCI_TRACE_LOW:"VIRTUAL_KEY_BOARD size = %d x %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,KJAVA_SYS_GFX_473_112_2_18_2_23_19_385,(uint8*)"dd", image_width, image_height);
    #endif
    
    GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID,&width,&height);
    
    screenWidth = width;
    screenHeight= height - image_height;
    //SCI_TRACE_LOW:"screen size = %d x %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,KJAVA_SYS_GFX_480_112_2_18_2_23_19_386,(uint8*)"dd", screenWidth, screenHeight);

    kjava_virtualScreenPixels = (KJavaPixel*)SCI_ALLOCA(sizeof(KJavaPixel) * (uint32)screenWidth * (uint32)screenHeight);

    if(PNULL == kjava_virtualScreenPixels)
    {
        //SCI_TRACE_LOW:"kjava_virtualScreenPixels alloc failed"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,KJAVA_SYS_GFX_486_112_2_18_2_23_19_387,(uint8*)"");
        return;
    }
    
    SCI_MEMSET(kjava_virtualScreenPixels, 0xff, (sizeof(KJavaPixel)* (uint32)screenWidth * (uint32)screenHeight));
    
    s_kjava_virtualScreenImage.data           = (KJavaPixel *)kjava_virtualScreenPixels;

    s_kjava_virtualScreenImage.width          = screenWidth;
    s_kjava_virtualScreenImage.height         = screenHeight;
    
    s_kjava_virtualScreenImage.transparency   = 0;
    s_kjava_virtualScreenImage.isMutable      = 1;
    
    // Seting initial java gfx parameter 
    s_kjavaGfxInfo.offscreen              = &s_kjava_virtualScreenImage;
    
    // Set the viewRegion and softKeyRegion 
    s_kjavaGfxInfo.viewRegion.x           = 0;
    s_kjavaGfxInfo.viewRegion.y           = 0;
    s_kjavaGfxInfo.viewRegion.width       = screenWidth;
    s_kjavaGfxInfo.viewRegion.height      = screenHeight - SPREADTRUM_LCD_SOFTKEY_HEIGHT;
    
    s_kjavaGfxInfo.softKeyRegion.x        = 0;
    s_kjavaGfxInfo.softKeyRegion.y        = screenHeight - SPREADTRUM_LCD_SOFTKEY_HEIGHT ;
    s_kjavaGfxInfo.softKeyRegion.width    = screenWidth;
    s_kjavaGfxInfo.softKeyRegion.height   = SPREADTRUM_LCD_SOFTKEY_HEIGHT;;
    
    s_kjavaGfxInfo.isColor                = 1;
    s_kjavaGfxInfo.numColors              = 65535;  

    s_kjavaGfxInfo.numAlphaLevels         = 256;
    
    s_kjavaGfxInfo.hasRepeatEvents        = 1;//change 0 to 1 for CETS
#ifdef TOUCHPANEL_TYPE
    s_kjavaGfxInfo.hasPointerEvents       = 1;
    s_kjavaGfxInfo.hasPointerMotionEvents = 1;
#else
    s_kjavaGfxInfo.hasPointerEvents       = 0;
    s_kjavaGfxInfo.hasPointerMotionEvents = 0;

#endif
    s_kjavaGfxInfo.wordwrap               = 1;
    s_kjavaGfxInfo.numSoftkeys            = 2;    

    
    error = GUILCD_GetInfo(MAIN_LCD_ID, &LCDInfo);

    LCDInfo.lcdbuff_ptr = (void*)UILAYER_GetLayerBufferPtr(&lcd_dev_info);

   // KJF_Init(screenWidth, screenHeight);   
#ifdef KJAVA_USE_STATIC_ASCII_FONT

    // Set default font 
    kjavaFonts[KJAVA_STATIC_FONT_ID].id            = KJAVA_STATIC_FONT_ID;
    kjavaFonts[KJAVA_STATIC_FONT_ID].face          = staticFace;
    kjavaFonts[KJAVA_STATIC_FONT_ID].style         = staticStyle;
    kjavaFonts[KJAVA_STATIC_FONT_ID].size          = staticSize;
    kjavaFonts[KJAVA_STATIC_FONT_ID].char_leading  = staticLeading;
    kjavaFonts[KJAVA_STATIC_FONT_ID].char_ascent   = staticAscent;
    kjavaFonts[KJAVA_STATIC_FONT_ID].char_descent  = staticDescent;
    kjavaFonts[KJAVA_STATIC_FONT_ID].char_height   = staticAscent+staticDescent;
#else


    /* Initialize jas font. */
    jas_GfxFontInit();

    /* Initialize default logic fonts. */
    for (cnt = 0; cnt < KJAVA_FONT_TYPE_NUM; cnt++)
    {
        SCI_MEMSET(&font, 0, sizeof(font));
        if (KJAVA_FONT_TYPE_IDX_DEFAULT == cnt)
        {
            jas_GfxGetDefaultFont(JAS_GFX_FONT_TYPE_DEFAULT, &font);
        }
        else if (KJAVA_FONT_TYPE_IDX_STATIC_TEXT == cnt)
        {
            jas_GfxGetDefaultFont(JAS_GFX_FONT_TYPE_STATIC_TEXT, &font);
        }
        else
        {
            jas_GfxGetDefaultFont(JAS_GFX_FONT_TYPE_INPUT_TEXT, &font);
        }
        s_default_fonts[cnt].id = font.log_font_id;
        s_default_fonts[cnt].size = font.size;
        s_default_fonts[cnt].style = font.style;
        s_default_fonts[cnt].face = font.face;
        s_default_fonts[cnt].char_ascent = font.ascent;
        s_default_fonts[cnt].char_descent = font.descent;
        s_default_fonts[cnt].char_height = font.height;
        s_default_fonts[cnt].char_leading = font.leading;
    }

#ifdef JAVA_SUPPORT_FONT_CACHE
	kjava_sys_gfx_loadCacheFont();
#endif
	
#endif

}

/*****************************************************************************/
//  Description : Finalize GFX component, need to release allocated resources 
//  Global resource dependence :
//  Author: 
//  Note:
/*****************************************************************************/
void kjava_gfx_fini(void)
{
#ifndef KJAVA_USE_STATIC_ASCII_FONT
	KJavaFontItem *cur_item;
	KJavaFontItem *del_item;
#endif

	//SCI_TRACE_LOW:"kjava_gfx_fini enter"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,KJAVA_SYS_GFX_599_112_2_18_2_23_19_388,(uint8*)"");

#ifndef KJAVA_USE_STATIC_ASCII_FONT
	cur_item = s_logic_fonts;
	while (cur_item != NULL)
	{
		del_item = cur_item;
		cur_item = cur_item->next;
        
		if(del_item->font!=PNULL)
        {
            SCI_FREE(del_item->font);
        }
		SCI_FREE(del_item);
	}
	s_logic_fonts = NULL;

#endif
    SCI_FREE(kjava_virtualScreenPixels);    
#ifdef  QUICKPAINT
    if(s_timer_id != 0)
    {
        MMK_StopTimer(s_timer_id);
        s_timer_id = 0;
    }
    
    s_pre_frame_tick = 0;
    s_cur_frame_tick = 0;
    is_jvm_hold_offscreen = FALSE;
#endif

}

/**
* \brief Find the suitable system font with the given criteria 
*        (size, face, and style).
*
* The function finds the suitable system font and return its font handle. Note
* that the function should use the last parameter for default font with the 
* this given criteria (size == 0, style == 0, and face == 0).
* 
* \param size      the size for the given criteria.
* \param style     the font style.
*
*                  One of the following is designated.
*                  <ul>
*                      <li>KJAVA_FONT_STYLE_PLAIN</li>
*                      <li>KJAVA_FONT_STYLE_BOLD</li>
*                      <li>KJAVA_FONT_STYLE_ITALIC</li>
*                      <li>KJAVA_FONT_STYLE_UNDERLINE</li>
*                  </ul>
*
* \param face      the font face. 
*                  
*                  One of the following is designated.
*                  <ul>
*                      <li>KJAVA_FONT_FACE_MONOSPACE</li>
*                      <li>KJAVA_FONT_FACE_PROPORTIONALE</li>
*                  </ul>
*
* \param defaultFont   the default font.
*                  
*                  One of the following is designated.
*                  <ul>
*                      <li>KJAVA_FONT_TYPE_DEFAULT</li>
*                      <li>KJAVA_TYPE_STATIC_TEXT</li>
*                      <li>KJAVA_TYPE_INPUT_TEXT</li>
*                  </ul>
*
* \return Return a handle identifying the designated font. If it's failed to
*         find suitable font, the function returns default system font at
*         least.
*/
/*****************************************************************************/
//  Description : Find the suitable system font with the given criteria 
//  Global resource dependence :
//  Author: 
//  Note:
/*****************************************************************************/
const KJavaFont* kjava_gfx_getSuitableFont(int size, int style, int face, int defaultFont)
{
#ifdef KJAVA_USE_STATIC_ASCII_FONT
	return &kjavaFonts[KJAVA_STATIC_FONT_ID];
#else    
//    return KJF_GetSuitableFont(face, style, size, defaultFont);

	KJavaFontItem *font_item = PNULL;
	JAS_GFX_FONT_S font = {0};
	
	//JWE_LOG(kjava_gfx_getSuitableFont, ("size=%d style=%d face=%d defaultFont=0x%x",size,style,face,defaultFont));     
	//SCI_TRACE_LOW:"kjava_gfx_getSuitableFont size=%d style=%d face=%d defaultFont=0x%x"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,KJAVA_SYS_GFX_686_112_2_18_2_23_19_389,(uint8*)"dddd",size,style,face,defaultFont);
	
	/* == Default font is required. == */
	if (0 == size && 0 == style && 0 == face)
	{
	    if (KJAVA_FONT_TYPE_INPUT_TEXT == defaultFont)
	    {
	        return (const KJavaFont *)
	            &s_default_fonts[KJAVA_FONT_TYPE_IDX_INPUT_TEXT];
	    }
	    else if (KJAVA_FONT_TYPE_STATIC_TEXT == defaultFont)
	    {
	        return (const KJavaFont *)
	            &s_default_fonts[KJAVA_FONT_TYPE_IDX_STATIC_TEXT];
	    }
	    else //KJAVA_FONT_TYPE_DEFAULT
	    {
	        size = KJAVA_FONT_SIZE_MEDIUM;
	    }
	}

	/* == Special font is required. by JkFontCreate == */

	/*
	 * Normal logic font is required. Search if it is already in the font list
	 * firstly. If absent, create a required font and append it to the list.
	 */
	font_item = s_logic_fonts;
	while (font_item != NULL)
	{
	    if (size == font_item->font->size &&
	        style == font_item->font->style &&
	        face == font_item->font->face)
	    {
	        return (const KJavaFont *)font_item->font;
	    }
	    font_item = font_item->next;
	}

	/* The required font is not found, create it then. */
	SCI_MEMSET(&font, 0, sizeof(font));
	switch(size)
	{
	case KJAVA_FONT_SIZE_SMALL:
	    font.size = JAS_GFX_FONT_SIZE_SMALL;
	    break;
	case KJAVA_FONT_SIZE_MEDIUM:
	    font.size = JAS_GFX_FONT_SIZE_MEDIUM;
	    break;
	case KJAVA_FONT_SIZE_LARGE:
	    font.size = JAS_GFX_FONT_SIZE_LARGE;
	    break;
	default:
	    font.size = JAS_GFX_FONT_SIZE_UNKNOWN;
	    break;
	}

	font.style = JAS_GFX_FONT_STYLE_PLAIN;//style是各种字体效果的组合
	
	if (0 != (style & KJAVA_FONT_STYLE_BOLD))
	{
	    font.style |= JAS_GFX_FONT_STYLE_BOLD;/*lint !e655*/
	}
	if (0 != (style & KJAVA_FONT_STYLE_ITALIC))
	{
	    font.style |= JAS_GFX_FONT_STYLE_ITALIC;/*lint !e655*/
	}
	if (0 != (style & KJAVA_FONT_STYLE_UNDERLINE))
	{
	    font.style |= JAS_GFX_FONT_STYLE_UNDERLINE;/*lint !e655*/
	}

	switch(face)
	{
		case KJAVA_FONT_FACE_DEFAULT:
		    font.face = JAS_GFX_FONT_FACE_DEFAULT;
		    break;
		case KJAVA_FONT_FACE_MONOSPACE:
		    font.face = JAS_GFX_FONT_FACE_MONOSPACE;
		    break;
		case KJAVA_FONT_FACE_PROPORTIONALE:
		    font.face = JAS_GFX_FONT_FACE_PROPORTIONAL;
		    break;
		default:
		    font.face = JAS_GFX_FONT_FACE_UNKNOWN;
		    break;
	}
	jas_GfxGetFont(&font);
	font_item = (KJavaFontItem *)SCI_ALLOC_APP(sizeof(KJavaFontItem));
		
	if (font_item != NULL)
	{
	    SCI_MEMSET(font_item, 0, sizeof(*font_item));
	    font_item->font = (KJavaFont *)SCI_ALLOC_APP(sizeof(KJavaFont));
	    if (NULL != font_item->font)
	    {
	        /* Fill all fields of the newly created font. */
	        SCI_MEMSET(font_item->font, 0, sizeof(*(font_item->font)));
	        font_item->font->size = size;
	        font_item->font->style = style;
	        font_item->font->face = face;
	        font_item->font->id = font.log_font_id;
	        font_item->font->char_ascent = font.ascent;
	        font_item->font->char_descent = font.descent;
	        font_item->font->char_height = font.height;
	        font_item->font->char_leading = font.leading;

	        /* Append the new font to logic font list. */
	        if (NULL == s_logic_fonts)
	        {
	            s_logic_fonts = font_item;
	        }
	        else
	        {
	            KJavaFontItem *p_item = s_logic_fonts;
	            while (NULL != p_item->next)
	            {
	                p_item = p_item->next;
	            }
	            p_item->next = font_item;
	        }
	        return (const KJavaFont *)font_item->font;
	    }
	    else
	    {
	        SCI_FREE(font_item);
	    }
	}


	/*
	 * Failed to create the required font for lack of memory,
	 * Return the default font then.
	 */

	return (const KJavaFont *)
	    &s_default_fonts[KJAVA_FONT_TYPE_IDX_DEFAULT];
#endif
}


/**
* \brief Get the width of given UCS2 string in the designated system font.
*        
* 
* \param font      the given font handle.
* \param string    the given UCS2 string.
* \param length    The number of characters in given UCS2 string
*
* \return Return the width of the UCS2 string in the designated font.
*/
int kjava_gfx_getCharsWidth (
                             const KJavaFont *font, const KJavaUnicode *str, int length)
{
	uint charsWidth = 0;
#if defined(KJAVA_USE_STATIC_ASCII_FONT) || defined(JAVA_SUPPORT_FONT_CACHE)
	int i;
#endif

	if(length == 0)
	    return 0;

#if defined(KJAVA_USE_STATIC_ASCII_FONT)

	for(i=0;i<length;i++) {
	    if(str[i]<128)
	        if(staticFontList[str[i]]!=0)
	            charsWidth += staticFontList[str[i]]->width;
	}
	
	return charsWidth;
	
#elif defined(JAVA_SUPPORT_FONT_CACHE)
	if ( ( font->style == s_default_fonts[KJAVA_FONT_TYPE_IDX_DEFAULT].style ) && ( font->size == s_default_fonts[KJAVA_FONT_TYPE_IDX_DEFAULT].size ) /*&& ( font->face == s_default_fonts[KJAVA_FONT_TYPE_IDX_DEFAULT].face )*/ )
	{
		for(i=0;i<length;i++)
		{   
			if(str[i]<KJAVA_SYS_GFX_DEFAULT_FONT_CHAR_NUM && dynamic_FontList[str[i]].data != 0)
			{
			     //charsWidth += staticFontList[str[i]]->width;
			     charsWidth += (uint)dynamic_FontList[str[i]].width;
			}		
			else
			{	
				uint charWidth = 0;
				jas_GfxGetStringWidth((uint)font->id, &str[i], 1,&charWidth);
				charsWidth += charWidth;
			}
		}

		return charsWidth;
	}
	else
	{
		jas_GfxGetStringWidth((uint)font->id, str, length,&charsWidth);
		return charsWidth;
	}

#else
	jas_GfxGetStringWidth((uint)font->id, str, length,&charsWidth);
	return charsWidth;
#endif

}

/**
* \brief Gets the number of characters that fit in the designated width.
*
* \param font      the given font handle.
* \param string    the given UCS2 string.
* \param length    The number of characters in given UCS2 string
* \param width     Designated with
*
* \return Return the number of characters. If the string designated 
*         in the str parameter contains a character not defined in the 
*         platform, treat it as a single blank space.
*/
int kjava_gfx_getCountChars (
                             const KJavaFont *font, const KJavaUnicode *str, int length, int width)
{
#if defined(KJAVA_USE_STATIC_ASCII_FONT) || defined(JAVA_SUPPORT_FONT_CACHE)
	int i;
        int charsWidth = 0;
        int charsCount = 0;
#endif
    //JWE_LOG(kjava_gfx_getCountChars,("[kjava_gfx_getCountChars] (font,str,length,width)(0x%08X,0x%08X,%d,%d)",font,str,length,width))
    //JWE_LOG(kjava_gfx_getCountChars,("[kjava_gfx_getCountChars] font ID = %d", font->id))      

       
  //  nWidth = KJF_GetCountChars(font, str, length, width);
#if defined(KJAVA_USE_STATIC_ASCII_FONT)

        
        for(i=0;i<length;i++) {
            if(str[i]<128) {
                if(staticFontList[str[i]]!=0)
                    charsWidth += staticFontList[str[i]]->width;
            }
            if(charsWidth > width)
                return charsCount;
            else
                charsCount++;
        }
        return (int)charsCount;

#elif defined(JAVA_SUPPORT_FONT_CACHE)
	if ( ( font->style == s_default_fonts[KJAVA_FONT_TYPE_IDX_DEFAULT].style ) && ( font->size == s_default_fonts[KJAVA_FONT_TYPE_IDX_DEFAULT].size ) /*&& ( font->face == s_default_fonts[KJAVA_FONT_TYPE_IDX_DEFAULT].face )*/ )
	{
		for(i=0;i<length;i++)
		{   
			if(str[i]<KJAVA_SYS_GFX_DEFAULT_FONT_CHAR_NUM && dynamic_FontList[str[i]].data != 0)
			{
			     //charsWidth += staticFontList[str[i]]->width;
			     charsWidth += dynamic_FontList[str[i]].width;
			}		
			else
			{	
				int charWidth = 0;
				jas_GfxGetStringWidth((uint)font->id, &str[i], 1,(uint *)&charWidth);
				charsWidth += charWidth;
			}

			if(charsWidth > width)
			    return charsCount;
			else
			    charsCount++;
		}

		return charsCount;
	}
	else
	{
		uint16 char_cnt = 0;
		jas_GfxGetCharCount((uint)font->id, str,length ,width, &char_cnt);
		return char_cnt;
	}


#else
	uint16 char_cnt = 0;
	jas_GfxGetCharCount((uint)font->id, str,length ,width, &char_cnt);
	return char_cnt;

#endif
}



void kjava_drawfont(
    KJavaImage* dst,
    KJavaImage* src,
    KJavaClip*  clip,
    int         x,
    int         y,
    KJavaPixel color
    )
{
    int x2, y2;
    int x_min,y_min,x_max,y_max;
    int w,h,src_x=0,src_y=0;
    int ifclip=0;

    KJavaPixel* dlptr;  //dst line ptr
    KJavaPixel* duptr;  //dst unit ptr
    KJavaPixel* duend;  //dst unit end
    unsigned short* slptr;  //src line ptr
    unsigned short* suptr;  //src unit ptr
    
#ifdef JAVA_SUPPORT_FONT_CACHE
    unsigned long src1;
    unsigned long dst1;
    unsigned long src2;
    unsigned long dst2;
#endif


    if (src == NULL) return;


    if(clip == NULL) {
        x_min = 0;
        y_min = 0;
        x_max = dst->width-1;
        y_max = dst->height-1;
    }
    else {
        x_min = clip->x;
        y_min = clip->y;
        x_max = clip->x + clip->width  - 1;
        y_max = clip->y + clip->height - 1;
        if (x_min < 0)  x_min = 0;
        if (y_min < 0)  y_min = 0;
        if (x_max >= dst->width )  x_max = dst->width  - 1;
        if (y_max >= dst->height)  y_max = dst->height - 1;
    }

    h = src->height;
    w = src->width;

    /* by [SC]: 03/15/2002, for image clipping */
    if (x > x_max ) return;
    if (x < x_min) {
        if ((x + w - 1) < x_min) return;
        if ((x + w - 1) > x_max) w = x_max - x_min +1;
        else w = x + w - x_min;
        src_x = x_min - x;
        x = x_min;
        ifclip = 1;
    }
    else if ((x + w - 1) > x_max) {
        w = x_max - x + 1;
        ifclip = 1;
    }

    if (y > y_max ) return;
    if (y < y_min) {
        if ((y + h - 1) < y_min) return;
        if ((y + h - 1) > y_max) h = y_max - y_min + 1;
        else h = y + h - y_min;
        src_y = y_min-y;
        y = y_min;
        ifclip=1;
    }
    else if ((y + h - 1) > y_max) {
        h = y_max - y + 1;
        ifclip =1;
    }

    x2 = x + w - 1;
    y2 = y + h - 1;


    // transparent image //
    dlptr = (KJavaPixel*)dst->data + y * dst->width + x;
    slptr = (unsigned short*)src->data + src_y * src->width + src_x;
    while (h-- > 0) {
        duptr = dlptr;
        duend = dlptr + w;
        suptr = slptr;
        dlptr += dst->width;
        slptr += src->width;
        while (duptr < duend) {
		if(0 != *suptr)
		{
		#ifdef JAVA_SUPPORT_FONT_CACHE
			src1 = *duptr;
			dst1 = color;
			src2=((src1<<16)|src1)&0x07E0F81F;
			dst2=((dst1<<16)|dst1)&0x07E0F81F;
			dst2=((((dst2-src2)*((*suptr)&0x001Fu))>>5)+src2)&0x07E0F81F;
			*duptr = (dst2>>16)|dst2;
		#else
			*duptr = color;
		#endif
		}
		
            duptr ++;
            suptr ++;
        }
    }

}


/**
* \brief Draw a character string into a given offscreen buffer with 
*        designated clipping rectangle, font, and pixel color.
*        
* \param offscreen the designated offscreen buffer.
* \param clip      the clipping range for drawing a string.
* \param font      the given font handle.
* \param string    the given UCS2 string.
* \param length    The number of characters in given UCS2 string.
* \param x         the x coordinate of the string in offscreen buffer.
* \param y         the y coordinate of the string in offscreen buffer.
* \param color     the color of the string. it should be the platform pixel
*                  format. (Maybe RGB: 565, 666, or 888)
*/

void kjava_gfx_drawChars (
                          KJavaImage      *offscreen, 
                          const KJavaClip *clip, 
                          const KJavaFont *font, 
                          const KJavaUnicode *string, int length, 
                          int x, int y, 
                          const KJavaPixel color)
{
/** 
* Advanced Features
*
* 1. Font character bitmap caching in KSI glue layer (kjava_sys_gfx.c) in 
*    runtime when Java is being initialized. Only for English characters 
*    because of JBenchmark.
* 2. ARM instructions have some bit operations, which can accelerate 
*    the interpreting of platform character bitmap structure.
    */
#if defined(KJAVA_USE_STATIC_ASCII_FONT) || defined(JAVA_SUPPORT_FONT_CACHE)
	int cur_x = x;
	int cur_y = y;
	int i;
#endif

#if !defined(KJAVA_USE_STATIC_ASCII_FONT)
	int x_min, y_min, x_max, y_max;
	int w, h;
	JAS_GFX_CLIP_S rect;
	JAS_GFX_SCREEN_BUFF_S screen;
#endif

	//SCI_TRACE_LOW("kjava_gfx_drawChars color=0x%x  font->id=%d font->face=%d font->style=%d font->size=%d font->char_leading=%d font->char_ascent=%d font->char_descent=%d font->char_height=%d\n",color, font->id,font->face,font->style,font->size,font->char_leading,font->char_ascent,font->char_descent,font->char_height);


	if(offscreen == NULL || clip == NULL || length == 0)
	{
		return;
	}

#if defined(KJAVA_USE_STATIC_ASCII_FONT)
	for(i=0; i<length; i++) {
	    if(string[i]<KJAVA_SYS_GFX_DEFAULT_FONT_CHAR_NUM) {
	        if(staticFontList[string[i]] != 0) {
	            kjava_drawfont(offscreen,staticFontList[string[i]],(KJavaClip*)clip, cur_x,cur_y,color);
	            cur_x += staticFontList[string[i]]->width;
	        }
	    }
	}	

#elif defined(JAVA_SUPPORT_FONT_CACHE)

	if(x >= offscreen->width || y >= offscreen->height ) //防止输入的坐标超出范围导致GUI_CreateRect assert
		return;

	if ( ( font->style == s_default_fonts[KJAVA_FONT_TYPE_IDX_DEFAULT].style ) && ( font->size == s_default_fonts[KJAVA_FONT_TYPE_IDX_DEFAULT].size ) /*&& ( font->face == s_default_fonts[KJAVA_FONT_TYPE_IDX_DEFAULT].face )*/ )
	{
		for(i=0;i<length;i++)
		{       
		#if 1
		    if(string[i]<KJAVA_SYS_GFX_DEFAULT_FONT_CHAR_NUM && dynamic_FontList[string[i]].data != 0)
		    {
		        //SCI_TRACE_LOW("kjava_gfx_drawChars draw cache font string[i]=%d\n",string[i]);
		        kjava_drawfont(offscreen,&(dynamic_FontList[string[i]]),(KJavaClip*)clip,cur_x,cur_y,color);
		        cur_x += dynamic_FontList[string[i]].width;
		    }
		#else
		    if(string[i]<KJAVA_SYS_GFX_DEFAULT_FONT_CHAR_NUM && staticFontList[string[i]] != 0)
		    {
		        kjava_drawfont(offscreen,staticFontList[string[i]],(KJavaClip*)clip,cur_x,cur_y,color);
		        cur_x += staticFontList[string[i]]->width;
		    }
		#endif
		    else
		    {
			/* Prepare the unicode string to be drawn. */
			uint charsWidth = 0;
			
			/* Calculate the rectangle for drawing. */
			x_min = (clip->x < 0 ? 0 : clip->x);
			y_min = (clip->y < 0 ? 0 : clip->y);
			x_max = ((clip->x + clip->width  - 1) >= offscreen->width ? (offscreen->width  - 1) : (clip->x + clip->width  - 1));
			x_max = (x_max < 0 ? 0 : x_max);
			y_max = ((clip->y + clip->height  - 1) >= offscreen->height ? (offscreen->height  - 1) : (clip->y + clip->height  - 1));
			y_max = (y_max < 0 ? 0 : y_max);
			w = (x_max > x_min ? (x_max - x_min + 1) : 0);
			h = (y_max > y_min ? (y_max - y_min + 1) : 0);

			if (0 == w || 0 == h || cur_x > x_max || cur_y > y_max)
			{
				//SCI_TRACE_LOW("kjava_gfx_drawChars (0 == w || 0 == h || x > x_max || y > y_max)");
				return;
			}

			rect.h = h;
			rect.w = w;
			rect.x = x_min;
			rect.y = y_min;

			screen.data = (JAS_GFX_PIXEL *)offscreen->data;
			screen.height = offscreen->height;
			screen.width = offscreen->width;

			/* Draw the text. */
			jas_GfxDrawString(&screen, (uint)font->id, (const wchar*)&string[i],1, cur_x, cur_y, &rect, (JAS_GFX_PIXEL)color);
			jas_GfxGetStringWidth((uint)font->id, &string[i], 1,&charsWidth);
			cur_x += (int)charsWidth; 
			
		    }
		}
	
	}
	else
	{
		/* Prepare the unicode string to be drawn. */

		/* Calculate the rectangle for drawing. */
		x_min = (clip->x < 0 ? 0 : clip->x);
		y_min = (clip->y < 0 ? 0 : clip->y);
		x_max = ((clip->x + clip->width  - 1) >= offscreen->width ? (offscreen->width  - 1) : (clip->x + clip->width  - 1));
		x_max = (x_max < 0 ? 0 : x_max);
		y_max = ((clip->y + clip->height  - 1) >= offscreen->height ? (offscreen->height  - 1) : (clip->y + clip->height  - 1));
		y_max = (y_max < 0 ? 0 : y_max);
		w = (x_max > x_min ? (x_max - x_min + 1) : 0);
		h = (y_max > y_min ? (y_max - y_min + 1) : 0);

		if (0 == w || 0 == h || x > x_max || y > y_max)
		{
			//SCI_TRACE_LOW("kjava_gfx_drawChars (0 == w || 0 == h || x > x_max || y > y_max)");
			return;
		}

		rect.h = h;
		rect.w = w;
		rect.x = x_min;
		rect.y = y_min;

		screen.data = (JAS_GFX_PIXEL *)offscreen->data;
		screen.height = offscreen->height;
		screen.width = offscreen->width;



		/* Draw the text. */
		jas_GfxDrawString(&screen, (uint)font->id, (const wchar*)string,length, x, y, &rect, (JAS_GFX_PIXEL)color);

	}

#else


	/* Prepare the unicode string to be drawn. */
	
	/* Calculate the rectangle for drawing. */
	x_min = (clip->x < 0 ? 0 : clip->x);
	y_min = (clip->y < 0 ? 0 : clip->y);
	x_max = ((clip->x + clip->width  - 1) >= offscreen->width ? (offscreen->width  - 1) : (clip->x + clip->width  - 1));
	x_max = (x_max < 0 ? 0 : x_max);
	y_max = ((clip->y + clip->height  - 1) >= offscreen->height ? (offscreen->height  - 1) : (clip->y + clip->height  - 1));
	y_max = (y_max < 0 ? 0 : y_max);
	w = (x_max > x_min ? (x_max - x_min + 1) : 0);
	h = (y_max > y_min ? (y_max - y_min + 1) : 0);

	if (0 == w || 0 == h || x > x_max || y > y_max)
	{
		//SCI_TRACE_LOW("kjava_gfx_drawChars (0 == w || 0 == h || x > x_max || y > y_max)");
		return;
	}

	rect.h = h;
	rect.w = w;
	rect.x = x_min;
	rect.y = y_min;

	screen.data = (JAS_GFX_PIXEL *)offscreen->data;
	screen.height = offscreen->height;
	screen.width = offscreen->width;

    /* Draw the text. */
	jas_GfxDrawString(&screen, (uint)font->id, (const wchar*)string,length, x, y, &rect, (JAS_GFX_PIXEL)color);

#endif



}

/**
* \brief Switches modes of the virtual screen size available to KJava.
*        When the mode switch succeeds, thereafter when setting device 
*        information for the graphics context to be obtained by 
*        kjava_gfx_getInfo(), be sure to reflect the mode change in the
*        set information.
*
* \param mode      the screen mode.
*
*                  One of the following is designated.
*                  <ul>
*                      <li>KJAVA_VIDEO_SCREEN_NORMAL</li>
*                      <li>KJAVA_VIDEO_SCREEN_FULL</li>
*                  </ul>
* \return Return 1 if the mode switch succeeded, or 0 if it failed.
*/
int kjava_gfx_requestScreenMode(int screenMode)
{
    
    //SCI_TRACE_LOW("kjava_gfx_requestScreenMode screenMode = (%d)", screenMode);
    switch(screenMode) 
    {    
    case KJAVA_VIDEO_SCREEN_NORMAL:
        
        /* Reset the viewRegion and softKeyRegion */
        s_kjavaGfxInfo.viewRegion.x           = 0;
        s_kjavaGfxInfo.viewRegion.y           = 0;
        s_kjavaGfxInfo.viewRegion.width       = screenWidth;
        s_kjavaGfxInfo.viewRegion.height      = screenHeight - SPREADTRUM_LCD_SOFTKEY_HEIGHT;
        
        s_kjavaGfxInfo.softKeyRegion.x        = 0;
        s_kjavaGfxInfo.softKeyRegion.y        = screenHeight - SPREADTRUM_LCD_SOFTKEY_HEIGHT ;
        s_kjavaGfxInfo.softKeyRegion.width    = screenWidth;
        s_kjavaGfxInfo.softKeyRegion.height   = SPREADTRUM_LCD_SOFTKEY_HEIGHT;;
        
        s_kjavaGfxInfo.numSoftkeys = 2;
        break;       

    default:
        
        /* Reset the viewRegion and softKeyRegion */
        s_kjavaGfxInfo.viewRegion.x           = 0;
        s_kjavaGfxInfo.viewRegion.y           = 0;
        s_kjavaGfxInfo.viewRegion.width       = screenWidth;
        s_kjavaGfxInfo.viewRegion.height      = screenHeight;
        
        s_kjavaGfxInfo.softKeyRegion.x        = 0;
        s_kjavaGfxInfo.softKeyRegion.y        = screenHeight ;
        s_kjavaGfxInfo.softKeyRegion.width    = 0;
        s_kjavaGfxInfo.softKeyRegion.height   = 0;;
        
        s_kjavaGfxInfo.numSoftkeys = 0;
        break;
        
    }

    return 1;
}

/**
* \brief Gets the video device properties.
*        
* \param info      The memory location for the video device information
*
*/
void kjava_gfx_getInfo(KJavaGfxInfo *info)
{
    //SCI_TRACE_LOW("[kjava_gfx_getInfo] enter.");
    
     /* Seting java gfx parameter */
    info->offscreen                 = s_kjavaGfxInfo.offscreen;
    
    SCI_MEMCPY(&info->viewRegion, &s_kjavaGfxInfo.viewRegion, sizeof(KJavaClip));
    SCI_MEMCPY(&info->softKeyRegion, &s_kjavaGfxInfo.softKeyRegion, sizeof(KJavaClip));
    
    info->isColor                   = s_kjavaGfxInfo.isColor;
    info->numColors                 = s_kjavaGfxInfo.numColors;
    info->numAlphaLevels            = s_kjavaGfxInfo.numAlphaLevels;
    info->hasRepeatEvents           = s_kjavaGfxInfo.hasRepeatEvents;
    info->hasPointerEvents          = s_kjavaGfxInfo.hasPointerEvents;
    info->hasPointerMotionEvents    = s_kjavaGfxInfo.hasPointerMotionEvents;
    info->wordwrap                  = s_kjavaGfxInfo.wordwrap;
    info->numSoftkeys               = s_kjavaGfxInfo.numSoftkeys;
    

}

/**
* \brief Get the virtual-screen for device. 
*
* The virtual screen is a graphics framebuffer, with the same size as 
* real screen.
*/
KJavaImage* kjava_gfx_getVirtualScreen(void)
{
    //SCI_TRACE_LOW("[kjava_gfx_getVirtualScreen] enter.");
        
    return s_kjavaGfxInfo.offscreen;
}

/**
* \brief Starts drawing on the virtual screen 
*/
void kjava_gfx_paintVirtualBegin(void)
{
#ifdef QUICKPAINT
    is_jvm_hold_offscreen = TRUE;
#endif
   JWE_LOG(kjava_gfx_paintVirtualBegin, ("enter"))
        
}

/**
* \brief Ends drawing on the virtual screen.
*
* \param flush     When this flag is 0, do not transfer the virtual screen 
*                  contents to the real screen.
*/

void kjava_gfx_paintVirtualEnd(int flush,const KJavaClip* clip)
{
    uint8*      source_ptr = PNULL;
    uint8*      dest_ptr = PNULL;
    int         copy_len = 0;
    GUI_RECT_T  clip_rect = {0};
    uint32      old_priority = 0;
	uint16      left = 0;
	uint16      top = 0;
	uint16      right = 0;
	uint16      bottom = 0;
    #ifdef VIRTUAL_KEY_BOARD
    BOOLEAN     if_fullscreen = FALSE;
    uint16      width,height;
    #endif
    
    //SCI_TRACE_LOW("kjava_gfx_paintVirtualEnd -- (x, y, w, h)=(%d, %d, %d, %d)",clip->x, clip->y, clip->width, clip->height);

	if(!MMIDEFAULT_IsBacklightOn()||!g_jvm_hold_lcd
     #ifdef JBLENDIA_SUPPORT_BACKGROUND_VM
     ||MMIAPIJAVA_IsJavaBackgroundRuning()
     #endif
     ) 
	{
	    #ifdef QUICKPAINT
        is_jvm_hold_offscreen = FALSE;
        #endif
		return;
	}
          
	if(MMIJAVA_IsDisplayLogo())
	{
        MMIJAVA_SetIsDisplayLogo(FALSE);
        #ifdef QUICKPAINT
        is_jvm_hold_offscreen = FALSE;
        #endif
        return;
	}
	else if(MMIJAVA_IsStillDisplayLogo())
	{
        MMIJAVA_SetIsStillDisplayLogo(FALSE);

        #ifdef VIRTUAL_KEY_BOARD
        if_fullscreen = TRUE;
        GUIFORM_SetVisible(MMIJAVA_KEYBOARD1_FORM,TRUE,TRUE);
        MMIJAVA_SetForm1ShowOrHide(TRUE);
        #endif
	}
    
	if(0 != flush)
	{
		if(clip->width == 0 || clip->height == 0) 
		{
			JWE_LOG(kjava_gfx_paintVirtualEnd, ("clip->width = %d, clip->height = %d",clip->width,clip->height));	
            
            #ifdef QUICKPAINT
            is_jvm_hold_offscreen = FALSE;
            #endif
            return;
		}
        
    	left = (clip->x < 0)? 0 : clip->x;
    	top = (clip->y < 0)? 0: clip->y ;
    	right = ((clip->x + clip->width-1)>= screenWidth)? (screenWidth-1) : (clip->x + clip->width-1);
    	bottom = ((clip->y + clip->height -1) >= screenWidth)? (screenHeight-1) : (clip->y + clip->height -1);         
        clip_rect.left = left;
        clip_rect.top = top;
        clip_rect.right = right;
        clip_rect.bottom = bottom ;

#ifdef QUICKPAINT

        #ifdef FS_PROFILE		
    	if(g_vm_exit <2)
    	{
    		kjava_profiler_startCount(4);	
    	}	
        #endif
    	s_cur_frame_tick = SCI_GetTickCount();
    	if(s_cur_frame_tick - s_pre_frame_tick < QUICKPAINT_INTERVAL) 
    	{
    		//SCI_TRACE_LOW("tick count < QUICKPAINT_INTERVAL");
    		MMIAPIJAVA_SendSignal(MSG_JAVA_PAINT,PNULL);            
            #ifdef FS_PROFILE		
    		if(g_vm_exit <2)
    		{
    			kjava_profiler_endCount(4);	
    		}	
            #endif	
            #ifdef QUICKPAINT
            is_jvm_hold_offscreen = FALSE;
            #endif

    		return;
    	}
    	else
    	{
    		s_pre_frame_tick = s_cur_frame_tick;
    	}

    	if ((PNULL != kjava_virtualScreenPixels) && MMK_IsFocusWin(MMIJAVA_MIDLET_WIN_ID))
    	{
#ifdef JAVA_SUPPORT_MEDIA_CAMERA    
            if(visialbe_id > 0)
            {
                LCD_FillRectTobuffer(kjava_virtualScreenPixels, screenWidth * screenHeight , 
                screenWidth,  screenHeight,clip_rect, MMI_BLACK_COLOR);  
            }
#endif
    	    #if 1
            SCI_ChangeThreadPriority(g_AKveId, MMIJAVA_TASK_PRORITY_HIGH, &old_priority);
    		source_ptr = (uint8*)kjava_virtualScreenPixels;
            source_ptr += (top*LCDInfo.lcd_width) << 1;
             
            dest_ptr =  LCDInfo.lcdbuff_ptr;
            if(PNULL==dest_ptr)
            {
                return;
            }
            dest_ptr += (top*LCDInfo.lcd_width) << 1;
            copy_len = ((clip_rect.bottom - clip_rect.top +1 )*LCDInfo.lcd_width) << 1;

            SCI_MEMCPY(dest_ptr, source_ptr, copy_len);
            
            #ifdef VIRTUAL_KEY_BOARD
            if(if_fullscreen)
            {
                GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID,&width,&height);
                clip_rect.right =  width - 1;
                clip_rect.bottom = height - 1;
            }
            #endif
        
        	GUILCD_InvalidateRect(
        		GUI_MAIN_LCD_ID, 
        		clip_rect,
        		0);	
            
            SCI_ChangeThreadPriority(g_AKveId, MMIJAVA_TASK_PRORITY_LOW, &old_priority);
            #else
    		LCD_CopyMemToMainLcdBufNoKeyColor(
    		kjava_virtualScreenPixels,
    		screenWidth,
    		screenHeight,
    		0,
    		0,
    		FALSE);
            #endif

        }
#else
		if ((PNULL != kjava_virtualScreenPixels) && MMK_IsFocusWin(MMIJAVA_MIDLET_WIN_ID))
		{
		    #if 1
            SCI_ChangeThreadPriority(g_AKveId, MMIJAVA_TASK_PRORITY_HIGH , &old_priority);
 		    source_ptr = (uint8*)kjava_virtualScreenPixels;
            source_ptr += (top*LCDInfo.lcd_width) << 1;
             
            dest_ptr =  LCDInfo.lcdbuff_ptr;
            if(PNULL==dest_ptr)
            {
                return;
            }
            dest_ptr += (top*LCDInfo.lcd_width) << 1;
            copy_len = ((clip_rect.bottom - clip_rect.top +1 )*LCDInfo.lcd_width) << 1;

            SCI_MEMCPY(dest_ptr, source_ptr, copy_len);
            clip_rect.left = left;
            clip_rect.top = top;
            clip_rect.right = right;
            clip_rect.bottom = bottom ;
            
            #ifdef VIRTUAL_KEY_BOARD
            if(if_fullscreen)
            {
                GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID,&width,&height);
                clip_rect.right =  width - 1;
                clip_rect.bottom = height - 1;
            }
            #endif
            
        	GUILCD_InvalidateRect(
        		GUI_MAIN_LCD_ID, 
        		clip_rect,
        		0);	
            SCI_ChangeThreadPriority(g_AKveId, MMIJAVA_TASK_PRORITY_LOW, &old_priority);
            #else
			LCD_CopyMemToMainLcdBufNoKeyColor(
			kjava_virtualScreenPixels,
			screenWidth,
			screenHeight,
			0,
			0,
			FALSE);
            #endif
		}
 #endif
	}
    
    
    #ifdef QUICKPAINT
    is_jvm_hold_offscreen = FALSE;
    #endif

#ifdef FS_PROFILE		
	if(g_vm_exit <2)
	{
		kjava_profiler_endCount(4);	
	}	
#endif
}


/**
* \brief Sets the type of the current Displayable displayed by the 
*        Java application.
*
* \param type      displayable type.
*
*                  One of the following is designated.
*                  <ul>
*                      <li>KJAVA_VIDEO_DISPLAYABLE_NULL</li>
*                      <li>KJAVA_VIDEO_DISPLAYABLE_SCREEN</li>
*                      <li>KJAVA_VIDEO_DISPLAYABLE_CANVAS</li>
*                      <li>KJAVA_VIDEO_DISPLAYABLE_PERMISSIONDIALOG</li>
*                  </ul>
*/
void kjava_gfx_setDisplayableType(int type)
{
    JWE_LOG(kjava_gfx_setDisplayableType, ("[kjava_gfx_setDisplayableType] (type) = (%d)",type));
#ifdef KVM_ENABLE_GAMECANVAS_OPT
    if ( g_Game_timer_Running )
    {
	 	 if(m_Gamecanvas_timer != 0)
  		{
			MMK_StopTimer(m_Gamecanvas_timer);
      			m_Gamecanvas_timer = 0;	
   		 }
        g_Game_timer_Running = 0;
    }
    m_pendingFlush = 0;

	if(type == 6)
	{
        //game canvas
		m_pendingFlush = 0;
		m_timeCount = 0;        
		g_Game_timer_Running = 1;
		MMIAPIJAVA_SendSignal(MSG_JAVA_PAINT,PNULL); 
	}
#endif
    return;
}


/**
*  \brief Ask the native system to decide if it could decode the given 
*         compressed image.
*
*  \param  srcBuf  pointer to compressed image of unknown format.
*  \param  srcSize size of srcBuf, in bytes.
*  \param  width   if srcBuf could be decoded by system, this variable would
*                  hold the width of the image.
*  \param  height  if srcBuf could be decoded by system, this variable would 
*                  hold the height of the image.
*
*  \return KJAVA_CODEC_SUCCESS if the image could be decoded by system, 
*          otherwise KJAVA_CODEC_UNSUPPORT.
*
*/
int kjava_gfx_imageAnalyze(const unsigned char* srcBuf, unsigned int srcSize, int* width, int* height)
{
    GUIIMG_INFO_T  img_info = {0};
    //SCI_TRACE_LOW:"[kjava_gfx_imageAnalyze] (srcBuf, srcSize, width, height) = (0x%08X, %d, %d, %d)"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,KJAVA_SYS_GFX_1671_112_2_18_2_23_21_390,(uint8*)"dddd", srcBuf, srcSize, width, height);

 #if defined(GFX_USE_EXTERNAL_JPEG_DECODER) || defined(GFX_USE_EXTERNAL_GIF_DECODER)|| defined(GFX_USE_EXTERNAL_PNG_DECODER)
    if(GUIIMG_GetImgInfo((uint8 *)srcBuf,srcSize,&img_info))
    {
        src_height = img_info.image_height;
        src_width = img_info.image_width;
        *height = src_height;
        *width = src_width;
        return KJAVA_CODEC_SUCCESS;
    }			
#endif
    return KJAVA_CODEC_UNSUPPORT;
}

static void RGB565toARGB(uint16* rgb, uint32 alpha, int length, uint32* argb)
{
    int32 i;
    uint32 RGB;
    for(i = 0; i < length; i++)
    {
        RGB = rgb[i];
        argb[i] = (alpha<<24) + ((RGB >> 11) << 19) + ((RGB >> 13) << 16) + 
            (((RGB << 21) >> 26) << 10) + (((RGB << 21) >> 30) << 8) + 
            ((RGB << 27) >> 24) + ((RGB << 27) >> 29) ;

        
	
    }
}


/*
in:rgbin565out5425 is 565 (16bit RRRRRGGGGGGBBBBB)
out: rgbin565out5425 is 5425 (16bit RRRRRGGGGAABBBBB)
*/
static void RGB565to5425(uint16* rgbin565out5425, uint8 * alpha, int32 length)
{
	int32 i = 0;	
	for (i = 0 ; i < length ; i++)
	{
		rgbin565out5425[i] = (alpha[i]&0x60) | (rgbin565out5425[i]&0xFF9F);
	}
}


/**
*        Decodes a compressed image.
* @param srcBuf   
*        A pointer to the buffer holding the compressed image to be decoded.
* @param srcSize
*        The size in bytes of the image data in the buffer indicated by <i>srcBuf</i>.
* @param argbData
*        A pointer to the memory area for putting the ARGB data of decoded source image
* @param dataLength
*        The size of the memory area for putting ARGB data. This value is width*height 
*        that return from kjava_gfx_imageAnalyze()
* @param processAlpha
*        Indicate Alpha channel used or not if drawing this image to any offscreen
*        0 - not used
*        1 - used
* @return 
*        Return one of the following values.
*        - #KJAVA_CODEC_SUCCESS  if the image was normally decoded.
*        - #KJAVA_CODEC_UNSUPPORT if the native decoder can't decode the source data format.
*        - #KJAVA_CODEC_BAD_FORMAT if the data contains bad format.
*        - #KJAVA_CODEC_NO_RESOURCE if enough resource could not be allocated to decode this image.
*        .
*
* Only the images checked by kjava_gfx_imageAnalyze() are passed to this function.
*/
int kjava_gfx_imageDecode( const unsigned char* srcBuf,
                           unsigned int srcSize, 
                           KJavaPixel* pixelData,
                           unsigned char* alphaData,
                           int dataLength,
                           int *processAlpha)
{
    //SCI_TRACE_LOW:"kjava_gfx_imageDecode,(srcBuf, srcSize, argbData, dataLength, processAlpha) = (0x%08X, %d, 0x%08X, %d, 0x%08X)"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,KJAVA_SYS_GFX_1747_112_2_18_2_23_21_391,(uint8*)"ddddd", srcBuf, srcSize,pixelData, dataLength, processAlpha);
    
 #if defined(GFX_USE_EXTERNAL_JPEG_DECODER) || defined(GFX_USE_EXTERNAL_GIF_DECODER) ||defined(GFX_USE_EXTERNAL_PNG_DECODER)    //所有图片格式
    {
        GUIIMG_DEC_OUT_T output_ptr = {0};
        GUIIMG_SRC_T src_ptr = {0};
        GUIIMG_DISPLAY_T display_ptr = {0};

        /*png*/    
        if(srcBuf[0] == 0x89 && srcBuf[1] == 0x50 && srcBuf[2] == 0x4E && srcBuf[3] == 0x47)
        {            
            //SCI_TRACE_LOW:"kjava_gfx_imageDecode type == png"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,KJAVA_SYS_GFX_1758_112_2_18_2_23_21_392,(uint8*)"");
            #ifdef SUPPORT_5425
            alphaData = (uint8*)SCI_ALLOCA(dataLength);
            if(PNULL == alphaData)
            {
                //SCI_TRACE_LOW:"kjava_gfx_imageDecode alphaData = NULL SCI_ALLOC_APP failed!!!!OutOfMemory!"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,KJAVA_SYS_GFX_1763_112_2_18_2_23_21_393,(uint8*)"");
                return KJAVA_CODEC_NO_RESOURCE;
            }             
            SCI_MEMSET(alphaData, 0,dataLength);            
            src_ptr.alpha_buf_ptr = alphaData;
            src_ptr.alpha_buf_size = dataLength;
            #endif
        }
        src_ptr.full_path_ptr = PNULL;
        src_ptr.is_file = FALSE;
        src_ptr.src_buf_ptr = (uint8 *)srcBuf;
        src_ptr.src_data_size = srcSize;

        display_ptr.crop_rect_ptr = PNULL;
        display_ptr.dest_height = src_height;
        display_ptr.dest_width = src_width;
        
        output_ptr.decode_data_ptr = pixelData;  /*lint !e64*/        
        output_ptr.decode_data_size  = dataLength*2;
        
        //SCI_TRACE_LOW:"kjava_gfx_imageDecode time start = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,KJAVA_SYS_GFX_1783_112_2_18_2_23_21_394,(uint8*)"d", SCI_GetTickCount());
        if( GUIIMG_Decode(&src_ptr,&display_ptr,&output_ptr))
        {
            //for png 
            if(srcBuf[0] == 0x89 && srcBuf[1] == 0x50 && srcBuf[2] == 0x4E && srcBuf[3] == 0x47)
            {
                #ifdef SUPPORT_5425
                *processAlpha = (int32)output_ptr.is_handle_alpha;
                if(output_ptr.is_handle_alpha)
                {
                    RGB565to5425(pixelData, alphaData, dataLength);                    
                    //SCI_TRACE_LOW:"kjava_gfx_imageDecode type == png, is_handle_alpha = %d"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,KJAVA_SYS_GFX_1794_112_2_18_2_23_21_395,(uint8*)"d", *processAlpha);
                }
                SCI_FREE(alphaData);   
                #endif        
            } 
            
            //SCI_TRACE_LOW:"kjava_gfx_imageDecode time end = %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,KJAVA_SYS_GFX_1800_112_2_18_2_23_21_396,(uint8*)"d", SCI_GetTickCount());
            return KJAVA_CODEC_SUCCESS;
        }
        else
        {            
            //SCI_TRACE_LOW:"kjava_gfx_imageDecode failed"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,KJAVA_SYS_GFX_1805_112_2_18_2_23_21_397,(uint8*)"");
        }
    }
#endif 
    return  KJAVA_CODEC_UNSUPPORT;
}
#endif
