/*****************************************************************************
** File Name:      mmipic_crop.c                                                  *
** Author:          chunyou                                                        *
** Date:             12/25/2011                                                *
** Copyright:      2011 Spreadtrum, Incorporated. All Rights Reserved.       *
** Description:    This file is used to describe pic crop                  *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 12/2011       chunyou         Create                                  *
******************************************************************************/


/**--------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/
#include "mmi_app_pic_crop_trc.h"
#ifdef WIN32
#include "std_header.h"
#endif

#ifdef MMI_PIC_CROP_SUPPORT
#include "os_api.h"
#include "mmipic_crop.h"
#include "mmifmm_id.h"
#include "mmifmm_export.h"
#include "mmidisplay_data.h"
#include "mmipic_crop_id.h"
#include "mmipic_crop_text.h"
#include "mmipic_crop_image.h"
#include "mmipic_crop_nv.h"
#include "mmipic_crop_export.h"
#include "mmi_position.h"
#include "guianim.h"
#include "mmi_text.h"
#include "mmi_image.h"
#include "mmipub.h"
#include "mmitheme_anim.h"
#include "mmi_appmsg.h"
#include "types.h"
#include "stdio.h"
#include "guiimg.h"
#include "guistring.h"
#include "guires.h"
#include "img_dec_interface.h"
#include "mmiptscr_save.h"
#include "guibutton.h"

/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/

#define  ROTATE_ICON_NUMBER      MMI_ROTATION_DEGREE_MAX
#define MMIPIC_ICON_BG_COLOR              0xc6df  //icon bg color
#define MMIPIC_MIN_UNIT       8

#define MMIPICCROP_FILE_NAME_ID_MAX				    10000000

//存放路径文件夹"PICCROP" 
#define MMIPICCROP_RESERVED_DIR                 'P', 'I', 'C', 'C', 'R', 'O' ,'P'
#define MMIPICCROP_RESERVED_DIR_LEN            7
//#define MMIPICCROP_WALLPAPER_NAME                (s_piccrop_wallpaper_name)//KM_res
#define MMIPICCROP_WALLPAPER_NAME_LEN            2

#define KM_POS_BIAS '\\'   //"\x00\x2f" // "\" 反斜线
#define KM_POS_DIOT '.'   //'.' 文件后缀名

//#define MMIPIC_FILEDATA_BUFFER_SIZE MMIPIC_PROCESS_BUFFER_SIZE  // (MMI_MAINSCREEN_WIDTH*MMI_MAINSCREEN_HEIGHT*4) to width*height*3+72K

#ifndef RGB_MAKE
#define RGB_MAKE(b, g, r)       ((uint16) (((r) << 16) | ((g) << 8) | (b)))
#endif
#define LOOP_565_TO_555(_565)                       \
{                                                       \
    uint32 _555 = 0;                                    \
    _555 = (((_565) >> 1) & 0x7FE0) | ((_565) & 0x001F);\
    (_565) = _555;                                      \
}
/*---------------------------------------------------------------------------*/
/*                          TYPE AND CONSTANT                                */
/*---------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
**                         EXTERNAL DECLARE                                 *
**--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
**                         GLOBAL DEFINITION                                *
**--------------------------------------------------------------------------*/
//LOCAL const wchar s_piccrop_reserved_dir[]        = { 'P', 'I', 'C', 'C', 'R', 'O' ,'P' ,0 }; 
//LOCAL const wchar s_piccrop_wallpaper_name[]        = { 'w', 'p', 0 }; 
const wchar g_mmipiccrop_save_subfix_wstr[] = { '.', 'j', 'p', 'g', 0 };//MMIPIC_SAVE_SUBFIX      "\x00\x2e\x00\x6a\x00\x70\x00\x67"      // pic文件后缀名.jpg
//lijinghua add for pic crop
LOCAL GUI_RECT_T		left_shadow_display_rect={0};
LOCAL GUI_RECT_T		top_shadow_display_rect={0};
LOCAL GUI_RECT_T		right_shadow_display_rect={0};
LOCAL GUI_RECT_T		bottom_shadow_display_rect={0};
LOCAL GUI_RECT_T		top_rect = {0};
LOCAL GUI_RECT_T		bottom_rect = {0};
LOCAL GUI_RECT_T		left_rect = {0};
LOCAL GUI_RECT_T		right_rect = {0};
LOCAL uint16 		    crop_index=0;
LOCAL uint16 		    square_crop_index=0;
LOCAL uint16 		    landscape_crop_index=0;
LOCAL int16  s_icon_move_up_or_down_crop_index=0;
LOCAL int16  s_icon_move_left_or_right_crop_index=0;
LOCAL BOOLEAN s_icon_is_square_crop = FALSE;
LOCAL BOOLEAN s_icon_is_landscape_crop = FALSE;
LOCAL GUI_RECT_T				 clip_rect_ptr={0};             //lijinghua add it for PB

//lijinghua add end

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          CONSTANT VARIABLES                               */
/*---------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
**                         FUNCTION DEFINITION                              *
**--------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description : init pic crop
//  Global resource dependence : 
//  Author: chunyou
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN CropIsSupportImgType (GUIANIM_TYPE_E anim_type)
{
    BOOLEAN             result = FALSE;
    
    switch (anim_type)
    {
    case GUIANIM_TYPE_IMAGE_JPG:
        result = TRUE;
        break;
        
    default:
        break;
    }

    return result;
}
/*****************************************************************************/
//  Description : init pic crop
//  Global resource dependence : 
//  Author: chunyou
//  Note: 
/*****************************************************************************/
PUBLIC MMI_PICCROP_ERROR_TYPE_E MMIPICCROP_Init (MMIPICCROP_SRC_T* crop_src)
{
    MMI_PICCROP_ERROR_TYPE_E err_type = MMI_PICCROP_ERR_EXIT;
    uint16  lcd_width = 0;
    uint16  lcd_height = 0;
    uint16 img_width = 0;
    uint16 img_height = 0;
    MN_RETURN_RESULT_E return_value = MN_RETURN_FAILURE;
    MMIPICCROP_INFO_T* crop_info_ptr = MMIPICCROP_GetCropInfoPtr();

    if(PNULL == crop_info_ptr)
    {
        //SCI_TRACE_LOW:"[MMIPICCROP] MMIPICCROP_Init crop_info_ptr is PNULL !!!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIC_CROP_130_112_2_18_2_46_11_0,(uint8*)"");
        return err_type;
    }
    if (PNULL == crop_src)
    {
        //SCI_TRACE_LOW:"[MMIPICCROP] MMIPICCROP_Init crop_src is PNULL !!!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIC_CROP_135_112_2_18_2_46_11_1,(uint8*)"");
        return err_type;
    }     
    //SCI_TRACE_LOW:"[MMIPICCROP]  MMIPICCROP_Init"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIC_CROP_138_112_2_18_2_46_11_2,(uint8*)"");

    do
    {
        SCI_MEMSET(crop_info_ptr, 0, sizeof(MMIPICCROP_INFO_T));

        MMINV_READ(MMINV_PICCROP_SETTING, &crop_info_ptr->setting_info, return_value);
        if (MN_RETURN_SUCCESS != return_value)
        {
            crop_info_ptr->setting_info.crop_file_id = 0;
            MMINV_WRITE(MMINV_PICCROP_SETTING, &crop_info_ptr->setting_info);
        }
    
        crop_info_ptr->crop_type = crop_src->crop_type;
        crop_info_ptr->callback_fun = crop_src->callback;

        crop_info_ptr->min_width= crop_src->min_width;
        crop_info_ptr->min_height= crop_src->min_height;
        crop_info_ptr->is_pre_wallpaper = crop_src->is_pre_wallpaper;

        if(crop_info_ptr->min_width == 0 || crop_info_ptr->min_height == 0)
        {
            GUIRES_GetImgWidthHeight(&img_width, &img_height, IMAGE_PICCROP_POINT, MMIPIC_CROP_MAIN_WIN_ID);
            crop_info_ptr->min_width = crop_info_ptr->frame_img_width + MMIPICCROP_MOVE_POINT_OFFSET*2;
            crop_info_ptr->min_height = crop_info_ptr->frame_img_width + MMIPICCROP_MOVE_POINT_OFFSET*2;
        }

        if(PNULL != crop_src->full_path_ptr && crop_src->full_path_len > 0)
        {            
            crop_info_ptr->is_file = TRUE;
            //file info
            crop_info_ptr->file_info.full_path_wstr_ptr = SCI_ALLOCA(sizeof(wchar)*(MMIFILE_FILE_NAME_MAX_LEN+1));
            if (PNULL == crop_info_ptr->file_info.full_path_wstr_ptr)
            {
                //SCI_TRACE_LOW:"[MMIPICCROP] MMIPICCROP_Init crop_info_ptr->file_info.full_path_wstr_ptr is PNULL"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIC_CROP_172_112_2_18_2_46_11_3,(uint8*)"");
                err_type = MMI_PICCROP_ERR_NO_MEMORY;
                break;
            }
            SCI_MEMSET(crop_info_ptr->file_info.full_path_wstr_ptr, 0, (sizeof(wchar)*(MMIFILE_FILE_NAME_MAX_LEN+1)));
            MMI_WSTRNCPY(crop_info_ptr->file_info.full_path_wstr_ptr, sizeof(wchar)*(crop_src->full_path_len+1),
                crop_src->full_path_ptr, crop_src->full_path_len,
                crop_src->full_path_len);
            crop_info_ptr->file_info.full_path_wstr_len = crop_src->full_path_len;

            if(!CropIsSupportImgType(MMIAPICOM_GetImgaeType(crop_src->full_path_ptr, crop_src->full_path_len)))
            {
                err_type = MMI_PICCROP_ERR_NOT_SUPPORT;
                break;
            }
        }
        else if(PNULL != crop_src->src_buf_ptr && crop_src->src_data_size > 0)
        {
            GUIRES_IMG_SRC_T src_data = {0};
            GUIRES_IMG_INFO_T img_info = {0};

            crop_info_ptr->is_file = FALSE;
            crop_info_ptr->src_buf_ptr = crop_src->src_buf_ptr;
            crop_info_ptr->src_data_size = crop_src->src_data_size;
            
            src_data.data_ptr = crop_src->src_buf_ptr;
            src_data.data_size = crop_src->src_data_size;
            GUIRES_GetImgInfo(&src_data, &img_info);
            if(!CropIsSupportImgType(img_info.img_type))
            {
                err_type = MMI_PICCROP_ERR_NOT_SUPPORT;
                break;
            }            
        }
        else
        {
            //SCI_TRACE_LOW:"[MMIPICCROP] MMIPICCROP_Init crop_src is error !!!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIC_CROP_208_112_2_18_2_46_11_4,(uint8*)"");
            break;
        }

        crop_info_ptr->random_name_ptr = SCI_ALLOCA(sizeof(wchar)*(MMIFILE_FILE_NAME_MAX_LEN+1));
        if (PNULL == crop_info_ptr->random_name_ptr)
        {
            //SCI_TRACE_LOW:"[MMIPICCROP] MMIPICCROP_Init crop_info_ptr->random_name_ptr is PNULL"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIC_CROP_215_112_2_18_2_46_11_5,(uint8*)"");
            err_type = MMI_PICCROP_ERR_NO_MEMORY;
            break;
        }
        SCI_MEMSET(crop_info_ptr->random_name_ptr, 0, (sizeof(wchar)*(MMIFILE_FILE_NAME_MAX_LEN+1)));
        crop_info_ptr->random_name_len = 0;
        
        crop_info_ptr->lcd_dev.lcd_id=GUI_MAIN_LCD_ID;
        crop_info_ptr->lcd_dev.block_id=GUI_BLOCK_MAIN;     

        GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID,&lcd_width,&lcd_height);
        crop_info_ptr->display_buffer_size = lcd_width * (lcd_height - MMIPICCROP_TITLE_HEIGHT - MMIPICCROP_BOTTOM_HEIGHT) * sizeof(GUI_COLOR_T);
        //crop_info_ptr->process_buffer_size = lcd_width * lcd_height * 3 + MMIPICCROP_PROCESS_BUFFER_FIXED_SIZE;
        //crop_info_ptr->jpg_buffer_size = lcd_width * lcd_height * sizeof(GUI_COLOR_T);
        //crop_info_ptr->output_buffer_size = lcd_width * lcd_height * sizeof(GUI_COLOR_T);
        err_type = MMI_PICCROP_SUCCESS;
    }while(0);    

    return err_type;
}

/*****************************************************************************/
//  Description : get crop client rect
//  Global resource dependence : 
//  Author: chunyou
//  Note:
/*****************************************************************************/
PUBLIC GUI_RECT_T MMIPICCROP_GetClientRect(void)
{
    GUI_RECT_T			client_rect = {0};
    uint16  lcd_width = 0;
    uint16  lcd_height = 0;

    GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID,&lcd_width,&lcd_height);

    client_rect.left = 0;
    client_rect.top = MMIPICCROP_TITLE_HEIGHT;
    client_rect.right = lcd_width - 1;
    client_rect.bottom = lcd_height - 1 - MMIPICCROP_BOTTOM_HEIGHT;

    return (client_rect);
}

/*****************************************************************************/
//  Description : get crop client rect height
//  Global resource dependence : 
//  Author: chunyou
//  Note:
/*****************************************************************************/
PUBLIC uint16 MMIPICCROP_GetClientRectHeight(void)
{
    uint16  lcd_width = 0;
    uint16  lcd_height = 0;

    GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID,&lcd_width,&lcd_height);

    return (lcd_height - MMIPICCROP_TITLE_HEIGHT - MMIPICCROP_BOTTOM_HEIGHT);
}

/*****************************************************************************/
//  Description : get image width and height by image path
//  Global resource dependence : 
//  Author: chunyou
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN GetImgInfoByPath(
                                 wchar* full_path_wstr_ptr,     //in:
                                 GUIIMG_INFO_T  *img_info_ptr   //in/out:
                                 )
{
    BOOLEAN             result = TRUE;
    IMG_DEC_RET_E       get_result = IMG_DEC_RET_SUCCESS;
    IMG_DEC_SRC_T       dec_src = {0};
    IMG_DEC_INFO_T      dec_info = {0}; /*lint !e64*/
    
    if(PNULL == img_info_ptr)
    {
        return FALSE;
    }
    
    //set decode src info
    dec_src.file_name_ptr = full_path_wstr_ptr;  /*lint !e413*/

    //get image info
    get_result = IMG_DEC_GetInfo(&dec_src,&dec_info);
    //SCI_TRACE_LOW:"GetImgInfoByPath:get info result is %d!"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIC_CROP_294_112_2_18_2_46_11_6,(uint8*)"d",get_result);
    if (IMG_DEC_RET_SUCCESS == get_result)
    {
        //set image info
        switch (dec_info.img_type)
        {
        case IMG_DEC_TYPE_BMP:
            img_info_ptr->image_width  = dec_info.img_detail_info.bmp_info.img_width;
            img_info_ptr->image_height = dec_info.img_detail_info.bmp_info.img_height;
            break;

        case IMG_DEC_TYPE_WBMP:
            img_info_ptr->image_width  = dec_info.img_detail_info.wbmp_info.img_width;
            img_info_ptr->image_height = dec_info.img_detail_info.wbmp_info.img_height;
            break;

        case IMG_DEC_TYPE_JPEG:
            img_info_ptr->image_width  = dec_info.img_detail_info.jpeg_info.img_width;
            img_info_ptr->image_height = dec_info.img_detail_info.jpeg_info.img_heigth;
            break;

        case IMG_DEC_TYPE_PNG:
            img_info_ptr->image_width  = dec_info.img_detail_info.png_info.img_width;
            img_info_ptr->image_height = dec_info.img_detail_info.png_info.img_height;
            break;

        case IMG_DEC_TYPE_GIF:
            img_info_ptr->image_width  = dec_info.img_detail_info.gif_info.img_width;
            img_info_ptr->image_height = dec_info.img_detail_info.gif_info.img_height;
            break;

        case IMG_DEC_TYPE_UNKNOWN:
            break;
        
        default:
            result = FALSE;
            break;
        }
    }
    else
    {
        result = FALSE;
    }
    
    return (result);
}

/*****************************************************************************/
//  Description : Decode jpg image
//  Global resource dependence : 
//  Author: chunyou
//  Note: 
/*****************************************************************************/
PUBLIC MMI_PICCROP_ERROR_TYPE_E MMIPICCROP_DecodeJpgImage(
                                                             void
                                                             )
{
    BOOLEAN             result = FALSE;
    MMI_PICCROP_ERROR_TYPE_E err_type = MMI_PICCROP_ERR_EXIT;
    uint32              buf_size = 0;
    GUIIMG_SRC_T        img_src = {0};
    GUIIMG_DISPLAY_T    img_display = {0};
    GUIIMG_DEC_OUT_T    img_output = {0};    
    uint16  lcd_width = 0;
    uint16  lcd_height = 0;
    GUI_RECT_T      display_rect = MMIPICCROP_GetClientRect();//MMITHEME_GetClientRectEx(MMIPIC_CROP_MAIN_WIN_ID);
    uint16          client_height = MMIPICCROP_GetClientRectHeight();//MMITHEME_GetClientRectHeightEx(MMIPIC_CROP_MAIN_WIN_ID);
    MMIPICCROP_INFO_T* crop_info_ptr = MMIPICCROP_GetCropInfoPtr();

    if(PNULL == crop_info_ptr)
    {
        //SCI_TRACE_LOW:"[MMIPICCROP] MMIPICCROP_DecodeJpgImage crop_info_ptr is PNULL !!!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIC_CROP_363_112_2_18_2_46_11_7,(uint8*)"");
        err_type = MMI_PICCROP_ERR_EXIT;
        return err_type;
    }

    GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID,&lcd_width,&lcd_height);

    if(crop_info_ptr->is_file)
    {            
        if(!GetImgInfoByPath(crop_info_ptr->file_info.full_path_wstr_ptr, &crop_info_ptr->original_img))
        {
            //MMIPICCROP_ASSERT_WARNING:"[MMIPICCROP] MMIPICCROP_DecodeJpgImage GetImgInfoByPath is FALSE"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIC_CROP_374_112_2_18_2_46_11_8,(uint8*)"");
            err_type = MMI_PICCROP_ERR_EXIT;
            return err_type;
        }
        //set image full path
        img_src.is_file = TRUE;
        img_src.full_path_ptr = crop_info_ptr->file_info.full_path_wstr_ptr;
    }
    else
    {
        GUIRES_GetImgWidthHeightByPtr(crop_info_ptr->src_buf_ptr, &crop_info_ptr->original_img.image_width, &crop_info_ptr->original_img.image_height, crop_info_ptr->src_data_size);
        img_src.src_buf_ptr = crop_info_ptr->src_buf_ptr;
        img_src.src_data_size = crop_info_ptr->src_data_size;
    }    

    //set image dest width and height
    img_display.dest_width = lcd_width;
    img_display.dest_height = MMIPICCROP_GetClientRectHeight();

    //set image decode output data buffer
    img_output.decode_data_ptr = (uint8*)SCI_ALLOCA(crop_info_ptr->display_buffer_size);
    if (PNULL == img_output.decode_data_ptr)
    {
        //MMIPICCROP_ASSERT_WARNING:"[MMIPICCROP] MMIPICCROP_DecodeJpgImage img_output.decode_data_ptr is PNULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIC_CROP_397_112_2_18_2_46_11_9,(uint8*)"");
        err_type = MMI_PICCROP_ERR_NO_MEMORY;
        return err_type;
    }
    img_output.decode_data_size = crop_info_ptr->display_buffer_size;
    
    //decode jpg
    result = GUIIMG_Decode(&img_src,
                &img_display,
                &img_output);
    
    if (result)
    {
        do
        {
            //alloc bitmap
            if (PNULL == crop_info_ptr->display_bitmap_ptr)
            {
                crop_info_ptr->display_bitmap_ptr = SCI_ALLOCA(sizeof(GUIIMG_BITMAP_T));
            }
            if (PNULL == crop_info_ptr->display_bitmap_ptr)
            {
                //MMIPICCROP_ASSERT_WARNING:"[MMIPICCROP] MMIPICCROP_DecodeJpgImage s_pic_editor_info.display_bitmap_ptr is PNULL"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIC_CROP_419_112_2_18_2_46_11_10,(uint8*)"");
                err_type = MMI_PICCROP_ERR_NO_MEMORY;
                break;
            }
            SCI_MEMSET(crop_info_ptr->display_bitmap_ptr,0,sizeof(GUIIMG_BITMAP_T));

            //alloc bitmap vpbits
            if (PNULL == crop_info_ptr->display_bitmap_ptr->bit_ptr)
            {
                crop_info_ptr->display_bitmap_ptr->bit_ptr = (uint8*)SCI_ALLOCA(crop_info_ptr->display_buffer_size);
            }
            if (PNULL == crop_info_ptr->display_bitmap_ptr->bit_ptr)
            {
                //MMIPICCROP_ASSERT_WARNING:"[MMIPICCROP] MMIPICCROP_DecodeJpgImage s_pic_editor_info.display_bitmap_ptr->bit_ptr is PNULL"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIC_CROP_432_112_2_18_2_46_11_11,(uint8*)"");
                err_type = MMI_PICCROP_ERR_NO_MEMORY;
                break;
            }
            buf_size = crop_info_ptr->display_buffer_size;
            SCI_MEMSET(crop_info_ptr->display_bitmap_ptr->bit_ptr,0,buf_size);

            //copy vpbits
            MMI_MEMCPY(crop_info_ptr->display_bitmap_ptr->bit_ptr,buf_size,
                img_output.decode_data_ptr,img_output.decode_data_size,
                img_output.decode_data_size);

            //set others param
            crop_info_ptr->display_bitmap_ptr->img_width = img_output.actual_width;
            crop_info_ptr->display_bitmap_ptr->img_height = img_output.actual_height;
            
            crop_info_ptr->display_rect.left   = (lcd_width - img_output.actual_width)/2;
            crop_info_ptr->display_rect.top    = display_rect.top + (int16)(client_height - img_output.actual_height)/2;
            crop_info_ptr->display_rect.right  = crop_info_ptr->display_rect.left + (int16)img_output.actual_width -1;
            crop_info_ptr->display_rect.bottom = crop_info_ptr->display_rect.top + (int16)img_output.actual_height -1;
            err_type = MMI_PICCROP_SUCCESS;
            MMK_PostMsg(MMIPIC_CROP_MAIN_WIN_ID,MSG_PIC_DISPLAY_IND,PNULL,0);
        }while(0);
    }

    //free vpbits
    if (PNULL != img_output.decode_data_ptr)
    {
        SCI_FREE(img_output.decode_data_ptr);
        img_output.decode_data_ptr = PNULL;
    }

    //SCI_TRACE_LOW:"[MMIPICCROP] MMIPICCROP_DecodeJpgImage err_type = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIC_CROP_464_112_2_18_2_46_11_12,(uint8*)"d", err_type);
    return (err_type);
}

/*****************************************************************************/
//  Description : get crop image data
//  Global resource dependence : 
//  Author: chunyou
//  Note: 
/*****************************************************************************/
LOCAL GUI_RECT_T CropGetOriginalImageRect(void)
{
    GUI_RECT_T crop_rect = {0};
    GUI_RECT_T dest_rect = {0};
    uint16 width = 0;
    uint16 height = 0;
    MMIPICCROP_INFO_T* crop_info_ptr = MMIPICCROP_GetCropInfoPtr();

    if(PNULL == crop_info_ptr)
    {
        //SCI_TRACE_LOW:"[MMIPICCROP] CropGetOriginalImageRect crop_info_ptr is PNULL !!!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIC_CROP_482_112_2_18_2_46_11_13,(uint8*)"");
        return crop_rect;
    }

    if(GUI_IntersectRect(&dest_rect, crop_info_ptr->crop_frame_rect, crop_info_ptr->display_rect))
    {
    //zhuyongjing add for pb Bug 509901
        if(crop_info_ptr->crop_type == MMIPIC_CROP_PB){
            uint16 x_length =  (uint16)((dest_rect.right - dest_rect.left + 1) * crop_info_ptr->original_img.image_width/(crop_info_ptr->display_rect.right - crop_info_ptr->display_rect.left + 1));
            uint16 y_length =  (uint16)((dest_rect.bottom - dest_rect.top + 1) * crop_info_ptr->original_img.image_height/(crop_info_ptr->display_rect.bottom - crop_info_ptr->display_rect.top + 1));
            uint16 length = MIN(x_length,y_length);

            crop_rect.left = (uint16)((dest_rect.left - crop_info_ptr->display_rect.left + 1) * crop_info_ptr->original_img.image_width / (crop_info_ptr->display_rect.right - crop_info_ptr->display_rect.left + 1));
            crop_rect.top =  (uint16)((dest_rect.top - crop_info_ptr->display_rect.top + 1) *crop_info_ptr->original_img.image_width / (crop_info_ptr->display_rect.right - crop_info_ptr->display_rect.left + 1));
            crop_rect.right = crop_rect.left + length - 1;
            crop_rect.bottom = crop_rect.top + length - 1;
        //add end
        }else{
            width = (uint16)((dest_rect.right - dest_rect.left + 1) * crop_info_ptr->original_img.image_width / (crop_info_ptr->display_rect.right - crop_info_ptr->display_rect.left + 1));
            height = (uint16)((dest_rect.bottom - dest_rect.top + 1) * crop_info_ptr->original_img.image_height / (crop_info_ptr->display_rect.bottom - crop_info_ptr->display_rect.top + 1));

            crop_rect.left = (uint16)((dest_rect.left - crop_info_ptr->display_rect.left + 1) * crop_info_ptr->original_img.image_width / (crop_info_ptr->display_rect.right - crop_info_ptr->display_rect.left + 1));
            crop_rect.top = (uint16)((dest_rect.top - crop_info_ptr->display_rect.top + 1) * crop_info_ptr->original_img.image_width / (crop_info_ptr->display_rect.right - crop_info_ptr->display_rect.left + 1));
            crop_rect.right = crop_rect.left + width - 1;
            crop_rect.bottom = crop_rect.top + height - 1;
        }
    } 
    
    return crop_rect;
}

/*****************************************************************************/
// 	Description : get file id string
//	Global resource dependence : none
//   Author: chunyou
//	Note:
/*****************************************************************************/
LOCAL uint16 CropGetFileIDString(char* file_name, const char* prefix, uint32 id, uint32 id_max, const char* file_type)
{
    uint16 id_len = 0;
    uint16 id_max_len = 0;
    char string_max[20] = {0};
    uint32 i = 0;
 
    if(id >= id_max)
    {
        //SCI_TRACE_LOW:"[MMIPICCROP] CropGetFileIDString id >= id_max"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIC_CROP_513_112_2_18_2_46_12_14,(uint8*)"");
        id = id_max - 100;
    }

    id_len = sprintf(file_name, "%d", id);
    id_max_len = sprintf(string_max, "%d", id_max);
    for(i = 0; i < id_len; i++)
    {
        string_max[id_max_len - id_len + i] = file_name[i];
    }
    string_max[0] = '_';
    string_max[id_max_len] = '.';
    id_len = sprintf(file_name, "%s%s%s", prefix, string_max, file_type);
    return id_len;
}

/*****************************************************************************/
//  Description : create save file name
//  Global resource dependence : 
//  Author: chunyou
//  Note: liyan.zhu modified the save path in10/31/2012
/*****************************************************************************/
LOCAL void CropCreateSaveFileName(void)
{
    const wchar*    file_dev_ptr = PNULL;
    uint16          file_dev_len = 0;
    char            temp_name[MMIFILE_FILE_NAME_MAX_LEN + 1] = {0};
    wchar          dest_name[MMIFILE_FILE_NAME_MAX_LEN + 1] = {0};
    uint32                circle = 1;
    register uint32       name_id = 0;
    register uint32       step = 1;
    register uint32       id_max = MMIPICCROP_FILE_NAME_ID_MAX;
    //wchar   syspath_name_ptr[] = {MMIMULTIM_DIR_SYSTEM_CHAR, '\\', MMIPICCROP_RESERVED_DIR};
    //uint16  syspath_name_len = (MMIMULTIM_DIR_SYSTEM_LEN+1+MMIPICCROP_RESERVED_DIR_LEN);
    MMIFILE_DEVICE_E    file_dev = MMI_DEVICE_SDCARD;
	wchar   *photopath_name_ptr  = PNULL;//根据需求修改piccrop后的地址
	uint16  photopath_name_len  = 0;
    wchar   piccroppath_name_ptr[] = {'\\', MMIPICCROP_RESERVED_DIR};
    MMIPICCROP_INFO_T* crop_info_ptr = MMIPICCROP_GetCropInfoPtr();

#ifdef CMCC_UI_STYLE	
	photopath_name_len   = MMIMULTIM_DIR_PHOTOS+1+MMIPICCROP_RESERVED_DIR_LEN;
#else	
	photopath_name_len   = MMIMULTIM_DIR_PIC_LEN+1+MMIPICCROP_RESERVED_DIR_LEN;
#endif
	//if (MMIFILE_FILE_NAME_MAX_LEN < photopath_name_len)
	//{
	//	SCI_TRACE_LOW("[mmipic_crop.c] CropCreateSaveFileName MMIFILE_FILE_NAME_MAX_LEN < photopath_name_len");
	//	MMIPUB_OpenAlertWarningWin(TXT_COM_FILE_NAME_TOO_LONG);
	//}
	
	photopath_name_ptr = SCI_ALLOC_APPZ(sizeof(wchar)*(MMIFILE_FILE_NAME_MAX_LEN + 1));
    
	if(PNULL == photopath_name_ptr)
	{
		SCI_TRACE_LOW("[mmipic_crop.c] CropCreateSaveFileName PNULL == photopath_name_ptr");
        return;
	}
#ifdef CMCC_UI_STYLE	
	MMIAPICOM_Wstrncpy(photopath_name_ptr,MMIMULTIM_DIR_PHOTOS,MMIMULTIM_DIR_PHOTOS);
#else	
	MMIAPICOM_Wstrncpy(photopath_name_ptr,MMIMULTIM_DIR_PICTURE,MMIMULTIM_DIR_PIC_LEN);
#endif	
	
	MMIAPICOM_Wstrncat(photopath_name_ptr,piccroppath_name_ptr,1+MMIPICCROP_RESERVED_DIR_LEN);

    if(PNULL == crop_info_ptr)
    {
        //SCI_TRACE_LOW:"[MMIPICCROP] CropCreateSaveFileName crop_info_ptr is PNULL !!!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIC_CROP_550_112_2_18_2_46_12_15,(uint8*)"");
		if (PNULL != photopath_name_ptr)
		{
			SCI_Free(photopath_name_ptr);
			photopath_name_ptr = PNULL;
		}
		return;
    }

    file_dev = MMIAPIFMM_GetDeviceTypeByPath(
                                        crop_info_ptr->file_info.full_path_wstr_ptr,
                                        MMIFILE_DEVICE_LEN
                                        );     
    MMIAPIFMM_GetSuitableFileDev(file_dev, crop_info_ptr->crop_bitmap_size, &file_dev);
    file_dev_len = MMIAPIFMM_GetDevicePathLen((file_dev));
    file_dev_ptr =  MMIAPIFMM_GetDevicePath((file_dev));
    crop_info_ptr->random_name_len = MMIFILE_FILE_NAME_MAX_LEN + 1;
            
    //if(MMIPIC_CROP_DEFAULT_SAVE == crop_info_ptr->crop_type)
    //{
        while(1)  /*lint !e716*/
        {
            name_id = crop_info_ptr->setting_info.crop_file_id;
            name_id += step;
            step = step * 2;
            if(name_id >= id_max)
            {
                circle ++;
                if(circle >= id_max)
                {
                    circle = 1;
                }
                name_id = circle;
                step = 1;
            }
            crop_info_ptr->setting_info.crop_file_id = name_id;

            SCI_MEMSET(crop_info_ptr->random_name_ptr, 0, sizeof(wchar)*(MMIFILE_FILE_NAME_MAX_LEN+1));
            SCI_MEMSET(temp_name, 0, sizeof(temp_name));
            SCI_MEMSET(dest_name, 0, sizeof(dest_name));
            
            CropGetFileIDString(temp_name, "CONTACT", name_id, id_max, MMIPICCROP_SAVE_FILE_TYPE);
            MMIAPICOM_StrToWstr(temp_name, dest_name);/*lint !e64*/       
            
            MMIAPIFMM_CombineFullPath(
                file_dev_ptr, 
                file_dev_len,
                photopath_name_ptr, 
                photopath_name_len,
                dest_name, 
                MMIAPICOM_Wstrlen(dest_name),
                crop_info_ptr->random_name_ptr,
                &crop_info_ptr->random_name_len
                );            
            
            if(!MMIAPIFMM_IsFileExist(crop_info_ptr->random_name_ptr, crop_info_ptr->random_name_len))                
            {
                break;
            }
        }
 /*   }
    else if(MMIPIC_CROP_WALLPAPER == crop_info_ptr->crop_type)
    {
        MMI_WSTRNCPY(dest_name,MMIPICCROP_WALLPAPER_NAME_LEN,
            MMIPICCROP_WALLPAPER_NAME,MMIPICCROP_WALLPAPER_NAME_LEN,
            MMIPICCROP_WALLPAPER_NAME_LEN);
        SCI_MEMSET(crop_info_ptr->random_name_ptr, 0, sizeof(wchar)*(MMIFILE_FILE_NAME_MAX_LEN+1));
        MMIAPIFMM_CombineFullPath(
                file_dev_ptr, 
                file_dev_len,
                syspath_name_ptr, 
                syspath_name_len,
                dest_name, 
                MMIAPICOM_Wstrlen(dest_name),
                crop_info_ptr->random_name_ptr,
                &crop_info_ptr->random_name_len
                );
        MMI_WSTRNCPY((crop_info_ptr->random_name_ptr+crop_info_ptr->random_name_len),(MMIFILE_FULL_PATH_MAX_LEN - crop_info_ptr->random_name_len),
            MMIPICCROP_SAVE_SUBFIX,MMIPICCROP_SAVE_SUBFIX_LEN,
            MMIPICCROP_SAVE_SUBFIX_LEN);
        crop_info_ptr->random_name_len += MMIPICCROP_SAVE_SUBFIX_LEN;
    }    */
	if (PNULL != photopath_name_ptr)
	{
		SCI_Free(photopath_name_ptr);
		photopath_name_ptr = PNULL;
	}
}

/*****************************************************************************/
//  Description : save image
//  Global resource dependence : 
//  Author: chunyou
//  Note: 
/*****************************************************************************/
PUBLIC MMI_PICCROP_ERROR_TYPE_E  MMIPICCROP_Save(void)
{
    //PEPROCESS_INPUT_T       input_param={0};
    //PEPROCESS_OUTPUT_T      output_param={0};
    //PEPROCESS_PARAMETER_T   process_param={0};        
    MMI_PICCROP_ERROR_TYPE_E     ret_value = MMI_PICCROP_SUCCESS; 
    //PEPROCESS_RESULT_E ref_return = PEPROCESS_OK;    
    //MMI_STRING_T  text_str ; 
    //wchar no_suffix_name[MMIFILE_FILE_NAME_MAX_LEN + 1] = {0};
    //uint16 no_suffix_name_len = 0;
    //wchar           device_name[MMIFILE_DEVICE_NAME_MAX_LEN+2] = {0};
    //uint16          device_name_len = 0;
    //wchar           dir_name[MMIFILE_DIR_NAME_MAX_LEN+2] = {0};
    //uint16          dir_name_len = 0;
    //wchar           new_full_path[MMIFILE_FULL_PATH_MAX_LEN+2] = {0};
    //uint16          new_full_path_len = MMIFILE_FULL_PATH_MAX_LEN+2;
    //wchar           new_file_name_ptr[MMIFILE_FILE_NAME_MAX_LEN + 1] = {0};
    //uint16          new_file_name_len = 0;
    //BOOLEAN     is_valid_name = TRUE;
    //MMIFILE_DEVICE_E dev = MMI_DEVICE_UDISK;
    MMIPICCROP_INFO_T* crop_info_ptr = MMIPICCROP_GetCropInfoPtr();

    if(PNULL == crop_info_ptr)
    {
        //SCI_TRACE_LOW:"[MMIPICCROP] MMIPICROP_Save crop_info_ptr is PNULL !!!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIC_CROP_657_112_2_18_2_46_12_16,(uint8*)"");
        return MMI_PICCROP_ERR_EXIT;
    }

    CropCreateSaveFileName();

    /*//set input param   
    input_param.src_ptr=crop_info_ptr->crop_bitmap_ptr->bit_ptr;
    input_param.out_buf_ptr=crop_info_ptr->output_buf_ptr;
    input_param.out_buf_size=crop_info_ptr->output_buffer_size;
    input_param.input_width=crop_info_ptr->crop_bitmap_ptr->img_width;
    input_param.input_height=crop_info_ptr->crop_bitmap_ptr->img_height;
    input_param.target_width=crop_info_ptr->crop_bitmap_ptr->img_width;
    input_param.target_height=crop_info_ptr->crop_bitmap_ptr->img_height;
    input_param.in_type=PEPROCESS_IMAGE_FORMAT_RGB_565;
    input_param.out_type=PEPROCESS_IMAGE_FORMAT_JPEG;
    
    //set process_param_ptr param
    process_param.process_buf_ptr = crop_info_ptr->process_buf_ptr;
    process_param.process_buf_size = crop_info_ptr->process_buffer_size;
    process_param.type = PEPROCESS_ENCODE_JPEG;
    
    //param
    process_param.process_param.encode_to_jpeg.quality_level=PEPROCESS_COMPRESS_LEVEL_LOW;
    
    //image param
    ref_return = PEPROCESS_ImageProcess(&input_param,&output_param,&process_param);
    if( PEPROCESS_OK != ref_return )
    {
        ret_value = MMI_PICCROP_ERR_EXIT;
        return ret_value;
    }
    
    if(MMIAPIFMM_IsFileExist( crop_info_ptr->random_name_ptr,crop_info_ptr->random_name_len))
    {
        MMIAPIFMM_DeleteFileSyn( crop_info_ptr->random_name_ptr,crop_info_ptr->random_name_len);        
    }
    if(MMIAPIFMM_WriteFilesDataSyn(crop_info_ptr->random_name_ptr, crop_info_ptr->random_name_len, input_param.out_buf_ptr,
            output_param.compress_out_size))
    {
        ret_value = MMI_PICCROP_SUCCESS;
    }
    else
    {
        ret_value = MMI_PICCROP_ERR_NO_SPACE;
    }*/

    if(MMIAPICOM_WriteBMPFile(crop_info_ptr->random_name_ptr, crop_info_ptr->crop_bitmap_ptr->img_width, 
        crop_info_ptr->crop_bitmap_ptr->img_height, crop_info_ptr->crop_bitmap_ptr->bit_ptr, 16))
    {
        ret_value = MMI_PICCROP_SUCCESS;
    }
    else
    {
        ret_value = MMI_PICCROP_ERR_NO_SPACE;
    }

    return ret_value;    
}

/*****************************************************************************/
//  Description : get dest image width height
//  Global resource dependence : 
//  Author: chunyou
//  Note: 
/*****************************************************************************/
LOCAL void CropGetDestImageWidthHeight(uint32* img_width, uint32* img_height, GUI_RECT_T crop_rect)
{
    int16 crop_width = crop_rect.right - crop_rect.left + 1;
    int16 crop_height = crop_rect.bottom - crop_rect.top + 1;

    if(crop_width <= MMIPICCROP_MAX_WIDTH && crop_height <= MMIPICCROP_MAX_HEIGHT)
    {
        *img_width = crop_width;
        *img_height = crop_height;
    }
    else if(crop_width > MMIPICCROP_MAX_WIDTH && crop_height < MMIPICCROP_MAX_HEIGHT)
    {
        *img_width = MMIPICCROP_MAX_WIDTH;
        *img_height = crop_height * MMIPICCROP_MAX_WIDTH / crop_width;
    }
    else if(crop_width < MMIPICCROP_MAX_WIDTH && crop_height > MMIPICCROP_MAX_HEIGHT)
    {
        *img_height = MMIPICCROP_MAX_HEIGHT;
        *img_width = crop_width * MMIPICCROP_MAX_HEIGHT / crop_height;
    }
    else if(crop_width > MMIPICCROP_MAX_WIDTH && crop_height > MMIPICCROP_MAX_HEIGHT)
    {
        if(crop_width * MMIPICCROP_MAX_HEIGHT / crop_height >= MMIPICCROP_MAX_WIDTH)
        {
            *img_width = MMIPICCROP_MAX_WIDTH;
            *img_height = crop_height * MMIPICCROP_MAX_WIDTH / crop_width;
        }
        else
        {
            *img_height = MMIPICCROP_MAX_HEIGHT;
            *img_width = crop_width * MMIPICCROP_MAX_HEIGHT / crop_height;
        }
    }
    else
    {
        *img_height = MIN(crop_width, MMIPICCROP_MAX_WIDTH);
        *img_width = MIN(crop_height, MMIPICCROP_MAX_HEIGHT);
    }
}

/*****************************************************************************/
//  Description : get crop image data
//  Global resource dependence : 
//  Author: chunyou
//  Note: 
/*****************************************************************************/
PUBLIC MMI_PICCROP_ERROR_TYPE_E MMIPICCROP_GetCropImageData(void)
{
    BOOLEAN             result = FALSE;
    MMI_PICCROP_ERROR_TYPE_E err_type = MMI_PICCROP_ERR_EXIT;
    GUIIMG_SRC_T        img_src = {0};
    GUIIMG_DISPLAY_T    img_display = {0};
    GUIIMG_DEC_OUT_T    img_output = {0};    
    uint16  lcd_width = 0;
    uint16  lcd_height = 0;
    GUI_RECT_T crop_rect = CropGetOriginalImageRect();
    MMIPICCROP_INFO_T* crop_info_ptr = MMIPICCROP_GetCropInfoPtr();

    if(PNULL == crop_info_ptr)
    {
        //SCI_TRACE_LOW:"[MMIPICCROP] MMIPICCROP_GetCropImageData crop_info_ptr is PNULL !!!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIC_CROP_779_112_2_18_2_46_12_17,(uint8*)"");
        err_type = MMI_PICCROP_ERR_EXIT;
        return err_type;
    }
    
    GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID,&lcd_width,&lcd_height);    
    if(crop_info_ptr->is_file)
    {
        //set image full path
        img_src.is_file = TRUE;
        img_src.full_path_ptr = crop_info_ptr->file_info.full_path_wstr_ptr;
    }
    else
    {
        img_src.src_buf_ptr = crop_info_ptr->src_buf_ptr;
        img_src.src_data_size = crop_info_ptr->src_data_size;
    }   

    //set image dest width and height
    CropGetDestImageWidthHeight(&img_display.dest_width, &img_display.dest_height, crop_rect);
	//lijinghua modify it
    //if(MMIPIC_CROP_WALLPAPER == crop_info_ptr->crop_type)
    //{
    
	if((crop_info_ptr->original_img.image_width > crop_info_ptr->min_width && crop_info_ptr->original_img.image_height > crop_info_ptr->min_height)|| MMIPIC_CROP_WALLPAPER == crop_info_ptr->crop_type)
	{
			if(img_display.dest_width < crop_info_ptr->min_width)
			{
				img_display.dest_width = crop_info_ptr->min_width;
				crop_rect.right = (int16)(crop_rect.left + img_display.dest_width - 1); /*lint !e737*/
			}
			if(img_display.dest_height < crop_info_ptr->min_height)
			{
				img_display.dest_height = crop_info_ptr->min_height;
				crop_rect.bottom = (int16)(crop_rect.top + img_display.dest_height - 1); /*lint !e737*/
			}
	}

    //}
	//lijinghua end
    img_display.crop_rect_ptr = &crop_rect;

    //set image decode output data buffer

    img_output.decode_data_size = img_display.dest_width * img_display.dest_height *2;

    if(NULL !=crop_info_ptr->crop_bitmap_ptr)
    {
        if(NULL !=crop_info_ptr->crop_bitmap_ptr->bit_ptr)
        {
            SCI_FREE(crop_info_ptr->crop_bitmap_ptr->bit_ptr);
            crop_info_ptr->crop_bitmap_ptr->bit_ptr=NULL;
        }
        SCI_FREE(crop_info_ptr->crop_bitmap_ptr);
        crop_info_ptr->crop_bitmap_ptr=NULL;
    }
    
    img_output.decode_data_ptr = (uint8*)SCI_ALLOCA(img_output.decode_data_size);
    if (PNULL == img_output.decode_data_ptr)
    {
        //MMIPICCROP_ASSERT_WARNING:"[MMIPICCROP] MMIPICCROP_GetCropImageData img_output.decode_data_ptr is PNULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIC_CROP_832_112_2_18_2_46_12_18,(uint8*)"");
        err_type = MMI_PICCROP_ERR_NO_MEMORY;
        return err_type;
    }
    
    //decode jpg
    result = GUIIMG_Decode(&img_src,
                &img_display,
                &img_output);
    
    if (result)// && img_output.actual_width > 0 && img_output.actual_height > 0)
    {
        do
        {
            //alloc bitmap
            if (PNULL == crop_info_ptr->crop_bitmap_ptr)
            {
                crop_info_ptr->crop_bitmap_ptr = SCI_ALLOCA(sizeof(GUIIMG_BITMAP_T));
            }
            if (PNULL == crop_info_ptr->crop_bitmap_ptr)
            {
                //MMIPICCROP_ASSERT_WARNING:"[MMIPICCROP] MMIPICCROP_GetCropImageData s_pic_editor_info.display_bitmap_ptr is PNULL"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIC_CROP_853_112_2_18_2_46_12_19,(uint8*)"");
                err_type = MMI_PICCROP_ERR_NO_MEMORY;
                break;
            }
            SCI_MEMSET(crop_info_ptr->crop_bitmap_ptr,0,sizeof(GUIIMG_BITMAP_T));
            if (PNULL != crop_info_ptr->crop_bitmap_ptr->bit_ptr)
            {
                SCI_FREE(crop_info_ptr->crop_bitmap_ptr->bit_ptr);
                crop_info_ptr->crop_bitmap_ptr->bit_ptr = PNULL;
            }
            crop_info_ptr->crop_bitmap_ptr->bit_ptr = img_output.decode_data_ptr;
            //set others param
            crop_info_ptr->crop_bitmap_ptr->img_width = img_output.actual_width;
            crop_info_ptr->crop_bitmap_ptr->img_height = img_output.actual_height;
            crop_info_ptr->crop_bitmap_size = img_output.decode_data_size;
            err_type = MMI_PICCROP_SUCCESS;
        }while(0);
    }

    //free vpbits
    if (PNULL != img_output.decode_data_ptr && !result)
    {
        SCI_FREE(img_output.decode_data_ptr);
        img_output.decode_data_ptr = PNULL;
    }
    
    //SCI_TRACE_LOW:"[MMIPICCROP] MMIPICCROP_GetCropImageData err_type = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIC_CROP_879_112_2_18_2_46_12_20,(uint8*)"d", err_type);
    return (err_type);
}

/*****************************************************************************/
//  Description : show error or normal message
//  Global resource dependence : 
//  Author:chunyou
//  Note: 
//  Param 
//  nput:  pic_error   value of MMI_PICCROP_ERROR_TYPE_E type
/*****************************************************************************/
PUBLIC void  MMIPICCROP_ShowMsg(MMI_PICCROP_ERROR_TYPE_E pic_error)
{
    MMIFILE_DEVICE_E file_dev = MMI_DEVICE_UDISK;
    BOOLEAN         ret = TRUE;
    MMIFILE_ERROR_E fs_error = SFS_ERROR_SYSTEM;
    uint32          msg_type = 0; // fs :0, alert:1 ,query:2, NO Msg: 3
    MMI_TEXT_ID_T   text1_id = TXT_NULL;
    MMI_TEXT_ID_T   text2_id = TXT_NULL;
    MMI_IMAGE_ID_T  image_id = IMAGE_PUBWIN_SUCCESS;
    MMIPICCROP_INFO_T* crop_info_ptr = MMIPICCROP_GetCropInfoPtr();

    if(PNULL == crop_info_ptr)
    {
        //SCI_TRACE_LOW:"[MMIPICCROP] MMIPICCROP_ShowMsg crop_info_ptr is PNULL !!!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIC_CROP_902_112_2_18_2_46_12_21,(uint8*)"");
        return;
    }

    //convert error type
    switch(pic_error)
    {
    case MMI_PICCROP_SUCCESS:
        msg_type = 1;
        text1_id = TXT_COMM_SAVE_SUCCESS;
        image_id = IMAGE_PUBWIN_SUCCESS;
        break;
    case MMI_PICCROP_ERR_NO_SPACE:
        fs_error = SFS_ERROR_NO_SPACE;
        break;
    default:
        msg_type = 1;
        text1_id = TXT_ERROR_OCCUR;
        image_id = IMAGE_PUBWIN_WARNING;				
        break;
    }


    switch(msg_type)
    {
    case 1:
        MMIPUB_OpenAlertWinByTextId(PNULL, text1_id, text2_id, image_id, PNULL, PNULL, MMIPUB_SOFTKEY_ONE, PNULL);
        break;
    //case 2:
    //    MMIPUB_OpenQueryWinByTextId(text1_id,image_id,PNULL,PNULL);
    //    break;
    //case 3:
    //    break;
    default:
        //dev_char = *(s_pic_editor_info.file_info.full_path_wstr_ptr);
        file_dev = MMIAPIFMM_GetDeviceTypeByPath(crop_info_ptr->file_info.full_path_wstr_ptr, MMIFILE_DEVICE_LEN);
        ret= MMIAPIFMM_CheckMemoryCard(file_dev);
        //除磁盘格式错误外的
        if(ret)
        {
            MMIAPIFMM_PromptFileErrorType(PNULL,fs_error, file_dev);
        }
        break;
    }
}

/*****************************************************************************/
//  Description : is valid rect
//  Global resource dependence : 
//  Author: chunyou
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN CropIsValidRect(GUI_RECT_T rect)
{
    BOOLEAN result = TRUE;
    uint16  lcd_width = 0;
    uint16  lcd_height = 0;
    uint16  width = 0;
    uint16  height = 0;
    GUI_RECT_T dest_rect = {0};
    MMIPICCROP_INFO_T* crop_info_ptr = MMIPICCROP_GetCropInfoPtr();

    if(PNULL == crop_info_ptr)
    {
        //SCI_TRACE_LOW:"[MMIPICCROP] CropIsValidRect crop_info_ptr is PNULL !!!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIC_CROP_964_112_2_18_2_46_12_22,(uint8*)"");
        return FALSE;
    }
    
    GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID,&lcd_width,&lcd_height);   
    switch(crop_info_ptr->crop_type)
    {
    case MMIPIC_CROP_WALLPAPER:
    case MMIPIC_CROP_DEFAULT_SAVE:
	case MMIPIC_CROP_PB:              //lijinghua add for PB
        if(GUI_IntersectRect(&dest_rect, rect, crop_info_ptr->display_rect) && MMIPICCROP_IsPermitCrop())
        {
            width = (uint16)((dest_rect.right - dest_rect.left + 1) * crop_info_ptr->original_img.image_width / (crop_info_ptr->display_rect.right - crop_info_ptr->display_rect.left + 1));
            height = (uint16)((dest_rect.bottom - dest_rect.top + 1) * crop_info_ptr->original_img.image_height / (crop_info_ptr->display_rect.bottom - crop_info_ptr->display_rect.top + 1));
            if(width >= crop_info_ptr->min_width && height >= crop_info_ptr->min_height)
            {
                result = TRUE;
            }
            else
            {
                result = FALSE;
            }
        }
        else
        {
            result = FALSE;
        }
        break;
    default:
        break;
    }

    return result;
}

/*****************************************************************************/
//  Description : is permit crop
//  Global resource dependence : 
//  Author: chunyou
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIPICCROP_IsPermitCrop(void)
{
    BOOLEAN result = TRUE;
    uint16  lcd_width = 0;
    uint16  lcd_height = 0;
    MMIPICCROP_INFO_T* crop_info_ptr = MMIPICCROP_GetCropInfoPtr();

    if(PNULL == crop_info_ptr)
    {
        //SCI_TRACE_LOW:"[MMIPICCROP] MMIPICCROP_IsPermitCrop crop_info_ptr is PNULL !!!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIC_CROP_1011_112_2_18_2_46_13_23,(uint8*)"");
        return FALSE;
    }    
    
    GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID,&lcd_width,&lcd_height);   
    switch(crop_info_ptr->crop_type)
    {
    case MMIPIC_CROP_WALLPAPER:
    case MMIPIC_CROP_DEFAULT_SAVE:
	case MMIPIC_CROP_PB:                           //lijinghua add for PB
        if(crop_info_ptr->original_img.image_width <= crop_info_ptr->min_width || crop_info_ptr->original_img.image_height <= crop_info_ptr->min_height)
        {
            result = FALSE;
        }
        break;

    default:
        break;
    }
    //SCI_TRACE_LOW:"[MMIPICCROP] MMIPICCROP_IsPermitCrop result = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIC_CROP_1029_112_2_18_2_46_13_24,(uint8*)"d", result);
    return result;
}
/*****************************************************************************/
//  Description : handle touch pen
//  Global resource dependence : 
//  Author: chunyou
//  Note: 
/*****************************************************************************/
PUBLIC void MMIPICCROP_HandleTpMsg(GUI_POINT_T point,MMI_MESSAGE_ID_E msg_id)
{
    static int16           relative_x=0;
    static int16           relative_y=0;
    static BOOLEAN         is_tp_press_down=FALSE;
    uint16 frame_width_offset = MMIPICCROP_MOVE_POINT_OFFSET;
    int16 point_dis = 0;
    GUI_RECT_T cur_rect = {0};
    GUI_RECT_T dest_rect = {0};
    //GUI_RECT_T      client_rect = MMIPICCROP_GetClientRect();
    static MMIPICCROP_TP_RECT_E tp_rect_type = MMIPIC_CROP_TP_NONE;
    static GUI_POINT_T start_point = {0};
    MMIPICCROP_INFO_T* crop_info_ptr = MMIPICCROP_GetCropInfoPtr();

    if(PNULL == crop_info_ptr)
    {
        //SCI_TRACE_LOW:"[MMIPICCROP] CropHandleTpMsg crop_info_ptr is PNULL !!!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIC_CROP_1052_112_2_18_2_46_13_25,(uint8*)"");
        return ;
    }

    switch(msg_id)
    {
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_TP_PRESS_DOWN:
        do
        {
            start_point.x = point.x;
            start_point.y = point.y;
            cur_rect.left = crop_info_ptr->crop_frame_rect.left - crop_info_ptr->frame_img_width/2 - frame_width_offset;
            cur_rect.top = crop_info_ptr->crop_frame_rect.top - crop_info_ptr->frame_img_width/2 - frame_width_offset;
            cur_rect.right = cur_rect.left + crop_info_ptr->frame_img_width + frame_width_offset*2;
            cur_rect.bottom = cur_rect.top + crop_info_ptr->frame_img_width + frame_width_offset*2;
            if(GUI_PointIsInRect(point, cur_rect))
            {
                tp_rect_type = MMIPIC_CROP_LEFT_TOP;
                relative_x = 0;
                relative_y = 0;
                is_tp_press_down = TRUE;
                break;
            }
            cur_rect.left = crop_info_ptr->crop_frame_rect.right - crop_info_ptr->frame_img_width/2 - frame_width_offset;
            cur_rect.top = crop_info_ptr->crop_frame_rect.top - crop_info_ptr->frame_img_width/2 - frame_width_offset;
            cur_rect.right = cur_rect.left + crop_info_ptr->frame_img_width + frame_width_offset*2;
            cur_rect.bottom = cur_rect.top + crop_info_ptr->frame_img_width + frame_width_offset*2;
            if(GUI_PointIsInRect(point, cur_rect))
            {
                tp_rect_type = MMIPIC_CROP_RIGHT_TOP;
                relative_x = 0;
                relative_y = 0;
                is_tp_press_down = TRUE;
                break;
            }
            cur_rect.left = crop_info_ptr->crop_frame_rect.left - crop_info_ptr->frame_img_width/2 - frame_width_offset;
            cur_rect.top = crop_info_ptr->crop_frame_rect.bottom - crop_info_ptr->frame_img_width/2 - frame_width_offset;
            cur_rect.right = cur_rect.left + crop_info_ptr->frame_img_width + frame_width_offset*2;
            cur_rect.bottom = cur_rect.top + crop_info_ptr->frame_img_width + frame_width_offset*2;
            if(GUI_PointIsInRect(point, cur_rect))
            {
                tp_rect_type = MMIPIC_CROP_LEFT_BOTTOM;
                relative_x = 0;
                relative_y = 0;
                is_tp_press_down = TRUE;
                break;
            }
            cur_rect.left = crop_info_ptr->crop_frame_rect.right - crop_info_ptr->frame_img_width/2 - frame_width_offset;
            cur_rect.top = crop_info_ptr->crop_frame_rect.bottom - crop_info_ptr->frame_img_width/2 - frame_width_offset;
            cur_rect.right = cur_rect.left + crop_info_ptr->frame_img_width + frame_width_offset*2;
            cur_rect.bottom = cur_rect.top + crop_info_ptr->frame_img_width + frame_width_offset*2;
            if(GUI_PointIsInRect(point, cur_rect))
            {
                tp_rect_type = MMIPIC_CROP_RIGHT_BOTTOM;
                relative_x = 0;
                relative_y = 0;
                is_tp_press_down = TRUE;
                break;
            }
            if(GUI_PointIsInRect(point, crop_info_ptr->crop_frame_rect))
            {
                tp_rect_type = MMIPIC_CROP_RECT_MIDDLE;
                relative_x = point.x - crop_info_ptr->crop_frame_rect.left;
                relative_y = point.y - crop_info_ptr->crop_frame_rect.top;
                is_tp_press_down = TRUE;
                break;
            }
            is_tp_press_down = FALSE;
            start_point.x = 0;
            start_point.y = 0;
        }while(0);
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_TP_PRESS_UP:
        is_tp_press_down=FALSE;
        relative_x = 0;
        relative_y = 0;
        start_point.x = 0;
        start_point.y = 0;
        tp_rect_type = MMIPIC_CROP_TP_NONE;
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_TP_PRESS_MOVE:
        if(is_tp_press_down && (abs(point.x - start_point.x) >= MMIPICCROP_MOVE_STEP_LEN || abs(point.y - start_point.y) >= MMIPICCROP_MOVE_STEP_LEN))
        {
            int16 offset_x = 0;
            int16 offset_y = 0;
            int16 offset_scale_x = 0;
            int16 offset_scale_y = 0;
            BOOLEAN flag = TRUE;
            switch(tp_rect_type)
            {
            case MMIPIC_CROP_LEFT_TOP:
                offset_x = point.x - crop_info_ptr->crop_frame_rect.left;
                offset_y = point.y - crop_info_ptr->crop_frame_rect.top;
                offset_scale_x = abs(offset_x);
                offset_scale_y = abs((int16)(offset_y * crop_info_ptr->scale_width/crop_info_ptr->scale_height));
                if(offset_x > 0 && offset_y > 0)
                {                    
                    if(offset_scale_x <= offset_scale_y)
                    {
                        cur_rect.left = crop_info_ptr->crop_frame_rect.left + offset_scale_x;
                        cur_rect.top = crop_info_ptr->crop_frame_rect.bottom - (crop_info_ptr->crop_frame_rect.right - cur_rect.left + 1) * crop_info_ptr->scale_height/crop_info_ptr->scale_width;
                    }
                    else
                    {
                        cur_rect.top = crop_info_ptr->crop_frame_rect.top + offset_scale_y;
                        cur_rect.left = crop_info_ptr->crop_frame_rect.right - (crop_info_ptr->crop_frame_rect.bottom - cur_rect.top + 1) * crop_info_ptr->scale_width/crop_info_ptr->scale_height;
                    }
                }
                else if(offset_x < 0 && offset_y < 0)
                {
                    if(offset_scale_x <= offset_scale_y)
                    {
                        cur_rect.left = crop_info_ptr->crop_frame_rect.left - offset_scale_x;
                        cur_rect.top = crop_info_ptr->crop_frame_rect.bottom - (crop_info_ptr->crop_frame_rect.right - cur_rect.left + 1) * crop_info_ptr->scale_height/crop_info_ptr->scale_width;
                    }
                    else
                    {
                        cur_rect.top = crop_info_ptr->crop_frame_rect.top - offset_scale_y;
                        cur_rect.left = crop_info_ptr->crop_frame_rect.right - (crop_info_ptr->crop_frame_rect.bottom - cur_rect.top + 1) * crop_info_ptr->scale_width/crop_info_ptr->scale_height;                        
                    }
                }
                else
                {
                    flag = FALSE;
                }
                cur_rect.right = crop_info_ptr->crop_frame_rect.right;
                cur_rect.bottom = crop_info_ptr->crop_frame_rect.bottom;                
                break;
            case MMIPIC_CROP_RIGHT_TOP:
                offset_x = point.x - crop_info_ptr->crop_frame_rect.right;
                offset_y = point.y - crop_info_ptr->crop_frame_rect.top;
                offset_scale_x = abs(offset_x);
                offset_scale_y = abs((int16)(offset_y * crop_info_ptr->scale_width/crop_info_ptr->scale_height));
                if(offset_x > 0 && offset_y < 0)
                {                    
                    if(offset_scale_x <= offset_scale_y)
                    {
                        cur_rect.right = crop_info_ptr->crop_frame_rect.right + offset_scale_x;
                        cur_rect.top = crop_info_ptr->crop_frame_rect.bottom - (cur_rect.right - crop_info_ptr->crop_frame_rect.left + 1) * crop_info_ptr->scale_height/crop_info_ptr->scale_width;
                    }
                    else
                    {
                        cur_rect.top = crop_info_ptr->crop_frame_rect.top - offset_scale_y;
                        cur_rect.right = crop_info_ptr->crop_frame_rect.left + (crop_info_ptr->crop_frame_rect.bottom - cur_rect.top + 1) * crop_info_ptr->scale_width/crop_info_ptr->scale_height;
                    }
                }
                else if(offset_x < 0 && offset_y > 0)
                {
                    if(offset_scale_x <= offset_scale_y)
                    {
                        cur_rect.right = crop_info_ptr->crop_frame_rect.right - offset_scale_x;
                        cur_rect.top = crop_info_ptr->crop_frame_rect.bottom - (cur_rect.right - crop_info_ptr->crop_frame_rect.left + 1) * crop_info_ptr->scale_height/crop_info_ptr->scale_width;
                    }
                    else
                    {
                        cur_rect.top = crop_info_ptr->crop_frame_rect.top + offset_scale_y;
                        cur_rect.right = crop_info_ptr->crop_frame_rect.left + (crop_info_ptr->crop_frame_rect.bottom - cur_rect.top + 1) * crop_info_ptr->scale_width/crop_info_ptr->scale_height;
                    }
                }
                else
                {
                    flag = FALSE;
                }
                cur_rect.left = crop_info_ptr->crop_frame_rect.left;
                cur_rect.bottom = crop_info_ptr->crop_frame_rect.bottom;  
                break;
            case MMIPIC_CROP_LEFT_BOTTOM:
                offset_x = point.x - crop_info_ptr->crop_frame_rect.left;
                offset_y = point.y - crop_info_ptr->crop_frame_rect.bottom;
                offset_scale_x = abs(offset_x);
                offset_scale_y = abs((int16)(offset_y * crop_info_ptr->scale_width/crop_info_ptr->scale_height));
                if(offset_x > 0 && offset_y < 0)
                {                    
                    if(offset_scale_x <= offset_scale_y)
                    {
                        cur_rect.left = crop_info_ptr->crop_frame_rect.left + offset_scale_x;
                        cur_rect.bottom = crop_info_ptr->crop_frame_rect.top + (crop_info_ptr->crop_frame_rect.right - cur_rect.left + 1) * crop_info_ptr->scale_height/crop_info_ptr->scale_width;
                    }
                    else
                    {
                        cur_rect.bottom = crop_info_ptr->crop_frame_rect.bottom - offset_scale_y;
                        cur_rect.left = crop_info_ptr->crop_frame_rect.right - (cur_rect.bottom - crop_info_ptr->crop_frame_rect.top + 1) * crop_info_ptr->scale_width/crop_info_ptr->scale_height;
                    }
                }
                else if(offset_x < 0 && offset_y > 0)
                {
                    if(offset_scale_x <= offset_scale_y)
                    {
                        cur_rect.left = crop_info_ptr->crop_frame_rect.left - offset_scale_x;
                        cur_rect.bottom = crop_info_ptr->crop_frame_rect.top + (crop_info_ptr->crop_frame_rect.right - cur_rect.left + 1) * crop_info_ptr->scale_height/crop_info_ptr->scale_width;
                    }
                    else
                    {
                        cur_rect.bottom = crop_info_ptr->crop_frame_rect.bottom + offset_scale_y;
                        cur_rect.left = crop_info_ptr->crop_frame_rect.right - (cur_rect.bottom - crop_info_ptr->crop_frame_rect.top + 1) * crop_info_ptr->scale_width/crop_info_ptr->scale_height;
                    }
                }
                else
                {
                    flag = FALSE;
                }
                cur_rect.right = crop_info_ptr->crop_frame_rect.right;
                cur_rect.top = crop_info_ptr->crop_frame_rect.top;   
                break;
            case MMIPIC_CROP_RIGHT_BOTTOM:
                offset_x = point.x - crop_info_ptr->crop_frame_rect.right;
                offset_y = point.y - crop_info_ptr->crop_frame_rect.bottom;
                offset_scale_x = abs(offset_x);
                offset_scale_y = abs((int16)(offset_y * crop_info_ptr->scale_width/crop_info_ptr->scale_height));
                if(offset_x > 0 && offset_y > 0)
                {                    
                    if(offset_scale_x <= offset_scale_y)
                    {
                        cur_rect.right = crop_info_ptr->crop_frame_rect.right + offset_scale_x;
                        cur_rect.bottom = crop_info_ptr->crop_frame_rect.top + (cur_rect.right - crop_info_ptr->crop_frame_rect.left + 1) * crop_info_ptr->scale_height/crop_info_ptr->scale_width;
                    }
                    else
                    {
                        cur_rect.bottom = crop_info_ptr->crop_frame_rect.bottom + offset_scale_y;
                        cur_rect.right = crop_info_ptr->crop_frame_rect.left + (cur_rect.bottom - crop_info_ptr->crop_frame_rect.top + 1) * crop_info_ptr->scale_width/crop_info_ptr->scale_height;
                    }
                }
                else if(offset_x < 0 && offset_y < 0)
                {
                    if(offset_scale_x <= offset_scale_y)
                    {
                        cur_rect.right = crop_info_ptr->crop_frame_rect.right - offset_scale_x;
                        cur_rect.bottom = crop_info_ptr->crop_frame_rect.top + (cur_rect.right - crop_info_ptr->crop_frame_rect.left + 1) * crop_info_ptr->scale_height/crop_info_ptr->scale_width;
                    }
                    else
                    {
                        cur_rect.bottom = crop_info_ptr->crop_frame_rect.bottom - offset_scale_y;
                        cur_rect.right = crop_info_ptr->crop_frame_rect.left + (cur_rect.bottom - crop_info_ptr->crop_frame_rect.top + 1) * crop_info_ptr->scale_width/crop_info_ptr->scale_height;
                    }
                }
                else
                {
                    flag = FALSE;
                }
                cur_rect.left = crop_info_ptr->crop_frame_rect.left;
                cur_rect.top = crop_info_ptr->crop_frame_rect.top;        
                break;
            case MMIPIC_CROP_RECT_MIDDLE:                
                cur_rect.left = point.x - relative_x;
                cur_rect.top = point.y - relative_y;
                cur_rect.right = cur_rect.left + crop_info_ptr->crop_frame_rect.right - crop_info_ptr->crop_frame_rect.left;
                cur_rect.bottom = cur_rect.top + crop_info_ptr->crop_frame_rect.bottom - crop_info_ptr->crop_frame_rect.top;
                flag = TRUE;
                break;
            default:
                break;                
            }

            //adjust cur rect
            if(cur_rect.left < crop_info_ptr->display_rect.left
                && crop_info_ptr->display_rect.left != crop_info_ptr->crop_frame_rect.left)
            {
                point_dis = crop_info_ptr->display_rect.left - cur_rect.left;
                cur_rect.left = crop_info_ptr->display_rect.left;
                cur_rect.right += point_dis;
            }
            if(cur_rect.top < crop_info_ptr->display_rect.top
                && crop_info_ptr->display_rect.top != crop_info_ptr->crop_frame_rect.top)
            {
                point_dis = crop_info_ptr->display_rect.top - cur_rect.top;
                cur_rect.top = crop_info_ptr->display_rect.top;
                cur_rect.bottom += point_dis;
            }
            if(cur_rect.right > crop_info_ptr->display_rect.right
                && crop_info_ptr->display_rect.right != crop_info_ptr->crop_frame_rect.right)
            {
                point_dis = cur_rect.right - crop_info_ptr->display_rect.right;
                cur_rect.right = crop_info_ptr->display_rect.right;
                cur_rect.left -= point_dis;
            }
            if(cur_rect.bottom > crop_info_ptr->display_rect.bottom
                && crop_info_ptr->display_rect.bottom != crop_info_ptr->crop_frame_rect.bottom)
            {
                point_dis = cur_rect.bottom - crop_info_ptr->display_rect.bottom;
                cur_rect.bottom = crop_info_ptr->display_rect.bottom;
                cur_rect.top -= point_dis;
            }
            
            if(flag && GUI_RectIsCovered(crop_info_ptr->display_rect, cur_rect) && GUI_IntersectRect(&dest_rect, cur_rect, crop_info_ptr->display_rect)
                && MIN(cur_rect.right - cur_rect.left + 1, cur_rect.bottom - cur_rect.top + 1) >= crop_info_ptr->frame_img_width  + frame_width_offset*2
                && (CropIsValidRect(cur_rect)  || MMIPIC_CROP_RECT_MIDDLE == tp_rect_type))
            {
                crop_info_ptr->crop_frame_rect.left = cur_rect.left;
                crop_info_ptr->crop_frame_rect.top = cur_rect.top;
                crop_info_ptr->crop_frame_rect.right = cur_rect.right;
                crop_info_ptr->crop_frame_rect.bottom = cur_rect.bottom;
                MMIPICCROP_DisplayImage(msg_id);

                start_point.x = point.x;
                start_point.y = point.y;
            }
        }
        break;    
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    default:
        break;
    }
    
}
//lijinghua add for pic crop
PUBLIC void MMIPICCROP_HandleMsg(MMI_MESSAGE_ID_E msg_id,MMI_WIN_ID_T  win_id)
{
    MMIPICCROP_INFO_T* crop_info_ptr = MMIPICCROP_GetCropInfoPtr();
#ifdef PIC_EDITOR_SUPPORT
	s_icon_is_square_crop = PIC_CROP_GetSquareCropStatus();
	s_icon_is_landscape_crop = PIC_CROP_GetLandscapeCropStatus();
#endif
    if(PNULL == crop_info_ptr)
    {
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIC_CROP_1052_112_2_18_2_46_13_25,(uint8*)"");
        return ;
    }
	    switch(msg_id)
    {
        case MSG_KEYREPEAT_UP:
		case MSG_APP_UP:
			if(crop_info_ptr->crop_type == MMIPIC_CROP_PB)
			{
			    if(clip_rect_ptr.top>=crop_info_ptr->display_rect.top)
				{
					s_icon_move_up_or_down_crop_index++;
					MMK_PostMsg(win_id,MSG_FULL_PAINT,PNULL,0);
				}
			}else
			{
				if(s_icon_is_square_crop)
				{
					if(top_rect.bottom>=crop_info_ptr->display_rect.top)
					{
						s_icon_move_up_or_down_crop_index++;
						MMK_PostMsg(win_id,MSG_FULL_PAINT,PNULL,0);
					}
				}else if(s_icon_is_landscape_crop)
				{
					if(top_rect.bottom>=crop_info_ptr->display_rect.top)
					{
						s_icon_move_up_or_down_crop_index++;
						MMK_PostMsg(win_id,MSG_FULL_PAINT,PNULL,0);
					}
				}else
				{
					if(crop_index>0&&top_rect.bottom>=crop_info_ptr->display_rect.top)
					{
						s_icon_move_up_or_down_crop_index++;
						MMK_PostMsg(win_id,MSG_FULL_PAINT,PNULL,0);
					}
				}
			}
			break;
		case MSG_KEYREPEAT_DOWN:
		case MSG_APP_DOWN:
			if(crop_info_ptr->crop_type == MMIPIC_CROP_PB)
			{
			    if(clip_rect_ptr.bottom<=crop_info_ptr->display_rect.bottom)
				{
					s_icon_move_up_or_down_crop_index--;
					MMK_PostMsg(win_id,MSG_FULL_PAINT,PNULL,0);
				}
			}else
			{
				if(s_icon_is_square_crop)
				{
					if(bottom_rect.top<=crop_info_ptr->display_rect.bottom)
					{
						s_icon_move_up_or_down_crop_index--;
						MMK_PostMsg(win_id,MSG_FULL_PAINT,PNULL,0);
					}
				}else if(s_icon_is_landscape_crop)
				{
					if(bottom_rect.top<=crop_info_ptr->display_rect.bottom)
					{
						s_icon_move_up_or_down_crop_index--;
						MMK_PostMsg(win_id,MSG_FULL_PAINT,PNULL,0);
					}
				}else
				{
					if(crop_index>0&&bottom_rect.top<=crop_info_ptr->display_rect.bottom)
					{
						s_icon_move_up_or_down_crop_index--;
						MMK_PostMsg(win_id,MSG_FULL_PAINT,PNULL,0);
					}
				}
			}
			break;
		case MSG_KEYREPEAT_LEFT:
		case MSG_APP_LEFT:
			if(crop_info_ptr->crop_type == MMIPIC_CROP_PB)
			{
			    if(clip_rect_ptr.left>=crop_info_ptr->display_rect.left)
				{
					s_icon_move_left_or_right_crop_index++;
					MMK_PostMsg(win_id,MSG_FULL_PAINT,PNULL,0);
				}
			}else
			{
				if(s_icon_is_square_crop)
				{
					if(square_crop_index>0	&& left_rect.right>=crop_info_ptr->display_rect.left)
					{
						s_icon_move_left_or_right_crop_index++;
						MMK_PostMsg(win_id,MSG_FULL_PAINT,PNULL,0);
					}
				}else if(s_icon_is_landscape_crop)
				{
					if(landscape_crop_index>0  && left_rect.right>=crop_info_ptr->display_rect.left)
					{
						s_icon_move_left_or_right_crop_index++;
						MMK_PostMsg(win_id,MSG_FULL_PAINT,PNULL,0);
					}
				}else
				{
					if(crop_index>0&&left_rect.right>=crop_info_ptr->display_rect.left)
					{
						s_icon_move_left_or_right_crop_index++;
						MMK_PostMsg(win_id,MSG_FULL_PAINT,PNULL,0);
					}
				}
			}
			break;
		case MSG_KEYREPEAT_RIGHT:
		case MSG_APP_RIGHT:
			if(crop_info_ptr->crop_type == MMIPIC_CROP_PB)
			{
			    if(clip_rect_ptr.right<=crop_info_ptr->display_rect.right)
				{
					s_icon_move_left_or_right_crop_index--;
					MMK_PostMsg(win_id,MSG_FULL_PAINT,PNULL,0);
				}
			}else
			{
				if(s_icon_is_square_crop)
				{
					if(square_crop_index>0 &&right_rect.left<=crop_info_ptr->display_rect.right)
					{
						s_icon_move_left_or_right_crop_index--;
						MMK_PostMsg(win_id,MSG_FULL_PAINT,PNULL,0);
					}
				}else if(s_icon_is_landscape_crop)
				{
					if(landscape_crop_index>0 &&right_rect.left<=crop_info_ptr->display_rect.right)
					{
						s_icon_move_left_or_right_crop_index--;
						MMK_PostMsg(win_id,MSG_FULL_PAINT,PNULL,0);
					}
				}else
				{
					if(crop_index>0&&right_rect.left<=crop_info_ptr->display_rect.right)
					{
						s_icon_move_left_or_right_crop_index--;
						MMK_PostMsg(win_id,MSG_FULL_PAINT,PNULL,0);
					}
				}
			}
			break;
		
		case MSG_CLOSE_WINDOW:
			ResetAllParam();
			break;
		default:
           break;
	}
	
}

//lijinghua end
/*****************************************************************************/
//  Description : display background and title
//  Global resource dependence : 
//  Author: chunyou
//  Note: 
/*****************************************************************************/
LOCAL void CropDisplayBgColor(void)
{
    uint16  lcd_width = 0;
    uint16  lcd_height = 0;
    GUI_RECT_T screen_rect = {0};
    MMIPICCROP_INFO_T* crop_info_ptr = MMIPICCROP_GetCropInfoPtr();

    if(PNULL == crop_info_ptr)
    {
        //SCI_TRACE_LOW:"[MMIPICCROP] CropDisplayBgAndTitle crop_info_ptr is PNULL !!!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIC_CROP_1369_112_2_18_2_46_13_26,(uint8*)"");
        return ;
    }
    
    GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID,&lcd_width,&lcd_height);       
    screen_rect.right = lcd_width - 1;
    screen_rect.bottom = lcd_height - 1;
    GUI_FillRect(&crop_info_ptr->lcd_dev, screen_rect, MMIPICCROP_BG_COLOR);
}

/*****************************************************************************/
//  Description : display background and title
//  Global resource dependence : 
//  Author: chunyou
//  Note: 
/*****************************************************************************/
LOCAL void CropDisplayTitle(void)
{
    uint16  lcd_width = 0;
    uint16  lcd_height = 0;
    GUI_RECT_T screen_rect = {0};
    MMIPICCROP_INFO_T* crop_info_ptr = MMIPICCROP_GetCropInfoPtr();

    if(PNULL == crop_info_ptr)
    {
        //SCI_TRACE_LOW:"[MMIPICCROP] CropDisplayBgAndTitle crop_info_ptr is PNULL !!!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIC_CROP_1392_112_2_18_2_46_13_27,(uint8*)"");
        return ;
    }
    
    GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID,&lcd_width,&lcd_height);
            
    //screen_rect.right = lcd_width - 1;
    //screen_rect.bottom = lcd_height - 1;
    //GUI_FillRect(&crop_info_ptr->lcd_dev, screen_rect, MMIPICCROP_BG_COLOR);

    screen_rect.left = 0;
    screen_rect.top = lcd_height - MMIPICCROP_BOTTOM_HEIGHT - 1;
    screen_rect.right = lcd_width - 1;
    screen_rect.bottom = lcd_height - 1;
    GUIRES_DisplayImg(PNULL,
            &screen_rect,
            PNULL,
            MMIPIC_CROP_MAIN_WIN_ID,
            IMAGE_PICCROP_BUTTON_BG,
            &crop_info_ptr->lcd_dev);
    //screen_rect.bottom = MMIPICCROP_TITLE_HEIGHT - 1;
    //GUI_FillRect(&crop_info_ptr->lcd_dev, screen_rect, MMIPICCROP_TITLE_BG_COLOR);

    GUIBUTTON_Update(PIC_CROP_OK_BUTTON_CTRL_ID);
    GUIBUTTON_Update(PIC_CROP_BACK_BUTTON_CTRL_ID);
    //MMK_SendMsg(PIC_CROP_TITLE_LABEL_CTRL_ID, MSG_CTL_PAINT, NULL);
}

/*****************************************************************************/
//  Description : crop frame display
//  Global resource dependence : 
//  Author: chunyou
//  Note: 
/*****************************************************************************/
LOCAL void CropFrameDisplay(void)
{
    GUI_RECT_T rect = {0};
    GUI_POINT_T point1 = {0};
    GUI_POINT_T point2 = {0};

    MMIPICCROP_INFO_T* crop_info_ptr = MMIPICCROP_GetCropInfoPtr();

    if(PNULL == crop_info_ptr)
    {
        //SCI_TRACE_LOW:"[MMIPICCROP] CropFrameDisplay crop_info_ptr is PNULL !!!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIC_CROP_1434_112_2_18_2_46_13_28,(uint8*)"");
        return ;
    }    

    if(crop_info_ptr->original_img.image_width > 4 
        && crop_info_ptr->original_img.image_height > 4)
    {
//lijinghua modify it
		if(crop_index>0)
	    {
			top_rect.left=left_shadow_display_rect.right-2;
			top_rect.right=right_shadow_display_rect.left+2;
			top_rect.top=top_shadow_display_rect.bottom-2;
			top_rect.bottom=top_shadow_display_rect.bottom;
			LCD_FillRect(&crop_info_ptr->lcd_dev,top_rect,MMI_WHITE_COLOR);
			left_rect.left=top_rect.left;
			left_rect.right=left_rect.left+2;
			left_rect.top=top_shadow_display_rect.bottom;
			left_rect.bottom=bottom_shadow_display_rect.top;
			LCD_FillRect(&crop_info_ptr->lcd_dev,left_rect,MMI_WHITE_COLOR);
			bottom_rect.left=top_rect.left;
			bottom_rect.right=top_rect.right;
			bottom_rect.top=bottom_shadow_display_rect.top;
			bottom_rect.bottom=bottom_rect.top+2;
			LCD_FillRect(&crop_info_ptr->lcd_dev,bottom_rect,MMI_WHITE_COLOR);
			right_rect.left=right_shadow_display_rect.left;
			right_rect.right=right_rect.left+2;
			right_rect.top=left_rect.top;
			right_rect.bottom=left_rect.bottom;
			LCD_FillRect(&crop_info_ptr->lcd_dev,right_rect,MMI_WHITE_COLOR);

			crop_info_ptr->crop_frame_rect.left = left_rect.right;
			crop_info_ptr->crop_frame_rect.top =top_rect.bottom;
			crop_info_ptr->crop_frame_rect.right = right_rect.left;
			crop_info_ptr->crop_frame_rect.bottom = bottom_rect.top;
		}else
		{
			top_rect.left=crop_info_ptr->display_rect.left-2;
			top_rect.right=crop_info_ptr->display_rect.right+2;
			top_rect.top=crop_info_ptr->display_rect.top-2;
			top_rect.bottom=crop_info_ptr->display_rect.top;
			LCD_FillRect(&crop_info_ptr->lcd_dev,top_rect,MMI_WHITE_COLOR);
			left_rect.left=top_rect.left;
			left_rect.right=left_rect.left+2;
			left_rect.top=crop_info_ptr->display_rect.top;
			left_rect.bottom=crop_info_ptr->display_rect.bottom;
			LCD_FillRect(&crop_info_ptr->lcd_dev,left_rect,MMI_WHITE_COLOR);
			bottom_rect.left=top_rect.left;
			bottom_rect.right=top_rect.right;
			bottom_rect.top=crop_info_ptr->display_rect.bottom-2;
			bottom_rect.bottom=crop_info_ptr->display_rect.bottom;
			LCD_FillRect(&crop_info_ptr->lcd_dev,bottom_rect,MMI_WHITE_COLOR);
			right_rect.left=crop_info_ptr->display_rect.right;
			right_rect.right=crop_info_ptr->display_rect.right+2;
			right_rect.top=left_rect.top;
			right_rect.bottom=left_rect.bottom;
			LCD_FillRect(&crop_info_ptr->lcd_dev,right_rect,MMI_WHITE_COLOR);

			crop_info_ptr->crop_frame_rect.left = left_rect.right;
			crop_info_ptr->crop_frame_rect.top =top_rect.bottom;
			crop_info_ptr->crop_frame_rect.right = right_rect.left;
			crop_info_ptr->crop_frame_rect.bottom = bottom_rect.top;
		}

		/*LCD_DrawRect(&crop_info_ptr->lcd_dev, crop_info_ptr->crop_frame_rect, MMI_WHITE_COLOR);
        rect.left = crop_info_ptr->crop_frame_rect.left - 1;
        rect.top = crop_info_ptr->crop_frame_rect.top - 1;
        rect.right = crop_info_ptr->crop_frame_rect.right - 1;
        rect.bottom = crop_info_ptr->crop_frame_rect.bottom - 1;
        LCD_DrawRect(&crop_info_ptr->lcd_dev, rect, MMI_WHITE_COLOR);
        point1.x = crop_info_ptr->crop_frame_rect.left + (crop_info_ptr->crop_frame_rect.right - crop_info_ptr->crop_frame_rect.left)/3;
        point1.y = crop_info_ptr->crop_frame_rect.top;
        point2.x = point1.x;
        point2.y = crop_info_ptr->crop_frame_rect.bottom;
        LCD_DrawLine(&crop_info_ptr->lcd_dev, point1.x, point1.y, point2.x, point2.y, MMI_WHITE_COLOR);
        point1.x = crop_info_ptr->crop_frame_rect.left + (crop_info_ptr->crop_frame_rect.right - crop_info_ptr->crop_frame_rect.left)*2/3;
        point1.y = crop_info_ptr->crop_frame_rect.top;
        point2.x = point1.x;
        point2.y = crop_info_ptr->crop_frame_rect.bottom;
        LCD_DrawLine(&crop_info_ptr->lcd_dev, point1.x, point1.y, point2.x, point2.y, MMI_WHITE_COLOR);
        point1.x = crop_info_ptr->crop_frame_rect.left;
        point1.y = crop_info_ptr->crop_frame_rect.top + (crop_info_ptr->crop_frame_rect.bottom - crop_info_ptr->crop_frame_rect.top)/3;;
        point2.x = crop_info_ptr->crop_frame_rect.right;
        point2.y = point1.y;
        LCD_DrawLine(&crop_info_ptr->lcd_dev, point1.x, point1.y, point2.x, point2.y, MMI_WHITE_COLOR);
        point1.x = crop_info_ptr->crop_frame_rect.left;
        point1.y = crop_info_ptr->crop_frame_rect.top + (crop_info_ptr->crop_frame_rect.bottom - crop_info_ptr->crop_frame_rect.top)*2/3;;
        point2.x = crop_info_ptr->crop_frame_rect.right;
        point2.y = point1.y;
        LCD_DrawLine(&crop_info_ptr->lcd_dev, point1.x, point1.y, point2.x, point2.y, MMI_WHITE_COLOR);*/
    }
    /*if(MMIPICCROP_IsPermitCrop())
    {
        point1.x = crop_info_ptr->crop_frame_rect.left - crop_info_ptr->frame_img_width/2;
        point1.y = crop_info_ptr->crop_frame_rect.top - crop_info_ptr->frame_img_width/2;
        GUIRES_DisplayImg(&point1,
            PNULL,
            PNULL,
            MMIPIC_CROP_MAIN_WIN_ID,
            IMAGE_PICCROP_POINT,
            &crop_info_ptr->lcd_dev);
        point1.x = crop_info_ptr->crop_frame_rect.right - crop_info_ptr->frame_img_width/2;
        point1.y = crop_info_ptr->crop_frame_rect.top - crop_info_ptr->frame_img_width/2;
        GUIRES_DisplayImg(&point1,
            PNULL,
            PNULL,
            MMIPIC_CROP_MAIN_WIN_ID,
            IMAGE_PICCROP_POINT,
            &crop_info_ptr->lcd_dev);
        point1.x = crop_info_ptr->crop_frame_rect.left - crop_info_ptr->frame_img_width/2;
        point1.y = crop_info_ptr->crop_frame_rect.bottom - crop_info_ptr->frame_img_width/2;
        GUIRES_DisplayImg(&point1,
            PNULL,
            PNULL,
            MMIPIC_CROP_MAIN_WIN_ID,
            IMAGE_PICCROP_POINT,
            &crop_info_ptr->lcd_dev);
        point1.x = crop_info_ptr->crop_frame_rect.right - crop_info_ptr->frame_img_width/2;
        point1.y = crop_info_ptr->crop_frame_rect.bottom - crop_info_ptr->frame_img_width/2;
        GUIRES_DisplayImg(&point1,
            PNULL,
            PNULL,
            MMIPIC_CROP_MAIN_WIN_ID,
            IMAGE_PICCROP_POINT,
            &crop_info_ptr->lcd_dev);
    }*/
}

//lijinghua add for square crop
LOCAL void SquareCropFrameDisplay(void)
{
    GUI_RECT_T rect = {0};
    GUI_POINT_T point1 = {0};
    GUI_POINT_T point2 = {0};

    MMIPICCROP_INFO_T* crop_info_ptr = MMIPICCROP_GetCropInfoPtr();

    if(PNULL == crop_info_ptr)
    {
        //SCI_TRACE_LOW:"[MMIPICCROP] CropFrameDisplay crop_info_ptr is PNULL !!!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIC_CROP_1434_112_2_18_2_46_13_28,(uint8*)"");
        return ;
    }    

    if(crop_info_ptr->original_img.image_width > 4 
        && crop_info_ptr->original_img.image_height > 4)
    {
#ifdef PIC_EDITOR_SUPPORT
		if(PIC_CROP_GetSquareCropStatus()&& square_crop_index >0)
	    {
			top_rect.left=left_shadow_display_rect.right-2;
			top_rect.right=right_shadow_display_rect.left+2;
			top_rect.top=top_shadow_display_rect.bottom-2;
			top_rect.bottom=top_shadow_display_rect.bottom;
			LCD_FillRect(&crop_info_ptr->lcd_dev,top_rect,MMI_WHITE_COLOR);
			left_rect.left=top_rect.left;
			left_rect.right=left_rect.left+2;
			left_rect.top=top_shadow_display_rect.bottom;
			left_rect.bottom=bottom_shadow_display_rect.top;
			LCD_FillRect(&crop_info_ptr->lcd_dev,left_rect,MMI_WHITE_COLOR);
			bottom_rect.left=top_rect.left;
			bottom_rect.right=top_rect.right;
			bottom_rect.top=bottom_shadow_display_rect.top;
			bottom_rect.bottom=bottom_rect.top+2;
			LCD_FillRect(&crop_info_ptr->lcd_dev,bottom_rect,MMI_WHITE_COLOR);
			right_rect.left=right_shadow_display_rect.left;
			right_rect.right=right_rect.left+2;
			right_rect.top=left_rect.top;
			right_rect.bottom=left_rect.bottom;
			LCD_FillRect(&crop_info_ptr->lcd_dev,right_rect,MMI_WHITE_COLOR);

			crop_info_ptr->crop_frame_rect.left = left_rect.right;
			crop_info_ptr->crop_frame_rect.top =top_rect.bottom;
			crop_info_ptr->crop_frame_rect.right = right_rect.left;
			crop_info_ptr->crop_frame_rect.bottom = bottom_rect.top;
		}else
#endif
		{
			top_rect.left=crop_info_ptr->display_rect.left-2;
			top_rect.right=crop_info_ptr->display_rect.right+2;
			top_rect.top=top_shadow_display_rect.bottom-2;
			top_rect.bottom=top_shadow_display_rect.bottom;
			LCD_FillRect(&crop_info_ptr->lcd_dev,top_rect,MMI_WHITE_COLOR);
			left_rect.left=top_rect.left;
			left_rect.right=left_rect.left+2;
			left_rect.top=top_shadow_display_rect.bottom;
			left_rect.bottom=bottom_shadow_display_rect.top;
			LCD_FillRect(&crop_info_ptr->lcd_dev,left_rect,MMI_WHITE_COLOR);
			bottom_rect.left=top_rect.left;
			bottom_rect.right=top_rect.right;
			bottom_rect.top=bottom_shadow_display_rect.top-2;
			bottom_rect.bottom=bottom_shadow_display_rect.top;
			LCD_FillRect(&crop_info_ptr->lcd_dev,bottom_rect,MMI_WHITE_COLOR);
			right_rect.left=crop_info_ptr->display_rect.right;
			right_rect.right=crop_info_ptr->display_rect.right+2;
			right_rect.top=left_rect.top;
			right_rect.bottom=left_rect.bottom;
			LCD_FillRect(&crop_info_ptr->lcd_dev,right_rect,MMI_WHITE_COLOR);

			crop_info_ptr->crop_frame_rect.left = left_rect.right;
			crop_info_ptr->crop_frame_rect.top =top_rect.bottom;
			crop_info_ptr->crop_frame_rect.right = right_rect.left;
			crop_info_ptr->crop_frame_rect.bottom = bottom_rect.top;
		}
	}
}
//lijinghua end
//lijinghua add for landscape crop
LOCAL void LandscapeCropFrameDisplay(void)
{
    GUI_RECT_T rect = {0};
    GUI_POINT_T point1 = {0};
    GUI_POINT_T point2 = {0};

    MMIPICCROP_INFO_T* crop_info_ptr = MMIPICCROP_GetCropInfoPtr();

    if(PNULL == crop_info_ptr)
    {
        //SCI_TRACE_LOW:"[MMIPICCROP] CropFrameDisplay crop_info_ptr is PNULL !!!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIC_CROP_1434_112_2_18_2_46_13_28,(uint8*)"");
        return ;
    }    

    if(crop_info_ptr->original_img.image_width > 4 
        && crop_info_ptr->original_img.image_height > 4)
    {
#ifdef PIC_EDITOR_SUPPORT
		if(PIC_CROP_GetLandscapeCropStatus()&& landscape_crop_index >0)
	    {
			top_rect.left=left_shadow_display_rect.right-2;
			top_rect.right=right_shadow_display_rect.left+2;
			top_rect.top=top_shadow_display_rect.bottom-2;
			top_rect.bottom=top_shadow_display_rect.bottom;
			LCD_FillRect(&crop_info_ptr->lcd_dev,top_rect,MMI_WHITE_COLOR);
			left_rect.left=top_rect.left;
			left_rect.right=left_rect.left+2;
			left_rect.top=top_shadow_display_rect.bottom;
			left_rect.bottom=bottom_shadow_display_rect.top;
			LCD_FillRect(&crop_info_ptr->lcd_dev,left_rect,MMI_WHITE_COLOR);
			bottom_rect.left=top_rect.left;
			bottom_rect.right=top_rect.right;
			bottom_rect.top=bottom_shadow_display_rect.top;
			bottom_rect.bottom=bottom_rect.top+2;
			LCD_FillRect(&crop_info_ptr->lcd_dev,bottom_rect,MMI_WHITE_COLOR);
			right_rect.left=right_shadow_display_rect.left;
			right_rect.right=right_rect.left+2;
			right_rect.top=left_rect.top;
			right_rect.bottom=left_rect.bottom;
			LCD_FillRect(&crop_info_ptr->lcd_dev,right_rect,MMI_WHITE_COLOR);

			crop_info_ptr->crop_frame_rect.left = left_rect.right;
			crop_info_ptr->crop_frame_rect.top =top_rect.bottom;
			crop_info_ptr->crop_frame_rect.right = right_rect.left;
			crop_info_ptr->crop_frame_rect.bottom = bottom_rect.top;
		}else
#endif
		{
			top_rect.left=crop_info_ptr->display_rect.left-2;
			top_rect.right=crop_info_ptr->display_rect.right+2;
			top_rect.top=top_shadow_display_rect.bottom-2;
			top_rect.bottom=top_shadow_display_rect.bottom;
			LCD_FillRect(&crop_info_ptr->lcd_dev,top_rect,MMI_WHITE_COLOR);
			left_rect.left=top_rect.left;
			left_rect.right=left_rect.left+2;
			left_rect.top=top_shadow_display_rect.bottom;
			left_rect.bottom=bottom_shadow_display_rect.top;
			LCD_FillRect(&crop_info_ptr->lcd_dev,left_rect,MMI_WHITE_COLOR);
			bottom_rect.left=top_rect.left;
			bottom_rect.right=top_rect.right;
			bottom_rect.top=bottom_shadow_display_rect.top-2;
			bottom_rect.bottom=bottom_shadow_display_rect.top;
			LCD_FillRect(&crop_info_ptr->lcd_dev,bottom_rect,MMI_WHITE_COLOR);
			right_rect.left=crop_info_ptr->display_rect.right;
			right_rect.right=crop_info_ptr->display_rect.right+2;
			right_rect.top=left_rect.top;
			right_rect.bottom=left_rect.bottom;
			LCD_FillRect(&crop_info_ptr->lcd_dev,right_rect,MMI_WHITE_COLOR);

			crop_info_ptr->crop_frame_rect.left = left_rect.right;
			crop_info_ptr->crop_frame_rect.top =top_rect.bottom;
			crop_info_ptr->crop_frame_rect.right = right_rect.left;
			crop_info_ptr->crop_frame_rect.bottom = bottom_rect.top;
		}
	}
}
//lijinghua end
/*****************************************************************************/
//  Description : display image
//  Global resource dependence : 
//  Author: chunyou
//  Note: 
/*****************************************************************************/
PUBLIC void MMIPICCROP_DisplayImage(MMI_MESSAGE_ID_E    msg_id)
{
    GUI_RECT_T      display_rect = MMIPICCROP_GetClientRect();
    GUI_POINT_T     dis_point = {0}; 
    MMIPICCROP_INFO_T* crop_info_ptr = MMIPICCROP_GetCropInfoPtr();
	MMI_STRING_T		title_str = {0};
    //SCI_TRACE_LOW:"[MMIPICCROP] MMIPICCROP_DisplayImage"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIC_CROP_1515_112_2_18_2_46_14_29,(uint8*)"");
    if(PNULL == crop_info_ptr)
    {
        //SCI_TRACE_LOW:"[MMIPICCROP] MMIPICCROP_DisplayImage crop_info_ptr is PNULL !!!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIC_CROP_1518_112_2_18_2_46_14_30,(uint8*)"");
        return ;
    }
       
   if (PNULL == crop_info_ptr->display_bitmap_ptr)
   {
        //SCI_TRACE_LOW:"[MMIPICCROP] MMIPICCROP_DisplayImage display_bitmap_ptr is PNULL !!!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIC_CROP_1524_112_2_18_2_46_14_31,(uint8*)"");
        return ;
   }
   
   if (PNULL == crop_info_ptr->display_bitmap_ptr->bit_ptr)
   {
        //SCI_TRACE_LOW:"[MMIPICCROP] MMIPICCROP_DisplayImage display_bitmap_ptr->bit_ptr is PNULL !!!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIC_CROP_1530_112_2_18_2_46_14_32,(uint8*)"");
        return ;
   }
	if(crop_info_ptr->crop_type == MMIPIC_CROP_PB)
	{
		MMIPICCROP_DisplayImageForPB(msg_id);
		GUIWIN_SetSoftkeyBtnGray(MMIPIC_CROP_MAIN_WIN_ID,0,FALSE);
	}else
	{
		   //image_width = crop_info_ptr->display_bitmap_ptr->img_width;
		   //image_height = crop_info_ptr->display_bitmap_ptr->img_height;
		   
		   //GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID,&lcd_width,&lcd_height);	   
		
		   CropDisplayBgColor();
		   MMI_GetLabelTextByLang(STR_CONTA_HDR_CROP_EXT01, &title_str );
		   if(MMIAPICOM_StatusAreaSetViewHeaderinfo(title_str .wstr_ptr, title_str .wstr_len))
		   {
				 GUIWIN_UpdateStb();
		   }
		   /*GUIRES_DisplayImg(PNULL,
				&display_rect,
				&display_rect,
				MMIPIC_CROP_MAIN_WIN_ID,
				IMAGE_COMMON_BG,
				&crop_info_ptr->lcd_dev);	   */
		   //lijinghua add it
		   display_rect.top = display_rect.top + 30;
		   display_rect.bottom = display_rect.bottom - 6;
		   if(crop_info_ptr->display_rect.top < 30)
		   {
			   crop_info_ptr->display_rect.top = display_rect.top;
		   }
		   //crop_result=PIC_CROP_GetCropStatus();
		   crop_index=PIC_CROP_GetCropIndex();
		   square_crop_index=PIC_SQUARE_CROP_GetCropIndex();
		   landscape_crop_index=PIC_LANDSCAPE_CROP_GetCropIndex();
		   dis_point.x = crop_info_ptr->display_rect.left;
		   dis_point.y = crop_info_ptr->display_rect.top;
		//lijinghua modify it
		   GUIIMG_DisplayBmp(FALSE,
				&crop_info_ptr->display_rect,
				&dis_point,
				crop_info_ptr->display_bitmap_ptr,
				&crop_info_ptr->lcd_dev);
#ifdef PIC_EDITOR_SUPPORT
			   if(crop_index ==0 && !PIC_CROP_GetSquareCropStatus() && !PIC_CROP_GetLandscapeCropStatus())
			   {
				  CropFrameDisplay();
				  GUIWIN_SetSoftkeyBtnGray(MMIPIC_CROP_MAIN_WIN_ID,0,TRUE);
			   }else if(crop_index>0 && !PIC_CROP_GetSquareCropStatus() && !PIC_CROP_GetLandscapeCropStatus())
			   {
				   GetCropShadowRect(crop_info_ptr,s_icon_move_up_or_down_crop_index,s_icon_move_left_or_right_crop_index);
				   CropFrameDisplay();
				   GUIWIN_SetSoftkeyBtnGray(MMIPIC_CROP_MAIN_WIN_ID,0,FALSE);
			   }else if(PIC_CROP_GetSquareCropStatus())
			   {
				   GetSquareCropShadowRect(crop_info_ptr,s_icon_move_up_or_down_crop_index,s_icon_move_left_or_right_crop_index);
				   SquareCropFrameDisplay();
				   GUIWIN_SetSoftkeyBtnGray(MMIPIC_CROP_MAIN_WIN_ID,0,FALSE);
			   }else if(PIC_CROP_GetLandscapeCropStatus())
			   {
				   GetLandscapeCropShadowRect(crop_info_ptr,s_icon_move_up_or_down_crop_index,s_icon_move_left_or_right_crop_index);
				   LandscapeCropFrameDisplay();
				   GUIWIN_SetSoftkeyBtnGray(MMIPIC_CROP_MAIN_WIN_ID,0,FALSE);
			   }
#endif
			if(MMIPIC_CROP_WALLPAPER == crop_info_ptr->crop_type)
			{
			    GUIWIN_SetSoftkeyBtnGray(MMIPIC_CROP_MAIN_WIN_ID,LEFT_BUTTON,FALSE);
			}
			//CropDisplayTitle(); 
	}
//lijinghua modify end
}

/*****************************************************************************/
//  Description : display image for pb
//  Global resource dependence : 
//  Author: lijinghua
//  Note: 
/*****************************************************************************/
PUBLIC void MMIPICCROP_DisplayImageForPB(MMI_MESSAGE_ID_E    msg_id)
{
   GUI_RECT_T      display_rect = MMIPICCROP_GetClientRect();
   GUI_POINT_T     dis_point = {0}; 
   MMIPICCROP_INFO_T* crop_info_ptr = MMIPICCROP_GetCropInfoPtr();
   uint16					radius=0;
   int16					x=0;
   int16					y=0;
   uint16  lcd_width = 0;
   uint16  lcd_height = 0;
   uint16 		    new_crop_index=0;
   MMI_STRING_T 	   title_str = {0};

   //SCI_TRACE_LOW:"[MMIPICCROP] MMIPICCROP_DisplayImage"
   SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIC_CROP_1515_112_2_18_2_46_14_29,(uint8*)"");
   if(PNULL == crop_info_ptr)
   {
       //SCI_TRACE_LOW:"[MMIPICCROP] MMIPICCROP_DisplayImage crop_info_ptr is PNULL !!!"
       SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIC_CROP_1518_112_2_18_2_46_14_30,(uint8*)"");
       return ;
   }
      
  if (PNULL == crop_info_ptr->display_bitmap_ptr)
  {
       //SCI_TRACE_LOW:"[MMIPICCROP] MMIPICCROP_DisplayImage display_bitmap_ptr is PNULL !!!"
       SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIC_CROP_1524_112_2_18_2_46_14_31,(uint8*)"");
       return ;
  }
  
  if (PNULL == crop_info_ptr->display_bitmap_ptr->bit_ptr)
  {
       //SCI_TRACE_LOW:"[MMIPICCROP] MMIPICCROP_DisplayImage display_bitmap_ptr->bit_ptr is PNULL !!!"
       SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIC_CROP_1530_112_2_18_2_46_14_32,(uint8*)"");
       return ;
  }

  //image_width = crop_info_ptr->display_bitmap_ptr->img_width;
  //image_height = crop_info_ptr->display_bitmap_ptr->img_height;
  
  //GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID,&lcd_width,&lcd_height);       
    CropDisplayBgColor();
    MMI_GetLabelTextByLang(STR_CONTA_HDR_CROP_EXT01, &title_str );
	if(MMIAPICOM_StatusAreaSetViewHeaderinfo(title_str .wstr_ptr, title_str .wstr_len))
	{
	      GUIWIN_UpdateStb();
	}
  /*GUIRES_DisplayImg(PNULL,
       &display_rect,
       &display_rect,
       MMIPIC_CROP_MAIN_WIN_ID,
       IMAGE_COMMON_BG,
       &crop_info_ptr->lcd_dev);      */

    display_rect.top = display_rect.top + 30;
//    display_rect.bottom = display_rect.bottom;
    if(crop_info_ptr->display_rect.top < 30)
    {
        crop_info_ptr->display_rect.top = display_rect.top;
    }
    dis_point.x = crop_info_ptr->display_rect.left;
    dis_point.y = crop_info_ptr->display_rect.top;
    GUIIMG_DisplayBmp(FALSE,
        &display_rect,
        &dis_point,
        crop_info_ptr->display_bitmap_ptr,
        &crop_info_ptr->lcd_dev);
    //CropFrameDisplay();
    //CropDisplayTitle(); 
    crop_index=PIC_CROP_GetCropIndex();
    GUILCD_GetLogicWidthHeight (GUI_MAIN_LCD_ID, &lcd_width, &lcd_height);
    x=crop_info_ptr->display_rect.left+(crop_info_ptr->display_rect.right-crop_info_ptr->display_rect.left)/2-s_icon_move_left_or_right_crop_index;
    y=crop_info_ptr->display_rect.top+(crop_info_ptr->display_rect.bottom-crop_info_ptr->display_rect.top)/2-s_icon_move_up_or_down_crop_index;
    if(crop_index==0)
    {
        if(crop_info_ptr->display_bitmap_ptr->img_height<176 || crop_info_ptr->display_bitmap_ptr->img_width<176)
        {
            radius=MIN(crop_info_ptr->display_bitmap_ptr->img_height,crop_info_ptr->display_bitmap_ptr->img_width);
            radius=radius/2;
            
        }else
        {
            radius=176/2;
        }
        clip_rect_ptr.left=x-radius;
        clip_rect_ptr.top=y-radius;
        clip_rect_ptr.right=x+radius;
        clip_rect_ptr.bottom=y+radius;
        //  LCD_DrawCircle(&crop_info_ptr->lcd_dev,&clip_rect_ptr,x,y,radius,MMI_WHITE_COLOR);
        LCD_FillLayerExceptRoundField(&crop_info_ptr->lcd_dev,x,y,radius,0x0000,60,2,MMI_WHITE_COLOR);
        crop_info_ptr->crop_frame_rect=clip_rect_ptr;
    }else
    {
        new_crop_index=2*(crop_index+1);
        if(crop_info_ptr->display_bitmap_ptr->img_height<176 || crop_info_ptr->display_bitmap_ptr->img_width<176)
        {
            radius=MIN(crop_info_ptr->display_bitmap_ptr->img_height,crop_info_ptr->display_bitmap_ptr->img_width);
            radius=radius/new_crop_index;
            
        }else
        {
            radius=176/new_crop_index;
        }
        clip_rect_ptr.left=x-radius;
        clip_rect_ptr.top=y-radius;
        clip_rect_ptr.right=x+radius;
        clip_rect_ptr.bottom=y+radius;
        //  LCD_DrawCircle(&crop_info_ptr->lcd_dev,&clip_rect_ptr,x,y,radius,MMI_WHITE_COLOR);
        LCD_FillLayerExceptRoundField(&crop_info_ptr->lcd_dev,x,y,radius,0x0000,60,2,MMI_WHITE_COLOR);
        crop_info_ptr->crop_frame_rect=clip_rect_ptr;
    }
    //LCD_FillLayerExceptRoundField(&crop_info_ptr->lcd_dev,100,100,50,0x0000,100,0,0);
}

/*****************************************************************************/
//  Description : error callback
//  Global resource dependence : 
//  Author: chunyou
//  Note: 
/*****************************************************************************/
PUBLIC void MMIPICCROP_ErrorCallback(MMI_PICCROP_ERROR_TYPE_E err_type)
{
    MMIPICCROP_INFO_T* crop_info_ptr = MMIPICCROP_GetCropInfoPtr();

    if(PNULL == crop_info_ptr)
    {
        //SCI_TRACE_LOW:"[MMIPICCROP] MMIPICCROP_ErrorCallback crop_info_ptr is PNULL !!!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIC_CROP_1568_112_2_18_2_46_14_33,(uint8*)"");
        return;
    }

    if(PNULL != crop_info_ptr->callback_fun)
    {
        crop_info_ptr->callback_t.full_path_ptr = crop_info_ptr->file_info.full_path_wstr_ptr;
        crop_info_ptr->callback_t.full_path_len = crop_info_ptr->file_info.full_path_wstr_len;
        crop_info_ptr->callback_t.err_type = err_type;
        crop_info_ptr->callback_t.type = crop_info_ptr->crop_type;
        crop_info_ptr->callback_t.is_pre_wallpaper = crop_info_ptr->is_pre_wallpaper;
        crop_info_ptr->callback_fun(&crop_info_ptr->callback_t);
    }
    //SCI_TRACE_LOW:"[MMIPICCROP] MMIPICCROP_ErrorCallback err_type = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIC_CROP_1581_112_2_18_2_46_14_34,(uint8*)"d", err_type);
    return;
}

/*****************************************************************************/
//  Description : Malloc  PROCESS TARGET  Mem
//  Global resource dependence : 
//  Author: chunyou
//  Note: 
/*****************************************************************************/
PUBLIC int32 MMIPICCROP_MallocMemForPicProcess(void)
{
    int32 ret = FALSE;
    MMIPICCROP_INFO_T* crop_info_ptr = MMIPICCROP_GetCropInfoPtr();

    if(PNULL == crop_info_ptr)
    {
        //SCI_TRACE_LOW:"[MMIPICCROP] MMIPICCROP_MallocMemForPicProcess crop_info_ptr is PNULL !!!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIC_CROP_1596_112_2_18_2_46_14_35,(uint8*)"");
        return ret;
    }
    
    /*crop_info_ptr->process_buf_ptr= (uint8*)SCI_ALLOCA(crop_info_ptr->process_buffer_size);
    if (PNULL == crop_info_ptr->process_buf_ptr)
    {
        //MMIPICCROP_ASSERT_WARNING:"[MMIPICCROP] MMIPICCROP_MallocMemForPicProcess s_pic_editor_info.process_buf_ptr is PNULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIC_CROP_1603_112_2_18_2_46_14_36,(uint8*)"");
        return ret;
    }
    
    crop_info_ptr->output_buf_ptr= (uint8*)SCI_ALLOCA(crop_info_ptr->output_buffer_size);
    if (PNULL == crop_info_ptr->output_buf_ptr)
    {
        //MMIPICCROP_ASSERT_WARNING:"[MMIPICCROP] MMIPICCROP_MallocMemForPicProcess s_pic_editor_info.output_buf_ptr is PNULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIC_CROP_1610_112_2_18_2_46_14_37,(uint8*)"");
        return ret;
    }

    crop_info_ptr->jpg_buf_ptr= (uint8*)SCI_ALLOCA(crop_info_ptr->jpg_buffer_size);
    if (PNULL == crop_info_ptr->jpg_buf_ptr)
    {
        //MMIPICCROP_ASSERT_WARNING:"[MMIPICCROP] MMIPICCROP_MallocMemForPicProcess s_pic_editor_info.jpg_buf_ptr is PNULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIC_CROP_1617_112_2_18_2_46_14_38,(uint8*)"");
        return ret;
    }*/
    ret = TRUE;
    
    return ret;
}

/*****************************************************************************/
//  Description : pic crop exit
//  Global resource dependence : 
//  Author: chunyou
//  Note: 
/*****************************************************************************/
PUBLIC void MMIPICCROP_Exit(void)
{
    MMIPICCROP_INFO_T* crop_info_ptr = MMIPICCROP_GetCropInfoPtr();

    if(PNULL == crop_info_ptr)
    {
        //SCI_TRACE_LOW:"[MMIPICCROP] MMIPICCROP_Exit crop_info_ptr is PNULL !!!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIC_CROP_1635_112_2_18_2_46_14_39,(uint8*)"");
        return ;
    }

    MMINV_WRITE(MMINV_PICCROP_SETTING, &crop_info_ptr->setting_info);
    /*if(NULL !=crop_info_ptr->process_buf_ptr)
    {
        SCI_FREE(crop_info_ptr->process_buf_ptr);
        crop_info_ptr->process_buf_ptr=NULL;
    }

    if(NULL !=crop_info_ptr->output_buf_ptr)
    {
        SCI_FREE(crop_info_ptr->output_buf_ptr);
        crop_info_ptr->output_buf_ptr=NULL;
    }*/
    if(NULL !=crop_info_ptr->display_bitmap_ptr)
    {
        if(NULL !=crop_info_ptr->display_bitmap_ptr->bit_ptr)
        {
            SCI_FREE(crop_info_ptr->display_bitmap_ptr->bit_ptr);
            crop_info_ptr->display_bitmap_ptr->bit_ptr=NULL;
        }
        SCI_FREE(crop_info_ptr->display_bitmap_ptr);
        crop_info_ptr->display_bitmap_ptr=NULL;
    }
    if(NULL !=crop_info_ptr->crop_bitmap_ptr)
    {
        if(NULL !=crop_info_ptr->crop_bitmap_ptr->bit_ptr)
        {
            SCI_FREE(crop_info_ptr->crop_bitmap_ptr->bit_ptr);
            crop_info_ptr->crop_bitmap_ptr->bit_ptr=NULL;
        }
        SCI_FREE(crop_info_ptr->crop_bitmap_ptr);
        crop_info_ptr->crop_bitmap_ptr=NULL;
    }
    if(PNULL != crop_info_ptr->random_name_ptr)
    {
        SCI_FREE(crop_info_ptr->random_name_ptr);
        crop_info_ptr->random_name_ptr = PNULL;
    }

    if(PNULL != crop_info_ptr->file_info.full_path_wstr_ptr)
    {
        SCI_FREE(crop_info_ptr->file_info.full_path_wstr_ptr);
        crop_info_ptr->file_info.full_path_wstr_ptr = PNULL;
    }
    
}
//lijinghua add
PUBLIC void GetCropShadowRect(MMIPICCROP_INFO_T* crop_info_ptr,int16 move_up_or_down,int16 move_left_or_right)
{
    uint16          crop_img_width=0;
	uint16          crop_img_height=0;
	crop_img_width=(crop_info_ptr->display_rect.right-crop_info_ptr->display_rect.left+1)/2;
	crop_img_height=(crop_info_ptr->display_rect.bottom-crop_info_ptr->display_rect.top+1)/2;

	left_shadow_display_rect.left=crop_info_ptr->display_rect.left;
	left_shadow_display_rect.right=crop_info_ptr->display_rect.left+crop_img_width/2+(crop_index-1)*crop_img_width/4-move_left_or_right;
	left_shadow_display_rect.top=crop_info_ptr->display_rect.top;
	left_shadow_display_rect.bottom=crop_info_ptr->display_rect.bottom;
	LCD_FillArgbRect(MMITHEME_GetDefaultLcdDev(),left_shadow_display_rect,MMI_BLACK_COLOR,255 * 60 / 100);

	top_shadow_display_rect.left=left_shadow_display_rect.right+1;
	top_shadow_display_rect.top=left_shadow_display_rect.top;
	top_shadow_display_rect.right=top_shadow_display_rect.left+crop_img_width/crop_index;
	top_shadow_display_rect.bottom=top_shadow_display_rect.top+crop_img_height/2+(crop_index-1)*crop_img_height/4-move_up_or_down;
	LCD_FillArgbRect(MMITHEME_GetDefaultLcdDev(),top_shadow_display_rect,MMI_BLACK_COLOR,255 * 60 / 100);

	right_shadow_display_rect.left=left_shadow_display_rect.right+crop_img_width/crop_index+2;
	right_shadow_display_rect.top=left_shadow_display_rect.top;
	right_shadow_display_rect.right=crop_info_ptr->display_rect.right;
	right_shadow_display_rect.bottom=left_shadow_display_rect.bottom;
	LCD_FillArgbRect(MMITHEME_GetDefaultLcdDev(),right_shadow_display_rect,MMI_BLACK_COLOR,255 * 60 / 100);

	bottom_shadow_display_rect.left=top_shadow_display_rect.left;
	bottom_shadow_display_rect.top=top_shadow_display_rect.bottom+crop_img_height/crop_index;
	bottom_shadow_display_rect.right=top_shadow_display_rect.right;
	bottom_shadow_display_rect.bottom=left_shadow_display_rect.bottom;
	LCD_FillArgbRect(MMITHEME_GetDefaultLcdDev(),bottom_shadow_display_rect,MMI_BLACK_COLOR,255 * 60 / 100);
}

PUBLIC void GetSquareCropShadowRect(MMIPICCROP_INFO_T* crop_info_ptr,int16 move_up_or_down,int16 move_left_or_right)
{
    uint16          crop_img_width=0;
	uint16          crop_img_height=0;

	crop_img_width=(crop_info_ptr->display_rect.right-crop_info_ptr->display_rect.left+1)/2;
	crop_img_height=(crop_info_ptr->display_rect.bottom-crop_info_ptr->display_rect.top -65 +1)/2;
    if(square_crop_index ==0)
	{
		top_shadow_display_rect.left=crop_info_ptr->display_rect.left;
		top_shadow_display_rect.top=crop_info_ptr->display_rect.top;
		top_shadow_display_rect.right=crop_info_ptr->display_rect.right;
		top_shadow_display_rect.bottom=crop_info_ptr->display_rect.top + 34 - move_up_or_down;
		LCD_FillArgbRect(MMITHEME_GetDefaultLcdDev(),top_shadow_display_rect,MMI_BLACK_COLOR,255 * 60 / 100);

		bottom_shadow_display_rect.left=top_shadow_display_rect.left;
		if(crop_info_ptr->display_rect.bottom<274)
		{
			bottom_shadow_display_rect.top=top_shadow_display_rect.bottom + 94;
		}else
		{
			bottom_shadow_display_rect.top=top_shadow_display_rect.bottom + 179;
		}
		bottom_shadow_display_rect.right=top_shadow_display_rect.right;
		bottom_shadow_display_rect.bottom=crop_info_ptr->display_rect.bottom;
		LCD_FillArgbRect(MMITHEME_GetDefaultLcdDev(),bottom_shadow_display_rect,MMI_BLACK_COLOR,255 * 60 / 100);
	}else
	{
		left_shadow_display_rect.left=crop_info_ptr->display_rect.left;
		left_shadow_display_rect.right=crop_info_ptr->display_rect.left+crop_img_width/2+(square_crop_index-1)*crop_img_width/4-move_left_or_right;
		left_shadow_display_rect.top=crop_info_ptr->display_rect.top;
		left_shadow_display_rect.bottom=crop_info_ptr->display_rect.bottom;
		LCD_FillArgbRect(MMITHEME_GetDefaultLcdDev(),left_shadow_display_rect,MMI_BLACK_COLOR,255 * 60 / 100);

		top_shadow_display_rect.left=left_shadow_display_rect.right+1;
		top_shadow_display_rect.top=crop_info_ptr->display_rect.top;
		top_shadow_display_rect.right=top_shadow_display_rect.left+crop_img_width/square_crop_index;
		top_shadow_display_rect.bottom=top_shadow_display_rect.top + 34 +crop_img_height/2 +(square_crop_index-1)*crop_img_height/4-move_up_or_down;
		LCD_FillArgbRect(MMITHEME_GetDefaultLcdDev(),top_shadow_display_rect,MMI_BLACK_COLOR,255 * 60 / 100);
		
		right_shadow_display_rect.left=left_shadow_display_rect.right+crop_img_width/square_crop_index+2;
		right_shadow_display_rect.top=left_shadow_display_rect.top;
		right_shadow_display_rect.right=crop_info_ptr->display_rect.right;
		right_shadow_display_rect.bottom=left_shadow_display_rect.bottom;
		LCD_FillArgbRect(MMITHEME_GetDefaultLcdDev(),right_shadow_display_rect,MMI_BLACK_COLOR,255 * 60 / 100);
		
		bottom_shadow_display_rect.left=top_shadow_display_rect.left;
		bottom_shadow_display_rect.top=top_shadow_display_rect.bottom+crop_img_height/square_crop_index;
		bottom_shadow_display_rect.right=top_shadow_display_rect.right;
		bottom_shadow_display_rect.bottom=left_shadow_display_rect.bottom;
		LCD_FillArgbRect(MMITHEME_GetDefaultLcdDev(),bottom_shadow_display_rect,MMI_BLACK_COLOR,255 * 60 / 100);
	}
}
PUBLIC void GetLandscapeCropShadowRect(MMIPICCROP_INFO_T* crop_info_ptr,int16 move_up_or_down,int16 move_left_or_right)
{
    uint16          crop_img_width=0;
	uint16          crop_img_height=0;

	crop_img_width=(crop_info_ptr->display_rect.right-crop_info_ptr->display_rect.left+1)/2;
	crop_img_height=(crop_info_ptr->display_rect.bottom-crop_info_ptr->display_rect.top -99 +1)/2;
    if(landscape_crop_index ==0)
	{
		top_shadow_display_rect.left=crop_info_ptr->display_rect.left;
		top_shadow_display_rect.top=crop_info_ptr->display_rect.top;
		top_shadow_display_rect.right=crop_info_ptr->display_rect.right;
		top_shadow_display_rect.bottom=crop_info_ptr->display_rect.top + 52 - move_up_or_down;
		LCD_FillArgbRect(MMITHEME_GetDefaultLcdDev(),top_shadow_display_rect,MMI_BLACK_COLOR,255 * 60 / 100);

		bottom_shadow_display_rect.left=top_shadow_display_rect.left;
		if(crop_info_ptr->display_rect.bottom<274)
		{
			bottom_shadow_display_rect.top=top_shadow_display_rect.bottom + 70;
		}else
		{
			bottom_shadow_display_rect.top=top_shadow_display_rect.bottom + 135;
		}
		bottom_shadow_display_rect.right=top_shadow_display_rect.right;
		bottom_shadow_display_rect.bottom=crop_info_ptr->display_rect.bottom;
		LCD_FillArgbRect(MMITHEME_GetDefaultLcdDev(),bottom_shadow_display_rect,MMI_BLACK_COLOR,255 * 60 / 100);
	}else
	{
		left_shadow_display_rect.left=crop_info_ptr->display_rect.left;
		left_shadow_display_rect.right=crop_info_ptr->display_rect.left+crop_img_width/2+(landscape_crop_index-1)*crop_img_width/4-move_left_or_right;
		left_shadow_display_rect.top=crop_info_ptr->display_rect.top;
		left_shadow_display_rect.bottom=crop_info_ptr->display_rect.bottom;
		LCD_FillArgbRect(MMITHEME_GetDefaultLcdDev(),left_shadow_display_rect,MMI_BLACK_COLOR,255 * 60 / 100);

		top_shadow_display_rect.left=left_shadow_display_rect.right+1;
		top_shadow_display_rect.top=crop_info_ptr->display_rect.top;
		top_shadow_display_rect.right=top_shadow_display_rect.left+crop_img_width/landscape_crop_index;
		top_shadow_display_rect.bottom=top_shadow_display_rect.top + 52 +crop_img_height/2 +(landscape_crop_index-1)*crop_img_height/4-move_up_or_down;
		LCD_FillArgbRect(MMITHEME_GetDefaultLcdDev(),top_shadow_display_rect,MMI_BLACK_COLOR,255 * 60 / 100);
		
		right_shadow_display_rect.left=left_shadow_display_rect.right+crop_img_width/landscape_crop_index+2;
		right_shadow_display_rect.top=left_shadow_display_rect.top;
		right_shadow_display_rect.right=crop_info_ptr->display_rect.right;
		right_shadow_display_rect.bottom=left_shadow_display_rect.bottom;
		LCD_FillArgbRect(MMITHEME_GetDefaultLcdDev(),right_shadow_display_rect,MMI_BLACK_COLOR,255 * 60 / 100);
		
		bottom_shadow_display_rect.left=top_shadow_display_rect.left;
		bottom_shadow_display_rect.top=top_shadow_display_rect.bottom+crop_img_height/landscape_crop_index;
		bottom_shadow_display_rect.right=top_shadow_display_rect.right;
		bottom_shadow_display_rect.bottom=left_shadow_display_rect.bottom;
		LCD_FillArgbRect(MMITHEME_GetDefaultLcdDev(),bottom_shadow_display_rect,MMI_BLACK_COLOR,255 * 60 / 100);
	}
}

PUBLIC void ResetAllParam()
{
	crop_index=0;
	s_icon_move_up_or_down_crop_index=0;
	s_icon_move_left_or_right_crop_index=0;
	square_crop_index =0;
	landscape_crop_index=0;
	s_icon_is_square_crop = FALSE;
	s_icon_is_landscape_crop = FALSE;
#ifdef PIC_EDITOR_SUPPORT
	PIC_CROP_SetSquareCropStatus(FALSE);
	PIC_CROP_SetLandscapeCropStatus(FALSE);
#endif
}
//lijinghua end
#endif



/*Edit by script, ignore 3 case. Thu Apr 26 19:01:03 2012*/ //IGNORE9527
