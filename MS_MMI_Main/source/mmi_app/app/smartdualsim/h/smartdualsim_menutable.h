#ifndef __SMARTDUALSIM_MENU_TABLE__
#define __SMARTDUALSIM_MENU_TABLE__

#include "guimenu.h"
#include "mmi_text.h"
#include "mmi_image.h"
#include "mmi_common.h"
#include "mmi_nv.h"
#include "mmi_module.h"
/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif

//#define SMARTDS_DISABLE_ASK_EVERY_TIME

typedef enum
{
//main menu
//    MMI_SMARTDUALSIM_CALL_FORWARDING = 0, 
    MMI_SMARTDUALSIM_FORWARD_TYPE = 0,
    MMI_SMARTDUALSIM_SIM_NUMBERS,
    MMI_SMARTDUALSIM_SIM_SETTINGS,
    MMI_SMARTDUALSIM_CALL_PREFERRED,
    MMI_SMARTDUALSIM_SMS_PREFERRED,
    MMI_SMARTDUALSIM_DATA_CONNECTION,
    MMI_SMARTDUALSIM_MAINMENU_ITEM_NUM
}MMI_SMARTDUALSIM_MAINMENU_ITEM_ID_E;

typedef enum
{
// sim settings
    MMI_SMARTDUALSIM_SIM_SETTINGS_SIM1 =0, 
    MMI_SMARTDUALSIM_SIM_SETTINGS_SIM2,
    MMI_SMARTDUALSIM_SIM_SETTINGS_ITEM_NUM
}MMI_SMARTDUALSIM_SIM_SETTINGS_ITEM_ID_E;


typedef enum
{
// settings for one sim
    MMI_SMARTDUALSIM_SIM_ONE_NAME = 0, 
    MMI_SMARTDUALSIM_SIM_ONE_STATUS,
    MMI_SMARTDUALSIM_SIM_ONE_NETWORK_SELECT,
    MMI_SMARTDUALSIM_SIM_ONE_PIN_REQUEST,
    MMI_SMARTDUALSIM_SIM_ONE_CHANGE_PIN,
    MMI_SMARTDUALSIM_SIM_ONE_VODAFONE_CARRIER,
    MMI_SMARTDUALSIM_SIM_ONE_FIXED_DIALING,
    MMI_SMARTDUALSIM_SIM_ONE_CHANGE_PIN2,
#if defined(OMA_CP_SEC_SUPPORT)
    MMI_SMARTDUALSIM_SIM_ONE_CUSTOMER_PIN,
#endif
    MMI_SMARTDUALSIM_SIM_ONE_ITEM_NUM
}MMI_SMARTDUALSIM_SIM_ONE_ITEM_ID_E;

typedef enum
{
//Vodafone Carrier
    MMI_SMARTDUALSIM_VODAFONE_UK_CONTRACT = 0,
    MMI_SMARTDUALSIM_VODAFONE_UK_PAYG,
    MMI_SMARTDUALSIM_VODAFONE_UK_ITEM_NUM
}MMI_SMARTDUALSIM_VODAFONE_UK_ITEM_ID_E;

typedef enum
{
//data connection
    MMI_SMARTDUALSIM_MOBILE_DATA_SWITCH = 0,
    MMI_SMARTDUALSIM_PREFEREED_SIM,
    MMI_SMARTDUALSIM_DATACONNECTION_ITEM_NUM
}MMI_SMARTDUALSIM_DATACONNECTION_ITEM_ID_E;

typedef enum
{
// preferred SIM
    MMI_SMARTDUALSIM_PREFERRED_SIM1 =0, 
    MMI_SMARTDUALSIM_PREFERRED_SIM2,
#ifndef SMARTDS_DISABLE_ASK_EVERY_TIME
    MMI_SMARTDUALSIM_ASK_EVERY_TIME, 
#endif
    MMI_SMARTDUALSIM_PREFERRED_ITEM_NUM
}MMI_SMARTDUALSIM_PREFERRED_ITEM_ID_E;

typedef enum
{
//forward types
    MMI_SMARTDUALSIM_FORWARD_SIM_BOTH = 0,  
    MMI_SMARTDUALSIM_FORWARD_SIM_1_TO_2,
    MMI_SMARTDUALSIM_FORWARD_SIM_2_TO_1,
    MMI_SMARTDUALSIM_FORWARD_OFF,
    MMI_SMARTDUALSIM_FORWARD_ITEM_NUM
}MMI_SMARTDUALSIM_FORWARD_ITEM_ID_E;

#define MENU_DEF(_TABLEID, _ITEMPTR, _STYLE, _TITLESTR, _TITLE_ICON, _TITLE_NUMICON, _TITLE_BACKGROUND, \
    _ITEMCOUNT) \
     _TABLEID,

typedef enum
{
    SMARTDUALSIM_MENU_START_LABEL = (MMI_MODULE_SMARTDUALSIM << 16),
    #include "smartdualsim_menutable.def"
    SMARTDUALSIM_MENU_MAX_LABEL
} SMARTDUALSIM_MENU_LABEL_E;

#undef MENU_DEF
PUBLIC void MMISmartDS_RegMenuGroup(void);

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif
/*-------------------------------------------------------------------------*/


#endif //__SMARTDUALSIM_MENU_TABLE__

