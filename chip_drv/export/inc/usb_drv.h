/******************************************************************************
 ** File Name:    usb_drv.h                                                    *
 ** Author:       Daniel.Ding                                                 *
 ** DATE:         3/25/2005                                                    *
 ** Copyright:    2005 Spreadtrum, Incoporated. All Rights Reserved.           *
 ** Description:                                                              *
 ******************************************************************************/
/******************************************************************************
 **                   Edit    History                                         *
 **---------------------------------------------------------------------------*
 ** DATE          NAME            DESCRIPTION                                 *
 ** 3/25/2005      Daniel.Ding     Create.                                     *
 ******************************************************************************/
#ifndef _USB_DRV_H_
#define _USB_DRV_H_
/*----------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **-------------------------------------------------------------------------- */

//#include "sci_types.h"
#include "os_api.h"
#include "usb200.h"
#include "isr_drvapi.h"
//#include "tb_hal.h"
/**---------------------------------------------------------------------------*
 **                             Compiler Flag                                 *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
extern   "C"
{
#endif
/**----------------------------------------------------------------------------*
**                               Micro Define                                 **
**----------------------------------------------------------------------------*/
#define USB_SPREADTRUM_VENDER_ID            0x1782
#define USB_SPREADTRUM_CHIP_ID              0x4d00
#define USB_SPREADTRUM_PID_TWOINTERFACE     0x3d00
#define USB_SPREADTRUM_VCOM_MASS_STORAGE_ID        0x3d01  // at+diag+udisk
#define USB_SPREADTRUM_CDROM_MODEM_ID       0x3d02  // modem + cdrom
#define USB_SPREADTRUM_DATA_DEBUG_MODEM_ID  0x3d03  // modem + cp trace + diag + at
#define USB_SPREADTRUM_CALIB_ID             0x4d02  // calib u2s diag

#define USB_SPREADTRUM_MASS_STORAGE_ID    0x4d01
#define USB_SPREADTRUM_VCOM_ID            0x4d00
#define USB_SPREADTRUM_PC_CAMERA_ID       0x4d04



//#define USB_MICROSOFT_PID_TWOINTERFACE    0x0001    // at, diag

#if defined CHIP_ENDIAN_LITTLE && !defined _BIG_ENDIAN
#define USB_WORD_SWAP_ENDIAN(x)   ((x)&0xFFFF)
#else
#define USB_WORD_SWAP_ENDIAN(x)   (((((x) & 0x0FF) << 8) | (((x) >> 8) & 0x0FF))&0xFFFF) /*lint !e572*/
#endif

#define USB_DWORD_SWAPENDIAN(x){\
        (((x & 0x0FF00)<< 8)|((x & 0x0FF)<<24) |\
         ((x & 0xFF000000)>>24)|((x & 0xFF0000)>>8))}


/**----------------------------------------------------------------------------*
**                             Data Prototype                                 **
**----------------------------------------------------------------------------*/
typedef struct  usb_isr_tag
{
    TB_ISR     usb_irq;
    TB_ISR     dma_epin;
    TB_ISR     dma_epout;
    TB_ISR     dma_ep1;
    TB_ISR     dma_ep2;
    TB_ISR     dma_ep3;
    TB_ISR     dma_ep4;
} USB_ISR_PTR_T;

typedef enum
{
    USB_SUCCESS = 0,
    USB_CONNECTING,
    USB_OTHER
} USB_ERROR_E;

typedef union
{
    struct
    {
#ifdef CHIP_ENDIAN_LITTLE
        uint   index   :8;
        uint   type    :8;
#else
        uint   type    :8;
        uint   index   :8;
#endif
    } bmDescriptor;
    short wValue;
} USB_HWORD_VALUE_U;

typedef enum
{
    USB_DEFAULT_POWER = 0,
    USB_BUS_POWER     = 0,
    USB_SELF_POWER    = 1
} USB_POWER_E;

typedef enum
{
    USB_ERR_INACK_TIMEOUT   = 0x0,
    USB_ERR_INVALID_CLASSCMD,
    USB_ERR_INVALID_VENDORCMD,
    USB_ERR_BSD_GETDEV_INFO,
    USB_ERR_BSD_OPENDEV    ,
    USB_ERR_BSD_CLOSEDEV   ,
    USB_ERR_PARAM_ERR ,
    USB_ERR_MAX
} USB_ERROR_TYPE_E;

/**----------------------------------------------------------------------------*
**                         Local Function Prototype                           **
**----------------------------------------------------------------------------*/

/**----------------------------------------------------------------------------*
**                           Function Prototype                               **
**----------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description:
//  Global resource dependence:
//  Author:         Daniel.Ding
//  Note:
/*****************************************************************************/
PUBLIC uint16 USB_WordEndianSwap (uint16 wWord);

/*****************************************************************************/
//  Description:
//  Global resource dependence:
//  Author:         Daniel.Ding
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN USB_FIFODWordBufEndianSwap (uint32 *src ,uint32 *des ,uint32 len);

/*****************************************************************************/
//  Description:
//  Global resource dependence:
//  Author:         Daniel.Ding
//  Note:
/*****************************************************************************/
PUBLIC uint32 USB_DWordEndianSwap (uint32 dWord);
/*****************************************************************************/
//  Description:
//  Global resource dependence:
//  Author:         Daniel.Ding
//  Note:           len is the number of word !
/*****************************************************************************/
//LCOAL BOOLEAN USB_DWordBufEndianSwap (uint32 *src ,uint32 *des ,uint32 len);
/*****************************************************************************/
//  Description:
//  Global resource dependence:
//  Author:         Daniel.Ding
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN USB_RxDWordFromFIFO (uint32 *buf ,uint8 len ,uint8 ep_id);

/*****************************************************************************/
//  Description:
//  Global resource dependence:
//  Author:         Daniel.Ding
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN USB_TxDWordToFIFO (uint32 *buf ,uint8 len ,uint8 ep_id);

/*****************************************************************************/
//  Description:
//  Global resource dependence:
//  Author:         Daniel.Ding
//  Note:
/*****************************************************************************/
PUBLIC void USB_Connect (void);
/*****************************************************************************/
//  Description:
//  Global resource dependence:
//  Author:         Daniel.Ding
//  Note:
/*****************************************************************************/
PUBLIC void USB_Disconnect (void);

/*****************************************************************************/
//  Description:
//  Global resource dependence:
//  Author:         Daniel.Ding
//  Note:
/*****************************************************************************/
PUBLIC void USB_EnableIrq (void);
/*****************************************************************************/
//  Description:
//  Global resource dependence:
//  Author:         Daniel.Ding
//  Note:
/*****************************************************************************/
PUBLIC void USB_DisableIrq (void);

/*****************************************************************************/
//  Description:    This function reset the usb clock
//  Global resource dependence:
//  Author:         Daniel.Ding
//  Note:
/*****************************************************************************/
PUBLIC void USB_Reset (void);

/*****************************************************************************/
//  Description:
//  Global resource dependence:
//  Author:         Daniel.Ding
//  Note:
/*****************************************************************************/
PUBLIC ISR_EXE_T USB_irq (uint32 param);

/*****************************************************************************/
//  Description:
//  Global resource dependence:
//  Author:         Daniel.Ding
//  Note:
/*****************************************************************************/
PUBLIC void USB_dma_epin (uint32 param);

/*****************************************************************************/
//  Description:
//  Global resource dependence:
//  Author:         Daniel.Ding
//  Note:
/*****************************************************************************/
PUBLIC void USB_dma_epout (uint32 param);

/*****************************************************************************/
//  Description:
//  Global resource dependence:
//  Author:         Daniel.Ding
//  Note:
/*****************************************************************************/
PUBLIC void USB_dma_ep1 (uint32 param);

/*****************************************************************************/
//  Description:
//  Global resource dependence:
//  Author:         Daniel.Ding
//  Note:
/*****************************************************************************/
PUBLIC void USB_dma_ep2 (uint32 param);


/*****************************************************************************/
//  Description:
//  Global resource dependence:
//  Author:         Daniel.Ding
//  Note:
/*****************************************************************************/
PUBLIC void USB_dma_ep3 (uint32 param);

/*****************************************************************************/
//  Description:
//  Global resource dependence:
//  Author:         Daniel.Ding
//  Note:
/*****************************************************************************/
PUBLIC void USB_dma_ep4 (uint32 param);


/*****************************************************************************/
//  Description:    Call this function to control epx data out ;
//  Global resource dependence:
//  Author:         Daniel.Ding
//  Note:
/*****************************************************************************/
PUBLIC void USB_EPxDataOutCtrl (BOOLEAN ctrl,uint32 ep_id);
/*****************************************************************************/
//  Description:    Call this function to control epx data in ;
//  Global resource dependence:
//  Author:         Daniel.Ding
//  Note:
/*****************************************************************************/
PUBLIC void USB_EPxDataInCtrl (BOOLEAN ctrl,uint32 ep_id);

/*****************************************************************************/
//  Description:    This function set usb is self-power or bus-power
//  Global resource dependence:
//  Author:         Daniel.Ding
//  Note:
/*****************************************************************************/
PUBLIC void USB_PowerSet (USB_POWER_E power);
/*****************************************************************************/
//  Description:    Call this function to control epx data out ;
//  Global resource dependence:
//  Author:         Daniel.Ding
//  Note:
/*****************************************************************************/
PUBLIC void USB_EPxFIFOReset (uint32 ep_id);

/*****************************************************************************/
//  Description:    This function is called when usb protctol error occurs
//  Global resource dependence:
//  Author:         weihua.wang
//  Note:           value: usb error type
/*****************************************************************************/
PUBLIC void USB_ErrorHandle (USB_ERROR_TYPE_E value);

/*****************************************************************************/
//  Description:
//  Global resource dependence:
//  Author:         Daniel.Ding
//  Note:           This function will expend 30 ms so it can't be used in
//                  interrupt handler ;
/*****************************************************************************/
PUBLIC void USB_ClkReset (void);

/*****************************************************************************/
//  Description:
//  Global resource dependence:
//  Author:         Daniel.Ding
//  Note:
/*****************************************************************************/
PUBLIC uint32 USB_EP1InDmaReq (uint32 *pBuf,uint32 len);

/*****************************************************************************/
//  Description:
//  Global resource dependence:
//  Author:         Daniel.Ding
//  Note:
/*****************************************************************************/
PUBLIC uint32 USB_EP2OutDmaReq (uint32 *pBuf,uint32 len);

/*****************************************************************************/
//  Description:
//  Global resource dependence:
//  Author:         Liangwen.zhen
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN USB_DisableAllEndPointDMA (void);

PUBLIC void USB_LDO_Control (BOOLEAN flag);

/**----------------------------------------------------------------------------*
**                         Compiler Flag                                      **
**----------------------------------------------------------------------------*/
#ifdef   __cplusplus
}
#endif
/**---------------------------------------------------------------------------*/
#endif
// End
