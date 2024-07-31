/****************************************************************************
** File Name:      sntp_unit.h                                                  *
** Author:         dewu jiang                                              *
** Date:           02/18/2011                                              *
** Copyright:                                                              *
** Description:    sntp                                                    *
****************************************************************************/
#ifdef SNTP_SUPPORT

#ifndef _SNTP_UNIT_H_
#define _SNTP_UNIT_H_

#include "socket_api.h"
#include "app_tcp_if.h"
#include "in_message.h"
#include "sci_types.h"
#include "os_api.h"
#include "cfl_list.h"
#include "cfl_char.h"


/*-------------------------------------------------------------------------- */
 /*                         MACRO DEFINITION                                                                    */
 /*--------------------------------------------------------------------------*/
#define SNTP_MAX_URL_LEN                    128
#define SNTP_MAX_REV_DATA_LEN       1024
#define SNTP_DEBUG
//#define DAPS_MEM_DEBUG
        
#ifndef DAPS_MEM_DEBUG
    #define SNTP_ALLOC(p) SCI_ALLOC(p)
    #define SNTP_FREE(p) SCI_FREE(p)
#else
    #define SNTP_ALLOC(p) CFLMEM_Malloc(p, __FILE__,__LINE__)
    #define SNTP_FREE(p) CFLMEM_Free(p)        
#endif

#ifdef SNTP_DEBUG
    #define SNTP_TRACE  SCI_TRACE_LOW
#else
    #define SNTP_TRACE
#endif

/******************************************************************************/
/*! \brief Send a signal.
 *  \author 
 *  \param[in]	_SIG_PTR        Pointer to the signal which will be sent
 *  \param[in]	_RECEIVER       ID of thread whihc receives this signal.
 *  \return None.
 */
/******************************************************************************/
#define SNTP_SEND_SIGNAL(_SIG_PTR, _RECEIVER) \
        SCI_SendSignal((xSignalHeader)(_SIG_PTR), _RECEIVER);

 /*---------------------------------------------------------------------------*/
/*                          Enum                                                                                       */
/*---------------------------------------------------------------------------*/
//socket type
typedef enum
{
    UDP_SOCKET = 0,
    TCP_SOCKET,
    RAW_SOCKET
}SOCK_TYPE_E;

 
/*---------------------------------------------------------------------------*/
/*                          VARIABLE                                                                                  */
/*---------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------*/
/*                          STRUCT                                                                                   */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           FUNCTION DECLARE                                                                   */
/*---------------------------------------------------------------------------*/
 /*****************************************************************************/
//  Description : socket
//  LOCAL resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC int32 SNTP_SockCreat(SOCK_TYPE_E sock_type, uint32 net_id);

/*****************************************************************************/
//  Description : socket close
//  LOCAL resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC void SNTP_SockClose(int32 sock_fd);

/*****************************************************************************/
//  Description : socket
//  LOCAL resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN SNTP_SendtoData(int32 sock_fd, char *data_ptr, int data_len,struct sci_sockaddr *to_ptr);

/*****************************************************************************/
//  Description : socket
//  LOCAL resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC int32  SNTP_RevFromData(int32 sock_fd, char *rev_data_ptr, int32 rev_data_len, struct sci_sockaddr *from_ptr);

/*****************************************************************************/
//  Description : socket
//  LOCAL resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN SNTP_GetHostByName(char *url_ptr, uint32 *ip_ptr);

/*****************************************************************************/
//  Description : socket
//  LOCAL resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN SNTP_GetHostByNameAsync(char *url_ptr, uint32 time_out);

/****************************************************************************/
//Description :init sntp list
//Global resource dependence : 
//Author:
//Note:	
/****************************************************************************/
PUBLIC void SNTP_ListInit(void);

/****************************************************************************/
//Description :destory sntp list
//Global resource dependence : 
//Author:
//Note:	
/****************************************************************************/
PUBLIC void SNTP_ListDestory();

/****************************************************************************/
//Description :insert sntp node
//Global resource dependence : 
//Author:
//Note:	
/****************************************************************************/
PUBLIC BOOLEAN SNTP_ListInsertNode( SNTP_APP_NODE_INFO_STR_T *sntp_node_info_ptr );

/****************************************************************************/
//Description : find info node by index
//Global resource dependence : 
//Author:
//Note:	
/****************************************************************************/
PUBLIC BOOLEAN SNTP_FindNoteByIndex(
                                uint32 index,
                                CFL_LIST_NODE_HANDLE *node_handle_ptr,
                                SNTP_APP_NODE_INFO_STR_T *sntp_node_info_ptr
                                );

/****************************************************************************/
//Description : update node info by index
//Global resource dependence : 
//Author:
//Note:	
/****************************************************************************/
PUBLIC BOOLEAN SNTP_UpdateNodeByNodeHand(
                                CFL_LIST_NODE_HANDLE node_handle,
                                SNTP_APP_NODE_INFO_STR_T *sntp_node_info_ptr
                                );

/****************************************************************************/
//Description : delete info node by index
//Global resource dependence : 
//Author:
//Note:	
/****************************************************************************/
PUBLIC BOOLEAN SNTP_DeleteNoteByIndex(uint32 index);

/*****************************************************************************/
//  Description : serialize SNTP_GET_TIME_SIG_REQ_T
//  Global resource dependence : 
//  Author:
//  Note: This function will malloc memory for signal, which should be free outer
/*****************************************************************************/
PUBLIC BOOLEAN SNTP_SerializeNettimeReqSigT(
                    const SNTP_GET_TIME_SIG_REQ_T   *sig_ptr,               //[IN] input signal pointer
                    void                         **serialized_ptr_ptr,   //[OUT] new signal pointer after serialization
                    uint32                       *serialiazed_size_ptr   //[OUT] new signal size in byte after serialization
                    );


#endif  //_SNTP_UNIT_H_
#endif  //SNTP_SUPPORT
