/******************************************************************************
 ** File Name: greeneye2_packet.h                                                       *
 ** Author:                                                     *
 ** DATE:                                                                     *
 ** Copyright:      2002 Spreatrum, Incoporated. All Rights Reserved.         *
 ** Description:                                                              *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE          	NAME             	DESCRIPTION                               *
 **                                   *                                *
 ******************************************************************************/
#ifndef GREENEYE2_PACKET_H_
#define GREENEYE2_PACKET_H_

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "greeneye2_download.h"

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    extern   "C" 
    {
#endif

/**---------------------------------------------------------------------------*
 **                         Data Structures                                   *
 **---------------------------------------------------------------------------*/
typedef enum _PACKET_TYPE {
  BSL_PACKET_TYPE_0 = 0,
  BSL_PACKET_TYPE_1,
  BSL_PACKET_TYPE_2,
} PACKET_TYPE;

#define cpu2be16(wValue) (((wValue & 0xFF) << 8) | ((wValue >> 8) & 0xFF))
#define cpu2be32(dwValue)                                       \
  (((dwValue & 0xFF) << 24) | (((dwValue >> 8) & 0xFF) << 16) | \
  (((dwValue >> 16) & 0xFF) << 8) | (dwValue >> 24))

/**---------------------------------------------------------------------------*
 **                         Function Prototypes                               *
 **---------------------------------------------------------------------------*/
int send_connect_message(GPS_PACKET_T *packet, void *arg);
int send_start_message(GPS_PACKET_T *packet, void *arg);
int send_end_message(GPS_PACKET_T *packet, void *arg);
int send_data_message(GPS_PACKET_T *packet, void *arg);
int send_exec_message(GPS_PACKET_T *packet, void *arg);
int send_uart_speed_message(GPS_PACKET_T *packet, void *arg);
unsigned short frm_chk(const unsigned short *src, int len);
unsigned short boot_checksum(const unsigned char *src, int len);
unsigned short calculate_crc(unsigned short crc, char const *buffer, int len);
int send_to_connect_device(GPS_PACKET_T *packet, void *arg);
void send_ComInit(uint32 baudrate);
int reivce_data_process(int uart_port,int gps_cmd_type);
int send_data_init(BOOLEAN flag);
void GPS_Uart_DmaStart(int uart_port);
void GPS_Uart_DmaClose();
/**--------------------------------------------------------------------------*
**                         Compiler Flag                                     *
**---------------------------------------------------------------------------*/
#ifdef   __cplusplus
}
#endif

#endif  // GREENEYE2_PACKET_H_