include Makefile.rules
include ./project_$(PROJECT).mk
include Makefile.verify
include Makefile.rules

#
#  project config
#

ifeq ($(strip $(SOUNDBANK_VER)), ARM_300KB)
  IN_SOUNDBANK_FILE := MS_Ref/source/soundbank/soundbank_22050_alaw8bits_dy_300k.sf2
  SOUNTBANK_BYTES_PER_SAMPLE := 1
endif

ifeq ($(strip $(SOUNDBANK_VER)), ARM_256KB)
  IN_SOUNDBANK_FILE := MS_Ref/source/soundbank/soundbank_dy_alaw8bit_128k_little.sf2
  SOUNTBANK_BYTES_PER_SAMPLE := 1
endif

ifeq ($(strip $(SOUNDBANK_VER)), ARM_700KB)
  IN_SOUNDBANK_FILE := MS_Ref/source/soundbank/soundbank_22050_alaw8bits_700k_little.sf2
  SOUNTBANK_BYTES_PER_SAMPLE := 1
endif

ifeq ($(strip $(SOUNDBANK_VER)), ARM_2MB)
  IN_SOUNDBANK_FILE := MS_Ref/source/soundbank/soundbank_44100_16bit_2M_little.sf2
  SOUNTBANK_BYTES_PER_SAMPLE := 2
endif

MMI_RES_DIR := $(strip $(MMI_RES_DIR))

#define resource replace flag
REP_FLAG := FALSE
ifeq ($(strip $(SPRD_CUSTOMER)), TRUE)
REP_FLAG := FALSE
endif

#resource all use unicode
ifeq ($(strip $(MMI_RES_TEXT_ALL_UNICODE_SUPPORT)), TRUE)
ALU_FLAG := TRUE
else
ALU_FLAG := FALSE
endif

#Remove Cross RepeatString
ifeq ($(strip $(NANDBOOT_SUPPORT)), TRUE)
CRM_FLAG := FALSE
else
CRM_FLAG := TRUE
endif

ifneq ($(strip $(VARIANT_PID)),)
VARIANT_LP := AA
VARIANT_PC := 56

VARIANT_LP := $(word 1,$(subst _, ,$(VARIANT_PID)))

VARIANT_PC := $(word 2,$(subst _, ,$(VARIANT_PID)))
endif



ifeq ($(strip $(VIDEO_PLAYER_SUPPORT)), TRUE)
SOURCES	 += videoplayer_mdu_def.h
#else
#SOURCES	 += mpeg4_mdu_def.h
endif

ifeq ($(strip $(STREAMING_SUPPORT)), TRUE)
ifeq ($(strip $(MOBILE_VIDEO_SUPPORT)), TRUE)
SOURCES += mobile_video_mdu_def.h
endif
endif
SOURCES	 += common_mdu_def.h tools_mdu_def.h clock_mdu_def.h bt_mdu_def.h camera_mdu_def.h cc_mdu_def.h ussd_mdu_def.h\
 connection_mdu_def.h dyna_mdu_def.h ebook_mdu_def.h engtd_mdu_def.h eng_mdu_def.h files_manager_mdu_def.h drm_mdu_def.h fm_mdu_def.h \
 game_boxman_mdu_def.h game_quintet_mdu_def.h game_link_mdu_def.h game_manager_mdu_def.h gps_mdu_def.h im_mdu_def.h \
 game_snake_mdu_def.h  \
 kuro_mdu_def.h mms_mdu_def.h audioplayer_mdu_def.h  multim_mdu_def.h \
 ocr_mdu_def.h pb_mdu_def.h pic_mdu_def.h pic_viewer_mdu_def.h record_mdu_def.h set_mdu_def.h sms_mdu_def.h stk_mdu_def.h \
 tp_mdu_def.h vt_mdu_def.h mobiletv_mdu_def.h sample_mdu_def.h otapush_mdu_def.h\
 dcd_mdu_def.h cs_mdu_def.h www_mdu_def.h dl_mdu_def.h  pim_mdu_def.h qq_mdu_def.h clipbrd_mdu_def.h search_mdu_def.h  asp_mdu_def.h\
 widget_mdu_def.h atv_mdu_def.h memo_mdu_def.h sns_mdu_def.h

ifeq ($(strip $(LEBAO_MUSIC_SUPPORT)), TRUE)
SRCPATH  += $(MMI_DIR)/source/mmi_app/app/lebao/h
SOURCES	 += lebao_mdu_def.h
endif

##zmt add start
ifeq ($(strip $(MATH_COUNT_SUPPORT)), TRUE)
SOURCES	 += math_count_mdu_def.h 
SRCPATH += $(MMI_DIR)/source/mmi_app/app/math_count/h
endif
ifeq ($(strip $(FORMULA_SUPPORT)), TRUE)
SRCPATH  += $(MMI_DIR)/source/mmi_app/app/formula/h
SOURCES	 += formula_mdu_def.h
endif
ifeq ($(strip $(WORD_CARD_SUPPORT)), TRUE)
SOURCES	 += zmt_word_mdu_def.h 
SRCPATH += $(MMI_DIR)/source/mmi_app/app/zmt_word/h
endif
ifeq ($(strip $(HANZI_CARD_SUPPORT)), TRUE)
SOURCES	 += zmt_hanzi_mdu_def.h 
SRCPATH += $(MMI_DIR)/source/mmi_app/app/zmt_hanzi/h
endif
ifeq ($(strip $(ZMT_PINYIN_SUPPORT)), TRUE)
SOURCES	 += zmt_pinyin_mdu_def.h 
SRCPATH += $(MMI_DIR)/source/mmi_app/app/zmt_pinyin/h
endif
ifeq ($(strip $(ZMT_YINBIAO_SUPPORT)), TRUE)
SOURCES	 += zmt_yinbiao_mdu_def.h
SRCPATH += $(MMI_DIR)/source/mmi_app/app/zmt_yinbiao/h
endif
ifeq ($(strip $(POETRY_LISTEN_SUPPORT)), TRUE)
SOURCES	 += zmt_poetry_mdu_def.h 
SRCPATH += $(MMI_DIR)/source/mmi_app/app/zmt_poetry/h
endif
ifeq ($(strip $(ZDT_HTTP_APP_SUPPORT)), TRUE)
SRCPATH  += $(MMI_DIR)/source/mmi_app/app/zdthttp/h
SOURCES	 += zdthttp_mdu_def.h
endif
##zmt add end

ifeq ($(strip $(MMIPB_SYNC_WITH_O365)), TRUE)
SOURCES	 += contactsync_mdu_def.h
endif

ifneq ($(strip $(JAVA_SUPPORT)), NONE)
SOURCES	 += java_mdu_def.h 
endif

ifeq ($(strip $(PUSH_EMAIL_SUPPORT)), TRUE)
SOURCES	 += push_mail_mdu_def.h
endif

ifneq ($(strip $(WIFI_SUPPORT)), NONE)
SOURCES	 += wifi_mdu_def.h
endif

ifeq ($(strip $(QBTHEME_SUPPORT)), TRUE)
SOURCES	 += qbtheme_mdu_def.h
endif

ifeq ($(strip $(WRE_SUPPORT)), TRUE)
SOURCES	 += wre_boot_mdu_def.h
endif

ifeq ($(strip $(IKEYBACKUP_SUPPORT)), TRUE)
SOURCES	 += ikeybackup_mdu_def.h
endif

ifeq ($(strip $(WEATHER_SUPPORT)), TRUE)
SOURCES	 += weather_mdu_def.h
endif

ifeq ($(strip $(OAUTH_SUPPORT)), TRUE)
SOURCES	 += netproxy_mdu_def.h
endif

ifeq ($(strip $(KING_MOVIE_SUPPORT)), ALL)
SOURCES	 += kmovie_mdu_def.h
else
ifeq ($(strip $(KING_MOVIE_SUPPORT)), EXTEND)
SOURCES	 += kmovie_mdu_def.h
else
ifeq ($(strip $(KING_MOVIE_SUPPORT)), STAND)
SOURCES	 += kmovie_mdu_def.h
endif
endif
endif

ifeq ($(strip $(PIC_CROP_SUPPORT)), TRUE)
SOURCES	 += pic_crop_mdu_def.h
SRCPATH  += $(MMI_DIR)/source/mmi_app/app/pic_crop/h
endif

ifeq ($(strip $(PDA_UI_DROPDOWN_WIN)), TRUE) 
SOURCES	 += dropdownwin_mdu_def.h
endif

ifeq ($(strip $(CSC_SUPPORT)),TRUE)
SOURCES	 += customer_mdu_def.h
endif
ifeq ($(strip $(MMIEMAIL_SUPPORT)),TRUE)
SOURCES	 += email_mdu_def.h
endif 
ifeq ($(strip $(SXH_APPLE_SUPPORT)),TRUE)
	MCFLAG_OPT		+= -DSXH_APPLE_SUPPORT
#	MCFLAG_OPT		+= -DSXH_APPLE_3GVEDIO_SUPPORT
	SOURCES		+= apple_mdu_def.h
endif
PROJECTS = mmi_res_prj_def.h



ifeq ($(strip $(VIDEO_PLAYER_SUPPORT)), TRUE)
SRCPATH += $(MMI_DIR)/source/mmi_app/app/videoplayer/h
#else
#SRCPATH += $(MMI_DIR)/source/mmi_app/app/mpeg4/h
endif

ifeq ($(strip $(JAVA_SUPPORT)), MYRIAD)
SRCPATH += $(MMI_DIR)/source/mmi_app/app/myriad_java/mmijava/h
endif

ifeq ($(strip $(JAVA_SUPPORT)), IA)
SRCPATH += $(MMI_DIR)/source/mmi_app/app/ia_java/h/jas
endif

ifeq ($(strip $(JAVA_SUPPORT)), SUN)
SRCPATH += $(MMI_DIR)/source/mmi_app/app/java/h
endif

ifeq ($(strip $(STREAMING_SUPPORT)), TRUE)
ifeq ($(strip $(MOBILE_VIDEO_SUPPORT)), TRUE)
SRCPATH += $(MMI_DIR)/source/mmi_app/app/mobile_video/h
endif
endif

ifeq ($(strip $(MCARE_V31_SUPPORT)),TRUE)
SOURCES	 += TencentMcareV31_mdu_def.h
SRCPATH += $(MMI_DIR)/source/mmi_app/app/TencentMcareV31/inc

endif

SRCPATH += $(MMI_DIR)/source/mmi_app/app/mms/h $(MMI_DIR)/source/mmi_app/app/audio/h $(MMI_DIR)/source/mmi_app/app/accessory/h \
 $(MMI_DIR)/source/mmi_app/app/cc/h $(MMI_DIR)/source/mmi_app/app/cl/h $(MMI_DIR)/source/mmi_app/common/h $(MMI_DIR)/source/mmi_app/app/ussd/h\
 $(MMI_DIR)/source/mmi_app/app/mainmenu/h $(MMI_DIR)/source/mmi_app/app/multim/h $(MMI_DIR)/source/mmi_app/app/pb/h \
 $(MMI_DIR)/source/mmi_app/app/phone/h $(MMI_DIR)/source/mmi_app/app/setting/h $(MMI_DIR)/source/mmi_app/app/sms/h \
 $(MMI_DIR)/source/mmi_app/app/ss/h $(MMI_DIR)/source/mmi_app/app/stk/h \ $(MMI_DIR)/source/mmi_app/app/www/h \
 $(MMI_DIR)/source/mmi_app/app/eng/h $(MMI_DIR)/source/mmi_app/app/mb/h \ $(MMI_DIR)/source/mmi_app/app/dl/h \
 $(MMI_DIR)/source/mmi_app/app/envset/h $(MMI_DIR)/source/mmi_app/app/msd/h  \
 $(MMI_DIR)/source/mmi_app/app/udisk/h $(MMI_DIR)/source/mmi_app/app/audioplayer/h $(MMI_DIR)/source/mmi_app/app/record/h \
 $(MMI_DIR)/source/mmi_app/app/fm/h \
 $(MMI_DIR)/source/mmi_app/app/ebook/h $(MMI_DIR)/source/mmi_app/app/game/game_boxman/h $(MMI_DIR)/source/mmi_app/app/camera/h \
 $(MMI_DIR)/source/mmi_app/app/game/game_snake/h \
 $(MMI_DIR)/source/mmi_app/app/game/game_link/h $(MMI_DIR)/source/mmi_app/app/game/game_manager/h \
 $(MMI_DIR)/source/mmi_app/app/game/game_quintet/h \
 $(MMI_DIR)/source/mmi_app/app/idle/h $(MMI_DIR)/source/mmi_app/app/connection/h \
 $(MMI_DIR)/source/mmi_app/app/im/h $(MMI_DIR)/source/mmi_app/app/vt/h $(MMI_DIR)/source/mmi_app/app/engtd/h \
 $(MMI_DIR)/source/mmi_app/app/block/h $(MMI_DIR)/source/mmi_app/app/pic/h \
 $(MMI_DIR)/source/mmi_app/app/vcard/h $(MMI_DIR)/source/mmi_app/app/fmm/h \
 $(MMI_DIR)/source/mmi_app/app/drm/h \
 $(MMI_DIR)/source/mmi_app/app/im/im_cstar/h \
 $(MMI_DIR)/source/mmi_app/app/im/im_t9/h \
 $(MMI_DIR)/source/mmi_app/app/gps/h \
 $(MMI_DIR)/source/mmi_app/app/tp/h $(MMI_DIR)/source/mmi_app/app/ocr/h $(MMI_DIR)/source/mmi_app/app/dynamic/h \
 $(MMI_DIR)/source/mmi_app/app/bt/h \
 $(MMI_DIR)/source/mmi_app/app/theme/h \
 $(MMI_DIR)/source/mmi_app/app/kuro/h \
 $(MMI_DIR)/source/mmi_app/app/pic_viewer/h \
 $(MMI_DIR)/source/mmi_app/app/mobiletv/h \
 $(MMI_DIR)/source/mmi_app/app/dcd/h \
 $(MMI_DIR)/source/mmi_app/app/cs/h \
 $(MMI_DIR)/source/mmi_app/app/sample/h \
 $(MMI_DIR)/source/mmi_app/app/otapush/h \
 $(MMI_DIR)/source/mmi_app/app/pim/h \
 $(MMI_DIR)/source/mmi_app/app/clipbrd/h \
 $(MMI_DIR)/source/mmi_app/app/asp/h \
 $(MMI_DIR)/source/pim/h \
 $(MMI_DIR)/source/mmi_app/app/search/h \
 $(MMI_DIR)/source/mmi_app/app/qq/h \
 $(MMI_DIR)/source/mmi_app/app/atv/h \
 $(MMI_DIR)/source/mmi_app/app/widget/h \
 $(MMI_DIR)/source/mmi_app/app/memo/h\
 $(MMI_DIR)/source/mmi_app/app/sns/h\
 $(MMI_DIR)/source/mmi_app/app/appsample/h\
 $(MMI_DIR)/source/resource \
 $(MMI_DIR)/source/resource/$(MMI_RES_DIR)

 
ifeq ($(strip $(BROWSER_SUPPORT)), TRUE)
SRCPATH += $(MMI_DIR)/source/mmi_app/app/browser/h $(MMI_DIR)/source/mmi_app/app/browser_manager/h
SOURCES += browser_mdu_def.h browser_manager_mdu_def.h 
endif


ifeq ($(strip $(BROWSER_SUPPORT_WEBKIT)), TRUE)
SRCPATH += $(MMI_DIR)/source/mmi_app/app/webkit/h
SOURCES	+= webkit_mdu_def.h
endif

ifeq ($(strip $(MMIPB_SYNC_WITH_O365)), TRUE)
SRCPATH += $(MMI_DIR)/source/mmi_app/app/contactsync/h
endif

ifeq ($(strip $(PUSH_EMAIL_SUPPORT)), TRUE)
SRCPATH += $(MMI_DIR)/source/mmi_app/app/push_mail/h
endif

ifeq ($(strip $(KING_MOVIE_SUPPORT)), ALL)
SRCPATH += $(MMI_DIR)/source/mmi_app/app/kmovie/h
else
ifeq ($(strip $(KING_MOVIE_SUPPORT)), EXTEND)
SRCPATH += $(MMI_DIR)/source/mmi_app/app/kmovie/h
else
ifeq ($(strip $(KING_MOVIE_SUPPORT)), STAND)
SRCPATH += $(MMI_DIR)/source/mmi_app/app/kmovie/h
endif
endif
endif

ifeq ($(strip $(AUTODEMO_SUPPORT)), TRUE)
SRCPATH += $(MMI_DIR)/source/mmi_app/app/autodemo/h
SOURCES += autodemo_mdu_def.h
endif


ifeq ($(strip $(WEATHER_SUPPORT)), TRUE)
SRCPATH += $(MMI_DIR)/source/mmi_app/app/weather/h
endif

ifeq ($(strip $(OAUTH_SUPPORT)), TRUE)
SRCPATH += $(MMI_DIR)/source/mmi_app/app/netproxy/h
endif

ifeq ($(strip $(PDA_UI_DROPDOWN_WIN)), TRUE) 
SRCPATH += $(MMI_DIR)/source/mmi_app/app/dropdownwin/h
endif
#ifeq ($(strip $(PDA_UI_SUPPORT)), ANDROID)


ifneq ($(strip $(WIFI_SUPPORT)), NONE)
SRCPATH += $(MMI_DIR)/source/mmi_app/app/wlan/h
endif

ifeq ($(strip $(QBTHEME_SUPPORT)), TRUE)
SRCPATH += $(MMI_DIR)/source/mmi_app/app/qbtheme/h
endif
ifeq ($(strip $(MET_MEX_SUPPORT)), TRUE)
SOURCES	 += met_mex_mdu_def.h
SRCPATH  += $(MMI_DIR)/source/mmi_app/app/met_mex/h
endif

ifeq ($(strip $(WRE_SUPPORT)), TRUE)
SRCPATH += $(MMI_DIR)/source/mmi_app/app/wre_boot/h
endif

ifeq ($(strip $(IKEYBACKUP_SUPPORT)), TRUE)
SRCPATH += $(MMI_DIR)/source/mmi_app/app/ikeybackup/h
endif

SRCPATH += $(MMI_DIR)\source\mmi_app\app\wallpaper\h
SOURCES	 += wallpaper_mdu_def.h

ifeq ($(strip $(LIVE_WALLPAPER_FRAMEWORK_SUPPORT)), TRUE)
SRCPATH += $(MMI_DIR)\source\mmi_app\app\livewallpaper\h
SOURCES	 += livewallpaper_mdu_def.h
endif
#add by zack@20110117 for sky start
ifeq ($(strip $(MRAPP_SUPPORT)), TRUE)
SOURCES += mrapp_mdu_def.h
SRCPATH += $(MMI_DIR)/source/mmi_app/app/mrapp/h
endif
#add by zack@20110117 for sky end
ifeq ($(strip $(HERO_ENGINE_SUPPORT)), TRUE)
SOURCES += heroengine_mdu_def.h
SRCPATH += $(MMI_DIR)/source/mmi_app/app/heroengine/h

SOURCES += heroapp_mdu_def.h
SRCPATH += $(MMI_DIR)/source/mmi_app/app/heroapp/h
endif

ifeq ($(strip $(MMIEMAIL_SUPPORT)),TRUE)
SRCPATH += $(MMI_DIR)/source/mmi_app/app/email/h
endif

ifeq ($(strip $(OPERA_MINI_SUPPORT)), VER42)
SOURCES	 += operamini_mdu_def.h
SRCPATH += $(MMI_DIR)/source/mmi_app/app/operamini4
endif
ifeq ($(strip $(OPERA_MINI_SUPPORT)), VER6)
SOURCES	 += operamini_mdu_def.h
SRCPATH += $(MMI_DIR)/source/mmi_app/app/operamini6
endif
ifeq ($(strip $(OPERA_MOBILE_SUPPORT)), V11)
SOURCES	 += operamobile_mdu_def.h
SRCPATH += $(MMI_DIR)/source/mmi_app/app/operamobile
endif


ifeq ($(strip $(CSC_SUPPORT)),TRUE)
SRCPATH += $(MMI_DIR)/source/mmi_app/app/customer/h
endif 

ifneq ($(strip $(MMIUZONE_SUPPORT)), NONE)
SOURCES	 += uzone_mdu_def.h 
SRCPATH += $(MMI_DIR)/source/mmi_app/app/uzone/h
endif

ifeq ($(strip $(SXH_APPLE_SUPPORT)),TRUE)
	SRCPATH	+= $(MMI_DIR)/source/mmi_app/app/apple/h
endif

#ifeq ($(strip $(SALES_SUPPORT)),TRUE)
SOURCES	 += sales_mdu_def.h
SRCPATH	+= $(MMI_DIR)/source/mmi_app/app/sales
#endif

#ifeq ($(strip $(FOTA_SUPPORT)),TRUE)
SOURCES	 += fota_mdu_def.h
SRCPATH	+= $(MMI_DIR)/source/mmi_app/app/fota/h
#endif


SOURCES	 += appsample_mdu_def.h

#20150601 wang.qinggang@byd.com add devicelock
SRCPATH	+= $(MMI_DIR)/source/mmi_app/app/devicelock/h
SOURCES	 += devicelock_mdu_def.h
#20150601 wang.qinggang@byd.com add devicelock end
#20150608 fu.ruilin@byd.com add control panel
SRCPATH += $(MMI_DIR)/source/mmi_app/app/cp/h
SOURCES += cp_mdu_def.h

#20150707 yan.guangsheng@byd.com add smartdualsim
ifeq ($(strip $(SMART_DUAL_SIM_SUPPORT)), TRUE)
ifeq ($(strip $(MMI_MULTI_SIM_SYS)), DUAL)
SRCPATH += $(MMI_DIR)/source/mmi_app/app/smartdualsim/h
SOURCES += smartdualsim_mdu_def.h
endif
endif

ifeq ($(strip $(MMIWNS_SUPPORT)), TRUE)
SOURCES	 += wns_mdu_def.h
SRCPATH	+= $(MMI_DIR)/source/mmi_app/app/wns/h
endif

#20150730 zhang.lu7@byd.com add data counter
SRCPATH += $(MMI_DIR)/source/mmi_app/app/datacounter/h
SOURCES += datacounter_mdu_def.h

ifeq ($(strip $(MMIENG_PING_SUPPORT)), TRUE)
SRCPATH	+= $(MMI_DIR)/source/mmi_app/app/ping/h
SOURCES	+= ping_mdu_def.h
endif
ifeq ($(strip $(SFR_SUPPORT)), TRUE)
SOURCES	 += selfreg_mdu_def.h
SRCPATH += $(MMI_DIR)/source/mmi_app/app/selfreg/h
endif

ifneq ($(strip $(TTS_SUPPORT)), NONE)
SOURCES	 += tts_mdu_def.h 
SRCPATH += $(MMI_DIR)/source/mmi_app/app/tts/h
endif

ifeq ($(strip $(CMCCDM_SUPPORT)), TRUE)
SOURCES	 += cmccdm_mdu_def.h
SRCPATH += $(MMI_DIR)/source/mmi_app/app/dm/h
endif
#
#  make process ,don't modify
#

vpath
vpath %.h        $(subst \,/,$(SRCPATH))

SOURCE_OBJ	= $(patsubst %.h, %.source, $(SOURCES))
PROJECT_OBJ	= $(patsubst %.h, %.project, $(PROJECTS))
SOURCE_DIR	= $(patsubst %_mdu_def.h,%, $(SOURCES))
ROOT_DIR	= $(shell $(PERL) ./make/perl_script/get_root_fullpath.pl)


.PHONEY:all
all :builddir macro_create $(SOURCE_OBJ) $(PROJECT_OBJ)

ifeq ($(strip $(MIDI_SUPPORT)), TRUE)
   ifeq ($(strip $(SOUNDBANK_MODE)),COMPACT)
	$(PERL) make/perl_script/midi_proc.pl "$(ROOT_DIR)/$(BUILD_DIR)/res/$(PROJECTS)" "-resdir" "$(ROOT_DIR)/$(MMI_DIR)/source/resource/$(MMI_RES_DIR)" "-tmpdir" "$(ROOT_DIR)/$(BUILD_DIR)/tmp" "-isbf" "$(ROOT_DIR)/$(IN_SOUNDBANK_FILE)"  "-sbps" "$(SOUNTBANK_BYTES_PER_SAMPLE)" "-osbf" "$(ROOT_DIR)/$(BUILD_DIR)/tmp/midi_soundbank_compact.c"
	$(COPY_FILE) $(call Path_process,$(BUILD_DIR)/tmp/midi_soundbank_compact.c) $(call Path_process,lib/modem/$(strip $(PROJECT))/)
   endif
endif

ifneq ($(strip $(CHIP_ENDIAN)), LITTLE)
ifeq ($(strip $(MMI_RES_ORIENT)),LANDSCAPE)
	$(PERL) make/perl_script/resgen.pl "$(ROOT_DIR)/$(BUILD_DIR)/res/$(PROJECTS)" "$(ROOT_DIR)/$(BUILD_DIR)/res/$(MMI_RES_DIR).bin" "$(PERL)"  -rep $(REP_FLAG) -alu $(ALU_FLAG) -crs $(CRM_FLAG)
else
	$(PERL) make/perl_script/resgen.pl "$(ROOT_DIR)/$(BUILD_DIR)/res/$(PROJECTS)" "$(ROOT_DIR)/$(BUILD_DIR)/res/$(MMI_RES_DIR).bin" "$(PERL)"  -rep $(REP_FLAG) -alu $(ALU_FLAG) -crs $(CRM_FLAG) -resdir "$(ROOT_DIR)/$(MMI_DIR)/source/resource/$(MMI_RES_DIR)"
endif
#ifeq ($(strip $(MULTI_THEME_SUPPORT)),TRUE)
	@$(COPY_FILE) $(call Path_process,$(BUILD_DIR)/res/$(MMI_RES_DIR).bin) $(call Path_process,$(BUILD_DIR)/img/)
	@$(COPY_FILE) $(call Path_process,$(BUILD_DIR)/res/$(MMI_RES_DIR)_win.bin) $(call Path_process,$(BUILD_DIR)/img/)
#else
#	@$(COPY_FILE) $(call Path_process,$(BUILD_DIR)/res/$(MMI_RES_DIR)_DEFAULT.bin)  $(call Path_process,$(BUILD_DIR)/img/$(MMI_RES_DIR).bin)
#	@$(COPY_FILE) $(call Path_process,$(BUILD_DIR)/res/$(MMI_RES_DIR)_DEFAULT_win.bin) $(call Path_process,$(BUILD_DIR)/img/$(MMI_RES_DIR)_win.bin)
#endif
else
ifeq ($(strip $(MMI_RES_ORIENT)),LANDSCAPE)
	$(PERL) make/perl_script/resgen.pl "$(ROOT_DIR)/$(BUILD_DIR)/res/$(PROJECTS)" "$(ROOT_DIR)/$(BUILD_DIR)/res/$(MMI_RES_DIR).bin" "$(PERL)" -L -rep $(REP_FLAG) -alu $(ALU_FLAG) -crs $(CRM_FLAG)
else
ifneq ($(strip $(VARIANT_LP)),)
	$(PERL) make/perl_script/resgen.pl "$(ROOT_DIR)/$(BUILD_DIR)/res/$(PROJECTS)" "$(ROOT_DIR)/$(BUILD_DIR)/res/$(MMI_RES_DIR)_$(VARIANT_LP).bin" "$(PERL)" -L -rep $(REP_FLAG) -alu $(ALU_FLAG) -crs $(CRM_FLAG) -resdir "$(ROOT_DIR)/$(BUILD_DIR)/res"
else 
	$(PERL) make/perl_script/resgen.pl "$(ROOT_DIR)/$(BUILD_DIR)/res/$(PROJECTS)" "$(ROOT_DIR)/$(BUILD_DIR)/res/$(MMI_RES_DIR).bin" "$(PERL)" -L -rep $(REP_FLAG) -alu $(ALU_FLAG) -crs $(CRM_FLAG) -resdir "$(ROOT_DIR)/$(MMI_DIR)/source/resource/$(MMI_RES_DIR)"
endif
endif


#ifeq ($(strip $(MULTI_THEME_SUPPORT)),TRUE)
ifneq ($(strip $(VARIANT_LP)),)
	@$(COPY_FILE) $(call Path_process,$(BUILD_DIR)/res/$(MMI_RES_DIR)_$(VARIANT_LP).bin) $(call Path_process,$(BUILD_DIR)/img/)
	@$(COPY_FILE) $(call Path_process,$(BUILD_DIR)/res/$(MMI_RES_DIR)_$(VARIANT_LP).bin) $(call Path_process,lib/modem/$(strip $(PROJECT))/)
else
	@$(COPY_FILE) $(call Path_process,$(BUILD_DIR)/res/$(MMI_RES_DIR).bin) $(call Path_process,$(BUILD_DIR)/img/)
	@$(COPY_FILE) $(call Path_process,$(BUILD_DIR)/res/$(MMI_RES_DIR).bin) $(call Path_process,lib/modem/$(strip $(PROJECT))/)
endif
#else
#	@$(COPY_FILE) $(call Path_process,$(BUILD_DIR)/res/$(MMI_RES_DIR)_DEFAULT.bin)  $(call Path_process,$(BUILD_DIR)/img/$(MMI_RES_DIR).bin)
#endif
endif
	@-$(COPY_FILE) $(call Path_process,$(BUILD_DIR)/res/*.txt) $(call Path_process,$(BUILD_DIR)/tmp/)
ifeq ($(strip $(VARIANT_LP)),)
	@-$(PERL) make/perl_script/rm_dir.pl $(BUILD_DIR)/res ""
endif

.PHONEY:builddir
builddir:
ifeq ($(strip $(MAKE_OS)), Linux)
	@echo Compiling in Linux
	@chmod -R +x make/make_cmd/
	@chmod -R +x make/perl_script
endif
	@echo $(PROJECT)
	@echo $(MMI_RES_DIR)
	@$(PERL) make/perl_script/mk_dir.pl "build" ""
	@$(PERL) make/perl_script/mk_dir.pl "$(BUILD_DIR)" ""
	@$(PERL) make/perl_script/rm_dir.pl res $(BUILD_DIR)/
ifneq ($(strip $(VARIANT_LP)),)
	@-$(PERL) make/perl_script/rm_file.pl $(MMI_RES_DIR)_$(VARIANT_LP).bin $(BUILD_DIR)/img/
else
	@-$(PERL) make/perl_script/rm_file.pl $(MMI_RES_DIR).bin $(BUILD_DIR)/img/
endif
	@-$(PERL) make/perl_script/rm_file.pl $(MMI_RES_DIR)_win.bin $(BUILD_DIR)/img/

	@$(PERL) make/perl_script/rm_file.pl midi_soundbank_compact.c $(BUILD_DIR)/tmp/
	@$(PERL) make/perl_script/mk_dir.pl "$(BUILD_DIR)/res" ""
	@$(PERL) make/perl_script/mk_dir.pl "$(BUILD_DIR)/img" ""	
#	$(COPY_DIR)  $(call Path_process,$(MMI_DIR)/source/resource/$(MMI_RES_DIR)/*)  $(call Path_process,$(BUILD_DIR)/res/)
ifeq ($(strip $(MMI_RES_ORIENT)),LANDSCAPE)
	@$(PERL) make/perl_script/mk_dir.pl "$(BUILD_DIR)/res/FONT" ""
	@$(PERL) make/perl_script/mk_dir.pl "$(BUILD_DIR)/res/RING" ""
	@$(PERL) make/perl_script/mk_dir.pl "$(BUILD_DIR)/res/common" ""
	@$(PERL) make/perl_script/mk_dir.pl "$(BUILD_DIR)/res/common/MMI_RES_DEFAULT" ""
	@$(PERL) make/perl_script/mk_dir.pl "$(BUILD_DIR)/res/common/MMI_RES_DEFAULT/DATA" ""
	$(COPY_FILE)  $(call Path_process,$(MMI_DIR)/source/resource/Common/*.xls)  $(call Path_process,$(BUILD_DIR)/res/)
	$(COPY_FILE)  $(call Path_process,$(MMI_DIR)/source/resource/Common/FONT/*)  $(call Path_process,$(BUILD_DIR)/res/FONT/)
	$(COPY_FILE)  $(call Path_process,$(MMI_DIR)/source/resource/Common/RING/*)  $(call Path_process,$(BUILD_DIR)/res/RING/)
	$(COPY_FILE)  $(call Path_process,$(MMI_DIR)/source/resource/Common/DATA/*)  $(call Path_process,$(BUILD_DIR)/res/common/MMI_RES_DEFAULT/DATA/)
	$(COPY_DIR)  $(call Path_process,$(MMI_DIR)/source/resource/$(MMI_RES_DIR)/*)  $(call Path_process,$(BUILD_DIR)/res/)
	$(PERL) make/perl_script/ProcessResDir.pl $(BUILD_DIR)/res
endif

ifneq ($(strip $(VARIANT_LP)),)
	@$(PERL) make/perl_script/mk_dir.pl "$(BUILD_DIR)/res/FONT" ""
	@$(PERL) make/perl_script/mk_dir.pl "$(BUILD_DIR)/res/RING" ""
ifeq ($(strip $(NEW_RESOURCE_SUPPORT)),TRUE)
	$(COPY_DIR)  $(call Path_process,$(MMI_DIR)/source/resource/Common/RING2)  $(call Path_process,$(BUILD_DIR)/res/RING/)
else
	$(COPY_DIR)  $(call Path_process,$(MMI_DIR)/source/resource/Common/RING)  $(call Path_process,$(BUILD_DIR)/res/RING/)
endif
ifeq ($(strip $(MMI_ORTHODOX_VERSION)),TRUE)
	$(COPY_DIR)  $(call Path_process,$(MMI_DIR)/source/resource/Common/RING_ORTH)  $(call Path_process,$(BUILD_DIR)/res/RING/)
endif
ifneq ($(wildcard $(MMI_DIR)/source/resource/Common/RING_$(VARIANT_LP)/*),)
	@-$(COPY_FILE)  $(call Path_process,$(MMI_DIR)/source/resource/Common/RING_$(VARIANT_LP)/*)   $(call Path_process,$(BUILD_DIR)/res/RING/) 
endif
	$(COPY_DIR)  $(call Path_process,$(MMI_DIR)/source/resource/$(MMI_RES_DIR)/*)  $(call Path_process,$(BUILD_DIR)/res/)
ifneq ($(wildcard $(MMI_DIR)/source/resource/$(MMI_RES_DIR)/LOGO_$(VARIANT_LP)/*),)	
	@-$(COPY_FILE)  $(call Path_process,$(MMI_DIR)/source/resource/$(MMI_RES_DIR)/LOGO_$(VARIANT_LP)/*)   $(call Path_process,$(BUILD_DIR)/res/*)
endif
	$(COPY_FILE)  $(call Path_process,$(MMI_DIR)/source/resource/Common/DATA/*)  $(call Path_process,$(BUILD_DIR)/res/common/MMI_RES_DEFAULT/DATA/)
ifneq ($(wildcard $(MMI_DIR)/source/resource/Common/DATA_$(VARIANT_LP)/*),)
	@-$(COPY_FILE)  $(call Path_process,$(MMI_DIR)/source/resource/Common/DATA_$(VARIANT_LP)/*)  $(call Path_process,$(BUILD_DIR)/res/common/MMI_RES_DEFAULT/DATA/)
endif
	$(COPY_DIR)  $(call Path_process,$(MMI_DIR)/source/resource/Common/FONT)  $(call Path_process,$(BUILD_DIR)/res/FONT/)
	$(COPY_FILE)  $(call Path_process,$(MMI_DIR)/source/resource/Common/*.xls)  $(call Path_process,$(BUILD_DIR)/res/)
ifeq ($(strip $(MMI_ORTHODOX_VERSION)),TRUE)
	$(PERL) make/perl_script/ProcessResDir_MS_Rescfg.pl $(BUILD_DIR)/res  ORTH
else
	$(PERL) make/perl_script/ProcessResDir_MS_Rescfg.pl $(BUILD_DIR)/res  "$(VARIANT_LP)"
endif

#ifneq ($(strip $(VARIANT_PC)),)
#	@-$(COPY_FILE)  $(call Path_process,$(MMI_DIR)/source/resource/Common/RING_$(VARIANT_LP)_$(VARIANT_PC)/*)   $(call Path_process,$(BUILD_DIR)/res/RING/*)
#	@-$(COPY_FILE)  $(call Path_process,$(MMI_DIR)/source/resource/$(MMI_RES_DIR)/LOGO_$(VARIANT_LP)_$(VARIANT_PC)/*)   $(call Path_process,$(BUILD_DIR)/res/*)
#	@-$(COPY_FILE)  $(call Path_process,$(MMI_DIR)/source/resource/Common/DATA_$(VARIANT_LP)_$(VARIANT_PC)/*)  $(call Path_process,$(BUILD_DIR)/res/common/MMI_RES_DEFAULT/DATA/)
#	$(PERL) make/perl_script/ProcessResDir_MS_Rescfg.pl $(BUILD_DIR)/res  "$(VARIANT_LP)_$(VARIANT_PC)
#endif
endif

	@$(PERL) make/perl_script/mk_dir.pl "$(BUILD_DIR)/tmp" ""
	@$(PERL) make/perl_script/mk_dir.pl "$(SOURCE_DIR)" $(BUILD_DIR)/res/

.PHONEY:macro_create
macro_create:
	@make PROJECT=$(PROJECT) -f $(call Path_process,make/pclint/pclint.mk)  -r -R COMPONENT=app_main resdep

%_mdu_def.source :%_mdu_def.h
	@$(ECHO) Compiling resource source file $< ...
	@$(ECHO)  $(VIA) Generate build/$(PROJECT)_builddir/res/$*/$*_mdu_def.h
	@$(ARMCC) $(IGNOR_WARNNINGS) $(VIA) $(BUILD_DIR)/tmp/app_main.macro -E $<  -o build/$(PROJECT)_builddir/res/$*/$*_mdu_def.h
	@$(ECHO) $*  build finished

%.project :%.h
	@$(ECHO) Compiling resource project file $< ...
	@$(PERL) make/perl_script/rm_dir.pl $*.h build/$(PROJECT)_builddir/res/
	@$(ARMCC) $(IGNOR_WARNNINGS) $(VIA) $(BUILD_DIR)/tmp/app_main.macro -E $<  -o build/$(PROJECT)_builddir/res/$*.h
	@$(ECHO) $*  build finished
	
.PHONY:clean
clean:
	@-$(PERL) make/perl_script/rm_dir.pl $(BUILD_DIR)/res ""
	@-$(PERL) make/perl_script/rm_file.pl $(MMI_RES_DIR).bin $(BUILD_DIR)/img/
	@-$(PERL) make/perl_script/rm_file.pl $(MMI_RES_DIR)_win.bin $(BUILD_DIR)/img/

