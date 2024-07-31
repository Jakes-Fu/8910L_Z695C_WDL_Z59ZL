include Makefile.rules
include ./project_$(PROJECT).mk
include Makefile.verify
include Makefile.rules

ifneq ($(strip $(MAKECMDGOALS)),clean)
  $(call CO_OPTION_TO_FILE,$(C_MACRO),$(BUILD_DIR)/dep/fota_bootloader_C_MACRO.txt)
  $(call A_OPTION_TO_FILE, $(A_MACRO),$(BUILD_DIR)/dep/fota_bootloader_A_MACRO.txt)
endif

PROD_CONFIG := $(shell $(PERL) ./make/perl_script/get_dirname.pl MS_Customize/source/product/config $(PROJECT) $(strip $(PRODUCT_CONFIG)))

CONFIGPATH   =  MS_Customize/source/product/config
DRVPATH      =  MS_Customize/source/product/driver

MCFLAG_OPT      += -DOS_NONE -DTF_FDL_BUILD -g --debug
MASMFLAG_OPT	+= -g --debug

MCFLAG_OPT      += -DTF_UPDATE_SC6530

ifeq ($(strip $(CHIP_ENDIAN)), LITTLE)
MCFLAG_OPT      += -DTF_LITTLE_ENDIAN
MASMFLAG_OPT    += -PD "TF_LITTLE_ENDIAN SETL {TRUE}"
endif

ifeq ($(strip $(PLATFORM)),	UIX8910)
MCFLAG_OPT 		+= -D_LITTLE_ENDIAN
SCATFILENAME 	= fdl_bootloader/fota_bootloader/src/uix8910_scatter.scf
endif

MINCPATH  = -Ifdl_bootloader/fota_bootloader/inc\
            -Ifdl_bootloader/fdl2/inc\
            -Ifdl_bootloader/common/inc\
            -Ichip_drv/export/inc \
            -Ichip_drv/source/h \
            -IRTOS/export/inc \
            -Icommon/export/inc \
            -Icommon/export/inc/trace_id \
            -IMS_Customize/export/inc \
            -I$(CONFIGPATH)/$(PRODUCT_CONFIG) \
            -I$(DRVPATH)/norflash/normcp_info\
            -IMS_Ref/export/inc \
            -IMS_Ref/source/scp/inc \
            -I$(DRVPATH)/tf_load/inc\
            -Ifdl_bootloader/nor_fdl/inc\
            -IMS_Ref/source/lcd/inc\
            -Ichip_drv/chip_module/gpio\
            -Ibuild/$(PROJECT)_builddir/tmp\
            -Ichip_drv/export/inc/outdated \
            -IMS_Customize/source/product/driver/spiflash/inc\
            -Ibase/ps/export/inc   \
            -Ifdl_bootloader/chip_drv/inc    

ifeq ($(strip $(MULTI_SIM_SYS)), TRI)
  MINCPATH  += -Itrisim/Layer1/export/inc
else
  MINCPATH  += -ILayer1/export/inc
endif

MINCPATH  += -Ifdl_bootloader/fota_bootloader/src/tf_lcd/inc
MINCPATH +=  -I$(CONFIGPATH)/$(strip $(PROJECT))

SRCPATH    =  fdl_bootloader/fota_bootloader/src \
              fdl_bootloader/fota_bootloader/src/nv \
              $(CONFIGPATH)/$(PRODUCT_CONFIG) \
              MS_Customize/source/product/driver/lcd\
              chip_drv/source/c \
              chip_drv/chip_module/ldo \
              chip_drv/chip_module/clock \
              chip_drv/chip_module/chip/uix8910 \
              chip_drv/chip_module/adi/uix8910 \
              $(DRVPATH)/nandflash\
              MS_Ref/source/scp/src\
              chip_drv/chip_module/gpio\
              chip_drv/chip_module/gpio/uix8910\
              chip_drv/chip_module/mmu/uix8910\
              chip_drv/chip_module/lcdc/v5\
              fdl_bootloader/chip_drv/src/watchdog\
              fdl_bootloader/chip_drv/src\
              version

SRCPATH   +=  fdl_bootloader/fota_bootloader/src/tf_lcd/src
SRCPATH   +=  MS_Ref/source/lcd/src
SRCPATH   +=  $(CONFIGPATH)/$(strip $(PROJECT))
SRCPATH   +=  fdl_bootloader/nor_fdl/src
SRCPATH   +=  fdl_bootloader/fota_bootloader/src/fs

SOURCES   =   tf_init.s \
                fota_bootloader_main.c \
                tf_display.c \
                tf_dummy.c \
                tf_str.c \
                tf_lcmprod.c \
                tf_lcmcfg.c \
                cc936.c \
                crc16.c \
                card_sdio.c \
                sdio_card_pal.c \
                serial.c

SOURCES   +=  ff.c diskio_sdio.c
 
ifeq ($(strip $(NANDBOOT_SUPPORT)), TRUE)
  SOURCES   += 	tf_main_nand.c\
				tf_nv_nand.c\
				tf_drv_nand.c\
				nand_spec.c				
else
  SOURCES   += 	tf_main_nor.c\
				tf_parse_nor.c\
				tf_nv_nor.c
		
  ifeq ($(strip $(FLASH_TYPE)), SPI)
  ifeq ($(strip $(PLATFORM)),	UIX8910)
    SOURCES	+=spiflash_v2.c spiflash_cfg.c
  else
    SOURCES	+=spiflash.c spiflash_cfg.c
  endif
  else
    SOURCES += flash_drv.c fdl_flash_cfg.c
  endif
endif


ifeq ($(strip $(PLATFORM)),	UIX8910)
  MINCPATH += -Ichip_drv/export/inc/outdated
  MINCPATH += -Ichip_drv/chip_plf
  MINCPATH += -Ichip_drv/chip_plf/uix8910
  MINCPATH += -Ichip_drv/chip_plf/export/inc
  MINCPATH += -Ichip_drv/chip_module/adi/uix8910
  MINCPATH += -Ichip_drv/chip_module/analog/sc2720
  MINCPATH += -Ichip_drv/chip_module/mmu
  MINCPATH += -Ichip_drv/chip_module/lcdc/uix8910
  MINCPATH += -Ichip_drv/chip_module/sdio/uix8910
  MINCPATH += -Icommon/export/inc/trace_id/
  MINCPATH += -IMS_Ref/source/scp/inc

  SRCPATH   +=  chip_drv/chip_module/sdio/uix8910 \
                chip_drv/chip_module/sdio/v5 \
                chip_drv/chip_module/lcdc/uix8910 \
                chip_drv/chip_module/sfc/uix8910 \
                chip_drv/chip_module/timer/uix8910 \
                chip_drv/chip_module/spinlock/uix8910 \
                chip_drv/chip_module/chip/uix8910 \
                chip_drv/chip_module/efuse \
                chip_drv/chip_module/efuse/uix8910 \
                chip_drv/chip_plf/uix8910 \
                chip_drv/chip_module/int/uix8910 \
                chip_drv/chip_module/ifc/apifc \
                chip_drv/chip_module/analog/sc2720 \
                chip_drv/chip_module/adi/uix8910 \
                chip_drv/chip_module/spinlock/uix8910 \
                chip_drv/chip_module/lcdc_v5 \
                chip_drv/chip_module/spi \
                chip_drv/chip_module/spi/v5 \
                chip_drv/chip_module/dma \
                chip_drv/chip_module/dma/uix8910 \
                chip_drv/chip_module/gpio \
                chip_drv/chip_module/gpio/uix8910 \
                fdl_bootloader/chip_drv/src/mmu \
                fdl_bootloader/chip_drv/src/usb_boot/usb11/usb_uix8910

  SOURCES   +=  mcd_sdmmc.c \
              hal_sdmmc.c \
              hal_gouda.c \
              apifc_phy.c \
              analog_phy_sc2720.c \
              adi_phy_uix8910.c \
              spinlock_phy_uix8910.c \
              sfc_phy_uix8910_v2.c \
              tf_lcd_uix8910.c \
              ldo_cfg.c \
              ldo_hal.c \
              efuse_phy_uix8910.c \
              efuse_phy_uix8910_pmic.c \
              efuse_hal.c \

  SOURCES   +=  tft_GC9305.c \
                tft_GC9307.c \
                tft_st7789v2.c \
                tft_JD9851.c
endif

################# ABUP FOTA START ################
ifeq ($(strip $(FOTA_SUPPORT)), ABUP)
SRCPATH += Third-party/abup/hal/src
MINCPATH += -IThird-party/abup/hal/include
SOURCES  += abup_bl_main.c \
			abup_bl_flash.c
ABUPFOTALIBS =Third-party/abup/lib/abup_wosun.a
endif
################# ABUP FOTA END ################
ifeq ($(strip $(RELEASE_INFO)), TRUE)
MCFLAG_OPT   	+= -D_VERSION_RELEASE_
else
MCFLAG_OPT   	+= -D_VERSION_DEBUG_
endif
             	

OBJECTS1    := $(patsubst %.c, %.o, $(SOURCES))
OBJECTS     := $(patsubst %.s, %.o, $(OBJECTS1))                        

vpath
vpath %.o  $(BUILD_DIR)/obj/fota_bootloader
vpath %.d  $(BUILD_DIR)/dep/fota_bootloader
vpath %.h  $(subst \,/,$(MINCPATH))
vpath %.c  $(subst \,/,$(SRCPATH))
vpath %.s  $(subst \,/,$(SRCPATH))
DEPPATH =  $(BUILD_DIR)/dep/fota_bootloader

all: builddir $(OBJECTS)

ifeq ($(strip $(FOTA_SUPPORT)), ABUP)
	@$(LINK) --info totals --map --symbols --scatter $(SCATFILENAME) --list fota_bootloader_$(PROJECT).map --first init.o --info sizes \
	$(addprefix $(BUILD_DIR)/obj/fota_bootloader/, $(OBJECTS)) $(ABUPFOTALIBS) --Output $(BUILD_DIR)/fota_bootloader/fota_bootloader.axf
else
	@$(LINK) --info totals --map --symbols --scatter $(SCATFILENAME) --list fota_bootloader_$(PROJECT).map --first init.o --info sizes \
	$(addprefix $(BUILD_DIR)/obj/fota_bootloader/, $(OBJECTS)) --Output $(BUILD_DIR)/fota_bootloader/fota_bootloader.axf
endif
	@$(FROMELF) -c --bin $(BUILD_DIR)/fota_bootloader/fota_bootloader.axf --output $(BUILD_DIR)/fota_bootloader/fota_bootloader.bin	
	@-$(PERL) make/perl_script/rm_file.pl $(BUILD_DIR)/img/fota_bootloader.bin ""
	@$(COPY_FILE) $(call Path_process,$(BUILD_DIR)/fota_bootloader/fota_bootloader.bin) $(call Path_process,$(BUILD_DIR)/img/fota_bootloader.bin)
	
ifneq ($(strip $(MAKECMDGOALS)),clean)
-include $(OBJECTS:.o=.d)
endif
%.d %.o:%.s
	@$(ECHO)
	@$(ECHO) Compiling Source File $<...
	$(ASM) $(MASMFLAG_OPT) $(AFLAGS) $(VIA) $(BUILD_DIR)/dep/fota_bootloader_A_MACRO.txt $(INCPATH) $<  $(MDFLAGS) $(DEPPATH)/$*.d.tmp  -o $(BUILD_DIR)/obj/fota_bootloader/$*.o
	@$(PERL) ./make/perl_script/path_process.pl ./$(DEPPATH)/$*.d.tmp  make/fota_bootloader/fota_bootloader.mk "$(MAKESHELL)"> ./$(DEPPATH)/$*.d
	@-$(PERL) make/perl_script/rm_file.pl ./$(DEPPATH)/$*.d.tmp ""
%.d %.o:%.c
	@$(ECHO)
	@$(ECHO) Compiling Source File $<...
	$(CC) $(MCFLAG_OPT) $(CFLAGS) $(VIA) $(BUILD_DIR)/dep/fota_bootloader_C_MACRO.txt -c $(MINCPATH) $(INCPATH) $< $(MDFLAGS) $(DEPPATH)/$*.d.tmp -o $(BUILD_DIR)/obj/fota_bootloader/$*.o
	@$(PERL) ./make/perl_script/path_process.pl ./$(DEPPATH)/$*.d.tmp  make/fota_bootloader/fota_bootloader.mk "$(MAKESHELL)"> ./$(DEPPATH)/$*.d
	@-$(PERL) make/perl_script/rm_file.pl $*.d.tmp $(DEPPATH)/

.PHONY:builddir
builddir:
	@$(PERL) make/perl_script/mk_dir.pl "fota_bootloader log dep obj" $(BUILD_DIR)
	@$(PERL) make/perl_script/mk_dir.pl "fota_bootloader" $(BUILD_DIR)/obj
	@$(PERL) make/perl_script/mk_dir.pl "fota_bootloader" $(BUILD_DIR)/dep

.PHONY:clean
clean:
	@-$(PERL) make/perl_script/rm_file.pl fota_bootloader.bin $(BUILD_DIR)/img/
	@-$(PERL) make/perl_script/rm_file.pl fota_bootloader.bin $(BUILD_DIR)/fdl_bootloader/fota_bootloader/
	@-$(PERL) make/perl_script/rm_dir.pl fota_bootloader $(BUILD_DIR)/obj
	@-$(PERL) make/perl_script/rm_dir.pl fota_bootloader $(BUILD_DIR)/dep
