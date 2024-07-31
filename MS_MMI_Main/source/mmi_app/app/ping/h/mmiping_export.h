//#define YYA_TEST 1

/*----------------------------------------------------------------------------*/
/*                          Include Files                                     */
/*----------------------------------------------------------------------------*/
#ifdef MMIENG_PING_SUPPORT
#include "sci_types.h"
#include "mmk_type.h"

/*----------------------------------------------------------------------------*/
/*                         Compiler Flag                                      */
/*----------------------------------------------------------------------------*/

#ifdef _cplusplus
extern   "C"
{
#endif

/*----------------------------------------------------------------------------*/
/*                         MACRO DEFINITION                                   */
/*----------------------------------------------------------------------------*/

#define PBSYNC_MAX_PHONE_NUM      MMIPB_MAX_NV_PHONE_NUM
#define PBSYNC_MAX_EMAIL_NUM      1//先定义为1，联系人存储结构体发生改变时切记同步

/*----------------------------------------------------------------------------*/
/*                          TYPE AND STRUCT                                   */
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/*                         Function Declare                                   */
/*----------------------------------------------------------------------------*/
PUBLIC BOOLEAN MMIAPIENG_OpenPingSettingWin(uint32 dual_sys, BOOLEAN isReturnOK, DPARAM param);
PUBLIC void MMIAPIENG_OpenPingSelectWin(void);
PUBLIC void MMIAPIPING_UpdatePacketShowWin(void);
PUBLIC BOOLEAN MMIAPIENG_CreateShowPacketInfoWin(void);
/*----------------------------------------------------------------------------*/
/*                         Compiler Flag                                      */
/*----------------------------------------------------------------------------*/
#ifdef _cplusplus
}
#endif
#endif
