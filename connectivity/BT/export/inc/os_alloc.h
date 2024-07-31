#ifndef __OS_ALLOC_H__
#define  __OS_ALLOC_H__
#include "sci_types.h"

//void *OS_Pmalloc(uint32 size);
#define OS_Pmalloc SCI_ALLOC_APP

//void *OS_Zpmalloc(uint32 size);
void OS_Pfree(void *ptr);
void OS_PfreeAndSetNull(void **ptr);
#endif /* __OS_ALLOC_H__*/