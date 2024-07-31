/******************************************************************************
 ** File Name:      keymap_sp9800.c                                          *
 ** Author:         Benjamin.Wang                                             *
 ** DATE:           30/05/2007                                                *
 ** Copyright:      2008 Spreadtrum, Incoporated. All Rights Reserved.        *
 ** Description:                                                              *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 30/05/2007     Benjamin.Wang    Create.                                   *
 ******************************************************************************/
#include "tb_dal.h" 

// For UIS8910C or UIS8910A refphone
LOCAL const uint16 keymap[] = {
 SCI_VK_1,    SCI_VK_2,    SCI_VK_3,  	 SCI_VK_LEFT,	SCI_VK_WEB,  		SCI_VK_INVALID_KEY, SCI_VK_INVALID_KEY, SCI_VK_INVALID_KEY,
 SCI_VK_4,    SCI_VK_5,	   SCI_VK_6,	 SCI_VK_RIGHT,	SCI_VK_INVALID_KEY,	SCI_VK_INVALID_KEY, SCI_VK_INVALID_KEY, SCI_VK_INVALID_KEY,
 SCI_VK_7,	  SCI_VK_8,    SCI_VK_9,     SCI_VK_UP,     SCI_VK_INVALID_KEY,	SCI_VK_INVALID_KEY, SCI_VK_INVALID_KEY, SCI_VK_INVALID_KEY,
 SCI_VK_STAR, SCI_VK_0,    SCI_VK_POUND, SCI_VK_DOWN,	SCI_VK_INVALID_KEY,	SCI_VK_INVALID_KEY, SCI_VK_INVALID_KEY, SCI_VK_INVALID_KEY,
 SCI_VK_CALL, SCI_VK_MENU_SELECT, SCI_VK_MENU_CANCEL,	SCI_VK_INVALID_KEY,	SCI_VK_INVALID_KEY,	SCI_VK_INVALID_KEY, SCI_VK_INVALID_KEY, SCI_VK_INVALID_KEY
 //SCI_VK_CANCEL,SCI_VK_CALL, SCI_VK_CALL2,	SCI_VK_INVALID_KEY,	SCI_VK_INVALID_KEY,	SCI_VK_INVALID_KEY, SCI_VK_INVALID_KEY, SCI_VK_INVALID_KEY
};

PUBLIC uint16* KPD_GetKeyMap( uint32* keymap_size)
{
    *keymap_size=0;
    *keymap_size = sizeof(keymap)/sizeof(keymap[0]);
    return (uint16*)keymap;
}


