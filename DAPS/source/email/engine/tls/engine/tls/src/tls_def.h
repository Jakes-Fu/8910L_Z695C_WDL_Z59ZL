#include "tf3rd_for_hz.h"
#include "we_def.h"

#define TLS_MALLOC        TF3RD_MALLOCMEMORY
#define TLS_FREE          TF3RD_MEMFREE
#define TLS_SIZEOF        sizeof
#define TLS_MEMSET        TF3RD_MEMSET
#define TLS_STRDUP        TF3RD_STRDUP
#define TLS_MEMCMP        TF3RD_MEMCMP
#define TLS_MEMCPY        TF3RD_MEMCPY
#define TLS_STRLEN        TF3RD_STRLEN
// #ifndef WE_UINT8
// typedef  unsigned char     WE_UINT8;
// #endif
// 
// #ifndef WE_UINT16
// typedef  unsigned short     WE_UINT16;
// #endif
// 
// #ifndef WE_UINT32
//  typedef  unsigned int  WE_UINT32;
// #endif
// 
// #ifndef WE_INT
// typedef  int                WE_INT;
// #endif
// 
// #ifndef WE_INT32
// typedef  int                WE_INT32;
// 
// #endif
// 
// #ifndef WE_UINT
// typedef  unsigned int       WE_UINT;
// #endif
// 
// #ifndef WE_HANDLE
// typedef  void *             WE_HANDLE;
// #endif
// 
// #ifndef WE_BOOL
// typedef  unsigned char      WE_BOOL;
// #endif
// 
// #ifndef WE_WCHAR
// typedef  unsigned short     WE_WCHAR;
// #endif
// 
// #ifndef WE_VOID
// typedef  void               WE_VOID;
// 
// #endif
// 
// #ifndef WE_LONG
// typedef  long              WE_LONG;
// #endif
// 
// #ifndef WE_UCHAR
// typedef  unsigned char     WE_UCHAR;
// #endif
// 
// #ifndef WE_CHAR
// typedef  char              WE_CHAR;
// #endif
// 
// #ifndef NULL
// #define NULL     (void*)0
// #endif
