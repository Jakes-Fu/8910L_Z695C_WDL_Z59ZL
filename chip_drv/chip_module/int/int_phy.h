/******************************************************************************
 ** File Name:      int_phy.h                                                 *
 ** Author:         Steve.Zhan                                                *
 ** DATE:           07/20/2010                                                *
 ** Copyright:      2010 Spreadtrum, Incoporated. All Rights Reserved.        *
 ** Description:    This file declearate the prototype of the physical level  *
 **                 function of interrupt module.                             *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** --------------------------------------------------------------------------*
 ** DATE           NAME             DESCRIPTION                               *
 ** 07/20/2010     Steve.Zhan       Create.                                   *
 **                                                                           *
 ******************************************************************************/
#ifndef _INT_PHY_H_
#define _INT_PHY_H_

#include "tx_port_thumb.h"

#define INT_DISABLE_ARM_IRQ_FIQ() do { \
        SCI_DisableIRQ(); \
        SCI_DisableFIQ(); \
    }while (0)

#define INT_ENABLE_ARM_IRQ_FIQ() do { \
        SCI_RestoreFIQ(); \
        SCI_RestoreIRQ(); \
    }while (0)

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description:    This function is called when an IRQ happens. Is used to get
//                  interrupt registers status value.
//  Dependency:
//  Author:         Steve.Zhan
//  Note:
/*****************************************************************************/
PUBLIC void INT_PHY_GetIrqIntStatus (void);


/*****************************************************************************/
//  Description:    This function is called when an FIQ happens. Is used to get
//                  interrupt registers status value.
//  Dependency:
//  Author:         Steve.Zhan
//  Note:
/*****************************************************************************/
PUBLIC void INT_PHY_GetFiqIntStatus (void);

/*****************************************************************************/
//  Description:    This function is called when system startup, only init. once.
//  Dependency:
//  Author:         Steve.Zhan
//  Note:
/*****************************************************************************/
PUBLIC void INT_PHY_Init (void);


extern PRI_INDEX_GET_STATUS_T irq_isr_array[MAX_ISR_NUM];
extern PRI_INDEX_GET_STATUS_T fiq_isr_array[MAX_ISR_NUM];
extern uint32 IrqUnitRegValue[ISR_UNIT_NUM];
extern uint32 FiqUnitRegValue[ISR_UNIT_NUM];





/*****************************************************************************/
//  Description:    using interrupt ligic number to clear the interrupt.
//
//  Dependency:
//  Author:         Steve.Zhan
//  Note:           ;
/*****************************************************************************/
PUBLIC void INT_PHY_IRQClear (uint32 logic_num);


/*****************************************************************************/
//  Description:    Enable certain interrupt source.
//
//  Dependency:
//  Author:         Steve.Zhan
//  Note:           ;
/*****************************************************************************/
PUBLIC void INT_PHY_IRQEnable (uint32 logic_num);

/*****************************************************************************/
//  Description:    Disable certain interrupt source.
//
//  Dependency:
//  Author:         Steve.Zhan
//  Note:           ;
/*****************************************************************************/
PUBLIC void INT_PHY_IRQDisable (uint32 logic_num);

/*****************************************************************************/
//  Description:    Disable All interrupt source.
//
//  Dependency:
//  Author:         Steve.Zhan
//  Note:           ;
/*****************************************************************************/
PUBLIC void INT_PHY_IRQDisableAll (void);

/*****************************************************************************/
//  Description:    Get IRQ status
//
//  Dependency:
//  Author:         Steve.Zhan
//  Note:           ;
/*****************************************************************************/
PUBLIC uint32 INT_PHY_IRQGetUint1Status (void);

/*****************************************************************************/
//  Description:    Get IRQ status, get uint0..group
//
//  Dependency:
//  Author:         Steve.Zhan
//  Note:           ;
/*****************************************************************************/
uint32 INT_PHY_IRQGetUint0Status(void);
/*****************************************************************************/
//  Description:    Get IRQ IE state
//
//  Dependency:
//  Author:         Yong.Li
//  Note:           ;
/*****************************************************************************/
PUBLIC uint32 INT_PHY_IRQGetIEState(void);

/*****************************************************************************/
//  Description:    Disable certain interrupt source.
//
//  Dependency:
//  Author:         Steve.Zhan
//  Note:           ;
/*****************************************************************************/
PUBLIC void INT_PHY_FIQDisable (uint32 logic_num);


/*****************************************************************************/
//  Description:    Disable the one interrupt source.
//
//  Dependency:
//  Author:         Steve.Zhan
//  Note:           ;
/*****************************************************************************/
PUBLIC void INT_PHY_FIQEnable (uint32 logicNum);

/*****************************************************************************/
//  Description:    Disable All interrupt source FOR fiq.
//
//  Dependency:
//  Author:         Steve.Zhan
//  Note:           ;
/*****************************************************************************/
PUBLIC void INT_PHY_FIQDisableAll (void);

PUBLIC void INT_PHYFiqDebug (void);

#endif


