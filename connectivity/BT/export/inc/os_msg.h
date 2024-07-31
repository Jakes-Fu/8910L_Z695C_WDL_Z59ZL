#ifndef __OS_MSG_H__
#define  __OS_MSG_H__
//#include "os_dev_define.h"
#include "os_api.h"

#define BT_OS_MSG_START  (0x100)
#define BT_FP_MSG_START  (0x200)
#define BT_APP_MSG_START  (0x300)

typedef enum
{
    OS_TIME_EVENT  = 0x1000,
    //OS_BACKGROUND_IRQ = 0x1001,
    //OS_MSG_TRANSPORT = 0x1002,  // no use for iAnywhere
    OS_MSG_EXTEND_EVM    = 0X1003,
    OS_MSG_EXTEND_TRIG_DATA = 0X1004,
    OS_MSG_SAVE_PCM_DATA = 0X1005,
    OS_MSG_GSM_L1_INFO = 0X1006,
    //OS_MSG_EXTEND_OPC_DATA  = 0x1005
} OS_INT_MSG_TYPE_E;

typedef struct OS_INT_MSG_T
{
    unsigned int message[4];
} OS_INT_MSG_T;

typedef enum
{
    OS_START_SCHED_MSG = BT_OS_MSG_START,
    OS_STOP_SCHED_MSG,
    OS_INT_HANDLE_MSG,
    OS_GSM_L1_INFO_MSG
    //OS_TIME_EVENT_MSG,
    //OS_EXTEND_EVM_MSG,
    //OS_EXTEND_TRIG_DATA_MSG
} CSR_OS_SCHED_MSG_E;

typedef struct BT_SIGNAL_T
{
    uint16 SignalCode;
    uint16 SignalSize;
    xSignalHeader  Pre;
    xSignalHeader  Suc;
    BLOCK_ID       Sender;
    void *SigP ;
} BT_SIGNAL_T;


extern void OS_SendMsgToSchedTask(uint32 event, void *msg_ptr);

#endif /* __OS_MSG_H__*/