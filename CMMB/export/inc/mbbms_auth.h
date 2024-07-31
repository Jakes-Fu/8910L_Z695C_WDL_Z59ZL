/******************************************************************************
** File Name:      mbbms_auth.h                                            *
** Author:                                                                    *
** Date:           2008/9/24                                                  *
** Copyright:      2008 Spreatrum, Incoporated. All Rights Reserved.          *
** Description:    This file is used for MMI application                *
*******************************************************************************
**                         Important Edit History                             *
** -------------------------------------------------------------------------- *
** DATE           NAME             DESCRIPTION                                *
** 2008/9        Hong.Yu           Create                                     *
2009-7-20: PUBLIC BOOLEAN MBBMS_GBA_Exit(void) function is added to terminate the GBA thread.
2009-7-20: Add MBBMS_GBA_Ready(void) function to check if GBA is ready.
******************************************************************************/

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/


#ifndef __MBBMS_AUTH_H__
#define __MBBMS_AUTH_H__

#include "sci_types.h"
#include "sfs.h"
#include "os_api.h"
//#include "cmd_common.h"
//#include "sig_code.h"
//#include "user_tasks_id.h"
#include "tasks_id.h"
 
#ifdef __cplusplus
extern   "C"
{
#endif
#define   MBBMS_IMSI_LEN_MAX		16
#define   MBBMS_IMPI_LEN_MAX		128
#define   MAX_DOMAIN_NAME_LENGTH	60
#define   MBBMS_CMMB_SN_LEN		16
#define   MBBMS_MRK_KSEXTNAF_LEN	128
#define  MBBMS_GBA_MAX_DESC_LEN	256

typedef enum{
		MBBMS_IDLE = 0,
		MBBMS_CAS_CHECKING,
		MBBMS_GBA_WORKING,
		MBBMS_MRK_WORKING
}MBBMS_GBA_STATE_E;

typedef enum{
		GBA_RET_OK = 0,
		GBA_RET_NOT_INIT,
		GBA_RET_BUSY,
		GBA_RET_IDLE
}MBBMS_GBA_RETURN_E;

#if 0 //REmoved the temp def.  These signal code should be defined in the sig_code.h
// signal type definitions to MMI
typedef enum{
	MBBMS_MMI_CAS_RES,
	MBBMS_MMI_GBA_RES,
	MBBMS_MMI_GBA_STOP_RES,
	MBBMS_MMI_MRK_RES,
	MBBMS_MMI_MRK_STOP_RES
}MBBMS_GBA_SIGNAL_E;
#endif

typedef enum {
MBBMS_SUCCESS,
MBBMS_IMPI_SUCCESS,
MBBMS_FAIL,
MBBMS_GBA_SYNC_FAIL,
MBBMS_SIM_FAIL,
MBBMS_GBA_AUTH_FAIL,
MBBMS_GBA_IMPI_FAIL,
MBBMS_NETWORK_FAIL,
MBBMS_MTK_6A88,
MBBMS_MTK_6985,
MBBMS_MTK_9865,
MBBMS_MTK_9862
}MBBMS_GBA_RESULT_E;

typedef enum {
		MBBMS_TYPE_IMSI,
		MBBMS_TYPE_IMPI,
		MBBMS_TYPE_INVALID
  } MBBMS_SIM_TYPE_E;


typedef struct
{
      _SIGNAL_VARS
	MBBMS_GBA_RESULT_E rst;
     void *ptr;  //only valid to MTK mskid error.	
}MBBMS_GBA_SIG_T;

typedef struct mbbms_gba_data_tag
{
	char imsi[MBBMS_IMSI_LEN_MAX];
	char impi[MBBMS_IMPI_LEN_MAX];
	char description[MBBMS_GBA_MAX_DESC_LEN];
	uint16 description_len;
	char bsf_domain_name[MAX_DOMAIN_NAME_LENGTH];
}mbbms_gba_data_t;

typedef struct mbbms_mrk_input_tag{
	char naf_id[64];
	char impi[MBBMS_IMPI_LEN_MAX];
}mbbms_mrk_input_t;

typedef struct mbbms_mrk_output_tag{
	char ks_ext_naf[MBBMS_MRK_KSEXTNAF_LEN];
	uint8 len;
}mbbms_mrk_output_t;
#if 0
 typedef enum
{
  DSX_RTP_AU_CONSTANT_SIZE_LENGTH,
  DSX_RTP_AU_VARIABLE_SIZE_LENGTH,
  DSX_RTP_AU_ABSENT_SIZE_LENGTH
} TDsxRtpAuSizeType;
#endif

typedef struct
{
  //TDsxRtpAuSizeType	auSizeLengthType;
    /**< Access unit size length type */
  uint32	  auSizeLength;
    /**< Access unit size length */
  uint32	  indexLength;
    /**< Number of bits on which the AU-Index is encoded in the first
     *   AU-header
    */
  uint32	  indexDeltaLength;
    /**< Number of bits on which the AU-Index-delta field is encoded in any
         non-first AU-header. */
  uint32	  ctsDeltaLength;
    /**< Number of bits on which the CTS-delta field is encoded in 
         the AU-header. */
  uint32  	dtsDeltaLength;
    /**< Number of bits on which the DTS-delta field is encoded in 
         the AU-header. */
  uint32		      useRandomAccessFlag;  //TRUE or FALSE
    /**< If TRUE, the RAP-flag is present in the AU-header, if FALSE,
         the RAP-flag is absent. */
  uint32	  streamStateLength;
    /**< The number of bits on which the Stream-state field is encoded in the 
         AU-header. */
  uint32	  auxiliaryDataSizeLength;
    /**< The number of bits that is used to encode the 
         auxiliary-data-size field. */
} MBBMS_RtpInitialData;


typedef enum
{
  MBBMS_ISMACRYP_MODE_AES_CTR_128
   /**<  AES_CTR_128 mode */
} MBBMS_IsmacrypEncryptionMode;


typedef void (*MBMSMIDWARE_CALLBACK) (uint32 para0,uint32 para1, void *ptr);


/**
 *  @brief
 *    Initial data used for ISMACryp encryption
*/
typedef struct
{
  MBBMS_IsmacrypEncryptionMode  mode;
    /**< ISMACryp encryption mode */
  uint8               initializationVectorLength;
    /**< length in bytes of the initial initialization vector*/
  uint8               deltaInitializationVectorLength;
    /**< length in bytes of the delta initialization vector, 0 if unused */
  BOOLEAN                    useSelectiveEncryption;
    /**< TRUE if selective encryption is used, FALSE otherwise */
  uint8               keyIndicatorLength;
    /**< Length in bytes of the key indicators */
  BOOLEAN                    useKeyIndicatorPerAu;
    /**< TRUE if a Key indicator appears per Access Unit (AU) basis,
     * FALSE otherwise
    */
  uint8         salt[8];
    /**< salt value (MSBF) */
} MBBMS_IsmacrypInitialData;


PUBLIC BOOLEAN  MBBMS_GBA_Ready(void);
 
PUBLIC BOOLEAN MBBMS_GBA_Init(void); 
PUBLIC BOOLEAN MBBMS_GBA_Exit(void);
PUBLIC BOOLEAN MBBMS_Set_WAP(char *wap_addr, uint16 port);
PUBLIC BOOLEAN MBBMS_Get_WAP(char *wap_addr, uint16 *port);
PUBLIC BOOLEAN MBBMS_Set_CMMBSN(char *sn, uint8 len); 
PUBLIC BOOLEAN MBBMS_Get_CMMBSN(char *sn, uint8 *len);
PUBLIC BOOLEAN MBBMS_Set_NAF(char *naf_addr, uint16 port);
PUBLIC BOOLEAN MBBMS_Get_NAF(char *naf_addr, uint16 *port);
PUBLIC BOOLEAN MBBMS_Set_NAF_Domain_Name(char *naf_addr);
PUBLIC BOOLEAN MBBMS_Get_NAF_Domain_Name(char *naf_addr);
 
PUBLIC BOOLEAN MBBMS_GBA_Get_State(void);
PUBLIC BOOLEAN MBBMS_SIM_Support_CAS(void);
PUBLIC  MBBMS_GBA_RETURN_E MBBMS_GBA_Operation(void);
PUBLIC  MBBMS_GBA_RETURN_E MBBMS_GBA_Stop_Operation(void);
PUBLIC  MBBMS_GBA_RETURN_E MBBMS_MRK_Operation(void);
PUBLIC  MBBMS_GBA_RETURN_E MBBMS_MRK_Stop_Operation(void);

PUBLIC  BOOLEAN MBBMS_GBA_Get_Data(mbbms_gba_data_t *data_ptr);
PUBLIC  BOOLEAN MBBMS_MRK_Get_Data(mbbms_mrk_output_t *data_ptr);

PUBLIC BOOLEAN MBBMS_Service_Vid_RTP_ISMA_Data(MBBMS_RtpInitialData rtpini, 
                    MBBMS_IsmacrypInitialData ismaini);
PUBLIC BOOLEAN MBBMS_Service_Aud_RTP_ISMA_Data(MBBMS_RtpInitialData rtpini, 
                    MBBMS_IsmacrypInitialData ismaini);
PUBLIC uint16 mbbms_get_current_time(char *str);


PUBLIC void  MBBMS_GBASetCallback(MBMSMIDWARE_CALLBACK gba_callback );
PUBLIC void  MBBMS_GBACleanCallback(void);
PUBLIC void MBBMS_Set_NetID(uint32 netid);
PUBLIC void MBBMS_Get_NetID(uint32 *netid);
PUBLIC BOOLEAN MBBMS_Set_User_Agent(char *useragent, uint8 len);
PUBLIC BOOLEAN MBBMS_Get_User_Agent(char *useragent, uint8 *len);
PUBLIC void MBBMS_GBA_Free_Data(mbbms_gba_data_t *data_ptr);

#ifdef __cplusplus
}
#endif
#endif
 
 
