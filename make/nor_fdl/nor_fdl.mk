include Makefile.rules
include ./project_$(PROJECT).mk
include Makefile.verify
include Makefile.rules

ifneq ($(strip $(MAKECMDGOALS)),clean)
  $(call CO_OPTION_TO_FILE,$(C_MACRO),$(BUILD_DIR)/dep/nor_fdl_C_MACRO.txt)
  $(call A_OPTION_TO_FILE, $(A_MACRO),$(BUILD_DIR)/dep/nor_fdl_A_MACRO.txt)
endif

PROD_CONFIG := $(shell $(PERL) ./make/perl_script/get_dirname.pl MS_Customize/source/product/config $(PROJECT) $(strip $(PRODUCT_CONFIG)))

MCFLAG_OPT   	= $(CPU) $(__CPU__) -O2  $(ENDIAN_OPT) $(APCS) /interwork
MASMFLAG_OPT    = $(CPU) $(__CPU__) $(KEEP) $(CODE16)  $(ENDIAN_OPT) $(APCS) /interwork

ifeq ($(strip $(COMPILER)),	RVCT)
MCFLAG_OPT  += --debug
MASMFLAG_OPT += --debug
else
MCFLAG_OPT  += -g+
MASMFLAG_OPT += -g
endif

CONFIGPATH      =  MS_Customize/source/product/config
DRVPATH         =  MS_Customize/source/product/driver

ifeq ($(strip $(PLATFORM)),	SC6530)
MCFLAG_OPT 		+= -DNOR_FDL_SC6800H  -D_LITTLE_ENDIAN

ifeq ($(strip $(CHIP_VER)),	6531)
SCATFILENAME 	= fdl_bootloader/nor_fdl/src/sc6531_scatter.scf
else
SCATFILENAME 	= fdl_bootloader/nor_fdl/src/sc6530_scatter.scf
endif
endif

ifeq ($(strip $(PLATFORM)),	SC6531EFM)
MCFLAG_OPT 		+= -D_LITTLE_ENDIAN
SCATFILENAME 	= fdl_bootloader/nor_fdl/src/sc6531efm_scatter.scf
endif

ifeq ($(strip $(PLATFORM)),	UIX8910)
MCFLAG_OPT 		+= -D_LITTLE_ENDIAN
SCATFILENAME 	= fdl_bootloader/nor_fdl/src/uix8910_scatter.scf
endif

MCFLAG_OPT 		+= -D_NEW_NORFDL

MINCPATH  = -Ifdl_bootloader/nor_fdl/inc            \
            -Ifdl_bootloader/chip_drv/inc           \
            -Ifdl_bootloader/common/inc             \
            -Ifdl_bootloader/chip_drv/src/watchdog  \
            -Ichip_drv/export/inc                   \
            -Ichip_drv/export/inc/outdated          \
            -Ichip_drv/chip_plf/export/inc          \
            -Ichip_drv/chip_module/watchdog/v5      \
            -Ichip_drv/chip_module/sys_timer/v5     \
            -I$(strip $(PS_PATH))/Layer1/export/inc \
            -IRTOS/export/inc                       \
            -Icommon/export/inc                     \
            -IMS_Customize/export/inc               \
            -I$(DRVPATH)/spiflash/inc               \
            -I$(DRVPATH)/norflash/normcp_info       \
            -IMS_Ref/export/inc                         

ifeq ($(strip $(SECURE_BOOT_SUPPORT)),	TRUE)
MINCPATH +=	-IMS_Ref/source/security/inc			
endif

ifeq ($(strip $(PLATFORM)),	SC6530)
MINCPATH += -Ichip_drv/chip_plf/sc6530
endif

ifeq ($(strip $(PLATFORM)),	SC6531EFM)
MINCPATH += -Ichip_drv/chip_plf/sc6531efm
endif	   

ifeq ($(strip $(PLATFORM)),	UIX8910)

ifeq ($(strip $(CHIP_VER)),	UIX8910MPW)
MINCPATH += -Ifdl_bootloader/chip_drv/inc/uix8910mpw \
            -Ichip_drv/chip_plf/uix8910mpw
else
ifneq "$(filter $(strip $(CHIP_VER)), UIX8910 UIS8910C UIS8910A UIS8910FF)" ""
MINCPATH += -Ifdl_bootloader/chip_drv/inc/uix8910  \
            -Ichip_drv/chip_plf/uix8910
endif
endif
endif	           

ifeq ($(strip $(PLATFORM)),	SC6530)

ifeq ($(strip $(CHIP_VER)),	6531)
MINCPATH += -Ichip_drv/chip_module/int/r2p0 \
	        -Ichip_drv/chip_module/usb/r3p0 
else
MINCPATH += -Ichip_drv/chip_module/int/v5 \
	        -Ichip_drv/chip_module/usb/v5 
endif
endif	        

ifeq ($(strip $(PLATFORM)),	SC6531EFM)
MINCPATH += -Ichip_drv/chip_module/int/r2p0 \
	        -Ichip_drv/chip_module/usb/r3p0 
endif
            
ifeq ($(strip $(PLATFORM)),	UIX8910)
MINCPATH += -Ichip_drv/chip_module/int/r2p0 \
	        -Ichip_drv/chip_module/usb/r3p0 
endif            
	    
MINCPATH +=  -I$(CONFIGPATH)/$(strip $(PROD_CONFIG))

SRCPATH	   =  fdl_bootloader/nor_fdl/src \
              $(DRVPATH)/norflash \
              fdl_bootloader/chip_drv/src \
              fdl_bootloader/common/src \
              fdl_bootloader/chip_drv/src/usb_boot/usb11 

ifeq ($(strip $(PLATFORM)),	SC6530)
ifeq ($(strip $(CHIP_VER)),	6531)
SRCPATH	   += fdl_bootloader/chip_drv/src/smc \
              fdl_bootloader/chip_drv/src/smc/r1p0 \
              fdl_bootloader/chip_drv/src/chip/v5 
endif
SRCPATH	   += fdl_bootloader/chip_drv/src/mmu 
endif

ifeq ($(strip $(PLATFORM)),	SC6531EFM)
SRCPATH	   += fdl_bootloader/chip_drv/src/chip/v5 \
              fdl_bootloader/chip_drv/src/mmu 
endif

ifeq ($(strip $(PLATFORM)),	UIX8910)
SRCPATH	   += fdl_bootloader/chip_drv/src/mmu \
              fdl_bootloader/chip_drv/src/mmu/arm_v7 \
              fdl_bootloader/chip_drv/src/usb_boot/usb11/usb_uix8910 \
              chip_drv/chip_module/sfc/uix8910
ifeq ($(strip $(CHIP_VER)),	UIX8910MPW)
SRCPATH	   += fdl_bootloader/chip_drv/src/chip/uix8910mpw \
              fdl_bootloader/chip_drv/src/smc/uix8910mpw
endif
ifneq "$(filter $(strip $(CHIP_VER)), UIX8910 UIS8910C UIS8910A UIS8910FF)" ""
SRCPATH	   += fdl_bootloader/chip_drv/src/chip/uix8910 \
              fdl_bootloader/chip_drv/src/smc/uix8910
endif
endif

SRCPATH	   +=  $(CONFIGPATH)/$(strip $(PROD_CONFIG))

SRCPATH	   += fdl_bootloader/chip_drv/src/watchdog
SRCPATH	   += chip_drv/chip_module/adi/v5
SRCPATH	   += fdl_bootloader/chip_drv/src/watchdog/v5
SRCPATH    += fdl_bootloader/chip_drv/src/efuse

ifeq ($(strip $(SECURE_BOOT_SUPPORT)),TRUE)
SRCPATH	   += MS_Ref/source/security/src
SRCPATH	   += fdl_bootloader/common/src/algo
endif

SOURCES		= 	fdl_crc.c           \
		  		fdl_main.c          \
		  		fdl_stdio.c	        \
				fdl_flash_cfg.c     \
				fdl_trc.c           \
				fdl_sys.c           \
				packet.c	        \
				drv_usb.c           \
				sio_drv.c	        \
				usb_boot.c	        \
				virtual_com.c       \
				init.s              \
				fdl_channel.c       \
				fdl_update_fixnv.c  \
				detect_error.c      \
				dl_engine.c         \
				chip_cfg.c          \
				adi_drv.c           \
				efuse_drv.c           \
				watchdog_hal.c      \
				watchdog_phy_v5.c

ifeq ($(strip $(SECURE_BOOT_SUPPORT)),    TRUE)
SOURCES 	+= secure_verify.c sha1_32.c hash_drv.c 
SOURCES 	+= rsa.c	\
						aes.c	\
						rsa_multidw.s	\
						rsa_sub.s 
endif
				
ifeq ($(strip $(FLASH_TYPE)),	SPI)
ifeq ($(strip $(PLATFORM)),	UIX8910)
SOURCES		+=	sfc_phy_uix8910_v2.c   \
					spiflash_v2.c      \
					spiflash_cfg.c
else
SOURCES		+=	sfc_phy_v5.c        \
					spiflash.c      \
					spiflash_cfg.c    
endif
else
SOURCES		+=	flash_drv.c         \
					normcp_spec.c   \
					flash_cfg.c					
endif

ifeq ($(strip $(PLATFORM)),	SC6530)
ifeq ($(strip $(CHIP_VER)),	6531)
SOURCES += 	smc_hal.c \
            smc_phy_r1p0.c \
            chip_phy_v5.c
endif
SOURCES +=  mmu.c \
            mmu_asm.s
endif

ifeq ($(strip $(PLATFORM)),	SC6531EFM)
SOURCES += 	mmu.c \
            mmu_asm.s
endif

ifeq ($(strip $(PLATFORM)),	UIX8910)
SOURCES += 	mmu.c \
            v7_mmu_cache.s \
            v7_mmu.c \
            clk_div.c \
            clock.c \
            core.c \
            dwc2_udc.c \
            dwc2_udc_rda.c \
            ep0.c \
            usb-serial.c \
            serial.c
endif


OBJECTS1    := $(patsubst %.c, %.o, $(SOURCES))
OBJECTS     := $(patsubst %.s, %.o, $(OBJECTS1))						

vpath
vpath %.o  $(BUILD_DIR)/obj/nor_fdl
vpath %.d  $(BUILD_DIR)/dep/nor_fdl
vpath %.h  $(subst \,/,$(MINCPATH))
vpath %.c  $(subst \,/,$(SRCPATH))
vpath %.s  $(subst \,/,$(SRCPATH))
DEPPATH	=  $(BUILD_DIR)/dep/nor_fdl

all: builddir $(OBJECTS)
ifeq ($(strip $(PLATFORM)), SC6530)
ifeq ($(strip $(COMPILER)),	RVCT)
	@$(LINK) --info totals --entry 0x34000000 --scatter $(SCATFILENAME) --map --symbols --list fdl_nor.map  --info sizes --xref \
	$(addprefix $(BUILD_DIR)/obj/nor_fdl/, $(OBJECTS)) --Output $(BUILD_DIR)/fdl_bootloader/nor_fdl.axf
	@$(FROMELF) -c --bin $(BUILD_DIR)/fdl_bootloader/nor_fdl.axf --output $(BUILD_DIR)/fdl_bootloader/nor_fdl.bin
else
	@$(LINK) -info totals -entry 0x34000000 -scatter $(SCATFILENAME) -map -symbols -list fdl_nor.map -first init.o -info sizes -xref \
    $(addprefix $(BUILD_DIR)/obj/nor_fdl/, $(OBJECTS)) -Output $(BUILD_DIR)/fdl_bootloader/nor_fdl.axf
	@$(FROMELF) -c -bin $(BUILD_DIR)/fdl_bootloader/nor_fdl.axf -output $(BUILD_DIR)/fdl_bootloader/nor_fdl.bin
endif	
endif

ifeq ($(strip $(PLATFORM)), SC6531EFM)
ifeq ($(strip $(COMPILER)),	RVCT)
	@$(LINK) --info totals --entry 0x14000010 --scatter $(SCATFILENAME) --map --symbols --list fdl_nor.map  --info sizes --xref \
	$(addprefix $(BUILD_DIR)/obj/nor_fdl/, $(OBJECTS)) --Output $(BUILD_DIR)/fdl_bootloader/nor_fdl.axf
	@$(FROMELF) -c --bin $(BUILD_DIR)/fdl_bootloader/nor_fdl.axf --output $(BUILD_DIR)/fdl_bootloader/nor_fdl.bin
else
	@$(LINK) -info totals -entry 0x14000010 -scatter $(SCATFILENAME) -map -symbols -list fdl_nor.map -first init.o -info sizes -xref \
    $(addprefix $(BUILD_DIR)/obj/nor_fdl/, $(OBJECTS)) -Output $(BUILD_DIR)/fdl_bootloader/nor_fdl.axf
	@$(FROMELF) -c -bin $(BUILD_DIR)/fdl_bootloader/nor_fdl.axf -output $(BUILD_DIR)/fdl_bootloader/nor_fdl.bin
endif	
endif

ifeq ($(strip $(PLATFORM)), UIX8910)
ifeq ($(strip $(COMPILER)),	RVCT)
	@$(LINK) --info totals --entry 0x80008600 --scatter $(SCATFILENAME) --map --symbols --list fdl_nor.map  --info sizes --xref \
	$(addprefix $(BUILD_DIR)/obj/nor_fdl/, $(OBJECTS)) --Output $(BUILD_DIR)/fdl_bootloader/nor_fdl.axf
	@$(FROMELF) -c --bin $(BUILD_DIR)/fdl_bootloader/nor_fdl.axf --output $(BUILD_DIR)/fdl_bootloader/nor_fdl.bin
else
	@$(LINK) -info totals -entry 0x80008600 -scatter $(SCATFILENAME) -map -symbols -list fdl_nor.map -first init.o -info sizes -xref \
    $(addprefix $(BUILD_DIR)/obj/nor_fdl/, $(OBJECTS)) -Output $(BUILD_DIR)/fdl_bootloader/nor_fdl.axf
	@$(FROMELF) -c -bin $(BUILD_DIR)/fdl_bootloader/nor_fdl.axf -output $(BUILD_DIR)/fdl_bootloader/nor_fdl.bin
endif	
endif

	@-$(PERL) make/perl_script/rm_file.pl $(BUILD_DIR)/img/nor_fdl.bin ""
	@$(COPY_FILE) $(call Path_process,$(BUILD_DIR)/fdl_bootloader/nor_fdl.bin) $(call Path_process,$(BUILD_DIR)/img/nor_fdl.bin)

ifneq ($(strip $(MAKE_VAR_DEPEND_D)),0)
-include $(OBJECTS:.o=.d)
endif

%.d %.o:%.s
	@$(ECHO)
	@$(ECHO) Compiling Source File $<...
	$(ASM) $(MASMFLAG_OPT) $(VIA) $(BUILD_DIR)/dep/nor_fdl_A_MACRO.txt $(INCPATH) $<  $(MDFLAGS) ./$(DEPPATH)/$*.d.tmp  -o $(BUILD_DIR)/obj/nor_fdl/$*.o
	@$(PERL) ./make/perl_script/path_process.pl ./$(DEPPATH)/$*.d.tmp  make/nor_fdl/nor_fdl.mk "$(MAKESHELL)"> ./$(DEPPATH)/$*.d
	@-$(PERL) make/perl_script/rm_file.pl ./$(DEPPATH)/$*.d.tmp ""
%.d %.o:%.c
	@$(ECHO)
	@$(ECHO) Compiling Source File $<...
	$(CC) $(IGNOR_WARNNINGS) $(MCFLAG_OPT) $(VIA) $(BUILD_DIR)/dep/nor_fdl_C_MACRO.txt -c $(MINCPATH) $(INCPATH) $< $(MDFLAGS) ./$(DEPPATH)/$*.d.tmp -o $(BUILD_DIR)/obj/nor_fdl/$*.o
	@$(PERL) ./make/perl_script/path_process.pl ./$(DEPPATH)/$*.d.tmp  make/nor_fdl/nor_fdl.mk "$(MAKESHELL)"> ./$(DEPPATH)/$*.d
	@-$(PERL) make/perl_script/rm_file.pl ./$(DEPPATH)/$*.d.tmp ""

.PHONY:builddir
builddir:
	@$(PERL) make/perl_script/mk_dir.pl "fdl_bootloader log dep obj" $(BUILD_DIR)
	@$(PERL) make/perl_script/mk_dir.pl "nor_fdl" $(BUILD_DIR)/obj

.PHONY:clean
clean:
	@-$(PERL) make/perl_script/rm_file.pl nor_fdl.bin $(BUILD_DIR)/img/ 
	@-$(PERL) make/perl_script/rm_file.pl nor_fdl.bin $(BUILD_DIR)/fdl_bootloader/
	@-$(PERL) make/perl_script/rm_dir.pl nor_fdl $(BUILD_DIR)/obj
	@-$(PERL) make/perl_script/rm_dir.pl nor_fdl $(BUILD_DIR)/dep
