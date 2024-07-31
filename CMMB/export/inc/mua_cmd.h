/******************************************************************************
 ** File Name:      mua_cmd.h                                                 *
 ** Author:         Zhang Hanbing                                             *
 ** DATE:           06/12/2009                                                *
 ** Copyright:      2009 Spreadtrum, Incorporated. All Rights Reserved.       *
 ** Description:    mbbms user authenticate command handle                    *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 06/12/2009     Zhang Hanbing        Create                                *
 ******************************************************************************/

  
#ifndef _MUA_CMD_H
#define _MUA_CMD_H

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "sci_types.h"
#include "sdua_drv.h"

#ifdef __cplusplus
extern   "C"
{
#endif

/**---------------------------------------------------------------------------*
 **                         MACRO Definations                                 *
 **---------------------------------------------------------------------------*/
#define MUACMD_RESP_RES_LEN (16*2+1)
#define MUACMD_RESP_CNONCE_LEN (16*2+1)
#define MUACMD_RESP_STATUS_LEN (1*2+1)

/**---------------------------------------------------------------------------*
 **                         Data Structures                                   *
 **---------------------------------------------------------------------------*/
 typedef enum _mua_card_type_tag
{
    MUA_CARD_SD,
    MUA_CARD_SIP,
    MUA_CARD_SMD = MUA_CARD_SIP,
    MUA_CARD_INVALID
}MUA_CARD_TYPE;

//参见移动多媒体广播手机电视业务SD卡技术规范090601pm.doc 7.2.1 
typedef enum _mua_ret_e_tag
{
	MUA_OK = 1,                      //success
	MUA_ERR,                         //指令执行错误
	MUA_ERR_MAC,                     //鉴权错误，不正确的mac
	MUA_ERR_NOT_SUPPORT_GSM_EV,      //鉴权错误，不支持gsm安全语境
	MUA_ERR_EXPIRED,                 //鉴权错误，密钥过期错误
	MUA_ERR_NOT_FOUND_KS,            //鉴权错误，没有找到ks
	MUA_ERR_NOT_FOUND_KEY,           //鉴权错误，密钥没有找到
	MUA_ERR_NOT_FOUND_BROADCAST_KEY  //鉴权错误，广电密钥没有找到
}MUA_RET_E;
typedef enum _mua_airnetwork_mode_tag
{
    MUA_AIRNETWORK_UNKNOWN = 0,
    MUA_AIRNETWORK_2G = 2,
    MUA_AIRNETWORK_3G = 3 
}MUA_AIRNETWORK_MODE;

typedef enum mua_label_tag
{
    LABEL_GBABS_MBMS_OPT_OK = 0xDB,
    LABEL_GBABS_MODE        = 0xDD,
    LABEL_GBANAF_MODE       = 0xDE
}MUA_LABEL_E;

//mbms security environment operation
typedef enum mua_mbms_security_env_type_tag
{
    MBMS_MSK_UPDATA =0,
    MBMS_MTK_CREATE
}MUA_MBMS_SECURITY_ENV_TYPE_E;

typedef enum mua_cmd_fram_tag
{
    FTAG_RESET = 0x12,
    FTAG_GENERAL = 0x14
}MUA_CMD_FRAME_TAG_E;

//the byte in the command header in the APDU
typedef enum mua_apdu_byte_tag
{
    APDU_CLA,
    APDU_INS,
    APDU_P1,
    APDU_P2,
    APDU_LC,
    APDU_CHEADER_NUM    //the byte number of the APDU command header
}MUA_APDU_BYTE_E;

//the instruction of MUA APDU format 
typedef struct mua_apdu_tag
{
    uint8  code[APDU_CHEADER_NUM]; //Header of the MUA APDU
    uint8* data;                   //Pointer to the data field of the MUA APDU
    uint8  le;
}MUA_APDU_T;

//Generic Bootstrapping Architecture  bootstrapping 3G data
typedef struct mua_gbabs_3g_data_tag
{
    uint8 label; //MUA_LABEL_E
    uint8 res_len;
    uint8* res;
    uint8 ck_len;
    uint8* ck;
    uint8 ik_len;
    uint8* ik;
    uint8 rand_len;
    uint8* rand_data;
}MUA_GBABS_3G_DATA_T;

//Generic Bootstrapping Architecture  bootstrapping 2G data
typedef struct mua_gbabs_2g_data_tag
{
    uint8 label; //MUA_LABEL_E
    uint8 res_len;
    uint8* res;
    uint8 kc_len;
    uint8* kc;
    uint8 rand_len;
    uint8* rand_data;
    uint8 ks_input_len;
    uint8* ks_input;
}MUA_GBABS_2G_DATA_T;

//Generic Bootstrapping Architecture  NAF mode
typedef struct mua_gbabs_naf_data_tag
{
    uint8 label; //MUA_LABEL_E
    uint8 naf_id_len;
    uint8* naf_id;
    uint8 impi_len;
    uint8* impi;
}MUA_GBABS_NAF_DATA_T;


//mbms security environment data
typedef struct mua_mbms_security_env_data_tag
{
    uint8 security_env; //MUA_MBMS_SECURITY_ENV_TYPE_E
    uint8 mikey_len;
    uint8* mikey;
}MUA_MBMS_SECURITY_ENV_DATA_T;

//B-TID and ks data
typedef struct mua_btid_data_tag
{
    uint8* btid;
    uint32 btid_len;
    uint8* ks;
    uint32 ks_len;
}MUA_BTID_DATA_T;

//response data
typedef struct mua_resp_data_tag
{
    uint8  data_length;
    uint8* data;
}MUA_RESP_DATA_T;

//response 3g data
typedef struct mua_resp_2g3g_tag
{
    uint8 status_len;
    uint8 status[MUACMD_RESP_STATUS_LEN];
    uint8 res_len;
    uint8 res[MUACMD_RESP_RES_LEN];
    uint8 cnonce_len;
    uint8 cnonce[MUACMD_RESP_CNONCE_LEN];
}MUA_RESP_2G3G_T;


typedef struct mua_comm_operation_tag
{
    BOOLEAN (*Open)(BOOLEAN is_cmmb_or_mbbms, BOOLEAN is_mbbms_bb_3g);
    SDUA_OPT_RESULT_E (*IoControl)(
                                     uint8 *i_pBuf, 
                                     uint32 i_nSize,
                                     uint8 i_frameTag,
                                     uint8* o_pBuf,
                                     uint16 *o_nSize,
                                     uint16 *o_sw 
                                     );
    BOOLEAN (*Close)(void);
}MUA_COMM_OPERATION_T;

/**---------------------------------------------------------------------------*
 **                         Global Variables                                  *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         Constant Variables                                *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         Function Prototypes                               *
 **---------------------------------------------------------------------------*/
/*****************************************************************************/
//! \fn BOOLEAN MUACMD_Open(MUA_CARD_TYPE ca_type)
//! \param ca_type
//! \return BOOLEAN TRUE ok
//! \brief  Description: This Function is used to init sd user auth 
//! \author Author: Zhang Hanbing
//! \note   Note: 
/*****************************************************************************/
BOOLEAN MUACMD_Open(MUA_CARD_TYPE ca_type);

 /*****************************************************************************/
//! \fn BOOLEAN MUACMD_Init(MUA_CARD_TYPE ca_type)
//! \param ca_type
//! \return BOOLEAN TRUE ok
//! \brief  Description: Init for power on sd ca card
//! \author Author: Zhang Hanbing
//! \note   Note: call by sdua_config.c file
/*****************************************************************************/
BOOLEAN MUACMD_Init(MUA_CARD_TYPE ca_type);

/*****************************************************************************/
//! \fn BOOLEAN MUACMD_Close(uint8 ca_type)
//! \param void.
//! \return BOOLEAN TRUE ok
//! \brief  Description: This Function is used to close sd user auth 
//! \author Author: Zhang Hanbing
//! \note   Note: 
/*****************************************************************************/
BOOLEAN MUACMD_Close(uint8 ca_type);

/*****************************************************************************/
//! \fn MUA_RET_E MUACMD_GBABS_3G(MUA_GBABS_3G_DATA_T *gbabs3g_data,  MUA_RESP_2G3G_T* resp_pack_data)
//! \param gbabs3g_data gbabs3g cmd data; resp_pack_data response data
//! \return MUA_RET_E
//! \brief  Description: send bootstrapping 3G cmd and return response data
//! \author Author: Zhang hanbing
//! \note   Note:
/*****************************************************************************/
MUA_RET_E MUACMD_GBABS_3G(MUA_GBABS_3G_DATA_T *gbabs3g_data,  MUA_RESP_2G3G_T* resp_pack_data);

/*****************************************************************************/
//! \fn MUA_RET_E MUACMD_GBABS_2G(MUA_GBABS_2G_DATA_T *gbabs2g_data,  MUA_RESP_2G3G_T* resp_pack_data )
//! \param gbabs2g_data gbabs2g cmd data; resp_pack_data response data
//! \return MUA_RET_E
//! \brief  Description: send bootstrapping 2G cmd and return response data
//! \author Author: Zhang hanbing
//! \note   Note:
/*****************************************************************************/
MUA_RET_E MUACMD_GBABS_2G(MUA_GBABS_2G_DATA_T *gbabs2g_data,  MUA_RESP_2G3G_T* resp_pack_data );

/*****************************************************************************/
//! \fn MUA_RET_E MUACMD_GBA_NAF(MUA_GBABS_NAF_DATA_T *naf_data,MUA_RESP_DATA_T* resp_pack_data)
//! \param naf_data naf_data; resp_pack_data response data
//! \return MUA_RET_E
//! \brief  Description: send bootstrapping naf mode cmd and return response
//! \author Author: Zhang hanbing
//! \note   Note:
/*****************************************************************************/
MUA_RET_E MUACMD_GBA_NAF(MUA_GBABS_NAF_DATA_T *naf_data,MUA_RESP_DATA_T* resp_pack_data);

/*****************************************************************************/
//! \fn MUA_RET_E MUACMD_MBMS(MUA_MBMS_SECURITY_ENV_DATA_T *sec_env_data,MUA_RESP_DATA_T* resp_pack_data)
//! \param sec_env_data security env data;resp_pack_data response data
//! \return MUA_RET_E
//! \brief  Description: send mbms cmd and return response data
//! \author Author: Zhang hanbing
//! \note   Note:
/*****************************************************************************/
MUA_RET_E MUACMD_MBMS(MUA_MBMS_SECURITY_ENV_DATA_T *sec_env_data,MUA_RESP_DATA_T* resp_pack_data);

/*****************************************************************************/
//! \fn BOOLEAN MUACMD_UPDATEBTID(MUA_BTID_DATA_T *btid_data)
//! \param btid_data B-TID,KS data
//! \return BOOLEAN TRUE is ok
//! \brief  Description: write b-tid and ks data and return result
//! \author Author: Zhang hanbing
//! \note   Note:
/*****************************************************************************/
BOOLEAN MUACMD_UPDATEBTID(MUA_BTID_DATA_T *btid_data);

/*****************************************************************************/
//! \fn BOOLEAN MUACMD_GetBTID(MUA_RESP_DATA_T* resp_pack_data )
//! \param resp_pack_data
//! \return BOOLEAN TRUE is ok
//! \brief  Description: get btid data
//! \author Author: Zhang hanbing
//! \note   Note: response data is the file content
/*****************************************************************************/
BOOLEAN MUACMD_GetBTID(MUA_RESP_DATA_T* resp_pack_data );

/*****************************************************************************/
//! \fn BOOLEAN MUACMD_MBBMS_IsEnabled(void)
//! \param 
//! \return BOOLEAN TRUE is ok
//! \brief  Description: is mbbms enable
//! \author Author: Zhang hanbing
//! \note   Note:
/*****************************************************************************/
BOOLEAN MUACMD_MBBMS_IsEnabled(void);

/*****************************************************************************/
//! \fn BOOLEAN MUACMD_MBBMS_GetCMMBSN(MUA_RESP_DATA_T* resp_pack_data)
//! \param resp_pack_data
//! \return BOOLEAN TRUE is ok
//! \brief  Description: get cmmbsn data
//! \author Author: Zhang hanbing
//! \note   Note:
/*****************************************************************************/
BOOLEAN MUACMD_MBBMS_GetCMMBSN(MUA_RESP_DATA_T* resp_pack_data);

/*****************************************************************************/
//! \fn BOOLEAN MUACMD_IsGBANeeded(void)
//! \param 
//! \return BOOLEAN TRUE is ok
//! \brief  Description: is need gba
//! \author Author: Zhang hanbing
//! \note   Note:
/*****************************************************************************/
BOOLEAN MUACMD_IsGBANeeded(void);

MUA_RET_E ParseCardSW(uint16 nStatus); //used for parse inno status

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/    
#ifdef __cplusplus
}
#endif

#endif  
