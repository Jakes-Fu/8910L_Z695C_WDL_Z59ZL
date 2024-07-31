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
/////////////////////////固话ROM配置////////////////////
//////////////////////////////////////////////////////////////////////////
//加载层
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
/////////////////////////内存配置////////////////////
//////////////////////////////////////////////////////////////////////////
//主内存内存类型 - media、asm、static
#define MCR_MEM_MAIN			MCR_MEMTYPE_DYNAMIC

//核心备用内存内存类型 - media、asm、static（例如加载层代码段以及运行内存）
#define MCR_MEM_KERNRESER		MCR_MEMTYPE_DYNAMIC

//应用备用内存内存类型 - media、asm、static(例如QQ挂机内存)
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
/////////////////////////静态内存配置////////////////////
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
/////////////////////////基本配置////////////////////
//////////////////////////////////////////////////////////////////////////
//固化层版本号
#define MCR_FIRMVERSION_OVERSEA		("100.03.04.014579")//海外版本

#define MCR_FIRMVERSION		("100.03.04.018962")

#ifndef  WIN32
#if !defined RELEASE_INFO  
#define __MCR_TESTMODE__
#endif
#endif
//由于目前加载层，QB,Q信，斗地主都存在问题，暂时先关闭掉横竖屏切换的支持
#if (defined SPREAD_PLAT_SC6800H) || (defined SPREAD_PLAT_SC8800G) || (defined SPREAD_PLAT_SC6530)  || (defined SPREAD_PLAT_SC8800G_PDA) 
//#define MCARE_3D_SUPPORT//动态横竖屏切换支持的宏
#endif


//此处展讯平台的宏来控制MCare的宏，需要FAE移植的时候，务必自行确认一遍
//平台的浏览器是否支持的宏
#if (defined BROWSER_SUPPORT) || (defined WAP_SUPPORT)
#define MCARE_BROWSER_SUPPORT
#endif
//厂商
#define MCR_VENDOR			("")//("HK")

//机型
#define MCR_TERMINALTYPE    ("")//("hd101")

//porting id removed and add it to file MHelperFunc

//平台
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


//展讯 Version
#define MCR_SPREAD_VER			SPREAD_SC6800H_1134

//此处展讯平台的宏来控制MCare的宏，需要FAE移植的时候，务必自行确认一遍
//键盘类型
#ifdef KEYPAD_TYPE_TYPICAL_KEYPAD
#define MCR_KEYTYPE			MKEYTYPE_NORMAL_KEYPAD
#elif defined KEYPAD_TYPE_QWERTY_KEYPAD
#define MCR_KEYTYPE			MKEYTYPE_QWERTY_KEYPAD
#elif defined KEYPAD_TYPE_COMPACT_KEYPAD
#define MCR_KEYTYPE			MKEYTYPE_NORMAL_KEYPAD
#elif defined KEYPAD_TYPE_FOUR_KEY_UNIVERSE
#define MCR_KEYTYPE			MKEYTYPE_NORMAL_KEYPAD
#endif


//此处展讯平台的宏来控制MCare的宏，需要FAE移植的时候，务必自行确认一遍
//触屏类型
#ifdef TOUCH_PANEL_SUPPORT
#define MCR_TOUCHTYPE		MTOUCHSCREEN_YES
#else
#define MCR_TOUCHTYPE		MTOUCHSCREEN_NO
#endif

//主存存储类型
#ifdef WIN32
#define MCR_STORAGE_TYPE    MCR_STORAGE_CARD
#else
#define MCR_STORAGE_TYPE    MCR_STORAGE_CARD
#endif


//最低存储要求
#define MCR_FS_REQ           (5*1024 * 1024)


//平台Ram内存 - 如果低于这个值需要修改一下
#define MCR_MEM_SIZE		 (1024 * 1024)


//此处展讯平台的宏来控制MCare的宏，需要FAE移植的时候，务必自行确认一遍
//3、4卡、wifi支持;如果手机支持相应项目，请打开该宏
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

//此处展讯平台的宏来控制MCare的宏，需要FAE移植的时候，务必自行确认一遍
#if (defined WLAN_SUPPORT) || (defined WLAN_PORTAL_SUPPORT)|| (defined WIFI_SUPPORT)
#define MCR_WIFI_SUPPORT
#endif

//设置网络链接中sim优先
#define MCARE_SET_SIM_PRIORITY

//是否支持多多后台运行(内存不足的时候，可以单独支持QQ或者Q信单独运行)
#define MCARE_SUPPORT_MULTI_BG
//QQ来消息是否点亮屏
//#define MCR_BLACKLIGHT_TURNON

//Q信支持挂后台的操作
#define MCARE_SUPPORT_QC_BG
//QQ支持挂后台的操作
#define MCARE_SUPPORT_QQ_BG

//UI配色方案
//Mcare平台的配色方案，目前支持3中颜色:    UI_THEME_BLUE,UI_THEME_PINK,UI_THEME_BROWN
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
