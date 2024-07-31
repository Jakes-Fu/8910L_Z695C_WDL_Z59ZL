/*
 * Copyright 2002-2003 Aplix Corporation. All rights reserved.
 */

#ifndef KJAVA_SYS_DATA_HEADER
#define KJAVA_SYS_DATA_HEADER

/**
 * @file kjava_sys_data.h
 *
 */

#ifdef __cplusplus
extern "C"{
#endif

/** @addtogroup core_system_service_ksi
 *  @{
 */
/**
 *  @addtogroup kjava_sys_data ROMized data interface
 *  @{
 *  PIK romized data IO. Please refer to the \ref page_pik_introduction for
 *  details. 
 */

/*-----------------------------------------------------------------------------
 * Marcos, Constants, Types, and Structures
 *----------------------------------------------------------------------------*/
/** \brief The struct indicates each file/directory slot in romized file system */
typedef struct _KJavaROMDataTag
{
    /** \brief name */
	const char*				name;
	/** \brief data content */
	const char*	            data;
	/** \brief data length */
	int						dataLength;
	/** \brief parent directory */
	int						parent;
	/** \brief child directory */
	int						child;
	/** \brief sibling */
	int						sibling;
	/** \brief if is directory */
	int						directory;

} KJavaROMData;

/*-----------------------------------------------------------------------------
 * Global Function(s) Prototype
 *----------------------------------------------------------------------------*/
/**
 * \brief Get ROMized data pointer.
 *
 * <b>DO NOT</b> implements this KSI directly. The implementation of this 
 * KSI is by PreInstall Tool Kit. Please refer
 * to \ref page_pik_introduction for details.
 *
 */
void* kjava_core_getROMInfo(void);

/** @} */
/** @} */

#ifdef __cplusplus
}
#endif

#endif /* KJAVA_SYS_DATA_HEADER */
