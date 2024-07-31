
MCFLAG_OPT      = 

MASMFLAG_OPT    = 


ifneq "$(filter $(strip $(CHIP_VER)), UIX8910 UIX8910MPW UIS8910C UIS8910A UIS8910FF)" ""
MCFLAG_OPT  += -DCHIP_DRV_UIX8910
MCFLAG_OPT  += -DARM_ARCH_V7
MCFLAG_OPT  += -D_SIO_WAKEUP_ARM
endif



ifeq ($(strip $(STACK_CHECK_SUPPORT)),    FALSE)
ifeq ($(strip $(CHIP_ENDIAN)), LITTLE)
MASMFLAG_OPT    = $(KEEP) $(CODE16) $(LITTLE_ENDIAN_OPT) $(APCS) /shlclient/interwork/
else
MASMFLAG_OPT    = $(KEEP) $(CODE16) $(BIG_ENDIAN_OPT) $(APCS) /shlclient/interwork/
endif
endif

#MCFLAG_OPT         += -S
ifeq ($(strip $(CHIP_ENDIAN)),    LITTLE)
MCFLAG_OPT         += -D_LITTLE_ENDIAN_DEBUG
#for usb modify only , should modify later
endif

MINCPATH =  MS_Ref/source/aud_config/inc_export \
            Third-party/xsr_stlmp/include \
            $(ATC_INCPATH) $(L1_INCPATH) \
            BASE/logsave/export/inc BASE/PS/export/inc 

ifeq ($(strip $(PLATFORM)),    UIX8910)

MINCPATH += chip_drv/export/inc/outdated
MINCPATH += chip_drv/export/inc
MINCPATH += chip_drv/chip_plf/export/inc
MINCPATH += chip_drv/chip_lib/inc
MINCPATH += chip_drv/chip_plf
MINCPATH += chip_drv/chip_module/mmu
MINCPATH += chip_drv/chip_module/audio
MINCPATH += chip_drv/chip_module/smc/uix8910
MINCPATH += chip_drv/chip_module/lzma/uix8910
MINCPATH += chip_drv/chip_module/sys_mail/uix8910	

MSRCPATH += chip_drv/chip_module/lzma/uix8910
MINCPATH += chip_drv/chip_module/chip/uix8910

MINCPATH += chip_drv/chip_module/smc/uix8910/ddr_timing
MINCPATH += chip_drv/chip_module/eic
MINCPATH += chip_drv/chip_module/eic/uix8910
MSRCPATH += chip_drv/chip_plf/common/src
MSRCPATH += chip_drv/chip_module/adc
MSRCPATH += chip_drv/chip_module/adc/r6p0
MSRCPATH += chip_drv/chip_module/global_ctl
MSRCPATH += chip_drv/chip_module/adi
MSRCPATH += chip_drv/chip_module/adi/uix8910
MSRCPATH += chip_drv/chip_module/analog
MSRCPATH += chip_drv/chip_module/analog/sc2720
MSRCPATH += chip_drv/chip_module/cardlog
MSRCPATH += chip_drv/chip_module/busmonitor
MSRCPATH += chip_drv/chip_module/busmonitor/uix8910
MSRCPATH += chip_drv/chip_module/charge/uix8910
MSRCPATH += chip_drv/chip_module/charge/v7
MSRCPATH += chip_drv/chip_module/chip
MSRCPATH += chip_drv/chip_module/clock
MSRCPATH += chip_drv/chip_module/dma
MSRCPATH += chip_drv/chip_module/dsp
MSRCPATH += chip_drv/chip_module/dsp/v5
MSRCPATH += chip_drv/chip_module/smc
MSRCPATH += chip_drv/chip_module/smc/uix8910
MSRCPATH += chip_drv/chip_module/efuse
MSRCPATH += chip_drv/chip_module/efuse/uix8910
MSRCPATH += chip_drv/chip_module/freq
MSRCPATH += chip_drv/chip_module/freq/v5
MSRCPATH += chip_drv/chip_module/gea
MSRCPATH += chip_drv/chip_module/gea/v5
MSRCPATH += chip_drv/chip_module/gpio
MSRCPATH += chip_drv/chip_module/i2c
MSRCPATH += chip_drv/chip_module/i2c/uix8910
MSRCPATH += chip_drv/chip_module/i2c/gpio_simu
MSRCPATH += chip_drv/chip_module/init
MSRCPATH += chip_drv/chip_module/int
MSRCPATH += chip_drv/chip_module/int/uix8910
MSRCPATH += chip_drv/chip_module/kpd
MSRCPATH += chip_drv/chip_module/kpd/uix8910
MSRCPATH += chip_drv/chip_module/lcdc
MSRCPATH += chip_drv/chip_module/lcdc/v5
MSRCPATH += chip_drv/chip_module/lcdc/uix8910
MSRCPATH += chip_drv/chip_module/ldo
MSRCPATH += chip_drv/chip_module/ldo/v5
MSRCPATH += chip_drv/chip_module/misc
MSRCPATH += chip_drv/chip_module/norflash
MSRCPATH += chip_drv/chip_module/pcm
MSRCPATH += chip_drv/chip_module/pcm/v5
MSRCPATH += chip_drv/chip_module/pin
MSRCPATH += chip_drv/chip_module/pwm
MSRCPATH += chip_drv/chip_module/pwm/v5
MSRCPATH += chip_drv/chip_module/rtc
MSRCPATH += chip_drv/chip_module/rotation
MSRCPATH += chip_drv/chip_module/rotation/uix8910
MSRCPATH += chip_drv/chip_module/rtc/uix8910
MSRCPATH += chip_drv/chip_module/sdio
MSRCPATH += chip_drv/chip_module/sdio/uix8910
MSRCPATH += chip_drv/chip_module/sim
MSRCPATH += chip_drv/chip_module/sim/v5
MSRCPATH += chip_drv/chip_module/sleep
MSRCPATH += chip_drv/chip_module/sleep/uix8910
MSRCPATH += chip_drv/chip_module/spi
MSRCPATH += chip_drv/chip_module/spi/uix8910
MSRCPATH += chip_drv/chip_module/spi/gpio_simu
MSRCPATH += chip_drv/chip_module/sfc
MSRCPATH += chip_drv/chip_module/sfc/uix8910
MSRCPATH += chip_drv/chip_module/sfc/sfc_common
MSRCPATH += chip_drv/chip_module/sys_timer
MSRCPATH += chip_drv/chip_module/sys_timer/dummy
MSRCPATH += chip_drv/chip_module/timer
MSRCPATH += chip_drv/chip_module/tpc
MSRCPATH += chip_drv/chip_module/tpc/r4p0
MSRCPATH += chip_drv/chip_module/uart
MSRCPATH += chip_drv/chip_module/usb
MSRCPATH += chip_drv/chip_module/watchdog
MSRCPATH += chip_drv/chip_module/watchdog/v5
MSRCPATH += chip_drv/chip_module/eic
MSRCPATH += chip_drv/chip_module/eic/uix8910
MSRCPATH += chip_drv/chip_module/audio
MSRCPATH += chip_drv/chip_module/audio/dai/vbc
MSRCPATH += chip_drv/chip_module/audio/codec/sprd
MSRCPATH += chip_drv/chip_module/audio/codec/sprd/v0
MSRCPATH += chip_drv/chip_module/audio/codec/sprd/v1
MSRCPATH += chip_drv/chip_module/fm/v0
MSRCPATH += chip_drv/chip_module/osc
MSRCPATH += chip_drv/chip_module/osc/sc6531efm
MSRCPATH += chip_drv/chip_module/sys_mail
MSRCPATH += chip_drv/chip_module/ifc/apifc
MSRCPATH += chip_drv/chip_module/spinlock/uix8910
MSRCPATH += chip_drv/chip_module/audio/codec/sprd/v2
MSRCPATH += chip_drv/chip_module/pagespy
MCFLAG_OPT += -DSPRD_AUDIO_USE_V1

ifeq ($(strip $(CHIP_VER)),    UIX8910MPW)
MINCPATH += chip_drv/chip_plf/uix8910mpw
MSRCPATH += chip_drv/chip_module/uart/uix8910mpw
MSRCPATH += chip_drv/chip_module/timer/uix8910mpw
MSRCPATH += chip_drv/chip_plf/uix8910mpw
MSRCPATH += chip_drv/chip_module/chip/uix8910mpw
MSRCPATH += chip_drv/chip_module/clock/uix8910mpw
MSRCPATH += chip_drv/chip_module/pin/uix8910mpw
MSRCPATH += chip_drv/chip_module/dma/uix8910mpw
MSRCPATH += chip_drv/chip_module/gpio/uix8910mpw
else
MINCPATH += chip_drv/chip_plf/uix8910
MINCPATH += chip_drv/chip_module/pin/uix8910
MSRCPATH += chip_drv/chip_module/uart/uix8910
MSRCPATH += chip_drv/chip_module/timer/uix8910
MSRCPATH += chip_drv/chip_plf/uix8910
MSRCPATH += chip_drv/chip_module/chip/uix8910
MSRCPATH += chip_drv/chip_module/clock/uix8910
MSRCPATH += chip_drv/chip_module/pin/uix8910
MSRCPATH += chip_drv/chip_module/dma/uix8910
MSRCPATH += chip_drv/chip_module/gpio/uix8910
MSRCPATH += chip_drv/chip_module/pagespy/uix8910
endif

ifneq ($(strip $(FPGA_VERIFICATION)), TRUE)
MCFLAG_OPT     += -DSDIO_HIGH_SPEED_UP_TO_48MHZ  
endif

MINCPATH += chip_drv/chip_module/charge

endif #platform UIX8910

ifeq ($(strip $(PLATFORM)),    UIX8910)

SOURCES +=    					adi_phy_uix8910.c                    \
                        lzma_phy_8910.c              \
                        analog_hal.c                    \
                        global_ctl_hal.c                \
                        sc6531efm_global_ctl.c              \
                        busmonitor_phy_uix8910.c             \
                        busmonitor_hal.c                \
                        cardlog_pal.c                   \
                        cardlog_uart.c                  \
                        charge_uix8910.c                 \
                        chg_phy_v7.c                    \
                        chip_phy_uix8910.c                \
                        mem_dump.c                      \
                        clock.c                         \
                        uix8910_clock_cfg.c               \
                        hash.c                          \
                        uix8910_dma_cfg.c                 \
                        dma_hal.c                       \
                        rotation_phy.c                  \
                        dma_phy_uix8910.c                  \
                        dsp_phy_v5.c                    \
                        dsp_hal.c                       \
                        smc_phy_uix8910.c                  \
                        efuse_phy_uix8910.c                  \
                        efuse_phy_uix8910_pmic.c                  \
                        efuse_hal.c                     \
                        freq_hal.c                      \
                        freq_phy_v5.c                   \
                        sc6531efm_freq_cfg.c                \
                        gea_phy_v5.c                    \
                        gpio_hal.c                      \
                        gpio_phy_uix8910.c                   \
                        uix8910_gpio_cfg.c                \
                        uix8910_i2c_cfg.c                 \
                        i2c_hal.c                       \
                        i2c_phy_uix8910.c                    \
                        i2c_phy_gpio_simu.c             \
                        tx_illdb.s                      \
                        boot_mode.c                     \
                        chip_drv_init.c                 \
                        int_hal.c                       \
                        int_hal_hisr.c                  \
                        int_phy_uix8910.c                  \
                        uix8910_int_cfg.c                 \
                        int_gic.s                       \
                        fiq_misc_drv.c                  \
                        kpd_phy_8910.c                    \
                        lcm_phy_v5.c                    \
                        lcd_if_spi.c                    \
                        lcd_if_hal.c                    \
                        hal_gouda.c                    \
                        ldo_cfg.c                       \
                        ldo_hal.c                       \
                        asm_cvt.c                       \
                        mcu_reboot.c                    \
                        misc.s                          \
                        retarget.c                      \
                        retarget_heap.c                 \
                        sio.c                           \
                        osi_fifo.c                      \
                        stack.s                         \
                        threadx_assert.c                \
                        pcm_drv_v5.c                    \
                        pinmap.c                        \
                        pwm_phy_v5.c                    \
                        rtc_phy_uix8910.c                    \
						sdio_dummy.c                    \
                        sim_drv.c                       \
                        sleep_asm_uix8910.s               \
                        sleep_aon_init_uix8910.s          \
                        sleep_phy_uix8910.c               \
                        sleep_oswtimer.c                  \
                        sleep_ddr_uix8910.c               \
                        uix8910_sleep_cfg.c               \
                        sleep_common.c                  \
                        spi_hal.c                       \
                        spi_phy_uix8910.c                    \
                        sys_timer_phy_dummy.c              \
                        timer_drv.c                     \
                        timer_hal.c                     \
                        timer_phy_uix8910.c                  \
                        uix8910_timer_cfg.c               \
                        uart_hal.c                      \
                        uart_phy_uix8910.c                 \
                        sc6531efm_audio_cfg.c               \
                        vbc_phy_v6.c                    \
                        vbc_alg_phy_v3.c                \
                        sprd_codec_dp_phy_v1.c          \
                        sprd_codec_phy.c                \
                        sprd_codec_state_v0.c           \
                        sprd_codec_phy_cfg.c            \
                        audio_codec_sprd.c              \
                        gx_udc.c                        \
                        uix8910_eic_cfg.c             \
                        eic_hal.c                       \
                        eic_phy_uix8910.c                    \
                        watchdog_hal.c                  \
                        watchdog_phy_v5.c               \
                        uix8910_mem_cfg.c             \
                        sfc_phy_uix8910_v2.c                    \
                        sfc_hal_v2.c                       \
                        cpu_endian.c                    \
                        adc_phy_r6p0.c                  \
                        usb_timer.c                     \
                        fm_phy_v0.c						\
                        osc_phy_sc6531efm.c				\
                        osc_hal.c						\
						sprd_codec_phy_v2.c             \
			hal_sdmmc.c                     \
                        analog_phy_sc2720.c 			\
                        sprd_codec_ap_phy_sc1161.c	    \
						sprd_codec_ap_phy_sc2720_headset.c	    \
                        md_ipc.c                        \
						nv_ipc.c                        \
						apifc_phy.c                     \
						spinlock_phy_uix8910.c          \
						pmic_26M_clock.c  						\
						load_modem.c                       \
						load_lzma_drv.c                       \
						fmd.c                              \
						mcu_clk.c                      \
						clk_div.c                      \
						pagespy_dmc.c                  \
						pagespy_dmc_phy_uix8910.c      \
						uix8910_spi_cfg.c
						
ifneq ($(strip $(MODEM_SYS_IN_AP_SIDE)), TRUE)
SOURCES += uix8910_stub.c						
MINCPATH += BASE/l4/export/inc  \
			BASE/layer1/export/inc  \
			BASE/PS/export/inc \
			BASE/sim/export/inc

			
					
endif						

ifeq ($(strip $(FPGA_VERIFICATION)), TRUE)
ifeq ($(strip $(AUDIO_CODEC)), LM49370)
    MSRCPATH += chip_drv/chip_module/audio/codec/lm49370
    SOURCES +=    audio_codec_lm49370.c                    \
                  SPI_LM49370_v3.c
else
# USC15241-SHUTTLE
    MINCPATH += chip_drv/chip_module/audio/codec/sprd
    MSRCPATH += chip_drv/chip_module/audio/codec/usc15241
    MSRCPATH += chip_drv/chip_module/audio/codec/usc15241/ctl_bus
    SOURCES +=    audio_codec_usc15241.c                    \
                  usc15241.c                                \
                  usc15241_state_v0.c                       \
                  usc15241_phy_cfg.c                        \
                  usc15241_i2c_drv.c                        \
                  usc15241_spi_drv.c

endif
endif


MINCPATH += chip_drv/chip_module/charge
MINCPATH += MS_Ref/source/efs/inc
MINCPATH += lib/modem/$(PROJECT)
MINCPATH += lib/modem/$(PROJECT)/cp_info
MINCPATH += lib/modem/$(PROJECT)/zsp_info

endif #platform UIX8910

ifeq ($(strip $(NANDBOOT_SUPPORT)), TRUE)

    SOURCES +=              norless_dummy.c
        
else                
MCFLAG_OPT     += -DNOR_SRAM_PLATFORM    
MASMFLAG_OPT	+= -PD "NOR_SRAM_PLATFORM SETL {TRUE}"
ifeq ($(strip $(CHIP_VER)), 6531)
    SOURCES +=              smc_hal.c 
else ifeq ($(strip $(CHIP_VER)), 6531EFM)
	SOURCES +=              smc_hal.c 
else ifneq ($(findstring $(strip $(CHIP_VER)),UIX8910 UIX8910MPW UIS8910C UIS8910A UIS8910FF),)
    SOURCES +=              smc_hal.c 
else
    SOURCES +=              emc_hal.c 
endif

    SOURCES +=              flash.c

endif

ifeq ($(strip $(USB_PROTOCOL_SUPPORT)), USB20)
SOURCES             += 	udc_polling.c usb20_app_config.c vcom_usb20.c usb_common.c
endif

ifeq ($(strip $(USB_PROTOCOL_SUPPORT)), USB11)
SOURCES             += 	vcom_usb.c usb_drv_e.c usb_drv_i.c
endif	

ifeq ($(strip $(GPIO_SIMULATE_SPI_SUPPORT)), TRUE)
SOURCES             +=         	spi_phy_gpio_simu.c
endif

ifeq ($(strip $(DUAL_BATTERY_SUPPORT)), TRUE)
SOURCES +=    dualbat_hal.c
endif


SOURCES             +=            spi_hal_com.c
    
###AUDIO ### ken.kuang add ##
SOURCES             +=             audio_hal.c
SOURCES             +=             audio_old_api.c
    
SOURCES             +=             audio_dai_vbc.c

####AUDIO END#####


