# *************************************************************************************************************************************
#  大部分公共的配置选项在 uis8910_phone_base_config.cfg 中，
#  All the common configuration is set in uis8910_phone_base_config.cfg 
# *************************************************************************************************************************************

# *************************************************************************************************************************************
#  
#  ZDT
# *************************************************************************************************************************************
ZDT_PRODUCT_VIBRATE = FALSE                                 ### ZDT_PRODUCT_VIBRATE
                                                     # Option1:TRUE
                                                     # Option2:FALSE                                          
 
ZDT_PRODUCT_Z685S= TRUE                                 ### ZDT_PRODUCT_Z685S
                                                     # Option1:TRUE
                                                     # Option2:FALSE    
                                                     
ZDT_PRODUCT_JMJ = TRUE                                 ### ZDT_PRODUCT_JMJ
                                                     # Option1:TRUE
                                                     # Option2:FALSE 
ZDT_PRODUCT_BH = TRUE                                 ### ZDT_PRODUCT_BH
                                                     # Option1:TRUE
                                                     # Option2:FALSE  
ZDT_PRODUCT_WDL = TRUE                                 ### ZDT_PRODUCT_WDL 
                                                     # Option1:TRUE
                                                     # Option2:FALSE
                                                     
                                                     
SOS_KEYLONGWEB = TRUE                                 ### SOS_KEYLONGWEB
                                                     # Option1:TRUE
                                                     # Option2:FALSE
                                                                                                        
ZDT_EDIT_NUM_IMG_SUPPORT = TRUE        			     ### ZDT_EDIT_NUM_IMG_SUPPORT
                                                     # Option1:TRUE
                                                     # Option2:FALSE 
                                                     
ZDT_PRODUCT_BT = FALSE                          ### ZDT_PRODUCT_BT
                                                     # Option1:TRUE
                                                     # Option2:FALSE                                                     
                                              
ZDT_PRODUCT_FM = FALSE                          ### ZDT_PRODUCT_FM
                                                     # Option1:TRUE
                                                     # Option2:FALSE                                                     

ZDT_PRODUCT_BROWSER = FALSE                          ### ZDT_PRODUCT_BROWSER
                                                     # Option1:TRUE
                                                     # Option2:FALSE   

PHONE_NUMBER_VOICE = TRUE                             ### SW VERSION  SUPPORT
                                                     # Option1:TRUE
                                                     # Option2:FALSE 			
                                                                                                           
MMI_MP3_FIXED_MUSIC   = TRUE                                 ### MMI_MP3_FIXED_MUSIC
                                                     # Option1:TRUE
                                                     # Option2:FALSE

ZDT_HANDSET_2IN1_SUPPORT = FALSE                          ### ZDT_PRODUCT_BROWSER
                                                     # Option1:TRUE
                                                     # Option2:FALSE 
ZDT_JMJ_B246T_CUSTOM   = FALSE    ### ZDT_PRODUCT_Z685S
								# Option1:TRUE
								# Option2:FALSE

ZDT_JMJ_B246T_MAINMENU   = FALSE	 ### ZDT_PRODUCT_Z685S
								# Option1:TRUE
								# Option2:FALSE

ZDT_JMJ_B246T_IDLE_AND_LOCKSCREEN	 = FALSE  ### ZDT_PRODUCT_Z685S
								# Option1:TRUE
								# Option2:FALSE

ZDT_JMJ_VOICE_KING	 = FALSE  ### ZDT_PRODUCT_Z685S
								# Option1:TRUE
								# Option2:FALSE

ZDT_VIDEO_PLAYER_8910	 = FALSE  ### ZDT_PRODUCT_Z685S, CAN SUPPORT SOME AVI FORMAT THAN 8910S
								# Option1:TRUE
								# Option2:FALSE
ZDT_PHILIPS_DIAL_SEARCH_STYLE = FALSE  	       ### MMI_MP3_FIXED_MUSIC


ZDT_WDL_Z62_SUPPORT = TRUE                          ### ZDT_WDL_A03ZL_SUPPORT
                                                     # Option1:TRUE
                                                     # Option2:FALSE 
ZDT_KEYLOCK_SCREEN_BIG_DAY_STYLE = TRUE                          ###
													 
ZDT_SHOW_SIM_NETWORK_RSSI_IN_FACTORY_MODE = TRUE  ### 
MMI_WHITELIST_SUPPORT  = TRUE # TRUE                       ### MMI_BLACKLIST_SUPPORT    打开了才行             

Z695_KEYLOCK_INT_SUPPORT = TRUE             ### Z695_KEYLOCK_INT_SUPPORT
ZDT_PRODUCT_Q98 = TRUE                                 ### ZDT_PRODUCT_Q98

LEBAO_MUSIC_DEVELOP_MODE = FALSE
LEBAO_MUSIC_SUPPORT = TRUE					      ## Lebao music app:   # Option1:TRUE   # Option2:FALSE
LEBAO_FEATURE_PHONE_STYLE = TRUE                          ## OLD MAN FEATURE PHONE STYLE.        # Option1:TRUE   meams phone with key   # Option2:FALSE means watch with tp
ZDT_VIDEO_KING_SUPPORT	= TRUE	 ### ZDT_VIDEO_KING_SUPPORT

ZDT_MUSIC_KING_SUPPORT = TRUE	 ### ZDT_MUSIC_KING_SUPPORT
#仅支持4G,删除2G,去掉--网络类型-菜单-4G 2G 自动切换等
MMI_ONLY_4G_DEL_2G_CUSTOM = TRUE

############################################HERO######################################################
HERO_ENGINE_SUPPORT=TRUE # 平台总宏

ifeq ($(strip $(HERO_ENGINE_SUPPORT)),TRUE)

HERO_ENGINE_SUPPORT_CLIENT_INFO=TRUE  #支持客户自定义集成商+机型的配置宏
      ifeq ($(strip $(HERO_ENGINE_SUPPORT_CLIENT_INFO)),TRUE)
      ##/*集成商最多3个小写字符,切记不可超过字符个数限制*/
	HERO_ENGINE_CFG_VAR_HANDSETID_FIRST =  wdl
      ##/*机型标识，最多4个小写字符,切记不可超过字符个数限制*/
	HERO_ENGINE_CFG_VAR_HANDSETID_SEC   = 62zl
      endif

HERO_APP_READER_OPEN=FALSE 		#电子书
HERO_APP_WEATHER_OPEN=TRUE 		#天气预报
HERO_ENGINE_HMGS_SUPPORT= FULL_ROM	#来电归属地 NONE,MIN_ROM,FULL_ROM,TCARD

################## 语音王#####
HERO_ENGINE_TTS_SUPPORT= TRUE
HERO_ENGINE_TTS_INROM_SUPPORT= TRUE
HERO_ENGINE_TTS_ALL_SUPPORT= TRUE
HERO_ENGINE_TTS_AKEY_TIME = KEY_0_TIME #一键报时 KEY_7_TIME,KEY_8_TIME,KEY_9_TIME,KEY_0_TIME,KEY_STAR_TIME
HERO_ENGINE_TTS_SWTICH_SUPPORT = TRUE  ##
HERO_ENGINE_TTS_TIME_BROADCAST_NO_WEEK_LUNAR = TRUE  ###
HERO_ENGINE_TTS_AKEY_TIME_FOR_SUPPORT_SOLARTOLUNAR = TRUE  ###
__TIHO_TTS_REMOVE_TIME_DESCRIPTION__ = TRUE 
################## 语音王#####
endif
############################################HERO######################################################
	   
# *************************************************************************************************************************************
#  ZDT ADD END
# *************************************************************************************************************************************    
MMI_MULTI_SIM_SYS = DUAL                             ### Platform Support Multi Sim Card Number
                                                     # Option1:SINGLE    #define MMI_MULTI_SIM_SYS_SINGLE
                                                     # Option2:DUAL      #define MMI_MULTI_SIM_SYS_DUAL
													 
#zmt add start
MATH_COUNT_SUPPORT = TRUE 			#口算达人
FORMULA_SUPPORT = TRUE				#乘法口诀
WORD_CARD_SUPPORT = TRUE			#单词卡
HANZI_CARD_SUPPORT = TRUE			#汉字卡
ZMT_PINYIN_SUPPORT = TRUE			#拼音讲解
ZMT_YINBIAO_SUPPORT = TRUE			#音标讲解
POETRY_LISTEN_SUPPORT = TRUE		#古诗词
ZDT_HTTP_APP_SUPPORT = TRUE
#zmt add end													 
													 
include uis8910_phone_user_base_config.cfg

# *************************************************************************************************************************************
# 如果有差异的话，需要从uis8910_phone_base_config.cfg提取出来，放到各自project_xxx.mk的这段注释的后面，
# 注意：
#    (1)目前不支持有重复宏定义，比如SRAM_SIZE = 128MBIT，然后后面有出现一处SRAM_SIZE = 256MBIT，两种不同的定义。
#    (2)这段注释的后面的宏定义不能和uis8910_phone_base_config.cfg 中的重复，如果有重复需要从uis8910_phone_base_config.cfg 中拿出来。
#    (3)RVCT_VERSION = RVCT41 或者 RVCT_VERSION = V504 不再出现在project_xxx.mk中，需要编译的时候手动在CMD中设置set CC_VER=RVCT41
#       或者set CC_VER=V504，如果不设置的话，在makefile.rules中会默认设置成RVCT_VERSION = RVCT41。
# Notice:
#    (1). Do not support multi-definition of one micro, such as define SRAM_SIZE = 128MBIT at first, then define SRAM_SIZE = 256MBIT in the end.
#    (2). The micro delow this comments should not re-defined with the micros in uis8910_phone_base_config.cfg. If there happened re-defined, you
#         should seprate the micro from  uis8910_phone_base_config.cfg and add it to individual project_xx.mk
#    (3). The difinition of RVCT_VERSION is not set anymore. You should set environment value in CMD terminator, such as set CC_VER=V504.
#         If you do not set CC_VER , the default value RVCT_VERSION = RVCT41 will be set in makefile.rules.
# *************************************************************************************************************************************

FLASH_SIZE = 128MBIT                                 ### FLASH_SIZE SIZE
                                                     # Option1:16MBIT     #define FLASH_SIZE_16MBIT
                                                     # Option2:32MBIT     #define FLASH_SIZE_32MBIT
                                                     # Option3:64MBIT     #define FLASH_SIZE_64MBIT
                                                     # Option4:128MBIT    #define FLASH_SIZE_128MBIT
						                             # Option5:128MBITX64MBIT    #define FLASH_SIZE_128MBITX64MBIT_OLD
													 # Option6:128MBITX64MBIT_NEW #define FLASH_SIZE_128MBITX64MBIT_NEW
											 
MULTI_FLASH_SUPPORT = FALSE													 
													 
													 
                                                     
SRAM_SIZE = 128MBIT                                   ### SRAM SIZE
                                                     # Option1:16MBIT     #define SRAM_SIZE_16MBIT
                                                     # Option2:32MBIT     #define SRAM_SIZE_32MBIT
                                                     # Option3:64MBIT     #define SRAM_SIZE_64MBIT
                                                     # Option4:128MBIT     #define SRAM_SIZE_128MBIT
                                                     # Option4:256MBIT     #define SRAM_SIZE_256MBIT
													 
AP_CP_RAM_SIZE = 40X88                               ##APXCP RAM SIZE 
                                                     # Option1:40MBITX88MBIT     #define AP_CP_RAM_SIZE_40X88
													 # Option1:24MBITX104MBIT     #define AP_CP_RAM_SIZE_24X104
                                                   

_USE_3_LINE_2_DATD_ = FALSE                          ### lcd 3 line 2 date support
                                                     # Option2:FALSE
													 
													 
PRODUCT_CONFIG = uis8910ff_refphone                   ### MS_Customize/source/product/config;scf file;nv;resource
                                                     # default: project name 

PRODUCT_BASELIB_DIR = UIS8910_ROM_16MB_DS_USER_Z62ZL           ### Dir name for base lib in MS_Code\lib\
                                                     # default: project name 
                                                     

													 
BT_LOG_MENU_SUPPORT = FALSE                          ### BT LOG Menu Select Support
                                                     # Option1:TRUE
                                                     # Option2:FALSE 
                                                     
                                                     
PLATFORM = UIX8910                                    ### Chip Platform Definition:
                                                     # Option1:SC6530   #define PLATFORM_SC6530
                                                     # Option2:SC6531EFM   #define PLATFORM_SC6531EFM
                                                     # Option2:UIX8910   #define PLATFORM_UIX8910
                                                     
CHIP_VER = UIS8910FF                                  ### Chip version:
                                                     # Option1:6800H1     #define CHIP_VER_6800H1
                                                     # Option2:6800H3     #define CHIP_VER_6800H3
	                                                 # Option3:6531EFM    #define CHIP_VER_6531EFM												 
                                                     # Option4:UIX8910MPW    #define CHIP_VER_UIX8910MPW
													 # Option5:UIX8910    #define CHIP_VER_UIX8910
													 # Option5:UIS8910C    #define CHIP_VER_UIS8910C
                                                     # Option6:UIS8910FF    #define CHIP_VER_UIS8910FF
													 
MODEM_BIN_FILE = UIX8910                             ## modem bin cache file
												     # Option1:UIX8910MPW
												     # Option2:UIX8910_FPGA
												     # Option3:UIX8910														 

FM_S_ANT_SUPPORT = FALSE                              ###
                                                     # Option0:TRUE     
                                                     # Option1:FALSE                                                    

						     													 
BZP_NO_RESOURCE = TRUE								## resource not zip when bzp_support
													# Option1:TRUE
													# Option1:FALSE                                                

BT_PBAP_SUPPORT = FALSE                              ### Switch Of PBAP Profile Support
                                                     # Option1:TRUE
                                                     # Option2:FALSE
													 
AUTOTEST_DEVICE_SUPPORT = TRUE                      ### AUTOTEST_DEVICE_SUPPORT
                                                     # Option1:TRUE
                                                     # Option2:FALSE
													 
UMEM_SUPPORT = TRUE                                 ### Switch Of nor flash udisk support
                                                     # Option1:TRUE
                                                     # Option2:FALSE	

UDISK_PC_CONNECT = TRUE                             ### Norflash disk pc access support UMEM_SUPPORT MUST set TRUE
                                                     # Option1:TRUE
                                                     # Option2:FALSE     
													 
SIM_LOCK_SUPPORT = FALSE                             ### SIM Lock Support 
                                                     # Option1:TRUE
                                                     # Option2:FALSE													 
													
SIMULATE_FLASH2_SUPPORT = FALSE

CHIP_ECO_SUPPORT = TRUE

POP_MDY_SUPPORT = TRUE

SECURE_BOOT_SUPPORT = FALSE			     ### Secure Boot Support
                                                     # Option1:TRUE
                                                     # Option2:FALSE

SECURITY_DATA_SUPPORT = FALSE			     ### Security Data Support
                                                     # Option1:TRUE
                                                     # Option2:FALSE

VIRTUAL_VIBRATE_FUNC = FALSE                         ### VIRTUAL VIBRATE FUNCTION
                                                     # Option0:TRUE     
                                                     # Option1:FALSE 


RF_BAND_SELFADAPTIVE = FALSE                    		### OPEN/CLOSE RF BAND SELFADAPTIVE FUNCTION
													# Option0:TRUE     
													# Option1:FALSE 
CHGMNG_PSE_SUPPORT = FALSE                    		### OPEN/CLOSE CHGMNG_PSE_SUPPORT
													# Option0:TRUE     

DEBUG_VERSION  = TRUE                                ### DEBUG_VERSION:
                                                     # Option1:TRUE
                                                     # Option2:FALSE


RELEASE_FLAG = FALSE                                  ### RELEASE_FLAG
                                                     # Option1:TRUE
                                                     # Option2:FALSE 


                                                     
BZP_SUPPORT  = FALSE                                  ###new memory management 
                                                     # Option1:TRUE
                                                     # Option2:FALSE 	
MODEM_BIN_FILE = MODEM_DEBUG													 
													 
BLUETOOTH_SUPPORT = NONE                             ### Bluetooth Chip Type: 
                                                     # Option1:NONE      #define BLUETOOTH_SUPPORT_NONE
                                                     # Option2:CSR_BC4_6 #define BLUETOOTH_SUPPORT_CRS_BC4_6
                                                     # Option3:CSR_BC4_6_LOW_MEM: low mem and code size , only support OPP HFG
                                                     # Option4:RTL8723   Realtek RTL8723AS wifi/bt combo chip
                                                     # Option5:IS1662    ISSC IS1662S bt chip
                                                     # Option6:SPRD_BT			#define BLUETOOTH_SUPPORT_SPRD_BT

BROWSER_SUPPORT = FALSE                              ### Browser SW Supplier:
                                                     # Option1:TRUE
                                                     # Option2:FALSE 

BROWSER_SUPPORT_DORADO = FALSE                        ### dorado Browser support:
                                                     # Option1:TRUE
                                                     # Option2:FALSE

BROWSER_OPERA_START_PAGE = FALSE                     ### Browser SW Supplier:
                                                     # Option1:TRUE
                                                     # Option2:FALSE

OPERA_MINI_SUPPORT = NONE                            #opera mini support                                                     
                                                     # Option1:VER42    OPERA_MINI_SUPPORT_VER42   建议使用VER42
                                                     # Option2:NONE     OPERA_MINI_SUPPORT_NONE   
                                                     
DL_SUPPORT = FALSE                                   ###Download Manager Supplier:
                                                     # Option1:TRUE
                                                     # Option2:FALSE

IM_ENGINE = CSTAR                                    ### Im Engine Support:
                                                     # Option1:CSTAR         #define IM_ENGINE_CSTAR
                                                     # Option2:T9            #define IM_ENGINE_T9
                                                     # Option3:IEKIE         #define IM_ENGINE_IEKIE(MUST use spml display engine)
                                                     # Optino4:NONE          #define IM_ENGINE_NONE

IM_SIMP_CHINESE_SUPPORT	= TRUE                       ### SIMPLE CHINESE SUPPORT:
                                                     # Option1:TRUE
                                                     # Option2:FALSE
													 
MMI_DISPLAY_SIMP_CHINESE_SUPPORT = TRUE              ### DISPLAY SIMPLE CHINESE SUPPORT:
                                                     # Option1:TRUE
                                                     # Option2:FALSE

IM_TRAD_CHINESE_SUPPORT = FALSE                      ### TRAD CHINESE SUPPORT:
                                                     # Option1:TRUE
                                                     # Option2:FALSE

MMI_DISPLAY_TRAD_CHINESE_SUPPORT = FALSE             ### DISPLAY TRAD CHINESE SUPPORT:
                                                     # Option1:TRUE
                                                     # Option2:FALSE

MODEM_BIN_FILE = UIS8910_ROM_16MB_DS_USER_Z62ZL