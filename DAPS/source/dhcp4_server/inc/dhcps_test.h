/*****************************************************************************
** File Name:       dhcps_test.h
** Author:          lisa.lin
** Date:            2011/08/12
** Copyright:       2011 Spreadtrum, Incorporated. All Rights Reserved.
** Description:     dhcp server test interface
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE             NAME                DESCRIPTION                          
** 2011/08/12       lisa.lin            create
******************************************************************************/

#ifndef  _DHCPS_TEST_H_
#define  _DHCPS_TEST_H_

#ifdef __cplusplus
extern "C" {
#endif
/*---------------------------------------------------------------------------*
 *                          Include Files                                    *
 *---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*
 *                          TYPE DEFINITION                                  *
 *---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*
 *                          FUNCTION DEFINITIONS                             *
 *---------------------------------------------------------------------------*/
/*****************************************************************************/
// Description: Set Param test function
// Global : none
// Author : lisa.lin
// Note :         
/*****************************************************************************/
PUBLIC void DHCPS_Test_SetParam(void);

/*****************************************************************************/
// Description: discovery and request packet for random ip and requested lease time
// Global : none
// Author : lisa.lin
// Note :         
/*****************************************************************************/
//random ip,requested lease time
PUBLIC void DHCPS_Test_RandomIP(int type);

/*****************************************************************************/
// Description: discovery and request packet for requested ip
// Global : none
// Author : lisa.lin
// Note :         
/*****************************************************************************/
PUBLIC void DHCPS_Test_RequestIP(int type);

/*****************************************************************************/
// Description: iscovery and request packet for static ip
// Global : none
// Author : lisa.lin
// Note :         
/*****************************************************************************/
PUBLIC void DHCPS_Test_StaticIP(int type);
/*---------------------------------------------------------------------------*
 *                          Compiler Flag                                    *
 *---------------------------------------------------------------------------*/
#ifdef __cplusplus
};  /* end of extern "C" */
#endif
///////////////////////////////////////////////////////////////////////////////
#endif  /* _DHCPS_API_H_ */
