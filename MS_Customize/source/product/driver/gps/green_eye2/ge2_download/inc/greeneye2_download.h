/******************************************************************************
 ** File Name:   greeneye2_download.h                                                    *
 ** Author:                                                       *
 ** DATE:                                                           *
 ** Copyright:    2005 Spreatrum, Incoporated. All Rights Reserved.           *
 ** Description:                                                              *
 *****************************************************************************/
/******************************************************************************
 **                   Edit    History                                         *
 **---------------------------------------------------------------------------* 
 ** DATE            NAME            DESCRIPTION                                 * 
 *****************************************************************************/
 
 #ifndef _GREENEYE2_DOWNLOAD_H
 #define _GREENEYE2_DOWNLOAD_H

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include"gps_drv.h"

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
extern "C"
{
#endif

/**---------------------------------------------------------------------------*
 **                         Data Structures                                   *
 **---------------------------------------------------------------------------*/
#define GE2_DOWNLOAD_DEBUG				0
#if  GE2_DOWNLOAD_DEBUG
#define DOWNLOAD_LOGD(_x_) SCI_TraceLow _x_
#define DOWNLOAD_LOGE(_x_) SCI_TraceLow _x_
#else
#define DOWNLOAD_LOGD(_x_)  ((void)0)
#define DOWNLOAD_LOGE(_x_)  ((void)0)
#endif

#define DL_PACKET_SIZE 2560
#define DL_MALLOC_BUFFSER_SIZE 3000

typedef struct _NV_HEADER {
  unsigned int magic;
  unsigned int len;
  unsigned int checksum;
  unsigned int version;
} nv_header_t;

struct image_info {
  unsigned char image_type;
  unsigned char *image_data;
  unsigned int image_size;
  unsigned int address;
  unsigned int jump_address;
};

typedef struct pmanager {
  BOOLEAN flag_connect;
  BOOLEAN flag_reboot;
  BOOLEAN flag_start;
  BOOLEAN flag_stop;
  BOOLEAN flag_dump;
} pmanager_t;

typedef enum _GPS_DOWNLOAD_STATE {
    GPS_DL_INIT,
    GPS_DL_START,
    GPS_DL_RUNNING,
    GPS_DL_FREE,
    GPS_DL_FINISH,
    GPS_DL_SUCCESS,
    GPS_DL_FAILURE,
} GPS_DL_STA_E;

#define GPS_DL_TIMEOUT (3000)  // (200) //ms
#define COM_BUFFER_SIZE 1048        //default com send/receive buffer size
#define GPS_DL_TX_WATER_MARK	127//8
#define GPS_DL_RX_WATER_MARK	127//48
#define GPS_GET_DELAY       50      //

typedef struct GPS_PACKET_tag
{
    int uart_port;
    int uart_speed;
    GPS_DL_STA_E download_state;
    int gps_cmd_type;
    char *data_buffer;
    char *uart_raw_buffer;
    char *uart_send_buffer;
    int     data_size;
    int translated_size;
    int     flag;
    GPS_MODE_E mode;
    struct image_info *gps_image_info;

} GPS_PACKET_T;

/**---------------------------------------------------------------------------*
 **                         Function Prototypes                               *
 **---------------------------------------------------------------------------*/
int GPS_DLGreenEye2();
 unsigned int GPS_DLGetImage(unsigned char **p_gps_image_ptr,unsigned char image_type); 
 PUBLIC void GPS_TaskInit(void);
/**--------------------------------------------------------------------------*
**                         Compiler Flag                                     *
**---------------------------------------------------------------------------*/
#ifdef   __cplusplus
}
#endif

 #endif
