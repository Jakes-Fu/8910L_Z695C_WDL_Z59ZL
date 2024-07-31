#ifndef _MMICP_MAINWIN_H_    
#define  _MMICP_MAINWIN_H_    

/**----------------------------------------------------------------------*
 **                         Include Files                                *
 **----------------------------------------------------------------------*/
#include "window_parse.h"
#include "mmicp_id.h"
#include "mmicp_nv.h"
#include "mmidisplay_data.h"

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
 
#ifdef __cplusplus
    extern   "C"
    {
#endif
#define MMICP_BRIGHTNESS_LEVEL_MAX    MMISET_CONTRAST_SEVEN
#define MMICP_NONE_CTRL_ID    0XFFFF

#define SIM_STATUS_HEIGH    24
#define SIM_STATUS_LEFT_MARGIN    2
#define SIM_STATUS_RIGHT_MARGIN    6
#define SIM_STATUS_TOP_MARGIN    4
#define SIM_ICON_IMAGE_HEIGH    16
#define SIM_ICON_IMAGE_WIDTH    18

#define BT_DATA_MARGIN    6
#define BT_AND_DATA_HEIGH    60
#define BT_AND_DATA_ICON_HEIGH    24
#define BT_AND_DATA_ICON_WIDTH    24

#define VOLUME_BAR_HEIGH    64
#define VOLUME_BAR_WIDTH    117
#define BRIGHTNESS_BAR_HEIGH    64
#define BRIGHTNESS_BAR_WIDTH    117

#define MMICP_LCD_CONTRAST_SPAN    13
#define MMICP_LCD_CONTRSAT_MIN    9
#define MMICP_LCD_CONTRSAT_MAX    100

#define FOCUS_FRAME_SIZE    2
#define BT_DATA_TEXT_COLOR_OPACITY    (255*6/10)
#define BT_DATA_RECT_DEFAULT_COLOR    GUI_RGB2RGB565(32,32,32)

#define CP_MAIN_BG_COLOR    MMI_BLACK_COLOR
#define BT_TEXT_OFF_COLOR    MMI_WHITE_COLOR
#define DATA_TEXT_OFF_COLOR    MMI_WHITE_COLOR
#define SIM_TEXT_COLOR    MMI_WHITE_COLOR
#define CP_BANNER_DELAY_TIME 500

MMI_APPLICATION_T    g_control_panel_app = {0};

typedef enum
{
    CP_BT_STATE_OFF,
    CP_BT_STATE_ON,
    CP_BT_STATE_VISIBLE,
    CP_BT_STATE_CONNECTED
} CP_BT_STATE_E;

typedef enum
{
    CP_SIM_STATE_VALID,
    CP_SIM_STATE_NOT_FOUND,
    CP_SIM_STATE_INVALID
} CP_SIM_STATE_E;

typedef enum
{
    CP_EMERGENCY_GENERAL = 0,
    CP_EMERGENCY_CHINA = 1,
    CP_EMERGENCY_AUSTRALIA,
    CP_EMERGENCY_NEWZEALAND,
    CP_EMERGENCY_MAX
} CP_NDT_EMERGENCY_UI_TYPE;

typedef struct
{
    GUI_COLOR_T              rect_focus_color;
    GUI_COLOR_T              text_focus_color;
    MMI_IMAGE_ID_T           bt_on_image;  
    MMI_IMAGE_ID_T           data_on_image;
}MMI_CP_THEME_INFO_T;

typedef enum
{
    CP_SIM_SEL_MENU_NODE_ROOT,
    CP_SIM_SEL_MENU_SIM1,
    CP_SIM_SEL_MENU_SIM2,
    CP_SIM_SEL_MENU_ALWAYS_ASK
} MMI_CP_SIM_SELECT;

typedef enum
{ 
    MMICP_MENU_DICTIONARY_ITEM_ID,
    MMICP_LANGUAGE_DICTIONARY_OFF,//T9 Off
    MMICP_LANGUAGE_ENGLISH,        //Ӣ��
    MMICP_LANGUAGE_SIMP_CHINESE,    //���ļ��� 
    MMICP_LANGUAGE_TRAD_CHINESE,    //���ķ��壨̨�壩
    MMICP_LANGUAGE_ARABIC,         //������
    MMICP_LANGUAGE_FRENCH,         //����
    MMICP_LANGUAGE_HINDI,            //ӡ��
    MMICP_LANGUAGE_HUNGARIAN,        //������
    MMICP_LANGUAGE_INDONESIAN,     //ӡ��������
    MMICP_LANGUAGE_MALAY,            //����
    MMICP_LANGUAGE_PORTUGUESE,     //������
    MMICP_LANGUAGE_RUSSIAN,        //����
    MMICP_LANGUAGE_SPANISH,        //������
    MMICP_LANGUAGE_TAGALOG,        //����ŵ
    MMICP_LANGUAGE_THAI,            //̩��
    MMICP_LANGUAGE_VIETNAMESE,     //Խ����
    MMICP_LANGUAGE_URDU,            //�ڶ���
    MMICP_LANGUAGE_ITALIAN,        //�����
    MMICP_LANGUAGE_PERSIAN,        //��˹
    MMICP_LANGUAGE_TURKISH,        //������
    MMICP_LANGUAGE_GERMAN,         //����
    MMICP_LANGUAGE_GREEK,            //ϣ��
    MMICP_LANGUAGE_HEBREW,         //ϣ����
    MMICP_LANGUAGE_BENGALI,        //�ϼ���
    MMICP_LANGUAGE_CZECH,            //�ݿ�
    MMICP_LANGUAGE_SLOVENIAN,        //˹ŵ������
    MMICP_LANGUAGE_ROMANIAN,        //��������
    MMICP_LANGUAGE_TELUGU,
    MMICP_LANGUAGE_MARATHI,
    MMICP_LANGUAGE_TAMIL,
    MMICP_LANGUAGE_GUJARATI,
    MMICP_LANGUAGE_KANNADA,
    MMICP_LANGUAGE_MALAYALAM,
    MMICP_LANGUAGE_ORIYA,
    MMICP_LANGUAGE_PUNJABI,
    MMICP_LANGUAGE_AFRIKAANS,
    MMICP_LANGUAGE_ALBANIAN,
    MMICP_LANGUAGE_ARMENIAN,
    MMICP_LANGUAGE_AZERBAIJANI,
    MMICP_LANGUAGE_BASQUE,
    MMICP_LANGUAGE_BULGARIAN,
    MMICP_LANGUAGE_CATALAN,
    MMICP_LANGUAGE_CROATIAN,
    MMICP_LANGUAGE_DANISH,
    MMICP_LANGUAGE_DUTCH,
    MMICP_LANGUAGE_ESTONIAN,
    MMICP_LANGUAGE_FILIPINO,
    MMICP_LANGUAGE_FINNISH,
    MMICP_LANGUAGE_GALICIAN,
    MMICP_LANGUAGE_GEORGIAN,
    MMICP_LANGUAGE_HAUSA,
    MMICP_LANGUAGE_ICELANDIC,
    MMICP_LANGUAGE_IGBO,
    MMICP_LANGUAGE_IRISH,
    MMICP_LANGUAGE_KAZAKH,
    MMICP_LANGUAGE_LATVIAN,
    MMICP_LANGUAGE_LITHUANIAN,
    MMICP_LANGUAGE_MACEDONIAN,
    MMICP_LANGUAGE_MOLDOVAN,
    MMICP_LANGUAGE_NORWEGIAN,
    MMICP_LANGUAGE_POLISH,
    MMICP_LANGUAGE_SERBIAN,
    MMICP_LANGUAGE_SESOTHO,
    MMICP_LANGUAGE_SLOVAK,
    MMICP_LANGUAGE_SWEDISH,
    MMICP_LANGUAGE_UKRAINIAN,
    MMICP_LANGUAGE_YORUBA,
    
    MMICP_LANGUAGE_XHOSA,        //������    add. chenyg@spread. 2011-05-12
    MMICP_LANGUAGE_ZULU,        //��³��    add. chenyg@spread. 2011-05-12
    MMICP_LANGUAGE_ASSAMESE,        //(ӡ����֧��)����ķ��    add. chenyg@spread. 2011-05-12
    MMICP_LANGUAGE_SWAHILI,    //˹������    add. chenyg@spread. 2011-05-12
    MMICP_LANGUAGE_MYANMAR,    //�����
    MMICP_LANGUAGE_AMHARIC,    //��ķ����
    MMICP_LANGUAGE_KHMER,      //����կ
    MMICP_LANGUAGE_LAO,    //����
    MMICP_LANGUAGE_UYGHUR,    //ά�����
    MMICP_LANGUAGE_TIBETAN,   //����
    MMICP_LANGUAGE_SINHALESE,    //ɮ٤��
    MMICP_LANGUAGE_BOSNIAN, 
    MMICP_LANGUAGE_TRAD_CHINESE_HK,
    MMICP_LANGUAGE_AMERICAN,    //English-US
    MMICP_LANGUAGE_PORTUGUESE_BR,
    MMICP_LANGUAGE_TURKMEN_LATIN,
    MMICP_LANGUAGE_SPANISH_AM,
    MMICP_LANGUAGE_BELARUS,
    MMICP_LANGUAGE_LINGALA,
    MMICP_LANGUAGE_MALAGASY,
    MMICP_LANGUAGE_PASHTO,
    MMICP_LANGUAGE_UZBEK,
    MMICP_LANGUAGE_BENGALI_BD,
    MMICP_MAX_LANGUAGE,

    MMICP_MENU_MENU_MAX_ITEM_ID
}MMICP_IM_LANGUAGE_ID_MENU_E;

#ifdef   __cplusplus
    }
#endif

#endif
