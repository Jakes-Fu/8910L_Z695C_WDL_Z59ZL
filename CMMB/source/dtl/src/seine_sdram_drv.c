

#define	USE_DEFAULT_CONFIG	1
#define	DEV_CLK_CFG			DEV_CLK_80M

#include "kd.h"
#include "seine_reg.h"

#include "seine_sdram_drv.h"
#include "lcmif_drv.h"
#include "mtv_cfg.h"
#include "dtl.h"
#include "dtl_recovery.h"
#include "mtv_setting.h"

#ifdef HOST_IS_LITTLEEND
#define SWAB16 Data16EndianChange
#define SWAB32 Data32EndianChange
#else
#define SWAB16
#define SWAB32
#endif
extern SDRAM_PARAM_S EMC_Device_Config;

void Data32EndianChange(uint32 *data32_ptr)
{
    uint8* data8_ptr = (uint8*)data32_ptr;
    uint8 tmp;
    tmp = data8_ptr[3];
    data8_ptr[3] = data8_ptr[0];
    data8_ptr[0] = tmp;

    tmp = data8_ptr[2];
    data8_ptr[2] = data8_ptr[1];
    data8_ptr[1] = tmp;
}

void Data16EndianChange(uint16 *data16_ptr)
{
    uint8* data8_ptr = (uint8*)data16_ptr;
    uint8 tmp;
    tmp = data8_ptr[1];
    data8_ptr[1] = data8_ptr[0];
    data8_ptr[0] = tmp;
}


KDuint32	read_reg32(KDuint32 address)
{
    KDboolean ret = KD_FALSE;
    KDuint32 readdata=0;
    DTL_RECOVER_CALLBACK tvboot_recover_callback;
    
    ret = LCMIF_Read(&readdata, address);

    SWAB32(&readdata);
    
    if ( MTV_RELEASE_MODE == MTV_GetAssertMode() )        
    {
        if (!ret)
        {
            DTL_SetErrorInfo(DTL_ERR_GETDATA);
            tvboot_recover_callback = DTLR_GetCallback();        
            if(tvboot_recover_callback)
            {
                tvboot_recover_callback(DTLR_GetParam(),DTL_ERR_GETDATA,0,0);
            }
        }
    }
    else
    {
        kdAssert(KD_TRUE==ret);
    }
    return readdata;
}

void	write_reg32(KDuint32 address,KDuint32 value)
{
    KDboolean ret = KD_FALSE;
    DTL_RECOVER_CALLBACK tvboot_recover_callback;
    
    SWAB32(&value);
    ret = LCMIF_Write(value, address);
    if ( MTV_RELEASE_MODE == MTV_GetAssertMode() )        
    {
        if (!ret)
        {
            DTL_SetErrorInfo(DTL_ERR_SENDDATA);
            tvboot_recover_callback = DTLR_GetCallback();        
            if(tvboot_recover_callback)
            {
                tvboot_recover_callback(DTLR_GetParam(),DTL_ERR_SENDDATA,0,0);
            }
        }
    }
    else
    {
        kdAssert(KD_TRUE == ret);
    }

}

/*
	As default configuration:
	Emc_clk derive from MPLL0,and MPLL0 = 26 * N/M/mpll_div0 = 26 * 2048/65/2=409.6M
	emc_clk = MPLL0/(emc_div+1)=409.2/(9+1)=41M
	To change emc_clk,need set Register CLK_DIV0.
	
	SDRAM的工作频率CLOCK=emc_clk/2;
	 
*/
void set_emc_clk(KDuint32 clk)
{
	KDuint32 temp;

   	switch(clk)
   	{
   		case DEV_CLK_100M:
   		{	//EMC CLK_SEL
		   temp = read_reg32(AHB_CLK_SEL);
		   temp &= 0xfe3fffff;
		   temp |= (0<<22);//100MHz
		   write_reg32(AHB_CLK_SEL,temp);

			//EMC clock divider
		   temp = read_reg32(AHB_CLK_DIV0);
		   temp &= 0xfc1fffff;
		   temp |= (1<<21);//100MHz
		   write_reg32(AHB_CLK_DIV0,temp);
		}
   		break;
   		case DEV_CLK_80M:
   		{	//EMC CLK_SEL
		   temp = read_reg32(AHB_CLK_SEL);
		   temp &= 0xfe3fffff;
		   temp |= (1<<22);//80MHz
		   write_reg32(AHB_CLK_SEL,temp);

			//EMC clock divider
		   temp = read_reg32(AHB_CLK_DIV0);
		   temp &= 0xfc1fffff;
		   temp |= (0<<21);//80MHz
		   write_reg32(AHB_CLK_DIV0,temp);
   		}
   		break;
   		case DEV_CLK_50M:
   		{	//EMC CLK_SEL
		   temp = read_reg32(AHB_CLK_SEL);
   		   temp &= 0xfe3fffff;
  		   temp |= (0<<22);//100MHz
		   write_reg32(AHB_CLK_SEL,temp);

			//EMC clock divider
		   temp = read_reg32(AHB_CLK_DIV0);
		   temp &= 0xfc1fffff;
		   temp |= (3<<21);//50MHz
		   write_reg32(AHB_CLK_DIV0,temp);
   		}
   		break;
   		case DEV_CLK_20M:
   		{	//EMC CLK_SEL
		   temp = read_reg32(AHB_CLK_SEL);
   		   temp &= 0xfe3fffff;
  		   temp |= (0<<22);//100MHz
		   write_reg32(AHB_CLK_SEL,temp);

			//EMC clock divider
		   temp = read_reg32(AHB_CLK_DIV0);
		   temp &= 0xfc1fffff;
		   temp |= (9<<21);//20MHz
		   write_reg32(AHB_CLK_DIV0,temp);
   		}
   		break;
   		
   	}

}
/*
	Set emc system clock includes DLL clock & EMC clock
*/
void Set_EMC_System_Clock(KDuint32 clock)
{
	KDuint32 temp;
   /////DLL CLOCK

   temp = read_reg32(AHB_CLK_EN0);
   temp |= (1<<9);
   write_reg32(AHB_CLK_EN0,temp);
   temp = read_reg32(AHB_CLK_DIV0);
   temp &= 0x83ffffff;
   temp |= (3<<26);
   write_reg32(AHB_CLK_DIV0,temp);

   set_emc_clk(clock);

}
/*
 Set the emc pin driver strength.
	 
*/
void SDRAM_PinDrv_Set(KDuint32 emc_dev)
{
	KDuint32 address,temp;
	
   ///////////////////////////////////////////
   //EMC PAD drive strength
   for(address=0x85000160;address<=0x85000224;address+=4)
   {
		if((address == 0x850001b4)||\
		   (address == 0x850001d8)||\
		   (address == 0x850001dc))
		{
		   continue;
		}
		temp = read_reg32(address);
		temp &= 0xffffffcf;

		//CKP and CKP# drv to max
		if((address == 0x850001a0)||\
		   (address == 0x850001a4))
		{
			temp &= 0xffffff83;
			if (emc_dev == EMC_DEV_DDR)
			{
				temp |= 0x10;//0x10;
			}
			else
			{
				temp |= 0x20;
			}
		}
			
		write_reg32(address,temp);	
   }
}

/*
	function:	start hardware auto refresh.
*/

void Enable_Auto_Refresh(void)
{
	KDuint32	reg_val;
	
	reg_val =read_reg32(EMC_DRAM_TIMING2);
	reg_val |= (1<<8);
	write_reg32(EMC_DRAM_TIMING2,reg_val);
}



/*
	function : 转换不同单位的时序参数为以时钟周期数
*/
#if (!USE_DEFAULT_CONFIG)
KDuint32	calculate_clock(VALUE_UNIT_S *value,KDuint32 emc_clk,KDuint32 def)
{
	if(value->value_type == 0)
	{
		return (value->value*emc_clk+999)/1000;
	}
	else if(value->value_type == 1)
	{
		return value->value;
	}
	else
	{
		return def;
	}
}
#endif
/*
	function : initialize the emc controller configuration
*/
void EMC_Controller_Init(SDRAM_PARAM_S *pSdramConfig,KDuint32 emc_clk)
{

	KDuint32	tRP,tRCDRD,tRCDWR,tWR,tRAS,tMRD,tRRD,tRFC,tWTR;
	KDuint32 reg_val,default_val,emc_dev;
	
	emc_dev = pSdramConfig->dev_type;
	write_reg32(EMC_CHANNEL_MODE0,0x33111111);
	write_reg32(EMC_AHB_TIMEOUT,0x000F10F1);
	write_reg32(EMC_CHANNEL_MODE1,0x32000000);
	write_reg32(EMC_CHANNEL_PRIORITY,0x00000000);
	write_reg32(EMC_CHANNEL_TIMEOUT0,0x0FF10FF1);
	write_reg32(EMC_CHANNEL_TIMEOUT1,0x0FF10FF1);
	write_reg32(EMC_CHANNEL_TIMEOUT2,0x0FF10FF1);
	
	default_val = 0x82262333;
	#if (!USE_DEFAULT_CONFIG)
	tRP = calculate_clock(&pSdramConfig->rp,emc_clk,default_val&0x7);
	tRP = tRP>7?7:tRP;
	
	tRCDRD = calculate_clock(&pSdramConfig->rcdrd,emc_clk,(default_val >> 4)&0x7);
	tRCDRD = tRCDRD>7?7:tRCDRD;
	
	tRCDWR = calculate_clock(&pSdramConfig->rcdwr,emc_clk,(default_val >> 8)&0x7);
	tRCDWR = tRCDWR>7?7:tRCDWR;

	tWR = calculate_clock(&pSdramConfig->wr,emc_clk,(default_val >> 12)&0x7);
	tWR = tWR>7?7:tWR;	
	
	tRAS = calculate_clock(&pSdramConfig->ras,emc_clk,(default_val >> 16)&0xf);
	tRAS = tRAS>15?15:tRAS;	
	
	tMRD = calculate_clock(&pSdramConfig->mrd,emc_clk,(default_val >> 20)&0x3);
	tMRD = tMRD>3?3:tMRD;	
	
	tRRD = calculate_clock(&pSdramConfig->rrd,emc_clk,(default_val >> 24)&0x7);
	tRRD = tRRD>3?3:tRRD;	
	
	tRFC = calculate_clock(&pSdramConfig->rfc,emc_clk,(default_val >> 28)&0xf);
	tRFC = tRFC>15?15:tRFC;

	reg_val = tRP | (tRCDRD<<4) | (tRCDWR<<8) | (tWR<<12)|\
			   (tRAS<<16) | (tMRD<<20) | (tRRD<<24) |(tRFC << 28);

	#else
	reg_val = default_val;
	#endif
	write_reg32(EMC_DRAM_TIMING0,reg_val) ;
	#if (!USE_DEFAULT_CONFIG)
	default_val = 0x0003A10;
	tWTR = calculate_clock(&pSdramConfig->wtr,emc_clk,default_val&0x7);
	tWTR= tWTR>7?7:tWTR;

	reg_val = pSdramConfig->ref*1000*emc_clk/pSdramConfig->row_num;
	reg_val = (reg_val << 20)|(tWTR<<16)|default_val|pSdramConfig->CAS_Latency;
	#else
	default_val = 0x12013a10;
	default_val |= pSdramConfig->CAS_Latency;
	reg_val = default_val;
	#endif
	
	write_reg32(EMC_DRAM_TIMING1, reg_val) ;

	write_reg32(EMC_SLEEP_CTRL , 0x013F00FF);
	
	if(emc_dev == EMC_DEV_DDR) 
	{
		if(pSdramConfig->CAS_Latency==3)
		{
	    	write_reg32(EMC_OUTPUT_TIMING0,0x7838202D);//cas=3;
	    }
	    else
	    {
	    	write_reg32(EMC_OUTPUT_TIMING0,0x1138202B);//cas=2;
	    }
    }
    else
    {
		if(pSdramConfig->CAS_Latency==2)
			write_reg32(EMC_OUTPUT_TIMING0,0x0500000C) ;
		else
			write_reg32(EMC_OUTPUT_TIMING0,0x1F00000E) ;
    } 

	write_reg32(EMC_OUTPUT_TIMING1,0x00000015);
	

	
	default_val = 0x00057538;
	#if (!USE_DEFAULT_CONFIG)
	if(ROW_SIZE_4Kb == pSdramConfig->row_size)
	{
		reg_val = 0x0007437;
	}
	else if(ROW_SIZE_8Kb == pSdramConfig->row_size)
	{
		if(pSdramConfig->bank_num == BM_4BANK) 
		{
			reg_val = 0x0007538;
		}
		else
		{
			reg_val = 0x0007448;
		}
		
	}
	else
	{
		default_val = 0x00057549;
	}
	
	reg_val = (pSdramConfig->bank_num << 20) |
			  (pSdramConfig->capacity << 16) | reg_val;
	#else
	default_val = 0x00007538;
	reg_val = (pSdramConfig->bank_num << 20) | \
			  (pSdramConfig->capacity << 16) |default_val;
	#endif
	write_reg32(EMC_ADDR_MAPPING,reg_val);
	if(emc_dev == EMC_DEV_DDR) 
	{
		write_reg32(EMC_INBND_CTRL0,0x09000020);
		
	}
	else
	{
		write_reg32(EMC_INBND_CTRL0,0x08000050);
	}
	
    if(emc_dev == EMC_DEV_DDR)
    {
   		write_reg32(EMC_INBND_CTRL1,0x01000020);//fast cas mode for FPGA
    }
    else
    {
   		write_reg32(EMC_INBND_CTRL1,0x01000050);//fast cas mode for FPGA
    }
    
    
	if(emc_dev == EMC_DEV_DDR)
	{
		if(pSdramConfig->CAS_Latency==2)
		{
			write_reg32(EMC_INBND_CTRL2,0x00400101);//cl=2,use internal clock
		}
		else
		{
			write_reg32(EMC_INBND_CTRL2,0x01000101);//cl=3,use internal clock
		}
	}
	else
	{
		if(pSdramConfig->CAS_Latency==2)
		{
			write_reg32(EMC_INBND_CTRL2,0x00200101);//cl=3,use internal clock
		}
		else
		{
			write_reg32(EMC_INBND_CTRL2,0x00800101);//cl=3,use internal clock
		}
		
	}


	
    if(emc_dev == EMC_DEV_DDR)
    {
    	write_reg32(EMC_MISC_CTRL0,0x00001111);//bit[24]: single_write_burst_read_mode
    }
    else
    {
    	write_reg32(EMC_MISC_CTRL0,0x00001011);//bit[24]: single_write_burst_read_mode
    }  
    write_reg32(EMC_MISC_CTRL1 , 0x0000000C);
	//write_reg32(MISC_CTRL1 , 0x0000010C);//clk_gating_enable
}
/*
	function : initialize the sdram device configuration
*/
void SDRAM_Device_Init(SDRAM_PARAM_S *pSdramConfig,KDuint32 cs_id)
{
	KDint reg_val;
	
	/*-------------- enable cke ------------*/
	write_reg32(EMC_SOFT_CMD,((cs_id <<28)|SOFT_CMD_SETCKE));
	while(read_reg32(EMC_SOFT_CMD)&SOFT_CMD_SETCKE);
       KD_TRACE_LOW("enable cke");    
	
	/*-------------- precharge all -------------*/
	write_reg32(EMC_SOFT_CMD,((cs_id <<28)|SOFT_CMD_PRECHG));
	while(read_reg32(EMC_SOFT_CMD)&SOFT_CMD_PRECHG);
       KD_TRACE_LOW("precharge all");    
	
	/*-------------- auto refresh -------------*/
	write_reg32((EMC_SOFT_CMD),((cs_id <<28)|SOFT_CMD_AUTOREF));
	while(read_reg32(EMC_SOFT_CMD)&SOFT_CMD_AUTOREF);
       KD_TRACE_LOW("auto refresh1");    
       
	/*-------------- auto refresh -------------*/
	write_reg32((EMC_SOFT_CMD),((cs_id <<28)|SOFT_CMD_AUTOREF));
	while(read_reg32(EMC_SOFT_CMD)&SOFT_CMD_AUTOREF);
       KD_TRACE_LOW("auto refresh2");    
       
	/*--------------- load mode register -------------*/
	reg_val = (cs_id <<28)|(SOFT_CMD_LOADREG)|(pSdramConfig->CAS_Latency<<4)|1; 
	
	write_reg32(EMC_SOFT_CMD,reg_val);
	while(read_reg32(EMC_SOFT_CMD)&SOFT_CMD_LOADREG);
       KD_TRACE_LOW("load mode register");    
       
	/*---------- load extended mode register ----------*/
	reg_val = (cs_id <<28)|(SOFT_CMD_LOADREG)|0x8000; 
	 
	write_reg32(EMC_SOFT_CMD,reg_val);
	while(read_reg32(EMC_SOFT_CMD)&SOFT_CMD_LOADREG);
       KD_TRACE_LOW("load extended mode register");    
       
}



void SDRAM_Init(void)
{
	KDint	emc_clk = DEV_CLK_CFG,dev_type = EMC_Device_Config.dev_type;
	

	/*--------- Initialize the emc related pins---------*/
	SDRAM_PinDrv_Set(dev_type);

	/*----------- Configure emc clock ------------*/
	Set_EMC_System_Clock(emc_clk);
	
	/*----------- Initialize the emc controller----------*/
	EMC_Controller_Init(&EMC_Device_Config,emc_clk/(1000*1000));
	
	/*----------- Initialize the sdram device -----------*/
	SDRAM_Device_Init(&EMC_Device_Config,0);
	SDRAM_Device_Init(&EMC_Device_Config,1);
	
	/*--------- start hardware auto refresh -------------*/
	Enable_Auto_Refresh();
}


