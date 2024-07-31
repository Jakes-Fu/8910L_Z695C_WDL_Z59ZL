/****************************************************************************
** File Name:      mmigame_snake_func.c                            
** Author:                                                            
** Date:           
** Copyright:      2004 Spreadtrum, Incorporated. All Rights Reserved.       
** Description:    This file is used to create windows of quintet
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE               NAME             DESCRIPTION                             
**
** 
****************************************************************************/
#define _MMIGAME_SNAKE_FUNC_C_  

/**--------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/
#ifdef WIN32
#include "std_header.h"
#endif
#ifdef GAME_SNAKE_SUPPORT
#include "window_parse.h"
#include "mmi_string.h"
#include "guilistbox.h"
#include "guimsgbox.h"
#include "mmidisplay_data.h" 
#include "guilcd.h"
#include "dal_img.h"
#include "tb_dal.h"
#include "mmi_nv.h"
#include "guiref_scale.h"
#include "mmk_timer.h"
#include "guilcd.h"
#include "guires.h"
#include "mmipub.h"
#include "mmigame_text.h"
#include "mmigame_snake_id.h"
#include "mmigame_snake_image.h"
#include "mmigame_snake_position.h" 
#include "mmigame_snake_text.h"   
#include "mmigame_snake_internal.h"
#include "mmigame_snake_nv.h"
/*---------------------------------------------------------------------------*/
/*                          MACRO DEFINITION                                 */
/*---------------------------------------------------------------------------*/
#if defined MAINLCD_SIZE_128X160
#define MMIGMSNAKE_GAME_FONT  MMI_DEFAULT_NORMAL_FONT
// ��ͣʱ��ʾ�����壬��Ҫ��Ŀ
#define MMIGMSNAKE_GAME_PAUSED_FONT  SONG_FONT_14
#elif defined MAINLCD_SIZE_176X220
#define MMIGMSNAKE_GAME_FONT  MMI_DEFAULT_NORMAL_FONT
// ��ͣʱ��ʾ�����壬��Ҫ��Ŀ
#define MMIGMSNAKE_GAME_PAUSED_FONT  SONG_FONT_20
#elif defined MAINLCD_SIZE_220X176
#define MMIGMSNAKE_GAME_FONT  MMI_DEFAULT_NORMAL_FONT
// ��ͣʱ��ʾ�����壬��Ҫ��Ŀ
#define MMIGMSNAKE_GAME_PAUSED_FONT  SONG_FONT_20
#elif defined MAINLCD_SIZE_240X320
#define MMIGMSNAKE_GAME_FONT  MMI_DEFAULT_NORMAL_FONT
// ��ͣʱ��ʾ�����壬��Ҫ��Ŀ
#define MMIGMSNAKE_GAME_PAUSED_FONT  SONG_FONT_24
#elif defined MAINLCD_SIZE_320X240
#define MMIGMSNAKE_GAME_FONT  MMI_DEFAULT_NORMAL_FONT
// ��ͣʱ��ʾ�����壬��Ҫ��Ŀ
#define MMIGMSNAKE_GAME_PAUSED_FONT  SONG_FONT_24
#elif defined MAINLCD_SIZE_240X400
#define MMIGMSNAKE_GAME_FONT  MMI_DEFAULT_NORMAL_FONT
// ��ͣʱ��ʾ�����壬��Ҫ��Ŀ
#define MMIGMSNAKE_GAME_PAUSED_FONT  SONG_FONT_24
#elif defined MAINLCD_SIZE_320X480
#define MMIGMSNAKE_GAME_FONT  MMI_DEFAULT_NORMAL_FONT
// ��ͣʱ��ʾ�����壬��Ҫ��Ŀ
#define MMIGMSNAKE_GAME_PAUSED_FONT  SONG_FONT_28
#else
// Ŀǰֻ��240*320��ʵ�֣������ֱ�������Ҫ�������С����ʱ������뱨����
#error
#endif
// ��Ϸ��д�������Ѷȵ�������ɫ
#define COLOR_SNAKE_STRING             MMI_BLUE_COLOR//������ɫ
/**--------------------------------------------------------------------------*
 **                         TYPE DEFINITION                                  *
 **--------------------------------------------------------------------------*/

typedef enum
{
    // ��ͷ�����ȷ���ߵķ���
    GRIDTYPE_GMSNAKE_HEAD,  
    // ����
    GRIDTYPE_GMSNAKE_BODY,
    // �߿��ԳԵķ���
    GRIDTYPE_GMSNAKE_FLOG, 
    GAMESNAKE_GRIDTYPE_MAX
}GAMESNAKE_GRIDTYPE_E;


/**---------------------------------------------------------------------------*
**                       LOCAL DATA DECLARATION
**---------------------------------------------------------------------------*/
// ��ǰ�ķ���
LOCAL  uint16       s_snake_currentscore = 0; 
// ���������ߵ�λ��snake_body_cache[0]Ϊ�ߵ�β�ͣ�s_snake_body_cache[s_snake_total-1]Ϊ��ͷ
//LOCAL  uint16       s_snake_body_cache[SNAKE_XGRIDS*SNAKE_YGRIDS] = {0};
LOCAL  uint16       *s_snake_body_cache = PNULL;
// ����������Ļ�еĸ��Ӹ����Ƿ��ǵ�ǰ�����ڵĵ�
//LOCAL  BOOLEAN      s_snake_is_snakebody[SNAKE_XGRIDS*SNAKE_YGRIDS] ={0} ; 
LOCAL  BOOLEAN       *s_snake_is_snakebody = PNULL;
// �߿��ԳԵķ����λ��
LOCAL  uint16       s_snake_flogindex = 0; 
// �ܹ�����(����)���Ե�һ�����鳤�ȼ�1.��ʼ�ݶ�4��
LOCAL  uint16       s_snake_total = 4;
// ����ͼƬ�����飬��ͬ���͸����ò�ͬͼƬ������GAMESNAKE_GRIDTYPE_E������һһ��Ӧ
LOCAL  const  MMI_IMAGE_ID_T s_snake_gridimg[] = 
    {IMAGE_GAME_SNAKE_HEAD,IMAGE_GAME_SNAKE_BODY,IMAGE_GAME_SNAKE_FLOG};
// ���˶��ķ���
PUBLIC  MMI_MESSAGE_ID_E    s_snake_direction = MSG_KEYDOWN_RIGHT;   
// �������ʱ�߸ı�ķ�����ݴ棬�ñ�����Ҫ��Ϊ�˿��ٰ����Ĵ���
PUBLIC  MMI_MESSAGE_ID_E    s_snake_direction_cache = 0;   
// ��ǰ��Ϸ���Ѷ�
PUBLIC  SNAKE_LEVEL_E       s_snake_current_level = SNAKE_LEVEL_EASY;      //��ǰ���Ѷ�
// ��ʱ�������˶���ʱ�䣬�����ѶȰ���ʱ��
PUBLIC uint8                s_snake_timer = 0;
// ��Ϸ״̬
PUBLIC SNAKE_GAME_STATE_E   s_snake_game_state = SNAKE_GAME_NULL;
// �Ѷȵȼ����ַ��������SNAKE_LEVEL_Eһһ��Ӧ
PUBLIC const MMI_TEXT_ID_T  s_snake_level_list[SNAKE_TOTAL_LEVEL] =
    {TXT_SNAKE_LEVEL_EASY,TXT_SNAKE_LEVEL_NORMAL,TXT_SNAKE_LEVEL_HARD,};

/**--------------------------------------------------------------------------*
**                         FUNCTION DECLARE                                                                           *
**--------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
**                       function bodies  -- local
**---------------------------------------------------------------------------*/

/*****************************************************************************/
//   FUNCTION:      MMIGMSNAKE_Init
//  Description :   
//
//  Global resource dependence : 
//   Author:        peilong.zhou
//  Note:   
/*****************************************************************************/
PUBLIC  BOOLEAN  MMIGMSNAKE_Init(void)
{
    uint16 nv_return = 0;
    uint16 high_score = 0;
    uint8 level[2] = {0};
    //��ʼ��nv
    MMINV_READ(MMINV_GMSNAKE_LEVER_SET, level, nv_return);
    if (MN_RETURN_SUCCESS != nv_return)
    {
        SCI_MEMSET(&level[0], 0, sizeof(level));
        MMINV_WRITE(MMINV_GMSNAKE_LEVER_SET, &level[0]);
    }
    MMINV_READ(MMINV_GMSNAKE_HIGH_SCORE, &high_score, nv_return);   
    if (MN_RETURN_SUCCESS != nv_return)
    {
        high_score = 0;
        MMINV_WRITE(MMINV_GMSNAKE_HIGH_SCORE, &high_score);
    }
    //levelֻ����level[1]����һ��������
    if (level[1] >= SNAKE_TOTAL_LEVEL)
    {
        level[1] = SNAKE_LEVEL_EASY;
    }
    s_snake_current_level = (SNAKE_LEVEL_E)level[1];
    return TRUE;
    
}

/*****************************************************************************/
//   FUNCTION:      MMIGMQUT_InitModule
//  Description :   
//
//  Global resource dependence : 
//   Author:        peilong.zhou
//  Note:   
/*****************************************************************************/
PUBLIC void MMIGMSNAKE_InitModule(void)
{
    //��ʼ��im menu
    MMIGMSNAKE_RegMenuGroup();  
    //��ʼ��im nv
    MMIGMSNAKE_RegNv();   
    MMIGAME_SNAKE_RegWinIdNameArr();
}


/*****************************************************************************/
//  FUNCTION:     Snake_MsgBox_Gameover
//  Description :   
//  Global resource dependence : 
//  Author:       peilong.zhou
//  Note:   
/*****************************************************************************/
PUBLIC void  Snake_MsgBox_Gameover(void)
{
    //��ʾ��Ϸ����������ؿ����Ҽ��˳�
    MMIPUB_OpenQueryWinByTextId(TXT_SNAKE_GAMEOVER,IMAGE_PUBWIN_QUERY,PNULL,PNULL);
}


/*****************************************************************************/
//  FUNCTION:     Snake_ResetFrog
//  Description :   �����߿ɳԵķ����λ��
//  Global resource dependence :
//  Author:       peilong.zhou
//  Note:   
/*****************************************************************************/
LOCAL BOOLEAN  Snake_ResetFrog(void)
{
    uint32 randomnum = 0;
    uint16 i = 0;
    // ��������
    randomnum = rand();
    //GETRAND((uint8*)&randomnum,2);
    // ��ȥ�߿����ܵĵ㣬���м�((SNAKE_XGRIDS-2)*(SNAKE_YGRIDS-2))����ŷ���
    randomnum = randomnum%((SNAKE_XGRIDS-2)*(SNAKE_YGRIDS-2));
    randomnum = (randomnum/(SNAKE_XGRIDS-2)+1)*(SNAKE_XGRIDS)+randomnum%(SNAKE_XGRIDS-2)+1;
    s_snake_flogindex = randomnum;
    // ������������������򽫷��������β������֤���鲻��������
    for (i = 0;i<s_snake_total;i++)
    {
        if (s_snake_body_cache[i] == randomnum)
        {
            s_snake_flogindex = s_snake_body_cache[0];
            break;
        }
    }
    return TRUE;
}

/*****************************************************************************/
//  FUNCTION:     Snake_AllocGameSpace
//  Description : ������Ϸ��Ҫ���ڴ�ռ�
//  Global resource dependence : 
//  Author:       leon.wang
//  Note:   
/*****************************************************************************/
PUBLIC BOOLEAN Snake_AllocGameSpace(void)
{
    
    if(PNULL != s_snake_body_cache)
    {
        SCI_FREE(s_snake_body_cache);
        s_snake_body_cache = PNULL;
    }
    if(PNULL != s_snake_is_snakebody)
    {
        SCI_FREE(s_snake_is_snakebody);
        s_snake_is_snakebody = PNULL;
    }

    s_snake_body_cache = (uint16*)SCI_ALLOCAZ(sizeof(uint16)*(SNAKE_XGRIDS*SNAKE_YGRIDS));
    s_snake_is_snakebody=(BOOLEAN*)SCI_ALLOCAZ(sizeof(BOOLEAN)*(SNAKE_XGRIDS*SNAKE_YGRIDS));

    if(PNULL == s_snake_is_snakebody || PNULL == s_snake_body_cache)
    {
        if(PNULL != s_snake_body_cache)
        {
            SCI_FREE(s_snake_body_cache);
            s_snake_body_cache = PNULL;
        }
        if(PNULL != s_snake_is_snakebody)
        {
            SCI_FREE(s_snake_is_snakebody);
            s_snake_is_snakebody = PNULL;
        }
        return FALSE;
    }

    return TRUE;
}

/*****************************************************************************/
//  FUNCTION:     Snake_InitGame
//  Description :   ��ʼ����Ϸ
//  Global resource dependence : 
//  Author:       peilong.zhou
//  Note:   
/*****************************************************************************/
PUBLIC BOOLEAN Snake_InitGame(void)
{

    if(PNULL == s_snake_is_snakebody || PNULL == s_snake_body_cache)
    {
        return FALSE;
    }
    else
    {
        // ��Ĭ��ֵ�������
        SCI_MEMSET(s_snake_body_cache,0x0,sizeof(uint16)*(SNAKE_XGRIDS*SNAKE_YGRIDS));
        SCI_MEMSET(s_snake_is_snakebody,0x0,sizeof(BOOLEAN)*(SNAKE_XGRIDS*SNAKE_YGRIDS));
    }

    s_snake_currentscore = 0;
    // Ĭ��4��
    s_snake_total = 4; 
    // Ĭ���з�������
    s_snake_direction = MSG_KEYDOWN_RIGHT; 
    // Ĭ���ߴ�������
    s_snake_body_cache[0] = 0;
    s_snake_body_cache[1] = 1;
    s_snake_body_cache[2] = 2;
    s_snake_body_cache[3] = 3;
    // Ĭ��4���ĵ������Ϊ����
    s_snake_is_snakebody[0] = 1;
    s_snake_is_snakebody[1] = 1;
    s_snake_is_snakebody[2] = 1;
    s_snake_is_snakebody[3] = 1;
    // ��Ϸ״̬��Ϊ��ʼ
    s_snake_game_state = SNAKE_GAME_ON; 
    // ��ʼһ���������
    Snake_ResetFrog();

    return TRUE;
}

/*****************************************************************************/
//  FUNCTION:     Snake_ExitGame
//  Description :   ������Ϸ
//  Global resource dependence : 
//  Author:       leon.wang
//  Note:   
/*****************************************************************************/
PUBLIC void Snake_ExitGame(void)
{  
    if(PNULL != s_snake_body_cache)
    {
        SCI_FREE(s_snake_body_cache);
        s_snake_body_cache = PNULL;
    }
    if(PNULL != s_snake_is_snakebody)
    {
        SCI_FREE(s_snake_is_snakebody);
        s_snake_is_snakebody = PNULL;
    }
}

/*****************************************************************************/
//  FUNCTION:     Snake_DrawGrid
//  Description :   ���ߵĸ���
//  Global resource dependence : 
//  Author:       peilong.zhou
//  Note:   
/*****************************************************************************/
LOCAL BOOLEAN Snake_DrawGrid(MMI_WIN_ID_T win_id,uint16 nIndex,GAMESNAKE_GRIDTYPE_E gridtype)
{
    GUI_RECT_T Rc = {0};
    GUI_LCD_DEV_INFO    lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
    // SNAKE_START_X��SNAKE_START_YΪ��Ϸ��ͼ���ӵ����
    // SNAKE_XGRIDS��SNAKE_YGRIDSΪ���ݸ�����
    // SNAKE_GRID_WIDTH,SNAKE_GRID_HEIGHTΪÿ��������ռ������
    Rc.left     = SNAKE_START_X + nIndex%SNAKE_XGRIDS*SNAKE_GRID_WIDTH;
    Rc.right    = Rc.left + SNAKE_GRID_WIDTH;
    Rc.top      = SNAKE_START_Y + nIndex/SNAKE_XGRIDS*SNAKE_GRID_HEIGHT;
    Rc.bottom = Rc.top + SNAKE_GRID_HEIGHT;

    // ��ͬ�������ò�ͬ��ͼ��
    GUIRES_DisplayImg(
                    PNULL,
                    &Rc,
                    PNULL,
                    win_id,
                    s_snake_gridimg[gridtype],
                    &lcd_dev_info);    

   // GUI_FillRect(&lcd_dev_info,Rc,clrFrame);
    return TRUE;
}

/*****************************************************************************/
//  FUNCTION:     MMIGMSNAKE_AddTwoString
//  Description :    ���������ִ�Ϊһ��
//  Global resource dependence : 
//  Author:      
//  Note:   
/*****************************************************************************/
PUBLIC MMI_STRING_T MMIGMSNAKE_AddTwoString(
                                          MMI_STRING_T first, 
                                          MMI_STRING_T second
                                          )
{
    MMI_STRING_T result = {0};
    uint16 j = 0;
    result.wstr_len = first.wstr_len + second.wstr_len;
    result.wstr_ptr = (wchar*)SCI_ALLOC_APP( (result.wstr_len + 1) * sizeof(wchar) );

    if (PNULL == result.wstr_ptr)
    {
        result.wstr_len = 0;
        return result;
    }
    
    MMI_WSTRNCPY( result.wstr_ptr, result.wstr_len, first.wstr_ptr, first.wstr_len, first.wstr_len );
    j = first.wstr_len;   
    MMI_WSTRNCPY( result.wstr_ptr+j, result.wstr_len-j, second.wstr_ptr, second.wstr_len, second.wstr_len );
    return result;
    
} 

/*****************************************************************************/
//  FUNCTION:     DrawTipString
//  Description :   ����
//  Global resource dependence : s_links_tip_times
//  Author:       Hao.Zhang
//  Note:   
/*****************************************************************************/
LOCAL void Snake_DrawTipString(void)
{
    GUI_POINT_T         startpoint = {0};
    GUI_RECT_T          scorerect = SNAKE_SCORE_STRING_RECT;
    GUI_RECT_T          levelrect = SNAKE_LEVEL_STRING_RECT;
    GUI_LCD_DEV_INFO    lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN}; 
    
    uint8   asc_str[12] = {0};
    wchar   wstr[12] = {0};
    MMI_STRING_T  str2 = {0};
    MMI_STRING_T  temp_str = {0};
    MMI_STRING_T  result = {0};
    GUISTR_STYLE_T str_style ={0};

    str_style.font = MMIGMSNAKE_GAME_FONT;
    str_style.font_color = COLOR_SNAKE_STRING;

    MMI_GetLabelTextByLang(TXT_SNAKE_SCORE, &temp_str);
    sprintf((char*)asc_str,":%d",s_snake_currentscore);
      
    str2.wstr_len = strlen((char*)asc_str);     
    str2.wstr_ptr = wstr;
    MMI_STRNTOWSTR( wstr, 12, asc_str, 12, str2.wstr_len );
    result = MMIGMSNAKE_AddTwoString(temp_str,str2);
    if(PNULL != result.wstr_ptr && 0 != result.wstr_len)
    {
        // ��Ļ��д��"����:XX" 
        str_style.align = ALIGN_LEFT;
        GUISTR_DrawTextToLCDInRect( 
                                    &lcd_dev_info, 
                                    &scorerect, 
                                    &scorerect,
                                    &result,
                                    &str_style,
                                    GUISTR_STATE_ALIGN,
                                    0); 
              
        SCI_FREE(result.wstr_ptr);
    }

    startpoint.x = scorerect.left;
    startpoint.y = scorerect.top;
    MMI_GetLabelTextByLang(s_snake_level_list[s_snake_current_level], &temp_str);
    // ��Ļ��д�ϵȼ�"��"
    str_style.align = ALIGN_RIGHT;
    GUISTR_DrawTextToLCDInRect( 
                                    &lcd_dev_info, 
                                    &levelrect, 
                                    &levelrect,
                                    &temp_str,
                                    &str_style,
                                    GUISTR_STATE_ALIGN,
                                    0); 


}

/*****************************************************************************/
//  FUNCTION:     Snake_Draw
//  Description :   ��Ϸ�滭
//  Global resource dependence : 
//  Author:       peilong.zhou
//  Note:   
/*****************************************************************************/
LOCAL void Snake_Draw(MMI_WIN_ID_T win_id)
{
    GUI_LCD_DEV_INFO    lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
    uint32              i = 0;
    uint32              snake_len = s_snake_total-1;

    // ������
#ifdef MMIGAME_SNAKE_LOW_MEMORY      //������Դʱ������ʹ����ɫƽ��
    {
        GUI_RECT_T      rect =MMITHEME_GetFullScreenRect();
   
        //���󱳾�
        GUI_FillRect(&lcd_dev_info, rect, MMI_DARK_YELLOW_COLOR);
        
        //����м䳡��
        rect.left = SNAKE_START_X;
        rect.right = SNAKE_START_X+SNAKE_XGRIDS*SNAKE_GRID_WIDTH;
        rect.top = SNAKE_START_Y;
        rect.bottom = SNAKE_START_Y+SNAKE_YGRIDS*SNAKE_GRID_HEIGHT;
        GUI_FillRect(&lcd_dev_info, rect, MMI_DARK_GREEN_COLOR);
    }
#else
    {
        GUI_POINT_T     point = {0, 0};
        //����ͼƬ��ʽ
        GUIRES_DisplayImg(
                        &point,
                        PNULL,
                        PNULL,
                        win_id,
                        IMAGE_GAME_SNAKE_BG,
                        &lcd_dev_info);
    }
#endif
    //�ò��ֶ����ݵĴ�����Ӧ�÷��ڻ�ͼ�����У���Ҫ�Ƴ�ȥ
    // �����ͼ�ߵ�״
    SCI_MEMSET(s_snake_is_snakebody,0x0,sizeof(BOOLEAN)*(SNAKE_XGRIDS*SNAKE_YGRIDS));
    // ������
    for (i = 0; i<snake_len; i++)
    {
        Snake_DrawGrid(win_id, s_snake_body_cache[i], GRIDTYPE_GMSNAKE_BODY);
        s_snake_is_snakebody[s_snake_body_cache[i]] = TRUE;
    }
    // ����ͷ
    Snake_DrawGrid(win_id,s_snake_body_cache[i],GRIDTYPE_GMSNAKE_HEAD);
    s_snake_is_snakebody[s_snake_body_cache[i]] = TRUE;
    // ������
    Snake_DrawGrid(win_id,s_snake_flogindex,GRIDTYPE_GMSNAKE_FLOG);
    // ���������Ѷ�
    Snake_DrawTipString();
}

/*****************************************************************************/
//  FUNCTION:     Snake_SetRunDirection
//  Description :   ͨ�����˶��ķ��������ߵ�״̬�ͳ���
//  Global resource dependence : 
//  Author:       peilong.zhou
//  Note:   ����false��gameover
/*****************************************************************************/
LOCAL BOOLEAN Snake_SetRunDirection(void)
{
    BOOLEAN bResult = FALSE;
    // ����Ѿ������ı䷽���򱣴����ڷ���
    if(s_snake_direction_cache != 0)
    {
        s_snake_direction = s_snake_direction_cache;
        s_snake_direction_cache = 0;
    }
    // ͨ���ߵķ��������ߵ�״̬
    switch (s_snake_direction)
    {
        case MSG_KEYDOWN_RIGHT:
            {
                // ����Ѿ������ұߣ������ң�gameover
                if (SNAKE_XGRIDS <= s_snake_body_cache[s_snake_total-1]%SNAKE_XGRIDS+1)
                {
                    return FALSE;
                }
                // ����ұ����������ڣ������ң�gameover
                if (s_snake_is_snakebody[s_snake_body_cache[s_snake_total-1]+1])
                {
                    return FALSE;
                }
                // ����ұ��ǿ��ԳԵķ��飬�������
                else if (s_snake_flogindex == s_snake_body_cache[s_snake_total-1]+1)
                {
                    s_snake_body_cache[s_snake_total] = s_snake_flogindex;
                    s_snake_is_snakebody[s_snake_flogindex] = TRUE;
                    s_snake_currentscore++;
                    s_snake_total++;
                    Snake_ResetFrog();
                    bResult = TRUE;
                }
                // �������״̬�����ǣ���������һ��
                else
                {
                    SCI_MEMCPY((void *)s_snake_body_cache,(const void *)&s_snake_body_cache[1],(s_snake_total-1)*sizeof(uint16));
                    s_snake_body_cache[s_snake_total-1] = s_snake_body_cache[s_snake_total-1]+1;
                    bResult = TRUE;
                }
            }
            break;
        case MSG_KEYDOWN_LEFT:
            {
                // ����Ѿ�������ߣ�������gameover
                if (0 == s_snake_body_cache[s_snake_total-1]%SNAKE_XGRIDS)
                {
                    return FALSE;
                }
                // ���������������ڣ�������gameover
                if (s_snake_is_snakebody[s_snake_body_cache[s_snake_total-1]-1])
                {
                    return FALSE;
                }
                // �������ǿ��ԳԵķ��飬�������
                else if (s_snake_flogindex == s_snake_body_cache[s_snake_total-1]-1)
                {
                    s_snake_body_cache[s_snake_total] = s_snake_flogindex;
                    s_snake_is_snakebody[s_snake_flogindex] = TRUE;
                    s_snake_currentscore++;
                    s_snake_total++;
                    Snake_ResetFrog();
                    bResult = TRUE;
                }
                // �������״̬�����ǣ���������һ��
                else
                {
                    SCI_MEMCPY((void *)s_snake_body_cache,(const void *)&s_snake_body_cache[1],(s_snake_total-1)*sizeof(uint16));
                    s_snake_body_cache[s_snake_total-1] = s_snake_body_cache[s_snake_total-1]-1;
                    bResult = TRUE;
                }
            }
            break;
        case MSG_KEYDOWN_DOWN:
            {
                // ����Ѿ������±ߣ������£�gameover
                if ((SNAKE_YGRIDS-1) <= (s_snake_body_cache[s_snake_total-1]/SNAKE_XGRIDS))
                {
                    return FALSE;
                }
                // ����±����������ڣ������£�gameover
                if (s_snake_is_snakebody[s_snake_body_cache[s_snake_total-1]+SNAKE_XGRIDS])
                {
                    return FALSE;
                }
                // ����±��ǿ��ԳԵķ��飬�������
                else if (s_snake_flogindex == s_snake_body_cache[s_snake_total-1]+SNAKE_XGRIDS)
                {
                    s_snake_body_cache[s_snake_total] = s_snake_flogindex;
                    s_snake_is_snakebody[s_snake_flogindex] = TRUE;
                    s_snake_currentscore++;
                    s_snake_total++;
                    Snake_ResetFrog();
                    bResult = TRUE;
                }
                // �������״̬�����ǣ���������һ��
                else
                {
                    SCI_MEMCPY((void *)s_snake_body_cache,(const void *)&s_snake_body_cache[1],(s_snake_total-1)*sizeof(uint16));
                    s_snake_body_cache[s_snake_total-1] = s_snake_body_cache[s_snake_total-1]+SNAKE_XGRIDS;
                    bResult = TRUE;
                }
            }
            break;
        case MSG_KEYDOWN_UP:
            {
                // ����Ѿ������ϱߣ������ϣ�gameover
                if (0 == s_snake_body_cache[s_snake_total-1]/SNAKE_XGRIDS)
                {
                    return FALSE;
                }
                // ����ϱ����������ڣ������ϣ�gameover
                if (s_snake_is_snakebody[s_snake_body_cache[s_snake_total-1]-SNAKE_XGRIDS])
                {
                    return FALSE;
                }
                // ����ϱ��ǿ��ԳԵķ��飬�������
                else if (s_snake_flogindex == s_snake_body_cache[s_snake_total-1]-SNAKE_XGRIDS)
                {
                    s_snake_body_cache[s_snake_total] = s_snake_flogindex;
                    s_snake_is_snakebody[s_snake_flogindex] = TRUE;
                    s_snake_currentscore++;
                    s_snake_total++;
                    Snake_ResetFrog();
                    bResult = TRUE;
                }
                // �������״̬�����ǣ���������һ��
                else
                {
                    SCI_MEMCPY((void *)s_snake_body_cache,(const void *)&s_snake_body_cache[1],(s_snake_total-1)*sizeof(uint16));
                    s_snake_body_cache[s_snake_total-1] = s_snake_body_cache[s_snake_total-1]-SNAKE_XGRIDS;
                    bResult = TRUE;
                }
            }
            break;
        default:
            //{
                // ��״̬��Ӧ�ô���
                //SCI_ASSERT(0);
           // }
            break;
    }
    return bResult;
}

/*****************************************************************************/
//  FUNCTION:     Snake_GameGoing
//  Description :   
//  Global resource dependence : s_snake_timer
//  Author:       peilong.zhou
//  Note:   
/*****************************************************************************/
PUBLIC void  Snake_GameGoing(MMI_WIN_ID_T win_id)
{
    if (0 != s_snake_timer)  
    {
        MMK_StopTimer(s_snake_timer);
        s_snake_timer = 0;
    }
    if (SNAKE_GAME_ON != s_snake_game_state)
    {
        GUIWIN_SetSoftkeyTextId(win_id,  TXT_NULL, TXT_COMMON_COMMON_CONTINUE, (MMI_TEXT_ID_T)STXT_RETURN, FALSE);  
    }
    else
    {
        GUIWIN_SetSoftkeyTextId(win_id,  TXT_NULL, TXT_SNAKE_PAUSE, (MMI_TEXT_ID_T)STXT_RETURN, FALSE);  
    }
    // ��Ϸ��ͣ��Ҫ��Ŀ��ʾ
    if (SNAKE_GAME_PAUSE == s_snake_game_state)
    {
        MMI_STRING_T        name_str = {0};
        GUI_LCD_DEV_INFO    lcd_dev_info = {GUI_MAIN_LCD_ID,GUI_BLOCK_MAIN};
        GUI_RECT_T          dis_rect = MMITHEME_GetClientRect();
        GUISTR_STYLE_T      str_style = {0};
        MMI_GetLabelTextByLang(TXT_SNAKE_GAMEPAUSED, &name_str);
        // ��Ϸ���н������滭
        Snake_Draw(win_id);
        str_style.font = MMIGMSNAKE_GAME_PAUSED_FONT;
        str_style.font_color = MMI_RED_COLOR;
        str_style.align = ALIGN_HVMIDDLE;
        GUISTR_DrawTextToLCDInRect( 
                                    &lcd_dev_info, 
                                    &dis_rect, 
                                    &dis_rect,
                                    &name_str,
                                    &str_style,
                                    GUISTR_STATE_ALIGN,
                                    0);
        return;
    }
    // û����on��״ֱ̬���˳� 
    else if (SNAKE_GAME_OVER == s_snake_game_state)
    {
        Snake_Draw(win_id);
        return;
    }
    else if (SNAKE_GAME_ON != s_snake_game_state)
    {
        return;
    }
    
    // �����ߵķ������false��gameover
    if (!Snake_SetRunDirection())
    {  
        s_snake_game_state = SNAKE_GAME_OVER;
    }
    // ��Ϸ���н������滭
    Snake_Draw(win_id);
    // gameoverʱ����ʾ
    if (SNAKE_GAME_OVER == s_snake_game_state)
    {
        Snake_SaveGameScore();
        // ��Ϸ���н������滭
        Snake_Draw(win_id);
        Snake_MsgBox_Gameover();
        return;
    }
    // ���ݲ�ͬ���Ѷȣ�����timer
    if (s_snake_current_level == SNAKE_LEVEL_EASY)
    {
        s_snake_timer = MMK_CreateTimer( 500, FALSE);
    }
    else if (s_snake_current_level == SNAKE_LEVEL_NORMAL)
    {
        s_snake_timer = MMK_CreateTimer( 300, FALSE);
    }
    else if (s_snake_current_level == SNAKE_LEVEL_HARD)
    {
        s_snake_timer = MMK_CreateTimer( 100, FALSE);
    }
}

/*****************************************************************************/
//  FUNCTION:     Snake_SaveGameScore
//  Description :   ��¼��Ϸ����
//  Global resource dependence : 
//  Author:       peilong.zhou
//  Note:   
/*****************************************************************************/
PUBLIC void Snake_SaveGameScore(void)
{
    uint16 high_score = 0;
    uint16 nv_return = 0;   
    // ��nv
    MMINV_READ(MMINV_GMSNAKE_HIGH_SCORE, &high_score, nv_return);    
    if (MN_RETURN_SUCCESS != nv_return)
    {
        high_score = 0;
        MMINV_WRITE(MMINV_GMSNAKE_HIGH_SCORE, &high_score);
    }
    // �������������߷֣����nv
    if (s_snake_currentscore > high_score)  
    {   
        high_score = s_snake_currentscore;  
        MMINV_WRITE(MMINV_GMSNAKE_HIGH_SCORE,&high_score);  
    }   
}

/*****************************************************************************/
//  FUNCTION:     Snake_GetLevelText
//  Description :   ��ȡlevel��Ӧ���ַ���
//  Global resource dependence : 
//  Author:      leon.wang
//  Note:   
/*****************************************************************************/
PUBLIC MMI_TEXT_ID_T Snake_GetLevelText(uint16 level)
{
    if(SNAKE_TOTAL_LEVEL>level)
    {
        return s_snake_level_list[level];
    }
    else
    {
        return s_snake_level_list[0];
    }

}
#endif

