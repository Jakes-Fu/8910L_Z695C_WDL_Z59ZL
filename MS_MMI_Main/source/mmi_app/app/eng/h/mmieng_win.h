/****************************************************************************
** File Name:      mmieng_win.h			                                    *
** Author:         allen				                                    *
** Date:           2004.09.10												*
** Copyright:      2003 Spreadtrum, Incoporated. All Rights Reserved.		*
** Description:    									               			*
*****************************************************************************
**                         Important Edit History							*
** -------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             	*
** 09/2004      allen               Create
** 
****************************************************************************/
#ifndef  _MMIENG_WIN_H_    
#define  _MMIENG_WIN_H_   

#include "mmk_app.h"
#include "mmidisplay_data.h"
#include "guicommon.h"
#include "guitext.h"
#include "mn_type.h"
#include "mmisrveng.h"

#ifdef __cplusplus
    extern "C"
    {
#endif

#define ENG_WIN_MAX_CHAR		18

#define ENG_SHOWRESULT_TIMER_DELAY	6000
#define ENG_TARGET_MAX_LEN		((MN_SMS_ASSERT_DEST_ADDR_LEN_MAX - 1) * 2)

typedef struct
{
	GUI_FONT_T 					text_font;      
	GUI_COLOR_T 				text_color;    
	GUI_COLOR_T 				spec_text_color;    
	GUI_COLOR_T 				bkgd_color;    
	uint8						char_space;
	uint8						line_space;
	uint16						scrollbar_width;
} MMI_ENG_DISPLAY_T;
	
#define MMI_MAX_SN_LEN              (24)


/* ***************************************************************************
                    
                      SP09 phase check structure

   *************************************************************************** */ 
#ifdef WIN32
#pragma pack(4)
#endif
//

#define CHANGE_ENDIAN_SEQUENCE_32(v)      ((v&0x000000ff)<<24 | (v&0x0000ff00)<<8 \
                                          | (v&0x00ff0000)>>8 |(v&0xff000000) >> 24) 
#define CHANGE_ENDIAN_SEQUENCE_16(v)      ((v&0x00ff)<<8 | (v&0xff00)>>8)
 
#define SP09_MAX_SN_LEN 				    MMI_MAX_SN_LEN
#define SP09_MAX_STATION_NUM		   	    (15)
#define SP09_MAX_STATION_NAME_LEN		    (10)
#define SP09_SPPH_MAGIC_NUMBER              (0X53503039)    // "SP09"   little-endian
#define SP09_MAX_LAST_DESCRIPTION_LEN       (32)

typedef enum
{
    PASS_1_FAIL_0  = 0x00,      // 1: pass, 0: fail
    PASS_0_FAIL_1  = 0x01,      // 1: fail, 0: pass

}SP09_SIGN_FLAG;

typedef struct _tagSP09_PHASE_CHECK
{ 
    uint32 Magic;                // "SP09"   (¨¤??¨®?¨²?aSP05)
    char    SN1[SP09_MAX_SN_LEN];       // SN , SN_LEN=24
    char    SN2[SP09_MAX_SN_LEN];       // add for Mobile
    int     StationNum;                 // the test station number of the testing
    char    StationName[SP09_MAX_STATION_NUM][SP09_MAX_STATION_NAME_LEN]; //15*10
    //BYTE    Reserved[13];               // 1¨¬?¡§?a0
    uint8    Reserved[13];               // 1¨¬?¡§?a0
    //BYTE    SignFlag;                   // internal flag 
    uint8    SignFlag;                   // internal flag 
    char    szLastFailDescription[SP09_MAX_LAST_DESCRIPTION_LEN]; //32
    //unsigned short  iTestSign;          // Bit0~Bit14 ---> station0~station 14  if tested. 0: tested, 1: not tested 
    uint16  iTestSign;          // Bit0~Bit14 ---> station0~station 14  if tested. 0: tested, 1: not tested     
    //unsigned short  iItem;              // part1: Bit0~ Bit_14 indicate test Station, 0: Pass, 1: fail    
    uint16  iItem;              // part1: Bit0~ Bit_14 indicate test Station, 0: Pass, 1: fail    
                                        // part2: Bit15 ¡À¨º??????3¨¦0 	
}SP09_PHASE_CHECK_T;


#define SP09_MAX_PHASE_BUFF_SIZE  sizeof(SP09_PHASE_CHECK_T)

#ifdef WIN32
#pragma pack()
#endif

	
#ifdef ENG_SUPPORT
/********************************************************************************
 NAME:			MMIENG_OpenGPSTestWin
 DESCRIPTION:	
 PARAM IN:		
 PARAM OUT:		
 AUTHOR:		jian.ma
 DATE:		0	2007.07.23
********************************************************************************/
PUBLIC void MMIENG_OpenGPSTestWin(void);

/*****************************************************************************/
// 	Description : start pingtest for pc link
//	Global resource dependence : none
//  Author: zhaohui
//	Note:
/*****************************************************************************/
PUBLIC void MMIENG_StartPingTest(void);

/********************************************************************************
 NAME:			MMIENG_OpenIQModeWin
 DESCRIPTION:	
 PARAM IN:		
 PARAM OUT:		
 AUTHOR:		zhaohui
 DATE:		0	2009.03.03
********************************************************************************/
PUBLIC void MMIENG_OpenIQModeWin(void);

/********************************************************************************
 NAME:			MMIEng_SetResultWinText
 DESCRIPTION:	
 PARAM IN:		txt1 - 
				txt2 - 
				txt3 - 
 PARAM OUT:		None
 AUTHOR:		allen
 DATE:			2004.09.10
********************************************************************************/
PUBLIC void MMIEng_SetResultWinText(MMI_STRING_T txt1, MMI_STRING_T txt2, MMI_STRING_T txt3);

/********************************************************************************
 NAME:			MMIENGWIN_OpenAudioSetWin
 DESCRIPTION:	
 PARAM IN:		
 PARAM OUT:		None
 AUTHOR:		ying.xu
 DATE:			2010.06.21
********************************************************************************/
PUBLIC BOOLEAN MMIENGWIN_OpenAudioSetWin(uint32 menuid);
#endif//ENG_SUPPORT

/********************************************************************************
 NAME:			MMIENGWIN_SetMMITestBit
 DESCRIPTION:	
 PARAM IN:		
 PARAM OUT:		None
 AUTHOR:		ying.xu
 DATE:			2010.08.17
********************************************************************************/
PUBLIC uint32  MMIENGWIN_SetMMITestBit(uint32 result);

/********************************************************************************
 NAME:			MMIENGWIN_OpenNVCountingWin
 DESCRIPTION:	
 PARAM IN:		
 PARAM OUT:		
 AUTHOR:		ying.xu
 DATE:		0	2010.08.17
********************************************************************************/
PUBLIC BOOLEAN MMIENGWIN_OpenNVCountingWin(void);

//gmw---begin
/********************************************************************************
 NAME:			HandleConfigNVTestWinMsg
 DESCRIPTION:	
 PARAM IN:		
 PARAM OUT:		
 AUTHOR:		mingwen.ge
 DATE:		0	2015.05.12
********************************************************************************/
PUBLIC BOOLEAN MMIENGWIN_OpenConfigNVTestWin(void);

//gmw---end

/********************************************************************************
 NAME:			Standby time recording ralated function
 DESCRIPTION:	
 PARAM IN:		
 PARAM OUT:		
 AUTHOR:		ying.xu
********************************************************************************/
PUBLIC void MMIENG_EnableAndResetStanbyRecord(uint32 enable);
PUBLIC void MMIENG_RecordStandbyInit(void);
PUBLIC uint32 MMIENG_GetStanbyRecordEnableState(void);
PUBLIC void MMIENG_OpenStandbyInfoWin(void);

/********************************************************************************
 NAME:			MMIENG_OpenGetFlashInfoWin
 DESCRIPTION:	
 PARAM IN:		
 PARAM OUT:		
 AUTHOR:		ying.xu
********************************************************************************/
PUBLIC void MMIENG_OpenGetFlashInfoWin(void);

/********************************************************************************
 NAME:			MMIENG_OpenuUpdateIMEIWin
 DESCRIPTION:	
 PARAM IN:		
 PARAM OUT:		
 AUTHOR:		ying.xu
********************************************************************************/
PUBLIC void MMIENG_OpenuUpdateIMEIWin(void);

#ifdef MCARE_V31_SUPPORT
PUBLIC BOOLEAN MMIAPIENG_StartMcareTest(void);
#endif

/********************************************************************************
 NAME:			MMIAPIENG_get/Set usb log
 DESCRIPTION:	
 PARAM IN:		
 PARAM OUT:		
 AUTHOR:		fen.xie
 DATE:			2010.07.06
********************************************************************************/
PUBLIC int32 MMIENG_GetUSBLog(BOOLEAN *is_on);
PUBLIC int32 MMIENG_SetUSBLog(BOOLEAN is_on);

/********************************************************************************
 NAME:			MMIENG_OpenMonkeyTestSettingWin
 DESCRIPTION:	
 PARAM IN:		
 PARAM OUT:		
 AUTHOR:		
 DATE:			
********************************************************************************/
PUBLIC BOOLEAN MMIENG_OpenMonkeyTestSettingWin(void);
//PUBLIC int32 MMIENG_IsMuteWhenMonkeyTesting(void);

/********************************************************************************
 NAME:          EngBTopenSSPDebugWin   
 AUTHOR:        Leon.Wang
 DATE:          2012.11.14
********************************************************************************/
PUBLIC void EngBTopenSSPDebugWin(void);

/********************************************************************************
 NAME:          EngBTopenBQPModeWin   
 AUTHOR:        Leon.Wang
 DATE:          2012.12.4
********************************************************************************/
PUBLIC void EngBTopenBQBModeWin(void);


#ifdef __cplusplus
    }
#endif

#endif


