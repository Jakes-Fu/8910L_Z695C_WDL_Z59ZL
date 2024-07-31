#ifndef __LEBAO_WIN_SEARCH_H
#define __LEBAO_WIN_SEARCH_H

#ifdef __cplusplus
extern "C" {
#endif

/*********************
*      INCLUDES
*********************/

/*********************
*      DEFINES
*********************/

/**********************
*      TYPEDEFS
**********************/

/**********************
* GLOBAL PROTOTYPES
**********************/
void lebao_search_open(void* data);
void lebao_search_close(void* data);
int lebao_search_is_opened(void);

void lebao_search_win_record_handle_msgid_get(MMI_MESSAGE_ID_E *start_msgid, MMI_MESSAGE_ID_E *stop_msgid);

/**********************
*      MACROS
**********************/

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /*__LEBAO_WIN_SEARCH_H*/