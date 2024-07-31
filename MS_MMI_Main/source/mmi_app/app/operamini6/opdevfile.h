/* -*- Mode: c++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*-
 *
 * Copyright (C) 1995-2011 Opera Software AS.  All rights reserved.
 *
 * This file is part of the Opera web browser.
 * It may not be distributed under any circumstances.
 */

#ifndef OPDEV_FILE_H
#define OPDEV_FILE_H

#include "optype.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/**
 * Context container for I/O operations.
 */
typedef struct _OD_FileContext
{
	/**
	 * Asynchronous callback information.
	 *
	 * If an asynchronous operation is requested, please make sure
	 * this field is correctly set before <b>ANY</b> call is going
	 * to be scheduled.
	 *
	 * If this is being set to NULL then a synchronous operation will
	 * be performed instead.
	 */
	OD_AsyncCall *call;
} OD_FileContext;

#define OD_MAX_DRIVE_COUNT			(64)

/**
 * Asynchronous operation types.
 */
typedef enum _OD_FileAsyncCallType
{
	/** Open. */
	OD_FILE_ASYNC_CALL_OPEN = 0,

	/** Commit pending writes. */
	OD_FILE_ASYNC_CALL_COMMIT,

	/** Delete. */
	OD_FILE_ASYNC_CALL_DELETE,

	/** Rename. */
	OD_FILE_ASYNC_CALL_RENAME,

	/** Read. */
	OD_FILE_ASYNC_CALL_READ,

	/** Write. */
	OD_FILE_ASYNC_CALL_WRITE,
} OD_FileAsyncCallType;

/**
 * Asynchronous call parameter structure.
 */
typedef struct _OD_FileAsyncCallParam
{
	/**
	 * Asynchronous call type.
	 *
	 * @see OD_FileAsyncCallType
	 */
	OD_FileAsyncCallType type;

	/**
	 * Handle to operate upon.
	 *
	 * @see OD_FileHandle
	 */
	OD_FileHandle handle;

	/**
	 * Filename to operate upon, used when handle is not needed.
	 *
	 * @see OD_FileName
	 */
	OD_FileName *filename;
} OD_FileAsyncCallParam;

/**
 * Callback definition for asynchronous I/O operations.
 *
 * Its usage is as follows:
 *
 * \code
 * OD_FileAsyncCallParam *param = malloc(sizeof(OD_FileAsyncCallParam));
 * OD_AsyncCall *call = malloc(sizeof(OD_AsyncCall));
 * OD_FileName *filename = NULL;
 * filename = tstrdup(file); // If handles are not needed.
 * call.sender = sender_task;
 * call.receiver = receiver_task;
 * call.call = fileAsyncCallback;
 * param.filename = filename;
 * call.param = param; // .type and .handle will be set for you.
 * context->data.file.call = call;
 * status = OD_FileRead(context, fh, buf, len, read);
 * \endcode
 *
 * and when responding to such an I/O operation:
 *
 * \code
 * void fileAsyncCallback(void* userParam)
 * {
 * 	OD_FileAsyncCallParam param = (OD_FileAsyncCallParam *)userParam;
 *      switch (param->type)
 *      {
 *      	// Finish I/O
 *      }
 *
 *      if (param.filename)
 *      	free(param.filename);
 *      free(param);
 * }
 * \endcode
 */
typedef void (*FileAsyncCallback)(void*);

/**
 * File open modes.
 */
typedef enum tag_OD_FileOpenMode
{
	/** Allowed to read. */
	OD_FILE_READ            = 1 << 0, /* 0x0001 */

	/** Allowed to write. */
	OD_FILE_WRITE           = 1 << 1, /* 0x0002 */

	/** Do not truncate file upon opening. */
	OD_FILE_APPEND          = 1 << 2, /* 0x0004 */

	/** Shared read. */
	OD_FILE_SHAREDREAD      = 1 << 3, /* 0x0008 */

	/** Shared write. */
	OD_FILE_SHAREDWRITE     = 1 << 4, /* 0x0010 */

	/** Overwrite upon opening. */
	OD_FILE_CREATE_ALWAYS   = 1 << 5, /* 0x0020 */

	/** Open directory for traversal. */
	OD_FILE_OPEN_DIR        = 1 << 6  /* 0x0040 */
} OD_FileOpenMode;

/**
 * File position seek points.
 */
typedef enum tag_OD_FileSeek
{
	/** Beginning of the file. */
	OD_SEEK_FROM_START,

	/** End of the file. */
	OD_SEEK_FROM_END,

	/** Current pointer. */
	OD_SEEK_FROM_CURRENT
} OD_FileSeek;

/**
 * Filesystem attributes bitmask.
 */
typedef enum tag_OD_FileAttr
{
	/** None. */
	OD_FILEATTR_NONE        = 0x00,

	/** Read only. */
	OD_FILEATTR_READ_ONLY   = 0x01,

	/** Hidden. */
	OD_FILEATTR_HIDDEN      = 0x02,

	/** System. */
	OD_FILEATTR_SYSTEM      = 0x04,

	/** Volume. */
	OD_FILEATTR_VOLUME      = 0x08,

	/** Directory. */
	OD_FILEATTR_DIR         = 0x10,

	/** Archive. Not used currently. */
	OD_FILEATTR_ARCHIVE     = 0x20,
} OD_FileAttr;

/**
 * Predefined paths for standard document types.
 */
typedef enum tag_OD_PredefinedPathType
{
	/** Pictures. */
	OD_PREDEFINEDPATH_TYPE_PICTURE = 0,

	/** Videos. */
	OD_PREDEFINEDPATH_TYPE_VIDEO = 1,

	/** Sound clips. */
	OD_PREDEFINEDPATH_TYPE_SOUND = 2,

	/** Music files. */
	OD_PREDEFINEDPATH_TYPE_MUSIC = 3,

	/** Documents. */
	OD_PREDEFINEDPATH_TYPE_DOCUMENT = 4,

	/** Anything else. */
	OD_PREDEFINEDPATH_TYPE_OTHER = 5,
} OD_PredefinedPathType;

/**
 * Packed timestamp to retain data in YYYY/MM/DD HH:MM:SS format.
 *
 * Please note that granularity is two seconds in order to be able to
 * fit a whole timestamp in a single 32-bits integer.
 *
 * Format is as follows:
 * <pre>
 *  3         2         1
 * 10987654321098765432109876543210
 * |     ||  ||   ||   ||    ||   |
 * |     ||  ||   ||   ||    |+---+-----> (5 bits) Seconds / 2
 * |     ||  ||   ||   ||    |
 * |     ||  ||   ||   |+----+----------> (6 bits) Minutes
 * |     ||  ||   ||   |
 * |     ||  ||   |+---+----------------> (5 bits) Hours
 * |     ||  ||   |
 * |     ||  |+---+---------------------> (5 bits) Days
 * |     ||  |
 * |     |+--+--------------------------> (4 bits) Months
 * |     |
 * +-----+------------------------------> (7 bits) Years after 1980
 * </pre>
 */
typedef unsigned int OD_PackedTimestamp;

/**
 * File handle query counter identifiers.
 */
typedef enum tag_OD_FileHandleCountType
{
	/** File handles in use by the current task. */
	OD_FILEHANDLE_COUNT_CURRENT_TASK = 0,

	/** File handles still available to the current task. */
	OD_FILEHANDLE_COUNT_AVAILABLE,

	/** File handles in use by all tasks combined. */
	OD_FILEHANDLE_COUNT_USED,

	/**
	 * Maximum number of file handles available to all tasks
	 * combined.
	 */
	OD_FILEHANDLE_COUNT_CONFIGURED
} OD_FileHandleCountType;


/************************************************************************/
/* Context                                                              */
/************************************************************************/

/**
 * Initialises the file I/O component given context.
 *
 * This must be called before attempting to perform any I/O operation,
 * or the behaviour of such operations will be unpredictable and generally
 * not what you want it to be.
 *
 * Multiple calls to #OD_FileDeinit are safe to be made, with no reentrant
 * behaviour.
 *
 * @see OD_FileDeinit
 *
 * @param[in] context the context.
 *
 * @return OD_SUCCESS if the operation succeeded, OD_NULL_POINTER if
 *         context is NULL, or the appropriate error code from the
 *         OD_Error enumeration.
 */
OD_ALL_TASK OD_STATUS OD_FileInit(OD_Context *context);

/**
 * Deinitialises the file I/O component given context.
 *
 * This must be called before closing the application or when being sure
 * that no more I/O operations are going to be performed in the future.
 *
 * It is safe to call #OD_FileInit afterwards, and multiple calls to
 * #OD_FileDeinit are safe to be made, with no reentrant behaviour.
 *
 * @see OD_FileInit
 *
 * @param[in] context the context.
 *
 * @return OD_SUCCESS if the operation succeeded, OD_NULL_POINTER if
 *         context is NULL, or the appropriate error code from the
 *         OD_Error enumeration.
 */
OD_ALL_TASK OD_STATUS OD_FileDeinit(OD_Context *context);

/************************************************************************/
/* File                                                                 */
/************************************************************************/

/**
 * Returns the value of the selected filehandler counter, or an error code
 * if it's the case.
 *
 * @param[in] request_type the filehandle counter to query.
 *
 * @return the value of the queried filehandle counter.
 */
OD_ALL_TASK int OD_CountFileHandles(OD_Context *context, OD_FileHandleCountType type);

/**
 * Opens a file with the given path using the given access flags.
 *
 * In case of failure, the variable pointed by handle will contain
 * OD_INVALID_FILE_HANDLE.
 *
 * @param[in] context the context.
 * @param[in] filename the path of the file to open.
 * @param[in] flags the access flags for the file.
 * @param[out] handle a pointer to the file handle variable container.
 *
 * @return OD_SUCCESS if the operation succeeded, OD_NULL_POINTER if
 *         any pointers are NULL, or the appropriate error code from the
 *         OD_Error enumeration.
 */
OD_ALL_TASK OD_STATUS OD_FileOpen(OD_Context *context, const OD_FileName *filename, OD_FileOpenMode flags, OD_FileHandle *handle);

/**
 * Closes a file referenced by the given handle.
 *
 * @param[in] context the context.
 * @param[in] handle the file handle to close.
 *
 * @return OD_SUCCESS if the operation succeeded, OD_NULL_POINTER if
 *         context is NULL, or the appropriate error code from the
 *         OD_Error enumeration.
 */
OD_ALL_TASK OD_STATUS OD_FileClose(OD_Context *context, OD_FileHandle handle);

/**
 * Commits pending write operations on the given handle.
 *
 * @param[in] context the context.
 * @param[in] handle the file handle to close.
 *
 * @return OD_SUCCESS if the operation succeeded, OD_NULL_POINTER if
 *         context is NULL, or the appropriate error code from the
 *         OD_Error enumeration.
 */
OD_ALL_TASK OD_STATUS OD_FileCommit(OD_Context *context, OD_FileHandle handle);

/**
 * Deletes a file with the given path.
 *
 * @see OD_XDelete
 *
 * @param[in] context the context.
 * @param[in] filename the path of the file to open.
 *
 * @return OD_SUCCESS if the operation succeeded, OD_NULL_POINTER if
 *         any pointers are NULL, or the appropriate error code from the
 *         OD_Error enumeration.
 */
OD_ALL_TASK OD_STATUS OD_FileDelete(OD_Context *context, const OD_FileName *filename);

/**
 * Extends the size of the file referenced from the given file handle by
 * the given amount of bytes.
 *
 * If length is equal than the current file size, the operation will take
 * no effect.  If length is more than the current file size,
 * the file size will be increased to match the given value, but there are
 * no guarantees on the content of the newly appended data.  There are
 * also no guarantees on whether the file pointer will not move.
 * If length is less than the current file size, the file position will
 * move to the given offset and the rest of the data will be discarded,
 * effectively resizing the file to the given amount of bytes.  A call to 
 * #OD_FileCommit is recommended afterwards, although it might not be
 * necessary.
 *
 * @param[in] context the context.
 * @param[in] handle the file handle.
 * @param[in] length the new file size, in bytes.
 *
 * @return OD_SUCCESS if the operation succeeded, OD_NULL_POINTER if
 *         context is NULL, or the appropriate error code from the
 *         OD_Error enumeration.
 */
OD_ALL_TASK OD_STATUS OD_FileExtend(OD_Context *context, OD_FileHandle handle, unsigned int length);

/**
 * Checks whether the given file does exist.
 *
 * @param[in] context the context.
 * @param[in] filename the name of the file to check the existence of, for folders it can end with or without path separator.
 *
 * @return OD_SUCCESS if the operation succeeded, OD_NULL_POINTER if
 *         any pointers are NULL, or the appropriate error code from the
 *         OD_Error enumeration.
 */
OD_ALL_TASK OD_BOOL OD_FileExist(OD_Context *context, const OD_FileName *filename);

/**
 * Retrieves the filesystem attributes of a given file.
 *
 * @see OD_FileAttr
 * @see OD_FileSetAttributes
 *
 * @param[in] context the context.
 * @param[in] filename the file name to retrieve filesystem attributes for.
 * @param[out] attr a pointer to a variable containing the attributes.
 *
 * @return OD_SUCCESS if the operation succeeded, OD_NULL_POINTER if
 *         any pointers are NULL, or the appropriate error code from the
 *         OD_Error enumeration.
 */
OD_ALL_TASK OD_STATUS OD_FileGetAttributes(OD_Context *context, const OD_FileName *filename, OD_FileAttr *attr);

/**
 * Retrieves the length, in bytes, of a given file.
 *
 * @param[in] context the context.
 * @param[in] filename the file name to retrieve filesystem attributes of.
 * @param[out] size a pointer to a variable containing the file size.
 *
 * @return OD_SUCCESS if the operation succeeded, OD_NULL_POINTER if
 *         any pointers are NULL, or the appropriate error code from the
 *         OD_Error enumeration.
 */
OD_ALL_TASK OD_STATUS OD_FileGetLength(OD_Context *context, OD_FileHandle hFile, unsigned int* size);

/**
 * Retrieves the current file position of a given handle.
 *
 * @param[in] context the context.
 * @param[in] handle the handle to retrieve the position of.
 * @param[out] pos a pointer to a variable containing the position.
 *
 * @return OD_SUCCESS if the operation succeeded, OD_NULL_POINTER if
 *         any pointers are NULL, or the appropriate error code from the
 *         OD_Error enumeration.
 */
OD_ALL_TASK OD_STATUS OD_FileGetPosition(OD_Context *context, OD_FileHandle hFile, OD_INT64* pos);

/**
 * Gets the file timestamps for the given handle.
 *
 * @see OD_PackedTimestamp
 *
 * @param[in] handle the handle to retrieve data for.
 * @param[out] creation_timestamp a pointer to the creation timestamp
 *             variable.
 * @param[out] last_modification_timestamp a pointer to the last
 *             modification timestamp variable.
 *
 * @return OD_SUCCESS if the operation succeeded, OD_NULL_POINTER if
 *         any pointers are NULL, or the appropriate error code from the
 *         OD_Error enumeration.
 */
OD_ALL_TASK OD_STATUS OD_FileGetTimestampsFromHandle(OD_Context *context, OD_FileHandle hFile, OD_PackedTimestamp *creation_timestamp, OD_PackedTimestamp *last_modification_timestamp);

/**
 * Gets the file timestamps from file name.
 *
 * @see OD_PackedTimestamp
 *
 * @param[in] filename the file to retrieve data for.
 * @param[out] creation_timestamp a pointer to the creation timestamp
 *             variable.
 * @param[out] last_modification_timestamp a pointer to the last
 *             modification timestamp variable.
 *
 * @return OD_SUCCESS if the operation succeeded, OD_NULL_POINTER if
 *         any pointers are NULL, or the appropriate error code from the
 *         OD_Error enumeration.
 */
OD_ALL_TASK OD_STATUS OD_FileGetTimestampsFromAttributes(OD_Context *context, const OD_FileName *filename, OD_PackedTimestamp *creation_timestamp, OD_PackedTimestamp *last_modification_timestamp);

/**
 * Checks whether a given file is forwardable to the DRM subsystem.
 *
 * @warning NO checks on the content of filename are performed.  In case
 *          of invalid or missing filenames, I/O errors or such,
 *          OD_FALSE will be returned.  An assertion will trigger in
 *          debug mode if any NULL parameter is being passed.
 *
 * @param[in] context the context.
 * @param[in] filename the file name to retrieve filesystem attributes.
 *
 * @return OD_TRUE if it's forwardable, OD_FALSE otherwise.
 */
OD_ALL_TASK OD_BOOL OD_FileIsForwardable(OD_Context *context, const OD_FileName *filename);

/**
 * Reads data from the given handle.
 *
 * @param[in] context the context.
 * @param[in] handle the handle to read from.
 * @param[in] buffer the buffer that will contain data read from the
 *            handle.
 * @param[in] length the amount of data to read, in bytes.
 * @param[in] read a pointer to a variable containing the amount of bytes
 *            effectively read.
 *
 * @return OD_SUCCESS if the operation is successful, OD_NULL_POINTER if
 *         any pointer is NULL, OD_INVALID_PARAM if the length is less than
 *         0, or the appropriate error code from the OD_Error enumeration.
 */
OD_ALL_TASK OD_STATUS OD_FileRead(OD_Context *context, OD_FileHandle handle, void* buffer, unsigned int length, unsigned int* read);

/**
 * Renames the given file into another given name.
 *
 * @param[in] context the context.
 * @param[in] filename the name of the file to rename.
 * @param[in] newname the new name for the file.
 * 
 * @return OD_SUCCESS if the operation succeeded, OD_NULL_POINTER if
 *         any pointers are NULL, or the appropriate error code from the
 *         OD_Error enumeration.
 */
OD_ALL_TASK OD_STATUS OD_FileRename(OD_Context *context, const OD_FileName *filename, const OD_FileName *newname);

/**
 * Sets the filesystem attributes of a given file.
 *
 * @see OD_FileAttr
 * @see OD_FileGetAttributes
 *
 * @param[in] context the context.
 * @param[in] filename the file name to set filesystem attributes for.
 * @param[in] attr the new filesystem attributes for the file.
 *
 * @return OD_SUCCESS if the operation succeeded, OD_NULL_POINTER if
 *         any pointers are NULL, or the appropriate error code from the
 *         OD_Error enumeration.
 */
OD_ALL_TASK OD_STATUS OD_FileSetAttributes(OD_Context *context, const OD_FileName *filename, unsigned int attr);

/**
 * Sets the file pointer for the given file.
 *
 * @see OD_FileGetPosition
 * @see OD_FileSeek
 *
 * @param[in] context the context.
 * @param[in] handle the handle to set the file pointer for.
 * @param[in] offset the new file pointer position, in bytes.
 * @param[in] forward flag indicating the direction for the pointer to move
 *            to.
 * @param[in] whence the origin for the pointer move operation.
 *
 * @return OD_SUCCESS if the operation succeeded, OD_NULL_POINTER if
 *         context is NULL, or the appropriate error code from the
 *         OD_Error enumeration.
 */
OD_ALL_TASK OD_STATUS OD_FileSetPosition(OD_Context *context, OD_FileHandle handle, OD_INT64 offset, OD_FileSeek whence);

/**
 * Truncates the given file.
 *
 * Equivalent to <code>OD_FileExtend(context, handle, OD_FileGetPosition)</code>.
 *
 * @param[in] context the context.
 * @param[in] handle the handle to truncate.
 *
 * @return OD_SUCCESS if the operation succeeded, OD_NULL_POINTER if
 *         context is NULL, or the appropriate error code from the
 *         OD_Error enumeration.
 */
OD_ALL_TASK OD_STATUS OD_FileTruncate(OD_Context *context, OD_FileHandle handle);

/**
 * Writes data to the given handle.
 *
 * @param[in] context the context.
 * @param[in] handle the handle to write to.
 * @param[in] buffer the buffer that will contain data to write to the
 *            handle.
 * @param[in] length the amount of data to write, in bytes.
 * @param[in] written a pointer to a variable containing the amount of bytes
 *            effectively written.
 *
 * @return OD_SUCCESS if the operation is successful, OD_NULL_POINTER if
 *         any pointer is NULL, OD_INVALID_PARAM if the length is less than
 *         0, or the appropriate error code from the OD_Error enumeration.
 */
OD_ALL_TASK OD_STATUS OD_FileWrite(OD_Context *context, OD_FileHandle handle, void *buffer, unsigned int length, unsigned int *written);

/**
 * Finds the first file matching the given search information.
 *
 * @note Please remember to always call <code>OD_FindClose</code> once
 *       you're done matching files or handles will leak.
 *
 * @see OD_FindNext
 * @see OD_FindClose
 *
 * @param[in] context the context
 * @param[out] handle a pointer to the find operation handle.
 * @param[in] pattern the filename pattern to look for.
 * @param[in] attr the attributes to look for.
 * @param[in] attrmask the attributes that should NOT be present for a
 *            matching entry.
 * @param[out] filename a pointer to the buffer that will contain the name
 *             of the first entry found.
 * @param[in] maxlength the maximum length entries must have to match.
 * @param[out] size a pointer to the variable that will contain the size,
 *             in bytes, for the first entry found.
 * @param[out] attributes a pointer to the variable that will contain the
 *             attributes for the first entry found.
 *
 * @return OD_SUCCESS if the operation is successful, OD_NULL_POINTER if
 *         any pointer is NULL, OD_INVALID_PARAM if the length is less than
 *         0, or the appropriate error code from the OD_Error enumeration.
 */
OD_ALL_TASK OD_STATUS OD_FindFirst(OD_Context *context, OD_FileFindHandle *handle, const OD_FileName *pattern, OD_FileAttr attr, OD_FileAttr attrmask, OD_FileName *filename, unsigned int maxlength, unsigned int *size, OD_FileAttr *attributes);

/**
 * Finds the next file matching the search information of the given handle.
 *
 * @note Please remember to always call <code>OD_FindClose</code> once
 *       you're done matching files or handles will leak.
 *
 * @see OD_FindFirst
 * @see OD_FindClose
 *
 * @param[in] context the context
 * @param[in] handle a find operation handle returned by #OD_FindFirst.
 * @param[out] filename a pointer to the buffer that will contain the name
 *             of the first entry found.
 * @param[in] maxlength the maximum length entries must have to match.
 * @param[out] size a pointer to the variable that will contain the size,
 *             in bytes, for the first entry found.
 * @param[out] attributes a pointer to the variable that will contain the
 *             attributes for the first entry found.
 *
 * @return OD_SUCCESS if the operation is successful, OD_NULL_POINTER if
 *         any pointer is NULL, OD_INVALID_PARAM if the length is less than
 *         0, or the appropriate error code from the OD_Error enumeration.
 */
OD_ALL_TASK OD_STATUS OD_FindNext(OD_Context *context, OD_FileFindHandle handle, OD_FileName *filename, unsigned int maxlength, unsigned int *size, OD_FileAttr *attributes);

/**
 * Closes the given file search handle.
 *
 * @see OD_FindFirst
 * @see OD_FindNext
 *
 * @param[in] context the context
 * @param[in] handle a find operation handle returned by #OD_FindFirst.
 *
 * @return OD_SUCCESS if the operation is successful, OD_NULL_POINTER if
 *         context is NULL, or the appropriate error code from the OD_Error
 *         enumeration.
 */
OD_ALL_TASK OD_STATUS OD_FindClose(OD_Context *context, OD_FileFindHandle handle);

/************************************************************************/
/* Directory                                                            */
/************************************************************************/

/**
 * Creates a directory with the given name.
 *
 * This may return #OD_SUCCESS in case the given directory already exists.
 * Moreover, there are no guarantees that paths with multiple levels of
 * missing directories will be honoured (creating the appropriate missing
 * directories along the way), therefore please use this function to create
 * only single-level directories.
 *
 * @see OD_DirectoryRemove
 * @see OD_XDelete
 *
 * @param[in] context the context.
 * @param[in] dir the directory name, can end with or without path separator.
 *
 * @return OD_SUCCESS if the operation is successful, OD_NULL_POINTER if
 *         any pointer is NULL, or the appropriate error code from the
 *         OD_Error enumeration.
 */
OD_ALL_TASK OD_STATUS OD_DirectoryCreate(OD_Context *context, const OD_FileName *dir);

/**
 * Removes a directory with the given name.
 *
 * @see OD_DirectoryCreate
 * @see OD_XDelete
 *
 * This may return #OD_SUCCESS in case the given directory does not exist.
 * This will only work for empty directories, if you need to either remove
 * also the files inside the given directory or remove the whole directory
 * tree recursively please look at #OD_XDelete instead.
 *
 * @param[in] context the context.
 * @param[in] dir the directory name, can end with or without path separator.
 *
 * @return OD_SUCCESS if the operation is successful, OD_NULL_POINTER if
 *         any pointer is NULL, or the appropriate error code from the
 *         OD_Error enumeration.
 */
OD_ALL_TASK OD_STATUS OD_DirectoryRemove(OD_Context *context, const OD_FileName *dir);

/**
 * Returns the size, in bytes, of all the elements in the given directory.
 *
 * @param[in] context the context.
 * @param[in] dir the directory name, can end with or without path separator.
 * @param[out] size a pointer to the variable that will contain the size.
 *
 * @return OD_SUCCESS if the operation is successful, OD_NULL_POINTER if
 *         any pointer is NULL, or the appropriate error code from the
 *         OD_Error enumeration.
 */
OD_ALL_TASK OD_STATUS OD_DirectoryGetSize(OD_Context *context, const OD_FileName *dir, unsigned int *size);

/**
 * Returns the predefined path for the given file category.
 *
 * @see OD_PredefinedPathType
 *
 * @param[in] context the context.
 * @param[in] pathtype the file category to request a path for.
 * @param[out] path a pointer to the buffer that will contain the path.
 *
 * @return OD_SUCCESS if the operation is successful, OD_NULL_POINTER if
 *         any pointer is NULL, or the appropriate error code from the
 *         OD_Error enumeration.
 */
OD_ALL_TASK OD_STATUS OD_DirectoryGetPredefined(OD_Context *context, OD_PredefinedPathType pathtype, OD_FileName *path);

/************************************************************************/
/* Drive                                                                */
/************************************************************************/

/**
 * Gets the drive letter for the first memory card installed.
 *
 * @warning NO checks on the content of context are performed.  In case
 *          of invalid or missing context, -1 will be returned. An
 *          assertion will trigger in debug mode if any NULL parameter is
 *          being passed.
 *
 * @param[in] context the context.
 *
 * @return the drive letter such as 'E', -1 to indicate failure.
 */
OD_ALL_TASK int OD_GetCardDriveLetter(OD_Context *context);

/**
 * Gets the drive letter for the drive containing public/shared data.
 *
 * @warning NO checks on the content of context are performed.  In case
 *          of invalid or missing context, -1 will be returned. An
 *          assertion will trigger in debug mode if any NULL parameter is
 *          being passed.
 *
 * @param[in] context the context.
 *
 * @return the drive letter such as 'E', -1 to indicate failure.
 */
OD_ALL_TASK int OD_GetPublicDriveLetter(OD_Context *context);

/**
 * Gets the drive letter for the drive containing system data.
 *
 * @warning NO checks on the content of context are performed.  In case
 *          of invalid or missing context, -1 will be returned. An
 *          assertion will trigger in debug mode if any NULL parameter is
 *          being passed.
 *
 * @param[in] context the context.
 *
 * @return the drive letter such as 'E', -1 to indicate failure.
 */
OD_ALL_TASK int OD_GetSystemDriveLetter(OD_Context *context);

/**
 * Gets the drive letter for the drive containing downloaded data.
 *
 * @warning NO checks on the content of context are performed.  In case
 *          of missing context, -1 will be returned. An assertion will
 *          trigger in debug mode if any NULL parameter is being passed.
 *
 * @param[in] context the context.
 *
 * @return the drive letter such as 'E', -1 to indicate failure.
 */
OD_ALL_TASK int OD_GetStorageDriveLetter(OD_Context *context);

/**
 * Gets all the drive letters on the device that are visible to end user.
 *
 * @param[in] context the context.
 * @param[out] driveletters an array that will contain the drive letters.
 * @param[out] drivenames an array of pointers that will contain the
 *             human-readable names for the drives being found.
 * @param[out] num a pointer to the variable that will contain the amount
 *             of drives that have been found.
 *
 * @return OD_SUCCESS if the operation is successful, OD_NULL_POINTER if
 *         any pointer is NULL, or the appropriate error code from the
 *         OD_Error enumeration.
 */
OD_ALL_TASK OD_STATUS OD_DriveGetAllVisibleLetters(OD_Context *context, char driveletters[], OD_FileName *drivenames[], int* num);

/**
 * Gets the drive letter for the drive Opera has been launched from.
 *
 * @warning NO checks on the content of context are performed.  In case
 *          of invalid or missing context, -1 will be returned. An
 *          assertion will trigger in debug mode if any NULL parameter is
 *          being passed.
 *
 * @param[in] context the context.
 *
 * @return the drive letter such as 'E', -1 to indicate failure.
 */
OD_ALL_TASK int OD_DriveGetCurrentLetter(OD_Context *context);

/**
 * Gets the drive letters on the device that are assigned to system
 * storage units.
 *
 * @param[in] context the context.
 * @param[out] driveletters an array that will contain the drive letters.
 * @param[out] num a pointer to the variable that will contain the amount
 *             of drives that have been found.
 *
 * @return OD_SUCCESS if the operation is successful, OD_NULL_POINTER if
 *         any pointer is NULL, or the appropriate error code from the
 *         OD_Error enumeration.
 */
OD_ALL_TASK OD_STATUS OD_DriveGetSystemLetters(OD_Context *context, char driveletters[], unsigned int* num);

/**
* Gets the prestorage directory (must end with path separator, '/' or '\\' according to the platform), it can be defined by custom (in OpCfg.h).
* Prestorage directory is for locating read-only resources, such as locales.
*
* @param[in] context the context.
* @param[out] buf the buffer to receive the path.
* @param[in] bufLen length of the buffer.
*
* @return OD_SUCCESS if the operation is successful, or the appropriate error code from the
*         OD_Error enumeration.
*/
OD_ALL_TASK OD_STATUS OD_GetPrestorageFolder(OD_Context *context, OD_FileName *buf, unsigned int bufLen);

/**
* Gets the storage directory (must end with path separator, '/' or '\\' according to the platform), it can be defined by custom (in OpCfg.h).
* Storage directory is for locating read-write resources, such as settings, saved pages, etc.
*
* @param[in] context the context.
* @param[out] buf the buffer to receive the path.
* @param[in] bufLen length of the buffer.
*
* @return OD_SUCCESS if the operation is successful, or the appropriate error code from the
*         OD_Error enumeration.
*/
OD_ALL_TASK OD_STATUS OD_GetStorageFolder(OD_Context *context, OD_FileName *buf, unsigned int bufLen);

/**
 * Returns whether the given drive is read-only.
 *
 * @warning NO checks on the content of context and drive are performed.
 *          In case of missing context, OD_FALSE will be returned. An
 *          assertion will trigger in debug mode if any NULL parameter is
 *          being passed.
 *
 * @param[in] context the context.
 * @param[in] drive the drive letter to check.
 *
 * @return OD_FALSE if the device is read-only or if drive points to an
 *         invalid letter, OD_TRUE otherwise.
 */
OD_ALL_TASK OD_BOOL OD_DriveIsWritable(OD_Context *context, int drive);

/*
 * Get the freeSpace of given drive.
 * @param[in] driveName the drive's name
 * @param[out] num a pointer to the variable that will contain  amount
 *             of freeSpace of  given drive
 * @return OD_SUCCESS if the operation is successful, OD_NULL_POINTER if
 *         any pointer is NULL, or the appropriate error code from the
 *         OD_Error enumeration.
 */
OD_ALL_TASK OD_STATUS OD_GetDriveFreeSpace(OD_WCHAR * driveName, OD_INT64 * freeSpace);

/************************************************************************/
/* Util                                                                 */
/************************************************************************/

/**
 * Returns whether two given paths point to the same file.
 *
 * @see OD_PredefinedPathType
 *
 * @param[in] context the context.
 * @param[in] path1 the first path.
 * @param[in] path2 the second path.
 * @param[out] equal a pointer to a boolean variable that will contain
 *             OD_TRUE if the two paths match or OD_FALSE otherwise.
 * @param[out] path a pointer to the buffer that will contain the path.
 *
 * @return OD_SUCCESS if the operation is successful, OD_NULL_POINTER if
 *         any pointer is NULL, or the appropriate error code from the
 *         OD_Error enumeration.
 */
OD_ALL_TASK OD_STATUS OD_PathsEqual(OD_Context *context, const OD_FileName *path1, const OD_FileName *path2, OD_BOOL *equal);

/**
 * Deletes the given path recursively.
 *
 * If the given path points to a directory, this function will recursively
 * go down each subdirectory and remove everything under them.  If it
 * points to a file, it will be simply be deleted.
 *
 * @see OD_FileDelete
 *
 * @param[in] context the context.
 * @param[in] path the path to remove.
 * @return OD_SUCCESS if the operation is successful, OD_NULL_POINTER if
 *         either context or path are set to NULL, or the appropriate
 *         error code from the OD_Error enumeration.
 */
OD_ALL_TASK OD_STATUS OD_XDelete(OD_Context *context, const OD_FileName *path);


#ifdef __cplusplus
};
#endif /* __cplusplus */

#endif /* OPDEV_FILE_H */
