/*****************************************************************************
** File Name:      mmipic_crop_export.h                                                  *
** Author:           chunyou                                                     *
** Date:             12/25/2011                                                *
** Copyright:      2011 preadtrum, Incoporated. All Rights Reserved.        *
** Description:    This file is used to mmipic_crop.c  mmipic_crop_wintab.c                 *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 12/2011       chunyou           Create
******************************************************************************/
#ifndef _MMIPIC_CROP_EXPORT_H_
#define _MMIPIC_CROP_EXPORT_H_ 

/**--------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/
#include "mmifmm_export.h"
#include "systypes.h"
/**--------------------------------------------------------------------------*
**                         Compiler Flag                                    *
**--------------------------------------------------------------------------*/
#ifdef __cplusplus
extern   "C"
{
#endif
/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
    **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
**                         TYPE AND CONSTANT                                *
**--------------------------------------------------------------------------*/
//lijinghua add it
typedef GUIANIM_FILE_INFO_T PICCROP_EDIT_FILE_INFO_T;
typedef struct 
{
    uint32              crop_file_id;
}MMIPICCROP_SETTING_INFO_T;
//lijinghua end it
typedef enum
{
    MMIPIC_CROP_WALLPAPER = 0,      //ǽֽ�ü�
    MMIPIC_CROP_DEFAULT_SAVE,        //������Ĭ��·��
    MMIPIC_CROP_CUSTOMIZE_SAVE,        //�������Զ���·��
    MMIPIC_CROP_PB,
    MMIPIC_CROP_TYPE_MAX
}MMIPICCROP_TYPE_E;

typedef enum
{
    MMI_PICCROP_SUCCESS = 0,          //�ɹ�
    MMI_PICCROP_SUCCESS_NO_CROP,          //�ɹ�,û�вü���ͼƬ�ߴ�̫С
    MMI_PICCROP_ERR_NO_SPACE,       // �ռ䲻��
    MMI_PICCROP_ERR_NOT_SUPPORT,       // ��ʽ��֧��
    MMI_PICCROP_ERR_NO_MEMORY,       // �ڴ治��
    MMI_PICCROP_ERR_CANCEL,       // ȡ���ü�
    MMI_PICCROP_ERR_EXIT,            // ������Ҫ�˳�ͼƬ�ü��Ĵ���
}MMI_PICCROP_ERROR_TYPE_E;

typedef struct 
{
    MMIPICCROP_TYPE_E type;     //�ü����ͣ��������ֵһ��
    MMI_PICCROP_ERROR_TYPE_E err_type;  //��������
    wchar* full_path_ptr;       //�ļ�·��  
    uint16 full_path_len;       //�ļ�·������
    BOOLEAN      is_pre_wallpaper;      //�Ƿ��ǻָ�ǰһ��ǽֽ���������ֵһ��
} MMIPICCROP_CB_T;
typedef void (*MMIPICCROP_CALLBACK)(MMIPICCROP_CB_T* callback);
//lijinghua add it for pic crop
typedef struct 
{
    MMIPICCROP_TYPE_E crop_type;    //�ü�����
    GUI_RECT_T      crop_frame_rect;    //�ü�ʱ��ʾ�Ŀ������
    uint16 frame_img_width;     //�ü������ܵ�Բ���
    uint16 frame_img_height;    //�ü������ܵ�Բ���    
    //wchar* selected_folder_ptr;
    wchar* random_name_ptr;     //������ļ���ָ��
    uint16 random_name_len;     //������ļ�������
    //uint32 edit_buffer_size;
    //uint32 process_buffer_size;
    GUI_LCD_DEV_INFO            lcd_dev;            //lcd dev
    PICCROP_EDIT_FILE_INFO_T      file_info;            //�ļ���Ϣ
    GUIIMG_BITMAP_T                  *display_bitmap_ptr;              //��ǰ��ʾbufָ��
    GUIIMG_BITMAP_T                  *crop_bitmap_ptr;      //�ü���buf
    uint32      crop_bitmap_size;       //�ü���buf ��С
    GUI_RECT_T                     display_rect;                    //display rect
    uint8                             *process_buf_ptr;                      //�м� ����ָ��
    //uint8                           *output_buf_ptr;                //�༭������bufָ��
    //uint8                           *jpg_buf_ptr; 
    //uint32 output_buffer_size;
    //uint32 jpg_buffer_size;
    uint32 display_buffer_size;     //��ʾ��buf��С
    //uint16 contact_img_min_width;
    uint16 scale_width;     //�ü�������
    uint16 scale_height;       //�ü�������
    GUIIMG_INFO_T original_img;     //ԭͼ��Ϣ
    MMIPICCROP_CALLBACK callback_fun;   //�ü���ص�����
    MMIPICCROP_CB_T callback_t; //�ص��Ľṹ
    BOOLEAN is_file;    //�Ƿ���������ļ�����trueΪ�ļ�����falseΪbuffer
    uint8       *src_buf_ptr;       /*!< ͼ��buffer��ַ @��Ϊpnullʱ��Ч */
    uint32      src_data_size;      /*!< ͼ��buffer��С @src_buf_ptr��Ϊpnull src_data_size��Ϊ0ʱ��Ч */
    uint16      min_width;                /*!< �ü���ͼƬ����С��ߣ�Ϊ0��ʾ������ */
    uint16      min_height;               /*!< �ü���ͼƬ����С��ߣ�Ϊ0��ʾ������ */
    BOOLEAN      is_pre_wallpaper; //�Ƿ��ǻָ�ǰһ��ǽֽ
    MMIPICCROP_SETTING_INFO_T setting_info;     //������Ϣ
} MMIPICCROP_INFO_T;
//lijinghua end it
typedef struct 
{
    uint16      min_width;                /*!< �ü���ͼƬ����С��ߣ�Ϊ0��ʾ������ */
    uint16      min_height;               /*!< �ü���ͼƬ����С��ߣ�Ϊ0��ʾ������ */
    uint8       *src_buf_ptr;       /*!< ͼ��buffer��ַ @��Ϊpnullʱ��Ч */
    uint32      src_data_size;      /*!< ͼ��buffer��С @src_buf_ptr��Ϊpnull src_data_size��Ϊ0ʱ��Ч */
    wchar      *full_path_ptr;     /*!< ͼ���ļ�·�� @��Ϊpnullʱ��Ч */
    uint16      full_path_len;      /*!< ͼ���ļ�·������ @full_path_ptr��Ϊpnull full_path_len��Ϊ0ʱ��Ч */
    MMIPICCROP_TYPE_E crop_type;
    MMIPICCROP_CALLBACK callback;     /*�ü�ͼƬ�����full_path_len=0��ʾʧ��*/    
    BOOLEAN      is_pre_wallpaper;      //�Ƿ��ǻָ�ǰһ��ǽֽ
} MMIPICCROP_SRC_T;

/**--------------------------------------------------------------------------*
**                         FUNCTION DEFINITION                              *
**--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : exit from pic crop
//  Global resource dependence : none
//  Author: chunyou
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIPICCROP_ExitFromPicCrop(void);

/*****************************************************************************/
//  Description : init pic crop module
//  Global resource dependence : 
//  Author: chunyou
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIPICCROP_InitModule(void);

/*****************************************************************************/
//  Description : open pic crop 
//  Global resource dependence : 
//  Author: chunyou
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIPICCROP_OpenPicCrop(
                                              MMIPICCROP_SRC_T* crop_src    //in:
                                              );
//lijinghua add it for pic crop
PUBLIC MMIPICCROP_INFO_T* MMIPICCROP_GetCropInfoPtr(void);
PUBLIC MMI_PICCROP_ERROR_TYPE_E  MMIPICCROP_Save(void);
//lijinghua end it
#endif

