/*
 * Copyright 2002-2003 Aplix Corporation. All rights reserved.
 */


#ifndef KJAVA_SYS_FS_INTERNAL_HEADER_
#define KJAVA_SYS_FS_INTERNAL_HEADER_

/**
 * @.h
 *
 */

#ifdef __cplusplus
extern "C"{
#endif


/*-----------------------------------------------------------------------------
 * Include Files
 *----------------------------------------------------------------------------*/



/*-----------------------------------------------------------------------------
 * Marcos, Constants, Types, and Structures
 *----------------------------------------------------------------------------*/
#define CONST_KJAVA_FILE_PATHSEP        '\\'

//#define CONST_NUM_AUTHORTIES            1       /* only \java\ is allowed */
#define CONST_MAX_NUM_FILE_HANDLES      20      /* max allowed file handles */
#define CONST_MAX_PATH_LEGNTH           256

/*
const KJavaPathMappingEntry PathMappingEntry[CONST_NUM_AUTHORTIES] =
{
    { "java", "D:\\@Java\\" },
};
*/
extern const KJavaPathMappingEntry PathMappingEntry[CONST_NUM_AUTHORTIES];

/* file handle */
typedef struct _KJavaFileHandleSlot{
    int              used;
    int              mode;
    SFS_HANDLE        handle;    /* kal fils handle */
    unsigned short   filename[CONST_MAX_PATH_LEGNTH];
} KJavaFileHandleSlot;

extern KJavaFileHandleSlot g_handle[CONST_MAX_NUM_FILE_HANDLES]; /* file handle pool */



/*-----------------------------------------------------------------------------
 * Function Prototypes
 *----------------------------------------------------------------------------*/





#ifdef __cplusplus
}
#endif

#endif /* KJAVA_SYS_FS_INTERNAL_HEADER_ */

