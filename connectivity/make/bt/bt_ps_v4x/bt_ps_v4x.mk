MCFLAG_OPT   	+= -D_CALIBRATION_VER_0009_
MCFLAG_OPT   	+= -D_RXLEVEL_PRECISION_
MCFLAG_OPT   	+= -D_RTOS
MCFLAG_OPT   	+= -D_DEBUG
MCFLAG_OPT   	+= -D_BT_USE_SFS

ifneq ($(strip $(DRM_SUPPORT)),)  
   ifneq ($(strip $(DRM_SUPPORT)), NONE)    
        MCFLAG_OPT		+=  -DDRM_SUPPORT_$(strip $(DRM_SUPPORT))  -DDRM_SUPPORT
   else
        MCFLAG_OPT		+=  -DDRM_SUPPORT_NONE
   endif
endif

ifeq ($(strip $(BLUETOOTH_SUPPORT)),ATHR_AR3002)
     MCFLAG_OPT   	+= -DPATCH_DEBUG_CHIP
endif

ifeq ($(strip $(PLATFORM)), SC6530)
   MCFLAG_OPT += -DBT_UART_DMA_DYNAMIC -DBT_SBC_DSP_ENCODE
endif					

ifeq ($(strip $(PLATFORM)), SC6800H)
   MCFLAG_OPT += -DBT_SBC_DSP_ENCODE
endif	

ifeq ($(strip $(PLATFORM)), SC8800G)
   MCFLAG_OPT += -DBT_SBC_ARM_ENCODE
endif

MINCPATH =  base/Layer1/export/inc \
						connectivity/BT/bt_ps_v4x/src \
						common/export/inc \
						MS_MMI/source/mmi_app/app/bt/h \
						MS_Ref/export/inc \
						chip_drv/export/inc \
						RTOS/export/inc \
						connectivity/BT/bt_ps_v4x/inc \
						chip_drv/source/h \
						connectivity/BT/export/inc \
						connectivity/BT/bt_ps_v4x/src \
						connectivity/BT/bt_ps_v4x/inc \
						connectivity/BT/bt_ps_v4x/bluesdk/stack \
						connectivity/BT/bt_ps_v4x/bluesdk/hcitrans/uart \
						connectivity/BT/bt_ps_v4x/bluesdk/hcitrans \
						connectivity/BT/bt_ps_v4x/bluesdk/inc \
						connectivity/BT/bt_ps_v4x/bluesdk/profiles/goep \
						connectivity/BT/bt_ps_v4x/bluesdk/profiles/hf \
						connectivity/BT/bt_ps_v4x/bluesdk/profiles/common \
						connectivity/BT/bt_ps_v4x/bluesdk/add-ins/at \
						connectivity/BT/bt_ps_v4x/bluesdk/add-ins/obex \
						connectivity/BT/bt_ps_v4x/bluesdk/profiles/ftp \
						connectivity/BT/bt_ps_v4x/bluesdk/profiles/opush \
						connectivity/BT/bt_ps_v4x/bluesdk/profiles/headset \
						connectivity/BT/bt_ps_v4x \
						chip_drv/chip_plf \
						connectivity/BT/bt_ps_v4x/bluesdk/inc/sys \
						chip_drv/export/inc/outdated \
						connectivity/BT/bt_ps_v4x/bluesdk/profiles/handsfree/inc \
						connectivity/BT/bt_ps_v4x/bluesdk/profiles/av/inc \
						connectivity/BT/bt_ps_v4x/bluesdk/profiles/obex/inc \
						connectivity/BT/bt_ps_v4x/bluesdk/profiles/obex/inc/sys \
						connectivity/BT/bt_ps_v4x/bluesdk/hcitrans/abcsp/config \
						connectivity/BT/bt_ps_v4x/bluesdk/profiles/headset/inc \
						connectivity/BT/bt_ps_v4x/kernel/inc 

ifeq ($(strip $(BLUETOOTH_SUPPORT)),ATHR_AR3002)
     MINCPATH += connectivity/BT/bt_ps_v4x/source/patch
endif


MSRCPATH = connectivity/BT/bt_ps_v4x/bluesdk/profiles/handsfree/add-ins/at \
 			connectivity/BT/bt_ps_v4x/bluesdk/profiles/handsfree/profiles/hf \
			connectivity/BT/bt_ps_v4x/bluesdk/profiles/av/profiles/common \
			connectivity/BT/bt_ps_v4x/bluesdk/profiles/av/profiles/a2vdp \
			connectivity/BT/bt_ps_v4x/bluesdk/profiles/av/profiles/avctp \
			connectivity/BT/bt_ps_v4x/bluesdk/profiles/av/profiles/avdtp \
			connectivity/BT/bt_ps_v4x/bluesdk/profiles/av/profiles/avrcp \
			connectivity/BT/bt_ps_v4x/bluesdk/profiles/av/profiles/a2dp \
			connectivity/BT/bt_ps_v4x/bluesdk/profiles/obex/add-ins/obex \
			connectivity/BT/bt_ps_v4x/bluesdk/profiles/obex/profiles/goep \
			connectivity/BT/bt_ps_v4x/bluesdk/profiles/obex/profiles/ftp \
			connectivity/BT/bt_ps_v4x/bluesdk/profiles/obex/profiles/opush \
			connectivity/BT/bt_ps_v4x/bluesdk/profiles/headset/profiles/headset \
			connectivity/BT/bt_ps_v4x/bluesdk/profiles/common \
			connectivity/BT/bt_ps_v4x/bluesdk/stack/controller \
			connectivity/BT/bt_ps_v4x/bluesdk/stack/hci \
			connectivity/BT/bt_ps_v4x/bluesdk/stack/l2cap \
			connectivity/BT/bt_ps_v4x/bluesdk/stack/rfcomm \
			connectivity/BT/bt_ps_v4x/bluesdk/stack/me \
			connectivity/BT/bt_ps_v4x/bluesdk/stack/sdp \
			connectivity/BT/bt_ps_v4x/bluesdk/stack \
			connectivity/BT/bt_ps_v4x/bluesdk/hcitrans/modinit \
			connectivity/BT/bt_ps_v4x/bluesdk/hcitrans \
			connectivity/BT/bt_ps_v4x/bluesdk/hcitrans/uart \
			connectivity/BT/bt_ps_v4x/bluesdk/osapi \
			connectivity/BT/bt_ps_v4x/src \
			connectivity/BT/bt_ps_v4x/ \
			connectivity/BT/bt_ps_v4x/kernel/os

ifeq ($(strip $(BLUETOOTH_SUPPORT)),ATHR_AR3002)
     MSRCPATH += connectivity/BT/bt_ps_v4x/source/patch
endif

#add for DRM 
MINCPATH += base/l4/export/inc base/sim/export/inc $(PS_INCPATH)

SOURCES				= at_dun.c at_hf.c at_hs.c at_pb.c at_sms.c atp.c \
								hfg.c hfg_at.c hfg_sdp.c hfg_sm.c hfg_util.c hfgalloc.c \
								avdevice.c avtp.c \
								a2valloc.c a2vdp.c a2vdp_sdp.c a2vdp_sm.c \
								avctp.c avctpcon.c avctpmsg.c \
								avalloc.c avdtp.c avdtpcon.c avsigmgr.c \
								avrcp.c avrcp_typ.c avrcpact.c avrcpatg.c avrcpbrws.c avrcppnl.c avrcpsdp.c \
								a2dp.c a2dp_sm.c msbcplc.c \
								obclient.c obdebug.c obex_types.c obex_utl.c obexauth.c obheader.c obparse.c observer.c obstack.c obxalloc.c obxbtstk.c obxl2btstk.c \
								goep.c goep_types.c\
								ftp.c \
								opush.c \
								conmgr.c \
								brphylink.c extflowspec.c phylink.c loglink.c \
								hci.c hci_evnt.c hci_proc.c hci_types.c hci_util.c prescan.c \
								fixmgr.c l2cap.c l2cap_a.c l2cap_e.c l2cap_if.c l2cap_sm.c l2cap_stub.c l2cap_types.c l2cap_utl.c l2capagent.c l2conless.c l2signal.c \
								rfc_ctl.c rfc_fcs.c rfc_sm.c rfc_types.c rfc_util.c rfcomm.c \
								me.c me_types.c meacc.c meamp.c mecon.c medev.c meif.c mesco.c mescodat.c mescoif.c mesec.c mesecahm.c mesecamp.c mesecash.c \
								sdp.c sdpclient.c sdpserv.c sdputil.c \
								btalloc.c btrom.c btutils.c debug.c eventmgr.c radiomgr.c rxbuff.c statemachine.c userheap.c utils.c \
								transprt.c \
								radiomod.c \
								ddb.c \
								osapi.c \
								btf_app_handle.c btf_obstore.c btf_opc.c btf_ops.c btf_fts.c btf_av.c btf_hf.c \
								a2dp_api.c \
								pf_common.c pf_file.c \
								bt_abs.c bt_dut.c \
								os_msg.c os_pmalloc.c os_sched.c os_task.c 

ifeq ($(strip $(BLUETOOTH_SUPPORT)),ATHR_AR3002)
     SOURCES += ath_ps_patch_arrary.c ar3kpsparser.c ar3kpsconfig.c
endif								

ifeq ($(strip $(BLUETOOTH_HCI_TRANS_BCSP)),TRUE)
		MINCPATH += connectivity/BT/bt_ps_v4x/bluesdk/hcitrans/abcsp/include connectivity/BT/bt_ps_v4x/bluesdk/hcitrans/abcsp/config
		MSRCPATH += connectivity/BT/bt_ps_v4x/bluesdk/hcitrans/abcsp connectivity/BT/bt_ps_v4x/bluesdk/hcitrans/abcsp/src connectivity/BT/bt_ps_v4x/patch
		SOURCES +=bccmd_boot_strap.c bccmd.c xa_abcsp.c abcsphci.c  abcsp_crc.c abcsp_free.c\
    abcsp_init.c\
    abcsp_malloc.c\
    abcsp_zmalloc.c\
    le.c\
    rxbcsp.c\
    rxmsg.c\
    rxmsgdemux.c\
    rxslip.c\
    txmsg.c\
    txrx.c\
    txslip.c
   
   MCFLAG_OPT	+= -DSPRD_BCSP		
else
 	 SOURCES += uarttran.c
endif

