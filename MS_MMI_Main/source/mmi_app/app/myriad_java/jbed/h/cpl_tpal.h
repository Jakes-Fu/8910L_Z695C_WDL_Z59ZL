
#ifndef __NATIVE_AMS_H__
#define __NATIVE_AMS_H__

#define JBEDAMS_STORAGE_NAME (0xFFEFFFEF)

typedef struct _AmsEvent
{
    int event;
    int result;
    int iData;
    void* pData;
    struct _AmsEvent * next;
} AmsEvent;

typedef struct _MMIEvent
{
    int    event;
    int    result;
    int    data_bytes;
    int    event_id;
    void * data;
    struct _MMIEvent * next;
} MMIEvent;

typedef struct _permStru
{
    unsigned short list_index;
    int curEvtID;
}PERMStru;

#if 0
#define MAX_UCS2_STR_SIZE (128)
typedef struct _MultiUCS2String
{
    int  length;
    unsigned short ucs2Str[MAX_UCS2_STR_SIZE];
}MultiUCS2String;
#endif

/* java midlet's permission body structure. */
typedef struct _NAMS_Permissions_Body
{
    unsigned char    allowed;
    unsigned char    current;
    unsigned char    changeable;
    unsigned short * permission_fg_name;
} NAMSPermissionsBody_t;

/* permission store , make up with permission bodys. */
typedef struct _NAMS_Permissions_T
{
    int    permissions_num;
    NAMSPermissionsBody_t * permssions_body;
} NAMSPermssions_t;

/* native ams insallation query. The value refer to nativeAms.java */
typedef enum _InstallQuery
{
    IQ_ASK_USER = 1,
    IQ_INFORM_USER = 2,
    IQ_RETAIN_RMS = 3,
    IQ_OLD_VERSION = 17,
    IQ_NEW_VERSION = 32,
    IQ_ALREADY_INSTALLED = 39,
    IQ_JAD_MOVED = 34,
    IQ_UNAUTHORIZED = 33,
    IQ_INSTALL_CONFIRM = 44,
    IQ_MIDLET_UNSIGNED = 87,
    IQ_MIDLET_SIGNED = 88,
} InstallQuery_e;

/* native ams result. The value refer to nativeAms.java */
typedef enum _NAMS_RES
{
    RES_NONE = 0,
    RES_OK = 1,
    RES_JAVA_HANDLE = 2,
    RES_NO_INSTALL_RUNNING = 3,
    RES_TOO_LATE_FOR_CANCEL = 4,
    RES_CONTINUE = 5,
    RES_CANCEL = 6,
    RES_JAD_URL = 7,
    RES_NOT_FOUND = 8,
    RES_DOWNLOADING = 9,
    RES_INSTALLING = 10,
    RES_RUNNING = 11,
    RES_NOT_RUNNING = 12,
    RES_NO_ICON = 13,
    RES_NOT_SET = 14,
    RES_ERROR = 15,
    RES_NOT_ENOUGH_SPACE = 16,
    RES_COPY_ERROR = 17,
    RES_CANT_CREATE_DIRECTORY = 18,
    RES_NO_STORAGES = 19,
    RES_NO_RUNNING_MIDLETS = 20,
    RES_RUNNING_MIDLETS = 21,
    RES_CANCELLING = 22,
    RES_SIZE_MISMATCH = 23,
    RES_DRM_INSTALL = 24,
    RES_YES = 25,
    RES_NO = 26,
    RES_DRM_ERROR = 27,
    RES_NOT_COMPLETELY_REMOVED = 28,
} NAMS_RES_e;

/* native ams events result! The value refer to invalidJadException.java*/
typedef enum _MMINAMS_EVENT_RESULT
{
    RESULT_NO_ERROR = 0,
    RESULT_COMPILATION_INSUFFICIENT_MEMORY = 66,
    RESULT_USER_CANCELLED = 74,
    RESULT_DOWNLOADING_INSUFFICIENT_STORAGE = 100
}MMINAMS_EVENT_RESULT_e;

/** Permission values used for PERMISSIONS and PERMISSIONS_ANSWERS */
typedef enum _NAMS_PERMISSION_ANSWER
{
    PERM_ALWAYS = 1,
    PERM_NEVER = 2,
    PERM_ASK = 4,
    PERM_SESSION = 8,
    PERM_ONESHOT = 16,
}NAMS_PERMISSION_ANSWER_e;

/** flag values used by EVENT_PERMISSIONS_ANSWERS */
typedef enum _NAMS_PERMISSION_ANSWER_FLAG
{
    EPA_CHECK = 1,
    EPA_SET = 2,
}NAMS_PERMISSION_ANSWER_FLAG_e;

#ifdef SUPPORT_CAMERA_CAPTURE
typedef enum _MMINAMS_SNAPSHOT_OPERATION_TYPE
{
    START_PREVIEW = 1,
    TAKE_SNAPSHOT,
    CLOSE_PREVIEW
#ifdef SUPPORT_BACKGROUND_RUN
    ,POPUP_REDKEYDIALOG
#endif
}MMINAMS_SNAPSHOT_OPERATION_TYPE;

typedef struct _MMINAMS_SNAPSHOT_OPT_PARAM
{
    MMINAMS_SNAPSHOT_OPERATION_TYPE optType;
    void * notifier;              /* The asynchronous I/O notifier. */
    void * userData;              /* The user define data. */
}MMINAMS_SNAPSHOT_OPT_PARAM;
#endif

/* native ams events code! The value refer to nativeAms.java*/
typedef enum _MMINamsEvent
{
    MMI_NAMS_EVENT_NONE = 0,
    MMI_NAMS_EVENT_LIST = 1,
    MMI_NAMS_EVENT_RUN = 2,
    MMI_NAMS_EVENT_UPDATE = 3,
    MMI_NAMS_EVENT_INSTALL_PROGRESS = 4,
    MMI_NAMS_EVENT_INSTALL = 5,
    MMI_NAMS_EVENT_INSTALL_CANCEL = 6,
    MMI_NAMS_EVENT_REMOVE = 7,
    MMI_NAMS_EVENT_PAUSE = 8,
    MMI_NAMS_EVENT_RESUME = 9,
    MMI_NAMS_EVENT_DESTROY = 10,
    MMI_NAMS_EVENT_DESTROYED = 11,
    MMI_NAMS_EVENT_INSTALL_STATUS = 12,
    MMI_NAMS_EVENT_INSTALL_QUERY = 13,
    MMI_NAMS_EVENT_INSTALL_UNSTOPPABLE = 14,
    MMI_NAMS_EVENT_INSTALL_PREVIEW_JAD = 15,
    MMI_NAMS_EVENT_INSTALL_AUTHENTICATION = 16,
    MMI_NAMS_EVENT_PAUSED = 17,
    MMI_NAMS_EVENT_RUNNING = 18,
    MMI_NAMS_EVENT_INSTALL_STEP = 19,
    MMI_NAMS_EVENT_INFO = 20,
    MMI_NAMS_EVENT_RENAME = 21,
    MMI_NAMS_EVENT_GET_ICONS = 22,
    MMI_NAMS_EVENT_PERMISSIONS = 23,
    MMI_NAMS_EVENT_PERMISSIONS_ANSWERS = 24,
    MMI_NAMS_EVENT_QUERY = 25,
    MMI_NAMS_EVENT_REMOVE_ALL = 26,
    MMI_NAMS_EVENT_SET = 27,
    MMI_NAMS_EVENT_INSTALL_STEP_COMPLETE = 28,
    MMI_NAMS_EVENT_SELECT_STORAGE = 29,
    MMI_NAMS_EVENT_REMOVABLE_MEDIA_CHANGED = 30,
    MMI_NAMS_EVENT_MOVE = 31,
    MMI_NAMS_EVENT_LIST_STORAGE = 32,
    MMI_NAMS_EVENT_REQUEST_INSTALL = 33,
    MMI_NAMS_EVENT_LIST_RUNNING_MIDLETS = 34,
    MMI_NAMS_EVENT_JAR_DOWNLOAD_PROGRESS = 35,
    MMI_NAMS_EVENT_JAR_DOWNLOAD = 36,
    MMI_NAMS_EVENT_JAR_DOWNLOAD_COMPLETE = 37,
    MMI_NAMS_EVENT_DRM_ACTIVATE = 38,
    MMI_NAMS_EVENT_DRM_REINSTALL = 39,
    MMI_NAMS_EVENT_RUN_DEBUG = 40,
    MMI_NAMS_EVENT_PERMISSIONS_RESET_ANSWERS = 41,
    MMI_NAMS_EVENT_PLATFORM_RESET = 42,
    MMI_NAMS_EVENT_REQUEST_PUSH = 43,
    MMI_NAMS_EVENT_REQUEST_SATSA = 44,
    MMI_NAMS_EVENT_REQUEST_CHAPI = 45,
    MMI_NAMS_EVENT_PREINSTALL_START = 46,
    MMI_NAMS_EVENT_PREINSTALL_FINISH = 47,
    MMI_NAMS_EVENT_RUNNING_MIDLET_MANAGER = 48,
    MMI_NAMS_EVENT_NOTIFY_LAUNCH = 49,
    MMI_NAMS_EVENT_SANITY_REPORT = 50,
    MMI_NAMS_EVENT_REQUEST_PERMISSION = 51,
    MMI_NAMS_EVENT_LIST_CERTIFICATES = 52,
    MMI_NAMS_EVENT_CHANGE_CERTIFICATE_STATE = 53,
    MMI_NAMS_EVENT_JAD_DOWNLOAD_PROGRESS = 54,
    MMI_NAMS_EVENT_JAD_DOWNLOAD = 55,
    MMI_NAMS_EVENT_JAD_DOWNLOAD_COMPLETE = 56,
    MMI_NAMS_EVENT_INSTALL_QUERY_YES_NO = 57,
    MMI_NAMS_EVENT_RUNTIME_ERROR = 58,
    MMI_NAMS_EVENT_LIST_FOLDER_CONTENTS = 59,
    MMI_NAMS_EVENT_CREATE_FOLDER = 60,
    MMI_NAMS_EVENT_MOVE_FOLDER = 61,
    MMI_NAMS_EVENT_RENAME_FOLDER = 62,
    MMI_NAMS_EVENT_REMOVE_FOLDER = 63,
    MMI_NAMS_EVENT_REMOVE_FOLDER_CONTENTS = 64,
    MMI_NAMS_EVENT_SELECT_INSTALL_FOLDER = 65,
    MMI_NAMS_EVENT_DRM_CHECK_RIGHTS = 66,
    MMI_NAMS_EVENT_DRM_CONSTRAINT = 67,
    MMI_NAMS_EVENT_SHOW_JAR_DOWNLOAD_SUCCESS = 69,
    MMI_NAMS_EVENT_COMMAND_INSTALL = 70,
    MMI_NAMS_EVENT_COMMAND_INSTALL_STATUS = 71,
    MMI_NAMS_EVENT_COMMAND_INSTALL_PROGRESS = 72,
    MMI_NAMS_EVENT_START_SCHEDULE = 98,
    MMI_NAMS_EVENT_START_OS_TIMER = 99,
    MMI_NAMS_EVENT_RESUME_SUSPEND_PLAYER = 100,
    MMI_NAMS_EVENT_PDP_ACTIVE = 101,
    MMI_NAMS_EVENT_PDP_DEACTIVE = 102,
    MMI_NAMS_EVENT_START_PUSH_TIMER = 103,
    MMI_NAMS_EVENT_JBED_THREAD_EXIT = 105,
    MMI_NAMS_EVENT_NOTIFY_UDISK_JBED_ISRUN = 106,
    MMI_NAMS_EVENT_REQUEST_BROWSER_FROM_MMI = 107,
    MMI_NAMS_EVENT_REQUEST_CALL_FROM_MMI = 108,
    MMI_NAMS_EVENT_CREATE_TEXTEDIT_WIN_FROM_MMI = 109,
    MMI_NAMS_EVENT_MEDIA_SNAPSHOT = 110,
    MMI_NAMS_EVENT_START_LCDBACKLIGHT = 111,
    MMI_NAMS_EVENT_STOP_LCDBACKLIGHT  = 112,
    MMI_NAMS_EVENT_CLOSE_TEXTEDIT_WIN_FROM_MMI = 113,
    MMI_NAMS_EVENT_CHECK_AND_FORCE_CLOSE_TEXTEDIT_WIN_FROM_MMI = 114,
#ifdef MICROSOFT_EXTRA_FUNCTION
    MMI_NAMS_EVENT_CREATE_THUMBNAIL = 115,
#endif // MICROSOFT_EXTRA_FUNCTION
}MMInAmsEvent_e;

typedef enum _InstallStatus
{
    IS_INSTALLING = 0,
    IS_COMPILING = 266,
    IS_OPTIMIZING_JAR_FILE = 267,
    IS_GETTING_JAR = 268,
    IS_MAX
}InstallStatus_e;

typedef enum _SuiteItemID
{
    NAMS_SI_NAME,
    NAMS_SI_ROOT,
#ifdef AMS_FOLDERS
    NAMS_SI_FOLDER,
#endif
    NAMS_SI_VENDOR,
    NAMS_SI_VERSION,
    NAMS_SI_DOMAIN,
    NAMS_SI_FILE_ROOT,
    NAMS_SI_REMOVABLE,
    NAMS_SI_JAD_URL,
    NAMS_SI_JAR_URL,
    NAMS_SI_ICON,
    NAMS_SI_MAX
}SuiteItemID;

typedef enum _MIDletItemID
{
    NAMS_MI_MIDLET,
    NAMS_MI_CLASS,
    NAMS_MI_ICON,
    NAMS_MI_MAX
}MIDletItemID;

typedef void (*PNAMSNOTIFY)(MMIEvent * aEvent);

/**
 * Native AMS intialize.
 * params: is_install
 *         TRUE  - call the func when start Install,set is_install as TRUE
 *         FALSE - set is_install as else FALSE
 */
void Jbed_nAms_intialize(int is_install);

/**
 * Native AMS finalize.
 */
void Jbed_nAms_finalize();


/**
 * regiter a Native AMS callback function to handle relevent
 * event with MMI side. Please note, this API should be available
 * during whole native ams life cycle.
 *
 * @ param pNotify
 *     \ The callback API which will be called.
 */
void Jbed_nAms_registerNotify(PNAMSNOTIFY pNotify);

/**
 * Install a MIDlet by jad/jar url path. The path can be a
 * local file url or a http/https network url.
 *
 * @ param appPath
 *     \ the path of the MIDlet needs to be installed
 * @ param pathLen
 *     \ the character length of appPath.
 * @ param isOtaInstall
 *     \ output parameter, true means begin an ota installation, otherwise local installation.
 * @ return true means success, otherwise failure.
 */
int Jbed_nAms_installBegin(unsigned short * appPath, int pathLen, BOOLEAN *isOtaInstall);


/**
 * For any Installing query events, after user confirm "YES",
 * this API can be used to continue installation.
 *
 * @param event
 *      \ current continue event.
 * @param operation
 *      \ the install operation, continue, cancel...
 * @param extraData
 *      \ the extra data of installation.
 */
void Jbed_nAms_installOperation(int event, int operation, void * extraData);

/**
 * Launch a MIDlet with given suite id and midlet number.
 * if a MIDlet has already been running at background, this API
 * can brew the MIDlet to foreground.
 *
 * @ param suiteId
 *     \ the MIDlet suiteId.
 * @ param midletNum
 *     \ the midletNum in the suiteId.
 */
void Jbed_nAms_launch(int suiteId, int midletNum);

/**
 * This API destroy a running MIDlet with fixed suite id and
 * MIDlet number.
 *
 * @ param suiteId
 *     \ the MIDlet suiteId.
 * @ param midletNum
 *     \ the midletNum in the suiteId.
 */
void Jbed_nAms_destroy(int suiteId, int midletNum);


/**
 * This API delete an installed MIDlet. If a MIDlet is in running
 * state, this API will do nothing.
 *
 * @ param suiteId
 *     \ the MIDlet suiteId.
 */
void Jbed_nAms_delete(int suiteId);


/**
 * If suiteId equals NULL, means get the all installed MIDlet suites.
 * otherwise, this API returns the number of MIDlets belongs to the
 * fixed suite.
 *
 * @ param suiteId
        \ the suite ID.
 * @ return the MIDlets count by different param.
 */
int Jbed_nAms_getSuiteMIDletCount(char * suiteId);


/**
 * Get Suites count.
 * If SuiteIdList is NULL, only calcute the MIDlet suite number.
 *
 * @ param suiteIdList
 *      \ the pass out value which records all installed MIDlet
 *        suite ID. An exists int array should be required.
 * @ param length
 *      \ the size the suiteIdList which passed in.
 * @ return
 *      \ the total number of the installed MIDlet suite ID. If
 *        the sutiteIdList size is less than the real number of
 *        the suite Ids, the *length* size array will be returned.
 */
int Jbed_nAms_getSuitesCount(int * suiteIdList, int length);


/**
 * Get the totally number of MIDlets which in the fixed suite.
 *
 * @param suiteId
 *      \ the suite Id.
 * @ return
 *      \ the number of MIDlets in one suite.
 */
int Jbed_nAms_getMIDletsCount(int suiteId);


/**
 * Get the relevant suite information by the index value and fixed item ID.
 *
 * @param suiteId
 *      \ suite ID.
 * @param item ID
 *      \ refer to enum SuiteItemID.
 * @param buf
 *      \ the pass out value which corresponding to the fixed item id.
 * @return
 *      \ return the length of the actually item size.
 */
int Jbed_nAms_querySuitesInfo(int suiteId, int itemId, void ** buf);

/**
 * Get the relevant MIDlet information by the index value, fixed item ID and given suite ID.
 *
 * @param suiteId
 *      \ suite ID
 * @param MIDletIdx
 *      \ midlet index which are less than the midlet totally count.
 * @param item ID
 *      \ refer to enum SMIDlet.
 * @param buf
 *      \ the pass out value which corresponding to the fixed item id.
 * @return
 *      \ return the length of the actually item size.
 */
int Jbed_nAms_queryMIDletInfoByIdx(int suiteId, int MIDletIdx, int itemId, void ** buf);


/**
 * Get the installed MIDlet information by fixed suite id;
 *
 * @param suiteId
 *      \ suite ID;
 */
void Jbed_nAms_queryDetail(int suiteId);

/**
 * Update the installed MIDlet by fixed suite id;
 *
 * @param suiteId
 *      \ suite ID;
 */
void Jbed_nAms_update(int suiteId);


/**
 * Delete the installed MIDlet by fixed suite id;
 *
 * @param suiteId
 *      \ suite ID;
 */
void Jbed_nAms_delete(int suiteId);

/**
 * Delete all installed MIDlets.
 */
void Jbed_nAms_deleteAll(void);


/**
 * Since some operations may cause the MIDlet information link list changed,
 * such as, install, remove, remove all, this API is used to update the list
 * which are cached in memory.
 */
void Jbed_nAms_updateMIDletInfo(void);


/**
 * Query the installed MIDlet permission information.
 *
 * @param suiteId
 *      \ the MIDlet suite Id.
 */
void Jbed_nAms_queryMIDletPermissions(int suiteId);


/**
 * Set the installed MIDlet permission answer;
 *
 * @param fgId
 *      \ function group id;
 * @param permission
 *      \ the permssion answer;
 * @param flag
 *      \ the current operation flag;
 */
void Jbed_nAms_setMIDletPermissions(int fgId, int permission, int flag);

/**
 * Pause the specified midlet.
 *
 * @param suiteId
 *      \ the MIDlet suite Id.
 * @param midletNum
 *      \ the MIDlet number.
 */
void Jbed_nAms_Pause(int suiteId, int midletNum);

/**
 * Set flag to let midlet not paused in background
 * Useful when nopauseinbackground() is configed in ams
 *
 * @param suiteId
 *      \ the MIDlet suite Id.
 * @param midletNum
 *      \ the MIDlet number.
 */
void Jbed_nAms_noPauseInBackground(int suiteId, int midletNum);


/* Use this function when you need to handle native ams request first.
 * Use Jbed_vmWantForeground()
 * directly when you need to change vm state at once.
 *
 * @param forground bool_t
 *      \ TRUE to send vm to forground
 *      \ FALSE to background
 */
#define STATE_IDLE         1
#define STATE_ACTIVE       2
#define STATE_FOREGROUND   3
/*if jbed state is been in idle,you cannot use the api,use Jbed_vmWantForeground deriectly*/
void Jbed_nAms_requestVmToState(int state);

/**
 * Handle out memory error.
 *
 * @param prompt
 *      \ the prompt string.
 * @param length
 *      \ the prompt string length.
 */
void Jbed_nAms_handleOutOfMemoryError(unsigned short * prompt, int length);

#ifdef SUPPORT_CAMERA_CAPTURE
/**
 * Request the snapshot operation.
 *
 * @param reqOptType
 *      \ the operation type.
 * @param reqNotifier
 *      \ the The asynchronous I/O notifier.
 * @param reqUserData
 *      \ The user define data.
*/
void CPL_media_requestSnapshotOperation(
                               MMINAMS_SNAPSHOT_OPERATION_TYPE reqOptType,
                               void *reqNotifier,
                               void *reqUserData);

/**
 * Excute the snapshot operation.
 *
 * @param optParam
 *      \ the MMINAMS_SNAPSHOT_OPT_PARAM tpye of operation parameter.
 */
void CPL_media_doSnapshotOperation(MMINAMS_SNAPSHOT_OPT_PARAM optParam);
#endif

void Jbed_nAms_notifyResumeSuspendPlayer(int playerHandle);

void Jbed_nAms_notifyJbedThreadExit(void);

void Jbed_nAms_finalizeWithMemoryNotEnough(void);

/*send msg to mmi to request native browser*/
enum
{
    PARAM_INIT_ALL = 1,
    PARAM_SET_FLAG,
    PARAM_GET,
};
void MMIAPIJAVA_NotifyRequestBrowserFromMMI(void);

/*send msg to mmi to make call*/
void MMIAPIJAVA_NotifyRequestCallFromMMI(int suite_id,char * url);

void MMIAPIJAVA_NotifyCreateTextEditWinFromMMI(void);
void MMIAPIJAVA_NotifyCloseTextEditWinFromMMI(void);
void MMIAPIJAVA_NotifyCheckAndForceCloseTextEditWinFromMMI(void);

#ifdef MICROSOFT_EXTRA_FUNCTION
void MMIAPIJAVA_NotifyCreateThumbnailFromMMI(int tag);
#endif

/* Get suite id from storage name.  */
int Jbed_nAms_getSuiteIdFromStorageName(unsigned short * data, int len);

/**
 * Check whether current suite ID is valid or not.
 * In most case, it used while SD Card is unplugged.
 * @param sid, suite id
 * @return TRUE means available, otherwise non-available;
 */
BOOLEAN Jbed_nAms_isMIDletSuiteAvailable(int32 sid);

/**
 * Get available suiteIDs.
 * If ids is NULL or idsListCount is 0, only return the available ids count;
 * Otherwise, Suite Id will be stored in param ids.
 * @param ids, the buffer used to store available Suite Id.
 * @param idsListCount, the size of buffer ids;
 * @return the available Suite Id count;
 */
int32 Jbed_nAms_getSDCardAvailableSuites(int32 *ids, int32 idsListCount);

#ifdef MICROSOFT_WNS_SUPPORT

/* FIXME: this is pre-defined size, still needs to confirm with Microsoft */
#define MAX_WNS_IDENTIFIER_LEN (128)

/**
 * Get suite id by WNS push message identifier which is specified in JAD
 * or MANIFEST file with property: MIDlet-WNS-AppName;
 * For better comparion, this property is stored in selector data with property
 * key "wns_identifier=". When platform side WNS engine received an WNS push
 * message for Java, it will launch corresponding application which is identified
 * by this property value.
 *
 * @param identifier, the identifer of WNS applicaions.
 * @return the suite Id;
 */
int Jbed_nAms_getSuiteIdByWnsIdentifier(char *identifier);

/**
 * Get icon by WNS push message identifier which is specified in JAD
 * or MANIFEST file with property: MIDlet-WNS-AppName;
 * Usage: You probably need to call the function twice:
 * The first time, you specify the parameter of buffer as NULL, the function will
 * return the size of icon buffer which you need to malloc. After you mallocated
 * the buffer passed as the parameter and call Jbed_nAms_getIconByWnsIdentifier
 * again, the icon data will be filled in this buffer. You are also responsible
 * for freeing the buffer.
 *
 * @param identifier, the identifer of WNS applicaions.
 * @param buffer, the buffer is used for filling icon data.
*  @param bufLen, the length of buffer, it is of no effect when buffer is passed as NULL.
 * @return the icon buffer size; return -1 means no responding icon or getting icon failed.
 */
int Jbed_nAms_getIconByWnsIdentifier(char *identifier, uint8 *buffer, int bufLen);

/**
 * If MIDlet Suite is launched by WNS push message, call this API to set push environment
 * which will be used by application on run-time; This API will trigger a property
 * "launched.by.wns.push.message = true", application can get this property to know whether
 * it was launched by WNS push or not.
 */
void Jbed_nAms_setLaunchByWnsPushEnvironment();


/**
 * This is an VM internal API, MMI or other modules cannot call this API directly,
 * otherwise may cause un-expected result. If MMI calls Jbed_nAms_setLaunchByWnsPushEnvironment(),
 * this API will return TRUE to indicate current run is lauched by WNS push message.
 */
BOOLEAN Jbed_nAms_isMIDletSuiteLaunchedByWnsPush();

#endif// MICROSOFT_WNS_SUPPORT

#endif
