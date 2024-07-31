#ifdef __cplusplus
extern "C" {
#endif

#include "os_api.h"
#include "ffs.h"
#include "sfs.h"
#include "mmifmm_export.h"

#include "javacall_file.h"
#include "javacall_dir.h"
#include "javacall_logging.h"
#include "mmi_common.h"
#include "mmi_nv.h"
#include "javacall_defs.h"
#include "javacall_dir.h"
    
//#define DEBUG_FILE javacall_printf
#define DEBUG_FILE 
	
	//����ļ�������Ҫ����ƽ̨������ȷ�����ô��Ķ���ֻ����ʱʹ��
//MMIFILE_FILE_NAME_MAX_LEN 255
    
wchar    javahome_path_name[MMIFILE_FULL_PATH_MAX_LEN ] = {0};
uint32    javahome_path_len = MMIFILE_FULL_PATH_MAX_LEN;

wchar    root_dir_name[MMIFILE_FULL_PATH_MAX_LEN] = {0};
uint16    root_dir_len = MMIFILE_FULL_PATH_MAX_LEN;

wchar  driver_name[MMIFILE_FULL_PATH_MAX_LEN] = {0};
uint16    driver_name_len = MMIFILE_FULL_PATH_MAX_LEN;

/**
* Initializes the File System
* @return <tt>JAVACALL_OK</tt> on success, <tt>JAVACALL_FAIL</tt> or negative value on error
*/
javacall_result javacall_file_init(void) {
    //��sddisk��Ŀ¼�½���"javahome"Ŀ¼,"javahome"Ŀ¼�´���"appdb"��"lib"Ŀ¼	
    int32 i = 0;

    wchar    config_path_name[MMIFILE_FULL_PATH_MAX_LEN] = {0};
    uint32    config_path_len = MMIFILE_FULL_PATH_MAX_LEN;

    wchar    appdb_path_name[MMIFILE_FULL_PATH_MAX_LEN] = {0};
    uint32    appdb_path_len = MMIFILE_FULL_PATH_MAX_LEN;

    DEBUG_FILE(LOG_STR_ENTER(""));

#ifdef SC6600L_ARM7
    if(MMIAPIFMM_GetDeviceStatus((uint16 *)MMIFILE_DEVICE_SDCARD, MMIFILE_DEVICE_SDCARD_LEN)){
        DEBUG_FILE(LOG_STR("SDCARD is ready!"));
        }else{//sddiskû��׼���ã����ܽ����ļ�������ֱ�ӷ���
            DEBUG_FILE(LOG_STR_LEAVE("...SDCARD is not exist !!"));
            return JAVACALL_FAIL;
        }
#endif

    helper_dir_get_home_path(javahome_path_name, &javahome_path_len);/*lint !e64 */

    javacall_dir_get_root_path(appdb_path_name, &appdb_path_len);/*lint !e64 */

    javacall_dir_get_config_path(config_path_name, &config_path_len);/*lint !e64 */

    for(i = 3;i<(int32)appdb_path_len;i++){
        root_dir_name[i-3] = *(appdb_path_name+i);
    }
    root_dir_len = (uint16)MMIAPICOM_Wstrlen(root_dir_name);

    driver_name[0] = *(appdb_path_name);
    driver_name[1] = NULL;    
    driver_name_len = 1;

    //�ж�javahomeĿ¼�Ƿ��Ѿ�����
    if(!MMIAPIFMM_IsFolderExist(javahome_path_name, (uint16)javahome_path_len)){
        //�����ڴ���javahomeĿ¼
        if(MMIAPIFMM_CreateDir(javahome_path_name, (uint16)javahome_path_len)){
            //����javaĿ¼�ɹ�
            DEBUG_FILE(LOG_STR("Create javahome_dir success!\n"));
        }else{//����javaĿ¼ʧ��
            DEBUG_FILE(LOG_STR_LEAVE("Create javahome_dir failed!"));
            return JAVACALL_FAIL;
        }
    }
    //���ڣ��ж�appdb��libĿ¼�Ƿ��Ѿ�����
    DEBUG_FILE(LOG_STR("...javahome_dir already exist!"));
    if(!MMIAPIFMM_IsFolderExist(appdb_path_name, (uint16)appdb_path_len)){
        //�����ڴ���appdbĿ¼
        if(MMIAPIFMM_CreateDir(appdb_path_name, (uint16)appdb_path_len)){
        //����appdbĿ¼�ɹ�
        DEBUG_FILE(LOG_STR("...Create appdb_dir success!\n"));
        }else{//����appdbĿ¼ʧ��
            DEBUG_FILE(LOG_STR_LEAVE("...Create appdb_dir failed!\n"));
            return JAVACALL_FAIL;
        }
    }else{
        DEBUG_FILE(LOG_STR("...appdb_dir already exist!"));
    }

    if(!MMIAPIFMM_IsFolderExist(config_path_name, (uint16)config_path_len)){
        //�����ڴ���libĿ¼
        if(MMIAPIFMM_CreateDir(config_path_name, (uint16)config_path_len)){
            //����javaĿ¼�ɹ�
            DEBUG_FILE(LOG_STR("...Create lib_dir success!"));
        }else{//����javaĿ¼ʧ��
            DEBUG_FILE(LOG_STR_LEAVE("...Create lib_dir failed!"));
            return JAVACALL_FAIL;
        }
    }else{
        DEBUG_FILE(LOG_STR("...lib_dir already exist!"));
    }
    return JAVACALL_OK;
}
/**
* Cleans up resources used by file system
* @return <tt>JAVACALL_OK</tt> on success, <tt>JAVACALL_FAIL</tt> or negative value on error
*/ 
javacall_result javacall_file_finalize(void) {
    DEBUG_FILE(LOG_STR_LEAVE("ok"));
    return JAVACALL_OK;
}

/**
* The open a file
* @param unicodeFileName path name in UNICODE of file to be opened
* @param fileNameLen length of file name
* @param flags open control flags
*        Applications must specify exactly one of the first three
*        values (file access modes) below in the value of "flags"
*        JAVACALL_FILE_O_RDONLY, JAVACALL_FILE_O_WRONLY, JAVACALL_FILE_O_RDWR
*
*        Any combination (bitwise-inclusive-OR) of the following may be used:
*        JAVACALL_FILE_O_CREAT, JAVACALL_FILE_O_TRUNC, JAVACALL_FILE_O_APPEND,
*
* @param handle address of pointer to file identifier
*        on successful completion, file identifier is returned in this 
*        argument. This identifier is platform specific and is opaque
*        to the caller.  
* @return <tt>JAVACALL_OK</tt> on success, 
*         <tt>JAVACALL_FAIL</tt> or negative value on error
* 
*/
javacall_result javacall_file_open(const javacall_utf16 * unicodeFileName, int fileNameLen, int flags, /*OUT*/ javacall_handle * handle) {
    MMIFILE_HANDLE fd = 0;
    uint32 oFlags = 0x00;
    wchar       full_path_name[MMIFILE_FULL_PATH_MAX_LEN+1] = {0};
    uint16       full_path_len = MMIFILE_FULL_PATH_MAX_LEN;

    DEBUG_FILE(LOG_STR_ENTER("file=%s, flags=0x%x"), javacall_wstr2str(unicodeFileName), flags);/*lint !e64 */

#ifdef NO_SUPPORT_AB_FS
    //skip the '/' char
    while(47 == *unicodeFileName){
        unicodeFileName++;
        fileNameLen--;
    }
#endif

    if(NULL == unicodeFileName || NULL == handle){
        DEBUG_FILE(LOG_STR_LEAVE("fail, para is null!"));
	return JAVACALL_FAIL;
    }
    if(MMIFILE_FILE_NAME_MAX_LEN < fileNameLen){
        DEBUG_FILE(LOG_STR_LEAVE("fail, fileNameLen is too long!"));
	return JAVACALL_FAIL;
    }

    /* unicodeFileName �ļ������û��':\'·��,���Զ�����Java·��*/
    if(fileNameLen >= 3 && unicodeFileName[1] == MMIFILE_COLON && unicodeFileName[2] == MMIFILE_SLASH){//�ļ�����':\'
        MMIAPICOM_Wstrncpy(full_path_name,(const wchar*)unicodeFileName,MIN(MMIFILE_FULL_PATH_MAX_LEN,MIN(MMIFILE_FULL_PATH_MAX_LEN,fileNameLen)));
    }else{//�ļ���δ��':\'
        if((javahome_path_len + fileNameLen+1) > MMIFILE_FULL_PATH_MAX_LEN){/*lint !e737 */
            DEBUG_FILE(LOG_STR_LEAVE("fail, file name length is too long!"));
            return JAVACALL_FAIL;
        }else{
            MMIAPIFMM_CombineFullPath(driver_name, driver_name_len,
				root_dir_name, root_dir_len,
				(const wchar*)unicodeFileName, fileNameLen,
				full_path_name, &full_path_len);
        }
    }

    //�ļ���ģʽmode����
    //SFS_MODE_READ              = 0x0001,  �ɶ�
    //SFS_MODE_WRITE                = 0x0002,  ��д. ��дģʽ�����create/open������һ�����
    //SFS_MODE_SHARE_READ      = 0x0004,  �����, ����ʹ��
    //SFS_MODE_SHARE_WRITE     = 0x0008,  ����д������ʹ��
    //SFS_MODE_CREATE_NEW      = 0x0010,  �½�,����Ѿ������򷵻�ʧ��
    //SFS_MODE_CREATE_ALWAYS = 0x0020,  �����½�������������½�����
    //SFS_MODE_OPEN_EXISTING   = 0x0030,  ���Ѵ��ڣ�����������򷵻�ʧ��
    //SFS_MODE_OPEN_ALWAYS     = 0x0040,  ���Ǵ򿪣�������������½�
    //SFS_MODE_APPEND               = 0x0050   open/create,�������ļ����ӣ�����FFS_MODE_OPEN_ALWAYS����˼

    //����������mode
    //#define JAVACALL_FILE_O_APPEND   0x400  
    //#define JAVACALL_FILE_O_TRUNC    0x200  
    //#define JAVACALL_FILE_O_CREAT    0x40  �������򴴽����������?
    //#define JAVACALL_FILE_O_RDONLY   0x00  
    //#define JAVACALL_FILE_O_RDWR     0x02  
    //#define JAVACALL_FILE_O_WRONLY   0x01  	

    //�Ƿ񴴽��ļ�
    if(flags & JAVACALL_FILE_O_CREAT){
        switch(flags & (~JAVACALL_FILE_O_CREAT))
            {
            case JAVACALL_FILE_O_RDONLY: //ֻ��
                oFlags = (SFS_MODE_SHARE_READ | SFS_MODE_OPEN_ALWAYS);/*lint !e655*/
                fd = MMIAPIFMM_CreateFile(full_path_name, oFlags, NULL, NULL);
                break;
            case JAVACALL_FILE_O_WRONLY: //ֻд
                oFlags = (SFS_MODE_SHARE_WRITE | SFS_MODE_OPEN_ALWAYS);/*lint !e655*/
                fd = MMIAPIFMM_CreateFile(full_path_name, oFlags, NULL, NULL);
                break;
            case JAVACALL_FILE_O_RDWR: //��д
                oFlags = (SFS_MODE_SHARE_READ | SFS_MODE_SHARE_WRITE | SFS_MODE_OPEN_ALWAYS);/*lint !e655*/
                fd = MMIAPIFMM_CreateFile(full_path_name, oFlags, NULL, NULL);
                break;
            case (JAVACALL_FILE_O_TRUNC | JAVACALL_FILE_O_WRONLY): /*lint !e655*///��ȡ+ֻд
                oFlags = (SFS_MODE_CREATE_ALWAYS | SFS_MODE_SHARE_WRITE);/*lint !e655*/
                fd = MMIAPIFMM_CreateFile(full_path_name, oFlags, NULL, NULL);
                break;
            case (JAVACALL_FILE_O_TRUNC | JAVACALL_FILE_O_RDWR): /*lint !e655*///��ȡ+��д
                oFlags = (SFS_MODE_CREATE_ALWAYS | SFS_MODE_SHARE_WRITE | SFS_MODE_SHARE_READ);/*lint !e655*/
                fd = MMIAPIFMM_CreateFile(full_path_name, oFlags, NULL, NULL);
                break;
            case (JAVACALL_FILE_O_APPEND | JAVACALL_FILE_O_WRONLY):/*lint !e655*/ //append+ֻд
                oFlags = (SFS_MODE_APPEND | SFS_MODE_SHARE_WRITE);/*lint !e655*/
                fd = MMIAPIFMM_CreateFile(full_path_name, oFlags, NULL, NULL);
                break;
            case (JAVACALL_FILE_O_APPEND | JAVACALL_FILE_O_RDWR):/*lint !e655*/ //append+��д
                oFlags = (SFS_MODE_APPEND | SFS_MODE_SHARE_WRITE | SFS_MODE_SHARE_READ);/*lint !e655*/
                fd = MMIAPIFMM_CreateFile(full_path_name, oFlags, NULL, NULL);
                break;
            default:
                break;
            }
    }else{
        switch(flags){
            case JAVACALL_FILE_O_RDONLY:
                oFlags = (SFS_MODE_SHARE_READ | SFS_MODE_OPEN_EXISTING);/*lint !e655*/
                fd = MMIAPIFMM_CreateFile(full_path_name, oFlags, NULL, NULL);
                break;
            case JAVACALL_FILE_O_WRONLY:
                oFlags = (SFS_MODE_SHARE_WRITE | SFS_MODE_OPEN_EXISTING);/*lint !e655*/
                fd = MMIAPIFMM_CreateFile(full_path_name, oFlags, NULL, NULL);
                break;
            case JAVACALL_FILE_O_RDWR:
                oFlags = (SFS_MODE_SHARE_READ | SFS_MODE_SHARE_WRITE | SFS_MODE_OPEN_EXISTING);/*lint !e655*/
                fd = MMIAPIFMM_CreateFile(full_path_name, oFlags, NULL, NULL);
                break;
            //+++++++++++++truncate+++++++++++++//
            case JAVACALL_FILE_O_TRUNC: //�������Ӧ�ò�����֣���nts�����
                oFlags = (SFS_MODE_SHARE_READ | SFS_MODE_OPEN_EXISTING);/*lint !e655*/
                fd = MMIAPIFMM_CreateFile(full_path_name, oFlags, NULL, NULL);
                //�ж��ļ��Ƿ����
                if(SFS_INVALID_HANDLE != fd) {//���ڣ������ļ����
                    oFlags = (SFS_MODE_SHARE_WRITE |SFS_MODE_CREATE_ALWAYS);/*lint !e655*/ //ͨ�������½��ļ����ǵķ�ʽ���
                    MMIAPIFMM_CloseFile(fd);
                    fd = MMIAPIFMM_CreateFile(full_path_name, oFlags, NULL, NULL);
                }
                break;
            case (JAVACALL_FILE_O_TRUNC | JAVACALL_FILE_O_WRONLY):/*lint !e655*/ //��ȡ+ֻд
                oFlags = (SFS_MODE_SHARE_READ | SFS_MODE_OPEN_EXISTING);/*lint !e655*/
                fd = MMIAPIFMM_CreateFile(full_path_name, oFlags, NULL, NULL);
                //�ж��ļ��Ƿ����
                if(SFS_INVALID_HANDLE != fd) {//���ڣ������ļ����
                    oFlags = (SFS_MODE_CREATE_ALWAYS | SFS_MODE_SHARE_WRITE);/*lint !e655*/ //ͨ�������½��ļ����ǵķ�ʽ���
                    MMIAPIFMM_CloseFile(fd);
                    fd = MMIAPIFMM_CreateFile(full_path_name, oFlags, NULL, NULL);
                }
                break;
            case (JAVACALL_FILE_O_TRUNC | JAVACALL_FILE_O_RDWR):/*lint !e655*/ //��ȡ+��д
                oFlags = (SFS_MODE_SHARE_READ | SFS_MODE_OPEN_EXISTING);/*lint !e655*/
                fd = MMIAPIFMM_CreateFile(full_path_name, oFlags, NULL, NULL);
                //�ж��ļ��Ƿ����
                if(SFS_INVALID_HANDLE != fd) {//���ڣ������ļ����
                    oFlags = (SFS_MODE_CREATE_ALWAYS | SFS_MODE_SHARE_WRITE | SFS_MODE_SHARE_READ);/*lint !e655*/ //ͨ�������½��ļ����ǵķ�ʽ���
                    MMIAPIFMM_CloseFile(fd);
                    fd = MMIAPIFMM_CreateFile(full_path_name, oFlags, NULL, NULL);
                }
                break;
            //+++++++++++++append+++++++++++++//
            case JAVACALL_FILE_O_APPEND: //�������Ӧ�ò�����֣���nts�����
                oFlags = (SFS_MODE_SHARE_READ | SFS_MODE_OPEN_EXISTING);/*lint !e655*/
                fd = MMIAPIFMM_CreateFile(full_path_name, oFlags, NULL, NULL);
                //�ж��ļ��Ƿ����
                if(SFS_INVALID_HANDLE != fd) {
                    oFlags = (SFS_MODE_SHARE_WRITE | SFS_MODE_APPEND);/*lint !e655*/
                    MMIAPIFMM_CloseFile(fd);
                    fd = MMIAPIFMM_CreateFile(full_path_name, oFlags, NULL, NULL);
                }
                break;
            case (JAVACALL_FILE_O_APPEND | JAVACALL_FILE_O_WRONLY):/*lint !e655*/ //append+ֻд
                oFlags = (SFS_MODE_SHARE_READ | SFS_MODE_OPEN_EXISTING);/*lint !e655*/
                fd = MMIAPIFMM_CreateFile(full_path_name, oFlags, NULL, NULL);
                //�ж��ļ��Ƿ����
                if(SFS_INVALID_HANDLE != fd) {
                    oFlags = (SFS_MODE_APPEND | SFS_MODE_SHARE_WRITE);/*lint !e655*/
                    MMIAPIFMM_CloseFile(fd);
                    fd = MMIAPIFMM_CreateFile(full_path_name, oFlags, NULL, NULL);
                }
                break;
            case (JAVACALL_FILE_O_APPEND | JAVACALL_FILE_O_RDWR):/*lint !e655*/ //append+��д
                oFlags = (SFS_MODE_SHARE_READ | SFS_MODE_OPEN_EXISTING);/*lint !e655*/
                fd = MMIAPIFMM_CreateFile(full_path_name, oFlags, NULL, NULL);
                //�ж��ļ��Ƿ����
                if(SFS_INVALID_HANDLE != fd) {
                    oFlags = (SFS_MODE_APPEND | SFS_MODE_SHARE_WRITE | SFS_MODE_SHARE_READ);/*lint !e655*/
                    MMIAPIFMM_CloseFile(fd);
                    fd = MMIAPIFMM_CreateFile(full_path_name, oFlags, NULL, NULL);
                }
                break;
            default:
                break;
        }
    }

    if(SFS_INVALID_HANDLE == fd) {
        *handle = NULL;
        DEBUG_FILE(LOG_STR_LEAVE("fail"));
        return JAVACALL_FAIL;
    }else{
        *handle = (void *)fd;
        DEBUG_FILE(LOG_STR_LEAVE("ok, handle=%d"), *handle);
        return JAVACALL_OK;
    }
}

/**
* Closes the file with the specified handlei
* @param handle handle of file to be closed
* @return <tt>JAVACALL_OK</tt> on success, 
*         <tt>JAVACALL_FAIL</tt> or negative value otherwise
*/
javacall_result javacall_file_close(javacall_handle handle) {
    if(SFS_NO_ERROR != MMIAPIFMM_CloseFile((MMIFILE_HANDLE)handle)){
        DEBUG_FILE(LOG_STR_LEAVE("fail, handle=%d"), handle);
        return JAVACALL_FAIL;
    }
    DEBUG_FILE(LOG_STR("ok, handle=%d"), handle);
    return JAVACALL_OK;
}


/**
* Reads a specified number of bytes from a file, 
* @param handle handle of file 
* @param buf buffer to which data is read
* @param size number of bytes to be read. Actual number of bytes
*              read may be less, if an end-of-file is encountered
* @return the number of bytes actually read
*/
long javacall_file_read(javacall_handle handle, unsigned char *buf, long size) {
    SFS_ERROR_E sfs_error = SFS_NO_ERROR;
    uint32 transmitted = 0;
    uint32 sizeToRead = 0;

    if(NULL == buf){
        DEBUG_FILE(LOG_STR_LEAVE("para is null!"));
        return 0;
    }

    //���������漰��long(64bit)��uint32(32bit)֮���ת����Ϊ�˷�ֹ�����������Ĵ���
    //��һ��size����32λ���͵ı�ʾ��Χ����ʹ������32λֵ
    sizeToRead = size&0xffffffff;/*lint !e737 */
    sfs_error = MMIAPIFMM_ReadFile((MMIFILE_HANDLE)handle, (uint8*)buf, sizeToRead, &transmitted, NULL);

    if (SFS_NO_ERROR == sfs_error){
        DEBUG_FILE(LOG_STR_LEAVE("ok, read=%d, handle=%d"), transmitted, handle);
        return (long)transmitted;
    }else{
        DEBUG_FILE(LOG_STR_LEAVE("fail, handle=%d, err=%d"), handle, sfs_error);
        return 0;
    }
}

/**
* Writes bytes to file
* @param handle handle of file 
* @param buf buffer to be written
* @param size number of bytes to write
* @return the number of bytes actually written. This is normally the same 
*         as size, but might be less (for example, if the persistent storage being 
*         written to fills up).
*/
long javacall_file_write(javacall_handle handle, const unsigned char *buf, long size) {
    SFS_ERROR_E sfs_error = SFS_NO_ERROR;
    uint32 transmitted = 0;
    uint32 sizeToWrite = 0;

    //���������漰��long(64bit)��uint32(32bit)֮���ת����Ϊ�˷�ֹ�����������Ĵ���
    //��һ��size����32λ���͵ı�ʾ��Χ����ʹ������32λֵ

    if(NULL == buf){
        DEBUG_FILE(LOG_STR_LEAVE("fail, para is null!"));
        return 0;
    }

    sizeToWrite = size&0xffffffff;/*lint !e737 */
    sfs_error = MMIAPIFMM_WriteFile((MMIFILE_HANDLE)handle, (uint8*)buf, sizeToWrite, &transmitted, NULL);

    if (SFS_NO_ERROR == sfs_error){
        DEBUG_FILE(LOG_STR_LEAVE("ok, write=%d, handle=%d"), transmitted, handle);
        return (long)transmitted;
    }else{
        DEBUG_FILE(LOG_STR_LEAVE("fail, handle=%d, err=%d"), handle, sfs_error);
        return 0;
    }
}

/**
* Deletes a file from the persistent storage.
* @param unicodeFileName name of file to be deleted
* @param fileNameLen length of file name
* @return JAVACALL_OK on success, <tt>JAVACALL_FAIL</tt> or negative value otherwise
*/
javacall_result javacall_file_delete(const javacall_utf16 * unicodeFileName, int fileNameLen) {
    SFS_ERROR_E sfs_error = SFS_NO_ERROR;

    wchar    full_path_name[MMIFILE_FULL_PATH_MAX_LEN + 1] = {0};
    uint16    full_path_len = MMIFILE_FULL_PATH_MAX_LEN;
   
#ifdef NO_SUPPORT_AB_FS
    //skip the '/' char
    while(47 == *unicodeFileName){
        unicodeFileName++;
        fileNameLen--;
    }
#endif
	
    if(NULL == unicodeFileName){
        DEBUG_FILE(LOG_STR_LEAVE("fail, para is null!"));
        return JAVACALL_FAIL;
    }

    if( fileNameLen > MMIFILE_FILE_NAME_MAX_LEN ) {
        DEBUG_FILE(LOG_STR_LEAVE("file name is too long!"));
        return JAVACALL_FAIL;
    }

    /*�ļ������û��':\'·��,���Զ�����Java·��*/
    if(fileNameLen >= 3 && unicodeFileName[1] == MMIFILE_COLON && unicodeFileName[2] == MMIFILE_SLASH){//�ļ�����':\'
        MMIAPICOM_Wstrncpy(full_path_name,(const wchar*)unicodeFileName,MIN(MMIFILE_FULL_PATH_MAX_LEN,fileNameLen));
    }else{//�ļ���δ��':\'
        if((javahome_path_len + fileNameLen+1) > MMIFILE_FULL_PATH_MAX_LEN){/*lint !e737 */
            DEBUG_FILE(LOG_STR_LEAVE("fail, file name length is too long!"));
            return JAVACALL_FAIL;
        }else{
            MMIAPIFMM_CombineFullPath(driver_name, driver_name_len,
				root_dir_name, root_dir_len,
				(const wchar*)unicodeFileName, fileNameLen,
				full_path_name, &full_path_len);
        }
    }

    sfs_error = MMIAPIFMM_DeleteFile(full_path_name,PNULL);

    if (SFS_NO_ERROR == sfs_error) {
        DEBUG_FILE(LOG_STR_LEAVE("ok, delete %s"), javacall_wstr2str(unicodeFileName));/*lint !e64 */
        return JAVACALL_OK;
    } else if (SFS_ERROR_NOT_EXIST == sfs_error) {
        DEBUG_FILE(LOG_STR_LEAVE("ok, delete %s"), javacall_wstr2str(unicodeFileName));/*lint !e64 */
        return JAVACALL_OK;
    }else{
        DEBUG_FILE(LOG_STR_LEAVE("fail, delete %s, err=%d"), javacall_wstr2str(unicodeFileName), sfs_error);/*lint !e64 */
        return JAVACALL_FAIL;
    }
}

/**
* The  truncate function is used to truncate the size of an open file in 
* the filesystem storage.
* @param handle identifier of file to be truncated
*         This is the identifier returned by javacall_file_open()
*         The handle may be optionally modified by the implementation
*         of this function
* @param size size to truncate to
* @return <tt>JAVACALL_OK</tt> on success, 
*         <tt>JAVACALL_FAIL</tt> or negative value on error
*/
javacall_result javacall_file_truncate(javacall_handle handle, javacall_int64 size) {
    SFS_ERROR_E sfs_error = SFS_NO_ERROR;
    uint32 newFileLen = 0;
    uint32 oldFileLen;
    int32 lenDet;

    //���������漰��long(64bit)��uint32(32bit)֮���ת����Ϊ�˷�ֹ�����������Ĵ���
    //��һ��size����32λ���͵ı�ʾ��Χ����ʹ������32λֵ
    if(size >=0){
        newFileLen = size&0xffffffff;/*lint !e737 */
    }else{
        newFileLen = -((-size)&0xffffffff);/*lint !e737 */
    }

    if(PNULL == handle){
        DEBUG_FILE(LOG_STR_LEAVE("fail, handle is null"));
        return JAVACALL_FAIL;
    }

    /*�õ��ļ�����*/
    /*��ΪչѶ��չ�ļ�ʱ,���沿����������������,������չ�ļ�ʱ,����ʹ��չѶ�ĺ���
    ��Ҫ���ļ���дָ�����ȵ�0*/
    
    oldFileLen = MMIAPIFMM_GetFileSize((MMIFILE_HANDLE)handle);
    DEBUG_FILE(LOG_STR("size for handle:%d"),oldFileLen);

    lenDet = newFileLen - oldFileLen;

    /*��С�ļ�*/
    if(lenDet<0){
        sfs_error = MMIAPIFMM_SetFileSize((MMIFILE_HANDLE)handle, newFileLen);
        if(sfs_error == SFS_NO_ERROR){
            return JAVACALL_OK;
        }else{
            return JAVACALL_FAIL;
        }
    }
    /*��չ�ļ�*/
    if(lenDet>0){
        int8 cZeroStr[1024] = {0};
        int32 result = 0;
        int32 i = 0;

        SCI_MEMSET(cZeroStr,0,1024);
        /*���α����õ��ļ���β*/
        result = javacall_file_seek(handle,0,MMIFILE_SEEK_END);/*lint !e64 !e747 !e712 */
        if(result == JAVACALL_FAIL){
            return JAVACALL_FAIL;
        }

        for(i = 0;i<lenDet/1024;i++){
            result= javacall_file_write(handle,(const BYTE*)cZeroStr,1024);
            if(result != 1024){
                return JAVACALL_FAIL;
            }
        }

        result = javacall_file_write(handle,(const BYTE*)cZeroStr,lenDet%1024);
        if(lenDet%1024 !=result){
            return JAVACALL_FAIL;
        }
        return JAVACALL_OK;
    }
    return JAVACALL_OK;
}

/**
* Sets the file pointer associated with a file identifier 
* @param handle identifier of file
*               This is the identifier returned by javacall_file_open()
* @param offset number of bytes to offset file position by
* @param flag controls from where offset is applied, from 
*                 the beginning, current position or the end
*                 Can be one of JAVACALL_FILE_SEEK_CUR, JAVACALL_FILE_SEEK_SET 
*                 or JAVACALL_FILE_SEEK_END
* @return on success the actual resulting offset from beginning of file
*         is returned, otherwise -1 is returned
*/
javacall_int64 javacall_file_seek(javacall_handle handle, javacall_int64 offset, javacall_file_seek_flags flag) {
    uint32 offset_new = 0;
    uint32 offset_seek = 0; //actual offset
    uint32 fileLen = 0;
    uint32 tellPos = 0;

    //���������漰��long(64bit)��uint32(32bit)֮���ת����Ϊ�˷�ֹ�����������Ĵ���
    //��һ��size����32λ���͵ı�ʾ��Χ����ʹ������32λֵ
    if(offset >=0){
        offset_new = offset&0xffffffff;/*lint !e737 */
    }else{
        offset_new = -((-offset)&0xffffffff);/*lint !e737 */
    }

    //�ļ�����
    fileLen = MMIAPIFMM_GetFileSize((MMIFILE_HANDLE)handle);
    //��ǰ�ļ�ָ���ƫ����
    tellPos = MMIAPIFMM_GetFilePointer((MMIFILE_HANDLE)handle, MMIFILE_SEEK_BEGIN);

    if (MMIFILE_SEEK_BEGIN == flag){ //seek from file start
        if(SFS_NO_ERROR == MMIAPIFMM_SetFilePointer((MMIFILE_HANDLE)handle, offset_new, flag)){
            DEBUG_FILE(LOG_STR_LEAVE("ok, seek from file start"));
            offset_seek = offset_new;
        }else{
            DEBUG_FILE(LOG_STR_LEAVE("fail, seek from file start!"));
            return (javacall_int64)-1;
        }
    }
    if (MMIFILE_SEEK_CUR == flag){ //seek from current
        if(SFS_NO_ERROR == MMIAPIFMM_SetFilePointer((MMIFILE_HANDLE)handle, offset_new, flag)){
            DEBUG_FILE(LOG_STR_LEAVE("ok, seek from current"));
            offset_seek = tellPos + offset_new;
        }else{
            DEBUG_FILE(LOG_STR_LEAVE("fail, seek from current"));
            return (javacall_int64)-1;
        }
    }
    if (MMIFILE_SEEK_END == flag){ //seek from file end
        if(SFS_NO_ERROR == MMIAPIFMM_SetFilePointer((MMIFILE_HANDLE)handle, offset_new, flag)){
            DEBUG_FILE(LOG_STR_LEAVE("ok, seek from end"));
            offset_seek = fileLen + offset_new;
            if (offset_seek <= 0){/*lint !e775 */ //���ļ�ĩβseek�����ֻ�ܵ��ļ���ͷ
	         offset_seek = 0;
            }
        }else{
            DEBUG_FILE(LOG_STR_LEAVE("fail, seek from current"));
            return (javacall_int64)-1;
        }
    }
	return (javacall_int64)offset_seek;
}

/**
* Get file size 
* @param handle identifier of file
*               This is the identifier returned by pcsl_file_open()
* @return size of file in bytes if successful, -1 otherwise
*/
javacall_int64 javacall_file_sizeofopenfile(javacall_handle handle) {
    uint32 length = 0;

    //length = MMIAPIFMM_GetFileSize((MMIFILE_HANDLE)handle);
    if(PNULL == handle){
        DEBUG_FILE(LOG_STR_LEAVE("fail, para is null!"));
        return (javacall_int64)-1;
    }
    if(SFS_ERROR_NONE != SFS_GetFileSize((MMIFILE_HANDLE)handle, &length)){
        DEBUG_FILE(LOG_STR_LEAVE("fail, SFS_GetFileSize fail"));
        return (javacall_int64)-1;
    }

    DEBUG_FILE(LOG_STR_LEAVE("size=%d, handle=%d"), length, handle);
    return (javacall_int64)length;
}

/**
* Get file size
* @param fileName name of file in unicode format
* @param fileNameLen length of file name
* @return size of file in bytes if successful, -1 otherwise 
*/
javacall_int64 javacall_file_sizeof(const javacall_utf16 * fileName, int fileNameLen) {
    MMIFILE_HANDLE fd = 0;
    uint32 length = 0;

    wchar    full_path_name[MMIFILE_FULL_PATH_MAX_LEN + 1] = {0};
    uint16    full_path_len = MMIFILE_FULL_PATH_MAX_LEN;

#ifdef NO_SUPPORT_AB_FS
    //skip the '/' char
    while(47 == *fileName){
        fileName++;
        fileNameLen--;
    }
#endif
    if(NULL == fileName){
        DEBUG_FILE(LOG_STR("...javacall_file_sizeof para is null!\n"));
        return (javacall_int64)-1;
    }

    if(MMIFILE_FILE_NAME_MAX_LEN < fileNameLen){
        DEBUG_FILE(LOG_STR("javacall_file_sizeof fileNameLen is too long!\n"));
        return (javacall_int64)-1;
    }

    /*�ļ������û��':\'·��,���Զ�����Java·��*/
    if(fileNameLen >= 3 && fileName[1] == MMIFILE_COLON && fileName[2] == MMIFILE_SLASH){//�ļ�����':\'
        MMIAPICOM_Wstrncpy(full_path_name,(const wchar*)fileName,MIN(MMIFILE_FULL_PATH_MAX_LEN,fileNameLen));
    }else{//�ļ���δ��':\'
        if((javahome_path_len + fileNameLen+1) > MMIFILE_FULL_PATH_MAX_LEN){/*lint !e737 */
            DEBUG_FILE(LOG_STR("...javacall_file_sizeof file name length is too long!"));
            return JAVACALL_FAIL;
        }else{
            MMIAPIFMM_CombineFullPath(driver_name, driver_name_len,
				root_dir_name, root_dir_len,
				(const wchar*)fileName, fileNameLen,
				full_path_name, &full_path_len);
        }
    }

    //�Կɶ�ģʽ�򿪸��ļ�
    fd = MMIAPIFMM_CreateFile((const wchar*)full_path_name, (SFS_MODE_SHARE_READ | SFS_MODE_OPEN_EXISTING), NULL, NULL);/*lint !e655*/
    if(SFS_INVALID_HANDLE == fd) {
        DEBUG_FILE(LOG_STR_LEAVE("fail to open file"));
        return (javacall_int64)-1;
    }

    //length = MMIAPIFMM_GetFileSize(fd);
    if(SFS_ERROR_NONE != SFS_GetFileSize(fd, &length)){
        DEBUG_FILE(LOG_STR_LEAVE("fail"));
        MMIAPIFMM_CloseFile(fd);
        return (javacall_int64)-1;
    }
    MMIAPIFMM_CloseFile(fd);
    DEBUG_FILE(LOG_STR_LEAVE("ok"));
    return (javacall_int64)length;
}

/**
* Check if the file exists in file system storage.
* @param fileName name of file in unicode format
* @param fileNameLen length of file name
* @return <tt>JAVACALL_OK </tt> if it exists and is a regular file, 
*         <tt>JAVACALL_FAIL</tt> or negative value otherwise (eg: 0 returned if it is a directory)
*/
javacall_result javacall_file_exist(const javacall_utf16 * fileName, int fileNameLen) {
    BOOLEAN ret = FALSE;

    wchar    full_path_name[MMIFILE_FULL_PATH_MAX_LEN + 1] = {0};
    uint16    full_path_len = MMIFILE_FULL_PATH_MAX_LEN;

#ifdef NO_SUPPORT_AB_FS
    //skip the '/' char
    while(47 == *fileName){
        fileName++;
        fileNameLen--;
    }
#endif
    if(NULL == fileName){
        DEBUG_FILE(LOG_STR_LEAVE("fail, para is null!"));
        return JAVACALL_FAIL;
    }
    if(MMIFILE_FILE_NAME_MAX_LEN < fileNameLen){
        DEBUG_FILE(LOG_STR_LEAVE("fileNameLen=%d is too long."), fileNameLen);
        return JAVACALL_FAIL;
    }

    /*�ļ������û��':\'·��,���Զ�����Java·��*/
    if(fileNameLen >= 3 && fileName[1] == MMIFILE_COLON && fileName[2] == MMIFILE_SLASH){//�ļ�����':\'
        MMIAPICOM_Wstrncpy(full_path_name,(const wchar*)fileName,MIN(MMIFILE_FULL_PATH_MAX_LEN,fileNameLen));
        full_path_len = fileNameLen;
    }else{//�ļ���δ��':\'
        if((javahome_path_len + fileNameLen+1) > MMIFILE_FULL_PATH_MAX_LEN){/*lint !e737 */
            DEBUG_FILE(LOG_STR_LEAVE("fail, file name length is too long"));
            return JAVACALL_FAIL;
        }else{
            MMIAPIFMM_CombineFullPath(driver_name, driver_name_len,
				root_dir_name, root_dir_len,
				(const wchar*)fileName, fileNameLen,
				full_path_name, &full_path_len);
        }
    }

    //�ж��ļ��Ƿ����
    ret = MMIAPIFMM_IsFileExist((const wchar*)full_path_name, full_path_len);

    if(TRUE == ret) {
        DEBUG_FILE(LOG_STR_LEAVE("ok, %s exists"), javacall_wstr2str(fileName));/*lint !e64 */
        return JAVACALL_OK;
    }
    DEBUG_FILE(LOG_STR_LEAVE("fail, %s not exist"), javacall_wstr2str(fileName));/*lint !e64 */
    return JAVACALL_FAIL;
}


/** Force the data to be written into the file system storage
* @param handle identifier of file
*               This is the identifier returned by javacall_file_open()
* @return JAVACALL_OK  on success, <tt>JAVACALL_FAIL</tt> or negative value otherwise
*/
javacall_result javacall_file_flush(javacall_handle handle) {
    //չѶƽ̨��д������֧�ָù��ܣ��������sd������udisk�ڵײ�������ƵĲ��������
    //����ҪӦ�ò��ٽ��иò�����
    DEBUG_FILE(LOG_STR_LEAVE("ok"));
    return JAVACALL_OK;
}

/**
* Renames the filename.
* @param unicodeOldFilename current name of file
* @param oldNameLen current name length
* @param unicodeNewFilename new name of file
* @param newNameLen length of new name
* @return <tt>JAVACALL_OK</tt>  on success, 
*         <tt>JAVACALL_FAIL</tt> or negative value otherwise
*/
javacall_result javacall_file_rename(const javacall_utf16 * unicodeOldFilename, int oldNameLen, const javacall_utf16 * unicodeNewFilename, int newNameLen) {
    SFS_ERROR_E sfs_error = SFS_NO_ERROR;
    wchar    old_full_path[MMIFILE_FULL_PATH_MAX_LEN +1] = {0};
    uint16    old_path_len = MMIFILE_FULL_PATH_MAX_LEN;
    wchar    new_full_path[MMIFILE_FULL_PATH_MAX_LEN +1] = {0};
    uint16    new_path_len = MMIFILE_FULL_PATH_MAX_LEN;
    wchar    saved_path[MMIFILE_FULL_PATH_MAX_LEN +1] = {0};

    DEBUG_FILE(LOG_STR_ENTER("old=%s"), javacall_wstr2str(unicodeOldFilename));/*lint !e64 */
    DEBUG_FILE(LOG_STR("new=%s"), javacall_wstr2str(unicodeNewFilename));/*lint !e64 */

#ifdef NO_SUPPORT_AB_FS
    //skip the '/' char
    while(47 == *unicodeOldFilename){
        unicodeOldFilename++;
        oldNameLen--;
    }
    while(47 == *unicodeNewFilename){
        unicodeNewFilename++;
        newNameLen--;
    }
#endif

    if(NULL == unicodeOldFilename || NULL == unicodeNewFilename){
        DEBUG_FILE(LOG_STR("...javacall_file_rename para is null!"));
        return JAVACALL_FAIL;
    }
    if(MMIFILE_FILE_NAME_MAX_LEN < oldNameLen || MMIFILE_FILE_NAME_MAX_LEN < newNameLen){
        DEBUG_FILE(LOG_STR("...javacall_file_rename file name is too long!"));
        return JAVACALL_FAIL;
    }

    /* unicodeOldFilename �ļ������û��':\'·��,���Զ�����Java·��*/
    if(oldNameLen >= 3 && unicodeOldFilename[1] == MMIFILE_COLON && unicodeOldFilename[2] == MMIFILE_SLASH){
        MMIAPICOM_Wstrncpy(old_full_path,(const wchar*)unicodeOldFilename,MIN(MMIFILE_FULL_PATH_MAX_LEN,oldNameLen));
        old_path_len = oldNameLen;
    }else{//�ļ���δ��':\'
        if((javahome_path_len + oldNameLen+1) > MMIFILE_FULL_PATH_MAX_LEN){/*lint !e737 */
            DEBUG_FILE(LOG_STR("...javacall_file_rename Old file name length is too long!"));
                return JAVACALL_FAIL;
        }else{
            MMIAPIFMM_CombineFullPath(driver_name, driver_name_len,
				root_dir_name, root_dir_len, 
				(const wchar*)unicodeOldFilename, oldNameLen, 
				old_full_path, &old_path_len);
        }
    }

    //��鱻rename�Ķ����Ƿ�ΪĿ¼����֧��Ŀ¼��rename
    if(MMIAPIFMM_IsFolderExist(old_full_path,old_path_len)){
	DEBUG_FILE(LOG_STR("...it's directory, cannot be renamed!\n"));
	return JAVACALL_FAIL;
    }

    /* unicodeNewFilename �ļ������û��':\'·��,���Զ�����Java·��*/
    if(newNameLen >= 3 && unicodeNewFilename[1] == MMIFILE_COLON && unicodeNewFilename[2] == MMIFILE_SLASH){
        MMIAPICOM_Wstrncpy(new_full_path,(const wchar*)unicodeNewFilename,MIN(MMIFILE_FULL_PATH_MAX_LEN,newNameLen));
        new_path_len = newNameLen;
    }else{//�ļ���δ��':\'
        if((javahome_path_len + oldNameLen+1)  > MMIFILE_FULL_PATH_MAX_LEN){/*lint !e737 */
            DEBUG_FILE(LOG_STR("...javacall_file_rename New file name length is too long!"));
            return JAVACALL_FAIL;
        }else{
            MMIAPIFMM_CombineFullPath(driver_name, driver_name_len,
                		root_dir_name, root_dir_len, 
				(const wchar*)unicodeNewFilename, newNameLen, 
				new_full_path, &new_path_len);
        }
    }

    //�Ƚ��½��ļ����Ƿ���ͬ�������ͬ��ֱ�ӷ��سɹ�
    if(MMIAPICOM_Wstrcmp(old_full_path, new_full_path)==0){
        DEBUG_FILE(LOG_STR("...src file and dest file is same(OK)!\n"));
        return JAVACALL_OK;
    }

    //����ļ��Ƿ��Ѿ����ڣ���֧�ָ������������ʧ��
    if(MMIAPIFMM_IsFileExist(new_full_path, new_path_len)){
        DEBUG_FILE(LOG_STR_LEAVE("fail, File to be renamed already exists"));
        return JAVACALL_FAIL;
    }

    // When old and new files are in the same disk, use SFS_RenameFile directly
    if (!MMIAPICOM_Wstrncmp(old_full_path, new_full_path, 3)) {
        sfs_error = SFS_RenameFile(old_full_path, new_full_path, NULL);
    } else {
        MMIAPICOM_Wstrcpy(saved_path, old_full_path);
    
        //MMIAPIFMM_SplitLastName(old_full_path, &old_path_len, old_file_name, &old_file_len);
        // MMIAPIFMM_SplitLastName(new_full_path, &new_path_len, new_file_name, &new_file_len);
    
        sfs_error = MMIAPIFMM_CopyFile(old_full_path,new_full_path);

        //sfs_error = SFS_CopyFile(old_full_path, old_file_name, new_full_path, new_file_name, NULL);
        if (SFS_NO_ERROR == sfs_error)
        {
            DEBUG_FILE(LOG_STR_LEAVE("ok"));
            sfs_error = MMIAPIFMM_DeleteFile(saved_path, NULL);
        }
    }

    if (SFS_NO_ERROR == sfs_error){
        DEBUG_FILE(LOG_STR_LEAVE("ok"));
        return JAVACALL_OK;
    }else{
        DEBUG_FILE(LOG_STR_LEAVE("fail"));
        return JAVACALL_FAIL;
    }        

}


#ifdef __cplusplus
}
#endif
