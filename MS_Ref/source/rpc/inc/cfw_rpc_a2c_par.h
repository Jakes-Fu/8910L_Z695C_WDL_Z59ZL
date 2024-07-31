// AUTO GENERATED

#define TAG_CFW_IsFreeUTI 0xac2b1b66
struct IPAR_CFW_IsFreeUTI
{
    uint16_t nUTI;
    uint32_t nServiceId;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_IsFreeUTI));

struct OPAR_CFW_IsFreeUTI
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_IsFreeUTI));

#define TAG_CFW_GetFreeUTI 0x8a0c270b
struct IPAR_CFW_GetFreeUTI
{
    uint32_t nServiceId;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_GetFreeUTI));

struct OPAR_CFW_GetFreeUTI
{
    uint32_t result;
    uint8_t nUTI;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_GetFreeUTI));

#define TAG_CFW_ShellControl 0xe4a00223
struct IPAR_CFW_ShellControl
{
    uint8_t nCommand;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_ShellControl));

struct OPAR_CFW_ShellControl
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_ShellControl));

#define TAG_CFW_CfgSetPhoneFunctionality 0xb4319259
struct IPAR_CFW_CfgSetPhoneFunctionality
{
    uint8_t nLevel;
    uint8_t nRst;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_CfgSetPhoneFunctionality));

struct OPAR_CFW_CfgSetPhoneFunctionality
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_CfgSetPhoneFunctionality));

#define TAG_CFW_CfgGetPhoneFunctionality 0xa046cd5e
struct OPAR_CFW_CfgGetPhoneFunctionality
{
    uint32_t result;
    uint8_t nLevel;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_CfgGetPhoneFunctionality));

#define TAG_CFW_CfgStoreUserProfile 0xa40eca33
struct OPAR_CFW_CfgStoreUserProfile
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_CfgStoreUserProfile));

#define TAG_CFW_CfgGetUserProfile 0xa53c084f
struct OPAR_CFW_CfgGetUserProfile
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_CfgGetUserProfile));

#define TAG_CFW_CfgRestoreFactoryProfile 0x074a8a28
struct OPAR_CFW_CfgRestoreFactoryProfile
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_CfgRestoreFactoryProfile));

#define TAG_CFW_CfgGetCharacterSet 0x69bee605
struct OPAR_CFW_CfgGetCharacterSet
{
    uint32_t result;
    uint8_t chset;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_CfgGetCharacterSet));

#define TAG_CFW_CfgSetCharacterSet 0x991e05de
struct IPAR_CFW_CfgSetCharacterSet
{
    uint8_t chset;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_CfgSetCharacterSet));

struct OPAR_CFW_CfgSetCharacterSet
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_CfgSetCharacterSet));

#define TAG_CFW_CfgSetIndicatorEvent 0xcbe35db9
struct IPAR_CFW_CfgSetIndicatorEvent
{
    uint16_t ind;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_CfgSetIndicatorEvent));

struct OPAR_CFW_CfgSetIndicatorEvent
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_CfgSetIndicatorEvent));

#define TAG_CFW_CfgGetIndicatorEvent 0x103eecc1
struct IPAR_CFW_CfgGetIndicatorEvent
{
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_CfgGetIndicatorEvent));

struct OPAR_CFW_CfgGetIndicatorEvent
{
    uint32_t result;
    uint16_t ind;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_CfgGetIndicatorEvent));

#define TAG_CFW_SetQualReport 0xd7c9260b
struct IPAR_CFW_SetQualReport
{
    bool bStart;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_SetQualReport));

struct OPAR_CFW_SetQualReport
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_SetQualReport));

#define TAG_CFW_CfgSetFirstPowerOnFlag 0x424b0ac3
struct IPAR_CFW_CfgSetFirstPowerOnFlag
{
    bool bFPO;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_CfgSetFirstPowerOnFlag));

struct OPAR_CFW_CfgSetFirstPowerOnFlag
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_CfgSetFirstPowerOnFlag));

#define TAG_CFW_CfgCheckFirstPowerOnFlag 0xdfab74a6
struct OPAR_CFW_CfgCheckFirstPowerOnFlag
{
    uint32_t result;
    bool bFPO;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_CfgCheckFirstPowerOnFlag));

#define TAG_CFW_CfgGetEventReport 0xfcada50f
struct OPAR_CFW_CfgGetEventReport
{
    uint32_t result;
    uint16_t pMer;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_CfgGetEventReport));

#define TAG_CFW_CfgSetEventReport 0xf37b9eb1
struct IPAR_CFW_CfgSetEventReport
{
    uint16_t pMer;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_CfgSetEventReport));

struct OPAR_CFW_CfgSetEventReport
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_CfgSetEventReport));

#define TAG_CFW_CfgSetResultCodePresentMode 0x9b269a8d
struct IPAR_CFW_CfgSetResultCodePresentMode
{
    uint8_t nMode;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_CfgSetResultCodePresentMode));

struct OPAR_CFW_CfgSetResultCodePresentMode
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_CfgSetResultCodePresentMode));

#define TAG_CFW_CfgGetResultCodePresentMode 0x6fc3900c
struct OPAR_CFW_CfgGetResultCodePresentMode
{
    uint32_t result;
    uint8_t nMode;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_CfgGetResultCodePresentMode));

#define TAG_CFW_CfgSetResultCodeFormatMode 0xa7d4e623
struct IPAR_CFW_CfgSetResultCodeFormatMode
{
    uint8_t pMode;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_CfgSetResultCodeFormatMode));

struct OPAR_CFW_CfgSetResultCodeFormatMode
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_CfgSetResultCodeFormatMode));

#define TAG_CFW_CfgGetResultCodeFormatMode 0xc0bd302c
struct OPAR_CFW_CfgGetResultCodeFormatMode
{
    uint32_t result;
    uint8_t pMode;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_CfgGetResultCodeFormatMode));

#define TAG_CFW_CfgSetEchoMode 0xd5b14f3b
struct IPAR_CFW_CfgSetEchoMode
{
    uint8_t nMode;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_CfgSetEchoMode));

struct OPAR_CFW_CfgSetEchoMode
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_CfgSetEchoMode));

#define TAG_CFW_CfgGetEchoMode 0x7a5ee337
struct OPAR_CFW_CfgGetEchoMode
{
    uint32_t result;
    uint8_t nMode;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_CfgGetEchoMode));

#define TAG_CFW_CfgGetExtendedErrorV1 0x72ad454c
struct OPAR_CFW_CfgGetExtendedErrorV1
{
    uint32_t result;
    CFW_EXTENDED_ERROR pExtendedError;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_CfgGetExtendedErrorV1));

#define TAG_CFW_CfgGetErrorReport 0xcbc0852d
struct OPAR_CFW_CfgGetErrorReport
{
    uint32_t result;
    uint8_t pLevel;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_CfgGetErrorReport));

#define TAG_CFW_CfgSetErrorReport 0x0f7a5643
struct IPAR_CFW_CfgSetErrorReport
{
    uint8_t pLevel;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_CfgSetErrorReport));

struct OPAR_CFW_CfgSetErrorReport
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_CfgSetErrorReport));

#define TAG_CFW_CfgDeviceSwithOff 0x71ed90b2
struct OPAR_CFW_CfgDeviceSwithOff
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_CfgDeviceSwithOff));

#define TAG_CFW_CfgSetConnectResultFormatCode 0xaa3996b2
struct IPAR_CFW_CfgSetConnectResultFormatCode
{
    uint8_t nValue;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_CfgSetConnectResultFormatCode));

struct OPAR_CFW_CfgSetConnectResultFormatCode
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_CfgSetConnectResultFormatCode));

#define TAG_CFW_CfgGetConnectResultFormatCode 0x92ddeb2c
struct OPAR_CFW_CfgGetConnectResultFormatCode
{
    uint32_t result;
    uint8_t nValue;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_CfgGetConnectResultFormatCode));

#define TAG_CFW_CfgSetSoftReset 0xb02b949a
struct IPAR_CFW_CfgSetSoftReset
{
    uint8_t cause;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_CfgSetSoftReset));

struct OPAR_CFW_CfgSetSoftReset
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_CfgSetSoftReset));

#define TAG_CFW_CfgGetSoftReset 0xc122799c
struct OPAR_CFW_CfgGetSoftReset
{
    uint32_t result;
    uint8_t cause;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_CfgGetSoftReset));

#define TAG_CFW_CfgSetSmsStorage 0x3f3995c2
struct IPAR_CFW_CfgSetSmsStorage
{
    uint8_t nStorage;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_CfgSetSmsStorage));

struct OPAR_CFW_CfgSetSmsStorage
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_CfgSetSmsStorage));

#define TAG_CFW_CfgGetSmsStorage 0xa0dae657
struct IPAR_CFW_CfgGetSmsStorage
{
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_CfgGetSmsStorage));

struct OPAR_CFW_CfgGetSmsStorage
{
    uint32_t result;
    uint8_t nStorage;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_CfgGetSmsStorage));

#define TAG_CFW_CfgSetNewSmsOption 0x592995fb
struct IPAR_CFW_CfgSetNewSmsOption
{
    uint8_t nOption;
    uint8_t nNewSmsStorage;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_CfgSetNewSmsOption));

struct OPAR_CFW_CfgSetNewSmsOption
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_CfgSetNewSmsOption));

#define TAG_CFW_CfgGetNewSmsOption 0x1ea60113
struct IPAR_CFW_CfgGetNewSmsOption
{
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_CfgGetNewSmsOption));

struct OPAR_CFW_CfgGetNewSmsOption
{
    uint32_t result;
    uint8_t nOption;
    uint8_t nNewSmsStorage;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_CfgGetNewSmsOption));

#define TAG_CFW_CfgSetSmsOverflowInd 0xb4b7a4da
struct IPAR_CFW_CfgSetSmsOverflowInd
{
    uint8_t nMode;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_CfgSetSmsOverflowInd));

struct OPAR_CFW_CfgSetSmsOverflowInd
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_CfgSetSmsOverflowInd));

#define TAG_CFW_CfgGetSmsOverflowInd 0x7fcfef26
struct IPAR_CFW_CfgGetSmsOverflowInd
{
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_CfgGetSmsOverflowInd));

struct OPAR_CFW_CfgGetSmsOverflowInd
{
    uint32_t result;
    uint8_t nMode;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_CfgGetSmsOverflowInd));

#define TAG_CFW_CfgSetSmsFormat 0xa9f316f2
struct IPAR_CFW_CfgSetSmsFormat
{
    uint8_t nFormat;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_CfgSetSmsFormat));

struct OPAR_CFW_CfgSetSmsFormat
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_CfgSetSmsFormat));

#define TAG_CFW_CfgGetSmsFormat 0x203e9990
struct IPAR_CFW_CfgGetSmsFormat
{
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_CfgGetSmsFormat));

struct OPAR_CFW_CfgGetSmsFormat
{
    uint32_t result;
    uint8_t nFormat;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_CfgGetSmsFormat));

#define TAG_CFW_CfgSetSmsShowTextModeParam 0x1503ad06
struct IPAR_CFW_CfgSetSmsShowTextModeParam
{
    uint8_t nShow;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_CfgSetSmsShowTextModeParam));

struct OPAR_CFW_CfgSetSmsShowTextModeParam
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_CfgSetSmsShowTextModeParam));

#define TAG_CFW_CfgGetSmsShowTextModeParam 0xcaec4d3b
struct IPAR_CFW_CfgGetSmsShowTextModeParam
{
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_CfgGetSmsShowTextModeParam));

struct OPAR_CFW_CfgGetSmsShowTextModeParam
{
    uint32_t result;
    uint8_t nShow;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_CfgGetSmsShowTextModeParam));

#define TAG_CFW_CfgSetSmsParam 0xd480bf02
struct IPAR_CFW_CfgSetSmsParam
{
    CFW_SMS_PARAMETER pInfo;
    uint16_t nUTI;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_CfgSetSmsParam));

struct OPAR_CFW_CfgSetSmsParam
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_CfgSetSmsParam));

#define TAG_CFW_CfgGetSmsParam 0xf2b23c71
struct IPAR_CFW_CfgGetSmsParam
{
    uint8_t nIndex;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_CfgGetSmsParam));

struct OPAR_CFW_CfgGetSmsParam
{
    uint32_t result;
    CFW_SMS_PARAMETER pInfo;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_CfgGetSmsParam));

#define TAG_CFW_CfgSetDefaultSmsParam 0xe6b2eaa7
struct IPAR_CFW_CfgSetDefaultSmsParam
{
    CFW_SMS_PARAMETER pInfo;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_CfgSetDefaultSmsParam));

struct OPAR_CFW_CfgSetDefaultSmsParam
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_CfgSetDefaultSmsParam));

#define TAG_CFW_CfgGetDefaultSmsParam 0xbe60af25
struct IPAR_CFW_CfgGetDefaultSmsParam
{
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_CfgGetDefaultSmsParam));

struct OPAR_CFW_CfgGetDefaultSmsParam
{
    uint32_t result;
    CFW_SMS_PARAMETER pInfo;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_CfgGetDefaultSmsParam));

#define TAG_CFW_CfgSetSmsStorageInfo 0x7e01cec5
struct IPAR_CFW_CfgSetSmsStorageInfo
{
    CFW_SMS_STORAGE_INFO pStorageInfo;
    uint16_t nUTI;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_CfgSetSmsStorageInfo));

struct OPAR_CFW_CfgSetSmsStorageInfo
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_CfgSetSmsStorageInfo));

#define TAG_CFW_CfgGetSmsStorageInfo 0x2e2a7c57
struct IPAR_CFW_CfgGetSmsStorageInfo
{
    uint16_t nUTI;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_CfgGetSmsStorageInfo));

struct OPAR_CFW_CfgGetSmsStorageInfo
{
    uint32_t result;
    CFW_SMS_STORAGE_INFO pStorageInfo;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_CfgGetSmsStorageInfo));

#define TAG_CFW_SimGetSmsTotalNum 0x8c9061a8
struct IPAR_CFW_SimGetSmsTotalNum
{
    uint16_t nUTI;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_SimGetSmsTotalNum));

struct OPAR_CFW_SimGetSmsTotalNum
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_SimGetSmsTotalNum));

#define TAG_CFW_CfgSelectSmsService 0xe1d549f5
struct IPAR_CFW_CfgSelectSmsService
{
    uint8_t nService;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_CfgSelectSmsService));

struct OPAR_CFW_CfgSelectSmsService
{
    uint32_t result;
    uint8_t pSupportedType;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_CfgSelectSmsService));

#define TAG_CFW_CfgQuerySmsService 0x398b2a95
struct OPAR_CFW_CfgQuerySmsService
{
    uint32_t result;
    uint8_t nService;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_CfgQuerySmsService));

#define TAG_CFW_CfgSetSmsCB 0xc3a3200e
struct IPAR_CFW_CfgSetSmsCB
{
    uint8_t mode;
    uint8_t mids[6];
    uint8_t dcss[6];
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_CfgSetSmsCB));

struct OPAR_CFW_CfgSetSmsCB
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_CfgSetSmsCB));

#define TAG_CFW_CfgGetSmsCB 0xc74fce6d
struct OPAR_CFW_CfgGetSmsCB
{
    uint32_t result;
    uint8_t pMode;
    uint8_t mids[6];
    uint8_t dcss[6];
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_CfgGetSmsCB));

#define TAG_CFW_SetSimStatus 0x610691f7
struct IPAR_CFW_SetSimStatus
{
    CFW_SIM_ID nSimID;
    CFW_SIM_STATUS nSimStatus;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_SetSimStatus));

#define TAG_CFW_GetSimStatus 0xeeef512f
struct IPAR_CFW_GetSimStatus
{
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_GetSimStatus));

struct OPAR_CFW_GetSimStatus
{
    CFW_SIM_STATUS result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_GetSimStatus));

#define TAG_CFW_SimClose 0x2fd3f031
struct IPAR_CFW_SimClose
{
    uint16_t nUTI;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_SimClose));

struct OPAR_CFW_SimClose
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_SimClose));

#define TAG_CFW_GetStackSimFileID 0xb0ff5bb2
struct IPAR_CFW_GetStackSimFileID
{
    uint16_t n3GppFileID;
    uint16_t EFPath;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_GetStackSimFileID));

struct OPAR_CFW_GetStackSimFileID
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_GetStackSimFileID));

#define TAG_CFW_SimReadBinary 0x6482e52d
struct IPAR_CFW_SimReadBinary
{
    uint8_t nFileId;
    uint8_t nOffset;
    uint8_t nBytesToRead;
    uint16_t nUTI;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_SimReadBinary));

struct OPAR_CFW_SimReadBinary
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_SimReadBinary));

#define TAG_CFW_SimUpdateBinary 0xeb4d03f5
struct IPAR_CFW_SimUpdateBinary
{
    uint8_t nFileId;
    uint8_t nOffset;
    uint8_t nBytesToWrite;
    uint16_t nUTI;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_SimUpdateBinary));

struct OPAR_CFW_SimUpdateBinary
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_SimUpdateBinary));

#define TAG_CFW_SimReadRecord 0xd1edf62e
struct IPAR_CFW_SimReadRecord
{
    uint8_t nFileID;
    uint8_t nRecordNum;
    uint16_t nUTI;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_SimReadRecord));

struct OPAR_CFW_SimReadRecord
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_SimReadRecord));

#define TAG_CFW_SimUpdateRecord 0x080cd798
struct IPAR_CFW_SimUpdateRecord
{
    uint8_t nFileID;
    uint8_t nRecordNum;
    uint8_t nRecordSize;
    uint16_t nUTI;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_SimUpdateRecord));

struct OPAR_CFW_SimUpdateRecord
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_SimUpdateRecord));

#define TAG_CFW_SimGetFileStatus 0x0c820824
struct IPAR_CFW_SimGetFileStatus
{
    uint8_t nFileID;
    uint16_t nUTI;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_SimGetFileStatus));

struct OPAR_CFW_SimGetFileStatus
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_SimGetFileStatus));

#define TAG_CFW_SmsMoInit 0xe5fdf9e0
struct IPAR_CFW_SmsMoInit
{
    uint16_t nUti;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_SmsMoInit));

struct OPAR_CFW_SmsMoInit
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_SmsMoInit));

#define TAG_CFW_SimInit 0x0a272beb
struct IPAR_CFW_SimInit
{
    bool bRstSim;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_SimInit));

struct OPAR_CFW_SimInit
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_SimInit));

#define TAG_CFW_CfgSetSimSwitch 0x14e06e43
struct IPAR_CFW_CfgSetSimSwitch
{
    uint8_t n;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_CfgSetSimSwitch));

struct OPAR_CFW_CfgSetSimSwitch
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_CfgSetSimSwitch));

#define TAG_CFW_CfgGetSimSwitch 0xb9697fc4
struct OPAR_CFW_CfgGetSimSwitch
{
    uint32_t result;
    uint8_t n;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_CfgGetSimSwitch));

#define TAG_SimSwitch 0x665f6ca9
struct IPAR_SimSwitch
{
    uint8_t SimNum;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_SimSwitch));

#define TAG_CFW_CfgSetPbkStrorageInfo 0x73af8928
struct IPAR_CFW_CfgSetPbkStrorageInfo
{
    CFW_PBK_STRORAGE_INFO pStorageInfo;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_CfgSetPbkStrorageInfo));

struct OPAR_CFW_CfgSetPbkStrorageInfo
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_CfgSetPbkStrorageInfo));

#define TAG_CFW_CfgGetPbkStrorageInfo 0xeee2d88b
struct OPAR_CFW_CfgGetPbkStrorageInfo
{
    uint32_t result;
    CFW_PBK_STRORAGE_INFO pStorageInfo;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_CfgGetPbkStrorageInfo));

#define TAG_CFW_CfgSetPbkStorage 0x2a2c1f97
struct IPAR_CFW_CfgSetPbkStorage
{
    uint8_t nStorage;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_CfgSetPbkStorage));

struct OPAR_CFW_CfgSetPbkStorage
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_CfgSetPbkStorage));

#define TAG_CFW_CfgGetPbkStorage 0xcd436267
struct IPAR_CFW_CfgGetPbkStorage
{
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_CfgGetPbkStorage));

struct OPAR_CFW_CfgGetPbkStorage
{
    uint32_t result;
    uint8_t nStorage;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_CfgGetPbkStorage));

#define TAG_CFW_CfgSetStoredPlmnList 0x3ce9c17e
struct IPAR_CFW_CfgSetStoredPlmnList
{
    CFW_StoredPlmnList pPlmnL;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_CfgSetStoredPlmnList));

struct OPAR_CFW_CfgSetStoredPlmnList
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_CfgSetStoredPlmnList));

#define TAG_CFW_CfgGetStoredPlmnList 0xe96c6a34
struct IPAR_CFW_CfgGetStoredPlmnList
{
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_CfgGetStoredPlmnList));

struct OPAR_CFW_CfgGetStoredPlmnList
{
    uint32_t result;
    CFW_StoredPlmnList pPlmnL;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_CfgGetStoredPlmnList));

#define TAG_CFW_CfgNwSetFrequencyBand 0xd2cedcbb
struct IPAR_CFW_CfgNwSetFrequencyBand
{
    uint8_t nBand;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_CfgNwSetFrequencyBand));

struct OPAR_CFW_CfgNwSetFrequencyBand
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_CfgNwSetFrequencyBand));

#define TAG_CFW_CfgNwGetFrequencyBand 0xa3e4654b
struct IPAR_CFW_CfgNwGetFrequencyBand
{
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_CfgNwGetFrequencyBand));

struct OPAR_CFW_CfgNwGetFrequencyBand
{
    uint32_t result;
    uint8_t nBand;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_CfgNwGetFrequencyBand));

#define TAG_CFW_CfgNwSetNetWorkMode 0xc40a65b9
struct IPAR_CFW_CfgNwSetNetWorkMode
{
    uint8_t nMode;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_CfgNwSetNetWorkMode));

struct OPAR_CFW_CfgNwSetNetWorkMode
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_CfgNwSetNetWorkMode));

#define TAG_CFW_CfgNwGetNetWorkMode 0x23c4e97f
struct IPAR_CFW_CfgNwGetNetWorkMode
{
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_CfgNwGetNetWorkMode));

struct OPAR_CFW_CfgNwGetNetWorkMode
{
    uint32_t result;
    uint8_t pMode;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_CfgNwGetNetWorkMode));

#define TAG_CFW_CfgSetNwStatus 0x9dccde75
struct IPAR_CFW_CfgSetNwStatus
{
    uint8_t n;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_CfgSetNwStatus));

struct OPAR_CFW_CfgSetNwStatus
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_CfgSetNwStatus));

#define TAG_CFW_CfgGetNwStatus 0xabfd1d2f
struct IPAR_CFW_CfgGetNwStatus
{
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_CfgGetNwStatus));

struct OPAR_CFW_CfgGetNwStatus
{
    uint32_t result;
    uint8_t pCmd;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_CfgGetNwStatus));

#define TAG_CFW_CfgNwGetOperatorInfo 0x0d3fa799
struct IPAR_CFW_CfgNwGetOperatorInfo
{
    uint32_t nIndex;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_CfgNwGetOperatorInfo));

struct OPAR_CFW_CfgNwGetOperatorInfo
{
    uint32_t result;
    uint8_t * pOperatorId;
    uint8_t * pOperatorName;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_CfgNwGetOperatorInfo));

#define TAG_CFW_CfgNwGetOperatorId 0xf6dd0cb5
struct IPAR_CFW_CfgNwGetOperatorId
{
    uint32_t pOperatorNameSize;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_CfgNwGetOperatorId));

struct OPAR_CFW_CfgNwGetOperatorId
{
    uint32_t result;
    uint8_t * pOperatorId;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_CfgNwGetOperatorId));

#define TAG_CFW_CfgNwGetOperatorName 0x74dc9c53
struct IPAR_CFW_CfgNwGetOperatorName
{
    uint8_t pOperatorId[6];
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_CfgNwGetOperatorName));

struct OPAR_CFW_CfgNwGetOperatorName
{
    uint32_t result;
    uint8_t * pOperatorName;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_CfgNwGetOperatorName));

#define TAG_CFW_CfgSetNwOperDispFormat 0xe503676d
struct IPAR_CFW_CfgSetNwOperDispFormat
{
    uint8_t nFormat;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_CfgSetNwOperDispFormat));

struct OPAR_CFW_CfgSetNwOperDispFormat
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_CfgSetNwOperDispFormat));

#define TAG_CFW_CfgGetNwOperDispFormat 0x5b070593
struct OPAR_CFW_CfgGetNwOperDispFormat
{
    uint32_t result;
    uint8_t nFormat;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_CfgGetNwOperDispFormat));

#define TAG_CFW_CfgSetSSN 0x27d28c5a
struct IPAR_CFW_CfgSetSSN
{
    uint8_t nCSSI;
    uint8_t nCSSU;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_CfgSetSSN));

struct OPAR_CFW_CfgSetSSN
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_CfgSetSSN));

#define TAG_CFW_CfgGetSSN 0xa99abe2e
struct IPAR_CFW_CfgGetSSN
{
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_CfgGetSSN));

struct OPAR_CFW_CfgGetSSN
{
    uint32_t result;
    uint8_t nCSSI;
    uint8_t nCSSU;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_CfgGetSSN));

#define TAG_CFW_CfgSetClir 0x3cf96f15
struct IPAR_CFW_CfgSetClir
{
    uint8_t nClir;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_CfgSetClir));

struct OPAR_CFW_CfgSetClir
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_CfgSetClir));

#define TAG_CFW_CfgGetClir 0xd6d006b2
struct IPAR_CFW_CfgGetClir
{
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_CfgGetClir));

struct OPAR_CFW_CfgGetClir
{
    uint32_t result;
    uint8_t nClir;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_CfgGetClir));

#define TAG_CFW_CfgSetClip 0x8ddaf837
struct IPAR_CFW_CfgSetClip
{
    uint8_t nEnable;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_CfgSetClip));

struct OPAR_CFW_CfgSetClip
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_CfgSetClip));

#define TAG_CFW_CfgGetClip 0x19b6c44a
struct IPAR_CFW_CfgGetClip
{
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_CfgGetClip));

struct OPAR_CFW_CfgGetClip
{
    uint32_t result;
    uint8_t nEnable;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_CfgGetClip));

#define TAG_CFW_CfgSetColp 0x2fd9ebda
struct IPAR_CFW_CfgSetColp
{
    uint8_t n;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_CfgSetColp));

struct OPAR_CFW_CfgSetColp
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_CfgSetColp));

#define TAG_CFW_CfgGetColp 0x3e336470
struct IPAR_CFW_CfgGetColp
{
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_CfgGetColp));

struct OPAR_CFW_CfgGetColp
{
    uint32_t result;
    uint8_t n;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_CfgGetColp));

#define TAG_CFW_CfgSetCallWaiting 0x24883270
struct IPAR_CFW_CfgSetCallWaiting
{
    uint8_t n;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_CfgSetCallWaiting));

struct OPAR_CFW_CfgSetCallWaiting
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_CfgSetCallWaiting));

#define TAG_CFW_CfgGetCallWaiting 0xb36a4c39
struct IPAR_CFW_CfgGetCallWaiting
{
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_CfgGetCallWaiting));

struct OPAR_CFW_CfgGetCallWaiting
{
    uint32_t result;
    uint8_t n;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_CfgGetCallWaiting));

#define TAG_CFW_CfgGetIMSI 0x2d03185d
struct IPAR_CFW_CfgGetIMSI
{
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_CfgGetIMSI));

struct OPAR_CFW_CfgGetIMSI
{
    uint32_t result;
    uint8_t pIMSI[15];
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_CfgGetIMSI));

#define TAG_CFW_CfgSetIMSI 0x88d9b097
struct IPAR_CFW_CfgSetIMSI
{
    uint8_t pIMSI[15];
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_CfgSetIMSI));

struct OPAR_CFW_CfgSetIMSI
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_CfgSetIMSI));

#define TAG_CFW_CfgGetCdmaImsi 0x8f00356e
struct IPAR_CFW_CfgGetCdmaImsi
{
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_CfgGetCdmaImsi));

struct OPAR_CFW_CfgGetCdmaImsi
{
    uint32_t result;
    uint8_t pCdmaImsi[10];
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_CfgGetCdmaImsi));

#define TAG_CFW_NwSetEpsMode 0x84d40ffe
struct IPAR_CFW_NwSetEpsMode
{
    uint8_t eps_mode;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_NwSetEpsMode));

struct OPAR_CFW_NwSetEpsMode
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_NwSetEpsMode));

#define TAG_CFW_CfgSetToneDuration 0x75013fe5
struct IPAR_CFW_CfgSetToneDuration
{
    uint8_t nToneDuration;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_CfgSetToneDuration));

struct OPAR_CFW_CfgSetToneDuration
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_CfgSetToneDuration));

#define TAG_CFW_CfgGetToneDuration 0x23e5688e
struct OPAR_CFW_CfgGetToneDuration
{
    uint32_t result;
    uint8_t pToneDuration;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_CfgGetToneDuration));

#define TAG_CFW_CfgSetIncomingCallResultMode 0xc4bee657
struct IPAR_CFW_CfgSetIncomingCallResultMode
{
    uint8_t nMode;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_CfgSetIncomingCallResultMode));

struct OPAR_CFW_CfgSetIncomingCallResultMode
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_CfgSetIncomingCallResultMode));

#define TAG_CFW_CfgGetIncomingCallResultMode 0xab6bc7f1
struct OPAR_CFW_CfgGetIncomingCallResultMode
{
    uint32_t result;
    uint8_t nMode;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_CfgGetIncomingCallResultMode));

#define TAG_CFW_CfgSetAudioOutputParam 0xf851b8fc
struct IPAR_CFW_CfgSetAudioOutputParam
{
    CFW_AUD_OUT_PARAMETER pAudOutParam;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_CfgSetAudioOutputParam));

struct OPAR_CFW_CfgSetAudioOutputParam
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_CfgSetAudioOutputParam));

#define TAG_CFW_CfgGetAudioOutputParam 0x356b9b3e
struct OPAR_CFW_CfgGetAudioOutputParam
{
    uint32_t result;
    CFW_AUD_OUT_PARAMETER pAudOutParam;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_CfgGetAudioOutputParam));

#define TAG_CFW_CfgSetAudioAudioMode 0x1e00e28a
struct IPAR_CFW_CfgSetAudioAudioMode
{
    uint8_t nAudioMode;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_CfgSetAudioAudioMode));

struct OPAR_CFW_CfgSetAudioAudioMode
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_CfgSetAudioAudioMode));

#define TAG_CFW_CfgGetAudioAudioMode 0x28488c88
struct OPAR_CFW_CfgGetAudioAudioMode
{
    uint32_t result;
    uint8_t pAudioMode;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_CfgGetAudioAudioMode));

#define TAG_CFW_CfgSetLoudspeakerVolumeLevel 0x832b47ad
struct IPAR_CFW_CfgSetLoudspeakerVolumeLevel
{
    uint8_t nVolumeLevel;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_CfgSetLoudspeakerVolumeLevel));

struct OPAR_CFW_CfgSetLoudspeakerVolumeLevel
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_CfgSetLoudspeakerVolumeLevel));

#define TAG_CFW_CfgGetLoudspeakerVolumeLevel 0x37be59b4
struct OPAR_CFW_CfgGetLoudspeakerVolumeLevel
{
    uint32_t result;
    uint8_t pVolumeLevel;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_CfgGetLoudspeakerVolumeLevel));

#define TAG_CFW_CfgGetLoudspeakerVolumeLevelRange 0x78fe7b0d
struct OPAR_CFW_CfgGetLoudspeakerVolumeLevelRange
{
    uint32_t result;
    uint8_t pMinVolumeLevel;
    uint8_t pMaxVolumeLevel;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_CfgGetLoudspeakerVolumeLevelRange));

#define TAG_CFW_ImsInit 0x92d862ef
struct IPAR_CFW_ImsInit
{
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_ImsInit));

struct OPAR_CFW_ImsInit
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_ImsInit));

#define TAG_CFW_ImsEnable 0xe4ed0761
struct IPAR_CFW_ImsEnable
{
    uint8_t isEmergency;
    uint16_t nUTI;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_ImsEnable));

struct OPAR_CFW_ImsEnable
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_ImsEnable));

#define TAG_CFW_ImsDisable 0x9c72a751
struct IPAR_CFW_ImsDisable
{
    uint8_t isEmergency;
    uint16_t nUTI;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_ImsDisable));

struct OPAR_CFW_ImsDisable
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_ImsDisable));

#define TAG_CFW_ImsSuspend 0xa45579e4
struct IPAR_CFW_ImsSuspend
{
    CFW_SIM_ID nSimID;
    CFW_IMS_SUSPEND_TYPE nType;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_ImsSuspend));

struct OPAR_CFW_ImsSuspend
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_ImsSuspend));

#define TAG_CFW_ImsResume 0xb76466e9
struct IPAR_CFW_ImsResume
{
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_ImsResume));

struct OPAR_CFW_ImsResume
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_ImsResume));

#define TAG_CFW_GprsGetNsapi 0x6198a170
struct IPAR_CFW_GprsGetNsapi
{
    uint8_t nCid;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_GprsGetNsapi));

struct OPAR_CFW_GprsGetNsapi
{
    uint8_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_GprsGetNsapi));

#define TAG_CFW_ImsSetDapsCallback 0x554d3316
struct IPAR_CFW_ImsSetDapsCallback
{
    uint8_t nCid;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_ImsSetDapsCallback));

struct OPAR_CFW_ImsSetDapsCallback
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_ImsSetDapsCallback));

#define TAG_CFW_ImsSetVolte 0xf52fc960
struct IPAR_CFW_ImsSetVolte
{
    uint8_t bSet;
    uint16_t nUTI;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_ImsSetVolte));

struct OPAR_CFW_ImsSetVolte
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_ImsSetVolte));

#define TAG_CFW_ImsIsSet 0x9f1f9c07
struct IPAR_CFW_ImsIsSet
{
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_ImsIsSet));

struct OPAR_CFW_ImsIsSet
{
    bool result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_ImsIsSet));

#define TAG_CFW_ImsFreeUti 0xfeb9adf0
struct OPAR_CFW_ImsFreeUti
{
    uint8_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_ImsFreeUti));

#define TAG_CFW_ImsActImsPdp 0xe60f5b9d
struct IPAR_CFW_ImsActImsPdp
{
    uint16_t nUTI;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_ImsActImsPdp));

struct OPAR_CFW_ImsActImsPdp
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_ImsActImsPdp));

#define TAG_CFW_ImsDoSavedEmcDail 0x555cee71
struct OPAR_CFW_ImsDoSavedEmcDail
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_ImsDoSavedEmcDail));

#define TAG_CFW_ImsSetPdpStatus 0x6b251398
struct IPAR_CFW_ImsSetPdpStatus
{
    uint8_t nStatus;
    uint8_t isEmergency;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_ImsSetPdpStatus));

struct OPAR_CFW_ImsSetPdpStatus
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_ImsSetPdpStatus));

#define TAG_CFW_ImsGetPdpStatus 0xe3bacaaf
struct IPAR_CFW_ImsGetPdpStatus
{
    uint8_t isEmergency;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_ImsGetPdpStatus));

struct OPAR_CFW_ImsGetPdpStatus
{
    uint8_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_ImsGetPdpStatus));

#define TAG_CFW_ImsCheckStatusAfterCc 0x2b9844f1
struct IPAR_CFW_ImsCheckStatusAfterCc
{
    uint8_t nCsImsFlag;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_ImsCheckStatusAfterCc));

struct OPAR_CFW_ImsCheckStatusAfterCc
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_ImsCheckStatusAfterCc));

#define TAG_CFW_SsSetClip 0x9d929a96
struct IPAR_CFW_SsSetClip
{
    uint8_t set;
    uint16_t nUTI;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_SsSetClip));

struct OPAR_CFW_SsSetClip
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_SsSetClip));

#define TAG_CFW_SsSetClir 0xad7d0f90
struct IPAR_CFW_SsSetClir
{
    uint8_t set;
    uint16_t nUTI;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_SsSetClir));

struct OPAR_CFW_SsSetClir
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_SsSetClir));

#define TAG_CFW_SsSetColp 0xc637a64f
struct IPAR_CFW_SsSetColp
{
    uint8_t set;
    uint16_t nUTI;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_SsSetColp));

struct OPAR_CFW_SsSetColp
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_SsSetColp));

#define TAG_CFW_ImsIsRegistered 0xdbf659dd
struct IPAR_CFW_ImsIsRegistered
{
    uint8_t isEmergency;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_ImsIsRegistered));

struct OPAR_CFW_ImsIsRegistered
{
    bool result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_ImsIsRegistered));

#define TAG_CFW_NWSetRat 0xb7e77ecb
struct IPAR_CFW_NWSetRat
{
    uint8_t nRat;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_NWSetRat));

struct OPAR_CFW_NWSetRat
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_NWSetRat));

#define TAG_CFW_NWGetRat 0x85e2fdf5
struct IPAR_CFW_NWGetRat
{
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_NWGetRat));

struct OPAR_CFW_NWGetRat
{
    uint8_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_NWGetRat));

#define TAG_CFW_NWSetStackRat 0x3cdf3da7
struct IPAR_CFW_NWSetStackRat
{
    uint8_t nRat;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_NWSetStackRat));

struct OPAR_CFW_NWSetStackRat
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_NWSetStackRat));

#define TAG_CFW_NWGetStackRat 0xa2cd3816
struct IPAR_CFW_NWGetStackRat
{
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_NWGetStackRat));

struct OPAR_CFW_NWGetStackRat
{
    uint8_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_NWGetStackRat));

#define TAG_CFW_NwSetNetMode 0x2c64c51d
struct IPAR_CFW_NwSetNetMode
{
    uint8_t nNetMode;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_NwSetNetMode));

struct OPAR_CFW_NwSetNetMode
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_NwSetNetMode));

#define TAG_CFW_NwGetAvailableOperators 0x76d14edf
struct IPAR_CFW_NwGetAvailableOperators
{
    uint16_t nUTI;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_NwGetAvailableOperators));

struct OPAR_CFW_NwGetAvailableOperators
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_NwGetAvailableOperators));

#define TAG_CFW_NwGetCurrentOperator 0x235d1b94
struct IPAR_CFW_NwGetCurrentOperator
{
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_NwGetCurrentOperator));

struct OPAR_CFW_NwGetCurrentOperator
{
    uint32_t result;
    uint8_t OperatorId[6];
    uint8_t pMode;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_NwGetCurrentOperator));

#define TAG_CFW_NwGetImei 0xfab521ed
struct IPAR_CFW_NwGetImei
{
    uint16_t nUTI;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_NwGetImei));

struct OPAR_CFW_NwGetImei
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_NwGetImei));

#define TAG_CFW_NwGetSignalQuality 0x9723220b
struct IPAR_CFW_NwGetSignalQuality
{
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_NwGetSignalQuality));

struct OPAR_CFW_NwGetSignalQuality
{
    uint32_t result;
    uint8_t pSignalLevel;
    uint8_t pBitError;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_NwGetSignalQuality));

#define TAG_CFW_NwGetStatus 0x9af699bd
struct IPAR_CFW_NwGetStatus
{
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_NwGetStatus));

struct OPAR_CFW_NwGetStatus
{
    uint32_t result;
    CFW_NW_STATUS_INFO pStatusInfo;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_NwGetStatus));

#define TAG_CFW_NwSetRegistration 0x825d1b6c
struct IPAR_CFW_NwSetRegistration
{
    uint8_t nOperatorId[6];
    uint8_t nMode;
    uint8_t nRat;
    uint16_t nUTI;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_NwSetRegistration));

struct OPAR_CFW_NwSetRegistration
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_NwSetRegistration));

#define TAG_CFW_NwDeRegister 0x47c56cdb
struct IPAR_CFW_NwDeRegister
{
    uint16_t nUTI;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_NwDeRegister));

struct OPAR_CFW_NwDeRegister
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_NwDeRegister));

#define TAG_CFW_SetComm 0xfae6ff84
struct IPAR_CFW_SetComm
{
    CFW_COMM_MODE nMode;
    uint8_t nStorageFlag;
    uint16_t nUTI;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_SetComm));

struct OPAR_CFW_SetComm
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_SetComm));

#define TAG_CFW_GetComm 0x965f544e
struct IPAR_CFW_GetComm
{
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_GetComm));

struct OPAR_CFW_GetComm
{
    uint32_t result;
    CFW_COMM_MODE nMode;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_GetComm));

#define TAG_CFW_NwSetFrequencyBand 0x60c38c93
struct IPAR_CFW_NwSetFrequencyBand
{
    uint8_t nBand;
    uint16_t nUTI;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_NwSetFrequencyBand));

struct OPAR_CFW_NwSetFrequencyBand
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_NwSetFrequencyBand));

#define TAG_CFW_NwAbortListOperators 0x6fcc1282
struct IPAR_CFW_NwAbortListOperators
{
    uint16_t nUTI;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_NwAbortListOperators));

struct OPAR_CFW_NwAbortListOperators
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_NwAbortListOperators));

#define TAG_CFW_GetNWTimerOutFlag 0x0a79d13a
struct IPAR_CFW_GetNWTimerOutFlag
{
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_GetNWTimerOutFlag));

struct OPAR_CFW_GetNWTimerOutFlag
{
    bool result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_GetNWTimerOutFlag));

#define TAG_CFW_SetNWTimerOutFlag 0xf65e019c
struct IPAR_CFW_SetNWTimerOutFlag
{
    bool bClear;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_SetNWTimerOutFlag));

#define TAG_CFW_CcAcceptSpeechCall 0x2b480237
struct IPAR_CFW_CcAcceptSpeechCall
{
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_CcAcceptSpeechCall));

struct OPAR_CFW_CcAcceptSpeechCall
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_CcAcceptSpeechCall));

#define TAG_CFW_CcGetCallStatus 0x46843945
struct IPAR_CFW_CcGetCallStatus
{
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_CcGetCallStatus));

struct OPAR_CFW_CcGetCallStatus
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_CcGetCallStatus));

#define TAG_CFW_GetCcCount 0x276ee189
struct IPAR_CFW_GetCcCount
{
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_GetCcCount));

struct OPAR_CFW_GetCcCount
{
    uint8_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_GetCcCount));

#define TAG_CFW_CcGetCurrentCall 0x8da891d9
struct IPAR_CFW_CcGetCurrentCall
{
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_CcGetCurrentCall));

struct OPAR_CFW_CcGetCurrentCall
{
    uint32_t result;
    CFW_CC_CURRENT_CALL_INFO CallInfo[7];
    uint8_t pCnt;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_CcGetCurrentCall));

#define TAG_CFW_CcCallHoldMultiparty 0xd2b4a305
struct IPAR_CFW_CcCallHoldMultiparty
{
    uint8_t nCmd;
    uint8_t nIndex;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_CcCallHoldMultiparty));

struct OPAR_CFW_CcCallHoldMultiparty
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_CcCallHoldMultiparty));

#define TAG_CFW_CcReleaseCallX 0x55e31e22
struct IPAR_CFW_CcReleaseCallX
{
    uint8_t nIndex;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_CcReleaseCallX));

struct OPAR_CFW_CcReleaseCallX
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_CcReleaseCallX));

#define TAG_CFW_CcInitiateSpeechCall_V2 0x10d83a33
struct IPAR_CFW_CcInitiateSpeechCall_V2
{
    CFW_DIALNUMBER_V2 pDialNumber;
    uint16_t nUTI;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_CcInitiateSpeechCall_V2));

struct OPAR_CFW_CcInitiateSpeechCall_V2
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_CcInitiateSpeechCall_V2));

#define TAG_CFW_CcInitiateSpeechCallEx_V2 0xf3beb108
struct IPAR_CFW_CcInitiateSpeechCallEx_V2
{
    CFW_DIALNUMBER_V2 pDialNumber;
    uint16_t nUTI;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_CcInitiateSpeechCallEx_V2));

struct OPAR_CFW_CcInitiateSpeechCallEx_V2
{
    uint32_t result;
    uint8_t pIndex;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_CcInitiateSpeechCallEx_V2));

#define TAG_CFW_CcEmcDial 0x096b6d42
struct IPAR_CFW_CcEmcDial
{
    uint8_t nSize;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_CcEmcDial));

struct OPAR_CFW_CcEmcDial
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_CcEmcDial));

#define TAG_CFW_CcEmcDialEx 0x48762fb4
struct IPAR_CFW_CcEmcDialEx
{
    uint8_t nDialNumberSize;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_CcEmcDialEx));

struct OPAR_CFW_CcEmcDialEx
{
    uint32_t result;
    uint8_t pIndex;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_CcEmcDialEx));

#define TAG_CFW_CcReleaseCall 0x8c4457d3
struct IPAR_CFW_CcReleaseCall
{
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_CcReleaseCall));

struct OPAR_CFW_CcReleaseCall
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_CcReleaseCall));

#define TAG_CFW_CcRejectCall 0xace9ca72
struct IPAR_CFW_CcRejectCall
{
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_CcRejectCall));

struct OPAR_CFW_CcRejectCall
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_CcRejectCall));

#define TAG_CFW_CcPlayTone 0x08790a03
struct IPAR_CFW_CcPlayTone
{
    int8_t iTone;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_CcPlayTone));

struct OPAR_CFW_CcPlayTone
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_CcPlayTone));

#define TAG_CFW_CcStopTone 0x615f0347
struct IPAR_CFW_CcStopTone
{
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_CcStopTone));

struct OPAR_CFW_CcStopTone
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_CcStopTone));

#define TAG_CFW_SsQueryCallWaiting 0x2e1489d8
struct IPAR_CFW_SsQueryCallWaiting
{
    uint8_t nClass;
    uint16_t nUTI;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_SsQueryCallWaiting));

struct OPAR_CFW_SsQueryCallWaiting
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_SsQueryCallWaiting));

#define TAG_CFW_SsSetCallWaiting 0xe9225666
struct IPAR_CFW_SsSetCallWaiting
{
    uint8_t nMode;
    uint8_t nClass;
    uint16_t nUTI;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_SsSetCallWaiting));

struct OPAR_CFW_SsSetCallWaiting
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_SsSetCallWaiting));

#define TAG_CFW_SsSetCallForwarding 0x7dbb6ec7
struct IPAR_CFW_SsSetCallForwarding
{
    CFW_SS_SET_CALLFORWARDING_INFO pCallForwarding;
    uint16_t nUTI;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_SsSetCallForwarding));

struct OPAR_CFW_SsSetCallForwarding
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_SsSetCallForwarding));

#define TAG_CFW_SsQueryCallForwarding 0xf5ab108b
struct IPAR_CFW_SsQueryCallForwarding
{
    uint8_t nReason;
    uint8_t nClass;
    uint16_t nUTI;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_SsQueryCallForwarding));

struct OPAR_CFW_SsQueryCallForwarding
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_SsQueryCallForwarding));

#define TAG_CFW_SsSetFacilityLock 0x26f63727
struct IPAR_CFW_SsSetFacilityLock
{
    uint16_t nFac;
    uint8_t nPwdSize;
    uint8_t nClass;
    uint8_t nMode;
    uint16_t nUTI;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_SsSetFacilityLock));

struct OPAR_CFW_SsSetFacilityLock
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_SsSetFacilityLock));

#define TAG_CFW_SsQueryFacilityLock 0x40860195
struct IPAR_CFW_SsQueryFacilityLock
{
    uint16_t nFac;
    uint8_t nClass;
    uint16_t nUTI;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_SsQueryFacilityLock));

struct OPAR_CFW_SsQueryFacilityLock
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_SsQueryFacilityLock));

#define TAG_CFW_SsChangePassword 0xb7128488
struct IPAR_CFW_SsChangePassword
{
    uint16_t nFac;
    uint8_t nOldPwdSize;
    uint8_t nNewPwdSize;
    uint16_t nUTI;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_SsChangePassword));

struct OPAR_CFW_SsChangePassword
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_SsChangePassword));

#define TAG_CFW_SsQueryClip 0x618b132d
struct IPAR_CFW_SsQueryClip
{
    uint16_t nUTI;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_SsQueryClip));

struct OPAR_CFW_SsQueryClip
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_SsQueryClip));

#define TAG_CFW_SsQueryClir 0xa6720283
struct IPAR_CFW_SsQueryClir
{
    uint16_t nUTI;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_SsQueryClir));

struct OPAR_CFW_SsQueryClir
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_SsQueryClir));

#define TAG_CFW_SsQueryColp 0x895b63e7
struct IPAR_CFW_SsQueryColp
{
    uint16_t nUTI;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_SsQueryColp));

struct OPAR_CFW_SsQueryColp
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_SsQueryColp));

#define TAG_CFW_SsQueryColr 0xd6d9fa1b
struct IPAR_CFW_SsQueryColr
{
    uint16_t nUTI;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_SsQueryColr));

struct OPAR_CFW_SsQueryColr
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_SsQueryColr));

#define TAG_CFW_SsSendUSSD 0x89f8a01a
struct IPAR_CFW_SsSendUSSD
{
    uint8_t nUsdStringSize;
    uint8_t nOption;
    uint8_t nDcs;
    uint16_t nUTI;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_SsSendUSSD));

struct OPAR_CFW_SsSendUSSD
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_SsSendUSSD));

#define TAG_CFW_SimChangePassword 0xb3513926
struct IPAR_CFW_SimChangePassword
{
    uint16_t nFac;
    uint8_t nOldPwdSize;
    uint8_t nNewPwdSize;
    uint16_t nUTI;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_SimChangePassword));

struct OPAR_CFW_SimChangePassword
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_SimChangePassword));

#define TAG_CFW_SimGetAuthenticationStatus 0x3f6c8048
struct IPAR_CFW_SimGetAuthenticationStatus
{
    uint16_t nUTI;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_SimGetAuthenticationStatus));

struct OPAR_CFW_SimGetAuthenticationStatus
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_SimGetAuthenticationStatus));

#define TAG_CFW_SimEnterAuthentication 0xda558016
struct IPAR_CFW_SimEnterAuthentication
{
    uint8_t nPinSize;
    uint8_t nNewPinSize;
    uint8_t nOption;
    uint16_t nUTI;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_SimEnterAuthentication));

struct OPAR_CFW_SimEnterAuthentication
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_SimEnterAuthentication));

#define TAG_CFW_SimSetFacilityLock 0xd6fab239
struct IPAR_CFW_SimSetFacilityLock
{
    uint16_t nFac;
    uint8_t nPwdSize;
    uint8_t nMode;
    uint16_t nUTI;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_SimSetFacilityLock));

struct OPAR_CFW_SimSetFacilityLock
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_SimSetFacilityLock));

#define TAG_CFW_SimGetFacilityLock 0xd28c1f50
struct IPAR_CFW_SimGetFacilityLock
{
    uint16_t nFac;
    uint16_t nUTI;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_SimGetFacilityLock));

struct OPAR_CFW_SimGetFacilityLock
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_SimGetFacilityLock));

#define TAG_CFW_SimGetProviderId 0x3bc96b1f
struct IPAR_CFW_SimGetProviderId
{
    uint16_t nUTI;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_SimGetProviderId));

struct OPAR_CFW_SimGetProviderId
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_SimGetProviderId));

#define TAG_CFW_SimSetPrefOperatorList 0x1f9e7d3c
struct IPAR_CFW_SimSetPrefOperatorList
{
    uint8_t nSize;
    uint16_t nUTI;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_SimSetPrefOperatorList));

struct OPAR_CFW_SimSetPrefOperatorList
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_SimSetPrefOperatorList));

#define TAG_CFW_SimGetPrefOperatorListMaxNum 0xa7a1c43e
struct IPAR_CFW_SimGetPrefOperatorListMaxNum
{
    uint16_t nUTI;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_SimGetPrefOperatorListMaxNum));

struct OPAR_CFW_SimGetPrefOperatorListMaxNum
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_SimGetPrefOperatorListMaxNum));

#define TAG_CFW_SimGetPrefOperatorList 0x2fc7c1ba
struct IPAR_CFW_SimGetPrefOperatorList
{
    uint16_t nUTI;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_SimGetPrefOperatorList));

struct OPAR_CFW_SimGetPrefOperatorList
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_SimGetPrefOperatorList));

#define TAG_CFW_SimAddPbkEntry 0x045ceb49
struct IPAR_CFW_SimAddPbkEntry
{
    uint8_t nStorage;
    CFW_SIM_PBK_ENTRY_INFO pEntryInfo;
    uint16_t nUTI;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_SimAddPbkEntry));

struct OPAR_CFW_SimAddPbkEntry
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_SimAddPbkEntry));

#define TAG_CFW_SimAddPbkEntry_V2 0xb5beb0cf
struct IPAR_CFW_SimAddPbkEntry_V2
{
    uint8_t nStorage;
    CFW_SIM_PBK_ENTRY_INFO_V2 pEntryInfo;
    uint16_t nUTI;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_SimAddPbkEntry_V2));

struct OPAR_CFW_SimAddPbkEntry_V2
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_SimAddPbkEntry_V2));

#define TAG_CFW_SimDeletePbkEntry 0xa64720ca
struct IPAR_CFW_SimDeletePbkEntry
{
    uint8_t nStorage;
    uint16_t nIndex;
    uint16_t nUTI;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_SimDeletePbkEntry));

struct OPAR_CFW_SimDeletePbkEntry
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_SimDeletePbkEntry));

#define TAG_CFW_SimGetPbkEntry 0xaa8591be
struct IPAR_CFW_SimGetPbkEntry
{
    uint8_t nStorage;
    uint16_t nIndex;
    uint16_t nUTI;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_SimGetPbkEntry));

struct OPAR_CFW_SimGetPbkEntry
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_SimGetPbkEntry));

#define TAG_CFW_SimListPbkEntries 0x62e3977e
struct IPAR_CFW_SimListPbkEntries
{
    uint8_t nStorage;
    uint16_t nIndexStart;
    uint16_t nIndexEnd;
    uint16_t nUTI;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_SimListPbkEntries));

struct OPAR_CFW_SimListPbkEntries
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_SimListPbkEntries));

#define TAG_CFW_SimListCountPbkEntries 0x5e84647a
struct IPAR_CFW_SimListCountPbkEntries
{
    uint8_t nStorage;
    uint16_t nIndexStart;
    uint16_t nCount;
    uint16_t nUTI;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_SimListCountPbkEntries));

struct OPAR_CFW_SimListCountPbkEntries
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_SimListCountPbkEntries));

#define TAG_CFW_SimGetPbkStrorageInfo 0x87cf01e4
struct IPAR_CFW_SimGetPbkStrorageInfo
{
    uint8_t nStorage;
    uint16_t nUTI;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_SimGetPbkStrorageInfo));

struct OPAR_CFW_SimGetPbkStrorageInfo
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_SimGetPbkStrorageInfo));

#define TAG_CFW_SimGetPbkStorage 0x4ab2b8c4
struct IPAR_CFW_SimGetPbkStorage
{
    uint8_t nStorage;
    uint16_t nUTI;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_SimGetPbkStorage));

struct OPAR_CFW_SimGetPbkStorage
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_SimGetPbkStorage));

#define TAG_CFW_SatActivation 0x2d9d2455
struct IPAR_CFW_SatActivation
{
    uint8_t nMode;
    uint16_t nUTI;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_SatActivation));

struct OPAR_CFW_SatActivation
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_SatActivation));

#define TAG_CFW_SatResponse 0x4fb39c55
struct IPAR_CFW_SatResponse
{
    uint8_t nCmdType;
    uint8_t nStatus;
    uint8_t nItemId;
    uint8_t InputStrLen;
    uint16_t nUTI;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_SatResponse));

struct OPAR_CFW_SatResponse
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_SatResponse));

#define TAG_CFW_SatGetCurCMD 0x5a8598b0
struct IPAR_CFW_SatGetCurCMD
{
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_SatGetCurCMD));

struct OPAR_CFW_SatGetCurCMD
{
    uint8_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_SatGetCurCMD));

#define TAG_CFW_GetMNCLen 0x95b333c9
struct IPAR_CFW_GetMNCLen
{
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_GetMNCLen));

struct OPAR_CFW_GetMNCLen
{
    uint8_t pLen;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_GetMNCLen));

#define TAG_CFW_CfgSimGetMeProfile 0x365a1560
struct OPAR_CFW_CfgSimGetMeProfile
{
    uint32_t result;
    CFW_PROFILE pMeProfile;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_CfgSimGetMeProfile));

#define TAG_CFW_CfgSimSetMeProfile 0x398eddda
struct IPAR_CFW_CfgSimSetMeProfile
{
    CFW_PROFILE pMeProfile;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_CfgSimSetMeProfile));

struct OPAR_CFW_CfgSimSetMeProfile
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_CfgSimSetMeProfile));

#define TAG_CFW_SimGetICCID 0x0807f6e5
struct IPAR_CFW_SimGetICCID
{
    uint16_t nUTI;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_SimGetICCID));

struct OPAR_CFW_SimGetICCID
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_SimGetICCID));

#define TAG_CFW_SmsAbortSendMessage 0x3d6b4b93
struct IPAR_CFW_SmsAbortSendMessage
{
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_SmsAbortSendMessage));

struct OPAR_CFW_SmsAbortSendMessage
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_SmsAbortSendMessage));

#define TAG_CFW_SmsDeleteMessage 0x50cab25c
struct IPAR_CFW_SmsDeleteMessage
{
    uint16_t nIndex;
    uint8_t nStatus;
    uint8_t nStorage;
    uint8_t nType;
    uint16_t nUTI;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_SmsDeleteMessage));

struct OPAR_CFW_SmsDeleteMessage
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_SmsDeleteMessage));

#define TAG_CFW_SmsSetUnRead2Read 0xde75aebe
struct IPAR_CFW_SmsSetUnRead2Read
{
    uint8_t nStorage;
    uint8_t nType;
    uint16_t nIndex;
    uint16_t nUTI;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_SmsSetUnRead2Read));

struct OPAR_CFW_SmsSetUnRead2Read
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_SmsSetUnRead2Read));

#define TAG_CFW_SmsListMessages 0xdb8ad850
struct IPAR_CFW_SmsListMessages
{
    CFW_SMS_LIST pListInfo;
    uint16_t nUTI;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_SmsListMessages));

struct OPAR_CFW_SmsListMessages
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_SmsListMessages));

#define TAG_CFW_SmsReadMessage 0x9e37b0ea
struct IPAR_CFW_SmsReadMessage
{
    uint8_t nStorage;
    uint8_t nType;
    uint16_t nIndex;
    uint16_t nUTI;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_SmsReadMessage));

struct OPAR_CFW_SmsReadMessage
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_SmsReadMessage));

#define TAG_CFW_SimGetServiceProviderName 0x67735c93
struct IPAR_CFW_SimGetServiceProviderName
{
    uint16_t nUTI;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_SimGetServiceProviderName));

struct OPAR_CFW_SimGetServiceProviderName
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_SimGetServiceProviderName));

#define TAG_CFW_SmsSetRead2UnRead 0xf4a2809f
struct IPAR_CFW_SmsSetRead2UnRead
{
    uint8_t nStorage;
    uint8_t nType;
    uint16_t nIndex;
    uint16_t nUTI;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_SmsSetRead2UnRead));

struct OPAR_CFW_SmsSetRead2UnRead
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_SmsSetRead2UnRead));

#define TAG_CFW_CheckIfAllowedRecvSms 0xee15e996
struct IPAR_CFW_CheckIfAllowedRecvSms
{
    bool bIfAllowed;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_CheckIfAllowedRecvSms));

#define TAG_CFW_SmsSendPduMessage 0xc021092d
struct IPAR_CFW_SmsSendPduMessage
{
    uint16_t nDataSize;
    uint16_t nUTI;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_SmsSendPduMessage));

struct OPAR_CFW_SmsSendPduMessage
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_SmsSendPduMessage));

#define TAG_CFW_SmsSendMessage_V2 0xab91f894
struct IPAR_CFW_SmsSendMessage_V2
{
    CFW_DIALNUMBER_V2 pNumber;
    uint16_t nDataSize;
    uint16_t nUTI;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_SmsSendMessage_V2));

struct OPAR_CFW_SmsSendMessage_V2
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_SmsSendMessage_V2));

#define TAG_CFW_SmsWriteMessage_V2 0x2c80e79f
struct IPAR_CFW_SmsWriteMessage_V2
{
    CFW_DIALNUMBER_V2 pNumber;
    uint16_t nDataSize;
    uint16_t nIndex;
    uint8_t nStorage;
    uint8_t nType;
    uint8_t nStatus;
    uint16_t nUTI;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_SmsWriteMessage_V2));

struct OPAR_CFW_SmsWriteMessage_V2
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_SmsWriteMessage_V2));

#define TAG_CFW_SmsSetUnSent2Sent 0xa8596703
struct IPAR_CFW_SmsSetUnSent2Sent
{
    uint8_t nStorage;
    uint8_t nType;
    uint16_t nIndex;
    uint8_t nSendStatus;
    uint16_t nUTI;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_SmsSetUnSent2Sent));

struct OPAR_CFW_SmsSetUnSent2Sent
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_SmsSetUnSent2Sent));

#define TAG_CFW_SmsGetMessageNodeEx 0xc6e5fb72
struct IPAR_CFW_SmsGetMessageNodeEx
{
    void * pListResult;
    uint16_t nIndex;
    CFW_SIM_ID nSimId;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_SmsGetMessageNodeEx));

struct OPAR_CFW_SmsGetMessageNodeEx
{
    uint32_t result;
    CFW_SMS_NODE_EX pNode;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_SmsGetMessageNodeEx));

#define TAG_CFW_SmsInitComplete 0x0aeb7786
struct IPAR_CFW_SmsInitComplete
{
    uint16_t nUTI;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_SmsInitComplete));

struct OPAR_CFW_SmsInitComplete
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_SmsInitComplete));

#define TAG_CFW_SetSMSConcat 0x41075729
struct IPAR_CFW_SetSMSConcat
{
    bool bConcat;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_SetSMSConcat));

struct OPAR_CFW_SetSMSConcat
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_SetSMSConcat));

#define TAG_CFW_GetSMSConcat 0x16e18027
struct IPAR_CFW_GetSMSConcat
{
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_GetSMSConcat));

struct OPAR_CFW_GetSMSConcat
{
    bool result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_GetSMSConcat));

#define TAG_CFW_SimReadMessage 0x24e30979
struct IPAR_CFW_SimReadMessage
{
    uint16_t nLocation;
    uint32_t nIndex;
    uint16_t nUTI;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_SimReadMessage));

struct OPAR_CFW_SimReadMessage
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_SimReadMessage));

#define TAG_CFW_SimWriteMessage 0x606ae82f
struct IPAR_CFW_SimWriteMessage
{
    uint16_t nLocation;
    uint32_t nIndex;
    uint8_t nDataSize;
    uint16_t nUTI;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_SimWriteMessage));

struct OPAR_CFW_SimWriteMessage
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_SimWriteMessage));

#define TAG_CFW_GprsSetPdpCxtV2 0x6e3e849a
struct IPAR_CFW_GprsSetPdpCxtV2
{
    uint8_t nCid;
    CFW_GPRS_PDPCONT_INFO_V2 pPdpCont;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_GprsSetPdpCxtV2));

struct OPAR_CFW_GprsSetPdpCxtV2
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_GprsSetPdpCxtV2));

#define TAG_CFW_GprsGetPdpCxtV2 0x28c73dae
struct IPAR_CFW_GprsGetPdpCxtV2
{
    uint8_t nCid;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_GprsGetPdpCxtV2));

struct OPAR_CFW_GprsGetPdpCxtV2
{
    uint32_t result;
    CFW_GPRS_PDPCONT_INFO_V2 pPdpCont;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_GprsGetPdpCxtV2));

#define TAG_CFW_GprsSetMinQos 0xdd6267c5
struct IPAR_CFW_GprsSetMinQos
{
    uint8_t nCid;
    CFW_GPRS_QOS pQos;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_GprsSetMinQos));

struct OPAR_CFW_GprsSetMinQos
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_GprsSetMinQos));

#define TAG_CFW_GprsGetMInQos 0x53a8e0fb
struct IPAR_CFW_GprsGetMInQos
{
    uint8_t nCid;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_GprsGetMInQos));

struct OPAR_CFW_GprsGetMInQos
{
    uint32_t result;
    CFW_GPRS_QOS pQos;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_GprsGetMInQos));

#define TAG_CFW_GprsSetReqQos 0x945f113e
struct IPAR_CFW_GprsSetReqQos
{
    uint8_t nCid;
    CFW_GPRS_QOS pQos;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_GprsSetReqQos));

struct OPAR_CFW_GprsSetReqQos
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_GprsSetReqQos));

#define TAG_CFW_GprsGetReqQos 0xf0322972
struct IPAR_CFW_GprsGetReqQos
{
    uint8_t nCid;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_GprsGetReqQos));

struct OPAR_CFW_GprsGetReqQos
{
    uint32_t result;
    CFW_GPRS_QOS pQos;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_GprsGetReqQos));

#define TAG_CFW_GprsGetPdpAddr 0x86a85765
struct IPAR_CFW_GprsGetPdpAddr
{
    uint8_t nCid;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_GprsGetPdpAddr));

struct OPAR_CFW_GprsGetPdpAddr
{
    uint32_t result;
    uint8_t nLength;
    uint8_t pPdpAdd[THE_PDP_ADDR_MAX_LEN];
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_GprsGetPdpAddr));

#define TAG_CFW_GprsGetstatus 0x6d2f4987
struct IPAR_CFW_GprsGetstatus
{
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_GprsGetstatus));

struct OPAR_CFW_GprsGetstatus
{
    uint32_t result;
    CFW_NW_STATUS_INFO pStatus;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_GprsGetstatus));

#define TAG_CFW_GetGprsAttState 0xe07bd895
struct IPAR_CFW_GetGprsAttState
{
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_GetGprsAttState));

struct OPAR_CFW_GetGprsAttState
{
    uint32_t result;
    uint8_t pState;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_GetGprsAttState));

#define TAG_CFW_GetGprsActState 0x0a671c97
struct IPAR_CFW_GetGprsActState
{
    uint8_t nCid;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_GetGprsActState));

struct OPAR_CFW_GetGprsActState
{
    uint32_t result;
    uint8_t pState;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_GetGprsActState));

#define TAG_CFW_GprsAct 0x63af5d12
struct IPAR_CFW_GprsAct
{
    uint8_t nState;
    uint8_t nCid;
    uint16_t nUTI;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_GprsAct));

struct OPAR_CFW_GprsAct
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_GprsAct));

#define TAG_CFW_GprsActEx 0x76a3cadf
struct IPAR_CFW_GprsActEx
{
    uint8_t nState;
    uint8_t nCid;
    uint16_t nUTI;
    CFW_SIM_ID nSimID;
    bool SavedInApp;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_GprsActEx));

struct OPAR_CFW_GprsActEx
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_GprsActEx));

#define TAG_CFW_AttDetach 0xc1988543
struct IPAR_CFW_AttDetach
{
    uint8_t nState;
    uint16_t nUTI;
    uint8_t AttDetachType;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_AttDetach));

struct OPAR_CFW_AttDetach
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_AttDetach));

#define TAG_CFW_GprsSecPdpAct 0x761ecef8
struct IPAR_CFW_GprsSecPdpAct
{
    uint8_t nState;
    uint8_t nCid;
    uint8_t nPCid;
    uint16_t nUTI;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_GprsSecPdpAct));

struct OPAR_CFW_GprsSecPdpAct
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_GprsSecPdpAct));

#define TAG_CFW_GprsSetPsm 0xb70c6d65
struct IPAR_CFW_GprsSetPsm
{
    uint8_t psmEnable;
    uint8_t bitmap;
    uint8_t t3324;
    uint8_t t3412;
    uint8_t nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_GprsSetPsm));

struct OPAR_CFW_GprsSetPsm
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_GprsSetPsm));

#define TAG_CFW_GprsSetCtxTFT 0x79d77a2b
struct IPAR_CFW_GprsSetCtxTFT
{
    uint8_t nCid;
    CFW_TFT_SET pTft;
    uint8_t nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_GprsSetCtxTFT));

struct OPAR_CFW_GprsSetCtxTFT
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_GprsSetCtxTFT));

#define TAG_CFW_GprsSetCtxEQos 0x22a1f04c
struct IPAR_CFW_GprsSetCtxEQos
{
    uint8_t nCid;
    CFW_EQOS pQos;
    uint8_t nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_GprsSetCtxEQos));

struct OPAR_CFW_GprsSetCtxEQos
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_GprsSetCtxEQos));

#define TAG_CFW_GprsGetCtxEQos 0xea5153a8
struct IPAR_CFW_GprsGetCtxEQos
{
    uint8_t nCid;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_GprsGetCtxEQos));

struct OPAR_CFW_GprsGetCtxEQos
{
    uint32_t result;
    CFW_EQOS pQos;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_GprsGetCtxEQos));

#define TAG_CFW_GprsCtxEpsModify 0x526ea981
struct IPAR_CFW_GprsCtxEpsModify
{
    uint16_t nUTI;
    uint8_t nCid;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_GprsCtxEpsModify));

struct OPAR_CFW_GprsCtxEpsModify
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_GprsCtxEpsModify));

#define TAG_CFW_GprsManualAcc 0x1514c385
struct IPAR_CFW_GprsManualAcc
{
    uint16_t nUTI;
    uint8_t nCid;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_GprsManualAcc));

struct OPAR_CFW_GprsManualAcc
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_GprsManualAcc));

#define TAG_CFW_GprsManualRej 0xcc80601e
struct IPAR_CFW_GprsManualRej
{
    uint16_t nUTI;
    uint8_t nCid;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_GprsManualRej));

struct OPAR_CFW_GprsManualRej
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_GprsManualRej));

#define TAG_CFW_GprsCtxModify 0x8271116b
struct IPAR_CFW_GprsCtxModify
{
    uint16_t nUTI;
    uint8_t nCid;
    CFW_GPRS_QOS Qos;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_GprsCtxModify));

struct OPAR_CFW_GprsCtxModify
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_GprsCtxModify));

#define TAG_CFW_GprsCtxModifyAcc 0x7ab90486
struct IPAR_CFW_GprsCtxModifyAcc
{
    uint16_t nUTI;
    uint8_t nCid;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_GprsCtxModifyAcc));

struct OPAR_CFW_GprsCtxModifyAcc
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_GprsCtxModifyAcc));

#define TAG_CFW_GprsCtxModifyRej 0x75d9a654
struct IPAR_CFW_GprsCtxModifyRej
{
    uint16_t nUTI;
    uint8_t nCid;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_GprsCtxModifyRej));

struct OPAR_CFW_GprsCtxModifyRej
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_GprsCtxModifyRej));

#define TAG_CFW_GprsSendData 0xa66b1aa9
struct IPAR_CFW_GprsSendData
{
    uint8_t nCid;
    uint8_t rai;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_GprsSendData));

struct OPAR_CFW_GprsSendData
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_GprsSendData));

#define TAG_CFW_GprsAtt 0xe4cc5064
struct IPAR_CFW_GprsAtt
{
    uint8_t nState;
    uint16_t nUTI;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_GprsAtt));

struct OPAR_CFW_GprsAtt
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_GprsAtt));

#define TAG_CFW_GetGprsSum 0x105b539b
struct IPAR_CFW_GetGprsSum
{
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_GetGprsSum));

struct OPAR_CFW_GetGprsSum
{
    uint32_t result;
    int32_t upsum;
    int32_t downsum;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_GetGprsSum));

#define TAG_CFW_ClearGprsSum 0x25e3db36
struct IPAR_CFW_ClearGprsSum
{
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_ClearGprsSum));

struct OPAR_CFW_ClearGprsSum
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_ClearGprsSum));

#define TAG_CFW_GprsSetSmsSeviceMode 0xd21e74d0
struct IPAR_CFW_GprsSetSmsSeviceMode
{
    uint8_t nService;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_GprsSetSmsSeviceMode));

struct OPAR_CFW_GprsSetSmsSeviceMode
{
    uint8_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_GprsSetSmsSeviceMode));

#define TAG_CFW_GprsGetSmsSeviceMode 0xea3e6cae
struct OPAR_CFW_GprsGetSmsSeviceMode
{
    uint8_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_GprsGetSmsSeviceMode));

#define TAG_CFW_SetDnsServerbyPdp 0x27543604
struct IPAR_CFW_SetDnsServerbyPdp
{
    uint8_t nCid;
    uint8_t nSimID;
    uint8_t Pro_DnsIp[4];
    uint8_t Sec_DnsIp[4];
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_SetDnsServerbyPdp));

struct OPAR_CFW_SetDnsServerbyPdp
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_SetDnsServerbyPdp));

#define TAG_CFW_getDnsServerbyPdp 0x0425d22f
struct IPAR_CFW_getDnsServerbyPdp
{
    uint8_t nCid;
    uint8_t nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_getDnsServerbyPdp));

struct OPAR_CFW_getDnsServerbyPdp
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_getDnsServerbyPdp));

#define TAG_getDNSServer 0x6f20505a
struct IPAR_getDNSServer
{
    uint8_t nCid;
    uint8_t nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_getDNSServer));

struct OPAR_getDNSServer
{
    uint32_t* result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_getDNSServer));

#define TAG_CFW_EmodGetBatteryInfo 0x1a469219
struct OPAR_CFW_EmodGetBatteryInfo
{
    bool result;
    CFW_EMOD_BATTERY_INFO pBatInfo;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_EmodGetBatteryInfo));

#define TAG_CFW_EmodAudioTestStart 0x995d38e0
struct IPAR_CFW_EmodAudioTestStart
{
    uint8_t type;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_EmodAudioTestStart));

#define TAG_CFW_EmodAudioTestEnd 0xae454ab0
#define TAG_CFW_EmodClearUserInfo 0x63df1884
struct OPAR_CFW_EmodClearUserInfo
{
    bool result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_EmodClearUserInfo));

#define TAG_CFW_EmodGetIMEI 0x75bf0c54
struct IPAR_CFW_EmodGetIMEI
{
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_EmodGetIMEI));

struct OPAR_CFW_EmodGetIMEI
{
    uint8_t pImei[16];
    uint8_t pImeiLen;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_EmodGetIMEI));

#define TAG_CFW_EmodSaveIMEI 0x2c611403
struct IPAR_CFW_EmodSaveIMEI
{
    uint8_t pImeiLen;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_EmodSaveIMEI));

#define TAG_CFW_EmodGetNvVersion 0x0a1a9480
struct OPAR_CFW_EmodGetNvVersion
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_EmodGetNvVersion));

#define TAG_CFW_EmodGetNvData 0x9912b42b
struct IPAR_CFW_EmodGetNvData
{
    uint8_t dataType;
    uint8_t nSim;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_EmodGetNvData));

struct OPAR_CFW_EmodGetNvData
{
    uint8_t * result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_EmodGetNvData));

#define TAG_CFW_EmodSetNvData 0x727f5ca5
struct IPAR_CFW_EmodSetNvData
{
    uint8_t dataType;
    uint16_t offset;
    uint16_t dataLength;
    uint8_t nSim;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_EmodSetNvData));

struct OPAR_CFW_EmodSetNvData
{
    int32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_EmodSetNvData));

#define TAG_CFW_EmodSaveNvData 0x152320c3
struct IPAR_CFW_EmodSaveNvData
{
    uint8_t dataType;
    uint8_t nSim;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_EmodSaveNvData));

struct OPAR_CFW_EmodSaveNvData
{
    int32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_EmodSaveNvData));

#define TAG_CFW_EmodSpecialNvProcess 0x2994f59c
struct IPAR_CFW_EmodSpecialNvProcess
{
    uint8_t operType;
    uint8_t dataType;
    uint16_t offset;
    uint16_t dataLength;
    uint8_t nSim;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_EmodSpecialNvProcess));

struct OPAR_CFW_EmodSpecialNvProcess
{
    int32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_EmodSpecialNvProcess));

#define TAG_CFW_EmodEngineerCmd 0x0bb7f08e
struct IPAR_CFW_EmodEngineerCmd
{
    uint32_t module;
    uint32_t cmd;
    uint32_t v1;
    uint32_t v2;
    uint32_t v3;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_EmodEngineerCmd));

struct OPAR_CFW_EmodEngineerCmd
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_EmodEngineerCmd));

#define TAG_CFW_EmodReadBindSIM 0x82ccd1ad
struct OPAR_CFW_EmodReadBindSIM
{
    CFW_EMOD_READ_BIND_SIM pReadBindSim;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_EmodReadBindSIM));

#define TAG_CFW_EmodReadBindNW 0xa6eed8f0
struct OPAR_CFW_EmodReadBindNW
{
    CFW_EMOD_READ_BIND_NW pReadBindNw;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_EmodReadBindNW));

#define TAG_CFW_EmodGetBindInfo 0x5eb87c1b
struct OPAR_CFW_EmodGetBindInfo
{
    bool bSim;
    bool bNetWork;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_EmodGetBindInfo));

#define TAG_CFW_EmodSetBindInfo 0x44bf3c87
struct IPAR_CFW_EmodSetBindInfo
{
    bool bSim;
    bool bNetWork;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_EmodSetBindInfo));

struct OPAR_CFW_EmodSetBindInfo
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_EmodSetBindInfo));

#define TAG_CFW_EmodUpdateSimBind 0xf6a7246e
struct IPAR_CFW_EmodUpdateSimBind
{
    uint8_t nIndex;
    uint8_t nLen;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_EmodUpdateSimBind));

struct OPAR_CFW_EmodUpdateSimBind
{
    CFW_EMOD_UPDATE_RETURN result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_EmodUpdateSimBind));

#define TAG_CFW_EmodUpdateNwBind 0x1bcf7121
struct IPAR_CFW_EmodUpdateNwBind
{
    uint8_t nIndex;
    uint8_t nLen;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_EmodUpdateNwBind));

struct OPAR_CFW_EmodUpdateNwBind
{
    CFW_EMOD_UPDATE_RETURN result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_EmodUpdateNwBind));

#define TAG_CFW_EmodGetProductInfo 0x6b0b2278
struct OPAR_CFW_EmodGetProductInfo
{
    CFW_EMOD_LOCT_PRODUCT_INFO pProduct;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_EmodGetProductInfo));

#define TAG_CFW_EmodGetRFCalibrationInfo 0xcbb6d554
struct OPAR_CFW_EmodGetRFCalibrationInfo
{
    CFW_EMOD_RF_CALIB_INFO pRfCalib;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_EmodGetRFCalibrationInfo));

#define TAG_CFW_EmodOutfieldTestStart 0x972830bc
struct IPAR_CFW_EmodOutfieldTestStart
{
    CFW_TSM_FUNCTION_SELECT pSelecFUN;
    uint16_t nUTI;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_EmodOutfieldTestStart));

struct OPAR_CFW_EmodOutfieldTestStart
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_EmodOutfieldTestStart));

#define TAG_CFW_EmodOutfieldTestEnd 0xffea09ab
struct IPAR_CFW_EmodOutfieldTestEnd
{
    uint16_t nUTI;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_EmodOutfieldTestEnd));

struct OPAR_CFW_EmodOutfieldTestEnd
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_EmodOutfieldTestEnd));

#define TAG_CFW_GetCellInfo 0xc361faa4
struct IPAR_CFW_GetCellInfo
{
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_GetCellInfo));

struct OPAR_CFW_GetCellInfo
{
    uint32_t result;
    CFW_TSM_CURR_CELL_INFO pCurrCellInfo;
    CFW_TSM_ALL_NEBCELL_INFO pNeighborCellInfo;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_GetCellInfo));

#define TAG_CFW_EmodSyncInfoTest 0x471bc7fc
struct IPAR_CFW_EmodSyncInfoTest
{
    bool bStart;
    uint16_t nUTI;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_EmodSyncInfoTest));

struct OPAR_CFW_EmodSyncInfoTest
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_EmodSyncInfoTest));

#define TAG_CFW_GetFreeCID 0x0a402eac
struct IPAR_CFW_GetFreeCID
{
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_GetFreeCID));

struct OPAR_CFW_GetFreeCID
{
    uint32_t result;
    uint8_t pCID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_GetFreeCID));

#define TAG_CFW_ReleaseCID 0xf1dcf009
struct IPAR_CFW_ReleaseCID
{
    uint8_t nCID;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_ReleaseCID));

struct OPAR_CFW_ReleaseCID
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_ReleaseCID));

#define TAG_CFW_SetCID 0xc921d725
struct IPAR_CFW_SetCID
{
    uint8_t nCID;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_SetCID));

struct OPAR_CFW_SetCID
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_SetCID));

#define TAG_CFW_isPdpActiveAvailable 0x641e4ad6
struct IPAR_CFW_isPdpActiveAvailable
{
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_isPdpActiveAvailable));

struct OPAR_CFW_isPdpActiveAvailable
{
    uint8_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_isPdpActiveAvailable));

#define TAG_cfw_SimReadElementaryFile 0xf15912cb
struct IPAR_cfw_SimReadElementaryFile
{
    uint16_t nUTI;
    uint32_t fileId;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_cfw_SimReadElementaryFile));

struct OPAR_cfw_SimReadElementaryFile
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_cfw_SimReadElementaryFile));

#define TAG_CFW_SimGetACMMax 0x5447da9e
struct IPAR_CFW_SimGetACMMax
{
    uint16_t nUTI;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_SimGetACMMax));

struct OPAR_CFW_SimGetACMMax
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_SimGetACMMax));

#define TAG_CFW_SimSetACMMax 0x1b686aea
struct IPAR_CFW_SimSetACMMax
{
    uint32_t iACMMaxValue;
    uint8_t nPinSize;
    uint16_t Nuti;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_SimSetACMMax));

struct OPAR_CFW_SimSetACMMax
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_SimSetACMMax));

#define TAG_CFW_SimGetACM 0xba3d62fe
struct IPAR_CFW_SimGetACM
{
    uint16_t nUTI;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_SimGetACM));

struct OPAR_CFW_SimGetACM
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_SimGetACM));

#define TAG_CFW_SimSetACM 0x95e61408
struct IPAR_CFW_SimSetACM
{
    uint32_t iCurValue;
    uint8_t nPinSize;
    uint16_t Nuti;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_SimSetACM));

struct OPAR_CFW_SimSetACM
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_SimSetACM));

#define TAG_CFW_SimGetPUCT 0xd1853aaf
struct IPAR_CFW_SimGetPUCT
{
    uint16_t nUTI;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_SimGetPUCT));

struct OPAR_CFW_SimGetPUCT
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_SimGetPUCT));

#define TAG_CFW_SimSetPUCT 0xf85926dc
struct IPAR_CFW_SimSetPUCT
{
    CFW_SIM_PUCT_INFO pPUCT;
    uint8_t nPinSize;
    uint16_t nUTI;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_SimSetPUCT));

struct OPAR_CFW_SimSetPUCT
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_SimSetPUCT));

#define TAG_CFW_EmodSetLteLockFreq 0xee64a452
struct IPAR_CFW_EmodSetLteLockFreq
{
    uint8_t number;
    CFW_SIM_ID nSimId;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_EmodSetLteLockFreq));

#define TAG_CFW_EmodGetLteLockFreq 0xab7975c4
struct IPAR_CFW_EmodGetLteLockFreq
{
    uint32_t freq[9];
    CFW_SIM_ID nSimId;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_EmodGetLteLockFreq));

struct OPAR_CFW_EmodGetLteLockFreq
{
    uint8_t number;
    uint32_t freq[9];
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_EmodGetLteLockFreq));

#define TAG_CFW_EmodSetLteLockBand 0x036f213c
struct IPAR_CFW_EmodSetLteLockBand
{
    uint8_t number;
    CFW_SIM_ID nSimId;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_EmodSetLteLockBand));

#define TAG_CFW_EmodGetLteLockBand 0x95cd0c5f
struct IPAR_CFW_EmodGetLteLockBand
{
    CFW_SIM_ID nSimId;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_EmodGetLteLockBand));

struct OPAR_CFW_EmodGetLteLockBand
{
    uint8_t number;
    uint16_t band[20];
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_EmodGetLteLockBand));

#define TAG_CFW_EmodGetCEMode 0x6b687af8
struct IPAR_CFW_EmodGetCEMode
{
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_EmodGetCEMode));

struct OPAR_CFW_EmodGetCEMode
{
    uint8_t pMode;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_EmodGetCEMode));

#define TAG_CFW_EmodSetCEMode 0x6f36868c
struct IPAR_CFW_EmodSetCEMode
{
    uint8_t nMode;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_EmodSetCEMode));

#define TAG_CFW_EmodGetVoicePrefer 0xccbe3de8
struct IPAR_CFW_EmodGetVoicePrefer
{
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_EmodGetVoicePrefer));

struct OPAR_CFW_EmodGetVoicePrefer
{
    uint8_t pVoicePrefer;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_EmodGetVoicePrefer));

#define TAG_CFW_EmodSetVoicePrefer 0xb9a37a07
struct IPAR_CFW_EmodSetVoicePrefer
{
    uint8_t nVoicePrefer;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_EmodSetVoicePrefer));

#define TAG_CFW_SetPollingTimer 0x42aa254a
struct IPAR_CFW_SetPollingTimer
{
    uint8_t PollingInterval;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_SetPollingTimer));

#define TAG_CFW_GetPollingTimer 0x6b497865
struct IPAR_CFW_GetPollingTimer
{
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_GetPollingTimer));

struct OPAR_CFW_GetPollingTimer
{
    uint8_t PollingInterval;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_GetPollingTimer));

#define TAG_CSW_SetAudioCodecCalibParam 0x8855397e
struct IPAR_CSW_SetAudioCodecCalibParam
{
    uint8_t nMode;
    uint8_t nPath;
    uint8_t nCtrl;
    uint16_t nParamLength;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CSW_SetAudioCodecCalibParam));

struct OPAR_CSW_SetAudioCodecCalibParam
{
    uint32_t result;
    uint8_t resultcode;
    uint8_t hasMsg;
    uint8_t resultMsg[900];
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CSW_SetAudioCodecCalibParam));

#define TAG_CSW_SetAudioZspVqeCalibParam 0x03d69255
struct IPAR_CSW_SetAudioZspVqeCalibParam
{
    uint8_t nMode;
    uint8_t nPath;
    uint8_t nCtrl;
    uint16_t nParamLength;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CSW_SetAudioZspVqeCalibParam));

struct OPAR_CSW_SetAudioZspVqeCalibParam
{
    uint32_t result;
    uint8_t resultcode;
    uint8_t hasMsg;
    uint8_t resultMsg[900];
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CSW_SetAudioZspVqeCalibParam));

#define TAG_CSW_SetAudioZspPreProcessCalibParam 0x2ebfacc0
struct IPAR_CSW_SetAudioZspPreProcessCalibParam
{
    uint8_t nMode;
    uint8_t nPath;
    uint8_t nCtrl;
    uint16_t nParamLength;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CSW_SetAudioZspPreProcessCalibParam));

struct OPAR_CSW_SetAudioZspPreProcessCalibParam
{
    uint32_t result;
    uint8_t resultcode;
    uint8_t hasMsg;
    uint8_t resultMsg[900];
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CSW_SetAudioZspPreProcessCalibParam));

#define TAG_CSW_SetAudioZspPostProcessCalibParam 0xbc760df9
struct IPAR_CSW_SetAudioZspPostProcessCalibParam
{
    uint8_t nMode;
    uint8_t nPath;
    uint8_t nCtrl;
    uint16_t nParamLength;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CSW_SetAudioZspPostProcessCalibParam));

struct OPAR_CSW_SetAudioZspPostProcessCalibParam
{
    uint32_t result;
    uint8_t resultcode;
    uint8_t hasMsg;
    uint8_t resultMsg[900];
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CSW_SetAudioZspPostProcessCalibParam));

#define TAG_CSW_WriteCalibParamToFlash 0x07600c35
struct IPAR_CSW_WriteCalibParamToFlash
{
    uint8_t nPath;
    uint8_t nCtrl;
    uint16_t nParamLength;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CSW_WriteCalibParamToFlash));

struct OPAR_CSW_WriteCalibParamToFlash
{
    uint32_t result;
    uint8_t resultcode;
    uint8_t hasMsg;
    uint8_t resultMsg[900];
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CSW_WriteCalibParamToFlash));

#define TAG_CSW_ExportCalibOrImportCalibFlashParam 0x725848a9
struct IPAR_CSW_ExportCalibOrImportCalibFlashParam
{
    uint8_t nPath;
    uint8_t nCtrl;
    uint16_t nOffset;
    uint16_t nlength;
    uint16_t nParamLength;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CSW_ExportCalibOrImportCalibFlashParam));

struct OPAR_CSW_ExportCalibOrImportCalibFlashParam
{
    uint32_t result;
    uint8_t resultcode;
    uint8_t hasMsg;
    uint8_t resultMsg[900];
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CSW_ExportCalibOrImportCalibFlashParam));

#define TAG_CSW_DumpPcmDataToTflash 0x1f016809
struct IPAR_CSW_DumpPcmDataToTflash
{
    uint8_t nPath;
    uint8_t nCtrl;
    uint16_t nParamLength;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CSW_DumpPcmDataToTflash));

struct OPAR_CSW_DumpPcmDataToTflash
{
    uint32_t result;
    uint8_t resultcode;
    uint8_t hasMsg;
    uint8_t resultMsg[900];
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CSW_DumpPcmDataToTflash));

#define TAG_CSW_VersionControl 0x717536f2
struct IPAR_CSW_VersionControl
{
    uint8_t nPath;
    uint8_t nCtrl;
    uint16_t nParamLength;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CSW_VersionControl));

struct OPAR_CSW_VersionControl
{
    uint32_t result;
    uint8_t resultcode;
    uint8_t hasMsg;
    uint8_t resultMsg[900];
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CSW_VersionControl));

#define TAG_CSW_SetAudioZspNxpCalibParam 0xec6140b2
struct IPAR_CSW_SetAudioZspNxpCalibParam
{
    uint8_t nMode;
    uint8_t nPath;
    uint8_t nCtrl;
    uint16_t nParamLength;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CSW_SetAudioZspNxpCalibParam));

struct OPAR_CSW_SetAudioZspNxpCalibParam
{
    uint32_t result;
    uint8_t resultcode;
    uint8_t hasMsg;
    uint8_t resultMsg[900];
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CSW_SetAudioZspNxpCalibParam));

#define TAG_CFW_NwSetFTA 0xdcd3a5a9
struct IPAR_CFW_NwSetFTA
{
    uint8_t nFta;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_NwSetFTA));

struct OPAR_CFW_NwSetFTA
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_NwSetFTA));

#define TAG_CFW_NwSetnSecurityUsedFlag 0xfbb6e127
struct IPAR_CFW_NwSetnSecurityUsedFlag
{
    uint8_t nSecurityUsedFlag;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_NwSetnSecurityUsedFlag));

struct OPAR_CFW_NwSetnSecurityUsedFlag
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_NwSetnSecurityUsedFlag));

#define TAG_CFW_GprsIpDnsInfo 0x88c37c19
struct IPAR_CFW_GprsIpDnsInfo
{
    uint8_t nCid;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_GprsIpDnsInfo));

struct OPAR_CFW_GprsIpDnsInfo
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_GprsIpDnsInfo));

#define TAG_CFW_GprsClrIpDns 0xeb30b3e0
struct OPAR_CFW_GprsClrIpDns
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_GprsClrIpDns));

#define TAG_hal_WriteGsmCalib 0x81c945ab
#define TAG_hal_WriteLteCalib 0xf6723896
#define TAG_CFW_CcCallMultipartyFunction 0x59557ac7
struct IPAR_CFW_CcCallMultipartyFunction
{
    uint8_t nFun;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_CcCallMultipartyFunction));

struct OPAR_CFW_CcCallMultipartyFunction
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_CcCallMultipartyFunction));

#define TAG_CFW_SimReadVoiceMailInfo 0xcb66a059
struct IPAR_CFW_SimReadVoiceMailInfo
{
    uint8_t nLineId;
    uint8_t nType;
    uint16_t nUTI;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_SimReadVoiceMailInfo));

struct OPAR_CFW_SimReadVoiceMailInfo
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_SimReadVoiceMailInfo));

#define TAG_CFW_SimUpdateVoiceMailInfo 0x1a63f26a
struct IPAR_CFW_SimUpdateVoiceMailInfo
{
    uint8_t nLineId;
    uint8_t nType;
    CFW_SIM_INFO_VOICEMAIL pVoiceMail;
    uint16_t nUTI;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_SimUpdateVoiceMailInfo));

struct OPAR_CFW_SimUpdateVoiceMailInfo
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_SimUpdateVoiceMailInfo));

#define TAG_CFW_CbSetContext 0xef062af4
struct IPAR_CFW_CbSetContext
{
    uint8_t CtxData[sizeof(CB_CTX_T)];
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_CbSetContext));

struct OPAR_CFW_CbSetContext
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_CbSetContext));

#define TAG_CFW_CbSetType 0x04f77c3d
struct IPAR_CFW_CbSetType
{
    uint8_t nCbType;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_CbSetType));

struct OPAR_CFW_CbSetType
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_CbSetType));

#define TAG_CFW_CbAct 0xd02d8d7a
struct IPAR_CFW_CbAct
{
    uint16_t nUTI;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_CbAct));

struct OPAR_CFW_CbAct
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_CbAct));

#define TAG_CFW_CbDeAct 0x7d7969f5
struct IPAR_CFW_CbDeAct
{
    uint16_t nUTI;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_CbDeAct));

struct OPAR_CFW_CbDeAct
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_CbDeAct));

#define TAG_CFW_SetPowerOffFlag 0xd5d4a325
struct IPAR_CFW_SetPowerOffFlag
{
    uint8_t bOff;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_SetPowerOffFlag));

struct OPAR_CFW_SetPowerOffFlag
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_SetPowerOffFlag));

#define TAG_CFW_NwSysConfig 0x3e65984a
struct IPAR_CFW_NwSysConfig
{
    uint8_t nMode;
    uint8_t nAcqorder;
    uint8_t nRoam;
    uint8_t nSrvdomain;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_NwSysConfig));

struct OPAR_CFW_NwSysConfig
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_NwSysConfig));

#define TAG_CFW_NwGetSysConfig 0xcfe5dd7b
struct IPAR_CFW_NwGetSysConfig
{
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_NwGetSysConfig));

struct OPAR_CFW_NwGetSysConfig
{
    uint32_t result;
    uint8_t pMode;
    uint8_t pAcqorder;
    uint8_t pRoam;
    uint8_t pSrvdomain;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_NwGetSysConfig));

#define TAG_CFW_SimGetSmsParameters 0xa6965bff
struct IPAR_CFW_SimGetSmsParameters
{
    uint8_t nIndex;
    uint16_t nUTI;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_SimGetSmsParameters));

struct OPAR_CFW_SimGetSmsParameters
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_SimGetSmsParameters));

#define TAG_CFW_SimSetSmsParameters 0xd0b065ed
struct IPAR_CFW_SimSetSmsParameters
{
    CFW_SIM_SMS_PARAMETERS pSmsParamArray;
    uint8_t nArrayCount;
    uint8_t nIndex;
    uint16_t nUTI;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_SimSetSmsParameters));

struct OPAR_CFW_SimSetSmsParameters
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_SimSetSmsParameters));

#define TAG_CFW_SimDeleteMessage 0xf4b2c147
struct IPAR_CFW_SimDeleteMessage
{
    uint16_t nLocation;
    uint16_t nIndex;
    uint16_t nStatus;
    uint16_t nUTI;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_SimDeleteMessage));

struct OPAR_CFW_SimDeleteMessage
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_SimDeleteMessage));

#define TAG_CFW_EmodSpLteDummyPara 0x2689cabc
struct IPAR_CFW_EmodSpLteDummyPara
{
    CFW_EmodSpLteDummyPara_t para;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_EmodSpLteDummyPara));

struct OPAR_CFW_EmodSpLteDummyPara
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_EmodSpLteDummyPara));

#define TAG_CFW_EmodRrtmParam 0xef959240
struct IPAR_CFW_EmodRrtmParam
{
    CFW_EmodRrtmParam_t para;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_EmodRrtmParam));

struct OPAR_CFW_EmodRrtmParam
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_EmodRrtmParam));

#define TAG_CFW_EmodGrrLteFreq 0xb3b37874
struct IPAR_CFW_EmodGrrLteFreq
{
    CFW_EmodGrrLteFreq_t para;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_EmodGrrLteFreq));

struct OPAR_CFW_EmodGrrLteFreq
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_EmodGrrLteFreq));

#define TAG_CFW_EmodRrdmParam 0x307d94f2
struct IPAR_CFW_EmodRrdmParam
{
    CFW_EmodRrdmParam_t para;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_EmodRrdmParam));

struct OPAR_CFW_EmodRrdmParam
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_EmodRrdmParam));

#define TAG_CFW_EmodL1Param 0xc69c0723
struct IPAR_CFW_EmodL1Param
{
    CFW_EmodL1Param_t para;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_EmodL1Param));

struct OPAR_CFW_EmodL1Param
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_EmodL1Param));

#define TAG_CFW_EmodSpCleanInfo 0x21813e4a
struct IPAR_CFW_EmodSpCleanInfo
{
    CFW_EmodSpCleanInfo_t para;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_EmodSpCleanInfo));

struct OPAR_CFW_EmodSpCleanInfo
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_EmodSpCleanInfo));

#define TAG_CFW_EmodGetGsmCalibInfo 0xe8f8e512
struct OPAR_CFW_EmodGetGsmCalibInfo
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_EmodGetGsmCalibInfo));

#define TAG_CFW_EmodGetLteCalibInfo 0x5c3b6a0e
struct OPAR_CFW_EmodGetLteCalibInfo
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_EmodGetLteCalibInfo));

#define TAG_CFW_NwSetXCAP 0xfd41ebd7
struct IPAR_CFW_NwSetXCAP
{
    uint32_t nLength;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_NwSetXCAP));

struct OPAR_CFW_NwSetXCAP
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_NwSetXCAP));

#define TAG_CFW_NwSetBSF 0xfb1a71d8
struct IPAR_CFW_NwSetBSF
{
    uint32_t nLength;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_NwSetBSF));

struct OPAR_CFW_NwSetBSF
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_NwSetBSF));

#define TAG_CFW_EmodSetTracePort 0xb17252a7
struct IPAR_CFW_EmodSetTracePort
{
    uint8_t port;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_EmodSetTracePort));

#define TAG_CFW_ImsSrvccCapSet 0x8fdc72bf
struct IPAR_CFW_ImsSrvccCapSet
{
    uint8_t bSupport;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_ImsSrvccCapSet));

struct OPAR_CFW_ImsSrvccCapSet
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_ImsSrvccCapSet));

#define TAG_CFW_ImsGetSrvccCap 0x2f976c7e
struct IPAR_CFW_ImsGetSrvccCap
{
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_ImsGetSrvccCap));

struct OPAR_CFW_ImsGetSrvccCap
{
    uint8_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_ImsGetSrvccCap));



#define TAG_CFW_NwChangeRat 0x96567f1f
struct IPAR_CFW_NwChangeRat
{
    uint8_t nRat;
    uint16_t nUTI;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_NwChangeRat));

struct OPAR_CFW_NwChangeRat
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_NwChangeRat));

#define TAG_CFW_GetNetinfo 0x01a48c19
struct IPAR_CFW_GetNetinfo
{
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_GetNetinfo));

struct OPAR_CFW_GetNetinfo
{
    uint32_t result;
    CFW_NET_INFO_T pNetinfo;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_GetNetinfo));

#define TAG_CFW_GetLETRealTimeInfo 0x66010143
struct IPAR_CFW_GetLETRealTimeInfo
{
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_GetLETRealTimeInfo));

struct OPAR_CFW_GetLETRealTimeInfo
{
    uint32_t result;
    CFW_NET_INFO_T pNetinfo;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_GetLETRealTimeInfo));

#define TAG_CFW_GetNetinfoStatistic 0xdf39cd5c
struct IPAR_CFW_GetNetinfoStatistic
{
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_GetNetinfoStatistic));

struct OPAR_CFW_GetNetinfoStatistic
{
    uint32_t result;
    CFW_NET_INFO_STATISTIC_T pNetinfoStatistic;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_GetNetinfoStatistic));

#define TAG_CFW_SetInitWithSim 0x9a24d26a
struct IPAR_CFW_SetInitWithSim
{
    bool bWith;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_SetInitWithSim));

#define TAG_CFW_GetSimSPN 0xbdd83a6a
struct IPAR_CFW_GetSimSPN
{
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_GetSimSPN));

struct OPAR_CFW_GetSimSPN
{
    uint32_t result;
    CFW_SPN_INFO pSPNInfo;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_GetSimSPN));

#define TAG_CFW_CfgSetAntenna 0x8dae8ade
struct IPAR_CFW_CfgSetAntenna
{
    uint16_t len;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_CfgSetAntenna));

struct OPAR_CFW_CfgSetAntenna
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_CfgSetAntenna));

#define TAG_CFW_ImsEnVolte 0x17c0fa00
struct IPAR_CFW_ImsEnVolte
{
    uint8_t bEn;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_ImsEnVolte));

struct OPAR_CFW_ImsEnVolte
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_ImsEnVolte));

#define TAG_CFW_SimGetStatus 0xbdaa958e
struct IPAR_CFW_SimGetStatus
{
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_SimGetStatus));

struct OPAR_CFW_SimGetStatus
{
    CFW_SIMSTATUS result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_SimGetStatus));

#define TAG_CFW_SimReadFiles 0x2a7cf76d
struct IPAR_CFW_SimReadFiles
{
    uint8_t nSimFile;
    uint16_t nUTI;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_SimReadFiles));

struct OPAR_CFW_SimReadFiles
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_SimReadFiles));

#define TAG_CFW_NwSetLockBCCH 0x8f8129ef
struct IPAR_CFW_NwSetLockBCCH
{
    bool bLock;
    uint16_t nUTI;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_NwSetLockBCCH));

struct OPAR_CFW_NwSetLockBCCH
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_NwSetLockBCCH));

#define TAG_CFW_NwGetLockBCCH 0x9052629e
struct OPAR_CFW_NwGetLockBCCH
{
    bool result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_NwGetLockBCCH));

#define TAG_CFW_SwitchUSBShareNetwork 0x08761de0
struct IPAR_CFW_SwitchUSBShareNetwork
{
    bool flag;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_SwitchUSBShareNetwork));

struct OPAR_CFW_SwitchUSBShareNetwork
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_SwitchUSBShareNetwork));

#define TAG_CFW_SatRefreshFileChange 0xf95de818
struct IPAR_CFW_SatRefreshFileChange
{
    CFW_SAT_REFRESH_FILE_CHG_REQ_T FileChangeReq;
    uint16_t nUTI;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_SatRefreshFileChange));
struct OPAR_CFW_SatRefreshFileChange
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_SatRefreshFileChange));


#define TAG_CFW_SatRefreshImsiChange 0xe0991e80
struct IPAR_CFW_SatRefreshImsiChange
{
    uint16_t nUTI;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_SatRefreshImsiChange));

struct OPAR_CFW_SatRefreshImsiChange
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_SatRefreshImsiChange));

#define TAG_CFW_SSCheckTestSim 0x0c87edd1
struct IPAR_CFW_SSCheckTestSim
{
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_SSCheckTestSim))

struct OPAR_CFW_SSCheckTestSim
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_SSCheckTestSim));
#define TAG_CFW_SimReset 0x7e6dec45
struct IPAR_CFW_SimReset
{
    uint16_t nUTI;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_SimReset));

struct OPAR_CFW_SimReset
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_SimReset));
#define TAG_CFW_CcAcceptSpeechCallEx 0x9a1144fe
struct IPAR_CFW_CcAcceptSpeechCallEx
{
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_CcAcceptSpeechCallEx));

struct OPAR_CFW_CcAcceptSpeechCallEx
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_CcAcceptSpeechCallEx));

#define TAG_CFW_SmsMtSmsPPAckReq 0x0e138a39
struct IPAR_CFW_SmsMtSmsPPAckReq
{
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_SmsMtSmsPPAckReq));

struct OPAR_CFW_SmsMtSmsPPAckReq
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_SmsMtSmsPPAckReq));

#define TAG_CFW_SmsMemoryFullReq 0xd7655a97
struct IPAR_CFW_SmsMemoryFullReq
{
    uint8_t nCause;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_SmsMemoryFullReq));

struct OPAR_CFW_SmsMemoryFullReq
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_SmsMemoryFullReq));

#define TAG_CFW_SmsMMAReq 0x9ae2bc1a
struct IPAR_CFW_SmsMMAReq
{
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_SmsMMAReq));

struct OPAR_CFW_SmsMMAReq
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_SmsMMAReq));

#define TAG_CFW_NwGetFTA 0xc37f54c0
struct IPAR_CFW_NwGetFTA
{
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_NwGetFTA));

struct OPAR_CFW_NwGetFTA
{
    uint8_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_NwGetFTA));

#define TAG_CFW_NwSetAutoAttachFlag 0x8beea828
struct IPAR_CFW_NwSetAutoAttachFlag
{
    uint8_t flag;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_NwSetAutoAttachFlag));

struct OPAR_CFW_NwSetAutoAttachFlag
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_NwSetAutoAttachFlag));

#define TAG_CFW_NwGetAutoAttachFlag 0x7a4fe641
struct IPAR_CFW_NwGetAutoAttachFlag
{
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_NwGetAutoAttachFlag));

struct OPAR_CFW_NwGetAutoAttachFlag
{
    uint8_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_NwGetAutoAttachFlag));

#define TAG_CFW_GprsSetInternalPdpCtx 0x14aedc7b
struct IPAR_CFW_GprsSetInternalPdpCtx
{
    CFW_INTERNAL_PDP_CTX_TYPE nType;
    CFW_GPRS_PDPCONT_INFO_V2 pPdpCont;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_GprsSetInternalPdpCtx));

struct OPAR_CFW_GprsSetInternalPdpCtx
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_GprsSetInternalPdpCtx));

#define TAG_CFW_EmodImsNvParamSet 0x2f55a10e
struct IPAR_CFW_EmodImsNvParamSet
{
    uint16_t param_id;
    uint16_t value_len;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_EmodImsNvParamSet));

struct OPAR_CFW_EmodImsNvParamSet
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_EmodImsNvParamSet));

#define TAG_CFW_EmodImsNvParamGet 0xa0c22862
struct IPAR_CFW_EmodImsNvParamGet
{
    uint16_t param_id;
    uint8_t value_ptr[256];
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_EmodImsNvParamGet));

struct OPAR_CFW_EmodImsNvParamGet
{
    uint32_t result;
    uint8_t value_ptr[256];
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_EmodImsNvParamGet));

#define TAG_CFW_ImsSetUaVersionInfo 0xd182b1b4
struct IPAR_CFW_ImsSetUaVersionInfo
{
    uint8_t nLen;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_ImsSetUaVersionInfo));


#define TAG_CFW_EmodResetImsDelta 0x931bd91a
struct OPAR_CFW_EmodResetImsDelta
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_EmodResetImsDelta));

#define TAG_CFW_CcEmcDialEx_V2 0x80243b3f
struct IPAR_CFW_CcEmcDialEx_V2
{
    uint8_t nDialNumberSize;
    CFW_CcEmergCateg emergCateg;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_CcEmcDialEx_V2));

struct OPAR_CFW_CcEmcDialEx_V2
{
    uint32_t result;
    uint8_t pIndex;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_CcEmcDialEx_V2));


#define TAG_CFW_SetGcfTest 0x994da05e
struct IPAR_CFW_SetGcfTest
{
    CFW_SIM_ID nSimID;
    bool nFlag;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_SetGcfTest));

struct OPAR_CFW_SetGcfTest
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_SetGcfTest));

#define TAG_CFW_NwGetLteSignalQuality 0x81fd8a88
struct IPAR_CFW_NwGetLteSignalQuality
{
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_NwGetLteSignalQuality));

struct OPAR_CFW_NwGetLteSignalQuality
{
    uint32_t result;
    uint8_t pSignalLevel;
    uint8_t pBitError;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_NwGetLteSignalQuality));

#define TAG_CFW_NwGetQualReport 0x8291ead7
struct IPAR_CFW_NwGetQualReport
{
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_NwGetQualReport));

struct OPAR_CFW_NwGetQualReport
{
    uint32_t result;
    CFW_NW_QUAL_INFO pQualReport;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_NwGetQualReport));

#define TAG_CFW_SimSetVirtualFlag 0xd9b805ba
struct IPAR_CFW_SimSetVirtualFlag
{
    uint8_t flag;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_SimSetVirtualFlag));

struct OPAR_CFW_SimSetVirtualFlag
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_SimSetVirtualFlag));

#define TAG_CFW_SetRRCRel 0x7121b58d
struct IPAR_CFW_SetRRCRel
{
    uint32_t value;
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_SetRRCRel));

struct OPAR_CFW_SetRRCRel
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_SetRRCRel));

#define TAG_CFW_GetRRCRel 0x4ace1ca9
struct IPAR_CFW_GetRRCRel
{
    CFW_SIM_ID nSimID;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct IPAR_CFW_GetRRCRel));

struct OPAR_CFW_GetRRCRel
{
    uint32_t result;
} ALIGNED(8);
CHECK_IPAR_SIZE(sizeof(struct OPAR_CFW_GetRRCRel));

// AUTO GENERATED END
