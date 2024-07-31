#ifndef _MMICP_EXPORT_H_
#define _MMICP_EXPORT_H_ 

/**--------------------------------------------------------------------------*
**                         Compiler Flag                                    *
**--------------------------------------------------------------------------*/
#ifdef __cplusplus
extern   "C"
{
#endif

/**---------------------------------------------------------------------------*
** Macro Declaration *
**---------------------------------------------------------------------------*/
/*****************************************************************************/
// 如下宏定义用以实现对外接口更名前后的兼容
/*****************************************************************************/
#ifdef MMI_FUNC_COMPATIBLE_SUPPORT

#endif //MMI_FUNC_COMPATIBLE_SUPPORT

typedef enum
{
    PREF_SIM_1 = 0,
    PREF_SIM_2 = 1,
    ALWAYS_ASK = 3
} PREFERRED_SIM_E;

PUBLIC BOOLEAN MMIAPICP_IsMainWinOpen();
PUBLIC void MMIAPICP_AppInit(void);
PUBLIC void MMIAPICP_InitModule(void);
PUBLIC void MMIAPICP_OpenMainWin(void);
PUBLIC PREFERRED_SIM_E MMIAPICP_GetPreferredSimState(void);
/**--------------------------------------------------------------------------*
**                         Compiler Flag                                    *
**--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif
    
#endif
