ifeq ($(strip $(PLATFORM)), UIX8910)
MINCPATH = MS_Ref/source/rpc/inc \
           MS_Ref/export/inc \
           MS_Include/chip_drv

MSRCPATH = MS_Ref/source/rpc/src
endif

SOURCES = rpc_daemon.c \
          rpc_dispatch_ap.c \
          rpc_hello_ap.c \
          cfw_event_names.c \
          cfw_rpc_ap.c \
          rpc_audio_ap.c \
	  rpc_verify_interface_ap.c \
