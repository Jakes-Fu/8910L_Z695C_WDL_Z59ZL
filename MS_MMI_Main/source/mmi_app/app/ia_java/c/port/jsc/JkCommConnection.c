/*
 * Copyright 2003 Aplix Corporation. All Rights Reserved.
 *
 * JBlend and all Aplix- or JBlend-related trademarks and logos are
 * trademarks or registered trademarks of Aplix in the U.S., Japan and
 * other countries.
 */

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "std_header.h"
#ifdef JAVA_SUPPORT_IA
#include "sci_types.h"
#include "jktypes.h"
#include "jkcommconnection.h"
#include "sio.h"
#include "os_api.h"
#include "jwe_log.h"
#include "amcommconnectionevent.h"
#include "jblend_platform.h"
#include "jkcommconnection.h"
#ifndef WIN32
#include "ucom_api.h"
#include "usbservice_api.h"
#endif
#include "mmiacc_id.h"
#include "mmiudisk_internal.h"
#include "mmijava_internal.h"
#include "mmi_appmsg.h"
/**-------------------------------------------------------------------------*
 **                         TYPE AND CONSTANT                                *
 **--------------------------------------------------------------------------*/
typedef struct 
{
    
    JKSint32    baud;
    JKSint32    dataBits;
    JKSint32    stopBits;
    JKSint32    parity;
    JKSint32    nComTimeoutValue;
} JAVA_DCB;

typedef struct _CommConnection 
{
    
    int32             used;
    JKCommHandle    handle;
    int32             state;
} CommConnection;



typedef struct
{
    JKCommHandle	handle;
    JKSint8			*buf;
    JKSint32		bufLen;
} WriteFuncArg;


/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
#define MAX_COMM_NUMBER 1
#define COM_PORT COM_DEBUG

#define GET_DATABUF_SIZE(data_size, _buf)  \
do  \
{   \
    if (_buf->end_point >= _buf->start_point) \
    {\
        data_size = (int32) (_buf->end_point - _buf->start_point);\
    }\
    else\
    {\
        data_size = (int32) (_buf->size + _buf->end_point - _buf->start_point);\
    }\
} while(0);
/**--------------------------------------------------------------------------*
 **                         STATIC DEFINITION                                *
 **--------------------------------------------------------------------------*/ 
static JAVA_SIGNAL *message = PNULL;
static CommConnection       connection[MAX_COMM_NUMBER] = {0};
static JAVA_DCB dcb = {0};
static BOOLEAN isJavaComm = FALSE;
const int32 name_arr[] = { BAUD_1200,  BAUD_2400,  BAUD_4800,   BAUD_9600,   BAUD_19200,
                          BAUD_38400, BAUD_57600, BAUD_115200, BAUD_230400, BAUD_460800,
                          BAUD_921600 };

const int32 baud_arr[] =  { 1200,  2400,   4800,   9600,   19200,  38400,
                           57600, 115200, 230400, 460800, 921600 };

/**--------------------------------------------------------------------------*
 **                         EXTERNAL DECLARE                                 *
 **--------------------------------------------------------------------------*/
extern BLOCK_ID g_AComId;
#ifndef WIN32
extern SIO_INFO_S      sio_port[MAX_LOGICAL_SIO_PORT_NUM];   // SIO information
#endif
/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : CommWriteData
//  Global resource dependence :
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL uint32 CommWriteData(uint8 *pData, int32 size);
/*****************************************************************************/
//  Description : CommAvailable
//  Global resource dependence :
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL uint32 CommAvailable(void);

/*****************************************************************************/
//  Description : CommClose
//  Global resource dependence :
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL void CommClose(void);

/*****************************************************************************/
//  Description : JkComm_getNewConnection
//  Global resource dependence :
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL int32 JkComm_getNewConnection(void);

/*****************************************************************************/
//  Description : JkComm_getConnectionFromHandle
//  Global resource dependence :
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL int32 JkComm_getConnectionFromHandle(JKCommHandle handle);
/*****************************************************************************/
//  Description : BAUDRATE
//  Global resource dependence :
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL int32 BAUDRATE (int32 baudrate);
/*****************************************************************************/
//  Description : _BAUDRATE
//  Global resource dependence :
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL int32 _BAUDRATE (int32 name);


/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : BAUDRATE
//  Global resource dependence :
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL int32 BAUDRATE (int32 baudrate)
{
    uint32 i = 0;
    for ( i= 0;  i < sizeof(baud_arr) / sizeof(baud_arr[0]);  i++) 
    { 
        if  (baudrate == baud_arr[i]) 
        {
            return name_arr[i];
        }  
    }
    return -1;
}

/*****************************************************************************/
//  Description : _BAUDRATE
//  Global resource dependence :
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL int32 _BAUDRATE (int32 name)
{
    uint32 i = 0;
    for ( i= 0;  i < sizeof(name_arr) / sizeof(name_arr[0]);  i++) 
    { 
        if  (name == name_arr[i]) 
        {
            return baud_arr[i];
        }  
    }
    return -1;
}

/*****************************************************************************/
//  Description : JkCommOpen
//  Global resource dependence :
//  Author: 
//  Note: 
/*****************************************************************************/
JKCommHandle JkCommOpen(const char* comportName)
{
	int32 index = -1;
	uint32 readLen = 0;
	uint32 dataLen = 0;	
	JKCommHandle ret = JK_INVALID_HANDLE;
	uint8 *buf = SCI_NULL;

	JWE_LOG(JkCommOpen,("comportName: %s", comportName))
	
	index = JkComm_getNewConnection();
	if (index < 0 || index >=  MAX_COMM_NUMBER) 
    {
		JWE_LOG(JkCommOpen,("invalid comportName: %s", comportName))
	}
	else
    {
	
		dcb.baud             = BAUD_115200;
		dcb.dataBits         = 8;
		dcb.stopBits         = 1;
		dcb.parity           = 0;
		dcb.nComTimeoutValue = 0;

		CommInit();
	    //clear data buffer
	    dataLen = CommAvailable();
	    if(dataLen>0)
	    {
	    	buf = SCI_ALLOC(dataLen+1);
			MMIJAVA_CommReadData(buf,dataLen, &readLen);
			SCI_FREE(buf);
		}
		
		Ajava_Initiliztion_COM();
		connection[index].handle =  (JKCommHandle)&connection[index];
		ret = (JKCommHandle)&connection[index];
	}
	return ret;

}

/*****************************************************************************/
//  Description : Gets a configuration of serial communication
//  Global resource dependence :
//  Author: 
//  Note: 
/*****************************************************************************/
JKSint JkCommGetParam(JKCommHandle handle, JKSint32 param, JKSint32* value)
{
	int32 index = 0;
	int32 ret = 0;
	
	JWE_LOG(JkCommGetParam, ( "[JkCommGetParam(%x, %d, %d)]\n", handle, param, value));
	
	index = JkComm_getConnectionFromHandle(handle);
	ret = JK_COMM_RESULT_SUCCESS;
	
    if (index < 0 || index >=  MAX_COMM_NUMBER) 
    {
    	JWE_LOG(JkCommGetParam,("JkCommGetParam error"))
    	ret = JK_COMM_RESULT_FAILURE;
    }
    else
    {
        switch(param)
        {
    	case JK_COMM_PARAM_TIMEOUT :	
    		*value = dcb.nComTimeoutValue;
    		break;    		
    	case JK_COMM_PARAM_BAUDRATE :
    		*value = _BAUDRATE(dcb.baud);
			break;
    	case JK_COMM_PARAM_BITSPERCHAR :
    		*value = (dcb.dataBits == 8) ? 8:7;
			break;
    	case JK_COMM_PARAM_STOPBITS :
    		*value = (dcb.stopBits == 1) ? 1:2;
			break;
    	case JK_COMM_PARAM_PARITY :
    		switch(dcb.parity)
            {
    		case 0 :
    			*value = JK_COMM_PARITY_NONE;
    			break;
			case 1 :
    			*value = JK_COMM_PARITY_ODD;
    			break;
			case 2 :
    			*value = JK_COMM_PARITY_EVEN;
    			break;
    		default:
    			ret = JK_COMM_RESULT_FAILURE;
                break;
    		}
			break;
    	default:
    		break;
		}
    }
    
	JWE_LOG(JkCommGetParam, ( "[JkCommGetParam(%x, %d, %d)]\n", handle, param, *value));
   	JWE_LOG(JkCommGetParam, ( "ret = %d", ret));

    return ret;
}


/*****************************************************************************/
//  Description : Sets a confuguration of serial communication
//  Global resource dependence :
//  Author: 
//  Note: 
/*****************************************************************************/
JKSint JkCommSetParam(JKCommHandle handle, JKSint32 param, JKSint32 value)
{
	int32 index = 0;
	int32 ret = JK_COMM_RESULT_SUCCESS;
	JWE_LOG(JkCommSetParam, ("[JkCommSetParam( %x, %d, %d )]", handle, param, value));

	index = JkComm_getConnectionFromHandle(handle);
    if (index < 0 || index >=  MAX_COMM_NUMBER)
    { 
    	JWE_LOG(JkCommGetParam,("JkCommSetParam error"))
    	ret = JK_COMM_RESULT_FAILURE;
    }
    else
    {
        switch(param)
        {
    	case JK_COMM_PARAM_TIMEOUT :	
   			dcb.nComTimeoutValue = value;
    		break;    		
    	case JK_COMM_PARAM_BAUDRATE :
			dcb.baud = BAUDRATE(value);
			break;    	
    	case JK_COMM_PARAM_BITSPERCHAR :
    		if (value==8)
            {
    			dcb.dataBits = value;
    		}
            else
            {
    			dcb.dataBits = value;
    		}
    		break;	
    	case JK_COMM_PARAM_STOPBITS :
    		if (value==1)
            {
    			dcb.stopBits = value;
    		}
            else
            {
    			dcb.stopBits = value;
    		}
    		break;	
    	case JK_COMM_PARAM_PARITY :
    		switch(value)
            {
    		case JK_COMM_PARITY_NONE :
    			dcb.parity = value;
    			break;
			case JK_COMM_PARITY_ODD :
				dcb.parity = value;
    			break;
			case JK_COMM_PARITY_EVEN :
				dcb.parity = value;
    			break;
    		default:
    			JWE_LOG(JkCommSetParam,("JK_COMM_PARAM_PARITY error"))
    			ret = JK_COMM_RESULT_FAILURE;
                break;
			}
    		break;

    	case JK_COMM_PARAM_EXTRA_CONNECT_TIMEOUT :
    	case JK_COMM_PARAM_EXTRA_READ_TIMEOUT :
    	case JK_COMM_PARAM_EXTRA_WRITE_TIMEOUT :
    	case JK_COMM_PARAM_AUTOCTS :
    	case JK_COMM_PARAM_AUTORTS :
    	default:
    		break;
   		}
    }
    
  	JWE_LOG(JkCommSetParam, ( "ret = %d", ret));

    return ret;  

	
}

/*****************************************************************************/
//  Description : Starts a serial communication.As Synchronous Mode
//  Global resource dependence :
//  Author: 
//  Note: 
/*****************************************************************************/
JKSint JkCommConnect(JKCommHandle handle)
{
/*	int32 index = 0;
	JWE_LOG(JkCommConnect,("JkCommConnect"))
	index = JkComm_getConnectionFromHandle(handle);
    if (index < 0 || index >=  MAX_COMM_NUMBER)
    { 
    	JWE_LOG(JkCommConnect,("JkCommConnect error"))
    	return JK_COMM_RESULT_FAILURE;
    }
    AmNotifyCommEvent(connection[index].handle, JK_COMM_EVENT_CONNECT , JK_COMM_RESULT_SUCCESS);
    JWE_LOG(JkCommConnect,("JkCommConnect callback end"))*/
    return JK_COMM_RESULT_SUCCESS;
    
}


/*****************************************************************************/
//  Description : Receives data from a serial device
//  Global resource dependence :
//  Author: 
//  Note: 
/*****************************************************************************/
JKSint32 JkCommRead(JKCommHandle handle, JKSint8* buffer, JKSint32 length)
{
	int32 index = 0;
	ReadFuncArg *funcArg = PNULL;
	
	index = JkComm_getConnectionFromHandle(handle);
    if (index < 0 || index >=  MAX_COMM_NUMBER)
    {
    	JWE_LOG(JkCommRead,("JkCommRead,error")) 
    	return JK_COMM_RESULT_FAILURE;
    }

	//JWE_LOG(JkCommRead,("JkCommRead"))
	funcArg = (ReadFuncArg*)SCI_ALLOC( sizeof( ReadFuncArg ) );
	funcArg->handle = handle;
	funcArg->buf = buffer;
	funcArg->bufLen = length;
	funcArg->used	= &connection[index].used;

	message = (JAVA_SIGNAL *)SCI_ALLOC(sizeof(JAVA_SIGNAL));
	message->signalCode = KJAVA_COMMREAD_SINGALCODE;
	message->signalSize = sizeof(JAVA_SIGNAL);
	message->pre = SCI_NULL;
	message->succ = SCI_NULL;
	message->pMsg = funcArg;
	SCI_SendSignal((xSignalHeader)message, g_AComId);
	//JWE_LOG(JkCommRead,("send message"))

// 	JWE_LOG(JkCommRead,("SCI_CreateThread"));
	 
	return JK_COMM_RESULT_SUCCESS;
}


/*****************************************************************************/
//  Description : Sends data to a serial device
//  Global resource dependence :
//  Author: 
//  Note: 
/*****************************************************************************/
JKSint32 JkCommWrite(JKCommHandle handle, const JKSint8* buffer, JKSint32 length)
{
/*	int32 index = 0;
    int32 ret = 0;
	JWE_LOG(JkCommWrite,("JkCommWrite, length: %d", length))
	index = JkComm_getConnectionFromHandle(handle);
    if (index < 0 || index >=  MAX_COMM_NUMBER) 
    {
    	return JK_COMM_RESULT_FAILURE;
    }

    ret = CommWriteData((uint8*)buffer, length);
	JWE_LOG(AJAVA_Main, ("comm write %d", ret));
	AmNotifyCommEvent(handle, JK_COMM_EVENT_WRITE, ret);*/
	return JK_COMM_RESULT_SUCCESS;
}

/*****************************************************************************/
//  Description : Gets the size of received data that can be acquired without I/O blocking
//  Global resource dependence :
//  Author: 
//  Note: 
/*****************************************************************************/
JKSint32 JkCommAvailable(JKCommHandle handle)
{
	int32 index = 0;
	int32 size = 0;
	
	index = JkComm_getConnectionFromHandle(handle);
    if (index < 0 || index >=  MAX_COMM_NUMBER) 
    {
   		return JK_COMM_RESULT_FAILURE;
    }
	SCI_Sleep(200);
	size = CommAvailable();
	JWE_LOG(JkCommAvailable,(" %d bytes available", size));
   	 
	return size;
}

/*****************************************************************************/
//  Description : Disconnects a connected serial communication
//  Global resource dependence :
//  Author: 
//  Note: 
/*****************************************************************************/
JKSint JkCommDisconnect(JKCommHandle handle)
{
/*	int32 index = 0;
	JWE_LOG(JkCommDisconnect,("JkCommDisconnect"));
	index = JkComm_getConnectionFromHandle(handle);

    if (index < 0 || index >=  MAX_COMM_NUMBER)
    {
		JWE_LOG(JkCommDisconnect,("JkCommDisconnect failed"));
    	return JK_COMM_RESULT_FAILURE;
   	}
	else
    {
		JWE_LOG(JkCommDisconnect,("JkCommDisconnect OK"));
	 	AmNotifyCommEvent(handle,JK_COMM_EVENT_DISCONNECT,JK_COMM_RESULT_SUCCESS);
	}*/
	return  JK_COMM_RESULT_SUCCESS;
}

/*****************************************************************************/
//  Description : Releases resources for the serial communication
//  Global resource dependence :
//  Author: 
//  Note: 
/*****************************************************************************/
JKSint JkCommClose(JKCommHandle handle)
{
	int32 index = 0;
	JWE_LOG(JkCommClose, ("JkCommClose"));
	index = JkComm_getConnectionFromHandle(handle);
    if (index < 0 || index >=  MAX_COMM_NUMBER) 
    {
    	return JK_COMM_RESULT_FAILURE;
    }

	CommClose();
    SCI_MEMSET(&connection[index],0,sizeof(CommConnection));
	return JK_COMM_RESULT_SUCCESS;
}

/*****************************************************************************/
//  Description : JkComm_getConnectionFromHandle
//  Global resource dependence :
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL int32 JkComm_getConnectionFromHandle(JKCommHandle handle)
{
	int32 i = 0;

	for(i = 0; i < MAX_COMM_NUMBER; i++)
    {
		if (connection[i].used == 1 && connection[i].handle == handle)
        {
			return i;
		}
	}	
	return -1;	
}

/*****************************************************************************/
//  Description : JkComm_getNewConnection
//  Global resource dependence :
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL int32 JkComm_getNewConnection(void)
{
	int32 i = 0;
	for(i = 0; i < MAX_COMM_NUMBER; i++)
    {
		if (connection[i].used == 0)
        {
			SCI_MEMSET(&(connection[i]), 0, sizeof(CommConnection));
			connection[i].used = 1;
			return i;
		}
	}
	return -1;
}


/*****************************************************************************/
//  Description : CommInit
//  Global resource dependence :
//  Author: 
//  Note: 
/*****************************************************************************/
BOOLEAN CommInit(void)
{
	isJavaComm = TRUE;
	MMIUDISK_StartUsbLog();
	return TRUE;
}
/*****************************************************************************/
//  Description : CommReadData
//  Global resource dependence :
//  Author: size->size to read
//  Note: readLen->real read data size (can be NULL)
/*****************************************************************************/
BOOLEAN MMIJAVA_CommReadData(uint8 *pBuf, int32 size, uint32 *readLen)
{
	uint32 rl = 0;
	
	if(!isJavaComm)
	{
		isJavaComm = TRUE;
	}
#ifndef WIN32
	SIO_ReadPPPFrame(COM_PORT, pBuf, size, &rl);
#endif
	if (readLen != NULL)
	{
		*readLen = rl;
	}

	return TRUE;
}

/*****************************************************************************/
//  Description : CommWriteData
//  Global resource dependence :
//  Author: 
//  Note: pData->data to write size->size to write return:written length
/*****************************************************************************/
LOCAL uint32 CommWriteData(uint8 *pData, int32 size)
{
	int32 i = 0;	
	if(!isJavaComm)
    {
		isJavaComm = TRUE;
	}
	for(i = 0; i < size; i++)
    {    
		CommPutChar(pData[i]);
    }
	return size;
}

/*****************************************************************************/
//  Description : CommPutChar
//  Global resource dependence :
//  Author: 
//  Note: ch->the char to send
/*****************************************************************************/
int32 CommPutChar(uint8 ch)
{
	uint32 writtenLen = 0;
	if(!isJavaComm)
    {
		isJavaComm = TRUE;
	}
#ifndef WIN32
	writtenLen = SIO_WritePPPFrame(COM_PORT, &ch, 1);
#endif
	if(writtenLen>0) 
    {
        return 0;
    }
	return -1;
}

/*****************************************************************************/
//  Description : CommGetChar
//  Global resource dependence :
//  Author: 
//  Note: -1: read failed! else low 8 bit is the data received
/*****************************************************************************/
int32 CommGetChar(void)
{
	uint32 rl = 0;
	uint8 data = 0;
	if(!isJavaComm)
    {
		isJavaComm = TRUE;
	}
#ifndef WIN32
	if (SIO_ReadPPPFrame(COM_PORT, &data, 1, &rl) != SIO_PPP_SUCCESS)
    {    
		return -1;
    }
#endif
	if (rl != 0)
    {    
		return (int32)data;
    }

	return -1;
}


/*****************************************************************************/
//  Description : CommAvailable
//  Global resource dependence :
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL uint32 CommAvailable(void)
{
    uint32 data_size = 0;
	//GET_BUF_DATA_SIZE(data_size, (&sio_port[COM_PORT].rx_buf)) 
#ifndef WIN32
	GET_DATABUF_SIZE(data_size, (&sio_port[COM_PORT].rx_buf)) /*lint !e737*/ 
#endif
	return data_size;
}


/*****************************************************************************/
//  Description : CommClose
//  Global resource dependence :
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL void CommClose(void)
{
	isJavaComm = FALSE;
}
#endif
