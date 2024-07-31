#include "port_cfg.h"
#include "http_socket.h"

#ifdef REAL_WATCH_RTOS

#include "os_api.h"
#include "socket_api.h"
#include "sci_api.h"
#include "IN_Message.h"


#if (LEBAO_USE_SSL_TYPE == 2)
	#include <openssl/ssl.h>
	#include <openssl/err.h>

	typedef struct {
		http_socket_t ctx;
		int initialized;
		
		SSL* ssl;
		SSL_CTX* sslCtx;
	} http_socket_internal_t;

	static int openssl_initialize_context(http_socket_t* ctx);
	static int openssl_ssl_connect(http_socket_t* ctx);
	static int openssl_ssl_send(http_socket_t* ctx, char const* buf, const size_t len);
	static int openssl_ssl_recv(http_socket_t* ctx, char* buf, const size_t len);
	static int openssl_ssl_close(http_socket_t* ctx);
#elif (LEBAO_USE_SSL_TYPE == 1)
	#include "ssl.h"
	#include "ssl_internal.h"
	#include "compat-1.3.h"
	#include "entropy.h"
	#include "ctr_drbg.h"

	typedef struct {
		http_socket_t ctx;
		int initialized;

		mbedtls_ssl_context ssl;
		mbedtls_ssl_config config;
		mbedtls_ctr_drbg_context drbg;
		mbedtls_entropy_context entropy;
	} http_socket_internal_t;

	static int mbedtls_initialize_context(http_socket_t* ctx);
	static int mbedtls_ssl_connect(http_socket_t* ctx);
	static int mbedtls_ssl_send(http_socket_t* ctx, char const* buf, const size_t len);
	static int mbedtls_ssl_recv(http_socket_t* ctx, char* buf, const size_t len);
	static int mbedtls_ssl_close(http_socket_t* ctx);
#elif (LEBAO_USE_SSL_TYPE == 3)
	#include "ssl_api.h"
	
	typedef struct {
		http_socket_t ctx;
		
		SSL_HANDLE ssl;
		int initialized;
	} http_socket_internal_t;

	static void sslapi_security_post_message(void* handle, uint32 id);
	static void sslapi_security_decrypt_data_output(void* handle, uint8* data, uint32 len);
	static int  sslapi_security_encrypt_data_output(void* handle, uint8* data, uint32 len);
	static void sslapi_security_show_certinfo(void* handle);

	static int sslapi_initialize_context(http_socket_t* ctx);
	static int sslapi_ssl_connect(http_socket_t* ctx);
	static int sslapi_ssl_send(http_socket_t* ctx, char const* buf, const size_t len);
	static int sslapi_ssl_recv(http_socket_t* ctx, char* buf, const size_t len);
	static int sslapi_ssl_close(http_socket_t* ctx);
#else
	typedef struct {
		http_socket_t ctx;
	} http_socket_internal_t;
#endif

typedef struct {
    _SIGNAL_VARS
    TCPIP_SOCKET_T  socket_id;  	
    uint32          error_code;
	char*			data;
	int				dataLen;
} http_socket_internal_event_t;

// private functions

static int socket_set_block(HTTP_SOCKET sock, int blocked)
{
	if (sock == HTTP_INVALID_SOCKET)
		return -1;

	return sci_sock_setsockopt(sock, (blocked == 0) ? SO_NBIO : SO_BIO, NULL);
}

static void socket_set_recv_timeout(HTTP_SOCKET sock, int seconds)
{
	unsigned long tv = seconds * 100; // 0.1s * seconds * 100
	if (sock == HTTP_INVALID_SOCKET)
		return;

	sci_sock_setsockopt(sock, SO_RCVTIMEO, (void *)&tv);
}

static void socket_set_send_timeout(HTTP_SOCKET sock, int seconds)
{
	unsigned long tv = seconds * 100;
	if (sock == HTTP_INVALID_SOCKET)
		return;

	sci_sock_setsockopt(sock, SO_SNDTIMEO, (void *)&tv);
}

static int socket_set_keep_alive(HTTP_SOCKET sock)
{
	unsigned long opt = 1;
	sci_sock_setsockopt(sock, SO_KEEPALIVE, (void *)&opt);
}

static void socket_set_send_buf(HTTP_SOCKET sock, int size)
{
	unsigned long opt = size;
	sci_sock_setsockopt(sock, SO_SNDBUF, (void *)&opt);
}

static void socket_set_recv_buf(HTTP_SOCKET sock, int size)
{
	unsigned long opt = size;
	sci_sock_setsockopt(sock, SO_RCVBUF, (void *)&opt);
}

static void socket_set_nodelay(HTTP_SOCKET sock, int nodelay)
{
	unsigned long opt = (nodelay == 0 ? 0 : 1);
	sci_sock_setsockopt(sock, TCP_NODELAY, (void *)&opt);
}

static int socket_is_connected(HTTP_SOCKET sock)
{
	char buffer[1];
	return sci_sock_recv(sock, buffer, 1, MSG_PEEK) > 0 ? 1 : 0;
}

// public functions

unsigned int http_socket_dns(char const* address, const int netId)
{
	uint32 ip = 0;
	struct sci_hostent* host = sci_gethostbyname_ext(address, netId);
	if (host != NULL) {
		if (4 == host->h_length) {
			os_memcpy(&ip, host->h_addr_list[0], 4);
		}
	}

	helper_trace("%s -> %s, netid=%d", address, inet_ntoa(ip), netId);
	return ip;
}

// 0 : error, other : ok
unsigned int http_socket_async_dns(char const* address, const int taskId, const int netId)
{
#ifdef LEBAO_PLATFORM_T117
	return sci_async_gethostbyname_ext(address, taskId, 3 * 1000, AF_INET, netId);
#else
	return sci_async_gethostbyname(address, taskId, 3 * 1000, netId);
#endif
}

http_socket_t* http_socket_create(char const* address, const unsigned int port, const int taskId, const int netId)
{
	http_socket_internal_t* internal = NULL;
	HTTP_SOCKET sock = sci_sock_socket(AF_INET, SOCK_STREAM, 0, netId);
	if (sock == HTTP_INVALID_SOCKET)
		return NULL;

	// socket_set_keep_alive(sock);
	// socket_set_send_buf(sock, 16 * 1024);
	// socket_set_recv_buf(sock, 16 * 1024);
	// socket_set_nodelay(sock, 1);

	// automatically set by sci_sock_asyncselect	
	// socket_set_block(sock, 0);
	// socket_set_send_timeout(sock, 5);
	// socket_set_recv_timeout(sock, 5);

	internal = os_malloc(sizeof(http_socket_internal_t));
	os_memset(internal, 0, sizeof(http_socket_internal_t));
	internal->ctx.sock = sock;
	internal->ctx.host = sdsnew(address);
	internal->ctx.port = port;
	internal->ctx.taskId = taskId;
	internal->ctx.netId = netId;
	internal->ctx.selectTaskId = 0;

	return &(internal->ctx);
}

int http_socket_connect(http_socket_t* ctx)
{
	HTTP_SOCKET sock = ctx->sock;
	int flag = 0;

	flag = sci_sock_asyncselect(sock, ctx->selectTaskId, AS_READ|AS_WRITE|AS_CONNECT|AS_CLOSE);
	if (flag != 0) {
		helper_trace("socket=0x%x, failed=%d", sock, flag);
		return -1;
	}
	else {
		struct sci_sockaddr sin = { 0 };
		
		sin.family = AF_INET;
		sin.ip_addr = ctx->ip;
		sin.port = htons(ctx->port);
		os_memset((void*)sin.sa_data, 0, (8 * sizeof(char)));
		
		flag = sci_sock_connect(sock, &sin, sizeof(sin));
		if (flag < 0) {
			int errcode = sci_sock_errno(sock);

			helper_trace("socket=0x%x, flag=%d, errno=%d", sock, flag, errcode);	
			if (errcode != EWOULDBLOCK && errcode != EINPROGRESS && errcode != EISCONN) {
				return -1;
			}
		}
	}

	return 0;
}

void http_socket_close(http_socket_t* ctx)
{
	http_socket_internal_t* internal = (http_socket_internal_t*)ctx;
	if (ctx == NULL)
		return;

	if (ctx->sock != HTTP_INVALID_SOCKET) {
		sci_sock_asyncselect(ctx->sock, ctx->selectTaskId, AS_NULL);
		http_socket_select_task_stop(ctx, 0);
	}

	if (ctx->isSSL) {
#if (LEBAO_USE_SSL_TYPE == 2)
		openssl_ssl_close(ctx);
#elif (LEBAO_USE_SSL_TYPE == 1)
		mbedtls_ssl_close(ctx);
#elif (LEBAO_USE_SSL_TYPE == 3)
		sslapi_ssl_close(ctx);
#endif
	}

	sdsfree(ctx->host);
	ctx->host = NULL;

	if (ctx->sock != HTTP_INVALID_SOCKET) {
		if (sci_sock_shutdown(ctx->sock, SD_BOTH) == TCPIP_SOCKET_ERROR) {
		}
		
		if (sci_sock_socketclose(ctx->sock) == TCPIP_SOCKET_ERROR) {
			// error
		}

		ctx->sock = HTTP_INVALID_SOCKET;
	}

	os_free(internal);
	internal = NULL;
}

int http_socket_send(http_socket_t* ctx, char const* buf, const size_t len)
{
	HTTP_SOCKET sock = ctx->sock;
	int bytes = 0;
	int left = (int)len;

	while (left > 0) {
		#if (LEBAO_USE_SSL_TYPE == 2)
			if (ctx->isSSL)
				bytes = openssl_ssl_send(ctx, buf, left);
			else
		#elif (LEBAO_USE_SSL_TYPE == 1)
			if (ctx->isSSL)
				bytes = mbedtls_ssl_send(ctx, buf, left);
			else
		#elif (LEBAO_USE_SSL_TYPE == 3)
			if (ctx->isSSL)
				bytes = sslapi_ssl_send(ctx, buf, left);
			else		
		#endif
				bytes = sci_sock_send(sock, buf, left, 0);
		
		if (bytes <= 0) {
			int error = sci_sock_errno(sock);
			if (EWOULDBLOCK == error || ENOBUFS == error || EINPROGRESS == error) 
				return (len - left);
			
			return -1;
		}

		buf += bytes;
		left -= bytes;
	}

	return (len - left);
}

int http_socket_recv(http_socket_t* ctx, char* buf, const size_t len)
{
	HTTP_SOCKET sock = ctx->sock;
	int left = len;
	int nRead = left > 4096 ? 4096 : left;
	int ret = 0;
	int bufLeft = 0;

	do {
		#if (LEBAO_USE_SSL_TYPE == 2)
			if (ctx->isSSL)
				ret = openssl_ssl_recv(ctx, buf, nRead);
			else
		#elif (LEBAO_USE_SSL_TYPE == 1)
			if (ctx->isSSL)
				ret = mbedtls_ssl_recv(ctx, buf, nRead);
			else
		#elif (LEBAO_USE_SSL_TYPE == 3)
			if (ctx->isSSL)
				ret = sslapi_ssl_recv(ctx, buf, nRead);
			else
		#endif
				ret = sci_sock_asyncrecv(sock, buf, nRead, 0, &bufLeft);

		if (ret > 0) {
			left -= ret;
			buf += ret;

			nRead = left > 4096 ? 4096 : left;
			if(nRead > bufLeft) nRead = bufLeft;
		}
		else {
			if (ret == 0) break;
			else return -1;
		}
	} while (left > 0 && bufLeft > 0);

	return (len - left);
}

int http_socket_select_task_run(http_socket_t* ctx, HTTP_EVENT_CALLBACK_T callback, void* data)
{
	BLOCK_ID taskId = SCI_IdentifyThread();

	unsigned short signalCode = 0;
	TCPIP_SOCKET_T sockId = 0;
	int errorCode = 0;

	int flag = -1; // 0 : finished, -1 : failed, others : continue
	int evt = 0;
	
	if(callback == NULL)
		return -1;

	do {
		xSignalHeader x = NULL;

		if(ctx != NULL) ctx->waitingSignal = 1;
		SCI_RECEIVE_SIGNAL(x, taskId);
		//SCI_PEEK_SIGNAL(x, taskId);
		if(ctx != NULL) ctx->waitingSignal = 0;

		signalCode = x->SignalCode;
		switch (signalCode) {
		case SOCKET_ASYNC_GETHOSTBYNAME_CNF: {
			ASYNC_GETHOSTBYNAME_CNF_SIG_T* dns = (ASYNC_GETHOSTBYNAME_CNF_SIG_T*)x;
			struct sci_hostent *hostent = PNULL;
			unsigned int ip = 0;

			errorCode = dns->error_code;
			hostent = &(dns->hostent);

			// the first only
			#if defined(LEBAO_PLATFORM_T117)
				if (errorCode == 0 && hostent->h_addr_list != NULL && dns->hostent.h_cntv4 > 0) {
					SCI_MEMCPY(&ip, hostent->h_addr_list[0], 4);
				}
			#else
				if (errorCode == 0 && hostent->h_addr_list != NULL && hostent->h_length == 4) {
					SCI_MEMCPY(&ip, hostent->h_addr_list[0], 4);
				}			
			#endif

			//(void *)dns->request_id
			evt = SOCK_LEBAO_DNS;
			sockId = (int)ip;
			break;
		}

		case SOCKET_CONNECT_EVENT_IND: {
			sockId = ((SOCKET_CONNECT_EVENT_IND_SIG_T*)x)->socket_id;
			errorCode = ((SOCKET_CONNECT_EVENT_IND_SIG_T*)x)->error_code;
			
			helper_trace("SOCKET_CONNECT_EVENT_IND, sock: 0x%x, code: %d, taskid=0x%x\n", sockId, errorCode, taskId);
			evt = SOCK_LEBAO_CONNECT;
			break;
		}

		case SOCKET_READ_EVENT_IND: 
			sockId = ((SOCKET_READ_EVENT_IND_SIG_T*)x)->socket_id;
			evt = SOCK_LEBAO_READ;
			break;

		case SOCKET_WRITE_EVENT_IND:
			sockId = ((SOCKET_WRITE_EVENT_IND_SIG_T*)x)->socket_id;
			evt = SOCK_LEBAO_WRITE;
			break;

		case SOCKET_CONNECTION_CLOSE_EVENT_IND:
			sockId = ((SOCKET_CONNECTION_CLOSE_EVENT_IND_SIG_T*)x)->socket_id;
			evt = SOCK_LEBAO_CLOSE;
			break;

		case SOCKET_READ_BUFFER_STATUS_EVENT_IND:
			sockId = ((SOCKET_READ_BUFFER_STATUS_EVENT_IND_SIG_T*)x)->socket_id;
			evt = SOCK_LEBAO_RDBUF;
			break;

		// internal event, not socket
		case HTTP_SIG_CLOSE_REQ:
			sockId = 0; // Warning: unkown socket, to fix it later
			evt = SOCK_LEBAO_EXCEPT;
			break;

		case HTTP_SIG_HEAD_REQ:
			sockId = ((http_socket_internal_event_t*)x)->socket_id;
			errorCode = ((http_socket_internal_event_t*)x)->error_code;
			evt = SOCK_LEBAO_SSL_CNT;	
			break;

		case HTTP_SIG_DATA_IND: {
			char* buf = ((http_socket_internal_event_t*)x)->data;
			int len = ((http_socket_internal_event_t*)x)->dataLen;
		
			evt = SOCK_LEBAO_SSL_RECV;
			sockId = ((http_socket_internal_event_t*)x)->socket_id;
			errorCode = ((http_socket_internal_event_t*)x)->error_code;

			// Todo : more safty
			flag = callback(evt, len, (void *)buf, (void *)data);
			evt = 0;
		}
			break;

		default:
			evt = 0;
			flag = 0;
			helper_trace("unknown socket event, signalCode=%d, taskId=0x%x", signalCode, taskId);
			break;
		}

		SCI_FREE_SIGNAL(x);
		x = NULL;

		if(evt != 0)
			flag = callback(evt, errorCode, (void *)sockId, (void *)data);
	}while(flag > 0);

	helper_trace("task quit, flag=%d, taskId=0x%x, signalCode=0x%x", flag, taskId, signalCode);
	return flag;
}

int http_socket_select_task_stop(http_socket_t* ctx, const int taskId)
{
	if(ctx == NULL && taskId == 0)
		return 0;   
	else {
		if(taskId != 0 || (ctx != NULL && ctx->waitingSignal != 0)) {
			http_socket_internal_event_t* sig = SCI_ALLOC(sizeof(http_socket_internal_event_t));
			sig->SignalCode = HTTP_SIG_CLOSE_REQ;

			if(SCI_SendSignalFront((xSignalHeader)sig, 
				(ctx != NULL) ? ctx->selectTaskId : taskId) != 0) {
				
				SCI_FREE_SIGNAL(sig);
				helper_trace("SCI_SendSignalFront failed\n");
				return -1;
			}
			else {
				helper_trace("SCI_SendSignalFront ok\n");
			}
		}
		
		return 0;
	}
}

int http_socket_ssl_connect(http_socket_t* ctx)
{
	if (ctx == NULL || ctx->isSSL == 0)
		return 0;

#if (LEBAO_USE_SSL_TYPE == 2)
	return openssl_ssl_connect(ctx);
#elif (LEBAO_USE_SSL_TYPE == 1)
	return mbedtls_ssl_connect(ctx);
#elif (LEBAO_USE_SSL_TYPE == 3)
	return sslapi_ssl_connect(ctx);
#else
	return 0;
#endif
}

// =================== openssl ================================================
#if (LEBAO_USE_SSL_TYPE == 2)

static int openssl_initialize_context(http_socket_t* ctx)
{
	http_socket_internal_t* internal = (http_socket_internal_t*)ctx;
	SSL_CTX * sslCtx = NULL;
	SSL* ssl = NULL;

	SSL_load_error_strings();
	SSLeay_add_ssl_algorithms();

	do {
		const char* kDefaultCipherList =
			"ECDHE-ECDSA-AES256-GCM-SHA384:"
			"ECDHE-RSA-AES256-GCM-SHA384:"
			"ECDHE-ECDSA-CHACHA20-POLY1305:"
			"ECDHE-RSA-CHACHA20-POLY1305:"
			"ECDHE-ECDSA-AES128-GCM-SHA256:"
			"ECDHE-RSA-AES128-GCM-SHA256:"
			"ECDHE-ECDSA-AES256-SHA384:"
			"ECDHE-RSA-AES256-SHA384:"
			"ECDHE-ECDSA-AES128-SHA256:"
			"ECDHE-RSA-AES128-SHA256";

		long flags = SSL_OP_ALL | SSL_OP_NO_SSLv2 | SSL_OP_NO_SSLv3 | SSL_OP_NO_TLSv1 | SSL_OP_NO_TLSv1_1;

		sslCtx = SSL_CTX_new(SSLv23_client_method());
		if (NULL == sslCtx)
			break;

		flags |= SSL_OP_NO_COMPRESSION;
		SSL_CTX_set_options(sslCtx, flags);

		SSL_CTX_set_mode(sslCtx, SSL_MODE_ACCEPT_MOVING_WRITE_BUFFER);
		SSL_CTX_set_mode(sslCtx, SSL_MODE_ENABLE_PARTIAL_WRITE);
		SSL_CTX_set_mode(sslCtx, SSL_MODE_AUTO_RETRY);

		if (SSL_CTX_set_cipher_list(sslCtx, kDefaultCipherList) != 1)
			break;

		SSL_CTX_set_verify(sslCtx, SSL_VERIFY_NONE, NULL);
		// SSL_CTX_set_alpn_protos(ctx, (const unsigned char *)"\x02h2", 3);

		ssl = SSL_new(sslCtx);
		if (NULL == ssl) {
			SSL_CTX_free(sslCtx);
			break;
		}

		internal->ssl = ssl;
		internal->sslCtx = sslCtx;

		if (sdslen(ctx->host) > 0)
			SSL_set_tlsext_host_name(internal->ssl, ctx->host);

		SSL_set_fd(internal->ssl, ctx->sock);
		SSL_set_connect_state(internal->ssl);

		internal->initialized = 1;
		return 0;
	} while (0);

	return -1;
}

static int openssl_ssl_connect(http_socket_t* ctx)
{
	http_socket_internal_t* internal = (http_socket_internal_t*)ctx;
	int ret = 0, err = 0;

	if (internal->initialized == 0) {
		ret = openssl_initialize_context(ctx);
		if (ret != 0 || internal->ssl == NULL)
			return -1;
	}
	else if (ctx->sslConnected == 1)
		return 0;

	ret = SSL_do_handshake(internal->ssl);
	if (ret == 1) {
		ctx->sslConnected = 1;
		helper_trace("SSL_do_handshake=0x%x\n", ret);
		return 0;
	}
	else {
		err = SSL_get_error(internal->ssl, ret);
	}

	return (err == SSL_ERROR_WANT_READ) ? (SOCK_LEBAO_READ | SOCK_LEBAO_EXCEPT) : ((err == SSL_ERROR_WANT_WRITE) ? (SOCK_LEBAO_WRITE | SOCK_LEBAO_EXCEPT) : -1);
}

static int openssl_ssl_send(http_socket_t* ctx, char const* buf, const size_t len)
{
	http_socket_internal_t* internal = (http_socket_internal_t*)ctx;
	return SSL_write(internal->ssl, buf, len);
}

static int openssl_ssl_recv(http_socket_t* ctx, char* buf, const size_t len)
{
	http_socket_internal_t* internal = (http_socket_internal_t*)ctx;
	return SSL_read(internal->ssl, buf, len);
}

static int openssl_ssl_close(http_socket_t* ctx)
{
	http_socket_internal_t* internal = (http_socket_internal_t*)ctx;

	if (internal->ssl != NULL) {
		SSL_shutdown(internal->ssl);
		SSL_free(internal->ssl);
		internal->ssl = NULL;
	}

	if (internal->sslCtx != NULL) {
		SSL_CTX_free(internal->sslCtx);
		internal->sslCtx = NULL;
	}

	return 0;
}

// =================== mbedtls ================================================
#elif (LEBAO_USE_SSL_TYPE == 1)
static void mbedtls_debug(void *ctx, int level, const char *file, int line, const char *str)
{
	((void)level);

	helper_trace("%s:%04d: %s", file, line, str);
}

static int mbedtls_raw_send(http_socket_t* ctx, const unsigned char* buf, const size_t len)
{
	HTTP_SOCKET sock = ctx->sock;
	int bytes = 0;

	bytes = sci_sock_send(sock, buf, len, 0);

	if (bytes < 0) {
		int error = sci_sock_errno(sock);
		if (EWOULDBLOCK == error || ENOBUFS == error || EINPROGRESS == error) 
			return MBEDTLS_ERR_SSL_WANT_WRITE;
		else
			return -1;
	}

	return bytes;
}

static int mbedtls_raw_recv(http_socket_t* ctx, unsigned char* buf, size_t len)
{
	HTTP_SOCKET sock = ctx->sock;
	int bytes = 0;
	int left = 0;

	bytes = sci_sock_asyncrecv(sock, buf, len, 0, &left);

	if (bytes < 0) {
		int error = sci_sock_errno(sock);
		if (error == EWOULDBLOCK || error == EINPROGRESS)
			return MBEDTLS_ERR_SSL_WANT_READ;
		else
			return -1;
	}

	return bytes;
}

static uint32_t __seed = 0;

static int entropy_source(void *data, uint8_t *output, size_t len, size_t *olen)
{
    uint32_t seed;

	if(__seed == 0) __seed = HAL_time();

	seed = (__seed++) >> 1;
	if(output != 0) seed |= (~(int)output / 2);
		
    if (len > sizeof(seed)) len = sizeof(seed);
    
    os_memcpy(output, &seed, len);

    *olen = len;
    return 0;
}

static int mbedtls_initialize_context(http_socket_t* ctx)
{
	http_socket_internal_t* internal = (http_socket_internal_t*)ctx;
	int ret = -1;
	char* pers = "https_client";
	
	mbedtls_entropy_init(&internal->entropy);
	mbedtls_ctr_drbg_init(&internal->drbg);

	mbedtls_entropy_add_source(&internal->entropy, entropy_source, NULL, MBEDTLS_ENTROPY_MAX_GATHER, MBEDTLS_ENTROPY_SOURCE_STRONG);

	ret = mbedtls_ctr_drbg_seed(&internal->drbg, mbedtls_entropy_func, &internal->entropy, (unsigned char *)pers, os_strlen(pers));
	if (ret != 0) {
		mbedtls_ctr_drbg_free(&internal->drbg);
		mbedtls_entropy_free(&internal->entropy);
		helper_trace("mbedtls_ctr_drbg_seed failed, 0x%x", -ret);
		return -1;
	}

	mbedtls_ssl_init(&(internal->ssl));
	mbedtls_ssl_config_init(&(internal->config));
	mbedtls_ssl_conf_rng(&(internal->config), mbedtls_ctr_drbg_random, &internal->drbg);
	mbedtls_ssl_config_defaults(&internal->config, MBEDTLS_SSL_IS_CLIENT, MBEDTLS_SSL_TRANSPORT_STREAM, MBEDTLS_SSL_PRESET_DEFAULT);
	mbedtls_ssl_conf_authmode(&internal->config, MBEDTLS_SSL_VERIFY_NONE);

	mbedtls_ssl_conf_rng(&internal->config, mbedtls_ctr_drbg_random, &internal->drbg);
	mbedtls_ssl_conf_dbg(&internal->config, mbedtls_debug, NULL);
	mbedtls_ssl_set_bio(&internal->ssl, (void *)ctx, mbedtls_raw_send, mbedtls_raw_recv, NULL);

	ret = mbedtls_ssl_setup(&internal->ssl, &internal->config);
	internal->initialized = 1;
	return ret;
}

static int mbedtls_ssl_connect(http_socket_t* ctx)
{
	http_socket_internal_t* internal = (http_socket_internal_t*)ctx;
	int ret = 0;

	if (internal->initialized == 0) {
		ret = mbedtls_initialize_context(ctx);
		if(ret != 0) {
			helper_trace("mbedtls_initialize_context=0x%x\n", -ret);
			return -1;
		}
	}
	else if (ctx->sslConnected == 1)
		return 0;

	ret = mbedtls_ssl_handshake(&internal->ssl);
	if (ret == 0) {
		ctx->sslConnected = 1;
		helper_trace("mbedtls_ssl_handshake=0x%x\n", ret);
		return 0;
	}

	return (ret == MBEDTLS_ERR_SSL_WANT_READ) ? (SOCK_LEBAO_READ | SOCK_LEBAO_EXCEPT) : ((ret == MBEDTLS_ERR_SSL_WANT_WRITE) ? (SOCK_LEBAO_WRITE | SOCK_LEBAO_EXCEPT) : -1);
}

static int mbedtls_ssl_send(http_socket_t* ctx, char const* buf, const size_t len)
{
	http_socket_internal_t* internal = (http_socket_internal_t*)ctx;
	return mbedtls_ssl_write(&internal->ssl, buf, len);
}

static int mbedtls_ssl_recv(http_socket_t* ctx, char* buf, const size_t len)
{
	http_socket_internal_t* internal = (http_socket_internal_t*)ctx;
	return mbedtls_ssl_read(&internal->ssl, buf, len);
}

static int mbedtls_ssl_close(http_socket_t* ctx)
{
	http_socket_internal_t* internal = (http_socket_internal_t*)ctx;

	mbedtls_ssl_close_notify(&internal->ssl);
	mbedtls_ctr_drbg_free(&internal->drbg);
	mbedtls_entropy_free(&internal->entropy);
	mbedtls_ssl_free(&internal->ssl);
	mbedtls_ssl_config_free(&internal->config);

	return 0;
}

// =================== ssl_api ================================================
#elif (LEBAO_USE_SSL_TYPE == 3)
static int sslapi_post_message_to_select_task(uint32 evt, http_socket_t* ctx, uint32 code, char* data, int len)
{
	if(ctx == NULL)
		return -1;   
	else {
		http_socket_internal_event_t* sig = SCI_ALLOC(sizeof(http_socket_internal_event_t));
		sig->SignalCode = evt;
		sig->socket_id = ctx->sock;
		sig->error_code = code;
		sig->data = data;
		sig->dataLen = len;

		if(SCI_SendSignal((xSignalHeader)sig, ctx->selectTaskId) != 0) {
			
			SCI_FREE_SIGNAL(sig);
			return -1;
		}

		return 0;
	}
}

static void sslapi_security_post_message(void* handle, uint32 id)
{
	http_socket_internal_t* internal = (http_socket_internal_t*)handle;

	helper_trace("SSL_AsyncMessageProc=%d, handle=0x%x", id, handle);

	switch (id) {
		case SSL_SEND_MESSAGE_SEND_END: 
			if(internal != NULL)
				internal->ctx.bufferIsFull = 0;
		break;
		
		case SSL_SEND_MESSAGE_HANDSHAKE_SUCC: {
			if(internal != NULL) {
				internal->ctx.sslConnected = 1;
				sslapi_post_message_to_select_task(HTTP_SIG_HEAD_REQ, &internal->ctx, 0, NULL, 0);
			}
		}
		break;
			
		case SSL_SEND_MESSAGE_FAIL:
			break;
			
		case SSL_SEND_MESSAGE_CLOSE_BY_SERVER: break;
		case SSL_SEND_MESSAGE_CANCLED_BY_USER: break;

		case 204: break; //alpn, h1
		case 205: break; //apln, h2
			
		default: break;
	}
}

static void sslapi_security_decrypt_data_output(void* handle, uint8* data, uint32 len)
{
	http_socket_internal_t* internal = (http_socket_internal_t*)handle;
	char* buf = NULL;

	if(handle == NULL || data == NULL || len <= 0) return;
	
	buf = os_malloc(len);
	os_memcpy(buf, data, len);
	
	SSL_AsyncMessageProc(internal->ssl, SSL_RECV_MESSAGE_RECV_SUCC, len);
	if(sslapi_post_message_to_select_task(HTTP_SIG_DATA_IND, &internal->ctx, 0, buf, len) != 0) {
		os_free(buf);
	}
	
	helper_trace("data=0x%x, len=%d\n", data, len);	
}

static int sslapi_security_encrypt_data_output(void* handle, uint8* data, uint32 len)
{
	http_socket_internal_t* internal = (http_socket_internal_t*)handle;
	HTTP_SOCKET sock = HTTP_INVALID_SOCKET;
	int bytes = 0;
	SSL_HANDLE ssl = 0;

	if(internal == NULL || internal->ssl == 0) {
		// memory leak ?
		return 0;
	}

	ssl = internal->ssl;

	if(data == NULL || len <= 0 || internal->ctx.sock == HTTP_INVALID_SOCKET) {
		SSL_AsyncMessageProc(ssl, SSL_RECV_MESSAGE_SEND_FAIL, len);
		return 0;
	}

	sock = internal->ctx.sock;
	bytes = sci_sock_send(sock, data, len, 0);

	helper_trace("data=0x%x, len=%d, sent=%d\n", data, len, bytes);

	if(internal->ssl == NULL) 
		return 0;

	// Todo : check bytes == len
	if(bytes > 0)
		SSL_AsyncMessageProc(ssl, SSL_RECV_MESSAGE_SEND_SUCC, bytes);
	else {
		// int error = sci_sock_errno(sock);
		// if (EWOULDBLOCK == error || ENOBUFS == error || EINPROGRESS == error) 
		//		internal->ctx.bufferIsFull = -1;
				
		internal->ctx.bufferIsFull = 1;
		SSL_AsyncMessageProc(ssl, SSL_RECV_MESSAGE_SEND_FAIL, len);
	}
	
	return (bytes > 0) ? bytes : 0;
}

static void sslapi_security_show_certinfo(void* handle)
{
	http_socket_internal_t* internal = (http_socket_internal_t*)handle;

	SSL_UserCnfCert(internal->ssl, TRUE);
	helper_trace("SSL_UserCnfCert TRUE\n");
}

static int sslapi_initialize_context(http_socket_t* ctx)
{
	http_socket_internal_t* internal = (http_socket_internal_t*)ctx;
//	SSL_Init();
	internal->ssl = SSL_Create(internal, ctx->sock, SSL_ASYNC);
	
	if(internal->ssl == NULL) {
		helper_trace("sslapi_initialize_context failed\n");
		return -1;
	}
	else {
		SSL_CALLBACK_T  callback = { 
		sslapi_security_post_message,
		sslapi_security_decrypt_data_output,
		sslapi_security_encrypt_data_output,
		sslapi_security_show_certinfo};

		SSL_AsyncRegCallback(internal->ssl, &callback);
		SSL_ProtocolChoose(internal->ssl, SSL_PROTOCOLTLS_1_2, SSL_ASYNC);
		#if defined(PLATFORM_UMS9117) || defined(PLATFORM_ANTISW3)
		SSL_SetExtensionMode(internal->ssl, SSL_HELLOEXT_ALPN_H2);
		#endif
		internal->initialized = 1;
		helper_trace("ssl_handle=0x%x\n", internal->ssl);
		return 0;
	}
}

static int sslapi_ssl_connect(http_socket_t* ctx)
{
	http_socket_internal_t* internal = (http_socket_internal_t*)ctx;
	SSL_RESULT_E result = SSL_FAIL;

	if (internal->initialized == 0) {
		result = sslapi_initialize_context(ctx);
		if(result != 0) {
			helper_trace("failed 0x%x\n", result);
			return -1;
		}
	}
	else if (ctx->sslConnected == 1)
		return 0;

	result = SSL_HandShake(internal->ssl, ctx->host, ctx->port, SSL_ASYNC);

	helper_trace("SSL_HandShake %d\n", result);
	return (ctx->sslConnected == 0) ? 1 : 0;
}

static int sslapi_ssl_send(http_socket_t* ctx, char const* buf, const size_t len)
{
	http_socket_internal_t* internal = (http_socket_internal_t*)ctx;
	if(internal->ctx.bufferIsFull != 0)
		return (internal->ctx.bufferIsFull > 0) ? 0 : -1;
	
//	helper_trace("SSL_EncryptPasser 0x%x, len=%d\n", buf, len);
	internal->ctx.bufferIsFull = 1;
	SSL_EncryptPasser(internal->ssl, buf, len);
//	helper_trace("SSL_EncryptPasser end");
	return len;
}

static int sslapi_ssl_recv(http_socket_t* ctx, char* buf, const size_t len)
{
	http_socket_internal_t* internal = (http_socket_internal_t*)ctx;
	HTTP_SOCKET sock = ctx->sock;
	int bytes = 0;
	int left = 0;

	do {
		bytes = sci_sock_asyncrecv(sock, buf, len, 0, &left);
//		helper_trace("sslapi_ssl_recv %d, left %d, buf(addr) 0x%x\n", bytes, left, buf);

		if (bytes <= 0)
			return -1;

		SSL_DecryptPasser(internal->ssl, buf, bytes);
	} while(left > 0);
	
	return bytes;
}

static int sslapi_ssl_close(http_socket_t* ctx)
{
	http_socket_internal_t* internal = (http_socket_internal_t*)ctx;

	if(internal->ssl != 0) {
		helper_trace("ssl_handle=0x%x\n", internal->ssl);
		SSL_Close(internal->ssl, TRUE);
	}

	internal->ssl = 0;
	ctx->sslConnected = 0;
	return 0;
}

#endif

#endif
