
MCFLAG_OPT   	= -Otime

#MCFLAG_OPT   	+=-DVOICE_ASSERT_MODE

ifeq ($(strip $(MODEM_PLATFORM)), TRUE)
MCFLAG_OPT      +=-DMODEM_RECORD_DEBUG_DATA_PARA
endif
MINCPATH =  MS_Ref/source/codec/voice/inc MS_Ref/export/inc \
 MS_Ref/source/aud_config/inc_export MS_Ref/source/audioservice/inc_export MS_Ref/source/audioservice/inc
MINCPATH +=$(L1_INCPATH) 	

ifeq ($(strip $(DUALMIC)), TRUE)
ifeq ($(strip $(DUALMIC_NORMAL)), TRUE)
MINCPATH += MS_Ref/export/inc/dualmic_normal
else
MINCPATH += MS_Ref/export/inc/dualmic
endif
else
MINCPATH += MS_Ref/export/inc/singlemic
endif
MINCPATH += chip_drv/export/inc
MSRCPATH =  MS_Ref/source/codec/voice/src

SOURCES			= voice_adp.c voiceprocess_adp.c aud_LVVE_ctrl.c  bt_nxp_process.c \
             aud_alc.c  


