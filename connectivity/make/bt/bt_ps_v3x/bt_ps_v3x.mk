MCFLAG_OPT    += -D_CALIBRATION_VER_0009_
MCFLAG_OPT    += -D_RXLEVEL_PRECISION_
MCFLAG_OPT    += -D_RTOS
MCFLAG_OPT    += -D_DEBUG
MCFLAG_OPT    += -D_BT_USE_SFS
ifeq ($(strip $(BT_A2DP_SUPPORT)),TRUE)
MCFLAG_OPT    += -DBT_A2DP_NEED_QOS
endif

 
ifneq ($(strip $(DRM_SUPPORT)), NONE)    
    MCFLAG_OPT    +=  -DDRM_SUPPORT_$(strip $(DRM_SUPPORT))  -DDRM_SUPPORT
else
    MCFLAG_OPT    +=  -DDRM_SUPPORT_NONE
endif


ifeq ($(strip $(BLUETOOTH_SUPPORT)),ATHR_AR3002)
     MCFLAG_OPT    += -DPATCH_DEBUG_CHIP
endif

ifeq ($(strip $(PLATFORM)), SC6530)
   MCFLAG_OPT    += -DBT_UART_DMA_DYNAMIC -DBT_SBC_DSP_ENCODE
endif					

ifeq ($(strip $(PLATFORM)), SC6531EFM)
   MCFLAG_OPT    += -DBT_UART_DMA_DYNAMIC -DBT_SBC_DSP_ENCODE -DNEMOG_GSM_BT_BER
endif

ifeq ($(strip $(PLATFORM)), UIX8910)
   MCFLAG_OPT    += -DBT_UART_DMA_DYNAMIC -DBT_SBC_DSP_ENCODE
endif


ifeq ($(strip $(PLATFORM)), SC8800G)
   MCFLAG_OPT    += -DBT_SBC_ARM_ENCODE
endif

ifeq ($(strip $(CHIP_VER)),6531) 
   MCFLAG_OPT    += -DBT_POWEROFF_CONTROLLER
endif

ifeq ($(strip $(CHIP_VER)),6531EFM) 
   MCFLAG_OPT    += -DBT_POWEROFF_CONTROLLER
endif

ifneq "$(filter $(strip $(CHIP_VER)), UIX8910 UIX8910MPW UIS8910C UIS8910A UIS8910FF)" ""
   MCFLAG_OPT    += -DBT_POWEROFF_CONTROLLER
endif


ifeq ($(strip $(CHIP_VER)),7701) 
   MCFLAG_OPT    += -DBT_POWEROFF_CONTROLLER
endif

ifeq ($(strip $(CHIP_VER)),7703A) 
   MCFLAG_OPT    += -DBT_POWEROFF_CONTROLLER
endif

ifeq ($(strip $(BT_HSP_SUPPORT)),TRUE)
     MCFLAG_OPT    += -DBT_HF_WITH_HS
endif

ifeq ($(strip $(BT_DIALER_SUPPORT)),TRUE)
     MCFLAG_OPT    += -DBT_FUNCTIONS_COMPLETE
endif

MINCPATH =  BASE/Layer1/export/inc \
	common/export/inc \
	MS_MMI/source/mmi_app/app/bt/h \
	MS_Ref/export/inc \
	RTOS/export/inc \
	chip_drv/export/inc \
	chip_drv/source/h \
	chip_drv/chip_plf \
	chip_drv/export/inc/outdated \
	connectivity/Trace_id/inc \
	connectivity/BT/export/inc \
	connectivity/BT/BT_PS_V3X \
	connectivity/BT/BT_PS_V3X/inc \
	connectivity/BT/BT_PS_V3X/src \
	connectivity/BT/BT_PS_V3X/bluesdk/stack \
	connectivity/BT/BT_PS_V3X/bluesdk/hcitrans/uart \
	connectivity/BT/BT_PS_V3X/bluesdk/hcitrans \
	connectivity/BT/BT_PS_V3X/bluesdk/inc \
	connectivity/BT/BT_PS_V3X/bluesdk/profiles/goep \
	connectivity/BT/BT_PS_V3X/bluesdk/profiles/hf \
	connectivity/BT/BT_PS_V3X/bluesdk/profiles/common \
	connectivity/BT/BT_PS_V3X/bluesdk/add-ins/at \
	connectivity/BT/BT_PS_V3X/bluesdk/add-ins/obex \
	connectivity/BT/BT_PS_V3X/bluesdk/profiles/ftp \
	connectivity/BT/BT_PS_V3X/bluesdk/profiles/opush \
	connectivity/BT/BT_PS_V3X/bluesdk/profiles/headset \
	connectivity/BT/BT_PS_V3X/bluesdk/inc/sys \
	connectivity/BT/BT_PS_V3X/bluesdk/profiles/handsfree/inc \
	connectivity/BT/BT_PS_V3X/bluesdk/profiles/av/inc \
	connectivity/BT/BT_PS_V3X/bluesdk/profiles/obex/inc \
	connectivity/BT/BT_PS_V3X/bluesdk/profiles/obex/inc/sys \
	connectivity/BT/BT_PS_V3X/bluesdk/hcitrans/abcsp/config \
	connectivity/BT/BT_PS_V3X/bluesdk/profiles/headset/inc \
	connectivity/BT/BT_PS_V3X/kernel/inc 

ifeq ($(strip $(BLUETOOTH_SUPPORT)),ATHR_AR3002)
     MINCPATH += connectivity/BT/BT_PS_V3X/source/patch
endif

ifeq ($(strip $(BT_SPP_SUPPORT)),TRUE)
     MINCPATH += connectivity/BT/BT_PS_V3X/bluesdk/profiles/spp 
endif

ifeq ($(strip $(BT_DUN_SUPPORT)),TRUE)
     MINCPATH += connectivity/BT/BT_PS_V3X/bluesdk/profiles/spp 
endif

ifeq ($(strip $(BT_PBAP_SUPPORT)),TRUE)
     MINCPATH += connectivity/BT/BT_PS_V3X/bluesdk/profiles/pbap 
endif

MSRCPATH =connectivity/BT/BT_PS_V3X \
	connectivity/BT/BT_PS_V3X/src \
	connectivity/BT/BT_PS_V3X/kernel/os \
	connectivity/BT/BT_PS_V3X/bluesdk/add-ins/at \
	connectivity/BT/BT_PS_V3X/bluesdk/profiles/hf \
	connectivity/BT/BT_PS_V3X/bluesdk/profiles/avctp \
	connectivity/BT/BT_PS_V3X/bluesdk/profiles/avdtp \
	connectivity/BT/BT_PS_V3X/bluesdk/profiles/a2dp \
	connectivity/BT/BT_PS_V3X/bluesdk/profiles/avrcp \
	connectivity/BT/BT_PS_V3X/bluesdk/add-ins/obex \
	connectivity/BT/BT_PS_V3X/bluesdk/profiles/goep \
	connectivity/BT/BT_PS_V3X/bluesdk/profiles/ftp \
	connectivity/BT/BT_PS_V3X/bluesdk/profiles/opush \
	connectivity/BT/BT_PS_V3X/bluesdk/profiles/obex/profiles/ftp \
	connectivity/BT/BT_PS_V3X/bluesdk/profiles/obex/profiles/opush \
	connectivity/BT/BT_PS_V3X/bluesdk/profiles/headset/profiles/headset \
	connectivity/BT/BT_PS_V3X/bluesdk/profiles/common \
	connectivity/BT/BT_PS_V3X/bluesdk/stack/controller \
	connectivity/BT/BT_PS_V3X/bluesdk/stack/hci \
	connectivity/BT/BT_PS_V3X/bluesdk/stack/l2cap \
	connectivity/BT/BT_PS_V3X/bluesdk/stack/rfcomm \
	connectivity/BT/BT_PS_V3X/bluesdk/stack/me \
	connectivity/BT/BT_PS_V3X/bluesdk/stack/sdp \
	connectivity/BT/BT_PS_V3X/bluesdk/stack \
	connectivity/BT/BT_PS_V3X/bluesdk/hcitrans/modinit \
	connectivity/BT/BT_PS_V3X/bluesdk/hcitrans \
	connectivity/BT/BT_PS_V3X/bluesdk/hcitrans/uart \
	connectivity/BT/BT_PS_V3X/bluesdk/osapi \
	connectivity/Trace_id

ifeq ($(strip $(BLUETOOTH_SUPPORT)),ATHR_AR3002)
     MSRCPATH += connectivity/BT/BT_PS_V3X/source/patch
endif

ifeq ($(strip $(BT_SPP_SUPPORT)),TRUE)
     MSRCPATH += connectivity/BT/BT_PS_V3X/bluesdk/profiles/spp 
endif

ifeq ($(strip $(BT_DUN_SUPPORT)),TRUE)
     MSRCPATH += connectivity/BT/BT_PS_V3X/bluesdk/profiles/spp 
endif

ifeq ($(strip $(BT_PBAP_SUPPORT)),TRUE)
     MSRCPATH += connectivity/BT/BT_PS_V3X/bluesdk/profiles/pbap 
endif

ifeq ($(strip $(BT_DUN_SUPPORT)),TRUE)
     MSRCPATH += connectivity/BT/BT_PS_V3X/bluesdk/profiles/dun 
endif

ifeq ($(strip $(BT_MAP_SUPPORT)),TRUE)
     MSRCPATH += connectivity/BT/BT_PS_V3X/bluesdk/profiles/map 
endif

ifeq ($(strip $(BT_SAP_SUPPORT)),TRUE)
     MSRCPATH += connectivity/BT/BT_PS_V3X/bluesdk/profiles/sap 
endif
 
#add for DRM 
MINCPATH += BASE/l4/export/inc BASE/sim/export/inc $(PS_INCPATH)

	SOURCES     = at_hf.c
	SOURCES    += at_hs.c
ifeq ($(strip $(BT_DUN_SUPPORT)),TRUE)	 
	SOURCES    += at_dun.c
endif
ifeq ($(strip $(BT_PBAP_SUPPORT)),TRUE)		
	SOURCES    += at_pb.c
endif	 
	SOURCES    += at_sms.c atp.c
	SOURCES    += hfgalloc.c 
	SOURCES    += hfg.c
	SOURCES    += hfg_at.c
	SOURCES    += hfg_sdp.c
	SOURCES    += hfg_sm.c
	SOURCES    += hfg_util.c
	SOURCES    += obclient.c
	SOURCES    += obdebug.c
	SOURCES    += obex_utl.c
	SOURCES    += obexauth.c
	SOURCES    += obheader.c
	SOURCES    += obparse.c
	SOURCES    += observer.c
	SOURCES    += obstack.c
	SOURCES    += obxalloc.c
	SOURCES    += obxbtstk.c
	SOURCES    += goep.c
	SOURCES    += opush.c
	SOURCES    += conmgr.c
	SOURCES    += hci.c
	SOURCES    += hci_evnt.c
	SOURCES    += hci_proc.c
	SOURCES    += hci_types.c
	SOURCES    += hci_util.c
	SOURCES    += prescan.c
	SOURCES    += l2cap.c 
	SOURCES    += l2cap_if.c 
	SOURCES    += l2cap_sm.c 
	SOURCES    += l2cap_types.c 
	SOURCES    += l2cap_utl.c 
	SOURCES    += rfc_ctl.c 
	SOURCES    += rfc_fcs.c 
	SOURCES    += rfc_sm.c 
	SOURCES    += rfc_types.c 
	SOURCES    += rfc_util.c 
	SOURCES    += rfcomm.c
	SOURCES    += me.c 
	SOURCES    += meacc.c 
	SOURCES    += mecon.c 
	SOURCES    += medev.c 
	SOURCES    += meif.c 
	SOURCES    += mesco.c 
	SOURCES    += mescodat.c 
	SOURCES    += mescoif.c 
	SOURCES    += mesec.c 
	SOURCES    += sdp.c 
	SOURCES    += sdpclient.c 
	SOURCES    += sdpserv.c 
	SOURCES    += sdputil.c
	SOURCES    += btalloc.c 
	SOURCES    += btrom.c 
	SOURCES    += btutils.c 
	SOURCES    += debug.c 
	SOURCES    += eventmgr.c 
	SOURCES    += radiomgr.c 
	SOURCES    += rxbuff.c 
	SOURCES    += utils.c 
	SOURCES    += transprt.c 
	SOURCES    += radiomod.c 
	SOURCES    += ddb.c 
	SOURCES    += osapi.c 
	SOURCES    += os_msg.c 
	SOURCES    += os_pmalloc.c 
	SOURCES    += os_task.c 
	SOURCES    += pf_file.c  
	SOURCES    += pf_common.c 
	SOURCES    += btf_app_handle.c 
	SOURCES    += btf_hf.c 
	SOURCES    += btf_obstore.c 
	SOURCES    += btf_opc.c 
	SOURCES    += btf_ops.c 
	SOURCES    += bt_dut.c 
	SOURCES    += bt_abs.c 
	SOURCES    += bt_ps_version.c
	SOURCES    += mta_tracescan_nosme.c
	SOURCES    += bta_dm_cfg.c
	SOURCES    += bta_dm_pm.c
	SOURCES    += ble_usr_add_service.c
	SOURCES    += ble_usr_service_data.c
	
ifeq ($(strip $(BLUETOOTH_SUPPORT)),ATHR_AR3002)
     SOURCES += ath_ps_patch_arrary.c ar3kpsparser.c ar3kpsconfig.c
endif								

ifeq ($(strip $(BT_HCI_TRANS)),BCSP)
	MINCPATH 	+= connectivity/BT/BT_PS_V3X/bluesdk/hcitrans/abcsp/include 
	MINCPATH 	+= connectivity/BT/BT_PS_V3X/bluesdk/hcitrans/abcsp/config
	
	MSRCPATH 	+= connectivity/BT/BT_PS_V3X/bluesdk/hcitrans/abcsp 
	MSRCPATH 	+= connectivity/BT/BT_PS_V3X/bluesdk/hcitrans/abcsp/src 
	MSRCPATH 	+= connectivity/BT/BT_PS_V3X/patch
	
	SOURCES    += bccmd_boot_strap.c 
	SOURCES    += bccmd.c 
	SOURCES    += xa_abcsp.c 
	SOURCES    += abcsphci.c  
	SOURCES    += abcsp_crc.c 
	SOURCES    += abcsp_free.c
	SOURCES    += abcsp_init.c
	SOURCES    += abcsp_malloc.c
	SOURCES    += abcsp_zmalloc.c
	SOURCES    += abcsp_le.c
	SOURCES    += rxbcsp.c
	SOURCES    += rxmsg.c
	SOURCES    += rxmsgdemux.c
	SOURCES    += rxslip.c
	SOURCES    += txmsg.c
	SOURCES    += txrx.c
	SOURCES    += txslip.c
	
	MCFLAG_OPT	+= -DSPRD_BCSP		
else
 	 SOURCES += uarttran.c
endif

ifeq ($(strip $(BT_AV_CT_SUPPORT)),TRUE)
	MINCPATH 	+= connectivity/BT/BT_PS_V3X/inc
	SOURCES    += btf_av_ct.c
ifeq ($(strip $(BT_A2DP_SUPPORT)),TRUE)

else
	SOURCES    += avctp.c 
	SOURCES    += avctpcon.c 
	SOURCES    += avctpmsg.c
	SOURCES    += avrcp.c 
	SOURCES    += avrcpsdp.c
	SOURCES    += avtp.c
endif
endif

ifeq ($(strip $(BT_A2DP_SUPPORT)),TRUE)
	SOURCES    += a2dp.c 
	SOURCES    += a2dp_sm.c 
	SOURCES    += a2dp_sdp.c 
	SOURCES    += a2alloc.c 
	SOURCES    += a2dp_api.c
	SOURCES    += avctp.c 
	SOURCES    += avctpcon.c 
	SOURCES    += avctpmsg.c
	SOURCES    += avalloc.c 
	SOURCES    += avdtp.c 
	SOURCES    += avdtpcon.c 
	SOURCES    += avsigmgr.c
	SOURCES    += avrcp.c 
	SOURCES    += avrcpsdp.c
	SOURCES    += avtp.c
	SOURCES    += btf_av.c
	SOURCES    += btf_av_tg.c
ifneq ($(strip $(BT_AV_CT_SUPPORT)),TRUE)
	SOURCES    += btf_av_ct.c
endif
endif

ifeq ($(strip $(BT_AV_SNK_SUPPORT)),TRUE)
ifeq ($(strip $(BT_A2DP_SUPPORT)),TRUE)
	SOURCES    += btf_av_sink.c
	SOURCES    += sbc.c
else
	SOURCES    += a2dp.c 
	SOURCES    += a2dp_sm.c 
	SOURCES    += a2dp_sdp.c 
	SOURCES    += a2alloc.c 
	SOURCES    += a2dp_api.c
	SOURCES    += avctp.c 
	SOURCES    += avctpcon.c 
	SOURCES    += avctpmsg.c
	SOURCES    += avalloc.c 
	SOURCES    += avdtp.c 
	SOURCES    += avdtpcon.c 
	SOURCES    += avsigmgr.c
	SOURCES    += avrcp.c 
	SOURCES    += avrcpsdp.c
	SOURCES    += avtp.c
	SOURCES    += btf_av_sink.c
	SOURCES    += sbc.c
endif
endif

ifeq ($(strip $(BT_FTS_SUPPORT)),TRUE)
    SOURCES    += btf_fts.c 
    SOURCES    += ftp.c
endif

ifeq ($(strip $(BT_SPP_SUPPORT)),TRUE)
    SOURCES    += btf_spp.c
    ifeq ($(strip $(BT_SPP_2ND_CHANNEL_SUPPORT)),TRUE) 
    SOURCES    += btf_spp2.c 
    endif
    SOURCES    += ring.c 
    SOURCES    += spp2os.c 
    SOURCES    += spp.c 
    SOURCES    += sppos.c 
endif

ifeq ($(strip $(BT_PBAP_SUPPORT)),TRUE)
    SOURCES     += btf_pbap.c 
    SOURCES     += pbap.c 
    SOURCES     += pbap_sdp.c 
    SOURCES     += pbapobs.c 
endif

ifeq ($(strip $(CHIP_VER)),6531) 
	SOURCES    += BT_RAM_CODE_6531.c
endif

ifeq ($(strip $(CHIP_VER)),6531EFM) 
	SOURCES    += BT_RAM_CODE_6531EFM.c
endif



ifneq "$(filter $(strip $(CHIP_VER)), UIX8910 UIX8910MPW UIS8910C UIS8910A UIS8910FF)" ""
	SOURCES    += BT_RAM_CODE_8910.c
endif


ifeq ($(strip $(BT_DUN_SUPPORT)),TRUE)
    SOURCES    += btf_dun.c
    ifneq ($(strip $(BT_SPP_SUPPORT)),TRUE)
    	SOURCES    += ring.c 
    	SOURCES    += spp2os.c 
    	SOURCES    += spp.c 
    	SOURCES    += sppos.c 
    endif
endif		

ifeq ($(strip $(BT_HFU_SUPPORT)),TRUE)
    SOURCES    += hf.c 
    SOURCES    += hf_at.c 
    SOURCES    += hf_cmd.c 
    SOURCES    += hf_cmgr.c 
    SOURCES    += hf_sdp.c
    SOURCES    += hf_sm.c
    SOURCES    += hf_util.c
    SOURCES    += hfalloc.c
    SOURCES    += btf_hfu.c 
endif

ifeq ($(strip $(BT_PS_BQB_TEST)),TRUE)
		MSRCPATH 	+= connectivity/BT/BT_PS_V3X/protocoltest 
		SOURCES    += btf_l2cap.c 
		SOURCES    += btf_gap.c 
		SOURCES    +=	btf_rfcomm.c
		SOURCES    +=	btf_main.c
endif

ifeq ($(strip $(BT_MAP_SUPPORT)),TRUE)
    SOURCES     += btf_map.c 
    SOURCES     += map.c 
    SOURCES     += map_sdp.c 
    SOURCES     += mapobs.c
    SOURCES     += maputil.c
endif

ifeq ($(strip $(BT_SAP_SUPPORT)),TRUE)
    SOURCES     += btf_sap.c 
    SOURCES     += sap.c 
endif
