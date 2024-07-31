//author: Justin, for watch version
#ifndef __LEBAO_API_H
#define __LEBAO_API_H

#ifdef __cplusplus
extern "C" {
#endif

#include "sds.h"
#include "link_list.h"
#include "http_client.h"

#ifndef REAL_WATCH_RTOS
#include <sys/types.h>
#endif

typedef enum
{
	LEBAO_STATUS_NO_CMCC = -200,
	LEBAO_STATUS_LOGIN_FAIL,
	LEBAO_STATUS_SEND_SMS_FAIL,
	LEBAO_STATUS_SERVER_NO_RESPONSE,
	LEBAO_STATUS_SERVER_RESPONSE_ERROR,

	LEBAO_STATUS_CAIXIN_UNKNOWN =           -10000,
	LEBAO_STATUS_CAIXIN_INVLID_REQ_PARAM =  -10001,
	LEBAO_STATUS_CAIXIN_NOT_LOGIN =         -10002,
	LEBAO_STATUS_CAIXIN_ROUTINE_EXCEPTION = -10003,
	LEBAO_STATUS_CAIXIN_INVLID_CODE =       -10004,
	LEBAO_STATUS_CAIXIN_AUTOFILE_SIZE_ZERO =    -10005,
	LEBAO_STATUS_CAIXIN_AUTOFILE_SIZE_INVALID = -10006,
	LEBAO_STATUS_CAIXIN_NEED_SVIP =         -10007,
	LEBAO_STATUS_CAIXIN_NEED_PURCHASE =     -10008,
	LEBAO_STATUS_CAIXIN_URL_ERROR =			-10101,		

} lebao_rc_error_code_t;

typedef enum
{
	LEBAO_MENU_SEARCH = 1,
	LEBAO_MENU_LOCAL,
	LEBAO_MENU_ORDER,
	LEBAO_MENU_HISTORY,
	LEBAO_MENU_ABOUT,

	LEBAO_MENU_CHART,

	LEBAO_MENU_MAX = 30,
} lebao_menu_id_t;

typedef enum
{
	LEBAO_MAINMENU_BEGIN = 0,	
	LEBAO_MAINMENU_MUSICS,
	LEBAO_MAINMENU_BOOKS,
	LEBAO_MAINMENU_MUSIC_EXT,
	LEBAO_MAINMENU_BOOK_EXT,
	LEBAO_MENU_BOOK_CONTENT,
	LEBAO_MENU_MUSIC_CONTENT,
	LEBAO_SEARCH_MUSIC_CONTENT,
} lebao_mainmenu_type_t;

typedef struct chart_info {
	unsigned char chartMenuItemType;
	unsigned char needUpdate;
	unsigned char subIndex;
	unsigned char chartMenuId;	// unused

	sds chartId;
	sds parenetId;
	sds chartName;
	sds singerName;
    sds chartPicUrl;

	sds copyrightId;	
	sds data;

	int chartType;	
	int collectStatus;
	
	void* subItemsData;
	void* control;
} chart_info_t;

typedef struct song_info {
	unsigned char chartMenuItemType;
	unsigned char needUpdate;
	unsigned char subIndex;		// unused
	unsigned char chartForm;	// unused

	sds chartId;
	sds parenetId;
	sds songName;
	sds singerName;
	sds picUrl;

	sds copyrightId;
	sds data;

	sds isHeart;
	sds contentId;
	sds ringListenUrl;
	sds ringDownloadUrl;	

	int collectStatus;
	int songType; // 0: free; 99: vip; others: purchase.
	int uriCacheTime;
	int isMusicType;
} song_info_t;

typedef struct product_info {
	sds channelCode;
	sds channelKey;
	sds defSeq;
	sds excode;
	sds info;
	sds orderType;
	sds orderUrl;

	sds secretKey;
	sds serviceId;
	sds serviceName;
	sds status;
	sds type;
	sds auth;

	sds asrType;
	int mp3Type;
} product_info_t;

typedef struct user_agreement {
	int protocolVersion;
	sds privacyProtocolUrl;
	sds privacyProtocolContent;
	sds serviceProtocolUrl;
	sds serviceProtocolContent;
} user_agreement_t;

typedef enum
{
	API_LEBAO_ASR_IFLYTEK,
	API_LEBAO_ASR_BAIDU,
	API_LEBAO_ASR_MIGU,
	API_LEBAO_ASR_MIGUDCT,
} lebao_api_asr_type_t;

void clear_chart(chart_info_t* chart);
void clear_song(song_info_t* song);
void clear_product(product_info_t* info);

void free_charts(link_list_t* charts);
void free_songs(link_list_t* songs);
void free_user_agreement(user_agreement_t * data);


void free_all_dynmanuitems(void);
link_list_t *find_subitem_by_chartId(link_list_t *list, const char *chartId);

int lebao_login(http_client_progress_t callback, void* caller);
//sds lebao_get_splash_pic(http_client_progress_t callback, void* caller);
int lebao_is_vip(http_client_progress_t callback, void* caller, const int postImsi);
//sds lebao_add_user(http_client_progress_t callback, void* caller);
link_list_t* lebao_query_chart_list(const int grade, const int pageNo, const int pageSize, http_client_progress_t callback, void* caller);
link_list_t* lebao_query_chart_music(const int type, const char* chartId, http_client_progress_t callback, void* caller);
//song_info_t* lebao_query_music_info(const char* contentId, http_client_progress_t callback, void* caller);

int lebao_download_to_file(const char* uri, const char* filename, const size_t rangeStart, const size_t exceedSize, const size_t maxSize, http_client_progress_t callback, void* caller, int* dwSize, int timeout);
int lebao_download_to_buffer(const char* uri, sds* buffer, const size_t rangeStart, const size_t exceedSize, const size_t maxSize, http_client_progress_t callback, void* caller);
int lebao_download_stream(const char* uri, const size_t rangeStart, const size_t rangeEnd, http_client_body_chunk_t dataCallback, http_client_progress_t callback, void* caller);


//sds lebao_order_product(const char* type, const product_info_t* info);
int lebao_order_by_msisdn(sds* orderId, sds* type);
sds lebao_check_order_status(const char* orderId, const char* type);
int lebao_submit_order_captcha(const char* orderId, const char* type, const char* captcha);
int lebao_query_smsinfo(http_client_progress_t callback, void* caller);

sds lebao_get_asr_token(const lebao_api_asr_type_t type, http_client_progress_t callback, void* caller);
sds lebao_post_baidu_voice(const char* uri, const char* contentType, const char* filename, const int filesize, http_client_progress_t callback, void* caller);
sds lebao_post_baidu_voice_stream(const char* uri, const char* contentType, link_list_t* stream, const int streamsize, http_client_progress_t callback, void* caller);
sds lebao_post_migu_voice(const char* contentType, const char* filename, const int filesize, http_client_progress_t callback, void* caller);
sds lebao_post_migu_voice_stream(const char* contentType, link_list_t* stream, const int streamsize, http_client_progress_t callback, void* caller);
song_info_t* lebao_query_ring_uri(const int type, const char* contentId, http_client_progress_t callback, void* caller);

user_agreement_t* lebao_get_user_agreement(http_client_progress_t callback, void* caller);

sds lebao_post_migu_asr2(const char* token, const char* contentType, const char* filename, const int filesize, http_client_progress_t callback, void* caller);
sds lebao_post_migu_asr2_stream(const char* token, const char* contentType, link_list_t* stream, const int streamsize, http_client_progress_t callback, void* caller);


int _srv_is_logging_thread_idle(void);
int lebao_caixin_login_check(http_client_progress_t callback, void* caller);
int lebao_caixin_get_smscode(http_client_progress_t callback, void* caller);
song_info_t* lebao_query_caixin_ring_url(const int type, const char* contentId, http_client_progress_t callback, void* caller, int timeout, int *limit);
int lebao_caixin_search_music_by_keyword(const char* key, const int count, http_client_progress_t callback, void* caller, link_list_t**list, int *out_num, int *remindSearchNum, int *isTourist);
link_list_t* lebao_query_favorite_music(const int type, const char* chartId, http_client_progress_t callback, void* caller);
sds lebao_post_cainxin_voice_stream(const char* contentType, link_list_t* stream, const int streamsize,	http_client_progress_t callback, void* caller);
int lebao_caixin_command_generateNum(http_client_progress_t callback, void* caller);
int lebao_caixin_command_postStartupLog(sds body, http_client_progress_t callback, void* caller);
int lebao_caixin_command_postSongLog(sds body, http_client_progress_t callback, void* caller);
int lebao_caixin_command_getPups(http_client_progress_t callback, void* caller);
int lebao_caixin_command_getMemberInfo_status(http_client_progress_t callback, void* caller, int *isVip);
int lebao_caixin_get_and_check_acInfo(http_client_progress_t callback, void* caller);
int lebao_caixin_command_collectFlow(http_client_progress_t callback, void* caller);
int lebao_caixin_get_login_qrcode(http_client_progress_t callback, void* caller);

link_list_t* lebao_caixin_query_sub_content(const int contentType, const char *chartId, http_client_progress_t callback, void* caller);

void lebao_ctrl_player_limit_msg(void);

#ifdef __cplusplus
}
#endif

#endif
