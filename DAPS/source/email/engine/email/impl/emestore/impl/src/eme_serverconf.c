
#include "we_def.h"
#include "eme_utils_api.h"
#include "eme_file_api.h"
#include "eme_dataaccess_priv.h"
#include "eme_serverconf_api.h"

#define EME_ELEMENT_STR_LEN       24
#define EME_ELEMENT_BEG_FORMAT    "<%s>"
#define EME_ELEMENT_END_FORMAT    "</%s>"
#define EME_SERVERCONF_BEG        "<SERVERCONF>"
#define EME_SERVERCONF_END        "</SERVERCONF>"

#define EMEFS_SERVER_CONF_FILE             "C:\\TechSoft\\email\\serverconf.txt"
#define EMEFS_SERVER_CONF_FILE_TEST        EMEFS_UDISK_ROOT"\\serverconf.txt"

typedef struct St_EmeElmInfo
{
   WE_UINT32  iId;
   WE_UCHAR   aucElm[EME_ELEMENT_STR_LEN];
}StEmeElmInfo;

enum
{
   EME_ACCOUNTNAME,
   EME_SERVERNAME,
   EME_SERVERTYPE,
   EME_SMTPNAME,
   EME_SMTPPORT,
   EME_SMTPSSL,
   EME_SMTPAUTH,
   EME_POP3NAME,
   EME_POP3PORT,
   EME_POP3SSL,
   EME_POP3AUTH,
   EME_IMAPNAME,
   EME_IMAPPORT,
   EME_IMAPSSL,
   EME_IMAPAUTH,
   EME_SIM1NETTYPE,
   EME_SIM2NETTYPE,
   EME_SIM3NETTYPE,
   EME_SIM4NETTYPE
};

const StEmeElmInfo gst_SerConf[] =
{
   {EME_ACCOUNTNAME, "ACCOUNTNAME"},
   {EME_SERVERNAME,  "SERVERNAME" },
   {EME_SERVERTYPE,  "SERVERTYPE" },
   {EME_SMTPNAME,    "SMTPNAME"   },
   {EME_SMTPPORT,    "SMTPPORT"   },
   {EME_SMTPSSL,     "SMTPSSL"    },
   {EME_SMTPAUTH,    "SMTPAUTH"   },
   {EME_POP3NAME,    "POP3NAME"   },
   {EME_POP3PORT,    "POP3PORT"   },
   {EME_POP3SSL,     "POP3SSL"    },
   {EME_POP3AUTH,    "POP3AUTH"   },
   {EME_IMAPNAME,    "IMAPNAME"   },
   {EME_IMAPPORT,    "IMAPPORT"   },
   {EME_IMAPSSL,     "IMAPSSL"    },
   {EME_IMAPAUTH,    "IMAPAUTH"   },
   {EME_SIM1NETTYPE, "SIM1NETTYPE"},
   {EME_SIM2NETTYPE, "SIM2NETTYPE"},
   {EME_SIM3NETTYPE, "SIM3NETTYPE"},
   {EME_SIM4NETTYPE, "SIM4NETTYPE"},
};


static WE_VOID EME_SerConf_ParseOneServerConf(WE_UCHAR *pucOneLine, EMS_ServerConf *pstSerConf)
{
   WE_CHAR   *pcElmBeg = NULL;
   WE_CHAR   *pcElmEnd = NULL;
   WE_UINT16 *pwcElm = NULL;
   WE_CHAR   aucElmBeg[EME_ELEMENT_STR_LEN] = {0};
   WE_CHAR   aucElmEnd[EME_ELEMENT_STR_LEN] = {0};
   WE_CHAR   aucElm[EME_ELEMENT_STR_LEN] = {0};
   WE_UINT32 uiCnt = sizeof(gst_SerConf) / sizeof(StEmeElmInfo);
   WE_UINT32 uiIndex = 0;

   EME_UTILS_MEMSET(pstSerConf, 0x00, sizeof(EMS_ServerConf));

   while(uiIndex < uiCnt)
   {
      EME_UTILS_MEMSET(aucElm, 0, EME_ELEMENT_STR_LEN);
      EME_UTILS_MEMSET(aucElmBeg, 0, EME_ELEMENT_STR_LEN);
      EME_UTILS_MEMSET(aucElmEnd, 0, EME_ELEMENT_STR_LEN);
      EME_UTILS_SPRINTF(aucElmBeg, EME_ELEMENT_BEG_FORMAT, gst_SerConf[uiIndex].aucElm);
      EME_UTILS_SPRINTF(aucElmEnd, EME_ELEMENT_END_FORMAT, gst_SerConf[uiIndex].aucElm);

      pcElmBeg = EME_UTILS_STRSTR((WE_CHAR*)pucOneLine, aucElmBeg);
      pcElmEnd = EME_UTILS_STRSTR((WE_CHAR*)pucOneLine, aucElmEnd);

      if(pcElmBeg != NULL && pcElmEnd != NULL && pcElmEnd > pcElmBeg)
      {
         WE_UINT32 uiCpyLen = 0;

         pcElmBeg += EME_UTILS_STRLEN(aucElmBeg);
         EME_UTILS_MEMCPY(aucElm, pcElmBeg, pcElmEnd - pcElmBeg);
         
         pwcElm = EME_UTILS_UTF8TOWSTR(NULL, aucElm);
         if(NULL != pwcElm)
         {
            uiCpyLen = (EME_UTILS_WSTRLEN(pwcElm) > EMS_SERCONF_STR_MAX_LEN ? EMS_SERCONF_STR_MAX_LEN : EME_UTILS_WSTRLEN(pwcElm)) * 2;

            switch(gst_SerConf[uiIndex].iId)
            {
            case EME_ACCOUNTNAME:
               EME_UTILS_MEMCPY(pstSerConf->wszActName, pwcElm, uiCpyLen);
               break;
            case EME_SERVERNAME:
               EME_UTILS_MEMCPY(pstSerConf->wszServerName, pwcElm, uiCpyLen);
               break;
            case EME_SERVERTYPE:
               pstSerConf->uiServerType = (WE_UINT32)EME_UTILS_ATOI(aucElm);
               break;
            case EME_SMTPNAME:
               EME_UTILS_MEMCPY(pstSerConf->wszSmtpServer, pwcElm, uiCpyLen);
               break;
            case EME_SMTPPORT:
               uiCpyLen = (EME_UTILS_WSTRLEN(pwcElm) > EMS_SERCONF_PORT_MAX_LEN ? EMS_SERCONF_PORT_MAX_LEN : EME_UTILS_WSTRLEN(pwcElm)) * 2;
               EME_UTILS_MEMCPY(pstSerConf->wszSmtpPort, pwcElm, uiCpyLen);
               break;
            case EME_SMTPSSL:
               pstSerConf->bSmtpSSL = (WE_CHAR)EME_UTILS_ATOI(aucElm);
               break;
            case EME_SMTPAUTH:
               pstSerConf->bSmtpAuth = (WE_CHAR)EME_UTILS_ATOI(aucElm);
               break;
            case EME_POP3NAME:
               EME_UTILS_MEMCPY(pstSerConf->wszPop3Server, pwcElm, uiCpyLen);
               break;
            case EME_POP3PORT:
               uiCpyLen = (EME_UTILS_WSTRLEN(pwcElm) > EMS_SERCONF_PORT_MAX_LEN ? EMS_SERCONF_PORT_MAX_LEN : EME_UTILS_WSTRLEN(pwcElm)) * 2;
               EME_UTILS_MEMCPY(pstSerConf->wszPop3Port, pwcElm, uiCpyLen);
               break;
            case EME_POP3SSL:
               pstSerConf->bPop3SSL = (WE_CHAR)EME_UTILS_ATOI(aucElm);
               break;
            case EME_POP3AUTH:
               break;
            case EME_IMAPNAME:
               EME_UTILS_MEMCPY(pstSerConf->wszImapServer, pwcElm, uiCpyLen);
               break;
            case EME_IMAPPORT:
               uiCpyLen = (EME_UTILS_WSTRLEN(pwcElm) > EMS_SERCONF_PORT_MAX_LEN ? EMS_SERCONF_PORT_MAX_LEN : EME_UTILS_WSTRLEN(pwcElm)) * 2;
               EME_UTILS_MEMCPY(pstSerConf->wszImapPort, pwcElm, uiCpyLen);
               break;
            case EME_IMAPSSL:
               pstSerConf->bImapSSL = (WE_CHAR)EME_UTILS_ATOI(aucElm);
               break;
            case EME_IMAPAUTH:
               break;
            case EME_SIM1NETTYPE:
               pstSerConf->sim1NetType = (WE_CHAR)EME_UTILS_ATOI(aucElm);
               break;
            case EME_SIM2NETTYPE:
               pstSerConf->sim2NetType = (WE_CHAR)EME_UTILS_ATOI(aucElm);
               break;
            case EME_SIM3NETTYPE:
               pstSerConf->sim3NetType = (WE_CHAR)EME_UTILS_ATOI(aucElm);
               break;
            case EME_SIM4NETTYPE:
               pstSerConf->sim4NetType = (WE_CHAR)EME_UTILS_ATOI(aucElm);
               break;
            default:
               break;
            }/*switch(gst_SerConf[uiIndex].iId)*/

            EME_UTILS_FREEIF(NULL, pwcElm);
         }/*if(NULL != pwcElm)*/

      }/*if(pcElmBeg != NULL && pcElmEnd != NULL && pcElmEnd > pcElmBeg)*/

      uiIndex++;
   }/*while(uiIndex < uiCnt)*/
   
   return;
}

extern void SP_CFG_GetSerConfigData(char** data_ptr, uint32* data_len);
WE_INT EME_SerConf_GetData(FILE_HANDLE hMgr, WE_UINT32 *puiNum, EMS_ServerConf *pstSerConf)
{
   WE_INT    nErr = EME_SUCCESS;
   
   WE_UCHAR  *pucFileBuff = NULL;
   WE_UINT32 uiFileSize = 0;
   WE_UCHAR  *pucTmpBeg = NULL;
   WE_UCHAR  *pucTmpEnd = NULL;
   WE_UCHAR  *pucSerConfBeg = NULL;
   WE_UCHAR  *pucSerConfEnd = NULL;
   WE_UINT32 uiIndex = 0;
   WE_UCHAR  *pucTmpBuff = 0;

   if(NULL == puiNum)
   {
      return EME_BADPARM;
   }
   
   do 
   {
#if 0
      EME_DataAccess_GetFileBuffer(hMgr, EMEFS_SERVER_CONF_FILE_TEST, (WE_CHAR**)&pucFileBuff, &uiFileSize);
      if(pucFileBuff == NULL || 0 == uiFileSize)
      {
         EME_DataAccess_GetFileBuffer(hMgr, EMEFS_SERVER_CONF_FILE, (WE_CHAR**)&pucFileBuff, &uiFileSize);
      }
      EME_UTILS_BREAKNULL(pucFileBuff, nErr, EME_FAILED, "get file failed!!!");
#else
      SP_CFG_GetSerConfigData(NULL, &uiFileSize);
      if(uiFileSize == 0)
      {
         nErr = EME_FAILED;
         EME_UTILS_BREAKIF(nErr, "get serconfig data size failed!!");
      }
      pucFileBuff = EME_UTILS_MALLOC(NULL, uiFileSize);
      EME_UTILS_BREAKNULL(pucFileBuff, nErr, EME_FAILED, "get file failed!!!");
      SP_CFG_GetSerConfigData(pucFileBuff, &uiFileSize);
#endif

      pucTmpBeg = pucFileBuff;

      /*check the secerconf.txt file is encode by utf8*/
      if(!(0xEF == *(pucFileBuff+0) && 0xBB == *(pucFileBuff+1) && 0xBF == *(pucFileBuff+2)))
      {
         nErr = EME_FAILED;
         EME_UTILS_BREAKIF(nErr, "the file encode is not utf8!!");
      }

      pucTmpBeg = pucFileBuff + 3;
      
      while(pucTmpBeg < pucFileBuff + uiFileSize)
      {
         while((0x20 == *pucTmpBeg || 0x09 == *pucTmpBeg))pucTmpBeg++;//skip blank and tab key

         pucTmpEnd = (WE_UCHAR*)EME_UTILS_STRSTR((WE_CHAR*)pucTmpBeg, "\r\n");
         if(pucTmpEnd != NULL)
         {
            if((pucTmpEnd == pucTmpBeg) || (*pucTmpBeg == '#'))
            {
               pucTmpBeg = pucTmpEnd + 2;
               continue;
            }
         }
         else
         {
            pucTmpEnd = pucFileBuff + uiFileSize;
            if(*pucTmpBeg == '#')
            {
               pucTmpBeg = pucTmpEnd;
               continue;
            }
         }

         pucSerConfBeg = (WE_UCHAR*)EME_UTILS_STRSTR((WE_CHAR*)pucTmpBeg, EME_SERVERCONF_BEG);
         pucSerConfEnd = (WE_UCHAR*)EME_UTILS_STRSTR((WE_CHAR*)pucTmpBeg, EME_SERVERCONF_END);

         if(NULL != pucSerConfBeg && NULL != pucSerConfEnd )
         {
            if(pucSerConfEnd > pucSerConfBeg)
            {
               pucSerConfEnd += EME_UTILS_STRLEN(EME_SERVERCONF_END);
               
               if(uiIndex < *puiNum && NULL != pstSerConf)
               {
                  pucTmpBuff = EME_UTILS_MALLOC(NULL, pucSerConfEnd - pucSerConfBeg + 1);
                  EME_UTILS_BREAKNULL(pucTmpBuff, nErr, EME_FAILED, "no memory!");
                  EME_UTILS_MEMSET(pucTmpBuff, 0x00, pucSerConfEnd - pucSerConfBeg + 1);
                  
                  EME_UTILS_MEMCPY(pucTmpBuff, pucSerConfBeg, pucSerConfEnd - pucSerConfBeg);
                  
                  EME_SerConf_ParseOneServerConf(pucTmpBuff, &pstSerConf[uiIndex]);
                  
                  EME_UTILS_FREEIF(NULL, pucTmpBuff);
               }
               
               pucTmpBeg = pucSerConfEnd;
               uiIndex++;
            }
            else
            {
               pucTmpBeg = pucSerConfBeg;
            }
         }
         else
         {
            break;
         }
      }

      if(NULL == pstSerConf)
      {
         *puiNum = uiIndex;
      }
      
   } while (0);
   
   EME_UTILS_FREEIF(NULL, pucFileBuff);

   return nErr;
}
