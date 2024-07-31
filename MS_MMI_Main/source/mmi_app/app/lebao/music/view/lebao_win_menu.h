#ifndef __LEBAO_WIN_MENU_H
#define __LEBAO_WIN_MENU_H

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
typedef enum
{
	LEBAO_MENU_ABOUT_LOGOUT = 100, 
	LEBAO_MENU_ABOUT_CLEAR_LOGIN_CACHE, 
	lebao_menu_about_max
} LEBAO_ABOUT_MENU_ID;

/**********************
* GLOBAL PROTOTYPES
**********************/
void lebao_menu_open(void* data);
void lebao_menu_close(void* data);
int  lebao_menu_is_opened(void);
int lebao_mainmenu_get_dynMenuIndex(int itemIndex);

void lebao_about_open(void* data);
void lebao_about_close(void* data);

/**********************
*      MACROS
**********************/

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /*__LEBAO_WIN_MENU_H*/