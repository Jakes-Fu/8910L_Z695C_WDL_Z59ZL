#ifndef __LEBAO_CONFIG_H
#define __LEBAO_CONFIG_H

#ifdef __cplusplus
extern "C" {
#endif

#include "sds.h"
#include "link_list.h"

#define ENABLE_FEATURE_HISTORY		1

#define CHARTS_MAX_HOME_ITEMS_NUM       50
#define CHARTS_MAX_MUSIC_ITEMS_NUM      100

typedef struct lebao_config {
	sds channel;
	sds key;
	sds accessToken;
	
	sds imei;
	sds msisdn;			// local sim
	sds imsi;			// local sim
	sds pimsi;			// userInfo.pimsi
	sds pmsisdn;		// userInfo.pmsisdn
	sds virtualMsisdn;	// userInfo.msisdn
	sds lastImsi;		// userInfo.imsi
	
	sds validTime;		// userInfo.validTime
	sds memberName;		// memberName
	sds memberOptionsTitle[8];
	sds memberOptionsDesc[8];
	int memberOptionsValid;

	sds rootPath;
	sds cachePath;
	sds playlistFilename;
	sds savePath;
	sds recordFile;

	int maxCycle;
	int showImage;
	int maxFiles;

	int maxSearch;
	int maxDaySearch;

	sds timestamp;
	sds nowDay;
	int mySearch;
	int myDaySearch;

	int spaceMinSize;
	int imageMaxSize;

	int mp3MaxBufferSize;
	int mp3MaxFileSize;
	int mp3MinFileSize;
	int mp3MaxCacheSize;
	int mp3ChunkSize;
	int mp3MaxSaveCount;
	int mp3MaxFavorites;
	int listMaxCount;

	sds smsCalled;
	sds smsCode;
	sds captcha;
	sds ivrCalled;

	int agreement;
	int imsiChanged;

	/* add for caixin */
	int acSignIndex;
	sds acSign;
	sds acSignName;
	sds serviceUrl;
	sds loginUrl;
	int giftInfoLastTime;

	int firstReg;	
	int historyLogin;	// 1: 快速登录		
	int first_check_vip_result;		// 保存第一次检查 vip 的结果.
	int guest_mode;

	int cmccUser;
	unsigned int giftCheckTime;

	int hiddenRingButtonConfig;	// getHiddenRingButtonConfig

	int playMaxCount;			// getButtonConfig
	int hiddenAlarmButtonConfig;
	sds searchTips;
	sds favTips;

	int aboutCount;
	sds aboutUrl[8];
	sds aboutMenu[8];
} lebao_config_t;

enum
{
	CX_SERVER_TIP_STR_BEGIN = 0,
	CX_SERVER_TIP_STR_TRAFFIC_WARN = CX_SERVER_TIP_STR_BEGIN,
	CX_SERVER_TIP_STR_PRIVACY,
	CX_SERVER_TIP_STR_ONEKEY_LOGIN,
	CX_SERVER_TIP_STR_SNED_CAPTCHA,
	CX_SERVER_TIP_STR_ORDER_VIP,	
	CX_SERVER_TIP_STR_VIP_SERVICE,
	CX_SERVER_TIP_STR_CLOSE_ACCOUNT,	// 7
	CX_SERVER_TIP_STR_LOGOUT,			// 8
	CX_SERVER_TIP_STR_CLR_CACHE,		// 9
	CX_SERVER_TIP_STR_GIFT_INFO,		// 9
	CX_SERVER_TIP_STR_SKIP_GIFT,		// 10
	CX_SERVER_TIP_STR_GUEST_LOGIN,		// 11
	CX_SERVER_TIP_STR_LONGIN_PHONE_INPUT, // 12, seq=17
	CX_SERVER_TIP_STR_END
};
extern sds _tipServerDescUtf8Arr[CX_SERVER_TIP_STR_END * 2];
extern int _tipServerDescFlag[CX_SERVER_TIP_STR_END];

/* add for caixin */
extern unsigned char gLebaoCaixinSupport;
extern unsigned char gLebaoCaixinSmsFilter;
extern sds gLebaoCainxinAppStartTime;
extern unsigned int gLebaoCainxinAppStartTimeValue;
extern link_list_t *glebaoDynMenuItems;

void lebao_set_key_config(const char * id, const char * key);

void lebao_set_account_type_config(int type);
char* lebao_get_account(void); // const

lebao_config_t* lebao_get_config(void);
int 	lebao_load_config(void);
int 	lebao_save_config(const char* filename);
int 	lebao_free_config(void);
void	lebao_delete_config_files(void);

sds lebao_get_ring_mp3_path(const char * id);
sds lebao_get_img_cache_path(const char * filename);
sds lebao_get_alarm_mp3_path(const char * id);
int lebao_player_music_is_exist_alarm(const char * id);
void lebao_clean_alarm_file(const char *exclude);

#ifdef __cplusplus
}
#endif

#endif // __LEBAO_CONFIG_H
