/*--------------------------------------------------------------------------//
| Version Control:
|
|   2002/09/21,     [YH] Fake Memory-Management module.
|   2002/10/20,     [YH] First Implementation
|
//--------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------//
| Function List for Advanced Java File System:  
|
| ------------------------- General Operation ----------------------------
| F0: ajmm_init()           ,   Initialization for Memory Management
| F1: ajmm_malloc()         ,   Allocate a memory
| F2: ajmm_free()           ,   Free allocated memory
| ------------------------------------------------------------------------
|
//--------------------------------------------------------------------------*/

#ifndef AJMM_API_HEADER
#define AJMM_API_HEADER

#include <stddef.h>

//#define ENABLE_AJMM_TRACE

//----------------------------------------------------------------------//
/**
 *  \brief Initialize AJMM
 *
 *  @param  allocatedMemory     the allocated memory for AJMM
 *  @param  totalSize           total size of the allocated memory
 *
 */
//----------------------------------------------------------------------//
void ajmm_init(char* allocatedMemory, int totalSize);
void ajmm_fini(void);


#ifdef ENABLE_AJMM_TRACE

#ifndef MODULE_NAME
#define MODULE_NAME     __FILE__
#endif

#define ajmm_malloc(x)      ajmm_malloc_impl((x), MODULE_NAME, __LINE__)
#define ajmm_realloc(x,a)   ajmm_realloc_impl((x), (a), MODULE_NAME, __LINE__)
#define ajmm_free(x)        ajmm_free_impl((x), MODULE_NAME, __LINE__)
#define ajmm_calloc(n,s)    ajmm_calloc_impl((n), (s), MODULE_NAME, __LINE__)
#define ajmm_strdup(str)    ajmm_strdup_impl((str), MODULE_NAME, __LINE__)


#else /* !ENABLE_AJMM_TRACE */

#define ajmm_malloc(x)      ajmm_malloc_impl((x), NULL, 0)
#define ajmm_realloc(x,a)   ajmm_realloc_impl((x), (a), NULL, 0)
#define ajmm_free(x)        ajmm_free_impl((x), NULL, 0)
#define ajmm_calloc(n,s)    ajmm_calloc_impl((n), (s), NULL, 0)

#endif /* ENABLE_AJMM_TRACE */


void* ajmm_malloc_impl(size_t nbytes, char *module, int line);
void* ajmm_realloc_impl(void* aptr, size_t nbytes, char *module, int line);
void  ajmm_free_impl(void* aptr, char *module, int line);
void* ajmm_calloc_impl(size_t n, size_t s, char *module, int line);
char* ajmm_strdup_impl(const char *str, char *module, int line);


#endif
