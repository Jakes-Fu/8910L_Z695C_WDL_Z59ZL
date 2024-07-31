/******************************************************************************
 ** File Name:      timer_phy_v0.c                                            *
 ** Author:         Ji.Ding                                                   *
 ** DATE:           08/09/2010                                                *
 ** Copyright:      2002 Spreadtrum, Incoporated. All Rights Reserved.         *
 ** Description:    This file defines the basic operation interfaces of       *
 **                 timer count .                                             *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME         DESCRIPTION                                   *
 ** 08/09/2010     DingJi       Create.                                       *
 ******************************************************************************/

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "sci_types.h"
//#include "com_drvapi.h"
#include "tb_hal.h"
#include "chip_plf_export.h"
#include "../timer_phy.h"

/**---------------------------------------------------------------------------*
 **                         Debugging Flag                                    *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
extern   "C"
{
#endif

/**---------------------------------------------------------------------------*
 **                         Global Variables                                  *
 **---------------------------------------------------------------------------*/
#define TIMER_UPDATE_TIME_OUT 0xffff
#define SCI_TICKTOMILLISECOND(_TICK)        (((uint64)_TICK *1000)  / 16384)

uint32 g_timer_ctrl_value[TIMER_PHY_NUMBER];

/**---------------------------------------------------------------------------*
 **                     Local Function Prototypes                             *
 **---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description:    Get TIMER register value with double read.
//  Author:         Yong.Li
//  Note:
/*****************************************************************************/
LOCAL uint32 _GetTimerValue (
    uint32 timer_id       // Indicate which timer to be get.
)
{
#if 0
    uint32  time_out = 0;
    volatile uint32 tmp_count1;
    volatile uint32 tmp_count2;
    timer_s *timer_ctl = TB_NULL;

    SCI_ASSERT ( (TIMER_0 == timer_id) || (TIMER_1 == timer_id));/*assert verified*/

    // Set local variable.
    timer_ctl = (timer_s *) ( (uint32) TIMER0_BASE + timer_id*0x20); 

    tmp_count1 = timer_ctl->value;
    tmp_count2 = timer_ctl->value;

    while(tmp_count1 != tmp_count2)
    {
        time_out++;
        if(time_out > TIMER_UPDATE_TIME_OUT)
        {
            SCI_PASSERT (0, ("Timer read value time out!"));/*assert verified*/
        }
        tmp_count1 = tmp_count2;
        tmp_count2 = timer_ctl->value;
    }

    return tmp_count1;
#else
    return 0;
#endif
}

//******************************************************************************
//  Description:  to set the timer register according to the configuration
//  Global resource dependence:  NONE
//  Author:  Ji.Ding
//  para:  timer_id: the timer index
//         config: the configuration from user
//  Note:  NONE
//******************************************************************************
LOCAL int32 timer_phy_config (uint32 timer_id, TIMER_PHY_CONFIG_T *config)
{
    HWP_TIMER_T *timer_ctl;
    uint32 *timer_ctl_value;
    
    if (timer_id >= (uint32) TIMER_CFG_get_number())
    {
        return -1;
    }

    if (config == NULL)
    {
        return -2;
    }
    
    switch(timer_id)
    {
        case TIMER_1:
        {
            timer_ctl = (HWP_TIMER_T*)(REG_TIMER1_BASE);
            break;
        }
        case TIMER_2:
        {
            timer_ctl = (HWP_TIMER_T*)(REG_TIMER2_BASE);
            break;
        }
        case TIMER_3:
        {
            timer_ctl = (HWP_TIMER_T*)(REG_TIMER3_BASE);
            break;
        }
    }

    timer_ctl_value = &g_timer_ctrl_value[timer_id];
    *timer_ctl_value = 0;
    
    if (config->mode == TIMER_MODE_FREE)
    {
        *timer_ctl_value |= TIMER_WRAP;
    }
    else
    {
        *timer_ctl_value &= ~TIMER_WRAP;
        *timer_ctl_value |= TIMER_REPEAT;
    }

    if (config->irq_op)
    {
        timer_ctl->Timer_Irq_Mask_Set = TIMER_OSTIMER_MASK;
    }
    else
    {
        timer_ctl->Timer_Irq_Mask_Clr = TIMER_OSTIMER_MASK;
    }

    *timer_ctl_value |= TIMER_LOAD | TIMER_LOADVAL(config->load_count);

    return 0;
}

//******************************************************************************
//  Description:  initialize the related timer register and environment
//  Global resource dependence:  NONE
//  Author:  Ji.Ding
//  para:  timer_id: the timer index
//  Note:  NONE
//******************************************************************************
LOCAL int32 timer_phy_init (uint32 timer_id)
{
    TIMER_PHY_CONFIG_T config;
    TIMER_PHY_STATUS_T *status;	

    if (timer_id >= (uint32) TIMER_CFG_get_number())
    {
        return -1;
    }

    status = TIMER_CFG_get_status(timer_id);
    if (!status)
    {
    	return -1;
    }

    config.irq_op = status->irq_status;
    config.mode = status->mode;
    config.load_count = status->max_count;

    timer_phy_config (timer_id, &config);

    return 0;

}

//******************************************************************************
//  Description:  to start timer counting
//  Global resource dependence:  NONE
//  Author:  Ji.Ding
//  para:  timer_id: the timer index
//  Note:  NONE
//******************************************************************************
LOCAL int32 timer_phy_enable (uint32 timer_id)
{
    HWP_TIMER_T *timer_ctl;

    if (timer_id >= (uint32) TIMER_CFG_get_number())
    {
        return -1;
    }

    switch(timer_id)
    {
        case TIMER_1:
        {
            timer_ctl = (HWP_TIMER_T*)(REG_TIMER1_BASE);
            break;
        }
        case TIMER_2:
        {
            timer_ctl = (HWP_TIMER_T*)(REG_TIMER2_BASE);
            break;
        }
        case TIMER_3:
        {
            timer_ctl = (HWP_TIMER_T*)(REG_TIMER3_BASE);
            break;
        }
    }
    
    timer_ctl->OSTimer_Ctrl = g_timer_ctrl_value[timer_id] | TIMER_ENABLE;

    return 0;

}

//******************************************************************************
//  Description:  to stop timer counting
//  Global resource dependence:  NONE
//  Author:  Ji.Ding
//  para:  timer_id: the timer index
//  Note:  NONE
//******************************************************************************
int32 timer_phy_disable (uint32 timer_id)
{
    HWP_TIMER_T *timer_ctl;

    if (timer_id >= (uint32) TIMER_CFG_get_number())
    {
        return -1;
    }

    switch(timer_id)
    {
        case TIMER_1:
        {
            timer_ctl = (HWP_TIMER_T*)(REG_TIMER1_BASE);
            break;
        }
        case TIMER_2:
        {
            timer_ctl = (HWP_TIMER_T*)(REG_TIMER2_BASE);
            break;
        }
        case TIMER_3:
        {
            timer_ctl = (HWP_TIMER_T*)(REG_TIMER3_BASE);
            break;
        }
    }
    
    timer_ctl->OSTimer_Ctrl &= ~TIMER_ENABLE;

    return 0;
}

//******************************************************************************
//  Description:  to clr the irq signal after timer generate
//  Global resource dependence:  NONE
//  Author:  Ji.Ding
//  para:  timer_id: the timer index
//  Note:  NONE
//******************************************************************************
LOCAL int32 timer_phy_clrirq (uint32 timer_id)
{
    HWP_TIMER_T *timer_ctl;

    if (timer_id >= (uint32) TIMER_CFG_get_number())
    {
        return -1;
    }
    
    switch(timer_id)
    {
        case TIMER_1:
        {
            timer_ctl = (HWP_TIMER_T*)(REG_TIMER1_BASE);
            break;
        }
        case TIMER_2:
        {
            timer_ctl = (HWP_TIMER_T*)(REG_TIMER2_BASE);
            break;
        }
        case TIMER_3:
        {
            timer_ctl = (HWP_TIMER_T*)(REG_TIMER3_BASE);
            break;
        }
    }

    timer_ctl->Timer_Irq_Clr = TIMER_OSTIMER_CLR;
    return 0;
}

//******************************************************************************
//  Description:  to get the current status of timer through checking the register
//  Global resource dependence:  NONE
//  Author:  Ji.Ding
//  para:  timer_id: the timer index
//         status: the struct pointer of status
//  Note:  NONE
//******************************************************************************
LOCAL int32 timer_phy_query (uint32 timer_id, TIMER_PHY_STATUS_T *status)
{
    HWP_TIMER_T *timer_ctl;
    uint32 temp;

    if (timer_id >= (uint32) TIMER_CFG_get_number())
    {
        return -1;
    }

    if (status == NULL)
    {
        return -2;
    }
    
    switch(timer_id)
    {
        case TIMER_1:
        {
            timer_ctl = (HWP_TIMER_T*)(REG_TIMER1_BASE);
            break;
        }
        case TIMER_2:
        {
            timer_ctl = (HWP_TIMER_T*)(REG_TIMER2_BASE);
            break;
        }
        case TIMER_3:
        {
            timer_ctl = (HWP_TIMER_T*)(REG_TIMER3_BASE);
            break;
        }
    }
    
    status->current_value = timer_ctl->OSTimer_CurVal;
    
    temp= timer_ctl->OSTimer_Ctrl;
    status->mode = (temp & TIMER_WRAP) ? TIMER_MODE_PERIOD : TIMER_MODE_FREE;
    status->running_status = (temp& (0x1<<7)) ? 1 : 0;

    temp= timer_ctl->Timer_Irq_Mask_Set;
    status->irq_status = (temp & TIMER_OSTIMER_MASK) ? 1 : 0;

    return 0;
}

//******************************************************************************
//  Description:  to change the count register of timer in special situation
//  Global resource dependence:  NONE
//  Author:  Ji.Ding
//  para:  timer_id: the timer index
//  Note:  this function just be used in some situation
//******************************************************************************
LOCAL int32 timer_phy_reload (uint32 timer_id)
{
    return 0;
}

/* the operation set of timer phy */
LOCAL TIMER_PHY_OPERATION_T timer_phy_op =
{
    timer_phy_init,
    timer_phy_enable,
    timer_phy_disable,
    timer_phy_config,
    timer_phy_query,
    timer_phy_clrirq,
    timer_phy_reload,
};

//******************************************************************************
//  Description:  to get the operation set of timer phy
//  Global resource dependence:  NONE
//  Author:  Ji.Ding
//  para:  NONE
//  Note:  NONE
//******************************************************************************
PUBLIC TIMER_PHY_OPERATION_T *timer_get_phy_operation (void)
{
    return &timer_phy_op;
}

#if defined(PLATFORM_UIX8910)
uint32 timer_TimTickEnable(uint32 timer_id)
{   
    HWP_TIMER_T *timer_ctl;
    if (timer_id >= (uint32) TIMER_CFG_get_number())
    {
        return -1;
    }
    
    switch(timer_id)
    {
        case TIMER_1:
        {
            timer_ctl = (HWP_TIMER_T*)(REG_TIMER1_BASE);
            break;
        }
        case TIMER_2:
        {
            timer_ctl = (HWP_TIMER_T*)(REG_TIMER2_BASE);
            break;
        }
        case TIMER_3:
        {
            timer_ctl = (HWP_TIMER_T*)(REG_TIMER3_BASE);
            break;
        }
    }
    return (timer_ctl->OSTimer_Ctrl & TIMER_ENABLE);
}

uint32 timer_TimTickIsEnabled(uint32 timer_id)
{   
    HWP_TIMER_T *timer_ctl;
    if (timer_id >= (uint32) TIMER_CFG_get_number())
    {
        return -1;
    }
    
    switch(timer_id)
    {
        case TIMER_1:
        {
            timer_ctl = (HWP_TIMER_T*)(REG_TIMER1_BASE);
            break;
        }
        case TIMER_2:
        {
            timer_ctl = (HWP_TIMER_T*)(REG_TIMER2_BASE);
            break;
        }
        case TIMER_3:
        {
            timer_ctl = (HWP_TIMER_T*)(REG_TIMER3_BASE);
            break;
        }
    }
    return (timer_ctl->OSTimer_Ctrl & TIMER_ENABLED);
}

uint32 timer_TimTickGetVal(uint32 timer_id)
{    
    HWP_TIMER_T *timer_ctl;
    if (timer_id >= (uint32) TIMER_CFG_get_number())
    {
        return -1;
    }
    
    switch(timer_id)
    {
        case TIMER_1:
        {
            timer_ctl = (HWP_TIMER_T*)(REG_TIMER1_BASE);
            break;
        }
        case TIMER_2:
        {
            timer_ctl = (HWP_TIMER_T*)(REG_TIMER2_BASE);
            break;
        }
        case TIMER_3:
        {
            timer_ctl = (HWP_TIMER_T*)(REG_TIMER3_BASE);
            break;
        }
     }
    return timer_ctl->OSTimer_CurVal;
}
#endif

uint32 timer_TimRealTickGet(uint32 timer_id)
{    
    HWP_TIMER_T *timer_ctl;
    if (timer_id >= (uint32) TIMER_CFG_get_number())
    {
        return -1;
    }
    
    switch(timer_id)
    {
        case TIMER_1:
        {
            timer_ctl = (HWP_TIMER_T*)(REG_TIMER1_BASE);
            break;
        }
        case TIMER_2:
        {
            timer_ctl = (HWP_TIMER_T*)(REG_TIMER2_BASE);
            break;
        }
        case TIMER_3:
        {
            timer_ctl = (HWP_TIMER_T*)(REG_TIMER3_BASE);
            break;
        }
     }
    return (timer_ctl->HWTimer_CurVal);
}

uint32 timer_TimTickGetHwVal(uint32 timer_id)
{    
    return (SCI_TICKTOMILLISECOND(timer_TimRealTickGet(timer_id)));
}

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
}
#endif  // End of timer_drv.c

