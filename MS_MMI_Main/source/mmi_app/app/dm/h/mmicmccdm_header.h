#ifndef __CMCCDM_HEADER_H__
#define __CMCCDM_HEADER_H__

typedef struct _CMCCDM_LINKER
{
    BOOLEAN flags;
    MN_DUAL_SYS_E sim_sys;
    uint32 net_id;
  //  SFR_HTTP_CONTEXT http_ctx;
  //  SFR_CARRIER *carrier;
  //  int http_return;
} CMCCDM_LINKER;

extern CMCCDM_LINKER g_cmccdm_linker;

#endif // __SFR_HEADER_H__
