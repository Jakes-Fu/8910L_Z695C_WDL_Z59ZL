/** Generated file, DO NOT edit it manually */
#ifndef OPDEVSIM_H
#define OPDEVSIM_H

#ifdef __cplusplus
extern "C" {
#endif

typedef OD_MMI_TASK OD_BOOL (*OD_BookmarkImportIsAvailable_func_t)(OD_Context *context);
typedef OD_MMI_TASK void (*OD_BookmarkImportEnumBegin_func_t)(OD_Context *context);
typedef OD_MMI_TASK void (*OD_BookmarkImportEnumEnd_func_t)(OD_Context *context);
typedef OD_MMI_TASK OD_STATUS (*OD_BookmarkImportGetNextDataSize_func_t)(OD_Context *context, OD_BookmarkImportEntryDataSize *entry_size);
typedef OD_MMI_TASK OD_STATUS (*OD_BookmarkImportGetNext_func_t)(OD_Context *context, OD_BookmarkImportEntry *entry);
typedef OD_MMI_TASK void (*OD_InitDebugger_func_t)(void);
typedef OD_MMI_TASK void (*OD_DeinitDebugger_func_t)(void);
typedef OD_ALL_TASK OD_STATUS (*OD_WriteLogFile_func_t)(OD_Context* context, const char *format, ...);
typedef OD_ALL_TASK OD_STATUS (*OD_Trace_func_t)(OD_TRACE_LEVEL level, const char *format, ...);
typedef OD_ALL_TASK OD_STATUS (*OD_TraceWString_func_t)(OD_TRACE_LEVEL level, const OD_WCHAR *wstr);
typedef OD_ALL_TASK OD_STATUS (*OD_PrintStackTrace_func_t)(OD_BOOL print_locals);
typedef OD_ALL_TASK OD_STATUS (*OD_SetDataBreakPoint_func_t)(unsigned int start_addr,unsigned int end_addr, unsigned int data,OD_DataBreakpointAction mode,OD_DataBreakPointRaiseCallback callback);
typedef OD_ALL_TASK void (*OD_Assert_func_t)(const char *exp, const char *file, int line);
typedef OD_ALL_TASK void (*OD_DASRequestSavePath_func_t)(const char* url, const char* mimetype, const char* filename, unsigned int filelength, int session_id,const char *header, unsigned int header_len, const char *data, unsigned int data_len);
typedef OD_ALL_TASK void (*OD_DASReportProgress_func_t)(int session_id, unsigned int progress, int seq_num, OD_DownloadStatus status, OD_DownloadCause cause);
typedef OD_ALL_TASK void (*OD_DASResponseCancelPrefetch_func_t)(int session_id);
typedef OD_ALL_TASK void (*OD_DASAbortPrefetch_func_t)(int session_id, OD_DownloadCause cause);
typedef OD_ALL_TASK void (*OD_DASStopResponse_func_t)(int session_id, unsigned int progress, int seq_num, OD_DownloadCause cause);
typedef OD_ALL_TASK void (*OD_DASShowDownloadList_func_t)(void);
typedef OD_ALL_TASK void (*OD_DASStartBrowser_func_t)(void);
typedef OD_ALL_TASK void (*OD_DASExitBrowser_func_t)(void);
typedef OD_ALL_TASK void (*OD_DASSendAuthenticationData_func_t)(OD_AuthenticationMethod auth_type,OD_AuthenticationMechanism auth_option,unsigned char* proxy_ip, unsigned short port, const char* url,const char* realm, const char* username, const char* password,const char* nonce, const char* opaque, const char* algorithm,const char* qop);
typedef OD_MMI_TASK OD_BOOL (*OD_DownloadIsMimeTypeSupport_func_t)(const char* type);
typedef OD_MMI_TASK OD_BOOL (*OD_DownloadIsDDMimeType_func_t)(const char* type);
typedef OD_MMI_TASK OD_STATUS (*OD_DownloadHandleMIMEDownloaded_func_t)(OD_Context *context, const char* url, const char* mimetype, const OD_FileName *filename);
typedef OD_ALL_TASK OD_STATUS (*OD_FileInit_func_t)(OD_Context *context);
typedef OD_ALL_TASK OD_STATUS (*OD_FileDeinit_func_t)(OD_Context *context);
typedef OD_ALL_TASK int (*OD_CountFileHandles_func_t)(OD_Context *context, OD_FileHandleCountType type);
typedef OD_ALL_TASK OD_STATUS (*OD_FileOpen_func_t)(OD_Context *context, const OD_FileName *filename, OD_FileOpenMode flags, OD_FileHandle *handle);
typedef OD_ALL_TASK OD_STATUS (*OD_FileClose_func_t)(OD_Context *context, OD_FileHandle handle);
typedef OD_ALL_TASK OD_STATUS (*OD_FileCommit_func_t)(OD_Context *context, OD_FileHandle handle);
typedef OD_ALL_TASK OD_STATUS (*OD_FileDelete_func_t)(OD_Context *context, const OD_FileName *filename);
typedef OD_ALL_TASK OD_STATUS (*OD_FileExtend_func_t)(OD_Context *context, OD_FileHandle handle, unsigned int length);
typedef OD_ALL_TASK OD_BOOL (*OD_FileExist_func_t)(OD_Context *context, const OD_FileName *filename);
typedef OD_ALL_TASK OD_STATUS (*OD_FileGetAttributes_func_t)(OD_Context *context, const OD_FileName *filename, OD_FileAttr *attr);
typedef OD_ALL_TASK OD_STATUS (*OD_FileGetLength_func_t)(OD_Context *context, OD_FileHandle hFile, unsigned int* size);
typedef OD_ALL_TASK OD_STATUS (*OD_FileGetPosition_func_t)(OD_Context *context, OD_FileHandle hFile, OD_INT64* pos);
typedef OD_ALL_TASK OD_STATUS (*OD_FileGetTimestampsFromHandle_func_t)(OD_Context *context, OD_FileHandle hFile, OD_PackedTimestamp *creation_timestamp, OD_PackedTimestamp *last_modification_timestamp);
typedef OD_ALL_TASK OD_STATUS (*OD_FileGetTimestampsFromAttributes_func_t)(OD_Context *context, const OD_FileName *filename, OD_PackedTimestamp *creation_timestamp, OD_PackedTimestamp *last_modification_timestamp);
typedef OD_ALL_TASK OD_BOOL (*OD_FileIsForwardable_func_t)(OD_Context *context, const OD_FileName *filename);
typedef OD_ALL_TASK OD_STATUS (*OD_FileRead_func_t)(OD_Context *context, OD_FileHandle handle, void* buffer, unsigned int length, unsigned int* read);
typedef OD_ALL_TASK OD_STATUS (*OD_FileRename_func_t)(OD_Context *context, const OD_FileName *filename, const OD_FileName *newname);
typedef OD_ALL_TASK OD_STATUS (*OD_FileSetAttributes_func_t)(OD_Context *context, const OD_FileName *filename, unsigned int attr);
typedef OD_ALL_TASK OD_STATUS (*OD_FileSetPosition_func_t)(OD_Context *context, OD_FileHandle handle, OD_INT64 offset, OD_FileSeek whence);
typedef OD_ALL_TASK OD_STATUS (*OD_FileTruncate_func_t)(OD_Context *context, OD_FileHandle handle);
typedef OD_ALL_TASK OD_STATUS (*OD_FileWrite_func_t)(OD_Context *context, OD_FileHandle handle, void *buffer, unsigned int length, unsigned int *written);
typedef OD_ALL_TASK OD_STATUS (*OD_FindFirst_func_t)(OD_Context *context, OD_FileFindHandle *handle, const OD_FileName *pattern, OD_FileAttr attr, OD_FileAttr attrmask, OD_FileName *filename, unsigned int maxlength, unsigned int *size, OD_FileAttr *attributes);
typedef OD_ALL_TASK OD_STATUS (*OD_FindNext_func_t)(OD_Context *context, OD_FileFindHandle handle, OD_FileName *filename, unsigned int maxlength, unsigned int *size, OD_FileAttr *attributes);
typedef OD_ALL_TASK OD_STATUS (*OD_FindClose_func_t)(OD_Context *context, OD_FileFindHandle handle);
typedef OD_ALL_TASK OD_STATUS (*OD_DirectoryCreate_func_t)(OD_Context *context, const OD_FileName *dir);
typedef OD_ALL_TASK OD_STATUS (*OD_DirectoryRemove_func_t)(OD_Context *context, const OD_FileName *dir);
typedef OD_ALL_TASK OD_STATUS (*OD_DirectoryGetSize_func_t)(OD_Context *context, const OD_FileName *dir, unsigned int *size);
typedef OD_ALL_TASK OD_STATUS (*OD_DirectoryGetPredefined_func_t)(OD_Context *context, OD_PredefinedPathType pathtype, OD_FileName *path);
typedef OD_ALL_TASK int (*OD_GetCardDriveLetter_func_t)(OD_Context *context);
typedef OD_ALL_TASK int (*OD_GetPublicDriveLetter_func_t)(OD_Context *context);
typedef OD_ALL_TASK int (*OD_GetSystemDriveLetter_func_t)(OD_Context *context);
typedef OD_ALL_TASK int (*OD_GetStorageDriveLetter_func_t)(OD_Context *context);
typedef OD_ALL_TASK OD_STATUS (*OD_DriveGetAllVisibleLetters_func_t)(OD_Context *context, char driveletters[], OD_FileName *drivenames[], int* num);
typedef OD_ALL_TASK int (*OD_DriveGetCurrentLetter_func_t)(OD_Context *context);
typedef OD_ALL_TASK OD_STATUS (*OD_DriveGetSystemLetters_func_t)(OD_Context *context, char driveletters[], unsigned int* num);
typedef OD_ALL_TASK OD_STATUS (*OD_GetPrestorageFolder_func_t)(OD_Context *context, OD_FileName *buf, unsigned int bufLen);
typedef OD_ALL_TASK OD_STATUS (*OD_GetStorageFolder_func_t)(OD_Context *context, OD_FileName *buf, unsigned int bufLen);
typedef OD_ALL_TASK OD_BOOL (*OD_DriveIsWritable_func_t)(OD_Context *context, int drive);
typedef OD_ALL_TASK OD_STATUS (*OD_GetDriveFreeSpace_func_t)(OD_WCHAR * driveName, OD_INT64 * freeSpace);
typedef OD_ALL_TASK OD_STATUS (*OD_PathsEqual_func_t)(OD_Context *context, const OD_FileName *path1, const OD_FileName *path2, OD_BOOL *equal);
typedef OD_ALL_TASK OD_STATUS (*OD_XDelete_func_t)(OD_Context *context, const OD_FileName *path);
typedef OD_ALL_TASK OD_STATUS (*OD_FontInit_func_t)(OD_Context *context);
typedef OD_ALL_TASK OD_STATUS (*OD_FontDeinit_func_t)(OD_Context *context);
typedef OD_ALL_TASK OD_STATUS (*OD_GetFontMetrics_func_t)(OD_Context *context, const OD_FontAttr *font_attr, OD_FontMetrics *font_metrics);
typedef OD_ALL_TASK OD_STATUS (*OD_GetCharMetrics_func_t)(OD_Context *context, const OD_FontAttr *font_attr, OD_WCHAR c, OD_GlyphMetrics *char_metrics);
typedef OD_ALL_TASK OD_STATUS (*OD_GetTextWidth_func_t)(OD_Context *context, const OD_FontAttr *font_attr, const OD_WCHAR *text, int text_len, int *text_width);
typedef OD_ALL_TASK OD_STATUS (*OD_MeasureString_func_t)(OD_Context *context, const OD_FontAttr *font_attr, const OD_WCHAR *text, int text_len, int max_extent, int *charfit_num, int *fit_width);
typedef OD_ALL_TASK OD_STATUS (*OD_DrawText_func_t)(OD_Context *context, const OD_FontAttr *font_attr, const OD_WCHAR *text, int text_len, const OD_DrawTextParams *params);
typedef OD_ALL_TASK OD_STATUS (*OD_GetTextData_func_t)(OD_Context *context, const OD_FontAttr *font_attr, const OD_WCHAR *text, int text_len, const OD_DrawTextParams *params);
typedef OD_MMI_TASK OD_STATUS (*OD_GDIInit_func_t)(OD_Context *context);
typedef OD_MMI_TASK OD_STATUS (*OD_GDIDeinit_func_t)(OD_Context *context);
typedef OD_MMI_TASK OD_BOOL (*OD_GDIUseNativeMemory_func_t)(OD_Context *context);
typedef OD_MMI_TASK OD_STATUS (*OD_GDIAllocMemory_func_t)(OD_Context *context, int size, char** pBuffer);
typedef OD_MMI_TASK void (*OD_GDIFreeMemory_func_t)(OD_Context *context, char* pBuffer);
typedef OD_MMI_TASK OD_STATUS (*OD_GDICreateLayer_func_t)(OD_Context *context, OD_PixelFormat format,int width, int height,char *buffer, OD_GDIHandle *handle);
typedef OD_MMI_TASK OD_STATUS (*OD_GDIDestroyLayer_func_t)(OD_Context *context, OD_GDIHandle handle);
typedef OD_MMI_TASK OD_STATUS (*OD_ShowOpera_func_t)(OD_Context *context);
typedef OD_MMI_TASK OD_STATUS (*OD_HideOpera_func_t)(OD_Context *context);
typedef OD_MMI_TASK OD_STATUS (*OD_Paint_func_t)(OD_Context *context, int x, int y, int w, int h);
typedef OD_MMI_TASK OD_STATUS (*OD_GetScreenBuffer_func_t)(OD_Context *context, char** buffer);
typedef OD_MMI_TASK OD_STATUS (*OD_GetViewportArea_func_t)(OD_Context *context, int *x, int *y, int *w, int *h);
typedef OD_MMI_TASK OD_STATUS (*OD_GetLayerDimension_func_t)(OD_LayerType type, int *width, int *height);
typedef OD_MMI_TASK OD_PixelFormat (*OD_GetLayerPixelFormat_func_t)(OD_LayerType type);
typedef OD_MMI_TASK int (*OD_GetLayerBPP_func_t)(OD_PixelFormat format);
typedef OD_MMI_TASK int (*OD_GetScreenDPI_func_t)(OD_Context *context);
typedef OD_MMI_TASK OD_RotationAngle (*OD_GetRotationAngle_func_t)(OD_Context *context);
typedef OD_MMI_TASK OD_STATUS (*OD_RotateScreen_func_t)(OD_Context *context, OD_RotationAngle angle);
typedef OD_MMI_TASK OD_ScreenMode (*OD_GetScreenMode_func_t)(OD_Context *context);
typedef OD_MMI_TASK OD_STATUS (*OD_ToggleFullScreen_func_t)(OD_Context *context, OD_BOOL bFullScreen);
typedef OD_MMI_TASK OD_STATUS (*OD_EnterFullScreenEditor_func_t)(OD_Context *context);
typedef OD_MMI_TASK OD_STATUS (*OD_CloseFullScreenEditor_func_t)(OD_Context *context);
typedef OD_MMI_TASK void (*OD_ImeEnterScreen_func_t)(OD_Context* context);
typedef OD_MMI_TASK void (*OD_ImeExitScreen_func_t)(OD_Context* context);
typedef OD_MMI_TASK void (*OD_ImeConnect_func_t)(OD_Context* context, OD_InputType input_type, const OD_WCHAR* lsk_label, const OD_WCHAR* rsk_label);
typedef OD_MMI_TASK void (*OD_ImeDisconnect_func_t)(OD_Context* context);
typedef OD_MMI_TASK void (*OD_ImeBeforeEditorOrCaretChanged_func_t)(void);
typedef OD_MMI_TASK int (*OD_ImeGetImuiHeight_func_t)(void);
typedef OD_MMI_TASK void (*OD_DrawImePart_func_t)(int x,int y,int w,int h);
typedef OD_MMI_TASK void (*OD_ImeSetSoftKeyLabel_func_t)(OD_KEY type, const unsigned short* label);
typedef OD_ALL_TASK OD_KEY (*OD_TranslateKeyCode_func_t)(int platform_key);
typedef OD_ALL_TASK OD_PENEVENT (*OD_TranslatePenEvent_func_t)(int platform_pen_evt);
typedef OD_MMI_TASK OD_STATUS (*KeyPenTracker_init_func_t)(OD_Context* context, KeyPenTracker* tracker, KeyPenTrackerType type, KeyPenTrackerCallback callback, OD_BOOL callbackReenterable);
typedef OD_MMI_TASK void (*KeyPenTracker_destroy_func_t)(OD_Context* context, KeyPenTracker* tracker);
typedef OD_MMI_TASK void (*KeyPenTracker_resetStatus_func_t)(OD_Context* context, KeyPenTracker* tracker);
typedef OD_MMI_TASK OD_STATUS (*KeyPenTracker_handleEvent_func_t)(OD_Context* context, KeyPenTracker* tracker, KeyPenTrackerEvent* event);
typedef OD_MMI_TASK void (*OD_ExitUI_func_t)(OD_Context *context);
typedef OD_MMI_TASK OD_STATUS (*OD_InitApp_func_t)(OD_Context *context);
typedef OD_MMI_TASK OD_STATUS (*OD_DeinitApp_func_t)(OD_Context *context);
typedef OD_ALL_TASK OD_STATUS (*OD_MemoryCreatePool_func_t)(OD_Context *context, char** mempool, int* pool_size, int default_size, int min_size);
typedef OD_ALL_TASK OD_STATUS (*OD_MemoryDestroyPool_func_t)(OD_Context *context);
typedef OD_ALL_TASK int (*OD_MemoryGetPoolSize_func_t)(OD_Context *context);
typedef OD_ALL_TASK void* (*OD_MemoryGetPoolBase_func_t)(OD_Context *context);
typedef OD_MMI_TASK OD_STATUS (*OD_MemoryBlocksInit_func_t)(OD_Context* context, int* pool_size, int default_size, int min_size);
typedef OD_MMI_TASK OD_STATUS (*OD_MemoryBlocksDeinit_func_t)(OD_Context* context);
typedef OD_MMI_TASK void* (*OD_Malloc_func_t)(OD_Context* context, int size);
typedef OD_MMI_TASK void (*OD_Free_func_t)(OD_Context* context, void* ptr);
typedef OD_ALL_TASK OD_NetID (*OD_NetGetID_func_t)(const OD_Context *context);
typedef OD_ALL_TASK OD_STATUS (*OD_NetGetProxy_func_t)(const OD_Context *context, OD_BOOL *use_proxy, OD_Proxy *proxy);
typedef OD_MMI_TASK OD_STATUS (*OD_NetInit_func_t)(OD_Context *context);
typedef OD_MMI_TASK OD_STATUS (*OD_NetDeinit_func_t)(OD_Context *context);
typedef OD_MMI_TASK OD_STATUS (*OD_NetActivateBearer_func_t)(OD_Context *context);
typedef OD_MMI_TASK OD_STATUS (*OD_NetDeactivateBearer_func_t)(OD_Context *context);
typedef OD_MMI_TASK OD_STATUS (*OD_NetGetActiveSimSlots_func_t)(OD_Context *context, OD_BOOL sim_slots[]);
typedef OD_MMI_TASK OD_STATUS (*OD_NetSelectSim_func_t)(OD_Context *context, OD_SimSelectedCallback callback);
typedef OD_MMI_TASK OD_STATUS (*OD_NetSelectApn_func_t)(OD_Context *context, int sim, OD_ApnSelectedCallback callback);
typedef OD_MMI_TASK OD_BOOL (*OD_BearerIsWifi_func_t)(OD_Context *context);
typedef OD_MMI_TASK OD_STATUS (*OD_NetCloseWin_func_t)(void);
typedef OD_MMI_TASK void (*OD_MobileResInit_func_t)(OD_Context *context);
typedef OD_MMI_TASK void (*OD_MiniResInit_func_t)(OD_Context *context);
typedef OD_MMI_TASK int (*OD_GetBreamStringCount_func_t)(OD_Context *context);
typedef OD_MMI_TASK OD_STATUS (*OD_GetBreamString_func_t)(OD_Context *context, int key, const OD_WCHAR** str);
typedef OD_CORE_TASK OD_STATUS (*OD_GetOperaString_func_t)(OD_Context *context, int key, const OD_WCHAR** str);
typedef OD_MMI_TASK const OD_FileBlobType* (*OD_GetStaticFiles_func_t)(OD_Context *context);
typedef OD_MMI_TASK const OD_FileBlobType* (*OD_GetDeployFiles_func_t)(OD_Context *context);
typedef OD_MMI_TASK OD_STATUS (*OD_SchemeInit_func_t)(OD_Context *context);
typedef OD_MMI_TASK OD_STATUS (*OD_SchemeDeinit_func_t)(OD_Context *context);
typedef OD_MMI_TASK OD_STATUS (*OD_SchemeHandle_func_t)(OD_Context *context, const char *uri);
typedef OD_ALL_TASK OD_STATUS (*OD_HostResolve_func_t)(OD_Context *context, const char* url, OD_HostResolveHandle* handle);
typedef OD_ALL_TASK OD_STATUS (*OD_SocketCreate_func_t)(OD_Context *context, OD_Socket * pSocket);
typedef OD_ALL_TASK OD_STATUS (*OD_SocketConnect_func_t)(OD_Context *context, OD_Socket socket, const OD_SocketAddress* pSocketAddress);
typedef OD_ALL_TASK OD_STATUS (*OD_SocketClose_func_t)(OD_Context *context, OD_Socket socket);
typedef OD_ALL_TASK OD_STATUS (*OD_SocketSend_func_t)(OD_Context *context, OD_Socket socket, const void* buffer, int len, int* bytes_written);
typedef OD_ALL_TASK OD_STATUS (*OD_SocketReceive_func_t)(OD_Context *context, OD_Socket socket, void* buffer, int len, int* bytes_received, int* bytes_left);
typedef OD_ALL_TASK OD_STATUS (*od_inet_aton_func_t)(char* addr, unsigned int *ip);
typedef OD_ALL_TASK OD_STATUS (*od_inet_ntoa_func_t)(unsigned int ip, char* addr);
typedef OD_MMI_TASK OD_STATUS (*OD_SystemInit_func_t)(OD_Context *context);
typedef OD_MMI_TASK OD_STATUS (*OD_SystemDeinit_func_t)(OD_Context *context);
typedef OD_MMI_TASK OD_STATUS (*OD_ClipboardSetData_func_t)(OD_Context *context, OD_CLIPBOARD_FORMAT format, void* data, int len);
typedef OD_MMI_TASK OD_STATUS (*OD_ClipboardGetData_func_t)(OD_Context *context, OD_CLIPBOARD_FORMAT format, void** data, int* len);
typedef OD_MMI_TASK OD_STATUS (*OD_ClipboardHasData_func_t)(OD_Context *context, OD_CLIPBOARD_FORMAT format, OD_BOOL* hasData);
typedef OD_ALL_TASK unsigned int (*OD_GetTickCount_func_t)(void);
typedef OD_ALL_TASK unsigned int (*OD_GetUTCTime_func_t)(OD_Context *context);
typedef OD_ALL_TASK OD_STATUS (*OD_GetLocalTime_func_t)(OD_Context *context, int *h, int *m, int *s);
typedef OD_MMI_TASK int (*OD_GetTimeZone_func_t)(OD_Context *context);
typedef OD_ALL_TASK OD_STATUS (*OD_GetSystemProperty_func_t)(OD_Context *context, OD_SYSTEM_PROPERTY_TYPE property, int *value);
typedef OD_ALL_TASK OD_STATUS (*OD_GetLangCountryCode_func_t)(OD_Context *context, char *buf, int buf_size);
typedef OD_ALL_TASK OD_STATUS (*OD_GetPlatformName_func_t)(OD_Context *context, char *buf, int buf_size);
typedef OD_MMI_TASK OD_STATUS (*OD_GetIMEI_func_t)(OD_Context *context, OD_IMEI *imei);
typedef OD_MMI_TASK OD_STATUS (*OD_GetIMSI_func_t)(OD_Context *context, OD_IMSI *imsi);
typedef OD_ALL_TASK OD_STATUS (*OD_TimerCreate_func_t)(OD_Context *context, OD_TaskID task, OD_TimerID* timer, OD_TimerArg userdata);
typedef OD_ALL_TASK OD_STATUS (*OD_TimerDestroy_func_t)(OD_Context *context, OD_TaskID task, OD_TimerID timer);
typedef OD_ALL_TASK OD_STATUS (*OD_TimerStart_func_t)(OD_Context *context, OD_TaskID task, OD_TimerID timer, OD_TIMERFUN callback, int delay, OD_TimerArg userdata);
typedef OD_ALL_TASK OD_STATUS (*OD_TimerStop_func_t)(OD_Context *context, OD_TaskID task, OD_TimerID timer);
typedef OD_ALL_TASK OD_BOOL (*OD_IsTimerStarted_func_t)(OD_Context *context, OD_TaskID task, OD_TimerID timer);
typedef OD_MMI_TASK OD_STATUS (*OD_PlatformOpenDocument_func_t)(OD_Context *context, const OD_FileName *path, const char *mimetype);
typedef OD_MMI_TASK OD_BOOL (*OD_PlatformCanOpenDocument_func_t)(OD_Context *context, const char * mimeType);
typedef OD_ALL_TASK OD_STATUS (*OD_AtomicInit_func_t)(OD_Context *context);
typedef OD_ALL_TASK OD_STATUS (*OD_AtomicDestroy_func_t)(OD_Context *context);
typedef OD_ALL_TASK int (*OD_AtomicIncrement_func_t)(OD_Context *context, int volatile* var);
typedef OD_ALL_TASK int (*OD_AtomicDecrement_func_t)(OD_Context *context, int volatile* var);
typedef OD_ALL_TASK OD_STATUS (*OD_CreateMutex_func_t)(const char* mutex_name, OD_MutexID* mutex);
typedef OD_ALL_TASK OD_STATUS (*OD_ReleaseMutex_func_t)(OD_Context *context, OD_MutexID mutex_id);
typedef OD_ALL_TASK OD_STATUS (*OD_EnterMutex_func_t)(OD_MutexID mutex_id);
typedef OD_ALL_TASK OD_STATUS (*OD_LeaveMutex_func_t)(OD_MutexID mutex_id);
typedef OD_ALL_TASK OD_SemID (*OD_CreateSemaphore_func_t)(OD_Context *context, const char* sem_name, unsigned int initial_count);
typedef OD_ALL_TASK OD_STATUS (*OD_ReleaseSemaphore_func_t)(OD_Context *context, OD_SemID semid, int count);
typedef OD_ALL_TASK OD_STATUS (*OD_TakeSemaphore_func_t)(OD_SemID semid, OD_WaitMode wait_mode);
typedef OD_ALL_TASK OD_STATUS (*OD_GiveSemaphore_func_t)(OD_SemID semid);
typedef OD_ALL_TASK OD_TaskID (*OD_GetCurrentTaskID_func_t)(void);
typedef OD_ALL_TASK OD_TaskID (*OD_GetMMITaskID_func_t)(OD_Context *context);
typedef OD_ALL_TASK OD_TaskID (*OD_GetGogiTaskID_func_t)(OD_Context* context);
typedef OD_MMI_TASK void (*OD_InitMMITaskContext_func_t)(OD_Context *context);
typedef OD_MMI_TASK void (*OD_InitGogiTaskContext_func_t)(OD_Context *context);
typedef OD_MMI_TASK OD_TaskID (*OD_CreateGogiTask_func_t)(OD_Context *context, const char* taskname, const char* queuename,int stacksize, int queuesize, int priority);
typedef OD_MMI_TASK void (*OD_DestroyGogiTask_func_t)(OD_Context* context);
typedef OD_ALL_TASK void (*OD_PostCall_func_t)(OD_AsyncCall* call);
typedef OD_ALL_TASK void (*OD_Yield_func_t)(OD_Context* context);
typedef OD_ALL_TASK int (*od_stricmp_func_t)(const char *s1, const char *s2);
typedef OD_ALL_TASK int (*od_strnicmp_func_t)(const char *s1, const char *s2, unsigned int n);
typedef OD_ALL_TASK unsigned int (*od_wcslen_func_t)(const OD_WCHAR* s);
typedef OD_ALL_TASK OD_WCHAR * (*od_wcsncpy_func_t)(OD_WCHAR *dest, const OD_WCHAR *src, unsigned int n);
typedef OD_ALL_TASK OD_WCHAR * (*od_wcsncat_func_t)(OD_WCHAR *dest, const OD_WCHAR *src, unsigned int n);
typedef OD_ALL_TASK const OD_WCHAR * (*od_wcschr_func_t)(const OD_WCHAR *s, int c0);
typedef OD_ALL_TASK const OD_WCHAR * (*od_wcsrchr_func_t)(const OD_WCHAR *s, int c0);
typedef OD_ALL_TASK OD_WCHAR * (*od_wcsstr_func_t)(const OD_WCHAR *haystack, const OD_WCHAR *needle0);
typedef OD_ALL_TASK int (*od_wcscmp_func_t)(const OD_WCHAR *s1, const OD_WCHAR *s2);
typedef OD_ALL_TASK int (*od_wcsncmp_func_t)(const OD_WCHAR *s1, const OD_WCHAR *s2, int n);
typedef OD_ALL_TASK int (*od_wcsicmp_func_t)(const OD_WCHAR *s1, const OD_WCHAR *s2);
typedef OD_ALL_TASK OD_BOOL (*od_wcsendwith_func_t)(const OD_WCHAR *s1, const OD_WCHAR* s2);

struct OpDevFunc
{
	OD_BookmarkImportIsAvailable_func_t  __OD_BookmarkImportIsAvailable;
	OD_BookmarkImportEnumBegin_func_t  __OD_BookmarkImportEnumBegin;
	OD_BookmarkImportEnumEnd_func_t  __OD_BookmarkImportEnumEnd;
	OD_BookmarkImportGetNextDataSize_func_t  __OD_BookmarkImportGetNextDataSize;
	OD_BookmarkImportGetNext_func_t  __OD_BookmarkImportGetNext;
	OD_InitDebugger_func_t  __OD_InitDebugger;
	OD_DeinitDebugger_func_t  __OD_DeinitDebugger;
	OD_WriteLogFile_func_t  __OD_WriteLogFile;
	OD_Trace_func_t  __OD_Trace;
	OD_TraceWString_func_t  __OD_TraceWString;
	OD_PrintStackTrace_func_t  __OD_PrintStackTrace;
	OD_SetDataBreakPoint_func_t  __OD_SetDataBreakPoint;
	OD_Assert_func_t  __OD_Assert;
	OD_DASRequestSavePath_func_t  __OD_DASRequestSavePath;
	OD_DASReportProgress_func_t  __OD_DASReportProgress;
	OD_DASResponseCancelPrefetch_func_t  __OD_DASResponseCancelPrefetch;
	OD_DASAbortPrefetch_func_t  __OD_DASAbortPrefetch;
	OD_DASStopResponse_func_t  __OD_DASStopResponse;
	OD_DASShowDownloadList_func_t  __OD_DASShowDownloadList;
	OD_DASStartBrowser_func_t  __OD_DASStartBrowser;
	OD_DASExitBrowser_func_t  __OD_DASExitBrowser;
	OD_DASSendAuthenticationData_func_t  __OD_DASSendAuthenticationData;
	OD_DownloadIsMimeTypeSupport_func_t  __OD_DownloadIsMimeTypeSupport;
	OD_DownloadIsDDMimeType_func_t  __OD_DownloadIsDDMimeType;
	OD_DownloadHandleMIMEDownloaded_func_t  __OD_DownloadHandleMIMEDownloaded;
	OD_FileInit_func_t  __OD_FileInit;
	OD_FileDeinit_func_t  __OD_FileDeinit;
	OD_CountFileHandles_func_t  __OD_CountFileHandles;
	OD_FileOpen_func_t  __OD_FileOpen;
	OD_FileClose_func_t  __OD_FileClose;
	OD_FileCommit_func_t  __OD_FileCommit;
	OD_FileDelete_func_t  __OD_FileDelete;
	OD_FileExtend_func_t  __OD_FileExtend;
	OD_FileExist_func_t  __OD_FileExist;
	OD_FileGetAttributes_func_t  __OD_FileGetAttributes;
	OD_FileGetLength_func_t  __OD_FileGetLength;
	OD_FileGetPosition_func_t  __OD_FileGetPosition;
	OD_FileGetTimestampsFromHandle_func_t  __OD_FileGetTimestampsFromHandle;
	OD_FileGetTimestampsFromAttributes_func_t  __OD_FileGetTimestampsFromAttributes;
	OD_FileIsForwardable_func_t  __OD_FileIsForwardable;
	OD_FileRead_func_t  __OD_FileRead;
	OD_FileRename_func_t  __OD_FileRename;
	OD_FileSetAttributes_func_t  __OD_FileSetAttributes;
	OD_FileSetPosition_func_t  __OD_FileSetPosition;
	OD_FileTruncate_func_t  __OD_FileTruncate;
	OD_FileWrite_func_t  __OD_FileWrite;
	OD_FindFirst_func_t  __OD_FindFirst;
	OD_FindNext_func_t  __OD_FindNext;
	OD_FindClose_func_t  __OD_FindClose;
	OD_DirectoryCreate_func_t  __OD_DirectoryCreate;
	OD_DirectoryRemove_func_t  __OD_DirectoryRemove;
	OD_DirectoryGetSize_func_t  __OD_DirectoryGetSize;
	OD_DirectoryGetPredefined_func_t  __OD_DirectoryGetPredefined;
	OD_GetCardDriveLetter_func_t  __OD_GetCardDriveLetter;
	OD_GetPublicDriveLetter_func_t  __OD_GetPublicDriveLetter;
	OD_GetSystemDriveLetter_func_t  __OD_GetSystemDriveLetter;
	OD_GetStorageDriveLetter_func_t  __OD_GetStorageDriveLetter;
	OD_DriveGetAllVisibleLetters_func_t  __OD_DriveGetAllVisibleLetters;
	OD_DriveGetCurrentLetter_func_t  __OD_DriveGetCurrentLetter;
	OD_DriveGetSystemLetters_func_t  __OD_DriveGetSystemLetters;
	OD_GetPrestorageFolder_func_t  __OD_GetPrestorageFolder;
	OD_GetStorageFolder_func_t  __OD_GetStorageFolder;
	OD_DriveIsWritable_func_t  __OD_DriveIsWritable;
	OD_GetDriveFreeSpace_func_t  __OD_GetDriveFreeSpace;
	OD_PathsEqual_func_t  __OD_PathsEqual;
	OD_XDelete_func_t  __OD_XDelete;
	OD_FontInit_func_t  __OD_FontInit;
	OD_FontDeinit_func_t  __OD_FontDeinit;
	OD_GetFontMetrics_func_t  __OD_GetFontMetrics;
	OD_GetCharMetrics_func_t  __OD_GetCharMetrics;
	OD_GetTextWidth_func_t  __OD_GetTextWidth;
	OD_MeasureString_func_t  __OD_MeasureString;
	OD_DrawText_func_t  __OD_DrawText;
	OD_GetTextData_func_t  __OD_GetTextData;
	OD_GDIInit_func_t  __OD_GDIInit;
	OD_GDIDeinit_func_t  __OD_GDIDeinit;
	OD_GDIUseNativeMemory_func_t  __OD_GDIUseNativeMemory;
	OD_GDIAllocMemory_func_t  __OD_GDIAllocMemory;
	OD_GDIFreeMemory_func_t  __OD_GDIFreeMemory;
	OD_GDICreateLayer_func_t  __OD_GDICreateLayer;
	OD_GDIDestroyLayer_func_t  __OD_GDIDestroyLayer;
	OD_ShowOpera_func_t  __OD_ShowOpera;
	OD_HideOpera_func_t  __OD_HideOpera;
	OD_Paint_func_t  __OD_Paint;
	OD_GetScreenBuffer_func_t  __OD_GetScreenBuffer;
	OD_GetViewportArea_func_t  __OD_GetViewportArea;
	OD_GetLayerDimension_func_t  __OD_GetLayerDimension;
	OD_GetLayerPixelFormat_func_t  __OD_GetLayerPixelFormat;
	OD_GetLayerBPP_func_t  __OD_GetLayerBPP;
	OD_GetScreenDPI_func_t  __OD_GetScreenDPI;
	OD_GetRotationAngle_func_t  __OD_GetRotationAngle;
	OD_RotateScreen_func_t  __OD_RotateScreen;
	OD_GetScreenMode_func_t  __OD_GetScreenMode;
	OD_ToggleFullScreen_func_t  __OD_ToggleFullScreen;
	OD_EnterFullScreenEditor_func_t  __OD_EnterFullScreenEditor;
	OD_CloseFullScreenEditor_func_t  __OD_CloseFullScreenEditor;
	OD_ImeEnterScreen_func_t  __OD_ImeEnterScreen;
	OD_ImeExitScreen_func_t  __OD_ImeExitScreen;
	OD_ImeConnect_func_t  __OD_ImeConnect;
	OD_ImeDisconnect_func_t  __OD_ImeDisconnect;
	OD_ImeBeforeEditorOrCaretChanged_func_t  __OD_ImeBeforeEditorOrCaretChanged;
	OD_ImeGetImuiHeight_func_t  __OD_ImeGetImuiHeight;
	OD_DrawImePart_func_t  __OD_DrawImePart;
	OD_ImeSetSoftKeyLabel_func_t  __OD_ImeSetSoftKeyLabel;
	OD_TranslateKeyCode_func_t  __OD_TranslateKeyCode;
	OD_TranslatePenEvent_func_t  __OD_TranslatePenEvent;
	KeyPenTracker_init_func_t  __KeyPenTracker_init;
	KeyPenTracker_destroy_func_t  __KeyPenTracker_destroy;
	KeyPenTracker_resetStatus_func_t  __KeyPenTracker_resetStatus;
	KeyPenTracker_handleEvent_func_t  __KeyPenTracker_handleEvent;
	OD_ExitUI_func_t  __OD_ExitUI;
	OD_InitApp_func_t  __OD_InitApp;
	OD_DeinitApp_func_t  __OD_DeinitApp;
	OD_MemoryCreatePool_func_t  __OD_MemoryCreatePool;
	OD_MemoryDestroyPool_func_t  __OD_MemoryDestroyPool;
	OD_MemoryGetPoolSize_func_t  __OD_MemoryGetPoolSize;
	OD_MemoryGetPoolBase_func_t  __OD_MemoryGetPoolBase;
	OD_MemoryBlocksInit_func_t  __OD_MemoryBlocksInit;
	OD_MemoryBlocksDeinit_func_t  __OD_MemoryBlocksDeinit;
	OD_Malloc_func_t  __OD_Malloc;
	OD_Free_func_t  __OD_Free;
	OD_NetGetID_func_t  __OD_NetGetID;
	OD_NetGetProxy_func_t  __OD_NetGetProxy;
	OD_NetInit_func_t  __OD_NetInit;
	OD_NetDeinit_func_t  __OD_NetDeinit;
	OD_NetActivateBearer_func_t  __OD_NetActivateBearer;
	OD_NetDeactivateBearer_func_t  __OD_NetDeactivateBearer;
	OD_NetGetActiveSimSlots_func_t  __OD_NetGetActiveSimSlots;
	OD_NetSelectSim_func_t  __OD_NetSelectSim;
	OD_NetSelectApn_func_t  __OD_NetSelectApn;
	OD_BearerIsWifi_func_t  __OD_BearerIsWifi;
	OD_NetCloseWin_func_t  __OD_NetCloseWin;
	OD_MobileResInit_func_t  __OD_MobileResInit;
	OD_MiniResInit_func_t  __OD_MiniResInit;
	OD_GetBreamStringCount_func_t  __OD_GetBreamStringCount;
	OD_GetBreamString_func_t  __OD_GetBreamString;
	OD_GetOperaString_func_t  __OD_GetOperaString;
	OD_GetStaticFiles_func_t  __OD_GetStaticFiles;
	OD_GetDeployFiles_func_t  __OD_GetDeployFiles;
	OD_SchemeInit_func_t  __OD_SchemeInit;
	OD_SchemeDeinit_func_t  __OD_SchemeDeinit;
	OD_SchemeHandle_func_t  __OD_SchemeHandle;
	OD_HostResolve_func_t  __OD_HostResolve;
	OD_SocketCreate_func_t  __OD_SocketCreate;
	OD_SocketConnect_func_t  __OD_SocketConnect;
	OD_SocketClose_func_t  __OD_SocketClose;
	OD_SocketSend_func_t  __OD_SocketSend;
	OD_SocketReceive_func_t  __OD_SocketReceive;
	od_inet_aton_func_t  __od_inet_aton;
	od_inet_ntoa_func_t  __od_inet_ntoa;
	OD_SystemInit_func_t  __OD_SystemInit;
	OD_SystemDeinit_func_t  __OD_SystemDeinit;
	OD_ClipboardSetData_func_t  __OD_ClipboardSetData;
	OD_ClipboardGetData_func_t  __OD_ClipboardGetData;
	OD_ClipboardHasData_func_t  __OD_ClipboardHasData;
	OD_GetTickCount_func_t  __OD_GetTickCount;
	OD_GetUTCTime_func_t  __OD_GetUTCTime;
	OD_GetLocalTime_func_t  __OD_GetLocalTime;
	OD_GetTimeZone_func_t  __OD_GetTimeZone;
	OD_GetSystemProperty_func_t  __OD_GetSystemProperty;
	OD_GetLangCountryCode_func_t  __OD_GetLangCountryCode;
	OD_GetPlatformName_func_t  __OD_GetPlatformName;
	OD_GetIMEI_func_t  __OD_GetIMEI;
	OD_GetIMSI_func_t  __OD_GetIMSI;
	OD_TimerCreate_func_t  __OD_TimerCreate;
	OD_TimerDestroy_func_t  __OD_TimerDestroy;
	OD_TimerStart_func_t  __OD_TimerStart;
	OD_TimerStop_func_t  __OD_TimerStop;
	OD_IsTimerStarted_func_t  __OD_IsTimerStarted;
	OD_PlatformOpenDocument_func_t  __OD_PlatformOpenDocument;
	OD_PlatformCanOpenDocument_func_t  __OD_PlatformCanOpenDocument;
	OD_AtomicInit_func_t  __OD_AtomicInit;
	OD_AtomicDestroy_func_t  __OD_AtomicDestroy;
	OD_AtomicIncrement_func_t  __OD_AtomicIncrement;
	OD_AtomicDecrement_func_t  __OD_AtomicDecrement;
	OD_CreateMutex_func_t  __OD_CreateMutex;
	OD_ReleaseMutex_func_t  __OD_ReleaseMutex;
	OD_EnterMutex_func_t  __OD_EnterMutex;
	OD_LeaveMutex_func_t  __OD_LeaveMutex;
	OD_CreateSemaphore_func_t  __OD_CreateSemaphore;
	OD_ReleaseSemaphore_func_t  __OD_ReleaseSemaphore;
	OD_TakeSemaphore_func_t  __OD_TakeSemaphore;
	OD_GiveSemaphore_func_t  __OD_GiveSemaphore;
	OD_GetCurrentTaskID_func_t  __OD_GetCurrentTaskID;
	OD_GetMMITaskID_func_t  __OD_GetMMITaskID;
	OD_GetGogiTaskID_func_t  __OD_GetGogiTaskID;
	OD_InitMMITaskContext_func_t  __OD_InitMMITaskContext;
	OD_InitGogiTaskContext_func_t  __OD_InitGogiTaskContext;
	OD_CreateGogiTask_func_t  __OD_CreateGogiTask;
	OD_DestroyGogiTask_func_t  __OD_DestroyGogiTask;
	OD_PostCall_func_t  __OD_PostCall;
	OD_Yield_func_t  __OD_Yield;
	od_stricmp_func_t  __od_stricmp;
	od_strnicmp_func_t  __od_strnicmp;
	od_wcslen_func_t  __od_wcslen;
	od_wcsncpy_func_t  __od_wcsncpy;
	od_wcsncat_func_t  __od_wcsncat;
	od_wcschr_func_t  __od_wcschr;
	od_wcsrchr_func_t  __od_wcsrchr;
	od_wcsstr_func_t  __od_wcsstr;
	od_wcscmp_func_t  __od_wcscmp;
	od_wcsncmp_func_t  __od_wcsncmp;
	od_wcsicmp_func_t  __od_wcsicmp;
	od_wcsendwith_func_t  __od_wcsendwith;
};

#define OD_BookmarkImportIsAvailable  getOpDevFuncs()->__OD_BookmarkImportIsAvailable
#define OD_BookmarkImportEnumBegin  getOpDevFuncs()->__OD_BookmarkImportEnumBegin
#define OD_BookmarkImportEnumEnd  getOpDevFuncs()->__OD_BookmarkImportEnumEnd
#define OD_BookmarkImportGetNextDataSize  getOpDevFuncs()->__OD_BookmarkImportGetNextDataSize
#define OD_BookmarkImportGetNext  getOpDevFuncs()->__OD_BookmarkImportGetNext
#define OD_InitDebugger  getOpDevFuncs()->__OD_InitDebugger
#define OD_DeinitDebugger  getOpDevFuncs()->__OD_DeinitDebugger
#define OD_WriteLogFile  getOpDevFuncs()->__OD_WriteLogFile
#define OD_Trace  getOpDevFuncs()->__OD_Trace
#define OD_TraceWString  getOpDevFuncs()->__OD_TraceWString
#define OD_PrintStackTrace  getOpDevFuncs()->__OD_PrintStackTrace
#define OD_SetDataBreakPoint  getOpDevFuncs()->__OD_SetDataBreakPoint
#define OD_Assert  getOpDevFuncs()->__OD_Assert
#define OD_DASRequestSavePath  getOpDevFuncs()->__OD_DASRequestSavePath
#define OD_DASReportProgress  getOpDevFuncs()->__OD_DASReportProgress
#define OD_DASResponseCancelPrefetch  getOpDevFuncs()->__OD_DASResponseCancelPrefetch
#define OD_DASAbortPrefetch  getOpDevFuncs()->__OD_DASAbortPrefetch
#define OD_DASStopResponse  getOpDevFuncs()->__OD_DASStopResponse
#define OD_DASShowDownloadList  getOpDevFuncs()->__OD_DASShowDownloadList
#define OD_DASStartBrowser  getOpDevFuncs()->__OD_DASStartBrowser
#define OD_DASExitBrowser  getOpDevFuncs()->__OD_DASExitBrowser
#define OD_DASSendAuthenticationData  getOpDevFuncs()->__OD_DASSendAuthenticationData
#define OD_DownloadIsMimeTypeSupport  getOpDevFuncs()->__OD_DownloadIsMimeTypeSupport
#define OD_DownloadIsDDMimeType  getOpDevFuncs()->__OD_DownloadIsDDMimeType
#define OD_DownloadHandleMIMEDownloaded  getOpDevFuncs()->__OD_DownloadHandleMIMEDownloaded
#define OD_FileInit  getOpDevFuncs()->__OD_FileInit
#define OD_FileDeinit  getOpDevFuncs()->__OD_FileDeinit
#define OD_CountFileHandles  getOpDevFuncs()->__OD_CountFileHandles
#define OD_FileOpen  getOpDevFuncs()->__OD_FileOpen
#define OD_FileClose  getOpDevFuncs()->__OD_FileClose
#define OD_FileCommit  getOpDevFuncs()->__OD_FileCommit
#define OD_FileDelete  getOpDevFuncs()->__OD_FileDelete
#define OD_FileExtend  getOpDevFuncs()->__OD_FileExtend
#define OD_FileExist  getOpDevFuncs()->__OD_FileExist
#define OD_FileGetAttributes  getOpDevFuncs()->__OD_FileGetAttributes
#define OD_FileGetLength  getOpDevFuncs()->__OD_FileGetLength
#define OD_FileGetPosition  getOpDevFuncs()->__OD_FileGetPosition
#define OD_FileGetTimestampsFromHandle  getOpDevFuncs()->__OD_FileGetTimestampsFromHandle
#define OD_FileGetTimestampsFromAttributes  getOpDevFuncs()->__OD_FileGetTimestampsFromAttributes
#define OD_FileIsForwardable  getOpDevFuncs()->__OD_FileIsForwardable
#define OD_FileRead  getOpDevFuncs()->__OD_FileRead
#define OD_FileRename  getOpDevFuncs()->__OD_FileRename
#define OD_FileSetAttributes  getOpDevFuncs()->__OD_FileSetAttributes
#define OD_FileSetPosition  getOpDevFuncs()->__OD_FileSetPosition
#define OD_FileTruncate  getOpDevFuncs()->__OD_FileTruncate
#define OD_FileWrite  getOpDevFuncs()->__OD_FileWrite
#define OD_FindFirst  getOpDevFuncs()->__OD_FindFirst
#define OD_FindNext  getOpDevFuncs()->__OD_FindNext
#define OD_FindClose  getOpDevFuncs()->__OD_FindClose
#define OD_DirectoryCreate  getOpDevFuncs()->__OD_DirectoryCreate
#define OD_DirectoryRemove  getOpDevFuncs()->__OD_DirectoryRemove
#define OD_DirectoryGetSize  getOpDevFuncs()->__OD_DirectoryGetSize
#define OD_DirectoryGetPredefined  getOpDevFuncs()->__OD_DirectoryGetPredefined
#define OD_GetCardDriveLetter  getOpDevFuncs()->__OD_GetCardDriveLetter
#define OD_GetPublicDriveLetter  getOpDevFuncs()->__OD_GetPublicDriveLetter
#define OD_GetSystemDriveLetter  getOpDevFuncs()->__OD_GetSystemDriveLetter
#define OD_GetStorageDriveLetter  getOpDevFuncs()->__OD_GetStorageDriveLetter
#define OD_DriveGetAllVisibleLetters  getOpDevFuncs()->__OD_DriveGetAllVisibleLetters
#define OD_DriveGetCurrentLetter  getOpDevFuncs()->__OD_DriveGetCurrentLetter
#define OD_DriveGetSystemLetters  getOpDevFuncs()->__OD_DriveGetSystemLetters
#define OD_GetPrestorageFolder  getOpDevFuncs()->__OD_GetPrestorageFolder
#define OD_GetStorageFolder  getOpDevFuncs()->__OD_GetStorageFolder
#define OD_DriveIsWritable  getOpDevFuncs()->__OD_DriveIsWritable
#define OD_GetDriveFreeSpace  getOpDevFuncs()->__OD_GetDriveFreeSpace
#define OD_PathsEqual  getOpDevFuncs()->__OD_PathsEqual
#define OD_XDelete  getOpDevFuncs()->__OD_XDelete
#define OD_FontInit  getOpDevFuncs()->__OD_FontInit
#define OD_FontDeinit  getOpDevFuncs()->__OD_FontDeinit
#define OD_GetFontMetrics  getOpDevFuncs()->__OD_GetFontMetrics
#define OD_GetCharMetrics  getOpDevFuncs()->__OD_GetCharMetrics
#define OD_GetTextWidth  getOpDevFuncs()->__OD_GetTextWidth
#define OD_MeasureString  getOpDevFuncs()->__OD_MeasureString
#define OD_DrawText  getOpDevFuncs()->__OD_DrawText
#define OD_GetTextData  getOpDevFuncs()->__OD_GetTextData
#define OD_GDIInit  getOpDevFuncs()->__OD_GDIInit
#define OD_GDIDeinit  getOpDevFuncs()->__OD_GDIDeinit
#define OD_GDIUseNativeMemory  getOpDevFuncs()->__OD_GDIUseNativeMemory
#define OD_GDIAllocMemory  getOpDevFuncs()->__OD_GDIAllocMemory
#define OD_GDIFreeMemory  getOpDevFuncs()->__OD_GDIFreeMemory
#define OD_GDICreateLayer  getOpDevFuncs()->__OD_GDICreateLayer
#define OD_GDIDestroyLayer  getOpDevFuncs()->__OD_GDIDestroyLayer
#define OD_ShowOpera  getOpDevFuncs()->__OD_ShowOpera
#define OD_HideOpera  getOpDevFuncs()->__OD_HideOpera
#define OD_Paint  getOpDevFuncs()->__OD_Paint
#define OD_GetScreenBuffer  getOpDevFuncs()->__OD_GetScreenBuffer
#define OD_GetViewportArea  getOpDevFuncs()->__OD_GetViewportArea
#define OD_GetLayerDimension  getOpDevFuncs()->__OD_GetLayerDimension
#define OD_GetLayerPixelFormat  getOpDevFuncs()->__OD_GetLayerPixelFormat
#define OD_GetLayerBPP  getOpDevFuncs()->__OD_GetLayerBPP
#define OD_GetScreenDPI  getOpDevFuncs()->__OD_GetScreenDPI
#define OD_GetRotationAngle  getOpDevFuncs()->__OD_GetRotationAngle
#define OD_RotateScreen  getOpDevFuncs()->__OD_RotateScreen
#define OD_GetScreenMode  getOpDevFuncs()->__OD_GetScreenMode
#define OD_ToggleFullScreen  getOpDevFuncs()->__OD_ToggleFullScreen
#define OD_EnterFullScreenEditor  getOpDevFuncs()->__OD_EnterFullScreenEditor
#define OD_CloseFullScreenEditor  getOpDevFuncs()->__OD_CloseFullScreenEditor
#define OD_ImeEnterScreen  getOpDevFuncs()->__OD_ImeEnterScreen
#define OD_ImeExitScreen  getOpDevFuncs()->__OD_ImeExitScreen
#define OD_ImeConnect  getOpDevFuncs()->__OD_ImeConnect
#define OD_ImeDisconnect  getOpDevFuncs()->__OD_ImeDisconnect
#define OD_ImeBeforeEditorOrCaretChanged  getOpDevFuncs()->__OD_ImeBeforeEditorOrCaretChanged
#define OD_ImeGetImuiHeight  getOpDevFuncs()->__OD_ImeGetImuiHeight
#define OD_DrawImePart  getOpDevFuncs()->__OD_DrawImePart
#define OD_ImeSetSoftKeyLabel  getOpDevFuncs()->__OD_ImeSetSoftKeyLabel
#define OD_TranslateKeyCode  getOpDevFuncs()->__OD_TranslateKeyCode
#define OD_TranslatePenEvent  getOpDevFuncs()->__OD_TranslatePenEvent
#define KeyPenTracker_init  getOpDevFuncs()->__KeyPenTracker_init
#define KeyPenTracker_destroy  getOpDevFuncs()->__KeyPenTracker_destroy
#define KeyPenTracker_resetStatus  getOpDevFuncs()->__KeyPenTracker_resetStatus
#define KeyPenTracker_handleEvent  getOpDevFuncs()->__KeyPenTracker_handleEvent
#define OD_ExitUI  getOpDevFuncs()->__OD_ExitUI
#define OD_InitApp  getOpDevFuncs()->__OD_InitApp
#define OD_DeinitApp  getOpDevFuncs()->__OD_DeinitApp
#define OD_MemoryCreatePool  getOpDevFuncs()->__OD_MemoryCreatePool
#define OD_MemoryDestroyPool  getOpDevFuncs()->__OD_MemoryDestroyPool
#define OD_MemoryGetPoolSize  getOpDevFuncs()->__OD_MemoryGetPoolSize
#define OD_MemoryGetPoolBase  getOpDevFuncs()->__OD_MemoryGetPoolBase
#define OD_MemoryBlocksInit  getOpDevFuncs()->__OD_MemoryBlocksInit
#define OD_MemoryBlocksDeinit  getOpDevFuncs()->__OD_MemoryBlocksDeinit
#define OD_Malloc  getOpDevFuncs()->__OD_Malloc
#define OD_Free  getOpDevFuncs()->__OD_Free
#define OD_NetGetID  getOpDevFuncs()->__OD_NetGetID
#define OD_NetGetProxy  getOpDevFuncs()->__OD_NetGetProxy
#define OD_NetInit  getOpDevFuncs()->__OD_NetInit
#define OD_NetDeinit  getOpDevFuncs()->__OD_NetDeinit
#define OD_NetActivateBearer  getOpDevFuncs()->__OD_NetActivateBearer
#define OD_NetDeactivateBearer  getOpDevFuncs()->__OD_NetDeactivateBearer
#define OD_NetGetActiveSimSlots  getOpDevFuncs()->__OD_NetGetActiveSimSlots
#define OD_NetSelectSim  getOpDevFuncs()->__OD_NetSelectSim
#define OD_NetSelectApn  getOpDevFuncs()->__OD_NetSelectApn
#define OD_BearerIsWifi  getOpDevFuncs()->__OD_BearerIsWifi
#define OD_NetCloseWin  getOpDevFuncs()->__OD_NetCloseWin
#define OD_MobileResInit  getOpDevFuncs()->__OD_MobileResInit
#define OD_MiniResInit  getOpDevFuncs()->__OD_MiniResInit
#define OD_GetBreamStringCount  getOpDevFuncs()->__OD_GetBreamStringCount
#define OD_GetBreamString  getOpDevFuncs()->__OD_GetBreamString
#define OD_GetOperaString  getOpDevFuncs()->__OD_GetOperaString
#define OD_GetStaticFiles  getOpDevFuncs()->__OD_GetStaticFiles
#define OD_GetDeployFiles  getOpDevFuncs()->__OD_GetDeployFiles
#define OD_SchemeInit  getOpDevFuncs()->__OD_SchemeInit
#define OD_SchemeDeinit  getOpDevFuncs()->__OD_SchemeDeinit
#define OD_SchemeHandle  getOpDevFuncs()->__OD_SchemeHandle
#define OD_HostResolve  getOpDevFuncs()->__OD_HostResolve
#define OD_SocketCreate  getOpDevFuncs()->__OD_SocketCreate
#define OD_SocketConnect  getOpDevFuncs()->__OD_SocketConnect
#define OD_SocketClose  getOpDevFuncs()->__OD_SocketClose
#define OD_SocketSend  getOpDevFuncs()->__OD_SocketSend
#define OD_SocketReceive  getOpDevFuncs()->__OD_SocketReceive
#define od_inet_aton  getOpDevFuncs()->__od_inet_aton
#define od_inet_ntoa  getOpDevFuncs()->__od_inet_ntoa
#define OD_SystemInit  getOpDevFuncs()->__OD_SystemInit
#define OD_SystemDeinit  getOpDevFuncs()->__OD_SystemDeinit
#define OD_ClipboardSetData  getOpDevFuncs()->__OD_ClipboardSetData
#define OD_ClipboardGetData  getOpDevFuncs()->__OD_ClipboardGetData
#define OD_ClipboardHasData  getOpDevFuncs()->__OD_ClipboardHasData
#define OD_GetTickCount  getOpDevFuncs()->__OD_GetTickCount
#define OD_GetUTCTime  getOpDevFuncs()->__OD_GetUTCTime
#define OD_GetLocalTime  getOpDevFuncs()->__OD_GetLocalTime
#define OD_GetTimeZone  getOpDevFuncs()->__OD_GetTimeZone
#define OD_GetSystemProperty  getOpDevFuncs()->__OD_GetSystemProperty
#define OD_GetLangCountryCode  getOpDevFuncs()->__OD_GetLangCountryCode
#define OD_GetPlatformName  getOpDevFuncs()->__OD_GetPlatformName
#define OD_GetIMEI  getOpDevFuncs()->__OD_GetIMEI
#define OD_GetIMSI  getOpDevFuncs()->__OD_GetIMSI
#define OD_TimerCreate  getOpDevFuncs()->__OD_TimerCreate
#define OD_TimerDestroy  getOpDevFuncs()->__OD_TimerDestroy
#define OD_TimerStart  getOpDevFuncs()->__OD_TimerStart
#define OD_TimerStop  getOpDevFuncs()->__OD_TimerStop
#define OD_IsTimerStarted  getOpDevFuncs()->__OD_IsTimerStarted
#define OD_PlatformOpenDocument  getOpDevFuncs()->__OD_PlatformOpenDocument
#define OD_PlatformCanOpenDocument  getOpDevFuncs()->__OD_PlatformCanOpenDocument
#define OD_AtomicInit  getOpDevFuncs()->__OD_AtomicInit
#define OD_AtomicDestroy  getOpDevFuncs()->__OD_AtomicDestroy
#define OD_AtomicIncrement  getOpDevFuncs()->__OD_AtomicIncrement
#define OD_AtomicDecrement  getOpDevFuncs()->__OD_AtomicDecrement
#define OD_CreateMutex  getOpDevFuncs()->__OD_CreateMutex
#define OD_ReleaseMutex  getOpDevFuncs()->__OD_ReleaseMutex
#define OD_EnterMutex  getOpDevFuncs()->__OD_EnterMutex
#define OD_LeaveMutex  getOpDevFuncs()->__OD_LeaveMutex
#define OD_CreateSemaphore  getOpDevFuncs()->__OD_CreateSemaphore
#define OD_ReleaseSemaphore  getOpDevFuncs()->__OD_ReleaseSemaphore
#define OD_TakeSemaphore  getOpDevFuncs()->__OD_TakeSemaphore
#define OD_GiveSemaphore  getOpDevFuncs()->__OD_GiveSemaphore
#define OD_GetCurrentTaskID  getOpDevFuncs()->__OD_GetCurrentTaskID
#define OD_GetMMITaskID  getOpDevFuncs()->__OD_GetMMITaskID
#define OD_GetGogiTaskID  getOpDevFuncs()->__OD_GetGogiTaskID
#define OD_InitMMITaskContext  getOpDevFuncs()->__OD_InitMMITaskContext
#define OD_InitGogiTaskContext  getOpDevFuncs()->__OD_InitGogiTaskContext
#define OD_CreateGogiTask  getOpDevFuncs()->__OD_CreateGogiTask
#define OD_DestroyGogiTask  getOpDevFuncs()->__OD_DestroyGogiTask
#define OD_PostCall  getOpDevFuncs()->__OD_PostCall
#define OD_Yield  getOpDevFuncs()->__OD_Yield
#define od_stricmp  getOpDevFuncs()->__od_stricmp
#define od_strnicmp  getOpDevFuncs()->__od_strnicmp
#define od_wcslen  getOpDevFuncs()->__od_wcslen
#define od_wcsncpy  getOpDevFuncs()->__od_wcsncpy
#define od_wcsncat  getOpDevFuncs()->__od_wcsncat
#define od_wcschr  getOpDevFuncs()->__od_wcschr
#define od_wcsrchr  getOpDevFuncs()->__od_wcsrchr
#define od_wcsstr  getOpDevFuncs()->__od_wcsstr
#define od_wcscmp  getOpDevFuncs()->__od_wcscmp
#define od_wcsncmp  getOpDevFuncs()->__od_wcsncmp
#define od_wcsicmp  getOpDevFuncs()->__od_wcsicmp
#define od_wcsendwith  getOpDevFuncs()->__od_wcsendwith

extern struct OpDevFunc* getOpDevFuncs();
extern void   OD_InitFuncs();

#ifdef __cplusplus
}
#endif

#endif // OPDEVSIM_H
