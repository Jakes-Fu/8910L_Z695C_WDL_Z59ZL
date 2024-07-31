/*
 * Copyright 2003 Aplix Corporation. All Rights Reserved.
 *
 * JBlend and all Aplix- or JBlend-related trademarks and logos are
 * trademarks or registered trademarks of Aplix in the U.S., Japan and
 * other countries.
 */

#include "std_header.h"
#ifdef JAVA_SUPPORT_IA
#include "kjava_sys_pe.h"
#include "os_api.h"
#include "jwe_log.h"


/*-----------------------------------------------------------------------------
* Marcos, Types, and Structures
*----------------------------------------------------------------------------*/
#define ASSERT_LOG(_x)  JWE_LOG_ERR(assert,_x)

#ifndef _AJSC_DEBUG
#undef  JWE_LOG
#define JWE_LOG(function,parameters)
#endif
#ifndef WIN32
#define USE_KJAVA_SECTION_VM_RAM
#endif
/*-----------------------------------------------------------------------------
* Local variables
*----------------------------------------------------------------------------*/

static KJavaPESectionInfo peSectionInfo[1];


/*-----------------------------------------------------------------------------
* External Function Prototypes
*----------------------------------------------------------------------------*/
#ifdef USE_KJAVA_SECTION_VM_RAM
/*
* In scatter file, put jblend lib in specific ram part
* reference setting below.
*
* jblend_ram +0
* {
*    jblend_*.o  (+RW,+ZI)
* }
*
*/

typedef char GLOBALSYM[];
extern GLOBALSYM Image$$jblend_ram$$ZI$$Base,/*lint !e27*/
Image$$jblend_ram$$ZI$$Length;  /*lint !e27 !e19*/
extern GLOBALSYM Image$$jblend_ram$$Base,/*lint !e27 !e19*/
Load$$jblend_ram$$Base,/*lint !e27*/
Image$$jblend_ram$$Length;/*lint !e27 !e19*/

//***********************************************************************************************
//  Description:Gets section information
//  Author:huangjc
//  Date:2007-09-03
//  Note:
//***********************************************************************************************
const KJavaPESectionInfo *kjava_core_getSectionInfo(int *number_p)
{
   // KJavaPESectionInfo peSectionInfo[1] = {0};
    JWE_LOG(kjava_core_getSectionInfo, ("kjava_core_getSectionInfo -- enter!"));
    *number_p = 1;
    
    /* JKT_Section_Info creation */
    peSectionInfo[0].type           = (int)KJAVA_SECTION_VM_RAM;
    peSectionInfo[0].dataAddress    = (void*)Image$$jblend_ram$$Base;/*lint !e27*/
    peSectionInfo[0].dataSize       = (unsigned int)Image$$jblend_ram$$Length;/*lint !e27*/
    peSectionInfo[0].bssAddress     = (void*)Image$$jblend_ram$$ZI$$Base;/*lint !e27*/
    peSectionInfo[0].bssSize        = (unsigned int)Image$$jblend_ram$$ZI$$Length;/*lint !e27*/
    peSectionInfo[0].initDataAddress= (void*)Load$$jblend_ram$$Base;/*lint !e27*/
    
    JWE_LOG(kjava_core_getSectionInfo, 
        ("type = %d, dataAddress = 0x%08x, dataSize = 0x%08x",
        peSectionInfo[0].type, peSectionInfo[0].dataAddress, peSectionInfo[0].dataSize));
    
    JWE_LOG(kjava_core_getSectionInfo, ("kjava_core_getSectionInfo -- Leave!"));
    
    return peSectionInfo;
}

#else
/*-----------------------------------------------------------------------------
 * Include Files
 *----------------------------------------------------------------------------*/
#include <stddef.h>
#include <windows.h>
#include <string.h>

/*-----------------------------------------------------------------------------
 * Local variables
 *----------------------------------------------------------------------------*/
static unsigned long data_address;
static unsigned long data_size;
static unsigned long bss_address;
static unsigned long bss_size;
static void* initData_address;

static int peInitFlag = 0;

/* PE format of a self-module is analyzed and the information on specific section is acquired */
int PE_getSectionInfo(const char *sectionName, unsigned long* address, unsigned long* size)
{
    int ret = -1;
    char filename[MAX_PATH];
    HANDLE hFile;
    HANDLE hFileMapping;
    PIMAGE_DOS_HEADER dosHeader;
	HMODULE hdll;

    /* parameter check */
    if (sectionName == NULL) return -1;
    if (address == NULL) return -1;
    if (size == NULL) return -1;

    /* initialization processing */
	hdll = GetModuleHandle("msdevkernel.dll");
	if(hdll == NULL) return -1;
    /* self-file name is acquired */
    if (GetModuleFileName(hdll, filename, sizeof(filename)) == 0) {
        /* file name is not known */
        goto END_RETURN;
    }

    /* file open */
    if ((hFile = CreateFile(filename, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0)) == INVALID_HANDLE_VALUE) {
        /* file open failure */
        goto END_RETURN;
    }

    /* File memory map generation */
    if ((hFileMapping = CreateFileMapping(hFile, NULL, PAGE_READONLY, 0, 0, NULL)) == 0) {
        goto END_FILECLOSE;
    }

    /* view-sets up from a map */
    if ((dosHeader = (PIMAGE_DOS_HEADER)MapViewOfFile(hFileMapping, FILE_MAP_READ, 0, 0, 0)) == NULL ) {
        goto END_MAPCLOSE;
    }

    /* format check */
    {
        int i;
        PIMAGE_NT_HEADERS pNTHeader = (PIMAGE_NT_HEADERS)((DWORD)dosHeader + dosHeader->e_lfanew);
        PIMAGE_SECTION_HEADER pSectionHeader = (PIMAGE_SECTION_HEADER)(pNTHeader+1);

        for (i = 0; i < pNTHeader->FileHeader.NumberOfSections; i++) {
            if (strcmp((char *)pSectionHeader[i].Name, sectionName) == 0) {
                /* applicable section discovery */
                *address = pSectionHeader[i].VirtualAddress + (long)hdll;
                *size    = pSectionHeader[i].Misc.VirtualSize;
                ret = 0;
                break;
            }
        }
    }

    /* end processing */
/* END_VIEWCLOSE: */
    /* view release */
  UnmapViewOfFile(dosHeader);
END_MAPCLOSE:
    /* map release */
    CloseHandle(hFileMapping);
END_FILECLOSE:
    /* file release */
    CloseHandle(hFile);
END_RETURN:
    return ret;
}

/*-----------------------------------------------------------------------------
 * Global functions
 *----------------------------------------------------------------------------*/

/**
 *
 */
const KJavaPESectionInfo *kjava_core_getSectionInfo(int *number_p) {

    *number_p = 1;
    if (peInitFlag)
        return peSectionInfo;

    /* execution file is analyzed and data and bss information are acquired */
    PE_getSectionInfo(".vmdata", &data_address, &data_size);
    PE_getSectionInfo(".vmbss",  &bss_address,  &bss_size);

    /* Since it does not have the initial value of a data section on the memory, it creates and substitutes for a copy */
    initData_address = (void*)malloc(data_size);
    memcpy( initData_address, (const void*)data_address, data_size );

    /* JKT_Section_Info creation */
    peSectionInfo[0].type           = (int)KJAVA_SECTION_VM_RAM;
    peSectionInfo[0].dataAddress    = (void*)data_address;
    peSectionInfo[0].dataSize       = (unsigned int)data_size;
    peSectionInfo[0].bssAddress     = (void*)bss_address;
    peSectionInfo[0].bssSize        = (unsigned int)bss_size;
    peSectionInfo[0].initDataAddress= initData_address;

    peInitFlag = 1;
    return peSectionInfo;
}

#endif

#endif
