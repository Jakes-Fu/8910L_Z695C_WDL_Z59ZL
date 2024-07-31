/*
 * Copyright 2003 Aplix Corporation. All Rights Reserved.
 *
 * JBlend and all Aplix- or JBlend-related trademarks and logos are
 * trademarks or registered trademarks of Aplix in the U.S., Japan and
 * other countries.
 */


#include "mmi_app_ia_java_trc.h"
#include <string.h>
#include "std_header.h"
#include "os_api.h"
#include "jblend_config.h"

#include "jktypes.h"
#include "jkvmbackground.h"
#include "mmijava_export.h"



void JkSetVmBackground(JKSint32 vmId) {
	//SCI_TRACE_LOW:"[JkSetVmBackground] enter vmId=%d"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,JKVMBACKGROUND_21_112_2_18_2_22_42_336,(uint8*)"d", vmId);
}

void JkSetVmForeground(JKSint32 vmId) {
	//SCI_TRACE_LOW:"[JkSetVmForeground] enter vmId=%d"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,JKVMBACKGROUND_25_112_2_18_2_22_42_337,(uint8*)"d", vmId);
}

JKUint8 JkGetVmBgFgState(){
#ifdef JBLENDIA_SUPPORT_BACKGROUND_VM	
	if(MMIAPIJAVA_IsJavaBackgroundRuning()){
		//SCI_TRACE_LOW:"[JkGetVmBgFgState] JK_BG_VM_STATE_BACKGROUND"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,JKVMBACKGROUND_31_112_2_18_2_22_42_338,(uint8*)"");
		return JK_BG_VM_STATE_BACKGROUND;
	}
	else{
		//SCI_TRACE_LOW:"[JkGetVmBgFgState] JK_BG_VM_STATE_FOREGROUND"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,JKVMBACKGROUND_35_112_2_18_2_22_42_339,(uint8*)"");
		return JK_BG_VM_STATE_FOREGROUND;
	}	
#else
    return 0;
#endif
}	

void  JkNotifyPopupDialog(const JKUint8* message){
	//SCI_TRACE_LOW:"[JkNotifyPopupDialog] = message= %s "
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,JKVMBACKGROUND_44_112_2_18_2_22_42_340,(uint8*)"s", message);

}


JKBool JkIsVmOnRealScreen(void)
{
	//SCI_TRACE_LOW:"[JkIsVmOnRealScreen]"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,JKVMBACKGROUND_51_112_2_18_2_22_43_341,(uint8*)"");
	return JK_TRUE;
}
