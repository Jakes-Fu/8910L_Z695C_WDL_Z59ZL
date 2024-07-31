/*
 * Copyright 2011 Myriad Group AG. All Rights Reserved.
 * $Id: //depot/dev/project/spreadtrum/6530_new/export/src/mmijava_id.c#1 $
 */
#include <jbed_exported_defines.h>
#include <sci_types.h>
#include <mmi_module.h>
#include <mmijava_id.h>
#include <mmi_modu_main.h>
#include <mmijava_common.h>

#define WIN_ID_DEF(win_id) #win_id

const uint8 java_id_name_array[][MMI_WIN_ID_NAME_MAX_LENGTH] =
{
    #include <mmijava_id.def>
};

#undef WIN_ID_DEF

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/

/**
 * Register window id name array.
 */
PUBLIC void MMIJAVA_RegWinIdNameArray(void)
{
    MMI_RegWinIdNameArr(MMI_MODULE_JAVA, (const uint8**)java_id_name_array);
}

