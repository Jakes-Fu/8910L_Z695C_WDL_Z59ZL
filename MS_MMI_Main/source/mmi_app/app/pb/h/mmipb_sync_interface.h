/*****************************************************************************
** File Name:      mmipb_sync_datalist.h                                         *
** Author:         shiwei.zhang                                                          *
** Date:           07/09/2015                                                           *
** Copyright:      2015 Spreadtrum, Incorporated. All Rights Reserved.  *
** Description:    This file deal with sync data .                                  *             
**                 ����ļ�����ϵ��ͬ�����ݽ��й���.  
***************************************************************************/
#ifdef MMIPB_SYNC_WITH_O365
#ifndef _MMIPB_SYNC_INTERFACE_H_
#define _MMIPB_SYNC_INTERFACE_H_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmk_type.h"
#include "mmi_custom_define.h"
#include "mmipb_common.h"
#include "mmipb_adapt.h"
#include "mmipb_interface.h"
/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif

/****************************************************************************
*  Description :�����ѯOffice365���ص�һ����ϵ����Ϣ����ת����phone֧�ֵ���ϵ�˸�ʽ��
*  Global resource dependence : 
*  Note: ����˵����
*   (1)contact_ptr����ת����phone֧�ָ�ʽ��Office365��ϵ����Ϣָ��
*   (2)chunk_end_flag��ʾ��ǰ��ѯ�����ݿ���������Ը������ж��Ƿ���Ҫ������ѯ���µ���ϵ����Ϣ������ǣ�������ɱ�����Ϣ���������server�ṩ��MMIAPIPBSYNC_SVR_GetAllContacts()�ӿڽ��в�ѯ��
*    (chunk_end_flagһ����ڴ������ݿ��е�10��ʱ��ΪTRUE)
*   (3)all_end_flag ��ʾ������ϵ����Ϣ���Ѳ�ѯ��ϣ�PB�˿��Ը��������ʶ������һ���Ĳ��������¼����ɵ�һ��ͬ����
*   (all_end_flagһ�����ڴ������һ�����ݿ�����һ��ʱ��ΪTRUE)
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIAPIPBSYNC_PB_GetAllContactsHandler(uint32 request_id, MMIPB_CONTACT_T *contact_ptr, BOOLEAN chunk_end_flag ,BOOLEAN all_end_flag);

/****************************************************************************
*  Description :�����ѯ���ص�Office365 ĳ��ʱ���Ժ���ӵ���ϵ�ˣ���ת����phone֧�ֵ���ϵ�˸�ʽ��
*   (1)contact_ptr  ����ת����phone֧�ָ�ʽ��Office365��ϵ����Ϣָ��
*   (2)chunk_end_flag   ��ʾ��ǰ��ѯ�����ݿ���������Ը������ж��Ƿ���Ҫ������ѯ���µ���ϵ����Ϣ������ǣ�������ɱ�����Ϣ���������server�ṩ��MMIAPIPBSYNC_SVR_GetAllContacts()�ӿڽ��в�ѯ��
*       chunk_end_flag    һ����ڴ������ݿ��е�10��ʱ��ΪTRUE.
*   (3)all_end_flag ��ʾ������ϵ����Ϣ���Ѳ�ѯ��ϣ�PB�˿��Ը��������ʶ������һ���Ĳ��������¼����ɵ�һ��ͬ����
*       all_end_flag   һ�����ڴ������һ�����ݿ�����һ��ʱ��ΪTRUE.
*   2015-07-22
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIAPIPBSYNC_PB_GetAllAddedContactsHandler(uint32 request_id, MMIPB_CONTACT_T *contact_ptr, BOOLEAN chunk_end_flag ,BOOLEAN all_end_flag);

/****************************************************************************
*  Description :    �����ѯ���ص�Office365ĳ��ʱ���Ժ��޸ĵ���ϵ�ˣ���ת����phone֧�ֵ���ϵ�˸�ʽ��
*   (1)contact_ptr  ����ת����phone֧�ָ�ʽ��Office365��ϵ����Ϣָ��
*   (2)chunk_end_flag   ��ʾ��ǰ��ѯ�����ݿ���������Ը������ж��Ƿ���Ҫ������ѯ���µ���ϵ����Ϣ������ǣ�������ɱ�����Ϣ���������server�ṩ��MMIAPIPBSYNC_SVR_GetAllContacts()�ӿڽ��в�ѯ��
*       chunk_end_flag    һ����ڴ������ݿ��е�10��ʱ��ΪTRUE.
*   (3)all_end_flag ��ʾ������ϵ����Ϣ���Ѳ�ѯ��ϣ�PB�˿��Ը��������ʶ������һ���Ĳ��������¼����ɵ�һ��ͬ����
*       all_end_flag   һ�����ڴ������һ�����ݿ�����һ��ʱ��ΪTRUE.
*   2015-07-22
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIAPIPBSYNC_PB_GetAllUpdatedContactsHandler(uint32 request_id, MMIPB_CONTACT_T *contact_ptr, BOOLEAN chunk_end_flag ,BOOLEAN all_end_flag);

/*****************************************************************************
*  Description : �����ѯָ��contactId����ϵ�����ݣ���ת����phone֧�ֵ���ϵ�˸�ʽ��
*  Global resource dependence : 
*  Note: contact_ptr ��ת����phone֧�ָ�ʽ��Office365��ϵ����Ϣָ��
*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIAPIPBSYNC_PB_GetContactByIdHandler(uint32 request_id, MMIPB_CONTACT_T *contact_ptr);

/*****************************************************************************/
//  Description : �����Office365��ѯ����һ����ϵ�˵�contactId
//  (1) o365_id : Office365��ϵ��Idָ��
//  (2) uiTime �޸�ʱ��
//  (3) chunk_end_flag : ��ʾ��ǰ��ѯ�����ݿ���������Ը������ж��Ƿ���Ҫ������ѯ���µ���ϵ����Ϣ��id���޸�ʱ�䣩������ǣ�������ɱ�����Ϣ���������server�ṩ��MMIAPIPBSYNC_SVR_GetAllIdAndModiTime()�ӿڽ��в�ѯ��
//      chunk_end_flag һ����ڴ������ݿ��е�10��ʱ��ΪTRUE.
//  (4) all_end_flag ��ʾ������ϵ��id ���Ѳ�ѯ��ϣ�PB �˿��Ը��������ʶ������һ���Ĳ������翪ʼ��phont_update_list����ͻ��顣
//      all_end_flagһ�����ڴ������һ�����ݿ�����һ����Ϣʱ��ΪTRUE.
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIAPIPBSYNC_PB_GetAllIdAndModiTimeHandler (uint32 request_id, char* o365_id, uint32 uiTime, BOOLEAN chunk_end_flag , BOOLEAN all_end_flag);

/*****************************************************************************/
//  Description : �����ѯ���ص�Office365��ǰ��ϵ������
//  Global resource dependence : 
//  Note: total_count Office365��ǰ��ϵ������
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIAPIPBSYNC_PB_GetTotalCountHandler(uint32 request_id, uint16 total_count);

/*****************************************************************************/
//  Description : �����ѯ���ص�Office365ĳ��ʱ���Ժ������ϵ�˵�����
//  Global resource dependence : 
//  Note: total_count Office365ĳ��ʱ���Ժ������ϵ�˵�����
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIAPIPBSYNC_PB_GetAllAddedContactsCountHandler(uint32 request_id, uint16 total_count);

/*****************************************************************************/
//  Description : �����ѯ���ص�Office365ĳ��ʱ���Ժ��޸���ϵ�˵�����
//  Global resource dependence : 
//  Note: total_count Office365ĳ��ʱ���Ժ������ϵ�˵�����
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIAPIPBSYNC_PB_GetAllUpdatedContactsCountHandler(uint32 request_id, uint16 total_count);

/*****************************************************************************/
//  Description : �������һ����ϵ�˵�Office365֮�󷵻ص���ϵ����Ϣ��contactid���޸�ʱ�䣩
//  Global resource dependence : 
//  Note: o365_id Office365��ϵ��Idָ�룬uiTime uint32����Office365��ϵ������޸�ʱ��
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIAPIPBSYNC_PB_AddContactHandler(uint32 request_id, char* o365_id, uint32 uiTime);

/*****************************************************************************/
//  Description : �����޸�Office365��һ����ϵ��֮�󷵻ص���ϵ����Ϣ��contactid���޸�ʱ�䣩
//  Global resource dependence : 
//  Note: o365_id Office365��ϵ��Idָ�룬uiTime uint32����Office365��ϵ������޸�ʱ��
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIAPIPBSYNC_PB_ModifyContactHandler(uint32 request_id, char* o365_id, uint32 uiTime, BOOLEAN bDeletedOnServer);

/*****************************************************************************/
//  Description : ����Delete Office365��һ����ϵ��call back
//  Global resource dependence : 
//  Note: 
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIAPIPBSYNC_PB_DeleteContactHandler(uint32 request_id, BOOLEAN isDeleted);

/*****************************************************************************/
//  Description : ����������Office365Ӧ���쳣ʱ��server���ô˽ӿ�
//  Global resource dependence : 
//  Note: uint32 error_code���������������ֱ�����������
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIAPIPBSYNC_PB_RequestFailHandler(uint32 request_id, uint32 error_code);

/*****************************************************************************/
//  Description : Callback hander  when login is done
//  Global resource dependence : 
//  Note: 
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIAPIPBSYNC_PB_LoginHandler(uint32 request_id);

/*****************************************************************************/
//  Description : Callback hander  when logout is done
//  Global resource dependence : 
//  Note: 
/*****************************************************************************/
PUBLIC MMIPB_ERROR_E MMIAPIPBSYNC_PB_LogoutHandler(uint32 request_id);

#ifdef   __cplusplus
    }
#endif
#endif

#endif


