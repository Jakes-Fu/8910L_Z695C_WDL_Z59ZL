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
#include "cmmb_trc.h"
#include "lcd_tv.h"
#include "dtl_cmdq.h"
#include "cmd_common.h"
#include "os_api.h"
#include "mtv_boot.h"
#include "gpio_drv.h"
#include "lcm_cfg.h"
#include "lcm_prod.h"
#include "kd.h"
#include "mtv_ctrl.h"

/**--------------------------------------------------------------------------*
 **                         EXTERNAL DEFINITION                              *
 **--------------------------------------------------------------------------*/
//extern const KDuint16 lcd_width;        // width of main lcd.
//extern const KDuint16 lcd_height;        // height of main lcd.
LOCAL KDuint16 s_m_lcd_width = 0;         // width of main lcd.
LOCAL KDuint16 s_m_lcd_height = 0;        // height of main lcd.
LOCAL BOOLEAN is_enable_invalidate = TRUE;
/**---------------------------------------------------------------------------*
 **                         Macro Definition                                   *
 **---------------------------------------------------------------------------*/
//#define LCDTV_NOLCM

#define VIDEO_WIDTH     320//(352)
#define VIDEO_HEIGHT    240//(288)

#define LCD_TV_ANGLE_E LCDC_ANGLE_E

#define LCDTV_OSD_BASE_QUEUE_ADDR    OSD_BUFFER_BASE_ADDR //0x90d00000
#define LCDTV_OSD_PAD_QUEUE_ADDR     (OSD_BUFFER_BASE_ADDR+OSD_BUFFER_MAX_SIZE/2)//0x90d80000           //buffer middle address

LOCAL LCDC_CMD_SPEC_T       s_lcd_tv_spec = {0};
LOCAL LCDC_CMD_VIDEO_SPEC_T s_lcd_tv_video_spec = {0};
LOCAL LCDC_CMD_OSD_SPEC_T   s_lcd_osd_spec = {0};
LOCAL KDboolean               s_lcd_need_rest = KD_TRUE;
//锁
LOCAL KDThreadMutex* s_lcdtv_mutex_ptr = PNULL;

//旋转角度
LOCAL LCD_TV_ANGLE_E s_lcd_tv_angle = LCDC_ANGLE_0;//LCD_TV_ANGLE_INVALID;

LOCAL LCDC_VIDEO_SCALLING_T scaler_coeff = {0};
LOCAL LCDC_YUV2RGB_T lcdc_yuv2rgb_coeff = {0};

//osd coordinate
typedef struct lcdc_osd_coordiante_tag
{
    LCDC_START_COORDINATE_U start;
    LCDC_END_COORDINATE_U end;
}LCDTV_OSD_COORDINATE_T;

LCDC_COLOR_CORRCTION_T CCE_Matrix =
{   
    {392,   -71,   -68},
    {-68,    387,   -67},
    {-67,   -72,    392},
    0
};

//chroma horizontal conefficient
//low 9 bits, 36 bit coefficient, left->MSB
int16 coef_c_hor[8][4] =
{
    { 0, 255,   1, 0 },
    { 0, 224,  32, 0 },
    { 0, 192,  64, 0 },
    { 0, 160,  96, 0 },
    { 0, 128, 128, 0 },
    { 0,  96, 160, 0 },
    { 0,  64, 192, 0 },
    { 0,  32, 224, 0 }
};

//luma vertical coefficient when scalling up
//low 9 bits, left->0 address
int16 coef_y_ver_down[9][16] =
{
    {  0, 0, 0, 255,   1, 0, 0, 0,  0,  0,  0,  0,  0,  0,  0,  0},
    {  0, 0, 0, 224,  32, 0, 0, 0,  0,  0,  0,  0,  0,  0,  0,  0},
    {  0, 0, 0, 192,  64, 0, 0, 0,  0,  0,  0,  0,  0,  0,  0,  0},
    {  0, 0, 0, 160,  96, 0, 0, 0,  0,  0,  0,  0,  0,  0,  0,  0},
    {  0, 0, 0, 128, 128, 0, 0, 0,  0,  0,  0,  0,  0,  0,  0,  0},
    {  0, 0, 0,  96, 160, 0, 0, 0,  0,  0,  0,  0,  0,  0,  0,  0},
    {  0, 0, 0,  64, 192, 0, 0, 0,  0,  0,  0,  0,  0,  0,  0,  0},
    {  0, 0, 0,  32, 224, 0, 0, 0,  0,  0,  0,  0,  0,  0,  0,  0},
    {255, 0, 0,   0,   0, 0, 0, 0,  0,  0,  0,  0,  0,  0,  0,  0}
};

int16 coef_y_hor[8][8] =
{
    { 0, 0, 0, 255,   1, 0, 0, 0 },
    { 0, 0, 0, 224,  32, 0, 0, 0 },
    { 0, 0, 0, 192,  64, 0, 0, 0 },
    { 0, 0, 0, 160,  96, 0, 0, 0 },
    { 0, 0, 0, 128, 128, 0, 0, 0 },
    { 0, 0, 0,  96, 160, 0, 0, 0 },
    { 0, 0, 0,  64, 192, 0, 0, 0 },
    { 0, 0, 0,  32, 224, 0, 0, 0 }
};

//chroma vertical coefficient when scalling up
//low 9 bits, left->0 address
int16 coef_c_ver_down[9][8] =
{
    {  1, 253,   1, 1,  0,  0,  0,  0 },
    {  0, 223,  32, 1,  0,  0,  0,  0 },
    {  0, 191,  64, 1,  0,  0,  0,  0 },
    {  0, 159,  96, 1,  0,  0,  0,  0 },
    {  0, 127, 128, 1,  0,  0,  0,  0 },
    {  0,  95, 160, 1,  0,  0,  0,  0 },
    {  0,  63, 192, 1,  0,  0,  0,  0 },
    {  0,  31, 224, 1,  0,  0,  0,  0 },
    {255,   0,   0, 0,  0,  1,  0,  0 }
};

/**---------------------------------------------------------------------------*
 **                      Function Declaration                                 *
 **---------------------------------------------------------------------------*/
/************************************************************************
* \fn  LOCAL LCDC_CMD_VIDEO_SPEC_T* LCD_GetLCDTVVideoLayerSpec(void)
* \param  void
* \return  LCDC_CMD_VIDEO_SPEC_T 
* \brief  get lcdc video layer specification
*
* \author Hanbing.Zhang
* \note   Nothing
************************************************************************/
LOCAL LCDC_CMD_VIDEO_SPEC_T* LCD_GetLCDTVVideoLayerSpec(void);

/************************************************************************
* \fn  LOCAL LCDC_CMD_OSD_SPEC_T* LCD_GetLCDTVOSDLayerSpec(void)
* \param  void
* \return  LCDC_CMD_OSD_SPEC_T 
* \brief  get lcdc osd layer specification
*
* \author Hanbing.Zhang
* \note   Nothing
************************************************************************/
LOCAL LCDC_CMD_OSD_SPEC_T* LCD_GetLCDTVOSDLayerSpec(void);

/************************************************************************
* \fn  LOCAL void LCD_SetAngle(
                        LCD_TV_ANGLE_E angle
                        )
* \param  angle 
* \return void  
* \brief  record the angle
*
* \author Hanbing.Zhang
* \note   Nothing
************************************************************************/
LOCAL void LCD_SetAngle(
                        LCD_TV_ANGLE_E angle
                        );

/************************************************************************
* \fn  LOCAL LCD_TV_ANGLE_E LCD_GetAngle(void)
* \param  void 
* \return rotate angle 
* \brief  get the angle
*
* \author Hanbing.Zhang
* \note   Nothing
************************************************************************/
LOCAL LCD_TV_ANGLE_E LCD_GetAngle(void);

/************************************************************************
* \fn  LOCAL void LCD_GetRotatedRectBuf( 
                                 KDuint16* rotate_buf_ptr,
                                 KDuint16 buf_len,
                                 KDuint16 left,
                                 KDuint16 top,
                                 KDuint16 right,
                                 KDuint16 bottom,
                                 LCD_TV_ANGLE_E angle
                                 )
* \param  rotate_buf_ptr data buffer
* \param  buf_len buffer length
* \param  left 
* \param  top
* \param  right
* \param  bottom
* \param  angle
* \return void  
* \brief  get the data buffer after rotate angle
*
* \author Hanbing.Zhang
* \note   Nothing
************************************************************************/
LOCAL void LCD_GetRotatedRectBuf( 
                                 KDuint16* rotate_buf_ptr,
                                 KDuint16 buf_len,
                                 KDuint16 left,
                                 KDuint16 top,
                                 KDuint16 right,
                                 KDuint16 bottom,
                                 LCD_TV_ANGLE_E angle
                                 );

/************************************************************************
* \fn  LOCAL KDboolean LCD_IsOSDBaseQueueType( 
*                                 KDuint16 left,
*                                 KDuint16 top,
*                                 KDuint16 right,
*                                 KDuint16 bottom
*                                 )
* \param  left 
* \param  top
* \param  right
* \param  bottom
* \return KD_TRUE Base Queue Type  
* \brief  Is OSD Base Queue Type
*
* \author Hanbing.Zhang
* \note   Nothing
************************************************************************/
LOCAL KDboolean LCD_IsOSDBaseQueueType( 
                                     KDuint16 left,
                                     KDuint16 top,
                                     KDuint16 right,
                                     KDuint16 bottom
                                     );

/************************************************************************
* \fn  LOCAL LCDC_CMD_SPEC_T* LCD_GetLCDTVSpec(void)
* \param  void
* \return  LCDC_CMD_SPEC_T 
* \brief  get lcdc specification
*
* \author Hanbing.Zhang
* \note   Nothing
************************************************************************/
LOCAL LCDC_CMD_SPEC_T* LCD_GetLCDTVSpec(void);

/************************************************************************
* \fn  LOCAL KDuint32 LCD_Convert565to888(KDuint16 c565)
* \param  c565 565 color
* \return  888 color 
* \brief  convert color 565 to 888
*
* \author Hanbing.Zhang
* \note   Nothing
************************************************************************/
LOCAL KDuint32 LCD_Convert565to888(KDuint16 c565);

#ifdef _SC6600V_WQVGA_
//-------------------------------------------------------------------------------------------------
//--Note:
//--    Added by miller.song at 2008-09-27 for wqvga_mp4 projection.
//--    Only used for mpeg4 decoder, refer to player_display.c in DV module.
//-------------------------------------------------------------------------------------------------
LOCAL ERR_LCD_E LCD6600V_Init_MP4(void);


LOCAL ERR_LCD_E LCD6600V_InvalidateRect_MP4(
                                   LCD_ID_E lcd_id,//ID of lcd to operate
                                   uint16 left,     //left of the rectangle
                                   uint16 top,     //top of the rectangle
                                   uint16 right,     //right of the rectangle
                                   uint16 bottom    //bottom of the rectangle.
                                   );

LOCAL ERR_LCD_E LCD6600V_Invalidate_MP4(
                                  LCD_ID_E lcd_id    //ID of lcd to operate
                                  );

#endif

/************************************************************************
* \fn  LOCAL KDuint32 LCD_Convert565to888(KDuint16 c565)
* \param  c565 565 color
* \return  888 color 
* \brief  convert color 565 to 888
*
* \author Hanbing.Zhang
* \note   Nothing
************************************************************************/
LOCAL KDuint32 LCD_Convert565to888(KDuint16 c565)
{
    KDuint32 color8 = 0;
    KDuint32 rcolor = 0;
    KDuint32 gcolor = 0;
    KDuint32 bcolor = 0;
    
    //convert 565 to 888
    bcolor = c565 &0x1F;
    if ( bcolor & 0x1 )
    {
        bcolor <<= 3;    
        bcolor |= (0x7);
    }
    else
    {
        bcolor <<= 3;
    }
    gcolor = ((c565>>5) & 0x3f);
    if ( gcolor & 0x1 )
    {
        gcolor <<= (2+8);
        gcolor |= ((0x3)<<8);
    }
    else
    {
        gcolor <<= (2+8);    
    }
    rcolor = ((c565>>11) & 0x1f);
    if ( rcolor & 0x1 )
    {
        rcolor <<= (3+8+8);
        rcolor |= (0x7<<(8+8));
    }
    else
    {
        rcolor <<= (3+8+8);    
    }
    
    color8 = rcolor|gcolor|bcolor;

    return color8;
}

/************************************************************************
* \fn  LOCAL LCDC_CMD_SPEC_T* LCD_GetLCDTVSpec(void)
* \param  void
* \return  LCDC_CMD_SPEC_T 
* \brief  get lcdc specification
*
* \author Hanbing.Zhang
* \note   Nothing
************************************************************************/
LOCAL LCDC_CMD_SPEC_T* LCD_GetLCDTVSpec(void)
{
#if 0
    s_lcd_tv_spec.back_color.value        = 0xff00;
    s_lcd_tv_spec.order                   = LCDC_LAYER_ORDER_OSD;
    s_lcd_tv_spec.color_correction_enable = 0;
    s_lcd_tv_spec.dithering_enable        = 1;
    s_lcd_tv_spec.color_coeff             = CCE_Matrix;
    s_lcd_tv_spec.ex_ram_ptr              = (KDuint32*)ROTATION_BUFFER_BASE_ADDR;//(0x90c00000);
    s_lcd_tv_spec.osd_base_queue_addr     = (KDuint32*)LCDTV_OSD_BASE_QUEUE_ADDR;
    s_lcd_tv_spec.osd_pad_queue_addr      = (KDuint32*)LCDTV_OSD_PAD_QUEUE_ADDR;
#endif    
    return &s_lcd_tv_spec;
}


/************************************************************************
* \fn  LOCAL  void LCD_Getyuv2rgbcoeff(void)
* \param  void
* \return  void 
* \brief  get coeff
*
* \author Hanbing.Zhang
* \note   Nothing
************************************************************************/
LOCAL  void LCD_Getyuv2rgbcoeff(void)
{
    lcdc_yuv2rgb_coeff.yuv2rgb_c0[0]=0x100;
    lcdc_yuv2rgb_coeff.yuv2rgb_c0[1]=0x000;
    lcdc_yuv2rgb_coeff.yuv2rgb_c0[2]=0x1C5;
    lcdc_yuv2rgb_coeff.yuv2rgb_c0[3]=0xF1D<<1;
    lcdc_yuv2rgb_coeff.yuv2rgb_c1[0]=0x100;
    lcdc_yuv2rgb_coeff.yuv2rgb_c1[1]=0xF49;
    lcdc_yuv2rgb_coeff.yuv2rgb_c1[2]=0xFA8;
    lcdc_yuv2rgb_coeff.yuv2rgb_c1[3]=0x087<<1;
    lcdc_yuv2rgb_coeff.yuv2rgb_c2[0]=0x100;
    lcdc_yuv2rgb_coeff.yuv2rgb_c2[1]=0x167;
    lcdc_yuv2rgb_coeff.yuv2rgb_c2[2]=0x000;
    lcdc_yuv2rgb_coeff.yuv2rgb_c2[3]=0xF4D<<1;
}

/************************************************************************
* \fn  LOCAL void LCD_Getcoeff(void)
* \param  void
* \return  void 
* \brief  get coeff
*
* \author Hanbing.Zhang
* \note   Nothing
************************************************************************/
LOCAL void LCD_Getcoeff(void)
{
    KDuint32 i;    
    for(i=0; i<LCDC_SCALLING_V_LUMA_NUM; i++)
    {
        KDuint32 coeff;
        coeff = coef_y_ver_down[i/16][i%16] & 0x1ff;
        scaler_coeff.v_luma_coeff[i] = coeff;
    }

    for(i=0; i<LCDC_SCALLING_V_CHROMA_NUM; i++)
    {
        KDuint32 coeff;
        coeff = coef_c_ver_down[i/8][i%8] & 0x1ff;
        scaler_coeff.v_chroma_coeff[i] = coeff;
    }

    for(i=0; i<LCDC_SCALLING_H_LUMA_NUM; i++)
    {
        KDuint32 coeff_high, coeff_mid, coeff_low;
        coeff_high = (coef_y_hor[i][0] >> 1)&0xff;
        scaler_coeff.h_luma_coeff_high[i] = coeff_high;

        coeff_mid = coef_y_hor[i][0]&0x1;
        coeff_mid <<= 9;
        coeff_mid += coef_y_hor[i][1]&0x1ff;
        coeff_mid <<= 9;
        coeff_mid += coef_y_hor[i][2]&0x1ff;
        coeff_mid <<= 9;
        coeff_mid += coef_y_hor[i][3]&0x1ff;
        coeff_mid <<= 4;
        coeff_mid += (coef_y_hor[i][4]>>5)&0xf;
        scaler_coeff.h_luma_coeff_mid[i] = coeff_mid;

        coeff_low = coef_y_hor[i][4]&0x1f;
        coeff_low <<= 9;
        coeff_low += coef_y_hor[i][5]&0x1ff;
        coeff_low <<= 9;
        coeff_low += coef_y_hor[i][6]&0x1ff;
        coeff_low <<= 9;
        coeff_low += coef_y_hor[i][7]&0x1ff;
        scaler_coeff.h_luma_coeff_low[i] = coeff_low;
    }

    for(i=0; i<LCDC_SCALLING_H_CHROMA_NUM; i++)
    {
        KDuint32 coeff_high, coeff_low;
        coeff_high = (coef_c_hor[i][0] >> 5)&0xf;
        scaler_coeff.h_chroma_coeff_high[i] = coeff_high;

        coeff_low = coef_c_hor[i][0]&0x1f;
        coeff_low <<= 9;
        coeff_low += coef_c_hor[i][1]&0x1ff;
        coeff_low <<= 9;
        coeff_low += coef_c_hor[i][2]&0x1ff;
        coeff_low <<= 9;
        coeff_low += coef_c_hor[i][3]&0x1ff;
        scaler_coeff.h_chroma_coeff_low[i] = coeff_low;
    }
}

/************************************************************************
* \fn  LOCAL LCDC_CMD_VIDEO_SPEC_T* LCD_GetLCDTVVideoLayerSpec(void)
* \param  void
* \return  LCDC_CMD_VIDEO_SPEC_T 
* \brief  get lcdc video layer specification
*
* \author Hanbing.Zhang
* \note   Nothing
************************************************************************/
LOCAL LCDC_CMD_VIDEO_SPEC_T* LCD_GetLCDTVVideoLayerSpec(void)
{
#if 0
    s_lcd_tv_video_spec.video_size.mBits.height   = VIDEO_HEIGHT;
    s_lcd_tv_video_spec.video_size.mBits.width    = VIDEO_WIDTH;
    s_lcd_tv_video_spec.video_start.mBits.start_x = 0;
    s_lcd_tv_video_spec.video_start.mBits.start_y = 0;
    s_lcd_tv_video_spec.video_end.mBits.end_x     = VIDEO_WIDTH-1;
    s_lcd_tv_video_spec.video_end.mBits.end_y     = VIDEO_HEIGHT-1;
    s_lcd_tv_video_spec.lcd_start.mBits.start_x   = 0;
    s_lcd_tv_video_spec.lcd_start.mBits.start_y   = 0;
    s_lcd_tv_video_spec.lcd_end.mBits.end_x       = s_m_lcd_width-1;//VIDEO_WIDTH/2-1;
    s_lcd_tv_video_spec.lcd_end.mBits.end_y       = s_m_lcd_height-1;//VIDEO_HEIGHT/2-1;
    //    s_lcd_tv_video_spec.colorkey.colorkey_min.value                      = 0x80; 
    //    s_lcd_tv_video_spec.colorkey.colorkey_max.value                      = 0x80; // 0x80-0xffffffff    
    //(LCDC_VIDEO_SCALLING_T*)0x90CD8000;      //9c000000+432*256*4*2
    s_lcd_tv_video_spec.scaler_coeff_ptr          = (LCDC_VIDEO_SCALLING_T*)(LCDTV_OSD_BASE_QUEUE_ADDR-sizeof(LCDC_VIDEO_SCALLING_T));
    //    s_lcd_tv_video_spec.reserved_0
    s_lcd_tv_video_spec.angle                     = LCDC_ANGLE_90;
    s_lcd_tv_video_spec.deinterlace_enable        = 1;
    s_lcd_tv_video_spec.colorkey_enable           = 0;
    s_lcd_tv_video_spec.rotate_enable             = 1;//0;
    s_lcd_tv_video_spec.gamma_enable              = 0;
    s_lcd_tv_video_spec.filter_tap                = 8;
    s_lcd_tv_video_spec.alpha                     = 0xA0;
    s_lcd_tv_video_spec.yuv2rgb_shift             = 8;
    s_lcd_tv_video_spec.reserved_2                = 0;
    
    LCD_Getyuv2rgbcoeff();
    s_lcd_tv_video_spec.yuv2rgb_coeff             = lcdc_yuv2rgb_coeff;
    
    s_lcd_tv_video_spec.autosize                  = 1;
#endif    
    return &s_lcd_tv_video_spec;
}

/************************************************************************
* \fn  LOCAL LCDC_CMD_OSD_SPEC_T* LCD_GetLCDTVOSDLayerSpec(void)
* \param  void
* \return  LCDC_OSD_SPEC_T 
* \brief  get lcdc osd layer specification
*
* \author Hanbing.Zhang
* \note   Nothing
************************************************************************/
LOCAL LCDC_CMD_OSD_SPEC_T* LCD_GetLCDTVOSDLayerSpec(void)
{
#if 0
    s_lcd_osd_spec.lcd_start.mBits.start_x  = 0;
    s_lcd_osd_spec.lcd_start.mBits.start_y  = 0;
    s_lcd_osd_spec.lcd_end.mBits.end_x      = s_m_lcd_width-1;
    s_lcd_osd_spec.lcd_end.mBits.end_y      = s_m_lcd_height-1;
    //s_lcd_osd_spec.colorkey               = 0x80;
    s_lcd_osd_spec.osd_format               = LCDC_OSD_FORMAT_RGB565;
    s_lcd_osd_spec.clut8_data_ptr           = PNULL;
    s_lcd_osd_spec.alpha                    = 0;//0xa0;
    s_lcd_osd_spec.colorkey_enable          = 0;
    //    s_lcd_osd_spec.reserved_0         = 0;
    //    s_lcd_osd_spec.reserved_1         = 0;
#endif
    return &s_lcd_osd_spec;
}

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
    KDboolean ret = KD_FALSE;

    if (MTVBOOT_IsOK())
    {
        ret = KD_TRUE;
    }
    return ret;
}

/************************************************************************
* \fn  LOCAL KDboolean LCD_IsOSDBaseQueueType( 
*                                 KDuint16 left,
*                                 KDuint16 top,
*                                 KDuint16 right,
*                                 KDuint16 bottom
*                                 )
* \param  left 
* \param  top
* \param  right
* \param  bottom
* \return KD_TRUE Base Queue Type  
* \brief  Is OSD Base Queue Type
*
* \author Hanbing.Zhang
* \note   Nothing
************************************************************************/
LOCAL KDboolean LCD_IsOSDBaseQueueType( 
                                     KDuint16 left,
                                     KDuint16 top,
                                     KDuint16 right,
                                     KDuint16 bottom
                                     )
{
    if ( (right-left) >= (s_m_lcd_width-1) 
        && ( bottom-top) >= (s_m_lcd_height-1) )
    {
        return KD_TRUE;
    }
    return KD_FALSE;
}

/************************************************************************
* \fn  LOCAL KDuint32 LCD_GetOSDWriteBufPos(
*                                  KDuint32 osd_que_addr
*                                  )
* \param  osd_que_addr
* \return  write buffer pos
* \brief  get osd write buffer pos
*
* \author Hanbing.Zhang
* \note   Nothing
************************************************************************/
LOCAL KDuint32 LCD_GetOSDWriteBufPos(
                                   KDuint32 osd_que_addr
                                   )
{
    KDuint32  write_pos  = LCDC_OSD_BUF_NUM;
    KDuint32  read_pos   = 0;
    KDuint32 wait_num = 0;
    
    DTL_GetData((KDuint8*)&write_pos,sizeof(KDuint32),osd_que_addr);
    INT_SWAP(write_pos);
    DTL_GetData((KDuint8*)&read_pos,sizeof(KDuint32),osd_que_addr + sizeof(KDuint32));
    INT_SWAP(read_pos);
    write_pos = (write_pos+1)%LCDC_OSD_BUF_NUM;
    if ( write_pos != read_pos )
    {
        return write_pos;
    }
    else
    {
lcd_wait:
        DTL_GetData((KDuint8*)&read_pos,sizeof(KDuint32),osd_que_addr + sizeof(KDuint32));
        INT_SWAP(read_pos);
        if ( write_pos != read_pos )
        {
            return write_pos;
        }
        else
        {
            KD_TRACE_LOW("LCD_GetOSDWriteBufPos pos identical wait_num=%d",wait_num);
            kdMsDelay(1);
            wait_num ++; 
            if (wait_num > 100)
            {
                return write_pos;
            }
            goto lcd_wait;
            //SCI_PASSERT(0,("LCD_GetOSDWriteBufPos pos identical"));
        }
    }
    /* pclint warning 527. comment off
    return write_pos;
    */
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
    KDuint32 init_pos[2] = {0};
    MTV_LCD_SIZE_T*  lcd_spec_ptr = PNULL;

#ifndef LCDTV_NOLCM

#ifdef _SC6600V_WQVGA_
    {
        //--Added by miller.song at 2008-09-27 for wqvga_mp4 projection.
        //--Only used for mpeg4 decoder, refer to player_display.c in DV module.
        extern BOOLEAN s_is_6600v_wqvga_mp4_using;
        if(s_is_6600v_wqvga_mp4_using)
        {
            return LCD6600V_Init_MP4();
        }
    }
#endif

    lcd_spec_ptr = MTV_GetLCDSize(0); //MAIN_LCD_ID
    s_m_lcd_width = lcd_spec_ptr->width;
    s_m_lcd_height = lcd_spec_ptr->height;
/*   
    DTLCMDQ_LCDC_Set_Spec(LCD_GetLCDTVSpec());
    DTLCMDQ_LCDC_Enable(KD_TRUE);

    LCD_Getcoeff();
    DTL_SendData((KDuint8*)&scaler_coeff, sizeof(LCDC_VIDEO_SCALLING_T), (LCDTV_OSD_BASE_QUEUE_ADDR-sizeof(LCDC_VIDEO_SCALLING_T)));
    DTLCMDQ_LCDC_Set_Layer_Spec((KDuint8*)(LCD_GetLCDTVVideoLayerSpec()),LCDC_LAYER_VIDEO);
    
    DTLCMDQ_LCDC_Set_Layer_Spec((KDuint8*)(LCD_GetLCDTVOSDLayerSpec()),LCDC_LAYER_OSD);

    DTLCMDQ_LCDC_Layer_Enable(LCDC_LAYER_VIDEO,KD_TRUE);
    DTLCMDQ_LCDC_Layer_Enable(LCDC_LAYER_OSD,KD_TRUE);
*/
    //clean up read write position
    INT_SWAP(init_pos[0]);
    INT_SWAP(init_pos[1]);
    DTL_SendData((KDuint8*)&init_pos,2*sizeof(KDuint32),LCDTV_OSD_BASE_QUEUE_ADDR );
    DTL_SendData((KDuint8*)&init_pos,2*sizeof(KDuint32),LCDTV_OSD_PAD_QUEUE_ADDR);

    //创建锁
    if ( s_lcdtv_mutex_ptr )
    {
        /* if you have API to fill the kdThreadMutexFree() interface, return 0
         * after invoking your API, otherwise, just return a non-zero number, say 1 */

        if ( 0 == kdThreadMutexFree(s_lcdtv_mutex_ptr))
        {        
            s_lcdtv_mutex_ptr = PNULL;
        }
    }
    if ( s_lcdtv_mutex_ptr == NULL)
    {
        s_lcdtv_mutex_ptr = kdThreadMutexCreate(KD_NULL);
    }
    kdAssert(PNULL != s_lcdtv_mutex_ptr);

    
#endif
    LCDTV_SetEnableInvalidate(TRUE);
    return 0;
}

/************************************************************************
* \fn  LOCAL void LCDTV_GetRectBuf(
*                           KDuint16 left,
*                           KDuint16 top,
*                           KDuint16 right,
*                           KDuint16 bottom
*                           )
* \param  left    left of the rectangle
* \param  top     top of the rectangle
* \param  right   right of the rectangle
* \param  bottom  bottom of the rectangle
* \return void
* \brief  get not full width refresh rect buffer
*
* \author Hanbing.Zhang
* \note   Nothing.  
************************************************************************/
LOCAL void LCDTV_GetRectBuf(
                            KDuint16 left,
                            KDuint16 top,
                            KDuint16 right,
                            KDuint16 bottom
                            )
{
    KDuint32 swap_num = 0;
    KDuint16 width = 0;
    KDuint16 height = 0;
    KDuint16 *dest_ptr = (KDuint16*)MTV_GetLCDOSDBuffer();
    KDuint16 *buf_ptr = (KDuint16 *)MTV_GetMainLCDBuffer();
    
    kdAssert(right<s_m_lcd_width);
    kdAssert(bottom<s_m_lcd_height);
    kdAssert(right>left);
    kdAssert(bottom>top);
    
    //not full width data
    if ( (right-left) != (s_m_lcd_width - 1) )
    {
        buf_ptr = buf_ptr + (top * s_m_lcd_width + left);     //pointer buffer start address
        width	=  right  - left + 1;                     //calculate width
        height  =  bottom - top  + 1;                     //calculate height
        do
        {	            
            kdMemcpy((KDuint8*)dest_ptr,(KDuint8*)buf_ptr, width<<1);
            for ( swap_num = 0; swap_num < width;swap_num++)
            {
                INT_SWAP(dest_ptr[swap_num]);
            }
            dest_ptr += width;
            buf_ptr += s_m_lcd_width;	            //buffer point to next line
            height--;                           //next line
        }while(height > 0);	    
    }
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
#ifndef LCDTV_NOLCM
    KDuint32 swap_num = 0;
    KDboolean is_osd_base_que = KD_FALSE;
    KDuint32  osd_que_addr = 0;
    KDuint32  write_pos    = 0;
    KDuint32  write_pos_addr    = 0;
    LCDTV_OSD_COORDINATE_T osd_coor = {0};
    KDuint16* rotate_buf_ptr = PNULL;
    KDuint16 *buf_ptr = (KDuint16 *)MTV_GetMainLCDBuffer();
    KDuint16 *dest_ptr = (KDuint16*)MTV_GetLCDOSDBuffer();    
    
    KDuint32 buf_len = ((bottom-top+1) * (right-left+1))<<1;

    if (!LCDTV_IsEnableInvalidate())
    {
        return ERR_LCD_OPERATE_FAIL;
    }

    is_osd_base_que = LCD_IsOSDBaseQueueType(left,top,right,bottom);

    if ( is_osd_base_que )
    {
        osd_que_addr = LCDTV_OSD_BASE_QUEUE_ADDR;
    }
    else
    {
        osd_que_addr = LCDTV_OSD_PAD_QUEUE_ADDR;
    }

    //获取锁 进入临界区
    kdThreadMutexLock(s_lcdtv_mutex_ptr);
    
    write_pos = LCD_GetOSDWriteBufPos(osd_que_addr);
    kdAssert( LCDC_OSD_BUF_NUM > write_pos);

    write_pos_addr = osd_que_addr;

    osd_que_addr += 2*sizeof(KDuint32) + write_pos*sizeof(LCDC_OSD_PAD_T);

    if ( !is_osd_base_que )
    {
        osd_coor.start.mBits.start_x = left;
        osd_coor.start.mBits.start_y = top;
        osd_coor.end.mBits.end_x     = right;
        osd_coor.end.mBits.end_y     = bottom;
        
        INT_SWAP(osd_coor.start.mBits.start_x);
        INT_SWAP(osd_coor.start.mBits.start_y);
        INT_SWAP(osd_coor.end.mBits.end_x);
        INT_SWAP(osd_coor.end.mBits.end_y);
        DTL_SendData((KDuint8*)&osd_coor,sizeof(LCDTV_OSD_COORDINATE_T),osd_que_addr);
    }

    osd_que_addr += sizeof(LCDTV_OSD_COORDINATE_T);
    if ( LCDC_ANGLE_0 == LCD_GetAngle())
    {            
        if ( (right-left+1) != s_m_lcd_width )
        {
            LCDTV_GetRectBuf(left,top,right,bottom);
            DTL_SendData((KDuint8*)MTV_GetLCDOSDBuffer(),buf_len,osd_que_addr);
        }
        else
        {
            buf_ptr += top * s_m_lcd_width;

            kdMemcpy((KDuint8*)dest_ptr, (KDuint8*)buf_ptr, buf_len);
            for ( swap_num = 0; swap_num < buf_len/2;swap_num++)
            {
                INT_SWAP(dest_ptr[swap_num]);
            }
            DTL_SendData((KDuint8*)dest_ptr,buf_len,osd_que_addr);
        }
    }
    else
    {
        rotate_buf_ptr = (KDuint16*)kdMalloc(buf_len*sizeof(KDuint8));
        kdAssert( PNULL!= rotate_buf_ptr );
        
        LCD_GetRotatedRectBuf(rotate_buf_ptr,buf_len,left,top,right,bottom,LCD_GetAngle());
        for ( swap_num = 0; swap_num < buf_len/2;swap_num++)
        {
            INT_SWAP(rotate_buf_ptr[swap_num]);
        }
        DTL_SendData((KDuint8*)rotate_buf_ptr,buf_len,osd_que_addr);
        kdFree(rotate_buf_ptr);
    }
    INT_SWAP(write_pos);
    DTL_SendData((KDuint8*)&write_pos,sizeof(KDuint32),write_pos_addr);
    
    if ( is_osd_base_que )
    {
        DTLCMDQ_LCDC_Display_OSD();
    }
    else
    {
        DTLCMDQ_LCDC_Update_OSD();
    }

    //释放锁 出临界区
    kdThreadMutexUnlock(s_lcdtv_mutex_ptr);
#endif    
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
#ifndef LCDTV_NOLCM
    KDboolean is_osd_base_que = KD_FALSE;
    KDuint32  swap_num = 0;
    KDuint32  osd_que_addr = 0;
    KDuint32  write_pos    = 0;
    KDuint32  write_pos_addr = 0;
    KDuint16 *dest_ptr = (KDuint16*)MTV_GetLCDOSDBuffer();    
    LCDTV_OSD_COORDINATE_T osd_coor = {0};
    KDuint32 buf_len = ((bottom-top+1) * (right-left+1))<<1;
    
    if (!LCDTV_IsEnableInvalidate())
    {
        return ERR_LCD_OPERATE_FAIL;
    }

    is_osd_base_que = LCD_IsOSDBaseQueueType(left,top,right,bottom);

    if ( is_osd_base_que )
    {
        osd_que_addr = LCDTV_OSD_BASE_QUEUE_ADDR;
    }
    else
    {
        osd_que_addr = LCDTV_OSD_PAD_QUEUE_ADDR;
    }

    //获取锁 进入临界区
    kdThreadMutexLock(s_lcdtv_mutex_ptr);
    
    write_pos = LCD_GetOSDWriteBufPos(osd_que_addr);
    kdAssert( LCDC_OSD_BUF_NUM != write_pos);

    write_pos_addr = osd_que_addr;

    osd_que_addr += 2*sizeof(KDuint32) + write_pos*sizeof(LCDC_OSD_PAD_T);

    if ( !is_osd_base_que )
    {
        osd_coor.start.mBits.start_x = left;
        osd_coor.start.mBits.start_y = top;
        osd_coor.end.mBits.end_x     = right;
        osd_coor.end.mBits.end_y     = bottom;

        INT_SWAP(osd_coor.start.mBits.start_x);
        INT_SWAP(osd_coor.start.mBits.start_y);
        INT_SWAP(osd_coor.end.mBits.end_x);
        INT_SWAP(osd_coor.end.mBits.end_y);

        DTL_SendData((KDuint8*)&osd_coor,sizeof(LCDTV_OSD_COORDINATE_T),osd_que_addr);
    }

    osd_que_addr += sizeof(LCDTV_OSD_COORDINATE_T);

    kdMemcpy((KDuint8*)dest_ptr,(KDuint8*)buf_ptr, buf_len);
    for ( swap_num = 0; swap_num < buf_len/2;swap_num++)
    {
        INT_SWAP(dest_ptr[swap_num]);
    }
    DTL_SendData((KDuint8*)dest_ptr,buf_len,osd_que_addr);
    INT_SWAP(write_pos);
    DTL_SendData((KDuint8*)&write_pos,sizeof(KDuint32),write_pos_addr);

    if ( is_osd_base_que )
    {
        DTLCMDQ_LCDC_Display_OSD();
    }
    else
    {
        DTLCMDQ_LCDC_Update_OSD();
    }

    //释放锁 出临界区
    kdThreadMutexUnlock(s_lcdtv_mutex_ptr);
#endif    
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
#ifndef LCDTV_NOLCM
    KDuint32 write_pos = 0;
    KDuint32 write_pos_addr = 0;
    KDuint32 osd_que_addr = LCDTV_OSD_BASE_QUEUE_ADDR;
    KDuint16 *buf_ptr = (KDuint16 *)MTV_GetMainLCDBuffer();
    KDuint16 *dest_ptr = (KDuint16*)MTV_GetLCDOSDBuffer();    
    KDuint32 buf_len = (s_m_lcd_width * s_m_lcd_height)<<1;
    KDuint32 swap_num = 0;

    if (!LCDTV_IsEnableInvalidate())
    {
        return ERR_LCD_OPERATE_FAIL;
    }

#ifdef _SC6600V_WQVGA_
    {
        //--Added by miller.song at 2008-09-27 for wqvga_mp4 projection.
        //--Only used for mpeg4 decoder, refer to player_display.c in DV module.
        extern BOOLEAN s_is_6600v_wqvga_mp4_using;
        if(s_is_6600v_wqvga_mp4_using)
        {
            return LCD6600V_Invalidate_MP4(lcd_id);
        }
    }
#endif
    
    //获取锁 进入临界区
    kdThreadMutexLock(s_lcdtv_mutex_ptr);

    write_pos = LCD_GetOSDWriteBufPos(osd_que_addr);
    kdAssert( LCDC_OSD_BUF_NUM != write_pos);
    
    write_pos_addr = osd_que_addr;

    osd_que_addr += 2*sizeof(KDuint32) + write_pos*sizeof(LCDC_OSD_PAD_T);
    osd_que_addr += sizeof(LCDTV_OSD_COORDINATE_T);

    kdMemcpy((KDuint8*)dest_ptr,(KDuint8*)buf_ptr, buf_len);
    for ( swap_num = 0; swap_num < buf_len/2;swap_num++)
    {
        INT_SWAP(dest_ptr[swap_num]);
    }
    DTL_SendData((KDuint8*)dest_ptr,buf_len,osd_que_addr);
    
    INT_SWAP(write_pos);
    DTL_SendData((KDuint8*)&write_pos,sizeof(KDuint32),write_pos_addr);
    DTLCMDQ_LCDC_Display_OSD();

    //释放锁 出临界区
    kdThreadMutexUnlock(s_lcdtv_mutex_ptr);
#endif
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
#ifndef LCDTV_NOLCM
    DTLCMDQ_LCDC_Set_Contrast(contrast);
#endif    
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
#ifndef LCDTV_NOLCM
    DTLCMDQ_LCDC_Set_Brightness(brightness);
#endif    
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
#ifndef LCDTV_NOLCM
    LCDC_CMD_VIDEO_SPEC_T *video_layer_ptr = PNULL;
    LCDC_CMD_OSD_SPEC_T   *osd_layer_ptr = PNULL;
    
    video_layer_ptr = LCD_GetLCDTVVideoLayerSpec();
    osd_layer_ptr = LCD_GetLCDTVOSDLayerSpec();

    osd_layer_ptr->lcd_start.mBits.start_x = left;
    osd_layer_ptr->lcd_start.mBits.start_y = top;
    osd_layer_ptr->lcd_end.mBits.end_x     = right;
    osd_layer_ptr->lcd_end.mBits.end_y     = bottom;
    
    video_layer_ptr->lcd_start.mBits.start_x = left;
    video_layer_ptr->lcd_start.mBits.start_y = top;
    video_layer_ptr->lcd_end.mBits.end_x     = right;
    video_layer_ptr->lcd_end.mBits.end_y     = bottom;
    
    DTLCMDQ_LCDC_Set_Layer_Spec((KDuint8*)video_layer_ptr,LCDC_LAYER_VIDEO);
    DTLCMDQ_LCDC_Set_Layer_Spec((KDuint8*)osd_layer_ptr,LCDC_LAYER_OSD);
#endif    
    return 0;
}

/************************************************************************
* \fn KDuint32  LCDTV_SetStartAddress(
*                                       KDuint32 lcd_id,//ID of lcd to operate
*                                       KDuint16 left,     //left of the display window
*                                       KDuint16 top     //top of the display window
*                                       )
* \param  lcd_id  LCD ID
* \param  left    left of the display window
* \param  top     top of the display window
* \return KDuint32
* \brief  set start address
*
* \author Hanbing.Zhang
* \note   Nothing.  
************************************************************************/
KDuint32  LCDTV_SetStartAddress(
                              KDuint32 lcd_id,//ID of lcd to operate
                              KDuint16 left,     //left of the display window
                              KDuint16 top     //top of the display window
                              )
{
#ifndef LCDTV_NOLCM
    LCDC_CMD_VIDEO_SPEC_T *video_layer_ptr = LCD_GetLCDTVVideoLayerSpec();
    LCDC_CMD_OSD_SPEC_T   *osd_layer_ptr = LCD_GetLCDTVOSDLayerSpec();

    osd_layer_ptr->lcd_start.mBits.start_x = left;
    osd_layer_ptr->lcd_start.mBits.start_y = top;
    
    video_layer_ptr->lcd_start.mBits.start_x = left;
    video_layer_ptr->lcd_start.mBits.start_y = top;
    
    DTLCMDQ_LCDC_Set_Layer_Spec((KDuint8*)video_layer_ptr,LCDC_LAYER_VIDEO);
    DTLCMDQ_LCDC_Set_Layer_Spec((KDuint8*)osd_layer_ptr,LCDC_LAYER_OSD);
#endif    
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
#ifndef LCDTV_NOLCM
    //set osd layer in the same time?
    LCDC_CMD_VIDEO_SPEC_T *video_spec_ptr = LCD_GetLCDTVVideoLayerSpec();

    //由于缩放系数问题，导致此处需要设置video尺寸，不合理
    switch (is_invert)
    {
    case 0:
        //because scalling only support 1/2,1/4,there should set the cor
        video_spec_ptr->lcd_start.mBits.start_x   = 41;
        video_spec_ptr->lcd_start.mBits.start_y   = 101;
        video_spec_ptr->lcd_end.mBits.end_x       = 200;
        video_spec_ptr->lcd_end.mBits.end_y       = 220;

        video_spec_ptr->angle         = LCDC_ANGLE_0;
        video_spec_ptr->rotate_enable = KD_FALSE;
        
        DTLCMDQ_LCDC_Set_Layer_Spec((KDuint8*)video_spec_ptr,LCDC_LAYER_VIDEO);
        
        LCD_SetAngle(LCDC_ANGLE_0);
        break;
    case 1:
        video_spec_ptr->lcd_start.mBits.start_x   = 0;
        video_spec_ptr->lcd_start.mBits.start_y   = 0;
        video_spec_ptr->lcd_end.mBits.end_x       = s_m_lcd_width-1;
        video_spec_ptr->lcd_end.mBits.end_y       = s_m_lcd_height-1;
        video_spec_ptr->angle         = LCDC_ANGLE_90;
        video_spec_ptr->rotate_enable = KD_TRUE;
        
        DTLCMDQ_LCDC_Set_Layer_Spec((KDuint8*)video_spec_ptr,LCDC_LAYER_VIDEO);
        
        LCD_SetAngle(LCDC_ANGLE_90);
        break;
    case 2:
        video_spec_ptr->lcd_start.mBits.start_x   = 0;
        video_spec_ptr->lcd_start.mBits.start_y   = 0;
        video_spec_ptr->lcd_end.mBits.end_x       = s_m_lcd_width-1;
        video_spec_ptr->lcd_end.mBits.end_y       = s_m_lcd_height-1;
        video_spec_ptr->angle         = LCDC_ANGLE_270;
        video_spec_ptr->rotate_enable = KD_TRUE;
        
        DTLCMDQ_LCDC_Set_Layer_Spec((KDuint8*)video_spec_ptr,LCDC_LAYER_VIDEO);
        
        LCD_SetAngle(LCDC_ANGLE_270);
        break;
    default:
        break;
    }
#endif
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
#ifndef LCDTV_NOLCM
    KDuint32 i;
    KDuint16 swap_color = (KDuint16)color;
    KDuint16 *buf_ptr = (KDuint16 *)MTV_GetMainLCDBuffer();
    
    INT_SWAP(swap_color);
    for( i=0; i<(s_m_lcd_height*s_m_lcd_width); i++)
    {
        *buf_ptr++ = swap_color;
    }
    
    LCDTV_Invalidate(lcd_id);
#endif    
}

/************************************************************************
* \fn  LOCAL void LCD_SetAngle(
                        LCD_TV_ANGLE_E angle
                        )
* \param  angle 
* \return void  
* \brief  record the angle
*
* \author Hanbing.Zhang
* \note   Nothing
************************************************************************/
LOCAL void LCD_SetAngle(
                        LCD_TV_ANGLE_E angle
                        )
{
    //s_lcd_tv_angle = angle;
}

/************************************************************************
* \fn  LOCAL LCD_TV_ANGLE_E LCD_GetAngle(void)
* \param  void 
* \return rotate angle 
* \brief  get the angle
*
* \author Hanbing.Zhang
* \note   Nothing
************************************************************************/
LOCAL LCD_TV_ANGLE_E LCD_GetAngle(void)
{
    return s_lcd_tv_angle;
}

/************************************************************************
* \fn  LOCAL void LCD_GetRotatedRectBuf( 
                                 KDuint16* rotate_buf_ptr,
                                 KDuint16 buf_len,
                                 KDuint16 left,
                                 KDuint16 top,
                                 KDuint16 right,
                                 KDuint16 bottom,
                                 LCD_TV_ANGLE_E angle
                                 )
* \param  rotate_buf_ptr data buffer
* \param  buf_len buffer length
* \param  left 
* \param  top
* \param  right
* \param  bottom
* \param  angle
* \return void  
* \brief  get the data buffer after rotate angle
*
* \author Hanbing.Zhang
* \note   Nothing
************************************************************************/
LOCAL void LCD_GetRotatedRectBuf( 
                                 KDuint16* rotate_buf_ptr,
                                 KDuint16 buf_len,
                                 KDuint16 left,
                                 KDuint16 top,
                                 KDuint16 right,
                                 KDuint16 bottom,
                                 LCD_TV_ANGLE_E angle
                                 )
{
    KDuint16 i = 0;
    KDuint16 j = 0;
    KDuint16 line = 0;
    KDuint32 swap_num = 0;
    KDuint16 *lcd_buf = PNULL;
    
    kdAssert( buf_len <= (right-left+1)*(bottom-top+1));
    
    lcd_buf = MTV_GetMainLCDBuffer();
    
    for ( swap_num = 0; swap_num < buf_len;swap_num++)
    {
        INT_SWAP(lcd_buf[swap_num]);
    }
    
    switch(angle)
    {
    case LCDC_ANGLE_90:
    case LCDC_ANGLE_270:
        for ( j = top; j < bottom; j ++ )
        {
            line = j * s_m_lcd_width;
            for ( i = left; i < right; i ++ )
            {
                *rotate_buf_ptr = *(lcd_buf + line + i );
            }
        }    
        break;
    default:
        break;
    }
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
#ifndef LCDTV_NOLCM
    LCDC_CMD_VIDEO_SPEC_T* video_layer_ptr = PNULL;
    
    kdAssert( video_bottom>=video_left && video_right>=video_top );
    
    video_layer_ptr=  LCD_GetLCDTVVideoLayerSpec();
    kdAssert(PNULL != video_layer_ptr);

    video_layer_ptr->lcd_start.mBits.start_x   = video_left;
    video_layer_ptr->lcd_start.mBits.start_y   = video_top;
    video_layer_ptr->lcd_end.mBits.end_x       = video_right;
    video_layer_ptr->lcd_end.mBits.end_y       = video_bottom;
    
    DTLCMDQ_LCDC_Set_Layer_Spec((KDuint8*)video_layer_ptr,LCDC_LAYER_VIDEO);
    
    //calculate scaler coeff and send scaler_coeff_ptr
#endif    
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
#ifndef LCDTV_NOLCM
    LCDC_CMD_SPEC_T*  lcd_spec_ptr = PNULL;
    LCDC_CMD_VIDEO_SPEC_T* video_layer_ptr = PNULL;
    LCDC_CMD_OSD_SPEC_T* osd_layer_ptr = PNULL;
    
    lcd_spec_ptr = LCD_GetLCDTVSpec();
    if ( layer_order != lcd_spec_ptr->order )
    {
        lcd_spec_ptr->order = layer_order;
        DTLCMDQ_LCDC_Set_Spec(lcd_spec_ptr);
    }

    if ( LCDC_LAYER_ORDER_VIDEO == layer_order )
    {
        video_layer_ptr = LCD_GetLCDTVVideoLayerSpec();
        kdAssert(PNULL!= video_layer_ptr);
        
        if ( alpha_enable )
        {
            video_layer_ptr->alpha = alpha;
        }
        
        if ( colorkey_enable )
        {
            video_layer_ptr->colorkey.colorkey_max.value = LCD_Convert565to888(colorkey_max);
            video_layer_ptr->colorkey.colorkey_min.value = LCD_Convert565to888(colorkey_min);
            video_layer_ptr->colorkey_enable = colorkey_enable;
        }
        DTLCMDQ_LCDC_Set_Layer_Spec((KDuint8*)video_layer_ptr,LCDC_LAYER_VIDEO);
        
    }
    else if ( LCDC_LAYER_ORDER_OSD == layer_order)
    {
        osd_layer_ptr = LCD_GetLCDTVOSDLayerSpec();
        kdAssert(PNULL!= osd_layer_ptr);
        
        if ( alpha_enable )
        {
            osd_layer_ptr->alpha = alpha;
        }
        if ( colorkey_enable )
        {
            osd_layer_ptr->colorkey.colorkey_max.value = LCD_Convert565to888(colorkey_max);
            osd_layer_ptr->colorkey.colorkey_min.value = LCD_Convert565to888(colorkey_min);
            osd_layer_ptr->colorkey_enable = colorkey_enable;
        }
        DTLCMDQ_LCDC_Set_Layer_Spec((KDuint8*)osd_layer_ptr,LCDC_LAYER_OSD);
    }
    else
    {
        //layer_order error
    }
#endif    
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
    KDuint32 back_color = 0;
    
    kdAssert(PNULL!= spec_ptr);
    
    back_color = LCD_Convert565to888(spec_ptr->back_color);
    
    if (LCDTV_IsOpen())
    {
        s_lcd_tv_spec.back_color.value        = back_color;
        s_lcd_tv_spec.order                   = spec_ptr->order;
        s_lcd_tv_spec.color_correction_enable = KD_FALSE;
        s_lcd_tv_spec.dithering_enable        = KD_TRUE;
        s_lcd_tv_spec.color_coeff             = CCE_Matrix;

        s_lcd_tv_spec.ex_ram_ptr              = (uint32*)ROTATION_BUFFER_BASE_ADDR;
        s_lcd_tv_spec.osd_base_queue_addr     = (uint32*)LCDTV_OSD_BASE_QUEUE_ADDR;
        s_lcd_tv_spec.osd_pad_queue_addr      = (uint32*)LCDTV_OSD_PAD_QUEUE_ADDR;
        
        DTLCMDQ_LCDC_Set_Spec((LCDC_CMD_SPEC_T*)&s_lcd_tv_spec);
        //DTLCMDQ_LCDC_Enable(KD_TRUE);
    }
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
    DTLCMDQ_LCDC_Enable(is_enable);
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
    KDuint32 colorkey_max = 0;
    KDuint32 colorkey_min = 0;
    
    kdAssert(PNULL != v_spec_ptr);

    colorkey_max = LCD_Convert565to888(v_spec_ptr->colorkey_max);
    colorkey_min = LCD_Convert565to888(v_spec_ptr->colorkey_min);

    if (LCDTV_IsOpen())
    {
        //s_lcd_tv_video_spec.video_size.mBits.height   = VIDEO_HEIGHT;
        //s_lcd_tv_video_spec.video_size.mBits.width    = VIDEO_WIDTH;
        //s_lcd_tv_video_spec.video_start.mBits.start_x = v_spec_ptr->video_left;
        //s_lcd_tv_video_spec.video_start.mBits.start_y = v_spec_ptr->video_top;
        //s_lcd_tv_video_spec.video_end.mBits.end_x     = v_spec_ptr->video_right;
        //s_lcd_tv_video_spec.video_end.mBits.end_y     = v_spec_ptr->video_bottom;
        s_lcd_tv_video_spec.lcd_start.mBits.start_x   = v_spec_ptr->lcd_disp_left;
        s_lcd_tv_video_spec.lcd_start.mBits.start_y   = v_spec_ptr->lcd_disp_top;
        s_lcd_tv_video_spec.lcd_end.mBits.end_x       = v_spec_ptr->lcd_disp_right;
        s_lcd_tv_video_spec.lcd_end.mBits.end_y       = v_spec_ptr->lcd_disp_bottom;
        s_lcd_tv_video_spec.colorkey.colorkey_max.value = colorkey_max;
        s_lcd_tv_video_spec.colorkey.colorkey_min.value = colorkey_min;

        s_lcd_tv_video_spec.scaler_coeff_ptr          = (LCDC_VIDEO_SCALLING_T*)(LCDTV_OSD_BASE_QUEUE_ADDR-sizeof(LCDC_VIDEO_SCALLING_T));
        s_lcd_tv_video_spec.angle                     = v_spec_ptr->angle;
        s_lcd_tv_video_spec.deinterlace_enable        = KD_TRUE;
        s_lcd_tv_video_spec.colorkey_enable           = v_spec_ptr->colorkey_enable;
        s_lcd_tv_video_spec.rotate_enable             = v_spec_ptr->rotate_enable;
        s_lcd_tv_video_spec.gamma_enable              = v_spec_ptr->gamma_enable;
        s_lcd_tv_video_spec.filter_tap                = 8;
        s_lcd_tv_video_spec.alpha                     = v_spec_ptr->alpha;
        s_lcd_tv_video_spec.yuv2rgb_shift             = 8;
        
        
        LCD_Getyuv2rgbcoeff();
        s_lcd_tv_video_spec.yuv2rgb_coeff             = lcdc_yuv2rgb_coeff;
        
        s_lcd_tv_video_spec.autosize                  = v_spec_ptr->auto_size;
        
        LCD_Getcoeff();
        DTL_SendData((KDuint8*)&scaler_coeff, sizeof(LCDC_VIDEO_SCALLING_T), (LCDTV_OSD_BASE_QUEUE_ADDR-sizeof(LCDC_VIDEO_SCALLING_T)));

        DTLCMDQ_LCDC_Set_Layer_Spec((KDuint8*)&s_lcd_tv_video_spec,LCDC_LAYER_VIDEO);

        DTLCMDQ_LCDC_Layer_Enable(LCDC_LAYER_VIDEO,KD_TRUE);
    }
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
    KDuint32 colorkey_max = 0;
    KDuint32 colorkey_min = 0;

    kdAssert(PNULL!= o_spec_ptr);

    colorkey_max = LCD_Convert565to888(o_spec_ptr->colorkey_max);
    colorkey_min = LCD_Convert565to888(o_spec_ptr->colorkey_min);

    if (LCDTV_IsOpen())
    {
        s_lcd_osd_spec.lcd_start.mBits.start_x  = o_spec_ptr->osd_left;
        s_lcd_osd_spec.lcd_start.mBits.start_y  = o_spec_ptr->osd_top;
        s_lcd_osd_spec.lcd_end.mBits.end_x      = o_spec_ptr->osd_right;
        s_lcd_osd_spec.lcd_end.mBits.end_y      = o_spec_ptr->osd_bottom;

        s_lcd_osd_spec.colorkey.colorkey_max.value = colorkey_max;
        s_lcd_osd_spec.colorkey.colorkey_min.value = colorkey_min;

        s_lcd_osd_spec.osd_format               = LCDC_OSD_FORMAT_RGB565;

        s_lcd_osd_spec.alpha                    = o_spec_ptr->alpha;
        s_lcd_osd_spec.colorkey_enable          = o_spec_ptr->colorkey_enable;
        
        DTLCMDQ_LCDC_Set_Layer_Spec((KDuint8*)&s_lcd_osd_spec,LCDC_LAYER_OSD);
        
        DTLCMDQ_LCDC_Layer_Enable(LCDC_LAYER_OSD,KD_TRUE);
    }
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
    LCDC_CMD_SPEC_T* lcd_spec_ptr = LCD_GetLCDTVSpec();
    
    kdAssert(PNULL!= lcd_spec_ptr);
    
    lcd_spec_ptr->color_correction_enable = color_correction_enable;
    
    lcd_spec_ptr->color_coeff             = *(LCDC_COLOR_CORRCTION_T*)c_correction_ptr;
    
    DTLCMDQ_LCDC_Set_Spec((LCDC_CMD_SPEC_T*)lcd_spec_ptr);
    DTLCMDQ_LCDC_Enable(KD_TRUE);

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
    return s_lcd_need_rest;
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
    s_lcd_need_rest = need_reset;
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
    KDuint16 i;
    KDuint16 gamma_table[17];
    DTL_CMDQ_CMD_T cmd;
    
    cmd.cmd_id = ARM_CMDQ_LCDC_SET_GAMMA_TABLE;
    
    kdMemcpy((KDuint8*)gamma_table,(KDuint8*)gamma_table_ptr,sizeof(KDuint16)*17);
    
    for(i=0; i<17; i++)
    {
        INT_SWAP(gamma_table[i]);
    }
    
    DTL_SendCMD_And_MsgParam((uint8*)gamma_table, sizeof(gamma_table), &cmd);
    
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
    is_enable_invalidate = is_enable;
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
    return is_enable_invalidate;
}

#ifdef _SC6600V_WQVGA_
//-------------------------------------------------------------------------------------------------
//--Note:
//--    Added by miller.song at 2008-09-27 for wqvga_mp4 projection.
//--    Only used for mpeg4 decoder, refer to player_display.c in DV module.
//-------------------------------------------------------------------------------------------------
LOCAL ERR_LCD_E LCD6600V_Init_MP4(void)
{
    uint32 init_pos[2] = {0};
    LCD_SPEC_INFO_T_PTR lcd_spec_ptr = PNULL;

    //SCI_TraceLow:"LCDTV_Init,in"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,LCD_TV_1804_112_2_17_23_9_12_33,(uint8*)"");


#ifndef LCDTV_NOLCM
    lcd_spec_ptr = LCM_GetLcdSpecInfo(MAIN_LCD_ID);
    s_m_lcd_width = lcd_spec_ptr->width;
    s_m_lcd_height = lcd_spec_ptr->height;
/*   
    DTLCMDQ_LCDC_Set_Spec(LCD_GetLCDTVSpec());
    DTLCMDQ_LCDC_Enable(TRUE);

    LCD_Getcoeff();
    DTL_SendData((uint8*)&scaler_coeff, sizeof(LCDC_VIDEO_SCALLING_T), (LCDTV_OSD_BASE_QUEUE_ADDR-sizeof(LCDC_VIDEO_SCALLING_T)));
    DTLCMDQ_LCDC_Set_Layer_Spec((uint8*)(LCD_GetLCDTVVideoLayerSpec()),LCDC_LAYER_VIDEO);
    
    DTLCMDQ_LCDC_Set_Layer_Spec((uint8*)(LCD_GetLCDTVOSDLayerSpec()),LCDC_LAYER_OSD);

    DTLCMDQ_LCDC_Layer_Enable(LCDC_LAYER_VIDEO,TRUE);
    DTLCMDQ_LCDC_Layer_Enable(LCDC_LAYER_OSD,TRUE);
*/
/*
    //clean up read write position
    DTL_SendData((uint8*)&init_pos,2*sizeof(uint32),LCDTV_OSD_BASE_QUEUE_ADDR );
    DTL_SendData((uint8*)&init_pos,2*sizeof(uint32),LCDTV_OSD_PAD_QUEUE_ADDR);
*/
    DTL_SendData((uint8*)&init_pos,2*sizeof(uint32),OSD_QUEUE_BUFFER_ADDR );

    //创建锁
    if ( s_lcdtv_mutex_ptr )
    {
        SCI_ASSERT(SCI_SUCCESS == SCI_DeleteMutex(s_lcdtv_mutex_ptr));
        s_lcdtv_mutex_ptr = PNULL;
    }
    s_lcdtv_mutex_ptr = SCI_CreateMutex("lcdtv_mutex", SCI_INHERIT);
    SCI_ASSERT(PNULL != s_lcdtv_mutex_ptr);

    
#endif    
    return ERR_LCD_NONE;
}


LOCAL ERR_LCD_E LCD6600V_InvalidateRect_MP4(
                                   LCD_ID_E lcd_id,//ID of lcd to operate
                                   uint16 left,     //left of the rectangle
                                   uint16 top,     //top of the rectangle
                                   uint16 right,     //right of the rectangle
                                   uint16 bottom    //bottom of the rectangle.
                                   )
{
#ifndef LCDTV_NOLCM     //  #if 0//
    BOOLEAN is_osd_base_que = FALSE;
    uint32  osd_que_addr = 0;
    uint32  write_pos    = 0;
    uint32  write_pos_addr    = 0;
    LCDTV_OSD_COORDINATE_T osd_coor = {0};
    uint16* rotate_buf_ptr = PNULL;
    uint16 *buf_ptr = (uint16 *)MEM_GetMainLCDBuffer();
    
    uint32 buf_len = ((bottom-top+1) * (right-left+1))<<1;

    //SCI_TraceLow:"LCDTV_InvalidateRect,in,(left,top)=(%d,%d),(right,bottom)=(%d,%d),lcd(w,h)=(%d,%d)"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,LCD_TV_1863_112_2_17_23_9_12_34,(uint8*)"dddddd",left,top,right,bottom,s_m_lcd_width,s_m_lcd_height);
    
/*
    is_osd_base_que = LCD_IsOSDBaseQueueType(left,top,right,bottom);

    if ( is_osd_base_que )
    {
        osd_que_addr = LCDTV_OSD_BASE_QUEUE_ADDR;
    }
    else
    {
        osd_que_addr = LCDTV_OSD_PAD_QUEUE_ADDR;
    }
*/
    is_osd_base_que = SCI_FALSE;
    osd_que_addr = OSD_QUEUE_BUFFER_ADDR;

    //获取锁 进入临界区
    SCI_GetMutex (s_lcdtv_mutex_ptr, SCI_WAIT_FOREVER);
    
    write_pos = LCD_GetOSDWriteBufPos(osd_que_addr);
    SCI_ASSERT( LCDC_OSD_BUF_NUM > write_pos);
    write_pos_addr = osd_que_addr;

    osd_que_addr += 2*sizeof(uint32) + write_pos*sizeof(LCDC_OSD_PAD_T);
    if ( !is_osd_base_que )
    {
        osd_coor.start.mBits.start_x = left;
        osd_coor.start.mBits.start_y = top;
        osd_coor.end.mBits.end_x     = right;
        osd_coor.end.mBits.end_y     = bottom;
        
        DTL_SendData((uint8*)&osd_coor,sizeof(LCDTV_OSD_COORDINATE_T),osd_que_addr);
    }

    osd_que_addr += sizeof(LCDTV_OSD_COORDINATE_T);
    if ( LCDC_ANGLE_0 == LCD_GetAngle())
    {            
        if ( (right-left+1) != s_m_lcd_width )
        {
            LCDTV_GetRectBuf(left,top,right,bottom);
            DTL_SendData((uint8*)s_lcd_ref_buffer,buf_len,osd_que_addr);
        }
        else
        {
            buf_ptr += top * s_m_lcd_width;
            DTL_SendData((uint8*)buf_ptr,buf_len,osd_que_addr);
        }
    }
    else
    {
        rotate_buf_ptr = (uint16*)SCI_ALLOC(buf_len*sizeof(uint8));
        SCI_ASSERT( PNULL!= rotate_buf_ptr );
        
        LCD_GetRotatedRectBuf(rotate_buf_ptr,buf_len,left,top,right,bottom,LCD_GetAngle());
        DTL_SendData((uint8*)rotate_buf_ptr,buf_len,osd_que_addr);
        SCI_FREE(rotate_buf_ptr);
    }
    DTL_SendData((uint8*)&write_pos,sizeof(uint32),write_pos_addr);
/*    
    if ( is_osd_base_que )
    {
        DTLCMDQ_LCDC_Display_OSD();
    }
    else*/
    {
        DTLCMDQ_LCDC_Update_OSD();
    }

    //释放锁 出临界区
    SCI_PutMutex(s_lcdtv_mutex_ptr);
#endif    
    return ERR_LCD_NONE;
}


LOCAL ERR_LCD_E LCD6600V_Invalidate_MP4(
                                  LCD_ID_E lcd_id    //ID of lcd to operate
                                  )
{
    LCD_SPEC_INFO_T_PTR lcd_spec_ptr = PNULL;

#ifndef LCDTV_NOLCM
    uint32 write_pos = 0;
    uint32 write_pos_addr = 0;
    uint32 osd_que_addr = OSD_QUEUE_BUFFER_ADDR;
    uint16 *buf_ptr = (uint16 *)MEM_GetMainLCDBuffer();
    uint32 buf_len = (s_m_lcd_width * s_m_lcd_height)<<1;
    
    lcd_spec_ptr = LCM_GetLcdSpecInfo(MAIN_LCD_ID);
    //s_m_lcd_width = lcd_spec_ptr->width;
    //s_m_lcd_height = lcd_spec_ptr->height;
    
    //SCI_TraceLow:"LCD_Invalidate_6600V_MP4,lcd_spec,(w,h)=(%d,%d)"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,LCD_TV_1954_112_2_17_23_9_12_35,(uint8*)"dd",lcd_spec_ptr->width,lcd_spec_ptr->height);

    return LCD6600V_InvalidateRect_MP4(MAIN_LCD_ID,0,0,lcd_spec_ptr->width-1,lcd_spec_ptr->height-1);
    
/*
    //获取锁 进入临界区
    SCI_GetMutex (s_lcdtv_mutex_ptr, SCI_WAIT_FOREVER);

    write_pos = LCD_GetOSDWriteBufPos(osd_que_addr);
    SCI_ASSERT( LCDC_OSD_BUF_NUM != write_pos);
    
    write_pos_addr = osd_que_addr;

    osd_que_addr += 2*sizeof(uint32) + write_pos*sizeof(LCDC_OSD_PAD_T);
    osd_que_addr += sizeof(LCDTV_OSD_COORDINATE_T);

    DTL_SendData((uint8*)buf_ptr,buf_len,osd_que_addr);
    
    DTL_SendData((uint8*)&write_pos,sizeof(uint32),write_pos_addr);
    DTLCMDQ_LCDC_Update_OSD();

    //释放锁 出临界区
    SCI_PutMutex(s_lcdtv_mutex_ptr);
*/
#endif
    return ERR_LCD_NONE;

}
#endif
