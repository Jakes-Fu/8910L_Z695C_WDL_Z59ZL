/*
 * Copyright 2002-2003 Aplix Corporation. All rights reserved.
 */
 
 
/**
 *  @file kjava_sys_wma.h
 *  Socket KSI
 */
 
#ifndef KJAVA_WMA_HEADER
#define KJAVA_WMA_HEADER

#ifdef __cplusplus
extern "C"{
#endif

/** @addtogroup wireless_message_ksi
 *  @{
 */


/** 
 * @addtogroup kjava_sys_wma Wireless Messaging
 * @{
 */
 
/** \brief Message type indicating a text message */
#define KJAVA_WMA_MESSAGE_TYPE_TEXT             1
/** \brief Message type indicating a binary message. */
#define KJAVA_WMA_MESSAGE_TYPE_BINARY 			2                                     
/** \brief Inidicate connection is server mode */
#define KJAVA_WMA_CONNECTION_MODE_SERVER		1
/** \brief Indicate connection is client mode */
#define KJAVA_WMA_CONNECTION_MODE_CLIENT		2
/** \brief Indicate the connection protocol is SMS */
#define KJAVA_WMA_PROTOCOL_SMS					1
/** \brief Indicate the connection protocol is CBS */
#define KJAVA_WMA_PROTOCOL_CBS					2
/** \brief Indicate the connection does not have port number */
#define KJAVA_WMA_PORT_NUMBER_NONE				-1

/** \brief Operation completed. */
#define KJAVA_WMA_OK							0
/** \brief Communication not permitted. */
#define KJAVA_WMA_ERROR_NOT_PERMITTED			(-1)
/** \brief Cannot create a connection. */
#define KJAVA_WMA_ERROR_CONNECTION_CANT_CREATE	(-2)
/** \brief The protocol does not exist. */
#define KJAVA_WMA_ERROR_PROTOCOL_DOESNOT_EXIST	(-3)
/** \brief An I/O error occurred. */
#define KJAVA_WMA_ERROR_IO_ERROR				(-4)
/** \brief Payload exceeds the maximum size. */
#define KJAVA_WMA_ERROR_PAYLOAD_OVER			(-5)
/** \brief No messages in the connection receive queue. */
#define KJAVA_WMA_ERROR_NO_DATA_RECEIVED		(-6)
/** \brief Notify VM when message has been sent by SMS thread */
#define  KJAVA_WMA_EVENT_MESSAGE_SENT   		1 
/** \brief Notify VM when message does not be sent due to timeout */
#define  KJAVA_WMA_EVENT_TIMEOUT   				2 
/** \brief Notify VM when there's incoming message */
#define  KJAVA_WMA_EVENT_MESSAGE_ARRIVED   		3  
/** \brief Notify VM when message when there's i/o error occured */
#define  KJAVA_WMA_EVENT_IO_ERROR   			4 

/** \brief The structure type for storing the information of a WMA Connection */
typedef struct KJavaWmaConnectionTag
{
	/** \brief Connection mode 
	 *   
	 * The open mode of the connection. The possible values are:
	 *    - #KJAVA_WMA_CONNECTION_MODE_SERVER
	 *    - #KJAVA_WMA_CONNECTION_MODE_CLIENT 
	 *    .
	 */
	int	mode;	
	
	/** \brief Connection Protocol
	 *
	 *   The protocol of the connection. The possible values are:
	 *   - #KJAVA_WMA_PROTOCOL_SMS
	 *   - #KJAVA_WMA_PROTOCOL_CBS
	 *   .
	 */
	int protocol;
	
	/** \brief The address of the connection.
	 *
	 * When the <i>mode</i> is #KJAVA_WMA_CONNECTION_MODE_SERVER, this field
	 * will be NULL indicates the address does not exist.
	 */	
	unsigned char* address;
	
	/** \brief Indicate the length of <i>address</i>. */
	int	addressLength;
	
	/** \brief Connection port number 
	 *
	 * If the connection port is not specified, the value in <i>port</i> 
	 * is #KJAVA_WMA_PORT_NUMBER_NONE.
	 */
	int port;

} KJavaWmaConnection;			         
                      
/** \brief The type of WMA Connection Handle */
typedef void* KJavaWmaHandle;	

/** \brief The structure for storing the WMA Message */
typedef struct KJavaWmaMessageTag
{
    /** \brief Indicates the type of the message, could be one of the follows:
     *
     *    - #KJAVA_WMA_MESSAGE_TYPE_TEXT
     *    - #KJAVA_WMA_MESSAGE_TYPE_BINARY
     *    .
     */
	int     			type;
	/** \brief Indicate the url of the message in utf8 encoding. */
	unsigned char*   	url;
	/** \brief Indicate the length of <i>url</i>. */
	int     			urlLength;
	/** \brief Indicate if the <i>timeStampHigh</i> and <i>timeStampLow</i>
	 *         contains meaningful information or not.
	 *
	 *         If the <i>isTimeStampAvailable</i> equal to 1, the <i>timeStampHigh</i> 
	 *         and <i>timeStampLow</i> contains timestamp information, otherwise the 
	 *         <i>timeStampHigh</i> and <i>timeStampLow</i> are useless.
	 */
	int     			isTimeStampAvailable;
	/** \brief A pointer to the memory area for storing the higher 32 bits for timestamp.
	 *
     *         When <i>isTimeStampAvailable</i> is equal to 1, this field contains the
     *         higher 32 bits for timestamp in millisecond since 1970.
	 */
	int     			timeStampHigh;
	/** \brief A pointer to the memory area for storing the lower 32 bits for timestamp.
	 *
     *         When <i>isTimeStampAvailable</i> is equal to 1, this field contains the
     *         lower 32 bits for timestamp in millisecond since 1970.
	 */
	int     			timeStampLow;
    /** \brief The data length for <i>binaryData</i> or <i>textData</i>.
	 *
     *         When <i>type</i> is equal to #KJAVA_WMA_MESSAGE_TYPE_TEXT, the
     *         <i>dataLength</i> is the character count of <i>textData</i>.
     *         When <i>type</i> is equal to #KJAVA_WMA_MESSAGE_TYPE_BINARY, the
     *         <i>dataLength</i> is the number of bytes in <i>binaryData</i>.    
	 */
	int					dataLength;
	/** \brief The binary data payload.
	 *
     *         When <i>type</i> is equal to #KJAVA_WMA_MESSAGE_TYPE_TEXT, the
     *         <i>binaryData</i> is set to NULL.
     *         When <i>type</i> is equal to #KJAVA_WMA_MESSAGE_TYPE_BINARY, the
     *         <i>binaryData</i> is binary data for message payload.
	 */
	unsigned char* 		binaryData;
	/** \brief The text data payload.
	 *
     *         When <i>type</i> is equal to #KJAVA_WMA_MESSAGE_TYPE_TEXT, the
     *         <i>textData</i> is the text data for message payload.
     *         When <i>type</i> is equal to #KJAVA_WMA_MESSAGE_TYPE_BINARY, the
     *         <i>textData</i> is set to NULL.
	 */
	unsigned short* 	textData;               
} KJavaWmaMessage;

/**
 * \brief Notify VM the incoming message event.
 *
 * @param handle The WMA handle handle 
 * @param event  The WMA event type that should notify VM by one of the follows: 
 *	      - #KJAVA_WMA_EVENT_MESSAGE_SENT The message has been sent by SMS thread. 
 *	      - #KJAVA_WMA_EVENT_TIMEOUT message The does not be sent due to timeout.
 *	      - #KJAVA_WMA_EVENT_MESSAGE_ARRIVED There is an incoming message. 
 *	      - #KJAVA_WMA_EVENT_IO_ERROR There is an i/o error occured.
 *        .
 * @param message
 *	      When <i>event</i> is equal to #KJAVA_WMA_EVENT_MESSAGE_ARRIVED, this field
 *        contains the message data and notify VM.
 */
typedef void (*KJavaWmaPostEvent)(KJavaWmaHandle handle, int event, KJavaWmaMessage* message);

/**
 * \brief  Open a WMA connection.
 *
 * @param  connection The connection information.
 * @param  handle The memory pointer for the memory for store the return 
 *                WMA connection handle.
 * @param  callback
 *		   Callback function for notify WMA event to VM.
 * @return
 *     - #KJAVA_WMA_OK The operation succeeds.
 *     - #KJAVA_WMA_ERROR_NOT_PERMITTED
 *     		Communication is not permitted on the connection designated in the URL. 
 *     - #KJAVA_WMA_ERROR_PROTOCOL_DOESNOT_EXIST
 *     		The protocol designated in the URL is not supported.
 *     - #KJAVA_WMA_ERROR_CONNECTION_CANT_CREATE
 *     		The port number designated in the URL is in use or reserved.
 *     - #KJAVA_WMA_ERROR_IO_ERROR An I/O error occurred.
 *     .
 */
int  kjava_wma_open (KJavaWmaConnection* connection, KJavaWmaHandle* handle, KJavaWmaPostEvent callback);

/**
 * \brief  Close the designated sms connection 
 *		
 * @param handle The connection identitier retrieved from #kjava_wma_open.
 * @return
 *         - #KJAVA_WMA_OK               Close a connection successfully.
 *		   - #KJAVA_WMA_ERROR_IO_ERROR   Error occurred.
 *         .
 */
int  kjava_wma_close (KJavaWmaHandle handle);

/**
 * \brief Send a message using a WMA connection.
 *
 * @param handle  The Connection identifier retrieved from #kjava_wma_open.
 * @param message The WMA message that request to be sent.
 * @return
 *         - #KJAVA_WMA_OK                  The message has been sent.
 *		   - #KJAVA_WMA_ERROR_IO_ERROR      The IO error occurred.
 *		   - #KJAVA_WMA_ERROR_PAYLOAD_OVER  The message exceeds system's layload.
 *         .
 */ 
int  kjava_wma_sendMessage (KJavaWmaHandle handle, KJavaWmaMessage* message);

/**
 * \brief Calculates the number of segments required for message sending.
 * 
 * @param message A pointer to the #KJavaWmaMessage structure containing information about
 *     the send message.
 * @return 
 *     Return the number of segments required to send the message.
 *     If the message size exceeds the maximum that can be sent, return 0.
 */
int kjava_wma_numberOfSegments(KJavaWmaMessage* message);


/** @} */ 
/** @} */ 

#ifdef __cplusplus
}
#endif 

#endif /* KJAVA_WMA_HEADER */   
