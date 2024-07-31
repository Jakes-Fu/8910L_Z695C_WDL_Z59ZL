/****************************************************************************************
** File Name:       layer1_aac.h                                                        *   
** Author:          Yu Guang Yao                                                        *
** DATE:            01/30/2007                                                          *
** CopyRight        2002 SpreadTrum Communications CORP. All Rights Reserved.           *
** Description:     This header file is to handle aac operations for Interface between  *
**                  MCU and DSP on 2k Shared Dual-Port Memory.                          *       
**                                                                                      *
*****************************************************************************************
**                  Edit History                                                        *   
** -------------------------------------------------------------------------------------*
**                                                                                      *
** DATE:            NAME                DESCIPTION                                      *
** 01/30/2007       Yu Guang Yao        Create.                                         *
*****************************************************************************************/

#ifndef __PRIORITY_BASE_H__
#define __PRIORITY_BASE_H__


/**-------------------------------------------------------------------------------------*
 **                         Dependencies                                                *
 **-------------------------------------------------------------------------------------*/
#include "os_api.h"

DECLARE_PRIORITY(PRI_CM) 
#define PRI_CM                       SCI_TASK_PRI(PRI_CM)

DECLARE_PRIORITY(PRI_DIAG)
#define PRI_DIAG                     SCI_TASK_PRI(PRI_DIAG)

DECLARE_PRIORITY(PRI_BASE_SIM)
#define PRI_BASE_SIM                 SCI_TASK_PRI(PRI_BASE_SIM)

DECLARE_PRIORITY(PRI_BASE_SIMAT_DUM)
#define PRI_BASE_SIMAT_DUM           SCI_TASK_PRI(PRI_BASE_SIMAT_DUM)

DECLARE_PRIORITY(PRI_BASE_RRC)
#define PRI_BASE_RRC                 SCI_TASK_PRI(PRI_BASE_RRC)

DECLARE_PRIORITY(PRI_BASE_PROCESSMM)
#define PRI_BASE_PROCESSMM           SCI_TASK_PRI(PRI_BASE_PROCESSMM)

DECLARE_PRIORITY(PRI_BASE_GMM)
#define PRI_BASE_GMM                 SCI_TASK_PRI(PRI_BASE_GMM)

DECLARE_PRIORITY(PRI_BASE_CMROUTER)
#define PRI_BASE_CMROUTER            SCI_TASK_PRI(PRI_BASE_CMROUTER)

DECLARE_PRIORITY(PRI_BASE_CCENTITY)
#define PRI_BASE_CCENTITY            SCI_TASK_PRI(PRI_BASE_CCENTITY)

DECLARE_PRIORITY(PRI_BASE_SMSCMENTITY)
#define PRI_BASE_SMSCMENTITY         SCI_TASK_PRI(PRI_BASE_SMSCMENTITY)

DECLARE_PRIORITY(PRI_BASE_RLROUTER)
#define PRI_BASE_RLROUTER            SCI_TASK_PRI(PRI_BASE_RLROUTER)

DECLARE_PRIORITY(PRI_BASE_RLENTITY)
#define PRI_BASE_RLENTITY            SCI_TASK_PRI(PRI_BASE_RLENTITY)

DECLARE_PRIORITY(PRI_BASE_SSENTITY)
#define PRI_BASE_SSENTITY            SCI_TASK_PRI(PRI_BASE_SSENTITY)

DECLARE_PRIORITY(PRI_BASE_MN)
#define PRI_BASE_MN                  SCI_TASK_PRI(PRI_BASE_MN)

DECLARE_PRIORITY(PRI_BASE_RRA)
#define PRI_BASE_RRA                 SCI_TASK_PRI(PRI_BASE_RRA)

DECLARE_PRIORITY(PRI_BASE_LAYER1)
#define PRI_BASE_LAYER1              SCI_TASK_PRI(PRI_BASE_LAYER1)

DECLARE_PRIORITY(PRI_BASE_NVSMS)
#define PRI_BASE_NVSMS               SCI_TASK_PRI(PRI_BASE_NVSMS) 

DECLARE_PRIORITY(PRI_BASE_DLR)
#define PRI_BASE_DLR                 SCI_TASK_PRI(PRI_BASE_DLR) 

DECLARE_PRIORITY(PRI_BASE_LLMAIN)
#define PRI_BASE_LLMAIN              SCI_TASK_PRI(PRI_BASE_LLMAIN) 

DECLARE_PRIORITY(PRI_BASE_MAC)
#define PRI_BASE_MAC                 SCI_TASK_PRI(PRI_BASE_MAC) 

DECLARE_PRIORITY(PRI_BASE_RLC)
#define PRI_BASE_RLC                 SCI_TASK_PRI(PRI_BASE_RLC) 

DECLARE_PRIORITY(PRI_BASE_SCT)
#define PRI_BASE_SCT                 SCI_TASK_PRI(PRI_BASE_SCT) 

DECLARE_PRIORITY(PRI_BASE_SMMAIN)
#define PRI_BASE_SMMAIN              SCI_TASK_PRI(PRI_BASE_SMMAIN) 

DECLARE_PRIORITY(PRI_BASE_SMENTITY)
#define PRI_BASE_SMENTITY            SCI_TASK_PRI(PRI_BASE_SMENTITY) 

DECLARE_PRIORITY(PRI_BASE_SNDCPMAIN)
#define PRI_BASE_SNDCPMAIN           SCI_TASK_PRI(PRI_BASE_SNDCPMAIN) 

DECLARE_PRIORITY(PRI_BASE_SNDCPENTITY)
#define PRI_BASE_SNDCPENTITY         SCI_TASK_PRI(PRI_BASE_SNDCPENTITY) 

DECLARE_PRIORITY(PRI_TESTPSTOL1)
#define PRI_TESTPSTOL1               SCI_TASK_PRI(PRI_TESTPSTOL1)

DECLARE_PRIORITY(PRI_TESTDSPTOL1)
#define PRI_TESTDSPTOL1              SCI_TASK_PRI(PRI_TESTDSPTOL1)

DECLARE_PRIORITY(PRI_BASE_CALIBRATION)
#define PRI_BASE_CALIBRATION         SCI_TASK_PRI(PRI_BASE_CALIBRATION)

DECLARE_PRIORITY(PRI_TDD_CALIBRATION1)
#define PRI_TDD_CALIBRATION1         SCI_TASK_PRI(PRI_TDD_CALIBRATION1)

DECLARE_PRIORITY(PRI_TDD_CALIBRATION2)
#define PRI_TDD_CALIBRATION2         SCI_TASK_PRI(PRI_TDD_CALIBRATION2)

DECLARE_PRIORITY(PRI_BASE_DLC)
#define PRI_BASE_DLC                 SCI_TASK_PRI(PRI_BASE_DLC)

DECLARE_PRIORITY(PRI_BASE_LLENTITY)
#define PRI_BASE_LLENTITY            SCI_TASK_PRI(PRI_BASE_LLENTITY)

DECLARE_PRIORITY(PRI_L1C_DUMP_MEM)
#define PRI_L1C_DUMP_MEM             SCI_TASK_PRI(PRI_L1C_DUMP_MEM)
 
DECLARE_PRIORITY(PRI_BASE_ATC_TASK)
#define PRI_BASE_ATC_TASK            SCI_TASK_PRI(PRI_BASE_ATC_TASK)

DECLARE_PRIORITY(PRI_LOG_SAVE_TASK)
#define PRI_LOG_SAVE_TASK            SCI_TASK_PRI(PRI_LOG_SAVE_TASK)

DECLARE_PRIORITY(PRI_MUX_RECV_TASK1)
#define PRI_MUX_RECV_TASK1           SCI_TASK_PRI(PRI_MUX_RECV_TASK1)

DECLARE_PRIORITY(PRI_MUX_RECV_TASK2)
#define PRI_MUX_RECV_TASK2           SCI_TASK_PRI(PRI_MUX_RECV_TASK2)

DECLARE_PRIORITY(PRI_HEIGHEST_PRIORITY)
#define  PRI_HEIGHEST_PRIORITY                 SCI_TASK_PRI(PRI_HEIGHEST_PRIORITY)

DECLARE_PRIORITY(PRI_PRIORITY_ONE)
#define  PRI_PRIORITY_ONE                      SCI_TASK_PRI(PRI_PRIORITY_ONE)

DECLARE_PRIORITY(PRI_PRIORITY_TWO)
#define  PRI_PRIORITY_TWO                      SCI_TASK_PRI(PRI_PRIORITY_TWO)

DECLARE_PRIORITY(PRI_PRIORITY_THREE)
#define  PRI_PRIORITY_THREE                    SCI_TASK_PRI(PRI_PRIORITY_THREE)

DECLARE_PRIORITY(PRI_PRIORITY_FOUR)
#define  PRI_PRIORITY_FOUR                     SCI_TASK_PRI(PRI_PRIORITY_FOUR)

DECLARE_PRIORITY(PRI_PRIORITY_FIVE)
#define  PRI_PRIORITY_FIVE                     SCI_TASK_PRI(PRI_PRIORITY_FIVE)

DECLARE_PRIORITY(PRI_PRIORITY_SIX)
#define  PRI_PRIORITY_SIX                      SCI_TASK_PRI(PRI_PRIORITY_SIX)

DECLARE_PRIORITY(PRI_PRIORITY_SEVEN)
#define  PRI_PRIORITY_SEVEN                    SCI_TASK_PRI(PRI_PRIORITY_SEVEN)

DECLARE_PRIORITY(PRI_PRIORITY_EIGHT)
#define  PRI_PRIORITY_EIGHT                    SCI_TASK_PRI(PRI_PRIORITY_EIGHT)

DECLARE_PRIORITY(PRI_PRIORITY_NINE)
#define  PRI_PRIORITY_NINE                     SCI_TASK_PRI(PRI_PRIORITY_NINE)

DECLARE_PRIORITY(PRI_PRIORITY_TEN)
#define  PRI_PRIORITY_TEN                      SCI_TASK_PRI(PRI_PRIORITY_TEN)

DECLARE_PRIORITY(PRI_PRIORITY_ELEVEN)
#define  PRI_PRIORITY_ELEVEN                   SCI_TASK_PRI(PRI_PRIORITY_ELEVEN)

DECLARE_PRIORITY(PRI_PRIORITY_TWELVE)
#define  PRI_PRIORITY_TWELVE                   SCI_TASK_PRI(PRI_PRIORITY_TWELVE)

DECLARE_PRIORITY(PRI_PRIORITY_THIRTEEN)
#define  PRI_PRIORITY_THIRTEEN                 SCI_TASK_PRI(PRI_PRIORITY_THIRTEEN)

DECLARE_PRIORITY(PRI_PRIORITY_FOURTEEN)
#define  PRI_PRIORITY_FOURTEEN                 SCI_TASK_PRI(PRI_PRIORITY_FOURTEEN)

DECLARE_PRIORITY(PRI_PRIORITY_FIFTEEN)
#define  PRI_PRIORITY_FIFTEEN                  SCI_TASK_PRI(PRI_PRIORITY_FIFTEEN)

DECLARE_PRIORITY(PRI_PRIORITY_SIXTEEN)
#define  PRI_PRIORITY_SIXTEEN                  SCI_TASK_PRI(PRI_PRIORITY_SIXTEEN)

DECLARE_PRIORITY(PRI_PRIORITY_SEVENTEEN)
#define  PRI_PRIORITY_SEVENTEEN                SCI_TASK_PRI(PRI_PRIORITY_SEVENTEEN)

DECLARE_PRIORITY(PRI_PRIORITY_EIGHTEEN)
#define  PRI_PRIORITY_EIGHTEEN                 SCI_TASK_PRI(PRI_PRIORITY_EIGHTEEN)

DECLARE_PRIORITY(PRI_LOWEST_PRIORITY)
#define  PRI_LOWEST_PRIORITY                   SCI_TASK_PRI(PRI_LOWEST_PRIORITY)

DECLARE_PRIORITY(PRI_CSVTRX_TASK)
#define  PRI_CSVTRX_TASK                       SCI_TASK_PRI(PRI_CSVTRX_TASK)

DECLARE_PRIORITY(PRI_CSVTTX_TASK)
#define  PRI_CSVTTX_TASK                       SCI_TASK_PRI(PRI_CSVTTX_TASK)

DECLARE_PRIORITY(PRI_SIM_WIN_DUMMY_TASK)
#define  PRI_SIM_WIN_DUMMY_TASK                SCI_TASK_PRI(PRI_SIM_WIN_DUMMY_TASK)

DECLARE_PRIORITY(PRI_CPANEL_TASK)
#define  PRI_CPANEL_TASK                       SCI_TASK_PRI(PRI_CPANEL_TASK)

DECLARE_PRIORITY(PRI_PSDummy_TASK)
#define  PRI_PSDummy_TASK                      SCI_TASK_PRI(PRI_PSDummy_TASK)

DECLARE_PRIORITY(PRI_DUMMY_RRC_TASK)
#define  PRI_DUMMY_RRC_TASK                    SCI_TASK_PRI(PRI_DUMMY_RRC_TASK)

DECLARE_PRIORITY(PRI_DEFAULT)
#define  PRI_DEFAULT                           SCI_TASK_PRI(PRI_DEFAULT)

#endif /*__PRIORITY_BASE_H__*/
