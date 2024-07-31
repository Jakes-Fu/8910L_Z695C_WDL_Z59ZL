/*****************************************************************************
** File Name:      mmipb_export.h                                                   *
** Author:                                                                   *
** Date:           03/09/2004                                                *
** Copyright:      2003 Spreadtrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to describe phonebook                   *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 09/2004        Tracy Zhang      Create
******************************************************************************/

#ifndef _MMIPB_EXPORT_H_
#define _MMIPB_EXPORT_H_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmk_type.h"
//#include "mmipb_mnapi.h"
//#include "mmk_type.h"
#ifdef MMI_OCR_ENABLE
#include "mmiocr_export.h"
#endif
#include "mmi_atc.h"
//#include "mmiset.h"
#include "mmiset_export.h"
//#include "mmipb_datalist.h"
#include "mmipb_interface.h"
#include "mmisearch_export.h"
#ifdef MMI_PIC_CROP_SUPPORT
#include "mmipic_crop_export.h"
#endif
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
// �ַ�������󳤶�
//#define MMIPB_MAX_STRING_LEN		(/*lint -save -e506 */MAX((MMIPB_NAME_MAX_LEN << 1), 100) /*lint -restore */)
#define MMIPB_MAX_FAVOURITE_NUM      20
/**--------------------------------------------------------------------------*
 **                         TYPE AND CONSTANT                                *
 **--------------------------------------------------------------------------*/
//PB Table������Ӧ������
typedef enum
{
    MMIPB_ALL_CONTACT,
#ifdef MMIPB_MOST_USED_SUPPORT
    MMIPB_MOSTUSED_CONTACT,
#endif
    MMIPB_GROUP_CONTACT,
#ifdef MMI_PDA_SUPPORT
    MMIPB_DIAL_APPLICATION,
    MMIPB_CALLLOG_APPLICATION,
#endif
    MMIPB_TAB_MAX_NUM
}MMIPB_SUB_FUNCTION_TYPE_E;

//��ϵ�˸��������ʾ��ʽ
typedef enum
{
    MMIPB_LIST_FIELD_WHOLE= 0x0,//��ϵ��������ʾ����һ����ϵ��һ����¼
    MMIPB_LIST_FIELD_NUMBER= 0x1,//����ֿ���ʾ����һ������һ����¼
    MMIPB_LIST_FIELD_MAIL =0x2 //Email�ֿ���ʾ����һ��Emailһ����¼
} MMIPB_LIST_FIELD_TYPE_E;


// ��¼�б������
typedef enum _MMIPB_LIST_TYPE_E
{
    MMIPB_LIST_NORMAL_MAIN  = 0x0, //�������б��൱��PB�����棬��option
    MMIPB_LIST_RADIO        = 0x1, //��ѡ�б�
    MMIPB_LIST_CHECK        = 0x2, //��ѡ�б�
} MMIPB_LIST_TYPE_E;

typedef enum _MMIPB_ADD_TYPE_T
{
    MMIPB_ADD_TYPE_MSISDN,
    MMIPB_ADD_TYPE_NUMBER,
    MMIPB_ADD_TYPE_EMAIL,
    MMIPB_ADD_TYPE_URL,
    MMIPB_ADD_TYPE_PIC,

}MMIPB_ADD_TYPE_T;

typedef enum _MMIPB_PREFERRED_SIM_TYPE_T
{
    MMIPB_PREFERRED_SIM_NONE = 0,
    MMIPB_PREFERRED_SIM1,
    MMIPB_PREFERRED_SIM2,
}MMIPB_PREFERRED_SIM_TYPE_T;

// �ַ���
typedef struct _MMIPB_STRING_T
{
	uint16	strlen;
	uint8	strbuf[MMIPB_MAX_STRING_LEN +2];
} MMIPB_STRING_T;

typedef enum _MMIPB_GROUP_MUTI_SELECT_FROM_TYPE_E
{
    MMIPB_GROUP_MUTI_SELECT_FROM_SMS,
    MMIPB_GROUP_MUTI_SELECT_FROM_EMAIL,
    MMIPB_GROUP_MUTI_SELECT_FROM_MAX,
} MMIPB_GROUP_MUTI_SELECT_FROM_TYPE_E;

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : ��ʼ��Phonebook��ȫ�ֱ������ص�������
//  Global resource dependence : g_mmipb_sim_t, g_mmipb_nv_t, g_mmipb_app
//  Author: Tracy.zhang
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIPB_InitApp 
(
	void
);

/*****************************************************************************/
// 	Description : register pb nv and menu
//	Global resource dependence : none
//  Author: bruce.chi
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPIPB_InitModule(void);

/*****************************************************************************/
//  Description : Resort Phonebook,
//  Global resource dependence : 
//  Author: baokun.yin
//RETURN: 
//  Note: ������������pb�����������������sim��ready������Ҫ��CALL_READY��SIM_NOT_READY
/*****************************************************************************/
PUBLIC void MMIAPIPB_ResortPB(void);

#ifdef MMIPB_GROUP_MANAGE_SUPPORT
/*****************************************************************************/
//  Description : reset pb group info
//  Global resource dependence : s_mmipb_group_info
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIPB_ResetPBGroupInfo(void);
#endif
/*****************************************************************************/
//  Description : re_initialize Phonebook,
//  Global resource dependence : 
//  Author: baokun.yin
//RETURN: 
//  Note: ���������ALL_READY��SIM_NOT_READY���ٽ�������
//  [�ر���ʾWarnning]:����������Žӿڣ�һ��Ҫ��֤sim�������·�ALL_READY��SIM_NOT_READY����sim�����³�ʼ��
/*****************************************************************************/
PUBLIC void MMIAPIPB_ReinitPB(MN_DUAL_SYS_E dual_sys);

/*****************************************************************************/
//  Description : MMIAPIPB_InitSimStatus
//  Global resource dependence : none
//  Author: Tracy.zhang
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIPB_InitSimStatus(MN_DUAL_SYS_E dual_sys);


/*****************************************************************************/
//  Description : PB����ں���������pb�����ڣ����ݲ�������ʾPB���б�
//  Global resource dependence : none
//  Author: baokun.yin
//  Note: 
//RETURN: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIPB_OpenMainWin(
                        MMIPB_SUB_FUNCTION_TYPE_E app_type,
                        MMIPB_LIST_TYPE_E list_type, 
                        MMIPB_LIST_FIELD_TYPE_E field_display_type,
                        uint16 		max_select_count,
                        const MMI_STRING_T  *str_input,
                        MMIPB_RETURN_CALLBACK_FUNC callback//can be PNULL
                        );
#if defined(FDN_SUPPORT)
/*****************************************************************************/
//  Description : FDN�б���
//  Global resource dependence : none
//  Author: baokun.yin
//  Note: 
//RETURN: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIPB_OpenFDNContactListWin(MN_DUAL_SYS_E dual_sys);
#endif 
/*****************************************************************************/
//  Description : PB�ⲿ���Һ�����ͨ���ַ������ҷ�����������ϵ���б�
//  Global resource dependence : none
//  Author: baokun.yin
//  Note: 
//RETURN: 
/*****************************************************************************/
PUBLIC BOOLEAN	MMIAPIPB_SearchAllWin(
                            MMIPB_LIST_TYPE_E list_type, 
                            MMIPB_LIST_FIELD_TYPE_E field_display_type,
                            MMI_STRING_T *search_str,
                            MMIPB_RETURN_CALLBACK_FUNC callback
                            );

/*****************************************************************************/
//  Description : ����handle
//  Global resource dependence : none
//  Author:baokun.yin 
//  Note: 
//RETURN: 
/*****************************************************************************/
PUBLIC MMIPB_HANDLE_T MMIAPIPB_CreateHandle(void);

/*****************************************************************************/
//  Description : ����handle
//  Global resource dependence : none
//  Author: baokun.yin
//  Note: 
//RETURN: 
/*****************************************************************************/
PUBLIC void  MMIAPIPB_CloseHandle(
                                 MMIPB_HANDLE_T handle
                                 );

/*****************************************************************************/
//  Description : ��ȡ�������ĵ�index+1����ϵ�ˡ�
//  Global resource dependence : none
//  Author: MARYXIAO
//  Note: OUT: contact_ptr
//        OUT: num_index
//        IN: handle
//        IN: index
/*****************************************************************************/
PUBLIC BOOLEAN  MMIAPIPB_GetSelectContact(MMIPB_HANDLE_T handle,MMIPB_CONTACT_T * contact_ptr, uint16 index, uint16 * num_index);
/*****************************************************************************/
//  Description : ��ȡ�������ĵ�index+1����ϵ��������
//  Global resource dependence : none
//  Author: MARYXIAO
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN  MMIAPIPB_GetSelectName(MMIPB_HANDLE_T handle,MMI_STRING_T *name_str, uint16 max_name_len,uint16 index);
/*****************************************************************************/
//  Description : ��ȡ�������ĵ�index+1����ϵ��email��
//  Global resource dependence : none
//  Author: MARYXIAO
//  Note: 
/*****************************************************************************/

PUBLIC BOOLEAN  MMIAPIPB_GetSelectEmail(MMIPB_HANDLE_T handle,MMI_STRING_T *mail_ptr, uint16 max_mail_len,uint16 index);
/*****************************************************************************/
//  Description : ��ȡ�������ĵ�index+1����ϵ�˺��롣
//  Global resource dependence : none
//  Author: MARYXIAO
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN  MMIAPIPB_GetSelectNumber(MMIPB_HANDLE_T handle, MMIPB_BCD_NUMBER_T *nameber_ptr, uint16 max_number_len,  uint16 index);

/*****************************************************************************/
//  Description : ��ȡ�������ĵ�index+1����ϵ��vcard��
//  Global resource dependence : none
//  Author: MARYXIAO
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN  MMIAPIPB_GetSelectVcard(MMIPB_HANDLE_T handle, uint8  *data_ptr, uint16 max_buf_len,  uint16 index);

/*****************************************************************************/
//  Description : �첽������ϵ��
//  Global resource dependence : 
//  Author:MARY.XIAO
//  Note:���ⲿ�ӿڵ���
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIAPIPB_AddContactAsyn(MMIPB_HANDLE_T handle, MMIPB_CONTACT_T *contact_ptr, MMIPB_ACCESS_CALLBACK_FUNC callback);

/*****************************************************************************/
//  Description : �첽 �༭��ϵ��
//  Global resource dependence : 
//  Author:MARY.XIAO
//  Note:���ⲿ�ӿڵ���
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIAPIPB_ModifyContactAsyn(MMIPB_HANDLE_T handle, MMIPB_CONTACT_T *contact_ptr, MMIPB_ACCESS_CALLBACK_FUNC callback);
#ifdef MMI_VCARD_SUPPORT
/*****************************************************************************/
// Description : ������ϵ��
// Global resource dependence :
// Author:mary.xiao
// Note: 
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIAPIPB_ExportVcard(uint16 entry_id, uint16 storage_id, const wchar *path_name_ptr);

/*****************************************************************************/
// Description : ������ϵ��
// Global resource dependence :
// Author:mary.xiao
// Note: 
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIAPIPB_ImportVcard(
                                    //uint16      to_storage_id,      
                                    const wchar *path_name_ptr,//IN
                                    MMIPB_CONTACT_T* contact_ptr//OUT
                                    );


/*****************************************************************************/
// Description : �첽������ϵ��
// Global resource dependence :
// Author:mary.xiao
// Note: 
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIAPIPB_ImportVcardAsyn(MMIPB_HANDLE_T handle, //IN
                                              const wchar *path_name_ptr,//IN
                                              MMIPB_ACCESS_CALLBACK_FUNC callback
                                              );
#endif

/*****************************************************************************/
//  Description : �첽ɾ����ϵ��
//  Global resource dependence : 
//  Author:MARY.XIAO
//  Note:���ⲿ�ӿڵ���
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIAPIPB_DeleteContactAsyn(
                                                MMIPB_HANDLE_T handle, 
                                                uint16 entry_id,
                                                uint16 storage_id, 
                                                MMIPB_ACCESS_CALLBACK_FUNC callback
                                                );
/*****************************************************************************/
//  Description : ������ϵ��
//  Global resource dependence : 
//  Author:MARY.XIAO
//  Note:
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIAPIPB_CopyContact(uint16 to_storage_id, uint16 from_entry_id, uint16 from_storage_id);
/*****************************************************************************/
//  Description : �첽������ϵ��
//  Global resource dependence : 
//  Author:MARY.XIAO
//  Note:�ṩ���ⲿ�ӿڵ���, ��ʱ���θýӿڣ�����
/*****************************************************************************/
// PUBLIC MMIPB_ERROR_E MMIAPIPB_CopyContactAsyn(
//                                               MMIPB_HANDLE_T handle, 
//                                               uint16 dest_storage_id, 
//                                               uint16 src_contact_id, 
//                                               uint16 src_storage_id, 
//                                               MMIPB_ACCESS_CALLBACK_FUNC callback
//                                               );

#ifdef MMIPB_GROUP_MANAGE_SUPPORT
/*****************************************************************************/
//  Description :дȺ������
//  Global resource dependence : 
//  Author:MARY.XIAO
//  Note:
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIAPIPB_WriteGroupName(MMIPB_NAME_T* name,  uint8 group_id);
#endif

/*****************************************************************************/
//  Description : ADD  CONTACT
//  Global resource dependence : 
//  Author:MARY.XIAO
//  Note:
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIAPIPB_AddContact(MMIPB_CONTACT_T *contact_ptr);
/*****************************************************************************/
//  Description : delete  CONTACT
//  Global resource dependence : none
//  Author: 
//  Note: ����Ϊ�ӿڣ��ṩ������ģ�����
/*****************************************************************************/

PUBLIC MMIPB_ERROR_E MMIAPIPB_DeleteContact(uint16 entry_id, uint16 storage_id);
/*****************************************************************************/
//  Description : MODIFY  CONTACT
//  Global resource dependence : 
//  Author:MARY.XIAO
//  Note:
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIAPIPB_ModifyContact(MMIPB_CONTACT_T *contact_ptr);

/*****************************************************************************/
//  Description : ��ȡ���豸��storage id
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC uint16 MMIAPIPB_GetStorageId(
                                    MN_DUAL_SYS_E dual_sys,
                                    PHONEBOOK_STORAGE_E storage
                                    );

#ifdef MMI_VCARD_SUPPORT 
/*****************************************************************************/
//  Description : ����ϵ����Ϣת��Ϊvcard���ݸ�ʽ
//  Global resource dependence : 
//  Author:
//  Note:
/*****************************************************************************/
MMIPB_ERROR_E	MMIAPIPB_ContactToVcard(
                                     MMIPB_CONTACT_T *contact_ptr,  //[IN] 
                                     uint32          data_max_size,//[IN]
                                     uint8 *data_ptr,//[OUT]
                                     uint32 *data_len_ptr//[OUT]
                                    );

/*****************************************************************************/
//  Description : ��vcard����ת��Ϊ��ϵ����Ϣ��ͨ�ø�ʽ
//  Global resource dependence : 
//  Author:
//  Note:
/*****************************************************************************/
MMIPB_ERROR_E	MMIAPIPB_VcardToContact(
                                    uint8 *data_ptr,//[IN]
                                    uint32  data_len,//[IN]
                                    MMIPB_CONTACT_T 	      *contact_ptr//[OUT]
                                    );
/*****************************************************************************/
//  Description : ��vcard����ת��Ϊ��ϵ����Ϣ��ͨ�ø�ʽ
//  Global resource dependence : 
//  Author:
//  Note:ר��java pimʹ�ã���Ҫ���������ú����"�����������Ƿ��ַ���"flag��־
/*****************************************************************************/
MMIPB_ERROR_E	MMIAPIPB_VcardToContactForPim(
                                    uint8 *data_ptr,//[IN]
                                    uint32  data_len,//[IN]
                                    MMIPB_CONTACT_T 	      *contact_ptr//[OUT]
                                    );
#endif

/*****************************************************************************/
//  Description : ��ô洢λ������Ӧ��������Ⱥ��ID��ͼƬID
//  Global resource dependence : none
//  Parameter:  bcd_number : ����ĵ绰����[IN]
//              name_ptr:  ����[OUT]
//              max_name_len:��������󳤶�(���ֽڼ�)  [IN]
//              is_use_default_name:�Ƿ���Ҫ����ȱʡ����[IN]
//   RETURN: TRUE, �����ɹ���FALSE, ����ʧ��
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIPB_GetNameByNumber(MMIPB_BCD_NUMBER_T    *bcd_number,
								 MMI_STRING_T *name_ptr,
								 uint16 max_name_len,
								 BOOLEAN is_use_default_name);

/*****************************************************************************/
//  Description : ��ȡ���������Ӧ�ĺ�������
//  Global resource dependence : none
//  Parameter:  bcd_number : ����ĵ绰����[IN]
//              type_ptr:  ��������[OUT]
//              max_name_len:���������ַ�������󳤶�(��wchar��)  [IN]
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIPB_GetNumberTypeByNumber(MMIPB_BCD_NUMBER_T    *bcd_number,
								 wchar *type_ptr,
								 uint16 max_name_len
);

/*****************************************************************************/
//  Description : ����Ӳ˵��Ĵ��ڡ�
//  Global resource dependence : g_mmipb_operate_type, g_mmipb_bcd_number_t
//  Author: Tracy.zhang
//  Note: �˺���������Ϣ�Ͳ�����ȡ���벢�����ʱ����ã�
//        ��ͨ����¼���뱣�浽�绰���е��ã�����ģ����á�
//        ������ģ�����ʱ����Ҫ������룻����ģ�����ʱ�����ô����롣
/*****************************************************************************/
PUBLIC void MMIAPIPB_AddContactWin
(
    void	*data_ptr,	    //IN: ����ĵ绰����
    uint8                               group_id,                  //In:group id,
    MMIPB_ADD_TYPE_T  type
    
);
#ifdef MMIPB_ADD_EXIST_CONTACT_SUPPORT
/*****************************************************************************/
//  Description : Ϊͨ����¼�򿪼�¼�б��ڡ�
//  Global resource dependence : none
//  Author: Tracy.zhang
//  Note: �˺�����ͨ����¼�����뱣�浽��������ʱ���á�
/*****************************************************************************/
PUBLIC void MMIAPIPB_AddToExistContact
(
    void	* data_ptr,
    MMIPB_ADD_TYPE_T  type,
   MMIPB_RETURN_CALLBACK_FUNC callback
);
#endif
/*****************************************************************************/
//  Description : ��õ绰���Ƿ���Ա�����ģ�����
//  Global resource dependence : 
//  Author: Tracy.zhang
//  Note: 
/*****************************************************************************/
BOOLEAN MMIAPIPB_IsPbReady(void); //RETURN: TRUE, ����ʹ��

/*****************************************************************************/
// 	Description : �򿪴��ڣ���ʾPBȫ����¼
//	Global resource dependence : 
//  Author: bruce.chi
//	Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIPB_OpenListAllGroupWin(void);

/*****************************************************************************/
//  Description : get custom photo path name
//  Global resource dependence : none
//  Author:
//  Note:	path_ptr length should be larger than MMIPB_MAX_CUSTOM_PHOTO_PATH_LEN
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIPB_GetCustomPhotoPathByNumber(
						MMIPB_BCD_NUMBER_T *bcd_num_ptr,
						wchar	*path_ptr,
						uint32	*path_len_ptr,
						uint32	*file_size_ptr
						);

/*****************************************************************************/
//  Description : get custom ring path name
//  Global resource dependence : none
//  Author:
//  Note:	path_ptr length should be larger than MMIPB_MAX_CUSTOM_PHOTO_PATH_LEN
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIPB_GetCustomRingPathByNumber(
						MMIPB_BCD_NUMBER_T *bcd_num_ptr,
						BOOLEAN  *is_fixed_id,
						wchar	*path_ptr,
						uint32	*path_len_ptr,
						uint32	*fixed_id
						);

/*****************************************************************************/
//  Description : set contact ring to defaul
//  Global resource dependence : none
//  Author:
//  Note:	
/*****************************************************************************/
PUBLIC void MMIAPIPB_ClearContactRing(
						MMIPB_BCD_NUMBER_T *bcd_num_ptr
						);
#ifdef ASP_SUPPORT
/*****************************************************************************/
//  Description : get asp path name or fixed id
//  Global resource dependence : none
//  Author:
//  Note:	path_ptr length should be larger than MMIPB_MAX_CUSTOM_PHOTO_PATH_LEN
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIPB_GetCustomASPPathByNumber(
						MMIPB_BCD_NUMBER_T *bcd_num_ptr,
						BOOLEAN  *is_fixed_id,
						wchar	*path_ptr,
						uint32	*path_len_ptr,
						uint32	*fixed_id
						);
#endif
/*****************************************************************************/
//  Description : get custom photo path name
//  Global resource dependence : none
//  Author:
//  Note:	path_ptr length should be larger than MMIPB_MAX_CUSTOM_PHOTO_PATH_LEN
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIPB_ReadPhotoFileName(uint16 entry_id, 
                                       uint16 storage_id, 
                                       MMIPB_CUSTOM_PHOTO_T *photo_ptr);


/*****************************************************************************/
//  Description : Refresh SIM���е�Phone book���޸�ȫ�ֱ�������Ϣ
//  Global resource dependence : g_mmipb_sim_t
//  Author: Kelly.li
//  Note: 
/*****************************************************************************/
// PUBLIC void MMIAPIPB_RefreshSimList (void);

/*****************************************************************************/
//  Description : set all Sim  to not initialize status
//  Global resource dependence : none
//  PARAMETER: 
//  Return��
//  Note: 
/*****************************************************************************/
//PUBLIC void MMIAPIPB_SetAllSIMNotInit(void);
#ifdef MMIPB_SIMDN_SUPPORT
/*****************************************************************************/
//  Description : �򿪱����������ô���
//  Global resource dependence : 
//  Author: bruce.chi
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIPB_OpenMSISDNWin(BOOLEAN is_in_wizard, BOOLEAN is_enter_from_callsetting);
#endif
#ifdef MMI_OCR_ENABLE
/*****************************************************************************/
//  Description : �����ϵ����Ϣ���绰��
//  Global resource dependence : g_mmipb_nv_entry_t
//  Author: Annie.an
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIPB_AddContactToPhonebook(MMIOCR_INFO_ENTRY_T *ocr_info);
#endif

/*****************************************************************************/
//  Description :	���PB
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIPB_AbortPB(void);

/*****************************************************************************/
//     Description : get sum size of all pb nv itme
//    Global resource dependence : 
//  Author: gang.tong
//    Note:
/*****************************************************************************/
PUBLIC uint32 MMIAPIPB_GetSumNvSize(void);
#ifdef MMI_VCARD_SUPPORT
/*****************************************************************************/
// Description : ����VCard data,���򿪸��ļ�
// Global resource dependence :
// Author:
// Note: 
/*****************************************************************************/
PUBLIC void  MMIAPIPB_OpenVcardContactWin(uint8  *data_ptr, uint32 data_size);

/*****************************************************************************/
// Description : ����VCard data,���򿪸��ļ�
// Global resource dependence :
// Author:
// Note: 
/*****************************************************************************/
PUBLIC void  MMIAPIPB_OpenVcardFileContactWin(uint16  *file_name_ptr);
#endif
/*****************************************************************************/
//  Description : �жϵ绰�����Ƿ���Ч
//  Global resource dependence : none
//  Author: Tracy.zhang
//  Note: 
/*****************************************************************************/

PUBLIC BOOLEAN MMIAPIPB_IsValidPhoneNumber
(
 uint8   npi_ton,
 uint8   number_length
 );

/*****************************************************************************/
//  Description : ��bcd��ʽ�ĵ绰����ת��Ϊ�ַ���
//  Global resource dependence : 
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC uint8 MMIAPIPB_BCDNumberToString(
                                    MMIPB_BCD_NUMBER_T  *bcd_number_ptr,//in   
                                    uint8               str_max_len,//IN
                                    uint8               *str_ptr//OUT                                    
                                    );

/*****************************************************************************/
//  Description : ���ַ���ת��Ϊbcd��ʽ�ĵ绰����
//  Global resource dependence : 
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC void	MMIAPIPB_StringToBCDNumber(
                                uint8               *str_ptr,//IN 
                                MMIPB_BCD_NUMBER_T  *bcd_number_ptr//OUT                                                                          
                                );

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : 
//  Author:
//  return 
//	Note: 
/*****************************************************************************/
PUBLIC BOOLEAN  MMIAPIPB_IsUseDefaultName(wchar* name_ptr);

//#ifndef PB_SEARCH_SURPPORT_NONE
/*****************************************************************************/
//  Description : ����������ַ������Һ����а����ô��ļ�¼�������а�������ĸƥ��ļ�¼
//  Global resource dependence : none
//  Author: 
//  Note: 
//RETURN: ���������������ʹ����ɺ�Ҫ����MMIAPIPB_SearchExit�ͷ�
/*****************************************************************************/
PUBLIC MMIPB_HANDLE_T MMIAPIPB_SearchList(MMI_STRING_T *search_str,
                                  MMIPB_LIST_FIELD_TYPE_E disp_field, 
                                  uint16 *count_ptr);

/*****************************************************************************/
//  Description : ����������ַ������Һ����а����ô��ļ�¼��ƥ������
//  Global resource dependence : none
//  Author: 
//  Note: 
//RETURN: ���������������ʹ����ɺ�Ҫ����MMIAPIPB_SearchExit�ͷ�
/*****************************************************************************/
PUBLIC MMIPB_HANDLE_T MMIAPIPB_SearchListAll(MMI_STRING_T *search_str, 
                               MMIPB_LIST_FIELD_TYPE_E field_type, 
                               uint16 *count_ptr
                               );

/*****************************************************************************/
//  Description : �ⲿ�����˳������ⲿ�����˳��ǣ�����ӿ�һ��Ҫ����
//  Global resource dependence : none
//  Author: 
//  Note: 
//RETURN: 
/*****************************************************************************/
PUBLIC void MMIAPIPB_SearchExit(MMIPB_HANDLE_T handle);

/*****************************************************************************/
//  Description : �������ص��ĵ�index����0��ʼ������¼���������ͺ���
//  Global resource dependence : none
//  Author: 
//  Note: 
//RETURN: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIPB_GetSearchEntry(MMIPB_HANDLE_T handle,
                                       MMI_STRING_T *name_str,//[OUT] 
                                       uint16 max_name_len, 
                                       MMIPB_BCD_NUMBER_T *bcd_num,//[OUT]
                                       uint32* group,
                                       uint32 index);

/*****************************************************************************/
//  Description : 
//  Global resource dependence :
//  Author: 
//  Note: 
//RETURN: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIPB_GetSearchEntryWithAvatar
(
 MMIPB_HANDLE_T handle,
 MMI_STRING_T *name_str,//[OUT] 
 uint16 max_name_len, 
 MMIPB_BCD_NUMBER_T *bcd_num,//[OUT]
 MMI_STRING_T *num_type_str,//[OUT] 
 uint32*   group,//[OUT] 
 MMI_STRING_T *abbr_ptr,//[OUT] 
 GUIANIM_FILE_INFO_T* anim_path,//[OUT] 
 uint16* entry_id, //[OUT] 
 uint16* storage_id, //[OUT]
 uint8* total, //[OUT]  ��4λ�Ǻ����ܸ�������4λ�ǵ�ǰ�����ǵڼ���
 uint32 index
 );

/*****************************************************************************/
//  Description : �������ص��ĵ�index����0��ʼ������¼��������Email
//  Global resource dependence : none
//  Author: 
//  Note: 
//RETURN: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIPB_GetSearchEntryWithEmail
(
 MMIPB_HANDLE_T handle,
 MMI_STRING_T *name_str,//[OUT] 
 uint16 max_name_len,
 MMI_STRING_T *email_str,//[OUT] 
 uint16 max_email_len,
 uint32 index
 );

/*****************************************************************************/
//  Description : �������ص��ĵ�index����0��ʼ������¼��contact_id and storage id
//  Global resource dependence : none
//  Author: 
//  Note: 
//RETURN: Ϊ�ⲿ���н�pbѡ���
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIPB_GetSearchEntryID(MMIPB_HANDLE_T handle,                                       
                                       uint32 index,
                                       uint16 *contact_id_ptr,
                                       uint16 *storage_id_ptr);									  

/*****************************************************************************/
//  Description : �������ص��ĵ�index����0��ʼ������¼��Сͼ��
//  Global resource dependence : none
//  Author: 
//  Note: 
//RETURN: 
/*****************************************************************************/
PUBLIC uint32 MMIAPIPB_GetSearchEntryIcon(uint32 index);

#ifdef MMIPB_MOST_USED_SUPPORT
//#endif
/*****************************************************************************/
//  Description : ���õ绰����ı�ʹ��

//  Note: ���¸ú����ʹ��Ƶ�ʡ�
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIPB_SetNumberUsed(uint8 *num_str, uint16 number_len);
#endif
/*****************************************************************************/
//  Description : update FDN list
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIPB_UpdateSIMDNList(void);

/*****************************************************************************/
//Description : compare bcd number
// Return: 0: number1 = number2; 1: number1 > number2; -1:number1 < number2
/*****************************************************************************/
PUBLIC int32 MMIAPIPB_BCDCmp(
							 const MMIPB_BCD_NUMBER_T * number1,
                             const MMIPB_BCD_NUMBER_T * number2
 );

/*****************************************************************************/
//  Description : �绰�����ְ��ƥ��
//  Global resource dependence : 
//  Author: 
//  Note:
//  Return: 0:���������ܹ�ְ��ƥ�䣬��������ƥ��
/*****************************************************************************/
PUBLIC int32 MMIAPIPB_BcdSmartCmp(const MMIPB_BCD_NUMBER_T *bcd_number1, //IN:
								  const MMIPB_BCD_NUMBER_T *bcd_number2//IN
								  );

/*****************************************************************************/
//  Description : ����UCS2�ַ������ַ����н��бȽ�
//  Global resource dependence : none
//  Author: 
//  Note: ���� ���١���"��"���ǰ���"bai"��"bu"���бȽϵ�
/*****************************************************************************/
PUBLIC int MMIAPIPB_CmpStrByLetters(
										   wchar   *ucs2_string_ptr1,        //IN:
										   uint8   wstr1_len,            //IN:
										   wchar   *ucs2_string_ptr2,        //IN:
										   uint8   wstr2_len            //IN:

										   );
#ifdef MMIWIDGET_SUPPORT

/*****************************************************************************/
//  Description : Get First Valid Number index from number array number_t_ptr
//  Global resource dependence : none
//  Author: xport
//  Note: 
/*****************************************************************************/
PUBLIC int32 MMIAPIPB_GetFirstValidNumberFromNumArray(MMIPB_BCD_NUMBER_T *number_t_ptr, uint32 max_count);
#endif

/*****************************************************************************/
// Description : open one contact details by number.
// Global resource dependence :
// Author:baokun.yin
// Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIPB_OpenNumberContactWin(MMIPB_BCD_NUMBER_T *number_ptr);

/*****************************************************************************/
// Description : open one favorite entry called only by widget.
// Global resource dependence :
// Author:
// Note: 
/*****************************************************************************/
PUBLIC void MMIAPIPB_OpenContactWin(uint16 entry_id, uint16 storage_id);													

/*****************************************************************************/
//  Description : delete all pb info 
//  Author: 
//  Note: for dm cmcc 
/*****************************************************************************/
PUBLIC void MMIAPIPB_DeleteAllPBInfo(void);


/*****************************************************************************/
//  Description : ��ÿ��е�entry_id
//  Global resource dependence : none
//  Author: Tracy.zhang
//  Note: 
/*****************************************************************************/

PUBLIC uint16 MMIAPIPB_GetFreeContactId		//RETURN: ���е�entry_id
                                       (
                                        uint16   storage_id
                                        );


#if defined(FDN_SUPPORT)
/*****************************************************************************/
//  Description : PIN code verify ok
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIPB_Pin2VerifyCnf(void);
#endif


/*****************************************************************************/
//  Discription: This function is used to send signal from mmi to mmi
//   
//  Global resource dependence: None
//  Author: baokun yin
//  Note : 
//  Return:
/*****************************************************************************/
PUBLIC  void MMIAPIPB_SendSignal(uint32 sig_id);

/*****************************************************************************/
//  Description : ����ֻ��м�¼���ܸ���
//  Global resource dependence : g_mmipb_nv_t
//  Author: Tracy.zhang
//  Note: 
/*****************************************************************************/
PUBLIC uint16 MMIAPIPB_GetPhoneContactCount(void);

/*****************************************************************************/
//  Description : ���SIM���к�����ܸ���
//  Global resource dependence : g_mmipb_entry_list
//  Author: Tracy.zhang
//  Note: 
/*****************************************************************************/

PUBLIC uint16 MMIAPIPB_GetAllSimContactCount
(
    void
);


/*****************************************************************************/
//  Description : ������ת��Ϊ�ַ���
//  Global resource dependence : none
//  Author: Tracy.zhang
//  Note: 
/*****************************************************************************/

PUBLIC void MMIPB_ConvertNameToString
(
    MMI_STRING_T    	*str_ptr,   //OUT:
    const MMIPB_NAME_T	*name_ptr   //IN:
);


/*****************************************************************************/
//  Description : ��ô洢����Ӧ�ĵ绰�����ܱ��������¼��
//  Global resource dependence : g_mmipb_entry_list, g_mmipb_nv_t
//  Author: Tracy.zhang
//  Note: 
/*****************************************************************************/
PUBLIC uint16 MMIAPIPB_GetStorageMaxCount(uint16 storage_id);   //RETURN: ����¼��

/*****************************************************************************/
//  Description : search sub string postion index in string
//  Global resource dependence : 
//  Author: 
//  Note:str_pos�Ŀռ�����ǵ����߷����
//  Return:��str_pos���ظ���PB���趨�Ĳ����㷨��string2��string1��ƥ����ַ���λ������
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIPB_Wstrstrpos(const MMI_STRING_T * string1,//IN
								   const MMI_STRING_T * string2,//IN
								   MMI_STRING_T       * str_pos,//OUT
								   BOOLEAN            is_char//TRUE:�ַ����ң�FALSE�����ֲ���
								   );
/*****************************************************************************/
//  Description : ���applet������ѡ���б�Ľڵ���,������group
//  Global resource dependence : 
//  Author:MARY.XIAO
//  Note:
/*****************************************************************************/
PUBLIC uint16  MMIAPIPB_GetSelectCount(MMI_HANDLE_T handle);

#ifdef MMI_PDA_SUPPORT
/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIPB_SwitchToPreTab(void);
#endif
/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIPB_SetCLUnProtect(BOOLEAN is_unprotect);

/*****************************************************************************/
//  Description : �첽������Ĵ���
//  Global resource dependence : 
//  Author:baokun.yin
//  Note:
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIAPIPB_CmdConfirm(                                                           
                              MMIPB_ERROR_E  result
                              );

/*****************************************************************************/
//  Description : �����ϵ����Ϣ
//  Global resource dependence : 
//  Author:MARY.XIAO
//  Note:
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIAPIPB_GetContactInfo(uint16 entry_id, uint16 storage_id, MMIPB_CONTACT_T* contact_ptr);

/*****************************************************************************/
//  Description :   ��ñ�������
//  Global resource dependence : 
//  Author: bruce.chi
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIPB_ReadMsisdnContact(MN_DUAL_SYS_E dual_sys, 
            MMIPB_CONTACT_T *pb_msisdn_ptr);

/*****************************************************************************/
//  Description :��ȡȺ������
//  Global resource dependence : 
//  Author:MARY.XIAO
//  Note:
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIAPIPB_ReadGroupName(MMIPB_NAME_T* name,  uint8 group_id);

/*****************************************************************************/
//  Description : ��ͨ��ϵ�˵Ļ�����Ϣ
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC MMIPB_CONTACT_BASE_INFO_T MMIAPIPB_GetContactBaseInfo(uint16 index);

/*****************************************************************************/
//  Description : ��ú���Ϊ��ͨ��ʽʱ����󳤶�
//  Global resource dependence : s_mmipb_max_len_t
//  Author: Tracy.zhang
//  Note: 
/*****************************************************************************/

uint8 MMIAPIPB_GetNumberNormalMaxLen
(
 void
 );
#ifdef  FDN_SUPPORT
/*****************************************************************************/
// 	Description : ȡ��sim���ض������б�Ĵ�������
//	Global resource dependence : 
//    Author:
//	Note: 
/*****************************************************************************/
PUBLIC uint32 MMIAPIPB_GetDailNumberListWinTab(void);
#endif
/*****************************************************************************/
//  Description :��ȡMSISD�Ĕ�Ŀ
//  Global resource dependence : none
//  Author: mary.xiao
//  Note: 
/*****************************************************************************/
PUBLIC uint16 MMIAPIPB_GetMSISDNum(void);

/*****************************************************************************/
//  Description :��ȡָ��SIM��MSISDNumber
//  Global resource dependence : none
//  Author: mary.xiao
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIPB_GetMSISDNumberArrayBySim(MMIPB_BCD_NUMBER_T *number,MN_DUAL_SYS_E dual_sys, int16 max_num);

#if defined(BLUETOOTH_SUPPORT) && defined(BT_BQB_SUPPORT)
//mdy by qing.yu@for BQB
/*****************************************************************************/
//  Description : get pb my phone number
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIPB_GetMSISDNumber(MMIPB_BCD_NUMBER_T *number);

//+++ TC_AG_OCM_BV_01_I
PUBLIC BOOLEAN MMIAPIPB_GetNumberByIndex(uint16 index, uint8* pstrbuf);
//--- TC_AG_OCM_BV_01_I
#endif

#ifdef SEARCH_SUPPORT
#ifdef MMIPB_SEARCH_CONTACT_CONTENT
/*****************************************************************************/
// 	Description : 
//	Global resource dependence : 
//    Author:
//	Note: 
/*****************************************************************************/
PUBLIC void MMIAPIPB_UpdatePbSearching(void);

/*****************************************************************************/
// 	Description : ����������pb�����ӿ�
//	Global resource dependence : 
//    Author:maryxiao
//	Note: 
/*****************************************************************************/
PUBLIC void MMIAPIPB_LocalAsyncSearch(const MMI_STRING_T *key_word_str);

/*****************************************************************************/
//     Description : stop async search pb
//    Global resource dependence : 
//  Author: tao.xue
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIPB_StopPbAsyncSearch(void);
/*****************************************************************************/
//     Description : is pb search end
//    Global resource dependence : 
//  Author: tao.xue
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIPB_IsPbAsyncSearchEnd(void);
#else
/*****************************************************************************/
// 	Description : ����������pb�����ӿ�
//	Global resource dependence : 
//    Author:maryxiao
//	Note: 
/*****************************************************************************/
PUBLIC void MMIAPIPB_LocalSearch(const MMI_STRING_T *key_word_str, MMISEARCH_DATA_T *match_data_ptr, uint32 max_num);
#endif

/*****************************************************************************/
// 	Description : ����������pb�����ӿ�
//	Global resource dependence : 
//    Author:maryxiao
//	Note: 
/*****************************************************************************/
PUBLIC void MMIAPIPB_FreeLocalSearchResource(void);
#endif
/*****************************************************************************/
// Description : ���������Դ洢��contacts����Ŀ
// Global resource dependence :
// Author:maryxiao
// Note: 
/*****************************************************************************/
PUBLIC uint16 MMIAPIPB_GetMaxContactNum(void);

/*****************************************************************************/
//  Description : ��ô洢λ������Ӧ��������Ⱥ��ID��ͼƬID
//  Global resource dependence : none
//  Parameter:  bcd_number : ����ĵ绰����[IN]
//              name_ptr:  ����[OUT]
//              max_name_len:��������󳤶�(���ֽڼ�)  [IN]
//              is_use_default_name:�Ƿ���Ҫ����ȱʡ����[IN]
//   RETURN: TRUE, �����ɹ���FALSE, ����ʧ��
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIPB_GetNameByNumber(MMIPB_BCD_NUMBER_T    *bcd_number,
								 MMI_STRING_T *name_ptr,
								 uint16 max_name_len,
								 BOOLEAN is_use_default_name);


/*****************************************************************************/
//  Description : THE INTERFACE for autotest only
//  Global resource dependence : none
//  Author:maryxiao
//  Note: 
/***************************************************************************/
PUBLIC BOOLEAN MMIAPIPB_FillPhoneContactFull(void);
#ifdef MMIPB_UID_SUPPORT
/*****************************************************************************/
//  Description : ���ݼ�¼uid ������Ӧ��¼��entry_id��storage_id
//  Global resource dependence : 
//  Author:mary.xiao
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIPB_GetcontactIdByUid(uint32 uid, uint16* entry_id_ptr, uint16* storage_id_ptr);

/*****************************************************************************/
//  Description : ���ݼ�¼uid ������Ӧ��¼��entry_id��storage_id
//  Global resource dependence : 
//  Author:mary.xiao
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIPB_GetUidByContactId(uint16 entry_id, uint16 storage_id, uint32* uid);
#endif

#ifdef TCARD_PB_SUPPORT
/*****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC uint16 MMIAPIPB_IsTCardContactExist(void);
#endif
/*****************************************************************************/
//  Description : get storage free count
//  Global resource dependence : 
//  Note: 
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIAPIPB_GetStorageFreeSpace(uint16 storage_id, uint16 *free_num_ptr);

#ifdef MMI_PIC_CROP_SUPPORT
PUBLIC void PicCropCallBack(MMIPICCROP_CB_T *crop_cb);
#endif

#ifdef MMI_VCARD_SUPPORT
/*****************************************************************************/
//  Description : judge whether export contact isnot
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIPB_IsExportContactOpc();
#endif

/*****************************************************************************/
//  Description : 
//  Global resource dependence : none
//  Parameter:  bcd_number : ����ĵ绰����[IN]
//              name_ptr:  ����[OUT]
//              max_name_len:��������󳤶�(���ֽڼ�)  [IN]
//              is_use_default_name:�Ƿ���Ҫ����ȱʡ����[IN]
//   RETURN: TRUE, �����ɹ���FALSE, ����ʧ��
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIPB_GetNameSurnameAvatarByNumber(MMIPB_BCD_NUMBER_T    *bcd_number,
											  MMI_STRING_T *name_ptr,
											  MMI_STRING_T *surname_ptr,
											  MMI_STRING_T *abbr_ptr,
											  GUIANIM_FILE_INFO_T* anim_path,
											  uint16 *entry_id,
											  uint16 *storage_id,
											  uint16 max_name_len,
											  BOOLEAN is_use_default_name);

/*****************************************************************************/
//  Description : 
//  Global resource dependence : none
//  Parameter:  bcd_number : ����ĵ绰����[IN]
//              name_ptr:  ����[OUT]
//              max_name_len:��������󳤶�(���ֽڼ�)  [IN]
//              is_use_default_name:�Ƿ���Ҫ����ȱʡ����[IN]
//   RETURN: TRUE, �����ɹ���FALSE, ����ʧ��
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIPB_GetNameAvatarByNumber
(
 MMIPB_BCD_NUMBER_T    *bcd_number,
 MMI_STRING_T *name_ptr,
 MMI_STRING_T *abbr_ptr,
 GUIANIM_FILE_INFO_T* anim_path,
 uint16 *entry_id,
 uint16 *storage_id,
 uint16 max_name_len,
 BOOLEAN is_use_default_name
 );

/*****************************************************************************/
//  Description : 
//  Global resource dependence : none
//  Parameter:  bcd_number : ����ĵ绰����[IN]
//              name_ptr:  ����[OUT]
//              max_name_len:��������󳤶�(���ֽڼ�)  [IN]
//              is_use_default_name:�Ƿ���Ҫ����ȱʡ����[IN]
//   RETURN: TRUE, �����ɹ���FALSE, ����ʧ��
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIPB_GetNameSurnameByNumber
(
 MMIPB_BCD_NUMBER_T    *bcd_number,
 MMI_STRING_T *name_ptr,
 MMI_STRING_T *surname_ptr,
 uint16 max_name_len,
 BOOLEAN is_use_default_name
 );

/*****************************************************************************/
//  Description : 
//  Global resource dependence : none
//  Parameter:  bcd_number : ����ĵ绰����[IN]
//              name_ptr:  ����[OUT]
//              max_name_len:��������󳤶�(���ֽڼ�)  [IN]
//              is_use_default_name:�Ƿ���Ҫ����ȱʡ����[IN]
//   RETURN: TRUE, �����ɹ���FALSE, ����ʧ��
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIPB_GetNameSurnamePreferredSimByNumber
(
 MMIPB_BCD_NUMBER_T    *bcd_number,
 MMI_STRING_T *name_ptr,
 MMI_STRING_T *surname_ptr,
 uint8* preferred_sim,
 uint16 max_name_len,
 BOOLEAN is_use_default_name
 );

/*****************************************************************************/
//  Description : ��ô洢λ������Ӧ��������Ⱥ��ID��ͼƬID
//  Global resource dependence : none
//  Parameter:  bcd_number : ����ĵ绰����[IN]
//              name_ptr:  ����[OUT]
//              max_name_len:��������󳤶�(���ֽڼ�)  [IN]
//              is_use_default_name:�Ƿ���Ҫ����ȱʡ����[IN]
//   RETURN: TRUE, �����ɹ���FALSE, ����ʧ��
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIPB_GetNamePreferredSimByNumber
(
 MMIPB_BCD_NUMBER_T    *bcd_number,
 MMI_STRING_T *name_ptr,
 uint8* preferred_sim,
 uint16 max_name_len,
 BOOLEAN is_use_default_name
 );

/*****************************************************************************/
//  Description : 
//  Global resource dependence : none
//   RETURN: TRUE, �����ɹ���FALSE, ����ʧ��
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIPB_GetNameAvatarByEmail
(
 wchar *email_str, //in
 MMI_STRING_T *name_ptr, //out
 MMI_STRING_T *abbr_ptr, //out
 GUIANIM_FILE_INFO_T* anim_path, //out
 uint16 *entry_id, //out
 uint16 *storage_id, //out
 uint16 max_name_len //in
 );

#ifdef MMI_PB_DYNAMIC_LOAD_SIM_CONTACT_SUPPORT
/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
BOOLEAN MMIAPIPB_IsLoadSimContactReady(void);

/*****************************************************************************/
//  Description : 
//  Global resource dependence :
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIPB_BeginToLoadSimContact(void);

/*****************************************************************************/
//  Description : 
//  Global resource dependence :
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC void MMIPB_ReleaseLoadSimContact(void);

#endif /*MMI_PB_DYNAMIC_LOAD_SIM_CONTACT_SUPPORT*/

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIPB_CreateDefaultGroups(void);

/*****************************************************************************/
//  Description : 
//  Global resource dependence :
//  Author: 
//  Note: 
//        
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIPB_IsCanSyncNow(void);

/*****************************************************************************/
//  Description : 
//  Global resource dependence :
//  Author: 
//  Note: 
//        
/*****************************************************************************/
PUBLIC void MMIAPIPB_SyncUpdateAllWin(uint16 entry_id, uint16 storage_id, MMIPB_OPC_E opt);

/*****************************************************************************/
//  Description :
//  Global resource dependence :
//  Author: maryxiao
//  Note: 
/*****************************************************************************/
PUBLIC  int8 MMIAPIPB_GetGroupIds
(
 uint8 *group_ids, //out    
 uint8 group_ids_num//in
 );

/*****************************************************************************/
//  Description :
//  Global resource dependence :
//  Author: maryxiao
//  Note: 
/*****************************************************************************/
PUBLIC  void MMIAPIPB_GetGroupNameById
(
 uint8 group_id ,//in
 uint16 group_name_max_len, //in
 MMI_STRING_T *group_name_str //out
 );

/*****************************************************************************/
// 	Description :
//	Global resource dependence :
//  Author:
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIPB_OpenMutiSelectWinByGroupId
(
 MMIPB_GROUP_MUTI_SELECT_FROM_TYPE_E from_type, 
 uint8 group_id,
 MMIPB_LIST_FIELD_TYPE_E field_display_type,
 MMIPB_SEARCH_TYPE_T             search_type, 
 uint16 		max_select_count,                     
 const MMI_STRING_T  *str_input,
 MMIPB_RETURN_CALLBACK_FUNC callback
 );

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIPB_IsSortBySurname(void);

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : 
//  Author:
//  return 
//	Note: 
/*****************************************************************************/
PUBLIC BOOLEAN  MMIAPIPB_WriteDisplayDebugMenu(BOOLEAN value);

/*****************************************************************************/
// 	Description :
//	Global resource dependence : 
//  Author:
//  return 
//	Note: 
/*****************************************************************************/
PUBLIC BOOLEAN  MMIAPIPB_IsDisplayDebugMenu();

/*****************************************************************************/
// 	Description :
//	Global resource dependence : 
//  Author:
//  return 
//	Note: 
/*****************************************************************************/
PUBLIC void MMIAPIPB_PrintDebugMsTime(const char* info_str);

#ifdef MMIPB_SYNC_WITH_O365
/*****************************************************************************/
//  Description : 
//  Global resource dependence :
//  Author: 
//  Note: 
//        
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIPB_DeleteAllOffice365Contacts(void);
#endif /*MMIPB_SYNC_WITH_O365*/

#ifdef MMI_PB_MULTILANG_SORT_SUPPORT
/*****************************************************************************/
//  Description : ����Ϣ��task���ú���������ϵ�˵���������
//  Global resource dependence : none
//  Author: bruce.chi
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIPB_BeginToReSort(void);

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIPB_MultiLang_SetSortLang(MMISET_LANGUAGE_TYPE_E sort_lang);

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC MMISET_LANGUAGE_TYPE_E MMIAPIPB_MultiLang_GetSortLang( void);

/*****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC int MMIPB_MultiLang_Wstrncmp(
                              const wchar* string1,
                              const wchar* string2,
                              size_t count
                              );

#endif /*MMI_PB_MULTILANG_SORT_SUPPORT*/

//zhuyongjing add for pb into gallery 20150826
#ifdef MMI_PIC_CROP_SUPPORT
PUBLIC void PicCropCallBack(MMIPICCROP_CB_T *crop_cb);
#endif
//add end

#ifdef MMI_BT_PBAP_SUPPORT
/*******************************************************************************/
//  Description :��ϵ��ͬ���������ͨѶ¼
//  Parameter: contact_ptr[in]: ��¼�����Ϣ
//  Global resource dependence :                                
//  Author: yanyan.an
//  Return : 
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIAPIBT_AddContact(MMIPB_CONTACT_T *contact_ptr);

/*******************************************************************************/
//  Description :get free space
//  Parameter: 
//  Global resource dependence :                                
//  Author: yao.chen
//  Return : 
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIAPIPB_GetPhoneStorageFreeSpace(void);

/*******************************************************************************/
//  Description :��ϵ��ͬ������ʼ��pbap�����б�
//  Parameter: 
//  Global resource dependence :                                
//  Author: yao.chen
//  Return : 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIPB_InitPBAPDataList(void);

/*******************************************************************************/
//  Description :notify pb start parser s_list_head
//  Parameter: 
//  Global resource dependence :                                
//  Author: yao.chen
//  Return : 
/*****************************************************************************/
PUBLIC void MMIAPIPB_NotifyPBStartParser(MMIPB_PBAP_PARSER_CALLBACK_FUNC callback);

/*******************************************************************************/
//  Description :��ϵ��ͬ������ʼ��pbap�����б�
//  Parameter: 
//  Global resource dependence :                                
//  Author: yao.chen
//  Return : 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIPB_InsertPBAPDataToList(void* buff,uint16 buff_len);

/*****************************************************************************/
//  Description :  ���ȫ��������ϵ��
//  Global resource dependence : 
//  Author:Yanyan.an
//  Note:���ⲿ�ӿڵ���
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIAPIPB_DeleteAllPbapSyncData(void);

/*****************************************************************************/
//  Description : �������timer
//  Global resource dependence : 
//  Author:allen.lee
//  Note:���ⲿ�ӿڵ���
/*****************************************************************************/
PUBLIC void MMIAPIPB_StopPBAPTimer(void);

/*****************************************************************************/
//  Description : Deregisters the Phonebook Access Client
//  Global resource dependence :                                
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIPB_PbapClientDeregister(void);

/*****************************************************************************/
//  Description : MMIAPIPB_ENTERMESSAGE_BACK_PLAY
//  Global resource dependence :                                
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIPB_ENTERMESSAGE_BACK_PLAY(void );

#endif


/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif
