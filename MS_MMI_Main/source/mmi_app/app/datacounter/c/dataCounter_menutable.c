#include "sci_types.h"
#include "mmi_image.h"
#include "guimenu.h"
#include "mmi_common.h"
#include "mmi_nv.h"
#include "mmi_menutable.h"

#include "datacounter_text.h"
#include "datacounter_menutable.h"

const GUIMENU_ITEM_T menu_datacounter_clear_all_table[] =
{
	{MMI_DATACOUNTER_CLEAR_ALL_COUNTER_DATA_MENU, TIP_NULL,{TXT_NULL,STXT_SOFTKEY_SELECT_MK,STXT_RETURN},STR_COUN_CLEAR_ALL_EXT01, 0, 0, 0, 0},
};

const GUIMENU_ITEM_T menu_datacounter_clear_data_table[] =
{
	{MMI_DATACOUNTER_CLEAR_DATA_COUNTER_DATA_MENU, TIP_NULL,{TXT_NULL,STXT_SOFTKEY_SELECT_MK,STXT_RETURN},STR_COUN_LIST_CLEAR_EXT02, 0, 0, 0, 0},
};

const GUIMENU_ITEM_T menu_datacounter_clear_call_table[] =
{
	{MMI_DATACOUNTER_CLEAR_CALL_COUNTER_DATA_MENU, TIP_NULL,{TXT_NULL,STXT_SOFTKEY_SELECT_MK,STXT_RETURN},STR_COUN_LIST_CLEAR_EXT02, 0, 0, 0, 0},
};

const GUIMENU_ITEM_T menu_datacounter_clear_sms_table[] =
{
	{MMI_DATACOUNTER_CLEAR_SMS_COUNTER_DATA_MENU, TIP_NULL,{TXT_NULL,STXT_SOFTKEY_SELECT_MK,STXT_RETURN},STR_COUN_LIST_CLEAR_EXT02, 0, 0, 0, 0},
};

const GUIMENU_ITEM_T menu_datacounter_clear_mms_table[] =
{
	{MMI_DATACOUNTER_CLEAR_MMS_COUNTER_DATA_MENU, TIP_NULL,{TXT_NULL,STXT_SOFTKEY_SELECT_MK,STXT_RETURN},STR_COUN_LIST_CLEAR_EXT02, 0, 0, 0, 0},
};

const GUIMENU_ITEM_T menu_datacounter_data_alert_set_table[] =
{
	{MMI_DATACOUNTER_DATA_ALERT_SET_OFF_MENU, TIP_NULL,{TXT_NULL,STR_HDR_SET_ALERT,STXT_RETURN},STR_COUN_ALERT_OFF_EXT01, 0, 0, 0, 0},
	{MMI_DATACOUNTER_DATA_ALERT_SET_ALL_MENU, TIP_NULL,{TXT_NULL,STR_HDR_SET_ALERT,STXT_RETURN},STR_DATA_ALERT_SETTINGS_ALL, 0, 0, 0, 0},
	{MMI_DATACOUNTER_DATA_ALERT_SET_SENT_MENU, TIP_NULL,{TXT_NULL,STR_HDR_SET_ALERT,STXT_RETURN},STR_DATA_ALERT_SETTINGS_SENT, 0, 0, 0, 0},
	{MMI_DATACOUNTER_DATA_ALERT_SET_RECEIVED_MENU, TIP_NULL,{TXT_NULL,STR_HDR_SET_ALERT,STXT_RETURN},STR_DATA_ALERT_SETTINGS_RECV, 0, 0, 0, 0},
};

const GUIMENU_ITEM_T menu_datacounter_call_alert_set_table[] =
{
	{MMI_DATACOUNTER_CALL_ALERT_SET_OFF_MENU, TIP_NULL,{TXT_NULL,STR_HDR_SET_ALERT,STXT_RETURN},STR_COUN_ALERT_OFF_EXT01, 0, 0, 0, 0},
	{MMI_DATACOUNTER_CALL_ALERT_SET_ALL_MENU, TIP_NULL,{TXT_NULL,STR_HDR_SET_ALERT,STXT_RETURN},STR_DATA_ALERT_SETTINGS_ALL, 0, 0, 0, 0},
	{MMI_DATACOUNTER_CALL_ALERT_SET_OUTGOING_MENU, TIP_NULL,{TXT_NULL,STR_HDR_SET_ALERT,STXT_RETURN},STR_COUN_LIST_CL_DIAL_EXT02, 0, 0, 0, 0},
	{MMI_DATACOUNTER_CALL_ALERT_SET_INCOMING_MENU, TIP_NULL,{TXT_NULL,STR_HDR_SET_ALERT,STXT_RETURN},STR_COUN_LIST_CL_RECEI_EXT02, 0, 0, 0, 0},
};

const GUIMENU_ITEM_T menu_datacounter_sms_alert_set_table[] =
{
	{MMI_DATACOUNTER_SMS_ALERT_SET_OFF_MENU, TIP_NULL,{TXT_NULL,STR_HDR_SET_ALERT,STXT_RETURN},STR_COUN_ALERT_OFF_EXT01, 0, 0, 0, 0},
	{MMI_DATACOUNTER_SMS_ALERT_SET_ALL_MENU, TIP_NULL,{TXT_NULL,STR_HDR_SET_ALERT,STXT_RETURN},STR_DATA_ALERT_SETTINGS_ALL, 0, 0, 0, 0},
	{MMI_DATACOUNTER_SMS_ALERT_SET_SENT_MENU, TIP_NULL,{TXT_NULL,STR_HDR_SET_ALERT,STXT_RETURN},STR_DATA_ALERT_SETTINGS_SENT, 0, 0, 0, 0},
	{MMI_DATACOUNTER_SMS_ALERT_SET_RECEIVED_MENU, TIP_NULL,{TXT_NULL,STR_HDR_SET_ALERT,STXT_RETURN},STR_DATA_ALERT_SETTINGS_RECV, 0, 0, 0, 0},
};

const GUIMENU_ITEM_T menu_datacounter_mms_alert_set_table[] =
{
	{MMI_DATACOUNTER_MMS_ALERT_SET_OFF_MENU, TIP_NULL,{TXT_NULL,STR_HDR_SET_ALERT,STXT_RETURN},STR_COUN_ALERT_OFF_EXT01, 0, 0, 0, 0},
	{MMI_DATACOUNTER_MMS_ALERT_SET_ALL_MENU, TIP_NULL,{TXT_NULL,STR_HDR_SET_ALERT,STXT_RETURN},STR_DATA_ALERT_SETTINGS_ALL, 0, 0, 0, 0},
	{MMI_DATACOUNTER_MMS_ALERT_SET_SENT_MENU, TIP_NULL,{TXT_NULL,STR_HDR_SET_ALERT,STXT_RETURN},STR_DATA_ALERT_SETTINGS_SENT, 0, 0, 0, 0},
	{MMI_DATACOUNTER_MMS_ALERT_SET_RECEIVED_MENU, TIP_NULL,{TXT_NULL,STR_HDR_SET_ALERT,STXT_RETURN},STR_DATA_ALERT_SETTINGS_RECV, 0, 0, 0, 0},
};

#define MENU_DEF(_TABLEID, _ITEMPTR, _STYLE, _TITLESTR, _TITLE_ICON, _TITLE_NUMICON, _TITLE_BACKGROUND, \
    _ITEMCOUNT) \
     _ITEMPTR, _STYLE, _TITLESTR, _TITLE_ICON, _TITLE_NUMICON, _TITLE_BACKGROUND, _ITEMCOUNT, 

const GUIMENU_GROUP_T datacounter_menu_tables[] =
{
    {NULL},
    #include "dataCounter_menutable.def"
};

PUBLIC void MMI_DataCounter_RegMenuGroup(void)
{
    MMI_RegMenuGroup(MMI_MODULE_DATA_COUNTER, datacounter_menu_tables);
}

#undef MENU_DEF

