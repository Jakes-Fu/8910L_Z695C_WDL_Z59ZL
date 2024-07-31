/*****************************************************************************
** File Name:      http_cache_api.h                                          *
** Author:          li.li                                                    *
** Date:           14/11/2007                                                *
** Copyright:      2007 Spreadtrum, Incorporated. All Rights Reserved.       *
** Description:    This file is used to describe the HTTP cache api          *
*****************************************************************************/

#ifndef _http_cache_api_H_
#define _http_cache_api_H_ 

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "sci_types.h"
#include "http_api.h"
#include "http_def.h"
#include "cfl_wchar.h"
#include "http_util.h"
/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif

/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
#define HTTP_CACHE_EXPIRATION (24*3600)
#define HTTP_CACHE_NODE_MAX_NUM 100
#define HTTP_CACHE_INVALID_STRING "invalid"        
/*---------------------------------------------------------------------------*/
/*                          TYPE AND CONSTANT                                */
/*---------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : initialize http cache
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC HTTP_ERROR_E HttpCacheInitialize(HTTP_CACHE_T* cache_ptr);

/*****************************************************************************/
//  Description : destroy http cache node
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC void HttpCacheNodeDestroy(HTTP_CACHE_NODE_T* cache_node_ptr);

/*****************************************************************************/
//  Description : create http cache node
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC HTTP_CACHE_NODE_T* HttpCacheNodeCreate(uint32 url_len,char* url_ptr,char* content_type_ptr);

/*****************************************************************************/
//  Description : get http cache
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC HTTP_ERROR_E HttpCacheDelete(HTTP_CACHE_T* cache_ptr,HTTP_CACHE_NODE_T* cache_node_ptr);

/*****************************************************************************/
//  Description : http cache finalize
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC HTTP_ERROR_E HttpCacheFinalize(HTTP_CACHE_T* cache_ptr);

/*****************************************************************************/
//  Description : if cache is enable
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN HttpCacheIsEnable(HTTP_CACHE_T* cache_ptr);
/*****************************************************************************/
//  Description : if cache is Expired
//  Global resource dependence : 
//  Author: mingzhe.jin
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN Http_CacheIsExpired(HTTP_CACHE_NODE_T* cache_node_ptr);

/*****************************************************************************/
//  Description : check if cache is full
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN HttpCacheIsFull(HTTP_CACHE_T* cache_ptr, uint32 new_size);

/*****************************************************************************/
//  Description : delete the HTTP cache node
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC HTTP_ERROR_E HttpCacheDeleteLRU(HTTP_CACHE_T* cache_ptr);

/*****************************************************************************/
//  Description : set cache path
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC HTTP_ERROR_E HttpSetCachePath(HTTP_CACHE_T* cache_ptr, HTTP_UNICODE_T* path_ptr);

/*****************************************************************************/
//  Description : set cache index file
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC HTTP_ERROR_E HttpSetCacheIndexFile(HTTP_CACHE_T* cache_ptr, char* file_name_ptr);

/*****************************************************************************/
//  Description : set cache enable
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC HTTP_ERROR_E HttpCacheSetEnable(HTTP_CACHE_T* cache_ptr, BOOLEAN is_enable);

/*****************************************************************************/
//  Description : get http cache
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC HTTP_CACHE_NODE_T* HttpGetCache(HTTP_CACHE_T* cache_ptr,uint32 url_len,char* url_ptr);

/*****************************************************************************/
//  Description : if cache is revalidate
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN HttpCacheIsRevalidate(HTTP_CACHE_NODE_T* cache_node_ptr);

/*****************************************************************************/
//  Description : get cache etag
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC char* HttpCacheGetEtag(HTTP_CACHE_NODE_T* cache_node_ptr);

/*****************************************************************************/
//  Description : get cache last modified
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC char* HttpCacheGetLastModified(HTTP_CACHE_NODE_T* cache_node_ptr);

/*****************************************************************************/
//  Description : find http cache node
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN HttpCacheFindNode(CFL_NODE_OBJECT node, void* find_node_ptr);

/*****************************************************************************/
//  Description : http cache apend
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC HTTP_ERROR_E HttpCacheAppend(HTTP_CACHE_T* cache_ptr,HTTP_CACHE_NODE_T* cache_node_ptr,uint32 content_len,char* content_ptr,BOOLEAN is_new);

/*****************************************************************************/
//  Description : set cache expires
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC HTTP_ERROR_E HttpCacheNodeSetExpires(HTTP_CACHE_NODE_T* cache_node_ptr, int32 delta_seconds,int32 expires);

/*****************************************************************************/
//  Description : delete the HTTP cache node
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC HTTP_ERROR_E HttpCacheInsert(HTTP_CACHE_T* cache_ptr, HTTP_CACHE_NODE_T* cache_node_ptr);

/*****************************************************************************/
//  Description : delete the HTTP cache list
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC void HttpCacheDeleteList(CFL_LIST_HANDLE cache_list_handle);

/*****************************************************************************/
//  Description : find http cache node by file name
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN HttpCacheFindNodeByName(
                                 CFL_NODE_OBJECT node, 
                                 void* file_name_ptr    //[IN]
                                 );

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif