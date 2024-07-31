/*==============================================================================
    FILE NAME   : eme_msgpart_api.h
    MODULE NAME : Email Engine


    GENERAL DESCRIPTION:
        Define Some MsgOpts for operate multipart-message's part.

    TechFaith Software Confidential Proprietary
    Copyright (c) 2006-2007 by TechFaith Software. All Rights Reserved.
================================================================================
    Revision History

    Modification                  Tracking
       Date          Author        Number          Description of changes
    ----------   --------------   --------   ----------------------------------
    2007-09-21   Wenkai Xu        01         Create

==============================================================================*/
#ifndef EME_MSGPART_API_H
#define EME_MSGPART_API_H
/*------------------------------------------------------------------------------
 *    Include Files
 *----------------------------------------------------------------------------*/
#include "eme_msgopts_api.h"

/*------------------------------------------------------------------------------
 *    Definitions And Constants
 *----------------------------------------------------------------------------*/
typedef IWEMsgValue*  HMsgPart;

/*=========================MsgPart Opts=========================*/
#define EME_PARTOPT_PARTID             EMAILPART_OPT_PARTID
#define EME_PARTOPT_SRCTYPE            EMAILPART_OPT_SRC_TYPE

#define EME_PARTOPT_BUFFSZ             EMAILPART_OPT_DATA_SIZE
#define EME_PARTOPT_PARTTYPE           EMAILPART_OPT_PART_TYPE




/* Data type: str */
#define EME_PARTOPT_CHARSET            EMAILPARAM_OPT_CHARSET
#define EME_PARTOPT_TRANSENC           EMAILPART_OPT_TRANS_ENCODING
#define EME_PARTOPT_CONTENTID          EMAILPART_OPT_CONTENT_ID
#define EME_PARTOPT_CONTENTDISP        EMAILPART_OPT_CONTENT_DISP
#define EME_PARTOPT_DATABUF            EMAILPART_OPT_DATA_BUFF

#define EME_PARTOPT_ATTACHNAME         EMAILPARAM_OPT_FILE_NAME
#define EME_PARTOPT_SECTIONID          EMAILPART_OPT_SECTION_ID



/*=========================Opts Vals=========================*/
#define EMEPART_SRCTYPE_BUFFER         EMAILPART_SRCTYPE_BUFF  //Source is a string buffer
#define EMEPART_SRCTYPE_FILE           EMAILPART_SRCTYPE_FILE  //Source is locate file

typedef WE_UINT32 EMEPART_SRCTYPE;


/* Email message part types */
#define EMEPART_TYPE_TEXT_PLAIN           EMAILPART_PARTTYPE_TEXTPLAIN
#define EMEPART_TYPE_TEXT_HTML            EMAILPART_PARTTYPE_TEXTHTML
#define EMEPART_TYPE_ATTACHMENT           EMAILPART_PARTTYPE_ATTACHMENT

typedef WE_UINT32 EMEPART_TYPE;


/*------------------------------------------------------------------------------
 *    Function Declarations
 *----------------------------------------------------------------------------*/
static __inline WE_INT32 HMSGPART_AddOpt(HMsgPart hMsgPart, MsgOpt *amo)
{
   return HMSGOPTS_AddOpt((HMsgOpts)hMsgPart, amo);
}

static __inline WE_INT32 HMSGPART_GetOpt(HMsgPart hMsgPart, WE_INT32 nOptId, WE_INT32 nIndex, MsgOpt *pmo)
{
   return HMSGOPTS_GetOpt((HMsgOpts)hMsgPart, nOptId, nIndex, pmo);
}

static __inline WE_INT32 HMSGPART_UpdateOpt(HMsgPart hMsgPart, WE_INT32 nIndex, MsgOpt *pmo)
{
   return HMSGOPTS_UpdateOpt((HMsgOpts)hMsgPart, nIndex, pmo);
}

static __inline WE_INT32 HMSGPART_RemoveOpt(HMsgPart hMsgPart, WE_INT32 nOptId, WE_INT32 nIndex)
{
   return HMSGOPTS_RemoveOpt((HMsgOpts)hMsgPart, nOptId, nIndex);
}

static __inline WE_UINT32 HMSGPART_GetOptCount(HMsgPart hMsgPart)
{
   return HMSGOPTS_GetOptCount((HMsgOpts)hMsgPart);
}

static __inline HMsgPart HMSGPART_New(WE_VOID *pvMemMgr)
{
   return (HMsgPart)HMSGOPTS_New(pvMemMgr);
}

static __inline WE_INT32 HMSGPART_AddRef(HMsgPart hMsgPart)
{
   return HMSGOPTS_AddRef((HMsgOpts)hMsgPart);
}

static __inline WE_VOID HMSGPART_Delete(HMsgPart hMsgPart)
{
   HMSGOPTS_Delete((HMsgOpts)hMsgPart);
}

#endif /* EME_MSGPART_API_H */