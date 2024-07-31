/** Generated file, DO NOT edit it manually */
#include "opdev.h"

struct OpDevFunc* g_od_funcs = NULL;
struct OpDevFunc g_opdevfuncs;

struct OpDevFunc* getOpDevFuncs()
{
	return &g_opdevfuncs;
}
#undef OD_BookmarkImportIsAvailable
#undef OD_BookmarkImportEnumBegin
#undef OD_BookmarkImportEnumEnd
#undef OD_BookmarkImportGetNextDataSize
#undef OD_BookmarkImportGetNext
#undef OD_InitDebugger
#undef OD_DeinitDebugger
#undef OD_WriteLogFile
#undef OD_Trace
#undef OD_TraceWString
#undef OD_PrintStackTrace
#undef OD_SetDataBreakPoint
#undef OD_Assert
#undef OD_DASRequestSavePath
#undef OD_DASReportProgress
#undef OD_DASResponseCancelPrefetch
#undef OD_DASAbortPrefetch
#undef OD_DASStopResponse
#undef OD_DASShowDownloadList
#undef OD_DASStartBrowser
#undef OD_DASExitBrowser
#undef OD_DASSendAuthenticationData
#undef OD_DownloadIsMimeTypeSupport
#undef OD_DownloadIsDDMimeType
#undef OD_DownloadHandleMIMEDownloaded
#undef OD_FileInit
#undef OD_FileDeinit
#undef OD_CountFileHandles
#undef OD_FileOpen
#undef OD_FileClose
#undef OD_FileCommit
#undef OD_FileDelete
#undef OD_FileExtend
#undef OD_FileExist
#undef OD_FileGetAttributes
#undef OD_FileGetLength
#undef OD_FileGetPosition
#undef OD_FileGetTimestampsFromHandle
#undef OD_FileGetTimestampsFromAttributes
#undef OD_FileIsForwardable
#undef OD_FileRead
#undef OD_FileRename
#undef OD_FileSetAttributes
#undef OD_FileSetPosition
#undef OD_FileTruncate
#undef OD_FileWrite
#undef OD_FindFirst
#undef OD_FindNext
#undef OD_FindClose
#undef OD_DirectoryCreate
#undef OD_DirectoryRemove
#undef OD_DirectoryGetSize
#undef OD_DirectoryGetPredefined
#undef OD_GetCardDriveLetter
#undef OD_GetPublicDriveLetter
#undef OD_GetSystemDriveLetter
#undef OD_GetStorageDriveLetter
#undef OD_DriveGetAllVisibleLetters
#undef OD_DriveGetCurrentLetter
#undef OD_DriveGetSystemLetters
#undef OD_GetPrestorageFolder
#undef OD_GetStorageFolder
#undef OD_DriveIsWritable
#undef OD_GetDriveFreeSpace
#undef OD_PathsEqual
#undef OD_XDelete
#undef OD_FontInit
#undef OD_FontDeinit
#undef OD_GetFontMetrics
#undef OD_GetCharMetrics
#undef OD_GetTextWidth
#undef OD_MeasureString
#undef OD_DrawText
#undef OD_GetTextData
#undef OD_GDIInit
#undef OD_GDIDeinit
#undef OD_GDIUseNativeMemory
#undef OD_GDIAllocMemory
#undef OD_GDIFreeMemory
#undef OD_GDICreateLayer
#undef OD_GDIDestroyLayer
#undef OD_ShowOpera
#undef OD_HideOpera
#undef OD_Paint
#undef OD_GetScreenBuffer
#undef OD_GetViewportArea
#undef OD_GetLayerDimension
#undef OD_GetLayerPixelFormat
#undef OD_GetLayerBPP
#undef OD_GetScreenDPI
#undef OD_GetRotationAngle
#undef OD_RotateScreen
#undef OD_GetScreenMode
#undef OD_ToggleFullScreen
#undef OD_EnterFullScreenEditor
#undef OD_CloseFullScreenEditor
#undef OD_ImeEnterScreen
#undef OD_ImeExitScreen
#undef OD_ImeConnect
#undef OD_ImeDisconnect
#undef OD_ImeBeforeEditorOrCaretChanged
#undef OD_ImeGetImuiHeight
#undef OD_DrawImePart
#undef OD_ImeSetSoftKeyLabel
#undef OD_TranslateKeyCode
#undef OD_TranslatePenEvent
#undef KeyPenTracker_init
#undef KeyPenTracker_destroy
#undef KeyPenTracker_resetStatus
#undef KeyPenTracker_handleEvent
#undef OD_ExitUI
#undef OD_InitApp
#undef OD_DeinitApp
#undef OD_MemoryCreatePool
#undef OD_MemoryDestroyPool
#undef OD_MemoryGetPoolSize
#undef OD_MemoryGetPoolBase
#undef OD_MemoryBlocksInit
#undef OD_MemoryBlocksDeinit
#undef OD_Malloc
#undef OD_Free
#undef OD_NetGetID
#undef OD_NetGetProxy
#undef OD_NetInit
#undef OD_NetDeinit
#undef OD_NetActivateBearer
#undef OD_NetDeactivateBearer
#undef OD_NetGetActiveSimSlots
#undef OD_NetSelectSim
#undef OD_NetSelectApn
#undef OD_BearerIsWifi
#undef OD_NetCloseWin
#undef OD_MobileResInit
#undef OD_MiniResInit
#undef OD_GetBreamStringCount
#undef OD_GetBreamString
#undef OD_GetOperaString
#undef OD_GetStaticFiles
#undef OD_GetDeployFiles
#undef OD_SchemeInit
#undef OD_SchemeDeinit
#undef OD_SchemeHandle
#undef OD_HostResolve
#undef OD_SocketCreate
#undef OD_SocketConnect
#undef OD_SocketClose
#undef OD_SocketSend
#undef OD_SocketReceive
#undef od_inet_aton
#undef od_inet_ntoa
#undef OD_SystemInit
#undef OD_SystemDeinit
#undef OD_ClipboardSetData
#undef OD_ClipboardGetData
#undef OD_ClipboardHasData
#undef OD_GetTickCount
#undef OD_GetUTCTime
#undef OD_GetLocalTime
#undef OD_GetTimeZone
#undef OD_GetSystemProperty
#undef OD_GetLangCountryCode
#undef OD_GetPlatformName
#undef OD_GetIMEI
#undef OD_GetIMSI
#undef OD_TimerCreate
#undef OD_TimerDestroy
#undef OD_TimerStart
#undef OD_TimerStop
#undef OD_IsTimerStarted
#undef OD_PlatformOpenDocument
#undef OD_PlatformCanOpenDocument
#undef OD_GetNativeString
#undef OD_AtomicInit
#undef OD_AtomicDestroy
#undef OD_AtomicIncrement
#undef OD_AtomicDecrement
#undef OD_CreateMutex
#undef OD_ReleaseMutex
#undef OD_EnterMutex
#undef OD_LeaveMutex
#undef OD_CreateSemaphore
#undef OD_ReleaseSemaphore
#undef OD_TakeSemaphore
#undef OD_GiveSemaphore
#undef OD_GetCurrentTaskID
#undef OD_GetMMITaskID
#undef OD_GetGogiTaskID
#undef OD_InitMMITaskContext
#undef OD_InitGogiTaskContext
#undef OD_CreateGogiTask
#undef OD_DestroyGogiTask
#undef OD_PostCall
#undef OD_Yield
#undef od_stricmp
#undef od_strnicmp
#undef od_wcslen
#undef od_wcsncpy
#undef od_wcsncat
#undef od_wcschr
#undef od_wcsrchr
#undef od_wcsstr
#undef od_wcscmp
#undef od_wcsncmp
#undef od_wcsicmp
#undef od_wcsendwith
void OD_InitFuncs()
{
	g_od_funcs = &g_opdevfuncs;
	memset(g_od_funcs, 0, sizeof(struct OpDevFunc));
	g_od_funcs->__OD_BookmarkImportIsAvailable = OD_BookmarkImportIsAvailable;
	g_od_funcs->__OD_BookmarkImportEnumBegin = OD_BookmarkImportEnumBegin;
	g_od_funcs->__OD_BookmarkImportEnumEnd = OD_BookmarkImportEnumEnd;
	g_od_funcs->__OD_BookmarkImportGetNextDataSize = OD_BookmarkImportGetNextDataSize;
	g_od_funcs->__OD_BookmarkImportGetNext = OD_BookmarkImportGetNext;
	g_od_funcs->__OD_InitDebugger = OD_InitDebugger;
	g_od_funcs->__OD_DeinitDebugger = OD_DeinitDebugger;
	g_od_funcs->__OD_WriteLogFile = OD_WriteLogFile;
	g_od_funcs->__OD_Trace = OD_Trace;
	g_od_funcs->__OD_TraceWString = OD_TraceWString;
	g_od_funcs->__OD_PrintStackTrace = OD_PrintStackTrace;
	g_od_funcs->__OD_SetDataBreakPoint = OD_SetDataBreakPoint;
	g_od_funcs->__OD_Assert = OD_Assert;
	g_od_funcs->__OD_DASRequestSavePath = OD_DASRequestSavePath;
	g_od_funcs->__OD_DASReportProgress = OD_DASReportProgress;
	g_od_funcs->__OD_DASResponseCancelPrefetch = OD_DASResponseCancelPrefetch;
	g_od_funcs->__OD_DASAbortPrefetch = OD_DASAbortPrefetch;
	g_od_funcs->__OD_DASStopResponse = OD_DASStopResponse;
	g_od_funcs->__OD_DASShowDownloadList = OD_DASShowDownloadList;
	g_od_funcs->__OD_DASStartBrowser = OD_DASStartBrowser;
	g_od_funcs->__OD_DASExitBrowser = OD_DASExitBrowser;
	g_od_funcs->__OD_DASSendAuthenticationData = OD_DASSendAuthenticationData;
	g_od_funcs->__OD_DownloadIsMimeTypeSupport = OD_DownloadIsMimeTypeSupport;
	g_od_funcs->__OD_DownloadIsDDMimeType = OD_DownloadIsDDMimeType;
	g_od_funcs->__OD_DownloadHandleMIMEDownloaded = OD_DownloadHandleMIMEDownloaded;
	g_od_funcs->__OD_FileInit = OD_FileInit;
	g_od_funcs->__OD_FileDeinit = OD_FileDeinit;
	g_od_funcs->__OD_CountFileHandles = OD_CountFileHandles;
	g_od_funcs->__OD_FileOpen = OD_FileOpen;
	g_od_funcs->__OD_FileClose = OD_FileClose;
	g_od_funcs->__OD_FileCommit = OD_FileCommit;
	g_od_funcs->__OD_FileDelete = OD_FileDelete;
	g_od_funcs->__OD_FileExtend = OD_FileExtend;
	g_od_funcs->__OD_FileExist = OD_FileExist;
	g_od_funcs->__OD_FileGetAttributes = OD_FileGetAttributes;
	g_od_funcs->__OD_FileGetLength = OD_FileGetLength;
	g_od_funcs->__OD_FileGetPosition = OD_FileGetPosition;
	g_od_funcs->__OD_FileGetTimestampsFromHandle = OD_FileGetTimestampsFromHandle;
	g_od_funcs->__OD_FileGetTimestampsFromAttributes = OD_FileGetTimestampsFromAttributes;
	g_od_funcs->__OD_FileIsForwardable = OD_FileIsForwardable;
	g_od_funcs->__OD_FileRead = OD_FileRead;
	g_od_funcs->__OD_FileRename = OD_FileRename;
	g_od_funcs->__OD_FileSetAttributes = OD_FileSetAttributes;
	g_od_funcs->__OD_FileSetPosition = OD_FileSetPosition;
	g_od_funcs->__OD_FileTruncate = OD_FileTruncate;
	g_od_funcs->__OD_FileWrite = OD_FileWrite;
	g_od_funcs->__OD_FindFirst = OD_FindFirst;
	g_od_funcs->__OD_FindNext = OD_FindNext;
	g_od_funcs->__OD_FindClose = OD_FindClose;
	g_od_funcs->__OD_DirectoryCreate = OD_DirectoryCreate;
	g_od_funcs->__OD_DirectoryRemove = OD_DirectoryRemove;
	g_od_funcs->__OD_DirectoryGetSize = OD_DirectoryGetSize;
	g_od_funcs->__OD_DirectoryGetPredefined = OD_DirectoryGetPredefined;
	g_od_funcs->__OD_GetCardDriveLetter = OD_GetCardDriveLetter;
	g_od_funcs->__OD_GetPublicDriveLetter = OD_GetPublicDriveLetter;
	g_od_funcs->__OD_GetSystemDriveLetter = OD_GetSystemDriveLetter;
	g_od_funcs->__OD_GetStorageDriveLetter = OD_GetStorageDriveLetter;
	g_od_funcs->__OD_DriveGetAllVisibleLetters = OD_DriveGetAllVisibleLetters;
	g_od_funcs->__OD_DriveGetCurrentLetter = OD_DriveGetCurrentLetter;
	g_od_funcs->__OD_DriveGetSystemLetters = OD_DriveGetSystemLetters;
	g_od_funcs->__OD_GetPrestorageFolder = OD_GetPrestorageFolder;
	g_od_funcs->__OD_GetStorageFolder = OD_GetStorageFolder;
	g_od_funcs->__OD_DriveIsWritable = OD_DriveIsWritable;
	g_od_funcs->__OD_GetDriveFreeSpace = OD_GetDriveFreeSpace;
	g_od_funcs->__OD_PathsEqual = OD_PathsEqual;
	g_od_funcs->__OD_XDelete = OD_XDelete;
	g_od_funcs->__OD_FontInit = OD_FontInit;
	g_od_funcs->__OD_FontDeinit = OD_FontDeinit;
	g_od_funcs->__OD_GetFontMetrics = OD_GetFontMetrics;
	g_od_funcs->__OD_GetCharMetrics = OD_GetCharMetrics;
	g_od_funcs->__OD_GetTextWidth = OD_GetTextWidth;
	g_od_funcs->__OD_MeasureString = OD_MeasureString;
	g_od_funcs->__OD_DrawText = OD_DrawText;
	g_od_funcs->__OD_GetTextData = OD_GetTextData;
	g_od_funcs->__OD_GDIInit = OD_GDIInit;
	g_od_funcs->__OD_GDIDeinit = OD_GDIDeinit;
	g_od_funcs->__OD_GDIUseNativeMemory = OD_GDIUseNativeMemory;
	g_od_funcs->__OD_GDIAllocMemory = OD_GDIAllocMemory;
	g_od_funcs->__OD_GDIFreeMemory = OD_GDIFreeMemory;
	g_od_funcs->__OD_GDICreateLayer = OD_GDICreateLayer;
	g_od_funcs->__OD_GDIDestroyLayer = OD_GDIDestroyLayer;
	g_od_funcs->__OD_ShowOpera = OD_ShowOpera;
	g_od_funcs->__OD_HideOpera = OD_HideOpera;
	g_od_funcs->__OD_Paint = OD_Paint;
	g_od_funcs->__OD_GetScreenBuffer = OD_GetScreenBuffer;
	g_od_funcs->__OD_GetViewportArea = OD_GetViewportArea;
	g_od_funcs->__OD_GetLayerDimension = OD_GetLayerDimension;
	g_od_funcs->__OD_GetLayerPixelFormat = OD_GetLayerPixelFormat;
	g_od_funcs->__OD_GetLayerBPP = OD_GetLayerBPP;
	g_od_funcs->__OD_GetScreenDPI = OD_GetScreenDPI;
	g_od_funcs->__OD_GetRotationAngle = OD_GetRotationAngle;
	g_od_funcs->__OD_RotateScreen = OD_RotateScreen;
	g_od_funcs->__OD_GetScreenMode = OD_GetScreenMode;
	g_od_funcs->__OD_ToggleFullScreen = OD_ToggleFullScreen;
	g_od_funcs->__OD_EnterFullScreenEditor = OD_EnterFullScreenEditor;
	g_od_funcs->__OD_CloseFullScreenEditor = OD_CloseFullScreenEditor;
	g_od_funcs->__OD_ImeEnterScreen = OD_ImeEnterScreen;
	g_od_funcs->__OD_ImeExitScreen = OD_ImeExitScreen;
	g_od_funcs->__OD_ImeConnect = OD_ImeConnect;
	g_od_funcs->__OD_ImeDisconnect = OD_ImeDisconnect;
	g_od_funcs->__OD_ImeBeforeEditorOrCaretChanged = OD_ImeBeforeEditorOrCaretChanged;
	g_od_funcs->__OD_ImeGetImuiHeight = OD_ImeGetImuiHeight;
	g_od_funcs->__OD_DrawImePart = OD_DrawImePart;
	g_od_funcs->__OD_ImeSetSoftKeyLabel = OD_ImeSetSoftKeyLabel;
	g_od_funcs->__OD_TranslateKeyCode = OD_TranslateKeyCode;
	g_od_funcs->__OD_TranslatePenEvent = OD_TranslatePenEvent;
	g_od_funcs->__KeyPenTracker_init = KeyPenTracker_init;
	g_od_funcs->__KeyPenTracker_destroy = KeyPenTracker_destroy;
	g_od_funcs->__KeyPenTracker_resetStatus = KeyPenTracker_resetStatus;
	g_od_funcs->__KeyPenTracker_handleEvent = KeyPenTracker_handleEvent;
	g_od_funcs->__OD_ExitUI = OD_ExitUI;
	g_od_funcs->__OD_InitApp = OD_InitApp;
	g_od_funcs->__OD_DeinitApp = OD_DeinitApp;
	g_od_funcs->__OD_MemoryCreatePool = OD_MemoryCreatePool;
	g_od_funcs->__OD_MemoryDestroyPool = OD_MemoryDestroyPool;
	g_od_funcs->__OD_MemoryGetPoolSize = OD_MemoryGetPoolSize;
	g_od_funcs->__OD_MemoryGetPoolBase = OD_MemoryGetPoolBase;
	g_od_funcs->__OD_MemoryBlocksInit = OD_MemoryBlocksInit;
	g_od_funcs->__OD_MemoryBlocksDeinit = OD_MemoryBlocksDeinit;
	g_od_funcs->__OD_Malloc = OD_Malloc;
	g_od_funcs->__OD_Free = OD_Free;
	g_od_funcs->__OD_NetGetID = OD_NetGetID;
	g_od_funcs->__OD_NetGetProxy = OD_NetGetProxy;
	g_od_funcs->__OD_NetInit = OD_NetInit;
	g_od_funcs->__OD_NetDeinit = OD_NetDeinit;
	g_od_funcs->__OD_NetActivateBearer = OD_NetActivateBearer;
	g_od_funcs->__OD_NetDeactivateBearer = OD_NetDeactivateBearer;
	g_od_funcs->__OD_NetGetActiveSimSlots = OD_NetGetActiveSimSlots;
	g_od_funcs->__OD_NetSelectSim = OD_NetSelectSim;
	g_od_funcs->__OD_NetSelectApn = OD_NetSelectApn;
	g_od_funcs->__OD_BearerIsWifi = OD_BearerIsWifi;
	g_od_funcs->__OD_NetCloseWin = OD_NetCloseWin;
	g_od_funcs->__OD_MobileResInit = OD_MobileResInit;
	g_od_funcs->__OD_MiniResInit = OD_MiniResInit;
	g_od_funcs->__OD_GetBreamStringCount = OD_GetBreamStringCount;
	g_od_funcs->__OD_GetBreamString = OD_GetBreamString;
	g_od_funcs->__OD_GetOperaString = OD_GetOperaString;
	g_od_funcs->__OD_GetStaticFiles = OD_GetStaticFiles;
	g_od_funcs->__OD_GetDeployFiles = OD_GetDeployFiles;
	g_od_funcs->__OD_SchemeInit = OD_SchemeInit;
	g_od_funcs->__OD_SchemeDeinit = OD_SchemeDeinit;
	g_od_funcs->__OD_SchemeHandle = OD_SchemeHandle;
	g_od_funcs->__OD_HostResolve = OD_HostResolve;
	g_od_funcs->__OD_SocketCreate = OD_SocketCreate;
	g_od_funcs->__OD_SocketConnect = OD_SocketConnect;
	g_od_funcs->__OD_SocketClose = OD_SocketClose;
	g_od_funcs->__OD_SocketSend = OD_SocketSend;
	g_od_funcs->__OD_SocketReceive = OD_SocketReceive;
	g_od_funcs->__od_inet_aton = od_inet_aton;
	g_od_funcs->__od_inet_ntoa = od_inet_ntoa;
	g_od_funcs->__OD_SystemInit = OD_SystemInit;
	g_od_funcs->__OD_SystemDeinit = OD_SystemDeinit;
	g_od_funcs->__OD_ClipboardSetData = OD_ClipboardSetData;
	g_od_funcs->__OD_ClipboardGetData = OD_ClipboardGetData;
	g_od_funcs->__OD_ClipboardHasData = OD_ClipboardHasData;
	g_od_funcs->__OD_GetTickCount = OD_GetTickCount;
	g_od_funcs->__OD_GetUTCTime = OD_GetUTCTime;
	g_od_funcs->__OD_GetLocalTime = OD_GetLocalTime;
	g_od_funcs->__OD_GetTimeZone = OD_GetTimeZone;
	g_od_funcs->__OD_GetSystemProperty = OD_GetSystemProperty;
	g_od_funcs->__OD_GetLangCountryCode = OD_GetLangCountryCode;
	g_od_funcs->__OD_GetPlatformName = OD_GetPlatformName;
	g_od_funcs->__OD_GetIMEI = OD_GetIMEI;
	g_od_funcs->__OD_GetIMSI = OD_GetIMSI;
	g_od_funcs->__OD_TimerCreate = OD_TimerCreate;
	g_od_funcs->__OD_TimerDestroy = OD_TimerDestroy;
	g_od_funcs->__OD_TimerStart = OD_TimerStart;
	g_od_funcs->__OD_TimerStop = OD_TimerStop;
	g_od_funcs->__OD_IsTimerStarted = OD_IsTimerStarted;
	g_od_funcs->__OD_PlatformOpenDocument = OD_PlatformOpenDocument;
	g_od_funcs->__OD_PlatformCanOpenDocument = OD_PlatformCanOpenDocument;
	g_od_funcs->__OD_GetNativeString = OD_GetNativeString;
	g_od_funcs->__OD_AtomicInit = OD_AtomicInit;
	g_od_funcs->__OD_AtomicDestroy = OD_AtomicDestroy;
	g_od_funcs->__OD_AtomicIncrement = OD_AtomicIncrement;
	g_od_funcs->__OD_AtomicDecrement = OD_AtomicDecrement;
	g_od_funcs->__OD_CreateMutex = OD_CreateMutex;
	g_od_funcs->__OD_ReleaseMutex = OD_ReleaseMutex;
	g_od_funcs->__OD_EnterMutex = OD_EnterMutex;
	g_od_funcs->__OD_LeaveMutex = OD_LeaveMutex;
	g_od_funcs->__OD_CreateSemaphore = OD_CreateSemaphore;
	g_od_funcs->__OD_ReleaseSemaphore = OD_ReleaseSemaphore;
	g_od_funcs->__OD_TakeSemaphore = OD_TakeSemaphore;
	g_od_funcs->__OD_GiveSemaphore = OD_GiveSemaphore;
	g_od_funcs->__OD_GetCurrentTaskID = OD_GetCurrentTaskID;
	g_od_funcs->__OD_GetMMITaskID = OD_GetMMITaskID;
	g_od_funcs->__OD_GetGogiTaskID = OD_GetGogiTaskID;
	g_od_funcs->__OD_InitMMITaskContext = OD_InitMMITaskContext;
	g_od_funcs->__OD_InitGogiTaskContext = OD_InitGogiTaskContext;
	g_od_funcs->__OD_CreateGogiTask = OD_CreateGogiTask;
	g_od_funcs->__OD_DestroyGogiTask = OD_DestroyGogiTask;
	g_od_funcs->__OD_PostCall = OD_PostCall;
	g_od_funcs->__OD_Yield = OD_Yield;
	g_od_funcs->__od_stricmp = od_stricmp;
	g_od_funcs->__od_strnicmp = od_strnicmp;
	g_od_funcs->__od_wcslen = od_wcslen;
	g_od_funcs->__od_wcsncpy = od_wcsncpy;
	g_od_funcs->__od_wcsncat = od_wcsncat;
	g_od_funcs->__od_wcschr = od_wcschr;
	g_od_funcs->__od_wcsrchr = od_wcsrchr;
	g_od_funcs->__od_wcsstr = od_wcsstr;
	g_od_funcs->__od_wcscmp = od_wcscmp;
	g_od_funcs->__od_wcsncmp = od_wcsncmp;
	g_od_funcs->__od_wcsicmp = od_wcsicmp;
	g_od_funcs->__od_wcsendwith = od_wcsendwith;
}
