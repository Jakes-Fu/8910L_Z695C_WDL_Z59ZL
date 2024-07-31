//@David.Jia 2007.6.19  begin
#include "ms_customize_trc.h"
#include "uartcom_drv.h"

#define COM_0   0
#define COM_1   1               //uart port

#define REG32(x)    (*(volatile uint32 *)(x))

typedef enum {
    COM_SEND,       //send complete
    COM_REC,        //receive complete, no data in uart rx fifo
    COM_REMAINDER,  //receive complete, remainder data in uart rx fifo
    COM_TIMEOUT     //receive timeout, received data number less than expected
} COM_EVENT; 

/*****************************************************************************/
//  FUNCTION:     COM_CallBack
//  Description:    callback function for com communication. called after
//      sent all bytes ,after received all bytes, receive timeout and error.
//      called in ISR, so only send a event to COM TASK and return.
//  INPUT:          event--indetify which event occur, num--bytes done.
//  return:         0--ok, others--error.
//  Author:         David.Jia
//  date:           2007.6.19
//	Note:           
/*****************************************************************************/
typedef int (*COM_CallBack)(COM_EVENT event, uint32 num);   

typedef struct {
    uint32 port;                    //uart port
    uint8* send_buf;                //save send buffer address and size
    uint32 send_len;    
    uint32 send_len_done;           //already sent
    COM_CallBack    send_callback;
    uint8* rec_buf;                 //save receive buffer address and size
    uint32 rec_len;
    uint32 rec_len_done;            //already received
    COM_CallBack    rec_callback;
    int error;                      //lastest error code
} COM_OBJ;


//@David.Jia 2007.6.23  begin
//Cycle Queue struct
typedef struct {
    uint8 *buf;
    uint32 size;        //queue volume
    uint32 head;
    uint32 tail;
    unsigned empty: 1;
    unsigned full:  1;
    unsigned overflow:  1;  //queue input after full, overwrite occur
}   CycleQueue;

#define COM_BUFFER_SIZE 2048        //default com send/receive buffer size
static uint8 s_send_buf[COM_BUFFER_SIZE];       //default send buffer
static uint8 s_rec_buf[COM_BUFFER_SIZE];        //

//com receive queue
CycleQueue Input_Q = {
    s_send_buf,
    COM_BUFFER_SIZE,
    0,
    0,
    1,              //empty
    0,         
    0,
};

//com send queue
CycleQueue Output_Q = {
    s_rec_buf,
    COM_BUFFER_SIZE,
    0,
    0,
    1,              //empty
    0,         
    0,
};

/*****************************************************************************/
//  FUNCTION:     QueueClean
//  Description:    clean cycle queue
//  INPUT:      pointer to a CycleQueue
//  OUTPUT:     void
//  Author:         David.Jia
//  date:           2007.6.23
//	Note:       
/*****************************************************************************/
static void QueueClean(CycleQueue *Q_ptr)
{
    Q_ptr->head = Q_ptr->tail = 0;
    Q_ptr->empty = 1;
    Q_ptr->full = 0;
    Q_ptr->overflow = 0;    
}

/*****************************************************************************/
//  FUNCTION:     QueueInsert(CycleQueue *Q_ptr, uint8 *data, uint32 len)
//  Description:    insert a string in cycle queue
//  INPUT:          Q_ptr--pointer to a CycleQueue, uint8 *data--string pointer, uint32 len--string length
//  OUTPUT:         >=0 actual insert number, <0 error
//  Author:         David.Jia
//  date:           2007.6.23
//	Note:        when queue is full, new data wil overwrite olds.
/*****************************************************************************/
static int QueueInsert(CycleQueue *Q_ptr, uint8 *data, uint32 len)
{
    int ret = 0;
#if 0       //no overwrite    
    if (!Q_ptr->full)
    {
        while(ret < len)
        {
            *(Q_ptr->buf + Q_ptr->head) = *(data + ret);
            Q_ptr->head = ++Q_ptr->head % Q_ptr->size;
            ret++;
            
            if (Q_ptr->head == Q_ptr->tail)
            {
                Q_ptr->full = 1;
                break;
            }
        }
    }
    
    if ((1==Q_ptr->empty) && (ret > 0)) 
    {
        Q_ptr->empty = 0;
    }
#else       //overwrite
    for (; ret < len; ret++)
    {
        *(Q_ptr->buf + Q_ptr->head) = *(data + ret);
        
        if ((1==Q_ptr->full) && (Q_ptr->head==Q_ptr->tail))
        {
            Q_ptr->overflow = 1;               
        }
        
        Q_ptr->head = ++Q_ptr->head % Q_ptr->size;
        
        if (Q_ptr->head == Q_ptr->tail)
        {
            Q_ptr->full = 1;
        }
        
        if (1 == Q_ptr->empty)
        {
            Q_ptr->empty = 0;
        }
    }
    
    if (Q_ptr->overflow)
    {
        Q_ptr->tail = Q_ptr->head;
    }
#endif
   
    return ret;       
}


/*****************************************************************************/
//  FUNCTION:     QueueDelete(CycleQueue *Q_ptr, uint8 *data, uint32 len)
//  Description:    delete a string from cycle queue
//  INPUT:          Q_ptr--pointer to a CycleQueue, uint8 *data--string pointer, uint32 len--string length
//  OUTPUT:         >=0 actual delete number, <0 error
//  Author:         David.Jia
//  date:           2007.6.23
//	Note:       
/*****************************************************************************/
static int QueueDelete(CycleQueue *Q_ptr, uint8 *data, uint32 len)
{
    int ret = 0;
    
    if (!Q_ptr->empty)
    {
        while(ret < len) 
        {
            *(data + ret) = *(Q_ptr->buf + Q_ptr->tail);
            Q_ptr->tail = ++Q_ptr->tail % Q_ptr->size;
            ret++;
            
            if (Q_ptr->tail == Q_ptr->head)
            {
                Q_ptr->empty = 1;
                break;
            }
        }
    }
   
    if ((ret>0) && (1==Q_ptr->full))   
    {
        Q_ptr->full = 0;
        Q_ptr->overflow = 0;
    }
    
    return ret;    
}

//@David.Jia 2007.6.23  end

static COM_OBJ com1_ins = {         //static instance of com 1
    0,              //port 0
    s_send_buf,     //default send buffer
    0,              //none to send
    0,              //none has send
    NULL,           //no callback
    s_rec_buf,      //default receive buffer
    0,
    0,
    NULL,
    0               //no error
};

//@David.Jia 2007.6.26  begin
//interface to map application

/*****************************************************************************/
//  FUNCTION:     Map_Read
//  Description:    read string from uart (initialized by COM_Init)
//  INPUT:          uint8* buf--data buffer alocated by caller, uint32 len--bytes number wish to read.
//  OUTPUT:         read byte number in fact.
//  Author:         David.Jia
//  date:           2007.6.26
//	Note:          
/*****************************************************************************/
int Map_Read(uint8 *buf, uint32 len)
{
    int rec;
    COM_OBJ *pCom = &com1_ins;
    
    UART_Rx_Int_Enable(pCom->port, FALSE);
    rec = QueueDelete(&Input_Q, buf, len);
    UART_Rx_Int_Enable(pCom->port, TRUE);
    
    return rec;
}

/*****************************************************************************/
//  FUNCTION:     Map_Write
//  Description:    write string to uart (initialized by COM_Init)
//  INPUT:          uint8* buf--data buffer alocated by caller, uint32 len--bytes number wish to read.
//  OUTPUT:         write byte number in fact.
//  Author:         David.Jia
//  date:           2007.6.26
//	Note:          if len > Output_Q.size, data will overwrite.
/*****************************************************************************/
int Map_Write(uint8 *buf, uint32 len)
{
    int ret;
    COM_OBJ *pCom = &com1_ins;
    
    UART_Tx_Int_Enable(pCom->port, FALSE);
    ret = QueueInsert(&Output_Q, buf, len);
    UART_Tx_Int_Enable(pCom->port, TRUE);
    
    return ret;
}


//@David.Jia 2007.6.26  end
#if 0
uint32 UART_GetRxFifoCnt(uint32 phy_port)
{
	uint32 uart_base[4] = {0x83000000, 0x84000000, 0x8E000000, 0x8F000000};
	
	
	return (*(volatile uint32 *)(uart_base[phy_port] + 0x000C)) & 0xFF;
}
#endif


/*****************************************************************************/
//  FUNCTION:     uart1_callback(uint32 event)
//  Description:    callback function for uartcom_drv.
//  Author:         David.Jia
//  date:           2007.6.19
//	Note:           assume there is still data in when call UART_ReadData.
//      return value will large than called UART_GetRxFifoCnt.
/*****************************************************************************/

uint8 tmp_buf[128+1];
char hex_buf[5];

static void uart1_callback(uint32 event)
{
    uint32 cnt_old, cnt;
    COM_OBJ *pcom = &com1_ins;
    COM_EVENT   status;
    
    //SCI_ASSERT(event < COM_MAX_EVENT);
    //SCI_TRACE_LOW:"[DRV_GPS_COM]<WARNING> uart1_callback: event > COM_MAX_EVENT\r\n"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,UARTCOM_SAMPLE_280_112_2_18_0_33_24_1726,(uint8*)"" );
   
    //SCI_TraceLow("\r\nevent=%d", event);
    //for (cnt = 0; cnt < 10; cnt++)
    //    SCI_TraceLow("\r\n*pcom+%d: %x", cnt, *((uint32*)(pcom)+cnt));
  //  for (cnt = 0; cnt < 5; cnt++)    
   //     SCI_TraceLow("\r\n*Input_Q+%d: %x", cnt, *((uint32*)&Input_Q + cnt));
//    for (cnt = 0; cnt < 5; cnt++)    
  //      SCI_TraceLow("\r\n*Output_Q+%d: %x", cnt, *((uint32*)&Output_Q + cnt));    
    
    switch (event)
    {
        case EVENT_DATA_TO_READ:    {      
            
            for (cnt = 0; cnt < 129; cnt++)     tmp_buf[cnt] = 0;
            for (cnt = 0; cnt < 5; cnt++)   hex_buf[cnt] = 0;
            
            cnt_old = UART_GetRxFifoCnt(pcom->port);            
            cnt = UART_ReadData(pcom->port, tmp_buf, 128);            
            pcom->rec_len_done += cnt;
            
            QueueInsert(&Input_Q, tmp_buf, cnt);
            
           // SCI_TraceLow("\r\nuart1_callback: cnt_old=%d, cnt=%d", cnt_old, cnt);
            /*
            for (cnt_old = 0; cnt_old < cnt; cnt_old++) 
            {
                sprintf(hex_buf, "$%02X", tmp_buf[cnt_old]);
                SCI_TraceLow(hex_buf);
            }
            */
            //UART_WriteData(pcom->port, tmp_buf, cnt);
                       
            break;
        }
        
        case EVENT_WRITE_COMPLETE:  {
            
            for (cnt = 0; cnt < 129; cnt++)     tmp_buf[cnt] = 0;
            cnt = UART_GetTxFifoCnt(pcom->port);
            cnt = QueueDelete(&Output_Q, tmp_buf, cnt);
            UART_WriteData(pcom->port, tmp_buf, cnt);
            
            if (0 == cnt) UART_Tx_Int_Enable(pcom->port, FALSE);
            
            break;    
        }
        
        case EVENT_INIT_COMPLETE:
            break;
        
        case EVENT_SHUTDOWN_COMPLETE:
            break;
        
        default:
            break;            
    }
}


/*****************************************************************************/
//  FUNCTION:     COM_Init
//  Description:    pin select of uart 1, and initialize uart 1.
//  return:         0--ok, others--error.
//  Author:         David.Jia
//  date:           2007.6.19
//	Note:           call before using uart 1.
/*****************************************************************************/
int COM_Init(void)
{
    int ret; 
    
    UART_INIT_PARA_T    uart1_st;
    
    //SCI_TRACE_LOW:"\r\nCOM1_Init."
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,UARTCOM_SAMPLE_350_112_2_18_0_33_24_1727,(uint8*)"");
    
    uart1_st.tx_watermark  = 96; // 0~127B
    uart1_st.rx_watermark  = 64; // 0~127B
    //uart1_st.baud_rate     = BAUD_115200;
    uart1_st.baud_rate     = BAUD_4800;
    uart1_st.parity        = FALSE; 
    uart1_st.parity_enable = PARITY_DISABLE;
    uart1_st.byte_size     = EIGHT_BITS;
    uart1_st.stop_bits     = ONE_STOP_BIT;
    uart1_st.flow_control  = NO_FLOW_CONTROL;
    uart1_st.ds_wakeup_en  = DS_WAKEUP_DISABLE;
    
    com1_ins.port = COM_1;
    
    //assume u1rxd/u1txd have selected in pinmap    
    ret = UART_Initilize(COM_1, &uart1_st, uart1_callback);
    QueueClean(&Input_Q);
    QueueClean(&Output_Q);
    UART_Tx_Int_Enable(COM_1, FALSE);
    
    return ret;
}

/*****************************************************************************/
//  FUNCTION:     COM_Close
//  Description:    pin deselect of uart 1, and close uart 1.
//  return:         0--ok, others--error.
//  Author:         David.Jia
//  date:           2007.6.19
//	Note:           call after using uart 1.
/*****************************************************************************/
int COM_Close(void)
{
    UART_INIT_PARA_T    uart1_st;
    
    //SCI_TRACE_LOW:"\r\nCOM0_Close."
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,UARTCOM_SAMPLE_384_112_2_18_0_33_24_1728,(uint8*)"");
    
    uart1_st.tx_watermark  = 96; // 0~127B
    uart1_st.rx_watermark  = 8; // 0~127B
    //uart1_st.baud_rate     = BAUD_115200;
    uart1_st.baud_rate     = BAUD_57600;
    uart1_st.parity        = FALSE; 
    uart1_st.parity_enable = PARITY_DISABLE;
    uart1_st.byte_size     = EIGHT_BITS;
    uart1_st.stop_bits     = ONE_STOP_BIT;
    uart1_st.flow_control  = NO_FLOW_CONTROL;
    uart1_st.ds_wakeup_en  = DS_WAKEUP_DISABLE;
    
    com1_ins.port = COM_1;
    
    //assume u1rxd/u1txd have selected in pinmap    
    return UART_Close(COM_1);
}