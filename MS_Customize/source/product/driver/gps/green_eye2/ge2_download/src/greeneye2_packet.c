/******************************************************************************
 ** File Name:      greeneye2_packet.c                                             *
 ** Author:                                                    *
 ** DATE:                                                          *
 ** Copyright:      2005 Spreadtrum, Incoporated. All Rights Reserved.         *
 ** Description:                             *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                                                                           *
 ** --------------------------------------------------------------------------*
 ** DATE           NAME             DESCRIPTION                               *
 ******************************************************************************/

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "sci_types.h"
#include "sci_api.h"
#include "com_drvapi.h"
#include "cmd_def.h"
#include "greeneye2_packet.h"
#include "greeneye2_crc.h"
#include "greeneye2_download.h"
#include "deepsleep_drvapi.h"
#include "os_api.h"
#include "deep_sleep.h"
#include "com_drv.h"
#include "uartcom_drv.h"
#include "uart_drvapi.h"
#include "sc_reg.h"
#include "tb_drv.h"
#include "os_msg.h"
#include "os_alloc.h"

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    extern   "C" 
    {
#endif
/**---------------------------------------------------------------------------*
 **                         variable section                                  *
 **---------------------------------------------------------------------------*/
typedef struct gps_dluart_tag
{
    uint8 port;
    uint32 tx_dma_channel;
    uint32 rx_dma_channel;   //Not use at present
    volatile uint32  base_addr;
} GPS_DLUART_T;

LOCAL GPS_DLUART_T    s_gps_dluart = {0};

#define GPS_UART_TX_DMA_CHANNEL   s_gps_dluart.tx_dma_channel
#define GPS_UART_BASE_ADDR        s_gps_dluart.base_addr
#define GPS_UART_COM              s_gps_dluart.port

#define BSL_UART_PACKET 0
#define BSL_SPI_PACKET 1

#define COUNTDOWN_START(name, begin, end, timeout)        \
  do {                                                    \
    begin = SCI_GetTickCount();     \
  } while (0)

#define COUNTDOWN_END(name, begin, end, timeout)           \
    {                                                       \
        end = SCI_GetTickCount();                                           \
        if ((end - begin) > (timeout)) { \
            DOWNLOAD_LOGD(("wait  %s  timeout!!!\n",name));         \
            break;                                            \
        }                                                      \
    }

#define WAIT_ACK_START(begin, end) COUNTDOWN_START("ack", begin, end, 1500)
#define WAIT_ACK_END(begin, end) COUNTDOWN_END("ack", begin, end, 1500)
#define WRITE_START(begin, end) COUNTDOWN_START("write", begin, end, 3)
#define WRITE_END(begin, end) COUNTDOWN_END("write", begin, end, 90)
#define WAIT_ACK_TIMEOUT_END(begin, end,timeout) COUNTDOWN_END("ack", begin, end, timeout)

//static unsigned long send_buffer[0x1000] = {0};
uint8 *send_buffer = SCI_NULL;

BOOLEAN dma_send_flag = SCI_TRUE;
/**---------------------------------------------------------------------------*
 **                         LOCAL Function Prototypes                               *
 **---------------------------------------------------------------------------*/
LOCAL void GPS_UartDma_WriteData(uint8 *start_addr, uint32 length);
LOCAL void GPS_TxDma_CallBack(uint32 num);

/**---------------------------------------------------------------------------*
 **                      Function  Definitions
 **---------------------------------------------------------------------------*/
/******************************************************************************
**  Description:    This function scan the data in src buffer ,add 0x7e as begin
**                  and end of src,at the same time replace 0x7E,0x7D with rules
**                  below:
**                        0x7e is replaced by 0x7d and 0x5e;
**                        0x7D is replace 0x7d and 0x5d
**                  the translated data will be save in dest buffer
**  Author:        
**  parameter:      dest is buffer result of process
**                  src is buffer where data is saved
**                  size is size of src data.
******************************************************************************/
static int translate_packet(char *dest, char *src, int size) {
    int i;
    int translated_size = 0;

    dest[translated_size++] = 0x7E;

    for (i = 0; i < size; i++) 
    {
        if (src[i] == 0x7E) 
        {
            dest[translated_size++] = 0x7D;
            dest[translated_size++] = 0x5E;
        } 
        else if (src[i] == 0x7D) 
        {
            dest[translated_size++] = 0x7D;
            dest[translated_size++] = 0x5D;
        } 
        else 
        {
            dest[translated_size++] = src[i];
        }
    }
    dest[translated_size++] = 0x7E;
    return translated_size;
}

/******************************************************************************
**  Description:    This function scan the data in src buffer ,add 0x7e as begin
**                  and end of src,at the same time replace 0x7E,0x7D with rules
**                  below:
**                        0x7e is replaced by 0x7d and 0x5e;
**                        0x7D is replace 0x7d and 0x5d
**                  the translated data will be save in dest buffer
**  Author:        
**  parameter:      dest is buffer result of process
**                  src is buffer where data is saved
**                  size is size of src data.
******************************************************************************/
static int untranslate_packet(char *dest, char *src, int size) {
    int i;
    int translated_size = 0;
    int status = 0;

    for (i = 0; i < size; i++) 
    {
        switch (status) {
            case BSL_PACKET_TYPE_0:
                if (src[i] == 0x7e) 
                    status = 1;
                break;
            case BSL_PACKET_TYPE_1:
                if (src[i] != 0x7e) {
                status = 2;
                dest[translated_size++] = src[i];
                }
                break;
            case BSL_PACKET_TYPE_2:
                if (src[i] == 0x7E) 
                {
                    unsigned short crc;
                    crc = crc_16_l_calc((char *)dest, translated_size - 2);
                    return translated_size;
                } 
                else if (dest[translated_size - 1] == 0x7D) 
                {
                    if (src[i] == 0x5E) 
                    {
                        dest[translated_size - 1] = 0x7E;
                    } 
                    else if (src[i] == 0x5D) 
                    {
                        dest[translated_size - 1] = 0x7D;
                    }
                } 
                else 
                {
                    dest[translated_size++] = src[i];
                }
                break;
        }
    }
    return translated_size;
}

/******************************************************************************
**  Description:    This function setup the download protocol packet
**  Author:        
**  parameter:      msg : msg type
**                  buffer: where packet is saved
**                  data_size: length of message body
**                  packet_type: UART message or SPI message.
******************************************************************************/
int setup_packet(cmd_pkt_type msg, char *buffer, int offset, int data_size,
                 int flag, int image_size) 
{
    struct pkt_header_tag *head;
    int length = sizeof(struct pkt_header_tag) + data_size;
    int total_size = 0;
    unsigned short crc16;

    head = (struct pkt_header_tag *)&buffer[4];
    switch (msg) 
    {
        case BSL_CMD_CONNECT:
            if (flag == BSL_UART_PACKET) 
            {
                head->pkt_type = cpu2be16((unsigned short)msg);
                head->pkt_size = cpu2be16((unsigned short)data_size);
                *((unsigned short *)&buffer[length + 4]) = 0;
                total_size = length + 2;
            } 
            else 
            {
                *((unsigned short *)&buffer[0]) = data_size;
                *((unsigned short *)&buffer[2]) = (unsigned short)msg;
                *((unsigned short *)&buffer[8]) = 0;
                *((unsigned short *)&buffer[4]) = boot_checksum((const unsigned char *)buffer + offset, data_size);
                *((unsigned short *)&buffer[6]) = boot_checksum((const unsigned char *)buffer, 6);
                total_size = 12;
            }
            break;
        case BSL_CMD_START_DATA:
            if (flag == BSL_UART_PACKET) 
            {
                head->pkt_type = cpu2be16((unsigned short)msg);
                head->pkt_size = cpu2be16((unsigned short)data_size);
                crc16 = crc_16_l_calc((char *)head, length);
                *((unsigned short *)&buffer[length + 4]) = cpu2be16(crc16);
                total_size = length + 2;
            } 
            else 
            {
                *((unsigned short *)&buffer[0]) = data_size;
                *((unsigned short *)&buffer[2]) = (unsigned short)msg;
                *((unsigned short *)&buffer[4]) = boot_checksum((const unsigned char *)buffer + offset, data_size);
                *((unsigned short *)&buffer[6]) = boot_checksum((const unsigned char *)buffer, 6);
                total_size = data_size + 8;
            }
            break;
        case BSL_CMD_MIDST_DATA:
            if (flag == BSL_UART_PACKET) 
            {
                head->pkt_type = cpu2be16((unsigned short)msg);
                head->pkt_size = cpu2be16((unsigned short)data_size);
                crc16 = crc_16_l_calc((char *)head, length);
                *((unsigned short *)&buffer[length + 4]) = cpu2be16(crc16);
                total_size = length + 2;
            } 
            else 
            {
                if (data_size == 0)
                    *((unsigned short *)&buffer[0]) = 4;
                else
                    *((unsigned short *)&buffer[0]) = image_size & 0xffff;
                *((unsigned short *)&buffer[2]) = (unsigned short)msg | ((image_size & 0xff0000) >> 8);
                *((unsigned short *)&buffer[4]) = 0;
                *((unsigned short *)&buffer[6]) = boot_checksum((const unsigned char *)buffer, 6);
                total_size = data_size + 8;
            }
            break;
        case BSL_CMD_END_DATA:
            if (flag == BSL_UART_PACKET) 
            {
                head->pkt_type = cpu2be16((unsigned short)msg);
                head->pkt_size = cpu2be16((unsigned short)data_size);
                crc16 = crc_16_l_calc((char *)head, length);
                *((unsigned short *)&buffer[length + 4]) = cpu2be16(crc16);
                total_size = length + 2;
            } 
            else 
            {
                *((unsigned short *)&buffer[0]) = 4;
                *((unsigned short *)&buffer[2]) = (unsigned short)msg;
                *((unsigned short *)&buffer[4]) = boot_checksum((const unsigned char *)(buffer + offset), data_size);
                *((unsigned short *)&buffer[6]) = boot_checksum((const unsigned char *)buffer, 6);
                *((unsigned int *)&buffer[8]) = 0;
                total_size = 12;
            }
            break;
        case BSL_CMD_EXEC_DATA:
            if (flag == BSL_UART_PACKET) 
            {
                head->pkt_type = cpu2be16((unsigned short)msg);
                head->pkt_size = cpu2be16((unsigned short)data_size);
                crc16 = crc_16_l_calc((char *)head, length);
                *((unsigned short *)&buffer[length + 4]) = cpu2be16(crc16);
                total_size = length + 2;
            } 
            else 
            {
                *((unsigned short *)&buffer[0]) = data_size;
                *((unsigned short *)&buffer[2]) = (unsigned short)msg;
                *((unsigned short *)&buffer[4]) = boot_checksum((const unsigned char *)(buffer + offset), data_size);
                *((unsigned short *)&buffer[6]) = boot_checksum((const unsigned char *)buffer, 6);
                total_size = data_size + 8;
            }
            break;
        case BSL_CMD_NORMAL_RESET:
            if (flag == BSL_UART_PACKET) 
            {
                head->pkt_type = cpu2be16((unsigned short)msg);  // 2 bytes of CMD
                head->pkt_size = cpu2be16((unsigned short)data_size);  // 2 bytes of message
                // length(the length of
                // message body!!!)
                crc16 = crc_16_l_calc((char *)head, length);  // see above: length = sizeof(struct
                // pkt_header_tag)+data_size;
                *((unsigned short *)&buffer[length + 4]) = cpu2be16(crc16);
                total_size = length + 2;  // add 2 bytes of CRC to total_size
                // the total_size is 10
            } 
            else 
            {
                *((unsigned short *)&buffer[0]) = data_size;
                *((unsigned short *)&buffer[2]) = (unsigned short)msg;
                *((unsigned short *)&buffer[4]) = boot_checksum((const unsigned char *)(buffer + offset), data_size);
                *((unsigned short *)&buffer[6]) = boot_checksum((const unsigned char *)buffer, 6);
                total_size = data_size + 8;
            }
            break;
        default:
            break;
    }
    return total_size;
}

/******************************************************************************
**  Description:    This function setup connect message by uart
**  Author:         
**  parameter:      none
******************************************************************************/
int send_connect_message(GPS_PACKET_T *packet, void *arg)
{
    char raw_buffer[32] = {0};
    int size;
    int translated_size;
    int retval;
    uint32 begin, end;
    int uart_port = packet->uart_port;
    int flag = packet->flag;

    size = setup_packet(BSL_CMD_CONNECT, raw_buffer, 8, 0, flag, 0);
    translated_size = translate_packet((char *)send_buffer, (char *)&raw_buffer[4], size);
    WRITE_START(begin, end);
    do {
        retval = UART_WriteDataEx(uart_port, send_buffer, translated_size,SCI_FALSE);
        if (retval > 0) 
        {
            translated_size -= retval;
        } 
        else 
        {
            DOWNLOAD_LOGD(("[GPS_DL]warning: write error,try again\n"));
            SCI_Sleep(300);
         }
        WRITE_END(begin, end);
    } while (translated_size > 0);
    if (translated_size <= 0)
    {
        DOWNLOAD_LOGD(("[GPS_DL]send connect message ok\n"));
        return GPS_DL_SUCCESS;
    }
    else
    {
        return GPS_DL_FAILURE;
    }
}
/******************************************************************************
**  Description:    This function setup data start message by uart
**  Author:        
**  parameter:      size : size of image to be sent
**                  addr : address where image to be saved in MODEM
******************************************************************************/
int send_start_message(GPS_PACKET_T *packet, void *arg)//(int uart_port, int size, unsigned long addr, int flag) 
{
    char raw_buffer[32] = {0};
    char *data = raw_buffer;
    int translated_size;
    int offset;
    int retval;
    struct pkt_header_tag head;
    uint32 begin, end;
    int uart_port = packet->uart_port;
    unsigned long addr = packet->gps_image_info[packet->mode].address;
    int size  = packet->data_size;
    int flag = packet->flag;

    *(unsigned long *)&raw_buffer[8] = cpu2be32(addr);
    *(unsigned long *)&raw_buffer[12] = cpu2be32(size);
    size = setup_packet(BSL_CMD_START_DATA, raw_buffer, 8, 8, flag, 0);
    if (flag == 0) 
    {
        translated_size = translate_packet((char *)send_buffer, (char *)&raw_buffer[4], size);
        WRITE_START(begin, end);
        do {
            retval = UART_WriteDataEx(uart_port, send_buffer, translated_size,SCI_FALSE);
            if (retval > 0) 
            {
                translated_size -= retval;
            } 
            else 
            {
                DOWNLOAD_LOGD(("[GPS_DL]warning: write error,try again\n"));
                SCI_Sleep(300);
            }
            WRITE_END(begin, end);
        } while (translated_size > 0);
        if (translated_size <= 0) 
            DOWNLOAD_LOGD(("[GPS_DL]send start message ok\n"));
    } 

    if (translated_size <= 0)
    {
        DOWNLOAD_LOGD(("[GPS_DL]send start message ok\n"));
        return GPS_DL_SUCCESS;
    }
    else
    {
        return GPS_DL_FAILURE;
    }
}

/******************************************************************************
**  Description:    This function setup data end message by uart
**  Author:         
**  parameter:      none
******************************************************************************/
int send_end_message(GPS_PACKET_T *packet, void *arg)
{
    char raw_buffer[32] = {0};
    char *data = raw_buffer;
    int size;
    int translated_size;
    int offset;
    int retval;
    struct pkt_header_tag head;
    uint32 begin, end;
    int uart_port = packet->uart_port;
    int flag = packet->flag;

    size = setup_packet(BSL_CMD_END_DATA, raw_buffer, 8, 0, flag, 0);
    if (flag == 0) 
    {
        translated_size = translate_packet((char *)send_buffer, (char *)&raw_buffer[4], size);
        WRITE_START(begin, end);
        do {
            retval = UART_WriteDataEx(uart_port, send_buffer, translated_size,SCI_FALSE);
            if (retval > 0) 
            {
                translated_size -= retval;
            } 
            else 
            {
                DOWNLOAD_LOGD(("[GPS_DL]warning: write error,try again\n"));
                SCI_Sleep(300);
            }
            WRITE_END(begin, end);
        } while (translated_size > 0);
    } 

    if (translated_size <= 0)
    {
        DOWNLOAD_LOGD(("[GPS_DL]send end message ok\n"));
        return GPS_DL_SUCCESS;
    }
    else
    {
        return GPS_DL_FAILURE;        
    }
}

/******************************************************************************
**  Description:    This function setup data message by uart
**  Author:         
**  parameter:      none
******************************************************************************/
int send_data_message(GPS_PACKET_T *packet, void *arg)//(int uart_port, char *buffer, int data_size, int flag,
                     // int image_size, int image_fd) 
{
    char raw_buffer[32] = {0};
    char *uart_buffer = NULL;//[3000]= {0};
    char *data = raw_buffer;
    int size = 0;
    int translated_size = 0;
    int offset = 0;
    int retval = 0;
    struct pkt_header_tag head;
    uint32 begin, end;
    int uart_port = packet->uart_port;
    char *buffer = packet->data_buffer;
    int data_size = packet->data_size;
    int flag = packet->flag;
    char *uart_send_buffer = NULL;
    
    uart_buffer = packet->uart_raw_buffer;
    uart_send_buffer = (char *)send_buffer;
    memset(uart_buffer, 0, DL_MALLOC_BUFFSER_SIZE);
    memset(uart_send_buffer, 0, DL_MALLOC_BUFFSER_SIZE);
    if (flag == 0) 
    {
        memcpy(&uart_buffer[8], buffer, data_size);
        size = setup_packet(BSL_CMD_MIDST_DATA, uart_buffer, 8, data_size, flag, 0);
        translated_size = translate_packet((char *)uart_send_buffer, (char *)&uart_buffer[4], size);
        WRITE_START(begin, end);
        data = (char *)uart_send_buffer;
        do {
            DOWNLOAD_LOGD(("[GPS_DL]send_data_message0 translated_size=%d\n", translated_size));
            if(dma_send_flag)
                GPS_UartDma_WriteData((char *)(data+offset),translated_size);
            retval = translated_size;
            if (retval > 0) 
            {
                translated_size -= retval;
                offset += retval;

            } 
            else 
            {
                DOWNLOAD_LOGD(("[GPS_DL]warning: write error,try again\n"));
                SCI_Sleep(300);
            }
        } while (translated_size > 0);
    }
    if (translated_size <= 0)
    {
        DOWNLOAD_LOGD(("[GPS_DL]send data message ok\n"));
        return GPS_DL_SUCCESS;
    }
    else
    {
        return GPS_DL_FAILURE;
    }
}

/******************************************************************************
**  Description:    This function setup execute message by uart
**  Author:         
**  parameter:      addr: address MODEM start to run.
******************************************************************************/
int send_exec_message(GPS_PACKET_T *packet, void *arg)//(int uart_port, unsigned long addr, int flag) 
{
    char raw_buffer[32] = {0};
    char *data = raw_buffer;
    int size;
    int translated_size;
    int offset;
    int retval;
    struct pkt_header_tag head;
    uint32 begin, end;
    int uart_port = packet->uart_port;
    unsigned long addr = packet->gps_image_info[packet->mode].jump_address;
    int flag = packet->flag;

    *(unsigned long *)&raw_buffer[8] = cpu2be32(addr);
    size = setup_packet(BSL_CMD_EXEC_DATA, raw_buffer, 8, 4, flag, 0);
    if (flag == 0) 
    {
        translated_size = translate_packet((char *)send_buffer, (char *)&raw_buffer[4], size);
        WRITE_START(begin, end);
        do {
          retval = UART_WriteDataEx(uart_port, send_buffer, translated_size,SCI_FALSE);
          if (retval > 0) 
          {
            translated_size -= retval;
          } 
          else 
          {
            DOWNLOAD_LOGD(("[GPS_DL]warning: write error,try again\n"));
            SCI_Sleep(300);
          }
          WRITE_END(begin, end);
        } while (translated_size > 0);

    } 
    if (translated_size <= 0) 
    {
        DOWNLOAD_LOGD(("[GPS_DL]send exec message ok\n"));
        return GPS_DL_SUCCESS;
    } 
    else
    {
        return GPS_DL_FAILURE;
    }
}

/******************************************************************************
**  Description:    This function setup uart speed by uart
**  Author:         
**  parameter:      
******************************************************************************/
int send_uart_speed_message(GPS_PACKET_T *packet, void *arg)
{
    char raw_buffer[32] = {0};
    char *data = raw_buffer;
    int size;
    int translated_size;
    int retval;
    int uart_port = packet->uart_port;
    int flag = packet->flag;
    int speed = packet->uart_speed;
    uint32 begin, end;
    *(unsigned long *)&raw_buffer[8] = cpu2be32(speed);
    size = setup_packet(BSL_CMD_NORMAL_RESET, raw_buffer, 8, 4, flag, 0);
    if (flag == 0) 
    {
        translated_size = translate_packet((char *)send_buffer, (char *)&raw_buffer[4], size);
        WRITE_START(begin, end);
        do {
            retval = UART_WriteDataEx(uart_port, send_buffer, translated_size,SCI_FALSE);
            if (retval > 0) 
                translated_size -= retval;
            WRITE_END(begin, end);
        } while (translated_size > 0);
        DOWNLOAD_LOGD(("[GPS_DL]write  success retval=%d\n", retval));
    } 
    else 
    {
        retval = UART_WriteDataEx(uart_port, raw_buffer, size,SCI_FALSE);
    }

    if (translated_size <= 0) 
    {
        #if 0
        DOWNLOAD_LOGD(("[GPS_DL]set AP uart speed = %ld\n", speed));
        SCI_Sleep(1);
        if(uart_port)
            *((volatile uint32 *)(ARM_UART1_BASE + ARM_UART_CLKD0))= (0xD & 0x0000ffff);
        else
            *((volatile uint32 *)(ARM_UART0_BASE + ARM_UART_CLKD0))= (0xD & 0x0000ffff);
        SCI_Sleep(1);
        #endif
        return GPS_DL_SUCCESS;
    } 
    else
    {
        return GPS_DL_FAILURE;
    }
}

/******************************************************************************
**  Description:    send_to_connect_device
**  Author:         
**  parameter:      
******************************************************************************/
int send_to_connect_device(GPS_PACKET_T *packet, void *arg)
{
    unsigned long hand_shake = 0x7E7E7E7E;
    int  ret;
    uint32 begin, end;
    int uart_port = packet->uart_port;
    
    DOWNLOAD_LOGD(("[GPS_DL]try to connect device......\n"));

    begin = SCI_GetTickCount();
    
    for (;;)
    {
        end = SCI_GetTickCount();

        if ((end - begin) >= 2000) 
        {
            DOWNLOAD_LOGE(("[GPS_DL]try to connect device timeout\n"));
            return GPS_DL_FAILURE;
        }

        ret = UART_WriteDataEx(uart_port, &hand_shake, sizeof(hand_shake),SCI_FALSE);
        if (ret == 0) 
        {
            DOWNLOAD_LOGE(("[GPS_DL]UART Send HandShake error %d\n", ret));
            continue;
        }
        else
            return GPS_DL_SUCCESS;
    }
    return GPS_DL_FAILURE;
}

/******************************************************************************
**  Description:    reivce_data_process
**  Author:         
**  parameter:      
******************************************************************************/
int reivce_data_process(int uart_port,int gps_cmd_type)
{
    int i, ret;
    char buffer[64] = {0};
    char *data = (char *)buffer;
    int size = 0;
    uint32 begin,end;
    int count = 0;
    DOWNLOAD_LOGD(("[GPS_DL]reivce_data_process\n"));
    
    //set uart speed do not check
    if(0x1 ==gps_cmd_type)
    {
	if(uart_port)
            *((volatile uint32 *)(ARM_UART1_BASE + ARM_UART_CLKD0))= (0xD & 0x0000ffff);
        else
            *((volatile uint32 *)(ARM_UART0_BASE + ARM_UART_CLKD0))= (0xD & 0x0000ffff);
        SCI_Sleep(1);
	return GPS_DL_SUCCESS;
    }
    ret = UART_ReadData(uart_port, data, 64);
    
    if (data[0] == 0x7E) 
    {
        if(gps_cmd_type > 0)
            ret = 8;
        for(i = 1;i < ret;i++)
        {
            DOWNLOAD_LOGD(("[GPS_DL]data received:data[%d] = 0x%x",i,data[i]));
            if (*(data+i) == 0x7E) 
            {
                DOWNLOAD_LOGD(("[GPS_DL]data string received end"));
                return GPS_DL_SUCCESS;
            }
        } 
    }


    for(i = 0;i < ret;i++)
        {
            DOWNLOAD_LOGD(("[GPS_DL]data received:data[%d] = 0x%x",i,data[i]));
        } 
    DOWNLOAD_LOGD(("[GPS_DL]reivce_data_process fail ret = %d\n",ret));
    return GPS_DL_FAILURE;
}

/******************************************************************************
**  Description:    send_data_init
**  Author:         
**  parameter:      
******************************************************************************/
int send_data_init(BOOLEAN flag)
{
    uint8 ret = 0;
    if(flag)
    {
        send_buffer = SCI_ALLOC(DL_MALLOC_BUFFSER_SIZE);
        if(NULL == send_buffer)
            return 0;
    }
    else
    {
        ret = SCI_FREE(send_buffer);
        if(ret != 0)
            return 0;
    }
    return 1;
}

/******************************************************************************
**  Description:    GPS_TxDma_CallBack
**  Author:         
**  parameter:      
******************************************************************************/
LOCAL void GPS_TxDma_CallBack(uint32 num)
{
	dma_send_flag = SCI_TRUE;
    DOWNLOAD_LOGD(("[GPS_DL]GPS_TxDma_CallBack\n"));
    DMA_HAL_ChnEnable(GPS_UART_TX_DMA_CHANNEL, SCI_FALSE);        // s_os_dma_int++;
    //DPSLP_UART_EnableApbSleep (GPS_UART_COM, ENABLE_APB_SLEEP); 
}

/******************************************************************************
**  Description:    GPS_UartDma_WriteData
**  Author:         
**  parameter:      
******************************************************************************/
LOCAL void GPS_UartDma_WriteData(uint8 *start_addr, uint32 length)
{
		dma_send_flag = SCI_FALSE;
        //Enable Uart Dma
        UART_DmaModeEnable(GPS_UART_COM, TRUE);

        //DMA_HAL_ChnEnable(GPS_UART_TX_DMA_CHANNEL, SCI_FALSE);
        DMA_HAL_ChnIntConfig(GPS_UART_TX_DMA_CHANNEL, DMA_CHN_INT_TYPE_TRANSDONE, GPS_TxDma_CallBack);
	
        UART_DmaWriteData(GPS_UART_COM, start_addr, length, GPS_UART_TX_DMA_CHANNEL);

        //DPSLP_UART_EnableApbSleep (GPS_UART_COM, DISABLE_APB_SLEEP);  //APB Should not sleep when use DMA

        
}

/******************************************************************************
**  Description:    GPS_Uart_DmaStart
**  Author:         
**  parameter:      
******************************************************************************/
void GPS_Uart_DmaStart(int uart_port)
{
    uint32 ret = 0xFFFFFFFF;
    s_gps_dluart.port = uart_port;

    ret = DMA_HAL_AllocChn(DMA_CHN_TRSF_TYPE_STANDARD);  
    UART_DMA_CONNECT(uart_port, UART_DMA_TX_MODE, ret);
    s_gps_dluart.tx_dma_channel = ret;
    DPSLP_UART_EnableApbSleep (GPS_UART_COM, DISABLE_APB_SLEEP);  //APB Should not sleep when use DMA
}

/******************************************************************************
**  Description:    GPS_Uart_DmaClose
**  Author:         
**  parameter:      
******************************************************************************/
void GPS_Uart_DmaClose()
{
    DMA_HAL_ChnEnable (GPS_UART_TX_DMA_CHANNEL,SCI_FALSE);
    UART_DMA_CONNECT(GPS_UART_COM, UART_DMA_TX_MODE, 0);	
    DMA_HAL_FreeChn(GPS_UART_TX_DMA_CHANNEL);  
    DPSLP_UART_EnableApbSleep (GPS_UART_COM, ENABLE_APB_SLEEP);
}

/**--------------------------------------------------------------------------*
**                         Compiler Flag                                     *
**---------------------------------------------------------------------------*/
#ifdef   __cplusplus
}
#endif

/*  End Of File */

