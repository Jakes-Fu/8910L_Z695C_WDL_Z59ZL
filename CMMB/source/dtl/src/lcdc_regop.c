
#include "mtv_cfg.h"
#include "mtv_ctrl.h"
#include "lcmif_drv.h"
#include "dtl.h"

#define INTV_REG_BASE               0x20200000
#define INTCV_IRQ_PROTECT           (INTV_REG_BASE + 0x0020)
#define INTCV_INT_SEL               (INTV_REG_BASE + 0x000c)
#define INTCV_INT_DIS               (INTV_REG_BASE + 0x0014)

#define AHBV_REG_BASE               0x20300000
#define AHBV_SOFT_RST               (AHBV_REG_BASE + 0x0000)
#define AHBV_CLK_EN0                (AHBV_REG_BASE + 0x0044)
#define AHBV_MODULE_EN              (AHBV_REG_BASE + 0x0004)

#define LCDCV_REG_BASE              0x20600000
#define LCDCV_REG_LCD_CTRL          (LCDCV_REG_BASE + 0x0000) 
#define LCDCV_REG_TOG               (LCDCV_REG_BASE + 0x0008)

#define LCDCV_MCU_REG_BASE          LCDCV_REG_BASE
#define LCDCV_REG_MCU_PARAMETER     (LCDCV_MCU_REG_BASE + 0x8000)
#define LCDCV_REG_MCU_RSTN          (LCDCV_MCU_REG_BASE + 0x8008)
#define LCDCV_REG_MCU_RDDATA        (LCDCV_MCU_REG_BASE + 0x8010)
#define LCDCV_REG_MCU_STATUS        (LCDCV_MCU_REG_BASE + 0x8014)
#define LCDCV_REG_MCU_MODE_SEL      (LCDCV_MCU_REG_BASE + 0x8018)
#define LCDCV_REG_MCU_RGB_MODE      (LCDCV_MCU_REG_BASE + 0x801c)
#define LCDCV_REG_MCU_SELPIN        (LCDCV_MCU_REG_BASE + 0x8020)
#define LCDCV_REG_MCU_RWMODE        (LCDCV_MCU_REG_BASE + 0x8034)
#define LCDCV_REG_MCU_CS0_ADDR0     (LCDCV_MCU_REG_BASE + 0x9400)
#define LCDCV_REG_MCU_CS0_ADDR1     (LCDCV_MCU_REG_BASE + 0x9404)



void DTL_SetVData(KDuint32 addr, KDuint32 value)
{
    KDuint32 data = value;
    INT_SWAP(data);
    LCMIF_Write_Data((uint8*)&data,sizeof(KDuint32), addr);
}

KDuint32 DTL_GetVData(KDuint32 addr)
{
    KDuint32 data = 0;
    LCMIF_Read_Data((uint8*)&data,sizeof(KDuint32), addr);
    INT_SWAP(data);
    return data;
}

void DTL_OrVData(KDuint32 addr, KDuint32 value)
{
    KDuint32 data = 0;
    LCMIF_Read_Data((uint8*)&data,sizeof(KDuint32), addr);
    INT_SWAP(data);
    data |= value;
    INT_SWAP(data);
    LCMIF_Write_Data((uint8*)&data,sizeof(KDuint32), addr);
}

void DTL_AndVData(KDuint32 addr, KDuint32 value)
{
    KDuint32 data = 0;
    LCMIF_Read_Data((uint8*)&data,sizeof(KDuint32), addr);
    INT_SWAP(data);
    data &= value;
    INT_SWAP(data);
    LCMIF_Write_Data((uint8*)&data,sizeof(KDuint32), addr);
}

void DTL_XorVData(KDuint32 addr, KDuint32 value)
{
    KDuint32 data = 0;
    LCMIF_Read_Data((uint8*)&data,sizeof(KDuint32), addr);
    INT_SWAP(data);
    data ^= value;
    INT_SWAP(data);
    LCMIF_Write_Data((uint8*)&data,sizeof(KDuint32), addr);
}

void DTL_InitV()
{
    //INIT SEINE SYSTEM
    DTL_SetVData(INTCV_IRQ_PROTECT, 0x1);
    DTL_SetVData(INTCV_INT_SEL, 0x0);
    DTL_SetVData(INTCV_INT_DIS, 0xffffffff);
    DTL_OrVData(AHBV_SOFT_RST, BIT_3);
    DTL_OrVData(AHBV_CLK_EN0, BIT_2|BIT_3);
    DTL_OrVData(AHBV_MODULE_EN,BIT_2);
    DTL_OrVData(AHBV_SOFT_RST, BIT_6);
    DTL_AndVData(AHBV_SOFT_RST, ~BIT_6);
    
    //INIT SEINE LCDC
    DTL_SetVData(LCDCV_REG_LCD_CTRL, 0x16);
    DTL_SetVData(LCDCV_REG_MCU_SELPIN, 0);
    DTL_SetVData(LCDCV_REG_MCU_MODE_SEL, 0);
    DTL_SetVData(LCDCV_REG_MCU_RGB_MODE, 1);
    DTL_SetVData(LCDCV_REG_MCU_RWMODE, 0);
    DTL_SetVData(LCDCV_REG_MCU_PARAMETER, 0XFFFFFFFF);
    DTL_XorVData(LCDCV_REG_TOG, BIT_1);
    
    DTL_SetVData(LCDCV_REG_MCU_RSTN, 1);
    kdMsDelay(10);
}

KDuint32 DTL_ReadVLCDC(KDuint32 cd_status)
{
    //DO SEINE LCDC READ
    while(1)
    {
        KDuint32 ahb_fifo;
        ahb_fifo = DTL_GetVData(LCDCV_REG_MCU_STATUS);
        if(ahb_fifo & BIT_2)
        {
            break;
        }
    }
    
    DTL_OrVData(LCDCV_REG_MCU_RWMODE, BIT_0);
    
    if(cd_status)
    {
        DTL_SetVData(LCDCV_REG_MCU_CS0_ADDR1, 0);
    }
    else
    {
        DTL_SetVData(LCDCV_REG_MCU_CS0_ADDR0, 0);
    }
    
    while(1)
    {
        KDuint32 ahb_fifo;
        ahb_fifo = DTL_GetVData(LCDCV_REG_MCU_STATUS);
        if(ahb_fifo & BIT_2)
        {
            break;
        }
    }
    
    DTL_AndVData(LCDCV_REG_MCU_RWMODE, 0);
    
    return (DTL_GetVData(LCDCV_REG_MCU_RDDATA) & 0x3ffff);
}



void DTL_WriteVLCDC(KDuint32 cd_status, KDuint32 data)
{
    while(1)
    {
        KDuint32 ahb_fifo;
        ahb_fifo = DTL_GetVData(LCDCV_REG_MCU_STATUS);
        if(BIT_1 != (ahb_fifo & BIT_1))
        {
            break;
        }
    }
    
    if(cd_status)
    {
        DTL_SetVData(LCDCV_REG_MCU_CS0_ADDR1, data);
    }
    else
    {
        DTL_SetVData(LCDCV_REG_MCU_CS0_ADDR0, data);
    }
}


#define DTL_DELAY_TIME  5

void DTL_LCDCTest()
{
    static KDuint32 lg_loop = 0;
    KDuint32 data;
    
    
    //SCI_DisableIRQ();
    
    MTVCFG_Open_26MClock();
    GPIO_MTVReset(TRUE);
    GPIO_MTVStandby(FALSE);
    kdMsDelay(DTL_DELAY_TIME);
    GPIO_MTVLCDBypass(FALSE);
    kdMsDelay(DTL_DELAY_TIME);
    GPIO_MTVReset(FALSE);
    kdMsDelay(DTL_DELAY_TIME);
    LCMIF_Init(FALSE);
    
    DTL_InitV();
    DTL_WriteVLCDC(0, lg_loop);
    
    data = DTL_ReadVLCDC(1);
    KD_TRACE_LOW("LG get lcdc data 0x%x", data);
    
    GPIO_MTVLCDBypass(TRUE);
    kdMsDelay(DTL_DELAY_TIME);
    GPIO_MTVStandby(TRUE);
    kdMsDelay(DTL_DELAY_TIME);
    
    //SCI_RestoreIRQ();
    
    lg_loop++;
    if(lg_loop > 10)
    {
        lg_loop = 0;
    }
}


/************************************************************************
* \fn  void DTL_VLCDCInit(void);
* \param void
* \return void 
* \brief  init v lcdc
*             
* \author Hanbing.Zhang
* \note   Nothing.  
************************************************************************/
void DTL_VLCDCInit(void)
{
    MTVCFG_Open_26MClock();

    GPIO_MTVReset(TRUE);
    GPIO_MTVStandby(FALSE);
    kdMsDelay(DTL_DELAY_TIME);
    GPIO_MTVLCDBypass(FALSE);
    kdMsDelay(DTL_DELAY_TIME);
    GPIO_MTVReset(FALSE);
    kdMsDelay(DTL_DELAY_TIME);

    LCMIF_Init(FALSE);
    
    DTL_InitV();
    
}

/************************************************************************
* \fn void DTL_VLCDCClose(void);
* \param void
* \return void 
* \brief  close v lcdc
*             
* \author Hanbing.Zhang
* \note   Nothing.  
************************************************************************/
void DTL_VLCDCClose(void)
{

    GPIO_MTVLCDBypass(TRUE);
    kdMsDelay(DTL_DELAY_TIME);
    GPIO_MTVStandby(TRUE);
    kdMsDelay(DTL_DELAY_TIME);
    
    MTVCFG_Close_26MClock();
}

/************************************************************************
* \fn KDuint32 DTL_ReadLCDID(KDuint32 wcmd_cd_status,
*                      KDuint32 lcdid_addr);
* \param cd_status write command cd status
* \param lcdid_addr lcd id address
* \return lcd id
* \brief  read v lcdc id
*             
* \author gang.li
* \note   call this after DTL_VLCDCInit.
************************************************************************/
KDuint32 DTL_ReadLCDID(KDuint32 wcmd_cd_status,
                       KDuint32 lcdid_addr)
{
    KDuint32 data = 0;

    DTL_VLCDCInit();

    DTL_WriteVLCDC(wcmd_cd_status, lcdid_addr);
    data = DTL_ReadVLCDC(!wcmd_cd_status);
    
    KD_TRACE_LOW("DTL_ReadLCDID()wcmd_cd_status=%d,lcdid_addr=%x,lcdid=%x",wcmd_cd_status,lcdid_addr,data);

    DTL_VLCDCClose();

    return data;
}


/************************************************************************
* \fn  PUBLIC void for fota(void)
* \param void
* \return void 
* \brief  power down seine some ldo
*             
* \author Hanbing.Zhang
* \note   Nothing.  
************************************************************************/
PUBLIC void DTL_CloseLdo(void)
{
    uint32  reg_data1  =  0x20;
    uint32  reg_data2  =  0x20202000;
    uint32  reg_data3  =  0x20;
    
    MTVCFG_Open_26MClock();
    //power on seine
    GPIO_MTVReset(TRUE);
    GPIO_MTVStandby(FALSE);
    kdMsDelay(DTL_DELAY_TIME);
    GPIO_MTVLCDBypass(FALSE);
    //kdMsDelay(5);
    
    kdMsDelay(DTL_DELAY_TIME);
    GPIO_MTVReset(FALSE);
    //for ( i =0; i< 1000000;i ++ );
    kdMsDelay(DTL_DELAY_TIME);
    
    LCMIF_Init(FALSE);
    
    LCMIF_Write(reg_data1,0X8400003c);
    LCMIF_Write(reg_data2,0X84000038);
    LCMIF_Write(reg_data3,0X84000034);
    
    LCMIF_Close();
    
    //power down seine
    GPIO_MTVLCDBypass(TRUE);
    kdMsDelay(DTL_DELAY_TIME);
    GPIO_MTVStandby(TRUE);
    
    MTVCFG_Close_26MClock();


}

