#ifndef __LZMA_PORT_H__
#define __LZMA_PORT_H__
#include "Lzma_dec.h"
#include "os_api.h"


#define  LZMA_IO        LZMA_IO_T

typedef struct LZMA_IO_Tag
{
   unsigned char* start_addr;
   unsigned int  offset;
   unsigned int  max_size;
}LZMA_IO_T;

#define  LZMA_IO        LZMA_IO_T


LZMA_IO* LZMA_IOOpen(unsigned char*  buf_addr, unsigned int buf_size);
size_t LZMA_IORead(void* buf, size_t size, size_t num, LZMA_IO* file);
size_t LZMA_IOWrite(void* buf, size_t size, size_t num, LZMA_IO* file);
int LZMA_IOClose(LZMA_IO* file);
int LZMA_IOTell(LZMA_IO* file);
ISzAlloc* LZMA_GetAllocFunc(void);

#endif