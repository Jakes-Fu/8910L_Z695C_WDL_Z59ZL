/*****************************************************************************
** File Name:      mmiim_t9_func.c                                           *
** Author:         haiwu.chen                                                *
** Date:           24/04/2012                                                *
** Copyright:      2007 Spreadtrum, Incorporated. All Rights Reserved.       *
** Description:    this file is used to adapter t9 function                  *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 24/04/2012     haiwu.chen         Create                                  *
******************************************************************************/
#ifdef IM_ENGINE_T9

#if defined(__cplusplus)
extern "C" {
#endif



/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
//#include "std_header.h"
#include "mmk_timer.h"
#include "t9.h"
#include "t9api.h"
#include "mmiim_t9_func.h"
#include "mmitheme_im.h"
#include "mmiim_base.h"
#include "mmiim_t9_base.h"
#include "mmi_text.h"
#include "mmi_data.h"
#include "mmiim.h"
#include "ctrlim_base.h"
/**--------------------------------------------------------------------------*
 **                         STATIC DEFINITION                                *
 **--------------------------------------------------------------------------*/
#ifdef T9CHINESE
//index storke: (1:横)、(2:竖)、(3:撇)、(4:捺)、(5:折)、(6:?)
LOCAL uint16 s_stroke_keys_values[] = {0, 0X4E00, 0X4E28, 0X4E3F, 0x4E36, 0x4E5B, 0XFF1F};
#endif

/**--------------------------------------------------------------------------*
 **                         EXTERNAL DEFINITION                              *
 **--------------------------------------------------------------------------*/
#ifdef MMI_IME_USER_DATABASE
LOCAL T9AWUdbInfo *s_gpAlphaShareUdbInfo = NULL;

//#ifdef IM_SIMP_CHINESE_SUPPORT
LOCAL T9CCAudbInfo *s_gpSimpCAUdbInfo = NULL;
//#endif

//#ifdef IM_TRAD_CHINESE_SUPPORT
LOCAL T9CCAudbInfo *s_gpTradCAUdbInfo = NULL;
//#endif
#endif


//language map of inputmethod client and engine
static IM_LANG_CLINET_ENGINE_CONVERT_T s_lang_client_engine_list[] =
{
    {GUIIM_LANG_CHS         ,        T9PIDChinese},
    {GUIIM_LANG_CHT         ,        T9PIDChinese},
    {GUIIM_LANG_ARABIC      ,        T9PIDArabic},
    {GUIIM_LANG_ASSAMESE    ,        T9PIDAssamese},
    {GUIIM_LANG_BENGALI     ,        T9PIDBengali},
    {GUIIM_LANG_BENGALI_BD  ,        T9PIDBengali},
    {GUIIM_LANG_GEORGIAN    ,        T9PIDGeorgian},
    {GUIIM_LANG_GUJARATI    ,        T9PIDGujarati},
    {GUIIM_LANG_HEBREW      ,        T9PIDHebrew},
    {GUIIM_LANG_HINDI       ,        T9PIDHindi},
    {GUIIM_LANG_KANNADA     ,        T9PIDKannada},
    {GUIIM_LANG_MALAYALAM   ,        T9PIDMalayalam},
    {GUIIM_LANG_MARATHI     ,        T9PIDMarathi},
    {GUIIM_LANG_ORIYA       ,        T9PIDOriya},
    {GUIIM_LANG_PERSIAN     ,        T9PIDFarsi},
    {GUIIM_LANG_PUNJABI     ,        T9PIDPunjabi},
    {GUIIM_LANG_TAGALOG     ,        T9PIDTagalog},
    {GUIIM_LANG_TAMIL       ,        T9PIDTamil},
    {GUIIM_LANG_TELUGU      ,        T9PIDTelugu},
    {GUIIM_LANG_THAI        ,        T9PIDThai},
    {GUIIM_LANG_URDU        ,        T9PIDUrdu},
    {GUIIM_LANG_AFRIKAANS   ,        T9PIDAfrikaans},
    {GUIIM_LANG_ALBANIAN    ,        T9PIDAlbanian},
    {GUIIM_LANG_ARMENIAN    ,        T9PIDArmenian},
    {GUIIM_LANG_AZERBAIJANI ,        T9PIDAzerbaijani},
    {GUIIM_LANG_BASQUE      ,        T9PIDBasque},
    {GUIIM_LANG_BOSNIAN ,            T9PIDBosnian},    
    {GUIIM_LANG_BULGARIAN   ,        T9PIDBulgarian},
    {GUIIM_LANG_CATALAN     ,        T9PIDCatalan},
    {GUIIM_LANG_CROATIAN    ,        T9PIDCroatian},
    {GUIIM_LANG_CZECH       ,        T9PIDCzech},
    {GUIIM_LANG_DANISH      ,        T9PIDDanish},
    {GUIIM_LANG_DUTCH       ,        T9PIDDutch},
    {GUIIM_LANG_ENGLISH     ,        T9PIDEnglish},
    {GUIIM_LANG_AMERICAN  ,          T9PIDEnglish},
    {GUIIM_LANG_ESTONIAN    ,        T9PIDEstonian},
    {GUIIM_LANG_FILIPINO    ,        T9PIDBulgarian},
    {GUIIM_LANG_FINNISH     ,        T9PIDFinnish},
    {GUIIM_LANG_FRENCH      ,        T9PIDFrench},
    {GUIIM_LANG_GALICIAN    ,        T9PIDBulgarian},
    {GUIIM_LANG_GERMAN      ,        T9PIDGerman},
    {GUIIM_LANG_GREEK       ,        T9PIDGreek},
    {GUIIM_LANG_HAUSA       ,        T9PIDHausa},
    {GUIIM_LANG_HUNGARIAN   ,        T9PIDHungarian},
    {GUIIM_LANG_ICELANDIC   ,        T9PIDIcelandic},
    {GUIIM_LANG_IGBO        ,        T9PIDIgbo},
    {GUIIM_LANG_INDONESIAN  ,        T9PIDIndonesian},
    {GUIIM_LANG_IRISH       ,        T9PIDIrish},
    {GUIIM_LANG_ITALIAN     ,        T9PIDItalian},
    {GUIIM_LANG_KAZAKH      ,        T9PIDKazakh},
    {GUIIM_LANG_LATVIAN     ,        T9PIDLatvian},
    {GUIIM_LANG_LITHUANIAN  ,        T9PIDLithuanian},
    {GUIIM_LANG_MACEDONIAN  ,        T9PIDMacedonian},
    {GUIIM_LANG_MALAY       ,        T9PIDMalay},
    {GUIIM_LANG_MOLDOVAN    ,        T9PIDMoldavian},
    {GUIIM_LANG_NORWEGIAN   ,        T9PIDNorwegian},
    {GUIIM_LANG_POLISH      ,        T9PIDPolish},
    {GUIIM_LANG_PORTUGUESE  ,        T9PIDPortuguese},
    {GUIIM_LANG_PORTUGUESE_BR  ,     T9PIDPortuguese},
    {GUIIM_LANG_ROMANIAN    ,        T9PIDRomanian},
    {GUIIM_LANG_RUSSIAN     ,        T9PIDRussian},
    {GUIIM_LANG_SERBIAN     ,        T9PIDSerbian},
    {GUIIM_LANG_SESOTHO     ,        T9PIDSesotho},
    {GUIIM_LANG_SLOVAK      ,        T9PIDSlovak},
    {GUIIM_LANG_SLOVENIAN   ,        T9PIDSlovenian},
    {GUIIM_LANG_SPANISH     ,        T9PIDSpanish},
    {GUIIM_LANG_SPANISH_AM  ,        T9PIDSpanish},
    {GUIIM_LANG_SWAHILI     ,        T9PIDSwahili},
    {GUIIM_LANG_SWEDISH     ,        T9PIDSwedish},
    {GUIIM_LANG_TURKISH     ,        T9PIDTurkish},
    {GUIIM_LANG_UKRAINIAN   ,        T9PIDUkrainian},
    {GUIIM_LANG_VIETNAMESE  ,        T9PIDVietnamese},
    {GUIIM_LANG_XHOSA       ,        T9PIDXhosa},
    {GUIIM_LANG_YORUBA      ,        T9PIDYoruba},
    {GUIIM_LANG_ZULU        ,        T9PIDZulu},
    {GUIIM_LANG_AMHARIC,             T9PIDAmharic},
    {GUIIM_LANG_SINHALESE,           T9PIDSinhala},
    {GUIIM_LANG_KHMER,               T9PIDKhmer},
    {GUIIM_LANG_PASHTO      ,        T9PIDPashto},
    {GUIIM_LANG_UZBEK       ,        T9PIDUzbek},
};

/**--------------------------------------------------------------------------*
 **                         GLOBAL DEFINITION                                *
 **--------------------------------------------------------------------------*/
#ifdef T9CHINESE
extern T9STATUS T9FARCALL T9CCLoadLdb(T9CCFieldInfo *pFieldInfo, T9ChineseData T9FARDATA *pT9CCLdbHeader);
#endif

extern T9U32 T9FARCALL T9ReadLdbData(T9FieldInfo *pFieldInfo, T9U32 dwOffset, T9FARPOINTER *ppbDst, T9U32 *pdwStart, T9U32 nSize, T9MINOR mCacheIndex);
extern BOOLEAN s_lang_status_info[MMISET_MAX_LANGUAGE + 1][2];


/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : Callback function which communicate with T9 core 
//  Global resource dependence : none
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
LOCAL T9STATUS T9FARCALL HandleRequestCallback(
    T9FieldInfo *pFieldInfo,
    T9Request *pRequest
);

#ifdef T9CHINESE
/*****************************************************************************/
//  Description : set chinese language database 
//  Global resource dependence : none
//  Author:  haiwu.chen
//  Note:
/*****************************************************************************/
LOCAL T9STATUS SetT9Language_Chs(MMIIM_T9_CHS_DATA_T *data_ptr);
#endif


/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DEFINE                            */
/*---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : Callback function which communicate with T9 core 
//  Global resource dependence : none
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
LOCAL T9STATUS T9FARCALL HandleRequestCallback(
    T9FieldInfo *pFieldInfo,
    T9Request *pRequest
)
{
    T9STATUS    mStatus = T9STATERROR;
	if (PNULL == pFieldInfo || PNULL == pRequest)
    {
        SCI_TRACE_LOW("[MMIIM] HandleRequestCallback: pFieldInfo or pRequest is null.");
        return mStatus;
    }
    SCI_TRACE_LOW("[MMIIM] pRequest->eType %d",pRequest->eType);
    switch(pRequest->eType) {

       case T9REQRELOADCAUDB:
            /* note: in the multi-thread environment, need to do some thread safe
            * here */
#ifdef T9CHINESE
#ifdef MMI_IME_USER_DATABASE
            if ((pFieldInfo->nLdbNum & T9PIDMASK) == T9PIDChinese) 
            {
//#if defined (IM_CHINESE_T_SUPPORT) || defined (IM_TRAD_CHINESE_SUPPORT)
                if ((pFieldInfo->nLdbNum & T9SIDMASK) == T9SIDChineseTraditional)
                {
					SCI_TRACE_LOW("[MMIIM] T9SIDMASK T9SIDChineseTraditional");
                    ((T9CCFieldInfo*) pFieldInfo)->pCAUdbInfo = s_gpTradCAUdbInfo;
                }
//#endif

//#if defined(IM_SIMP_CHINESE_SUPPORT) || defined(IM_CHINESE_S_SUPPORT)
                if ((pFieldInfo->nLdbNum & T9SIDMASK) == T9SIDChineseSimplified) 
                {
					SCI_TRACE_LOW("[MMIIM] T9SIDMASK T9SIDChineseSimplified");
                    ((T9CCFieldInfo*) pFieldInfo)->pCAUdbInfo = s_gpSimpCAUdbInfo;
                }
//#endif
            }
#endif
#endif
            
            mStatus = T9STATNONE;
            break;
#ifdef T9_7_4_4
    case T9REQSPELLSYM :    /* External multi-tap request (Bopomofo and Korean) */
            mStatus = GetNextSymOnKey(pFieldInfo, pRequest);
            break;  
#endif
    case T9REQTIMEOUT:
            if(PNULL != pRequest)/*lint !e774*/
            {
                //s_timer_id = pRequest->data.sTimeout.mTimerID;
                MMK_StopTimer(T9_EXPL_TIMER_ID);
                //MMK_StartTimer(T9_EXPL_TIMER_ID, T9_EXPL_TIMER_OUT, FALSE);
            }
            mStatus = T9STATNONE;
           break;
       default:
           mStatus = T9STATNONE;
    }
    return mStatus;
}



/*---------------------------------------------------------------------------*/
/*                          GLOBAL FUNCTION DEFINE                           */
/*---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : This function is used to init t9 by data_ptr
//  lobal resource dependence : none
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
PUBLIC T9STATUS MMIIM_T9_Init_Alpha(MMIIM_T9_DATA_T *data_ptr)
{
    T9STATUS sStatus = T9STATERROR;
    T9SYMB * text_buf_ptr = PNULL;
    uint16 text_buf_size = T9_MAX_TXT_BUF_SIZE * sizeof(T9SYMB);
    T9AuxType * aux_buf_ptr = PNULL;
    uint16 aux_buf_size = T9_MAX_TXT_BUF_SIZE * sizeof(T9AuxType);
    GUIIM_LANGUAGE_T lang = GUIIM_LANG_NONE;
    uint8 *ldb_buff_ptr = PNULL;
    uint32 ldb_buf_size = MMIIM_T9GetMaxLdbSize()* sizeof(uint8);

    if (PNULL == data_ptr)
    {
        SCI_TRACE_LOW("[MMIIM] MMIIM_T9_Init_Alpha : data_ptr is null.");
        return sStatus;
    }
    
	//Init language ldb date
	lang = data_ptr->init_param.method_param_ptr->im_set.lang;
    //MMIIM_T9_InitLDBInfo(lang);
	//if re entry didn't check it, than "data_ptr" initialized twice, caused memory leak.just add check
	if (data_ptr->aw_field_info.G.psTxtBuf)
	{
		SCI_FREE (data_ptr->aw_field_info.G.psTxtBuf);
	}

	if (data_ptr->aw_field_info.G.paAuxBuf)
	{
		SCI_FREE (data_ptr->aw_field_info.G.paAuxBuf);
	}
    if(data_ptr->t9_ldb_buff)
    {
        SCI_FREE(data_ptr->t9_ldb_buff);
        data_ptr->t9_ldb_buff = PNULL;
    }    
    // Allocate and initialize the Alphabetic field structure
    SCI_MEMSET(&data_ptr->aw_field_info, 0, sizeof(T9AWFieldInfo));

    //每个输入法应用都有一个独立的BUF数据
    text_buf_ptr = (T9SYMB*)SCI_ALLOC_APP(text_buf_size);
    SCI_MEMSET(text_buf_ptr, 0, text_buf_size);
    
    aux_buf_ptr = (T9AuxType*)SCI_ALLOC_APP(aux_buf_size);
    SCI_MEMSET(aux_buf_ptr, 0, aux_buf_size);

    ldb_buff_ptr = (uint8*)SCI_ALLOC_APP(ldb_buf_size);
    SCI_MEMSET(ldb_buff_ptr, 0, ldb_buf_size);
    
    data_ptr->aw_field_info.G.psTxtBuf = text_buf_ptr;
    data_ptr->aw_field_info.G.paAuxBuf = aux_buf_ptr;
    data_ptr->t9_ldb_buff = ldb_buff_ptr;

    MMIIM_T9_InitLDBInfo(data_ptr);
    // Initialize the T9 Alpha system
    sStatus = T9AWInitialize(&data_ptr->aw_field_info, 
                             data_ptr->aw_field_info.G.psTxtBuf, 
                             data_ptr->aw_field_info.G.paAuxBuf,
                             T9_MAX_TXT_BUF_SIZE,
                             0,
                             HandleRequestCallback,
                             T9ReadLdbData
                             );
    
    SCI_TRACE_LOW("[MMIIM] T9AWInitialize sStatus=%d", sStatus);

    // reset text buf
    if (sStatus == T9STATNONE) 
    {
        T9ResetTextBuf(&data_ptr->aw_field_info.G, T9_MAX_TXT_BUF_SIZE);
        sStatus = MMIIM_T9_SetT9Language(data_ptr);
    }
    
#ifdef MMI_IME_USER_DATABASE
    if (sStatus == T9STATNONE)
    {
        // Initialize the T9 Alpha DBs
        sStatus = T9AWUdbActivate(
                                  &data_ptr->aw_field_info,
                                  s_gpAlphaShareUdbInfo,
                                  T9_UDB_SIZE,
                                  T9_UDB_FENCE,
                                  data_ptr->aw_field_info.G.bSymbolClass
                                  );

        SCI_TRACE_LOW("[MMIIM] T9AWUdbActivate sStatus=%d", sStatus);
    }
#endif

#ifdef IM_WORD_COMPLETION_SUPPORT
    sStatus = T9AWSetLdbCompletion(&data_ptr->aw_field_info);
    SCI_TRACE_LOW("[MMIIM] T9AWSetLdbCompletion sStatus=%d", sStatus);

    #ifdef MMI_IME_USER_DATABASE
    sStatus = T9AWSetUdbCustWordCompletion(&data_ptr->aw_field_info);
    SCI_TRACE_LOW("[MMIIM] T9AWSetUdbCustWordCompletion sStatus=%d", sStatus);
    #endif
#else
    sStatus = T9AWClrLdbCompletion(&data_ptr->aw_field_info);
    SCI_TRACE_LOW("[MMIIM] T9AWClrLdbCompletion sStatus=%d", sStatus);

    #ifdef MMI_IME_USER_DATABASE
    sStatus = T9AWClrUdbCustWordCompletion(&data_ptr->aw_field_info);
    SCI_TRACE_LOW("[MMIIM] T9AWClrUdbCustWordCompletion sStatus=%d", sStatus);
    #endif
#endif

    //set active word left
    //sStatus = T9AWActivateWordLeft(&s_aw_field_info);

    if (sStatus == T9STATNONE)
    {
        //set pointers to functions requires by T9
        data_ptr->aw_field_info.G.T9HandleRequest = HandleRequestCallback;
        data_ptr->aw_field_info.G.T9ReadLdbData = T9ReadLdbData;
    }
    
    return sStatus;
}

/*****************************************************************************/
//  Description : This function is used to terminate t9 by data_ptr
//  lobal resource dependence : none
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
PUBLIC T9STATUS MMIIM_T9_Ter_Alpha(MMIIM_T9_DATA_T *data_ptr)
{
    T9STATUS sStatus = T9STATERROR;
    
    if (PNULL == data_ptr)
    {
        SCI_TRACE_LOW("[MMIIM] MMIIM_T9_Ter_Alpha : data_ptr is null.");
        return sStatus;
    }
    
    if (PNULL != data_ptr->aw_field_info.G.psTxtBuf)
    {
        SCI_FREE(data_ptr->aw_field_info.G.psTxtBuf);
        data_ptr->aw_field_info.G.psTxtBuf = PNULL;
    }
    
    if (PNULL != data_ptr->aw_field_info.G.paAuxBuf)
    {
        SCI_FREE(data_ptr->aw_field_info.G.paAuxBuf);
        data_ptr->aw_field_info.G.paAuxBuf = PNULL;
    }

    if(PNULL != data_ptr->t9_ldb_buff)
    {
        SCI_FREE(data_ptr->t9_ldb_buff);
        data_ptr->t9_ldb_buff = PNULL;
    }
    
    sStatus = T9STATNONE;
    
    return sStatus;
}

/*****************************************************************************/
//  Description : set input mode type, smart or multitap mode
//  Global resource dependence : none
//  Author: haiwu.chen
//  Note: 处理是否multitap or smart 模式
/*****************************************************************************/
PUBLIC T9STATUS T9FARCALL MMIIM_T9_SetMode(MMIIM_T9_DATA_T *data_ptr)
{
    T9STATUS sStatus = T9STATERROR;
    GUIIM_TYPE_T type = GUIIM_TYPE_NONE;
    
    if (PNULL == data_ptr)
    {
        SCI_TRACE_LOW("[MMIIM] MMIIM_T9_SetMode : data_ptr is null.");
        return sStatus;
    }

    type = data_ptr->init_param.method_param_ptr->im_set.type;
    
    if (type == GUIIM_TYPE_SMART || type == GUIIM_TYPE_ENGLISH)
    {
        sStatus = T9AWClrMultitapMode(&data_ptr->aw_field_info);
    }
    else if (type == GUIIM_TYPE_MULTITAP || type == GUIIM_TYPE_ABC)
    {
        sStatus = T9AWSetMultitapMode(&data_ptr->aw_field_info);
    }
    else
    {
        sStatus = T9STATNONE;
    }
    
    SCI_TRACE_LOW("[MMIIM] MMIIM_T9_SetMode, type=%d, sStatus=%d", type, sStatus);
    
    return sStatus;
}

/*****************************************************************************/
//  Description : set input capital type
//  Global resource dependence : none
//  Author: haiwu.chen
//  Note: 处理是否capital or not 模式
/*****************************************************************************/
PUBLIC T9STATUS T9FARCALL MMIIM_T9_SetCapital(MMIIM_T9_DATA_T *data_ptr)
{
    T9STATUS sStatus = T9STATERROR;
    GUIIM_CAPITAL_MODE_T capital = GUIIM_MODE_MAX;
    
    if (PNULL == data_ptr)
    {
        SCI_TRACE_LOW("[MMIIM] MMIIM_T9_SetMode : data_ptr is null.");
        return sStatus;
    }
    
    capital = data_ptr->init_param.method_param_ptr->im_set.capital;
    
    if (GUIIM_MODE_LEAD_UPPER == capital)
    {
        //sStatus = T9AWSetCapState(&data_ptr->aw_field_info);
        sStatus = T9AWClrCapState(&data_ptr->aw_field_info);
        sStatus = T9AWSetShfState(&data_ptr->aw_field_info);
    }
    else if (GUIIM_MODE_UPPER == capital)
    {
        sStatus = T9AWClrShfState(&data_ptr->aw_field_info);
        sStatus = T9AWSetCapState(&data_ptr->aw_field_info);
    }
    else if (GUIIM_MODE_LOWER == capital)
    {
        sStatus = T9AWClrShfState(&data_ptr->aw_field_info);
        sStatus = T9AWClrCapState(&data_ptr->aw_field_info);
    }
    else
    {
        sStatus = T9STATNONE;
    }
    
    SCI_TRACE_LOW("[MMIIM] MMIIM_T9_SetCapital cap=%d, sStatus=%d", capital, sStatus);
    
    return sStatus;
}

/*****************************************************************************/
//  Description : handle t9 reset text buf
//  Global resource dependence : none
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
PUBLIC T9STATUS T9FARCALL MMIIM_T9_ResetTextBuf(T9FieldInfo *pFieldInfo)
{
    T9STATUS sStatus = T9STATERROR;
    
    if (PNULL == pFieldInfo)
    {
        SCI_TRACE_LOW("[MMIIM] MMIIM_T9_ResetTextBuf: data_ptr is null.");
        return sStatus;
    }
    
    sStatus = T9ResetTextBuf(pFieldInfo, T9_MAX_TXT_BUF_SIZE);

    if (T9STATNONE != sStatus)
    {
        //not succeed, than print
        SCI_TRACE_LOW("[MMIIM] MMIIM_T9_ResetTextBuf: return status=%d", sStatus);
    }
    
    return sStatus;
}

/*****************************************************************************/
//  Description : handle t9 add text string
//  Global resource dependence : none
//  Author: haiwu.chen
//  Note: 
/*****************************************************************************/
PUBLIC T9STATUS T9FARCALL MMIIM_T9_AddTextString(
    T9FieldInfo *pFieldInfo,
    const T9SYMB *psBuf,
    T9UINT nLen
)
{
    T9STATUS sStatus = T9STATERROR;
    // T9SYMB sSymID = 0;
    
    if (PNULL == pFieldInfo || PNULL == psBuf || 0 == nLen)
    {
        SCI_TRACE_LOW("[MMIIM] MMIIM_T9_AddTextString: param is null.");
        return sStatus;
    }
    
    SCI_TRACE_LOW("[MMIIM] MMIIM_T9_AddTextString: nLen=%d.", nLen);

    sStatus = T9AddTextString(pFieldInfo, psBuf, nLen);
    
    SCI_TRACE_LOW("[MMIIM] MMIIM_T9_AddTextString: return status=%d", sStatus);
    
    return sStatus;
}

/*****************************************************************************/
//  Description : get t9 client language by engine language
//  Global resource dependence : none
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
PUBLIC uint16 MMIIM_T9_GetEngineLangByClientLang(GUIIM_LANGUAGE_T client_lang)
{
    uint16 i = 0;
    uint16 nRet = T9PIDNone;

    for (i = 0; i < ARR_SIZE(s_lang_client_engine_list); i++)
    {
        if (client_lang == s_lang_client_engine_list[i].client_lang)
        {
            nRet = s_lang_client_engine_list[i].engine_lang;
            break;
        }
    }

    return nRet;
}

/*****************************************************************************/
//  Description : set language database 
//  Global resource dependence : none
//  Author:  haiwu.chen
//  Note:
/*****************************************************************************/
PUBLIC T9STATUS MMIIM_T9_SetT9Language(MMIIM_T9_DATA_T *data_ptr)
{
    T9STATUS sStatus = T9STATERROR;
    T9UINT nLangNum = T9PIDNone;
    GUIIM_LANGUAGE_T lang = GUIIM_LANG_NONE;
    
    if (PNULL == data_ptr)
    {
        SCI_TRACE_LOW("[MMIIM] MMIIM_T9_SetT9Language : data_ptr is null.");
        return sStatus;
    }

    lang = data_ptr->init_param.method_param_ptr->im_set.lang;
    
    SCI_TRACE_LOW("[MMIIM] MMIIM_T9_SetT9Language: lang=%d", lang);
        
    nLangNum = MMIIM_T9_GetEngineLangByClientLang(lang);
    if (T9PIDNone != nLangNum)
    {
        sStatus = T9AWSetLanguage(&data_ptr->aw_field_info, 
            nLangNum, &(data_ptr->aw_field_info.G), 0);    
    }

    SCI_TRACE_LOW("[MMIIM] MMIIM_T9_SetT9Language sStatus=%d", sStatus);
    
    return sStatus;
}

/*****************************************************************************/
//  Description : is it this language chinese language
//  Global resource dependence : none
//  Author: haiwu.chen
//  Note: chinese contain: simple chinese and traditional chinese
/*****************************************************************************/
PUBLIC BOOLEAN MMIIM_T9_IsChinese(GUIIM_LANGUAGE_T lang)
{
    BOOLEAN is_ret = FALSE;
    
    if (GUIIM_LANG_CHS == lang 
        || GUIIM_LANG_CHT == lang
        ||GUIIM_LANG_TRAD_CHINESE_HK==lang)
    {
        is_ret = TRUE;
    }

    return is_ret;
}

/*****************************************************************************/
//  Description : is it this language alphabetic language
//  Global resource dependence : none
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIIM_T9_IsAlphabetic(GUIIM_LANGUAGE_T lang)
{
    BOOLEAN is_ret = FALSE;
    
    if (GUIIM_LANG_ARABIC <= lang && GUIIM_LANG_MAX > lang)
    {
        is_ret = TRUE;
    }

    return is_ret;
}

/*****************************************************************************/
//  Description : handle t9 key function
//  Global resource dependence : none
//  Author: haiwu.chen
//  Note: 处理按键消息
/*****************************************************************************/
PUBLIC T9STATUS T9FARCALL MMIIM_T9_HandleKey(T9FieldInfo *pFieldInfo, T9KEY eKeyID)
{
    T9STATUS sStatus = T9STATERROR;
    
    if (PNULL == pFieldInfo)
    {
        SCI_TRACE_LOW("[MMIIM] MMIIM_T9_HandleKey : data_ptr is null.");
        return sStatus;
    }
    
    SCI_TRACE_LOW("[MMIIM] MMIIM_T9_HandleKey: eKeyID=%d.", eKeyID);
    
    sStatus = T9HandleKey(pFieldInfo, eKeyID);
    
    if (T9STATNONE != sStatus)
    {
        //not succeed, than print
    }
    SCI_TRACE_LOW("[MMIIM] T9HandleKey: return status=%d", sStatus);
    
    return sStatus;
}

/*****************************************************************************/
//  Description : MMIIM_T9_AWActivateWordLeft
//  Global resource dependence : none
//  Author: haiwu.chen
//  Note: active a left-word when user delete chars
/*****************************************************************************/
PUBLIC T9STATUS T9FARCALL MMIIM_T9_AWActivateWordLeft(MMIIM_T9_DATA_T *data_ptr)
{
    T9STATUS sStatus = T9STATERROR;
    
    if (PNULL == data_ptr)
    {
        SCI_TRACE_LOW("[MMIIM] MMIIM_T9_AWActivateWordLeft : data_ptr is null.");
        return sStatus;
    }
    
    SCI_TRACE_LOW("[MMIIM] MMIIM_T9_AWActivateWordLeft.");
    
    sStatus = T9AWActivateWordLeft(&data_ptr->aw_field_info);
    
    if (T9STATNONE != sStatus)
    {
        //not succeed, than print
        SCI_TRACE_LOW("[MMIIM] MMIIM_T9_AWActivateWordLeft: return status=%d", sStatus);
    }
    
    return sStatus;
}

/*****************************************************************************/
//  Description : handle t9 add explicit char
//  Global resource dependence : none
//  Author: haiwu.chen
//  Note: key_index, MMIIM_KEY_1:0, MMIIM_KEY_2:1
//  input_sym用于26键直接输入的按键字符; 
/*****************************************************************************/
PUBLIC T9STATUS T9FARCALL MMIIM_T9_AddExplicitChar(
    T9FieldInfo *pFieldInfo,
    T9SYMB input_sym
)
{
    T9STATUS sStatus = T9STATERROR;
    
    if (PNULL == pFieldInfo)
    {
        SCI_TRACE_LOW("[MMIIM] MMIIM_T9_AddExplicitChar : pFieldInfo is null.");
        return sStatus;
    }
    
    SCI_TRACE_LOW("[MMIIM] MMIIM_T9_AddExplicitChar: pFieldInfo=%d, input_sym=%d.",
        pFieldInfo, input_sym);
    
    sStatus = T9AddExplicitChar(pFieldInfo, input_sym);
    
    return sStatus;
}

/*****************************************************************************/
//  Description : handle t9 reset text buf
//  Global resource dependence : none
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
PUBLIC T9STATUS T9FARCALL MMIIM_T9_Cursor(
    T9FieldInfo *pFieldInfo,
    T9CA eType, 
    T9UINT nCount
)
{
    T9STATUS sStatus = T9STATERROR;
    
    if (PNULL == pFieldInfo)
    {
        SCI_TRACE_LOW("[MMIIM] MMIIM_T9_Cursor : data_ptr is null.");
        return sStatus;
    }
    
    SCI_TRACE_LOW("[MMIIM] MMIIM_T9_Cursor:eType=%d, nCount=%d.", eType, nCount);

    sStatus = T9Cursor(pFieldInfo, eType, nCount);

    if (T9STATNONE != sStatus)
    {
        //not succeed, than print
        SCI_TRACE_LOW("[MMIIM] MMIIM_T9_Cursor: return status=%d", sStatus);
    }
    
    return sStatus;
}

/*****************************************************************************/
//  Description : handle t9 reset text buf
//  Global resource dependence : none
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
PUBLIC T9STATUS T9FARCALL MMIIM_T9_TimeOut(
    T9FieldInfo *pFieldInfo,
    T9MINOR mTimerID
)
{
    T9STATUS sStatus = T9STATERROR;
    
    if (PNULL == pFieldInfo)
    {
        SCI_TRACE_LOW("[MMIIM] MMIIM_T9_Cursor : data_ptr is null.");
        return sStatus;
    }
    
    sStatus = T9TimeOut(pFieldInfo, mTimerID);

    if (T9STATNONE != sStatus)
    {
        //not succeed, than print
        SCI_TRACE_LOW("[MMIIM] MMIIM_T9_TimeOut: return status=%d", sStatus);
    }
    
    return sStatus;
}

/*****************************************************************************/
//  Description : handle t9 add udb custom word
//  Global resource dependence : none
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
PUBLIC T9STATUS T9FARCALL MMIIM_T9_AddUdbCustomWord(
    MMIIM_T9_DATA_T *data_ptr,
    T9SYMB *psBuf, 
    T9MINOR mBufLen
)
{
    T9STATUS sStatus = T9STATERROR;
    
    if (PNULL == data_ptr || PNULL == psBuf || 0 == mBufLen)
    {
        SCI_TRACE_LOW("[MMIIM] MMIIM_T9_AddUdbCustomWord : data_ptr is null.");
        return sStatus;
    }

    sStatus = T9AWUdbAddCustWord(&data_ptr->aw_field_info, psBuf, mBufLen);

    if (T9STATNONE != sStatus)
    {
        //not succeed, than print
        SCI_TRACE_LOW("[MMIIM] MMIIM_T9_AddUdbCustomWord: return status=%d", sStatus);
    }
    
    return sStatus;
}

#ifdef MMI_IME_USER_DATABASE
/*****************************************************************************/
//  Description : load user database 
//  Global resource dependence : none
//  Author:haiwu.chen
//  Note: read from nv maybe just use one time when boot mobile
/*****************************************************************************/
PUBLIC BOOLEAN MMIIM_T9_LoadUserData(void)
{
    BOOLEAN is_ret = FALSE;
   	GUIIM_LANGUAGE_T input_lang = GUIIM_LANG_NONE;
	
	input_lang = MMIAPIIM_GetLanguage();

	if (PNULL == s_gpAlphaShareUdbInfo)
    {
        s_gpAlphaShareUdbInfo = (T9AWUdbInfo*)SCI_ALLOC_APP(T9_UDB_SIZE);
    }
    SCI_MEMSET(s_gpAlphaShareUdbInfo, 0x00, T9_UDB_SIZE);

    MMIAPIIM_GetAlphaUDBData((uint8 *)s_gpAlphaShareUdbInfo);
    is_ret = TRUE;

#ifdef IM_SIMP_CHINESE_SUPPORT
	if(GUIIM_LANG_CHS == input_lang)
	{
		if (PNULL == s_gpSimpCAUdbInfo)
	    {
	        s_gpSimpCAUdbInfo = (T9CCAudbInfo *)SCI_ALLOC_APP(T9_CAUDB_SIZE);
	        if (PNULL == s_gpSimpCAUdbInfo)
	        {
	            SCI_TRACE_LOW("[MMIIM] MMIIM_T9_LoadUserData: s_gpSimpCAUdbInfo not enough memory!");
	            is_ret = FALSE;
	        }
	    }
	    SCI_MEMSET(s_gpSimpCAUdbInfo, 0x00, T9_CAUDB_SIZE);
	    MMIAPIIM_GetChsUDBData((uint8 *)s_gpSimpCAUdbInfo);
	    is_ret = TRUE;
	}

#endif

#ifdef IM_TRAD_CHINESE_SUPPORT
	else if(GUIIM_LANG_CHT == input_lang|| GUIIM_LANG_TRAD_CHINESE_HK ==input_lang)
	{
		if (PNULL == s_gpTradCAUdbInfo)
	    {
	        s_gpTradCAUdbInfo = (T9CCAudbInfo *)SCI_ALLOC_APP(T9_CAUDB_SIZE);
	        if (PNULL == s_gpTradCAUdbInfo)
	        {
	            SCI_TRACE_LOW("[MMIIM] MMIIM_T9_LoadUserData: s_gpTradCAUdbInfo not enough memory!");
	            is_ret = FALSE;
	        }
	    }
	    SCI_MEMSET(s_gpTradCAUdbInfo, 0x00, T9_CAUDB_SIZE);
	    MMIAPIIM_GetChtUDBData((uint8 *)s_gpTradCAUdbInfo);
	    is_ret = TRUE;
	}
 
#endif

    return (is_ret);
}

/*****************************************************************************/
//  Description : save user database
//  Global resource dependence : none
//  Author:haiwu.chen
//  Note: save data from memory to nv when is power off
/*****************************************************************************/
PUBLIC BOOLEAN MMIIM_T9_SaveUserData(void)
{
    BOOLEAN is_ret = FALSE;
    
    MMIAPIIM_SetAlphaUDBData((uint8 *)s_gpAlphaShareUdbInfo);
    is_ret = TRUE;

#ifdef IM_SIMP_CHINESE_SUPPORT    
    MMIAPIIM_SetChsUDBData((uint8 *)s_gpSimpCAUdbInfo);
    is_ret = TRUE;
#endif

#ifdef IM_TRAD_CHINESE_SUPPORT    
    MMIAPIIM_SetChtUDBData((uint8 *)s_gpTradCAUdbInfo);
    is_ret = TRUE;
#endif

    return (is_ret);
}
#endif


#ifdef T9CHINESE

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DEFINE                            */
/*---------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description : set chinese language database 
//  Global resource dependence : none
//  Author:  haiwu.chen
//  Note:
/*****************************************************************************/
LOCAL T9STATUS SetT9Language_Chs(MMIIM_T9_CHS_DATA_T *data_ptr)
{
    T9STATUS sStatus = T9STATERROR;
    T9UINT nLangNum = T9PIDNone;
    GUIIM_LANGUAGE_T lang = GUIIM_LANG_NONE;
    T9CCAudbInfo * cc_udb_ptr = PNULL;
    
    if (PNULL == data_ptr
        || PNULL == data_ptr->init_param.method_param_ptr)
    {
        SCI_TRACE_LOW("[MMIIM] SetT9Language_Chs : data_ptr is null.");
        return sStatus;
    }

    //lang = MMIAPIIM_GetLanguage();//data_ptr->init_param.method_param_ptr->im_set.lang;
    lang = data_ptr->init_param.method_param_ptr->im_set.lang;
    SCI_TRACE_LOW("[MMIIM] SetT9Language_Chs: lang=%d", lang);

    if (GUIIM_LANG_CHS == lang) 
    {
        nLangNum = T9PIDChinese | T9SIDChineseSimplified;
#ifdef MMI_IME_USER_DATABASE
//#ifdef IM_SIMP_CHINESE_SUPPORT
        cc_udb_ptr = s_gpSimpCAUdbInfo;
//#endif
#endif        
    }
    else if (GUIIM_LANG_CHT == lang ||GUIIM_LANG_TRAD_CHINESE_HK ==lang) 
    {
        nLangNum = T9PIDChinese | T9SIDChineseTraditional;
#ifdef MMI_IME_USER_DATABASE
//#ifdef IM_TRAD_CHINESE_SUPPORT
        cc_udb_ptr = s_gpTradCAUdbInfo;
//#endif
#endif
    }
    if(PNULL == cc_udb_ptr)
    {
		SCI_TRACE_LOW("[MMIIM] SetT9Language_Chs ERROR cc_udb_ptr PNULL");
    }
	SCI_TRACE_LOW("[MMIIM] SetT9Language_Chs nLangNum=%d", nLangNum);
	
    sStatus = T9CCSetLanguage(&data_ptr->cc_field_info, 
        nLangNum, &(data_ptr->cc_field_info.G), cc_udb_ptr);

    SCI_TRACE_LOW("[MMIIM] SetT9Language_Chs sStatus=%d", sStatus);
    
    return sStatus;
}

/*---------------------------------------------------------------------------*/
/*                          GLOBAL FUNCTION DEFINE                           */
/*---------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description : This function is used to init t9 chinese by data_ptr
//  lobal resource dependence : none
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
PUBLIC T9STATUS MMIIM_T9_Init_Chs(MMIIM_T9_CHS_DATA_T *data_ptr)
{
    T9STATUS sStatus = T9STATERROR;
    T9SYMB * text_buf_ptr = PNULL;
    uint16 text_buf_size = T9_MAX_TXT_BUF_SIZE * sizeof(T9SYMB);
    T9AuxType * aux_buf_ptr = PNULL;
    uint16 aux_buf_size = T9_MAX_TXT_BUF_SIZE * sizeof(T9AuxType);
    GUIIM_LANGUAGE_T input_lang = GUIIM_LANG_NONE;
    uint8 *ldb_buff_ptr = PNULL;
    uint32 ldb_buf_size =MMIIM_T9GetMaxLdbSize() * sizeof(uint8);
	
    
    if (PNULL == data_ptr)
    {
        SCI_TRACE_LOW("[MMIIM] MMIIM_T9_Init_Chs : data_ptr is null.");
        return sStatus;
    }
    input_lang = data_ptr->init_param.method_param_ptr->im_set.lang;//MMIAPIIM_GetLanguage();
	
    SCI_TRACE_LOW("[MMIIM] MMIIM_T9_Init_Chs: text_buf_size=0x%x, aux_buf_size=0x%x, ldb_buf_size=0x%x", text_buf_size, aux_buf_size, ldb_buf_size);
	//if re entry didn't check it, than "data_ptr" initialized twice, caused memory leak.just add check
	if (data_ptr->cc_field_info.G.psTxtBuf)
	{
		SCI_FREE (data_ptr->aw_field_info.G.psTxtBuf);
        data_ptr->aw_field_info.G.psTxtBuf = PNULL;
	}

	if (data_ptr->cc_field_info.G.paAuxBuf)
	{
		SCI_FREE (data_ptr->aw_field_info.G.paAuxBuf);
        data_ptr->aw_field_info.G.paAuxBuf = PNULL;
	}

    if(data_ptr->t9_ldb_buff)
    {
        SCI_FREE(data_ptr->t9_ldb_buff);
        data_ptr->t9_ldb_buff = PNULL;
    }
    
    // Allocate and initialize the Chinese field structure
    SCI_MEMSET(&data_ptr->cc_field_info, 0, sizeof(T9CCFieldInfo));

    text_buf_ptr = (T9SYMB*)SCI_ALLOC_APP(text_buf_size);
    SCI_MEMSET(text_buf_ptr, 0, text_buf_size);
    
    aux_buf_ptr = (T9AuxType*)SCI_ALLOC_APP(aux_buf_size);
    SCI_MEMSET(aux_buf_ptr, 0, aux_buf_size);

    ldb_buff_ptr = (uint8*)SCI_ALLOC_APP(ldb_buf_size);
    SCI_MEMSET(ldb_buff_ptr, 0, ldb_buf_size);
    //Init language ldb date
 
    data_ptr->cc_field_info.G.psTxtBuf = text_buf_ptr;
    data_ptr->cc_field_info.G.paAuxBuf = aux_buf_ptr;
    data_ptr->t9_ldb_buff = ldb_buff_ptr;
    
    sStatus = MMIIM_T9_InitCHSLDBInfo(data_ptr);
    SCI_TRACE_LOW("[MMIIM] MMIIM_T9_Init_Chs: MMIIM_T9_InitCHSLDBInfo sStatus=%d", sStatus);

    // Initialize the T9 Chinese system */
    sStatus = T9CCInitialize(&data_ptr->cc_field_info,
                             data_ptr->cc_field_info.G.psTxtBuf,
                             data_ptr->cc_field_info.G.paAuxBuf,
                             T9_MAX_TXT_BUF_SIZE, 
                             0, // nBufLen
                             HandleRequestCallback, 
                             MMIIM_T9_CC_SELECTPAGE_NUM,
                             T9CCLoadLdb
                            );
    
    SCI_TRACE_LOW("[MMIIM] T9CCInitialize sStatus=%d", sStatus);

    // reset text buf
    if (sStatus == T9STATNONE) 
    {
        T9ResetTextBuf(&data_ptr->cc_field_info.G, T9_MAX_TXT_BUF_SIZE);
    }

#ifdef MMI_IME_USER_DATABASE
//#if defined(IM_SIMP_CHINESE_SUPPORT) || defined(IM_CHINESE_S_SUPPORT)
    // Initialize the T9 Chinese DBs
    if(GUIIM_LANG_CHS == input_lang)
    {
        if (sStatus == T9STATNONE) 
        {
            sStatus = T9CCAudbActivate(&data_ptr->cc_field_info, s_gpSimpCAUdbInfo, T9_CAUDB_SIZE, 2);
            SCI_TRACE_LOW("[MMIIM] T9CCAudbActivate simpchinese sStatus=%d", sStatus);
        }
    }

//#endif
//#if defined (IM_CHINESE_T_SUPPORT) || defined (IM_TRAD_CHINESE_SUPPORT)
    // Initialize the T9 Chinese DBs
    if(GUIIM_LANG_CHT == input_lang||GUIIM_LANG_TRAD_CHINESE_HK==input_lang)
    {
        if (sStatus == T9STATNONE) 
        {
            sStatus = T9CCAudbActivate(&data_ptr->cc_field_info, s_gpTradCAUdbInfo, T9_CAUDB_SIZE, 2);
            SCI_TRACE_LOW("[MMIIM] T9CCAudbActivate tradchinese sStatus=%d", sStatus);
        }
    }

//#endif

#endif

    // initialize the Chinese language
    if (sStatus == T9STATNONE)
    {
        sStatus = SetT9Language_Chs(data_ptr);
    }

#ifdef MMI_IME_USER_DATABASE
//#if defined(IM_SIMP_CHINESE_SUPPORT) || defined(IM_CHINESE_S_SUPPORT)
	if(GUIIM_LANG_CHS == input_lang)
	{
		if (sStatus == T9STATNONE) 
	    {
	        sStatus = T9CCAudbActivate(&data_ptr->cc_field_info, s_gpSimpCAUdbInfo, T9_CAUDB_SIZE, 2);
	        SCI_TRACE_LOW("[MMIIM] T9CCAudbActivate simpchinese againsStatus=%d", sStatus);
	    }
	}
//#endif
//#if defined (IM_CHINESE_T_SUPPORT) || defined (IM_TRAD_CHINESE_SUPPORT)
	if(GUIIM_LANG_CHT == input_lang||GUIIM_LANG_TRAD_CHINESE_HK ==input_lang)
	{
		if (sStatus == T9STATNONE) 
	    {
	        sStatus = T9CCAudbActivate(&data_ptr->cc_field_info, s_gpTradCAUdbInfo, T9_CAUDB_SIZE, 2);
	        SCI_TRACE_LOW("[MMIIM] T9CCAudbActivate tradchineses again Status=%d", sStatus);
	    }
	}

//#endif

#endif
    
    /* nDymCtxtFence can not be greater than half of nSelectpageMax,
     * if the user set it wrong, t9 core will detect it and keep
     * the right value */
    if (sStatus == T9STATNONE)
    {
        sStatus = T9CCSetDynCtxtFence(&data_ptr->cc_field_info, data_ptr->cc_field_info.nSelectPageMax / 2);
        SCI_TRACE_LOW("[MMIIM] T9CCSetDynCtxtFence sStatus=%d", sStatus);
    }
    
    /* nDymCtxtSearchDepth can not be greater than T9CCDYNCTXTSEARCHMAXDEPTH, if
     * user set it larger than it, t9 core will detect it and keep the
     * right one */
    if (sStatus == T9STATNONE)
    {
        sStatus = T9CCSetDynCtxtSearchDepth(&data_ptr->cc_field_info, T9_DYM_CTXT_SEARCH_DEPTH );
        SCI_TRACE_LOW("[MMIIM] T9CCSetDynCtxtSearchDepth sStatus=%d", sStatus);
    }

    if (sStatus == T9STATNONE)
    {
        //set pointers to functions requires by T9
        data_ptr->cc_field_info.G.T9HandleRequest = HandleRequestCallback;
        data_ptr->cc_field_info.G.T9ReadLdbData = T9ReadLdbData;
    }

    return sStatus;
}

/*****************************************************************************/
//  Description : This function is used to terminate t9 chinese by data_ptr
//  lobal resource dependence : none
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
PUBLIC T9STATUS MMIIM_T9_Ter_Chs(MMIIM_T9_CHS_DATA_T *data_ptr)
{
    T9STATUS sStatus = T9STATERROR;
    
    if (PNULL == data_ptr)
    {
        SCI_TRACE_LOW("[MMIIM] MMIIM_T9_Ter_Chs : data_ptr is null.");
        return sStatus;
    }
    
    if (PNULL != data_ptr->cc_field_info.G.psTxtBuf)
    {
        SCI_FREE(data_ptr->cc_field_info.G.psTxtBuf);
        data_ptr->cc_field_info.G.psTxtBuf = PNULL;
    }
    
    if (PNULL != data_ptr->cc_field_info.G.paAuxBuf)
    {
        SCI_FREE(data_ptr->cc_field_info.G.paAuxBuf);
        data_ptr->cc_field_info.G.paAuxBuf = PNULL;
    }

    if(PNULL != data_ptr->t9_ldb_buff)
    {
        SCI_FREE(data_ptr->t9_ldb_buff);
        data_ptr->t9_ldb_buff = PNULL;
    }
    sStatus = T9STATNONE;
    
    return sStatus;
}

/*****************************************************************************/
//  Description : set input mode type, smart or multitap mode
//  Global resource dependence : none
//  Author: haiwu.chen
//  Note: smart and stroke mode
/*****************************************************************************/
PUBLIC T9STATUS T9FARCALL MMIIM_T9_SetMode_Chs(MMIIM_T9_CHS_DATA_T *data_ptr)
{
    T9STATUS sStatus = T9STATERROR;
    GUIIM_TYPE_T type = GUIIM_TYPE_NONE;
    
    if (PNULL == data_ptr)
    {
        SCI_TRACE_LOW("[MMIIM] MMIIM_T9_SetMode_Chs : data_ptr is null.");
        return sStatus;
    }

    type = data_ptr->init_param.method_param_ptr->im_set.type;
    
    if (type == GUIIM_TYPE_SMART)
    {
        sStatus = T9CCSetAmbigSpellMode(&data_ptr->cc_field_info);
        SCI_TRACE_LOW("[MMIIM] T9CCSetAmbigSpellMode sStatus=%d", sStatus);
    }
    else if (type == GUIIM_TYPE_STROKE)
    {
        sStatus = T9CCSetStrokeMode(&data_ptr->cc_field_info);
        SCI_TRACE_LOW("[MMIIM] T9CCSetStrokeMode sStatus=%d", sStatus);
    }
    else
    {
        sStatus = T9STATNONE;
    }
    
    SCI_TRACE_LOW("[MMIIM] MMIIM_T9_SetMode_Chs, type=%d, sStatus=%d", type, sStatus);
    
    return sStatus;
}

/*****************************************************************************/
//  Description : handle t9 specify phonetic spelling
//  Global resource dependence : none
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
PUBLIC T9STATUS T9FARCALL MMIIM_T9_SpecifySpell(
    MMIIM_T9_CHS_DATA_T *data_ptr, 
    T9UINT nValue
)
{
    T9STATUS sStatus = T9STATERROR;
    
    if (PNULL == data_ptr || 0 == nValue)
    {
        SCI_TRACE_LOW("[MMIIM] HandleHorDirectKeyMsg: wrong param, nvalue=%d.", nValue);
        return sStatus;
    }
    
    sStatus = T9CCSpecifySpell(&data_ptr->cc_field_info, nValue);
    
    if (T9STATNONE != sStatus)
    {
        //not succeed, than print
        SCI_TRACE_LOW("[MMIIM] MMIIM_T9_SpecifySpell: return status=%d", sStatus);
    }
    
    return sStatus;
}

/*****************************************************************************/
//  Description : get chinese core data, and set to adapt bar rect
//  Global resource dependence : none
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
PUBLIC void MMIIM_T9_GetChsCoreData(MMIIM_T9_CHS_DATA_T *data_ptr)
{
    BOOLEAN has_data = FALSE;
    GUIIM_TYPE_T mode_type = GUIIM_TYPE_NONE;
    // T9STATUS sStatus = T9STATERROR;
    // T9UINT cnt = 0;
    // uint16 nWordIndex = 0;
    // uint16 word_cnt = 0;
    // uint32 tick1 = 0;
    // uint32 tick2 = 0;
    uint16 i = 0;
    uint16 ch = 0;
    
    if (PNULL == data_ptr)
    {
        SCI_TRACE_LOW("[MMIIM] MMIIM_T9_GetChsCoreData: param data_ptr is null!");
        return;
    }

    mode_type = data_ptr->init_param.method_param_ptr->im_set.type;

    //cand bar, common in pinyin and storke
    SCI_MEMSET(data_ptr->bar_cand.text, 0x00, MMIIM_TEXT_BAR_CHAR_MAX*sizeof(wchar));
    for (i = 0; i < data_ptr->cc_field_info.nSelectPageLen && i < MMIIM_T9_CC_SELECTPAGE_NUM; i++)
    {
        data_ptr->bar_cand.text[2*i] = data_ptr->cc_field_info.pwSelectPage[i];
        data_ptr->bar_cand.text[2*i+1] = ' ';
    }
    data_ptr->bar_cand.text_len = 2*data_ptr->cc_field_info.nSelectPageLen;
    data_ptr->bar_cand.page_index = data_ptr->cc_field_info.nSelectPageNum;

	if (data_ptr->bar_cand.hot_index >= data_ptr->cc_field_info.nSelectPageLen)
	{
		data_ptr->bar_cand.hot_index = data_ptr->cc_field_info.nSelectPageLen - 1;
	}
	data_ptr->bar_edit.hot_index = 0;
    MMIIM_TextBarGenSeg(&data_ptr->bar_cand, ' ', MMIIM_BAR_CAND);

    //edit bar, pinyin use spell; stroke use pbKeyBuf    
    if (mode_type == GUIIM_TYPE_SMART)
    {
        has_data = FALSE;
        
        SCI_MEMSET(data_ptr->bar_edit.text, 0x00, MMIIM_TEXT_BAR_CHAR_MAX*sizeof(wchar));
        if (0 < data_ptr->cc_field_info.nKeyBufLen)
        {
            ch = data_ptr->cc_field_info.pbSpellBuf[0];
            for (i = 0; i < MMIIM_TEXT_BAR_CHAR_MAX && ch != 0; i++)
            {
                data_ptr->bar_edit.text[i] = data_ptr->cc_field_info.pbSpellBuf[i];
                ch = data_ptr->cc_field_info.pbSpellBuf[i+1];
            }
            if ((i > 0) && (i < MMIIM_TEXT_BAR_CHAR_MAX))
            {
                data_ptr->bar_edit.text[i] = ' ';
                data_ptr->bar_edit.text_len = (uint8)i+1;
                data_ptr->bar_edit.page_index = 0;
                data_ptr->bar_edit.hot_index = data_ptr->cc_field_info.bSpellCode - 1;

                MMIIM_TextBarGenSeg(&data_ptr->bar_edit, ' ', MMIIM_BAR_EDIT);

                has_data = TRUE;
            }
        }

        if (!has_data)
        {
            data_ptr->bar_edit.text_len = 0;
            data_ptr->bar_edit.page_index = 0;
            data_ptr->bar_edit.seg_cnt = 0;
            data_ptr->bar_edit.hot_index = 0;
        }
    }
    else if (mode_type == GUIIM_TYPE_STROKE)
    {
        has_data = FALSE;
        
        //spell bar，注意只copy 64而非128，因为64足够，T9 是u8结构的，而我们是u16
        SCI_MEMSET(data_ptr->bar_edit.text, 0x00, MMIIM_TEXT_BAR_CHAR_MAX*sizeof(wchar));
        if (0 < data_ptr->cc_field_info.nKeyBufLen)
        {
            uint8 stroke_idx = 0;
            uint8 stroke_cnt = 0;
            for (i = 0; i < data_ptr->cc_field_info.nKeyBufLen; i++)
            {
                stroke_idx = data_ptr->cc_field_info.pbKeyBuf[i];
                if (stroke_idx > 6 || stroke_idx < 1)
                {
                    continue;
                }

                data_ptr->bar_edit.text[stroke_cnt++] = s_stroke_keys_values[stroke_idx];
            }
            if (stroke_cnt > 0)
            {
                data_ptr->bar_edit.text[stroke_cnt] = ' ';
                data_ptr->bar_edit.text_len = stroke_cnt + 1;
                data_ptr->bar_edit.page_index = 0;
                data_ptr->bar_edit.hot_index = 0;

                MMIIM_TextBarGenSeg(&data_ptr->bar_edit, ' ', MMIIM_BAR_EDIT);

                has_data = TRUE;
            }
        }

        if (!has_data)
        {
            data_ptr->bar_edit.text_len = 0;
            data_ptr->bar_edit.page_index = 0;
            data_ptr->bar_edit.seg_cnt = 0;
            data_ptr->bar_edit.hot_index = 0;
        }
    }
    
    return;
}



#endif

#endif //IM_ENGINE_T9
