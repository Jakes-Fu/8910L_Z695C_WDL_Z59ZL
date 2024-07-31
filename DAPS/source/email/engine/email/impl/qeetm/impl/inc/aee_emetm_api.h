/*=====================================================================================
    FILE NAME   : aee_emetm_api.h
    MODULE NAME : Email engine
      

    GENERAL DESCRIPTION
      Supply api function for aeetm new and set virtual table.

    TechFaith Software Confidential Proprietary
    Copyright (c) 2007 by TechFaith Software. All Rights Reserved.
=======================================================================================
    Revision History

    Modification                   Tracking
    Date              Author       Number       Description of changes
    ----------   --------------   ---------   --------------------------------------
    2007-03-02     Jiang Dingqing                create aee_emetm_api.h

=====================================================================================*/
#ifndef _AEE_EMETM_API_H
#define _AEE_EMETM_API_H

/*---------------------------------------------------------------
*       Include Files
* ------------------------------------------------------------*/
/*Include System head file*/
 #include "eme_file_api.h"

#include "AEEEMETM.H"

/*Include Program Global head file*/
//#include "eme_tmlib_api.h"
#include "eme_utils_api.h"
#include "eme_tm_api.h"
#include "eme_callback.h"
/*Include module head file*/

/*---------------------------------------------------------------------
 *             Definitions and Constants 
 * ------------------------------------------------------------------*/
/*Define Constant Macro start*/
/*Define Constant Macro end*/


/*Define Macro Function start*/
/*Define Macro Function end*/

/*Define ENUM type start*/
/*Define ENUM type end*/
/*---------------------------------------------------------------------
 *  Here you may want to include extra documentation on data structures
 * ------------------------------------------------------------------*/
/*Define base type start*/
/*Define base type end*/

/*Define function type start*/
/*Define function type end*/

/*Define struct &  union type start*/
typedef struct EMETM
{
 	// Class member variables
   FILEMGR_HANDLE*piFileMgr;
   void*          pvCSC;
   
	St_EME_MemHandle* pMemHdr;
   void*          piShell;
	void*          piNetMgr;
	//transaction callback.
	AEECallback	m_TransCallback;
	// signal callback
	AEECallback m_SigCallback;
	ITMHandle pvTmHandle;

   AEECallback m_DequeueCallback;
   AEECallback m_DeleteCallback;

   WE_UINT32 tuiEmlTMState;
	
} EMETM;



 

/*---------------------------------------------------------------------
 *             Function Documentation
 * --------------------------------------------------------------------
 * 
 *    FUNCTION        EXAConform_InitIsComplete()
 * 
 *    DESCRIPTION     Initializes the EXAConform Services 	
 * 
 *    DEPENDENCIES    Must be called prior to any other EXAConform
 *                    function calls  
 * 
 *    PARAMETERS      None
 * 
 *    RETURN VALUE    boolean:  TRUE if initialization was successful
 *                               FALSE if initialization failed   
 *  	
 *    SIDE EFFECTS    None	
 * 
 *    ================================================================
 * 
 *    FUNCTION        EXAConform_Check()
 * 
 *    DESCRIPTION     Checks for the level of coding standard
 *                    conformity that is specified by input parameter	
 * 
 *    DEPENDENCIES    NONE 
 * 
 *    PARAMETERS      newCmd 
 * 
 *    RETURN VALUE    EXACmdStsType
 *   	
 *    SIDE EFFECTS    May wake up your boss
 * 
 * ------------------------------------------------------------------*/

/* Remember header files should contain all the information a user
 * would need to use this service */

#endif             /* #ifndef _AEE_EMETM_API_H */
