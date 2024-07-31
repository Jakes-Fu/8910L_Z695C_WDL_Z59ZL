/*****************************************************************************
** File Name:      youngtong_tts_interface.c                                       *
** Author:                                                                   *
** Date:           2007.5.8                                                  *
** Copyright:      All Rights Reserved.                                      *
** Description:    Youngtong tts                                             *
*****************************************************************************/


#include <string.h>
#include "sfs.h"
#include "mmifmm_export.h"
#include "socket_api.h"
#include "socket_types.h"
#include "mn_type.h"
#include "mmipdp_export.h"
#include "mmiconnection_export.h"

#include "yt_tts_socket_typedef.h"
#include "tts_nv.h"

#define TTS_PDP_RETRY_MAX (2)//(60)

#define TTS_SVR_INFO_FNAME_1  "svr_1.dat"
#define TTS_SVR_INFO_FNAME_2  "svr_2.dat"

#define TTS_SVR_INFO_FHANDLE_1 (0x43219901)
#define TTS_SVR_INFO_FHANDLE_2 (0x43219902)


LOCAL int32 s_tts_ser_info_handle_1 = SFS_INVALID_HANDLE;
LOCAL int32 s_tts_ser_info_handle_2 = SFS_INVALID_HANDLE;

LOCAL uint8 s_yttts_pdp_retry = 0;
LOCAL uint8 s_yttts_act_timer = 0;
LOCAL uint32 s_yttts_net_id = 0xFFFFFFFF;

static uint32 get_file_open_mode(const char *mode)
{
	uint32 opt = 0;

	SCI_ASSERT(PNULL != mode);/*assert verified*/

	if(0 == strcmp(mode, "r") || 0 == strcmp(mode, "rb"))
	{
		opt = SFS_MODE_OPEN_EXISTING | SFS_MODE_READ;
	}
	else if (0 == strcmp(mode, "r+"))
	{
		opt = SFS_MODE_OPEN_EXISTING | SFS_MODE_READ | SFS_MODE_WRITE;
	}
	else if (0 == strcmp(mode, "w") || 0 == strcmp(mode, "wb"))
	{
		opt = SFS_MODE_OPEN_ALWAYS | SFS_MODE_WRITE;
	}
	else if (0 == strcmp(mode, "w+"))
	{
		opt = SFS_MODE_CREATE_ALWAYS | SFS_MODE_READ | SFS_MODE_WRITE;
	}
	else if (0 == strcmp(mode, "a"))
	{
		opt = SFS_MODE_APPEND | SFS_MODE_WRITE;
	}
	else if (0 == strcmp(mode, "a+"))
	{
		opt = SFS_MODE_APPEND | SFS_MODE_WRITE | SFS_MODE_READ ;
	} 
	else if(0 == strcmp(mode, "rb+"))
	{
		opt = SFS_MODE_OPEN_ALWAYS|SFS_MODE_WRITE | SFS_MODE_READ ;
	}
	else
	{
		opt = SFS_MODE_OPEN_EXISTING | SFS_MODE_READ;
	}

	return opt;
}


int yt_fopen(char *strFileName, char *strMode)
{
	int i, retHandle = -1;
	unsigned short file_name_utf16[150];
	unsigned char charOne;
	SFS_HANDLE hHandle;

	if (strcmp(strFileName, TTS_SVR_INFO_FNAME_1) == 0)
	{
	    s_tts_ser_info_handle_1 = TTS_SVR_INFO_FHANDLE_1;
		SCI_TraceLow("[YT300] yt_fopen(open NV 1 OK) ");
		return s_tts_ser_info_handle_1;
	}
	else if (strcmp(strFileName, TTS_SVR_INFO_FNAME_2) == 0)
	{
	    s_tts_ser_info_handle_2 = TTS_SVR_INFO_FHANDLE_2;
		SCI_TraceLow("[YT300] yt_fopen(open NV 2 OK) ");
		return s_tts_ser_info_handle_2;
	}

	//we need to convert file name from ANSI to UTf16
	i = 0;
	while(1)
	{
		charOne = strFileName[i];
		file_name_utf16[i] = charOne;
		if('\0' == charOne)break;
		if(i >= 149) break;
		i ++;	
	}
	file_name_utf16[i] = '\0';

    SCI_TraceLow("[YT300] yt_fopen(strMode = %s, strFileName = %s) ", strMode, strFileName);

    // if C:\xxx can not open D: \XXX, E:\XXX

	for(file_name_utf16[0] = 'C'; file_name_utf16[0]<='E'; file_name_utf16[0]++)
	{
		hHandle = SFS_CreateFileInternal(file_name_utf16, get_file_open_mode(strMode), PNULL, PNULL);

		if(SFS_INVALID_HANDLE != hHandle)
			break;

		hHandle = -1;
	}
	SCI_TraceLow("[YT300] yt_fopen(return = %d)", hHandle);

	return (int)hHandle;
}

int yt_read_server_info(int id, void *buffer, size_t size)
{
    // id = 0, 1
    // size = 256
    // Read data buffer from nv
    MMITTS_NVGetData(id, buffer);
}

int yt_write_server_info(int id, void *buffer, size_t size)
{
    // id = 0, 1
    // size = 256
    
    // Write data buffer to nv
    MMITTS_NVSetData(id, buffer);
}

int yt_fread(void *buffer, size_t size, size_t count, int fid)
{
	uint32 bytes_read, bytes_to_read;
	SFS_HANDLE hHandle = SFS_INVALID_HANDLE;

	hHandle = (SFS_HANDLE)fid;
	bytes_to_read = (uint32)(size*count);
	
	if (hHandle == s_tts_ser_info_handle_1)
	{
	    yt_read_server_info(0, buffer, bytes_to_read);
		SCI_TraceLow("[YT300] yt_fread( NV 1 OK) ");
		return 0;
	}
	else if (hHandle == s_tts_ser_info_handle_2)
	{
		yt_read_server_info(1, buffer, bytes_to_read);
		SCI_TraceLow("[YT300] yt_fread( NV 2 OK) ");
		return 0;
	}


	if (SFS_INVALID_HANDLE!= hHandle)
	{
		if(SFS_ERROR_NONE == SFS_ReadFile(hHandle, buffer, bytes_to_read, &bytes_read, NULL))
		{
		    SCI_TraceLow("[YT300] yt_fread(OK, read byte = %d bytes_to_read = %d) ", bytes_read, bytes_to_read);

		    if (bytes_to_read != bytes_read)
		    {
		        return 0;
		    }
		    else
		    {
		        return bytes_read;
		    }
		}
		else
		{
		    SCI_TraceLow("[YT300] yt_fread(FAIL) ");
		    return 0;
		}
	}
	else
	{
		return 0;
	}
}

int yt_fwrite(void *buffer, size_t size, size_t count, int fid)
{
	uint32 bytes_write, bytes_to_write;
	SFS_HANDLE hHandle = SFS_INVALID_HANDLE;

	hHandle = (SFS_HANDLE)fid;
	bytes_to_write = (uint32)(size*count);
	if (hHandle == s_tts_ser_info_handle_1)
	{
	    yt_write_server_info(0, buffer, bytes_to_write);
	    SCI_TraceLow("[YT300] yt_fwrite( NV 1 OK) ");
	    return 0;
	}
	else if (hHandle == s_tts_ser_info_handle_2)
	{
	    yt_write_server_info(1, buffer, bytes_to_write);
	    SCI_TraceLow("[YT300] yt_fwrite( NV 2 OK) ");
	    return 0;
	}

	if (SFS_INVALID_HANDLE!= hHandle)
	{
	    if(SFS_ERROR_NONE == SFS_WriteFile(hHandle, buffer, bytes_to_write, &bytes_write, NULL))
	    {
	        SCI_TraceLow("[YT300] yt_fwrite(OK, bytes_write = %d bytes_to_write = %d) ", bytes_write, bytes_to_write);

	        if (bytes_write != bytes_to_write)
	        {
	            return 0;
	        }
	        else
	        {
	            return bytes_write;
	        }
	    }
	    else
	    {
	        SCI_TraceLow("[YT300] yt_fwrite(FAIL) ");

	        return 0;
	    }
	}
	else
	{
	    return 0;
	}

}

int yt_fseek(int fid, long offset, int origin)
{
    int nReturn = 0;

    nReturn = SFS_SetFilePointer(fid, offset, origin);
    SCI_TraceLow("[YT300] yt_fseek  fd = %x, offset = %d, origin = %d", fid, offset, origin);

    return nReturn;
}

int yt_fclose(int fid)
{
	SFS_HANDLE hHandle = (long)fid;

	if (hHandle == s_tts_ser_info_handle_1)
	{
	    s_tts_ser_info_handle_1 = SFS_INVALID_HANDLE;
		SCI_TraceLow("[YT300] yt_fclose(close NV 1 OK) ");
		return 0;
	}
	else if (hHandle == s_tts_ser_info_handle_2)
	{
	    s_tts_ser_info_handle_2 = SFS_INVALID_HANDLE;
		SCI_TraceLow("[YT300] yt_fclose(close NV 2 OK) ");
		return 0;
	}
	
	if(SFS_INVALID_HANDLE != hHandle)
	{
		SFS_CloseFile(hHandle);
		SCI_TraceLow("[YT300] yt_fclose(OK) ");
		return 0;
	}
	return -1;
}



/////////////////////////////////////////
/////////////////////////////////////////////

void yt_get_chip_info(char* pBufferForChipInfo, int *pInfoSize)
{
	uint8  tmp_chinfo[20];
	char   str_chinfo[20];
	int i;
    if(0 == EFUSE_HAL_ReadUid(tmp_chinfo))
    {
        for(i=0; i<8; i++)  // Convert hexadecimal numbers to strings
        { 
           str_chinfo[2*i] ='0' +  tmp_chinfo[i] /16;
           str_chinfo[2*i+1]  = '0' + tmp_chinfo[i] % 16;
	    if(str_chinfo[2*i] > '9')
	    {
	    	str_chinfo[2*i] = str_chinfo[2*i]  + 7;
	    }
	    if(str_chinfo[2*i+1]  >'9')
	    {
	    	str_chinfo[2*i+1] = str_chinfo[2*i+1]  + 7;
	    }
        }
	  str_chinfo[2*i]  = '\0';
	  memcpy(pBufferForChipInfo, str_chinfo, 17 * sizeof(char));
	  SCI_TraceLow("[YT300] yt_get_chip_info(uid = %s)", pBufferForChipInfo);
        *pInfoSize = 16;
    }
}

///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////

static char* get_host_addr_list[5];
static struct in_addr_yt get_host_in = {0};
static struct yt_hostent get_host_ret = {0};

//7 networking callback function API definitions --- [START]
static struct yt_hostent* yt_gethostbyname(const char *strHostName)
{
    struct sci_hostent *pSciHost = NULL;

    if (strHostName == NULL)
    {
        SCI_TraceLow("[YT300] yt_gethostbyname(strHostName == NULL)");
        return NULL;
    }

    memset(&get_host_in, 0, sizeof(struct in_addr_yt));

    pSciHost = sci_gethostbyname_ext(strHostName, s_yttts_net_id);

    if (pSciHost == NULL)
    {
        SCI_TraceLow("[YT300] yt_gethostbyname(sci return == NULL)");
        return NULL;
    }

    SCI_TraceLow("[YT300] yt_gethostbyname(%s) ip -- (%d.%d.%d.%d)", 
		strHostName, pSciHost->h_addr_list[0][0], pSciHost->h_addr_list[0][1], 
		pSciHost->h_addr_list[0][2], pSciHost->h_addr_list[0][3]);

//return (struct yt_hostent *)pSciHost;

    get_host_in.s_addr = (pSciHost->h_addr_list[0][3] << 24)
		|(pSciHost->h_addr_list[0][2] << 16) 
		|(pSciHost->h_addr_list[0][1] << 8) 
		|(pSciHost->h_addr_list[0][0]);

    get_host_ret.h_name = pSciHost->h_name;
    get_host_ret.h_aliases = pSciHost->h_aliases;
    get_host_ret.h_length = pSciHost->h_length;
    get_host_ret.h_addrtype = pSciHost->h_addrtype;
    get_host_ret.h_addr_list = &get_host_addr_list;
    get_host_ret.h_addr_list[0] = &get_host_in;
	
    return &get_host_ret; 
}

static int yt_socket(int af, int type, int protocol)
{
    int nRet = 0;

    nRet = sci_sock_socket(af, type, protocol, s_yttts_net_id);
    SCI_TraceLow("[YT300] yt_socket(af = %d, type =%d, protocal = %d, net id = %d) return %d", af, type, protocol, s_yttts_net_id, nRet);

    return nRet; 
}

static unsigned int yt_inet_addr(const char *strPtr)
{
    unsigned int nRet = 0;

    SCI_TraceLow("[YT300] yt_inet_addr(%s)", strPtr);

    sci_parse_host_ext(strPtr, &nRet, 0, s_yttts_net_id);

    SCI_TraceLow("[YT300] yt_inet_addr(%s) to ip-addr (%x)", strPtr, nRet);

    return nRet;
}

static int yt_connect(int s, struct yt_sockaddr *name, yt_socklen_t namelen)
{
    int nRet = 0;

    struct sci_sockaddr sock_addr = {0};
    struct yt_sockaddr_in *in = (struct yt_sockaddr_in *)name;

    SCI_TraceLow("[YT300] yt_connect(%x)", s);
    SCI_TraceLow("[YT300] yt_connect(name->sin_len = %d)", in->sin_len);
    SCI_TraceLow("[YT300] yt_connect(name->sin_family = %d)", in->sin_family);
    SCI_TraceLow("[YT300] yt_connect(name->sin_port = %d)", in->sin_port);
    SCI_TraceLow("[YT300] yt_connect(name->sin_addr = %x)", in->sin_addr.s_addr);

    sock_addr.family = in->sin_family;
    //sock_addr.port = htons(5000/*in->sin_port*/);
    sock_addr.port = in->sin_port;
    sock_addr.ip_addr = in->sin_addr.s_addr;

    memcpy(&sock_addr.sa_data, in->sin_zero, 8);
    nRet = sci_sock_connect(s, &sock_addr, sizeof(sock_addr));

    return nRet; 
}

static int yt_send(int s, const void *dataptr, size_t size, int nTimeout)
{
    int nRet = 0;

    
    SCI_TraceLow("[YT300] yt_send(%x),nTimeout=%d", s,nTimeout);

    nRet = sci_sock_send(s, dataptr, size, 0);

    return nRet;
}

#if 1
#if 1
static int yt_recv(int s, void *mem, size_t len, int nTimeout)
{
    struct timeval {
        long    tv_sec;         /* seconds */
        long    tv_usec;        /* and microseconds */
    } timearg = {0};
    int nRet = 0;
    int nErr = 0;
    int nRetry = 0;
 
    timearg.tv_sec = 0;
    timearg.tv_usec = 5000;
 
    sci_sock_setsockopt(s, SO_BIO, NULL);
    sci_sock_setsockopt(s, SO_RCVTIMEO, &timearg);
 
    nRetry = 100;
    do
    {
        nRet = sci_sock_recv(s, mem, len, 0);
        nErr = sci_sock_errno(s);
         nRetry--;
         SCI_TraceLow("[YT300] yt_recv (return = %d, err = %d, retry = %d) ", nRet, nErr, nRetry);
         SCI_Sleep(10);
    }
    while (nRet == -1 && nErr == EWOULDBLOCK && nRetry > 0);
 
    return nRet;
}

#else
static int yt_recv(int s, void *mem, size_t len, int nTimeout)
{
    int nErr = 0;
    int nRet = 0;
    struct timeval {
        long    tv_sec;         /* seconds */
        long    tv_usec;        /* and microseconds */
    } timearg = {0};

    timearg.tv_sec = 0;
    timearg.tv_usec = 5000;

    // set recv timeout 5ms
    sci_sock_setsockopt(s, SO_RCVTIMEO, &timearg);
    
    SCI_TraceLow("[YT300] yt_recv(%x)", s);
    nRet = sci_sock_recv(s, mem, len, 0);

    nErr = sci_sock_errno(s);
    SCI_TraceLow("[YT300] yt_recv(return = %d, err = %d)", nRet,nErr);
    #if 1
    if (nRet < 0) 
        nRet = 0;
    #endif
    return nRet; 
}
#endif
#else
static int yt_recv(int s, void *mem, size_t len, int nTimeout)
{
    int nRet = 0;
    
    SCI_TraceLow("[YT300] yt_recv(%x),nTimeout=%d", s,nTimeout);
    nRet = sci_sock_recv(s, mem, len, 0);
    
    return nRet; 
}
#endif

static int yt_closesocket(int s)
{
    int nRet = 0;

    SCI_TraceLow("[YT300] yt_closesocket (%x)", s);
    nRet = sci_sock_shutdown(s, SD_BOTH) ;

    return nRet; 
}


LOCAL void yt_print(char *p_text, unsigned int *p_timeSnapshot)
{
    SCI_TraceLow("[YT LIB LOG]: %s", p_text);
}

int yt_mocor_porting_init(void)
{
    //set 4 callback functions for File I/O
    yt_tts_set_cb_fopen(yt_fopen);
    yt_tts_set_cb_fread(yt_fread);
    yt_tts_set_cb_fwrite(yt_fwrite);
    yt_tts_set_cb_fclose(yt_fclose);
    yt_tts_set_cb_fseek(yt_fseek);


    //set 2 file names in ANSI encoding
    yt_tts_set_file_one(TTS_SVR_INFO_FNAME_1);
    yt_tts_set_file_two(TTS_SVR_INFO_FNAME_2);


    //set 7 callback functions for networking
    yt_tts_set_cb_gethostbyname(yt_gethostbyname);
    yt_tts_set_cb_socket(yt_socket);
    yt_tts_set_cb_inet_addr(yt_inet_addr);
    yt_tts_set_cb_connect(yt_connect);
    yt_tts_set_cb_send(yt_send);
    yt_tts_set_cb_recv(yt_recv);
    yt_tts_set_cb_closesocket(yt_closesocket);


    //set callback function for getting chip ID...
    yt_tts_set_cb_getchipinfo(yt_get_chip_info);

    //set callback function for printing trace 
    //yt_tts_set_cb_print_info_300(yt_print);
}

//////////////////////////////////////////////////

static void yt_start_pdp(void);

static void yt_tts_pdp_handle(MMIPDP_CNF_INFO_T *msg_ptr)
{
    if(PNULL == msg_ptr || msg_ptr->app_handler != MMI_MODULE_TTS)
    {
        return;
    }
    SCI_TraceLow("[MMITTS] HandleTTSPdpMsg -- msg = %d", msg_ptr->msg_id);

    switch(msg_ptr->msg_id) 
    {
        case MMIPDP_ACTIVE_CNF: 
            SCI_TraceLow("[MMITTS] MMIPDP_ACTIVE_CNF--- result = %d", msg_ptr->result);
            if(MMIPDP_RESULT_SUCC == msg_ptr->result)
            {
                SCI_TraceLow("[MMITTS] MMIPDP_ACTIVE_CNF--- net id  = %d", msg_ptr->nsapi);
                s_yttts_net_id = msg_ptr->nsapi;
#if 1
                #if !defined(WIN32)
                yt_tts_start_activation_300();
                SCI_TraceLow("[MMITTS] yt_tts_start_activation_300--- after (%d)", yt_tts_is_active_300());
                #endif
                MMIAPIPDP_Deactive(MMI_MODULE_TTS);
#else
                TTSAPI_Activation();
#endif
            }
            else if (0 != yt_tts_is_active_300() && s_yttts_pdp_retry < TTS_PDP_RETRY_MAX)
            {
                MMK_StopTimer(s_yttts_act_timer);
                s_yttts_act_timer = MMK_CreateTimerCallback(100*1000, yt_start_pdp, PNULL, FALSE);
            }

            break;

        case MMIPDP_DEACTIVE_CNF:
            SCI_TraceLow("[MMITTS] MMIPDP_DEACTIVE_CNF");
            s_yttts_net_id = 0;
            if (0 != yt_tts_is_active_300() && s_yttts_pdp_retry < TTS_PDP_RETRY_MAX)
            {
                MMK_StopTimer(s_yttts_act_timer);
                s_yttts_act_timer = MMK_CreateTimerCallback(100*1000, yt_start_pdp, PNULL, FALSE);
            }
            break;

        case MMIPDP_DEACTIVE_IND:
            SCI_TraceLow("[MMITTS] MMIPDP_DEACTIVE_IND");
            s_yttts_net_id = 0;
            MMIAPIPDP_Deactive(MMI_MODULE_TTS);
            break;	        

        default:
            break;
    }
}

static void yt_start_pdp(void)
{
    MMIPDP_ACTIVE_INFO_T    active_info     = {0};
    MMICONNECTION_LINKSETTING_DETAIL_T* linksetting = PNULL;
    char*   apn_str = PNULL;
    MN_DUAL_SYS_E sim_sys = 0;
    SCI_TraceLow("[MMITTS] yt_start_pdp");
    s_yttts_pdp_retry ++; //TERENCE---2020/09/14: we need to increase s_yttts_pdp_retry here
    if (FALSE == MMIAPIPHONE_GetDataServiceSIM(&sim_sys))
    {
        SCI_TraceLow("[MMITTS] yt_active_pdp [SIM ERROR]");
        goto retry;    
    }
    linksetting = MMIAPICONNECTION_GetLinkSettingItemByIndex(sim_sys, 0);
    if(PNULL != linksetting && 0 != linksetting->apn_len)
    {
        apn_str = (char*)linksetting->apn;
    }
    active_info.app_handler         = MMI_MODULE_TTS;
    active_info.dual_sys            = sim_sys;    
    active_info.apn_ptr             = apn_str;    
    active_info.user_name_ptr       = NULL;    
    active_info.psw_ptr             = NULL;    
    active_info.priority            = 3;    
    active_info.ps_service_rat      = MN_UNSPECIFIED;    
    active_info.ps_interface        = MMIPDP_INTERFACE_GPRS;    
    active_info.handle_msg_callback = yt_tts_pdp_handle;    
    active_info.ps_service_type = STREAMING_E/*BROWSER_E*/;    
    active_info.storage = MN_GPRS_STORAGE_ALL; 
#ifdef IPVERSION_SUPPORT_V4_V6
    active_info.ip_type   = linksetting->ip_type;
#endif

    if (MMIAPIPDP_Active(&active_info))
        return;

retry:
    if (s_yttts_pdp_retry < TTS_PDP_RETRY_MAX)
    {
        MMK_StopTimer(s_yttts_act_timer);
        s_yttts_act_timer = MMK_CreateTimerCallback(10*1000, yt_start_pdp, PNULL, FALSE);
    }
    return;
}


void yt_mocor_init (void)
{
    SCI_TraceLow("[MMITTS] yt_mocor_init.");

    yt_mocor_porting_init();

    if (0 == yt_tts_is_active_300())
    {
        SCI_TraceLow("[MMITTS] yt_mocor_init -- actived --.");
        return;
    }

    MMK_StopTimer(s_yttts_act_timer);
    s_yttts_pdp_retry = 0;
    s_yttts_act_timer = MMK_CreateTimerCallback(10*1000, yt_start_pdp, PNULL, FALSE);
    SCI_TraceLow("[MMITTS] yt_mocor_init -- not actived -- to active . timer id : %d", s_yttts_act_timer);
}

#if defined(WIN32)
int yt_tts_start_activation_300(void)
{}
#endif
