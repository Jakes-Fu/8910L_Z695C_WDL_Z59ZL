/*
 * Copyright 2003,2004 Aplix Corporation. All rights reserved.
 */

/**
 * @file JKWirelessMessagingMms.h
 *  MMS Communication JSI Header File
 */

/**
 * @addtogroup WMA_MMS_JSI
 * @{
 */

#ifndef __JKWIRELESSMESSAGING_MMS_H__
#define __JKWIRELESSMESSAGING_MMS_H__

#include "jkwirelessmessagingtypedef.h"

#ifdef __cplusplus
extern "C" {
#endif

/*
 *  Message type definitions
 */


/**
 * Message type indicating a multipart message.
 */
#define JK_WMA_MESSAGE_TYPE_MULTIPART 3

/**
 * The receive processing was normally completed.
 */
#define JK_WMA_MMS_RECEIVE_COMPLETE     1

/**
 * The receive processing was interrupted.
 */
#define JK_WMA_MMS_RECEIVE_ABORT        2

/**
 * The processing was interrupted by connection closing by the application.
 */
#define JK_WMA_MMS_RECEIVE_ABORT_USER_CLOSE     1

/**
 * Could not continue receive processing due to insufficient memory.
 */
#define JK_WMA_MMS_RECEIVE_ABORT_OUT_OF_MEMORY  2

/**
 * The processing was interrupted by an error notification by the native system.
 */
#define JK_WMA_MMS_RECEIVE_ABORT_NATIVE_ERROR   3

/**
 * A structure containing headers of multipart message
 *
 * This is a structure type for containing information of
 * the headers of multipart message.
 */
typedef struct JKT_Wma_Multipart_Message_Header {
    /**
     * A pointer to the memory area holding the name of a multipart message header field.
     *
     * The character encoding of the string is US-ASCII.
     * The string is not terminated by '\0'. 
     */
    JKSint8*   fieldName;

    /**
     * The length in octets of the string pointed to by the <i>fieldName</i> member.
     */
    JKSint32   fieldNameLength;

    /**
     * A pointer to the memory area holding the value of a multipart message header field.
     *
     * The character encoding of the string is UTF-8.
     * The string is not terminated by '\0'. 
     */
    JKUint8*   fieldValue;

    /**
     * The length in octets of the field value pointed to by the <i>fieldValue</i> member.
     */
    JKSint32   fieldValueLength;

} JKT_Wma_Multipart_Message_Header;

/**
 * A structure containing an address
 */
typedef struct JKT_Wma_Address {
    /**
     * A pointer to the memory area holding an address.
      *
     * The character encoding of the string is UTF-8.
     * The string is not terminated by '\0'. 
     */
    JKUint8*   address;

    /**
     * The length in octets of the address string pointed to by the <i>address</i> member.
     */
    JKSint32   addressLength;


} JKT_Wma_Address;


/**
 * A structure containing the message part in a multipart message body.
 */
typedef struct JKT_Wma_Message_Part
{
    /**
     * A pointer to the memory area holding the string indicating the Content-ID.
     *
     * The character encoding of the string is UTF-8.
     * The string is not terminated by '\0'. 
     * Set #JK_NULL when Content-ID is not included.
     * For a send message, JBlend set the Content-ID.
     */
    JKUint8* contentId; 

    /**
     * The length in octets of the string pointed to by the <i>contentId</i> member.
     *
     * Set 0 when Content-ID is not included.
     */
    JKSint32 contentIdLength;

    /**
     * A pointer to the memory area holding the string indicating the Content-Location.
     *
     * The character encoding of the string is UTF-8.
     * The string is not terminated by '\0'. 
     * Set #JK_NULL when Content-Location is not included.
     */
    JKUint8* contentLocation;

    /**
     * The length in octets of the string pointed to by the <i>contentLocation</i> member.
     *
     * Set 0 when Content-Location is not included.
     */
    JKSint32 contentLocationLength;

    /**
     * A pointer to the memory area holding the string indicating the MIME Content-Type of 
     * a message part body.
     *
     * The character encoding of the string is UTF-8.
     * The string is not terminated by '\0'. 
     * Setting MIME Content-Type to this member is mandatory.
     */
    JKUint8* mimeType;

    /**
     * The length in octets of the string pointed to by the <i>mimeType</i> member.
     */
    JKSint32 mimeTypeLength;

    /**
     * A pointer to the memory area holding the string indicating the Charset-Encoding of 
     * a message part body.
     *
     * The character encoding of the string is UTF-8.
     * The string is not terminated by '\0'. 
     * Set #JK_NULL when Charset-Encoding is not included.
     * The string indicating Transfer-Encoding must not be set to this member.
     */
    JKUint8* encoding;

    /**
     * The length in octets of the string pointed to by the <i>encoding</i> member.
     *
     * Set 0 when Charset-Encoding is not included.
     */
    JKSint32 encodingLength;

    /**
     * A pointer to the memory area holding the array containing the contents of 
     * a message part body.
     *
     * Set #JK_NULL when content is not included.
     */
    JKUint8* content;
    /**
     * The length in octets of the byte array pointed to by the <i>content</i> member.
     *
     * Set 0 when content is not included.
     */
    JKSint32 contentLength;
 
} JKT_Wma_Message_Part;


/**
 * A structure containing a multipart message.
 *
 * This structure type is used when JBlend passes a send message to the native system by #JkWmaMmsSendMessage(), 
 * and when JBlend requests information or data of a received message by #JkWmaMmsGetReceivedMessageInfo() or
 * #JkWmaMmsReceiveMessage().
 */
typedef struct JKT_Wma_Multipart_Message
{
    /**
     * A pointer to the memory area holding the string indicating Subject.
     *
     * The character encoding of the string is UTF-8.
     * The string is not terminated by '\0'. 
     * Set #JK_NULL when Subject is not included.
     */
    JKUint8* subject;

    /**
     * The length in octets of the string pointed to by the <i>subject</i> member.
     *
     * Set 0 when Subject is not included.
     */
    JKSint32 subjectLength;

    /**
     * A pointer to the JKT_Wma_Address structure holding the source address 
     * designated as From.
     *
     * Set #JK_NULL when this structure represents a send message.
     */
    JKT_Wma_Address*   fromAddress;

    /**
     * The number of adresses designated as To.
     *
     * Set 0 when no address is set in a To field.
     */
    JKSint32 toAddressCount;

    /**
     * A pointer to the JKT_Wma_Address structure array holding the addresses 
     * designated as To.
     *
     * The array element counts is given by the value of the <i>toAddressCount</i> member.
     * Set #JK_NULL when no address is set in a To field.
     */
    JKT_Wma_Address*   toAddress;

    /**
     * The number of adresses designated as Cc.
     *
     * Set 0 when no address is set in a Cc field.
     */
    JKSint32 ccAddressCount;

    /**
     * A pointer to the JKT_Wma_Address structure array holding the addresses 
     * designated as Cc.
     *
     * The array element counts is given by the value of the <i>ccAddressCount</i> member.
     * Set #JK_NULL when no address is set in a Cc field.
     */
    JKT_Wma_Address*   ccAddress;

    /**
     * The number of adresses designated as Bcc.
     *
     * Set 0 when no address is set in a Bcc field.
     */
    JKSint32 bccAddressCount;

    /**
     * A pointer to the JKT_Wma_Address structure array holding the addresses 
     * designated as Bcc.
     *
     * The array element counts is given by the value of the <i>bccAddressCount</i> member.
     * Set #JK_NULL when no address is set in a  Bcc field.
     */
    JKT_Wma_Address*   bccAddress;

    /**
     * A pointer to the memory area holding the string indicating the Content-Type.
     *
     * The character encoding of the string is UTF-8.
     * The string is not terminated by '\0'. 
     * Set #JK_NULL when Content-Type is not included.
     * The Start Content-ID, Application-ID, and Reply-To-Application-ID
     * are not handled by this member.
     */
    JKUint8* contentType;

    /**
     * The length in octets of the string pointed to by the <i>contentType</i> member.
     *
     * Set 0 when Content-Type is not included.
     */
    JKSint32 contentTypeLength;

    /**
     * A pointer to the memory area holding the string indicating the Start Content-Type.
     *
     * The character encoding of the string is UTF-8.
     * The string is not terminated by '\0'. 
     * Set #JK_NULL when Start Content-Type is not included.
     */
    JKUint8* startContentId; 

    /**
     * The length in octets of the string pointed to by the <i>startContentId</i> member.
     *
     * Set 0 when Start Content-Type is not included.
     */
    JKSint32 startContentIdLength;

    /**
     * A pointer to the memory area holding the string indicating the 
     * destination Application-ID.
     *
     * The character encoding of the string is US-ASCII.
     * The string is not terminated by '\0'. 
     * Set #JK_NULL when Application-ID is not included.
     */
    JKUint8* applicationId;

    /**
     * The length in octets of the string pointed to by the <i>applicationId</i> member.
     *
     * Set 0 when Application-ID is not included.
     */
    JKSint32 applicationIdLength;

    /**
     * A pointer to the memory area holding the string indicating the 
     * Application-ID that is included in the send message as a reply destination.
     *
     * The character encoding of the string is US-ASCII.
     * The string is not terminated by '\0'. 
     * Set #JK_NULL when Reply-To-Application-ID is not included.
     */
    JKUint8* replyToApplicationId;

    /**
     * The length in octets of the string pointed to by the <i>replyToApplicationId</i> member.
     *
     * Set 0 when Reply-To-Application-ID is not included.
     */
    JKSint32 replyToApplicationIdLength;

    /**
     * The flag indicating that the message send time is included in the received message.
     *
     * <ul>
     *  <li>Set #JK_FALSE when this structure represents a send message.
     *  <li>When this structure represents a received message, and if
     *      the send time is set in <i>timeStampHigh</i> and <i>timeStampLow</i>, 
     *      set #JK_TRUE, else set #JK_FALSE.
     * </ul>
     */
    JKBool   isTimeStampAvailable;

    /**
     * The high 32 bit of the send time set in the received message, expressed in 
     * milliseconds from January 1, 1970, 00:00:00.000 GMT. 
     *
     * The value of this member is valid  when the <i>isTimeStampAvailable</i> member
     * is #JK_TRUE.
     */
    JKUint32   timeStampHigh;

    /**
     * The low 32 bit of the send time set in the received message, expressed in 
     * milliseconds from January 1, 1970, 00:00:00.000 GMT. 
     *
     * The value of this member is valid  when the <i>isTimeStampAvailable</i> member
     * is #JK_TRUE.
     */
    JKUint32   timeStampLow;

    /**
     * The number of the message headers included in the multipart message.
     *
     * Set 0 when no message header is included.
     */
    JKSint32 headerCount;

    /**
     * A pointer to a #JKT_Wma_Multipart_Message_Header structure array 
     * holding the message header information set in a multipart message.
     *
     * The array element count is given in the <i>headerCount</i> member.
     *
     * The headers set in this structure array are accessible by the WMA 2.0 
     * <code>MultipartMessage</code> class <code>setHeader</code> and 
     * <code>getHeader</code> methods.
     * In the current WMA 2.0 specification, the two headers 
     * "X-Mms-Delivery-Time" and "X-Mms-Priority" can be accessed.
     *
     * Set #JK_NULL when no message header is included.
     */
    JKT_Wma_Multipart_Message_Header*   header;

    /**
     * The number of message parts attached to a multipart message. 
     *
     * If there are no attached message parts, set 0.
     */
    JKSint32 messagePartCount;

    /**
     * A pointer to the #JKT_Wma_Message_Part structure array holding
     * the message parts attached to a multipart message.
     *
     * The array element count is given in the <i>messagePartCount</i> member.
     *
     * If there are no attached message parts, set #JK_NULL.
     */
    JKT_Wma_Message_Part*   messageParts;

} JKT_Wma_Multipart_Message;



/**
 * A structure containing information about restrictions concerning
 * multipart messages.
 *
 * This structure contains information about restrictions imposed by the native
 * system on multipart message creation and sending. 
 */
typedef struct JKT_Wma_Multipart_Message_Restriction {

    /**
     *  The maximum number of addresses that can be designated as To.
     */
    JKSint32 toAddressCount;

    /**
     *  The maximum number of addresses that can be designated as Cc.
     */
    JKSint32 ccAddressCount;

    /**
     *  The maximum number of addresses that can be designated as Bcc.
     */
    JKSint32 bccAddressCount;

    /**
     *  The maximum number in total of addresses that can be designated as To, Cc, or Bcc.
     */
    JKSint32 totalAddressCount;

    /**
     * The upper limit (in octets) on the size of messages that can be sent. 
     */
    JKSint32 messageSize;

    /**
     * The upper limit (in octets) on the content size for message part creation. 
     */
    JKSint32 messagePartSize;

} JKT_Wma_Multipart_Message_Restriction;



/**
 *        Opens a connection for MMS messaging.
 * @param url
 *        A pointer to the memory area holding a string indicating the URL address part. 
 *        The string is encoded in "UTF-8" and is not terminated by '\0'. Only the address
 *        part of the URL is given in this string, not including the URL scheme (mms://)
 *        or Application-ID.
 * @param urlLength
 *        The length in bytes of the string indicated by <i>url</i>. 
 *        If no address part is designated in URL, 0 is designated.
 * @param applicationId
 *        A pointer to a memory area holding a string indicating the Application-ID. 
 *        The string is encoded in "US-ASCII" and is not terminated by '\0'. 
 *        If no Application-ID was designated for the URL designated when the MMS 
 *        connection was opened, #JK_NULL is passed in this parameter.
 *        <br><br>
 *        If a value other than #JK_NULL is set in url, the Application-ID
 *        indicated in this parameter is the send destination Application-ID.
 *        If #JK_NULL is designated, the Application-ID indicated in this parameter
 *        is the Application-ID listening for message receipt.
 *        <br><br>
 * @param applicationIdlLength
 *        The length in bytes of the string designated by <i>applicationId</i>.
 *        If no Application-ID was designated for the URL designated when the MMS 
 *        connection was opened, 0 is designated.
 * @param connectionHandle
 *        A pointer to the memory area for putting the connection handle issued by this function. 
 *        If a connection is opened successfully, put a connection handle in the memory location
 *        indicated by this pointer. If connection opening fails, set #JK_INVALID_HANDLE here.
 * @param restrictionInfo
 *        A pointer to a memory area for putting the #JKT_Wma_Multipart_Message_Restriction 
 *        structure. If a connection is opened successfully, set values in the members of 
 *        this #JKT_Wma_Multipart_Message_Restriction structure and put the structure in the
 *        address indicated in this parameter. If connection opening fails, nothing needs to 
 *        be stored here. This pointer remains valid until JkWmaMmsClose() is called.
 * @return
 *        Return #JK_WMA_OK if the operation succeeds.<br>
 *        In case of failure, return one of the following error codes. 
 *        <dl>
 *         <dt>#JK_WMA_ERROR_NOT_PERMITTED</dt>
 *          <dd>MMS communication is not permitted. </dd>      
 *         <dt>#JK_WMA_ERROR_PROTOCOL_DOESNOT_EXIST</dt>
 *          <dd>MMS is not supported.</dd>      
 *         <dt>#JK_WMA_ERROR_CONNECTION_CANT_CREATE</dt>
 *          <dd>The Application-ID designated for creating server mode connecion 
 *           is already used for other connection, or the ID is reserved.</dd>
 *         <dt>#JK_WMA_ERROR_IO_ERROR</dt>
 *          <dd>An I/O error occurred.</dd>      
 *        </dl>
 *<p>
 * Create a connection for MMS communication, issue a handle identifying the 
 * connection uniquely, and put the handle in the buffer indicated by the 
 * <i>connectionHandle</i> argument. 
 * If a value other than #JK_NULL is designated in the <i>url</i> parameter,
 * create a client mode connection; if #JK_NULL is designated, create 
 * a server mode connection.
 *
 * Until JBlend closes the connection by calling JkWmaMmsClose(), the handle 
 * passed by this function is used as a connection ID. 
 * The native system must keep track of the association between this handle 
 * and the connection and its assigned resources, so that they can be 
 * identified by the handle. 
 *
 * If the connection designated in the <i>url</i> parameter is monitored 
 * by the native system for push registry as specified in MIDP 2.0, the 
 * function must be implemented so as to ensure the following:
 * - That the connection can be opened normally without errors.
 * - That messages received before the connection was opened can also be 
 *   retrieved from the queue by JBlend using JkWmaMmsGetReceivedMessageInfo() and 
 *   JkWmaMmsReceiveMessage(). 
 * .
 */
JKSint32 JkWmaMmsOpen( JKUint8* url,
                       JKSint32 urlLength, 
                       JKUint8* applicationId,
                       JKSint32 applicationIdLength, 
                       JKHandle* connectionHandle,
                       JKT_Wma_Multipart_Message_Restriction* restrictionInfo   );


/**
 *        Closes an MMS connection.
 * @param connectionHandle
 *        The handle of the connection to be closed. 
 *        The handle issued by JkWmaMmsOpen() is designated. 
 * @return
 *        Return #JK_WMA_OK if the operation succeeds.<br>
 *        In case of failure, return #JK_WMA_ERROR_IO_ERROR. 
 * 
 * This function is called to close the connection designated by the 
 * <i>connectionHandle</i> parameter. Free the resources allocated to 
 * the designated connection. Once this function has been called, 
 * the connection designated by <i>connectionHandle</i> is no longer 
 * used by JBlend. 
 *
 * Do not allow an I/O wait to occur for this function. 
 *
 * If the connection designated by the connectionHandle parameter is 
 * monitored by the native system for push registry as specified in 
 * MIDP 2.0, the implementation of this function must not have any 
 * influence on connection monitoring by the native system. 
 */
JKSint32 JkWmaMmsClose( JKHandle connectionHandle );


/**
 *        Calculates the number of segments required for multipart message sending. 
 * @param connectionHandle
 *        The handle of the connection to be closed. 
 *        The handle issued by JkWmaMmsOpen() is designated. 
 * @param message
 *        A pointer to the #JKT_Wma_Multipart_Message structure containing 
 *        information about the send message. 
 * @param messageSize
 *        The total size in octets of the string indicating Subject and 
 *        the message part contents.
 * @return
 *        Return the number of segments required to send the message. 
 *        If the message size exceeds the maximum that can be sent, return 0. 
 */
JKSint32 JkWmaMmsNumberOfSegments( JKHandle connectionHandle, JKT_Wma_Multipart_Message* message, JKSint32 messageSize );

/**
 *        Calculates the number of segments required for message sending based on the URL. 
 * @param url
 *        A pointer to the memory area holding a string indicating the URL. 
 *        The string is encoded in "UTF-8" and is not terminated by '\0'. 
 *        The string includes the URL scheme (mms://) and Application-ID.
 * @param urlLength
 *        The length in bytes of the string indicated by <i>url</i>. 
 * @param message
 *        A pointer to the #JKT_Wma_Multipart_Message structure containing 
 *        information about the send message. 
 * @param messageSize
 *        The total size in octets of the string indicating Subject and 
 *        the message part contents.
 * @return
 *        Return the number of segments required to send the designated 
 *        message by the protocol associated with the designated URL. 
 *        If the message size exceeds the maximum that can be sent, return 0. 
 */
JKSint32 JkWmaMmsNumberOfSegmentsByUrl( JKUint8* url,JKSint32 urlLength, JKT_Wma_Multipart_Message* message, JKSint32 messageSize );

/**
 *        Sends a multipart message. 
 * @param connectionHandle
 *        The handle of the connection for sending the message. 
 *        The handle issued by JkWmaMmsOpen() is designated.  
 * @param message
 *        A pointer to the #JKT_Wma_Multipart_Message structure containing 
 *        information about the send message. 
 * @return
 *        Return #JK_WMA_OK if the operation succeeds.<br>
 *        In case of failure, return one of the following error codes. 
 *        <dl>
 *         <dt>#JK_WMA_ERROR_NOT_PERMITTED</dt>
 *          <dd>Sending is not permitted on the connection. </dd>      
 *         <dt>#JK_WMA_ERROR_IO_ERROR</dt>
 *          <dd>An I/O error occurred.</dd>      
 *         <dt>#JK_WMA_ERROR_PAYLOAD_OVER</dt>
 *          <dd>The message size exceeded the maximum. </dd>      
 *        </dl>
 *
 * In this function, start multipart message send processing and return 
 * to the caller immediately. Use AMWmaMmsEventPost() to notify JBlend of 
 * the send result. 
 * One of the following values should be passed in the <i>event</i> 
 * parameter of AMWmaMmsEventPost().
 *
 * <dl>
 *  <dt>#AM_WMA_MMS_EVENT_MESSAGE_SENT</dt>
 *   <dd>Message sending was complete. </dd>
 *  <dt>#AM_WMA_MMS_EVENT_TIMEOUT</dt>
 *   <dd>Message sending timed out.</dd>
 *  <dt>#AM_WMA_MMS_EVENT_IO_ERROR</dt>
 *   <dd>A communication error occurred.</dd>   
 * </dl>
 *
 * Only the message in a <code>javax.wireless.messaging.MultipartMessage</code> 
 * object passed from the Java application is stored in the 
 * JKT_Wma_Multipart_Message structure pointed to by the <i>message</i>
 * parameter.
 * In no case is the message in a <code>BinaryMessage</code> or 
 * <code>TextMessage</code> passed by the parameter.
 * @see AMWmaMmsEventPost()
 */
JKSint32 JkWmaMmsSendMessage( JKHandle connectionHandle, JKT_Wma_Multipart_Message* message );


/**
 *        Gets information about the element count of the structure array
 *        holding the received message.
 * @param connectionHandle
 *        The handle of the connection for which information is to be obtained. 
 *        The handle issued by JkWmaMmsOpen() is designated. 
 * @param message
 *        A pointer to the #JKT_Wma_Multipart_Message structure for putting 
 *        the information about the received message.
 * @return
 *        Return #JK_WMA_OK if the operation succeeds.<br>
 *        In case of failure, return one of the following error codes. 
 *        <dl>
 *         <dt>#JK_WMA_ERROR_NOT_PERMITTED</dt>
 *          <dd>Receiving is not permitted on the connection. </dd>      
 *         <dt>#JK_WMA_ERROR_NO_DATA_RECEIVED</dt>
 *          <dd>No messsage in the connection receive queue. </dd>      
 *        </dl>
 *<p>
 * In the following members of the #JKT_Wma_Multipart_Message structure 
 * designated in the <i>message</i> parameter, make settings for the first message
 * in the received message queue.
 * <ul>
 *  <li><b>toAddressCount</b> (The number of addresses in "To")
 *  <li><b>ccAddressCount</b> (The number of addresses in "Cc")
 *  <li><b>bccAddressCount</b> (The number of addresses in "Bcc")
 *  <li><b>headerCount</b> (The number of headers)
 *  <li><b>messagePartCount</b> (The number of message parts)
 * </ul>
 *
 * The purpose of this function is to get the size of the structure array in the
 * #JKT_Wma_Multipart_Message structure. When this function completes normally,
 * JBlend allocates a memory area for storing the structure array, then calls
 * #JkWmaMmsGetReceivedMessageLengthInfo().
 *
 * Do not allow an I/O wait to occur for this function. 
 *
 * @see JkWmaMmsReceiveMessage()
 * @see JkWmaMmsGetReceivedMessageLengthInfo()
 */
JKSint32 JkWmaMmsGetReceivedMessageInfo( JKHandle connectionHandle, JKT_Wma_Multipart_Message* message );

/**
 *        Gets the sizes of the variable length data included in 
 *        the received message.
 * @param connectionHandle
 *        The handle of the connection for which information is to be obtained. 
 *        The handle issued by JkWmaMmsOpen() is designated. 
 * @param message
 *        A pointer to the #JKT_Wma_Multipart_Message structure for putting 
 *        the information about the received message.
 * @return
 *        Return #JK_WMA_OK if the operation succeeds.<br>
 *        In case of failure, return one of the following error codes. 
 *        <dl>
 *         <dt>#JK_WMA_ERROR_NOT_PERMITTED</dt>
 *          <dd>Receiving is not permitted on the connection. </dd>      
 *         <dt>#JK_WMA_ERROR_NO_DATA_RECEIVED</dt>
 *          <dd>No messsage in the connection receive queue. </dd>      
 *        </dl>
 *<p>
 * In the following members of the #JKT_Wma_Multipart_Message structure 
 * designated in the <i>message</i> parameter, make settings for the first message
 * in the received message queue.
 * <ul>
 *  <li><b>subjectLength</b> <br>            (The length of the Subject field)
 *  <li><b>fromAddress->addressLength</b><br>(The lenght of the address designated in the From field)
 *  <li><b>toAddress[].addressLength</b> <br>(The each length of the addresses designated in the To field)
 *  <li><b>ccAddress[].addressLength</b> <br>(The each length of the addresses designated in the Cc field)
 *  <li><b>bccAddress[].addressLength</b><br>(The each length of the addresses designated in the Bcc field)
 *  <li><b>header[].fieldNameLength</b>      (The length of the header field names)
 *  <li><b>header[].fieldValueLength</b>     (The length of the header field values)
 *  <li><b>contentTypeLength</b> <br>        (The length of the Content-Type field)
 *  <li><b>applicationIdLength</b> <br>      (The length of the Application-ID field)
 *  <li><b>replyToApplicationIdLength</b><br>(The length of the Reply-To-Application-ID field)
 *  <li><b>startContentIdLength</b> <br>     (The length of the Start Content-ID field)
 *  <li><b>messageParts[].contentIdLength</b>(The lenghth of each Content-ID field of the message parts)
 *  <li><b>messageParts[].contentLocationLength</b>(The lenghth of each Content-Location field of the message parts)
 *  <li><b>messageParts[].mimeTypeLength</b> (The lenghth of each MIME Content-Type field of the message parts)
 *  <li><b>messageParts[].encodingLength</b> (The lenghth of each encoding field of the message parts)
 *  <li><b>messageParts[].contentLength</b>  (The lenghth of each Content field of the message parts)
 * </ul>
 *
 * The purpose of this function is to get the sizes of the variable length 
 * data stored in the #JKT_Wma_Multipart_Message structure.
 *
 * This function is called upon normal completion of
 * #JkWmaMmsGetReceivedMessageInfo(). When JkWmaMmsGetReceivedMessageInfo()
 * completes normally, JBlend allocates an area for the variable-length structure 
 * array obtained by JkWmaMmsGetReceivedMessageInfo(), based on the number of 
 * array elements indicated by the JKT_Wma_Multipart_Message members, then calls 
 * JkWmaMmsReceiveMessage(). The number of array elements is set in the message
 * parameter. 
 *
 * Do not allow an I/O wait to occur for this function. 
 *   
 * @see JkWmaMmsReceiveMessage()
 * @see JkWmaMmsGetReceivedMessageInfo()
 */
JKSint32 JkWmaMmsGetReceivedMessageLengthInfo( JKHandle connectionHandle, JKT_Wma_Multipart_Message* message );

/**
 *        Requests message data.
 * @param connectionHandle
 *        The handle of the connection for receiving the message. 
 *        The handle issued by #JkWmaMmsOpen() is designated. 
 * @param message
 *        A pointer to the #JKT_Wma_Multipart_Message structure for writing 
 *        the information about the received data.
 * @return
 *        Return #JK_WMA_OK if the operation succeeds.<br>
 *        In case of failure, return one of the following error codes. 
 *        <dl>
 *         <dt>#JK_WMA_ERROR_NOT_PERMITTED</dt>
 *          <dd>Receiving is not permitted on the connection. </dd>      
 *         <dt>#JK_WMA_ERROR_NO_DATA_RECEIVED</dt>
 *          <dd>No messsage in the connection receive queue. </dd>      
 *        </dl>
 *
 * In this function, start multipart message write processing and return 
 * to the caller immediately. Use AMWmaMmsEventPost() to notify JBlend of 
 * the  processing result. 
 * One of the following values should be passed in the <i>event</i> 
 * parameter of AMWmaMmsEventPost().
 * <dl>
 *  <dt>#AM_WMA_MMS_EVENT_MESSAGE_WRITING_COMPLETE</dt>
 *   <dd>Message writing was completed. </dd>
 *  <dt>#AM_WMA_MMS_EVENT_MESSAGE_WRITING_ERROR </dt>
 *   <dd>Message writing was failed. </dd>
 * </dl>
 * In the following members of the #JKT_Wma_Multipart_Message structure 
 * designated in the <i>message</i> parameter, make settings for the first message
 * in the received message queue.
 *<p>
 * <ul>
 *  <li><b>subject</b> <br> (Subject)
 *  <li><b>fromAddress->address</b> <br>(The address designated in From fields)
 *  <li><b>toAddress[].address</b> <br> (The addresses designated in the To field)
 *  <li><b>ccAddress[].address</b> <br> (The addresses designated in the Cc field)
 *  <li><b>bccAddress[].address</b> <br>(The addresses designated in the Bcc field)
 *  <li><b>isTimeStampAvailable</b> <br>(The flag indicating whether the send time is set)
 *  <li><b>timeStampHigh</b> <br> (The high 32 bit of the send time)
 *  <li><b>timeStampLow</b> <br>  (The low 32 bit of the send time)
 *  <li><b>header[].fieldName</b> (Header field names)
 *  <li><b>header[].fieldValue</b> (Header field values)
 *  <li><b>contentTypeLength</b> <br>(Content-Type)
 *  <li><b>applicationId</b> <br>(Application-ID)
 *  <li><b>replyToApplicationId</b> <br>(Reply-To-Application-ID)
 *  <li><b>startContentId</b> <br>(Start Content-ID)
 *  <li><b>messageParts[].contentId</b> (The Content-ID field of the message parts)
 *  <li><b>messageParts[].contentLocation</b> (The Content-Location field of the message parts)
 *  <li><b>messageParts[].mimeType</b> (The MIME Content-Type field of the message parts)
 *  <li><b>messageParts[].encoding</b> (The encoding field of the message parts)
 *  <li><b>messageParts[].content</b> (The Content field of the message parts)
 * </ul>
 *
 *  Before calling this function, JBlend calls #JkWmaGetReceivedMessageInfo() and
 *  #JkWmaMmsGetReceivedMessageLengthInfo() in succession. Based on the information
 *  obtained by these functions, JBlend allocates all the memory space for storing
 *  variable-length message data, and passes the addresses to the native system by
 *  means of the JKT_Wma_Multipart_Message structure indicated in the <i>message</i> parameter.
 *  The values obtained by the above functions are set as the number of elements of
 *  the array passed in this structure, and as information representing the size of 
 *  the variable-length data.
 *
 * Do not allow an I/O wait to occur for this function. 
 *
 * @see JkWmaMmsGetReceivedMessageInfo()
 * @see JkWmaMmsGetReceivedMessageLengthInfo()
 * @see AMWmaMmsEventPost()
 */
JKSint32 JkWmaMmsReceiveMessage( JKHandle connectionHandle, JKT_Wma_Multipart_Message* message );

/**
 *        Notifies the native system of the result of the message data reception.
 * @param connectionHandle
 *        The handle of the connection for receiving the message. 
 *        The handle issued by #JkWmaMmsOpen() is designated. 
 * @param status
 *        The message reception result. One of the following codes is passed.
 *        <dl>
 *         <dt>#JK_WMA_MMS_RECEIVE_COMPLETE</dt>
 *          <dd>The reception processing was completed</dd>
 *         <dt>#JK_WMA_MMS_RECEIVE_ABORT</dt>
 *          <dd>The reception processing was interrupted</dd>
 *        </dl><br>
 * @param cause
 *        The cause of the interruption of message reception.
 *        One of the following codes is passed when #JK_WMA_MMS_RECEIVE_ABORT
 *        is designated in the <i>status</i> parameter.
 *        <dl>
 *         <dt>#JK_WMA_MMS_RECEIVE_ABORT_USER_CLOSE</dt>
 *          <dd>The processing was interrupted by connection closing by the application.</dd>
 *         <dt>#JK_WMA_MMS_RECEIVE_ABORT_OUT_OF_MEMORY</dt>
 *          <dd>Could not continue processing due to insufficient memory.</dd>
 *         <dt>#JK_WMA_MMS_RECEIVE_ABORT_NATIVE_ERROR</dt>
 *          <dd>The processing was interrupted by an error notification by the native system.</dd>
 *        </dl>
 * <p>
 * JBlend notifies the native system of the result of the message data reception
 * by calling this function in the following cases.
 *  - The native system calls AMWmaMmsEventPost() passing the result 
 *    of the asynchronous processing requested by JkWmaMmsReceiveMessage().
 *  - The Java application requests closing of the connection while 
 *    JBlend is waiting for the completion of the asynchronous processing
 *    which it requested by JkWmaMmsReceiveMessage().
 *  - JBlend fails to allocate memory for storing the received message.
 *  .
 *
 * The native system must perform the following processing depending on the
 * value indicated in the <i>status</i> parameter.
 * <dl>
 *  <dt>#JK_WMA_MMS_RECEIVE_COMPLETE</dt>
 *   <dd>Delete the received message in the queue.
 *   </dd>
 *  <dt>#JK_WMA_MMS_RECEIVE_ABORT</dt>
 *   <dd>Abort the transfer of the received message to the
 *       members of the JKT_Wma_Multipart_Message structure 
 *       pointed to by the <i>message</i> parameter of 
 *       JkWmaMmsReceiveMessage().
 *       Whether or not to retain the received message is implementation-dependent.
 *   </dd>
 * </dl>
 *
 * When this function returns, JBlend continues processing assuming the processing 
 * in this function has succeeded.
 *
 * This function is not called if one of the following functions
 * returns error in the message reception sequence performed by JBlend.
 * - JkWmaMmsGetReceivedMessageInfo() 
 * - JkWmaMmsGetReceivedMessageLengthInfo() 
 * - JkWmaMmsReceiveMessage() 
 * 
 *
 * @see JkWmaMmsGetReceivedMessageInfo()
 * @see JkWmaMmsGetReceivedMessageLengthInfo()
 * @see JkWmaMmsReceiveMessage()
 */
void JkWmaMmsReceiveMessageEnd( JKHandle connectionHandle, JKSint32 status, JKSint32 cause );

/**
 *        Checks if receiving is permitted.
 * @param connectionHandle
 *        The handle of the connection for receiving the message.
 *        The handle issued by #JkWmaMmsOpen() is designated.
 * @return
 *        If receiving is permitted, return #JK_TRUE.<br>
 *        If receiving is not permitted, return #JK_FALSE.
 *
 *  Do not allow an I/O wait to occur for this function.
 */
JKBool JkWmaMmsIsPermittedToReceive( JKHandle connectionHandle );

/**
 *        Checks if the Charset-Encoding is valid.
 * @param mimeType
 *        A pointer to the memory area holding the string indicating 
 *        the MIME Content-Type of a message part body.
 *        The character encoding of the string is UTF-8.
 *        The string is not terminated by '\0'. 
 * @param mimeTypeLength
 *        The length in octets of the string pointed to by the <i>mimeType</i> .
 * @param encoding
 *        A pointer to the memory area holding the string indicating 
 *        the Charset-Encoding of a message part body.
 *        The character encoding of the string is UTF-8.
 *        The string is not terminated by '\0'. 
 *        #JK_NULL is designated if Charset-Encoding is not contained in the body.
 * @param encodingLength
 *        The length in octets of the string pointed to by the <i>encoding</i>.
 * @return
 *        Return #JK_TRUE if <i>encoding</i> is valid for <i>mimeType</i>;
 *        else return #JK_FALSE.
 */
JKBool JkWmaMmsIsValidEncoding( JKUint8 *mimeType, JKSint32 mimeTypeLength,
                                JKUint8 *encoding, JKSint32 encodingLength );

/**
 *        Gets the upper limit on the content size for message part body.
 * @return
 *        Return the upper limit (in octets) on the content size for message part body.
 *
 *  The upper limit is implementation-dependent.
 */
JKSint32 JkWmaMmsGetMaxPartContentLength( void );

#ifdef __cplusplus
}
#endif




#endif /* !__JKWIRELESSMESSAGING_MMS_H__ */

/** @} end of group */

