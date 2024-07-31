#ifdef FOTA_SUPPORT

#include "mmi_module.h"
#include "mmi_base_common.h"
#include "mmi_filemgr.h"
#include "mem_prod.h"
#include "fota_update.h"

#define ERASE_BLOCK_SIZE (64 * 1024)

typedef struct 
{
    char version[16];
    int size;
} APN_HAEDER;

PUBLIC BOOLEAN MMIFOTA_CheckApn(void)
{
    BOOLEAN ret = TRUE;
    int len;
    wchar f_buff[128];
    APN_HAEDER _header;
    MMIFILE_HANDLE fh;
    MMIFILE_ERROR_E e;

    MMIAPICOM_Wstrcpy(f_buff, Fota_GetUpdatePacPath());
    MMIAPICOM_Wstrcat(f_buff, Fota_GetApnFileName());

    if (!MMIAPIFMM_IsFileExist(f_buff, MMIAPICOM_Wstrlen(f_buff)))
    {
        SCI_TraceLow("[Fota Update] apn file not found");
        return FALSE;
    }

    fh = MMIAPIFMM_CreateFile(f_buff, SFS_MODE_READ | SFS_MODE_OPEN_EXISTING, NULL, NULL);
    if (fh == SFS_INVALID_HANDLE)
    {
        SCI_TraceLow("[Fota Update] apn file open error");
        return FALSE;
    }

    if (0 != MMIAPIFMM_ReadFile(fh, &_header, sizeof(APN_HAEDER), &len, NULL) && len != sizeof(APN_HAEDER))
    {
        SCI_TraceLow("[Fota Update] apn file read header error");
        ret = FALSE;
    }
    else
    {
        SCI_TraceLow("[Fota Update] apn file read version [%s]", _header.version);
        SCI_TraceLow("[Fota Update] apn file read size [%d]", _header.size);
	 ret = TRUE;
    }

    MMIAPIFMM_CloseFile(fh);
    return ret;
}

PUBLIC void MMIFOTA_UpdateApn(void)
{
    unsigned int len, i;
    unsigned char *addr, *p_buff;
    wchar f_buff[128];
    MMIFILE_HANDLE fh;
    MMIFILE_ERROR_E e;

    MMIAPICOM_Wstrcpy(f_buff, Fota_GetUpdatePacPath());
    MMIAPICOM_Wstrcat(f_buff, Fota_GetApnFileName());

    if (!MMIAPIFMM_IsFileExist(f_buff, MMIAPICOM_Wstrlen(f_buff)))
    {
        SCI_TraceLow("[Fota Update] apn file not found");
        return;
    }

    len = PROD_GetApnMemSection(&addr);
    if ((((uint32)addr) % ERASE_BLOCK_SIZE) != 0 || len == 0 || (len % ERASE_BLOCK_SIZE) != 0)
    {
        SCI_TraceLow("[Fota Update] apn section error [addr = 0x%x len = %d]", addr, len);
        return;
    }

    fh = MMIAPIFMM_CreateFile(f_buff, SFS_MODE_READ | SFS_MODE_OPEN_EXISTING, NULL, NULL);
    if (fh == SFS_INVALID_HANDLE)
    {
        SCI_TraceLow("[Fota Update] apn file open error");
        return;
    }
    SCI_TraceLow("[Fota Update] apn update flash [0x%x, %d]", addr, len);

    for (i = 0; i < (len + ERASE_BLOCK_SIZE - 1) / ERASE_BLOCK_SIZE; i++)
    {
        FLASH_Erase(addr + i * ERASE_BLOCK_SIZE);
    }

    p_buff = SCI_ALLOC_APPZ(ERASE_BLOCK_SIZE);

    while(0 == MMIAPIFMM_ReadFile(fh, p_buff, ERASE_BLOCK_SIZE, &len, NULL) && len > 0)
    {
	 FLASH_Write(addr, p_buff, len);
	 addr += len;
    }

    MMIAPIFMM_CloseFile(fh);
    SCI_FREE(p_buff);

    SCI_TraceLow("[Fota Update] apn update finished !!!");
}

PUBLIC BOOLEAN MMIFOTA_CheckConfigNV(void)
{
    BOOLEAN ret = TRUE;
    wchar f_buff[128];

    MMIAPICOM_Wstrcpy(f_buff, Fota_GetUpdatePacPath());
    MMIAPICOM_Wstrcat(f_buff, Fota_GetCnvFileName());

    if (!MMIAPIFMM_IsFileExist(f_buff, MMIAPICOM_Wstrlen(f_buff)))
    {
        SCI_TraceLow("[Fota Update] config nv file not found");
        return FALSE;
    }

    //TODO

    return ret;
}

PUBLIC void MMIFOTA_UpdateConfigNV(void)
{
    unsigned int len, i;
    unsigned char *addr, *p_buff;
    wchar f_buff[128];
    MMIFILE_HANDLE fh;
    MMIFILE_ERROR_E e;
    BOOLEAN ret;

    MMIAPICOM_Wstrcpy(f_buff, Fota_GetUpdatePacPath());
    MMIAPICOM_Wstrcat(f_buff, Fota_GetCnvFileName());

    if (!MMIAPIFMM_IsFileExist(f_buff, MMIAPICOM_Wstrlen(f_buff)))
    {
        SCI_TraceLow("[Fota Update] config nv file not found");
        return;
    }

    len = PROD_GetConfigNVMemSection(&addr);
    if ((((uint32)addr) % ERASE_BLOCK_SIZE) != 0 || len == 0 || (len % ERASE_BLOCK_SIZE) != 0)
    {
        SCI_TraceLow("[Fota Update] config nv section error [addr = 0x%x len = %d]", addr, len);
        return;
    }

    fh = MMIAPIFMM_CreateFile(f_buff, SFS_MODE_READ | SFS_MODE_OPEN_EXISTING, NULL, NULL);
    if (fh == SFS_INVALID_HANDLE)
    {
        SCI_TraceLow("[Fota Update] config nv file open error");
        return;
    }
    SCI_TraceLow("[Fota Update] config nv update flash [0x%x, %d]", addr, len);

    for (i = 0; i < (len + ERASE_BLOCK_SIZE - 1) / ERASE_BLOCK_SIZE; i++)
    {
        ret = FLASH_Erase(addr + i * ERASE_BLOCK_SIZE);
	 SCI_TraceLow("[Fota Update] cnv FLASH_Erase [0x%x] -- %d", addr + i * ERASE_BLOCK_SIZE, ret);
    }

    p_buff = SCI_ALLOC_APPZ(ERASE_BLOCK_SIZE);

    while(0 == MMIAPIFMM_ReadFile(fh, p_buff, ERASE_BLOCK_SIZE, &len, NULL) && len > 0)
    {
	 ret = FLASH_Write(addr, p_buff, len);
	 SCI_TraceLow("[Fota Update] cnv FLASH_Write [0x%x, %d] -- %d", addr,  len, ret);
	 addr += len;
    }

    MMIAPIFMM_CloseFile(fh);
    SCI_FREE(p_buff);

    SCI_TraceLow("[Fota Update] config nv update finished !!!");
}

PUBLIC BOOLEAN MMIFOTA_CheckIMS(void)
{
    BOOLEAN ret = TRUE;
    wchar f_buff[128];

    MMIAPICOM_Wstrcpy(f_buff, Fota_GetUpdatePacPath());
    MMIAPICOM_Wstrcat(f_buff, Fota_GetImsFileName());

    if (!MMIAPIFMM_IsFileExist(f_buff, MMIAPICOM_Wstrlen(f_buff)))
    {
        SCI_TraceLow("[Fota Update] ims file not found");
        return FALSE;
    }

    //TODO

    return ret;
}

PUBLIC void MMIFOTA_UpdateIMS(void)
{
    unsigned int len, i;
    unsigned char *addr, *p_buff;
    wchar f_buff[128];
    MMIFILE_HANDLE fh;
    MMIFILE_ERROR_E e;

    MMIAPICOM_Wstrcpy(f_buff, Fota_GetUpdatePacPath());
    MMIAPICOM_Wstrcat(f_buff, Fota_GetImsFileName());

    if (!MMIAPIFMM_IsFileExist(f_buff, MMIAPICOM_Wstrlen(f_buff)))
    {
        SCI_TraceLow("[Fota Update] ims file not found");
        return;
    }

    len = PROD_GetImsMemSection(&addr);
    if ((((uint32)addr) % ERASE_BLOCK_SIZE) != 0 || len == 0 || (len % ERASE_BLOCK_SIZE) != 0)
    {
        SCI_TraceLow("[Fota Update] ims section error [addr = 0x%x len = %d]", addr, len);
        return;
    }

    fh = MMIAPIFMM_CreateFile(f_buff, SFS_MODE_READ | SFS_MODE_OPEN_EXISTING, NULL, NULL);
    if (fh == SFS_INVALID_HANDLE)
    {
        SCI_TraceLow("[Fota Update] ims file open error");
        return;
    }
    SCI_TraceLow("[Fota Update] ims update flash [0x%x, %d]", addr, len);

    for (i = 0; i < (len + ERASE_BLOCK_SIZE - 1) / ERASE_BLOCK_SIZE; i++)
    {
        FLASH_Erase(addr + i * ERASE_BLOCK_SIZE);
    }

    p_buff = SCI_ALLOC_APPZ(ERASE_BLOCK_SIZE);

    while(0 == MMIAPIFMM_ReadFile(fh, p_buff, ERASE_BLOCK_SIZE, &len, NULL) && len > 0)
    {
	 FLASH_Write(addr, p_buff, len);
	 addr += len;
    }

    MMIAPIFMM_CloseFile(fh);
    SCI_FREE(p_buff);

    SCI_TraceLow("[Fota Update] ims update finished !!!");
}

PUBLIC BOOLEAN MMIFOTA_CheckToggle(void)
{
    BOOLEAN ret = TRUE;
    wchar f_buff[128];

    MMIAPICOM_Wstrcpy(f_buff, Fota_GetUpdatePacPath());
    MMIAPICOM_Wstrcat(f_buff, Fota_GetToggleFileName());

    if (!MMIAPIFMM_IsFileExist(f_buff, MMIAPICOM_Wstrlen(f_buff)))
    {
        SCI_TraceLow("[Fota Update] ims toggle file not found");
        return FALSE;
    }

    //TODO

    return ret;
}

PUBLIC void MMIFOTA_UpdateToggle(void)
{
    unsigned int len, i;
    unsigned char *addr, *p_buff;
    wchar f_buff[128];
    MMIFILE_HANDLE fh;
    MMIFILE_ERROR_E e;

    MMIAPICOM_Wstrcpy(f_buff, Fota_GetUpdatePacPath());
    MMIAPICOM_Wstrcat(f_buff, Fota_GetToggleFileName());

    if (!MMIAPIFMM_IsFileExist(f_buff, MMIAPICOM_Wstrlen(f_buff)))
    {
        SCI_TraceLow("[Fota Update] ims toggle file not found");
        return;
    }

    len = PROD_GetImsToggleMemSection(&addr);
    if ((((uint32)addr) % ERASE_BLOCK_SIZE) != 0 || len == 0 || (len % ERASE_BLOCK_SIZE) != 0)
    {
        SCI_TraceLow("[Fota Update] ims toggle section error [addr = 0x%x len = %d]", addr, len);
        return;
    }

    fh = MMIAPIFMM_CreateFile(f_buff, SFS_MODE_READ | SFS_MODE_OPEN_EXISTING, NULL, NULL);
    if (fh == SFS_INVALID_HANDLE)
    {
        SCI_TraceLow("[Fota Update] imstoggle  file open error");
        return;
    }
    SCI_TraceLow("[Fota Update] ims toggle update flash [0x%x, %d]", addr, len);

    for (i = 0; i < (len + ERASE_BLOCK_SIZE - 1) / ERASE_BLOCK_SIZE; i++)
    {
        FLASH_Erase(addr + i * ERASE_BLOCK_SIZE);
    }

    p_buff = SCI_ALLOC_APPZ(ERASE_BLOCK_SIZE);

    while(0 == MMIAPIFMM_ReadFile(fh, p_buff, ERASE_BLOCK_SIZE, &len, NULL) && len > 0)
    {
	 FLASH_Write(addr, p_buff, len);
	 addr += len;
    }

    MMIAPIFMM_CloseFile(fh);
    SCI_FREE(p_buff);

    SCI_TraceLow("[Fota Update] ims toggle update finished !!!");
}

#endif

