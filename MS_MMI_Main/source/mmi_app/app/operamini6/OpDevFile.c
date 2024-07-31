/* -*- Mode: c++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*-
*
* Copyright (C) 1995-2011 Opera Software AS.  All rights reserved.
*
* This file is part of the Opera web browser.
* It may not be distributed under any circumstances.
*/

#include "opcfg.h"
#include "opdevinternal.h"
#include "opimport.h"

#include "sci_types.h"
#include "sfs.h"
#include "mmi_filetask.h"

// No interface to get available handlers on SPRD for now.
#define CHECKFILEHANDLE
//#define CHECKFILEHANDLE do {if (FS_CountUsedFH(FS_FH_COUNT_AVAILABLE) < 4) return OPDEV_ERR_NO_MEMORY;} while (0)

#define OD_CHECK_FILE_HANDLE(X, RET)		{OD_ASSERT((X) != OD_INVALID_FILE_HANDLE);if ((X) == OD_INVALID_FILE_HANDLE) return RET;}
#define OD_CHECK_FILE_HANDLE_RETURN_STATUS(X)	OD_CHECK_FILE_HANDLE(X, OD_INVALID_PARAM)

#define OD_CLONE_PATH_REMOVE_SEPARATOR(buf, path, pathLen) \
				OD_FileName buf[OD_FILE_NAME_MAX_LENGTH] = {0}; \
				\
				OD_ASSERT(pathLen <= OD_FILE_NAME_MAX_LENGTH); \
				od_wcsncpy(buf, path, (pathLen) - 1); \
				buf[(pathLen) - 1] = 0;

#define CONSTRUCT_CALLBACK_PACK(callback, func, call) \
            (   \
                (callback).complete_route = (SFS_COMPLETETION_ROUTINE)(func), \
                (callback).param = (unsigned int)(call), \
                &(callback) \
            )


OD_STATUS OD_FileInit(OD_Context *context)
{
    OD_STATUS ret = OD_SUCCESS;

	OD_CHECK_ARGS_RETURN_STATUS(context);
    context->data.file.call = NULL;
    return ret;
}

OD_STATUS OD_FileDeinit(OD_Context *context)
{
    OD_STATUS ret = OD_SUCCESS;

	OD_CHECK_ARGS_RETURN_STATUS(context);
    context->data.file.call = NULL;
    return ret;
}

static OD_STATUS _OD_TranslateFsError(int code)
{
    switch (code)
    {
    case SFS_NO_ERROR:              // SFS_ERROR_NONE == SFS_NO_ERROR
        return OD_SUCCESS;
    case SFS_ERROR_IO_PENDING:
        return OD_ERROR_PENDING;
    case SFS_ERROR_INVALID_PARAM:
        return OD_INVALID_PARAM;
    case SFS_ERROR_NOT_EXIST:
    case SFS_ERROR_SYSTEM:
    case SFS_ERROR_DEVICE:
        return OD_FILE_NOT_FOUND;
    case SFS_ERROR_HAS_EXIST:
        return OD_FILE_EXISTS;
    case SFS_ERROR_NO_SPACE:
        return OD_FILE_NO_DISK;
    case SFS_ERROR_ACCESS:
    case SFS_ERROR_DRM_ACCESS_DENIED:
        return OD_FILE_NO_ACCESS;
    default:
        break;
    }

    // can't translate into a specified error, return a generic error code.
    return OD_ERROR;
}

static void _OD_FileAsyncCallback(SFS_ERROR_E error, unsigned int userData, unsigned int param1, unsigned int param2) \
{ 
    OD_AsyncCall *call = (OD_AsyncCall*)userData;
    OD_PostCall(call);
}

static unsigned int _OD_TranslateFileOpenMode(int flags)
{
    // On SPRD the mode is divided into two parts: access & operation.
    // For access mode, only one mode can be used.
    unsigned int mode = 0;

    if (flags & OD_FILE_APPEND)
    {
        mode = SFS_MODE_APPEND;
    }
    else if (flags & OD_FILE_CREATE_ALWAYS || flags & OD_FILE_WRITE || flags & OD_FILE_SHAREDWRITE)
    {
        mode = SFS_MODE_CREATE_ALWAYS;
    }
    else    /* only UPDATE, READ, etc. */
    {
        mode = SFS_MODE_OPEN_EXISTING;
    }
    
    if (flags & OD_FILE_READ)
    {
        mode |= SFS_MODE_READ;
    }
    if (flags & OD_FILE_WRITE)
    {
        mode |= SFS_MODE_WRITE;
    }
    if (flags & OD_FILE_SHAREDREAD)
    {
        mode |= SFS_MODE_SHARE_READ;
    }
    if (flags & OD_FILE_SHAREDWRITE)
    {
        mode |= SFS_MODE_SHARE_WRITE;
    }

    // On SPRD only SFS_MODE_APPEDN is not sufficient, add Read/Write flag.
    if (flags & OD_FILE_APPEND)
    {
        mode |= SFS_MODE_READ;
        mode |= SFS_MODE_WRITE;
    }

    return mode;
}

static OD_BOOL _OD_IsDriveAvailable(OD_Context *context, const OD_WCHAR *driveName);

int OD_CountFileHandles(OD_Context *context, OD_FileHandleCountType type)
{
	OD_CHECK_ARGS(context, -1);

    // TODO, no interface on SPRD.
    OD_ASSERT(!"Not implemented!");
    return -1;
}

OD_STATUS OD_FileOpen(OD_Context *context, const OD_FileName *filename, OD_FileOpenMode flags, OD_FileHandle* handle)
{
    OD_STATUS result = OD_SUCCESS;
    unsigned int mode = 0;
    SFS_ERROR_E res = SFS_NO_ERROR;
    OD_WCHAR dir[OD_FILE_NAME_MAX_LENGTH] = {0};
	OD_WCHAR *fileNameOnly = NULL;
    OD_WCHAR *pos = NULL;
    SFS_OVERLAPPED_T callback = {0};
	SFS_HANDLE sfs_handle = 0;

	OD_TRACE_LOW("OD_FileOpen: filename:");
	OD_TRACE_W_LOW(filename);
    CHECKFILEHANDLE;

	OD_CHECK_ARGS_RETURN_STATUS(context);
	OD_CHECK_ARGS_RETURN_STATUS(filename);
	OD_CHECK_ARGS_RETURN_STATUS(handle);

    mode = _OD_TranslateFileOpenMode(flags);
	od_wcsncpy(dir, filename, OD_FILE_NAME_MAX_LENGTH - 1);
	dir[OD_FILE_NAME_MAX_LENGTH - 1] = L'\0';
	pos = (OD_WCHAR*)od_wcsrchr(dir, OD_FILE_DIRECTORY_SEPARATOR);
	if (!pos)
	{
#ifdef WIN32
		OD_ASSERT(!"This code assumes there's always a path separator in the file name.");
#endif /* WIN32 */
		return OD_FILE_NOT_FOUND;
	}

    fileNameOnly = pos + 1;
    *pos = L'\0';

    if (context->data.file.call != NULL)
    {
        OD_ASSERT(context->data.file.call->req.param != NULL);
        ((OD_FileAsyncCallParam*)context->data.file.call->req.param)->type = OD_FILE_ASYNC_CALL_OPEN;
        ((OD_FileAsyncCallParam*)context->data.file.call->req.param)->handle = OD_INVALID_FILE_HANDLE;
    }

    res = SFS_CreateFileAsyn(dir, fileNameOnly, mode, 0, 0, &sfs_handle, context->data.file.call != NULL ?
                                                    CONSTRUCT_CALLBACK_PACK(callback, _OD_FileAsyncCallback, context->data.file.call) :
                                                    NULL);

	*handle = sfs_handle;
    *pos = OD_FILE_DIRECTORY_SEPARATOR;

    return _OD_TranslateFsError(res);
}

OD_STATUS OD_FileClose(OD_Context *context, OD_FileHandle hFile)
{
	OD_CHECK_ARGS_RETURN_STATUS(context);
	OD_CHECK_FILE_HANDLE_RETURN_STATUS(hFile);

    return _OD_TranslateFsError(SFS_CloseFile(hFile));
}

OD_STATUS OD_FileCommit(OD_Context *context, OD_FileHandle hFile)
{
	// Dummy according to customer's request. They say it may potentially lead to performance issue to call SFS_FlushFile.
	return OD_SUCCESS;
    /*
	SFS_OVERLAPPED_T callback = {0};

	OD_CHECK_ARGS_RETURN_STATUS(context);
	OD_CHECK_FILE_HANDLE_RETURN_STATUS(hFile);

    if (context->data.file.call != NULL)
    {
        OD_ASSERT(context->data.file.call->req.param != NULL);
        ((OD_FileAsyncCallParam*)context->data.file.call->req.param)->type = OD_FILE_ASYNC_CALL_OPEN;
        ((OD_FileAsyncCallParam*)context->data.file.call->req.param)->handle = hFile;
    }
    return _OD_TranslateFsError(SFS_FlushFile(hFile, context->data.file.call != NULL ?
                                                        CONSTRUCT_CALLBACK_PACK(callback, _OD_FileAsyncCallback, context->data.file.call) :
                                                        NULL));
	*/
}

OD_STATUS OD_FileDelete(OD_Context *context, const OD_FileName *filename)
{
    SFS_OVERLAPPED_T callback = {0};
	OD_STATUS ret = OD_SUCCESS;

	OD_CHECK_ARGS_RETURN_STATUS(context);
	OD_CHECK_ARGS_RETURN_STATUS(filename);

    if (context->data.file.call != NULL)
    {
        OD_ASSERT(context->data.file.call->req.param != NULL);
        ((OD_FileAsyncCallParam*)context->data.file.call->req.param)->type = OD_FILE_ASYNC_CALL_OPEN;
        ((OD_FileAsyncCallParam*)context->data.file.call->req.param)->handle = OD_INVALID_FILE_HANDLE;
    }
    ret = _OD_TranslateFsError(SFS_DeleteFile(filename, context->data.file.call != NULL ?
                                                            CONSTRUCT_CALLBACK_PACK(callback, _OD_FileAsyncCallback, context->data.file.call) :
                                                            NULL));
	ret = OD_FALSE == OD_FileExist(context, filename) ? OD_SUCCESS : ret;

	OD_TRACE_LOW("OD_FileDelete: ret=%d, filename:", ret);
	OD_TRACE_W_LOW(filename);

	return ret;
}

OD_STATUS OD_FileExtend(OD_Context *context, OD_FileHandle hFile, unsigned int length)
{
	OD_CHECK_ARGS_RETURN_STATUS(context);
	OD_CHECK_FILE_HANDLE_RETURN_STATUS(hFile);

    return _OD_TranslateFsError(SFS_SetFileSize(hFile, length));
}

OD_BOOL OD_FileExist(OD_Context *context, const OD_FileName *filename)
{
    int res = SFS_NO_ERROR;
    unsigned short at;
	unsigned int len = od_wcslen(filename);

    CHECKFILEHANDLE;

	OD_CHECK_ARGS(context, OD_FALSE);
	OD_CHECK_ARGS(filename, OD_FALSE);

	OD_ASSERT(len > 0);

	if(od_wcscmp(filename + 1, L":\\") == 0)
	{
		/** workaround for root directory. */
		OD_WCHAR driveName[2] = {0};
		driveName[0] = filename[0];
		if(_OD_IsDriveAvailable(context, (const OD_WCHAR *)driveName))
			return OD_TRUE;
		return OD_FALSE;
	}

	if (len > 0 && filename[len - 1] == '\\')
	{
		OD_CLONE_PATH_REMOVE_SEPARATOR(buf, filename, len);
		res = SFS_GetAttr(buf, &at);
	}
	else
	{
		res = SFS_GetAttr(filename, &at);
	}

    if (res == SFS_NO_ERROR)
    {
        return OD_TRUE;
    }
    if(res == SFS_ERROR_NOT_EXIST)
    {
        return OD_FALSE;
    }

    return OD_FALSE;
}

OD_STATUS OD_FileGetAttributes(OD_Context *context, const OD_FileName *filename, OD_FileAttr *attr)
{
    int res = SFS_NO_ERROR;
    unsigned short at;

    CHECKFILEHANDLE;

	OD_CHECK_ARGS_RETURN_STATUS(context);
	OD_CHECK_ARGS_RETURN_STATUS(filename);
	OD_CHECK_ARGS_RETURN_STATUS(attr);

	*attr = 0;
	if(od_wcscmp(filename + 1, L":\\") == 0)
	{
		/** workaround for C:\ directory. We can't get attributes from such directory in SPRD.*/
		OD_WCHAR driveName[2] = {0};
		driveName[0] = filename[0];
		if(_OD_IsDriveAvailable(context, (const OD_WCHAR *)driveName))
		{
			*attr |= OD_FILEATTR_DIR;
			return OD_SUCCESS;
		}
		return OD_FILE_NO_DISK;
	}

    res = SFS_GetAttr(filename, &at);
    if (res != SFS_NO_ERROR)
    {
        return _OD_TranslateFsError(res);
    }

    if (at & SFS_ATTR_RO)
    {
        *attr |= OD_FILEATTR_READ_ONLY;
    }
    if (at & SFS_ATTR_HIDDEN)
    {
        *attr |= OD_FILEATTR_HIDDEN;
    }
    if (at & SFS_ATTR_SYS)
    {
        *attr |= OD_FILEATTR_SYSTEM;
    }
    if (at & SFS_ATTR_VOLUME)
    {
        *attr |= OD_FILEATTR_VOLUME;
    }
    if (at & SFS_ATTR_DIR)
    {
        *attr |= OD_FILEATTR_DIR;
    }
    if (at & SFS_ATTR_ARCH)
    {
        *attr |= OD_FILEATTR_ARCHIVE;
    }
    
    return OD_SUCCESS;
}

OD_STATUS OD_FileGetLength(OD_Context *context, OD_FileHandle hFile, unsigned int* size)
{
	OD_CHECK_ARGS_RETURN_STATUS(context);
	OD_CHECK_ARGS_RETURN_STATUS(size);
	OD_CHECK_FILE_HANDLE_RETURN_STATUS(hFile);

    return _OD_TranslateFsError(SFS_GetFileSize(hFile, (unsigned long int*)size));
}

OD_STATUS OD_FileGetPosition(OD_Context *context, OD_FileHandle hFile, OD_INT64* pos)
{
	unsigned int upos;
	OD_STATUS status;

	OD_CHECK_ARGS_RETURN_STATUS(context);
	OD_CHECK_ARGS_RETURN_STATUS(pos);
	OD_CHECK_FILE_HANDLE_RETURN_STATUS(hFile);

    status = _OD_TranslateFsError(SFS_GetFilePointer(hFile, SFS_SEEK_BEGIN, &upos));
    if(pos && status == OD_SUCCESS) *pos = upos;

    return status;
}

OD_STATUS OD_FileGetTimestampsFromHandle(OD_Context *context, OD_FileHandle hFile, OD_PackedTimestamp *creation_timestamp,
                                         OD_PackedTimestamp *last_modification_timestamp)
{
    OD_ASSERT(!"Not implemented!");

	OD_CHECK_ARGS_RETURN_STATUS(context);
	OD_CHECK_ARGS_RETURN_STATUS(creation_timestamp);
	OD_CHECK_ARGS_RETURN_STATUS(last_modification_timestamp);
	OD_CHECK_FILE_HANDLE_RETURN_STATUS(hFile);

    return OD_ERROR;
}

OD_STATUS OD_FileGetTimestampsFromAttributes(OD_Context *context, const OD_FileName *filename, OD_PackedTimestamp *creation_timestamp, OD_PackedTimestamp *last_modification_timestamp)
{
    SFS_FIND_DATA_T file_info = {0};
	SFS_HANDLE find_handle;

    CHECKFILEHANDLE;

    OD_ASSERT(filename != NULL);
	find_handle = SFS_FindFirstFile(filename, &file_info);
    if (find_handle!=0)
    {
		*creation_timestamp =
			file_info.create_time.sec +
			(file_info.create_time.min << 5) +
			(file_info.create_time.hour << 11) +
			(file_info.create_Date.mday << 16) +
			(file_info.create_Date.mon << 21) +
			(file_info.create_Date.year << 25);

		*last_modification_timestamp =
			file_info.modify_time.sec +
			(file_info.modify_time.min << 5) +
			(file_info.modify_time.hour << 11) +
			(file_info.modify_Date.mday << 16) +
			(file_info.modify_Date.mon << 21) +
			(file_info.modify_Date.year << 25);

		SFS_FindClose(find_handle);
		return OD_SUCCESS;
    }
    return OD_ERROR;
}

OD_BOOL OD_FileIsForwardable(OD_Context *context, const OD_FileName *filename)
{
    // TODO: return TRUE for now, check the DRM interface to supplement it.
    int forwardeable = OD_TRUE;

	OD_CHECK_ARGS(context, OD_FALSE);
	OD_CHECK_ARGS(filename, OD_FALSE);

    return forwardeable;
}

OD_STATUS OD_FileRead(OD_Context *context, OD_FileHandle hFile, void* buffer, unsigned int length, unsigned int* read)
{
    SFS_OVERLAPPED_T callback = {0};

	OD_CHECK_ARGS_RETURN_STATUS(context);
	OD_CHECK_ARGS_RETURN_STATUS(buffer);
	OD_CHECK_ARGS_RETURN_STATUS(read);
	OD_CHECK_FILE_HANDLE_RETURN_STATUS(hFile);

    if (context->data.file.call != NULL)
    {
        OD_ASSERT(context->data.file.call->req.param != NULL);
        ((OD_FileAsyncCallParam*)context->data.file.call->req.param)->type = OD_FILE_ASYNC_CALL_OPEN;
        ((OD_FileAsyncCallParam*)context->data.file.call->req.param)->handle = hFile;
    }
    if(length > 0)
    {
        return _OD_TranslateFsError(SFS_ReadFile(hFile, buffer, length, read, context->data.file.call != NULL ?
                                                                                CONSTRUCT_CALLBACK_PACK(callback, _OD_FileAsyncCallback, context->data.file.call) :
                                                                                NULL));
    }
    else
    {
        if (read) *read = 0;
        return OD_INVALID_PARAM;
    }
}

OD_STATUS OD_FileRename(OD_Context *context, const OD_FileName *filename, const OD_FileName *newname)
{
    SFS_OVERLAPPED_T callback = {0};
	OD_STATUS ret = OD_SUCCESS;
    CHECKFILEHANDLE;

	OD_CHECK_ARGS_RETURN_STATUS(context);
	OD_CHECK_ARGS_RETURN_STATUS(filename);
	OD_CHECK_ARGS_RETURN_STATUS(newname);

    if (context->data.file.call != NULL)
    {
        OD_ASSERT(context->data.file.call->req.param != NULL);
        ((OD_FileAsyncCallParam*)context->data.file.call->req.param)->type = OD_FILE_ASYNC_CALL_OPEN;
        ((OD_FileAsyncCallParam*)context->data.file.call->req.param)->handle = OD_INVALID_FILE_HANDLE;
    }
    ret = _OD_TranslateFsError(SFS_RenameFile(filename, newname, context->data.file.call != NULL ?
                                                                    CONSTRUCT_CALLBACK_PACK(callback, _OD_FileAsyncCallback, context->data.file.call) :
                                                                    NULL));

	OD_TRACE_LOW("OD_FileRename: ret=%d, filename & newname:", ret);
	OD_TRACE_W_LOW(filename);
	OD_TRACE_W_LOW(newname);

	return ret;
}

OD_STATUS OD_FileSetAttributes(OD_Context *context, const OD_FileName *filename, unsigned int attr)
{
    unsigned short at;
    CHECKFILEHANDLE;

	OD_CHECK_ARGS_RETURN_STATUS(context);
	OD_CHECK_ARGS_RETURN_STATUS(filename);

    at = 0;
    if (attr & OD_FILEATTR_READ_ONLY)
    {
        at |= SFS_ATTR_RO;
    }
    if (attr & OD_FILEATTR_HIDDEN)
    {
        at |= SFS_ATTR_HIDDEN;
    }
    if (attr & OD_FILEATTR_SYSTEM)
    {
        at |= SFS_ATTR_SYS;
    }
    if (attr & OD_FILEATTR_VOLUME)
    {
        at |= SFS_ATTR_VOLUME;
    }
    if (attr & OD_FILEATTR_DIR)
    {
        /** we can't set this attribute in SPRD platform, so ignore it */
        /* at |= SFS_ATTR_DIR; */
    }
    if (attr & OD_FILEATTR_ARCHIVE)
    {
        at |= SFS_ATTR_ARCH;
    }
    return _OD_TranslateFsError(SFS_SetAttr(filename, at));
}

OD_STATUS OD_FileSetPosition(OD_Context *context, OD_FileHandle hFile, OD_INT64 offset, OD_FileSeek whence)
{
    int res = SFS_NO_ERROR;

	OD_CHECK_ARGS_RETURN_STATUS(context);
	OD_CHECK_FILE_HANDLE_RETURN_STATUS(hFile);

    switch (whence)
    {
    case OD_SEEK_FROM_START:
        res = SFS_SetFilePointer(hFile, offset, SFS_SEEK_BEGIN);
        break;
    case OD_SEEK_FROM_CURRENT:
        res = SFS_SetFilePointer(hFile, offset, SFS_SEEK_CUR);
        break;
    case OD_SEEK_FROM_END:
        res = SFS_SetFilePointer(hFile, offset, SFS_SEEK_END);
        break;
    default:
        res = SFS_ERROR_INVALID_PARAM;
        break;
    }

    return _OD_TranslateFsError(res);
}

OD_STATUS OD_FileTruncate(OD_Context *context, OD_FileHandle hFile)
{
    OD_INT64 pos;
    OD_STATUS ret;

	OD_CHECK_ARGS_RETURN_STATUS(context);
	OD_CHECK_FILE_HANDLE_RETURN_STATUS(hFile);

    ret = OD_FileGetPosition(context, hFile, &pos);
    if (ret != OD_SUCCESS)
    {
        return ret;
    }

    return _OD_TranslateFsError(SFS_SetFileSize(hFile, (uint32)pos));
}

OD_STATUS OD_FileWrite(OD_Context *context, OD_FileHandle hFile, void* buffer, unsigned int length, unsigned int* written)
{
    SFS_OVERLAPPED_T callback = {0};

	OD_CHECK_ARGS_RETURN_STATUS(context);
	OD_CHECK_ARGS_RETURN_STATUS(buffer);
	OD_CHECK_ARGS_RETURN_STATUS(written);
	OD_CHECK_FILE_HANDLE_RETURN_STATUS(hFile);

    if (context->data.file.call != NULL)
    {
        OD_ASSERT(context->data.file.call->req.param != NULL);
        ((OD_FileAsyncCallParam*)context->data.file.call->req.param)->type = OD_FILE_ASYNC_CALL_OPEN;
        ((OD_FileAsyncCallParam*)context->data.file.call->req.param)->handle = hFile;
    }
    if(length > 0)
    {
        return _OD_TranslateFsError(SFS_WriteFile(hFile, buffer, length, written, context->data.file.call != NULL ?
                                                                                    CONSTRUCT_CALLBACK_PACK(callback, _OD_FileAsyncCallback, context->data.file.call) :
                                                                                    NULL));
    }
    else
    {
        if (written) *written = 0;
        return OD_INVALID_PARAM;
    }
}

OD_STATUS OD_FindFirst(OD_Context *context, OD_FileHandle *handle, const OD_FileName *pattern, OD_FileAttr attr, 
                       OD_FileAttr attrmask, OD_FileName *filename, unsigned int maxlength, unsigned int *size, OD_FileAttr *attributes)
{
    // FIXME: Too big stack size occupied!
    int result;
    OD_WCHAR dir[OD_FILE_NAME_MAX_LENGTH] = {0};
    OD_WCHAR *pat = NULL;
    unsigned short ctlStr[OD_FILE_CTL_STR_MAX_LENGTH] = L"/A:";
    SFS_FIND_DATA_T find_data = {0};
    SFS_HANDLE find_handle = OD_INVALID_FILE_HANDLE;
    OD_WCHAR *pos = NULL;
    int i = 0;

    struct {
        int attr;
        OD_WCHAR *str;
    } ctl_str_map[] = 
    {
        { OD_FILEATTR_READ_ONLY,    L"R" },
        { OD_FILEATTR_HIDDEN,       L"H" },
        { OD_FILEATTR_SYSTEM,       L"S" },
    //  { OD_FILEATTR_VOLUME,       L"" },
        { OD_FILEATTR_DIR,          L"NF" },
        { OD_FILEATTR_ARCHIVE,      L"A" }
    };

    CHECKFILEHANDLE;
	OD_CHECK_ARGS_RETURN_STATUS(context);
	OD_CHECK_ARGS_RETURN_STATUS(filename);
	OD_CHECK_ARGS_RETURN_STATUS(pattern);
	OD_CHECK_ARGS_RETURN_STATUS(handle);

	od_wcsncpy(dir, pattern, OD_FILE_NAME_MAX_LENGTH - 1);
	dir[OD_FILE_NAME_MAX_LENGTH - 1] = L'\0';
    pos = (OD_WCHAR*)od_wcsrchr(dir, OD_FILE_DIRECTORY_SEPARATOR);
	if (!pos)
	{
		OD_ASSERT(!"Shouldn't happen, the path is broken.");
		return OD_ERROR;
	}
    pat = pos + 1;
    *pos = L'\0';

	if(attr == 0)
	{
		attr = (OD_FileAttr)0xFF;
	}
    // Evaluate the control string
    if (attr == 0 && attrmask == 0)
    {
        ctlStr[0] = L'\0';
    }
    else
    {
        for (i = 0; i < sizeof(ctl_str_map)/sizeof(ctl_str_map[0]); ++i)
        {
            if (attr & ctl_str_map[i].attr)
            {
                od_wcsncat(ctlStr, L"+", sizeof(ctlStr)/sizeof(ctlStr[0]) - od_wcslen(ctlStr) - 1);
                od_wcsncat(ctlStr, ctl_str_map[i].str, sizeof(ctlStr)/sizeof(ctlStr[0]) - od_wcslen(ctlStr) - 1);
            }
        }
        for (i = 0; i < sizeof(ctl_str_map)/sizeof(ctl_str_map[0]); ++i)
        {
            if (attrmask & ctl_str_map[i].attr)
            {
                od_wcsncat(ctlStr, L"-", sizeof(ctlStr)/sizeof(ctlStr[0]) - od_wcslen(ctlStr) - 1);
                od_wcsncat(ctlStr, ctl_str_map[i].str, sizeof(ctlStr)/sizeof(ctlStr[0]) - od_wcslen(ctlStr) - 1);
            }
        }
    }
    
    result = SFS_FindFirstFileCtrl(dir, pat, ctlStr, &find_handle, &find_data, NULL);

    // Restore the path
    *pos = OD_FILE_DIRECTORY_SEPARATOR;

    if (result == SFS_ERROR_NONE && find_handle != 0)
    {
        *filename = L'\0';
        od_wcsncat((OD_WCHAR*)filename, find_data.name, maxlength/sizeof(filename[0]) - 1);

        *handle = find_handle;
        if (size != NULL)
        {
            *size = find_data.length;
        }
        if (attributes != NULL)
        {
            *attributes = 0;
            if (find_data.attr & SFS_ATTR_RO)
            {
                *attributes |= OD_FILEATTR_READ_ONLY;
            }
            if (find_data.attr & SFS_ATTR_HIDDEN)
            {
                *attributes |= OD_FILEATTR_HIDDEN;
            }
            if (find_data.attr & SFS_ATTR_SYS)
            {
                *attributes |= OD_FILEATTR_SYSTEM;
            }
            if (find_data.attr & SFS_ATTR_VOLUME)
            {
                *attributes |= OD_FILEATTR_VOLUME;
            }
            if (find_data.attr & SFS_ATTR_DIR)
            {
                *attributes |= OD_FILEATTR_DIR;
            }
            if (find_data.attr & SFS_ATTR_ARCH)
            {
                *attributes |= OD_FILEATTR_ARCHIVE;
            }
        }
        return OD_SUCCESS;
    }

    return OD_ERROR;
}

OD_STATUS OD_FindNext(OD_Context *context, OD_FileHandle hFind, OD_FileName *filename, unsigned int maxlength, unsigned int *size, OD_FileAttr *attributes)
{
    int res = SFS_NO_ERROR;
    SFS_FIND_DATA_T find_data = {0};

	OD_CHECK_ARGS_RETURN_STATUS(context);
	OD_CHECK_ARGS_RETURN_STATUS(filename);
	OD_CHECK_FILE_HANDLE_RETURN_STATUS(hFind);

    res = SFS_FindNextFile(hFind, &find_data);

    if (res == SFS_NO_ERROR)
    {
        *filename = L'\0';
        od_wcsncat((OD_WCHAR*)filename, find_data.name, maxlength/sizeof(filename[0]) - 1);
        if (size != NULL)
        {
            *size = find_data.length;
        }
        if (attributes != NULL)
        {
            unsigned short at = find_data.attr;
            *attributes = 0;
            if (at & SFS_ATTR_RO)
            {
                *attributes |= OD_FILEATTR_READ_ONLY;
            }
            if (at & SFS_ATTR_HIDDEN)
            {
                *attributes |= OD_FILEATTR_HIDDEN;
            }
            if (at & SFS_ATTR_SYS)
            {
                *attributes |= OD_FILEATTR_SYSTEM;
            }
            if (at & SFS_ATTR_VOLUME)
            {
                *attributes |= OD_FILEATTR_VOLUME;
            }
            if (at & SFS_ATTR_DIR)
            {
                *attributes |= OD_FILEATTR_DIR;
            }
            if (at & SFS_ATTR_ARCH)
            {
                *attributes |= OD_FILEATTR_ARCHIVE;
            }
        }
    }
    else if (res == SFS_ERROR_NOT_EXIST)
    {
        return OD_FILE_NO_MORE_FILES;
    }

    return _OD_TranslateFsError(res);
}

OD_STATUS OD_FindClose(OD_Context *context, OD_FileHandle hFind)
{
	OD_CHECK_ARGS_RETURN_STATUS(context);
	OD_CHECK_FILE_HANDLE_RETURN_STATUS(hFind);

    return _OD_TranslateFsError(SFS_FindClose(hFind));
}

OD_STATUS OD_DirectoryCreate(OD_Context *context, const OD_FileName *dir)
{
	OD_STATUS ret = OD_SUCCESS;
	OD_FileAttr attr = OD_FILEATTR_NONE;
	unsigned int len = od_wcslen(dir);
	CHECKFILEHANDLE;

	OD_CHECK_ARGS_RETURN_STATUS(context);
	OD_CHECK_ARGS_RETURN_STATUS(dir);

	OD_ASSERT(len > 0);
	if (len > 0 && dir[len - 1] == '\\')
	{
		OD_CLONE_PATH_REMOVE_SEPARATOR(buf, dir, len);
		ret = _OD_TranslateFsError(SFS_CreateDirectory(buf));
	}
	else
	{
		ret = _OD_TranslateFsError(SFS_CreateDirectory(dir));
	}

	ret = (OD_SUCCESS == OD_FileGetAttributes(context, dir, &attr) 
		&& ((attr & OD_FILEATTR_DIR) || (attr & OD_FILEATTR_VOLUME)))
		? OD_SUCCESS : ret;

	OD_TRACE_LOW("OD_DirectoryCreate: ret=%d, dir:", ret);
	OD_TRACE_W_LOW(dir);

	return ret;
}

OD_STATUS OD_DirectoryRemove(OD_Context *context, const OD_FileName *dir)
{
	OD_STATUS ret = OD_SUCCESS;
	unsigned int len = od_wcslen(dir);

    CHECKFILEHANDLE;
	OD_CHECK_ARGS_RETURN_STATUS(context);
	OD_CHECK_ARGS_RETURN_STATUS(dir);

	OD_ASSERT(len > 0);
	if (len > 0 && dir[len - 1] == '\\')
	{
		OD_CLONE_PATH_REMOVE_SEPARATOR(buf, dir, len);
		ret = _OD_TranslateFsError(SFS_DeleteDirectory(dir));
	}
	else
	{
		ret = _OD_TranslateFsError(SFS_DeleteDirectory(dir));
	}

	ret = OD_FALSE == OD_FileExist(context, dir) ? OD_SUCCESS : ret;

	OD_TRACE_LOW("OD_DirectoryRemove: ret=%d, dir:", ret);
	OD_TRACE_W_LOW(dir);
	return ret;
}

OD_STATUS OD_DirectoryGetSize(OD_Context *context, const OD_FileName *dir, unsigned int *size)
{
	OD_CHECK_ARGS_RETURN_STATUS(context);
	OD_CHECK_ARGS_RETURN_STATUS(dir);
	OD_CHECK_ARGS_RETURN_STATUS(size);

    // FIXME: no corresponding interface on SDK.
    return OD_NOT_SUPPORTED;
}

OD_STATUS OD_DirectoryGetPredefined(OD_Context *context, OD_PredefinedPathType pathtype, OD_FileName *path)
{
    const OD_WCHAR* const picture_path  = OD_FILE_PREDEFINED_PATH_PICTURE;
    const OD_WCHAR* const video_path    = OD_FILE_PREDEFINED_PATH_VIDEO;
    const OD_WCHAR* const sound_path    = OD_FILE_PREDEFINED_PATH_SOUND;
    const OD_WCHAR* const music_path    = OD_FILE_PREDEFINED_PATH_MUSIC;
    const OD_WCHAR* const document_path = OD_FILE_PREDEFINED_PATH_DOCUMENT;
    const OD_WCHAR* const other_path    = OD_FILE_PREDEFINED_PATH_OTHER;
    int i = 0;

	OD_CHECK_ARGS_RETURN_STATUS(context);
	OD_CHECK_ARGS_RETURN_STATUS(path);

    switch (pathtype)
    {
    case OD_PREDEFINEDPATH_TYPE_PICTURE:
        if (picture_path[0] == 0)
        {
            path[0] = 0;
        }
        else
        {
            while(picture_path[i] != 0)
            {
                path[i] = picture_path[i];
                i++;
            }
            path[i] = 0;
            path[0] = OD_GetStorageDriveLetter(context);
        }
        break;
    case OD_PREDEFINEDPATH_TYPE_VIDEO:
        if (video_path[0] == 0)
        {
            path[0] = 0;
        }
        else
        {
            while(video_path[i] != 0)
            {
                path[i] = video_path[i];
                i++;
            }
            path[i] = 0;
            path[0] = OD_GetStorageDriveLetter(context);
        }
        break;
    case OD_PREDEFINEDPATH_TYPE_SOUND:
        if (sound_path[0] == 0)
        {
            path[0]= 0;
        }
        else
        {
            while(sound_path[i] != 0)
            {
                path[i]=sound_path[i];
                i++;
            }
            path[i] = 0;
            path[0] = OD_GetStorageDriveLetter(context);
        }
        break;		
    case OD_PREDEFINEDPATH_TYPE_MUSIC:
        if (music_path[0] == 0)
        {
            path[0] = 0;
        }
        else
        {
            while(music_path[i] != 0)
            {
                path[i] = music_path[i];
                i++;
            }
            path[i] = 0;
            path[0] = OD_GetStorageDriveLetter(context);
        }
        break;
    case OD_PREDEFINEDPATH_TYPE_DOCUMENT:
        if (document_path[0] == 0)
        {
            path[0] = 0;
        }
        else
        {
            while(document_path[i] != 0)
            {
                path[i]=document_path[i];
                i++;
            }
            path[i] = 0;
            path[0] = OD_GetStorageDriveLetter(context);
        }
        break;
    case OD_PREDEFINEDPATH_TYPE_OTHER:
    default:
        if (other_path[0] == 0)
        {
            path[0] = 0;
        }
        else
        {
            while(other_path[i] != 0)
            {
                path[i]=other_path[i];
                i++;
            }
            path[i] = 0;
            path[0] = OD_GetStorageDriveLetter(context);
        }
        break;
    }
    return OD_SUCCESS;
}

#define OD_FILE_DISK_C_NAME             (L"C")
#define OD_FILE_DISK_D_NAME             (L"D")
#define OD_FILE_DISK_E_NAME             (L"E")

#define OD_FILE_DISK_C_LETTER           MMIFILE_C_LETTER
#define OD_FILE_DISK_D_LETTER           MMIFILE_D_LETTER
#define OD_FILE_DISK_E_LETTER           MMIFILE_E_LETTER

static OD_BOOL _OD_IsDriveAvailable(OD_Context *context, const OD_WCHAR *driveName)
{
    return SFS_ERROR_NONE == SFS_GetDeviceStatus(driveName);
}

int OD_GetPublicDriveLetter(OD_Context *context)
{
	OD_CHECK_ARGS(context, -1);

    if (_OD_IsDriveAvailable(context, OD_FILE_DISK_E_NAME))
    {
        return OD_FILE_DISK_E_LETTER;
    }
    else if (_OD_IsDriveAvailable(context, OD_FILE_DISK_D_NAME))
    {
        return OD_FILE_DISK_D_LETTER;
    }
    return -1;
}

int OD_GetSystemDriveLetter(OD_Context *context)
{
    if (_OD_IsDriveAvailable(context, OD_FILE_DISK_C_NAME))
    {
        return OD_FILE_DISK_C_LETTER;
    }
    else if (_OD_IsDriveAvailable(context, OD_FILE_DISK_D_NAME))
    {
        return OD_FILE_DISK_D_LETTER;
    }
    return -1;
}

int OD_GetStorageDriveLetter(OD_Context *context)
{
	OD_CHECK_ARGS(context, -1);

    if (_OD_IsDriveAvailable(context, OD_FILE_DISK_E_NAME))
    {
        return OD_FILE_DISK_E_LETTER;
    }
    else if (_OD_IsDriveAvailable(context, OD_FILE_DISK_D_NAME))
    {
        return OD_FILE_DISK_D_LETTER;
    }
    return -1;
}

int OD_GetCardDriveLetter(OD_Context *context)
{
	OD_CHECK_ARGS(context, -1);

    if (_OD_IsDriveAvailable(context, OD_FILE_DISK_E_NAME))
    {
        return OD_FILE_DISK_E_LETTER;
    }
    return -1;
}

OD_STATUS OD_DriveGetAllVisibleLetters(OD_Context *context, char driveletters[], OD_FileName *drivenames[], int* num)
{
    int i = 0, k = 0;
	/* We don't want end users to get access to the system drive (C:) */
    char drives[] = { /*OD_FILE_DISK_C_LETTER,*/ OD_FILE_DISK_D_LETTER, OD_FILE_DISK_E_LETTER };
    const OD_FileName *name[] = { /*OD_FILE_DISK_C_NAME,*/ OD_FILE_DISK_D_NAME, OD_FILE_DISK_E_NAME };

	OD_CHECK_ARGS_RETURN_STATUS(context);
	OD_CHECK_ARGS_RETURN_STATUS(num);

    k = 0;
    for (i = 0; i < sizeof(drives) / sizeof(drives[0]); ++i)
    {
        if (_OD_IsDriveAvailable(context, name[i]))
        {
            driveletters[k] = drives[i];

            if (drivenames)
            {
                drivenames[k] = (OD_FileName *)name[i];
            }
            ++k;
        }
    }
    *num = k;
    return OD_SUCCESS;
}

int OD_DriveGetCurrentLetter(OD_Context *context)
{
	OD_CHECK_ARGS(context, -1);

    // TODO, no interface on SPRD.
    return OD_ERROR;
}

// TODO: What if there's two or more SD cards?
OD_STATUS OD_DriveGetSystemLetters(OD_Context *context, char driveletters[], unsigned int* num)
{
	OD_CHECK_ARGS_RETURN_STATUS(context);
	OD_CHECK_ARGS_RETURN_STATUS(num);

    // Assume there is only one system drive.
    *num = 1;
    if (_OD_IsDriveAvailable(context, OD_FILE_DISK_C_NAME))
    {
        driveletters[0] = OD_FILE_DISK_C_LETTER;
        return OD_SUCCESS;
    }
    else if (_OD_IsDriveAvailable(context, OD_FILE_DISK_D_NAME))
    {
        driveletters[0] = OD_FILE_DISK_D_LETTER;
        return OD_SUCCESS;
    }
    return OD_ERROR;
}

OD_STATUS OD_GetPrestorageFolder(OD_Context *context, OD_FileName *buf, unsigned int bufLen)
{
#ifdef OD_FILE_CUSTOM_PRESTORAGE_FOLDER
	OD_FileName folder[] = OD_FILE_CUSTOM_PRESTORAGE_FOLDER;
	unsigned int len = od_wcslen(folder);

	OD_CHECK_ARGS_RETURN_STATUS(context);
	OD_CHECK_ARGS_RETURN_STATUS(buf);

	if (len == 0 || (unsigned int)bufLen <= len || folder[len - 1] != L'\\' || !OD_FileExist(context, folder))
	{
		return OD_ERROR;
	}
	od_wcsncpy(buf, folder, bufLen);
	buf[bufLen - 1] = 0;
	return OD_SUCCESS;
#else
	return OD_ERROR;
#endif /* OD_FILE_CUSTOM_PRESTORAGE_FOLDER */
}

OD_STATUS OD_GetStorageFolder(OD_Context *context, OD_FileName *buf, unsigned int bufLen)
{
#ifdef OD_FILE_CUSTOM_STORAGE_FOLDER
	OD_FileName folder[] = OD_FILE_CUSTOM_STORAGE_FOLDER;
	OD_FileName volume[2] = {0};
	unsigned int len = od_wcslen(folder);

	OD_CHECK_ARGS_RETURN_STATUS(context);
	OD_CHECK_ARGS_RETURN_STATUS(buf);

	if (len == 0 || (unsigned int)bufLen <= len || folder[len - 1] != L'\\')
	{
		return OD_ERROR;
	}
	OD_ASSERT(len > 0);
	volume[0] = folder[0];
	if (!_OD_IsDriveAvailable(context, volume))
	{
		// Try to resign the drive letter to public drive
		folder[0] = OD_GetPublicDriveLetter(context);
		volume[0] = folder[0];
		if (!_OD_IsDriveAvailable(context, volume))
		{
			return OD_ERROR;
		}
	}
	od_wcsncpy(buf, folder, bufLen);
	buf[bufLen - 1] = 0;
	return OD_SUCCESS;
#else
	return OD_ERROR;
#endif /* OD_FILE_CUSTOM_STORAGE_FOLDER */
}

OD_BOOL OD_DriveIsWritable(OD_Context *context, int drive)
{
	OD_CHECK_ARGS(context, OD_FALSE);
    // Return true for now.
    return OD_TRUE;
}

OD_STATUS OD_GetDriveFreeSpace(OD_WCHAR * driveName, OD_INT64 * freeSpace)
{
    uint32 freeSpaceHigh=0, freeSpaceLow=0;
    OD_INT64 tmp=0;

	OD_CHECK_ARGS_RETURN_STATUS(driveName);
	OD_CHECK_ARGS_RETURN_STATUS(freeSpace);

    if(!MMIAPIFMM_GetDeviceFreeSpace((const OD_WCHAR *)driveName, od_wcslen(driveName), &freeSpaceHigh, &freeSpaceLow))
        return OD_ERROR;

    tmp=freeSpaceHigh;
    tmp<<=32;
    tmp+=freeSpaceLow;
    *freeSpace=tmp;

    return OD_SUCCESS;
}

OD_STATUS static _OD_UnifyPath(OD_FileName *filename)
{
    int i = 0;
    OD_WCHAR *name = NULL;
    OD_WCHAR *p = NULL;
    OD_WCHAR *q = NULL;
    int len = od_wcslen(filename);
    
    for (i = 0; i < len; ++i)
    {
        if (filename[i] == L'/')
        {
            filename[i] = L'\\';
        }
    }

    /* Removing the last '\' */
    if(filename[ len -1 ] == '\\')
    {
        filename[ len - 1 ] = 0;
        --len;
    }
    
    /* Remove relative path "." and ".."
       Only searching for '\.\' and '\..\' don't cover the situation when it end with 
       '\.' and '\..'.
    */
    name = filename;
    while ((p = od_wcsstr(name, L"\\.\\"))
        || ( od_wcsendwith(name, L"\\.") && (p = name + od_wcslen(name) -2)))
    {
        name = p;
        q = name + od_wcslen(L"\\.");
        while (*q)
        {
            *p++ = *q++;
        }
        *p = L'\0';
    }

    name = filename;
    while ((p = od_wcsstr(name, L"\\..\\"))
        || ( od_wcsendwith(name, L"\\..") && (p = name + od_wcslen(name) -3)))
    {
        OD_WCHAR *pos;
        pos = name = p;
        *pos = L'\0';
        p = (OD_WCHAR *)od_wcsrchr(filename, L'\\');
        if (p == NULL)
        {
            return OD_ERROR;
        }
        *pos = L'\\';
        q = name + od_wcslen(L"\\..");
        while (*q)
        {
            *p++ = *q++;
        }
        *p = L'\0';
    }

    return OD_SUCCESS;
}

OD_STATUS OD_PathsEqual(OD_Context *context, const OD_FileName *path1, const OD_FileName *path2, OD_BOOL* equal)
{
    /* Just compare the unified string since 
       1. there's no link on platform.
       2. no API to get the normalized absolute path
    */
    OD_WCHAR filename1[OD_FILE_NAME_MAX_LENGTH] = {0};
    OD_WCHAR filename2[OD_FILE_NAME_MAX_LENGTH] = {0};
    int res = -1;

	OD_CHECK_ARGS_RETURN_STATUS(context);
	OD_CHECK_ARGS_RETURN_STATUS(path1);
	OD_CHECK_ARGS_RETURN_STATUS(path2);
	OD_CHECK_ARGS_RETURN_STATUS(equal);

    od_wcsncat(filename1, path1, OD_FILE_NAME_MAX_LENGTH - 1);
    od_wcsncat(filename2, path2, OD_FILE_NAME_MAX_LENGTH - 1);
    if (_OD_UnifyPath(filename1) != OD_SUCCESS || _OD_UnifyPath(filename2) != OD_SUCCESS)
    {
        return OD_ERROR;
    }
    res = od_wcsicmp(filename1, filename2);

    if (res == 0)
    {
        *equal = OD_TRUE;
    }
    else
    {
        *equal = OD_FALSE;
    }

    return OD_SUCCESS;
}

OD_STATUS OD_XDelete(OD_Context *context, const OD_FileName *path)
{
#ifndef PLATFORM_SC6600L
	OD_WCHAR dir[OD_FILE_NAME_MAX_LENGTH] = {0};
	OD_WCHAR *fileNameOnly = NULL;
	OD_WCHAR *pos = NULL;
	unsigned int len = 0;

	OD_CHECK_ARGS_RETURN_STATUS(context);
	OD_CHECK_ARGS_RETURN_STATUS(path);

	od_wcsncpy(dir, path, OD_FILE_NAME_MAX_LENGTH - 1);
	dir[OD_FILE_NAME_MAX_LENGTH - 1] = L'\0';
	len = od_wcslen(dir);
	if (len >= 1 && dir[len - 1] == OD_FILE_DIRECTORY_SEPARATOR)
	{
		dir[len - 1] = L'\0';
	}
	pos = (OD_WCHAR*)od_wcsrchr(dir, OD_FILE_DIRECTORY_SEPARATOR);
	if (!pos)
	{
#ifdef WIN32
		OD_ASSERT(!"This code assumes there's always a path separator in the file name.");
#endif /* WIN32 */
		return OD_FILE_NOT_FOUND;
	}

	fileNameOnly = pos + 1;
	*pos = L'\0';

	return _OD_TranslateFsError(MMIAPIFMM_DeleteTreeEx(dir, fileNameOnly, SCI_TRUE));
#else
	// FIXME: no API on 6600.
	return OD_ERROR;
#endif /* PLATFORM_SC6600L */
}

static void OD_FileTest(OD_Context *ctx)
{
#define CloseIf(handle)  do{ if(handle != OD_INVALID_FILE_HANDLE) OD_FileClose(ctx, handle);}while(0)

    OD_FileHandle handle = OD_INVALID_FILE_HANDLE;

    //Test case sensitive, should be non-sensitive
    OD_STATUS res = OD_FileOpen(ctx, L"E:\\@operamobile\\en.lng", OD_FILE_READ, &handle);
    OD_ASSERT(res == OD_SUCCESS && handle != OD_INVALID_FILE_HANDLE);
    CloseIf(handle);

    /* Following test will fail...
    //Test if support ".."
    handle = OD_FileOpen(ctx, L"E:\\@OperaMobile\\..\\@OperaMobile\\en.lng", OD_FILE_READ);
    OD_ASSERT(handle != OD_INVALID_FILE_HANDLE);
    CloseIf(handle);

    //Test if support "."
    handle = OD_FileOpen(ctx, L"E:\\@OperaMobile\\.\\en.lng", OD_FILE_READ);
    OD_ASSERT(handle != OD_INVALID_FILE_HANDLE);
    CloseIf(handle);

    //Test if support repeated "\\"
    handle = OD_FileOpen(ctx, L"E:\\\\@OperaMobile\\en.lng", OD_FILE_READ);
    OD_ASSERT(handle != OD_INVALID_FILE_HANDLE);
    CloseIf(handle);
    */
}
