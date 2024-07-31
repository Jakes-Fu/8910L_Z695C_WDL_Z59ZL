/*=====================================================================================
    FILE: eme_imap_parser.h
    MODULE NAME : IMAP

  
    GENERAL DESCRIPTION:
    Declaration of IMAP Stack parser functions

    TechFaith Software Confidential Proprietary
    Copyright (c) 2007 by TechFaith Software. All Rights Reserved.
=======================================================================================   
    Revision History

    Modification                   Tracking
    Date              Author       Number              Description of changes
    --------          -----        ---------------     ----------------------------------
    05/15/2007        Hubin                            Change copyright
=====================================================================================*/
#ifndef __EME_IMAP_PARSER_H__
#define __EME_IMAP_PARSER_H__



#include "eme_imap_utils.h"
#include "eme_imap_def.h"

/* check whether the response is end (which tagged with -tfMail-)
   if end, get response result and break.else if no response data, return
   and wait for more response */
#define EME_IMAP_BREAK_IF_RESPONSE_END(pcResponse)\
{\
   WE_CHAR* pcResponseTemp = NULL;\
\
   if (0 == *pcResponse)\
      return;\
\
   if(pcResponse == EME_UTILS_STRSTR(pcResponse,"-tfMail- "))\
   {\
      pcResponseTemp = pcResponse + EME_UTILS_STRLEN("-tfMail- ");\
      if ( NULL == EME_UTILS_StrStrNC(pcResponseTemp,"OK")\
         || NULL != EME_UTILS_StrStrNC(pcResponseTemp,"NO")\
         || NULL != EME_UTILS_StrStrNC(pcResponseTemp,"BAD"))\
      {\
         if (NULL != EME_UTILS_StrStrNC(pcResponseTemp,"[TRYCREATE]"))\
            iRet = EME_MAILBOX_NOT_EXISTS;\
         else\
            iRet = EME_SERVER_RESPONSE_ERR;\
         break;\
      }\
      else\
      {\
         iRet = EME_SUCCESS;\
         break;\
      }\
   }\
}\

/* only save untagged response data, it is prefix with "* " */
WE_INT32 EME_IMAP_SaveResponseData(struct St_IMAPHandle* pstIMAP, WE_CHAR* pcResponse);

/* save mail's data, include header(exclude bodystructure), whole mail, mail's part */
WE_INT32 EME_IMAP_SaveMailData(struct St_IMAPHandle* pstIMAP, WE_CHAR* pcResponse);

#endif /* __EME_IMAP_PARSER_H__ */