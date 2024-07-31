/*
 * Copyright 2003 Aplix Corporation. All Rights Reserved.
 *
 * JBlend and all Aplix- or JBlend-related trademarks and logos are
 * trademarks or registered trademarks of Aplix in the U.S., Japan and
 * other countries.
 */
#include "std_header.h"
//#ifdef JAVA_SUPPORT_IA
#if 0
#include "aj_type.h"
#include "jktypes.h"
#include "ajms_ab.h"
#include "ajms_pcs.h"
#include "ajms_ota.h"
#include "ajms_arc.h"
#include "ajms_cfg_eime.h"
AJ_S32 ajms_ab_getSutieListCount( const AJ_S32 categoryId )
{
    return 0;
}
JKBool AmNotifyPointerEvent( JKSint32 type, JKSint32 buttonNo, JKSint32 x, JKSint32 y )
{
    return 0;
}
AJ_S32 ajms_arc_resumeApp(void)
{
    return 0;
}
AJ_S32 ajms_arc_suspendApp(void)
{
    return 0;
    
}
AJ_S32 ajms_arc_getState(void)
{
    return 0;
}
AJ_S32 ajms_ab_getSuiteList( const AJ_S32           categoryId, 
                                   AJMS_Suite*      suiteList, 
                             const AJ_S32           sortBy )
{
    return 0;
}
AJ_S32 ajms_ab_getMidletListCount( const AJ_S32 suiteId )
{
    return 0;
}
AJ_S32 ajms_ota_delete( const AJ_S32 suiteId )
{
    return 0;
}
AJ_S32 ajms_pcs_getGroupCount( AJ_S32 domainId )
{
    return 0;
}

AJ_S32 ajms_mss_setGroupMode( const AJ_S32 suiteId, 
                              const AJ_S32 groupId, 
                              const AJ_S32 mode,
                              AJMS_DomainRestriction *info )
{
    return 0;
}
AJ_S32 ajms_mss_getGroupMode( const AJ_S32  suiteId, 
                              const AJ_S32  groupId,
                                    AJ_S32 *mode )
{
    return 0;
}
AJ_S32 ajms_ota_check(
                             const AJ_S32                      dataSize,
                             const AJ_U8                      *dataAddr,
                             AJMS_Reader                       dataRead,
                             const AJ_S32                      dataType,
                             const AJ_U8                      *jadUrl,
                             AJMS_CheckResult                 *result,
                             AJMS_CheckCB                      callback)
{
    return 0;
}
AJ_S32 ajms_ota_install( const AJ_S32                      categoryId,
                         const AJ_S32                      jadSize,
                         const AJ_U8                      *jadAddr,
                         AJMS_Reader                       jadRead,
                         const AJ_U8                      *jadUrl,
                         const AJ_S32                      jarSize,
                         const AJ_U8                      *jarAddr,
                         AJMS_Reader                       jarRead,
                         const AJ_S32                      flag,
                         const AJ_S32                      oldId,
                         AJMS_InstallResult               *result,
                         AJMS_InstallCB                    callback)
{
    return 0;
}
AJ_S32 ajms_init(AJMS_DEBUG_FilterCallback filter)
{
    return 0;
}
void ajms_cfg_finalizeEime(AJ_U16* resultStr, AJ_S32 resultStrLen, AJ_S32 resultState)
{
    
}
AJ_S32 ajms_pcs_getGroup( AJ_S32 domainId, 
                          AJ_S32 groupId, 
                          AJMS_DomainGroup* info )
{
    return 0;
}
AJ_S32 ajms_fini(void)
{
    return 0;
}
int ajsc_keyEventHandler(int type, int keyCode, short keyChar)
{
    return 0;
}
AJ_S32 ajms_arc_terminateApp(void)
{
    return 0;
}
AJ_S32 ajms_arc_runApp(
                const AJ_S32 suiteId,
                const AJ_S32 midletId,
                AJMS_VmEventHandler vmEventHandler)
{
    return 0;
}
void ajms_cfg_registerEimeHandler( AJMS_EimeLaunch    launchCb, 
                                   AJMS_EimeTerminate terminateCb)
{
    
}
AJ_S32 ajms_pcs_setStringTable( AJ_S32 tableId )
{
    return 0;
}
AJ_S32 ajms_ab_getMidletList( const AJ_S32        suiteId, 
                                    AJMS_Midlet*  midletArray )
{
    return 0;
}
PUBLIC BOOLEAN AmoiReadVcomData(uint8 *pBuf, int size, uint32 *readLen)
{
    return 0;
}
#endif