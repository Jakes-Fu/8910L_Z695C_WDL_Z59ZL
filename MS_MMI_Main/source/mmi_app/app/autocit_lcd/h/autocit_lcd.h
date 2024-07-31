#ifndef _AUTOCIT_LCD_H_
#define _AUTOCIT_LCD_H_

#ifdef __cplusplus
extern   "C"
{
#endif

typedef enum
{
    autocit_lcd_type_white = 1,
    autocit_lcd_type_black,
    autocit_lcd_type_red,
    autocit_lcd_type_green,
    autocit_lcd_type_blue,
    autocit_lcd_type_white_black,
    autocit_lcd_type_black_white,
    autocit_lcd_type_8X8_grid,
    autocit_lcd_type_all,
}AUTOCIT_LCD_TYPE;

/*****************************************************************************/
// 	Description : AUTOCIT_OpenMainWin
//	Global resource dependence : 
//  Author: Cheney Wu
//	Note:
/*****************************************************************************/
PUBLIC void AUTOCIT_OpenMainWin(void);

/*****************************************************************************/
// 	Description : AUTOCIT_LCD_OpenMainWin
//	Global resource dependence : 
//  Author: Cheney Wu
//	Note:
/*****************************************************************************/
PUBLIC void AUTOCIT_LCD_OpenMainWin(AUTOCIT_LCD_TYPE type);

/*****************************************************************************/
// 	Description : AUTOCIT_LCD_CloseMainWin
//	Global resource dependence : 
//  Author: Cheney Wu
//	Note:
/*****************************************************************************/
PUBLIC void AUTOCIT_LCD_CloseMainWin(void);

#ifdef __cplusplus
}
#endif

#endif

