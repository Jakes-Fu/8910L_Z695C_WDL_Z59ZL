// AUTO GENERATED

void RPC_CFW_GetIMEIFromAP(void *in, void *out)
{
    struct IPAR_CFW_GetIMEIFromAP *ipar = (struct IPAR_CFW_GetIMEIFromAP *)in;
    struct OPAR_CFW_GetIMEIFromAP *opar = (struct OPAR_CFW_GetIMEIFromAP *)out;
    CFW_GetIMEIFromAP(opar->pImei, &opar->pImeiLen, ipar->nSimID);
}

bool RECV_EV_CFW_CC_SPEECH_CALL_IND(rpcEventHeader_t *event)
{
    return rpcUnpackPointerEvent(event, sizeof(CFW_SPEECH_CALL_IND), 0);
}

bool RECV_EV_CFW_CC_INITIATE_SPEECH_CALL_RSP(rpcEventHeader_t *event)
{
    return rpcUnpackPointerEvent(event, SIZE_IF_TYPE1(sizeof(CFW_TELNUMBER)), 0);
}

bool RECV_EV_CFW_NEW_SMS_IND(rpcEventHeader_t *event)
{
    return rpcUnpackPointerEvent(event, SIZE_IF_TYPE0(sizeof(CFW_NEW_SMS_NODE_EX)), 0);
}

bool RECV_EV_CFW_NW_NETWORKINFO_IND(rpcEventHeader_t *event)
{
    return rpcUnpackPointerEvent(event, sizeof(CFW_NW_NETWORK_INFO), 0);
}

bool RECV_EV_CFW_NW_SET_REGISTRATION_RSP(rpcEventHeader_t *event)
{
    return rpcUnpackPointerEvent(event, SIZE_IF_TYPE0(6), 0);
}

bool RECV_EV_CFW_GPRS_DATA_IND(rpcEventHeader_t *event)
{
    return rpcUnpackPointerEvent(event, CFW_GPRS_DATA_SIZE, 0);
}

bool RECV_EV_CFW_SIM_GET_PBK_ENTRY_RSP(rpcEventHeader_t *event)
{
    return rpcUnpackPointerEvent(event, SIZE_PARAM2HI16L8_IF_TYPE0, 0);
}

bool RECV_EV_CFW_SIM_GET_PBK_STRORAGE_INFO_RSP(rpcEventHeader_t *event)
{
    return rpcUnpackPointerEvent(event, SIZE_PARAM2LO16_IF_TYPE0, 0);
}

bool RECV_EV_CFW_SIM_GET_PROVIDER_ID_RSP(rpcEventHeader_t *event)
{
    return rpcUnpackPointerEvent(event, SIZE_PARAM2_IF_TYPE0, 0);
}

bool RECV_EV_CFW_SIM_READ_BINARY_RSP(rpcEventHeader_t *event)
{
    return rpcUnpackPointerEvent(event, SIZE_PARAM2HI16_IF_TYPE0, 0);
}

bool RECV_EV_CFW_SIM_GET_PREF_OPERATOR_LIST_RSP(rpcEventHeader_t *event)
{
    return rpcUnpackPointerEvent(event, COUNT_PARAM2_IF_TYPE0(3), 0);
}

bool RECV_EV_CFW_SIM_GET_FILE_STATUS_RSP(rpcEventHeader_t *event)
{
    return rpcUnpackPointerEvent(event, SIZE_PARAM2HI16_IF_TYPE0, 0);
}

bool RECV_EV_CFW_SIM_READ_RECORD_RSP(rpcEventHeader_t *event)
{
    return rpcUnpackPointerEvent(event, SIZE_PARAM2HI16_IF_TYPE0, 0);
}

bool RECV_EV_CFW_SIM_READ_MESSAGE_RSP(rpcEventHeader_t *event)
{
    return rpcUnpackPointerEvent(event, SMS_PDU_LEN_IF_TYPE0, 0);
}

bool RECV_EV_CFW_SIM_LIST_MESSAGE_RSP(rpcEventHeader_t *event)
{
    return rpcUnpackPointerEvent(event, SMS_PDU_LEN_IF_TYPE0, 0);
}

bool RECV_EV_CFW_SMS_READ_MESSAGE_RSP(rpcEventHeader_t *event)
{
    return rpcUnpackPointerEvent(event, EV_CFW_SMS_READ_MESSAGE_RSP_PTR1_SIZE, 0);
}

bool RECV_EV_CFW_NW_GET_AVAIL_OPERATOR_RSP(rpcEventHeader_t *event)
{
    return rpcUnpackPointerEvent(event, EV_CFW_NW_GET_AVAIL_OPERATOR_RSP_PTR1_SIZE, 0);
}

bool RECV_EV_CFW_NW_GET_QSCANF_RSP(rpcEventHeader_t *event)
{
    return rpcUnpackPointerEvent(event, EV_CFW_NW_GET_QSCANF_RSP_PTR1_SIZE, 0);
}

bool RECV_EV_CFW_SIM_READ_VOICEMIAL_RSP(rpcEventHeader_t *event)
{
    return rpcUnpackPointerEvent(event, SIZE_PARAM2_IF_TYPE0, 0);
}

bool RECV_EV_CFW_CB_MSG_IND(rpcEventHeader_t *event)
{
    return rpcUnpackPointerEvent(event, SIZE_PARAM2_IF_TYPE0, 0);
}

bool RECV_EV_CFW_CB_PAGE_IND(rpcEventHeader_t *event)
{
    return rpcUnpackPointerEvent(event, SIZE_PARAM2_IF_TYPE0, 0);
}

bool RECV_EV_CFW_SIM_GET_SMS_PARAMETERS_RSP(rpcEventHeader_t *event)
{
    return rpcUnpackPointerEvent(event, SIZE_IF_TYPE0(sizeof(CFW_SIM_SMS_PARAMETERS)), 0);
}

bool RECV_EV_CFW_SAT_CMDTYPE_IND(rpcEventHeader_t *event)
{
    return rpcUnpackPointerEvent(event, SIZE_PARAM2_IF_TYPE0, 0);
}

bool RECV_EV_CFW_SS_QUERY_CALL_FORWARDING_RSP(rpcEventHeader_t *event)
{
    return rpcUnpackPointerEvent(event, SIZE_IF_TYPE0(sizeof(CFW_SS_QUERY_CALLFORWARDING_RSP)), 0);
}

bool RECV_EV_CFW_SS_SET_CALL_FORWARDING_RSP(rpcEventHeader_t *event)
{
    return rpcUnpackPointerEvent(event, SIZE_IF_TYPE0(sizeof(CFW_SS_SET_CALLFORWARDING_INFO)), 0);
}

bool RECV_EV_CFW_SS_SEND_USSD_RSP(rpcEventHeader_t *event)
{
    return rpcUnpackPointerEvent(event, SS_USSD_IF_TYPE0(sizeof(CFW_SS_USSD_IND_INFO_V2)), 0);
}

bool RECV_EV_CFW_SS_USSD_IND(rpcEventHeader_t *event)
{
    return rpcUnpackPointerEvent(event, SIZE_IF_TYPE0(sizeof(CFW_SS_USSD_IND_INFO_V2)), 0);
}

bool RECV_EV_CFW_EMC_NUM_LIST_IND(rpcEventHeader_t *event)
{
    return rpcUnpackPointerEvent(event, SIZE_IF_TYPE0(sizeof(CFW_NW_EMC_NUM_LIST_INFO)), 0);
}

bool RECV_EV_CFW_SIM_READ_FILES_RSP(rpcEventHeader_t *event)
{
    return rpcUnpackPointerEvent(event, SIZE_PARAM2_IF_TYPE0, 0);
}

bool RECV_EV_CFW_SAT_REFRESH_FILE_CHG_RSP(rpcEventHeader_t *event)
{
    return rpcUnpackPointerEvent(event, sizeof(CFW_SAT_REFRESH_FILE_CHG_CNF_T), 0);
}

bool RECV_EV_CFW_SAT_RESPONSE_RSP(rpcEventHeader_t *event)
{
    return rpcUnpackPointerEvent(event, 0, SAT_RSP_RSP_IF_TYPE0);
}


// AUTO GENERATED END
