#ifndef _TLS_DBG_H_
#define _TLS_DBG_H_

#include "we_cfg.h"
#ifdef WE_CFG_BREW_PLATFORM
#include "AEESTDLIB.h"
#else
#ifndef WIN32_DEBUG
#include "tf3rd_for_HZ.h"
#endif
#endif
/*******************************
    WAP_LOG_WRITE_FLAG
0 : only display on screen
1 : only write in file
2 : both
*******************************/
#define TLS_LOG_WRITE_FLAG  0

/*******************************
  0: off
  1: on
*******************************/

#define WAP_TLS_DBG_ON       1
#define WAP_ITLS_DBG_ON      1
#define WAP_TLS_DBG_MSG      1

//typedef enum{
#define E_WAP_DBG_TLS        (WE_INT32)0x00000020 
#define E_WAP_DBG_ITLS       (WE_INT32)0x00040000 
#define E_TLS_DBG_MSG        (WE_INT32)0x00000001 
#define E_WAP_DBG_ALL        (WE_INT32)0x7FFFFFFF
//}E_WAP_DBG_MODULE;

//typedef enum{
#define E_WAP_DBG_LEVEL_LOW    (WE_INT32)0x00
#define E_WAP_DBG_LEVEL_MID    (WE_INT32)0x01
#define E_WAP_DBG_LEVEL_HIGH   (WE_INT32)0x02
#define E_WAP_DBG_LEVEL_ERR    (WE_INT32)0x03
#define E_WAP_DBG_LEVEL_FATAL  (WE_INT32)0x04
#define E_WAP_DBG_LEVEL_TOTAL  (WE_INT32)0x05
//}E_WAP_DBG_LEVEL;

#ifdef WE_DEBUG_INFO
#if WAP_LOG_WRITE_FLAG>0
#define WAP_LOG_WRITE_TO_FILE
#endif

#define WAP_DBG_LEVEL E_WAP_DBG_LEVEL_LOW

#define WAP_DBG_ON  (WE_INT32)(WAP_TLS_DBG_ON*E_WAP_DBG_TLS|WAP_TLS_DBG_MSG*E_TLS_DBG_MSG|WAP_ITLS_DBG_ON*WAP_ITLS_DBG_ON)
                     
void  WapLogPrintfNone(const WE_CHAR *format,...);

#if WAP_LOG_WRITE_FLAG>0
void WapPrintf(const WE_CHAR *format,...);
void WapPrintfName(const WE_CHAR *format,...);
void Wap_InitializeDbg( void );
void Wap_TerminateDbg( void );
#else
#define WapPrintf TF3RD_MEM_DBGPRINTF
//#define WapPrintf
#define WapPrintfName WapPrintf
#endif

// #define WAP_LOG_PRINT_EXT(eModule,eLevel) ((((eModule)&WAP_DBG_ON)&&((eLevel)>=WAP_DBG_LEVEL))\
//                                       ?(WapPrintfName("\r\n%s-%d: ",__FILE__,__LINE__),WapPrintf)\
//                                       :WapLogPrintfNone)
//#define WAP_LOG_PRINT(x) WAP_LOG_PRINT_EXT x
#define WAP_LOG_PRINT(x)  

#else
#define WAP_LOG_PRINT(x)  
#endif/* end WE_DEBUG_INFO */

#endif  /* end _WE_DBG_H_ */

