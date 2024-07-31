/*
 *
 * Copyright  1990-2009 Sun Microsystems, Inc. All Rights Reserved.
 * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER
 * 
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License version
 * 2 only, as published by the Free Software Foundation.
 * 
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License version 2 for more details (a copy is
 * included at /legal/license.txt).
 * 
 * You should have received a copy of the GNU General Public License
 * version 2 along with this work; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA
 * 02110-1301 USA
 * 
 * Please contact Sun Microsystems, Inc., 4150 Network Circle, Santa
 * Clara, CA 95054 or visit www.sun.com if you need additional
 * information or have any questions.
 */

#include "os_api.h"
#include "guilcd.h"
#include "mmi_position.h"
#include "mmijava_main.h"
#include "mmijava_video.h"
//#include "mmijava_id.h"

#include "javacall_lcd.h"
#include "javacall_lifecycle.h"
#include "javacall_platform_defs.h"
#include "javacall_logging.h"
#include "mmi_appmsg.h"
#include "javacall_memory.h"
#include "mmidisplay_data.h"
#include "mmijava_dc.h"
#define TRANSPARENT 0x1
#define TOPBARHIGHT MMI_STATUSBAR_HEIGHT

//javacall_bool VideoPlayerHasLCDControl;

static LCD_INFO_T LCDInfo;
LOCAL javacall_bool s_fullScreenMode = JAVACALL_TRUE;
LOCAL int s_flushStartLine = TOPBARHIGHT;
//LOCAL javacall_bool s_lcdFirstFlush = JAVACALL_TRUE;
#ifdef USE_PCSL_MEM_FOR_LCD
LOCAL javacall_pixel* s_lcdBuffer = NULL;
#endif

LOCAL javacall_bool s_backgroundAllowed = JAVACALL_FALSE;
LOCAL javacall_bool s_cmccKeepBackground = JAVACALL_FALSE;
#ifdef __cplusplus
extern "C" {
#endif
int javacall_lcd_get_flushStartLine() {
    return s_flushStartLine;
}
    
/**
 * The function javacall_lcd_init is called by during Java VM startup, allowing the
 * platform to perform device specific initializations. The function is required to
 * provide the supported screen capabilities:
 * - Display Width
 * - Display Height
 * - Color encoding: Either 32bit ARGB format, 15 bit 565 encoding or 24 bit RGB encoding
 * 
 * \par
 * 
 * Once this API call is invoked, the VM will receive display focus.\n
 * <b>Note:</b> Consider the case where the platform tries to assume control over the
 * display while the VM is running by pausing the Java platform. In this case, the
 * platform is required to save the VRAM screen buffer: Whenever the Java
 * platform is resumed, the stored screen buffers must be restored to original
 * state.
 * 
 * @param screenWidth width of screen
 * @param screenHeight width of screen
 * @param colorEncoding color encoding, one of the following:
 *              -# JAVACALL_LCD_COLOR_RGB565
 *              -# JAVACALL_LCD_COLOR_ARGB
 *              -# JAVACALL_LCD_COLOR_RGB888   
 *              -# JAVACALL_LCD_COLOR_OTHER    
 *                
 * @retval JAVACALL_OK      success
 * @retval JAVACALL_FAIL    fail
 */
javacall_result javacall_lcd_init(void) {
    GUI_LCD_DEV_INFO lcd_dev_info = {GUI_MAIN_LCD_ID, 0};

    s_fullScreenMode = JAVACALL_TRUE;
    s_flushStartLine = TOPBARHIGHT;
#ifdef USE_PCSL_MEM_FOR_LCD
    if (s_lcdBuffer != NULL) {
        javacall_print("Java> LCD init already done\n");
    } else {
    
         GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID,&LCDInfo.lcd_width, &LCDInfo.lcd_height);   

#if 1
        LCDInfo.lcdbuff_ptr = (void*)UILAYER_GetLayerBufferPtr(&lcd_dev_info);
#endif
        


        s_lcdBuffer = (javacall_pixel*)pcsl_mem_malloc_javacall(LCDInfo.lcd_width*(LCDInfo.lcd_height + TOPBARHIGHT)*sizeof(javacall_pixel));
        if (NULL==s_lcdBuffer) {
            javacall_print("Java> LCD buffer fail - OOM\n");
            return JAVACALL_FAIL;
        }
    }
#endif
    return JAVACALL_OK;
}


/**
 * The function javacall_lcd_finalize is called by during Java VM shutdown, 
 * allowing the  * platform to perform device specific lcd-related shutdown
 * operations.  
 * The VM guarantees not to call other lcd functions before calling 
 * javacall_lcd_init( ) again.
 *                
 * @retval JAVACALL_OK      success
 * @retval JAVACALL_FAIL    fail
 */
javacall_result javacall_lcd_finalize(void){
    //s_lcdFirstFlush = JAVACALL_TRUE;
    s_fullScreenMode = JAVACALL_FALSE;
    s_flushStartLine = TOPBARHIGHT;
#ifdef USE_PCSL_MEM_FOR_LCD
    if (NULL==s_lcdBuffer) {
        javacall_printf("Java> LCD finalize already done\n");
    } else {
        pcsl_mem_free_javacall(s_lcdBuffer);
        s_lcdBuffer = NULL;
    }
#endif
	return JAVACALL_OK;
} 
    
/**
 * Get screen raster pointer
 *
 * @return pointer to video ram mapped memory region of size  
 *         ( LCDSGetScreenWidth() * LCDSGetScreenHeight() )  
 */
extern BOOLEAN g_vmHoldLcd;/*lint !e526*/
  javacall_pixel* javacall_lcd_get_screen(int hardwareId,
                                        int* screenWidth,
                                        int* screenHeight,
                                        javacall_lcd_color_encoding_type* colorEncoding){

#ifndef USE_PCSL_MEM_FOR_LCD

#if 1
    LCDInfo.lcdbuff_ptr = (void*)UILAYER_GetLayerBufferPtr(&lcd_dev_info);
#endif
    /*javacall_printf("Call LCD_GetInfo with LCD id %x, returns %x, width %d, height %d, bits %d, buffer %x",
        LCDID, error, LCDInfo.lcd_width, LCDInfo.lcd_height, LCDInfo.bits_per_pixel, LCDInfo.lcdbuff_ptr);*/
  
#endif
    
    if (NULL != screenWidth) {
        *screenWidth = LCDInfo.lcd_width;
    }
    
    if (NULL != screenHeight) {
        *screenHeight = LCDInfo.lcd_height - s_flushStartLine;
    }
    
    if (NULL != colorEncoding) {
        *colorEncoding = JAVACALL_LCD_COLOR_RGB565;
    }
    
#ifdef USE_PCSL_MEM_FOR_LCD
    return (javacall_pixel* )s_lcdBuffer;
#else
    return  (javacall_pixel*)LCDInfo.lcdbuff_ptr + LCDInfo.lcd_width * s_flushStartLine;
#endif
}
    
/**
 * The following function is used to flush the image from the Video RAM raster to
 * the LCD display. \n
 * The function call should not be CPU time expensive, and should return
 * immediately. It should avoid memory bulk memory copying of the entire raster.
 *
 * @retval JAVACALL_OK      success
 * @retval JAVACALL_FAIL    fail
 */
javacall_result javacall_lcd_flush(int hardwareId) {
    return javacall_lcd_flush_partial(hardwareId, 0, LCDInfo.lcd_height - s_flushStartLine);
}

/**
 * Set or unset full screen mode.
 * 
 * This function should return <code>JAVACALL_FAIL</code> if full screen mode
 * is not supported.
 * Subsequent calls to <code>javacall_lcd_get_screen()</code> will return
 * a pointer to the relevant offscreen pixel buffer of the corresponding screen
 * mode as well s the corresponding screen dimensions, after the screen mode has
 * changed.
 * 
 * @param useFullScreen if <code>JAVACALL_TRUE</code>, turn on full screen mode.
 *                      if <code>JAVACALL_FALSE</code>, use normal screen mode.

 * @retval JAVACALL_OK   success
 * @retval JAVACALL_FAIL failure
 */
javacall_result javacall_lcd_set_full_screen_mode(int hardwareId, javacall_bool useFullScreen) {
    javacall_bool statusBarStatusChanged;
    
    if (FALSE == g_vmHoldLcd) { 
        return JAVACALL_OK;
    }
    
    statusBarStatusChanged = s_fullScreenMode == useFullScreen? JAVACALL_FALSE : JAVACALL_TRUE;
    s_fullScreenMode = useFullScreen;
    if (s_fullScreenMode == JAVACALL_TRUE) {
        s_flushStartLine = 0;
        if (statusBarStatusChanged == JAVACALL_TRUE) {
            MMIJAVA_SendJVMMessage2MMI(APP_JAVA_STATUSBAR_OPERATION_IND, DESTROY_STATUS_BAR);
        }
    } else {
        s_flushStartLine = TOPBARHIGHT;
        if (statusBarStatusChanged == JAVACALL_TRUE) {
            MMIJAVA_SendJVMMessage2MMI(APP_JAVA_STATUSBAR_OPERATION_IND, CREATE_STATUS_BAR);
        }
    }
    return JAVACALL_OK;
}

javacall_bool javacall_lcd_is_full_screen_mode(int hardwareId) {
    return s_fullScreenMode;
}
   
/**
 * Flush the screen raster to the display. 
 * This function should not be CPU intensive and should not perform bulk memory
 * copy operations.
 * The following API uses partial flushing of the VRAM, thus may reduce the
 * runtime of the expensive flush operation: It should be implemented on
 * platforms that support it
 * 
 * @param ystart start vertical scan line to start from
 * @param yend last vertical scan line to refresh
 *
 * @retval JAVACALL_OK      success
 * @retval JAVACALL_FAIL    fail 
 */
javacall_result javacall_lcd_flush_partial(int hardwareId, int ystart, int yend){
#ifdef USE_PCSL_MEM_FOR_LCD
    uint8 *raster8, *bmp8;
    int cpy_len;
    int parth;
#endif
    GUI_RECT_T rect = {0};
    
//    javacall_printf(LOG_STR("s_flushStartLine =%d, ystart=%d,yend=%d"),s_flushStartLine, ystart,yend);
//    if(MMIJAVA_Is_Paused()) 
//    {
        //avoid circle wait to dead lock
//        SCI_Sleep(1);
//    }

    if (FALSE == g_vmHoldLcd) {
        return JAVACALL_OK;
    }
    
    
//    Create the status bar when VM start
//    if ((JAVACALL_TRUE == s_lcdFirstFlush) && (JAVACALL_FALSE == s_fullScreenMode)) 
//    if (JAVACALL_FALSE == s_fullScreenMode) 
//    {
//        MMIJAVA_SendJVMMessage2MMI(APP_JAVA_STATUSBAR_OPERATION_IND, CREATE_STATUS_BAR);
//        s_lcdFirstFlush = JAVACALL_FALSE;
//    }
    
    if (yend <= 0) {
        return JAVACALL_OK;
    }
    
    if (0 > ystart) {
        ystart = 0;
    }
    
    if (LCDInfo.lcd_height < s_flushStartLine + yend) {
        yend = LCDInfo.lcd_height - s_flushStartLine;
    }
    
    if (ystart >= yend) {
        return JAVACALL_OK;
    }
    //vp
    if(mmijava_IsVPRunning())
    {  
       mmijava_GetVPLocation(&rect);    
       LCD_FillRectTobuffer(s_lcdBuffer, LCDInfo.lcd_height * LCDInfo.lcd_width , 
        LCDInfo.lcd_width,  LCDInfo.lcd_height,  rect, MMI_BLACK_COLOR);
        hardwareId = -1;
    }
    //dc
    if(MMIJAVA_CAMERA_SNAPSHOT_WITH_PREVIEW == mmijava_dc_get_state()
        || MMIJAVA_CAMERA_DC_PREVIEWING == mmijava_dc_get_state())
        {
            mmijava_dc_get_rect(&rect);
            LCD_FillRectTobuffer(s_lcdBuffer, LCDInfo.lcd_height * LCDInfo.lcd_width , 
             LCDInfo.lcd_width,  LCDInfo.lcd_height,  rect, MMI_BLACK_COLOR);     
             
             hardwareId = -1;
        }

//upgrade jvm task priority ,same as app task, to avoid being interrupted by app task
if(-1 != hardwareId)        //called in app task ,dont change jvm task prioritys
{
    MMIJAVA_UpThreadPriority();    
}

#ifdef USE_PCSL_MEM_FOR_LCD
    bmp8 = (uint8*)s_lcdBuffer;
    bmp8 += (ystart*LCDInfo.lcd_width) << 1;
    
    raster8 =  LCDInfo.lcdbuff_ptr;
    raster8 += ((s_flushStartLine+ystart)*LCDInfo.lcd_width) << 1;
    
    parth = yend - ystart;
    cpy_len = (parth*LCDInfo.lcd_width) << 1;

    SCI_MEMCPY(raster8, bmp8, cpy_len);
#endif
    
    rect.left = 0;
    rect.top = (uint16)ystart + s_flushStartLine; 
    rect.right = LCDInfo.lcd_width - 1;
    rect.bottom=(uint16)yend + s_flushStartLine - 1;	

	GUILCD_InvalidateRect(
		GUI_MAIN_LCD_ID, 
		rect,
		0);	
            
//    MMIAPICOM_OtherTaskToMMI( MMI_JAVA_WIN_ID, MSG_JAVA_LCD_FLUSH, &rect, sizeof(GUI_RECT_T) );

    //finishig memory copying, degrade jvm task
    if(-1 != hardwareId)    //called in app task ,dont change jvm task priority
    {
        MMIJAVA_DownThreadPriority();
    }
    return JAVACALL_OK;
}
    
javacall_bool javacall_direct_flush(void *src, int height) {
    //javacall_pixel backupLcd[320*240];
    //int LcdLen = LCDInfo.lcd_width*LCDInfo.lcd_height*sizeof(javacall_pixel);
    //backupLcd = javacall_malloc(LCDInfo.lcd_width*LCDInfo.lcd_height*sizeof(javacall_pixel));

    /*if (backupLcd == NULL) {
        javacall_printf("[javacall_direct_flush] >> OOM, can not allocate memory for backup lcd\n");
        return JAVACALL_FALSE;
    }*/

    if (FALSE == g_vmHoldLcd) {
        return JAVACALL_TRUE;
    }
    
    if (height > LCDInfo.lcd_height - s_flushStartLine) {
        
        height = LCDInfo.lcd_height - s_flushStartLine;
        javacall_printf(LOG_STR("height = %d, startline = %d"), height, s_flushStartLine);
    }

    //SCI_MEMCPY(backupLcd, LCDInfo.lcdbuff_ptr, LcdLen);
#ifdef USE_PCSL_MEM_FOR_LCD
    SCI_MEMCPY(s_lcdBuffer, src, LCDInfo.lcd_width * height * sizeof(javacall_pixel));/*lint !e737 */
#else
    SCI_MEMCPY((void *)((javacall_pixel*)LCDInfo.lcdbuff_ptr + LCDInfo.lcd_width * s_flushStartLine), src, LCDInfo.lcd_width * height * sizeof(javacall_pixel));/*lint !e737 */
#endif
    javacall_lcd_flush_partial(0, 0, height);

    //SCI_MEMCPY(LCDInfo.lcdbuff_ptr, backupLcd, LcdLen);
    //javacall_free(backupLcd);
    return JAVACALL_TRUE;
}

javacall_bool javacall_lcd_reverse_orientation(int hardwareId) {
    (void)hardwareId;
    return JAVACALL_FALSE;
}

void javacall_lcd_handle_clamshell() {
    //Not implemented
}
 
javacall_bool javacall_lcd_get_reverse_orientation(int hardwareId) {
    (void)hardwareId;
    return JAVACALL_FALSE;
}

/**
 * checks the implementation supports native softbutton layer.
 * 
 * @retval JAVACALL_TRUE   implementation supports native softbutton layer
 * @retval JAVACALL_FALSE  implementation does not support native softbutton layer
 */
javacall_bool javacall_lcd_is_native_softbutton_layer_supported () {
    return JAVACALL_FALSE;
}

/**
 * The following function is used to set the softbutton label in the native
 * soft button layer.
 * 
 * @param label the label for the softbutton
 * @param len the length of the label
 * @param index the corresponding index of the command
 * 
 * @retval JAVACALL_OK      success
 * @retval JAVACALL_FAIL    fail
 */
javacall_result javacall_lcd_set_native_softbutton_label(const javacall_utf16* label,
                                                         int len,
                                                         int index){
     return JAVACALL_FAIL;
}

int javacall_lcd_get_screen_width(int hardwareId) {
    (void)hardwareId;
    return LCDInfo.lcd_width;
}
 
int javacall_lcd_get_screen_height(int hardwareId) {
    (void)hardwareId;
    return LCDInfo.lcd_height - s_flushStartLine;
}

int javacall_lcd_get_full_screen_height(int hardwareId) {
    (void)hardwareId;
    return LCDInfo.lcd_height;
}

/**
 * get currently enabled hardware display id
 */
int javacall_lcd_get_current_hardwareId() {
    return 0;  
}


/** 
 * Get display device name by id
 * @param hardwareId unique id of hardware screen
 */
char* javacall_lcd_get_display_name(int hardwareId) {
  (void)hardwareId;
  return "prim";
}


/**
 * Check if the display device is primary
 * @param hardwareId unique id of hardware screen
 */
javacall_bool javacall_lcd_is_display_primary(int hardwareId) {
    (void)hardwareId;
    return JAVACALL_TRUE;
}

/**
 * Check if the display device is build-in
 * @param hardwareId unique id of hardware screen
 */
javacall_bool javacall_lcd_is_display_buildin(int hardwareId) {
    (void)hardwareId; 
    return JAVACALL_TRUE;
}

/**
 * Check if the display device supports pointer events
 * @param hardwareId unique id of hardware screen
 */
javacall_bool javacall_lcd_is_display_pen_supported(int hardwareId) {
    (void)hardwareId; 
    return JAVACALL_TRUE;
}

/**
 * Check if the display device supports pointer motion  events
 * @param hardwareId unique id of hardware screen
 */
javacall_bool javacall_lcd_is_display_pen_motion_supported(int hardwareId){
    (void)hardwareId; 
    return JAVACALL_TRUE;
}

/**
 * Get display device capabilities
 * @param hardwareId unique id of hardware screen
 */
int javacall_lcd_get_display_capabilities(int hardwareId) {
    return 0xFF;
}


/**
 * Get the list of screen ids
 * @param return number of screens 
 * @return the lit of ids 
 */
int* javacall_lcd_get_display_device_ids(int* n) {
    static int ids[]={0};
    *n = 1;
    return &ids[0];
}

void javacall_lcd_display_in_background(javacall_bool background) {
    //should not call mmi function in java task
    //MMIJAVA_RunFromBackground(background);   
    if(JAVACALL_FALSE == background)
    {
        javacall_lcd_flush(0);
    }
}

void javacall_lcd_display_set_background_allowed(javacall_bool background) {
    //s_backgroundAllowed = background;
    // allow all application to run at background
#ifdef MEMORY_CUT
	s_backgroundAllowed = JAVACALL_FALSE;
#else
    s_backgroundAllowed = JAVACALL_TRUE;
#endif    
}

javacall_bool javacall_lcd_get_background_allowed() {
    return s_backgroundAllowed;
}
/* pass in true when CMCC-Keep-Background = yes */
void javacall_lcd_display_set_cmcc_keep_background(javacall_bool keepBackground) {
    s_cmccKeepBackground = keepBackground;
}

javacall_bool javacall_lcd_get_cmcc_keep_background() {
    return s_cmccKeepBackground;
}

int java_lcd_GetYStartFromLCD(void)
{
   return s_flushStartLine; 
}    

#ifdef __cplusplus
} //extern "C"
#endif


