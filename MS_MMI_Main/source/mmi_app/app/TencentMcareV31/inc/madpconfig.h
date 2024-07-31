#ifndef _madpconfig_h
#define _madpconfig_h

#ifdef __cplusplus   
extern "C"{   
#endif // __cplusplus

#include "madptype.h"


//[some features for customize]
//__CFG_YES__,__CFG_NO__
#ifdef __MCARE_OVERSEA__
#define MCR_REGION_OVERSEA  __CFG_YES__ 
#else
#define MCR_REGION_OVERSEA  __CFG_NO__ 
#endif

//MTK Version
#define MCR_MTK_VER			MTK_VER_1012

//////////////////////////////////////////////////////////////////////////
/////////////////////////�̻�ROM����////////////////////
//////////////////////////////////////////////////////////////////////////
//���ز�
#define MCR_USE_ROM_LOADEDLAYER		__CFG_TMZ__
//QB
#if MCR_REGION_OVERSEA == __CFG_YES__
#define MCR_USE_ROM_QQB				__CFG_TMC__
#else
#define MCR_USE_ROM_QQB				__CFG_NONE__
#endif

//QQ
#define MCR_USE_ROM_QQ				__CFG_NONE__

//QQ
#define MCR_USE_ROM_QC				__CFG_NONE__


//////////////////////////////////////////////////////////////////////////
/////////////////////////�ڴ�����////////////////////
//////////////////////////////////////////////////////////////////////////
//���ڴ��ڴ����� - media��asm��static
#define MCR_MEM_MAIN			MCR_MEMTYPE_DYNAMIC

//���ı����ڴ��ڴ����� - media��asm��static��������ز������Լ������ڴ棩
#define MCR_MEM_KERNRESER		MCR_MEMTYPE_DYNAMIC

//Ӧ�ñ����ڴ��ڴ����� - media��asm��static(����QQ�һ��ڴ�)
#define MCR_MEM_APPRESER		MCR_MEMTYPE_DYNAMIC



#ifdef MSHELL_DEBUG
#define MDEBUG_EXTRA_MEM_SIZE   (120*1024)
#else
#define MDEBUG_EXTRA_MEM_SIZE   (0)
#endif

#if MCR_REGION_OVERSEA == __CFG_YES__
#define MKERN_MEM_SIZE 	(60 * 1024 + MDEBUG_EXTRA_MEM_SIZE)
#else
#define MKERN_MEM_SIZE 	(120 * 1024 + MDEBUG_EXTRA_MEM_SIZE)
#endif

//////////////////////////////////////////////////////////////////////////
/////////////////////////��̬�ڴ�����////////////////////
//////////////////////////////////////////////////////////////////////////
#if 1// add by nick for dubug 20111101
#define MCR_MEM_MAIN_STATIC_SIZE			(1024 * 800)
#define MCR_MEM_KERN_STATIC_SIZE			(MKERN_MEM_SIZE)
#define MCR_MEM_APP_STATIC_SIZE				(1024*100)
#else

#if MCR_MEM_MAIN == MCR_MEMTYPE_STATIC
#define MCR_MEM_MAIN_STATIC_SIZE			(1024 * 800)
#else
#define MCR_MEM_MAIN_STATIC_SIZE			(0)
#endif

#if MCR_MEM_KERNRESER == MCR_MEMTYPE_STATIC
#define MCR_MEM_KERN_STATIC_SIZE		    (MKERN_MEM_SIZE)
#else
#define MCR_MEM_KERN_STATIC_SIZE			(0)
#endif

#if MCR_MEM_APPRESER == MCR_MEMTYPE_STATIC
#define MCR_MEM_APP_STATIC_SIZE				(1024*100)
#else
#define MCR_MEM_APP_STATIC_SIZE				(0)
#endif

#endif//#if 1

#define MCR_MEM_STATIC_TOTAL    (MCR_MEM_MAIN_STATIC_SIZE+MCR_MEM_KERN_STATIC_SIZE+MCR_MEM_APP_STATIC_SIZE)





//////////////////////////////////////////////////////////////////////////
/////////////////////////��������////////////////////
//////////////////////////////////////////////////////////////////////////
//�̻���汾��
#define MCR_FIRMVERSION_OVERSEA		("100.03.04.014579")//����汾

#define MCR_FIRMVERSION		("100.03.04.018962")

#ifndef  WIN32
#if !defined RELEASE_INFO  
#define __MCR_TESTMODE__
#endif
#endif
//����Ŀǰ���ز㣬QB,Q�ţ����������������⣬��ʱ�ȹرյ��������л���֧��
#if (defined SPREAD_PLAT_SC6800H) || (defined SPREAD_PLAT_SC8800G) || (defined SPREAD_PLAT_SC6530)  || (defined SPREAD_PLAT_SC8800G_PDA) 
//#define MCARE_3D_SUPPORT//��̬�������л�֧�ֵĺ�
#endif


//�˴�չѶƽ̨�ĺ�������MCare�ĺ꣬��ҪFAE��ֲ��ʱ���������ȷ��һ��
//ƽ̨��������Ƿ�֧�ֵĺ�
#if (defined BROWSER_SUPPORT) || (defined WAP_SUPPORT)
#define MCARE_BROWSER_SUPPORT
#endif
//����
#define MCR_VENDOR			("")//("HK")

//����
#define MCR_TERMINALTYPE    ("")//("hd101")

//porting id removed and add it to file MHelperFunc

//ƽ̨
#ifdef SPREAD_PLAT_SC6800H
#define MCR_PLATFORMID		PLATFORMID_MCARE_SPREAD_SC6800H
#elif (defined SPREAD_PLAT_SC8800G) ||(defined SPREAD_PLAT_SC8800G_PDA) 
#define MCR_PLATFORMID		PLATFORMID_MCARE_SPREAD_SC8800G
#elif (defined SPREAD_PLAT_SC6530)
#define MCR_PLATFORMID		PLATFORMID_MCARE_SPREAD_SC6530
#elif (defined SPREAD_PLAT_SC6620)
#define MCR_PLATFORMID		PLATFORMID_MCARE_SPREAD_SC6620
#elif (defined SPREAD_PLAT_SC6610)
#define MCR_PLATFORMID		PLATFORMID_MCARE_SPREAD_SC6610
#elif (defined SPREAD_PLAT_SC6600L) || (defined SPREAD_PLAT_SC6600L_1025) || (defined SPREAD_PLAT_SC6600L_0937)
#define MCR_PLATFORMID		PLATFORMID_MCARE_SPREAD_SC6600L
#endif


//չѶ Version
#define MCR_SPREAD_VER			SPREAD_SC6800H_1134

//�˴�չѶƽ̨�ĺ�������MCare�ĺ꣬��ҪFAE��ֲ��ʱ���������ȷ��һ��
//��������
#ifdef KEYPAD_TYPE_TYPICAL_KEYPAD
#define MCR_KEYTYPE			MKEYTYPE_NORMAL_KEYPAD
#elif defined KEYPAD_TYPE_QWERTY_KEYPAD
#define MCR_KEYTYPE			MKEYTYPE_QWERTY_KEYPAD
#elif defined KEYPAD_TYPE_COMPACT_KEYPAD
#define MCR_KEYTYPE			MKEYTYPE_NORMAL_KEYPAD
#elif defined KEYPAD_TYPE_FOUR_KEY_UNIVERSE
#define MCR_KEYTYPE			MKEYTYPE_NORMAL_KEYPAD
#endif


//�˴�չѶƽ̨�ĺ�������MCare�ĺ꣬��ҪFAE��ֲ��ʱ���������ȷ��һ��
//��������
#ifdef TOUCH_PANEL_SUPPORT
#define MCR_TOUCHTYPE		MTOUCHSCREEN_YES
#else
#define MCR_TOUCHTYPE		MTOUCHSCREEN_NO
#endif

//����洢����
#ifdef WIN32
#define MCR_STORAGE_TYPE    MCR_STORAGE_CARD
#else
#define MCR_STORAGE_TYPE    MCR_STORAGE_CARD
#endif


//��ʹ洢Ҫ��
#define MCR_FS_REQ           (5*1024 * 1024)


//ƽ̨Ram�ڴ� - ����������ֵ��Ҫ�޸�һ��
#define MCR_MEM_SIZE		 (1024 * 1024)


//�˴�չѶƽ̨�ĺ�������MCare�ĺ꣬��ҪFAE��ֲ��ʱ���������ȷ��һ��
//3��4����wifi֧��;����ֻ�֧����Ӧ��Ŀ����򿪸ú�
#ifndef MMI_MULTI_SIM_SYS_SINGLE

#if (defined MMI_MULTI_SIM_SYS_QUAD) || (defined MMI_MULTI_SIM_SYS_TRI) || (defined MMI_MULTI_SIM_SYS_DUAL)
#define MCR_SIM2_SUPPORT  
#endif

#if (defined MMI_MULTI_SIM_SYS_QUAD) || (defined MMI_MULTI_SIM_SYS_TRI)
#define MCR_SIM3_SUPPORT
#endif

#ifdef MMI_MULTI_SIM_SYS_QUAD
#define MCR_SIM4_SUPPORT
#endif

#endif

//�˴�չѶƽ̨�ĺ�������MCare�ĺ꣬��ҪFAE��ֲ��ʱ���������ȷ��һ��
#if (defined WLAN_SUPPORT) || (defined WLAN_PORTAL_SUPPORT)|| (defined WIFI_SUPPORT)
#define MCR_WIFI_SUPPORT
#endif

//��������������sim����
#define MCARE_SET_SIM_PRIORITY

//�Ƿ�֧�ֶ���̨����(�ڴ治���ʱ�򣬿��Ե���֧��QQ����Q�ŵ�������)
#define MCARE_SUPPORT_MULTI_BG
//QQ����Ϣ�Ƿ������
//#define MCR_BLACKLIGHT_TURNON

//Q��֧�ֹҺ�̨�Ĳ���
#define MCARE_SUPPORT_QC_BG
//QQ֧�ֹҺ�̨�Ĳ���
#define MCARE_SUPPORT_QQ_BG

//UI��ɫ����
//Mcareƽ̨����ɫ������Ŀǰ֧��3����ɫ:    UI_THEME_BLUE,UI_THEME_PINK,UI_THEME_BROWN
#define UI_SYS_THEME_COLOR   (UI_THEME_BLUE)


//#if MCR_MTK_VER >= MTK_VER_952
//#define USE_SYSTEM_ZLIB    //whether to use system zlib..prefer to use to reduce rom space
//#endif

#define MCR_CAMERA_SUPPORT      __CFG_NO__

//[some features for customize]
#define MCR_SUPPORT_WAP_BROWSER     __CFG_NO__

#if MCR_REGION_OVERSEA == __CFG_YES__
#define USE_SYSBRW_DTCNT   //whether to use system browser's data account profile instead

#define __MCR_LANG_ENGLISH__            __CFG_YES__
#define __MCR_LANG_HINDI__              __CFG_NO__
#define __MCR_LANG_VIETNAMESE__         __CFG_NO__
#define __MCR_LANG_INDONESIAN__         __CFG_NO__
#define __MCR_LANG_THAI__               __CFG_NO__
//#define USE_SPLASH_LOGO  //whether to put splash logo on when entering to mcare
#else
//#define USE_SYSBRW_DTCNT   //
#define USE_SPLASH_LOGO  //
#endif


#ifdef __cplusplus   
}   
#endif // __cplusplus
#endif // _madpconfig_h
