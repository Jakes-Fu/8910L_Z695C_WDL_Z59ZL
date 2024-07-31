// AUTO GENERATED

uint32_t rpcHelloOnCP(uint32_t n)
{
    size_t ivar_size = 0;
    size_t call_size = sizeof(rpcCallHeader_t) + sizeof(struct IPAR_rpcHelloOnCP) + ivar_size;
    size_t rsp_size = sizeof(rpcRespHeader_t) + sizeof(struct OPAR_rpcHelloOnCP);
    size_t mem_size = (call_size > rsp_size) ? call_size : rsp_size;
    void *mem = (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT) ? malloc(mem_size) : alloca(mem_size);
    rpcCallHeader_t *call = (rpcCallHeader_t *)mem;
    struct IPAR_rpcHelloOnCP *ipar = (struct IPAR_rpcHelloOnCP *)((char*)mem + sizeof(rpcCallHeader_t));
    struct OPAR_rpcHelloOnCP *opar = (struct OPAR_rpcHelloOnCP *)((char*)mem + sizeof(rpcRespHeader_t));
    uint32_t result;

    call->h.size = call_size;
    call->api_tag = TAG_rpcHelloOnCP;
    call->caller_rsp_ptr = (uint32_t)mem;
    call->rsp_size = rsp_size;

    ipar->n = n;
    rpcSendCall(RPC_CHANNEL, call);

    result = opar->result;
    if (mem_size > CONFIG_RPC_SEND_STACK_ALLOCA_LIMIT)
        free(mem);
    return result;
}


// AUTO GENERATED END
