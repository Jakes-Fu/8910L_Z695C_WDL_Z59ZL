/******************************************************************************
 ** File Name:      lcd_tv.c                                                  *
 ** Author:         Hanbing.Zhang                                             *
 ** DATE:           29/08/2007                                                *
 ** Copyright:      2004 Spreadtrum, Incorporated. All Rights Reserved.       *
 ** Description:    This file defines the basic operation of lcd tv           *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME                 DESCRIPTION                           *
 ** 08/29/2007      Hanbing.Zhang        Create.                              *
 ******************************************************************************/

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "lcd_tv.h"
#include "dtl_cmdq.h"
#include "cmd_common.h"
#include "os_api.h"
#include "mtv_boot.h"
#include "gpio_drv.h"
#include "lcm_cfg.h"
#include "kd.h"
#include "mtv_ctrl.h"

/**--------------------------------------------------------------------------*
 **                         EXTERNAL DEFINITION                              *
 **--------------------------------------------------------------------------*/
/**---------------------------------------------------------------------------*
 **                         Macro Definition                                   *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                      Function Declaration                                 *
 **---------------------------------------------------------------------------*/

/************************************************************************
* \fn  KDboolean LCDTV_IsOpen(void)
* \param  void
* \return KD_TRUE: Open
* \brief  tv open that means lcd controled by seine
*
* \author Hanbing.Zhang
* \note   Nothing.  
************************************************************************/
KDboolean LCDTV_IsOpen(void)
{
    return 0;
}


/************************************************************************
* \fn  KDuint32 LCDTV_Init(void)
* \param  void
* \return KDuint32
* \brief  lcd init when tv begin to open
*
* \author Hanbing.Zhang
* \note   Nothing.  
************************************************************************/
KDuint32 LCDTV_Init(void)
{
    return 0;
}

/************************************************************************
* \fn  KDuint32 LCDTV_InvalidateRect(
*                                  KDuint32 lcd_id,//ID of lcd to operate
*                                  KDuint16 left,     //left of the rectangle
*                                  KDuint16 top,     //top of the rectangle
*                                  KDuint16 right,     //right of the rectangle
*                                  KDuint16 bottom    //bottom of the rectangle.
*                                  )
* \param  lcd_id  LCD ID
* \param  left    left of the rectangle
* \param  top     top of the rectangle
* \param  right   right of the rectangle
* \param  bottom  bottom of the rectangle
* \return KDuint32
* \brief  invalid rectangle
*
* \author Hanbing.Zhang
* \note   Nothing.  
************************************************************************/
KDuint32 LCDTV_InvalidateRect(
                            KDuint32 lcd_id,//ID of lcd to operate
                            KDuint16 left,     //left of the rectangle
                            KDuint16 top,     //top of the rectangle
                            KDuint16 right,     //right of the rectangle
                            KDuint16 bottom    //bottom of the rectangle.
                            )
{
    return 0;
}

/************************************************************************
* \fn  KDuint32 LCDTV_InvalidateRect_Image(
*                                           KDuint32 lcd_id,//ID of lcd to operate
*                                           KDuint16 left,     //left of the rectangle
*                                           KDuint16 top,     //top of the rectangle
*                                           KDuint16 right,     //right of the rectangle
*                                           KDuint16 bottom,    //bottom of the rectangle
*                                           KDuint16 *buf_ptr	 //buffer pointer of the image to display
*                                           )
* \param  lcd_id  LCD ID
* \param  left    left of the rectangle
* \param  top     top of the rectangle
* \param  right   right of the rectangle
* \param  bottom  bottom of the rectangle
* \param  buf_ptr buffer pointer
* \return KDuint32
* \brief  invalid rectangle
*
* \author Hanbing.Zhang
* \note   Nothing.  
************************************************************************/
KDuint32 LCDTV_InvalidateRect_Image(
                                  KDuint32 lcd_id,//ID of lcd to operate
                                  KDuint16 left,     //left of the rectangle
                                  KDuint16 top,     //top of the rectangle
                                  KDuint16 right,     //right of the rectangle
                                  KDuint16 bottom,    //bottom of the rectangle
                                  KDuint16 *buf_ptr	 //buffer pointer of the image to display
                                  )
{
    return 0;
}

/************************************************************************
* \fn  KDuint32 LCDTV_Invalidate(
*                               KDuint32 lcd_id    //ID of lcd to operate
*                               )
* \param  lcd_id LCD ID 
* \return KDuint32
* \brief  invalid lcd
*
* \author Hanbing.Zhang
* \note   Nothing
************************************************************************/
KDuint32 LCDTV_Invalidate(
                        KDuint32 lcd_id    //ID of lcd to operate
                        )
{
    return 0;
}

/************************************************************************
* \fn  KDuint32 LCDTV_SetContrast(
*                                 KDuint32 lcd_id,    //ID of lcd to operate.
*                                 KDuint16  contrast    //contrast value to set.
*                                )
* \param  lcd_id LCD ID 
* \param  contrast
* \return void  
* \brief  set contrast
*
* \author Hanbing.Zhang
* \note   Nothing
************************************************************************/
KDuint32 LCDTV_SetContrast(
                         KDuint32 lcd_id,    //ID of lcd to operate.
                         KDuint16  contrast    //contrast value to set.
                         )
{
    return 0;
}

/************************************************************************
* \fn  KDuint32 LCDTV_SetBrightness(
*                                    KDuint32 lcd_id,    //ID of lcd to operate.
*                                    KDuint16 brightness    //brightness value to set.
*                                    )
* \param  lcd_id LCD ID 
* \param  brightness
* \return void  
* \brief  set brightness
*
* \author Hanbing.Zhang
* \note   Nothing
************************************************************************/
KDuint32 LCDTV_SetBrightness(
                           KDuint32 lcd_id,    //ID of lcd to operate.
                           KDuint16 brightness    //brightness value to set.
                           )
{
    return 0;
}

/************************************************************************
* \fn KDuint32  LCDTV_SetDisplayWindow(
*                                        KDuint32 lcd_id,//ID of lcd to operate
*                                        KDuint16 left,     //left of the display window
*                                        KDuint16 top,     //top of the display window
*                                        KDuint16 right,     //right of the display window
*                                        KDuint16 bottom    //bottom of the display window
*                                        )
* \param  lcd_id  LCD ID
* \param  left    left of the display window
* \param  top     top of the display window
* \param  right   right of the display window
* \param  bottom  bottom of the display window
* \return KDuint32
* \brief  set display window
*
* \author Hanbing.Zhang
* \note   Nothing.  
************************************************************************/
KDuint32  LCDTV_SetDisplayWindow(
                               KDuint32 lcd_id,//ID of lcd to operate
                               KDuint16 left,     //left of the display window
                               KDuint16 top,     //top of the display window
                               KDuint16 right,     //right of the display window
                               KDuint16 bottom    //bottom of the display window
                               )
{
    return 0;
}


/************************************************************************
* \fn  KDuint32  LCDTV_SetDirection(
*                                    KDuint32 lcd_id,//ID of lcd to operate
*                                    KDuint8 is_invert
*                                    )
* \param  lcd_id LCD ID 
* \param  is_invert
* \return void  
* \brief  set direction
*
* \author Hanbing.Zhang
* \note   Nothing
************************************************************************/
KDuint32  LCDTV_SetDirection(
                           KDuint32 lcd_id,//ID of lcd to operate
                           KDuint8 is_invert
                           )
{
    return 0;
}

/************************************************************************
* \fn  void LCDTV_Clear(
*                     KDuint32 lcd_id,     //id of lcd to operate.
*                     KDuint32 color        //color to fill the whole lcd.
*                     )
* \param  lcd_id LCD ID 
* \param  color
* \return void  
* \brief  clear lcd by color
*
* \author Hanbing.Zhang
* \note   Nothing
************************************************************************/
void LCDTV_Clear(
                 KDuint32 lcd_id,     //id of lcd to operate.
                 KDuint32 color        //color to fill the whole lcd.
                 )
{

}
/************************************************************************
* \fn  void LCDTV_SetVideoDisplayRect(
*                                   KDuint16 video_left,
*                                   KDuint16 video_top,
*                                   KDuint16 video_right,
*                                   KDuint16 video_bottom
*                                   )
* \param  video_left 
* \param  video_top
* \param  video_right 
* \param  video_bottom
* \return void
* \brief  set video display rect
*
* \author Hanbing.Zhang
* \note   Nothing
************************************************************************/
void LCDTV_SetVideoDisplayRect(
                               KDuint16 video_left,
                               KDuint16 video_top,
                               KDuint16 video_right,
                               KDuint16 video_bottom
                               )
{
}

/************************************************************************
* \fn void LCDTV_SetAlphaandColorKey(
*                                   LCDTV_LAYER_ORDER_E layer_order,
*                                   KDuint8              alpha,
*                                   KDboolean            alpha_enable,
*                                   KDuint16             colorkey_max,
*                                   KDuint16             colorkey_min,
*                                   KDboolean            colorkey_enable
*                                   )
* \param  layer_order 
* \param  alpha 
* \param  alpha_enable 
* \param  color_key_max 
* \param  color_key_min
* \param  colorkey_enable 
* \return void
* \brief  set alpha and colorkey
*
* \author Hanbing.Zhang
* \note   Nothing
************************************************************************/
void LCDTV_SetAlphaandColorKey(
                               LCDTV_LAYER_ORDER_E layer_order,
                               KDuint8              alpha,
                               KDboolean            alpha_enable,
                               KDuint16             colorkey_max,
                               KDuint16             colorkey_min,
                               KDboolean            colorkey_enable
                               )
{
}

/************************************************************************
* \fn  void LCDTV_SetLCDSpec(
                             LCDTV_SPEC_T *spec_ptr
                             )
* \param  spec_ptr lcd spec pointer
* \return void  
* \brief  set lcd spec
*         
*
* \author Hanbing.Zhang
* \note   Nothing
************************************************************************/
void LCDTV_SetLCDSpec(
                      LCDTV_SPEC_T *spec_ptr
                      )
{

}

/************************************************************************
* \fn  void LCDTV_EnableLCDSpec(
*                               KDboolean is_enable
*                               )
* \param  is_enable
* \return void  
* \brief  enable lcd spec
*         
*
* \author Hanbing.Zhang
* \note   Nothing
************************************************************************/
void LCDTV_EnableLCDSpec(
                         KDboolean is_enable
                         )
{
 
}

/************************************************************************
* \fn  void LCDTV_SetVideoSpec(
*                            LCDTV_VIDEO_SPEC_T *v_spec_ptr
*                            )
* \param  v_spec_ptr video layer spec pointer
* \return void  
* \brief  set video layer spec
*         
*
* \author Hanbing.Zhang
* \note   Nothing
************************************************************************/
void LCDTV_SetVideoSpec(
                        LCDTV_VIDEO_SPEC_T *v_spec_ptr
                        )
{
}
/************************************************************************
* \fn  void LCDTV_SetOSDSpec(
*                            LCDTV_OSD_SPEC_T *v_spec_ptr
*                            )
* \param  o_spec_ptr osd layer spec pointer
* \return void  
* \brief  set osd layer spec
*         
*
* \author Hanbing.Zhang
* \note   Nothing
************************************************************************/
void LCDTV_SetOSDSpec(
                      LCDTV_OSD_SPEC_T *o_spec_ptr
                      )
{

}

/************************************************************************
* \fn  void LCDTV_SetColorCorrection(
*                                    KDboolean                 color_correction_enable,
*                                    LCDTV_COLOR_CORRCTION_T *c_correction_ptr
*                                    )
* \param  color_correction_enable 
* \param  c_correction_ptr         correction matrix
* \return void  
* \brief  set lcd color correction matrix
*         
*
* \author Hanbing.Zhang
* \note   Nothing
************************************************************************/
void LCDTV_SetColorCorrection(
                              KDboolean                 color_correction_enable,
                              LCDTV_COLOR_CORRCTION_T *c_correction_ptr
                              )
{

}

/************************************************************************
* \fn  KDboolean LCDTV_IsLCDNeedReset( void )
* \param  void
* \return KD_TRUE need reset
* \brief  need reset or not
*         
*
* \author Hanbing.Zhang
* \note   Nothing
************************************************************************/
KDboolean LCDTV_IsLCDNeedReset( void )
{
    return 0;
}

/************************************************************************
* \fn  void LCDTV_SetLCDNeedReset(
*                                 KDboolean need_reset
*                                 )
* \param  need_reset
* \return void  
* \brief  
*         
*
* \author Hanbing.Zhang
* \note   Nothing
************************************************************************/
void LCDTV_SetLCDNeedReset(
                           KDboolean need_reset
                           )
{

}


/************************************************************************
* \fn  void LCDTV_SetGammaTable(
*                        KDuint16 *gamma_table_ptr
*                        )
* \param  gamma_table_ptr
* \return void  
* \brief  set gamma table
*         
*
* \author Hanbing.Zhang
* \note   Nothing
************************************************************************/
void LCDTV_SetGammaTable(
                         KDuint16 *gamma_table_ptr
                         )
{

    
}

/************************************************************************
* \fn  void LCDTV_SetEnableInvalidate(
*                                    KDboolean is_enable
*                                    )
* \param  is_enable
* \return void  
* \brief  
*         
*
* \author Hanbing.Zhang
* \note   Nothing
************************************************************************/
void LCDTV_SetEnableInvalidate(
                               KDboolean is_enable
                               )
{

}

/************************************************************************
* \fn  KDboolean LCDTV_IsEnableInvalidate(
*                                        void
*                                        )
* \param  void
* \return BOOLEAN is eanble invalidate  
* \brief  
*         
*
* \author Hanbing.Zhang
* \note   Nothing
************************************************************************/
KDboolean LCDTV_IsEnableInvalidate(
                                   void
                                   )
{
    return 0;
}


PUBLIC void DTL_MTVSoftSwitchON(BOOLEAN is_on)
{
}

#include "mtv_log.h"
PUBLIC void MTVLOG_Init(MTV_LOG_PARAM_T  log_param, BOOLEAN  is_sd_plug_in)
{

}

PUBLIC void MTVLOG_Exit(void)
{
}

/*lint -save -e765 */       
PUBLIC void SPIAUDIO_Mute(uint32 is_mute)
{
}


void GDACMAN_Init(void)
{
}

int GDAC_SetVolume(int volume)
{
    return 0;
}

int GDAC_GetVolume(void)
{
    return 0;
}

PUBLIC void DTL_TurnoffSeineLdo(void)
{
}

/*lint -restore */








