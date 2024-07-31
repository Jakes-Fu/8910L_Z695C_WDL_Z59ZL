#ifndef _HTTPC_NET_H
#define _HTTPC_NET_H

#include <stdint.h>
#include <stdbool.h>
#include <inttypes.h>
#define SOCKET_ERROR -1
int httpc_net_connect(httpc_t* context);
int httpc_net_read(httpc_t* context, char* buf, uint32_t len, uint32_t timeout);
int httpc_net_write(httpc_t* context, const char* buf, uint32_t len, uint32_t timeout);
int httpc_net_disconnect(httpc_t* context);

#endif /* IOTX_COMMON_NET_H */
