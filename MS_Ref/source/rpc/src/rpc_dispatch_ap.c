// AUTO GENERATED

extern void *bsearch(const void *key, const void *base,
                     unsigned nmemb, unsigned size,
                     int (*compar)(const void *, const void *));

typedef struct
{
    unsigned tag;
    void *function;
} tagFunction_t;

typedef struct
{
    unsigned tag;
    const char *name;
} tagName_t;

static int _tagCompare(const void *key, const void *p)
{
    unsigned tag = (unsigned)(unsigned long)key;
    const tagFunction_t *ct = (const tagFunction_t *)p;
    return (tag > ct->tag) ? 1 : (tag < ct->tag) ? -1 : 0;
}

#define DECLARE_API(api) void api(void)

DECLARE_API(RPC_CFW_GetIMEIFromAP);
DECLARE_API(RPC_rpcHelloOnAP);
DECLARE_API(RPC_AUD_ToneStopEnd);
DECLARE_API(RPC_AUD_SetCodecOpStatus);

static const tagFunction_t gRpcApiTable[] = {
    {0x10ead86d, RPC_AUD_ToneStopEnd},
    {0x62e05df9, RPC_CFW_GetIMEIFromAP},
    {0x63ad8ff5, RPC_rpcHelloOnAP},
    {0xc070ad19, RPC_AUD_SetCodecOpStatus},
};

void *rpcFindFunctionByTag(unsigned tag)
{
    void *p = bsearch((const void *)(unsigned long)tag,
                      gRpcApiTable,
                      sizeof(gRpcApiTable) / sizeof(gRpcApiTable[0]),
                      sizeof(gRpcApiTable[0]),
                      _tagCompare);

    return (p != (void *)0) ? ((tagFunction_t *)p)->function : (void *)0;
}


void *rpcFindEventSender(unsigned id) { return (void *)0; }

DECLARE_API(RECV_EV_CFW_NW_SET_REGISTRATION_RSP);
DECLARE_API(RECV_EV_CFW_NW_GET_AVAIL_OPERATOR_RSP);
DECLARE_API(RECV_EV_CFW_NW_GET_QSCANF_RSP);
DECLARE_API(RECV_EV_CFW_CC_INITIATE_SPEECH_CALL_RSP);
DECLARE_API(RECV_EV_CFW_SS_SET_CALL_FORWARDING_RSP);
DECLARE_API(RECV_EV_CFW_SS_QUERY_CALL_FORWARDING_RSP);
DECLARE_API(RECV_EV_CFW_SS_SEND_USSD_RSP);
DECLARE_API(RECV_EV_CFW_SIM_GET_PBK_ENTRY_RSP);
DECLARE_API(RECV_EV_CFW_SIM_GET_PBK_STRORAGE_INFO_RSP);
DECLARE_API(RECV_EV_CFW_SIM_GET_PROVIDER_ID_RSP);
DECLARE_API(RECV_EV_CFW_SIM_READ_MESSAGE_RSP);
DECLARE_API(RECV_EV_CFW_SIM_LIST_MESSAGE_RSP);
DECLARE_API(RECV_EV_CFW_SIM_READ_BINARY_RSP);
DECLARE_API(RECV_EV_CFW_SIM_GET_SMS_PARAMETERS_RSP);
DECLARE_API(RECV_EV_CFW_SAT_RESPONSE_RSP);
DECLARE_API(RECV_EV_CFW_SIM_GET_PREF_OPERATOR_LIST_RSP);
DECLARE_API(RECV_EV_CFW_SIM_GET_FILE_STATUS_RSP);
DECLARE_API(RECV_EV_CFW_SIM_READ_RECORD_RSP);
DECLARE_API(RECV_EV_CFW_SIM_READ_VOICEMIAL_RSP);
DECLARE_API(RECV_EV_CFW_SIM_READ_FILES_RSP);
DECLARE_API(RECV_EV_CFW_SMS_READ_MESSAGE_RSP);
DECLARE_API(RECV_EV_CFW_SAT_REFRESH_FILE_CHG_RSP);
DECLARE_API(RECV_EV_CFW_CC_SPEECH_CALL_IND);
DECLARE_API(RECV_EV_CFW_NEW_SMS_IND);
DECLARE_API(RECV_EV_CFW_SS_USSD_IND);
DECLARE_API(RECV_EV_CFW_SAT_CMDTYPE_IND);
DECLARE_API(RECV_EV_CFW_NW_NETWORKINFO_IND);
DECLARE_API(RECV_EV_CFW_GPRS_DATA_IND);
DECLARE_API(RECV_EV_CFW_CB_MSG_IND);
DECLARE_API(RECV_EV_CFW_CB_PAGE_IND);
DECLARE_API(RECV_EV_CFW_EMC_NUM_LIST_IND);

static const tagFunction_t gRpcEventRecvTable[] = {
    {0x000001f8, RECV_EV_CFW_NW_SET_REGISTRATION_RSP},
    {0x000001fa, RECV_EV_CFW_NW_GET_AVAIL_OPERATOR_RSP},
    {0x00000200, RECV_EV_CFW_NW_GET_QSCANF_RSP},
    {0x00000229, RECV_EV_CFW_CC_INITIATE_SPEECH_CALL_RSP},
    {0x0000025c, RECV_EV_CFW_SS_SET_CALL_FORWARDING_RSP},
    {0x0000025d, RECV_EV_CFW_SS_QUERY_CALL_FORWARDING_RSP},
    {0x00000264, RECV_EV_CFW_SS_SEND_USSD_RSP},
    {0x000002bf, RECV_EV_CFW_SIM_GET_PBK_ENTRY_RSP},
    {0x000002c2, RECV_EV_CFW_SIM_GET_PBK_STRORAGE_INFO_RSP},
    {0x000002c3, RECV_EV_CFW_SIM_GET_PROVIDER_ID_RSP},
    {0x000002cd, RECV_EV_CFW_SIM_READ_MESSAGE_RSP},
    {0x000002ce, RECV_EV_CFW_SIM_LIST_MESSAGE_RSP},
    {0x000002d0, RECV_EV_CFW_SIM_READ_BINARY_RSP},
    {0x000002d2, RECV_EV_CFW_SIM_GET_SMS_PARAMETERS_RSP},
    {0x000002d9, RECV_EV_CFW_SAT_RESPONSE_RSP},
    {0x000002dc, RECV_EV_CFW_SIM_GET_PREF_OPERATOR_LIST_RSP},
    {0x000002ea, RECV_EV_CFW_SIM_GET_FILE_STATUS_RSP},
    {0x000002eb, RECV_EV_CFW_SIM_READ_RECORD_RSP},
    {0x000002f5, RECV_EV_CFW_SIM_READ_VOICEMIAL_RSP},
    {0x000002f8, RECV_EV_CFW_SIM_READ_FILES_RSP},
    {0x000002fd, RECV_EV_CFW_SMS_READ_MESSAGE_RSP},
    {0x000007d0, RECV_EV_CFW_SAT_REFRESH_FILE_CHG_RSP},
    {0x0000138e, RECV_EV_CFW_CC_SPEECH_CALL_IND},
    {0x00001390, RECV_EV_CFW_NEW_SMS_IND},
    {0x00001394, RECV_EV_CFW_SS_USSD_IND},
    {0x00001397, RECV_EV_CFW_SAT_CMDTYPE_IND},
    {0x00001399, RECV_EV_CFW_NW_NETWORKINFO_IND},
    {0x0000139e, RECV_EV_CFW_GPRS_DATA_IND},
    {0x000013b0, RECV_EV_CFW_CB_MSG_IND},
    {0x000013b1, RECV_EV_CFW_CB_PAGE_IND},
    {0x000013bf, RECV_EV_CFW_EMC_NUM_LIST_IND},
};

void *rpcFindEventUnpacker(unsigned tag)
{
    void *p = bsearch((const void *)(unsigned long)tag,
                      gRpcEventRecvTable,
                      sizeof(gRpcEventRecvTable) / sizeof(gRpcEventRecvTable[0]),
                      sizeof(gRpcEventRecvTable[0]),
                      _tagCompare);

    return (p != (void *)0) ? ((tagFunction_t *)p)->function : (void *)0;
}

static const tagName_t gApiNameTable[] = {
    {0x01a48c19, "CFW_GetNetinfo"},
    {0x036f213c, "CFW_EmodSetLteLockBand"},
    {0x03d69255, "CSW_SetAudioZspVqeCalibParam"},
    #if defined(CFW_GPRS_SUPPORT)
    {0x0425d22f, "CFW_getDnsServerbyPdp"},
    #endif
    {0x045ceb49, "CFW_SimAddPbkEntry"},
    {0x04f77c3d, "CFW_CbSetType"},
    {0x074a8a28, "CFW_CfgRestoreFactoryProfile"},
    {0x07600c35, "CSW_WriteCalibParamToFlash"},
    {0x0807f6e5, "CFW_SimGetICCID"},
    {0x080cd798, "CFW_SimUpdateRecord"},
    {0x08761de0, "CFW_SwitchUSBShareNetwork"},
    {0x08790a03, "CFW_CcPlayTone"},
    {0x096b6d42, "CFW_CcEmcDial"},
    {0x0a1a9480, "CFW_EmodGetNvVersion"},
    {0x0a272beb, "CFW_SimInit"},
    {0x0a402eac, "CFW_GetFreeCID"},
    #if defined(CFW_GPRS_SUPPORT)
    {0x0a671c97, "CFW_GetGprsActState"},
    #endif
    {0x0a79d13a, "CFW_GetNWTimerOutFlag"},
    {0x0aeb7786, "CFW_SmsInitComplete"},
    {0x0bb7f08e, "CFW_EmodEngineerCmd"},
    {0x0c820824, "CFW_SimGetFileStatus"},
    {0x0c87edd1, "CFW_SSCheckTestSim"},
    {0x0d3fa799, "CFW_CfgNwGetOperatorInfo"},
    {0x0e138a39, "CFW_SmsMtSmsPPAckReq"},
    {0x0f7a5643, "CFW_CfgSetErrorReport"},
    {0x103eecc1, "CFW_CfgGetIndicatorEvent"},
    #if defined(CFW_GPRS_SUPPORT)
    {0x105b539b, "CFW_GetGprsSum"},
    #endif
    {0x10d83a33, "CFW_CcInitiateSpeechCall_V2"},
    {0x10ead86d, "AUD_ToneStopEnd"},
    {0x14aedc7b, "CFW_GprsSetInternalPdpCtx"},
    #if defined(SIM_SWITCH_ENABLE)
    {0x14e06e43, "CFW_CfgSetSimSwitch"},
    #endif
    {0x1503ad06, "CFW_CfgSetSmsShowTextModeParam"},
    #if defined(CFW_GPRS_SUPPORT)
    {0x1514c385, "CFW_GprsManualAcc"},
    #endif
    {0x152320c3, "CFW_EmodSaveNvData"},
    {0x16e18027, "CFW_GetSMSConcat"},
    {0x17c0fa00, "CFW_ImsEnVolte"},
    {0x19b6c44a, "CFW_CfgGetClip"},
    {0x1a469219, "CFW_EmodGetBatteryInfo"},
    {0x1a63f26a, "CFW_SimUpdateVoiceMailInfo"},
    {0x1b0b9a67, "aud_SetAudDeviceCFG"},
    {0x1b686aea, "CFW_SimSetACMMax"},
    {0x1bcf7121, "CFW_EmodUpdateNwBind"},
    {0x1c29ffec, "aud_StreamStop"},
    {0x1e00e28a, "CFW_CfgSetAudioAudioMode"},
    {0x1ea60113, "CFW_CfgGetNewSmsOption"},
    {0x1f016809, "CSW_DumpPcmDataToTflash"},
    {0x1f9e7d3c, "CFW_SimSetPrefOperatorList"},
    {0x203e9990, "CFW_CfgGetSmsFormat"},
    {0x20f7d018, "aud_StreamStart"},
    {0x21813e4a, "CFW_EmodSpCleanInfo"},
    #if defined(CFW_GPRS_SUPPORT) && defined(LTE_SUPPORT)
    {0x22a1f04c, "CFW_GprsSetCtxEQos"},
    #endif
    {0x235d1b94, "CFW_NwGetCurrentOperator"},
    {0x23c4e97f, "CFW_CfgNwGetNetWorkMode"},
    {0x23e5688e, "CFW_CfgGetToneDuration"},
    {0x24883270, "CFW_CfgSetCallWaiting"},
    {0x24e30979, "CFW_SimReadMessage"},
    #if defined(CFW_GPRS_SUPPORT)
    {0x25e3db36, "CFW_ClearGprsSum"},
    #endif
    {0x2689cabc, "CFW_EmodSpLteDummyPara"},
    {0x26f63727, "CFW_SsSetFacilityLock"},
    {0x2718f091, "hal_zspMusicPlayStop"},
    #if defined(CFW_GPRS_SUPPORT)
    {0x27543604, "CFW_SetDnsServerbyPdp"},
    #endif
    {0x276ee189, "CFW_GetCcCount"},
    {0x27d28c5a, "CFW_CfgSetSSN"},
    {0x28488c88, "CFW_CfgGetAudioAudioMode"},
    #if defined(CFW_GPRS_SUPPORT)
    {0x28c73dae, "CFW_GprsGetPdpCxtV2"},
    #endif
    {0x2994f59c, "CFW_EmodSpecialNvProcess"},
    {0x2a2c1f97, "CFW_CfgSetPbkStorage"},
    {0x2a7cf76d, "CFW_SimReadFiles"},
    {0x2b480237, "CFW_CcAcceptSpeechCall"},
    #if defined(CFW_VOLTE_SUPPORT)
    {0x2b9844f1, "CFW_ImsCheckStatusAfterCc"},
    #endif
    {0x2c611403, "CFW_EmodSaveIMEI"},
    {0x2c64c51d, "CFW_NwSetNetMode"},
    {0x2c80e79f, "CFW_SmsWriteMessage_V2"},
    {0x2d03185d, "CFW_CfgGetIMSI"},
    {0x2d9d2455, "CFW_SatActivation"},
    {0x2e1489d8, "CFW_SsQueryCallWaiting"},
    {0x2e2a7c57, "CFW_CfgGetSmsStorageInfo"},
    {0x2ebfacc0, "CSW_SetAudioZspPreProcessCalibParam"},
    {0x2f55a10e, "CFW_EmodImsNvParamSet"},
    {0x2f976c7e, "CFW_ImsGetSrvccCap"},
    {0x2fc7c1ba, "CFW_SimGetPrefOperatorList"},
    {0x2fd3f031, "CFW_SimClose"},
    {0x2fd9ebda, "CFW_CfgSetColp"},
    {0x307d94f2, "CFW_EmodRrdmParam"},
    {0x356b9b3e, "CFW_CfgGetAudioOutputParam"},
    {0x365a1560, "CFW_CfgSimGetMeProfile"},
    {0x37be59b4, "CFW_CfgGetLoudspeakerVolumeLevel"},
    {0x38ba7a57, "rpcHelloOnCP"},
    {0x398b2a95, "CFW_CfgQuerySmsService"},
    {0x398eddda, "CFW_CfgSimSetMeProfile"},
    {0x3bc96b1f, "CFW_SimGetProviderId"},
    {0x3cdf3da7, "CFW_NWSetStackRat"},
    {0x3ce9c17e, "CFW_CfgSetStoredPlmnList"},
    {0x3cf96f15, "CFW_CfgSetClir"},
    {0x3d6b4b93, "CFW_SmsAbortSendMessage"},
    {0x3e336470, "CFW_CfgGetColp"},
    {0x3e65984a, "CFW_NwSysConfig"},
    {0x3f3995c2, "CFW_CfgSetSmsStorage"},
    {0x3f6c8048, "CFW_SimGetAuthenticationStatus"},
    {0x40860195, "CFW_SsQueryFacilityLock"},
    {0x41075729, "CFW_SetSMSConcat"},
    {0x424b0ac3, "CFW_CfgSetFirstPowerOnFlag"},
    #if defined(_SIM_HOT_PLUG_)
    {0x42aa254a, "CFW_SetPollingTimer"},
    #endif
    {0x440eda14, "aud_StreamRecordStop"},
    {0x44bf3c87, "CFW_EmodSetBindInfo"},
    {0x46843945, "CFW_CcGetCallStatus"},
    {0x471bc7fc, "CFW_EmodSyncInfoTest"},
    {0x47c56cdb, "CFW_NwDeRegister"},
    {0x48762fb4, "CFW_CcEmcDialEx"},
    {0x4ab2b8c4, "CFW_SimGetPbkStorage"},
    {0x4ace1ca9, "CFW_GetRRCRel"},
    {0x4fb39c55, "CFW_SatResponse"},
    {0x50cab25c, "CFW_SmsDeleteMessage"},
    #if defined(CFW_GPRS_SUPPORT) && defined(LTE_SUPPORT)
    {0x526ea981, "CFW_GprsCtxEpsModify"},
    #endif
    #if defined(CFW_GPRS_SUPPORT)
    {0x53a8e0fb, "CFW_GprsGetMInQos"},
    #endif
    {0x5447da9e, "CFW_SimGetACMMax"},
    #if defined(CFW_VOLTE_SUPPORT)
    {0x554d3316, "CFW_ImsSetDapsCallback"},
    #endif
    #if defined(CFW_VOLTE_SUPPORT)
    {0x555cee71, "CFW_ImsDoSavedEmcDail"},
    #endif
    #if defined(LTE_SUPPORT)
    {0x55e31e22, "CFW_CcReleaseCallX"},
    #endif
    {0x592995fb, "CFW_CfgSetNewSmsOption"},
    {0x59557ac7, "CFW_CcCallMultipartyFunction"},
    {0x5a8598b0, "CFW_SatGetCurCMD"},
    {0x5b070593, "CFW_CfgGetNwOperDispFormat"},
    {0x5c3b6a0e, "CFW_EmodGetLteCalibInfo"},
    {0x5e84647a, "CFW_SimListCountPbkEntries"},
    {0x5eb87c1b, "CFW_EmodGetBindInfo"},
    {0x606ae82f, "CFW_SimWriteMessage"},
    {0x60c38c93, "CFW_NwSetFrequencyBand"},
    {0x610691f7, "CFW_SetSimStatus"},
    {0x615f0347, "CFW_CcStopTone"},
    {0x618b132d, "CFW_SsQueryClip"},
    #if defined(CFW_VOLTE_SUPPORT)
    {0x6198a170, "CFW_GprsGetNsapi"},
    #endif
    {0x62e05df9, "CFW_GetIMEIFromAP"},
    {0x62e3977e, "CFW_SimListPbkEntries"},
    {0x63ad8ff5, "rpcHelloOnAP"},
    #if defined(CFW_GPRS_SUPPORT)
    {0x63af5d12, "CFW_GprsAct"},
    #endif
    {0x63df1884, "CFW_EmodClearUserInfo"},
    {0x641e4ad6, "CFW_isPdpActiveAvailable"},
    {0x6482e52d, "CFW_SimReadBinary"},
    {0x66010143, "CFW_GetLETRealTimeInfo"},
    #if defined(SIM_SWITCH_ENABLE)
    {0x665f6ca9, "SimSwitch"},
    #endif
    {0x67735c93, "CFW_SimGetServiceProviderName"},
    {0x69bee605, "CFW_CfgGetCharacterSet"},
    {0x6b0b2278, "CFW_EmodGetProductInfo"},
    #if defined(CFW_VOLTE_SUPPORT)
    {0x6b251398, "CFW_ImsSetPdpStatus"},
    #endif
    #if defined(_SIM_HOT_PLUG_)
    {0x6b497865, "CFW_GetPollingTimer"},
    #endif
    {0x6b687af8, "CFW_EmodGetCEMode"},
    #if defined(CFW_GPRS_SUPPORT)
    {0x6d2f4987, "CFW_GprsGetstatus"},
    #endif
    #if defined(CFW_GPRS_SUPPORT)
    {0x6e3e849a, "CFW_GprsSetPdpCxtV2"},
    #endif
    #if defined(CFW_GPRS_SUPPORT)
    {0x6f20505a, "getDNSServer"},
    #endif
    {0x6f36868c, "CFW_EmodSetCEMode"},
    {0x6fc3900c, "CFW_CfgGetResultCodePresentMode"},
    {0x6fcc1282, "CFW_NwAbortListOperators"},
    {0x7121b58d, "CFW_SetRRCRel"},
    {0x717536f2, "CSW_VersionControl"},
    {0x71ed90b2, "CFW_CfgDeviceSwithOff"},
    {0x725848a9, "CSW_ExportCalibOrImportCalibFlashParam"},
    {0x727f5ca5, "CFW_EmodSetNvData"},
    {0x72ad454c, "CFW_CfgGetExtendedErrorV1"},
    {0x73af8928, "CFW_CfgSetPbkStrorageInfo"},
    {0x74dc9c53, "CFW_CfgNwGetOperatorName"},
    {0x75013fe5, "CFW_CfgSetToneDuration"},
    {0x75bf0c54, "CFW_EmodGetIMEI"},
    #if defined(CFW_GPRS_SUPPORT)
    {0x75d9a654, "CFW_GprsCtxModifyRej"},
    #endif
    #if defined(CFW_GPRS_SUPPORT) && defined(LTE_SUPPORT)
    {0x761ecef8, "CFW_GprsSecPdpAct"},
    #endif
    #if defined(CFW_GPRS_SUPPORT)
    {0x76a3cadf, "CFW_GprsActEx"},
    #endif
    {0x76d14edf, "CFW_NwGetAvailableOperators"},
    {0x78fe7b0d, "CFW_CfgGetLoudspeakerVolumeLevelRange"},
    #if defined(CFW_GPRS_SUPPORT) && defined(LTE_SUPPORT)
    {0x79d77a2b, "CFW_GprsSetCtxTFT"},
    #endif
    {0x7a4fe641, "CFW_NwGetAutoAttachFlag"},
    {0x7a5ee337, "CFW_CfgGetEchoMode"},
    #if defined(CFW_GPRS_SUPPORT)
    {0x7ab90486, "CFW_GprsCtxModifyAcc"},
    #endif
    {0x7d7969f5, "CFW_CbDeAct"},
    {0x7dbb6ec7, "CFW_SsSetCallForwarding"},
    {0x7e01cec5, "CFW_CfgSetSmsStorageInfo"},
    {0x7e6dec45, "CFW_SimReset"},
    {0x7fcfef26, "CFW_CfgGetSmsOverflowInd"},
    {0x80243b3f, "CFW_CcEmcDialEx_V2"},
    {0x81c945ab, "hal_WriteGsmCalib"},
    {0x81fd8a88, "CFW_NwGetLteSignalQuality"},
    {0x825d1b6c, "CFW_NwSetRegistration"},
    #if defined(CFW_GPRS_SUPPORT)
    {0x8271116b, "CFW_GprsCtxModify"},
    #endif
    {0x8291ead7, "CFW_NwGetQualReport"},
    {0x82ccd1ad, "CFW_EmodReadBindSIM"},
    {0x832b47ad, "CFW_CfgSetLoudspeakerVolumeLevel"},
    #if defined(LTE_SUPPORT)
    {0x84d40ffe, "CFW_NwSetEpsMode"},
    #endif
    {0x85e2fdf5, "CFW_NWGetRat"},
    #if defined(CFW_GPRS_SUPPORT)
    {0x86a85765, "CFW_GprsGetPdpAddr"},
    #endif
    {0x87cf01e4, "CFW_SimGetPbkStrorageInfo"},
    {0x8855397e, "CSW_SetAudioCodecCalibParam"},
    {0x88c37c19, "CFW_GprsIpDnsInfo"},
    {0x88d9b097, "CFW_CfgSetIMSI"},
    {0x895b63e7, "CFW_SsQueryColp"},
    {0x89f8a01a, "CFW_SsSendUSSD"},
    {0x8a0c270b, "CFW_GetFreeUTI"},
    {0x8beea828, "CFW_NwSetAutoAttachFlag"},
    {0x8c4457d3, "CFW_CcReleaseCall"},
    {0x8c9061a8, "CFW_SimGetSmsTotalNum"},
    {0x8da891d9, "CFW_CcGetCurrentCall"},
    {0x8dae8ade, "CFW_CfgSetAntenna"},
    {0x8ddaf837, "CFW_CfgSetClip"},
    {0x8f00356e, "CFW_CfgGetCdmaImsi"},
    {0x8f8129ef, "CFW_NwSetLockBCCH"},
    {0x8fdc72bf, "CFW_ImsSrvccCapSet"},
    {0x9052629e, "CFW_NwGetLockBCCH"},
    #if defined(CFW_VOLTE_SUPPORT)
    {0x92d862ef, "CFW_ImsInit"},
    #endif
    {0x92ddeb2c, "CFW_CfgGetConnectResultFormatCode"},
    {0x931bd91a, "CFW_EmodResetImsDelta"},
    {0x940784bc, "aud_Setup"},
    #if defined(CFW_GPRS_SUPPORT)
    {0x945f113e, "CFW_GprsSetReqQos"},
    #endif
    {0x95b333c9, "CFW_GetMNCLen"},
    {0x95cd0c5f, "CFW_EmodGetLteLockBand"},
    {0x95e61408, "CFW_SimSetACM"},
    {0x96567f1f, "CFW_NwChangeRat"},
    {0x965f544e, "CFW_GetComm"},
    {0x9723220b, "CFW_NwGetSignalQuality"},
    {0x972830bc, "CFW_EmodOutfieldTestStart"},
    {0x9912b42b, "CFW_EmodGetNvData"},
    {0x991e05de, "CFW_CfgSetCharacterSet"},
    {0x994da05e, "CFW_SetGcfTest"},
    {0x995d38e0, "CFW_EmodAudioTestStart"},
    {0x9a1144fe, "CFW_CcAcceptSpeechCallEx"},
    {0x9a24d26a, "CFW_SetInitWithSim"},
    {0x9ae2bc1a, "CFW_SmsMMAReq"},
    {0x9af699bd, "CFW_NwGetStatus"},
    {0x9b269a8d, "CFW_CfgSetResultCodePresentMode"},
    #if defined(CFW_VOLTE_SUPPORT)
    {0x9c72a751, "CFW_ImsDisable"},
    #endif
    #if defined(CFW_VOLTE_SUPPORT)
    {0x9d929a96, "CFW_SsSetClip"},
    #endif
    {0x9dccde75, "CFW_CfgSetNwStatus"},
    {0x9e37b0ea, "CFW_SmsReadMessage"},
    #if defined(CFW_VOLTE_SUPPORT)
    {0x9f1f9c07, "CFW_ImsIsSet"},
    #endif
    {0xa046cd5e, "CFW_CfgGetPhoneFunctionality"},
    {0xa0c22862, "CFW_EmodImsNvParamGet"},
    {0xa0dae657, "CFW_CfgGetSmsStorage"},
    {0xa2cd3816, "CFW_NWGetStackRat"},
    {0xa3e4654b, "CFW_CfgNwGetFrequencyBand"},
    {0xa40eca33, "CFW_CfgStoreUserProfile"},
    #if defined(CFW_VOLTE_SUPPORT)
    {0xa45579e4, "CFW_ImsSuspend"},
    #endif
    {0xa53c084f, "CFW_CfgGetUserProfile"},
    {0xa64720ca, "CFW_SimDeletePbkEntry"},
    #if defined(CFW_GPRS_SUPPORT)
    {0xa66b1aa9, "CFW_GprsSendData"},
    #endif
    {0xa6720283, "CFW_SsQueryClir"},
    {0xa6965bff, "CFW_SimGetSmsParameters"},
    {0xa6eed8f0, "CFW_EmodReadBindNW"},
    {0xa7a1c43e, "CFW_SimGetPrefOperatorListMaxNum"},
    {0xa7d4e623, "CFW_CfgSetResultCodeFormatMode"},
    {0xa8596703, "CFW_SmsSetUnSent2Sent"},
    {0xa99abe2e, "CFW_CfgGetSSN"},
    {0xa9f316f2, "CFW_CfgSetSmsFormat"},
    {0xaa3996b2, "CFW_CfgSetConnectResultFormatCode"},
    {0xaa8591be, "CFW_SimGetPbkEntry"},
    {0xab6bc7f1, "CFW_CfgGetIncomingCallResultMode"},
    {0xab7975c4, "CFW_EmodGetLteLockFreq"},
    {0xab91f894, "CFW_SmsSendMessage_V2"},
    {0xabfd1d2f, "CFW_CfgGetNwStatus"},
    {0xac2b1b66, "CFW_IsFreeUTI"},
    {0xace9ca72, "CFW_CcRejectCall"},
    #if defined(CFW_VOLTE_SUPPORT)
    {0xad7d0f90, "CFW_SsSetClir"},
    #endif
    {0xae454ab0, "CFW_EmodAudioTestEnd"},
    {0xafcd013d, "aud_StreamRecord"},
    {0xb02b949a, "CFW_CfgSetSoftReset"},
    {0xb0ff5bb2, "CFW_GetStackSimFileID"},
    {0xb17252a7, "CFW_EmodSetTracePort"},
    {0xb3513926, "CFW_SimChangePassword"},
    {0xb36a4c39, "CFW_CfgGetCallWaiting"},
    {0xb3b37874, "CFW_EmodGrrLteFreq"},
    {0xb4319259, "CFW_CfgSetPhoneFunctionality"},
    {0xb4b7a4da, "CFW_CfgSetSmsOverflowInd"},
    {0xb5beb0cf, "CFW_SimAddPbkEntry_V2"},
    #if defined(CFW_GPRS_SUPPORT) && defined(LTE_SUPPORT)
    {0xb70c6d65, "CFW_GprsSetPsm"},
    #endif
    {0xb7128488, "CFW_SsChangePassword"},
    #if defined(CFW_VOLTE_SUPPORT)
    {0xb76466e9, "CFW_ImsResume"},
    #endif
    {0xb7e77ecb, "CFW_NWSetRat"},
    #if defined(SIM_SWITCH_ENABLE)
    {0xb9697fc4, "CFW_CfgGetSimSwitch"},
    #endif
    {0xb9a37a07, "CFW_EmodSetVoicePrefer"},
    {0xba3d62fe, "CFW_SimGetACM"},
    {0xbc760df9, "CSW_SetAudioZspPostProcessCalibParam"},
    {0xbdaa958e, "CFW_SimGetStatus"},
    {0xbdd83a6a, "CFW_GetSimSPN"},
    {0xbe60af25, "CFW_CfgGetDefaultSmsParam"},
    {0xc021092d, "CFW_SmsSendPduMessage"},
    {0xc070ad19, "AUD_SetCodecOpStatus"},
    {0xc0bd302c, "CFW_CfgGetResultCodeFormatMode"},
    {0xc122799c, "CFW_CfgGetSoftReset"},
    #if defined(CFW_GPRS_SUPPORT)
    {0xc1988543, "CFW_AttDetach"},
    #endif
    {0xc361faa4, "CFW_GetCellInfo"},
    {0xc37f54c0, "CFW_NwGetFTA"},
    {0xc3a3200e, "CFW_CfgSetSmsCB"},
    {0xc40a65b9, "CFW_CfgNwSetNetWorkMode"},
    {0xc4bee657, "CFW_CfgSetIncomingCallResultMode"},
    #if defined(CFW_VOLTE_SUPPORT)
    {0xc637a64f, "CFW_SsSetColp"},
    #endif
    {0xc69c0723, "CFW_EmodL1Param"},
    {0xc6e5fb72, "CFW_SmsGetMessageNodeEx"},
    {0xc74fce6d, "CFW_CfgGetSmsCB"},
    {0xc921d725, "CFW_SetCID"},
    {0xcaec4d3b, "CFW_CfgGetSmsShowTextModeParam"},
    {0xcb66a059, "CFW_SimReadVoiceMailInfo"},
    {0xcbb6d554, "CFW_EmodGetRFCalibrationInfo"},
    {0xcbc0852d, "CFW_CfgGetErrorReport"},
    {0xcbe35db9, "CFW_CfgSetIndicatorEvent"},
    #if defined(CFW_GPRS_SUPPORT)
    {0xcc80601e, "CFW_GprsManualRej"},
    #endif
    {0xccbe3de8, "CFW_EmodGetVoicePrefer"},
    {0xcd436267, "CFW_CfgGetPbkStorage"},
    {0xce8bf0bd, "hal_zspMusicPlayStart"},
    {0xcfe5dd7b, "CFW_NwGetSysConfig"},
    {0xd02d8d7a, "CFW_CbAct"},
    {0xd0b065ed, "CFW_SimSetSmsParameters"},
    {0xd182b1b4, "CFW_ImsSetUaVersionInfo"},
    {0xd1853aaf, "CFW_SimGetPUCT"},
    {0xd1edf62e, "CFW_SimReadRecord"},
    #if defined(CFW_GPRS_SUPPORT)
    {0xd21e74d0, "CFW_GprsSetSmsSeviceMode"},
    #endif
    {0xd28c1f50, "CFW_SimGetFacilityLock"},
    {0xd2b4a305, "CFW_CcCallHoldMultiparty"},
    {0xd2cedcbb, "CFW_CfgNwSetFrequencyBand"},
    {0xd480bf02, "CFW_CfgSetSmsParam"},
    {0xd5b14f3b, "CFW_CfgSetEchoMode"},
    {0xd5d4a325, "CFW_SetPowerOffFlag"},
    {0xd6d006b2, "CFW_CfgGetClir"},
    {0xd6d9fa1b, "CFW_SsQueryColr"},
    {0xd6fab239, "CFW_SimSetFacilityLock"},
    {0xd7655a97, "CFW_SmsMemoryFullReq"},
    {0xd7c9260b, "CFW_SetQualReport"},
    {0xd9b805ba, "CFW_SimSetVirtualFlag"},
    {0xda558016, "CFW_SimEnterAuthentication"},
    {0xdb8ad850, "CFW_SmsListMessages"},
    #if defined(CFW_VOLTE_SUPPORT)
    {0xdbf659dd, "CFW_ImsIsRegistered"},
    #endif
    {0xdcd3a5a9, "CFW_NwSetFTA"},
    #if defined(CFW_GPRS_SUPPORT)
    {0xdd6267c5, "CFW_GprsSetMinQos"},
    #endif
    {0xde75aebe, "CFW_SmsSetUnRead2Read"},
    {0xdf39cd5c, "CFW_GetNetinfoStatistic"},
    {0xdfab74a6, "CFW_CfgCheckFirstPowerOnFlag"},
    #if defined(CFW_GPRS_SUPPORT)
    {0xe07bd895, "CFW_GetGprsAttState"},
    #endif
    {0xe0991e80, "CFW_SatRefreshImsiChange"},
    {0xe1d549f5, "CFW_CfgSelectSmsService"},
    #if defined(CFW_VOLTE_SUPPORT)
    {0xe3bacaaf, "CFW_ImsGetPdpStatus"},
    #endif
    {0xe4a00223, "CFW_ShellControl"},
    #if defined(CFW_GPRS_SUPPORT)
    {0xe4cc5064, "CFW_GprsAtt"},
    #endif
    #if defined(CFW_VOLTE_SUPPORT)
    {0xe4ed0761, "CFW_ImsEnable"},
    #endif
    {0xe503676d, "CFW_CfgSetNwOperDispFormat"},
    {0xe5fdf9e0, "CFW_SmsMoInit"},
    #if defined(CFW_VOLTE_SUPPORT)
    {0xe60f5b9d, "CFW_ImsActImsPdp"},
    #endif
    {0xe6b2eaa7, "CFW_CfgSetDefaultSmsParam"},
    {0xe8f8e512, "CFW_EmodGetGsmCalibInfo"},
    {0xe9225666, "CFW_SsSetCallWaiting"},
    {0xe96c6a34, "CFW_CfgGetStoredPlmnList"},
    {0xe9d0fbdf, "vois_SetCfg"},
    #if defined(CFW_GPRS_SUPPORT)
    {0xea3e6cae, "CFW_GprsGetSmsSeviceMode"},
    #endif
    #if defined(CFW_GPRS_SUPPORT) && defined(LTE_SUPPORT)
    {0xea5153a8, "CFW_GprsGetCtxEQos"},
    #endif
    {0xeb30b3e0, "CFW_GprsClrIpDns"},
    {0xeb4d03f5, "CFW_SimUpdateBinary"},
    {0xec6140b2, "CSW_SetAudioZspNxpCalibParam"},
    {0xee15e996, "CFW_CheckIfAllowedRecvSms"},
    {0xee64a452, "CFW_EmodSetLteLockFreq"},
    {0xeee2d88b, "CFW_CfgGetPbkStrorageInfo"},
    {0xeeef512f, "CFW_GetSimStatus"},
    {0xef062af4, "CFW_CbSetContext"},
    {0xef959240, "CFW_EmodRrtmParam"},
    #if defined(CFW_GPRS_SUPPORT)
    {0xf0322972, "CFW_GprsGetReqQos"},
    #endif
    {0xf15912cb, "cfw_SimReadElementaryFile"},
    {0xf1dcf009, "CFW_ReleaseCID"},
    {0xf2b23c71, "CFW_CfgGetSmsParam"},
    {0xf37b9eb1, "CFW_CfgSetEventReport"},
    {0xf3beb108, "CFW_CcInitiateSpeechCallEx_V2"},
    {0xf4a2809f, "CFW_SmsSetRead2UnRead"},
    {0xf4b2c147, "CFW_SimDeleteMessage"},
    #if defined(CFW_VOLTE_SUPPORT)
    {0xf52fc960, "CFW_ImsSetVolte"},
    #endif
    {0xf5ab108b, "CFW_SsQueryCallForwarding"},
    {0xf65e019c, "CFW_SetNWTimerOutFlag"},
    {0xf6723896, "hal_WriteLteCalib"},
    {0xf6a7246e, "CFW_EmodUpdateSimBind"},
    {0xf6dd0cb5, "CFW_CfgNwGetOperatorId"},
    {0xf851b8fc, "CFW_CfgSetAudioOutputParam"},
    {0xf85926dc, "CFW_SimSetPUCT"},
    {0xf95de818, "CFW_SatRefreshFileChange"},
    {0xfab521ed, "CFW_NwGetImei"},
    {0xfae6ff84, "CFW_SetComm"},
    {0xfb1a71d8, "CFW_NwSetBSF"},
    {0xfbb6e127, "CFW_NwSetnSecurityUsedFlag"},
    {0xfcada50f, "CFW_CfgGetEventReport"},
    {0xfd41ebd7, "CFW_NwSetXCAP"},
    #if defined(CFW_VOLTE_SUPPORT)
    {0xfeb9adf0, "CFW_ImsFreeUti"},
    #endif
    {0xffea09ab, "CFW_EmodOutfieldTestEnd"},
};

const char *rpcFindNameByTag(unsigned tag)
{
    void *p = bsearch((const void *)(unsigned long)tag,
                      gApiNameTable,
                      sizeof(gApiNameTable) / sizeof(gApiNameTable[0]),
                      sizeof(gApiNameTable[0]),
                      _tagCompare);

    return (p != (void *)0) ? ((tagName_t *)p)->name : "";
}

// AUTO GENERATED END
