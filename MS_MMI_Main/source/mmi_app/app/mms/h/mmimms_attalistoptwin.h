/****************************************************************************
** File Name:      mmimms_attalistoptwin.h                                     *
** Author:         arvin.zhang                                              *
** Date:           08/16/2015                                              *
** Copyright:      2015 Spreadtrum, Incorporated. All Rights Reserved.     *
** Description:                                                            *
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE                NAME             DESCRIPTION                             *
** 08/2015    arvin.zhang               create
** 
****************************************************************************/

/**---------------------------------------------------------------------------*
 **                         Include Files                                     *
 **---------------------------------------------------------------------------*/
#include "mmi_app_mms_trc.h"
#ifdef WIN32
#include "std_header.h"
#endif
#include "window_parse.h"
#include "mmimms_id.h"
#include "mmi_appmsg.h"
#include "mmipub.h"
#include "mmicc_export.h"
#include "mmk_timer.h"
#include "guires.h"
#include "guilcd.h"
#include "mmitheme_pos.h"

#include "mmimms_text.h"
#include "mms_image.h"
#include "mmimms_export.h"
#include "mmimms_internal.h"
#include "mmimms_net.h"
#include "mmicountedtime_export.h"
#include "mmialarm_export.h"
#include "mmi_default.h"

/**---------------------------------------------------------------------------*
 **                         Macro Declaration    							  *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         TYPE AND CONSTANT    							  *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         Constant Variables                                *
 **---------------------------------------------------------------------------*/
 
/**--------------------------------------------------------------------------*
 **                         STATIC DEFINITION                                *
 **--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : Create the mms attachments list based on different operation type
//  Global resource dependence : none
//  Author: arvin
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIMMS_CreateATTAListOptWin(
                                           ADD_DATA init_param
                                           );