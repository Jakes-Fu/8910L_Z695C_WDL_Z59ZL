#ifndef _HTTPC_H_
#define _HTTPC_H_

#include <stdint.h>
#include <stdbool.h>
#include <inttypes.h>

#include "httpc_common.h"
#include "httpc_api.h"
#ifdef __cplusplus
extern "C" {
#endif

#define HTTPC_AUTHB_SIZE     128
#define HTTPC_CHUNK_SIZE     1024
#define HTTPC_SEND_BUF_SIZE  1024

#define HTTPC_MAX_SCHEME_LEN 8
#define MAX_HOST_LEN         64
#define HTTPC_MAX_URL_LEN    1024

    typedef enum {
        HTTPC_GET,
        HTTPC_POST,
        HTTPC_PUT,
        HTTPC_DELETE,
        HTTPC_HEAD
    } HTTPC_REQUEST_TYPE;

    typedef struct {
        char*    header;               /**< Custom header. */
        char*    auth_user;            /**< Username for basic authentication. */
        char*    auth_password;        /**< Password for basic authentication. */
        int      retrieve_len;         /**< Content length to be retrieved. */
        bool     is_more;              /**< Indicates if more data needs to be retrieved. */
        bool     is_chunked;           /**< Response data is encoded in portions/chunks.*/
        bool     is_usr_ssl;
        uint32_t app ;
        HTTPC_GET_PARAM_T   get_param;
        HTTPC_POST_PARAM_T  post_param;
        
        char*    post_content_type;    /**< Content type of the post data. */
        char*    post_buf;             /**< User data to be posted. */
        uint32_t post_buf_len;         /**< Post data length. */

        int      response_code;        /**< Response code. */
        char*    response_buf;         /**< Buffer to store the response data. */
        uint32_t response_buf_len;     /**< Response buffer length. */
        uint32_t response_content_len; /**< Response content length. */

        //const char*     url;
        const char      host[MAX_HOST_LEN];
        uint16_t        port;
        const char*     ca_crt;
        uint16_t        ca_crt_len;
        uintptr_t       net_handle;
        HTTPC_REQUEST_TYPE method;
        uint32_t        timeout;
    } httpc_t;

#ifdef __cplusplus
}
#endif

#endif /* __HTTPC_H__ */

