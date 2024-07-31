
MCFLAG_OPT      = 

MASMFLAG_OPT    = 

ifeq ($(strip $(CHIP_VER)),    6531)
MCFLAG_OPT  += -DCHIP_DRV_SC6531
endif

ifeq ($(strip $(CHIP_VER)),    6531EFM)
MCFLAG_OPT  += -DCHIP_DRV_SC6531EFM
endif


ifneq "$(filter $(strip $(CHIP_VER)), UIX8910 UIX8910MPW UIS8910C UIS8910A UIS8910FF)" ""
MCFLAG_OPT  += -DCHIP_DRV_UIX8910
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

MINCPATH =  Third-party/xsr_stlmp/include
MINCPATH += chip_drv/export/inc/outdated
MINCPATH += chip_drv/export/inc
MINCPATH += chip_drv/chip_plf/export/inc

ifeq ($(strip $(CHIP_VER)), 6531)
MINCPATH += chip_drv/chip_lib/inc
else
ifeq ($(strip $(CHIP_VER)), 6531EFM)
MINCPATH += chip_drv/chip_lib/inc
else
ifneq "$(filter $(strip $(CHIP_VER)), UIX8910 UIX8910MPW UIS8910C UIS8910A UIS8910FF)" ""
MINCPATH += chip_drv/chip_lib/inc
else
MINCPATH += chip_drv/chip_module/tpc
MINCPATH += chip_drv/chip_module/tpc/v5
endif
endif
endif



ifeq ($(strip $(PLATFORM)), SC6530)

ifeq ($(strip $(CHIP_VER)), 6531)
MINCPATH += chip_drv/chip_plf/sc6531
MSRCPATH += chip_drv/chip_plf/sc6531
else
MINCPATH += chip_drv/chip_plf/sc6530
MSRCPATH += chip_drv/chip_plf/sc6530
endif
MSRCPATH += chip_drv/chip_module/mmu
MSRCPATH += chip_drv/chip_module/mmu/v0

ifeq ($(strip $(CHIP_VER)), 6531)
MSRCPATH += chip_drv/chip_lib/src
else
MSRCPATH += chip_drv/chip_module/tpc
MSRCPATH += chip_drv/chip_module/tpc/v5
endif

MSRCPATH += chip_drv/chip_module/vfp
MSRCPATH += chip_drv/chip_module/lzma
MSRCPATH += chip_drv/chip_module/lzma/v5

MCFLAG_OPT     += -DVB_DMA_SUPPORT -DSDIO_HIGH_SPEED_UP_TO_48MHZ
MASMFLAG_OPT    += -PD "MMU_ENABLE SETL {TRUE}"
endif

ifeq ($(strip $(PLATFORM)), SC6531EFM)
MINCPATH += chip_drv/chip_plf/sc6531efm
MSRCPATH += chip_drv/chip_plf/sc6531efm

MSRCPATH += chip_drv/chip_module/mmu
MSRCPATH += chip_drv/chip_module/mmu/v0

MSRCPATH += chip_drv/chip_lib/src

MSRCPATH += chip_drv/chip_module/lzma
MSRCPATH += chip_drv/chip_module/lzma/v5

MCFLAG_OPT     += -DVB_DMA_SUPPORT -DSDIO_HIGH_SPEED_UP_TO_48MHZ
MASMFLAG_OPT    += -PD "MMU_ENABLE SETL {TRUE}"
endif

ifeq ($(strip $(PLATFORM)), UIX8910)
MINCPATH += chip_drv/chip_plf/uix8910
MSRCPATH += chip_drv/chip_plf/uix8910

MSRCPATH += chip_drv/chip_module/mmu
MSRCPATH += chip_drv/chip_module/mmu/arm_v7

MSRCPATH += chip_drv/chip_lib/src

MSRCPATH += chip_drv/chip_module/lzma
MSRCPATH += chip_drv/chip_module/lzma/uix8910

MCFLAG_OPT     += -DVB_DMA_SUPPORT -DSDIO_HIGH_SPEED_UP_TO_48MHZ
MASMFLAG_OPT    += -PD "MMU_ENABLE SETL {TRUE}"
endif


ifeq ($(strip $(PLATFORM)),    UIX8910)
SOURCES +=              v7_mmu_cache.s                       \
                        lzma_hal.c                      \
                        lzma_phy_8910.c

SOURCES += uix8910_mmu_config.c

ifeq ($(strip $(VM_SUPPORT)),    TRUE)
SOURCES += mmu_bzp.c  mmu_vm_bzp.c v7_mmu.c
else
SOURCES += mmu.c mmu_vm.c                        
endif

SOURCES += tpc_algorithm.c

endif

