/* Copyright (C) 2018 RDA Technologies Limited and/or its affiliates("RDA").
 * All rights reserved.
 *
 * This software is supplied "AS IS" without any warranties.
 * RDA assumes no responsibility or liability for the use of the software,
 * conveys no license or title under any patent, copyright, or mask work
 * right to the product. RDA reserves the right to make changes in the
 * software without notification.  RDA also make no representation or
 * warranty that such application will be suitable for the specified use
 * without further testing or modification.
 */

// AUTO GENERATED

uint32_t CFW_IsFreeUTI(uint16_t nUTI, uint32_t nServiceId)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CFW_IsFreeUTI) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_CFW_IsFreeUTI);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct IPAR_CFW_IsFreeUTI *ipar = (struct IPAR_CFW_IsFreeUTI *)((char*)mem + sizeof(rpcCallHeader_t));
    struct OPAR_CFW_IsFreeUTI *opar = (struct OPAR_CFW_IsFreeUTI *)((char*)mem + sizeof(rpcRespHeader_t));
    uint32_t result;

    call->h.size = call_size;
    call->api_tag = TAG_CFW_IsFreeUTI;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    ipar->nUTI = nUTI;
    ipar->nServiceId = nServiceId;
    rpcSendCall(RPC_CHANNEL, call);

    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

uint32_t CFW_GetFreeUTI(uint32_t nServiceId, uint8_t *nUTI)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CFW_GetFreeUTI) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_CFW_GetFreeUTI);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct IPAR_CFW_GetFreeUTI *ipar = (struct IPAR_CFW_GetFreeUTI *)((char*)mem + sizeof(rpcCallHeader_t));
    struct OPAR_CFW_GetFreeUTI *opar = (struct OPAR_CFW_GetFreeUTI *)((char*)mem + sizeof(rpcRespHeader_t));
    uint32_t result;

    call->h.size = call_size;
    call->api_tag = TAG_CFW_GetFreeUTI;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    ipar->nServiceId = nServiceId;
    rpcSendCall(RPC_CHANNEL, call);

    *nUTI = opar->nUTI;
    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

uint32_t CFW_ShellControl(uint8_t nCommand)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CFW_ShellControl) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_CFW_ShellControl);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct IPAR_CFW_ShellControl *ipar = (struct IPAR_CFW_ShellControl *)((char*)mem + sizeof(rpcCallHeader_t));
    struct OPAR_CFW_ShellControl *opar = (struct OPAR_CFW_ShellControl *)((char*)mem + sizeof(rpcRespHeader_t));
    uint32_t result;

    call->h.size = call_size;
    call->api_tag = TAG_CFW_ShellControl;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    ipar->nCommand = nCommand;
    rpcSendCall(RPC_CHANNEL, call);

    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

uint32_t CFW_CfgSetPhoneFunctionality(uint8_t nLevel, uint8_t nRst)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CFW_CfgSetPhoneFunctionality) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_CFW_CfgSetPhoneFunctionality);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct IPAR_CFW_CfgSetPhoneFunctionality *ipar = (struct IPAR_CFW_CfgSetPhoneFunctionality *)((char*)mem + sizeof(rpcCallHeader_t));
    struct OPAR_CFW_CfgSetPhoneFunctionality *opar = (struct OPAR_CFW_CfgSetPhoneFunctionality *)((char*)mem + sizeof(rpcRespHeader_t));
    uint32_t result;

    call->h.size = call_size;
    call->api_tag = TAG_CFW_CfgSetPhoneFunctionality;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    ipar->nLevel = nLevel;
    ipar->nRst = nRst;
    rpcSendCall(RPC_CHANNEL, call);

    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

uint32_t CFW_CfgGetPhoneFunctionality(uint8_t *nLevel)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + 0 + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_CFW_CfgGetPhoneFunctionality);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct OPAR_CFW_CfgGetPhoneFunctionality *opar = (struct OPAR_CFW_CfgGetPhoneFunctionality *)((char*)mem + sizeof(rpcRespHeader_t));
    uint32_t result;

    call->h.size = call_size;
    call->api_tag = TAG_CFW_CfgGetPhoneFunctionality;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    rpcSendCall(RPC_CHANNEL, call);

    *nLevel = opar->nLevel;
    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

uint32_t CFW_CfgStoreUserProfile(void)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + 0 + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_CFW_CfgStoreUserProfile);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct OPAR_CFW_CfgStoreUserProfile *opar = (struct OPAR_CFW_CfgStoreUserProfile *)((char*)mem + sizeof(rpcRespHeader_t));
    uint32_t result;

    call->h.size = call_size;
    call->api_tag = TAG_CFW_CfgStoreUserProfile;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    rpcSendCall(RPC_CHANNEL, call);

    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

uint32_t CFW_CfgGetUserProfile(void)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + 0 + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_CFW_CfgGetUserProfile);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct OPAR_CFW_CfgGetUserProfile *opar = (struct OPAR_CFW_CfgGetUserProfile *)((char*)mem + sizeof(rpcRespHeader_t));
    uint32_t result;

    call->h.size = call_size;
    call->api_tag = TAG_CFW_CfgGetUserProfile;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    rpcSendCall(RPC_CHANNEL, call);

    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

uint32_t CFW_CfgRestoreFactoryProfile(void)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + 0 + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_CFW_CfgRestoreFactoryProfile);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct OPAR_CFW_CfgRestoreFactoryProfile *opar = (struct OPAR_CFW_CfgRestoreFactoryProfile *)((char*)mem + sizeof(rpcRespHeader_t));
    uint32_t result;

    call->h.size = call_size;
    call->api_tag = TAG_CFW_CfgRestoreFactoryProfile;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    rpcSendCall(RPC_CHANNEL, call);

    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

uint32_t CFW_CfgGetCharacterSet(uint8_t *chset)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + 0 + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_CFW_CfgGetCharacterSet);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct OPAR_CFW_CfgGetCharacterSet *opar = (struct OPAR_CFW_CfgGetCharacterSet *)((char*)mem + sizeof(rpcRespHeader_t));
    uint32_t result;

    call->h.size = call_size;
    call->api_tag = TAG_CFW_CfgGetCharacterSet;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    rpcSendCall(RPC_CHANNEL, call);

    *chset = opar->chset;
    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

uint32_t CFW_CfgSetCharacterSet(uint8_t chset)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CFW_CfgSetCharacterSet) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_CFW_CfgSetCharacterSet);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct IPAR_CFW_CfgSetCharacterSet *ipar = (struct IPAR_CFW_CfgSetCharacterSet *)((char*)mem + sizeof(rpcCallHeader_t));
    struct OPAR_CFW_CfgSetCharacterSet *opar = (struct OPAR_CFW_CfgSetCharacterSet *)((char*)mem + sizeof(rpcRespHeader_t));
    uint32_t result;

    call->h.size = call_size;
    call->api_tag = TAG_CFW_CfgSetCharacterSet;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    ipar->chset = chset;
    rpcSendCall(RPC_CHANNEL, call);

    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

uint32_t CFW_CfgSetIndicatorEvent(uint16_t ind, CFW_SIM_ID nSimID)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CFW_CfgSetIndicatorEvent) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_CFW_CfgSetIndicatorEvent);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct IPAR_CFW_CfgSetIndicatorEvent *ipar = (struct IPAR_CFW_CfgSetIndicatorEvent *)((char*)mem + sizeof(rpcCallHeader_t));
    struct OPAR_CFW_CfgSetIndicatorEvent *opar = (struct OPAR_CFW_CfgSetIndicatorEvent *)((char*)mem + sizeof(rpcRespHeader_t));
    uint32_t result;

    call->h.size = call_size;
    call->api_tag = TAG_CFW_CfgSetIndicatorEvent;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    ipar->ind = ind;
    ipar->nSimID = nSimID;
    rpcSendCall(RPC_CHANNEL, call);

    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

uint32_t CFW_CfgGetIndicatorEvent(uint16_t *ind, CFW_SIM_ID nSimID)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CFW_CfgGetIndicatorEvent) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_CFW_CfgGetIndicatorEvent);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct IPAR_CFW_CfgGetIndicatorEvent *ipar = (struct IPAR_CFW_CfgGetIndicatorEvent *)((char*)mem + sizeof(rpcCallHeader_t));
    struct OPAR_CFW_CfgGetIndicatorEvent *opar = (struct OPAR_CFW_CfgGetIndicatorEvent *)((char*)mem + sizeof(rpcRespHeader_t));
    uint32_t result;

    call->h.size = call_size;
    call->api_tag = TAG_CFW_CfgGetIndicatorEvent;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    ipar->nSimID = nSimID;
    rpcSendCall(RPC_CHANNEL, call);

    *ind = opar->ind;
    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

uint32_t CFW_SetQualReport(bool bStart, CFW_SIM_ID nSimID)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CFW_SetQualReport) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_CFW_SetQualReport);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct IPAR_CFW_SetQualReport *ipar = (struct IPAR_CFW_SetQualReport *)((char*)mem + sizeof(rpcCallHeader_t));
    struct OPAR_CFW_SetQualReport *opar = (struct OPAR_CFW_SetQualReport *)((char*)mem + sizeof(rpcRespHeader_t));
    uint32_t result;

    call->h.size = call_size;
    call->api_tag = TAG_CFW_SetQualReport;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    ipar->bStart = bStart;
    ipar->nSimID = nSimID;
    rpcSendCall(RPC_CHANNEL, call);

    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

uint32_t CFW_CfgSetFirstPowerOnFlag(bool bFPO)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CFW_CfgSetFirstPowerOnFlag) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_CFW_CfgSetFirstPowerOnFlag);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct IPAR_CFW_CfgSetFirstPowerOnFlag *ipar = (struct IPAR_CFW_CfgSetFirstPowerOnFlag *)((char*)mem + sizeof(rpcCallHeader_t));
    struct OPAR_CFW_CfgSetFirstPowerOnFlag *opar = (struct OPAR_CFW_CfgSetFirstPowerOnFlag *)((char*)mem + sizeof(rpcRespHeader_t));
    uint32_t result;

    call->h.size = call_size;
    call->api_tag = TAG_CFW_CfgSetFirstPowerOnFlag;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    ipar->bFPO = bFPO;
    rpcSendCall(RPC_CHANNEL, call);

    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

uint32_t CFW_CfgCheckFirstPowerOnFlag(bool *bFPO)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + 0 + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_CFW_CfgCheckFirstPowerOnFlag);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct OPAR_CFW_CfgCheckFirstPowerOnFlag *opar = (struct OPAR_CFW_CfgCheckFirstPowerOnFlag *)((char*)mem + sizeof(rpcRespHeader_t));
    uint32_t result;

    call->h.size = call_size;
    call->api_tag = TAG_CFW_CfgCheckFirstPowerOnFlag;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    rpcSendCall(RPC_CHANNEL, call);

    *bFPO = opar->bFPO;
    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

uint32_t CFW_CfgGetEventReport(uint16_t *pMer)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + 0 + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_CFW_CfgGetEventReport);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct OPAR_CFW_CfgGetEventReport *opar = (struct OPAR_CFW_CfgGetEventReport *)((char*)mem + sizeof(rpcRespHeader_t));
    uint32_t result;

    call->h.size = call_size;
    call->api_tag = TAG_CFW_CfgGetEventReport;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    rpcSendCall(RPC_CHANNEL, call);

    *pMer = opar->pMer;
    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

uint32_t CFW_CfgSetEventReport(uint16_t pMer)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CFW_CfgSetEventReport) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_CFW_CfgSetEventReport);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct IPAR_CFW_CfgSetEventReport *ipar = (struct IPAR_CFW_CfgSetEventReport *)((char*)mem + sizeof(rpcCallHeader_t));
    struct OPAR_CFW_CfgSetEventReport *opar = (struct OPAR_CFW_CfgSetEventReport *)((char*)mem + sizeof(rpcRespHeader_t));
    uint32_t result;

    call->h.size = call_size;
    call->api_tag = TAG_CFW_CfgSetEventReport;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    ipar->pMer = pMer;
    rpcSendCall(RPC_CHANNEL, call);

    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

uint32_t CFW_CfgSetResultCodePresentMode(uint8_t nMode)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CFW_CfgSetResultCodePresentMode) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_CFW_CfgSetResultCodePresentMode);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct IPAR_CFW_CfgSetResultCodePresentMode *ipar = (struct IPAR_CFW_CfgSetResultCodePresentMode *)((char*)mem + sizeof(rpcCallHeader_t));
    struct OPAR_CFW_CfgSetResultCodePresentMode *opar = (struct OPAR_CFW_CfgSetResultCodePresentMode *)((char*)mem + sizeof(rpcRespHeader_t));
    uint32_t result;

    call->h.size = call_size;
    call->api_tag = TAG_CFW_CfgSetResultCodePresentMode;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    ipar->nMode = nMode;
    rpcSendCall(RPC_CHANNEL, call);

    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

uint32_t CFW_CfgGetResultCodePresentMode(uint8_t *nMode)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + 0 + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_CFW_CfgGetResultCodePresentMode);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct OPAR_CFW_CfgGetResultCodePresentMode *opar = (struct OPAR_CFW_CfgGetResultCodePresentMode *)((char*)mem + sizeof(rpcRespHeader_t));
    uint32_t result;

    call->h.size = call_size;
    call->api_tag = TAG_CFW_CfgGetResultCodePresentMode;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    rpcSendCall(RPC_CHANNEL, call);

    *nMode = opar->nMode;
    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

uint32_t CFW_CfgSetResultCodeFormatMode(uint8_t pMode)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CFW_CfgSetResultCodeFormatMode) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_CFW_CfgSetResultCodeFormatMode);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct IPAR_CFW_CfgSetResultCodeFormatMode *ipar = (struct IPAR_CFW_CfgSetResultCodeFormatMode *)((char*)mem + sizeof(rpcCallHeader_t));
    struct OPAR_CFW_CfgSetResultCodeFormatMode *opar = (struct OPAR_CFW_CfgSetResultCodeFormatMode *)((char*)mem + sizeof(rpcRespHeader_t));
    uint32_t result;

    call->h.size = call_size;
    call->api_tag = TAG_CFW_CfgSetResultCodeFormatMode;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    ipar->pMode = pMode;
    rpcSendCall(RPC_CHANNEL, call);

    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

uint32_t CFW_CfgGetResultCodeFormatMode(uint8_t *pMode)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + 0 + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_CFW_CfgGetResultCodeFormatMode);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct OPAR_CFW_CfgGetResultCodeFormatMode *opar = (struct OPAR_CFW_CfgGetResultCodeFormatMode *)((char*)mem + sizeof(rpcRespHeader_t));
    uint32_t result;

    call->h.size = call_size;
    call->api_tag = TAG_CFW_CfgGetResultCodeFormatMode;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    rpcSendCall(RPC_CHANNEL, call);

    *pMode = opar->pMode;
    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

uint32_t CFW_CfgSetEchoMode(uint8_t nMode)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CFW_CfgSetEchoMode) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_CFW_CfgSetEchoMode);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct IPAR_CFW_CfgSetEchoMode *ipar = (struct IPAR_CFW_CfgSetEchoMode *)((char*)mem + sizeof(rpcCallHeader_t));
    struct OPAR_CFW_CfgSetEchoMode *opar = (struct OPAR_CFW_CfgSetEchoMode *)((char*)mem + sizeof(rpcRespHeader_t));
    uint32_t result;

    call->h.size = call_size;
    call->api_tag = TAG_CFW_CfgSetEchoMode;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    ipar->nMode = nMode;
    rpcSendCall(RPC_CHANNEL, call);

    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

uint32_t CFW_CfgGetEchoMode(uint8_t *nMode)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + 0 + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_CFW_CfgGetEchoMode);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct OPAR_CFW_CfgGetEchoMode *opar = (struct OPAR_CFW_CfgGetEchoMode *)((char*)mem + sizeof(rpcRespHeader_t));
    uint32_t result;

    call->h.size = call_size;
    call->api_tag = TAG_CFW_CfgGetEchoMode;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    rpcSendCall(RPC_CHANNEL, call);

    *nMode = opar->nMode;
    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

uint32_t CFW_CfgGetExtendedErrorV1(CFW_EXTENDED_ERROR *pExtendedError)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + 0 + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_CFW_CfgGetExtendedErrorV1);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct OPAR_CFW_CfgGetExtendedErrorV1 *opar = (struct OPAR_CFW_CfgGetExtendedErrorV1 *)((char*)mem + sizeof(rpcRespHeader_t));
    uint32_t result;

    call->h.size = call_size;
    call->api_tag = TAG_CFW_CfgGetExtendedErrorV1;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    rpcSendCall(RPC_CHANNEL, call);

    *pExtendedError = opar->pExtendedError;
    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

uint32_t CFW_CfgGetErrorReport(uint8_t *pLevel)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + 0 + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_CFW_CfgGetErrorReport);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct OPAR_CFW_CfgGetErrorReport *opar = (struct OPAR_CFW_CfgGetErrorReport *)((char*)mem + sizeof(rpcRespHeader_t));
    uint32_t result;

    call->h.size = call_size;
    call->api_tag = TAG_CFW_CfgGetErrorReport;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    rpcSendCall(RPC_CHANNEL, call);

    *pLevel = opar->pLevel;
    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

uint32_t CFW_CfgSetErrorReport(uint8_t pLevel)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CFW_CfgSetErrorReport) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_CFW_CfgSetErrorReport);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct IPAR_CFW_CfgSetErrorReport *ipar = (struct IPAR_CFW_CfgSetErrorReport *)((char*)mem + sizeof(rpcCallHeader_t));
    struct OPAR_CFW_CfgSetErrorReport *opar = (struct OPAR_CFW_CfgSetErrorReport *)((char*)mem + sizeof(rpcRespHeader_t));
    uint32_t result;

    call->h.size = call_size;
    call->api_tag = TAG_CFW_CfgSetErrorReport;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    ipar->pLevel = pLevel;
    rpcSendCall(RPC_CHANNEL, call);

    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

uint32_t CFW_CfgDeviceSwithOff(void)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + 0 + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_CFW_CfgDeviceSwithOff);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct OPAR_CFW_CfgDeviceSwithOff *opar = (struct OPAR_CFW_CfgDeviceSwithOff *)((char*)mem + sizeof(rpcRespHeader_t));
    uint32_t result;

    call->h.size = call_size;
    call->api_tag = TAG_CFW_CfgDeviceSwithOff;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    rpcSendCall(RPC_CHANNEL, call);

    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

uint32_t CFW_CfgSetConnectResultFormatCode(uint8_t nValue)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CFW_CfgSetConnectResultFormatCode) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_CFW_CfgSetConnectResultFormatCode);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct IPAR_CFW_CfgSetConnectResultFormatCode *ipar = (struct IPAR_CFW_CfgSetConnectResultFormatCode *)((char*)mem + sizeof(rpcCallHeader_t));
    struct OPAR_CFW_CfgSetConnectResultFormatCode *opar = (struct OPAR_CFW_CfgSetConnectResultFormatCode *)((char*)mem + sizeof(rpcRespHeader_t));
    uint32_t result;

    call->h.size = call_size;
    call->api_tag = TAG_CFW_CfgSetConnectResultFormatCode;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    ipar->nValue = nValue;
    rpcSendCall(RPC_CHANNEL, call);

    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

uint32_t CFW_CfgGetConnectResultFormatCode(uint8_t *nValue)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + 0 + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_CFW_CfgGetConnectResultFormatCode);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct OPAR_CFW_CfgGetConnectResultFormatCode *opar = (struct OPAR_CFW_CfgGetConnectResultFormatCode *)((char*)mem + sizeof(rpcRespHeader_t));
    uint32_t result;

    call->h.size = call_size;
    call->api_tag = TAG_CFW_CfgGetConnectResultFormatCode;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    rpcSendCall(RPC_CHANNEL, call);

    *nValue = opar->nValue;
    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

uint32_t CFW_CfgSetSoftReset(uint8_t cause)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CFW_CfgSetSoftReset) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_CFW_CfgSetSoftReset);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct IPAR_CFW_CfgSetSoftReset *ipar = (struct IPAR_CFW_CfgSetSoftReset *)((char*)mem + sizeof(rpcCallHeader_t));
    struct OPAR_CFW_CfgSetSoftReset *opar = (struct OPAR_CFW_CfgSetSoftReset *)((char*)mem + sizeof(rpcRespHeader_t));
    uint32_t result;

    call->h.size = call_size;
    call->api_tag = TAG_CFW_CfgSetSoftReset;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    ipar->cause = cause;
    rpcSendCall(RPC_CHANNEL, call);

    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

uint32_t CFW_CfgGetSoftReset(uint8_t *cause)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + 0 + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_CFW_CfgGetSoftReset);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct OPAR_CFW_CfgGetSoftReset *opar = (struct OPAR_CFW_CfgGetSoftReset *)((char*)mem + sizeof(rpcRespHeader_t));
    uint32_t result;

    call->h.size = call_size;
    call->api_tag = TAG_CFW_CfgGetSoftReset;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    rpcSendCall(RPC_CHANNEL, call);

    *cause = opar->cause;
    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

uint32_t CFW_CfgSetSmsStorage(uint8_t nStorage, CFW_SIM_ID nSimID)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CFW_CfgSetSmsStorage) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_CFW_CfgSetSmsStorage);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct IPAR_CFW_CfgSetSmsStorage *ipar = (struct IPAR_CFW_CfgSetSmsStorage *)((char*)mem + sizeof(rpcCallHeader_t));
    struct OPAR_CFW_CfgSetSmsStorage *opar = (struct OPAR_CFW_CfgSetSmsStorage *)((char*)mem + sizeof(rpcRespHeader_t));
    uint32_t result;

    call->h.size = call_size;
    call->api_tag = TAG_CFW_CfgSetSmsStorage;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    ipar->nStorage = nStorage;
    ipar->nSimID = nSimID;
    rpcSendCall(RPC_CHANNEL, call);

    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

uint32_t CFW_CfgGetSmsStorage(uint8_t *nStorage, CFW_SIM_ID nSimID)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CFW_CfgGetSmsStorage) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_CFW_CfgGetSmsStorage);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct IPAR_CFW_CfgGetSmsStorage *ipar = (struct IPAR_CFW_CfgGetSmsStorage *)((char*)mem + sizeof(rpcCallHeader_t));
    struct OPAR_CFW_CfgGetSmsStorage *opar = (struct OPAR_CFW_CfgGetSmsStorage *)((char*)mem + sizeof(rpcRespHeader_t));
    uint32_t result;

    call->h.size = call_size;
    call->api_tag = TAG_CFW_CfgGetSmsStorage;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    ipar->nSimID = nSimID;
    rpcSendCall(RPC_CHANNEL, call);

    *nStorage = opar->nStorage;
    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

uint32_t CFW_CfgSetNewSmsOption(uint8_t nOption, uint8_t nNewSmsStorage, CFW_SIM_ID nSimID)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CFW_CfgSetNewSmsOption) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_CFW_CfgSetNewSmsOption);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct IPAR_CFW_CfgSetNewSmsOption *ipar = (struct IPAR_CFW_CfgSetNewSmsOption *)((char*)mem + sizeof(rpcCallHeader_t));
    struct OPAR_CFW_CfgSetNewSmsOption *opar = (struct OPAR_CFW_CfgSetNewSmsOption *)((char*)mem + sizeof(rpcRespHeader_t));
    uint32_t result;

    call->h.size = call_size;
    call->api_tag = TAG_CFW_CfgSetNewSmsOption;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    ipar->nOption = nOption;
    ipar->nNewSmsStorage = nNewSmsStorage;
    ipar->nSimID = nSimID;
    rpcSendCall(RPC_CHANNEL, call);

    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

uint32_t CFW_CfgGetNewSmsOption(uint8_t *nOption, uint8_t *nNewSmsStorage, CFW_SIM_ID nSimID)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CFW_CfgGetNewSmsOption) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_CFW_CfgGetNewSmsOption);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct IPAR_CFW_CfgGetNewSmsOption *ipar = (struct IPAR_CFW_CfgGetNewSmsOption *)((char*)mem + sizeof(rpcCallHeader_t));
    struct OPAR_CFW_CfgGetNewSmsOption *opar = (struct OPAR_CFW_CfgGetNewSmsOption *)((char*)mem + sizeof(rpcRespHeader_t));
    uint32_t result;

    call->h.size = call_size;
    call->api_tag = TAG_CFW_CfgGetNewSmsOption;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    ipar->nSimID = nSimID;
    rpcSendCall(RPC_CHANNEL, call);

    *nOption = opar->nOption;
    *nNewSmsStorage = opar->nNewSmsStorage;
    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

uint32_t CFW_CfgSetSmsOverflowInd(uint8_t nMode, CFW_SIM_ID nSimID)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CFW_CfgSetSmsOverflowInd) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_CFW_CfgSetSmsOverflowInd);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct IPAR_CFW_CfgSetSmsOverflowInd *ipar = (struct IPAR_CFW_CfgSetSmsOverflowInd *)((char*)mem + sizeof(rpcCallHeader_t));
    struct OPAR_CFW_CfgSetSmsOverflowInd *opar = (struct OPAR_CFW_CfgSetSmsOverflowInd *)((char*)mem + sizeof(rpcRespHeader_t));
    uint32_t result;

    call->h.size = call_size;
    call->api_tag = TAG_CFW_CfgSetSmsOverflowInd;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    ipar->nMode = nMode;
    ipar->nSimID = nSimID;
    rpcSendCall(RPC_CHANNEL, call);

    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

uint32_t CFW_CfgGetSmsOverflowInd(uint8_t *nMode, CFW_SIM_ID nSimID)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CFW_CfgGetSmsOverflowInd) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_CFW_CfgGetSmsOverflowInd);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct IPAR_CFW_CfgGetSmsOverflowInd *ipar = (struct IPAR_CFW_CfgGetSmsOverflowInd *)((char*)mem + sizeof(rpcCallHeader_t));
    struct OPAR_CFW_CfgGetSmsOverflowInd *opar = (struct OPAR_CFW_CfgGetSmsOverflowInd *)((char*)mem + sizeof(rpcRespHeader_t));
    uint32_t result;

    call->h.size = call_size;
    call->api_tag = TAG_CFW_CfgGetSmsOverflowInd;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    ipar->nSimID = nSimID;
    rpcSendCall(RPC_CHANNEL, call);

    *nMode = opar->nMode;
    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

uint32_t CFW_CfgSetSmsFormat(uint8_t nFormat, CFW_SIM_ID nSimID)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CFW_CfgSetSmsFormat) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_CFW_CfgSetSmsFormat);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct IPAR_CFW_CfgSetSmsFormat *ipar = (struct IPAR_CFW_CfgSetSmsFormat *)((char*)mem + sizeof(rpcCallHeader_t));
    struct OPAR_CFW_CfgSetSmsFormat *opar = (struct OPAR_CFW_CfgSetSmsFormat *)((char*)mem + sizeof(rpcRespHeader_t));
    uint32_t result;

    call->h.size = call_size;
    call->api_tag = TAG_CFW_CfgSetSmsFormat;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    ipar->nFormat = nFormat;
    ipar->nSimID = nSimID;
    rpcSendCall(RPC_CHANNEL, call);

    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

uint32_t CFW_CfgGetSmsFormat(uint8_t *nFormat, CFW_SIM_ID nSimID)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CFW_CfgGetSmsFormat) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_CFW_CfgGetSmsFormat);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct IPAR_CFW_CfgGetSmsFormat *ipar = (struct IPAR_CFW_CfgGetSmsFormat *)((char*)mem + sizeof(rpcCallHeader_t));
    struct OPAR_CFW_CfgGetSmsFormat *opar = (struct OPAR_CFW_CfgGetSmsFormat *)((char*)mem + sizeof(rpcRespHeader_t));
    uint32_t result;

    call->h.size = call_size;
    call->api_tag = TAG_CFW_CfgGetSmsFormat;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    ipar->nSimID = nSimID;
    rpcSendCall(RPC_CHANNEL, call);

    *nFormat = opar->nFormat;
    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

uint32_t CFW_CfgSetSmsShowTextModeParam(uint8_t nShow, CFW_SIM_ID nSimID)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CFW_CfgSetSmsShowTextModeParam) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_CFW_CfgSetSmsShowTextModeParam);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct IPAR_CFW_CfgSetSmsShowTextModeParam *ipar = (struct IPAR_CFW_CfgSetSmsShowTextModeParam *)((char*)mem + sizeof(rpcCallHeader_t));
    struct OPAR_CFW_CfgSetSmsShowTextModeParam *opar = (struct OPAR_CFW_CfgSetSmsShowTextModeParam *)((char*)mem + sizeof(rpcRespHeader_t));
    uint32_t result;

    call->h.size = call_size;
    call->api_tag = TAG_CFW_CfgSetSmsShowTextModeParam;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    ipar->nShow = nShow;
    ipar->nSimID = nSimID;
    rpcSendCall(RPC_CHANNEL, call);

    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

uint32_t CFW_CfgGetSmsShowTextModeParam(uint8_t *nShow, CFW_SIM_ID nSimID)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CFW_CfgGetSmsShowTextModeParam) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_CFW_CfgGetSmsShowTextModeParam);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct IPAR_CFW_CfgGetSmsShowTextModeParam *ipar = (struct IPAR_CFW_CfgGetSmsShowTextModeParam *)((char*)mem + sizeof(rpcCallHeader_t));
    struct OPAR_CFW_CfgGetSmsShowTextModeParam *opar = (struct OPAR_CFW_CfgGetSmsShowTextModeParam *)((char*)mem + sizeof(rpcRespHeader_t));
    uint32_t result;

    call->h.size = call_size;
    call->api_tag = TAG_CFW_CfgGetSmsShowTextModeParam;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    ipar->nSimID = nSimID;
    rpcSendCall(RPC_CHANNEL, call);

    *nShow = opar->nShow;
    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

uint32_t CFW_CfgSetSmsParam(CFW_SMS_PARAMETER *pInfo, uint16_t nUTI, CFW_SIM_ID nSimID)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CFW_CfgSetSmsParam) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_CFW_CfgSetSmsParam);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct IPAR_CFW_CfgSetSmsParam *ipar = (struct IPAR_CFW_CfgSetSmsParam *)((char*)mem + sizeof(rpcCallHeader_t));
    struct OPAR_CFW_CfgSetSmsParam *opar = (struct OPAR_CFW_CfgSetSmsParam *)((char*)mem + sizeof(rpcRespHeader_t));
    uint32_t result;

    call->h.size = call_size;
    call->api_tag = TAG_CFW_CfgSetSmsParam;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    ipar->pInfo = *pInfo;
    ipar->nUTI = nUTI;
    ipar->nSimID = nSimID;
    rpcSendCall(RPC_CHANNEL, call);

    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

uint32_t CFW_CfgGetSmsParam(CFW_SMS_PARAMETER *pInfo, uint8_t nIndex, CFW_SIM_ID nSimID)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CFW_CfgGetSmsParam) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_CFW_CfgGetSmsParam);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct IPAR_CFW_CfgGetSmsParam *ipar = (struct IPAR_CFW_CfgGetSmsParam *)((char*)mem + sizeof(rpcCallHeader_t));
    struct OPAR_CFW_CfgGetSmsParam *opar = (struct OPAR_CFW_CfgGetSmsParam *)((char*)mem + sizeof(rpcRespHeader_t));
    uint32_t result;

    call->h.size = call_size;
    call->api_tag = TAG_CFW_CfgGetSmsParam;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    ipar->nIndex = nIndex;
    ipar->nSimID = nSimID;
    rpcSendCall(RPC_CHANNEL, call);

    *pInfo = opar->pInfo;
    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

uint32_t CFW_CfgSetDefaultSmsParam(CFW_SMS_PARAMETER *pInfo, CFW_SIM_ID nSimID)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CFW_CfgSetDefaultSmsParam) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_CFW_CfgSetDefaultSmsParam);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct IPAR_CFW_CfgSetDefaultSmsParam *ipar = (struct IPAR_CFW_CfgSetDefaultSmsParam *)((char*)mem + sizeof(rpcCallHeader_t));
    struct OPAR_CFW_CfgSetDefaultSmsParam *opar = (struct OPAR_CFW_CfgSetDefaultSmsParam *)((char*)mem + sizeof(rpcRespHeader_t));
    uint32_t result;

    call->h.size = call_size;
    call->api_tag = TAG_CFW_CfgSetDefaultSmsParam;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    ipar->pInfo = *pInfo;
    ipar->nSimID = nSimID;
    rpcSendCall(RPC_CHANNEL, call);

    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

uint32_t CFW_CfgGetDefaultSmsParam(CFW_SMS_PARAMETER *pInfo, CFW_SIM_ID nSimID)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CFW_CfgGetDefaultSmsParam) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_CFW_CfgGetDefaultSmsParam);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct IPAR_CFW_CfgGetDefaultSmsParam *ipar = (struct IPAR_CFW_CfgGetDefaultSmsParam *)((char*)mem + sizeof(rpcCallHeader_t));
    struct OPAR_CFW_CfgGetDefaultSmsParam *opar = (struct OPAR_CFW_CfgGetDefaultSmsParam *)((char*)mem + sizeof(rpcRespHeader_t));
    uint32_t result;

    call->h.size = call_size;
    call->api_tag = TAG_CFW_CfgGetDefaultSmsParam;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    ipar->nSimID = nSimID;
    rpcSendCall(RPC_CHANNEL, call);

    *pInfo = opar->pInfo;
    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

uint32_t CFW_CfgSetSmsStorageInfo(CFW_SMS_STORAGE_INFO *pStorageInfo, uint16_t nUTI, CFW_SIM_ID nSimID)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CFW_CfgSetSmsStorageInfo) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_CFW_CfgSetSmsStorageInfo);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct IPAR_CFW_CfgSetSmsStorageInfo *ipar = (struct IPAR_CFW_CfgSetSmsStorageInfo *)((char*)mem + sizeof(rpcCallHeader_t));
    struct OPAR_CFW_CfgSetSmsStorageInfo *opar = (struct OPAR_CFW_CfgSetSmsStorageInfo *)((char*)mem + sizeof(rpcRespHeader_t));
    uint32_t result;

    call->h.size = call_size;
    call->api_tag = TAG_CFW_CfgSetSmsStorageInfo;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    ipar->pStorageInfo = *pStorageInfo;
    ipar->nUTI = nUTI;
    ipar->nSimID = nSimID;
    rpcSendCall(RPC_CHANNEL, call);

    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

uint32_t CFW_CfgGetSmsStorageInfo(CFW_SMS_STORAGE_INFO *pStorageInfo, uint16_t nUTI, CFW_SIM_ID nSimID)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CFW_CfgGetSmsStorageInfo) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_CFW_CfgGetSmsStorageInfo);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct IPAR_CFW_CfgGetSmsStorageInfo *ipar = (struct IPAR_CFW_CfgGetSmsStorageInfo *)((char*)mem + sizeof(rpcCallHeader_t));
    struct OPAR_CFW_CfgGetSmsStorageInfo *opar = (struct OPAR_CFW_CfgGetSmsStorageInfo *)((char*)mem + sizeof(rpcRespHeader_t));
    uint32_t result;

    call->h.size = call_size;
    call->api_tag = TAG_CFW_CfgGetSmsStorageInfo;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    ipar->nUTI = nUTI;
    ipar->nSimID = nSimID;
    rpcSendCall(RPC_CHANNEL, call);

    *pStorageInfo = opar->pStorageInfo;
    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

uint32_t CFW_SimGetSmsTotalNum(uint16_t nUTI, CFW_SIM_ID nSimID)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CFW_SimGetSmsTotalNum) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_CFW_SimGetSmsTotalNum);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct IPAR_CFW_SimGetSmsTotalNum *ipar = (struct IPAR_CFW_SimGetSmsTotalNum *)((char*)mem + sizeof(rpcCallHeader_t));
    struct OPAR_CFW_SimGetSmsTotalNum *opar = (struct OPAR_CFW_SimGetSmsTotalNum *)((char*)mem + sizeof(rpcRespHeader_t));
    uint32_t result;

    call->h.size = call_size;
    call->api_tag = TAG_CFW_SimGetSmsTotalNum;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    ipar->nUTI = nUTI;
    ipar->nSimID = nSimID;
    rpcSendCall(RPC_CHANNEL, call);

    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

uint32_t CFW_CfgSelectSmsService(uint8_t nService, uint8_t *pSupportedType)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CFW_CfgSelectSmsService) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_CFW_CfgSelectSmsService);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct IPAR_CFW_CfgSelectSmsService *ipar = (struct IPAR_CFW_CfgSelectSmsService *)((char*)mem + sizeof(rpcCallHeader_t));
    struct OPAR_CFW_CfgSelectSmsService *opar = (struct OPAR_CFW_CfgSelectSmsService *)((char*)mem + sizeof(rpcRespHeader_t));
    uint32_t result;

    call->h.size = call_size;
    call->api_tag = TAG_CFW_CfgSelectSmsService;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    ipar->nService = nService;
    rpcSendCall(RPC_CHANNEL, call);

    *pSupportedType = opar->pSupportedType;
    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

uint32_t CFW_CfgQuerySmsService(uint8_t *nService)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + 0 + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_CFW_CfgQuerySmsService);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct OPAR_CFW_CfgQuerySmsService *opar = (struct OPAR_CFW_CfgQuerySmsService *)((char*)mem + sizeof(rpcRespHeader_t));
    uint32_t result;

    call->h.size = call_size;
    call->api_tag = TAG_CFW_CfgQuerySmsService;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    rpcSendCall(RPC_CHANNEL, call);

    *nService = opar->nService;
    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

uint32_t CFW_CfgSetSmsCB(uint8_t mode, uint8_t mids[6], uint8_t dcss[6])
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CFW_CfgSetSmsCB) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_CFW_CfgSetSmsCB);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct IPAR_CFW_CfgSetSmsCB *ipar = (struct IPAR_CFW_CfgSetSmsCB *)((char*)mem + sizeof(rpcCallHeader_t));
    struct OPAR_CFW_CfgSetSmsCB *opar = (struct OPAR_CFW_CfgSetSmsCB *)((char*)mem + sizeof(rpcRespHeader_t));
    uint32_t result;

    call->h.size = call_size;
    call->api_tag = TAG_CFW_CfgSetSmsCB;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    ipar->mode = mode;
    memcpy(ipar->mids, mids, sizeof(ipar->mids));
    memcpy(ipar->dcss, dcss, sizeof(ipar->dcss));
    rpcSendCall(RPC_CHANNEL, call);

    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

uint32_t CFW_CfgGetSmsCB(uint8_t *pMode, uint8_t mids[6], uint8_t dcss[6])
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + 0 + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_CFW_CfgGetSmsCB);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct OPAR_CFW_CfgGetSmsCB *opar = (struct OPAR_CFW_CfgGetSmsCB *)((char*)mem + sizeof(rpcRespHeader_t));
    uint32_t result;

    call->h.size = call_size;
    call->api_tag = TAG_CFW_CfgGetSmsCB;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    rpcSendCall(RPC_CHANNEL, call);

    *pMode = opar->pMode;
    memcpy(mids, opar->mids, sizeof(opar->mids));
    memcpy(dcss, opar->dcss, sizeof(opar->dcss));
    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

void CFW_SetSimStatus(CFW_SIM_ID nSimID, CFW_SIM_STATUS nSimStatus)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CFW_SetSimStatus) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + 0;
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct IPAR_CFW_SetSimStatus *ipar = (struct IPAR_CFW_SetSimStatus *)((char*)mem + sizeof(rpcCallHeader_t));

    call->h.size = call_size;
    call->api_tag = TAG_CFW_SetSimStatus;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    ipar->nSimID = nSimID;
    ipar->nSimStatus = nSimStatus;
    rpcSendCall(RPC_CHANNEL, call);

    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
}

CFW_SIM_STATUS CFW_GetSimStatus(CFW_SIM_ID nSimID)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CFW_GetSimStatus) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_CFW_GetSimStatus);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct IPAR_CFW_GetSimStatus *ipar = (struct IPAR_CFW_GetSimStatus *)((char*)mem + sizeof(rpcCallHeader_t));
    struct OPAR_CFW_GetSimStatus *opar = (struct OPAR_CFW_GetSimStatus *)((char*)mem + sizeof(rpcRespHeader_t));
    CFW_SIM_STATUS result;

    call->h.size = call_size;
    call->api_tag = TAG_CFW_GetSimStatus;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    ipar->nSimID = nSimID;
    rpcSendCall(RPC_CHANNEL, call);

    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

uint32_t CFW_SimClose(uint16_t nUTI, CFW_SIM_ID nSimID)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CFW_SimClose) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_CFW_SimClose);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct IPAR_CFW_SimClose *ipar = (struct IPAR_CFW_SimClose *)((char*)mem + sizeof(rpcCallHeader_t));
    struct OPAR_CFW_SimClose *opar = (struct OPAR_CFW_SimClose *)((char*)mem + sizeof(rpcRespHeader_t));
    uint32_t result;

    call->h.size = call_size;
    call->api_tag = TAG_CFW_SimClose;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    ipar->nUTI = nUTI;
    ipar->nSimID = nSimID;
    rpcSendCall(RPC_CHANNEL, call);

    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

uint32_t CFW_GetStackSimFileID(uint16_t n3GppFileID, uint16_t EFPath, CFW_SIM_ID nSimID)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CFW_GetStackSimFileID) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_CFW_GetStackSimFileID);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct IPAR_CFW_GetStackSimFileID *ipar = (struct IPAR_CFW_GetStackSimFileID *)((char*)mem + sizeof(rpcCallHeader_t));
    struct OPAR_CFW_GetStackSimFileID *opar = (struct OPAR_CFW_GetStackSimFileID *)((char*)mem + sizeof(rpcRespHeader_t));
    uint32_t result;

    call->h.size = call_size;
    call->api_tag = TAG_CFW_GetStackSimFileID;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    ipar->n3GppFileID = n3GppFileID;
    ipar->EFPath = EFPath;
    ipar->nSimID = nSimID;
    rpcSendCall(RPC_CHANNEL, call);

    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

uint32_t CFW_SimReadBinary(uint8_t nFileId, uint8_t nOffset, uint8_t nBytesToRead, uint16_t nUTI, CFW_SIM_ID nSimID)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CFW_SimReadBinary) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_CFW_SimReadBinary);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct IPAR_CFW_SimReadBinary *ipar = (struct IPAR_CFW_SimReadBinary *)((char*)mem + sizeof(rpcCallHeader_t));
    struct OPAR_CFW_SimReadBinary *opar = (struct OPAR_CFW_SimReadBinary *)((char*)mem + sizeof(rpcRespHeader_t));
    uint32_t result;

    call->h.size = call_size;
    call->api_tag = TAG_CFW_SimReadBinary;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    ipar->nFileId = nFileId;
    ipar->nOffset = nOffset;
    ipar->nBytesToRead = nBytesToRead;
    ipar->nUTI = nUTI;
    ipar->nSimID = nSimID;
    rpcSendCall(RPC_CHANNEL, call);

    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

uint32_t CFW_SimUpdateBinary(uint8_t nFileId, uint8_t nOffset, uint8_t * pData, uint8_t nBytesToWrite, uint16_t nUTI, CFW_SIM_ID nSimID)
{
    size_t ivar_size = ALIGNUP8(nBytesToWrite+1);
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CFW_SimUpdateBinary) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_CFW_SimUpdateBinary);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct IPAR_CFW_SimUpdateBinary *ipar = (struct IPAR_CFW_SimUpdateBinary *)((char*)mem + sizeof(rpcCallHeader_t));
    struct OPAR_CFW_SimUpdateBinary *opar = (struct OPAR_CFW_SimUpdateBinary *)((char*)mem + sizeof(rpcRespHeader_t));
    char *ivar_ptr = (char*)mem + sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CFW_SimUpdateBinary);
    uint32_t result;

    call->h.size = call_size;
    call->api_tag = TAG_CFW_SimUpdateBinary;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    ipar->nFileId = nFileId;
    ipar->nOffset = nOffset;
    memcpy(ivar_ptr, pData, nBytesToWrite);
    ivar_ptr[nBytesToWrite] = (char)0;
    ivar_ptr += ALIGNUP8(nBytesToWrite + 1);
    ipar->nBytesToWrite = nBytesToWrite;
    ipar->nUTI = nUTI;
    ipar->nSimID = nSimID;
    rpcSendCall(RPC_CHANNEL, call);

    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

uint32_t CFW_SimReadRecord(uint8_t nFileID, uint8_t nRecordNum, uint16_t nUTI, CFW_SIM_ID nSimID)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CFW_SimReadRecord) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_CFW_SimReadRecord);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct IPAR_CFW_SimReadRecord *ipar = (struct IPAR_CFW_SimReadRecord *)((char*)mem + sizeof(rpcCallHeader_t));
    struct OPAR_CFW_SimReadRecord *opar = (struct OPAR_CFW_SimReadRecord *)((char*)mem + sizeof(rpcRespHeader_t));
    uint32_t result;

    call->h.size = call_size;
    call->api_tag = TAG_CFW_SimReadRecord;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    ipar->nFileID = nFileID;
    ipar->nRecordNum = nRecordNum;
    ipar->nUTI = nUTI;
    ipar->nSimID = nSimID;
    rpcSendCall(RPC_CHANNEL, call);

    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

uint32_t CFW_SimUpdateRecord(uint8_t nFileID, uint8_t nRecordNum, uint8_t nRecordSize, uint8_t * pData, uint16_t nUTI, CFW_SIM_ID nSimID)
{
    size_t ivar_size = ALIGNUP8(nRecordSize+1);
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CFW_SimUpdateRecord) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_CFW_SimUpdateRecord);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct IPAR_CFW_SimUpdateRecord *ipar = (struct IPAR_CFW_SimUpdateRecord *)((char*)mem + sizeof(rpcCallHeader_t));
    struct OPAR_CFW_SimUpdateRecord *opar = (struct OPAR_CFW_SimUpdateRecord *)((char*)mem + sizeof(rpcRespHeader_t));
    char *ivar_ptr = (char*)mem + sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CFW_SimUpdateRecord);
    uint32_t result;

    call->h.size = call_size;
    call->api_tag = TAG_CFW_SimUpdateRecord;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    ipar->nFileID = nFileID;
    ipar->nRecordNum = nRecordNum;
    ipar->nRecordSize = nRecordSize;
    memcpy(ivar_ptr, pData, nRecordSize);
    ivar_ptr[nRecordSize] = (char)0;
    ivar_ptr += ALIGNUP8(nRecordSize + 1);
    ipar->nUTI = nUTI;
    ipar->nSimID = nSimID;
    rpcSendCall(RPC_CHANNEL, call);

    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

uint32_t CFW_SimGetFileStatus(uint8_t nFileID, uint16_t nUTI, CFW_SIM_ID nSimID)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CFW_SimGetFileStatus) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_CFW_SimGetFileStatus);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct IPAR_CFW_SimGetFileStatus *ipar = (struct IPAR_CFW_SimGetFileStatus *)((char*)mem + sizeof(rpcCallHeader_t));
    struct OPAR_CFW_SimGetFileStatus *opar = (struct OPAR_CFW_SimGetFileStatus *)((char*)mem + sizeof(rpcRespHeader_t));
    uint32_t result;

    call->h.size = call_size;
    call->api_tag = TAG_CFW_SimGetFileStatus;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    ipar->nFileID = nFileID;
    ipar->nUTI = nUTI;
    ipar->nSimID = nSimID;
    rpcSendCall(RPC_CHANNEL, call);

    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

uint32_t CFW_SmsMoInit(uint16_t nUti, CFW_SIM_ID nSimID)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CFW_SmsMoInit) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_CFW_SmsMoInit);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct IPAR_CFW_SmsMoInit *ipar = (struct IPAR_CFW_SmsMoInit *)((char*)mem + sizeof(rpcCallHeader_t));
    struct OPAR_CFW_SmsMoInit *opar = (struct OPAR_CFW_SmsMoInit *)((char*)mem + sizeof(rpcRespHeader_t));
    uint32_t result;

    call->h.size = call_size;
    call->api_tag = TAG_CFW_SmsMoInit;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    ipar->nUti = nUti;
    ipar->nSimID = nSimID;
    rpcSendCall(RPC_CHANNEL, call);

    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

uint32_t CFW_SimInit(bool bRstSim, CFW_SIM_ID nSimID)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CFW_SimInit) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_CFW_SimInit);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct IPAR_CFW_SimInit *ipar = (struct IPAR_CFW_SimInit *)((char*)mem + sizeof(rpcCallHeader_t));
    struct OPAR_CFW_SimInit *opar = (struct OPAR_CFW_SimInit *)((char*)mem + sizeof(rpcRespHeader_t));
    uint32_t result;

    call->h.size = call_size;
    call->api_tag = TAG_CFW_SimInit;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    ipar->bRstSim = bRstSim;
    ipar->nSimID = nSimID;
    rpcSendCall(RPC_CHANNEL, call);

    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

#if defined(SIM_SWITCH_ENABLE)
uint32_t CFW_CfgSetSimSwitch(uint8_t n)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CFW_CfgSetSimSwitch) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_CFW_CfgSetSimSwitch);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct IPAR_CFW_CfgSetSimSwitch *ipar = (struct IPAR_CFW_CfgSetSimSwitch *)((char*)mem + sizeof(rpcCallHeader_t));
    struct OPAR_CFW_CfgSetSimSwitch *opar = (struct OPAR_CFW_CfgSetSimSwitch *)((char*)mem + sizeof(rpcRespHeader_t));
    uint32_t result;

    call->h.size = call_size;
    call->api_tag = TAG_CFW_CfgSetSimSwitch;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    ipar->n = n;
    rpcSendCall(RPC_CHANNEL, call);

    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

#endif

#if defined(SIM_SWITCH_ENABLE)
uint32_t CFW_CfgGetSimSwitch(uint8_t *n)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + 0 + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_CFW_CfgGetSimSwitch);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct OPAR_CFW_CfgGetSimSwitch *opar = (struct OPAR_CFW_CfgGetSimSwitch *)((char*)mem + sizeof(rpcRespHeader_t));
    uint32_t result;

    call->h.size = call_size;
    call->api_tag = TAG_CFW_CfgGetSimSwitch;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    rpcSendCall(RPC_CHANNEL, call);

    *n = opar->n;
    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

#endif

#if defined(SIM_SWITCH_ENABLE)
void SimSwitch(uint8_t SimNum)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_SimSwitch) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + 0;
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct IPAR_SimSwitch *ipar = (struct IPAR_SimSwitch *)((char*)mem + sizeof(rpcCallHeader_t));

    call->h.size = call_size;
    call->api_tag = TAG_SimSwitch;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    ipar->SimNum = SimNum;
    rpcSendCall(RPC_CHANNEL, call);

    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
}

#endif

uint32_t CFW_CfgSetPbkStrorageInfo(CFW_PBK_STRORAGE_INFO *pStorageInfo)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CFW_CfgSetPbkStrorageInfo) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_CFW_CfgSetPbkStrorageInfo);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct IPAR_CFW_CfgSetPbkStrorageInfo *ipar = (struct IPAR_CFW_CfgSetPbkStrorageInfo *)((char*)mem + sizeof(rpcCallHeader_t));
    struct OPAR_CFW_CfgSetPbkStrorageInfo *opar = (struct OPAR_CFW_CfgSetPbkStrorageInfo *)((char*)mem + sizeof(rpcRespHeader_t));
    uint32_t result;

    call->h.size = call_size;
    call->api_tag = TAG_CFW_CfgSetPbkStrorageInfo;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    ipar->pStorageInfo = *pStorageInfo;
    rpcSendCall(RPC_CHANNEL, call);

    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

uint32_t CFW_CfgGetPbkStrorageInfo(CFW_PBK_STRORAGE_INFO *pStorageInfo)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + 0 + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_CFW_CfgGetPbkStrorageInfo);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct OPAR_CFW_CfgGetPbkStrorageInfo *opar = (struct OPAR_CFW_CfgGetPbkStrorageInfo *)((char*)mem + sizeof(rpcRespHeader_t));
    uint32_t result;

    call->h.size = call_size;
    call->api_tag = TAG_CFW_CfgGetPbkStrorageInfo;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    rpcSendCall(RPC_CHANNEL, call);

    *pStorageInfo = opar->pStorageInfo;
    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

uint32_t CFW_CfgSetPbkStorage(uint8_t nStorage, CFW_SIM_ID nSimID)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CFW_CfgSetPbkStorage) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_CFW_CfgSetPbkStorage);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct IPAR_CFW_CfgSetPbkStorage *ipar = (struct IPAR_CFW_CfgSetPbkStorage *)((char*)mem + sizeof(rpcCallHeader_t));
    struct OPAR_CFW_CfgSetPbkStorage *opar = (struct OPAR_CFW_CfgSetPbkStorage *)((char*)mem + sizeof(rpcRespHeader_t));
    uint32_t result;

    call->h.size = call_size;
    call->api_tag = TAG_CFW_CfgSetPbkStorage;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    ipar->nStorage = nStorage;
    ipar->nSimID = nSimID;
    rpcSendCall(RPC_CHANNEL, call);

    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

uint32_t CFW_CfgGetPbkStorage(uint8_t *nStorage, CFW_SIM_ID nSimID)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CFW_CfgGetPbkStorage) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_CFW_CfgGetPbkStorage);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct IPAR_CFW_CfgGetPbkStorage *ipar = (struct IPAR_CFW_CfgGetPbkStorage *)((char*)mem + sizeof(rpcCallHeader_t));
    struct OPAR_CFW_CfgGetPbkStorage *opar = (struct OPAR_CFW_CfgGetPbkStorage *)((char*)mem + sizeof(rpcRespHeader_t));
    uint32_t result;

    call->h.size = call_size;
    call->api_tag = TAG_CFW_CfgGetPbkStorage;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    ipar->nSimID = nSimID;
    rpcSendCall(RPC_CHANNEL, call);

    *nStorage = opar->nStorage;
    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

uint32_t CFW_CfgSetStoredPlmnList(CFW_StoredPlmnList *pPlmnL, CFW_SIM_ID nSimID)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CFW_CfgSetStoredPlmnList) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_CFW_CfgSetStoredPlmnList);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct IPAR_CFW_CfgSetStoredPlmnList *ipar = (struct IPAR_CFW_CfgSetStoredPlmnList *)((char*)mem + sizeof(rpcCallHeader_t));
    struct OPAR_CFW_CfgSetStoredPlmnList *opar = (struct OPAR_CFW_CfgSetStoredPlmnList *)((char*)mem + sizeof(rpcRespHeader_t));
    uint32_t result;

    call->h.size = call_size;
    call->api_tag = TAG_CFW_CfgSetStoredPlmnList;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    ipar->pPlmnL = *pPlmnL;
    ipar->nSimID = nSimID;
    rpcSendCall(RPC_CHANNEL, call);

    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

uint32_t CFW_CfgGetStoredPlmnList(CFW_StoredPlmnList *pPlmnL, CFW_SIM_ID nSimID)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CFW_CfgGetStoredPlmnList) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_CFW_CfgGetStoredPlmnList);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct IPAR_CFW_CfgGetStoredPlmnList *ipar = (struct IPAR_CFW_CfgGetStoredPlmnList *)((char*)mem + sizeof(rpcCallHeader_t));
    struct OPAR_CFW_CfgGetStoredPlmnList *opar = (struct OPAR_CFW_CfgGetStoredPlmnList *)((char*)mem + sizeof(rpcRespHeader_t));
    uint32_t result;

    call->h.size = call_size;
    call->api_tag = TAG_CFW_CfgGetStoredPlmnList;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    ipar->nSimID = nSimID;
    rpcSendCall(RPC_CHANNEL, call);

    *pPlmnL = opar->pPlmnL;
    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

uint32_t CFW_CfgNwSetFrequencyBand(uint8_t nBand, CFW_SIM_ID nSimID)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CFW_CfgNwSetFrequencyBand) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_CFW_CfgNwSetFrequencyBand);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct IPAR_CFW_CfgNwSetFrequencyBand *ipar = (struct IPAR_CFW_CfgNwSetFrequencyBand *)((char*)mem + sizeof(rpcCallHeader_t));
    struct OPAR_CFW_CfgNwSetFrequencyBand *opar = (struct OPAR_CFW_CfgNwSetFrequencyBand *)((char*)mem + sizeof(rpcRespHeader_t));
    uint32_t result;

    call->h.size = call_size;
    call->api_tag = TAG_CFW_CfgNwSetFrequencyBand;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    ipar->nBand = nBand;
    ipar->nSimID = nSimID;
    rpcSendCall(RPC_CHANNEL, call);

    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

uint32_t CFW_CfgNwGetFrequencyBand(uint8_t *nBand, CFW_SIM_ID nSimID)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CFW_CfgNwGetFrequencyBand) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_CFW_CfgNwGetFrequencyBand);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct IPAR_CFW_CfgNwGetFrequencyBand *ipar = (struct IPAR_CFW_CfgNwGetFrequencyBand *)((char*)mem + sizeof(rpcCallHeader_t));
    struct OPAR_CFW_CfgNwGetFrequencyBand *opar = (struct OPAR_CFW_CfgNwGetFrequencyBand *)((char*)mem + sizeof(rpcRespHeader_t));
    uint32_t result;

    call->h.size = call_size;
    call->api_tag = TAG_CFW_CfgNwGetFrequencyBand;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    ipar->nSimID = nSimID;
    rpcSendCall(RPC_CHANNEL, call);

    *nBand = opar->nBand;
    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

uint32_t CFW_CfgNwSetNetWorkMode(uint8_t nMode, CFW_SIM_ID nSimID)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CFW_CfgNwSetNetWorkMode) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_CFW_CfgNwSetNetWorkMode);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct IPAR_CFW_CfgNwSetNetWorkMode *ipar = (struct IPAR_CFW_CfgNwSetNetWorkMode *)((char*)mem + sizeof(rpcCallHeader_t));
    struct OPAR_CFW_CfgNwSetNetWorkMode *opar = (struct OPAR_CFW_CfgNwSetNetWorkMode *)((char*)mem + sizeof(rpcRespHeader_t));
    uint32_t result;

    call->h.size = call_size;
    call->api_tag = TAG_CFW_CfgNwSetNetWorkMode;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    ipar->nMode = nMode;
    ipar->nSimID = nSimID;
    rpcSendCall(RPC_CHANNEL, call);

    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

uint32_t CFW_CfgNwGetNetWorkMode(uint8_t *pMode, CFW_SIM_ID nSimID)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CFW_CfgNwGetNetWorkMode) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_CFW_CfgNwGetNetWorkMode);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct IPAR_CFW_CfgNwGetNetWorkMode *ipar = (struct IPAR_CFW_CfgNwGetNetWorkMode *)((char*)mem + sizeof(rpcCallHeader_t));
    struct OPAR_CFW_CfgNwGetNetWorkMode *opar = (struct OPAR_CFW_CfgNwGetNetWorkMode *)((char*)mem + sizeof(rpcRespHeader_t));
    uint32_t result;

    call->h.size = call_size;
    call->api_tag = TAG_CFW_CfgNwGetNetWorkMode;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    ipar->nSimID = nSimID;
    rpcSendCall(RPC_CHANNEL, call);

    *pMode = opar->pMode;
    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

uint32_t CFW_CfgSetNwStatus(uint8_t n, CFW_SIM_ID nSimID)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CFW_CfgSetNwStatus) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_CFW_CfgSetNwStatus);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct IPAR_CFW_CfgSetNwStatus *ipar = (struct IPAR_CFW_CfgSetNwStatus *)((char*)mem + sizeof(rpcCallHeader_t));
    struct OPAR_CFW_CfgSetNwStatus *opar = (struct OPAR_CFW_CfgSetNwStatus *)((char*)mem + sizeof(rpcRespHeader_t));
    uint32_t result;

    call->h.size = call_size;
    call->api_tag = TAG_CFW_CfgSetNwStatus;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    ipar->n = n;
    ipar->nSimID = nSimID;
    rpcSendCall(RPC_CHANNEL, call);

    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

uint32_t CFW_CfgGetNwStatus(uint8_t *pCmd, CFW_SIM_ID nSimID)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CFW_CfgGetNwStatus) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_CFW_CfgGetNwStatus);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct IPAR_CFW_CfgGetNwStatus *ipar = (struct IPAR_CFW_CfgGetNwStatus *)((char*)mem + sizeof(rpcCallHeader_t));
    struct OPAR_CFW_CfgGetNwStatus *opar = (struct OPAR_CFW_CfgGetNwStatus *)((char*)mem + sizeof(rpcRespHeader_t));
    uint32_t result;

    call->h.size = call_size;
    call->api_tag = TAG_CFW_CfgGetNwStatus;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    ipar->nSimID = nSimID;
    rpcSendCall(RPC_CHANNEL, call);

    *pCmd = opar->pCmd;
    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

uint32_t CFW_CfgNwGetOperatorInfo(uint8_t * *pOperatorId, uint8_t * *pOperatorName, uint32_t nIndex)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CFW_CfgNwGetOperatorInfo) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_CFW_CfgNwGetOperatorInfo);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct IPAR_CFW_CfgNwGetOperatorInfo *ipar = (struct IPAR_CFW_CfgNwGetOperatorInfo *)((char*)mem + sizeof(rpcCallHeader_t));
    struct OPAR_CFW_CfgNwGetOperatorInfo *opar = (struct OPAR_CFW_CfgNwGetOperatorInfo *)((char*)mem + sizeof(rpcRespHeader_t));
    uint32_t result;

    call->h.size = call_size;
    call->api_tag = TAG_CFW_CfgNwGetOperatorInfo;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    ipar->nIndex = nIndex;
    rpcSendCall(RPC_CHANNEL, call);

    *pOperatorId = opar->pOperatorId;
    *pOperatorName = opar->pOperatorName;
    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

uint32_t CFW_CfgNwGetOperatorId(uint8_t * *pOperatorId, uint8_t * pOperatorName)
{
    uint32_t pOperatorNameSize = strlen((const char *)pOperatorName) + 1;
    size_t ivar_size = ALIGNUP8(pOperatorNameSize);
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CFW_CfgNwGetOperatorId) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_CFW_CfgNwGetOperatorId);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct IPAR_CFW_CfgNwGetOperatorId *ipar = (struct IPAR_CFW_CfgNwGetOperatorId *)((char*)mem + sizeof(rpcCallHeader_t));
    struct OPAR_CFW_CfgNwGetOperatorId *opar = (struct OPAR_CFW_CfgNwGetOperatorId *)((char*)mem + sizeof(rpcRespHeader_t));
    char *ivar_ptr = (char*)mem + sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CFW_CfgNwGetOperatorId);
    uint32_t result;

    call->h.size = call_size;
    call->api_tag = TAG_CFW_CfgNwGetOperatorId;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    memcpy(ivar_ptr, pOperatorName, pOperatorNameSize);
    ivar_ptr += ALIGNUP8(pOperatorNameSize);
    ipar->pOperatorNameSize = pOperatorNameSize;
    rpcSendCall(RPC_CHANNEL, call);

    *pOperatorId = opar->pOperatorId;
    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

uint32_t CFW_CfgNwGetOperatorName(uint8_t pOperatorId[6], uint8_t * *pOperatorName)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CFW_CfgNwGetOperatorName) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_CFW_CfgNwGetOperatorName);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct IPAR_CFW_CfgNwGetOperatorName *ipar = (struct IPAR_CFW_CfgNwGetOperatorName *)((char*)mem + sizeof(rpcCallHeader_t));
    struct OPAR_CFW_CfgNwGetOperatorName *opar = (struct OPAR_CFW_CfgNwGetOperatorName *)((char*)mem + sizeof(rpcRespHeader_t));
    uint32_t result;

    call->h.size = call_size;
    call->api_tag = TAG_CFW_CfgNwGetOperatorName;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    memcpy(ipar->pOperatorId, pOperatorId, sizeof(ipar->pOperatorId));
    rpcSendCall(RPC_CHANNEL, call);

    *pOperatorName = opar->pOperatorName;
    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

uint32_t CFW_CfgSetNwOperDispFormat(uint8_t nFormat)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CFW_CfgSetNwOperDispFormat) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_CFW_CfgSetNwOperDispFormat);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct IPAR_CFW_CfgSetNwOperDispFormat *ipar = (struct IPAR_CFW_CfgSetNwOperDispFormat *)((char*)mem + sizeof(rpcCallHeader_t));
    struct OPAR_CFW_CfgSetNwOperDispFormat *opar = (struct OPAR_CFW_CfgSetNwOperDispFormat *)((char*)mem + sizeof(rpcRespHeader_t));
    uint32_t result;

    call->h.size = call_size;
    call->api_tag = TAG_CFW_CfgSetNwOperDispFormat;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    ipar->nFormat = nFormat;
    rpcSendCall(RPC_CHANNEL, call);

    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

uint32_t CFW_CfgGetNwOperDispFormat(uint8_t *nFormat)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + 0 + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_CFW_CfgGetNwOperDispFormat);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct OPAR_CFW_CfgGetNwOperDispFormat *opar = (struct OPAR_CFW_CfgGetNwOperDispFormat *)((char*)mem + sizeof(rpcRespHeader_t));
    uint32_t result;

    call->h.size = call_size;
    call->api_tag = TAG_CFW_CfgGetNwOperDispFormat;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    rpcSendCall(RPC_CHANNEL, call);

    *nFormat = opar->nFormat;
    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

uint32_t CFW_CfgSetSSN(uint8_t nCSSI, uint8_t nCSSU, CFW_SIM_ID nSimID)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CFW_CfgSetSSN) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_CFW_CfgSetSSN);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct IPAR_CFW_CfgSetSSN *ipar = (struct IPAR_CFW_CfgSetSSN *)((char*)mem + sizeof(rpcCallHeader_t));
    struct OPAR_CFW_CfgSetSSN *opar = (struct OPAR_CFW_CfgSetSSN *)((char*)mem + sizeof(rpcRespHeader_t));
    uint32_t result;

    call->h.size = call_size;
    call->api_tag = TAG_CFW_CfgSetSSN;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    ipar->nCSSI = nCSSI;
    ipar->nCSSU = nCSSU;
    ipar->nSimID = nSimID;
    rpcSendCall(RPC_CHANNEL, call);

    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

uint32_t CFW_CfgGetSSN(uint8_t *nCSSI, uint8_t *nCSSU, CFW_SIM_ID nSimID)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CFW_CfgGetSSN) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_CFW_CfgGetSSN);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct IPAR_CFW_CfgGetSSN *ipar = (struct IPAR_CFW_CfgGetSSN *)((char*)mem + sizeof(rpcCallHeader_t));
    struct OPAR_CFW_CfgGetSSN *opar = (struct OPAR_CFW_CfgGetSSN *)((char*)mem + sizeof(rpcRespHeader_t));
    uint32_t result;

    call->h.size = call_size;
    call->api_tag = TAG_CFW_CfgGetSSN;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    ipar->nSimID = nSimID;
    rpcSendCall(RPC_CHANNEL, call);

    *nCSSI = opar->nCSSI;
    *nCSSU = opar->nCSSU;
    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

uint32_t CFW_CfgSetClir(uint8_t nClir, CFW_SIM_ID nSimID)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CFW_CfgSetClir) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_CFW_CfgSetClir);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct IPAR_CFW_CfgSetClir *ipar = (struct IPAR_CFW_CfgSetClir *)((char*)mem + sizeof(rpcCallHeader_t));
    struct OPAR_CFW_CfgSetClir *opar = (struct OPAR_CFW_CfgSetClir *)((char*)mem + sizeof(rpcRespHeader_t));
    uint32_t result;

    call->h.size = call_size;
    call->api_tag = TAG_CFW_CfgSetClir;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    ipar->nClir = nClir;
    ipar->nSimID = nSimID;
    rpcSendCall(RPC_CHANNEL, call);

    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

uint32_t CFW_CfgGetClir(uint8_t *nClir, CFW_SIM_ID nSimID)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CFW_CfgGetClir) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_CFW_CfgGetClir);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct IPAR_CFW_CfgGetClir *ipar = (struct IPAR_CFW_CfgGetClir *)((char*)mem + sizeof(rpcCallHeader_t));
    struct OPAR_CFW_CfgGetClir *opar = (struct OPAR_CFW_CfgGetClir *)((char*)mem + sizeof(rpcRespHeader_t));
    uint32_t result;

    call->h.size = call_size;
    call->api_tag = TAG_CFW_CfgGetClir;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    ipar->nSimID = nSimID;
    rpcSendCall(RPC_CHANNEL, call);

    *nClir = opar->nClir;
    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

uint32_t CFW_CfgSetClip(uint8_t nEnable, CFW_SIM_ID nSimID)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CFW_CfgSetClip) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_CFW_CfgSetClip);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct IPAR_CFW_CfgSetClip *ipar = (struct IPAR_CFW_CfgSetClip *)((char*)mem + sizeof(rpcCallHeader_t));
    struct OPAR_CFW_CfgSetClip *opar = (struct OPAR_CFW_CfgSetClip *)((char*)mem + sizeof(rpcRespHeader_t));
    uint32_t result;

    call->h.size = call_size;
    call->api_tag = TAG_CFW_CfgSetClip;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    ipar->nEnable = nEnable;
    ipar->nSimID = nSimID;
    rpcSendCall(RPC_CHANNEL, call);

    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

uint32_t CFW_CfgGetClip(uint8_t *nEnable, CFW_SIM_ID nSimID)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CFW_CfgGetClip) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_CFW_CfgGetClip);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct IPAR_CFW_CfgGetClip *ipar = (struct IPAR_CFW_CfgGetClip *)((char*)mem + sizeof(rpcCallHeader_t));
    struct OPAR_CFW_CfgGetClip *opar = (struct OPAR_CFW_CfgGetClip *)((char*)mem + sizeof(rpcRespHeader_t));
    uint32_t result;

    call->h.size = call_size;
    call->api_tag = TAG_CFW_CfgGetClip;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    ipar->nSimID = nSimID;
    rpcSendCall(RPC_CHANNEL, call);

    *nEnable = opar->nEnable;
    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

uint32_t CFW_CfgSetColp(uint8_t n, CFW_SIM_ID nSimID)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CFW_CfgSetColp) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_CFW_CfgSetColp);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct IPAR_CFW_CfgSetColp *ipar = (struct IPAR_CFW_CfgSetColp *)((char*)mem + sizeof(rpcCallHeader_t));
    struct OPAR_CFW_CfgSetColp *opar = (struct OPAR_CFW_CfgSetColp *)((char*)mem + sizeof(rpcRespHeader_t));
    uint32_t result;

    call->h.size = call_size;
    call->api_tag = TAG_CFW_CfgSetColp;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    ipar->n = n;
    ipar->nSimID = nSimID;
    rpcSendCall(RPC_CHANNEL, call);

    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

uint32_t CFW_CfgGetColp(uint8_t *n, CFW_SIM_ID nSimID)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CFW_CfgGetColp) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_CFW_CfgGetColp);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct IPAR_CFW_CfgGetColp *ipar = (struct IPAR_CFW_CfgGetColp *)((char*)mem + sizeof(rpcCallHeader_t));
    struct OPAR_CFW_CfgGetColp *opar = (struct OPAR_CFW_CfgGetColp *)((char*)mem + sizeof(rpcRespHeader_t));
    uint32_t result;

    call->h.size = call_size;
    call->api_tag = TAG_CFW_CfgGetColp;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    ipar->nSimID = nSimID;
    rpcSendCall(RPC_CHANNEL, call);

    *n = opar->n;
    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

uint32_t CFW_CfgSetCallWaiting(uint8_t n, CFW_SIM_ID nSimID)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CFW_CfgSetCallWaiting) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_CFW_CfgSetCallWaiting);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct IPAR_CFW_CfgSetCallWaiting *ipar = (struct IPAR_CFW_CfgSetCallWaiting *)((char*)mem + sizeof(rpcCallHeader_t));
    struct OPAR_CFW_CfgSetCallWaiting *opar = (struct OPAR_CFW_CfgSetCallWaiting *)((char*)mem + sizeof(rpcRespHeader_t));
    uint32_t result;

    call->h.size = call_size;
    call->api_tag = TAG_CFW_CfgSetCallWaiting;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    ipar->n = n;
    ipar->nSimID = nSimID;
    rpcSendCall(RPC_CHANNEL, call);

    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

uint32_t CFW_CfgGetCallWaiting(uint8_t *n, CFW_SIM_ID nSimID)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CFW_CfgGetCallWaiting) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_CFW_CfgGetCallWaiting);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct IPAR_CFW_CfgGetCallWaiting *ipar = (struct IPAR_CFW_CfgGetCallWaiting *)((char*)mem + sizeof(rpcCallHeader_t));
    struct OPAR_CFW_CfgGetCallWaiting *opar = (struct OPAR_CFW_CfgGetCallWaiting *)((char*)mem + sizeof(rpcRespHeader_t));
    uint32_t result;

    call->h.size = call_size;
    call->api_tag = TAG_CFW_CfgGetCallWaiting;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    ipar->nSimID = nSimID;
    rpcSendCall(RPC_CHANNEL, call);

    *n = opar->n;
    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

uint32_t CFW_CfgGetIMSI(uint8_t pIMSI[15], CFW_SIM_ID nSimID)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CFW_CfgGetIMSI) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_CFW_CfgGetIMSI);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct IPAR_CFW_CfgGetIMSI *ipar = (struct IPAR_CFW_CfgGetIMSI *)((char*)mem + sizeof(rpcCallHeader_t));
    struct OPAR_CFW_CfgGetIMSI *opar = (struct OPAR_CFW_CfgGetIMSI *)((char*)mem + sizeof(rpcRespHeader_t));
    uint32_t result;

    call->h.size = call_size;
    call->api_tag = TAG_CFW_CfgGetIMSI;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    ipar->nSimID = nSimID;
    rpcSendCall(RPC_CHANNEL, call);

    memcpy(pIMSI, opar->pIMSI, sizeof(opar->pIMSI));
    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

uint32_t CFW_CfgSetIMSI(uint8_t pIMSI[15], CFW_SIM_ID nSimID)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CFW_CfgSetIMSI) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_CFW_CfgSetIMSI);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct IPAR_CFW_CfgSetIMSI *ipar = (struct IPAR_CFW_CfgSetIMSI *)((char*)mem + sizeof(rpcCallHeader_t));
    struct OPAR_CFW_CfgSetIMSI *opar = (struct OPAR_CFW_CfgSetIMSI *)((char*)mem + sizeof(rpcRespHeader_t));
    uint32_t result;

    call->h.size = call_size;
    call->api_tag = TAG_CFW_CfgSetIMSI;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    memcpy(ipar->pIMSI, pIMSI, sizeof(ipar->pIMSI));
    ipar->nSimID = nSimID;
    rpcSendCall(RPC_CHANNEL, call);

    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

#if defined(LTE_SUPPORT)
uint32_t CFW_NwSetEpsMode(uint8_t eps_mode, CFW_SIM_ID nSimID)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CFW_NwSetEpsMode) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_CFW_NwSetEpsMode);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct IPAR_CFW_NwSetEpsMode *ipar = (struct IPAR_CFW_NwSetEpsMode *)((char*)mem + sizeof(rpcCallHeader_t));
    struct OPAR_CFW_NwSetEpsMode *opar = (struct OPAR_CFW_NwSetEpsMode *)((char*)mem + sizeof(rpcRespHeader_t));
    uint32_t result;

    call->h.size = call_size;
    call->api_tag = TAG_CFW_NwSetEpsMode;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    ipar->eps_mode = eps_mode;
    ipar->nSimID = nSimID;
    rpcSendCall(RPC_CHANNEL, call);

    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

#endif

uint32_t CFW_CfgSetToneDuration(uint8_t nToneDuration)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CFW_CfgSetToneDuration) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_CFW_CfgSetToneDuration);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct IPAR_CFW_CfgSetToneDuration *ipar = (struct IPAR_CFW_CfgSetToneDuration *)((char*)mem + sizeof(rpcCallHeader_t));
    struct OPAR_CFW_CfgSetToneDuration *opar = (struct OPAR_CFW_CfgSetToneDuration *)((char*)mem + sizeof(rpcRespHeader_t));
    uint32_t result;

    call->h.size = call_size;
    call->api_tag = TAG_CFW_CfgSetToneDuration;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    ipar->nToneDuration = nToneDuration;
    rpcSendCall(RPC_CHANNEL, call);

    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

uint32_t CFW_CfgGetToneDuration(uint8_t *pToneDuration)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + 0 + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_CFW_CfgGetToneDuration);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct OPAR_CFW_CfgGetToneDuration *opar = (struct OPAR_CFW_CfgGetToneDuration *)((char*)mem + sizeof(rpcRespHeader_t));
    uint32_t result;

    call->h.size = call_size;
    call->api_tag = TAG_CFW_CfgGetToneDuration;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    rpcSendCall(RPC_CHANNEL, call);

    *pToneDuration = opar->pToneDuration;
    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

uint32_t CFW_CfgSetIncomingCallResultMode(uint8_t nMode)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CFW_CfgSetIncomingCallResultMode) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_CFW_CfgSetIncomingCallResultMode);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct IPAR_CFW_CfgSetIncomingCallResultMode *ipar = (struct IPAR_CFW_CfgSetIncomingCallResultMode *)((char*)mem + sizeof(rpcCallHeader_t));
    struct OPAR_CFW_CfgSetIncomingCallResultMode *opar = (struct OPAR_CFW_CfgSetIncomingCallResultMode *)((char*)mem + sizeof(rpcRespHeader_t));
    uint32_t result;

    call->h.size = call_size;
    call->api_tag = TAG_CFW_CfgSetIncomingCallResultMode;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    ipar->nMode = nMode;
    rpcSendCall(RPC_CHANNEL, call);

    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

uint32_t CFW_CfgGetIncomingCallResultMode(uint8_t *nMode)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + 0 + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_CFW_CfgGetIncomingCallResultMode);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct OPAR_CFW_CfgGetIncomingCallResultMode *opar = (struct OPAR_CFW_CfgGetIncomingCallResultMode *)((char*)mem + sizeof(rpcRespHeader_t));
    uint32_t result;

    call->h.size = call_size;
    call->api_tag = TAG_CFW_CfgGetIncomingCallResultMode;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    rpcSendCall(RPC_CHANNEL, call);

    *nMode = opar->nMode;
    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

uint32_t CFW_CfgSetAudioOutputParam(CFW_AUD_OUT_PARAMETER *pAudOutParam)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CFW_CfgSetAudioOutputParam) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_CFW_CfgSetAudioOutputParam);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct IPAR_CFW_CfgSetAudioOutputParam *ipar = (struct IPAR_CFW_CfgSetAudioOutputParam *)((char*)mem + sizeof(rpcCallHeader_t));
    struct OPAR_CFW_CfgSetAudioOutputParam *opar = (struct OPAR_CFW_CfgSetAudioOutputParam *)((char*)mem + sizeof(rpcRespHeader_t));
    uint32_t result;

    call->h.size = call_size;
    call->api_tag = TAG_CFW_CfgSetAudioOutputParam;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    ipar->pAudOutParam = *pAudOutParam;
    rpcSendCall(RPC_CHANNEL, call);

    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

uint32_t CFW_CfgGetAudioOutputParam(CFW_AUD_OUT_PARAMETER *pAudOutParam)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + 0 + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_CFW_CfgGetAudioOutputParam);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct OPAR_CFW_CfgGetAudioOutputParam *opar = (struct OPAR_CFW_CfgGetAudioOutputParam *)((char*)mem + sizeof(rpcRespHeader_t));
    uint32_t result;

    call->h.size = call_size;
    call->api_tag = TAG_CFW_CfgGetAudioOutputParam;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    rpcSendCall(RPC_CHANNEL, call);

    *pAudOutParam = opar->pAudOutParam;
    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

uint32_t CFW_CfgSetAudioAudioMode(uint8_t nAudioMode)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CFW_CfgSetAudioAudioMode) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_CFW_CfgSetAudioAudioMode);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct IPAR_CFW_CfgSetAudioAudioMode *ipar = (struct IPAR_CFW_CfgSetAudioAudioMode *)((char*)mem + sizeof(rpcCallHeader_t));
    struct OPAR_CFW_CfgSetAudioAudioMode *opar = (struct OPAR_CFW_CfgSetAudioAudioMode *)((char*)mem + sizeof(rpcRespHeader_t));
    uint32_t result;

    call->h.size = call_size;
    call->api_tag = TAG_CFW_CfgSetAudioAudioMode;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    ipar->nAudioMode = nAudioMode;
    rpcSendCall(RPC_CHANNEL, call);

    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

uint32_t CFW_CfgGetAudioAudioMode(uint8_t *pAudioMode)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + 0 + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_CFW_CfgGetAudioAudioMode);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct OPAR_CFW_CfgGetAudioAudioMode *opar = (struct OPAR_CFW_CfgGetAudioAudioMode *)((char*)mem + sizeof(rpcRespHeader_t));
    uint32_t result;

    call->h.size = call_size;
    call->api_tag = TAG_CFW_CfgGetAudioAudioMode;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    rpcSendCall(RPC_CHANNEL, call);

    *pAudioMode = opar->pAudioMode;
    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

uint32_t CFW_CfgSetLoudspeakerVolumeLevel(uint8_t nVolumeLevel)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CFW_CfgSetLoudspeakerVolumeLevel) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_CFW_CfgSetLoudspeakerVolumeLevel);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct IPAR_CFW_CfgSetLoudspeakerVolumeLevel *ipar = (struct IPAR_CFW_CfgSetLoudspeakerVolumeLevel *)((char*)mem + sizeof(rpcCallHeader_t));
    struct OPAR_CFW_CfgSetLoudspeakerVolumeLevel *opar = (struct OPAR_CFW_CfgSetLoudspeakerVolumeLevel *)((char*)mem + sizeof(rpcRespHeader_t));
    uint32_t result;

    call->h.size = call_size;
    call->api_tag = TAG_CFW_CfgSetLoudspeakerVolumeLevel;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    ipar->nVolumeLevel = nVolumeLevel;
    rpcSendCall(RPC_CHANNEL, call);

    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

uint32_t CFW_CfgGetLoudspeakerVolumeLevel(uint8_t *pVolumeLevel)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + 0 + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_CFW_CfgGetLoudspeakerVolumeLevel);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct OPAR_CFW_CfgGetLoudspeakerVolumeLevel *opar = (struct OPAR_CFW_CfgGetLoudspeakerVolumeLevel *)((char*)mem + sizeof(rpcRespHeader_t));
    uint32_t result;

    call->h.size = call_size;
    call->api_tag = TAG_CFW_CfgGetLoudspeakerVolumeLevel;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    rpcSendCall(RPC_CHANNEL, call);

    *pVolumeLevel = opar->pVolumeLevel;
    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

uint32_t CFW_CfgGetLoudspeakerVolumeLevelRange(uint8_t *pMinVolumeLevel, uint8_t *pMaxVolumeLevel)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + 0 + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_CFW_CfgGetLoudspeakerVolumeLevelRange);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct OPAR_CFW_CfgGetLoudspeakerVolumeLevelRange *opar = (struct OPAR_CFW_CfgGetLoudspeakerVolumeLevelRange *)((char*)mem + sizeof(rpcRespHeader_t));
    uint32_t result;

    call->h.size = call_size;
    call->api_tag = TAG_CFW_CfgGetLoudspeakerVolumeLevelRange;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    rpcSendCall(RPC_CHANNEL, call);

    *pMinVolumeLevel = opar->pMinVolumeLevel;
    *pMaxVolumeLevel = opar->pMaxVolumeLevel;
    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

#if defined(CFW_VOLTE_SUPPORT)
uint32_t CFW_ImsInit(CFW_SIM_ID nSimID)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CFW_ImsInit) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_CFW_ImsInit);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct IPAR_CFW_ImsInit *ipar = (struct IPAR_CFW_ImsInit *)((char*)mem + sizeof(rpcCallHeader_t));
    struct OPAR_CFW_ImsInit *opar = (struct OPAR_CFW_ImsInit *)((char*)mem + sizeof(rpcRespHeader_t));
    uint32_t result;

    call->h.size = call_size;
    call->api_tag = TAG_CFW_ImsInit;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    ipar->nSimID = nSimID;
    rpcSendCall(RPC_CHANNEL, call);

    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

#endif

#if defined(CFW_VOLTE_SUPPORT)
uint32_t CFW_ImsEnable(uint8_t isEmergency, uint16_t nUTI, CFW_SIM_ID nSimID)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CFW_ImsEnable) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_CFW_ImsEnable);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct IPAR_CFW_ImsEnable *ipar = (struct IPAR_CFW_ImsEnable *)((char*)mem + sizeof(rpcCallHeader_t));
    struct OPAR_CFW_ImsEnable *opar = (struct OPAR_CFW_ImsEnable *)((char*)mem + sizeof(rpcRespHeader_t));
    uint32_t result;

    call->h.size = call_size;
    call->api_tag = TAG_CFW_ImsEnable;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    ipar->isEmergency = isEmergency;
    ipar->nUTI = nUTI;
    ipar->nSimID = nSimID;
    rpcSendCall(RPC_CHANNEL, call);

    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

#endif

#if defined(CFW_VOLTE_SUPPORT)
uint32_t CFW_ImsDisable(uint8_t isEmergency, uint16_t nUTI, CFW_SIM_ID nSimID)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CFW_ImsDisable) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_CFW_ImsDisable);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct IPAR_CFW_ImsDisable *ipar = (struct IPAR_CFW_ImsDisable *)((char*)mem + sizeof(rpcCallHeader_t));
    struct OPAR_CFW_ImsDisable *opar = (struct OPAR_CFW_ImsDisable *)((char*)mem + sizeof(rpcRespHeader_t));
    uint32_t result;

    call->h.size = call_size;
    call->api_tag = TAG_CFW_ImsDisable;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    ipar->isEmergency = isEmergency;
    ipar->nUTI = nUTI;
    ipar->nSimID = nSimID;
    rpcSendCall(RPC_CHANNEL, call);

    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

#endif

#if defined(CFW_VOLTE_SUPPORT)
uint32_t CFW_ImsSuspend(CFW_SIM_ID nSimID, CFW_IMS_SUSPEND_TYPE nType)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CFW_ImsSuspend) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_CFW_ImsSuspend);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct IPAR_CFW_ImsSuspend *ipar = (struct IPAR_CFW_ImsSuspend *)((char*)mem + sizeof(rpcCallHeader_t));
    struct OPAR_CFW_ImsSuspend *opar = (struct OPAR_CFW_ImsSuspend *)((char*)mem + sizeof(rpcRespHeader_t));
    uint32_t result;

    call->h.size = call_size;
    call->api_tag = TAG_CFW_ImsSuspend;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    ipar->nSimID = nSimID;
    ipar->nType = nType;
    rpcSendCall(RPC_CHANNEL, call);

    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

#endif

#if defined(CFW_VOLTE_SUPPORT)
uint32_t CFW_ImsResume(CFW_SIM_ID nSimID)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CFW_ImsResume) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_CFW_ImsResume);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct IPAR_CFW_ImsResume *ipar = (struct IPAR_CFW_ImsResume *)((char*)mem + sizeof(rpcCallHeader_t));
    struct OPAR_CFW_ImsResume *opar = (struct OPAR_CFW_ImsResume *)((char*)mem + sizeof(rpcRespHeader_t));
    uint32_t result;

    call->h.size = call_size;
    call->api_tag = TAG_CFW_ImsResume;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    ipar->nSimID = nSimID;
    rpcSendCall(RPC_CHANNEL, call);

    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

#endif

#if defined(CFW_VOLTE_SUPPORT)
uint8_t CFW_GprsGetNsapi(uint8_t nCid, CFW_SIM_ID nSimID)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CFW_GprsGetNsapi) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_CFW_GprsGetNsapi);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct IPAR_CFW_GprsGetNsapi *ipar = (struct IPAR_CFW_GprsGetNsapi *)((char*)mem + sizeof(rpcCallHeader_t));
    struct OPAR_CFW_GprsGetNsapi *opar = (struct OPAR_CFW_GprsGetNsapi *)((char*)mem + sizeof(rpcRespHeader_t));
    uint8_t result;

    call->h.size = call_size;
    call->api_tag = TAG_CFW_GprsGetNsapi;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    ipar->nCid = nCid;
    ipar->nSimID = nSimID;
    rpcSendCall(RPC_CHANNEL, call);

    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

#endif

#if defined(CFW_VOLTE_SUPPORT)
uint32_t CFW_ImsSetDapsCallback(uint8_t nCid, CFW_SIM_ID nSimID)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CFW_ImsSetDapsCallback) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_CFW_ImsSetDapsCallback);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct IPAR_CFW_ImsSetDapsCallback *ipar = (struct IPAR_CFW_ImsSetDapsCallback *)((char*)mem + sizeof(rpcCallHeader_t));
    struct OPAR_CFW_ImsSetDapsCallback *opar = (struct OPAR_CFW_ImsSetDapsCallback *)((char*)mem + sizeof(rpcRespHeader_t));
    uint32_t result;

    call->h.size = call_size;
    call->api_tag = TAG_CFW_ImsSetDapsCallback;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    ipar->nCid = nCid;
    ipar->nSimID = nSimID;
    rpcSendCall(RPC_CHANNEL, call);

    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

#endif

#if defined(CFW_VOLTE_SUPPORT)
uint32_t CFW_ImsSetVolte(uint8_t bSet, uint16_t nUTI, CFW_SIM_ID nSimID)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CFW_ImsSetVolte) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_CFW_ImsSetVolte);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct IPAR_CFW_ImsSetVolte *ipar = (struct IPAR_CFW_ImsSetVolte *)((char*)mem + sizeof(rpcCallHeader_t));
    struct OPAR_CFW_ImsSetVolte *opar = (struct OPAR_CFW_ImsSetVolte *)((char*)mem + sizeof(rpcRespHeader_t));
    uint32_t result;

    call->h.size = call_size;
    call->api_tag = TAG_CFW_ImsSetVolte;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    ipar->bSet = bSet;
    ipar->nUTI = nUTI;
    ipar->nSimID = nSimID;
    rpcSendCall(RPC_CHANNEL, call);

    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

#endif

#if defined(CFW_VOLTE_SUPPORT)
bool CFW_ImsIsSet(CFW_SIM_ID nSimID)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CFW_ImsIsSet) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_CFW_ImsIsSet);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct IPAR_CFW_ImsIsSet *ipar = (struct IPAR_CFW_ImsIsSet *)((char*)mem + sizeof(rpcCallHeader_t));
    struct OPAR_CFW_ImsIsSet *opar = (struct OPAR_CFW_ImsIsSet *)((char*)mem + sizeof(rpcRespHeader_t));
    bool result;

    call->h.size = call_size;
    call->api_tag = TAG_CFW_ImsIsSet;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    ipar->nSimID = nSimID;
    rpcSendCall(RPC_CHANNEL, call);

    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

#endif

#if defined(CFW_VOLTE_SUPPORT)
uint8_t CFW_ImsFreeUti(void)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + 0 + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_CFW_ImsFreeUti);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct OPAR_CFW_ImsFreeUti *opar = (struct OPAR_CFW_ImsFreeUti *)((char*)mem + sizeof(rpcRespHeader_t));
    uint8_t result;

    call->h.size = call_size;
    call->api_tag = TAG_CFW_ImsFreeUti;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    rpcSendCall(RPC_CHANNEL, call);

    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

#endif

#if defined(CFW_VOLTE_SUPPORT)
uint32_t CFW_ImsActImsPdp(uint16_t nUTI, CFW_SIM_ID nSimID)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CFW_ImsActImsPdp) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_CFW_ImsActImsPdp);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct IPAR_CFW_ImsActImsPdp *ipar = (struct IPAR_CFW_ImsActImsPdp *)((char*)mem + sizeof(rpcCallHeader_t));
    struct OPAR_CFW_ImsActImsPdp *opar = (struct OPAR_CFW_ImsActImsPdp *)((char*)mem + sizeof(rpcRespHeader_t));
    uint32_t result;

    call->h.size = call_size;
    call->api_tag = TAG_CFW_ImsActImsPdp;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    ipar->nUTI = nUTI;
    ipar->nSimID = nSimID;
    rpcSendCall(RPC_CHANNEL, call);

    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

#endif

#if defined(CFW_VOLTE_SUPPORT)
uint32_t CFW_ImsDoSavedEmcDail(void)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + 0 + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_CFW_ImsDoSavedEmcDail);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct OPAR_CFW_ImsDoSavedEmcDail *opar = (struct OPAR_CFW_ImsDoSavedEmcDail *)((char*)mem + sizeof(rpcRespHeader_t));
    uint32_t result;

    call->h.size = call_size;
    call->api_tag = TAG_CFW_ImsDoSavedEmcDail;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    rpcSendCall(RPC_CHANNEL, call);

    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

#endif

#if defined(CFW_VOLTE_SUPPORT)
uint32_t CFW_ImsSetPdpStatus(uint8_t nStatus, uint8_t isEmergency, CFW_SIM_ID nSimID)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CFW_ImsSetPdpStatus) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_CFW_ImsSetPdpStatus);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct IPAR_CFW_ImsSetPdpStatus *ipar = (struct IPAR_CFW_ImsSetPdpStatus *)((char*)mem + sizeof(rpcCallHeader_t));
    struct OPAR_CFW_ImsSetPdpStatus *opar = (struct OPAR_CFW_ImsSetPdpStatus *)((char*)mem + sizeof(rpcRespHeader_t));
    uint32_t result;

    call->h.size = call_size;
    call->api_tag = TAG_CFW_ImsSetPdpStatus;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    ipar->nStatus = nStatus;
    ipar->isEmergency = isEmergency;
    ipar->nSimID = nSimID;
    rpcSendCall(RPC_CHANNEL, call);

    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

#endif

#if defined(CFW_VOLTE_SUPPORT)
uint8_t CFW_ImsGetPdpStatus(uint8_t isEmergency, CFW_SIM_ID nSimID)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CFW_ImsGetPdpStatus) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_CFW_ImsGetPdpStatus);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct IPAR_CFW_ImsGetPdpStatus *ipar = (struct IPAR_CFW_ImsGetPdpStatus *)((char*)mem + sizeof(rpcCallHeader_t));
    struct OPAR_CFW_ImsGetPdpStatus *opar = (struct OPAR_CFW_ImsGetPdpStatus *)((char*)mem + sizeof(rpcRespHeader_t));
    uint8_t result;

    call->h.size = call_size;
    call->api_tag = TAG_CFW_ImsGetPdpStatus;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    ipar->isEmergency = isEmergency;
    ipar->nSimID = nSimID;
    rpcSendCall(RPC_CHANNEL, call);

    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

#endif

#if defined(CFW_VOLTE_SUPPORT)
uint32_t CFW_ImsCheckStatusAfterCc(uint8_t nCsImsFlag, CFW_SIM_ID nSimID)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CFW_ImsCheckStatusAfterCc) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_CFW_ImsCheckStatusAfterCc);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct IPAR_CFW_ImsCheckStatusAfterCc *ipar = (struct IPAR_CFW_ImsCheckStatusAfterCc *)((char*)mem + sizeof(rpcCallHeader_t));
    struct OPAR_CFW_ImsCheckStatusAfterCc *opar = (struct OPAR_CFW_ImsCheckStatusAfterCc *)((char*)mem + sizeof(rpcRespHeader_t));
    uint32_t result;

    call->h.size = call_size;
    call->api_tag = TAG_CFW_ImsCheckStatusAfterCc;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    ipar->nCsImsFlag = nCsImsFlag;
    ipar->nSimID = nSimID;
    rpcSendCall(RPC_CHANNEL, call);

    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

#endif

#if defined(CFW_VOLTE_SUPPORT)
uint32_t CFW_SsSetClip(uint8_t set, uint16_t nUTI, CFW_SIM_ID nSimID)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CFW_SsSetClip) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_CFW_SsSetClip);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct IPAR_CFW_SsSetClip *ipar = (struct IPAR_CFW_SsSetClip *)((char*)mem + sizeof(rpcCallHeader_t));
    struct OPAR_CFW_SsSetClip *opar = (struct OPAR_CFW_SsSetClip *)((char*)mem + sizeof(rpcRespHeader_t));
    uint32_t result;

    call->h.size = call_size;
    call->api_tag = TAG_CFW_SsSetClip;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    ipar->set = set;
    ipar->nUTI = nUTI;
    ipar->nSimID = nSimID;
    rpcSendCall(RPC_CHANNEL, call);

    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

#endif

#if defined(CFW_VOLTE_SUPPORT)
uint32_t CFW_SsSetClir(uint8_t set, uint16_t nUTI, CFW_SIM_ID nSimID)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CFW_SsSetClir) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_CFW_SsSetClir);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct IPAR_CFW_SsSetClir *ipar = (struct IPAR_CFW_SsSetClir *)((char*)mem + sizeof(rpcCallHeader_t));
    struct OPAR_CFW_SsSetClir *opar = (struct OPAR_CFW_SsSetClir *)((char*)mem + sizeof(rpcRespHeader_t));
    uint32_t result;

    call->h.size = call_size;
    call->api_tag = TAG_CFW_SsSetClir;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    ipar->set = set;
    ipar->nUTI = nUTI;
    ipar->nSimID = nSimID;
    rpcSendCall(RPC_CHANNEL, call);

    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

#endif

#if defined(CFW_VOLTE_SUPPORT)
uint32_t CFW_SsSetColp(uint8_t set, uint16_t nUTI, CFW_SIM_ID nSimID)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CFW_SsSetColp) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_CFW_SsSetColp);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct IPAR_CFW_SsSetColp *ipar = (struct IPAR_CFW_SsSetColp *)((char*)mem + sizeof(rpcCallHeader_t));
    struct OPAR_CFW_SsSetColp *opar = (struct OPAR_CFW_SsSetColp *)((char*)mem + sizeof(rpcRespHeader_t));
    uint32_t result;

    call->h.size = call_size;
    call->api_tag = TAG_CFW_SsSetColp;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    ipar->set = set;
    ipar->nUTI = nUTI;
    ipar->nSimID = nSimID;
    rpcSendCall(RPC_CHANNEL, call);

    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

#endif

#if defined(CFW_VOLTE_SUPPORT)
bool CFW_ImsIsRegistered(uint8_t isEmergency, CFW_SIM_ID nSimID)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CFW_ImsIsRegistered) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_CFW_ImsIsRegistered);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct IPAR_CFW_ImsIsRegistered *ipar = (struct IPAR_CFW_ImsIsRegistered *)((char*)mem + sizeof(rpcCallHeader_t));
    struct OPAR_CFW_ImsIsRegistered *opar = (struct OPAR_CFW_ImsIsRegistered *)((char*)mem + sizeof(rpcRespHeader_t));
    bool result;

    call->h.size = call_size;
    call->api_tag = TAG_CFW_ImsIsRegistered;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    ipar->isEmergency = isEmergency;
    ipar->nSimID = nSimID;
    rpcSendCall(RPC_CHANNEL, call);

    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

#endif

uint32_t CFW_NWSetRat(uint8_t nRat, CFW_SIM_ID nSimID)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CFW_NWSetRat) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_CFW_NWSetRat);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct IPAR_CFW_NWSetRat *ipar = (struct IPAR_CFW_NWSetRat *)((char*)mem + sizeof(rpcCallHeader_t));
    struct OPAR_CFW_NWSetRat *opar = (struct OPAR_CFW_NWSetRat *)((char*)mem + sizeof(rpcRespHeader_t));
    uint32_t result;

    call->h.size = call_size;
    call->api_tag = TAG_CFW_NWSetRat;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    ipar->nRat = nRat;
    ipar->nSimID = nSimID;
    rpcSendCall(RPC_CHANNEL, call);

    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

uint8_t CFW_NWGetRat(CFW_SIM_ID nSimID)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CFW_NWGetRat) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_CFW_NWGetRat);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct IPAR_CFW_NWGetRat *ipar = (struct IPAR_CFW_NWGetRat *)((char*)mem + sizeof(rpcCallHeader_t));
    struct OPAR_CFW_NWGetRat *opar = (struct OPAR_CFW_NWGetRat *)((char*)mem + sizeof(rpcRespHeader_t));
    uint8_t result;

    call->h.size = call_size;
    call->api_tag = TAG_CFW_NWGetRat;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    ipar->nSimID = nSimID;
    rpcSendCall(RPC_CHANNEL, call);

    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

uint32_t CFW_NWSetStackRat(uint8_t nRat, CFW_SIM_ID nSimID)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CFW_NWSetStackRat) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_CFW_NWSetStackRat);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct IPAR_CFW_NWSetStackRat *ipar = (struct IPAR_CFW_NWSetStackRat *)((char*)mem + sizeof(rpcCallHeader_t));
    struct OPAR_CFW_NWSetStackRat *opar = (struct OPAR_CFW_NWSetStackRat *)((char*)mem + sizeof(rpcRespHeader_t));
    uint32_t result;

    call->h.size = call_size;
    call->api_tag = TAG_CFW_NWSetStackRat;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    ipar->nRat = nRat;
    ipar->nSimID = nSimID;
    rpcSendCall(RPC_CHANNEL, call);

    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

uint8_t CFW_NWGetStackRat(CFW_SIM_ID nSimID)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CFW_NWGetStackRat) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_CFW_NWGetStackRat);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct IPAR_CFW_NWGetStackRat *ipar = (struct IPAR_CFW_NWGetStackRat *)((char*)mem + sizeof(rpcCallHeader_t));
    struct OPAR_CFW_NWGetStackRat *opar = (struct OPAR_CFW_NWGetStackRat *)((char*)mem + sizeof(rpcRespHeader_t));
    uint8_t result;

    call->h.size = call_size;
    call->api_tag = TAG_CFW_NWGetStackRat;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    ipar->nSimID = nSimID;
    rpcSendCall(RPC_CHANNEL, call);

    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}
uint32_t CFW_CfgGetCdmaImsi(uint8_t pCdmaImsi[10], CFW_SIM_ID nSimID)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CFW_CfgGetCdmaImsi) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_CFW_CfgGetCdmaImsi);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem;
    rpcCallHeader_t *call;
    struct IPAR_CFW_CfgGetCdmaImsi *ipar;
    struct OPAR_CFW_CfgGetCdmaImsi *opar;
    uint32_t result;
    int rpc_result;

    mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    call = (rpcCallHeader_t *)mem;
    ipar = (struct IPAR_CFW_CfgGetCdmaImsi *)((char *)mem + sizeof(rpcCallHeader_t));
    opar = (struct OPAR_CFW_CfgGetCdmaImsi *)((char *)mem + sizeof(rpcRespHeader_t));

    call->h.size = call_size;
    call->api_tag = TAG_CFW_CfgGetCdmaImsi;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    ipar->nSimID = nSimID;
    rpc_result = rpcSendCall(RPC_CHANNEL, call);
    (void)rpc_result;

    memcpy(pCdmaImsi, opar->pCdmaImsi, sizeof(opar->pCdmaImsi));
    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}


uint32_t CFW_NwSetNetMode(uint8_t nNetMode, CFW_SIM_ID nSimID)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CFW_NwSetNetMode) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_CFW_NwSetNetMode);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct IPAR_CFW_NwSetNetMode *ipar = (struct IPAR_CFW_NwSetNetMode *)((char*)mem + sizeof(rpcCallHeader_t));
    struct OPAR_CFW_NwSetNetMode *opar = (struct OPAR_CFW_NwSetNetMode *)((char*)mem + sizeof(rpcRespHeader_t));
    uint32_t result;

    call->h.size = call_size;
    call->api_tag = TAG_CFW_NwSetNetMode;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    ipar->nNetMode = nNetMode;
    ipar->nSimID = nSimID;
    rpcSendCall(RPC_CHANNEL, call);

    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

uint32_t CFW_NwGetAvailableOperators(uint16_t nUTI, CFW_SIM_ID nSimID)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CFW_NwGetAvailableOperators) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_CFW_NwGetAvailableOperators);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct IPAR_CFW_NwGetAvailableOperators *ipar = (struct IPAR_CFW_NwGetAvailableOperators *)((char*)mem + sizeof(rpcCallHeader_t));
    struct OPAR_CFW_NwGetAvailableOperators *opar = (struct OPAR_CFW_NwGetAvailableOperators *)((char*)mem + sizeof(rpcRespHeader_t));
    uint32_t result;

    call->h.size = call_size;
    call->api_tag = TAG_CFW_NwGetAvailableOperators;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    ipar->nUTI = nUTI;
    ipar->nSimID = nSimID;
    rpcSendCall(RPC_CHANNEL, call);

    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

uint32_t CFW_NwGetCurrentOperator(uint8_t OperatorId[6], uint8_t *pMode, CFW_SIM_ID nSimID)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CFW_NwGetCurrentOperator) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_CFW_NwGetCurrentOperator);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct IPAR_CFW_NwGetCurrentOperator *ipar = (struct IPAR_CFW_NwGetCurrentOperator *)((char*)mem + sizeof(rpcCallHeader_t));
    struct OPAR_CFW_NwGetCurrentOperator *opar = (struct OPAR_CFW_NwGetCurrentOperator *)((char*)mem + sizeof(rpcRespHeader_t));
    uint32_t result;

    call->h.size = call_size;
    call->api_tag = TAG_CFW_NwGetCurrentOperator;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    ipar->nSimID = nSimID;
    rpcSendCall(RPC_CHANNEL, call);

    memcpy(OperatorId, opar->OperatorId, sizeof(opar->OperatorId));
    *pMode = opar->pMode;
    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

uint32_t CFW_NwGetImei(uint16_t nUTI, CFW_SIM_ID nSimID)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CFW_NwGetImei) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_CFW_NwGetImei);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct IPAR_CFW_NwGetImei *ipar = (struct IPAR_CFW_NwGetImei *)((char*)mem + sizeof(rpcCallHeader_t));
    struct OPAR_CFW_NwGetImei *opar = (struct OPAR_CFW_NwGetImei *)((char*)mem + sizeof(rpcRespHeader_t));
    uint32_t result;

    call->h.size = call_size;
    call->api_tag = TAG_CFW_NwGetImei;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    ipar->nUTI = nUTI;
    ipar->nSimID = nSimID;
    rpcSendCall(RPC_CHANNEL, call);

    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

uint32_t CFW_NwGetSignalQuality(uint8_t *pSignalLevel, uint8_t *pBitError, CFW_SIM_ID nSimID)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CFW_NwGetSignalQuality) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_CFW_NwGetSignalQuality);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct IPAR_CFW_NwGetSignalQuality *ipar = (struct IPAR_CFW_NwGetSignalQuality *)((char*)mem + sizeof(rpcCallHeader_t));
    struct OPAR_CFW_NwGetSignalQuality *opar = (struct OPAR_CFW_NwGetSignalQuality *)((char*)mem + sizeof(rpcRespHeader_t));
    uint32_t result;

    call->h.size = call_size;
    call->api_tag = TAG_CFW_NwGetSignalQuality;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    ipar->nSimID = nSimID;
    rpcSendCall(RPC_CHANNEL, call);

    *pSignalLevel = opar->pSignalLevel;
    *pBitError = opar->pBitError;
    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

uint32_t CFW_NwGetStatus(CFW_NW_STATUS_INFO *pStatusInfo, CFW_SIM_ID nSimID)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CFW_NwGetStatus) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_CFW_NwGetStatus);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct IPAR_CFW_NwGetStatus *ipar = (struct IPAR_CFW_NwGetStatus *)((char*)mem + sizeof(rpcCallHeader_t));
    struct OPAR_CFW_NwGetStatus *opar = (struct OPAR_CFW_NwGetStatus *)((char*)mem + sizeof(rpcRespHeader_t));
    uint32_t result;

    call->h.size = call_size;
    call->api_tag = TAG_CFW_NwGetStatus;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    ipar->nSimID = nSimID;
    rpcSendCall(RPC_CHANNEL, call);

    *pStatusInfo = opar->pStatusInfo;
    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

uint32_t CFW_NwSetRegistration(uint8_t nOperatorId[6], uint8_t nMode, uint8_t nRat, uint16_t nUTI, CFW_SIM_ID nSimID)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CFW_NwSetRegistration) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_CFW_NwSetRegistration);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct IPAR_CFW_NwSetRegistration *ipar = (struct IPAR_CFW_NwSetRegistration *)((char*)mem + sizeof(rpcCallHeader_t));
    struct OPAR_CFW_NwSetRegistration *opar = (struct OPAR_CFW_NwSetRegistration *)((char*)mem + sizeof(rpcRespHeader_t));
    uint32_t result;

    call->h.size = call_size;
    call->api_tag = TAG_CFW_NwSetRegistration;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    memcpy(ipar->nOperatorId, nOperatorId, sizeof(ipar->nOperatorId));
    ipar->nMode = nMode;
    ipar->nRat = nRat;
    ipar->nUTI = nUTI;
    ipar->nSimID = nSimID;
    rpcSendCall(RPC_CHANNEL, call);

    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

uint32_t CFW_NwDeRegister(uint16_t nUTI, CFW_SIM_ID nSimID)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CFW_NwDeRegister) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_CFW_NwDeRegister);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct IPAR_CFW_NwDeRegister *ipar = (struct IPAR_CFW_NwDeRegister *)((char*)mem + sizeof(rpcCallHeader_t));
    struct OPAR_CFW_NwDeRegister *opar = (struct OPAR_CFW_NwDeRegister *)((char*)mem + sizeof(rpcRespHeader_t));
    uint32_t result;

    call->h.size = call_size;
    call->api_tag = TAG_CFW_NwDeRegister;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    ipar->nUTI = nUTI;
    ipar->nSimID = nSimID;
    rpcSendCall(RPC_CHANNEL, call);

    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

uint32_t CFW_SetComm(CFW_COMM_MODE nMode, uint8_t nStorageFlag, uint16_t nUTI, CFW_SIM_ID nSimID)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CFW_SetComm) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_CFW_SetComm);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct IPAR_CFW_SetComm *ipar = (struct IPAR_CFW_SetComm *)((char*)mem + sizeof(rpcCallHeader_t));
    struct OPAR_CFW_SetComm *opar = (struct OPAR_CFW_SetComm *)((char*)mem + sizeof(rpcRespHeader_t));
    uint32_t result;

    call->h.size = call_size;
    call->api_tag = TAG_CFW_SetComm;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    ipar->nMode = nMode;
    ipar->nStorageFlag = nStorageFlag;
    ipar->nUTI = nUTI;
    ipar->nSimID = nSimID;
    rpcSendCall(RPC_CHANNEL, call);

    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

uint32_t CFW_GetComm(CFW_COMM_MODE *nMode, CFW_SIM_ID nSimID)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CFW_GetComm) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_CFW_GetComm);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct IPAR_CFW_GetComm *ipar = (struct IPAR_CFW_GetComm *)((char*)mem + sizeof(rpcCallHeader_t));
    struct OPAR_CFW_GetComm *opar = (struct OPAR_CFW_GetComm *)((char*)mem + sizeof(rpcRespHeader_t));
    uint32_t result;

    call->h.size = call_size;
    call->api_tag = TAG_CFW_GetComm;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    ipar->nSimID = nSimID;
    rpcSendCall(RPC_CHANNEL, call);

    *nMode = opar->nMode;
    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

uint32_t CFW_NwSetFrequencyBand(uint8_t nBand, uint16_t nUTI, CFW_SIM_ID nSimID)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CFW_NwSetFrequencyBand) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_CFW_NwSetFrequencyBand);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct IPAR_CFW_NwSetFrequencyBand *ipar = (struct IPAR_CFW_NwSetFrequencyBand *)((char*)mem + sizeof(rpcCallHeader_t));
    struct OPAR_CFW_NwSetFrequencyBand *opar = (struct OPAR_CFW_NwSetFrequencyBand *)((char*)mem + sizeof(rpcRespHeader_t));
    uint32_t result;

    call->h.size = call_size;
    call->api_tag = TAG_CFW_NwSetFrequencyBand;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    ipar->nBand = nBand;
    ipar->nUTI = nUTI;
    ipar->nSimID = nSimID;
    rpcSendCall(RPC_CHANNEL, call);

    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

uint32_t CFW_NwAbortListOperators(uint16_t nUTI, CFW_SIM_ID nSimID)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CFW_NwAbortListOperators) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_CFW_NwAbortListOperators);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct IPAR_CFW_NwAbortListOperators *ipar = (struct IPAR_CFW_NwAbortListOperators *)((char*)mem + sizeof(rpcCallHeader_t));
    struct OPAR_CFW_NwAbortListOperators *opar = (struct OPAR_CFW_NwAbortListOperators *)((char*)mem + sizeof(rpcRespHeader_t));
    uint32_t result;

    call->h.size = call_size;
    call->api_tag = TAG_CFW_NwAbortListOperators;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    ipar->nUTI = nUTI;
    ipar->nSimID = nSimID;
    rpcSendCall(RPC_CHANNEL, call);

    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

bool CFW_GetNWTimerOutFlag(CFW_SIM_ID nSimID)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CFW_GetNWTimerOutFlag) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_CFW_GetNWTimerOutFlag);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct IPAR_CFW_GetNWTimerOutFlag *ipar = (struct IPAR_CFW_GetNWTimerOutFlag *)((char*)mem + sizeof(rpcCallHeader_t));
    struct OPAR_CFW_GetNWTimerOutFlag *opar = (struct OPAR_CFW_GetNWTimerOutFlag *)((char*)mem + sizeof(rpcRespHeader_t));
    bool result;

    call->h.size = call_size;
    call->api_tag = TAG_CFW_GetNWTimerOutFlag;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    ipar->nSimID = nSimID;
    rpcSendCall(RPC_CHANNEL, call);

    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

void CFW_SetNWTimerOutFlag(bool bClear, CFW_SIM_ID nSimID)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CFW_SetNWTimerOutFlag) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + 0;
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct IPAR_CFW_SetNWTimerOutFlag *ipar = (struct IPAR_CFW_SetNWTimerOutFlag *)((char*)mem + sizeof(rpcCallHeader_t));

    call->h.size = call_size;
    call->api_tag = TAG_CFW_SetNWTimerOutFlag;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    ipar->bClear = bClear;
    ipar->nSimID = nSimID;
    rpcSendCall(RPC_CHANNEL, call);

    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
}

uint32_t CFW_CcAcceptSpeechCall(CFW_SIM_ID nSimID)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CFW_CcAcceptSpeechCall) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_CFW_CcAcceptSpeechCall);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct IPAR_CFW_CcAcceptSpeechCall *ipar = (struct IPAR_CFW_CcAcceptSpeechCall *)((char*)mem + sizeof(rpcCallHeader_t));
    struct OPAR_CFW_CcAcceptSpeechCall *opar = (struct OPAR_CFW_CcAcceptSpeechCall *)((char*)mem + sizeof(rpcRespHeader_t));
    uint32_t result;

    call->h.size = call_size;
    call->api_tag = TAG_CFW_CcAcceptSpeechCall;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    ipar->nSimID = nSimID;
    rpcSendCall(RPC_CHANNEL, call);

    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

uint32_t CFW_CcGetCallStatus(CFW_SIM_ID nSimID)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CFW_CcGetCallStatus) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_CFW_CcGetCallStatus);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct IPAR_CFW_CcGetCallStatus *ipar = (struct IPAR_CFW_CcGetCallStatus *)((char*)mem + sizeof(rpcCallHeader_t));
    struct OPAR_CFW_CcGetCallStatus *opar = (struct OPAR_CFW_CcGetCallStatus *)((char*)mem + sizeof(rpcRespHeader_t));
    uint32_t result;

    call->h.size = call_size;
    call->api_tag = TAG_CFW_CcGetCallStatus;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    ipar->nSimID = nSimID;
    rpcSendCall(RPC_CHANNEL, call);

    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

uint8_t CFW_GetCcCount(CFW_SIM_ID nSimID)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CFW_GetCcCount) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_CFW_GetCcCount);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct IPAR_CFW_GetCcCount *ipar = (struct IPAR_CFW_GetCcCount *)((char*)mem + sizeof(rpcCallHeader_t));
    struct OPAR_CFW_GetCcCount *opar = (struct OPAR_CFW_GetCcCount *)((char*)mem + sizeof(rpcRespHeader_t));
    uint8_t result;

    call->h.size = call_size;
    call->api_tag = TAG_CFW_GetCcCount;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    ipar->nSimID = nSimID;
    rpcSendCall(RPC_CHANNEL, call);

    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

uint32_t CFW_CcGetCurrentCall(CFW_CC_CURRENT_CALL_INFO CallInfo[7], uint8_t *pCnt, CFW_SIM_ID nSimID)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CFW_CcGetCurrentCall) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_CFW_CcGetCurrentCall);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct IPAR_CFW_CcGetCurrentCall *ipar = (struct IPAR_CFW_CcGetCurrentCall *)((char*)mem + sizeof(rpcCallHeader_t));
    struct OPAR_CFW_CcGetCurrentCall *opar = (struct OPAR_CFW_CcGetCurrentCall *)((char*)mem + sizeof(rpcRespHeader_t));
    uint32_t result;

    call->h.size = call_size;
    call->api_tag = TAG_CFW_CcGetCurrentCall;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    ipar->nSimID = nSimID;
    rpcSendCall(RPC_CHANNEL, call);

    memcpy(CallInfo, opar->CallInfo, sizeof(opar->CallInfo));
    *pCnt = opar->pCnt;
    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

uint32_t CFW_CcCallHoldMultiparty(uint8_t nCmd, uint8_t nIndex, CFW_SIM_ID nSimID)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CFW_CcCallHoldMultiparty) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_CFW_CcCallHoldMultiparty);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct IPAR_CFW_CcCallHoldMultiparty *ipar = (struct IPAR_CFW_CcCallHoldMultiparty *)((char*)mem + sizeof(rpcCallHeader_t));
    struct OPAR_CFW_CcCallHoldMultiparty *opar = (struct OPAR_CFW_CcCallHoldMultiparty *)((char*)mem + sizeof(rpcRespHeader_t));
    uint32_t result;

    call->h.size = call_size;
    call->api_tag = TAG_CFW_CcCallHoldMultiparty;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    ipar->nCmd = nCmd;
    ipar->nIndex = nIndex;
    ipar->nSimID = nSimID;
    rpcSendCall(RPC_CHANNEL, call);

    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

#if defined(LTE_SUPPORT)
uint32_t CFW_CcReleaseCallX(uint8_t nIndex, CFW_SIM_ID nSimID)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CFW_CcReleaseCallX) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_CFW_CcReleaseCallX);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct IPAR_CFW_CcReleaseCallX *ipar = (struct IPAR_CFW_CcReleaseCallX *)((char*)mem + sizeof(rpcCallHeader_t));
    struct OPAR_CFW_CcReleaseCallX *opar = (struct OPAR_CFW_CcReleaseCallX *)((char*)mem + sizeof(rpcRespHeader_t));
    uint32_t result;

    call->h.size = call_size;
    call->api_tag = TAG_CFW_CcReleaseCallX;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    ipar->nIndex = nIndex;
    ipar->nSimID = nSimID;
    rpcSendCall(RPC_CHANNEL, call);

    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

#endif

uint32_t CFW_CcInitiateSpeechCall_V2(CFW_DIALNUMBER_V2 *pDialNumber, uint16_t nUTI, CFW_SIM_ID nSimID)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CFW_CcInitiateSpeechCall_V2) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_CFW_CcInitiateSpeechCall_V2);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct IPAR_CFW_CcInitiateSpeechCall_V2 *ipar = (struct IPAR_CFW_CcInitiateSpeechCall_V2 *)((char*)mem + sizeof(rpcCallHeader_t));
    struct OPAR_CFW_CcInitiateSpeechCall_V2 *opar = (struct OPAR_CFW_CcInitiateSpeechCall_V2 *)((char*)mem + sizeof(rpcRespHeader_t));
    uint32_t result;

    call->h.size = call_size;
    call->api_tag = TAG_CFW_CcInitiateSpeechCall_V2;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    ipar->pDialNumber = *pDialNumber;
    ipar->nUTI = nUTI;
    ipar->nSimID = nSimID;
    rpcSendCall(RPC_CHANNEL, call);

    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

uint32_t CFW_CcInitiateSpeechCallEx_V2(CFW_DIALNUMBER_V2 *pDialNumber, uint8_t *pIndex, uint16_t nUTI, CFW_SIM_ID nSimID)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CFW_CcInitiateSpeechCallEx_V2) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_CFW_CcInitiateSpeechCallEx_V2);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct IPAR_CFW_CcInitiateSpeechCallEx_V2 *ipar = (struct IPAR_CFW_CcInitiateSpeechCallEx_V2 *)((char*)mem + sizeof(rpcCallHeader_t));
    struct OPAR_CFW_CcInitiateSpeechCallEx_V2 *opar = (struct OPAR_CFW_CcInitiateSpeechCallEx_V2 *)((char*)mem + sizeof(rpcRespHeader_t));
    uint32_t result;

    call->h.size = call_size;
    call->api_tag = TAG_CFW_CcInitiateSpeechCallEx_V2;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    ipar->pDialNumber = *pDialNumber;
    ipar->nUTI = nUTI;
    ipar->nSimID = nSimID;
    rpcSendCall(RPC_CHANNEL, call);

    *pIndex = opar->pIndex;
    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

uint32_t CFW_CcEmcDial(uint8_t * pNumber, uint8_t nSize, CFW_SIM_ID nSimID)
{
    size_t ivar_size = ALIGNUP8(nSize+1);
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CFW_CcEmcDial) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_CFW_CcEmcDial);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct IPAR_CFW_CcEmcDial *ipar = (struct IPAR_CFW_CcEmcDial *)((char*)mem + sizeof(rpcCallHeader_t));
    struct OPAR_CFW_CcEmcDial *opar = (struct OPAR_CFW_CcEmcDial *)((char*)mem + sizeof(rpcRespHeader_t));
    char *ivar_ptr = (char*)mem + sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CFW_CcEmcDial);
    uint32_t result;

    call->h.size = call_size;
    call->api_tag = TAG_CFW_CcEmcDial;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    memcpy(ivar_ptr, pNumber, nSize);
    ivar_ptr[nSize] = (char)0;
    ivar_ptr += ALIGNUP8(nSize + 1);
    ipar->nSize = nSize;
    ipar->nSimID = nSimID;
    rpcSendCall(RPC_CHANNEL, call);

    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

uint32_t CFW_CcEmcDialEx(uint8_t * pDialNumber, uint8_t nDialNumberSize, uint8_t *pIndex, CFW_SIM_ID nSimID)
{
    size_t ivar_size = ALIGNUP8(nDialNumberSize+1);
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CFW_CcEmcDialEx) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_CFW_CcEmcDialEx);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct IPAR_CFW_CcEmcDialEx *ipar = (struct IPAR_CFW_CcEmcDialEx *)((char*)mem + sizeof(rpcCallHeader_t));
    struct OPAR_CFW_CcEmcDialEx *opar = (struct OPAR_CFW_CcEmcDialEx *)((char*)mem + sizeof(rpcRespHeader_t));
    char *ivar_ptr = (char*)mem + sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CFW_CcEmcDialEx);
    uint32_t result;

    call->h.size = call_size;
    call->api_tag = TAG_CFW_CcEmcDialEx;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    memcpy(ivar_ptr, pDialNumber, nDialNumberSize);
    ivar_ptr[nDialNumberSize] = (char)0;
    ivar_ptr += ALIGNUP8(nDialNumberSize + 1);
    ipar->nDialNumberSize = nDialNumberSize;
    ipar->nSimID = nSimID;
    rpcSendCall(RPC_CHANNEL, call);

    *pIndex = opar->pIndex;
    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

uint32_t CFW_CcReleaseCall(CFW_SIM_ID nSimID)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CFW_CcReleaseCall) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_CFW_CcReleaseCall);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct IPAR_CFW_CcReleaseCall *ipar = (struct IPAR_CFW_CcReleaseCall *)((char*)mem + sizeof(rpcCallHeader_t));
    struct OPAR_CFW_CcReleaseCall *opar = (struct OPAR_CFW_CcReleaseCall *)((char*)mem + sizeof(rpcRespHeader_t));
    uint32_t result;

    call->h.size = call_size;
    call->api_tag = TAG_CFW_CcReleaseCall;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    ipar->nSimID = nSimID;
    rpcSendCall(RPC_CHANNEL, call);

    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

uint32_t CFW_CcRejectCall(CFW_SIM_ID nSimID)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CFW_CcRejectCall) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_CFW_CcRejectCall);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct IPAR_CFW_CcRejectCall *ipar = (struct IPAR_CFW_CcRejectCall *)((char*)mem + sizeof(rpcCallHeader_t));
    struct OPAR_CFW_CcRejectCall *opar = (struct OPAR_CFW_CcRejectCall *)((char*)mem + sizeof(rpcRespHeader_t));
    uint32_t result;

    call->h.size = call_size;
    call->api_tag = TAG_CFW_CcRejectCall;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    ipar->nSimID = nSimID;
    rpcSendCall(RPC_CHANNEL, call);

    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

uint32_t CFW_CcPlayTone(int8_t iTone, CFW_SIM_ID nSimID)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CFW_CcPlayTone) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_CFW_CcPlayTone);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct IPAR_CFW_CcPlayTone *ipar = (struct IPAR_CFW_CcPlayTone *)((char*)mem + sizeof(rpcCallHeader_t));
    struct OPAR_CFW_CcPlayTone *opar = (struct OPAR_CFW_CcPlayTone *)((char*)mem + sizeof(rpcRespHeader_t));
    uint32_t result;

    call->h.size = call_size;
    call->api_tag = TAG_CFW_CcPlayTone;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    ipar->iTone = iTone;
    ipar->nSimID = nSimID;
    rpcSendCall(RPC_CHANNEL, call);

    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

uint32_t CFW_CcStopTone(CFW_SIM_ID nSimID)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CFW_CcStopTone) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_CFW_CcStopTone);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct IPAR_CFW_CcStopTone *ipar = (struct IPAR_CFW_CcStopTone *)((char*)mem + sizeof(rpcCallHeader_t));
    struct OPAR_CFW_CcStopTone *opar = (struct OPAR_CFW_CcStopTone *)((char*)mem + sizeof(rpcRespHeader_t));
    uint32_t result;

    call->h.size = call_size;
    call->api_tag = TAG_CFW_CcStopTone;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    ipar->nSimID = nSimID;
    rpcSendCall(RPC_CHANNEL, call);

    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

uint32_t CFW_SsQueryCallWaiting(uint8_t nClass, uint16_t nUTI, CFW_SIM_ID nSimID)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CFW_SsQueryCallWaiting) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_CFW_SsQueryCallWaiting);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct IPAR_CFW_SsQueryCallWaiting *ipar = (struct IPAR_CFW_SsQueryCallWaiting *)((char*)mem + sizeof(rpcCallHeader_t));
    struct OPAR_CFW_SsQueryCallWaiting *opar = (struct OPAR_CFW_SsQueryCallWaiting *)((char*)mem + sizeof(rpcRespHeader_t));
    uint32_t result;

    call->h.size = call_size;
    call->api_tag = TAG_CFW_SsQueryCallWaiting;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    ipar->nClass = nClass;
    ipar->nUTI = nUTI;
    ipar->nSimID = nSimID;
    rpcSendCall(RPC_CHANNEL, call);

    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

uint32_t CFW_SsSetCallWaiting(uint8_t nMode, uint8_t nClass, uint16_t nUTI, CFW_SIM_ID nSimID)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CFW_SsSetCallWaiting) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_CFW_SsSetCallWaiting);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct IPAR_CFW_SsSetCallWaiting *ipar = (struct IPAR_CFW_SsSetCallWaiting *)((char*)mem + sizeof(rpcCallHeader_t));
    struct OPAR_CFW_SsSetCallWaiting *opar = (struct OPAR_CFW_SsSetCallWaiting *)((char*)mem + sizeof(rpcRespHeader_t));
    uint32_t result;

    call->h.size = call_size;
    call->api_tag = TAG_CFW_SsSetCallWaiting;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    ipar->nMode = nMode;
    ipar->nClass = nClass;
    ipar->nUTI = nUTI;
    ipar->nSimID = nSimID;
    rpcSendCall(RPC_CHANNEL, call);

    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

uint32_t CFW_SsSetCallForwarding(CFW_SS_SET_CALLFORWARDING_INFO *pCallForwarding, uint16_t nUTI, CFW_SIM_ID nSimID)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CFW_SsSetCallForwarding) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_CFW_SsSetCallForwarding);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct IPAR_CFW_SsSetCallForwarding *ipar = (struct IPAR_CFW_SsSetCallForwarding *)((char*)mem + sizeof(rpcCallHeader_t));
    struct OPAR_CFW_SsSetCallForwarding *opar = (struct OPAR_CFW_SsSetCallForwarding *)((char*)mem + sizeof(rpcRespHeader_t));
    uint32_t result;

    call->h.size = call_size;
    call->api_tag = TAG_CFW_SsSetCallForwarding;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    ipar->pCallForwarding = *pCallForwarding;
    ipar->nUTI = nUTI;
    ipar->nSimID = nSimID;
    rpcSendCall(RPC_CHANNEL, call);

    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

uint32_t CFW_SsQueryCallForwarding(uint8_t nReason, uint8_t nClass, uint16_t nUTI, CFW_SIM_ID nSimID)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CFW_SsQueryCallForwarding) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_CFW_SsQueryCallForwarding);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct IPAR_CFW_SsQueryCallForwarding *ipar = (struct IPAR_CFW_SsQueryCallForwarding *)((char*)mem + sizeof(rpcCallHeader_t));
    struct OPAR_CFW_SsQueryCallForwarding *opar = (struct OPAR_CFW_SsQueryCallForwarding *)((char*)mem + sizeof(rpcRespHeader_t));
    uint32_t result;

    call->h.size = call_size;
    call->api_tag = TAG_CFW_SsQueryCallForwarding;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    ipar->nReason = nReason;
    ipar->nClass = nClass;
    ipar->nUTI = nUTI;
    ipar->nSimID = nSimID;
    rpcSendCall(RPC_CHANNEL, call);

    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

uint32_t CFW_SsSetFacilityLock(uint16_t nFac, uint8_t * nBufPwd, uint8_t nPwdSize, uint8_t nClass, uint8_t nMode, uint16_t nUTI, CFW_SIM_ID nSimID)
{
    size_t ivar_size = ALIGNUP8(nPwdSize+1);
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CFW_SsSetFacilityLock) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_CFW_SsSetFacilityLock);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct IPAR_CFW_SsSetFacilityLock *ipar = (struct IPAR_CFW_SsSetFacilityLock *)((char*)mem + sizeof(rpcCallHeader_t));
    struct OPAR_CFW_SsSetFacilityLock *opar = (struct OPAR_CFW_SsSetFacilityLock *)((char*)mem + sizeof(rpcRespHeader_t));
    char *ivar_ptr = (char*)mem + sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CFW_SsSetFacilityLock);
    uint32_t result;

    call->h.size = call_size;
    call->api_tag = TAG_CFW_SsSetFacilityLock;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    ipar->nFac = nFac;
    memcpy(ivar_ptr, nBufPwd, nPwdSize);
    ivar_ptr[nPwdSize] = (char)0;
    ivar_ptr += ALIGNUP8(nPwdSize + 1);
    ipar->nPwdSize = nPwdSize;
    ipar->nClass = nClass;
    ipar->nMode = nMode;
    ipar->nUTI = nUTI;
    ipar->nSimID = nSimID;
    rpcSendCall(RPC_CHANNEL, call);

    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

uint32_t CFW_SsQueryFacilityLock(uint16_t nFac, uint8_t nClass, uint16_t nUTI, CFW_SIM_ID nSimID)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CFW_SsQueryFacilityLock) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_CFW_SsQueryFacilityLock);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct IPAR_CFW_SsQueryFacilityLock *ipar = (struct IPAR_CFW_SsQueryFacilityLock *)((char*)mem + sizeof(rpcCallHeader_t));
    struct OPAR_CFW_SsQueryFacilityLock *opar = (struct OPAR_CFW_SsQueryFacilityLock *)((char*)mem + sizeof(rpcRespHeader_t));
    uint32_t result;

    call->h.size = call_size;
    call->api_tag = TAG_CFW_SsQueryFacilityLock;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    ipar->nFac = nFac;
    ipar->nClass = nClass;
    ipar->nUTI = nUTI;
    ipar->nSimID = nSimID;
    rpcSendCall(RPC_CHANNEL, call);

    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

uint32_t CFW_SsChangePassword(uint16_t nFac, uint8_t * pBufOldPwd, uint8_t nOldPwdSize, uint8_t * pBufNewPwd, uint8_t nNewPwdSize, uint16_t nUTI, CFW_SIM_ID nSimID)
{
    size_t ivar_size = ALIGNUP8(nOldPwdSize+1) + ALIGNUP8(nNewPwdSize+1);
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CFW_SsChangePassword) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_CFW_SsChangePassword);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct IPAR_CFW_SsChangePassword *ipar = (struct IPAR_CFW_SsChangePassword *)((char*)mem + sizeof(rpcCallHeader_t));
    struct OPAR_CFW_SsChangePassword *opar = (struct OPAR_CFW_SsChangePassword *)((char*)mem + sizeof(rpcRespHeader_t));
    char *ivar_ptr = (char*)mem + sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CFW_SsChangePassword);
    uint32_t result;

    call->h.size = call_size;
    call->api_tag = TAG_CFW_SsChangePassword;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    ipar->nFac = nFac;
    memcpy(ivar_ptr, pBufOldPwd, nOldPwdSize);
    ivar_ptr[nOldPwdSize] = (char)0;
    ivar_ptr += ALIGNUP8(nOldPwdSize + 1);
    ipar->nOldPwdSize = nOldPwdSize;
    memcpy(ivar_ptr, pBufNewPwd, nNewPwdSize);
    ivar_ptr[nNewPwdSize] = (char)0;
    ivar_ptr += ALIGNUP8(nNewPwdSize + 1);
    ipar->nNewPwdSize = nNewPwdSize;
    ipar->nUTI = nUTI;
    ipar->nSimID = nSimID;
    rpcSendCall(RPC_CHANNEL, call);

    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

uint32_t CFW_SsQueryClip(uint16_t nUTI, CFW_SIM_ID nSimID)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CFW_SsQueryClip) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_CFW_SsQueryClip);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct IPAR_CFW_SsQueryClip *ipar = (struct IPAR_CFW_SsQueryClip *)((char*)mem + sizeof(rpcCallHeader_t));
    struct OPAR_CFW_SsQueryClip *opar = (struct OPAR_CFW_SsQueryClip *)((char*)mem + sizeof(rpcRespHeader_t));
    uint32_t result;

    call->h.size = call_size;
    call->api_tag = TAG_CFW_SsQueryClip;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    ipar->nUTI = nUTI;
    ipar->nSimID = nSimID;
    rpcSendCall(RPC_CHANNEL, call);

    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

uint32_t CFW_SsQueryClir(uint16_t nUTI, CFW_SIM_ID nSimID)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CFW_SsQueryClir) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_CFW_SsQueryClir);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct IPAR_CFW_SsQueryClir *ipar = (struct IPAR_CFW_SsQueryClir *)((char*)mem + sizeof(rpcCallHeader_t));
    struct OPAR_CFW_SsQueryClir *opar = (struct OPAR_CFW_SsQueryClir *)((char*)mem + sizeof(rpcRespHeader_t));
    uint32_t result;

    call->h.size = call_size;
    call->api_tag = TAG_CFW_SsQueryClir;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    ipar->nUTI = nUTI;
    ipar->nSimID = nSimID;
    rpcSendCall(RPC_CHANNEL, call);

    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

uint32_t CFW_SsQueryColp(uint16_t nUTI, CFW_SIM_ID nSimID)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CFW_SsQueryColp) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_CFW_SsQueryColp);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct IPAR_CFW_SsQueryColp *ipar = (struct IPAR_CFW_SsQueryColp *)((char*)mem + sizeof(rpcCallHeader_t));
    struct OPAR_CFW_SsQueryColp *opar = (struct OPAR_CFW_SsQueryColp *)((char*)mem + sizeof(rpcRespHeader_t));
    uint32_t result;

    call->h.size = call_size;
    call->api_tag = TAG_CFW_SsQueryColp;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    ipar->nUTI = nUTI;
    ipar->nSimID = nSimID;
    rpcSendCall(RPC_CHANNEL, call);

    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

uint32_t CFW_SsQueryColr(uint16_t nUTI, CFW_SIM_ID nSimID)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CFW_SsQueryColr) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_CFW_SsQueryColr);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct IPAR_CFW_SsQueryColr *ipar = (struct IPAR_CFW_SsQueryColr *)((char*)mem + sizeof(rpcCallHeader_t));
    struct OPAR_CFW_SsQueryColr *opar = (struct OPAR_CFW_SsQueryColr *)((char*)mem + sizeof(rpcRespHeader_t));
    uint32_t result;

    call->h.size = call_size;
    call->api_tag = TAG_CFW_SsQueryColr;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    ipar->nUTI = nUTI;
    ipar->nSimID = nSimID;
    rpcSendCall(RPC_CHANNEL, call);

    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

uint32_t CFW_SsSendUSSD(uint8_t * pUsdString, uint8_t nUsdStringSize, uint8_t nOption, uint8_t nDcs, uint16_t nUTI, CFW_SIM_ID nSimID)
{
    size_t ivar_size = ALIGNUP8(nUsdStringSize+1);
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CFW_SsSendUSSD) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_CFW_SsSendUSSD);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct IPAR_CFW_SsSendUSSD *ipar = (struct IPAR_CFW_SsSendUSSD *)((char*)mem + sizeof(rpcCallHeader_t));
    struct OPAR_CFW_SsSendUSSD *opar = (struct OPAR_CFW_SsSendUSSD *)((char*)mem + sizeof(rpcRespHeader_t));
    char *ivar_ptr = (char*)mem + sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CFW_SsSendUSSD);
    uint32_t result;

    call->h.size = call_size;
    call->api_tag = TAG_CFW_SsSendUSSD;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    memcpy(ivar_ptr, pUsdString, nUsdStringSize);
    ivar_ptr[nUsdStringSize] = (char)0;
    ivar_ptr += ALIGNUP8(nUsdStringSize + 1);
    ipar->nUsdStringSize = nUsdStringSize;
    ipar->nOption = nOption;
    ipar->nDcs = nDcs;
    ipar->nUTI = nUTI;
    ipar->nSimID = nSimID;
    rpcSendCall(RPC_CHANNEL, call);

    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

uint32_t CFW_SimChangePassword(uint16_t nFac, uint8_t * pBufOldPwd, uint8_t nOldPwdSize, uint8_t * pBufNewPwd, uint8_t nNewPwdSize, uint16_t nUTI, CFW_SIM_ID nSimID)
{
    size_t ivar_size = ALIGNUP8(nOldPwdSize+1) + ALIGNUP8(nNewPwdSize+1);
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CFW_SimChangePassword) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_CFW_SimChangePassword);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct IPAR_CFW_SimChangePassword *ipar = (struct IPAR_CFW_SimChangePassword *)((char*)mem + sizeof(rpcCallHeader_t));
    struct OPAR_CFW_SimChangePassword *opar = (struct OPAR_CFW_SimChangePassword *)((char*)mem + sizeof(rpcRespHeader_t));
    char *ivar_ptr = (char*)mem + sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CFW_SimChangePassword);
    uint32_t result;

    call->h.size = call_size;
    call->api_tag = TAG_CFW_SimChangePassword;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    ipar->nFac = nFac;
    memcpy(ivar_ptr, pBufOldPwd, nOldPwdSize);
    ivar_ptr[nOldPwdSize] = (char)0;
    ivar_ptr += ALIGNUP8(nOldPwdSize + 1);
    ipar->nOldPwdSize = nOldPwdSize;
    memcpy(ivar_ptr, pBufNewPwd, nNewPwdSize);
    ivar_ptr[nNewPwdSize] = (char)0;
    ivar_ptr += ALIGNUP8(nNewPwdSize + 1);
    ipar->nNewPwdSize = nNewPwdSize;
    ipar->nUTI = nUTI;
    ipar->nSimID = nSimID;
    rpcSendCall(RPC_CHANNEL, call);

    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

uint32_t CFW_SimGetAuthenticationStatus(uint16_t nUTI, CFW_SIM_ID nSimID)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CFW_SimGetAuthenticationStatus) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_CFW_SimGetAuthenticationStatus);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct IPAR_CFW_SimGetAuthenticationStatus *ipar = (struct IPAR_CFW_SimGetAuthenticationStatus *)((char*)mem + sizeof(rpcCallHeader_t));
    struct OPAR_CFW_SimGetAuthenticationStatus *opar = (struct OPAR_CFW_SimGetAuthenticationStatus *)((char*)mem + sizeof(rpcRespHeader_t));
    uint32_t result;

    call->h.size = call_size;
    call->api_tag = TAG_CFW_SimGetAuthenticationStatus;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    ipar->nUTI = nUTI;
    ipar->nSimID = nSimID;
    rpcSendCall(RPC_CHANNEL, call);

    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

uint32_t CFW_SimEnterAuthentication(uint8_t * pPin, uint8_t nPinSize, uint8_t * pNewPin, uint8_t nNewPinSize, uint8_t nOption, uint16_t nUTI, CFW_SIM_ID nSimID)
{
    size_t ivar_size = ALIGNUP8(nPinSize+1) + ALIGNUP8(nNewPinSize+1);
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CFW_SimEnterAuthentication) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_CFW_SimEnterAuthentication);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct IPAR_CFW_SimEnterAuthentication *ipar = (struct IPAR_CFW_SimEnterAuthentication *)((char*)mem + sizeof(rpcCallHeader_t));
    struct OPAR_CFW_SimEnterAuthentication *opar = (struct OPAR_CFW_SimEnterAuthentication *)((char*)mem + sizeof(rpcRespHeader_t));
    char *ivar_ptr = (char*)mem + sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CFW_SimEnterAuthentication);
    uint32_t result;

    call->h.size = call_size;
    call->api_tag = TAG_CFW_SimEnterAuthentication;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    memcpy(ivar_ptr, pPin, nPinSize);
    ivar_ptr[nPinSize] = (char)0;
    ivar_ptr += ALIGNUP8(nPinSize + 1);
    ipar->nPinSize = nPinSize;
    memcpy(ivar_ptr, pNewPin, nNewPinSize);
    ivar_ptr[nNewPinSize] = (char)0;
    ivar_ptr += ALIGNUP8(nNewPinSize + 1);
    ipar->nNewPinSize = nNewPinSize;
    ipar->nOption = nOption;
    ipar->nUTI = nUTI;
    ipar->nSimID = nSimID;
    rpcSendCall(RPC_CHANNEL, call);

    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

uint32_t CFW_SimSetFacilityLock(uint16_t nFac, uint8_t * pBufPwd, uint8_t nPwdSize, uint8_t nMode, uint16_t nUTI, CFW_SIM_ID nSimID)
{
    size_t ivar_size = ALIGNUP8(nPwdSize+1);
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CFW_SimSetFacilityLock) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_CFW_SimSetFacilityLock);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct IPAR_CFW_SimSetFacilityLock *ipar = (struct IPAR_CFW_SimSetFacilityLock *)((char*)mem + sizeof(rpcCallHeader_t));
    struct OPAR_CFW_SimSetFacilityLock *opar = (struct OPAR_CFW_SimSetFacilityLock *)((char*)mem + sizeof(rpcRespHeader_t));
    char *ivar_ptr = (char*)mem + sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CFW_SimSetFacilityLock);
    uint32_t result;

    call->h.size = call_size;
    call->api_tag = TAG_CFW_SimSetFacilityLock;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    ipar->nFac = nFac;
    memcpy(ivar_ptr, pBufPwd, nPwdSize);
    ivar_ptr[nPwdSize] = (char)0;
    ivar_ptr += ALIGNUP8(nPwdSize + 1);
    ipar->nPwdSize = nPwdSize;
    ipar->nMode = nMode;
    ipar->nUTI = nUTI;
    ipar->nSimID = nSimID;
    rpcSendCall(RPC_CHANNEL, call);

    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

uint32_t CFW_SimGetFacilityLock(uint16_t nFac, uint16_t nUTI, CFW_SIM_ID nSimID)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CFW_SimGetFacilityLock) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_CFW_SimGetFacilityLock);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct IPAR_CFW_SimGetFacilityLock *ipar = (struct IPAR_CFW_SimGetFacilityLock *)((char*)mem + sizeof(rpcCallHeader_t));
    struct OPAR_CFW_SimGetFacilityLock *opar = (struct OPAR_CFW_SimGetFacilityLock *)((char*)mem + sizeof(rpcRespHeader_t));
    uint32_t result;

    call->h.size = call_size;
    call->api_tag = TAG_CFW_SimGetFacilityLock;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    ipar->nFac = nFac;
    ipar->nUTI = nUTI;
    ipar->nSimID = nSimID;
    rpcSendCall(RPC_CHANNEL, call);

    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

uint32_t CFW_SimGetProviderId(uint16_t nUTI, CFW_SIM_ID nSimID)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CFW_SimGetProviderId) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_CFW_SimGetProviderId);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct IPAR_CFW_SimGetProviderId *ipar = (struct IPAR_CFW_SimGetProviderId *)((char*)mem + sizeof(rpcCallHeader_t));
    struct OPAR_CFW_SimGetProviderId *opar = (struct OPAR_CFW_SimGetProviderId *)((char*)mem + sizeof(rpcRespHeader_t));
    uint32_t result;

    call->h.size = call_size;
    call->api_tag = TAG_CFW_SimGetProviderId;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    ipar->nUTI = nUTI;
    ipar->nSimID = nSimID;
    rpcSendCall(RPC_CHANNEL, call);

    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

uint32_t CFW_SimSetPrefOperatorList(uint8_t * pOperatorList, uint8_t nSize, uint16_t nUTI, CFW_SIM_ID nSimID)
{
    size_t ivar_size = ALIGNUP8(nSize+1);
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CFW_SimSetPrefOperatorList) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_CFW_SimSetPrefOperatorList);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct IPAR_CFW_SimSetPrefOperatorList *ipar = (struct IPAR_CFW_SimSetPrefOperatorList *)((char*)mem + sizeof(rpcCallHeader_t));
    struct OPAR_CFW_SimSetPrefOperatorList *opar = (struct OPAR_CFW_SimSetPrefOperatorList *)((char*)mem + sizeof(rpcRespHeader_t));
    char *ivar_ptr = (char*)mem + sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CFW_SimSetPrefOperatorList);
    uint32_t result;

    call->h.size = call_size;
    call->api_tag = TAG_CFW_SimSetPrefOperatorList;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    memcpy(ivar_ptr, pOperatorList, nSize);
    ivar_ptr[nSize] = (char)0;
    ivar_ptr += ALIGNUP8(nSize + 1);
    ipar->nSize = nSize;
    ipar->nUTI = nUTI;
    ipar->nSimID = nSimID;
    rpcSendCall(RPC_CHANNEL, call);

    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

uint32_t CFW_SimGetPrefOperatorListMaxNum(uint16_t nUTI, CFW_SIM_ID nSimID)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CFW_SimGetPrefOperatorListMaxNum) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_CFW_SimGetPrefOperatorListMaxNum);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct IPAR_CFW_SimGetPrefOperatorListMaxNum *ipar = (struct IPAR_CFW_SimGetPrefOperatorListMaxNum *)((char*)mem + sizeof(rpcCallHeader_t));
    struct OPAR_CFW_SimGetPrefOperatorListMaxNum *opar = (struct OPAR_CFW_SimGetPrefOperatorListMaxNum *)((char*)mem + sizeof(rpcRespHeader_t));
    uint32_t result;

    call->h.size = call_size;
    call->api_tag = TAG_CFW_SimGetPrefOperatorListMaxNum;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    ipar->nUTI = nUTI;
    ipar->nSimID = nSimID;
    rpcSendCall(RPC_CHANNEL, call);

    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

uint32_t CFW_SimGetPrefOperatorList(uint16_t nUTI, CFW_SIM_ID nSimID)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CFW_SimGetPrefOperatorList) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_CFW_SimGetPrefOperatorList);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct IPAR_CFW_SimGetPrefOperatorList *ipar = (struct IPAR_CFW_SimGetPrefOperatorList *)((char*)mem + sizeof(rpcCallHeader_t));
    struct OPAR_CFW_SimGetPrefOperatorList *opar = (struct OPAR_CFW_SimGetPrefOperatorList *)((char*)mem + sizeof(rpcRespHeader_t));
    uint32_t result;

    call->h.size = call_size;
    call->api_tag = TAG_CFW_SimGetPrefOperatorList;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    ipar->nUTI = nUTI;
    ipar->nSimID = nSimID;
    rpcSendCall(RPC_CHANNEL, call);

    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

uint32_t CFW_SimAddPbkEntry(uint8_t nStorage, CFW_SIM_PBK_ENTRY_INFO *pEntryInfo, uint16_t nUTI, CFW_SIM_ID nSimID)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CFW_SimAddPbkEntry) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_CFW_SimAddPbkEntry);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct IPAR_CFW_SimAddPbkEntry *ipar = (struct IPAR_CFW_SimAddPbkEntry *)((char*)mem + sizeof(rpcCallHeader_t));
    struct OPAR_CFW_SimAddPbkEntry *opar = (struct OPAR_CFW_SimAddPbkEntry *)((char*)mem + sizeof(rpcRespHeader_t));
    uint32_t result;

    call->h.size = call_size;
    call->api_tag = TAG_CFW_SimAddPbkEntry;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    ipar->nStorage = nStorage;
    ipar->pEntryInfo = *pEntryInfo;
    ipar->nUTI = nUTI;
    ipar->nSimID = nSimID;
    rpcSendCall(RPC_CHANNEL, call);

    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

uint32_t CFW_SimAddPbkEntry_V2(uint8_t nStorage, CFW_SIM_PBK_ENTRY_INFO_V2 *pEntryInfo, uint16_t nUTI, CFW_SIM_ID nSimID)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CFW_SimAddPbkEntry_V2) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_CFW_SimAddPbkEntry_V2);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct IPAR_CFW_SimAddPbkEntry_V2 *ipar = (struct IPAR_CFW_SimAddPbkEntry_V2 *)((char*)mem + sizeof(rpcCallHeader_t));
    struct OPAR_CFW_SimAddPbkEntry_V2 *opar = (struct OPAR_CFW_SimAddPbkEntry_V2 *)((char*)mem + sizeof(rpcRespHeader_t));
    uint32_t result;

    call->h.size = call_size;
    call->api_tag = TAG_CFW_SimAddPbkEntry_V2;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    ipar->nStorage = nStorage;
    ipar->pEntryInfo = *pEntryInfo;
    ipar->nUTI = nUTI;
    ipar->nSimID = nSimID;
    rpcSendCall(RPC_CHANNEL, call);

    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

uint32_t CFW_SimDeletePbkEntry(uint8_t nStorage, uint16_t nIndex, uint16_t nUTI, CFW_SIM_ID nSimID)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CFW_SimDeletePbkEntry) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_CFW_SimDeletePbkEntry);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct IPAR_CFW_SimDeletePbkEntry *ipar = (struct IPAR_CFW_SimDeletePbkEntry *)((char*)mem + sizeof(rpcCallHeader_t));
    struct OPAR_CFW_SimDeletePbkEntry *opar = (struct OPAR_CFW_SimDeletePbkEntry *)((char*)mem + sizeof(rpcRespHeader_t));
    uint32_t result;

    call->h.size = call_size;
    call->api_tag = TAG_CFW_SimDeletePbkEntry;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    ipar->nStorage = nStorage;
    ipar->nIndex = nIndex;
    ipar->nUTI = nUTI;
    ipar->nSimID = nSimID;
    rpcSendCall(RPC_CHANNEL, call);

    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

uint32_t CFW_SimGetPbkEntry(uint8_t nStorage, uint16_t nIndex, uint16_t nUTI, CFW_SIM_ID nSimID)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CFW_SimGetPbkEntry) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_CFW_SimGetPbkEntry);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct IPAR_CFW_SimGetPbkEntry *ipar = (struct IPAR_CFW_SimGetPbkEntry *)((char*)mem + sizeof(rpcCallHeader_t));
    struct OPAR_CFW_SimGetPbkEntry *opar = (struct OPAR_CFW_SimGetPbkEntry *)((char*)mem + sizeof(rpcRespHeader_t));
    uint32_t result;

    call->h.size = call_size;
    call->api_tag = TAG_CFW_SimGetPbkEntry;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    ipar->nStorage = nStorage;
    ipar->nIndex = nIndex;
    ipar->nUTI = nUTI;
    ipar->nSimID = nSimID;
    rpcSendCall(RPC_CHANNEL, call);

    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

uint32_t CFW_SimListPbkEntries(uint8_t nStorage, uint16_t nIndexStart, uint16_t nIndexEnd, uint16_t nUTI, CFW_SIM_ID nSimID)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CFW_SimListPbkEntries) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_CFW_SimListPbkEntries);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct IPAR_CFW_SimListPbkEntries *ipar = (struct IPAR_CFW_SimListPbkEntries *)((char*)mem + sizeof(rpcCallHeader_t));
    struct OPAR_CFW_SimListPbkEntries *opar = (struct OPAR_CFW_SimListPbkEntries *)((char*)mem + sizeof(rpcRespHeader_t));
    uint32_t result;

    call->h.size = call_size;
    call->api_tag = TAG_CFW_SimListPbkEntries;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    ipar->nStorage = nStorage;
    ipar->nIndexStart = nIndexStart;
    ipar->nIndexEnd = nIndexEnd;
    ipar->nUTI = nUTI;
    ipar->nSimID = nSimID;
    rpcSendCall(RPC_CHANNEL, call);

    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

uint32_t CFW_SimListCountPbkEntries(uint8_t nStorage, uint16_t nIndexStart, uint16_t nCount, uint16_t nUTI, CFW_SIM_ID nSimID)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CFW_SimListCountPbkEntries) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_CFW_SimListCountPbkEntries);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct IPAR_CFW_SimListCountPbkEntries *ipar = (struct IPAR_CFW_SimListCountPbkEntries *)((char*)mem + sizeof(rpcCallHeader_t));
    struct OPAR_CFW_SimListCountPbkEntries *opar = (struct OPAR_CFW_SimListCountPbkEntries *)((char*)mem + sizeof(rpcRespHeader_t));
    uint32_t result;

    call->h.size = call_size;
    call->api_tag = TAG_CFW_SimListCountPbkEntries;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    ipar->nStorage = nStorage;
    ipar->nIndexStart = nIndexStart;
    ipar->nCount = nCount;
    ipar->nUTI = nUTI;
    ipar->nSimID = nSimID;
    rpcSendCall(RPC_CHANNEL, call);

    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

uint32_t CFW_SimGetPbkStrorageInfo(uint8_t nStorage, uint16_t nUTI, CFW_SIM_ID nSimID)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CFW_SimGetPbkStrorageInfo) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_CFW_SimGetPbkStrorageInfo);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct IPAR_CFW_SimGetPbkStrorageInfo *ipar = (struct IPAR_CFW_SimGetPbkStrorageInfo *)((char*)mem + sizeof(rpcCallHeader_t));
    struct OPAR_CFW_SimGetPbkStrorageInfo *opar = (struct OPAR_CFW_SimGetPbkStrorageInfo *)((char*)mem + sizeof(rpcRespHeader_t));
    uint32_t result;

    call->h.size = call_size;
    call->api_tag = TAG_CFW_SimGetPbkStrorageInfo;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    ipar->nStorage = nStorage;
    ipar->nUTI = nUTI;
    ipar->nSimID = nSimID;
    rpcSendCall(RPC_CHANNEL, call);

    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

uint32_t CFW_SimGetPbkStorage(uint8_t nStorage, uint16_t nUTI, CFW_SIM_ID nSimID)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CFW_SimGetPbkStorage) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_CFW_SimGetPbkStorage);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct IPAR_CFW_SimGetPbkStorage *ipar = (struct IPAR_CFW_SimGetPbkStorage *)((char*)mem + sizeof(rpcCallHeader_t));
    struct OPAR_CFW_SimGetPbkStorage *opar = (struct OPAR_CFW_SimGetPbkStorage *)((char*)mem + sizeof(rpcRespHeader_t));
    uint32_t result;

    call->h.size = call_size;
    call->api_tag = TAG_CFW_SimGetPbkStorage;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    ipar->nStorage = nStorage;
    ipar->nUTI = nUTI;
    ipar->nSimID = nSimID;
    rpcSendCall(RPC_CHANNEL, call);

    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

uint32_t CFW_SatActivation(uint8_t nMode, uint16_t nUTI, CFW_SIM_ID nSimID)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CFW_SatActivation) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_CFW_SatActivation);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct IPAR_CFW_SatActivation *ipar = (struct IPAR_CFW_SatActivation *)((char*)mem + sizeof(rpcCallHeader_t));
    struct OPAR_CFW_SatActivation *opar = (struct OPAR_CFW_SatActivation *)((char*)mem + sizeof(rpcRespHeader_t));
    uint32_t result;

    call->h.size = call_size;
    call->api_tag = TAG_CFW_SatActivation;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    ipar->nMode = nMode;
    ipar->nUTI = nUTI;
    ipar->nSimID = nSimID;
    rpcSendCall(RPC_CHANNEL, call);

    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

uint32_t CFW_SatResponse(uint8_t nCmdType, uint8_t nStatus, uint8_t nItemId, void * pInputString, uint8_t InputStrLen, uint16_t nUTI, CFW_SIM_ID nSimID)
{
    size_t ivar_size = ALIGNUP8(InputStrLen+1);
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CFW_SatResponse) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_CFW_SatResponse);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct IPAR_CFW_SatResponse *ipar = (struct IPAR_CFW_SatResponse *)((char*)mem + sizeof(rpcCallHeader_t));
    struct OPAR_CFW_SatResponse *opar = (struct OPAR_CFW_SatResponse *)((char*)mem + sizeof(rpcRespHeader_t));
    char *ivar_ptr = (char*)mem + sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CFW_SatResponse);
    uint32_t result;

    call->h.size = call_size;
    call->api_tag = TAG_CFW_SatResponse;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    ipar->nCmdType = nCmdType;
    ipar->nStatus = nStatus;
    ipar->nItemId = nItemId;
    memcpy(ivar_ptr, pInputString, InputStrLen);
    ivar_ptr[InputStrLen] = (char)0;
    ivar_ptr += ALIGNUP8(InputStrLen + 1);
    ipar->InputStrLen = InputStrLen;
    ipar->nUTI = nUTI;
    ipar->nSimID = nSimID;
    rpcSendCall(RPC_CHANNEL, call);

    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

uint8_t CFW_SatGetCurCMD(CFW_SIM_ID nSimID)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CFW_SatGetCurCMD) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_CFW_SatGetCurCMD);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct IPAR_CFW_SatGetCurCMD *ipar = (struct IPAR_CFW_SatGetCurCMD *)((char*)mem + sizeof(rpcCallHeader_t));
    struct OPAR_CFW_SatGetCurCMD *opar = (struct OPAR_CFW_SatGetCurCMD *)((char*)mem + sizeof(rpcRespHeader_t));
    uint8_t result;

    call->h.size = call_size;
    call->api_tag = TAG_CFW_SatGetCurCMD;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    ipar->nSimID = nSimID;
    rpcSendCall(RPC_CHANNEL, call);

    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

void CFW_GetMNCLen(uint8_t *pLen, CFW_SIM_ID nSimID)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CFW_GetMNCLen) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_CFW_GetMNCLen);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct IPAR_CFW_GetMNCLen *ipar = (struct IPAR_CFW_GetMNCLen *)((char*)mem + sizeof(rpcCallHeader_t));
    struct OPAR_CFW_GetMNCLen *opar = (struct OPAR_CFW_GetMNCLen *)((char*)mem + sizeof(rpcRespHeader_t));

    call->h.size = call_size;
    call->api_tag = TAG_CFW_GetMNCLen;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    ipar->nSimID = nSimID;
    rpcSendCall(RPC_CHANNEL, call);

    *pLen = opar->pLen;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
}

uint32_t CFW_CfgSimGetMeProfile(CFW_PROFILE *pMeProfile)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + 0 + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_CFW_CfgSimGetMeProfile);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct OPAR_CFW_CfgSimGetMeProfile *opar = (struct OPAR_CFW_CfgSimGetMeProfile *)((char*)mem + sizeof(rpcRespHeader_t));
    uint32_t result;

    call->h.size = call_size;
    call->api_tag = TAG_CFW_CfgSimGetMeProfile;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    rpcSendCall(RPC_CHANNEL, call);

    *pMeProfile = opar->pMeProfile;
    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

uint32_t CFW_CfgSimSetMeProfile(CFW_PROFILE *pMeProfile)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CFW_CfgSimSetMeProfile) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_CFW_CfgSimSetMeProfile);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct IPAR_CFW_CfgSimSetMeProfile *ipar = (struct IPAR_CFW_CfgSimSetMeProfile *)((char*)mem + sizeof(rpcCallHeader_t));
    struct OPAR_CFW_CfgSimSetMeProfile *opar = (struct OPAR_CFW_CfgSimSetMeProfile *)((char*)mem + sizeof(rpcRespHeader_t));
    uint32_t result;

    call->h.size = call_size;
    call->api_tag = TAG_CFW_CfgSimSetMeProfile;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    ipar->pMeProfile = *pMeProfile;
    rpcSendCall(RPC_CHANNEL, call);

    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

uint32_t CFW_SimGetICCID(uint16_t nUTI, CFW_SIM_ID nSimID)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CFW_SimGetICCID) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_CFW_SimGetICCID);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct IPAR_CFW_SimGetICCID *ipar = (struct IPAR_CFW_SimGetICCID *)((char*)mem + sizeof(rpcCallHeader_t));
    struct OPAR_CFW_SimGetICCID *opar = (struct OPAR_CFW_SimGetICCID *)((char*)mem + sizeof(rpcRespHeader_t));
    uint32_t result;

    call->h.size = call_size;
    call->api_tag = TAG_CFW_SimGetICCID;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    ipar->nUTI = nUTI;
    ipar->nSimID = nSimID;
    rpcSendCall(RPC_CHANNEL, call);

    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

uint32_t CFW_SmsAbortSendMessage(CFW_SIM_ID nSimID)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CFW_SmsAbortSendMessage) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_CFW_SmsAbortSendMessage);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct IPAR_CFW_SmsAbortSendMessage *ipar = (struct IPAR_CFW_SmsAbortSendMessage *)((char*)mem + sizeof(rpcCallHeader_t));
    struct OPAR_CFW_SmsAbortSendMessage *opar = (struct OPAR_CFW_SmsAbortSendMessage *)((char*)mem + sizeof(rpcRespHeader_t));
    uint32_t result;

    call->h.size = call_size;
    call->api_tag = TAG_CFW_SmsAbortSendMessage;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    ipar->nSimID = nSimID;
    rpcSendCall(RPC_CHANNEL, call);

    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

uint32_t CFW_SmsDeleteMessage(uint16_t nIndex, uint8_t nStatus, uint8_t nStorage, uint8_t nType, uint16_t nUTI, CFW_SIM_ID nSimID)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CFW_SmsDeleteMessage) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_CFW_SmsDeleteMessage);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct IPAR_CFW_SmsDeleteMessage *ipar = (struct IPAR_CFW_SmsDeleteMessage *)((char*)mem + sizeof(rpcCallHeader_t));
    struct OPAR_CFW_SmsDeleteMessage *opar = (struct OPAR_CFW_SmsDeleteMessage *)((char*)mem + sizeof(rpcRespHeader_t));
    uint32_t result;

    call->h.size = call_size;
    call->api_tag = TAG_CFW_SmsDeleteMessage;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    ipar->nIndex = nIndex;
    ipar->nStatus = nStatus;
    ipar->nStorage = nStorage;
    ipar->nType = nType;
    ipar->nUTI = nUTI;
    ipar->nSimID = nSimID;
    rpcSendCall(RPC_CHANNEL, call);

    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

uint32_t CFW_SmsSetUnRead2Read(uint8_t nStorage, uint8_t nType, uint16_t nIndex, uint16_t nUTI, CFW_SIM_ID nSimID)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CFW_SmsSetUnRead2Read) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_CFW_SmsSetUnRead2Read);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct IPAR_CFW_SmsSetUnRead2Read *ipar = (struct IPAR_CFW_SmsSetUnRead2Read *)((char*)mem + sizeof(rpcCallHeader_t));
    struct OPAR_CFW_SmsSetUnRead2Read *opar = (struct OPAR_CFW_SmsSetUnRead2Read *)((char*)mem + sizeof(rpcRespHeader_t));
    uint32_t result;

    call->h.size = call_size;
    call->api_tag = TAG_CFW_SmsSetUnRead2Read;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    ipar->nStorage = nStorage;
    ipar->nType = nType;
    ipar->nIndex = nIndex;
    ipar->nUTI = nUTI;
    ipar->nSimID = nSimID;
    rpcSendCall(RPC_CHANNEL, call);

    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

uint32_t CFW_SmsListMessages(CFW_SMS_LIST *pListInfo, uint16_t nUTI, CFW_SIM_ID nSimID)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CFW_SmsListMessages) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_CFW_SmsListMessages);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct IPAR_CFW_SmsListMessages *ipar = (struct IPAR_CFW_SmsListMessages *)((char*)mem + sizeof(rpcCallHeader_t));
    struct OPAR_CFW_SmsListMessages *opar = (struct OPAR_CFW_SmsListMessages *)((char*)mem + sizeof(rpcRespHeader_t));
    uint32_t result;

    call->h.size = call_size;
    call->api_tag = TAG_CFW_SmsListMessages;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    ipar->pListInfo = *pListInfo;
    ipar->nUTI = nUTI;
    ipar->nSimID = nSimID;
    rpcSendCall(RPC_CHANNEL, call);

    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

uint32_t CFW_SmsReadMessage(uint8_t nStorage, uint8_t nType, uint16_t nIndex, uint16_t nUTI, CFW_SIM_ID nSimID)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CFW_SmsReadMessage) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_CFW_SmsReadMessage);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct IPAR_CFW_SmsReadMessage *ipar = (struct IPAR_CFW_SmsReadMessage *)((char*)mem + sizeof(rpcCallHeader_t));
    struct OPAR_CFW_SmsReadMessage *opar = (struct OPAR_CFW_SmsReadMessage *)((char*)mem + sizeof(rpcRespHeader_t));
    uint32_t result;

    call->h.size = call_size;
    call->api_tag = TAG_CFW_SmsReadMessage;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    ipar->nStorage = nStorage;
    ipar->nType = nType;
    ipar->nIndex = nIndex;
    ipar->nUTI = nUTI;
    ipar->nSimID = nSimID;
    rpcSendCall(RPC_CHANNEL, call);

    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

uint32_t CFW_SimGetServiceProviderName(uint16_t nUTI, CFW_SIM_ID nSimID)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CFW_SimGetServiceProviderName) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_CFW_SimGetServiceProviderName);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct IPAR_CFW_SimGetServiceProviderName *ipar = (struct IPAR_CFW_SimGetServiceProviderName *)((char*)mem + sizeof(rpcCallHeader_t));
    struct OPAR_CFW_SimGetServiceProviderName *opar = (struct OPAR_CFW_SimGetServiceProviderName *)((char*)mem + sizeof(rpcRespHeader_t));
    uint32_t result;

    call->h.size = call_size;
    call->api_tag = TAG_CFW_SimGetServiceProviderName;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    ipar->nUTI = nUTI;
    ipar->nSimID = nSimID;
    rpcSendCall(RPC_CHANNEL, call);

    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

uint32_t CFW_SmsSetRead2UnRead(uint8_t nStorage, uint8_t nType, uint16_t nIndex, uint16_t nUTI, CFW_SIM_ID nSimID)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CFW_SmsSetRead2UnRead) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_CFW_SmsSetRead2UnRead);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct IPAR_CFW_SmsSetRead2UnRead *ipar = (struct IPAR_CFW_SmsSetRead2UnRead *)((char*)mem + sizeof(rpcCallHeader_t));
    struct OPAR_CFW_SmsSetRead2UnRead *opar = (struct OPAR_CFW_SmsSetRead2UnRead *)((char*)mem + sizeof(rpcRespHeader_t));
    uint32_t result;

    call->h.size = call_size;
    call->api_tag = TAG_CFW_SmsSetRead2UnRead;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    ipar->nStorage = nStorage;
    ipar->nType = nType;
    ipar->nIndex = nIndex;
    ipar->nUTI = nUTI;
    ipar->nSimID = nSimID;
    rpcSendCall(RPC_CHANNEL, call);

    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

void CFW_CheckIfAllowedRecvSms(bool bIfAllowed, CFW_SIM_ID nSimID)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CFW_CheckIfAllowedRecvSms) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + 0;
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct IPAR_CFW_CheckIfAllowedRecvSms *ipar = (struct IPAR_CFW_CheckIfAllowedRecvSms *)((char*)mem + sizeof(rpcCallHeader_t));

    call->h.size = call_size;
    call->api_tag = TAG_CFW_CheckIfAllowedRecvSms;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    ipar->bIfAllowed = bIfAllowed;
    ipar->nSimID = nSimID;
    rpcSendCall(RPC_CHANNEL, call);

    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
}

uint32_t CFW_SmsSendPduMessage(uint8_t * pData, uint16_t nDataSize, uint16_t nUTI, CFW_SIM_ID nSimID)
{
    size_t ivar_size = ALIGNUP8(nDataSize+1);
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CFW_SmsSendPduMessage) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_CFW_SmsSendPduMessage);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct IPAR_CFW_SmsSendPduMessage *ipar = (struct IPAR_CFW_SmsSendPduMessage *)((char*)mem + sizeof(rpcCallHeader_t));
    struct OPAR_CFW_SmsSendPduMessage *opar = (struct OPAR_CFW_SmsSendPduMessage *)((char*)mem + sizeof(rpcRespHeader_t));
    char *ivar_ptr = (char*)mem + sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CFW_SmsSendPduMessage);
    uint32_t result;

    call->h.size = call_size;
    call->api_tag = TAG_CFW_SmsSendPduMessage;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    memcpy(ivar_ptr, pData, nDataSize);
    ivar_ptr[nDataSize] = (char)0;
    ivar_ptr += ALIGNUP8(nDataSize + 1);
    ipar->nDataSize = nDataSize;
    ipar->nUTI = nUTI;
    ipar->nSimID = nSimID;
    rpcSendCall(RPC_CHANNEL, call);

    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

uint32_t CFW_SmsSendMessage_V2(CFW_DIALNUMBER_V2 *pNumber, uint8_t * pData, uint16_t nDataSize, uint16_t nUTI, CFW_SIM_ID nSimID)
{
    size_t ivar_size = ALIGNUP8(nDataSize+1);
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CFW_SmsSendMessage_V2) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_CFW_SmsSendMessage_V2);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct IPAR_CFW_SmsSendMessage_V2 *ipar = (struct IPAR_CFW_SmsSendMessage_V2 *)((char*)mem + sizeof(rpcCallHeader_t));
    struct OPAR_CFW_SmsSendMessage_V2 *opar = (struct OPAR_CFW_SmsSendMessage_V2 *)((char*)mem + sizeof(rpcRespHeader_t));
    char *ivar_ptr = (char*)mem + sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CFW_SmsSendMessage_V2);
    uint32_t result;

    call->h.size = call_size;
    call->api_tag = TAG_CFW_SmsSendMessage_V2;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    ipar->pNumber = *pNumber;
    memcpy(ivar_ptr, pData, nDataSize);
    ivar_ptr[nDataSize] = (char)0;
    ivar_ptr += ALIGNUP8(nDataSize + 1);
    ipar->nDataSize = nDataSize;
    ipar->nUTI = nUTI;
    ipar->nSimID = nSimID;
    rpcSendCall(RPC_CHANNEL, call);

    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

uint32_t CFW_SmsWriteMessage_V2(CFW_DIALNUMBER_V2 *pNumber, uint8_t * pData, uint16_t nDataSize, uint16_t nIndex, uint8_t nStorage, uint8_t nType, uint8_t nStatus, uint16_t nUTI, CFW_SIM_ID nSimID)
{
    size_t ivar_size = ALIGNUP8(nDataSize+1);
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CFW_SmsWriteMessage_V2) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_CFW_SmsWriteMessage_V2);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct IPAR_CFW_SmsWriteMessage_V2 *ipar = (struct IPAR_CFW_SmsWriteMessage_V2 *)((char*)mem + sizeof(rpcCallHeader_t));
    struct OPAR_CFW_SmsWriteMessage_V2 *opar = (struct OPAR_CFW_SmsWriteMessage_V2 *)((char*)mem + sizeof(rpcRespHeader_t));
    char *ivar_ptr = (char*)mem + sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CFW_SmsWriteMessage_V2);
    uint32_t result;

    call->h.size = call_size;
    call->api_tag = TAG_CFW_SmsWriteMessage_V2;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    ipar->pNumber = *pNumber;
    memcpy(ivar_ptr, pData, nDataSize);
    ivar_ptr[nDataSize] = (char)0;
    ivar_ptr += ALIGNUP8(nDataSize + 1);
    ipar->nDataSize = nDataSize;
    ipar->nIndex = nIndex;
    ipar->nStorage = nStorage;
    ipar->nType = nType;
    ipar->nStatus = nStatus;
    ipar->nUTI = nUTI;
    ipar->nSimID = nSimID;
    rpcSendCall(RPC_CHANNEL, call);

    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

uint32_t CFW_SmsSetUnSent2Sent(uint8_t nStorage, uint8_t nType, uint16_t nIndex, uint8_t nSendStatus, uint16_t nUTI, CFW_SIM_ID nSimID)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CFW_SmsSetUnSent2Sent) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_CFW_SmsSetUnSent2Sent);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct IPAR_CFW_SmsSetUnSent2Sent *ipar = (struct IPAR_CFW_SmsSetUnSent2Sent *)((char*)mem + sizeof(rpcCallHeader_t));
    struct OPAR_CFW_SmsSetUnSent2Sent *opar = (struct OPAR_CFW_SmsSetUnSent2Sent *)((char*)mem + sizeof(rpcRespHeader_t));
    uint32_t result;

    call->h.size = call_size;
    call->api_tag = TAG_CFW_SmsSetUnSent2Sent;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    ipar->nStorage = nStorage;
    ipar->nType = nType;
    ipar->nIndex = nIndex;
    ipar->nSendStatus = nSendStatus;
    ipar->nUTI = nUTI;
    ipar->nSimID = nSimID;
    rpcSendCall(RPC_CHANNEL, call);

    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

uint32_t CFW_SmsGetMessageNodeEx(void * pListResult, uint16_t nIndex, CFW_SMS_NODE_EX *pNode, CFW_SIM_ID nSimId)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CFW_SmsGetMessageNodeEx) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_CFW_SmsGetMessageNodeEx);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct IPAR_CFW_SmsGetMessageNodeEx *ipar = (struct IPAR_CFW_SmsGetMessageNodeEx *)((char*)mem + sizeof(rpcCallHeader_t));
    struct OPAR_CFW_SmsGetMessageNodeEx *opar = (struct OPAR_CFW_SmsGetMessageNodeEx *)((char*)mem + sizeof(rpcRespHeader_t));
    uint32_t result;

    call->h.size = call_size;
    call->api_tag = TAG_CFW_SmsGetMessageNodeEx;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    ipar->pListResult = pListResult;
    ipar->nIndex = nIndex;
    ipar->nSimId = nSimId;
    rpcSendCall(RPC_CHANNEL, call);

    *pNode = opar->pNode;
    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

uint32_t CFW_SmsInitComplete(uint16_t nUTI, CFW_SIM_ID nSimID)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CFW_SmsInitComplete) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_CFW_SmsInitComplete);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct IPAR_CFW_SmsInitComplete *ipar = (struct IPAR_CFW_SmsInitComplete *)((char*)mem + sizeof(rpcCallHeader_t));
    struct OPAR_CFW_SmsInitComplete *opar = (struct OPAR_CFW_SmsInitComplete *)((char*)mem + sizeof(rpcRespHeader_t));
    uint32_t result;

    call->h.size = call_size;
    call->api_tag = TAG_CFW_SmsInitComplete;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    ipar->nUTI = nUTI;
    ipar->nSimID = nSimID;
    rpcSendCall(RPC_CHANNEL, call);

    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

uint32_t CFW_SetSMSConcat(bool bConcat, CFW_SIM_ID nSimID)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CFW_SetSMSConcat) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_CFW_SetSMSConcat);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct IPAR_CFW_SetSMSConcat *ipar = (struct IPAR_CFW_SetSMSConcat *)((char*)mem + sizeof(rpcCallHeader_t));
    struct OPAR_CFW_SetSMSConcat *opar = (struct OPAR_CFW_SetSMSConcat *)((char*)mem + sizeof(rpcRespHeader_t));
    uint32_t result;

    call->h.size = call_size;
    call->api_tag = TAG_CFW_SetSMSConcat;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    ipar->bConcat = bConcat;
    ipar->nSimID = nSimID;
    rpcSendCall(RPC_CHANNEL, call);

    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

bool CFW_GetSMSConcat(CFW_SIM_ID nSimID)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CFW_GetSMSConcat) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_CFW_GetSMSConcat);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct IPAR_CFW_GetSMSConcat *ipar = (struct IPAR_CFW_GetSMSConcat *)((char*)mem + sizeof(rpcCallHeader_t));
    struct OPAR_CFW_GetSMSConcat *opar = (struct OPAR_CFW_GetSMSConcat *)((char*)mem + sizeof(rpcRespHeader_t));
    bool result;

    call->h.size = call_size;
    call->api_tag = TAG_CFW_GetSMSConcat;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    ipar->nSimID = nSimID;
    rpcSendCall(RPC_CHANNEL, call);

    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

uint32_t CFW_SimReadMessage(uint16_t nLocation, uint32_t nIndex, uint16_t nUTI, CFW_SIM_ID nSimID)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CFW_SimReadMessage) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_CFW_SimReadMessage);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct IPAR_CFW_SimReadMessage *ipar = (struct IPAR_CFW_SimReadMessage *)((char*)mem + sizeof(rpcCallHeader_t));
    struct OPAR_CFW_SimReadMessage *opar = (struct OPAR_CFW_SimReadMessage *)((char*)mem + sizeof(rpcRespHeader_t));
    uint32_t result;

    call->h.size = call_size;
    call->api_tag = TAG_CFW_SimReadMessage;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    ipar->nLocation = nLocation;
    ipar->nIndex = nIndex;
    ipar->nUTI = nUTI;
    ipar->nSimID = nSimID;
    rpcSendCall(RPC_CHANNEL, call);

    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

uint32_t CFW_SimWriteMessage(uint16_t nLocation, uint32_t nIndex, uint8_t * pData, uint8_t nDataSize, uint16_t nUTI, CFW_SIM_ID nSimID)
{
    size_t ivar_size = ALIGNUP8(nDataSize+1);
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CFW_SimWriteMessage) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_CFW_SimWriteMessage);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct IPAR_CFW_SimWriteMessage *ipar = (struct IPAR_CFW_SimWriteMessage *)((char*)mem + sizeof(rpcCallHeader_t));
    struct OPAR_CFW_SimWriteMessage *opar = (struct OPAR_CFW_SimWriteMessage *)((char*)mem + sizeof(rpcRespHeader_t));
    char *ivar_ptr = (char*)mem + sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CFW_SimWriteMessage);
    uint32_t result;

    call->h.size = call_size;
    call->api_tag = TAG_CFW_SimWriteMessage;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    ipar->nLocation = nLocation;
    ipar->nIndex = nIndex;
    memcpy(ivar_ptr, pData, nDataSize);
    ivar_ptr[nDataSize] = (char)0;
    ivar_ptr += ALIGNUP8(nDataSize + 1);
    ipar->nDataSize = nDataSize;
    ipar->nUTI = nUTI;
    ipar->nSimID = nSimID;
    rpcSendCall(RPC_CHANNEL, call);

    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

#if defined(CFW_GPRS_SUPPORT)
uint32_t CFW_GprsSetPdpCxtV2(uint8_t nCid, const CFW_GPRS_PDPCONT_INFO_V2 *pPdpCont, CFW_SIM_ID nSimID)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CFW_GprsSetPdpCxtV2) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_CFW_GprsSetPdpCxtV2);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct IPAR_CFW_GprsSetPdpCxtV2 *ipar = (struct IPAR_CFW_GprsSetPdpCxtV2 *)((char*)mem + sizeof(rpcCallHeader_t));
    struct OPAR_CFW_GprsSetPdpCxtV2 *opar = (struct OPAR_CFW_GprsSetPdpCxtV2 *)((char*)mem + sizeof(rpcRespHeader_t));
    uint32_t result;

    call->h.size = call_size;
    call->api_tag = TAG_CFW_GprsSetPdpCxtV2;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    ipar->nCid = nCid;
    ipar->pPdpCont = *pPdpCont;
    ipar->nSimID = nSimID;
    rpcSendCall(RPC_CHANNEL, call);

    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

#endif

#if defined(CFW_GPRS_SUPPORT)
uint32_t CFW_GprsGetPdpCxtV2(uint8_t nCid, CFW_GPRS_PDPCONT_INFO_V2 *pPdpCont, CFW_SIM_ID nSimID)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CFW_GprsGetPdpCxtV2) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_CFW_GprsGetPdpCxtV2);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct IPAR_CFW_GprsGetPdpCxtV2 *ipar = (struct IPAR_CFW_GprsGetPdpCxtV2 *)((char*)mem + sizeof(rpcCallHeader_t));
    struct OPAR_CFW_GprsGetPdpCxtV2 *opar = (struct OPAR_CFW_GprsGetPdpCxtV2 *)((char*)mem + sizeof(rpcRespHeader_t));
    uint32_t result;

    call->h.size = call_size;
    call->api_tag = TAG_CFW_GprsGetPdpCxtV2;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    ipar->nCid = nCid;
    ipar->nSimID = nSimID;
    rpcSendCall(RPC_CHANNEL, call);

    *pPdpCont = opar->pPdpCont;
    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

#endif

#if defined(CFW_GPRS_SUPPORT)
uint32_t CFW_GprsSetMinQos(uint8_t nCid, CFW_GPRS_QOS *pQos, CFW_SIM_ID nSimID)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CFW_GprsSetMinQos) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_CFW_GprsSetMinQos);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct IPAR_CFW_GprsSetMinQos *ipar = (struct IPAR_CFW_GprsSetMinQos *)((char*)mem + sizeof(rpcCallHeader_t));
    struct OPAR_CFW_GprsSetMinQos *opar = (struct OPAR_CFW_GprsSetMinQos *)((char*)mem + sizeof(rpcRespHeader_t));
    uint32_t result;

    call->h.size = call_size;
    call->api_tag = TAG_CFW_GprsSetMinQos;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    ipar->nCid = nCid;
    ipar->pQos = *pQos;
    ipar->nSimID = nSimID;
    rpcSendCall(RPC_CHANNEL, call);

    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

#endif

#if defined(CFW_GPRS_SUPPORT)
uint32_t CFW_GprsGetMInQos(uint8_t nCid, CFW_GPRS_QOS *pQos, CFW_SIM_ID nSimID)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CFW_GprsGetMInQos) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_CFW_GprsGetMInQos);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct IPAR_CFW_GprsGetMInQos *ipar = (struct IPAR_CFW_GprsGetMInQos *)((char*)mem + sizeof(rpcCallHeader_t));
    struct OPAR_CFW_GprsGetMInQos *opar = (struct OPAR_CFW_GprsGetMInQos *)((char*)mem + sizeof(rpcRespHeader_t));
    uint32_t result;

    call->h.size = call_size;
    call->api_tag = TAG_CFW_GprsGetMInQos;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    ipar->nCid = nCid;
    ipar->nSimID = nSimID;
    rpcSendCall(RPC_CHANNEL, call);

    *pQos = opar->pQos;
    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

#endif

#if defined(CFW_GPRS_SUPPORT)
uint32_t CFW_GprsSetReqQos(uint8_t nCid, CFW_GPRS_QOS *pQos, CFW_SIM_ID nSimID)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CFW_GprsSetReqQos) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_CFW_GprsSetReqQos);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct IPAR_CFW_GprsSetReqQos *ipar = (struct IPAR_CFW_GprsSetReqQos *)((char*)mem + sizeof(rpcCallHeader_t));
    struct OPAR_CFW_GprsSetReqQos *opar = (struct OPAR_CFW_GprsSetReqQos *)((char*)mem + sizeof(rpcRespHeader_t));
    uint32_t result;

    call->h.size = call_size;
    call->api_tag = TAG_CFW_GprsSetReqQos;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    ipar->nCid = nCid;
    ipar->pQos = *pQos;
    ipar->nSimID = nSimID;
    rpcSendCall(RPC_CHANNEL, call);

    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

#endif

#if defined(CFW_GPRS_SUPPORT)
uint32_t CFW_GprsGetReqQos(uint8_t nCid, CFW_GPRS_QOS *pQos, CFW_SIM_ID nSimID)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CFW_GprsGetReqQos) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_CFW_GprsGetReqQos);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct IPAR_CFW_GprsGetReqQos *ipar = (struct IPAR_CFW_GprsGetReqQos *)((char*)mem + sizeof(rpcCallHeader_t));
    struct OPAR_CFW_GprsGetReqQos *opar = (struct OPAR_CFW_GprsGetReqQos *)((char*)mem + sizeof(rpcRespHeader_t));
    uint32_t result;

    call->h.size = call_size;
    call->api_tag = TAG_CFW_GprsGetReqQos;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    ipar->nCid = nCid;
    ipar->nSimID = nSimID;
    rpcSendCall(RPC_CHANNEL, call);

    *pQos = opar->pQos;
    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

#endif

#if defined(CFW_GPRS_SUPPORT)
uint32_t CFW_GprsGetPdpAddr(uint8_t nCid, uint8_t *nLength, uint8_t pPdpAdd[THE_PDP_ADDR_MAX_LEN], CFW_SIM_ID nSimID)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CFW_GprsGetPdpAddr) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_CFW_GprsGetPdpAddr);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct IPAR_CFW_GprsGetPdpAddr *ipar = (struct IPAR_CFW_GprsGetPdpAddr *)((char*)mem + sizeof(rpcCallHeader_t));
    struct OPAR_CFW_GprsGetPdpAddr *opar = (struct OPAR_CFW_GprsGetPdpAddr *)((char*)mem + sizeof(rpcRespHeader_t));
    uint32_t result;

    call->h.size = call_size;
    call->api_tag = TAG_CFW_GprsGetPdpAddr;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    ipar->nCid = nCid;
    ipar->nSimID = nSimID;
    rpcSendCall(RPC_CHANNEL, call);

    *nLength = opar->nLength;
    memcpy(pPdpAdd, opar->pPdpAdd, sizeof(opar->pPdpAdd));
    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

#endif

#if defined(CFW_GPRS_SUPPORT)
uint32_t CFW_GprsGetstatus(CFW_NW_STATUS_INFO *pStatus, CFW_SIM_ID nSimID)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CFW_GprsGetstatus) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_CFW_GprsGetstatus);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct IPAR_CFW_GprsGetstatus *ipar = (struct IPAR_CFW_GprsGetstatus *)((char*)mem + sizeof(rpcCallHeader_t));
    struct OPAR_CFW_GprsGetstatus *opar = (struct OPAR_CFW_GprsGetstatus *)((char*)mem + sizeof(rpcRespHeader_t));
    uint32_t result;

    call->h.size = call_size;
    call->api_tag = TAG_CFW_GprsGetstatus;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    ipar->nSimID = nSimID;
    rpcSendCall(RPC_CHANNEL, call);

    *pStatus = opar->pStatus;
    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

#endif

#if defined(CFW_GPRS_SUPPORT)
uint32_t CFW_GetGprsAttState(uint8_t *pState, CFW_SIM_ID nSimID)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CFW_GetGprsAttState) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_CFW_GetGprsAttState);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct IPAR_CFW_GetGprsAttState *ipar = (struct IPAR_CFW_GetGprsAttState *)((char*)mem + sizeof(rpcCallHeader_t));
    struct OPAR_CFW_GetGprsAttState *opar = (struct OPAR_CFW_GetGprsAttState *)((char*)mem + sizeof(rpcRespHeader_t));
    uint32_t result;

    call->h.size = call_size;
    call->api_tag = TAG_CFW_GetGprsAttState;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    ipar->nSimID = nSimID;
    rpcSendCall(RPC_CHANNEL, call);

    *pState = opar->pState;
    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

#endif

#if defined(CFW_GPRS_SUPPORT)
uint32_t CFW_GetGprsActState(uint8_t nCid, uint8_t *pState, CFW_SIM_ID nSimID)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CFW_GetGprsActState) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_CFW_GetGprsActState);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct IPAR_CFW_GetGprsActState *ipar = (struct IPAR_CFW_GetGprsActState *)((char*)mem + sizeof(rpcCallHeader_t));
    struct OPAR_CFW_GetGprsActState *opar = (struct OPAR_CFW_GetGprsActState *)((char*)mem + sizeof(rpcRespHeader_t));
    uint32_t result;

    call->h.size = call_size;
    call->api_tag = TAG_CFW_GetGprsActState;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    ipar->nCid = nCid;
    ipar->nSimID = nSimID;
    rpcSendCall(RPC_CHANNEL, call);

    *pState = opar->pState;
    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

#endif

#if defined(CFW_GPRS_SUPPORT)
uint32_t CFW_GprsAct(uint8_t nState, uint8_t nCid, uint16_t nUTI, CFW_SIM_ID nSimID)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CFW_GprsAct) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_CFW_GprsAct);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct IPAR_CFW_GprsAct *ipar = (struct IPAR_CFW_GprsAct *)((char*)mem + sizeof(rpcCallHeader_t));
    struct OPAR_CFW_GprsAct *opar = (struct OPAR_CFW_GprsAct *)((char*)mem + sizeof(rpcRespHeader_t));
    uint32_t result;

    call->h.size = call_size;
    call->api_tag = TAG_CFW_GprsAct;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    ipar->nState = nState;
    ipar->nCid = nCid;
    ipar->nUTI = nUTI;
    ipar->nSimID = nSimID;
    rpcSendCall(RPC_CHANNEL, call);

    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

#endif

#if defined(CFW_GPRS_SUPPORT)
uint32_t CFW_GprsActEx(uint8_t nState, uint8_t nCid, uint16_t nUTI, CFW_SIM_ID nSimID, bool SavedInApp)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CFW_GprsActEx) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_CFW_GprsActEx);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct IPAR_CFW_GprsActEx *ipar = (struct IPAR_CFW_GprsActEx *)((char*)mem + sizeof(rpcCallHeader_t));
    struct OPAR_CFW_GprsActEx *opar = (struct OPAR_CFW_GprsActEx *)((char*)mem + sizeof(rpcRespHeader_t));
    uint32_t result;

    call->h.size = call_size;
    call->api_tag = TAG_CFW_GprsActEx;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    ipar->nState = nState;
    ipar->nCid = nCid;
    ipar->nUTI = nUTI;
    ipar->nSimID = nSimID;
    ipar->SavedInApp = SavedInApp;
    rpcSendCall(RPC_CHANNEL, call);

    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

#endif

#if defined(CFW_GPRS_SUPPORT)
uint32_t CFW_AttDetach(uint8_t nState, uint16_t nUTI, uint8_t AttDetachType, CFW_SIM_ID nSimID)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CFW_AttDetach) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_CFW_AttDetach);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct IPAR_CFW_AttDetach *ipar = (struct IPAR_CFW_AttDetach *)((char*)mem + sizeof(rpcCallHeader_t));
    struct OPAR_CFW_AttDetach *opar = (struct OPAR_CFW_AttDetach *)((char*)mem + sizeof(rpcRespHeader_t));
    uint32_t result;

    call->h.size = call_size;
    call->api_tag = TAG_CFW_AttDetach;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    ipar->nState = nState;
    ipar->nUTI = nUTI;
    ipar->AttDetachType = AttDetachType;
    ipar->nSimID = nSimID;
    rpcSendCall(RPC_CHANNEL, call);

    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

#endif

#if defined(CFW_GPRS_SUPPORT) && defined(LTE_SUPPORT)
uint32_t CFW_GprsSecPdpAct(uint8_t nState, uint8_t nCid, uint8_t nPCid, uint16_t nUTI, CFW_SIM_ID nSimID)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CFW_GprsSecPdpAct) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_CFW_GprsSecPdpAct);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct IPAR_CFW_GprsSecPdpAct *ipar = (struct IPAR_CFW_GprsSecPdpAct *)((char*)mem + sizeof(rpcCallHeader_t));
    struct OPAR_CFW_GprsSecPdpAct *opar = (struct OPAR_CFW_GprsSecPdpAct *)((char*)mem + sizeof(rpcRespHeader_t));
    uint32_t result;

    call->h.size = call_size;
    call->api_tag = TAG_CFW_GprsSecPdpAct;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    ipar->nState = nState;
    ipar->nCid = nCid;
    ipar->nPCid = nPCid;
    ipar->nUTI = nUTI;
    ipar->nSimID = nSimID;
    rpcSendCall(RPC_CHANNEL, call);

    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

#endif

#if defined(CFW_GPRS_SUPPORT) && defined(LTE_SUPPORT)
uint32_t CFW_GprsSetPsm(uint8_t psmEnable, uint8_t bitmap, uint8_t t3324, uint8_t t3412, uint8_t nSimID)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CFW_GprsSetPsm) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_CFW_GprsSetPsm);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct IPAR_CFW_GprsSetPsm *ipar = (struct IPAR_CFW_GprsSetPsm *)((char*)mem + sizeof(rpcCallHeader_t));
    struct OPAR_CFW_GprsSetPsm *opar = (struct OPAR_CFW_GprsSetPsm *)((char*)mem + sizeof(rpcRespHeader_t));
    uint32_t result;

    call->h.size = call_size;
    call->api_tag = TAG_CFW_GprsSetPsm;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    ipar->psmEnable = psmEnable;
    ipar->bitmap = bitmap;
    ipar->t3324 = t3324;
    ipar->t3412 = t3412;
    ipar->nSimID = nSimID;
    rpcSendCall(RPC_CHANNEL, call);

    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

#endif

#if defined(CFW_GPRS_SUPPORT) && defined(LTE_SUPPORT)
uint32_t CFW_GprsSetCtxTFT(uint8_t nCid, CFW_TFT_SET *pTft, uint8_t nSimID)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CFW_GprsSetCtxTFT) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_CFW_GprsSetCtxTFT);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct IPAR_CFW_GprsSetCtxTFT *ipar = (struct IPAR_CFW_GprsSetCtxTFT *)((char*)mem + sizeof(rpcCallHeader_t));
    struct OPAR_CFW_GprsSetCtxTFT *opar = (struct OPAR_CFW_GprsSetCtxTFT *)((char*)mem + sizeof(rpcRespHeader_t));
    uint32_t result;

    call->h.size = call_size;
    call->api_tag = TAG_CFW_GprsSetCtxTFT;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    ipar->nCid = nCid;
    ipar->pTft = *pTft;
    ipar->nSimID = nSimID;
    rpcSendCall(RPC_CHANNEL, call);

    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

#endif

#if defined(CFW_GPRS_SUPPORT) && defined(LTE_SUPPORT)
uint32_t CFW_GprsSetCtxEQos(uint8_t nCid, CFW_EQOS *pQos, uint8_t nSimID)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CFW_GprsSetCtxEQos) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_CFW_GprsSetCtxEQos);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct IPAR_CFW_GprsSetCtxEQos *ipar = (struct IPAR_CFW_GprsSetCtxEQos *)((char*)mem + sizeof(rpcCallHeader_t));
    struct OPAR_CFW_GprsSetCtxEQos *opar = (struct OPAR_CFW_GprsSetCtxEQos *)((char*)mem + sizeof(rpcRespHeader_t));
    uint32_t result;

    call->h.size = call_size;
    call->api_tag = TAG_CFW_GprsSetCtxEQos;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    ipar->nCid = nCid;
    ipar->pQos = *pQos;
    ipar->nSimID = nSimID;
    rpcSendCall(RPC_CHANNEL, call);

    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

#endif

#if defined(CFW_GPRS_SUPPORT) && defined(LTE_SUPPORT)
uint32_t CFW_GprsGetCtxEQos(uint8_t nCid, CFW_EQOS *pQos, CFW_SIM_ID nSimID)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CFW_GprsGetCtxEQos) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_CFW_GprsGetCtxEQos);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct IPAR_CFW_GprsGetCtxEQos *ipar = (struct IPAR_CFW_GprsGetCtxEQos *)((char*)mem + sizeof(rpcCallHeader_t));
    struct OPAR_CFW_GprsGetCtxEQos *opar = (struct OPAR_CFW_GprsGetCtxEQos *)((char*)mem + sizeof(rpcRespHeader_t));
    uint32_t result;

    call->h.size = call_size;
    call->api_tag = TAG_CFW_GprsGetCtxEQos;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    ipar->nCid = nCid;
    ipar->nSimID = nSimID;
    rpcSendCall(RPC_CHANNEL, call);

    *pQos = opar->pQos;
    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

#endif

#if defined(CFW_GPRS_SUPPORT) && defined(LTE_SUPPORT)
uint32_t CFW_GprsCtxEpsModify(uint16_t nUTI, uint8_t nCid, CFW_SIM_ID nSimID)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CFW_GprsCtxEpsModify) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_CFW_GprsCtxEpsModify);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct IPAR_CFW_GprsCtxEpsModify *ipar = (struct IPAR_CFW_GprsCtxEpsModify *)((char*)mem + sizeof(rpcCallHeader_t));
    struct OPAR_CFW_GprsCtxEpsModify *opar = (struct OPAR_CFW_GprsCtxEpsModify *)((char*)mem + sizeof(rpcRespHeader_t));
    uint32_t result;

    call->h.size = call_size;
    call->api_tag = TAG_CFW_GprsCtxEpsModify;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    ipar->nUTI = nUTI;
    ipar->nCid = nCid;
    ipar->nSimID = nSimID;
    rpcSendCall(RPC_CHANNEL, call);

    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

#endif

#if defined(CFW_GPRS_SUPPORT)
uint32_t CFW_GprsManualAcc(uint16_t nUTI, uint8_t nCid, CFW_SIM_ID nSimID)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CFW_GprsManualAcc) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_CFW_GprsManualAcc);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct IPAR_CFW_GprsManualAcc *ipar = (struct IPAR_CFW_GprsManualAcc *)((char*)mem + sizeof(rpcCallHeader_t));
    struct OPAR_CFW_GprsManualAcc *opar = (struct OPAR_CFW_GprsManualAcc *)((char*)mem + sizeof(rpcRespHeader_t));
    uint32_t result;

    call->h.size = call_size;
    call->api_tag = TAG_CFW_GprsManualAcc;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    ipar->nUTI = nUTI;
    ipar->nCid = nCid;
    ipar->nSimID = nSimID;
    rpcSendCall(RPC_CHANNEL, call);

    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

#endif

#if defined(CFW_GPRS_SUPPORT)
uint32_t CFW_GprsManualRej(uint16_t nUTI, uint8_t nCid, CFW_SIM_ID nSimID)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CFW_GprsManualRej) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_CFW_GprsManualRej);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct IPAR_CFW_GprsManualRej *ipar = (struct IPAR_CFW_GprsManualRej *)((char*)mem + sizeof(rpcCallHeader_t));
    struct OPAR_CFW_GprsManualRej *opar = (struct OPAR_CFW_GprsManualRej *)((char*)mem + sizeof(rpcRespHeader_t));
    uint32_t result;

    call->h.size = call_size;
    call->api_tag = TAG_CFW_GprsManualRej;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    ipar->nUTI = nUTI;
    ipar->nCid = nCid;
    ipar->nSimID = nSimID;
    rpcSendCall(RPC_CHANNEL, call);

    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

#endif

#if defined(CFW_GPRS_SUPPORT)
uint32_t CFW_GprsCtxModify(uint16_t nUTI, uint8_t nCid, CFW_GPRS_QOS *Qos, CFW_SIM_ID nSimID)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CFW_GprsCtxModify) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_CFW_GprsCtxModify);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct IPAR_CFW_GprsCtxModify *ipar = (struct IPAR_CFW_GprsCtxModify *)((char*)mem + sizeof(rpcCallHeader_t));
    struct OPAR_CFW_GprsCtxModify *opar = (struct OPAR_CFW_GprsCtxModify *)((char*)mem + sizeof(rpcRespHeader_t));
    uint32_t result;

    call->h.size = call_size;
    call->api_tag = TAG_CFW_GprsCtxModify;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    ipar->nUTI = nUTI;
    ipar->nCid = nCid;
    ipar->Qos = *Qos;
    ipar->nSimID = nSimID;
    rpcSendCall(RPC_CHANNEL, call);

    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

#endif

#if defined(CFW_GPRS_SUPPORT)
uint32_t CFW_GprsCtxModifyAcc(uint16_t nUTI, uint8_t nCid, CFW_SIM_ID nSimID)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CFW_GprsCtxModifyAcc) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_CFW_GprsCtxModifyAcc);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct IPAR_CFW_GprsCtxModifyAcc *ipar = (struct IPAR_CFW_GprsCtxModifyAcc *)((char*)mem + sizeof(rpcCallHeader_t));
    struct OPAR_CFW_GprsCtxModifyAcc *opar = (struct OPAR_CFW_GprsCtxModifyAcc *)((char*)mem + sizeof(rpcRespHeader_t));
    uint32_t result;

    call->h.size = call_size;
    call->api_tag = TAG_CFW_GprsCtxModifyAcc;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    ipar->nUTI = nUTI;
    ipar->nCid = nCid;
    ipar->nSimID = nSimID;
    rpcSendCall(RPC_CHANNEL, call);

    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

#endif

#if defined(CFW_GPRS_SUPPORT)
uint32_t CFW_GprsCtxModifyRej(uint16_t nUTI, uint8_t nCid, CFW_SIM_ID nSimID)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CFW_GprsCtxModifyRej) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_CFW_GprsCtxModifyRej);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct IPAR_CFW_GprsCtxModifyRej *ipar = (struct IPAR_CFW_GprsCtxModifyRej *)((char*)mem + sizeof(rpcCallHeader_t));
    struct OPAR_CFW_GprsCtxModifyRej *opar = (struct OPAR_CFW_GprsCtxModifyRej *)((char*)mem + sizeof(rpcRespHeader_t));
    uint32_t result;

    call->h.size = call_size;
    call->api_tag = TAG_CFW_GprsCtxModifyRej;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    ipar->nUTI = nUTI;
    ipar->nCid = nCid;
    ipar->nSimID = nSimID;
    rpcSendCall(RPC_CHANNEL, call);

    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

#endif

#if defined(CFW_GPRS_SUPPORT)
uint32_t CFW_GprsSendData(uint8_t nCid, CFW_GPRS_DATA* pGprsData, uint8_t rai, CFW_SIM_ID nSimID)
{
    size_t ivar_size = ALIGNUP8(sizeof(CFW_GPRS_DATA) + pGprsData->nDataLength+1);
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CFW_GprsSendData) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_CFW_GprsSendData);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct IPAR_CFW_GprsSendData *ipar = (struct IPAR_CFW_GprsSendData *)((char*)mem + sizeof(rpcCallHeader_t));
    struct OPAR_CFW_GprsSendData *opar = (struct OPAR_CFW_GprsSendData *)((char*)mem + sizeof(rpcRespHeader_t));
    char *ivar_ptr = (char*)mem + sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CFW_GprsSendData);
    uint32_t result;

    call->h.size = call_size;
    call->api_tag = TAG_CFW_GprsSendData;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    ipar->nCid = nCid;
    memcpy(ivar_ptr, pGprsData, sizeof(CFW_GPRS_DATA) + pGprsData->nDataLength);
    ivar_ptr[sizeof(CFW_GPRS_DATA) + pGprsData->nDataLength] = (char)0;
    ivar_ptr += ALIGNUP8(sizeof(CFW_GPRS_DATA) + pGprsData->nDataLength + 1);
    ipar->rai = rai;
    ipar->nSimID = nSimID;
    rpcSendCall(RPC_CHANNEL, call);

    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

#endif

#if defined(CFW_GPRS_SUPPORT)
uint32_t CFW_GprsAtt(uint8_t nState, uint16_t nUTI, CFW_SIM_ID nSimID)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CFW_GprsAtt) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_CFW_GprsAtt);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct IPAR_CFW_GprsAtt *ipar = (struct IPAR_CFW_GprsAtt *)((char*)mem + sizeof(rpcCallHeader_t));
    struct OPAR_CFW_GprsAtt *opar = (struct OPAR_CFW_GprsAtt *)((char*)mem + sizeof(rpcRespHeader_t));
    uint32_t result;

    call->h.size = call_size;
    call->api_tag = TAG_CFW_GprsAtt;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    ipar->nState = nState;
    ipar->nUTI = nUTI;
    ipar->nSimID = nSimID;
    rpcSendCall(RPC_CHANNEL, call);

    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

#endif

#if defined(CFW_GPRS_SUPPORT)
uint32_t CFW_GetGprsSum(int32_t *upsum, int32_t *downsum, CFW_SIM_ID nSimID)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CFW_GetGprsSum) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_CFW_GetGprsSum);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct IPAR_CFW_GetGprsSum *ipar = (struct IPAR_CFW_GetGprsSum *)((char*)mem + sizeof(rpcCallHeader_t));
    struct OPAR_CFW_GetGprsSum *opar = (struct OPAR_CFW_GetGprsSum *)((char*)mem + sizeof(rpcRespHeader_t));
    uint32_t result;

    call->h.size = call_size;
    call->api_tag = TAG_CFW_GetGprsSum;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    ipar->nSimID = nSimID;
    rpcSendCall(RPC_CHANNEL, call);

    *upsum = opar->upsum;
    *downsum = opar->downsum;
    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

#endif

#if defined(CFW_GPRS_SUPPORT)
uint32_t CFW_ClearGprsSum(CFW_SIM_ID nSimID)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CFW_ClearGprsSum) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_CFW_ClearGprsSum);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct IPAR_CFW_ClearGprsSum *ipar = (struct IPAR_CFW_ClearGprsSum *)((char*)mem + sizeof(rpcCallHeader_t));
    struct OPAR_CFW_ClearGprsSum *opar = (struct OPAR_CFW_ClearGprsSum *)((char*)mem + sizeof(rpcRespHeader_t));
    uint32_t result;

    call->h.size = call_size;
    call->api_tag = TAG_CFW_ClearGprsSum;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    ipar->nSimID = nSimID;
    rpcSendCall(RPC_CHANNEL, call);

    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

#endif

#if defined(CFW_GPRS_SUPPORT)
uint8_t CFW_GprsSetSmsSeviceMode(uint8_t nService)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CFW_GprsSetSmsSeviceMode) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_CFW_GprsSetSmsSeviceMode);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct IPAR_CFW_GprsSetSmsSeviceMode *ipar = (struct IPAR_CFW_GprsSetSmsSeviceMode *)((char*)mem + sizeof(rpcCallHeader_t));
    struct OPAR_CFW_GprsSetSmsSeviceMode *opar = (struct OPAR_CFW_GprsSetSmsSeviceMode *)((char*)mem + sizeof(rpcRespHeader_t));
    uint8_t result;

    call->h.size = call_size;
    call->api_tag = TAG_CFW_GprsSetSmsSeviceMode;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    ipar->nService = nService;
    rpcSendCall(RPC_CHANNEL, call);

    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

#endif

#if defined(CFW_GPRS_SUPPORT)
uint8_t CFW_GprsGetSmsSeviceMode(void)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + 0 + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_CFW_GprsGetSmsSeviceMode);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct OPAR_CFW_GprsGetSmsSeviceMode *opar = (struct OPAR_CFW_GprsGetSmsSeviceMode *)((char*)mem + sizeof(rpcRespHeader_t));
    uint8_t result;

    call->h.size = call_size;
    call->api_tag = TAG_CFW_GprsGetSmsSeviceMode;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    rpcSendCall(RPC_CHANNEL, call);

    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

#endif

#if defined(CFW_GPRS_SUPPORT)
uint32_t CFW_SetDnsServerbyPdp(uint8_t nCid, uint8_t nSimID, uint8_t Pro_DnsIp[4], uint8_t Sec_DnsIp[4])
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CFW_SetDnsServerbyPdp) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_CFW_SetDnsServerbyPdp);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct IPAR_CFW_SetDnsServerbyPdp *ipar = (struct IPAR_CFW_SetDnsServerbyPdp *)((char*)mem + sizeof(rpcCallHeader_t));
    struct OPAR_CFW_SetDnsServerbyPdp *opar = (struct OPAR_CFW_SetDnsServerbyPdp *)((char*)mem + sizeof(rpcRespHeader_t));
    uint32_t result;

    call->h.size = call_size;
    call->api_tag = TAG_CFW_SetDnsServerbyPdp;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    ipar->nCid = nCid;
    ipar->nSimID = nSimID;
    memcpy(ipar->Pro_DnsIp, Pro_DnsIp, sizeof(ipar->Pro_DnsIp));
    memcpy(ipar->Sec_DnsIp, Sec_DnsIp, sizeof(ipar->Sec_DnsIp));
    rpcSendCall(RPC_CHANNEL, call);

    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

#endif

#if defined(CFW_GPRS_SUPPORT)
uint32_t CFW_getDnsServerbyPdp(uint8_t nCid, uint8_t nSimID)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CFW_getDnsServerbyPdp) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_CFW_getDnsServerbyPdp);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct IPAR_CFW_getDnsServerbyPdp *ipar = (struct IPAR_CFW_getDnsServerbyPdp *)((char*)mem + sizeof(rpcCallHeader_t));
    struct OPAR_CFW_getDnsServerbyPdp *opar = (struct OPAR_CFW_getDnsServerbyPdp *)((char*)mem + sizeof(rpcRespHeader_t));
    uint32_t result;

    call->h.size = call_size;
    call->api_tag = TAG_CFW_getDnsServerbyPdp;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    ipar->nCid = nCid;
    ipar->nSimID = nSimID;
    rpcSendCall(RPC_CHANNEL, call);

    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

#endif

#if defined(CFW_GPRS_SUPPORT)
uint32_t* getDNSServer(uint8_t nCid, uint8_t nSimID)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_getDNSServer) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_getDNSServer);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct IPAR_getDNSServer *ipar = (struct IPAR_getDNSServer *)((char*)mem + sizeof(rpcCallHeader_t));
    struct OPAR_getDNSServer *opar = (struct OPAR_getDNSServer *)((char*)mem + sizeof(rpcRespHeader_t));
    uint32_t* result;

    call->h.size = call_size;
    call->api_tag = TAG_getDNSServer;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    ipar->nCid = nCid;
    ipar->nSimID = nSimID;
    rpcSendCall(RPC_CHANNEL, call);

    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

#endif

bool CFW_EmodGetBatteryInfo(CFW_EMOD_BATTERY_INFO *pBatInfo)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + 0 + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_CFW_EmodGetBatteryInfo);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct OPAR_CFW_EmodGetBatteryInfo *opar = (struct OPAR_CFW_EmodGetBatteryInfo *)((char*)mem + sizeof(rpcRespHeader_t));
    bool result;

    call->h.size = call_size;
    call->api_tag = TAG_CFW_EmodGetBatteryInfo;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    rpcSendCall(RPC_CHANNEL, call);

    *pBatInfo = opar->pBatInfo;
    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

void CFW_EmodAudioTestStart(uint8_t type)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CFW_EmodAudioTestStart) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + 0;
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct IPAR_CFW_EmodAudioTestStart *ipar = (struct IPAR_CFW_EmodAudioTestStart *)((char*)mem + sizeof(rpcCallHeader_t));

    call->h.size = call_size;
    call->api_tag = TAG_CFW_EmodAudioTestStart;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    ipar->type = type;
    rpcSendCall(RPC_CHANNEL, call);

    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
}

void CFW_EmodAudioTestEnd(void)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + 0 + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + 0;
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;

    call->h.size = call_size;
    call->api_tag = TAG_CFW_EmodAudioTestEnd;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    rpcSendCall(RPC_CHANNEL, call);

    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
}

bool CFW_EmodClearUserInfo(void)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + 0 + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_CFW_EmodClearUserInfo);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct OPAR_CFW_EmodClearUserInfo *opar = (struct OPAR_CFW_EmodClearUserInfo *)((char*)mem + sizeof(rpcRespHeader_t));
    bool result;

    call->h.size = call_size;
    call->api_tag = TAG_CFW_EmodClearUserInfo;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    rpcSendCall(RPC_CHANNEL, call);

    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

void CFW_EmodGetIMEI(uint8_t pImei[16], uint8_t *pImeiLen, CFW_SIM_ID nSimID)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CFW_EmodGetIMEI) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_CFW_EmodGetIMEI);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct IPAR_CFW_EmodGetIMEI *ipar = (struct IPAR_CFW_EmodGetIMEI *)((char*)mem + sizeof(rpcCallHeader_t));
    struct OPAR_CFW_EmodGetIMEI *opar = (struct OPAR_CFW_EmodGetIMEI *)((char*)mem + sizeof(rpcRespHeader_t));

    call->h.size = call_size;
    call->api_tag = TAG_CFW_EmodGetIMEI;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    ipar->nSimID = nSimID;
    rpcSendCall(RPC_CHANNEL, call);

    memcpy(pImei, opar->pImei, sizeof(opar->pImei));
    *pImeiLen = opar->pImeiLen;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
}

void CFW_EmodSaveIMEI(uint8_t * pImei, uint8_t pImeiLen, CFW_SIM_ID nSimID)
{
    size_t ivar_size = ALIGNUP8(pImeiLen+1);
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CFW_EmodSaveIMEI) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + 0;
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct IPAR_CFW_EmodSaveIMEI *ipar = (struct IPAR_CFW_EmodSaveIMEI *)((char*)mem + sizeof(rpcCallHeader_t));
    char *ivar_ptr = (char*)mem + sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CFW_EmodSaveIMEI);

    call->h.size = call_size;
    call->api_tag = TAG_CFW_EmodSaveIMEI;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    memcpy(ivar_ptr, pImei, pImeiLen);
    ivar_ptr[pImeiLen] = (char)0;
    ivar_ptr += ALIGNUP8(pImeiLen + 1);
    ipar->pImeiLen = pImeiLen;
    ipar->nSimID = nSimID;
    rpcSendCall(RPC_CHANNEL, call);

    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
}

uint32_t CFW_EmodGetNvVersion(void)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + 0 + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_CFW_EmodGetNvVersion);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct OPAR_CFW_EmodGetNvVersion *opar = (struct OPAR_CFW_EmodGetNvVersion *)((char*)mem + sizeof(rpcRespHeader_t));
    uint32_t result;

    call->h.size = call_size;
    call->api_tag = TAG_CFW_EmodGetNvVersion;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    rpcSendCall(RPC_CHANNEL, call);

    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

uint8_t * CFW_EmodGetNvData(uint8_t dataType, uint8_t nSim)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CFW_EmodGetNvData) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_CFW_EmodGetNvData);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct IPAR_CFW_EmodGetNvData *ipar = (struct IPAR_CFW_EmodGetNvData *)((char*)mem + sizeof(rpcCallHeader_t));
    struct OPAR_CFW_EmodGetNvData *opar = (struct OPAR_CFW_EmodGetNvData *)((char*)mem + sizeof(rpcRespHeader_t));
    uint8_t * result;

    call->h.size = call_size;
    call->api_tag = TAG_CFW_EmodGetNvData;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    ipar->dataType = dataType;
    ipar->nSim = nSim;
    rpcSendCall(RPC_CHANNEL, call);

    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

int32_t CFW_EmodSetNvData(uint8_t dataType, uint16_t offset, uint16_t dataLength, uint8_t * data, uint8_t nSim)
{
    size_t ivar_size = ALIGNUP8(dataLength+1);
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CFW_EmodSetNvData) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_CFW_EmodSetNvData);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct IPAR_CFW_EmodSetNvData *ipar = (struct IPAR_CFW_EmodSetNvData *)((char*)mem + sizeof(rpcCallHeader_t));
    struct OPAR_CFW_EmodSetNvData *opar = (struct OPAR_CFW_EmodSetNvData *)((char*)mem + sizeof(rpcRespHeader_t));
    char *ivar_ptr = (char*)mem + sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CFW_EmodSetNvData);
    int32_t result;

    call->h.size = call_size;
    call->api_tag = TAG_CFW_EmodSetNvData;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    ipar->dataType = dataType;
    ipar->offset = offset;
    ipar->dataLength = dataLength;
    memcpy(ivar_ptr, data, dataLength);
    ivar_ptr[dataLength] = (char)0;
    ivar_ptr += ALIGNUP8(dataLength + 1);
    ipar->nSim = nSim;
    rpcSendCall(RPC_CHANNEL, call);

    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

int32_t CFW_EmodSaveNvData(uint8_t dataType, uint8_t nSim)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CFW_EmodSaveNvData) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_CFW_EmodSaveNvData);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct IPAR_CFW_EmodSaveNvData *ipar = (struct IPAR_CFW_EmodSaveNvData *)((char*)mem + sizeof(rpcCallHeader_t));
    struct OPAR_CFW_EmodSaveNvData *opar = (struct OPAR_CFW_EmodSaveNvData *)((char*)mem + sizeof(rpcRespHeader_t));
    int32_t result;

    call->h.size = call_size;
    call->api_tag = TAG_CFW_EmodSaveNvData;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    ipar->dataType = dataType;
    ipar->nSim = nSim;
    rpcSendCall(RPC_CHANNEL, call);

    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

int32_t CFW_EmodSpecialNvProcess(uint8_t operType, uint8_t dataType, uint16_t offset, uint16_t dataLength, uint8_t * data, uint8_t nSim)
{
    size_t ivar_size = ALIGNUP8(dataLength+1);
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CFW_EmodSpecialNvProcess) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_CFW_EmodSpecialNvProcess);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct IPAR_CFW_EmodSpecialNvProcess *ipar = (struct IPAR_CFW_EmodSpecialNvProcess *)((char*)mem + sizeof(rpcCallHeader_t));
    struct OPAR_CFW_EmodSpecialNvProcess *opar = (struct OPAR_CFW_EmodSpecialNvProcess *)((char*)mem + sizeof(rpcRespHeader_t));
    char *ivar_ptr = (char*)mem + sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CFW_EmodSpecialNvProcess);
    int32_t result;

    call->h.size = call_size;
    call->api_tag = TAG_CFW_EmodSpecialNvProcess;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    ipar->operType = operType;
    ipar->dataType = dataType;
    ipar->offset = offset;
    ipar->dataLength = dataLength;
    memcpy(ivar_ptr, data, dataLength);
    ivar_ptr[dataLength] = (char)0;
    ivar_ptr += ALIGNUP8(dataLength + 1);
    ipar->nSim = nSim;
    rpcSendCall(RPC_CHANNEL, call);

    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

uint32_t CFW_EmodEngineerCmd(uint32_t module, uint32_t cmd, uint32_t v1, uint32_t v2, uint32_t v3)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CFW_EmodEngineerCmd) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_CFW_EmodEngineerCmd);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct IPAR_CFW_EmodEngineerCmd *ipar = (struct IPAR_CFW_EmodEngineerCmd *)((char*)mem + sizeof(rpcCallHeader_t));
    struct OPAR_CFW_EmodEngineerCmd *opar = (struct OPAR_CFW_EmodEngineerCmd *)((char*)mem + sizeof(rpcRespHeader_t));
    uint32_t result;

    call->h.size = call_size;
    call->api_tag = TAG_CFW_EmodEngineerCmd;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    ipar->module = module;
    ipar->cmd = cmd;
    ipar->v1 = v1;
    ipar->v2 = v2;
    ipar->v3 = v3;
    rpcSendCall(RPC_CHANNEL, call);

    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

void CFW_EmodReadBindSIM(CFW_EMOD_READ_BIND_SIM *pReadBindSim)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + 0 + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_CFW_EmodReadBindSIM);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct OPAR_CFW_EmodReadBindSIM *opar = (struct OPAR_CFW_EmodReadBindSIM *)((char*)mem + sizeof(rpcRespHeader_t));

    call->h.size = call_size;
    call->api_tag = TAG_CFW_EmodReadBindSIM;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    rpcSendCall(RPC_CHANNEL, call);

    *pReadBindSim = opar->pReadBindSim;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
}

void CFW_EmodReadBindNW(CFW_EMOD_READ_BIND_NW *pReadBindNw)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + 0 + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_CFW_EmodReadBindNW);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct OPAR_CFW_EmodReadBindNW *opar = (struct OPAR_CFW_EmodReadBindNW *)((char*)mem + sizeof(rpcRespHeader_t));

    call->h.size = call_size;
    call->api_tag = TAG_CFW_EmodReadBindNW;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    rpcSendCall(RPC_CHANNEL, call);

    *pReadBindNw = opar->pReadBindNw;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
}

void CFW_EmodGetBindInfo(bool *bSim, bool *bNetWork)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + 0 + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_CFW_EmodGetBindInfo);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct OPAR_CFW_EmodGetBindInfo *opar = (struct OPAR_CFW_EmodGetBindInfo *)((char*)mem + sizeof(rpcRespHeader_t));

    call->h.size = call_size;
    call->api_tag = TAG_CFW_EmodGetBindInfo;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    rpcSendCall(RPC_CHANNEL, call);

    *bSim = opar->bSim;
    *bNetWork = opar->bNetWork;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
}

uint32_t CFW_EmodSetBindInfo(bool bSim, bool bNetWork)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CFW_EmodSetBindInfo) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_CFW_EmodSetBindInfo);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct IPAR_CFW_EmodSetBindInfo *ipar = (struct IPAR_CFW_EmodSetBindInfo *)((char*)mem + sizeof(rpcCallHeader_t));
    struct OPAR_CFW_EmodSetBindInfo *opar = (struct OPAR_CFW_EmodSetBindInfo *)((char*)mem + sizeof(rpcRespHeader_t));
    uint32_t result;

    call->h.size = call_size;
    call->api_tag = TAG_CFW_EmodSetBindInfo;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    ipar->bSim = bSim;
    ipar->bNetWork = bNetWork;
    rpcSendCall(RPC_CHANNEL, call);

    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

CFW_EMOD_UPDATE_RETURN CFW_EmodUpdateSimBind(uint8_t nIndex, uint8_t * pBindSimNum, uint8_t nLen)
{
    size_t ivar_size = ALIGNUP8(nLen+1);
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CFW_EmodUpdateSimBind) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_CFW_EmodUpdateSimBind);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct IPAR_CFW_EmodUpdateSimBind *ipar = (struct IPAR_CFW_EmodUpdateSimBind *)((char*)mem + sizeof(rpcCallHeader_t));
    struct OPAR_CFW_EmodUpdateSimBind *opar = (struct OPAR_CFW_EmodUpdateSimBind *)((char*)mem + sizeof(rpcRespHeader_t));
    char *ivar_ptr = (char*)mem + sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CFW_EmodUpdateSimBind);
    CFW_EMOD_UPDATE_RETURN result;

    call->h.size = call_size;
    call->api_tag = TAG_CFW_EmodUpdateSimBind;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    ipar->nIndex = nIndex;
    memcpy(ivar_ptr, pBindSimNum, nLen);
    ivar_ptr[nLen] = (char)0;
    ivar_ptr += ALIGNUP8(nLen + 1);
    ipar->nLen = nLen;
    rpcSendCall(RPC_CHANNEL, call);

    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

CFW_EMOD_UPDATE_RETURN CFW_EmodUpdateNwBind(uint8_t nIndex, uint8_t * pBindNwNum, uint8_t nLen)
{
    size_t ivar_size = ALIGNUP8(nLen+1);
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CFW_EmodUpdateNwBind) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_CFW_EmodUpdateNwBind);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct IPAR_CFW_EmodUpdateNwBind *ipar = (struct IPAR_CFW_EmodUpdateNwBind *)((char*)mem + sizeof(rpcCallHeader_t));
    struct OPAR_CFW_EmodUpdateNwBind *opar = (struct OPAR_CFW_EmodUpdateNwBind *)((char*)mem + sizeof(rpcRespHeader_t));
    char *ivar_ptr = (char*)mem + sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CFW_EmodUpdateNwBind);
    CFW_EMOD_UPDATE_RETURN result;

    call->h.size = call_size;
    call->api_tag = TAG_CFW_EmodUpdateNwBind;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    ipar->nIndex = nIndex;
    memcpy(ivar_ptr, pBindNwNum, nLen);
    ivar_ptr[nLen] = (char)0;
    ivar_ptr += ALIGNUP8(nLen + 1);
    ipar->nLen = nLen;
    rpcSendCall(RPC_CHANNEL, call);

    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

void CFW_EmodGetProductInfo(CFW_EMOD_LOCT_PRODUCT_INFO *pProduct)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + 0 + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_CFW_EmodGetProductInfo);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct OPAR_CFW_EmodGetProductInfo *opar = (struct OPAR_CFW_EmodGetProductInfo *)((char*)mem + sizeof(rpcRespHeader_t));

    call->h.size = call_size;
    call->api_tag = TAG_CFW_EmodGetProductInfo;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    rpcSendCall(RPC_CHANNEL, call);

    *pProduct = opar->pProduct;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
}

void CFW_EmodGetRFCalibrationInfo(CFW_EMOD_RF_CALIB_INFO *pRfCalib)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + 0 + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_CFW_EmodGetRFCalibrationInfo);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct OPAR_CFW_EmodGetRFCalibrationInfo *opar = (struct OPAR_CFW_EmodGetRFCalibrationInfo *)((char*)mem + sizeof(rpcRespHeader_t));

    call->h.size = call_size;
    call->api_tag = TAG_CFW_EmodGetRFCalibrationInfo;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    rpcSendCall(RPC_CHANNEL, call);

    *pRfCalib = opar->pRfCalib;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
}

uint32_t CFW_EmodOutfieldTestStart(CFW_TSM_FUNCTION_SELECT *pSelecFUN, uint16_t nUTI, CFW_SIM_ID nSimID)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CFW_EmodOutfieldTestStart) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_CFW_EmodOutfieldTestStart);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct IPAR_CFW_EmodOutfieldTestStart *ipar = (struct IPAR_CFW_EmodOutfieldTestStart *)((char*)mem + sizeof(rpcCallHeader_t));
    struct OPAR_CFW_EmodOutfieldTestStart *opar = (struct OPAR_CFW_EmodOutfieldTestStart *)((char*)mem + sizeof(rpcRespHeader_t));
    uint32_t result;

    call->h.size = call_size;
    call->api_tag = TAG_CFW_EmodOutfieldTestStart;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    ipar->pSelecFUN = *pSelecFUN;
    ipar->nUTI = nUTI;
    ipar->nSimID = nSimID;
    rpcSendCall(RPC_CHANNEL, call);

    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

uint32_t CFW_EmodOutfieldTestEnd(uint16_t nUTI, CFW_SIM_ID nSimID)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CFW_EmodOutfieldTestEnd) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_CFW_EmodOutfieldTestEnd);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct IPAR_CFW_EmodOutfieldTestEnd *ipar = (struct IPAR_CFW_EmodOutfieldTestEnd *)((char*)mem + sizeof(rpcCallHeader_t));
    struct OPAR_CFW_EmodOutfieldTestEnd *opar = (struct OPAR_CFW_EmodOutfieldTestEnd *)((char*)mem + sizeof(rpcRespHeader_t));
    uint32_t result;

    call->h.size = call_size;
    call->api_tag = TAG_CFW_EmodOutfieldTestEnd;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    ipar->nUTI = nUTI;
    ipar->nSimID = nSimID;
    rpcSendCall(RPC_CHANNEL, call);

    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

uint32_t CFW_GetCellInfo(CFW_TSM_CURR_CELL_INFO *pCurrCellInfo, CFW_TSM_ALL_NEBCELL_INFO *pNeighborCellInfo, CFW_SIM_ID nSimID)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CFW_GetCellInfo) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_CFW_GetCellInfo);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct IPAR_CFW_GetCellInfo *ipar = (struct IPAR_CFW_GetCellInfo *)((char*)mem + sizeof(rpcCallHeader_t));
    struct OPAR_CFW_GetCellInfo *opar = (struct OPAR_CFW_GetCellInfo *)((char*)mem + sizeof(rpcRespHeader_t));
    uint32_t result;

    call->h.size = call_size;
    call->api_tag = TAG_CFW_GetCellInfo;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    ipar->nSimID = nSimID;
    rpcSendCall(RPC_CHANNEL, call);

    *pCurrCellInfo = opar->pCurrCellInfo;
    *pNeighborCellInfo = opar->pNeighborCellInfo;
    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

uint32_t CFW_EmodSyncInfoTest(bool bStart, uint16_t nUTI, CFW_SIM_ID nSimID)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CFW_EmodSyncInfoTest) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_CFW_EmodSyncInfoTest);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct IPAR_CFW_EmodSyncInfoTest *ipar = (struct IPAR_CFW_EmodSyncInfoTest *)((char*)mem + sizeof(rpcCallHeader_t));
    struct OPAR_CFW_EmodSyncInfoTest *opar = (struct OPAR_CFW_EmodSyncInfoTest *)((char*)mem + sizeof(rpcRespHeader_t));
    uint32_t result;

    call->h.size = call_size;
    call->api_tag = TAG_CFW_EmodSyncInfoTest;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    ipar->bStart = bStart;
    ipar->nUTI = nUTI;
    ipar->nSimID = nSimID;
    rpcSendCall(RPC_CHANNEL, call);

    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

uint32_t CFW_GetFreeCID(uint8_t *pCID, CFW_SIM_ID nSimID)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CFW_GetFreeCID) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_CFW_GetFreeCID);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct IPAR_CFW_GetFreeCID *ipar = (struct IPAR_CFW_GetFreeCID *)((char*)mem + sizeof(rpcCallHeader_t));
    struct OPAR_CFW_GetFreeCID *opar = (struct OPAR_CFW_GetFreeCID *)((char*)mem + sizeof(rpcRespHeader_t));
    uint32_t result;

    call->h.size = call_size;
    call->api_tag = TAG_CFW_GetFreeCID;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    ipar->nSimID = nSimID;
    rpcSendCall(RPC_CHANNEL, call);

    *pCID = opar->pCID;
    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

uint32_t CFW_ReleaseCID(uint8_t nCID, CFW_SIM_ID nSimID)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CFW_ReleaseCID) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_CFW_ReleaseCID);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct IPAR_CFW_ReleaseCID *ipar = (struct IPAR_CFW_ReleaseCID *)((char*)mem + sizeof(rpcCallHeader_t));
    struct OPAR_CFW_ReleaseCID *opar = (struct OPAR_CFW_ReleaseCID *)((char*)mem + sizeof(rpcRespHeader_t));
    uint32_t result;

    call->h.size = call_size;
    call->api_tag = TAG_CFW_ReleaseCID;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    ipar->nCID = nCID;
    ipar->nSimID = nSimID;
    rpcSendCall(RPC_CHANNEL, call);

    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

uint32_t CFW_SetCID(uint8_t nCID, CFW_SIM_ID nSimID)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CFW_SetCID) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_CFW_SetCID);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct IPAR_CFW_SetCID *ipar = (struct IPAR_CFW_SetCID *)((char*)mem + sizeof(rpcCallHeader_t));
    struct OPAR_CFW_SetCID *opar = (struct OPAR_CFW_SetCID *)((char*)mem + sizeof(rpcRespHeader_t));
    uint32_t result;

    call->h.size = call_size;
    call->api_tag = TAG_CFW_SetCID;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    ipar->nCID = nCID;
    ipar->nSimID = nSimID;
    rpcSendCall(RPC_CHANNEL, call);

    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

uint8_t CFW_isPdpActiveAvailable(CFW_SIM_ID nSimID)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CFW_isPdpActiveAvailable) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_CFW_isPdpActiveAvailable);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct IPAR_CFW_isPdpActiveAvailable *ipar = (struct IPAR_CFW_isPdpActiveAvailable *)((char*)mem + sizeof(rpcCallHeader_t));
    struct OPAR_CFW_isPdpActiveAvailable *opar = (struct OPAR_CFW_isPdpActiveAvailable *)((char*)mem + sizeof(rpcRespHeader_t));
    uint8_t result;

    call->h.size = call_size;
    call->api_tag = TAG_CFW_isPdpActiveAvailable;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    ipar->nSimID = nSimID;
    rpcSendCall(RPC_CHANNEL, call);

    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

uint32_t cfw_SimReadElementaryFile(uint16_t nUTI, uint32_t fileId, CFW_SIM_ID nSimID)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_cfw_SimReadElementaryFile) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_cfw_SimReadElementaryFile);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct IPAR_cfw_SimReadElementaryFile *ipar = (struct IPAR_cfw_SimReadElementaryFile *)((char*)mem + sizeof(rpcCallHeader_t));
    struct OPAR_cfw_SimReadElementaryFile *opar = (struct OPAR_cfw_SimReadElementaryFile *)((char*)mem + sizeof(rpcRespHeader_t));
    uint32_t result;

    call->h.size = call_size;
    call->api_tag = TAG_cfw_SimReadElementaryFile;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    ipar->nUTI = nUTI;
    ipar->fileId = fileId;
    ipar->nSimID = nSimID;
    rpcSendCall(RPC_CHANNEL, call);

    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

uint32_t CFW_SimGetACMMax(uint16_t nUTI, CFW_SIM_ID nSimID)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CFW_SimGetACMMax) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_CFW_SimGetACMMax);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct IPAR_CFW_SimGetACMMax *ipar = (struct IPAR_CFW_SimGetACMMax *)((char*)mem + sizeof(rpcCallHeader_t));
    struct OPAR_CFW_SimGetACMMax *opar = (struct OPAR_CFW_SimGetACMMax *)((char*)mem + sizeof(rpcRespHeader_t));
    uint32_t result;

    call->h.size = call_size;
    call->api_tag = TAG_CFW_SimGetACMMax;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    ipar->nUTI = nUTI;
    ipar->nSimID = nSimID;
    rpcSendCall(RPC_CHANNEL, call);

    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

uint32_t CFW_SimSetACMMax(uint32_t iACMMaxValue, uint8_t * pPin2, uint8_t nPinSize, uint16_t Nuti, CFW_SIM_ID nSimID)
{
    size_t ivar_size = ALIGNUP8(nPinSize+1);
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CFW_SimSetACMMax) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_CFW_SimSetACMMax);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct IPAR_CFW_SimSetACMMax *ipar = (struct IPAR_CFW_SimSetACMMax *)((char*)mem + sizeof(rpcCallHeader_t));
    struct OPAR_CFW_SimSetACMMax *opar = (struct OPAR_CFW_SimSetACMMax *)((char*)mem + sizeof(rpcRespHeader_t));
    char *ivar_ptr = (char*)mem + sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CFW_SimSetACMMax);
    uint32_t result;

    call->h.size = call_size;
    call->api_tag = TAG_CFW_SimSetACMMax;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    ipar->iACMMaxValue = iACMMaxValue;
    memcpy(ivar_ptr, pPin2, nPinSize);
    ivar_ptr[nPinSize] = (char)0;
    ivar_ptr += ALIGNUP8(nPinSize + 1);
    ipar->nPinSize = nPinSize;
    ipar->Nuti = Nuti;
    ipar->nSimID = nSimID;
    rpcSendCall(RPC_CHANNEL, call);

    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

uint32_t CFW_SimGetACM(uint16_t nUTI, CFW_SIM_ID nSimID)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CFW_SimGetACM) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_CFW_SimGetACM);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct IPAR_CFW_SimGetACM *ipar = (struct IPAR_CFW_SimGetACM *)((char*)mem + sizeof(rpcCallHeader_t));
    struct OPAR_CFW_SimGetACM *opar = (struct OPAR_CFW_SimGetACM *)((char*)mem + sizeof(rpcRespHeader_t));
    uint32_t result;

    call->h.size = call_size;
    call->api_tag = TAG_CFW_SimGetACM;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    ipar->nUTI = nUTI;
    ipar->nSimID = nSimID;
    rpcSendCall(RPC_CHANNEL, call);

    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

uint32_t CFW_SimSetACM(uint32_t iCurValue, uint8_t * pPin2, uint8_t nPinSize, uint16_t Nuti, CFW_SIM_ID nSimID)
{
    size_t ivar_size = ALIGNUP8(nPinSize+1);
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CFW_SimSetACM) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_CFW_SimSetACM);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct IPAR_CFW_SimSetACM *ipar = (struct IPAR_CFW_SimSetACM *)((char*)mem + sizeof(rpcCallHeader_t));
    struct OPAR_CFW_SimSetACM *opar = (struct OPAR_CFW_SimSetACM *)((char*)mem + sizeof(rpcRespHeader_t));
    char *ivar_ptr = (char*)mem + sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CFW_SimSetACM);
    uint32_t result;

    call->h.size = call_size;
    call->api_tag = TAG_CFW_SimSetACM;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    ipar->iCurValue = iCurValue;
    memcpy(ivar_ptr, pPin2, nPinSize);
    ivar_ptr[nPinSize] = (char)0;
    ivar_ptr += ALIGNUP8(nPinSize + 1);
    ipar->nPinSize = nPinSize;
    ipar->Nuti = Nuti;
    ipar->nSimID = nSimID;
    rpcSendCall(RPC_CHANNEL, call);

    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

uint32_t CFW_SimGetPUCT(uint16_t nUTI, CFW_SIM_ID nSimID)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CFW_SimGetPUCT) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_CFW_SimGetPUCT);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct IPAR_CFW_SimGetPUCT *ipar = (struct IPAR_CFW_SimGetPUCT *)((char*)mem + sizeof(rpcCallHeader_t));
    struct OPAR_CFW_SimGetPUCT *opar = (struct OPAR_CFW_SimGetPUCT *)((char*)mem + sizeof(rpcRespHeader_t));
    uint32_t result;

    call->h.size = call_size;
    call->api_tag = TAG_CFW_SimGetPUCT;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    ipar->nUTI = nUTI;
    ipar->nSimID = nSimID;
    rpcSendCall(RPC_CHANNEL, call);

    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

uint32_t CFW_SimSetPUCT(CFW_SIM_PUCT_INFO *pPUCT, uint8_t * pPin2, uint8_t nPinSize, uint16_t nUTI, CFW_SIM_ID nSimID)
{
    size_t ivar_size = ALIGNUP8(nPinSize+1);
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CFW_SimSetPUCT) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_CFW_SimSetPUCT);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct IPAR_CFW_SimSetPUCT *ipar = (struct IPAR_CFW_SimSetPUCT *)((char*)mem + sizeof(rpcCallHeader_t));
    struct OPAR_CFW_SimSetPUCT *opar = (struct OPAR_CFW_SimSetPUCT *)((char*)mem + sizeof(rpcRespHeader_t));
    char *ivar_ptr = (char*)mem + sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CFW_SimSetPUCT);
    uint32_t result;

    call->h.size = call_size;
    call->api_tag = TAG_CFW_SimSetPUCT;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    ipar->pPUCT = *pPUCT;
    memcpy(ivar_ptr, pPin2, nPinSize);
    ivar_ptr[nPinSize] = (char)0;
    ivar_ptr += ALIGNUP8(nPinSize + 1);
    ipar->nPinSize = nPinSize;
    ipar->nUTI = nUTI;
    ipar->nSimID = nSimID;
    rpcSendCall(RPC_CHANNEL, call);

    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

void CFW_EmodSetLteLockFreq(uint8_t number, uint32_t * freq, CFW_SIM_ID nSimId)
{
    size_t ivar_size = ALIGNUP8(number*sizeof(uint32_t)+1);
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CFW_EmodSetLteLockFreq) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + 0;
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct IPAR_CFW_EmodSetLteLockFreq *ipar = (struct IPAR_CFW_EmodSetLteLockFreq *)((char*)mem + sizeof(rpcCallHeader_t));
    char *ivar_ptr = (char*)mem + sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CFW_EmodSetLteLockFreq);

    call->h.size = call_size;
    call->api_tag = TAG_CFW_EmodSetLteLockFreq;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    ipar->number = number;
    memcpy(ivar_ptr, freq, number*sizeof(uint32_t));
    ivar_ptr[number*sizeof(uint32_t)] = (char)0;
    ivar_ptr += ALIGNUP8(number*sizeof(uint32_t) + 1);
    ipar->nSimId = nSimId;
    rpcSendCall(RPC_CHANNEL, call);

    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
}

void CFW_EmodGetLteLockFreq(uint8_t *number, uint32_t freq[9], CFW_SIM_ID nSimId)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CFW_EmodGetLteLockFreq) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_CFW_EmodGetLteLockFreq);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct IPAR_CFW_EmodGetLteLockFreq *ipar = (struct IPAR_CFW_EmodGetLteLockFreq *)((char*)mem + sizeof(rpcCallHeader_t));
    struct OPAR_CFW_EmodGetLteLockFreq *opar = (struct OPAR_CFW_EmodGetLteLockFreq *)((char*)mem + sizeof(rpcRespHeader_t));

    call->h.size = call_size;
    call->api_tag = TAG_CFW_EmodGetLteLockFreq;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    memcpy(ipar->freq, freq, sizeof(ipar->freq));
    ipar->nSimId = nSimId;
    rpcSendCall(RPC_CHANNEL, call);

    *number = opar->number;
    memcpy(freq, opar->freq, sizeof(opar->freq));
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
}

void CFW_EmodSetLteLockBand(uint8_t number, uint16_t * band, CFW_SIM_ID nSimId)
{
    size_t ivar_size = ALIGNUP8(number*sizeof(uint16_t)+1);
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CFW_EmodSetLteLockBand) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + 0;
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct IPAR_CFW_EmodSetLteLockBand *ipar = (struct IPAR_CFW_EmodSetLteLockBand *)((char*)mem + sizeof(rpcCallHeader_t));
    char *ivar_ptr = (char*)mem + sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CFW_EmodSetLteLockBand);

    call->h.size = call_size;
    call->api_tag = TAG_CFW_EmodSetLteLockBand;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    ipar->number = number;
    memcpy(ivar_ptr, band, number*sizeof(uint16_t));
    ivar_ptr[number*sizeof(uint16_t)] = (char)0;
    ivar_ptr += ALIGNUP8(number*sizeof(uint16_t) + 1);
    ipar->nSimId = nSimId;
    rpcSendCall(RPC_CHANNEL, call);

    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
}

void CFW_EmodGetLteLockBand(uint8_t *number, uint16_t band[20], CFW_SIM_ID nSimId)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CFW_EmodGetLteLockBand) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_CFW_EmodGetLteLockBand);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct IPAR_CFW_EmodGetLteLockBand *ipar = (struct IPAR_CFW_EmodGetLteLockBand *)((char*)mem + sizeof(rpcCallHeader_t));
    struct OPAR_CFW_EmodGetLteLockBand *opar = (struct OPAR_CFW_EmodGetLteLockBand *)((char*)mem + sizeof(rpcRespHeader_t));

    call->h.size = call_size;
    call->api_tag = TAG_CFW_EmodGetLteLockBand;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    ipar->nSimId = nSimId;
    rpcSendCall(RPC_CHANNEL, call);

    *number = opar->number;
    memcpy(band, opar->band, sizeof(opar->band));
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
}

void CFW_EmodGetCEMode(uint8_t *pMode, CFW_SIM_ID nSimID)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CFW_EmodGetCEMode) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_CFW_EmodGetCEMode);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct IPAR_CFW_EmodGetCEMode *ipar = (struct IPAR_CFW_EmodGetCEMode *)((char*)mem + sizeof(rpcCallHeader_t));
    struct OPAR_CFW_EmodGetCEMode *opar = (struct OPAR_CFW_EmodGetCEMode *)((char*)mem + sizeof(rpcRespHeader_t));

    call->h.size = call_size;
    call->api_tag = TAG_CFW_EmodGetCEMode;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    ipar->nSimID = nSimID;
    rpcSendCall(RPC_CHANNEL, call);

    *pMode = opar->pMode;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
}

void CFW_EmodSetCEMode(uint8_t nMode, CFW_SIM_ID nSimID)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CFW_EmodSetCEMode) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + 0;
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct IPAR_CFW_EmodSetCEMode *ipar = (struct IPAR_CFW_EmodSetCEMode *)((char*)mem + sizeof(rpcCallHeader_t));

    call->h.size = call_size;
    call->api_tag = TAG_CFW_EmodSetCEMode;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    ipar->nMode = nMode;
    ipar->nSimID = nSimID;
    rpcSendCall(RPC_CHANNEL, call);

    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
}

void CFW_EmodGetVoicePrefer(uint8_t *pVoicePrefer, CFW_SIM_ID nSimID)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CFW_EmodGetVoicePrefer) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_CFW_EmodGetVoicePrefer);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct IPAR_CFW_EmodGetVoicePrefer *ipar = (struct IPAR_CFW_EmodGetVoicePrefer *)((char*)mem + sizeof(rpcCallHeader_t));
    struct OPAR_CFW_EmodGetVoicePrefer *opar = (struct OPAR_CFW_EmodGetVoicePrefer *)((char*)mem + sizeof(rpcRespHeader_t));

    call->h.size = call_size;
    call->api_tag = TAG_CFW_EmodGetVoicePrefer;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    ipar->nSimID = nSimID;
    rpcSendCall(RPC_CHANNEL, call);

    *pVoicePrefer = opar->pVoicePrefer;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
}

void CFW_EmodSetVoicePrefer(uint8_t nVoicePrefer, CFW_SIM_ID nSimID)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CFW_EmodSetVoicePrefer) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + 0;
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct IPAR_CFW_EmodSetVoicePrefer *ipar = (struct IPAR_CFW_EmodSetVoicePrefer *)((char*)mem + sizeof(rpcCallHeader_t));

    call->h.size = call_size;
    call->api_tag = TAG_CFW_EmodSetVoicePrefer;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    ipar->nVoicePrefer = nVoicePrefer;
    ipar->nSimID = nSimID;
    rpcSendCall(RPC_CHANNEL, call);

    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
}

#if defined(_SIM_HOT_PLUG_)
void CFW_SetPollingTimer(uint8_t PollingInterval, CFW_SIM_ID nSimID)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CFW_SetPollingTimer) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + 0;
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct IPAR_CFW_SetPollingTimer *ipar = (struct IPAR_CFW_SetPollingTimer *)((char*)mem + sizeof(rpcCallHeader_t));

    call->h.size = call_size;
    call->api_tag = TAG_CFW_SetPollingTimer;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    ipar->PollingInterval = PollingInterval;
    ipar->nSimID = nSimID;
    rpcSendCall(RPC_CHANNEL, call);

    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
}

#endif

#if defined(_SIM_HOT_PLUG_)
void CFW_GetPollingTimer(uint8_t *PollingInterval, CFW_SIM_ID nSimID)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CFW_GetPollingTimer) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_CFW_GetPollingTimer);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct IPAR_CFW_GetPollingTimer *ipar = (struct IPAR_CFW_GetPollingTimer *)((char*)mem + sizeof(rpcCallHeader_t));
    struct OPAR_CFW_GetPollingTimer *opar = (struct OPAR_CFW_GetPollingTimer *)((char*)mem + sizeof(rpcRespHeader_t));

    call->h.size = call_size;
    call->api_tag = TAG_CFW_GetPollingTimer;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    ipar->nSimID = nSimID;
    rpcSendCall(RPC_CHANNEL, call);

    *PollingInterval = opar->PollingInterval;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
}

#endif

uint32_t CSW_SetAudioCodecCalibParam(uint8_t *resultcode, uint8_t *hasMsg, uint8_t resultMsg[900], uint8_t nMode, uint8_t nPath, uint8_t nCtrl, uint8_t * nParam, uint16_t nParamLength)
{
    size_t ivar_size = ALIGNUP8(nParamLength+1);
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CSW_SetAudioCodecCalibParam) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_CSW_SetAudioCodecCalibParam);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct IPAR_CSW_SetAudioCodecCalibParam *ipar = (struct IPAR_CSW_SetAudioCodecCalibParam *)((char*)mem + sizeof(rpcCallHeader_t));
    struct OPAR_CSW_SetAudioCodecCalibParam *opar = (struct OPAR_CSW_SetAudioCodecCalibParam *)((char*)mem + sizeof(rpcRespHeader_t));
    char *ivar_ptr = (char*)mem + sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CSW_SetAudioCodecCalibParam);
    uint32_t result;

    call->h.size = call_size;
    call->api_tag = TAG_CSW_SetAudioCodecCalibParam;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    ipar->nMode = nMode;
    ipar->nPath = nPath;
    ipar->nCtrl = nCtrl;
    memcpy(ivar_ptr, nParam, nParamLength);
    ivar_ptr[nParamLength] = (char)0;
    ivar_ptr += ALIGNUP8(nParamLength + 1);
    ipar->nParamLength = nParamLength;
    rpcSendCall(RPC_CHANNEL, call);

    *resultcode = opar->resultcode;
    *hasMsg = opar->hasMsg;
    memcpy(resultMsg, opar->resultMsg, sizeof(opar->resultMsg));
    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

uint32_t CSW_SetAudioZspVqeCalibParam(uint8_t *resultcode, uint8_t *hasMsg, uint8_t resultMsg[900], uint8_t nMode, uint8_t nPath, uint8_t nCtrl, uint8_t * nParam, uint16_t nParamLength)
{
    size_t ivar_size = ALIGNUP8(nParamLength+1);
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CSW_SetAudioZspVqeCalibParam) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_CSW_SetAudioZspVqeCalibParam);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct IPAR_CSW_SetAudioZspVqeCalibParam *ipar = (struct IPAR_CSW_SetAudioZspVqeCalibParam *)((char*)mem + sizeof(rpcCallHeader_t));
    struct OPAR_CSW_SetAudioZspVqeCalibParam *opar = (struct OPAR_CSW_SetAudioZspVqeCalibParam *)((char*)mem + sizeof(rpcRespHeader_t));
    char *ivar_ptr = (char*)mem + sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CSW_SetAudioZspVqeCalibParam);
    uint32_t result;

    call->h.size = call_size;
    call->api_tag = TAG_CSW_SetAudioZspVqeCalibParam;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    ipar->nMode = nMode;
    ipar->nPath = nPath;
    ipar->nCtrl = nCtrl;
    memcpy(ivar_ptr, nParam, nParamLength);
    ivar_ptr[nParamLength] = (char)0;
    ivar_ptr += ALIGNUP8(nParamLength + 1);
    ipar->nParamLength = nParamLength;
    rpcSendCall(RPC_CHANNEL, call);

    *resultcode = opar->resultcode;
    *hasMsg = opar->hasMsg;
    memcpy(resultMsg, opar->resultMsg, sizeof(opar->resultMsg));
    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

uint32_t CSW_SetAudioZspPreProcessCalibParam(uint8_t *resultcode, uint8_t *hasMsg, uint8_t resultMsg[900], uint8_t nMode, uint8_t nPath, uint8_t nCtrl, uint8_t * nParam, uint16_t nParamLength)
{
    size_t ivar_size = ALIGNUP8(nParamLength+1);
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CSW_SetAudioZspPreProcessCalibParam) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_CSW_SetAudioZspPreProcessCalibParam);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct IPAR_CSW_SetAudioZspPreProcessCalibParam *ipar = (struct IPAR_CSW_SetAudioZspPreProcessCalibParam *)((char*)mem + sizeof(rpcCallHeader_t));
    struct OPAR_CSW_SetAudioZspPreProcessCalibParam *opar = (struct OPAR_CSW_SetAudioZspPreProcessCalibParam *)((char*)mem + sizeof(rpcRespHeader_t));
    char *ivar_ptr = (char*)mem + sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CSW_SetAudioZspPreProcessCalibParam);
    uint32_t result;

    call->h.size = call_size;
    call->api_tag = TAG_CSW_SetAudioZspPreProcessCalibParam;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    ipar->nMode = nMode;
    ipar->nPath = nPath;
    ipar->nCtrl = nCtrl;
    memcpy(ivar_ptr, nParam, nParamLength);
    ivar_ptr[nParamLength] = (char)0;
    ivar_ptr += ALIGNUP8(nParamLength + 1);
    ipar->nParamLength = nParamLength;
    rpcSendCall(RPC_CHANNEL, call);

    *resultcode = opar->resultcode;
    *hasMsg = opar->hasMsg;
    memcpy(resultMsg, opar->resultMsg, sizeof(opar->resultMsg));
    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

uint32_t CSW_SetAudioZspPostProcessCalibParam(uint8_t *resultcode, uint8_t *hasMsg, uint8_t resultMsg[900], uint8_t nMode, uint8_t nPath, uint8_t nCtrl, uint8_t * nParam, uint16_t nParamLength)
{
    size_t ivar_size = ALIGNUP8(nParamLength+1);
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CSW_SetAudioZspPostProcessCalibParam) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_CSW_SetAudioZspPostProcessCalibParam);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct IPAR_CSW_SetAudioZspPostProcessCalibParam *ipar = (struct IPAR_CSW_SetAudioZspPostProcessCalibParam *)((char*)mem + sizeof(rpcCallHeader_t));
    struct OPAR_CSW_SetAudioZspPostProcessCalibParam *opar = (struct OPAR_CSW_SetAudioZspPostProcessCalibParam *)((char*)mem + sizeof(rpcRespHeader_t));
    char *ivar_ptr = (char*)mem + sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CSW_SetAudioZspPostProcessCalibParam);
    uint32_t result;

    call->h.size = call_size;
    call->api_tag = TAG_CSW_SetAudioZspPostProcessCalibParam;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    ipar->nMode = nMode;
    ipar->nPath = nPath;
    ipar->nCtrl = nCtrl;
    memcpy(ivar_ptr, nParam, nParamLength);
    ivar_ptr[nParamLength] = (char)0;
    ivar_ptr += ALIGNUP8(nParamLength + 1);
    ipar->nParamLength = nParamLength;
    rpcSendCall(RPC_CHANNEL, call);

    *resultcode = opar->resultcode;
    *hasMsg = opar->hasMsg;
    memcpy(resultMsg, opar->resultMsg, sizeof(opar->resultMsg));
    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

uint32_t CSW_WriteCalibParamToFlash(uint8_t *resultcode, uint8_t *hasMsg, uint8_t resultMsg[900], uint8_t nPath, uint8_t nCtrl, uint8_t * nParam, uint16_t nParamLength)
{
    size_t ivar_size = ALIGNUP8(nParamLength+1);
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CSW_WriteCalibParamToFlash) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_CSW_WriteCalibParamToFlash);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct IPAR_CSW_WriteCalibParamToFlash *ipar = (struct IPAR_CSW_WriteCalibParamToFlash *)((char*)mem + sizeof(rpcCallHeader_t));
    struct OPAR_CSW_WriteCalibParamToFlash *opar = (struct OPAR_CSW_WriteCalibParamToFlash *)((char*)mem + sizeof(rpcRespHeader_t));
    char *ivar_ptr = (char*)mem + sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CSW_WriteCalibParamToFlash);
    uint32_t result;

    call->h.size = call_size;
    call->api_tag = TAG_CSW_WriteCalibParamToFlash;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    ipar->nPath = nPath;
    ipar->nCtrl = nCtrl;
    memcpy(ivar_ptr, nParam, nParamLength);
    ivar_ptr[nParamLength] = (char)0;
    ivar_ptr += ALIGNUP8(nParamLength + 1);
    ipar->nParamLength = nParamLength;
    rpcSendCall(RPC_CHANNEL, call);

    *resultcode = opar->resultcode;
    *hasMsg = opar->hasMsg;
    memcpy(resultMsg, opar->resultMsg, sizeof(opar->resultMsg));
    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

uint32_t CSW_ExportCalibOrImportCalibFlashParam(uint8_t *resultcode, uint8_t *hasMsg, uint8_t resultMsg[900], uint8_t nPath, uint8_t nCtrl, uint16_t nOffset, uint16_t nlength, uint8_t * nParam, uint16_t nParamLength)
{
    size_t ivar_size = ALIGNUP8(nParamLength+1);
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CSW_ExportCalibOrImportCalibFlashParam) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_CSW_ExportCalibOrImportCalibFlashParam);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct IPAR_CSW_ExportCalibOrImportCalibFlashParam *ipar = (struct IPAR_CSW_ExportCalibOrImportCalibFlashParam *)((char*)mem + sizeof(rpcCallHeader_t));
    struct OPAR_CSW_ExportCalibOrImportCalibFlashParam *opar = (struct OPAR_CSW_ExportCalibOrImportCalibFlashParam *)((char*)mem + sizeof(rpcRespHeader_t));
    char *ivar_ptr = (char*)mem + sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CSW_ExportCalibOrImportCalibFlashParam);
    uint32_t result;

    call->h.size = call_size;
    call->api_tag = TAG_CSW_ExportCalibOrImportCalibFlashParam;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    ipar->nPath = nPath;
    ipar->nCtrl = nCtrl;
    ipar->nOffset = nOffset;
    ipar->nlength = nlength;
    memcpy(ivar_ptr, nParam, nParamLength);
    ivar_ptr[nParamLength] = (char)0;
    ivar_ptr += ALIGNUP8(nParamLength + 1);
    ipar->nParamLength = nParamLength;
    rpcSendCall(RPC_CHANNEL, call);

    *resultcode = opar->resultcode;
    *hasMsg = opar->hasMsg;
    memcpy(resultMsg, opar->resultMsg, sizeof(opar->resultMsg));
    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

uint32_t CSW_DumpPcmDataToTflash(uint8_t *resultcode, uint8_t *hasMsg, uint8_t resultMsg[900], uint8_t nPath, uint8_t nCtrl, uint8_t * nParam, uint16_t nParamLength)
{
    size_t ivar_size = ALIGNUP8(nParamLength+1);
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CSW_DumpPcmDataToTflash) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_CSW_DumpPcmDataToTflash);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct IPAR_CSW_DumpPcmDataToTflash *ipar = (struct IPAR_CSW_DumpPcmDataToTflash *)((char*)mem + sizeof(rpcCallHeader_t));
    struct OPAR_CSW_DumpPcmDataToTflash *opar = (struct OPAR_CSW_DumpPcmDataToTflash *)((char*)mem + sizeof(rpcRespHeader_t));
    char *ivar_ptr = (char*)mem + sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CSW_DumpPcmDataToTflash);
    uint32_t result;

    call->h.size = call_size;
    call->api_tag = TAG_CSW_DumpPcmDataToTflash;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    ipar->nPath = nPath;
    ipar->nCtrl = nCtrl;
    memcpy(ivar_ptr, nParam, nParamLength);
    ivar_ptr[nParamLength] = (char)0;
    ivar_ptr += ALIGNUP8(nParamLength + 1);
    ipar->nParamLength = nParamLength;
    rpcSendCall(RPC_CHANNEL, call);

    *resultcode = opar->resultcode;
    *hasMsg = opar->hasMsg;
    memcpy(resultMsg, opar->resultMsg, sizeof(opar->resultMsg));
    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

uint32_t CSW_VersionControl(uint8_t *resultcode, uint8_t *hasMsg, uint8_t resultMsg[900], uint8_t nPath, uint8_t nCtrl, uint8_t * nParam, uint16_t nParamLength)
{
    size_t ivar_size = ALIGNUP8(nParamLength+1);
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CSW_VersionControl) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_CSW_VersionControl);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct IPAR_CSW_VersionControl *ipar = (struct IPAR_CSW_VersionControl *)((char*)mem + sizeof(rpcCallHeader_t));
    struct OPAR_CSW_VersionControl *opar = (struct OPAR_CSW_VersionControl *)((char*)mem + sizeof(rpcRespHeader_t));
    char *ivar_ptr = (char*)mem + sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CSW_VersionControl);
    uint32_t result;

    call->h.size = call_size;
    call->api_tag = TAG_CSW_VersionControl;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    ipar->nPath = nPath;
    ipar->nCtrl = nCtrl;
    memcpy(ivar_ptr, nParam, nParamLength);
    ivar_ptr[nParamLength] = (char)0;
    ivar_ptr += ALIGNUP8(nParamLength + 1);
    ipar->nParamLength = nParamLength;
    rpcSendCall(RPC_CHANNEL, call);

    *resultcode = opar->resultcode;
    *hasMsg = opar->hasMsg;
    memcpy(resultMsg, opar->resultMsg, sizeof(opar->resultMsg));
    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

uint32_t CSW_SetAudioZspNxpCalibParam(uint8_t *resultcode, uint8_t *hasMsg, uint8_t resultMsg[900], uint8_t nMode, uint8_t nPath, uint8_t nCtrl, uint8_t * nParam, uint16_t nParamLength)
{
    size_t ivar_size = ALIGNUP8(nParamLength+1);
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CSW_SetAudioZspNxpCalibParam) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_CSW_SetAudioZspNxpCalibParam);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct IPAR_CSW_SetAudioZspNxpCalibParam *ipar = (struct IPAR_CSW_SetAudioZspNxpCalibParam *)((char*)mem + sizeof(rpcCallHeader_t));
    struct OPAR_CSW_SetAudioZspNxpCalibParam *opar = (struct OPAR_CSW_SetAudioZspNxpCalibParam *)((char*)mem + sizeof(rpcRespHeader_t));
    char *ivar_ptr = (char*)mem + sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CSW_SetAudioZspNxpCalibParam);
    uint32_t result;

    call->h.size = call_size;
    call->api_tag = TAG_CSW_SetAudioZspNxpCalibParam;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    ipar->nMode = nMode;
    ipar->nPath = nPath;
    ipar->nCtrl = nCtrl;
    memcpy(ivar_ptr, nParam, nParamLength);
    ivar_ptr[nParamLength] = (char)0;
    ivar_ptr += ALIGNUP8(nParamLength + 1);
    ipar->nParamLength = nParamLength;
    rpcSendCall(RPC_CHANNEL, call);

    *resultcode = opar->resultcode;
    *hasMsg = opar->hasMsg;
    memcpy(resultMsg, opar->resultMsg, sizeof(opar->resultMsg));
    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

uint32_t CFW_NwSetFTA(uint8_t nFta, CFW_SIM_ID nSimID)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CFW_NwSetFTA) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_CFW_NwSetFTA);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct IPAR_CFW_NwSetFTA *ipar = (struct IPAR_CFW_NwSetFTA *)((char*)mem + sizeof(rpcCallHeader_t));
    struct OPAR_CFW_NwSetFTA *opar = (struct OPAR_CFW_NwSetFTA *)((char*)mem + sizeof(rpcRespHeader_t));
    uint32_t result;

    call->h.size = call_size;
    call->api_tag = TAG_CFW_NwSetFTA;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    ipar->nFta = nFta;
    ipar->nSimID = nSimID;
    rpcSendCall(RPC_CHANNEL, call);

    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

uint32_t CFW_NwSetnSecurityUsedFlag(uint8_t nSecurityUsedFlag, CFW_SIM_ID nSimID)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CFW_NwSetnSecurityUsedFlag) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_CFW_NwSetnSecurityUsedFlag);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct IPAR_CFW_NwSetnSecurityUsedFlag *ipar = (struct IPAR_CFW_NwSetnSecurityUsedFlag *)((char*)mem + sizeof(rpcCallHeader_t));
    struct OPAR_CFW_NwSetnSecurityUsedFlag *opar = (struct OPAR_CFW_NwSetnSecurityUsedFlag *)((char*)mem + sizeof(rpcRespHeader_t));
    uint32_t result;

    call->h.size = call_size;
    call->api_tag = TAG_CFW_NwSetnSecurityUsedFlag;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    ipar->nSecurityUsedFlag = nSecurityUsedFlag;
    ipar->nSimID = nSimID;
    rpcSendCall(RPC_CHANNEL, call);

    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

uint32_t CFW_GprsIpDnsInfo(uint8_t nCid, CFW_SIM_ID nSimID)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CFW_GprsIpDnsInfo) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_CFW_GprsIpDnsInfo);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct IPAR_CFW_GprsIpDnsInfo *ipar = (struct IPAR_CFW_GprsIpDnsInfo *)((char*)mem + sizeof(rpcCallHeader_t));
    struct OPAR_CFW_GprsIpDnsInfo *opar = (struct OPAR_CFW_GprsIpDnsInfo *)((char*)mem + sizeof(rpcRespHeader_t));
    uint32_t result;

    call->h.size = call_size;
    call->api_tag = TAG_CFW_GprsIpDnsInfo;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    ipar->nCid = nCid;
    ipar->nSimID = nSimID;
    rpcSendCall(RPC_CHANNEL, call);

    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

uint32_t CFW_GprsClrIpDns(void)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + 0 + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_CFW_GprsClrIpDns);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct OPAR_CFW_GprsClrIpDns *opar = (struct OPAR_CFW_GprsClrIpDns *)((char*)mem + sizeof(rpcRespHeader_t));
    uint32_t result;

    call->h.size = call_size;
    call->api_tag = TAG_CFW_GprsClrIpDns;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    rpcSendCall(RPC_CHANNEL, call);

    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

void hal_WriteGsmCalib(void)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + 0 + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + 0;
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;

    call->h.size = call_size;
    call->api_tag = TAG_hal_WriteGsmCalib;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    rpcSendCall(RPC_CHANNEL, call);

    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
}

void hal_WriteLteCalib(void)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + 0 + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + 0;
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;

    call->h.size = call_size;
    call->api_tag = TAG_hal_WriteLteCalib;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    rpcSendCall(RPC_CHANNEL, call);

    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
}

uint32_t CFW_CcCallMultipartyFunction(uint8_t nFun, CFW_SIM_ID nSimID)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CFW_CcCallMultipartyFunction) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_CFW_CcCallMultipartyFunction);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct IPAR_CFW_CcCallMultipartyFunction *ipar = (struct IPAR_CFW_CcCallMultipartyFunction *)((char*)mem + sizeof(rpcCallHeader_t));
    struct OPAR_CFW_CcCallMultipartyFunction *opar = (struct OPAR_CFW_CcCallMultipartyFunction *)((char*)mem + sizeof(rpcRespHeader_t));
    uint32_t result;

    call->h.size = call_size;
    call->api_tag = TAG_CFW_CcCallMultipartyFunction;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    ipar->nFun = nFun;
    ipar->nSimID = nSimID;
    rpcSendCall(RPC_CHANNEL, call);

    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

uint32_t CFW_SimReadVoiceMailInfo(uint8_t nLineId, uint8_t nType, uint16_t nUTI, CFW_SIM_ID nSimID)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CFW_SimReadVoiceMailInfo) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_CFW_SimReadVoiceMailInfo);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct IPAR_CFW_SimReadVoiceMailInfo *ipar = (struct IPAR_CFW_SimReadVoiceMailInfo *)((char*)mem + sizeof(rpcCallHeader_t));
    struct OPAR_CFW_SimReadVoiceMailInfo *opar = (struct OPAR_CFW_SimReadVoiceMailInfo *)((char*)mem + sizeof(rpcRespHeader_t));
    uint32_t result;

    call->h.size = call_size;
    call->api_tag = TAG_CFW_SimReadVoiceMailInfo;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    ipar->nLineId = nLineId;
    ipar->nType = nType;
    ipar->nUTI = nUTI;
    ipar->nSimID = nSimID;
    rpcSendCall(RPC_CHANNEL, call);

    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

uint32_t CFW_SimUpdateVoiceMailInfo(uint8_t nLineId, uint8_t nType, CFW_SIM_INFO_VOICEMAIL *pVoiceMail, uint16_t nUTI, CFW_SIM_ID nSimID)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CFW_SimUpdateVoiceMailInfo) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_CFW_SimUpdateVoiceMailInfo);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct IPAR_CFW_SimUpdateVoiceMailInfo *ipar = (struct IPAR_CFW_SimUpdateVoiceMailInfo *)((char*)mem + sizeof(rpcCallHeader_t));
    struct OPAR_CFW_SimUpdateVoiceMailInfo *opar = (struct OPAR_CFW_SimUpdateVoiceMailInfo *)((char*)mem + sizeof(rpcRespHeader_t));
    uint32_t result;

    call->h.size = call_size;
    call->api_tag = TAG_CFW_SimUpdateVoiceMailInfo;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    ipar->nLineId = nLineId;
    ipar->nType = nType;
    ipar->pVoiceMail = *pVoiceMail;
    ipar->nUTI = nUTI;
    ipar->nSimID = nSimID;
    rpcSendCall(RPC_CHANNEL, call);

    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

uint32_t CFW_CbSetContext(uint8_t CtxData[sizeof(CB_CTX_T)], CFW_SIM_ID nSimID)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CFW_CbSetContext) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_CFW_CbSetContext);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct IPAR_CFW_CbSetContext *ipar = (struct IPAR_CFW_CbSetContext *)((char*)mem + sizeof(rpcCallHeader_t));
    struct OPAR_CFW_CbSetContext *opar = (struct OPAR_CFW_CbSetContext *)((char*)mem + sizeof(rpcRespHeader_t));
    uint32_t result;

    call->h.size = call_size;
    call->api_tag = TAG_CFW_CbSetContext;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    memcpy(ipar->CtxData, CtxData, sizeof(ipar->CtxData));
    ipar->nSimID = nSimID;
    rpcSendCall(RPC_CHANNEL, call);

    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

uint32_t CFW_CbSetType(uint8_t nCbType, CFW_SIM_ID nSimID)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CFW_CbSetType) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_CFW_CbSetType);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct IPAR_CFW_CbSetType *ipar = (struct IPAR_CFW_CbSetType *)((char*)mem + sizeof(rpcCallHeader_t));
    struct OPAR_CFW_CbSetType *opar = (struct OPAR_CFW_CbSetType *)((char*)mem + sizeof(rpcRespHeader_t));
    uint32_t result;

    call->h.size = call_size;
    call->api_tag = TAG_CFW_CbSetType;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    ipar->nCbType = nCbType;
    ipar->nSimID = nSimID;
    rpcSendCall(RPC_CHANNEL, call);

    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

uint32_t CFW_CbAct(uint16_t nUTI, CFW_SIM_ID nSimID)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CFW_CbAct) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_CFW_CbAct);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct IPAR_CFW_CbAct *ipar = (struct IPAR_CFW_CbAct *)((char*)mem + sizeof(rpcCallHeader_t));
    struct OPAR_CFW_CbAct *opar = (struct OPAR_CFW_CbAct *)((char*)mem + sizeof(rpcRespHeader_t));
    uint32_t result;

    call->h.size = call_size;
    call->api_tag = TAG_CFW_CbAct;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    ipar->nUTI = nUTI;
    ipar->nSimID = nSimID;
    rpcSendCall(RPC_CHANNEL, call);

    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

uint32_t CFW_CbDeAct(uint16_t nUTI, CFW_SIM_ID nSimID)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CFW_CbDeAct) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_CFW_CbDeAct);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct IPAR_CFW_CbDeAct *ipar = (struct IPAR_CFW_CbDeAct *)((char*)mem + sizeof(rpcCallHeader_t));
    struct OPAR_CFW_CbDeAct *opar = (struct OPAR_CFW_CbDeAct *)((char*)mem + sizeof(rpcRespHeader_t));
    uint32_t result;

    call->h.size = call_size;
    call->api_tag = TAG_CFW_CbDeAct;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    ipar->nUTI = nUTI;
    ipar->nSimID = nSimID;
    rpcSendCall(RPC_CHANNEL, call);

    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

uint32_t CFW_SetPowerOffFlag(uint8_t bOff, CFW_SIM_ID nSimID)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CFW_SetPowerOffFlag) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_CFW_SetPowerOffFlag);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct IPAR_CFW_SetPowerOffFlag *ipar = (struct IPAR_CFW_SetPowerOffFlag *)((char*)mem + sizeof(rpcCallHeader_t));
    struct OPAR_CFW_SetPowerOffFlag *opar = (struct OPAR_CFW_SetPowerOffFlag *)((char*)mem + sizeof(rpcRespHeader_t));
    uint32_t result;

    call->h.size = call_size;
    call->api_tag = TAG_CFW_SetPowerOffFlag;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    ipar->bOff = bOff;
    ipar->nSimID = nSimID;
    rpcSendCall(RPC_CHANNEL, call);

    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

uint32_t CFW_NwSysConfig(uint8_t nMode, uint8_t nAcqorder, uint8_t nRoam, uint8_t nSrvdomain, CFW_SIM_ID nSimID)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CFW_NwSysConfig) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_CFW_NwSysConfig);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct IPAR_CFW_NwSysConfig *ipar = (struct IPAR_CFW_NwSysConfig *)((char*)mem + sizeof(rpcCallHeader_t));
    struct OPAR_CFW_NwSysConfig *opar = (struct OPAR_CFW_NwSysConfig *)((char*)mem + sizeof(rpcRespHeader_t));
    uint32_t result;

    call->h.size = call_size;
    call->api_tag = TAG_CFW_NwSysConfig;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    ipar->nMode = nMode;
    ipar->nAcqorder = nAcqorder;
    ipar->nRoam = nRoam;
    ipar->nSrvdomain = nSrvdomain;
    ipar->nSimID = nSimID;
    rpcSendCall(RPC_CHANNEL, call);

    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

uint32_t CFW_NwGetSysConfig(uint8_t *pMode, uint8_t *pAcqorder, uint8_t *pRoam, uint8_t *pSrvdomain, CFW_SIM_ID nSimID)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CFW_NwGetSysConfig) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_CFW_NwGetSysConfig);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct IPAR_CFW_NwGetSysConfig *ipar = (struct IPAR_CFW_NwGetSysConfig *)((char*)mem + sizeof(rpcCallHeader_t));
    struct OPAR_CFW_NwGetSysConfig *opar = (struct OPAR_CFW_NwGetSysConfig *)((char*)mem + sizeof(rpcRespHeader_t));
    uint32_t result;

    call->h.size = call_size;
    call->api_tag = TAG_CFW_NwGetSysConfig;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    ipar->nSimID = nSimID;
    rpcSendCall(RPC_CHANNEL, call);

    *pMode = opar->pMode;
    *pAcqorder = opar->pAcqorder;
    *pRoam = opar->pRoam;
    *pSrvdomain = opar->pSrvdomain;
    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

uint32_t CFW_SimGetSmsParameters(uint8_t nIndex, uint16_t nUTI, CFW_SIM_ID nSimID)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CFW_SimGetSmsParameters) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_CFW_SimGetSmsParameters);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct IPAR_CFW_SimGetSmsParameters *ipar = (struct IPAR_CFW_SimGetSmsParameters *)((char*)mem + sizeof(rpcCallHeader_t));
    struct OPAR_CFW_SimGetSmsParameters *opar = (struct OPAR_CFW_SimGetSmsParameters *)((char*)mem + sizeof(rpcRespHeader_t));
    uint32_t result;

    call->h.size = call_size;
    call->api_tag = TAG_CFW_SimGetSmsParameters;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    ipar->nIndex = nIndex;
    ipar->nUTI = nUTI;
    ipar->nSimID = nSimID;
    rpcSendCall(RPC_CHANNEL, call);

    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

uint32_t CFW_SimSetSmsParameters(CFW_SIM_SMS_PARAMETERS *pSmsParamArray, uint8_t nArrayCount, uint8_t nIndex, uint16_t nUTI, CFW_SIM_ID nSimID)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CFW_SimSetSmsParameters) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_CFW_SimSetSmsParameters);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct IPAR_CFW_SimSetSmsParameters *ipar = (struct IPAR_CFW_SimSetSmsParameters *)((char*)mem + sizeof(rpcCallHeader_t));
    struct OPAR_CFW_SimSetSmsParameters *opar = (struct OPAR_CFW_SimSetSmsParameters *)((char*)mem + sizeof(rpcRespHeader_t));
    uint32_t result;

    call->h.size = call_size;
    call->api_tag = TAG_CFW_SimSetSmsParameters;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    ipar->pSmsParamArray = *pSmsParamArray;
    ipar->nArrayCount = nArrayCount;
    ipar->nIndex = nIndex;
    ipar->nUTI = nUTI;
    ipar->nSimID = nSimID;
    rpcSendCall(RPC_CHANNEL, call);

    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

uint32_t CFW_SimDeleteMessage(uint16_t nLocation, uint16_t nIndex, uint16_t nStatus, uint16_t nUTI, CFW_SIM_ID nSimID)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CFW_SimDeleteMessage) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_CFW_SimDeleteMessage);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct IPAR_CFW_SimDeleteMessage *ipar = (struct IPAR_CFW_SimDeleteMessage *)((char*)mem + sizeof(rpcCallHeader_t));
    struct OPAR_CFW_SimDeleteMessage *opar = (struct OPAR_CFW_SimDeleteMessage *)((char*)mem + sizeof(rpcRespHeader_t));
    uint32_t result;

    call->h.size = call_size;
    call->api_tag = TAG_CFW_SimDeleteMessage;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    ipar->nLocation = nLocation;
    ipar->nIndex = nIndex;
    ipar->nStatus = nStatus;
    ipar->nUTI = nUTI;
    ipar->nSimID = nSimID;
    rpcSendCall(RPC_CHANNEL, call);

    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

uint32_t CFW_EmodSpLteDummyPara(CFW_EmodSpLteDummyPara_t para, CFW_SIM_ID nSimID)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CFW_EmodSpLteDummyPara) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_CFW_EmodSpLteDummyPara);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct IPAR_CFW_EmodSpLteDummyPara *ipar = (struct IPAR_CFW_EmodSpLteDummyPara *)((char*)mem + sizeof(rpcCallHeader_t));
    struct OPAR_CFW_EmodSpLteDummyPara *opar = (struct OPAR_CFW_EmodSpLteDummyPara *)((char*)mem + sizeof(rpcRespHeader_t));
    uint32_t result;

    call->h.size = call_size;
    call->api_tag = TAG_CFW_EmodSpLteDummyPara;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    ipar->para = para;
    ipar->nSimID = nSimID;
    rpcSendCall(RPC_CHANNEL, call);

    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

uint32_t CFW_EmodRrtmParam(CFW_EmodRrtmParam_t para, CFW_SIM_ID nSimID)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CFW_EmodRrtmParam) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_CFW_EmodRrtmParam);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct IPAR_CFW_EmodRrtmParam *ipar = (struct IPAR_CFW_EmodRrtmParam *)((char*)mem + sizeof(rpcCallHeader_t));
    struct OPAR_CFW_EmodRrtmParam *opar = (struct OPAR_CFW_EmodRrtmParam *)((char*)mem + sizeof(rpcRespHeader_t));
    uint32_t result;

    call->h.size = call_size;
    call->api_tag = TAG_CFW_EmodRrtmParam;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    ipar->para = para;
    ipar->nSimID = nSimID;
    rpcSendCall(RPC_CHANNEL, call);

    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

uint32_t CFW_EmodGrrLteFreq(CFW_EmodGrrLteFreq_t para, CFW_SIM_ID nSimID)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CFW_EmodGrrLteFreq) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_CFW_EmodGrrLteFreq);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct IPAR_CFW_EmodGrrLteFreq *ipar = (struct IPAR_CFW_EmodGrrLteFreq *)((char*)mem + sizeof(rpcCallHeader_t));
    struct OPAR_CFW_EmodGrrLteFreq *opar = (struct OPAR_CFW_EmodGrrLteFreq *)((char*)mem + sizeof(rpcRespHeader_t));
    uint32_t result;

    call->h.size = call_size;
    call->api_tag = TAG_CFW_EmodGrrLteFreq;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    ipar->para = para;
    ipar->nSimID = nSimID;
    rpcSendCall(RPC_CHANNEL, call);

    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

uint32_t CFW_EmodRrdmParam(CFW_EmodRrdmParam_t para, CFW_SIM_ID nSimID)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CFW_EmodRrdmParam) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_CFW_EmodRrdmParam);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct IPAR_CFW_EmodRrdmParam *ipar = (struct IPAR_CFW_EmodRrdmParam *)((char*)mem + sizeof(rpcCallHeader_t));
    struct OPAR_CFW_EmodRrdmParam *opar = (struct OPAR_CFW_EmodRrdmParam *)((char*)mem + sizeof(rpcRespHeader_t));
    uint32_t result;

    call->h.size = call_size;
    call->api_tag = TAG_CFW_EmodRrdmParam;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    ipar->para = para;
    ipar->nSimID = nSimID;
    rpcSendCall(RPC_CHANNEL, call);

    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

uint32_t CFW_EmodL1Param(CFW_EmodL1Param_t para, CFW_SIM_ID nSimID)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CFW_EmodL1Param) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_CFW_EmodL1Param);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct IPAR_CFW_EmodL1Param *ipar = (struct IPAR_CFW_EmodL1Param *)((char*)mem + sizeof(rpcCallHeader_t));
    struct OPAR_CFW_EmodL1Param *opar = (struct OPAR_CFW_EmodL1Param *)((char*)mem + sizeof(rpcRespHeader_t));
    uint32_t result;

    call->h.size = call_size;
    call->api_tag = TAG_CFW_EmodL1Param;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    ipar->para = para;
    ipar->nSimID = nSimID;
    rpcSendCall(RPC_CHANNEL, call);

    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

uint32_t CFW_EmodSpCleanInfo(CFW_EmodSpCleanInfo_t para, CFW_SIM_ID nSimID)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CFW_EmodSpCleanInfo) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_CFW_EmodSpCleanInfo);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct IPAR_CFW_EmodSpCleanInfo *ipar = (struct IPAR_CFW_EmodSpCleanInfo *)((char*)mem + sizeof(rpcCallHeader_t));
    struct OPAR_CFW_EmodSpCleanInfo *opar = (struct OPAR_CFW_EmodSpCleanInfo *)((char*)mem + sizeof(rpcRespHeader_t));
    uint32_t result;

    call->h.size = call_size;
    call->api_tag = TAG_CFW_EmodSpCleanInfo;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    ipar->para = para;
    ipar->nSimID = nSimID;
    rpcSendCall(RPC_CHANNEL, call);

    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

uint32_t CFW_EmodGetGsmCalibInfo(void)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + 0 + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_CFW_EmodGetGsmCalibInfo);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct OPAR_CFW_EmodGetGsmCalibInfo *opar = (struct OPAR_CFW_EmodGetGsmCalibInfo *)((char*)mem + sizeof(rpcRespHeader_t));
    uint32_t result;

    call->h.size = call_size;
    call->api_tag = TAG_CFW_EmodGetGsmCalibInfo;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    rpcSendCall(RPC_CHANNEL, call);

    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

uint32_t CFW_EmodGetLteCalibInfo(void)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + 0 + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_CFW_EmodGetLteCalibInfo);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct OPAR_CFW_EmodGetLteCalibInfo *opar = (struct OPAR_CFW_EmodGetLteCalibInfo *)((char*)mem + sizeof(rpcRespHeader_t));
    uint32_t result;

    call->h.size = call_size;
    call->api_tag = TAG_CFW_EmodGetLteCalibInfo;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    rpcSendCall(RPC_CHANNEL, call);

    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

uint32_t CFW_NwSetXCAP(uint8_t * pXCAP, uint32_t nLength, CFW_SIM_ID nSimID)
{
    size_t ivar_size = ALIGNUP8(nLength+1);
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CFW_NwSetXCAP) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_CFW_NwSetXCAP);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct IPAR_CFW_NwSetXCAP *ipar = (struct IPAR_CFW_NwSetXCAP *)((char*)mem + sizeof(rpcCallHeader_t));
    struct OPAR_CFW_NwSetXCAP *opar = (struct OPAR_CFW_NwSetXCAP *)((char*)mem + sizeof(rpcRespHeader_t));
    char *ivar_ptr = (char*)mem + sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CFW_NwSetXCAP);
    uint32_t result;

    call->h.size = call_size;
    call->api_tag = TAG_CFW_NwSetXCAP;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    memcpy(ivar_ptr, pXCAP, nLength);
    ivar_ptr[nLength] = (char)0;
    ivar_ptr += ALIGNUP8(nLength + 1);
    ipar->nLength = nLength;
    ipar->nSimID = nSimID;
    rpcSendCall(RPC_CHANNEL, call);

    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

uint32_t CFW_NwSetBSF(uint8_t * pBSF, uint32_t nLength, CFW_SIM_ID nSimID)
{
    size_t ivar_size = ALIGNUP8(nLength+1);
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CFW_NwSetBSF) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_CFW_NwSetBSF);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct IPAR_CFW_NwSetBSF *ipar = (struct IPAR_CFW_NwSetBSF *)((char*)mem + sizeof(rpcCallHeader_t));
    struct OPAR_CFW_NwSetBSF *opar = (struct OPAR_CFW_NwSetBSF *)((char*)mem + sizeof(rpcRespHeader_t));
    char *ivar_ptr = (char*)mem + sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CFW_NwSetBSF);
    uint32_t result;

    call->h.size = call_size;
    call->api_tag = TAG_CFW_NwSetBSF;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    memcpy(ivar_ptr, pBSF, nLength);
    ivar_ptr[nLength] = (char)0;
    ivar_ptr += ALIGNUP8(nLength + 1);
    ipar->nLength = nLength;
    ipar->nSimID = nSimID;
    rpcSendCall(RPC_CHANNEL, call);

    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

void CFW_EmodSetTracePort(uint8_t port)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CFW_EmodSetTracePort) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + 0;
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct IPAR_CFW_EmodSetTracePort *ipar = (struct IPAR_CFW_EmodSetTracePort *)((char*)mem + sizeof(rpcCallHeader_t));

    call->h.size = call_size;
    call->api_tag = TAG_CFW_EmodSetTracePort;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    ipar->port = port;
    rpcSendCall(RPC_CHANNEL, call);

    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
}

uint32_t CFW_ImsSrvccCapSet(uint8_t bSupport, CFW_SIM_ID nSimID)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CFW_ImsSrvccCapSet) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_CFW_ImsSrvccCapSet);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct IPAR_CFW_ImsSrvccCapSet *ipar = (struct IPAR_CFW_ImsSrvccCapSet *)((char*)mem + sizeof(rpcCallHeader_t));
    struct OPAR_CFW_ImsSrvccCapSet *opar = (struct OPAR_CFW_ImsSrvccCapSet *)((char*)mem + sizeof(rpcRespHeader_t));
    uint32_t result;

    call->h.size = call_size;
    call->api_tag = TAG_CFW_ImsSrvccCapSet;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    ipar->bSupport = bSupport;
    ipar->nSimID = nSimID;
    rpcSendCall(RPC_CHANNEL, call);

    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

uint8_t CFW_ImsGetSrvccCap(CFW_SIM_ID nSimID)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CFW_ImsGetSrvccCap) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_CFW_ImsGetSrvccCap);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct IPAR_CFW_ImsGetSrvccCap *ipar = (struct IPAR_CFW_ImsGetSrvccCap *)((char*)mem + sizeof(rpcCallHeader_t));
    struct OPAR_CFW_ImsGetSrvccCap *opar = (struct OPAR_CFW_ImsGetSrvccCap *)((char*)mem + sizeof(rpcRespHeader_t));
    uint8_t result;

    call->h.size = call_size;
    call->api_tag = TAG_CFW_ImsGetSrvccCap;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    ipar->nSimID = nSimID;
    rpcSendCall(RPC_CHANNEL, call);

    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}



uint32_t CFW_NwChangeRat(uint8_t nRat, uint16_t nUTI, CFW_SIM_ID nSimID)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CFW_NwChangeRat) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_CFW_NwChangeRat);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct IPAR_CFW_NwChangeRat *ipar = (struct IPAR_CFW_NwChangeRat *)((char*)mem + sizeof(rpcCallHeader_t));
    struct OPAR_CFW_NwChangeRat *opar = (struct OPAR_CFW_NwChangeRat *)((char*)mem + sizeof(rpcRespHeader_t));
    uint32_t result;

    call->h.size = call_size;
    call->api_tag = TAG_CFW_NwChangeRat;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    ipar->nRat = nRat;
    ipar->nUTI = nUTI;
    ipar->nSimID = nSimID;
    rpcSendCall(RPC_CHANNEL, call);

    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

uint32_t CFW_GetNetinfo(CFW_NET_INFO_T *pNetinfo, CFW_SIM_ID nSimID)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CFW_GetNetinfo) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_CFW_GetNetinfo);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct IPAR_CFW_GetNetinfo *ipar = (struct IPAR_CFW_GetNetinfo *)((char*)mem + sizeof(rpcCallHeader_t));
    struct OPAR_CFW_GetNetinfo *opar = (struct OPAR_CFW_GetNetinfo *)((char*)mem + sizeof(rpcRespHeader_t));
    uint32_t result;

    call->h.size = call_size;
    call->api_tag = TAG_CFW_GetNetinfo;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    ipar->nSimID = nSimID;
    rpcSendCall(RPC_CHANNEL, call);

    *pNetinfo = opar->pNetinfo;
    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

uint32_t CFW_GetLETRealTimeInfo(CFW_NET_INFO_T *pNetinfo, CFW_SIM_ID nSimID)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CFW_GetLETRealTimeInfo) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_CFW_GetLETRealTimeInfo);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct IPAR_CFW_GetLETRealTimeInfo *ipar = (struct IPAR_CFW_GetLETRealTimeInfo *)((char*)mem + sizeof(rpcCallHeader_t));
    struct OPAR_CFW_GetLETRealTimeInfo *opar = (struct OPAR_CFW_GetLETRealTimeInfo *)((char*)mem + sizeof(rpcRespHeader_t));
    uint32_t result;

    call->h.size = call_size;
    call->api_tag = TAG_CFW_GetLETRealTimeInfo;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    ipar->nSimID = nSimID;
    rpcSendCall(RPC_CHANNEL, call);

    *pNetinfo = opar->pNetinfo;
    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

uint32_t CFW_GetNetinfoStatistic(CFW_NET_INFO_STATISTIC_T *pNetinfoStatistic, CFW_SIM_ID nSimID)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CFW_GetNetinfoStatistic) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_CFW_GetNetinfoStatistic);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct IPAR_CFW_GetNetinfoStatistic *ipar = (struct IPAR_CFW_GetNetinfoStatistic *)((char*)mem + sizeof(rpcCallHeader_t));
    struct OPAR_CFW_GetNetinfoStatistic *opar = (struct OPAR_CFW_GetNetinfoStatistic *)((char*)mem + sizeof(rpcRespHeader_t));
    uint32_t result;

    call->h.size = call_size;
    call->api_tag = TAG_CFW_GetNetinfoStatistic;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    ipar->nSimID = nSimID;
    rpcSendCall(RPC_CHANNEL, call);

    *pNetinfoStatistic = opar->pNetinfoStatistic;
    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

void CFW_SetInitWithSim(bool bWith, CFW_SIM_ID nSimID)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CFW_SetInitWithSim) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + 0;
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct IPAR_CFW_SetInitWithSim *ipar = (struct IPAR_CFW_SetInitWithSim *)((char*)mem + sizeof(rpcCallHeader_t));

    call->h.size = call_size;
    call->api_tag = TAG_CFW_SetInitWithSim;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    ipar->bWith = bWith;
    ipar->nSimID = nSimID;
    rpcSendCall(RPC_CHANNEL, call);

    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
}

uint32_t CFW_GetSimSPN(CFW_SIM_ID nSimID, CFW_SPN_INFO *pSPNInfo)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CFW_GetSimSPN) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_CFW_GetSimSPN);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct IPAR_CFW_GetSimSPN *ipar = (struct IPAR_CFW_GetSimSPN *)((char*)mem + sizeof(rpcCallHeader_t));
    struct OPAR_CFW_GetSimSPN *opar = (struct OPAR_CFW_GetSimSPN *)((char*)mem + sizeof(rpcRespHeader_t));
    uint32_t result;

    call->h.size = call_size;
    call->api_tag = TAG_CFW_GetSimSPN;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    ipar->nSimID = nSimID;
    rpcSendCall(RPC_CHANNEL, call);

    *pSPNInfo = opar->pSPNInfo;
    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

uint32_t CFW_CfgSetAntenna(uint8_t * data, uint16_t len)
{
    size_t ivar_size = ALIGNUP8(len+1);
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CFW_CfgSetAntenna) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_CFW_CfgSetAntenna);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct IPAR_CFW_CfgSetAntenna *ipar = (struct IPAR_CFW_CfgSetAntenna *)((char*)mem + sizeof(rpcCallHeader_t));
    struct OPAR_CFW_CfgSetAntenna *opar = (struct OPAR_CFW_CfgSetAntenna *)((char*)mem + sizeof(rpcRespHeader_t));
    char *ivar_ptr = (char*)mem + sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CFW_CfgSetAntenna);
    uint32_t result;

    call->h.size = call_size;
    call->api_tag = TAG_CFW_CfgSetAntenna;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    memcpy(ivar_ptr, data, len);
    ivar_ptr[len] = (char)0;
    ivar_ptr += ALIGNUP8(len + 1);
    ipar->len = len;
    rpcSendCall(RPC_CHANNEL, call);

    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

uint32_t CFW_ImsEnVolte(uint8_t bEn, CFW_SIM_ID nSimID)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CFW_ImsEnVolte) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_CFW_ImsEnVolte);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct IPAR_CFW_ImsEnVolte *ipar = (struct IPAR_CFW_ImsEnVolte *)((char*)mem + sizeof(rpcCallHeader_t));
    struct OPAR_CFW_ImsEnVolte *opar = (struct OPAR_CFW_ImsEnVolte *)((char*)mem + sizeof(rpcRespHeader_t));
    uint32_t result;

    call->h.size = call_size;
    call->api_tag = TAG_CFW_ImsEnVolte;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    ipar->bEn = bEn;
    ipar->nSimID = nSimID;
    rpcSendCall(RPC_CHANNEL, call);

    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

CFW_SIMSTATUS CFW_SimGetStatus(CFW_SIM_ID nSimID)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CFW_SimGetStatus) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_CFW_SimGetStatus);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct IPAR_CFW_SimGetStatus *ipar = (struct IPAR_CFW_SimGetStatus *)((char*)mem + sizeof(rpcCallHeader_t));
    struct OPAR_CFW_SimGetStatus *opar = (struct OPAR_CFW_SimGetStatus *)((char*)mem + sizeof(rpcRespHeader_t));
    CFW_SIMSTATUS result;

    call->h.size = call_size;
    call->api_tag = TAG_CFW_SimGetStatus;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    ipar->nSimID = nSimID;
    rpcSendCall(RPC_CHANNEL, call);

    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

uint32_t CFW_SimReadFiles(uint8_t * pSimFile, uint8_t nSimFile, uint16_t nUTI, CFW_SIM_ID nSimID)
{
    size_t ivar_size = ALIGNUP8(nSimFile+1);
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CFW_SimReadFiles) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_CFW_SimReadFiles);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct IPAR_CFW_SimReadFiles *ipar = (struct IPAR_CFW_SimReadFiles *)((char*)mem + sizeof(rpcCallHeader_t));
    struct OPAR_CFW_SimReadFiles *opar = (struct OPAR_CFW_SimReadFiles *)((char*)mem + sizeof(rpcRespHeader_t));
    char *ivar_ptr = (char*)mem + sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CFW_SimReadFiles);
    uint32_t result;

    call->h.size = call_size;
    call->api_tag = TAG_CFW_SimReadFiles;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    memcpy(ivar_ptr, pSimFile, nSimFile);
    ivar_ptr[nSimFile] = (char)0;
    ivar_ptr += ALIGNUP8(nSimFile + 1);
    ipar->nSimFile = nSimFile;
    ipar->nUTI = nUTI;
    ipar->nSimID = nSimID;
    rpcSendCall(RPC_CHANNEL, call);

    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

uint32_t CFW_NwSetLockBCCH(bool bLock, uint16_t nUTI, CFW_SIM_ID nSimID)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CFW_NwSetLockBCCH) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_CFW_NwSetLockBCCH);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct IPAR_CFW_NwSetLockBCCH *ipar = (struct IPAR_CFW_NwSetLockBCCH *)((char*)mem + sizeof(rpcCallHeader_t));
    struct OPAR_CFW_NwSetLockBCCH *opar = (struct OPAR_CFW_NwSetLockBCCH *)((char*)mem + sizeof(rpcRespHeader_t));
    uint32_t result;

    call->h.size = call_size;
    call->api_tag = TAG_CFW_NwSetLockBCCH;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    ipar->bLock = bLock;
    ipar->nUTI = nUTI;
    ipar->nSimID = nSimID;
    rpcSendCall(RPC_CHANNEL, call);

    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

bool CFW_NwGetLockBCCH(void)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + 0 + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_CFW_NwGetLockBCCH);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct OPAR_CFW_NwGetLockBCCH *opar = (struct OPAR_CFW_NwGetLockBCCH *)((char*)mem + sizeof(rpcRespHeader_t));
    bool result;

    call->h.size = call_size;
    call->api_tag = TAG_CFW_NwGetLockBCCH;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    rpcSendCall(RPC_CHANNEL, call);

    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

uint32_t CFW_SwitchUSBShareNetwork(bool flag)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CFW_SwitchUSBShareNetwork) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_CFW_SwitchUSBShareNetwork);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct IPAR_CFW_SwitchUSBShareNetwork *ipar = (struct IPAR_CFW_SwitchUSBShareNetwork *)((char*)mem + sizeof(rpcCallHeader_t));
    struct OPAR_CFW_SwitchUSBShareNetwork *opar = (struct OPAR_CFW_SwitchUSBShareNetwork *)((char*)mem + sizeof(rpcRespHeader_t));
    uint32_t result;

    call->h.size = call_size;
    call->api_tag = TAG_CFW_SwitchUSBShareNetwork;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    ipar->flag = flag;
    rpcSendCall(RPC_CHANNEL, call);

    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}
uint32_t CFW_SatRefreshFileChange(CFW_SAT_REFRESH_FILE_CHG_REQ_T FileChangeReq, uint16_t nUTI, CFW_SIM_ID nSimID)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CFW_SatRefreshFileChange) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_CFW_SatRefreshFileChange);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct IPAR_CFW_SatRefreshFileChange *ipar = (struct IPAR_CFW_SatRefreshFileChange *)((char*)mem + sizeof(rpcCallHeader_t));
    struct OPAR_CFW_SatRefreshFileChange *opar = (struct OPAR_CFW_SatRefreshFileChange *)((char*)mem + sizeof(rpcRespHeader_t));
    uint32_t result;

    call->h.size = call_size;
    call->api_tag = TAG_CFW_SatRefreshFileChange;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;
    ipar->FileChangeReq = FileChangeReq;
    ipar->nUTI = nUTI;
    ipar->nSimID = nSimID;
    rpcSendCall(RPC_CHANNEL, call);

    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}
uint32_t CFW_SatRefreshImsiChange(uint16_t nUTI, CFW_SIM_ID nSimID)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CFW_SatRefreshImsiChange) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_CFW_SatRefreshImsiChange);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct IPAR_CFW_SatRefreshImsiChange *ipar = (struct IPAR_CFW_SatRefreshImsiChange *)((char*)mem + sizeof(rpcCallHeader_t));
    struct OPAR_CFW_SatRefreshImsiChange *opar = (struct OPAR_CFW_SatRefreshImsiChange *)((char*)mem + sizeof(rpcRespHeader_t));
    uint32_t result;

    call->h.size = call_size;
    call->api_tag = TAG_CFW_SatRefreshImsiChange;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    ipar->nUTI = nUTI;
    ipar->nSimID = nSimID;
    rpcSendCall(RPC_CHANNEL, call);

    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}
uint32_t CFW_SSCheckTestSim(CFW_SIM_ID nSimID)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CFW_SSCheckTestSim) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_CFW_SSCheckTestSim);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct IPAR_CFW_SSCheckTestSim *ipar = (struct IPAR_CFW_SSCheckTestSim *)((char*)mem + sizeof(rpcCallHeader_t));
    struct OPAR_CFW_SSCheckTestSim *opar = (struct OPAR_CFW_SSCheckTestSim *)((char*)mem + sizeof(rpcRespHeader_t));
    uint32_t result;

    call->h.size = call_size;
    call->api_tag = TAG_CFW_SSCheckTestSim;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    ipar->nSimID = nSimID;
    rpcSendCall(RPC_CHANNEL, call);

    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}
uint32_t CFW_SimReset(uint16_t nUTI, CFW_SIM_ID nSimID)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CFW_SimReset) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_CFW_SimReset);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct IPAR_CFW_SimReset *ipar = (struct IPAR_CFW_SimReset *)((char*)mem + sizeof(rpcCallHeader_t));
    struct OPAR_CFW_SimReset *opar = (struct OPAR_CFW_SimReset *)((char*)mem + sizeof(rpcRespHeader_t));
    uint32_t result;

    call->h.size = call_size;
    call->api_tag = TAG_CFW_SimReset;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    ipar->nUTI = nUTI;
    ipar->nSimID = nSimID;
    rpcSendCall(RPC_CHANNEL, call);

    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}
uint32_t CFW_CcAcceptSpeechCallEx(CFW_SIM_ID nSimID)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CFW_CcAcceptSpeechCallEx) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_CFW_CcAcceptSpeechCallEx);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct IPAR_CFW_CcAcceptSpeechCallEx *ipar = (struct IPAR_CFW_CcAcceptSpeechCallEx *)((char*)mem + sizeof(rpcCallHeader_t));
    struct OPAR_CFW_CcAcceptSpeechCallEx *opar = (struct OPAR_CFW_CcAcceptSpeechCallEx *)((char*)mem + sizeof(rpcRespHeader_t));
    uint32_t result;

    call->h.size = call_size;
    call->api_tag = TAG_CFW_CcAcceptSpeechCallEx;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    ipar->nSimID = nSimID;
    rpcSendCall(RPC_CHANNEL, call);

    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

uint32_t CFW_SmsMtSmsPPAckReq(CFW_SIM_ID nSimID)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CFW_SmsMtSmsPPAckReq) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_CFW_SmsMtSmsPPAckReq);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct IPAR_CFW_SmsMtSmsPPAckReq *ipar = (struct IPAR_CFW_SmsMtSmsPPAckReq *)((char*)mem + sizeof(rpcCallHeader_t));
    struct OPAR_CFW_SmsMtSmsPPAckReq *opar = (struct OPAR_CFW_SmsMtSmsPPAckReq *)((char*)mem + sizeof(rpcRespHeader_t));
    uint32_t result;

    call->h.size = call_size;
    call->api_tag = TAG_CFW_SmsMtSmsPPAckReq;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    ipar->nSimID = nSimID;
    rpcSendCall(RPC_CHANNEL, call);

    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}


uint32_t CFW_SmsMemoryFullReq(uint8_t nCause, CFW_SIM_ID nSimID)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CFW_SmsMemoryFullReq) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_CFW_SmsMemoryFullReq);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct IPAR_CFW_SmsMemoryFullReq *ipar = (struct IPAR_CFW_SmsMemoryFullReq *)((char*)mem + sizeof(rpcCallHeader_t));
    struct OPAR_CFW_SmsMemoryFullReq *opar = (struct OPAR_CFW_SmsMemoryFullReq *)((char*)mem + sizeof(rpcRespHeader_t));
    uint32_t result;

    call->h.size = call_size;
    call->api_tag = TAG_CFW_SmsMemoryFullReq;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    ipar->nCause = nCause;
    ipar->nSimID = nSimID;
    rpcSendCall(RPC_CHANNEL, call);

    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

uint32_t CFW_SmsMMAReq(CFW_SIM_ID nSimID)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CFW_SmsMMAReq) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_CFW_SmsMMAReq);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct IPAR_CFW_SmsMMAReq *ipar = (struct IPAR_CFW_SmsMMAReq *)((char*)mem + sizeof(rpcCallHeader_t));
    struct OPAR_CFW_SmsMMAReq *opar = (struct OPAR_CFW_SmsMMAReq *)((char*)mem + sizeof(rpcRespHeader_t));
    uint32_t result;

    call->h.size = call_size;
    call->api_tag = TAG_CFW_SmsMMAReq;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    ipar->nSimID = nSimID;
    rpcSendCall(RPC_CHANNEL, call);

    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

uint8_t CFW_NwGetFTA(CFW_SIM_ID nSimID)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CFW_NwGetFTA) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_CFW_NwGetFTA);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct IPAR_CFW_NwGetFTA *ipar = (struct IPAR_CFW_NwGetFTA *)((char*)mem + sizeof(rpcCallHeader_t));
    struct OPAR_CFW_NwGetFTA *opar = (struct OPAR_CFW_NwGetFTA *)((char*)mem + sizeof(rpcRespHeader_t));
    uint8_t result;

    call->h.size = call_size;
    call->api_tag = TAG_CFW_NwGetFTA;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    ipar->nSimID = nSimID;
    rpcSendCall(RPC_CHANNEL, call);

    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

uint32_t CFW_NwSetAutoAttachFlag(uint8_t flag, CFW_SIM_ID nSimID)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CFW_NwSetAutoAttachFlag) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_CFW_NwSetAutoAttachFlag);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct IPAR_CFW_NwSetAutoAttachFlag *ipar = (struct IPAR_CFW_NwSetAutoAttachFlag *)((char*)mem + sizeof(rpcCallHeader_t));
    struct OPAR_CFW_NwSetAutoAttachFlag *opar = (struct OPAR_CFW_NwSetAutoAttachFlag *)((char*)mem + sizeof(rpcRespHeader_t));
    uint32_t result;

    call->h.size = call_size;
    call->api_tag = TAG_CFW_NwSetAutoAttachFlag;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    ipar->flag = flag;
    ipar->nSimID = nSimID;
    rpcSendCall(RPC_CHANNEL, call);

    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

uint8_t CFW_NwGetAutoAttachFlag(CFW_SIM_ID nSimID)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CFW_NwGetAutoAttachFlag) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_CFW_NwGetAutoAttachFlag);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct IPAR_CFW_NwGetAutoAttachFlag *ipar = (struct IPAR_CFW_NwGetAutoAttachFlag *)((char*)mem + sizeof(rpcCallHeader_t));
    struct OPAR_CFW_NwGetAutoAttachFlag *opar = (struct OPAR_CFW_NwGetAutoAttachFlag *)((char*)mem + sizeof(rpcRespHeader_t));
    uint8_t result;

    call->h.size = call_size;
    call->api_tag = TAG_CFW_NwGetAutoAttachFlag;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    ipar->nSimID = nSimID;
    rpcSendCall(RPC_CHANNEL, call);

    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

uint32_t CFW_GprsSetInternalPdpCtx(CFW_INTERNAL_PDP_CTX_TYPE nType, const CFW_GPRS_PDPCONT_INFO_V2 *pPdpCont, CFW_SIM_ID nSimID)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CFW_GprsSetInternalPdpCtx) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_CFW_GprsSetInternalPdpCtx);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct IPAR_CFW_GprsSetInternalPdpCtx *ipar = (struct IPAR_CFW_GprsSetInternalPdpCtx *)((char*)mem + sizeof(rpcCallHeader_t));
    struct OPAR_CFW_GprsSetInternalPdpCtx *opar = (struct OPAR_CFW_GprsSetInternalPdpCtx *)((char*)mem + sizeof(rpcRespHeader_t));
    uint32_t result;

    call->h.size = call_size;
    call->api_tag = TAG_CFW_GprsSetInternalPdpCtx;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    ipar->nType = nType;
    ipar->pPdpCont = *pPdpCont;
    ipar->nSimID = nSimID;
    rpcSendCall(RPC_CHANNEL, call);

    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

uint32_t CFW_EmodImsNvParamSet(uint16_t param_id, uint8_t * value_ptr, uint16_t value_len, CFW_SIM_ID nSimID)
{
    size_t ivar_size = ALIGNUP8(value_len*sizeof(uint8_t)+1);
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CFW_EmodImsNvParamSet) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_CFW_EmodImsNvParamSet);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct IPAR_CFW_EmodImsNvParamSet *ipar = (struct IPAR_CFW_EmodImsNvParamSet *)((char*)mem + sizeof(rpcCallHeader_t));
    struct OPAR_CFW_EmodImsNvParamSet *opar = (struct OPAR_CFW_EmodImsNvParamSet *)((char*)mem + sizeof(rpcRespHeader_t));
    char *ivar_ptr = (char*)mem + sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CFW_EmodImsNvParamSet);
    uint32_t result;

    call->h.size = call_size;
    call->api_tag = TAG_CFW_EmodImsNvParamSet;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    ipar->param_id = param_id;
    memcpy(ivar_ptr, value_ptr, value_len*sizeof(uint8_t));
    ivar_ptr[value_len*sizeof(uint8_t)] = (char)0;
    ivar_ptr += ALIGNUP8(value_len*sizeof(uint8_t) + 1);
    ipar->value_len = value_len;
    ipar->nSimID = nSimID;
    rpcSendCall(RPC_CHANNEL, call);

    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

uint32_t CFW_EmodImsNvParamGet(uint16_t param_id, uint8_t value_ptr[256], CFW_SIM_ID nSimID)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CFW_EmodImsNvParamGet) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_CFW_EmodImsNvParamGet);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct IPAR_CFW_EmodImsNvParamGet *ipar = (struct IPAR_CFW_EmodImsNvParamGet *)((char*)mem + sizeof(rpcCallHeader_t));
    struct OPAR_CFW_EmodImsNvParamGet *opar = (struct OPAR_CFW_EmodImsNvParamGet *)((char*)mem + sizeof(rpcRespHeader_t));
    uint32_t result;

    call->h.size = call_size;
    call->api_tag = TAG_CFW_EmodImsNvParamGet;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    ipar->param_id = param_id;
    memcpy(ipar->value_ptr, value_ptr, sizeof(ipar->value_ptr));
    ipar->nSimID = nSimID;
    rpcSendCall(RPC_CHANNEL, call);

    memcpy(value_ptr, opar->value_ptr, sizeof(opar->value_ptr));
    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

void CFW_ImsSetUaVersionInfo(int8_t * pVersionInfo, uint8_t nLen)
{
    size_t ivar_size = ALIGNUP8(nLen+1);
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CFW_ImsSetUaVersionInfo) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + 0;
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct IPAR_CFW_ImsSetUaVersionInfo *ipar = (struct IPAR_CFW_ImsSetUaVersionInfo *)((char*)mem + sizeof(rpcCallHeader_t));
    char *ivar_ptr = (char*)mem + sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CFW_ImsSetUaVersionInfo);

    call->h.size = call_size;
    call->api_tag = TAG_CFW_ImsSetUaVersionInfo;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    memcpy(ivar_ptr, pVersionInfo, nLen);
    ivar_ptr[nLen] = (char)0;
    ivar_ptr += ALIGNUP8(nLen + 1);
    ipar->nLen = nLen;
    rpcSendCall(RPC_CHANNEL, call);

    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
}



uint32_t CFW_EmodResetImsDelta(void)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + 0 + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_CFW_EmodResetImsDelta);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct OPAR_CFW_EmodResetImsDelta *opar = (struct OPAR_CFW_EmodResetImsDelta *)((char*)mem + sizeof(rpcRespHeader_t));
    uint32_t result;

    call->h.size = call_size;
    call->api_tag = TAG_CFW_EmodResetImsDelta;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    rpcSendCall(RPC_CHANNEL, call);

    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

uint32_t CFW_CcEmcDialEx_V2(uint8_t * pDialNumber, uint8_t nDialNumberSize, uint8_t *pIndex, CFW_CcEmergCateg emergCateg, CFW_SIM_ID nSimID)
{
    size_t ivar_size = ALIGNUP8(nDialNumberSize+1);
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CFW_CcEmcDialEx_V2) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_CFW_CcEmcDialEx_V2);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct IPAR_CFW_CcEmcDialEx_V2 *ipar = (struct IPAR_CFW_CcEmcDialEx_V2 *)((char*)mem + sizeof(rpcCallHeader_t));
    struct OPAR_CFW_CcEmcDialEx_V2 *opar = (struct OPAR_CFW_CcEmcDialEx_V2 *)((char*)mem + sizeof(rpcRespHeader_t));
    char *ivar_ptr = (char*)mem + sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CFW_CcEmcDialEx_V2);
    uint32_t result;

    call->h.size = call_size;
    call->api_tag = TAG_CFW_CcEmcDialEx_V2;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    memcpy(ivar_ptr, pDialNumber, nDialNumberSize);
    ivar_ptr[nDialNumberSize] = (char)0;
    ivar_ptr += ALIGNUP8(nDialNumberSize + 1);
    ipar->nDialNumberSize = nDialNumberSize;
    ipar->emergCateg = emergCateg;
    ipar->nSimID = nSimID;
    rpcSendCall(RPC_CHANNEL, call);

    *pIndex = opar->pIndex;
    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}


uint32_t CFW_SetGcfTest(CFW_SIM_ID nSimID, bool nFlag)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CFW_SetGcfTest) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_CFW_SetGcfTest);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct IPAR_CFW_SetGcfTest *ipar = (struct IPAR_CFW_SetGcfTest *)((char*)mem + sizeof(rpcCallHeader_t));
    struct OPAR_CFW_SetGcfTest *opar = (struct OPAR_CFW_SetGcfTest *)((char*)mem + sizeof(rpcRespHeader_t));
    uint32_t result;

    call->h.size = call_size;
    call->api_tag = TAG_CFW_SetGcfTest;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    ipar->nSimID = nSimID;
    ipar->nFlag = nFlag;
    rpcSendCall(RPC_CHANNEL, call);

    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

uint32_t CFW_NwGetLteSignalQuality(uint8_t *pSignalLevel, uint8_t *pBitError, CFW_SIM_ID nSimID)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CFW_NwGetLteSignalQuality) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_CFW_NwGetLteSignalQuality);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct IPAR_CFW_NwGetLteSignalQuality *ipar = (struct IPAR_CFW_NwGetLteSignalQuality *)((char*)mem + sizeof(rpcCallHeader_t));
    struct OPAR_CFW_NwGetLteSignalQuality *opar = (struct OPAR_CFW_NwGetLteSignalQuality *)((char*)mem + sizeof(rpcRespHeader_t));
    uint32_t result;

    call->h.size = call_size;
    call->api_tag = TAG_CFW_NwGetLteSignalQuality;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    ipar->nSimID = nSimID;
    rpcSendCall(RPC_CHANNEL, call);

    *pSignalLevel = opar->pSignalLevel;
    *pBitError = opar->pBitError;
    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

uint32_t CFW_SetRRCRel(uint32_t value, CFW_SIM_ID nSimID)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CFW_SetRRCRel) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_CFW_SetRRCRel);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct IPAR_CFW_SetRRCRel *ipar = (struct IPAR_CFW_SetRRCRel *)((char*)mem + sizeof(rpcCallHeader_t));
    struct OPAR_CFW_SetRRCRel *opar = (struct OPAR_CFW_SetRRCRel *)((char*)mem + sizeof(rpcRespHeader_t));
    uint32_t result;

    call->h.size = call_size;
    call->api_tag = TAG_CFW_SetRRCRel;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    ipar->value = value;
    ipar->nSimID = nSimID;
    rpcSendCall(RPC_CHANNEL, call);

    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

uint32_t CFW_GetRRCRel(CFW_SIM_ID nSimID)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CFW_GetRRCRel) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_CFW_GetRRCRel);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct IPAR_CFW_GetRRCRel *ipar = (struct IPAR_CFW_GetRRCRel *)((char*)mem + sizeof(rpcCallHeader_t));
    struct OPAR_CFW_GetRRCRel *opar = (struct OPAR_CFW_GetRRCRel *)((char*)mem + sizeof(rpcRespHeader_t));
    uint32_t result;

    call->h.size = call_size;
    call->api_tag = TAG_CFW_GetRRCRel;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    ipar->nSimID = nSimID;
    rpcSendCall(RPC_CHANNEL, call);

    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

uint32_t CFW_NwGetQualReport(CFW_NW_QUAL_INFO *pQualReport, CFW_SIM_ID nSimID)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CFW_NwGetQualReport) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_CFW_NwGetQualReport);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct IPAR_CFW_NwGetQualReport *ipar = (struct IPAR_CFW_NwGetQualReport *)((char*)mem + sizeof(rpcCallHeader_t));
    struct OPAR_CFW_NwGetQualReport *opar = (struct OPAR_CFW_NwGetQualReport *)((char*)mem + sizeof(rpcRespHeader_t));
    uint32_t result;

    call->h.size = call_size;
    call->api_tag = TAG_CFW_NwGetQualReport;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    ipar->nSimID = nSimID;
    rpcSendCall(RPC_CHANNEL, call);

    *pQualReport = opar->pQualReport;
    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

uint32_t CFW_SimSetVirtualFlag(uint8_t flag, CFW_SIM_ID nSimID)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_CFW_SimSetVirtualFlag) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_CFW_SimSetVirtualFlag);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct IPAR_CFW_SimSetVirtualFlag *ipar = (struct IPAR_CFW_SimSetVirtualFlag *)((char*)mem + sizeof(rpcCallHeader_t));
    struct OPAR_CFW_SimSetVirtualFlag *opar = (struct OPAR_CFW_SimSetVirtualFlag *)((char*)mem + sizeof(rpcRespHeader_t));
    uint32_t result;

    call->h.size = call_size;
    call->api_tag = TAG_CFW_SimSetVirtualFlag;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    ipar->flag = flag;
    ipar->nSimID = nSimID;
    rpcSendCall(RPC_CHANNEL, call);

    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}

// AUTO GENERATED END
