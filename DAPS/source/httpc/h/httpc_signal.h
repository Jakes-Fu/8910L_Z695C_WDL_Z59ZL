#ifndef _HTTPC_SIGNAL_H_
#define _HTTPC_SIGNAL_H_

#include "httpc.h"
#include "daps_trc.h"
#include "sci_types.h"
#include "os_api.h"
#include "in_message.h"
#include "tasks_id.h"
#include "httpc_api.h"

/*----------------------------------------------------------------------------*/
/*                         Compiler Flag                                      */
/*----------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif

PUBLIC  HTTPC_ERROR_E httpc_send_signal(xSignalHeader signal_ptr,BLOCK_ID thread_id);

PUBLIC void httpc_send_init_cnf(
         httpc_t* context, xSignalHeaderRec* init_req_sig,
	      HTTPC_ERROR_E error_code);
		 
PUBLIC void httpc_send_data_ind(
         httpc_t* context, HTTPC_APP_INSTANCE_T app_instance,
         char* data_ptr, uint32 len, uint32 response_code);
		 
PUBLIC void httpc_send_close_cnf( httpc_t* context, HTTPC_ERROR_E error_code);

#ifdef __cplusplus
    }
#endif
#endif  // End _NPS_SIGNAL_H