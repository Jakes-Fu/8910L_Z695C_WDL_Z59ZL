/**
 * @addtogroup platform_src_sirf_pal 
 * @{
 */

/*
 *                   SiRF Technology, Inc. GPS Software
 *
 *    Copyright (c) 2005-2009 by SiRF Technology, Inc.  All rights reserved.
 *
 *    This Software is protected by United States copyright laws and
 *    international treaties.  You may not reverse engineer, decompile
 *    or disassemble this Software.
 *
 *    WARNING:
 *    This Software contains SiRF Technology Inc.’s confidential and
 *    proprietary information. UNAUTHORIZED COPYING, USE, DISTRIBUTION,
 *    PUBLICATION, TRANSFER, SALE, RENTAL OR DISCLOSURE IS PROHIBITED
 *    AND MAY RESULT IN SERIOUS LEGAL CONSEQUENCES.  Do not copy this
 *    Software without SiRF Technology, Inc.’s  express written
 *    permission.   Use of any portion of the contents of this Software
 *    is subject to and restricted by your signed written agreement with
 *    SiRF Technology, Inc.
 *
 */

/**
 * @file   sirf_pal_tcpip.c
 *
 * @brief  SiRF PAL networking
 *
 */

#include "ms_customize_trc.h"
#include "sirf_errors.h"
#include "sirf_types.h"
#include "sirf_pal_tcpip.h"
#include "sirf_pal_os_thread.h"

#include "socket_api.h"
#include "ssl_api.h"

#include "csr_util.h"

/* Enter C naming convention */
#ifdef __cplusplus
extern "C" {
#endif


static tSIRF_INT32 SIRF_Security_Send_Data(void *ssl_app_ptr, tSIRF_UINT8 *data_ptr, tSIRF_UINT32 data_len);

static void SIRF_Security_Receive_Data(void *ssl_app_ptr, tSIRF_UINT8 *data_ptr, tSIRF_UINT32 data_len);

static void SIRF_Security_Post_Message(void *ssl_app_ptr, tSIRF_UINT32 msg_id);
	
static void SIRF_Security_Show_Cert_Info(void *ssl_app_ptr);

#define WAIT_FOR_SSL_ENCRYPTION_CSR                   (100)              /*in milliseconds*/
#define MAX_ENCRYPTED_DATA_BUFFER_SIZE            (8 * 1024)

#define WAIT_FOR_SSL_DECRYPTION_CSR                   (100)              /*in milliseconds*/
#define MAX_DECRYPTED_DATA_BUFFER_SIZE            (8 * 1024)

static int socket_handle_csr = -1;
static tSIRF_UINT32 socket_type_csr = (tSIRF_UINT32)SIRF_PAL_NET_SOCKET_TYPE_DEFAULT;
static tSIRF_UINT8 security_csr = FALSE;
static SSL_HANDLE ssl_handle_csr = 0;
static SSL_CALLBACK_T ssl_cb_csr = {SIRF_Security_Post_Message, SIRF_Security_Receive_Data, SIRF_Security_Send_Data, SIRF_Security_Show_Cert_Info};

static tSIRF_UINT32 original_data_len = 0;
volatile tSIRF_UINT8 encrypted_csr = FALSE;
static tSIRF_UINT8 encrypted_data_buf[MAX_ENCRYPTED_DATA_BUFFER_SIZE];
static tSIRF_UINT32 encrypted_data_len = 0;
static tSIRF_UINT32 encrypted_data_len_written = 0;

static tSIRF_UINT32 decrypted_csr = FALSE;
static tSIRF_UINT8 decrypted_data_buf[MAX_DECRYPTED_DATA_BUFFER_SIZE];
static tSIRF_UINT32 decrypted_data_len = 0;
static tSIRF_UINT32 handShaked_csr = FALSE;

static tSIRF_INT32 SIRF_Security_Send_Data(void *ssl_app_ptr, tSIRF_UINT8 *data_ptr, tSIRF_UINT32 data_len)
{
	if((data_len == 0) || (data_ptr == NULL))
	{
		//SCI_TRACE_LOW:"CSR Andy SIRF_Security_Send_Data Error data_len %d, data_ptr 0x%x"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,SIRF_PAL_TCPIP_81_112_2_18_0_33_19_1627,(uint8*)"dd", data_len, data_ptr);
	}
	else
	{
		if(data_len < MAX_ENCRYPTED_DATA_BUFFER_SIZE)
		{
			//SCI_TRACE_LOW:"CSR Andy SIRF_Security_Send_Data data_len %d"
			SCI_TRACE_ID(TRACE_TOOL_CONVERT,SIRF_PAL_TCPIP_87_112_2_18_0_33_19_1628,(uint8*)"d", data_len);

			if(original_data_len == 0)
			{
				uint32 ret = 0;
				uint32 ret_recv = 0;
				
				//SCI_TRACE_LOW:"CSR Andy SIRF_Security_Send_Data sci_sock_send"
				SCI_TRACE_ID(TRACE_TOOL_CONVERT,SIRF_PAL_TCPIP_94_112_2_18_0_33_19_1629,(uint8*)"");
				ret = sci_sock_send(socket_handle_csr, (char*)data_ptr, data_len, 0);

				if(ret > 0)
				{
					//SCI_TRACE_LOW:"CSR Andy SIRF_Security_Send_Data sci_sock_send successfully with bytes %d"
					SCI_TRACE_ID(TRACE_TOOL_CONVERT,SIRF_PAL_TCPIP_99_112_2_18_0_33_19_1630,(uint8*)"d", ret);

					if(ret !=  data_len)
					{
						//SCI_TRACE_LOW:"CSR Andy SIRF_Security_Send_Data Warning sci_sock_send data not all sent"
						SCI_TRACE_ID(TRACE_TOOL_CONVERT,SIRF_PAL_TCPIP_103_112_2_18_0_33_19_1631,(uint8*)"");
					}
					// success

					SIRF_PAL_OS_THREAD_Sleep(2000);
					
					if(socket_type_csr == SIRF_PAL_NET_SOCKET_TYPE_DEFAULT)
					{
						tSIRF_UINT32 readset[2]  = {SIRF_PAL_NET_INVALID_SOCKET, SIRF_PAL_NET_INVALID_SOCKET};
					
						readset[0] = socket_handle_csr;

						//SCI_TRACE_LOW:"CSR Andy SIRF_Security_Send_Data SIRF_PAL_NET_Select"
						SCI_TRACE_ID(TRACE_TOOL_CONVERT,SIRF_PAL_TCPIP_115_112_2_18_0_33_19_1632,(uint8*)"");
						
						if(SIRF_SUCCESS != SIRF_PAL_NET_Select(readset, NULL, NULL, 60000))
						{
							//SCI_TRACE_LOW:"CSR Andy SIRF_Security_Send_Data Error SIRF_PAL_NET_Select"
							SCI_TRACE_ID(TRACE_TOOL_CONVERT,SIRF_PAL_TCPIP_119_112_2_18_0_33_19_1633,(uint8*)"");
							SSL_AsyncMessageProc(ssl_handle_csr, SSL_RECV_MESSAGE_SEND_SUCC, data_len);
							return data_len;
						}
					}
					
					ret_recv = sci_sock_recv(socket_handle_csr, (char*)decrypted_data_buf, sizeof(decrypted_data_buf), 0);

					if(ret_recv > 0)
					{
						//SCI_TRACE_LOW:"CSR Andy SIRF_Security_Send_Data sci_sock_recv data with bytes %d"
						SCI_TRACE_ID(TRACE_TOOL_CONVERT,SIRF_PAL_TCPIP_129_112_2_18_0_33_19_1634,(uint8*)"d", ret_recv);
						//SCI_TRACE_LOW:"CSR Andy SIRF_Security_Send_Data SSL_DecryptPasser Start"
						SCI_TRACE_ID(TRACE_TOOL_CONVERT,SIRF_PAL_TCPIP_130_112_2_18_0_33_19_1635,(uint8*)"");
						SSL_DecryptPasser(ssl_handle_csr, decrypted_data_buf, ret_recv);
						//SCI_TRACE_LOW:"CSR Andy SIRF_Security_Send_Data SSL_DecryptPasser End"
						SCI_TRACE_ID(TRACE_TOOL_CONVERT,SIRF_PAL_TCPIP_132_112_2_18_0_33_19_1636,(uint8*)"");
					}
					else if(ret_recv == 0)
					{
						//SCI_TRACE_LOW:"CSR Andy SIRF_Security_Send_Data Error sci_sock_recv"
						SCI_TRACE_ID(TRACE_TOOL_CONVERT,SIRF_PAL_TCPIP_136_112_2_18_0_33_19_1637,(uint8*)"");
					}
					else
					{
						int err = sci_sock_errno(socket_handle_csr);
					
						//SCI_TRACE_LOW:"CSR Andy SIRF_Security_Send_Data sci_sock_errno for sci_sock_recv"
						SCI_TRACE_ID(TRACE_TOOL_CONVERT,SIRF_PAL_TCPIP_142_112_2_18_0_33_19_1638,(uint8*)"");
					
						if(EWOULDBLOCK == err)
						{
							//SCI_TRACE_LOW:"CSR Andy SIRF_Security_Send_Data sci_sock_errno Pending for sci_sock_recv"
							SCI_TRACE_ID(TRACE_TOOL_CONVERT,SIRF_PAL_TCPIP_146_112_2_18_0_33_19_1639,(uint8*)"");
						}
						else
						{
							//SCI_TRACE_LOW:"CSR Andy SIRF_Security_Send_Data Error sci_sock_errno err %d for sci_sock_recv"
							SCI_TRACE_ID(TRACE_TOOL_CONVERT,SIRF_PAL_TCPIP_150_112_2_18_0_33_19_1640,(uint8*)"d", err);
						}
					}	
				}
				else if(0 == ret)
				{
					//SCI_TRACE_LOW:"CSR Andy SIRF_Security_Send_Data Error sci_sock_send"
					SCI_TRACE_ID(TRACE_TOOL_CONVERT,SIRF_PAL_TCPIP_156_112_2_18_0_33_19_1641,(uint8*)"");
				}
				else
				{
					int err = sci_sock_errno(socket_handle_csr);
					
					//SCI_TRACE_LOW:"CSR Andy SIRF_Security_Send_Data sci_sock_errno for sci_sock_send"
					SCI_TRACE_ID(TRACE_TOOL_CONVERT,SIRF_PAL_TCPIP_162_112_2_18_0_33_19_1642,(uint8*)"");
					
					if(EWOULDBLOCK == err)
					{
						//SCI_TRACE_LOW:"CSR Andy SIRF_Security_Send_Data sci_sock_errno Pending for sci_sock_send"
						SCI_TRACE_ID(TRACE_TOOL_CONVERT,SIRF_PAL_TCPIP_166_112_2_18_0_33_19_1643,(uint8*)"");
					}
					else
					{
						//SCI_TRACE_LOW:"CSR Andy SIRF_Security_Send_Data Error sci_sock_errno err %d for sci_sock_send"
						SCI_TRACE_ID(TRACE_TOOL_CONVERT,SIRF_PAL_TCPIP_170_112_2_18_0_33_19_1644,(uint8*)"d", err);
					}
				}
			}
			else
			{
				//SCI_TRACE_LOW:"CSR Andy SIRF_Security_Send_Data CsrMemCpy"
				SCI_TRACE_ID(TRACE_TOOL_CONVERT,SIRF_PAL_TCPIP_176_112_2_18_0_33_19_1645,(uint8*)"");
				CsrMemCpy(encrypted_data_buf, data_ptr, data_len);
			}
			
			SSL_AsyncMessageProc(ssl_handle_csr, SSL_RECV_MESSAGE_SEND_SUCC, data_len);

			if(original_data_len != 0)
			{
				encrypted_data_len = data_len;
				encrypted_csr = TRUE;
			}
		}
		else
		{
			//SCI_TRACE_LOW:"CSR Andy SIRF_Security_Send_Data Error data_len %d > max %d"
			SCI_TRACE_ID(TRACE_TOOL_CONVERT,SIRF_PAL_TCPIP_190_112_2_18_0_33_19_1646,(uint8*)"dd", data_len, MAX_ENCRYPTED_DATA_BUFFER_SIZE);
		}
	}

	SSL_AsyncMessageProc(ssl_handle_csr, SSL_RECV_MESSAGE_SEND_SUCC, data_len);
	return data_len;
}

static void SIRF_Security_Receive_Data(void *ssl_app_ptr, tSIRF_UINT8 *data_ptr, tSIRF_UINT32 data_len)
{
	if(data_len < MAX_DECRYPTED_DATA_BUFFER_SIZE)
	{
		//SCI_TRACE_LOW:"CSR Andy SIRF_Security_Receive_Data data_len %d"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,SIRF_PAL_TCPIP_202_112_2_18_0_33_19_1647,(uint8*)"d", data_len);
		CsrMemCpy(decrypted_data_buf, data_ptr, data_len);
		decrypted_data_len = data_len;
		decrypted_csr = TRUE;
	}
	else
	{
		//SCI_TRACE_LOW:"CSR Andy SIRF_Security_Receive_Data Error data_len %d > max %d"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,SIRF_PAL_TCPIP_209_112_2_18_0_33_19_1648,(uint8*)"dd", data_len, MAX_DECRYPTED_DATA_BUFFER_SIZE);
	}

	SSL_AsyncMessageProc(ssl_handle_csr, SSL_RECV_MESSAGE_RECV_SUCC, data_len);
}

static void SIRF_Security_Post_Message(void *ssl_app_ptr, tSIRF_UINT32 msg_id)
{
	switch(msg_id)
	{
		case SSL_SEND_MESSAGE_HANDSHAKE_SUCC:
		{
			handShaked_csr = TRUE;
			//SCI_TRACE_LOW:"CSR Andy SIRF_Security_Post_Message SSL_SEND_MESSAGE_HANDSHAKE_SUCC"
			SCI_TRACE_ID(TRACE_TOOL_CONVERT,SIRF_PAL_TCPIP_222_112_2_18_0_33_19_1649,(uint8*)"");
			break;
		}
		case SSL_SEND_MESSAGE_FAIL:
		{
			//SCI_TRACE_LOW:"CSR Andy SIRF_Security_Post_Message SSL_SEND_MESSAGE_FAIL"
			SCI_TRACE_ID(TRACE_TOOL_CONVERT,SIRF_PAL_TCPIP_227_112_2_18_0_33_19_1650,(uint8*)"");
			break;
		}
		case SSL_SEND_MESSAGE_CLOSE_BY_SERVER:
		{
			//SCI_TRACE_LOW:"CSR Andy SIRF_Security_Post_Message SSL_SEND_MESSAGE_CLOSE_BY_SERVER"
			SCI_TRACE_ID(TRACE_TOOL_CONVERT,SIRF_PAL_TCPIP_232_112_2_18_0_33_19_1651,(uint8*)"");
			break;
		}
		case SSL_SEND_MESSAGE_CANCLED_BY_USER:
		{
			//SCI_TRACE_LOW:"CSR Andy SIRF_Security_Post_Message SSL_SEND_MESSAGE_CANCLED_BY_USER"
			SCI_TRACE_ID(TRACE_TOOL_CONVERT,SIRF_PAL_TCPIP_237_112_2_18_0_33_19_1652,(uint8*)"");
			break;
		}
		default:
		{
			//SCI_TRACE_LOW:"CSR Andy SIRF_Security_Post_Message msg_id %d"
			SCI_TRACE_ID(TRACE_TOOL_CONVERT,SIRF_PAL_TCPIP_242_112_2_18_0_33_20_1653,(uint8*)"d", msg_id);
			break;
		}
	}
}

static void SIRF_Security_Show_Cert_Info(void *ssl_app_ptr)
{
	//SCI_TRACE_LOW:"CSR Andy SIRF_Security_Show_Cert_Info"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,SIRF_PAL_TCPIP_250_112_2_18_0_33_20_1654,(uint8*)"");
	SSL_UserCnfCert(ssl_handle_csr, TRUE);
}

/* This function should be called by SIRF_PAL_Init */
tSIRF_RESULT SIRF_PAL_NET_Init(tSIRF_VOID)
{
	SSL_RESULT_E ssl_result = SSL_FAIL;
	
	//SCI_TRACE_LOW:"CSR Andy SIRF_PAL_NET_Init"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,SIRF_PAL_TCPIP_258_112_2_18_0_33_20_1655,(uint8*)"");
	socket_handle_csr = -1;
	socket_type_csr = SIRF_PAL_NET_SOCKET_TYPE_DEFAULT;
	security_csr = FALSE;
	ssl_handle_csr = 0;

	original_data_len = 0;
	encrypted_csr = FALSE;
	encrypted_data_len = 0;
	encrypted_data_len_written = 0;

	decrypted_csr = FALSE;
	handShaked_csr = FALSE;
	ssl_result = SSL_Init();

	if(ssl_result == SSL_OK)
	{
		return SIRF_SUCCESS;
	}
	else
	{
		return SIRF_PAL_OS_ERROR;
	}
}

/* This function should be called by SIRF_PAL_Destroy */
tSIRF_RESULT SIRF_PAL_NET_Destroy(tSIRF_VOID)
{
	SSL_RESULT_E ssl_result = SSL_FAIL;
	
	socket_handle_csr = -1;
	socket_type_csr = SIRF_PAL_NET_SOCKET_TYPE_DEFAULT;
	security_csr = FALSE;
	ssl_handle_csr = 0;
	
	original_data_len = 0;
	encrypted_csr = FALSE;
	encrypted_data_len = 0;
	encrypted_data_len_written = 0;

	decrypted_csr = FALSE;
	handShaked_csr = FALSE;
	ssl_result = SSL_Destroy();

	if(ssl_result == SSL_OK)
	{
		return SIRF_SUCCESS;
	}
	else
	{
		return SIRF_PAL_OS_ERROR;
	}
}


/*----------------------------------------------------------------------
 * Create Close
 *----------------------------------------------------------------------*/
tSIRF_RESULT SIRF_PAL_NET_CreateSocket( tSIRF_SOCKET *socket_handle, tSIRF_UINT32 socket_type )
{
       int sock = -1;
	
	if(socket_handle == NULL)
	{
		return SIRF_PAL_OS_ERROR;
	}
	
	/*Andy: An input of net_id '0' means low level driver will choose available network connectiong automatically. Could be WiFi or GPRS either*/
       sock = sci_sock_socket(AF_INET, SOCK_STREAM, 0, 0);/*Net ID needed*/

	if(sock == -1)
	{
	    return SIRF_PAL_OS_ERROR;
	}
	   
	if (socket_type == SIRF_PAL_NET_SOCKET_TYPE_DEFAULT)
	{
		int result_setsockopt = -1;
		
		result_setsockopt = sci_sock_setsockopt(sock, SO_NBIO, 0);

		if(result_setsockopt == -1)
		{
			return SIRF_PAL_OS_ERROR;
		}
	}
	
	*socket_handle = sock;
	socket_handle_csr = sock;
	socket_type_csr = socket_type;
	return SIRF_SUCCESS;
}

tSIRF_RESULT SIRF_PAL_NET_CloseSocket(  tSIRF_SOCKET  socket_handle )
{
	int ret = 0;
	SSL_RESULT_E result_ssl = SSL_FAIL;

	socket_handle_csr = -1;

	if(security_csr)
	{
		result_ssl = SSL_Close(ssl_handle_csr, TRUE);

		if(result_ssl != SSL_OK)
		{
			return SIRF_PAL_OS_ERROR;
		}
	}
	
	ret = sci_sock_shutdown(socket_handle, SD_BOTH);

	if(-1 == ret)
	{
		return SIRF_PAL_OS_ERROR;
	}

       ret = sci_sock_socketclose(socket_handle);

	if(-1 == ret)
   	{
   		//int err = sci_sock_errno(socket_handle);
		return SIRF_PAL_OS_ERROR; // error
   	}
	
	return SIRF_SUCCESS;
}



/*----------------------------------------------------------------------
 * Client function
 *----------------------------------------------------------------------*/
tSIRF_RESULT SIRF_PAL_NET_Connect( tSIRF_SOCKET socket_handle, 
                                   tSIRF_CHAR *addr, 
                                   tSIRF_UINT16 port, 
                                   tSIRF_UINT32 security)
{
       int32 sock = socket_handle;
	struct sci_sockaddr dst_addr = {0};
	int ret = 0;
	int temp_count = 0;
	
	if(addr == NULL)
	{
		return SIRF_PAL_OS_ERROR; // error
	}

	do
	{
		ret = sci_parse_host(addr, &dst_addr.ip_addr, 0);
		temp_count ++;
		SIRF_PAL_OS_THREAD_Sleep(500);
	}while((ret == 1) && (temp_count < 100));
	
	if(0 != ret)
	{	
		if(1 == ret)
		{
			/*Andy: Need to consider how to handle the case of pending here*/
			return SIRF_PAL_OS_ERROR;// pending
		}
		else
		{
			return SIRF_PAL_OS_ERROR; // error
		}
	}

	dst_addr.family = AF_INET;
	dst_addr.port = port;

	if(security == FALSE)
	{
		//SCI_TRACE_LOW:"CSR Andy SIRF_PAL_NET_Connect sci_sock_connect"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,SIRF_PAL_TCPIP_428_112_2_18_0_33_20_1656,(uint8*)"");
       	ret = sci_sock_connect(sock, &dst_addr, 0);
	   
		if(-1 == ret)
	   	{
	 		int err = sci_sock_errno(sock);
					
	   		//SCI_TRACE_LOW:"CSR Andy SIRF_PAL_NET_Connect sci_sock_connect sci_sock_errno"
	   		SCI_TRACE_ID(TRACE_TOOL_CONVERT,SIRF_PAL_TCPIP_435_112_2_18_0_33_20_1657,(uint8*)"");
			
			if(EINPROGRESS == err)
			{
				//SCI_TRACE_LOW:"CSR Andy SIRF_PAL_NET_Connect sci_sock_connect Pending"
				SCI_TRACE_ID(TRACE_TOOL_CONVERT,SIRF_PAL_TCPIP_439_112_2_18_0_33_20_1658,(uint8*)"");
				return SIRF_PAL_NET_CONNECT_INPROGRESS; // pending
			}
			else
			{
				//SCI_TRACE_LOW:"CSR Andy SIRF_PAL_NET_Connect Error sci_sock_connect %d"
				SCI_TRACE_ID(TRACE_TOOL_CONVERT,SIRF_PAL_TCPIP_444_112_2_18_0_33_20_1659,(uint8*)"d", err);
				return SIRF_PAL_OS_ERROR; // error
			}
	   	}
	}
	else
	{
		SSL_RESULT_E result_ssl = SSL_FAIL;
		void *pTemp = &ssl_handle_csr;

		//SCI_TRACE_LOW:"CSR Andy SIRF_PAL_NET_Connect SSL sci_sock_connect"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,SIRF_PAL_TCPIP_454_112_2_18_0_33_20_1660,(uint8*)"");
       	ret = sci_sock_connect(sock, &dst_addr, 0);

		if(socket_type_csr == SIRF_PAL_NET_SOCKET_TYPE_DEFAULT)
		{
			tSIRF_UINT32 readset[2]  = {SIRF_PAL_NET_INVALID_SOCKET, SIRF_PAL_NET_INVALID_SOCKET};
			tSIRF_UINT32 writeset[2]  = {SIRF_PAL_NET_INVALID_SOCKET, SIRF_PAL_NET_INVALID_SOCKET};
			tSIRF_UINT32 exceptset[2] = {SIRF_PAL_NET_INVALID_SOCKET, SIRF_PAL_NET_INVALID_SOCKET};
		
			readset[0] = sock;
			writeset[0] = sock;
			exceptset[0] = sock;

			//SCI_TRACE_LOW:"CSR Andy SIRF_PAL_NET_Connect SSL SIRF_PAL_NET_Select"
			SCI_TRACE_ID(TRACE_TOOL_CONVERT,SIRF_PAL_TCPIP_467_112_2_18_0_33_20_1661,(uint8*)"");
			
			if(SIRF_SUCCESS != SIRF_PAL_NET_Select(readset, writeset, exceptset, 60000))
			{
				//SCI_TRACE_LOW:"CSR Andy SIRF_PAL_NET_Connect SSL Error SIRF_PAL_NET_Select"
				SCI_TRACE_ID(TRACE_TOOL_CONVERT,SIRF_PAL_TCPIP_471_112_2_18_0_33_20_1662,(uint8*)"");
				return SIRF_PAL_OS_ERROR;
			}
		}
		
		//SCI_TRACE_LOW:"CSR Andy SIRF_PAL_NET_Connect SSL_Create"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,SIRF_PAL_TCPIP_476_112_2_18_0_33_20_1663,(uint8*)"");
		ssl_handle_csr = SSL_Create(pTemp, sock, TRUE/*FALSE*/); /*TRUE: Async*/
		//SCI_TRACE_LOW:"CSR Andy SIRF_PAL_NET_Connect SSL_Create ssl_handle_csr %d"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,SIRF_PAL_TCPIP_478_112_2_18_0_33_20_1664,(uint8*)"d", ssl_handle_csr);

		if(ssl_handle_csr == 0)
		{
		       //SCI_TRACE_LOW:"CSR Andy SIRF_PAL_NET_Connect SSL_Create Fail"
		       SCI_TRACE_ID(TRACE_TOOL_CONVERT,SIRF_PAL_TCPIP_482_112_2_18_0_33_20_1665,(uint8*)"");
			return SIRF_PAL_OS_ERROR; // error
		}

		SSL_AsyncRegCallback(ssl_handle_csr, &ssl_cb_csr);
		SSL_ProtocolChoose(ssl_handle_csr, SSL_PROTOCOLSSL, TRUE/*FALSE*/);

		//SCI_TRACE_LOW:"CSR Andy SIRF_PAL_NET_Connect SSL_HandShake"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,SIRF_PAL_TCPIP_489_112_2_18_0_33_20_1666,(uint8*)"");
		//SCI_TRACE_LOW:"CSR Andy SIRF_PAL_NET_Connect SSL_HandShake %s"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,SIRF_PAL_TCPIP_490_112_2_18_0_33_20_1667,(uint8*)"s", inet_ntoa(dst_addr.ip_addr));
		result_ssl = SSL_HandShake(ssl_handle_csr, inet_ntoa(dst_addr.ip_addr), port, TRUE);

		if(result_ssl !=  SSL_OK)
		{
			//SCI_TRACE_LOW:"CSR Andy SIRF_PAL_NET_Connect SSL_HandShake Successfully"
			SCI_TRACE_ID(TRACE_TOOL_CONVERT,SIRF_PAL_TCPIP_495_112_2_18_0_33_20_1668,(uint8*)"");
			return SIRF_PAL_OS_ERROR; // error
		}

		temp_count = 0;
		
		while(handShaked_csr == FALSE)
		{
			temp_count ++;
			//SCI_TRACE_LOW:"CSR Andy SIRF_PAL_NET_Connect SSL_HandShake count: %d, ret %d"
			SCI_TRACE_ID(TRACE_TOOL_CONVERT,SIRF_PAL_TCPIP_504_112_2_18_0_33_20_1669,(uint8*)"dd", temp_count, ret);
			SIRF_PAL_OS_THREAD_Sleep(500);
		}
		
		security_csr = TRUE;
	}
	
	return SIRF_SUCCESS;
}


/*----------------------------------------------------------------------
 * Server functions bind, listen, accept
 *----------------------------------------------------------------------*/
tSIRF_RESULT SIRF_PAL_NET_Bind(    tSIRF_SOCKET socket_handle, tSIRF_SOCKADDR *name )
{
	int sock = socket_handle;
	struct sci_sockaddr addr = {0};
	int ret = 0;

	if(name == NULL)
	{
		return SIRF_PAL_OS_ERROR; // error
	}
	
	/*Andy: Need to check if addr and port are of the same format as the platform's*/
	addr.family = AF_INET;
	addr.ip_addr = name->sin_addr;
	addr.port = name->sin_port;

	ret = sci_sock_bind(sock, &addr, 0);
	
	if(-1 == ret)
	{
		int err;
		
		err = sci_sock_errno(sock);
		
		if(EINPROGRESS == err)
		{
			return SIRF_PAL_NET_CONNECT_INPROGRESS; // pending
		}
		else
		{
			return SIRF_PAL_OS_ERROR; // error
		}
	}

	return SIRF_SUCCESS;
}

tSIRF_RESULT SIRF_PAL_NET_Listen(  tSIRF_SOCKET socket_handle )
{
	int sock = socket_handle;
	int ret = 0;

	ret = sci_sock_listen(sock, SOMAXCONN);
	
	if(-1 == ret)
	{
		int err;
		
		err = sci_sock_errno(sock);
		
		if( EINPROGRESS == err )
		{
			return SIRF_PAL_NET_CONNECT_INPROGRESS; // pending
		}
		else
		{
			return SIRF_PAL_OS_ERROR; // error
		}
	}

	return SIRF_SUCCESS;
}

tSIRF_RESULT SIRF_PAL_NET_Accept(  tSIRF_SOCKET listener_handle, 
                                   tSIRF_SOCKET *socket_handle, 
                                   tSIRF_SOCKADDR *name, 
                                   tSIRF_UINT32 security )
{
	int listen_sock = listener_handle;
	int conn_sock = 0;
	struct sci_sockaddr conn_addr = {0};
	
	if(socket_handle == NULL)
	{
		return SIRF_PAL_OS_ERROR; //error
	}
	
	if(name == NULL)
	{
		return SIRF_PAL_OS_ERROR; //error
	}

	if(security)
	{
		/*Andy: SSL is not supported in their TCP/IP driver. */
		return SIRF_PAL_OS_ERROR;
	}
	
	// non security
	conn_sock = sci_sock_accept(listen_sock, &conn_addr, 0);
	
	if( -1 == conn_sock )
	{
		if( EWOULDBLOCK == sci_sock_errno(listen_sock) )
		{
			return SIRF_PAL_NET_CONNECT_WOULD_BLOCK;//pending
		}
		else
		{
			return SIRF_PAL_OS_ERROR; //error
		}
	}

	/*Andy: Need to check if addr and port are of the same format as the platform's*/
	name->sin_addr = conn_addr.ip_addr;
	name->sin_port = conn_addr.port;
	*socket_handle = conn_sock;

	return SIRF_SUCCESS;
}


/*----------------------------------------------------------------------
 * Common functions select, send->Write recv->Read
 *----------------------------------------------------------------------*/
tSIRF_RESULT SIRF_PAL_NET_Select( tSIRF_SOCKET *read, tSIRF_SOCKET *write, tSIRF_SOCKET *excpt, tSIRF_UINT32 timeout )
{
    tSIRF_INT32 i;
    int res, to;
    sci_fd_set fdr, fdw, fde;

    SCI_FD_ZERO(&fdr);
    SCI_FD_ZERO(&fdw);
    SCI_FD_ZERO(&fde);

    if (read)
    {
        for (i=0; SIRF_PAL_NET_INVALID_SOCKET != read[i]; i++)
        {
            SCI_FD_SET(read[i], &fdr);
        }
    }

    if (write)
    {
        for (i=0; SIRF_PAL_NET_INVALID_SOCKET != write[i]; i++)
        {
            SCI_FD_SET(write[i], &fdw);
        }
    }

    if (excpt)
    {
        for (i=0; SIRF_PAL_NET_INVALID_SOCKET != excpt[i]; i++)
        {
            SCI_FD_SET(excpt[i], &fde);
        }
    }

    if (timeout < (SIRF_TIMEOUT_INFINITE/2))
    {
        to = timeout/100;
    }
    else
    {
        to = SIRF_TIMEOUT_INFINITE/2;
    }

    res = sci_sock_select(&fdr, &fdw, &fde, to);

    if (0 < res)
    {
        if (read)
        {
            for (i=0; read[i] != SIRF_PAL_NET_INVALID_SOCKET; i++)
            {
                if (!SCI_FD_ISSET(read[i], &fdr))
                {
                    read[i] = SIRF_PAL_NET_INVALID_SOCKET;
                }
            }
        }
        if (write)
        {
            for (i=0; write[i]  != SIRF_PAL_NET_INVALID_SOCKET; i++)
            {
                if (!SCI_FD_ISSET(write[i], &fdw))
                {
                    write[i] = SIRF_PAL_NET_INVALID_SOCKET;
                }
            }
        }
        if (excpt)
        {
            for (i=0; excpt[i] != SIRF_PAL_NET_INVALID_SOCKET; i++)
            {
                if (!SCI_FD_ISSET(excpt[i], &fde))
                {
                    excpt[i] = SIRF_PAL_NET_INVALID_SOCKET;
                }
            }
        }
    }

    if (0 < res)
    {
        return SIRF_SUCCESS;
    }
    else if (0 == res)
    {
	if(read)
	{
        for (i=0; SIRF_PAL_NET_INVALID_SOCKET != read[i]; i++)
        {
            read[i] = SIRF_PAL_NET_INVALID_SOCKET;
        }
	}
	if(write)
	{
        for (i=0; SIRF_PAL_NET_INVALID_SOCKET != write[i]; i++)
        {
            write[i] = SIRF_PAL_NET_INVALID_SOCKET;
        }
	}
	if(excpt)
	{
        for (i=0; SIRF_PAL_NET_INVALID_SOCKET != excpt[i]; i++)
        {
            excpt[i] = SIRF_PAL_NET_INVALID_SOCKET;
        }
	}
        return SIRF_PAL_NET_TIMEOUT;
    }
    else
    {
	 if(read)	
	 {
        for (i=0; SIRF_PAL_NET_INVALID_SOCKET != read[i]; i++)
        {
            read[i] = SIRF_PAL_NET_INVALID_SOCKET;
        }
	 }
	 if(write)
	{
        for (i=0; SIRF_PAL_NET_INVALID_SOCKET != write[i]; i++)
        {
            write[i] = SIRF_PAL_NET_INVALID_SOCKET;
        }
	}
	 if(excpt)
	 {
        for (i=0; SIRF_PAL_NET_INVALID_SOCKET != excpt[i]; i++)
        {
            excpt[i] = SIRF_PAL_NET_INVALID_SOCKET;
        }
	}
        return SIRF_PAL_NET_ERROR;
    }
}


tSIRF_RESULT SIRF_PAL_NET_Read( tSIRF_SOCKET socket_handle, tSIRF_UINT8 *buffer, tSIRF_UINT32 size, tSIRF_UINT32 *bytes_read )
{
	int sock = socket_handle;
	int ret = 0;

	if(buffer == NULL)
	{
		return SIRF_PAL_OS_ERROR; // error
	}

	if(bytes_read == NULL)
	{
		return SIRF_PAL_OS_ERROR; // error
	}

	ret = sci_sock_recv(sock, (char*)buffer, size, 0);
	
	if(ret > 0)
	{
		if(security_csr == TRUE)
		{
			decrypted_csr = FALSE;
			decrypted_data_len = 0;
			//SCI_TRACE_LOW:"CSR Andy SIRF_PAL_NET_Read SSL SSL_DecryptPasser "
			SCI_TRACE_ID(TRACE_TOOL_CONVERT,SIRF_PAL_TCPIP_790_112_2_18_0_33_21_1670,(uint8*)"");
			SSL_DecryptPasser(ssl_handle_csr, buffer, ret);

			while(decrypted_csr == FALSE)
			{
				//SCI_TRACE_LOW:"CSR Andy SIRF_PAL_NET_Read SSL SSL_DecryptPasser Waiting"
				SCI_TRACE_ID(TRACE_TOOL_CONVERT,SIRF_PAL_TCPIP_795_112_2_18_0_33_21_1671,(uint8*)"");
				SIRF_PAL_OS_THREAD_Sleep(WAIT_FOR_SSL_ENCRYPTION_CSR);
			}


			if((decrypted_data_len > 0) && (decrypted_data_len <= size))
			{
				//SCI_TRACE_LOW:"CSR Andy SIRF_PAL_NET_Read SSL SSL_DecryptPasser Done with decrypted_data_len %d"
				SCI_TRACE_ID(TRACE_TOOL_CONVERT,SIRF_PAL_TCPIP_802_112_2_18_0_33_21_1672,(uint8*)"d", decrypted_data_len);
				CsrMemCpy(buffer, decrypted_data_buf, decrypted_data_len);
				*bytes_read = decrypted_data_len;
			}
			else
			{
				//SCI_TRACE_LOW:"CSR Andy SIRF_PAL_NET_Read SSL Error SSL_DecryptPasser Done with decrypted_data_len %d (buffer size %d)"
				SCI_TRACE_ID(TRACE_TOOL_CONVERT,SIRF_PAL_TCPIP_808_112_2_18_0_33_21_1673,(uint8*)"dd", decrypted_data_len, size);
				*bytes_read = 0;
				return SIRF_PAL_OS_ERROR;
			}
		}
		else
		{
			*bytes_read = ret;
		}
	}
	else if(0 == ret)
	{
		*bytes_read = 0;
		return SIRF_PAL_NET_CONNECTION_CLOSED; 
	}
	else
	{
		int err = sci_sock_errno(sock);
		
		if(EWOULDBLOCK == err)
		{
			return SIRF_PAL_NET_CONNECT_WOULD_BLOCK;// pending
		}
		else
		{
			return SIRF_PAL_OS_ERROR; // error
		}
	}
		
	return SIRF_SUCCESS;
}

tSIRF_RESULT SIRF_PAL_NET_Write( tSIRF_SOCKET socket_handle, tSIRF_UINT8 *packet, tSIRF_UINT32 length, tSIRF_UINT32 *bytes_written )
{
	int sock = socket_handle;
	uint32 ret = 0;
	
	if(packet == NULL)
	{
		return SIRF_PAL_OS_ERROR; // error
	}

	if(bytes_written == NULL)
	{
		return SIRF_PAL_OS_ERROR; // error
	}

	if(security_csr == TRUE)
	{
		//SCI_TRACE_LOW:"CSR Andy SIRF_PAL_NET_Write SSL"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,SIRF_PAL_TCPIP_857_112_2_18_0_33_21_1674,(uint8*)"");
		
		if(original_data_len == 0)
		{
			original_data_len = length;
			encrypted_csr = FALSE;
			encrypted_data_len = 0;
			encrypted_data_len_written = 0;
			//SCI_TRACE_LOW:"CSR Andy SIRF_PAL_NET_Write SSL SSL_EncryptPasser"
			SCI_TRACE_ID(TRACE_TOOL_CONVERT,SIRF_PAL_TCPIP_865_112_2_18_0_33_21_1675,(uint8*)"");
			SSL_EncryptPasser(ssl_handle_csr, packet, length);

			while(encrypted_csr == FALSE)
			{
				//SCI_TRACE_LOW:"CSR Andy SIRF_PAL_NET_Write SSL SSL_EncryptPasser Waiting"
				SCI_TRACE_ID(TRACE_TOOL_CONVERT,SIRF_PAL_TCPIP_870_112_2_18_0_33_21_1676,(uint8*)"");
				SIRF_PAL_OS_THREAD_Sleep(WAIT_FOR_SSL_ENCRYPTION_CSR);
			}
		}

		//SCI_TRACE_LOW:"CSR Andy SIRF_PAL_NET_Write SSL SSL_EncryptPasser Done with encrypted_data_len_written %d"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,SIRF_PAL_TCPIP_875_112_2_18_0_33_21_1677,(uint8*)"d", encrypted_data_len);

		if(encrypted_data_len > 0)
		{
			//SCI_TRACE_LOW:"CSR Andy SIRF_PAL_NET_Write SSL sci_sock_send"
			SCI_TRACE_ID(TRACE_TOOL_CONVERT,SIRF_PAL_TCPIP_879_112_2_18_0_33_21_1678,(uint8*)"");
			ret = sci_sock_send(sock, (char*)&(encrypted_data_buf[encrypted_data_len_written]), encrypted_data_len, 0);
		}
		else
		{
			//SCI_TRACE_LOW:"CSR Andy SIRF_PAL_NET_Write SSL Error with encrypted_data_len_written 0"
			SCI_TRACE_ID(TRACE_TOOL_CONVERT,SIRF_PAL_TCPIP_884_112_2_18_0_33_21_1679,(uint8*)"");
			return SIRF_PAL_OS_ERROR;
		}

		if(ret > 0)
		{
			//SCI_TRACE_LOW:"CSR Andy SIRF_PAL_NET_Write SSL sci_sock_send successfully with bytes %d"
			SCI_TRACE_ID(TRACE_TOOL_CONVERT,SIRF_PAL_TCPIP_890_112_2_18_0_33_21_1680,(uint8*)"d", ret);
			encrypted_data_len = encrypted_data_len - ret;
			encrypted_data_len_written = encrypted_data_len_written + ret;

			//SCI_TRACE_LOW:"CSR Andy SIRF_PAL_NET_Write SSL bytes left %d, new writing position %d"
			SCI_TRACE_ID(TRACE_TOOL_CONVERT,SIRF_PAL_TCPIP_894_112_2_18_0_33_21_1681,(uint8*)"dd", encrypted_data_len, encrypted_data_len_written);
			
			if(encrypted_data_len == 0)
			{
				//SCI_TRACE_LOW:"CSR Andy SIRF_PAL_NET_Write SSL A set of encrption data done "
				SCI_TRACE_ID(TRACE_TOOL_CONVERT,SIRF_PAL_TCPIP_898_112_2_18_0_33_21_1682,(uint8*)"");
				*bytes_written = original_data_len;
				original_data_len = 0;
			}
			else
			{
				*bytes_written = 0;
			}
			// success
		}
		else if(0 == ret)
		{
			//SCI_TRACE_LOW:"CSR Andy SIRF_PAL_NET_Write SSL Error sci_sock_send"
			SCI_TRACE_ID(TRACE_TOOL_CONVERT,SIRF_PAL_TCPIP_910_112_2_18_0_33_21_1683,(uint8*)"");
			*bytes_written = 0;
			original_data_len = 0;
			return SIRF_PAL_NET_CONNECTION_CLOSED; 
		}
		else
		{
			int err = sci_sock_errno(sock);
			
			//SCI_TRACE_LOW:"CSR Andy SIRF_PAL_NET_Write SSL sci_sock_errno"
			SCI_TRACE_ID(TRACE_TOOL_CONVERT,SIRF_PAL_TCPIP_919_112_2_18_0_33_21_1684,(uint8*)"");
			*bytes_written = 0;
			
			if(EWOULDBLOCK == err)
			{
				//SCI_TRACE_LOW:"CSR Andy SIRF_PAL_NET_Write SSL sci_sock_errno Pending"
				SCI_TRACE_ID(TRACE_TOOL_CONVERT,SIRF_PAL_TCPIP_924_112_2_18_0_33_21_1685,(uint8*)"");
				return SIRF_PAL_NET_CONNECT_WOULD_BLOCK;// pending
			}
			else
			{
				//SCI_TRACE_LOW:"CSR Andy SIRF_PAL_NET_Write SSL Error sci_sock_errno err %d"
				SCI_TRACE_ID(TRACE_TOOL_CONVERT,SIRF_PAL_TCPIP_929_112_2_18_0_33_21_1686,(uint8*)"d", err);
				original_data_len = 0;
				return SIRF_PAL_OS_ERROR; // error
			}
		}
	}
	else
	{
		ret = sci_sock_send(sock, (char*)packet, length, 0);
		
		if(ret > 0)
		{
			*bytes_written = ret;
			// success
		}
		else if(0 == ret)
		{
			*bytes_written = 0;
			return SIRF_PAL_NET_CONNECTION_CLOSED; 
		}
		else
		{
			int err = sci_sock_errno(sock);
			
			*bytes_written = 0;
			
			if(EWOULDBLOCK == err)
			{
				return SIRF_PAL_NET_CONNECT_WOULD_BLOCK;// pending
			}
			else
			{
				return SIRF_PAL_OS_ERROR; // error
			}
		}
	}
	
	return SIRF_SUCCESS;
}

tSIRF_RESULT SIRF_PAL_NET_GetPeerName( tSIRF_SOCKET socket_handle, tSIRF_SOCKADDR *name )
{
       int32 sock = socket_handle;
	struct sci_sockaddr peer_addr = {0};
	
	if(name == NULL)
	{
		return SIRF_PAL_OS_ERROR; // error
	}
	
	if(-1 == sci_sock_getpeername(sock, &peer_addr))
	{
		return SIRF_PAL_OS_ERROR;// error
	}
	
	name->sin_addr = peer_addr.ip_addr;
	name->sin_port = peer_addr.port;
	return SIRF_SUCCESS;
}

/* Leave C naming convention */
#ifdef __cplusplus
}
#endif

/**
 * @}
 * End of file.
 */


