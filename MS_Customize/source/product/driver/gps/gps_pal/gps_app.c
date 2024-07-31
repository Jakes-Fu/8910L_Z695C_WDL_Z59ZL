
/**
 * @addtogroup demo_app
 * @{
 */

/***************************************************************************
 *                                                                         *
 *                   SiRF Technology, Inc. GPS Software                    *
 *                                                                         *
 *    Copyright (c) 2010 by SiRF Technology, Inc. All rights reserved.*
 *                                                                         *
 *    This Software is protected by United States copyright laws and       *
 *    international treaties.  You may not reverse engineer, decompile     *
 *    or disassemble this Software.                                        *
 *                                                                         *
 *    WARNING:                                                             *
 *    This Software contains SiRF Technology Inc.s confidential and        *
 *    proprietary information. UNAUTHORIZED COPYING, USE, DISTRIBUTION,    *
 *    PUBLICATION, TRANSFER, SALE, RENTAL OR DISCLOSURE IS PROHIBITED      *
 *    AND MAY RESULT IN SERIOUS LEGAL CONSEQUENCES.  Do not copy this      *
 *    Software without SiRF Technology, Inc.s  express written             *
 *    permission.   Use of any portion of the contents of this Software    *
 *    is subject to and restricted by your signed written agreement with   *
 *    SiRF Technology, Inc.                                                *
 *                                                                         *
 **************************************************************************/
/* This define is used to enable and disable the AGPS implementation */
//#define KST_AGPS

/**
 * @file   csr_sirf_spreadtrum_demo_app.c
 *
 * @brief  Demo app to run on Spreadtrum platform
 */

#include "ms_customize_trc.h"
#include "LSM_APIs.h"
#include "sirfnav_ui_ctrl.h"
#include "sirf_heap_sif.h"
#include "gps_app.h"
#include "csr_util.h"
#include "csr_types.h"
#include "sirf_pal.h"

#include "os_api.h"
#include "priority_system.h"
#ifdef KST_AGPS
#include "mn_api.h"
#include "mn_type.h"
#endif

#define ENABLE_NMEA_LISTENER           1
#define ENABLE_DEBUG_LISTENER          0

#define LPL_HEAP (128*1024)  /* 128KB */
#define DEFAULT_HOR_ERROR              1000
#define DEFAULT_VER_ERROR              3000
#define MAX_PORT_STRING_LENGTH (64)

tSIRF_SEMAPHORE               semPosition = NULL; /* semaphore to wait for position */
static CsrUint32              PositionCount;
static LSM_commonCfgData      LSM_CommonCfg;
static LSM_SIsessionCfgData   LSM_SessionCfg;
static CsrUint32              sessionID;
tPAL_CONFIG                   g_userPalConfig;

static tSIRF_RESULT gpioInitialization(tSIRF_VOID);
static tSIRF_RESULT gpioDeInitialization(tSIRF_VOID);

typedef struct _AGPSConfiguration
{
   /* System Setting */
   tSIRF_CHAR        GPSPort[MAX_PORT_STRING_LENGTH];
   tSIRF_CHAR        ResetPort[MAX_PORT_STRING_LENGTH];
   tSIRF_CHAR        AUXPort[MAX_PORT_STRING_LENGTH];
   tSIRF_CHAR        GPRSModemPort[MAX_PORT_STRING_LENGTH];
   tSIRF_CHAR        Apn[50];
   tSIRF_CHAR        Login[50];
   tSIRF_CHAR        Password[50];
   tSIRF_CHAR        Bearer[50];
   tSIRF_CHAR        Modem[50];
   tSIRF_CHAR        PhoneNumber[50];
   
   /* LSM Common settings */
   tSIRF_UINT16      numOfSessions;
   tSIRF_UINT16      serverPort;  
   tSIRF_CHAR        serverAddress[256];
   tSIRF_UINT8       isSecure;
   
   /* LSM Session settings */
   tSIRF_UINT8       resetType;
   
   tSIRF_UINT8       aidingType;
   tSIRF_BOOL        isPosTechMSB ;
   tSIRF_BOOL        isPosTechMSA;
   tSIRF_BOOL        isPosTechAuto;
   tSIRF_BOOL        isPosTechECID;
   tSIRF_UINT8       prefLocMethod; 
   
   tSIRF_UINT8       numberOfFixes;
   tSIRF_UINT8       timeBetweenFixes;

   tSIRF_UINT8       AssistProtocol;
   tSIRF_BOOL        isApproximateLocationPresent; 
   tSIRF_UINT16      approximateAltitude; 
   tSIRF_DOUBLE      approximateLongitude;  
   tSIRF_DOUBLE      approximateLatitude; 

   tSIRF_BOOL        QoSinSUPLSTART;
   tSIRF_UINT8       maxLocAge; 
   tSIRF_UINT8       QoSMaxResponseTime;
   tSIRF_UINT8       QoSVerticalAccuracy; 
   tSIRF_UINT8       QoSHorizontalAccuracy;
    
   tSIRF_UINT8       identification;  

   tSIRF_BOOL        isAPM;
   tSIRF_UINT8       powerControl;
   tSIRF_UINT8       triggers; 

   tSIRF_UINT8       loggingType;
   tSIRF_CHAR        briefLogFileName[256];
   tSIRF_CHAR        agpsLogFileName[256];
   tSIRF_CHAR        detailedLogFileName[256];
   tSIRF_CHAR        prevLocFileName[256];
   tSIRF_CHAR        slcLogFileName[256];

   tSIRF_BOOL        isCapNavBitAiding;
   tSIRF_BOOL        isCapCGEE;
   tSIRF_BOOL        isCapSGEE;
   tSIRF_BOOL        eeFileUpdate;
   
   tSIRF_BOOL        networkStatus;
   
   /* SGEE server specific parameter*/
   tSIRF_CHAR      SGEEserverAddress[2][80];
   tSIRF_UINT16    SGEEserverPort;
   tSIRF_CHAR      SGEEserverFile[256];
   tSIRF_CHAR      SGEEserverAuth[256];
}   AGPSConfig;


static AGPSConfig *getCfgPointer(tSIRF_VOID)
{
   static AGPSConfig agpsConfig = {{0}};
   return(&agpsConfig);
}

tSIRF_RESULT getConfig(AGPSConfig *pAgpsConfig)
{
   /* System Setting */
   CsrStrNCpy(pAgpsConfig->GPSPort,       "\\\\.\\COM20", sizeof(pAgpsConfig->GPSPort));
   CsrStrNCpy(pAgpsConfig->ResetPort,     "\\\\.\\COM21", sizeof(pAgpsConfig->ResetPort));
   CsrStrNCpy(pAgpsConfig->AUXPort,       "\\\\.\\COM19", sizeof(pAgpsConfig->AUXPort));
   CsrStrNCpy(pAgpsConfig->GPRSModemPort, "\\\\.\\COM20", sizeof(pAgpsConfig->GPRSModemPort));

   /* LSM Common settings */
   pAgpsConfig->numOfSessions           = 10;
   pAgpsConfig->isSecure                = 0;   /* No[0] Yes[1] */
   pAgpsConfig->serverPort              = 0;
   CsrStrNCpy(pAgpsConfig->serverAddress, "", sizeof(pAgpsConfig->serverAddress) - 1);
   
   /* LSM Session settings */
   pAgpsConfig->resetType     = 4;   /* Reset Type: Factory[0] Cold[1] Hot[2] Warm[3] Auto[4]*/
   pAgpsConfig->aidingType    = 2;   /* Aiding Type: NoAiding[0] LocalAiding[1] NetworkAiding[2] */
   pAgpsConfig->isPosTechMSB  = 1;   /* No[0] Yes[1] */
   pAgpsConfig->isPosTechMSA  = 0;   /* No[0] Yes[1] */
   pAgpsConfig->isPosTechAuto = 0;   /* No[0] Yes[1] */
   pAgpsConfig->isPosTechECID = 0;   /* No[0] Yes[1] */
   pAgpsConfig->prefLocMethod = 2;   /* PreferredLocMethod: MSB[0] MSA[1] NONE[2]*/

   pAgpsConfig->timeBetweenFixes = 1;
   pAgpsConfig->numberOfFixes    = 10;   /* Continous[0] n fixes[n] */

   pAgpsConfig->AssistProtocol   = 0;   /* AssistProtocol : SUPL_RRLP[0] SUPL_RRC[1] CP_RRLP[2] CP_RRC[3] */

   pAgpsConfig->approximateAltitude          = 0;
   pAgpsConfig->approximateLongitude         = 121.5862765;
   pAgpsConfig->approximateLatitude          = 31.2482364;
   pAgpsConfig->isApproximateLocationPresent = 0; /* Absent[0] Present[1] */
   
   pAgpsConfig->QoSinSUPLSTART = 0;   /* Absent[0] Present[1] */
   pAgpsConfig->maxLocAge = 0;
   pAgpsConfig->QoSMaxResponseTime = 120;
   pAgpsConfig->QoSVerticalAccuracy = 100;
   pAgpsConfig->QoSHorizontalAccuracy = 100;

   pAgpsConfig->identification = 0;
   pAgpsConfig->powerControl = 0;
   pAgpsConfig->triggers = 0;
   pAgpsConfig->isAPM = 0;

   pAgpsConfig->loggingType = 1;   /* Logging Type: None[0] SingleSession[1] MultiSession[2]*/
   CsrStrNCpy(pAgpsConfig->briefLogFileName,    "D:\\brieflog.txt", sizeof(pAgpsConfig->briefLogFileName)- 1);
   CsrStrNCpy(pAgpsConfig->agpsLogFileName,     "D:\\AGPSlog.txt", sizeof(pAgpsConfig->agpsLogFileName)- 1);
   CsrStrNCpy(pAgpsConfig->detailedLogFileName, "D:\\Detaillog.txt", sizeof(pAgpsConfig->detailedLogFileName)- 1);
   CsrStrNCpy(pAgpsConfig->prevLocFileName,     "", sizeof(pAgpsConfig->prevLocFileName)- 1);
   CsrStrNCpy(pAgpsConfig->slcLogFileName,      "D:\\SLClog.gp2", sizeof(pAgpsConfig->slcLogFileName)- 1);

   pAgpsConfig->isCapCGEE    = 0;   /* Absent[0] Present[1] */
   pAgpsConfig->isCapSGEE    = 0;   /* Absent[0] Present[1] */
   pAgpsConfig->eeFileUpdate = 0;   /* No[0] Yes[1] */

   return SIRF_SUCCESS;
}

#if ENABLE_NMEA_LISTENER
tSIRF_VOID LSM_NMEAListener( LSM_UINT8 type, LSM_UINT8* message, LSM_UINT32 messageLength )
{
   char msg[512];

   CsrMemCpy( msg, message, messageLength );
   msg[messageLength] = '\0';
   //SCI_TRACE_LOW:"%s"
   SCI_TRACE_ID(TRACE_TOOL_CONVERT,GPS_APP_210_112_2_18_0_33_11_1584,(uint8*)"s", msg);
   SCI_TRACE_LOW("KSTDEBUG: NMEA NMEA NMEA"); //KST DEBUG
} 
#endif /* ENABLE_NMEA_LISTENER */

#if ENABLE_DEBUG_LISTENER
tSIRF_VOID LSM_Debug_Listener( LSM_CHAR* pMsg )
{
   if(NULL == pMsg)
   {
      //SCI_TRACE_LOW:"APP: NULL debug Message."
      SCI_TRACE_ID(TRACE_TOOL_CONVERT,GPS_APP_219_112_2_18_0_33_11_1585,(uint8*)"");
      return;
   }
   
   //SCI_TRACE_LOW:"%s"
   SCI_TRACE_ID(TRACE_TOOL_CONVERT,GPS_APP_223_112_2_18_0_33_11_1586,(uint8*)"s", pMsg);
}
#endif /* ENABLE_DEBUG_LISTENER */

static void LSM_PositionCallBack(LsmCallbackData *pLsmCallbackData)
{
   tSIRF_RESULT result;
   AGPSConfig *pAgpsCfg;
   
   if (LSM_LPL_REF_LOC == pLsmCallbackData->eLsmEvent)
   {
      //SCI_TRACE_LOW:"APP: INFO: got ref loc"
      SCI_TRACE_ID(TRACE_TOOL_CONVERT,GPS_APP_234_112_2_18_0_33_11_1587,(uint8*)"");
   }
   
   if ( LSM_TO_CLOSE == pLsmCallbackData->eLsmEvent )
   {
      if(NULL != semPosition)
      {
         result = SIRF_PAL_OS_SEMAPHORE_Release( semPosition );
         if ( result != SIRF_SUCCESS )
         {
            //SCI_TRACE_LOW:"APP: ERROR: SIRF_PAL_OS_SEMAPHORE_Release() error: 0x%X\n"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,GPS_APP_244_112_2_18_0_33_11_1588,(uint8*)"d", result);
         }
      }
   }

   if ( LSM_LPL_LOC_W_QOS == pLsmCallbackData->eLsmEvent )
   {
      PositionCount++;
	  pAgpsCfg = getCfgPointer();
      if (LSM_LPL_MET_NUM_FIX == pLsmCallbackData->eLsmEvent)
      {
         //SCI_TRACE_LOW:"App: INFO: got all (%d/%d) fixes"
         SCI_TRACE_ID(TRACE_TOOL_CONVERT,GPS_APP_255_112_2_18_0_33_11_1589,(uint8*)"dd", PositionCount, pAgpsCfg->numberOfFixes);
      }
      else
      {
         //SCI_TRACE_LOW:"APP: INFO: got a fix (%d/%d)"
         SCI_TRACE_ID(TRACE_TOOL_CONVERT,GPS_APP_259_112_2_18_0_33_11_1590,(uint8*)"dd", PositionCount, pAgpsCfg->numberOfFixes);
      }
   }
}


static LSM_BOOL getLSMCommonConfiguration(LSM_commonCfgData *pCommonCfgData)
{
   AGPSConfig *pAgpsCfg = getCfgPointer();
   CsrMemSet(pCommonCfgData, 0, sizeof(LSM_commonCfgData));
   
   /* GPS Port Settings parameter */
   pCommonCfgData->lsmInitData.gpsDevice.configType = LSM_ETRACKER;
   CsrStrNCpy( pCommonCfgData->lsmInitData.gpsDevice.DeviceConfig.trackerCfg.m_SerialPort,
            pAgpsCfg->GPSPort,
            sizeof(pCommonCfgData->lsmInitData.gpsDevice.DeviceConfig.trackerCfg.m_SerialPort));

   pCommonCfgData->lsmInitData.gpsDevice.DeviceConfig.trackerCfg.m_BaudRate = 115200;
   pCommonCfgData->lsmInitData.gpsDevice.DeviceConfig.trackerCfg.gpsStartMode = SIRFNAV_UI_CTRL_MODE_AUTO;

   /* use LPL default heap */
   pCommonCfgData->lsmInitData.lpl_mem.memSize = LPL_HEAP;
   pCommonCfgData->lsmInitData.lpl_mem.pMem = NULL;

   pCommonCfgData->lsmInitData.lplPowerControl.cgpsCtrl = LSM_DISABLE_CGPSC;
   pCommonCfgData->lsmInitData.lplPowerControl.gpsPwrCtrlCap = LSM_NO_GPS_POWER_CONTROL;
   
   /*set ICD */
   pCommonCfgData->CommonCfgBitMap.isSLCICDSet = 1;
   pCommonCfgData->ICDFromUser.AI3ICD = AI3_REV_NUM_22;
   pCommonCfgData->ICDFromUser.FICD   = F_REV_NUM_21;

   /* Network Setting parameter */
   pCommonCfgData->CommonCfgBitMap.isCommonNTWKParamSet = 1;
   pCommonCfgData->commonNetworkCfg.bSecure = pAgpsCfg->isSecure;
   CsrStrCpy(pCommonCfgData->commonNetworkCfg.SuplServerInfo.ipAddr, 
                                            pAgpsCfg->serverAddress);
   pCommonCfgData->commonNetworkCfg.SuplServerInfo.port = pAgpsCfg->serverPort;
   pCommonCfgData->commonNetworkCfg.BindAddress.bBind = 0;

   /* Reset Type */
   pCommonCfgData->CommonCfgBitMap.isResetTypeSet = 1;
   pCommonCfgData->gpsResetType = (eLSM_RESET_TYPE)pAgpsCfg->resetType;

   /* SET Capabilities */ 
   pCommonCfgData->CommonCfgBitMap.isCapSet = 1;
   pCommonCfgData->PosMethodCapabilities.cap.msBased = (LSM_UINT8)pAgpsCfg->isPosTechMSB;    
   pCommonCfgData->PosMethodCapabilities.cap.msAssisted = (LSM_UINT8)pAgpsCfg->isPosTechMSA; 
   pCommonCfgData->PosMethodCapabilities.cap.autonomous = (LSM_UINT8)pAgpsCfg->isPosTechAuto; 
   pCommonCfgData->PosMethodCapabilities.cap.eCID = (LSM_UINT8)pAgpsCfg->isPosTechECID;
   pCommonCfgData->PosMethodCapabilities.prefLocMethod = (eLSM_PREFERRED_LOC_METHOD)pAgpsCfg->prefLocMethod;
   
   /* Aiding Type */ 
   pCommonCfgData->CommonCfgBitMap.isAidingTypeParamSet = 1;
   pCommonCfgData->aidingType = (eLSM_AIDING_TYPE)pAgpsCfg->aidingType;

   /* Logging Information */
   pCommonCfgData->CommonCfgBitMap.isLogInfoSet = 1;
   pCommonCfgData->logInfo.loggingtype = (eLSM_LOGGING_TYPE)pAgpsCfg->loggingType;
   pCommonCfgData->logInfo.logfiles.briefLogFileName = pAgpsCfg->briefLogFileName;
   pCommonCfgData->logInfo.logfiles.agpsLogFileName = pAgpsCfg->agpsLogFileName;
   pCommonCfgData->logInfo.logfiles.detailedLogFileName =  pAgpsCfg->detailedLogFileName;
   pCommonCfgData->logInfo.logfiles.slcLogFileName =  pAgpsCfg->slcLogFileName;
   
   /* Posiiton Callback*/
   pCommonCfgData->lplStatusCallback = LSM_PositionCallBack; /*compulsory*/

#if ENABLE_NMEA_LISTENER
   pCommonCfgData->CommonCfgBitMap.isNMEAenabled = 1;
   pCommonCfgData->nmeaCallback = LSM_NMEAListener;
#endif    

#if ENABLE_DEBUG_LISTENER
   pCommonCfgData->CommonCfgBitMap.isDbgListenerSet = 0;
   pCommonCfgData->dbgCallback = LSM_Debug_Listener; 
#endif

    return 1;
}

static LSM_BOOL getLSMSessionCfgData(LSM_SIsessionCfgData *pSessionCfgData)
{
#ifdef KST_AGPS
   ERR_MNPHONE_CODE_E mnres;
   APP_MN_NCELL_RSSI_CNF_T ncell_meas;
   MN_PHONE_CELL_MEAS_T scell_meas;
#endif /* KST_AGPS */
   AGPSConfig *pAgpsCfg = getCfgPointer();
   CsrMemSet(pSessionCfgData, 0, sizeof(LSM_SIsessionCfgData));

   /*Approximate Location*/
   pSessionCfgData->SessionCfgBitMap.isApproxLocInfoSet = pAgpsCfg->isApproximateLocationPresent;
   pSessionCfgData->approxLoc.lat = pAgpsCfg->approximateLatitude;
   pSessionCfgData->approxLoc.lon = pAgpsCfg->approximateLongitude;
   pSessionCfgData->approxLoc.alt = pAgpsCfg->approximateAltitude;
   pSessionCfgData->approxLoc.horErr = DEFAULT_HOR_ERROR;
   pSessionCfgData->approxLoc.verErr = DEFAULT_VER_ERROR;

#ifdef KST_AGPS
   /* Get GSM infor if it is available */
   mnres = MNPHONE_GetMeasReportEx(MN_DUAL_SYS_1, &ncell_meas, &scell_meas);
   if (mnres == ERR_MNPHONE_NO_ERR)
   {
         pSessionCfgData->SessionCfgBitMap.isGSMInfoSet = SIRF_TRUE;
         pSessionCfgData->gsmInfo.mcc = scell_meas.lai.mcc;
         pSessionCfgData->gsmInfo.mnc = scell_meas.lai.mnc;
         pSessionCfgData->gsmInfo.lac = scell_meas.lai.lac;
         pSessionCfgData->gsmInfo.cid = scell_meas.cell_id;
   }
#endif /* KST_AGPS */

   /*Network Parameter settings*/
   pSessionCfgData->SessionCfgBitMap.isSessionNTWKParamSet = 1;
   pSessionCfgData->sessionNetworkCfg.BindAddress.bBind = 0; /* FALSE */
   pSessionCfgData->sessionNetworkCfg.bSecure = pAgpsCfg->isSecure;
   strcpy(pSessionCfgData->sessionNetworkCfg.SuplServerInfo.ipAddr, pAgpsCfg->serverAddress);
   pSessionCfgData->sessionNetworkCfg.SuplServerInfo.port = pAgpsCfg->serverPort;

   /*Reset Type*/    
   pSessionCfgData->SessionCfgBitMap.isResetTypeSet = 1;
   pSessionCfgData->gpsResetType = (eLSM_RESET_TYPE)pAgpsCfg->resetType;
   
   /* Aiding Type */ 
   pSessionCfgData->SessionCfgBitMap.isAidingTypeParamSet = 1;
   pSessionCfgData->aidingType = (eLSM_AIDING_TYPE)pAgpsCfg->aidingType;

   /*QoS*/
   pSessionCfgData->QoS.horizontalAccuracy = pAgpsCfg->QoSHorizontalAccuracy;
   pSessionCfgData->QoS.verticalAccuracy = pAgpsCfg->QoSVerticalAccuracy;
   pSessionCfgData->QoS.maxResponseTime = pAgpsCfg->QoSMaxResponseTime;
   pSessionCfgData->isQoSInSUPLStart = pAgpsCfg->QoSinSUPLSTART;

   pSessionCfgData->numFixes = pAgpsCfg->numberOfFixes;
   pSessionCfgData->timeInterval = pAgpsCfg->timeBetweenFixes;
   
   pSessionCfgData->isNWConnectionAvailable = pAgpsCfg->networkStatus;
   return (LSM_TRUE); /*TRUE*/
}

tSIRF_RESULT LPL_Init(void) 
{
   tSIRF_RESULT result;

   AGPSConfig *pAgpsCfg = getCfgPointer();
   getConfig(pAgpsCfg); 
   
   /* Reset setting parameter for ASIC evaluation board*/
   CsrStrNCpy((char*)g_userPalConfig.on_off_port, UI_CTRL_DEFAULT_ON_OFF_PORT, MAX_PORT_NUM_STRING_LENGTH);
   g_userPalConfig.on_off_line_usage                = UI_CTRL_MODE_HW_ON_NONE;
   g_userPalConfig.ext_sreset_n_line_usage          = UI_CTRL_MODE_HW_RESET_NONE;

   result = gpioInitialization();
   if ( result != SIRF_SUCCESS )
   {
      //SCI_TRACE_LOW:"APP: gpioInitialization() error: 0x%X\n"
      SCI_TRACE_ID(TRACE_TOOL_CONVERT,GPS_APP_413_112_2_18_0_33_11_1591,(uint8*)"d", result );
      return result;
   }

   if (0 == getLSMCommonConfiguration(&LSM_CommonCfg))
   {
      //SCI_TRACE_LOW:"APP: ERROR: Init cannot set LSM Common configuration data"
      SCI_TRACE_ID(TRACE_TOOL_CONVERT,GPS_APP_419_112_2_18_0_33_11_1592,(uint8*)"");
      return (SIRF_FAILURE);
   }

   //SCI_TRACE_LOW:"APP: INFO: LSM version is %s"
   SCI_TRACE_ID(TRACE_TOOL_CONVERT,GPS_APP_423_112_2_18_0_33_11_1593,(uint8*)"s", LSM_GetLSMVersion());
   
   if (LSM_FALSE == LSM_Init(&LSM_CommonCfg))
   {
      //SCI_TRACE_LOW:"APP: ERROR: Init failed calling LSM_Init"
      SCI_TRACE_ID(TRACE_TOOL_CONVERT,GPS_APP_427_112_2_18_0_33_11_1594,(uint8*)"");
      return SIRF_FAILURE;
   }

   return SIRF_SUCCESS;
}

tSIRF_RESULT LPL_Deinit(void) 
{
   tSIRF_RESULT result;

   if (LSM_FALSE == LSM_Deinit())
   {
      //SCI_TRACE_LOW:"APP: ERROR: Deinit failed calling LSM_Deinit"
      SCI_TRACE_ID(TRACE_TOOL_CONVERT,GPS_APP_440_112_2_18_0_33_11_1595,(uint8*)"");
   }

   result = gpioDeInitialization();
   if ( result != SIRF_SUCCESS )
   {
      //SCI_TRACE_LOW:"gpioDeInitialization() error: 0x%X\n"
      SCI_TRACE_ID(TRACE_TOOL_CONVERT,GPS_APP_446_112_2_18_0_33_11_1596,(uint8*)"d", result );
      return result;
   }

   return SIRF_SUCCESS;
}

/* Open is called when CreateFile is called by the application */
tSIRF_RESULT LPL_Open(void)
{
   tSIRF_RESULT ret;
#ifdef KST_AGPS
   SETID_Info set_id;
   MN_IMSI_T imsi_id;
   ERR_MNPHONE_CODE_E mnres;
   APP_MN_NCELL_RSSI_CNF_T ncell_meas;
   MN_PHONE_CELL_MEAS_T scell_meas;
   LSM_netCellID cellInfo;
#endif
   AGPSConfig *pAgpsCfg = getCfgPointer();
   getConfig(pAgpsCfg); 
   
   PositionCount = 0;
   
   ret = SIRF_PAL_OS_SEMAPHORE_Create(&semPosition, 0);
   if(SIRF_SUCCESS != ret)
   {
      //SCI_TRACE_LOW:"APP: Semaphore (semPosition) Creation Failed!"
      SCI_TRACE_ID(TRACE_TOOL_CONVERT,GPS_APP_472_112_2_18_0_33_11_1597,(uint8*)"");
      return ret;
   }

   if (0 == getLSMSessionCfgData(&LSM_SessionCfg))
   {
      //SCI_TRACE_LOW:"APP: ERROR: Open cannot obtain session cfg data"
      SCI_TRACE_ID(TRACE_TOOL_CONVERT,GPS_APP_478_112_2_18_0_33_11_1598,(uint8*)"");
      (tSIRF_VOID)SIRF_PAL_OS_SEMAPHORE_Delete( semPosition );
      return SIRF_FAILURE;
   }

#ifdef KST_AGPS
   imsi_id = MNSIM_GetImsiEx(MN_DUAL_SYS_1);
   if ((imsi_id.imsi_len < 1) || (imsi_id.imsi_len > SET_ID_LENGTH))
   {
      //SCI_TRACE_LOW:"APP: ERROR: Unable to get SET ID: %d"
      SCI_TRACE_ID(TRACE_TOOL_CONVERT,GPS_APP_487_112_2_18_0_33_11_1599,(uint8*)"d");
   }
   else
   {
   	set_id.SETidType = LSM_IMSI;
	CsrMemCpy(&set_id.SETidValue[0], &imsi_id.imsi_val[0], imsi_id.imsi_len);
      CP_SendSETIDInfo(&set_id, SIRF_TRUE);
   }
   mnres = MNPHONE_GetMeasReportEx(MN_DUAL_SYS_1, &ncell_meas, &scell_meas);
   if (mnres == ERR_MNPHONE_NO_ERR)
   {
         cellInfo.eNetworkType = LSM_GSM;
         cellInfo.m.gsm_cellid.mcc = scell_meas.lai.mcc;
         cellInfo.m.gsm_cellid.mnc = scell_meas.lai.mnc;
         cellInfo.m.gsm_cellid.lac = scell_meas.lai.lac;
         cellInfo.m.gsm_cellid.cid = scell_meas.cell_id;
         if ((cellInfo.m.gsm_cellid.mcc == 0) && (cellInfo.m.gsm_cellid.mnc == 0) && (cellInfo.m.gsm_cellid.lac == 0) && (cellInfo.m.gsm_cellid.cid == 0))
         {
             CP_SendCellInfo(&cellInfo, LSM_FALSE);
         }
         else
         {
             CP_SendCellInfo(&cellInfo, LSM_TRUE);
         }
   }
#endif /* KST_AGPS */

   sessionID = LSM_Start(&LSM_SessionCfg);
   if (0 == sessionID)
   {
      //SCI_TRACE_LOW:"APP: ERROR: Open failed calling LSM_Start"
      SCI_TRACE_ID(TRACE_TOOL_CONVERT,GPS_APP_517_112_2_18_0_33_12_1600,(uint8*)"");
      (tSIRF_VOID)SIRF_PAL_OS_SEMAPHORE_Delete( semPosition );
      return SIRF_FAILURE;
   }
                          
   return SIRF_SUCCESS;
}

tSIRF_RESULT LPL_Close (void) 
{
   if (LSM_FALSE == LSM_Stop(sessionID, LSM_FALSE ))
   {
      //SCI_TRACE_LOW:"APP: ERROR: Close failed calling LSM_Stop!"
      SCI_TRACE_ID(TRACE_TOOL_CONVERT,GPS_APP_529_112_2_18_0_33_12_1601,(uint8*)"");
      return SIRF_FAILURE;
   }
   (tSIRF_VOID)SIRF_PAL_OS_SEMAPHORE_Delete( semPosition );
   semPosition = NULL;
   return SIRF_SUCCESS;
}

static tSIRF_RESULT gpioInitialization(tSIRF_VOID)
{
   tSIRF_RESULT result;

   result = SIRF_PAL_HW_OpenRESET(SIRF_PAL_HW_RESET_HIGH);

   if(SIRF_SUCCESS == result)
   {
      result = SIRF_PAL_HW_OpenON_OFF(SIRF_PAL_HW_ON_LOW);

      if(SIRF_SUCCESS != result)
      {
         //SCI_TRACE_LOW:"Failed to open On_Off port in Main \n"
         SCI_TRACE_ID(TRACE_TOOL_CONVERT,GPS_APP_549_112_2_18_0_33_12_1602,(uint8*)"");
         return result;
      }
   }
   else
   {
      //SCI_TRACE_LOW:"Failed to open Reset port in Main \n"
      SCI_TRACE_ID(TRACE_TOOL_CONVERT,GPS_APP_555_112_2_18_0_33_12_1603,(uint8*)"");
      return result;
   }

   return result;
}

static tSIRF_RESULT gpioDeInitialization(tSIRF_VOID)
{
   tSIRF_RESULT result;
   tSIRF_RESULT ret_val = SIRF_FAILURE;

   result = SIRF_PAL_HW_CloseRESET();
   if(SIRF_SUCCESS == result)
   {
      ret_val = result;
   }
   result = SIRF_PAL_HW_CloseON_OFF();
   if(SIRF_SUCCESS == result)
   {
      ret_val = result;
   }
   return ret_val;
}




BLOCK_ID s_gps_thread_id = SCI_INVALID_BLOCK_ID;
#define GPS_OS_THREAD_STACK_SIZE (4*1024)
#define s_gps_thread_task_priority     PRI_GPS_TASK    //31
#define GPS_OS_THREAD_QUEUE_NUM    2
SCI_EVENT_GROUP_PTR gps_entry_event = NULL;
void Low_gps_Init(void);
void StartGpsThread(void);


PUBLIC void gps_Init(void)
{

    StartGpsThread();
    gps_entry_event = SCI_CreateEvent("gps_init_event");
    SCI_ASSERT(gps_entry_event != NULL);
    
    SCI_SetEvent(gps_entry_event, 1, SCI_OR);
    
}
PUBLIC void gps_deinit(void)
{
    if(s_gps_thread_id != SCI_INVALID_BLOCK_ID)
    {
        SCI_TerminateThread(s_gps_thread_id);
        SCI_DeleteThread(s_gps_thread_id);
        s_gps_thread_id = SCI_INVALID_BLOCK_ID;
    }

    if(gps_entry_event != NULL)
    {
        SCI_DeleteEvent(gps_entry_event);
        gps_entry_event = NULL;
    }
    
}

LOCAL void Gps_Task_Entry(uint32 argc, void *argv)
{
    uint32 u32_actual_event;
    
    do
    {
        SCI_GetEvent(gps_entry_event, 
                     1, 
                      SCI_OR_CLEAR, 
                      &u32_actual_event, 
                      SCI_WAIT_FOREVER);
    
        //SCI_TRACE_LOW:"start gps init"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,GPS_APP_631_112_2_18_0_33_12_1604,(uint8*)"");
        
        if(u32_actual_event)
        Low_gps_Init();
        
        //SCI_TRACE_LOW:"end gps init"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,GPS_APP_636_112_2_18_0_33_12_1605,(uint8*)"");
    }while(1);/*lint !e506*/

    
}

void StartGpsThread(void)
{
	
	//SCI_TRACE_LOW:"[GPS]StartGpsThread:priority=0x%X\r\n"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,GPS_APP_645_112_2_18_0_33_12_1606,(uint8*)"d", s_gps_thread_task_priority);
    s_gps_thread_id = SCI_CreateThread("T_GPS_SCHED", "Q_GPS_SCHED", Gps_Task_Entry, 0, NULL, GPS_OS_THREAD_STACK_SIZE, 
									GPS_OS_THREAD_QUEUE_NUM, s_gps_thread_task_priority, SCI_PREEMPT, SCI_AUTO_START);

    SCI_ASSERT(SCI_INVALID_BLOCK_ID != s_gps_thread_id);/*assert verified*/
    
    //SCI_TRACE_LOW:"[GPS]OS_StartThread - s_gps_sched_thread_id = 0x%X\r\n"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,GPS_APP_651_112_2_18_0_33_12_1607,(uint8*)"d", s_gps_thread_id);
}


void Low_gps_Init(void)
{
   unsigned int  sessionCount = 0;
   AGPSConfig *pAgpsCfg = NULL;
   tSIRF_RESULT result;

   Heap_Open();

   //SCI_TRACE_LOW:"APP: ============ SiRFNavIV LSM Spreadtrum Demo App ============="
   SCI_TRACE_ID(TRACE_TOOL_CONVERT,GPS_APP_663_112_2_18_0_33_12_1608,(uint8*)"");

    /*Initialize the PAL*/
   result = SIRF_PAL_Init();
   if ( SIRF_SUCCESS !=  result)
   {
      //SCI_TRACE_LOW:"ERROR: SIRF_PAL_Init Failed %d"
      SCI_TRACE_ID(TRACE_TOOL_CONVERT,GPS_APP_669_112_2_18_0_33_12_1609,(uint8*)"d", result);
      return; /* FAILURE */
   }

   pAgpsCfg =  getCfgPointer();

   result = LPL_Init();
   if(SIRF_SUCCESS != result)
   {
      //SCI_TRACE_LOW:"APP: LPL Init Failed: 0x%x"
      SCI_TRACE_ID(TRACE_TOOL_CONVERT,GPS_APP_678_112_2_18_0_33_12_1610,(uint8*)"d",result);
       /*Destroy the PAL*/
      result = SIRF_PAL_Destroy();
      if (SIRF_SUCCESS !=  result)
      {
         //SCI_TRACE_LOW:"ERROR: SIRF_PAL_Destroy Failed %d"
         SCI_TRACE_ID(TRACE_TOOL_CONVERT,GPS_APP_683_112_2_18_0_33_12_1611,(uint8*)"d", result);
      }
      Heap_Close();
      return; /* FAILURE */
   }

   while(sessionCount < pAgpsCfg->numOfSessions)
   {
      //SCI_TRACE_LOW:"APP: Session: %d of %d"
      SCI_TRACE_ID(TRACE_TOOL_CONVERT,GPS_APP_691_112_2_18_0_33_12_1612,(uint8*)"dd", sessionCount+1, pAgpsCfg->numOfSessions);
      result = LPL_Open();
      if(SIRF_SUCCESS != result)
      {
         //SCI_TRACE_LOW:"APP: LPL Session Open Failed"
         SCI_TRACE_ID(TRACE_TOOL_CONVERT,GPS_APP_695_112_2_18_0_33_12_1613,(uint8*)"");
         break;
      }

      /* Waiting for position */
      SIRF_PAL_OS_SEMAPHORE_Wait(semPosition, SIRF_TIMEOUT_INFINITE);


      result = LPL_Close();
      if (SIRF_SUCCESS !=  result)
      {
         //SCI_TRACE_LOW:"ERROR: LPL_Close Failed %d"
         SCI_TRACE_ID(TRACE_TOOL_CONVERT,GPS_APP_706_112_2_18_0_33_12_1614,(uint8*)"d", result);
      }
      
      (tSIRF_VOID)SIRF_PAL_OS_THREAD_Sleep( (tSIRF_UINT32)(500));
      
      //SCI_TRACE_LOW:"APP: LPL Session Closed."
      SCI_TRACE_ID(TRACE_TOOL_CONVERT,GPS_APP_711_112_2_18_0_33_12_1615,(uint8*)"");
      sessionCount++;
   }

   result = LPL_Deinit();
   if (SIRF_SUCCESS !=  result)
   {
      //SCI_TRACE_LOW:"ERROR: LPL_Deinit Failed %d"
      SCI_TRACE_ID(TRACE_TOOL_CONVERT,GPS_APP_718_112_2_18_0_33_12_1616,(uint8*)"d", result);
   }

   //SCI_TRACE_LOW:"APP: All Sessions completed"
   SCI_TRACE_ID(TRACE_TOOL_CONVERT,GPS_APP_721_112_2_18_0_33_12_1617,(uint8*)"");

   /*Destroy the PAL*/
   result = SIRF_PAL_Destroy();
   if (SIRF_SUCCESS !=  result)
   {
      //SCI_TRACE_LOW:"ERROR: SIRF_PAL_Destroy Failed %d"
      SCI_TRACE_ID(TRACE_TOOL_CONVERT,GPS_APP_727_112_2_18_0_33_12_1618,(uint8*)"d", result);
   }

   Heap_Close();
   return; /* SUCCESS */
}

/**
 * @}
 */
