/*==============================================================================
    FILE NAME   : eme_msgopts_api.h
    MODULE NAME : Email Engine


    GENERAL DESCRIPTION:
         HMsgOpts represents a stack or list of options, 
         which may be single- or multi-valued. Access to the options stack 
         is dictated by the order in which the options are added. 

    TechFaith Software Confidential Proprietary
    Copyright (c) 2006-2007 by TechFaith Software. All Rights Reserved.
================================================================================
    Revision History

    Modification                  Tracking
       Date          Author        Number          Description of changes
    ----------   --------------   --------   ----------------------------------
    2007-09-20   Wenkai Xu        01         Create
    2007-10-09   Wenkai Xu        02         Change some api's comment

==============================================================================*/
#ifndef EME_MSGOPTS_API_H
#define EME_MSGOPTS_API_H

/*------------------------------------------------------------------------------
 *    Include Files
 *----------------------------------------------------------------------------*/
#include "we_def.h"
#include "eme_error.h"
#include "we_msg.h"

/*------------------------------------------------------------------------------
 *    Definitions And Constants
 *----------------------------------------------------------------------------*/
typedef IWEMsgValue*  HMsgOpts;

/*
 * MsgOpt
 *	Holds an id, value pair for HMsgOpts
 */
typedef struct MsgOpt
{
   WE_INT32 nId;  /* option id, chosen from on of the MSGOPT_* defines in follow */
   WE_INT32 type;
   WE_INT32 size;
   WE_VOID  *pVal;/* option value, depends on nId */
} MsgOpt;


/*
 * MsgOpt option ids
 *
 * MsgOpt ids fall in ranges that are designed to keep the pVal data opaque 
 * while helping users deal with copying the data if necessary.
 */
/*
 * pVals are reserved, as are ids, up to 0x0ffff
 */
#define MSGOPT_RESERVED       WEMSG_OPT_RESERVED
#define MSGOPT_RESERVED_LAST  WEMSG_OPT_RESERVED_LAST
#define MSGOPT_END            WEMSG_OPT_END /* no pVal, ignore rest, used for terminating a vector of MsgOpts */
#define MSGOPT_ANY            WEMSG_OPT_ANY /* no pVal, matches any opt, used for enumeration */
#define MSGOPT_REMOVED        WEMSG_OPT_REMOVED /* not used */


/*
 * pVals are null-terminated strings, valid ids up to 0x01ffff
 */
#define MSGOPT_SZ             WEMSG_OPT_SZ
#define MSGOPT_SZ_LAST        WEMSG_OPT_SZ_LAST


/*
 * pVals are 32-bit quantities, no need to copy, valid ids up to 0x02ffff
 */
#define MSGOPT_32BIT          WEMSG_OPT_32BIT
#define MSGOPT_32BIT_LAST     WEMSG_OPT_32BIT_LAST


/*
 * pVals are pointers to opaque, but copyable structures, 
 * the first two bytes give the length of the following data.
 * The length is little-endian, low byte first.
 */
#define MSGOPT_VARBUFFER      WEMSG_OPT_VARBUFFER
#define MSGOPT_VARBUFFER_LAST WEMSG_OPT_VARBUFFER_LAST


/*
 * pVals are null-terminated wchar strings, valid ids up to 0x03ffff
 */
#define MSGOPT_WSZ            WEMSG_OPT_WSZ
#define MSGOPT_WSZ_LAST       WEMSG_OPT_WSZ_LAST

/*------------------------------------------------------------------------------
*    Function Declarations
*----------------------------------------------------------------------------*/
WE_INT32 HMSGOPTS_AddOpt(HMsgOpts hMsgOpts, MsgOpt *amo);
WE_INT32 HMSGOPTS_GetOpt(HMsgOpts hMsgOpts, WE_INT32 nOptId, WE_INT32 nIndex, MsgOpt *pmo);
WE_INT32 HMSGOPTS_UpdateOpt(HMsgOpts hMsgOpts, WE_INT32 nIndex, MsgOpt *pmo);
WE_INT32 HMSGOPTS_RemoveOpt(HMsgOpts hMsgOpts, WE_INT32 nOptId, WE_INT32 nIndex);
WE_UINT32 HMSGOPTS_GetOptCount(HMsgOpts hMsgOpts);

HMsgOpts HMSGOPTS_New(WE_VOID *pvMemMgr);
WE_INT32 HMSGOPTS_AddRef(HMsgOpts hMsgOpts);
WE_VOID HMSGOPTS_Delete(HMsgOpts hMsgOpts);

#define MSGOPT_GETVARBUFFERLENGTH(pbuf)   (WE_UINT16)(((WE_UCHAR *)(pbuf))[0] + (((WE_UCHAR *)(pbuf))[1] << 8))
#define MSGOPT_GETVARBUFFERDATA(pbuf)     (WE_VOID *)((WE_UCHAR *)(pbuf) + sizeof(WE_UINT16))
/*------------------------------------------------------------------------------
 *    Data Structure Documentation
 *----------------------------------------------------------------------------*/
/*==============================================================================
 *    Data Structure:      MsgOpt
 * 
 *    Description:   Holds an id, value pair for HMsgOpts.
 * 
 *    Definition:
 *       typedef struct MsgOpt
 *       {
 *          WE_INT32 nId;
 *          WE_VOID  *pVal;
 *       } MsgOpt;
 * 
 *    Members:
 *       nId: option id
 *
 *       pVal: option value, depends on nId
 *
 *    Comments:
 *       MsgOpt structures are the basic communication building block for
 *       representing email message.  The structures, when passed to AddOpt, 
 *       are not themselves copied, the contents are copied out
 * 
 *============================================================================*/

/*------------------------------------------------------------------------------
 *    Function Documentation
 *----------------------------------------------------------------------------*/
/*==============================================================================
 *    Function:      HMSGOPTS_AddOpt
 * 
 *    Description:   Adds the list of MsgOpts to the current message option set.
 * 
 *    Prototype:
 *       WE_INT32 HMSGOPTS_AddOpt(HMsgOpts hMsgOpts, MsgOpt *amo);
 * 
 *    Parameters:
 *       hMsgOpts [in]: The handler.
 *       amo      [in]: Array of MsgOpt structures, terminated by a MsgOpt with
 *                      nId set to MSGOPT_END.
 * 
 *    Return Value:
 *       EME_SUCCESS:   If the options could be added successfully
 *       EME_NOMEMORY:  If enough memory couldn't be allocated
 *  	   
 *    Comments:
 * 
 *============================================================================*/
/*==============================================================================
 *    Function:      HMSGOPTS_GetOpt
 * 
 *    Description:   Find a MsgOpt in the list of MsgOpts in hMsgOpts matching nOptId.  
 *                   There may be multiple matches (either in the case of a 
 *                   multi-valued MsgOpt, or in the case of overrides). 
 *                   In this case nIndex specifies how far into the list of 
 *                   MsgOpts that match nOptId to look. pmo is filled in 
 *                   with a the matching MsgOpt contents. nIndex is zero-based.
 * 
 *    Prototype:
 *       WE_INT32 HMSGOPTS_GetOpt(HMsgOpts hMsgOpts, WE_INT32 nOptId, WE_INT32 nIndex, MsgOpt *pmo);
 * 
 *    Parameters:
 *       hMsgOpts [in]: The handler.
 *       nOptId   [in]: Id of the option to find in the options list, MSGOPT_ANY 
 *                      returns the nIndexth option, if there is one...
 *       nIndex   [in]: Which (if there are multiple MsgOpts of the same id) to get
 *                      nIndex counts from most recently AddOpt()ed option, it is
 *                      zero-based.
 *       pmo     [out]: where to stick the found MsgOpt.
 * 
 *    Return Value:
 *       EME_SUCCESS:   If the options could be found successfully
 *       EME_FAILED:    If the specified MsgOpt couldn't be found
 *  	   
 *    Comments:
 * 
 *============================================================================*/
/*==============================================================================
 *    Function:      HMSGOPTS_UpdateOpt
 * 
 *    Description:   Update the nIndex'th MsgOpt matching pmo.
 * 
 *    Prototype:
 *       WE_INT32 HMSGOPTS_UpdateOpt(HMsgOpts hMsgOpts, WE_INT32 nIndex, MsgOpt *pmo);
 * 
 *    Parameters:
 *       hMsgOpts [in]: The handler.
 *       nIndex   [in]: Which (if there are multiple MsgOpts of the same id) to get
 *                      nIndex counts from most recently AddOpt()ed option, it is
 *                      zero-based.
 *       pmo      [in]: MsgOpt pointer to witch want to update.
 * 
 *    Return Value:
 *       EME_SUCCESS:   If the options could be update successfully
 *       EME_NOMEMORY:  If enough memory couldn't be allocated
 *       EME_FAILED:    If the specified MsgOpt couldn't be found
 *  	   
 *    Comments:
 * 
 *============================================================================*/
/*==============================================================================
 *    Function:      HMSGOPTS_RemoveOpt
 * 
 *    Description:   Remove the nIndex'th MsgOpt matching nOptId.
 * 
 *    Prototype:
 *       WE_INT32 HMSGOPTS_RemoveOpt(HMsgOpts hMsgOpts, WE_INT32 nOptId, WE_INT32 nIndex);
 * 
 *    Parameters:
 *       hMsgOpts [in]: The handler.
 *       nOptId   [in]: Id of the option to remove from the options list
 *       nIndex   [in]: Which (if there are multiple MsgOpts of the same id) to get
 *                      nIndex counts from most recently AddOpt()ed option, it is
 *                      zero-based.
 * 
 *    Return Value:
 *       EME_SUCCESS:   If the options could be removed successfully
 *       EME_FAILED:    If the specified MsgOpt couldn't be found
 *  	   
 *    Comments:
 * 
 *============================================================================*/
/*==============================================================================
 *    Function:      HMSGOPTS_GetOptCount
 * 
 *    Description:   Return the MsgOpts number in the HMsgOpts.
 * 
 *    Prototype:
 *       WE_UINT32 HMSGOPTS_GetOptCount(HMsgOpts hMsgOpts);
 * 
 *    Parameters:
 *       hMsgOpts [in]: The handler.
 * 
 *    Return Value:
 *       The MsgOpts number.
 * 
 *    Comments:
 * 
 *============================================================================*/

#endif /* EME_MSGOPTS_API_H */