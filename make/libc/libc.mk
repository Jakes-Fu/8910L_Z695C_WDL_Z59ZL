#Preprocessor
#MCFLAG_OPT = --gnu
MCFLAG_OPT = -DKAL_ON_RTT

#Directory
MINCPATH = RTOS/export/inc
MINCPATH += MS_Ref/export/inc

#Local Dir inc
MINCPATH += external/libc

#Local Dir src
MSRCPATH = external/libc

SOURCES = fake_pthread.c fake_filesystem.c fake_memory.c fake_io.c
