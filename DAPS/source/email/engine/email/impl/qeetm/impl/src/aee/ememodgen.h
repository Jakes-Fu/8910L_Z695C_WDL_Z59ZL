/*=====================================================================================
    FILE NAME   : ememodgen.h
    MODULE NAME : Email engine
      
    GENERAL DESCRIPTION
      defines of email engine mod.

    TechFaith Software Confidential Proprietary
    Copyright (c) 2007 by TechFaith Software. All Rights Reserved.
=======================================================================================
    Revision History

    Modification                   Tracking
    Date              Author       Number       Description of changes
    ----------   --------------   ---------   --------------------------------------
    2007-03-02     Jiang Dingqing                create

=====================================================================================*/
#ifndef _EMEMODGEN_H_
#define _EMEMODGEN_H_
/*===============================================================================
    INCLUDES AND VARIABLE DEFINITIONS
===============================================================================*/

/*-------------------------------------------------------------------
      Include Files
-------------------------------------------------------------------*/
#include "AEE.h"			// Standard AEE definitions
#include "AEEShell.h"	// AEE Shell Services
#include "AEEEMETM.h"
#include "eme_utils_api.h"
/*-------------------------------------------------------------------
      Type Declarations
-------------------------------------------------------------------*/

//Address of IMODULE_CreateInstance() function
typedef int (*PFNMODCREATEINST)(AEECLSID, IShell*, IModule*, void**);

//Address of Freedata function for the module
typedef void (*PFNFREEMODDATA)(IModule*);


//Structure that implements the IModule interface
typedef struct _EMEMod                                     
{
	DECLARE_VTBL(IModule)	// Virtual Table with pointers to IModule functions
	
	uint32	      m_nRefs;			// Reference count for this module
   IShell *       m_pIShell;		// Pointer to IShell

	//Address of CreateInstance function of the module. This is needed for
	// static modules
	PFNMODCREATEINST	pfnModCrInst; 

	//Address of the function to free the module data. This is needed for static
	// modules that define their own data.
	PFNFREEMODDATA		pfnModFreeData;

   IFileMgr				*piFileMgr;
   IEMETMVtbl        AEEEMETMVtbl;

   St_EME_MemHandle  stMemHdr;
} EMEMod;

//-------------------------------------------------------------------
// Global Constant Declarations
//-------------------------------------------------------------------

//-------------------------------------------------------------------
// Global Data Declarations
//-------------------------------------------------------------------


//===============================================================================
// FUNCTION DECLARATIONS 
//===============================================================================


#ifndef  AEE_LOAD_DLL
#  if ((defined(AEE_SIMULATOR) || defined(WIN32)) && !defined(_AEEWIN)) || ( defined(_AEEWIN) && defined(_M_IX86))
#     define  AEE_LOAD_DLL
#  endif
#endif

// Prototype for the standard Module Load function. When a module is loaded,
// this is the first function that is invoked.
#ifdef AEE_LOAD_DLL

	// Need to prefix this function with __declspec(dllexport) so that this function
	// is exported from the DLL. 
 __declspec(dllexport) int  AEEMod_Load(IShell *ps, void * ph, IModule ** pMod);

#else

	int  AEEMod_Load(IShell *ps, void * ph, IModule ** pMod);

#endif   // AEE_LOAD_DLL

// Create instance function for dynamic modules
int AEEClsCreateInstance(AEECLSID ClsId, IShell *pIShell, IModule *pMod, void **ppobj);

//Module load function for static modules
int      AEEStaticMod_New(int16 nSize, IShell *pIShell, void * ph, IModule ** ppMod,PFNMODCREATEINST pfnMC,PFNFREEMODDATA pfnMF);



#endif  //_EMEMODGEN_H_


