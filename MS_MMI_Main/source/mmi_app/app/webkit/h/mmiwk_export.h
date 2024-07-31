#ifndef _MMIWK_EXPORT_H_
#define _MMIWK_EXPORT_H_

#include "sci_types.h"
#include "mmk_type.h"
#include "mmibrowser_export.h"
#include "wkapi.h"
#include "mn_type.h"

#ifdef __cplusplus
extern   "C" {
#endif

/*****************************************************************************/
//  Description : initialize webkit mmi module
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIWK_Init(void);

/*****************************************************************************/
//  Description : initialize Webkit mmi
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIWK_InitModule(void);

PUBLIC int MMIWK_Exit(uint16 exit_type);
PUBLIC int MMIWK_OutOfMemory(void);
//PUBLIC int MMIWK_Minimize(void);

PUBLIC int  MMIWK_Entry(const MMIBROWSER_ENTRY_PARAM *entry_param_ptr);
PUBLIC void MMIWK_HandPaintInd(GUI_RECT_T *invalidateRect, GUIIMG_BITMAP_T *img_ptr);
PUBLIC void MMIAPIWK_HandleNewSMSInd(APP_MN_SMS_IND_T *param);

PUBLIC void MMIWK_OpenEditor(WK_INPUT_TYPE_E input_type,uint16* input_text,uint32 input_text_len);


#ifdef __cplusplus
}
#endif

#endif//_MMIWK_H_
