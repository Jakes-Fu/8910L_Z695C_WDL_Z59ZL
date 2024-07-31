#if defined(BROWSER_SUPPORT_WEBKIT) && ((!defined(WIN32)) || (defined(_MSC_VER) && (_MSC_VER >= 1400)))
#include "mmiwk_id.h"
#include "mmiwk_text.h"
#include "wkapi.h"
#include "mmiwk_export.h"

#include "guiimg.h"
/*---------------------------------------------------------------------------*/
/*                          TYPE AND CONSTANT                                */
/*---------------------------------------------------------------------------*/



/**--------------------------------------------------------------------------*
**                         FUNCTION DECALARION                              *
**--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : the process message function of the webkit application
//  Global resource dependence : none
//  Author: sam.hua
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleWebKitMsg( PWND app_ptr, uint16 msg_id, DPARAM param);



/**--------------------------------------------------------------------------*
**                         LOCAL VARIABLES                                   *
**--------------------------------------------------------------------------*/
MMI_APPLICATION_T			g_wkmmi_app = {0};  //webkit mmi application
/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : the process message function of the webkit application
//  Global resource dependence : none
//  Author: sam.hua
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleWebKitMsg( PWND app_ptr, uint16 msg_id, DPARAM param)
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
	MMI_WIN_ID_T query_win_id = MMIWK_QUERY_WIN_ID;

    switch( msg_id )
    {
        case WK_SIG_INVALIDATE_RECT_IND:
            {
            WK_SIG_INVALIDATE_RECT_IND_T* signal_ptr =  (WK_SIG_INVALIDATE_RECT_IND_T*)((uint32)param);
            if (signal_ptr == PNULL) break;
            MMIWK_HandPaintInd(&(signal_ptr->paint_rect), signal_ptr->bit_ptr);
            }
            break;
        case WK_SIG_UPDATE_PROGRESS_IND:
            {
            WK_SIG_UPDATE_PROGRESS_IND_T* signal_ptr =  (WK_SIG_UPDATE_PROGRESS_IND_T*)((uint32)param);
            if (signal_ptr == PNULL) break;
            SCI_TRACE_LOW("mmiwk: MMIWK HandleWebKitMsg  WK_SIG_UPDATE_PROGRESS_IND");
            MMIWK_UpdateProgressInd(signal_ptr->type,signal_ptr->progress);
            }
            break;
        case WK_SIG_IM_STATE_CHANGED_IND:
            {
            WK_IM_STATE_CHANGED_IND_T* signal_ptr =  (WK_IM_STATE_CHANGED_IND_T*)((uint32)param);
            if (signal_ptr == PNULL) break;
            SCI_TRACE_LOW("mmiwk: MMIWK HandleWebKitMsg  WK_SIG_IM_STATE_CHANGED_IND signal_ptr->text_len =%d",signal_ptr->text_len);
            MMIWK_OpenEditor(signal_ptr->type,signal_ptr->text,signal_ptr->text_len);
            }    
            break;

        case WK_SIG_NAVIGATION_CNF:
            {
            WK_SIG_NAVIGATION_CNF_T* signal_ptr =  (WK_SIG_NAVIGATION_CNF_T*)((uint32)param);
            if (signal_ptr == PNULL) break;
            SCI_TRACE_LOW("mmiwk: MMIWK HandleWebKitMsg  WK_SIG_NAVIGATION_CNF_T");
            //check navi_type/navi_result/is_firstPage;
            if(signal_ptr->navi_result !=0)
                {
                if((signal_ptr->navi_type == WK_NAVIGATION_GO_BACK) &&
                        (signal_ptr->is_firstPage == TRUE))
                    {
                    SCI_TRACE_LOW("mmiwk: MMIWK HandleWebKitMsg  send MSG_KEYUP_RED to mmiWK");
                    if(MMK_IsFocusWin(MMIWK_WIN_ID))
                        {
                            MMK_PostMsg(MMIWK_WIN_ID, MSG_KEYUP_RED, 0,0);
                        }
                }
            }
            }    
            break;
            
        case WK_SIG_OUT_OF_MEMORY_IND:
            {
            //MMIPUB_OpenQueryWinByTextId(TXT_EXIT_QUERY, IMAGE_PUBWIN_QUERY, &query_win_id, PNULL);
            MMIWK_OutOfMemory();
            result = MMI_RESULT_FALSE; 
            }
            break;
        case WK_SIG_EXIT_CNF:
            {
            WK_Destroy();
            }
            result = MMI_RESULT_FALSE;      
            break;
        default:
            result = MMI_RESULT_FALSE;      
        break;

    }
    return result;
}

/********************************************************************
//  Description :webkit app init
//  Global resource dependence :
//  Author: sam.hua
//  Note:
*********************************************************************/
PUBLIC void MMIAPIWK_InitModule(void)
{
    //register menu here
    MMIWK_RegMenuGroup();
    //register win id
    MMIWK_RegWinIdNameArr();
}
/*****************************************************************************/
// 	Description : webkit init
//	Global resource dependence : 
//   Author:
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPIWK_Init(void)
{
    g_wkmmi_app.ProcessMsg = HandleWebKitMsg;
    g_wkmmi_app.component_type = CT_APPLICATION;
}

#endif //_MSC_VER >= 1400
